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


#ifndef _AON_IFC_H_
#define _AON_IFC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'aon_ifc'."
#endif


#include "globals.h"
#include "gallite_generic_config.h"

// =============================================================================
//  MACROS
// =============================================================================
#define AON_IFC_ADDR_ALIGN                      (0)
#define AON_IFC_TC_LEN                          (23)
#define AON_IFC_STD_CHAN_NB                     (AON_IFC_NB_STD_CHANNEL)
#define AON_IFC_RFSPI_CHAN                      (0)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// AON_IFC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_AON_IFC_BASE           0x0510A000
#else
#define REG_AON_IFC_BASE           0x5010A000
#endif

typedef volatile struct
{
    REG32                          get_ch;                       //0x00000000
    REG32                          dma_status;                   //0x00000004
    REG32                          debug_status;                 //0x00000008
    REG32 Reserved_0000000C;                    //0x0000000C
    struct
    {
        REG32                      control;                      //0x00000010
        REG32                      status;                       //0x00000014
        REG32                      start_addr;                   //0x00000018
        REG32                      tc;                           //0x0000001C
    } std_ch[AON_IFC_STD_CHAN_NB];
    REG32                          ch_rfspi_control;             //0x00000030
    REG32                          ch_rfspi_status;              //0x00000034
    REG32                          ch_rfspi_start_addr;          //0x00000038
    REG32                          ch_rfspi_end_addr;            //0x0000003C
    REG32                          ch_rfspi_tc;                  //0x00000040
    REG32 Reserved_00000044[47];                //0x00000044
} HWP_AON_IFC_T;

#define hwp_aonIfc                 ((HWP_AON_IFC_T*) REG_ACCESS_ADDRESS(REG_AON_IFC_BASE))


//get_ch
typedef union {
    REG32 v;
    struct {
        REG32 ch_to_use : 4; // [3:0], read only
        REG32 __31_4 : 28;
    } b;
} REG_AON_IFC_GET_CH_T;

//dma_status
typedef union {
    REG32 v;
    struct {
        REG32 ch_enable : 2; // [1:0], read only
        REG32 __15_2 : 14;
        REG32 ch_busy : 2; // [17:16], read only
        REG32 __31_18 : 14;
    } b;
} REG_AON_IFC_DMA_STATUS_T;

//debug_status
typedef union {
    REG32 v;
    struct {
        REG32 dbg_status : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_AON_IFC_DEBUG_STATUS_T;

//control
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0]
        REG32 disable : 1; // [1]
        REG32 rd_hw_exch : 1; // [2]
        REG32 wr_hw_exch : 1; // [3]
        REG32 autodisable : 1; // [4]
        REG32 size : 1; // [5]
        REG32 __7_6 : 2;
        REG32 req_src : 5; // [12:8]
        REG32 __15_13 : 3;
        REG32 flush : 1; // [16]
        REG32 __31_17 : 15;
    } b;
} REG_AON_IFC_CONTROL_T;

//status
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0], read only
        REG32 __3_1 : 3;
        REG32 fifo_empty : 1; // [4], read only
        REG32 __31_5 : 27;
    } b;
} REG_AON_IFC_STATUS_T;

//tc
typedef union {
    REG32 v;
    struct {
        REG32 tc : 23; // [22:0]
        REG32 __31_23 : 9;
    } b;
} REG_AON_IFC_TC_T;

//ch_rfspi_control
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0], write set
        REG32 disable : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_AON_IFC_CH_RFSPI_CONTROL_T;

//ch_rfspi_status
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0], read only
        REG32 __3_1 : 3;
        REG32 fifo_empty : 1; // [4], read only
        REG32 __7_5 : 3;
        REG32 fifo_level : 5; // [12:8], read only
        REG32 __31_13 : 19;
    } b;
} REG_AON_IFC_CH_RFSPI_STATUS_T;

//ch_rfspi_tc
typedef union {
    REG32 v;
    struct {
        REG32 tc : 14; // [13:0]
        REG32 __31_14 : 18;
    } b;
} REG_AON_IFC_CH_RFSPI_TC_T;


//get_ch
#define AON_IFC_CH_TO_USE(n)       (((n)&15)<<0)
#define AON_IFC_CH_TO_USE_MASK     (15<<0)
#define AON_IFC_CH_TO_USE_SHIFT    (0)

//dma_status
#define AON_IFC_CH_ENABLE(n)       (((n)&3)<<0)
#define AON_IFC_CH_BUSY(n)         (((n)&3)<<16)

//debug_status
#define AON_IFC_DBG_STATUS         (1<<0)

//control
#define AON_IFC_ENABLE             (1<<0)
#define AON_IFC_DISABLE            (1<<1)
#define AON_IFC_RD_HW_EXCH         (1<<2)
#define AON_IFC_WR_HW_EXCH         (1<<3)
#define AON_IFC_AUTODISABLE        (1<<4)
#define AON_IFC_SIZE               (1<<5)
#define AON_IFC_REQ_SRC(n)         (((n)&31)<<8)
#define AON_IFC_REQ_SRC_MASK       (31<<8)
#define AON_IFC_REQ_SRC_SHIFT      (8)
#define AON_IFC_REQ_SRC_AON_APB_DMA_ID_TX_DEBUG_UART (0<<8)
#define AON_IFC_REQ_SRC_AON_APB_DMA_ID_RX_DEBUG_UART (1<<8)
#define AON_IFC_REQ_SRC_AON_APB_DMA_ID_RX_VAD (5<<8)
#define AON_IFC_REQ_SRC_V_AON_APB_DMA_ID_TX_DEBUG_UART (0)
#define AON_IFC_REQ_SRC_V_AON_APB_DMA_ID_RX_DEBUG_UART (1)
#define AON_IFC_REQ_SRC_V_AON_APB_DMA_ID_RX_VAD (5)
#define AON_IFC_FLUSH              (1<<16)

//status
//#define AON_IFC_ENABLE           (1<<0)
#define AON_IFC_FIFO_EMPTY         (1<<4)

//start_addr
#define AON_IFC_START_ADDR(n)      (((n)&0xFFFFFFFF)<<0)

//tc
#define AON_IFC_TC(n)              (((n)&0x7FFFFF)<<0)

//ch_rfspi_control
//#define AON_IFC_ENABLE           (1<<0)
//#define AON_IFC_DISABLE          (1<<1)

//ch_rfspi_status
//#define AON_IFC_ENABLE           (1<<0)
//#define AON_IFC_FIFO_EMPTY       (1<<4)
#define AON_IFC_FIFO_LEVEL(n)      (((n)&31)<<8)

//ch_rfspi_start_addr
#define AON_IFC_START_AHB_ADDR(n)  (((n)&0xFFFFFFFF)<<0)

//ch_rfspi_end_addr
#define AON_IFC_END_AHB_ADDR(n)    (((n)&0xFFFFFFFF)<<0)

//ch_rfspi_tc
#define AON_IFC_TC_X1(n)           (((n)&0x3FFF)<<0)





#endif

