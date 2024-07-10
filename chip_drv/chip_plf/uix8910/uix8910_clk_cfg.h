#ifndef _TGT_AP_CLOCK_CFG_H_
#define _TGT_AP_CLOCK_CFG_H_
//#include "tgt_ap_mem_config.h"

/*
 * PLL Freqs
 */
#define _TGT_AP_PLL_CPU_FREQ    (800)
#define _TGT_AP_PLL_BUS_FREQ    (800)
#define _TGT_AP_PLL_MEM_FREQ    (800)
#define _TGT_AP_PLL_USB_FREQ    (480)

/*
 * DDR clock settings
 */
#if !defined(TGT_AP_MEM_TYPE_PSRAM)
#define _TGT_AP_DDR_TYPE	(2)
#define _TGT_AP_DDR_CLOCK	(200)
#define _TGT_AP_DDR_CLOCK_1X	(_TGT_AP_DDR_CLOCK)
#define _TGT_AP_DDR_CLOCK_2X	(_TGT_AP_DDR_CLOCK_1X * 2)

/*
 * PSRAM clock settings
 */
#else
#define _TGT_AP_DDR_TYPE	(0)
#define _TGT_AP_DDR_CLOCK	(200)
#define _TGT_AP_DDR_CLOCK_1X	(_TGT_AP_DDR_CLOCK)
#define _TGT_AP_DDR_CLOCK_2X	(_TGT_AP_DDR_CLOCK_1X * 2)
#endif /* !defined(TGT_AP_MEM_TYPE_PSRAM) */

/*
 * DDR parameters
 */
#define _TGT_AP_DDR_LOWPWR
#define _TGT_AP_DDR_ODT         (2)
#define _TGT_AP_DDR_RON         (0)
#define _TGT_AP_DDR_CHIP_BITS   (0)

#define _TGT_AP_DDR_WIDTH       (32)

#if (_TGT_AP_DDR_WIDTH == 8)
#define _TGT_AP_DDR_MEM_BITS    (0)
#elif (_TGT_AP_DDR_WIDTH == 16)
#define _TGT_AP_DDR_MEM_BITS    (1)
#elif (_TGT_AP_DDR_WIDTH == 32)
#define _TGT_AP_DDR_MEM_BITS    (2)
#else
#error "Invalid DDR WIDTH"
#endif

#define _TGT_AP_DDR_BANK_BITS   (3)
#define _TGT_AP_DDR_ROW_BITS    (3)
#define _TGT_AP_DDR_COL_BITS    (1)

/*
 * CLK settings
 */
#define _TGT_AP_CLK_CPU		(400)
#define _TGT_AP_CLK_AXI		(400)
#define _TGT_AP_CLK_AHB		(200)
#define _TGT_AP_CLK_APB		(200)
#define _TGT_AP_CLK_MEM		(_TGT_AP_DDR_CLOCK)
#define _TGT_AP_CLK_FLH_CTRL	(200)
#define _TGT_AP_CLK_FLH_DEV0	(200)
#define _TGT_AP_CLK_FLH_DEV1	(200)

#endif //_TGT_AP_CLOCK_CFG_H_
