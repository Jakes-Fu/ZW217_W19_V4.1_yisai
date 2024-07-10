/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _RDA8910M_HARD_H_
#define _RDA8910M_HARD_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'rda8910m_hard'."
#endif


#include "globals.h"
#include "global_macros.h"
#include "gallite_generic_config.h"
#include "debug_host.h"
#include "debug_host_internals.h"
#include "debug_uart.h"
#include "arm_axidma.h"
#include "aes.h"
#include "lzma.h"
#include "f8.h"
#include "fpi3_gprs.h"
#include "gouda.h"
#include "lcdc.h"
#include "spi_flash.h"
#include "gic400_reg.h"
#include "gpio.h"
#include "i2c_master.h"
#include "pagespy_dmc.h"
#include "vad.h"
#include "sci.h"
#include "lps.h"
#include "spi.h"
#include "sys_ctrl.h"
#include "iomux.h"
#include "spinlock.h"
#include "efuse_ctrl.h"
#include "ap_ifc.h"
#include "aon_ifc.h"
#include "timer.h"
#include "timer_ap.h"
//#include "arm_uart.h"
#include "keypad.h"
#include "pwm.h"
#include "calendar.h"
#include "aif.h"
#include "aud_2ad.h"
#include "usbc.h"
#include "usbc11.h"
#include "sdmmc.h"
#include "camera.h"
#include "adi_mst.h"
#include "analog_reg.h"
#include "lvds.h"
#include "sys_imem.h"
#include "sys_axi_cfg.h"
#include "dmc400.h"
#include "lpddr_phy.h"
#include "psram_phy.h"
#include "lpddr_psram.h"
#include "dbg_a5.h"
#include "pmu_a5.h"
#include "etm_a5.h"
#include "cti.h"
#include "etb.h"
#include "atb_funnel.h"
#include "timestamp.h"
#include "cp_zsp_uart.h"
#include "rda2720m_adc.h"
#include "rda2720m_aud.h"
#include "rda2720m_aud_codec.h"
#include "rda2720m_aud_ifa.h"
#include "rda2720m_bltc.h"
#include "rda2720m_cal.h"
#include "rda2720m_eic.h"
#include "rda2720m_fgu.h"
#include "rda2720m_global.h"
#include "rda2720m_int.h"
#include "rda2720m_pin.h"
#include "rda2720m_psm.h"
#include "rda2720m_rtc.h"
#include "rda2720m_tmr.h"
#include "rda2720m_wdg.h"
#include "cp_idle.h"
#include "cp_mailbox.h"
#include "cp_clkrst.h"
#include "cp_pwrctrl.h"
#include "cp_sysreg.h"
#include "cp_monitor.h"
#include "cp_bb_wd.h"
#include "cp_zsp_irqh.h"
#include "cp_zsp_axidma.h"
#include "cp_zsp_aud_dft.h"
#include "cp_zsp_busmon.h"
#include "cp_zsp_wd.h"
#include "cp_lte_pusch.h"
#include "cp_lte_ldtc1.h"
#include "cp_lte_ldtc.h"
#include "cp_lte_dlfft.h"
#include "cp_lte_coeff.h"
#include "cp_lte_rfad.h"
#include "cp_lte_uldft.h"
#include "cp_lte_txrx.h"
#include "cp_lte_measpwr.h"
#include "cp_lte_iddet.h"
#include "cp_lte_csirs.h"
#include "cp_lte_ulpcdci.h"
#include "cp_lte_corr.h"
#include "cp_lte_otdoa.h"
#include "cp_lte_rxcapt.h"
#include "bb2g_ram.h"
#include "bb_cp2.h"
#include "bb_irq.h"
#include "bb_rom.h"
#include "cholk.h"
#include "cipher.h"
#include "cipher_a53.h"
#include "cordic.h"
#include "excor.h"
#include "itlv.h"
#include "rf_if.h"
#include "rf_spi.h"
#include "sys_ifc.h"
#include "gge_sys_ctrl.h"
#include "tcu.h"
#include "vitac.h"
#include "wdt.h"
#include "xcor.h"
#include "mips32r6.h"
#include "ela.h"
#include "evitac.h"
#include "dma.h"
#include "nb_cell_search.h"
#include "nb_common.h"
#include "nb_ctrl.h"
#include "nb_ds_bsel.h"
#include "nb_fft_rsrp.h"
#include "nb_intc.h"
#include "nb_meas.h"
#include "nb_tx_chsc.h"
#include "nb_tx_frontend.h"
#include "nb_tx_pusch_encoder.h"
#include "nb_viterbi.h"
#include "rf_registers.h"
#include "rf_timer.h"
#include "rf_uart.h"
#include "rf_wdt.h"
#include "rf_sys_ctrl.h"
#include "rffe_reg.h"
#include "rf_pulp_irq.h"
#include "rf_pulp_sleep.h"
#include "rf_pulp_debug_unit.h"
#include "rf_rtc.h"
#include "tsen_adc.h"
#include "dfe.h"
#include "rf_dlpf.h"
#include "rf_et.h"
#include "wcn_bb_ifc.h"
#include "wcn_ble_link.h"
#include "wcn_bt_core.h"
#include "wcn_bt_link.h"
#include "wcn_bt_modem.h"
#include "wcn_cache_ctrl.h"
#include "wcn_comregs.h"
#include "wcn_dbm.h"
#include "wcn_fm_dsp.h"
#include "wcn_pulp_irq.h"
#include "wcn_pulp_sleep.h"
#include "wcn_pulp_debug_unit.h"
#include "wcn_rf_if.h"
#include "wcn_sys_ctrl.h"
#include "wcn_sys_ifc.h"
#include "wcn_systick.h"
#include "wcn_trap.h"
#include "wcn_uart.h"
#include "wcn_wdt.h"
#include "wcn_wlan.h"

// =============================================================================
//  MACROS
// =============================================================================
/// Ap APB base
#define REG_AP_APB_BASE                         (0X08800000)
#define REG_SYS_AHB_BASE                        (0X09000000)
/// PSRAM base
#define LPDDR_BASE                              (0X80000000)
#define LPDDR_BASE_FOR_GGE                      (0X08000000)
/// PSRAM base
#define PSRAM_BASE                              (0X80000000)
#define PSRAM_BASE_FOR_GGE                      (0X08000000)
/// ADI mst base
#define AON_ADI_MST_BASE                        (0X50300000)
/// System AON APB base
#define REG_AON_APB_BASE                        (0X50100000)
#define REG_AON_APB_BASE_FOR_GGE                (0X05100000)
/// COREISHGT Base
#define AON_CORESIGHT_BASE                      (0X50400000)
#define BB_SYS_ADDR_BASE                        (0X50080000)
#define BB_SYS_ADDR_BASE_FOR_GGE                (0X05080000)
#define ZSP_SYS_ADDR_BASE                       (0X22000000)
#define LTE_SYS_ADDR_BASE                       (0X25000000)
#define GGE_BB_APB_ADDR_BASE                    (0X40000000)
#define GGE_BB_APB_ADDR_BASE_FOR_GGE            (0X00000000)
#define GGE_SYS_APB_ADDR_BASE                   (0X40020000)
#define GGE_SYS_APB_ADDR_BASE_FOR_GGE           (0X00020000)
#define RF_APB_ADDR_BASE                        (0X50030000)
#define RF_APB_ADDR_BASE_FOR_GGE                (0X05030000)
#define WCN_SYS_APB_ADDR_BASE                   (0X15000000)

// =============================================================================
//  TYPES
// =============================================================================



#endif

