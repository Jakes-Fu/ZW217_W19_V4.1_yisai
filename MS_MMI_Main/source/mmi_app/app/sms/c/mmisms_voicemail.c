/****************************************************************************
** File Name:      mmisms_voicemail.c                                            *
** Author:                                                                  *
** Date:           9/28/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe voice mail.                *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 09/28/2007     liming.deng      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_voicemail.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmipub.h"
#include "mmi_id.h"
#include "window_parse.h"
//#ifdef MMI_GUI_STYLE_TYPICAL
#include "mmisms_menutable.h"
//#endif
#include "mmiidle_export.h"
#include "guilabel.h"
#include "mmiidle_statusbar.h"
#include "mmicc_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISMS_VOICEMAIL_LINE1  1
#define MMISMS_VOICEMAIL_LINE2  2
#define MMISMS_VOICEMAIL_FAX     3
#define MMISMS_VOICEMAIL_DATA   4
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL  MN_DUAL_SYS_E  s_dual_sys = MN_DUAL_SYS_1;
LOCAL  MMISMS_VMN_RECORD_ID_E s_vmn_type = MMISMS_LINE1_VMN_RECORD_ID;
LOCAL  MN_SMS_VMW_IND_FLAG_T s_vmw_flag_type[MMI_DUAL_SYS_MAX] = {0};
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
MMI_SMS_VMN_T g_cur_vmnum_info; //当前voice mail号码信息
extern MMISMS_VMN_OPER_TYPE_E g_vmn_oper_type;//查询语音信箱号是否为了通话转移   
extern BOOLEAN g_is_sms_vm_header;
//extern  MMISMS_SET_VM_ADDR_STATUS_T g_set_voimess_info ; //向sim卡存储语音信箱

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to get whether new vm being
//    Global resource dependence : none
//  Author:samboo.shen
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNewVMBeing(void);


/*****************************************************************************/
//     Description : to get whether new vm being
//    Global resource dependence : none
//  Author:samboo.shen
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNewVMBeingEx(MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
//     Description : 
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetVMAddr( 
                             MN_DUAL_SYS_E dual_sys,
                             MMISMS_VMN_RECORD_ID_E vmn_type
                             )
{
    s_dual_sys = dual_sys;
    s_vmn_type = vmn_type;
}

/*****************************************************************************/
//     Description : to handle the singal APP_MN_SET_VOIMESS_ADDR_CNF
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSetVoiMessAddrCnf( 
                                           DPARAM param
                                           )
{
    APP_MN_SET_VMW_ADDR_CNF_T set_vm_addr_info = *(APP_MN_SET_VMW_ADDR_CNF_T *)param;
    PHONEBOOK_ENTRY_T pb_entry = {0};
    MN_RETURN_RESULT_E  update_result = MN_RETURN_FAILURE;
    ERR_MNDATAMAG_CODE_E result = ERR_MNDATAMAG_NO_ERR;
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 mail_len = 0;
    MMI_STRING_T num_ptr = {0};
    wchar voice_mail_wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    
    //对消息解析，得出存号码是否成功 
    if ( MN_SMS_OPERATE_SUCCESS !=set_vm_addr_info.cause)
    {   
        switch( g_cur_vmnum_info.record_id ) 
        {
        case MMISMS_VOICEMAIL_LINE1:
            pb_entry.number_len = g_cur_vmnum_info.line1_vmn_info.num_len;
            pb_entry.ton        = g_cur_vmnum_info.line1_vmn_info.number_type; 
            SCI_MEMSET( pb_entry.number, 0xFF, sizeof(pb_entry.number) );
            SCI_MEMCPY( pb_entry.number, g_cur_vmnum_info.line1_vmn_info.bcd_num, pb_entry.number_len );
            break;
            
        case MMISMS_VOICEMAIL_LINE2:
            pb_entry.number_len = g_cur_vmnum_info.line2_vmn_info.num_len;
            pb_entry.ton        = g_cur_vmnum_info.line2_vmn_info.number_type; 
            SCI_MEMSET( pb_entry.number, 0xFF, sizeof(pb_entry.number) );
            SCI_MEMCPY( pb_entry.number, g_cur_vmnum_info.line2_vmn_info.bcd_num, pb_entry.number_len );
            break;
            
        case MMISMS_VOICEMAIL_FAX:
            pb_entry.number_len = g_cur_vmnum_info.fax_vmn_info.num_len;
            pb_entry.ton        = g_cur_vmnum_info.fax_vmn_info.number_type; 
            SCI_MEMSET( pb_entry.number, 0xFF, sizeof(pb_entry.number) );
            SCI_MEMCPY( pb_entry.number, g_cur_vmnum_info.fax_vmn_info.bcd_num, pb_entry.number_len );
            break;
            
        case MMISMS_VOICEMAIL_DATA:
            pb_entry.number_len = g_cur_vmnum_info.data_vmn_info.num_len;
            pb_entry.ton        = g_cur_vmnum_info.data_vmn_info.number_type;
            SCI_MEMSET( pb_entry.number, 0xFF, sizeof(pb_entry.number) );
            SCI_MEMCPY( pb_entry.number, g_cur_vmnum_info.data_vmn_info.bcd_num, pb_entry.number_len );
            break;
        default:
            //SCI_TRACE_LOW:"MMISMS_HandleSetVoiMessAddrCnf: record_id is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_115_112_2_18_2_53_44_497,(uint8*)"");
            break;
        }
        
        pb_entry.npi = MN_NUM_PLAN_ISDN_TELE;
        pb_entry.entry_id = g_cur_vmnum_info.record_id;
        result = PHONEBOOK_UpdateVoiceMailAddrEx(set_vm_addr_info.dual_sys, &pb_entry);
        if(ERR_MNDATAMAG_NO_ERR == result)
        {
            update_result = MN_RETURN_SUCCESS;         
        }
    }
    
    //关闭等待窗口
    MMK_CloseWin(MAIN_WAIT_WIN_ID);
    
    if ( (MN_SMS_OPERATE_SUCCESS == set_vm_addr_info.cause )|| 
        (MN_RETURN_SUCCESS== update_result))
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        //提示设置成功
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);     
#endif   
    }
    else
    {
        //提示设置失败
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }     

    switch(g_cur_vmnum_info.record_id)
    {
    case MMISMS_VOICEMAIL_LINE1:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.line1_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.line1_vmn_info.num_len, 
                            g_cur_vmnum_info.line1_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
    case MMISMS_VOICEMAIL_LINE2:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.line2_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.line2_vmn_info.num_len, 
                            g_cur_vmnum_info.line2_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;

    case MMISMS_VOICEMAIL_FAX:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.fax_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.fax_vmn_info.num_len, 
                            g_cur_vmnum_info.fax_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;

    case MMISMS_VOICEMAIL_DATA:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.data_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.data_vmn_info.num_len, 
                            g_cur_vmnum_info.data_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;

    default:
        break;
    }

    mail_len = MIN(mail_len, MMISMS_PBNUM_MAX_LEN);

    MMI_STRNTOWSTR(voice_mail_wstr,
                                MMISMS_PBNUM_MAX_LEN,
                                (uint8 *)voice_mail,
                                MMISMS_PBNUM_MAX_LEN,
                                mail_len);

    // set the edit box init param
    num_ptr.wstr_ptr = voice_mail_wstr;
    num_ptr.wstr_len = mail_len;
    GUILABEL_SetText(
                                    MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID,
                                    &num_ptr,
                                    FALSE
                                    );
    
    MMK_CloseWin(MMISMS_INPUTVM_WIN_ID);
    if(MMK_IsOpenWin(MMISMS_VM_EDIT_WIN_ID))
    {
        MMK_SendMsg(MMISMS_VM_EDIT_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);
    }
}

/*****************************************************************************/
//     Description : to handle the singal APP_MN_GET_VOIMESS_ADDR_CNF
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleGetVoiMessAddrCnf( 
                                           DPARAM param
                                           )
{
    APP_MN_GET_VMW_ADDR_CNF_T		get_vm_addr_info = *(APP_MN_GET_VMW_ADDR_CNF_T *)param;
    uint8							mail_len = 0;
    /*MN_MULTI_SUBSCRIBER_ID_TYPE_E*/uint8	record_id = 0;//20150618.vm
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
#endif
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    #ifndef MMI_GUI_STYLE_TYPICAL
    static uint8 count = 0;
    #endif
    SCI_TRACE_LOW("MMISMS:MMISMS_HandleGetVoiMessAddrCnf:get_vm_addr_info.dual_sys=%d, count = %d", get_vm_addr_info.dual_sys, count);

    if (get_vm_addr_info.dual_sys >=  MMI_DUAL_SYS_MAX)
    {
        if(MMK_IsOpenWin(MAIN_WAIT_WIN_ID))
        {
            //关闭等待窗口
            MMK_CloseWin(MAIN_WAIT_WIN_ID);        
        }
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleGetVoiMessAddrCnf get_vm_addr_info.dual_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_236_112_2_18_2_53_44_498,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleGetVoiMessAddrCnf:get APP_MN_GET_VMW_ADDR_CNF.The result is %d,record  id is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_239_112_2_18_2_53_44_499,(uint8*)"dd",get_vm_addr_info.cause,get_vm_addr_info.line_id);
    SCI_TRACE_LOW("MMISMS:MMISMS_HandleGetVoiMessAddrCnf:alphaid Len = %d, cause=%d", get_vm_addr_info.vmw_addr_t.alpha_id_len, get_vm_addr_info.cause);

#ifndef MMI_GUI_STYLE_TYPICAL
    if(g_vmn_oper_type != MMISMS_VMN_OPER_TYPE_SMS_VM_SETTING)
#endif
    {    
        if(MMK_IsOpenWin(MAIN_WAIT_WIN_ID))
        {
            //关闭等待窗口
            MMK_CloseWin(MAIN_WAIT_WIN_ID);        
        }
        else if (MN_SMS_SIM_READ_FAILED != get_vm_addr_info.cause)
        {
            return;
        }
    }
	
	SCI_TRACE_LOW("MMISMS_HandleGetVoiMessAddrCnf, g_vmn_oper_type = %d, get_vm_addr_info.cause = %d", g_vmn_oper_type, get_vm_addr_info.cause);
    
    if ( MN_SMS_OPERATE_SUCCESS == get_vm_addr_info.cause)
    {
        SCI_TRACE_LOW("MMISMS:MMISMS_HandleGetVoiMessAddrCnf: MN_SMS_OPERATE_SUCCESS");
        //record_id = get_vm_addr_info.line_id;//20150618.vm
        switch(get_vm_addr_info.vm_type)	//20150618.vm
        {
            case MN_VOICE_MAIL:
                if(MN_MULTI_SUBSCRIBER_ID_1 == get_vm_addr_info.line_id)
                {
                    record_id = MMISMS_VOICEMAIL_LINE1;
                }
                else if(MN_MULTI_SUBSCRIBER_ID_2 == get_vm_addr_info.line_id)
                {
                    record_id = MMISMS_VOICEMAIL_LINE2;
                }
                break;

            case MN_VOICE_MAIL_FAX:
                record_id = MMISMS_VOICEMAIL_FAX;
                break;

            case MN_VOICE_MAIL_EMAIL:
                record_id = MMISMS_VOICEMAIL_DATA;
                break;

            default:
                //do nothing;
                break;
        }
        #ifndef MMI_GUI_STYLE_TYPICAL
        count++;
        #endif
        switch( record_id ) 
        {
        case MMISMS_VOICEMAIL_LINE1:
            g_cur_vmnum_info.line1_vmn_info.num_len = get_vm_addr_info.vmw_addr_t.mailbox_number_len;
            SCI_MEMSET( g_cur_vmnum_info.line1_vmn_info.bcd_num, 0xFF, sizeof(g_cur_vmnum_info.line1_vmn_info.bcd_num)); 
            SCI_MEMCPY( g_cur_vmnum_info.line1_vmn_info.bcd_num,get_vm_addr_info.vmw_addr_t.mailbox_number, g_cur_vmnum_info.line1_vmn_info.num_len );
#ifdef MMI_SMS_VOICE_MAIL_DISPLAY_NAME_SUPPORT
            SCI_MEMSET( g_cur_vmnum_info.alpha_id, 0x00, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN); 
            SCI_MEMCPY( g_cur_vmnum_info.alpha_id, 
                get_vm_addr_info.vmw_addr_t.alpha_id, 
                MIN(PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN,get_vm_addr_info.vmw_addr_t.alpha_id_len)); 
            g_cur_vmnum_info.alpha_len = get_vm_addr_info.vmw_addr_t.alpha_id_len;
#endif
            g_cur_vmnum_info.line1_vmn_info.number_type = get_vm_addr_info.vmw_addr_t.ton;
            mail_len = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.line1_vmn_info.number_type, 
                g_cur_vmnum_info.line1_vmn_info.num_len, 
                g_cur_vmnum_info.line1_vmn_info.bcd_num, 
                voice_mail,
                MMISMS_PBNUM_MAX_LEN + 2);
            break;
            
        case MMISMS_VOICEMAIL_LINE2:
            g_cur_vmnum_info.line2_vmn_info.num_len = get_vm_addr_info.vmw_addr_t.mailbox_number_len;
            SCI_MEMSET( g_cur_vmnum_info.line2_vmn_info.bcd_num, 0xFF, sizeof(g_cur_vmnum_info.line2_vmn_info.bcd_num)); 
            SCI_MEMCPY( g_cur_vmnum_info.line2_vmn_info.bcd_num,get_vm_addr_info.vmw_addr_t.mailbox_number, g_cur_vmnum_info.line2_vmn_info.num_len );
#ifdef MMI_SMS_VOICE_MAIL_DISPLAY_NAME_SUPPORT
            SCI_MEMSET( g_cur_vmnum_info.alpha_id, 0x00, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN); 
            SCI_MEMCPY( g_cur_vmnum_info.alpha_id, 
                get_vm_addr_info.vmw_addr_t.alpha_id, 
                MIN(PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN,get_vm_addr_info.vmw_addr_t.alpha_id_len)); 
            g_cur_vmnum_info.alpha_len = get_vm_addr_info.vmw_addr_t.alpha_id_len;
#endif
            g_cur_vmnum_info.line2_vmn_info.number_type = get_vm_addr_info.vmw_addr_t.ton;
            mail_len = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.line2_vmn_info.number_type, 
                g_cur_vmnum_info.line2_vmn_info.num_len, 
                g_cur_vmnum_info.line2_vmn_info.bcd_num, 
                voice_mail,
                MMISMS_PBNUM_MAX_LEN + 2);
            break;
            
        case MMISMS_VOICEMAIL_FAX:
            g_cur_vmnum_info.fax_vmn_info.num_len = get_vm_addr_info.vmw_addr_t.mailbox_number_len;
            SCI_MEMSET( g_cur_vmnum_info.fax_vmn_info.bcd_num, 0xFF, sizeof(g_cur_vmnum_info.fax_vmn_info.bcd_num)); 
            SCI_MEMCPY( g_cur_vmnum_info.fax_vmn_info.bcd_num,get_vm_addr_info.vmw_addr_t.mailbox_number, g_cur_vmnum_info.fax_vmn_info.num_len );
            g_cur_vmnum_info.fax_vmn_info.number_type = get_vm_addr_info.vmw_addr_t.ton;
            mail_len = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.fax_vmn_info.number_type, 
                g_cur_vmnum_info.fax_vmn_info.num_len, 
                g_cur_vmnum_info.fax_vmn_info.bcd_num, 
                voice_mail,
                MMISMS_PBNUM_MAX_LEN + 2);
            break;
            
        case MMISMS_VOICEMAIL_DATA:
            g_cur_vmnum_info.data_vmn_info.num_len = get_vm_addr_info.vmw_addr_t.mailbox_number_len;
            SCI_MEMSET( g_cur_vmnum_info.data_vmn_info.bcd_num, 0xFF, sizeof(g_cur_vmnum_info.data_vmn_info.bcd_num)); 
            SCI_MEMCPY( g_cur_vmnum_info.data_vmn_info.bcd_num,get_vm_addr_info.vmw_addr_t.mailbox_number, g_cur_vmnum_info.data_vmn_info.num_len );
            g_cur_vmnum_info.data_vmn_info.number_type = get_vm_addr_info.vmw_addr_t.ton;
            mail_len = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.data_vmn_info.number_type, 
                g_cur_vmnum_info.data_vmn_info.num_len, 
                g_cur_vmnum_info.data_vmn_info.bcd_num, 
                voice_mail,
                MMISMS_PBNUM_MAX_LEN + 2);            
            
            //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleGetVoiMessAddrCnf: get voice addr from sim card"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_315_112_2_18_2_53_45_500,(uint8*)"");
            break;

        default:
            //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleGetVoiMessAddrCnf: THE record_id is wrong"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_319_112_2_18_2_53_45_501,(uint8*)"");
            break;
        }
    }
    else
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        count++;
#endif
        switch ( g_vmn_oper_type )
        {
        case MMISMS_VMN_OPER_TYPE_CALL_DIVERT:
            record_id = MMIAPISET_GetVMRecordId();            
            break;
            
        case MMISMS_VMN_OPER_TYPE_SMS_VM_SETTING:
            #ifdef MMI_GUI_STYLE_TYPICAL
            //to get the menu id
            GUIMENU_GetId( MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
            switch ( menu_id )
            {
#ifdef MMI_CSP_SUPPORT
            case MMISMS_MENU_LINE1_VMN_ITEM_ID:    
                record_id = 1;
                break;            
            case MMISMS_MENU_LINE2_VMN_ITEM_ID:
                record_id = 2;
                break;
#endif
            case MMISMS_MENU_FAX_VMN_ITEM_ID:
                record_id = 3;
                break;
            case MMISMS_MENU_DATA_VMN_ITEM_ID:
                record_id = 4;
                break;            
            default:
                //SCI_TRACE_LOW:" the menu id is error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_353_112_2_18_2_53_45_502,(uint8*)"");
                break;
            }    
            #else
            record_id = count;
            #endif
            break;
            
            case MMISMS_VMN_OPER_TYPE_IDLE_DIAL:
                // @baokun fix cr99885 begin
                get_vm_addr_info.dual_sys = s_dual_sys;
                //get_vm_addr_info.line_id = s_vmn_type;//20150618.vm
                record_id = s_vmn_type;
                //@baokun fix cr99885 end
                break;
                
            default:
                break;
        }
        MMISMS_GetVoiceMailAddr(get_vm_addr_info.dual_sys, record_id, voice_mail, &mail_len, MMISMS_PBNUM_MAX_LEN + 2 );
        if(0== mail_len && MN_SMS_SIM_READ_FAILED==get_vm_addr_info.cause && MMISMS_VMN_OPER_TYPE_CALL_DIVERT==g_vmn_oper_type)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SMS_BOX_EMPTY);
        }
        //SCI_TRACE_LOW:"HandleGetVoiMessAddrCnf: get the voice addr for the voicemail box!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_373_112_2_18_2_53_45_503,(uint8*)"");
    }
 
    switch ( g_vmn_oper_type )
    {
        SCI_TRACE_LOW("g_vmn_oper_type is %d",g_vmn_oper_type);
    case MMISMS_VMN_OPER_TYPE_CALL_DIVERT:
        #ifndef MMI_GUI_STYLE_TYPICAL
        count = 0;
        #endif
        MMIAPISET_OpenDivertCallWin(mail_len,voice_mail);
        break;
        
    case MMISMS_VMN_OPER_TYPE_SMS_VM_SETTING:
        // open show voice mail number window
#ifndef MMI_GUI_STYLE_TYPICAL
		//warning,if PS modify, this part should be adjust. now only line1 and line2, no fax and data, so count can not 4
         if(count == 4 /*|| (count >= 2 && (MN_SMS_SIM_READ_FAILED == get_vm_addr_info.cause || MN_SMS_OPERATE_SUCCESS == get_vm_addr_info.cause))*/)//20150618.vm
         {
             count = 0;
             if(MMK_IsOpenWin(MAIN_WAIT_WIN_ID))
             {
                 //关闭等待窗口
                 MMK_CloseWin(MAIN_WAIT_WIN_ID);        
             }
             else
             {
                 return;
             }
#ifndef MMI_MULTI_SIM_SYS_SINGLE               
            MMK_CloseWin(MMISMS_COM_SELECT_SIM_WIN_ID);
#endif
            MMISMS_OpenVoiceMailWin();
         }
         //20150618.vm
         else
         {
            OpenVMListWin(get_vm_addr_info.dual_sys,(MMISMS_LINE1_VMN_RECORD_ID + count));
         }
		
#else
		 MMISMS_EnterShowVmnWin();
#endif
        break;
        
    case MMISMS_VMN_OPER_TYPE_IDLE_DIAL:
#ifndef MMI_GUI_STYLE_TYPICAL
        count = 0;
#endif
		if(MMK_IsOpenWin(MAIN_WAIT_WIN_ID))
		{
			//关闭等待窗口
			MMK_CloseWin(MAIN_WAIT_WIN_ID);        
		}
        MMISMS_OpenCallVMWin(get_vm_addr_info.dual_sys, (MMISMS_VMN_RECORD_ID_E)/*get_vm_addr_info.line_id*/record_id);//20150618.vm
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//     Description : to handle the singal APP_MN_GET_VOIMESS_FLAG_CNF
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
/*****************************************************************************/
//     Description : to handle the singal APP_MN_GET_VOIMESS_FLAG_CNF
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleGetVoiMessFlagCnf( 
                                           DPARAM param
                                           )
{
    APP_MN_GET_VMW_FLAG_CNF_T  vm_addr_flag_info = *( APP_MN_GET_VMW_FLAG_CNF_T *)param;
    
    if(vm_addr_flag_info.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISMS_HandleGetVoiMessFlagCnf err dual sys: %d", vm_addr_flag_info.dual_sys);
        return;
    }
    
    if(MN_SMS_OPERATE_SUCCESS == vm_addr_flag_info.cause)
    {
        MMISMS_SetVMInfo(vm_addr_flag_info.dual_sys, &vm_addr_flag_info.vmw_flag);
    }
    
    MMISMS_UpdateVMFlagState();
}

/*****************************************************************************/
//     Description : Set Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_UpdateVMFlagState(void)
{
    BOOLEAN is_new_vm = FALSE;

    is_new_vm = IsNewVMBeing();
    
    MAIN_SetIdleVMFlagState( is_new_vm );

    SCI_TRACE_LOW("MMISMS: MMISMS_UpdateVMFlagState: if there is new voice message = %d", is_new_vm);
}

/*****************************************************************************/
//     Description : Set Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetVMInfo(MN_DUAL_SYS_E dual_sys, MN_SMS_VMW_IND_FLAG_T *vmw_flag)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == vmw_flag)
    {
        return;
    }

    SCI_MEMSET(&s_vmw_flag_type[dual_sys], 0x00, sizeof(s_vmw_flag_type[dual_sys]));
    SCI_MEMCPY(&s_vmw_flag_type[dual_sys], vmw_flag, sizeof(s_vmw_flag_type[dual_sys]));

    MMINV_WRITE(MMINV_SMS_VM_FLAG, s_vmw_flag_type);
}

/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ResetVMInfo(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    SCI_MEMSET(&s_vmw_flag_type[dual_sys], 0x00, sizeof(s_vmw_flag_type[dual_sys]));

    MMINV_WRITE(MMINV_SMS_VM_FLAG, s_vmw_flag_type);
}

/*****************************************************************************/
//     Description : Init Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitVMInfo(void)
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SMS_VM_FLAG, s_vmw_flag_type, return_val);

    if (return_val != MN_RETURN_SUCCESS)
    {
        SCI_MEMSET(s_vmw_flag_type, 0x00, sizeof(s_vmw_flag_type));

        MMINV_WRITE(MMINV_SMS_VM_FLAG, s_vmw_flag_type);
    }
}

/*****************************************************************************/
//     Description : to handle the singal APP_MN_VMW_FALG_IND
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleNewVMFlagInd( 
                                      DPARAM param
                                      )
{
    APP_MN_VMW_FALG_IND_T new_vm_flag_info = *(APP_MN_VMW_FALG_IND_T *)param;
    BOOLEAN is_new_vm = FALSE;

    if(new_vm_flag_info.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISMS_HandleNewVMFlagInd err dual sys: %d", new_vm_flag_info.dual_sys);
        return;
    }

    if (!g_is_sms_vm_header)
    {
        MMISMS_SetVMInfo(new_vm_flag_info.dual_sys, &new_vm_flag_info.vmw_flag);

        is_new_vm = IsNewVMBeingEx(new_vm_flag_info.dual_sys);
    }
    else
    {
        g_is_sms_vm_header = FALSE;

        is_new_vm = IsNewVMBeingEx(new_vm_flag_info.dual_sys);
    }

    if (is_new_vm)
    {
        MMIDEFAULT_TurnOnBackLight();
        MMIAPISMS_PlayNewSMSRing(new_vm_flag_info.dual_sys);
    }
    
    MMISMS_UpdateVMFlagState();
}

/*****************************************************************************/
//     Description : to get whether new vm being
//    Global resource dependence : none
//  Author:samboo.shen
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNewVMBeing(void)
{
    BOOLEAN is_new_vm = FALSE;
    MN_DUAL_SYS_E  dual_sys = MMI_DUAL_SYS_MAX;    

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        if (MMIAPIPHONE_IsSimCardOk(dual_sys))
        {
            uint8 line = MMIAPISET_GetCallLineInfo(dual_sys);

            if(1 == line)
            {
#if defined (MODEM_SUPPORT_SPRD)
                is_new_vm = s_vmw_flag_type[dual_sys].line2_status;
#elif defined(MODEM_SUPPORT_RDA)
                if( s_vmw_flag_type[dual_sys].line2_waiting_count)
                {
                    is_new_vm =1;
                }
                else
                {
                    is_new_vm = 0;
                }
#endif
            }
            else
            {
#if defined (MODEM_SUPPORT_SPRD)
                is_new_vm = s_vmw_flag_type[dual_sys].line1_status;
#elif defined(MODEM_SUPPORT_RDA)
                if( s_vmw_flag_type[dual_sys].line1_waiting_count)
                {
                    is_new_vm =1;
                }
                else
                {
                    is_new_vm = 0;
                }
#endif
            }

            if(!is_new_vm)
            {
                if(s_vmw_flag_type[dual_sys].is_datafax_flag_present)
                {
#if defined (MODEM_SUPPORT_SPRD)
                    is_new_vm = s_vmw_flag_type[dual_sys].data_status || s_vmw_flag_type[dual_sys].fax_status;
#elif defined(MODEM_SUPPORT_RDA)
                    if( s_vmw_flag_type[dual_sys].data_waiting_count|| s_vmw_flag_type[dual_sys].fax_waiting_count)
                    {
                        is_new_vm =1;
                    }
                    else
                    {
                        is_new_vm = 0;
                    }
#endif
                }
            }
        
            if(is_new_vm)
            {
                break;
            }
        }
    }    
    return is_new_vm;
}

/*****************************************************************************/
//     Description : to get whether new vm being
//    Global resource dependence : none
//  Author:samboo.shen
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNewVMBeingEx(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_new_vm = FALSE;
    uint8 line = MMIAPISET_GetCallLineInfo(dual_sys);

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return is_new_vm;
    }

    if (!MMIAPIPHONE_IsSimCardOk(dual_sys))
    {
        return is_new_vm;
    }

    if(1 == line)
    {
#if defined (MODEM_SUPPORT_SPRD)
        is_new_vm = s_vmw_flag_type[dual_sys].line2_status;
#elif defined(MODEM_SUPPORT_RDA)
        if( s_vmw_flag_type[dual_sys].line2_waiting_count)
        {
            is_new_vm =1;
        }
        else
        {
            is_new_vm = 0;
        }
#endif
    }
    else
    {
#if defined (MODEM_SUPPORT_SPRD)
        is_new_vm = s_vmw_flag_type[dual_sys].line1_status;
#elif defined(MODEM_SUPPORT_RDA)
        if( s_vmw_flag_type[dual_sys].line1_waiting_count)
        {
            is_new_vm =1;
        }
        else
        {
            is_new_vm = 0;
        }
#endif
    }

    if(!is_new_vm)
    {
        if(s_vmw_flag_type[dual_sys].is_datafax_flag_present)
        {
#if defined (MODEM_SUPPORT_SPRD)
            is_new_vm = s_vmw_flag_type[dual_sys].data_status || s_vmw_flag_type[dual_sys].fax_status;
#elif defined(MODEM_SUPPORT_RDA)
            if( s_vmw_flag_type[dual_sys].data_waiting_count|| s_vmw_flag_type[dual_sys].fax_waiting_count)
            {
                is_new_vm =1;
            }
            else
            {
                is_new_vm = 0;
            }
#endif
        }
    }

    return is_new_vm;
}

/*****************************************************************************/
//     Description : to get the voice mail
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_GetVoiceMailAddr( 
                                        MN_DUAL_SYS_E   dual_sys,
                                        uint8 record_id,
                                        uint8 *voice_mail_ptr, 
                                        uint8 *mail_len_ptr,
                                        uint8    max_mail_len
                                        )
{
    BOOLEAN ret_val = FALSE;
    PHONEBOOK_ENTRY_T  pb_entry = {0};
    ERR_MNDATAMAG_CODE_E retcode = ERR_MNDATAMAG_NO_ERR;
    MN_RETURN_RESULT_E result  = MN_RETURN_FAILURE;
    
    if (PNULL == voice_mail_ptr || PNULL == mail_len_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetVoiceMailAddr mail_len_ptr voice_mail_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_523_112_2_18_2_53_45_505,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET( &pb_entry, 0, sizeof( PHONEBOOK_ENTRY_T ) );
    pb_entry.entry_id = record_id; 
    
    retcode = PHONEBOOK_ReadVoiceMailAddrEx(dual_sys, &pb_entry);
    #ifdef MMI_GUI_STYLE_TYPICAL
    SCI_MEMSET(&g_cur_vmnum_info,0x00,sizeof(MMI_SMS_VMN_T));
    #endif
    if (ERR_MNDATAMAG_NO_ERR == retcode)
    {
        result = MN_RETURN_SUCCESS;
    }
    SCI_TRACE_LOW("MMISMS:MMISMS_GetVoiceMailAddr  pb_entry.alpha_len=%d,result=%d", pb_entry.alpha_len, result);
    //从电话本中读出号码
    if(MN_RETURN_SUCCESS ==  result)
    {
        SCI_TRACE_LOW("zhangqiang test pb_entry.entry_id =%d",pb_entry.entry_id);
        SCI_TRACE_BUF("zhangqiang test pb_entry.number",pb_entry.number,sizeof(pb_entry.number));
        SCI_TRACE_LOW("zhangqiang test pb_entry.number_len =%d",pb_entry.number_len);
        switch( pb_entry.entry_id ) 
        {
        case MMISMS_VOICEMAIL_LINE1:    
            g_cur_vmnum_info.line1_vmn_info.num_len = pb_entry.number_len;
            g_cur_vmnum_info.line1_vmn_info.number_type = pb_entry.ton;
            SCI_MEMSET( g_cur_vmnum_info.line1_vmn_info.bcd_num, 0, sizeof( g_cur_vmnum_info.line1_vmn_info.bcd_num ));
            SCI_MEMCPY( g_cur_vmnum_info.line1_vmn_info.bcd_num, pb_entry.number,g_cur_vmnum_info.line1_vmn_info.num_len);
#ifdef MMI_SMS_VOICE_MAIL_DISPLAY_NAME_SUPPORT
            SCI_MEMSET( g_cur_vmnum_info.alpha_id, 0x00, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN); 
            SCI_MEMCPY( g_cur_vmnum_info.alpha_id, pb_entry.alpha, MIN(PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN,pb_entry.alpha_len)); 
            g_cur_vmnum_info.alpha_len = pb_entry.alpha_len;
#endif
            (*mail_len_ptr ) = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.line1_vmn_info.number_type, 
                g_cur_vmnum_info.line1_vmn_info.num_len, 
                g_cur_vmnum_info.line1_vmn_info.bcd_num, 
                voice_mail_ptr,
                max_mail_len);
            ret_val = TRUE;
            break;            
        case MMISMS_VOICEMAIL_LINE2:
            g_cur_vmnum_info.line2_vmn_info.num_len = pb_entry.number_len;
            g_cur_vmnum_info.line2_vmn_info.number_type = pb_entry.ton;
            SCI_MEMSET( g_cur_vmnum_info.line2_vmn_info.bcd_num, 0, sizeof( g_cur_vmnum_info.line2_vmn_info.bcd_num ));
            SCI_MEMCPY( g_cur_vmnum_info.line2_vmn_info.bcd_num, pb_entry.number,g_cur_vmnum_info.line2_vmn_info.num_len);
#ifdef MMI_SMS_VOICE_MAIL_DISPLAY_NAME_SUPPORT
            SCI_MEMSET( g_cur_vmnum_info.alpha_id, 0x00, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN); 
            SCI_MEMCPY( g_cur_vmnum_info.alpha_id, pb_entry.alpha, MIN(PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN,pb_entry.alpha_len)); 
            g_cur_vmnum_info.alpha_len = pb_entry.alpha_len;            
#endif
            (*mail_len_ptr ) = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.line2_vmn_info.number_type, 
                g_cur_vmnum_info.line2_vmn_info.num_len, 
                g_cur_vmnum_info.line2_vmn_info.bcd_num, 
                voice_mail_ptr,
                max_mail_len);
            ret_val = TRUE;
            break;
        case MMISMS_VOICEMAIL_FAX:
            g_cur_vmnum_info.fax_vmn_info.num_len = pb_entry.number_len;
            g_cur_vmnum_info.fax_vmn_info.number_type = pb_entry.ton;
            SCI_MEMSET( g_cur_vmnum_info.fax_vmn_info.bcd_num, 0, sizeof( g_cur_vmnum_info.fax_vmn_info.bcd_num ));
            SCI_MEMCPY( g_cur_vmnum_info.fax_vmn_info.bcd_num, pb_entry.number,g_cur_vmnum_info.fax_vmn_info.num_len);
            (*mail_len_ptr ) = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.fax_vmn_info.number_type, 
                g_cur_vmnum_info.fax_vmn_info.num_len, 
                g_cur_vmnum_info.fax_vmn_info.bcd_num, 
                voice_mail_ptr,
                max_mail_len);
            ret_val = TRUE;
            break;
        case MMISMS_VOICEMAIL_DATA:
            g_cur_vmnum_info.data_vmn_info.num_len  = pb_entry.number_len;
            g_cur_vmnum_info.data_vmn_info.number_type = pb_entry.ton;
            SCI_MEMSET( g_cur_vmnum_info.data_vmn_info.bcd_num, 0, sizeof( g_cur_vmnum_info.data_vmn_info.bcd_num ));
            SCI_MEMCPY( g_cur_vmnum_info.data_vmn_info.bcd_num, pb_entry.number, g_cur_vmnum_info.data_vmn_info.num_len);
            (*mail_len_ptr ) = MMIAPICOM_GenDispNumber( 
                g_cur_vmnum_info.data_vmn_info.number_type, 
                g_cur_vmnum_info.data_vmn_info.num_len, 
                g_cur_vmnum_info.data_vmn_info.bcd_num, 
                voice_mail_ptr,
                max_mail_len);
            ret_val = TRUE;                
            break;
            
        default:
            break;
        }
    }   
    SCI_TRACE_LOW("zhangqiang test max_mail_len =%d",max_mail_len);
    SCI_TRACE_BUF("zhangqiang test voice_mail_ptr",voice_mail_ptr,max_mail_len);
    return ( ret_val );
}

/*****************************************************************************/
//     Description : to set the voice mail
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_SetVoiceMailAddr(
                                MN_DUAL_SYS_E   dual_sys,
                                uint8    *voice_mail_ptr,    //IN:
                                uint8    mail_len            //IN:
                                )
{
    BOOLEAN ret_val = FALSE;
    PHONEBOOK_ENTRY_T pb_entry = {0};
    MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW,0,0};
    MN_SMS_VMW_MBNUM_T voimess_addr_info = {0};  
    ERR_MNDATAMAG_CODE_E retcode = ERR_MNDATAMAG_NO_ERR;

    if (PNULL == voice_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SetVoiceMailAddr voice_mail_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_VOICEMAIL_621_112_2_18_2_53_45_506,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(&pb_entry, 0, sizeof (PHONEBOOK_ENTRY_T));
    SCI_MEMSET(pb_entry.number, 0xFF, sizeof (pb_entry.number));
    MMIAPICOM_GenPartyNumber( 
        voice_mail_ptr, 
        mail_len, 
        &party_num 
        );
    
    // set to phone_book format
    pb_entry.entry_id    = 1;
    pb_entry.ton        = party_num.number_type;
    pb_entry.npi        = MN_NUM_PLAN_ISDN_TELE;
    pb_entry.number_len = MIN(party_num.num_len, sizeof (pb_entry.number));
    
    SCI_MEMCPY( 
        pb_entry.number, 
        party_num.bcd_num, 
        pb_entry.number_len 
        );
    
    //set the parameter ，未设置全
    voimess_addr_info.alpha_id_len   = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN;//要改
    voimess_addr_info.mailbox_number_len = MIN(party_num.num_len, PHONEBOOK_MAX_NORMAL_DN_NUMBER_LEN);
    voimess_addr_info.is_ton_npi_exist = TRUE;
    voimess_addr_info.is_ccp_exist     = FALSE;
    voimess_addr_info.ccp_id           = 0;
    voimess_addr_info.is_ext_exist     = FALSE;
    voimess_addr_info.ton              = party_num.number_type;
    voimess_addr_info.npi              = MN_NUM_PLAN_ISDN_TELE;
    voimess_addr_info.ext_id           = 0;  //要改
    SCI_MEMSET( voimess_addr_info.mailbox_number, 0, sizeof(voimess_addr_info.mailbox_number));
    SCI_MEMCPY( voimess_addr_info.mailbox_number, party_num.bcd_num, voimess_addr_info.mailbox_number_len);
    
    //先判断sim卡是否支持cphs协议，若支持则语音信箱的号码存入sim卡
    //MNSMS_SetVoiMessAddrEx(dual_sys, 0,&voimess_addr_info);  
    
//    if( (g_set_voimess_info.is_get_set_vm_addr_cnf) && (g_set_voimess_info.is_set_vm_addr_success))
//    {
//        ret_val = TRUE;
//    }
//    else //sim卡不支持cphs协议，按原有方式存信箱号码
    {
        retcode = PHONEBOOK_UpdateVoiceMailAddrEx(dual_sys, &pb_entry);
        if (ERR_MNDATAMAG_NO_ERR == retcode)
        {
            ret_val = TRUE;
        }
    }
    //将状态设为默认值
//    g_set_voimess_info.is_get_set_vm_addr_cnf = FALSE;
//    g_set_voimess_info.is_set_vm_addr_success = FALSE;
    
    return (ret_val);
}
/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMI_SMS_VMN_T MMISMS_GetCurVmInfo(void)
{
    return g_cur_vmnum_info;
}

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurVmInfo(MMI_SMS_VMN_T *cur_vmnum_info)
{
    SCI_MEMCPY(&g_cur_vmnum_info,cur_vmnum_info,sizeof(MMI_SMS_VMN_T));
}


/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitCurVmInfo(void)
{
    SCI_MEMSET(&g_cur_vmnum_info,0x00,sizeof(MMI_SMS_VMN_T));
}

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetVmOperType(MMISMS_VMN_OPER_TYPE_E type)
{
    g_vmn_oper_type = type;
}

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_VMN_OPER_TYPE_E MMISMS_GetVmOperType(void)
{
    return g_vmn_oper_type;
}

/*****************************************************************************/
//     Description : to open the window to call the voice mail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenCallVMWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type)
{
    uint8 voice_mail[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 mail_len = 0;

    if (dual_sys >= MMI_DUAL_SYS_MAX || vmn_type >= MMISMS_VMN_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_OpenCallVMWin dual_sys >= MMI_DUAL_SYS_MAX || vmn_type >= MMISMS_VMN_TYPE_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_15849_112_2_18_2_54_54_783,(uint8*)"");
        return;
    }

    switch( vmn_type ) 
    {
    case MMISMS_LINE1_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.line1_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.line1_vmn_info.num_len, 
                            g_cur_vmnum_info.line1_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
        
    case MMISMS_LINE2_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.line2_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.line2_vmn_info.num_len, 
                            g_cur_vmnum_info.line2_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
    case MMISMS_FAX_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.fax_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.fax_vmn_info.num_len, 
                            g_cur_vmnum_info.fax_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;

    case MMISMS_DATA_VMN_RECORD_ID:
        mail_len = MMIAPICOM_GenDispNumber( 
                            g_cur_vmnum_info.data_vmn_info.number_type,
                            (uint8)g_cur_vmnum_info.data_vmn_info.num_len, 
                            g_cur_vmnum_info.data_vmn_info.bcd_num,
                            voice_mail,
                            MMISMS_PBNUM_MAX_LEN + 2
                            );
        break;
        
    default:
        break;
    }
    
    if (0 == mail_len)
    {
        //提示用户语音信箱为空
        //Modify by huangfengming for cr149701
#ifndef MMI_MULTI_SIM_SYS_SINGLE       
            MMIPUB_OpenAlertWarningWin(TXT_SMS1_VOICEMAIL_EMPTY+((MMI_TEXT_ID_T)dual_sys-MN_DUAL_SYS_1));        
#else
            MMIPUB_OpenAlertWarningWin(TXT_SMS_SIM_VOICEMAIL_EMPTY);
#endif
    }
    else
    {
        CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
#ifdef MMI_SMS_VOICE_MAIL_DISPLAY_NAME_SUPPORT
        MMI_STRING_T  cc_name = {0};
        wchar wcName[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN + 1] = {0};

        SCI_TRACE_LOW("MMISMS:MMISMS_OpenCallVMWin alphaLen=%d", g_cur_vmnum_info.alpha_len);
        cc_name.wstr_len = g_cur_vmnum_info.alpha_len;
        SCI_MEMSET( wcName, 0x00, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN); 
        MMI_STRNTOWSTR( wcName,
                PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN,
                (const uint8 *)g_cur_vmnum_info.alpha_id,
                PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN,
                g_cur_vmnum_info.alpha_len);
        cc_name.wstr_ptr = wcName;
#endif
        cc_result = MMIAPICC_MakeCall(
                                                        dual_sys,
                                                        voice_mail,
                                                        mail_len,
                                                        PNULL,
                                                        #ifdef MMI_SMS_VOICE_MAIL_DISPLAY_NAME_SUPPORT
                                                        (g_cur_vmnum_info.alpha_len?&cc_name:PNULL),
                                                        #else
                                                        PNULL,
                                                        #endif
                                                        MMIAPICC_GetCallSimType(dual_sys),
                                                        CC_CALL_NORMAL_CALL,
                                                        PNULL
                                                        );

        if (CC_RESULT_SUCCESS == cc_result)
        {
            MMK_CloseWin( MMISMS_SHOW_VMN_WIN_ID );
        }
    }
}


#endif
