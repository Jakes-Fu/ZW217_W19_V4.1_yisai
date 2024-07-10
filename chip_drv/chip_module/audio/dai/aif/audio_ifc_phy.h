/******************************************************************************
 ** File Name:      audio_ifc_phy.h                                           *
 ** Author:         jen.li                                                    *
 ** DATE:           6/15/2022                                                 *
 ** Copyright:      Copyright (c) 2022 Unisoc Communications Inc.             *
 ** Description:    Intelligent Flow Controller                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME           DESCRIPTION                                 *
 ** 6/15/2022     jen.li          Create.                                     *
 *****************************************************************************/
#ifndef _AUD_IFC_PHY_H_
#define _AUD_IFC_PHY_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#include "audio_drvapi.h"


/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
// direction flag
#define AUDIO_IFC_DIRECTION_NUM 2
#define AUDIO_IFC_PLAYBACK  1
#define AUDIO_IFC_CAPTURE   2

#define REG_AUDIO_IFC_BASE 0x5140E100

#define REG_AUD_IFC_CH0_CONTROL         (REG_AUDIO_IFC_BASE + 0x0000)
#define REG_AUD_IFC_CH0_STATUS          (REG_AUDIO_IFC_BASE + 0x0004)
#define REG_AUD_IFC_CH0_STARTADDR       (REG_AUDIO_IFC_BASE + 0x0008)
#define REG_AUD_IFC_CH0_FIFOSIZE        (REG_AUDIO_IFC_BASE + 0x000C)
#define REG_AUD_IFC_CH0_INTMASK         (REG_AUDIO_IFC_BASE + 0x0014)
#define REG_AUD_IFC_CH0_INTCLEAR        (REG_AUDIO_IFC_BASE + 0x0018)
#define REG_AUD_IFC_CH0_CURAHBADDR      (REG_AUDIO_IFC_BASE + 0x001C)

#define REG_AUD_IFC_CH1_CONTROL         (REG_AUDIO_IFC_BASE + 0x0020)
#define REG_AUD_IFC_CH1_STATUS          (REG_AUDIO_IFC_BASE + 0x0024)
#define REG_AUD_IFC_CH1_STARTADDR       (REG_AUDIO_IFC_BASE + 0x0028)
#define REG_AUD_IFC_CH1_FIFOSIZE        (REG_AUDIO_IFC_BASE + 0x002C)
#define REG_AUD_IFC_CH1_INTMASK         (REG_AUDIO_IFC_BASE + 0x0034)
#define REG_AUD_IFC_CH1_INTCLEAR        (REG_AUDIO_IFC_BASE + 0x0038)
#define REG_AUD_IFC_CH1_CURAHBADDR      (REG_AUDIO_IFC_BASE + 0x003C)

// status
//#define AUDIO_IFC_ENABLE         (1<<0)
#define AUDIO_IFC_FIFO_EMPTY       (1<<4)
#define AUDIO_IFC_CAUSE_IEF        (1<<8)
#define AUDIO_IFC_CAUSE_IHF        (1<<9)
#define AUDIO_IFC_CAUSE_I4F        (1<<10)
#define AUDIO_IFC_CAUSE_I3_4F      (1<<11)
#define AUDIO_IFC_CAUSE_AHB_ERROR  (1<<12)
#define AUDIO_IFC_IEF              (1<<16)
#define AUDIO_IFC_IHF              (1<<17)
#define AUDIO_IFC_I4F              (1<<18)
#define AUDIO_IFC_I3_4F            (1<<19)
#define AUDIO_IFC_AHB_ERROR        (1<<20)
#define AUDIO_IFC_CH_IDLE          (1<<21)

//int_mask
#define AUDIO_IFC_END_FIFO         (1<<8)
#define AUDIO_IFC_HALF_FIFO        (1<<9)
#define AUDIO_IFC_QUARTER_FIFO     (1<<10)
#define AUDIO_IFC_THREE_QUARTER_FIFO (1<<11)
#define AUDIO_IFC_AHB_ERROR_X1     (1<<12)

// Intelligent Flow Controller registers
// control 0x00000000
typedef union {
    volatile unsigned int dwValue;
    struct
    {
        volatile unsigned int enable : 1;       // [0]
        volatile unsigned int disable : 1;      // [1]
        volatile unsigned int reserve_3_2 : 2;  // [3:2]
        volatile unsigned int auto_disable : 1; // [4]
        volatile unsigned int reserve_31_5 : 27;// [31:5]
    }mBits;
} AUDIO_IFC_CONTROL_U;

// status 0x00000004
typedef union {
    volatile unsigned int dwValue;
    struct
    {
        volatile unsigned int enable : 1;       // [0], read only
        volatile unsigned int reserve_3_1 : 3;  // [3:1]
        volatile unsigned int fifo_empty : 1;   // [4], read only
        volatile unsigned int reserve_7_5 : 3;  // [7:5]
        volatile unsigned int cause_ief : 1;    // [8], read only , ifc end fifo
        volatile unsigned int cause_ihf : 1;    // [9], read only , ifc half fifo
        volatile unsigned int cause_i4f : 1;    // [10], read only, 1/4 fifo
        volatile unsigned int cause_i3_4f : 1;  // [11], read only, 3/4 fifo
        volatile unsigned int cause_ahb_error:1;    // [12], read only
        volatile unsigned int reserve_15_13 : 3;// [15:13]
        volatile unsigned int ief : 1;          // [16], read only
        volatile unsigned int ihf : 1;          // [17], read only
        volatile unsigned int i4f : 1;          // [18], read only
        volatile unsigned int i3_4f : 1;        // [19], read only
        volatile unsigned int status_ahb_error : 1; // [20], read only
        volatile unsigned int reserve_31_20 : 11;// [31:21]
    }mBits;
} AUDIO_IFC_STATUS_U;

// start_addr 0x00000008
typedef union {
    volatile unsigned int dwValue;
    struct
    {
        volatile unsigned int reserve_1_0 : 2; // [1:0]
        volatile unsigned int start_addr : 30; // [31:2]
    }mBits;
} AUDIO_IFC_START_ADDR_U;

// fifo_size 0x0000000c
typedef union {
    volatile unsigned int dwValue;
    struct
    {
        volatile unsigned int reserve_3_0 : 4;  // [3:0]
        volatile unsigned int fifo_size : 11;   // [14:4]
        volatile unsigned int reserve_31_15 : 17;// [31:15]
    }mBits;
} AUDIO_IFC_FIFO_SIZE_U;

// int_mask
typedef union {
    volatile unsigned int dwValue;
    struct
    {
        volatile unsigned int reserve_7_0 : 8;          // [7:0]
        volatile unsigned int end_fifo : 1;             // [8]
        volatile unsigned int half_fifo : 1;            // [9]
        volatile unsigned int quarter_fifo : 1;         // [10] , 1/4
        volatile unsigned int three_quarter_fifo : 1;   // [11] , 3/4
        volatile unsigned int ahb_error : 1;            // [12]
        volatile unsigned int reserve_31_13 : 19;       // [31:13]
    }mBits;
} AUDIO_IFC_INT_MASK_U;

// int_clear
typedef union {
    volatile unsigned int dwValue;
    struct
    {
        volatile unsigned int reserve_7_0 : 8;          // [7:0]
        volatile unsigned int end_fifo : 1;             // [8], write clear
        volatile unsigned int half_fifo : 1;            // [9], write clear
        volatile unsigned int quarter_fifo : 1;         // [10], write clear,1/4 fifo
        volatile unsigned int three_quarter_fifo : 1;   // [11], write clear,3/4 fifo
        volatile unsigned int clear_ahb_error : 1;      // [12], write clear
        volatile unsigned int reserve_31_13 : 19;       // [31:13]
    }mBits;
} AUDIO_IFC_INT_CLEAR_U;

// cur_ahb_addr
typedef union {
    volatile unsigned int dwValue;
    struct
    {
        volatile unsigned int reserve_1_0 : 2;   // [1:0]
        volatile unsigned int cur_ahb_addr : 30; // [31:2], read only
    }mBits;
} AUDIO_IFC_CUR_AHB_ADDR_U;

typedef volatile struct
{
    struct
    {
        AUDIO_IFC_CONTROL_U control;        // 0x00000000
        AUDIO_IFC_STATUS_U status;          // 0x00000004
        AUDIO_IFC_START_ADDR_U start_addr;  // 0x00000008
        AUDIO_IFC_FIFO_SIZE_U fifo_size;    // 0x0000000c
        unsigned int reserved;                  // 0x00000010
        AUDIO_IFC_INT_MASK_U int_mask;      // 0x00000014
        AUDIO_IFC_INT_CLEAR_U int_clear;    // 0x00000018
        AUDIO_IFC_CUR_AHB_ADDR_U cur_ahb_addr; // 0x0000001c
    } ch[AUDIO_IFC_DIRECTION_NUM];
} AUDIO_IFC_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC int32 __ifc_start(uint32 addr, uint32 length, uint32 direction, uint8 is_dlul);
PUBLIC int32 __ifc_StopPlay(void);
PUBLIC int32 __ifc_StopRecord(void);
PUBLIC uint8 __ifc_PlayReachedHalf(void);
PUBLIC uint8 __ifc_PlayReachedEnd(void);
PUBLIC uint8 __ifc_RecordReachedHalf(void);
PUBLIC uint8 __ifc_RecordReachedEnd(void);
PUBLIC int32 __ifc_get_cur_ahb_addr(uint32 direction);

PUBLIC int32 __ifc_get_cause_ief_status(uint32 direction);
PUBLIC int32 __ifc_get_cause_ihf_status(uint32 direction);
PUBLIC int32 __ifc_set_int_clear_half_fifo(uint32 direction);
PUBLIC int32 __ifc_set_int_clear_end_fifo(uint32 direction);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_AUD_IFC_PHY_H_
