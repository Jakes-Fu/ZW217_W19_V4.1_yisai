/*****************************************************************************
** File Name:       watch_commonwin_internal.h                               *
** Author:          fangfang.yao                                             *
** Date:            2020/02/28                                               *
** Copyright:                                                                *
** Description:     This file is used to describe commonwin internal export  *
**                  interface                                                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION                      *
** 02/28/2020           fangfang.yao          Create                         *
******************************************************************************/

#ifndef _WATCH_COMMONWIN_INTERNAL_H_
#define _WATCH_COMMONWIN_INTERNAL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/

typedef struct
{
    GUI_RECT_T      tipRect;
    GUI_RECT_T      btnRect;
}WATCH_COMMONWIN_LAYOUT_QUERY_1BTN_T;

typedef struct
{
    GUI_RECT_T      tipRect1;
    GUI_RECT_T      tipRect2;
    GUI_RECT_T      btnRect;
}WATCH_COMMONWIN_LAYOUT_QUERY_1BTN_WITH2STR_T;

typedef struct
{
    GUI_RECT_T      tipRect;
    GUI_RECT_T      leftBtnRect;
    GUI_RECT_T      rightBtnRect;
}WATCH_COMMONWIN_LAYOUT_QUERY_2BTN_T;

typedef struct
{
    GUI_RECT_T      picRect;
    GUI_RECT_T      tipRect;
}WATCH_COMMONWIN_LAYOUT_NOTE_FULLSCREEN_T;

typedef struct
{
    GUI_RECT_T      tipRect;
}WATCH_COMMONWIN_LAYOUT_NOTE_1LINE_T;

typedef struct
{
    GUI_RECT_T      tipRect;
}WATCH_COMMONWIN_LAYOUT_NOTE_2LINE_T;

typedef struct
{
    GUI_RECT_T      picRect;
}WATCH_COMMONWIN_LAYOUT_WAITING_FULLSCREEN_T;

typedef struct
{
    GUI_RECT_T      tipRect;
}WATCH_COMMONWIN_LAYOUT_WAITING_1LINE_T;
typedef struct
{
    GUI_RECT_T      tipRect;
    GUI_RECT_T      picRect;
}WATCH_COMMONWIN_LAYOUT_WAITING_1ICON_1STR_T;

typedef struct
{
    GUI_RECT_T      progressRect;
    GUI_RECT_T      topImgRect;
    GUI_RECT_T      bottomImgRect;
}WATCH_COMMONWIN_LAYOUT_PROGRESS_T;

#ifdef ADULT_WATCH_SUPPORT
typedef struct
{
    GUI_RECT_T      tipRect;
    GUI_RECT_T      picRect;
}ADULT_WATCH_COMMONWIN_LAYOUT_WAITING_1ICON_1STR_T;
#endif
typedef union
{
    WATCH_COMMONWIN_LAYOUT_QUERY_1BTN_T            layoutQuery1Btn;
    WATCH_COMMONWIN_LAYOUT_QUERY_1BTN_WITH2STR_T   layoutQuery1BtnWith2Str;
    WATCH_COMMONWIN_LAYOUT_QUERY_2BTN_T            layoutQuery2Btn;
    WATCH_COMMONWIN_LAYOUT_NOTE_FULLSCREEN_T       layoutNoteFullScreen;
    WATCH_COMMONWIN_LAYOUT_NOTE_1LINE_T            layoutNote1Line;
    WATCH_COMMONWIN_LAYOUT_NOTE_2LINE_T            layoutNote2Line;
    WATCH_COMMONWIN_LAYOUT_WAITING_FULLSCREEN_T    layoutWaitingFullScreen;
    WATCH_COMMONWIN_LAYOUT_WAITING_1LINE_T         layoutWaiting1Line;
    WATCH_COMMONWIN_LAYOUT_WAITING_1ICON_1STR_T layoutWaiting1Icon1Str;
    WATCH_COMMONWIN_LAYOUT_PROGRESS_T               layoutProgress;

}WATCH_COMMONWIN_LAYOUT_T ;

typedef struct
{
    MMI_WIN_ID_T            winId;          //win id
    GUI_RECT_T              bgRect;         //common win的坐标
    WATCH_COMMONWIN_TYPE_E  pwType;         //note type
    PROCESSMSG_FUNC         userFunc;       //用户传入的回调
    MMI_WIN_PRIORITY_E      winPriority;    //priority
    uint8                   timerId;        // timer id
    WATCH_SOFTKEY_TEXT_ID_T softkeyText;    //softkey text
    uint32 timeOutExit;//超时退出时间 0为不退出
}WATCH_COMMONWIN_INFO_T;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
//position
#define WATCH_COMMON_CUSTOM_STR_LEN         (255)

//QuetyWin
#define WATCH_QUERYWIN_TEXT_1BTN_RECT           DP2PX_RECT( 12, 12, 227, 167 )
#if defined SCREEN_SHAPE_CIRCULAR
#define WATCH_QUERYWIN_TEXT_1BTN_RECT           DP2PX_RECT( 36, 36, 203, 150 )
#define WATCH_QUERYWIN_TEXT_1BTN_WITH2STR_RECT1 DP2PX_RECT( 36, 60, 203, 88 )
#define WATCH_QUERYWIN_TEXT_1BTN_WITH2STR_RECT2 DP2PX_RECT( 36, 92, 203, 116 )
#elif defined SCREEN_SHAPE_SQUARE
#define WATCH_QUERYWIN_TEXT_1BTN_WITH2STR_RECT1 DP2PX_RECT( 12, 12, 227, 80 )
#define WATCH_QUERYWIN_TEXT_1BTN_WITH2STR_RECT2 DP2PX_RECT( 12, 81, 227, 167 )
#endif

#if defined SCREEN_SHAPE_CIRCULAR
#define WATCH_QUERYWIN_BTN_1BTN_RECT            DP2PX_RECT( 96, 168, 143, 215 )
#elif defined SCREEN_SHAPE_SQUARE
#define WATCH_QUERYWIN_BTN_1BTN_RECT            DP2PX_RECT( 12, 180, 227, 227 )
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
#define WATCH_QUERYWIN_TEXT_2BTN_RECT           DP2PX_RECT( 36, 36, 204, 150 )
#define WATCH_QUERYWIN_LEFTBTN_2BTN_RECT        DP2PX_RECT( 60, 168, 108, 216 )
#define WATCH_QUERYWIN_RIGHTBTN_2BTN_RECT       DP2PX_RECT( 132, 168, 180, 216 )
#else
#define WATCH_QUERYWIN_TEXT_2BTN_RECT           DP2PX_RECT( 12, 12, 227, 151 )
#define WATCH_QUERYWIN_LEFTBTN_2BTN_RECT        DP2PX_RECT( 32, 164, 95, 227 )
#define WATCH_QUERYWIN_RIGHTBTN_2BTN_RECT       DP2PX_RECT( 144, 164, 207, 227 )
#endif
#ifdef ADULT_WATCH_SUPPORT
#define ADULT_WATCH_QUERYWIN_TEXT_2BTN_RECT           DP2PX_RECT( 36, 36, 204, 150 )
#define ADULT_WATCH_QUERYWIN_LEFTBTN_2BTN_RECT        DP2PX_RECT( 60, 168, 108, 216 )
#define ADULT_WATCH_QUERYWIN_RIGHTBTN_2BTN_RECT       DP2PX_RECT( 132, 168, 180, 216 )

#define ADULT_WATCH_QUERYWIN_BTN_1BTN_RECT            DP2PX_RECT( 96, 96, 144, 144 )

#define ADULT_WATCH_QUERYWIN_BTN_1BTN_1STR_RECT       DP2PX_RECT( 96, 66, 144, 114 )
#define ADULT_WATCH_QUERYWIN_TEXT_1BTN_1STR_RECT      DP2PX_RECT( 36, 144, 204, 204 )
#endif

#define WATCH_PROGRESSWIN_TOP_IMG_RECT           DP2PX_RECT(104,12,136,40)
#define WATCH_PROGRESSWIN_PROGRESS_RECT          DP2PX_RECT(88,56,151,184)
#define WATCH_PROGRESSWIN_BOTTOM_IMG_RECT        DP2PX_RECT(104,196,136,228)

#define WATCH_PROGRESSWIN_TOP_IMG_RECT_H           DP2PX_RECT(30, 114, 250, 186)
#define WATCH_PROGRESSWIN_PROGRESS_RECT_H          DP2PX_RECT(40,140,200,150)
#define WATCH_PROGRESSWIN_BOTTOM_IMG_RECT_H        DP2PX_RECT(206,154,238,182)

//NoteWin
#define WATCH_NOTEWIN_PIC_FULLSCREEN_RECT       DP2PX_RECT( 36, 0, 203, 167 )
#define WATCH_NOTEWIN_TEXT_FULLSCREEN_RECT      DP2PX_RECT( 24, 168, 215, 227 )
#define WATCH_NOTEWIN_TEXT_1LINE_RECT           DP2PX_RECT( 12, 180, 227, 227 )
#define WATCH_NOTEWIN_TEXT_2LINE_RECT           DP2PX_RECT( 12, 156, 227, 227 )
#ifdef SCREEN_SHAPE_CIRCULAR
#define ADULT_WATCH_NOTEWIN_PIC_1ICON_1STR_RECT       DP2PX_RECT( 66, 36, 174, 144)
#define ADULT_WATCH_NOTEWIN_TEXT_1ICON_1STR_RECT      DP2PX_RECT( 36, 144, 204, 204 )
#define ADULT_WATCH_NOTEWIN_TEXT_1LINE_RECT           DP2PX_RECT( 30, 156, 210, 204 )
#define ADULT_WATCH_NOTEWIN_TEXT_2LINE_RECT           DP2PX_RECT( 30, 144, 210, 204 )
#endif

//WaitingWin
#define WATCH_WAITINGWIN_PIC_FULLSCREEN_RECT    DP2PX_RECT( 88, 88, 151, 151 )
#if defined SCREEN_SHAPE_CIRCULAR
#define WATCH_WAITINGWIN_TEXT_1LINE_RECT        DP2PX_RECT( 30, 156, 210, 204 )
#elif defined SCREEN_SHAPE_SQUARE
#define WATCH_WAITINGWIN_TEXT_1LINE_RECT        DP2PX_RECT( 20, 170, 219, 199 )
#endif


#define WATCH_WAITINGWIN_1ICON_1STR_TEXT_RECT       DP2PX_RECT( 36, 144, 204, 204 )
#define WATCH_WAITINGWIN_1ICON_1STR_PIC_RECT        DP2PX_RECT( 66, 36, 174, 144 )
#define WATCH_WAITINGWIN_1ICON_1STR_RECT            DP2PX_RECT( 36, 36, 204, 204 )

#ifdef ADULT_WATCH_SUPPORT
#define ADULT_WATCH_WAITINGWIN_1ICON_1STR_TEXT_RECT       DP2PX_RECT( 36, 144, 204, 204 )
#define ADULT_WATCH_WAITINGWIN_1ICON_1STR_PIC_RECT        DP2PX_RECT( 66, 36, 174, 144 )
#define ADULT_WATCH_WAITINGWIN_1ICON_1STR_RECT            DP2PX_RECT( 36, 36, 204, 204 )
#define ADULT_WATCH_WAITINGWIN_TEXT_1LINE_RECT            DP2PX_RECT( 36, 108, 204, 168 )
#define ADULT_WATCH_WAITINGWIN_1STR_WAVEICON_TEXT_RECT    DP2PX_RECT( 36, 24, 204, 72 )
#define ADULT_WATCH_WAITINGWIN_1STR_WAVEICON_IMAGE_RECT   DP2PX_RECT( 90, 102, 150, 138 )
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: set text style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] text_rect : label display rect
//             [In] custom_str_ptr : label display string
//             [In] image_id : label background image,if not, default fill color
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetTextBoxStyle(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_RECT_T      text_rect,
                                            MMI_STRING_T    *custom_str_ptr,
                                            MMI_IMAGE_ID_T  image_id
                                           );

/*****************************************************************************/
//  Description: set label style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] text_rect : label display rect
//             [In] custom_str_ptr : label display string
//             [In] image_id : label background image,if not, default fill color
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetLableStyle(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_RECT_T      text_rect,
                                            MMI_STRING_T    *custom_str_ptr,
                                            MMI_IMAGE_ID_T  image_id
                                           );

/*****************************************************************************/
//  Description: set button style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] btn_rect : button display rect
//             [In] btn_str : button display string
//             [In] btn_img : button display image
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetBtnStyle(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_RECT_T      btn_rect,
                                            MMI_STRING_T    *btn_str_ptr,
                                            MMI_IMAGE_ID_T  btn_img
                                          );

/*****************************************************************************/
//  Description: create label ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateLabelCtrl(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id
                                      );

/*****************************************************************************/
//  Description: create textbox ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateTextboxCtrl(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id
                                      );

/*****************************************************************************/
//  Description: create button ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateButtonCtrl(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id
                                      );

/*****************************************************************************/
//  Description: set commonwin layout
//  Parameter: [In] pTheme_ptr : commonwin sub theme info
//             [In] type : commonwin type
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_GetLayoutByType( WATCH_COMMONWIN_LAYOUT_T* pTheme_ptr,WATCH_COMMONWIN_TYPE_E type );

/*****************************************************************************/
//  Description: set commonwin sub theme info
//  Parameter: [In] input_str : the string which while be copied
//             [In] output_str_ptr : the output string
//             [Out] output_str_ptr
//             [Return] result : TRUE(copy success),FALSE(copy failed)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CommonWin_CopyStringInfo( MMI_STRING_T *input_str_ptr,MMI_STRING_T *output_str_ptr );
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description: create label ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateAnimCtrl(MMI_HANDLE_T    win_handle,MMI_CTRL_ID_T   ctrl_id);
/*****************************************************************************/
//  Description: set label style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] text_rect : label display rect
//             [In] custom_str_ptr : label display string
//             [In] image_id : label background image,if not, default fill color
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetAnimStyle(MMI_CTRL_ID_T   ctrl_id,GUI_RECT_T      image_rect, MMI_IMAGE_ID_T  image_id);
#endif
#endif
