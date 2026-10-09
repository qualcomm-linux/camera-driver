/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _CAM_VFE900_H_
#define _CAM_VFE900_H_
#include "cam_vfe_top_ver4.h"
#include "cam_vfe_core.h"
#include "cam_vfe_bus_ver3.h"
#include "cam_irq_controller.h"

#define CAM_VFE_BUS_VER3_900_MAX_CLIENTS     40
#define CAM_VFE_900_NUM_DBG_REG              14

static struct cam_vfe_top_ver4_ipp_violation_desc vfe900_ipp_violation_desc[] = {
	{
		.bitmask = BIT(0),
		.desc = "CLC_STATS_AWB_BG_TINTLESS",
	},
	{
		.bitmask = BIT(1),
		.desc = "CLC_STATS_AWB_BG_IR",
	},
	{
		.bitmask = BIT(2),
		.desc = "STATS_BHIST_IR",
	},
	{
		.bitmask = BIT(3),
		.desc = "CLC_STATS_AWB_BG_IRC_AEC",
	},
	{
		.bitmask = BIT(4),
		.desc = "STATS_BHIST_IRC_AEC",
	},
	{
		.bitmask = BIT(5),
		.desc = "CLC_STATS_AWB_BG_AEC",
	},
	{
		.bitmask = BIT(6),
		.desc = "STATS_BHIST_AEC",
	},
	{
		.bitmask = BIT(7),
		.desc = "STATS_RS",
	},
	{
		.bitmask = BIT(8),
		.desc = "STATS_BFW_AWB",
	},
	{
		.bitmask = BIT(9),
		.desc = "CLC_STATS_AWB_BG_AWB",
	},
	{
		.bitmask = BIT(10),
		.desc = "STATS_BHIST_TMC",
	},
	{
		.bitmask = BIT(11),
		.desc = "CROP_RND_CLAMP_W_IR_OUT",
	},
	{
		.bitmask = BIT(12),
		.desc = "CLC_PIXEL_INTERLEAVER_AI_0_OUT",
	},
	{
		.bitmask = BIT(13),
		.desc = "CLC_PIXEL_INTERLEAVER_AI_1_OUT",
	},
	{
		.bitmask = BIT(14),
		.desc = "CLC_PIXEL_INTERLEAVER_MAIN_OUT",
	},
};

static struct cam_vfe_top_ver4_wr_client_desc vfe900_wr_client_desc[] = {
	{
		.wm_id = 0,
		.desc = "MAIN_C0",
	},
	{
		.wm_id = 1,
		.desc = "MAIN_C1",
	},
	{
		.wm_id = 2,
		.desc = "MAIN_C2",
	},
	{
		.wm_id = 3,
		.desc = "MAIN_UV",
	},
	{
		.wm_id = 4,
		.desc = "PIXEL_RAW",
	},
	{
		.wm_id = 5,
		.desc = "W_IR",
	},
	{
		.wm_id = 6,
		.desc = "AI_1_C0",
	},
	{
		.wm_id = 7,
		.desc = "AI_1_C1",
	},
	{
		.wm_id = 8,
		.desc = "AI_1_C2",
	},
	{
		.wm_id = 9,
		.desc = "AI_1_UV",
	},
	{
		.wm_id = 10,
		.desc = "AI_2_C0",
	},
	{
		.wm_id = 11,
		.desc = "AI_2_C1",
	},
	{
		.wm_id = 12,
		.desc = "AI_2_C2",
	},
	{
		.wm_id = 13,
		.desc = "AI_2_UV",
	},
	{
		.wm_id = 14,
		.desc = "HV_DS16",
	},
	{
		.wm_id = 15,
		.desc = "HV_DS4",
	},
	{
		.wm_id = 16,
		.desc = "STATS_BG_IR",
	},
	{
		.wm_id = 17,
		.desc = "STATS_BHIST_IR",
	},
	{
		.wm_id = 18,
		.desc = "STATS_BG_TINTLESS",
	},
	{
		.wm_id = 19,
		.desc = "STATS_BG_IRC_AEC",
	},
	{
		.wm_id = 20,
		.desc = "STATS_BHIST_IRC_AEC",
	},
	{
		.wm_id = 21,
		.desc = "STATS_BG_AEC",
	},
	{
		.wm_id = 22,
		.desc = "STATS_BHIST_AEC",
	},
	{
		.wm_id = 23,
		.desc = "STATS_BAYER_RS",
	},
	{
		.wm_id = 24,
		.desc = "STATS_BFW_AWB",
	},
	{
		.wm_id = 25,
		.desc = "STATS_BG_AWB",
	},
	{
		.wm_id = 26,
		.desc = "STATS_BHIST_TMC",
	},
	{
		.wm_id = 27,
		.desc = "STATS_BLTM",
	},
	{
		.wm_id = 28,
		.desc = "RDI_0",
	},
	{
		.wm_id = 29,
		.desc = "RDI_1",
	},
	{
		.wm_id = 30,
		.desc = "RDI_2",
	},
	{
		.wm_id = 31,
		.desc = "RDI_3",
	},
	{
		.wm_id = 32,
		.desc = "RDI_4",
	},
	{
		.wm_id = 33,
		.desc = "RDI_5",
	},
	{
		.wm_id = 34,
		.desc = "RDI_6",
	},
	{
		.wm_id = 35,
		.desc = "RDI_7",
	},
	{
		.wm_id = 36,
		.desc = "RDI_8",
	},
	{
		.wm_id = 37,
		.desc = "RDI_9",
	},
	{
		.wm_id = 38,
		.desc = "RDI_10",
	},
	{
		.wm_id = 39,
		.desc = "RDI_11",
	},
};

static struct cam_vfe_top_ver4_top_err_irq_desc vfe900_top_irq_err_desc[] = {
	{
		.bitmask = BIT(4),
		.err_name = "MAIN PP VIOLATION",
		.desc = "MAIN_PP pipeline violation",
	},
	{
		.bitmask = BIT(5),
		.err_name = "SCALAR VIOLATION",
		.desc = "IFE SCALAR HM pipeline violation",
	},
	{
		.bitmask = BIT(6),
		.err_name = "PCP VIOLATION",
		.desc = "IFE PCP HM pipeline violation",
	},
	{
		.bitmask = BIT(7),
		.err_name = "TMC VIOLATION",
		.desc = "IFE TMC pipeline violation",
	},
	{
		.bitmask = BIT(15),
		.err_name = "DIAG VIOLATION",
		.desc = "HBI is less than the minimum required HBI",
	},
};

static struct cam_irq_register_set vfe900_top_irq_reg_set[3] = {
	{
		.mask_reg_offset   = 0x000000A8,
		.clear_reg_offset  = 0x000000B4,
		.status_reg_offset = 0x000000C0,
		.test_set_val      = BIT(1),
		.test_sub_val      = BIT(0),
	},
	{
		.mask_reg_offset   = 0x000000AC,
		.clear_reg_offset  = 0x000000B8,
		.status_reg_offset = 0x000000C4,
	},
	{
		.mask_reg_offset   = 0x000000B0,
		.clear_reg_offset  = 0x000000BC,
		.status_reg_offset = 0x000000CC,
	},
};

static struct cam_irq_controller_reg_info vfe900_top_irq_reg_info = {
	.num_registers = 3,
	.irq_reg_set = vfe900_top_irq_reg_set,
	.global_irq_cmd_offset = 0x000000A4,
	.global_clear_bitmask  = 0x00000001,
	.global_set_bitmask    = 0x00000010,
	.clear_all_bitmask     = 0xFFFFFFFF,
};

static uint32_t vfe900_top_debug_reg[] = {
	0x00000138,
	0x0000013C,
	0x00000140,
	0x00000144,
	0x00000148,
	0x0000014C,
	0x00000150,
	0x00000154,
	0x00000158,
	0x0000015C,
	0x00000160,
	0x00000164,
	0x00000168,
	0x0000016C,
};

static struct cam_vfe_top_ver4_reg_offset_common vfe900_top_common_reg = {
	.hw_version               = 0x00000000,
	.hw_capability            = 0x00000004,
	.lens_feature             = 0x00000008,
	.stats_feature            = 0x0000000C,
	.color_feature            = 0x00000010,
	.zoom_feature             = 0x00000014,
	.core_cgc_ovd_0           = 0x00000018,
	.ahb_cgc_ovd              = 0x00000020,
	.core_cfg_0               = 0x00000024,
	.core_cfg_1               = 0x00000028,
	.diag_config              = 0x0000007C,
	.diag_sensor_status_0     = 0x00000080,
	.diag_sensor_status_1     = 0x00000084,
	.diag_frm_cnt_status_0    = 0x00000088,
	.diag_frm_cnt_status_1    = 0x0000008C,
	.diag_frm_cnt_status_2    = 0x00000090,
	.stats_throttle_cfg_0     = 0x00000098,
	.stats_throttle_cfg_1     = 0x0000009C,
	.stats_throttle_cfg_2     = 0x000000A0,
	.ipp_violation_status     = 0x00000100,
	.bus_violation_status     = 0x00000764,
	.bus_overflow_status      = 0x00000768,
	.num_perf_counters        = 4,
	.perf_count_reg = {
		{
			.perf_count_cfg    = 0x00000188,
			.perf_count_cfg_mc = 0x0000018C,
			.perf_pix_count    = 0x00000190,
			.perf_line_count   = 0x00000194,
			.perf_stall_count  = 0x00000198,
			.perf_always_count = 0x0000019C,
			.perf_count_status = 0x000001A0,
		},
		{
			.perf_count_cfg    = 0x000001A4,
			.perf_count_cfg_mc = 0x000001A8,
			.perf_pix_count    = 0x000001AC,
			.perf_line_count   = 0x000001B0,
			.perf_stall_count  = 0x000001B4,
			.perf_always_count = 0x000001B8,
			.perf_count_status = 0x000001BC,
		},
		{
			.perf_count_cfg    = 0x000001C0,
			.perf_count_cfg_mc = 0x000001C4,
			.perf_pix_count    = 0x000001C8,
			.perf_line_count   = 0x000001CC,
			.perf_stall_count  = 0x000001D0,
			.perf_always_count = 0x000001D4,
			.perf_count_status = 0x000001D8,
		},
		{
			.perf_count_cfg    = 0x000001DC,
			.perf_count_cfg_mc = 0x000001E0,
			.perf_pix_count    = 0x000001E4,
			.perf_line_count   = 0x000001E8,
			.perf_stall_count  = 0x000001EC,
			.perf_always_count = 0x000001F0,
			.perf_count_status = 0x000001F4,
		},
	},
	.top_debug_cfg            = 0x00000170,
	.num_top_debug_reg        = CAM_VFE_900_NUM_DBG_REG,
	.top_debug = vfe900_top_debug_reg,
	.frame_timing_irq_reg_idx = CAM_IFE_IRQ_CAMIF_REG_STATUS1,
};

static struct cam_vfe_ver4_path_reg_data vfe900_pp_common_reg_data = {
	.sof_irq_mask                    = 0x0000000F,
	.epoch0_irq_mask                 = 0x0,
	.epoch1_irq_mask                 = 0x0,
	.eof_irq_mask                    = 0x00000F00,
	/* FUNC_IRQ_0 violations: MAIN_PP b4, SCALAR b5, PCP b6, TMC b7, DIAG b15 */
	.error_irq_mask                  = 0x80F0,
	.enable_diagnostic_hw            = 0x1,
	.top_debug_cfg_en                = 3,
	.ipp_violation_mask              = 0x10,
	.is_mc_path                      = true,
	/* SOF|EOF combined per IPP context: CTXT0=b0|b8, CTXT1=b1|b9, CTXT2=b2|b10, CTXT3=b3|b11 */
	.frm_irq_hw_ctxt_mask = {
		0x101,
		0x202,
		0x404,
		0x808,
	},
};

static struct cam_vfe_ver4_path_reg_data vfe900_vfe_full_rdi_reg_data[12] = {
	{
		.sof_irq_mask                    = 0x1,
		.eof_irq_mask                    = 0x10000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x2,
		.eof_irq_mask                    = 0x20000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x4,
		.eof_irq_mask                    = 0x40000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x8,
		.eof_irq_mask                    = 0x80000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x10,
		.eof_irq_mask                    = 0x100000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x20,
		.eof_irq_mask                    = 0x200000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x40,
		.eof_irq_mask                    = 0x400000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x80,
		.eof_irq_mask                    = 0x800000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x100,
		.eof_irq_mask                    = 0x1000000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x200,
		.eof_irq_mask                    = 0x2000000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x400,
		.eof_irq_mask                    = 0x4000000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
	{
		.sof_irq_mask                    = 0x800,
		.eof_irq_mask                    = 0x8000000,
		.error_irq_mask                  = 0x0,
		.enable_diagnostic_hw            = 0x1,
		.top_debug_cfg_en                = 3,
	},
};

struct cam_vfe_ver4_path_hw_info
	vfe900_rdi_hw_info_arr[] = {
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[0],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[1],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[2],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[3],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[4],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[5],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[6],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[7],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[8],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[9],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[10],
	},
	{
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_vfe_full_rdi_reg_data[11],
	},
};

static struct cam_vfe_top_ver4_debug_reg_info vfe900_dbg_reg_info[CAM_VFE_900_NUM_DBG_REG][8] = {
	VFE_DBG_INFO_ARRAY_4bit(
		"downscale_mn_c_ai_out1",
		"crop_rnd_clamp_downscale_mn_y_ai_out1_2",
		"downscale_mn_y_ai_out1_2",
		"crop_rnd_clamp_downscale_mn_y_ai_out1_1",
		"downscale_mn_y_ai_out1_1",
		"crop_rnd_clamp_downscale_mn_y_ai_out1_0",
		"downscale_mn_y_ai_out1_0",
		"vid_output_packer_c"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"vid_output_packer_y2",
		"vid_output_packer_y1",
		"vid_output_packer_y0",
		"crop_rnd_clamp_downscale_mn_c_vid",
		"downscale_mn_c_vid",
		"crop_rnd_clamp_downscale_mn_y_vid_2",
		"downscale_mn_y_vid_2",
		"crop_rnd_clamp_downscale_mn_y_vid_1"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"downscale_mn_y_vid_1",
		"crop_rnd_clamp_downscale_mn_y_vid_0",
		"downscale_mn_y_vid_0",
		"rp2d_ds16_disp",
		"rp2d_ds16_disp",
		"crop_rnd_clamp_post_downscale_4to1_c_ds16_disp",
		"downscale_4to1_c_disp_ds16",
		"crop_rnd_clamp_post_downscale_4to1_y_ds16_disp"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"downscale_4to1_y_disp_ds16_idle",
		"rp2d_ds4_disp",
		"rp2d_ds4_disp",
		"crop_rnd_clamp_post_downscale_4to1_c_ds4_disp",
		"downscale_4to1_c_disp_ds4",
		"crop_rnd_clamp_post_downscale_4to1_y_ds4_disp",
		"downscale_4to1_y_disp_ds4",
		"crop_rnd_clamp_post_downscale_mn_c_disp"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"downscale_mn_c_disp",
		"crop_rnd_clamp_post_downscale_mn_y_disp",
		"downscale_mn_y_disp",
		"uvg",
		"color_xform",
		"glut_rgb",
		"glut",
		"gtm"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"color_correct",
		"isp_throttle",
		"demo",
		"hvx_tap2",
		"lcac",
		"bayer_ltm",
		"bayer_gtm",
		"bls"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"bpc_abf",
		"gic",
		"wb_gain",
		"pdpc_bpc_1d_post_lsc",
		"lsc",
		"pdpc_bpc_1d",
		"crop_rnd_clamp_pixe_raw_out",
		"hvx_tap1"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"channel_gain",
		"compdecomp_idle",
		"hvx_tap0",
		"preprocess_idle",
		"RDI2",
		"RDI1",
		"RDI0",
		"CLIENT_24:AI_OUT_2_C2"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"CLIENT_23:AI_OUT_2_UV/C1",
		"CLIENT_22:AI_OUT_2_Y/C1",
		"CLIENT_21:AI_OUT_1_C2",
		"CLIENT_20:AI_OUT_1_UV/C1",
		"CLIENT_19:AI_OUT_1_Y/C1",
		"CLIENT_15:Stats FLICKER RS",
		"CLIENT_14:Stats GTM BHIST",
		"CLIENT_13:Stats AWB BFW"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"CLIENT_12:Stats AWB BG",
		"CLIENT_11:Stats Tintless BG",
		"CLIENT_10:Stats AEC BHIST",
		"CLIENT_9:Stats AEC BG",
		"CLIENT_8:LTM STATS",
		"CLIENT_7:Pixel Raw",
		"CLIENT_6:DISP 16:1",
		"CLIENT_5:DISP 4:1"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"CLIENT_4:DISP C 1:1",
		"CLIENT_3:DISP Y 1:1",
		"CLIENT_2:VID C2",
		"CLIENT_1:VID UV/C1",
		"CLIENT_0:VID Y/C0",
		"PP",
		"CLIENT_25",
		"CLIENT_26"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"CLIENT_27",
		"STATS_IR_BHIST",
		"STATS_IR_BG",
		"CROP_RND_CLAMP_IR",
		"XCFA_RGBIR_REMOX",
		"XCFA_BPC_PDPC",
		"XCFA_CHANNEL_GAINS",
		"ISP_THROTTLE_IR"
	),
	VFE_DBG_INFO_ARRAY_4bit(
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved",
		"reserved"
	),
};

static struct cam_vfe_top_ver4_hw_info vfe900_top_hw_info = {
	.common_reg = &vfe900_top_common_reg,
	.vfe_full_hw_info = {
		.common_reg     = &vfe900_top_common_reg,
		.reg_data       = &vfe900_pp_common_reg_data,
	},
	.rdi_hw_info            = vfe900_rdi_hw_info_arr,
	.wr_client_desc         = vfe900_wr_client_desc,
	.num_ipp_violation_errors = ARRAY_SIZE(vfe900_ipp_violation_desc),
	.ipp_violation_desc     = vfe900_ipp_violation_desc,
	.num_mux = 13,
	.mux_type = {
		CAM_VFE_CAMIF_VER_4_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
		CAM_VFE_RDI_VER_1_0,
	},
	.num_rdi                         = ARRAY_SIZE(vfe900_rdi_hw_info_arr),
	.num_top_errors                  = ARRAY_SIZE(vfe900_top_irq_err_desc),
	.top_err_desc                    = vfe900_top_irq_err_desc,
	.debug_reg_info                  = &vfe900_dbg_reg_info,
};

static struct cam_irq_register_set vfe900_bus_irq_reg[2] = {
	{
		.mask_reg_offset   = 0x00000718,
		.clear_reg_offset  = 0x00000720,
		.status_reg_offset = 0x00000728,
		.set_reg_offset    = 0x00000750,
	},
	{
		.mask_reg_offset   = 0x0000071C,
		.clear_reg_offset  = 0x00000724,
		.status_reg_offset = 0x0000072C,
		.set_reg_offset    = 0x00000754,
	},
};

static struct cam_vfe_bus_ver3_err_irq_desc vfe900_bus_irq_err_desc_0[] = {
	{
		.bitmask = BIT(26),
		.err_name = "IPCC_FENCE_DATA_ERR",
		.desc = "IPCC or FENCE Data was not available in the Input Fifo",
	},
	{
		.bitmask = BIT(27),
		.err_name = "IPCC_FENCE_ADDR_ERR",
		.desc = "IPCC or FENCE address fifo was empty and read was attempted",
	},
	{
		.bitmask = BIT(28),
		.err_name = "CONS_VIOLATION",
		.desc = "Programming of software registers violated the constraints",
	},
	{
		.bitmask = BIT(30),
		.err_name = "VIOLATION",
		.desc = "Client has a violation in ccif protocol at input",
	},
	{
		.bitmask = BIT(31),
		.err_name = "IMAGE_SIZE_VIOLATION",
		.desc = "Programmed image size is not same as image size from the CCIF",
	},
};

static struct cam_vfe_bus_ver3_err_irq_desc vfe900_bus_irq_err_desc_1[] = {
	{
		.bitmask = BIT(28),
		.err_name = "EARLY_DONE",
		.desc = "Buf done for each client. Early done irq for clients STATS_BAF",
	},
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_0 = { /* MAIN_C0 */
	.meta_addr        = 0x00000F40,
	.meta_cfg         = 0x00000F44,
	.mode_cfg         = 0x00000F48,
	.stats_ctrl       = 0x00000F4C,
	.ctrl_2           = 0x00000F50,
	.lossy_thresh0    = 0x00000F54,
	.lossy_thresh1    = 0x00000F58,
	.off_lossy_var    = 0x00000F5C,
	.bw_limit         = 0x00000F1C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_1 = { /* MAIN_C1 */
	.meta_addr        = 0x00001040,
	.meta_cfg         = 0x00001044,
	.mode_cfg         = 0x00001048,
	.stats_ctrl       = 0x0000104C,
	.ctrl_2           = 0x00001050,
	.lossy_thresh0    = 0x00001054,
	.lossy_thresh1    = 0x00001058,
	.off_lossy_var    = 0x0000105C,
	.bw_limit         = 0x0000101C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_2 = { /* MAIN_C2 */
	.meta_addr        = 0x00001140,
	.meta_cfg         = 0x00001144,
	.mode_cfg         = 0x00001148,
	.stats_ctrl       = 0x0000114C,
	.ctrl_2           = 0x00001150,
	.lossy_thresh0    = 0x00001154,
	.lossy_thresh1    = 0x00001158,
	.off_lossy_var    = 0x0000115C,
	.bw_limit         = 0x0000111C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_3 = { /* MAIN_UV */
	.meta_addr        = 0x00001240,
	.meta_cfg         = 0x00001244,
	.mode_cfg         = 0x00001248,
	.stats_ctrl       = 0x0000124C,
	.ctrl_2           = 0x00001250,
	.lossy_thresh0    = 0x00001254,
	.lossy_thresh1    = 0x00001258,
	.off_lossy_var    = 0x0000125C,
	.bw_limit         = 0x0000121C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_6 = { /* AI_1_C0 */
	.meta_addr        = 0x00001540,
	.meta_cfg         = 0x00001544,
	.mode_cfg         = 0x00001548,
	.stats_ctrl       = 0x0000154C,
	.ctrl_2           = 0x00001550,
	.lossy_thresh0    = 0x00001554,
	.lossy_thresh1    = 0x00001558,
	.off_lossy_var    = 0x0000155C,
	.bw_limit         = 0x0000151C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_7 = { /* AI_1_C1 */
	.meta_addr        = 0x00001640,
	.meta_cfg         = 0x00001644,
	.mode_cfg         = 0x00001648,
	.stats_ctrl       = 0x0000164C,
	.ctrl_2           = 0x00001650,
	.lossy_thresh0    = 0x00001654,
	.lossy_thresh1    = 0x00001658,
	.off_lossy_var    = 0x0000165C,
	.bw_limit         = 0x0000161C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_8 = { /* AI_1_C2 */
	.meta_addr        = 0x00001740,
	.meta_cfg         = 0x00001744,
	.mode_cfg         = 0x00001748,
	.stats_ctrl       = 0x0000174C,
	.ctrl_2           = 0x00001750,
	.lossy_thresh0    = 0x00001754,
	.lossy_thresh1    = 0x00001758,
	.off_lossy_var    = 0x0000175C,
	.bw_limit         = 0x0000171C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_9 = { /* AI_1_UV */
	.meta_addr        = 0x00001840,
	.meta_cfg         = 0x00001844,
	.mode_cfg         = 0x00001848,
	.stats_ctrl       = 0x0000184C,
	.ctrl_2           = 0x00001850,
	.lossy_thresh0    = 0x00001854,
	.lossy_thresh1    = 0x00001858,
	.off_lossy_var    = 0x0000185C,
	.bw_limit         = 0x0000181C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_10 = { /* AI_2_C0 */
	.meta_addr        = 0x00001940,
	.meta_cfg         = 0x00001944,
	.mode_cfg         = 0x00001948,
	.stats_ctrl       = 0x0000194C,
	.ctrl_2           = 0x00001950,
	.lossy_thresh0    = 0x00001954,
	.lossy_thresh1    = 0x00001958,
	.off_lossy_var    = 0x0000195C,
	.bw_limit         = 0x0000191C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_11 = { /* AI_2_C1 */
	.meta_addr        = 0x00001A40,
	.meta_cfg         = 0x00001A44,
	.mode_cfg         = 0x00001A48,
	.stats_ctrl       = 0x00001A4C,
	.ctrl_2           = 0x00001A50,
	.lossy_thresh0    = 0x00001A54,
	.lossy_thresh1    = 0x00001A58,
	.off_lossy_var    = 0x00001A5C,
	.bw_limit         = 0x00001A1C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_12 = { /* AI_2_C2 */
	.meta_addr        = 0x00001B40,
	.meta_cfg         = 0x00001B44,
	.mode_cfg         = 0x00001B48,
	.stats_ctrl       = 0x00001B4C,
	.ctrl_2           = 0x00001B50,
	.lossy_thresh0    = 0x00001B54,
	.lossy_thresh1    = 0x00001B58,
	.off_lossy_var    = 0x00001B5C,
	.bw_limit         = 0x00001B1C,
	.ubwc_comp_en_bit = BIT(1),
};

static struct cam_vfe_bus_ver3_reg_offset_ubwc_client
	vfe900_ubwc_regs_client_13 = { /* AI_2_UV */
	.meta_addr        = 0x00001C40,
	.meta_cfg         = 0x00001C44,
	.mode_cfg         = 0x00001C48,
	.stats_ctrl       = 0x00001C4C,
	.ctrl_2           = 0x00001C50,
	.lossy_thresh0    = 0x00001C54,
	.lossy_thresh1    = 0x00001C58,
	.off_lossy_var    = 0x00001C5C,
	.bw_limit         = 0x00001C1C,
	.ubwc_comp_en_bit = BIT(1),
};
static uint32_t vfe900_out_port_mid[][32] = {
	/* 0  FULL (MAIN): C0, C1, C2, UV - each data+meta (clients 0/1/2/3)  */
	{32, 34, 36, 38, 33, 35, 37, 39,
	 40, 42, 44, 46, 41, 43, 45, 47,
	 48, 50, 52, 54, 49, 51, 53, 55,
	 56, 58, 60, 62, 57, 59, 61, 63},
	/* 1  PIXEL_RAW (client 4): data+meta                                */
	{32, 34, 36, 38, 33, 35, 37, 39},
	/* 2  IR (W_IR, client 5): data+meta                                 */
	{40, 42, 44, 46, 41, 43, 45, 47},
	{32, 33, 34, 35},         /* 3  DS16 (HV_DS16, client 14)            */
	{36, 37, 38, 39},         /* 4  DS4 (HV_DS4, client 15)              */
	/* 5  AI_OUT_1: C0, C1, C2, UV - each data+meta (clients 6/7/8/9)     */
	{32, 34, 36, 38, 33, 35, 37, 39,
	 40, 42, 44, 46, 41, 43, 45, 47,
	 48, 50, 52, 54, 49, 51, 53, 55,
	 56, 58, 60, 62, 57, 59, 61, 63},
	/* 6  AI_OUT_2: C0, C1, C2, UV - each data+meta (clients 10/11/12/13) */
	{32, 34, 36, 38, 33, 35, 37, 39,
	 40, 42, 44, 46, 41, 43, 45, 47,
	 48, 50, 52, 54, 49, 51, 53, 55,
	 56, 58, 60, 62, 57, 59, 61, 63},
	{32, 33, 34, 35},         /* 7  STATS_IR_BG (client 16)              */
	{40, 41, 42, 43},         /* 8  STATS_IR_BHIST (client 17)           */
	{36, 37, 38, 39},         /* 9  STATS_TL_BG (BG_TINTLESS, client 18) */
	{40, 41, 42, 43},         /* 10 STATS_BG_IRC_AEC (client 19)         */
	{44, 45, 46, 47},         /* 11 STATS_BHIST_IRC_AEC (client 20)      */
	{44, 45, 46, 47},         /* 12 STATS_AEC_BG (client 21)            */
	{48, 49, 50, 51},         /* 13 STATS_AEC_BHIST (client 22)          */
	{48, 49, 50, 51},         /* 14 STATS_BAYER_RS (client 23)           */
	{52, 53, 54, 55},         /* 15 AWB_BFW (client 24)                  */
	{56, 57, 58, 59},         /* 16 STATS_AWB_BG (client 25)             */
	{60, 61, 62, 63},         /* 17 STATS_TMC_BHIST (client 26)          */
	{52, 53, 54, 55},         /* 18 LTM_STATS (BLTM, client 27)          */
	{48},                     /* 19 RDI_0  (client 28)                   */
	{49},                     /* 20 RDI_1  (client 29)                   */
	{50},                     /* 21 RDI_2  (client 30)                   */
	{51},                     /* 22 RDI_3  (client 31)                   */
	{52},                     /* 23 RDI_4  (client 32)                   */
	{53},                     /* 24 RDI_5  (client 33)                   */
	{54},                     /* 25 RDI_6  (client 34)                   */
	{55},                     /* 26 RDI_7  (client 35)                   */
	{56},                     /* 27 RDI_8  (client 36)                   */
	{57},                     /* 28 RDI_9  (client 37)                   */
	{58},                     /* 29 RDI_10 (client 38)                   */
	{59},                     /* 30 RDI_11 (client 39)                   */
};

static struct cam_vfe_bus_ver3_hw_info vfe900_bus_hw_info = {
	.common_reg = {
		.hw_version                       = 0x00000700,
		.cgc_ovd                          = 0x00000708,
		.ctxt_sel                         = 0x0000083C,
		.ctxt_rd_sel                      = 0x00000840,
		.mc_write_sel_shift               = 0x0,
		.mc_ctxt_mask                     = 0x3,
		.ubwc_static_ctrl                 = 0x00000758,
		.pwr_iso_cfg                      = 0x0000075C,
		.overflow_status_clear            = 0x00000760,
		.ccif_violation_status            = 0x00000764,
		.overflow_status                  = 0x00000768,
		.image_size_violation_status      = 0x00000770,
		.debug_status_top_cfg             = 0x000007F0,
		.debug_status_top                 = 0x000007F4,
		.test_bus_ctrl                    = 0x00000844,
		.irq_reg_info = {
			.num_registers            = 2,
			.irq_reg_set              = vfe900_bus_irq_reg,
			.global_irq_cmd_offset    = 0x00000730,
			.global_clear_bitmask     = 0x00000001,
		},
	},
	.num_client = CAM_VFE_BUS_VER3_900_MAX_CLIENTS,
	.bus_client_reg = {
		/* BUS Client 0 MAIN_C0 */
		{
			.cfg                       = 0x00000F00,
			.image_addr                = 0x00000F04,
			.frame_incr                = 0x00000F08,
			.image_cfg_0               = 0x00000F0C,
			.image_cfg_1               = 0x00000F10,
			.image_cfg_2               = 0x00000F14,
			.packer_cfg                = 0x00000F18,
			.frame_header_addr         = 0x00000F20,
			.frame_header_incr         = 0x00000F24,
			.frame_header_cfg          = 0x00000F28,
			.irq_subsample_period      = 0x00000F30,
			.irq_subsample_pattern     = 0x00000F34,
			.framedrop_period          = 0x00000F38,
			.framedrop_pattern         = 0x00000F3C,
			.mmu_prefetch_cfg          = 0x00000F60,
			.mmu_prefetch_max_offset   = 0x00000F64,
			.system_cache_cfg          = 0x00000F68,
			.ctxt_cfg                  = 0x00000F78,
			.addr_status_0             = 0x00000F90,
			.addr_status_1             = 0x00000F94,
			.addr_status_2             = 0x00000F98,
			.addr_status_3             = 0x00000F9C,
			.debug_status_cfg          = 0x00000F7C,
			.debug_status_0            = 0x00000F80,
			.debug_status_1            = 0x00000F84,
			.debug_status_ctxt         = 0x00000F88,
			.bw_limiter_addr           = 0x00000F1C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_0,
			.supported_formats        =  BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |  BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 1 MAIN_C1 */
		{
			.cfg                       = 0x00001000,
			.image_addr                = 0x00001004,
			.frame_incr                = 0x00001008,
			.image_cfg_0               = 0x0000100C,
			.image_cfg_1               = 0x00001010,
			.image_cfg_2               = 0x00001014,
			.packer_cfg                = 0x00001018,
			.frame_header_addr         = 0x00001020,
			.frame_header_incr         = 0x00001024,
			.frame_header_cfg          = 0x00001028,
			.irq_subsample_period      = 0x00001030,
			.irq_subsample_pattern     = 0x00001034,
			.framedrop_period          = 0x00001038,
			.framedrop_pattern         = 0x0000103C,
			.mmu_prefetch_cfg          = 0x00001060,
			.mmu_prefetch_max_offset   = 0x00001064,
			.system_cache_cfg          = 0x00001068,
			.ctxt_cfg                  = 0x00001078,
			.addr_status_0             = 0x00001090,
			.addr_status_1             = 0x00001094,
			.addr_status_2             = 0x00001098,
			.addr_status_3             = 0x0000109C,
			.debug_status_cfg          = 0x0000107C,
			.debug_status_0            = 0x00001080,
			.debug_status_1            = 0x00001084,
			.debug_status_ctxt         = 0x00001088,
			.bw_limiter_addr           = 0x0000101C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_1,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 2 MAIN_C2 */
		{
			.cfg                       = 0x00001100,
			.image_addr                = 0x00001104,
			.frame_incr                = 0x00001108,
			.image_cfg_0               = 0x0000110C,
			.image_cfg_1               = 0x00001110,
			.image_cfg_2               = 0x00001114,
			.packer_cfg                = 0x00001118,
			.frame_header_addr         = 0x00001120,
			.frame_header_incr         = 0x00001124,
			.frame_header_cfg          = 0x00001128,
			.irq_subsample_period      = 0x00001130,
			.irq_subsample_pattern     = 0x00001134,
			.framedrop_period          = 0x00001138,
			.framedrop_pattern         = 0x0000113C,
			.mmu_prefetch_cfg          = 0x00001160,
			.mmu_prefetch_max_offset   = 0x00001164,
			.system_cache_cfg          = 0x00001168,
			.ctxt_cfg                  = 0x00001178,
			.addr_status_0             = 0x00001190,
			.addr_status_1             = 0x00001194,
			.addr_status_2             = 0x00001198,
			.addr_status_3             = 0x0000119C,
			.debug_status_cfg          = 0x0000117C,
			.debug_status_0            = 0x00001180,
			.debug_status_1            = 0x00001184,
			.debug_status_ctxt         = 0x00001188,
			.bw_limiter_addr           = 0x0000111C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_2,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 3 MAIN_UV */
		{
			.cfg                       = 0x00001200,
			.image_addr                = 0x00001204,
			.frame_incr                = 0x00001208,
			.image_cfg_0               = 0x0000120C,
			.image_cfg_1               = 0x00001210,
			.image_cfg_2               = 0x00001214,
			.packer_cfg                = 0x00001218,
			.frame_header_addr         = 0x00001220,
			.frame_header_incr         = 0x00001224,
			.frame_header_cfg          = 0x00001228,
			.irq_subsample_period      = 0x00001230,
			.irq_subsample_pattern     = 0x00001234,
			.framedrop_period          = 0x00001238,
			.framedrop_pattern         = 0x0000123C,
			.mmu_prefetch_cfg          = 0x00001260,
			.mmu_prefetch_max_offset   = 0x00001264,
			.system_cache_cfg          = 0x00001268,
			.ctxt_cfg                  = 0x00001278,
			.addr_status_0             = 0x00001290,
			.addr_status_1             = 0x00001294,
			.addr_status_2             = 0x00001298,
			.addr_status_3             = 0x0000129C,
			.debug_status_cfg          = 0x0000127C,
			.debug_status_0            = 0x00001280,
			.debug_status_1            = 0x00001284,
			.debug_status_ctxt         = 0x00001288,
			.bw_limiter_addr           = 0x0000121C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_3,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 4 PIXEL_RAW */
		{
			.cfg                       = 0x00001300,
			.image_addr                = 0x00001304,
			.frame_incr                = 0x00001308,
			.image_cfg_0               = 0x0000130C,
			.image_cfg_1               = 0x00001310,
			.image_cfg_2               = 0x00001314,
			.packer_cfg                = 0x00001318,
			.frame_header_addr         = 0x00001320,
			.frame_header_incr         = 0x00001324,
			.frame_header_cfg          = 0x00001328,
			.irq_subsample_period      = 0x00001330,
			.irq_subsample_pattern     = 0x00001334,
			.framedrop_period          = 0x00001338,
			.framedrop_pattern         = 0x0000133C,
			.mmu_prefetch_cfg          = 0x00001360,
			.mmu_prefetch_max_offset   = 0x00001364,
			.system_cache_cfg          = 0x00001368,
			.ctxt_cfg                  = 0x00001378,
			.addr_status_0             = 0x00001390,
			.addr_status_1             = 0x00001394,
			.addr_status_2             = 0x00001398,
			.addr_status_3             = 0x0000139C,
			.debug_status_cfg          = 0x0000137C,
			.debug_status_0            = 0x00001380,
			.debug_status_1            = 0x00001384,
			.debug_status_ctxt         = 0x00001388,
			.bw_limiter_addr           = 0x0000131C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16) | BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_12) | BIT_ULL(CAM_FORMAT_MIPI_RAW_14),
		},
		/* BUS Client 5 W_IR */
		{
			.cfg                       = 0x00001400,
			.image_addr                = 0x00001404,
			.frame_incr                = 0x00001408,
			.image_cfg_0               = 0x0000140C,
			.image_cfg_1               = 0x00001410,
			.image_cfg_2               = 0x00001414,
			.packer_cfg                = 0x00001418,
			.frame_header_addr         = 0x00001420,
			.frame_header_incr         = 0x00001424,
			.frame_header_cfg          = 0x00001428,
			.irq_subsample_period      = 0x00001430,
			.irq_subsample_pattern     = 0x00001434,
			.framedrop_period          = 0x00001438,
			.framedrop_pattern         = 0x0000143C,
			.mmu_prefetch_cfg          = 0x00001460,
			.mmu_prefetch_max_offset   = 0x00001464,
			.system_cache_cfg          = 0x00001468,
			.ctxt_cfg                  = 0x00001478,
			.addr_status_0             = 0x00001490,
			.addr_status_1             = 0x00001494,
			.addr_status_2             = 0x00001498,
			.addr_status_3             = 0x0000149C,
			.debug_status_cfg          = 0x0000147C,
			.debug_status_0            = 0x00001480,
			.debug_status_1            = 0x00001484,
			.debug_status_ctxt         = 0x00001488,
			.bw_limiter_addr           = 0x0000141C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16) | BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_12) | BIT_ULL(CAM_FORMAT_MIPI_RAW_14),
		},
		/* BUS Client 6 AI_1_C0 */
		{
			.cfg                       = 0x00001500,
			.image_addr                = 0x00001504,
			.frame_incr                = 0x00001508,
			.image_cfg_0               = 0x0000150C,
			.image_cfg_1               = 0x00001510,
			.image_cfg_2               = 0x00001514,
			.packer_cfg                = 0x00001518,
			.frame_header_addr         = 0x00001520,
			.frame_header_incr         = 0x00001524,
			.frame_header_cfg          = 0x00001528,
			.irq_subsample_period      = 0x00001530,
			.irq_subsample_pattern     = 0x00001534,
			.framedrop_period          = 0x00001538,
			.framedrop_pattern         = 0x0000153C,
			.mmu_prefetch_cfg          = 0x00001560,
			.mmu_prefetch_max_offset   = 0x00001564,
			.system_cache_cfg          = 0x00001568,
			.ctxt_cfg                  = 0x00001578,
			.addr_status_0             = 0x00001590,
			.addr_status_1             = 0x00001594,
			.addr_status_2             = 0x00001598,
			.addr_status_3             = 0x0000159C,
			.debug_status_cfg          = 0x0000157C,
			.debug_status_0            = 0x00001580,
			.debug_status_1            = 0x00001584,
			.debug_status_ctxt         = 0x00001588,
			.bw_limiter_addr           = 0x0000151C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_6,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 7 AI_1_C1 */
		{
			.cfg                       = 0x00001600,
			.image_addr                = 0x00001604,
			.frame_incr                = 0x00001608,
			.image_cfg_0               = 0x0000160C,
			.image_cfg_1               = 0x00001610,
			.image_cfg_2               = 0x00001614,
			.packer_cfg                = 0x00001618,
			.frame_header_addr         = 0x00001620,
			.frame_header_incr         = 0x00001624,
			.frame_header_cfg          = 0x00001628,
			.irq_subsample_period      = 0x00001630,
			.irq_subsample_pattern     = 0x00001634,
			.framedrop_period          = 0x00001638,
			.framedrop_pattern         = 0x0000163C,
			.mmu_prefetch_cfg          = 0x00001660,
			.mmu_prefetch_max_offset   = 0x00001664,
			.system_cache_cfg          = 0x00001668,
			.ctxt_cfg                  = 0x00001678,
			.addr_status_0             = 0x00001690,
			.addr_status_1             = 0x00001694,
			.addr_status_2             = 0x00001698,
			.addr_status_3             = 0x0000169C,
			.debug_status_cfg          = 0x0000167C,
			.debug_status_0            = 0x00001680,
			.debug_status_1            = 0x00001684,
			.debug_status_ctxt         = 0x00001688,
			.bw_limiter_addr           = 0x0000161C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_7,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 8 AI_1_C2 */
		{
			.cfg                       = 0x00001700,
			.image_addr                = 0x00001704,
			.frame_incr                = 0x00001708,
			.image_cfg_0               = 0x0000170C,
			.image_cfg_1               = 0x00001710,
			.image_cfg_2               = 0x00001714,
			.packer_cfg                = 0x00001718,
			.frame_header_addr         = 0x00001720,
			.frame_header_incr         = 0x00001724,
			.frame_header_cfg          = 0x00001728,
			.irq_subsample_period      = 0x00001730,
			.irq_subsample_pattern     = 0x00001734,
			.framedrop_period          = 0x00001738,
			.framedrop_pattern         = 0x0000173C,
			.mmu_prefetch_cfg          = 0x00001760,
			.mmu_prefetch_max_offset   = 0x00001764,
			.system_cache_cfg          = 0x00001768,
			.ctxt_cfg                  = 0x00001778,
			.addr_status_0             = 0x00001790,
			.addr_status_1             = 0x00001794,
			.addr_status_2             = 0x00001798,
			.addr_status_3             = 0x0000179C,
			.debug_status_cfg          = 0x0000177C,
			.debug_status_0            = 0x00001780,
			.debug_status_1            = 0x00001784,
			.debug_status_ctxt         = 0x00001788,
			.bw_limiter_addr           = 0x0000171C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_8,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 9 AI_1_UV */
		{
			.cfg                       = 0x00001800,
			.image_addr                = 0x00001804,
			.frame_incr                = 0x00001808,
			.image_cfg_0               = 0x0000180C,
			.image_cfg_1               = 0x00001810,
			.image_cfg_2               = 0x00001814,
			.packer_cfg                = 0x00001818,
			.frame_header_addr         = 0x00001820,
			.frame_header_incr         = 0x00001824,
			.frame_header_cfg          = 0x00001828,
			.irq_subsample_period      = 0x00001830,
			.irq_subsample_pattern     = 0x00001834,
			.framedrop_period          = 0x00001838,
			.framedrop_pattern         = 0x0000183C,
			.mmu_prefetch_cfg          = 0x00001860,
			.mmu_prefetch_max_offset   = 0x00001864,
			.system_cache_cfg          = 0x00001868,
			.ctxt_cfg                  = 0x00001878,
			.addr_status_0             = 0x00001890,
			.addr_status_1             = 0x00001894,
			.addr_status_2             = 0x00001898,
			.addr_status_3             = 0x0000189C,
			.debug_status_cfg          = 0x0000187C,
			.debug_status_0            = 0x00001880,
			.debug_status_1            = 0x00001884,
			.debug_status_ctxt         = 0x00001888,
			.bw_limiter_addr           = 0x0000181C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_9,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 10 AI_2_C0 */
		{
			.cfg                       = 0x00001900,
			.image_addr                = 0x00001904,
			.frame_incr                = 0x00001908,
			.image_cfg_0               = 0x0000190C,
			.image_cfg_1               = 0x00001910,
			.image_cfg_2               = 0x00001914,
			.packer_cfg                = 0x00001918,
			.frame_header_addr         = 0x00001920,
			.frame_header_incr         = 0x00001924,
			.frame_header_cfg          = 0x00001928,
			.irq_subsample_period      = 0x00001930,
			.irq_subsample_pattern     = 0x00001934,
			.framedrop_period          = 0x00001938,
			.framedrop_pattern         = 0x0000193C,
			.mmu_prefetch_cfg          = 0x00001960,
			.mmu_prefetch_max_offset   = 0x00001964,
			.system_cache_cfg          = 0x00001968,
			.ctxt_cfg                  = 0x00001978,
			.addr_status_0             = 0x00001990,
			.addr_status_1             = 0x00001994,
			.addr_status_2             = 0x00001998,
			.addr_status_3             = 0x0000199C,
			.debug_status_cfg          = 0x0000197C,
			.debug_status_0            = 0x00001980,
			.debug_status_1            = 0x00001984,
			.debug_status_ctxt         = 0x00001988,
			.bw_limiter_addr           = 0x0000191C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_10,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 11 AI_2_C1 */
		{
			.cfg                       = 0x00001A00,
			.image_addr                = 0x00001A04,
			.frame_incr                = 0x00001A08,
			.image_cfg_0               = 0x00001A0C,
			.image_cfg_1               = 0x00001A10,
			.image_cfg_2               = 0x00001A14,
			.packer_cfg                = 0x00001A18,
			.frame_header_addr         = 0x00001A20,
			.frame_header_incr         = 0x00001A24,
			.frame_header_cfg          = 0x00001A28,
			.irq_subsample_period      = 0x00001A30,
			.irq_subsample_pattern     = 0x00001A34,
			.framedrop_period          = 0x00001A38,
			.framedrop_pattern         = 0x00001A3C,
			.mmu_prefetch_cfg          = 0x00001A60,
			.mmu_prefetch_max_offset   = 0x00001A64,
			.system_cache_cfg          = 0x00001A68,
			.ctxt_cfg                  = 0x00001A78,
			.addr_status_0             = 0x00001A90,
			.addr_status_1             = 0x00001A94,
			.addr_status_2             = 0x00001A98,
			.addr_status_3             = 0x00001A9C,
			.debug_status_cfg          = 0x00001A7C,
			.debug_status_0            = 0x00001A80,
			.debug_status_1            = 0x00001A84,
			.debug_status_ctxt         = 0x00001A88,
			.bw_limiter_addr           = 0x00001A1C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_11,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 12 AI_2_C2 */
		{
			.cfg                       = 0x00001B00,
			.image_addr                = 0x00001B04,
			.frame_incr                = 0x00001B08,
			.image_cfg_0               = 0x00001B0C,
			.image_cfg_1               = 0x00001B10,
			.image_cfg_2               = 0x00001B14,
			.packer_cfg                = 0x00001B18,
			.frame_header_addr         = 0x00001B20,
			.frame_header_incr         = 0x00001B24,
			.frame_header_cfg          = 0x00001B28,
			.irq_subsample_period      = 0x00001B30,
			.irq_subsample_pattern     = 0x00001B34,
			.framedrop_period          = 0x00001B38,
			.framedrop_pattern         = 0x00001B3C,
			.mmu_prefetch_cfg          = 0x00001B60,
			.mmu_prefetch_max_offset   = 0x00001B64,
			.system_cache_cfg          = 0x00001B68,
			.ctxt_cfg                  = 0x00001B78,
			.addr_status_0             = 0x00001B90,
			.addr_status_1             = 0x00001B94,
			.addr_status_2             = 0x00001B98,
			.addr_status_3             = 0x00001B9C,
			.debug_status_cfg          = 0x00001B7C,
			.debug_status_0            = 0x00001B80,
			.debug_status_1            = 0x00001B84,
			.debug_status_ctxt         = 0x00001B88,
			.bw_limiter_addr           = 0x00001B1C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_12,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 13 AI_2_UV */
		{
			.cfg                       = 0x00001C00,
			.image_addr                = 0x00001C04,
			.frame_incr                = 0x00001C08,
			.image_cfg_0               = 0x00001C0C,
			.image_cfg_1               = 0x00001C10,
			.image_cfg_2               = 0x00001C14,
			.packer_cfg                = 0x00001C18,
			.frame_header_addr         = 0x00001C20,
			.frame_header_incr         = 0x00001C24,
			.frame_header_cfg          = 0x00001C28,
			.irq_subsample_period      = 0x00001C30,
			.irq_subsample_pattern     = 0x00001C34,
			.framedrop_period          = 0x00001C38,
			.framedrop_pattern         = 0x00001C3C,
			.mmu_prefetch_cfg          = 0x00001C60,
			.mmu_prefetch_max_offset   = 0x00001C64,
			.system_cache_cfg          = 0x00001C68,
			.ctxt_cfg                  = 0x00001C78,
			.addr_status_0             = 0x00001C90,
			.addr_status_1             = 0x00001C94,
			.addr_status_2             = 0x00001C98,
			.addr_status_3             = 0x00001C9C,
			.debug_status_cfg          = 0x00001C7C,
			.debug_status_0            = 0x00001C80,
			.debug_status_1            = 0x00001C84,
			.debug_status_ctxt         = 0x00001C88,
			.bw_limiter_addr           = 0x00001C1C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.ubwc_regs                = &vfe900_ubwc_regs_client_13,
			.supported_formats        = BIT_ULL(CAM_FORMAT_UBWC_NV12) |
				BIT_ULL(CAM_FORMAT_NV12) |
				BIT_ULL(CAM_FORMAT_BAYER_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_10) | BIT_ULL(CAM_FORMAT_MIPI_RAW_12) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_14) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16),
		},
		/* BUS Client 14 HV_DS16 */
		{
			.cfg                       = 0x00001D00,
			.image_addr                = 0x00001D04,
			.frame_incr                = 0x00001D08,
			.image_cfg_0               = 0x00001D0C,
			.image_cfg_1               = 0x00001D10,
			.image_cfg_2               = 0x00001D14,
			.packer_cfg                = 0x00001D18,
			.frame_header_addr         = 0x00001D20,
			.frame_header_incr         = 0x00001D24,
			.frame_header_cfg          = 0x00001D28,
			.irq_subsample_period      = 0x00001D30,
			.irq_subsample_pattern     = 0x00001D34,
			.framedrop_period          = 0x00001D38,
			.framedrop_pattern         = 0x00001D3C,
			.mmu_prefetch_cfg          = 0x00001D60,
			.mmu_prefetch_max_offset   = 0x00001D64,
			.system_cache_cfg          = 0x00001D68,
			.ctxt_cfg                  = 0x00001D78,
			.addr_status_0             = 0x00001D90,
			.addr_status_1             = 0x00001D94,
			.addr_status_2             = 0x00001D98,
			.addr_status_3             = 0x00001D9C,
			.debug_status_cfg          = 0x00001D7C,
			.debug_status_0            = 0x00001D80,
			.debug_status_1            = 0x00001D84,
			.debug_status_ctxt         = 0x00001D88,
			.bw_limiter_addr           = 0x00001D1C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.supported_formats        = BIT_ULL(CAM_FORMAT_TP10) | BIT_ULL(CAM_FORMAT_PD10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) | BIT_ULL(CAM_FORMAT_GBR_TP10) |
				BIT_ULL(CAM_FORMAT_GBR_UBWC_TP10),
		},
		/* BUS Client 15 HV_DS4 */
		{
			.cfg                       = 0x00001E00,
			.image_addr                = 0x00001E04,
			.frame_incr                = 0x00001E08,
			.image_cfg_0               = 0x00001E0C,
			.image_cfg_1               = 0x00001E10,
			.image_cfg_2               = 0x00001E14,
			.packer_cfg                = 0x00001E18,
			.frame_header_addr         = 0x00001E20,
			.frame_header_incr         = 0x00001E24,
			.frame_header_cfg          = 0x00001E28,
			.irq_subsample_period      = 0x00001E30,
			.irq_subsample_pattern     = 0x00001E34,
			.framedrop_period          = 0x00001E38,
			.framedrop_pattern         = 0x00001E3C,
			.mmu_prefetch_cfg          = 0x00001E60,
			.mmu_prefetch_max_offset   = 0x00001E64,
			.system_cache_cfg          = 0x00001E68,
			.ctxt_cfg                  = 0x00001E78,
			.addr_status_0             = 0x00001E90,
			.addr_status_1             = 0x00001E94,
			.addr_status_2             = 0x00001E98,
			.addr_status_3             = 0x00001E9C,
			.debug_status_cfg          = 0x00001E7C,
			.debug_status_0            = 0x00001E80,
			.debug_status_1            = 0x00001E84,
			.debug_status_ctxt         = 0x00001E88,
			.bw_limiter_addr           = 0x00001E1C,
			/* IMG comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP0 bits 3:0) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_0,
			.supported_formats        = BIT_ULL(CAM_FORMAT_TP10) |  BIT_ULL(CAM_FORMAT_PD10) |
				BIT_ULL(CAM_FORMAT_UBWC_TP10) | BIT_ULL(CAM_FORMAT_GBR_TP10) |
				BIT_ULL(CAM_FORMAT_GBR_UBWC_TP10),
		},
		/* BUS Client 16 STATS_BG_IR */
		{
			.cfg                       = 0x00001F00,
			.image_addr                = 0x00001F04,
			.frame_incr                = 0x00001F08,
			.image_cfg_0               = 0x00001F0C,
			.image_cfg_1               = 0x00001F10,
			.image_cfg_2               = 0x00001F14,
			.packer_cfg                = 0x00001F18,
			.frame_header_addr         = 0x00001F20,
			.frame_header_incr         = 0x00001F24,
			.frame_header_cfg          = 0x00001F28,
			.line_done_cfg             = 0x00001F2C,
			.irq_subsample_period      = 0x00001F30,
			.irq_subsample_pattern     = 0x00001F34,
			.framedrop_period          = 0x00001F38,
			.framedrop_pattern         = 0x00001F3C,
			.mmu_prefetch_cfg          = 0x00001F60,
			.mmu_prefetch_max_offset   = 0x00001F64,
			.system_cache_cfg          = 0x00001F68,
			.ctxt_cfg                  = 0x00001F78,
			.addr_status_0             = 0x00001F90,
			.addr_status_1             = 0x00001F94,
			.addr_status_2             = 0x00001F98,
			.addr_status_3             = 0x00001F9C,
			.debug_status_cfg          = 0x00001F7C,
			.debug_status_0            = 0x00001F80,
			.debug_status_1            = 0x00001F84,
			.debug_status_ctxt         = 0x00001F88,
			.bw_limiter_addr           = 0x00001F1C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 17 STATS_BHIST_IR */
		{
			.cfg                       = 0x00002000,
			.image_addr                = 0x00002004,
			.frame_incr                = 0x00002008,
			.image_cfg_0               = 0x0000200C,
			.image_cfg_1               = 0x00002010,
			.image_cfg_2               = 0x00002014,
			.packer_cfg                = 0x00002018,
			.frame_header_addr         = 0x00002020,
			.frame_header_incr         = 0x00002024,
			.frame_header_cfg          = 0x00002028,
			.irq_subsample_period      = 0x00002030,
			.irq_subsample_pattern     = 0x00002034,
			.framedrop_period          = 0x00002038,
			.framedrop_pattern         = 0x0000203C,
			.mmu_prefetch_cfg          = 0x00002060,
			.mmu_prefetch_max_offset   = 0x00002064,
			.system_cache_cfg          = 0x00002068,
			.ctxt_cfg                  = 0x00002078,
			.addr_status_0             = 0x00002090,
			.addr_status_1             = 0x00002094,
			.addr_status_2             = 0x00002098,
			.addr_status_3             = 0x0000209C,
			.debug_status_cfg          = 0x0000207C,
			.debug_status_0            = 0x00002080,
			.debug_status_1            = 0x00002084,
			.debug_status_ctxt         = 0x00002088,
			.bw_limiter_addr           = 0x0000201C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 18 STATS_BG_TINTLESS */
		{
			.cfg                       = 0x00002100,
			.image_addr                = 0x00002104,
			.frame_incr                = 0x00002108,
			.image_cfg_0               = 0x0000210C,
			.image_cfg_1               = 0x00002110,
			.image_cfg_2               = 0x00002114,
			.packer_cfg                = 0x00002118,
			.frame_header_addr         = 0x00002120,
			.frame_header_incr         = 0x00002124,
			.frame_header_cfg          = 0x00002128,
			.irq_subsample_period      = 0x00002130,
			.irq_subsample_pattern     = 0x00002134,
			.framedrop_period          = 0x00002138,
			.framedrop_pattern         = 0x0000213C,
			.mmu_prefetch_cfg          = 0x00002160,
			.mmu_prefetch_max_offset   = 0x00002164,
			.system_cache_cfg          = 0x00002168,
			.ctxt_cfg                  = 0x00002178,
			.addr_status_0             = 0x00002190,
			.addr_status_1             = 0x00002194,
			.addr_status_2             = 0x00002198,
			.addr_status_3             = 0x0000219C,
			.debug_status_cfg          = 0x0000217C,
			.debug_status_0            = 0x00002180,
			.debug_status_1            = 0x00002184,
			.debug_status_ctxt         = 0x00002188,
			.bw_limiter_addr           = 0x0000211C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 19 STATS_BG_IRC_AEC */
		{
			.cfg                       = 0x00002200,
			.image_addr                = 0x00002204,
			.frame_incr                = 0x00002208,
			.image_cfg_0               = 0x0000220C,
			.image_cfg_1               = 0x00002210,
			.image_cfg_2               = 0x00002214,
			.packer_cfg                = 0x00002218,
			.frame_header_addr         = 0x00002220,
			.frame_header_incr         = 0x00002224,
			.frame_header_cfg          = 0x00002228,
			.irq_subsample_period      = 0x00002230,
			.irq_subsample_pattern     = 0x00002234,
			.framedrop_period          = 0x00002238,
			.framedrop_pattern         = 0x0000223C,
			.mmu_prefetch_cfg          = 0x00002260,
			.mmu_prefetch_max_offset   = 0x00002264,
			.system_cache_cfg          = 0x00002268,
			.ctxt_cfg                  = 0x00002278,
			.addr_status_0             = 0x00002290,
			.addr_status_1             = 0x00002294,
			.addr_status_2             = 0x00002298,
			.addr_status_3             = 0x0000229C,
			.debug_status_cfg          = 0x0000227C,
			.debug_status_0            = 0x00002280,
			.debug_status_1            = 0x00002284,
			.debug_status_ctxt         = 0x00002288,
			.bw_limiter_addr           = 0x0000221C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 20 STATS_BHIST_IRC_AEC */
		{
			.cfg                       = 0x00002300,
			.image_addr                = 0x00002304,
			.frame_incr                = 0x00002308,
			.image_cfg_0               = 0x0000230C,
			.image_cfg_1               = 0x00002310,
			.image_cfg_2               = 0x00002314,
			.packer_cfg                = 0x00002318,
			.frame_header_addr         = 0x00002320,
			.frame_header_incr         = 0x00002324,
			.frame_header_cfg          = 0x00002328,
			.irq_subsample_period      = 0x00002330,
			.irq_subsample_pattern     = 0x00002334,
			.framedrop_period          = 0x00002338,
			.framedrop_pattern         = 0x0000233C,
			.mmu_prefetch_cfg          = 0x00002360,
			.mmu_prefetch_max_offset   = 0x00002364,
			.system_cache_cfg          = 0x00002368,
			.ctxt_cfg                  = 0x00002378,
			.addr_status_0             = 0x00002390,
			.addr_status_1             = 0x00002394,
			.addr_status_2             = 0x00002398,
			.addr_status_3             = 0x0000239C,
			.debug_status_cfg          = 0x0000237C,
			.debug_status_0            = 0x00002380,
			.debug_status_1            = 0x00002384,
			.debug_status_ctxt         = 0x00002388,
			.bw_limiter_addr           = 0x0000231C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 21 STATS_BG_AEC */
		{
			.cfg                       = 0x00002400,
			.image_addr                = 0x00002404,
			.frame_incr                = 0x00002408,
			.image_cfg_0               = 0x0000240C,
			.image_cfg_1               = 0x00002410,
			.image_cfg_2               = 0x00002414,
			.packer_cfg                = 0x00002418,
			.frame_header_addr         = 0x00002420,
			.frame_header_incr         = 0x00002424,
			.frame_header_cfg          = 0x00002428,
			.irq_subsample_period      = 0x00002430,
			.irq_subsample_pattern     = 0x00002434,
			.framedrop_period          = 0x00002438,
			.framedrop_pattern         = 0x0000243C,
			.mmu_prefetch_cfg          = 0x00002460,
			.mmu_prefetch_max_offset   = 0x00002464,
			.system_cache_cfg          = 0x00002468,
			.ctxt_cfg                  = 0x00002478,
			.addr_status_0             = 0x00002490,
			.addr_status_1             = 0x00002494,
			.addr_status_2             = 0x00002498,
			.addr_status_3             = 0x0000249C,
			.debug_status_cfg          = 0x0000247C,
			.debug_status_0            = 0x00002480,
			.debug_status_1            = 0x00002484,
			.debug_status_ctxt         = 0x00002488,
			.bw_limiter_addr           = 0x0000241C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 22 STATS_BHIST_AEC */
		{
			.cfg                       = 0x00002500,
			.image_addr                = 0x00002504,
			.frame_incr                = 0x00002508,
			.image_cfg_0               = 0x0000250C,
			.image_cfg_1               = 0x00002510,
			.image_cfg_2               = 0x00002514,
			.packer_cfg                = 0x00002518,
			.frame_header_addr         = 0x00002520,
			.frame_header_incr         = 0x00002524,
			.frame_header_cfg          = 0x00002528,
			.irq_subsample_period      = 0x00002530,
			.irq_subsample_pattern     = 0x00002534,
			.framedrop_period          = 0x00002538,
			.framedrop_pattern         = 0x0000253C,
			.mmu_prefetch_cfg          = 0x00002560,
			.mmu_prefetch_max_offset   = 0x00002564,
			.system_cache_cfg          = 0x00002568,
			.ctxt_cfg                  = 0x00002578,
			.addr_status_0             = 0x00002590,
			.addr_status_1             = 0x00002594,
			.addr_status_2             = 0x00002598,
			.addr_status_3             = 0x0000259C,
			.debug_status_cfg          = 0x0000257C,
			.debug_status_0            = 0x00002580,
			.debug_status_1            = 0x00002584,
			.debug_status_ctxt         = 0x00002588,
			.bw_limiter_addr           = 0x0000251C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 23 STATS_BAYER_RS */
		{
			.cfg                       = 0x00002600,
			.image_addr                = 0x00002604,
			.frame_incr                = 0x00002608,
			.image_cfg_0               = 0x0000260C,
			.image_cfg_1               = 0x00002610,
			.image_cfg_2               = 0x00002614,
			.packer_cfg                = 0x00002618,
			.frame_header_addr         = 0x00002620,
			.frame_header_incr         = 0x00002624,
			.frame_header_cfg          = 0x00002628,
			.irq_subsample_period      = 0x00002630,
			.irq_subsample_pattern     = 0x00002634,
			.framedrop_period          = 0x00002638,
			.framedrop_pattern         = 0x0000263C,
			.mmu_prefetch_cfg          = 0x00002660,
			.mmu_prefetch_max_offset   = 0x00002664,
			.system_cache_cfg          = 0x00002668,
			.ctxt_cfg                  = 0x00002678,
			.addr_status_0             = 0x00002690,
			.addr_status_1             = 0x00002694,
			.addr_status_2             = 0x00002698,
			.addr_status_3             = 0x0000269C,
			.debug_status_cfg          = 0x0000267C,
			.debug_status_0            = 0x00002680,
			.debug_status_1            = 0x00002684,
			.debug_status_ctxt         = 0x00002688,
			.bw_limiter_addr           = 0x0000261C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 24 STATS_BFW_AWB */
		{
			.cfg                       = 0x00002700,
			.image_addr                = 0x00002704,
			.frame_incr                = 0x00002708,
			.image_cfg_0               = 0x0000270C,
			.image_cfg_1               = 0x00002710,
			.image_cfg_2               = 0x00002714,
			.packer_cfg                = 0x00002718,
			.frame_header_addr         = 0x00002720,
			.frame_header_incr         = 0x00002724,
			.frame_header_cfg          = 0x00002728,
			.line_done_cfg             = 0x0000272C,
			.irq_subsample_period      = 0x00002730,
			.irq_subsample_pattern     = 0x00002734,
			.framedrop_period          = 0x00002738,
			.framedrop_pattern         = 0x0000273C,
			.mmu_prefetch_cfg          = 0x00002760,
			.mmu_prefetch_max_offset   = 0x00002764,
			.system_cache_cfg          = 0x00002768,
			.ctxt_cfg                  = 0x00002778,
			.addr_status_0             = 0x00002790,
			.addr_status_1             = 0x00002794,
			.addr_status_2             = 0x00002798,
			.addr_status_3             = 0x0000279C,
			.debug_status_cfg          = 0x0000277C,
			.debug_status_0            = 0x00002780,
			.debug_status_1            = 0x00002784,
			.debug_status_ctxt         = 0x00002788,
			.bw_limiter_addr           = 0x0000271C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 25 STATS_BG_AWB */
		{
			.cfg                       = 0x00002800,
			.image_addr                = 0x00002804,
			.frame_incr                = 0x00002808,
			.image_cfg_0               = 0x0000280C,
			.image_cfg_1               = 0x00002810,
			.image_cfg_2               = 0x00002814,
			.packer_cfg                = 0x00002818,
			.frame_header_addr         = 0x00002820,
			.frame_header_incr         = 0x00002824,
			.frame_header_cfg          = 0x00002828,
			.irq_subsample_period      = 0x00002830,
			.irq_subsample_pattern     = 0x00002834,
			.framedrop_period          = 0x00002838,
			.framedrop_pattern         = 0x0000283C,
			.mmu_prefetch_cfg          = 0x00002860,
			.mmu_prefetch_max_offset   = 0x00002864,
			.system_cache_cfg          = 0x00002868,
			.ctxt_cfg                  = 0x00002878,
			.addr_status_0             = 0x00002890,
			.addr_status_1             = 0x00002894,
			.addr_status_2             = 0x00002898,
			.addr_status_3             = 0x0000289C,
			.debug_status_cfg          = 0x0000287C,
			.debug_status_0            = 0x00002880,
			.debug_status_1            = 0x00002884,
			.debug_status_ctxt         = 0x00002888,
			.bw_limiter_addr           = 0x0000281C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 26 STATS_BHIST_TMC */
		{
			.cfg                       = 0x00002900,
			.image_addr                = 0x00002904,
			.frame_incr                = 0x00002908,
			.image_cfg_0               = 0x0000290C,
			.image_cfg_1               = 0x00002910,
			.image_cfg_2               = 0x00002914,
			.packer_cfg                = 0x00002918,
			.frame_header_addr         = 0x00002920,
			.frame_header_incr         = 0x00002924,
			.frame_header_cfg          = 0x00002928,
			.irq_subsample_period      = 0x00002930,
			.irq_subsample_pattern     = 0x00002934,
			.framedrop_period          = 0x00002938,
			.framedrop_pattern         = 0x0000293C,
			.mmu_prefetch_cfg          = 0x00002960,
			.mmu_prefetch_max_offset   = 0x00002964,
			.system_cache_cfg          = 0x00002968,
			.ctxt_cfg                  = 0x00002978,
			.addr_status_0             = 0x00002990,
			.addr_status_1             = 0x00002994,
			.addr_status_2             = 0x00002998,
			.addr_status_3             = 0x0000299C,
			.debug_status_cfg          = 0x0000297C,
			.debug_status_0            = 0x00002980,
			.debug_status_1            = 0x00002984,
			.debug_status_ctxt         = 0x00002988,
			.bw_limiter_addr           = 0x0000291C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 27 STATS_BLTM */
		{
			.cfg                       = 0x00002A00,
			.image_addr                = 0x00002A04,
			.frame_incr                = 0x00002A08,
			.image_cfg_0               = 0x00002A0C,
			.image_cfg_1               = 0x00002A10,
			.image_cfg_2               = 0x00002A14,
			.packer_cfg                = 0x00002A18,
			.frame_header_addr         = 0x00002A20,
			.frame_header_incr         = 0x00002A24,
			.frame_header_cfg          = 0x00002A28,
			.irq_subsample_period      = 0x00002A30,
			.irq_subsample_pattern     = 0x00002A34,
			.framedrop_period          = 0x00002A38,
			.framedrop_pattern         = 0x00002A3C,
			.mmu_prefetch_cfg          = 0x00002A60,
			.mmu_prefetch_max_offset   = 0x00002A64,
			.system_cache_cfg          = 0x00002A68,
			.ctxt_cfg                  = 0x00002A78,
			.addr_status_0             = 0x00002A90,
			.addr_status_1             = 0x00002A94,
			.addr_status_2             = 0x00002A98,
			.addr_status_3             = 0x00002A9C,
			.debug_status_cfg          = 0x00002A7C,
			.debug_status_0            = 0x00002A80,
			.debug_status_1            = 0x00002A84,
			.debug_status_ctxt         = 0x00002A88,
			.bw_limiter_addr           = 0x00002A1C,
			/* STATS comp-done grp (CSID BUF_DONE_IRQ_STATUS SUBGRP1 bits 7:4) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_1,
			.supported_formats        = 0xFFFFFFFFFFFFFFFF,
		},
		/* BUS Client 28 RDI_0 */
		{
			.cfg                       = 0x00002B00,
			.image_addr                = 0x00002B04,
			.frame_incr                = 0x00002B08,
			.image_cfg_0               = 0x00002B0C,
			.image_cfg_1               = 0x00002B10,
			.image_cfg_2               = 0x00002B14,
			.packer_cfg                = 0x00002B18,
			.frame_header_addr         = 0x00002B20,
			.frame_header_incr         = 0x00002B24,
			.frame_header_cfg          = 0x00002B28,
			.line_done_cfg             = 0x00002B2C,
			.irq_subsample_period      = 0x00002B30,
			.irq_subsample_pattern     = 0x00002B34,
			.framedrop_period          = 0x00002B38,
			.framedrop_pattern         = 0x00002B3C,
			.mmu_prefetch_cfg          = 0x00002B60,
			.mmu_prefetch_max_offset   = 0x00002B64,
			.system_cache_cfg          = 0x00002B68,
			.addr_status_0             = 0x00002B90,
			.addr_status_1             = 0x00002B94,
			.addr_status_2             = 0x00002B98,
			.addr_status_3             = 0x00002B9C,
			.debug_status_cfg          = 0x00002B7C,
			.debug_status_0            = 0x00002B80,
			.debug_status_1            = 0x00002B84,
			.debug_status_ctxt         = 0x00002B88,
			.bw_limiter_addr           = 0x00002B1C,
			/* RDI0 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI0 bit 16) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_2,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_12) | BIT_ULL(CAM_FORMAT_MIPI_RAW_14) |
				BIT_ULL(CAM_FORMAT_PLAIN128) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16) | BIT_ULL(CAM_FORMAT_PLAIN64),
		},
		/* BUS Client 29 RDI_1 */
		{
			.cfg                       = 0x00002C00,
			.image_addr                = 0x00002C04,
			.frame_incr                = 0x00002C08,
			.image_cfg_0               = 0x00002C0C,
			.image_cfg_1               = 0x00002C10,
			.image_cfg_2               = 0x00002C14,
			.packer_cfg                = 0x00002C18,
			.frame_header_addr         = 0x00002C20,
			.frame_header_incr         = 0x00002C24,
			.frame_header_cfg          = 0x00002C28,
			.line_done_cfg             = 0x00002C2C,
			.irq_subsample_period      = 0x00002C30,
			.irq_subsample_pattern     = 0x00002C34,
			.framedrop_period          = 0x00002C38,
			.framedrop_pattern         = 0x00002C3C,
			.mmu_prefetch_cfg          = 0x00002C60,
			.mmu_prefetch_max_offset   = 0x00002C64,
			.system_cache_cfg          = 0x00002C68,
			.addr_status_0             = 0x00002C90,
			.addr_status_1             = 0x00002C94,
			.addr_status_2             = 0x00002C98,
			.addr_status_3             = 0x00002C9C,
			.debug_status_cfg          = 0x00002C7C,
			.debug_status_0            = 0x00002C80,
			.debug_status_1            = 0x00002C84,
			.debug_status_ctxt         = 0x00002C88,
			.bw_limiter_addr           = 0x00002C1C,
			/* RDI1 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI1 bit 17) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_3,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_12) | BIT_ULL(CAM_FORMAT_MIPI_RAW_14) |
				BIT_ULL(CAM_FORMAT_PLAIN128) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16) | BIT_ULL(CAM_FORMAT_PLAIN64),
		},
		/* BUS Client 30 RDI_2 */
		{
			.cfg                       = 0x00002D00,
			.image_addr                = 0x00002D04,
			.frame_incr                = 0x00002D08,
			.image_cfg_0               = 0x00002D0C,
			.image_cfg_1               = 0x00002D10,
			.image_cfg_2               = 0x00002D14,
			.packer_cfg                = 0x00002D18,
			.frame_header_addr         = 0x00002D20,
			.frame_header_incr         = 0x00002D24,
			.frame_header_cfg          = 0x00002D28,
			.line_done_cfg             = 0x00002D2C,
			.irq_subsample_period      = 0x00002D30,
			.irq_subsample_pattern     = 0x00002D34,
			.framedrop_period          = 0x00002D38,
			.framedrop_pattern         = 0x00002D3C,
			.mmu_prefetch_cfg          = 0x00002D60,
			.mmu_prefetch_max_offset   = 0x00002D64,
			.system_cache_cfg          = 0x00002D68,
			.addr_status_0             = 0x00002D90,
			.addr_status_1             = 0x00002D94,
			.addr_status_2             = 0x00002D98,
			.addr_status_3             = 0x00002D9C,
			.debug_status_cfg          = 0x00002D7C,
			.debug_status_0            = 0x00002D80,
			.debug_status_1            = 0x00002D84,
			.debug_status_ctxt         = 0x00002D88,
			.bw_limiter_addr           = 0x00002D1C,
			/* RDI2 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI2 bit 18) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_4,
			.supported_formats        = BIT_ULL(CAM_FORMAT_MIPI_RAW_10) |
				BIT_ULL(CAM_FORMAT_MIPI_RAW_12) | BIT_ULL(CAM_FORMAT_MIPI_RAW_14) |
				BIT_ULL(CAM_FORMAT_PLAIN128) | BIT_ULL(CAM_FORMAT_PLAIN8) |
				BIT_ULL(CAM_FORMAT_PLAIN16_8) | BIT_ULL(CAM_FORMAT_PLAIN16_10) |
				BIT_ULL(CAM_FORMAT_PLAIN16_12) | BIT_ULL(CAM_FORMAT_PLAIN16_14) |
				BIT_ULL(CAM_FORMAT_PLAIN16_16) | BIT_ULL(CAM_FORMAT_PLAIN64),
		},
		/* BUS Client 31 RDI_3 */
		{
			.cfg                       = 0x00002E00,
			.image_addr                = 0x00002E04,
			.frame_incr                = 0x00002E08,
			.image_cfg_0               = 0x00002E0C,
			.image_cfg_1               = 0x00002E10,
			.image_cfg_2               = 0x00002E14,
			.packer_cfg                = 0x00002E18,
			.frame_header_addr         = 0x00002E20,
			.frame_header_incr         = 0x00002E24,
			.frame_header_cfg          = 0x00002E28,
			.line_done_cfg             = 0x00002E2C,
			.irq_subsample_period      = 0x00002E30,
			.irq_subsample_pattern     = 0x00002E34,
			.framedrop_period          = 0x00002E38,
			.framedrop_pattern         = 0x00002E3C,
			.mmu_prefetch_cfg          = 0x00002E60,
			.mmu_prefetch_max_offset   = 0x00002E64,
			.system_cache_cfg          = 0x00002E68,
			.addr_status_0             = 0x00002E90,
			.addr_status_1             = 0x00002E94,
			.addr_status_2             = 0x00002E98,
			.addr_status_3             = 0x00002E9C,
			.debug_status_cfg          = 0x00002E7C,
			.debug_status_0            = 0x00002E80,
			.debug_status_1            = 0x00002E84,
			.debug_status_ctxt         = 0x00002E88,
			.bw_limiter_addr           = 0x00002E1C,
			/* RDI3 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI3 bit 19) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_5,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 32 RDI_4 */
		{
			.cfg                       = 0x00002F00,
			.image_addr                = 0x00002F04,
			.frame_incr                = 0x00002F08,
			.image_cfg_0               = 0x00002F0C,
			.image_cfg_1               = 0x00002F10,
			.image_cfg_2               = 0x00002F14,
			.packer_cfg                = 0x00002F18,
			.frame_header_addr         = 0x00002F20,
			.frame_header_incr         = 0x00002F24,
			.frame_header_cfg          = 0x00002F28,
			.line_done_cfg             = 0x00002F2C,
			.irq_subsample_period      = 0x00002F30,
			.irq_subsample_pattern     = 0x00002F34,
			.framedrop_period          = 0x00002F38,
			.framedrop_pattern         = 0x00002F3C,
			.mmu_prefetch_cfg          = 0x00002F60,
			.mmu_prefetch_max_offset   = 0x00002F64,
			.system_cache_cfg          = 0x00002F68,
			.addr_status_0             = 0x00002F90,
			.addr_status_1             = 0x00002F94,
			.addr_status_2             = 0x00002F98,
			.addr_status_3             = 0x00002F9C,
			.debug_status_cfg          = 0x00002F7C,
			.debug_status_0            = 0x00002F80,
			.debug_status_1            = 0x00002F84,
			.debug_status_ctxt         = 0x00002F88,
			.bw_limiter_addr           = 0x00002F1C,
			/* RDI4 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI4 bit 20) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_6,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 33 RDI_5 */
		{
			.cfg                       = 0x00003000,
			.image_addr                = 0x00003004,
			.frame_incr                = 0x00003008,
			.image_cfg_0               = 0x0000300C,
			.image_cfg_1               = 0x00003010,
			.image_cfg_2               = 0x00003014,
			.packer_cfg                = 0x00003018,
			.frame_header_addr         = 0x00003020,
			.frame_header_incr         = 0x00003024,
			.frame_header_cfg          = 0x00003028,
			.line_done_cfg             = 0x0000302C,
			.irq_subsample_period      = 0x00003030,
			.irq_subsample_pattern     = 0x00003034,
			.framedrop_period          = 0x00003038,
			.framedrop_pattern         = 0x0000303C,
			.mmu_prefetch_cfg          = 0x00003060,
			.mmu_prefetch_max_offset   = 0x00003064,
			.system_cache_cfg          = 0x00003068,
			.addr_status_0             = 0x00003090,
			.addr_status_1             = 0x00003094,
			.addr_status_2             = 0x00003098,
			.addr_status_3             = 0x0000309C,
			.debug_status_cfg          = 0x0000307C,
			.debug_status_0            = 0x00003080,
			.debug_status_1            = 0x00003084,
			.debug_status_ctxt         = 0x00003088,
			.bw_limiter_addr           = 0x0000301C,
			/* RDI5 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI5 bit 21) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_7,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 34 RDI_6 */
		{
			.cfg                       = 0x00003100,
			.image_addr                = 0x00003104,
			.frame_incr                = 0x00003108,
			.image_cfg_0               = 0x0000310C,
			.image_cfg_1               = 0x00003110,
			.image_cfg_2               = 0x00003114,
			.packer_cfg                = 0x00003118,
			.frame_header_addr         = 0x00003120,
			.frame_header_incr         = 0x00003124,
			.frame_header_cfg          = 0x00003128,
			.line_done_cfg             = 0x0000312C,
			.irq_subsample_period      = 0x00003130,
			.irq_subsample_pattern     = 0x00003134,
			.framedrop_period          = 0x00003138,
			.framedrop_pattern         = 0x0000313C,
			.mmu_prefetch_cfg          = 0x00003160,
			.mmu_prefetch_max_offset   = 0x00003164,
			.system_cache_cfg          = 0x00003168,
			.addr_status_0             = 0x00003190,
			.addr_status_1             = 0x00003194,
			.addr_status_2             = 0x00003198,
			.addr_status_3             = 0x0000319C,
			.debug_status_cfg          = 0x0000317C,
			.debug_status_0            = 0x00003180,
			.debug_status_1            = 0x00003184,
			.debug_status_ctxt         = 0x00003188,
			.bw_limiter_addr           = 0x0000311C,
			/* RDI6 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI6 bit 22) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_8,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 35 RDI_7 */
		{
			.cfg                       = 0x00003200,
			.image_addr                = 0x00003204,
			.frame_incr                = 0x00003208,
			.image_cfg_0               = 0x0000320C,
			.image_cfg_1               = 0x00003210,
			.image_cfg_2               = 0x00003214,
			.packer_cfg                = 0x00003218,
			.frame_header_addr         = 0x00003220,
			.frame_header_incr         = 0x00003224,
			.frame_header_cfg          = 0x00003228,
			.line_done_cfg             = 0x0000322C,
			.irq_subsample_period      = 0x00003230,
			.irq_subsample_pattern     = 0x00003234,
			.framedrop_period          = 0x00003238,
			.framedrop_pattern         = 0x0000323C,
			.mmu_prefetch_cfg          = 0x00003260,
			.mmu_prefetch_max_offset   = 0x00003264,
			.system_cache_cfg          = 0x00003268,
			.addr_status_0             = 0x00003290,
			.addr_status_1             = 0x00003294,
			.addr_status_2             = 0x00003298,
			.addr_status_3             = 0x0000329C,
			.debug_status_cfg          = 0x0000327C,
			.debug_status_0            = 0x00003280,
			.debug_status_1            = 0x00003284,
			.debug_status_ctxt         = 0x00003288,
			.bw_limiter_addr           = 0x0000321C,
			/* RDI7 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI7 bit 23) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_9,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 36 RDI_8 */
		{
			.cfg                       = 0x00003300,
			.image_addr                = 0x00003304,
			.frame_incr                = 0x00003308,
			.image_cfg_0               = 0x0000330C,
			.image_cfg_1               = 0x00003310,
			.image_cfg_2               = 0x00003314,
			.packer_cfg                = 0x00003318,
			.frame_header_addr         = 0x00003320,
			.frame_header_incr         = 0x00003324,
			.frame_header_cfg          = 0x00003328,
			.line_done_cfg             = 0x0000332C,
			.irq_subsample_period      = 0x00003330,
			.irq_subsample_pattern     = 0x00003334,
			.framedrop_period          = 0x00003338,
			.framedrop_pattern         = 0x0000333C,
			.mmu_prefetch_cfg          = 0x00003360,
			.mmu_prefetch_max_offset   = 0x00003364,
			.system_cache_cfg          = 0x00003368,
			.addr_status_0             = 0x00003390,
			.addr_status_1             = 0x00003394,
			.addr_status_2             = 0x00003398,
			.addr_status_3             = 0x0000339C,
			.debug_status_cfg          = 0x0000337C,
			.debug_status_0            = 0x00003380,
			.debug_status_1            = 0x00003384,
			.debug_status_ctxt         = 0x00003388,
			.bw_limiter_addr           = 0x0000331C,
			/* RDI8 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI8 bit 24) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_10,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 37 RDI_9 */
		{
			.cfg                       = 0x00003400,
			.image_addr                = 0x00003404,
			.frame_incr                = 0x00003408,
			.image_cfg_0               = 0x0000340C,
			.image_cfg_1               = 0x00003410,
			.image_cfg_2               = 0x00003414,
			.packer_cfg                = 0x00003418,
			.frame_header_addr         = 0x00003420,
			.frame_header_incr         = 0x00003424,
			.frame_header_cfg          = 0x00003428,
			.line_done_cfg             = 0x0000342C,
			.irq_subsample_period      = 0x00003430,
			.irq_subsample_pattern     = 0x00003434,
			.framedrop_period          = 0x00003438,
			.framedrop_pattern         = 0x0000343C,
			.mmu_prefetch_cfg          = 0x00003460,
			.mmu_prefetch_max_offset   = 0x00003464,
			.system_cache_cfg          = 0x00003468,
			.addr_status_0             = 0x00003490,
			.addr_status_1             = 0x00003494,
			.addr_status_2             = 0x00003498,
			.addr_status_3             = 0x0000349C,
			.debug_status_cfg          = 0x0000347C,
			.debug_status_0            = 0x00003480,
			.debug_status_1            = 0x00003484,
			.debug_status_ctxt         = 0x00003488,
			.bw_limiter_addr           = 0x0000341C,
			/* RDI9 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI9 bit 25) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_11,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 38 RDI_10 */
		{
			.cfg                       = 0x00003500,
			.image_addr                = 0x00003504,
			.frame_incr                = 0x00003508,
			.image_cfg_0               = 0x0000350C,
			.image_cfg_1               = 0x00003510,
			.image_cfg_2               = 0x00003514,
			.packer_cfg                = 0x00003518,
			.frame_header_addr         = 0x00003520,
			.frame_header_incr         = 0x00003524,
			.frame_header_cfg          = 0x00003528,
			.line_done_cfg             = 0x0000352C,
			.irq_subsample_period      = 0x00003530,
			.irq_subsample_pattern     = 0x00003534,
			.framedrop_period          = 0x00003538,
			.framedrop_pattern         = 0x0000353C,
			.mmu_prefetch_cfg          = 0x00003560,
			.mmu_prefetch_max_offset   = 0x00003564,
			.system_cache_cfg          = 0x00003568,
			.addr_status_0             = 0x00003590,
			.addr_status_1             = 0x00003594,
			.addr_status_2             = 0x00003598,
			.addr_status_3             = 0x0000359C,
			.debug_status_cfg          = 0x0000357C,
			.debug_status_0            = 0x00003580,
			.debug_status_1            = 0x00003584,
			.debug_status_ctxt         = 0x00003588,
			.bw_limiter_addr           = 0x0000351C,
			/* RDI10 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI10 bit 26) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_12,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
		/* BUS Client 39 RDI_11 */
		{
			.cfg                       = 0x00003600,
			.image_addr                = 0x00003604,
			.frame_incr                = 0x00003608,
			.image_cfg_0               = 0x0000360C,
			.image_cfg_1               = 0x00003610,
			.image_cfg_2               = 0x00003614,
			.packer_cfg                = 0x00003618,
			.frame_header_addr         = 0x00003620,
			.frame_header_incr         = 0x00003624,
			.frame_header_cfg          = 0x00003628,
			.line_done_cfg             = 0x0000362C,
			.irq_subsample_period      = 0x00003630,
			.irq_subsample_pattern     = 0x00003634,
			.framedrop_period          = 0x00003638,
			.framedrop_pattern         = 0x0000363C,
			.mmu_prefetch_cfg          = 0x00003660,
			.mmu_prefetch_max_offset   = 0x00003664,
			.system_cache_cfg          = 0x00003668,
			.addr_status_0             = 0x00003690,
			.addr_status_1             = 0x00003694,
			.addr_status_2             = 0x00003698,
			.addr_status_3             = 0x0000369C,
			.debug_status_cfg          = 0x0000367C,
			.debug_status_0            = 0x00003680,
			.debug_status_1            = 0x00003684,
			.debug_status_ctxt         = 0x00003688,
			.bw_limiter_addr           = 0x0000361C,
			/* RDI11 comp-done grp (CSID BUF_DONE_IRQ_STATUS INFO_RDI11 bit 27) */
			.comp_group               = CAM_VFE_BUS_VER3_COMP_GRP_13,
			.supported_formats        = BIT_ULL(CAM_FORMAT_PLAIN128),
		},
	},
	.num_out = 31,
	.vfe_out_hw_info = {
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_FULL,
			.max_width     = 9000,
			.max_height    = 16384,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[0],
			.num_mid       = 32,
			/* 4th (UV) plane not driven yet - UAPI caps planes at 3; phase 2 */
			.num_wm        = 2,
			.mc_based      = true,
			.wm_idx        = {
				0,
				3,
				1,
				2,
			},
			.name          = {
				"FULL_Y",
				"FULL_UV",
				"FULL_C1",
				"FULL_C2",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RAW_DUMP,
			.max_width     = 9000,
			.max_height    = 16384,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[1],
			.num_mid       = 8,
			.num_wm        = 1,
			.wm_idx        = {
				4,
			},
			.name          = {
				"PIXEL_RAW",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_IR,
			.max_width     = 9000,
			.max_height    = 16384,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[2],
			.num_mid       = 8,
			.num_wm        = 1,
			.cntxt_cfg_except = true,
			.wm_idx        = {
				5,
			},
			.name          = {
				"IR_OUT",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_DS16,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[3],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.wm_idx        = {
				14,
			},
			.name          = {
				"HV_DS_16",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_DS4,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[4],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.wm_idx        = {
				15,
			},
			.name          = {
				"HV_DS_4",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_AI_OUT_1,
			.max_width     = 4500,
			.max_height    = 16384,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[5],
			.num_mid       = 32,
			/* 4th (UV) plane not driven yet - UAPI caps planes at 3; phase 2 */
			.num_wm        = 2,
			.mc_based      = true,
			.wm_idx        = {
				6,
				9,
				7,
				8,
			},
			.name          = {
				"AI_OUT_1_Y",
				"AI_OUT_1_UV",
				"AI_OUT_1_C1",
				"AI_OUT_1_C2",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_AI_OUT_2,
			.max_width     = 2250,
			.max_height    = 16384,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[6],
			.num_mid       = 32,
			/* 4th (UV) plane not driven yet - UAPI caps planes at 3; phase 2 */
			.num_wm        = 2,
			.mc_based      = true,
			.wm_idx        = {
				10,
				13,
				11,
				12,
			},
			.name          = {
				"AI_OUT_2_Y",
				"AI_OUT_2_UV",
				"AI_OUT_2_C1",
				"AI_OUT_2_C2",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_IR_BG,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[7],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				16,
			},
			.name          = {
				"STATS_IR_BG",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_IR_BHIST,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[8],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				17,
			},
			.name          = {
				"STATS_IR_BHIST",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_TL_BG,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[9],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				18,
			},
			.name          = {
				"STATS_TL_BG",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_BG_IRC_AEC,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[10],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				19,
			},
			.name          = {
				"STATS_BG_IRC_AEC",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_BHIST_IRC_AEC,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[11],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				20,
			},
			.name          = {
				"STATS_BHIST_IRC_AEC",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_AEC_BG,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[12],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				21,
			},
			.name          = {
				"STATS_AEC_BG",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_AEC_BHIST,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[13],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				22,
			},
			.name          = {
				"STATS_AEC_BHIST",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_BAYER_RS,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[14],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				23,
			},
			.name          = {
				"STATS_BAYER_RS",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_AWB_BFW,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[15],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				24,
			},
			.name          = {
				"AWB_BFW",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_AWB_BG,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[16],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				25,
			},
			.name          = {
				"STATS_AWB_BG",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_STATS_TMC_BHIST,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[17],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				26,
			},
			.name          = {
				"STATS_TMC_BHIST",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_LTM_STATS,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_0,
			.mid           = vfe900_out_port_mid[18],
			.num_mid       = 4,
			.num_wm        = 1,
			.mc_based      = true,
			.mc_grp_shift  = 4,
			.wm_idx        = {
				27,
			},
			.name          = {
				"LTM",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI0,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_1,
			.mid           = vfe900_out_port_mid[19],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				28,
			},
			.name          = {
				"RDI_0",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI1,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_2,
			.mid           = vfe900_out_port_mid[20],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				29,
			},
			.name          = {
				"RDI_1",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI2,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_3,
			.mid           = vfe900_out_port_mid[21],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				30,
			},
			.name          = {
				"RDI_2",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI3,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_4,
			.mid           = vfe900_out_port_mid[22],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				31,
			},
			.name          = {
				"RDI_3",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI4,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_5,
			.mid           = vfe900_out_port_mid[23],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				32,
			},
			.name          = {
				"RDI_4",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI5,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_6,
			.mid           = vfe900_out_port_mid[24],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				33,
			},
			.name          = {
				"RDI_5",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI6,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_7,
			.mid           = vfe900_out_port_mid[25],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				34,
			},
			.name          = {
				"RDI_6",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI7,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_8,
			.mid           = vfe900_out_port_mid[26],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				35,
			},
			.name          = {
				"RDI_7",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI8,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_9,
			.mid           = vfe900_out_port_mid[27],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				36,
			},
			.name          = {
				"RDI_8",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI9,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_10,
			.mid           = vfe900_out_port_mid[28],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				37,
			},
			.name          = {
				"RDI_9",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI10,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_11,
			.mid           = vfe900_out_port_mid[29],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				38,
			},
			.name          = {
				"RDI_10",
			},
		},
		{
			.vfe_out_type  = CAM_VFE_BUS_VER3_VFE_OUT_RDI11,
			.max_width     = -1,
			.max_height    = -1,
			.source_group  = CAM_VFE_BUS_VER3_SRC_GRP_12,
			.mid           = vfe900_out_port_mid[30],
			.num_mid       = 1,
			.num_wm        = 1,
			.line_based    = 1,
			.wm_idx        = {
				39,
			},
			.name          = {
				"RDI_11",
			},
		},
	},
	.num_cons_err = 29,
	.constraint_error_list = {
		{
			.bitmask = BIT(0),
			.error_description = "PPC 1x1 input not supported"
		},
		{
			.bitmask = BIT(1),
			.error_description = "PPC 1x2 input not supported"
		},
		{
			.bitmask = BIT(2),
			.error_description = "PPC 2x1 input not supported"
		},
		{
			.bitmask = BIT(3),
			.error_description = "PPC 2x2 input not supported"
		},
		{
			.bitmask = BIT(4),
			.error_description = "Pack 8 BPP format not supported"
		},
		{
			.bitmask = BIT(5),
			.error_description = "Pack 16 format not supported"
		},
		{
			.bitmask = BIT(6),
			.error_description = "Pack 32 BPP format not supported"
		},
		{
			.bitmask = BIT(7),
			.error_description = "Pack 64 BPP format not supported"
		},
		{
			.bitmask = BIT(8),
			.error_description = "Pack MIPI 20 format not supported"
		},
		{
			.bitmask = BIT(9),
			.error_description = "Pack MIPI 14 format not supported"
		},
		{
			.bitmask = BIT(10),
			.error_description = "Pack MIPI 12 format not supported"
		},
		{
			.bitmask = BIT(11),
			.error_description = "Pack MIPI 10 format not supported"
		},
		{
			.bitmask = BIT(12),
			.error_description = "Pack 128 BPP format not supported"
		},
		{
			.bitmask = BIT(13),
			.error_description = "UBWC NV12 format not supported"
		},
		{
			.bitmask = BIT(14),
			.error_description = "UBWC NV12 4R format not supported"
		},
		{
			.bitmask = BIT(15),
			.error_description = "UBWC TP10 format not supported"
		},
		{
			.bitmask = BIT(16),
			.error_description = "Frame based Mode not supported"
		},
		{
			.bitmask = BIT(17),
			.error_description = "Index based Mode not supported"
		},
		{
			.bitmask = BIT(18),
			.error_description = "FIFO image addr unalign"
		},
		{
			.bitmask = BIT(19),
			.error_description = "FIFO ubwc addr unalign"
		},
		{
			.bitmask = BIT(20),
			.error_description = "FIFO frmheader addr unalign"
		},
		{
			.bitmask = BIT(21),
			.error_description = "Image address unalign"
		},
		{
			.bitmask = BIT(22),
			.error_description = "UBWC address unalign"
		},
		{
			.bitmask = BIT(23),
			.error_description = "Frame Header address unalign"
		},
		{
			.bitmask = BIT(24),
			.error_description = "Stride unalign"
		},
		{
			.bitmask = BIT(25),
			.error_description = "X Initialization unalign"
		},
		{
			.bitmask = BIT(26),
			.error_description = "Image Width unalign"
		},
		{
			.bitmask = BIT(27),
			.error_description = "Image Height unalign"
		},
		{
			.bitmask = BIT(28),
			.error_description = "Meta Stride unalign"
		},
	},
	.num_bus_errors_0      = ARRAY_SIZE(vfe900_bus_irq_err_desc_0),
	.num_bus_errors_1      = ARRAY_SIZE(vfe900_bus_irq_err_desc_1),
	.bus_err_desc_0        = vfe900_bus_irq_err_desc_0,
	.bus_err_desc_1        = vfe900_bus_irq_err_desc_1,
	.num_comp_grp          = 14,
	.support_consumed_addr = true,
	.comp_done_mask = {
		0xF, 0xF0,
		BIT(16), BIT(17), BIT(18), BIT(19), BIT(20), BIT(21),
		BIT(22), BIT(23), BIT(24), BIT(25), BIT(26), BIT(27),
	},
	.top_irq_shift         = 0,
	.max_out_res           = CAM_ISP_IFE_OUT_RES_BASE + 62,
	.pack_align_shift      = 5,
	.max_bw_counter_limit  = 0xFF,
};

static struct cam_vfe_irq_hw_info vfe900_irq_hw_info = {
	.reset_mask    = 0,
	.supported_irq = CAM_VFE_HW_IRQ_CAP_EXT_CSID,
	.top_irq_reg   = &vfe900_top_irq_reg_info,
};

static struct cam_vfe_hw_info cam_vfe900_hw_info = {
	.irq_hw_info                  = &vfe900_irq_hw_info,

	.bus_version                   = CAM_VFE_BUS_VER_3_0,
	.bus_hw_info                   = &vfe900_bus_hw_info,

	.top_version                   = CAM_VFE_TOP_VER_4_0,
	.top_hw_info                   = &vfe900_top_hw_info,
};

#endif /* _CAM_VFE900_H_ */
