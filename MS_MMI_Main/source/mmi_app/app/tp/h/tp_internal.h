/*****************************************************************************
** File Name:      tp_internal.h                                                   *
** Author:                                                                   *
** Date:           11/17/2005                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       wancan.you       Create
** 06/03/2010    vine.yuan        Add declarations for TP Self-Adaptive featur
******************************************************************************/

#ifndef _TP_INTERNAL_H_
#define _TP_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"

#ifdef TP_SELFADAPTIVE_ENABLE
#include "tp_api.h"
#endif //TP_SELFADAPTIVE_ENABLE
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

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
// 	Description : register tp module nv len and max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
void TP_RegTPNv(void);

/*****************************************************************************/
// 	Description : 设置是否时第一次开机
//	Global resource dependence : 
//  Author: Robert
//	Note:
/*****************************************************************************/
void  TP_SetPowerOnFirst(
                                           BOOLEAN                 is_first   // 是否时第一次开机
                                       );

/*****************************************************************************/
// 	Description : 是否时第一次开机
//	Global resource dependence : 
//  Author: Robert
//	Note:
/*****************************************************************************/
BOOLEAN TP_IsPowerOnFirst(void);

/*****************************************************************************/
// 	Description : coordinate window open
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
void COORDINATE_OpenWin(BOOLEAN is_first);

/*****************************************************************************/
// 	Description : 开机时初始化触摸屏参数
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
void COORDINATE_Init(void);
/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void TP_RegWinIdNameArr(void);
#ifdef TP_SELFADAPTIVE_ENABLE
/*****************************************************************************/
// 	Description : To update the current self-adaptive mode,and save into nv
//	Global resource dependence : 
//  Author: vine.yuan
//	Note:
/*****************************************************************************/
void TP_UpdateAdaptMode(TP_MODE_E adapt_mode);

/*****************************************************************************/
// 	Description : To get the current self-adaptive mode.
//	Global resource dependence : 
//  Author: vine.yuan
//	Note:
/*****************************************************************************/
TP_MODE_E TP_GetCurAdaptMode(void);

/*****************************************************************************/
// 	Description : To initialize the self-adaptive mode for TP.
//				If fail, return FALSE, or TRUE
//	Global resource dependence : 
//  Author: vine.yuan
//	Note:
/*****************************************************************************/
BOOLEAN TP_InitAdaptMode(void);

#endif //TP_SELFADAPTIVE_ENABLE
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


