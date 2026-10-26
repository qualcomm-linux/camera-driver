// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <linux/module.h>
#include <linux/uaccess.h>
#include "cam_sensor_fsync.h"
#include "cam_sensor_dev.h"
#include "cam_sensor_core.h"
#include "cam_sensor_io.h"
#include "cam_debug_util.h"
#include "cam_common_util.h"
#include "cam_sensor_util.h"

static int cam_sensor_fsync_validate_timer(struct cam_sensor_ctrl_t *s_ctrl,
	struct cci_gpio_timing_schema *timer_info,
	struct cci_timer_freq_info *freq_info,
	struct cci_timer_trigger_point_info *tpoint_info)
{
	uint64_t frame_time_us;
	int rc;

	if (tpoint_info->tp.tpoint_perframe_info >= CCI_TIMER_PERFRAME_MAX) {
		CAM_ERR(CAM_SENSOR,
			"SYNC_INFO: invalid tpoint_perframe_info=%u (max=%d)",
			tpoint_info->tp.tpoint_perframe_info,
			CCI_TIMER_PERFRAME_MAX - 1);
		return -EINVAL;
	}

	if (freq_info->freq_mode >= CCI_TIMER_FREQ_MODE_MAX) {
		CAM_ERR(CAM_SENSOR,
			"SYNC_INFO: invalid freq_mode=%u (max=%d)",
			freq_info->freq_mode, CCI_TIMER_FREQ_MODE_MAX - 1);
		return -EINVAL;
	}

	/*
	 * Store the trigger point / refcount for stage-based GPIO fsync.
	 * Only infinite frequency mode uses stage-based triggering (no
	 * per-frame request); it fires post ACQUIRE_DEV / START_DEV once
	 * refcount_to_trigger sensors reach the stored trigger point.
	 */
	if (freq_info->freq_mode == CCI_TIMER_INFINITE_FRAME)
		cam_sensor_fsync_trigger_set(
			tpoint_info->tp.tpoint_fsync_info,
			tpoint_info->refcount_to_trigger);

	if (timer_info->event_count == 0 ||
	    timer_info->event_count > CAM_CCI_TIMER_MAX_EVENTS) {
		CAM_ERR(CAM_SENSOR,
			"SYNC_INFO: invalid event_count=%u (valid: 1..%d)",
			timer_info->event_count, CAM_CCI_TIMER_MAX_EVENTS);
		return -EINVAL;
	}

	/* If no FPS is available, use 30 as default */
	if (s_ctrl->sensor_res[s_ctrl->last_updated_req % MAX_PER_FRAME_ARRAY].fps > 0)
		frame_time_us = 1000000 /
			s_ctrl->sensor_res[s_ctrl->last_updated_req % MAX_PER_FRAME_ARRAY].fps;
	else
		frame_time_us = 33333;

	CAM_DBG(CAM_SENSOR,
		 "SYNC_INFO: event_count=%u tpoint_perframe=%d freq_mode=%d [%s] number_of_frames=%u",
		 timer_info->event_count,
		 tpoint_info->tp.tpoint_perframe_info,
		 freq_info->freq_mode,
		 (freq_info->freq_mode == CCI_TIMER_INFINITE_FRAME) ?
			"INFINITE" : "FINITE",
		 freq_info->number_of_frames);

	rc = cam_sensor_util_validate_pulse_durations(timer_info, frame_time_us);
	if (rc < 0)
		return rc;

	return 0;
}

int cam_sensor_fsync_handle_blob(uint8_t *blob_data, uint32_t blob_size,
	struct cam_sensor_ctrl_t *s_ctrl)
{
	struct cci_sync_info *sync_info = NULL;
	struct cci_timer_fsync_info *fsync_cfg = NULL;
	void *mode_cfg = NULL;
	uint32_t idx;
	int rc;

	/* Step 1: Minimum size check for the fixed-size top-level struct */
	if (blob_size < sizeof(struct cci_sync_info)) {
		CAM_ERR(CAM_SENSOR,
			"SYNC_INFO: blob too small: got=%u need>=%zu",
			blob_size, sizeof(struct cci_sync_info));
		return -EINVAL;
	}

	sync_info = (struct cci_sync_info *)blob_data;

	/* Step 2: Validate operational_mode */
	if (sync_info->operational_mode == CCI_TIMER_MODE_NO_OP ||
	    sync_info->operational_mode >= CCI_TIMER_MODE_SYNC_MAX) {
		CAM_ERR(CAM_SENSOR,
			"SYNC_INFO: invalid operational_mode=%u (valid range: %d..%d)",
			sync_info->operational_mode,
			CCI_TIMER_MODE_NO_OP + 1,
			CCI_TIMER_MODE_SYNC_MAX - 1);
		return -EINVAL;
	}

	/* Only CCI_TIMER_MODE_SYNC_WITH_SINGLE_QUEUE is currently supported */
	if (sync_info->operational_mode != CCI_TIMER_MODE_SYNC_WITH_SINGLE_QUEUE) {
		CAM_ERR(CAM_SENSOR, "SYNC_INFO: unsupported operationalMode %d",
			sync_info->operational_mode);
		return -EOPNOTSUPP;
	}

	/* Step 3: Validate the mode-specific payload size/pointer */
	if (sync_info->config_size < sizeof(struct cci_timer_fsync_info) ||
	    sync_info->config_size > CAM_SENSOR_FSYNC_MAX_CONFIG_SIZE ||
	    !sync_info->config_ptr) {
		CAM_ERR(CAM_SENSOR,
			"SYNC_INFO: invalid config_size=%u config_ptr=%s (need %zu..%d)",
			sync_info->config_size,
			sync_info->config_ptr ? "set" : "NULL",
			sizeof(struct cci_timer_fsync_info),
			CAM_SENSOR_FSYNC_MAX_CONFIG_SIZE);
		return -EINVAL;
	}

	/* Step 4: Pull the mode-specific payload in from userspace */
	mode_cfg = kzalloc(sync_info->config_size, GFP_KERNEL);
	if (!mode_cfg) {
		CAM_ERR(CAM_SENSOR, "SYNC_INFO: failed to alloc %u bytes",
			sync_info->config_size);
		return -ENOMEM;
	}

	if (copy_from_user(mode_cfg,
		u64_to_user_ptr(sync_info->config_ptr),
		sync_info->config_size)) {
		CAM_ERR(CAM_SENSOR, "SYNC_INFO: copy_from_user failed for config_ptr");
		rc = -EFAULT;
		goto free_mode_cfg;
	}

	fsync_cfg = (struct cci_timer_fsync_info *)mode_cfg;

	if (fsync_cfg->hdr.size < sizeof(struct cci_timer_fsync_info)) {
		CAM_ERR(CAM_SENSOR,
			"SYNC_INFO: mode payload hdr.size=%u smaller than struct=%zu",
			fsync_cfg->hdr.size, sizeof(struct cci_timer_fsync_info));
		rc = -EINVAL;
		goto free_mode_cfg;
	}

	CAM_DBG(CAM_SENSOR,
		 "SYNC_INFO: operational_mode=%d config_size=%u",
		 sync_info->operational_mode, sync_info->config_size);

	/* Step 5: Validate the decoded timer entry */
	rc = cam_sensor_fsync_validate_timer(s_ctrl, &fsync_cfg->timer_info,
		&fsync_cfg->freq_info, &fsync_cfg->tpoint_info);
	if (rc < 0)
		goto free_mode_cfg;

	/* Step 6: Convert timing schema into a CCI GPIO command buffer and
	 * store it in the per-request fsync slot
	 */
	if (s_ctrl->io_master_info.master_type == CCI_MASTER) {
		struct cam_sensor_fsync_slot *slot;

		if (!s_ctrl->per_frame_fsync) {
			CAM_ERR(CAM_SENSOR, "SYNC_INFO: per_frame_fsync not allocated");
			rc = -ENOMEM;
			goto free_mode_cfg;
		}

		idx = s_ctrl->last_updated_req % MAX_PER_FRAME_ARRAY;
		slot = &s_ctrl->per_frame_fsync[idx];

		/*
		 * D1 (deferred): only a single GPIO queue (cmd_buf[0]) is
		 * populated here. Multi-queue support will be added as part
		 * of the fsync object redesign.
		 */
		if (fsync_cfg->freq_info.freq_mode == CCI_TIMER_INFINITE_FRAME)
			rc = cam_cci_build_infinite_mode_cmd_buf(
				&fsync_cfg->timer_info, &slot->cmd_buf[0]);
		else
			rc = cam_cci_timing_schema_to_cmd_buf(&fsync_cfg->timer_info,
				&slot->cmd_buf[0]);
		if (rc < 0) {
			CAM_ERR(CAM_SENSOR,
				"Failed to convert timing schema: %d", rc);
			goto free_mode_cfg;
		}

		slot->num_queues = 1;
		slot->request_id = s_ctrl->last_updated_req;
		slot->is_valid = true;

		CAM_DBG(CAM_SENSOR,
			"SYNC_INFO: converted to cmd_buf[0] (%u cmds), "
			"stored in per_frame_fsync[%u] req_id=%lld",
			slot->cmd_buf[0].cmd_count, idx, s_ctrl->last_updated_req);
	} else {
		CAM_INFO(CAM_SENSOR,
			"SYNC_INFO: SENSOR [%s] is not on CCI master, "
			"do not trigger SYNC CFG!", s_ctrl->sensor_name);
	}

	s_ctrl->fsync_blob_ready = true;

	CAM_INFO(CAM_SENSOR,
		 "SYNC_INFO: decode OK - mode=%d req_id=%lld cached into per_frame_fsync[%u]",
		 sync_info->operational_mode,
		 s_ctrl->last_updated_req,
		 (uint32_t)(s_ctrl->last_updated_req % MAX_PER_FRAME_ARRAY));

	rc = 0;

free_mode_cfg:
	kfree(mode_cfg);
	return rc;
}

int cam_sensor_fsync_apply(struct cam_sensor_ctrl_t *s_ctrl,
		int64_t req_id, enum cam_cci_cmd_type cmd_opcode)
{
	uint32_t idx = req_id % MAX_PER_FRAME_ARRAY;
	struct cam_sensor_fsync_slot *slot;
	struct cam_sensor_cci_client *cci_client;
	int rc = 0;

	if (!s_ctrl->per_frame_fsync) {
		CAM_ERR(CAM_SENSOR, "Sensor[%s] per_frame_fsync not allocated",
			s_ctrl->sensor_name);
		return -EINVAL;
	}

	slot = &s_ctrl->per_frame_fsync[idx];

	if (!slot->is_valid || slot->request_id != req_id) {
		CAM_ERR(CAM_SENSOR,
			"Sensor[%s] no valid fsync slot for req_id=%lld "
			"(slot: valid=%d req_id=%lld)",
			s_ctrl->sensor_name, req_id,
			slot->is_valid, slot->request_id);
		return -EINVAL;
	}

	if (s_ctrl->io_master_info.master_type != CCI_MASTER ||
	    !s_ctrl->io_master_info.cci_client) {
		CAM_ERR(CAM_SENSOR,
			"Sensor[%s] GPIO queue only supported on CCI master",
			s_ctrl->sensor_name);
		rc = -EINVAL;
		goto clear;
	}

	cci_client = s_ctrl->io_master_info.cci_client;

	CAM_DBG(CAM_SENSOR, "Sensor[%s] fsync apply req_id=%lld num_queues=%u",
		s_ctrl->sensor_name, req_id, slot->num_queues);

	/*
	 * Pass cmd_buf[0] directly to the CCI layer via the cci_client
	 * staging field. This will be removed once cam_cci_load_gpio_queue()
	 * is updated to accept a cmd_buf pointer directly (item 6 of the
	 * fsync redesign).
	 */
	cci_client->cmd_buf = slot->cmd_buf[0];

	/*
	 * MSM_CCI_TIMER_FSYNC_INDEPENDENT now handles load + start +
	 * transient queue release in a single call.
	 */
	rc = cam_sensor_cci_i2c_util(&s_ctrl->io_master_info, cmd_opcode);
	if (rc < 0)
		CAM_ERR(CAM_SENSOR,
			"Sensor[%s] GPIO fsync failed rc=%d req_id=%lld",
			s_ctrl->sensor_name, rc, req_id);

clear:
	slot->is_valid = false;
	slot->request_id = 0;
	slot->num_queues = 0;

	return rc;
}
