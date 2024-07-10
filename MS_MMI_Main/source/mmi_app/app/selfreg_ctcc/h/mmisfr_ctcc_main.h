/*****************************************************************************
** File Name:      mmisfr_ctcc_main.h
** Author:
** Copyright (C) 2000-2100, UNISOC Technoloies Co.,Ltd.
** Description:
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                                  NAME                                DESCRIPTION
** 2020/11/16                     mark.zhang1                   Create
******************************************************************************/

#ifndef _MMISFR_CTCC_MAIN_H_
#define _MMISFR_CTCC_MAIN_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiphone_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SFR_ICCID_LENGTH (MNSIM_ICCID_ID_NUM_LEN<<1)
#define MMISFR_STR_LEN (64)

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**--------------------------------------------------------------------------*/
typedef struct _SFR_ICCD
{
    char num[SFR_ICCID_LENGTH];
} SFR_ICCID;

typedef enum
{
    MMINV_SFR_SIM_ICCID = MMI_MODULE_SFR_CTCC << 16,
    MMINV_SFR_ITEM_NUM
} MMISFR_NV_ITEM_E;

typedef enum
{
    MMISFR_INDEX_SYS_VERSION = 0,
    MMISFR_INDEX_SOFTWARE_VER,
    MMISFR_INDEX_SOFTWARE_NAME,
    MMISFR_INDEX_UETYPE,
    MMISFR_INDEX_EID,
    MMISFR_INDEX_MAX
}MMISFR_INDEX_E;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CTCC_InitModule(void) ;

/*****************************************************************************/
//  Description :
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CTCC_HandleNetworkStatus(void);

/*****************************************************************************/
//  Description :
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC char *sfr_get_iccid(MN_DUAL_SYS_E sim_index);

/*****************************************************************************/
//  Description :
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void sfr_save_iccid(MN_DUAL_SYS_E sim_index) ;

/*****************************************************************************/
//  Description :set SYS_VERSION,SOFTWARE_VER,SOFTWARE_NAME,UETYPEµÈÐÅÏ¢
//  Param :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN sfr_set_str(MMISFR_INDEX_E str_index, const char str[MMISFR_STR_LEN + 1]);

#endif

