/* SPDX-License-Identifier: GPL-2.0-only WITH Linux-syscall-note */
/*
 * Copyright (c) 2016-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __UAPI_CAM_SENSOR_H__
#define __UAPI_CAM_SENSOR_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#include <media/cam_defs.h>

#define CAM_SENSOR_PROBE_CMD      (CAM_COMMON_OPCODE_MAX + 1)
#define CAM_FLASH_MAX_LED_TRIGGERS 2
#define MAX_OIS_NAME_SIZE 32
#define MAX_OIS_FW_COUNT  2
#define CAM_CSIPHY_SECURE_MODE_ENABLED 1
#define CAM_SENSOR_NAME_MAX_SIZE 32

#define SKEW_CAL_MASK             BIT(1)
#define PREAMBLE_PATTEN_CAL_MASK  BIT(2)

/* CSIPHY driver cmd buffer meta types */
#define CAM_CSIPHY_PACKET_META_LANE_INFO           0
#define CAM_CSIPHY_PACKET_META_GENERIC_BLOB        1
#define CAM_CSIPHY_PACKET_META_LANE_INFO_V2        2

/* CSIPHY blob types */
#define CAM_CSIPHY_GENERIC_BLOB_TYPE_CDR_CONFIG    0
#define CAM_CSIPHY_GENERIC_BLOB_TYPE_AUX_CONFIG    1

/* CSIPHY CDR tolerance operations */
#define CAM_CSIPHY_CDR_ADD_TOLERANCE               1
#define CAM_CSIPHY_CDR_SUB_TOLERANCE               2

/* SENSOR driver cmd buffer meta types */
#define CAM_SENSOR_PACKET_I2C_COMMANDS             0
#define CAM_SENSOR_PACKET_GENERIC_BLOB             1
#define CAM_SENSOR_GENERIC_BLOB_MODESWITCHPD_INFO  2
#define CAM_SENSOR_GENERIC_BLOB_SYNC_INFO          3

/* SENSOR blob types */
#define CAM_SENSOR_GENERIC_BLOB_RES_INFO           0

#define CAM_SENSOR_GET_QUERY_CAP_V2

enum camera_sensor_cmd_type {
	CAMERA_SENSOR_CMD_TYPE_INVALID,
	CAMERA_SENSOR_CMD_TYPE_PROBE,
	CAMERA_SENSOR_CMD_TYPE_PWR_UP,
	CAMERA_SENSOR_CMD_TYPE_PWR_DOWN,
	CAMERA_SENSOR_CMD_TYPE_I2C_INFO,
	CAMERA_SENSOR_CMD_TYPE_I2C_RNDM_WR,
	CAMERA_SENSOR_CMD_TYPE_I2C_RNDM_RD,
	CAMERA_SENSOR_CMD_TYPE_I2C_CONT_WR,
	CAMERA_SENSOR_CMD_TYPE_I2C_CONT_RD,
	CAMERA_SENSOR_CMD_TYPE_WAIT,
	CAMERA_SENSOR_FLASH_CMD_TYPE_INIT_INFO,
	CAMERA_SENSOR_FLASH_CMD_TYPE_FIRE,
	CAMERA_SENSOR_FLASH_CMD_TYPE_RER,
	CAMERA_SENSOR_FLASH_CMD_TYPE_QUERYCURR,
	CAMERA_SENSOR_FLASH_CMD_TYPE_WIDGET,
	CAMERA_SENSOR_CMD_TYPE_RD_DATA,
	CAMERA_SENSOR_FLASH_CMD_TYPE_INIT_FIRE,
	CAMERA_SENSOR_OIS_CMD_TYPE_FW_INFO,
	CAMERA_SENSOR_CMD_TYPE_RES_INFO,
	CAMERA_SENSOR_CMD_TYPE_I2C_RD_APPEND_WR,
	CAMERA_SENSOR_CMD_TYPE_I2C_SEQUENTIAL_XFER_LOCK,
	CAMERA_SENSOR_CMD_TYPE_I2C_SEQUENTIAL_XFER_UNLOCK,
	CAMERA_SENSOR_CMD_TYPE_MAX,
};

enum cam_actuator_packet_opcodes {
	CAM_ACTUATOR_PACKET_OPCODE_INIT,
	CAM_ACTUATOR_PACKET_AUTO_MOVE_LENS,
	CAM_ACTUATOR_PACKET_MANUAL_MOVE_LENS,
	CAM_ACTUATOR_PACKET_OPCODE_READ,
	CAM_ACTUATOR_PACKET_NOP_OPCODE = 127
};

enum cam_eeprom_packet_opcodes {
	CAM_EEPROM_PACKET_OPCODE_INIT,
	CAM_EEPROM_WRITE
};

enum cam_ois_packet_opcodes {
	CAM_OIS_PACKET_OPCODE_INIT,
	CAM_OIS_PACKET_OPCODE_OIS_CONTROL,
	CAM_OIS_PACKET_OPCODE_READ,
	CAM_OIS_PACKET_OPCODE_WRITE_TIME
};

enum camera_sensor_i2c_op_code {
	CAMERA_SENSOR_I2C_OP_INVALID,
	CAMERA_SENSOR_I2C_OP_RNDM_WR,
	CAMERA_SENSOR_I2C_OP_RNDM_WR_VERF,
	CAMERA_SENSOR_I2C_OP_CONT_WR_BRST,
	CAMERA_SENSOR_I2C_OP_CONT_WR_BRST_VERF,
	CAMERA_SENSOR_I2C_OP_CONT_WR_SEQN,
	CAMERA_SENSOR_I2C_OP_CONT_WR_SEQN_VERF,
	CAMERA_SENSOR_I2C_OP_RNDM_RD,
	CAMERA_SENSOR_I2C_OP_CONT_RD,
	CAMERA_SENSOR_I2C_OP_RD_APPEND_WR,
	CAMERA_SENSOR_I2C_OP_SEQUENTIAL_XFER_LOCK,
	CAMERA_SENSOR_I2C_OP_SEQUENTIAL_XFER_UNLOCK,
	CAMERA_SENSOR_I2C_OP_MAX,
};

enum camera_sensor_wait_op_code {
	CAMERA_SENSOR_WAIT_OP_INVALID,
	CAMERA_SENSOR_WAIT_OP_COND,
	CAMERA_SENSOR_WAIT_OP_HW_UCND,
	CAMERA_SENSOR_WAIT_OP_SW_UCND,
	CAMERA_SENSOR_WAIT_OP_MAX,
};

enum cam_tpg_packet_opcodes {
	CAM_TPG_PACKET_OPCODE_INVALID = 0,
	CAM_TPG_PACKET_OPCODE_INITIAL_CONFIG,
	CAM_TPG_PACKET_OPCODE_NOP,
	CAM_TPG_PACKET_OPCODE_UPDATE,
	CAM_TPG_PACKET_OPCODE_MAX,
};

enum cam_sensor_packet_opcodes {
	CAM_SENSOR_PACKET_OPCODE_SENSOR_STREAMON,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_UPDATE,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_INITIAL_CONFIG,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_PROBE,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_CONFIG,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_STREAMOFF,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_READ,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_FRAME_SKIP_UPDATE,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_PROBE_V2,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_REG_BANK_UNLOCK  = 9,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_REG_BANK_LOCK    = 10,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_BUBBLE_UPDATE    = 11,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_REGULATOR_ON     = 12,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_REGULATOR_OFF    = 13,
	CAM_SENSOR_PACKET_OPCODE_AON_PARTIAL_BRIDGE_ENABLE  = 14,
	CAM_SENSOR_PACKET_OPCODE_AON_PARTIAL_BRIDGE_DISABLE = 15,
	CAM_SENSOR_PACKET_OPCODE_SENSOR_NOP = 127,
};

enum cam_endianness_type {
	CAM_ENDIANNESS_BIG,
	CAM_ENDIANNESS_LITTLE,
};

enum tpg_command_type_t {
	TPG_CMD_TYPE_INVALID = 0,
	TPG_CMD_TYPE_GLOBAL_CONFIG,
	TPG_CMD_TYPE_STREAM_CONFIG,
	TPG_CMD_TYPE_ILLUMINATION_CONFIG,
	TPG_CMD_TYPE_SETTINGS_CONFIG,
	TPG_CMD_TYPE_MAX,
};

enum tpg_pattern_t {
	TPG_PATTERN_INVALID = 0,
	TPG_PATTERN_REAL_IMAGE,
	TPG_PATTERN_RANDOM_PIXL,
	TPG_PATTERN_RANDOM_INCREMENTING_PIXEL,
	TPG_PATTERN_COLOR_BAR,
	TPG_PATTERN_ALTERNATING_55_AA,
	TPG_PATTERN_ALTERNATING_USER_DEFINED,
	TPG_PATTERN_MAX,
};

enum tpg_color_bar_mode_t {
	TPG_COLOR_BAR_MODE_INVALID = 0,
	TPG_COLOR_BAR_MODE_NORMAL,
	TPG_COLOR_BAR_MODE_SPLIT,
	TPG_COLOR_BAR_MODE_ROTATING,
	TPG_COLOR_BAR_MODE_MAX,
};

enum tpg_image_format_t {
	TPG_IMAGE_FORMAT_INVALID = 0,
	TPG_IMAGE_FORMAT_BAYER,
	TPG_IMAGE_FORMAT_QCFA,
	TPG_IMAGE_FORMAT_YUV,
	TPG_IMAGE_FORMAT_JPEG,
	TPG_IMAGE_FORMAT_MAX,
};

enum tpg_phy_type_t {
	TPG_PHY_TYPE_INVALID = 0,
	TPG_PHY_TYPE_DPHY,
	TPG_PHY_TYPE_CPHY,
	TPG_PHY_TYPE_MAX,
};

enum tpg_pixel_type_t {
	TPG_PIXEL_TYPE_INVALID = 0,
	TPG_PIXEL_TYPE_RED,
	TPG_PIXEL_TYPE_GREEN,
	TPG_PIXEL_TYPE_BLUE,
	TPG_PIXEL_TYPE_IR,
	TPG_PIXEL_TYPE_MONO,
};

enum tpg_exposure_type_t {
	TPG_EXPOSUREL_TYPE_INVALID = 0,
	TPG_EXPOSURE_TYPE_LONG,
	TPG_EXPOSURE_TYPE_MIDDLE,
	TPG_EXPOSURE_TYPE_SHORT,
};

enum xcfa_type_t {
	XCFA_TYPE_BAYER = 0,
	XCFA_TYPE_QUADCFA,
	XCFA_TYPE_THREEXTHREECFA,
	XCFA_TYPE_FOURXFOURCFA,
	XCFA_TYPE_RGBIR,
	XCFA_TYPE_RGBWC,
	XCFA_TYPE_RGBWK,
	XCFA_TYPE_UNCONVENTIONAL_BAYER,
};

enum tpg_interleaving_format_t {
	TPG_INTERLEAVING_FORMAT_INVALID = 0,
	TPG_INTERLEAVING_FORMAT_FRAME,
	TPG_INTERLEAVING_FORMAT_LINE,
	TPG_INTERLEAVING_FORMAT_SHDR,
	TPG_INTERLEAVING_FORMAT_SPARSE_PD,
	TPG_INTERLEAVING_FORMAT_MAX,
};

enum tpg_shutter_t {
	TPG_SHUTTER_TYPE_INVALID = 0,
	TPG_SHUTTER_TYPE_ROLLING,
	TPG_SHUTTER_TYPE_GLOBAL,
	TPG_SHUTTER_TYPE_MAX,
};

enum tpg_stream_t {
	TPG_STREAM_TYPE_INVALID = 0,
	TPG_STREAM_TYPE_IMAGE,
	TPG_STREAM_TYPE_PDAF,
	TPG_STREAM_TYPE_META,
	TPG_STREAM_TYPE_MAX,
};

enum tpg_cfa_arrangement_t {
	TPG_CFA_ARRANGEMENT_TYPE_INVALID = 0,
	TPG_CFA_ARRANGEMENT_TYPE_MAX,
};

/**
 * struct cam_sensor_query_cap - capabilities info for sensor
 *
 * @slot_info        :  Indicates about the slotId or cell Index
 * @secure_camera    :  Camera is in secure/Non-secure mode
 * @pos_pitch        :  Sensor position pitch
 * @pos_roll         :  Sensor position roll
 * @pos_yaw          :  Sensor position yaw
 * @actuator_slot_id :  Actuator slot id which connected to sensor
 * @eeprom_slot_id   :  EEPROM slot id which connected to sensor
 * @ois_slot_id      :  OIS slot id which connected to sensor
 * @flash_slot_id    :  Flash slot id which connected to sensor
 * @csiphy_slot_id   :  CSIphy slot id which connected to sensor
 *
 */
struct  cam_sensor_query_cap {
	__u32        slot_info;
	__u32        secure_camera;
	__u32        pos_pitch;
	__u32        pos_roll;
	__u32        pos_yaw;
	__u32        actuator_slot_id;
	__u32        eeprom_slot_id;
	__u32        ois_slot_id;
	__u32        flash_slot_id;
	__u32        csiphy_slot_id;
} __attribute__((packed));

/**
 * struct cam_csiphy_query_cap - capabilities info for csiphy
 *
 * @slot_info        :  Indicates about the slotId or cell Index
 * @version          :  CSIphy version
 * @clk lane         :  Of the 5 lanes, informs lane configured
 *                      as clock lane
 * @reserved
 */
struct cam_csiphy_query_cap {
	__u32            slot_info;
	__u32            version;
	__u32            clk_lane;
	__u32            reserved;
} __attribute__((packed));

/**
 * struct cam_actuator_query_cap - capabilities info for actuator
 *
 * @slot_info        :  Indicates about the slotId or cell Index
 * @reserved
 */
struct cam_actuator_query_cap {
	__u32            slot_info;
	__u32            reserved;
} __attribute__((packed));

/**
 * struct cam_eeprom_query_cap_t - capabilities info for eeprom
 *
 * @slot_info                  :  Indicates about the slotId or cell Index
 * @eeprom_kernel_probe        :  Indicates about the kernel or userspace probe
 */
struct cam_eeprom_query_cap_t {
	__u32            slot_info;
	__u16            eeprom_kernel_probe;
	__u16            is_multimodule_mode;
} __attribute__((packed));

/**
 * struct cam_ois_query_cap_t - capabilities info for ois
 *
 * @slot_info                  :  Indicates about the slotId or cell Index
 */
struct cam_ois_query_cap_t {
	__u32            slot_info;
	__u16            reserved;
} __attribute__((packed));

/**
 * struct cam_tpg_query_cap - capabilities info for tpg
 *
 * @slot_info        :  Indicates about the slotId or cell Index
 * @version          :  TPG version , in msb
 * @reserved         :  Reserved for future Use
 * @secure_camera    :  Camera is in secure/Non-secure mode
 * @csiphy_slot_id   :  CSIphy slot id which connected to sensor
 */
struct cam_tpg_query_cap {
	__u32        slot_info;
	__u32        version;
	__u32        secure_camera;
	__u32        csiphy_slot_id;
	__u32        reserved[2];
} __attribute__((packed));


/**
 * struct cam_cmd_i2c_info - Contains slave I2C related info
 *
 * @slave_addr      :    Slave address
 * @i2c_freq_mode   :    4 bits are used for I2c freq mode
 * @cmd_type        :    Explains type of command
 */
struct cam_cmd_i2c_info {
	__u32    slave_addr;
	__u8     i2c_freq_mode;
	__u8     cmd_type;
	__u16    reserved;
} __attribute__((packed));

/**
 * Below macro definition is the param mask for
 * cam_cmd_sensor_res_info.
 */
#define CAM_SENSOR_FEATURE_MASK                    BIT(0)
#define CAM_SENSOR_NUM_BATCHED_FRAMES              BIT(1)

/* Below macro definition is the sub definition for CAM_SENSOR_FEATURE_MASK */
#define CAM_SENSOR_FEATURE_NONE                    0
#define CAM_SENSOR_FEATURE_AEB_ON                  BIT(0)
#define CAM_SENSOR_FEATURE_AEB_UPDATE              BIT(1)
#define CAM_SENSOR_FEATURE_AEB_OFF                 BIT(2)
#define CAM_SENSOR_FEATURE_INSENSOR_HDR_3EXP_ON    BIT(3)
#define CAM_SENSOR_FEATURE_INSENSOR_HDR_3EXP_OFF   BIT(4)

/**
 * struct cam_cmd_sensor_res_info - Contains sensor res info
 *
 * res_index is the key property, it specifies the
 * combinations of other properties enclosed in this
 * structure.
 *
 * @res_index        : The resolution index that gets updated
 *                     during a mode switch
 * @fps              : Frame rate
 * @width            : Pixel width to output to csiphy
 * @height           : Pixel height to output to csiphy
 * @caps             : Specifies capability sensor is configured
 *                     for, (eg, XCFA, HFR), num_exposures and
 *                     PDAF type
 * @num_valid_params : Number of valid params
 * @valid_param_mask : Valid param mask
 * @params           : params
 */
struct cam_sensor_res_info {
	__u16 res_index;
	__u32 fps;
	__u32 width;
	__u32 height;
	char  caps[64];
	__u32 num_valid_params;
	__u32 valid_param_mask;
	__u16 params[3];
} __attribute__((packed));

/**
 * struct cam_ois_opcode - Contains OIS opcode
 *
 * @prog            :    OIS FW prog register address
 * @coeff           :    OIS FW coeff register address
 * @pheripheral     :    OIS pheripheral
 * @memory          :    OIS memory
 */
struct cam_ois_opcode {
	__u32 prog;
	__u32 coeff;
	__u32 pheripheral;
	__u32 memory;
} __attribute__((packed));

/**
 * struct cam_cmd_ois_info - Contains OIS slave info
 *
 * @slave_addr            :    OIS i2c slave address
 * @i2c_freq_mode         :    i2c frequency mode
 * @cmd_type              :    Explains type of command
 * @ois_fw_flag           :    indicates if fw is present or not
 * @is_ois_calib          :    indicates the calibration data is available
 * @ois_name              :    OIS name
 * @opcode                :    opcode
 */
struct cam_cmd_ois_info {
	__u32                 slave_addr;
	__u8                  i2c_freq_mode;
	__u8                  cmd_type;
	__u8                  ois_fw_flag;
	__u8                  is_ois_calib;
	char                  ois_name[MAX_OIS_NAME_SIZE];
	struct cam_ois_opcode opcode;
} __attribute__((packed));


/**
 * struct cam_cmd_ois_fw_param - Contains OIS firmware param
 *
 * NOTE: if this struct is updated,
 * please also update version in struct cam_cmd_ois_fw_info
 *
 * @fw_name         :       firmware file name
 * @fw_start_pos    :       data start position in file
 * @fw_size         :       firmware size
 * @fw_len_per_write:       data length per write in bytes
 * @fw_addr_type    :       addr type
 * @fw_data_type    :       data type
 * @fw_operation    :       type of operation
 * @reserved        :       reserved for 32-bit alignment
 * @fw_delayUs      :       delay in cci write
 * @fw_reg_addr     :       start register addr to write
 * @fw_init_size    :       size of fw download init settings
 * @fw_finalize_size:       size of fw download finalize settings
 */
struct cam_cmd_ois_fw_param {
	char        fw_name[MAX_OIS_NAME_SIZE];
	__u32       fw_start_pos;
	__u32       fw_size;
	__u32       fw_len_per_write;
	__u8        fw_addr_type;
	__u8        fw_data_type;
	__u8        fw_operation;
	__u8        reserved;
	__u32       fw_delayUs;
	__u32       fw_reg_addr;
	__u32       fw_init_size;
	__u32       fw_finalize_size;
} __attribute__((packed));

/**
 * struct cam_cmd_ois_fw_info - Contains OIS firmware info
 *
 * @version         :       version info
 *                          NOTE: if struct cam_cmd_ois_fw_param is updated,
 *                          version here needs to be updated too.
 * @reserved        :       reserved
 * @cmd_type        :       Explains type of command
 * @fw_count        :       firmware count
 * @endianness      :       endianness combo:
 *                          bit[3:0] firmware data's endianness
 *                          bit[7:4] endian type of input parameter to ois driver, say QTime
 * @fw_param        :       includes firmware parameters
 * @num_valid_params:       Number of valid params
 * @param_mask      :       Mask to indicate fields in params
 * @params          :       Additional Params
 */
struct cam_cmd_ois_fw_info {
	__u32                           version;
	__u8                            reserved;
	__u8                            cmd_type;
	__u8                            fw_count;
	__u8                            endianness;
	struct cam_cmd_ois_fw_param     fw_param[MAX_OIS_FW_COUNT];
	__u32                           num_valid_params;
	__u32                           param_mask;
	__u32                           params[4];
} __attribute__((packed));

/**
 * struct cam_cmd_probe - Contains sensor slave info
 *
 * @data_type       :   Slave register data type
 * @addr_type       :   Slave register address type
 * @op_code         :   Don't Care
 * @cmd_type        :   Explains type of command
 * @reg_addr        :   Slave register address
 * @expected_data   :   Data expected at slave register address
 * @data_mask       :   Data mask if only few bits are valid
 * @camera_id       :   Indicates the slot to which camera
 *                      needs to be probed
 * @reserved
 */
struct cam_cmd_probe {
	__u8     data_type;
	__u8     addr_type;
	__u8     op_code;
	__u8     cmd_type;
	__u32    reg_addr;
	__u32    expected_data;
	__u32    data_mask;
	__u16    camera_id;
	__u16    reserved;
} __attribute__((packed));

/**
 * struct cam_cmd_probe_v2 - Contains sensor slave info version 2
 *
 * @data_type         :   Slave register data type
 * @addr_type         :   Slave register address type
 * @op_code           :   Don't Care
 * @cmd_type          :   Explains type of command
 * @reg_addr          :   Slave register address
 * @expected_data     :   Data expected at slave register address
 * @data_mask         :   Data mask if only few bits are valid
 * @camera_id         :   Indicates the slot to which camera
 *                      needs to be probed
 * @pipeline_delay    :   Pipeline delay
 * @logical_camera_id :   Logical Camera ID
 * @sensor_name       :   Sensor's name
 * @reserved
 */
struct cam_cmd_probe_v2 {
	__u8     data_type;
	__u8     addr_type;
	__u8     op_code;
	__u8     cmd_type;
	__u32    reg_addr;
	__u32    expected_data;
	__u32    data_mask;
	__u16    camera_id;
	__u16    pipeline_delay;
	__u32    logical_camera_id;
	char     sensor_name[CAM_SENSOR_NAME_MAX_SIZE];
	__u32    reserved[4];
} __attribute__((packed));

/**
 * struct cam_power_settings - Contains sensor power setting info
 *
 * @power_seq_type  :   Type of power sequence
 * @reserved
 * @config_val_low  :   Lower 32 bit value configuration value
 * @config_val_high :   Higher 32 bit value configuration value
 *
 */
struct cam_power_settings {
	__u16    power_seq_type;
	__u16    reserved;
	__u32    config_val_low;
	__u32    config_val_high;
} __attribute__((packed));

/**
 * struct cam_cmd_power - Explains about the power settings
 *
 * @count           :    Number of power settings follows
 * @reserved
 * @cmd_type        :    Explains type of command
 * @power_settings  :    Contains power setting info
 */
struct cam_cmd_power {
	__u32                       count;
	__u8                        reserved;
	__u8                        cmd_type;
	__u16                       more_reserved;
	union {
		struct cam_power_settings   power_settings[1];
		__DECLARE_FLEX_ARRAY(struct cam_power_settings, power_settings_flex);
	};
} __attribute__((packed));

/**
 * struct i2c_rdwr_header - header of READ/WRITE I2C command
 *
 * @ count           :   Number of registers / data / reg-data pairs
 * @ op_code         :   Operation code
 * @ cmd_type        :   Command buffer type
 * @ data_type       :   I2C data type
 * @ addr_type       :   I2C address type
 * @ reserved
 */
struct i2c_rdwr_header {
	__u32    count;
	__u8     op_code;
	__u8     cmd_type;
	__u8     data_type;
	__u8     addr_type;
} __attribute__((packed));

/**
 * struct i2c_random_wr_payload - payload for I2C random write
 *
 * @ reg_addr        :   Register address
 * @ reg_data        :   Register data
 * @ mask            :   mask value
 *
 */
struct i2c_random_wr_payload {
	__u32     reg_addr;
	__u32     reg_data;
	__u32     mask;
} __attribute__((packed));

/**
 * struct cam_cmd_i2c_random_wr - I2C random write command
 * @ header            :   header of READ/WRITE I2C command
 * @ random_wr_payload :   payload for I2C random write
 */
struct cam_cmd_i2c_random_wr {
	struct i2c_rdwr_header       header;
	union {
		struct i2c_random_wr_payload random_wr_payload[1];
		__DECLARE_FLEX_ARRAY(struct i2c_random_wr_payload, random_wr_payload_flex);
	};
} __attribute__((packed));

/**
 * struct cam_cmd_read - I2C read command
 * @ reg_data        :   Register data
 * @ reserved
 */
struct cam_cmd_read {
	__u32                reg_data;
	__u32                reserved;
} __attribute__((packed));

/**
 * struct cam_cmd_i2c_continuous_wr - I2C continuous write command
 * @ header          :   header of READ/WRITE I2C command
 * @ reg_addr        :   Register address
 * @ data_read       :   I2C read command
 */
struct cam_cmd_i2c_continuous_wr {
	struct i2c_rdwr_header header;
	__u32                  reg_addr;
	union {
		struct cam_cmd_read    data_read[1];
		__DECLARE_FLEX_ARRAY(struct cam_cmd_read, data_read_flex);
	};
} __attribute__((packed));

/**
 * struct cam_cmd_i2c_random_rd - I2C random read command
 * @ header          :   header of READ/WRITE I2C command
 * @ data_read       :   I2C read command
 */
struct cam_cmd_i2c_random_rd {
	struct i2c_rdwr_header header;
	union {
		struct cam_cmd_read    data_read[1];
		__DECLARE_FLEX_ARRAY(struct cam_cmd_read, data_read_flex);
	};
} __attribute__((packed));

/**
 * struct cam_cmd_i2c_continuous_rd - I2C continuous continuous read command
 * @ header          :   header of READ/WRITE I2C command
 * @ reg_addr        :   Register address
 *
 */
struct cam_cmd_i2c_continuous_rd {
	struct i2c_rdwr_header header;
	__u32                  reg_addr;
} __attribute__((packed));

/**
 * struct cam_cmd_i2c_continuous_rd - I2C continuous continuous read command
 * @ header          :   header of READ/WRITE I2C command
 * @ reserved        :
 * @ op_code         :   Opcode
 * @ cmd_type        :   Explains type of command
 * @ lock            :   lock or unlock
 *
 */
struct cam_cmd_i2c_sequential_xfer {
	struct i2c_rdwr_header header;
	__u16    reserved;
	__u8     op_code;
	__u8     cmd_type;
	__u32    lock;
} __attribute__((packed));

/**
 * struct cam_cmd_conditional_wait - Conditional wait command
 * @data_type       :   Data type
 * @addr_type       :   Address type
 * @op_code         :   Opcode
 * @cmd_type        :   Explains type of command
 * @timeout         :   Timeout for retries
 * @reserved
 * @reg_addr        :   Register Address
 * @reg_data        :   Register data
 * @data_mask       :   Data mask if only few bits are valid
 * @camera_id       :   Indicates the slot to which camera
 *                      needs to be probed
 *
 */
struct cam_cmd_conditional_wait {
	__u8     data_type;
	__u8     addr_type;
	__u16    reserved;
	__u8     op_code;
	__u8     cmd_type;
	__u16    timeout;
	__u32    reg_addr;
	__u32    reg_data;
	__u32    data_mask;
} __attribute__((packed));

/**
 * struct cam_cmd_unconditional_wait - Un-conditional wait command
 * @delay           :   Delay
 * @op_code         :   Opcode
 * @cmd_type        :   Explains type of command
 */
struct cam_cmd_unconditional_wait {
	__s16    delay;
	__s16    reserved;
	__u8     op_code;
	__u8     cmd_type;
	__u16    reserved1;
} __attribute__((packed));

/**
 * cam_csiphy_cdr_sweep_params : Provides cdr blob structre
 *
 * @cdr_tolerance        : CDR tolerance param
 * @tolerance_op_type    : Determines if the tolerance needs to be added/subtracted
 *                         from default CDR value
 * @configured_cdr       : Configured CDR value for all the lanes for the
 *                         selected data rate, default +/- tolerance,
 *                         this is the output
 * @num_valid_params     : Number of valid params
 * @valid_param_mask     : Valid param mask
 * @params               : params
 *
 */
struct cam_csiphy_cdr_sweep_params {
	__u32 cdr_tolerance;
	__u32 tolerance_op_type;
	__u32 configured_cdr;
	__u32 num_valid_params;
	__u32 valid_param_mask;
	__u32 params[3];
};

/**
 * cam_csiphy_aux_settings_params : Provides aux blob structre
 *
 * @data_rate_aux_mask : Auxiliary settings update for different data rates,
 *                       this is the output
 * @num_valid_params   : Number of valid params
 * @valid_param_mask   : Valid param mask
 * @params             : params
 *
 */
struct cam_csiphy_aux_settings_params {
	__u64 data_rate_aux_mask;
	__u32 num_valid_params;
	__u32 valid_param_mask;
	__u32 params[2];
};

/**
 * cam_csiphy_info       : Provides cmdbuffer structre
 * @lane_assign          : Lane sensor will be using
 * @mipi_flags           : Phy flags for different calibration operations
 * @lane_cnt             : Total number of lanes
 * @secure_mode          : Secure mode flag to enable / disable
 * @settle_time          : Settling time in ms
 * @data_rate            : Data rate
 *
 */
struct cam_csiphy_info {
	__u16    reserved;
	__u16    lane_assign;
	__u16    mipi_flags;
	__u8     lane_cnt;
	__u8     secure_mode;
	__u64    settle_time;
	__u64    data_rate;
} __attribute__((packed));

/**
 * cam_csiphy_info_v2    : Provides cmdbuffer structre
 * @version              : Version number
 * @lane_assign          : Lane sensor will be using
 * @mipi_flags           : Phy flags for different calibration operations
 * @lane_cnt             : Total number of lanes
 * @secure_mode          : Secure mode flag to enable / disable
 * @settle_time          : Settling time in ms
 * @data_rate            : Data rate
 * @channel_type         : Channel type indicates apply which channel settings
 * @num_valid_params     : Number of valid params
 * @param_mask           : Mask to indicate what the parameters are
 * @params               : Additional params
 */
struct cam_csiphy_info_v2 {
	__u16    version;
	__u16    lane_assign;
	__u16    mipi_flags;
	__u8     lane_cnt;
	__u8     secure_mode;
	__u64    settle_time;
	__u64    data_rate;
	__u32    channel_type;
	__u32    num_vaild_params;
	__u32    param_mask;
	__u32    params[5];
} __attribute__((packed));

/**
 * cam_csiphy_acquire_dev_info : Information needed for
 *                               csiphy at the time of acquire
 * @combo_mode                 : Indicates the device mode of operation
 * @cphy_dphy_combo_mode       : Info regarding cphy_dphy_combo mode
 * @csiphy_3phase              : Details whether 3Phase / 2Phase operation
 * @reserve
 *
 */
struct cam_csiphy_acquire_dev_info {
	__u32    combo_mode;
	__u16    cphy_dphy_combo_mode;
	__u8     csiphy_3phase;
	__u8     reserve;
} __attribute__((packed));

/**
 * cam_sensor_acquire_dev : Updates sensor acuire cmd
 * @device_handle  :    Updates device handle
 * @session_handle :    Session handle for acquiring device
 * @handle_type    :    Resource handle type
 * @reserved
 * @info_handle    :    Handle to additional info
 *                      needed for sensor sub modules
 *
 */
struct cam_sensor_acquire_dev {
	__u32    session_handle;
	__u32    device_handle;
	__u32    handle_type;
	__u32    reserved;
	__u64    info_handle;
} __attribute__((packed));

/**
 * cam_tpg_acquire_dev : Updates tpg acuire cmd
 * @device_handle  :    Updates device handle
 * @session_handle :    Session handle for acquiring device
 * @handle_type    :    Resource handle type
 * @reserved
 * @info_handle    :    Handle to additional info
 *                      needed for sensor sub modules
 */
struct cam_tpg_acquire_dev {
	__u32    session_handle;
	__u32    device_handle;
	__u32    handle_type;
	__u32    reserved;
	__u64    info_handle;
} __attribute__((packed));

/**
 * cam_sensor_streamon_dev : StreamOn command for the sensor
 * @session_handle :    Session handle for acquiring device
 * @device_handle  :    Updates device handle
 * @handle_type    :    Resource handle type
 * @reserved
 * @info_handle    :    Information Needed at the time of streamOn
 *
 */
struct cam_sensor_streamon_dev {
	__u32    session_handle;
	__u32    device_handle;
	__u32    handle_type;
	__u32    reserved;
	__u64    info_handle;
} __attribute__((packed));


/**
 * stream_dimension : Stream dimension
 *
 * @left   : left pixel locaiton of stream
 * @top    : top  pixel location of stream
 * @width  : width of the image stream
 * @height : Height of the image stream
 */
struct stream_dimension {
	__u32 left;
	__u32 top;
	__u32 width;
	__u32 height;
};

/**
 * tpg_command_header_t : tpg command common header
 *
 * @cmd_type    : command type
 * @size        : size of the command including header
 * @cmd_version : version of the command associated
 */
struct tpg_command_header_t {
	__u32 cmd_type;
	__s64 size;
	__u32 cmd_version;
} __attribute__((packed));

/**
 * tpg_pixel_coordinate_t : pixel coordinate structure
 *
 * @xcoordinate           : X coordinate
 * @ycoordinate           : Y coordiante
 * @pixel_type            : red green blue ir mono
 */
struct tpg_pixel_coordinate_t {
	__u32 xcoordinate;
	__u32 ycoordinate;
	__u32 exposure_type;
	__u32 pixel_type;
} __attribute__((packed));

/**
 * tpg_cfa_information_t      : tpg cfa information structure
 *
 * @number_of_pixel_per_color : number of pixel per color
 * @pattern_width             : pattern width
 * @pattern_height            : pattern height
 * @pixel_coordinate_count    : pixel coordinate count
 * @pixel_coordinate          : pixel coordinate array
 */
struct tpg_cfa_information_t {
	__u32 number_of_pixel_per_color;
	__u32 pattern_width;
	__u32 pattern_height;
	__u32 pixel_coordinate_count;
	struct tpg_pixel_coordinate_t pixel_coordinate[64];
} __attribute__((packed));

/**
 * tpg_reg_settings : TPG register settings
 *
 * @reg_offset : register offset
 * @reg_value  : register value
 * @operation  : operation
 * @delay_us   : delay in micro second
 */
struct tpg_reg_settings {
	__u32 reg_offset;
	__u32 reg_value;
	__u32 operation;
	__u32 delay_us;
	__u32 reserved[4];
} __attribute__((packed));

/**
 * tpg_settings_config_t : settings configuration command structure
 *
 * @header                : common header
 * @settings_array_offset : settings array offset
 * @settings_array_size   : settings array size
 * @active_count          : active count
 * @param_mask            : Mask to indicate fields in params
 * @params                : Additional Params
 */
struct tpg_settings_config_t {
	struct tpg_command_header_t header;
	__u32 settings_array_offset;
	__u32 settings_array_size;
	__u32 active_count;
	__u32 param_mask;
	__u32 params[4];
} __attribute__((packed));

/**
 * tpg_global_config_t : global configuration command structure
 *
 * @header              : common header
 * @phy_type            : phy type , cpy , dphy
 * @lane_count          : number of lanes used
 * @interleaving_format : interleaving format used
 * @phy_mode            : phy mode of operation
 * @shutter_type        : shutter type
 * @mode                : if any specific mode needs to configured
 * @hbi                 : horizontal blanking intervel
 * @vbi                 : vertical blanking intervel
 * @skip_pattern        : frame skip pattern
 * @tpg_clock           : tpg clock
 * @reserved            : reserved for future use
 */
struct tpg_global_config_t {
	struct tpg_command_header_t header;
	enum tpg_phy_type_t phy_type;
	__u8 lane_count;
	enum tpg_interleaving_format_t interleaving_format;
	__u8 phy_mode;
	enum tpg_shutter_t shutter_type;
	__u32 mode;
	__u32 hbi;
	__u32 vbi;
	__u32 skip_pattern;
	__u64 tpg_clock;
	__u32 reserved[4];
} __attribute__((packed));

/**
 * tpg_old_stream_config_t : stream configuration command
 *
 * @header:  common tpg command header
 * @pattern_type     : tpg pattern type used in this stream
 * @cb_mode          : tpg color bar mode used in this stream
 * @frame_count      : frame count in case of trigger burst mode
 * @stream_type      : type of stream like image pdaf etc
 * @stream_dimension : Dimension of the stream
 * @pixel_depth      : bits per each pixel
 * @cfa_arrangement  : color filter arragement
 * @output_format    : output image format
 * @hbi              : horizontal blanking intervel
 * @vbi              : vertical   blanking intervel
 * @vc               : virtual channel of this stream
 * @dt               : data type of this stream
 * @skip_pattern     : skip pattern for this stream
 * @rotate_period    : rotate period for this stream
 * @reserved         : reserved for future use
 */
struct tpg_old_stream_config_t {
	struct tpg_command_header_t header;
	enum tpg_pattern_t pattern_type;
	enum tpg_color_bar_mode_t cb_mode;
	__u32 frame_count;
	enum tpg_stream_t stream_type;
	struct stream_dimension stream_dimension;
	__u8 pixel_depth;
	enum tpg_cfa_arrangement_t cfa_arrangement;
	enum tpg_image_format_t output_format;
	__u32 hbi;
	__u32 vbi;
	__u16 vc;
	__u16 dt;
	__u32 skip_pattern;
	__u32 rotate_period;
	__u32 reserved[4];
} __attribute__((packed));

/**
 * tpg_stream_config_t : stream configuration command
 *
 * @header:  common tpg command header
 * @pattern_type     : tpg pattern type used in this stream
 * @cb_mode          : tpg color bar mode used in this stream
 * @frame_count      : frame count in case of trigger burst mode
 * @stream_type      : type of stream like image pdaf etc
 * @stream_dimension : Dimension of the stream
 * @pixel_depth      : bits per each pixel
 * @cfa_arrangement  : color filter arragement
 * @output_format    : output image format
 * @hbi              : horizontal blanking intervel
 * @vbi              : vertical   blanking intervel
 * @vc               : virtual channel of this stream
 * @dt               : data type of this stream
 * @skip_pattern     : skip pattern for this stream
 * @rotate_period    : rotate period for this stream
 * @xcfa_debug       : for xcfa debug;
 * @shdr_line_offset0 : for shdr line offset0
 * @shdr_line_offset1 : for shdr line offset1
 * @reserved         : reserved for future use
 */
struct tpg_stream_config_t {
	struct tpg_command_header_t header;
	enum tpg_pattern_t pattern_type;
	enum tpg_color_bar_mode_t cb_mode;
	__u32 frame_count;
	enum tpg_stream_t stream_type;
	struct stream_dimension stream_dimension;
	__u8 pixel_depth;
	enum tpg_cfa_arrangement_t cfa_arrangement;
	enum tpg_image_format_t output_format;
	__u32 hbi;
	__u32 vbi;
	__u16 vc;
	__u16 dt;
	__u32 skip_pattern;
	__u32 rotate_period;
	__u32 xcfa_debug;
	__u32 shdr_line_offset0;
	__u32 shdr_line_offset1;
	__u32 reserved[4];
} __attribute__((packed));

/**
 * tpg_stream_config_t : stream configuration command
 *
 * @header:  common tpg command header
 * @pattern_type     : tpg pattern type used in this stream
 * @cb_mode          : tpg color bar mode used in this stream
 * @frame_count      : frame count in case of trigger burst mode
 * @stream_type      : type of stream like image pdaf etc
 * @stream_dimension : Dimension of the stream
 * @pixel_depth      : bits per each pixel
 * @cfa_arrangement  : color filter arragement
 * @output_format    : output image format
 * @hbi              : horizontal blanking intervel
 * @vbi              : vertical   blanking intervel
 * @vc               : virtual channel of this stream
 * @dt               : data type of this stream
 * @skip_pattern     : skip pattern for this stream
 * @rotate_period    : rotate period for this stream
 * @shdr_line_offset0 : for shdr line offset0
 * @shdr_line_offset1 : for shdr line offset1
 * @cfa_info_exist    : cfa info exists
 * @cfa_info          : cfa information
 * @xcfa_type         : xcfa type
 * @reserved          : reserved for future use
 */
struct tpg_stream_config_v3_t {
	struct tpg_command_header_t header;
	__u32 pattern_type;
	__u32 cb_mode;
	__u32 frame_count;
	__u32 stream_type;
	struct stream_dimension stream_dimension;
	__u32 pixel_depth;
	__u32 cfa_arrangement;
	__u32 output_format;
	__u32 hbi;
	__u32 vbi;
	__u16 vc;
	__u16 dt;
	__u32 skip_pattern;
	__u32 rotate_period;
	__u32 xcfa_debug;
	__u32 shdr_line_offset0;
	__u32 shdr_line_offset1;
	__u32 cfa_info_exist;
	struct tpg_cfa_information_t cfa_info;
	__u32 xcfa_type;
	__u32 reserved[5];
} __attribute__((packed));

/**
 * tpg_illumination_control : illumianation control command
 *
 * @header         : common header for tpg command
 * @vc             : virtual channel to identify the stream
 * @dt             : dt to identify the stream
 * @exposure_short : short exposure time
 * @exposure_mid   : mid exposure time
 * @exposure_long  : long exposure time
 * @r_gain         : r channel gain
 * @g_gain         : g channel gain
 * @b_gain         : b channel gain
 * @reserved       : reserved for future use
 */
struct tpg_illumination_control {
	struct tpg_command_header_t header;
	__u16 vc;
	__u16 dt;
	__u32 exposure_short;
	__u32 exposure_mid;
	__u32 exposure_long;
	__u16 r_gain;
	__u16 g_gain;
	__u16 b_gain;
	__u32 reserved[4];
} __attribute__((packed));

/**
 * struct cam_flash_init : Init command for the flash
 * @flash_type  :    flash hw type
 * @reserved
 * @cmd_type    :    command buffer type
 */
struct cam_flash_init {
	__u32    flash_type;
	__u8     reserved;
	__u8     cmd_type;
	__u16    reserved1;
} __attribute__((packed));

/**
 * struct cam_flash_set_rer : RedEyeReduction command buffer
 *
 * @count             :   Number of flash leds
 * @opcode            :   Command buffer opcode
 *			CAM_FLASH_FIRE_RER
 * @cmd_type          :   command buffer operation type
 * @num_iteration     :   Number of led turn on/off sequence
 * @reserved
 * @led_on_delay_ms   :   flash led turn on time in ms
 * @led_off_delay_ms  :   flash led turn off time in ms
 * @led_current_ma    :   flash led current in ma
 *
 */
struct cam_flash_set_rer {
	__u32    count;
	__u8     opcode;
	__u8     cmd_type;
	__u16    num_iteration;
	__u32    led_on_delay_ms;
	__u32    led_off_delay_ms;
	__u32    led_current_ma[CAM_FLASH_MAX_LED_TRIGGERS];
} __attribute__((packed));

/**
 * struct cam_flash_set_on_off : led turn on/off command buffer
 *
 * @count                  : Number of Flash leds
 * @opcode                 : Command buffer opcodes
 *			     CAM_FLASH_FIRE_LOW
 *			     CAM_FLASH_FIRE_HIGH
 *			     CAM_FLASH_OFF
 * @cmd_type               : Command buffer operation type
 * @led_current_ma         : Flash led current in ma
 * @time_on_duration_ns    : Flash time on duration in ns
 * @led_on_wait_time_ns    : Flash led turn on wait time in ns
 *
 */
struct cam_flash_set_on_off {
	__u32    count;
	__u8     opcode;
	__u8     cmd_type;
	__u16    reserved;
	__u32    led_current_ma[CAM_FLASH_MAX_LED_TRIGGERS];
	__u64    time_on_duration_ns;
	__u64    led_on_wait_time_ns;
} __attribute__((packed));

/**
 * struct cam_flash_query_curr : query current command buffer
 *
 * @reserved
 * @opcode            :   command buffer opcode
 * @cmd_type          :   command buffer operation type
 * @query_current_ma  :   battery current in ma
 *
 */
struct cam_flash_query_curr {
	__u16    reserved;
	__u8     opcode;
	__u8     cmd_type;
	__u32    query_current_ma;
} __attribute__ ((packed));

/**
 * struct cam_flash_query_cap  :  capabilities info for flash
 *
 * @slot_info           :  Indicates about the slotId or cell Index
 * @max_current_flash   :  max supported current for flash
 * @max_duration_flash  :  max flash turn on duration
 * @max_current_torch   :  max supported current for torch
 *
 */
struct cam_flash_query_cap_info {
	__u32    slot_info;
	__u32    max_current_flash[CAM_FLASH_MAX_LED_TRIGGERS];
	__u32    max_duration_flash[CAM_FLASH_MAX_LED_TRIGGERS];
	__u32    max_current_torch[CAM_FLASH_MAX_LED_TRIGGERS];
} __attribute__ ((packed));

#define VIDIOC_MSM_CCI_CFG \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 23, struct cam_cci_ctrl)
/**
 * struct cam_flash_query_cap_v2  :  capabilities info for flash
 *
 * @version             :  Version to indicate the change
 * @slot_info           :  Indicates about the slotId or cell Index
 * @max_current_flash   :  max supported current for flash
 * @max_duration_flash  :  max flash turn on duration
 * @max_current_torch   :  max supported current for torch
 * @flash_type          :  Flag to indicate flash type (i2c/pmic)
 * @num_valid_params    :  Number of valid params to pass
 * @param_mask          :  Param mask for the params passed
 * @params              :  Array to contain future parameters
 *
 */
struct cam_flash_query_cap_info_v2 {
	__u32    version;
	__u32    slot_info;
	__u32    max_current_flash[CAM_FLASH_MAX_LED_TRIGGERS];
	__u32    max_duration_flash[CAM_FLASH_MAX_LED_TRIGGERS];
	__u32    max_current_torch[CAM_FLASH_MAX_LED_TRIGGERS];
	__u32    flash_type;
	__u32    num_valid_params;
	__u32    param_mask;
	__u32    params[3];
} __attribute__ ((packed));

/* CCI Timer infrastructure */

#define CAM_CCI_TIMER_MAX_EVENTS 27

enum cci_timer_freq_mode {
	CCI_TIMER_FREQ_INVALID = 0,
	CCI_TIMER_INFINITE_FRAME,	/* sensor stop/streamoff */
	CCI_TIMER_FINITE_FRAME,		/* single/multiple frames */
	CCI_TIMER_FREQ_MODE_MAX
};

enum cci_timer_fsync_trigger_point {
	CCI_TIMER_FSYNC_TP_INVALID = 0,
	CCI_TIMER_FSYNC_ACQUIRE,
	CCI_TIMER_FSYNC_STREAM_ON,
	CCI_TIMER_FSYNC_MAX
};

enum cci_timer_per_frame_trigger_point {
	CCI_TIMER_PERFRAME_EPOCH = 0,
	CCI_TIMER_PERFRAME_IMMEDIATE,
	CCI_TIMER_PERFRAME_EOF,
	CCI_TIMER_PERFRAME_MAX
};

enum cci_timer_gpio_ops_mode {
	CCI_TIMER_MODE_NO_OP = 0,
	/*
	 * CCI_TIMER_MODE_SYNC_INDEPENDENT:
	 * Used for independent link. i.e. for MIPI single MIPI sensor and
	 * for GMSL single deserializer
	 * GMSL:
	 *   - single deserializer can connect multiple sensor or single
	 *     sensor, all underneath connected sensors will get the same
	 *     XVS signal to perform any operation.
	 * FPGA:
	 * NOTE: Only one gpio control is needed. Expected number of
	 *       eventCount = 2, one for high and one for low.
	 * Usecase:
	 *   - Single MIPI/FPGA Sensors with one timer gpio
	 *   - Single desr - multiple/single sensor with perport and single
	 *     timer gpio
	 */
	CCI_TIMER_MODE_SYNC_INDEPENDENT,
	/*
	 * CCI_TIMER_MODE_SYNC_WITH_MULTI_QUEUE:
	 * This is for Multiple timer gpios on multiple Q
	 *   - Multiple GPIO_Q can support different FPS
	 * NOTE: As multiple GPIO_Q needs to be in sync with each other this
	 *       has to be in MCX usecase. NonMCX usecase it is not guaranteed
	 *       to be in sync as there is not any deterministic time for
	 *       camera stream to start.
	 * UseCase:
	 *   - MCX Usecase: Multiple FPS with different gpios
	 */
	CCI_TIMER_MODE_SYNC_WITH_MULTI_QUEUE,
	/*
	 * CCI_TIMER_MODE_SYNC_WITH_SINGLE_QUEUE:
	 * This is to sync multiple timers with Single GPIO_Q
	 * - Single GPIO_Q can support different FPS but has to be
	 *     multiplication factor. i.e. 5, 10, 15 or 15, 30, 60.
	 * NOTE: This mode can only be supported Max of 3 variable frame
	 *       rates with GPIO queue depth.
	 * UseCase:
	 *   - MCX:
	 *       - Perport multiple deser/multiple groups
	 *   - Multiple Deser: same FPS multiple GPIOS
	 *   - Single Sensor: driving multiple CCI timers for different
	 *     operation.
	 */
	CCI_TIMER_MODE_SYNC_WITH_SINGLE_QUEUE,
	/*
	 * CCI_TIMER_MODE_SYNC_WITH_ASYNC_CID_INPUT:
	 * This is used to get sync with CSID driven notification
	 *   - CSID -> programs CID based on VC and DT
	 * NOTE: This can apply to both Single and Multiple Queue Sync
	 */
	CCI_TIMER_MODE_SYNC_WITH_ASYNC_CID_INPUT,
	/*
	 * CCI_TIMER_MODE_SYNC_WITH_ASYNC_GPIO_INPUT:
	 * This is to driver GPIO_Q from external HW event via
	 * CCI_ASYNC Gpio
	 *   - This can also support both independent and Multiple Queue sync
	 */
	CCI_TIMER_MODE_SYNC_WITH_ASYNC_GPIO_INPUT,
	/*
	 * CCI_TIMER_MODE_SYNC_WITH_ASYNC_I2C_QUEUE_INPUT:
	 * i2c cmd queue is triggering GPIO timer queue for the operation
	 * NOTE: Only single slave should be connected to that CCI or single
	 * slave communication should be there when using this command.
	 * Two or more slave communication will not be deterministic
	 * for particular sensor based trigger.
	 */
	CCI_TIMER_MODE_SYNC_WITH_ASYNC_I2C_QUEUE_INPUT,
	CCI_TIMER_MODE_SYNC_MAX
};

enum cci_gpio_level {
	CCI_GPIO_LEVEL_LOW  = 0,
	CCI_GPIO_LEVEL_HIGH = 1,
};

/**
 * struct cci_timer_freq_info - CCI timer frequency information
 * @freq_mode:         Frequency mode (infinite or finite frames) of type enum cci_timer_freq_mode
 * @number_of_frames:  Number of frames for finite mode
 * @reserved:          Reserved for padding and future use
 */
struct cci_timer_freq_info {
	__u32 freq_mode;
	__u16 number_of_frames;
	__u16 reserved;
};


/**
 * struct cci_gpio_timing_event - GPIO timing event configuration
 * @gpio_number:         GPIO number (0-4)
 * @delay_to_trigger_ns: Relative time with respect to last event
 * @level:               GPIO level (HIGH or LOW) of type enum cci_gpio_level
 * @reserved:            Reserved for padding and future use
 * @external_event:      External event information
 */
struct cci_gpio_timing_event {
	__s64 gpio_number;
	__s64 delay_to_trigger_ns;
	__u32 level;
	__u32 reserved;
};

/**
 * struct cci_gpio_timing_schema - GPIO timing schema
 * @events:       Array of GPIO timing events (max CAM_CCI_TIMER_MAX_EVENTS)
 * @event_count:  Number of events in the array
 * @reserved:     Future use
 */
struct cci_gpio_timing_schema {
	struct cci_gpio_timing_event events[CAM_CCI_TIMER_MAX_EVENTS];
	__u32  event_count;
	__u32  reserved;
};

/**
 * struct cci_timer_trigger_point_info - Trigger point information
 * @tpoint_fsync_info:    Trigger point of type enum cci_timer_fsync_trigger_point.
 *                        Valid when repeat_freq_info.freq_mode is
 *                        CCI_TIMER_INFINITE_FRAME
 * @tpoint_perframe_info: Per-frame trigger point of type enum cci_timer_per_frame_trigger_point.
 *                        Valid when repeat_freq_info.freq_mode is
 *                        CCI_TIMER_FINITE_FRAME
 * @refcount_to_trigger:  Number of sensors participating in the synchronized
 *                        group identified by @shared_sync_id. The GPIO queue is
 *                        started only once this many sync blobs have been
 *                        received for that group and the trigger point is met.
 * @shared_sync_id:       Identifier of the synchronized group. All sensors that
 *                        must be frame-synchronized together share the same
 *                        value. Only sensors that support fsync and take part in
 *                        the session send a sync blob, so group membership is
 *                        implied by blob arrival.
 */
struct cci_timer_trigger_point_info {
	union {
		__u32 tpoint_fsync_info;
		__u32 tpoint_perframe_info;
	} tp;
	__u16   refcount_to_trigger;
	__u16   shared_sync_id;
};

/*
 * Generic top-level UAPI structure.
 * This structure must remain unchanged when new modes are added.
 */
struct cci_sync_info {
	__u16 size;
	__u16 version;

	__u32 operational_mode;

	/* Userspace pointer to mode-specific configuration */
	__aligned_u64 config_ptr;

	/* Size of mode-specific configuration */
	__u32 config_size;

	/* Must be zero */
	__u32 reserved[4];
};

/*
 * Common header present in every mode-specific payload.
 * Allows individual payloads to evolve independently.
 */
struct cci_mode_hdr {
	__u16 size;
	__u16 version;

	/* Must be zero */
	__u32 reserved[2];
};

/*
 * Existing mode payloads.
 * Additional members may be appended in the future.
 */
struct cci_timer_fsync_info {
	struct cci_mode_hdr hdr;

	struct cci_gpio_timing_schema timer_info;
	struct cci_timer_freq_info freq_info;
	struct cci_timer_trigger_point_info tpoint_info;
};

struct cci_async_gpio_info {
	struct cci_mode_hdr hdr;
	__s64 async_gpio_number;
	struct cci_gpio_timing_schema timer_info;
	struct cci_timer_freq_info freq_info;
};

struct cci_async_csid_info {
	struct cci_mode_hdr hdr;
	__u16 vc;
	__u16 dt;
	__u16 line_to_trigger;
	__s16 master_slot_idx;
	struct cci_gpio_timing_schema timer_info;
	struct cci_timer_freq_info freq_info;
};


#endif /* _UAPI_LINUX_CCI_TIMER_H */
