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


#ifndef _LPS_H_
#define _LPS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'lps'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define LP_FRAC_NB_BITS                         (16)
#define LP_COUNT_INT_NB_BITS                    (19)
#define LP_COUNT_NB_BITS                        (LP_COUNT_INT_NB_BITS + LP_FRAC_NB_BITS)
#define LP_RATE_INT_NB_BITS                     (12)
#define LP_RATE_NB_BITS                         (LP_RATE_INT_NB_BITS + LP_FRAC_NB_BITS)
#define SYS_COUNT_NB_BITS                       (29)
#define FRAME_COUNT_NB_BITS                     (32)
#define PU_COUNT_NB_BITS                        (12)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// LPS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_GSM_LPS_BASE           0x05102000
#else
#define REG_GSM_LPS_BASE           0x50102000
#endif
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_NB_LPS_BASE            0x0510B000
#else
#define REG_NB_LPS_BASE            0x5010B000
#endif

typedef volatile struct
{
    REG32                          LPS_SF_Ctrl;                  //0x00000000
    REG32                          LPS_SF_Status;                //0x00000004
    REG32                          LPS_SF_Frames;                //0x00000008
    REG32                          LPS_SF_PU_Frames;             //0x0000000C
    REG32                          LPS_SF_Restart_Time;          //0x00000010
    REG32                          LPS_SF_Frame_Period;          //0x00000014
    /// The rate is the number of System Clocks per 32kHz Clocks.
    REG32                          LPS_SF_Rate;                  //0x00000018
    REG32                          LPS_SF_Elapsed_Frames;        //0x0000001C
    REG32                          LPS_SF_Sys_Count;             //0x00000020
    REG32                          LPS_IRQ;                      //0x00000024
    REG32                          LPS_IRQ_Mask;                 //0x00000028
} HWP_LPS_T;

#define hwp_gsmLps                 ((HWP_LPS_T*) REG_ACCESS_ADDRESS(REG_GSM_LPS_BASE))
#define hwp_nbLps                  ((HWP_LPS_T*) REG_ACCESS_ADDRESS(REG_NB_LPS_BASE))


//LPS_SF_Ctrl
typedef union {
    REG32 v;
    struct {
        REG32 lps_sf_enable : 1; // [0]
        REG32 __3_1 : 3;
        REG32 lps_sf_lowpower : 2; // [5:4]
        REG32 __7_6 : 2;
        REG32 lps_sf_wakeup0 : 1; // [8]
        REG32 lps_sf_wakeup0_cfg : 1; // [9]
        REG32 __31_10 : 22;
    } b;
} REG_LPS_LPS_SF_CTRL_T;

//LPS_SF_Status
typedef union {
    REG32 v;
    struct {
        REG32 lps_sf_ready : 1; // [0], read only
        REG32 __3_1 : 3;
        REG32 lps_sf_slowrunning : 1; // [4], read only
        REG32 __7_5 : 3;
        REG32 lps_sf_calibrationdone : 1; // [8], read only
        REG32 __11_9 : 3;
        REG32 lps_sf_pu_reached : 1; // [12], read only
        REG32 __15_13 : 3;
        REG32 lps_sf_tcu_restart : 1; // [16], read only
        REG32 __31_17 : 15;
    } b;
} REG_LPS_LPS_SF_STATUS_T;

//LPS_SF_Restart_Time
typedef union {
    REG32 v;
    struct {
        REG32 lps_sf_restart_time : 19; // [18:0]
        REG32 __31_19 : 13;
    } b;
} REG_LPS_LPS_SF_RESTART_TIME_T;

//LPS_SF_Frame_Period
typedef union {
    REG32 v;
    struct {
        REG32 lps_sf_frame_period : 19; // [18:0]
        REG32 __31_19 : 13;
    } b;
} REG_LPS_LPS_SF_FRAME_PERIOD_T;

//LPS_SF_Rate
typedef union {
    REG32 v;
    struct {
        REG32 lps_sf_rate_frac : 16; // [15:0]
        REG32 lps_sf_rate_int : 12; // [27:16]
        REG32 __31_28 : 4;
    } b;
} REG_LPS_LPS_SF_RATE_T;

//LPS_SF_Sys_Count
typedef union {
    REG32 v;
    struct {
        REG32 lps_sf_sys_count : 29; // [28:0], read only
        REG32 __31_29 : 3;
    } b;
} REG_LPS_LPS_SF_SYS_COUNT_T;

//LPS_IRQ
typedef union {
    REG32 v;
    struct {
        REG32 lps_irq_calibration_done_cause : 1; // [0], write clear
        REG32 lps_irq_slow_running_cause : 1; // [1], write clear
        REG32 lps_irq_pu_reached_cause : 1; // [2], write clear
        REG32 lps_irq_tcu_restart_cause : 1; // [3], write clear
        REG32 __15_4 : 12;
        REG32 lps_irq_calibration_done_status : 1; // [16], write clear
        REG32 lps_irq_slow_running_status : 1; // [17], write clear
        REG32 lps_irq_pu_reached_status : 1; // [18], write clear
        REG32 lps_irq_tcu_restart_status : 1; // [19], write clear
        REG32 __31_20 : 12;
    } b;
} REG_LPS_LPS_IRQ_T;

//LPS_IRQ_Mask
typedef union {
    REG32 v;
    struct {
        REG32 lps_irq_calibration_done_mask : 1; // [0]
        REG32 lps_irq_slow_running_mask : 1; // [1]
        REG32 lps_irq_pu_reached_mask : 1; // [2]
        REG32 lps_irq_tcu_restart_mask : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_LPS_LPS_IRQ_MASK_T;


//LPS_SF_Ctrl
#define LPS_LPS_SF_ENABLE          (1<<0)
#define LPS_LPS_SF_LOWPOWER(n)     (((n)&3)<<4)
#define LPS_LPS_SF_LOWPOWER_MASK   (3<<4)
#define LPS_LPS_SF_LOWPOWER_SHIFT  (4)
#define LPS_LPS_SF_LOWPOWER_STOP   (0<<4)
#define LPS_LPS_SF_LOWPOWER_SKIPFRAME (1<<4)
#define LPS_LPS_SF_LOWPOWER_CALIB  (3<<4)
#define LPS_LPS_SF_LOWPOWER_V_STOP (0)
#define LPS_LPS_SF_LOWPOWER_V_SKIPFRAME (1)
#define LPS_LPS_SF_LOWPOWER_V_CALIB (3)
#define LPS_LPS_SF_WAKEUP0_DISABLED (0<<8)
#define LPS_LPS_SF_WAKEUP0_ENABLED (1<<8)
#define LPS_LPS_SF_WAKEUP0_V_DISABLED (0)
#define LPS_LPS_SF_WAKEUP0_V_ENABLED (1)
#define LPS_LPS_SF_WAKEUP0_CFG_0   (0<<9)
#define LPS_LPS_SF_WAKEUP0_CFG_1   (1<<9)
#define LPS_LPS_SF_WAKEUP0_CFG_V_0 (0)
#define LPS_LPS_SF_WAKEUP0_CFG_V_1 (1)

//LPS_SF_Status
#define LPS_LPS_SF_READY           (1<<0)
#define LPS_LPS_SF_SLOWRUNNING     (1<<4)
#define LPS_LPS_SF_CALIBRATIONDONE (1<<8)
#define LPS_LPS_SF_PU_REACHED      (1<<12)
#define LPS_LPS_SF_TCU_RESTART     (1<<16)

//LPS_SF_Frames
#define LPS_LPS_SF_FRAME(n)        (((n)&0xFFFFFFFF)<<0)

//LPS_SF_PU_Frames
#define LPS_LPS_SF_PU_FRAME(n)     (((n)&0xFFFFFFFF)<<0)

//LPS_SF_Restart_Time
#define LPS_LPS_SF_RESTART_TIME(n) (((n)&0x7FFFF)<<0)

//LPS_SF_Frame_Period
#define LPS_LPS_SF_FRAME_PERIOD(n) (((n)&0x7FFFF)<<0)

//LPS_SF_Rate
#define LPS_LPS_SF_RATE_INT(n)     (((n)&0xFFF)<<16)
#define LPS_LPS_SF_RATE_FRAC(n)    (((n)&0xFFFF)<<0)

//LPS_SF_Elapsed_Frames
#define LPS_LPS_SF_ELAPSED_FRAMES(n) (((n)&0xFFFFFFFF)<<0)

//LPS_SF_Sys_Count
#define LPS_LPS_SF_SYS_COUNT(n)    (((n)&0x1FFFFFFF)<<0)

//LPS_IRQ
#define LPS_LPS_IRQ_CALIBRATION_DONE_CAUSE (1<<0)
#define LPS_LPS_IRQ_SLOW_RUNNING_CAUSE (1<<1)
#define LPS_LPS_IRQ_PU_REACHED_CAUSE (1<<2)
#define LPS_LPS_IRQ_TCU_RESTART_CAUSE (1<<3)
#define LPS_LPS_IRQ_CALIBRATION_DONE_STATUS (1<<16)
#define LPS_LPS_IRQ_SLOW_RUNNING_STATUS (1<<17)
#define LPS_LPS_IRQ_PU_REACHED_STATUS (1<<18)
#define LPS_LPS_IRQ_TCU_RESTART_STATUS (1<<19)
#define LPS_LPS_PU_IRQ_CAUSE(n)    (((n)&15)<<0)
#define LPS_LPS_PU_IRQ_CAUSE_MASK  (15<<0)
#define LPS_LPS_PU_IRQ_CAUSE_SHIFT (0)
#define LPS_LPS_PU_IRQ_STATUS(n)   (((n)&15)<<16)
#define LPS_LPS_PU_IRQ_STATUS_MASK (15<<16)
#define LPS_LPS_PU_IRQ_STATUS_SHIFT (16)

//LPS_IRQ_Mask
#define LPS_LPS_IRQ_CALIBRATION_DONE_MASK (1<<0)
#define LPS_LPS_IRQ_SLOW_RUNNING_MASK (1<<1)
#define LPS_LPS_IRQ_PU_REACHED_MASK (1<<2)
#define LPS_LPS_IRQ_TCU_RESTART_MASK (1<<3)
#define LPS_LPS_IRQ_MASK(n)        (((n)&15)<<0)
#define LPS_LPS_IRQ_MASK_MASK      (15<<0)
#define LPS_LPS_IRQ_MASK_SHIFT     (0)





#endif

