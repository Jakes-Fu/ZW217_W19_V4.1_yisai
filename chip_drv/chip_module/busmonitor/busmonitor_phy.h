/******************************************************************************
 ** File Name:      busmonitor_phy.h                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           07/29/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/29/2010     liuhao     Create.                                   *
 ******************************************************************************/
#ifndef _BUSMONITOR_PHY_H_
#define _BUSMONITOR_PHY_H_
#include "isr_drvapi.h"
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
#define BM_DNT_CARE_VALUE       0

#define BM_MASK_NONE_VALUE 0
#define BM_MASK_ALL_VALUE 0xFFFFFFFF

#define BM_DISABLE      0
#define BM_ENABLE       1

#define BM_ACCESS_MODE_READ     0
#define BM_ACCESS_MODE_WRITE        1

#define BM_ACCESS_SIZE_BYTE     0
#define BM_ACCESS_SIZE_HALFWORD     1
#define BM_ACCESS_SIZE_WORD     2

#define BM_MASTER_ARM           1
#define BM_MASTER_DMA           2
#define BM_MASTER_TIC         3
#define BM_MASTER_DCAM           4

#define BM_ARM_DATA      0
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define BM_ARM_INSTRUCTION         1
#define BM_ARM7_ACCESS_EMC         2
#define BM_DMA_W                   3

#define BM_SDIO                    4
#define BM_DCAM                    5
#define BM_ARM7_ACCESS_NONE_EMC    6
#define BM_DMA_R                   7

#define BM_LZMA                    8
#define BM_LCM_ACCESS_EMC          9
#define BM_LCM_ACCESS_LCM          10
#endif

typedef struct _BM_POINT
{
    BOOLEAN     enabled;
    ON_BM_RAISE on_raise;
    unsigned long data;
} BM_POINT;

typedef struct _BM_STATUS
{
    BOOLEAN     clk_eb;
    BM_POINT    points[MAX_BM_POINTS_NUM];
} BM_STATUS;

typedef struct _BM_POINT_SETTING
{
    unsigned long addr_min;
    unsigned long addr_max;
    unsigned long addr_mask;
    unsigned long data_min;
    unsigned long data_max;
    unsigned long data_mask; //data & (~data_mask)
    unsigned long data_target;
    unsigned long data_mask2; //
    unsigned long access_mode;
    unsigned long size_en;
    unsigned long access_size;
    unsigned char burst_en;
    unsigned char burst_type;
    unsigned long chn_sel;
    unsigned char master_en;
    unsigned char master_type;
    ON_BM_RAISE on_raise;
} BM_POINT_SETTING;


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC void bm_close (void);
PUBLIC void bm_set_point (int point, const BM_POINT_SETTING *setting);
PUBLIC ISR_EXE_T bm_isr (uint32 fiq_num);
PUBLIC void bm_get_match (uint32 *bm_match_addr, uint32 *bm_match_data);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
