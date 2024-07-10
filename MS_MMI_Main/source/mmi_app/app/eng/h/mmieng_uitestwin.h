/****************************************************************************
** File Name:      mmieng_uitestwin.h	                                    *
** Author:          allen				                                    *
** Date:             2004.09.07												*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved. 		*
** Description:    									               			*
*****************************************************************************
**                         Important Edit History                        							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             						*
** 09/2004      allen               Create
** 
****************************************************************************/
#ifndef  _MMIENG_UITESTWIN_H_    
#define  _MMIENG_UITESTWIN_H_   

#include "mmicom_panel.h"
#include "mmk_app.h"
#include "mmidisplay_data.h"
#include "guicommon.h"
#include "mmieng_position.h"
#ifdef __cplusplus
    extern "C"
    {
#endif

//////////////////////////////
// here define the mini test flag
//////////////////////////////

//#define _MINITEST


#define UITEST_COLOR_BLACK		0x20E4
#define UITEST_COLOR_WHITE		0xFFFF
#define UITEST_COLOR_RED		0xF800
#define UITEST_COLOR_GREEN		0x07E0
#define UITEST_COLOR_BLUE		0x001F

#define UI_TEST_KEY_NUM		25//#ifdef SC6800_DVB   #define UI_TEST_KEY_NUM		29

typedef enum{
	MMI_UI_NULL,
	MMI_UI_SHOWVERSION,
	MMI_UI_KEYBOARD,
	MMI_UI_MAIN_LCD,
	MMI_UI_SUB_LCD,
	MMI_UI_HEADSET,
	MMI_UI_FLIP,
	MMI_UI_RTC,
	MMI_UI_VIBERATE,
	MMI_UI_BACKLIGHT,
	MMI_UI_MELODY,
	MMI_UI_LOOPBACK,
	MMI_UI_CAMERA_SET_PARAM,
	MMI_UI_CAMERA,
	MMI_UI_INSERTSD,
	MMI_UI_REMOVESD,
	MMI_UI_TVOUT,
	MMI_UI_TFLASH,
	MMI_UI_COM,
#ifdef FM_SUPPORT
    MMI_UI_FM,
#endif

#ifdef APP_ENG_BLUETOOTH_SUPPORT
	MMI_UI_BLUETOOTH,
#endif
	MMI_UI_END
} MMI_UITEST_STEP;


typedef struct
{
	GUI_FONT_T 		text_font;      
	GUI_COLOR_T 	text_color;    
	GUI_COLOR_T 	bkgd_color;    
	uint8			char_space;
} MMI_UITEST_DISPLAY_T;	

typedef struct
{
    SIGNAL_VARS
    uint32       handle;
    uint32       opcode;
    uint32       status;
    uint32       win_id;
    uint32       msg_id; 
}SENSORHUB_ENGTEST_CB_T;
#ifdef _MINITEST
/********************************************************************************
 NAME:			MMIENG_MiniTestTimerCallback
 DESCRIPTION:	
 PARAM IN:		signal_ptr -- 
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.10.23
********************************************************************************/
void MMIENG_MiniTestTimerCallback(Signal* signal_ptr);
#endif

/********************************************************************************
 NAME:			MMIENG_RunUITestNextStep
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		songbin.zeng
 DATE:			2006.12.01
********************************************************************************/
PUBLIC MMI_RESULT_E UIReadyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E UITestSubLCDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
//@Lin.Lin, 2005-2-2, end

PUBLIC MMI_RESULT_E UITestRtcWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E UITestVibrateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E UITestBacklightWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef  CAMERA_SUPPORT
PUBLIC MMI_RESULT_E UITestCameraWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

PUBLIC MMI_RESULT_E UITestVideoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E UITestHeadsetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E UITestLoopbackWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E UITestMelodyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef FM_SUPPORT
PUBLIC MMI_RESULT_E UITestFmWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#ifdef BLUETOOTH_SUPPORT
PUBLIC MMI_RESULT_E UITestBluetoothWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

PUBLIC MMI_RESULT_E UITestKeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E UITestMainLCDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/* BEGIN: Added by George.Liu 01526, 2010/7/12   PN:WiFi_UITest */
#ifdef MMI_ENG_WIFI_SUPPORT
PUBLIC MMI_RESULT_E UITestWiFiWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifdef UAL_WIFI_SUPPORT
PUBLIC void MMIAPIENG_OpenWifiScanWin(uint8 is_need_button);
#endif
#endif
/* END:   Added by George.Liu 01526, 2010/7/12   PN:WiFi_UITest */

#ifdef CAMERA_SUPPORT
PUBLIC void UITestCameraEnter(void);
PUBLIC void UITestVideoEnter(void);
#endif

/********************************************************************************
 NAME:			UITestSDWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.08
********************************************************************************/
PUBLIC MMI_RESULT_E UITestSDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/********************************************************************************
 NAME:			UITestTVOutWinHandleMsg
 DESCRIPTION:     handle message of TV out test window 
 AUTHOR:              yuehz
 DATE:			2005.12.25
********************************************************************************/
PUBLIC MMI_RESULT_E UITestTVOutWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef APP_ENG_TEST_TFLASH_SUPPORT
/********************************************************************************
 NAME:			UITestTFlashWinHandleMsg
 DESCRIPTION:
 AUTHOR:		guopeng.zhang
 DATE:			2005.12.25
********************************************************************************/
//modified by yuehz 20060410
PUBLIC MMI_RESULT_E UITestTFlashWinHandleMsg(
    MMI_WIN_ID_T  win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM           param
);
#endif

#ifdef APP_ENG_COMTEST_SUPPORT
/********************************************************************************
 NAME:			UITestCOMWinHandleMsg
 DESCRIPTION:     handle message of com  test window 
 AUTHOR:              yuehz
 DATE:			2006.04.18
********************************************************************************/
PUBLIC MMI_RESULT_E UITestCOMWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

PUBLIC MMI_RESULT_E UITestCameraSetParamWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : handle the message of dc window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIENG_ForEngWinMsg(
                                          MMI_WIN_ID_T		win_id, 
                                          MMI_MESSAGE_ID_E	msg_id, 
                                          DPARAM				param	
                                          );

/*****************************************************************************/
// 	Description : handle the message of dc window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_IsDCEngMode(void);

/*****************************************************************************/
//  Description : handle the message of video test
//  Global resource dependence : none
//  Author : zhichao.zhang
//  Note :
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIENG_VideoForEngWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );

/********************************************************************************
 NAME:			UITESTWIN_OpenPhoneTestWin
 DESCRIPTION:	open uitest main menu
 AUTHOR:		ying.xu
 DATE:			2010.8.10
********************************************************************************/
#ifdef ENG_SUPPORT
PUBLIC BOOLEAN UITESTWIN_OpenPhoneTestWin(void);
#endif

#ifdef MEDIA_DEMO_SUPPORT
PUBLIC BOOLEAN UITESTWIN_OpenAudioDemoWin(void);
PUBLIC BOOLEAN UITESTWIN_OpenVideoDemoWin(void);
PUBLIC BOOLEAN UITESTWIN_OpenMediaDemoWin(void);
#endif
/*****************************************************************************/
//  Descripti
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMILOOPBACK_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);
#ifdef __cplusplus
    }
#endif

#endif

