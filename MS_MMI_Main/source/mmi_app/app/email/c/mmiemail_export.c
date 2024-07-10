/****************************************************************************
** File Name:      mmiemail_export.c                                       *
** Author:                                                         	       *
** Date:           05/13/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_email_trc.h"
#include "mmk_app.h"
#include "mmiemail_export.h"
#include "mmiemail_edit.h"
#include "mmiemail_view.h"
#include "brweng_tf2hz_email.h"
#include "EMS_Store_API.h"
#include "mmiemail_id.h"
#include "brweng_tf2hz_email.h"
#include "mmiemail_setting.h"
#include "mmiemail_net.h"
#include "mmiemail_utils.h"
#include "mmiemail_sp_api.h"
#include "email_text.h"
#if !defined(MMI_PDA_SUPPORT)
#include "mmiemail_internal.h"
#endif

#include "tf3rd_for_hz.h"
#include "mmiemail_menutable.h"
#include "mmiphone_export.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmiemail_signal.h"

#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmi_default.h"
#include "window_parse.h"
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
#include "mmisrvfmm_export.h"
#include "mmiemail_edit.h"
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
#ifdef MMI_WIFI_SUPPORT
extern BOOLEAN       g_email_is_use_wifi;
#endif
extern HEmlSPMgr     g_email_sp_handle;
extern BOOLEAN       g_email_pdp_active;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_mmiemail_app = {0};  /*lint !e552*/
EMAOPT_T        *g_email_opt_ptr = PNULL;
static EMAIL_VECTOR_T  *s_email_act_vector = PNULL;

uint16         inbox_sort_type = EMA_BOXS_ORDERBY_REC_NEW;
uint16         other_box_sort_type = EMA_BOXS_ORDERBY_DATE;
BOOLEAN        g_email_is_engine_task_busy = FALSE;
extern BOOLEAN           g_save_email_on_red_key;
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
LOCAL MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T s_pb_edit_doc = {0};
LOCAL BOOLEAN s_is_pb_send_edit =0;
#endif
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: handle select email account window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCheckAccountWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );
/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIEMAIL_CHECK_ACCOUNT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleCheckAccountWinMsg),    
        WIN_ID(EMAIL_CHECK_ACCOUNT_WIN_ID),
        WIN_TITLE(TXT_EMAIL_PLS_SELECT_ACCOUNT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_CHECK_ACCOUNT_LISTBOX_CTRL_ID),
        END_WIN
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
//add by eva just for test
// typedef struct TSEmailActInfo
// {
//    void *pawAct;
//    void *pawActName;
//    void *pawUserName;
//    void *pawEmail;
//    void *pawPop;
//    void *pawSmtp;
//    void *popPort;
//    void *smtpPort;
//    void *pawPwd;
//    BOOLEAN bSSL;
//    BOOLEAN bAut;
// }TSEmailActInfo;
// 
// static TSEmailActInfo g_ts_email_act_info[] =
// {
//    {L"163",      L"163",        L"UserName",   L"hangzhoutechsoft@163.com",         L"pop.163.com",    L"smtp.163.com",   L"110",   L"25",   L"123456",     FALSE, TRUE  },
//    {L"TechSoft", L"TechSoft",   L"UserName",   L"test1@techsoft.cn",                L"techsoft.cn",    L"techsoft.cn",    L"110",   L"25",   L"123456",     FALSE, TRUE  },
//    {L"Gmail",    L"Gmail",      L"UserName",   L"hangzhoutechsoft.test@gmail.com",  L"pop.gmail.com",  L"smtp.gmail.com", L"995",   L"465",  L"123456abc",  TRUE,  TRUE  },
//    {L"Yahoo",    L"Yahoo",      L"UserName",   L"hztechsoft@yahoo.cn",              L"pop.mail.yahoo.com.cn",     L"smtp.mail.yahoo.com.cn",    L"995",   L"465",   L"123456",  TRUE, TRUE},
//    {L"Hotmail",  L"Hotmail",    L"UserName",   L"techsofttest@hotmail.com",         L"pop3.live.com",     L"smtp.live.com",    L"995",   L"25",   L"123456",  TRUE, TRUE}
// };

// PUBLIC MMIEmail_CreateDefaultAccount(void)
// {
// 	HEMSStore *hStore = PNULL;
// 	int  iRet = 0;
// 	EMS_ACTID uiActId = 0;
// 	EMS_BOXID uiBoxId = 0;
// 	EMS_PRFITEM_VAL stVal[2] = {0};
// 
//    int  iIndex = 0;
//    int  iCnt = sizeof(g_ts_email_act_info) / sizeof(TSEmailActInfo);
// 	
// 	hStore = HEMSSTORE_New(PNULL, PNULL);
// 
//    while(iIndex < iCnt)
//    {
//       uiActId = 0;
//       uiBoxId = 0;
// 
//       iRet += HEMSSTORE_CreateAccount(hStore, g_ts_email_act_info[iIndex].pawAct, &uiActId);
//       iRet += HEMSSTORE_CreateBox(hStore, L"Inbox", &uiBoxId, uiActId, 0x01);
//       iRet += HEMSSTORE_CreateBox(hStore, L"Outbox", &uiBoxId, uiActId, 0x01);
//       iRet += HEMSSTORE_CreateBox(hStore, L"Sentbox", &uiBoxId, uiActId, 0x01);
//       iRet += HEMSSTORE_CreateBox(hStore, L"Draftbox", &uiBoxId, uiActId, 0x01);
//       
//       /*account base info*/
//       stVal[0].pData = g_ts_email_act_info[iIndex].pawActName;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_ACCOUNT_NAME, &stVal);
//       
//       stVal[0].pData = g_ts_email_act_info[iIndex].pawUserName;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_USER_NAME, &stVal);
//       
//       stVal[0].pData = g_ts_email_act_info[iIndex].pawEmail;
//       iRet = HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_ACCOUNT_EMAIL, &stVal);
//       
//       stVal[0].pData = g_ts_email_act_info[iIndex].pawEmail;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_REPLY_TO_EMAIL, &stVal);
//       
//       stVal[0].pData = L"500";
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_MAX_MSG_SIZE, &stVal);
//       
//       stVal[0].pData = L"10";
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_NUMBER_LIMIT, &stVal);
//       
//       stVal[0].pData = (void*)1;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_EMAIL_DEFAULTINBOXID, &stVal);
//       
//       stVal[0].pData = (void*)TRUE;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_RECEIVER_LEAVE_COPY, &stVal);
//       
//       stVal[0].pData = (void*)FALSE;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_DELETE_LEAVE_COPY, &stVal);
//       
//       stVal[0].pData = (void*)TRUE;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_SAVE_SENT, &stVal);
//       
//       stVal[0].pData = (void*)TRUE;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_INCLUDE_ORIGINAL, &stVal);
//       
//       stVal[0].pData = (void*)EMS_ADD_SIGNATURE_ALL_MAIL;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_EMAIL_ADD_SIGNATURE_MODE, &stVal);
//       
//       stVal[0].pData = (void*)EMS_DOWNLOAD_ASK_USER;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_EMAIL_DOWNLOAD_MODE, &stVal);
//       
//       stVal[0].pData = (void*)0x01;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_RECEIVE_SERVER_TYPE, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].pawEmail;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_EMAIL_USER_ID, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].pawPwd;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_EMAIL_PASSWORD, &stVal);
//       
//       /*smtp info*/
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].pawEmail;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_SMTP_USER_ID, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].pawPwd;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_SMTP_PASSWORD, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].pawSmtp;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_SMTP_SERVER, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].smtpPort;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_SMTP_SERVER_PORT, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].bSSL;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_SMTP_SSL_OR_TLS, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].bAut;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_SMTP_AUTHENTICATE, &stVal);
//       
//       /*pop info*/
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].pawPop;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_POP3_SERVER, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].popPort;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_POP3_PORT, &stVal);
//       
//       stVal[0].pData = (void*)g_ts_email_act_info[iIndex].bSSL;
//       iRet += HEMSSTORE_SetActCfgItem(hStore, uiActId, EMS_PRFITEM_POP3_SSL_OR_TLS, &stVal);
//       
//       if(iRet != 0)
//       {
//          HEMSSTORE_DeleteAccount(hStore, uiActId);
//       }
// 
//       iIndex++;
//    }
// 	
// 	HEMSSTORE_Delete(hStore);
// 	
// 	return;
// }

/*****************************************************************************/
// 	Description : handle email sig
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEmailSig(
                                  PWND     app_ptr,    //pointer of application
                                  uint16   msg_id,     //the identify of message
                                  DPARAM   param       //the param of the message
                                  )
{
    
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint32 param_buf = (uint32)param + sizeof(xSignalHeaderRec);
    
    switch(msg_id)
    {
    case EMAIL_SIG_TRANS_RESULT:
        {
            EMAIL_TRANSRESULT_T *trans_ret_ptr = (EMAIL_TRANSRESULT_T*)(*(uint32**)param_buf );
            EMA_UTILS_TRACE("HandleEmailSig receive EMAIL_SIG_TRANS_RESULT");
            EMA_UTILS_TRACE("the trans type is %d, the result is%d", trans_ret_ptr->trans_type, trans_ret_ptr->iresult);
            
            if (EMA_TRANS_SMTP_SEND_MAIL == trans_ret_ptr->trans_type)
            {
                MMK_SendMsg(EMAIL_SENDING_WIN_ID, EVT_EMA_SEND_RET, trans_ret_ptr);
            }
            switch(trans_ret_ptr->trans_type)
            {
            case EMA_TRANS_POP3_FETCH_NEW_MAILS_HEADER:
            case EMA_TRANS_POP3_FETCH_NEW_MAILS:
            case EMA_TRANS_POP3_FETCH_SPEC_MAIL:
            case EMA_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER:
            case EMA_TRANS_IMAP4_FETCH_NEW_MAILS:
            case EMA_TRANS_IMAP4_FETCH_SPEC_MAIL:
                MMK_SendMsg(EMAIL_RECVING_WIN_ID, EVT_EMA_RECEIVE_RET, trans_ret_ptr);
                break;
            default:
                break;
            }
            
            if (TRUE == g_email_pdp_active)
            {
                MMIEMAIL_PDPDeactive();
            }
#ifdef MMI_WIFI_SUPPORT
            if (TRUE == g_email_is_use_wifi)
            {
                MMIEMAIL_DeRegWifiNotify();
            }
#endif
            g_email_is_engine_task_busy = FALSE;
            EMA_UTILS_TRACE("mmiemail_export.c set g_email_is_engine_task_busy FALSE, EMAIL_SIG_TRANS_RESULT");
            if(!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
            {
                MMIEMAIL_ExitSetting();
            }
        }
        break;		
    case EMAIL_SIG_NEW_MESSAGE:
        {
            EMAIL_MSG_T *msg_ptr = (EMAIL_MSG_T*)(*(uint32**)param_buf);
            
            TF3RD_MEMFREE(msg_ptr->emailid_ptr);
            TF3RD_MEMFREE(msg_ptr->uidl_ptr);
            TF3RD_MEMFREE(msg_ptr);
        }
        break;
    case EMAIL_SIG_MAILBOX_GETED:
        break;
    case EMAIL_SIG_MESSAGE_SIZE_NOTIFY:
        {   
            EMAIL_MSG_SIZE_NOTIFY_T *size_notify_ptr = (EMAIL_MSG_SIZE_NOTIFY_T*)(*(uint32**)param_buf);
            EMA_UTILS_TRACE("HandleEmailSig receive EMAIL_SIG_MESSAGE_SIZE_NOTIFY");
            EMA_UTILS_TRACE("the total size is %d, the is curr size %d", size_notify_ptr->total_size, size_notify_ptr->trans_fered_size);
            
            if (MMK_IsOpenWin(EMAIL_SENDING_WIN_ID))
            {
                MMK_SendMsg(EMAIL_SENDING_WIN_ID, EVT_EMA_MESSAGE_SIZE_NOTIFY, param_buf);
            }
            else
            {
                MMK_SendMsg(EMAIL_RECVING_WIN_ID, EVT_EMA_MESSAGE_SIZE_NOTIFY, param_buf);
            }
        }
        break;
    case EMAIL_SIG_MESSAGE_NUMBER_NOTIFY:
        {   
            EMAIL_MSG_NUMBER_NOTIFY_T *num_notify_ptr = (EMAIL_MSG_NUMBER_NOTIFY_T*)(*(uint32**)param_buf);
            EMA_UTILS_TRACE("HandleEmailSig receive EMAIL_SIG_MESSAGE_NUMBER_NOTIFY");
            EMA_UTILS_TRACE("the total num is %d, the is curr num %d", num_notify_ptr->total_email, num_notify_ptr->curr_email);
            
            MMK_SendMsg(EMAIL_RECVING_WIN_ID, EVT_EMA_MESSAGE_NUMBER_NOTIFY, param_buf);
        }
        break;
    case EMAIL_SIG_INFO_NOTIFY:
        break;
    case EMAIL_SIG_REGEX_CALLBACK:
        break;
    case EMAIL_SIG_DELETEALL_RESULT:
        {
            StEMSDelAllNotify *del_notify_ptr = (StEMSDelAllNotify*)(*(uint32**)param_buf);
            TF3RD_MEMFREE(del_notify_ptr);
            MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_DELETEALL_RESULT, 0);
            g_email_is_engine_task_busy = FALSE;
            SCI_TRACE_LOW("mmiemail_export.c set g_email_is_engine_task_busy FALSE, EMAIL_SIG_DELETEALL_RESULT");
            if(!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
            {
                MMIEMAIL_ExitSetting();
            }
        }
        break;
    case EMAIL_SIG_FACTORY_RESET_RESULT:
        {
            MMK_SendMsg(EMAIL_SYSTEM_SETTING_WIN_ID, EVT_EMA_FACTORY_RESET, 0);
            g_email_is_engine_task_busy = FALSE;
            SCI_TRACE_LOW("mmiemail_export.c set g_email_is_engine_task_busy FALSE, EMAIL_SIG_FACTORY_RESET_RESULT");
            if(!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
            {
                MMIEMAIL_ExitSetting();
            }
        }
        break;
    case EMAIL_SIG_DELETE_ACCOUNT_RESULT:
        {
           MMK_SendMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_DEL_ACC_RET, 0);
           g_email_is_engine_task_busy = FALSE;
           SCI_TRACE_LOW("mmiemail_export.c set g_email_is_engine_task_busy FALSE, EMAIL_SIG_DELETE_ACCOUNT_RESULT");
           if(!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
           {
               MMIEMAIL_ExitSetting();
           }
        }
        break;
    case EMAIL_SIG_UPDATE_STATE_RESULT:
        {
            StEMSUpdateState *del_notify_ptr = (StEMSUpdateState*)(*(uint32**)param_buf);
            TF3RD_MEMFREE(del_notify_ptr);
            MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMS_UPDATE_STATE_RESULT, 0);
            g_email_is_engine_task_busy = FALSE;
            SCI_TRACE_LOW("mmiemail_export.c set g_email_is_engine_task_busy FALSE, EMAIL_SIG_UPDATE_STATE_RESULT");
            if(!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
            {
                MMIEMAIL_ExitSetting();
            }
        }
        break;
    case EMAIL_SIG_SAVE_EMAIL_RET:
        if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID) || g_save_email_on_red_key)
        {
            MMIEMAIL_HandleEditSaveAsDraftResult(*(uint32 *)param_buf);
        }
        break;
    case EMAIL_SIG_SEND_EMAIL_RET:
        if (MMK_IsOpenWin(EMAIL_SENDING_WIN_ID))
        {
            MMK_SendMsg(EMAIL_SENDING_WIN_ID, EVT_EMA_SEND_EMAIL_RET, param_buf);
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify SMS
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyEmailFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMIEMAIL]: HandleSIMPlugNotifyEmailFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEMAIL_EXPORT_341_112_2_18_2_16_13_0,(uint8*)"dd", dual_sys, notify_event);
    
    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        break;
        
    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        MMK_CloseWin(EMAIL_NETWORK_PATH_WIN_ID);     
        break;
        
    default:
        break;
    }
}
#endif
/*************************************************************************************/
// 	Description : email app init
//	Global resource dependence :
//  Author:
//	Note:
/*************************************************************************************/
PUBLIC void MMIAPIEMAIL_InitApp(void)
{
#ifdef SIM_PLUG_IN_SUPPORT
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T email_plug_notify_info = {0};
#endif
    g_mmiemail_app.ProcessMsg = HandleEmailSig;
    g_mmiemail_app.component_type = CT_APPLICATION;
	BRWENG20_CreateEmailTaskThread();
    MMIEMAIL_CreateEmailTaskThread();
	//MMIEmail_CreateDefaultAccount();
#ifdef SIM_PLUG_IN_SUPPORT
    email_plug_notify_info.module_id = MMI_MODULE_EMAIL;
    email_plug_notify_info.notify_func = HandleSIMPlugNotifyEmailFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&email_plug_notify_info);
#endif
}

/*****************************************************************************/
// Description : handle query window message
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryWinMsg(
                                     MMI_WIN_ID_T win_id,
                                     MMI_MESSAGE_ID_E msg_id,
                                     DPARAM param
                                     )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    //MMIATV_SERVICE_NODE_T *cur_node_ptr = PNULL;
    static BOOLEAN is_exist_email = TRUE;//for email memory leak
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_exist_email = TRUE;
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
//        MMIEMAIL_SelectEmailAcc();
        is_exist_email = FALSE;
        MMIEMAIL_EntryEmailTip();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (is_exist_email)
        {
            MMIEMAIL_ExitSetting();
        }
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}


/*****************************************************************************/
// 	Description: handle select email account window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCheckAccountWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_CHECK_ACCOUNT_LISTBOX_CTRL_ID;
   uint16         index = 0;
    EMASP_ACTINFO_T * act_info_ptr = PNULL;
    int32         result = 0;
    static BOOLEAN is_exist_email = TRUE;//for email memory leak
    
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
      {
         uint32 i= 0;
         
         GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
         GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
         uint32              num                 = 0;
         uint32          act_id = 0;
         
         item_data.softkey_id[0] = TXT_COMMON_OK;
         item_data.softkey_id[1] = TXT_NULL;
         item_data.softkey_id[2] = STXT_RETURN;
         item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
         item_t.item_data_ptr = &item_data;
         item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
         
         num = MMIEMAIL_VectorGetVectorNumber(s_email_act_vector);
         GUILIST_SetMaxItem(ctrl_id, num, FALSE);
        
         result = MMIEMAIL_SP_CFG_GetActiveAct(EMA_GETCFG_HANDLE(g_email_sp_handle), &act_id);
         EMA_UTILS_BREAKIF(result, "get MMIEMAIL_SP_CFG_GetActiveAct failed");

         for (i=0; i<num; i++)
         {
            MMIEMAIL_VectorGetAt(s_email_act_vector, i, &act_info_ptr);
            item_data.item_content[0].item_data.text_buffer.wstr_len = EMA_UTILS_WSTRLEN(act_info_ptr->act_name);
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = act_info_ptr->act_name;
            GUILIST_AppendItem(ctrl_id, &item_t);
//             if (act_id == act_info_ptr->act_id)
//             {
//                 GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
//             }
         }
         GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
         MMK_SetAtvCtrl(win_id, ctrl_id); 
         is_exist_email = TRUE;
      }
      break;
   case MSG_CTL_OK:
   case MSG_APP_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      is_exist_email = FALSE;
      index = GUILIST_GetCurItemIndex(ctrl_id);
      MMIEMAIL_VectorGetAt(s_email_act_vector, index, &act_info_ptr);
      MMIEMAIL_SP_CFG_SetActiveAct(EMA_GETCFG_HANDLE(g_email_sp_handle), act_info_ptr->act_id);
      MMIEMAIL_InitSetting(act_info_ptr->act_id);
      MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, g_email_opt_ptr);
      MMIEMAIL_FreeOptPtr();
      MMK_CloseWin(win_id); 
      break;
      
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
   case MSG_CLOSE_WINDOW:
       if (PNULL != s_email_act_vector)
       {
           MMIEMAIL_VectorDelete(s_email_act_vector);
           s_email_act_vector = PNULL;
       }
        if (is_exist_email)
        {
            MMIEMAIL_ExitSetting();
        }
       break;
   default:
      recode = MMI_RESULT_FALSE;
      break;
   }
   
   return recode;
}

/*****************************************************************************/
// 	Description: start setting
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
LOCAL int32 CheckAccount(void)
{
    int32 result = EMAIL_ERROR;
    int32 num = 0;

    do 
    {
        result = MMIEMAIL_VectorNew(&s_email_act_vector);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_VectorNew failed, vector_ptr is NULL");
        
        result = MMIEMAIL_SP_Store_GetActInfo(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),0,EMASP_ACTINFO_MAX_NUMBER,s_email_act_vector);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_ResetSettings--MMIEMAIL_SP_Store_GetActInfo failed");

        num = MMIEMAIL_VectorGetVectorNumber(s_email_act_vector);

        if (1 == num)
        {
            MMIEMAIL_InitSetting(1);
            result = MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)g_email_opt_ptr);
            MMIEMAIL_FreeOptPtr();
            MMIEMAIL_VectorDelete(s_email_act_vector);
            s_email_act_vector = PNULL;
        }
        else if (1 < num)
        {
            MMK_CreatePubListWin((uint32*)MMIEMAIL_CHECK_ACCOUNT_WIN_TAB, PNULL);
            result = EMAIL_SUCCESS;
        }
        else
        {
            EMA_UTILS_LOG_ERROR(("[MMIEMAIL]CheckAccount act_num = 0"));
            result = EMAIL_ERROR;
        }
    } while (0);

    return result;
}
/*************************************************************************************/
// 	Description : entry email edit
//	Global resource dependence :
//  Author:
//	Note:
/*************************************************************************************/
PUBLIC int32 MMIEMAIL_EditEmail(EMAOPT_T  *opt_ptr)
{
    int32 result = EMAIL_ERROR;
    uint32 i =0;
    MMI_WIN_ID_T  query_win_id = EMAIL_NOACC_QUERY_WIN_ID;
 
    if (!MMIEMAIL_IsDeviceReady(TRUE))
    {
        return EMAIL_ERROR;
    }

    //弹出等待窗口，提前促使释放窗口内存
    MMIPUB_OpenTempWaitWin();
    BLKMEM_GarbageClean();
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_EMAIL);
#endif

   MMIEmail_ThreadEnable();
   
    do 
    {
        if (!MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
        {            
            if(NULL == g_email_sp_handle)
            {
                result = MMIEMAIL_SP_InitHandle(&g_email_sp_handle);
                EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_InitHandle failed");
            }
            
            i = 0;
            while(EMA_OPT_END != opt_ptr[i].opt_id)
            {
                i++;
            }
            
            g_email_opt_ptr = EMA_UTILS_MALLOC(PNULL, (i + 1) *sizeof(EMAOPT_T));
            EMA_UTILS_BREAKNULL(g_email_opt_ptr, result, "MMIEMAIL_EditEmail, copy opt failed");
            SCI_MEMSET(g_email_opt_ptr, 0x00, (i + 1) *sizeof(EMAOPT_T));
            
            i = 0;
            while(EMA_OPT_END != opt_ptr[i].opt_id)
            {
                g_email_opt_ptr[i].opt_id = opt_ptr[i].opt_id;
                g_email_opt_ptr[i].val_ptr = EMA_UTILS_WSTRDUP(PNULL, opt_ptr[i].val_ptr);
                EMA_UTILS_BREAKNULL(g_email_opt_ptr[i].val_ptr, result, "MMIEMAIL_EditEmail, copy opt failed");
                i++;
            }
            g_email_opt_ptr[i].opt_id = EMA_OPT_END;
            //EMA_UTILS_BREAKIF(result, "MMIEMAIL_EditEmail, copy opt failed");

            if(E_EMAILA_NOT_ACCOUNT == MMIEMAIL_SP_CFG_ExsitAct(EMA_GETCFG_HANDLE(g_email_sp_handle)))
            {                
                MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_NOACCOUNT, IMAGE_PUBWIN_QUERY, &query_win_id, HandleQueryWinMsg);
            }
            else
            {
                result = CheckAccount();
                //result = MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)opt_ptr);
            }
        }
        else
        {
            result = MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)opt_ptr);
        }
    } while (0);
    
    if (EMAIL_SUCCESS != result)
    {
        MMIEMAIL_ExitSetting();
    }
    return result;	
}


/**************************************************************************************/
// 	Description : entry email module
//	Global resource dependence :
//  Author:
//	Note:
/**************************************************************************************/
PUBLIC void MMIAPIEMAIL_Entry(void)
{
    if (g_email_is_engine_task_busy)
    {
        MMIPUB_OpenAlertWarningWin(TXT_EMAIL_BUSY);
        return;
    }
	MMIEMAIL_SettingStart();  
//     just for test
//     EMAOPT_T  *pstOpts = PNULL;
//     
//     pstOpts  = EMA_UTILS_MALLOC(PNULL, 2 *sizeof(EMAOPT_T));
//     
//     pstOpts[0].opt_id = EMA_OPT_TO;
//     pstOpts[0].val_ptr = (void*)L"123";
//     
//     pstOpts[1].opt_id = EMA_OPT_END;
//     pstOpts[1].val_ptr = PNULL;
//     
//     MMIAPIEMAIL_EditEmail(pstOpts); 
}

/**************************************************************************************/
// 	Description : init email module
//	Global resource dependence :
//  Author: 
//	Note:
/**************************************************************************************/
PUBLIC void MMIAPIEMAIL_InitModule(void)
{
   MMIEMAIL_RegMenuGroup();
}


/*****************************************************************************/
// 	Description : judge email is running
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEMAIL_IsRunning(void)
{
    if (MMK_IsOpenWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID) || MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMEMAIL+++++++++email is running"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEMAIL_EXPORT_576_112_2_18_2_16_14_1,(uint8*)"");
        return TRUE;
    }
    return FALSE;
}


/*****************************************************************************/
// 	Description : exit form email
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_SyncEmail(void)
{
    EMA_UTILS_TRACE("----MMIAPIEMAIL_SyncEmail begin");
    MMK_CloseMiddleWin(EMAIL_WIN_ID_START, EMAIL_MAX_WIN_ID);
    MMIEMAIL_SP_Store_SyncMsgIndexInfo(EMA_GETSTORE_HANDLE(g_email_sp_handle));
    EMA_UTILS_TRACE("----MMIAPIEMAIL_SyncEmail end");
}


/*****************************************************************************/
// 	Description : email reset factory
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIEMAIL_ResetFactory(void)
{
    if (g_email_is_engine_task_busy)
    {
        return SUCCESS;
    }
    return MMIEMAIL_ResetSettings();
}
/*************************************************************************************/
// 	Description : entry email edit with mail addr
//	Global resource dependence :
//  Author:
//	Note:
/*************************************************************************************/
PUBLIC int32 MMIAPIEMAIL_EditEmail(const MMI_STRING_T   *email_addr_to)
{
    EMAOPT_T *pstOpts = PNULL;
    int32 result = EMAIL_ERROR;

    if (g_email_is_engine_task_busy)
    {
        MMIPUB_OpenAlertWarningWin(TXT_EMAIL_BUSY);
        return SUCCESS;
    }

    if (PNULL != email_addr_to && PNULL != email_addr_to->wstr_ptr &&  0 != email_addr_to->wstr_len)
    {
        if (email_addr_to->wstr_len < EMA_MAX_ADDR_LEN)
        {
            //SCI_TRACE_LOW:"[EMAIL]MMIAPIEMAIL_EditEmail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEMAIL_EXPORT_616_112_2_18_2_16_14_2,(uint8*)"");
            pstOpts  = SCI_ALLOCA(2 *sizeof(EMAOPT_T));
            if (PNULL != pstOpts)
            {
                pstOpts[0].opt_id = EMA_OPT_TO;
                pstOpts[0].val_ptr = (void*)email_addr_to->wstr_ptr;
                
                pstOpts[1].opt_id = EMA_OPT_END;
                pstOpts[1].val_ptr = PNULL;
                
                result = MMIEMAIL_EditEmail(pstOpts);  
                if(EMAIL_ERROR == result)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMM_INIT_FAIL);
                    return result;
                }
                SCI_FREE(pstOpts);
            }
            else
            {
                //SCI_TRACE_LOW:"[EMAIL]MMIAPIEMAIL_EditEmail, alloc memory fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEMAIL_EXPORT_631_112_2_18_2_16_14_3,(uint8*)"");
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NAME_TOO_LONG);
        }
    }
    else
    {
        result = MMIEMAIL_EditEmail(PNULL);
        if(EMAIL_ERROR == result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMM_INIT_FAIL);
            return result;
        }
    }
    return SUCCESS;
}

/*************************************************************************************/
// 	Description : entry email edit
//	Global resource dependence :
//  Author:
//	Note:
/*************************************************************************************/
PUBLIC int32 MMIAPIEMAIL_EditEmailEx(EMAOPT_T  *opt_ptr)
{
    if (g_email_is_engine_task_busy)
    {
        MMIPUB_OpenAlertWarningWin(TXT_EMAIL_BUSY);
        return SUCCESS;
    }
    else
    {
        return MMIEMAIL_EditEmail(opt_ptr);
    }

}
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
/*****************************************************************************/
//  Description : 设置pb的设置vcard
//  Global resource dependence : g_pb_edit_doc
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_SetSendVcardDocument(MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T *pb_edit_doc_ptr)
{
    s_pb_edit_doc.attach_list_ptr = pb_edit_doc_ptr->attach_list_ptr;
    s_pb_edit_doc.attach_num = pb_edit_doc_ptr->attach_num;
}
/*****************************************************************************/
//  Description : 获取pb的设置vcard
//  Global resource dependence : g_pb_edit_doc
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T MMIAPIEMAIL_GetSendVcardDocument(void)
{
    return s_pb_edit_doc ;
}
/*****************************************************************************/
//  Description : 设置pb的是否设置vcard
//  Global resource dependence : g_is_pb_send_edit
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_SetIsPbSendEdit(BOOLEAN is_pb_send_edit)
{
    s_is_pb_send_edit = is_pb_send_edit;
}
/*****************************************************************************/
//  Description : 获取pb的是否设置vcard
//  Global resource dependence : g_is_pb_send_edit
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEMAIL_GetIsPbSendEdit()
{
    return s_is_pb_send_edit;
}
/*****************************************************************************/
//  Description : 通过pb给email发送vcard
//  Global resource dependence : 
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_EditEMAILFromFile(MMIFILE_FILE_INFO_T *file_info_ptr)
{
    MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc ={0};
    MMIAPIEMAIL_SetIsPbSendEdit(TRUE);
    if (MMIEMAIL_EditAddAttachment(&pb_edit_doc, file_info_ptr, MMIFMM_FILE_TYPE_NORMAL))
    {
        MMIAPIEMAIL_SetSendVcardDocument(&pb_edit_doc);								                   
    }
    MMIAPIEMAIL_Entry();
}
#endif
/*Edit by script, ignore 1 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
