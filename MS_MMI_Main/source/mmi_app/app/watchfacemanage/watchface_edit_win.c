/*****************************************************************************
** File Name:    watchface_edit_win.c                                        *
** Author:        yifan.yang                                                 *
** Date:           8/25/2021                                                 *
** Copyright:    2004 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:  This file is used to describe watchface edit                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/25/2021     yifan.yang            Create                                *
******************************************************************************/
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "watch_launcher_common.h"
#include "watchface_edit_win.h"
#include "watch_launcher_main.h"
#include "watch_slidepage.h"
#include "mmicom_trace.h"
#include "guiimg.h"
#include "mmiset_export.h"
#include "watch_launcher_multe_function_dial.h"
#include "ctrlanim_export.h"
#include "watchface_config.h"
#include "graphics_draw.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct EDITCLOCKWINDATA_T
{
    GUI_RECT_T display_rect;
    BOOLEAN is_tp_pressed;
    BOOLEAN is_tp_moved;
    BOOLEAN is_tp_released;
    MMISET_CLOCK_DISPLAY_STYLE_E type;
}EDITCLOCKWINDATA_T;
typedef struct WATCHFACEEDITIDX_T
{
    //当前显示的表盘index
    uint16 s_idx;
    //当前在显示的窗口
    uint16 s_winidx;
    //窗口1显示的表盘index
    uint16 s_1_idx;
    //窗口2显示的表盘index
    uint16 s_2_idx;
    //窗口3显示的表盘index
    uint16 s_3_idx;
}WATCHFACEEDITIDX_T;
typedef enum
{
    WATCHFACE_INIT,
    WATCHFACE_UPDATE,
    WATCHFACE_RESET,
    WATCHFACE_GET,
    WATCHFACE_MAX_MSG
} WATCHFACE_MSG_ID_E;
typedef struct CLICKACTION_T
{
    //响应区域
    GUI_RECT_T rect;
    //响应函数
    BOOLEAN (*func)(void);
}CLICKACTION_T;
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define TRACE_WATCHFACE_EDIT_ENTER TRACE_WATCHFACE("enter")
#define TRACE_WATCHFACE_EDIT_EXIT TRACE_WATCHFACE("exit")
#define WATCHFACE_EDIT_ELEM_NUM (3)
#define CLOCKPANEL_RECT  DP2PX_RECT(0, 0, 240, 240)
#define DISIMG_RECT DP2PX_RECT(0, 0, 240, 240)//全屏显示
#define LEFT_BTN_RECT DP2PX_RECT(0, 108, 24, 132)
#define RIGHT_BTN_RECT DP2PX_RECT(216, 108, 240, 132)
#define EDIT_BTN_RECT DP2PX_RECT(108, 196, 132, 230)
#define WATCHFACE_MAINSCREEN_HALF_HEIGHT DP2PX_VALUE(120)
#define WATCHFACE_MAINSCREEN_HALF_WIDTH DP2PX_VALUE(120)
#define WATCHFACE_EDIT_IMG_HEIGHT (DP2PX_VALUE(240)*0.625)//根据设计要求，预览图宽高为屏幕宽高的0.625
#define WATCHFACE_EDIT_IMG_WIDTH (DP2PX_VALUE(240)*0.625)
#define WATCHFACE_EDIT_WIN1 (0)
#define WATCHFACE_EDIT_WIN2 (1)
#define WATCHFACE_EDIT_WIN3 (2)

#define LEFT_BTN_TP_RECT DP2PX_RECT(0, 72, 96, 168)
#define RIGHT_BTN_TP_RECT DP2PX_RECT(144, 72, 240, 168)
LOCAL MMI_RESULT_E HandleWatchFaceEditWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*define edit  wintab*/
WINDOW_TABLE(WATCHFACE_EDIT_WIN1TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWatchFaceEditWinMsg),
    WIN_ID(WATCHFACE_EDIT_WIN1_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_FLING_CLOSE_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCHFACE_EDIT_WIN2TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWatchFaceEditWinMsg),
    WIN_ID(WATCHFACE_EDIT_WIN2_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_FLING_CLOSE_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
WINDOW_TABLE(WATCHFACE_EDIT_WIN3TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWatchFaceEditWinMsg),
    WIN_ID(WATCHFACE_EDIT_WIN3_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_FLING_CLOSE_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
LOCAL BOOLEAN EnterNextWin(void);
LOCAL BOOLEAN EnterPreWin(void);

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/

LOCAL MMI_HANDLE_T s_winhandle = 0;
LOCAL WATCHFACEEDITIDX_T s_edit_idx = {WATCHFACE_MAX_NUM,0,WATCHFACE_MAX_NUM,WATCHFACE_MAX_NUM,WATCHFACE_MAX_NUM};
LOCAL CLICKACTION_T click_action_array[]={{LEFT_BTN_TP_RECT, EnterPreWin},{RIGHT_BTN_TP_RECT, EnterNextWin},{{0,0,0,0}, PNULL}};
/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                    *
**---------------------------------------------------------------------------*/

/****************************************************************************
//  Description : OnSwitchClick                                             *
//  Parameter:  [In]:point//触点位置                                        *
//                   action//处理函数                                       *
//              [Out]:                                                      *
//              [Return]:                                                   *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
LOCAL BOOLEAN OnSwitchClick(GUI_POINT_T point, CLICKACTION_T* action)
{
    TRACE_WATCHFACE("[EDIT]enter");
    if (PNULL == action)
    {
        TRACE_WATCHFACE("[EDIT]clickaction is null");
        return FALSE;
    }
    while(PNULL != action->func)
    {
        if(point.x>action->rect.left
            && point.x<action->rect.right
            && point.y>action->rect.top
            && point.y<action->rect.bottom)
        {
            TRACE_WATCHFACE("[EDIT]point on click");
            return action->func();
        }
        action++;
    }
    TRACE_WATCHFACE("[EDIT]exit");
    return FALSE;
}
LOCAL BOOLEAN EnterNextWin(void)
{
    TRACE_WATCHFACE("[EDIT]enter");
    MMK_SendMsg(MMK_GetFocusWinHandle(), MSG_KEYDOWN_RIGHT, NULL);
    return TRUE;
}
LOCAL BOOLEAN EnterPreWin(void)
{
    TRACE_WATCHFACE("[EDIT]enter");
    MMK_SendMsg(MMK_GetFocusWinHandle(), MSG_KEYDOWN_LEFT, NULL);
    return TRUE;
}
/****************************************************************************
//  Description : get watchface edit index                                  *
//  Parameter:  [In]:                                                       *
//              [Out]:                                                      *
//              [Return]:   edit index                                      *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
LOCAL WATCHFACEEDITIDX_T* GET_WATCHFACE_EDITIDX()
{
    return &s_edit_idx;
}

LOCAL void DrawClockImage(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T image_id, GUI_RECT_T rect)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T dis_rect = {0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    dis_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, rect);
    GUIRES_DisplayImg(PNULL,
                       &dis_rect,
                       PNULL,
                       win_id,
                       image_id,
                       &lcd_dev_info
                       );
}

LOCAL EDITCLOCKWINDATA_T* CreateInitData(void)
{
    EDITCLOCKWINDATA_T *data = NULL;
    GUI_RECT_T rect = LAUNCHER_EDITCLOCKWIN_RECT;
    data = SCI_ALLOC_APPZ(sizeof(EDITCLOCKWINDATA_T));
    if(PNULL == data)
    {
        SCI_ASSERT(0);
    }
    data->display_rect = rect;
    TRACE_WATCHFACE("[EDIT]alloc memory addr = %0X.", data);
    return data;
}

/****************************************************************************
//  Description : get decode data by image full path                        *
//  Parameter:  [In]:       p_path //image路径                              *
//                          path_len //路径长度                             *
//                          size //指针所占内存大小                         *
//              [Out]:      p_img_info //image信息                          *
//              [Return]:   result                                          *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
LOCAL BOOLEAN GetDecodeData_ByImagePath(uint16* p_path, uint16 path_len, GUIIMG_BITMAP_T* p_img_info, uint32 size)
{
    GUIIMG_SRC_T            img_src = {0};
    GUIIMG_DISPLAY_T        img_display = {0};
    GUIIMG_DEC_OUT_T        img_output = {0};
    uint16                  img_width = 0;
    uint16                  img_height = 0;
    BOOLEAN                 result = TRUE;

    TRACE_WATCHFACE("[EDIT]enter GetDecodeData_ByImagePath");

    if (PNULL == p_path || 0 == path_len)
    {
        TRACE_WATCHFACE("[EDIT]Path_ptr is null or path_len is 0!!");
        return result;
    }
    if (PNULL == p_img_info->bit_ptr)
    {
        TRACE_WATCHFACE("[EDIT]malloc for p_img_info->bit_ptr fail!!");
        return result;
    }
    // get image width && height
    if (!CTRLANIM_GetImgWidthHeight(&img_width, &img_height, p_path, path_len))
    {
        return result;
    }
    TRACE_WATCHFACE("[EDIT]img_width is %d, img_height is %d", img_width, img_height);

    if (0 == img_width || 0 == img_height)
    {
        TRACE_WATCHFACE("[EDIT]Image width or height is 0!!");
        return result;
    }

    img_src.is_file = TRUE;
    img_src.full_path_ptr = p_path;

    img_display.dest_height = img_height;
    img_display.dest_width = img_width;
    img_display.is_handle_transparent = TRUE;

    img_output.data_type = GUIIMG_DATE_TYPE_ARGB888;
    img_output.decode_data_size = size;
    img_output.decode_data_ptr = p_img_info->bit_ptr;

    if (PNULL == img_output.decode_data_ptr)
    {
        TRACE_WATCHFACE("[EDIT]malloc for img_output.decode_data_ptr fail!!");
        return result;
    }
    if(!GUIIMG_Decode(&img_src, &img_display, &img_output))
    {
        TRACE_WATCHFACE("[EDIT]Decode image fail!!");
        return result;
    }

    p_img_info->date_type = img_output.data_type;
    p_img_info->img_height = img_display.dest_height;
    p_img_info->img_width = img_display.dest_width;

    result = TRUE;
    return result;
}

/****************************************************************************
//  Description : zoom img                                                  *
//  Parameter:  [In]:  p_data_info  //缩放前的数据的指针                    *
//                     size //缩放前的数据的指针所占的内存大小              *
//              [Out]: p_zoom_data_info  //缩放后的数据的指针               *
//              [Return]:  result    //函数返回值                           *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
LOCAL BOOLEAN ZoomWatchFaceEditImg(GUIIMG_BITMAP_T*  p_data_info, uint32 size, GUIIMG_BITMAP_T* p_zoom_data_info)
{
    SCALE_IMAGE_IN_T   scale_in = {0};
    SCALE_IMAGE_OUT_T scale_out = {0};
    BOOLEAN              result = FALSE;
    uint32         scale_result = 1;

    TRACE_WATCHFACE("[EDIT]enter ZoomWatchFaceEditImg");

    if (PNULL == p_data_info)
    {
        TRACE_WATCHFACE("[EDIT]p_data_info is null");
        return result;
    }

    if (PNULL == p_data_info->bit_ptr)
    {
        TRACE_WATCHFACE("[EDIT]p_zoom_data_info->bit_ptr is null");
        return result;
    }

    if (PNULL == p_zoom_data_info)
    {
        TRACE_WATCHFACE("[EDIT]p_data_info is null");
        return result;
    }


    if (PNULL == p_zoom_data_info->bit_ptr)
    {
        TRACE_WATCHFACE("[EDIT]p_zoom_data_info->bit_ptr is null");
        return result;
    }

    scale_in.src_chn.chn0.size    = size;
    scale_in.src_chn.chn0.ptr     = p_data_info->bit_ptr;
    scale_in.src_format           = IMGREF_FORMAT_ARGB888;
    scale_in.src_size.w           = p_data_info->img_width;
    scale_in.src_size.h           = p_data_info->img_height;

    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = p_data_info->img_width;
    scale_in.src_trim_rect.h      = p_data_info->img_height;

    scale_in.scale_mode           = SCALE_MODE_NO_DISTORT;

    scale_in.target_buf.size      = p_zoom_data_info->img_height * p_zoom_data_info->img_width * sizeof(uint32);
    scale_in.target_buf.ptr       = p_zoom_data_info->bit_ptr;
    if (PNULL == scale_in.target_buf.ptr)
    {
        TRACE_WATCHFACE("[EDIT]scale_in.target_buf.ptr is null");
        return result;
    }

    scale_in.target_format        = IMGREF_FORMAT_ARGB888;
    scale_in.target_size.h        = p_zoom_data_info->img_height;
    scale_in.target_size.w        = p_zoom_data_info->img_width;

    scale_result= GRAPH_ScaleImage(&scale_in, &scale_out);

    if (SCI_SUCCESS != scale_result)
    {
        TRACE_WATCHFACE("[EDIT]GRAPH_ScaleImage failed.");
        return result;
    }

    result = TRUE;
    return result;
}

/****************************************************************************
//  Description : watchface edit action                                     *
//  Parameter:  [In]:       type //窗体style                                *
//                          wf_msg_id //watchface msg_id                    *
//                          winidx //窗体index                              *
//              [Out]:                                                      *
//              [Return]:   index                                           *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
LOCAL uint16 WatchFaceEditAction(MMISET_CLOCK_DISPLAY_STYLE_E type, WATCHFACE_MSG_ID_E wf_msg_id, uint16 winidx)
{
    WATCHFACEEDITIDX_T* p_edit_idx = PNULL;
    uint16 totalnum = 0; 
    uint16 wf_cur_idx = 0;
    TRACE_WATCHFACE("[EDIT]Enter WatchFaceEditAction.");

    p_edit_idx = GET_WATCHFACE_EDITIDX();
    if (PNULL == p_edit_idx)
    {
        TRACE_WATCHFACE("[EDIT]p_edit_idx is NULL.");
        return WATCHFACE_MAX_NUM;
    }

    totalnum = WATCHFACE_Get_Watchface_Num();
    TRACE_WATCHFACE("[EDIT]watchface totalnum is %d", totalnum);

    WATCHFACE_Get_CurWatchface_Index(&wf_cur_idx);
    TRACE_WATCHFACE("[EDIT]current watchface index is %d", wf_cur_idx);
    switch (wf_msg_id)
    {
        case WATCHFACE_INIT:
        {
            TRACE_WATCHFACE("[EDIT]WATCHFACE_INIT.");
            if (MMISET_CLOCK_STYLE_DYNAMIC_1 == type)
            {
                p_edit_idx->s_idx = wf_cur_idx;
                p_edit_idx->s_1_idx = p_edit_idx->s_idx;
                if ((totalnum - 1) == p_edit_idx->s_idx)
                {
                    p_edit_idx->s_2_idx = 0;
                }
                else
                {
                    p_edit_idx->s_2_idx = p_edit_idx->s_idx + 1;
                }
                if (0 == p_edit_idx->s_idx)
                {
                    p_edit_idx->s_3_idx = (totalnum - 1);
                }
                else
                {
                    p_edit_idx->s_3_idx = p_edit_idx->s_idx - 1;
                }
            }
            else
            {
                TRACE_WATCHFACE("[EDIT]Initialization id not required.");
            }
            break;
        }
        case WATCHFACE_UPDATE:
        {
            TRACE_WATCHFACE("[EDIT]WATCHFACE_UPDATE.");
            if (0 == p_edit_idx->s_winidx)//当前焦点窗为1
            {
                if(1 == winidx)//焦点窗由1变成2,s_idx进1，更新窗3 idx
                {
                    if ((totalnum - 1) == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_idx = 0;
                    }
                    else
                    {
                        p_edit_idx->s_idx = p_edit_idx->s_idx + 1;
                    }
                    if ((totalnum - 1) == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_3_idx = 0;
                    }
                    else
                    {
                        p_edit_idx->s_3_idx = p_edit_idx->s_idx + 1;
                    }
                }
                else if(2 == winidx)//焦点窗由1变成3,s_idx退1，更新窗2 idx
                {
                    if (0 == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_idx = (totalnum - 1);
                    }
                    else
                    {
                        p_edit_idx->s_idx = p_edit_idx->s_idx - 1;
                    }
                    if (0 == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_2_idx = (totalnum - 1);
                    }
                    else
                    {
                        p_edit_idx->s_2_idx = p_edit_idx->s_idx - 1;
                    }
                }
                else
                {
                    TRACE_WATCHFACE("[EDIT]Index doesn't update.");
                }
                p_edit_idx->s_winidx = winidx;
            }
            else if (1 == p_edit_idx->s_winidx) //当前焦点窗为2
            {
                if(2 == winidx)//焦点窗由2变成3,s_idx进1，更新窗1 idx
                {
                    if ((totalnum - 1) == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_idx = 0;
                    }
                    else
                    {
                        p_edit_idx->s_idx = p_edit_idx->s_idx + 1;
                    }
                    if ((totalnum - 1) == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_1_idx = 0;
                    }
                    else
                    {
                        p_edit_idx->s_1_idx = p_edit_idx->s_idx + 1;
                    }
                }
                else if(0 == winidx)//焦点窗由2变成1,s_idx退1，更新窗3 idx
                {
                    if (0 == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_idx = (totalnum - 1);
                    }
                    else
                    {
                        p_edit_idx->s_idx = p_edit_idx->s_idx - 1;
                    }
                    if (0 == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_3_idx = (totalnum - 1);
                    }
                    else
                    {
                        p_edit_idx->s_3_idx = p_edit_idx->s_idx - 1;
                    }
                }
                else
                {
                    TRACE_WATCHFACE("[EDIT]Index doesn't update.");
                }
                p_edit_idx->s_winidx = winidx;
            }
            else if  (2 == p_edit_idx->s_winidx) //当前焦点窗为3
            {
                if(0 == winidx)//焦点窗由3变成1,s_idx进1，更新窗2 idx
                {
                    if ((totalnum - 1) == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_idx = 0;
                    }
                    else
                    {
                        p_edit_idx->s_idx = p_edit_idx->s_idx + 1;
                    }
                    if ((totalnum - 1) == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_2_idx = 0;
                    }
                    else
                    {
                        p_edit_idx->s_2_idx = p_edit_idx->s_idx + 1;
                    }
                }
                else if(1 == winidx)//焦点窗由3变成2,s_idx退1，更新窗1 idx
                {
                    if (0 == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_idx = (totalnum - 1);
                    }
                    else
                    {
                        p_edit_idx->s_idx = p_edit_idx->s_idx - 1;
                    }
                    if (0 == p_edit_idx->s_idx)
                    {
                        p_edit_idx->s_1_idx = (totalnum - 1);
                    }
                    else
                    {
                        p_edit_idx->s_1_idx = p_edit_idx->s_idx - 1;
                    }
                }
                else
                {
                    TRACE_WATCHFACE("[EDIT]Index doesn't update.");
                }
                p_edit_idx->s_winidx = winidx;
            }
            else
            {
                TRACE_WATCHFACE("[EDIT]Update error.");
            }
            break;
        }
        case WATCHFACE_GET:
        {
            TRACE_WATCHFACE("[EDIT]WATCHFACE_GET.");
            if (WATCHFACE_MAX_NUM == p_edit_idx->s_idx)
            {
                return wf_cur_idx;
            }
            else
            {
                if (MMISET_CLOCK_STYLE_DYNAMIC_1 == type)
                {
                    return p_edit_idx->s_1_idx;
                }
                else if (MMISET_CLOCK_STYLE_DYNAMIC_2 == type)
                {
                    return p_edit_idx->s_2_idx;
                }
                else if  (MMISET_CLOCK_STYLE_DYNAMIC_3 == type)
                {
                    return p_edit_idx->s_3_idx;
                }
            }
            break;
        }
        case WATCHFACE_RESET:
        {
            TRACE_WATCHFACE("[EDIT]WATCHFACE_RESET.");
            p_edit_idx->s_1_idx = WATCHFACE_MAX_NUM;
            p_edit_idx->s_2_idx = WATCHFACE_MAX_NUM;
            p_edit_idx->s_3_idx = WATCHFACE_MAX_NUM;
            p_edit_idx->s_idx = WATCHFACE_MAX_NUM;
            p_edit_idx->s_winidx = 0;
            break;
        }
        default:
            break;
    }
    return totalnum;
}
/****************************************************************************
//  Description : watchface display image                                   *
//  Parameter:  [In]:       pInfo //附加信息                                *
//                          win_id                                          *
//              [Out]:                                                      *
//              [Return]:   result                                          *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
LOCAL BOOLEAN WatchFaceDisImg(EDITCLOCKWINDATA_T *pInfo, MMI_WIN_ID_T win_id)
{
    uint16                        idx = WATCHFACE_MAX_NUM;
    GUI_RECT_T               win_rect = {0};
    GUI_RECT_T        clockpanel_rect = CLOCKPANEL_RECT;
    GUI_RECT_T               dis_rect = DISIMG_RECT;
    GUI_RECT_T              left_rect = LEFT_BTN_RECT;
    GUI_RECT_T             right_rect = RIGHT_BTN_RECT;
    GUI_LCD_DEV_INFO     lcd_dev_info = {0};
    uint16*               p_full_path = PNULL;
    uint16              full_path_len = 0;
    char*               p_config_info = PNULL;
    GUI_POINT_T                 point = {0};
    GUIIMG_BITMAP_T         data_info = {0};
    GUIIMG_BITMAP_T    zoom_data_info = {0};
    BOOLEAN                    result = FALSE;
    BOOLEAN           get_path_result = FALSE;
    BOOLEAN    get_widthheight_result = FALSE;
    BOOLEAN     get_decodedata_result = FALSE;
    BOOLEAN               zoom_result = FALSE;
    uint32                   buf_size = 0;

    GUI_RECT_T             edit_rect  = EDIT_BTN_RECT;
    BOOLEAN                editable   = FALSE;

    if (PNULL == pInfo)
    {
        TRACE_WATCHFACE("[EDIT]pInfo is null");
        return result;
    }

    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    //fill background color
    GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
    clockpanel_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clockpanel_rect);
    GUIRES_DisplayImg(NULL,&clockpanel_rect,PNULL,win_id,res_aw_edit_clock_mask,&lcd_dev_info);

    if (WATCHFACE_MAX_NUM == WatchFaceEditAction(pInfo->type, WATCHFACE_GET, NULL))
    {
        TRACE_WATCHFACE("[EDIT]WatchFaceEditAction failed.");
        return result;
    }
    idx = WatchFaceEditAction(pInfo->type, WATCHFACE_GET, NULL);

    p_full_path = SCI_ALLOC_APPZ(sizeof(wchar)*(WATCHFACE_FILES_FULLPATH_LEN + 1));
    if (PNULL == p_full_path)
    {
        TRACE_WATCHFACE("[EDIT]Alloca for p_full_path fail!!");
        return result;
    }

    get_path_result = WATCHFACE_Get_PreImgPath_ByIdx(idx, WATCHFACE_FILES_FULLPATH_LEN + 1, p_full_path, &full_path_len);
    if (!get_path_result)
    {
        TRACE_WATCHFACE("[EDIT]GetImgFullPathByIdx failed");
        if (PNULL != p_full_path)
        {
            SCI_FREE(p_full_path);
            p_full_path = PNULL;
        }
        return result;
    }

    get_widthheight_result = CTRLANIM_GetImgWidthHeight(&data_info.img_width, &data_info.img_height, p_full_path, full_path_len);
    if (!get_widthheight_result)
    {
        TRACE_WATCHFACE("[EDIT]GetDecodeData_ByImagePath failed");
        if (PNULL != p_full_path)
        {
            SCI_FREE(p_full_path);
            p_full_path = PNULL;
        }
        return result;
    }

    buf_size = data_info.img_width * data_info.img_height * sizeof(uint32);
    data_info.bit_ptr= (void*)SCI_ALLOC_APPZ(buf_size);
    if(PNULL == data_info.bit_ptr)
    {
        TRACE_WATCHFACE("[EDIT]Alloca for data_info.bit_ptr fail!!");
        if (PNULL != p_full_path)
        {
            SCI_FREE(p_full_path);
            p_full_path = PNULL;
        }
        return result;
    }
    
    get_decodedata_result = GetDecodeData_ByImagePath(p_full_path,full_path_len,&data_info, buf_size);

    if (PNULL != p_full_path)
    {
        SCI_FREE(p_full_path);
        p_full_path = PNULL;
    }

    if (!get_decodedata_result)
    {
        TRACE_WATCHFACE("[EDIT]GetDecodeData_ByImagePath failed");
        if (PNULL != data_info.bit_ptr)
        {
            SCI_FREE(data_info.bit_ptr);
            data_info.bit_ptr = PNULL;
        }
        return result;
    }

#ifdef WIN32
    //模拟器不支持缩放使用的函数，因此将缩放功能屏蔽
    point.x = WATCHFACE_MAINSCREEN_HALF_WIDTH - data_info.img_width/2;//使图片显示在屏幕中心位置
    point.y = WATCHFACE_MAINSCREEN_HALF_HEIGHT - data_info.img_height/2;

    GUIIMG_DisplayBmp(FALSE,&dis_rect,&point,&data_info,&lcd_dev_info);
    if(PNULL != data_info.bit_ptr)
    {
        SCI_FREE(data_info.bit_ptr);
        data_info.bit_ptr = PNULL;
    }
#else
    zoom_data_info.img_height = WATCHFACE_EDIT_IMG_HEIGHT;
    zoom_data_info.img_width = WATCHFACE_EDIT_IMG_WIDTH;
    zoom_data_info.date_type = data_info.date_type;
    zoom_data_info.bit_ptr = SCI_ALLOC_APPZ(zoom_data_info.img_height * zoom_data_info.img_width * sizeof(uint32));
    if(PNULL == zoom_data_info.bit_ptr)
    {
        TRACE_WATCHFACE("[EDIT]Alloca for zoom_data_info.bit_ptr fail!!");
        return result;
    }
    zoom_result = ZoomWatchFaceEditImg(&data_info, buf_size, &zoom_data_info);

    if(PNULL != data_info.bit_ptr)
    {
        SCI_FREE(data_info.bit_ptr);
        data_info.bit_ptr = PNULL;
    }

    if (!zoom_result)
    {
        TRACE_WATCHFACE("[EDIT]ZoomWatchFaceEditImg failed");
        if (PNULL != zoom_data_info.bit_ptr)
        {
            SCI_FREE(zoom_data_info.bit_ptr);
            zoom_data_info.bit_ptr = PNULL;
        }
        return result;
    }

    point.x = WATCHFACE_MAINSCREEN_HALF_WIDTH - zoom_data_info.img_width/2;//使图片显示在屏幕中心位置
    point.y = WATCHFACE_MAINSCREEN_HALF_HEIGHT - zoom_data_info.img_height/2;

    GUIIMG_DisplayBmp(FALSE,&dis_rect,&point,&zoom_data_info,&lcd_dev_info);
    if(PNULL != zoom_data_info.bit_ptr)
    {
        SCI_FREE(zoom_data_info.bit_ptr);
        zoom_data_info.bit_ptr = PNULL;
    }
#endif

    left_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, left_rect);
    right_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, right_rect);
    edit_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, edit_rect);

    GUIRES_DisplayImg(PNULL, &left_rect, PNULL, win_id, res_aw_edit_ic_left, &lcd_dev_info);
    GUIRES_DisplayImg(PNULL, &right_rect, PNULL, win_id, res_aw_edit_ic_right, &lcd_dev_info);
#ifdef WATCHFACE_CTRL_EDIT_SUPPORT
    WATCHFACE_GetEditable_ByIdx(idx,&editable);
    if(TRUE == editable)
    {
        GUIRES_DisplayImg(PNULL, &edit_rect, PNULL, win_id, res_aw_edit_ic_edit, &lcd_dev_info);
    }
    else
    {
        TRACE_WATCHFACE("[EDIT]not editable");
    }
#endif
    result = TRUE;
    return result;
}

LOCAL MMI_RESULT_E HandleWatchFaceEditWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    EDITCLOCKWINDATA_T *pInfo = MMK_GetWinAddDataPtr(win_id);
    TRACE_WATCHFACE("[EDIT]add_data addr = %0X.", (uint32)pInfo);  //bug id 1941703 CID 426196
    TRACE_WATCHFACE("[EDIT]win_id = %0X. msg_id = %0X", win_id, msg_id);


    if (PNULL == pInfo)
    {
        TRACE_WATCHFACE("[EDIT]PARAM ERROR");
        recode = MMI_RESULT_FALSE;
        return recode;
    }

    MMI_CheckAllocatedMemInfo();

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_WATCHFACE("[EDIT]MSG_OPEN_WINDOW");
            WatchFaceEditAction(pInfo->type, WATCHFACE_INIT, NULL);
            break;
        }

        case MSG_KEYDOWN_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            WatchFaceEditAction(pInfo->type, WATCHFACE_RESET, NULL);
            TRACE_WATCHFACE("[EDIT]MSG_CLOSE_WINDOW");
            WatchSLIDEPAGE_DestoryHandle(s_winhandle);

            if (PNULL != pInfo)
            {
                TRACE_WATCHFACE("[EDIT]free add_data. addr = %0X.", (uint32)pInfo);// bug id 1941703 CID 426196
                MMK_FreeWinAddData(win_id);
            }
            break;
        }

        case MSG_FULL_PAINT:
        {

            TRACE_WATCHFACE("[EDIT]MSG_FULL_PAINT");
            WatchFaceDisImg(pInfo, win_id);
            break;
        }

        case MSG_APP_WEB:
        {
            TRACE_WATCHFACE("[EDIT]MSG_APP_WEB");
            TRACE_WATCHFACE("[EDIT]edit selected clock type = %d.", pInfo->type);

            MMIAPISET_SetClockDisplayStyle(pInfo->type);
            MMK_CloseWin(win_id);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            TRACE_WATCHFACE("[EDIT]MSG_TP_PRESS_DOWN");
            pInfo->is_tp_pressed = TRUE;
            pInfo->is_tp_moved = FALSE;
            pInfo->is_tp_released = FALSE;
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            TRACE_WATCHFACE("[EDIT]MSG_TP_PRESS_MOVE");
            pInfo->is_tp_moved = TRUE;
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            TRACE_WATCHFACE("[EDIT]MSG_TP_PRESS_UP");
            TRACE_WATCHFACE("[EDIT]Edit selected clock type = %d.", pInfo->type);
            if (pInfo->is_tp_pressed/* && !pInfo->is_tp_moved*/)
            {
                uint16       idx = WATCHFACE_MAX_NUM;
                BOOLEAN editable = FALSE;
                GUI_POINT_T  point = {0};
                GUI_RECT_T   edit_rect = EDIT_BTN_RECT;
                BOOLEAN      is_click = FALSE;
                idx = WatchFaceEditAction(pInfo->type, WATCHFACE_GET, NULL);
                if (WATCHFACE_MAX_NUM != idx)
                {
                    point.x = MMK_GET_TP_X(param);
                    point.y = MMK_GET_TP_Y(param);

                    is_click = OnSwitchClick(point, click_action_array);
                    //idx = WatchFaceEditAction(pInfo->type, WATCHFACE_GET, NULL);
                    if (TRUE == is_click)
                    {
                        return MMI_RESULT_TRUE;
                    }
#ifdef WATCHFACE_CTRL_EDIT_SUPPORT
                    WATCHFACE_GetEditable_ByIdx(idx,&editable);
                    if(TRUE == editable)
                    {


                        if( point.x > edit_rect.left
                         && point.x < edit_rect.right
                         && point.y > edit_rect.top
                         && point.y < edit_rect.bottom )
                        {
                            WATCHFACE_MTFDial_Win_Enter(idx);
                        }
                        else
                        {
                            TRACE_WATCHFACE("[EDIT]not edit rect.");
                            WATCHFACE_Set_CurWatchface_Index(idx);
                        }
                    }
                    else
#endif
                    {
                        WATCHFACE_Set_CurWatchface_Index(idx);
                    }
                }
                WatchFaceEditAction(pInfo->type, WATCHFACE_RESET, NULL);
                MMK_CloseWin(win_id);
            }
            break;
        }
#endif
        default:
        {
            recode = MMI_RESULT_FALSE;
            break;
        }
    }
    MMI_CheckAllocatedMemInfo();
    TRACE_WATCHFACE_EDIT_EXIT;
    return recode;
}

LOCAL void WATCHFACE_Edit_Win1_Enter(void)
{
    EDITCLOCKWINDATA_T* data = CreateInitData();
    MMI_WIN_ID_T win_id = WATCHFACE_EDIT_WIN1_ID;
    if(PNULL == data)
    {
        TRACE_WATCHFACE("[EDIT]Alloca for data fail!!");
        return;
    }
    TRACE_WATCHFACE_EDIT_ENTER;
    data->type = MMISET_CLOCK_STYLE_DYNAMIC_1;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMK_CreateWin((uint32 *)WATCHFACE_EDIT_WIN1TAB, data);
    TRACE_WATCHFACE_EDIT_EXIT;
}

LOCAL void WATCHFACE_Edit_Win2_Enter(void)
{
    EDITCLOCKWINDATA_T* data = CreateInitData();
    MMI_WIN_ID_T win_id = WATCHFACE_EDIT_WIN2_ID;
    if(PNULL == data)
    {
        TRACE_WATCHFACE("[EDIT]Alloca for data fail!!");
        return;
    }
    TRACE_WATCHFACE_EDIT_ENTER;
    data->type = MMISET_CLOCK_STYLE_DYNAMIC_2;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    MMK_CreateWin((uint32 *)WATCHFACE_EDIT_WIN2TAB, data);
    TRACE_WATCHFACE_EDIT_EXIT;
}
LOCAL void WATCHFACE_Edit_Win3_Enter(void)
{
    EDITCLOCKWINDATA_T* data = CreateInitData();
    MMI_WIN_ID_T win_id = WATCHFACE_EDIT_WIN3_ID;
    if(PNULL == data)
    {
        TRACE_WATCHFACE("[EDIT]Alloca for data fail!!");
        return;
    }
    TRACE_WATCHFACE_EDIT_ENTER;
    data->type = MMISET_CLOCK_STYLE_DYNAMIC_3;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    MMK_CreateWin((uint32 *)WATCHFACE_EDIT_WIN3TAB, data);
    TRACE_WATCHFACE_EDIT_EXIT;
}



LOCAL MMI_RESULT_E WATCHFACE_Edit_HandleCb(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_WATCHFACE("[EDIT]win_id = %0X. msg_id = %0X", win_id, msg_id);
    switch (msg_id)
    {
        case MSG_SLIDEPAGE_END:
        {
            tWatchSlidePage* p_sp_msg = PNULL;
            p_sp_msg = WatchSLIDEPAGE_GetSlideInfoByHandle(s_winhandle);
            if(PNULL == p_sp_msg)
            {
                TRACE_WATCHFACE("[EDIT]p_sp_msg id PNULL.");
                return MMI_RESULT_FALSE;
            }
            WatchFaceEditAction(NULL, WATCHFACE_UPDATE, p_sp_msg->cur_page_index);
            break;
        }
        default:
        {
            recode = MMI_RESULT_FALSE;
            break;
        }
    }
    return recode;
}

/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/
/****************************************************************************
//  Description : open watchface eidt window                                *
//  Patameter:  [In]:       None                                            *
                [Out]:      None                                            *
                [Return]:   None                                            *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
PUBLIC void WATCHFACE_Edit_Slidepage_Enter(void)
{
    tWatchSlidePageItem elem[LAUNCHER_EDITCLOCKWIN_NUM] = {0};
    uint8 elem_num = WATCHFACE_EDIT_ELEM_NUM;
    TRACE_WATCHFACE_EDIT_ENTER;
    s_winhandle = WatchSLIDEPAGE_CreateHandle();

    elem[0].fun_enter_win = WATCHFACE_Edit_Win1_Enter;
    elem[0].win_id = WATCHFACE_EDIT_WIN1_ID;
    elem[1].fun_enter_win = WATCHFACE_Edit_Win2_Enter;
    elem[1].win_id = WATCHFACE_EDIT_WIN2_ID;
    elem[2].fun_enter_win = WATCHFACE_Edit_Win3_Enter;
    elem[2].win_id = WATCHFACE_EDIT_WIN3_ID;
    WatchSLIDEPAGE_Open(s_winhandle, elem, elem_num, 0, TRUE, WATCHFACE_Edit_HandleCb);
    TRACE_WATCHFACE_EDIT_EXIT;
}

/****************************************************************************
//  Description : update img                                                *
//  Patameter:  [In]:       None                                            *
                [Out]:      None                                            *
                [Return]:   None                                            *
//  Author: yifan.yang                                                      *
//  Note:   update img when add or delete watchface                         *
*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Update_PreviewImg(void)
{
    WATCHFACEEDITIDX_T* p_update_idx = PNULL;
    uint16              totalnum     = 0;
    BOOLEAN             result       = FALSE;
    TRACE_WATCHFACE("[EDIT]Enter WATCHFACE_Update_Img.");

    p_update_idx = GET_WATCHFACE_EDITIDX();
    if (PNULL == p_update_idx)
    {
        TRACE_WATCHFACE("[EDIT]p_edit_idx is NULL.");
        return result;
    }

    totalnum = WATCHFACE_Get_Watchface_Num();
    TRACE_WATCHFACE("[EDIT]total number is %d", totalnum);
    if (0 == totalnum)
    {
        return result;
    }

    p_update_idx->s_idx = 0;
    TRACE_WATCHFACE("focus win is %d", p_update_idx->s_idx + 1);
    switch (p_update_idx->s_winidx)
    {
        case WATCHFACE_EDIT_WIN1:
        {
            p_update_idx->s_1_idx = p_update_idx->s_idx;
            p_update_idx->s_2_idx = p_update_idx->s_idx + 1;
            p_update_idx->s_3_idx = totalnum - 1;
            result = TRUE;
            break;
        }
        case WATCHFACE_EDIT_WIN2:
        {
            p_update_idx->s_2_idx = p_update_idx->s_idx;
            p_update_idx->s_3_idx = p_update_idx->s_idx + 1;
            p_update_idx->s_1_idx = totalnum - 1;
            result = TRUE;
            break;
        }
        case WATCHFACE_EDIT_WIN3:
        {
            p_update_idx->s_3_idx = p_update_idx->s_idx;
            p_update_idx->s_1_idx = p_update_idx->s_idx + 1;
            p_update_idx->s_2_idx = totalnum - 1;
            result = TRUE;
            break;
        }
        default:
        {
            TRACE_WATCHFACE("p_update_idx->s_idx is error");
            break;
        }
    }
    if (TRUE == result)
    {
        MMK_SendMsg(WATCHFACE_EDIT_WIN1_ID, MSG_FULL_PAINT, NULL);
        MMK_SendMsg(WATCHFACE_EDIT_WIN2_ID, MSG_FULL_PAINT, NULL);
        MMK_SendMsg(WATCHFACE_EDIT_WIN3_ID, MSG_FULL_PAINT, NULL);
    }
    return result;
}

