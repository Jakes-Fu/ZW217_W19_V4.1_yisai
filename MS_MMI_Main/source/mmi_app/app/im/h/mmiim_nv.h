/*************************************************************************
 ** File Name:      mmiim_nv.h                                           *
 ** Author:         Jassmine                                             *
 ** Date:           2008/12/19                                           *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/12/19    Jassmine           Create.                             *
*************************************************************************/

#ifndef MMIIM_NV_H_
#define MMIIM_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmiim_base.h"
#ifdef IM_ENGINE_T9
#include "mmiim_internal.h"
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_IM_PINYIN_FUZZY = MMI_MODULE_IM << 16,
    MMINV_IM_HW_SPEED,
    MMINV_IM_HW_THICKNESS,
    MMINV_IM_HW_COLOR,
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    MMINV_IM_HW_AREA_TYPE,      //手写区域类型，当前主要为全屏和半屏区分
#endif
#ifdef MMI_IM_PDA_SUPPORT
    MMINV_IM_KEYBOARD_SIZE,
#endif    
    MMINV_IM_DEFAULT_METHOD,
    MMINV_IM_LANGUAGE,

#if defined MMI_IM_PDA_SUPPORT
    MMINV_IM_PREV_LANGUAGE,     //记录前一次非英语的语言
#endif

	MMINV_IM_KEYBOARD_TYPE, //0:9KEY; 1:26KEY

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
	IMNV_SOGOU_UPDATE_FREQUENCY,
	IMNV_SOGOU_UPDATE_SETTING,
	IMNV_SOGOU_UPDATE_TIME,
	IMNV_SOGOU_UPDATE_EVENT_INDEX,
    IMNV_SOGOU_SKIN_SORT_TYPE,
#endif

    MMINV_IM_WORD_PREDIC,                // used for word predictive is on or off, true or false
    
#ifdef MMI_IME_USER_DATABASE
    MMINV_IM_ALPHA_UDB,

#ifdef IM_SIMP_CHINESE_SUPPORT
    MMINV_IM_CHS_UDB,
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT  
    MMINV_IM_CHT_UDB,
#endif

#endif

    MMINV_IM_MAX_NV_ITEM_NUM
} MMIIM_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register im module nv len and max item
//	Global resource dependence : none
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIIM_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

