/*****************************************************************************
** File Name:      mmitheme_iconfolder.h                                     *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007      Jassmine              Creat
******************************************************************************/

#ifndef _MMITHEME_ICONFOLDER_H_
#define _MMITHEME_ICONFOLDER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cafcontrol.h"
#include "mmk_type.h"

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
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//the anim theme
typedef struct
{
    uint16              icon_max_num;       //display icon max number

    uint16              icon_width;         //icon width
    uint16              icon_height;        //icon height

    GUI_BG_T            icon_bg;            //bg
    GUI_BORDER_T        icon_border;        //icon border

    GUI_BG_T            bg;                 //bg
    GUI_BORDER_T        border;             //border

    MMI_IMAGE_ID_T      mark_img_id;        //mark image id

    GUI_LCD_DEV_INFO    lcd_dev;            //lcd dev
} MMITHEME_ICONFOLDER_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get icon folder theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetIconFolderTheme(
                                 MMITHEME_ICONFOLDER_T  *theme_ptr //in/out:
                                 );

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetIconRect(
                                       uint16       index,
                                       uint16       icon_width,
                                       uint16       icon_height,
                                       GUI_RECT_T   folder_rect
                                       );

/*****************************************************************************/
//  Description : get tp down icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetTpDownIconRect(
                                             uint16         index,
                                             GUI_RECT_T     rect
                                             );

/*****************************************************************************/
//  Description : get icon rotate angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetIconAngle(
                                    uint16  index
                                    );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

