/*****************************************************************************
** File Name:       watch_commonwin_export.h                                 *
** Author:          fangfang.yao                                             *
** Date:            2020/02/28                                               *
** Copyright:                                                                *
** Description:     This file is used to describe commonwin export interface *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION                      *
** 02/28/2020           fangfang.yao          Create                         *
******************************************************************************/

#ifndef _WATCH_COMMONWIN_EXPORT_H_
#define _WATCH_COMMONWIN_EXPORT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"

/**--------------------------------------------------------------------------*
**                          macro declaration                                *
**---------------------------------------------------------------------------*/
typedef struct
{
    MMI_TEXT_ID_T softkeyLeft;
    MMI_TEXT_ID_T softkeyMiddle;
    MMI_TEXT_ID_T softkeyRight;
}WATCH_SOFTKEY_TEXT_ID_T;

typedef enum
{
    WATCH_COMMONWIN_TYPE_QUERY_1BTN = 0,
    WATCH_COMMONWIN_TYPE_QUERY_1BTN_WITH2STR,
    WATCH_COMMONWIN_TYPE_QUERY_2BTN,
    WATCH_COMMONWIN_TYPE_NOTE_FULLSCREEN,
    WATCH_COMMONWIN_TYPE_NOTE_1LINE,
    WATCH_COMMONWIN_TYPE_NOTE_2LINE,
    WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN,
    WATCH_COMMONWIN_TYPE_WAITING_1LINE,
    WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR,
    WATCH_COMMONWIN_TYPE_PROGRESS,
    WATCH_COMMONWIN_TYPE_PROGRESS_H,    
#ifdef ADULT_WATCH_SUPPORT
    ADULT_WATCH_COMMONWIN_TYPE_QUERY_2BTN,
    ADULT_WATCH_COMMONWIN_TYPE_QUERY_1BTN_1Str,
    ADULT_WATCH_COMMONWIN_TYPE_WAITING_1STR_WAVEICON,
    ADULT_WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR,
    ADULT_WATCH_COMMONWIN_TYPE_WAITING_1LINE,
    ADULT_WATCH_COMMONWIN_TYPE_QUERY_1BTN,
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
    ADULT_WATCH_COMMONWIN_TYPE_NOTE_1ICON_1STR,
    ADULT_WATCH_COMMONWIN_TYPE_NOTE_1LINE,
    ADULT_WATCH_COMMONWIN_TYPE_NOTE_2LINE,
#endif
    WATCH_COMMONWIN_TYPE_MAX,
} WATCH_COMMONWIN_TYPE_E;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: enter query window (one button)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : custom rect display string
//             [In] btn_str : button display string
//             [In] btn_image : button display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_QueryWin_1Btn_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_STRING_T    *btn_str,
                                                        MMI_IMAGE_ID_T  btn_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter query window (one button ,two str)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str1 : first custom rect display string
//             [In] tip_str2 : second custom rect display string
//             [In] btn_str : button display string
//             [In] btn_image : button display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_QueryWin_1Btn_With2Str_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str1,
                                                        MMI_STRING_T    *tip_str2,
                                                        MMI_STRING_T    *btn_str,
                                                        MMI_IMAGE_ID_T  btn_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter query window (two button)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : custom rect display string
//             [In] left_btn_str : left button display string
//             [In] right_btn_str : right button display string
//             [In] left_btn_image : left button display image
//             [In] right_btn_image : right button display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_QueryWin_2Btn_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_STRING_T    *left_btn_str,
                                                        MMI_STRING_T    *right_btn_str,
                                                        MMI_IMAGE_ID_T  left_btn_image,
                                                        MMI_IMAGE_ID_T  right_btn_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter note window (full screen)
//  Parameter: [In] win_id : commonwin ID
//             [In] pic_str : picture display string
//             [In] tip_str : label display string
//             [In] pic_image : picture display image
//             [In] tip_image : label display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_NoteWin_FullScreen_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *pic_str,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  pic_image,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter note window (only one line)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] tip_image : label display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_NoteWin_1Line_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter note window (for two line)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] tip_image : label display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_NoteWin_2Line_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter waiting window (full screen)
//  Parameter: [In] win_id : commonwin ID
//             [In] pic_str : picture display string
//             [In] pic_image : picture display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_WaitingWin_FullScreen_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *pic_str,
                                                        MMI_IMAGE_ID_T  pic_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter waiting window (only one line)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] tip_image : label display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_WaitingWin_1Line_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

PUBLIC void WatchCOM_WaitingWin_1Icon_1Str_EX(
                                                        MMI_WIN_ID_T          win_id,
                                                        MMI_STRING_T         *tip_str,
                                                        GUI_RECT_T              *txt_rect,
                                                        MMI_IMAGE_ID_T      img_id,
                                                        GUI_RECT_T              *img_rect,
                                                        PROCESSMSG_FUNC  userFunc
                                                     );
/*****************************************************************************/
//  Description: enter waiting window (full screen)
//  Parameter: [In] win_id : commonwin ID
//             [In] pic_str : picture display string
//             [In] pic_image : picture display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [In] timeOut : timeOut to close waiting win
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_WaitingTimeOutWin_FullScreen_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *pic_str,
                                                        MMI_IMAGE_ID_T  pic_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc,
                                                        uint32 timeOut
                                                     );

PUBLIC void WatchCOM_NoteWin_Set_Font(GUI_FONT_T *font_ptr,GUI_COLOR_T *font_color_ptr);
PUBLIC BOOLEAN WatchCOM_QueryWin_1Btn_Set_Tip_String(MMI_WIN_ID_T win_id, MMI_STRING_T *tip_str);

/*****************************************************************************/
//  Description: enter progress window 
//  Parameter: [In] win_id : commonwin ID
//             [In] top_image : top display image
//             [In] bottom_image : bottom display image
//             [In] userFunc : custom callback API
//             [In] maxCount: progress max count. 
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ProgressWin_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_IMAGE_ID_T top_image,
                                                        MMI_IMAGE_ID_T bottom_image,
                                                        uint32  totalCount,
                                                        uint32  curProgress,
                                                        PROCESSMSG_FUNC userFunc
                                                     );


/*****************************************************************************/
//  Description: enter progress window 
//  Parameter: [In] win_id : commonwin ID
//             [In] top_image : top display image
//             [In] bottom_image : bottom display image
//             [In] userFunc : custom callback API
//             [In] maxCount: progress max count. 
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ProgressWin_Enter_Ex(
                                                        MMI_WIN_ID_T    win_id,
                                                        WATCH_COMMONWIN_TYPE_E type,
                                                        MMI_IMAGE_ID_T top_image,
                                                        MMI_IMAGE_ID_T bottom_image,
                                                        uint32  totalCount,
                                                        uint32  curProgress,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: set progress value 
//  Parameter: [In] win_id : commonwin ID
//              [In] progress : progress value to set.
//             [Out] None
//             [Return] None.
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ProgressWin_SetProgress(MMI_WIN_ID_T win_id, uint32 progress);

/*****************************************************************************/
//  Description: set constraint  
//  Parameter: [In] win_id : commonwin ID
//              [In] max : max progress value to adjust.
//              [In] min: min progress value to adjust.
//             [Out] None
//             [Return] None.
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ProgressWin_SetConstraint(MMI_WIN_ID_T win_id, uint32 max, uint32 min);


PUBLIC void WatchCOM_NoteWin_FullScreen_Ente_Timeout(MMI_WIN_ID_T win_id,MMI_TEXT_ID_T tip_id,MMI_IMAGE_ID_T tip_image,PROCESSMSG_FUNC userFunc,uint32 timeOut);
PUBLIC void WatchCOM_NoteWin_1Line_Enter_Timeout(MMI_WIN_ID_T win_id,MMI_TEXT_ID_T tip_id,PROCESSMSG_FUNC userFunc,uint32 timeOut);


#ifdef  ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description: enter waiting window (1Icon_1Str)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : display string
//             [In] tip_image : display image
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_WaitingWin_1Icon_1Str_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     );
/*****************************************************************************/
//  Description: enter waiting window (1str and WaveIcon)
//  Parameter: [In] win_id : commonwin ID
//             [In] str : display string
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_WaitingWin_1str_WaveIcon_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *str,
                                                        PROCESSMSG_FUNC userFunc
                                                        );
/*****************************************************************************/
//  Description: enter note window (full screen)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] pic_image : picture display image
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  pic_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     );
/*****************************************************************************/
//  Description: enter note window (only one line)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] tip_image : label display image
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/

PUBLIC void Adult_WatchCOM_NoteWin_1Line_Toast_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     );
/*****************************************************************************/
//  Description: enter note window (for two line)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] tip_image : label display image
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void Adult_WatchCOM_NoteWin_2Line_Toast_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     );
PUBLIC void Adult_WatchCOM_NoteWin_Set_Font(
                                            GUI_FONT_T    *font_ptr,
                                            GUI_COLOR_T   *font_color_ptr
                                            );

/*****************************************************************************/
//  Description: enter waiting window (only one line)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] tip_image : label display image
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_WaitingWin_1Line_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter query window (two button)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : custom rect display string
//             [In] left_btn_str : left button display string
//             [In] right_btn_str : right button display string
//             [In] left_btn_image : left button display image
//             [In] right_btn_image : right button display image
//             [In] softkey_text: the softkey text IDs for display
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_QueryWin_2Btn_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T *tip_str,
                                                        MMI_STRING_T *left_btn_str,
                                                        MMI_STRING_T *right_btn_str,
                                                        MMI_IMAGE_ID_T left_btn_image,
                                                        MMI_IMAGE_ID_T right_btn_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter query window (one button one str)
//  Parameter: [In] win_id : commonwin ID
//             [In] btn_str : button display string
//             [In] btn_image : button display image
//             [In] tip_str : custom rect display string
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_QueryWin_1Btn_1Str_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *btn_str,
                                                        MMI_IMAGE_ID_T  btn_image,
                                                        MMI_STRING_T    *tip_str,
                                                        PROCESSMSG_FUNC userFunc
                                                     );

/*****************************************************************************/
//  Description: enter query window (one button)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : custom rect display string
//             [In] btn_str : button display string
//             [In] btn_image : button display image
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_QueryWin_1Btn_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *btn_str,
                                                        MMI_IMAGE_ID_T  btn_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     );
#endif
#endif

