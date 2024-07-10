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


#ifndef _ARM_AXIDMA_H_
#define _ARM_AXIDMA_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'arm_axidma'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// ARM_AXIDMA_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_DMA_BASE               0x090C0000

typedef volatile struct
{
    REG32                          AXIDMA_CONF;                  //0x00000000
    REG32                          AXIDMA_DELAY;                 //0x00000004
    REG32                          AXIDMA_STATUS;                //0x00000008
    REG32                          AXIDMA_IRQ_STAT;              //0x0000000C
    REG32                          AXIDMA_ARM_REQ_STAT;          //0x00000010
    REG32                          AXIDMA_ARM_ACK_STAT;          //0x00000014
    REG32                          AXIDMA_ZSP_REQ_STAT0;         //0x00000018
    REG32                          AXIDMA_ZSP_REQ_STAT1;         //0x0000001C
    REG32                          AXIDMA_CH_IRQ_DISTR;          //0x00000020
    REG32 Reserved_00000024[7];                 //0x00000024
    REG32                          AXIDMA_C0_CONF;               //0x00000040
    REG32                          AXIDMA_C0_MAP;                //0x00000044
    REG32                          AXIDMA_C0_SADDR;              //0x00000048
    REG32                          AXIDMA_C0_DADDR;              //0x0000004C
    REG32                          AXIDMA_C0_COUNT;              //0x00000050
    REG32                          AXIDMA_C0_COUNTP;             //0x00000054
    REG32                          AXIDMA_C0_STATUS;             //0x00000058
    REG32                          AXIDMA_C0_SGADDR;             //0x0000005C
    REG32                          AXIDMA_C0_SGCONF;             //0x00000060
    REG32                          AXIDMA_C0_SET;                //0x00000064
    REG32                          AXIDMA_C0_CLR;                //0x00000068
    REG32 Reserved_0000006C[5];                 //0x0000006C
    REG32                          AXIDMA_C1_CONF;               //0x00000080
    REG32                          AXIDMA_C1_MAP;                //0x00000084
    REG32                          AXIDMA_C1_SADDR;              //0x00000088
    REG32                          AXIDMA_C1_DADDR;              //0x0000008C
    REG32                          AXIDMA_C1_COUNT;              //0x00000090
    REG32                          AXIDMA_C1_COUNTP;             //0x00000094
    REG32                          AXIDMA_C1_STATUS;             //0x00000098
    REG32                          AXIDMA_C1_SGADDR;             //0x0000009C
    REG32                          AXIDMA_C1_SGCONF;             //0x000000A0
    REG32                          AXIDMA_C1_SET;                //0x000000A4
    REG32                          AXIDMA_C1_CLR;                //0x000000A8
    REG32 Reserved_000000AC[5];                 //0x000000AC
    REG32                          AXIDMA_C2_CONF;               //0x000000C0
    REG32                          AXIDMA_C2_MAP;                //0x000000C4
    REG32                          AXIDMA_C2_SADDR;              //0x000000C8
    REG32                          AXIDMA_C2_DADDR;              //0x000000CC
    REG32                          AXIDMA_C2_COUNT;              //0x000000D0
    REG32                          AXIDMA_C2_COUNTP;             //0x000000D4
    REG32                          AXIDMA_C2_STATUS;             //0x000000D8
    REG32                          AXIDMA_C2_SGADDR;             //0x000000DC
    REG32                          AXIDMA_C2_SGCONF;             //0x000000E0
    REG32                          AXIDMA_C2_SET;                //0x000000E4
    REG32                          AXIDMA_C2_CLR;                //0x000000E8
    REG32 Reserved_000000EC[5];                 //0x000000EC
    REG32                          AXIDMA_C3_CONF;               //0x00000100
    REG32                          AXIDMA_C3_MAP;                //0x00000104
    REG32                          AXIDMA_C3_SADDR;              //0x00000108
    REG32                          AXIDMA_C3_DADDR;              //0x0000010C
    REG32                          AXIDMA_C3_COUNT;              //0x00000110
    REG32                          AXIDMA_C3_COUNTP;             //0x00000114
    REG32                          AXIDMA_C3_STATUS;             //0x00000118
    REG32                          AXIDMA_C3_SGADDR;             //0x0000011C
    REG32                          AXIDMA_C3_SGCONF;             //0x00000120
    REG32                          AXIDMA_C3_SET;                //0x00000124
    REG32                          AXIDMA_C3_CLR;                //0x00000128
    REG32 Reserved_0000012C[5];                 //0x0000012C
    REG32                          AXIDMA_C4_CONF;               //0x00000140
    REG32                          AXIDMA_C4_MAP;                //0x00000144
    REG32                          AXIDMA_C4_SADDR;              //0x00000148
    REG32                          AXIDMA_C4_DADDR;              //0x0000014C
    REG32                          AXIDMA_C4_COUNT;              //0x00000150
    REG32                          AXIDMA_C4_COUNTP;             //0x00000154
    REG32                          AXIDMA_C4_STATUS;             //0x00000158
    REG32                          AXIDMA_C4_SGADDR;             //0x0000015C
    REG32                          AXIDMA_C4_SGCONF;             //0x00000160
    REG32                          AXIDMA_C4_SET;                //0x00000164
    REG32                          AXIDMA_C4_CLR;                //0x00000168
    REG32 Reserved_0000016C[5];                 //0x0000016C
    REG32                          AXIDMA_C5_CONF;               //0x00000180
    REG32                          AXIDMA_C5_MAP;                //0x00000184
    REG32                          AXIDMA_C5_SADDR;              //0x00000188
    REG32                          AXIDMA_C5_DADDR;              //0x0000018C
    REG32                          AXIDMA_C5_COUNT;              //0x00000190
    REG32                          AXIDMA_C5_COUNTP;             //0x00000194
    REG32                          AXIDMA_C5_STATUS;             //0x00000198
    REG32                          AXIDMA_C5_SGADDR;             //0x0000019C
    REG32                          AXIDMA_C5_SGCONF;             //0x000001A0
    REG32                          AXIDMA_C5_SET;                //0x000001A4
    REG32                          AXIDMA_C5_CLR;                //0x000001A8
    REG32 Reserved_000001AC[5];                 //0x000001AC
    REG32                          AXIDMA_C6_CONF;               //0x000001C0
    REG32                          AXIDMA_C6_MAP;                //0x000001C4
    REG32                          AXIDMA_C6_SADDR;              //0x000001C8
    REG32                          AXIDMA_C6_DADDR;              //0x000001CC
    REG32                          AXIDMA_C6_COUNT;              //0x000001D0
    REG32                          AXIDMA_C6_COUNTP;             //0x000001D4
    REG32                          AXIDMA_C6_STATUS;             //0x000001D8
    REG32                          AXIDMA_C6_SGADDR;             //0x000001DC
    REG32                          AXIDMA_C6_SGCONF;             //0x000001E0
    REG32                          AXIDMA_C6_SET;                //0x000001E4
    REG32                          AXIDMA_C6_CLR;                //0x000001E8
    REG32 Reserved_000001EC[5];                 //0x000001EC
    REG32                          AXIDMA_C7_CONF;               //0x00000200
    REG32                          AXIDMA_C7_MAP;                //0x00000204
    REG32                          AXIDMA_C7_SADDR;              //0x00000208
    REG32                          AXIDMA_C7_DADDR;              //0x0000020C
    REG32                          AXIDMA_C7_COUNT;              //0x00000210
    REG32                          AXIDMA_C7_COUNTP;             //0x00000214
    REG32                          AXIDMA_C7_STATUS;             //0x00000218
    REG32                          AXIDMA_C7_SGADDR;             //0x0000021C
    REG32                          AXIDMA_C7_SGCONF;             //0x00000220
    REG32                          AXIDMA_C7_SET;                //0x00000224
    REG32                          AXIDMA_C7_CLR;                //0x00000228
    REG32 Reserved_0000022C[5];                 //0x0000022C
    REG32                          AXIDMA_C8_CONF;               //0x00000240
    REG32                          AXIDMA_C8_MAP;                //0x00000244
    REG32                          AXIDMA_C8_SADDR;              //0x00000248
    REG32                          AXIDMA_C8_DADDR;              //0x0000024C
    REG32                          AXIDMA_C8_COUNT;              //0x00000250
    REG32                          AXIDMA_C8_COUNTP;             //0x00000254
    REG32                          AXIDMA_C8_STATUS;             //0x00000258
    REG32                          AXIDMA_C8_SGADDR;             //0x0000025C
    REG32                          AXIDMA_C8_SGCONF;             //0x00000260
    REG32                          AXIDMA_C8_SET;                //0x00000264
    REG32                          AXIDMA_C8_CLR;                //0x00000268
    REG32 Reserved_0000026C[5];                 //0x0000026C
    REG32                          AXIDMA_C9_CONF;               //0x00000280
    REG32                          AXIDMA_C9_MAP;                //0x00000284
    REG32                          AXIDMA_C9_SADDR;              //0x00000288
    REG32                          AXIDMA_C9_DADDR;              //0x0000028C
    REG32                          AXIDMA_C9_COUNT;              //0x00000290
    REG32                          AXIDMA_C9_COUNTP;             //0x00000294
    REG32                          AXIDMA_C9_STATUS;             //0x00000298
    REG32                          AXIDMA_C9_SGADDR;             //0x0000029C
    REG32                          AXIDMA_C9_SGCONF;             //0x000002A0
    REG32                          AXIDMA_C9_SET;                //0x000002A4
    REG32                          AXIDMA_C9_CLR;                //0x000002A8
    REG32 Reserved_000002AC[5];                 //0x000002AC
    REG32                          AXIDMA_C10_CONF;              //0x000002C0
    REG32                          AXIDMA_C10_MAP;               //0x000002C4
    REG32                          AXIDMA_C10_SADDR;             //0x000002C8
    REG32                          AXIDMA_C10_DADDR;             //0x000002CC
    REG32                          AXIDMA_C10_COUNT;             //0x000002D0
    REG32                          AXIDMA_C10_COUNTP;            //0x000002D4
    REG32                          AXIDMA_C10_STATUS;            //0x000002D8
    REG32                          AXIDMA_C10_SGADDR;            //0x000002DC
    REG32                          AXIDMA_C10_SGCONF;            //0x000002E0
    REG32                          AXIDMA_C10_SET;               //0x000002E4
    REG32                          AXIDMA_C10_CLR;               //0x000002E8
    REG32 Reserved_000002EC[5];                 //0x000002EC
    REG32                          AXIDMA_C11_CONF;              //0x00000300
    REG32                          AXIDMA_C11_MAP;               //0x00000304
    REG32                          AXIDMA_C11_SADDR;             //0x00000308
    REG32                          AXIDMA_C11_DADDR;             //0x0000030C
    REG32                          AXIDMA_C11_COUNT;             //0x00000310
    REG32                          AXIDMA_C11_COUNTP;            //0x00000314
    REG32                          AXIDMA_C11_STATUS;            //0x00000318
    REG32                          AXIDMA_C11_SGADDR;            //0x0000031C
    REG32                          AXIDMA_C11_SGCONF;            //0x00000320
    REG32                          AXIDMA_C11_SET;               //0x00000324
    REG32                          AXIDMA_C11_CLR;               //0x00000328
} HWP_ARM_AXIDMA_T;

#define hwp_dma                    ((HWP_ARM_AXIDMA_T*) REG_ACCESS_ADDRESS(REG_DMA_BASE))


//AXIDMA_CONF
typedef union {
    REG32 v;
    struct {
        REG32 stop : 1; // [0]
        REG32 stop_ie : 1; // [1]
        REG32 priority : 1; // [2]
        REG32 outstand : 2; // [4:3]
        REG32 mode_sel : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_ARM_AXIDMA_AXIDMA_CONF_T;

//AXIDMA_DELAY
typedef union {
    REG32 v;
    struct {
        REG32 delay : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_DELAY_T;

//AXIDMA_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 ch_num : 4; // [3:0], read only
        REG32 stop_status : 1; // [4], read only
        REG32 __31_5 : 27;
    } b;
} REG_ARM_AXIDMA_AXIDMA_STATUS_T;

//AXIDMA_IRQ_STAT
typedef union {
    REG32 v;
    struct {
        REG32 ch0_irq : 1; // [0], read only
        REG32 ch1_irq : 1; // [1], read only
        REG32 ch2_irq : 1; // [2], read only
        REG32 ch3_irq : 1; // [3], read only
        REG32 ch4_irq : 1; // [4], read only
        REG32 ch5_irq : 1; // [5], read only
        REG32 ch6_irq : 1; // [6], read only
        REG32 ch7_irq : 1; // [7], read only
        REG32 ch8_irq : 1; // [8], read only
        REG32 ch9_irq : 1; // [9], read only
        REG32 ch10_irq : 1; // [10], read only
        REG32 ch11_irq : 1; // [11], read only
        REG32 __31_12 : 20;
    } b;
} REG_ARM_AXIDMA_AXIDMA_IRQ_STAT_T;

//AXIDMA_ARM_REQ_STAT
typedef union {
    REG32 v;
    struct {
        REG32 irq0 : 1; // [0], read only
        REG32 irq1 : 1; // [1], read only
        REG32 irq2 : 1; // [2], read only
        REG32 irq3 : 1; // [3], read only
        REG32 irq4 : 1; // [4], read only
        REG32 irq5 : 1; // [5], read only
        REG32 irq6 : 1; // [6], read only
        REG32 irq7 : 1; // [7], read only
        REG32 irq8 : 1; // [8], read only
        REG32 irq9 : 1; // [9], read only
        REG32 irq10 : 1; // [10], read only
        REG32 irq11 : 1; // [11], read only
        REG32 irq12 : 1; // [12], read only
        REG32 irq13 : 1; // [13], read only
        REG32 irq14 : 1; // [14], read only
        REG32 irq15 : 1; // [15], read only
        REG32 irq16 : 1; // [16], read only
        REG32 irq17 : 1; // [17], read only
        REG32 irq18 : 1; // [18], read only
        REG32 irq19 : 1; // [19], read only
        REG32 irq20 : 1; // [20], read only
        REG32 irq21 : 1; // [21], read only
        REG32 irq22 : 1; // [22], read only
        REG32 irq23 : 1; // [23], read only
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_ARM_REQ_STAT_T;

//AXIDMA_ARM_ACK_STAT
typedef union {
    REG32 v;
    struct {
        REG32 ack0 : 1; // [0], read only
        REG32 ack1 : 1; // [1], read only
        REG32 ack2 : 1; // [2], read only
        REG32 ack3 : 1; // [3], read only
        REG32 ack4 : 1; // [4], read only
        REG32 ack5 : 1; // [5], read only
        REG32 ack6 : 1; // [6], read only
        REG32 ack7 : 1; // [7], read only
        REG32 ack8 : 1; // [8], read only
        REG32 ack9 : 1; // [9], read only
        REG32 ack10 : 1; // [10], read only
        REG32 ack11 : 1; // [11], read only
        REG32 ack12 : 1; // [12], read only
        REG32 ack13 : 1; // [13], read only
        REG32 ack14 : 1; // [14], read only
        REG32 ack15 : 1; // [15], read only
        REG32 ack16 : 1; // [16], read only
        REG32 ack17 : 1; // [17], read only
        REG32 ack18 : 1; // [18], read only
        REG32 ack19 : 1; // [19], read only
        REG32 ack20 : 1; // [20], read only
        REG32 ack21 : 1; // [21], read only
        REG32 ack22 : 1; // [22], read only
        REG32 ack23 : 1; // [23], read only
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_ARM_ACK_STAT_T;

//AXIDMA_ZSP_REQ_STAT0
typedef union {
    REG32 v;
    struct {
        REG32 req0 : 3; // [2:0]
        REG32 reserved8_7 : 1; // [3], read only
        REG32 req1 : 3; // [6:4]
        REG32 reserved7_6 : 1; // [7], read only
        REG32 req2 : 3; // [10:8]
        REG32 reserved6_5 : 1; // [11], read only
        REG32 req3 : 3; // [14:12]
        REG32 reserved5_4 : 1; // [15], read only
        REG32 req4 : 3; // [18:16]
        REG32 reserved4_3 : 1; // [19], read only
        REG32 req5 : 3; // [22:20]
        REG32 reserved3_2 : 1; // [23], read only
        REG32 req6 : 3; // [26:24]
        REG32 reserved2_1 : 1; // [27], read only
        REG32 req7 : 3; // [30:28]
        REG32 reserved1_0 : 1; // [31], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_ZSP_REQ_STAT0_T;

//AXIDMA_ZSP_REQ_STAT1
typedef union {
    REG32 v;
    struct {
        REG32 req8 : 3; // [2:0]
        REG32 reserved4_3 : 1; // [3], read only
        REG32 req9 : 3; // [6:4]
        REG32 reserved3_2 : 1; // [7], read only
        REG32 req10 : 3; // [10:8]
        REG32 reserved2_1 : 1; // [11], read only
        REG32 req11 : 3; // [14:12]
        REG32 reserved1_0 : 17; // [31:15], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_ZSP_REQ_STAT1_T;

//AXIDMA_CH_IRQ_DISTR
typedef union {
    REG32 v;
    struct {
        REG32 ch0_irq_en0 : 1; // [0]
        REG32 ch1_irq_en0 : 1; // [1]
        REG32 ch2_irq_en0 : 1; // [2]
        REG32 ch3_irq_en0 : 1; // [3]
        REG32 ch4_irq_en0 : 1; // [4]
        REG32 ch5_irq_en0 : 1; // [5]
        REG32 ch6_irq_en0 : 1; // [6]
        REG32 ch7_irq_en0 : 1; // [7]
        REG32 ch8_irq_en0 : 1; // [8]
        REG32 ch9_irq_en0 : 1; // [9]
        REG32 ch10_irq_en0 : 1; // [10]
        REG32 ch11_irq_en0 : 1; // [11]
        REG32 __31_12 : 20;
    } b;
} REG_ARM_AXIDMA_AXIDMA_CH_IRQ_DISTR_T;

//AXIDMA_C0_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_CONF_T;

//AXIDMA_C0_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_MAP_T;

//AXIDMA_C0_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_COUNT_T;

//AXIDMA_C0_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_COUNTP_T;

//AXIDMA_C0_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_STATUS_T;

//AXIDMA_C0_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_SGCONF_T;

//AXIDMA_C0_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_SET_T;

//AXIDMA_C0_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C0_CLR_T;

//AXIDMA_C1_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_CONF_T;

//AXIDMA_C1_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_MAP_T;

//AXIDMA_C1_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_COUNT_T;

//AXIDMA_C1_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_COUNTP_T;

//AXIDMA_C1_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_STATUS_T;

//AXIDMA_C1_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_SGCONF_T;

//AXIDMA_C1_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_SET_T;

//AXIDMA_C1_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C1_CLR_T;

//AXIDMA_C2_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_CONF_T;

//AXIDMA_C2_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_MAP_T;

//AXIDMA_C2_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_COUNT_T;

//AXIDMA_C2_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_COUNTP_T;

//AXIDMA_C2_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_STATUS_T;

//AXIDMA_C2_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_SGCONF_T;

//AXIDMA_C2_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_SET_T;

//AXIDMA_C2_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C2_CLR_T;

//AXIDMA_C3_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_CONF_T;

//AXIDMA_C3_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_MAP_T;

//AXIDMA_C3_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_COUNT_T;

//AXIDMA_C3_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_COUNTP_T;

//AXIDMA_C3_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_STATUS_T;

//AXIDMA_C3_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_SGCONF_T;

//AXIDMA_C3_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_SET_T;

//AXIDMA_C3_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C3_CLR_T;

//AXIDMA_C4_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_CONF_T;

//AXIDMA_C4_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_MAP_T;

//AXIDMA_C4_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_COUNT_T;

//AXIDMA_C4_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_COUNTP_T;

//AXIDMA_C4_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_STATUS_T;

//AXIDMA_C4_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_SGCONF_T;

//AXIDMA_C4_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_SET_T;

//AXIDMA_C4_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C4_CLR_T;

//AXIDMA_C5_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_CONF_T;

//AXIDMA_C5_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_MAP_T;

//AXIDMA_C5_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_COUNT_T;

//AXIDMA_C5_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_COUNTP_T;

//AXIDMA_C5_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_STATUS_T;

//AXIDMA_C5_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_SGCONF_T;

//AXIDMA_C5_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_SET_T;

//AXIDMA_C5_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C5_CLR_T;

//AXIDMA_C6_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_CONF_T;

//AXIDMA_C6_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_MAP_T;

//AXIDMA_C6_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_COUNT_T;

//AXIDMA_C6_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_COUNTP_T;

//AXIDMA_C6_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_STATUS_T;

//AXIDMA_C6_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_SGCONF_T;

//AXIDMA_C6_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_SET_T;

//AXIDMA_C6_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C6_CLR_T;

//AXIDMA_C7_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_CONF_T;

//AXIDMA_C7_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_MAP_T;

//AXIDMA_C7_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_COUNT_T;

//AXIDMA_C7_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_COUNTP_T;

//AXIDMA_C7_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_STATUS_T;

//AXIDMA_C7_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_SGCONF_T;

//AXIDMA_C7_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_SET_T;

//AXIDMA_C7_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C7_CLR_T;

//AXIDMA_C8_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_CONF_T;

//AXIDMA_C8_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_MAP_T;

//AXIDMA_C8_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_COUNT_T;

//AXIDMA_C8_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_COUNTP_T;

//AXIDMA_C8_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_STATUS_T;

//AXIDMA_C8_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_SGCONF_T;

//AXIDMA_C8_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_SET_T;

//AXIDMA_C8_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C8_CLR_T;

//AXIDMA_C9_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_CONF_T;

//AXIDMA_C9_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_MAP_T;

//AXIDMA_C9_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_COUNT_T;

//AXIDMA_C9_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_COUNTP_T;

//AXIDMA_C9_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_STATUS_T;

//AXIDMA_C9_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_SGCONF_T;

//AXIDMA_C9_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_SET_T;

//AXIDMA_C9_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C9_CLR_T;

//AXIDMA_C10_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_CONF_T;

//AXIDMA_C10_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_MAP_T;

//AXIDMA_C10_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_COUNT_T;

//AXIDMA_C10_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_COUNTP_T;

//AXIDMA_C10_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_STATUS_T;

//AXIDMA_C10_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_SGCONF_T;

//AXIDMA_C10_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_SET_T;

//AXIDMA_C10_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C10_CLR_T;

//AXIDMA_C11_CONF
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 data_type : 2; // [2:1]
        REG32 syn_irq : 1; // [3]
        REG32 irq_f : 1; // [4]
        REG32 irq_t : 1; // [5]
        REG32 saddr_fix : 1; // [6]
        REG32 daddr_fix : 1; // [7]
        REG32 force_trans : 1; // [8]
        REG32 req_sel : 1; // [9]
        REG32 count_sel : 1; // [10]
        REG32 zsp_dma_ack_en : 1; // [11]
        REG32 saddr_turnaround : 1; // [12]
        REG32 daddr_turnaround : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_CONF_T;

//AXIDMA_C11_MAP
typedef union {
    REG32 v;
    struct {
        REG32 req_source : 5; // [4:0]
        REG32 reserved2_1 : 3; // [7:5], read only
        REG32 ack_map : 5; // [12:8]
        REG32 reserved1_0 : 19; // [31:13], read only
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_MAP_T;

//AXIDMA_C11_COUNT
typedef union {
    REG32 v;
    struct {
        REG32 count : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_COUNT_T;

//AXIDMA_C11_COUNTP
typedef union {
    REG32 v;
    struct {
        REG32 countp : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_COUNTP_T;

//AXIDMA_C11_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 run : 1; // [0], write clear
        REG32 count_finish : 1; // [1], write clear
        REG32 countp_finish : 1; // [2], write clear
        REG32 sg_finish : 1; // [3], write clear
        REG32 sg_count : 16; // [19:4], write clear
        REG32 sg_suspend : 1; // [20], write clear
        REG32 count_finish_sta : 1; // [21], write clear
        REG32 countp_finish_sta : 1; // [22], write clear
        REG32 sg_finish_sta : 1; // [23], write clear
        REG32 sg_suspend_sta : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_STATUS_T;

//AXIDMA_C11_SGCONF
typedef union {
    REG32 v;
    struct {
        REG32 sg_en : 1; // [0], write clear
        REG32 sg_finish_ie : 1; // [1]
        REG32 sg_suspend_ie : 1; // [2]
        REG32 desc_rd_ctrl : 1; // [3]
        REG32 sg_num : 16; // [19:4]
        REG32 __31_20 : 12;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_SGCONF_T;

//AXIDMA_C11_SET
typedef union {
    REG32 v;
    struct {
        REG32 run_set : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_SET_T;

//AXIDMA_C11_CLR
typedef union {
    REG32 v;
    struct {
        REG32 run_clr : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_ARM_AXIDMA_AXIDMA_C11_CLR_T;


//AXIDMA_CONF
#define ARM_AXIDMA_RESERVED_0(n)   (((n)&0x3FFFFFF)<<6)
#define ARM_AXIDMA_MODE_SEL        (1<<5)
#define ARM_AXIDMA_OUTSTAND(n)     (((n)&3)<<3)
#define ARM_AXIDMA_PRIORITY        (1<<2)
#define ARM_AXIDMA_STOP_IE         (1<<1)
#define ARM_AXIDMA_STOP            (1<<0)

//AXIDMA_DELAY
#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
#define ARM_AXIDMA_DELAY(n)        (((n)&0xFFFF)<<0)

//AXIDMA_STATUS
#define ARM_AXIDMA_RESERVED_0_X2(n) (((n)&0x7FFFFFF)<<5)
#define ARM_AXIDMA_STOP_STATUS     (1<<4)
#define ARM_AXIDMA_CH_NUM(n)       (((n)&15)<<0)

//AXIDMA_IRQ_STAT
#define ARM_AXIDMA_RESERVED_0_X3(n) (((n)&0xFFFFF)<<12)
#define ARM_AXIDMA_CH11_IRQ        (1<<11)
#define ARM_AXIDMA_CH10_IRQ        (1<<10)
#define ARM_AXIDMA_CH9_IRQ         (1<<9)
#define ARM_AXIDMA_CH8_IRQ         (1<<8)
#define ARM_AXIDMA_CH7_IRQ         (1<<7)
#define ARM_AXIDMA_CH6_IRQ         (1<<6)
#define ARM_AXIDMA_CH5_IRQ         (1<<5)
#define ARM_AXIDMA_CH4_IRQ         (1<<4)
#define ARM_AXIDMA_CH3_IRQ         (1<<3)
#define ARM_AXIDMA_CH2_IRQ         (1<<2)
#define ARM_AXIDMA_CH1_IRQ         (1<<1)
#define ARM_AXIDMA_CH0_IRQ         (1<<0)

//AXIDMA_ARM_REQ_STAT
#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
#define ARM_AXIDMA_IRQ23           (1<<23)
#define ARM_AXIDMA_IRQ22           (1<<22)
#define ARM_AXIDMA_IRQ21           (1<<21)
#define ARM_AXIDMA_IRQ20           (1<<20)
#define ARM_AXIDMA_IRQ19           (1<<19)
#define ARM_AXIDMA_IRQ18           (1<<18)
#define ARM_AXIDMA_IRQ17           (1<<17)
#define ARM_AXIDMA_IRQ16           (1<<16)
#define ARM_AXIDMA_IRQ15           (1<<15)
#define ARM_AXIDMA_IRQ14           (1<<14)
#define ARM_AXIDMA_IRQ13           (1<<13)
#define ARM_AXIDMA_IRQ12           (1<<12)
#define ARM_AXIDMA_IRQ11           (1<<11)
#define ARM_AXIDMA_IRQ10           (1<<10)
#define ARM_AXIDMA_IRQ9            (1<<9)
#define ARM_AXIDMA_IRQ8            (1<<8)
#define ARM_AXIDMA_IRQ7            (1<<7)
#define ARM_AXIDMA_IRQ6            (1<<6)
#define ARM_AXIDMA_IRQ5            (1<<5)
#define ARM_AXIDMA_IRQ4            (1<<4)
#define ARM_AXIDMA_IRQ3            (1<<3)
#define ARM_AXIDMA_IRQ2            (1<<2)
#define ARM_AXIDMA_IRQ1            (1<<1)
#define ARM_AXIDMA_IRQ0            (1<<0)

//AXIDMA_ARM_ACK_STAT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
#define ARM_AXIDMA_ACK23           (1<<23)
#define ARM_AXIDMA_ACK22           (1<<22)
#define ARM_AXIDMA_ACK21           (1<<21)
#define ARM_AXIDMA_ACK20           (1<<20)
#define ARM_AXIDMA_ACK19           (1<<19)
#define ARM_AXIDMA_ACK18           (1<<18)
#define ARM_AXIDMA_ACK17           (1<<17)
#define ARM_AXIDMA_ACK16           (1<<16)
#define ARM_AXIDMA_ACK15           (1<<15)
#define ARM_AXIDMA_ACK14           (1<<14)
#define ARM_AXIDMA_ACK13           (1<<13)
#define ARM_AXIDMA_ACK12           (1<<12)
#define ARM_AXIDMA_ACK11           (1<<11)
#define ARM_AXIDMA_ACK10           (1<<10)
#define ARM_AXIDMA_ACK9            (1<<9)
#define ARM_AXIDMA_ACK8            (1<<8)
#define ARM_AXIDMA_ACK7            (1<<7)
#define ARM_AXIDMA_ACK6            (1<<6)
#define ARM_AXIDMA_ACK5            (1<<5)
#define ARM_AXIDMA_ACK4            (1<<4)
#define ARM_AXIDMA_ACK3            (1<<3)
#define ARM_AXIDMA_ACK2            (1<<2)
#define ARM_AXIDMA_ACK1            (1<<1)
#define ARM_AXIDMA_ACK0            (1<<0)

//AXIDMA_ZSP_REQ_STAT0
#define ARM_AXIDMA_RESERVED1_0     (1<<31)
#define ARM_AXIDMA_REQ7(n)         (((n)&7)<<28)
#define ARM_AXIDMA_RESERVED2_1     (1<<27)
#define ARM_AXIDMA_REQ6(n)         (((n)&7)<<24)
#define ARM_AXIDMA_RESERVED3_2     (1<<23)
#define ARM_AXIDMA_REQ5(n)         (((n)&7)<<20)
#define ARM_AXIDMA_RESERVED4_3     (1<<19)
#define ARM_AXIDMA_REQ4(n)         (((n)&7)<<16)
#define ARM_AXIDMA_RESERVED5_4     (1<<15)
#define ARM_AXIDMA_REQ3(n)         (((n)&7)<<12)
#define ARM_AXIDMA_RESERVED6_5     (1<<11)
#define ARM_AXIDMA_REQ2(n)         (((n)&7)<<8)
#define ARM_AXIDMA_RESERVED7_6     (1<<7)
#define ARM_AXIDMA_REQ1(n)         (((n)&7)<<4)
#define ARM_AXIDMA_RESERVED8_7     (1<<3)
#define ARM_AXIDMA_REQ0(n)         (((n)&7)<<0)

//AXIDMA_ZSP_REQ_STAT1
#define ARM_AXIDMA_RESERVED1_0_X1(n) (((n)&0x1FFFF)<<15)
#define ARM_AXIDMA_REQ11(n)        (((n)&7)<<12)
#define ARM_AXIDMA_RESERVED2_1_X1  (1<<11)
#define ARM_AXIDMA_REQ10(n)        (((n)&7)<<8)
#define ARM_AXIDMA_RESERVED3_2_X1  (1<<7)
#define ARM_AXIDMA_REQ9(n)         (((n)&7)<<4)
#define ARM_AXIDMA_RESERVED4_3_X1  (1<<3)
#define ARM_AXIDMA_REQ8(n)         (((n)&7)<<0)

//AXIDMA_CH_IRQ_DISTR
//#define ARM_AXIDMA_RESERVED_0_X3(n) (((n)&0xFFFFF)<<12)
#define ARM_AXIDMA_CH11_IRQ_EN0    (1<<11)
#define ARM_AXIDMA_CH10_IRQ_EN0    (1<<10)
#define ARM_AXIDMA_CH9_IRQ_EN0     (1<<9)
#define ARM_AXIDMA_CH8_IRQ_EN0     (1<<8)
#define ARM_AXIDMA_CH7_IRQ_EN0     (1<<7)
#define ARM_AXIDMA_CH6_IRQ_EN0     (1<<6)
#define ARM_AXIDMA_CH5_IRQ_EN0     (1<<5)
#define ARM_AXIDMA_CH4_IRQ_EN0     (1<<4)
#define ARM_AXIDMA_CH3_IRQ_EN0     (1<<3)
#define ARM_AXIDMA_CH2_IRQ_EN0     (1<<2)
#define ARM_AXIDMA_CH1_IRQ_EN0     (1<<1)
#define ARM_AXIDMA_CH0_IRQ_EN0     (1<<0)

//AXIDMA_C0_CONF
#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
#define ARM_AXIDMA_ZSP_DMA_ACK_EN  (1<<11)
#define ARM_AXIDMA_COUNT_SEL       (1<<10)
#define ARM_AXIDMA_REQ_SEL         (1<<9)
#define ARM_AXIDMA_FORCE_TRANS     (1<<8)
#define ARM_AXIDMA_DADDR_FIX       (1<<7)
#define ARM_AXIDMA_SADDR_FIX       (1<<6)
#define ARM_AXIDMA_IRQ_T           (1<<5)
#define ARM_AXIDMA_IRQ_F           (1<<4)
#define ARM_AXIDMA_SYN_IRQ         (1<<3)
#define ARM_AXIDMA_DATA_TYPE(n)    (((n)&3)<<1)
#define ARM_AXIDMA_START           (1<<0)

//AXIDMA_C0_MAP
#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
#define ARM_AXIDMA_ACK_MAP(n)      (((n)&31)<<8)
#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
#define ARM_AXIDMA_REQ_SOURCE(n)   (((n)&31)<<0)

//AXIDMA_C0_SADDR
#define ARM_AXIDMA_S_ADDR(n)       (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C0_DADDR
#define ARM_AXIDMA_D_ADDR(n)       (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C0_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
#define ARM_AXIDMA_COUNT(n)        (((n)&0xFFFFFF)<<0)

//AXIDMA_C0_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
#define ARM_AXIDMA_COUNTP(n)       (((n)&0xFFFF)<<0)

//AXIDMA_C0_STATUS
#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
#define ARM_AXIDMA_SG_SUSPEND_STA  (1<<24)
#define ARM_AXIDMA_SG_FINISH_STA   (1<<23)
#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
#define ARM_AXIDMA_SG_SUSPEND      (1<<20)
#define ARM_AXIDMA_SG_COUNT(n)     (((n)&0xFFFF)<<4)
#define ARM_AXIDMA_SG_FINISH       (1<<3)
#define ARM_AXIDMA_COUNTP_FINISH   (1<<2)
#define ARM_AXIDMA_COUNT_FINISH    (1<<1)
#define ARM_AXIDMA_RUN             (1<<0)

//AXIDMA_C0_SGADDR
#define ARM_AXIDMA_SG_ADDR(n)      (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C0_SGCONF
#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
#define ARM_AXIDMA_SG_NUM(n)       (((n)&0xFFFF)<<4)
#define ARM_AXIDMA_DESC_RD_CTRL    (1<<3)
#define ARM_AXIDMA_SG_SUSPEND_IE   (1<<2)
#define ARM_AXIDMA_SG_FINISH_IE    (1<<1)
#define ARM_AXIDMA_SG_EN           (1<<0)

//AXIDMA_C0_SET
#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
#define ARM_AXIDMA_RUN_SET         (1<<0)

//AXIDMA_C0_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
#define ARM_AXIDMA_RUN_CLR         (1<<0)

//AXIDMA_C1_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C1_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C1_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C1_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C1_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C1_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C1_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C1_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C1_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C1_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C1_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C2_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C2_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C2_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C2_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C2_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C2_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C2_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C2_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C2_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C2_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C2_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C3_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C3_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C3_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C3_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C3_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C3_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C3_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C3_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C3_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C3_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C3_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C4_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C4_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C4_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C4_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C4_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C4_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C4_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C4_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C4_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C4_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C4_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C5_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C5_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C5_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C5_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C5_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C5_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C5_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C5_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C5_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C5_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C5_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C6_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C6_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C6_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C6_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C6_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C6_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C6_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C6_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C6_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C6_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C6_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C7_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C7_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C7_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C7_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C7_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C7_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C7_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C7_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C7_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C7_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C7_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C8_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C8_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C8_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C8_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C8_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C8_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C8_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C8_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C8_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C8_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C8_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C9_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C9_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C9_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C9_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C9_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C9_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C9_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C9_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C9_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C9_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C9_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C10_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C10_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C10_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C10_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C10_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C10_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C10_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C10_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C10_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C10_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C10_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)

//AXIDMA_C11_CONF
//#define ARM_AXIDMA_RESERVED_0_X5(n) (((n)&0x3FFFF)<<14)
//#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
//#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
//#define ARM_AXIDMA_ZSP_DMA_ACK_EN (1<<11)
//#define ARM_AXIDMA_COUNT_SEL     (1<<10)
//#define ARM_AXIDMA_REQ_SEL       (1<<9)
//#define ARM_AXIDMA_FORCE_TRANS   (1<<8)
//#define ARM_AXIDMA_DADDR_FIX     (1<<7)
//#define ARM_AXIDMA_SADDR_FIX     (1<<6)
//#define ARM_AXIDMA_IRQ_T         (1<<5)
//#define ARM_AXIDMA_IRQ_F         (1<<4)
//#define ARM_AXIDMA_SYN_IRQ       (1<<3)
//#define ARM_AXIDMA_DATA_TYPE(n)  (((n)&3)<<1)
//#define ARM_AXIDMA_START         (1<<0)

//AXIDMA_C11_MAP
//#define ARM_AXIDMA_RESERVED1_0_X2(n) (((n)&0x7FFFF)<<13)
//#define ARM_AXIDMA_ACK_MAP(n)    (((n)&31)<<8)
//#define ARM_AXIDMA_RESERVED2_1_X2(n) (((n)&7)<<5)
//#define ARM_AXIDMA_REQ_SOURCE(n) (((n)&31)<<0)

//AXIDMA_C11_SADDR
//#define ARM_AXIDMA_S_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C11_DADDR
//#define ARM_AXIDMA_D_ADDR(n)     (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C11_COUNT
//#define ARM_AXIDMA_RESERVED_0_X4(n) (((n)&0xFF)<<24)
//#define ARM_AXIDMA_COUNT(n)      (((n)&0xFFFFFF)<<0)

//AXIDMA_C11_COUNTP
//#define ARM_AXIDMA_RESERVED_0_X1(n) (((n)&0xFFFF)<<16)
//#define ARM_AXIDMA_COUNTP(n)     (((n)&0xFFFF)<<0)

//AXIDMA_C11_STATUS
//#define ARM_AXIDMA_RESERVED_0_X6(n) (((n)&0x7F)<<25)
//#define ARM_AXIDMA_SG_SUSPEND_STA (1<<24)
//#define ARM_AXIDMA_SG_FINISH_STA (1<<23)
//#define ARM_AXIDMA_COUNTP_FINISH_STA (1<<22)
//#define ARM_AXIDMA_COUNT_FINISH_STA (1<<21)
//#define ARM_AXIDMA_SG_SUSPEND    (1<<20)
//#define ARM_AXIDMA_SG_COUNT(n)   (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_SG_FINISH     (1<<3)
//#define ARM_AXIDMA_COUNTP_FINISH (1<<2)
//#define ARM_AXIDMA_COUNT_FINISH  (1<<1)
//#define ARM_AXIDMA_RUN           (1<<0)

//AXIDMA_C11_SGADDR
//#define ARM_AXIDMA_SG_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//AXIDMA_C11_SGCONF
//#define ARM_AXIDMA_RESERVED_0_X7(n) (((n)&0xFFF)<<20)
//#define ARM_AXIDMA_SG_NUM(n)     (((n)&0xFFFF)<<4)
//#define ARM_AXIDMA_DESC_RD_CTRL  (1<<3)
//#define ARM_AXIDMA_SG_SUSPEND_IE (1<<2)
//#define ARM_AXIDMA_SG_FINISH_IE  (1<<1)
//#define ARM_AXIDMA_SG_EN         (1<<0)

//AXIDMA_C11_SET
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_SET       (1<<0)

//AXIDMA_C11_CLR
//#define ARM_AXIDMA_RESERVED_0_X8(n) (((n)&0x7FFFFFFF)<<1)
//#define ARM_AXIDMA_RUN_CLR       (1<<0)





#endif

