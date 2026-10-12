/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _CAM_SENSOR_FSYNC_H_
#define _CAM_SENSOR_FSYNC_H_

#include <linux/types.h>
#include <media/cam_sensor.h>
#include "cam_cci_dev.h"

struct cam_sensor_ctrl_t;

/* Upper bound on cci_sync_info.config_size to cap the kernel allocation
 * used to stage the mode-specific payload copied in from config_ptr.
 */
#define CAM_SENSOR_FSYNC_MAX_CONFIG_SIZE 4096

/* Sentinel request id for fsync configurations that are not tied to any
 * per-frame request. Infinite mode is armed at a stage boundary (ACQUIRE /
 * STREAM_ON), where no request has been submitted yet, so it cannot be keyed
 * on a request id.
 */
#define CAM_SENSOR_FSYNC_REQ_ID_NONE (-1LL)

/**
 * struct cam_sensor_fsync_slot - GPIO fsync command buffer storage
 * @cmd_buf:    Pre-converted GPIO command buffers, one per queue needed
 * @num_queues: Number of GPIO queues needed for this request
 * @request_id: Request ID this slot belongs to, or CAM_SENSOR_FSYNC_REQ_ID_NONE
 *              for stage-triggered (infinite mode) configurations
 * @is_valid:   Slot contains a valid fsync configuration
 */
struct cam_sensor_fsync_slot {
	struct cam_cci_gpio_cmd_buf cmd_buf[GPIO_Q_MAX];
	uint8_t                     num_queues;
	int64_t                     request_id;
	bool                        is_valid;
};

/**
 * cam_sensor_fsync_handle_blob - Handle CAM_SENSOR_GENERIC_BLOB_SYNC_INFO blob
 * @blob_data: Raw pointer to blob payload
 * @blob_size: Byte length of the blob payload
 * @s_ctrl:    Sensor control structure to populate
 *
 * Decodes and validates a cci_sync_info blob received from userspace,
 * converts the timing schema to GPIO command buffers, and stores the
 * result. Infinite (stage-triggered) mode is stored in s_ctrl->fsync_infinite;
 * finite mode is stored in per_frame_fsync[] keyed on the request id.
 *
 * Returns: 0 on success, negative error code on failure
 */
int cam_sensor_fsync_handle_blob(uint8_t *blob_data, uint32_t blob_size,
	struct cam_sensor_ctrl_t *s_ctrl);

/**
 * cam_sensor_fsync_apply - Load and start the GPIO queue for a fsync config
 * @s_ctrl:      Sensor control structure
 * @req_id:      Request ID to apply, or CAM_SENSOR_FSYNC_REQ_ID_NONE to apply
 *               the stage-triggered (infinite mode) configuration
 * @cmd_opcode:  CCI opcode used to program the queue
 *
 * Loads the pre-converted GPIO command buffer into the CCI hardware GPIO
 * queue, then starts the queue.
 *
 * Returns: 0 on success, negative error code on failure
 */
int cam_sensor_fsync_apply(struct cam_sensor_ctrl_t *s_ctrl,
		int64_t req_id, enum cam_cci_cmd_type cmd_opcode);

/**
 * cam_sensor_fsync_reset - Drop this sensor's stage-triggered fsync config
 * @s_ctrl: Sensor control structure
 *
 * Invalidates the infinite-mode fsync slot and the decoded-blob flag. Called
 * when the sensor leaves a state in which the cached configuration is still
 * meaningful (release / shutdown).
 */
void cam_sensor_fsync_reset(struct cam_sensor_ctrl_t *s_ctrl);

#endif /* _CAM_SENSOR_FSYNC_H_ */
