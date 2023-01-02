// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#include <dt-bindings/gce/mt6985-gce.h>

#include "cmdq-util.h"

#define GCE_D_PA	0x1e980000
#define GCE_M_PA	0x1e990000

const char *cmdq_thread_module_dispatch(phys_addr_t gce_pa, s32 thread)
{
	if (gce_pa == GCE_D_PA) {
		switch (thread) {
		case 0 ... 9:
			return "DISP";
		case 16 ... 19:
			return "MML";
		case 20 ... 21:
			return "MDP";
		default:
			return "CMDQ";
		}
	} else if (gce_pa == GCE_M_PA) {
		switch (thread) {
		case 0 ... 5:
		case 10 ... 11:
		case 16 ... 22:
			return "ISP";
		case 6 ... 7:
			return "VFMT";
		case 12:
		case 23:
			return "VENC";
		default:
			return "CMDQ";
		}
	}

	return "CMDQ";
}

const char *cmdq_event_module_dispatch(phys_addr_t gce_pa, const u16 event,
	s32 thread)
{
	switch (event) {
	case CMDQ_SYNC_TOKEN_CONFIG_DIRTY:
	case CMDQ_SYNC_TOKEN_STREAM_EOF:
	case CMDQ_SYNC_TOKEN_ESD_EOF:
	case CMDQ_SYNC_TOKEN_STREAM_BLOCK:
	case CMDQ_SYNC_TOKEN_CABC_EOF:
		return "DISP";
	case CMDQ_SYNC_TOKEN_IMGSYS_POOL_1
		... CMDQ_SYNC_TOKEN_IMGSYS_POOL_100:
	case CMDQ_SYNC_TOKEN_IMGSYS_WPE_EIS
		... CMDQ_SYNC_TOKEN_IPESYS_ME:
	case CMDQ_SYNC_TOKEN_IMGSYS_POOL_101
		... CMDQ_SYNC_TOKEN_IMGSYS_POOL_140:
		return "IMGSYS";

	case CMDQ_EVENT_GPR_TIMER ... CMDQ_EVENT_GPR_TIMER + 32:
		return cmdq_thread_module_dispatch(gce_pa, thread);
	}

	if (gce_pa == GCE_D_PA) // GCE-D
		switch (event) {
		case CMDQ_EVENT_OVLSYS_OVLSYS_EVENT_0
			... CMDQ_EVENT_OVLSYS1_OVLSYS1_EVENT_63:
			return "DISP";
		case CMDQ_EVENT_MMLSYS1_MDP_RDMA0_SOF
			... CMDQ_EVENT_MMLSYS_BUF_UNDERRUN_ENG_EVENT_2:
			return "MDP";
		case CMDQ_EVENT_DISPSYS_DISP_EVENT_0
			... CMDQ_EVENT_DISPSYS1_DISP1_EVENT_127:
			return "DISP";
		case CMDQ_EVENT_GCE_EVENT_DSI0_TE_I
			... CMDQ_EVENT_DPTX_DPTX_EVENT_1:
			return "DISP";
		default:
			return "CMDQ";
		}

	if (gce_pa == GCE_M_PA) // GCE-M
		switch (event) {
		case CMDQ_EVENT_VDEC1_LINE_COUNT_THRESHOLD_INTERRUPT
			... CMDQ_EVENT_VDEC1_GCE_CNT_OP_THRESHOLD:
			return "VDEC";
		case CMDQ_EVENT_VENC3_VENC_FRAME_DONE
			... CMDQ_EVENT_VENC1_VPS_HEADER_DONE:
			return "VENC";
		case CMDQ_EVENT_VDEC2_LINE_COUNT_THRESHOLD_INTERRUPT
			... CMDQ_EVENT_VDEC2_GCE_CNT_OP_THRESHOLD:
			return "VDEC";
		case CMDQ_EVENT_IMG_IMG_SOF
			... CMDQ_EVENT_IMG_IMGSYS_IPE_MMG_DONE:
			return "IMG";
		case CMDQ_EVENT_CAM_CAM_SUBA_SW_PASS1_DONE
			... CMDQ_EVENT_CAM_ADL_RD_FRAME_DONE:
			return "CAM";
		default:
			return "CMDQ";
		}

	return "CMDQ";
}

u32 cmdq_util_hw_id(u32 pa)
{
	switch (pa) {
	case GCE_D_PA:
		return 0;
	case GCE_M_PA:
		return 1;
	default:
		cmdq_err("unknown addr:%x", pa);
	}

	return 0;
}

u32 cmdq_test_get_subsys_list(u32 **regs_out)
{
	static u32 regs[] = {
		0x1f003000,	/* mdp_wrot0 */
		0x14000100,	/* mmsys_config */
		0x14001000,	/* dispsys */
		0x15101200,	/* imgsys */
		0x1000106c,	/* infra */
	};

	*regs_out = regs;
	return ARRAY_SIZE(regs);
}

const char *cmdq_util_hw_name(void *chan)
{
	u32 hw_id = cmdq_util_hw_id((u32)cmdq_mbox_get_base_pa(chan));

	if (hw_id == 0)
		return "GCE-D";

	if (hw_id == 1)
		return "GCE-M";

	return "CMDQ";
}

bool cmdq_thread_ddr_module(const s32 thread)
{
	switch (thread) {
	case 0 ... 6:
	case 8 ... 9:
	case 15:
		return false;
	default:
		return true;
	}
}

bool cmdq_mbox_hw_trace_thread(void *chan)
{
	const phys_addr_t gce_pa = cmdq_mbox_get_base_pa(chan);
	const s32 idx = cmdq_mbox_chan_id(chan);

	if (gce_pa == GCE_D_PA)
		switch (idx) {
		case 16 ... 19: // MML
			cmdq_log("%s: pa:%pa idx:%d", __func__, &gce_pa, idx);
			return false;
		}

	return true;
}

struct cmdq_util_platform_fp platform_fp = {
	.thread_module_dispatch = cmdq_thread_module_dispatch,
	.event_module_dispatch = cmdq_event_module_dispatch,
	.util_hw_id = cmdq_util_hw_id,
	.test_get_subsys_list = cmdq_test_get_subsys_list,
	.util_hw_name = cmdq_util_hw_name,
	.thread_ddr_module = cmdq_thread_ddr_module,
	.hw_trace_thread = cmdq_mbox_hw_trace_thread,
};

static int __init cmdq_platform_init(void)
{
	cmdq_util_set_fp(&platform_fp);
	return 0;
}
module_init(cmdq_platform_init);

MODULE_LICENSE("GPL v2");
