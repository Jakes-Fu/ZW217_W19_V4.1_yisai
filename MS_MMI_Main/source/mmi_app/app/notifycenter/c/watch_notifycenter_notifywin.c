/*****************************************************************************
** File Name:      watch_notifycenter_notifywin.c                                               *
** Author:                                                                   *
** Date:           1/15/2021                                               *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2021        dandan.cao       Create                                   *
******************************************************************************/
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
//#include "watch_notifycenter.h"
#include "watch_notifycenter_id.h"
#include "ctrllist_export.h"
#include "guilistbox.h"
#include "mmitheme_button.h"
#include "guibutton.h"
#include "watch_notifycenter_text.h"
#include "watch_notifycenter_image.h"
#include "watch_notifycenter_position.h"
#include "mmi_font.h"
#include "mmi_image.h"
#include "window_parse.h"
#include "mmidisplay_color.h"
#include "mmi_custom_define.h"
#include "guilabel.h"
#include "watch_slidewin.h"
#include "watch_notifycenter.h"
#include "guitext.h"
#include "watch_notifycenter.h"
#include "watch_common_list.h"//WARN746
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define NOTIFY_LIST_TITLE_TEXT_COLOR GUI_RGB2RGB565(0x21, 0x96, 0xf3)
#define NOTIFY_LIST_SOFTKEY_TEXT_COLOR GUI_RGB2RGB565(0xFF, 0xFF, 0xFF)
/*****************************************************************************/
//  Description : Main handle of notifycenter win
//  Global resource dependence : none
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyListWin_HandleWinMsg(
                        MMI_WIN_ID_T        win_id,
                        MMI_MESSAGE_ID_E    msg_id,
                        DPARAM              param
                        );

/*****************************************************************************/
//  Description : Main handle of notifydetail win
//  Global resource dependence : none
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyDetailWin_HandleWinMsg(
                        MMI_WIN_ID_T        win_id,
                        MMI_MESSAGE_ID_E    msg_id,
                        DPARAM              param
                        );
/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
WINDOW_TABLE( NOTIFYLIST_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(NOTIFYLIST_WIN_ID),
    WIN_FUNC( (uint32)NotifyListWin_HandleWinMsg),
    END_WIN
};
WINDOW_TABLE( NOTIFYDETAIL_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(NOTIFYDETAIL_WIN_ID),
    WIN_FUNC( (uint32)NotifyDetailWin_HandleWinMsg),
    END_WIN
};
/*****************************************************************************/
//  Description : displaylistrect
//  Global resource dependence :
//  Author: dandan.cao
//  Note:
/*****************************************************************************/

LOCAL void NotifyListWin_SetCtrlParam(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T              rect_ptr = {0};
    MMI_CONTROL_CREATE_T	create = {0};
    GUILIST_INIT_DATA_T	    listinitData = {0};

    //create list
    create.ctrl_id              = NOTIFY_LIST_CTRL_ID;
    create.guid                 = SPRD_GUI_LIST_ID;
    create.parent_win_handle    = NOTIFYLIST_WIN_ID;
    create.init_data_ptr        = &listinitData;
    MMK_CreateControl( &create );
    MMK_SetAtvCtrl(win_id, NOTIFY_LIST_CTRL_ID);
    rect_ptr.bottom = NOTIFYLISTWIN_TITLE_HEIGHT + NOTIFYLISTWIN_LIST_HEIGHT*2 ;
    rect_ptr.top = NOTIFYLISTWIN_TITLE_HEIGHT;
    rect_ptr.left = NOTIFYLISTWIN_LEFT_MARGIN;
    rect_ptr.right = NOTIFYLISTWIN_MAINSCREEN_HEIGHT;
    GUILIST_SetListState( NOTIFY_LIST_CTRL_ID, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
    GUILIST_SetListState( NOTIFY_LIST_CTRL_ID, GUILIST_STATE_TEXTSCROLL_ENABLE, FALSE);
    GUILIST_SetNeedPrgbar(NOTIFY_LIST_CTRL_ID,FALSE);
    GUILIST_SetListState( NOTIFY_LIST_CTRL_ID, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE ); //bug-1620319
    CTRLLIST_SetRect(NOTIFY_LIST_CTRL_ID,&rect_ptr);
}

LOCAL void NotifyListWin_DisplayBackground(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          rect = NOTIFYLISTWIN_BG_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0,0};
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    LCD_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);
}
/*****************************************************************************/
//  Description : displaylist
//  Global resource dependence :
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
LOCAL void NotifyListWin_DisplayList(MMI_WIN_ID_T win_id)
{
    uint8           i    = 0;
    uint32          total_num = 0;
    NOTIFY_HANDLE_T notify_handle = 0;
    NOTIFY_NODE_T * p_notify_node = PNULL;
    MMI_CTRL_ID_T   ctrl_id = NOTIFY_LIST_CTRL_ID;
    MMI_STRING_T    notify_title_str = {0};
    MMI_STRING_T    notify_short_str = {0};
    MMI_STRING_T    title_item_str = {0};
    MMI_STRING_T    softkey_item_str = {0};
    GUI_RECT_T title_rect = NOTIFYLISTWIN_TITLE_RECT;
    GUI_RECT_T label_rect = NOTIFYLISTWIN_LABEL_RECT;
    GUISTR_STYLE_T      title_text_style      = {0};
    GUISTR_STYLE_T      soft_text_style      = {0};
    GUISTR_STATE_T      text_state      = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0,0};

    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);

    //display title
    title_text_style.align = ALIGN_HVMIDDLE;
    title_text_style.font  = DP_FONT_16;
    title_text_style.font_color = NOTIFY_LIST_TITLE_TEXT_COLOR;
    MMI_GetLabelTextByLang(TXT_NOTIFYLIST_TITLE,&title_item_str);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *) &title_rect,       //the fixed display area
        (const GUI_RECT_T      *) &title_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&title_item_str,
        &title_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
         );

    //display soft key
    soft_text_style.align = ALIGN_HVMIDDLE;
    soft_text_style.font  = DP_FONT_20;
    soft_text_style.font_color = GUI_RGB2RGB565(0x21, 0x96, 0xf3);
    MMI_GetLabelTextByLang(TXT_NOTIFYLIST_SOFTKEY,&softkey_item_str);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *) &label_rect,       //the fixed display area
        (const GUI_RECT_T      *) &label_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&softkey_item_str,
        &soft_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
         );

    total_num = NotifyCenter_GetTotalNum();
    GUILIST_SetMaxItem(ctrl_id, total_num, FALSE);
    GUILIST_RemoveAllItems(ctrl_id);

    for(i = 0; i < total_num; i++)
    {
        //get notify node data
        notify_handle = NotifyCenter_GetHandleByPos(i);
        p_notify_node = NotifyCenter_GetNode(notify_handle);
        if(PNULL != p_notify_node)
        {
            notify_title_str.wstr_ptr = p_notify_node->notify_info.title_wstr;
            notify_title_str.wstr_len = MMIAPICOM_Wstrlen(notify_title_str.wstr_ptr);
            notify_short_str.wstr_ptr = p_notify_node->notify_info.short_desc_wstr;
            notify_short_str.wstr_len = MMIAPICOM_Wstrlen(notify_short_str.wstr_ptr);
            //append item to list
            AdultWatchCOM_ListItem_Draw_1Icon_2Str_2line(ctrl_id,p_notify_node->notify_info.notify_image,notify_title_str,notify_short_str);
        }
        else
        {
            SCI_TRACE_LOW("NotifyListWin_DisplayList is right");
            break;
        }
    }
}
LOCAL void NotifyListWin_DisplayNoMsg(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          image_rect  = NOTIFYLISTWIN_IMAGE_RECT;
    GUI_RECT_T          label_image_softkey_rect = NOTIFYLISTWIN_IMAGE_SOFTKEY_REC;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {0,0};
    MMI_STRING_T        item_str = {0};
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      text_state      = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE; 
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    GUIRES_DisplayImg(PNULL, &image_rect, PNULL, win_id, res_aw_info_nomassage,&lcd_dev_info);

    //draw text
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font  = DP2PX_FONT(SONG_FONT_20);
    text_style.font_color = NOTIFY_LIST_SOFTKEY_TEXT_COLOR;   
    MMI_GetLabelTextByLang(TXT_NOTIFYLIST_IMAGE_SOFTKEY,&item_str);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *) &label_image_softkey_rect,       //the fixed display area
        (const GUI_RECT_T      *) &label_image_softkey_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&item_str,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
         );
}

LOCAL void NotifyDetailWin_SetCtrlParam(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T              disply_rect     = NOTIFY_APPNAME_AND_TIME_RECT;
    GUI_RECT_T              text_rect       = {0};
    GUILABEL_INIT_DATA_T	labelinitData   = {0};
    MMI_CONTROL_CREATE_T	create          = {0};
    GUITEXT_INIT_DATA_T	    textinitData    = {0};

    //create label
    labelinitData.align         = GUILABEL_ALIGN_MIDDLE;
    create.ctrl_id              = NOTIFY_DETAIL_LABEL_CTRL_ID;
    create.guid                 = SPRD_GUI_LABEL_ID;
    create.parent_win_handle    = win_id;
    create.init_data_ptr        = &labelinitData;
    MMK_CreateControl( &create );
    GUILABEL_SetRect(NOTIFY_DETAIL_LABEL_CTRL_ID,&disply_rect,FALSE);
    //create textbox
    create.ctrl_id              = NOTIFY_TEXTBOX_CTRL_ID;
    create.guid                 = SPRD_GUI_TEXTBOX_ID;
    create.parent_win_handle    = win_id;
    create.init_data_ptr        = &textinitData;
    MMK_CreateControl( &create );
    text_rect.left = NOTIFY_TEXT_LEFT_MARGIN;
    text_rect.top = NOTIFY_STATUSBAR_HEIGHT + NOTIFY_TEXT_LEFT_MARGIN + NOTIFY_NUMBER_HEIGHT + NOTIFY_TEXT_MARGIN;
    text_rect.right = NOTIFYLISTWIN_MAINSCREEN_HEIGHT - NOTIFY_TEXT_LEFT_MARGIN;
    text_rect.bottom = NOTIFYLISTWIN_MAINSCREEN_HEIGHT - 1;
    GUITEXT_SetRect(NOTIFY_TEXTBOX_CTRL_ID,&text_rect);
    GUITEXT_SetMargin(NOTIFY_TEXTBOX_CTRL_ID,2);
    GUITEXT_SetAlign(NOTIFY_TEXTBOX_CTRL_ID,ALIGN_HMIDDLE);
    GUITEXT_SetClipboardEnabled (NOTIFY_TEXTBOX_CTRL_ID,FALSE);
}

LOCAL void NotifyDetailWin_DisplayDetail(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T              detail_image_rect   = NOTIFYDETAILWIN_IMAGE_RECT;
    GUI_LCD_DEV_INFO        lcd_dev_info        = {0,0};
    MMI_IMAGE_ID_T          notify_image        = NULL;
    NOTIFY_NODE_T *         p_notify_node       = PNULL;
    NOTIFY_HANDLE_T         notify_handle       = 0;
    MMI_STRING_T            detail_str          = {0};
    MMI_STRING_T            n_string    = {0};
    MMI_STRING_T            t_string    = {0};
    wchar                   separative  = ' ';
    MMI_STRING_T            temp_str    = {0};
    MMI_STRING_T            temp1_str   = {0};
    MMI_STRING_T            display_str = {0};
    MMI_STRING_T            notify_name_str = {0};
    MMI_STRING_T            notify_time_str = {0};
    GUI_BG_T                txtbox_bg       = {0};
    GUI_COLOR_T             text_font_color = GUI_RGB2RGB565(0xFF, 0xFF, 0xFF);
    GUI_FONT_T              text_font   = DP_FONT_20;

    //load data
    notify_handle = (NOTIFY_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
    p_notify_node = NotifyCenter_GetNode(notify_handle);
    if(PNULL!= p_notify_node)
    {
        notify_image = p_notify_node->notify_info.notify_image;
        notify_name_str.wstr_ptr = p_notify_node->notify_info.app_name_wstr;
        notify_name_str.wstr_len = MMIAPICOM_Wstrlen(notify_name_str.wstr_ptr);
        notify_time_str.wstr_ptr = p_notify_node->notify_info.time_wstr;
        notify_time_str.wstr_len = MMIAPICOM_Wstrlen(notify_time_str.wstr_ptr);
    }
    else
    {
        SCI_TRACE_LOW("dandan NotifyDetailWin_DisplayDetail export data failed");//lint-1668377
        return;
    }
    //display app image
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    GUIRES_DisplayImg(PNULL, &detail_image_rect, PNULL, win_id, notify_image,&lcd_dev_info);

    //display appname and time
    temp_str.wstr_ptr = &separative;
    temp_str.wstr_len = 1;

    MMIAPICOM_CatTwoString(&temp1_str, &notify_name_str, &temp_str);
    MMIAPICOM_CatTwoString(&display_str, &temp1_str, &notify_time_str);
    MMIAPICOM_DestroyString(temp1_str);
    GUILABEL_SetFont(NOTIFY_DETAIL_LABEL_CTRL_ID,DP_FONT_16,GUI_RGB2RGB565(0x21, 0x96, 0xf3));
    GUILABEL_SetText(NOTIFY_DETAIL_LABEL_CTRL_ID,&display_str,FALSE);
    MMIAPICOM_DestroyString(display_str);

    //display detail message
    txtbox_bg.bg_type = GUI_BG_COLOR;
    txtbox_bg.color = MMI_BLACK_COLOR;
    detail_str.wstr_ptr = p_notify_node->notify_info.detail_desc_wstr;
    detail_str.wstr_len = MMIAPICOM_Wstrlen(detail_str.wstr_ptr);
    GUITEXT_SetBg(NOTIFY_TEXTBOX_CTRL_ID,&txtbox_bg);
    GUITEXT_SetString(NOTIFY_TEXTBOX_CTRL_ID,detail_str.wstr_ptr,detail_str.wstr_len,FALSE);
    GUITEXT_SetFont(NOTIFY_TEXTBOX_CTRL_ID,&text_font,&text_font_color);
}

LOCAL void  HandleNotifyWinTpPressDown(MMI_WIN_ID_T        win_id,        //IN:
										  DPARAM            param        //IN:
										  )
{
    GUI_POINT_T      point = {0};
	GUI_RECT_T       label_rect = NOTIFYLISTWIN_LABEL_RECT;
    point.x=MMK_GET_TP_X(param);
    point.y=MMK_GET_TP_Y(param);
    if(GUI_PointIsInRect(point, label_rect))
	{
        NotifyCenter_DeleteAll();
        MMK_CloseWin(win_id);
	}
}
LOCAL void NotifyListWin_DisplayListWin(MMI_WIN_ID_T win_id,uint32 total_num)
{
	if(0 == total_num)
	{
        if(PNULL!=MMK_GetCtrlPtr(NOTIFY_LIST_CTRL_ID))
        {
            MMK_DestroyDynaCtrl(NOTIFY_LIST_CTRL_ID);
        }
        NotifyListWin_DisplayNoMsg(win_id);
    }
    else
    {
        if(PNULL==MMK_GetCtrlPtr(NOTIFY_LIST_CTRL_ID))
        {
            GUI_LCD_DEV_INFO parent_lcd = {0};
            MMK_GetWinLcdDevInfo(win_id, &parent_lcd);
            NotifyListWin_SetCtrlParam(win_id);
            MMK_SetCtrlLcdDevInfo(NOTIFY_LIST_CTRL_ID, &parent_lcd);
        }
        NotifyListWin_DisplayList(win_id);
    }
}
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : notifylist win
//  Global resource dependence :
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyListWin_HandleWinMsg(
                        MMI_WIN_ID_T        win_id,
                        MMI_MESSAGE_ID_E    msg_id,
                        DPARAM              param
                        )
{
    MMI_RESULT_E        recode      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id     = NOTIFY_LIST_CTRL_ID;
    uint16              index       = 0;
    uint32              total_num   = 0;
    NOTIFY_HANDLE_T     notify_handle   = 0;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            total_num = NotifyCenter_GetTotalNum();
            if(0 != total_num)
            {
                 NotifyListWin_SetCtrlParam(win_id);
            }
        }
        break;
        case MSG_FULL_PAINT:
        {
            total_num = NotifyCenter_GetTotalNum();
            //draw bg
            NotifyListWin_DisplayBackground(win_id);
            //draw list
            NotifyListWin_DisplayListWin(win_id,total_num);
        }
        break;
        case MSG_KEYDOWN_USER_FUNC://bug-1630319
        {
            MMK_SendMsg(win_id, MSG_APP_DOWN, PNULL);
            break;
        }
        case MSG_CTL_PENOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                    case NOTIFY_LIST_CTRL_ID:
                    {
                        index = GUILIST_GetCurItemIndex(ctrl_id);
                        notify_handle = NotifyCenter_GetHandleByPos(index);
                        NotifyCenter_EnterDetailWin(notify_handle);
                    }
                    break;
                    default:
                    { 
                        SCI_TRACE_LOW("NT:((MMI_NOTIFY_T*) param)->src_id is failed");
                    }
                    break;
                }
            }
        }
        break;
        case MSG_TP_PRESS_UP:
        {
            HandleNotifyWinTpPressDown(win_id,param);
        }
        break;
        case MSG_SLIDEWIN_END:
        {
            total_num = NotifyCenter_GetTotalNum();
            if(total_num > 0)
            {
                GUILIST_SetNeedPrgbar(NOTIFY_LIST_CTRL_ID,TRUE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL );
            }
        }
        break;

        case MSG_GET_FOCUS:
        {
             total_num = NotifyCenter_GetTotalNum();
             if(0 == total_num)
             {
                MMK_CloseWin(win_id);
             }
        }
        break;
        case MSG_LOSE_FOCUS:
        {
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
        }
        break;
        default:
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    }
    return recode;
}
/*****************************************************************************/
//  Description : notifydetail win
//  Global resource dependence :
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyDetailWin_HandleWinMsg(
                        MMI_WIN_ID_T        win_id,
                        MMI_MESSAGE_ID_E    msg_id,
                        DPARAM              param
                        )
{
    MMI_RESULT_E            recode      = MMI_RESULT_TRUE;
    NOTIFY_HANDLE_T         notify_handle   = 0;
    NOTIFY_NODE_T *         p_notify_node       = PNULL;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            NotifyDetailWin_SetCtrlParam(win_id);
        }
        break;
        case MSG_FULL_PAINT:
        {
            //draw bg
            NotifyListWin_DisplayBackground(win_id);
            //draw detail
            NotifyDetailWin_DisplayDetail(win_id);
        }
        break;

        case MSG_CTL_PENOK:
        {
            notify_handle = (NOTIFY_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
            p_notify_node = NotifyCenter_GetNode(notify_handle);
            if(PNULL!= p_notify_node)
            {
                if(PNULL != p_notify_node->notify_info.cb_NotifyEnterApp)
                {
                    p_notify_node->notify_info.cb_NotifyEnterApp(notify_handle);
                }
            }
            MMK_CloseWin(win_id);
            if(MMK_IsOpenWin(NOTIFYLIST_WIN_ID))
            {
                MMK_CloseWin(NOTIFYLIST_WIN_ID);
            }
        }
        break;
        case MSG_GET_FOCUS:
        {

        }
        break;
        case MSG_LOSE_FOCUS:
        {

        }
        break;
        case MSG_CLOSE_WINDOW:
        {
            notify_handle = (NOTIFY_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
            NotifyCenter_Read(notify_handle);
        }
        break;
        default:
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    }
    return recode;
}

/*****************************************************************************/
// Description : enterlistwin
//	Global resource dependence : none
//  Author: dandan.cao
//	Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_EnterListWin(void)
{
	GUI_RECT_T roiRect = NOTIFYLISTWIN_ROIREGION;

	MMI_WIN_ID_T win_id = NOTIFYLIST_WIN_ID;

    MMI_WIN_ID_T old_id = MMK_GetFocusWinId();

	if (MMK_IsOpenWin(win_id))
	{
		MMK_CloseWin(win_id);
	}

	MMK_CreateWin(NOTIFYLIST_WIN_TAB, PNULL);

    do
    {
        uint32 handle = 0;
        tSlideWinInitData init = {0};
        init.win_id = win_id;
        init.win_before_id = old_id;
        init.direction = SLIDEWIN_DIRECTION_VERT;
        init.mode = SLIDEWIN_STARTUPMODE_TP;
        init.style = SLIDEWIN_STYLE_ENABLE_ROI | SLIDEWIN_STYLE_ENABLE_RANGE | SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE;
        init.init_pos = NOTIFYLISTWIN_INIT_POS;
        init.stop_pos = NOTIFYLISTWIN_STOP_POS;
        init.close_pos = NOTIFYLISTWIN_END_POS;
        init.range_min = 0;
        init.range_max = NOTIFYLISTWIN_INIT_POS;
        init.roi_region = roiRect;
        handle = WatchSLIDEWIN_Create(&init);

        WatchSLIDEWIN_Start(handle);
    } while (0);

}
/*****************************************************************************/
//Description : enterdetailwin
//	Global resource dependence : none
//  Author: dandan.cao
//	Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_EnterDetailWin(NOTIFY_HANDLE_T notify_handle)
{
    MMK_CreateWin((uint32 *)NOTIFYDETAIL_WIN_TAB,notify_handle);
}

