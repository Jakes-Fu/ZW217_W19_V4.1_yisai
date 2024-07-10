/****************************************************************************
** File Name:      mmiemail_net.c                                         *
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

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmiconnection_export.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "sysMemFile.h"
#include "mmi_signal_ext.h"
#include "mmicc_export.h"
#include "mmiphone_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_id.h"
#include "email_text.h"
#include "mmipub.h"
#include "mmiemail_net.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
BOOLEAN  g_email_is_use_wifi = FALSE;
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern uint32    g_email_is_send;
extern HEmlSPMgr  g_email_sp_handle;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC BOOLEAN g_email_pdp_active     = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void HandleEmailPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : active pdp for email 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_PDPActive(MN_DUAL_SYS_E dual_sys)
{
#ifndef WIN32
    BOOLEAN is_ative = FALSE;
    uint8 setting_index = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    uint32  cfg_item = 0;
    EMASP_PRFITEM_VAL val = {0};
    uint32  shift = 0;
    uint32  result =0;
    EMA_UTILS_TRACE("MMIEmail_PDPActive start!!!, the g_email_pdp_active is %d, the dual_sys is %d",
        g_email_pdp_active, dual_sys);

    if(TRUE == g_email_pdp_active)
    {
       EMA_UTILS_TRACE("MMIEmail_PDPActive, pdp is active, return");
       MMIEMAIL_PDPDeactive();
       return FALSE;
    }

    result = MMIEMAIL_SP_Store_GetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, EMASP_PRFITEM_USER1_PROFILE, &val);
    if(EMAIL_SUCCESS != result)
    {	
	EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetActCfgItem return failed!~~~~~~~~~~~~~"));
    }

    cfg_item = (uint32)val.pData; 
	EMA_UTILS_TRACE("MMIEmail_PDPActive the cfg_item index is =0x%08x, dual_sys=%d", cfg_item, dual_sys);
    shift = dual_sys * 8;
#ifndef WIN32
    setting_index = (uint8)(cfg_item >> shift);
#else
    setting_index = (uint8)(cfg_item >> shift);
#endif

    EMA_UTILS_TRACE("MMIEmail_PDPActive the profile index is =%d, dual_sys=%d", setting_index, dual_sys);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, setting_index);
    if(setting_item_ptr)
    {
        EMA_UTILS_TRACE("MMIEmail_PDPActive apn=%s",setting_item_ptr->apn);

        app_info.app_handler = MMI_MODULE_EMAIL;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.dual_sys = dual_sys;
        app_info.priority = setting_index;
        app_info.ps_service_rat = MN_UNSPECIFIED;
        app_info.handle_msg_callback = HandleEmailPDPMsg;
        MNGPRS_ReadStorageExtReqQosEx(dual_sys,MN_GPRS_STORAGE_ALL,MMS_E,&app_info.qos_info);

        EMA_UTILS_TRACE("MMIEmail_PDPActive dual_sys=%d",dual_sys);

        is_ative = MMIAPIPDP_Active(&app_info);
    }

    EMA_UTILS_TRACE("MMIEmail_PDPActive return %d!!!", is_ative);

    return is_ative;
#else
    if (g_email_is_send == EMA_TRANS_TYPE_SEND_EMAIL)
    {
       if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
       {
          MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_SEND, 0, 0);
       }
       else if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
       {
          MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RESEND, 0, 0);
       }
       else if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
       {
          MMK_PostMsg(EMAIL_BOX_WIN_ID, EVT_EMA_RESEND, 0, 0);
       }
    }
    else if (g_email_is_send == EMA_TRANS_TYPE_RETRIEVE_EMAIL)
    {
       if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
       {
          MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
       }
       else if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
       {
          MMK_PostMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
       }
       else if (MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
       {
          MMK_PostMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
       }
    }
    else if (g_email_is_send == EMA_TRANS_TYPE_SEND_RR)
    {
       MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_SENDREADREPORT, 0, 0);
    }
    return TRUE;
#endif
}

/*****************************************************************************/
//  Description : deactive pdp for email 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_PDPDeactive(void)
{
    BOOLEAN is_ative = FALSE;

    is_ative = MMIAPIPDP_Deactive(MMI_MODULE_EMAIL);

    return is_ative;
}

/*****************************************************************************/
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleEmailPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    EMA_UTILS_TRACE("HandleEmailPDPMsg msg_id=%d",msg_ptr->msg_id);
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
       if(msg_ptr->result == MMIPDP_RESULT_SUCC)
       {
          EMA_UTILS_TRACE("HandleEmailPDPMsg active!!!, the net id is %d", msg_ptr->nsapi);
          g_email_pdp_active = TRUE;
       }
       else
       {
          EMA_UTILS_TRACE("HandleEmailPDPMsg failed the result is = %d!!!", msg_ptr->result);

          if (g_email_is_send == EMA_TRANS_TYPE_SEND_EMAIL)
          {
             if (MMK_IsOpenWin(EMAIL_SENDING_WIN_ID))
             {
                MMK_CloseWin(EMAIL_SENDING_WIN_ID);
                if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
                {
                    MMK_CloseWin(EMAIL_EDIT_WIN_ID);
                }
                MMIPUB_OpenAlertFailWin( TXT_COMM_SEND_FAILED);
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
          else if (g_email_is_send == EMA_TRANS_TYPE_RETRIEVE_EMAIL)
          {
             if (MMK_IsOpenWin(EMAIL_RECVING_WIN_ID))
             {
                EMA_UTILS_LOG_ERROR(("HandleEmailPDPMsg MMIPDP_DEACTIVE_CNF"));
                MMK_CloseWin(EMAIL_RECVING_WIN_ID);
                MMIPUB_OpenAlertFailWin( TXT_COMMON_RECEIVE_FAIL);
             }
          }

          if (TRUE == g_email_pdp_active)
          {
             MMIEMAIL_PDPDeactive();
          }
          return;
       }

       if (g_email_is_send == EMA_TRANS_TYPE_SEND_EMAIL)
       {
          if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
          {
             EMA_UTILS_TRACE("MMEMAIL-----MMK_PostMsg EVT_EMA_INNER_OUTBOX_RESEND to edit win");
             MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_SEND, (DPARAM)(&msg_ptr->nsapi), sizeof(uint32));
          }
          else if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
          {
             EMA_UTILS_TRACE("MMEMAIL-----MMK_PostMsg EVT_EMA_INNER_OUTBOX_RESEND to view win");
             MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RESEND, (DPARAM)(&msg_ptr->nsapi), sizeof(uint32));
          }
          else if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
          {
             EMA_UTILS_TRACE("MMEMAIL-----MMK_PostMsg EVT_EMA_INNER_OUTBOX_RESEND to box win");
             MMK_PostMsg(EMAIL_BOX_WIN_ID, EVT_EMA_RESEND, (DPARAM)(&msg_ptr->nsapi), sizeof(uint32));
          }
       }
       else if (g_email_is_send == EMA_TRANS_TYPE_RETRIEVE_EMAIL)
       {
          EMA_UTILS_TRACE("MMEMAIL-----MMK_PostMsg EVT_EMA_INNER_INBOX_RECEIVE to box win");
          if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
          {
             MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RECEIVE, (DPARAM)(&msg_ptr->nsapi), sizeof(uint32));
          }
          else if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
          {
             MMK_PostMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_RECEIVE, (DPARAM)(&msg_ptr->nsapi), sizeof(uint32));
          }
          else if (MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
          {
             MMK_PostMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_RECEIVE, (DPARAM)(&msg_ptr->nsapi), sizeof(uint32));
          }
       }
       else if (g_email_is_send == EMA_TRANS_TYPE_SEND_RR)
       {
          EMA_UTILS_TRACE("MMEMAIL-----MMK_PostMsg EVT_EMA_INNER_INBOX_SENDREADREPORT to box win");
          MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_SENDREADREPORT, (DPARAM)&(msg_ptr->nsapi), sizeof(uint32));
       }
       
       break;
       
    case MMIPDP_DEACTIVE_CNF:
        g_email_pdp_active = FALSE;
        EMA_UTILS_TRACE("HandleEmailPDPMsg: MMIPDP_DEACTIVE_CNF");
        break;
        
    case MMIPDP_DEACTIVE_IND:
        g_email_pdp_active = FALSE;
        EMA_UTILS_TRACE("HandleEmailPDPMsg: MMIPDP_DEACTIVE_IND");
        break;
        
    default:
        break;
    }
}


