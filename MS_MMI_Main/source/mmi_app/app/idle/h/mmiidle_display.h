/*****************************************************************************
** File Name:      mmiidle_display.h                                         *
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

#ifndef _MMIIDLE_DISPLAY_H_
#define _MMIIDLE_DISPLAY_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guianim.h"
#include "guifont.h"

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
#define MMIIDLE_ITEM_MAX_NUM    7   //display 4 prompt info

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//shortcut focus
typedef enum
{
    MMIIDLE_SHORTCUT_FOCUS_1,       //focus 1 icon
    MMIIDLE_SHORTCUT_FOCUS_2,       //focus 2 icon
    MMIIDLE_SHORTCUT_FOCUS_3,       //focus 3 icon
    MMIIDLE_SHORTCUT_FOCUS_4,       //focus 4 icon
    MMIIDLE_SHORTCUT_FOCUS_5,       //focus 5 icon
    MMIIDLE_SHORTCUT_FOCUS_6,       //focus 6 icon
    MMIIDLE_SHORTCUT_FOCUS_MAX
} MMIIDLE_SHORTCUT_FOCUS_E;

//idle prompt
typedef enum
{
    MMIIDLE_PROMPT_POS_LINE,    //line position
    MMIIDLE_PROMPT_POS_SELECT,  //select position
    MMIIDLE_PROMPT_POS_ICON,    //icon position
    MMIIDLE_PROMPT_POS_TEXT,    //text position
    MMIIDLE_PROMPT_POS_MAX
} MMIIDLE_PROMPT_POS_E;

//idle line type
typedef enum
{
    MMIIDLE_LINE_NONE,      //none    
    MMIIDLE_LINE_TIME,      //time
    MMIIDLE_LINE_SIM1,      //sim1
    MMIIDLE_LINE_SIM2,      //sim2   
    MMIIDLE_LINE_SIM3,      //sim3   
    MMIIDLE_LINE_SIM4,      //sim4
    MMIIDLE_LINE_SCHEDULE,  //schedule
    MMIIDLE_LINE_COUNT_DOWN,//count down
    MMIIDLE_LINE_MP3,       //mp3
    MMIIDLE_LINE_FM,        //fm
    MMIIDLE_LINE_WORDS,     //words
    MMIIDLE_LINE_STK_IDLE_TEXT,     //stk idle text
    #ifdef MMI_ENABLE_DCD
    MMIIDLE_LINE_DCD, // DCD , added by @jun.hu, 2009/7/23
    #endif

    #if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)     
    MMIIDLE_LINE_WLAN, // wlan added by rong.gu
    #endif
    MMIIDLE_LINE_SHORTCUT,  //shortcut
    MMIIDLE_LINE_MAX
} MMIIDLE_LINE_TYPE_E;

//idle tp type
typedef enum
{
    MMIIDLE_TP_NONE,        //none
    MMIIDLE_TP_TIME,        //time
    MMIIDLE_TP_DATE,        //date
    MMIIDLE_TP_WEEK,        //week
    MMIIDLE_TP_LINE2,       //line 2
    MMIIDLE_TP_LINE3,       //line 3
    MMIIDLE_TP_LINE4,       //line 4
    MMIIDLE_TP_LINE5,       //line 5
    MMIIDLE_TP_LINE6,       //line 6
    #ifdef MMI_ENABLE_DCD//added by @jun.hu, 2009/7/23
    MMIIDLE_TP_DCD,
    #endif
    MMIIDLE_TP_SHORTCUT1,   //shortcut 1
    MMIIDLE_TP_SHORTCUT2,   //shortcut 2
    MMIIDLE_TP_SHORTCUT3,   //shortcut 3
    MMIIDLE_TP_SHORTCUT4,   //shortcut 4
    MMIIDLE_TP_SHORTCUT5,   //shortcut 5
    MMIIDLE_TP_SHORTCUT6,   //shortcut 6
    MMIIDLE_TP_LEFT_SK,     //left softkey
    MMIIDLE_TP_RIGHT_SK,    //right softkey
    MMIIDLE_TP_MIDDLE_SK,   //middle softkey
    MMIIDLE_TP_MAX
} MMIIDLE_TP_TYPE_E;

//idle shortcut text and image
typedef struct 
{
    MMI_IMAGE_ID_T  img_id; //image id
    MMI_TEXT_ID_T   txt_id; //text id
    MMI_IMAGE_ID_T  focus_img_id; //image id
    void            (*func)(void);
} MMIIDLE_SHORTCUT_T;

//idle line info,include shortcut and prompt
typedef struct 
{
    uint16                      line_num;                           //line number
    MMIIDLE_LINE_TYPE_E         line_type[MMIIDLE_ITEM_MAX_NUM];    //line type
    MMIIDLE_LINE_TYPE_E         line_focus_type;                    //focus line type
    MMIIDLE_SHORTCUT_FOCUS_E    shortcut_focus;                     //focus shortcut
} MMIIDLE_LINE_INFO_T;

//idle focus
typedef struct 
{
    GUI_POINT_T     line_point[MMIIDLE_ITEM_MAX_NUM];   //line point
    GUI_POINT_T     select_point[MMIIDLE_ITEM_MAX_NUM]; //select point
    GUI_POINT_T     icon_point[MMIIDLE_ITEM_MAX_NUM];   //icon point
    GUI_POINT_T     text_point[MMIIDLE_ITEM_MAX_NUM];   //text point
} MMIIDLE_ITEM_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle smart idle message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_SmartHandleMsg(
                                   MMI_WIN_ID_T         win_id,
                                   MMI_MESSAGE_ID_E     msg_id,
                                   DPARAM               param
                                   );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create istyle menu
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateIStyleMenu(
                                     MMI_HANDLE_T win_handle
                                     );

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : handle istyle idle message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_IStyleHandleMsg(
                                            MMI_WIN_ID_T         win_id,
                                            MMI_MESSAGE_ID_E     msg_id,
                                            DPARAM               param
                                            );
#endif //MMI_ISTYLE_SUPPORT

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
