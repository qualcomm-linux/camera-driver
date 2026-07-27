/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2014-2020, The Linux Foundation. All rights reserved.
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _CAM_COMPAT_H_
#define _CAM_COMPAT_H_

#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/component.h>
#include <linux/iommu.h>
#include <linux/list_sort.h>
#include <linux/spi/spi.h>
#include <linux/firmware/qcom/qcom_scm.h>
#include <linux/firmware.h>

#include "cam_csiphy_dev.h"
#include "cam_cpastop_hw.h"
#include "cam_smmu_api.h"
#include "cam_soc_info.h"

#if defined(USE_DOWNSTREAM_DMA_BUFF)
#include <linux/qcom-dma-mapping.h>
#endif
#include <linux/i3c/master.h>

#include <linux/interconnect.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 14, 0)
MODULE_IMPORT_NS("DMA_BUF");
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5, 18, 0)
MODULE_IMPORT_NS(DMA_BUF);
#endif

/*
 * <linux/of_gpio.h> and of_get_named_gpio() were removed in kernel 7.1-rc2.
 *
 * Provide cam_of_get_named_gpio(dev, np, propname, index) as a general
 * drop-in replacement that works on both old and new kernels:
 *
 *   kernel < 7.1 : delegates directly to of_get_named_gpio(np, propname, index)
 *
 *   kernel >= 7.1: reconstructs the integer GPIO number using only public APIs:
 *     1. of_parse_phandle_with_args_map(np, propname, "gpio", index, &args)
 *        mirrors what the kernel's own of_get_named_gpiod_flags() does:
 *        it resolves the DT property and follows any gpio-map remapping,
 *        returning the gpio-chip device_node and the hardware pin specifier.
 *     2. gpio_device_find_by_fwnode() locates the gpio_device for that node.
 *     3. gpio_device_get_chip() + gc->of_xlate() translates the DT specifier
 *        to the chip-local hardware offset, exactly as the kernel does.
 *     4. gpio_device_get_base() + hw_offset gives the Linux GPIO number.
 *     5. gpio_device_put() releases the reference from step 2.
 *
 * All files that previously included <linux/of_gpio.h> only for the header
 * (no API calls) simply drop that include; cam_compat.h guards it internally.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 1, 0)
#include <linux/gpio/consumer.h>
#include <linux/gpio/driver.h>
static inline int cam_of_get_named_gpio(struct device *dev,
					const struct device_node *np,
					const char *propname, int index)
{
	struct of_phandle_args args;
	struct gpio_device *gdev;
	struct gpio_chip *gc;
	int base, hw_offset, ret;

	/*
	* Use the _map variant so that gpio-map remapping (e.g. on PMIC GPIO
	* controllers) is handled identically to the kernel's own
	* of_get_named_gpiod_flags() path.
	*/
	ret = of_parse_phandle_with_args_map(np, propname, "gpio",
					     index, &args);
	if (ret)
		return ret;

	gdev = gpio_device_find_by_fwnode(of_fwnode_handle(args.np));
	of_node_put(args.np);
	if (!gdev)
		return -EPROBE_DEFER;

	/*
	* Translate the DT specifier to a chip-local hardware offset using
	* the chip's of_xlate callback (same as of_xlate_and_get_gpiod_flags
	* in gpiolib-of.c).  Fall back to args.args[0] for chips that have no
	* custom xlate (the default of_gpio_twocell_xlate also returns args[0]).
	*/
	gc = gpio_device_get_chip(gdev);

	/*
	* NOTE: gc->of_xlate is called without holding the gpio_device lock.
	* This is a best-effort translation; in practice the chip is stable
	* during probe. A fully race-free path would use gpiod_get_index().
	*/
	if (gc && gc->of_xlate)
		hw_offset = gc->of_xlate(gc, &args, NULL);
	else
		hw_offset = args.args[0];

	base = gpio_device_get_base(gdev);
	gpio_device_put(gdev);

	if (hw_offset < 0)
		return hw_offset;

	if (base < 0)
		return base;

	CAM_DBG(CAM_UTIL,
		"cam_of_get_named_gpio: base %d, hw_offset %d, total %d",
		base, hw_offset, base + hw_offset);

	return base + hw_offset;
}
#else
#include <linux/of_gpio.h>
static inline int cam_of_get_named_gpio(struct device *dev,
					const struct device_node *np,
					const char *propname, int index)
{
	return of_get_named_gpio(np, propname, index);
}
#endif

#include <linux/gpio.h>
static inline int cam_gpio_request_one(unsigned int gpio,
				       unsigned long flags, const char *label)
{
	int rc;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
	struct gpio_desc *desc;
	struct gpio_chip *gc;
	char *owner_label;
#endif

	rc = gpio_request_one(gpio, flags, label);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
	if (rc == -EBUSY) {
		/*
		 * On kernel >= 7.0, CONFIG_GPIO_SHARED causes gpiolib-shared.c
		 * to pre-claim any GPIO that appears in 2+ enabled DT nodes via
		 * gpiod_request_commit(desc, "shared") at chip registration time.
		 * This sets GPIOD_FLAG_REQUESTED before any driver runs, so the
		 * first gpio_request_one() returns -EBUSY even though no real
		 * driver owns the line.
		 *
		 * Verify the owner label is "shared" (the kernel shim's
		 * hardcoded label) before releasing, so we never steal a GPIO
		 * that is legitimately owned by another driver.
		 */
		desc = gpio_to_desc(gpio);
		gc   = desc ? gpiod_to_chip(desc) : NULL;
		owner_label = gc ?
			gpiochip_dup_line_label(gc, gpiod_hwgpio(desc)) : NULL;

		if (!IS_ERR_OR_NULL(owner_label) &&
		    strcmp(owner_label, "shared") == 0) {
			CAM_DBG(CAM_UTIL,
				"gpio %u pre-owned by kernel shim, taking ownership",
				gpio);
			kfree(owner_label);
			gpio_free(gpio);
			rc = gpio_request_one(gpio, flags, label);
		} else {
			kfree(owner_label);
		}
	}
#endif
	return rc;
}

#ifdef CONFIG_DOMAIN_ID_SECURE_CAMERA
#include <linux/IClientEnv.h>
#include <linux/ITrustedCameraDriver.h>
#include <linux/CTrustedCameraDriver.h>
#endif

#if IS_REACHABLE(CONFIG_INTERCONNECT_QCOM)
#include <linux/interconnect.h>
#endif

#if IS_REACHABLE(CONFIG_DMABUF_HEAPS)
#ifdef CONFIG_ARCH_QTI_VM
#include <linux/dma-heap.h>
#include <linux/qcom_tui_heap.h>
#endif
#endif

#if (KERNEL_VERSION(6, 7, 0) <= LINUX_VERSION_CODE)
#define CAM_SUBDEV_NAME_SIZE 32
#else
#define CAM_SUBDEV_NAME_SIZE V4L2_SUBDEV_NAME_SIZE
#endif

#define IS_CSF25(x, y) ((((x) == 2) && ((y) == 5)) ? 1 : 0)

struct cam_qcom_pas_context {
	struct device *dev;
	u32 pas_id;
	phys_addr_t mem_phys;
	size_t mem_size;
	void *ptr;
	dma_addr_t phys;
	ssize_t size;
	bool use_tzmem;
};

struct cam_fw_alloc_info {
	struct device *fw_dev;
	void          *fw_kva;
	uint64_t       fw_hdl;
};

/**
 * cam_timer_delete_sync_compat()
 * @brief : Compatibility helper to delete a timer synchronously.
 * @timer : Pointer to the timer_list structure to be deleted.
 */
static inline void cam_timer_delete_sync_compat(struct timer_list *timer)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 15, 0)
    timer_delete_sync(timer);
#else
    del_timer_sync(timer);
#endif
}

int cam_dma_fence_signal_locked(struct dma_fence *fence);
int cam_reserve_icp_fw(struct cam_fw_alloc_info *icp_fw, size_t fw_length);
/*
 * dma_fence_spinlock() was introduced in kernel 7.1-rc2 alongside the
 * struct dma_fence lock field refactor (spinlock_t *lock -> union of
 * extern_lock / inline_lock).  Provide the same helper for older kernels
 * so call sites can use dma_fence_spinlock(fence) unconditionally.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(7, 1, 0)
static inline spinlock_t *dma_fence_spinlock(struct dma_fence *fence)
{
	return fence->lock;
}
#endif

void cam_unreserve_icp_fw(struct cam_fw_alloc_info *icp_fw, size_t fw_length);
int camera_component_match_add_drivers(struct device *master_dev,
	struct component_match **match_list);
void cam_check_iommu_faults(struct iommu_domain *domain,
	struct cam_smmu_pf_info *pf_info);
void cam_free_clear(const void *);
int cam_compat_util_get_dmabuf_va(struct dma_buf *dmabuf, uintptr_t *vaddr);
void cam_compat_util_put_dmabuf_va(struct dma_buf *dmabuf, void *vaddr);
void cam_smmu_util_iommu_custom(struct device *dev,
	dma_addr_t discard_start, size_t discard_length);
int cam_get_ddr_type(void);
int cam_get_ddr_info(struct ddrinfo *ddr);

int cam_req_mgr_ordered_list_cmp(void *priv,
	const struct list_head *head_1, const struct list_head *head_2);

int cam_compat_util_get_irq(struct cam_hw_soc_info *soc_info);

long cam_dma_buf_set_name(struct dma_buf *dmabuf, const char *name);

const struct device *cam_cpas_get_rsc_dev_for_drv(uint32_t index);

int cam_cpas_start_drv_for_dev(const struct device *dev);

int cam_cpas_stop_drv_for_dev(const struct device *dev);

int cam_cpas_drv_channel_switch_for_dev(const struct device *dev);

inline struct icc_path *cam_icc_get_path(struct device *dev,
	const int src_id, const int dst_id, const char *path_name, bool use_path_name);

#ifndef QCOM_SCM_CAMERA_MAX_QOS_CNT
#define QCOM_SCM_CAMERA_MAX_QOS_CNT	2

struct qcom_scm_camera_qos {
	u32 offset;
	u32 val;
};

/*
 * Dummy function to override qcom_scm_camera_update_camnoc_qos
 * when this definition is missing in the kernel.
 */
static inline int qcom_scm_camera_update_camnoc_qos(uint32_t use_case_id,
	uint32_t cam_qos_cnt, struct qcom_scm_camera_qos *cam_qos)
{
	return 0;
}
#endif /* QCOM_SCM_CAMERA_MAX_QOS_CNT */

int cam_update_camnoc_qos_settings(uint32_t use_case_id,
	uint32_t num_arg, struct qcom_scm_camera_qos *scm_buf);

static inline void cam_qcom_pas_ctx_set_use_tzmem(void *ctx, bool enable)
{
	((struct cam_qcom_pas_context *)ctx)->use_tzmem = enable;
}

int camera_i2c_compare_dev(struct device *dev, const void *data);

#ifdef CONFIG_SPECTRA_SECURE
void cam_cpastop_scm_write(struct cam_cpas_hw_errata_wa *errata_wa);
int cam_ife_notify_safe_lut_scm(bool safe_trigger);
int cam_csiphy_notify_secure_mode(struct csiphy_device *csiphy_dev,
	bool protect, int32_t offset);
#endif /* CONFIG_SPECTRA_SECURE */

void qcom_clk_dump(struct clk *clk, void *regulator,
		   bool calltrace);

int cam_actuator_i2c_driver_remove_common(struct i2c_client *client);
int cam_eeprom_i2c_driver_remove_common(struct i2c_client *client);
int cam_flash_i2c_driver_remove_common(struct i2c_client *client);
int cam_ois_i2c_driver_remove_common(struct i2c_client *client);
int cam_sensor_i2c_driver_remove_common(struct i2c_client *client);
int cam_eeprom_spi_driver_remove_common(struct spi_device *sdev);

#if KERNEL_VERSION(6, 1, 0) <= LINUX_VERSION_CODE
void cam_i3c_driver_remove(struct i3c_device *client);
#else
int cam_i3c_driver_remove(struct i3c_device *client);
#endif

#if KERNEL_VERSION(6, 1, 0) <= LINUX_VERSION_CODE
void cam_actuator_i2c_driver_remove(struct i2c_client *client);
#else
static int32_t cam_actuator_i2c_driver_remove(struct i2c_client *client);
#endif

#if KERNEL_VERSION(6, 1, 0) <= LINUX_VERSION_CODE
void cam_eeprom_i2c_driver_remove(struct i2c_client *client);
#else
static int32_t cam_eeprom_i2c_driver_remove(struct i2c_client *client);
#endif

#if KERNEL_VERSION(6, 1, 0) <= LINUX_VERSION_CODE
void cam_flash_i2c_driver_remove(struct i2c_client *client);
#else
static int32_t cam_flash_i2c_driver_remove(struct i2c_client *client);
#endif

#if KERNEL_VERSION(6, 1, 0) <= LINUX_VERSION_CODE
void cam_ois_i2c_driver_remove(struct i2c_client *client);
#else
static int32_t cam_ois_i2c_driver_remove(struct i2c_client *client);
#endif

#if KERNEL_VERSION(6, 1, 0) <= LINUX_VERSION_CODE
void cam_sensor_i2c_driver_remove(struct i2c_client *client);
#else
static int32_t cam_sensor_i2c_driver_remove(struct i2c_client *client);
#endif

#if KERNEL_VERSION(5, 18, 0) <= LINUX_VERSION_CODE
void cam_eeprom_spi_driver_remove(struct spi_device *sdev);
#else
static int32_t cam_eeprom_spi_driver_remove(struct spi_device *sdev);
#endif

#ifndef CONFIG_SPECTRA_KT
int cam_smmu_fetch_csf_version(struct cam_csf_version *csf_version);
#endif /* ifndef CONFIG_SPECTRA_KT */

bool cam_secure_get_vfe_fd_port_config(void);
unsigned long cam_update_dma_map_attributes(unsigned long attr);
size_t cam_align_dma_buf_size(size_t len);
int cam_get_subpart_info(uint32_t *part_info, uint32_t max_num_cam);
#endif /* _CAM_COMPAT_H_ */
