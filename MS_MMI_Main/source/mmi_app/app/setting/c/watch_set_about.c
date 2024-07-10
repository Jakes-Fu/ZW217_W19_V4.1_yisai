/*****************************************************************************
** File Name:      watch_set_about.c                                        *
** Author:         bin.wang1                                                 *
** Date:           03/26/2020                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2020      bin.wang1              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
//#include "guilabel.h"
//#include "guibutton.h"
#include "guitext.h"
#include "version.h"

#include "mmidisplay_data.h"
#include "mmicom_trace.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "watch_set_about.h"

#include "watch_set_position.h"
#include "mmisrveng.h"
#include "watch_common.h"
#include "mmieng_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define SETTINGS_VERSION_INFO_MAX_LEN           (255)
#ifdef ZTE_SUPPORT_240X284
#define WATCH_ABOUT_RECT      DP2PX_RECT(20,46,220,283)
#else
#ifdef SCREEN_SHAPE_CIRCULAR
#define WATCH_ABOUT_RECT      DP2PX_RECT(20,46,220,239)
#endif
#endif

/**--------------------------------------------------------------------------*
 **                         RECT DEFINITION                                  *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#define WATCH_MODE_MANUF        ZDT_SFR_MANUF
#define WATCH_MODE_NAME        ZDT_SFR_TYPE
#define WATCH_HARDWARE_VERSION  ZDT_SFR_HW_VER
#define WATCH_SOFTWARE_VERSION  ZDT_SFR_SW_VER

#ifdef ZTE_WATCH
LOCAL void Settings_About_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    const char*     pVersionStr = PNULL;
    uint16          versionStrLen = 0;
    wchar*          pDisplayWStr = PNULL;
    uint16          displayWStrLen = 0;
    wchar           newLineCode = CR_CHAR;
    MMI_STRING_T    newLineStr = { 0 };
    MMI_CTRL_ID_T   textCtrlId = MMISET_ABOUT_TEXT_CTRL_ID;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_FONT_T      font = WATCH_FONT_16;
    GUI_COLOR_T     color = MMI_WHITE_COLOR;
    GUI_RECT_T      text_rect = WATCH_ABOUT_RECT;
    GUI_BG_T        text_bg = {0};
#else
    GUI_FONT_T      font = WATCH_DEFAULT_NORMAL_FONT;
    GUI_COLOR_T     color = MMI_BLACK_COLOR;
#endif
	 MN_IMEISVN_T imeisvn = {0};/*lint !e64*/
    uint8 imei_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    char*           temp_ptr = WATCH_SOFTWARE_VERSION;//PNULL;
    uint16          temp_len = 0;
    uint16 	imei_len = 0;
	uint8 	i;
    pDisplayWStr = (wchar*)SCI_ALLOC_APPZ( sizeof(wchar)*(SETTINGS_VERSION_INFO_MAX_LEN+1) );
    if( PNULL == pDisplayWStr )
    {
        TRACE_APP_SETTINGS("pDisplayWStr is PNULL!!");
        return;
    }
    newLineStr.wstr_ptr = &newLineCode;
    newLineStr.wstr_len = 1;
#if 0
    //Hardware Version
#ifdef WIN32
    pVersionStr = (char*)"HW Version: uws317_watch";
#else
    pVersionStr = VERSION_GetInfo( HW_VERSION );
#endif
    versionStrLen = SCI_STRLEN( (char*)pVersionStr );
    MMIAPICOM_StrcatFromStrToUCS2( pDisplayWStr, &displayWStrLen, (uint8*)pVersionStr, versionStrLen );
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
	//MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_MODEL_NAME);
	//MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
    //MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line

    //Software Version
#ifdef WIN32
    pVersionStr = (char*)"Platform Version: MOCOR_W09.23_Debug";
#else
    pVersionStr = VERSION_GetInfo( PLATFORM_VERSION );   /*lint !e605*/
#endif
    versionStrLen = SCI_STRLEN( (char*)pVersionStr );
    MMIAPICOM_StrcatFromStrToUCS2( pDisplayWStr, &displayWStrLen, (uint8*)pVersionStr, versionStrLen );
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
    #endif

	MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_MODEL_ZTE);

	//	MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
	//MMIAPICOM_ConvertAlphabetToUnicode(WATCH_MODE_NAME,pDisplayWStr,SETTINGS_VERSION_INFO_MAX_LEN);
	//displayWStrLen += MMIAPICOM_Wstrlen(pDisplayWStr);
		MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, WATCH_MODE_NAME, strlen(WATCH_MODE_NAME));
	    MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行

		//MMI_GetLabelTextByLang(TXT_WATCH_IMEI, &title_str);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &title_str);
		MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_IMEI);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行

#if 1
		for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
		   {
		    if (MNNV_GetIMEISVNEx(i, &imeisvn))
		       {
		         
				 MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei, MN_MAX_IMEI_LENGTH<<1, (char*)imei_str);
				 
				 imei_len = (uint16)strlen((char *)imei_str) - 1;
				 MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, (uint8 *)&imei_str[1], imei_len);
				 MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
		       }
		   }
		#endif

		
		
		// SwVersionInfo(content_text);
		//yangyu add
		//MMI_GetLabelTextByLang(TXT_WATCH_SOFTWARE_VERSION, &title_str);
        //MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &title_str);
        MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_SOFTWARE_VERSION);
        //MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
        //temp_ptr = ZDT_GetVersionInfo();
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
		//yangyu end

		
        //MMI_GetLabelTextByLang(TXT_WATCH_HARDWARE_VERSION, &title_str);
       // MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &title_str);
        MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_HARDWARE_VERSION);
       // MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
        MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, WATCH_HARDWARE_VERSION, strlen(WATCH_HARDWARE_VERSION));
        //MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
	
		//content_str.wstr_ptr = content_text;
		//content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
		
    text_bg.bg_type = GUI_BG_COLOR;
    text_bg.color = MMI_BLACK_COLOR;
    CTRLTEXT_SetAlign(textCtrlId,ALIGN_HVMIDDLE);
    GUITEXT_IsDisplayPrg(FALSE,textCtrlId);
    GUITEXT_SetBg(textCtrlId, &text_bg);
    GUITEXT_SetRect( textCtrlId, &text_rect);
    GUITEXT_SetClipboardEnabled (textCtrlId,FALSE);//bug 2087169

    GUITEXT_SetString( textCtrlId, pDisplayWStr, displayWStrLen, FALSE );
    GUITEXT_SetFont( textCtrlId, &font, &color );

    SCI_FREE( pDisplayWStr );
}
#else

LOCAL void Settings_About_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    const char*     pVersionStr = PNULL;
    uint16          versionStrLen = 0;
    wchar*          pDisplayWStr = PNULL;
    uint16          displayWStrLen = 0;
    wchar           newLineCode = CR_CHAR;
    MMI_STRING_T    newLineStr = { 0 };
    MMI_CTRL_ID_T   textCtrlId = MMISET_ABOUT_TEXT_CTRL_ID;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_FONT_T      font = WATCH_FONT_22;
    GUI_COLOR_T     color = MMI_WHITE_COLOR;
    GUI_RECT_T      text_rect = WATCH_ABOUT_RECT;
    GUI_BG_T        text_bg = {0};
#else
    GUI_FONT_T      font = WATCH_DEFAULT_NORMAL_FONT;
    GUI_COLOR_T     color = MMI_BLACK_COLOR;
#endif
	 MN_IMEISVN_T imeisvn = {0};/*lint !e64*/
    uint8 imei_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    char*           temp_ptr = WATCH_SOFTWARE_VERSION;//PNULL;
    uint16          temp_len = 0;
    uint16 	imei_len = 0;
	uint8 	i;
    pDisplayWStr = (wchar*)SCI_ALLOC_APPZ( sizeof(wchar)*(SETTINGS_VERSION_INFO_MAX_LEN+1) );
    if( PNULL == pDisplayWStr )
    {
        TRACE_APP_SETTINGS("pDisplayWStr is PNULL!!");
        return;
    }
    newLineStr.wstr_ptr = &newLineCode;
    newLineStr.wstr_len = 1;
#if 0
    //Hardware Version
#ifdef WIN32
    pVersionStr = (char*)"HW Version: uws317_watch";
#else
    pVersionStr = VERSION_GetInfo( HW_VERSION );
#endif
    versionStrLen = SCI_STRLEN( (char*)pVersionStr );
    MMIAPICOM_StrcatFromStrToUCS2( pDisplayWStr, &displayWStrLen, (uint8*)pVersionStr, versionStrLen );
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
	//MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_MODEL_NAME);
	//MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
    //MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line

    //Software Version
#ifdef WIN32
    pVersionStr = (char*)"Platform Version: MOCOR_W09.23_Debug";
#else
    pVersionStr = VERSION_GetInfo( PLATFORM_VERSION );   /*lint !e605*/
#endif
    versionStrLen = SCI_STRLEN( (char*)pVersionStr );
    MMIAPICOM_StrcatFromStrToUCS2( pDisplayWStr, &displayWStrLen, (uint8*)pVersionStr, versionStrLen );
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
    #endif

	MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_MODEL_NAME);

		MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
		MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, WATCH_MODE_NAME, strlen(WATCH_MODE_NAME));
	    MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行

		//MMI_GetLabelTextByLang(TXT_WATCH_IMEI, &title_str);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &title_str);
		MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_IMEI);
		MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行

#if 1
		for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
		   {
		    if (MNNV_GetIMEISVNEx(i, &imeisvn))
		       {
		         
				 MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei, MN_MAX_IMEI_LENGTH<<1, (char*)imei_str);
				 
				 imei_len = (uint16)strlen((char *)imei_str) - 1;
				 MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, (uint8 *)&imei_str[1], imei_len);
				 MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
		       }
		   }
		#endif

		
		
		// SwVersionInfo(content_text);
		//yangyu add
		//MMI_GetLabelTextByLang(TXT_WATCH_SOFTWARE_VERSION, &title_str);
        //MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &title_str);
        MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_SOFTWARE_VERSION);
        MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
        //temp_ptr = ZDT_GetVersionInfo();
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
		//yangyu end

		
        //MMI_GetLabelTextByLang(TXT_WATCH_HARDWARE_VERSION, &title_str);
       // MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &title_str);
        MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,TXT_WATCH_HARDWARE_VERSION);
        MMIAPICOM_StrcatFromSTRINGToUCS2(pDisplayWStr, &displayWStrLen, &newLineStr);//换行
        MMIAPICOM_StrcatFromStrToUCS2(pDisplayWStr, &displayWStrLen, WATCH_HARDWARE_VERSION, strlen(WATCH_HARDWARE_VERSION));
        //MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
	
		//content_str.wstr_ptr = content_text;
		//content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
		
#ifdef SCREEN_SHAPE_CIRCULAR
    text_bg.bg_type = GUI_BG_COLOR;
    text_bg.color = MMI_BLACK_COLOR;

    GUITEXT_IsDisplayPrg(FALSE,textCtrlId);
    GUITEXT_SetBg(textCtrlId, &text_bg);
    GUITEXT_SetRect( textCtrlId, &text_rect);
    GUITEXT_SetClipboardEnabled (textCtrlId,FALSE);//bug 2087169
#endif
    GUITEXT_SetString( textCtrlId, pDisplayWStr, displayWStrLen, FALSE );
    GUITEXT_SetFont( textCtrlId, &font, &color );

    SCI_FREE( pDisplayWStr );
}
#endif

#ifdef ADULT_WATCH_SUPPORT
LOCAL void Settings_About_FullPaint( MMI_WIN_ID_T win_id )
{
    MMI_STRING_T	 str_ptr = { 0 };
    const char*      pVersionStr = PNULL;
    uint16           versionStrLen = 0;
    wchar*           pDisplayWStr = PNULL;
    uint16           displayWStrLen = 0;
    wchar            newLineCode = CR_CHAR;
    MMI_STRING_T     newLineStr = { 0 };
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUISTR_STYLE_T   text_style = {0};
    GUISTR_STATE_T   text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUI_RECT_T       win_rect = DP2PX_RECT(0, 0, 240, 240);

    pDisplayWStr = (wchar*)SCI_ALLOC_APPZ( sizeof(wchar)*(SETTINGS_VERSION_INFO_MAX_LEN+1) );
    if( PNULL == pDisplayWStr )
    {
        TRACE_APP_SETTINGS("pDisplayWStr is PNULL!!");
        return;
    }
    newLineStr.wstr_ptr = &newLineCode;
    newLineStr.wstr_len = 1;

    //Hardware Version
#ifdef WIN32
    pVersionStr = (char*)"HW Version: uws317_watch";
#else
    pVersionStr = VERSION_GetInfo( HW_VERSION );
#endif
    versionStrLen = SCI_STRLEN( (char*)pVersionStr );
    MMIAPICOM_StrcatFromStrToUCS2( pDisplayWStr, &displayWStrLen, (uint8*)pVersionStr, versionStrLen );
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line

    //Software Version
#ifdef WIN32
    pVersionStr = (char*)"Platform Version: MOCOR_W09.23_Debug";
#else
    pVersionStr = VERSION_GetInfo( PLATFORM_VERSION );   /*lint !e605*/
#endif
    versionStrLen = SCI_STRLEN( (char*)pVersionStr );
    MMIAPICOM_StrcatFromStrToUCS2( pDisplayWStr, &displayWStrLen, (uint8*)pVersionStr, versionStrLen );
    MMIAPICOM_StrcatFromSTRINGToUCS2( pDisplayWStr, &displayWStrLen, &newLineStr );   //new line

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    text_style.align = ALIGN_HMIDDLE;
    text_style.font = ADULT_WATCH_DEFAULT_NORMAL_FONT;
    text_style.font_color = MMI_WHITE_COLOR;
    win_rect.top += MMI_TITLE_THEME_HEIGHT;
    LCD_FillRect(&lcd_dev_info,win_rect,MMI_BLACK_COLOR);
    str_ptr.wstr_ptr = pDisplayWStr;
    str_ptr.wstr_len = displayWStrLen;

    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &win_rect,
        &win_rect,
        &str_ptr,	   
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

    SCI_FREE( pDisplayWStr );
}
#endif
LOCAL void Settings_About_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E  HandleSettingsRegardWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
#ifndef ADULT_WATCH_SUPPORT
        case MSG_OPEN_WINDOW:
        {
            Settings_About_OPEN_WINDOW( win_id );
            break;
        }
#ifdef SCREEN_SHAPE_CIRCULAR
        case MSG_FULL_PAINT:
        {
            MMI_MENU_GROUP_ID_T         group_id        =   0;
		    MMI_MENU_ID_T               menu_id         =   0;
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
	LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
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
			
	MMI_GetLabelTextByLang(TEXT_WATCH_INFO, &string);
	GUISTR_DrawTextToLCDInRect( 
					(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
					(const GUI_RECT_T	   *)&text_rect,	   //the fixed display area
					(const GUI_RECT_T	   *)&text_rect,	   //用户要剪切的实际区域
					(const MMI_STRING_T    *)&string,
					&text_style,
					state,
					GUISTR_TEXT_DIR_AUTO
					);
#endif

			
            //WATCHCOM_DisplayBackground(win_id);
            break;
        }
#endif
#else
        case MSG_FULL_PAINT:
            Settings_About_FullPaint( win_id );
            break;
#endif
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_About_APP_CANCEL( win_id );
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        case MSG_CTL_PENOK:
#endif
        case MSG_KEYDOWN_5:
        {
            if(!MMIENG_IsMonkeyTestMode())
            {
                MMIAPIENG_MainWinEnter( 0,FALSE );
            }
            break;
        }
		case MSG_TP_PRESS_UP:
		{
		        GUI_POINT_T   point = {0};
			 GUI_RECT_T   back_rect=WINDOW_BACK_RECT; 
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);

                     if (GUI_PointIsInRect(point, back_rect))
                     {
			     MMK_CloseWin(win_id);
			     return;	 
                     }
		}
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> regard
WINDOW_TABLE( MMISET_ABOUT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsRegardWindow ),
    WIN_ID( MMISET_ABOUT_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_SET_SYSTEM_ABOUT),
#else
    CREATE_TEXT_CTRL( MMISET_ABOUT_TEXT_CTRL_ID ),
    //WIN_SOFTKEY( TXT_NULL, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

PUBLIC void Settings_RegardWin_Enter( void )
{
    MMI_HANDLE_T win_handle = MMK_CreateWin( (uint32 *)MMISET_ABOUT_WIN_TAB, PNULL );
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
}

