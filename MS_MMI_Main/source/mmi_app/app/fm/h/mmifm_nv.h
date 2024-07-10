/*************************************************************************
 ** File Name:      FM_nv.h                                              *
 ** Author:         liqing.peng                                          *
 ** Date:           2006/05/17                                           *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/05/17     liqing.peng      Create.                              *
*************************************************************************/

#ifndef FM_NV_H_
#define FM_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmifm_internal.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    FMNV_APP_INFO = MMI_MODULE_FM << 16,
    FMNV_PREDEFINED_LIST,
    FMNV_CUR_INFO, 
    FMNV_CHANNEL_LIST,
    FM_MAX_NV_ITEM_NUM
}FM_NV_ITEM_E;

typedef enum
{
    MMIFM_NV_FIRST_ITEM = MMI_MODULE_FM << 16,
    MMIFM_NV_CUR_INFO = MMI_MODULE_FM << 16,
    MMIFM_FIRST_CHANNEL, 
    MMIFM_LAST_CHANNEL = MMIFM_FIRST_CHANNEL + MMIFM_MAX_CHANNEL_NUM -1,    
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
	MMINV_FM_TIMER_RECORD_INFO, //FM定时录音信息
#endif
    MMIFM_NV_LAST_ITEM 
}MMIFM_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

