/****************************************************************************
** File Name:      mmiemail_dlg.c                                          *
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

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmipub.h"
#include "mmi_image.h"
#include "mmk_type.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "email_text.h"
#include "mmiemail_dlg.h"
#include "mmiemail_id.h"
#include "mmiemail_def.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_export.h"
#include "AEEEMEError.h"
#include "mmiemail_net.h"

#include "mmimms_id.h"
#include "mmimms_net.h"
#include "mmimms_internal.h"
#include "tf3rd_for_hz.h"
#include "mmiemail_edit.h"
#include "mmiemail_signal.h"

#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#include "mmiemail_setting.h"
#endif
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

#define  MMIEMAIL_SENDING_STR_MAX_LEN	32

#ifdef MAINLCD_SIZE_320X480
	#define  MMIEMAIL_QUERY_STRING_MAX_NUM_H  20
	#define  MMIEMAIL_QUERY_STRING_MAX_NUM_V  15
#elif defined MAINLCD_SIZE_240X320
    #define  MMIEMAIL_QUERY_STRING_MAX_NUM_H  12
	#define  MMIEMAIL_QUERY_STRING_MAX_NUM_V  10
#elif defined MAINLCD_SIZE_176X220
    #define  MMIEMAIL_QUERY_STRING_MAX_NUM_H  12
	#define  MMIEMAIL_QUERY_STRING_MAX_NUM_V  10
#else
	#define  MMIEMAIL_QUERY_STRING_MAX_NUM_H  12
	#define  MMIEMAIL_QUERY_STRING_MAX_NUM_V  10
#endif

#define  MMIEMAIL_ELLIPSE_STR_LEN       3

#define  MMIEMAIL_SIZE_FLOAT_A          0.100000
#define  MMIEMAIL_SIZE_FLOAT_B          0.000000
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
extern HEmlSPMgr  g_email_sp_handle;
extern BOOLEAN    g_email_is_use_wifi;
extern BOOLEAN    g_email_pdp_active;
extern uint32     g_email_is_send;

static  uint32    s_keep_dual_sys = 0;
static  BOOLEAN   s_is_saved = FALSE;


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
wchar   g_email_send_str[EMA_MAX_ADDR_LEN] = {0};
wchar   g_email_receive_str[EMA_MAX_ADDR_LEN] = {0};
/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/
typedef struct dlg_notify_tag
{
    WE_INT32  curr;
    WE_INT32  total;
    BOOLEAN   is_size;
}DLG_NOTIFY_T;


LOCAL  DLG_NOTIFY_T s_dlg_notify = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : display receive size
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL void DisplayRecvSize(uint32 now_size, uint32 totalsize, BOOLEAN isbody, MMI_WIN_ID_T win_id);

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
                                     );

/*****************************************************************************/
// Description : handle receiving window message
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceivingWin(
                                       MMIMMS_WINDOW_ID_E win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );

/*****************************************************************************/
//  Description :handle sending window message
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendingWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description: open query window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_OpenQueryWinByID(MMI_TEXT_ID_T  text_id, MMI_WIN_ID_T  win_id)
{	
	MMIPUB_OpenQueryWinByTextId(text_id, IMAGE_PUBWIN_QUERY, &win_id, HandleQueryWinMsg);
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
    
	switch(msg_id)
	{
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
	case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        MMK_CloseWin(win_id);
        if (EMAIL_SAVE_QUERY_WIN_ID == win_id)
        {
            MMK_SendMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_ASK_SAVEDIALOG, (DPARAM)TRUE);
        }
        else if (EMAIL_VIEW_DELETE_QUERY_WIN_ID == win_id)
        {
            MMK_SendMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_DELETE_EML, 0);
        }
        else if(EMAIL_EMAILBODY_ASKDOWN == win_id)
        {
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_SIM_SELECT, 0);
        }
        else if (EMAIL_NOSUBJECT_QUERY_WIN_ID == win_id)
        {
            MMK_SendMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_NOSUBJECT_SEND, 0);
        }
        else if (EMAIL_NOSUBJECT_QUERY_WIN_ID_EX == win_id)
        {
            MMK_SendMsg(EMAIL_DRAFTBOX_CHILD_WIN_ID, EVT_EMA_NOSUBJECT_SEND, (DPARAM)TRUE);
        }
        else if(EMAIL_CONFIRM_READREPORT == win_id)
        {
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_ASK_SENDREADREPORT, 0);
        }
        else if (EMAIL_DELETE_FILE_QUERY_WIN_ID == win_id)
        {
            MMK_SendMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_ASK_DELETEFILE, 0);
        }
        break;
		
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		if (EMAIL_SAVE_QUERY_WIN_ID == win_id)
		{
			MMK_SendMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_ASK_SAVEDIALOG, (DPARAM)FALSE);
		}
		break;
		
	default:
		result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
	}
	
	return result;
}

/*****************************************************************************/
// Description : process after email send successfully
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcSentSuccessfully(uint32 email_id)
{
    int result = EMAIL_ERROR;
    EMAIL_ACT_MSGPREFERENCE_T *email_pref_ptr = PNULL;
    EMAIL_MSGDATACACHE_T *date_chche_ptr = PNULL;
    BOOLEAN is_save_sent = FALSE;
    uint32 result1 =0;
    do 
    {
        email_pref_ptr = (EMAIL_ACT_MSGPREFERENCE_T *)EMA_UTILS_MALLOC(NULL, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
        EMA_UTILS_BREAKNULL(email_pref_ptr, result, "EmailAUIC_PublicProc_SentSuccessfully, email_pref_ptr is NULL.");
        EMA_UTILS_MEMSET((void*)email_pref_ptr, 0, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
        
        result = MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, email_pref_ptr);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_SentSuccessfully, MMIEMAIL_SP_CFG_GetEmailPreference failed.");
        
        is_save_sent = email_pref_ptr->is_save_sentemail;
        EMA_UTILS_TRACE("ProcSentSuccessfully, the is_save_sent is %d", is_save_sent);
        
        result = MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), email_pref_ptr);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_SentSuccessfully, MMIEMAIL_SP_CFG_DEL_EmailPreference failed.");
        EMA_UTILS_FREE(NULL, email_pref_ptr);
        
        date_chche_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(NULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(date_chche_ptr, result, "EmailAUIC_PublicProc_SentSuccessfully, date_chche_ptr is NULL.");
        EMA_UTILS_MEMSET(date_chche_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, date_chche_ptr);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_SentSuccessfully, MMIEMAIL_SP_Store_GetEmailData failed.");
        
        if (date_chche_ptr->attach_num > 0)
        {
            date_chche_ptr->mail_status = EMA_SENT_WITH_ATTACH;
        }
        else
        {
            date_chche_ptr->mail_status = EMA_SENT;
        }
        
        result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, date_chche_ptr);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_SentSuccessfully, MMIEMAIL_SP_Store_UpdateEmail failed.");
        
        MMIEMAIL_ReleaseEmailDataCache(NULL, date_chche_ptr);
        
        if (TRUE == is_save_sent)
        {
            EMA_UTILS_TRACE("MMIEMAIL_SP_Store_MoveEmailtoFolder, the mover msg id is %d", email_id);
            result = MMIEMAIL_SP_Store_MoveEmailtoFolder(EMA_GETSTORE_HANDLE(g_email_sp_handle),
                EMA_SP_ACTIVE_ACCOUNT,
                EMA_OUTBOX, 
                EMA_SP_ACTIVE_ACCOUNT,
                EMA_SENTBOX,
                email_id);
            EMA_UTILS_BREAKIF(result, "ProcSentSuccessfully, MMIEMAIL_SP_Store_MoveEmailtoFolder failed.");
        }
        else
        {
            result = MMIEMAIL_SP_Store_DeleteEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, email_id);
            EMA_UTILS_BREAKIF(result, "ProcSentSuccessfully, MMIEMAIL_SP_Store_DeleteEmail failed.");
        }
    } while(0);
	
	if (EMAIL_SUCCESS != result)
	{
            result1 = MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), email_pref_ptr);
	     if (EMAIL_SUCCESS != result1)
	     {
		  EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DEL_EmailPreference, error"));
	     }
		return FALSE;
	}
	
	return TRUE;
}
/*****************************************************************************/
//  Description :display recv and send size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
// LOCAL void MMIEmail_DisplaySendRecvSize(uint32 now_size, uint32 totalsize, BOOLEAN is_fresh, MMI_WIN_ID_T win_id)
// {
//     MMI_STRING_T receipients                                = {0};
//     MMI_STRING_T text_sending                             = {0};
// 
// 
//     if( !MMK_IsFocusWin(EMAIL_SENDING_WIN_ID))
//     {
//         return;
//     }
//     
//     receipients.wstr_ptr = g_email_send_str;
//     receipients.wstr_len = EMA_UTILS_WSTRLEN(g_email_send_str);
// 
//     MMI_GetLabelTextByLang(TXT_SENDING, &text_sending);
//         
//     MMIPUB_SetWaitWinText(is_fresh, 2, &receipients, &text_sending, PNULL, win_id);
// }

/*****************************************************************************/
//  Description :handle sending window message
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendingWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
#if 0//ndef MMI_PDA_SUPPORT
    MMIPUB_INFO_T *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
#endif
    //uint16 temp_size      = 0;    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (EMA_TRANS_TYPE_SEND_EMAIL == g_email_is_send && MMK_IsOpenWin(EMAIL_EDIT_WIN_ID)) 
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_NULL, FALSE);
//#ifdef MMI_PDA_SUPPORT
            MMK_PostMsg(win_id, EVT_EMA_WAITING_SENDMAIL, 0, 0); 
            s_is_saved = FALSE;
//#endif
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
        }
        break;
#if 0//ndef MMI_PDA_SUPPORT
    case MSG_TIMER: 
        if(PNULL != win_info_ptr && *(uint8 *)param == win_info_ptr->update_delay_timer_id) 
        {         
            if (EMA_TRANS_TYPE_SEND_EMAIL == g_email_is_send && MMK_IsOpenWin(EMAIL_EDIT_WIN_ID)) 
            { 
                MMK_PostMsg(win_id, EVT_EMA_WAITING_SENDMAIL, 0, 0); 
                s_is_saved = FALSE;
            } 
        } 
        break; 
#endif
    case MSG_FULL_PAINT:
        err_code = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_GET_FOCUS:
        //        if (g_email_cur_size > 0)
        //        {
        //            MMIEmail_DisplaySendRecvSize(g_email_cur_size, g_email_total_size, TRUE, win_id);
        //        }
        
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            BOOLEAN is_busy = FALSE;
            uint32  trans_state = 0;

            if (EMA_TRANS_TYPE_SEND_EMAIL == g_email_is_send && MMK_IsOpenWin(EMAIL_EDIT_WIN_ID)
                && FALSE == s_is_saved) 
            { 
                break;
            } 
            
            MMIEMAIL_SP_TRS_IsBusy(EMA_GETTRANS_HANDLE(g_email_sp_handle), &is_busy, &trans_state);
            EMA_UTILS_TRACE("HandleSendingWin, the  tran status is %d, %d", is_busy, trans_state);
            if (is_busy)
            {
                MMIEMAIL_SP_TRS_CancelCurrentTrans(EMA_GETTRANS_HANDLE(g_email_sp_handle));
            }
            else
            {
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
                MMK_CloseWin(win_id);
                
                if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
                {
                    MMK_CloseWin(EMAIL_EDIT_WIN_ID);
                }
                
                MMIPUB_OpenAlertWarningWin(TXT_USER_CANCEL);       
                
                if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
                {
                    MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, 0);
                }
                
                if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
                {
                    MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
                }
            }
        }
        break;
    case MSG_CLOSE_WINDOW:
        EMA_UTILS_TRACE("HandleSendingWin receive MSG_CLOSE_WINDOW");
        break;
    case EVT_EMA_WAITING_SENDMAIL:
        EMA_UTILS_TRACE("HandleSendingWin, receive EVT_EMA_WAITING_SENDMAIL");
        MMIEMAIL_EmailEventInterface(MMIEMAIL_EMAIL_APP, MMIEMAIL_EVT_SAVE_AND_SEND_EMAIL, PNULL, PNULL);
        break;
    case EVT_EMA_SEND_EMAIL_RET:
        {
            s_is_saved = TRUE;
            if (*((uint32*)param))
            {
                MMIEMAIL_HandleEditSendEmailResult(*((uint32*)param));
            }
            else
            {
                MMI_STRING_T receipient = {0};
                MMI_STRING_T fill_str = {0};
                MMI_STRING_T downloading_str = {0};
                
                EMA_UTILS_MEMSET(&downloading_str, 0, sizeof(downloading_str));
                MMI_GetLabelTextByLang(TXT_SENDING, &downloading_str);
                
                receipient.wstr_ptr = g_email_send_str;
                receipient.wstr_len = EMA_UTILS_WSTRLEN(g_email_send_str);
                
                fill_str.wstr_ptr = (wchar*)L" ";
                fill_str.wstr_len = EMA_UTILS_WSTRLEN((wchar*)L" ");
                
                MMIPUB_SetWaitWinText(TRUE, 3, &receipient, &downloading_str, &fill_str, win_id);
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
                if (EMA_TRANS_TYPE_SEND_EMAIL == g_email_is_send && MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
                {
                    if (TRUE == g_email_is_use_wifi)
                    {
                        EMA_UTILS_TRACE("HandleSendingWin, use wifi to send email");
                        MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_SEND, 0, 0);
                    }
                    else
                    {
                        MMIEMAIL_PDPActive(s_keep_dual_sys);
                    }
                }
            }
        }
        break;
    case EVT_EMA_MESSAGE_SIZE_NOTIFY:
        {
            EMAIL_MSG_SIZE_NOTIFY_T *size_notify_ptr = (EMAIL_MSG_SIZE_NOTIFY_T*)(*(uint32**)param);
            EMA_UTILS_TRACE("HandleSendingWin receive EVT_EMA_MESSAGE_SIZE_NOTIFY");
            EMA_UTILS_TRACE("the total size is %d, the is curr size %d", size_notify_ptr->total_size, size_notify_ptr->trans_fered_size);
            
            if (NULL != size_notify_ptr)
            {
                MMI_STRING_T receipient = {0};
                MMI_STRING_T fill_str = {0};
                MMI_STRING_T downloading_str = {0};
                uint8 display_now_size[MMIEMAIL_SENDING_STR_MAX_LEN]     = {0};
                
                EMA_UTILS_MEMSET(&downloading_str, 0, sizeof(downloading_str));
                MMI_GetLabelTextByLang(TXT_SENDING, &downloading_str);
                
                receipient.wstr_ptr = g_email_send_str;
                receipient.wstr_len = EMA_UTILS_WSTRLEN(g_email_send_str);
                
                if (size_notify_ptr->trans_fered_size * 100 / size_notify_ptr->total_size < 1)
                {
                    EMA_UTILS_SNPRINTF((void *)display_now_size,MMIEMAIL_SENDING_STR_MAX_LEN , "%d%%", 1);
                }
                else
                {
                    EMA_UTILS_SNPRINTF((void *)display_now_size,MMIEMAIL_SENDING_STR_MAX_LEN , "%d%%", size_notify_ptr->trans_fered_size * 100 / size_notify_ptr->total_size);
                }
                
                fill_str.wstr_ptr = EMA_UTILS_STRTOWSTR(PNULL, (char*)display_now_size);
                fill_str.wstr_len = EMA_UTILS_WSTRLEN(fill_str.wstr_ptr);
                
                MMIPUB_SetWaitWinText(TRUE, 3, &receipient, &downloading_str, &fill_str, win_id);
                //MMIEmail_DisplaySendRecvSize(size_notify_ptr->uiTransferedSize, size_notify_ptr->uiTotalSize, TRUE, win_id);
                EMA_UTILS_FREEIF(PNULL, fill_str.wstr_ptr);
                TF3RD_MEMFREE(size_notify_ptr);
            }
        }
        break;
    case EVT_EMA_SEND_RET:
       {
          EMAIL_TRANSRESULT_T *trans_ret_ptr = (EMAIL_TRANSRESULT_T *)param;
          uint32			email_msg_id = 0;

          SCI_TRACE_BUF("zhangqiang test sending result",trans_ret_ptr,sizeof(EMAIL_TRANSRESULT_T));
          EMA_UTILS_TRACE("trans_ret_ptr->iresult is %d",trans_ret_ptr->iresult);

          MMIEMAIL_SP_TRS_GetEmailidByTransId(EMA_GETTRANS_HANDLE(g_email_sp_handle), trans_ret_ptr->trans_id,(uint32*)&email_msg_id);
          MMIEMAIL_SP_TRS_DeleteTrans(EMA_GETTRANS_HANDLE(g_email_sp_handle), trans_ret_ptr->trans_id, email_msg_id);
          MMIDEFAULT_SetBackLight(TRUE);
          if (EMAIL_SUCCESS == trans_ret_ptr->iresult)
          {
             EMA_UTILS_TRACE("HandleSendingWin -- receive EVT_EMA_OP_SENTSUCCESSFULLY");
             ProcSentSuccessfully(email_msg_id);
             EMA_UTILS_TRACE("HandleSendingWin -- close waiting dialog");
             MMK_CloseWin(win_id);
             
                if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
                {
                    MMK_CloseWin(EMAIL_EDIT_WIN_ID);
                }
                else if(MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
                {
                    MMK_CloseWin(EMAIL_VIEW_WIN_ID);
                }
                EMA_UTILS_TRACE("HandleSendingWin -- pop send sussfull dialog");
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_COMM_SEND_SUCCEED);
#endif
            }
            else
            {
                MMK_CloseWin(win_id);
                if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
                {
                    MMK_CloseWin(EMAIL_EDIT_WIN_ID);
                }

                if (EME_USERCANCEL  == trans_ret_ptr->iresult)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_USER_CANCEL);   
                }
                if (EME_NOSTORAGE  == trans_ret_ptr->iresult)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOSPACE);   
                }
                else if (EME_SIZE_LIMIT_EXCEED == trans_ret_ptr->iresult)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
                }
                else if (EME_CONNECT_ERROR == trans_ret_ptr->iresult || EME_DNS_ERROR == trans_ret_ptr->iresult)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NET_ERR);
                }
                else if (EME_ACCOUNT_INVAL == trans_ret_ptr->iresult || EME_LOGIN_ERROR == trans_ret_ptr->iresult)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_INVALID_ACCOUNT);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin( TXT_COMM_SEND_FAILED);
                }   
            }
            
            if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
            {
                MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, 0);
            }
            
            if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
            {
                MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
            }
            
            TF3RD_MEMFREE(trans_ret_ptr);
        }
        break;
    default :
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
// 	Description: create sending dialog window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_CreateSendingWin(MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T receipient = {0};
    MMI_STRING_T fill_str = {0};
    MMI_STRING_T downloading_str = {0};
    MMI_STRING_T wait_str = {0};

    EMA_UTILS_MEMSET(&downloading_str, 0, sizeof(downloading_str));
    MMI_GetLabelTextByLang(TXT_SENDING, &downloading_str);
    
    receipient.wstr_ptr = g_email_send_str;
    receipient.wstr_len = EMA_UTILS_WSTRLEN(g_email_send_str);
    
    fill_str.wstr_ptr = (wchar*)L" ";
    fill_str.wstr_len = EMA_UTILS_WSTRLEN((wchar*)L" ");
    
    if (EMA_TRANS_TYPE_SEND_EMAIL == g_email_is_send && MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
    {
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
        MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,EMAIL_SENDING_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)HandleSendingWin);
    }
    else
    {
        MMIPUB_OpenWaitWin(3, &receipient, &downloading_str, &fill_str, EMAIL_SENDING_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)HandleSendingWin);
    }

    if (!g_email_is_use_wifi && 
        !(EMA_TRANS_TYPE_SEND_EMAIL == g_email_is_send && MMK_IsOpenWin(EMAIL_EDIT_WIN_ID)))
    {
        EMA_UTILS_TRACE("MMIEmail_CreateSendingWin, call MMIEmail_PDPActive");
        MMIEMAIL_PDPActive(dual_sys);
    }
    s_keep_dual_sys = dual_sys;
    return;
}

/*****************************************************************************/
// 	Description: create receiving dialog window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_CreateRecvingWaitingWin(MN_DUAL_SYS_E dual_sys)
{

   MMI_STRING_T accout_name = {0};
   MMI_STRING_T tmp_prompt ={0};
   MMI_STRING_T tmp_download ={0};
   MMI_STRING_T fill_str = {0};
   MMI_STRING_T temp_downloading_str = {0};
   MMI_STRING_T downloading_str = {0};
   uint8 ellipse_str[MMIEMAIL_ELLIPSE_STR_LEN] = { '.', '.', '.'};
   uint8 email_to_string_max_num = 0;
   wchar str_account_name[ MMIEMAIL_QUERY_STRING_MAX_NUM_H + MMIEMAIL_ELLIPSE_STR_LEN * 2+1] = {0}; 
   wchar str_download[ MMIEMAIL_QUERY_STRING_MAX_NUM_H + MMIEMAIL_ELLIPSE_STR_LEN * 2+1] = {0}; 
   if(MMITHEME_IsMainScreenLandscape())
   {
       email_to_string_max_num = MMIEMAIL_QUERY_STRING_MAX_NUM_H;
   }
   else
   {
       email_to_string_max_num = MMIEMAIL_QUERY_STRING_MAX_NUM_V;
   }
   //g_email_send_record_id = record_id;
   EMA_UTILS_MEMSET(&temp_downloading_str, 0, sizeof(temp_downloading_str));
   MMI_GetLabelTextByLang(TXT_EMAIL_RETRIEVINGHEADBODY, &temp_downloading_str);
   //make downloading_str display correctly! 
   tmp_download.wstr_ptr = temp_downloading_str.wstr_ptr;
   tmp_download.wstr_len = temp_downloading_str.wstr_len;
   if (tmp_download.wstr_len <= email_to_string_max_num )
   {
       downloading_str.wstr_ptr = tmp_download.wstr_ptr;
       downloading_str.wstr_len = tmp_download.wstr_len;
   }
   else
   { 
       MMI_WSTRNCPY( str_download,
           email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN * 2,
           tmp_download.wstr_ptr,
           email_to_string_max_num,
           email_to_string_max_num);
       
       MMI_STRNTOWSTR( str_download + email_to_string_max_num,
           MMIEMAIL_ELLIPSE_STR_LEN,
           (const uint8 *)ellipse_str,
           MMIEMAIL_ELLIPSE_STR_LEN,
           MMIEMAIL_ELLIPSE_STR_LEN);
       downloading_str.wstr_ptr = str_download;
       downloading_str.wstr_len = (uint16)( email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN );
    }

   //make g_email_receive_str display correctly! 
   tmp_prompt.wstr_ptr = g_email_receive_str;
   tmp_prompt.wstr_len = EMA_UTILS_WSTRLEN(g_email_receive_str);
   if (tmp_prompt.wstr_len <= email_to_string_max_num )
   {
       accout_name.wstr_ptr = tmp_prompt.wstr_ptr;
       accout_name.wstr_len = tmp_prompt.wstr_len;
   }
   else
   { 
       MMI_WSTRNCPY( str_account_name,
                     email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN * 2,
                     tmp_prompt.wstr_ptr,
                     email_to_string_max_num,
                     email_to_string_max_num);

       MMI_STRNTOWSTR( str_account_name + email_to_string_max_num,
                      MMIEMAIL_ELLIPSE_STR_LEN,
                      (const uint8 *)ellipse_str,
                      MMIEMAIL_ELLIPSE_STR_LEN,
                      MMIEMAIL_ELLIPSE_STR_LEN);
       accout_name.wstr_ptr = str_account_name;
       accout_name.wstr_len = (uint16)( email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN );
    }
//display fill_str
   fill_str.wstr_ptr = (wchar*)L" ";
   fill_str.wstr_len = EMA_UTILS_WSTRLEN((wchar*)L" ");

   MMIPUB_OpenWaitWin(3, &downloading_str, &accout_name, &fill_str,EMAIL_RECVING_WIN_ID,IMAGE_NULL,
      ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)HandleReceivingWin);

   if (!g_email_is_use_wifi)
   {
      EMA_UTILS_TRACE("MMIEmail_CreateRecvingWaitingWin, call MMIEmail_PDPActive");
      MMIEMAIL_PDPActive(dual_sys);
   }
   return TRUE;
}

/*****************************************************************************/
// Description : handle receiving window message
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceivingWin(
                                       MMIMMS_WINDOW_ID_E win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
   MMI_RESULT_E err_code                    = MMI_RESULT_TRUE;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      //MMIEmaila_DisplaySendRecvSize(60, 200, FALSE, win_id);
      GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
      break;   
   case MSG_GET_FOCUS:
       if (s_dlg_notify.curr > 0)
       {
           DisplayRecvSize(s_dlg_notify.curr, s_dlg_notify.total, s_dlg_notify.is_size, win_id);
           EMA_UTILS_MEMSET(&s_dlg_notify, 0x00, sizeof(DLG_NOTIFY_T));
       }
       break;
   case MSG_APP_RED:
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
      {
         BOOLEAN is_busy = FALSE;
         uint32  trans_state = 0;
         
         MMIEMAIL_SP_TRS_IsBusy(EMA_GETTRANS_HANDLE(g_email_sp_handle), &is_busy, &trans_state);
         EMA_UTILS_TRACE("HandleSendingWin, the  tran status is %d, %d", is_busy, trans_state);
         if (is_busy)
         {
            MMIEMAIL_SP_TRS_CancelCurrentTrans(EMA_GETTRANS_HANDLE(g_email_sp_handle));
         }
         else
         {
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
             MMK_CloseWin(win_id);
             MMIPUB_OpenAlertWarningWin(TXT_USER_CANCEL);
         }
      }
      break;
   case MSG_CLOSE_WINDOW:
      EMA_UTILS_MEMSET(&s_dlg_notify, 0x00, sizeof(DLG_NOTIFY_T));
      break;
   case EVT_EMA_MESSAGE_SIZE_NOTIFY:
      {
         EMAIL_MSG_SIZE_NOTIFY_T *size_notify_ptr = (EMAIL_MSG_SIZE_NOTIFY_T*)(*(uint32**)param);
         EMA_UTILS_TRACE("HandleReceivingWin receive EVT_EMA_MESSAGE_SIZE_NOTIFY");
         EMA_UTILS_TRACE("the total size is %d, the is curr size %d", size_notify_ptr->total_size, size_notify_ptr->trans_fered_size);
         
         if (NULL != size_notify_ptr)
         {
            s_dlg_notify.curr = size_notify_ptr->trans_fered_size;
            s_dlg_notify.total = size_notify_ptr->total_size;
            s_dlg_notify.is_size = TRUE;
            DisplayRecvSize(size_notify_ptr->trans_fered_size, size_notify_ptr->total_size, TRUE, win_id);
            TF3RD_MEMFREE(size_notify_ptr);
         }
      }
      break;
   case EVT_EMA_MESSAGE_NUMBER_NOTIFY:
      {
         EMAIL_MSG_NUMBER_NOTIFY_T *num_notify_ptr = (EMAIL_MSG_NUMBER_NOTIFY_T*)(*(uint32**)param);
         EMA_UTILS_TRACE("HandleReceivingWin receive EMAIL_SIG_MESSAGE_NUMBER_NOTIFY");
         EMA_UTILS_TRACE("the total size is %d, the is curr size %d", num_notify_ptr->total_email, num_notify_ptr->curr_email);
         
         if (NULL != num_notify_ptr)
         {
            s_dlg_notify.curr = num_notify_ptr->curr_email;
            s_dlg_notify.total = num_notify_ptr->total_email;
            s_dlg_notify.is_size = FALSE;
            DisplayRecvSize(num_notify_ptr->curr_email, num_notify_ptr->total_email, FALSE, win_id);
            TF3RD_MEMFREE(num_notify_ptr);
         }
      }
      break;
   case EVT_EMA_RECEIVE_RET:
	{
         EMAIL_TRANSRESULT_T *trans_ret_ptr = (EMAIL_TRANSRESULT_T *)param;
         uint32			email_msg_id = NULL;

         MMIEMAIL_SP_TRS_GetEmailidByTransId(EMA_GETTRANS_HANDLE(g_email_sp_handle), trans_ret_ptr->trans_id,(uint32*)&email_msg_id);
         MMIEMAIL_SP_TRS_DeleteTrans(EMA_GETTRANS_HANDLE(g_email_sp_handle), trans_ret_ptr->trans_id, email_msg_id);
         MMIDEFAULT_SetBackLight(TRUE);

         EMA_UTILS_LOG_INFO(("HandleReceivingWin trans_ret_ptr->iresult = %d", trans_ret_ptr->iresult));
         if (EME_SUCCESS == trans_ret_ptr->iresult)
         {
            MMK_CloseWin(win_id);
            if (EMA_TRANS_POP3_FETCH_SPEC_MAIL == trans_ret_ptr->trans_type || EMA_TRANS_IMAP4_FETCH_SPEC_MAIL == trans_ret_ptr->trans_type)
            {
               MMK_SendMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_VIEW_UPDATE, (DPARAM)email_msg_id);
            }
            else
            {
               MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, 0);
            }
         }
         else
         {
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, 0);
            MMK_CloseWin(win_id);
            if (EME_USERCANCEL  == trans_ret_ptr->iresult)
            {
                MMIPUB_OpenAlertWarningWin(TXT_USER_CANCEL);   
            }
            if (EME_NOSTORAGE  == trans_ret_ptr->iresult)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOSPACE);   
            }
            else if (EME_NO_NEWMAIL == trans_ret_ptr->iresult)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NO_NEWMAIL);
            }
            else if (EME_SIZE_LIMIT_EXCEED == trans_ret_ptr->iresult)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
            }
            else if (EME_CONNECT_ERROR == trans_ret_ptr->iresult || EME_DNS_ERROR == trans_ret_ptr->iresult)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NET_ERR);
            }
            else if (EME_ACCOUNT_INVAL == trans_ret_ptr->iresult || EME_LOGIN_ERROR == trans_ret_ptr->iresult)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_INVALID_ACCOUNT);
            }
            else
            {
                MMIPUB_OpenAlertFailWin( TXT_COMMON_RECEIVE_FAIL);
            }
         }
         TF3RD_MEMFREE(trans_ret_ptr);
	}      
      break;
   default :
      err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
      break;
   }    
   
   return err_code;
}


/*****************************************************************************/
// Description : display receive size
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL void DisplayRecvSize(uint32 now_size, uint32 totalsize, BOOLEAN isbody, MMI_WIN_ID_T win_id)
{
    wchar display_now_title[MMIEMAIL_SENDING_STR_MAX_LEN]    = {0};
    uint8 display_now_size[MMIEMAIL_SENDING_STR_MAX_LEN]     = {0};
    //wchar display_total_title[MMIEMAIL_SENDING_STR_MAX_LEN]  = {0};
    //uint8 display_total_size[MMIEMAIL_SENDING_STR_MAX_LEN]   = {0};
    wchar now_title[MMIEMAIL_SENDING_STR_MAX_LEN]            = {0};
    //wchar total_title[MMIEMAIL_SENDING_STR_MAX_LEN]          = {0};
    MMI_STRING_T now_str                                  = {0};
    //MMI_STRING_T total_str                                = {0};
    MMI_STRING_T text_now_str                             = {0};
    MMI_STRING_T text_total_str                           = {0};
    MMI_STRING_T text_retry_str                           = {0};
    //uint32         len                                   = 0;
    MMI_STRING_T accout_name = {0};
    uint8 ellipse_str[MMIEMAIL_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    uint8 email_to_string_max_num = 0;
    wchar str[ MMIEMAIL_QUERY_STRING_MAX_NUM_H + MMIEMAIL_ELLIPSE_STR_LEN * 2+1] = {0}; 
    wchar str_download[ MMIEMAIL_QUERY_STRING_MAX_NUM_H + MMIEMAIL_ELLIPSE_STR_LEN * 2+1] = {0}; 
    MMI_STRING_T temp_downloading_str ={0};
    MMI_STRING_T tmp_download ={0};
    MMI_STRING_T downloading_str ={0};
        if(MMITHEME_IsMainScreenLandscape())
        {
            email_to_string_max_num = MMIEMAIL_QUERY_STRING_MAX_NUM_H;
        }
        else
        {
            email_to_string_max_num = MMIEMAIL_QUERY_STRING_MAX_NUM_V;
        }
        
        EMA_UTILS_TRACE("entry MMIEmaila_DisplaySendRecvSize");
        if( !MMK_IsFocusWin(EMAIL_RECVING_WIN_ID))
        {
            return;
        }
        
        if (0 != now_size)
        {
            EMA_UTILS_TRACE("FNH now_size = %ld, total_size = %ld",now_size, totalsize);
            //         if (isbody)
            //         {
            //            _snprintf((void *)display_total_size,MMIEMAIL_SENDING_STR_MAX_LEN ,":%.1fK",(float)totalsize/ONE_K_LEN); 
            //            if (((float)totalsize/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)totalsize/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
            //            {
            //               _snprintf((void*)display_total_size, MMIEMAIL_SENDING_STR_MAX_LEN,":%.1fk\r",0.1);
            //            }
            //            MMI_GetLabelTextByLang(TXT_EMAIL_TOTAL_SIZE, &total_str);
            //         }
            //         else
            //         {
            //             _snprintf((void *)display_total_size,MMIEMAIL_SENDING_STR_MAX_LEN ,":%d", totalsize); 
            //             MMI_GetLabelTextByLang(TXT_EMAIL_TOTAL_NUM, &total_str);
            //         }  
            //       
            //         MMI_STRNTOWSTR(display_total_title, MMIEMAIL_SENDING_STR_MAX_LEN, 
            //             (uint8 *)display_total_size, MMIEMAIL_SENDING_STR_MAX_LEN, strlen((const char *)display_total_size));
            //         
            //         MMI_WSTRNCPY(total_title, MMIEMAIL_SENDING_STR_MAX_LEN, total_str.wstr_ptr, total_str.wstr_len, total_str.wstr_len);
            //         MMI_WSTRNCPY(total_title + total_str.wstr_len, MMIEMAIL_SENDING_STR_MAX_LEN, 
            //             display_total_title, MMIEMAIL_SENDING_STR_MAX_LEN, strlen((const char *)display_total_size));
            //         text_total_str.wstr_ptr = total_title;
            //         text_total_str.wstr_len = total_str.wstr_len + strlen((const char *)display_total_size);
            //         
            //         if (0 == now_size)
            //         {
            //             now_size = 1;
            //         }
            
//g_email_send_record_id = record_id;
            EMA_UTILS_MEMSET(&temp_downloading_str, 0, sizeof(temp_downloading_str));
            MMI_GetLabelTextByLang(TXT_EMAIL_RETRIEVINGHEADBODY, &temp_downloading_str);
//make downloading_str display correctly! 
            tmp_download.wstr_ptr = temp_downloading_str.wstr_ptr;
            tmp_download.wstr_len = temp_downloading_str.wstr_len;
            if (tmp_download.wstr_len <= email_to_string_max_num )
            {
                downloading_str.wstr_ptr = tmp_download.wstr_ptr;
                downloading_str.wstr_len = tmp_download.wstr_len;
            }
            else
            { 
                MMI_WSTRNCPY( str_download,
                    email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN * 2,
                    tmp_download.wstr_ptr,
                    email_to_string_max_num,
                    email_to_string_max_num);
                
                MMI_STRNTOWSTR( str_download + email_to_string_max_num,
                    MMIEMAIL_ELLIPSE_STR_LEN,
                    (const uint8 *)ellipse_str,
                    MMIEMAIL_ELLIPSE_STR_LEN,
                    MMIEMAIL_ELLIPSE_STR_LEN);
                downloading_str.wstr_ptr = str_download;
                downloading_str.wstr_len = (uint16)( email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN );
            }
//  make g_email_receive_str display correctly!            
            text_total_str.wstr_ptr = g_email_receive_str;
            text_total_str.wstr_len = EMA_UTILS_WSTRLEN(g_email_receive_str);
            if (text_total_str.wstr_len <= email_to_string_max_num )
            {
                accout_name.wstr_ptr = text_total_str.wstr_ptr;
                accout_name.wstr_len = text_total_str.wstr_len;
            }
            else
            { 
                MMI_WSTRNCPY( str,
                    email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN * 2,
                    text_total_str.wstr_ptr,
                    email_to_string_max_num,
                    email_to_string_max_num);
                
                MMI_STRNTOWSTR( str + email_to_string_max_num,
                    MMIEMAIL_ELLIPSE_STR_LEN,
                    (const uint8 *)ellipse_str,
                    MMIEMAIL_ELLIPSE_STR_LEN,
                    MMIEMAIL_ELLIPSE_STR_LEN);
                accout_name.wstr_ptr = str;
                accout_name.wstr_len = (uint16)( email_to_string_max_num + MMIEMAIL_ELLIPSE_STR_LEN );
            }
            if (isbody)
            {
#if 0
                if (((float)now_size/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)now_size/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
                {
                    len = EMA_UTILS_SNPRINTF((void*)display_now_size, MMIEMAIL_SENDING_STR_MAX_LEN,"%.1fk", 0.1);
                }
                else
                {
                    len = EMA_UTILS_SNPRINTF((void *)display_now_size, MMIEMAIL_SENDING_STR_MAX_LEN ,"%.1fK",(float)now_size/ONE_K_LEN); 
                }
                
                if (((float)totalsize/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)totalsize/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
                {
                    EMA_UTILS_SNPRINTF((void*)(display_now_size + len), MMIEMAIL_SENDING_STR_MAX_LEN, "/%.1fk", 0.1);
                }
                else
                {
                    EMA_UTILS_SNPRINTF((void *)(display_now_size + len),MMIEMAIL_SENDING_STR_MAX_LEN , "/%.1fK",(float)totalsize/ONE_K_LEN); 
                }
#endif
                
                if (now_size * 100 / totalsize < 1)
                {
                    EMA_UTILS_SNPRINTF((void *)display_now_size,MMIEMAIL_SENDING_STR_MAX_LEN , "%d%%", 1);
                }
                else
                {
                    EMA_UTILS_SNPRINTF((void *)display_now_size,MMIEMAIL_SENDING_STR_MAX_LEN , "%d%%", now_size * 100 / totalsize);
                }
            }
            else
            {
                EMA_UTILS_SNPRINTF((void *)display_now_size,MMIEMAIL_SENDING_STR_MAX_LEN ,":%d", now_size); 
            }
            
            MMI_STRNTOWSTR(display_now_title, MMIEMAIL_SENDING_STR_MAX_LEN, 
                (uint8 *)display_now_size, MMIEMAIL_SENDING_STR_MAX_LEN, EMA_UTILS_STRLEN((const char *)display_now_size));
            
            if (!isbody)
            {
                MMI_GetLabelTextByLang(TXT_EMAIL_RETRIEVED, &now_str);
                MMI_WSTRNCPY(now_title, MMIEMAIL_SENDING_STR_MAX_LEN, now_str.wstr_ptr, now_str.wstr_len, now_str.wstr_len);
            }
            
            MMI_WSTRNCPY(now_title + now_str.wstr_len, MMIEMAIL_SENDING_STR_MAX_LEN, 
                display_now_title, MMIEMAIL_SENDING_STR_MAX_LEN, EMA_UTILS_STRLEN((const char *)display_now_size));
            
            text_now_str.wstr_ptr = now_title;
            text_now_str.wstr_len = now_str.wstr_len + EMA_UTILS_STRLEN((const char *)display_now_size);        
            
            MMIPUB_SetWaitWinText(TRUE, 3, &downloading_str, &accout_name, &text_now_str, win_id);
    }
}



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
