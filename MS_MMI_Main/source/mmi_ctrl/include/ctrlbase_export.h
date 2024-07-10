/***************************************************************************
** File Name:      ctrlbase_export.h                                       *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLBASE_EXPORT_H_
#define _CTRLBASE_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "cafcontrol.h"

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

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

typedef enum
{
    CTRL_TP_PRIORITY_NORMAL,
    CTRL_TP_PRIORITY_HIGH,
    CTRL_TP_PRIORITY_HIGHER,

    CTRL_TOPMOST_MAX
} CTRL_TP_PRIORITY_E;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : update control and all its children
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLBASE_Update(
    MMI_HANDLE_T   ctrl_handle
    );

/*****************************************************************************/
// 	Description : get both rect
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetBothRect(
    MMI_HANDLE_T        ctrl_handle,
    GUI_BOTH_RECT_T     *both_rect_ptr
    );

/*****************************************************************************/
// 	Description : set both rect
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetBothRect(
    MMI_HANDLE_T            ctrl_handle,
    const GUI_BOTH_RECT_T   *both_rect_ptr
    );

/*****************************************************************************/
// 	Description : get rect
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetRect(
    MMI_HANDLE_T            ctrl_handle,
    GUI_RECT_T              *rect_ptr
    );

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetRect(
    MMI_HANDLE_T            ctrl_handle,
    const GUI_RECT_T        *rect_ptr
    );

/*****************************************************************************/
// 	Description : get lcd dev info
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetLcdDevInfo(
    MMI_HANDLE_T              ctrl_handle,
    GUI_LCD_DEV_INFO          *lcd_dev_ptr
    );

/*****************************************************************************/
// 	Description : set lcd dev info
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetLcdDevInfo(
    MMI_HANDLE_T                 ctrl_handle,
    const GUI_LCD_DEV_INFO       *lcd_dev_ptr
    );

/*****************************************************************************/
// 	Description : get bg
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetBg(
    MMI_HANDLE_T          ctrl_handle,
    GUI_BG_T              *bg_ptr
    );

/*****************************************************************************/
// 	Description : set bg
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetBg(
    MMI_HANDLE_T          ctrl_handle,
    GUI_BG_T              *bg_ptr
    );

/*****************************************************************************/
// 	Description : get border
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetBorder(
    MMI_HANDLE_T      ctrl_handle,
    GUI_BORDER_T      *border_ptr
    );

/*****************************************************************************/
// 	Description : set border
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetBorder(
    MMI_HANDLE_T      ctrl_handle,
    GUI_BORDER_T      *border_ptr
    );

/*****************************************************************************/
// 	Description : get padding
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetPadding(
    MMI_HANDLE_T      ctrl_handle,
    GUI_PADDING_T     *padding_ptr
    );

/*****************************************************************************/
// 	Description : set padding
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetPadding(
    MMI_HANDLE_T      ctrl_handle,
    GUI_PADDING_T     *padding_ptr
    );

/*****************************************************************************/
// 	Description : set whether ctrl can active
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetCanActive(
    MMI_HANDLE_T        ctrl_handle,
    BOOLEAN             can_active
    );

/*****************************************************************************/
// 	Description : set whether ctrl can active under kbd mode
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetCanKbdActive(
    MMI_HANDLE_T        ctrl_handle,
    BOOLEAN             can_kbd_active
    );

/*****************************************************************************/
// 	Description : get active
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetActive(
    MMI_HANDLE_T      ctrl_handle
    );

/*****************************************************************************/
// 	Description : set active
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetActive(
    MMI_HANDLE_T      ctrl_handle
    );

/*****************************************************************************/
// 	Description : get enabled
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetEnabled(
    MMI_HANDLE_T      ctrl_handle
    );

/*****************************************************************************/
// 	Description : set enabled
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetEnabled(
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_enabled
    );

/*****************************************************************************/
// 	Description : get visible
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetVisible(
    MMI_HANDLE_T      ctrl_handle
    );

/*****************************************************************************/
// 	Description : set visible
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetVisible(
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_visible
    );

/*****************************************************************************/
// 	Description : set tp enabled
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetTpEnabled(
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_tp_enabled
    );

/*****************************************************************************/
// 	Description : set kbd enabled
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetKbdEnabled(
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_kbd_enabled
    );

/*****************************************************************************/
// 	Description : get user data
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC uint32 CTRLBASE_GetUserData(
    MMI_HANDLE_T      ctrl_handle
    );

/*****************************************************************************/
// 	Description : set user data
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetUserData(
    MMI_HANDLE_T      ctrl_handle,
    uint32            user_data
    );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/

#ifdef _cplusplus
	}
#endif

#endif  // _CTRLBASE_EXPORT_H_
