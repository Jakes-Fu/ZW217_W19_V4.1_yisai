/****************************************************************************
** File Name:      mmimms_edit.c                                           *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmimms_id.h"
#include "ffs.h"
#include "mmisd_export.h"
#include "mmipb_export.h"
#include "dal_time.h"
#include "mmk_type.h"
#include "mmi_appmsg.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmisms_export.h"
#include "mmisms_text.h"
#include "guiurl.h"
#include "mmienvset_export.h"
#include "mmicc_export.h"
#include "mmipub.h"
#include "mmimtv_export.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "gpio_prod_api.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget_export.h"
#endif
//#include "mmist_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmiudisk_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmimms_main.h"
#include "mmimms_export.h"
#include "mmimms_internal.h"
#include "mmimms_text.h"
#include "mmimms_net.h"
#include "mmimms_file.h"
#include "mmimms_push.h"
#include "mmiota_export.h"
#include "mmimms_setting.h"
#include "mmiidle_export.h"
#include "mmialarm_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmicountedtime_export.h"
#include "mmimms_control_main.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmidc_export.h"
#include "mmidm_export.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef STREAMING_HS_SUPPORT
#include "mmismp_export.h"
#endif

#ifdef MMIWIDGET_SUPPORT
#include "mmiwidget.h"
#include"mmiwidget_id.h"
#endif

#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#ifdef SCREENSAVER_SUPPORT 
#include "mmiss_export.h"
#endif
#include "Mmiidle_statusbar.h"
#include "mmifdn_export.h"
#include "mn_api.h"
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mmiatv_export.h"
#ifdef  SXH_APPLE_SUPPORT
#include "OEMOS.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_VIBRA_TIMEROUT       3000
#define MMIMMS_MEMSTATE_MAX_LEN     20
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_mms_entry_wins[MMS_ENTRY_WIN_MAX] = {0};   //窗口映射表，记录mms窗口是否打开
LOCAL uint8               s_play_mms_vibrator_timer_id = 0;  //play mms vibrator timer id
LOCAL uint32 mms_retry_recv_record_id = 0;
LOCAL BOOLEAN s_is_mms_noopen = FALSE;
MMIMMS_SEND_RECV_GLOBLEE_T g_mms_send_recv_add_data = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :new msg pop menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIMMS_HandleNewMsgWin(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                              );


/*****************************************************************************/
//  Description : mms play ring call back function
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void MMSRingPlayCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//  Description : add mms richtext items
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
#ifndef   MMI_GUI_STYLE_TYPICAL
LOCAL void AddMmsMemStateItems(
                               MMI_CTRL_ID_T ctrl_id,
                               GUILIST_ITEM_DATA_T *item_data,
                               uint16 *index,
                                 MMIMMS_MSG_TYPE_E msg_type);

#else
LOCAL void AddMmsMemStateItems(
                               MMI_CTRL_ID_T ctrl_id,
                               GUIRICHTEXT_ITEM_T *item_data,
                               uint16 *index,
                                 MMIMMS_MSG_TYPE_E msg_type);
#endif
/*****************************************************************************/
// 	Description: 检查所有的mms窗口是否有打开的
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIfEntryWinAllClose(void);

/*****************************************************************************/
// 	Description: 把MMS_ENTRY_WIN_E代表的窗口设置成为打开
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
LOCAL void SetEntryWinFlag(MMS_ENTRY_WIN_E entry,BOOLEAN is_open);

/*****************************************************************************/
//  Description : deal with delivery rpt
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIMMS_DeliveryRptArrive(MMIMMS_MSG_RECEIVED_T *recv_mms_info_ptr);

/*****************************************************************************/
//  Description : set now new mms record id
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void  MMIMMS_SetNewMMSId(uint32 record_id);

/*****************************************************************************/
//  Description : new mms or new notifycation notify win
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void MMIMMS_NewMMSOrNotiMsg(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id, uint32 record_id, BOOLEAN is_alert_win);

/*****************************************************************************/
//  Description : set now operation download record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void  MMIMMS_SetNowDownloadMMSId(uint32 record_id);

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsForbitNewMsgWin(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: 把MMS_ENTRY_WIN_E代表的窗口设置成为打开
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
LOCAL void SetEntryWinFlag(MMS_ENTRY_WIN_E entry,BOOLEAN is_open)
{
    if(MMS_ENTRY_WIN_MAX > entry)
    {
        s_mms_entry_wins[entry] = is_open;
    } 
}

/*****************************************************************************/
// 	Description: 检查所有的mms窗口是否有打开的
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIfEntryWinAllClose(void)
{
    BOOLEAN result = TRUE;
    int8 win_index = 0;
    for(; win_index < MMS_ENTRY_WIN_MAX; win_index++)
    {
        if(s_mms_entry_wins[win_index])
        {
            //SCI_TRACE_LOW:"MMIMMS_CheckIfEntryWinAllOpen,%d opened"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_219_112_2_18_2_38_25_194,(uint8*)"d",win_index);
            result = FALSE;
            break;
        }
    }
    return result;
}

/*****************************************************************************/
// 	Description: 彩信窗口初始化函数，里面包括mms模块内存的初始化
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
void MMIMMS_WinInit(MMS_ENTRY_WIN_E mms_entry_e)
{
    BOOLEAN check_res = FALSE;
    
    check_res = CheckIfEntryWinAllClose();//调用CheckIfEntryWinAllOpen检查是否其他窗口打开，然后SetEntryWinFlag
    SetEntryWinFlag(mms_entry_e , TRUE);
    if (TRUE == check_res)//初始化模块内存
    {
        MMIMMS_InitBlock();
    }       
}

/*****************************************************************************/
// 	Description: 彩信窗口退出函数，里面包括mms模块内存的销毁化
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
void MMIMMS_WinExit(MMS_ENTRY_WIN_E mms_entry_e)
{    
    BOOLEAN check_res = FALSE;
    
    SetEntryWinFlag(mms_entry_e , FALSE);
    check_res = CheckIfEntryWinAllClose();//调用CheckIfEntryWinAllOpen检查是否其他窗口打开，然后SetEntryWinFlag    
    if (TRUE == check_res)//销毁模块内存
    {
        MMIMMS_DestroyBlock();
    }     
}

/*****************************************************************************/
//  Description :create new mms msg pop win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CreateNewMmsMsgWin(void)
{
    //MMK_CreateWin((uint32 *)MMIMMS_EDIT_MENU_TAB,PNULL);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void SMS_ValidatePrivacyReadMMS(BOOLEAN is_permitted, DPARAM param_ptr)
{
    uint32 record_id = 0;
    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            record_id = *(uint32*)param_ptr;
            if (0 != record_id)
            {
                MMIAPIMMS_ReadMMS(record_id);
            }
        }
    }
}

/*****************************************************************************/
//  Description :new msg pop menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIMMS_HandleNewMsgWin(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint32 record_id      = 0;
    MMISMS_ORDER_INFO_T *cursor_ptr  = PNULL;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;

    record_id = MMIMMS_GetNewMMSId();
    if (0 != record_id)
    {
        cursor_ptr = MMIMMS_GetListItemByID(record_id);
        if (PNULL != cursor_ptr)
        {
            folder_type = cursor_ptr->folder_type;
        }
    }
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        //todo  左软键是进入，右软键是退出
        if (
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_IsPhoneLocked() ||
#endif
            s_is_mms_noopen 
            || MMIAPISMS_IsInSMSWin()
            || MMIAPIMMS_IsInMMSWin()
#ifdef MMI_OTA_SUPPORT
            || MMIAPIOTA_IsInOTAWin()
#endif
            || MMISMS_FOLDER_SECURITY == folder_type//安全信箱下载完成新彩信只是提示，不要求阅读
            )
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
#ifdef CMCC_TEST_FLAG
            //CMCC 入库测试修改
            if(MMIAPIDC_IsOpened())
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
				SCI_TRACE_LOW("MMIMMS_HandleNewMsgWin one softkey");
            }
            else
#endif
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_READ, STXT_EXIT, FALSE);
            }
        }
        err_code = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //todo 进入收件箱
        if (s_is_mms_noopen)
        {
            break;
        }
        if (MMISMS_FOLDER_SECURITY == folder_type)
        {
            break;
        }
        if (   MMIAPISMS_IsInSMSWin()
            || MMIAPIMMS_IsInMMSWin()
#ifdef MMI_OTA_SUPPORT
            || MMIAPIOTA_IsInOTAWin()
#endif
            )
        {
            break;
        }
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {   
#ifdef MET_MEX_SUPPORT        
            if(MMIMEX_ConflictPromt())
            {
                // mex kernel is running and may result in mem conflict
                break;
            }
#endif                
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, SMS_ValidatePrivacyReadMMS, &record_id, sizeof(record_id));
            MMK_CloseWin(win_id);
        }        
        break;

case MSG_TIMER:
        //SCI_TRACE_LOW:"MMISMS_HandleNewMsgWin %d"
        SCI_TRACE_LOW("MMIMMS_HandleNewMsgWin GetVibStatus:%d", MMIAPIENVSET_GetVibStatus());
        if (MMIENVSET_IsOnetimeVibTimerOut(param))
        {
            if (MMIAPIENVSET_GetVibStatus())
            {
                MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//关振动
                MMIAPIENVSET_SetVibStatus(FALSE);
                MMIENVSET_StartOnetimeVibTimer();
            }
            else
            {
                MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_MSG);
                MMIAPIENVSET_SetVibStatus(TRUE);
                MMIENVSET_StartOnetimeVibTimer();
            }
        }
        else
        {
            err_code = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        }
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    case MSG_CLOSE_WINDOW:
        //todo 停止响铃
        s_is_mms_noopen = FALSE;
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
        err_code = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default :
        err_code = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description : recieve noti and now download mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_RecvAndDownloadMMS(void)
{
    MMIMMS_MSG_RECEIVED_T *temp_recv_mms_info_ptr = PNULL;

    temp_recv_mms_info_ptr = g_mms_send_recv_add_data.recv_mms_info_ptr;
    if (PNULL == temp_recv_mms_info_ptr)
    {
        return;
    }

    if (MMS_PUSH_TYPE_DELIVERY_IND == temp_recv_mms_info_ptr->type)
    {
        //送达报告处理
        MMIMMS_DeliveryRptArrive(temp_recv_mms_info_ptr);
        //MMIMMS_PlayMMSSound();
    }
    else if (MMS_PUSH_TYPE_NOTIFICATION_IND == temp_recv_mms_info_ptr->type
          || MMS_PUSH_TYPE_READORIG_IND == temp_recv_mms_info_ptr->type)
    {
#ifdef MET_MEX_SUPPORT
        if(MMIMEX_IsSysRunning())
        {
            // mex application is running, let mex to process
            return;
        }
#endif        
        if (MMIMMS_PUSH_NEW == temp_recv_mms_info_ptr->push_status)
        {
            //提示新彩信通知,播放铃声或者震动 
            MMIMMS_NewMMSOrNotiMsg(MMIMMS_NEWMSG_WIN_ID, TXT_MMS_NEW_MMS, temp_recv_mms_info_ptr->record_id, FALSE);
        }
        else
        {
            //error open alert win
            MMIMMS_RecvMMSAlertWin(temp_recv_mms_info_ptr->push_status);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMMS_RecvAndDownloadMMS error type nor delivery or mms noti"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_426_112_2_18_2_38_26_195,(uint8*)"");
    }        
}

/*****************************************************************************/
//  Description : new mms or new notifycation notify win
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void MMIMMS_NewMMSOrNotiMsg(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id, uint32 record_id, BOOLEAN is_alert_win)
{
    MMI_STRING_T prompt_str         = {0};
    uint32  time                    = 0;      
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T alert_win_id       = win_id;
    MN_DUAL_SYS_E             dual_sys  = MN_DUAL_SYS_1;
    MMISMS_ORDER_INFO_T * mms_info_ptr = PNULL;
    MMIPUB_SOFTKEY_STYLE_E softkey_style = MMIPUB_SOFTKEY_CUSTOMER;

    MMIDEFAULT_SetBackLight(TRUE);
    MMIMMS_SetNewMMSId(record_id);
#if 0  // cr135373 stack overload  
    if (MMK_IsOpenWin(alert_win_id))
    {
        MMK_CloseWin(alert_win_id);
    }
#endif    

    if (MMIMMS_GetIsStopDownloadForFlyMode())//如果飞行模式停止下载彩信则不提示彩信下载失败
    {
        return;
    }
	#ifdef  SXH_APPLE_SUPPORT//sxh az  彩信
    	  if(AppleIsRun())
    	  	{
    	  		MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMIMMS_VIBRA_TIMEROUT);
	  		return ;
    	  	}
	#endif  
    //提示新彩信,播放铃声或者震动 
    if(IsForbitNewMsgWin())
    {
        win_priority = WIN_LOWEST_LEVEL;
    }
#ifdef VIDEO_PLAYER_SUPPORT
    else if(MMIAPIVP_IsOpenPlayerWin())
    {
        win_priority = WIN_LOWEST_LEVEL;
    }
#endif  
    else
    {
        win_priority = WIN_ONE_LEVEL;
        if(MMIAPISMS_IsInSMSWin()
           || MMIAPIMMS_IsInMMSWin()
#ifdef MMI_OTA_SUPPORT
           || MMIAPIOTA_IsInOTAWin()
#endif
          )
        {
    	    SCI_TRACE_LOW("MMIAPISMS_IsInSMSWin");
            softkey_style = MMIPUB_SOFTKEY_ONE;
        }
    }
    
#ifdef CMCC_TEST_FLAG
    //CMCC 入库测试修改
    if(MMIAPIDC_IsOpened())
    {
    	SCI_TRACE_LOW("MMIAPIDC_IsOpened");
        softkey_style = MMIPUB_SOFTKEY_ONE;
    }
#endif

#ifdef MET_MEX_SUPPORT
    if(MMIMEX_IsSysKernalRunning())
    {
        // mex is running show in background
        win_priority = WIN_LOWEST_LEVEL;
        softkey_style = MMIPUB_SOFTKEY_ONE;
    	SCI_TRACE_LOW("MMIMEX_IsSysKernalRunning");
    }
#endif
    SCI_TRACE_LOW("MMIMMS_NewMMSOrNotiMsg softkey_style = %d, is_alert_win = %d",softkey_style, is_alert_win);
    MMI_GetLabelTextByLang(text_id, &prompt_str);    
    //SCI_TRACE_LOW:"MMIMMS_NewMMSOrNotiMsg p:%d, s:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_487_112_2_18_2_38_26_196,(uint8*)"dd", win_priority, s_is_mms_noopen);
    if (s_is_mms_noopen)
    {        
        if (is_alert_win)
        {
            if (MMK_IsOpenWin(alert_win_id))// cr135373 stack overload
            {
                MMIPUB_SetAlertWinTextByPtr(alert_win_id,&prompt_str,PNULL,TRUE);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextPtr(&time,&prompt_str,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,&win_priority,softkey_style,MMIMMS_HandleNewMsgWin);/*lint !e64*/
            }
        }
        else
        {
            if (   PNULL != g_mms_send_recv_add_data.recv_mms_info_ptr 
                && 0 != g_mms_send_recv_add_data.recv_mms_info_ptr->receiver[0])
            {
                uint16 temp_to_len = 0;
                MMI_STRING_T dilivery_to_str = {0};
                wchar dilivery_wch_to[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
                MMIPB_BCD_NUMBER_T bcd_num = {0};
                MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
                MMI_STRING_T  to_name_string = {0};
                wchar number_name_wstr[MMIPB_NAME_MAX_LEN + 1]= {0};
                BOOLEAN find_result = FALSE;

                temp_to_len = strlen((char *)g_mms_send_recv_add_data.recv_mms_info_ptr->receiver);
                temp_to_len = MIN(temp_to_len, MMIMMS_MAX_ADDRESS_LEN);

                MMIAPICOM_GenPartyNumber(g_mms_send_recv_add_data.recv_mms_info_ptr->receiver, temp_to_len, &party_num);
                bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                SCI_MEMCPY(bcd_num.number, party_num.bcd_num, bcd_num.number_len);
                to_name_string.wstr_ptr = number_name_wstr;                
                find_result = MMIAPIPB_GetNameByNumber(&bcd_num, &to_name_string, MMIPB_NAME_MAX_LEN, FALSE);
                if (find_result && 0 < to_name_string.wstr_len)
                {          
                    if (MMK_IsOpenWin(alert_win_id))// cr135373 stack overload
                    {
                        MMIPUB_SetAlertWinTextByPtr(alert_win_id,&to_name_string,&prompt_str,TRUE);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWinByTextPtr(&time,&to_name_string,&prompt_str,IMAGE_PUBWIN_SUCCESS,&alert_win_id,&win_priority,softkey_style,MMIMMS_HandleNewMsgWin);/*lint !e64*/
                    }
                }
                else
                {
                    MMI_STRNTOWSTR(dilivery_wch_to, MMIMMS_MAX_ADDRESS_LEN, g_mms_send_recv_add_data.recv_mms_info_ptr->receiver, MMIMMS_MAX_ADDRESS_LEN, temp_to_len);
                    dilivery_to_str.wstr_ptr = dilivery_wch_to;
                    dilivery_to_str.wstr_len = temp_to_len;
                    if (MMK_IsOpenWin(alert_win_id))// cr135373 stack overload
                    {
                        MMIPUB_SetAlertWinTextByPtr(alert_win_id,&dilivery_to_str,&prompt_str,TRUE);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWinByTextPtr(&time,&dilivery_to_str,&prompt_str,IMAGE_PUBWIN_SUCCESS,&alert_win_id,&win_priority,softkey_style,MMIMMS_HandleNewMsgWin);/*lint !e64*/
                    }
                }               
            }
            else
            {
                if (MMK_IsOpenWin(alert_win_id))// cr135373 stack overload
                {
                    MMIPUB_SetAlertWinTextByPtr(alert_win_id,&prompt_str,PNULL,TRUE);
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextPtr(&time,&prompt_str,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,&win_priority,softkey_style,MMIMMS_HandleNewMsgWin);/*lint !e64*/
                }
            } 
            MMIPUB_SetWinTitleTextId(alert_win_id, TXT_COMMON_PROMPT, FALSE);
        }
    }
    else
    {
        if (is_alert_win)
        {
            s_is_mms_noopen = TRUE;
            if (MMK_IsOpenWin(alert_win_id))// cr135373 stack overload
            {
                MMIPUB_SetAlertWinTextByPtr(alert_win_id,&prompt_str,PNULL,TRUE);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextPtr(&time,&prompt_str,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,&win_priority,softkey_style,MMIMMS_HandleNewMsgWin);/*lint !e64*/
            }
        }
        else
        {
#ifdef MMIWIDGET_SUPPORT
            if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
            {
                MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_NEWSMS_ID, FALSE, TRUE);            
            }

            if(MMK_IsFocusWin(MAIN_IDLE_WIN_ID) && (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()))
            {
            }
            else
#endif
//             if (MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle()
//                 || MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle()
//                 || WIN_ONE_LEVEL == win_priority)
            {
#ifdef CMCC_TEST_FLAG
                //CMCC 入库测试修改
                if(MMIAPIDC_IsOpened())
                {
                    MMIPUB_OpenAlertWinByTextPtr(&time,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,MMIMMS_HandleNewMsgWin);/*lint !e64*/
                }
                else
#endif
                {
                    if (MMK_IsOpenWin(alert_win_id))// cr135373 stack overload
                    {
                        MMIPUB_SetAlertWinTextByPtr(alert_win_id,&prompt_str,PNULL,TRUE);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWinByTextPtr(&time,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMIMMS_HandleNewMsgWin);/*lint !e64*/
                    }
                }

                MMIPUB_SetWinTitleTextId(alert_win_id, TXT_COMMON_PROMPT, FALSE);
            }
            
        }        
    }
    if (0 != record_id)
    {
        mms_info_ptr = MMIMMS_GetListItemByID(record_id);
    }
    if (PNULL != mms_info_ptr)
    {
        dual_sys = mms_info_ptr->dual_sys;
    }
    MMIMMS_PlayMMSSound(dual_sys);
/*wei.ren cr124504 add*/        
#ifdef PDA_UI_DROPDOWN_WIN
    if (PNULL != mms_info_ptr)
    {
        if(MMISMS_MT_NOT_DOWNLOAD != mms_info_ptr->mo_mt_type )
        {
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
        }
        else
        {
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MMS_NOTIFY);
        }
    }
#endif
/*wei.ren cr124504 add*/    
}

/*****************************************************************************/
//  Description : mms read detail and download mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_DetailAndDownloadMMS(MN_DUAL_SYS_E   dual_sys, uint32 record_id)
{
    MMIMMS_ERROR_E  edit_err = MMIMMS_NO_ERROR;
    //调用control函数启动后台下载，是否提示相应的原因
    edit_err = MMIMMS_ReceiveMMS(dual_sys, record_id);
    if (MMIMMS_NO_ERROR != edit_err)
    {
        //提示彩信初始化失败
        //SCI_TRACE_LOW:"MMIMMS_DetailAndDownloadMMS: call recv func fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_571_112_2_18_2_38_26_197,(uint8*)"");
        MMIMMS_EditMMSAlertWin(edit_err);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : is sim card ok
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsSimCardOK(MN_DUAL_SYS_E dual_sys)
{
    return MMIAPIPHONE_IsSimAvailable(dual_sys);
}

/*****************************************************************************/
//  Description : mms play ring call back function
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void MMSRingPlayCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_593_112_2_18_2_38_26_198,(uint8*)"d",result);

    MMIAPISET_AlertRingPlayResult(result, param);
    MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//maryxiao cr231219
    MMIENVSET_StopOnetimeVibTimer();
    MMIAPIENVSET_SetVibStatus(FALSE);
    //MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
}

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_HandleTimer(DPARAM param)
{
    if (s_play_mms_vibrator_timer_id == *(uint8*)param)
    {
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_MSG);
        //MMIAPISET_StopRing(MMISET_RING_TYPE_MSG); //changed by minghu.mao cr166768
        if (0 != s_play_mms_vibrator_timer_id)
        {
            MMK_StopTimer(s_play_mms_vibrator_timer_id);
            s_play_mms_vibrator_timer_id = 0;
        }
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : play mms sound
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
PUBLIC void MMIMMS_PlayMMSSound(MN_DUAL_SYS_E dual_sys)
{
    MMISET_MSG_RING_TYPE_E      ring_type = MMISET_MSG_RING;
    //MN_DUAL_SYS_E             dual_sys  = MN_DUAL_SYS_1;

    MMIDEFAULT_SetBackLight(TRUE);

    //MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_MSG);
    MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
    MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);

    /*
    if (0 != s_play_mms_vibrator_timer_id)
    {
        MMK_StopTimer(s_play_mms_vibrator_timer_id);
        s_play_mms_vibrator_timer_id = 0;
    }
    */
    
    //有新的需要播放铃声，把原来的播放铃声先停掉。
    ring_type = (MMISET_MSG_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(dual_sys, MSG_RING_TYPE);
    
    if (!MMIDEFAULT_IsPermitPlayNewMsgRing())
    {
        if(MMISET_MSG_RING == ring_type || MMISET_MSG_VIBRA_AND_RING == ring_type)
        {
            ring_type = MMISET_MSG_VIBRA;
        }
    }
    if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) && !MMIAPIUDISK_UdiskIsRun())
    {
        //SCI_TRACE_LOW:"MMS: PlayMMSRing normal call mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_656_112_2_18_2_38_26_199,(uint8*)"");
        MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG_IN_CALL,MMSRingPlayCallBack);
    }
    else 
    {
        //SCI_TRACE_LOW:"MMS: MMIMMS_PlayMMSSound ring_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_661_112_2_18_2_38_26_200,(uint8*)"d", ring_type);
		
#ifdef JAVA_SUPPORT
        if(MMIAPIJAVA_IsJavaRuning())
        {
            ring_type = MMISET_MSG_VIBRA;
        }
#endif
#ifdef WRE_SUPPORT
		if(WRE_IsFocusRunWin())
		{
			ring_type = MMISET_MSG_VIBRA;
		}
#endif		
        switch( ring_type )
        {
        case MMISET_MSG_RING:
            MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG,MMSRingPlayCallBack);
            //if (GPIO_CheckHeadsetStatus())     //耳机插入时启动振动
            //{
                //MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMIMMS_VIBRA_TIMEROUT);
                /*
                if ( 0 == s_play_mms_vibrator_timer_id)
                {
                    //MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_MSG);
                    s_play_mms_vibrator_timer_id = MMK_CreateWinTimer(VIRTUAL_WIN_ID, MMIMMS_VIBRA_TIMEROUT, FALSE);
                }
                */
            //}
            break;
            
        case MMISET_MSG_VIBRA:
            if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_MSG))
            {
                MMIAPISET_StopAllVibrator();
                MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMIMMS_VIBRA_TIMEROUT);
                /*
                if ( 0 == s_play_mms_vibrator_timer_id)
                {
                    //MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_MSG);
                    s_play_mms_vibrator_timer_id = MMK_CreateWinTimer(VIRTUAL_WIN_ID, MMIMMS_VIBRA_TIMEROUT, FALSE);
                }
                */
            }
            break;
            
        case MMISET_MSG_SILENT:
            break;
            
        case MMISET_MSG_VIBRA_AND_RING:
            if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_MSG))
            {
                if ( 0 == MMIAPIENVSET_GetActiveModeOptValue(dual_sys, MSG_RING_VOL) )
                {
                    MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMIMMS_VIBRA_TIMEROUT);
                }
                else
                {
                    MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//maryxiao cr231219
                    MMIENVSET_StopOnetimeVibTimer();
                    MMIAPIENVSET_SetVibStatus(FALSE);
                    MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG, MMSRingPlayCallBack);
                    // 开启震动
                    MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_MSG);
                    MMIENVSET_StartWinOnetimeVibTimer(MMIMMS_NEWMSG_WIN_ID); // MMIENVSET_StartWinOnetimeVibTimer(g_s_prompt_win_handle);
                    MMIAPIENVSET_SetVibStatus(TRUE);
                
                    /*
                    if ( 0 == s_play_mms_vibrator_timer_id)
                    {
                        //MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_MSG);
                        s_play_mms_vibrator_timer_id = MMK_CreateWinTimer(VIRTUAL_WIN_ID, MMIMMS_VIBRA_TIMEROUT, FALSE);
                    }
                    */
                }
            }
            break;
            
        default:
            break;
        }
    }
}

/*****************************************************************************/
// 	Description: deal with the control msg and dispatch the msg
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMMS_HandleControlMsg(
                                    uint16   	    msg_id,
                                    DPARAM        param
                                    )
{
    MMIMMS_MSG_RECEIVED_T *temp_recv_mms_info_ptr = PNULL;
    MMIMMS_ERROR_E op_error                       = MMIMMS_ERROR_MAX;

    switch(msg_id) 
    {
    case MSG_MMS_RECEIVED://收到彩信通知
        temp_recv_mms_info_ptr = (MMIMMS_MSG_RECEIVED_T *)param;
        if (PNULL != temp_recv_mms_info_ptr)
        {
            g_mms_send_recv_add_data.recv_mms_info_ptr = temp_recv_mms_info_ptr;
            MMIMMS_RecvAndDownloadMMS();
        }
        
    	break;
    case MSG_MMS_AUTODOWNLOAD_IND:
        if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
        {
            MMK_PostMsg(MMIMMS_DOWNLOADING_WIN_ID, msg_id, param,0);
        }
        break;
    case MSG_MMS_NETLINK_SUCCESS:       //彩信PDP激活成功，可以更新彩信下载图标状态
    case MSG_MMS_NETCONNECT_SUCCESS:
        {
            if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
            {
                MMK_PostMsg(MMIMMS_DOWNLOADING_WIN_ID, msg_id, param,0);
            }
            {//后台下载更新图标
                MMIMMS_PROGRESS_T * temp_progress_ptr = PNULL;                      

                temp_progress_ptr = (MMIMMS_PROGRESS_T *)param;
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_HandleControlMsg PDP Active Success!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_778_112_2_18_2_38_26_201,(uint8*)"");
                if ( 
                    MMIMMS_SENDRECV_SENDRESPONSE == temp_progress_ptr->type ||
                    MMIMMS_SENDRECV_SENDRPT == temp_progress_ptr->type)
                {//当收到彩信通知，手动下载时候回复彩信服务器不用更新图标
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_HandleControlMsg reply the recv mms!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_783_112_2_18_2_38_26_202,(uint8*)"");
                    break;
                }
                if(MMIMMS_SENDRECV_RECVING == temp_progress_ptr->type)
                {
                    g_mms_send_recv_add_data.is_downloading_mms = TRUE;
                    g_mms_send_recv_add_data.is_sending_mms = FALSE;
                    MMIMMS_SetNowDownloadMMSId(temp_progress_ptr->record_id);
                }
                else if(MMIMMS_SENDRECV_SENDING == temp_progress_ptr->type)
                {
                    g_mms_send_recv_add_data.is_sending_mms = TRUE;
                    g_mms_send_recv_add_data.is_downloading_mms = FALSE;
                    MMIMMS_SetNowDownloadMMSId(0);
                }
                else
                {
                    break;
                }
                
                MAIN_SetMsgState(TRUE);
                MMIAPISMS_UpdateMsgStatus();
            }            
        }        
        break;
    case MSG_MMS_DATA_RECEIVED:
        {
            MMIMMS_PROGRESS_T * temp_progress_ptr = PNULL;                      

            temp_progress_ptr = (MMIMMS_PROGRESS_T *)param;
            if (PNULL != temp_progress_ptr)
            {
                g_mms_send_recv_add_data.progress_ptr = temp_progress_ptr;
                if (mms_retry_recv_record_id != (uint32)temp_progress_ptr->record_id)
                {
                    MMIMMS_SetIsMmsRetry(FALSE);
                }             
                if (temp_progress_ptr->record_id == (int32)MMIMMS_GetNowReadMMSId())
                {
                    //如果当前阅读的彩信正在接收数据，则要关闭阅读界面，打开彩信接收界面
                    //开始下载后要关闭彩信通知阅读界面，防止正在阅读彩信通知下载后开始下载冲突
                    if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
                    {
                        MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
                    }
                    //MMSMMI_CreateDownloadWaitingWin(temp_progress_ptr->dual_sys, temp_progress_ptr->record_id);
                }
                if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
                {
                    MMK_PostMsg(MMIMMS_DOWNLOADING_WIN_ID, msg_id, PNULL,0);
                }
                else
                {
                    //当没有开启查看下载窗口的时候，不用处理。
                    //SCI_TRACE_LOW:"[MMIMMS]MSG_MMS_DATA_RECEIVED:download size cur_size=%d, total_size=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_822_112_2_18_2_38_26_203,(uint8*)"dd", temp_progress_ptr->current_size,temp_progress_ptr->total_size);
                }
            }
        }
        break;
    case MSG_MMS_RECV_END:
        {
            MMIMMS_MSG_RECV_END_T * temp_recv_ptr = PNULL;
            
            MMIMMS_SetIsMmsRetry(FALSE);

            temp_recv_ptr = (MMIMMS_MSG_RECV_END_T *)param;
            mms_retry_recv_record_id = 0;
            if (PNULL != temp_recv_ptr)
            {
                g_mms_send_recv_add_data.recv_ptr = temp_recv_ptr;
                {
                    //下载完成的时候把正在下载图标更新成为未读图标
                    MMIAPISMS_UpdateMsgStatus();
                }
                
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_HandleControlMsg recv error = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_843_112_2_18_2_38_26_204,(uint8*)"d", temp_recv_ptr->recv_error);

                if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
                {
                    MMK_PostMsg(MMIMMS_DOWNLOADING_WIN_ID, msg_id, PNULL,0);
                }
                else
                {
                    //idle 界面提示
                    //如果是自动下载成功，则提示新彩信，如果手动下载成功则不提示，如果下载失败则提示下载失败
                    op_error = temp_recv_ptr->recv_error;
                    if (MMIMMS_NO_ERROR == op_error)
                    {
                        //if (MMIMMS_RECV_NEWMMS == temp_recv_ptr->recv_type)
                        {
                            MMIMMS_NewMMSOrNotiMsg(MMIMMS_NEWMSG_WIN_ID, TXT_MMS_NEW_MMS, temp_recv_ptr->record_id, FALSE);
                        }
                    }
                    else
                    {
                        if (MMIMMS_ERROR_INVALIDPERIOD == op_error)
                        {
                            MMIMMS_NewMMSOrNotiMsg(MMIMMS_NEWMSG_WIN_ID, TXT_MMS_EXPIRED, temp_recv_ptr->record_id,TRUE);
                        }
                        else if(MMIMMS_ERROR_NO_SPACE == op_error)
                        {
                            MMIMMS_NewMMSOrNotiMsg(MMIMMS_NEWMSG_WIN_ID, TXT_MMS_FULLSPACE, temp_recv_ptr->record_id, TRUE);
                        }
                        //////////////////////////////////////////////////////////////////////////
                        // added by feng.xiao
                        #ifdef DATA_ROAMING_SUPPORT
                            else if (MMIMMS_ERROR_DATA_ROAMING_DISABLE == op_error)
                            {
                                MMIMMS_NewMMSOrNotiMsg(MMIMMS_NEWMSG_WIN_ID, TXT_DATA_ROAMING_DISABLED, temp_recv_ptr->record_id, TRUE);
                            }
                        #endif
                        else
                        {
                            MMIMMS_NewMMSOrNotiMsg(MMIMMS_NEWMSG_WIN_ID, TXT_COMMON_RECEIVE_FAIL, temp_recv_ptr->record_id, TRUE);
                        }                                           
                    }               
                }
            } 
        }        
        break;
    case MSG_MMS_DATA_SENT:
        {
            MMIMMS_PROGRESS_T * temp_progress_ptr = PNULL;
            
            temp_progress_ptr = (MMIMMS_PROGRESS_T *)param;
            if (PNULL != temp_progress_ptr)
            {
                g_mms_send_recv_add_data.progress_ptr = temp_progress_ptr;
            } 
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]MSG_MMS_DATA_SENT:temp_progress_ptr is NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_899_112_2_18_2_38_27_205,(uint8*)"");
                break;
            }
            if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
            {
                MMK_PostMsg(MMIMMS_SENDING_WIN_ID, msg_id, PNULL,0);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]MSG_MMS_DATA_SENT:send size cur_size=%d, total_size=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_908_112_2_18_2_38_27_206,(uint8*)"dd", temp_progress_ptr->current_size,temp_progress_ptr->total_size);
            }
        }
        break;
    case MSG_MMS_SEND_END:
        {
            MMIMMS_MSG_SEND_END_T * temp_send_ptr = PNULL;
            
            MMIMMS_SetIsMmsRetry(FALSE);
            
            temp_send_ptr = (MMIMMS_MSG_SEND_END_T *)param;
            if (PNULL != temp_send_ptr)
            {
                g_mms_send_recv_add_data.send_ptr = temp_send_ptr;
                op_error = temp_send_ptr->send_error; 
#if 0
                if (MMIMMS_NO_ERROR != op_error && temp_send_ptr->is_saved)
                {
                    MMISMS_ORDER_INFO_T *order_info_ptr = PNULL;
                    
                    order_info_ptr = MMIMMS_GetListItemByID(temp_send_ptr->record_id);
                    MMIAPISMS_UpdateChatShow(order_info_ptr);
                }
#endif
                if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
                {
                    MMK_PostMsg(MMIMMS_SENDING_WIN_ID, msg_id, PNULL,0);
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIMMS]MSG_MMS_SEND_END error id = %d, is_saved = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_929_112_2_18_2_38_27_207,(uint8*)"dd", op_error, temp_send_ptr->is_saved);
                    switch(op_error) 
                    {
                    case MMIMMS_NO_ERROR:
                        //提示发送成功，是否保存彩信
                        if (temp_send_ptr->is_saved)
                        {//提示发送成功
                            //MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_SUCCEED, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                
                        }
                        else 
                        {//提示发送成功彩信未保存
                            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_SUCCEED, TXT_MMI_SAVEFAILED, IMAGE_PUBWIN_SUCCESS);               
                        }
                        break;
                    case MMIMMS_ERROR_FAIL:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_NETLINK_FAILED:
                    case MMIMMS_ERROR_NETLINK_DISCONNECT:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_COMMON_LINK_FAILED, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_SERVICE_DENIED:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_SERVICE_DENIED, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_FORMAT_CORRUPT:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_FORMAT_CORRUPT, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_ADDRESS_UNRESOLVED:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_ADDRESS_UNRESOLVED, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_MESSAGE_NOT_FOUND:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_MESSAGE_NOT_FOUND, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_SERVER_BUSY:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_COMMON_TEXT_SERVER_BUSY, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_NOT_ACCEPTED:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_NOT_ACCEPTED, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_UNSUPORTED_MESSAGE:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_UNSUPORTED_MESSAGE, IMAGE_PUBWIN_WARNING);            
                        break;
                    case MMIMMS_ERROR_NETLINK_FDN_ONLY:
                        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_CC_FDN_ONLY, IMAGE_PUBWIN_WARNING);
                        break;
                    case MMIMMS_ERROR_CANCEL:
                        if (temp_send_ptr->is_saved)
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_CANCEL_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
                        }  
                        else
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_CANCEL_SUCCESS, TXT_MMI_SAVEFAILED, IMAGE_PUBWIN_SUCCESS);
                        }
                        break;
                    default:
                        break;
                    }
                }
                if (MMK_IsOpenWin(MMIMMS_SEND_MMSREADRPT_WAITING_WIN_ID))
                {
                    MMK_PostMsg(MMIMMS_SEND_MMSREADRPT_WAITING_WIN_ID, msg_id, PNULL,0);
                }
            }             
        }        
        break;
    case MSG_MMS_SAVE_END:
        break;
    case MSG_MMS_DELETEMMS_END:
        break;
    case MSG_MMS_NETDISCONNECT:     //彩信PDP去活完成，可以更新彩信下载图标状态
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_HandleControlMsg PDP deactive!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_998_112_2_18_2_38_27_208,(uint8*)"");
            g_mms_send_recv_add_data.is_downloading_mms = FALSE;
            g_mms_send_recv_add_data.is_sending_mms = FALSE;
            MMIMMS_SetNowDownloadMMSId(0);
            //启动闹钟
            if (!MMIAPIALM_CheckAndStartDelayedAlarm())
            {
                MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
            }
            MAIN_SetMsgState(TRUE);
        }
        break;
    case MSG_MMS_SEND_RETRYING:
        MMIMMS_SetIsMmsRetry(TRUE);
        if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
        {
            MMK_PostMsg(MMIMMS_SENDING_WIN_ID, msg_id, PNULL,0);
        }
        MMIMMS_SetIsMmsRetry(TRUE);
        break;
    case MSG_MMS_RECV_RETRYING:
        {
            MMIMMS_PROGRESS_T * temp_progress_ptr = PNULL;
            temp_progress_ptr = (MMIMMS_PROGRESS_T *)param;
            MMIMMS_SetIsMmsRetry(TRUE);
            mms_retry_recv_record_id = temp_progress_ptr->record_id;
            if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
            {
                MMK_PostMsg(MMIMMS_DOWNLOADING_WIN_ID, msg_id, PNULL,0);
            }
        }                
        break;
    default:
        break;
    }
}


#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : set mms memory info to richtext item
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetMMSMemState(MMI_CTRL_ID_T ctrl_id, GUILIST_ITEM_DATA_T *item_data, uint16 *index)
{
    if (PNULL == item_data || PNULL == index)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SetMMSMemState PNULL == item_data index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1044_112_2_18_2_38_27_209,(uint8*)"");
        return;
    }
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_MMS);
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_MMS_PUSH);
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_WAP_PUSH);
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_OTA);
    return;
}
#else
/*****************************************************************************/
//  Description : set mms memory info to richtext item
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetMMSMemState(MMI_CTRL_ID_T ctrl_id, GUIRICHTEXT_ITEM_T *item_data, uint16 *index)
{
    if (PNULL == item_data || PNULL == index)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SetMMSMemState PNULL == item_data index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1062_112_2_18_2_38_27_210,(uint8*)"");
        return;
    }
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_MMS);
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_MMS_PUSH);
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_WAP_PUSH);
    AddMmsMemStateItems(ctrl_id, item_data, index, MMIMMS_TYPE_OTA);
    return;
}
#endif
/*****************************************************************************/
//  Description : add mms richtext items
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
#ifndef MMI_GUI_STYLE_TYPICAL   //用list实现该界面
LOCAL void AddMmsMemStateItems(
                                 MMI_CTRL_ID_T ctrl_id,
                                 GUILIST_ITEM_DATA_T *item_data,
                                 uint16 *index,
                                 MMIMMS_MSG_TYPE_E msg_type)
{    
    uint8 mem_string[MMIMMS_MEMSTATE_MAX_LEN]            = {0};
    wchar mem_wstring[MMIMMS_MEMSTATE_MAX_LEN + 1]       = {0};    
    MMI_TEXT_ID_T	text_id                              = TXT_NULL;
    MMI_STRING_T mem_status                              = {0};
    MMI_STRING_T string_prefix                           = {0};
//  uint16 wstr_len                                      = 0;
    uint32 mms_num                                       = 0;
    uint32 mms_max_num                                   = 0;
    uint8 totalsize_string[MMIMMS_MEMSTATE_MAX_LEN]      = {0};
    wchar totalsize_wstring[MMIMMS_MEMSTATE_MAX_LEN + 1] = {0};
    uint8 used_size_string[MMIMMS_MEMSTATE_MAX_LEN]      = {0};
    wchar used_size_wstring[MMIMMS_MEMSTATE_MAX_LEN + 1] = {0};
    uint32 total_size                                    = 0;
    uint32 used_size                                     = 0;
    wchar display_wstr[MMIMMS_MEMSTATE_MAX_LEN]          = {0};
    wchar divided_wstr[]                                 = {'/'};
    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    if (PNULL == item_data || PNULL == index)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:AddMmsMemStateItems PNULL == item_data index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1102_112_2_18_2_38_27_211,(uint8*)"");
        return;
    }
    
    switch(msg_type)
    {
    case MMIMMS_TYPE_MMS://mms
        mms_num = MMIMMS_GetTotalNum();
        mms_max_num = MMIMMS_MAX_MMS_NUM;
        text_id = TXT_MMS_MMS_MEMORY;
        break;
        
    case MMIMMS_TYPE_MMS_PUSH://mms push
        mms_num = MMIMMS_GetUnsettleNum();
        mms_max_num = MMIMMS_MAX_MMS_NUM;
        text_id = TXT_MMS_UNDOWNLOAD_MMS_MEMORY;
        break;
        
    case MMIMMS_TYPE_WAP_PUSH://wap push
        //mms_num = MMIAPIBRW_GetPushTotalNum();
#ifdef MMI_OTA_SUPPORT
        mms_num = MMIAPIOTA_GetPushTotalNum();
#endif
        mms_max_num = MMICMSBRW_MAX_ITEM_NUM;
        text_id = TXT_MMS_WAP_PUSH_MEMORY;
        break;
        
    case MMIMMS_TYPE_OTA://ota
#ifdef MMI_OTA_SUPPORT
        mms_num = MMIAPIOTA_GetOTATotalNum();
#endif
        mms_max_num = MMIOTA_CONFIG_INFO_MAX;
        text_id = TXT_MMS_OTA_MEMORY;
        break;
        
    default:
        //SCI_TRACE_LOW:"[MMIMMS]:AddMmsMemStateItems default"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1134_112_2_18_2_38_27_212,(uint8*)"");
        break;
    }
    
    MMI_GetLabelTextByLang(text_id, &string_prefix);
    
    item_t.item_data_ptr = item_data;
    
    item_data->item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
   
    item_data->item_content[0].item_data.text_buffer.wstr_len = string_prefix.wstr_len;
    item_data->item_content[0].item_data.text_buffer.wstr_ptr = string_prefix.wstr_ptr;

    mem_status.wstr_ptr = mem_wstring;
    mem_status.wstr_len  = sprintf((char *)mem_string, "%ld/%ld", mms_num, mms_max_num);
    
    MMI_STRNTOWSTR(mem_status.wstr_ptr,
        MMIMMS_MEMSTATE_MAX_LEN,
        (uint8 *)mem_string,
        MMIMMS_MEMSTATE_MAX_LEN,
        mem_status.wstr_len);

    item_data->item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data->item_content[1].item_data.text_buffer.wstr_len = mem_status.wstr_len;
    item_data->item_content[1].item_data.text_buffer.wstr_ptr = mem_status.wstr_ptr;

    if (msg_type == MMIMMS_TYPE_MMS)
    {
        
        if (!MMIMMS_GetTotalSizeAndAllSize(&used_size, &total_size))
        {
            return;
        }
        //SCI_TRACE_LOW:"AddMmsMemStateItems used_size = %ld, total_size = %ld"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1167_112_2_18_2_38_27_213,(uint8*)"dd",used_size, total_size);
        _snprintf((void *)used_size_string, MMIMMS_MEMSTATE_MAX_LEN ,"%.1fM",(float)used_size/(ONE_K_LEN * ONE_K_LEN)); 
        if (((float)used_size/(ONE_K_LEN * ONE_K_LEN)) < MMIMMS_SIZE_FLOAT_A && ((float)used_size/(ONE_K_LEN * ONE_K_LEN)) > MMIMMS_SIZE_FLOAT_B)
        {
            //_snprintf((void*)used_size_string, MMIMMS_SLIDE_TIME_STR_LEN,"%.1fM",0.1);
	     _snprintf((void*)used_size_string, MMIMMS_MEMSTATE_MAX_LEN,"%.1fM",0.1);//converity
        }
        MMI_STRNTOWSTR(used_size_wstring, MMIMMS_MEMSTATE_MAX_LEN, (uint8 *)used_size_string, MMIMMS_MEMSTATE_MAX_LEN, sizeof(used_size_string));

        MMI_WSTRNCPY(display_wstr, MMIMMS_MEMSTATE_MAX_LEN, used_size_wstring, MMIMMS_MEMSTATE_MAX_LEN, sizeof(used_size_string));

        MMIAPICOM_Wstrncat(display_wstr, divided_wstr, 1);

        _snprintf((void *)totalsize_string, MMIMMS_MEMSTATE_MAX_LEN ,"%.1fM",(float)total_size/(ONE_K_LEN * ONE_K_LEN)); 
        if (((float)total_size/(ONE_K_LEN * ONE_K_LEN)) < MMIMMS_SIZE_FLOAT_A && ((float)total_size/(ONE_K_LEN * ONE_K_LEN)) > MMIMMS_SIZE_FLOAT_B)
        {
            //_snprintf((void*)totalsize_string, MMIMMS_SLIDE_TIME_STR_LEN,"%.1fM",0.1);
	     _snprintf((void*)totalsize_string, MMIMMS_MEMSTATE_MAX_LEN,"%.1fM",0.1);//converity
        }
        MMI_STRNTOWSTR(totalsize_wstring, MMIMMS_MEMSTATE_MAX_LEN, (uint8 *)totalsize_string, MMIMMS_MEMSTATE_MAX_LEN, sizeof(totalsize_string));
        
        MMIAPICOM_Wstrncat(display_wstr, totalsize_wstring, sizeof(totalsize_string));

        item_data->item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data->item_content[2].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(display_wstr);
        item_data->item_content[2].item_data.text_buffer.wstr_ptr = display_wstr;
        item_t.item_style = GUIITEM_STYLE_THREE_LINE_TEXT;
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    }
    GUILIST_AppendItem( ctrl_id, &item_t );
    
}
#else
LOCAL void AddMmsMemStateItems(
                                 MMI_CTRL_ID_T ctrl_id,
                                 GUIRICHTEXT_ITEM_T *item_data,
                                 uint16 *index,
                                 MMIMMS_MSG_TYPE_E msg_type)
{
    uint8 mem_string[MMIMMS_MEMSTATE_MAX_LEN]            = {0};
    wchar mem_wstring[MMIMMS_MEMSTATE_MAX_LEN + 1]       = {0};    
    MMI_TEXT_ID_T	text_id                              = TXT_NULL;
    MMI_STRING_T mem_status                              = {0};
    MMI_STRING_T string_prefix                           = {0};
//  uint16 wstr_len                                      = 0;
    uint32 mms_num                                       = 0;
    uint32 mms_max_num                                   = 0;
    uint8 totalsize_string[MMIMMS_MEMSTATE_MAX_LEN]      = {0};
    wchar totalsize_wstring[MMIMMS_MEMSTATE_MAX_LEN + 1] = {0};
    uint8 used_size_string[MMIMMS_MEMSTATE_MAX_LEN]      = {0};
    wchar used_size_wstring[MMIMMS_MEMSTATE_MAX_LEN + 1] = {0};
    uint32 total_size                                    = 0;
    uint32 used_size                                     = 0;
    wchar display_wstr[MMIMMS_MEMSTATE_MAX_LEN]          = {0};
    wchar divided_wstr[]                                 = {'/'};
    
    if (PNULL == item_data || PNULL == index)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:AddMmsMemStateItems PNULL == item_data index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1226_112_2_18_2_38_27_214,(uint8*)"");
        return;
    }
    
    switch(msg_type)
    {
    case MMIMMS_TYPE_MMS://mms
        mms_num = MMIMMS_GetTotalNum();
        mms_max_num = MMIMMS_MAX_MMS_NUM;
        text_id = TXT_MMS_MMS_MEMORY;
        break;
        
    case MMIMMS_TYPE_MMS_PUSH://mms push
        mms_num = MMIMMS_GetUnsettleNum();
        mms_max_num = MMIMMS_MAX_MMS_NUM;
        text_id = TXT_MMS_UNDOWNLOAD_MMS_MEMORY;
        break;
        
    case MMIMMS_TYPE_WAP_PUSH://wap push
        //mms_num = MMIAPIBRW_GetPushTotalNum();
#ifdef MMI_OTA_SUPPORT
        mms_num = MMIAPIOTA_GetPushTotalNum();
#endif
        mms_max_num = MMICMSBRW_MAX_ITEM_NUM;
        text_id = TXT_MMS_WAP_PUSH_MEMORY;
        break;
        
    case MMIMMS_TYPE_OTA://ota
#ifdef MMI_OTA_SUPPORT
        mms_num = MMIAPIOTA_GetOTATotalNum();
#endif
        mms_max_num = MMIOTA_CONFIG_INFO_MAX;
        text_id = TXT_MMS_OTA_MEMORY;
        break;
        
    default:
        //SCI_TRACE_LOW:"[MMIMMS]:AddMmsMemStateItems default"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1258_112_2_18_2_38_27_215,(uint8*)"");
        break;
    }
    
    MMI_GetLabelTextByLang(text_id, &string_prefix);
    
    item_data->text_data.buf.str_ptr = string_prefix.wstr_ptr;    
    item_data->text_data.buf.len = string_prefix.wstr_len;    
    GUIRICHTEXT_AddItem(ctrl_id, item_data, index);
    
    mem_status.wstr_ptr = mem_wstring;
    mem_status.wstr_len  = sprintf((char *)mem_string, "%ld/%ld", mms_num, mms_max_num);
    
    MMI_STRNTOWSTR(mem_status.wstr_ptr,
        MMIMMS_MEMSTATE_MAX_LEN,
        (uint8 *)mem_string,
        MMIMMS_MEMSTATE_MAX_LEN,
        mem_status.wstr_len);
    
    item_data->text_data.buf.str_ptr = mem_status.wstr_ptr;    
    item_data->text_data.buf.len = mem_status.wstr_len;
    GUIRICHTEXT_AddItem(ctrl_id, item_data, index);
    if (msg_type == MMIMMS_TYPE_MMS)
    {
        if (!MMIMMS_GetTotalSizeAndAllSize(&used_size, &total_size))
        {
            return;
        }
        //SCI_TRACE_LOW:"AddMmsMemStateItems used_size = %ld, total_size = %ld"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1286_112_2_18_2_38_27_216,(uint8*)"dd",used_size, total_size);
        _snprintf((void *)used_size_string, MMIMMS_MEMSTATE_MAX_LEN ,"%.1fM",(float)used_size/(ONE_K_LEN * ONE_K_LEN)); 
        if (((float)used_size/(ONE_K_LEN * ONE_K_LEN)) < MMIMMS_SIZE_FLOAT_A && ((float)used_size/(ONE_K_LEN * ONE_K_LEN)) > MMIMMS_SIZE_FLOAT_B)
        {
            _snprintf((void*)used_size_string, MMIMMS_SLIDE_TIME_STR_LEN,"%.1fM",0.1);
        }
        MMI_STRNTOWSTR(used_size_wstring, MMIMMS_MEMSTATE_MAX_LEN, (uint8 *)used_size_string, MMIMMS_MEMSTATE_MAX_LEN, sizeof(used_size_string));

        MMI_WSTRNCPY(display_wstr, MMIMMS_MEMSTATE_MAX_LEN, used_size_wstring, MMIMMS_MEMSTATE_MAX_LEN, sizeof(used_size_string));

        MMIAPICOM_Wstrncat(display_wstr, divided_wstr, 1);

        _snprintf((void *)totalsize_string, MMIMMS_MEMSTATE_MAX_LEN ,"%.1fM",(float)total_size/(ONE_K_LEN * ONE_K_LEN)); 
        if (((float)total_size/(ONE_K_LEN * ONE_K_LEN)) < MMIMMS_SIZE_FLOAT_A && ((float)total_size/(ONE_K_LEN * ONE_K_LEN)) > MMIMMS_SIZE_FLOAT_B)
        {
            _snprintf((void*)totalsize_string, MMIMMS_SLIDE_TIME_STR_LEN,"%.1fM",0.1);
        }
        MMI_STRNTOWSTR(totalsize_wstring, MMIMMS_MEMSTATE_MAX_LEN, (uint8 *)totalsize_string, MMIMMS_MEMSTATE_MAX_LEN, sizeof(totalsize_string));
        
        MMIAPICOM_Wstrncat(display_wstr, totalsize_wstring, sizeof(totalsize_string));

        item_data->text_data.buf.str_ptr = display_wstr;    
        item_data->text_data.buf.len = MMIAPICOM_Wstrlen(display_wstr);
        GUIRICHTEXT_AddItem(ctrl_id, item_data, index);
    }
}
#endif

/*****************************************************************************/
//  Description : alert window when receive new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_RecvMMSAlertWin(MMIMMS_PUSH_RECV_E mms_error_e)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T  win_id = MMIMMS_ALERT_WIN_ID;

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }    
    //提示新彩信,播放铃声或者震动     
#ifdef BROWSER_SUPPORT
    if (MMIAPIBROWSER_IsActive())
    {
        win_priority = WIN_ONE_LEVEL;
    }
#endif
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtConnecting())
    {
        win_priority = WIN_ONE_LEVEL;
    }
#endif

    if(MMIAPIMTV_IsPlaying())
    {
        win_priority = WIN_ONE_LEVEL;
    }
    
    switch(mms_error_e) 
    {
    case MMIMMS_PUSH_NEW_FULLSPACE://空间不足
    case MMIMMS_PUSH_DEFERRED:
        text_id = TXT_MMS_FULLSPACE;        
    	break;
    case MMIMMS_PUSH_INVALID_SIZE://彩信过大
        text_id = TXT_MMS_INVALID_SIZE;        
        break;
    case MMIMMS_PUSH_NEW_FULLNUM://彩信条数满
        text_id = TXT_MMS_FULL_NUMBER;        
        break;
    case MMIMMS_PUSH_EXPIRED://彩信过期
        text_id = TXT_MMS_EXPIRED;        
    	break;
    case MMIMMS_PUSH_RETRIVED:
    case MMIMMS_PUSH_REJECTED:    
    case MMIMMS_PUSH_UNRECOGNISED:
    case MMIMMS_PUSH_SFS_ERROR:
        text_id = TXT_MMS_RECV_ERROR;        
        break;
    default:
        break;
    }
    if (TXT_NULL != text_id)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,&win_id,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : deal with delivery rpt
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIMMS_DeliveryRptArrive(MMIMMS_MSG_RECEIVED_T *recv_mms_info_ptr)
{
//  uint32  time = 0;
    
    //SCI_TRACE_LOW:"MMIMMS_DeliveryRpt_Arrive IN "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1386_112_2_18_2_38_28_217,(uint8*)"");
    if (MMK_IsOpenWin(MMIMMS_ARRIVE_RPT_WIN_ID))
    {
        MMK_CloseWin(MMIMMS_ARRIVE_RPT_WIN_ID);
    }
#ifdef SCREENSAVER_SUPPORT	
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif 
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_DeliveryRptArrive push_status is%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_MAIN_NEW_1397_112_2_18_2_38_28_218,(uint8*)"d",recv_mms_info_ptr->push_status);
    switch(recv_mms_info_ptr->push_status)
    {
    case MMIMMS_PUSH_RETRIVED:
        s_is_mms_noopen = TRUE;
        MMIMMS_NewMMSOrNotiMsg(MMIMMS_ARRIVE_RPT_WIN_ID, TXT_DELIVERYRPT_RETRIVED, 0, FALSE);
        break;
    case MMIMMS_PUSH_REJECTED:
        s_is_mms_noopen = TRUE;
        MMIMMS_NewMMSOrNotiMsg(MMIMMS_ARRIVE_RPT_WIN_ID, TXT_DELIVERYRPT_REJECTED, 0, FALSE);
        break; 
    case  MMIMMS_PUSH_DEFERRED:
        s_is_mms_noopen = TRUE;
        MMIMMS_NewMMSOrNotiMsg(MMIMMS_ARRIVE_RPT_WIN_ID, TXT_DELIVERYRPT_DEFERRED, 0, FALSE);
        break; 
    case MMIMMS_PUSH_EXPIRED:
        s_is_mms_noopen = TRUE;
        MMIMMS_NewMMSOrNotiMsg(MMIMMS_ARRIVE_RPT_WIN_ID, TXT_DELIVERYRPT_EXPIRED, 0, FALSE);
        break; 
    case MMIMMS_PUSH_UNRECOGNISED:
        s_is_mms_noopen = TRUE;
        MMIMMS_NewMMSOrNotiMsg(MMIMMS_ARRIVE_RPT_WIN_ID, TXT_DELIVERYRPT_UNRECOGNISED, 0, FALSE);
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMMS_IsDownloadingMMS(void)
{
    return g_mms_send_recv_add_data.is_downloading_mms;
}

/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMMS_IsSendingMMS(void)
{
    return g_mms_send_recv_add_data.is_sending_mms;
}

/*****************************************************************************/
//  Description : set now operation  read record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetNowReadMMSId(uint32 record_id)
{
    g_mms_send_recv_add_data.mms_noti_op_record_id = record_id;
}

/*****************************************************************************/
//  Description : get now operation  read record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetNowReadMMSId(void)
{
    return g_mms_send_recv_add_data.mms_noti_op_record_id;
}

/*****************************************************************************/
//  Description : set now operation download record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void  MMIMMS_SetNowDownloadMMSId(uint32 record_id)
{
    g_mms_send_recv_add_data.now_downloading_record_id = record_id;
}

/*****************************************************************************/
//  Description : get now operation download record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetNowDownloadMMSId(void)
{
    return g_mms_send_recv_add_data.now_downloading_record_id;
}

/*****************************************************************************/
//  Description : set now new mms record id
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void  MMIMMS_SetNewMMSId(uint32 record_id)
{
    g_mms_send_recv_add_data.new_mms_record_id = record_id;
}

/*****************************************************************************/
//  Description : get now new mms record id
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetNewMMSId(void)
{
    return g_mms_send_recv_add_data.new_mms_record_id;
}

/*****************************************************************************/
//  Description : set now operation  mms retry send or receive
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetIsMmsRetry(BOOLEAN is_retry)
{
    g_mms_send_recv_add_data.is_mms_retry = is_retry;
}

/*****************************************************************************/
//  Description : get now operation  mms retry send or receive
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetIsMmsRetry(void)
{
    return g_mms_send_recv_add_data.is_mms_retry;
}

/*****************************************************************************/
//  Description : set is stop mms downloading for fly mode open
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetIsStopDownloadForFlyMode(BOOLEAN is_open)
{
    g_mms_send_recv_add_data.is_stop_download_for_fly_mode = is_open;
}

/*****************************************************************************/
//  Description : get is stop mms downloading for fly mode open
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetIsStopDownloadForFlyMode(void)
{
    return g_mms_send_recv_add_data.is_stop_download_for_fly_mode;
}

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsForbitNewMsgWin(void)
{
    if( MMIAPIUdisk_IsOpenPcCamera() 
#ifdef MMI_RECORD_SUPPORT
        || MMIAPIRECORD_IsRecording() 
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#ifdef CMCC_TEST_FLAG
        || MMIAPIVP_IsPlayWinFocus()
#endif
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
#if !defined(CMCC_TEST_FLAG)
        || MMIAPIDC_IsOpened()
#endif
        || MMIAPIDM_GetHandsetLock()
        || MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)
#ifdef   MMI_PDA_SUPPORT
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
        || MMK_IsFocusWin(MMIKL_PSW_WIN_ID)
#endif
#endif    
		|| MMIAPIATV_IsRecording()
#if defined(CMCC_UI_STYLE)
        || (MMIAPICC_IsInState(CC_IN_CALL_STATE))
#endif
#ifdef BROWSER_SUPPORT
        || MMIAPIBROWSER_IsRunning()
#endif
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsAllowSendMMSByFDN(MN_DUAL_SYS_E dual_sys, MMI_STRING_T edit_number)
{ 
    BOOLEAN is_allow_mms = TRUE;
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MN_CALLED_NUMBER_T called_num= {0};
    MMI_STRING_T to_string = {0};
    wchar *number_wstr = PNULL;
    wchar *cur_wchar = PNULL;
    uint8 to_nuber[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    wchar cur_to_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    uint16 cur_unber_len = 0;
    uint16 tem_unber_len = 0;
    uint16 total_num_len = 0;

    fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);
    if (PNULL == fdn_context_ptr || !fdn_context_ptr->is_fdn_enable)//fdn没有开启
    {
        return TRUE;
    }
    to_string = edit_number;
    number_wstr = to_string.wstr_ptr;
    cur_wchar = number_wstr;
    while ('\0' != *cur_wchar && to_string.wstr_len > total_num_len)
    {
        cur_unber_len++;
        total_num_len++;
        if (';' == *cur_wchar || '\0' == *(cur_wchar + 1))
        {
            if (';' != *cur_wchar && '\0' == *(cur_wchar + 1))
            {
                tem_unber_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_unber_len);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wchar - cur_unber_len + 1, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                MMI_WSTRNTOSTR(to_nuber, MMIMMS_MAX_ADDRESS_LEN, cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                if (MMIAPICOM_IsPhoneNumber(to_nuber, tem_unber_len) && MMIAPICOM_GenPartyNumber(to_nuber, tem_unber_len, &party_num))
                {
                    called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
                    called_num.number_type = party_num.number_type;
                    called_num.num_len = party_num.num_len;
                    SCI_MEMCPY(called_num.party_num, party_num.bcd_num, party_num.num_len);
                    if (!PHONEBOOK_CheckFdnEx(dual_sys, &called_num))
                    {
                        is_allow_mms = FALSE;
                        break;
                    }    
                }
                else
                {
                    is_allow_mms = FALSE;
                    break;
                }
                cur_unber_len = 0;
            }
            else
            {
                tem_unber_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_unber_len - 1);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wchar - cur_unber_len + 1, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                MMI_WSTRNTOSTR(to_nuber, MMIMMS_MAX_ADDRESS_LEN, cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                if (MMIAPICOM_IsPhoneNumber(to_nuber, tem_unber_len) && MMIAPICOM_GenPartyNumber(to_nuber, tem_unber_len, &party_num))
                {
                    called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
                    called_num.number_type = party_num.number_type;
                    called_num.num_len = party_num.num_len;
                    SCI_MEMCPY(called_num.party_num, party_num.bcd_num, party_num.num_len);
                    if (!PHONEBOOK_CheckFdnEx(dual_sys, &called_num))
                    {
                        is_allow_mms = FALSE;
                        break;
                    }             
                }
                else
                {
                    is_allow_mms = FALSE;
                    break;
                }
                cur_unber_len = 0;
            }
        }
        cur_wchar = cur_wchar + 1;
    }
    return is_allow_mms;
}

/*****************************************************************************/
//  Description : is MMS number Valid
//  Global resource dependence : 
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsMMSNumberValid(MMI_STRING_T edit_number)
{ 
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MMI_STRING_T to_string = {0};
    wchar *number_wstr = PNULL;
    wchar *cur_wchar = PNULL;
    uint8 to_nuber[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    wchar cur_to_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    uint16 cur_unber_len = 0;
    uint16 tem_unber_len = 0;
    uint16 total_num_len = 0;
    BOOLEAN is_valid = TRUE;

    to_string = edit_number;
    number_wstr = to_string.wstr_ptr;
    cur_wchar = number_wstr;
    while ('\0' != *cur_wchar && to_string.wstr_len > total_num_len)
    {
        cur_unber_len++;
        total_num_len++;
        if (';' == *cur_wchar || '\0' == *(cur_wchar + 1))
        {
            if (';' != *cur_wchar && '\0' == *(cur_wchar + 1))
            {
                int32 email_len = 0;

                tem_unber_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_unber_len);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wchar - cur_unber_len + 1, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                MMI_WSTRNTOSTR(to_nuber, MMIMMS_MAX_ADDRESS_LEN, cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                if (MMIAPICOM_IsPhoneNumber(to_nuber, tem_unber_len) && MMIAPICOM_GenPartyNumber(to_nuber, tem_unber_len, &party_num))
                {
                    is_valid = TRUE;
                }
                else if (URL_IsMailtoDefault((URL_CHAR_T const *)cur_to_wstr, &email_len))
                {
                    is_valid = TRUE;
                }
                else
                {
                    is_valid = FALSE;

                    break;
                }

                cur_unber_len = 0;
				SCI_MEMSET(cur_to_wstr,0,(MMIMMS_MAX_ADDRESS_LEN + 1));
            }
            else
            {
                int32 email_len = 0;

                tem_unber_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_unber_len - 1);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wchar - cur_unber_len + 1, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                MMI_WSTRNTOSTR(to_nuber, MMIMMS_MAX_ADDRESS_LEN, cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, tem_unber_len);
                if (MMIAPICOM_IsPhoneNumber(to_nuber, tem_unber_len) && MMIAPICOM_GenPartyNumber(to_nuber, tem_unber_len, &party_num))
                {
                    is_valid = TRUE;
                }
                else if (URL_IsMailtoDefault((URL_CHAR_T const *)cur_to_wstr, &email_len))
                {
                    is_valid = TRUE;
                }
                else
                {
                    is_valid = FALSE;

                    break;
                }

                cur_unber_len = 0;
				SCI_MEMSET(cur_to_wstr,0,(MMIMMS_MAX_ADDRESS_LEN + 1));
            }
        }
        cur_wchar = cur_wchar + 1;
    }

    return is_valid;
}

