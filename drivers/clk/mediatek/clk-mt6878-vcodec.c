// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2023 MediaTek Inc.
 * Author: Chuan-wen Chen <chuan-wen.chen@mediatek.com>
 */

#include <linux/clk-provider.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

#include "clk-mtk.h"
#include "clk-gate.h"

#include <dt-bindings/clock/mt6878-clk.h>

#define MT_CCF_BRINGUP		1

/* Regular Number Definition */
#define INV_OFS			-1
#define INV_BIT			-1

static const struct mtk_gate_regs vde20_cg_regs = {
	.set_ofs = 0x0,
	.clr_ofs = 0x4,
	.sta_ofs = 0x0,
};

static const struct mtk_gate_regs vde21_cg_regs = {
	.set_ofs = 0x8,
	.clr_ofs = 0xC,
	.sta_ofs = 0x8,
};

#define GATE_VDE20(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &vde20_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr_inv,	\
	}

#define GATE_VDE21(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &vde21_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr_inv,	\
	}

static const struct mtk_gate vde2_clks[] = {
	/* VDE20 */
	GATE_VDE20(CLK_VDE2_VDEC_CKEN, "vde2_vdec_cken",
			"top_vdec_ck"/* parent */, 0),
	GATE_VDE20(CLK_VDE2_VDEC_ACTIVE, "vde2_vdec_active",
			"top_vdec_ck"/* parent */, 4),
	/* VDE21 */
	GATE_VDE21(CLK_VDE2_LARB_CKEN, "vde2_larb_cken",
			"top_vdec_ck"/* parent */, 0),
};

static const struct mtk_clk_desc vde2_mcd = {
	.clks = vde2_clks,
	.num_clks = CLK_VDE2_NR_CLK,
};

static const struct mtk_gate_regs ven1_cg_regs = {
	.set_ofs = 0x4,
	.clr_ofs = 0x8,
	.sta_ofs = 0x0,
};

#define GATE_VEN1(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ven1_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr_inv,	\
	}

static const struct mtk_gate ven1_clks[] = {
	GATE_VEN1(CLK_VEN1_CKE0_LARB, "ven1_larb",
			"top_venc_ck"/* parent */, 0),
	GATE_VEN1(CLK_VEN1_CKE1_VENC, "ven1_venc",
			"top_venc_ck"/* parent */, 4),
	GATE_VEN1(CLK_VEN1_CKE2_JPGENC, "ven1_jpgenc",
			"top_venc_ck"/* parent */, 8),
	GATE_VEN1(CLK_VEN1_CKE5_GALS, "ven1_gals",
			"top_venc_ck"/* parent */, 28),
};

static const struct mtk_clk_desc ven1_mcd = {
	.clks = ven1_clks,
	.num_clks = CLK_VEN1_NR_CLK,
};

static const struct of_device_id of_match_clk_mt6878_vcodec[] = {
	{
		.compatible = "mediatek,mt6878-vdecsys",
		.data = &vde2_mcd,
	}, {
		.compatible = "mediatek,mt6878-vencsys",
		.data = &ven1_mcd,
	}, {
		/* sentinel */
	}
};


static int clk_mt6878_vcodec_grp_probe(struct platform_device *pdev)
{
	int r;

#if MT_CCF_BRINGUP
	pr_notice("%s: %s init begin\n", __func__, pdev->name);
#endif

	r = mtk_clk_simple_probe(pdev);
	if (r)
		dev_err(&pdev->dev,
			"could not register clock provider: %s: %d\n",
			pdev->name, r);

#if MT_CCF_BRINGUP
	pr_notice("%s: %s init end\n", __func__, pdev->name);
#endif

	return r;
}

static struct platform_driver clk_mt6878_vcodec_drv = {
	.probe = clk_mt6878_vcodec_grp_probe,
	.driver = {
		.name = "clk-mt6878-vcodec",
		.of_match_table = of_match_clk_mt6878_vcodec,
	},
};

module_platform_driver(clk_mt6878_vcodec_drv);
MODULE_LICENSE("GPL");
