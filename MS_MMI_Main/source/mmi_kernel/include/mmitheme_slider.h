/******************************************************************************
** File Name:      mmitheme_slider.h                                          *
** Author:         xiaoqing.lu                                                *
** Date:           08/13/2012                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the data struct of           *
**                              slider control                                *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 08/13/2012    Xiaoqing.lu      Create			                          *
******************************************************************************/

#ifndef _MMITHEME_SLIDER_H_
#define _MMITHEME_SLIDER_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"

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

// MMI_SLIDER_STYLE_T
typedef struct
{
    BOOLEAN     is_has_bg;

    GUI_COLOR_T     channel_bg_color;
    GUI_COLOR_T     channel_disable_bg_color;
    MMI_IMAGE_ID_T  channel_bg_image_id;
    MMI_IMAGE_ID_T  channel_disable_bg_image_id;

    BOOLEAN         is_disp_fg;
    GUI_COLOR_T     channel_fg_color;
    GUI_COLOR_T     channel_disable_fg_color;
    MMI_IMAGE_ID_T  channel_fg_image_id;
    MMI_IMAGE_ID_T  channel_disable_fg_image_id;

    BOOLEAN is_has_thumb;
    GUI_COLOR_T thumb_color;
    GUI_COLOR_T disable_thumb_color;
    MMI_IMAGE_ID_T thumb_image_id;
    MMI_IMAGE_ID_T disable_thumb_image_id;
}MMI_SLIDER_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Slider 控件用户信息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetSliderStyle(
                                    MMI_SLIDER_STYLE_T *slider_style_ptr
                                    );


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_SLIDER_H_
/*! @} */
