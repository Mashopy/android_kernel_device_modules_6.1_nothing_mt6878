/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#ifndef __GED_BASE_H__
#define __GED_BASE_H__

#include <linux/compiler.h>
#include "ged_type.h"

#define GED_TAG "[GPU/GED]"
#ifdef GED_DEBUG_LOG
	#define GED_LOGD(fmt, args...) \
		pr_info(GED_TAG"[DEBUG]@%s: "fmt"\n", __func__, ##args)
#else
	#define GED_LOGD(fmt, args...) do {} while (0)
#endif /* GED_DEBUG_LOG */

#define GED_LOGD_IF(cond, ...)  do { if (cond) GED_LOGDD(__VA_ARGS__); } while (0)
#define GED_LOGDD(fmt, args...) pr_info(GED_TAG"[D]@%s: "fmt"\n", __func__, ##args)
#define GED_LOGI(fmt, args...) pr_info(GED_TAG"[INFO]@%s: "fmt"\n", __func__, ##args)
#define GED_LOGE(fmt, args...) pr_info(GED_TAG"[ERROR]@%s: "fmt"\n", __func__, ##args)
#define GED_CONTAINER_OF(ptr, type, member) \
	((type *)(((char *)ptr) - offsetof(type, member)))

unsigned long ged_copy_to_user(void __user *pvTo,
	const void *pvFrom, unsigned long ulBytes);

unsigned long ged_copy_from_user(void *pvTo,
	const void __user *pvFrom, unsigned long ulBytes);

void *ged_alloc(int i32Size);

void *ged_alloc_atomic(int i32Size);

void ged_free(void *pvBuf, int i32Size);

long ged_get_pid(void);

unsigned long long ged_get_time(void);

unsigned int ged_get_segment_id(void);

struct GED_FILE_PRIVATE_BASE {
	void (*free_func)(void *f);
};

#endif
