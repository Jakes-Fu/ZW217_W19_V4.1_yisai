/*****************************************************************************
** File Name:      mmitheme_update.h                                         *
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

#ifndef _MMITHEME_UPDATE_H_
#define _MMITHEME_UPDATE_H_ 

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

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//update rect
typedef struct mmitheme_rect_tag
{
    GUI_RECT_T                  rect;       //rect
    struct mmitheme_rect_tag    *prev_ptr;  //previous rect pointer
    struct mmitheme_rect_tag    *next_ptr;  //next rect pointer
} MMITHEME_RECT_T;

//update rect
typedef struct 
{
    uint16          num;                //rect num
    MMITHEME_RECT_T *first_rect_ptr;    //rect list
} MMITHEME_UPDATE_RECT_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init update
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InitUpdate(void);

/*****************************************************************************/
// 	Description : store the update rect, include the main and sub lcd
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_StoreUpdateRect(
                                        GUI_LCD_DEV_INFO const  *lcd_dev_ptr,   //in:
                                        GUI_RECT_T              update_rect     //in:
                                        );

/*****************************************************************************/
// 	Description : update the rect, include the main and sub lcd
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_UpdateRect(void);

/*****************************************************************************/
// 	Description : set update delay count
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetUpdateDelayCount(
                                         uint8  delay_count
                                         );

/*****************************************************************************/
// 	Description : update multilayer to lcd buffer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_UpdateMultiLayer(void);

/*****************************************************************************/
// 	Description :is update full screen
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsUpdateFullScreen(void);

/*****************************************************************************/
// 	Description : init main lcd store the update rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InitMainLcdStortUpdateRect(void);

/*****************************************************************************/
// 	Description :is update rect dirty
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsUpdateRectDirty(void);

/*****************************************************************************/
// 	Description : update the rect, include the main and sub lcd ,ex for the livewallpaper
//	Global resource dependence : 
//  Author: arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_UpdateRectEx(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

