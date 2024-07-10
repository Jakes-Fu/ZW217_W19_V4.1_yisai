/****************************************************************************
** File Name:      mmitheme_dropdownlist.h                                  *
** Author:         Xiaoqing.lu                                              *
** Date:           6/2/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 02/2009        Xiaoqing.lu       Create                                  *
**                                                                          *
****************************************************************************/

#ifndef  _MMITHEME_DROPDOWNLIST_H_    
#define  _MMITHEME_DROPDOWNLIST_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
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
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/

//the dropdownlist display style 
typedef struct
{
  GUI_BORDER_T   border;
  GUI_COLOR_T    font_color;            //字体颜色
  GUI_FONT_T     font;                  //字体
  uint8          font_space;            //字间距
  uint16         item_height;           //每条Item的高度
  uint16         default_item_num;      //每条Item的高度
  uint8          left_space;            //字符左间距
  MMI_IMAGE_ID_T    unpressed_arrow_icon;   //未按下的下拉箭头图标
  MMI_IMAGE_ID_T    pressed_arrow_icon;     //已按下的下拉箭头图标

  GUI_BG_T      bg_info;
  GUI_BG_T      list_bg_info;
}MMI_DROPDOWNLIST_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the dropdownlist style, is called by dropdownlist control
//  Global resource dependence : 
//  Author: yelianna
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetDropDownListStyle(
                                             MMI_DROPDOWNLIST_STYLE_T* style_ptr
                                             );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* END _MMITHEME_DROPDOWNLIST_H_  */
/*! @} */
