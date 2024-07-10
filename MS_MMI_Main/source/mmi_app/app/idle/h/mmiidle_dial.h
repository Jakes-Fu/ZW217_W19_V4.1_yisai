/*****************************************************************************
** File Name:      mmiidle_dial.h                                            *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2007       Jassmine              Creat
******************************************************************************/

#ifndef _MMIIDLE_DIAL_H_
#define _MMIIDLE_DIAL_H_

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
//dial button
//#ifdef TOUCH_PANEL_SUPPORT
typedef enum
{
    MMIIDLE_DIAL_BUTTON_1,
    MMIIDLE_DIAL_BUTTON_2,
    MMIIDLE_DIAL_BUTTON_3,
    MMIIDLE_DIAL_BUTTON_4,
    MMIIDLE_DIAL_BUTTON_5,
    MMIIDLE_DIAL_BUTTON_6,
    MMIIDLE_DIAL_BUTTON_7,
    MMIIDLE_DIAL_BUTTON_8,
    MMIIDLE_DIAL_BUTTON_9,
    MMIIDLE_DIAL_BUTTON_STAR,
    MMIIDLE_DIAL_BUTTON_0,
    MMIIDLE_DIAL_BUTTON_HASH,
    MMIIDLE_DIAL_BUTTON_SAVE,
    MMIIDLE_DIAL_BUTTON_CALL,
#ifdef VT_SUPPORT
    MMIIDLE_DIAL_BUTTON_VT,
#endif
#ifdef MMI_IP_CALL_SUPPORT
    MMIIDLE_DIAL_BUTTON_IPCALL,
#endif
#if !(defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT))
    MMIIDLE_DIAL_BUTTON_SMS,
#endif
    MMIIDLE_DIAL_BUTTON_DEL,
#if defined(TOUCH_PANEL_SUPPORT) 
    MMIIDLE_DIAL_BUTTON_HIDE_PANEL,
    MMIIDLE_DIAL_BUTTON_SHOW_PANEL,
    MMIIDLE_DIAL_BUTTON_HIDE_DTMF,
#ifdef MMI_ISTYLE_SUPPORT
    MMIIDLE_DIAL_BUTTON_SHOW_MATCH_ARROW,
#endif
#endif
    MMIIDLE_DIAL_BUTTON_MAX,
} MMIIDLE_DIAL_BUTTON_E;
//#endif


//dial button info
typedef struct
{
    MMIIDLE_DIAL_BUTTON_E   index;
    MMI_IMAGE_ID_T          bg_img;
    MMI_IMAGE_ID_T          bg_img_p;
    MMI_IMAGE_ID_T          icon_img;
    MMI_IMAGE_ID_T          icon_img_p;
    MMI_IMAGE_ID_T          bg_img_gray;
} MMIIDLE_DIAL_BUTTON_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
