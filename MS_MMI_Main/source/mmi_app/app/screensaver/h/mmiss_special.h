/*****************************************************************************
** File Name:      mmiidle_screensave_special.h                                      *
** Author:                                                                   *
** Date:           05/15/2008                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/15/2008     bin.ji              Create
******************************************************************************/

#ifndef _MMIIDLE_SCREENSAVER_SPECIAL_H_
#define _MMIIDLE_SCREENSAVER_SPECIAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
#ifdef SCREENSAVER_SUPPORT

#include "mmiset_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
	SPESCR_STAR,		//“星”屏保
	SPESCR_SNAKE,		// 贪吃蛇屏保
	SPESCR_SEED,		//生命游戏屏保
	
	SPESCR_MAX	
}MMIIDLE_SPESCR_TYPE_E;

typedef struct 
{
    void (*init_proc)(void);        //初始化环境
    void (*redraw_proc)(void);      //重绘屏幕
    void (*trigger_proc)(void);     //timer处理
}MMIIDLE_SPESCR_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 打开屏保预览窗口
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_OpenSpeScrSaverPreviewWin(MMIIDLE_SPESCR_TYPE_E   scr_type);

/*****************************************************************************/
// 	Description : 打开屏保窗口
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
void MMIAPIIDLESS_OpenSpecialScreenSaver(MMIIDLE_SPESCR_TYPE_E   scr_type);

/*****************************************************************************/
// 	Description : 菜单type转成screentype
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC MMIIDLE_SPESCR_TYPE_E MMIAPIIDLESS_ConvertSCRSaverType(
                                                 MMISET_SCREENSAVER_TYPE_E menu_type
                                                 );

#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
