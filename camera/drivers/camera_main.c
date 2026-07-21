// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2019-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */
#include <linux/module.h>
#include <linux/build_bug.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>

#include "cam_req_mgr_dev.h"
#include "cam_sync_api.h"
#include "cam_smmu_api.h"
#include "cam_cpas_hw_intf.h"
#include "cam_cdm_intf_api.h"

#include "cam_ife_csid_dev.h"
#include "cam_vfe.h"
#include "cam_sfe_dev.h"
#include "cam_isp_dev.h"

#include "cam_res_mgr_api.h"
#include "cam_cci_dev.h"
#include "cam_sensor_dev.h"
#include "cam_actuator_dev.h"
#include "cam_csiphy_dev.h"
#include "cam_eeprom_dev.h"
#include "cam_ois_dev.h"
#include "cam_tpg_dev.h"
#include "cam_flash_dev.h"

#include "cam_icp_v1_dev.h"
#include "cam_icp_v2_dev.h"
#include "ipe_core.h"
#include "bps_core.h"
#include "ofe_core.h"
#include "cam_icp_subdev.h"

#include "jpeg_dma_core.h"
#include "jpeg_enc_core.h"
#include "cam_jpeg_dev.h"

#include "cre_core.h"
#include "cam_cre_dev.h"

#include "cam_fd_hw_intf.h"
#include "cam_fd_dev.h"

#include "cam_lrme_hw_intf.h"
#include "cam_lrme_dev.h"

#include "cam_custom_dev.h"
#include "cam_custom_csid_dev.h"
#include "cam_custom_sub_mod_dev.h"

#include "cam_debug_util.h"

#include "ope_dev_intf.h"
#include "cre_dev_intf.h"

#include "cam_tfe_dev.h"
#include "cam_tfe_csid.h"
#include "cam_csid_ppi100.h"
#include "camera_main.h"

#include "cam_generated_h"

const char camera_banner[] = "Camera-Banner: (" CAMERA_COMPILE_HOST
	") (" CAMERA_COMPILE_TIME ") (" CAMERA_CC_VERSION ")";

#ifdef CONFIG_CAM_PRESIL
extern int cam_presil_framework_dev_init_from_main(void);
extern void cam_presil_framework_dev_exit_from_main(void);
#endif

struct camera_submodule_component {
	int (*init)(void);
	void (*exit)(void);
};

struct camera_submodule {
	char *name;
	uint num_component;
	const struct camera_submodule_component *component;
};

static const struct camera_submodule_component camera_base[] = {
    {.init = &cam_req_mgr_init, .exit = &cam_req_mgr_exit},
    {.init = &cam_sync_init, .exit = &cam_sync_exit},
    {.init = &cam_smmu_init_module, .exit = &cam_smmu_exit_module},
    {.init = &cam_cpas_dev_init_module, .exit = &cam_cpas_dev_exit_module},
    {.init = &cam_cdm_intf_init_module, .exit = &cam_cdm_intf_exit_module},
    {.init = &cam_hw_cdm_init_module, .exit = &cam_hw_cdm_exit_module},
};

static const struct camera_submodule_component camera_tfe[] = {
#ifdef CONFIG_SPECTRA_TFE
	{.init = &cam_csid_ppi100_init_module, .exit = &cam_csid_ppi100_exit_module},
	{.init = &cam_tfe_init_module, .exit = &cam_tfe_exit_module},
	{.init = &cam_tfe_csid_init_module, .exit = &cam_tfe_csid_exit_module},
#endif
};

static const struct camera_submodule_component camera_isp[] = {
#ifdef CONFIG_SPECTRA_ISP
	{.init = &cam_ife_csid_init_module, .exit = &cam_ife_csid_exit_module},
	{.init = &cam_ife_csid_lite_init_module, .exit = &cam_ife_csid_lite_exit_module},
	{.init = &cam_vfe_init_module, .exit = &cam_vfe_exit_module},
	{.init = &cam_sfe_init_module, .exit = &cam_sfe_exit_module},
	{.init = &cam_isp_dev_init_module, .exit = &cam_isp_dev_exit_module},
#endif
};

static const struct camera_submodule_component camera_sensor[] = {
#ifdef CONFIG_SPECTRA_SENSOR
	{.init = &cam_res_mgr_init, .exit = &cam_res_mgr_exit},
	{.init = &cam_cci_init_module, .exit = &cam_cci_exit_module},
	{.init = &cam_csiphy_init_module, .exit = &cam_csiphy_exit_module},
	{.init = &cam_tpg_init_module, .exit = &cam_tpg_exit_module},
	{.init = &cam_actuator_driver_init, .exit = &cam_actuator_driver_exit},
	{.init = &cam_sensor_driver_init, .exit = &cam_sensor_driver_exit},
	{.init = &cam_eeprom_driver_init, .exit = &cam_eeprom_driver_exit},
	{.init = &cam_ois_driver_init, .exit = &cam_ois_driver_exit},
	{.init = &cam_flash_init_module, .exit = &cam_flash_exit_module},
#endif
};

static const struct camera_submodule_component camera_icp[] = {
#ifdef CONFIG_SPECTRA_ICP
	{.init = &cam_icp_v1_init_module, .exit = &cam_icp_v1_exit_module},
	{.init = &cam_icp_v2_init_module, .exit = &cam_icp_v2_exit_module},
	{.init = &cam_ipe_init_module, .exit = &cam_ipe_exit_module},
	{.init = &cam_bps_init_module, .exit = &cam_bps_exit_module},
	{.init = &cam_ofe_init_module, .exit = &cam_ofe_exit_module},
	{.init = &cam_icp_init_module, .exit = &cam_icp_exit_module},
#endif
};

static const struct camera_submodule_component camera_ope[] = {
#ifdef CONFIG_SPECTRA_OPE
	{.init = &cam_ope_init_module, .exit = &cam_ope_exit_module},
	{.init = &cam_ope_subdev_init_module, .exit = &cam_ope_subdev_exit_module},
#endif
};

static const struct camera_submodule_component camera_cre[] = {
#ifdef CONFIG_SPECTRA_CRE
	{.init = &cam_cre_init_module, .exit = &cam_cre_exit_module},
	{.init = &cam_cre_subdev_init_module, .exit = &cam_cre_subdev_exit_module},
#endif
};
static const struct camera_submodule_component camera_jpeg[] = {
#ifdef CONFIG_SPECTRA_JPEG
	{.init = &cam_jpeg_enc_init_module, .exit = &cam_jpeg_enc_exit_module},
	{.init = &cam_jpeg_dma_init_module, .exit = &cam_jpeg_dma_exit_module},
	{.init = &cam_jpeg_dev_init_module, .exit = &cam_jpeg_dev_exit_module},
#endif
};

static const struct camera_submodule_component camera_fd[] = {
#ifdef CONFIG_SPECTRA_FD
	{.init = &cam_fd_hw_init_module, .exit = &cam_fd_hw_exit_module},
	{.init = &cam_fd_dev_init_module, .exit = &cam_fd_dev_exit_module},
#endif
};

static const struct camera_submodule_component camera_lrme[] = {
#ifdef CONFIG_SPECTRA_LRME
	{.init = &cam_lrme_hw_init_module, .exit = &cam_lrme_hw_exit_module},
	{.init = &cam_lrme_dev_init_module, .exit = &cam_lrme_dev_exit_module},
#endif
};

static const struct camera_submodule_component camera_custom[] = {
#ifdef CONFIG_SPECTRA_CUSTOM
	{.init = &cam_custom_hw_sub_module_init, .exit = &cam_custom_hw_sub_module_exit},
	{.init = &cam_custom_csid_driver_init, .exit = &cam_custom_csid_driver_exit},
	{.init = &cam_custom_dev_init_module, .exit = &cam_custom_dev_exit_module},
#endif
};

static const struct camera_submodule_component camera_presil[] = {
#ifdef CONFIG_CAM_PRESIL
	{.init = &cam_presil_framework_dev_init_from_main,
	 .exit = &cam_presil_framework_dev_exit_from_main},
#endif
};

static const struct camera_submodule submodule_table[] = {
	{
		.name = "Camera BASE",
		.num_component = ARRAY_SIZE(camera_base),
		.component = camera_base,
	},
	{
		.name = "Camera TFE",
		.num_component = ARRAY_SIZE(camera_tfe),
		.component = camera_tfe,
	},
	{
		.name = "Camera ISP",
		.num_component = ARRAY_SIZE(camera_isp),
		.component = camera_isp,
	},
	{
		.name = "Camera SENSOR",
		.num_component = ARRAY_SIZE(camera_sensor),
		.component = camera_sensor
	},
	{
		.name = "Camera ICP",
		.num_component = ARRAY_SIZE(camera_icp),
		.component = camera_icp,
	},
	{
		.name = "Camera OPE",
		.num_component = ARRAY_SIZE(camera_ope),
		.component = camera_ope,
	},
	{
		.name = "Camera JPEG",
		.num_component = ARRAY_SIZE(camera_jpeg),
		.component = camera_jpeg,
	},
	{
		.name = "Camera FD",
		.num_component = ARRAY_SIZE(camera_fd),
		.component = camera_fd,
	},
	{
		.name = "Camera LRME",
		.num_component = ARRAY_SIZE(camera_lrme),
		.component = camera_lrme,
	},
	{
		.name = "Camera CRE",
		.num_component = ARRAY_SIZE(camera_cre),
		.component = camera_cre,
	},
	{
		.name = "Camera CUSTOM",
		.num_component = ARRAY_SIZE(camera_custom),
		.component = camera_custom,
	},
	{
		.name = "Camera Presil",
		.num_component = ARRAY_SIZE(camera_presil),
		.component = camera_presil,
	}
};

static int camera_verify_submodules(void)
{
	int rc = 0;
	int i, j, num_components;

	for (i = 0; i < ARRAY_SIZE(submodule_table); i++) {
		num_components = submodule_table[i].num_component;
		for (j = 0; j < num_components; j++) {
			if (!submodule_table[i].component[j].init ||
				!submodule_table[i].component[j].exit) {
				CAM_ERR(CAM_UTIL,
					"%s module has init = %ps, exit = %ps",
					submodule_table[i].name,
					submodule_table[i].component[j].init,
					submodule_table[i].component[j].exit);
				rc = -EINVAL;
				goto end;
			}
		}
	}

end:
	return rc;
}

static void __camera_exit(int i, int j)
{
	uint num_exits;

	/* Exit from current submodule */
	for (j -= 1; j >= 0; j--)
		submodule_table[i].component[j].exit();

	/* Exit remaining submodules */
	for (i -= 1; i >= 0; i--) {
		num_exits = submodule_table[i].num_component;
		for (j = num_exits - 1; j >= 0; j--)
			submodule_table[i].component[j].exit();
	}
}

static const struct of_device_id of_camera_bus_match_table[] = {
	{ .compatible = "simple-bus", },
	{} /* Empty terminated list */
};

static int cam_main_probe(struct platform_device *pdev)
{
	int rc;
	uint i, j, num_inits;
	struct device_node *node = pdev->dev.of_node;

	CAM_INFO(CAM_UTIL, "%s", camera_banner);
	rc = camera_verify_submodules();
	if (rc)
		goto end_init;

	cam_debugfs_init();

	/* For Probing all available submodules */
	for (i = 0; i < ARRAY_SIZE(submodule_table); i++) {
		num_inits = submodule_table[i].num_component;
		CAM_DBG(CAM_UTIL, "Number of %s components: %u",
			submodule_table[i].name, num_inits);
		for (j = 0; j < num_inits; j++) {
			rc = submodule_table[i].component[j].init();
			if (rc) {
				CAM_ERR(CAM_UTIL,
					"%s module failure at %ps rc = %d",
					submodule_table[i].name,
					submodule_table[i].component[j].init,
					rc);
				__camera_exit(i, j);
				goto end_init;
			}
		}
	}

	/* Populate all the parrent + child nodes here... */
	rc = of_platform_populate(node, of_camera_bus_match_table, NULL, &pdev->dev);
	if (rc) {
		CAM_ERR(CAM_UTIL,"Camera main populate failed %d", rc);
		goto end_init;
	}
	CAM_INFO(CAM_UTIL, "Spectra camera driver initialized rc : %d", rc);


end_init:
	return rc;
}

static void cam_main_remove(struct platform_device *pdev)
{
	__camera_exit(ARRAY_SIZE(submodule_table), 0);
	cam_debugfs_deinit();
	CAM_INFO(CAM_UTIL, "Spectra camera driver exited!");
}

static const struct of_device_id camera_main_dt_match[] = {
#if defined(CONFIG_QCS9100) || defined(CONFIG_QCS8300)
	{.compatible = "qcom,camera"},
#endif
#if defined(CONFIG_QCS615)
	{.compatible = "qcom,camera_qcs615"},
#endif
#if defined(CONFIG_X1E80100)
	{.compatible = "qcom,camera_x1e80100"},
#endif
#if defined(CONFIG_SHIKRA)
	{.compatible = "qcom,camera_shikra"},
#endif
	{}
};

static struct platform_driver camera_main_driver = {
	.probe = cam_main_probe,
	.remove = cam_main_remove,
	.driver = {
		.name = "camera_main",
		.owner = THIS_MODULE,
		.of_match_table = camera_main_dt_match,
	},
};

static int camera_init(void)
{
	struct device_node *node;
	const struct of_device_id *match;
	int rc = -ENODEV;

	for_each_matching_node(node, camera_main_dt_match) {
		match = of_match_node(camera_main_dt_match, node);
		if (match) {
			rc = platform_driver_register(&camera_main_driver);
			if (rc < 0) {
				CAM_ERR(CAM_UTIL, "Platform register failed = %d for camera", rc);
			}
			break;
		}
	}

	if (rc == -ENODEV) {
		CAM_INFO(CAM_UTIL, "No matching device found for camera driver = %d", rc);
	}
	return rc;
}

static void camera_exit(void)
{
	platform_driver_unregister(&camera_main_driver);
}

module_init(camera_init);
module_exit(camera_exit);

MODULE_DESCRIPTION("Spectra camera driver");
MODULE_LICENSE("GPL v2");
