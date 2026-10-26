/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017-2018,2020, The Linux Foundation. All rights reserved.
 *Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _CAM_SENSOR_CORE_H_
#define _CAM_SENSOR_CORE_H_

#include "cam_sensor_dev.h"

/**
 * @s_ctrl: Sensor ctrl structure
 *
 * This API powers up the camera sensor module
 */
int cam_sensor_power_up(struct cam_sensor_ctrl_t *s_ctrl);
/**
 * @s_ctrl:     Sensor ctrl structure
 * @csl_packet: CSL packet containing regulator settings
 * @cmd_desc:   Command buffer descriptor
 * @io_cfg:     IO configuration
 *
 * This API enables sensor regulator rails on behalf of AON sensor
 */
int cam_sensor_aon_regulator_on(struct cam_sensor_ctrl_t *s_ctrl,
	struct cam_packet *csl_packet,
	struct cam_cmd_buf_desc *cmd_desc,
	struct cam_buf_io_cfg *io_cfg);

/**
 * @s_ctrl:     Sensor ctrl structure
 * @csl_packet: CSL packet containing regulator settings
 * @cmd_desc:   Command buffer descriptor
 * @io_cfg:     IO configuration
 *
 * This API disables sensor regulator rails on behalf of AON sensor
 */
int cam_sensor_aon_regulator_off(struct cam_sensor_ctrl_t *s_ctrl,
	struct cam_packet *csl_packet,
	struct cam_cmd_buf_desc *cmd_desc,
	struct cam_buf_io_cfg *io_cfg);

/**
 * @s_ctrl: Sensor ctrl structure
 *
 * This API powers down the camera sensor module
 */
int cam_sensor_power_down(struct cam_sensor_ctrl_t *s_ctrl);

/**
 * @sd: V4L2 subdevice
 * @on: Turn off/on flag
 *
 * This API powers down the sensor module
 */
int cam_sensor_power(struct v4l2_subdev *sd, int on);

/**
 * @s_ctrl: Sensor ctrl structure
 * @req_id: Request id
 * @opcode: opcode for settings
 *
 * This API applies the req_id settings to sensor
 */
int cam_sensor_apply_settings(struct cam_sensor_ctrl_t *s_ctrl, int64_t req_id,
	enum cam_sensor_packet_opcodes opcode);

/**
 * @apply: Req mgr structure for applying request
 *
 * This API applies the request that is mentioned
 */
int cam_sensor_apply_request(struct cam_req_mgr_apply_request *apply);

/**
 * @apply: Req mgr structure for notifying frame skip
 *
 * This API notifies a frame is skipped
 */
int cam_sensor_notify_frame_skip(struct cam_req_mgr_apply_request *apply);

/**
 * @flush: Req mgr structure for flushing request
 *
 * This API flushes the request that is mentioned
 */
int cam_sensor_flush_request(struct cam_req_mgr_flush_request *flush);

/**
 * @info: Sub device info to req mgr
 *
 * Publish the subdevice info
 */
int cam_sensor_publish_dev_info(struct cam_req_mgr_device_info *info);

/**
 * @link: Link setup info
 *
 * This API establishes link with sensor subdevice with req mgr
 */
int cam_sensor_establish_link(struct cam_req_mgr_core_dev_link_setup *link);

/**
 * @evt_data: Event data info
 *
 * This API processes the event which is published by request mgr
 */
int cam_sensor_process_evt(struct cam_req_mgr_link_evt_data *evt_data);

/**
 * @s_ctrl: Sensor ctrl structure
 * @arg:    Camera control command argument
 *
 * This API handles the camera control argument reached to sensor
 */
int32_t cam_sensor_driver_cmd(struct cam_sensor_ctrl_t *s_ctrl, void *arg);

/**
 * @s_ctrl:              Sensor contributing this fsync configuration; the first
 *                       caller becomes the group's GPIO queue owner
 * @tpoint:              Trigger point stage of type enum
 *                       cci_timer_fsync_trigger_point at which to fire the
 *                       GPIO fsync (ACQUIRE or STREAM_ON)
 * @refcount_to_trigger: Number of sensors that must reach @tpoint before the
 *                       GPIO queue is started
 *
 * Stores the GPIO fsync trigger point and reference count decoded from a
 * SYNC_INFO blob into the shared trigger tracker. Called once per participant
 * from the fsync blob handler with identical parameters, so it is idempotent:
 * an already-tracked configuration is left untouched rather than rewinding the
 * count that earlier participants contributed.
 */
void cam_sensor_fsync_trigger_set(
	struct cam_sensor_ctrl_t *s_ctrl,
	uint32_t tpoint,
	uint32_t refcount_to_trigger);

/**
 * @s_ctrl: Sensor ctrl structure
 *
 * This API handles the camera sensor close/shutdown
 */
void cam_sensor_shutdown(struct cam_sensor_ctrl_t *s_ctrl);

#endif /* _CAM_SENSOR_CORE_H_ */
