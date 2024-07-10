/*****************************************************************************
** File Name:      mmiudisk_wintab.c                                            *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2005       Jassmine.Meng     Creat
******************************************************************************/

#define _MMIUDISK_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_udisk_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#ifdef WIN32
#include "mmiudisk_simu.h"
#include "os_param.h"
#else
#include "usbservice_api.h"
#include "mmi_pccamera.h"
#include "os_param.h"
#ifdef USB_WEBCAMERA_SUPPORT
//#include "UPCC_api.h"
#include "dal_dcamera.h"
#endif
#endif
#ifdef BT_DUN_SUPPORT
#include "sio.h"
#endif
#include "mmiacc_id.h"
#include "mmiacc_text.h"
//#include "mmiacc_menutable.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "guilistbox.h"
#include "mmisd_export.h"
#include "mmiudisk_internal.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#include "java_mmi_text.h"
#endif
#include "mmieng_id.h"
#include "mmimp3_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmisms_export.h"
#include "mmipb_export.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "guisoftkey.h"
#include "mmiudisk_export.h"
#include "mmi_default.h"
#include "guirichtext.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
//#include "mmifmm_menutable.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#endif
#include "mmipb_text.h"
#ifdef QQ_SUPPORT
#include "qq2009.h"
#include "qq_text.h"
#endif

#include "guiform.h"
#include "guilabel.h"

#ifdef MCARE_V31_SUPPORT
#include "McfInterface.h"
#include "mcare_Interface.h"
#endif
#ifdef DSP_USB_LOG
#include "dsp_log.h"      //xuefang.jiang 20110804
#endif

#include "mmi_event_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#include "mmimultim_text.h"
#include "mmisrvaud_api.h"
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#ifdef DSP_USB_LOG
#ifndef WIN32
#include "ref_param.h"
#endif
#endif

#include "mmidata_share.h"
#include "watch_commonwin_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define  USB_SERVICE_CHARGE   USB_SERVICE_NULL
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL   uint32                s_current_device = MMI_DEVICE_UDISK;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdultWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif
/*****************************************************************************/
//  Description : 设置USB服务列表
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint8 SetUSBServiceList(
								  MMI_CTRL_ID_T        ctrl_id
                                  );
/*****************************************************************************/
//  Description : USB服务有效数目
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint8 GetValidUSBServiceCount(void);
/*****************************************************************************/
//  Description : 根据服务类型，获得该类型的显示的字符串ID
//  Global resource dependence : 
//  Author: 
//  Note:返回该该类型的显示的字符串ID
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetServiceNameTextID(USB_SERVICE_E type);

/*****************************************************************************/
//  Description : to handle select usb operation
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUsbOperSelectWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );
/*****************************************************************************/
//  Description : to display used memory and unused memory
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMemoryDetailWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : open Memory detail win
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void OpenMemDetailWin(uint32 device);

/*****************************************************************************/
//  Description : Handle UPCC  Sensor ID Win Msg
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUPCCSensorWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                      );

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

//the detail window
WINDOW_TABLE(MMIUDISK_MEM_DETAIL_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMemoryDetailWinMsg ), 
    WIN_ID( MMIFMM_MEMORY_DETAIL_WIN_ID ),
    WIN_TITLE(TXT_COMMON_DETAIL_MEMORY),
    //CREATE_RICHTEXT_CTRL(MMIUDISK_MEM_DETAIL_TEXT_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIUDISK_MEM_DETAIL_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIUDISK_MEM_DETAIL_LABEL_CTRL_ID,MMIUDISK_MEM_DETAIL_FORM_CTRL_ID),
        CHILD_ANIM_CTRL(FALSE,FALSE,MMIUDISK_MEM_DETAIL_ANIM_CTRL_ID,MMIUDISK_MEM_DETAIL_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for usb operation select window
WINDOW_TABLE( MMIUDISK_USB_OPER_SELECT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleUsbOperSelectWindow ),    
    WIN_ID( MMIUDISK_USB_OPER_SELECT_WIN_ID ),
    WIN_HIDE_STATUS,
#if (!defined SCREEN_SHAPE_CIRCULAR) || (defined ADULT_WATCH_SUPPORT)
    WIN_TITLE( TXT_UDISK_PLS_SELECT ),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIUDISK_USB_OPER_SELECT_CTRL_ID),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

//the window for usb operation select window
WINDOW_TABLE( MMIUDISK_USB_OPER_SELECT_IN_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_LOWEST_LEVEL ),
    WIN_FUNC( (uint32)HandleUsbOperSelectWindow ),    
    WIN_ID( MMIUDISK_USB_OPER_SELECT_WIN_ID ),
#if (!defined SCREEN_SHAPE_CIRCULAR) || (defined ADULT_WATCH_SUPPORT)
    WIN_TITLE( TXT_UDISK_PLS_SELECT ),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIUDISK_USB_OPER_SELECT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef USB_WEBCAMERA_SUPPORT
//the window for upcc sensor
WINDOW_TABLE( MMIUDISK_UPCC_SENSOR_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleUPCCSensorWinMsg),    
    WIN_ID( MMIUDISK_UPCC_SENSOR_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE( TXT_SWITCH_SENSOR ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

                                       
/*****************************************************************************/
//  Description : open usb select window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_OpenUsbOperWindow(void)
{
    if(GetValidUSBServiceCount() == 0)
    {
        //no usb service avaliable
        return FALSE;
    }
#ifdef  RELEASE_INFO
        return FALSE;
#endif
    
    MMK_CloseWin(MMIENG_CARDLOG_WIN_ID);
    if (MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))
    {
        return TRUE;
    }
    if (MMK_CreateWin((uint32 *)MMIUDISK_USB_OPER_SELECT_WIN_TAB, PNULL))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}

/*****************************************************************************/
//  Description : open usb select window
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_UsbExpiredInCallWindow(void)
{
    if (MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))
    {
        return TRUE;
    }
    if (MMK_CreateWin((uint32 *)MMIUDISK_USB_OPER_SELECT_IN_CALL_WIN_TAB, PNULL))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
   
}

/*****************************************************************************/
//  Description : start pc camera service
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_StartUPCC(void)
{
    //start pc camera
#ifdef USB_WEBCAMERA_SUPPORT
    //SCI_TRACE_LOW:"[mmiudisk]: MMIUDISK_StartUPCC"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_296_112_2_18_3_4_9_55,(uint8*)"");
    
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_UDISK_PC);
#endif        

    MMK_ReturnIdleWin();
    MMIPUB_OpenTempWaitWin();
    
    MMK_CreateWin((uint32*)MMIUDISK_UPCC_SENSOR_WIN_TAB, PNULL);
#endif
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void UDISK_ValidatePrivacyUsbOper(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMI_WIN_ID_T win_id = 0;
    
    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            win_id = *(MMI_WIN_ID_T*)param_ptr;    
        }

        MMK_SendMsg(win_id, MSG_SET_VALIDATE_PRIVACY_SUCCESS, PNULL);
    }
}

/*****************************************************************************/
//  Description : to handle select usb operation
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN  WBOOT_IsWRERunning(void);
#endif
LOCAL MMI_RESULT_E  HandleUsbOperSelectWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E                recode              =   MMI_RESULT_TRUE;
    uint16                      cur_selection       =   0;
    MMI_CTRL_ID_T               ctrl_id             =   MMIUDISK_USB_OPER_SELECT_CTRL_ID;
    //POWER_RESTART_CONDITION_E   restart_condition   =   RESTART_BY_NONE;
    // uint8                       item_num            =   0;
    USB_SERVICE_E               service_type        = USB_SERVICE_NULL;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};

            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
        }
#endif
		MMIDEFAULT_TurnOnBackLight();
        	MMIAPIUdisk_SetUSBDelay(FALSE); 
		SetUSBServiceList(ctrl_id);
		MMK_SetAtvCtrl(win_id, ctrl_id);

#if 0
        if(MMIUDISK_CurrentAppCanbeAbort())
        {
            //current condition can start udisk
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_USB, UDISK_ValidatePrivacyUsbOper, (void*)&win_id, sizeof(win_id));
        }
#endif
        break;
    
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_MENU:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if(GUILIST_GetItemData(ctrl_id, cur_selection, &service_type))
        {
            //GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            switch(service_type)
            {
                case USB_SERVICE_UDISK:
#ifdef MCARE_V31_SUPPORT
                    if(MCareV31_Exit_Confirm())
                    {
                        Mcare_Udisk_Start();/*lint !e718 !e18*/
                        MMK_CloseWin(MMIUDISK_WAIT_WIN_ID);        
                        //MMK_CloseWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
                        return MMI_RESULT_FALSE;
                    }
#endif
#ifdef MET_MEX_SUPPORT
                    if(MMIMEX_ConflictPromt())
                    {
                        // mex kernel is running and may result in file error
                        GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
                        break;
                    }
#endif

                    //start udisk service
                    if(MMIUDISK_CurrentAppCanbeAbort())
                    {
                        //current condition can start udisk
                        MMIAPISET_ValidatePrivacy(MMISET_PROTECT_USB, UDISK_ValidatePrivacyUsbOper, (void*)&win_id, sizeof(win_id));
                    }
                    else
                    {
                        //current app can not be exit, so can not start U disk
                        //GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
                        GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
                        MMIPUB_OpenAlertWarningWin(TXT_COM_CANNOT_START_UDISK);                    
                    }
                    break;
#ifdef USB_VUART
                case USB_SERVICE_UCOM:
                    if(USB_SERVICE_NULL !=  USB_GetCurService())
                    {
                        USB_StopService(USB_GetCurService());
                    }
                    //start pc comunicate service
#ifdef BT_DUN_SUPPORT
                    if(SIO_UserPortBusy())
                    {
                        //SCI_TRACE_LOW:"UART selected SIO_UserPortBusy"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_393_112_2_18_3_4_10_56,(uint8*)"");
                        //GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
                        MMIPUB_OpenAlertWarningWin(TXT_COMM_UCOM_BUSY);
                    }
                    else
#endif
                    {
                        if (MMIAPIMTV_IsMTVRunning())
                        {
                            //need exit MobileTV first
                            //GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
                            MMIPUB_OpenAlertWarningWin(TXT_EXIT_MOBILETV);
                            break;
                        }
                        if(MMIAPIPHONE_GetSimOkNum(PNULL,0) == 0)
                        {
                            //no sim exist
                            //GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
#ifdef ADULT_WATCH_SUPPORT
                            MMIUDISK_WatchOpenErrPrompt(TXT_PB_SIM_ERROR, usb_storage_connect_fail);
#else
                            MMIPUB_OpenAlertWarningWin(TXT_PB_SIM_ERROR);
#endif
                            break;
                        }
                        if(MMIAPISMS_IsOrderOk()&& MMIAPIPB_IsPbReady())
                        {
                            //start this service
                            MMIUSBSHARE_ConnectPDP();
                            MMIUSBSHARE_EnterConnect();
                            //GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
                            // MMIUDISK_StartVirtualUart();
#ifdef BLUETOOTH_SUPPORT
                            MMIAPIBT_CloseDUNWin();
#endif
                            MMK_CloseWin(win_id);
                        }
                        else
                        {
                            //sms or pb not ready can not start this service
                            //GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
                            MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
                        }
                    }
                    break;
#endif
#ifdef USB_WEBCAMERA_SUPPORT
                case USB_SERVICE_UPCC:
#ifdef WRE_SUPPORT //wre运行,内存不足，提示退出
			if(WBOOT_IsWRERunning())
			{
				//GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
				MMI_STRING_T txt_exit_WRE={0X00};
				txt_exit_WRE.wstr_len=18;
				txt_exit_WRE.wstr_ptr=L"Exit wre, please!";
				MMIPUB_OpenAlertWinByTextPtr(PNULL, &txt_exit_WRE, PNULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
				break;
			}
#endif
                 //QQ运行后，内存不足，进入DC要提示QQ退出
#ifdef QQ_SUPPORT
                if (QQ_IsRunning())
                {
					//GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
                    MMIPUB_OpenAlertWarningWin( TXT_EXIT_QQ);
                    break;
                }
#endif

                //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
                if (MMIAPIJAVA_IsJavaRuning())
                {
					//GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
                    MMIPUB_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
                    break;
                }
#endif
#ifdef MET_MEX_SUPPORT
				if(MMIMEX_ConflictPromt())
				{
		            // mex kernel is running and may result in mem conflict
					break;
				}
#endif
				  if(MMIUDISK_CurrentAppCanbeAbort())
				  {
					  MMIUDISK_StartUPCC();
				  }
				  else
				  {
					  //GUILIST_SetSelectedItem(ctrl_id, cur_selection, FALSE);
                      GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
					  MMIPUB_OpenAlertWarningWin(TXT_COM_CANNOT_START_PCC);                    
				  }
				  break;
#endif
#ifdef USB_LOG
                case USB_SERVICE_LOG:
                    if(USB_SERVICE_NULL !=  USB_GetCurService())
                    {
                        USB_StopService(USB_GetCurService());
                    }
                    //GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
                    MMIUDISK_StartUsbLog();
                    MMK_CloseWin(win_id);
                    break;
#endif
                default:
                    if(USB_SERVICE_NULL !=  USB_GetCurService())
                    {
                        USB_StopService(USB_GetCurService());
                    }
                    MMK_CloseWin(win_id);
                    break;
            }
        }
        else
        {
            //get service type error
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            MMK_CloseWin(win_id);
        }             
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        if(USB_SERVICE_NULL !=  USB_GetCurService())
        {
               USB_StopService(USB_GetCurService());
        }
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_SET_VALIDATE_PRIVACY_SUCCESS:
        if(RESTART_BY_CHARGE == POWER_GetRestartCondition())
        {
            //GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            MMIUDISK_StartUdiskInPowerOff();                            
            MMK_CloseWin( win_id );      
        }
        else
        {

           MMIUDISK_StartUdisk();
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        MMK_CloseWin( win_id );
        break;
#endif        
		
#ifdef FLASH_SUPPORT
	case MSG_CLOSE_WINDOW:
		MMIAPIPHONE_ScrDelCallBack(win_id);
		break;
#endif    
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle usb pc camera win msg
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUDISK_HandleATCExtCmd(void)
{
    MMI_WIN_ID_T udisk_uart_win_id = MMIUDISK_VUART_WAITING_WIN_ID;

    if (MMK_IsOpenWin(udisk_uart_win_id))
    {
        MMK_PostMsg(udisk_uart_win_id, MSG_MMI_PC_SYNC_READY, PNULL,0);
    }

    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsStopWainWinOpened(void)
{
    return MMK_IsOpenWin(MMIUDISK_STOP_WAIT_WIN_ID);
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsFormatWaitWinOpened(void)
{
    return MMK_IsOpenWin(MMISDCARD_FORMATTING_WIN_ID);
}


/*****************************************************************************/
//  Description : to display used memory and unused memory
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMemoryDetailWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    //GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    //MMI_CTRL_ID_T       ctrl_id         =   MMIUDISK_MEM_DETAIL_TEXT_CTRL_ID;
    uint32              device          =   0;
    //uint32              time_out        =   0;
    MMI_STRING_T        str_t           =   {0};
    //MMI_WIN_ID_T        alert_win_id    =   MMISDCARD_FORMATTING_WIN_ID;
    
    //lcd_dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    ///lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    device = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_TurnOnBackLight();
        //MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_UDISK);
        MMIUDISK_SetMemoryDetail((MMIFILE_DEVICE_E)device); 
		MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceMemStatusText(device), &str_t);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
            
       // GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);

		GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);	
        MMK_SetAtvCtrl(win_id, MMIUDISK_MEM_DETAIL_FORM_CTRL_ID);
        break;
		
	case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:

		//MMIPUB_OpenQueryWinByTextId(TXT_IS_FORMAT_SDCARD,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        break;
//    case MSG_PROMPTWIN_OK:
//        MMIPUB_CloseQuerytWin(PNULL);
//
//        MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,MMIAPISD_NotInterruptFfs);
//        time_out = MMI_3SECONDS;
//        if (MMI_DEVICE_UDISK == device)
//        {   
//            if (MMIUDISK_FormatUdisk())
//            {
//                MMK_CloseWin(alert_win_id);
//                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,MMIUDISK_HandleUdiskAlertClosedMsg);
//                MMIAPIFMM_UpdateFmmList(device,TRUE);
//            }
//            else
//            {
//                MMK_CloseWin(alert_win_id);
//                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,MMIUDISK_HandleUdiskAlertClosedMsg);
//            }
//            MMK_CloseWin(win_id);
//        }
//        else
//        {
//           if (MMIAPISD_IsSDPlugIn())
//            {
//          #ifdef MAINLCD_SIZE_128X64
//          #else
//                MMIAPISD_HandleBeforeSDCardFormat();
//                    MMIAPIFMM_FormatDeviceAsyn(
//                        MMIFILE_DEVICE_SDCARD, 
//                        MMIFILE_DEVICE_SDCARD_LEN, 
//                        MMIFILE_FORMAT_AUTO,
//                        win_id, 
//                        MSG_UDISK_FORMAT_CNF
//                        );
//         #endif
//            }
//            else
//            {
//                MMK_CloseWin(alert_win_id);
//                MMIPUB_OpenAlertWinByTextId(
//                    &time_out,  
//                    TXT_COMMON_SD_FAIL,
//                    TXT_NULL,
//                    IMAGE_PUBWIN_FAIL,
//                    PNULL,
//                    PNULL,
//                    MMIPUB_SOFTKEY_ONE,
//                    PNULL
//                     );
//                 MMK_CloseWin(win_id);
//            }
//        }
//
//        break;
//                
//    case MSG_PROMPTWIN_CANCEL:
//        MMIPUB_CloseQuerytWin(PNULL);
//        break;
//        
//    case MSG_UDISK_FORMAT_CNF:
//        MMK_CloseWin(alert_win_id);
//        MMIAPIUdisk_HandleFormatCnf(*(BOOLEAN *)param);
//        MMIAPIFMM_UpdateFmmList(device,TRUE);
//        MMK_CloseWin(win_id);
//        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
       // MMIPUB_CloseAlertWin();
        MMK_CloseWin(win_id);
        break; 
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open Memory detail win
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void OpenMemDetailWin(uint32 device)
{
    MMK_CreateWin((uint32 *)MMIUDISK_MEM_DETAIL_WIN_TAB , (ADD_DATA)device);
}

/*****************************************************************************/
//  Description : open Memory detail win
//  Global resource dependence : 
//  Author:jian.ma
//modified by ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUDISK_OpenMemDetailWin(uint32 device)
{    
	OpenMemDetailWin(device);
    //BOOLEAN is_udisk_exist = FALSE;
    //BOOLEAN is_sdcard_exist = FALSE;

    //MMIAPIUDISK_GetAllDeviceStatus(&is_udisk_exist, &is_sdcard_exist);

/*
    if (is_udisk_exist&&is_sdcard_exist)
    {
        MMK_CreateWin((uint32 *)MMIUDISK_DEVICE_SELECT_WIN_TAB , PNULL);
    }
    else 
    */
//     if(MMI_DEVICE_SDCARD == device)
//     {
//         if (is_sdcard_exist)
//         {
//             OpenMemDetailWin(MMI_DEVICE_SDCARD);
//         }    	
//     }
//     else if (MMI_DEVICE_UDISK == device)
//     {
//         if (is_udisk_exist)
//         {
//             OpenMemDetailWin(MMI_DEVICE_UDISK);
//         }    
//     } 
//     else
//     {
//         SCI_ASSERT(0);
//     }

}

/*****************************************************************************/
//  Description : 设置USB服务列表
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint8 SetUSBServiceList(
								  MMI_CTRL_ID_T        ctrl_id
                                  )
{
    // uint16  cur_selection   =   0;
    // uint16  i = 0;
	BOOLEAN is_service_support = FALSE;
    POWER_RESTART_CONDITION_E   restart_condition = RESTART_BY_NONE;
    uint8 service_num = USB_GetServiceCount();
#ifdef USB_LOG    
	uint8 assert_mode =0;
#endif
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
    GUI_RECT_T list_rect = {0, 30, 239, 209};
#endif
	USB_SERVICE_E serive_type = USB_SERVICE_NULL;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    restart_condition = POWER_GetRestartCondition();
	GUILIST_SetMaxItem(ctrl_id, service_num + 1, FALSE );  //max item 4  
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
    GUILIST_SetRect(ctrl_id,&list_rect);
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
#endif
	service_num = 0;
    //First service is default Charge
    service_num ++;
#ifdef ADULT_WATCH_SUPPORT
    item_t.item_style    = GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH; //bug1585529
#elif SCREEN_SHAPE_CIRCULAR
    item_t.item_style    = GUIITEM_STYLE_1STR_1LINE_RADIO_LAYOUT1;
#else
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
#endif
    item_t.user_data = USB_SERVICE_CHARGE;
    
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = GetServiceNameTextID(USB_SERVICE_CHARGE);
    
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    GUILIST_AppendItem( ctrl_id, &item_t );  
	do
	{
		is_service_support = FALSE;
		serive_type = USB_GetNextService();
		if(USB_SERVICE_UDISK == serive_type)
		{

			// if sd card or u disk exist.
			if (MMI_DEVICE_NUM > MMIAPIFMM_GetFirstValidDevice())		
			{
				is_service_support = TRUE;
			}
		}
		else if(USB_SERVICE_UCOM == serive_type)
		{
			//pc data communicate
#ifdef USB_VUART
			if ((RESTART_BY_CHARGE != restart_condition) &&(RESTART_BY_ALARM != restart_condition))
			{
				is_service_support = TRUE;
			}
#endif			
		}
		else if(USB_SERVICE_UPCC == serive_type)
		{
			//pc camera
#ifdef USB_WEBCAMERA_SUPPORT
			if ((RESTART_BY_CHARGE != restart_condition) &&(RESTART_BY_ALARM != restart_condition))
			{
				is_service_support = TRUE;

			}
#endif
		}
		else if(USB_SERVICE_LOG == serive_type)
		{
			//usb log
#ifdef USB_LOG
			if (!MMIAPISD_IsCardLogOn())
			{
				OSPARAM_GetAssertMode(&assert_mode);
				if (SCI_ASSERT_DEBUG_MODE == assert_mode)
				{
					is_service_support = TRUE;
				}
			}
#endif
		}
		else// if(USB_SERVICE_NULL != serive_type)
		{
			is_service_support = FALSE;
		}
		if(is_service_support)
		{
			//append to service list
			service_num ++;
#ifdef ADULT_WATCH_SUPPORT
            item_t.item_style    = GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH; //bug1585529
#elif SCREEN_SHAPE_CIRCULAR
            item_t.item_style    = GUIITEM_STYLE_1STR_1LINE_RADIO_LAYOUT1;
#else
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
#endif
			item_t.user_data = serive_type;

			item_t.item_data_ptr = &item_data;
			item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
			item_data.item_content[0].item_data.text_id = GetServiceNameTextID(serive_type);
			
			item_data.softkey_id[0] = TXT_COMMON_OK;
			item_data.softkey_id[1] = TXT_NULL;
			item_data.softkey_id[2] = STXT_RETURN;
			GUILIST_AppendItem( ctrl_id, &item_t );  
		}
	}while(USB_SERVICE_NULL != serive_type && (USB_SERVICE_MAX != serive_type));
    //always set charge selected
    GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
    return service_num;
}

/*****************************************************************************/
//  Description : 设置USB服务列表
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint8 GetValidUSBServiceCount(void)
{
    // uint16  cur_selection   =   0;
    // uint16  i = 0;
    POWER_RESTART_CONDITION_E   restart_condition = RESTART_BY_NONE;
#ifdef USB_LOG    
	uint8 assert_mode =0;
#endif
    uint8 service_num = 0;
	USB_SERVICE_E serive_type = USB_SERVICE_NULL;
    restart_condition = POWER_GetRestartCondition();
	
	do
	{
		serive_type = USB_GetNextService();
		if(USB_SERVICE_UDISK == serive_type)
		{
			// if sd card or u disk exist.
			if (MMI_DEVICE_NUM > MMIAPIFMM_GetFirstValidDevice())		
			{
				service_num++;
			}
		}
		else if(USB_SERVICE_UCOM == serive_type)
		{
			//pc data communicate
#ifdef USB_VUART
			if ((RESTART_BY_CHARGE != restart_condition) &&(RESTART_BY_ALARM != restart_condition))
			{
					service_num++;
			}
#endif			
		}
		else if(USB_SERVICE_UPCC == serive_type)
		{
			//pc camera
#ifdef USB_WEBCAMERA_SUPPORT
			if ((RESTART_BY_CHARGE != restart_condition) &&(RESTART_BY_ALARM != restart_condition))
			{
					service_num++;

			}
#endif
		}
		else if(USB_SERVICE_LOG == serive_type)
		{
			//usb log
#ifdef USB_LOG
			if (!MMIAPISD_IsCardLogOn())
			{
				OSPARAM_GetAssertMode(&assert_mode);
				if (SCI_ASSERT_DEBUG_MODE == assert_mode)
				{
					service_num++;
				}
			}
#endif
		}

	}while(USB_SERVICE_NULL != serive_type && (USB_SERVICE_MAX != serive_type));

    return service_num;
}

/*****************************************************************************/
//  Description : 根据服务类型，获得该类型的显示的字符串ID
//  Global resource dependence : 
//  Author: 
//  Note:返回该该类型的显示的字符串ID
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetServiceNameTextID(USB_SERVICE_E type)
{
	MMI_TEXT_ID_T text_id = TXT_NULL;

	switch(type)
	{
	case USB_SERVICE_UDISK:
		text_id = TXT_UDISK_UDISK;
		break;
	case USB_SERVICE_UCOM:
		text_id = TXT_UDISK_VUART;
		break;
	case USB_SERVICE_UPCC:
		text_id = TXT_UDISK_PCC;
		break;
	case USB_SERVICE_LOG:
		text_id = TXT_COMM_USB_LOG;
		break;
    case USB_SERVICE_CHARGE:
        text_id = TXT_CHARGING;		
        break;
	default:
		break;
	}

	return text_id;
}

/*****************************************************************************/
//  Description : 获得当前服务类型的显示的字符串ID
//  Global resource dependence : 
//  Author: 
//  Note:返回该该类型的显示的字符串ID
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIUDISK_GetCurServiceNameTextID(void)
{
    USB_SERVICE_E serive_type = USB_SERVICE_NULL;
    MMI_TEXT_ID_T text_id = 0;

    serive_type = USB_GetCurService();
    if (serive_type < USB_SERVICE_MAX)
    {
        text_id = GetServiceNameTextID(serive_type);
    }

    return text_id;
}
#ifdef ADULT_WATCH_SUPPORT
PUBLIC void MMIUDISK_WatchOpenNotiPrompt(MMI_TEXT_ID_T alert_text_id, MMI_IMAGE_ID_T img_id)
{
    MMI_STRING_T commentString ;
    WATCH_SOFTKEY_TEXT_ID_T   softkey_test = {TXT_NULL,STXT_RETURN,TXT_NULL};
    MMI_GetLabelTextByLang(alert_text_id, &commentString);
    if(MMK_IsOpenWin(MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID))
    {
        MMK_CloseWin( MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID );
    }
    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID, &commentString, img_id,  PNULL );
}

PUBLIC void MMIUDISK_WatchOpenErrPrompt(MMI_TEXT_ID_T alert_text_id, MMI_IMAGE_ID_T img_id)
{
    MMI_STRING_T commentString ;
    WATCH_SOFTKEY_TEXT_ID_T   softkey_test = {TXT_NULL,STXT_RETURN,TXT_NULL};
    MMI_GetLabelTextByLang(alert_text_id, &commentString);
    if(MMK_IsOpenWin(MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID))
    {
        MMK_CloseWin( MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID );
    }
    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID, &commentString, img_id, PNULL );
}

PUBLIC void MMIUDISK_WatchClosePrompt(void)
{
    if(MMK_IsOpenWin(MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID))
    {
        MMK_CloseWin( MMIUDISK_USB_SERVICE_NOTIFY_WIN_ID );
    }
}

#endif
/*****************************************************************************/
//  Description : open waiting window    
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC void MMIUDISK_OpenWaitWin(
								 uint32          usb_srv_type,
								 MMI_WIN_ID_T    alert_win_id,
								 MMI_TEXT_ID_T   alert_text_id)
{
    MMI_STRING_T    prompt_str = {0};

	MMI_GetLabelTextByLang(alert_text_id, &prompt_str);
#ifdef ADULT_WATCH_SUPPORT
    {
        MMIUDISK_WatchClosePrompt();//all result window should be removed.
        AdultWatchCOM_WaitingWin_1str_WaveIcon_Enter(
                    MMIUDISK_USB_SERVICE_START_WIN_ID,
                    PNULL,
                    HandleAdultWaitWinMsg);
    }
#else
    MMIPUB_OpenWaitWinEx(MMK_GetFirstAppletHandle(), 1,&prompt_str,PNULL,PNULL,alert_win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleWaitWinMsg, usb_srv_type);
#endif
}

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdultWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    USB_SERVICE_E       service_type = 0; //now it is not get from user_data, so do not support upcc.
    //if someday watch should support upcc, modify this function reference to HandleWaitWinMsg()

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_TRACE_LOW("[mmiudisk]: HandleAdultWaitWinMsg MSG_OPEN_WINDOW");
        break;

    case MSG_MMI_USB_SERVICE_STARTED:
        {
            if(MMI_USB_SERVICE_UDISK == *(MMI_USBSRV_TYPE *)param)
            {
#ifdef ADULT_WATCH_SUPPORT
                MMIUDISK_WatchClosePrompt();
                MMIUDISK_WatchOpenNotiPrompt(TXT_UDISK_START, usb_storage_connect_success);
#else
                MMK_CloseWin(alert_id);
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_UDISK_START,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
            }
            SCI_SLEEP(500);
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_MMI_USB_SERVICE_STOPPED:
        SCI_SLEEP(500);
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        break; 
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        break;
    }
    return (result);
}

#endif
/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    // static BOOLEAN      s_need_stop_service = FALSE;
	MMIPUB_INFO_T       *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
	USB_SERVICE_E       service_type = 0;//MMK_GetWinAddDataPtr(win_id);
	MMI_WIN_ID_T        alert_id = MMIFMM_UDISK_ALERT_WIN;

	if(win_info_ptr != PNULL)
	{
		service_type = (USB_SERVICE_E)win_info_ptr->user_data;
	}
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //SCI_TRACE_LOW:"[mmiudisk]: HandleUPCCUsingWindow MSG_OPEN_WINDOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_1017_112_2_18_3_4_11_57,(uint8*)"");
		if(USB_SERVICE_UPCC == service_type)
		{
		    MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_CUR);
		}
		result  = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
#ifdef USB_WEBCAMERA_SUPPORT     
    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"[mmiudisk]: HandleUPCCUsingWindow MSG_FULL_PAINT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_1026_112_2_18_3_4_11_58,(uint8*)"");
		if(USB_SERVICE_UPCC == service_type)
		{
			//GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,TXT_NULL,FALSE);
			//开启pc camera背光需常亮
			MMIDEFAULT_AllowTurnOffBackLight(FALSE);

			if(!MMIUDISK_OpenPCCamera() || !UPCC_StartVideoStreaming())
			{
				//open pc camera error
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
                MMK_CloseWin(win_id);		
			}
//			if (!USB_StartService(USB_SERVICE_UPCC))
//			{
//				MMIPUB_OpenAlertFailWin(TXT_ERROR);
//				MMK_CloseWin(win_id);
//				break;
// 			}
		}
        //SCI_TRACE_LOW:"[mmiudisk]: HandleUPCCUsingWindow MMIUDISK_OpenPCCamera sucess"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_1046_112_2_18_3_4_11_59,(uint8*)"");
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
#endif
	case MSG_MMI_USB_SERVICE_STARTED:
#ifdef USB_WEBCAMERA_SUPPORT
		if(MMI_USB_SERVICE_UPCC == *(MMI_USBSRV_TYPE *)param)
		{
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            
			if(!UPCC_StartVideoStreaming())
			{
				//open pc camera error
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
                MMK_CloseWin(win_id);		
			}
		}
		else 
#endif
		{
			if(MMI_USB_SERVICE_UDISK == *(MMI_USBSRV_TYPE *)param)
			{
				MMK_CloseWin(alert_id);
				MMIPUB_OpenAlertWinByTextId(PNULL,TXT_UDISK_START,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
			}
			MMK_CloseWin(win_id);
		}
		break;
	case MSG_MMI_USB_SERVICE_STOPPED:
        MMK_CloseWin(win_id);
		break;

    case MSG_LOSE_FOCUS:
        //SCI_TRACE_LOW:"[mmiudisk]: HandleUPCCUsingWindow MSG_LOSE_FOCUS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_1079_112_2_18_3_4_11_60,(uint8*)"");
		if(USB_SERVICE_UPCC == service_type)
		{
			//USB_StopService(USB_SERVICE_UPCC);
			MMIAPIUDISK_ClosePCCamera();
		}
		result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break; 
	case MSG_APP_RED:
		MMK_CloseWin(win_id);
		result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
		break;		
    case MSG_CLOSE_WINDOW:
#ifdef USB_WEBCAMERA_SUPPORT
		if(USB_SERVICE_UPCC == service_type)
		{
			USB_StopService(USB_SERVICE_UPCC);
			MMIAPIUDISK_ClosePCCamera();
			//恢复背光
			MMIDEFAULT_AllowTurnOffBackLight(TRUE);

			//@songbin.zeng,2006-08-21,memory leak
			result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
			MMK_CloseWin(MMIUDISK_UPCC_MAIN_MENU_WIN_ID);
			MMISRVAUD_FreeVirtualHandle("UDISK WAIT");
#ifdef UI_P3D_SUPPORT
            MMI_Enable3DEffect(MMI_3D_EFFECT_UDISK_PC);
#endif        

		}
#endif
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description :usb service started
//  Global resource dependence : 
//  Author:jassmine.meng
// te: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_USBServiceStartInd(MMI_USBSRV_TYPE usb_srv_type)
{
	if (MMI_USB_SERVICE_UDISK == usb_srv_type)
	{
		MMIFW_PumpUdiskMountEvent(DEV_UDISK, TRUE);
	}
	
     if(MMK_IsOpenWin(MMIUDISK_USB_SERVICE_START_WIN_ID))
	 {
		 MMK_PostMsg(MMIUDISK_USB_SERVICE_START_WIN_ID, MSG_MMI_USB_SERVICE_STARTED, &usb_srv_type, sizeof(MMI_USBSRV_TYPE));
	 }
}


/*****************************************************************************/
//  Discription: Handle blue tooth new file win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleUdiskStopWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
	 /*关机充电流程下直接关机*/
	 if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
	 {
		 MMIDEFAULT_TurnOffBackLight();
		 POWER_PowerOff();	
	 }
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return result;     
}

/*****************************************************************************/
//  Description :usb service stopped
//  Global resource dependence : 
//  Author:jassmine.meng
// te: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_USBServiceStopInd(MMI_USBSRV_TYPE usb_srv_type)
{
    if(MMI_USB_SERVICE_UDISK == usb_srv_type)
    {
        if(!MMIAPIUDISK_GetIsPowerOn())
        {
            MMIDEFAULT_TurnOffBackLight();
            POWER_PowerOff();
            return;
        }
        //MMIAPIUdisk_SetRunStatus(FALSE);
        //MMK_PostMsg(MMIUDISK_STOP_WAIT_WIN_ID, MSG_MMI_USB_SERVICE_STOPPED, &usb_srv_type, sizeof(MMI_USBSRV_TYPE));
        MMIUDISK_RestoreFS();
        //prompt disable
#ifdef DSP_USB_LOG
#ifndef WIN32
        if((BOOLEAN)REFPARAM_GetDspCardLogFlag())
        {
            SIO_DSPCardLogOpen();   //xuefang.jiang 20110802
        }
#endif
#endif
        MMIFW_PumpUdiskMountEvent(DEV_UDISK, FALSE);

        {
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
            MMI_STRING_T txt_wait = {0};
            WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
            MMI_GetLabelTextByLang(TXT_UDISK_WAIT_STOP_UDISK, &txt_wait);
            WatchCOM_NoteWin_1Line_Enter(MMIUDISK_STOP_WAIT_WIN_ID,&txt_wait,res_common_toast_bg_1,softkey,PNULL);
#else
            MMIUDISK_OpenWaitWin(USB_SERVICE_UDISK, MMIUDISK_STOP_WAIT_WIN_ID, TXT_UDISK_WAIT_STOP_UDISK);
            MMIPUB_SetWaitWinIsupdatedelay(MMIUDISK_STOP_WAIT_WIN_ID, FALSE);
#endif
        }

    }
    if(MMK_IsOpenWin(MMIUDISK_STOP_WAIT_WIN_ID))
    {
        MMK_PostMsg(MMIUDISK_STOP_WAIT_WIN_ID, MSG_MMI_USB_SERVICE_STOPPED, &usb_srv_type, sizeof(MMI_USBSRV_TYPE));
    }
    if(MMK_IsOpenWin(MMIUDISK_USB_SERVICE_START_WIN_ID))
    {
        MMK_PostMsg(MMIUDISK_USB_SERVICE_START_WIN_ID, MSG_MMI_USB_SERVICE_STOPPED, &usb_srv_type, sizeof(MMI_USBSRV_TYPE));
    }
    MMIAPIUdisk_SetRunStatus(FALSE);
    //MMIAPIUdisk_VUARTEndService();
}


/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISRVAUD_ReqVirtualHandle("UDISK WAIT", MMISRVAUD_PRI_NORMAL);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    case MSG_UDISK_FORMAT_CNF:
        MMIAPIUdisk_HandleFormatCnf(*(BOOLEAN *)param);
    //   if (MMIFMM_GetCurrentDevice() == s_current_device)
    //    {
            MMIAPIFMM_UpdateFmmList(s_current_device,TRUE);
   //     }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        break;
	case MSG_CLOSE_WINDOW:
             MMISRVAUD_FreeVirtualHandle("UDISK WAIT");
		break;
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

#ifdef USB_WEBCAMERA_SUPPORT
/*****************************************************************************/
//  Description : start pc camera service
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void StartUPCC(void)
{
    //start pc camera
    //SCI_TRACE_LOW:"[mmiudisk]: MMIUDISK_StartUPCC"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_1269_112_2_18_3_4_11_61,(uint8*)"");

    if(!MMIUDISK_OpenPCCamera())
    {
        //open pc camera error
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        
        return;
    }

    USB_StartService(USB_SERVICE_UPCC);
    
    //stop search file    
    MMIAPIFMM_SearchFileStop();
    
    MMITHEME_SetUpdateDelayCount(1);
    MMK_ReturnIdleWin();
    
    MMIUDISK_OpenWaitWin(USB_SERVICE_UPCC,  MMIUDISK_USB_SERVICE_START_WIN_ID, TXT_UDISK_PCC);
}

/********************************************************************************
 NAME:          AppendListItem
 DESCRIPTION:   
 PARAM IN:      list_id - 
 PARAM OUT:     None
 AUTHOR:        songbin.zeng
 DATE:          2006.08.08
********************************************************************************/
LOCAL void AppendListItem(
                            MMI_CTRL_ID_T list_id,
                            MMI_TEXT_ID_T *text_id_tab_ptr,
                            int tab_len
                            )
{
    uint16                  i         = 0;
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

    if ((PNULL == text_id_tab_ptr)
        || (0 == tab_len)
       )
    {
        return;
    }
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;

    item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_COMMON_OK;
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN; 
    
    for ( i = 0; i < tab_len; i ++ )
    {
        item_data.item_content[0].item_data.text_id = text_id_tab_ptr[i];
        
        GUILIST_AppendItem( list_id, &item_t );
    }
}

/*****************************************************************************/
//Description : create list box
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CreateListBox(MMI_WIN_ID_T win_id,
                           MMI_CTRL_ID_T ctrl_id 
                          )
{
    GUI_BOTH_RECT_T   both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUILIST_INIT_DATA_T init_data = {0};
    uint16 cur_selection = 0;
    MMI_TEXT_ID_T  text_id_tab[] ={TXT_POSTPOSITIVE_SENSOR, TXT_PREPOSITIVE_SENSOR};
    int16 item_total_num = ARR_SIZE(text_id_tab);
    
    init_data.both_rect = both_rect;
    init_data.type = GUILIST_RADIOLIST_E;
       
    // 动态创建
    GUILIST_CreateListBox(win_id, 0, ctrl_id, &init_data);

    GUILIST_SetMaxItem(ctrl_id, item_total_num, FALSE );
    GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
    
    AppendListItem(
        ctrl_id, 
        text_id_tab, 
        item_total_num
        );
        
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    
    MMK_SetAtvCtrl(win_id,ctrl_id);
}

/*****************************************************************************/
//  Description : Handle UPCC  Sensor ID Win Msg
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUPCCSensorWinMsg(MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                      )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIUDISK_UPCC_MAIN_MENU_CTRL_ID;
    BOOLEAN ret = FALSE;
    int16 sensor_number = 0;
    SENSOR_ID_E sensor_id = 0;
    static MMIUDISK_UPCC_ERR_E upcc_err = 0;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        upcc_err = UPCC_ERR_CANCEL;
        MMISRVAUD_ReqVirtualHandle("UDISK WAIT", MMISRVAUD_PRI_NORMAL);
        
        //open dc
        ret = MMIUDISK_OpenPCCamera();

        //SCI_TRACE_LOW:"mmiudisk HandleUPCCSensorWinMsg openpccamera ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_1386_112_2_18_3_4_12_62,(uint8*)"d",ret);
        if (ret)
        {
            // 2 sensor
            sensor_number = MMIUDISK_GetSensorNumber();
            //here, to close pc camera for displaying menu
            MMIAPIUDISK_ClosePCCamera();
            
            if (2 <= sensor_number)
            {
                CreateListBox(win_id, MMIUDISK_UPCC_MAIN_MENU_CTRL_ID);
            }
            else
            {
                //send web msg to itself
                MMK_PostMsg(win_id, MSG_UDISK_USB_SERVICE_PC_CAMERA, PNULL, 0);
            }
        }
        else
        {
            upcc_err = UPCC_ERR_OPEN;
            MMK_CloseWin(win_id);                            
        }        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        upcc_err = UPCC_ERR_CANCEL;
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        sensor_id = (SENSOR_ID_E)GUILIST_GetCurItemIndex(ctrl_id);

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, sensor_id, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,sensor_id);

        MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, NULL);
        
        //here, to close pc camera for displaying menu
        ret = MMIUDISK_OpenPCCamera();
        if (ret)
        {
            if (MMIUDISK_SetPCCameraSensorID(sensor_id))
            {
                MMK_PostMsg(win_id, MSG_UDISK_USB_SERVICE_PC_CAMERA, PNULL, 0);
            }
            else
            {
               upcc_err = UPCC_ERR_SET;
               MMK_CloseWin(win_id);                             
            }
            
            MMIAPIUDISK_ClosePCCamera();
        }    
        else
        {
            upcc_err = UPCC_ERR_OPEN;
            MMK_CloseWin(win_id);                            
        }
        break;
        
    case MSG_UDISK_USB_SERVICE_PC_CAMERA:
        upcc_err = UPCC_SUCCESS;
        
        //open success,then close select win
        MMK_CloseWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
        MMK_CloseWin(win_id);
                
        StartUPCC();
        break;
    case MSG_CLOSE_WINDOW:
        //SCI_TRACE_LOW:"mmiudisk HandleUPCCSensorWinMsg MSG_CLOSE_WINDOW upcc_err=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_WINTAB_1458_112_2_18_3_4_12_63,(uint8*)"d",upcc_err);
                  
        if (upcc_err)
        {
            if (UPCC_ERR_CANCEL != upcc_err)
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            

            if (UPCC_ERR_SET == upcc_err)
            {
                MMIAPIUDISK_ClosePCCamera();
            }
            MMISRVAUD_FreeVirtualHandle("UDISK WAIT");

#ifdef UI_P3D_SUPPORT
            MMI_Enable3DEffect(MMI_3D_EFFECT_UDISK_PC);
#endif        
        }

        MMK_DestroyControl(ctrl_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif


#ifdef MCARE_V31_SUPPORT
PUBLIC void MCAREV31_StartUdisk(void)
{
	MINT win_id = MMIUDISK_USB_OPER_SELECT_WIN_ID;
	MMIAPISET_ValidatePrivacy(MMISET_PROTECT_USB, UDISK_ValidatePrivacyUsbOper, (void*)&win_id, sizeof(win_id));
}
#endif//#ifdef MCARE_V31_SUPPORT



				


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:55 2012*/ //IGNORE9527
