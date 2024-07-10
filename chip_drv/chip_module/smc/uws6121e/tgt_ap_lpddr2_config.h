#ifndef _TGT_AP_LPDDR2_CFG_H_
#define _TGT_AP_LPDDR2_CFG_H_
#include "uix8910_clk_cfg.h"

#if 1//!defined(CONFIG_RDA_FPGA)
/*
 * EVB board
 */
#if (_TGT_AP_DDR_CLOCK == 200)
#include "ddr_timing/8910_lpddr2_16x2_200m.h"
#elif (_TGT_AP_DDR_CLOCK == 333)
#include "ddr_timing/8910_lpddr2_16x2_333m.h"
#elif (_TGT_AP_DDR_CLOCK == 400)
#include "ddr_timing/8910_lpddr2_16x2_400m.h"
#elif (_TGT_AP_DDR_CLOCK == 533)
#include "ddr_timing/8910_lpddr2_16x2_533m.h"
#else
#error "Invalid _TGT_AP_DDR_CLOCK value for LPDDR2 !"
#endif

/*
 * FPGA board
 */
#else
#include "ddr_timing/8910_lpddr2_16x2_60m_fpga.h"
#endif

#endif
