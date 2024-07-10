/*****************************************************************************
** File Name:       watch_commonwin_waiting.c                                *
** Author:          fangfang.yao                                             *
** Date:            2020/03/09                                               *
** Copyright:                                                                *
** Description:     This file is used to process msg to main window of       *
**                  waitingwin                                               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION
** 03/09/2020           fangfang.yao          Create
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
#include "mmi_common.h"
#include "guitext.h"
#include "mmi_anim.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct
{
    MMI_STRING_T                picStr;
    MMI_IMAGE_ID_T              picImage;
    GUI_RECT_T                  picRect;
}WATCH_WAITINGWIN_INFO_FULLSCREEN_T;

typedef struct
{
    MMI_STRING_T                tipStr;
    MMI_IMAGE_ID_T              tipImage;
    GUI_RECT_T                  tipRect;
}WATCH_WAITINGWIN_INFO_1LINE_T;

typedef struct
{
    MMI_STRING_T                tipStr;
    MMI_IMAGE_ID_T              picImage;
    GUI_RECT_T                  tipRect;
    GUI_RECT_T                  picRect;
}WATCH_WAITINGWIN_INFO_1ICON_1STR_FULLSCREEN_T;

typedef union
{
    WATCH_WAITINGWIN_INFO_FULLSCREEN_T infoFullScreen;
    WATCH_WAITINGWIN_INFO_1LINE_T      info1Line;
    WATCH_WAITINGWIN_INFO_1ICON_1STR_FULLSCREEN_T      info1icon1str;

}WATCH_WAITINGWIN_INFO_U;

typedef struct
{
    uint32 timeOut;
    WATCH_COMMONWIN_INFO_T     baseInfo;
    WATCH_WAITINGWIN_INFO_U selfInfo;
}WATCH_WAITINGWIN_DATA_T;


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                     LOCAL FUNCTION DECLARATION                            */
/*---------------------------------------------------------------------------*/
LOCAL void CommonWin_WaitingWin_DestoryData( WATCH_WAITINGWIN_DATA_T **p_win_info );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION                                   */
/*---------------------------------------------------------------------------*/

LOCAL void HandleWaitingWindowTimeoutCallBack(uint8 timer_id, uint32 param)
{
    MMI_WIN_ID_T            winId       = (MMI_WIN_ID_T)param;
    WATCH_WAITINGWIN_DATA_T    *pWinInfo   = (WATCH_WAITINGWIN_DATA_T *) MMK_GetWinAddDataPtr(winId);

    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("pWinInfo is null");
        return;
    }
    TRACE_APP_COMMON("enter waiting win callback");
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

LOCAL MMI_RESULT_E HandleMsgWaitingWinWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E            recode      = MMI_RESULT_TRUE;
    WATCH_WAITINGWIN_DATA_T *pWinInfo   = (WATCH_WAITINGWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);

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
            if( WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN == pWinInfo->baseInfo.pwType)
            {
                MMK_SetActiveCtrl(WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID, FALSE);
            }
            else if( WATCH_COMMONWIN_TYPE_WAITING_1LINE == pWinInfo->baseInfo.pwType)
            {
                MMK_SetActiveCtrl(WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID, FALSE);
            }
            else if( WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR == pWinInfo->baseInfo.pwType)
            {
                MMK_SetActiveCtrl(WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID, FALSE);
            }

#ifdef ADULT_WATCH_SUPPORT
            else if( ADULT_WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR == pWinInfo->baseInfo.pwType)
            {
                MMK_SetActiveCtrl(WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID, FALSE);
            }
#endif
            else
            {
                TRACE_APP_COMMON("pWinInfo->baseInfo.pwType = %d!",pWinInfo->baseInfo.pwType);
            }
            if(0 != pWinInfo->baseInfo.timerId)
            {
                MMK_StopTimer(pWinInfo->baseInfo.timerId);
                pWinInfo->baseInfo.timerId = 0;
            }
            if(pWinInfo->timeOut > 500)
            {
                pWinInfo->baseInfo.timerId = MMK_CreateTimerCallback(pWinInfo->timeOut ,HandleWaitingWindowTimeoutCallBack,(uint32)win_id, FALSE);
            }           
        }
        break;

        case MSG_FULL_PAINT:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            if( (pWinInfo->baseInfo.pwType == WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN)
                || (pWinInfo->baseInfo.pwType == WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR)
#ifdef ADULT_WATCH_SUPPORT
             || (pWinInfo->baseInfo.pwType == ADULT_WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR)
             || (pWinInfo->baseInfo.pwType == ADULT_WATCH_COMMONWIN_TYPE_WAITING_1STR_WAVEICON)
#endif
            )
            {
                WATCHCOM_DisplayBackground(win_id);
            }
            else
            {
                TRACE_APP_COMMON("pWinInfo->baseInfo.pwType = %d",pWinInfo->baseInfo.pwType);
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
            CommonWin_WaitingWin_DestoryData(&pWinInfo);
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

LOCAL void CommonWin_WaitingWin_FullScreen_Layout( WATCH_WAITINGWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    dispRect.bottom = dispRect.right;
    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY( &p_win_info->selfInfo.infoFullScreen.picRect, &layout.layoutWaitingFullScreen.picRect, sizeof(GUI_RECT_T));
}

LOCAL void CommonWin_WaitingWin_1Line_Layout( WATCH_WAITINGWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = WATCH_WAITINGWIN_TEXT_1LINE_RECT;

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1Line.tipRect, &layout.layoutWaiting1Line.tipRect,sizeof(GUI_RECT_T));
}

LOCAL void CommonWin_WaitingWin_1Icon_1Str_Layout( WATCH_WAITINGWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1icon1str.tipRect, &layout.layoutWaiting1Icon1Str.tipRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1icon1str.picRect, &layout.layoutWaiting1Icon1Str.picRect,sizeof(GUI_RECT_T));
}

LOCAL void CommonWin_WaitingWin_CreateWin( MMI_WIN_ID_T win_id, WATCH_WAITINGWIN_DATA_T *p_win_info )
{
    MMI_WINDOW_CREATE_T winCreate = {0};

    if(PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL");
        //return;
    }

    GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,LCD_ANGLE_0);
    winCreate.applet_handle = MMK_GetFirstAppletHandle();
    winCreate.win_id = win_id;
    winCreate.func = HandleMsgWaitingWinWindow;

    winCreate.win_priority = p_win_info->baseInfo.winPriority;
    winCreate.add_data_ptr = p_win_info;
    winCreate.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;

    //½ûÖ¹Ó²Í¼±êÏìÓ¦
    if( p_win_info->baseInfo.winPriority>= WIN_ONE_LEVEL )
    {
        winCreate.window_style |= WS_DISABLE_HWICON;
    }

    winCreate.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;

    MMK_CreateWindow( &winCreate );

}

LOCAL void CommonWin_WaitingWin_DestoryData( WATCH_WAITINGWIN_DATA_T **p_win_info )
{
    if (PNULL != *p_win_info)
    {
        if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN)
        {
            if (PNULL != (*p_win_info)->selfInfo.infoFullScreen.picStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.infoFullScreen.picStr.wstr_ptr);
                (*p_win_info)->selfInfo.infoFullScreen.picStr.wstr_ptr = PNULL;
            }
        }
        else if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_WAITING_1LINE)
        {
            if (PNULL != (*p_win_info)->selfInfo.info1Line.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1Line.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.info1Line.tipStr.wstr_ptr = PNULL;
            }
        }
        else if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR)
        {
            if (PNULL != (*p_win_info)->selfInfo.info1icon1str.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1icon1str.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.info1icon1str.tipStr.wstr_ptr = PNULL;
            }
        }
#ifdef ADULT_WATCH_SUPPORT
        else if((*p_win_info)->baseInfo.pwType == ADULT_WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR)
        {
            if (PNULL != (*p_win_info)->selfInfo.info1icon1str.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1icon1str.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.info1icon1str.tipStr.wstr_ptr = PNULL;
            }
        }
#endif
        else
        {
            TRACE_APP_COMMON("(*p_win_info)->baseInfo.pwType = %d",(*p_win_info)->baseInfo.pwType);
        }
        SCI_FREE(*p_win_info);
        *p_win_info = PNULL;
    }
}

LOCAL WATCH_WAITINGWIN_DATA_T *CommonWin_WaitingWin_FullScreen_CreateData(
                                                                                    MMI_WIN_ID_T win_id,
                                                                                    MMI_STRING_T *pic_str,
                                                                                    MMI_IMAGE_ID_T pic_image,
                                                                                    WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                                    PROCESSMSG_FUNC userFunc
                                                                                )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_WAITINGWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_WAITINGWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN;
    pWinInfo->baseInfo.winPriority    = WIN_ONE_LEVEL;
    pWinInfo->baseInfo.softkeyText.softkeyLeft  = softkey_text.softkeyLeft;
    pWinInfo->baseInfo.softkeyText.softkeyMiddle= softkey_text.softkeyMiddle;
    pWinInfo->baseInfo.softkeyText.softkeyRight = softkey_text.softkeyRight;
    pWinInfo->selfInfo.infoFullScreen.picImage = pic_image;
    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( pic_str, &pWinInfo->selfInfo.infoFullScreen.picStr);
    if( result == FALSE)
    {
        CommonWin_WaitingWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}

LOCAL WATCH_WAITINGWIN_DATA_T *CommonWin_WaitingWin_1Line_CreateData(
                                                                            MMI_WIN_ID_T    win_id,
                                                                            MMI_STRING_T    *tip_str,
                                                                            MMI_IMAGE_ID_T  tip_image,
                                                                            WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                            PROCESSMSG_FUNC userFunc
                                                                          )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_WAITINGWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_WAITINGWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_WAITING_1LINE;
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
        CommonWin_WaitingWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}


LOCAL WATCH_WAITINGWIN_DATA_T *CommonWin_WaitingWin_1Icon_1Str_CreateData(
                                                                            MMI_WIN_ID_T    win_id,
                                                                            MMI_STRING_T    *str,
                                                                            MMI_IMAGE_ID_T  image,
                                                                            PROCESSMSG_FUNC userFunc
                                                                          )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_WAITINGWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_WAITINGWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
#if defined ADULT_WATCH_SUPPORT
    pWinInfo->baseInfo.pwType         = ADULT_WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR;
#else // defined SCREEN_SHAPE_CIRCULAR
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR;
#endif
    pWinInfo->baseInfo.winPriority    = WIN_ONE_LEVEL;
    pWinInfo->selfInfo.info1icon1str.picImage= image;

    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( str, &pWinInfo->selfInfo.info1icon1str.tipStr);
    if( result == FALSE)
    {
        CommonWin_WaitingWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}


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
                                                     )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T          picImage    = 0;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("waitingwin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Waiting_EnterWin,win_id is 0x%x",win_id);
    if( pic_image != 0)
    {
        picImage = pic_image;
    }
    else
    {
        picImage = image_watch_waitingwin_fullscreen_pic;
    }
    pWinInfo = CommonWin_WaitingWin_FullScreen_CreateData( win_id, pic_str, picImage, softkey_text, userFunc);
    CommonWin_WaitingWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateLabelCtrl( win_id, WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID);

    //get waitingwin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN);

    //WaitingWin layout
    CommonWin_WaitingWin_FullScreen_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetLableStyle(
                            WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.picRect,
                            pic_str,
                            picImage
                           );
}

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
                                                     )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T          tipImage    = 0;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("waitingwin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Waiting_EnterWin,win_id is 0x%x",win_id);

    if( tip_image != 0)
    {
        tipImage = tip_image;
    }
    else
    {
#if defined SCREEN_SHAPE_CIRCULAR
        tipImage = res_common_toast_bg_1;
#elif defined SCREEN_SHAPE_SQUARE
        tipImage = image_watch_waitingwin_1line_tip_bg;
#endif
    }

    pWinInfo = CommonWin_WaitingWin_1Line_CreateData( win_id, tip_str, tipImage, softkey_text, userFunc);

    CommonWin_WaitingWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID);

    //get waitingwin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_WAITING_1LINE);

    //WaitingWin layout
    CommonWin_WaitingWin_1Line_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1Line.tipRect,
                            tip_str,
                            tipImage
                           );
}


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
PUBLIC void WatchCOM_WaitingWin_1Icon_1Str_EX(
                                                        MMI_WIN_ID_T          win_id,
                                                        MMI_STRING_T         *tip_str,
                                                        GUI_RECT_T              *txt_rect,
                                                        MMI_IMAGE_ID_T      img_id,
                                                        GUI_RECT_T              *img_rect,
                                                        PROCESSMSG_FUNC  userFunc
                                                     )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    GUI_FONT_T  tempFont    = WATCH_FONT_22;
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("waitingwin is open");
        return;
    }
    else if(tip_str == PNULL && img_id == NULL)
    {
            TRACE_APP_COMMON("Error: image and text both null, return.");
    }

    TRACE_APP_COMMON("EnterWin,win_id is 0x%x",win_id);

    pWinInfo = CommonWin_WaitingWin_1Icon_1Str_CreateData( win_id, tip_str, img_id, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("waitingwin's pWinInfo is PNULL");
        return;
    }

    CommonWin_WaitingWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateLabelCtrl( win_id, WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID);
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID);

    //get waitingwin layout
    CommonWin_GetLayoutByType(&styleLayout, WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR);

    if(txt_rect != PNULL)
    {
        styleLayout.layoutWaiting1Icon1Str.tipRect = *txt_rect;
    }

    if(img_rect != PNULL)
   {
        styleLayout.layoutWaiting1Icon1Str.picRect = *img_rect;
    }

    //WaitingWin layout
    CommonWin_WaitingWin_1Icon_1Str_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    if(tip_str != PNULL)
    {
        //TRACE_APP_COMMON("tipRect [%d,%d,%d,%d]",
        //                                        pWinInfo->selfInfo.info1icon1str.tipRect.left,
        //                                        pWinInfo->selfInfo.info1icon1str.tipRect.top,
        //                                        pWinInfo->selfInfo.info1icon1str.tipRect.right ,
        //                                        pWinInfo->selfInfo.info1icon1str.tipRect.bottom);
        CommonWin_SetTextBoxStyle(
                                WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID,
                                pWinInfo->selfInfo.info1icon1str.tipRect,
                                tip_str,
                                0
                               );
        GUITEXT_SetFont( WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );

    }

    if(img_id != NULL)
    {
            //TRACE_APP_COMMON("picRect [%d,%d,%d,%d]",
           //                                     pWinInfo->selfInfo.info1icon1str.picRect.left,
           //                                     pWinInfo->selfInfo.info1icon1str.picRect.top,
           //                                     pWinInfo->selfInfo.info1icon1str.picRect.right ,
           //                                     pWinInfo->selfInfo.info1icon1str.picRect.bottom);
        CommonWin_SetLableStyle(
                                WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID,
                                pWinInfo->selfInfo.info1icon1str.picRect,
                                PNULL,
                                img_id
                               );

    }

}
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
                                                     )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T          picImage    = 0;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("waitingwin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Waiting_EnterWin,win_id is 0x%x",win_id);
    if( pic_image != 0)
    {
        picImage = pic_image;
    }
    else
    {
        picImage = image_watch_waitingwin_fullscreen_pic;
    }
    pWinInfo = CommonWin_WaitingWin_FullScreen_CreateData( win_id, pic_str, picImage, softkey_text, userFunc);
    pWinInfo->timeOut = timeOut;
    CommonWin_WaitingWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateLabelCtrl( win_id, WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID);

    //get waitingwin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN);

    //WaitingWin layout
    CommonWin_WaitingWin_FullScreen_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetLableStyle(
                            WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID,
                            pWinInfo->selfInfo.infoFullScreen.picRect,
                            pic_str,
                            picImage
                           );
}


#ifdef ADULT_WATCH_SUPPORT

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
PUBLIC void AdultWatchCOM_WaitingWin_1Icon_1Str_Enter(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_STRING_T    *tip_str,
                                                        MMI_IMAGE_ID_T  tip_image,
                                                        PROCESSMSG_FUNC userFunc
                                                     )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T          tipImage    = 0;
    GUI_FONT_T  tempFont    = ADULT_WATCH_DEFAULT_NORMAL_FONT;
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("waitingwin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Waiting_EnterWin,win_id is 0x%x",win_id);

    if( tip_image != 0)
    {
        tipImage = tip_image;
    }
    else
    {
        tipImage = image_watch_waitingwin_1line_tip_bg;
    }

    pWinInfo = CommonWin_WaitingWin_1Icon_1Str_CreateData( win_id, tip_str, tipImage, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("waitingwin's pWinInfo is PNULL");
        return;
    }

    CommonWin_WaitingWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateLabelCtrl( win_id, WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID);
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID);

    //get waitingwin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR);

    //WaitingWin layout
    CommonWin_WaitingWin_1Icon_1Str_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1icon1str.tipRect,
                            tip_str,
                            0
                           );
    GUITEXT_SetFont( WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );
    CommonWin_SetLableStyle(
                            WATCH_COMMON_WAITINGWIN_PIC_LABEL_CTRL_ID,
                            pWinInfo->selfInfo.info1icon1str.picRect,
                            PNULL,
                            tip_image
                           );
}
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
                                                     )
{
    WATCH_WAITINGWIN_DATA_T *pWinInfo   = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T          tipImage    = 0;
    WATCH_SOFTKEY_TEXT_ID_T     softkey_text = {0};
    GUI_FONT_T  tempFont    = ADULT_WATCH_DEFAULT_NORMAL_FONT;
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("waitingwin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Waiting_EnterWin,win_id is 0x%x",win_id);

    if( tip_image != 0)
    {
        tipImage = tip_image;
    }
    else
    {
        tipImage = 0;
    }

    pWinInfo = CommonWin_WaitingWin_1Line_CreateData( win_id, tip_str, tipImage, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("waitingwin's pWinInfo is PNULL");
        return;
    }
    CommonWin_WaitingWin_CreateWin(win_id,pWinInfo);


    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID);

    //get waitingwin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_WAITING_1LINE);

    //WaitingWin layout
    CommonWin_WaitingWin_1Line_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1Line.tipRect,
                            tip_str,
                            tipImage
                           );
    GUITEXT_SetFont( WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );
}

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
                                                        )
{
    WATCH_WAITINGWIN_DATA_T  *pWinInfo     = PNULL;
    WATCH_COMMONWIN_LAYOUT_T styleLayout   = {0};
    MMI_IMAGE_ID_T           image         = ADULT_WATCH_WAITING_ANIM;
    GUI_FONT_T               tempFont      = ADULT_WATCH_DEFAULT_NORMAL_FONT;
    GUI_COLOR_T              fontColor     = WATCH_COMMONWIN_FONT_COLOR;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("waitingwin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Waiting_EnterWin,win_id is 0x%x",win_id);

    pWinInfo = CommonWin_WaitingWin_1Icon_1Str_CreateData( win_id, str, image, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("waitingwin's pWinInfo is PNULL");
        return;
    }

    CommonWin_WaitingWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateAnimCtrl( win_id, ADULT_WATCH_COMMON_WAITINGWIN_PIC_ANIM_CTRL_ID);
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID);

    //get waitingwin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_WAITING_1STR_WAVEICON);

    //WaitingWin layout
    CommonWin_WaitingWin_1Icon_1Str_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1icon1str.tipRect,
                            str,
                            0
                            );
    GUITEXT_SetFont( WATCH_COMMON_WAITINGWIN_TIP_TEXTBOX_CTRL_ID, &tempFont, &fontColor );

    CommonWin_SetAnimStyle(ADULT_WATCH_COMMON_WAITINGWIN_PIC_ANIM_CTRL_ID,
                           pWinInfo->selfInfo.info1icon1str.picRect,
                           image);

}
#endif
