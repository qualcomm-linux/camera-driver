// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2017-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include "cam_cci_dev.h"
#include "cam_req_mgr_dev.h"
#include "cam_sensor_util.h"
#include "cam_cci_soc.h"
#include "cam_cci_core.h"
#include "camera_main.h"
#include "uapi/linux/sched/types.h"
#include "linux/sched/types.h"
#include "linux/sched.h"

#define CCI_MAX_DELAY 1000000
#define QUEUE_SIZE 100
#define CCI_CPAS_GPIO_MUX1_SHIFT 10

struct cci_irq_data {
	int32_t  is_valid;
	uint32_t irq_status0;
	uint32_t irq_status1;
	enum cci_i2c_master_t master;
	enum cci_i2c_queue_t queue;
};

static struct v4l2_subdev *g_cci_subdev[MAX_CCI] = { 0 };
static struct dentry *debugfs_root;
static struct cci_irq_data cci_irq_queue[QUEUE_SIZE] = { 0 };
static int32_t head;
static int32_t tail;


/*
 * Hardware descriptor instances — one per compatible string.
 * All register offsets are relative to the CAMERA_TOP (CPAS TOP) base,
 * which is the base used by cam_cpas_reg_write(CAM_CPAS_REGBASE_CPASTOP).
 *
 * nordschleife_2.0 offsets (from IPCAT SWI):
 *   CAMERA_TOP base : 0x09a01000
 *   mux_offset      = 0x00ec  (CCU_O_MUX_SEL in CAMERA_TOP)
 */
static const struct cam_cci_hw_info cam_cci_hw_info_standalone = {
	.variant         = CAM_CCI_HW_STANDALONE,
	.max_masters     = 2,
	.mux_offset      = 0,
};

static const struct cam_cci_hw_info cam_cci_hw_info_ccu_legacy = {
	.variant         = CAM_CCI_HW_CCU_LEGACY,
	.max_masters     = 2,
	.mux_offset      = 0xec,
};

static const struct cam_cci_hw_info cam_cci_hw_info_ccu_qup = {
	.variant         = CAM_CCI_HW_CCU_QUP,
	.max_masters     = 5,     /* 5 Serial Engines per QUP instance */
	.mux_offset      = 0xec,
};


static inline int32_t increment_index(int32_t index)
{
	return (index + 1) % QUEUE_SIZE;
}

struct v4l2_subdev *cam_cci_get_subdev(int cci_dev_index)
{
	struct v4l2_subdev *sub_device = NULL;

	if ((cci_dev_index < MAX_CCI) && (g_cci_subdev[cci_dev_index] != NULL))
		sub_device = g_cci_subdev[cci_dev_index];
	else
		CAM_WARN(CAM_CCI, "CCI subdev not available at Index: %u, MAX_CCI : %u",
			cci_dev_index, MAX_CCI);

	return sub_device;
}


/* Helper: GPIO command type -> human-readable string */
#define CCI_POLL_TIMEOUT_US  50000
#define CCI_POLL_INTERVAL_US 5

static int __cci_find_free_gpio_queue(struct cci_device *cci_dev, int *queue)
{
	int i;

	for (i = 0; i < GPIO_Q_MAX; i++) {
		if (!cci_dev->gpio_queue[i].is_acquired) {
			cci_dev->gpio_queue[i].is_acquired = true;
			*queue = i;
			CAM_DBG(CAM_CCI, "GPIO queue %d acquired", i);
			return 0;
		}
	}

	CAM_ERR(CAM_CCI, "No free GPIO queue available");
	return -EBUSY;
}

static int __cci_configure_cpas(struct cci_device *cci_dev,
	struct cam_cci_ctrl *c_ctrl)
{
	int rc = 0;
	uint32_t cpas_handle = cci_dev->cpas_handle;
	uint32_t top_mux;
	uint32_t second_level_mux;
	uint32_t val;
	uint32_t cpas_mux_val = 0;
	int cci_index = cci_dev->soc_info.index;
	int cmd_type = c_ctrl->cmd;
	int cci_timer_index;

	rc = cam_cpas_reg_read(cpas_handle, CAM_CPAS_REGBASE_CPASTOP,
		CCI_GPIO_CPAS_MUX_EN, true, &val);
	if (rc) {
		CAM_ERR(CAM_CCI, "CPAS reg read failed rc=%d", rc);
		return rc;
	}

	CAM_INFO(CAM_CCI, "BEFORE WRITE CPAS VALUE: 0x%x", val);

	/*
	 * TODO (fsync redesign item 3): compute timer mask dynamically
	 * by walking all registered cci_clients on cci_dev and OR-ing
	 * their timer_mask values. For now use -1 (enables all timers)
	 * which matches the previous behaviour.
	 */
	cci_timer_index = -1;

	if (cmd_type == MSM_CCI_TIMER_FSYNC_INFINITE ||
	    cmd_type == MSM_CCI_TIMER_FSYNC_INDEPENDENT) {
		switch (cci_index) {
		case 0:
			/*
			* CCI_0: assert MUX_EN[4:0] to select CCI_0 over CCI_1
			*
			* GPIO bits 0-4 (cci_timer_index <= CCI_TIMER4):
			*   MUX1_EN[4:0] = 1 → route MUX_EN[4:0] path → CCI_0
			*   e.g. Timer2 → GPIO bit 2: reg = 0x000FFFFF
			*
			* GPIO bits 5-9 (cci_timer_index > CCI_TIMER4):
			*   MUX1_EN[9:5] = 0 → route MUX_EN[4:0] path → CCI_0
			*   e.g. Timer7 → GPIO bit 7: reg = 0x000DFFFF
			*/
			CAM_INFO(CAM_CCI,
				"cci: %d is about to enable all cci timers",
				cci_index);

			/* MUX_EN[4:0] = 1 → select CCI_0 */
			top_mux = BIT(CCI_TIMER0) | BIT(CCI_TIMER1) |
					BIT(CCI_TIMER2) | BIT(CCI_TIMER3) |
					BIT(CCI_TIMER4);

			if (cci_timer_index <= CCI_TIMER4) {
				/* MUX1_EN[4:0] = 1 → enable MUX_EN[4:0] path for GPIO bits 0-4 */
				second_level_mux = BIT(CCI_TIMER0) | BIT(CCI_TIMER1) |
								BIT(CCI_TIMER2) | BIT(CCI_TIMER3) |
								BIT(CCI_TIMER4);
			} else {
				/* MUX1_EN[9:5] = 0 → enable MUX_EN[4:0] path for GPIO bits 5-9 */
				second_level_mux = 0x00;
			}
			second_level_mux <<= CCI_CPAS_GPIO_MUX1_SHIFT;
			cpas_mux_val = top_mux | second_level_mux;
			break;

		case 1:
			/*
			* CCI_1: clear MUX_EN[4:0] to select CCI_1 over CCI_0
			*
			* GPIO bits 0-4 (cci_timer_index <= CCI_TIMER4):
			*   MUX1_EN[4:0] = 1 → route MUX_EN[4:0]=0 path → CCI_1
			*   e.g. Timer2 → GPIO bit 2: reg = 0x000FFFFB
			*
			* GPIO bits 5-9 (cci_timer_index > CCI_TIMER4):
			*   MUX1_EN[9:5] = 0 → route MUX_EN[4:0]=0 path → CCI_1
			*   e.g. Timer7 → GPIO bit 7: reg = 0x000DFFFB
			*/
			CAM_INFO(CAM_CCI,
				"cci: %d is about to enable all cci timers",
				cci_index);

			/* MUX_EN[4:0] = 0 → select CCI_1 */
			top_mux = 0x00;

			if (cci_timer_index <= CCI_TIMER4) {
				/* MUX1_EN[4:0] = 1 → enable MUX_EN[4:0] path for GPIO bits 0-4 */
				second_level_mux = BIT(CCI_TIMER0) | BIT(CCI_TIMER1) |
								BIT(CCI_TIMER2) | BIT(CCI_TIMER3) |
								BIT(CCI_TIMER4);
			} else {
				/* MUX1_EN[9:5] = 0 → enable MUX_EN[4:0] path for GPIO bits 5-9 */
				second_level_mux = 0x00;
			}
			second_level_mux <<= CCI_CPAS_GPIO_MUX1_SHIFT;
			cpas_mux_val = top_mux | second_level_mux;
			break;

		case 2:
			/*
			* CCI_2: assert MUX_EN[9:5] to select CCI_2 over CCI_3
			*
			* GPIO bits 0-4 (cci_timer_index <= CCI_TIMER4):
			*   MUX1_EN[4:0] = 0 → route MUX_EN[9:5] path → CCI_2
			*   e.g. Timer2 → GPIO bit 2: reg = 0x000FEFFF  ⬅ your target
			*
			* GPIO bits 5-9 (cci_timer_index > CCI_TIMER4):
			*   MUX1_EN[9:5] = 1 → route MUX_EN[9:5] path → CCI_2
			*   e.g. Timer7 → GPIO bit 7: reg = 0x000FFFFF
			*/
			CAM_INFO(CAM_CCI,
				"cci: %d is about to enable all cci timers",
				cci_index);

			/* MUX_EN[9:5] = 1 → select CCI_2 */
			top_mux = BIT(CCI_TIMER5) | BIT(CCI_TIMER6) |
					BIT(CCI_TIMER7) | BIT(CCI_TIMER8) |
					BIT(CCI_TIMER9);

			if (cci_timer_index <= CCI_TIMER4) {
				/* MUX1_EN[4:0] = 0 → enable MUX_EN[9:5] path for GPIO bits 0-4 */
				second_level_mux = 0x00;
			} else {
				/* MUX1_EN[9:5] = 1 → enable MUX_EN[9:5] path for GPIO bits 5-9 */
				second_level_mux = BIT(CCI_TIMER5) | BIT(CCI_TIMER6) |
								BIT(CCI_TIMER7) | BIT(CCI_TIMER8) |
								BIT(CCI_TIMER9);
			}
			second_level_mux <<= CCI_CPAS_GPIO_MUX1_SHIFT;
			cpas_mux_val = top_mux | second_level_mux;
			break;

		case 3:
			/*
			* CCI_3: clear MUX_EN[9:5] to select CCI_3 over CCI_2
			*
			* GPIO bits 0-4 (cci_timer_index <= CCI_TIMER4):
			*   MUX1_EN[4:0] = 0 → route MUX_EN[9:5]=0 path → CCI_3
			*   e.g. Timer2 → GPIO bit 2: reg = 0x000FEF7F
			*
			* GPIO bits 5-9 (cci_timer_index > CCI_TIMER4):
			*   MUX1_EN[9:5] = 1 → route MUX_EN[9:5]=0 path → CCI_3
			*   e.g. Timer7 → GPIO bit 7: reg = 0x000FFF7F
			*/
			CAM_INFO(CAM_CCI,
				"cci: %d is about to enable all cci timers",
				cci_index);

			/* MUX_EN[9:5] = 0 → select CCI_3 */
			top_mux = 0x00;

			if (cci_timer_index <= CCI_TIMER4) {
				/* MUX1_EN[4:0] = 0 → enable MUX_EN[9:5] path for GPIO bits 0-4 */
				second_level_mux = 0x00;
			} else {
				/* MUX1_EN[9:5] = 1 → enable MUX_EN[9:5] path for GPIO bits 5-9 */
				second_level_mux = BIT(CCI_TIMER5) | BIT(CCI_TIMER6) |
								BIT(CCI_TIMER7) | BIT(CCI_TIMER8) |
								BIT(CCI_TIMER9);
			}
			second_level_mux <<= CCI_CPAS_GPIO_MUX1_SHIFT;
			cpas_mux_val = top_mux | second_level_mux;
			break;

		default:
			CAM_ERR(CAM_CCI, "cci_index is not valid: %d", cci_index);
			return -EINVAL;
		}
	}

	CAM_INFO(CAM_CCI, "CPAS_MUX_VAL: 0x%x", cpas_mux_val);
	rc = cam_cpas_reg_write(cpas_handle, CAM_CPAS_REGBASE_CPASTOP,
		CCI_GPIO_CPAS_MUX_EN, true, cpas_mux_val);
	if (rc) {
		CAM_ERR(CAM_CCI, "CPAS reg write failed rc=%d", rc);
		return rc;
	}

	rc = cam_cpas_reg_read(cpas_handle, CAM_CPAS_REGBASE_CPASTOP,
		CCI_GPIO_CPAS_MUX_EN, true, &val);
	if (rc) {
		CAM_ERR(CAM_CCI, "CPAS reg read failed rc=%d", rc);
		return rc;
	}

	CAM_INFO(CAM_CCI, "POST WRITE CPAS VALUE: 0x%x", val);
	return rc;
}

static int cam_cci_poll_gpio_queue_empty(void __iomem *base, int queue)
{
	uint32_t wordcount;
	int32_t timeout = CCI_POLL_TIMEOUT_US;

	while (timeout > 0) {
		wordcount = cam_io_r_mb(base +
			(CCI_GPIO_WORD_COUNT_ADDR + (0x100 * queue)));
		CAM_DBG(CAM_CCI, "GPIO queue word count: %u", wordcount);

		if (wordcount == 0) {
			CAM_DBG(CAM_CCI, "GPIO Q[%d]! Time passed: %d us",
				queue, (CCI_POLL_TIMEOUT_US - timeout));
			return 0;
		}

		usleep_range(CCI_POLL_INTERVAL_US, CCI_POLL_INTERVAL_US + 10);
		timeout -= CCI_POLL_INTERVAL_US;
	}

	CAM_ERR(CAM_CCI, "Timeout polling GPIO Q[%d]! Remaining words: %d",
		queue, wordcount);

	return -ETIMEDOUT;
}

static int cam_cci_load_gpio_queue(struct v4l2_subdev *sd,
	struct cam_cci_ctrl *c_ctrl,
	const struct cam_cci_gpio_cmd_buf *cmd_buf)
{
	int rc = 0;
	uint32_t reg_addr;
	struct cam_hw_soc_info *soc_info = NULL;
	void __iomem *base = NULL;
	uint32_t wordcount = 0;
	int queue = 0;
	int queue_size = 0;
	struct cci_device *cci_dev;

	cci_dev = v4l2_get_subdevdata(sd);
	if (!cci_dev || !c_ctrl || !cmd_buf) {
		CAM_ERR(CAM_CCI,
			"Invalid params cci_dev:%pK c_ctrl:%pK cmd_buf:%pK",
			cci_dev, c_ctrl, cmd_buf);
		return -EINVAL;
	}

	if (!cmd_buf->cmd_buf_ready || cmd_buf->cmd_count == 0) {
		CAM_ERR(CAM_CCI, "cmd_buf not ready or empty: ready=%d count=%d",
			cmd_buf->cmd_buf_ready, cmd_buf->cmd_count);
		return -EINVAL;
	}

	mutex_lock(&cci_dev->init_mutex);
	rc = __cci_find_free_gpio_queue(cci_dev, &queue);
	if (rc) {
		CAM_ERR(CAM_CCI, "No free GPIO queue");
		mutex_unlock(&cci_dev->init_mutex);
		return rc;
	}
	c_ctrl->cci_info->acquired_gpio_queue = queue;

	queue_size = (queue == GPIO_Q0 ?
		CCI_GPIO_Q0_MAX_WORD_COUNT : CCI_GPIO_Q1_MAX_WORD_COUNT);

	rc = __cci_configure_cpas(cci_dev, c_ctrl);
	if (rc) {
		CAM_ERR(CAM_CCI, "CPAS Mux configuration failed");
		goto release_queue;
	}

	if (queue_size < cmd_buf->cmd_count) {
		CAM_ERR(CAM_CCI,
			"cmd_buf (%u cmds) exceeds queue %d capacity (%d words)",
			cmd_buf->cmd_count, queue, queue_size);
		rc = -EINVAL;
		goto release_queue;
	}

	soc_info = &cci_dev->soc_info;
	base = soc_info->reg_map[0].mem_base;
	reg_addr = CCI_GPIO_QUEUE_LOAD_ADDR(queue);

	CAM_DBG(CAM_CCI, "Loading %u cmds into GPIO queue %d (reg=0x%x)",
		cmd_buf->cmd_count, queue, reg_addr);

	rc = cam_cci_write_gpio_cmd_buf(base, reg_addr, cmd_buf);
	if (rc) {
		CAM_ERR(CAM_CCI, "Failed writing cmd_buf to GPIO queue %d: %d",
			queue, rc);
		goto release_queue;
	}

	wordcount = cam_io_r_mb(base +
		(CCI_GPIO_WORD_COUNT_ADDR + (0x100 * queue)));
	cam_io_w_mb(wordcount, base +
		(CCI_GPIO_EXECUTE_WC_ADDR + (0x100 * queue)));

	mutex_unlock(&cci_dev->init_mutex);
	return 0;

release_queue:
	cci_dev->gpio_queue[queue].is_acquired = false;
	c_ctrl->cci_info->acquired_gpio_queue = -1;
	mutex_unlock(&cci_dev->init_mutex);
	return rc;
}

static int __cci_gpio_queue_start(struct v4l2_subdev *sd,
	struct cam_cci_ctrl *c_ctrl)
{
	int rc = 0;
	struct cam_hw_soc_info *soc_info = NULL;
	void __iomem *base = NULL;
	struct cci_device *cci_dev = NULL;
	uint32_t val, wordcount;
	bool is_infinite_mode = false;

	cci_dev = v4l2_get_subdevdata(sd);
	if (!cci_dev || !c_ctrl) {
		CAM_ERR(CAM_CCI,
			"Failed: invalid params cci_dev:%pK, c_ctrl:%pK",
			cci_dev, c_ctrl);
		return -EINVAL;
	}

	soc_info = &cci_dev->soc_info;
	base = soc_info->reg_map[0].mem_base;

	wordcount = cam_io_r_mb(base + (CCI_GPIO_WORD_COUNT_ADDR +
		(0x100 * (c_ctrl->cci_info->acquired_gpio_queue))));
	if (wordcount == 0) {
		CAM_DBG(CAM_CCI, "Queue is empty, nothing to be done!");
		return 0;
	}

	val = 0x10 << (c_ctrl->cci_info->acquired_gpio_queue);
	cam_io_w_mb(val, base + CCI_QUEUE_START_ADDR);

	/*
	 * A queue built for infinite frequency mode ends in
	 * CCI_GPIO_CONTINUE_CMD and loops indefinitely by design, so it
	 * never drains -- polling for empty would time out. Detect this
	 * from the last command in the buffer just loaded and skip the
	 * poll only in that case; every other mode still drains and polls
	 * as before.
	 */
	if (c_ctrl->cci_info->cmd_buf.cmd_count > 0) {
		uint32_t last_cmd = c_ctrl->cci_info->cmd_buf.buf[
			c_ctrl->cci_info->cmd_buf.cmd_count - 1] & 0xF;

		is_infinite_mode = (last_cmd == CCI_GPIO_CONTINUE_CMD);
	}

	if (is_infinite_mode) {
		CAM_DBG(CAM_CCI,
			"Infinite mode GPIO queue %d started, skipping drain poll",
			c_ctrl->cci_info->acquired_gpio_queue);
		return 0;
	}

	rc = cam_cci_poll_gpio_queue_empty(base,
		c_ctrl->cci_info->acquired_gpio_queue);
	if (rc)
		CAM_ERR(CAM_CCI, "GPIO queue drain timeout: %d", rc);

	return rc;
}

static int __cci_halt_gpio_queue(struct v4l2_subdev *sd,
	struct cam_cci_ctrl *c_ctrl)
{
	int rc = 0;
	struct cam_hw_soc_info *soc_info = NULL;
	void __iomem *base = NULL;
	uint32_t val = 0x00;
	struct cci_device *cci_dev = NULL;
	int queue = -1;

	cci_dev = v4l2_get_subdevdata(sd);
	if (!cci_dev || !c_ctrl) {
		CAM_ERR(CAM_CCI,
			"Failed: invalid params cci_dev:%pK, c_ctrl:%pK",
			cci_dev, c_ctrl);
		return -EINVAL;
	}

	soc_info = &cci_dev->soc_info;
	base = soc_info->reg_map[0].mem_base;

	val |= 1 << (CCI_GPIO_HALT_REQ_SHIFT +
		c_ctrl->cci_info->acquired_gpio_queue);
	cam_io_w_mb(val, base + CCI_HALT_REQ_ADDR);

	if (c_ctrl->cci_info->acquired_gpio_queue >= 0) {
		queue = c_ctrl->cci_info->acquired_gpio_queue;
		mutex_lock(&cci_dev->init_mutex);
		cci_dev->gpio_queue[queue].is_acquired = false;
		c_ctrl->cci_info->acquired_gpio_queue = -1;
		mutex_unlock(&cci_dev->init_mutex);
		CAM_DBG(CAM_CCI, "GPIO queue %d released", queue);
	}

	return rc;
}

int cam_cci_fsync_core_cfg(struct v4l2_subdev *sd,
	struct cam_cci_ctrl *cci_ctrl)
{
	int rc = 0;

	switch (cci_ctrl->cmd) {
	case MSM_CCI_TIMER_FSYNC_INDEPENDENT:
		/*
		 * Load the GPIO queue from the cmd_buf passed via cci_client,
		 * start it, then release the queue immediately (transient
		 * ownership — the queue is held only for this trigger cycle).
		 */
		rc = cam_cci_load_gpio_queue(sd, cci_ctrl,
			&cci_ctrl->cci_info->cmd_buf);
		if (rc) {
			CAM_ERR(CAM_CCI, "GPIO queue load failed: %d", rc);
			break;
		}
		rc = __cci_gpio_queue_start(sd, cci_ctrl);
		if (rc)
			CAM_ERR(CAM_CCI, "GPIO queue start failed: %d", rc);

		__cci_halt_gpio_queue(sd, cci_ctrl);
		break;
	case MSM_CCI_TIMER_FSYNC_INFINITE:
		/*
		 * Load the GPIO queue from the cmd_buf passed via cci_client,
		 * start it, then release the queue immediately (transient
		 * ownership — the queue is held only for this trigger cycle).
		 */
		rc = cam_cci_load_gpio_queue(sd, cci_ctrl,
			&cci_ctrl->cci_info->cmd_buf);
		if (rc) {
			CAM_ERR(CAM_CCI, "GPIO queue load failed: %d", rc);
			break;
		}
		rc = __cci_gpio_queue_start(sd, cci_ctrl);
		if (rc) {
			CAM_ERR(CAM_CCI, "GPIO queue start failed: %d", rc);
			__cci_halt_gpio_queue(sd, cci_ctrl);
		}
		break;
	case MSM_CCI_GPIO_QUEUE_HALT:
		CAM_DBG(CAM_CCI, "MSM_CCI_GPIO_QUEUE_HALT");
		rc = __cci_halt_gpio_queue(sd, cci_ctrl);
		break;
	case MSM_CCI_GPIO_QUEUE_START:
		CAM_DBG(CAM_CCI, "MSM_CCI_GPIO_QUEUE_START");
		rc = __cci_gpio_queue_start(sd, cci_ctrl);
		break;
	default:
		rc = -ENOIOCTLCMD;
		break;
	}

	return rc;
}

static int __cci_init_gpio_queue(struct cci_device *cci_dev)
{
	int rc = 0;
	int i = 0;

	for (i = 0; i < GPIO_Q_MAX; i++) {
		mutex_init(&(cci_dev->gpio_queue[i].gpio_q_mutex));
		cci_dev->gpio_queue[i].fps = 0;
		cci_dev->gpio_queue[i].is_acquired = false;
	}

	return rc;
}

static long cam_cci_subdev_ioctl(struct v4l2_subdev *sd,
	unsigned int cmd, void *arg)
{
	int32_t rc = 0;

	if (arg == NULL) {
		CAM_ERR(CAM_CCI, "Args is Null");
		return -EINVAL;
	}

	switch (cmd) {
	case VIDIOC_MSM_CCI_CFG:
		CAM_ERR_RATE_LIMIT(CAM_CCI,
			"VIDIOC_MSM_CCI_CFG via ioctl is not supported");
		rc = -EOPNOTSUPP;
		break;
	case VIDIOC_CAM_CONTROL:
		break;
	default:
		CAM_ERR_RATE_LIMIT(CAM_CCI, "Invalid ioctl cmd: %d", cmd);
		rc = -ENOIOCTLCMD;
	}

	return rc;
}

#ifdef CONFIG_COMPAT
static long cam_cci_subdev_compat_ioctl(struct v4l2_subdev *sd,
	unsigned int cmd, unsigned long arg)
{
	return cam_cci_subdev_ioctl(sd, cmd, NULL);
}
#endif

irqreturn_t cam_cci_irq(int irq_num, void *data)
{
	uint32_t irq_status0, irq_status1, reg_bmsk;
	uint32_t irq_update_rd_done = 0;
	struct cci_device *cci_dev = data;
	struct cam_hw_soc_info *soc_info =
		&cci_dev->soc_info;
	void __iomem *base = soc_info->reg_map[0].mem_base;
	unsigned long flags;
	bool rd_done_th_assert = false;
	struct cam_cci_master_info *cci_master_info;
	irqreturn_t rc = IRQ_HANDLED;
	int32_t  next_head;

	irq_status0 = cam_io_r_mb(base + CCI_IRQ_STATUS_0_ADDR);
	irq_status1 = cam_io_r_mb(base + CCI_IRQ_STATUS_1_ADDR);
	CAM_DBG(CAM_CCI,
		"BASE: %p, irq0:%x irq1:%x",
		base, irq_status0, irq_status1);

	cam_io_w_mb(irq_status0, base + CCI_IRQ_CLEAR_0_ADDR);
	cam_io_w_mb(irq_status1, base + CCI_IRQ_CLEAR_1_ADDR);

	reg_bmsk = CCI_IRQ_MASK_1_RMSK;
	if ((irq_status1 & CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD) &&
	!(irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_RD_DONE_BMSK)) {
		reg_bmsk &= ~CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD;
		spin_lock_irqsave(&cci_dev->lock_status, flags);
		cci_dev->irqs_disabled |=
			CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD;
		spin_unlock_irqrestore(&cci_dev->lock_status, flags);
	}

	if ((irq_status1 & CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD) &&
	!(irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_RD_DONE_BMSK)) {
		reg_bmsk &= ~CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD;
		spin_lock_irqsave(&cci_dev->lock_status, flags);
		cci_dev->irqs_disabled |=
			CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD;
		spin_unlock_irqrestore(&cci_dev->lock_status, flags);
	}

	if (reg_bmsk != CCI_IRQ_MASK_1_RMSK) {
		cam_io_w_mb(reg_bmsk, base + CCI_IRQ_MASK_1_ADDR);
		CAM_DBG(CAM_CCI, "Updating the reg mask for irq1: 0x%x",
			reg_bmsk);
	} else if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_RD_DONE_BMSK ||
		irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_RD_DONE_BMSK) {
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_RD_DONE_BMSK) {
			spin_lock_irqsave(&cci_dev->lock_status, flags);
			if (cci_dev->irqs_disabled &
				CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD) {
				irq_update_rd_done |=
					CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD;
				cci_dev->irqs_disabled &=
					~CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD;
			}
			spin_unlock_irqrestore(&cci_dev->lock_status, flags);
		}
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_RD_DONE_BMSK) {
			spin_lock_irqsave(&cci_dev->lock_status, flags);
			if (cci_dev->irqs_disabled &
				CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD) {
				irq_update_rd_done |=
					CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD;
				cci_dev->irqs_disabled &=
					~CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD;
			}
			spin_unlock_irqrestore(&cci_dev->lock_status, flags);
		}
	}

	if (irq_update_rd_done != 0) {
		irq_update_rd_done |= cam_io_r_mb(base + CCI_IRQ_MASK_1_ADDR);
		cam_io_w_mb(irq_update_rd_done, base + CCI_IRQ_MASK_1_ADDR);
	}

	cam_io_w_mb(0x1, base + CCI_IRQ_GLOBAL_CLEAR_CMD_ADDR);

	if (irq_status0 & CCI_IRQ_STATUS_0_RST_DONE_ACK_BMSK) {
		struct cam_cci_master_info *cci_master_info;
		if (cci_dev->cci_master_info[MASTER_0].reset_pending == true) {
			cci_master_info = &cci_dev->cci_master_info[MASTER_0];
			cci_dev->cci_master_info[MASTER_0].reset_pending =
				false;
			if (!cci_master_info->status)
				complete(&cci_master_info->reset_complete);

			complete_all(&cci_master_info->rd_done);
			complete_all(&cci_master_info->th_complete);
		}
		if (cci_dev->cci_master_info[MASTER_1].reset_pending == true) {
			cci_master_info = &cci_dev->cci_master_info[MASTER_1];
			cci_dev->cci_master_info[MASTER_1].reset_pending =
				false;
			if (!cci_master_info->status)
				complete(&cci_master_info->reset_complete);

			complete_all(&cci_master_info->rd_done);
			complete_all(&cci_master_info->th_complete);
		}
	}

	if ((irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_RD_DONE_BMSK) &&
		(irq_status1 & CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD)) {
		cci_dev->cci_master_info[MASTER_0].status = 0;
		rd_done_th_assert = true;
		complete(&cci_dev->cci_master_info[MASTER_0].th_complete);
		complete(&cci_dev->cci_master_info[MASTER_0].rd_done);
	}
	if ((irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_RD_DONE_BMSK) &&
		(!rd_done_th_assert)) {
		cci_dev->cci_master_info[MASTER_0].status = 0;
		rd_done_th_assert = true;
		if (cci_dev->is_burst_read[MASTER_0])
			complete(
			&cci_dev->cci_master_info[MASTER_0].th_complete);
		complete(&cci_dev->cci_master_info[MASTER_0].rd_done);
	}
	if ((irq_status1 & CCI_IRQ_STATUS_1_I2C_M0_RD_THRESHOLD) &&
		(!rd_done_th_assert)) {
		cci_dev->cci_master_info[MASTER_0].status = 0;
		complete(&cci_dev->cci_master_info[MASTER_0].th_complete);
	}
	if (irq_status1 & CCI_IRQ_STATUS_1_I2C_M1_Q0_THRESHOLD)
	{
		cci_master_info = &cci_dev->cci_master_info[MASTER_1];
		spin_lock_irqsave(&cci_dev->lock_status, flags);
		trace_cam_cci_burst(cci_dev->soc_info.index, 1, 0,
			"th_irq honoured irq1",	irq_status1);
		CAM_DBG(CAM_CCI, "CCI%d_M1_Q0: th_irq honoured irq1: 0x%x th_irq_ref_cnt: %d",
			cci_dev->soc_info.index, irq_status1,
			cci_master_info->th_irq_ref_cnt[QUEUE_0]);
		if (cci_master_info->th_irq_ref_cnt[QUEUE_0] == 1) {
			complete(&cci_master_info->th_burst_complete[QUEUE_0]);
		} else {
			// Decrement Threshold irq ref count
			cci_master_info->th_irq_ref_cnt[QUEUE_0]--;
			next_head = increment_index(head);
			if (next_head == tail) {
				CAM_ERR(CAM_CCI,
					"CCI%d_M1_Q0 CPU Scheduing Issue: "
					"Unable to process BURST",
					cci_dev->soc_info.index);
				rc = IRQ_NONE;
			} else {
				cci_irq_queue[head].irq_status0 = irq_status0;
				cci_irq_queue[head].irq_status1 = irq_status1;
				cci_irq_queue[head].master = MASTER_1;
				cci_irq_queue[head].queue = QUEUE_0;
				cci_irq_queue[head].is_valid = 1;
				head = next_head;
				// wake up Threaded irq Handler
				rc = IRQ_WAKE_THREAD;
			}
		}
		spin_unlock_irqrestore(&cci_dev->lock_status, flags);
	}
	if (irq_status1 & CCI_IRQ_STATUS_1_I2C_M1_Q1_THRESHOLD)
	{
		cci_master_info = &cci_dev->cci_master_info[MASTER_1];
		spin_lock_irqsave(&cci_dev->lock_status, flags);
		trace_cam_cci_burst(cci_dev->soc_info.index, 1, 1,
			"th_irq honoured irq1",	irq_status1);
		CAM_DBG(CAM_CCI,
			"CCI%d_M1_Q1: th_irq honoured irq1: 0x%x th_irq_ref_cnt: %d",
			cci_dev->soc_info.index, irq_status1,
			cci_master_info->th_irq_ref_cnt[QUEUE_1]);
		if (cci_master_info->th_irq_ref_cnt[QUEUE_1] == 1) {
			complete(&cci_master_info->th_burst_complete[QUEUE_1]);
		} else {
			// Decrement Threshold irq ref count
			cci_master_info->th_irq_ref_cnt[QUEUE_1]--;
			next_head = increment_index(head);
			if (next_head == tail) {
				CAM_ERR(CAM_CCI,
					"CCI%d_M1_Q0 CPU Scheduing Issue: "
					"Unable to process BURST",
					cci_dev->soc_info.index);
				rc = IRQ_NONE;
			} else {
				cci_irq_queue[head].irq_status0 = irq_status0;
				cci_irq_queue[head].irq_status1 = irq_status1;
				cci_irq_queue[head].master = MASTER_1;
				cci_irq_queue[head].queue = QUEUE_1;
				cci_irq_queue[head].is_valid = 1;
				head = next_head;
				// wake up Threaded irq Handler
				rc = IRQ_WAKE_THREAD;
			}
		}
		spin_unlock_irqrestore(&cci_dev->lock_status, flags);
	}
	if (irq_status1 & CCI_IRQ_STATUS_1_I2C_M0_Q0_THRESHOLD)
	{
		cci_master_info = &cci_dev->cci_master_info[MASTER_0];
		spin_lock_irqsave(&cci_dev->lock_status, flags);
		trace_cam_cci_burst(cci_dev->soc_info.index, 0, 0,
			"th_irq honoured irq1",	irq_status1);
		CAM_DBG(CAM_CCI,
			"CCI%d_M0_Q0: th_irq honoured irq1: 0x%x th_irq_ref_cnt: %d",
			cci_dev->soc_info.index, irq_status1,
			cci_master_info->th_irq_ref_cnt[QUEUE_0]);
		if (cci_master_info->th_irq_ref_cnt[QUEUE_0] == 1) {
			complete(&cci_master_info->th_burst_complete[QUEUE_0]);
		} else {
			// Decrement Threshold irq ref count
			cci_master_info->th_irq_ref_cnt[QUEUE_0]--;
			next_head = increment_index(head);
			if (next_head == tail) {
				CAM_ERR(CAM_CCI,
					"CCI%d_M1_Q0 CPU Scheduing Issue: "
					"Unable to process BURST",
					cci_dev->soc_info.index);
				rc = IRQ_NONE;
			} else {
				cci_irq_queue[head].irq_status0 = irq_status0;
				cci_irq_queue[head].irq_status1 = irq_status1;
				cci_irq_queue[head].master = MASTER_0;
				cci_irq_queue[head].queue = QUEUE_0;
				cci_irq_queue[head].is_valid = 1;
				head = next_head;
				// wake up Threaded irq Handler
				rc = IRQ_WAKE_THREAD;
			}
		}
		spin_unlock_irqrestore(&cci_dev->lock_status, flags);
	}
	if (irq_status1 & CCI_IRQ_STATUS_1_I2C_M0_Q1_THRESHOLD)
	{
		cci_master_info = &cci_dev->cci_master_info[MASTER_0];
		spin_lock_irqsave(&cci_dev->lock_status, flags);
		trace_cam_cci_burst(cci_dev->soc_info.index, 0, 1,
			"th_irq honoured irq1",	irq_status1);
		CAM_DBG(CAM_CCI,
			"CCI%d_M0_Q1: th_irq honoured irq1: 0x%x th_irq_ref_cnt: %d",
			cci_dev->soc_info.index, irq_status1,
			cci_master_info->th_irq_ref_cnt[QUEUE_1]);
		if (cci_master_info->th_irq_ref_cnt[QUEUE_1] == 1) {
			complete(&cci_master_info->th_burst_complete[QUEUE_1]);
		} else {
			// Decrement Threshold irq ref count
			cci_master_info->th_irq_ref_cnt[QUEUE_1]--;
			next_head = increment_index(head);
			if (next_head == tail) {
				CAM_ERR(CAM_CCI,
					"CCI%d_M1_Q0 CPU Scheduing Issue: "
					"Unable to process BURST",
					cci_dev->soc_info.index);
				rc = IRQ_NONE;
			} else {
				cci_irq_queue[head].irq_status0 = irq_status0;
				cci_irq_queue[head].irq_status1 = irq_status1;
				cci_irq_queue[head].master = MASTER_0;
				cci_irq_queue[head].queue = QUEUE_1;
				cci_irq_queue[head].is_valid = 1;
				head = next_head;
				// wake up Threaded irq Handler
				rc = IRQ_WAKE_THREAD;
			}
		}
		spin_unlock_irqrestore(&cci_dev->lock_status, flags);
	}
	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_Q0_REPORT_BMSK) {
		struct cam_cci_master_info *cci_master_info;

		cci_master_info = &cci_dev->cci_master_info[MASTER_0];
		spin_lock_irqsave(
			&cci_dev->cci_master_info[MASTER_0].lock_q[QUEUE_0],
			flags);
		atomic_set(&cci_master_info->q_free[QUEUE_0], 0);
		cci_master_info->status = 0;
		if (atomic_read(&cci_master_info->done_pending[QUEUE_0]) == 1) {
			complete(&cci_master_info->report_q[QUEUE_0]);
			atomic_set(&cci_master_info->done_pending[QUEUE_0], 0);
		}
		spin_unlock_irqrestore(
			&cci_dev->cci_master_info[MASTER_0].lock_q[QUEUE_0],
			flags);
	}
	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_Q1_REPORT_BMSK) {
		struct cam_cci_master_info *cci_master_info;

		cci_master_info = &cci_dev->cci_master_info[MASTER_0];
		spin_lock_irqsave(
			&cci_dev->cci_master_info[MASTER_0].lock_q[QUEUE_1],
			flags);
		atomic_set(&cci_master_info->q_free[QUEUE_1], 0);
		cci_master_info->status = 0;
		if (atomic_read(&cci_master_info->done_pending[QUEUE_1]) == 1) {
			complete(&cci_master_info->report_q[QUEUE_1]);
			atomic_set(&cci_master_info->done_pending[QUEUE_1], 0);
		}
		spin_unlock_irqrestore(
			&cci_dev->cci_master_info[MASTER_0].lock_q[QUEUE_1],
			flags);
	}
	rd_done_th_assert = false;
	if ((irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_RD_DONE_BMSK) &&
		(irq_status1 & CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD)) {
		cci_dev->cci_master_info[MASTER_1].status = 0;
		rd_done_th_assert = true;
		complete(&cci_dev->cci_master_info[MASTER_1].th_complete);
		complete(&cci_dev->cci_master_info[MASTER_1].rd_done);
	}
	if ((irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_RD_DONE_BMSK) &&
		(!rd_done_th_assert)) {
		cci_dev->cci_master_info[MASTER_1].status = 0;
		rd_done_th_assert = true;
		if (cci_dev->is_burst_read[MASTER_1])
			complete(
			&cci_dev->cci_master_info[MASTER_1].th_complete);
		complete(&cci_dev->cci_master_info[MASTER_1].rd_done);
	}
	if ((irq_status1 & CCI_IRQ_STATUS_1_I2C_M1_RD_THRESHOLD) &&
		(!rd_done_th_assert)) {
		cci_dev->cci_master_info[MASTER_1].status = 0;
		complete(&cci_dev->cci_master_info[MASTER_1].th_complete);
	}
	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_Q0_REPORT_BMSK) {
		struct cam_cci_master_info *cci_master_info;

		cci_master_info = &cci_dev->cci_master_info[MASTER_1];
		spin_lock_irqsave(
			&cci_dev->cci_master_info[MASTER_1].lock_q[QUEUE_0],
			flags);
		atomic_set(&cci_master_info->q_free[QUEUE_0], 0);
		cci_master_info->status = 0;
		if (atomic_read(&cci_master_info->done_pending[QUEUE_0]) == 1) {
			complete(&cci_master_info->report_q[QUEUE_0]);
			atomic_set(&cci_master_info->done_pending[QUEUE_0], 0);
		}
		spin_unlock_irqrestore(
			&cci_dev->cci_master_info[MASTER_1].lock_q[QUEUE_0],
			flags);
	}
	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_Q1_REPORT_BMSK) {
		struct cam_cci_master_info *cci_master_info;

		cci_master_info = &cci_dev->cci_master_info[MASTER_1];
		spin_lock_irqsave(
			&cci_dev->cci_master_info[MASTER_1].lock_q[QUEUE_1],
			flags);
		atomic_set(&cci_master_info->q_free[QUEUE_1], 0);
		cci_master_info->status = 0;
		if (atomic_read(&cci_master_info->done_pending[QUEUE_1]) == 1) {
			complete(&cci_master_info->report_q[QUEUE_1]);
			atomic_set(&cci_master_info->done_pending[QUEUE_1], 0);
		}
		spin_unlock_irqrestore(
			&cci_dev->cci_master_info[MASTER_1].lock_q[QUEUE_1],
			flags);
	}
	if (irq_status1 & CCI_IRQ_STATUS_1_I2C_M0_RD_PAUSE)
		CAM_DBG(CAM_CCI, "RD_PAUSE ON MASTER_0");

	if (irq_status1 & CCI_IRQ_STATUS_1_I2C_M1_RD_PAUSE)
		CAM_DBG(CAM_CCI, "RD_PAUSE ON MASTER_1");

	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_Q0Q1_HALT_ACK_BMSK) {
		cci_dev->cci_master_info[MASTER_0].reset_pending = true;
		cam_io_w_mb(CCI_M0_RESET_RMSK,
			base + CCI_RESET_CMD_ADDR);
	}
	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_Q0Q1_HALT_ACK_BMSK) {
		cci_dev->cci_master_info[MASTER_1].reset_pending = true;
		cam_io_w_mb(CCI_M1_RESET_RMSK,
			base + CCI_RESET_CMD_ADDR);
	}
	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_ERROR_BMSK) {
		cci_dev->cci_master_info[MASTER_0].status = -EINVAL;
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_Q0_NACK_ERROR_BMSK) {
			if (cci_dev->is_probing) {
				CAM_INFO(CAM_CCI,
					"Base:%pK,cci: %d, M0_Q0 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
			} else {
				CAM_ERR(CAM_CCI,
					"Base:%pK,cci: %d, M0_Q0 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
				trace_cam_cci_burst(cci_dev->soc_info.index, 0, 0,
					"NACK_ERROR irq0", irq_status0);
			}
			cam_cci_dump_registers(cci_dev, MASTER_0,
					QUEUE_0);
			if ((cci_dev->cci_master_info[MASTER_0].th_irq_ref_cnt[QUEUE_0]) > 0) {
				complete_all(&cci_dev->cci_master_info[MASTER_0].
					th_burst_complete[QUEUE_0]);
			}
			complete_all(&cci_dev->cci_master_info[MASTER_0]
				.report_q[QUEUE_0]);
		}
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_Q1_NACK_ERROR_BMSK) {
			if (cci_dev->is_probing) {
				CAM_INFO(CAM_CCI,
					"Base:%pK,cci: %d, M0_Q1 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
			} else {
				CAM_ERR(CAM_CCI,
					"Base:%pK,cci: %d, M0_Q1 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
				trace_cam_cci_burst(cci_dev->soc_info.index, 0, 1,
					"NACK_ERROR irq0", irq_status0);
			}
			cam_cci_dump_registers(cci_dev, MASTER_0,
					QUEUE_1);
			if ((cci_dev->cci_master_info[MASTER_0].th_irq_ref_cnt[QUEUE_1]) > 0) {
				complete_all(&cci_dev->cci_master_info[MASTER_0].
					th_burst_complete[QUEUE_1]);
			}
			complete_all(&cci_dev->cci_master_info[MASTER_0]
			.report_q[QUEUE_1]);
		}
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_Q0Q1_ERROR_BMSK)
			CAM_ERR(CAM_CCI,
			"Base:%pK, cci: %d, M0 QUEUE_OVER/UNDER_FLOW OR CMD ERR: 0x%x",
				base, cci_dev->soc_info.index, irq_status0);
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M0_RD_ERROR_BMSK)
			CAM_ERR(CAM_CCI,
				"Base: %pK, M0 RD_OVER/UNDER_FLOW ERROR: 0x%x",
				base, irq_status0);

		cci_dev->cci_master_info[MASTER_0].reset_pending = true;
		cam_io_w_mb(CCI_M0_RESET_RMSK, base + CCI_RESET_CMD_ADDR);
	}
	if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_ERROR_BMSK) {
		cci_dev->cci_master_info[MASTER_1].status = -EINVAL;
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_Q0_NACK_ERROR_BMSK) {
			if (cci_dev->is_probing) {
				CAM_INFO(CAM_CCI,
					"Base:%pK, cci: %d, M1_Q0 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
			} else {
				CAM_ERR(CAM_CCI,
					"Base:%pK, cci: %d, M1_Q0 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
				trace_cam_cci_burst(cci_dev->soc_info.index, 1, 0,
					"NACK_ERROR irq0", irq_status0);
			}
			cam_cci_dump_registers(cci_dev, MASTER_1,
					QUEUE_0);
			if ((cci_dev->cci_master_info[MASTER_1].th_irq_ref_cnt[QUEUE_0]) > 0) {
				complete_all(&cci_dev->cci_master_info[MASTER_1].
					th_burst_complete[QUEUE_0]);
			}
			complete_all(&cci_dev->cci_master_info[MASTER_1]
			.report_q[QUEUE_0]);
		}
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_Q1_NACK_ERROR_BMSK) {
			if (cci_dev->is_probing) {
				CAM_INFO(CAM_CCI,
					"Base:%pK, cci: %d, M1_Q1 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
			} else {
				CAM_ERR(CAM_CCI,
					"Base:%pK, cci: %d, M1_Q1 NACK ERROR: 0x%x",
					base, cci_dev->soc_info.index, irq_status0);
				trace_cam_cci_burst(cci_dev->soc_info.index, 1, 1,
					"NACK_ERROR irq0", irq_status0);
			}
			cam_cci_dump_registers(cci_dev, MASTER_1,
				QUEUE_1);
			if ((cci_dev->cci_master_info[MASTER_1].th_irq_ref_cnt[QUEUE_1]) > 0) {
				complete_all(&cci_dev->cci_master_info[MASTER_1].
					th_burst_complete[QUEUE_1]);
			}
			complete_all(&cci_dev->cci_master_info[MASTER_1]
			.report_q[QUEUE_1]);
		}
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_Q0Q1_ERROR_BMSK)
			CAM_ERR(CAM_CCI,
			"Base:%pK, cci: %d, M1 QUEUE_OVER_UNDER_FLOW OR CMD ERROR:0x%x",
				base, cci_dev->soc_info.index, irq_status0);
		if (irq_status0 & CCI_IRQ_STATUS_0_I2C_M1_RD_ERROR_BMSK)
			CAM_ERR(CAM_CCI,
				"Base:%pK, cci: %d, M1 RD_OVER/UNDER_FLOW ERROR: 0x%x",
				base, cci_dev->soc_info.index, irq_status0);

		cci_dev->cci_master_info[MASTER_1].reset_pending = true;
		cam_io_w_mb(CCI_M1_RESET_RMSK, base + CCI_RESET_CMD_ADDR);
	}

	return rc;
}

irqreturn_t cam_cci_threaded_irq(int irq_num, void *data)
{
	struct cci_device *cci_dev = data;
	struct cam_hw_soc_info *soc_info =
		&cci_dev->soc_info;
	struct cci_irq_data cci_data;
	unsigned long flags;
	uint32_t triggerHalfQueue = 1;
	struct task_struct *task = current;

	CAM_INFO(CAM_CCI, "CCI%d: nice: %d rt-Priority: %d cci_dev: %p",
		soc_info->index, task_nice(current), task->rt_priority, cci_dev);
	spin_lock_irqsave(&cci_dev->lock_status, flags);
	if (tail != head) {
		cci_data = cci_irq_queue[tail];
		tail = increment_index(tail);
		/*
		 * "head" and "tail" variables are shared across
		 * Top half and Bottom Half routines, Hence place a
		 * lock while accessing these variables.
		 */
		spin_unlock_irqrestore(&cci_dev->lock_status, flags);
		cam_cci_data_queue_burst_apply(cci_dev,
			cci_data.master, cci_data.queue, triggerHalfQueue);
		spin_lock_irqsave(&cci_dev->lock_status, flags);
	}
	spin_unlock_irqrestore(&cci_dev->lock_status, flags);
	return IRQ_HANDLED;
}

static int cam_cci_irq_routine(struct v4l2_subdev *sd, u32 status,
	bool *handled)
{
	struct cci_device *cci_dev = NULL;
	irqreturn_t ret;
	struct cam_hw_soc_info *soc_info = NULL;

	if (!sd) {
		CAM_ERR(CAM_CCI, "Error No data in subdev");
		return -EINVAL;
	}

	cci_dev = v4l2_get_subdevdata(sd);
	if (!cci_dev) {
		CAM_ERR(CAM_CCI, "cci_dev NULL");
		return -EINVAL;
	}

	soc_info = &cci_dev->soc_info;

	ret = cam_cci_irq(soc_info->irq_num[0], cci_dev);
	if (ret == IRQ_NONE)
		CAM_ERR(CAM_CCI, "Interrupt was not handled properly, ret %d", ret);

	*handled = true;
	return 0;
}

static struct v4l2_subdev_core_ops cci_subdev_core_ops = {
	.ioctl = cam_cci_subdev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl32 = cam_cci_subdev_compat_ioctl,
#endif
	.interrupt_service_routine = cam_cci_irq_routine,
};

static const struct v4l2_subdev_ops cci_subdev_ops = {
	.core = &cci_subdev_core_ops,
};

static const struct v4l2_subdev_internal_ops cci_subdev_intern_ops;

static int cam_cci_get_debug(void *data, u64 *val)
{
	struct cci_device *cci_dev = (struct cci_device *)data;

	*val = cci_dev->dump_en;

	return 0;
}

static int cam_cci_set_debug(void *data, u64 val)
{
	struct cci_device *cci_dev = (struct cci_device *)data;

	cci_dev->dump_en = val;

	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(cam_cci_debug,
	cam_cci_get_debug,
	cam_cci_set_debug, "%16llu\n");

static int cam_cci_create_debugfs_entry(struct cci_device *cci_dev)
{
	int rc = 0, idx;
	struct dentry *dbgfileptr = NULL;
	static char * const filename[] = { "en_dump_cci0", "en_dump_cci1",
		"en_dump_cci2", "en_dump_cci3"};

	if (!cam_debugfs_available())
		return 0;

	if (!debugfs_root) {
		rc = cam_debugfs_create_subdir("cci", &dbgfileptr);
		if (rc) {
			CAM_ERR(CAM_CCI, "debugfs directory creation fail");
			return rc;
		}
		debugfs_root = dbgfileptr;
	}

	idx = cci_dev->soc_info.index;
	if (idx >= ARRAY_SIZE(filename)) {
		CAM_ERR(CAM_CCI, "cci-dev %d invalid", idx);
		return -ENODEV;
	}

	debugfs_create_file(filename[idx], 0644, debugfs_root, cci_dev, &cam_cci_debug);

	return 0;
}

static int cam_cci_component_bind(struct device *dev,
	struct device *master_dev, void *data)
{
	struct cam_cpas_register_params cpas_parms;
	struct cci_device *new_cci_dev;
	struct cam_hw_soc_info *soc_info = NULL;
	int rc = 0;
	struct platform_device *pdev = to_platform_device(dev);

	new_cci_dev = devm_kzalloc(&pdev->dev, sizeof(struct cci_device),
		GFP_KERNEL);
	if (!new_cci_dev) {
		CAM_ERR(CAM_CCI, "Memory allocation failed for cci_dev");
		return -ENOMEM;
	}
	soc_info = &new_cci_dev->soc_info;

	new_cci_dev->v4l2_dev_str.pdev = pdev;

	soc_info->pdev = pdev;
	soc_info->dev = &pdev->dev;
	soc_info->dev_name = pdev->name;

	new_cci_dev->hw_info = of_device_get_match_data(&pdev->dev);
	if (!new_cci_dev->hw_info) {
		CAM_ERR(CAM_CCI, "CCI: no hw_info for compatible string");
		goto cci_no_resource;
	}
	if (new_cci_dev->hw_info->variant == CAM_CCI_HW_CCU_QUP) {
		CAM_INFO(CAM_CCI, "CCU QUP I2C path is not supported");
		return -EOPNOTSUPP;
	}

	rc = cam_cci_parse_dt_info(new_cci_dev);
	if (rc < 0) {
		CAM_ERR(CAM_CCI, "Resource get Failed rc:%d", rc);
		goto cci_no_resource;
	}

	CAM_DBG(CAM_CCI, "CCI%d: hw variant=%d",
		new_cci_dev->soc_info.index,
		new_cci_dev->hw_info->variant);

	new_cci_dev->v4l2_dev_str.internal_ops =
		&cci_subdev_intern_ops;
	new_cci_dev->v4l2_dev_str.ops =
		&cci_subdev_ops;
	strscpy(new_cci_dev->device_name, CAMX_CCI_DEV_NAME,
		sizeof(new_cci_dev->device_name));
	new_cci_dev->v4l2_dev_str.name =
		new_cci_dev->device_name;
	new_cci_dev->v4l2_dev_str.sd_flags = V4L2_SUBDEV_FL_HAS_EVENTS;
	new_cci_dev->v4l2_dev_str.ent_function =
		CAM_CCI_DEVICE_TYPE;
	new_cci_dev->v4l2_dev_str.token =
		new_cci_dev;

	rc = cam_register_subdev(&(new_cci_dev->v4l2_dev_str));
	if (rc < 0) {
		CAM_ERR(CAM_CCI, "Fail with cam_register_subdev rc: %d", rc);
		goto cci_no_resource;
	}

	platform_set_drvdata(pdev, &(new_cci_dev->v4l2_dev_str.sd));
	v4l2_set_subdevdata(&new_cci_dev->v4l2_dev_str.sd, new_cci_dev);
	if (soc_info->index >= MAX_CCI) {
		CAM_ERR(CAM_CCI, "Invalid index: %d max supported:%d",
			soc_info->index, MAX_CCI-1);
		goto cci_no_resource;
	}

	g_cci_subdev[soc_info->index] = &new_cci_dev->v4l2_dev_str.sd;
	mutex_init(&(new_cci_dev->init_mutex));
	CAM_DBG(CAM_CCI, "Device Type :%d", soc_info->index);

	__cci_init_gpio_queue(new_cci_dev);
	cpas_parms.cam_cpas_client_cb = NULL;
	cpas_parms.cell_index = soc_info->index;
	cpas_parms.dev = &pdev->dev;
	cpas_parms.userdata = new_cci_dev;
	strscpy(cpas_parms.identifier, "cci", CAM_HW_IDENTIFIER_LENGTH);
	rc = cam_cpas_register_client(&cpas_parms);
	if (rc) {
		CAM_ERR(CAM_CCI, "CPAS registration failed rc:%d", rc);
		goto cci_unregister_subdev;
	}

	CAM_DBG(CAM_CCI, "CPAS registration successful handle=%d",
		cpas_parms.client_handle);
	new_cci_dev->cpas_handle = cpas_parms.client_handle;

	rc = cam_cci_create_debugfs_entry(new_cci_dev);
	if (rc) {
		CAM_WARN(CAM_CCI, "debugfs creation failed");
		rc = 0;
	}
#ifdef CONFIG_SPECTRA_SENSOR_SYSFS_UTIL
	if (cam_sysfs_add_cci(new_cci_dev)) {
		CAM_DBG(CAM_CCI, "CCI%d sysfs creation success", soc_info->index);
	} else {
		CAM_DBG(CAM_CCI, "CCI%d sysfs creation failed", soc_info->index);
	}
#endif /*CONFIG_SPECTRA_SENSOR_SYSFS_UTIL*/
	head = 0;
	tail = 0;
	CAM_DBG(CAM_CCI, "Component bound successfully");
	return rc;

cci_unregister_subdev:
	cam_unregister_subdev(&(new_cci_dev->v4l2_dev_str));
cci_no_resource:
	devm_kfree(&pdev->dev, new_cci_dev);
	return rc;
}

static void cam_cci_component_unbind(struct device *dev,
	struct device *master_dev, void *data)
{
	int rc = 0;
	struct platform_device *pdev = to_platform_device(dev);

	struct v4l2_subdev *subdev = platform_get_drvdata(pdev);
	struct cci_device *cci_dev = NULL;

	if (!subdev) {
		CAM_ERR(CAM_CCI, "Error No data in subdev");
		return;
	}

	cci_dev = v4l2_get_subdevdata(subdev);
	if (!cci_dev) {
		CAM_ERR(CAM_CCI, "Error No data in cci_dev");
		return;
	}

	if (!cci_dev) {
		CAM_ERR(CAM_CCI, "cci_dev NULL");
		return;
	}

	cam_cpas_unregister_client(cci_dev->cpas_handle);
	debugfs_root = NULL;
#ifdef CONFIG_SPECTRA_SENSOR_SYSFS_UTIL
	cam_sysfs_remove_cci(cci_dev);
#endif /*CONFIG_SPECTRA_SENSOR_SYSFS_UTIL*/
	cam_cci_soc_remove(pdev, cci_dev);
	rc = cam_unregister_subdev(&(cci_dev->v4l2_dev_str));
	if (rc < 0)
		CAM_ERR(CAM_CCI, "Fail with cam_unregister_subdev. rc:%d", rc);
	devm_kfree(&pdev->dev, cci_dev);
}

static const struct component_ops cam_cci_component_ops = {
	.bind = cam_cci_component_bind,
	.unbind = cam_cci_component_unbind,
};

static int cam_cci_platform_probe(struct platform_device *pdev)
{
	int rc = 0;

	CAM_DBG(CAM_CCI, "Adding CCI component");
	rc = component_add(&pdev->dev, &cam_cci_component_ops);
	if (rc)
		CAM_ERR(CAM_CCI, "failed to add component rc: %d", rc);

	return rc;
}

static void cam_cci_device_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &cam_cci_component_ops);
}

static const struct of_device_id cam_cci_dt_match[] = {
	/* Standalone legacy CCI — existing SoCs (Lemans, etc.) */
	{ .compatible = "qcom,cci",
	  .data = &cam_cci_hw_info_standalone },
	/* CCU wrapper — legacy CCI path (Auto SoCs, nordschleife) */
	{ .compatible = "qcom,ccu-cci",
	  .data = &cam_cci_hw_info_ccu_legacy },
	/* CCU wrapper — QuP I2C path (future) */
	{ .compatible = "qcom,ccu-qup-i2c",
	  .data = &cam_cci_hw_info_ccu_qup },
	{}
};

MODULE_DEVICE_TABLE(of, cam_cci_dt_match);

struct platform_driver cci_driver = {
	.probe = cam_cci_platform_probe,
	.remove = cam_cci_device_remove,
	.driver = {
		.name = CAMX_CCI_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = cam_cci_dt_match,
		.suppress_bind_attrs = true,
	},
};

int cam_cci_init_module(void)
{
	return platform_driver_register(&cci_driver);
}

void cam_cci_exit_module(void)
{
#ifdef CONFIG_SPECTRA_SENSOR_SYSFS_UTIL
	cam_sysfs_exit();
#endif /*CONFIG_SPECTRA_SENSOR_SYSFS_UTIL*/
	platform_driver_unregister(&cci_driver);
}

MODULE_DESCRIPTION("MSM CCI driver");
MODULE_LICENSE("GPL v2");

static const char *cam_cci_gpio_cmd_type_str(enum cam_cci_gpio_cmd_type cmd)
{
	switch (cmd) {
	case CCI_GPIO_SET_PARAM_CMD:                return "SET_PARAM";
	case CCI_GPIO_WAIT_CMD:                     return "WAIT";
	case CCI_GPIO_WAIT_SYNC_CMD:                return "WAIT_SYNC";
	case CCI_GPIO_WAIT_GPIO_IN_EVENT_CMD:       return "WAIT_GPIO_IN_EVENT";
	case CCI_GPIO_WAIT_I2C_Q_TRIG_EVENT_CMD:    return "WAIT_I2C_Q_TRIG_EVENT";
	case CCI_GPIO_OUT_CMD:                      return "OUT";
	case CCI_GPIO_TRIG_EVENT_CMD:               return "TRIG_EVENT";
	case CCI_GPIO_REPORT_CMD:                   return "REPORT";
	case CCI_GPIO_REPEAT_CMD:                   return "REPEAT";
	case CCI_GPIO_CONTINUE_CMD:                 return "CONTINUE";
	case CCI_GPIO_INVALID_CMD:                  return "INVALID";
	default:                                    return "UNKNOWN";
	}
}

static int cam_cci_prepare_delay_chain(struct cam_cci_gpio_cmd_buf *gpio_cmds,
	uint64_t delay, uint32_t max, enum cam_cci_gpio_cmd_type op)
{
	uint64_t loop = delay / max;

	while (loop > 0) {
		if (gpio_cmds->cmd_count >= CCI_MAX_GPIO_QUEUE_SIZE) {
			CAM_ERR(CAM_CCI, "GPIO cmd buffer overflow in delay chain");
			return -EOVERFLOW;
		}
		gpio_cmds->buf[gpio_cmds->cmd_count] = max << 4 | op;
		gpio_cmds->cmd_count++;
		delay -= max;
		--loop;
	}

	if (delay > 0) {
		if (gpio_cmds->cmd_count >= CCI_MAX_GPIO_QUEUE_SIZE) {
			CAM_ERR(CAM_CCI, "GPIO cmd buffer overflow in delay chain");
			return -EOVERFLOW;
		}
		gpio_cmds->buf[gpio_cmds->cmd_count] = delay << 4 | op;
		gpio_cmds->cmd_count++;
	}

	return 0;
}

int cam_cci_fill_gpio_cmd_buffer(struct cam_cci_gpio_cmd_buf *gpio_cmds,
	int op, uint32_t val)
{
	uint64_t delay = 0;
	int rc = 0;

	if (gpio_cmds->cmd_count >= CCI_MAX_GPIO_QUEUE_SIZE)
		return -EOVERFLOW;

	CAM_DBG(CAM_CCI, "Adding new gpio cmd %s val %u",
		cam_cci_gpio_cmd_type_str(op), val);

	switch (op) {
	case CCI_GPIO_SET_PARAM_CMD:
		/* val is sensor number to which WAIT_SYNC cmd needs to be synchronized to */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	case CCI_GPIO_WAIT_CMD: {
		/* val is wait count [31:4] */
		delay = NSEC_TO_CCI_CLK_CYCLES((uint64_t)val * 1000);
		CAM_DBG(CAM_CCI, "val %llu delay %llu", val, delay);
		if (delay > CCI_GPIO_MAX_DELAY_28BIT_TIMER) {
			rc = cam_cci_prepare_delay_chain(gpio_cmds, delay,
				CCI_GPIO_MAX_DELAY_28BIT_TIMER, op);
			if (rc < 0)
				return rc;
		} else {
			gpio_cmds->buf[gpio_cmds->cmd_count] = delay << 4 | op;
			gpio_cmds->cmd_count++;
		}
		break;
	}
	case CCI_GPIO_WAIT_SYNC_CMD: {
		/* val is wait count [31:18] Line number to be in sync with[17:4] */
		delay = NSEC_TO_CCI_CLK_CYCLES(val);
		CAM_DBG(CAM_CCI, "val %llu delay %llu", val, delay);
		if (delay > CCI_GPIO_MAX_DELAY_14BIT_TIMER) {
			rc = cam_cci_prepare_delay_chain(gpio_cmds, delay,
				CCI_GPIO_MAX_DELAY_14BIT_TIMER, op);
			if (rc < 0)
				return rc;
		} else {
			gpio_cmds->buf[gpio_cmds->cmd_count] = delay << 3 | op;
			gpio_cmds->cmd_count++;
		}
		break;
	}
	case CCI_GPIO_WAIT_GPIO_IN_EVENT_CMD:
		/* val is TRIG_TYPE[9:8] GPIO_IN# [5:4] */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	case CCI_GPIO_WAIT_I2C_Q_TRIG_EVENT_CMD:
		/* val is I2C Queue # [5:4] */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	case CCI_GPIO_OUT_CMD:
		/* val is GPIO_VALUE[16:12] GPIO_WR_EN[8:4] */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	case CCI_GPIO_TRIG_EVENT_CMD:
		/* this is always used with WAIT_GPIO_EVENT from I2C queue cmd set
		 * val is 0 */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	case CCI_GPIO_REPORT_CMD:
		/* val is Capture Report Data [9] IRQ_EN[8] REPORT_ID[7:4] */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	case CCI_GPIO_REPEAT_CMD:
		/* val is REPEAT COUNT [9:4] */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	case CCI_GPIO_CONTINUE_CMD:
		/* specifies the end of repeat loop. This should be the last cmd in a queue
		 * val is 0 */
		gpio_cmds->buf[gpio_cmds->cmd_count] = val << 4 | op;
		gpio_cmds->cmd_count++;
		break;
	default:
		CAM_ERR(CAM_CCI, "Unsupported GPIO Queue operation %s:%d",
			cam_cci_gpio_cmd_type_str(op), op);
		return -EINVAL;
	}

	return 0;
}

int cam_cci_timing_schema_to_cmd_buf(struct cci_gpio_timing_schema *schema,
	struct cam_cci_gpio_cmd_buf *cmd_buf)
{
	uint8_t gpio_w_en = 0;
	uint16_t gpio_val = 0;
	bool pending_gpio_out_cmd = false;
	int32_t gpio_idx = -1;
	size_t i;
	int rc;

	if (!schema || !cmd_buf) {
		CAM_ERR(CAM_CCI, "Invalid params schema=%pK cmd_buf=%pK",
			schema, cmd_buf);
		return -EINVAL;
	}

	if (schema->event_count == 0 ||
	    schema->event_count > CAM_CCI_TIMER_MAX_EVENTS) {
		CAM_ERR(CAM_CCI, "Invalid event_count=%u (valid: 1..%d)",
			schema->event_count, CAM_CCI_TIMER_MAX_EVENTS);
		return -EINVAL;
	}

	/* Initialize cmd_buf - prevents helper from resetting it */
	cmd_buf->cmd_count = 0;
	cmd_buf->cmd_buf_ready = false;
	memset(&cmd_buf->buf, 0, sizeof(cmd_buf->buf));

	/* Convert events using the comprehensive helper */
	for (i = 0; i < schema->event_count; i++) {
		/* Handle delay */
		if (schema->events[i].delay_to_trigger_ns) {
			if (pending_gpio_out_cmd) {
				/* Flush pending OUT command using helper */
				rc = cam_cci_fill_gpio_cmd_buffer(
					cmd_buf,
					CCI_GPIO_OUT_CMD,
					(gpio_val << 8) | gpio_w_en);
				if (rc < 0) {
					CAM_ERR(CAM_CCI,
						"Failed to add OUT cmd: %d", rc);
					return rc;
				}
				pending_gpio_out_cmd = false;
			}
			gpio_w_en = 0;
			gpio_val = 0;

			/* Add WAIT command using helper - handles delay chaining */
			rc = cam_cci_fill_gpio_cmd_buffer(
				cmd_buf,
				CCI_GPIO_WAIT_CMD,
				schema->events[i].delay_to_trigger_ns);
			if (rc < 0) {
				CAM_ERR(CAM_CCI,
					"Failed to add WAIT cmd: %d", rc);
				return rc;
			}
		}

		/* Get GPIO index */
		gpio_idx = cam_sensor_util_remap_timer_to_gpioq_mask(schema->events[i].gpio_number);
		if (gpio_idx < 0) {
			CAM_ERR(CAM_CCI, "Invalid GPIO number %lld",
				schema->events[i].gpio_number);
			return -EINVAL;
		}

		/* Build GPIO command */
		gpio_w_en |= 1 << gpio_idx;
		if (schema->events[i].level)
			gpio_val |= BIT(gpio_idx);
		else
			gpio_val &= ~BIT(gpio_idx);
		pending_gpio_out_cmd = true;
	}

	/* Flush final OUT command using helper */
	if (pending_gpio_out_cmd) {
		rc = cam_cci_fill_gpio_cmd_buffer(
			cmd_buf,
			CCI_GPIO_OUT_CMD,
			(gpio_val << 8) | gpio_w_en);
		if (rc < 0) {
			CAM_ERR(CAM_CCI,
				"Failed to add final OUT cmd: %d", rc);
			return rc;
		}
	}

	cmd_buf->cmd_buf_ready = true;

	CAM_DBG(CAM_CCI, "Converted timing schema to cmd_buf: %d commands",
		cmd_buf->cmd_count);

	return 0;
}

int cam_cci_build_infinite_mode_cmd_buf(struct cci_gpio_timing_schema *schema,
	struct cam_cci_gpio_cmd_buf *cmd_buf)
{
	int rc;

	if (!schema || !cmd_buf) {
		CAM_ERR(CAM_CCI, "Invalid params schema=%pK cmd_buf=%pK",
			schema, cmd_buf);
		return -EINVAL;
	}

	rc = cam_cci_timing_schema_to_cmd_buf(schema, cmd_buf);
	if (rc < 0) {
		CAM_ERR(CAM_CCI, "Failed to convert timing schema: %d", rc);
		return rc;
	}

	/* Need room for REPEAT (prepended) + REPORT + CONTINUE (appended) */
	if (cmd_buf->cmd_count + 3 > CCI_MAX_GPIO_QUEUE_SIZE) {
		CAM_ERR(CAM_CCI,
			"cmd_buf (%u cmds) has no room for REPEAT/REPORT/CONTINUE (max=%d)",
			cmd_buf->cmd_count, CCI_MAX_GPIO_QUEUE_SIZE);
		return -EOVERFLOW;
	}

	/* Prepend CCI_GPIO_REPEAT_CMD so the queue loops from the top */
	memmove(&cmd_buf->buf[1], &cmd_buf->buf[0],
		cmd_buf->cmd_count * sizeof(cmd_buf->buf[0]));
	cmd_buf->buf[0] = 0 << 4 | CCI_GPIO_REPEAT_CMD;
	cmd_buf->cmd_count++;

	/* Append CCI_GPIO_REPORT_CMD */
	rc = cam_cci_fill_gpio_cmd_buffer(cmd_buf, CCI_GPIO_REPORT_CMD, 1);
	if (rc < 0) {
		CAM_ERR(CAM_CCI, "Failed to add REPORT cmd: %d", rc);
		return rc;
	}

	/* Append CCI_GPIO_CONTINUE_CMD to close the repeat loop */
	rc = cam_cci_fill_gpio_cmd_buffer(cmd_buf, CCI_GPIO_CONTINUE_CMD, 0);
	if (rc < 0) {
		CAM_ERR(CAM_CCI, "Failed to add CONTINUE cmd: %d", rc);
		return rc;
	}

	cmd_buf->cmd_buf_ready = true;

	CAM_DBG(CAM_CCI,
		"Built infinite mode cmd_buf: %d commands (REPEAT/.../REPORT/CONTINUE)",
		cmd_buf->cmd_count);

	return 0;
}

int cam_cci_write_gpio_cmd_buf(void __iomem *base, uint32_t reg_addr,
	const struct cam_cci_gpio_cmd_buf *cmd_buf)
{
	int i;

	if (!base || !cmd_buf) {
		CAM_ERR(CAM_CCI, "Invalid params base=%pK cmd_buf=%pK",
			base, cmd_buf);
		return -EINVAL;
	}

	if (cmd_buf->cmd_count == 0) {
		CAM_ERR(CAM_CCI, "cmd_buf is empty, nothing to write");
		return -EINVAL;
	}

	for (i = 0; i < cmd_buf->cmd_count; i++)
		cam_io_w_mb(cmd_buf->buf[i], base + reg_addr);

	return 0;
}
