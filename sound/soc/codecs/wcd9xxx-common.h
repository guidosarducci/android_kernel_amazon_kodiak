/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef WCD9XXX_CODEC_COMMON

#define WCD9XXX_CODEC_COMMON

#include "wcd9xxx-resmgr.h"

#define WCD9XXX_CLSH_REQ_ENABLE true
#define WCD9XXX_CLSH_REQ_DISABLE false

#define WCD9XXX_CLSH_EVENT_PRE_DAC 0x01
#define WCD9XXX_CLSH_EVENT_POST_PA 0x02

/* Basic states for Class H state machine.
 * represented as a bit mask within a u8 data type
 * bit 0: EAR mode
 * bit 1: HPH Left mode
 * bit 2: HPH Right mode
 * bit 3: Lineout mode
 * bit 4: Ultrasound mode
 */
#define	WCD9XXX_CLSH_STATE_IDLE 0x00
#define	WCD9XXX_CLSH_STATE_EAR (0x01 << 0)
#define	WCD9XXX_CLSH_STATE_HPHL (0x01 << 1)
#define	WCD9XXX_CLSH_STATE_HPHR (0x01 << 2)
#define	WCD9XXX_CLSH_STATE_LO (0x01 << 3)
#define NUM_CLSH_STATES ((0x01 << 4) - 1)

#ifdef CONFIG_MACH_URSA
/* URSA-3964 - DO NOT REMOVE RELATED CHANGES */
#define	WCD9XXX_CLSAB_STATE_IDLE  0x00
#define WCD9XXX_CLSAB_STATE_HPHL (0x01 << 1)
#define WCD9XXX_CLSAB_STATE_HPHR (0x01 << 2)

#define WCD9XXX_CLSAB_REQ_ENABLE  true
#define WCD9XXX_CLSAB_REQ_DISABLE false
#endif

/* Derived State: Bits 1 and 2 should be set for Headphone stereo */
#define WCD9XXX_CLSH_STATE_HPH_ST (WCD9XXX_CLSH_STATE_HPHL | \
						WCD9XXX_CLSH_STATE_HPHR)


struct wcd9xxx_reg_mask_val {
	u16	reg;
	u8	mask;
	u8	val;
};

enum ncp_fclk_level {
	NCP_FCLK_LEVEL_8,
	NCP_FCLK_LEVEL_5,
	NCP_FCLK_LEVEL_MAX,
};

/* Class H data that the codec driver will maintain */
struct wcd9xxx_clsh_cdc_data {
	u8 state;
	int buck_mv;
	int clsh_users;
	int buck_users;
	int ncp_users[NCP_FCLK_LEVEL_MAX];
	struct wcd9xxx_resmgr *resmgr;
};

struct wcd9xxx_anc_header {
	u32 reserved[3];
	u32 num_anc_slots;
};

enum wcd9xxx_buck_volt {
	WCD9XXX_CDC_BUCK_UNSUPPORTED = 0,
	WCD9XXX_CDC_BUCK_MV_1P8 = 1800000,
	WCD9XXX_CDC_BUCK_MV_2P15 = 2150000,
};

extern void wcd9xxx_clsh_fsm(struct snd_soc_codec *codec,
		struct wcd9xxx_clsh_cdc_data *cdc_clsh_d,
		u8 req_state, bool req_type, u8 clsh_event);

#ifdef CONFIG_MACH_URSA
extern void wcd9xxx_enable_high_perf_mode(struct snd_soc_codec *codec,
				struct wcd9xxx_clsh_cdc_data *clsh_d,
				u8 req_state, bool req_type);
#endif

extern void wcd9xxx_clsh_init(struct wcd9xxx_clsh_cdc_data *clsh,
			      struct wcd9xxx_resmgr *resmgr);

enum wcd9xxx_codec_event {
	WCD9XXX_CODEC_EVENT_CODEC_UP = 0,
};

#endif
