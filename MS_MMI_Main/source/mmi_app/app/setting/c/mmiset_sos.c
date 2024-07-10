/*****************************************************************************
** File Name:      mmiset_sos.c                                          *
** Author:                                                                   *
** Date:           11/04/2016                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe sos manager                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2016    Arvin.zhang       Create
******************************************************************************/
#ifdef MMI_INDIAN_SOS_SUPPORT
#define _MMISET_SOS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilcd.h"
#include "guianim.h"
#include "guitext.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_textfun.h"
#include "mmi_image.h"
#include "mmiidle_export.h"
#include "mmiset_display.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmi_nv.h"
#include "mmiset_text.h"
#include "mmiset_wintab.h"
#include "mmiset_image.h"
#include "mmiset_func.h"
#include "mmiset_internal.h"
#include "mmiset_menutable.h"
#include "mmi_wallpaper_export.h"
#include "mmiudisk_export.h"
#include "mmi_resource.h"
#include "guibutton.h" //for pclint warning
#include "guisetlist.h"
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmipb_export.h"
#include "mmisms_export.h"
#include "dal_dcamera.h"
#include "mmidc_export.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//#define MMISET_SOS_CONTACTS_MAX_NUM 5 /*five sos contacts at most according to the Req*/
#define MMISET_SOS_MAIN_MEUN_NUM 3
#ifdef MMIDC_F_FLASH
#define MMISET_SOS_LED_FLASH_TIMEOUT 200//ms
#define MMISET_SOS_TYPE_SETTINGS_NUM 3//sos_switch + xxx(sos_msg_sent_switch)xxx + sos_ring_alert + sos_led_alert
#else
#define MMISET_SOS_TYPE_SETTINGS_NUM 2//sos_switch + xxx(sos_msg_sent_switch)xxx + sos_ring_alert
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
// extern const MMISET_FIXED_ANIM_WALLPAPER_T g_fixed_anim_wallpaper[];
//extern void LCD_SetBackLightBrightness( uint32  brightless );
//extern uint16 g_power_animate_index;

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint16 g_cur_operating_number_index = 0;
#ifdef MMIDC_F_FLASH
LOCAL BOOLEAN s_sos_led_flash_switch = TRUE;
LOCAL uint8   s_sos_led_flash_timer = 0;
#endif
LOCAL BOOLEAN s_is_sos_running = FALSE;
LOCAL BOOLEAN s_sos_message_sent_delay_flag = FALSE;
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static BOOLEAN s_sos_enabled = TRUE;
static BOOLEAN s_sos_msg_sent_enabled = TRUE;
static BOOLEAN s_sos_led_alert_enabled = TRUE;
static BOOLEAN s_sos_ring_alert_enabled = TRUE;
static MMISET_SOS_MESSAGE_CONTENT_T s_sos_msg_info = {0};/*sos message content and its length*/
static MMISET_SOS_CONTACT_ITEM_T s_sos_contact_list[MMISET_SOS_CONTACTS_MAX_NUM] = {0}; /*sos number list*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : judge if there's emergency number existes in contact list or not 
//	Global resource dependence : none
//	Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEmergencyNumberExistForMessageSent(void);

/*****************************************************************************/
// 	Description : SetSOSDelaySentFlag 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSMessageDelaySentFlag(BOOLEAN is_enabled);

/*****************************************************************************/
// 	Description : GetSOSDelaySentFlag
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSOSMessageDelaySentFlag(void);

/*****************************************************************************/
// 	Description : set sos switch on/off 
//	Global resource dependence : none
//    Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSSwitchState(BOOLEAN is_enabled);

#ifdef MMIDC_F_FLASH
/*****************************************************************************/
// 	Description : set sos led alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSLEDAlertSwitchState(BOOLEAN is_enabled);

/*****************************************************************************/
//  Description: toggle sos alert flashing
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void toggleFlash(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description: start sos led light alert flashing  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void startSOSLEDAlert(void);

/*****************************************************************************/
//  Description: stop sos led light alert flashing  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void StopSOSLEDAlert(void);
#endif

/*****************************************************************************/
// 	Description : set sos ring alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSRingAlertSwitchState(BOOLEAN is_enabled);

/*****************************************************************************/
// 	Description : set sos msg sent switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSMsgSentSwitchState(BOOLEAN is_enabled);

/*****************************************************************************/
// 	Description : set sos msg info 
//	Global resource dependence : s_sos_msg_info
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSOSMsgContent(MMISET_SOS_MESSAGE_CONTENT_T* msg_info_ptr);

/*****************************************************************************/
// 	Description : set sos contact list info 
//	Global resource dependence : s_sos_contact_list
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSOSContactInfoByIndex(uint16 index, MMISET_SOS_CONTACT_ITEM_T* contact_item_ptr);

/*****************************************************************************/
// 	Description : passed to sos call func as the callback
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void CancelButtonCallback(BOOLEAN is_need_exit_sos);

/*****************************************************************************/
// 	Description : stop sos procedures (stop flash light and stop the alert ring) 
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void StopSOSProcedures(void);

/*****************************************************************************/
//  Description: the callback func for sos message sending  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void SOSMessageSentCallback(BOOLEAN is_succss, DPARAM parm);

/*****************************************************************************/
//  Description: send sos message to every emergency contact  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void SendSOSMessage(void);

/*****************************************************************************/
//  Description: load and get sos help info string  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note: HAVE TO free memory by caller!!!
//
/*****************************************************************************/
LOCAL void GetSOSAboutInfoString(MMI_STRING_T *info_str);

/*****************************************************************************/
//  Description: Handle sos help info win msg
//  Global resource dependence: none 
//  Author: Arvin.zhang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSOSAboutInfoTextBoxWinMsg(
											      MMI_WIN_ID_T win_id, 
											      MMI_MESSAGE_ID_E msg_id, 
											      DPARAM param 
											      );

/*****************************************************************************/
//  Description :Init SOS Type Settings List Ctrl
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void  InitSOSTypeSettingsListCtrl(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle sos type settings win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSOSTypeSettingsWin(
	MMI_WIN_ID_T    win_id, 
	MMI_MESSAGE_ID_E   msg_id, 
	DPARAM             param
);

/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSOSMessageEditBoxWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : set list from PB  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSOSNumberSelectedFromPB(MMIPB_HANDLE_T handle);

/*****************************************************************************/
// 	Description : set sos number select contact callback
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
LOCAL void SetSOSNumberSelectContactCallback(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description : sos number editing window
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNumberEditBoxWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : update list
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL void UpdateNumberListUI(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : append list
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendNumberListItem(
                                MMI_CTRL_ID_T             ctrl_id,         //要设置的列表控件ID号 (输入参数)
                                uint16                    pos,
                                BOOLEAN                   is_update
                                );

/*****************************************************************************/
//  Description : to handle the sos number list option window
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNumberlistOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : handle number list win
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetSOSNumberListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: 窗口的ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: 窗口的内部消息ID
                                            DPARAM                 param        //IN: 相应消息的参数
                                            );

/*****************************************************************************/
//  Description: Handle sos settings main menu win msg  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
//the window for sos main menu
WINDOW_TABLE( MMISET_SET_SOS_WIN_TAB ) = 
{
    WIN_TITLE(TXT_SETTINGS_SOS),
    WIN_FUNC((uint32)HandleMainMenuWinMsg),    
    WIN_ID(MMISET_SET_SOS_WIN_ID),
    CREATE_MENU_CTRL(MENU_SET_SOS_LIST_OPT, MMISET_SOS_MAINMENU_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN  
};

//the window for sos number list win
WINDOW_TABLE( MMISET_SET_SOS_NUMBER_LIST_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SET_SOS_NUMBER_LIST),
    WIN_FUNC((uint32) HandleSetSOSNumberListWindow),    
    WIN_ID( MMISET_SET_SOS_NUMBER_LIST_WIN_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif    
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISET_SOS_NUMBERLIST_LIST_CTRL_ID),
    END_WIN
};

//the window for sos number list opt win
WINDOW_TABLE( MMISET_SET_SOS_NUMBER_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleNumberlistOptWinMsg),
    WIN_ID(MMISET_SET_SOS_NUMBER_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SOS_NUMBERLIST_OPT,MMISET_SOS_NUMBERLIST_LIST_OPT_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif
    END_WIN
};

WINDOW_TABLE(MMISET_SET_SOS_NUMBER_EDITBOX_WIN_TAB) = 
{
    WIN_ID( MMISET_SET_SOS_NUMBER_EDITBOX_WIN_ID),
    WIN_TITLE(TXT_COMMON_NUMBER),    
    WIN_FUNC((uint32)HandleNumberEditBoxWinMsg),
#ifdef MMI_PDA_SUPPORT
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_SOS_NUMBER_EDIT_FORM_CTRL_ID),
	CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_PHONE_NUM_MAX_LEN,MMISET_SOS_NUMBER_EDIT_CTRL_ID, MMISET_SOS_NUMBER_EDIT_FORM_CTRL_ID),
	CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMISET_SOS_NUMBER_EDIT_SOFTKEY_CTRL_ID, MMISET_SOS_NUMBER_EDIT_FORM_CTRL_ID),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_EDIT_PHONENUM_CTRL(MMISET_PHONE_NUM_MAX_LEN, MMISET_SOS_NUMBER_EDIT_CTRL_ID),
#endif
    END_WIN
};

//the window for sos message content edit
WINDOW_TABLE( MMISET_SET_SOS_MESSAGE_CONTENT_EDIT_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SET_SOS_MESSAGE_SETTINGS ),
    WIN_FUNC((uint32) HandleSOSMessageEditBoxWinMsg),    
    WIN_ID( MMISET_SET_SOS_MESSAGE_CONTENT_EDIT_WIN_ID),
#ifdef MMI_PDA_SUPPORT
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_SOS_MSG_CONTENT_EDIT_FORM_CTRL_ID),
	CHILD_EDIT_TEXT_CTRL(TRUE,MMISET_SOS_MSG_CONTENT_MAX_LEN,MMISET_SOS_MSG_CONTENT_EDIT_CTRL_ID, MMISET_SOS_MSG_CONTENT_EDIT_FORM_CTRL_ID),
	CHILD_SOFTKEY_CTRL(STXT_EXIT, STXT_SAVE, STXT_RETURN,MMISET_SOS_MSG_CONTENT_EDIT_SOFTKEY_CTRL_ID, MMISET_SOS_MSG_CONTENT_EDIT_FORM_CTRL_ID),
#else
	WIN_SOFTKEY(STXT_EXIT, STXT_SAVE, STXT_RETURN),
	CREATE_EDIT_TEXT_CTRL(MMISET_SOS_MSG_CONTENT_MAX_LEN, MMISET_SOS_MSG_CONTENT_EDIT_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISET_SET_SOS_TYPE_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSOSTypeSettingsWin),    
    WIN_ID( MMISET_SET_SOS_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_SOS_SETTINGS),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SOS_SETTINGS_TYPE_LIST_CTRL_ID),
    END_WIN
};

//the window showing help info of sos 
WINDOW_TABLE(MMISET_SET_SOS_ABOUT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleSOSAboutInfoTextBoxWinMsg),    
    WIN_ID(MMISET_SET_SOS_ABOUT_WIN_ID),
    WIN_TITLE(TXT_HELP),
    CREATE_TEXT_CTRL(MMISET_SOS_ABOUT_TEXT_BOX_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_EXIT),
#endif
    END_WIN    
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : judge if there's emergency number existes in contact list or not 
//	Global resource dependence : none
//	Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEmergencyNumberExistForMessageSent(void)
{
	uint8 index = 0;
	BOOLEAN is_existed = FALSE;
	uint8 sos_send_num[MMISET_PHONE_NUM_MAX_LEN] = {0};
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

	if(MMIAPIPHONE_GetSimAvailableNum(&dual_sys, 1))
	{
		dual_sys = MMIAPISET_GetActiveSim();

		for(index = 0; index < MMISET_SOS_CONTACTS_MAX_NUM; index++)
		{
			if(s_sos_contact_list[index].num_len > 0)
			{
				SCI_MEMSET(sos_send_num, 0, MMISET_PHONE_NUM_MAX_LEN);
				MMIAPICOM_WstrToStr(s_sos_contact_list[index].num, sos_send_num);
				if(MMIAPICC_IsEmergencyNum(sos_send_num, s_sos_contact_list[index].num_len, dual_sys, FALSE))
				{
					is_existed = TRUE;
					break;
				}
			}
		}
	}

	SCI_TRACE_LOW("[SOS]IsEmergencyNumberExistForMessageSent result = %d", is_existed);
	return is_existed;
}

/*****************************************************************************/
// 	Description : SetSOSDelaySentFlag 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSMessageDelaySentFlag(
                              BOOLEAN is_enabled
                              )
{
    s_sos_message_sent_delay_flag = is_enabled;
    SCI_TRACE_LOW("[SOS]SetSOSDelaySentFlag to %d", is_enabled);
}

/*****************************************************************************/
// 	Description : GetSOSDelaySentFlag
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSOSMessageDelaySentFlag(void)
{
    SCI_TRACE_LOW("[SOS]GetSOSDelaySentFlag %d", s_sos_message_sent_delay_flag);
    return s_sos_message_sent_delay_flag;
}

/*****************************************************************************/
// 	Description : set sos switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSSwitchState(
                              BOOLEAN is_enabled
                              )
{
    s_sos_enabled = is_enabled;
    MMINV_WRITE(MMINV_SET_SOS_SWITCH, &s_sos_enabled);
    SCI_TRACE_LOW("[SOS]SetSOSSwitchState to %d", is_enabled);
}

#ifdef MMIDC_F_FLASH
/*****************************************************************************/
// 	Description : set sos led alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSLEDAlertSwitchState(BOOLEAN is_enabled)
{
    s_sos_led_alert_enabled = is_enabled;
    MMINV_WRITE(MMINV_SET_SOS_LED_ALERT_SWITCH, &s_sos_led_alert_enabled);
    SCI_TRACE_LOW("[SOS]SetSOSLEDAlertSwitchState to %d", is_enabled);
}

/*****************************************************************************/
//  Description: toggle sos alert flashing
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void toggleFlash(uint8 timer_id, uint32 param)
{
    BOOLEAN is_flash_on = !s_sos_led_flash_switch;
#ifndef WIN32
    GPIO_SetFlashLight(is_flash_on);
#endif
    s_sos_led_flash_switch = is_flash_on;
    SCI_TRACE_LOW("[SOS] toggleFlash set flash  to %d", is_flash_on);
}

/*****************************************************************************/
//  Description: start sos led light alert flashing  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void startSOSLEDAlert(void)
{
    SCI_TRACE_LOW("[SOS] startSOSLEDAlert enter.");
	if(s_sos_led_alert_enabled)
	{
		s_sos_led_flash_switch = TRUE;
		#ifndef WIN32
	    GPIO_SetFlashLight(s_sos_led_flash_switch);
		#endif

		if(s_sos_led_flash_timer)
		{
			MMK_StopTimer(s_sos_led_flash_timer);   
			s_sos_led_flash_timer = 0;    
		}
		s_sos_led_flash_timer = MMK_CreateTimerCallback(MMISET_SOS_LED_FLASH_TIMEOUT, toggleFlash, 0, TRUE);
	}
}

/*****************************************************************************/
//  Description: stop sos led light alert flashing  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void StopSOSLEDAlert(void)
{
    SCI_TRACE_LOW("[SOS] StopSOSLEDAlert enter.");
	if(s_sos_led_alert_enabled)
	{
	    s_sos_led_flash_switch = FALSE;

	    if(s_sos_led_flash_timer)
	    {
	        MMK_StopTimer(s_sos_led_flash_timer);   
			s_sos_led_flash_timer = 0;    
	    }
		#ifndef WIN32
	    GPIO_SetFlashLight(s_sos_led_flash_switch);
		#endif
	}
}
#endif

/*****************************************************************************/
// 	Description : start sos ring alert 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void StartSOSRingAlert()
{
    if(s_sos_ring_alert_enabled && s_is_sos_running)
    {
        SCI_TRACE_LOW("[SOS]StartSOSRingAlert");
        MMIAPISET_PlayRing((MN_DUAL_SYS_E)0, FALSE, 0, 0xffff, MMISET_RING_TYPE_SOS, PNULL);
        //MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
    }
}

/*****************************************************************************/
// 	Description : stop sos ring alert 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void StopSOSRingAlert()
{
    if(s_sos_ring_alert_enabled)
    {
        SCI_TRACE_LOW("[SOS]StopSOSRingAlert");
        MMIAPISET_StopRing(MMISET_RING_TYPE_SOS);
    }
}

/*****************************************************************************/
// 	Description : set sos ring alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSRingAlertSwitchState(BOOLEAN is_enabled)
{
    s_sos_ring_alert_enabled = is_enabled;
    MMINV_WRITE(MMINV_SET_SOS_RING_ALERT_SWITCH, &s_sos_ring_alert_enabled);
    SCI_TRACE_LOW("[SOS]SetSOSRingAlertSwitchState to %d", is_enabled);
}

/*****************************************************************************/
// 	Description : set sos msg sent switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void SetSOSMsgSentSwitchState(
                              BOOLEAN is_enabled
                              )
{
    s_sos_msg_sent_enabled = is_enabled;
    MMINV_WRITE(MMINV_SET_SOS_MSG_SENT_SWITCH, &s_sos_msg_sent_enabled);
    SCI_TRACE_LOW("[SOS]SetSOSMsgSentSwitchState to %d", is_enabled);
}

/*****************************************************************************/
// 	Description : set sos msg info 
//	Global resource dependence : s_sos_msg_info
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSOSMsgContent(
                              MMISET_SOS_MESSAGE_CONTENT_T* msg_info_ptr
                              )
{
    BOOLEAN result = FALSE;
	//uint16 msg_content_len = 0;
	//wchar msg_content[MMISET_SOS_MSG_CONTENT_MAX_LEN + 1] = {0};

    if((PNULL != msg_info_ptr) && (msg_info_ptr->msg_len > 0)) //empty is invalid for sos scene
    {
		//MMI_WSTRNCPY(msg_content, MMISET_SOS_MSG_CONTENT_MAX_LEN, msg_info_ptr->msg_content, msg_info_ptr->msg_len, msg_info_ptr->msg_len);
		//msg_content_len = MIN(MMISET_SOS_MSG_CONTENT_MAX_LEN, msg_info_ptr->msg_len);

		//reset the buffer of the previous msg content
        SCI_MEMSET(s_sos_msg_info.msg_content, 0X00, sizeof(s_sos_msg_info.msg_content));
		MMI_WSTRNCPY(s_sos_msg_info.msg_content, MMISET_SOS_MSG_CONTENT_MAX_LEN, msg_info_ptr->msg_content, msg_info_ptr->msg_len, msg_info_ptr->msg_len);
		s_sos_msg_info.msg_len = MIN(MMISET_SOS_MSG_CONTENT_MAX_LEN, msg_info_ptr->msg_len);

		//update nv item
		MMINV_WRITE(MMINV_SET_SOS_MSG_INFO, &s_sos_msg_info);

		SCI_TRACE_LOW("[SOS]SetSOSMsgContent len = %d, new content = %s", s_sos_msg_info.msg_len, s_sos_msg_info.msg_content);
		result = TRUE;
    }

    SCI_TRACE_LOW("[SOS]SetSOSMsgContent result = %d", result);
    return result;
}

/*****************************************************************************/
// 	Description : set sos contact list info 
//	Global resource dependence : s_sos_contact_list
//    Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSOSContactInfoByIndex(
                              uint16 index,
                              MMISET_SOS_CONTACT_ITEM_T* contact_item_ptr
                              )
{
    BOOLEAN result = FALSE;

    if((PNULL != contact_item_ptr) && (index < MMISET_SOS_CONTACTS_MAX_NUM))
    {
		//reset the buffer of the current contact number and replace with the new one
		SCI_MEMSET(s_sos_contact_list[index].num, 0X00, MMISET_PHONE_NUM_MAX_LEN);
		MMI_WSTRNCPY(s_sos_contact_list[index].num, MMISET_PHONE_NUM_MAX_LEN, contact_item_ptr->num,  contact_item_ptr->num_len, contact_item_ptr->num_len);
		s_sos_contact_list[index].num_len = MIN(MMISET_PHONE_NUM_MAX_LEN, contact_item_ptr->num_len);

		//update contact list nv overall
		MMINV_WRITE(MMINV_SET_SOS_CONTACT_LIST, &s_sos_contact_list);

		SCI_TRACE_LOW("[SOS]SetSOSContactInfoByIndex index = %d, num_len = %d, new_number = %s", index, s_sos_contact_list[index].num_len, s_sos_contact_list[index].num);
		result = TRUE;
    }

    SCI_TRACE_LOW("[SOS]SetSOSContactInfoByIndex result = %d", result);
    return result;
}

/*****************************************************************************/
// 	Description : passed to sos call func as the callback
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void CancelButtonCallback(BOOLEAN is_need_exit_sos)
{
    SCI_TRACE_LOW("[SOS]CancelButtonCallback enter, is_need_exit_sos = %d", is_need_exit_sos);

	if (GetSOSMessageDelaySentFlag())
	{
		//sent the emergency msg if needed
		SendSOSMessage();    
	}

    if (!is_need_exit_sos)
    {
        SCI_TRACE_LOW("[SOS]:CancelButtonCallback keep sos running!");
		MMK_CreateTimerCallback(2500, StartSOSRingAlert, 0, 0);
        return;
    }

    if(MMK_IsOpenWin(MMISET_SET_SOS_ABOUT_WIN_ID))
    {
        MMK_PostMsg(MMISET_SET_SOS_ABOUT_WIN_ID, MSG_APP_CANCEL, PNULL, NULL);
    }
    else
    {
        StopSOSProcedures();
    }
}

/*****************************************************************************/
// 	Description : stop sos procedures (stop flash light and stop the alert ring) 
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
LOCAL void StopSOSProcedures(void)
{
    SCI_TRACE_LOW("[SOS]StopSOSProcedures enter s_is_sos_running = %d", s_is_sos_running);

    if(s_is_sos_running)
    {
        s_is_sos_running = FALSE;
        StopSOSRingAlert();
        #ifdef MMIDC_F_FLASH
        StopSOSLEDAlert();
        #endif
		SetSOSMessageDelaySentFlag(FALSE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        SCI_TRACE_LOW("[SOS]StopSOSProcedures end_time = %d", SCI_GetTickCount());
    }
}

/*****************************************************************************/
//  Description: the callback func for sos message sending  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void SOSMessageSentCallback(BOOLEAN is_succss, DPARAM parm)
{
    //do nothing here temporarily for lower time consuming in case of an emergency.
}

/*****************************************************************************/
//  Description: send sos message to every emergency contact  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL void SendSOSMessage(void)
{
    uint8 index = 0;
    BOOLEAN result = FALSE;
    MMI_STRING_T sos_msg_str = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMISMS_MULTI_SEND_DATA_T send_data = {0};
    uint8 contacts_index = 0;
    uint8 sos_send_num[MMISET_PHONE_NUM_MAX_LEN] = {0};

    if(!s_sos_msg_sent_enabled)
    {
        SCI_TRACE_LOW("[SOS] SendSOSMessage exit for sos msg sent is disabled.");
        return;
    }

    if(MMIAPIPHONE_GetSimAvailableNum(&dual_sys, 1))
    {
        dual_sys = MMIAPISET_GetActiveSim();
    	 sos_msg_str.wstr_len = s_sos_msg_info.msg_len;               
    	 //sos_msg_str.wstr_ptr = (wchar *)s_sos_msg_info.msg_content;
    	 sos_msg_str.wstr_ptr = (wchar*)SCI_ALLOCAZ(sizeof(wchar)*sos_msg_str.wstr_len);
        if(PNULL == sos_msg_str.wstr_ptr)
        {
            SCI_TRACE_LOW("SOSMessageSentCallback memory alloc failed.");
            return;
        }
        SCI_MEMSET(sos_msg_str.wstr_ptr, 0x00, sizeof(wchar)*sos_msg_str.wstr_len);
        //SCI_MEMCPY(sos_msg_str.wstr_ptr, (wchar *)s_sos_msg_info.msg_content,sos_msg_str.wstr_len);
        
        MMIAPICOM_Wstrncpy(sos_msg_str.wstr_ptr, s_sos_msg_info.msg_content, sos_msg_str.wstr_len);

    	 send_data.dual_sys = (MN_DUAL_SYS_E)dual_sys;
        send_data.sms_content_ptr = &sos_msg_str;
        send_data.is_need_packet = TRUE;
        send_data.send_callback = PNULL;
        for(index = 0; index < MMISET_SOS_CONTACTS_MAX_NUM; index++)
        {
            if(s_sos_contact_list[index].num_len > 0)
            {
                SCI_MEMSET(sos_send_num, 0, MMISET_PHONE_NUM_MAX_LEN);
                MMIAPICOM_WstrToStr(s_sos_contact_list[index].num, sos_send_num);
                if(MMIAPICC_IsEmergencyNum(sos_send_num, s_sos_contact_list[index].num_len, dual_sys, FALSE))
                {
                    //If the current number is emergency type, it can't send sms message and make a call to this number simultaneously.
                    //The two actions are in conflit therefore making call to emergency number is failed.
                    continue;
                }
                MMI_WSTRNTOSTR((uint8 *)send_data.dest_num[contacts_index], MMISMS_NUMBER_MAX_LEN + 2,
                s_sos_contact_list[index].num, s_sos_contact_list[index].num_len, s_sos_contact_list[index].num_len);
                SCI_TRACE_LOW("[SOS]sendSOSMessage via sim %d to number %s.", dual_sys, (uint8 *)send_data.dest_num[contacts_index]);
                contacts_index++;
            }  
        }
        send_data.dest_num_len = contacts_index;
        result = MMIAPISMS_MultiSendSms(&send_data);
        SCI_TRACE_LOW("[SOS]sendSOSMessage result %d.", result);
        if(PNULL != sos_msg_str.wstr_ptr)
        {
            SCI_FREE(sos_msg_str.wstr_ptr);
        }
    }
    else
    {
    	SCI_TRACE_LOW("[SOS]sendSOSMessage failed for there's no available sim card.");
    }

}

/*****************************************************************************/
//  Description: load and get sos help info string  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note: HAVE TO free memory by caller!!!
//
/*****************************************************************************/
LOCAL void GetSOSAboutInfoString(MMI_STRING_T *info_str)
{
    uint16  lf_len = 2;
    uint16  str_len = 0;
    uint16  u16lf[] = {0x0A,0x0D,'\0'};
    //wchar   *lf_ptr = NULL;
    wchar   *str_ptr = NULL;
    MMI_STRING_T            number_list_help_str = {0};
    MMI_STRING_T            message_settings_help_str = {0};
    MMI_STRING_T            type_settings_help_str = {0};

    MMI_GetLabelTextByLang(TXT_SET_SOS_HELP_NUMBER_LIST, &number_list_help_str);
    MMI_GetLabelTextByLang(TXT_SET_SOS_HELP_MESSAGE_SETTINGS, &message_settings_help_str);
    MMI_GetLabelTextByLang(TXT_SET_SOS_HELP_TYPE_SETTINGS, &type_settings_help_str);

    str_len = number_list_help_str.wstr_len + message_settings_help_str.wstr_len
		+ type_settings_help_str.wstr_len + MMISET_SOS_MAIN_MEUN_NUM * (lf_len + 1);
    str_ptr = (wchar *)SCI_ALLOC_APPZ(str_len*sizeof(wchar));

    if(PNULL != str_ptr)
    {
        SCI_MEMSET(str_ptr, 0x00, str_len*sizeof(wchar));
   	info_str->wstr_ptr = str_ptr;
    	info_str->wstr_len = 0;

        MMIAPICOM_Wstrncat(info_str->wstr_ptr, number_list_help_str.wstr_ptr, number_list_help_str.wstr_len);
        info_str->wstr_len += number_list_help_str.wstr_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, lf_len);
        info_str->wstr_len += lf_len;

        MMIAPICOM_Wstrncat(info_str->wstr_ptr, message_settings_help_str.wstr_ptr, message_settings_help_str.wstr_len);
        info_str->wstr_len += message_settings_help_str.wstr_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, lf_len);
        info_str->wstr_len += lf_len;

        MMIAPICOM_Wstrncat(info_str->wstr_ptr, type_settings_help_str.wstr_ptr, type_settings_help_str.wstr_len);
        info_str->wstr_len += type_settings_help_str.wstr_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, lf_len);
        info_str->wstr_len += lf_len;
    }
}

/*****************************************************************************/
//  Description: Handle sos help info win msg
//  Global resource dependence: none 
//  Author: Arvin.zhang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSOSAboutInfoTextBoxWinMsg(
                                                      MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param 
                                                      )
{  
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMISET_SOS_ABOUT_TEXT_BOX_CTRL_ID;
    MMI_STRING_T            content_str = {0};

    SCI_TRACE_LOW("[SOS]HandleSOSAboutInfoTextBoxWinMsg, msg_id = 0x%x",msg_id);

    switch( msg_id ) 
    {
    case MSG_OPEN_WINDOW:
        if((BOOLEAN)MMK_GetWinAddDataPtr(win_id))
        {
            MMI_GetLabelTextByLang(TXT_SET_SOS_TITLE, &content_str);
            GUIWIN_SetTitleText(win_id, content_str.wstr_ptr, content_str.wstr_len, FALSE);
			SCI_TRACE_LOW("[SOS]HandleSOSAboutInfoTextBoxWinMsg open for sos running.");
        }
        else
        {
            GetSOSAboutInfoString(&content_str.wstr_ptr);
            GUITEXT_SetString(ctrl_id,content_str.wstr_ptr,content_str.wstr_len,FALSE);
            
            if(NULL != content_str.wstr_ptr)
            {
                SCI_FREE(content_str.wstr_ptr);
                content_str.wstr_ptr = NULL;
            }
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_GET_FOCUS:
        if (s_is_sos_running)
        {
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        break;

    case MSG_FULL_PAINT:
        break;

#if defined(MMI_PDA_SUPPORT)
	case MSG_KEYUP_POWER:
#if 0
		//if should not exit sos mode when power key pressed, enable the following code available
		result = MMI_RESULT_TRUE;
		break;
#endif
#endif
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (s_is_sos_running)
        {
            StopSOSProcedures();
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description :Init SOS Type Settings List Ctrl
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void  InitSOSTypeSettingsListCtrl(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    MMI_STRING_T  text_str = {0};
    MMI_IMAGE_ID_T  image_id = IMAGE_NULL;

    GUILIST_SetMaxItem(ctrl_id, MMISET_SOS_TYPE_SETTINGS_NUM, FALSE );   
   
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SETTINGS_SOS,
    text_str,
    (s_sos_enabled ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON),
    (uint32)0);
/*Hide the sos message sending switch from UI according to the latest requirement doc 12/01/2016*/
#if 0    
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_SOS_MESSAGE_SENDING,
    text_str,
    (s_sos_msg_sent_enabled ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON),
    (uint32)0);
#endif//0
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_SOS_RING_ALERT_SETTINGS,
    text_str,
    (s_sos_ring_alert_enabled ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON),
    (uint32)0);

#ifdef MMIDC_F_FLASH
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_SOS_LED_ALERT_SETTINGS,
    text_str,
    (s_sos_led_alert_enabled ? IMAGE_CHECK_SELECTED_ICON : IMAGE_CHECK_UNSELECTED_ICON),
    (uint32)0);
#endif

    if(!s_sos_enabled)
    {
		//skip the first item --- 'sos'
		for(index = 1; index < MMISET_SOS_TYPE_SETTINGS_NUM; index++)
		{
			GUILIST_SetItemGray(ctrl_id, index, TRUE);
			GUILIST_SetItemInvalid(ctrl_id, index, TRUE);   
		} 
    }
}


/*****************************************************************************/
//  Description : handle sos type settings win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSOSTypeSettingsWin(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SOS_SETTINGS_TYPE_LIST_CTRL_ID;
    uint16 index = 0;

    SCI_TRACE_LOW("[SOS]HandleSOSTypeSettingsWin, msg_id = 0x%x",msg_id);
    
    switch(msg_id)
    {
		case MSG_OPEN_WINDOW:
			InitSOSTypeSettingsListCtrl(ctrl_id);
			MMK_SetAtvCtrl(win_id, ctrl_id);
			break;

        //case MSG_GET_FOCUS:
        case MSG_SET_SOS_SETTINGS_TYPE_LIST_UPDATE:
			index = GUILIST_GetCurItemIndex(ctrl_id);        
			GUILIST_RemoveAllItems(ctrl_id);
			InitSOSTypeSettingsListCtrl(ctrl_id);
			GUILIST_SetCurItemIndex(ctrl_id,index);
			MMK_SetAtvCtrl(win_id, ctrl_id);
			break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:                
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
			{
				index = GUILIST_GetCurItemIndex(ctrl_id);
				switch(index)
				{
					case 0:
						SetSOSSwitchState((BOOLEAN)!MMIAPISET_GetSOSSwitchState());
						break;
					#if 0
					case 1:
						SetSOSMsgSentSwitchState((BOOLEAN)!MMIAPISET_GetSOSMsgSentSwitchState());
						break;
					#endif
					case 1:
						SetSOSRingAlertSwitchState((BOOLEAN)!MMIAPISET_GetSOSRingAlertSwitchState());
						break;

					#ifdef MMIDC_F_FLASH
					case 2:
						SetSOSLEDAlertSwitchState((BOOLEAN)!MMIAPISET_GetSOSLEDAlertSwitchState());
						break;
					#endif

					default:
						break;
				}
				MMK_PostMsg(win_id, MSG_SET_SOS_SETTINGS_TYPE_LIST_UPDATE, PNULL, 0);
				MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
			}
			break;
                 
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
                
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSOSMessageEditBoxWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_CTRL_ID_T      ctrl_id = MMISET_SOS_MSG_CONTENT_EDIT_CTRL_ID;
    MMI_HANDLE_T       ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    MMI_RESULT_E        result   = MMI_RESULT_TRUE;
    MMI_STRING_T        str_info = {0};
    MMI_STRING_T        str_t    = {0};
    BOOLEAN             str_is_valid = TRUE;
    MMISET_SOS_MESSAGE_CONTENT_T msg_info = {0};

    SCI_TRACE_LOW("[SOS]HandleSOSMessageEditBoxWinMsg, msg_id = 0x%x",msg_id);

    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		GUIEDIT_SetString(ctrl_id, &s_sos_msg_info.msg_content, s_sos_msg_info.msg_len);
		MMK_SetAtvCtrl(win_id, ctrl_handle); 
		break;        

#if defined MMI_PDA_SUPPORT && defined TOUCH_PANEL_SUPPORT      
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
				case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
				case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID: 
					MMK_SendMsg(win_id,MSG_APP_OK, PNULL);
					break;

				case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
					MMK_SendMsg(win_id,MSG_CTL_MIDSK, PNULL);
					break;
				default:
					break;
            }
		}
		break;	
#endif

    case MSG_CTL_MIDSK:
        // 从EDITBOX中获得数据
        GUIEDIT_GetString(ctrl_id, &str_info);
        
        if(0 == str_info.wstr_len || PNULL == str_info.wstr_ptr)
        {
        	MMIPUB_OpenAlertWarningWin(TXT_COMM_INPUT_NULL);
        }
		else
		{
			msg_info.msg_len = str_info.wstr_len;
			MMI_WSTRNCPY(msg_info.msg_content, MMISET_SOS_MSG_CONTENT_MAX_LEN, str_info.wstr_ptr, str_info.wstr_len, str_info.wstr_len);
			SetSOSMsgContent(&msg_info);
			MMK_CloseWin(win_id);
		}
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
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


/*****************************************************************************/
//  Description : set list from PB  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSameSOSNumberExisted(BOOLEAN is_wchar_buf, void* buffer, uint8 length)
{
    BOOLEAN is_existed = FALSE;
    uint16 index= 0, cmp_count = 0;

    if(PNULL != buffer)
    {
		for(index = 0; index < MMISET_SOS_CONTACTS_MAX_NUM; index++)
		{
			//ignore the current operating item
			if (g_cur_operating_number_index != index)
			{
				cmp_count = (uint32)MAX(s_sos_contact_list[index].num_len, length);
				if(((!is_wchar_buf) && !MMIAPICOM_WstrStrncmp((const wchar *)s_sos_contact_list[index].num, buffer, cmp_count))
					|| (is_wchar_buf && !MMIAPICOM_Wstrcmp((const wchar *)s_sos_contact_list[index].num, buffer)))
				{
					is_existed = TRUE;
					break;
				}
			}
		}
    }

	SCI_TRACE_LOW("[SOS]IsSameSOSNumberExisted is_wchar_buf = %d, is_existed = %d", is_wchar_buf, is_existed);
	return is_existed;

}

/*****************************************************************************/
//  Description : set list from PB  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSOSNumberSelectedFromPB(MMIPB_HANDLE_T handle)
{
    BOOLEAN return_val = TRUE;
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 tele_len = 0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};  
    uint16 index= 0;
    uint32 cmp_count =0;
    MMISET_SOS_CONTACT_ITEM_T new_contact_item = {0};

	//first, get the selected contact's number
	SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));
	MMIAPIPB_GetSelectNumber(handle, &number,MMIPB_BCD_NUMBER_MAX_LEN, 0);
	tele_len = MMIAPIPB_BCDNumberToString(&number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);

    if(0 == tele_len || !MMIAPICOM_IsValidNumberString(&tele_num, tele_len))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_INVALID_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return_val = FALSE;                   
    }
    else
    {
        //then, check if the number exists in current sos number list or not
        //if the new one, save to the nv list
        if(IsSameSOSNumberExisted(FALSE, &tele_num, tele_len))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
        }
        else
        {
            new_contact_item.num_len = tele_len;
            MMI_STRNTOWSTR(
                new_contact_item.num, 
                MMISET_PHONE_NUM_MAX_LEN + 2, 
                tele_num, 
                MMISET_PHONE_NUM_MAX_LEN + 2, 
                tele_len
                );
            SetSOSContactInfoByIndex(g_cur_operating_number_index, &new_contact_item);
            MMK_PostMsg(MMISET_SET_SOS_NUMBER_LIST_WIN_ID, MSG_SET_SOS_NUMBER_LIST_UPDATE, (DPARAM)&g_cur_operating_number_index, sizeof(uint16));
	    }
    }

	return return_val;
}


/*****************************************************************************/
// 	Description : set sos number select contact callback
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
LOCAL void SetSOSNumberSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_SOS_NUMBER_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);      
}

/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNumberEditBoxWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_CTRL_ID_T      ctrl_id = MMISET_SOS_NUMBER_EDIT_CTRL_ID;
    MMI_HANDLE_T       ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    MMI_RESULT_E        result   = MMI_RESULT_TRUE;
    MMI_STRING_T        str_info = {0};
    MMI_STRING_T        str_t    = {0};
    BOOLEAN             str_is_valid = TRUE;  
    MMISET_SOS_CONTACT_ITEM_T new_contact_item = {0};

    SCI_TRACE_LOW("[SOS]HandleNumberEditBoxWinMsg, msg_id = 0x%x",msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIEDIT_SetString(ctrl_id, &s_sos_contact_list[g_cur_operating_number_index].num, s_sos_contact_list[g_cur_operating_number_index].num_len);
		MMK_SetAtvCtrl(win_id, ctrl_handle); 
        break;        

#if defined MMI_PDA_SUPPORT && defined TOUCH_PANEL_SUPPORT      
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
			case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
				MMK_SendMsg(win_id,MSG_APP_OK, PNULL);    
				break;
			case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:             
				MMK_CloseWin(win_id);
				break;
			default:
				break;
            }
		}
		break;	
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // 从EDITBOX中获得数据
        GUIEDIT_GetString(ctrl_id, &str_info);

        if(0 == str_info.wstr_len || !MMIAPICOM_IsValidNumberStringEx(&str_info))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_INVALID_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            str_is_valid = FALSE;                   
        }

        if(str_is_valid)
        {
            if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
            {
                new_contact_item.num_len = str_info.wstr_len; 
                MMI_WSTRNCPY(new_contact_item.num, MMISET_PHONE_NUM_MAX_LEN, str_info.wstr_ptr, str_info.wstr_len, str_info.wstr_len);
            }

			if(IsSameSOSNumberExisted(TRUE, &new_contact_item.num, new_contact_item.num_len))
			{
				MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
			}
			else
			{
				SetSOSContactInfoByIndex(g_cur_operating_number_index, &new_contact_item);
				MMK_PostMsg(MMISET_SET_SOS_NUMBER_LIST_WIN_ID, MSG_SET_SOS_NUMBER_LIST_UPDATE, (DPARAM)&g_cur_operating_number_index, sizeof(uint16));
				MMK_CloseWin(win_id); 
			} 
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
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


/*****************************************************************************/
//  Description : to handle the sos number list option menu window
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleNumberListMenuMsg(
                                 MMI_MENU_ID_T menu_id,    //IN: 被选中的菜单项
                                 MMI_CTRL_ID_T ctrl_id
                                 )
{
    uint32 user_data = 0;

    switch (menu_id)
    {
    case ID_SET_SOS_NUMBER_EDIT:                     
        MMK_CreateWin((uint32 *)MMISET_SET_SOS_NUMBER_EDITBOX_WIN_TAB,PNULL);
        break;
        
    case ID_SET_SOS_NUMBER_CLEAR:
		{
			MMISET_SOS_CONTACT_ITEM_T contact_item = {0};
			SetSOSContactInfoByIndex(g_cur_operating_number_index, &contact_item);
			MMK_PostMsg(MMISET_SET_SOS_NUMBER_LIST_WIN_ID, MSG_SET_SOS_NUMBER_LIST_UPDATE, (DPARAM)&g_cur_operating_number_index, sizeof(uint16));
		}
		break;
#if 0
    case ID_SET_SOS_NUMBER_CLEAR_ALL:
        break;
#endif
    case ID_SET_SOS_NUMBER_CONTACT:
        MMIAPIPB_OpenMainWin(
            MMIPB_ALL_CONTACT,//
            MMIPB_LIST_RADIO, //IN: 号码列表的类型
            MMIPB_LIST_FIELD_WHOLE,
            1,//如果是多选时，此项限制了多选的最大个数
            PNULL,
            SetSOSNumberSelectedFromPB
            );
        break;

    default:
        break;
    }

}


/*****************************************************************************/
//  Description : update list
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL void UpdateNumberListUI(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           )
{
    uint16 index = 0;

    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem( ctrl_id, MMISET_SOS_CONTACTS_MAX_NUM, FALSE );//max item 5
    
    for (;index < MMISET_SOS_CONTACTS_MAX_NUM; index++)
    {
        AppendNumberListItem(ctrl_id, index, FALSE);
    }
}

/*****************************************************************************/
//  Description : append list
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendNumberListItem(
                                MMI_CTRL_ID_T             ctrl_id,         //要设置的列表控件ID号 (输入参数)
                                uint16                    pos,
                                BOOLEAN                   is_update
                                )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 list_index = 0;
    MMI_STRING_T str_num = {0};

    if(MMISET_SOS_CONTACTS_MAX_NUM < pos)
    {
    	return;
    }

    item_t.item_data_ptr = &item_data;
    //set item style
#ifndef MMI_THEME_LIST_MINI_SUPPORT
   item_t.item_style = GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT;
#else
   item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
#endif



#if 0
    //name
    name_len = MIN(g_blacklist_ptr->blacklist[pos].name_len,MMIPB_NAME_MAX_LEN);
    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;    
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = name_len;
    
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = g_blacklist_ptr->blacklist[pos].name;

    list_index++;
#endif
    //number
    str_num.wstr_len = MIN(s_sos_contact_list[pos].num_len,MMISET_PHONE_NUM_MAX_LEN);

    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = str_num.wstr_len;
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = s_sos_contact_list[pos].num;

    list_index++;    
    
    //设置功能软键
    item_data.softkey_id[0] = STXT_OPTION;
#ifdef MMI_GUI_STYLE_MINISCREEN
    item_data.softkey_id[1] = IMAGE_NULL;
#else
    item_data.softkey_id[1] = TXT_NULL;
#endif
    item_data.softkey_id[2] = STXT_RETURN;
    
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
//  Description : to handle the sos number list option window
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNumberlistOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SOS_NUMBERLIST_LIST_OPT_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;

    SCI_TRACE_LOW("[SOS]HandleNumberlistOptWinMsg, msg_id = 0x%x",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        HandleNumberListMenuMsg(menu_id,ctrl_id);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//  Description : handle number list win
//  Global resource dependence : 
//  Author:Arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetSOSNumberListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: 窗口的ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: 窗口的内部消息ID
                                            DPARAM                 param        //IN: 相应消息的参数
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SOS_NUMBERLIST_LIST_CTRL_ID;
    uint16 index = 0;

    SCI_TRACE_LOW("[SOS]HandleSetSOSNumberListWindow, msg_id = 0x%x",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		g_cur_operating_number_index = 0;
        UpdateNumberListUI(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;

    case MSG_SET_SOS_NUMBER_LIST_UPDATE:
        UpdateNumberListUI(ctrl_id);
        index = *(uint16*)param;
        GUILIST_SetCurItemIndex(ctrl_id, index);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifndef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
#ifdef MMI_PDA_SUPPORT 
	case MSG_CTL_LIST_LONGOK:
#endif
        g_cur_operating_number_index =GUILIST_GetCurItemIndex(ctrl_id);
        MMK_CreateWin((uint32 *)MMISET_SET_SOS_NUMBER_LIST_OPT_WIN_TAB, PNULL);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        g_cur_operating_number_index =GUILIST_GetCurItemIndex(ctrl_id);
        MMK_CreateWin((uint32 *)MMISET_SET_SOS_NUMBER_EDITBOX_WIN_TAB, PNULL);
        break;


    case MMI_SELECT_CONTACT_CNF:
        SetSOSNumberSelectedFromPB((MMIPB_HANDLE_T)param);
        UpdateNumberListUI(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0); 
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;
/*
#ifdef MMI_PDA_SUPPORT        
    case MMI_CL_MULTI_SELECT_LIST_RETURN:
        SetBlacklistFromCL();
        UpdateBlackList(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0); 
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;        
#endif
*/
    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
      
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description: Handle sos settings main menu win msg  
//  Global resource dependence: none 
//  Author: Arvin.zhang 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
   MMI_MENU_GROUP_ID_T         group_id   =   0;
   MMI_MENU_ID_T               menu_id    =   0;
   MMI_RESULT_E                result     =   MMI_RESULT_TRUE;
   MMI_CTRL_ID_T               ctrl_id    =   MMISET_SOS_MAINMENU_CTRL_ID;

    SCI_TRACE_LOW("[SOS]HandleMainMenuWinMsg, msg_id = 0x%x",msg_id);
    
    switch(msg_id)
    {
		case MSG_OPEN_WINDOW:
			MMK_SetAtvCtrl(win_id, ctrl_id);
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
			break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_MIDSK:
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
			switch (menu_id)
			{
				case ID_SET_SOS_NUMBER_LIST:
					MMK_CreateWin((uint32 *)MMISET_SET_SOS_NUMBER_LIST_WIN_TAB,PNULL);
					break;

				case ID_SET_SOS_MESSAGE_SETTINGS:
					MMK_CreateWin((uint32 *)MMISET_SET_SOS_MESSAGE_CONTENT_EDIT_WIN_TAB,PNULL);
					break;

				case ID_SET_SOS_TYPE_SETTINGS:
					MMK_CreateWin((uint32 *)MMISET_SET_SOS_TYPE_WIN_TAB,PNULL);
					break;

				case ID_SET_SOS_INFO:
					MMK_CreateWin((uint32 *)MMISET_SET_SOS_ABOUT_WIN_TAB,PNULL);
					break;

				default:
					break;
			}
			break;

		case MSG_GET_FOCUS: 
			break;

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
			//MMK_DestroyDynaCtrl(MMIBT_MAIN_MENU_CTRL_ID);
			break;

		default:
			result = MMI_RESULT_FALSE;
			break;
	}
    return result;
}

/*****************************************************************************/
// 	Description : init sos context info
//	Global resource dependence : none
//    Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_InitSOSContextInfo(void)
{
    MMI_STRING_T        text_str = {0};
    MN_RETURN_RESULT_E          return_value        =   MN_RETURN_FAILURE;

    //get sos switch info from nv
    MMINV_READ(MMINV_SET_SOS_SWITCH,&s_sos_enabled,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
    	s_sos_enabled = TRUE;
    	MMINV_WRITE(MMINV_SET_IDLEDISPLAY_INFO,&s_sos_enabled);   
    }

    //get sos msg sent switch info from nv
    MMINV_READ(MMINV_SET_SOS_MSG_SENT_SWITCH,&s_sos_msg_sent_enabled,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
    	s_sos_msg_sent_enabled = TRUE;
    	MMINV_WRITE(MMINV_SET_SOS_MSG_SENT_SWITCH,&s_sos_msg_sent_enabled);   
    }

#ifdef MMIDC_F_FLASH
    //get sos led alert switch info from nv
    MMINV_READ(MMINV_SET_SOS_LED_ALERT_SWITCH,&s_sos_led_alert_enabled,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
    	s_sos_led_alert_enabled = TRUE;
    	MMINV_WRITE(MMINV_SET_SOS_LED_ALERT_SWITCH,&s_sos_led_alert_enabled);   
    }
#endif

    //get sos ring alert switch info from nv
    MMINV_READ(MMINV_SET_SOS_RING_ALERT_SWITCH,&s_sos_ring_alert_enabled,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
    	s_sos_ring_alert_enabled = TRUE;
    	MMINV_WRITE(MMINV_SET_SOS_RING_ALERT_SWITCH,&s_sos_ring_alert_enabled);   
    }

    //get sos msg content info from nv
    MMINV_READ(MMINV_SET_SOS_MSG_INFO,&s_sos_msg_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
		MMI_GetLabelTextByLang(TXT_SET_SOS_MSG_DEFAULT,&text_str);
        SCI_MEMSET(s_sos_msg_info.msg_content, 0X00, sizeof(s_sos_msg_info.msg_content));

		MMI_WSTRNCPY(s_sos_msg_info.msg_content, MMISET_SOS_MSG_CONTENT_MAX_LEN, text_str.wstr_ptr,  text_str.wstr_len, text_str.wstr_len);
        s_sos_msg_info.msg_len = MIN(MMISET_SOS_MSG_CONTENT_MAX_LEN, text_str.wstr_len);

    	MMINV_WRITE(MMINV_SET_SOS_MSG_INFO,&s_sos_msg_info);   
    }

    //get sos contact list info from nv
    MMINV_READ(MMINV_SET_SOS_CONTACT_LIST,&s_sos_contact_list,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(s_sos_contact_list, 0, (MMISET_SOS_CONTACTS_MAX_NUM * sizeof(MMISET_SOS_CONTACT_ITEM_T)));
    	MMINV_WRITE(MMINV_SET_SOS_CONTACT_LIST,&s_sos_contact_list);   
    }
	
}

/*****************************************************************************/
// 	Description : factory reset the sos context info 
//	Global resource dependence : none
//  Author: arvin.zhang
//	Note: should keep all initial config the same with the req
/*****************************************************************************/
PUBLIC void MMISET_ResetSOSContextInfo(void)
{
    MMI_STRING_T        text_str = {0};

    //sos switch default value is TRUE
    s_sos_enabled = TRUE;
    MMINV_WRITE(MMINV_SET_IDLEDISPLAY_INFO,&s_sos_enabled);

    s_sos_msg_sent_enabled = TRUE;
    MMINV_WRITE(MMINV_SET_SOS_MSG_SENT_SWITCH,&s_sos_msg_sent_enabled);
	
#ifdef MMIDC_F_FLASH
    //sos led alert switch default value is TRUE
    s_sos_led_alert_enabled = TRUE;
    MMINV_WRITE(MMINV_SET_SOS_LED_ALERT_SWITCH,&s_sos_led_alert_enabled);
#endif  

    	s_sos_ring_alert_enabled = TRUE;
    	MMINV_WRITE(MMINV_SET_SOS_RING_ALERT_SWITCH,&s_sos_ring_alert_enabled);

    MMI_GetLabelTextByLang(TXT_SET_SOS_MSG_DEFAULT,&text_str);
    SCI_MEMSET(s_sos_msg_info.msg_content, 0X00, sizeof(s_sos_msg_info.msg_content));
    MMI_WSTRNCPY(s_sos_msg_info.msg_content, MMISET_SOS_MSG_CONTENT_MAX_LEN, text_str.wstr_ptr,  text_str.wstr_len, text_str.wstr_len);
    s_sos_msg_info.msg_len = MIN(MMISET_SOS_MSG_CONTENT_MAX_LEN, text_str.wstr_len);
    MMINV_WRITE(MMINV_SET_SOS_MSG_INFO,&s_sos_msg_info);   

    SCI_MEMSET(s_sos_contact_list, 0, (MMISET_SOS_CONTACTS_MAX_NUM * sizeof(MMISET_SOS_CONTACT_ITEM_T)));
    MMINV_WRITE(MMINV_SET_SOS_CONTACT_LIST,&s_sos_contact_list);   
}

/*****************************************************************************/
// 	Description : start sos procedures 
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StartSOSProcedures(void)
{
    SCI_TRACE_LOW("[SOS]MMIAPISET_StartSOSProcedures s_sos_enabled=%d.", s_sos_enabled);

    if(s_sos_enabled)
    {
        s_is_sos_running = TRUE;
        SCI_TRACE_LOW("[SOS]MMIAPISET_StartSOSProcedures start_time = %d", SCI_GetTickCount());

        MMK_CreateWin((uint32 *)MMISET_SET_SOS_ABOUT_WIN_TAB,(ADD_DATA)s_is_sos_running);

		//flash the light or lamp if needed
		#ifdef MMIDC_F_FLASH
		startSOSLEDAlert();
		#endif
		StartSOSRingAlert();

		//make sos call to the emergency contacts one by one until call connected
		MMICC_MakeSosCall(&s_sos_contact_list, (MMISET_SOS_CALLBACK_PFUNC)CancelButtonCallback);

		//sent the emergency msg if needed or mark to send later
		if(!IsEmergencyNumberExistForMessageSent())
		{
			SendSOSMessage();
		}
		else
		{
			SetSOSMessageDelaySentFlag(TRUE);
		}

        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
    }
}
/*****************************************************************************/
// 	Description : enter sos setting window 
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterSOSSettingWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_SET_SOS_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : get sos switch on/off 
//	Global resource dependence : s_sos_enabled
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSSwitchState(void)
{
    return s_sos_enabled;
}

#ifdef MMIDC_F_FLASH
/*****************************************************************************/
// 	Description : get sos led alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSLEDAlertSwitchState(void)
{
    return s_sos_led_alert_enabled;
}
#endif

/*****************************************************************************/
// 	Description : get sos ring alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSRingAlertSwitchState(void)
{
    return s_sos_ring_alert_enabled;
}

/*****************************************************************************/
// 	Description : get sos msg sent switch on/off 
//	Global resource dependence : s_sos_msg_sent_enabled
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSMsgSentSwitchState(void)
{
    return s_sos_msg_sent_enabled;
}

/*****************************************************************************/
// 	Description : get sos msg info 
//	Global resource dependence : s_sos_msg_info
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC MMISET_SOS_MESSAGE_CONTENT_T* MMIAPISET_GetSOSMsgContent(void)
{
    return &s_sos_msg_info;
}

/*****************************************************************************/
// 	Description : get sos contact list  info 
//	Global resource dependence : s_sos_contact_list
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC MMISET_SOS_CONTACT_ITEM_T* MMIAPISET_GetSOSContactListInfo(void)
{
    return &s_sos_contact_list;
}

/*****************************************************************************/
// 	Description : get if sos is running or not
//	Global resource dependence : s_is_sos_running
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSOSProcRunning(void)
{
    return s_is_sos_running;
}

#endif//#ifdef MMI_INDIAN_SOS_SUPPORT
