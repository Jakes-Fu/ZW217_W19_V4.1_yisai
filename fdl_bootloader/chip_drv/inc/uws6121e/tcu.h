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


#ifndef _TCU_H_
#define _TCU_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'tcu'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define NB_TCO                                  (12)
#define NB_TCU_PROG_EVENTS                      (60)

// ============================================================================
// INTERNAL_TCO_MAPPING_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Internal TCO mapping
    TCO_GMSK_ON                                 = 0x00000006,
    TCO_TX_OEN                                  = 0x00000007,
    TCO_TX_ON                                   = 0x00000008,
    TCO_RX_ON                                   = 0x00000009,
    TCO_RX_DEC_ON                               = 0x0000000A,
    TCO_PDN                                     = 0x0000000B
} INTERNAL_TCO_MAPPING_T;


// ============================================================================
// TCU_EVENT_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Clear TCO 0 : set the TCO 0 to the inactive state
/// To clear TCO n, use event 2*n
    CLR_TCO_0                                   = 0x00000000,
/// Set TCO 0 : set the TCO 0 to the active state
/// To set TCO n, use event 2*n+1
    SET_TCO_0                                   = 0x00000001,
/// ...
    CLR_TCO_1                                   = 0x00000002,
    SET_TCO_1                                   = 0x00000003,
    CLR_TCO_2                                   = 0x00000004,
    SET_TCO_2                                   = 0x00000005,
    CLR_TCO_3                                   = 0x00000006,
    SET_TCO_3                                   = 0x00000007,
    CLR_TCO_4                                   = 0x00000008,
    SET_TCO_4                                   = 0x00000009,
    CLR_TCO_5                                   = 0x0000000A,
    SET_TCO_5                                   = 0x0000000B,
/// stop modulation
    STOP_GMSK                                   = 0x0000000C,
/// starts modulation and output on IQ DAC
    START_GMSK                                  = 0x0000000D,
    HIGHZ_IQ_DAC                                = 0x0000000E,
    DRIVE_IQ_DAC                                = 0x0000000F,
    DISABLE_IQ_DAC                              = 0x00000010,
    ENABLE_IQ_DAC                               = 0x00000011,
/// disable IQ ADC
    DISABLE_IQ_ADC                              = 0x00000012,
/// enable IQ ADC
    ENABLE_IQ_ADC                               = 0x00000013,
/// stop recording IQ samples
    STOP_RFIN_RECORD                            = 0x00000014,
/// start recording IQ samples
    START_RFIN_RECORD                           = 0x00000015,
/// Clear RF_PDN
    CLR_PDN                                     = 0x00000016,
/// Set RF_PDN
    SET_PDN                                     = 0x00000017,
/// Send RF spi command
    SEND_SPI_CMD                                = 0x00000018,
    NEXT_GAIN                                   = 0x00000019,
    FIRST_GAIN                                  = 0x0000001A,
    NEXT_FC                                     = 0x0000001B,
/// Start Ramp 0
    PA_RAMP0                                    = 0x0000001C,
/// Start Ramp 1
    PA_RAMP1                                    = 0x0000001D,
/// Start Ramp 2
    PA_RAMP2                                    = 0x0000001E,
/// Start Ramp 3
    PA_RAMP3                                    = 0x0000001F,
/// Start Ramp 4
    PA_RAMP4                                    = 0x00000020,
    RX_SOC                                      = 0x00000021,
    DIGRF_STB                                   = 0x00000022,
/// Trigger BCPU TCU irq 0
    BCPU_TCU_IRQ0                               = 0x00000023,
/// Trigger BCPU TCU irq 1
    BCPU_TCU_IRQ1                               = 0x00000024,
/// Trigger XCPU TCU irq 0
    XCPU_TCU_IRQ0                               = 0x00000025,
/// Trigger XCPU TCU irq 1
    XCPU_TCU_IRQ1                               = 0x00000026,
/// End of the WakeUp Mode
    WAKEUP_DONE                                 = 0x00000027,
/// Start of Rf_spi Transfer
    RFSPI_START                                 = 0x00000028,
/// End of Rf_spi Transfer
    RFSPI_END                                   = 0x00000029,
    NO_EVENT                                    = 0x0000003F
} TCU_EVENT_T;


// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// TCU_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_TCU_GSM_BASE           0x00021000
#else
#define REG_TCU_GSM_BASE           0x40021000
#endif
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_TCU_NB_BASE            0x00022000
#else
#define REG_TCU_NB_BASE            0x40022000
#endif

typedef volatile struct
{
    REG32                          Ctrl;                         //0x00000000
    REG32                          Wrap_Val;                     //0x00000004
    REG32                          Cur_Val;                      //0x00000008
    REG32                          Latch;                        //0x0000000C
    REG32                          Setup;                        //0x00000010
    REG32                          Disable_Event;                //0x00000014
    REG32                          Enable_Event;                 //0x00000018
    REG32                          Set_TCO;                      //0x0000001C
    REG32                          Clr_TCO;                      //0x00000020
    REG32                          Cfg_Clk_Div;                  //0x00000024
    REG32                          TCU_IRQ;                      //0x00000028
    REG32                          TCU_IRQ_Mask;                 //0x0000002C
    REG32                          GLOBAL_SYNC_CTRL;             //0x00000030
    REG32                          TCO_DBG_SEL;                  //0x00000034
    REG32                          Rfspi_Conflict_Val;           //0x00000038
    REG32 Reserved_0000003C[49];                //0x0000003C
    REG32                          Event[NB_TCU_PROG_EVENTS];    //0x00000100
} HWP_TCU_T;

#define hwp_tcuGsm                 ((HWP_TCU_T*) REG_ACCESS_ADDRESS(REG_TCU_GSM_BASE))
#define hwp_tcuNb                  ((HWP_TCU_T*) REG_ACCESS_ADDRESS(REG_TCU_NB_BASE))


//Ctrl
typedef union {
    REG32 v;
    struct {
        REG32 load_val : 14; // [13:0]
        REG32 __15_14 : 2;
        REG32 enable : 1; // [16]
        REG32 __19_17 : 3;
        REG32 load : 1; // [20]
        REG32 __27_21 : 7;
        REG32 nolatch : 1; // [28]
        REG32 __29_29 : 1;
        REG32 wakeup_en : 1; // [30]
        REG32 __31_31 : 1;
    } b;
} REG_TCU_CTRL_T;

//Wrap_Val
typedef union {
    REG32 v;
    struct {
        REG32 wrap_val : 14; // [13:0]
        REG32 __31_14 : 18;
    } b;
} REG_TCU_WRAP_VAL_T;

//Cur_Val
typedef union {
    REG32 v;
    struct {
        REG32 cur_val : 14; // [13:0], read only
        REG32 __31_14 : 18;
    } b;
} REG_TCU_CUR_VAL_T;

//Latch
typedef union {
    REG32 v;
    struct {
        REG32 forcelatch_area_0 : 1; // [0]
        REG32 forcelatch_area_1 : 1; // [1]
        REG32 forcelatch_area_2 : 1; // [2]
        REG32 forcelatch_area_3 : 1; // [3]
        REG32 forcelatch_area_4 : 1; // [4]
        REG32 forcelatch_area_5 : 1; // [5]
        REG32 forcelatch_area_6 : 1; // [6]
        REG32 forcelatch_area_7 : 1; // [7]
        REG32 __15_8 : 8;
        REG32 force_noevent : 1; // [16]
        REG32 __30_17 : 14;
        REG32 clearprogarea : 1; // [31]
    } b;
} REG_TCU_LATCH_T;

//Setup
typedef union {
    REG32 v;
    struct {
        REG32 pol_tco_0 : 1; // [0]
        REG32 pol_tco_1 : 1; // [1]
        REG32 pol_tco_2 : 1; // [2]
        REG32 pol_tco_3 : 1; // [3]
        REG32 pol_tco_4 : 1; // [4]
        REG32 pol_tco_5 : 1; // [5]
        REG32 pol_tco_6 : 1; // [6]
        REG32 pol_tco_7 : 1; // [7]
        REG32 pol_tco_8 : 1; // [8]
        REG32 pol_tco_9 : 1; // [9]
        REG32 pol_tco_10 : 1; // [10]
        REG32 pol_tco_11 : 1; // [11]
        REG32 __27_12 : 16;
        REG32 write_error : 1; // [28], write clear
        REG32 __30_29 : 2;
        REG32 debug_active : 1; // [31]
    } b;
} REG_TCU_SETUP_T;

//Disable_Event
typedef union {
    REG32 v;
    struct {
        REG32 disable_tco_0 : 1; // [0]
        REG32 disable_tco_1 : 1; // [1]
        REG32 disable_tco_2 : 1; // [2]
        REG32 disable_tco_3 : 1; // [3]
        REG32 disable_tco_4 : 1; // [4]
        REG32 disable_tco_5 : 1; // [5]
        REG32 disable_tco_gmsk_on : 1; // [6]
        REG32 disable_tco_tx_oen : 1; // [7]
        REG32 disable_tco_tx_on : 1; // [8]
        REG32 disable_tco_rx_on : 1; // [9]
        REG32 disable_tco_rx_dec_on : 1; // [10]
        REG32 disable_tco_pdn : 1; // [11]
        REG32 disable_send_spi_cmd : 1; // [12]
        REG32 disable_next_gain : 1; // [13]
        REG32 disable_first_gain : 1; // [14]
        REG32 disable_next_fc : 1; // [15]
        REG32 disable_ramp_0 : 1; // [16]
        REG32 disable_ramp_1 : 1; // [17]
        REG32 disable_ramp_2 : 1; // [18]
        REG32 disable_ramp_3 : 1; // [19]
        REG32 disable_ramp_4 : 1; // [20]
        REG32 disable_rx_soc : 1; // [21]
        REG32 disable_digrf_strobe : 1; // [22]
        REG32 disable_bcpu_irq_0 : 1; // [23]
        REG32 disable_bcpu_irq_1 : 1; // [24]
        REG32 disable_xcpu_irq_0 : 1; // [25]
        REG32 disable_xcpu_irq_1 : 1; // [26]
        REG32 __27_27 : 1;
        REG32 disable_rfspi_start : 1; // [28]
        REG32 disable_rfspi_end : 1; // [29]
        REG32 __30_30 : 1;
        REG32 disable_rf_spi_marked_cmd : 1; // [31]
    } b;
} REG_TCU_DISABLE_EVENT_T;

//Enable_Event
typedef union {
    REG32 v;
    struct {
        REG32 enable_tco_0 : 1; // [0], write set
        REG32 enable_tco_1 : 1; // [1], write set
        REG32 enable_tco_2 : 1; // [2], write set
        REG32 enable_tco_3 : 1; // [3], write set
        REG32 enable_tco_4 : 1; // [4], write set
        REG32 enable_tco_5 : 1; // [5], write set
        REG32 enable_tco_gmsk_on : 1; // [6], write set
        REG32 enable_tco_tx_oen : 1; // [7], write set
        REG32 enable_tco_tx_on : 1; // [8], write set
        REG32 enable_tco_rx_on : 1; // [9], write set
        REG32 enable_tco_rx_dec_on : 1; // [10], write set
        REG32 enable_tco_pdn : 1; // [11], write set
        REG32 enable_send_spi_cmd : 1; // [12], write set
        REG32 enable_next_gain : 1; // [13], write set
        REG32 enable_first_gain : 1; // [14], write set
        REG32 enable_next_fc : 1; // [15], write set
        REG32 enable_ramp_0 : 1; // [16], write set
        REG32 enable_ramp_1 : 1; // [17], write set
        REG32 enable_ramp_2 : 1; // [18], write set
        REG32 enable_ramp_3 : 1; // [19], write set
        REG32 enable_ramp_4 : 1; // [20], write set
        REG32 enable_rx_soc : 1; // [21], write set
        REG32 enable_digrf_strobe : 1; // [22], write set
        REG32 enable_bcpu_irq_0 : 1; // [23], write set
        REG32 enable_bcpu_irq_1 : 1; // [24], write set
        REG32 enable_xcpu_irq_0 : 1; // [25], write set
        REG32 enable_xcpu_irq_1 : 1; // [26], write set
        REG32 __27_27 : 1;
        REG32 enable_rfspi_start : 1; // [28]
        REG32 enable_rfspi_end : 1; // [29]
        REG32 __30_30 : 1;
        REG32 enable_rf_spi_marked_cmd : 1; // [31], write set
    } b;
} REG_TCU_ENABLE_EVENT_T;

//Set_TCO
typedef union {
    REG32 v;
    struct {
        REG32 set_tco_0 : 1; // [0], write set
        REG32 set_tco_1 : 1; // [1], write set
        REG32 set_tco_2 : 1; // [2], write set
        REG32 set_tco_3 : 1; // [3], write set
        REG32 set_tco_4 : 1; // [4], write set
        REG32 set_tco_5 : 1; // [5], write set
        REG32 set_tco_6 : 1; // [6], write set
        REG32 set_tco_7 : 1; // [7], write set
        REG32 set_tco_8 : 1; // [8], write set
        REG32 set_tco_9 : 1; // [9], write set
        REG32 set_tco_10 : 1; // [10], write set
        REG32 set_tco_11 : 1; // [11], write set
        REG32 __31_12 : 20;
    } b;
} REG_TCU_SET_TCO_T;

//Clr_TCO
typedef union {
    REG32 v;
    struct {
        REG32 clr_tco_0 : 1; // [0], write clear
        REG32 clr_tco_1 : 1; // [1], write clear
        REG32 clr_tco_2 : 1; // [2], write clear
        REG32 clr_tco_3 : 1; // [3], write clear
        REG32 clr_tco_4 : 1; // [4], write clear
        REG32 clr_tco_5 : 1; // [5], write clear
        REG32 clr_tco_6 : 1; // [6], write clear
        REG32 clr_tco_7 : 1; // [7], write clear
        REG32 clr_tco_8 : 1; // [8], write clear
        REG32 clr_tco_9 : 1; // [9], write clear
        REG32 clr_tco_10 : 1; // [10], write clear
        REG32 clr_tco_11 : 1; // [11], write clear
        REG32 __31_12 : 20;
    } b;
} REG_TCU_CLR_TCO_T;

//Cfg_Clk_Div
typedef union {
    REG32 v;
    struct {
        REG32 __28_0 : 29;
        REG32 tcu_clk_same_sys : 1; // [29]
        REG32 enable_dai_simple_208k : 1; // [30]
        REG32 enable_qbit : 1; // [31]
    } b;
} REG_TCU_CFG_CLK_DIV_T;

//TCU_IRQ
typedef union {
    REG32 v;
    struct {
        REG32 tcu_sync_done_cause : 1; // [0], write clear
        REG32 __15_1 : 15;
        REG32 tcu_sync_done_status : 1; // [16], write clear
        REG32 __31_17 : 15;
    } b;
} REG_TCU_TCU_IRQ_T;

//TCU_IRQ_Mask
typedef union {
    REG32 v;
    struct {
        REG32 tcu_sync_done_mask : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_TCU_TCU_IRQ_MASK_T;

//GLOBAL_SYNC_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 tcu_sync_enable : 1; // [0]
        REG32 __1_1 : 1;
        REG32 tcu_sync_value : 14; // [15:2]
        REG32 __31_16 : 16;
    } b;
} REG_TCU_GLOBAL_SYNC_CTRL_T;

//TCO_DBG_SEL
typedef union {
    REG32 v;
    struct {
        REG32 tco_dbg0_sel : 4; // [3:0]
        REG32 tco_dbg1_sel : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_TCU_TCO_DBG_SEL_T;

//Rfspi_Conflict_Val
typedef union {
    REG32 v;
    struct {
        REG32 rfspi_conflict_val : 14; // [13:0], read only
        REG32 __31_14 : 18;
    } b;
} REG_TCU_RFSPI_CONFLICT_VAL_T;

//Event
typedef union {
    REG32 v;
    struct {
        REG32 event_time : 14; // [13:0]
        REG32 __15_14 : 2;
        REG32 event_id : 6; // [21:16]
        REG32 __31_22 : 10;
    } b;
} REG_TCU_EVENT_T;


//Ctrl
#define TCU_LOAD_VAL(n)            (((n)&0x3FFF)<<0)
#define TCU_ENABLE_DISABLED        (0<<16)
#define TCU_ENABLE_ENABLED         (1<<16)
#define TCU_ENABLE_V_DISABLED      (0)
#define TCU_ENABLE_V_ENABLED       (1)
#define TCU_LOAD                   (1<<20)
#define TCU_NOLATCH_NORMAL         (0<<28)
#define TCU_NOLATCH_FORCE_ONLY     (1<<28)
#define TCU_NOLATCH_V_NORMAL       (0)
#define TCU_NOLATCH_V_FORCE_ONLY   (1)
#define TCU_WAKEUP_EN              (1<<30)

//Wrap_Val
#define TCU_WRAP_VAL(n)            (((n)&0x3FFF)<<0)

//Cur_Val
#define TCU_CUR_VAL(n)             (((n)&0x3FFF)<<0)

//Latch
#define TCU_FORCELATCH_AREA_0      (1<<0)
#define TCU_FORCELATCH_AREA_1      (1<<1)
#define TCU_FORCELATCH_AREA_2      (1<<2)
#define TCU_FORCELATCH_AREA_3      (1<<3)
#define TCU_FORCELATCH_AREA_4      (1<<4)
#define TCU_FORCELATCH_AREA_5      (1<<5)
#define TCU_FORCELATCH_AREA_6      (1<<6)
#define TCU_FORCELATCH_AREA_7      (1<<7)
#define TCU_FORCE_NOEVENT          (1<<16)
#define TCU_CLEARPROGAREA          (1<<31)
#define TCU_FORCELATCH(n)          (((n)&0xFF)<<0)
#define TCU_FORCELATCH_MASK        (0xFF<<0)
#define TCU_FORCELATCH_SHIFT       (0)

//Setup
#define TCU_POL_TCO_0_ACTIVE_HIGH  (0<<0)
#define TCU_POL_TCO_0_ACTIVE_LOW   (1<<0)
#define TCU_POL_TCO_0_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_0_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_1_ACTIVE_HIGH  (0<<1)
#define TCU_POL_TCO_1_ACTIVE_LOW   (1<<1)
#define TCU_POL_TCO_1_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_1_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_2_ACTIVE_HIGH  (0<<2)
#define TCU_POL_TCO_2_ACTIVE_LOW   (1<<2)
#define TCU_POL_TCO_2_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_2_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_3_ACTIVE_HIGH  (0<<3)
#define TCU_POL_TCO_3_ACTIVE_LOW   (1<<3)
#define TCU_POL_TCO_3_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_3_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_4_ACTIVE_HIGH  (0<<4)
#define TCU_POL_TCO_4_ACTIVE_LOW   (1<<4)
#define TCU_POL_TCO_4_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_4_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_5_ACTIVE_HIGH  (0<<5)
#define TCU_POL_TCO_5_ACTIVE_LOW   (1<<5)
#define TCU_POL_TCO_5_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_5_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_6_ACTIVE_HIGH  (0<<6)
#define TCU_POL_TCO_6_ACTIVE_LOW   (1<<6)
#define TCU_POL_TCO_6_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_6_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_7_ACTIVE_HIGH  (0<<7)
#define TCU_POL_TCO_7_ACTIVE_LOW   (1<<7)
#define TCU_POL_TCO_7_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_7_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_8_ACTIVE_HIGH  (0<<8)
#define TCU_POL_TCO_8_ACTIVE_LOW   (1<<8)
#define TCU_POL_TCO_8_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_8_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_9_ACTIVE_HIGH  (0<<9)
#define TCU_POL_TCO_9_ACTIVE_LOW   (1<<9)
#define TCU_POL_TCO_9_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_9_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_10_ACTIVE_HIGH (0<<10)
#define TCU_POL_TCO_10_ACTIVE_LOW  (1<<10)
#define TCU_POL_TCO_10_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_10_V_ACTIVE_LOW (1)
#define TCU_POL_TCO_11_ACTIVE_HIGH (0<<11)
#define TCU_POL_TCO_11_ACTIVE_LOW  (1<<11)
#define TCU_POL_TCO_11_V_ACTIVE_HIGH (0)
#define TCU_POL_TCO_11_V_ACTIVE_LOW (1)
#define TCU_WRITE_ERROR            (1<<28)
#define TCU_DEBUG_ACTIVE_NORMAL    (0<<31)
#define TCU_DEBUG_ACTIVE_DEBUG     (1<<31)
#define TCU_DEBUG_ACTIVE_V_NORMAL  (0)
#define TCU_DEBUG_ACTIVE_V_DEBUG   (1)
#define TCU_TCO_POLARITY(n)        (((n)&0xFFF)<<0)
#define TCU_TCO_POLARITY_MASK      (0xFFF<<0)
#define TCU_TCO_POLARITY_SHIFT     (0)

//Disable_Event
#define TCU_DISABLE_TCO_0          (1<<0)
#define TCU_DISABLE_TCO_1          (1<<1)
#define TCU_DISABLE_TCO_2          (1<<2)
#define TCU_DISABLE_TCO_3          (1<<3)
#define TCU_DISABLE_TCO_4          (1<<4)
#define TCU_DISABLE_TCO_5          (1<<5)
#define TCU_DISABLE_TCO_GMSK_ON    (1<<6)
#define TCU_DISABLE_TCO_TX_OEN     (1<<7)
#define TCU_DISABLE_TCO_TX_ON      (1<<8)
#define TCU_DISABLE_TCO_RX_ON      (1<<9)
#define TCU_DISABLE_TCO_RX_DEC_ON  (1<<10)
#define TCU_DISABLE_TCO_PDN        (1<<11)
#define TCU_DISABLE_SEND_SPI_CMD   (1<<12)
#define TCU_DISABLE_NEXT_GAIN      (1<<13)
#define TCU_DISABLE_FIRST_GAIN     (1<<14)
#define TCU_DISABLE_NEXT_FC        (1<<15)
#define TCU_DISABLE_RAMP_0         (1<<16)
#define TCU_DISABLE_RAMP_1         (1<<17)
#define TCU_DISABLE_RAMP_2         (1<<18)
#define TCU_DISABLE_RAMP_3         (1<<19)
#define TCU_DISABLE_RAMP_4         (1<<20)
#define TCU_DISABLE_RX_SOC         (1<<21)
#define TCU_DISABLE_DIGRF_STROBE   (1<<22)
#define TCU_DISABLE_BCPU_IRQ_0     (1<<23)
#define TCU_DISABLE_BCPU_IRQ_1     (1<<24)
#define TCU_DISABLE_XCPU_IRQ_0     (1<<25)
#define TCU_DISABLE_XCPU_IRQ_1     (1<<26)
#define TCU_DISABLE_RFSPI_START    (1<<28)
#define TCU_DISABLE_RFSPI_END      (1<<29)
#define TCU_DISABLE_RF_SPI_MARKED_CMD (1<<31)
#define TCU_DISABLE_TCO(n)         (((n)&0x3F)<<0)
#define TCU_DISABLE_TCO_MASK       (0x3F<<0)
#define TCU_DISABLE_TCO_SHIFT      (0)
#define TCU_DISABLE_INTERNAL_TCO(n) (((n)&0x3F)<<6)
#define TCU_DISABLE_INTERNAL_TCO_MASK (0x3F<<6)
#define TCU_DISABLE_INTERNAL_TCO_SHIFT (6)
#define TCU_DISABLE_RAMP(n)        (((n)&31)<<16)
#define TCU_DISABLE_RAMP_MASK      (31<<16)
#define TCU_DISABLE_RAMP_SHIFT     (16)
#define TCU_DISABLE_BCPU_IRQ(n)    (((n)&3)<<23)
#define TCU_DISABLE_BCPU_IRQ_MASK  (3<<23)
#define TCU_DISABLE_BCPU_IRQ_SHIFT (23)
#define TCU_DISABLE_XCPU_IRQ(n)    (((n)&3)<<25)
#define TCU_DISABLE_XCPU_IRQ_MASK  (3<<25)
#define TCU_DISABLE_XCPU_IRQ_SHIFT (25)

//Enable_Event
#define TCU_ENABLE_TCO_0           (1<<0)
#define TCU_ENABLE_TCO_1           (1<<1)
#define TCU_ENABLE_TCO_2           (1<<2)
#define TCU_ENABLE_TCO_3           (1<<3)
#define TCU_ENABLE_TCO_4           (1<<4)
#define TCU_ENABLE_TCO_5           (1<<5)
#define TCU_ENABLE_TCO_GMSK_ON     (1<<6)
#define TCU_ENABLE_TCO_TX_OEN      (1<<7)
#define TCU_ENABLE_TCO_TX_ON       (1<<8)
#define TCU_ENABLE_TCO_RX_ON       (1<<9)
#define TCU_ENABLE_TCO_RX_DEC_ON   (1<<10)
#define TCU_ENABLE_TCO_PDN         (1<<11)
#define TCU_ENABLE_SEND_SPI_CMD    (1<<12)
#define TCU_ENABLE_NEXT_GAIN       (1<<13)
#define TCU_ENABLE_FIRST_GAIN      (1<<14)
#define TCU_ENABLE_NEXT_FC         (1<<15)
#define TCU_ENABLE_RAMP_0          (1<<16)
#define TCU_ENABLE_RAMP_1          (1<<17)
#define TCU_ENABLE_RAMP_2          (1<<18)
#define TCU_ENABLE_RAMP_3          (1<<19)
#define TCU_ENABLE_RAMP_4          (1<<20)
#define TCU_ENABLE_RX_SOC          (1<<21)
#define TCU_ENABLE_DIGRF_STROBE    (1<<22)
#define TCU_ENABLE_BCPU_IRQ_0      (1<<23)
#define TCU_ENABLE_BCPU_IRQ_1      (1<<24)
#define TCU_ENABLE_XCPU_IRQ_0      (1<<25)
#define TCU_ENABLE_XCPU_IRQ_1      (1<<26)
#define TCU_ENABLE_RFSPI_START     (1<<28)
#define TCU_ENABLE_RFSPI_END       (1<<29)
#define TCU_ENABLE_RF_SPI_MARKED_CMD (1<<31)
#define TCU_ENABLE_TCO(n)          (((n)&0x3F)<<0)
#define TCU_ENABLE_TCO_MASK        (0x3F<<0)
#define TCU_ENABLE_TCO_SHIFT       (0)
#define TCU_ENABLE_INTERNAL_TCO(n) (((n)&0x3F)<<6)
#define TCU_ENABLE_INTERNAL_TCO_MASK (0x3F<<6)
#define TCU_ENABLE_INTERNAL_TCO_SHIFT (6)
#define TCU_ENABLE_RAMP(n)         (((n)&31)<<16)
#define TCU_ENABLE_RAMP_MASK       (31<<16)
#define TCU_ENABLE_RAMP_SHIFT      (16)
#define TCU_ENABLE_BCPU_IRQ(n)     (((n)&3)<<23)
#define TCU_ENABLE_BCPU_IRQ_MASK   (3<<23)
#define TCU_ENABLE_BCPU_IRQ_SHIFT  (23)
#define TCU_ENABLE_XCPU_IRQ(n)     (((n)&3)<<25)
#define TCU_ENABLE_XCPU_IRQ_MASK   (3<<25)
#define TCU_ENABLE_XCPU_IRQ_SHIFT  (25)

//Set_TCO
#define TCU_SET_TCO_0              (1<<0)
#define TCU_SET_TCO_1              (1<<1)
#define TCU_SET_TCO_2              (1<<2)
#define TCU_SET_TCO_3              (1<<3)
#define TCU_SET_TCO_4              (1<<4)
#define TCU_SET_TCO_5              (1<<5)
#define TCU_SET_TCO_6              (1<<6)
#define TCU_SET_TCO_7              (1<<7)
#define TCU_SET_TCO_8              (1<<8)
#define TCU_SET_TCO_9              (1<<9)
#define TCU_SET_TCO_10             (1<<10)
#define TCU_SET_TCO_11             (1<<11)
#define TCU_SET_TCO(n)             (((n)&0xFFF)<<0)
#define TCU_SET_TCO_MASK           (0xFFF<<0)
#define TCU_SET_TCO_SHIFT          (0)

//Clr_TCO
#define TCU_CLR_TCO_0              (1<<0)
#define TCU_CLR_TCO_1              (1<<1)
#define TCU_CLR_TCO_2              (1<<2)
#define TCU_CLR_TCO_3              (1<<3)
#define TCU_CLR_TCO_4              (1<<4)
#define TCU_CLR_TCO_5              (1<<5)
#define TCU_CLR_TCO_6              (1<<6)
#define TCU_CLR_TCO_7              (1<<7)
#define TCU_CLR_TCO_8              (1<<8)
#define TCU_CLR_TCO_9              (1<<9)
#define TCU_CLR_TCO_10             (1<<10)
#define TCU_CLR_TCO_11             (1<<11)
#define TCU_CLR_TCO(n)             (((n)&0xFFF)<<0)
#define TCU_CLR_TCO_MASK           (0xFFF<<0)
#define TCU_CLR_TCO_SHIFT          (0)

//Cfg_Clk_Div
#define TCU_TCU_CLK_SAME_SYS_DISABLED (0<<29)
#define TCU_TCU_CLK_SAME_SYS_ENABLED (1<<29)
#define TCU_TCU_CLK_SAME_SYS_V_DISABLED (0)
#define TCU_TCU_CLK_SAME_SYS_V_ENABLED (1)
#define TCU_ENABLE_DAI_SIMPLE_208K_DISABLED (0<<30)
#define TCU_ENABLE_DAI_SIMPLE_208K_ENABLED (1<<30)
#define TCU_ENABLE_DAI_SIMPLE_208K_V_DISABLED (0)
#define TCU_ENABLE_DAI_SIMPLE_208K_V_ENABLED (1)
#define TCU_ENABLE_QBIT_DISABLED   (0<<31)
#define TCU_ENABLE_QBIT_ENABLED    (1<<31)
#define TCU_ENABLE_QBIT_V_DISABLED (0)
#define TCU_ENABLE_QBIT_V_ENABLED  (1)

//TCU_IRQ
#define TCU_TCU_SYNC_DONE_CAUSE    (1<<0)
#define TCU_TCU_SYNC_DONE_STATUS   (1<<16)
#define TCU_TCU_IRQ_CAUSE          (1<<0)
#define TCU_TCU_IRQ_CAUSE_MASK     (1<<0)
#define TCU_TCU_IRQ_CAUSE_SHIFT    (0)
#define TCU_TCU_IRQ_STATUS         (1<<16)
#define TCU_TCU_IRQ_STATUS_MASK    (1<<16)
#define TCU_TCU_IRQ_STATUS_SHIFT   (16)

//TCU_IRQ_Mask
#define TCU_TCU_SYNC_DONE_MASK     (1<<0)
#define TCU_TCU_IRQ_MASK           (1<<0)
#define TCU_TCU_IRQ_MASK_MASK      (1<<0)
#define TCU_TCU_IRQ_MASK_SHIFT     (0)

//GLOBAL_SYNC_CTRL
#define TCU_TCU_SYNC_ENABLE        (1<<0)
#define TCU_TCU_SYNC_VALUE(n)      (((n)&0x3FFF)<<2)

//TCO_DBG_SEL
#define TCU_TCO_DBG0_SEL(n)        (((n)&15)<<0)
#define TCU_TCO_DBG1_SEL(n)        (((n)&15)<<4)

//Rfspi_Conflict_Val
#define TCU_RFSPI_CONFLICT_VAL(n)  (((n)&0x3FFF)<<0)

//Event
#define TCU_EVENT_TIME(n)          (((n)&0x3FFF)<<0)
#define TCU_EVENT_ID(n)            (((n)&0x3F)<<16)
#define TCU_EVENT_ID_CLR_TCO_0     (0x0<<16)
#define TCU_EVENT_ID_SET_TCO_0     (0x1<<16)
#define TCU_EVENT_ID_CLR_TCO_1     (0x2<<16)
#define TCU_EVENT_ID_SET_TCO_1     (0x3<<16)
#define TCU_EVENT_ID_CLR_TCO_2     (0x4<<16)
#define TCU_EVENT_ID_SET_TCO_2     (0x5<<16)
#define TCU_EVENT_ID_CLR_TCO_3     (0x6<<16)
#define TCU_EVENT_ID_SET_TCO_3     (0x7<<16)
#define TCU_EVENT_ID_CLR_TCO_4     (0x8<<16)
#define TCU_EVENT_ID_SET_TCO_4     (0x9<<16)
#define TCU_EVENT_ID_CLR_TCO_5     (0xA<<16)
#define TCU_EVENT_ID_SET_TCO_5     (0xB<<16)
#define TCU_EVENT_ID_STOP_GMSK     (0xC<<16)
#define TCU_EVENT_ID_START_GMSK    (0xD<<16)
#define TCU_EVENT_ID_HIGHZ_IQ_DAC  (0xE<<16)
#define TCU_EVENT_ID_DRIVE_IQ_DAC  (0xF<<16)
#define TCU_EVENT_ID_DISABLE_IQ_DAC (0x10<<16)
#define TCU_EVENT_ID_ENABLE_IQ_DAC (0x11<<16)
#define TCU_EVENT_ID_DISABLE_IQ_ADC (0x12<<16)
#define TCU_EVENT_ID_ENABLE_IQ_ADC (0x13<<16)
#define TCU_EVENT_ID_STOP_RFIN_RECORD (0x14<<16)
#define TCU_EVENT_ID_START_RFIN_RECORD (0x15<<16)
#define TCU_EVENT_ID_CLR_PDN       (0x16<<16)
#define TCU_EVENT_ID_SET_PDN       (0x17<<16)
#define TCU_EVENT_ID_SEND_SPI_CMD  (0x18<<16)
#define TCU_EVENT_ID_NEXT_GAIN     (0x19<<16)
#define TCU_EVENT_ID_FIRST_GAIN    (0x1A<<16)
#define TCU_EVENT_ID_NEXT_FC       (0x1B<<16)
#define TCU_EVENT_ID_PA_RAMP0      (0x1C<<16)
#define TCU_EVENT_ID_PA_RAMP1      (0x1D<<16)
#define TCU_EVENT_ID_PA_RAMP2      (0x1E<<16)
#define TCU_EVENT_ID_PA_RAMP3      (0x1F<<16)
#define TCU_EVENT_ID_PA_RAMP4      (0x20<<16)
#define TCU_EVENT_ID_RX_SOC        (0x21<<16)
#define TCU_EVENT_ID_DIGRF_STB     (0x22<<16)
#define TCU_EVENT_ID_BCPU_TCU_IRQ0 (0x23<<16)
#define TCU_EVENT_ID_BCPU_TCU_IRQ1 (0x24<<16)
#define TCU_EVENT_ID_XCPU_TCU_IRQ0 (0x25<<16)
#define TCU_EVENT_ID_XCPU_TCU_IRQ1 (0x26<<16)
#define TCU_EVENT_ID_WAKEUP_DONE   (0x27<<16)
#define TCU_EVENT_ID_RFSPI_START   (0x28<<16)
#define TCU_EVENT_ID_RFSPI_END     (0x29<<16)
#define TCU_EVENT_ID_NO_EVENT      (0x3F<<16)
#define TCU_EVENT_ID_V_CLR_TCO_0   (0x0)
#define TCU_EVENT_ID_V_SET_TCO_0   (0x1)
#define TCU_EVENT_ID_V_CLR_TCO_1   (0x2)
#define TCU_EVENT_ID_V_SET_TCO_1   (0x3)
#define TCU_EVENT_ID_V_CLR_TCO_2   (0x4)
#define TCU_EVENT_ID_V_SET_TCO_2   (0x5)
#define TCU_EVENT_ID_V_CLR_TCO_3   (0x6)
#define TCU_EVENT_ID_V_SET_TCO_3   (0x7)
#define TCU_EVENT_ID_V_CLR_TCO_4   (0x8)
#define TCU_EVENT_ID_V_SET_TCO_4   (0x9)
#define TCU_EVENT_ID_V_CLR_TCO_5   (0xA)
#define TCU_EVENT_ID_V_SET_TCO_5   (0xB)
#define TCU_EVENT_ID_V_STOP_GMSK   (0xC)
#define TCU_EVENT_ID_V_START_GMSK  (0xD)
#define TCU_EVENT_ID_V_HIGHZ_IQ_DAC (0xE)
#define TCU_EVENT_ID_V_DRIVE_IQ_DAC (0xF)
#define TCU_EVENT_ID_V_DISABLE_IQ_DAC (0x10)
#define TCU_EVENT_ID_V_ENABLE_IQ_DAC (0x11)
#define TCU_EVENT_ID_V_DISABLE_IQ_ADC (0x12)
#define TCU_EVENT_ID_V_ENABLE_IQ_ADC (0x13)
#define TCU_EVENT_ID_V_STOP_RFIN_RECORD (0x14)
#define TCU_EVENT_ID_V_START_RFIN_RECORD (0x15)
#define TCU_EVENT_ID_V_CLR_PDN     (0x16)
#define TCU_EVENT_ID_V_SET_PDN     (0x17)
#define TCU_EVENT_ID_V_SEND_SPI_CMD (0x18)
#define TCU_EVENT_ID_V_NEXT_GAIN   (0x19)
#define TCU_EVENT_ID_V_FIRST_GAIN  (0x1A)
#define TCU_EVENT_ID_V_NEXT_FC     (0x1B)
#define TCU_EVENT_ID_V_PA_RAMP0    (0x1C)
#define TCU_EVENT_ID_V_PA_RAMP1    (0x1D)
#define TCU_EVENT_ID_V_PA_RAMP2    (0x1E)
#define TCU_EVENT_ID_V_PA_RAMP3    (0x1F)
#define TCU_EVENT_ID_V_PA_RAMP4    (0x20)
#define TCU_EVENT_ID_V_RX_SOC      (0x21)
#define TCU_EVENT_ID_V_DIGRF_STB   (0x22)
#define TCU_EVENT_ID_V_BCPU_TCU_IRQ0 (0x23)
#define TCU_EVENT_ID_V_BCPU_TCU_IRQ1 (0x24)
#define TCU_EVENT_ID_V_XCPU_TCU_IRQ0 (0x25)
#define TCU_EVENT_ID_V_XCPU_TCU_IRQ1 (0x26)
#define TCU_EVENT_ID_V_WAKEUP_DONE (0x27)
#define TCU_EVENT_ID_V_RFSPI_START (0x28)
#define TCU_EVENT_ID_V_RFSPI_END   (0x29)
#define TCU_EVENT_ID_V_NO_EVENT    (0x3F)





#endif

