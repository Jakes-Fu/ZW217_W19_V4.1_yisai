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
#define REG_DMA_BASE               0x020C0000

typedef volatile struct
{
    REG32                          axidma_conf;                  //0x00000000
    REG32                          axidma_delay;                 //0x00000004
    REG32                          axidma_status;                //0x00000008
    REG32                          axidma_irq_stat;              //0x0000000C
    REG32                          axidma_arm_req_stat;          //0x00000010
    REG32                          axidma_arm_ack_stat;          //0x00000014
    REG32                          axidma_zsp_req_stat0;         //0x00000018
    REG32                          axidma_zsp_req_stat1;         //0x0000001C
    REG32                          axidma_ch_irq_distr;          //0x00000020
    REG32 Reserved_00000024[7];                 //0x00000024
    struct
    {
        REG32                      conf;                         //0x00000040
        REG32                      map;                          //0x00000044
        REG32                      saddr;                        //0x00000048
        REG32                      daddr;                        //0x0000004C
        REG32                      count;                        //0x00000050
        REG32                      countp;                       //0x00000054
        REG32                      status;                       //0x00000058
        REG32                      sgaddr;                       //0x0000005C
        REG32                      sgconf;                       //0x00000060
        REG32 Reserved_00000024[7];             //0x00000024
    } axidma_ch[12];
} HWP_ARM_AXIDMA_T;

#define hwp_dma                    ((HWP_ARM_AXIDMA_T*) REG_ACCESS_ADDRESS(REG_DMA_BASE))


//axidma_conf
#define ARM_AXIDMA_STOP_NOT_STOP   (0<<0)
#define ARM_AXIDMA_STOP_STOP       (1<<0)
#define ARM_AXIDMA_STOP_IE         (1<<1)
#define ARM_AXIDMA_PRIORITY        (1<<2)
#define ARM_AXIDMA_OUTSTAND_OUTSTAND_2 (0<<3)
#define ARM_AXIDMA_OUTSTAND_OUTSTAND_3 (1<<3)
#define ARM_AXIDMA_OUTSTAND_OUTSTAND_4 (2<<3)
#define ARM_AXIDMA_MODE_SEL_ARM_MODE (0<<5)
#define ARM_AXIDMA_MODE_SEL_ZSP_MODE (1<<5)

//axidma_delay
#define ARM_AXIDMA_DELAY(n)        (((n)&0xFFFF)<<0)

//axidma_status
#define ARM_AXIDMA_CH_NUM_CH0_FINISH (0<<0)
#define ARM_AXIDMA_CH_NUM_CH1_FINISH (1<<0)
#define ARM_AXIDMA_CH_NUM_CH11_FINISH (11<<0)
#define ARM_AXIDMA_STOP_STATUS     (1<<4)

//axidma_irq_stat
#define ARM_AXIDMA_CH0_IRQ         (1<<0)
#define ARM_AXIDMA_CH1_IRQ         (1<<1)
#define ARM_AXIDMA_CH2_IRQ         (1<<2)
#define ARM_AXIDMA_CH3_IRQ         (1<<3)
#define ARM_AXIDMA_CH4_IRQ         (1<<4)
#define ARM_AXIDMA_CH5_IRQ         (1<<5)
#define ARM_AXIDMA_CH6_IRQ         (1<<6)
#define ARM_AXIDMA_CH7_IRQ         (1<<7)
#define ARM_AXIDMA_CH8_IRQ         (1<<8)
#define ARM_AXIDMA_CH9_IRQ         (1<<9)
#define ARM_AXIDMA_CH10_IRQ        (1<<10)
#define ARM_AXIDMA_CH11_IRQ        (1<<11)

//axidma_arm_req_stat
#define ARM_AXIDMA_IRQ0            (1<<0)
#define ARM_AXIDMA_IRQ1            (1<<1)
#define ARM_AXIDMA_IRQ2            (1<<2)
#define ARM_AXIDMA_IRQ3            (1<<3)
#define ARM_AXIDMA_IRQ4            (1<<4)
#define ARM_AXIDMA_IRQ5            (1<<5)
#define ARM_AXIDMA_IRQ6            (1<<6)
#define ARM_AXIDMA_IRQ7            (1<<7)
#define ARM_AXIDMA_IRQ8            (1<<8)
#define ARM_AXIDMA_IRQ9            (1<<9)
#define ARM_AXIDMA_IRQ10           (1<<10)
#define ARM_AXIDMA_IRQ11           (1<<11)
#define ARM_AXIDMA_IRQ12           (1<<12)
#define ARM_AXIDMA_IRQ13           (1<<13)
#define ARM_AXIDMA_IRQ14           (1<<14)
#define ARM_AXIDMA_IRQ15           (1<<15)
#define ARM_AXIDMA_IRQ16           (1<<16)
#define ARM_AXIDMA_IRQ17           (1<<17)
#define ARM_AXIDMA_IRQ18           (1<<18)
#define ARM_AXIDMA_IRQ19           (1<<19)
#define ARM_AXIDMA_IRQ20           (1<<20)
#define ARM_AXIDMA_IRQ21           (1<<21)
#define ARM_AXIDMA_IRQ22           (1<<22)
#define ARM_AXIDMA_IRQ23           (1<<23)

//axidma_arm_ack_stat
#define ARM_AXIDMA_ACK0            (1<<0)
#define ARM_AXIDMA_ACK1            (1<<1)
#define ARM_AXIDMA_ACK2            (1<<2)
#define ARM_AXIDMA_ACK3            (1<<3)
#define ARM_AXIDMA_ACK4            (1<<4)
#define ARM_AXIDMA_ACK5            (1<<5)
#define ARM_AXIDMA_ACK6            (1<<6)
#define ARM_AXIDMA_ACK7            (1<<7)
#define ARM_AXIDMA_ACK8            (1<<8)
#define ARM_AXIDMA_ACK9            (1<<9)
#define ARM_AXIDMA_ACK10           (1<<10)
#define ARM_AXIDMA_ACK11           (1<<11)
#define ARM_AXIDMA_ACK12           (1<<12)
#define ARM_AXIDMA_ACK13           (1<<13)
#define ARM_AXIDMA_ACK14           (1<<14)
#define ARM_AXIDMA_ACK15           (1<<15)
#define ARM_AXIDMA_ACK16           (1<<16)
#define ARM_AXIDMA_ACK17           (1<<17)
#define ARM_AXIDMA_ACK18           (1<<18)
#define ARM_AXIDMA_ACK19           (1<<19)
#define ARM_AXIDMA_ACK20           (1<<20)
#define ARM_AXIDMA_ACK21           (1<<21)
#define ARM_AXIDMA_ACK22           (1<<22)
#define ARM_AXIDMA_ACK23           (1<<23)

//axidma_zsp_req_stat0
#define ARM_AXIDMA_REQ0_NOT_MOVE_NUM (0<<0)
#define ARM_AXIDMA_REQ0_MOV_NUM_ONCE (1<<0)
#define ARM_AXIDMA_REQ0_MOV_NUM_TWICE (2<<0)
#define ARM_AXIDMA_REQ0_MOV_NUM_7_TIMES (7<<0)
#define ARM_AXIDMA_REQ1_NOT_MOVE_NUM (0<<4)
#define ARM_AXIDMA_REQ1_MOV_NUM_ONCE (1<<4)
#define ARM_AXIDMA_REQ1_MOV_NUM_TWICE (2<<4)
#define ARM_AXIDMA_REQ1_MOV_NUM_7_TIMES (7<<4)
#define ARM_AXIDMA_REQ2_NOT_MOVE_NUM (0<<8)
#define ARM_AXIDMA_REQ2_MOV_NUM_ONCE (1<<8)
#define ARM_AXIDMA_REQ2_MOV_NUM_TWICE (2<<8)
#define ARM_AXIDMA_REQ2_MOV_NUM_7_TIMES (7<<8)
#define ARM_AXIDMA_REQ3_NOT_MOVE_NUM (0<<12)
#define ARM_AXIDMA_REQ3_MOV_NUM_ONCE (1<<12)
#define ARM_AXIDMA_REQ3_MOV_NUM_TWICE (2<<12)
#define ARM_AXIDMA_REQ3_MOV_NUM_7_TIMES (7<<12)
#define ARM_AXIDMA_REQ4_NOT_MOVE_NUM (0<<16)
#define ARM_AXIDMA_REQ4_MOV_NUM_ONCE (1<<16)
#define ARM_AXIDMA_REQ4_MOV_NUM_TWICE (2<<16)
#define ARM_AXIDMA_REQ4_MOV_NUM_7_TIMES (7<<16)
#define ARM_AXIDMA_REQ5_NOT_MOVE_NUM (0<<20)
#define ARM_AXIDMA_REQ5_MOV_NUM_ONCE (1<<20)
#define ARM_AXIDMA_REQ5_MOV_NUM_TWICE (2<<20)
#define ARM_AXIDMA_REQ5_MOV_NUM_7_TIMES (7<<20)
#define ARM_AXIDMA_REQ6_NOT_MOVE_NUM (0<<24)
#define ARM_AXIDMA_REQ6_MOV_NUM_ONCE (1<<24)
#define ARM_AXIDMA_REQ6_MOV_NUM_TWICE (2<<24)
#define ARM_AXIDMA_REQ6_MOV_NUM_7_TIMES (7<<24)
#define ARM_AXIDMA_REQ7_NOT_MOVE_NUM (0<<28)
#define ARM_AXIDMA_REQ7_MOV_NUM_ONCE (1<<28)
#define ARM_AXIDMA_REQ7_MOV_NUM_TWICE (2<<28)
#define ARM_AXIDMA_REQ7_MOV_NUM_7_TIMES (7<<28)

//axidma_zsp_req_stat1
#define ARM_AXIDMA_REQ8_NOT_MOVE_NUM (0<<0)
#define ARM_AXIDMA_REQ8_MOV_NUM_ONCE (1<<0)
#define ARM_AXIDMA_REQ8_MOV_NUM_TWICE (2<<0)
#define ARM_AXIDMA_REQ8_MOV_NUM_7_TIMES (7<<0)
#define ARM_AXIDMA_REQ9_NOT_MOVE_NUM (0<<4)
#define ARM_AXIDMA_REQ9_MOV_NUM_ONCE (1<<4)
#define ARM_AXIDMA_REQ9_MOV_NUM_TWICE (2<<4)
#define ARM_AXIDMA_REQ9_MOV_NUM_7_TIMES (7<<4)
#define ARM_AXIDMA_REQ10_NOT_MOVE_NUM (0<<8)
#define ARM_AXIDMA_REQ10_MOV_NUM_ONCE (1<<8)
#define ARM_AXIDMA_REQ10_MOV_NUM_TWICE (2<<8)
#define ARM_AXIDMA_REQ10_MOV_NUM_7_TIMES (7<<8)
#define ARM_AXIDMA_REQ11_NOT_MOVE_NUM (0<<12)
#define ARM_AXIDMA_REQ11_MOV_NUM_ONCE (1<<12)
#define ARM_AXIDMA_REQ11_MOV_NUM_TWICE (2<<12)
#define ARM_AXIDMA_REQ11_MOV_NUM_7_TIMES (7<<12)

//conf
#define ARM_AXIDMA_START_STOP      (0<<0)
#define ARM_AXIDMA_START_START     (1<<0)
#define ARM_AXIDMA_DATA_TYPE_BYTE  (0<<1)
#define ARM_AXIDMA_DATA_TYPE_HALF_WORD (1<<1)
#define ARM_AXIDMA_DATA_TYPE_WORD  (2<<1)
#define ARM_AXIDMA_DATA_TYPE_DWORD (3<<1)
#define ARM_AXIDMA_SYN_IRQ         (1<<3)
#define ARM_AXIDMA_IRQ_F           (1<<4)
#define ARM_AXIDMA_IRQ_T           (1<<5)
#define ARM_AXIDMA_SADDR_FIX       (1<<6)
#define ARM_AXIDMA_DADDR_FIX       (1<<7)
#define ARM_AXIDMA_FORCE_TRANS     (1<<8)
#define ARM_AXIDMA_SEQ_SEL         (1<<9)
#define ARM_AXIDMA_COUNT_SEL_MOV_1_COUNT (0<<10)
#define ARM_AXIDMA_COUNT_SEL_MOV_ALL_COUNT (1<<10)
#define ARM_AXIDMA_ZSP_DMA_ACK_EN  (1<<11)
#define ARM_AXIDMA_SADDR_TURNAROUND (1<<12)
#define ARM_AXIDMA_DADDR_TURNAROUND (1<<13)
#define ARM_AXIDMA_SECURITY_EN  (1<<14)
#define ARM_AXIDMA_ERR_INT_EN   (1<<15)

//map
#define ARM_AXIDMA_REQ_SOURCE_IRQ0 (0<<0)
#define ARM_AXIDMA_REQ_SOURCE_IRQ1 (1<<0)
#define ARM_AXIDMA_REQ_SOURCE_IRQ2 (2<<0)
#define ARM_AXIDMA_REQ_SOURCE_IRQ23 (23<<0)
#define ARM_AXIDMA_ACK_MAP_ACK0    (0<<8)
#define ARM_AXIDMA_ACK_MAP_ACK1    (1<<8)
#define ARM_AXIDMA_ACK_MAP_ACK2    (2<<8)
#define ARM_AXIDMA_ACK_MAP_ACK23   (23<<8)

//saddr
#define ARM_AXIDMA_S_ADDR(n)       (((n)&0xFFFFFFFF)<<0)

//daddr
#define ARM_AXIDMA_D_ADDR(n)       (((n)&0xFFFFFFFF)<<0)

//count
#define ARM_AXIDMA_COUNT(n)        (((n)&0xFFFFFF)<<0)

//countp
#define ARM_AXIDMA_COUNTP(n)       (((n)&0xFFFF)<<0)

//status
#define ARM_AXIDMA_RUN_IDLE        (0<<0)
#define ARM_AXIDMA_RUN_BUSY        (1<<0)
#define ARM_AXIDMA_COUNT_FINISH_NOT_FINISH (0<<1)
#define ARM_AXIDMA_COUNT_FINISH_FINISH (1<<1)
#define ARM_AXIDMA_COUNTP_FINISH_NOT_FINISH (0<<2)
#define ARM_AXIDMA_COUNTP_FINISH_FINISH (1<<2)
#define ARM_AXIDMA_SG_FINISH_NOT_FINISH (0<<3)
#define ARM_AXIDMA_SG_FINISH_FINISH (1<<3)
#define ARM_AXIDMA_SG_COUNT_COUNT_0 (0x0<<4)
#define ARM_AXIDMA_SG_COUNT_COUNT_1 (0x1<<4)
#define ARM_AXIDMA_SG_COUNT_COUNT_5 (0x5<<4)
#define ARM_AXIDMA_SG_COUNT_COUNT_65535 (0xFFFF<<4)
#define ARM_AXIDMA_SG_SUSPEND      (1<<20)

//sgaddr
#define ARM_AXIDMA_SG_ADDR(n)      (((n)&0xFFFFFFFF)<<0)

//sgconf
#define ARM_AXIDMA_SG_EN_DISABLE   (0<<0)
#define ARM_AXIDMA_SG_EN_ENABLE    (1<<0)
#define ARM_AXIDMA_SG_FINISH_IE_DISABLE (0<<1)
#define ARM_AXIDMA_SG_FINISH_IE_ENABLE (1<<1)
#define ARM_AXIDMA_SG_SUSPEND_IE_DISABLE (0<<2)
#define ARM_AXIDMA_SG_SUSPEND_IE_ENABLE (1<<2)
#define ARM_AXIDMA_DESC_RD_CTRL    (1<<3)
#define ARM_AXIDMA_SG_NUM(n)       (((n)&0xFFFF)<<4)





#endif

