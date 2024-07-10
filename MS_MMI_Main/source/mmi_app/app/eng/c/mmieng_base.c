/****************************************************************************
** File Name:      mmieng_base.h							                                                 *
** Author:          hui.zhao								                                                 *
** Date:             2009.07.28														*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.      				*
** Description:    this part will also be used when eng_support macor is closed  									               					*
*****************************************************************************
**                         Important Edit History                        							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             						*
** 07/2009      hui.zhao               Create
**
****************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guilabel.h"

#include "version.h"

//#include "mmieng.h"
#include "mmieng_internal.h"
#include "mmieng_id.h"
#include "mmieng_win.h"
#include "mmieng_text.h"
#include "mmieng_uitestwin.h"
#ifdef SPRD_MARLIN2_INTEG_SUPPORT
#include "sprdwcn_bsp_common.h"
#endif
#include "gps_interface.h"
#include "mn_api.h"

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMIENG_VERSION_BUF_MAX_LEN  100

/**--------------------------------------------------------------------------*
			                         LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E EngShowVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void ShowVersionInfo(BOOLEAN   need_update);
#ifdef ENG_SUPPORT
LOCAL MMI_RESULT_E EngShowOutVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
/**--------------------------------------------------------------------------*
			                         GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/
const MMI_ENG_DISPLAY_T mmi_eng_display =
{
	MMI_DEFAULT_TEXT_FONT,				// text_font
	MMI_DEFAULT_TEXT_COLOR,				// text_color
	MMI_RED_COLOR,						// spec_text_color
	MMI_WHITE_COLOR,//NEWMS00181616     // bkgd_color
	0,									// char_space
	0,									// line_space
	SCROLL_BAR_WIDTH					// scrollbar_width
};

BOOLEAN g_mmieng_showversion_from_ui = FALSE;	//from uitest
BOOLEAN g_mmieng_showversion_for1 = FALSE; //  TRUE: #*8378#1#; FALSE: #*8378#0#

/**--------------------------------------------------------------------------*
**                         WINTAB DEFINITION                                *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIENG_SHOWVERSION_WIN_TAB) =
{
    WIN_HIDE_STATUS,
	WIN_FUNC((uint32)EngShowVersionWinHandleMsg),
	WIN_ID(MMIENG_SHOWVERSION_WIN_ID),
#if defined MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_TITLE(TXT_ENG_VERSION_INFO),
	//WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
    CREATE_TEXT_CTRL(MMIENG_SHOW_VERSION_CTRL_ID),
    END_WIN
};

#ifdef ENG_SUPPORT
//显示外部版本号
WINDOW_TABLE(MMIENG_SHOW_OUTVERSION_WIN_TAB) =
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
	WIN_FUNC((uint32)EngShowOutVersionWinHandleMsg),
	WIN_ID(MMIENG_SHOW_OUTVERSION_WIN_ID),
	WIN_TITLE(TXT_ENG_VERSION_INFO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	//CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIENG_LABEL1_CTRL_ID),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
//#ifdef SHOW_HW_VERSION
//	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIENG_LABEL2_CTRL_ID),
//	CREATE_TEXT_CTRL(MMIENG_TEXTBOX2_CTRL_ID),
//#endif
	END_WIN
};

/*===============================================================================
								IMPLEMENTATION
===============================================================================*/
/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowOutVersionWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_SHOW_OUTVERSION_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			EngShowOutVersionWinHandleMsg
 DESCRIPTION:
 PARAM IN:		win_id -
				msg_id -
				param -
 PARAM OUT:
 AUTHOR:		allen
 DATE:			2004.09.08
********************************************************************************/
LOCAL MMI_RESULT_E EngShowOutVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;

	int32 length = 0;
	int32 offset = 0;
// 	char buffer[MMIENG_VERSION_BUF_MAX_LEN] = {0};
	char text[MMIENG_VERSION_BUF_MAX_LEN]= {0} ;

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
        {
            const char* str = PNULL;
            wchar wstr[MMIENG_VERSION_BUF_MAX_LEN] = {0};

            str = "Software V:\n";
            length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

#ifndef _WIN32
            //text.wstr_ptr = COMMON_GetMMIOutVersionInfo();
            str =  VERSION_GetInfo(MMI_VERSION);
#else
            str = "GPHONE_R1.0.8003.C0.2";	// this is a example for simulator
#endif
	   length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

		text[offset] = CR_CHAR;
		offset += 1;

#ifdef SHOW_HW_VERSION
            str = "Hardware V:\n";
            length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

#ifndef _WIN32
            str =  VERSION_GetInfo(HW_VERSION);
#else
            str = "SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
#endif

	   length = strlen(str);
	    SCI_MEMCPY(text + offset, str, length);
		offset += length ;

		text[offset] = CR_CHAR;
		offset += 1;
#endif
            MMI_STRNTOWSTR( wstr, MMIENG_VERSION_BUF_MAX_LEN, (uint8*)text, offset, offset );

            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr, offset, FALSE);

        }
        break;

	case MSG_FULL_PAINT:
		// the softkey is different between UI-TEST mode and Engineer mode
		if (g_mmieng_showversion_from_ui)
			GUIWIN_SetSoftkeyTextId(MMIENG_SHOW_OUTVERSION_WIN_ID,  TXT_NULL, (MMI_TEXT_ID_T)STXT_UP, TXT_NULL, FALSE);
		else
			GUIWIN_SetSoftkeyTextId(MMIENG_SHOW_OUTVERSION_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
		break;


    case MSG_APP_OK:
    case MSG_APP_CANCEL:
	case MSG_CTL_MIDSK:
	case MSG_CTL_OK:
	case MSG_CTL_CANCEL:
		// only in UI-TEST mode, UP key will do the next action; otherwise will do nothing
		if (g_mmieng_showversion_from_ui)
		{
			MMIAPIENG_RunUITestNextStep();
		}
		 MMK_CloseWin(win_id);

		break;

	case MSG_CLOSE_WINDOW:
		g_mmieng_showversion_for1 = FALSE;
		break;

#ifdef FLIP_PHONE_SUPPORT
	case MSG_KEYDOWN_FLIP:		// close flip
		// in UI-TEST mode, close FLIP will do nothing
		if (!g_mmieng_showversion_from_ui)
			result = MMI_RESULT_FALSE;
		break;
#endif

	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}
#endif
/********************************************************************************/
//  NAME:			MMIAPIENG_CreateShowOutVersionWin
//  DESCRIPTION:
//  PARAM IN:
//  PARAM OUT:
//  AUTHOR:		songbin.zeng
//  DATE:			2006.10.11
/********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowVersionWin(void)
{
    MMI_HANDLE_T handle = PNULL;

    handle = MMK_CreateWin((uint32*)MMIENG_SHOWVERSION_WIN_TAB, PNULL);

    if (PNULL == handle)
    {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************************/
//  NAME:          EngShowVersionWinHandleMsg
//  DESCRIPTION:
//  PARAM IN:      win_id -
//                 msg_id -
//                 param -
//  PARAM OUT:
//  AUTHOR:        allen
//  DATE:          2004.09.08
/********************************************************************************/
LOCAL MMI_RESULT_E EngShowVersionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(!g_mmieng_showversion_for1)
        {
            if(g_mmieng_showversion_from_ui)
            {
                GUITEXT_SetCircularHandle(FALSE,MMIENG_SHOW_VERSION_CTRL_ID);
            }
        }
        MMK_SetAtvCtrl(win_id, MMIENG_SHOW_VERSION_CTRL_ID);

        // fix cr187363
        // set version info into text when created.
        // while setting at MSG_FULL_PAINT, it couldn't be selected all by clipboard
        ShowVersionInfo(FALSE); //for 3D effect cr190356, modify parameter from TRUE to FALSE to avoid to draw content to LCD in open_window msg.

        // the softkey is different between UI-TEST mode and Engineer mode
        if (!g_mmieng_showversion_for1)
        {
            GUIWIN_SetSoftkeyTextId(MMIENG_SHOWVERSION_WIN_ID,  TXT_ENG_PASS, TXT_NULL, TXT_ENG_FAIL, FALSE);
        }
        else
        {
             GUIWIN_SetSoftkeyTextId(MMIENG_SHOWVERSION_WIN_ID,  TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, STXT_RETURN, FALSE);
        }

        break;

    case MSG_FULL_PAINT:
     break;
     
     //add chunjuanliang
         case MSG_KEYDOWN_OK:
         case MSG_CTL_OK:
         {
             if (!g_mmieng_showversion_for1)
             {
                 //save result = 1
                 MMIAPIENG_SaveTestResult(win_id,1);
                 //call nextstep
                 MMIAPIENG_RunUITestNextStep();
                
             }
      
             //close winid;
             MMK_CloseWin(win_id);
      
             break;
         }
         case MSG_KEYDOWN_CANCEL:
		 case MSG_CTL_CANCEL:
         {
             if (!g_mmieng_showversion_for1)
             {
                 //save result = 0
                 MMIAPIENG_SaveTestResult(win_id,0);
                 //call nextstep
                 MMIAPIENG_RunUITestNextStep();
             }
      
             //close winid;
             MMK_CloseWin(win_id);
      
             break;
         }
         //add chunjuanliang


#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    //case MSG_KEYDOWN_OK:
    //case MSG_KEYDOWN_CANCEL:
    //case MSG_CTL_OK:
    //case MSG_CTL_CANCEL:
        /*
        // only in Engineer mode, OK / CANCEL key will close this window; otherwise will do nothing
        if (!g_mmieng_showversion_for1)
        {
            if (g_mmieng_showversion_from_ui)
            {
              MMIAPIENG_RunUITestNextStep();
            }
        }
        MMK_CloseWin(win_id);
    */
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        if (!g_mmieng_showversion_for1)
        {
            if (!g_mmieng_showversion_from_ui)
            {
                MMK_CloseWin(MMIENG_SHOWVERSION_WIN_ID);
            }
        }
        else
        {
            g_mmieng_showversion_for1 = FALSE;
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP:      // close flip
        // in UI-TEST mode, close FLIP will do nothing
        if (g_mmieng_showversion_for1)
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif

    case MSG_CLOSE_WINDOW:
        // not in UI-TEST mode
        if (g_mmieng_showversion_for1)
        {
            g_mmieng_showversion_for1 = FALSE ;
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : 显示版本信息(包括软件,硬件,base版本)
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowVersionInfo(
                            BOOLEAN   need_update
                            )
{
    wchar           unicode_cr = 0x0d;
    MMI_STRING_T    cr_s = {0};
    MMI_STRING_T    text_s = {0};
    //uint16          uint16_str[MMIENG_3RDPARTYVER_TEXT_LEN + 1] = {0};
    uint16          uint16_str_len = 0;
    char*           temp_ptr = PNULL;
    uint16          temp_len = 0;
    uint8           modem_ver[MODEM_VERSION_STR_LEN +1] = {0};
    uint16           *uint16_str = PNULL;

    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;

    uint16_str = (uint16*)SCI_ALLOC_APPZ( sizeof(uint16)*(MMIENG_VERSION_TEXT_LEN+1));

    if(PNULL == uint16_str)
    {
        SCI_TRACE_LOW("ShowVersionInfo malloc fail");
        return;
    }

//Software V:
    temp_ptr = (char*)"Software V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#ifndef WIN32
    temp_ptr = VERSION_GetInfo(PLATFORM_VERSION);//get platform version
#else
    temp_ptr = (char*)"Platform Version:MOCOR_W09.23_Debug";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

//Project V:
    temp_ptr = (char*)"Project V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
    temp_ptr = VERSION_GetInfo(PROJECT_VERSION);//get project version
#else
    temp_ptr = (char*)"Project Version:sc7701_320X480_PDA_64MB";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

//Hardware V:
    temp_ptr = (char*)"Hardware V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
    temp_ptr = VERSION_GetInfo(HW_VERSION);//get hw version
#else
    temp_ptr = (char*)"HW Version:SC6600L_sp6601l";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

 //Base V:
    temp_ptr = (char*)"Base V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
#ifdef PLATFORM_SC6800H
    temp_ptr = SCI_GetBaseVersionInfo();
#else
    temp_ptr = VERSION_GetInfo(BASE_VERSION);//get base version
#endif
#else
    temp_ptr = (char*)"BASE Version:BASE_W09.23";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

//Modem V:
    temp_ptr = (char*)"Modem V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
    MMIPI_GetModemVersion(modem_ver);//temp_ptr = Modem_GetVersionInfo(); //get modem version
#else
    //modem_ver = (char*)"Modem Version: Modem 0.0";
#endif
    temp_len = SCI_STRLEN((char*)modem_ver);
    SCI_TRACE_LOW(" modem ver temp_len =%d",temp_len);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)modem_ver, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

//AP V:
    temp_ptr = (char*)"AP V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#if defined (PLATFORM_ANTISW3)
#ifndef WIN32
    temp_ptr = VERSION_GetInfo(AP_VERSION );
#else
    temp_ptr = (char*)"AP Version: XXXX";
#endif
#elif defined (PLATFORM_UWS6121E)
#ifndef _WIN32
    temp_ptr = VERSION_GetInfo(AP_SVN_VERSION);     /*lint !e605*/
#else
//    temp_ptr = "SP7100A-V1.0.1-B7-M6416 SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
    temp_ptr = (char*)"AP SVN :    0000";
#endif
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

 //GPS V:
#ifdef GPS_SUPPORT
    temp_ptr = (char*)"GPS V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
    // TODO:temp_ptr = GPS_GetVersionInfo(); //get gps version
#else
    temp_ptr = (char*)"GPS Version: GPS 0.0";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    SCI_TRACE_LOW("[engbase][Version]: gps temp char :%s,len:%d",temp_ptr,temp_len);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif

//BT V:
#ifdef BLUETOOTH_SUPPORT
    temp_ptr = (char*)"BT V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
    temp_ptr = SCI_GetBTVersionInfo(); //Get BT version
#else
    temp_ptr = (char*)"host:15.36P11 controller:";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#endif

//WCN V:
#ifdef MMI_WIFI_SUPPORT
    temp_ptr = (char*)"WIFI V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
#ifndef WIN32
    temp_ptr = wcn_cp2_version_get(); //Get wifi version
#else
    temp_ptr = (char*)"WIFI Version:0.0";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
/*
    temp_ptr = (char*)"AP WIFI V:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
    temp_ptr = VERSION_GetInfo(SPRDWIFI_VERSION);
#else
    temp_ptr = (char*)"AP WIFI Version:0.0";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
	*/
#endif

 //Build Time:
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    temp_ptr = (char*)"Build Version:";
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef WIN32
    temp_ptr = VERSION_GetInfo(BUILD_TIME);     /*lint !e605*/
#else
    temp_ptr = (char*)"2010-03-24";
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
    //MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    GUITEXT_SetString(MMIENG_SHOW_VERSION_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, need_update);

    if(PNULL != uint16_str)
    {
        SCI_FREE(uint16_str);
        uint16_str = PNULL;
    }
}



/*****************************************************************************/
//  Description : MMIENGSET_AppendListItemByTextIdExt
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIENGSET_AppendListItemByTextIdExt(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
	BOOLEAN             result = FALSE;
	CTRLLIST_ITEM_T      item = {0};
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

	item.style_ptr = THEMELIST_GetStyle (item_style);
	item.data_ptr = &item_data;
	item.state |= GUIITEM_STATE_DISABLE_MARK;
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[0].item_data.text_id = text_id;

	item_data.softkey_id[0] = left_softkey_id;
	item_data.softkey_id[1] = middle_softkey_id;
	item_data.softkey_id[2] = right_softkey_id;	
	
	result = CTRLLIST_AppendItem (ctrl_id, &item);
    
}

/*Edit by script, ignore 1 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
