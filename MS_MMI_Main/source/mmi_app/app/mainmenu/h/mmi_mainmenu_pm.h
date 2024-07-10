/*****************************************************************************
** File Name:      ctrlmainmenu_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_PM_H_
#define _MMI_MAINMENU_PM_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "ctrlmainmenu_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIMAINMENU_DrawItemIcon(
                                        MMI_HANDLE_T win_handle,
                                        CTRLMAINMENU_ITEM_T const *item_ptr,
                                        GUI_LCD_DEV_INFO const *lcd_dev_info_ptr,
                                        GUI_RECT_T const *icon_rect_ptr,
                                        GUI_RECT_T const *disp_rect_ptr
                                        );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


