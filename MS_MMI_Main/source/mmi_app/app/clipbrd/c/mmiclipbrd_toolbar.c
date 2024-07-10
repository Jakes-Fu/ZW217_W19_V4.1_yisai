/*****************************************************************************
** File Name:      mmiclipbrd_toolbar.c                                      *
** Author:         hua.fang                                                  *
** Date:           08/05/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011        hua.fang          Create                                   *
******************************************************************************/

#include "mmi_app_clipbrd_trc.h"
#include "mmiclipbrd_internal.h"
#include "mmidisplay_data.h"

#ifdef GUIF_CLIPBOARD
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "guictrl_api.h"
#include "mmiclipbrd_id.h"
#include "mmiclipbrd_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guibutton.h"
#include "mmitheme_pos.h"
#include "mmiclipbrd_image.h"
#include "guires.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMICLIPBRD_TOOLBAR_BUTT_NUM     5

#if defined(MAINLCD_SIZE_320X480)
#define MMICLIPBRD_TOOLBAR_BUTT_MARGIN  20
#else
#define MMICLIPBRD_TOOLBAR_BUTT_MARGIN  10
#endif
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
    CLIPBRDMENU_DATA_T  clipbrd_data;
    GUI_LCD_DEV_INFO    lcd_dev_info;
    MMI_HANDLE_T        button[MMICLIPBRD_TOOLBAR_BUTT_NUM];
    GUI_RECT_T          rect;
} MMICLIPBRD_TOOLCTRL_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateClipbrdLayer(
                              MMI_HANDLE_T          ctrl_handle,        // in
                              int16                 x,                  // in
                              int16                 y,                  // in
                              int16                 width,              // in
                              int16                 height,             // in
                              GUI_LCD_DEV_INFO      *lcd_dev_info_ptr   // out
                              );

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseClipbrdLayer(
                            GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                            );

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendClipbrdLayer(
                           GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                           );

/*****************************************************************************/
//  Description : clipboard toolbar msg handler
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClipboardToolbarMsg(
                                             MMI_WIN_ID_T        win_id,   // [in]
                                             MMI_MESSAGE_ID_E    msg_id,   // [in]
                                             DPARAM              param     // [in]
                                             );

/*****************************************************************************/
//  Description : send clipboard menu msg to ctrl
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SendMenuMsgToCtrl(
                             MMI_HANDLE_T dst_ctrl_handle,  // [in]
                             MMI_CTRL_ID_T button_id          // [in]
                             );

LOCAL GUI_RECT_T CalcButtonRect(
                          CLIPBRDMENU_DATA_T    *clipbrd_data_ptr,
                          GUI_POINT_T           *start_pt,
                          uint16                height,
                          GUI_RECT_T            *button_rect_ptr,
                          uint16                *button_num_ptr
                          );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// clipboard menu window
WINDOW_TABLE( MMICLIPBRD_TOOLBAR_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleClipboardToolbarMsg),
    WIN_ID(MMICLIPBRD_MENU_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : show clipboard toolbar
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T _MMICLIPBRD_ShowToolbar(                                    // [ret]  return clipboard window handle
                                        const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                        )
{
    MMI_HANDLE_T            win_handle = 0;
    MMICLIPBRD_TOOLCTRL_T   *clipboard_data_ptr = PNULL;
    //MMI_WINDOW_CREATE_T     win_create = {0};
    GUI_RECT_T              screen_rect = {0};

    if (PNULL == data_ptr)
    {
        return 0;
    }

    if (data_ptr->is_copy_disabled
        && data_ptr->is_cut_disabled
        && data_ptr->is_paste_disabled
        && data_ptr->is_select_all_disabled
        && data_ptr->is_select_disabled)
    {
        return 0;
    }

    // 先把之前的内容刷屏了，不然LOSE_FOCUS时刷的内容不对，是之前内容
    //GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
    MMITHEME_UpdateRect();

    // alloc memory to store data which will be used when show menu.
    clipboard_data_ptr = (MMICLIPBRD_TOOLCTRL_T*)SCI_ALLOC_APP(sizeof(MMICLIPBRD_TOOLCTRL_T));
    SCI_MEMSET(clipboard_data_ptr, 0, sizeof(MMICLIPBRD_TOOLCTRL_T));

    clipboard_data_ptr->clipbrd_data = *data_ptr;

    // the data is stored in window
    win_handle = MMK_CreateWin(                 // 用子窗口，如果剪贴板创建在子窗口上，会收不到TP消息
            (uint32*)MMICLIPBRD_TOOLBAR_WIN_TAB,
            (ADD_DATA)clipboard_data_ptr);
    //win_create.add_data_ptr = (ADD_DATA)clipboard_data_ptr;
    //win_create.applet_handle = MMK_GetFirstAppletHandle();
    //win_create.func = HandleClipboardToolbarMsg;
    //win_create.parent_win_handle = MMK_GetWinHandleByCtrl(data_ptr->dst_ctrl_handle);
    //win_create.window_move_style = MOVE_FORBIDDEN;
    //win_create.window_style = WS_HAS_TRANSPARENT;
    //win_create.win_id = MMICLIPBRD_MENU_WIN_ID;
    //win_handle = MMK_CreateWindow(&win_create);
    //if (0 == win_handle)
    //{
    //    SCI_FREE(clipboard_data_ptr);
    //}

    if (0 != win_handle)
    {
        screen_rect = MMITHEME_GetFullScreenRect();
        MMK_SetWinRect(win_handle, &screen_rect);
    }

    return win_handle;
}

/*****************************************************************************/
//  Description : close clipboard toolbar
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN _MMICLIPBRD_CloseToolbar(                       // [ret]
                                    MMI_HANDLE_T handle    // [in] clipboard window handle
                                    )
{
    return MMK_CloseWin(handle);
}

/*****************************************************************************/
//  Description : clipboard toolbar msg handler
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClipboardToolbarMsg(
                                             MMI_WIN_ID_T        win_id,   // [in]
                                             MMI_MESSAGE_ID_E    msg_id,   // [in]
                                             DPARAM              param     // [in]
                                             )
{
    int32                   i = 0;
    MMI_RESULT_E            result_code = MMI_RESULT_FALSE;
    MMICLIPBRD_TOOLCTRL_T   *clipbrd_toolctrl_ptr = PNULL;
    GUIBUTTON_INIT_DATA_T   init_data = {0};
    MMI_HANDLE_T            dst_ctrl_handle = 0;
    IGUICTRL_T              *ctrl_ptr = PNULL;
    BOOLEAN                 is_parent_handled = FALSE;
    uint16                  button_num = 0;
    uint16                  button_index = 0;
    uint16                  image_height = 0;
    GUI_POINT_T             tp_point = {0};
    MMI_NOTIFY_T            *notify_param_ptr = PNULL;
    GUI_RECT_T              button_rect_arr[MMICLIPBRD_TOOLBAR_BUTT_NUM] = {0};
    GUI_RECT_T              lcd_rect = {0};
    GUI_FONT_ALL_T          font_all = {0};
    GUI_BG_T                press_bg = {0};
    uint32                  src_id = 0;

    // BUTTON上的文字，必须对应
    MMI_TEXT_ID_T           text_id[MMICLIPBRD_TOOLBAR_BUTT_NUM] = {
            STXT_SELECT,
            TXT_IKB_SELECT_ALL,
            TXT_COMMON_COPY,
            TXT_COMMON_CUT,
            TXT_CLIPBRDMENU_PASTE};

    // 控件ID，必须对应
    MMI_CTRL_ID_T button_id[MMICLIPBRD_TOOLBAR_BUTT_NUM] = {
            MMICLIPBRD_SELECT_BUTTON_CTRL_ID,
            MMICLIPBRD_SELALL_BUTTON_CTRL_ID,
            MMICLIPBRD_COPY_BUTTON_CTRL_ID,
            MMICLIPBRD_CUT_BUTTON_CTRL_ID,
            MMICLIPBRD_PASTE_BUTTON_CTRL_ID
            };

    clipbrd_toolctrl_ptr = (MMICLIPBRD_TOOLCTRL_T*)MMK_GetWinAddDataPtr(win_id);
    if (PNULL == clipbrd_toolctrl_ptr)     // data which is set at creating win
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIRES_GetImgWidthHeight(
                PNULL,
                &image_height,
                IMAGE_CLIPBRD_BG,
                win_id);

        lcd_rect = CalcButtonRect(
                &clipbrd_toolctrl_ptr->clipbrd_data,
                &clipbrd_toolctrl_ptr->clipbrd_data.start_point,
                image_height,
                button_rect_arr,
                &button_num);

        clipbrd_toolctrl_ptr->rect = lcd_rect;

        init_data.bg.bg_type = GUI_BG_IMG;
        press_bg.bg_type = GUI_BG_IMG;

        font_all.color = MMI_BLACK_COLOR;
        font_all.font = MMI_DEFAULT_SMALL_FONT;

        for (i = 0; i < MMICLIPBRD_TOOLBAR_BUTT_NUM; i++)
        {
            if (button_rect_arr[i].top == button_rect_arr[i].bottom)
            {
                continue;
            }

            // BUTTON上的图片
            if (1 == button_num)        // 只有一个BUTTON的情况
            {
                init_data.bg.img_id  = IMAGE_CLIPBRD_BG;
                press_bg.img_id = IMAGE_CLIPBRD_BG_DOWN;
            }
            else if (0 == button_index)     // 左边
            {
                init_data.bg.img_id  = IMAGE_CLIPBRD_BG_LEFT;
                press_bg.img_id = IMAGE_CLIPBRD_BG_LEFT_DOWN;
            }
            else if (button_index == button_num - 1)        // 右边
            {
                init_data.bg.img_id  = IMAGE_CLIPBRD_BG_RIGHT;
                press_bg.img_id = IMAGE_CLIPBRD_BG_RIGHT_DOWN;
            }
            else                            // 中间
            {
                init_data.bg.img_id  = IMAGE_CLIPBRD_BG_MIDDLE;
                press_bg.img_id = IMAGE_CLIPBRD_BG_MIDDLE_DOWN;
            }

            button_index++;

            init_data.both_rect.v_rect = button_rect_arr[i];
            init_data.both_rect.h_rect = button_rect_arr[i];

            // 创建BUTTON
            clipbrd_toolctrl_ptr->button[i] = GUIBUTTON_CreateDynamic(win_id, button_id[i], &init_data);

            if (0 != clipbrd_toolctrl_ptr->button[i])
            {
                MMK_SetAtvCtrl(win_id, clipbrd_toolctrl_ptr->button[i]);

                if (0 == clipbrd_toolctrl_ptr->lcd_dev_info.block_id 
                    || UILAYER_NULL_HANDLE == clipbrd_toolctrl_ptr->lcd_dev_info.block_id)
                {
                    if (!CreateClipbrdLayer(
                            clipbrd_toolctrl_ptr->button[i],
                            lcd_rect.left,
                            lcd_rect.top,
                            lcd_rect.right - lcd_rect.left + 1,
                            lcd_rect.bottom - lcd_rect.top + 1,
                            &clipbrd_toolctrl_ptr->lcd_dev_info))
                    {
                        break;
                    }

                    AppendClipbrdLayer(&clipbrd_toolctrl_ptr->lcd_dev_info);
                }

                // 设置BUTTON
                GUIBUTTON_SetTextId(clipbrd_toolctrl_ptr->button[i], text_id[i]);
                GUIBUTTON_SetFont(clipbrd_toolctrl_ptr->button[i], &font_all);
                GUIAPICTRL_SetLcdDevInfo(clipbrd_toolctrl_ptr->button[i], &clipbrd_toolctrl_ptr->lcd_dev_info);
                GUIBUTTON_SetRunSheen(clipbrd_toolctrl_ptr->button[i], FALSE);
                GUIBUTTON_SetPressedBg(clipbrd_toolctrl_ptr->button[i], &press_bg);
            }
        }

        result_code = MMI_RESULT_TRUE;
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        // 点击在其他地方，关闭
        if (!GUI_PointIsInRect(tp_point, clipbrd_toolctrl_ptr->rect))
        {
            MMK_CloseWin(win_id);

            MMK_RedispatchMSGTpDown(tp_point.x, tp_point.y);
        }

        result_code = MMI_RESULT_TRUE;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:     // when select one item in menu
    {
        notify_param_ptr = (MMI_NOTIFY_T*)param;
        if (PNULL != notify_param_ptr)
        {
            dst_ctrl_handle = clipbrd_toolctrl_ptr->clipbrd_data.dst_ctrl_handle;
            src_id = notify_param_ptr->src_id;
        }

        MMK_CloseWin(win_id);

        // fix NEWMS00120416:
        // 这里先close掉剪贴板窗口，再通知主控件消息
        // 若顺序相反，则会因这里的消息在消息队列里，后close引发的消息在后，如果是select消息，
        // 还会去打开新窗口，这一系列消息，使该刷新的没刷新，而引发界面问题.
        if (PNULL != notify_param_ptr && 0 != dst_ctrl_handle)
        {
            SendMenuMsgToCtrl(dst_ctrl_handle, src_id);
        }
        result_code = MMI_RESULT_TRUE;
    }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_NOTIFY_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        result_code = MMI_RESULT_TRUE;
        break;

    // @fix cr210977
    case MSG_APP_RED:
        result_code = MMI_RESULT_FALSE;             // 默认让MMK_HandlePublicKey处理

        // 先保存一下dst_ctrl，因为MMK_CloseWin后会把clipboard_data清掉
        dst_ctrl_handle = clipbrd_toolctrl_ptr->clipbrd_data.dst_ctrl_handle;

        // 如果是editbox，则需特殊处理一下，有些应用按RED键后不是直接回IDLE
        ctrl_ptr = MMK_GetCtrlPtr(dst_ctrl_handle);
        if (PNULL != ctrl_ptr
            && SPRD_GUI_EDITBOX_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
        {
            MMK_CloseWin(win_id);

            // 发给控件，如果控件不处理，会发给应用的
            is_parent_handled = MMK_SendMsg(dst_ctrl_handle, msg_id, param);
            if (is_parent_handled)
            {
                result_code = MMI_RESULT_TRUE;
            }
        }

        break;

    case MSG_FULL_PAINT:
        AppendClipbrdLayer(&clipbrd_toolctrl_ptr->lcd_dev_info);
        break;

    case MSG_NOTIFY_UPDATE:
        {
            GUI_RECT_T  *dis_rect_ptr = (GUI_RECT_T*)param;
            UILAYER_ClearRect(&clipbrd_toolctrl_ptr->lcd_dev_info, *dis_rect_ptr);
        }
        break;

    case MSG_CLOSE_WINDOW:
        // send message to tell clipboard menu is closed
        // 改成POST消息，因为目标控件要在MSG_NOTIFY_CLIPBRD_MENU_CLOSED消息
        // 里判断窗口是否有焦点，同步的话，调用IsFocusWin有问题
        MMK_PostMsg(clipbrd_toolctrl_ptr->clipbrd_data.dst_ctrl_handle, MSG_NOTIFY_CLIPBRD_MENU_CLOSED, 0, 0);

        for (i = 0; i < MMICLIPBRD_TOOLBAR_BUTT_NUM; i++)
        {
            MMK_DestroyControl(button_id[i]);
        }

        ReleaseClipbrdLayer(&clipbrd_toolctrl_ptr->lcd_dev_info);

        SCI_FREE(clipbrd_toolctrl_ptr);

        result_code = MMI_RESULT_TRUE;
        break;

    default:
        if ((msg_id & KEY_PRESSED) == KEY_PRESSED)
        {
            BOOLEAN result = FALSE;

            MMK_CloseWin(win_id);

            result = MMK_RedispatchKeypress(msg_id, param);

            if (result)
            {
                result_code = MMI_RESULT_TRUE;
            }
            else
            {
                result_code = MMI_RESULT_FALSE;
            }
        }
        break;
    }

    return result_code;
}

LOCAL GUI_RECT_T CalcButtonRect(
                          CLIPBRDMENU_DATA_T    *clipbrd_data_ptr,
                          GUI_POINT_T           *start_pt,
                          uint16                height,
                          GUI_RECT_T            *button_rect_ptr,
                          uint16                *button_num_ptr
                          )
{
    int16           i = 0;
    uint16          str_width = 0;
    MMI_STRING_T    text_str = {0};
    BOOLEAN         is_invalid[MMICLIPBRD_TOOLBAR_BUTT_NUM] = {0};
    GUI_RECT_T      cur_rect = {0};
    GUI_RECT_T      screen_rect = {0};

    // BUTTON上的文字，必须对应
    MMI_TEXT_ID_T   text_id[MMICLIPBRD_TOOLBAR_BUTT_NUM] = {
            STXT_SELECT,
            TXT_IKB_SELECT_ALL,
            TXT_COMMON_COPY,
            TXT_COMMON_CUT,
            TXT_CLIPBRDMENU_PASTE
            };

    if (PNULL != clipbrd_data_ptr && PNULL != button_rect_ptr && PNULL != start_pt)
    {
        cur_rect.left = start_pt->x;
        cur_rect.right = start_pt->x;
        cur_rect.top = start_pt->y;
        cur_rect.bottom = cur_rect.top + height - 1;

        if (cur_rect.top < 0)
        {
            cur_rect.bottom = cur_rect.bottom - cur_rect.top;
            cur_rect.top = 0;
        }

        is_invalid[0] = clipbrd_data_ptr->is_select_disabled;
        is_invalid[1] = clipbrd_data_ptr->is_select_all_disabled;
        is_invalid[2] = clipbrd_data_ptr->is_copy_disabled;
        is_invalid[3] = clipbrd_data_ptr->is_cut_disabled;
        is_invalid[4] = clipbrd_data_ptr->is_paste_disabled;

        for (i = 0; i < MMICLIPBRD_TOOLBAR_BUTT_NUM; i++)
        {
            if (!is_invalid[i])
            {
                MMITHEME_GetLabelTextByLang(text_id[i], &text_str);

                str_width = GUI_CalculateStringPiexlNum(
                        text_str.wstr_ptr,
                        text_str.wstr_len,
                        MMI_DEFAULT_SMALL_FONT,
                        2);

                button_rect_ptr[i] = cur_rect;
                button_rect_ptr[i].left = cur_rect.right;
                button_rect_ptr[i].right = cur_rect.right + str_width + MMICLIPBRD_TOOLBAR_BUTT_MARGIN - 1;

                cur_rect.right = button_rect_ptr[i].right + 1;

                (*button_num_ptr)++;
            }
        }

        // 如果超出屏幕，将其放到屏幕内
        screen_rect = MMITHEME_GetFullScreenRect();
        if(cur_rect.right - cur_rect.left > screen_rect.right - screen_rect.left)
        {
            /*如果计算出的显示范围超过了屏幕区域，则使用n等分方案*/
            cur_rect.left = screen_rect.left;
            cur_rect.right = screen_rect.left;
            for (i = 0; i < MMICLIPBRD_TOOLBAR_BUTT_NUM; i++)
            {
                if (!is_invalid[i])
                {
                    str_width = (screen_rect.right - screen_rect.left + 1 - MMICLIPBRD_TOOLBAR_BUTT_MARGIN*(*button_num_ptr))/(*button_num_ptr);
                    
                    button_rect_ptr[i] = cur_rect;
                    button_rect_ptr[i].left = cur_rect.right;
                    button_rect_ptr[i].right = cur_rect.right + str_width + MMICLIPBRD_TOOLBAR_BUTT_MARGIN - 1;

                    cur_rect.right = button_rect_ptr[i].right + 1;
                }
            }
            cur_rect.right = screen_rect.right;
        }
        else if (cur_rect.right > screen_rect.right)
        {
            for (i = 0; i < MMICLIPBRD_TOOLBAR_BUTT_NUM; i++)
            {
                if (button_rect_ptr[i].top != button_rect_ptr[i].bottom)
                {
                    button_rect_ptr[i].left -= (cur_rect.right - screen_rect.right);
                    button_rect_ptr[i].right -= (cur_rect.right - screen_rect.right);
                }
            }

            cur_rect.left -= (cur_rect.right - screen_rect.right);
            cur_rect.right = screen_rect.right;
        }
    }

    return cur_rect;
}

/*****************************************************************************/
//  Description : send clipboard menu msg to ctrl
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SendMenuMsgToCtrl(
                             MMI_HANDLE_T dst_ctrl_handle,  // [in]
                             MMI_CTRL_ID_T button_id          // [in]
                             )
{
    MMI_MESSAGE_ID_E msg_id = 0;

    switch (button_id)
    {
    case MMICLIPBRD_CUT_BUTTON_CTRL_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_CUT;
        break;

    case MMICLIPBRD_COPY_BUTTON_CTRL_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_COPY;
        break;

    case MMICLIPBRD_PASTE_BUTTON_CTRL_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_PASTE;
        break;

    case MMICLIPBRD_SELALL_BUTTON_CTRL_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_SELECTALL;
        break;

    case MMICLIPBRD_SELECT_BUTTON_CTRL_ID:
        msg_id = MSG_NOTIFY_CLIPBRD_SELECT;
        break;

    default:
        //SCI_TRACE_LOW:"== SendMenuMsgToCtrl == msg id %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICLIPBRD_TOOLBAR_604_112_2_18_2_12_2_1,(uint8*)"d", button_id);
        break;
    }

    if (0 != msg_id)
    {
        MMK_PostMsg(dst_ctrl_handle, msg_id, 0, 0);
        MMK_PostMsg(dst_ctrl_handle, MSG_CTL_PAINT, 0, 0);
    }
}

/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateClipbrdLayer(
                              MMI_HANDLE_T          ctrl_handle,        // in
                              int16                 x,                  // in
                              int16                 y,                  // in
                              int16                 width,              // in
                              int16                 height,             // in
                              GUI_LCD_DEV_INFO      *lcd_dev_info_ptr   // out
                              )
{
    BOOLEAN             result = FALSE;
    UILAYER_RESULT_E    layer_result = UILAYER_RESULT_SUCCESS;
    UILAYER_CREATE_T    create_info = {0};

    if (PNULL != lcd_dev_info_ptr
        && (0 == lcd_dev_info_ptr->block_id || UILAYER_NULL_HANDLE == lcd_dev_info_ptr->block_id)
        && UILAYER_IsMultiLayerEnable())
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = ctrl_handle;
        create_info.offset_x = x;
        create_info.offset_y = y;
        create_info.width = width;;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_FORMAT_DOUBLE;

        layer_result = UILAYER_CreateLayer(&create_info, lcd_dev_info_ptr);

        if (UILAYER_RESULT_SUCCESS == layer_result)
        {
            UILAYER_Clear(lcd_dev_info_ptr);

            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseClipbrdLayer(
                            GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                            )
{
    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        UILAYER_RELEASELAYER(lcd_dev_info_ptr);
//        UILAYER_ReleaseLayer(lcd_dev_info_ptr);
//
//        lcd_dev_info_ptr->lcd_id = GUI_MAIN_LCD_ID;
//        lcd_dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendClipbrdLayer(
                           GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                           )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        append_layer.lcd_dev_info = *lcd_dev_info_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;

        UILAYER_AppendBltLayer(&append_layer);
    }
}
#else

/*****************************************************************************/
//  Description : show clipboard toolbar
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T _MMICLIPBRD_ShowToolbar(                                    // [ret]  return clipboard window handle
                                            const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                            )
{
    return 0;
}

/*****************************************************************************/
//  Description : close clipboard toolbar
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN _MMICLIPBRD_CloseToolbar(                       // [ret]
                                    MMI_HANDLE_T handle    // [in] clipboard window handle
                                    )
{
    return FALSE;
}

#endif



/*Edit by script, ignore 1 case. Thu Apr 26 19:00:51 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
