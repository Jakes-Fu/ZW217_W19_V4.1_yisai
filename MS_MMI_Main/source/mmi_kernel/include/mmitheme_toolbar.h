/****************************************************************************
** File Name:      mmitheme_toolbar.h                                       *
** Author:         James.Zhang                                              *
** Date:           9/02/2010                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 9/2010         James.Zhang      Create                                   *
**                                                                          *
****************************************************************************/

#ifndef _MMITHEME_TOOLBAR_H_
#define _MMITHEME_TOOLBAR_H_ 

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
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

typedef struct  
{
    MMI_IMAGE_ID_T  bg_image;
    MMI_IMAGE_ID_T  bg_icon;

    MMI_IMAGE_ID_T  bg_icon_highlight;
    MMI_IMAGE_ID_T  bg_hit;
    MMI_IMAGE_ID_T  separator_img;

    uint16          left_margin;
    uint16          right_margin;
    uint16          top_margin;
    uint16          item_width;
    uint16          item_height;
    
    MMI_IMAGE_ID_T  icon_more;
    GUI_COLOR_T     font_color;
    GUI_FONT_T      font;
    uint32          auto_shrink_interval;
    uint32          pop_anim_interval;
    uint32          pop_anim_delay_ticks;
    int16           pop_anim_step_v;
    int16           pop_anim_step_h;          

}MMITHEMETOOLBAR_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取toolbar控件的风格信息.
//  Global resource dependence : 
//  Author: 
//  Note:James.Zhang
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetToolbarStyle(
                                        MMITHEMETOOLBAR_STYLE_T *style_ptr
                                        );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_TOOLBAR_H_
