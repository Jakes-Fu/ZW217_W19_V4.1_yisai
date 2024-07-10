/*****************************************************************************
** File Name:      mmitheme_pos.h                                            *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMITHEME_POS_H_
#define _MMITHEME_POS_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
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
#define MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH    4

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get win default title rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetWinTitleHeight(
                                         void
                                         );

/*****************************************************************************/
//  Description : is full screen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsFullScreenRect( 
                                         GUI_RECT_T     rect
                                         );

/*****************************************************************************/
// 	Description : get full screen rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetFullScreenRect(void);

/*****************************************************************************/
// 	Description : get full screen rect by win handle
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetFullScreenRectEx(
                                               MMI_HANDLE_T win_handle
                                               );

/*****************************************************************************/
// 	Description : get full screen rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetFullScreenBothRect(void);

/*****************************************************************************/
//  Description : get client rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetClientRect(void);

/*****************************************************************************/
//  Description : get client rect by win handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetClientRectEx(
										   MMI_HANDLE_T	win_handle
										   );

/*****************************************************************************/
//  Description : get win default client rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetWinClientBothRect(
                                                     MMI_HANDLE_T    win_handle
                                                     );

/*****************************************************************************/
//  Description : get win default title rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetWinTitleBothRect(
                                                    MMI_HANDLE_T    win_handle
                                                    );

/*****************************************************************************/
//  Description : get tab rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetWinTabBothRect(
                                                  MMI_HANDLE_T   win_handle
                                                  );

/*****************************************************************************/
//  Description : get status bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetStatusBarBothRect(void);

/*****************************************************************************/
//  Description : get softkey rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetSoftkeyRect(
                                          uint16    softkey_index
                                          );

/*****************************************************************************/
//  Description : get the right pixel of the main screen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetMainScreenRightPixel(void);

/*****************************************************************************/
//  Description : get the bottom pixel of the main screen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetMainScreenBottomPixel(void);

/*****************************************************************************/
//  Description : get the message box rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetMsgBoxRect(void);

/*****************************************************************************/
//  Description : MMITHEME_IsMainScreenLandscape
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: Ö÷ÆÁÊÇ·ñºáÆÁ
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMainScreenLandscape(void);

/*****************************************************************************/
//  Description : get client rect width
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClientRectWidth(void);

/*****************************************************************************/
//  Description : get client rect height
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClientRectHeight(void);

/*****************************************************************************/
//  Description : get client rect height by win handle
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClientRectHeightEx(MMI_HANDLE_T	win_handle);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

