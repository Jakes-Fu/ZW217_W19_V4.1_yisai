/***************************************************************************
** File Name:      mmitheme_layer_special.h                                *
** Author:         hua.fang                                                *
** Date:           07/07/2011                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMITHEME_LAYER_SPECIAL_H_
#define _MMITHEME_LAYER_SPECIAL_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"
#include "guistring.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMITHEME_ZOOM_MEM_ALIGN 4
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// 缩放动画所需数据
typedef struct
{
    GUI_RECT_T*         rect_ptr;               // 若为空，则取内容层的rect
    GUI_LCD_DEV_INFO    content_lcd_dev;        // 内容层
    GUI_LCD_DEV_INFO    mask_lcd_dev;           // 蒙板层
    GUI_COLOR_T         mask_color;             // 蒙板颜色
    uint8               mask_alpha;             // 蒙板透明度
} MMITHEME_ZOOM_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : play zoom anim
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 传入蒙板层和内容层，原则假设这两层没有添加到层系统里，纯粹作为数据层来处理，所
// 以，我将它们添加或移除层系统，对用户都不会有影响，而最终的结果，也是将这两层移
// 除，如果用户要显示层的话，必须调完此接口后，主动将层添加进层系统中。
// 内容层的数据不会被破坏，但蒙层的数据，由于要做渐变效果，有可能会受到破坏，如果
// 用户需要原始数据不被破坏，需做下备份。
/*****************************************************************************/
PUBLIC void MMITHEME_PlayZoomAnim(
                                  MMITHEME_ZOOM_DATA_T      *zoom_data_ptr,     // 动画数据
                                  BOOLEAN                   is_zoom_out         // 是否缩小
                                  );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMITHEME_LAYER_SPECIAL_H_

