/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _CAM_VFE_LITE90X_H_
#define _CAM_VFE_LITE90X_H_
#include "cam_vfe_camif_ver3.h"
#include "cam_vfe_top_ver4.h"
#include "cam_vfe_core.h"
#include "cam_vfe_bus_ver3.h"
#include "cam_irq_controller.h"

#define CAM_VFE_90X_NUM_DBG_REG 4


static struct cam_vfe_top_ver4_wr_client_desc vfe90x_wr_client_desc[] = {
	{
		.wm_id = 0,
		.desc = "RDI_0",
	},
	{
		.wm_id = 1,
		.desc = "RDI_1",
	},
	{
		.wm_id = 2,
		.desc = "RDI_2",
	},
	{
		.wm_id = 3,
		.desc = "RDI_3",
	},
	{
		.wm_id = 4,
		.desc = "RDI_4",
	},
	{
		.wm_id = 5,
		.desc = "RDI_5",
	},
	{
		.wm_id = 6,
		.desc = "RDI_6",
	},
	{
		.wm_id = 7,
		.desc = "RDI_7",
	},
};

static struct cam_irq_register_set vfe90x_top_irq_reg_set[2] = {
	{
		.mask_reg_offset   = 0x00002028,
		.clear_reg_offset  = 0x00002030,
		.status_reg_offset = 0x00002038,
		.test_set_val      = BIT(0),
		.test_sub_val      = BIT(0),
	},
	{
		.mask_reg_offset   = 0x0000202C,
		.clear_reg_offset  = 0x00002034,
		.status_reg_offset = 0x0000203C,
	},
};

static struct cam_irq_controller_reg_info vfe90x_top_irq_reg_info = {
	.num_registers = 2,
	.irq_reg_set = vfe90x_top_irq_reg_set,
	.global_irq_cmd_offset = 0x00002024,
	.global_clear_bitmask  = 0x00000001,
	.global_set_bitmask    = 0x00000010,
	.clear_all_bitmask     = 0xFFFFFFFF,
};

static uint32_t vfe90x_top_debug_reg[] = {
	0x000020E4,
	0x000020E8,
	0x000020EC,
	0x000020F0,
};

static struct cam_vfe_top_ver4_reg_offset_common vfe90x_top_common_reg = {
	.hw_version               = 0x00002000,
	.hw_capability            = 0x00002004,
	.diag_config              = 0x00002010,
	.diag_sensor_status_0     = 0x00002014,
	.diag_sensor_status_1     = 0x00002018,
	.bus_violation_status     = 0x00002464,
	.bus_overflow_status      = 0x00002468,
	.top_debug_cfg            = 0x000020E0,
	.num_top_debug_reg        = CAM_VFE_90X_NUM_DBG_REG,
	.top_debug                = vfe90x_top_debug_reg,
	.frame_timing_irq_reg_idx = CAM_IFE_IRQ_CAMIF_REG_STATUS1,
};

static struct cam_vfe_ver4_path_reg_data vfe90x_rdi_reg_data[8] = {
	{
		.sof_irq_mask                    = 0x1,
		.eof_irq_mask                    = 0x2,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
	{
		.sof_irq_mask                    = 0x4,
		.eof_irq_mask                    = 0x8,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
	{
		.sof_irq_mask                    = 0x10,
		.eof_irq_mask                    = 0x20,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
	{
		.sof_irq_mask                    = 0x40,
		.eof_irq_mask                    = 0x80,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
	{
		.sof_irq_mask                    = 0x100,
		.eof_irq_mask                    = 0x200,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
	{
		.sof_irq_mask                    = 0x400,
		.eof_irq_mask                    = 0x800,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
	{
		/*
		 * RDI6 - SOF/EOF bit positions per TOP_FUNC_IRQ_MASK_1
		 * (RDI6_SOF = bit 12, RDI6_EOF = bit 13), confirmed via
		 * ip_catalog swi_get_module_details field metadata.
		 */
		.sof_irq_mask                    = 0x1000,
		.eof_irq_mask                    = 0x2000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
	{
		/* RDI7 - RDI7_SOF = bit 14, RDI7_EOF = bit 15 */
		.sof_irq_mask                    = 0x4000,
		.eof_irq_mask                    = 0x8000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 0x3,
	},
};

static struct cam_vfe_ver4_path_hw_info
	vfe90x_rdi_hw_info[] = {
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[0],
	},
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[1],
	},
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[2],
	},
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[3],
	},
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[4],
	},
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[5],
	},
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[6],
	},
	{
		.common_reg     = &vfe90x_top_common_reg,
		.reg_data       = &vfe90x_rdi_reg_data[7],
	},
};

static struct cam_vfe_top_ver4_debug_reg_info vfe90x_dbg_reg_info[CAM_VFE_90X_NUM_DBG_REG][8] = {
	VFE_DBG_INFO_ARRAY_4bit(
		"test_bus_reserved",
		"test_bus_reserved",
		"test_bus_reserved",
		"test_bus_reserved",
		"test_bus_reserved",
		"test_bus_reserved",
		"test_bus_reserved",
		"test_bus_reserved"
	),
	{
		VFE_DBG_INFO(32, "non-CLC info"),
		VFE_DBG_INFO(32, "non-CLC info"),
		VFE_DBG_INFO(32, "non-CLC info"),
		VFE_DBG_INFO(32, "non-CLC info"),
		VFE_DBG_INFO(32, "non-CLC info"),
		VFE_DBG_INFO(32, "non-CLC info"),
		VFE_DBG_INFO(32, "non-CLC info"),
		VFE_DBG_INFO(32, "non-CLC info"),
	},
	VFE_DBG_INFO_ARRAY_4bit(
		"csid_ccif_rdi0_vld_out",
		"csid_ccif_rdi1_vld_out",
		"csid_ccif_rdi3_vld_out",
		"csid_ccif_rdi4_vld_out",
		"csid_ccif_rdi5_vld_out",
		"unused",
		"unused",
		"unused"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"unused",
		"unused",
		"unused",
		"unused",
		"unused",
		"unused",
		"unused",
		"unused"
	),
};

static struct cam_vfe_top_ver4_hw_info vfe90x_top_hw_info = {
	.common_reg = &vfe90x_top_common_reg,
	.rdi_hw_info =   vfe90x_rdi_hw_info,
	.vfe_full_hw_info = {
		.common_reg     = &vfe90x_top_common_reg,
	},
	.wr_client_desc         = vfe90x_wr_client_desc,
	.num_mux = 8,
	.mux_type = {
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
	},
	.num_rdi        = ARRAY_SIZE(vfe90x_rdi_hw_info),
	.debug_reg_info = &vfe90x_dbg_reg_info,
};

static struct cam_irq_register_set vfe90x_bus_irq_reg[1] = {
		{
			.mask_reg_offset   = 0x00002418,
			.clear_reg_offset  = 0x00002420,
			.status_reg_offset = 0x00002428,
			.set_reg_offset    = 0x00002450,
		},
};

static uint32_t vfe90x_out_port_mid[][4] = {
	{16, 0, 0, 0},
	{17, 0, 0, 0},
	{18, 0, 0, 0},
	{19, 0, 0, 0},
	{20, 0, 0, 0},
	{21, 0, 0, 0},
	{22, 0, 0, 0},
	{23, 0, 0, 0},
};

static struct cam_vfe_bus_ver3_hw_info vfe90x_bus_hw_info = {
	.common_reg = {
		.hw_version                       = 0x00002400,
		.cgc_ovd                          = 0x00002408,
		.pwr_iso_cfg                      = 0x0000245C,
		.overflow_status_clear            = 0x00002460,
		.ccif_violation_status            = 0x00002464,
		.overflow_status                  = 0x00002468,
		.image_size_violation_status      = 0x00002470,
		.debug_status_top_cfg             = 0x000024F0,
		.debug_status_top                 = 0x000024F4,
		.test_bus_ctrl                    = 0x00002544,
		.irq_reg_info = {
			.num_registers            = 1,
			.irq_reg_set              = vfe90x_bus_irq_reg,
			.global_irq_cmd_offset    = 0x00002430,
			.global_clear_bitmask     = 0x00000001,
		},
	},
	.num_client = 8,
	.bus_client_reg = {
		/* BUS Client 0 RDI0 */
		{
			.cfg                      = 0x00002C00,
			.image_addr               = 0x00002C04,
			.frame_incr               = 0x00002C08,
			.image_cfg_0              = 0x00002C0C,
			.image_cfg_1              = 0x00002C10,
			.image_cfg_2              = 0x00002C14,
			.packer_cfg               = 0x00002C18,
			.frame_header_addr        = 0x00002C20,
			.frame_header_incr        = 0x00002C24,
			.frame_header_cfg         = 0x00002C28,
			.line_done_cfg            = 0x00002C2C,
			.irq_subsample_period     = 0x00002C30,
			.irq_subsample_pattern    = 0x00002C34,
			.mmu_prefetch_cfg         = 0x00002C60,
			.mmu_prefetch_max_offset  = 0x00002C64,
			.system_cache_cfg         = 0x00002C68,
			.addr_status_0            = 0x00002C90,
			.addr_status_1            = 0x00002C94,
			.addr_status_2            = 0x00002C98,
			.addr_status_3            = 0x00002C9C,
			.debug_status_cfg         = 0x00002C7C,
			.debug_status_0           = 0x00002C80,
			.debug_status_1           = 0x00002C84,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
		/* BUS Client 1 RDI1 */
		{
			.cfg                      = 0x00002D00,
			.image_addr               = 0x00002D04,
			.frame_incr               = 0x00002D08,
			.image_cfg_0              = 0x00002D0C,
			.image_cfg_1              = 0x00002D10,
			.image_cfg_2              = 0x00002D14,
			.packer_cfg               = 0x00002D18,
			.frame_header_addr        = 0x00002D20,
			.frame_header_incr        = 0x00002D24,
			.frame_header_cfg         = 0x00002D28,
			.line_done_cfg            = 0x00002D2C,
			.irq_subsample_period     = 0x00002D30,
			.irq_subsample_pattern    = 0x00002D34,
			.mmu_prefetch_cfg         = 0x00002D60,
			.mmu_prefetch_max_offset  = 0x00002D64,
			.system_cache_cfg         = 0x00002D68,
			.addr_status_0            = 0x00002D90,
			.addr_status_1            = 0x00002D94,
			.addr_status_2            = 0x00002D98,
			.addr_status_3            = 0x00002D9C,
			.debug_status_cfg         = 0x00002D7C,
			.debug_status_0           = 0x00002D80,
			.debug_status_1           = 0x00002D84,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
		/* BUS Client 2 RDI2 */
		{
			.cfg                      = 0x00002E00,
			.image_addr               = 0x00002E04,
			.frame_incr               = 0x00002E08,
			.image_cfg_0              = 0x00002E0C,
			.image_cfg_1              = 0x00002E10,
			.image_cfg_2              = 0x00002E14,
			.packer_cfg               = 0x00002E18,
			.frame_header_addr        = 0x00002E20,
			.frame_header_incr        = 0x00002E24,
			.frame_header_cfg         = 0x00002E28,
			.line_done_cfg            = 0x00002E2C,
			.irq_subsample_period     = 0x00002E30,
			.irq_subsample_pattern    = 0x00002E34,
			.mmu_prefetch_cfg         = 0x00002E60,
			.mmu_prefetch_max_offset  = 0x00002E64,
			.system_cache_cfg         = 0x00002E68,
			.addr_status_0            = 0x00002E90,
			.addr_status_1            = 0x00002E94,
			.addr_status_2            = 0x00002E98,
			.addr_status_3            = 0x00002E9C,
			.debug_status_cfg         = 0x00002E7C,
			.debug_status_0           = 0x00002E80,
			.debug_status_1           = 0x00002E84,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_2,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
		/* BUS Client 3 RDI3 */
		{
			.cfg                      = 0x00002F00,
			.image_addr               = 0x00002F04,
			.frame_incr               = 0x00002F08,
			.image_cfg_0              = 0x00002F0C,
			.image_cfg_1              = 0x00002F10,
			.image_cfg_2              = 0x00002F14,
			.packer_cfg               = 0x00002F18,
			.frame_header_addr        = 0x00002F20,
			.frame_header_incr        = 0x00002F24,
			.frame_header_cfg         = 0x00002F28,
			.line_done_cfg            = 0x00002F2C,
			.irq_subsample_period     = 0x00002F30,
			.irq_subsample_pattern    = 0x00002F34,
			.mmu_prefetch_cfg         = 0x00002F60,
			.mmu_prefetch_max_offset  = 0x00002F64,
			.system_cache_cfg         = 0x00002F68,
			.addr_status_0            = 0x00002F90,
			.addr_status_1            = 0x00002F94,
			.addr_status_2            = 0x00002F98,
			.addr_status_3            = 0x00002F9C,
			.debug_status_cfg         = 0x00002F7C,
			.debug_status_0           = 0x00002F80,
			.debug_status_1           = 0x00002F84,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_3,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
		/* BUS Client 4 RDI4 */
		{
			.cfg                      = 0x00003000,
			.image_addr               = 0x00003004,
			.frame_incr               = 0x00003008,
			.image_cfg_0              = 0x0000300C,
			.image_cfg_1              = 0x00003010,
			.image_cfg_2              = 0x00003014,
			.packer_cfg               = 0x00003018,
			.frame_header_addr        = 0x00003020,
			.frame_header_incr        = 0x00003024,
			.frame_header_cfg         = 0x00003028,
			.line_done_cfg            = 0x0000302C,
			.irq_subsample_period     = 0x00003030,
			.irq_subsample_pattern    = 0x00003034,
			.mmu_prefetch_cfg         = 0x00003060,
			.mmu_prefetch_max_offset  = 0x00003064,
			.system_cache_cfg         = 0x00003068,
			.addr_status_0            = 0x00003090,
			.addr_status_1            = 0x00003094,
			.addr_status_2            = 0x00003098,
			.addr_status_3            = 0x0000309C,
			.debug_status_cfg         = 0x0000307C,
			.debug_status_0           = 0x00003080,
			.debug_status_1           = 0x00003084,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_4,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
		/* BUS Client 5 RDI5 */
		{
			.cfg                      = 0x00003100,
			.image_addr               = 0x00003104,
			.frame_incr               = 0x00003108,
			.image_cfg_0              = 0x0000310C,
			.image_cfg_1              = 0x00003110,
			.image_cfg_2              = 0x00003114,
			.packer_cfg               = 0x00003118,
			.frame_header_addr        = 0x00003120,
			.frame_header_incr        = 0x00003124,
			.frame_header_cfg         = 0x00003128,
			.line_done_cfg            = 0x0000312C,
			.irq_subsample_period     = 0x00003130,
			.irq_subsample_pattern    = 0x00003134,
			.mmu_prefetch_cfg         = 0x00003160,
			.mmu_prefetch_max_offset  = 0x00003164,
			.system_cache_cfg         = 0x00003168,
			.addr_status_0            = 0x00003190,
			.addr_status_1            = 0x00003194,
			.addr_status_2            = 0x00003198,
			.addr_status_3            = 0x0000319C,
			.debug_status_cfg         = 0x0000317C,
			.debug_status_0           = 0x00003180,
			.debug_status_1           = 0x00003184,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_5,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
		/* BUS Client 6 RDI6 (CSID adds YUV_CHROMA_CONVERSION, no bus-side change) */
		{
			.cfg                      = 0x00003200,
			.image_addr               = 0x00003204,
			.frame_incr               = 0x00003208,
			.image_cfg_0              = 0x0000320C,
			.image_cfg_1              = 0x00003210,
			.image_cfg_2              = 0x00003214,
			.packer_cfg               = 0x00003218,
			.frame_header_addr        = 0x00003220,
			.frame_header_incr        = 0x00003224,
			.frame_header_cfg         = 0x00003228,
			.line_done_cfg            = 0x0000322C,
			.irq_subsample_period     = 0x00003230,
			.irq_subsample_pattern    = 0x00003234,
			.mmu_prefetch_cfg         = 0x00003260,
			.mmu_prefetch_max_offset  = 0x00003264,
			.system_cache_cfg         = 0x00003268,
			.addr_status_0            = 0x00003290,
			.addr_status_1            = 0x00003294,
			.addr_status_2            = 0x00003298,
			.addr_status_3            = 0x0000329C,
			.debug_status_cfg         = 0x0000327C,
			.debug_status_0           = 0x00003280,
			.debug_status_1           = 0x00003284,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_6,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
		/* BUS Client 7 RDI7 (CSID adds YUV_CHROMA_CONVERSION, no bus-side change) */
		{
			.cfg                      = 0x00003300,
			.image_addr               = 0x00003304,
			.frame_incr               = 0x00003308,
			.image_cfg_0              = 0x0000330C,
			.image_cfg_1              = 0x00003310,
			.image_cfg_2              = 0x00003314,
			.packer_cfg               = 0x00003318,
			.frame_header_addr        = 0x00003320,
			.frame_header_incr        = 0x00003324,
			.frame_header_cfg         = 0x00003328,
			.line_done_cfg            = 0x0000332C,
			.irq_subsample_period     = 0x00003330,
			.irq_subsample_pattern    = 0x00003334,
			.mmu_prefetch_cfg         = 0x00003360,
			.mmu_prefetch_max_offset  = 0x00003364,
			.system_cache_cfg         = 0x00003368,
			.addr_status_0            = 0x00003390,
			.addr_status_1            = 0x00003394,
			.addr_status_2            = 0x00003398,
			.addr_status_3            = 0x0000339C,
			.debug_status_cfg         = 0x0000337C,
			.debug_status_0           = 0x00003380,
			.debug_status_1           = 0x00003384,
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_7,
			.ubwc_regs                = NULL,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_6) | BIT_ULL(CAM_FORMAT_MIPI_RAW_8) |
				BIT_ULL(CAM_FORMAT_YUV422) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_MIPI_RAW_16) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_20) | BIT_ULL(CAM_FORMAT_PLAIN128) |
				BIT_ULL(CAM_FORMAT_PLAIN32_20) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_10) | BIT_ULL(CAM_FORMAT_PLAIN16_12) |
				BIT_ULL(CAM_FORMAT_PLAIN16_14) | BIT_ULL(CAM_FORMAT_PLAIN16_16) |
				BIT_ULL(CAM_FORMAT_PLAIN64) | BIT_ULL(CAM_FORMAT_YUV422_10),
		},
	},
	.num_out = 8,
	.vfe_out_hw_info = {
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI0,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_1,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[0],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				0,
			},
			.name          = {
				"LITE_0",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI1,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_2,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[1],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				1,
			},
			.name          = {
				"LITE_1",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI2,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_3,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[2],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				2,
			},
			.name          = {
				"LITE_2",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI3,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_4,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[3],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				3,
			},
			.name          = {
				"LITE_3",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI4,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_5,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[4],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				4,
			},
			.name          = {
				"LITE_4",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI5,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_6,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[5],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				5,
			},
			.name          = {
				"LITE_5",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI6,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_7,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[6],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				6,
			},
			.name          = {
				"LITE_6",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI7,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_8,
			.num_wm        = 1,
			.mid           = vfe90x_out_port_mid[7],
			.num_mid       = 1,
			.line_based    = 1,
			.wm_idx        = {
				7,
			},
			.name          = {
				"LITE_7",
			},
		},
	},
	.num_comp_grp    = 8,
	.support_consumed_addr = true,
	.comp_done_mask = {
		BIT(16), BIT(17), BIT(18), BIT(19), BIT(20), BIT(21), BIT(22), BIT(23)
	},
	.top_irq_shift   = 0,
	.max_out_res = CAM_ISP_IFE_OUT_RES_BASE + 56,
};

static struct cam_vfe_irq_hw_info vfe90x_irq_hw_info = {
	.reset_mask    = 0,
	.supported_irq = CAM_VFE_HW_IRQ_CAP_LITE_EXT_CSID,
	.top_irq_reg   = &vfe90x_top_irq_reg_info,
};

static struct cam_vfe_hw_info cam_vfe_lite90x_hw_info = {
	.irq_hw_info                   = &vfe90x_irq_hw_info,

	.bus_version                   = CAM_VFE_BUS_VER_3_0,
	.bus_hw_info                   = &vfe90x_bus_hw_info,

	.top_version                   = CAM_VFE_TOP_VER_4_0,
	.top_hw_info                   = &vfe90x_top_hw_info,
};

#endif /* _CAM_VFE_LITE90X_H_ */
