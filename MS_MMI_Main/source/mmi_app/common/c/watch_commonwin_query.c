/*****************************************************************************
** File Name:       watch_commonwin_query.c                                  *
** Author:          fangfang.yao                                             *
** Date:            2020/02/28                                               *
** Copyright:                                                                *
** Description:     This file is used to process msg to main window of       *
**                  querywin                                                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION
** 02/28/2020           fangfang.yao          Create
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
#include "guibutton.h"
#include "guilabel.h"
#include "mmicom_trace.h"
#include "watch_common.h"
#include "watch_commonwin_export.h"
#include "watch_commonwin_internal.h"
#include "guires.h"
#include "guitext.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct
{
    MMI_STRING_T                tipStr;
    MMI_STRING_T                btnStr;
    MMI_IMAGE_ID_T              btnImage;
    GUI_RECT_T                  tipRect;
    GUI_RECT_T                  btnRect;
}WATCH_QUERYWIN_INFO_1BTN_T;

typedef struct
{
    MMI_STRING_T                tipStr1;
    MMI_STRING_T                tipStr2;
    MMI_STRING_T                btnStr;
    MMI_IMAGE_ID_T              btnImage;
    GUI_RECT_T                  tipRect1;
    GUI_RECT_T                  tipRect2;
    GUI_RECT_T                  btnRect;
}WATCH_QUERYWIN_INFO_1BTN_WITH2STR_T;

typedef struct
{
    MMI_STRING_T                tipStr;
    MMI_STRING_T                leftBtnStr;
    MMI_STRING_T                rightBtnStr;
    MMI_IMAGE_ID_T              leftBtnImage;
    MMI_IMAGE_ID_T              rightBtnImage;
    GUI_RECT_T                  tipRect;
    GUI_RECT_T                  leftBtnRect;
    GUI_RECT_T                  rightBtnRect;
}WATCH_QUERYWIN_INFO_2BTN_T;

typedef union
{
    WATCH_QUERYWIN_INFO_1BTN_T          info1Btn;
    WATCH_QUERYWIN_INFO_1BTN_WITH2STR_T info1BtnWith2Str;
    WATCH_QUERYWIN_INFO_2BTN_T          info2Btn;
}WATCH_QUERYWIN_INFO_U;

typedef struct
{
    WATCH_COMMONWIN_INFO_T  baseInfo;
    WATCH_QUERYWIN_INFO_U selfInfo;
}WATCH_QUERYWIN_DATA_T;

#define WATCH_QUERYWIN_NOIMAGE_BTN_LENGTH   DP2PX_VALUE(120)// BUG - 1327489

/*---------------------------------------------------------------------------*/
/*                     LOCAL FUNCTION DECLARATION                            */
/*---------------------------------------------------------------------------*/
LOCAL void CommonWin_QueryWin_DestoryData( WATCH_QUERYWIN_DATA_T **p_win_info );
LOCAL MMI_MESSAGE_ID_E Handle_TOUCH_PANEL_SUPPORT(MMI_MESSAGE_ID_E msg_id, 
                                                                                                            DPARAM param, 
                                                                                                            WATCH_QUERYWIN_DATA_T   *pWinInfo);
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION                                   */
/*---------------------------------------------------------------------------*/
LOCAL MMI_MESSAGE_ID_E Handle_TOUCH_PANEL_SUPPORT(MMI_MESSAGE_ID_E msg_id, 
                                                                                                               DPARAM param, 
                                                                                                               WATCH_QUERYWIN_DATA_T   *pWinInfo)
{
    switch(msg_id)
    {
        case MSG_NOTIFY_BUTTON_DOWN:
        {
            if((pWinInfo->baseInfo.pwType == WATCH_COMMONWIN_TYPE_QUERY_1BTN) || (pWinInfo->baseInfo.pwType == WATCH_COMMONWIN_TYPE_QUERY_1BTN_WITH2STR))
            {
                if(WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id
#ifdef ADULT_WATCH_SUPPORT
                 || ADULT_WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id
#endif
			)//button被选中
                {
                    return MSG_NOTIFY_MIDSK;
                }
            }
            else if(pWinInfo->baseInfo.pwType == WATCH_COMMONWIN_TYPE_QUERY_2BTN)
            {
                if(WATCH_COMMON_QUERYWIN_LEFTBTN_2BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id
#ifdef ADULT_WATCH_SUPPORT
                 || ADULT_WATCH_COMMON_QUERYWIN_LEFTBTN_2BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id
#endif
                  )//left button被选中
                {
                    return MSG_NOTIFY_OK;
                }
                else if(WATCH_COMMON_QUERYWIN_RIGHTBTN_2BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id
#ifdef ADULT_WATCH_SUPPORT
                 || ADULT_WATCH_COMMON_QUERYWIN_RIGHTBTN_2BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id
#endif
)//right button被选中
                {
                    return MSG_NOTIFY_CANCEL;
                }
            }
            else
            {
                TRACE_APP_COMMON("pWinInfo->baseInfo.pwType = %d",pWinInfo->baseInfo.pwType);
            }
        }
        return msg_id;
        break;
        default:
            return msg_id;
            break;
    }
}
LOCAL MMI_RESULT_E HandleMsgQueryWinWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E            recode      = MMI_RESULT_TRUE;
    WATCH_QUERYWIN_DATA_T   *pWinInfo   = (WATCH_QUERYWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);

    if ( PNULL == pWinInfo )
    {
        TRACE_APP_COMMON("The win_info_ptr is PNULL!");
        return MMI_RESULT_FALSE;
    }
#ifdef TOUCH_PANEL_SUPPORT
    msg_id = Handle_TOUCH_PANEL_SUPPORT(msg_id, param, pWinInfo);   
#endif //TOUCH_PANEL_SUPPORT

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            MMK_SetActiveCtrl(WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID, FALSE);
        }
        break;

        case MSG_FULL_PAINT:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            WATCHCOM_DisplayBackground(win_id);
        }
        break;

        case MSG_CLOSE_WINDOW:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            CommonWin_QueryWin_DestoryData(&pWinInfo);
        }
        break;

        case MSG_NOTIFY_OK:
        case MSG_NOTIFY_CANCEL:
        case MSG_NOTIFY_MIDSK:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                recode = pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            else
            {
                MMK_CloseWin( win_id );
            }
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

LOCAL void CommonWin_QueryWin_1Btn_Layout( WATCH_QUERYWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    dispRect.bottom = dispRect.right;

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1Btn.tipRect, &layout.layoutQuery1Btn.tipRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1Btn.btnRect, &layout.layoutQuery1Btn.btnRect,sizeof(GUI_RECT_T));

}

LOCAL void CommonWin_QueryWin_1Btn_With2Str_Layout( WATCH_QUERYWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    dispRect.bottom = dispRect.right;

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1BtnWith2Str.tipRect1, &layout.layoutQuery1BtnWith2Str.tipRect1,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1BtnWith2Str.tipRect2, &layout.layoutQuery1BtnWith2Str.tipRect2,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info1BtnWith2Str.btnRect, &layout.layoutQuery1BtnWith2Str.btnRect,sizeof(GUI_RECT_T));

}

LOCAL void CommonWin_QueryWin_2Btn_Layout( WATCH_QUERYWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    dispRect.bottom = dispRect.right;

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info2Btn.tipRect, &layout.layoutQuery2Btn.tipRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info2Btn.leftBtnRect, &layout.layoutQuery2Btn.leftBtnRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.info2Btn.rightBtnRect, &layout.layoutQuery2Btn.rightBtnRect,sizeof(GUI_RECT_T));
    if( p_win_info->selfInfo.info2Btn.leftBtnImage == 0 && p_win_info->selfInfo.info2Btn.rightBtnImage == 0 )// BUG - 1327489
    {
        p_win_info->selfInfo.info2Btn.leftBtnRect.left = 0;
        p_win_info->selfInfo.info2Btn.leftBtnRect.right= WATCH_QUERYWIN_NOIMAGE_BTN_LENGTH - 1;
        p_win_info->selfInfo.info2Btn.rightBtnRect.left = WATCH_QUERYWIN_NOIMAGE_BTN_LENGTH;
        p_win_info->selfInfo.info2Btn.rightBtnRect.right= dispRect.right;
    }

}

LOCAL void CommonWin_QueryWin_CreateWin(
                                MMI_WIN_ID_T win_id,
                                WATCH_QUERYWIN_DATA_T *p_win_info
                                )
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
    winCreate.func = HandleMsgQueryWinWindow;

    winCreate.win_priority = p_win_info->baseInfo.winPriority;
    winCreate.add_data_ptr = p_win_info;
    winCreate.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;

    //禁止硬图标响应
    if( p_win_info->baseInfo.winPriority>= WIN_ONE_LEVEL )
    {
        winCreate.window_style |= WS_DISABLE_HWICON;
    }

    winCreate.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;

    MMK_CreateWindow( &winCreate );

}

LOCAL void CommonWin_QueryWin_DestoryData( WATCH_QUERYWIN_DATA_T **p_win_info )
{
    if (PNULL != *p_win_info)
    {
        if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_QUERY_1BTN)
        {
            if (PNULL != (*p_win_info)->selfInfo.info1Btn.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1Btn.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.info1Btn.tipStr.wstr_ptr = PNULL;
            }
            if (PNULL != (*p_win_info)->selfInfo.info1Btn.btnStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1Btn.btnStr.wstr_ptr);
                (*p_win_info)->selfInfo.info1Btn.btnStr.wstr_ptr = PNULL;
            }
        }
        else if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_QUERY_1BTN_WITH2STR)
        {
            if (PNULL != (*p_win_info)->selfInfo.info1BtnWith2Str.tipStr1.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1BtnWith2Str.tipStr1.wstr_ptr);
                (*p_win_info)->selfInfo.info1BtnWith2Str.tipStr1.wstr_ptr = PNULL;
            }
            if (PNULL != (*p_win_info)->selfInfo.info1BtnWith2Str.tipStr2.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1BtnWith2Str.tipStr2.wstr_ptr);
                (*p_win_info)->selfInfo.info1BtnWith2Str.tipStr2.wstr_ptr = PNULL;
            }
            if (PNULL != (*p_win_info)->selfInfo.info1BtnWith2Str.btnStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info1BtnWith2Str.btnStr.wstr_ptr);
                (*p_win_info)->selfInfo.info1BtnWith2Str.btnStr.wstr_ptr = PNULL;
            }
        }
        else if((*p_win_info)->baseInfo.pwType == WATCH_COMMONWIN_TYPE_QUERY_2BTN)
        {
            if (PNULL != (*p_win_info)->selfInfo.info2Btn.tipStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info2Btn.tipStr.wstr_ptr);
                (*p_win_info)->selfInfo.info2Btn.tipStr.wstr_ptr = PNULL;
            }
            if (PNULL != (*p_win_info)->selfInfo.info2Btn.leftBtnStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info2Btn.leftBtnStr.wstr_ptr);
                (*p_win_info)->selfInfo.info2Btn.leftBtnStr.wstr_ptr = PNULL;
            }
            if (PNULL != (*p_win_info)->selfInfo.info2Btn.rightBtnStr.wstr_ptr)
            {
                SCI_FREE((*p_win_info)->selfInfo.info2Btn.rightBtnStr.wstr_ptr);
                (*p_win_info)->selfInfo.info2Btn.rightBtnStr.wstr_ptr = PNULL;
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

LOCAL WATCH_QUERYWIN_DATA_T *CommonWin_QueryWin_1Btn_CreateData(
                                                                                MMI_WIN_ID_T win_id,
                                                                                MMI_STRING_T *tip_str,
                                                                                MMI_STRING_T *btn_str,
                                                                                MMI_IMAGE_ID_T btn_image,
                                                                                WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                                PROCESSMSG_FUNC userFunc
                                                                            )
{
    WATCH_QUERYWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_QUERYWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_QUERYWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_QUERY_1BTN;
    pWinInfo->baseInfo.winPriority    = WIN_ONE_LEVEL;
    pWinInfo->baseInfo.softkeyText.softkeyLeft  = softkey_text.softkeyLeft;
    pWinInfo->baseInfo.softkeyText.softkeyMiddle= softkey_text.softkeyMiddle;
    pWinInfo->baseInfo.softkeyText.softkeyRight = softkey_text.softkeyRight;
    pWinInfo->selfInfo.info1Btn.btnImage = btn_image;
    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( tip_str, &pWinInfo->selfInfo.info1Btn.tipStr);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    result = CommonWin_CopyStringInfo( btn_str, &pWinInfo->selfInfo.info1Btn.btnStr);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}

LOCAL WATCH_QUERYWIN_DATA_T *CommonWin_QueryWin_1Btn_With2Str_CreateData(
                                                                                MMI_WIN_ID_T win_id,
                                                                                MMI_STRING_T *tip_str1,
                                                                                MMI_STRING_T *tip_str2,
                                                                                MMI_STRING_T *btn_str,
                                                                                MMI_IMAGE_ID_T btn_image,
                                                                                WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                                PROCESSMSG_FUNC userFunc
                                                                            )
{
    WATCH_QUERYWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_QUERYWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_QUERYWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId          = win_id;
    pWinInfo->baseInfo.pwType         = WATCH_COMMONWIN_TYPE_QUERY_1BTN_WITH2STR;
    pWinInfo->baseInfo.winPriority    = WIN_ONE_LEVEL;
    pWinInfo->baseInfo.softkeyText.softkeyLeft  = softkey_text.softkeyLeft;
    pWinInfo->baseInfo.softkeyText.softkeyMiddle= softkey_text.softkeyMiddle;
    pWinInfo->baseInfo.softkeyText.softkeyRight = softkey_text.softkeyRight;
    pWinInfo->selfInfo.info1BtnWith2Str.btnImage = btn_image;
    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( tip_str1, &pWinInfo->selfInfo.info1BtnWith2Str.tipStr1);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    result = CommonWin_CopyStringInfo( tip_str2, &pWinInfo->selfInfo.info1BtnWith2Str.tipStr2);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    result = CommonWin_CopyStringInfo( btn_str, &pWinInfo->selfInfo.info1BtnWith2Str.btnStr);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}

LOCAL WATCH_QUERYWIN_DATA_T *CommonWin_QueryWin_2Btn_CreateData(
                                                                                MMI_WIN_ID_T win_id,
                                                                                MMI_STRING_T *tip_str,
                                                                                MMI_STRING_T *left_btn_str,
                                                                                MMI_STRING_T *right_btn_str,
                                                                                MMI_IMAGE_ID_T left_btn_image,
                                                                                MMI_IMAGE_ID_T right_btn_image,
                                                                                WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                                                PROCESSMSG_FUNC userFunc
                                                                            )
{
    WATCH_QUERYWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_QUERYWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_QUERYWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId        = win_id;
    pWinInfo->baseInfo.pwType       = WATCH_COMMONWIN_TYPE_QUERY_2BTN;
    pWinInfo->baseInfo.winPriority  = WIN_ONE_LEVEL;
    pWinInfo->baseInfo.softkeyText.softkeyLeft  = softkey_text.softkeyLeft;
    pWinInfo->baseInfo.softkeyText.softkeyMiddle= softkey_text.softkeyMiddle;
    pWinInfo->baseInfo.softkeyText.softkeyRight = softkey_text.softkeyRight;
    pWinInfo->selfInfo.info2Btn.leftBtnImage    = left_btn_image;
    pWinInfo->selfInfo.info2Btn.rightBtnImage   = right_btn_image;

    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    result = CommonWin_CopyStringInfo( tip_str, &pWinInfo->selfInfo.info2Btn.tipStr);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    result = CommonWin_CopyStringInfo( left_btn_str, &pWinInfo->selfInfo.info2Btn.leftBtnStr);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    result = CommonWin_CopyStringInfo( right_btn_str, &pWinInfo->selfInfo.info2Btn.rightBtnStr);
    if( result == FALSE)
    {
        CommonWin_QueryWin_DestoryData(&pWinInfo);
        return PNULL;
    }

    return pWinInfo;
}

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
                                                     )
{
    WATCH_QUERYWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              btnImage    = 0;
#ifdef ADULT_WATCH_SUPPORT
    GUI_RECT_T                  btnRect = {0};
    uint16 btnWidth = 0;
    uint16 img_width=0;
    uint16 img_height=0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
#endif
    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("querywin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( btn_image != 0)
    {
        btnImage = btn_image;
    }
    else
    {
        btnImage = image_watch_querywin_1btn_bg;
    }
    pWinInfo = CommonWin_QueryWin_1Btn_CreateData( win_id, tip_str, btn_str, btnImage, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("querywin's pWinInfo is PNULL");
        return;
    }
    CommonWin_QueryWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID);
    CommonWin_CreateButtonCtrl( win_id, WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID);

    //get notewin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_QUERY_1BTN);

    //NoteWin layout
    CommonWin_QueryWin_1Btn_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1Btn.tipRect,
                            tip_str,
                            0
                           );
#ifdef ADULT_WATCH_SUPPORT
    btnRect = pWinInfo->selfInfo.info1Btn.btnRect;

    btnWidth = btnRect.right - btnRect.left;

    if((0 != btn_image) && (PNULL == btn_str))
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        SCI_ASSERT(lcd_width>0);
        SCI_ASSERT(lcd_height>0);
        GUIRES_GetImgWidthHeight(&img_width, &img_height, btn_image, win_id);
        if((btnWidth > img_width) &&(PNULL == btn_str) )
        {
           //set btn rect by img rect.
           btnRect.left = lcd_width/2 - img_width/2;
           btnRect.right= lcd_width/2 + img_width/2;
        }

        btnRect.top = (img_height>(btnRect.bottom - btnRect.top))?(btnRect.bottom - img_height): btnRect.top;
    }
    SCI_TRACE_LOW("[SFL]WatchCOM_QueryWin_1Btn_Enter: l:%d, t:%d, r:%d, b:%d",btnRect.left,btnRect.top,btnRect.right,btnRect.bottom);

    CommonWin_SetBtnStyle(
                            WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID,
                            btnRect,
                            btn_str,
                            btnImage
                         );
#else
    CommonWin_SetBtnStyle(
                            WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID,
                            pWinInfo->selfInfo.info1Btn.btnRect,
                            btn_str,
                            btnImage
                         );
#endif
}

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
                                                     )
{
    WATCH_QUERYWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              btnImage    = 0;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("querywin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( btn_image != 0)
    {
        btnImage = btn_image;
    }
    else
    {
        btnImage = image_watch_querywin_1btn_bg;
    }
    pWinInfo = CommonWin_QueryWin_1Btn_With2Str_CreateData( win_id, tip_str1,tip_str2, btn_str, btnImage, softkey_text, userFunc);
    CommonWin_QueryWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID);
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_QUERYWIN_TEXTBOX2_CTRL_ID);
    CommonWin_CreateButtonCtrl( win_id, WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID);

    //get notewin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_QUERY_1BTN_WITH2STR);

    //NoteWin layout
    CommonWin_QueryWin_1Btn_With2Str_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1BtnWith2Str.tipRect1,
                            tip_str1,
                            0
                           );
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_QUERYWIN_TEXTBOX2_CTRL_ID,
                            pWinInfo->selfInfo.info1BtnWith2Str.tipRect2,
                            tip_str2,
                            0
                           );
    CommonWin_SetBtnStyle(
                            WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID,
                            pWinInfo->selfInfo.info1BtnWith2Str.btnRect,
                            btn_str,
                            btnImage
                         );
}

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
                                                        MMI_STRING_T *tip_str,
                                                        MMI_STRING_T *left_btn_str,
                                                        MMI_STRING_T *right_btn_str,
                                                        MMI_IMAGE_ID_T left_btn_image,
                                                        MMI_IMAGE_ID_T right_btn_image,
                                                        WATCH_SOFTKEY_TEXT_ID_T softkey_text,
                                                        PROCESSMSG_FUNC userFunc
                                                     )
{
    WATCH_QUERYWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("querywin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);
    pWinInfo = CommonWin_QueryWin_2Btn_CreateData( win_id, tip_str, left_btn_str, right_btn_str, left_btn_image, right_btn_image, softkey_text, userFunc);

    CommonWin_QueryWin_CreateWin(win_id,pWinInfo);
#ifndef TOUCH_PANEL_SUPPORT
    //create softkey
    GUIWIN_CreateSoftkeyDyn(    win_id,
                                pWinInfo->baseInfo.softkeyText.softkeyLeft,
                                pWinInfo->baseInfo.softkeyText.softkeyMiddle,
                                pWinInfo->baseInfo.softkeyText.softkeyRight
                                );
#endif
    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID);
    CommonWin_CreateButtonCtrl( win_id, WATCH_COMMON_QUERYWIN_LEFTBTN_2BTN_CTRL_ID);
    CommonWin_CreateButtonCtrl( win_id, WATCH_COMMON_QUERYWIN_RIGHTBTN_2BTN_CTRL_ID);

    //get notewin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_QUERY_2BTN);

    //NoteWin layout
    CommonWin_QueryWin_2Btn_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info2Btn.tipRect,
                            tip_str,
                            0
                           );
    CommonWin_SetBtnStyle(
                            WATCH_COMMON_QUERYWIN_LEFTBTN_2BTN_CTRL_ID,
                            pWinInfo->selfInfo.info2Btn.leftBtnRect,
                            left_btn_str,
                            left_btn_image
                         );
    CommonWin_SetBtnStyle(
                            WATCH_COMMON_QUERYWIN_RIGHTBTN_2BTN_CTRL_ID,
                            pWinInfo->selfInfo.info2Btn.rightBtnRect,
                            right_btn_str,
                            right_btn_image
                         );
}

/*****************************************************************************/
//  Description: set 1btn query win tip string dynamicly.
//  Parameter: [In] win_id : commonwin ID
//             [In] tip_str : custom rect display string
//             [Out] None
//             [Return] None
//  Author:
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN WatchCOM_QueryWin_1Btn_Set_Tip_String(MMI_WIN_ID_T win_id, MMI_STRING_T *tip_str)
{
    BOOLEAN result = FALSE;
    WATCH_QUERYWIN_DATA_T   *pWinInfo   = (WATCH_QUERYWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);

    if ( PNULL == pWinInfo )
    {
        TRACE_APP_COMMON("The win_info_ptr is PNULL!");
        return FALSE;
    }
    SCI_MEMSET(pWinInfo->selfInfo.info1Btn.tipStr.wstr_ptr, 0, sizeof(wchar)*WATCH_COMMON_CUSTOM_STR_LEN);
    SCI_MEMCPY(pWinInfo->selfInfo.info1Btn.tipStr.wstr_ptr, tip_str->wstr_ptr,(sizeof(wchar)*(tip_str->wstr_len + 1)));
    pWinInfo->selfInfo.info1Btn.tipStr.wstr_len = tip_str->wstr_len;
    result = GUITEXT_SetString( WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID, tip_str->wstr_ptr, tip_str->wstr_len, TRUE );
    return result;
}

#ifdef  ADULT_WATCH_SUPPORT

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
                                                     )

{
    WATCH_QUERYWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    WATCH_SOFTKEY_TEXT_ID_T     softkey_text = {0};

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("querywin is open");
        return;
    }
    TRACE_APP_COMMON("AdultWatchCOM_QueryWin_2Btn_Enter,win_id is 0x%x",win_id);
    pWinInfo = CommonWin_QueryWin_2Btn_CreateData( win_id, tip_str, left_btn_str, right_btn_str, left_btn_image, right_btn_image, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("querywin's pWinInfo is PNULL");
        return;
    }
    CommonWin_QueryWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, ADULT_WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID);
    CommonWin_CreateButtonCtrl( win_id, ADULT_WATCH_COMMON_QUERYWIN_LEFTBTN_2BTN_CTRL_ID);
    CommonWin_CreateButtonCtrl( win_id, ADULT_WATCH_COMMON_QUERYWIN_RIGHTBTN_2BTN_CTRL_ID);

    //get notewin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_QUERY_2BTN);

    //NoteWin layout
    CommonWin_QueryWin_2Btn_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            ADULT_WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info2Btn.tipRect,
                            tip_str,
                            0
                           );
    CommonWin_SetBtnStyle(
                            ADULT_WATCH_COMMON_QUERYWIN_LEFTBTN_2BTN_CTRL_ID,
                            pWinInfo->selfInfo.info2Btn.leftBtnRect,
                            left_btn_str,
                            left_btn_image
                         );
    CommonWin_SetBtnStyle(
                            ADULT_WATCH_COMMON_QUERYWIN_RIGHTBTN_2BTN_CTRL_ID,
                            pWinInfo->selfInfo.info2Btn.rightBtnRect,
                            right_btn_str,
                            right_btn_image
                         );
}

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
                                                     )
{
    WATCH_QUERYWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              btnImage    = 0;
    WATCH_SOFTKEY_TEXT_ID_T     softkey_text = {0};

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("querywin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( btn_image != 0)
    {
        btnImage = btn_image;
    }
    else
    {
        btnImage = image_watch_querywin_1btn_bg;
    }
    pWinInfo = CommonWin_QueryWin_1Btn_CreateData( win_id, tip_str, btn_str, btnImage, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("querywin's pWinInfo is PNULL");
        return;
    }
    CommonWin_QueryWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateTextboxCtrl( win_id, WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID);
    CommonWin_CreateButtonCtrl( win_id, WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID);

    //get notewin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_QUERY_1BTN_1Str);

    //NoteWin layout
    CommonWin_QueryWin_1Btn_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetTextBoxStyle(
                            WATCH_COMMON_QUERYWIN_TEXTBOX_CTRL_ID,
                            pWinInfo->selfInfo.info1Btn.tipRect,
                            tip_str,
                            0
                           );
    CommonWin_SetBtnStyle(
                            WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID,
                            pWinInfo->selfInfo.info1Btn.btnRect,
                            btn_str,
                            btnImage
                         );
}
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
                                                     )
{
    WATCH_QUERYWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T    styleLayout = {0};
    MMI_IMAGE_ID_T              btnImage    = 0;
    WATCH_SOFTKEY_TEXT_ID_T     softkey_text = {0};
    MMI_STRING_T                *tip_str = PNULL;

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("querywin is open");
        return;
    }
    TRACE_APP_COMMON("MMI_Common_Note_EnterWin,win_id is 0x%x",win_id);

    if( btn_image != 0)
    {
        btnImage = btn_image;
    }
    else
    {
        btnImage = image_watch_querywin_1btn_bg;
    }
    pWinInfo = CommonWin_QueryWin_1Btn_CreateData( win_id, tip_str, btn_str, btnImage, softkey_text, userFunc);
    if (PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("qerywin's pWinInfo is PNULL");
        return;
    }

    CommonWin_QueryWin_CreateWin(win_id,pWinInfo);

    //Create ctrl
    CommonWin_CreateButtonCtrl( win_id, ADULT_WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID);

    //get notewin layout
    CommonWin_GetLayoutByType(&styleLayout,ADULT_WATCH_COMMONWIN_TYPE_QUERY_1BTN);

    //NoteWin layout
    CommonWin_QueryWin_1Btn_Layout(pWinInfo,styleLayout);

    //Set ctrl style
    CommonWin_SetBtnStyle(
                            ADULT_WATCH_COMMON_QUERYWIN_BTN_1BTN_CTRL_ID,
                            pWinInfo->selfInfo.info1Btn.btnRect,
                            btn_str,
                            btnImage
                         );
}

#endif
