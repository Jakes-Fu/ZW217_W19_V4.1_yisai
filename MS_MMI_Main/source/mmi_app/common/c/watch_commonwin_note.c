/*****************************************************************************
** File Name:       watch_commonwin_note.c                                   *
** Author:          fangfang.yao                                             *
** Date:            2020/03/05                                               *
** Copyright:                                                                *
** Description:     This file is used to process msg to main window of       *
**                  notewin                                                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION
** 03/05/2020           fangfang.yao          Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "dal_time.h"
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_theme.h"
#include "mmidisplay_data.h"
#include "mmi_default.h"
#include "guistring.h"
#include "guilabel.h"
#include "mmicom_trace.h"
#include "watch_common.h"
#include "watch_commonwin_export.h"
#include "watch_commonwin_internal.h"
//#ifdef SCREEN_SHAPE_CIRCULAR
#include "mmi_common.h"
#include "guitext.h"
//#endif
/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct
{
    MMI_STRING_T                picStr;
    MMI_STRING_T                tipStr;
    MMI_IMAGE_ID_T              picImage;
    MMI_IMAGE_ID_T              tipImage;
    GUI_RECT_T                  picRect;
    GUI_RECT_T                  tipRect;
}WATCH_NOTEWIN_INFO_FULLSCREEN_T;

typedef struct
{
    MMI_STRING_T                tipStr;
    MMI_IMAGE_ID_T              tipImage;
    GUI_RECT_T                  tipRect;
}WATCH_NOTEWIN_1LINE_INFO_T;

typedef struct
{
    MMI_STRING_T                tipStr;
    MMI_IMAGE_ID_T              tipImage;
    GUI_RECT_T                  tipRect;
}WATCH_NOTEWIN_2LINE_INFO_T;

typedef union
{
    WATCH_NOTEWIN_INFO_FULLSCREEN_T infoFullScreen;
    WATCH_NOTEWIN_1LINE_INFO_T      info1Line;
    WATCH_NOTEWIN_2LINE_INFO_T      info2Line;
}WATCH_NOTEWIN_INFO_U;

typedef struct
{
    WATCH_COMMONWIN_INFO_T  baseInfo;
    WATCH_NOTEWIN_INFO_U  selfInfo;
}WATCH_NOTEWIN_DATA_T;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define NOTEWIN_DISP_TIME_PERIOD        (1500)

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                     LOCAL FUNCTION DECLARATION                            */
/*---------------------------------------------------------------------------*/
LOCAL void CommonWin_NoteWin_DestoryData( WATCH_NOTEWIN_DATA_T **p_win_info );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION                                   */
/*---------------------------------------------------------------------------*/

LOCAL void HandleNoteWindowTimeoutCallBack(uint8 timer_id, uint32 param)
{
    MMI_WIN_ID_T            winId       = (MMI_WIN_ID_T)param;
    WATCH_NOTEWIN_DATA_T    *pWinInfo   = (WATCH_NOTEWIN_DATA_T *) MMK_GetWinAddDataPtr(winId);

    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("pWinInfo is null");
        return;
    }
    TRACE_APP_COMMON("enter notwin callback");
    if(pWinInfo->baseInfo.timerId == timer_id)
    {
        TRACE_APP_COMMON("pWinInfo->baseInfo.timerId == timer_id");
        if(PNULL != pWinInfo->baseInfo.userFunc)
        {
            pWinInfo->baseInfo.userFunc( winId, MSG_TIMER, PNULL );
        }
        MMK_CloseWin(winId);
    }
}

LOCAL MMI_RESULT_E HandleMsgNoteWinWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E            recode      = MMI_RESULT_TRUE;
    WATCH_NOTEWIN_DATA_T   *pWinInfo    = (WATCH_NOTEWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);

    if ( PNULL == pWinInfo )
    {
        TRACE_APP_COMMON("The win_info_ptr is PNULL!");
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            MMK_SetActiveCtrl(WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, FALSE);

            if(0 != pWinInfo->baseInfo.timerId)
            {
                MMK_StopTimer(pWinInfo->baseInfo.timerId);
                pWinInfo->baseInfo.timerId = 0;
            }
            if(pWinInfo->baseInfo.timeOutExit > 0)
            {
                pWinInfo->baseInfo.timerId = MMK_CreateTimerCallback( pWinInfo->baseInfo.timeOutExit ,HandleNoteWindowTimeoutCallBack,(uint32)win_id, FALSE);
            }            
        }
        break;

        case MSG_FULL_PAINT:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            if( pWinInfo->baseInfo.pwType == WATCH_COMMONWIN_TYPE_NOTE_FULLSCREEN )
            {
                WATCHCOM_DisplayBackground(win_id);
            }
            else
            {
                TRACE_APP_COMMON("pWinInfo->baseInfo.pwType = %d",pWinInfo->baseInfo.pwType);
            }
        }
        break;

        case MSG_NOTIFY_MIDSK:// BUG - 1332795，1333163
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            else
            {
                MMK_CloseWin(win_id);
            }
        }
        break;

        case MSG_CLOSE_WINDOW:
        {
            if(0 != pWinInfo->baseInfo.timerId)
            {
                MMK_StopTimer(pWinInfo->baseInfo.timerId);
                pWinInfo->baseInfo.timerId = 0;
            }
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            CommonWin_NoteWin_DestoryData(&pWinInfo);
        }
        break;

        default:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                recode = pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;
    }

    return recode;
}

LOCAL void CommonWin_NoteWin_FullScreen_Layout( WATCH_NOTEWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    dispRect.bottom = dispRect.right;

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.infoFullScreen.picRect, &layout.layoutNoteFullScreen.picRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.infoFullScreen.tipRect, &layout.layoutNoteFullScreen.tipRect,sizeof(GUI_RECT_T));

}

LOCAL void CommonWin_NoteWin_1Line_Layout( WATCH_NOTEWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = WATCH_NOTEWIN_TEXT_1LINE_RECT;

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1Line.tipRect, &layout.layoutNote1Line.tipRect,sizeof(GUI_RECT_T));
}

LOCAL void CommonWin_NoteWin_1Line_FullScreen_Layout( WATCH_NOTEWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    dispRect.bottom = dispRect.right;
    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1Line.tipRect, &layout.layoutNote1Line.tipRect,sizeof(GUI_RECT_T));
}

LOCAL void CommonWin_NoteWin_2Line_Layout( WATCH_NOTEWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = WATCH_NOTEWIN_TEXT_2LINE_RECT;

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info2Line.tipRect, &layout.layoutNote2Line.tipRect,sizeof(GUI_RECT_T));
}

LOCAL void CommonWin_NoteWin_CreateWin(
                                MMI_WIN_ID_T win_id,
                                WATCH_NOTEWIN_DATA_T *p_win_info
                                )
{
    MMI_WINDOW_CREATE_T winCreate = {0};

    if(PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL");
        return;
    }

    GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,LCD_ANGLE_0);
    winCreate.applet_handle = MMK_GetFirstAppletHandle();
    winCreate.win_id        = win_id;
    winCreate.func          = HandleMsgNoteWinWindow;

    winCreate.win_priority  = p_win_info->baseInfo.winPriority;
    winCreate.add_data_ptr  = p_win_info;
    winCreate.window_style  = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;

    //禁止硬图标响应
    if( p_win_info->baseInfo.winPriority>= WIN_ONE_LEVEL )
    {
        winCreate.window_style |= WS_DISABLE_HWICON;
    }

    winCreate.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;

    MMK_CreateWindow( &winCreate );

}

LOCAL void CommonWin_NoteWin_DestoryData( WATCH_NOTEWIN_DATA_T **p_win_info )
{
    if (PNULL != *p_win_info)
    {
        if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_NOTE_FULLSCREEN)
        {
            if (PNULL != (*p_win_info)->selfInfo.infoFullScreen.picStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.infoFullScreen.picStr.wstr_ptr);
                (*p_win_info)->selfInfo.infoFullScreen.picStr.wstr_ptr = PNULL;
            }
            if (PNULL != (*p_win_info)->selfInfo.infoFullScreen.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.infoFullScreen.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.infoFullScreen.tipStr.wstr_ptr = PNULL;
            }
        }
        else if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_NOTE_1LINE)
        {
            if (PNULL != (*p_win_info)->selfInfo.info1Line.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1Line.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.info1Line.tipStr.wstr_ptr = PNULL;
            }
        }
        else if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_NOTE_2LINE)
        {
            if (PNULL != (*p_win_info)->selfInfo.info2Line.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info2Line.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.info2Line.tipStr.wstr_ptr = PNULL;
            }
        }

        else
        {
            TRACE_APP_COMMON("(*p_win_info)->baseInfo.pwType = %d",(*p_win_info)->baseInfo.pwType);
        }
        SCI_FREE(*p_win_info);
        *p_win_info = PNULL;
    }
}

LOCAL WATCH_NOTEWIN_DATA_T *CommonWin_NoteWin_FullScreen_CreateData(
                                                                                    MMI_WIN_ID_T win_id,
                                                                                    MMI_STRING_T *pic_str,
                                                                                    MMI_STRING_T *tip_str,
                                                                                    MMI_IMAGE_ID_T pic_image,
                                                                                    MMI_IMAGE_ID_T tip_image,
                                                                                    WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                                    PROCESSMSG_FUNC userFunc
                                                                                )
{
    WATCH_NOTEWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_NOTEWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_NOTEWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_NOTE_FULLSCREEN;
    pWinInfo->baseInfo.winPriority    = WIN_ONE_LEVEL;
    pWinInfo->baseInfo.softkeyText.softkeyLeft  = softkey_text.softkeyLeft;
    pWinInfo->baseInfo.softkeyText.softkeyMiddle= softkey_text.softkeyMiddle;
    pWinInfo->baseInfo.softkeyText.softkeyRight = softkey_text.softkeyRight;
    pWinInfo->selfInfo.infoFullScreen.picImage = pic_image;
    pWinInfo->selfInfo.infoFullScreen.tipImage = tip_image;
    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( pic_str, &pWinInfo->selfInfo.infoFullScreen.picStr);
    if( result == FALSE)
    {
        CommonWin_NoteWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    result = CommonWin_CopyStringInfo( tip_str, &pWinInfo->selfInfo.infoFullScreen.tipStr);
    if( result == FALSE)
    {
        CommonWin_NoteWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}

LOCAL WATCH_NOTEWIN_DATA_T *CommonWin_NoteWin_1Line_CreateData(
                                                                            MMI_WIN_ID_T    win_id,
                                                                            MMI_STRING_T    *tip_str,
                                                                            MMI_IMAGE_ID_T  tip_image,
                                                                            WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                            PROCESSMSG_FUNC userFunc
                                                                          )
{
    WATCH_NOTEWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_NOTEWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_NOTEWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_NOTE_1LINE;
    pWinInfo->baseInfo.winPriority    = WIN_ONE_LEVEL;
    pWinInfo->baseInfo.softkeyText.softkeyLeft  = softkey_text.softkeyLeft;
    pWinInfo->baseInfo.softkeyText.softkeyMiddle= softkey_text.softkeyMiddle;
    pWinInfo->baseInfo.softkeyText.softkeyRight = softkey_text.softkeyRight;
    pWinInfo->selfInfo.info1Line.tipImage= tip_image;

    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( tip_str, &pWinInfo->selfInfo.info1Line.tipStr);
    if( result == FALSE)
    {
        CommonWin_NoteWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}

LOCAL WATCH_NOTEWIN_DATA_T *CommonWin_NoteWin_2Line_CreateData(
                                                                            MMI_WIN_ID_T    win_id,
                                                                            MMI_STRING_T    *tip_str,
                                                                            MMI_IMAGE_ID_T  tip_image,
                                                                            WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                            PROCESSMSG_FUNC userFunc
                                                                          )
{
    WATCH_NOTEWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_NOTEWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_NOTEWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_NOTE_2LINE;
    pWinInfo->baseInfo.winPriority    = WIN_ONE_LEVEL;
    pWinInfo->baseInfo.softkeyText.softkeyLeft  = softkey_text.softkeyLeft;
    pWinInfo->baseInfo.softkeyText.softkeyMiddle= softkey_text.softkeyMiddle;
    pWinInfo->baseInfo.softkeyText.softkeyRight = softkey_text.softkeyRight;
    pWinInfo->selfInfo.info2Line.tipImage= tip_image;

    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( tip_str, &pWinInfo->selfInfo.info2Line.tipStr);
    if( result == FALSE)
    {
        CommonWin_NoteWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}
#ifndef SCREEN_SHAPE_CIRCULAR
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
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);
    pWinInfo = CommonWin_NoteWin_FullScreen_CreateData( win_id, pic_str, tip_str, pic_image, tip_image, softkey_text, userFunc);
    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateLabelCtrl( win_id, WATCH_COMMON_NOTEWIN_PIC_LABEL_CTRL_ID);
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_NOTE_FULLSCREEN);

    //NoteWin layout
    CommonWin_NoteWin_FullScreen_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetLableStyle(
                            WATCH_COMMON_NOTEWIN_PIC_LABEL_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.picRect,
                            pic_str,
                            pic_image
                           );
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.tipRect,
                            tip_str,
                            tip_image
                           );
}

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
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo    = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              tipImage    = 0;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( tip_image != 0)
    {
        tipImage = tip_image;
    }
    else
    {
        tipImage = image_watch_notewin_1Line_tip_bg;
    }

    pWinInfo = CommonWin_NoteWin_1Line_CreateData( win_id, tip_str, tipImage, softkey_text, userFunc);

    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_NOTE_1LINE);

    //NoteWin layout
    CommonWin_NoteWin_1Line_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1Line.tipRect,
                            tip_str,
                            tipImage
                           );
}

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
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo    = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              tipImage    = 0;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( tip_image != 0)
    {
        tipImage = tip_image;
    }
    else
    {
        tipImage = image_watch_notewin_2Line_tip_bg;
    }

    pWinInfo = CommonWin_NoteWin_2Line_CreateData( win_id, tip_str, tipImage, softkey_text, userFunc);

    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_NOTE_2LINE);

    //NoteWin layout
    CommonWin_NoteWin_1Line_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info2Line.tipRect,
                            tip_str,
                            tipImage
                           );
}

#else
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
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {0};
#ifdef ADULT_WATCH_SUPPORT
    GUI_FONT_T  tempFont    = ADULT_WATCH_DEFAULT_NORMAL_FONT;
#else
    GUI_FONT_T  tempFont    = DP_FONT_22;
#endif
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);
    pWinInfo = CommonWin_NoteWin_FullScreen_CreateData( win_id, PNULL, tip_str, pic_image, IMAGE_NULL, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("notewin's pWinInfo is PNULL");
        return;
    }
    pWinInfo->baseInfo.timeOutExit = NOTEWIN_DISP_TIME_PERIOD;
    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateLabelCtrl( win_id, WATCH_COMMON_NOTEWIN_PIC_LABEL_CTRL_ID);
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_NOTE_1ICON_1STR);

    //NoteWin layout
    CommonWin_NoteWin_FullScreen_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetLableStyle(
                            WATCH_COMMON_NOTEWIN_PIC_LABEL_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.picRect,
                            PNULL,
                            pic_image
                           );
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.tipRect,
                            tip_str,
                            0
                           );
    GUITEXT_SetFont( WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );
}
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
//  Author: miao.liu2
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
                                                     )
{
    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(win_id, tip_str, pic_image, userFunc);
}

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
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo    = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              tipImage    = 0;
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {0};
#ifdef ADULT_WATCH_SUPPORT
    GUI_FONT_T  tempFont    = ADULT_WATCH_DEFAULT_NORMAL_FONT;
#else
    GUI_FONT_T  tempFont    = DP_FONT_20;
#endif
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( tip_image != 0)
    {
        tipImage = tip_image;
    }
    else
    {
#ifdef ADULT_WATCH_SUPPORT
        tipImage = res_aw_toast_1;
#else
        tipImage = res_common_toast_bg_1;
#endif
    }

    pWinInfo = CommonWin_NoteWin_1Line_CreateData( win_id, tip_str, tipImage, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("notewin's pWinInfo is PNULL");
        return;
    }
    pWinInfo->baseInfo.timeOutExit = NOTEWIN_DISP_TIME_PERIOD;
    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);


    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_NOTE_1LINE);

    //NoteWin layout
    CommonWin_NoteWin_1Line_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1Line.tipRect,
                            tip_str,
                            tipImage
                           );
    GUITEXT_SetFont( WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );
}

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
                                                     )
{
    Adult_WatchCOM_NoteWin_1Line_Toast_Enter(win_id, tip_str, tip_image, userFunc);
}

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
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo    = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              tipImage    = 0;
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {0};
#ifdef ADULT_WATCH_SUPPORT
    GUI_FONT_T  tempFont    = ADULT_WATCH_DEFAULT_NORMAL_FONT;
#else
    GUI_FONT_T  tempFont    = DP_FONT_20;
#endif
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( tip_image != 0)
    {
        tipImage = tip_image;
    }
    else
    {
#ifdef ADULT_WATCH_SUPPORT
        tipImage = res_aw_toast_2;
#else
        tipImage = res_common_toast_bg_2;
#endif
    }

    pWinInfo = CommonWin_NoteWin_2Line_CreateData( win_id, tip_str, tipImage, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("notewin's pWinInfo is PNULL");
        return;
    }
    pWinInfo->baseInfo.timeOutExit = NOTEWIN_DISP_TIME_PERIOD;
    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);


    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_NOTE_2LINE);

    //NoteWin layout
    CommonWin_NoteWin_1Line_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info2Line.tipRect,
                            tip_str,
                            tipImage
                           );
    GUITEXT_SetFont( WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );
}

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
PUBLIC void WatchCOM_NoteWin_2Line_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     )
{
    Adult_WatchCOM_NoteWin_2Line_Toast_Enter(win_id, tip_str, tip_image, userFunc);
}

#endif

/*****************************************************************************/
//  Description:WatchCOM_NoteWin_Set_Font
//  Parameter: [In] font_ptr font size
//             [In] font_color_ptr : font color
//             [Out] None
//             [Return] None
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_NoteWin_Set_Font(GUI_FONT_T *font_ptr,GUI_COLOR_T *font_color_ptr)
{
    GUITEXT_SetFont( WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, font_ptr, font_color_ptr );
}

PUBLIC void WatchCOM_NoteWin_Stop_TimeOut_Exit(MMI_WIN_ID_T    win_id)
{
    WATCH_NOTEWIN_DATA_T   *pWinInfo    = (WATCH_NOTEWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);
    if(pWinInfo != PNULL)
    {
         if(0 != pWinInfo->baseInfo.timerId)
        {
            MMK_StopTimer(pWinInfo->baseInfo.timerId);
            pWinInfo->baseInfo.timerId = 0;
        }
    }
}

/*****************************提示窗口************************************************/
//  Description: enter note window (full screen)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : label display string
//             [In] pic_image : picture display image
//             [In] userFunc : custom callback API
//             [In] timeOut : timeOut to close note windows
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_NoteWin_1Icon_1Str_Enter_Timeout(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_TEXT_ID_T    tip_id,
                                                        MMI_IMAGE_ID_T  pic_image,
                                                        PROCESSMSG_FUNC userFunc,
                                                        uint32 timeOut
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {0};
#ifdef ADULT_WATCH_SUPPORT
    GUI_FONT_T  tempFont    = ADULT_WATCH_DEFAULT_NORMAL_FONT;
#else
    GUI_FONT_T  tempFont    = DP_FONT_22;
#endif
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;
    MMI_STRING_T     tip_content      = {0}; 

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);
    MMI_GetLabelTextByLang(tip_id, &tip_content);
    pWinInfo = CommonWin_NoteWin_FullScreen_CreateData( win_id, PNULL, &tip_content, pic_image, IMAGE_NULL, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("notewin's pWinInfo is PNULL");
        return;
    }
    pWinInfo->baseInfo.timeOutExit = timeOut;
    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateLabelCtrl( win_id, WATCH_COMMON_NOTEWIN_PIC_LABEL_CTRL_ID);
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_NOTE_1ICON_1STR);

    //NoteWin layout
    CommonWin_NoteWin_FullScreen_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetLableStyle(
                            WATCH_COMMON_NOTEWIN_PIC_LABEL_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.picRect,
                            PNULL,
                            pic_image
                           );
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.tipRect,
                            &tip_content,
                            0
                           );
    GUITEXT_SetFont( WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );
}
/*******************************提示窗口 timeOut 为0 时不退出**********************************************/
//  Description: enter note window (full screen)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_id : label display string id
//             [In] tip_image : label display image
//             [In] userFunc : custom callback API
//             [In] timeOut : timeOut to close note windows
//             [Out] None
//             [Return] None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_NoteWin_FullScreen_Ente_Timeout(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_TEXT_ID_T    tip_id,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        PROCESSMSG_FUNC userFunc,
                                                        uint32 timeOut
                                                     )
{
    WatchCOM_NoteWin_1Icon_1Str_Enter_Timeout(win_id, tip_id, tip_image, userFunc, timeOut);
}

/*****************************************************************************/
//  Description: enter note window (only one line)
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_id : label display string id
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_NoteWin_1Line_Enter_Timeout(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_TEXT_ID_T    tip_id,
                                                        PROCESSMSG_FUNC userFunc,
                                                        uint32 timeOut
                                                     )
{
    WATCH_NOTEWIN_DATA_T        *pWinInfo    = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              tipImage    = 0;
    MMI_STRING_T     tip_content      = {0}; 
    WATCH_SOFTKEY_TEXT_ID_T   softket_text = {TXT_NULL,TXT_NULL,TXT_NULL};
    GUI_RECT_T tipTempRect  = MMITHEME_GetFullScreenRect();

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("notewin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);
                            
    MMI_GetLabelTextByLang(tip_id, &tip_content);
    pWinInfo = CommonWin_NoteWin_1Line_CreateData( win_id, &tip_content, 0, softket_text, userFunc);
    pWinInfo->baseInfo.timeOutExit = timeOut;
    CommonWin_NoteWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID);

    //get querywin layout
    //CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_NOTE_1LINE);
    SCI_MEMCPY(&styleLayout.layoutNote1Line.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );

    //NoteWin layout
    CommonWin_NoteWin_1Line_FullScreen_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, pWinInfo->selfInfo.info1Line.tipRect, &tip_content,0);
}


#ifdef ADULT_WATCH_SUPPORT
PUBLIC void Adult_WatchCOM_NoteWin_Set_Font(	GUI_FONT_T *font_ptr,GUI_COLOR_T *font_color_ptr)
{
    GUITEXT_SetFont( WATCH_COMMON_NOTEWIN_TIP_TEXTBOX_CTRL_ID, font_ptr, font_color_ptr );
}
#endif
