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
#include "ctrlform_export.h"
#include "guictrl_api.h"
#include "guires.h"
#include "mmi_adapt_screen.h"
/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct
{
    MMI_IMAGE_ID_T              topImage;
    MMI_IMAGE_ID_T              bottomImage;
    MMI_HANDLE_T                prgbox_handle;
    GUI_RECT_T                  top_img_rect;
    GUI_RECT_T                  progress_rect;
    GUI_RECT_T                  bottom_img_rect;
    uint32                      totalCount;
    uint32                      curProgress;
    uint32                      topLimit;
    uint32                      bottomLimit;
}WATCH_PROGRESSWIN_INFO_T;

typedef struct
{
    WATCH_COMMONWIN_INFO_T      baseInfo;
    WATCH_PROGRESSWIN_INFO_T   selfInfo;
}WATCH_PROGRESSWIN_DATA_T;

#define MSG_PRG_SET_PROGRESS   0xdd01
#define MSG_PRG_SET_CONSTRAINT 0xdd02

/*---------------------------------------------------------------------------*/
/*                     LOCAL FUNCTION DECLARATION                            */
/*---------------------------------------------------------------------------*/
LOCAL void CommonWin_ProgressWin_DestoryData( WATCH_PROGRESSWIN_DATA_T **p_win_info );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION                                   */
/*---------------------------------------------------------------------------*/

LOCAL MMI_RESULT_E HandleMsgProgressWinWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E            recode      = MMI_RESULT_TRUE;
    WATCH_PROGRESSWIN_DATA_T   *pWinInfo   = (WATCH_PROGRESSWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info  = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    if ( PNULL == pWinInfo )
    {
        TRACE_APP_COMMON("progress create The pWinInfo is PNULL!");
        return MMI_RESULT_FALSE;
    }
    TRACE_APP_COMMON("[SFL]HandleMsgProgressWinWindow msgid:%d",msg_id );

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            if (0 == pWinInfo->selfInfo.prgbox_handle)
            {
                MMI_CONTROL_CREATE_T create = {0};
                GUIPRGBOX_INIT_DATA_T init_data = {0};
                IGUICTRL_T* ctrl_ptr = PNULL;

                if (!GUI_IsInvalidRect(pWinInfo->selfInfo.progress_rect))
                {
                    init_data.is_forbid_paint = TRUE;
                    init_data.style = GUIPRGBOX_STYLE_HORIZONTAL_PROCESS ; //GUIPRGBOX_STYLE_VERTICAL_PROCESS;
                    init_data.both_rect.v_rect = init_data.both_rect.h_rect = pWinInfo->selfInfo.progress_rect;
                    init_data.theme_type = MMITHEME_PRGBOX_TYPE_WIDEBAR_PROGRESS;
                    init_data.lcd_id = GUI_MAIN_LCD_ID;

                    create.ctrl_id = 0;
                    create.guid    = SPRD_GUI_PRGBOX_ID;
                    create.parent_win_handle = win_id;
                    create.parent_ctrl_handle = 0; 
                    create.init_data_ptr = &init_data;

                    ctrl_ptr = MMK_CreateControl(&create);
                    if(PNULL != ctrl_ptr)
                    {
                        pWinInfo->selfInfo.prgbox_handle =  GUICTRL_GetCtrlHandle((IGUICTRL_T *)ctrl_ptr);
                    }
                    else
                    {
                        TRACE_APP_COMMON("progress create fail.");
                        break;
                    }

                    if (0 != pWinInfo->selfInfo.prgbox_handle)
                    {
                        GUIPRGBOX_SetParamByCtrl(pWinInfo->selfInfo.prgbox_handle, pWinInfo->selfInfo.totalCount, 1);
                        GUIPRGBOX_SetPosByCtrl(TRUE, pWinInfo->selfInfo.curProgress, 0, pWinInfo->selfInfo.prgbox_handle);

                        MMK_SetActiveCtrl(pWinInfo->selfInfo.prgbox_handle, TRUE);
                        GUIAPICTRL_SetState (pWinInfo->selfInfo.prgbox_handle, GUICTRL_STATE_TOPMOST_1, TRUE);
                    }
                    else
                    {
                        TRACE_APP_COMMON("create progress control fail.");
                        break;
                    }
                }
                else
                {
                    TRACE_APP_COMMON("progress ctrl rect is invalid.");
                    break;
                }
            }
            else
            {
                TRACE_APP_COMMON("progress window create, progress ctrl handle is not 0.");
                break;
            }

            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }

        }
        break;

        case MSG_FULL_PAINT:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            WATCHCOM_DisplayBackground(win_id);
            //Draw uper img and btm img.
            GUIRES_DisplayImg (PNULL,
                   &pWinInfo->selfInfo.top_img_rect,
                   PNULL,
                   win_id,
                   pWinInfo->selfInfo.topImage,
                   &lcd_dev_info);
            GUIRES_DisplayImg (PNULL,
                   &pWinInfo->selfInfo.bottom_img_rect,
                   PNULL,
                   win_id,
                   pWinInfo->selfInfo.bottomImage,
                   &lcd_dev_info);
            if (0 != pWinInfo->selfInfo.prgbox_handle)
            {
                GUIPRGBOX_UpdateByCtrl(pWinInfo->selfInfo.prgbox_handle);
            }
        }
        break;

        case MSG_NOTIFY_PRGBOX_CHANGE:
        {
            uint32 current_pos = 0;
            GUIPRGBOX_NOTIFY_T notify_info = {0};
            if(PNULL == param)
            {
                TRACE_APP_COMMON("param is PNULL");
                return MMI_RESULT_FALSE;
            }
            SCI_MEMCPY(&notify_info, param, sizeof(GUIPRGBOX_NOTIFY_T));
            if(notify_info.notify.src_handle == pWinInfo->selfInfo.prgbox_handle)
            {
                current_pos = notify_info.item_index;
                TRACE_APP_COMMON("got progbox message move, cur pos:%d", current_pos);
                pWinInfo->selfInfo.curProgress = current_pos;
                if(PNULL != pWinInfo->baseInfo.userFunc)
                {
                    pWinInfo->baseInfo.userFunc( win_id, msg_id, &current_pos );
                }
            }
            else
            {
                TRACE_APP_COMMON("src_handle mismatching");
            }
        }
        break;

        case MSG_PRG_SET_PROGRESS:
        {
            if(MMK_IsFocusWin(win_id))
            {
                GUIPRGBOX_SetPosByCtrl(TRUE, pWinInfo->selfInfo.curProgress, 0, pWinInfo->selfInfo.prgbox_handle);
            }
            else
            {
                GUIPRGBOX_SetPosByCtrl(TRUE, pWinInfo->selfInfo.curProgress, 0, pWinInfo->selfInfo.prgbox_handle);
            }
        }
        break;

        case MSG_PRG_SET_CONSTRAINT:
        {
            if(MMK_IsFocusWin(win_id))
            {
                //CTRLPROGRESSBAR_SetConstraint(pWinInfo->selfInfo.prgbox_handle, pWinInfo->selfInfo.topLimit,  pWinInfo->selfInfo.bottomLimit);
            }
            else
            {
                //CTRLPROGRESSBAR_SetConstraint(pWinInfo->selfInfo.prgbox_handle, pWinInfo->selfInfo.topLimit, pWinInfo->selfInfo.bottomLimit);
            }
        }
        break;

        case MSG_CLOSE_WINDOW:
        {
            if(PNULL != pWinInfo->baseInfo.userFunc)
            {
                pWinInfo->baseInfo.userFunc( win_id, msg_id, param );
            }
            if (0 != pWinInfo->selfInfo.prgbox_handle)
            {
                MMK_DestroyControl(pWinInfo->selfInfo.prgbox_handle);
                pWinInfo->selfInfo.prgbox_handle = 0;
            }
            CommonWin_ProgressWin_DestoryData(&pWinInfo);
        }
        break;

        case MSG_APP_OK:
        case MSG_NOTIFY_OK:
        case MSG_NOTIFY_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_NOTIFY_MIDSK:
        case MSG_APP_WEB:
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

LOCAL void CommonWin_ProgressWin_Layout( WATCH_PROGRESSWIN_DATA_T *p_win_info , WATCH_COMMONWIN_LAYOUT_T layout )
{
    GUI_RECT_T dispRect = MMITHEME_GetFullScreenRect();

    if ( PNULL == p_win_info)
    {
        TRACE_APP_COMMON("The p_win_info is PNULL!");
        return;
    }

    dispRect.bottom = dispRect.right;

    SCI_MEMCPY(&p_win_info->baseInfo.bgRect, &dispRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.top_img_rect, &layout.layoutProgress.topImgRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.progress_rect, &layout.layoutProgress.progressRect,sizeof(GUI_RECT_T));
    SCI_MEMCPY(&p_win_info->selfInfo.bottom_img_rect, &layout.layoutProgress.bottomImgRect,sizeof(GUI_RECT_T));
}

LOCAL void CommonWin_ProgressWin_CreateWin(
                                MMI_WIN_ID_T win_id,
                                WATCH_PROGRESSWIN_DATA_T *p_win_info
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
    winCreate.win_id = win_id;
    winCreate.func = HandleMsgProgressWinWindow;

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

LOCAL void CommonWin_ProgressWin_DestoryData( WATCH_PROGRESSWIN_DATA_T **p_win_info )
{
    if (PNULL != *p_win_info)
    {
        SCI_FREE(*p_win_info);
        *p_win_info = PNULL;
    }
}

LOCAL WATCH_PROGRESSWIN_DATA_T *CommonWin_ProgressWin_CreateData(
                                                                                MMI_WIN_ID_T win_id,
                                                                                WATCH_COMMONWIN_TYPE_E type,
                                                                                MMI_IMAGE_ID_T top_image,
                                                                                MMI_IMAGE_ID_T bottom_image,
                                                                                uint32  totalCount,
                                                                                uint32  curProgress,
                                                                                PROCESSMSG_FUNC userFunc
                                                                            )
{
    WATCH_PROGRESSWIN_DATA_T *pWinInfo = PNULL;
    BOOLEAN result = TRUE;

    //alloc space
    pWinInfo = SCI_ALLOC_APP(sizeof(WATCH_PROGRESSWIN_DATA_T));
    if(PNULL == pWinInfo)
    {
        TRACE_APP_COMMON("The pWinInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(pWinInfo,0,sizeof(WATCH_PROGRESSWIN_DATA_T));

    //set pw info
    pWinInfo->baseInfo.winId        = win_id;
    pWinInfo->baseInfo.pwType       = type;
    pWinInfo->baseInfo.winPriority  = WIN_ONE_LEVEL;
    pWinInfo->selfInfo.topImage= top_image;
    pWinInfo->selfInfo.bottomImage= bottom_image;
    pWinInfo->selfInfo.curProgress = curProgress;
    pWinInfo->selfInfo.totalCount = totalCount;
    pWinInfo->selfInfo.prgbox_handle = 0;

    if(PNULL != userFunc)
    {
        pWinInfo->baseInfo.userFunc   = userFunc;
    }

    return pWinInfo;
}

/*****************************************************************************/
//  Description: enter progress window 
//  Parameter: [In] win_id : commonwin ID
//             [In] top_image : top display image
//             [In] bottom_image : bottom display image
//             [In] userFunc : custom callback API
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
                                                     )
{
    WATCH_PROGRESSWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T        styleLayout = {0};

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("ProgressWin is open");
        return;
    }
    TRACE_APP_COMMON("WatchCOM_ProgressWin_Enter,win_id is 0x%x",win_id);
	if(curProgress > totalCount)
	{
		curProgress = totalCount;
	}
	else if(0 > curProgress)
	{
		curProgress = 0;
	}

    pWinInfo = CommonWin_ProgressWin_CreateData( win_id, WATCH_COMMONWIN_TYPE_PROGRESS , top_image, bottom_image, totalCount, curProgress, userFunc);

    CommonWin_ProgressWin_CreateWin(win_id,pWinInfo);

    //get ProgressWin layout
    CommonWin_GetLayoutByType(&styleLayout,WATCH_COMMONWIN_TYPE_PROGRESS);

    //ProgressWin layout
    CommonWin_ProgressWin_Layout(pWinInfo,styleLayout);
}

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
                                                     )
{
	
    WATCH_PROGRESSWIN_DATA_T       *pWinInfo = PNULL;
    WATCH_COMMONWIN_LAYOUT_T        styleLayout = {0};

    if(MMK_IsOpenWin(win_id))
    {
        TRACE_APP_COMMON("ProgressWin is open");
        return;
    }
    TRACE_APP_COMMON("WatchCOM_ProgressWin_Enter_Ex,win_id is 0x%x",win_id);
	if(curProgress > totalCount)
	{
		curProgress = totalCount;
	}
	else if(0 > curProgress)
	{
		curProgress = 0;
	}

    pWinInfo = CommonWin_ProgressWin_CreateData( win_id, type , top_image, bottom_image, totalCount, curProgress, userFunc);

    CommonWin_ProgressWin_CreateWin(win_id,pWinInfo);

    //get ProgressWin layout
    CommonWin_GetLayoutByType(&styleLayout,type);

    //ProgressWin layout
    CommonWin_ProgressWin_Layout(pWinInfo,styleLayout);
}

/*****************************************************************************/
//  Description: set progress value 
//  Parameter: [In] win_id : commonwin ID
//              [In] progress : progress value to set.
//             [Out] None
//             [Return] None.
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_ProgressWin_SetProgress(MMI_WIN_ID_T win_id, uint32 progress)
{
    WATCH_PROGRESSWIN_DATA_T   *pWinInfo   = (WATCH_PROGRESSWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);

    if ( PNULL == pWinInfo )
    {
        TRACE_APP_COMMON("set progress: The pWinInfo is PNULL!");
        return;
    }
    if(progress > pWinInfo->selfInfo.totalCount)
    {
        pWinInfo->selfInfo.curProgress = pWinInfo->selfInfo.totalCount;
    }
    else if(0 > progress)
    {
        pWinInfo->selfInfo.curProgress = 0;
    }
    else
    {
        pWinInfo->selfInfo.curProgress = progress;
    }

    MMK_PostMsg(win_id, MSG_PRG_SET_PROGRESS, (DPARAM)PNULL, 0);
}




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
PUBLIC void WatchCOM_ProgressWin_SetConstraint(MMI_WIN_ID_T win_id, uint32 max, uint32 min)
{
    WATCH_PROGRESSWIN_DATA_T   *pWinInfo   = (WATCH_PROGRESSWIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);

    if ( PNULL == pWinInfo )
    {
        TRACE_APP_COMMON("set progress: The pWinInfo is PNULL!");
        return;
    }
    if(max > pWinInfo->selfInfo.totalCount)
    {
        pWinInfo->selfInfo.topLimit= pWinInfo->selfInfo.totalCount;
    }
    else if(0 > max)
    {
        pWinInfo->selfInfo.topLimit = 0;
    }
    else
    {
        pWinInfo->selfInfo.topLimit = max;
    }

    if(min > pWinInfo->selfInfo.totalCount)
    {
        pWinInfo->selfInfo.bottomLimit= pWinInfo->selfInfo.totalCount;
    }
    else if(0 > min)
    {
        pWinInfo->selfInfo.bottomLimit = 0;
    }
    else
    {
        pWinInfo->selfInfo.bottomLimit = min;
    }
    
    MMK_PostMsg(win_id, MSG_PRG_SET_CONSTRAINT, (DPARAM)PNULL, 0);

}



