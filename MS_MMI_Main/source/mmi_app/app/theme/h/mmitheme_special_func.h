/****************************************************************************
** File Name:      mmitheme_special_func.h                                             *
** Author:         bin.ji                                              *
** Date:           04/29/2008                                             *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    窗口特效            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/29/2008     bin.ji       Create
** 
****************************************************************************/
#ifndef _MMITHEME_SPECIAL_FUNC_H_
#define _MMITHEME_SPECIAL_FUNC_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmidisplay_data.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

typedef enum
{
    MMITHEME_SPECIAL_NONE,
    MMITHEME_SPECIAL_MAINMENU,          //待机界面进入主菜单
    MMITHEME_SPECIAL_PIC_SLIDE,         //图片幻灯片演示
    MMITHEME_SPECIAL_PIC_FULLSCREEN,    //图片全屏显示时切换下一张
    MMITHEME_SPECIAL_MAX
}MMITHEME_SPECIAL_TYPE_E;   //特效场景
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/*****************************************************************************/
// 	Description : 采用特效刷屏
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_UpdateRectBySpecialEffect(
                                               GUI_RECT_T *rect_ptr
                                               );

/*****************************************************************************/
// 	Description : 设置哪个应用用到了刷屏特效，不同的应用需要的特效不同
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
void MMITHEME_SetSpecialEffectDirty(
                                    MMITHEME_SPECIAL_TYPE_E dirty_flag
                                    );

#endif


