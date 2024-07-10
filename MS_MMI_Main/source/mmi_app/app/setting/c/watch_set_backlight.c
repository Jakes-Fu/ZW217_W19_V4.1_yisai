/*****************************************************************************
** File Name:      watch_set_backlight.c                                          *
** Author:         qi.liu1                                                 *
** Date:           06/11/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 06/11/2020           qi.liu1             Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"

#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "watch_set_clock.h"
#include "watch_set_about.h"
#include "watch_set_about.h"
#include "watch_commonwin_export.h"
#include "watch_set_backlight.h"
#include "mmiset_export.h"
#include "guisetlist.h"

#include "mmidisplay_color.h"
#include "watch_common.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST	list_item_forever	= {TXT_SET_TIME_OPEN_FOREVER};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST	list_item_30s = {TXT_SET_TIME_30S};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST	list_item_20s = {TXT_SET_TIME_20S};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST	list_item_15s = {TXT_SET_TIME_15S};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST	list_item_10s = {TXT_COMM_10SECONDS};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST	list_item_5s = {TXT_SET_TIME_5S};

static MMISET_BACKLIGHT_SET_T      g_backlightinfo     =   {0};		//back light info

LOCAL uint8 s_current_index = 0;
#ifdef ADULT_WATCH_SUPPORT
LOCAL WATCHCOM_LIST_ITEM__ST s_demo_backlight_list_data[] =
{
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,	&list_item_forever,   PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,	&list_item_30s,       PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,	&list_item_20s,       PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,	&list_item_15s,       PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,	&list_item_10s,       PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,	&list_item_5s,        PNULL},
};
#else
LOCAL WATCHCOM_LIST_ITEM__ST s_demo_backlight_list_data[] =
{
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,	&list_item_forever,   PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,	&list_item_30s,  PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,	&list_item_20s,   PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,	&list_item_15s,   PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,	&list_item_10s,   PNULL},
	{WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,	&list_item_5s,   PNULL},
};
#endif

/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : Settings_BackLight_FULL_PAINT
//  Global resource dependence :
//  Author:qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void Settings_BackLight_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    uint16 curSelection = 0;
    int32 listOffset = 0;
    //uint16 listNum=0;
#ifdef ZTE_WATCH
	WATCHCOM_DisplayBackground(win_id);
#else
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
	GUI_RECT_T          content_rect={0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; ////xiongkai ADD_TITLE_IN_ALL_SETTING_MENU	
	GUI_RECT_T          title_rect={0,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 	
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
	GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	
	GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
	GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
	MMI_STRING_T        string = {0};	
	GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
	
		//draw TITLE bg		
			GUIRES_DisplayImg(PNULL,
			    &rect,
			    PNULL,
			    win_id,
			    IMAGE_THEME_BLACK_BG_SPEC, 
			    &lcd_dev_info); 

		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = SONG_FONT_28;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;

		MMI_GetLabelTextByLang(TXT_SET_BRIGHT_SCREEN_TIME, &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );	
#endif
#endif

    // get top item offset
    GUILIST_GetTopItemOffset(MMISET_BACKLIGHT_LIST_CTRL_ID, &listOffset);

    
    /* append list items*/
    //change to radio list
    CTRLLIST_ChangeDisplayType(MMISET_BACKLIGHT_LIST_CTRL_ID,GUILIST_RADIOLIST_E,TRUE);
   // listNum = sizeof(s_demo_backlight_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
   // WatchCOM_RadioList_Create(s_demo_backlight_list_data,listNum,MMISET_BACKLIGHT_LIST_CTRL_ID);
    g_backlightinfo = WATCHAPISET_GetBackLightinfo();

    //select backlight type
    SCI_TRACE_LOW("watchset:Settings_BackLight_FULL_PAINT backlight_type = %d",g_backlightinfo.backlight_type);
    switch (g_backlightinfo.backlight_type)
    {
        case MMISET_BACKLIGHT_OPEN:
            switch (g_backlightinfo.backlight_time)
            {
                case MMISET_TIME_30:
                    curSelection = 1;
                    break;

                 case MMISET_TIME_20:
                    curSelection = 2;
                    break;

                 case MMISET_TIME_15:
                    curSelection = 3;
                    break;

                 case MMISET_TIME_10:
                    curSelection = 4;
                    break;

                 case MMISET_TIME_5:
                    curSelection = 5;
                    break;

                 default:
		            SCI_TRACE_LOW("watchset:Settings_BackLight_FULL_PAINT default");
                 break;
           }
           break;

           //常开
       case MMISET_BACKLIGHT_OPEN_FOREVER:
           curSelection = 0;
           break;

       case MMISET_BACKLIGHT_CLOSE:
		   SCI_TRACE_LOW("watchset:Settings_BackLight_FULL_PAINT MMISET_BACKLIGHT_CLOSE");
           break;

       default:
           break;
    }
	
    GUILIST_SetSelectedItem(MMISET_BACKLIGHT_LIST_CTRL_ID, curSelection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(MMISET_BACKLIGHT_LIST_CTRL_ID, curSelection);

    // set top item offset
    GUILIST_SetTopItemOffset(MMISET_BACKLIGHT_LIST_CTRL_ID, listOffset);
}

LOCAL void Settings_BackLight_APP_WEB(void)
{
	uint32	cur_selection	=	0;

	//get the selected item
	cur_selection = GUILIST_GetCurItemIndex(MMISET_BACKLIGHT_LIST_CTRL_ID);
	
	SCI_TRACE_LOW("watchlq:Settings_BackLight_APP_WEB:cur_selection=%d",cur_selection);

	switch (cur_selection)
	{
	case 0:
		g_backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;
		break;

	case 1:
		g_backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		g_backlightinfo.backlight_time = MMISET_TIME_30;
		break;

	case 2:
		g_backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		g_backlightinfo.backlight_time = MMISET_TIME_20;
		break;

	case 3:
		g_backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		g_backlightinfo.backlight_time = MMISET_TIME_15;
		break;

	case 4:
		g_backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		g_backlightinfo.backlight_time = MMISET_TIME_10;
		break;

	case 5:
		g_backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		g_backlightinfo.backlight_time = MMISET_TIME_5;
		break;

	default:
		break;
	}
#ifndef ZTE_WATCH
    MMIAPISET_SetBackLightInfo(g_backlightinfo);
    MMIDEFAULT_StartLcdBackLightTimer();//bug 2130532
    MMK_CloseWin(MMISET_BACKLIGHT_WIN_ID);
#endif
}



LOCAL MMI_RESULT_E HandleSettingsBacklightWindow(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
)
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msgId)
    {
        case MSG_OPEN_WINDOW: 
        {
	    uint16 listNum=0;
#ifndef ZTE_WATCH
        GUI_RECT_T list_rect = WATCHLIST_RECT;
#else
	 GUI_RECT_T list_rect = WATCHK1LIST_RECT;
	GUI_RECT_T watchk1_setok_rect = WATCHK1_SETOK_RECT;
	 GUI_BG_T    bg_info ={0};
	 bg_info.bg_type = GUI_BG_IMG;
	 bg_info.img_id = IMAGE_COMMON_ONEBTN_BG_IMAG;
	GUIBUTTON_SetRect(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID, &watchk1_setok_rect);
	//CTRLBUTTON_SetTextId(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID,TXT_COMMON_OK );
	//CTRLBUTTON_SetFontSize(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID,SONG_FONT_24 );
	//CTRLBUTTON_SetFontColor(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID,MMI_WHITE_COLOR);
	//CTRLBUTTON_SetBg(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID,&bg_info);
#endif
	    GUILIST_SetRect(MMISET_BACKLIGHT_LIST_CTRL_ID,&list_rect);
	    listNum = sizeof(s_demo_backlight_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
          WatchCOM_RadioList_Create(s_demo_backlight_list_data,listNum,MMISET_BACKLIGHT_LIST_CTRL_ID);
		  
	
           break;
        }
        case MSG_FULL_PAINT:
        {
           Settings_BackLight_FULL_PAINT(winId);
           break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
       case MSG_CTL_OK:
       case MSG_APP_OK:
       case MSG_APP_WEB:
       case MSG_CTL_MIDSK:
        {
			if(MMISET_BACKLIGHT_LIST_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
				Settings_BackLight_APP_WEB();
			#ifdef ZTE_WATCH
			else if(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID ==((MMI_NOTIFY_T*)param)->src_id)
				{
				 MMIAPISET_SetBackLightInfo(g_backlightinfo);
    				MMIDEFAULT_StartLcdBackLightTimer();//bug 2130532
    				MMK_CloseWin(MMISET_BACKLIGHT_WIN_ID);
				}
			#endif
            break;
        }
#ifndef ZTE_WATCH
	case MSG_TP_PRESS_UP:
	{
		GUI_RECT_T   back_rect=WINDOW_BACK_RECT; 
		GUI_POINT_T   point = {0};

		point.x = MMK_GET_TP_X(param);
		point.y = MMK_GET_TP_Y(param);
		if (GUI_PointIsInRect(point, back_rect))
		{
			MMK_CloseWin( winId);
			return;
		}
	}
     		break;
#endif
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(winId);
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}


WINDOW_TABLE( MMISET_BACKLIGHT_WIN_TAB ) =
{
	WIN_FUNC((uint32)HandleSettingsBacklightWindow ),
	WIN_ID( MMISET_BACKLIGHT_WIN_ID ),
	WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_SET_BACK_LIGHT),
#endif    
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_BACKLIGHT_LIST_CTRL_ID),
#ifdef ZTE_WATCH
	CREATE_BUTTON_CTRL( IMAGE_COMMON_ONEBTN_BG_IMAG, MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID),
#endif
	//WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
	END_WIN
};

 /**--------------------------------------------------------------------------*
  **						 FUNCTION DEFINITION							  *
  **--------------------------------------------------------------------------*/
 // idle -> settings -> backlight
 PUBLIC void Settings_BackLight_Enter( void )
 {
	 MMK_CreateWin( (uint32 *)MMISET_BACKLIGHT_WIN_TAB, PNULL );
 }

