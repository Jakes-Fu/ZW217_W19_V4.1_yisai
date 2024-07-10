#include "mmi_app_mrapp_trc.h"
#ifdef HERO_ENGINE_SUPPORT
/****************************************************************************
** File Name:      mmimrapp_wintab.c                            
** Author:              qgp                                                    
** Date:            2006/05/15
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.       
** Description:    This file is used to create windows of mr app
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     	qgp	         Create
** 
****************************************************************************/
#ifndef _HERO_ENGINE_WINTAB_C_
#define _HERO_ENGINE_WINTAB_C_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "window_parse.h"
#include "Mmi_text.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guimsgbox.h"
#include "guicommon.h"
#include "guitext.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "guiedit.h"
#include "guilistbox.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "tb_dal.h"
#include "mmiacc_menutable.h"
#include "mmipub.h"
#include "mmialarm_internal.h"
#include "mmialarm_export.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
#include "mmiset_export.h"
#include "tb_hal.h"
#include "mmi_image.h"
#include "heroEngineExport.h"
#include "heroEngineInternal.h"
#include "heroEngineId.h"
#include "window_parse.h"
#ifdef M_NEW_VER
#ifdef HERO_PORT_BASE_NILE
#include "freq_cfg.h"
#else
#include "sc6600m_freq.h"
#endif
#else
#include "chip.h"
#endif
#include "mmi_appmsg.h"
#include "mmipb_common.h"
#include "mmipb_interface.h"
#include "Mmipb_export.h"
#include "mmi_common.h"
//#include "im_id.h"
#include "mmistk_app.h"
#include "mmifilearray_export.h"
#include "mmiconnection_export.h"
#include "mmiconnection_text.h"

#ifdef __HERO_CFG_CAMERA_SUPPORT__
#include "dal_dcamera.h"
#include "dcamera_misc.h"
#include "dcamera_common.h"
#endif

#include "version.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                       LOCAL DATA DECLARATION
 **---------------------------------------------------------------------------*/
 
LOCAL uint8		s_hrTimer = 0;
#ifdef HERO_ENGINE_SUPPORT
LOCAL uint8		s_heroTimer = 0;
#endif
//modify 20181114 begin
LOCAL uint8     s_smartTimer = 0;
LOCAL uint8     s_callTimer = 0;
//modify 20181114 end
LOCAL uint8		s_hr_vibra_timer = 0;
LOCAL uint8		s_hr_gprsProtectTimer = 0;
LOCAL uint8		s_hr_prompt_timer = 0;
LOCAL uint8		s_hr_mrPlaySound_timer = 0;//2008-1-2 add for hr_playSound() protect broken off by other player,as sms .
LOCAL uint8		s_hr_gprsRetryTimer = 0;//2008-2-25 add fro mocor
LOCAL uint8		s_hr_AccessUrl_timer = 0;
//2009-7-7 add begin
LOCAL uint8		s_hr_getApnAccount_timer = 0;
LOCAL uint8		s_hr_setApnAccount_timer = 0;
//2009-7-7 add end

/* begin:added by Tommy.yan 20120120 */
LOCAL uint8     s_hr_deleteSMS_timer = 0;
LOCAL uint8		s_hr_getSMSContent_timer = 0;
LOCAL uint8		s_hr_getSMSCapacity_timer = 0;
LOCAL uint8     s_hr_setSMSState_timer = 0;
/* end:added by Tommy.yan 20120120 */

LOCAL uint8     s_hr_getSCNumber_timer = 0;
LOCAL uint8     s_hr_statusIcon_timer  = 0;
LOCAL char*	    s_hr_AccessUrl_url = 0;
LOCAL uint32	s_hr_gsensor_flag  = 0;
LOCAL BOOLEAN	s_hr_vmPaused = FALSE;
LOCAL BOOLEAN	s_hr_keyDown = FALSE;
LOCAL HERO_APP_CPUCLK_MNG_T s_hr_cpuClk_mng;

LOCAL MMI_HERO_APP_DIALOG_PARAMETERS_T *s_hr_dialogWinList = NULL;

LOCAL uint8 s_hr_winTab[HERO_APP_DYN_WIN_ID_MAXNUM]  = {0};
LOCAL uint8 s_hr_ctrlTab[HERO_APP_DYN_WIN_ID_MAXNUM] = {0};


LOCAL const int32 s_hero_KeyTable[] = {
HR_KEY_NONE,    //#define KEY_NONE				0x00				/*!< press none */
HR_KEY_UP,      //#define KEY_UP				0x01				/*!< press [up] */
HR_KEY_DOWN,    //#define KEY_DOWN				0x02				/*!< press [down] */
HR_KEY_LEFT,    //#define KEY_LEFT				0x03				/*!< press [left] */
HR_KEY_RIGHT,   //#define KEY_RIGHT				0x04				/*!< press [right] */
HR_KEY_SEND,    //#define KEY_GREEN				0x05				/*!< press [Green] */
HR_KEY_CAPTURE,//#define KEY_CAMREA				0x06				/*!< press [CAMERA] */
HR_KEY_NONE,    //0x0007 NO define
HR_KEY_1,       //#define KEY_1					0x08				/*!< press [1] */
HR_KEY_2,       //#define KEY_2					0x09				/*!< press [2] */
HR_KEY_3,       //#define KEY_3					0x0a				/*!< press [3] */
HR_KEY_4,       //#define KEY_4					0x0b				/*!< press [4] */
HR_KEY_5,       //#define KEY_5					0x0c				/*!< press [5] */
HR_KEY_6,       //#define KEY_6					0x0d				/*!< press [6] */
HR_KEY_7,       //#define KEY_7					0x0e				/*!< press [7] */
HR_KEY_8,       //#define KEY_8					0x0f				/*!< press [8] */
HR_KEY_9,       //#define KEY_9					0x10				/*!< press [9] */
HR_KEY_STAR,    //#define KEY_STAR				0x11				/*!< press [*] */
HR_KEY_0,       //#define KEY_0					0x12				/*!< press [0] */
HR_KEY_POUND,   //#define KEY_HASH				0x13				/*!< press [#] */
HR_KEY_NONE,    //#define KEY_SPUP              0x14
HR_KEY_NONE,    //#define KEY_SPDW              0x15
HR_KEY_SOFTLEFT,//#define KEY_OK				0x16				/*!< press left softkey(OK) */
HR_KEY_SOFTRIGHT,//#define KEY_CANCEL			0x17				/*!< press right softkey (Cancel) */
HR_KEY_POWER,     //#define KEY_RED				0x18				/*!< press red key */
HR_KEY_VOLUME_UP,//#define KEY_UPSIDEKEY		0x19				/*!< press up side key */
HR_KEY_VOLUME_DOWN,//#define KEY_DOWNSIDEKEY	0x1a				/*!< press down side key */
HR_KEY_SELECT,      //#define KEY_WEB			0x1b				/*!< press web key */

HR_KEY_NONE,  //#define KEY_GPIO_SIG1        	0x1c
HR_KEY_NONE,  //#define KEY_GPIO_SIG2        	0x1d
HR_KEY_NONE,  //#define KEY_FLIP				0x1e				/*!< flip gpio  */
HR_KEY_NONE,  //#define KEY_HEADSET_BUTTIN		0x1f				/*!< handset gpio buttion */
HR_KEY_NONE,  //#define KEY_HEADSET_DETECT		0x20				/*!< handset gpio detect */
HR_KEY_NONE,  //#define KEY_SDCARD_DETECT		0x21				/*!< SD card detct */
HR_KEY_NONE,  //#define KEY_VIDEO_TEL    	    0x22
HR_KEY_NONE,  //#define KEY_PLAYANDSTOP    	    0x23
HR_KEY_NONE,  //#define KEY_FORWARD    		    0x24
HR_KEY_NONE,  //#define KEY_BACKWARD    		0x25
HR_KEY_NONE,  //#define KEY_MIDDLE              0x26

HR_KEY_NONE,     //#define KEY_AT       0x27
HR_KEY_QWERTY_Q, //#define KEY_Q        0x28
HR_KEY_QWERTY_W, //#define KEY_W		0x29  
HR_KEY_QWERTY_E, //#define KEY_E		0x2a  
HR_KEY_QWERTY_R, //#define KEY_R		0x2b  
HR_KEY_QWERTY_T, //#define KEY_T		0x2c  
HR_KEY_QWERTY_Y, //#define KEY_Y		0x2d  
HR_KEY_QWERTY_U, //#define KEY_U		0x2e  
HR_KEY_QWERTY_I, //#define KEY_I		0x2f  
HR_KEY_QWERTY_O, //#define KEY_O		0x30  
HR_KEY_QWERTY_P, //#define KEY_P		0x31  
HR_KEY_QWERTY_A, //#define KEY_A		0x32  
HR_KEY_QWERTY_S, //#define KEY_S		0x33  
HR_KEY_QWERTY_D, //#define KEY_D		0x34  
HR_KEY_QWERTY_F, //#define KEY_F		0x35  
HR_KEY_QWERTY_G, //#define KEY_G		0x36  
HR_KEY_QWERTY_H, //#define KEY_H		0x37  
HR_KEY_QWERTY_J, //#define KEY_J		0x38  
HR_KEY_QWERTY_K, //#define KEY_K		0x39  
HR_KEY_QWERTY_L, //#define KEY_L		0x3a  
HR_KEY_QWERTY_DEL, //#define KEY_DEL		0x3b  
HR_KEY_QWERTY_Z, //#define KEY_Z		0x3c  
HR_KEY_QWERTY_X, //#define KEY_X		0x3d  
HR_KEY_QWERTY_C, //#define KEY_C		0x3e  
HR_KEY_QWERTY_V, //#define KEY_V		0x3f  
HR_KEY_QWERTY_B, //#define KEY_B		0x40  
HR_KEY_QWERTY_N, //#define KEY_N		0x41  
HR_KEY_QWERTY_M, //#define KEY_M		0x42  
HR_KEY_NONE,     //#define KEY_COMMA	0x43
HR_KEY_NONE,     //#define KEY_PERIOD	0x44
HR_KEY_QWERTY_ENTER, //#define KEY_ENTER	 0x45
HR_KEY_QWERTY_FN,    //#define KEY_FN		 0x46
HR_KEY_QWERTY_SHIFT, //#define KEY_SHIFT	 0x47
HR_KEY_NONE,         //#define KEY_AT_QWERTY 0x48
HR_KEY_QWERTY_SPACE, //#define KEY_SPACE	 0x49
HR_KEY_NONE,         //#define KEY_AND		 0x4a
HR_KEY_NONE,         //#define KEY_QUESTION	 0x4b
HR_KEY_QWERTY_CTRL,  //#define KEY_CTRL	     0x4c

HR_KEY_NONE, //#define KEY_PLUS                0x4d  // +
HR_KEY_NONE, //#define KEY_LEFT_PARENTHESIS    0x4e  // (
HR_KEY_NONE, //#define KEY_RIGHT_PARENTHESIS   0x4f  // )
HR_KEY_NONE, //#define KEY_MINUS               0x50  // -
HR_KEY_NONE, //#define KEY_DOUBLE_QUOTES       0x51  // "
HR_KEY_NONE, //#define KEY_SEMICOLON           0x52  // ;
HR_KEY_NONE, //#define KEY_COLON               0x53  // : 
HR_KEY_NONE, //#define KEY_SLASH               0x54  // /
HR_KEY_NONE, //#define KEY_SHIFT_AND           0x55  // &
HR_KEY_NONE, //#define KEY_EXCLAMATION         0x56  // !

HR_KEY_NONE, //#define KEY_SLIDE               0x57  
HR_KEY_NONE, //#define KEY_TV			       0x58  
HR_KEY_NONE, //#define KEY_CALL2			   0x59  
HR_KEY_NONE, //#define KEY_CALL3               0x5a  
HR_KEY_NONE, //#define KEY_MP3                 0x5b  
HR_KEY_NONE, //#define KEY_NOTES               0x5c  
HR_KEY_NONE, //#define KEY_SMS                 0x5d  
HR_KEY_NONE, //#define KEY_CALENDER            0x5e  
HR_KEY_NONE, //#define KEY_IE                  0x5f  
HR_KEY_NONE, //KEY_HANG                        0x60  

HR_KEY_VOLUME_UP,   //#define KEY_VOL_UP       0x61  
HR_KEY_VOLUME_DOWN, //#define KEY_VOL_DOWN     0x62  
HR_KEY_NONE, //#define KEY_MO                  0x63  
HR_KEY_NONE, //#define KEY_HOOK                0x64  
HR_KEY_NONE, //#define KEY_HANDFREE            0x65  
HR_KEY_NONE, //#define KEY_CALL4               0x66  
HR_KEY_NONE, //#define KEY_POWER               0x67  
HR_KEY_SOFTLEFT, //#define KEY_MENU            0x68  

//end at//#define    MSG_KEYLONG              		0x0030              /*!< LAST_KEYUP_MSG_ID + 0x01 */
};


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
BOOLEAN s_hr_inMrSelfWinCreating = FALSE;
BOOLEAN s_hr_PBOfCancelMsg = FALSE;
BOOLEAN s_hr_isWindowOfPB = FALSE;
BOOLEAN s_hr_isReadBySMS = FALSE;
uint8 s_hr_fbhup2idle = 0;
BOOLEAN	s_hr_allReady = FALSE;//mmi res ok
BOOLEAN	s_hr_VMReady = FALSE;//vm res ok
#ifdef HR_BINDTO_VIRTUAL_WIN
uint32 g_hr_vmBackRun = HERO_APP_MAX;
#endif
uint8 g_hr_filebrowser_path[256] = {0};

LOCAL uint8     s_hr_phb_operate_timer = 0;
LOCAL uint8     s_hr_phb_search_timer=0;
PUBLIC void MMIHEROAPP_StopPHBOperateTimer(void);
void MMIHEROAPP_PHBSearchCallBack();

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern hr_screen_angle_t g_hr_screen_angle;
extern MMI_APPLICATION_T g_hr_main_app;
extern HERO_APP_PBINFO_T* g_hr_pbinfo;
extern uint8 g_hr_curUserApnAccuntId; /*lint !e752*/

extern uint32 MMI_SetFreq(PROD_FREQ_INDEX_E freq); /*lint !e762*/
extern uint32 MMI_RestoreFreq(void); /*lint !e762*/

void MMIHEROAPP_AccessUrl(void);

/*wuwenjie 2011-12-01 START*/
extern uint8 hr_getHostTimerId(void);
extern void hr_gethostTimerDeal(void);
/*wuwenjie 2011-12-01 END*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
// 	Description : init local resource
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL  void MMIHEROAPP_initLocalRes(void);

/*****************************************************************************/
// 	Description : get pb info back
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL void MMIHEROAPP_pbInfoReturn(MMIPB_HANDLE_T handle);


/*****************************************************************************/
// 	Description : process the key event
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ProcessHeroAppKeyMsg(
									  MMI_HERO_APP_KEYTYPE_E kType,
									  void *info
									  );

/*****************************************************************************/
// 	Description :动态窗口消息处理函数
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIHEROAPP_HandleDynWinMsg(
											 MMI_HERO_APP_WINDOW_ID_E win_id, 
											 MMI_MESSAGE_ID_E msg_id, 
											 DPARAM param
											 );

/*****************************************************************************/
// 	Description : 释放窗体相关的用户数据
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL void MMIHEROAPP_freeDynWinData(MMI_HERO_APP_ADD_PARAMETERS_T* addData);

/*****************************************************************************/
// 	Description : 获取一空闲的 ID,或释放一id
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL int32 MMIHEROAPP_OpDynId(MMI_HERO_APP_DYN_WIN_TYPE_E winType, int32 opId, MMI_HERO_APP_ID_TYPE_E idType);

/*****************************************************************************/
// 	Description : 初始化窗体及控件ID
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL void MMIHEROAPP_initDynIdTable(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIHEROAPP_HandlePromptWinMsg(
												MMI_WIN_ID_T win_id, 
												MMI_MESSAGE_ID_E msg_id, 
												DPARAM param
												);

/*****************************************************************************/
// 	Description : the process message function of the mr app root win
//	Global resource dependence : 
//  Author:        qgp
//	Note:   
/*****************************************************************************/
LOCAL void HeroEditSelectPBContactCallback(
							MMIPB_HANDLE_T handle
							);

/*****************************************************************************/
// 	Description : the process message function of the mr app root win
//	Global resource dependence : 
//  Author:        qgp
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHeroAppBaseWinMsg( 
										 MMI_WIN_ID_T win_id, 
										 MMI_MESSAGE_ID_E msg_id, 
										 DPARAM param
										 );

/*****************************************************************************/
// 	Description : the message handler function of select data account win
//	Global resource dependence : 
//  Author: zack
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHeroAppAccountWinMsg( 
											MMI_WIN_ID_T win_id, 
											MMI_MESSAGE_ID_E msg_id, 
											DPARAM param
											);

#ifdef __HERO_CFG_CAMERA_SUPPORT__
/*****************************************************************************/
// 	Description : the message handler function of camera win
//	Global resource dependence : 
//  Author: zack
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHeroCameraWinMsg( 
											MMI_WIN_ID_T win_id, 
											MMI_MESSAGE_ID_E msg_id, 
											DPARAM param
											);
#endif

LOCAL MMI_RESULT_E HandleHeroShowFactoryInfoWinMsg( 
											MMI_WIN_ID_T win_id, 
											MMI_MESSAGE_ID_E msg_id, 
											DPARAM param
											);
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

//this window is the base win (a manager win) for mr app entity
WINDOW_TABLE( MMIHEROAPP_BASE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ), 
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    WIN_FUNC((uint32) HandleHeroAppBaseWinMsg),    
    WIN_ID(MMI_HERO_APP_BASE_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

//this window is select data account win for user
WINDOW_TABLE( MMIHEROAPP_ACCOUNT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ), 
    WIN_TITLE(TXT_COMMON_WWW_ACCOUNT),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, HERO_APP_ACCOUNT_LISTCTRL_ID),
    WIN_FUNC((uint32) HandleHeroAppAccountWinMsg), 
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    WIN_ID(MMI_HERO_APP_ACCOUNT_WIN_ID),
    END_WIN
};

#ifdef __HERO_CFG_CAMERA_SUPPORT__
WINDOW_TABLE( MMIHEROAPP_CAMERA_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ), 
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    WIN_FUNC((uint32) HandleHeroCameraWinMsg),    
    WIN_ID(MMI_HERO_APP_CAMERA_WIN_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMIHEROAPP_SHOWFACTORYINFO_TEXT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleHeroShowFactoryInfoWinMsg),
    WIN_ID(HERO_APP_SHOWFACTORYINFO_TEXT_WIN_ID),
    //#if !defined(PLATFORM_UIX8910)	
    //WIN_TITLE(TXT_HERO_SHOW_FACTORY_INFO),
    //#endif
    CREATE_TEXT_CTRL(HERO_APP_SHOWFACTORYINFO_TEXT_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**---------------------------------------------------------------------------*
 **                       function bodies 
 **---------------------------------------------------------------------------*/
#if defined(WIN32)
int32 hr_event(int16 type, int32 param1, int32 param2)
{
	return 0;
}

int32 hr_stop(void)
{
	return 0;
}
#endif
 
/*****************************************************************************/
//	   Description : to handle the message of prompt alert window
//	  Global resource dependence : 
//	Author:qgp
//	  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E	MMIHEROAPP_HandleQueryWin1Msg(
												MMI_WIN_ID_T     win_id, 
												MMI_MESSAGE_ID_E msg_id, 
												DPARAM param
												)
{
	BOOLEAN recode = MMI_RESULT_TRUE;

	switch(msg_id)
	{
	case MSG_APP_OK:
    case MSG_APP_WEB:
		MMK_CloseWin(win_id);
		MMIHEROAPP_exitMrApp(FALSE);

        MMIHEROAPP_StopStatusIconTimer();
		#if !defined(PLATFORM_UIX8910)	
        MAIN_SetIdleHeroappState(FALSE); /*lint !e718 !e18*/
		#endif

		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_TIMER://pass timer
		break;
		
	default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	
	return recode;
}

/*****************************************************************************/
// 	Description : the mr app entity entry
//	Global resource dependence : 
//  Author: qgp
//	Note: param: prototype is "%xxx.mrp" or NULL
/*****************************************************************************/
#ifdef HERO_USE_UDISK_FIRST
char  g_hero_appparam[64];
#endif
BOOLEAN MMIHEROAPP_HrAppRun(void* param, uint32 appId)
{	
	BOOLEAN ret = FALSE;
	MMIHR_Trace(("enter MMIHEROAPP_HrAppRun entryParam=%s, appId=%d, g_hr_curApp=%d, g_hr_vmBackRun=%d", (char*)param, appId, g_hr_curApp, g_hr_vmBackRun));
#ifdef HERO_USE_UDISK_FIRST
	SCI_MEMSET(g_hero_appparam,0,sizeof(g_hero_appparam));
	SCI_MEMCPY(g_hero_appparam,(char*)param,strlen((char*)param))
#endif

	if(MMK_IsOpenWin(MMI_HERO_APP_BASE_WIN_ID))
	{
		if(!MMK_IsFocusWin(MMI_HERO_APP_BASE_WIN_ID))
		{
			MMK_WinGrabFocus(MMI_HERO_APP_BASE_WIN_ID);//获得FOCUS
		}
		MMIHR_Trace(("MMIHEROAPP_HrAppRun MMI_HERO_APP_BASE_WIN_ID is opend!"));
	}
	else
	{
#ifdef HR_BINDTO_VIRTUAL_WIN
		if(g_hr_vmBackRun != HERO_APP_MAX)
		{
			if(appId == g_hr_vmBackRun)
			{
				if(MMK_CreateWin((uint32*)MMIHEROAPP_BASE_WIN_TAB, (ADD_DATA)param))
				{
					//MMIHEROAPP_pauseMp3();
					hr_event(HR_LOCALUI_EVENT, HR_LOCALUI_APPRESUME, 0);//send back2front event to app
					ret = TRUE;
				}
			}
		#ifdef HERO_ENGINE_FAMILY_CONTROL_SUPPORT
			else if(g_hr_vmBackRun == HR_APPID_FAMILY_BGRUN || g_hr_vmBackRun == HR_APPID_FAMILY)
			{
				MMIHEROAPP_exitMrApp(TRUE);
		        MMIHEROAPP_StopStatusIconTimer();
				#if !defined(PLATFORM_UIX8910)	
		        MAIN_SetIdleHeroappState(FALSE);
                #endif
		        MMIHEROAPP_setCurApp(appId);
				MMIHEROAPP_pauseMp3();
				ret = MMK_CreateWin((uint32*)MMIHEROAPP_BASE_WIN_TAB, (ADD_DATA)param);
			}
		#endif
			else
			{
				MMI_STRING_T string = {0};
				#ifdef __HERO_ENGINE_OVERSEA__
                		uint8   temp_buf1[64] = {0};
                		uint16  temp_buf2[64] = {0};
                		sprintf((char*)temp_buf1, "%s", "Exit background app?");
                		string.wstr_len = MMIHEROAPP_GB2UCS(temp_buf2, temp_buf1, strlen(temp_buf1));
                		string.wstr_ptr = temp_buf2;
                #else
				//提示:请先退出后台应用
				string.wstr_len = sizeof("\xF7\x8B\x48\x51\x00\x90\xFA\x51\x0E\x54\xF0\x53\x94\x5E\x28\x75\x00\x00")/2-1;
				string.wstr_ptr = (uint8*)"\xF7\x8B\x48\x51\x00\x90\xFA\x51\x0E\x54\xF0\x53\x94\x5E\x28\x75\x00\x00"; /*lint !e64*/
				#endif
				MMIPUB_OpenAlertWinByTextPtr(NULL,&string,NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,MMIHEROAPP_HandleQueryWin1Msg);
			}
		}
		else
		{
			MMIHEROAPP_setCurApp(appId);
			MMIHEROAPP_pauseMp3();
			ret = MMK_CreateWin((uint32*)MMIHEROAPP_BASE_WIN_TAB, (ADD_DATA)param);
		}
#else
		MMIHEROAPP_setCurApp(appId);
		MMIHEROAPP_pauseMp3();
		ret = MMK_CreateWin((uint32*)MMIHEROAPP_BASE_WIN_TAB, (ADD_DATA)param);
#endif
	}

	MMIHR_Trace(("exit MMIHEROAPP_HrAppRun entryParam=%s, appId=%d, g_hr_curApp=%d, g_hr_vmBackRun=%d", (char*)param, appId, g_hr_curApp, g_hr_vmBackRun));
	return ret;	
}

BOOLEAN MMIHEROAPP_StartApp(uint8* param)
{
	int32 i = 0;
    BOOLEAN ret = 0;
    uint8 entry[64] = {0};

	//SCI_TRACE_LOW:"mrapp MMIHEROAPP_StartApp param = %s"
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMRAPP_WINTAB_479_112_2_18_3_12_53_7,(uint8*)"s", param);

    /*if(*param == '%')
    {
        while((*param != ',') && (*param != '\0'))
        {
			entry[i] = *param;
            param++;
			i++;
        }
		entry[i] = '\0';

		if (*param != '\0')
		{
			param++;
		}
		else
		{
			param = NULL;
		}
        
		//SCI_TRACE_LOW:"mrapp MMIHEROAPP_StartApp entry = %s param = %s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMRAPP_WINTAB_500_112_2_18_3_12_53_8,(uint8*)"ss", entry, param);

		ret = hr_start_app_ex((char*)entry, (char*)param);
    }
    else
    {*/
    	#if !defined(WIN32)
        ret = hr_mainRom((char*)param);
		#endif
    //}

    return ret;
}

/*****************************************************************************/
// 	Description : the process message function of the mr app root win
//	Global resource dependence : 
//  Author:        qgp
//	Note:   
/*****************************************************************************/
PUBLIC void MMIHEROAPP_SelectPBContactCallback(
										MMIPB_HANDLE_T handle
										)
{
	MMIHR_Trace(("enter MMIHEROAPP_SelectPBContactCallback() handle = 0x%x", handle));
	MMK_SendMsg(MMI_HERO_APP_BASE_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);
}

void MMIHEROAPP_ClearScreen(void)
{
	uint16       str_width  = 0;
	uint16       str_height = 0;
	uint16       lcd_width  = 0;
	uint16       lcd_height = 0;
    GUI_RECT_T   rect  = {0};
	GUI_POINT_T  point = {0};
	MMI_STRING_T text  = {0};
   	GUISTR_STYLE_T text_style = {0};
	GUISTR_STATE_T text_state = GUISTR_STATE_ANGLE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	
	MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &text);
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    rect.top  = 0;
	rect.left = 0;
	rect.right  = lcd_width;
	rect.bottom = lcd_height;
	LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

	str_width  = GUI_GetStringWidth(MMI_DEFAULT_TEXT_FONT, text.wstr_ptr, text.wstr_len);
	str_height = GUI_GetStringHeight(MMI_DEFAULT_TEXT_FONT, text.wstr_ptr, text.wstr_len);

	point.x = (lcd_width-str_width)/2;
	point.y = (lcd_height-str_height)/2;

    text_style.effect = FONT_EFFECT_REVERSE_EDGE; 
    text_style.angle = MMK_GetScreenAngle();
    text_style.align = ALIGN_LEFT;
    text_style.font  = MMI_DEFAULT_TEXT_FONT;
    text_style.font_color = MMI_WHITE_COLOR;
    
	GUISTR_DrawTextToLCDSingleLine(&lcd_dev_info, &point, &text, &text_style, text_state); 
}

/*****************************************************************************/
// 	Description : the process message function of the mr app root win
//	Global resource dependence : 
//   Author:        qgp
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHeroAppBaseWinMsg( 
										 MMI_WIN_ID_T win_id, 
										 MMI_MESSAGE_ID_E msg_id, 
										 DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	MMIHR_Trace(("enter HandleHeroAppBaseWinMsg() msg_id = 0x%x, param = 0x%x", msg_id, param));
    
	switch(msg_id)
	{	    
	case MSG_OPEN_WINDOW:
		{
			uint32 timout = 1000;
			s_hr_keyDown = FALSE;
			s_hr_vmPaused = FALSE;
            s_hr_gsensor_flag = MMISET_GetIsSensorOpenFlag();
            MMISET_SetIsSensorOpenFlag(0);
#ifdef HR_BINDTO_VIRTUAL_WIN
			if(g_hr_vmBackRun != HERO_APP_MAX)
			{
				g_hr_vmBackRun = HERO_APP_MAX;
				break;
			}
#endif	
			MMIHEROAPP_initLocalRes();
			MMIHEROAPP_initDynIdTable();
			MMIHEROAPP_initAllExtraSrc();
			MMIHEROAPP_freeAllSnode();
			s_hr_allReady = TRUE;

#ifdef HERO_ENGINE_SUPPORT_CLKADAPT
			MMIHR_Trace(("#######HAL_GetMCUClk: %d", HAL_GetMCUClk()));
			s_hr_cpuClk_mng.orgCpuClk = HAL_GetMCUClk();
			MMIHEROAPP_CLKSetted(TRUE, FREQ_INDEX_APP_BASE, s_hr_cpuClk_mng.orgCpuClk);
			MMIHEROAPP_SetArmClkHigh();
#endif
            //MMIPUB_OpenAlertWinByTextId(&timout, TXT_COMMON_WAITING,
            //    TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
			s_hr_prompt_timer = MMK_CreateWinTimer(MMI_HERO_APP_BASE_WIN_ID, 50, FALSE);//delay to start dsm app, for the wait win to show 

	#if defined(MAINLCD_SUPPORT_272X480)
			MMIDEFAULT_AllowTurnOffBackLight(FALSE);
	#endif
			break;
		}
	case MSG_CLOSE_WINDOW:
		MMIHR_Trace(("#MSG_CLOSE_WINDOW"));
#ifdef HR_BINDTO_VIRTUAL_WIN
		if(g_hr_vmBackRun != HERO_APP_MAX)
		{
			break;
		}
#endif

		if(s_hr_VMReady)
		{
			s_hr_VMReady = FALSE;
			hr_stop();
		}	

		MMIHEROAPP_freeMmiRes();

	#if defined(MAINLCD_SUPPORT_272X480)
			MMIDEFAULT_AllowTurnOffBackLight(TRUE);
	#endif
		break;
			
	case MSG_TIMER:	
		if(s_hr_prompt_timer == *(uint8*)param)
		{

			char* addData = (char*)MMK_GetWinAddDataPtr(win_id);
			int32 ret = HR_SUCCESS;
			
			MMK_StopTimer(s_hr_prompt_timer);
			s_hr_prompt_timer = 0;	
			s_hr_VMReady = TRUE;
            MMIHR_Trace(("DSM entry = %s", addData));
#ifndef WIN32
            ret = (int32)MMIHEROAPP_StartApp((const char*)addData); /*lint !e64*/
#endif

#ifdef HERO_ENGINE_SUPPORT_CLKADAPT
			if(!s_hr_cpuClk_mng.setted)
				MMIHEROAPP_RestoreARMClk();
#endif
			if(ret != HR_SUCCESS)
			{
				s_hr_VMReady = FALSE;
				MMK_CloseWin(MMI_HERO_APP_BASE_WIN_ID);
			}

			MMIHR_Trace(("hero. DSM start param = %s, ret = %d, vm_ready = %d", addData, ret, s_hr_VMReady));
		}	
		break;
		
	case MSG_LOSE_FOCUS:
		MMIHR_Trace(("ROOT. MSG_LOSE_FOCUS"));
		MMIHEROAPP_pauseApp();
		break;

	case MSG_GET_FOCUS:
		MMIHR_Trace(("ROOT. MSG_GET_FOCUS"));
		MMIHEROAPP_resumeApp();
        /*add by zack@20110921 start*/
        if(g_hr_pbinfo != NULL)
        {
			if(g_hr_pbinfo->needInform == TRUE)
			{
            	g_hr_pbinfo->needInform = FALSE;
            	hr_event(HR_LOCALUI_EVENT, HR_LOCALUI_OK, 0);
			}

			if(s_hr_PBOfCancelMsg)
			{
				s_hr_PBOfCancelMsg = FALSE;
            	hr_event(HR_LOCALUI_EVENT, HR_LOCALUI_CANCEL, 0);
			}
        }
        /*add by zack@20110921 end*/
		break;

	case HERO_APP_MSG_NETINIT_RESULT:
		if(param != NULL)
		{
			MMIHEROAPP_netInitResult(((MMI_HERO_APP_NETINIT_RESULT_T*)param)->result);
			MMIHR_Trace(("hero. net init result return."));
		}
		break;
        
	case HERO_APP_MSG_NETREDAIL_IND:
		MMIHEROAPP_redail();
		break;

	case HERO_APP_MSG_SYS_EXIT:
#if defined(CUSTOMER_PRJ_NAME_F3006_CB)
	case MSG_KEYDOWN_CANCEL:
#endif	
		MMIHEROAPP_closeAllWins();	
		break;

	case HERO_APP_MSG_ASY_CLOSE_MENU:
		if(param != NULL)
		{
			int menuId = *((int*)param);
			if(!s_hr_allReady)
			{
				MMIHR_TraceT(("HERO_APP_MSG_ASY_CLOSE_MENU not ready !!!"));
				break;
			}
			if(MMK_IsOpenWin((MMI_WIN_ID_T)menuId))
			{
				if(MMK_CloseWin((MMI_WIN_ID_T)menuId))
				{
					recode = MMI_RESULT_TRUE;
				}
			}
		}
		break;

/* begin:modified by Tommy.yan 20120222 */
	case MMI_SELECT_CONTACT_CNF: 
		MMIHEROAPP_pbInfoReturn((MMIPB_HANDLE_T)param);
		break;
/* end:modified by Tommy.yan 20120222 */

#ifdef HERO_HANDSET_IS_SUPPORT_TOUCHPANEL
	case MSG_TP_PRESS_DOWN:
		recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_DOWN, (void*)param);
		break;
	case MSG_TP_PRESS_UP:
		recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_UP, (void*)param);
		break;
//2010-2-24 add move & update HERO_PORT_VER to 5
	case MSG_TP_PRESS_MOVE:
		recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_MOVE, (void*)param);
		break;
//2010-2-24 add end
#endif
		
	case MSG_KEYPRESSUP_RED:
	case MSG_KEYDOWN_RED:
	case MSG_KEYUP_RED://2007add
    	MMIHR_Trace(("hero HandleHeroAppBaseWinMsg() msg_id = 0x%x, s_hr_fbhup2idle = %d", msg_id, s_hr_fbhup2idle));
		if(s_hr_fbhup2idle != 1)	
        {      
			recode = MMI_RESULT_FALSE;
		#if 0//defined(__HR_SUPPORT_BACKSTAGE__)
			if (MMIHEROAPP_GetCurApp() == HERO_APP_XXX && MMIHEROAPP_GetVMState() == VM_RUN)
			{
			         recode = hr_event(HR_KEY_RELEASE, HR_KEY_POWER, 0);
			}
		#endif
        }
        else
        {
            recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_KEY_NORMAL, (void*)&msg_id);
        }
		break;

	default:
		if(s_hr_VMReady)
		{
			recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_KEY_NORMAL, (void*)&msg_id);
		}
		break;
	}

	MMIHR_Trace(("exit HandleHeroAppBaseWinMsg() msg_id = 0x%x, recode = %d", msg_id, recode));
	return recode;
}

/*****************************************************************************/
// 	Description : 初始化窗体及控件ID
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL void MMIHEROAPP_initDynIdTable(void)
{
	MMIHR_Trace(("hero. MMIHEROAPP_initDynIdTable()"));
	SCI_MEMSET(s_hr_winTab, 0, sizeof(s_hr_winTab));
	SCI_MEMSET(s_hr_ctrlTab, 0, sizeof(s_hr_ctrlTab));
}

/*****************************************************************************/
// 	Description : 获取一空闲的 ID,或释放一id
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL int32 MMIHEROAPP_OpDynId(MMI_HERO_APP_DYN_WIN_TYPE_E winType, int32 opId, MMI_HERO_APP_ID_TYPE_E idType)
{
	int32 id = -1;
	int32 i = 0;
	uint8 *tablePtr = NULL;
	int32 baseId = 0;
	uint8 tableSize = 0;

	MMIHR_Trace(("hero. MMIHEROAPP_OpDynId() optype:%d, idType: %d", opId, idType));

	if(opId == 0)//this indi that: not free ,not get too
	{
		return id;
	}
	
	if(winType < HERO_APP_WINTYPE_MAX && winType >= 0) /*lint !e568*/
	{
		if(idType == HERO_APP_ID_WIN)
		{
			tablePtr = (uint8*)s_hr_winTab;
			tableSize = sizeof(s_hr_winTab);
			baseId = HERO_APP_DYN_BASE_WIN_ID;
		}
		else if(idType == HERO_APP_ID_CTRL)
		{
			tablePtr = (uint8*)s_hr_ctrlTab;
			tableSize = sizeof(s_hr_ctrlTab);
			baseId = HERO_APP_DYN_BASE_CTRL_ID;
		}
	}

	if(tablePtr != NULL)
	{
		if(opId == -1)//get a new idle win id
		{
			for(i = 0; i < tableSize; i++)
			{
				if(tablePtr[i] == 0)
				{
					id = baseId + i;
					tablePtr[i] = 1;
					if((idType != HERO_APP_ID_WIN) || !MMK_IsOpenWin(id))
					{
						break;
					}
					else
					{
						id = -1;
					}
				}
			}
		}
		else//free the spec id to idle
		{
			i = opId - baseId;
			if(i >= 0 && i < tableSize)
			{
				tablePtr[i] = 0;
			}
		}
	}
	
	MMIHR_Trace(("hero. get id :%d", id));

	return id;
}

/*****************************************************************************/
// 	Description : 释放窗体相关的用户数据
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL void MMIHEROAPP_freeDynWinData(MMI_HERO_APP_ADD_PARAMETERS_T* addData)
{
	MMIHR_Trace(("hero. MMIHEROAPP_freeDynWinData()"));
	
	if(addData != NULL)
	{
		uint8 *titlePtr = NULL;
		uint8 *textPtr  = NULL;
		HERO_APP_MENU_DATA *menuPtr = NULL;
		HERO_APP_LIST_ITEM_T *delElem, *elemPtr;
		
		/*free dynamic id*/
		MMIHEROAPP_OpDynId(addData->dynWinType, addData->winId, HERO_APP_ID_WIN);
		if(addData->dynWinType != HERO_APP_WINTYPE_APP)
		{
			MMIHEROAPP_OpDynId(addData->dynWinType, addData->ctrlId, HERO_APP_ID_CTRL);
		}
		
		/*free user additional data*/
		switch(addData->dynWinType)
		{
		case HERO_APP_WINTYPE_DIALOG:
			break;
			
		case HERO_APP_WINTYPE_TEXT:
			titlePtr = (uint8*)addData->u.text.title.wstr_ptr; 
            textPtr  = (uint8*)addData->u.text.textInfo.wstr_ptr;//add by zack@20110916
			break;
			
		case HERO_APP_WINTYPE_EDITOR:
			titlePtr = (uint8*)addData->u.edit.title.wstr_ptr;
			if(addData->u.edit.textOut.wstr_ptr!= NULL)
			{
				SCI_FREE(addData->u.edit.textOut.wstr_ptr);
			}
			break;
			
		case HERO_APP_WINTYPE_MENU:
			menuPtr = &addData->u.menu;
			titlePtr = (uint8*)menuPtr->title.wstr_ptr;
			
			if(menuPtr->listElem != NULL)
			{
				delElem = menuPtr->listElem->nextItem;
				while(delElem != NULL)
				{
					elemPtr = delElem->nextItem;
					SCI_FREE(delElem);
					delElem = elemPtr;
				}
			
				SCI_FREE(menuPtr->listElem);
			}
			break;
			
		case HERO_APP_WINTYPE_APP:
			break;
			
		default:
			MMIHR_Trace(("error para: %d", addData->dynWinType));
			break;
		}
		
		if(titlePtr != NULL)
		{
			SCI_FREE(titlePtr);
		}
        /*add by zack@20110916 start*/
    	if(textPtr != NULL)
		{
			SCI_FREE(textPtr);
		}
        /*add by zack@20110916 end*/
		SCI_FREE(addData);
	}
}

/*****************************************************************************/
//  Description : creat softkey control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MMIHEROAPP_CreatSoftkeyCtrl(
									 MMI_HANDLE_T win_handle,     //win id
									 MMI_TEXT_ID_T leftsoft_id,
									 MMI_TEXT_ID_T midsoft_id,
									 MMI_TEXT_ID_T rightsoft_id
									 )
{
    GUISOFTKEY_INIT_DATA_T	softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T	softkey_ctrl_id = 0;

    softkey_data.leftsoft_id = leftsoft_id;
    softkey_data.midsoft_id	= midsoft_id;
    softkey_data.rightsoft_id = rightsoft_id;

    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr = &softkey_data;

#ifdef MMI_PDA_SUPPORT
	if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
	{
		MMK_SetWinDisplayStyleState(win_handle, WS_HAS_BUTTON_SOFTKEY, TRUE);
	}
#endif
    MMK_CreateControl( &softkey_create );
    MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
}

/*****************************************************************************/
// 	Description :根据动态ID 创建一需求窗体
//	Global resource dependence : none
//  Author: qgp
//	Note:  the return value: -2 the caller need to free the addData; -1 the caller need not
/*****************************************************************************/
int32 MMIHEROAPP_createADynWin(MMI_HERO_APP_ADD_PARAMETERS_T *addData, MMI_STRING_T *textInfo)
{
	int32		winId = -1;
	int32		ctrlId = -1;
	MMI_HERO_APP_DYN_WIN_TYPE_E winType;
	BOOLEAN		result = FALSE;
	MMI_WINDOW_CREATE_T win_create = {0};
	MMI_CONTROL_CREATE_T create = {0};
	MMI_HANDLE_T win_handle;
	GUI_BORDER_T    edit_border = {0}; //add by zack@20120809
	MMIHR_Trace(("hero. MMIHEROAPP_createADynWin()"));
	
	if(addData == NULL)
	{
		return -2;
	}
	
	addData->winId = 0;
	addData->ctrlId = 0;
	winType = addData->dynWinType;
	winId = MMIHEROAPP_OpDynId(winType, -1, HERO_APP_ID_WIN);//get id
	if(winId == -1)
	{
		return -2;
	}	
	addData->winId = winId;
	/*modify by zack@20120809 start*/
#ifdef MMI_RES_ORIENT_BOTH
	MMK_SetScreenAngle(LCD_ANGLE_0);
#endif
    /*modify by zack@20120809 end*/
	win_create.applet_handle = MMK_GetFirstAppletHandle();
	win_create.win_id = addData->winId;
	win_create.func = MMIHEROAPP_HandleDynWinMsg;
	win_create.win_priority = WIN_ONE_LEVEL;
	win_create.add_data_ptr = (ADD_DATA)addData;
	win_create.window_move_style = MOVE_FORBIDDEN;
#ifdef PDA_UI_ANDROID_SOFTKEY
    if(winType == HERO_APP_WINTYPE_TEXT || winType == HERO_APP_WINTYPE_EDITOR || winType == HERO_APP_WINTYPE_DIALOG)
    {
        win_create.window_style = WS_HAS_SOFTKEY;
    }
#endif
	s_hr_inMrSelfWinCreating = TRUE;
	win_handle = MMK_CreateWindow(&win_create);
    MMK_SetWinSupportAngle(win_handle, WIN_SUPPORT_ANGLE_CUR);
	s_hr_inMrSelfWinCreating = FALSE;
	
	MMIHR_Trace(("hero. MMK_CreateWindow() result: 0x%x", win_handle));	
	if(win_handle != 0)
	{
		result = FALSE;
		if(winType != HERO_APP_WINTYPE_APP)
		{
			ctrlId = MMIHEROAPP_OpDynId(winType, -1, HERO_APP_ID_CTRL);//get id
			if(ctrlId == -1)
			{
				MMK_CloseWin(addData->winId);
				return -1;
			}
			addData->ctrlId = ctrlId;
		}
		
		switch(winType)
		{
		case HERO_APP_WINTYPE_TEXT:
			{
				GUITEXT_INIT_DATA_T init_data = {0};
				MMI_TEXT_ID_T 	left_sk_id = TXT_NULL;
				MMI_TEXT_ID_T 	right_sk_id = TXT_NULL;
				if(addData->u.text.type == HR_DIALOG_OK)
				{
					left_sk_id = TXT_COMMON_OK;
				}
				else if(addData->u.text.type == HR_DIALOG_OK_CANCEL)
				{
					left_sk_id = TXT_COMMON_OK;
					right_sk_id = STXT_CANCEL;
				}
				else if(addData->u.text.type == HR_DIALOG_CANCEL_OK)
				{
					left_sk_id = STXT_CANCEL;
					right_sk_id = TXT_COMMON_OK;
				}
				else
				{
					right_sk_id = STXT_RETURN;
				}
                GUIWIN_CreateTitleDyna(addData->winId, TXT_NULL);
                MMIHEROAPP_CreatSoftkeyCtrl(addData->winId, left_sk_id, TXT_NULL, right_sk_id);
                init_data.both_rect = MMITHEME_GetWinClientBothRect(addData->winId);
				create.ctrl_id = addData->ctrlId;
				create.guid    = SPRD_GUI_TEXTBOX_ID;
				create.parent_win_handle = addData->winId;
				create.init_data_ptr = (void*)&init_data; /*lint !e733*/
				MMK_CreateControl(&create); 
				result = TRUE;
			}
			break;
		case HERO_APP_WINTYPE_EDITOR:
			{
				HERO_APP_EDIT_DATA *editPtr = &addData->u.edit;
				GUIEDIT_INIT_DATA_T init_data = {0};
                BOOLEAN is_disp_im_icon = TRUE;
                BOOLEAN is_disp_num_info = TRUE;
                //init_data.both_rect = MMITHEME_GetWinClientBothRect(addData->winId);
                //init_data.both_rect.v_rect.top = init_data.both_rect.v_rect.top + MMI_TITLE_THEME_HEIGHT;
                //init_data.both_rect.h_rect.top = init_data.both_rect.h_rect.top + MMI_TITLE_THEME_HEIGHT_H;
				editPtr->maxLen = (editPtr->maxLen > MMI_HERO_APP_DEFAULT_MAX_LEN) ? MMI_HERO_APP_DEFAULT_MAX_LEN : editPtr->maxLen;//temp.
				switch(editPtr->type)
				{
				default:
				case HR_EDIT_ANY:
                    GUIWIN_CreateTitleDyna(addData->winId, TXT_NULL);
                    MMIHEROAPP_CreatSoftkeyCtrl(addData->winId, STXT_OK, TXT_NULL, STXT_RETURN);

                    init_data.both_rect = MMITHEME_GetWinClientBothRect(addData->winId);
						init_data.type = GUIEDIT_TYPE_TEXT;
						create.ctrl_id = addData->ctrlId;
						create.guid = SPRD_GUI_EDITBOX_ID;
						create.parent_win_handle = addData->winId; 
						create.init_data_ptr = (void*)&init_data; /*lint !e733*/
						MMK_CreateControl( &create );
                    GUIEDIT_SetIm(addData->ctrlId,GUIIM_TYPE_SET_ALL,GUIIM_TYPE_SMART);
                    GUIEDIT_SetStyle(addData->ctrlId, GUIEDIT_STYLE_MULTI);//多行
                    GUIEDIT_SetTextMaxLen(addData->ctrlId,editPtr->maxLen,editPtr->maxLen);
                    GUIEDIT_SetString(addData->ctrlId, textInfo->wstr_ptr, textInfo->wstr_len);    
                    GUIEDIT_SetDispImIcon(addData->ctrlId,&is_disp_im_icon,&is_disp_num_info);

                    //init_data.add_data.text_add.cur_inputmethod = IM_SMART_MODE;
					break;
				case HR_EDIT_NUMERIC:
                    GUIWIN_CreateTitleDyna(addData->winId, TXT_NULL);
                    MMIHEROAPP_CreatSoftkeyCtrl(addData->winId, STXT_OK, TXT_NULL, STXT_RETURN);

                    init_data.both_rect = MMITHEME_GetWinClientBothRect(addData->winId);

                    init_data.type = GUIEDIT_TYPE_PHONENUM;
                    init_data.str_max_len = editPtr->maxLen;
                    create.ctrl_id = addData->ctrlId;
                    create.guid = SPRD_GUI_EDITBOX_ID;
                    create.parent_win_handle = addData->winId; 
                    create.init_data_ptr = &init_data;
                    MMK_CreateControl( &create );
                    GUIEDIT_SetString(addData->ctrlId, textInfo->wstr_ptr, textInfo->wstr_len);
                    GUIEDIT_SetIm(addData->ctrlId,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
					break;
				case HR_EDIT_PASSWORD:
                    GUIWIN_CreateTitleDyna(addData->winId, TXT_NULL);
                    MMIHEROAPP_CreatSoftkeyCtrl(addData->winId, STXT_OK, TXT_NULL, STXT_RETURN);

                    init_data.both_rect = MMITHEME_GetWinClientBothRect(addData->winId);

                    init_data.type = GUIEDIT_TYPE_PASSWORD;
                    create.ctrl_id = addData->ctrlId;
                    create.guid = SPRD_GUI_EDITBOX_ID;
                    create.parent_win_handle = addData->winId; 
                    create.init_data_ptr = &init_data;
                    MMK_CreateControl( &create );

                    GUIEDIT_SetIm(addData->ctrlId, GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
                    GUIEDIT_SetMaxLen(addData->ctrlId, editPtr->maxLen);
                    GUIEDIT_SetAlign(addData->ctrlId, ALIGN_LEFT);//靠左上起
                    GUIEDIT_SetDispImIcon(addData->ctrlId, &is_disp_im_icon, PNULL);//显示输入法的icon
                    GUIEDIT_SetString(addData->ctrlId, textInfo->wstr_ptr, textInfo->wstr_len);
					/* add by zack@20120809 start */
					edit_border.type  = GUI_BORDER_NONE;
					GUIEDIT_SetBorder(addData->ctrlId, &edit_border);
                    /* add by zack@20120809 end */
					break;
                case HR_EDIT_ALPHA:
                    GUIWIN_CreateTitleDyna(addData->winId, TXT_NULL);
                    MMIHEROAPP_CreatSoftkeyCtrl(addData->winId, STXT_OK, TXT_NULL, STXT_RETURN);

                    init_data.both_rect = MMITHEME_GetWinClientBothRect(addData->winId);
					init_data.type = GUIEDIT_TYPE_TEXT;
					create.ctrl_id = addData->ctrlId;
					create.guid = SPRD_GUI_EDITBOX_ID;
					create.parent_win_handle = addData->winId; 
					create.init_data_ptr = (void*)&init_data; /*lint !e733*/
					MMK_CreateControl( &create );
                    GUIEDIT_SetIm(addData->ctrlId,GUIIM_TYPE_DIGITAL|GUIIM_TYPE_ABC|GUIIM_TYPE_ENGLISH,GUIIM_TYPE_ENGLISH);
                    GUIEDIT_SetStyle(addData->ctrlId, GUIEDIT_STYLE_MULTI);//多行
                    GUIEDIT_SetTextMaxLen(addData->ctrlId,editPtr->maxLen,editPtr->maxLen);
                    GUIEDIT_SetString(addData->ctrlId, textInfo->wstr_ptr, textInfo->wstr_len);    
                    GUIEDIT_SetDispImIcon(addData->ctrlId,&is_disp_im_icon,&is_disp_num_info);

                    //init_data.add_data.text_add.cur_inputmethod = IM_SMART_MODE;
                    break;
				}
				result = TRUE;
			}
			break;
		case HERO_APP_WINTYPE_MENU:
			{
				GUIMENU_INIT_DATA_T  init_data = {0};
                init_data.both_rect = MMITHEME_GetWinClientBothRect(addData->winId);
                init_data.both_rect.v_rect.top = init_data.both_rect.v_rect.top + MMI_TITLE_THEME_HEIGHT;
                init_data.both_rect.h_rect.top = init_data.both_rect.h_rect.top + MMI_TITLE_THEME_HEIGHT_H;
#ifdef MMI_PDA_SUPPORT
				init_data.both_rect.v_rect.bottom = init_data.both_rect.v_rect.bottom + (int16)MMITHEME_GetWinSoftkeyHeight(addData->winId);
				init_data.both_rect.h_rect.bottom = init_data.both_rect.h_rect.bottom + (int16)MMITHEME_GetWinSoftkeyHeight(addData->winId);
#endif
				init_data.is_static = FALSE;
				init_data.node_max_num = GUIMENU_DYNA_NODE_MAX_NUM;
				init_data.menu_style = GUIMENU_STYLE_SECOND;
				create.ctrl_id = addData->ctrlId;
				create.guid    = SPRD_GUI_MENU_ID;
				create.parent_win_handle = addData->winId;
				create.init_data_ptr = (void*)&init_data; /*lint !e733*/
				MMK_CreateControl(&create);
#ifndef MMI_PDA_SUPPORT
				MMIHEROAPP_CreatSoftkeyCtrl(addData->winId, TXT_NULL, TXT_NULL, TXT_NULL);
				GUIMENU_SetDynamicMenuSoftkey(addData->ctrlId, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#endif
				
				GUIWIN_CreateTitleDyna(addData->winId, TXT_NULL);
				if(addData->u.menu.title.wstr_ptr!= NULL)
				{
					GUIMENU_SetMenuTitle(&addData->u.menu.title, addData->ctrlId);
				}
				
				result = TRUE;
			}
			break;
		case HERO_APP_WINTYPE_APP:
			result = TRUE;
			break;
		default:
			break;
		}
		
		if(result != TRUE)
		{			
			MMK_CloseWin(addData->winId);
			winId = -1;
		}
	}
	else 
	{
		MMIHEROAPP_OpDynId(winType, addData->winId, HERO_APP_ID_WIN);//free id
		winId = -2;
	}
		
	return winId;
}


/*****************************************************************************/
// 	Description :动态窗口消息处理函数
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIHEROAPP_HandleDynWinMsg(
											 MMI_HERO_APP_WINDOW_ID_E win_id, 
											 MMI_MESSAGE_ID_E msg_id, 
											 DPARAM param
											 )
{
	BOOLEAN recode = MMI_RESULT_TRUE;
	MMI_HERO_APP_ADD_PARAMETERS_T* addData = (MMI_HERO_APP_ADD_PARAMETERS_T*)MMK_GetWinAddDataPtr(win_id);

	MMIHR_Trace(("hero. MMIHEROAPP_HandleDynWinMsg win: %d, msg_id :0x%x, addData:0x%x", win_id, msg_id, addData));

	if(addData == NULL)
	{	
		return MMI_RESULT_FALSE;
	}
	
	MMIHR_Trace(("hero. dynWinType: %d", addData->dynWinType));
	if(msg_id == MSG_LOSE_FOCUS)
	{
		MMIHEROAPP_pauseApp();
	}
	else if(msg_id == MSG_GET_FOCUS)
	{
		MMIHEROAPP_resumeApp();
	}
/* begin:modified by Tommy.yan 2012022 */
	else if(msg_id == MMI_SELECT_CONTACT_CNF)
	{
		MMIHEROAPP_pbInfoReturn((MMIPB_HANDLE_T)param);
	}
/* end:modified by Tommy.yan 2012022 */
	else
	{
		switch(addData->dynWinType)
		{
		case HERO_APP_WINTYPE_TEXT:
			{
				HERO_APP_TEXT_DATA *textPtr = &addData->u.text;
				switch(msg_id)
				{
				case MSG_OPEN_WINDOW:
					GUITEXT_SetString(addData->ctrlId, textPtr->textInfo.wstr_ptr, textPtr->textInfo.wstr_len, FALSE);
					
#ifdef MMI_PDA_SUPPORT
					if(HR_DIALOG_OK_CANCEL == textPtr->type)
					{
						GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            			GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_COMMON_OK, FALSE);
            			GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_APP_OK, 0);
					}
					else if(HR_DIALOG_OK == textPtr->type)
					{
						GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);
						GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            			GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_COMMON_OK, FALSE);
            			GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_APP_OK, 0);
					}
					else if(HR_DIALOG_CANCEL_OK == textPtr->type)
					{
						GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
						GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
            			GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, TXT_COMMON_OK, FALSE);
            			GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_LEFT, MSG_APP_OK, 0);
					}
#endif
					break;
                    
				case MSG_FULL_PAINT:
					{
						if(textPtr->title.wstr_ptr != NULL)
						{
							GUIWIN_SetTitleText(win_id, textPtr->title.wstr_ptr, textPtr->title.wstr_len, TRUE);				
						}
						MMK_SetAtvCtrl( win_id, addData->ctrlId);
					}
					break;
					
				case MSG_CLOSE_WINDOW:
					MMIHEROAPP_freeDynWinData(addData);
					break;
#ifdef MMI_PDA_SUPPORT
     			case MSG_CTL_PENOK:
        			if (PNULL != param)
        			{
            			uint src_id = ((MMI_NOTIFY_T*)param)->src_id;
            			switch(src_id)
            			{
            			case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                			MMK_SendMsg(win_id, MSG_APP_OK, NULL);
                			break;
            			case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                			break;
            			case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                 			MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);
                			break;
            			default:
                			break;
            			}
        			}
        			break;
#else
    			case MSG_CTL_PENOK:
#endif
				case MSG_CTL_MIDSK:
				case MSG_APP_OK:
				case MSG_CTL_OK:
				case MSG_APP_WEB:
				case MSG_KEYUP_OK:	
#ifdef MMI_PDA_SUPPORT
				case MSG_APP_MENU:
#endif
					if(textPtr->type == HR_DIALOG_OK || textPtr->type == HR_DIALOG_OK_CANCEL)
					{
						ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_SELECT, NULL);
					}
					else if(textPtr->type == HR_DIALOG_CANCEL_OK)
					{
						ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_RETURN, NULL);
					}
					break;
					
				case MSG_CTL_CANCEL:
				case MSG_APP_CANCEL:
					if(textPtr->type == HR_DIALOG_CANCEL || textPtr->type == HR_DIALOG_OK_CANCEL)
					{
						ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_RETURN, NULL);
					}
					else if(textPtr->type == HR_DIALOG_CANCEL_OK)
					{
						ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_SELECT, NULL);
					}
					break;
					
				case MSG_KEYPRESSUP_RED:
				case MSG_KEYDOWN_RED:
					if(s_hr_fbhup2idle != 1)	
						recode = MMI_RESULT_FALSE;
					break;			
                    
				default:
					recode = MMI_RESULT_FALSE;
					break;
				}	
			}
			break;
		
		case HERO_APP_WINTYPE_EDITOR:
			{
				HERO_APP_EDIT_DATA *editPtr = &addData->u.edit;
				
				switch(msg_id)
				{
				case MSG_OPEN_WINDOW:
					MMK_SetAtvCtrl(win_id, addData->ctrlId);
					if(editPtr->title.wstr_ptr!= NULL)
					{
						GUIWIN_SetTitleText(addData->winId, (uint8 *)editPtr->title.wstr_ptr, (uint16)editPtr->title.wstr_len, FALSE); /*lint !e64*/
					}

#ifdef MMI_PDA_SUPPORT
					GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_COMMON_OK, FALSE);
            		GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_APP_OK, 0);
//					GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_ISTYLE_TITLE_OK, IMAGE_ISTYLE_TITLE_OK_DOWN, FALSE);
#endif

					break;

				case MSG_FULL_PAINT:
					MMIHR_Trace(("hero.edit. MSG_FULL_PAINT"));
					break;
					
				case MSG_CLOSE_WINDOW:
					MMIHEROAPP_freeDynWinData(addData);
					break;

#ifdef MMI_PDA_SUPPORT
     			case MSG_CTL_PENOK:
        			if (PNULL != param)
        			{
            			uint src_id = ((MMI_NOTIFY_T*)param)->src_id;
            			switch(src_id)
            			{
            			case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                			MMK_SendMsg(win_id, MSG_APP_OK, NULL);
                			break;
            			case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                			break;
            			case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                 			MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);
                			break;
            			default:
                			break;
            			}
        			}
        			break;
#else
    			case MSG_CTL_PENOK:
#endif				
				case MSG_CTL_OK:
                case MSG_APP_WEB:
                case MSG_APP_OK:
			    case MSG_CTL_MIDSK:		
#ifdef MMI_PDA_SUPPORT
				case MSG_APP_MENU:
#endif
					ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_SELECT, NULL);
					break;
					
				case MSG_CTL_CANCEL:
                case MSG_APP_CANCEL:
					ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_RETURN, NULL);
					break;
					
				default:
					recode = MMI_RESULT_FALSE;
					break;
				}	
			}
			break;
			
		case HERO_APP_WINTYPE_MENU:
			{
				HERO_APP_MENU_DATA *menuPtr = &addData->u.menu;
				HERO_APP_LIST_ITEM_T *findElem;
				uint16 curIndex;
				int32 key;
				BOOLEAN found = FALSE;
				
				curIndex = GUIMENU_GetCurNodeId(addData->ctrlId) - 1;
				menuPtr->curFocusItemIdx = curIndex;
				
				switch(msg_id)
				{
				case MSG_OPEN_WINDOW:
					MMIHR_Trace(("hero.menu. MSG_OPEN_WINDOW"));
					MMK_SetAtvCtrl( win_id, addData->ctrlId);
					break;

				case MSG_CLOSE_WINDOW:
					MMIHEROAPP_freeDynWinData(addData);
					break;
					
				case MSG_CTL_MIDSK:
				case MSG_CTL_OK:
				case MSG_CTL_PENOK:
                case MSG_APP_OK:
                case MSG_APP_WEB:
					findElem = menuPtr->listElem;
					while(findElem != NULL)
					{
						if(findElem->index == curIndex)
						{
							key = findElem->key;
							found = TRUE;
							break;
						}
						findElem = findElem->nextItem;
					}
					
					if(found)
					{
						menuPtr->preFocusItemIdx = curIndex;
						ProcessHeroAppKeyMsg(MMI_HERO_APP_MENU_SELECT, (void*)&key);
					}
					break;
					
				case MSG_CTL_CANCEL:
                case MSG_APP_CANCEL:
					ProcessHeroAppKeyMsg(MMI_HERO_APP_MENU_RETURN, NULL);
					break;

				case MSG_CTL_LIST_MOVEUP:
				case MSG_CTL_LIST_MOVEDOWN:
				case MSG_CTL_LIST_MOVEBOTTOM:
				case MSG_CTL_LIST_MOVETOP:
					MMIHR_Trace(("*****************MSG_CTL_LIST_XXX, msg_id: %d", msg_id));

					if(msg_id == MSG_CTL_LIST_MOVEUP)
					{
						if(curIndex == 0)
						{
							menuPtr->curFocusItemIdx = menuPtr->maxItemNum - 1;
						}
						else
						{
							menuPtr->curFocusItemIdx = curIndex - 1;
						}
					}
					else if(msg_id == MSG_CTL_LIST_MOVEDOWN)
					{
						if(curIndex == menuPtr->maxItemNum - 1)
						{
							menuPtr->curFocusItemIdx = 0;
						}
						else
						{
							menuPtr->curFocusItemIdx = curIndex + 1;
						}
					}
					else if(msg_id == MSG_CTL_LIST_MOVEBOTTOM)
					{
						menuPtr->curFocusItemIdx = 0;
					}
					else
					{
						menuPtr->curFocusItemIdx = menuPtr->maxItemNum - 1;
					}
					
					MMIHR_Trace(("curIndex: %d, maxItemNum: %d", curIndex, menuPtr->maxItemNum));
					menuPtr->preFocusItemIdx = menuPtr->curFocusItemIdx;
					break;	

				default:
					recode = MMI_RESULT_FALSE;
					break;
				}	
			}
			break;
			
		case HERO_APP_WINTYPE_APP:
			{
				switch(msg_id)
				{
				case MSG_OPEN_WINDOW:
				case MSG_FULL_PAINT:
					break;
				case MSG_CLOSE_WINDOW:
					MMIHEROAPP_freeDynWinData(addData);
					break;
#ifdef HERO_HANDSET_IS_SUPPORT_TOUCHPANEL
				case MSG_TP_PRESS_DOWN:
					recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_DOWN, (void*)param);
					break;
				case MSG_TP_PRESS_UP:
					recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_UP, (void*)param);
					break;
//2010-2-24 add move & update HERO_PORT_VER to 5
				case MSG_TP_PRESS_MOVE:
					recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_MOVE, (void*)param);
					break;
//2010-2-24 add end
#endif
				case MSG_KEYPRESSUP_RED:
				case MSG_KEYDOWN_RED:
					if(s_hr_fbhup2idle != 1)	
						recode = MMI_RESULT_FALSE;
					break;
				default:
					recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_KEY_NORMAL, (void*)&msg_id);
					break;
				}	
			}
			break;
            
		default:
			MMIHR_Trace(("error para: %d",addData->dynWinType));
			recode = MMI_RESULT_FALSE;
			break;
		}
	}
	
	MMIHR_Trace(("MMIHEROAPP_HandleDynWinMsg() win:%d, msg_id: 0x%x, recode :%d", win_id, msg_id, recode));
	return recode;	
}

/*****************************************************************************/
// 	Description : get according icon of a item
//	Global resource dependence : none
//  Author: qgp
//	Note: item icon 数目依赖平台定义了的数目，当前为20
/*****************************************************************************/
MMI_IMAGE_ID_T MMIHEROAPP_getItemIcon(MMI_HERO_APP_MENUICON_OP_TYPE_E opType, uint16 index)
{
	MMI_IMAGE_ID_T imageId;
	uint16 fixIdx;
	MMIHR_Trace(("MMIHEROAPP_getItemIcon index: %d",index));

	/*item icon 数目依赖平台定义了的数目，当前为20*/
	fixIdx = index%20;

	switch(fixIdx)
	{
	case 0:
		imageId = IMAGE_NUMBER_1;
		break;
	case 1:
		imageId = IMAGE_NUMBER_2;
		break;
	case 2:
		imageId = IMAGE_NUMBER_3;
		break;
	case 3:
		imageId = IMAGE_NUMBER_4;
		break;
	case 4:
		imageId = IMAGE_NUMBER_5;
		break;
    case 5:
		imageId = IMAGE_NUMBER_6;
		break;
	case 6:
		imageId = IMAGE_NUMBER_7;
    	break;
	case 7:
		imageId = IMAGE_NUMBER_8;
		break;
	case 8:
		imageId = IMAGE_NUMBER_9;
		break;
	case 9:
		imageId = IMAGE_NUMBER_10;
		break;
	case 10:
		imageId = IMAGE_NUMBER_11;
		break;
	case 11:
		imageId = IMAGE_NUMBER_12;
		break;
	case 12:
		imageId = IMAGE_NUMBER_13;
		break;
	case 13:
		imageId = IMAGE_NUMBER_14;
		break;
	case 14:
		imageId = IMAGE_NUMBER_15;
		break;
	case 15:
		imageId = IMAGE_NUMBER_16;
		break;
	case 16:
		imageId = IMAGE_NUMBER_17;
		break;
	case 17:
		imageId = IMAGE_NUMBER_18;
		break;
	case 18:
		imageId = IMAGE_NUMBER_19;
		break;
	case 19:
		imageId = IMAGE_NUMBER_20;
		break;
	default:
		imageId = IMAGE_NUMBER_1;
		break;
	}

	MMIHR_Trace(("MMIHEROAPP_getItemIcon imageId: %d",imageId));
	return imageId;
}

/*****************************************************************************/
// 	Description : process the key event
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ProcessHeroAppKeyMsg(
									  MMI_HERO_APP_KEYTYPE_E kType,
									  void *info
									  )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	MMI_MESSAGE_ID_E  msg_id;
	int16 subType = 0;
	int32 key = -1;
	int32 mrResult = HR_FAILED;
	MMIHR_Trace(("hero. ProcessHeroAppKeyMsg() kType:%d, p?: %d", kType, s_hr_vmPaused));

	switch(kType)
	{
	case MMI_HERO_APP_KEY_NORMAL:
		msg_id = *(MMI_MESSAGE_ID_E*)info;
        MMIHR_Trace(("hero. keyid:0x%04x, KeyTableLen: %d", msg_id, ARR_SIZE(s_hero_KeyTable)));
	
		if(msg_id == MSG_KEYDOWN_SDCARD_DETECT)
        {
			hr_event(HR_TCARD_EVENT, HR_TCARD_PLUGIN, 0);
		}
        else if(msg_id == MSG_KEYUP_SDCARD_DETECT)
        {
			hr_event(HR_TCARD_EVENT, HR_TCARD_PLUGOUT, 0);
		}
		/* add by zack@20110907 start */
        else if(msg_id == MSG_KEYDOWN_HEADSET_DETECT || msg_id == MSG_KEYUP_HEADSET_DETECT)
        {
            recode = MMI_RESULT_FALSE;
        }
        /* add by zack@20110907 end */
        else
        {
            if((msg_id >= MSG_KEYPRESSUP) && (msg_id - MSG_KEYPRESSUP < ARR_SIZE(s_hero_KeyTable)))
			{
				msg_id = msg_id - MSG_KEYPRESSUP;
				/*add by zack@20120902 start*/
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        msg_id = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(msg_id);
#endif
                /*add by zack@20120902 end*/
				if(s_hr_keyDown == TRUE)
				{
					key = s_hero_KeyTable[msg_id];
					subType = HR_KEY_RELEASE;
					s_hr_keyDown = FALSE;
				}
			}
			else if((msg_id >= MSG_KEYDOWN) && (msg_id - MSG_KEYDOWN < ARR_SIZE(s_hero_KeyTable)))
			{
				msg_id = msg_id - MSG_KEYDOWN;
				/*add by zack@20120902 start*/
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        msg_id = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(msg_id);
#endif
                /*add by zack@20120902 end*/
				if(s_hr_keyDown == FALSE)
				{
					key = s_hero_KeyTable[msg_id];
					subType = HR_KEY_PRESS;
					s_hr_keyDown = TRUE;
				}
			}
            else if((msg_id >= MSG_KEYUP) && (msg_id - MSG_KEYUP < ARR_SIZE(s_hero_KeyTable)))
			{
				msg_id = msg_id - MSG_KEYUP;
				/*add by zack@20120902 start*/
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        msg_id = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(msg_id);
#endif
                /*add by zack@20120902 end*/
				if(s_hr_keyDown == TRUE)
				{
					key = s_hero_KeyTable[msg_id];
					subType = HR_KEY_RELEASE;
					s_hr_keyDown = FALSE;
				}
			}
			
			if(key != -1)
			{
				mrResult = hr_event(subType, key, 0);
			}
		}
		break;
		
#ifdef HERO_HANDSET_IS_SUPPORT_TOUCHPANEL
	case MMI_HERO_APP_MOUSE_DOWN:
		if(info != NULL)
		{
			if(s_hr_keyDown == FALSE)
			{
				GUI_POINT_T TpPoint = {0};
				s_hr_keyDown = TRUE;
				TpPoint.x = MMK_GET_TP_X(info);
				TpPoint.y = MMK_GET_TP_Y(info);
				MMIHR_Trace(("down x: %d, y: %d", TpPoint.x, TpPoint.y));
				mrResult = hr_event(HR_MOUSE_DOWN, (int32)TpPoint.x, (int32)TpPoint.y);
			}
		}
		break;
		
	case MMI_HERO_APP_MOUSE_UP:
		if(info != NULL)
		{
			if(s_hr_keyDown == TRUE)
			{
				GUI_POINT_T TpPoint = {0};
				s_hr_keyDown = FALSE;
				TpPoint.x = MMK_GET_TP_X(info);
				TpPoint.y = MMK_GET_TP_Y(info);
				MMIHR_Trace(("up x: %d, y: %d", TpPoint.x, TpPoint.y));
				mrResult = hr_event(HR_MOUSE_UP, (int32)TpPoint.x, (int32)TpPoint.y);
			}
		}
		break;

//2010-2-24 add move & update HERO_PORT_VER to 5
	case MMI_HERO_APP_MOUSE_MOVE:
		if(info != NULL)
		{
			if(s_hr_keyDown == TRUE)
			{
				GUI_POINT_T TpPoint = {0};
				TpPoint.x = MMK_GET_TP_X(info);
				TpPoint.y = MMK_GET_TP_Y(info);
				MMIHR_Trace(("move x: %d, y: %d", TpPoint.x, TpPoint.y));
				mrResult = hr_event(HR_MOUSE_MOVE, (int32)TpPoint.x, (int32)TpPoint.y);
			}
		}
        break;
//2010-2-24 add end
#endif		
		
	case MMI_HERO_APP_MENU_SELECT:	
		s_hr_keyDown = FALSE;
		mrResult = hr_event(HR_MENU_SELECT, *(int32*)info, 0);
		break;
		
	case MMI_HERO_APP_MENU_RETURN:	
		s_hr_keyDown = FALSE;
		mrResult = hr_event(HR_MENU_RETURN, 0, 0);
		break;
		
	case MMI_HERO_APP_DIALOG_SELECT:
		s_hr_keyDown = FALSE;
		mrResult = hr_event(HR_DIALOG_EVENT, HR_DIALOG_KEY_OK, 0);
		break;
		
	case MMI_HERO_APP_DIALOG_RETURN:
		s_hr_keyDown = FALSE;
		mrResult = hr_event(HR_DIALOG_EVENT, HR_DIALOG_KEY_CANCEL, 0);
		break;

	default:
		break;
	}

	MMIHR_Trace(("_mrResult_ : %d, key: 0x%x", mrResult, key));
		
	if(mrResult != HR_SUCCESS || key == HR_KEY_NONE)
	{
		recode = MMI_RESULT_FALSE;
	}
	
	return recode;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
MMI_HERO_APP_DIALOG_PARAMETERS_T*  MMIHEROAPP_GetPromptWinAddDataPtr(MMI_WIN_ID_T win_id)
{
	MMI_HERO_APP_DIALOG_PARAMETERS_T *cur, *find=NULL;
	
	cur = s_hr_dialogWinList;
	while(cur != NULL)
	{
		if(cur->winId == win_id)
		{
			find = cur;
			break;
		}
		else
		{
			cur = cur->next;	
		}
	}

	return find;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIHEROAPP_HandlePromptWinMsg(
												 MMI_WIN_ID_T win_id, 
												 MMI_MESSAGE_ID_E msg_id, 
												 DPARAM param
												 )
{
	BOOLEAN recode = MMI_RESULT_TRUE;
	MMI_HERO_APP_DIALOG_PARAMETERS_T *addData = (MMI_HERO_APP_DIALOG_PARAMETERS_T*)MMIHEROAPP_GetPromptWinAddDataPtr(win_id);
	HERO_APP_DIALOG_DATA *dialog;

	MMIHR_Trace(("hero. MMIHEROAPP_HandlePromptWinMsg msg_id :0x%x, addData:0x%x", msg_id, addData));

	if(addData == NULL)
	{	
		return MMI_RESULT_FALSE;
	}
		
	dialog = (HERO_APP_DIALOG_DATA *)&addData->dialog;
	switch(msg_id)
	{
	case MSG_FULL_PAINT:
		{
			MMI_TEXT_ID_T 	left_sk_id = TXT_NULL;
			MMI_TEXT_ID_T 	right_sk_id = TXT_NULL;

			if(dialog->type == HR_DIALOG_OK)
			{
				left_sk_id = TXT_COMMON_OK;
			}
			else if(dialog->type == HR_DIALOG_OK_CANCEL)
			{
				left_sk_id = TXT_COMMON_OK;
				right_sk_id = STXT_CANCEL;
			}
			else if(dialog->type != HR_DIALOG_NOKEY)
			{
				right_sk_id = STXT_RETURN;
			}
			
			s_hr_inMrSelfWinCreating = TRUE;
			MMIPUB_SetWinSoftkey(win_id, left_sk_id, right_sk_id, FALSE);
			/* modify by zzh@20100112 start */
			/* 解决版本升级时频繁更新下载进度重启的问题。
			   AlertWin处理MSG_FULL_PAINT消息时会重新创建自动关闭AlertWin的定时器，
			   移植层创建的AlertWin不需要自动关闭故FULL_PAINT时只刷新窗口内容即可。*/
			//MMIPUB_DisplayPubWin(win_id);
			recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
			/* modify by zzh@20100112 end */
			s_hr_inMrSelfWinCreating = FALSE;
		}
		break;

	case MSG_CLOSE_WINDOW:
		{
    		MMI_HERO_APP_DIALOG_PARAMETERS_T *cur, *pre;
    				
    		MMIHEROAPP_OpDynId(HERO_APP_WINTYPE_DIALOG, addData->winId, HERO_APP_ID_WIN);//free id
    		/*delete the node from list*/
    		if(addData == s_hr_dialogWinList)
    		{
    			s_hr_dialogWinList = addData->next;
    		}
    		else
    		{
    			cur = s_hr_dialogWinList->next;
    			pre = s_hr_dialogWinList;
    			while(cur != NULL)
    			{
    				if(cur == addData)
    				{
    					pre->next = addData->next;
    					break;
    				}
    				else
    				{
    					pre = cur;
    					cur = cur->next;	
    				}
    			}
    		}
    		SCI_FREE(addData);
    		MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		}
		break;
		
	case MSG_GET_FOCUS:
		//刷新背景窗口
		MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
		MMITHEME_SetUpdateDelayCount(1);
		MMIHEROAPP_resumeApp();
		break;

	//new ver del. case MSG_TIMER://pass timer
	//	break;
		
	case MSG_LOSE_FOCUS:
		MMIHEROAPP_pauseApp();
		break;	
		
	case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_CTL_PENOK:
	case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT
	case MSG_APP_MENU:
#endif
		if(dialog->type == HR_DIALOG_OK || dialog->type == HR_DIALOG_OK_CANCEL)
			ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_SELECT, NULL);
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		if(dialog->type == HR_DIALOG_CANCEL || dialog->type == HR_DIALOG_OK_CANCEL)
			ProcessHeroAppKeyMsg(MMI_HERO_APP_DIALOG_RETURN, NULL);
		break;
        
	case MSG_KEYPRESSUP_RED:
	case MSG_KEYDOWN_RED:
		if(s_hr_fbhup2idle != 1)	
			recode = MMI_RESULT_FALSE;
		break;
        
	default:
        //modify by zack@20100628 本地UI对话框普通按键事件不需要传给VM
#if 0//def HERO_APP_SIMUGAME_OPEN
        if(MMIHEROAPP_GetCurApp() == MRAPP_NESGAME && dialog->type == HR_DIALOG_CANCEL)
        {
    		ProcessHeroAppKeyMsg(MMI_HERO_APP_KEY_NORMAL, (void*)&msg_id);
        }
#endif
        //modify by zack@20110907 NES模拟器自定义按键时需要将所有按键事件传给MRP
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}		

	MMIHR_Trace(("MMIHEROAPP_HandlePromptWinMsg() recode :%d, event: 0x%x", recode, msg_id));
	return recode;	
}

/*****************************************************************************/
//  Description : handle file browser win message
//  Global resource dependence :
//  Author: zack.zhang
//  Note:modify by zack@20120415
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIHEROAPP_HandleFileBrowserWinMsg(
        													 MMI_WIN_ID_T          win_id,
        													 MMI_MESSAGE_ID_E      msg_id,
        													 DPARAM                param
        													 )
{
	int32 len = 0;
    int32 ret = 0;
    MMI_RESULT_E       result = MMI_RESULT_TRUE;
    FILEARRAY_DATA_T   file_data = {0};
	static int32 isSelecet = 0;

	MMIHR_Trace(("MMIHEROAPP_HandleFileBrowserWinMsg win_id = %d, msg_id = 0x%04x isSelecet=%d s_hr_vmPaused=%d",win_id, msg_id, isSelecet, s_hr_vmPaused));
    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
		isSelecet = 0;
		result = MMI_RESULT_FALSE;
		break;

	case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_PENOK:
		isSelecet = 1;
        MMIFMM_GetOpenFileWinCurFile(&file_data); /*lint !e718 !e18*/
        MMIHR_Trace(("MMIHEROAPP_HandleFileBrowserWinMsg len = %d",file_data.name_len));
		/*modify by zack@20120816 start*/
        if (file_data.type == FILEARRAY_TYPE_FOLDER)
		{
			ret = MMI_RESULT_FALSE;
		}
        else
		/*modify by zack@20120816 end*/
        if(file_data.name_len > 0)
        {
            SCI_MEMSET(g_hr_filebrowser_path, 0x00, sizeof(g_hr_filebrowser_path));
            len = GUI_WstrToGB(g_hr_filebrowser_path, file_data.filename, file_data.name_len);
			MMIHR_Trace(("len = %d path = %s", len, g_hr_filebrowser_path));
            //MMK_CloseWin(win_id);
			MMK_SendMsg(win_id, MSG_FMM_CLOSE_LAYER_WIN_BY_CLIENT, NULL);
            ret = hr_event(HR_LOCALUI_EVENT, HR_DIALOG_KEY_OK, 0);
        }
		else
		{
			//MMK_CloseWin(win_id);
			MMK_SendMsg(win_id, MSG_FMM_CLOSE_LAYER_WIN_BY_CLIENT, NULL);
            ret = hr_event(HR_LOCALUI_EVENT, HR_DIALOG_KEY_CANCEL, 0);
		}
        break;

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		//MMK_CloseWin(win_id);
		MMK_SendMsg(win_id, MSG_FMM_CLOSE_LAYER_WIN_BY_CLIENT, NULL);
        ret = hr_event(HR_LOCALUI_EVENT, HR_DIALOG_KEY_CANCEL, 0);
        break;

	case MSG_CLOSE_WINDOW:
		if(isSelecet == 0)
		{
			if(s_hr_vmPaused)
            {
                MMIHEROAPP_resumeApp();
            }         
			ret = hr_event(HR_LOCALUI_EVENT, HR_DIALOG_KEY_CANCEL, 0);
		}
		result = MMI_RESULT_FALSE;
		break;
        
    default:
		result = MMI_RESULT_FALSE;
        //result = MMIFMM_HandleOpenFileByLayerWin(win_id, msg_id, param); /*lint !e718 !e18*/
        break;
    }

	MMIHR_Trace(("MMIHEROAPP_HandleFileBrowserWinMsg Exit"));
    return result;
}


/*****************************************************************************/
// 	Description :创建一dialog win
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
int32 MMIHEROAPP_createPromptWin(MMI_HERO_APP_DIALOG_PARAMETERS_T *addData, MMI_STRING_T *textInfo)
{
	int32		winId = -1;
	BOOLEAN		result = FALSE;
	MMI_HERO_APP_DIALOG_PARAMETERS_T *cur, *pre;
	MMI_IMAGE_ID_T imgId;
	uint32 time_period = 0xffffff;
	
	MMIHR_Trace(("hero. MMIHEROAPP_createPromptWin()"));
	
	winId = MMIHEROAPP_OpDynId(HERO_APP_WINTYPE_DIALOG, -1, HERO_APP_ID_WIN);//get id
	if(winId == -1)
	{
		return HR_FAILED;
	}	
	addData->winId = winId;

	if(addData->dialog.type == HR_DIALOG_OK_CANCEL)
	{
		imgId = IMAGE_PUBWIN_QUERY;
	}
	else
	{
		imgId = IMAGE_PUBWIN_WARNING;
	}
	s_hr_inMrSelfWinCreating = TRUE;
	MMIPUB_OpenAlertWinByTextPtr(&time_period, textInfo, NULL, imgId, &addData->winId, NULL, MMIPUB_SOFTKEY_CUSTOMER, (MMIPUB_HANDLE_FUNC)MMIHEROAPP_HandlePromptWinMsg);		
	s_hr_inMrSelfWinCreating = FALSE;//move to it handle as some latest sw the pop is asyn created
	MMIHR_Trace(("MMIPUB_OpenAlertWinByTextPtr(), result: %d, id: %d", result, addData->winId));
    //MMIPUB_SetAlertWinTextByPtrEx(addData->winId, textInfo, TRUE);
	cur = s_hr_dialogWinList;
	pre = s_hr_dialogWinList;
	while(cur != NULL)
	{
		pre = cur;
		cur = cur->next;
	}
	
	if(pre == NULL)
	{
		s_hr_dialogWinList = addData;
	}
	else
	{
		pre->next = addData;
	}

	return addData->winId;
}

/*****************************************************************************/
// 	Description : init local resource
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL  void MMIHEROAPP_initLocalRes(void)
{
	MMIHR_Trace(("hero. MMIHEROAPP_initLocalRes()"));
	MMIHEROAPP_setDefFilePath();
	MMIHEROAPP_timerStop(0);	
	MMIHEROAPP_stopShake();
	MMIHEROAPP_gprsProtectTimerStop();
}


/* begin:modified by Tommy.yan 20120222 */
/*****************************************************************************/
// 	Description : get pb info back
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
LOCAL void MMIHEROAPP_pbInfoReturn(MMIPB_HANDLE_T handle)
{
	MMIPB_BCD_NUMBER_T	bcd_number = {0};
	uint8	tele_len = 0;
	uint8	temp_num[MMIPB_BCD_NUMBER_MAX_LEN*2 + 2]= {0};
	wchar   pb_name_str[HR_PHB_MAX_NAME + 1] = {0};
	wchar	pb_email_str[HR_PHB_MAX_NUMBER + 1] = {0};
	MMI_STRING_T content = {0};
	uint16 num_index = 0;
	MMIPB_CONTACT_T contact_ptr = {0};
	
	if(g_hr_pbinfo != NULL)
	{
		SCI_MEMSET(g_hr_pbinfo, 0, sizeof(HERO_APP_PBINFO_T));
		g_hr_pbinfo->result = HR_SUCCESS;
		
		if(MMIAPIPB_GetSelectCount(handle) > 0)
		{
			/*get number*/
			if (MMIAPIPB_GetSelectNumber(handle, &bcd_number, MMIPB_BCD_NUMBER_MAX_LEN, 0))
			{
				tele_len = MMIAPICOM_GenDispNumber( 
						MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton), 
						(uint8)MIN(bcd_number.number_len, MMIPB_BCD_NUMBER_MAX_LEN), 
						bcd_number.number,  
						temp_num,
						(MMIPB_BCD_NUMBER_MAX_LEN*2 + 2)
					);
				MMI_STRNTOWSTR(g_hr_pbinfo->info.number, (HR_PHB_MAX_NUMBER-1)/2, temp_num, (MMIPB_BCD_NUMBER_MAX_LEN*2 + 2), tele_len); /*lint !e64*/
			}
				
			/*get name*/
			content.wstr_ptr= pb_name_str;
			MMIAPIPB_GetSelectName(handle, &content, HR_PHB_MAX_NAME, 0);
			MMIAPICOM_Wstrcpy((uint16*)g_hr_pbinfo->info.name, content.wstr_ptr);
				
			/*get email*/
			content.wstr_ptr= pb_email_str;
			MMIAPIPB_GetSelectEmail(handle, &content, HR_PHB_MAX_NUMBER, 0);
			MMIAPICOM_Wstrcpy((uint16*)g_hr_pbinfo->info.emailAddress, content.wstr_ptr);
		}
	}
	else
	{
		g_hr_pbinfo->result = HR_FAILED;
	}

	g_hr_pbinfo->haveBack = 1;
    /*modify by zack@20110921 start*/
    /*
     *把调用hr_event放到BaseWin获得焦点的地方，否则动态横竖屏切换时会有花屏现象
     */
	//hr_event(HR_LOCALUI_EVENT, HR_LOCALUI_OK, 0);
	g_hr_pbinfo->needInform = TRUE;
	s_hr_PBOfCancelMsg = FALSE;

    /*modify by zack@20110921 end*/
}
/* end:modified by Tommy.yan 2012022 */

/*****************************************************************************/
// 	Description : stop the vibrator timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_stopShake(void)
{
	if(s_hr_vibra_timer > 0)
	{
		MMK_StopTimer(s_hr_vibra_timer);
		s_hr_vibra_timer = 0;
		MMIAPISET_StopVibrator(MMISET_RING_TYPE_OTHER);
	}
}

/*****************************************************************************/
// 	Description : start a timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_timerStart(uint16 t)
{
	MMIHR_TraceT(("hero. MMIHEROAPP_timerStart() t: %d", t));

	if(!s_hr_allReady)
	{
		MMIHR_TraceT(("hero.timer not ready !!!"));
		return 0;
	}
	if (s_hrTimer != 0)
	{
		MMK_StopTimer(s_hrTimer);
	}
	s_hrTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, t, FALSE);

	MMIHR_TraceT(("hero. return s_hrTimer: %d", s_hrTimer));
	return s_hrTimer;
}

/*****************************************************************************/
// 	Description : start a vibrator
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_startShake(int32 t)
{
    MMIHR_Trace(("hero. MMIHEROAPP_startShake() t: %d", t));

    if(!s_hr_allReady)
	{
		return 0;
	}
	
	MMIHEROAPP_stopShake();
	s_hr_vibra_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, t, FALSE);
	if(s_hr_vibra_timer != 0)
	{
		MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_OTHER);
	}

	return s_hr_vibra_timer;
}

/*****************************************************************************/
// 	Description : stop the timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_timerStop(int32 t)
{
	MMIHR_TraceT(("hero. MMIHEROAPP_timerStop() s_hrTimer: %d", s_hrTimer));

	if (s_hrTimer > 0)
	{
		MMK_StopTimer(s_hrTimer);
		s_hrTimer = 0;
	}
}

/*****************************************************************************/
// 	Description : start a timer for gprs activing timeout protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_gprsProtectTimerStart(uint16 t)
{
	MMIHR_Trace(("hero. MMIHEROAPP_gprsProtectTimerStart() t: %d", t));
	
	if(!s_hr_allReady)
	{
		return 0;
	}
	
	if (s_hr_gprsProtectTimer > 0)
	{
		MMK_StopTimer(s_hr_gprsProtectTimer);
	}

	s_hr_gprsProtectTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, t, FALSE);

	MMIHR_Trace(("hero. s_hr_gprsProtectTimer: %d", s_hr_gprsProtectTimer));
	return s_hr_gprsProtectTimer;
}

/*****************************************************************************/
// 	Description : stop the protect timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_gprsProtectTimerStop(void)
{
	if (s_hr_gprsProtectTimer > 0)
	{
		MMK_StopTimer(s_hr_gprsProtectTimer);
		s_hr_gprsProtectTimer = 0;
	}
}

/*****************************************************************************/
// 	Description : start a timer for gprs activing timeout protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_gprsRetryTimerStart(uint16 t)
{	
	if(!s_hr_allReady)
    {
		return 0;
	}
	
	if (s_hr_gprsRetryTimer > 0)
    {
		MMK_StopTimer(s_hr_gprsRetryTimer);
	}

	s_hr_gprsRetryTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, t, FALSE);
	MMIHR_Trace(("hero. MMIHEROAPP_gprsRetryTimerStart: %d", s_hr_gprsRetryTimer));
	return s_hr_gprsRetryTimer;
}

/*****************************************************************************/
// 	Description : stop the protect timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_gprsRetryTimerStop(void)
{
	if (s_hr_gprsRetryTimer > 0)
    {
		MMK_StopTimer(s_hr_gprsRetryTimer);
		s_hr_gprsRetryTimer = 0;
	}
}

/*****************************************************************************/
// 	Description : set all the resource state
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_setAllReady(BOOLEAN ready)
{
	s_hr_allReady = ready;
}

/*****************************************************************************/
// 	Description : pause mr app
//	Global resource dependence : none
//  Author: qgp
//	Note:  
//comment. 分为一下情况:
// 1。被mr app自己开的窗体覆盖:则不调用hr_pauseApp(),应该由app自己管理暂停那事件。
// 2。被其他窗体(如call)覆盖:则调用hr_pauseApp(),
/*****************************************************************************/
int32 MMIHEROAPP_pauseApp(void)
{
	int32 res;
	MMIHR_Trace(("hero. MMIHEROAPP_pauseApp() : %d, %d, %d, %d", s_hr_vmPaused, s_hrTimer, s_hr_inMrSelfWinCreating, s_hr_VMReady));

	if(s_hr_VMReady)
	{
		if(s_hr_keyDown == TRUE)
		{
			s_hr_keyDown = FALSE;//current mr win have been covered, so reset the key event flag
		}

		if(!s_hr_inMrSelfWinCreating && !s_hr_vmPaused)
		{
            MMIHEROAPP_PauseAudioPlayer(); /*lint !e718 !e18*/ //add for mpmusic 2010-07-07  
            #if !defined(WIN32)
			res = hr_pauseApp();
			#endif
			MMIHR_Trace(("paused! res: %d, %d", res, s_hrTimer));
			s_hr_vmPaused = TRUE;
			MMIHEROAPP_StopPlaySoundProtect();//2008-3-14 add, fix sound error after get a call .
			return res;
		}
		else
		{
			return HR_SUCCESS;
		}
	}

	return HR_IGNORE;
}

/*****************************************************************************/
// 	Description : resume mr app
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
int32 MMIHEROAPP_resumeApp(void)
{
	int32 res;
	
	MMIHR_Trace(("hero. MMIHEROAPP_resumeApp() :%d, %d, %d", s_hr_vmPaused, s_hrTimer, s_hr_VMReady));	
    
	if(s_hr_VMReady)
	{
		if(s_hr_vmPaused)
		{
#ifdef HERO_ENGINE_SUPPORT_CLKADAPT
			MMIHR_Trace(("hero. pre clk:%d, cur:%d", s_hr_cpuClk_mng.curCpuClk, HAL_GetMCUClk()));
			if(s_hr_cpuClk_mng.curCpuClk != HAL_GetMCUClk())
			{
				MMIHEROAPP_SetArmClkHigh();
			}
#endif
			#if !defined(WIN32)
			res = hr_resumeApp();
			#endif
            MMIHEROAPP_ResumeAudioPlayer(); /*lint !e718 !e18*/ //add for mpmusic 2010-07-07  
			MMIHR_Trace(("resume! res: %d, %d", res, s_hrTimer));
			s_hr_vmPaused = FALSE;
			return res;
		}
		else
		{
			if(g_hr_pbinfo !=NULL && g_hr_pbinfo->haveBack == 0)
			{
				g_hr_pbinfo->haveBack = 1;
				hr_event(HR_LOCALUI_EVENT, HR_LOCALUI_CANCEL, 0);
			}
			return HR_SUCCESS;
		}
	}

	return HR_IGNORE;
}

/*****************************************************************************/
// 	Description : set arm clk to changed
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
BOOLEAN MMIHEROAPP_CLKSetted(BOOLEAN setted, uint32 curCpuClkParam, uint32 curCpuClk)
{
	BOOLEAN old = s_hr_cpuClk_mng.setted;
	
	s_hr_cpuClk_mng.setted = setted;
	s_hr_cpuClk_mng.curCpuClkParam = curCpuClkParam;
	s_hr_cpuClk_mng.curCpuClk = curCpuClk;

	return old;
}

#ifdef HR_BINDTO_VIRTUAL_WIN
/*****************************************************************************/
//	Description : 
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_ifHrTimer(DPARAM param)
{
	BOOLEAN ret = TRUE;
	MMIHR_TraceT(("MMIHEROAPP_ifHrTimer() param:%d", param));
	
	if(s_hrTimer == *(uint8*)param)
	{
		MMIHEROAPP_timerStop(0);
		#if !defined(WIN32)
		hr_timer();
		#endif
		
#ifdef MRAPP_TIMERWATCH_DOG	
		if(s_mr_protectTimer.mrTimerStarted == TRUE && !MMK_IsTimerActive(s_hrTimer))
		{
			MMIHR_Trace(("s_hrTimer evn lost!!"));
			MMK_StartWinTimer(VIRTUAL_WIN_ID, s_hrTimer, s_mr_protectTimer.timerOut, FALSE);
		}
#endif
	}
//modify 20181114 begin
	else if(s_smartTimer == *(uint8*)param)
	{
		extern void hr_smart_timer_callback();
		hr_smart_timer_callback();
	}
	else if(s_callTimer ==  *(uint8*)param)
	{
		extern void hr_call_timer_callback();
		hr_call_timer_callback();
	}
//modify 20181114 end
	///mike.song add by 20140610
#ifdef HERO_ENGINE_SUPPORT
	else if(s_heroTimer == *(uint8*)param)
	{
		extern void heroTimerCallback();
		heroTimerCallback();
	}
#endif
	///mike.song add by 20140610
	/*wuwenjie 2011-12-01 START*/
	else if(hr_getHostTimerId()== *(uint8*)param)
	{
		hr_gethostTimerDeal();
	}
	/*wuwenjie 2011-12-01 END*/
	else if(s_hr_mrPlaySound_timer == *(uint8*)param)
	{
		MMIHEROAPP_playingCheck();
	}
	else if(s_hr_AccessUrl_timer == *(uint8*)param)
	{
		MMIHEROAPP_AccessUrl();
	}
	else if(s_hr_vibra_timer == *(uint8*)param)
	{
		MMIHEROAPP_stopShake();
	}
	else if(s_hr_gprsProtectTimer == *(uint8*)param)
	{
		MMIHEROAPP_gprsProtectTimerStop();
		MMIHEROAPP_netInitResult(HR_FAILED);
	}
	else if(s_hr_gprsRetryTimer == *(uint8*)param)
	{
		MMIHEROAPP_gprsRetryTimerStop();
		MMIHEROAPP_redail();
	}
//2009-7-7 add begin
#ifdef __HERO_ENGINE_OVERSEA__
	else if (s_hr_getApnAccount_timer == *(uint8*)param)
    {
		MMIHEROAPP_getApnAccountTimerStop();
		MMIHEROAPP_getApnAccountRet(TRUE);
	}
    else if(s_hr_setApnAccount_timer == *(uint8*)param)
    {
        MMIHEROAPP_setApnAccountTimerStop();
        MMIHEROAPP_setApnAccountRet(); /*lint !e718 !e18*/
    } 
#endif
//2009-7-7 add end
	else if (s_hr_getSCNumber_timer == *(uint8*)param)
    {
		MMIHEROAPP_GetSCNumberTimerStop();
		MMIHEROAPP_GetSCNumberRet(); /*lint !e718 !e18*/
	}
	/* begin:added by Tommy.yan 20120120 */
	else if (s_hr_getSMSCapacity_timer == *(uint8*)param)
	{
		MMIHEROAPP_GetSMSCapacityTimerStop();
		MMIHEROAPP_GetSMSCapacityCallBack(); /*lint !e718 !e18*/
	}

	else if (s_hr_getSMSContent_timer == *(uint8*)param)
	{
		MMIHEROAPP_GetSMSContentTimerStop();
		MMIHEROAPP_GetSMSContentCallBack(); /*lint !e718 !e18*/
	}

	else if (s_hr_deleteSMS_timer == *(uint8*)param)
	{
		MMIHEROAPP_DeleteSMSTimerStop();
		MMIHEROAPP_DeleteSMSCallBack(); /*lint !e718 !e18*/
	}

	else if(s_hr_setSMSState_timer == * (uint8*)param)
	{
		MMIHEROAPP_SetSMSStateTimerStop();
		MMIHEROAPP_SetSMSStateCallBack(); /*lint !e718 !e18*/
	}

	else if (s_hr_phb_operate_timer == *(uint8*)param)
	{
		MMIHEROAPP_StopPHBOperateTimer();
		//MMIHEROAPP_PHBCallBack(); /*lint !e718 !e746*/
	}
	else if(s_hr_phb_search_timer == *(uint8*)param)
	{
		MMIHEROAPP_PHBSearchCallBack(); /*lint !e746*/
	}
	/* end:added by Tommy.yan 20120120 */	
    else if (s_hr_statusIcon_timer == *(uint8*)param)
    {
		MMIHEROAPP_HandleStatusIconTimer();
	}
#ifdef __HERO_APP_MOTION_SUPPORT__
    else if(hr_motion_handle_timer(*(uint8*)param))
    {
        MMIHR_Trace(("[Motion] timer "));
    }
#endif
	else 
	{
		ret = FALSE;
	}

	return ret;
}
#endif

/*****************************************************************************/
//	Description : 
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIHEROAPP_handleMrMsg(MMI_MESSAGE_ID_E msg_id, DPARAM param)
{ /*lint !e18*/
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    MMIHR_Trace(("hero. MMIHEROAPP_handleMrMsg msg_id=%d", msg_id));
#ifdef HR_BINDTO_VIRTUAL_WIN
	switch(msg_id)
	{
		case MSG_TIMER:
			MMIHEROAPP_ifHrTimer(param);
			break;
		case HERO_APP_MSG_NETINIT_RESULT:
			MMIHR_Trace(("hero. HERO_APP_MSG_NETINIT_RESULT"));
			if(param != NULL)
			{
				MMIHEROAPP_netInitResult(((MMI_HERO_APP_NETINIT_RESULT_T*)param)->result);
				MMIHR_Trace(("hero. net init result return."));
			}
			break;
		case HERO_APP_MSG_NETREDAIL_IND:
			MMIHR_Trace(("hero. HERO_APP_MSG_NETREDAIL_IND."));
			MMIHEROAPP_redail();
			break;
		case HERO_APP_MSG_SYS_EXIT:	
			MMIHR_Trace(("hero. HERO_APP_MSG_SYS_EXIT."));
			MMIHEROAPP_exitMrApp(FALSE);
			break;

        case APP_AUDIO_END_IND:
            MMIHEROAPP_HandleAudioPlayCBMsg((MmiAudioCallbackMsgS *)param);
            break;

        case MMISMS_MSG_DELETE_NEXT:
            MMIHEROAPP_DeleteSMSCallBack(param);
            break;

             /*热拔T卡的情况下提示用户并强制退出VM*/
        case MSG_HANDLE_SD_ERR:
            if(MMIHEROAPP_appIsActived())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_SD_FAIL);
                MMK_ReturnIdleWin();
            }
            break;
        
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	
#endif
	return result;
}

/*****************************************************************************/
// 	Description : free mr mmi res
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_freeMmiRes(void)
{
	MMIHR_Trace(("MMIHEROAPP_freeMmiRes()"));
	MMIHEROAPP_initLocalRes();
	MMIHEROAPP_CloseHrNet();
	MMIHEROAPP_freeAllExtraSrc();
#ifdef HR_BINDTO_VIRTUAL_WIN
	g_hr_vmBackRun = HERO_APP_MAX;
#endif

#ifdef HERO_ENGINE_SUPPORT_CLKADAPT
	MMIHR_Trace(("#######HAL_GetMCUClk1: %d", HAL_GetMCUClk()));
	if(MMIHEROAPP_CLKSetted(FALSE, 0, 0))
	{
		MMIHEROAPP_RestoreARMClk();
	}
	MMIHR_Trace(("#######HAL_GetMCUClk2: %d", HAL_GetMCUClk()));
#endif
    MMISET_SetIsSensorOpenFlag(s_hr_gsensor_flag);
	s_hr_allReady = FALSE;
	
	MMIHR_Trace(("MMIHEROAPP_freeMmiRes() out"));
}	

/*****************************************************************************/
// 	Description : exit vm ,because use hr_stop() in this function, so if trigger by mr app, must use Asyn
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_exitMrApp(BOOLEAN Asyn)
{
	MMIHR_Trace(("MMIHEROAPP_exitMrApp(), %d", Asyn));
	MMIHEROAPP_setAllReady(FALSE);
	if(Asyn)
	{
		MMK_PostMsg(VIRTUAL_WIN_ID, HERO_APP_MSG_SYS_EXIT, NULL,	0);
		MMI_TriggerMMITask();
	}
	else
	{
		if(s_hr_VMReady)
		{
			s_hr_VMReady = FALSE;
			hr_stop();
			MMIHEROAPP_freeMmiRes();
		}
	}
#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
	{
		extern void hr_freeFirstMenuId(void);
		hr_freeFirstMenuId();
	}
#endif
	MMIHR_Trace(("MMIHEROAPP_exitMrApp() out"));
}

/*****************************************************************************/
// 	Description : add for hr_playSound() protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_startPlaySoundProtect(void)
{
	MMIHR_Trace(("hero. MMIHEROAPP_startPlaySoundProtect()"));

	if(!s_hr_allReady)
	{
		return 0;
	}
	
	MMIHEROAPP_StopPlaySoundProtect();
	s_hr_mrPlaySound_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 1000, TRUE);
	return s_hr_mrPlaySound_timer;
}

/*****************************************************************************/
// 	Description : add for hr_playSound() protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_StopPlaySoundProtect(void)
{
	MMIHR_Trace(("hero. MMIHEROAPP_StopPlaySoundProtect() tr: %d", s_hr_mrPlaySound_timer));

	if (s_hr_mrPlaySound_timer > 0)
	{
		MMK_StopTimer(s_hr_mrPlaySound_timer);
		s_hr_mrPlaySound_timer = 0;
	}
}

/*****************************************************************************/
// 	Description : add for start wap broswer in mr apps, when app's mem is shared with wap
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_delayAccessUrl(char* url)
{
	int32 len;

	MMIHR_Trace(("hero. MMIHEROAPP_delayAccessUrl()"));
	if(!url || (len = strlen(url)) == 0)
    {
		return;
	}
	
	if(s_hr_AccessUrl_url)
    {
		SCI_FREE(s_hr_AccessUrl_url);
	}
	
	s_hr_AccessUrl_url = (char*)SCI_ALLOC(len+2);
	if(!s_hr_AccessUrl_url)
    {
		return;
	}
	
	memset(s_hr_AccessUrl_url, 0, len+2);
	memcpy(s_hr_AccessUrl_url, url, len);

	if(s_hr_AccessUrl_timer > 0)
    {
		MMK_StopTimer(s_hr_AccessUrl_timer);
	}
	
	s_hr_AccessUrl_timer = MMK_CreateWinTimer(	VIRTUAL_WIN_ID,	20,	FALSE);

	MMIHR_Trace(("hero. s_hr_AccessUrl_timer: %d", s_hr_AccessUrl_timer));
}

void MMIHEROAPP_AccessUrl(void)
{
#if 0
	MMIHR_Trace(("hero. MMIHEROAPP_AccessUrl(), %d, %d", s_hr_AccessUrl_timer, s_hr_AccessUrl_url));
	if(s_hr_AccessUrl_timer && s_hr_AccessUrl_url)
    {
		MMK_StopTimer(s_hr_AccessUrl_timer);
		s_hr_AccessUrl_timer = 0;
		MMIHEROAPP_closeHeroApp();//exit mr app to free mem of shared
		MMICMSBRW_AccessUrl((uint8*)s_hr_AccessUrl_url);
		SCI_FREE(s_hr_AccessUrl_url);
		s_hr_AccessUrl_url = 0;
	}
#endif
}


/*****************************************************************************/
// 	Description : close mr base win及其上启动的所有win.
//	Global resource dependence : none
//  Author: qgp
//	Note: 2008-6-11 add
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_closeAllWins(void)
{
	int32 i;
	
	/*mr local wins*/
	for(i = MMI_HERO_APP_BASE_WIN_ID; i < HERO_APP_MAX_WIN_ID; i++)
	{
		if(MMK_IsOpenWin(i))
		{
			MMK_CloseWin(i);
		}
	}
	
	return TRUE;
}

//2009-7-7 add begin
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_startGetApnAccountTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}

	MMIHEROAPP_getApnAccountTimerStop();
	s_hr_getApnAccount_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 50, TRUE);
	return s_hr_getApnAccount_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_getApnAccountTimerStop(void)
{
	if (s_hr_getApnAccount_timer > 0)
	{
		MMK_StopTimer(s_hr_getApnAccount_timer);
		s_hr_getApnAccount_timer = 0;
	}
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_startSetApnAccountTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}

	MMIHEROAPP_setApnAccountTimerStop();
	s_hr_setApnAccount_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 50, FALSE);
	return s_hr_setApnAccount_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_setApnAccountTimerStop(void)
{
	if (s_hr_setApnAccount_timer > 0)
	{
		MMK_StopTimer(s_hr_setApnAccount_timer);
		s_hr_setApnAccount_timer = 0;
	}
}
//2009-7-7 add end

/* begin:added by Tommy.yan 20120120 */
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC uint8 MMIHEROAPP_StartGetSMSCapacityTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}

	MMIHEROAPP_GetSMSCapacityTimerStop();
	s_hr_getSMSCapacity_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 50, FALSE);
	return s_hr_getSMSCapacity_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_GetSMSCapacityTimerStop(void)
{
	if (s_hr_getSMSCapacity_timer > 0)
	{
		MMK_StopTimer(s_hr_getSMSCapacity_timer);
		s_hr_getSMSCapacity_timer = 0;
	}
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC uint8 MMIHEROAPP_StartGetSMSContentTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}

	MMIHEROAPP_GetSMSContentTimerStop();
	s_hr_getSMSContent_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 500, FALSE);
	return s_hr_getSMSContent_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_GetSMSContentTimerStop(void)
{
	if (s_hr_getSMSContent_timer > 0)
	{
		MMK_StopTimer(s_hr_getSMSContent_timer);
		s_hr_getSMSContent_timer = 0;
	}
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC uint8 MMIHEROAPP_StartDeleteSMSTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}

	MMIHEROAPP_DeleteSMSTimerStop();
	s_hr_deleteSMS_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 50, FALSE);
	return s_hr_deleteSMS_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_DeleteSMSTimerStop(void)
{
	if (s_hr_deleteSMS_timer > 0)
	{
		MMK_StopTimer(s_hr_deleteSMS_timer);
		s_hr_deleteSMS_timer = 0;
	}
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC uint8 MMIHEROAPP_StartSetSMSStateTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}

	MMIHEROAPP_SetSMSStateTimerStop();
	s_hr_setSMSState_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 50, FALSE);
	return s_hr_setSMSState_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Tommy.yan
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_SetSMSStateTimerStop(void)
{
	if (s_hr_setSMSState_timer > 0)
	{
		MMK_StopTimer(s_hr_setSMSState_timer);
		s_hr_setSMSState_timer = 0;
	}
}
/* end:added by Tommy.yan 20120120 */

//2010-06-24 add begin
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC uint8 MMIHEROAPP_StartGetSCNumberTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}

	MMIHEROAPP_GetSCNumberTimerStop();
	s_hr_getSCNumber_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 50, FALSE);
	return s_hr_getSCNumber_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_GetSCNumberTimerStop(void)
{
	if (s_hr_getSCNumber_timer > 0)
	{
		MMK_StopTimer(s_hr_getSCNumber_timer);
		s_hr_getSCNumber_timer = 0;
	}
}
//2010-06-24 add end

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC uint8 MMIHEROAPP_StartStatusIconTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}
    
	MMIHEROAPP_StopStatusIconTimer();
	s_hr_statusIcon_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 1000, TRUE);
	return s_hr_statusIcon_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_StopStatusIconTimer(void)
{
	if (s_hr_statusIcon_timer > 0)
	{
		MMK_StopTimer(s_hr_statusIcon_timer);
		s_hr_statusIcon_timer = 0;
	}
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_HandleStatusIconTimer(void)
{

	static int flag = 0;

	MMIHR_Trace(("heroapp MMIHEROAPP_HandleStatusIconTimer flag = %d", flag));
    #if !defined(PLATFORM_UIX8910)	
	MAIN_SetIdleHeroappState(flag); /*lint !e718 !e18*/
    #endif	

	flag = !flag;
	GUIWIN_UpdateStb();

}

/*****************************************************************************/
// 	Description : VM后台运行时启动WAP、Camera等模块之前先调用该接口，由该接口
//                根据传入的模块ID来决定强制退出后台应用还是给出提示
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_HandleEventFromOtherModule(int32 module_id)
{ /*lint !e18*/
    BOOLEAN isExit = FALSE;
    MMI_STRING_T string = {0};

    MMIHR_Trace(("mrapp MMIHEROAPP_HandleEventFromOtherModule module_id = %d", module_id));

    if(!MMIHEROAPP_appIsActived())
    {
        return FALSE;
    }

    switch(module_id)
    {
    case MMI_MODULE_CAMERA:
        break;

#ifdef BROWSER_SUPPORT_DORADO
    case MMI_MODULE_MMIBROWSER:
        break;
#endif

#ifdef ATV_SUPPORT
    case MMI_MODULE_ATV:
        break;
#endif

#ifdef QQ_SUPPORT
    case MMI_MODULE_QQ:
        break;
#endif

    default:
        isExit = TRUE;
        break;
    }

    if(isExit == TRUE)
    {
		MMIHEROAPP_exitMrApp(FALSE);

        MMIHEROAPP_StopStatusIconTimer();
		#if !defined(PLATFORM_UIX8910)	
        MAIN_SetIdleHeroappState(FALSE); /*lint !e718 !e18*/
	    #endif
        return FALSE;
    }
    else
    {
/*inkleak 2017-05-08 start(内存不足的提示语乱码修改)*/
#ifdef __HERO_ENGINE_OVERSEA__
		uint8   temp_buf1[64] = {0};
		uint16  temp_buf2[64] = {0};
		sprintf((char*)temp_buf1, "%s", "Exit background app?");
		string.wstr_len = MMIHEROAPP_GB2UCS(temp_buf2, temp_buf1, strlen(temp_buf1));
		string.wstr_ptr = temp_buf2;
#else
		//提示:请先退出后台应用
		string.wstr_len = sizeof("\xF7\x8B\x48\x51\x00\x90\xFA\x51\x0E\x54\xF0\x53\x94\x5E\x28\x75\x00\x00")/2-1;
		string.wstr_ptr = (uint8*)"\xF7\x8B\x48\x51\x00\x90\xFA\x51\x0E\x54\xF0\x53\x94\x5E\x28\x75\x00\x00"; /*lint !e64*/
#endif
		MMIPUB_OpenAlertWinByTextPtr(NULL,&string,NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,MMIHEROAPP_HandleQueryWin1Msg);
        return TRUE;
    }
}

int32 MMIHEROAPP_OpenSelectDataAccountWin(void)
{/*lint !e18*/
    return MMK_CreateWin((uint32 *)MMIHEROAPP_ACCOUNT_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :append one line icon text list item
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void HeroAppendOneLineTextListItem(
                                        MMI_CTRL_ID_T       ctrl_id,
                                        GUIITEM_STYLE_E    item_style,
                                        MMI_TEXT_ID_T       left_softkey_id,
                                        MMI_IMAGE_ID_T      icon_id,
                                        const wchar*        wstr_ptr,
                                        uint16              wstr_len,
                                        uint16              pos,
                                        BOOLEAN             is_update
                                        )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16                  i         = 0;
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if( 0 != icon_id )
    {
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = icon_id;
        i++;
    }
    
    item_data.item_content[i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[i].item_data.text_buffer.wstr_len = wstr_len;
    
    item_data.item_content[i].item_data.text_buffer.wstr_ptr = wstr_ptr;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}


/*****************************************************************************/
// 	Description : handle message of setting list window of browser
//	Global resource dependence : none
//  Author: zack
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHeroAppAccountWinMsg(
											MMI_WIN_ID_T	    win_id,
											MMI_MESSAGE_ID_E	msg_id,
											DPARAM           param
											)
{
#ifdef __HERO_ENGINE_OVERSEA__
	uint8			i = 0;
	uint32          icon_id = 0;
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = HERO_APP_ACCOUNT_LISTCTRL_ID;
	MMICONNECTION_SETTING_T *connectin_setting_info = NULL;
#if (HERO_SPR_VERSION >= 0x10A1140)
    MN_DUAL_SYS_E   simsel = MMIHEROAPP_GetActiveSim(); /*lint !e718*/
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //动态添加LIST ITEM
        GUILIST_SetMaxItem( ctrl_id, MMICONNECTION_MAX_SETTING_NUM, FALSE);//max item 20

		//获取GPRS连接信息
		connectin_setting_info = MMIMRAPP_GetConnetInfo(); /*lint !e718 !e18*/
#if (HERO_SPR_VERSION >= 0x10A1140)
        for(i = 0; i < connectin_setting_info->settinglink_num[simsel]; i++)                              //显示LIST
        {
			if(i == g_hr_curUserApnAccuntId)
			{
				icon_id = IMAGE_RADIO_SELECTED_ICON;
			}
			else
			{
				icon_id = IMAGE_RADIO_UNSELECTED_ICON;
			}
            HeroAppendOneLineTextListItem(ctrl_id,
                GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
                TXT_COMMON_OK,
                icon_id,
                connectin_setting_info->setting_detail[simsel][i].name,
                connectin_setting_info->setting_detail[simsel][i].name_len,
                i,
                FALSE
                );
        } 
		if(g_hr_curUserApnAccuntId >= connectin_setting_info->settinglink_num[simsel])
		{
			GUILIST_SetCurItemIndex(ctrl_id, 0);
		}
		else
		{
			GUILIST_SetCurItemIndex(ctrl_id, g_hr_curUserApnAccuntId);
		}
#else
        for(i = 0; i < connectin_setting_info->settinglink_num; i++)                              //显示LIST
        {
			if(i == g_hr_curUserApnAccuntId)
			{
				icon_id = IMAGE_RADIO_SELECTED_ICON;
			}
			else
			{
				icon_id = IMAGE_RADIO_UNSELECTED_ICON;
			}
            HeroAppendOneLineTextListItem(ctrl_id,
                GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
                TXT_COMMON_OK,
                icon_id,
                connectin_setting_info->setting_detail[i].name,
                connectin_setting_info->setting_detail[i].name_len,
                i,
                FALSE
                );
        } 
		if(g_hr_curUserApnAccuntId >= connectin_setting_info->settinglink_num)
		{
			GUILIST_SetCurItemIndex(ctrl_id, 0);
		}
		else
		{
			GUILIST_SetCurItemIndex(ctrl_id, g_hr_curUserApnAccuntId);
		}
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
    case MSG_APP_WEB:
        {
            i = GUILIST_GetCurItemIndex(ctrl_id);
			g_hr_curUserApnAccuntId = i;
			MMK_CloseWin(win_id);
			MMIHEROAPP_getApnAccountRet(FALSE);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        MMIHR_Trace(("mrapp HandleSettingListWinMsg() msg_id = %d", msg_id));
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
#endif
}/*lint !e533*/

PUBLIC BOOLEAN MMIHEROAPP_IsWindowOfPB(void)
{/*lint !e18*/
	return s_hr_isWindowOfPB;
}

PUBLIC BOOLEAN MMIHEROAPP_IsReadBySMS(void)
{/*lint !e18*/
	return s_hr_isReadBySMS;
}

PUBLIC BOOLEAN MMIHEROAPP_IsVMPaused(void)
{ /*lint !e18*/
    if(s_hr_vmPaused)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

PUBLIC BOOLEAN MMIHEROAPP_IsWindowCreatedBySky(void)
{ /*lint !e18*/
    return s_hr_inMrSelfWinCreating;
}

PUBLIC BOOLEAN MMIHEROAPP_SetIsReadBySMS(BOOLEAN IsReadBySMS)
{/*lint !e18*/
	s_hr_isReadBySMS = IsReadBySMS;
	return TRUE;
}

/* begin:added by Tommy.yan 20120201 */
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_PHBSearchCallBack()
{
	s_hr_phb_search_timer=0;
	hr_event(HR_LOCALUI_EVENT, HR_LOCALUI_OK, 0);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC uint8 MMIHEROAPP_StartPHBOperateTimer(void)
{
	if(!s_hr_allReady)
	{
		return 0;
	}
    
	MMIHEROAPP_StopPHBOperateTimer();
	s_hr_phb_operate_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 1000, FALSE);
	return s_hr_phb_operate_timer;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC void MMIHEROAPP_StopPHBOperateTimer(void)
{
	if (s_hr_phb_operate_timer > 0)
	{
		MMK_StopTimer(s_hr_phb_operate_timer);
		s_hr_phb_operate_timer = 0;
	}
}
/* end:added by Tommy.yan 20120201 */

#ifdef __HERO_CFG_CAMERA_SUPPORT__
int32 hr_camera_create(void)
{
#ifndef WIN32
    int32 ret = 0;

    ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
    MMIHR_Trace(("[SKYCAMERA] DCAMERA_Open ret = %d", ret));
    if(ret == DCAMERA_OP_SUCCESS)
    {
        s_hr_inMrSelfWinCreating = TRUE;
        MMK_CreateWin((uint32 *)MMIHEROAPP_CAMERA_WIN_TAB, PNULL);
        s_hr_inMrSelfWinCreating = FALSE;

        return HR_SUCCESS;
    }
    else
    {
        DCAMERA_Close();
        return HR_FAILED;
    }
#endif
}

/*****************************************************************************/
// 	Description : the message handler function of camera win
//	Global resource dependence : 
//  Author: zack
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHeroCameraWinMsg( 
											MMI_WIN_ID_T win_id, 
											MMI_MESSAGE_ID_E msg_id, 
											DPARAM param
											)
{
    GUI_COLOR_T  color = MMI_BLACK_COLOR;
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO dev_info_ptr = {0};
#if (HERO_SPR_VERSION < 0x10A1140)
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
#endif
	MMIHR_Trace(("enter HandleHeroCameraWinMsg() msg_id = 0x%x, param = 0x%x", msg_id, param));
    
	switch(msg_id)
	{	    
	case MSG_OPEN_WINDOW:
#ifdef WIN32
        GUIBLOCK_SetType(0, color, GUIBLOCK_TYPE_OSD, &dev_info_ptr);
#else
        GUIBLOCK_SetType(255, color, GUIBLOCK_TYPE_COMMON, &dev_info_ptr);
#endif
#if (HERO_SPR_VERSION < 0x10A1140)
        GUIBLOCK_ClearRect(&dev_info_ptr);
        GUIBLOCK_SetRect(full_rect, &dev_info_ptr);
        GUIBLOCK_ClearByColor(MMI_BLACK_COLOR,  &dev_info_ptr);		
        GUIBLOCK_Enable(&dev_info_ptr);  
#endif
		break;
        
	case MSG_CLOSE_WINDOW:
        hr_camera_destroy(0);
		break;
		
	case MSG_LOSE_FOCUS:
        hr_camera_pause(0);
		MMIHEROAPP_pauseApp();
		break;

	case MSG_GET_FOCUS:
		MMIHEROAPP_resumeApp();
        hr_camera_resume(0);
		break;

    case MSG_TIMER:
        hr_camera_handle_timer(*(uint8*)param);
        break;
        
#ifdef HERO_HANDSET_IS_SUPPORT_TOUCHPANEL
	case MSG_TP_PRESS_DOWN:
		recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_DOWN, (void*)param);
		break;
	case MSG_TP_PRESS_UP:
		recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_UP, (void*)param);
		break;
	case MSG_TP_PRESS_MOVE:
		recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_MOUSE_MOVE, (void*)param);
		break;
#endif
		
	case MSG_KEYPRESSUP_RED:
	case MSG_KEYDOWN_RED:
	case MSG_KEYUP_RED://2007add
    	MMIHR_Trace(("mrapp HandleHeroCameraWinMsg() msg_id = 0x%x, s_hr_fbhup2idle = %d", msg_id, s_hr_fbhup2idle));
		if(s_hr_fbhup2idle != 1)	
        {      
			recode = MMI_RESULT_FALSE;
        }
        else
        {
            recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_KEY_NORMAL, (void*)&msg_id);
        }
		break;

	default:
		if(s_hr_VMReady)
		{
			recode = ProcessHeroAppKeyMsg(MMI_HERO_APP_KEY_NORMAL, (void*)&msg_id);
		}
		break;
	}

	MMIHR_Trace(("exit HandleHeroCameraWinMsg() msg_id = 0x%x, recode = %d", msg_id, recode));
	return recode;
}
#endif

LOCAL void HeroShowFactoryInfo(void)
{
    hr_userinfo usrinfo;
    MMI_STRING_T    text_s = {0};
    uint16           uint16_str[500 + 1] = {0};
    uint16          uint16_str_len = 0;
    uint16          temp_len = 0;
    uint8       temp_data[500+1] = {0};
    uint8       u8_data[500+1] = {0};
    uint8       u8_data2[500+1] = {0};
    char temp[256]={0};
    int templen=0;	
	uint8  temp_bufin[34] = {0};
	uint8 d[2] = {'b', 0};
	int32 len = 0;
	 struct{
		 uint32 total; 
		 uint32 tunit; 
		 uint32 account; 
		 uint32 unit; 
	 }*space = NULL;

    hr_getUserInfo(&usrinfo);	
    templen=hr_get_local_version(temp, sizeof(temp) - 1);

    memset(temp_data, 0, sizeof(temp_data));

    hr_setting_get_handsetID_ex(temp_bufin,34,NULL,NULL,NULL);

    hr_fs_get_free_space((uint8*)d, sizeof(d), (uint8**)&space, &len, NULL);	
	
    sprintf((char*)temp_data, "FactoryId:%s_%s\nHandsetId:%s\nExt HandsetId:%s\nSmartID:%s\nimsi:%s\nimei:%s\nbuildtime:%s\nfree:%d\ntotal:%d", 
		                  usrinfo.manufactory, 
		                  HERO_ENGINE_MANUFACTORY_EX, 
		                  usrinfo.type, temp_bufin,
		                   __HR_SMART_AUTH_ID__, 
		                   usrinfo.IMSI, usrinfo.IMEI, 
		                   VERSION_GetInfo(BUILD_TIME),
		                   space->account,
		                   space->total
		                   );

    strcat(u8_data, temp_data);

    temp_len = SCI_STRLEN((char*)u8_data);       
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, u8_data, temp_len );  
    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;

    GUITEXT_SetString(HERO_APP_SHOWFACTORYINFO_TEXT_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
}

LOCAL MMI_RESULT_E HandleHeroShowFactoryInfoWinMsg( 
											MMI_WIN_ID_T win_id, 
											MMI_MESSAGE_ID_E msg_id, 
											DPARAM param
											)
{
    MMI_STRING_T            text = {0};
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    GUI_RECT_T                rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
	 case MSG_GET_FOCUS:
		    //MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(TXT_HERO_SHOW_FACTORY_INFO);
            break;

        case MSG_FULL_PAINT:
        {
            GUIRES_DisplayImg(PNULL, &rect, &rect, win_id, IMAGE_COMMON_BG, &lcd_dev_info);
            //#if !defined(PLATFORM_UIX8910)	
            //GUIWIN_SetTitleTextId(win_id, TXT_HERO_SHOW_FACTORY_INFO, FALSE);
	     //#endif	
            HeroShowFactoryInfo();	
            MMK_SetAtvCtrl(win_id, HERO_APP_SHOWFACTORYINFO_TEXT_CTRL_ID);

            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, (MMI_TEXT_ID_T)TXT_NULL, STXT_RETURN, FALSE);
            //GUIWIN_SetSoftkeyTextId(win_id,  STXT_SOFTKEY_OK_MK, (MMI_TEXT_ID_T)TXT_NULL, STXT_RETURN, FALSE);
         }
        break;

        case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	case MSG_APP_WEB:
        case MSG_NOTIFY_MIDSK:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
		break;    
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
    return (result);
}

PUBLIC BOOLEAN MMIHERO_ShowFactoryInfoWin(void)
{
    return MMK_CreateWin((uint32*)MMIHEROAPP_SHOWFACTORYINFO_TEXT_WIN_TAB,PNULL);
}

//modify 20181114 begin
PUBLIC void MMIHERO_StopSmartTimer()
{
	if (s_smartTimer > 0)
	{
		MMK_StopTimer(s_smartTimer);
		s_smartTimer = 0;
	}
}

PUBLIC uint8 MMIHERO_StartSmartTimer(uint16 t)
{
	MMIHERO_StopSmartTimer();
	s_smartTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, t, FALSE);
	return s_smartTimer;
}

PUBLIC uint8 MMIHERO_StopCallTimer()
{
	if(s_callTimer > 0)
	{
		MMK_StopTimer(s_callTimer);
		s_callTimer = 0;
	}
}

PUBLIC uint8 MMIHERO_StartCallTimer()
{
	if(s_callTimer > 0)
	{
		MMK_StopTimer(s_callTimer);
	}
	s_callTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, 50, FALSE);
	return s_callTimer;
}
//modify 20181114 end

///mike.song add by 20140610
#ifdef HERO_ENGINE_SUPPORT
/**********************************************************************
*							DSM										    *
***********************************************************************/
PUBLIC void MMIHERO_StopDsmTimer(void)
{
	if (s_heroTimer > 0)
	{
		MMK_StopTimer(s_heroTimer);
		s_heroTimer = 0;
	}
}

PUBLIC uint8 MMIHERO_startDsmTimer(uint16 t)
{
	MMIHERO_StopDsmTimer();
	s_heroTimer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, t, TRUE);
	return s_heroTimer;
}
#endif
///mike.song add by 20140610

#endif//#_HERO_ENGINE_WINTAB_C_
#endif//#end HERO_ENGINE_SUPPORT

