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


#ifndef _CP_IDLE_H_
#define _CP_IDLE_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'cp_idle'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CP_IDLE_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_IDLE_BASE              0x05090000
#else
#define REG_IDLE_BASE              0x50090000
#endif

typedef volatile struct
{
    REG32                          IDL_CTRL_SYS1;                //0x00000000
    REG32                          IDL_CTRL_SYS2;                //0x00000004
    REG32                          IDL_EN;                       //0x00000008
    REG32                          IDL_M_TIMER;                  //0x0000000C
    REG32                          IDL_WCN_EN;                   //0x00000010
    REG32                          IDL_CTRL_TIMER;               //0x00000014
    REG32                          IDL_M2_SYS;                   //0x00000018
    REG32                          IDL_TC_START;                 //0x0000001C
    REG32                          IDL_TC_END;                   //0x00000020
    REG32                          IDL_AWK_TIMER;                //0x00000024
    REG32                          GSM_LP_PU_DONE;               //0x00000028
    REG32                          GSM_FRAME_INTEN;              //0x0000002C
    REG32                          GSM_FRAME_INT_STA;            //0x00000030
    REG32                          LTEM1_FRAME_INTEN;            //0x00000034
    REG32                          LTEM1_FRAME_INT_STA;          //0x00000038
    REG32                          LTEM2_FRAME_INTEN;            //0x0000003C
    REG32                          LTEM2_FRAME_INT_STA;          //0x00000040
    REG32                          IDL_STA;                      //0x00000044
    REG32                          IDL_H_CTRL;                   //0x00000048
    REG32                          IDL_H_VAL;                    //0x0000004C
    REG32                          IDL_H_GSM;                    //0x00000050
    REG32                          IDL_H_LTEM;                   //0x00000054
    REG32                          IDL_AWK_EN;                   //0x00000058
    REG32                          IDL_AWK_ST;                   //0x0000005C
    REG32                          IDL_AWK_SELF;                 //0x00000060
    REG32                          IDL_OSW1_EN;                  //0x00000064
    REG32                          IDL_OSW1_CONT;                //0x00000068
    REG32                          IDL_FN_GSM;                   //0x0000006C
    REG32                          IDL_FN_LTEM1;                 //0x00000070
    REG32                          IDL_FN_LTEM2;                 //0x00000074
    REG32                          IDL_LTEM_RFL;                 //0x00000078
    REG32                          IDL_LTEM_SFL;                 //0x0000007C
    REG32                          IDL_SIG_EN;                   //0x00000080
    REG32                          IDL_SIG_TIMER;                //0x00000084
    REG32                          IDL_32K_REF;                  //0x00000088
    REG32                          CP_INTEN;                     //0x0000008C
    REG32                          CP_INTEN_SET;                 //0x00000090
    REG32                          CP_INTEN_CLR;                 //0x00000094
    REG32                          CP_INT_STA;                   //0x00000098
    REG32                          AP_INTEN;                     //0x0000009C
    REG32                          AP_INTEN_SET;                 //0x000000A0
    REG32                          AP_INTEN_CLR;                 //0x000000A4
    REG32                          AP_INT_STA;                   //0x000000A8
    REG32                          LTEM1_CFSR_HFN;               //0x000000AC
    REG32                          LTEM1_CFSR_FN;                //0x000000B0
    REG32                          LTEM1_CFSRS;                  //0x000000B4
    REG32                          LTEM1_CFSR_RDH;               //0x000000B8
    REG32                          LTEM1_CFSR_RDL;               //0x000000BC
    REG32                          LTEM1_FRAMC;                  //0x000000C0
    REG32                          LTEM1_FRAML;                  //0x000000C4
    REG32                          LTEM1_FRAMLS;                 //0x000000C8
    REG32                          LTEM1_CFSR_TPH;               //0x000000CC
    REG32                          LTEM1_CFSR_TPL;               //0x000000D0
    REG32                          LTEM1_FRAMC_TP;               //0x000000D4
    REG32                          LTEM2_CFSR_HFN;               //0x000000D8
    REG32                          LTEM2_CFSR_FN;                //0x000000DC
    REG32                          LTEM2_CFSRS;                  //0x000000E0
    REG32                          LTEM2_CFSR_RDH;               //0x000000E4
    REG32                          LTEM2_CFSR_RDL;               //0x000000E8
    REG32                          LTEM2_FRAMC;                  //0x000000EC
    REG32                          LTEM2_FRAML;                  //0x000000F0
    REG32                          LTEM2_FRAMLS;                 //0x000000F4
    REG32                          LTEM2_CFSR_TPH;               //0x000000F8
    REG32                          LTEM2_CFSR_TPL;               //0x000000FC
    REG32                          LTEM2_FRAMC_TP;               //0x00000100
    REG32                          GSM_CFSR;                     //0x00000104
    REG32                          GSM_CFSRS;                    //0x00000108
    REG32                          GSM_CFSRO;                    //0x0000010C
    REG32                          LTEM1_FHL;                    //0x00000110
    REG32                          LTEM1_FLL;                    //0x00000114
    REG32                          LTEM1_FCL;                    //0x00000118
    REG32                          LTEM2_FHL;                    //0x0000011C
    REG32                          LTEM2_FLL;                    //0x00000120
    REG32                          LTEM2_FCL;                    //0x00000124
    REG32                          GSM_FL;                       //0x00000128
    REG32                          GSM_FCL;                      //0x0000012C
    REG32                          TPCTRL;                       //0x00000130
    REG32                          LAYOUTT;                      //0x00000134
    REG32                          LAYOUTCTRL;                   //0x00000138
    REG32                          LTEM1_FINT_DLY1;              //0x0000013C
    REG32                          LTEM1_FINT_DLY2;              //0x00000140
    REG32                          LTEM2_FINT_DLY1;              //0x00000144
    REG32                          LTEM2_FINT_DLY2;              //0x00000148
    REG32                          FINT_EN;                      //0x0000014C
    REG32                          TIMER_EN;                     //0x00000150
    REG32                          IDLE_FRAME_STA;               //0x00000154
    REG32                          IDLE_FRAME_LTEM1;             //0x00000158
    REG32                          IDLE_FRAME_LTEM2;             //0x0000015C
    REG32                          IDLE_FRAME_GSM;               //0x00000160
    REG32                          IDLE_FRAME_LTE;               //0x00000164
    REG32                          IDLE_FRAME_LTE_CONF;          //0x00000168
    REG32                          LTEM_REF_FN;                  //0x0000016C
    REG32                          LTEM_REF_FNL;                 //0x00000170
    REG32                          LTEM_REF_FCL;                 //0x00000174
    REG32                          REF_32K_FNL;                  //0x00000178
    REG32                          LTEM_REF_FC;                  //0x0000017C
    REG32                          GSM_FRAML;                    //0x00000180
    REG32                          IDL_OSW2_EN;                  //0x00000184
    REG32                          IDL_OSW2_CONT;                //0x00000188
    REG32                          IDLE_FRAMC_GSM;               //0x0000018C
    REG32                          LTEM1_FINT_DLY3;              //0x00000190
    REG32                          LTEM2_FINT_DLY3;              //0x00000194
    REG32                          IDLE_TIME_SEL;                //0x00000198
    REG32                          IDLE_TIME;                    //0x0000019C
    REG32                          IDL_H_GSM_LP;                 //0x000001A0
    REG32                          IDL_H_LTEM_LP;                //0x000001A4
    REG32                          IDL_TC_START_NB;              //0x000001A8
    REG32                          IDL_TC_END_NB;                //0x000001AC
    REG32                          NB_LP_PU_DONE;                //0x000001B0
    REG32                          IDL_H_NB;                     //0x000001B4
    REG32                          IDL_H_NB_LP;                  //0x000001B8
    REG32                          IDL_FN_NB;                    //0x000001BC
    REG32                          NB_FRAME_INTEN;               //0x000001C0
    REG32                          NB_FRAME_INT_STA;             //0x000001C4
    REG32                          NB_CFSR;                      //0x000001C8
    REG32                          NB_FRAML;                     //0x000001CC
    REG32                          NB_CFSRS;                     //0x000001D0
    REG32                          NB_CFSRO;                     //0x000001D4
    REG32                          NB_FL;                        //0x000001D8
    REG32                          NB_FCL;                       //0x000001DC
    REG32                          IDLE_FRAME_NB;                //0x000001E0
    REG32                          IDLE_FRAMC_NB;                //0x000001E4
    REG32                          IDL_AWK_EN_SET;               //0x000001E8
    REG32                          IDL_AWK_EN_CLR;               //0x000001EC
    REG32                          GSM_FRAMC;                    //0x000001F0
    REG32                          NB_FRAMC;                     //0x000001F4
    REG32                          ELIMINAT_JITTER;              //0x000001F8
    REG32                          GSM_EN_SEL;                   //0x000001FC
    REG32                          NB_EN_SEL;                    //0x00000200
    REG32                          PD_PLL_SW;                    //0x00000204
    REG32                          PD_PLL_SW_SET;                //0x00000208
    REG32                          PD_PLL_SW_CLR;                //0x0000020C
    REG32                          PD_PLL_SEL;                   //0x00000210
    REG32                          PD_PLL_SEL_SET;               //0x00000214
    REG32                          PD_PLL_SEL_CLR;               //0x00000218
    REG32                          IDLE_CG_SW;                   //0x0000021C
    REG32                          IDLE_CG_SW_SET;               //0x00000220
    REG32                          IDLE_CG_SW_CLR;               //0x00000224
    REG32                          IDLE_CG_SEL;                  //0x00000228
    REG32                          IDLE_CG_SEL_SET;              //0x0000022C
    REG32                          IDLE_CG_SEL_CLR;              //0x00000230
    REG32                          RF_IDLE_ENABLE_SW;            //0x00000234
    REG32                          RF_IDLE_ENABLE_SEL;           //0x00000238
    REG32 Reserved_0000023C[8];                 //0x0000023C
    REG32                          MEM_EMA_CFG;                  //0x0000025C
    REG32                          UART_CTRL;                    //0x00000260
    REG32                          DDR_LATCH;                    //0x00000264
    REG32                          PAD_CTRL;                     //0x00000268
    REG32 Reserved_0000026C[4];                 //0x0000026C
    REG32                          PAD_CTRL_UART_TXD;            //0x0000027C
    REG32                          MON_SEL;                      //0x00000280
    REG32                          MON_SEL_SET;                  //0x00000284
    REG32                          MON_SEL_CLR;                  //0x00000288
    REG32                          TARGET_TIMER;                 //0x0000028C
    REG32                          TARGET_TIMER_EN;              //0x00000290
    REG32                          TARGET_VALUE_LOCK;            //0x00000294
    REG32                          TARGET_TIMER_STAT;            //0x00000298
    REG32                          SLOW_SYS_CLK_SEL_HWEN;        //0x0000029C
    REG32                          SLOW_CLK_SEL_HWEN;            //0x000002A0
    REG32                          SLEEP_PROT_TIME;              //0x000002A4
    REG32 Reserved_000002A8[3926];              //0x000002A8
    REG32                          IDLE_RES0;                    //0x00004000
    REG32                          IDLE_RES1;                    //0x00004004
    REG32                          IDLE_RES2;                    //0x00004008
    REG32                          IDLE_RES3;                    //0x0000400C
    REG32                          IDLE_RES4;                    //0x00004010
    REG32                          IDLE_RES5;                    //0x00004014
    REG32                          IDLE_RES6;                    //0x00004018
    REG32                          IDLE_RES7;                    //0x0000401C
    REG32                          IDLE_RES8;                    //0x00004020
    REG32                          IDLE_RES9;                    //0x00004024
    REG32                          IDLE_RES10;                   //0x00004028
    REG32                          IDLE_RES11;                   //0x0000402C
} HWP_CP_IDLE_T;

#define hwp_idle                   ((HWP_CP_IDLE_T*) REG_ACCESS_ADDRESS(REG_IDLE_BASE))


//IDL_CTRL_SYS1
typedef union {
    REG32 v;
    struct {
        REG32 idct_ctrl_sys1 : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_IDL_CTRL_SYS1_T;

//IDL_CTRL_SYS2
typedef union {
    REG32 v;
    struct {
        REG32 idct_ctrl_sys2 : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_IDL_CTRL_SYS2_T;

//IDL_EN
typedef union {
    REG32 v;
    struct {
        REG32 idl_cp_en : 1; // [0]
        REG32 idl_ap_en : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_CP_IDLE_IDL_EN_T;

//IDL_WCN_EN
typedef union {
    REG32 v;
    struct {
        REG32 wcn_chip_pd : 1; // [0]
        REG32 wcn_pd_xtal : 1; // [1]
        REG32 wcn_pd_pll : 1; // [2]
        REG32 wcn_idle_cg : 1; // [3]
        REG32 wcn_res_val : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_CP_IDLE_IDL_WCN_EN_T;

//IDL_CTRL_TIMER
typedef union {
    REG32 v;
    struct {
        REG32 idct_ctrl_timer : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_IDL_CTRL_TIMER_T;

//IDL_M2_SYS
typedef union {
    REG32 v;
    struct {
        REG32 m2_sys : 16; // [15:0]
        REG32 m1_sys : 16; // [31:16]
    } b;
} REG_CP_IDLE_IDL_M2_SYS_T;

//IDL_TC_START
typedef union {
    REG32 v;
    struct {
        REG32 tc_start_mod : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_CP_IDLE_IDL_TC_START_T;

//IDL_TC_END
typedef union {
    REG32 v;
    struct {
        REG32 tc_end_mod : 2; // [1:0]
        REG32 __3_2 : 2;
        REG32 tc_end_framc : 17; // [20:4]
        REG32 __31_21 : 11;
    } b;
} REG_CP_IDLE_IDL_TC_END_T;

//IDL_AWK_TIMER
typedef union {
    REG32 v;
    struct {
        REG32 wake_timer : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_IDL_AWK_TIMER_T;

//GSM_LP_PU_DONE
typedef union {
    REG32 v;
    struct {
        REG32 lp_pu_done : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_GSM_LP_PU_DONE_T;

//GSM_FRAME_INTEN
typedef union {
    REG32 v;
    struct {
        REG32 gsm_frame_irq_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_GSM_FRAME_INTEN_T;

//GSM_FRAME_INT_STA
typedef union {
    REG32 v;
    struct {
        REG32 gsm_frame_int_sta : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_GSM_FRAME_INT_STA_T;

//LTEM1_FRAME_INTEN
typedef union {
    REG32 v;
    struct {
        REG32 ltem1_frame1_irq_en : 1; // [0]
        REG32 ltem1_frame2_irq_en : 1; // [1]
        REG32 ltem1_frame3_irq_en : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_CP_IDLE_LTEM1_FRAME_INTEN_T;

//LTEM1_FRAME_INT_STA
typedef union {
    REG32 v;
    struct {
        REG32 ltem1_frame_int_sta : 3; // [2:0], write clear
        REG32 __31_3 : 29;
    } b;
} REG_CP_IDLE_LTEM1_FRAME_INT_STA_T;

//LTEM2_FRAME_INTEN
typedef union {
    REG32 v;
    struct {
        REG32 ltem2_frame1_irq_en : 1; // [0]
        REG32 ltem2_frame2_irq_en : 1; // [1]
        REG32 ltem2_frame3_irq_en : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_CP_IDLE_LTEM2_FRAME_INTEN_T;

//LTEM2_FRAME_INT_STA
typedef union {
    REG32 v;
    struct {
        REG32 ltem2_frame_int_sta : 3; // [2:0], write clear
        REG32 __31_3 : 29;
    } b;
} REG_CP_IDLE_LTEM2_FRAME_INT_STA_T;

//IDL_STA
typedef union {
    REG32 v;
    struct {
        REG32 idle_sys_stat : 1; // [0], read only
        REG32 idle_gsm_timer_stat : 1; // [1], read only
        REG32 idle_ltem1_timer_stat : 1; // [2], read only
        REG32 idle_ltem2_timer_stat : 1; // [3], read only
        REG32 h_stat : 1; // [4], read only
        REG32 idle_nb_timer_stat : 1; // [5], read only
        REG32 __31_6 : 26;
    } b;
} REG_CP_IDLE_IDL_STA_T;

//IDL_H_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 reserved2_1 : 1; // [0], read only
        REG32 h_ctrl_en : 1; // [1]
        REG32 h_auto_en : 1; // [2]
        REG32 h_run_time : 4; // [6:3]
        REG32 reserved1_0 : 25; // [31:7], read only
    } b;
} REG_CP_IDLE_IDL_H_CTRL_T;

//IDL_H_VAL
typedef union {
    REG32 v;
    struct {
        REG32 h_value : 27; // [26:0]
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_H_VAL_T;

//IDL_H_GSM
typedef union {
    REG32 v;
    struct {
        REG32 h_value : 27; // [26:0]
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_H_GSM_T;

//IDL_H_LTEM
typedef union {
    REG32 v;
    struct {
        REG32 h_value : 27; // [26:0]
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_H_LTEM_T;

//IDL_AWK_EN
typedef union {
    REG32 v;
    struct {
        REG32 awk0_en : 1; // [0]
        REG32 awk1_en : 1; // [1]
        REG32 awk2_en : 1; // [2]
        REG32 awk3_en : 1; // [3]
        REG32 awk4_en : 1; // [4]
        REG32 awk5_en : 1; // [5]
        REG32 awk6_en : 1; // [6]
        REG32 awk7_en : 1; // [7]
        REG32 awk_osw1_en : 1; // [8]
        REG32 awk_osw2_en : 1; // [9]
        REG32 awk_self_en : 1; // [10]
        REG32 gsm_lp_pu_reach_en : 1; // [11]
        REG32 nb_lp_pu_reach_en : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_CP_IDLE_IDL_AWK_EN_T;

//IDL_AWK_ST
typedef union {
    REG32 v;
    struct {
        REG32 awk0_awk_stat : 1; // [0], write clear
        REG32 awk1_awk_stat : 1; // [1], write clear
        REG32 awk2_awk_stat : 1; // [2], write clear
        REG32 awk3_awk_stat : 1; // [3], write clear
        REG32 awk4_awk_stat : 1; // [4], write clear
        REG32 awk5_awk_stat : 1; // [5], write clear
        REG32 awk6_awk_stat : 1; // [6], write clear
        REG32 awk7_awk_stat : 1; // [7], write clear
        REG32 awk_osw1_stat : 1; // [8], write clear
        REG32 awk_osw2_stat : 1; // [9], write clear
        REG32 awk_self_stat : 1; // [10], write clear
        REG32 gsm_lp_pu_reach_stat : 1; // [11], write clear
        REG32 nb_lp_pu_reach_stat : 1; // [12], write clear
        REG32 __15_13 : 3;
        REG32 awk_up_stat : 1; // [16], write clear
        REG32 __19_17 : 3;
        REG32 idle_stat : 1; // [20], write clear
        REG32 __23_21 : 3;
        REG32 pow_sta : 1; // [24], write clear
        REG32 thr_sta : 1; // [25], write clear
        REG32 pow_dfe_sta : 1; // [26], read only
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_AWK_ST_T;

//IDL_AWK_SELF
typedef union {
    REG32 v;
    struct {
        REG32 wake_self : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_IDL_AWK_SELF_T;

//IDL_OSW1_EN
typedef union {
    REG32 v;
    struct {
        REG32 osw1_time : 31; // [30:0]
        REG32 osw1_en : 1; // [31]
    } b;
} REG_CP_IDLE_IDL_OSW1_EN_T;

//IDL_FN_LTEM1
typedef union {
    REG32 v;
    struct {
        REG32 idfn_sub_ltem : 4; // [3:0], read only
        REG32 idfn_rad_ltem : 28; // [31:4], read only
    } b;
} REG_CP_IDLE_IDL_FN_LTEM1_T;

//IDL_FN_LTEM2
typedef union {
    REG32 v;
    struct {
        REG32 idfn_sub_ltem : 4; // [3:0], read only
        REG32 idfn_rad_ltem : 28; // [31:4], read only
    } b;
} REG_CP_IDLE_IDL_FN_LTEM2_T;

//IDL_LTEM_RFL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_idle_radioframe_parameter : 21; // [20:0]
        REG32 __31_21 : 11;
    } b;
} REG_CP_IDLE_IDL_LTEM_RFL_T;

//IDL_LTEM_SFL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_idle_frame_parameter : 17; // [16:0]
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_IDL_LTEM_SFL_T;

//IDL_SIG_EN
typedef union {
    REG32 v;
    struct {
        REG32 chip_pd_en : 1; // [0]
        REG32 pd_xtal_en : 1; // [1]
        REG32 pd_pll_en : 1; // [2]
        REG32 idle_cg_en : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_CP_IDLE_IDL_SIG_EN_T;

//IDL_SIG_TIMER
typedef union {
    REG32 v;
    struct {
        REG32 t1 : 8; // [7:0]
        REG32 t2 : 8; // [15:8]
        REG32 t3 : 8; // [23:16]
        REG32 t4 : 8; // [31:24]
    } b;
} REG_CP_IDLE_IDL_SIG_TIMER_T;

//CP_INTEN
typedef union {
    REG32 v;
    struct {
        REG32 layout_irq : 1; // [0]
        REG32 idle_h_irq : 1; // [1]
        REG32 idle_frame_irq : 1; // [2]
        REG32 tstamp_irq : 1; // [3]
        REG32 osw1_irq : 1; // [4]
        REG32 gsm_tc_start_irq : 1; // [5]
        REG32 gsm_tc_end_irq : 1; // [6]
        REG32 gsm_pu_reach_irq : 1; // [7]
        REG32 timer_awk_irq : 1; // [8]
        REG32 sys_wak_irq : 1; // [9]
        REG32 nb_tc_start_irq : 1; // [10]
        REG32 nb_tc_end_irq : 1; // [11]
        REG32 nb_pu_reach_irq : 1; // [12]
        REG32 target_irq : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_CP_INTEN_T;

//CP_INTEN_SET
typedef union {
    REG32 v;
    struct {
        REG32 int_en_set : 14; // [13:0], write set
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_CP_INTEN_SET_T;

//CP_INTEN_CLR
typedef union {
    REG32 v;
    struct {
        REG32 int_en_clr : 14; // [13:0], write clear
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_CP_INTEN_CLR_T;

//CP_INT_STA
typedef union {
    REG32 v;
    struct {
        REG32 cp_int_sta : 14; // [13:0], write clear
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_CP_INT_STA_T;

//AP_INTEN
typedef union {
    REG32 v;
    struct {
        REG32 osw2_irq : 1; // [0]
        REG32 gsm_pu_reach_irq : 1; // [1]
        REG32 timer_awk_irq : 1; // [2]
        REG32 sys_wak_irq : 1; // [3]
        REG32 nb_pu_reach_irq : 1; // [4]
        REG32 target_irq : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_CP_IDLE_AP_INTEN_T;

//AP_INTEN_SET
typedef union {
    REG32 v;
    struct {
        REG32 int_en_set : 6; // [5:0], write set
        REG32 __31_6 : 26;
    } b;
} REG_CP_IDLE_AP_INTEN_SET_T;

//AP_INTEN_CLR
typedef union {
    REG32 v;
    struct {
        REG32 int_en_clr : 6; // [5:0], write clear
        REG32 __31_6 : 26;
    } b;
} REG_CP_IDLE_AP_INTEN_CLR_T;

//AP_INT_STA
typedef union {
    REG32 v;
    struct {
        REG32 ap_int_sta : 6; // [5:0], write clear
        REG32 __31_6 : 26;
    } b;
} REG_CP_IDLE_AP_INT_STA_T;

//LTEM1_CFSR_HFN
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_hfn : 22; // [21:0]
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM1_CFSR_HFN_T;

//LTEM1_CFSR_FN
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_sub : 4; // [3:0]
        REG32 ltem_cfsr_rad : 10; // [13:4]
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM1_CFSR_FN_T;

//LTEM1_CFSRS
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsrs : 24; // [23:0]
        REG32 adjust_direct : 1; // [24]
        REG32 active_time : 1; // [25]
        REG32 __31_26 : 6;
    } b;
} REG_CP_IDLE_LTEM1_CFSRS_T;

//LTEM1_CFSR_RDH
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_rdh : 22; // [21:0], read only
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM1_CFSR_RDH_T;

//LTEM1_CFSR_RDL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_rdl_sub : 4; // [3:0], read only
        REG32 ltem_cfsr_rdl_rad : 10; // [13:4], read only
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM1_CFSR_RDL_T;

//LTEM1_FRAMC
typedef union {
    REG32 v;
    struct {
        REG32 lframc : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM1_FRAMC_T;

//LTEM1_FRAML
typedef union {
    REG32 v;
    struct {
        REG32 lframl : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM1_FRAML_T;

//LTEM1_FRAMLS
typedef union {
    REG32 v;
    struct {
        REG32 lframls : 16; // [15:0]
        REG32 active_time : 1; // [16]
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_LTEM1_FRAMLS_T;

//LTEM1_CFSR_TPH
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_tph : 22; // [21:0]
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM1_CFSR_TPH_T;

//LTEM1_CFSR_TPL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_tpl : 14; // [13:0]
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM1_CFSR_TPL_T;

//LTEM1_FRAMC_TP
typedef union {
    REG32 v;
    struct {
        REG32 ltem_framc_tp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM1_FRAMC_TP_T;

//LTEM2_CFSR_HFN
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_hfn : 22; // [21:0]
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM2_CFSR_HFN_T;

//LTEM2_CFSR_FN
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_sub : 4; // [3:0]
        REG32 ltem_cfsr_rad : 10; // [13:4]
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM2_CFSR_FN_T;

//LTEM2_CFSRS
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsrs : 24; // [23:0]
        REG32 adjust_direct : 1; // [24]
        REG32 active_time : 1; // [25]
        REG32 __31_26 : 6;
    } b;
} REG_CP_IDLE_LTEM2_CFSRS_T;

//LTEM2_CFSR_RDH
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_rdh : 22; // [21:0], read only
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM2_CFSR_RDH_T;

//LTEM2_CFSR_RDL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_rdl_sub : 4; // [3:0], read only
        REG32 ltem_cfsr_rdl_rad : 10; // [13:4], read only
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM2_CFSR_RDL_T;

//LTEM2_FRAMC
typedef union {
    REG32 v;
    struct {
        REG32 lframc : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM2_FRAMC_T;

//LTEM2_FRAML
typedef union {
    REG32 v;
    struct {
        REG32 lframl : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM2_FRAML_T;

//LTEM2_FRAMLS
typedef union {
    REG32 v;
    struct {
        REG32 lframls : 16; // [15:0]
        REG32 active_time : 1; // [16]
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_LTEM2_FRAMLS_T;

//LTEM2_CFSR_TPH
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_tph : 22; // [21:0]
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM2_CFSR_TPH_T;

//LTEM2_CFSR_TPL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_cfsr_tpl : 14; // [13:0]
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM2_CFSR_TPL_T;

//LTEM2_FRAMC_TP
typedef union {
    REG32 v;
    struct {
        REG32 ltem_framc_tp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM2_FRAMC_TP_T;

//GSM_CFSR
typedef union {
    REG32 v;
    struct {
        REG32 gsm_cfsr_gsm_cfsr : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_CP_IDLE_GSM_CFSR_T;

//GSM_CFSRS
typedef union {
    REG32 v;
    struct {
        REG32 gsm_cfsrs_gsm_cfsrs : 24; // [23:0]
        REG32 adjust_direct : 1; // [24]
        REG32 __31_25 : 7;
    } b;
} REG_CP_IDLE_GSM_CFSRS_T;

//GSM_CFSRO
typedef union {
    REG32 v;
    struct {
        REG32 gsm_cfsr_overflow : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_CP_IDLE_GSM_CFSRO_T;

//LTEM1_FHL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_fhl : 22; // [21:0], read only
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM1_FHL_T;

//LTEM1_FLL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_fll : 14; // [13:0], read only
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM1_FLL_T;

//LTEM1_FCL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_fcl : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM1_FCL_T;

//LTEM2_FHL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_fhl : 22; // [21:0], read only
        REG32 __31_22 : 10;
    } b;
} REG_CP_IDLE_LTEM2_FHL_T;

//LTEM2_FLL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_fll : 14; // [13:0], read only
        REG32 __31_14 : 18;
    } b;
} REG_CP_IDLE_LTEM2_FLL_T;

//LTEM2_FCL
typedef union {
    REG32 v;
    struct {
        REG32 ltem_fcl : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM2_FCL_T;

//GSM_FL
typedef union {
    REG32 v;
    struct {
        REG32 fl : 24; // [23:0], read only
        REG32 __31_24 : 8;
    } b;
} REG_CP_IDLE_GSM_FL_T;

//GSM_FCL
typedef union {
    REG32 v;
    struct {
        REG32 gsm_fcl_gsm_fcl : 17; // [16:0], read only
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_GSM_FCL_T;

//TPCTRL
typedef union {
    REG32 v;
    struct {
        REG32 inner_ctrl : 1; // [0]
        REG32 reserved3_2 : 3; // [3:1], read only
        REG32 inner_confg : 2; // [5:4]
        REG32 reserved2_1 : 2; // [7:6], read only
        REG32 mod_sel : 3; // [10:8]
        REG32 reserved1_0 : 21; // [31:11], read only
    } b;
} REG_CP_IDLE_TPCTRL_T;

//LAYOUTCTRL
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0]
        REG32 timer_select : 1; // [1]
        REG32 reserved2_1 : 6; // [7:2], read only
        REG32 chip_count : 15; // [22:8]
        REG32 reserved1_0 : 9; // [31:23], read only
    } b;
} REG_CP_IDLE_LAYOUTCTRL_T;

//LTEM1_FINT_DLY1
typedef union {
    REG32 v;
    struct {
        REG32 delay_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM1_FINT_DLY1_T;

//LTEM1_FINT_DLY2
typedef union {
    REG32 v;
    struct {
        REG32 delay_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM1_FINT_DLY2_T;

//LTEM2_FINT_DLY1
typedef union {
    REG32 v;
    struct {
        REG32 delay_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM2_FINT_DLY1_T;

//LTEM2_FINT_DLY2
typedef union {
    REG32 v;
    struct {
        REG32 delay_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM2_FINT_DLY2_T;

//FINT_EN
typedef union {
    REG32 v;
    struct {
        REG32 lte_m1_fint_enable : 10; // [9:0]
        REG32 reserved2_1 : 2; // [11:10], read only
        REG32 lte_m2_fint_enable : 10; // [21:12]
        REG32 reserved1_0 : 10; // [31:22], read only
    } b;
} REG_CP_IDLE_FINT_EN_T;

//TIMER_EN
typedef union {
    REG32 v;
    struct {
        REG32 lte_m1_timer_enable : 1; // [0]
        REG32 lte_m2_timer_enable : 1; // [1]
        REG32 gsm_timer_enable : 1; // [2]
        REG32 ltem_timer_enable : 1; // [3]
        REG32 nb_timer_enable : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_CP_IDLE_TIMER_EN_T;

//IDLE_FRAME_STA
typedef union {
    REG32 v;
    struct {
        REG32 lte_m1_frame_state : 1; // [0], write clear
        REG32 lte_m2_frame_state : 1; // [1], write clear
        REG32 gsm_frame_state : 1; // [2], write clear
        REG32 lte_m_frame_state : 1; // [3], write clear
        REG32 nb_frame_state : 1; // [4], write clear
        REG32 __31_5 : 27;
    } b;
} REG_CP_IDLE_IDLE_FRAME_STA_T;

//IDLE_FRAME_LTEM1
typedef union {
    REG32 v;
    struct {
        REG32 frame_cfsr : 22; // [21:0]
        REG32 reserved2_1 : 2; // [23:22], read only
        REG32 frame_conf : 1; // [24]
        REG32 reserved1_0 : 7; // [31:25], read only
    } b;
} REG_CP_IDLE_IDLE_FRAME_LTEM1_T;

//IDLE_FRAME_LTEM2
typedef union {
    REG32 v;
    struct {
        REG32 frame_cfsr : 22; // [21:0]
        REG32 reserved2_1 : 2; // [23:22], read only
        REG32 frame_conf : 1; // [24]
        REG32 reserved1_0 : 7; // [31:25], read only
    } b;
} REG_CP_IDLE_IDLE_FRAME_LTEM2_T;

//IDLE_FRAME_GSM
typedef union {
    REG32 v;
    struct {
        REG32 frame_cfsr : 24; // [23:0]
        REG32 frame_conf : 1; // [24]
        REG32 __31_25 : 7;
    } b;
} REG_CP_IDLE_IDLE_FRAME_GSM_T;

//IDLE_FRAME_LTE_CONF
typedef union {
    REG32 v;
    struct {
        REG32 frame_ref_lte_conf : 1; // [0]
        REG32 resrved : 31; // [31:1], read only
    } b;
} REG_CP_IDLE_IDLE_FRAME_LTE_CONF_T;

//LTEM_REF_FCL
typedef union {
    REG32 v;
    struct {
        REG32 ref_ltem_fcl : 15; // [14:0], read only
        REG32 __31_15 : 17;
    } b;
} REG_CP_IDLE_LTEM_REF_FCL_T;

//LTEM_REF_FC
typedef union {
    REG32 v;
    struct {
        REG32 ltem_ref_fc_ltem_ref_fc : 15; // [14:0], read only
        REG32 __31_15 : 17;
    } b;
} REG_CP_IDLE_LTEM_REF_FC_T;

//GSM_FRAML
typedef union {
    REG32 v;
    struct {
        REG32 gsm_framl : 17; // [16:0]
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_GSM_FRAML_T;

//IDL_OSW2_EN
typedef union {
    REG32 v;
    struct {
        REG32 osw2_time : 31; // [30:0]
        REG32 osw2_en : 1; // [31]
    } b;
} REG_CP_IDLE_IDL_OSW2_EN_T;

//IDLE_FRAMC_GSM
typedef union {
    REG32 v;
    struct {
        REG32 framc_cfsr : 17; // [16:0]
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_IDLE_FRAMC_GSM_T;

//LTEM1_FINT_DLY3
typedef union {
    REG32 v;
    struct {
        REG32 delay_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM1_FINT_DLY3_T;

//LTEM2_FINT_DLY3
typedef union {
    REG32 v;
    struct {
        REG32 delay_time : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_LTEM2_FINT_DLY3_T;

//IDLE_TIME_SEL
typedef union {
    REG32 v;
    struct {
        REG32 time_sel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_IDLE_TIME_SEL_T;

//IDL_H_GSM_LP
typedef union {
    REG32 v;
    struct {
        REG32 h_value : 27; // [26:0]
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_H_GSM_LP_T;

//IDL_H_LTEM_LP
typedef union {
    REG32 v;
    struct {
        REG32 h_value : 27; // [26:0]
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_H_LTEM_LP_T;

//IDL_TC_START_NB
typedef union {
    REG32 v;
    struct {
        REG32 tc_start_mod : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_CP_IDLE_IDL_TC_START_NB_T;

//IDL_TC_END_NB
typedef union {
    REG32 v;
    struct {
        REG32 tc_end_mod : 2; // [1:0]
        REG32 __3_2 : 2;
        REG32 tc_end_framc : 17; // [20:4]
        REG32 __31_21 : 11;
    } b;
} REG_CP_IDLE_IDL_TC_END_NB_T;

//NB_LP_PU_DONE
typedef union {
    REG32 v;
    struct {
        REG32 lp_pu_done : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_NB_LP_PU_DONE_T;

//IDL_H_NB
typedef union {
    REG32 v;
    struct {
        REG32 h_value : 27; // [26:0]
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_H_NB_T;

//IDL_H_NB_LP
typedef union {
    REG32 v;
    struct {
        REG32 h_value : 27; // [26:0]
        REG32 __31_27 : 5;
    } b;
} REG_CP_IDLE_IDL_H_NB_LP_T;

//NB_FRAME_INTEN
typedef union {
    REG32 v;
    struct {
        REG32 nb_frame_irq_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_NB_FRAME_INTEN_T;

//NB_FRAME_INT_STA
typedef union {
    REG32 v;
    struct {
        REG32 gsm_frame_int_sta : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_NB_FRAME_INT_STA_T;

//NB_CFSR
typedef union {
    REG32 v;
    struct {
        REG32 gsm_cfsr : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_CP_IDLE_NB_CFSR_T;

//NB_FRAML
typedef union {
    REG32 v;
    struct {
        REG32 nb_framl : 17; // [16:0]
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_NB_FRAML_T;

//NB_CFSRS
typedef union {
    REG32 v;
    struct {
        REG32 nb_cfsrs_nb_cfsrs : 24; // [23:0]
        REG32 adjust_direct : 1; // [24]
        REG32 __31_25 : 7;
    } b;
} REG_CP_IDLE_NB_CFSRS_T;

//NB_CFSRO
typedef union {
    REG32 v;
    struct {
        REG32 nb_cfsr_overflow : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_CP_IDLE_NB_CFSRO_T;

//NB_FL
typedef union {
    REG32 v;
    struct {
        REG32 fl : 24; // [23:0], read only
        REG32 __31_24 : 8;
    } b;
} REG_CP_IDLE_NB_FL_T;

//NB_FCL
typedef union {
    REG32 v;
    struct {
        REG32 nb_fcl_nb_fcl : 17; // [16:0], read only
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_NB_FCL_T;

//IDLE_FRAME_NB
typedef union {
    REG32 v;
    struct {
        REG32 frame_cfsr : 24; // [23:0]
        REG32 frame_conf : 1; // [24]
        REG32 __31_25 : 7;
    } b;
} REG_CP_IDLE_IDLE_FRAME_NB_T;

//IDLE_FRAMC_NB
typedef union {
    REG32 v;
    struct {
        REG32 framc_cfsr : 17; // [16:0]
        REG32 __31_17 : 15;
    } b;
} REG_CP_IDLE_IDLE_FRAMC_NB_T;

//IDL_AWK_EN_SET
typedef union {
    REG32 v;
    struct {
        REG32 awk_en_set : 13; // [12:0], write set
        REG32 __31_13 : 19;
    } b;
} REG_CP_IDLE_IDL_AWK_EN_SET_T;

//IDL_AWK_EN_CLR
typedef union {
    REG32 v;
    struct {
        REG32 awk_en_clear : 13; // [12:0], write clear
        REG32 __31_13 : 19;
    } b;
} REG_CP_IDLE_IDL_AWK_EN_CLR_T;

//GSM_FRAMC
typedef union {
    REG32 v;
    struct {
        REG32 framc : 17; // [16:0], read only
        REG32 __19_17 : 3;
        REG32 rd_enable : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_CP_IDLE_GSM_FRAMC_T;

//NB_FRAMC
typedef union {
    REG32 v;
    struct {
        REG32 framc : 17; // [16:0], read only
        REG32 __19_17 : 3;
        REG32 rd_enable : 1; // [20]
        REG32 __31_21 : 11;
    } b;
} REG_CP_IDLE_NB_FRAMC_T;

//ELIMINAT_JITTER
typedef union {
    REG32 v;
    struct {
        REG32 elimiate_en : 8; // [7:0]
        REG32 eliminat_time : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_CP_IDLE_ELIMINAT_JITTER_T;

//GSM_EN_SEL
typedef union {
    REG32 v;
    struct {
        REG32 select : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_GSM_EN_SEL_T;

//NB_EN_SEL
typedef union {
    REG32 v;
    struct {
        REG32 select : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_NB_EN_SEL_T;

//PD_PLL_SW
typedef union {
    REG32 v;
    struct {
        REG32 bbpll1 : 1; // [0]
        REG32 bbpll2 : 1; // [1]
        REG32 apll : 1; // [2]
        REG32 audiopll : 1; // [3]
        REG32 usbpll : 1; // [4]
        REG32 mempll : 1; // [5]
        REG32 dsipll : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_PD_PLL_SW_T;

//PD_PLL_SW_SET
typedef union {
    REG32 v;
    struct {
        REG32 pdpllswset : 7; // [6:0], write set
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_PD_PLL_SW_SET_T;

//PD_PLL_SW_CLR
typedef union {
    REG32 v;
    struct {
        REG32 pdpllswclr : 7; // [6:0], write clear
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_PD_PLL_SW_CLR_T;

//PD_PLL_SEL
typedef union {
    REG32 v;
    struct {
        REG32 bbpll1 : 1; // [0]
        REG32 bbpll2 : 1; // [1]
        REG32 apll : 1; // [2]
        REG32 audiopll : 1; // [3]
        REG32 usbpll : 1; // [4]
        REG32 mempll : 1; // [5]
        REG32 dsipll : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_PD_PLL_SEL_T;

//PD_PLL_SEL_SET
typedef union {
    REG32 v;
    struct {
        REG32 pdpllselset : 7; // [6:0], write set
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_PD_PLL_SEL_SET_T;

//PD_PLL_SEL_CLR
typedef union {
    REG32 v;
    struct {
        REG32 pdpllselclr : 7; // [6:0], write clear
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_PD_PLL_SEL_CLR_T;

//IDLE_CG_SW
typedef union {
    REG32 v;
    struct {
        REG32 bbpll1 : 1; // [0]
        REG32 bbpll2 : 1; // [1]
        REG32 apll : 1; // [2]
        REG32 audiopll : 1; // [3]
        REG32 usbpll : 1; // [4]
        REG32 mempll : 1; // [5]
        REG32 dsipll : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_IDLE_CG_SW_T;

//IDLE_CG_SW_SET
typedef union {
    REG32 v;
    struct {
        REG32 idlecgswset : 7; // [6:0], write set
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_IDLE_CG_SW_SET_T;

//IDLE_CG_SW_CLR
typedef union {
    REG32 v;
    struct {
        REG32 idlecgswclr : 7; // [6:0], write clear
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_IDLE_CG_SW_CLR_T;

//IDLE_CG_SEL
typedef union {
    REG32 v;
    struct {
        REG32 bbpll1 : 1; // [0]
        REG32 bbpll2 : 1; // [1]
        REG32 apll : 1; // [2]
        REG32 audiopll : 1; // [3]
        REG32 usbpll : 1; // [4]
        REG32 mempll : 1; // [5]
        REG32 dsipll : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_IDLE_CG_SEL_T;

//IDLE_CG_SEL_SET
typedef union {
    REG32 v;
    struct {
        REG32 idlecgselset : 7; // [6:0], write set
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_IDLE_CG_SEL_SET_T;

//IDLE_CG_SEL_CLR
typedef union {
    REG32 v;
    struct {
        REG32 idlecgselclr : 7; // [6:0], write clear
        REG32 __31_7 : 25;
    } b;
} REG_CP_IDLE_IDLE_CG_SEL_CLR_T;

//RF_IDLE_ENABLE_SW
typedef union {
    REG32 v;
    struct {
        REG32 rfidleenablesw : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_RF_IDLE_ENABLE_SW_T;

//RF_IDLE_ENABLE_SEL
typedef union {
    REG32 v;
    struct {
        REG32 rfidleenablesel : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_RF_IDLE_ENABLE_SEL_T;

//MEM_EMA_CFG
typedef union {
    REG32 v;
    struct {
        REG32 rftpd_rmea : 1; // [0]
        REG32 rftpd_rma : 4; // [4:1]
        REG32 rftpd_rmeb : 1; // [5]
        REG32 rftpd_rmb : 4; // [9:6]
        REG32 __31_10 : 22;
    } b;
} REG_CP_IDLE_MEM_EMA_CFG_T;

//UART_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 enable_clk_uart : 1; // [0]
        REG32 rst_ctrl_uart : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_CP_IDLE_UART_CTRL_T;

//DDR_LATCH
typedef union {
    REG32 v;
    struct {
        REG32 lpddr_latch : 1; // [0]
        REG32 psram_latch : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_CP_IDLE_DDR_LATCH_T;

//PAD_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_1_rxd_wpus : 1; // [0]
        REG32 pad_uart_1_rxd_se : 1; // [1]
        REG32 pad_uart_1_rxd_pull_up : 1; // [2]
        REG32 pad_uart_1_rxd_pull_dowe : 1; // [3]
        REG32 pad_uart_1_rxd_drv : 2; // [5:4]
        REG32 pad_uart_1_rxd_pull_frc : 1; // [6]
        REG32 pad_chip_pd_wpus : 1; // [7]
        REG32 pad_chip_pd_se : 1; // [8]
        REG32 pad_chip_pd_pull_up : 1; // [9]
        REG32 pad_chip_pd_pull_dowe : 1; // [10]
        REG32 pad_chip_pd_drv : 2; // [12:11]
        REG32 pad_chip_pd_pull_frc : 1; // [13]
        REG32 pad_chip_pd_out_frc : 1; // [14]
        REG32 pad_chip_pd_out : 1; // [15]
        REG32 pad_gpio_6_wpus : 1; // [16]
        REG32 pad_gpio_6_se : 1; // [17]
        REG32 pad_gpio_6_pull_up : 1; // [18]
        REG32 pad_gpio_6_pull_dowe : 1; // [19]
        REG32 pad_gpio_6_drv : 2; // [21:20]
        REG32 pad_gpio_6_pull_frc : 1; // [22]
        REG32 pad_osc_32k_wpus : 1; // [23]
        REG32 pad_osc_32k_se : 1; // [24]
        REG32 pad_osc_32k_drv : 2; // [26:25]
        REG32 pad_misc_idle_wpdi : 1; // [27]
        REG32 __31_28 : 4;
    } b;
} REG_CP_IDLE_PAD_CTRL_T;

//PAD_CTRL_UART_TXD
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_1_txd_wpus : 1; // [0]
        REG32 pad_uart_1_txd_se : 1; // [1]
        REG32 pad_uart_1_txd_pull_up : 1; // [2]
        REG32 pad_uart_1_txd_pull_dowe : 1; // [3]
        REG32 pad_uart_1_txd_drv : 2; // [5:4]
        REG32 pad_uart_1_txd_pull_frc : 1; // [6]
        REG32 pad_uart_1_txd_out_frc : 1; // [7]
        REG32 pad_uart_1_txd_out : 1; // [8]
        REG32 __31_9 : 23;
    } b;
} REG_CP_IDLE_PAD_CTRL_UART_TXD_T;

//MON_SEL
typedef union {
    REG32 v;
    struct {
        REG32 mon0_sel : 2; // [1:0]
        REG32 mon1_sel : 2; // [3:2]
        REG32 mon2_sel : 2; // [5:4]
        REG32 mon3_sel : 2; // [7:6]
        REG32 mon4_sel : 2; // [9:8]
        REG32 mon5_sel : 2; // [11:10]
        REG32 mon6_sel : 2; // [13:12]
        REG32 mon7_sel : 2; // [15:14]
        REG32 mon8_sel : 2; // [17:16]
        REG32 mon9_sel : 2; // [19:18]
        REG32 mon10_sel : 2; // [21:20]
        REG32 mon11_sel : 2; // [23:22]
        REG32 mon12_sel : 2; // [25:24]
        REG32 mon13_sel : 2; // [27:26]
        REG32 mon14_sel : 2; // [29:28]
        REG32 mon15_sel : 2; // [31:30]
    } b;
} REG_CP_IDLE_MON_SEL_T;

//TARGET_TIMER_EN
typedef union {
    REG32 v;
    struct {
        REG32 disable_target_timer : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_TARGET_TIMER_EN_T;

//TARGET_TIMER_STAT
typedef union {
    REG32 v;
    struct {
        REG32 timer_stat_122m : 1; // [0], read only
        REG32 timer_stat_32k : 1; // [1], read only
        REG32 __31_2 : 30;
    } b;
} REG_CP_IDLE_TARGET_TIMER_STAT_T;

//SLOW_SYS_CLK_SEL_HWEN
typedef union {
    REG32 v;
    struct {
        REG32 hwen : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_SLOW_SYS_CLK_SEL_HWEN_T;

//SLOW_CLK_SEL_HWEN
typedef union {
    REG32 v;
    struct {
        REG32 hwen : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CP_IDLE_SLOW_CLK_SEL_HWEN_T;

//SLEEP_PROT_TIME
typedef union {
    REG32 v;
    struct {
        REG32 prot_time : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CP_IDLE_SLEEP_PROT_TIME_T;


//IDL_CTRL_SYS1
#define CP_IDLE_RESERVED_0(n)      (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_IDCT_CTRL_SYS1     (1<<0)

//IDL_CTRL_SYS2
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_IDCT_CTRL_SYS2     (1<<0)

//IDL_EN
#define CP_IDLE_RESERVED_0_X1(n)   (((n)&0x3FFFFFFF)<<2)
#define CP_IDLE_IDL_AP_EN          (1<<1)
#define CP_IDLE_IDL_CP_EN          (1<<0)

//IDL_M_TIMER
#define CP_IDLE_IDCT_M_SYS(n)      (((n)&0xFFFFFFFF)<<0)

//IDL_WCN_EN
#define CP_IDLE_RESERVED_0_X2(n)   (((n)&0x7FFFFFF)<<5)
#define CP_IDLE_WCN_RES_VAL        (1<<4)
#define CP_IDLE_WCN_IDLE_CG        (1<<3)
#define CP_IDLE_WCN_PD_PLL         (1<<2)
#define CP_IDLE_WCN_PD_XTAL        (1<<1)
#define CP_IDLE_WCN_CHIP_PD        (1<<0)

//IDL_CTRL_TIMER
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_IDCT_CTRL_TIMER    (1<<0)

//IDL_M2_SYS
#define CP_IDLE_M1_SYS(n)          (((n)&0xFFFF)<<16)
#define CP_IDLE_M2_SYS(n)          (((n)&0xFFFF)<<0)

//IDL_TC_START
//#define CP_IDLE_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_IDLE_TC_START_MOD(n)    (((n)&3)<<0)

//IDL_TC_END
#define CP_IDLE_RESERVED_0_X3(n)   (((n)&0x7FF)<<21)
#define CP_IDLE_TC_END_FRAMC(n)    (((n)&0x1FFFF)<<4)
#define CP_IDLE_RESERVED_1(n)      (((n)&3)<<2)
#define CP_IDLE_TC_END_MOD(n)      (((n)&3)<<0)

//IDL_AWK_TIMER
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_WAKE_TIMER         (1<<0)

//GSM_LP_PU_DONE
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_LP_PU_DONE         (1<<0)

//GSM_FRAME_INTEN
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_GSM_FRAME_IRQ_EN   (1<<0)

//GSM_FRAME_INT_STA
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_GSM_FRAME_INT_STA  (1<<0)

//LTEM1_FRAME_INTEN
#define CP_IDLE_RESERVED_0_X4(n)   (((n)&0x1FFFFFFF)<<3)
#define CP_IDLE_LTEM1_FRAME3_IRQ_EN (1<<2)
#define CP_IDLE_LTEM1_FRAME2_IRQ_EN (1<<1)
#define CP_IDLE_LTEM1_FRAME1_IRQ_EN (1<<0)

//LTEM1_FRAME_INT_STA
//#define CP_IDLE_RESERVED_0_X4(n) (((n)&0x1FFFFFFF)<<3)
#define CP_IDLE_LTEM1_FRAME_INT_STA(n) (((n)&7)<<0)

//LTEM2_FRAME_INTEN
//#define CP_IDLE_RESERVED_0_X4(n) (((n)&0x1FFFFFFF)<<3)
#define CP_IDLE_LTEM2_FRAME3_IRQ_EN (1<<2)
#define CP_IDLE_LTEM2_FRAME2_IRQ_EN (1<<1)
#define CP_IDLE_LTEM2_FRAME1_IRQ_EN (1<<0)

//LTEM2_FRAME_INT_STA
//#define CP_IDLE_RESERVED_0_X4(n) (((n)&0x1FFFFFFF)<<3)
#define CP_IDLE_LTEM2_FRAME_INT_STA(n) (((n)&7)<<0)

//IDL_STA
#define CP_IDLE_RESERVED_0_X5(n)   (((n)&0x3FFFFFF)<<6)
#define CP_IDLE_IDLE_NB_TIMER_STAT (1<<5)
#define CP_IDLE_H_STAT             (1<<4)
#define CP_IDLE_IDLE_LTEM2_TIMER_STAT (1<<3)
#define CP_IDLE_IDLE_LTEM1_TIMER_STAT (1<<2)
#define CP_IDLE_IDLE_GSM_TIMER_STAT (1<<1)
#define CP_IDLE_IDLE_SYS_STAT      (1<<0)

//IDL_H_CTRL
#define CP_IDLE_RESERVED1_0(n)     (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_H_RUN_TIME(n)      (((n)&15)<<3)
#define CP_IDLE_H_AUTO_EN          (1<<2)
#define CP_IDLE_H_CTRL_EN          (1<<1)
#define CP_IDLE_RESERVED2_1        (1<<0)

//IDL_H_VAL
#define CP_IDLE_RESERVED_0_X6(n)   (((n)&31)<<27)
#define CP_IDLE_H_VALUE(n)         (((n)&0x7FFFFFF)<<0)

//IDL_H_GSM
//#define CP_IDLE_RESERVED_0_X6(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_H_LTEM
//#define CP_IDLE_RESERVED_0_X6(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_AWK_EN
#define CP_IDLE_RESERVED_0_X7(n)   (((n)&0x7FFF)<<17)
#define CP_IDLE_NB_LP_PU_REACH_EN  (1<<12)
#define CP_IDLE_GSM_LP_PU_REACH_EN (1<<11)
#define CP_IDLE_AWK_SELF_EN        (1<<10)
#define CP_IDLE_AWK_OSW2_EN        (1<<9)
#define CP_IDLE_AWK_OSW1_EN        (1<<8)
#define CP_IDLE_AWK7_EN            (1<<7)
#define CP_IDLE_AWK6_EN            (1<<6)
#define CP_IDLE_AWK5_EN            (1<<5)
#define CP_IDLE_AWK4_EN            (1<<4)
#define CP_IDLE_AWK3_EN            (1<<3)
#define CP_IDLE_AWK2_EN            (1<<2)
#define CP_IDLE_AWK1_EN            (1<<1)
#define CP_IDLE_AWK0_EN            (1<<0)
#define CP_IDLE_RESERVED_1_X1      (1<<13)
#define CP_IDLE_RESERVED_2         (1<<14)
#define CP_IDLE_RESERVED_3         (1<<15)
#define CP_IDLE_RESERVED_4         (1<<16)

//IDL_AWK_ST
//#define CP_IDLE_RESERVED_0_X6(n) (((n)&31)<<27)
#define CP_IDLE_POW_DFE_STA        (1<<26)
#define CP_IDLE_THR_STA            (1<<25)
#define CP_IDLE_POW_STA            (1<<24)
#define CP_IDLE_RESERVED_1_X2(n)   (((n)&7)<<21)
#define CP_IDLE_IDLE_STAT          (1<<20)
#define CP_IDLE_RESERVED_2_X1(n)   (((n)&7)<<17)
#define CP_IDLE_AWK_UP_STAT        (1<<16)
#define CP_IDLE_RESERVED_3_X1(n)   (((n)&7)<<13)
#define CP_IDLE_NB_LP_PU_REACH_STAT (1<<12)
#define CP_IDLE_GSM_LP_PU_REACH_STAT (1<<11)
#define CP_IDLE_AWK_SELF_STAT      (1<<10)
#define CP_IDLE_AWK_OSW2_STAT      (1<<9)
#define CP_IDLE_AWK_OSW1_STAT      (1<<8)
#define CP_IDLE_AWK7_AWK_STAT      (1<<7)
#define CP_IDLE_AWK6_AWK_STAT      (1<<6)
#define CP_IDLE_AWK5_AWK_STAT      (1<<5)
#define CP_IDLE_AWK4_AWK_STAT      (1<<4)
#define CP_IDLE_AWK3_AWK_STAT      (1<<3)
#define CP_IDLE_AWK2_AWK_STAT      (1<<2)
#define CP_IDLE_AWK1_AWK_STAT      (1<<1)
#define CP_IDLE_AWK0_AWK_STAT      (1<<0)

//IDL_AWK_SELF
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_WAKE_SELF          (1<<0)

//IDL_OSW1_EN
#define CP_IDLE_OSW1_EN            (1<<31)
#define CP_IDLE_OSW1_TIME(n)       (((n)&0x7FFFFFFF)<<0)

//IDL_OSW1_CONT
#define CP_IDLE_OSW1_COUNT(n)      (((n)&0xFFFFFFFF)<<0)

//IDL_FN_GSM
#define CP_IDLE_IDFN_GSM(n)        (((n)&0xFFFFFFFF)<<0)

//IDL_FN_LTEM1
#define CP_IDLE_IDFN_RAD_LTEM(n)   (((n)&0xFFFFFFF)<<4)
#define CP_IDLE_IDFN_SUB_LTEM(n)   (((n)&15)<<0)

//IDL_FN_LTEM2
//#define CP_IDLE_IDFN_RAD_LTEM(n) (((n)&0xFFFFFFF)<<4)
//#define CP_IDLE_IDFN_SUB_LTEM(n) (((n)&15)<<0)

//IDL_LTEM_RFL
//#define CP_IDLE_RESERVED_0_X3(n) (((n)&0x7FF)<<21)
#define CP_IDLE_LTEM_IDLE_RADIOFRAME_PARAMETER(n) (((n)&0x1FFFFF)<<0)

//IDL_LTEM_SFL
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_LTEM_IDLE_FRAME_PARAMETER(n) (((n)&0x1FFFF)<<0)

//IDL_SIG_EN
#define CP_IDLE_RESERVED_0_X8(n)   (((n)&0xFFFFFFF)<<4)
#define CP_IDLE_IDLE_CG_EN         (1<<3)
#define CP_IDLE_PD_PLL_EN          (1<<2)
#define CP_IDLE_PD_XTAL_EN         (1<<1)
#define CP_IDLE_CHIP_PD_EN         (1<<0)

//IDL_SIG_TIMER
#define CP_IDLE_T4(n)              (((n)&0xFF)<<24)
#define CP_IDLE_T3(n)              (((n)&0xFF)<<16)
#define CP_IDLE_T2(n)              (((n)&0xFF)<<8)
#define CP_IDLE_T1(n)              (((n)&0xFF)<<0)

//IDL_32K_REF
#define CP_IDLE_REK_REF(n)         (((n)&0xFFFFFFFF)<<0)

//CP_INTEN
#define CP_IDLE_RESERVED_0_X9(n)   (((n)&0x3FFFF)<<14)
#define CP_IDLE_TARGET_IRQ         (1<<13)
#define CP_IDLE_NB_PU_REACH_IRQ    (1<<12)
#define CP_IDLE_NB_TC_END_IRQ      (1<<11)
#define CP_IDLE_NB_TC_START_IRQ    (1<<10)
#define CP_IDLE_SYS_WAK_IRQ        (1<<9)
#define CP_IDLE_TIMER_AWK_IRQ      (1<<8)
#define CP_IDLE_GSM_PU_REACH_IRQ   (1<<7)
#define CP_IDLE_GSM_TC_END_IRQ     (1<<6)
#define CP_IDLE_GSM_TC_START_IRQ   (1<<5)
#define CP_IDLE_OSW1_IRQ           (1<<4)
#define CP_IDLE_TSTAMP_IRQ         (1<<3)
#define CP_IDLE_IDLE_FRAME_IRQ     (1<<2)
#define CP_IDLE_IDLE_H_IRQ         (1<<1)
#define CP_IDLE_LAYOUT_IRQ         (1<<0)

//CP_INTEN_SET
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_INT_EN_SET(n)      (((n)&0x3FFF)<<0)

//CP_INTEN_CLR
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_INT_EN_CLR(n)      (((n)&0x3FFF)<<0)

//CP_INT_STA
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_CP_INT_STA(n)      (((n)&0x3FFF)<<0)

//AP_INTEN
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x3FFFFFF)<<6)
#define CP_IDLE_TARGET_IRQ_X1      (1<<5)
#define CP_IDLE_NB_PU_REACH_IRQ_X1 (1<<4)
#define CP_IDLE_SYS_WAK_IRQ_X1     (1<<3)
#define CP_IDLE_TIMER_AWK_IRQ_X1   (1<<2)
#define CP_IDLE_GSM_PU_REACH_IRQ_X1 (1<<1)
#define CP_IDLE_OSW2_IRQ           (1<<0)

//AP_INTEN_SET
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x3FFFFFF)<<6)
#define CP_IDLE_INT_EN_SET_X1(n)   (((n)&0x3F)<<0)

//AP_INTEN_CLR
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x3FFFFFF)<<6)
#define CP_IDLE_INT_EN_CLR_X1(n)   (((n)&0x3F)<<0)

//AP_INT_STA
//#define CP_IDLE_RESERVED_0_X5(n) (((n)&0x3FFFFFF)<<6)
#define CP_IDLE_AP_INT_STA(n)      (((n)&0x3F)<<0)

//LTEM1_CFSR_HFN
#define CP_IDLE_RESERVED_0_X10(n)  (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_CFSR_HFN(n)   (((n)&0x3FFFFF)<<0)

//LTEM1_CFSR_FN
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_CFSR_RAD(n)   (((n)&0x3FF)<<4)
#define CP_IDLE_LTEM_CFSR_SUB(n)   (((n)&15)<<0)

//LTEM1_CFSRS
#define CP_IDLE_RESERVED_0_X11(n)  (((n)&0x3F)<<26)
#define CP_IDLE_ACTIVE_TIME        (1<<25)
#define CP_IDLE_ADJUST_DIRECT      (1<<24)
#define CP_IDLE_LTEM_CFSRS(n)      (((n)&0xFFFFFF)<<0)

//LTEM1_CFSR_RDH
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_CFSR_RDH(n)   (((n)&0x3FFFFF)<<0)

//LTEM1_CFSR_RDL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_CFSR_RDL_RAD(n) (((n)&0x3FF)<<4)
#define CP_IDLE_LTEM_CFSR_RDL_SUB(n) (((n)&15)<<0)

//LTEM1_FRAMC
#define CP_IDLE_RESERVED_0_X12(n)  (((n)&0xFFFF)<<16)
#define CP_IDLE_LFRAMC(n)          (((n)&0xFFFF)<<0)

//LTEM1_FRAML
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_LFRAML(n)          (((n)&0xFFFF)<<0)

//LTEM1_FRAMLS
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_ACTIVE_TIME_X1     (1<<16)
#define CP_IDLE_LFRAMLS(n)         (((n)&0xFFFF)<<0)

//LTEM1_CFSR_TPH
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_CFSR_TPH(n)   (((n)&0x3FFFFF)<<0)

//LTEM1_CFSR_TPL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_CFSR_TPL(n)   (((n)&0x3FFF)<<0)

//LTEM1_FRAMC_TP
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_LTEM_FRAMC_TP(n)   (((n)&0xFFFF)<<0)

//LTEM2_CFSR_HFN
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_CFSR_HFN(n) (((n)&0x3FFFFF)<<0)

//LTEM2_CFSR_FN
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_CFSR_RAD(n) (((n)&0x3FF)<<4)
//#define CP_IDLE_LTEM_CFSR_SUB(n) (((n)&15)<<0)

//LTEM2_CFSRS
//#define CP_IDLE_RESERVED_0_X11(n) (((n)&0x3F)<<26)
//#define CP_IDLE_ACTIVE_TIME      (1<<25)
//#define CP_IDLE_ADJUST_DIRECT    (1<<24)
//#define CP_IDLE_LTEM_CFSRS(n)    (((n)&0xFFFFFF)<<0)

//LTEM2_CFSR_RDH
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_CFSR_RDH(n) (((n)&0x3FFFFF)<<0)

//LTEM2_CFSR_RDL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_CFSR_RDL_RAD(n) (((n)&0x3FF)<<4)
//#define CP_IDLE_LTEM_CFSR_RDL_SUB(n) (((n)&15)<<0)

//LTEM2_FRAMC
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LFRAMC(n)        (((n)&0xFFFF)<<0)

//LTEM2_FRAML
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LFRAML(n)        (((n)&0xFFFF)<<0)

//LTEM2_FRAMLS
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
//#define CP_IDLE_ACTIVE_TIME_X1   (1<<16)
//#define CP_IDLE_LFRAMLS(n)       (((n)&0xFFFF)<<0)

//LTEM2_CFSR_TPH
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_CFSR_TPH(n) (((n)&0x3FFFFF)<<0)

//LTEM2_CFSR_TPL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_CFSR_TPL(n) (((n)&0x3FFF)<<0)

//LTEM2_FRAMC_TP
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LTEM_FRAMC_TP(n) (((n)&0xFFFF)<<0)

//GSM_CFSR
#define CP_IDLE_RESERVED_0_X13(n)  (((n)&0xFF)<<24)
#define CP_IDLE_GSM_CFSR_GSM_CFSR(n) (((n)&0xFFFFFF)<<0)

//GSM_CFSRS
#define CP_IDLE_RESERVED_0_X14(n)  (((n)&0x7F)<<25)
//#define CP_IDLE_ADJUST_DIRECT    (1<<24)
#define CP_IDLE_GSM_CFSRS_GSM_CFSRS(n) (((n)&0xFFFFFF)<<0)

//GSM_CFSRO
//#define CP_IDLE_RESERVED_0_X13(n) (((n)&0xFF)<<24)
#define CP_IDLE_GSM_CFSR_OVERFLOW(n) (((n)&0xFFFFFF)<<0)

//LTEM1_FHL
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x3FF)<<22)
#define CP_IDLE_LTEM_FHL(n)        (((n)&0x3FFFFF)<<0)

//LTEM1_FLL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
#define CP_IDLE_LTEM_FLL(n)        (((n)&0x3FFF)<<0)

//LTEM1_FCL
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_LTEM_FCL(n)        (((n)&0xFFFF)<<0)

//LTEM2_FHL
//#define CP_IDLE_RESERVED_0_X10(n) (((n)&0x3FF)<<22)
//#define CP_IDLE_LTEM_FHL(n)      (((n)&0x3FFFFF)<<0)

//LTEM2_FLL
//#define CP_IDLE_RESERVED_0_X9(n) (((n)&0x3FFFF)<<14)
//#define CP_IDLE_LTEM_FLL(n)      (((n)&0x3FFF)<<0)

//LTEM2_FCL
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_LTEM_FCL(n)      (((n)&0xFFFF)<<0)

//GSM_FL
//#define CP_IDLE_RESERVED_0_X13(n) (((n)&0xFF)<<24)
#define CP_IDLE_FL(n)              (((n)&0xFFFFFF)<<0)

//GSM_FCL
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_GSM_FCL_GSM_FCL(n) (((n)&0x1FFFF)<<0)

//TPCTRL
#define CP_IDLE_RESERVED1_0_X1(n)  (((n)&0x1FFFFF)<<11)
#define CP_IDLE_MOD_SEL(n)         (((n)&7)<<8)
#define CP_IDLE_RESERVED2_1_X1(n)  (((n)&3)<<6)
#define CP_IDLE_INNER_CONFG(n)     (((n)&3)<<4)
#define CP_IDLE_RESERVED3_2(n)     (((n)&7)<<1)
#define CP_IDLE_INNER_CTRL         (1<<0)

//LAYOUTT
#define CP_IDLE_LAYOUTT(n)         (((n)&0xFFFFFFFF)<<0)

//LAYOUTCTRL
#define CP_IDLE_RESERVED1_0_X2(n)  (((n)&0x1FF)<<23)
#define CP_IDLE_CHIP_COUNT(n)      (((n)&0x7FFF)<<8)
#define CP_IDLE_RESERVED2_1_X2(n)  (((n)&0x3F)<<2)
#define CP_IDLE_TIMER_SELECT       (1<<1)
#define CP_IDLE_ENABLE             (1<<0)

//LTEM1_FINT_DLY1
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_DELAY_TIME(n)      (((n)&0xFFFF)<<0)

//LTEM1_FINT_DLY2
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//LTEM2_FINT_DLY1
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//LTEM2_FINT_DLY2
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//FINT_EN
#define CP_IDLE_RESERVED1_0_X3(n)  (((n)&0x3FF)<<22)
#define CP_IDLE_LTE_M2_FINT_ENABLE(n) (((n)&0x3FF)<<12)
#define CP_IDLE_RESERVED2_1_X3(n)  (((n)&3)<<10)
#define CP_IDLE_LTE_M1_FINT_ENABLE(n) (((n)&0x3FF)<<0)

//TIMER_EN
//#define CP_IDLE_RESERVED_0_X2(n) (((n)&0x7FFFFFF)<<5)
#define CP_IDLE_NB_TIMER_ENABLE    (1<<4)
#define CP_IDLE_LTEM_TIMER_ENABLE  (1<<3)
#define CP_IDLE_GSM_TIMER_ENABLE   (1<<2)
#define CP_IDLE_LTE_M2_TIMER_ENABLE (1<<1)
#define CP_IDLE_LTE_M1_TIMER_ENABLE (1<<0)

//IDLE_FRAME_STA
//#define CP_IDLE_RESERVED_0_X2(n) (((n)&0x7FFFFFF)<<5)
#define CP_IDLE_NB_FRAME_STATE     (1<<4)
#define CP_IDLE_LTE_M_FRAME_STATE  (1<<3)
#define CP_IDLE_GSM_FRAME_STATE    (1<<2)
#define CP_IDLE_LTE_M2_FRAME_STATE (1<<1)
#define CP_IDLE_LTE_M1_FRAME_STATE (1<<0)

//IDLE_FRAME_LTEM1
#define CP_IDLE_RESERVED1_0_X4(n)  (((n)&0x7F)<<25)
#define CP_IDLE_FRAME_CONF         (1<<24)
#define CP_IDLE_RESERVED2_1_X4(n)  (((n)&3)<<22)
#define CP_IDLE_FRAME_CFSR(n)      (((n)&0x3FFFFF)<<0)

//IDLE_FRAME_LTEM2
//#define CP_IDLE_RESERVED1_0_X4(n) (((n)&0x7F)<<25)
//#define CP_IDLE_FRAME_CONF       (1<<24)
//#define CP_IDLE_RESERVED2_1_X4(n) (((n)&3)<<22)
//#define CP_IDLE_FRAME_CFSR(n)    (((n)&0x3FFFFF)<<0)

//IDLE_FRAME_GSM
//#define CP_IDLE_RESERVED_0_X14(n) (((n)&0x7F)<<25)
//#define CP_IDLE_FRAME_CONF       (1<<24)
#define CP_IDLE_FRAME_CFSR_X1(n)   (((n)&0xFFFFFF)<<0)

//IDLE_FRAME_LTE
#define CP_IDLE_FRAME_REF_CFSR(n)  (((n)&0xFFFFFFFF)<<0)

//IDLE_FRAME_LTE_CONF
#define CP_IDLE_RESRVED(n)         (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_FRAME_REF_LTE_CONF (1<<0)

//LTEM_REF_FN
#define CP_IDLE_LTEM_REF_FN_LTEM_REF_FN(n) (((n)&0xFFFFFFFF)<<0)

//LTEM_REF_FNL
#define CP_IDLE_REF_LTEM_FNL(n)    (((n)&0xFFFFFFFF)<<0)

//LTEM_REF_FCL
#define CP_IDLE_RESERVED_0_X15(n)  (((n)&0x1FFFF)<<15)
#define CP_IDLE_REF_LTEM_FCL(n)    (((n)&0x7FFF)<<0)

//REF_32K_FNL
#define CP_IDLE_REF_32K_FNL_REF_32K_FNL(n) (((n)&0xFFFFFFFF)<<0)

//LTEM_REF_FC
//#define CP_IDLE_RESERVED_0_X15(n) (((n)&0x1FFFF)<<15)
#define CP_IDLE_LTEM_REF_FC_LTEM_REF_FC(n) (((n)&0x7FFF)<<0)

//GSM_FRAML
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_GSM_FRAML(n)       (((n)&0x1FFFF)<<0)

//IDL_OSW2_EN
#define CP_IDLE_OSW2_EN            (1<<31)
#define CP_IDLE_OSW2_TIME(n)       (((n)&0x7FFFFFFF)<<0)

//IDL_OSW2_CONT
#define CP_IDLE_OSW2_COUNT(n)      (((n)&0xFFFFFFFF)<<0)

//IDLE_FRAMC_GSM
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_FRAMC_CFSR(n)      (((n)&0x1FFFF)<<0)

//LTEM1_FINT_DLY3
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//LTEM2_FINT_DLY3
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
//#define CP_IDLE_DELAY_TIME(n)    (((n)&0xFFFF)<<0)

//IDLE_TIME_SEL
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_TIME_SEL           (1<<0)

//IDLE_TIME
#define CP_IDLE_IDL_TIME(n)        (((n)&0xFFFFFFFF)<<0)

//IDL_H_GSM_LP
//#define CP_IDLE_RESERVED_0_X6(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_H_LTEM_LP
//#define CP_IDLE_RESERVED_0_X6(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_TC_START_NB
//#define CP_IDLE_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
//#define CP_IDLE_TC_START_MOD(n)  (((n)&3)<<0)

//IDL_TC_END_NB
//#define CP_IDLE_RESERVED_0_X3(n) (((n)&0x7FF)<<21)
//#define CP_IDLE_TC_END_FRAMC(n)  (((n)&0x1FFFF)<<4)
//#define CP_IDLE_RESERVED_1(n)    (((n)&3)<<2)
//#define CP_IDLE_TC_END_MOD(n)    (((n)&3)<<0)

//NB_LP_PU_DONE
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
//#define CP_IDLE_LP_PU_DONE       (1<<0)

//IDL_H_NB
//#define CP_IDLE_RESERVED_0_X6(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_H_NB_LP
//#define CP_IDLE_RESERVED_0_X6(n) (((n)&31)<<27)
//#define CP_IDLE_H_VALUE(n)       (((n)&0x7FFFFFF)<<0)

//IDL_FN_NB
#define CP_IDLE_IDFN_NB(n)         (((n)&0xFFFFFFFF)<<0)

//NB_FRAME_INTEN
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_NB_FRAME_IRQ_EN    (1<<0)

//NB_FRAME_INT_STA
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
//#define CP_IDLE_GSM_FRAME_INT_STA (1<<0)

//NB_CFSR
//#define CP_IDLE_RESERVED_0_X13(n) (((n)&0xFF)<<24)
#define CP_IDLE_GSM_CFSR(n)        (((n)&0xFFFFFF)<<0)

//NB_FRAML
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_NB_FRAML(n)        (((n)&0x1FFFF)<<0)

//NB_CFSRS
//#define CP_IDLE_RESERVED_0_X14(n) (((n)&0x7F)<<25)
//#define CP_IDLE_ADJUST_DIRECT    (1<<24)
#define CP_IDLE_NB_CFSRS_NB_CFSRS(n) (((n)&0xFFFFFF)<<0)

//NB_CFSRO
//#define CP_IDLE_RESERVED_0_X13(n) (((n)&0xFF)<<24)
#define CP_IDLE_NB_CFSR_OVERFLOW(n) (((n)&0xFFFFFF)<<0)

//NB_FL
//#define CP_IDLE_RESERVED_0_X13(n) (((n)&0xFF)<<24)
//#define CP_IDLE_FL(n)            (((n)&0xFFFFFF)<<0)

//NB_FCL
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
#define CP_IDLE_NB_FCL_NB_FCL(n)   (((n)&0x1FFFF)<<0)

//IDLE_FRAME_NB
//#define CP_IDLE_RESERVED_0_X14(n) (((n)&0x7F)<<25)
//#define CP_IDLE_FRAME_CONF       (1<<24)
//#define CP_IDLE_FRAME_CFSR_X1(n) (((n)&0xFFFFFF)<<0)

//IDLE_FRAMC_NB
//#define CP_IDLE_RESERVED_0_X7(n) (((n)&0x7FFF)<<17)
//#define CP_IDLE_FRAMC_CFSR(n)    (((n)&0x1FFFF)<<0)

//IDL_AWK_EN_SET
#define CP_IDLE_RESERVED_0_X16(n)  (((n)&0x7FFFF)<<13)
#define CP_IDLE_AWK_EN_SET(n)      (((n)&0x1FFF)<<0)

//IDL_AWK_EN_CLR
//#define CP_IDLE_RESERVED_0_X16(n) (((n)&0x7FFFF)<<13)
#define CP_IDLE_AWK_EN_CLEAR(n)    (((n)&0x1FFF)<<0)

//GSM_FRAMC
//#define CP_IDLE_RESERVED_0_X3(n) (((n)&0x7FF)<<21)
#define CP_IDLE_RD_ENABLE          (1<<20)
#define CP_IDLE_RESERVED_1_X3(n)   (((n)&7)<<17)
#define CP_IDLE_FRAMC(n)           (((n)&0x1FFFF)<<0)

//NB_FRAMC
//#define CP_IDLE_RESERVED_0_X3(n) (((n)&0x7FF)<<21)
//#define CP_IDLE_RD_ENABLE        (1<<20)
//#define CP_IDLE_RESERVED_1_X3(n) (((n)&7)<<17)
//#define CP_IDLE_FRAMC(n)         (((n)&0x1FFFF)<<0)

//ELIMINAT_JITTER
//#define CP_IDLE_RESERVED_0_X12(n) (((n)&0xFFFF)<<16)
#define CP_IDLE_ELIMINAT_TIME(n)   (((n)&0xFF)<<8)
#define CP_IDLE_ELIMIATE_EN(n)     (((n)&0xFF)<<0)

//GSM_EN_SEL
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_SELECT             (1<<0)

//NB_EN_SEL
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
//#define CP_IDLE_SELECT           (1<<0)

//PD_PLL_SW
#define CP_IDLE_RESERVED_0_X17(n)  (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_DSIPLL             (1<<6)
#define CP_IDLE_MEMPLL             (1<<5)
#define CP_IDLE_USBPLL             (1<<4)
#define CP_IDLE_AUDIOPLL           (1<<3)
#define CP_IDLE_APLL               (1<<2)
#define CP_IDLE_BBPLL2             (1<<1)
#define CP_IDLE_BBPLL1             (1<<0)

//PD_PLL_SW_SET
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_PDPLLSWSET(n)      (((n)&0x7F)<<0)

//PD_PLL_SW_CLR
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_PDPLLSWCLR(n)      (((n)&0x7F)<<0)

//PD_PLL_SEL
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
//#define CP_IDLE_DSIPLL           (1<<6)
//#define CP_IDLE_MEMPLL           (1<<5)
//#define CP_IDLE_USBPLL           (1<<4)
//#define CP_IDLE_AUDIOPLL         (1<<3)
//#define CP_IDLE_APLL             (1<<2)
//#define CP_IDLE_BBPLL2           (1<<1)
//#define CP_IDLE_BBPLL1           (1<<0)

//PD_PLL_SEL_SET
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_PDPLLSELSET(n)     (((n)&0x7F)<<0)

//PD_PLL_SEL_CLR
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_PDPLLSELCLR(n)     (((n)&0x7F)<<0)

//IDLE_CG_SW
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
//#define CP_IDLE_DSIPLL           (1<<6)
//#define CP_IDLE_MEMPLL           (1<<5)
//#define CP_IDLE_USBPLL           (1<<4)
//#define CP_IDLE_AUDIOPLL         (1<<3)
//#define CP_IDLE_APLL             (1<<2)
//#define CP_IDLE_BBPLL2           (1<<1)
//#define CP_IDLE_BBPLL1           (1<<0)

//IDLE_CG_SW_SET
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_IDLECGSWSET(n)     (((n)&0x7F)<<0)

//IDLE_CG_SW_CLR
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_IDLECGSWCLR(n)     (((n)&0x7F)<<0)

//IDLE_CG_SEL
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
//#define CP_IDLE_DSIPLL           (1<<6)
//#define CP_IDLE_MEMPLL           (1<<5)
//#define CP_IDLE_USBPLL           (1<<4)
//#define CP_IDLE_AUDIOPLL         (1<<3)
//#define CP_IDLE_APLL             (1<<2)
//#define CP_IDLE_BBPLL2           (1<<1)
//#define CP_IDLE_BBPLL1           (1<<0)

//IDLE_CG_SEL_SET
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_IDLECGSELSET(n)    (((n)&0x7F)<<0)

//IDLE_CG_SEL_CLR
//#define CP_IDLE_RESERVED_0_X17(n) (((n)&0x1FFFFFF)<<7)
#define CP_IDLE_IDLECGSELCLR(n)    (((n)&0x7F)<<0)

//RF_IDLE_ENABLE_SW
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_RFIDLEENABLESW     (1<<0)

//RF_IDLE_ENABLE_SEL
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_RFIDLEENABLESEL    (1<<0)

//MEM_EMA_CFG
#define CP_IDLE_RESERVED_0_X18(n)  (((n)&0x3FFFFF)<<10)
#define CP_IDLE_RFTPD_RMB(n)       (((n)&15)<<6)
#define CP_IDLE_RFTPD_RMEB         (1<<5)
#define CP_IDLE_RFTPD_RMA(n)       (((n)&15)<<1)
#define CP_IDLE_RFTPD_RMEA         (1<<0)

//UART_CTRL
//#define CP_IDLE_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_IDLE_RST_CTRL_UART      (1<<1)
#define CP_IDLE_ENABLE_CLK_UART    (1<<0)

//DDR_LATCH
//#define CP_IDLE_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_IDLE_PSRAM_LATCH        (1<<1)
#define CP_IDLE_LPDDR_LATCH        (1<<0)

//PAD_CTRL
#define CP_IDLE_RESERVED_0_X19(n)  (((n)&15)<<28)
#define CP_IDLE_PAD_MISC_IDLE_WPDI (1<<27)
#define CP_IDLE_PAD_OSC_32K_DRV(n) (((n)&3)<<25)
#define CP_IDLE_PAD_OSC_32K_SE     (1<<24)
#define CP_IDLE_PAD_OSC_32K_WPUS   (1<<23)
#define CP_IDLE_PAD_GPIO_6_PULL_FRC (1<<22)
#define CP_IDLE_PAD_GPIO_6_DRV(n)  (((n)&3)<<20)
#define CP_IDLE_PAD_GPIO_6_PULL_DOWE (1<<19)
#define CP_IDLE_PAD_GPIO_6_PULL_UP (1<<18)
#define CP_IDLE_PAD_GPIO_6_SE      (1<<17)
#define CP_IDLE_PAD_GPIO_6_WPUS    (1<<16)
#define CP_IDLE_PAD_CHIP_PD_OUT    (1<<15)
#define CP_IDLE_PAD_CHIP_PD_OUT_FRC (1<<14)
#define CP_IDLE_PAD_CHIP_PD_PULL_FRC (1<<13)
#define CP_IDLE_PAD_CHIP_PD_DRV(n) (((n)&3)<<11)
#define CP_IDLE_PAD_CHIP_PD_PULL_DOWE (1<<10)
#define CP_IDLE_PAD_CHIP_PD_PULL_UP (1<<9)
#define CP_IDLE_PAD_CHIP_PD_SE     (1<<8)
#define CP_IDLE_PAD_CHIP_PD_WPUS   (1<<7)
#define CP_IDLE_PAD_UART_1_RXD_PULL_FRC (1<<6)
#define CP_IDLE_PAD_UART_1_RXD_DRV(n) (((n)&3)<<4)
#define CP_IDLE_PAD_UART_1_RXD_PULL_DOWE (1<<3)
#define CP_IDLE_PAD_UART_1_RXD_PULL_UP (1<<2)
#define CP_IDLE_PAD_UART_1_RXD_SE  (1<<1)
#define CP_IDLE_PAD_UART_1_RXD_WPUS (1<<0)

//PAD_CTRL_UART_TXD
#define CP_IDLE_RESERVED_0_X20(n)  (((n)&0x7FFFFF)<<9)
#define CP_IDLE_PAD_UART_1_TXD_OUT (1<<8)
#define CP_IDLE_PAD_UART_1_TXD_OUT_FRC (1<<7)
#define CP_IDLE_PAD_UART_1_TXD_PULL_FRC (1<<6)
#define CP_IDLE_PAD_UART_1_TXD_DRV(n) (((n)&3)<<4)
#define CP_IDLE_PAD_UART_1_TXD_PULL_DOWE (1<<3)
#define CP_IDLE_PAD_UART_1_TXD_PULL_UP (1<<2)
#define CP_IDLE_PAD_UART_1_TXD_SE  (1<<1)
#define CP_IDLE_PAD_UART_1_TXD_WPUS (1<<0)

//MON_SEL
#define CP_IDLE_MON15_SEL(n)       (((n)&3)<<30)
#define CP_IDLE_MON14_SEL(n)       (((n)&3)<<28)
#define CP_IDLE_MON13_SEL(n)       (((n)&3)<<26)
#define CP_IDLE_MON12_SEL(n)       (((n)&3)<<24)
#define CP_IDLE_MON11_SEL(n)       (((n)&3)<<22)
#define CP_IDLE_MON10_SEL(n)       (((n)&3)<<20)
#define CP_IDLE_MON9_SEL(n)        (((n)&3)<<18)
#define CP_IDLE_MON8_SEL(n)        (((n)&3)<<16)
#define CP_IDLE_MON7_SEL(n)        (((n)&3)<<14)
#define CP_IDLE_MON6_SEL(n)        (((n)&3)<<12)
#define CP_IDLE_MON5_SEL(n)        (((n)&3)<<10)
#define CP_IDLE_MON4_SEL(n)        (((n)&3)<<8)
#define CP_IDLE_MON3_SEL(n)        (((n)&3)<<6)
#define CP_IDLE_MON2_SEL(n)        (((n)&3)<<4)
#define CP_IDLE_MON1_SEL(n)        (((n)&3)<<2)
#define CP_IDLE_MON0_SEL(n)        (((n)&3)<<0)

//MON_SEL_SET
#define CP_IDLE_MON_SEL_SET(n)     (((n)&0xFFFFFFFF)<<0)

//MON_SEL_CLR
#define CP_IDLE_MON_SEL_CLR(n)     (((n)&0xFFFFFFFF)<<0)

//TARGET_TIMER
#define CP_IDLE_TARGET_TIME(n)     (((n)&0xFFFFFFFF)<<0)

//TARGET_TIMER_EN
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_DISABLE_TARGET_TIMER (1<<0)

//TARGET_VALUE_LOCK
#define CP_IDLE_LOCK_VALUE(n)      (((n)&0xFFFFFFFF)<<0)

//TARGET_TIMER_STAT
//#define CP_IDLE_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_IDLE_TIMER_STAT_32K     (1<<1)
#define CP_IDLE_TIMER_STAT_122M    (1<<0)

//SLOW_SYS_CLK_SEL_HWEN
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
#define CP_IDLE_HWEN               (1<<0)

//SLOW_CLK_SEL_HWEN
//#define CP_IDLE_RESERVED_0(n)    (((n)&0x7FFFFFFF)<<1)
//#define CP_IDLE_HWEN             (1<<0)

//SLEEP_PROT_TIME
#define CP_IDLE_RESERVED_0_X21(n)  (((n)&0xFFFFFF)<<8)
#define CP_IDLE_PROT_TIME(n)       (((n)&0xFF)<<0)

//IDLE_RES0
#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES1
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES2
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES3
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES4
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES5
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES6
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES7
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES8
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES9
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES10
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)

//IDLE_RES11
//#define CP_IDLE_RESERVEDVALUE_0(n) (((n)&0xFFFFFFFF)<<0)





#endif

