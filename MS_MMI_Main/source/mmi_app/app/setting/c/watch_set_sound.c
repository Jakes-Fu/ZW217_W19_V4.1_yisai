/*****************************************************************************
** File Name:      watch_set_sound.c                                         *
** Author:         bin.wang1                                                 *
** Date:           02/21/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description: Display sound settings items                                 *
******************************************************************************
**                         Important Edit History
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/21/2020           bin.wang1             Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmicom_trace.h"

#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "mmienvset_export.h"
#include "watch_set_sound.h"
#include "watch_common_radiolist_win.h"
#include "watch_commonwin_export.h"
#include "watch_common.h"
#include "watch_set_mmsound.h"
#include "watch_set_notesound.h"
#include "mmidisplay_data.h"

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define SETTINGS_SOUND_RING_ITEM_MAX                (3)
#define SETTINGS_SOUND_CHARGE_ITEM_MAX              (2)

typedef enum
{
    SETTINGS_SOUND_MM_SOUND,
    SETTINGS_SOUND_NOTE_SOUND,
    SETTINGS_SOUND_RING_SOUND,
    SETTINGS_SOUND_ALARM_SOUND,
    SETTINGS_SOUND_CHARGE_SOUND,
    SETTINGS_SOUND_NOTE_TYPE,
    SETTINGS_SOUND_ITEM_MAX
}SETTINGS_SOUND_LIST_ITEM_E;

//================================================================================/
//================== settings->sound->call ring select win data ==================/
//================================================================================/
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_call_ring_item_one = { TXT_SET_SOUND_CALL_RING_ONE };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_call_ring_item_two = { TXT_SET_SOUND_CALL_RING_TWO };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_call_ring_item_thr = { TXT_SET_SOUND_CALL_RING_THREE };

#ifdef ADULT_WATCH_SUPPORT
LOCAL WATCHCOM_LIST_ITEM__ST s_sound_call_ring_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_call_ring_item_one,     PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_call_ring_item_two,     PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_call_ring_item_thr,     PNULL },
};
#else
LOCAL WATCHCOM_LIST_ITEM__ST s_sound_call_ring_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_call_ring_item_one,     PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_call_ring_item_two,     PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_call_ring_item_thr,     PNULL },
};
#endif

LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_sound_call_ring_win              = { MMISET_CALL_RING_SELECT_WIN_ID,
                                                                            MMISET_CALL_RING_SELECT_LIST_CTRL_ID,
                                                                            s_sound_call_ring_list,
                                                                            PNULL,
                                                                            0,
                                                                            SETTINGS_SOUND_RING_ITEM_MAX,
#ifdef ADULT_WATCH_SUPPORT
                                                                            TXT_SET_SOUND_LIST_RING_SOUND
#endif
};

//================================================================================/
//================== settings->sound->alarm ring select win data =================/
//================================================================================/
LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_sound_alarm_ring_win             = { MMISET_ALARM_RING_SELECT_WIN_ID,
                                                                            MMISET_ALARM_RING_SELECT_LIST_CTRL_ID,
                                                                            s_sound_call_ring_list,
                                                                            PNULL,
                                                                            0,
                                                                            SETTINGS_SOUND_RING_ITEM_MAX,
#ifdef ADULT_WATCH_SUPPORT
                                                                            TXT_SET_SOUND_LIST_ALARM_SOUND
#endif
};

//================================================================================/
//================== settings->sound->charge ring select win data ================/
//================================================================================/

#ifdef ADULT_WATCH_SUPPORT
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_charge_item_none       = { TXT_NONE };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_charge_item_default    = { TXT_SET_SOUND_DEFAULT_RING };
LOCAL WATCHCOM_LIST_ITEM__ST s_sound_charge_ring_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_charge_item_none,      PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_charge_item_default,   PNULL },
};
#else
#ifdef SCREEN_SHAPE_CIRCULAR
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_charge_item_none       = { TXT_NONE };
#else
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_charge_item_none       = { TXT_RING_SILENT };
#endif
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_charge_item_default    = { TXT_SET_SOUND_DEFAULT_RING };
LOCAL WATCHCOM_LIST_ITEM__ST s_sound_charge_ring_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_charge_item_none,      PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_charge_item_default,   PNULL },
};
#endif

LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_sound_charge_ring_win            = { MMISET_CHARGE_RING_SELECT_WIN_ID,
                                                                            MMISET_CHARGE_RING_SELECT_LIST_CTRL_ID,
                                                                            s_sound_charge_ring_list,
                                                                            PNULL,
                                                                            0,
                                                                            SETTINGS_SOUND_CHARGE_ITEM_MAX,
#ifdef ADULT_WATCH_SUPPORT
                                                                            TXT_SET_SOUND_LIST_CHARGE_SOUND
#endif
};

//================================================================================/
//================== settings->sound->note type select win data ==================/
//================================================================================/
typedef enum
{
    SETTINGS_SOUND_NOTE_TYPE_RING,
    SETTINGS_SOUND_NOTE_TYPE_VIBRA,
    SETTINGS_SOUND_NOTE_TYPE_RING_AND_VIBRA,
    SETTINGS_SOUND_NOTE_TYPE_MAX
}SETTINGS_SOUND_NOTE_TYPE_ITEM_E;


#ifdef ADULT_WATCH_SUPPORT
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_only_ring     = { TXT_SET_SOUND_NOTE_MODE_ONLY_RING };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_only_vibrate  = { TXT_SET_SOUND_NOTE_MODE_ONLY_VIBAR };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_ring_vibrate  = { TXT_SET_SOUND_NOTE_MODE_RING_AND_VIBAR };
LOCAL WATCHCOM_LIST_ITEM__ST s_sound_note_type_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_note_only_ring,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_note_only_vibrate, PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_sound_note_ring_vibrate, PNULL },
};
#else
#ifdef SCREEN_SHAPE_CIRCULAR
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_only_ring     = { TXT_SET_RING_RING };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_only_vibrate  = { TXT_RING_VIBRA };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_ring_vibrate  = { TXT_SET_RING_VIBRA_AND_RING };
#else
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_only_ring     = { TXT_SET_SOUND_NOTE_MODE_ONLY_RING };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_only_vibrate  = { TXT_SET_SOUND_NOTE_MODE_ONLY_VIBRATION };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_sound_note_ring_vibrate  = { TXT_SET_SOUND_NOTE_MODE_RING_AND_VIBRATION };
#endif
LOCAL WATCHCOM_LIST_ITEM__ST s_sound_note_type_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_note_only_ring,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_note_only_vibrate, PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_sound_note_ring_vibrate, PNULL },
};
#endif

LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_sound_note_type_win              = { MMISET_NOTE_TYPE_SELECT_WIN_ID,
                                                                            MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID,
                                                                            s_sound_note_type_list,
                                                                            PNULL,
                                                                            0,
                                                                            SETTINGS_SOUND_NOTE_TYPE_MAX,
#ifdef ADULT_WATCH_SUPPORT
                                                                            TXT_SET_SOUND_LIST_NOTE_MODE
#endif
};

//================================================================================/
//================== settings->sound or ring preview logic =======================/
//================================================================================/

typedef struct
{
    MMISET_ALL_RING_TYPE_E type;
    uint16 id;
    uint8 volume;
    BOOLEAN isVibrate;
}MMI_SETTINGS_RING_PREVIEW_PARAM;

#define SETTINGS_RING_PRIVIEW_DEFAULT_TIME              (1000)
#define SETTINGS_RING_PRIVIEW_REPEAT_TIMES              (1)
#define SETTINGS_VIBRATE_PRIVIEW_REPEAT_TIMES           (20)

LOCAL uint8 s_settings_preview_timer_id = 0;

LOCAL BOOLEAN s_settings_ring_preview_is_runing = FALSE;

LOCAL BOOLEAN s_settings_vibrate_preview_is_runing = FALSE;
LOCAL uint8 s_settings_vibrate_preview_timer_id = 0;
LOCAL uint8 s_settings_vibrate_preview_count = 0;
LOCAL MMISET_ALL_RING_TYPE_E s_settings_vibrate_type = MMISET_RING_TYPE_CALL;

LOCAL MMI_SETTINGS_RING_PREVIEW_PARAM s_settings_preview_ring_param = { 0 };


LOCAL void SettingsStopVibratePreviewTimer( void )
{
    TRACE_APP_SETTINGS(" enter");

    if ( s_settings_vibrate_preview_timer_id != 0 )
    {
        MMK_StopTimer( s_settings_vibrate_preview_timer_id );
        s_settings_vibrate_preview_timer_id = 0;
    }
}

LOCAL void _Callback_Settings_RingPreview( MMISRVAUD_REPORT_RESULT_E result, DPARAM param )
{
    TRACE_APP_SETTINGS(" enter");

    s_settings_ring_preview_is_runing = FALSE;

    if( TRUE == s_settings_vibrate_preview_is_runing )
    {
        SettingsStopVibratePreviewTimer();

        MMIAPISET_StopVibrator( s_settings_vibrate_type );
        s_settings_vibrate_preview_is_runing = FALSE;
    }
}

LOCAL void _Callback_Settings_VibrateTimer( uint8 timer_id, uint32 param )
{
    TRACE_APP_SETTINGS("enter");

    s_settings_vibrate_preview_count++;

    if( TRUE == s_settings_vibrate_preview_is_runing )
    {
        TRACE_APP_SETTINGS("stop vibrate");
        MMIAPISET_StopVibrator( (MMISET_ALL_RING_TYPE_E)param );
        s_settings_vibrate_preview_is_runing = FALSE;

        s_settings_vibrate_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_RING_PRIVIEW_DEFAULT_TIME, _Callback_Settings_VibrateTimer, (uint32)s_settings_vibrate_type, FALSE );
    }
    else
    {
        if( s_settings_vibrate_preview_count < SETTINGS_VIBRATE_PRIVIEW_REPEAT_TIMES )
        {
            TRACE_APP_SETTINGS("start vibrate");
            MMIAPIENVSET_StartVibrator( (MMISET_ALL_RING_TYPE_E)param );
            s_settings_vibrate_preview_is_runing = TRUE;

            s_settings_vibrate_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_RING_PRIVIEW_DEFAULT_TIME, _Callback_Settings_VibrateTimer, (uint32)s_settings_vibrate_type, FALSE );
        }
    }
}

LOCAL void _Callback_Settings_PreviewTimer( uint8 timer_id, uint32 param )
{
    MN_DUAL_SYS_E                       dualSys = MMIAPISET_GetActiveSim();
    MMI_SETTINGS_RING_PREVIEW_PARAM*    pPreviewParam = (MMI_SETTINGS_RING_PREVIEW_PARAM*)param;

    TRACE_APP_SETTINGS(" enter");

    if( s_settings_preview_timer_id != timer_id )
    {
        TRACE_APP_SETTINGS("timer_id error!");
        return;
    }
    s_settings_preview_timer_id = 0;

    if( PNULL == pPreviewParam )
    {
        TRACE_APP_SETTINGS("pPreviewParam is null!");
        return;
    }

    if( pPreviewParam->type != MMISET_RING_TYPE_MAX )
    {
        MMIAPISET_PreviewRing( dualSys, pPreviewParam->id, pPreviewParam->volume, SETTINGS_RING_PRIVIEW_REPEAT_TIMES, pPreviewParam->type, _Callback_Settings_RingPreview );
        s_settings_ring_preview_is_runing = TRUE;
    }

    if( TRUE == pPreviewParam->isVibrate )
    {
        s_settings_vibrate_preview_count = 0;
        s_settings_vibrate_type = pPreviewParam->type;
        s_settings_vibrate_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_RING_PRIVIEW_DEFAULT_TIME, _Callback_Settings_VibrateTimer, (uint32)s_settings_vibrate_type, FALSE );

        MMIAPIENVSET_StartVibrator( s_settings_vibrate_type );
        s_settings_vibrate_preview_is_runing  = TRUE;
    }
}

LOCAL void SettingsStopPreviewTimer( void )
{
    TRACE_APP_SETTINGS(" enter");

    if ( s_settings_preview_timer_id != 0 )
    {
        MMK_StopTimer( s_settings_preview_timer_id );
        s_settings_preview_timer_id = 0;
    }
}

PUBLIC void Settings_StopPreviewRing( void )
{
    TRACE_APP_SETTINGS(" enter");

    SettingsStopPreviewTimer();
    SettingsStopVibratePreviewTimer();

    if( TRUE == s_settings_ring_preview_is_runing )
    {
        MMIAPISET_StopAppRing();
        s_settings_ring_preview_is_runing = FALSE;
    }
    if( TRUE == s_settings_vibrate_preview_is_runing )
    {
        MMIAPISET_StopVibrator( s_settings_vibrate_type );
        s_settings_vibrate_preview_is_runing = FALSE;
    }
}

PUBLIC void Settings_PreviewRing( uint16 ring_id, MMISET_ALL_RING_TYPE_E ring_type, uint8 ring_volume, BOOLEAN is_vibrate )
{
    TRACE_APP_SETTINGS(" enter");

    Settings_StopPreviewRing();

    s_settings_preview_ring_param.id = ring_id;
    s_settings_preview_ring_param.type = ring_type;
    s_settings_preview_ring_param.volume = ring_volume;
    s_settings_preview_ring_param.isVibrate = is_vibrate;

    s_settings_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_RING_PRIVIEW_DEFAULT_TIME, _Callback_Settings_PreviewTimer, (uint32)(&s_settings_preview_ring_param), FALSE );
}
#ifdef ZTE_WATCH
//#if 1
//================================================================================/
//================== settings->sound->call ring select win =======================/
//================================================================================/
LOCAL MMI_RESULT_E HandleCallRingSelWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode =  MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();

    switch(msg_id)
    {

        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_CALL_RING_SELECT_LIST_CTRL_ID );
            uint16 ringId = curIdx + 1;
            uint8  volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );

            Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, volume, FALSE );
           // MMIAPIENVSET_SetFixedCallRingId( dualSys, ringId, MMIAPIENVSET_GetActiveModeId() );
            break;
        }
	case MSG_OPEN_WINDOW:
	{
		uint16 listNum=0;
		uint16 ringId =0;

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
		GUILIST_SetRect(MMISET_CALL_RING_SELECT_LIST_CTRL_ID,&list_rect);
		listNum = sizeof(s_sound_call_ring_list)/sizeof(WATCHCOM_LIST_ITEM__ST);
		WatchCOM_RadioList_Create(s_sound_call_ring_list,listNum,MMISET_CALL_RING_SELECT_LIST_CTRL_ID);
		/* append list items*/
		//change to radio list
		CTRLLIST_ChangeDisplayType(MMISET_CALL_RING_SELECT_LIST_CTRL_ID,GUILIST_RADIOLIST_E,TRUE);
		ringId = MMIAPIENVSET_GetFixCallRingId( dualSys, MMIAPIENVSET_GetActiveModeId() );
		ringId = ringId-1;
		 GUILIST_SetSelectedItem(MMISET_CALL_RING_SELECT_LIST_CTRL_ID, ringId, TRUE);

    		//set current item
   		GUILIST_SetCurItemIndex(MMISET_CALL_RING_SELECT_LIST_CTRL_ID, ringId);
		break;
	}
	case MSG_FULL_PAINT:
	{
		uint16 curSelection = 0;
		int32 listOffset = 0;
		 uint16          ringId =0;
		WATCHCOM_DisplayBackground(win_id);

		GUILIST_GetTopItemOffset(MMISET_CALL_RING_SELECT_LIST_CTRL_ID, &listOffset);

		curSelection = GUILIST_GetCurItemIndex( MMISET_CALL_RING_SELECT_LIST_CTRL_ID );

		GUILIST_SetSelectedItem(MMISET_CALL_RING_SELECT_LIST_CTRL_ID, curSelection, TRUE);

		//set current item
		GUILIST_SetCurItemIndex(MMISET_CALL_RING_SELECT_LIST_CTRL_ID, curSelection);

		// set top item offset
		GUILIST_SetTopItemOffset(MMISET_CALL_RING_SELECT_LIST_CTRL_ID, listOffset);					

		break;
	}
	
        case MSG_GET_FOCUS:// Bug 2113002
        {
            WATCHCOM_DisplayBackground(win_id);
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
		if(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID ==((MMI_NOTIFY_T*)param)->src_id)
		{
			uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_CALL_RING_SELECT_LIST_CTRL_ID );
			uint16 ringId = curIdx + 1;
			Settings_StopPreviewRing();
			MMIAPIENVSET_SetFixedCallRingId( dualSys, ringId, MMIAPIENVSET_GetActiveModeId() );
			MMK_CloseWin(win_id);
		}
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
        {
            Settings_StopPreviewRing();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
WINDOW_TABLE( MMISET_CALLRINGSEL_WIN_TAB ) =
{
	WIN_FUNC((uint32)HandleCallRingSelWindow ),
	WIN_ID( MMISET_CALLRINGSEL_WIN_ID ),
	WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_SET_BACK_LIGHT),
#endif    
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_RING_SELECT_LIST_CTRL_ID),
	CREATE_BUTTON_CTRL( IMAGE_COMMON_ONEBTN_BG_IMAG, MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID),
	//WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
	END_WIN
};

PUBLIC void Settings_CallRingSelectWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_CALLRINGSEL_WIN_TAB, PNULL );
}
#else
//================================================================================/
//================== settings->sound->call ring select win =======================/
//================================================================================/
LOCAL MMI_RESULT_E _Callback_CallRingSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode =  MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();

    switch(msg_id)
    {

        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_CALL_RING_SELECT_LIST_CTRL_ID );
            uint16 ringId = curIdx + 1;
            uint8  volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );

            Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, volume, FALSE );

            MMIAPIENVSET_SetFixedCallRingId( dualSys, ringId, MMIAPIENVSET_GetActiveModeId() );


            break;
        }
		case MSG_FULL_PAINT:
        {
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
			GUI_RECT_T          content_rect={0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; ////xiongkai ADD_TITLE_IN_ALL_SETTING_MENU	
			GUI_RECT_T          title_rect={0,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 	
			GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
			GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	
			GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
			GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
			MMI_STRING_T        string = {0};	
			GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
#endif

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
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

			MMI_GetLabelTextByLang(TXT_RING, &string);
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
            break;
        }
	case MSG_TP_PRESS_UP:
	{
		GUI_RECT_T   back_rect=WINDOW_BACK_RECT; 
		GUI_POINT_T   point = {0};

		point.x = MMK_GET_TP_X(param);
		point.y = MMK_GET_TP_Y(param);
		if (GUI_PointIsInRect(point, back_rect))
		{
			MMK_CloseWin( win_id);
			return;
		}
	}
	break;
        case MSG_GET_FOCUS:// Bug 2113002
        {
            WATCHCOM_DisplayBackground(win_id);
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
        {
            Settings_StopPreviewRing();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
PUBLIC void Settings_CallRingSelectWin_Enter( void )
{
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    uint16          ringId = MMIAPIENVSET_GetFixCallRingId( dualSys, MMIAPIENVSET_GetActiveModeId() );
    GUI_RECT_T          list_rect = WATCHLIST_RECT;

    s_sound_call_ring_win.listCurIdx    = ringId - 1;
    s_sound_call_ring_win.pCallback     = _Callback_CallRingSelectWin;
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_RadioListWin_Create( &s_sound_call_ring_win );
#else
    WatchCOM_RadioListWin_Create( &s_sound_call_ring_win );
#ifdef SCREEN_SHAPE_CIRCULAR
    GUILIST_SetRect(MMISET_CALL_RING_SELECT_LIST_CTRL_ID,&list_rect);
#endif
#endif
}

#endif

//================================================================================/
//================== settings->sound->alarm ring select win ======================/
//================================================================================/

LOCAL MMI_RESULT_E _Callback_AlarmRingSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_ALARM_RING_SELECT_LIST_CTRL_ID );
            uint16 ringId = curIdx + 1;
            uint8  volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_ALARM );

            Settings_PreviewRing( ringId, MMISET_RING_TYPE_ALARM, volume, FALSE );

            MMIAPIENVSET_SetFixedAlarmRingId( curIdx+1, MMIAPIENVSET_GetActiveModeId() );
            break;
        }
        case MSG_GET_FOCUS:// Bug 2113002
        {
            WATCHCOM_DisplayBackground(win_id);
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
        {
            Settings_StopPreviewRing();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
PUBLIC void Settings_AlarmRingSelectWin_Enter( void )
{
    uint16 ringId = MMIAPIENVSET_GetFixAlarmRingId( MMIAPIENVSET_GetActiveModeId() );
    GUI_RECT_T          list_rect = WATCHLIST_RECT;

    s_sound_alarm_ring_win.listCurIdx = ringId-1;
    s_sound_alarm_ring_win.pCallback = _Callback_AlarmRingSelectWin;
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_RadioListWin_Create( &s_sound_alarm_ring_win );
#else
    WatchCOM_RadioListWin_Create( &s_sound_alarm_ring_win );
#ifdef SCREEN_SHAPE_CIRCULAR
    GUILIST_SetRect(MMISET_ALARM_RING_SELECT_LIST_CTRL_ID,&list_rect);
#endif
#endif
}

//================================================================================/
//================== settings->sound->charge ring select win =====================/
//================================================================================/

LOCAL MMI_RESULT_E _Callback_ChargeRingSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint16          curIdx = GUILIST_GetCurItemIndex( MMISET_CHARGE_RING_SELECT_LIST_CTRL_ID );
            uint16          ringId = curIdx; // here index 0 mean silent
            uint8           volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );
            MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();

            if( 0 == curIdx ) //silent not play ring
            {
                Settings_StopPreviewRing();
            }
            else
            {
                Settings_PreviewRing( ringId, MMISET_RING_TYPE_CHARGE, volume, FALSE );
            }

            MMIENVSET_SetActiveModeOptValue( CHARGE_NOTIFY, (uint8)ringId, dualSys );
            break;
        }
        case MSG_GET_FOCUS:// Bug 2113002
        {
            WATCHCOM_DisplayBackground(win_id);
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
        {
            Settings_StopPreviewRing();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
PUBLIC void Settings_ChargeRingSelectWin_Enter( void )
{
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    GUI_RECT_T          list_rect = WATCHLIST_RECT;

    s_sound_charge_ring_win.listCurIdx = (uint16)MMIAPIENVSET_GetActiveModeOptValue( dualSys, CHARGE_NOTIFY );
    s_sound_charge_ring_win.pCallback = _Callback_ChargeRingSelectWin;
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_RadioListWin_Create( &s_sound_charge_ring_win );
#else
#ifdef SCREEN_SHAPE_CIRCULAR
    WatchCOM_RadioListWin_Create( &s_sound_charge_ring_win );
    GUILIST_SetRect(MMISET_CHARGE_RING_SELECT_LIST_CTRL_ID,&list_rect);
#else
    WatchCOM_RadioListWin_Create( &s_sound_charge_ring_win );
#endif
#endif
}

//================================================================================/
//================== settings->sound->note type select win =======================/
//================================================================================/
#ifdef ZTE_WATCH
//#if 1
LOCAL uint16 SoundGetNoteTypeSelectedIndex( void )
{
    uint16          index = SETTINGS_SOUND_NOTE_TYPE_RING_AND_VIBRA;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    // use call type as all ring type(call msg alarm)
    uint8           noteType = MMIAPIENVSET_GetActiveModeOptValue( dualSys, CALL_RING_TYPE );

    if( MMISET_CALL_RING == noteType )
    {
        index = SETTINGS_SOUND_NOTE_TYPE_RING;
    }
    else if( MMISET_CALL_VIBRA == noteType )
    {
        index = SETTINGS_SOUND_NOTE_TYPE_VIBRA;
    }
    else if( MMISET_CALL_VIBRA_AND_RING == noteType )
    {
        index = SETTINGS_SOUND_NOTE_TYPE_RING_AND_VIBRA;
    }
    else
    {
        TRACE_APP_SETTINGS("noteType error!! set to default!!");
        index = SETTINGS_SOUND_NOTE_TYPE_RING_AND_VIBRA;
        MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA_AND_RING, dualSys );
        MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
        MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_CALL_VIBRA_AND_RING, dualSys );
    }
    return index;
}
LOCAL MMI_RESULT_E Handle_NoteTypeSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode =  MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();

    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
	{
		uint16 listNum=0;
		uint16 notetype_id =0;

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
		GUILIST_SetRect(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID,&list_rect);
		listNum = sizeof(s_sound_note_type_list)/sizeof(WATCHCOM_LIST_ITEM__ST);
		WatchCOM_RadioList_Create(s_sound_note_type_list,listNum,MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID);
		/* append list items*/
		//change to radio list
		//CTRLLIST_ChangeDisplayType(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID,GUILIST_RADIOLIST_E,TRUE);
		notetype_id = SoundGetNoteTypeSelectedIndex( );
		 //set selected item
		GUILIST_SetSelectedItem(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID, notetype_id, TRUE);

		//set current item
		GUILIST_SetCurItemIndex( MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID, notetype_id );

		MMK_SetAtvCtrl( win_id, MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID );
		break;
	}
        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID );
            uint16 ringId = MMIAPIENVSET_GetFixCallRingId( dualSys, MMIAPIENVSET_GetActiveModeId() );
            uint8  volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );

            TRACE_APP_WLAN("curIdx %d ringId %d volume %d dualSys %d", curIdx, ringId, volume, dualSys);

            if( SETTINGS_SOUND_NOTE_TYPE_RING == curIdx )
            {
                Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, volume, FALSE );

                //MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_RING, dualSys );
                //MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_RING, dualSys );
                //MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_RING, dualSys ); // alarm use msg type
            }
            else if( SETTINGS_SOUND_NOTE_TYPE_VIBRA == curIdx )
            {
                //MMISET_RING_TYPE_MAX not play ring
                Settings_PreviewRing( ringId, MMISET_RING_TYPE_MAX, volume, TRUE );

               // MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA, dualSys );
               // MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA, dualSys );
              //  MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_VIBRA, dualSys );
            }
            else
            {
                Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, volume, TRUE );

                //MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA_AND_RING, dualSys );
               // MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
                //MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
            }

            break;
        }
        case MSG_GET_FOCUS:// Bug 2113002
        case MSG_FULL_PAINT:
        {
		uint16 curSelection = 0;
		int32 listOffset = 0;
		uint16         notetype_id =0;
		WATCHCOM_DisplayBackground(win_id);

		GUILIST_GetTopItemOffset(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID, &listOffset);

		curSelection = GUILIST_GetCurItemIndex( MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID );

		GUILIST_SetSelectedItem(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID, curSelection, TRUE);

		//set current item
		GUILIST_SetCurItemIndex(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID, curSelection);

		// set top item offset
		GUILIST_SetTopItemOffset(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID, listOffset);
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
		if(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID ==((MMI_NOTIFY_T*)param)->src_id)
		{
			uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID );
			Settings_StopPreviewRing();
			 if( SETTINGS_SOUND_NOTE_TYPE_RING == curIdx )
	            {
	                MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_RING, dualSys );
	                MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_RING, dualSys );
	                MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_RING, dualSys ); // alarm use msg type
	            }
	            else if( SETTINGS_SOUND_NOTE_TYPE_VIBRA == curIdx )
	            {
	                //MMISET_RING_TYPE_MAX not play ring
	                MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA, dualSys );
	                MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA, dualSys );
	                MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_VIBRA, dualSys );
	            }
	            else
	            {
	                MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA_AND_RING, dualSys );
	                MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
	                MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
	            }
			MMK_CloseWin(win_id);
		}
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
        {
            Settings_StopPreviewRing();
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}



WINDOW_TABLE( MMISET_NOTE_TYPE_SELECT_WIN_TAB ) =
{
	WIN_FUNC((uint32)Handle_NoteTypeSelectWin ),
	WIN_ID( MMISET_NOTE_TYPE_SELECT_WIN_ID ),
	WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_SET_BACK_LIGHT),
#endif    
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID),
	CREATE_BUTTON_CTRL( IMAGE_COMMON_ONEBTN_BG_IMAG, MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID),
	//WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
	END_WIN
};

PUBLIC void Settings_NoteTypeSelectWin_Enter( void )
{
    MMI_HANDLE_T win_handle = MMK_CreateWin( (uint32 *)MMISET_NOTE_TYPE_SELECT_WIN_TAB, PNULL );
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
}

#else
LOCAL MMI_RESULT_E _Callback_NoteTypeSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode =  MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();

    switch(msg_id)
    {
        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID );
            uint16 ringId = MMIAPIENVSET_GetFixCallRingId( dualSys, MMIAPIENVSET_GetActiveModeId() );
            uint8  volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );

            TRACE_APP_WLAN("curIdx %d ringId %d volume %d dualSys %d", curIdx, ringId, volume, dualSys);

            if( SETTINGS_SOUND_NOTE_TYPE_RING == curIdx )
            {
                Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, volume, FALSE );

                MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_RING, dualSys );
                MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_RING, dualSys );
                MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_RING, dualSys ); // alarm use msg type
            }
            else if( SETTINGS_SOUND_NOTE_TYPE_VIBRA == curIdx )
            {
                //MMISET_RING_TYPE_MAX not play ring
                Settings_PreviewRing( ringId, MMISET_RING_TYPE_MAX, volume, TRUE );

                MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA, dualSys );
                MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA, dualSys );
                MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_VIBRA, dualSys );
            }
            else
            {
                Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, volume, TRUE );

                MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA_AND_RING, dualSys );
                MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
                MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
            }

            break;
        }
        case MSG_GET_FOCUS:// Bug 2113002
        {
            WATCHCOM_DisplayBackground(win_id);
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
        {
            Settings_StopPreviewRing();
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

LOCAL uint16 SoundGetNoteTypeSelectedIndex( void )
{
    uint16          index = SETTINGS_SOUND_NOTE_TYPE_RING_AND_VIBRA;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    // use call type as all ring type(call msg alarm)
    uint8           noteType = MMIAPIENVSET_GetActiveModeOptValue( dualSys, CALL_RING_TYPE );

    if( MMISET_CALL_RING == noteType )
    {
        index = SETTINGS_SOUND_NOTE_TYPE_RING;
    }
    else if( MMISET_CALL_VIBRA == noteType )
    {
        index = SETTINGS_SOUND_NOTE_TYPE_VIBRA;
    }
    else if( MMISET_CALL_VIBRA_AND_RING == noteType )
    {
        index = SETTINGS_SOUND_NOTE_TYPE_RING_AND_VIBRA;
    }
    else
    {
        TRACE_APP_SETTINGS("noteType error!! set to default!!");
        index = SETTINGS_SOUND_NOTE_TYPE_RING_AND_VIBRA;
        MMIENVSET_SetActiveModeOptValue( CALL_RING_TYPE, MMISET_CALL_VIBRA_AND_RING, dualSys );
        MMIENVSET_SetActiveModeOptValue( MSG_RING_TYPE, MMISET_MSG_VIBRA_AND_RING, dualSys );
        MMIENVSET_SetActiveModeOptValue( ALARM_RING_TYPE, MMISET_CALL_VIBRA_AND_RING, dualSys );
    }
    return index;
}

PUBLIC void Settings_NoteTypeSelectWin_Enter( void )
{
    GUI_RECT_T          list_rect = WATCHLIST_RECT;

    s_sound_note_type_win.listCurIdx    = SoundGetNoteTypeSelectedIndex();
    s_sound_note_type_win.pCallback     = _Callback_NoteTypeSelectWin;
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_RadioListWin_Create( &s_sound_note_type_win );
#else
#ifdef SCREEN_SHAPE_CIRCULAR
    WatchCOM_RadioListWin_Create( &s_sound_note_type_win );
    GUILIST_SetRect(MMISET_NOTE_TYPE_SELECT_LIST_CTRL_ID,&list_rect);
#else
    WatchCOM_RadioListWin_Create( &s_sound_note_type_win );
#endif
#endif
}
#endif

//================================================================================/
//======================= settings->sound win ====================================/
//================================================================================/

/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/

LOCAL void Settings_Sound_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T   listCtrlId =  MMISET_SOUND_LIST_CTRL_ID;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T          list_rect = WATCHLIST_RECT;
    GUILIST_SetRect(MMISET_SOUND_LIST_CTRL_ID,&list_rect);
#endif
    GUILIST_SetMaxItem( listCtrlId, SETTINGS_SOUND_ITEM_MAX,  FALSE );
    MMK_SetAtvCtrl( win_id, listCtrlId );
}

#ifdef ADULT_WATCH_SUPPORT
LOCAL MMI_RESULT_E _Callback_MM_SoundWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_PRGBOX_CHANGE:
        {
            uint8 cur_vol = *(uint8*)param;
            uint16 ringId = MMIAPIENVSET_GetFixCallRingId( MN_DUAL_SYS_1, MMIAPIENVSET_GetActiveModeId() );

            MMIAPISET_SetMultimVolume( cur_vol );
            Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, cur_vol, FALSE );

            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

LOCAL MMI_RESULT_E _Callback_Progress_SoundWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_PRGBOX_CHANGE:
        {
            uint8 cur_vol = *(uint8*)param;
            uint16 ringId = MMIAPIENVSET_GetFixCallRingId( MN_DUAL_SYS_1, MMIAPIENVSET_GetActiveModeId() );

            TRACE_APP_SETTINGS("_Callback_Progress_SoundWin current vol is %d,ringId is %d", cur_vol,ringId);

            MMIENVSET_SetActiveModeOptValue( CALL_RING_VOL, cur_vol, MN_DUAL_SYS_1 );
            MMIENVSET_SetActiveModeOptValue( MSG_RING_VOL, cur_vol, MN_DUAL_SYS_1 );
            MMIENVSET_SetActiveModeOptValue( ALARM_RING_VOL, cur_vol, MN_DUAL_SYS_1 );

            Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, cur_vol, FALSE );
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif
LOCAL void Settings_Sound_APP_OK( void )
{
    uint16 index = GUILIST_GetCurItemIndex( MMISET_SOUND_LIST_CTRL_ID );

    switch (index)
    {
        case SETTINGS_SOUND_MM_SOUND:
        {
#ifdef ADULT_WATCH_SUPPORT
            uint32 MMVolume = (uint32)MMIAPISET_GetMultimVolume();

            TRACE_APP_SETTINGS("Settings_Sound_APP_OK,current mm volume is %d",MMVolume);
            WatchCOM_ProgressWin_Enter(MMISET_MM_SOUND_WIN_ID,res_aw_volume_high,res_aw_volume_low,MMISET_VOL_MAX,MMVolume,_Callback_MM_SoundWin);
#else
            Settings_MMSoundWin_Enter();
#endif
            break;
        }

        case SETTINGS_SOUND_NOTE_SOUND:
        {
#ifdef ADULT_WATCH_SUPPORT
            uint32 NoteVolume = (uint32)MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_CALL);

            TRACE_APP_SETTINGS("Settings_Sound_APP_OK,current note volume is %d",NoteVolume);
            WatchCOM_ProgressWin_Enter(MMISET_MM_SOUND_WIN_ID,res_aw_volume_high,res_aw_volume_low,MMISET_VOL_MAX,NoteVolume,_Callback_Progress_SoundWin);
#else
            Settings_NoteSoundWin_Enter();
#endif
            break;
        }

        case SETTINGS_SOUND_RING_SOUND:
        {
            Settings_CallRingSelectWin_Enter();
            break;
        }

        case SETTINGS_SOUND_ALARM_SOUND:
        {
            Settings_AlarmRingSelectWin_Enter();
            break;
        }

        case SETTINGS_SOUND_CHARGE_SOUND:
        {
            Settings_ChargeRingSelectWin_Enter();
            break;
        }

        case SETTINGS_SOUND_NOTE_TYPE:
        {
            Settings_NoteTypeSelectWin_Enter();
            break;
        }

        default:
        {
            TRACE_APP_SETTINGS(" index error %d", index);
            break;
        }
    }
}

LOCAL void Settings_Sound_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_Sound_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16          curSelection = 0;
    int32           listOffset = 0;
    uint16          ringId = 0;
    uint16          modeIndex = 0;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    MMI_TEXT_ID_T*  pStr2TextId = TXT_NULL;
    MMI_STRING_T    itemStr = { 0 };
    MMI_STRING_T    itemStr2 ={ 0 };
    MMI_CTRL_ID_T   listCtrlId =  MMISET_SOUND_LIST_CTRL_ID;
    WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST* pStStr2Text;

    // get top item offset
    GUILIST_GetTopItemOffset( listCtrlId, &listOffset );
    // get current select item index
    curSelection = GUILIST_GetCurItemIndex( listCtrlId );
    GUILIST_RemoveAllItems( listCtrlId );

    //multi-media sound
    MMI_GetLabelTextByLang( TXT_SET_SOUND_LIST_MM_SOUND, &itemStr );
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );
#else
#ifdef SCREEN_SHAPE_CIRCULAR
    WatchCOM_ListItem_Draw_1Str( listCtrlId, itemStr);
#else
    WatchCOM_ListItem_Draw_1Str_1Icon( listCtrlId, itemStr, IMAGE_SET_SOUND_LIST_RIGHT );
#endif
#endif

    //note sound
    MMI_GetLabelTextByLang( TXT_SET_SOUND_LIST_NOTE_SOUND, &itemStr );
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );
#else
#ifdef SCREEN_SHAPE_CIRCULAR
    WatchCOM_ListItem_Draw_1Str( listCtrlId, itemStr);
#else
    WatchCOM_ListItem_Draw_1Str_1Icon( listCtrlId, itemStr, IMAGE_SET_SOUND_LIST_RIGHT );
#endif
#endif

    //ring sound
    MMI_GetLabelTextByLang( TXT_SET_SOUND_LIST_RING_SOUND,  &itemStr );
    ringId = MMIAPIENVSET_GetFixCallRingId( dualSys, MMIAPIENVSET_GetActiveModeId() );
    if( ringId <= SETTINGS_SOUND_RING_ITEM_MAX )
    {
        pStStr2Text = (WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST*)s_sound_call_ring_list[ringId-1].data_ptr;
        if( pStStr2Text != PNULL )
        {
            MMI_GetLabelTextByLang( pStStr2Text->text_id,  &itemStr2 );
        }
        else
        {
            TRACE_APP_SETTINGS("pStStr2Text is PNULL!!");
        }
    }
    else
    {
        TRACE_APP_SETTINGS("ringId error %d", ringId);
    }
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_ListItem_Draw_2Str_2line( listCtrlId, itemStr, itemStr2);
#else
    WatchCOM_ListItem_Draw_2Str( listCtrlId, itemStr, itemStr2 );
#endif

    //alarm sound
    MMI_GetLabelTextByLang( TXT_SET_SOUND_LIST_ALARM_SOUND,  &itemStr );
    ringId = MMIAPIENVSET_GetFixAlarmRingId( MMIAPIENVSET_GetActiveModeId() );
    if( ringId <= SETTINGS_SOUND_RING_ITEM_MAX )
    {
        pStStr2Text = (WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST*)s_sound_call_ring_list[ringId-1].data_ptr;
        if( pStStr2Text != PNULL )
        {
            MMI_GetLabelTextByLang( pStStr2Text->text_id,  &itemStr2 );
        }
        else
        {
            TRACE_APP_SETTINGS("pStStr2Text is PNULL!!");
        }
    }
    else
    {
        TRACE_APP_SETTINGS("ringId error %d", ringId);
    }
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_ListItem_Draw_2Str_2line( listCtrlId, itemStr, itemStr2);
#else
    WatchCOM_ListItem_Draw_2Str( listCtrlId, itemStr, itemStr2 );
#endif

    //charge sound
    MMI_GetLabelTextByLang( TXT_SET_SOUND_LIST_CHARGE_SOUND, &itemStr );
    ringId = (uint16)MMIAPIENVSET_GetActiveModeOptValue( dualSys, CHARGE_NOTIFY );
    if( ringId <= SETTINGS_SOUND_CHARGE_ITEM_MAX )
    {
        pStStr2Text = (WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST*)s_sound_charge_ring_list[ringId].data_ptr;
        if( pStStr2Text != PNULL )
        {
            MMI_GetLabelTextByLang( pStStr2Text->text_id,  &itemStr2 );
        }
        else
        {
            TRACE_APP_SETTINGS("pStStr2Text is PNULL!!");
        }
    }
    else
    {
        TRACE_APP_SETTINGS("ringId error %d", ringId);
    }

#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_ListItem_Draw_2Str_2line( listCtrlId, itemStr, itemStr2);
#else
    WatchCOM_ListItem_Draw_2Str(  listCtrlId, itemStr, itemStr2 );
#endif

    //note mode
    MMI_GetLabelTextByLang( TXT_SET_SOUND_LIST_NOTE_MODE, &itemStr );
    modeIndex = SoundGetNoteTypeSelectedIndex();
    if( modeIndex <= SETTINGS_SOUND_NOTE_TYPE_MAX )
    {
        pStStr2Text = (WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST*)s_sound_note_type_list[modeIndex].data_ptr;
        if( pStStr2Text != PNULL )
        {
            MMI_GetLabelTextByLang( pStStr2Text->text_id,  &itemStr2 );
        }
        else
        {
            TRACE_APP_SETTINGS("pStStr2Text is PNULL!!");
        }
    }
    else
    {
        TRACE_APP_SETTINGS("modeIndex error %d", modeIndex);
    }
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_ListItem_Draw_2Str_2line( listCtrlId, itemStr, itemStr2);
#else
    WatchCOM_ListItem_Draw_2Str( listCtrlId, itemStr, itemStr2 );
#endif
    //set current item
    GUILIST_SetCurItemIndex( listCtrlId, curSelection );
    // set top item offset
    GUILIST_SetTopItemOffset( listCtrlId, listOffset );
}

/*****************************************************************************/
//  Description : to handle Settings window message
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingsSoundWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Settings_Sound_OPEN_WINDOW( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            WATCHCOM_DisplayBackground(win_id);
#endif
            Settings_Sound_FULL_PAINT( win_id );
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_Sound_APP_OK();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_Sound_APP_CANCEL( win_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
WINDOW_TABLE( MMISET_SOUND_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleSettingsSoundWindow ),
    WIN_ID( MMISET_SOUND_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_SET_LIST_SOUND),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SOUND_LIST_CTRL_ID),
#ifndef ADULT_WATCH_SUPPORT
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
#endif    
    END_WIN
};



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
// idle -> settings -> sound
PUBLIC void Settings_SoundWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_SOUND_WIN_TAB, PNULL );
}

