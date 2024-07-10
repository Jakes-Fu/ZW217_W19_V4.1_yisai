/****************************************************************************
** File Name:      mmisms_save.c                                            *
** Author:                                                                  *
** Date:           8/23/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe saving SMS.                *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2007        liming.deng      Create                                  *
**                                                                         *
****************************************************************************/
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_save.h"
#include "mmisms_app.h"
#include "mmisms_send.h"
#include "mmisms_order.h"
#include "mn_api.h"
#include "mmisms_export.h"
#include "mmi_signal_ext.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmipub.h"
#include "mmisms_edit.h"
#include "mmi_text.h"
#include "mmisms_set.h"
#include "mmisms_delete.h"
#ifdef JAVA_SUPPORT
#include "mmismsjava_api.h"
#endif
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"
#include "mmisms_read.h"
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmimms_export.h"
#endif
#include "mmipb_app.h"
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#include "met_mex_app_export.h"
#endif

#include "mmisms_appcontrol.h"
#include "mmismsapp_interface.h"

#ifdef MMI_SMS_SYN_SUPPORT
#include "mmibt_id.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISMS_VCARD_MAX_UCS2_BYTE_LEN        133 //134
#define MMISMS_VCARD_MAX_UCS2_SPLIT_LEN        128 //128

#define MMISMS_VCARD_MAX_DEF_BYTE_LEN        154
#define MMISMS_VCARD_MAX_DEF_SPLIT_LEN        146

#define MMISMS_VCARD_MAX_8BIT_BYTE_LEN        133//134
#define MMISMS_VCARD_MAX_8BIT_SPLIT_LEN_SAVE        127//128

MMISMS_ORDER_ID_T s_mmisms_next_long_sms_ptr = PNULL;

#ifdef MMI_SMS_SYN_SUPPORT
#define COMMAND_TOTAL 9  //sms syn

typedef enum
{
    IMRNT, //Total unread sms
    IMROT, //Total read sms
    IMRNH, //unread sms head
    IMRNC, //unread sms content
    IMROH, //read sms head
    IMROC, //read sms content
    IMSRT, //Send status
    IMSDT, //Deliver status
    IMNMI, //New Message is arrive
    IM_MAX
}MMISMS_AT_COMMAND_E;

LOCAL uint16 total_unread = 0;
LOCAL uint16 total_read = 0;
LOCAL BOOLEAN newsms_flag = FALSE;
LOCAL BOOLEAN long_sms_flag = FALSE;
LOCAL uint16 newsms_seq = 0;
LOCAL uint16 con_len = 0;

LOCAL uint8 sms_content[MMISMS_MAX_MESSAGE_LEN + 1] = {0};

BOOLEAN Syn_end = FALSE;

LOCAL BOOLEAN syn_unread = FALSE;
LOCAL BOOLEAN sms_unread_flag = FALSE;


LOCAL const char command[][5] ={

                "IMRNT",
                "IMROT",
                "IMRNH",
                "IMRNC",
                "IMROH",
                "IMROC",
                "IMSRT",
                "IMSDT",
                "IMNMI"
};

LOCAL uint8 number[40];
LOCAL uint8 time_date[14];

LOCAL uint8 syn_sms_total = 0;
#endif

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS��ȫ�ֱ���
extern MMISMS_SENDINFO_T s_mmisms_sendlist;

#ifdef IKEYBACKUP_SUPPORT 
extern MN_SMS_STATUS_E   g_sms_restore_status;
extern MMISMS_FOLDER_TYPE_E g_sms_restore_folder_type;
extern MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *g_restore_param_in_ptr;
extern MMISMS_MO_MT_TYPE_E g_mo_mt_type;
extern uint32 g_sms_save_time;
#endif

uint16 vcard_vcal_cur_save_index = 0; 

#ifdef MMI_SMS_SYN_SUPPORT
/*****************************************************************************/
//  Description : send syn next sms request
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
PUBLIC void MMISMS_SendNextSmsRequest();
#endif

/*****************************************************************************/
//     Description : to send saved request to MN
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SaveMsgReq(
                                   MN_DUAL_SYS_E            dual_sys,
                                   uint8                    *dest_addr_ptr,        //IN:
                                   uint8                    dest_len,            //IN:
                                   APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                   MN_SMS_STORAGE_E            storage,            //IN:
                                   MN_SMS_ALPHABET_TYPE_E    alphabet_type        //IN:
                                   );

/*****************************************************************************/
//     Description : to send saved request to MN
//    Global resource dependence : none
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SaveClass0MsgReq(
                                   MN_DUAL_SYS_E            dual_sys,
                                   uint8                    *ori_addr_ptr,        //IN:
                                   uint8                    ori_addr_len,            //IN:
                                   APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                   MN_SMS_STORAGE_E            storage,            //IN:
                                   MN_SMS_ALPHABET_TYPE_E    alphabet_type        //IN:
                                   );

/*****************************************************************************/
//     Description : to send update request to MN
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_UpdateMsgReq(                                            //RETURN:
                                            MN_DUAL_SYS_E           dual_sys,
                                            uint8                    *dest_addr_ptr,        //IN:
                                            uint8                    dest_len,            //IN:
                                            MN_SMS_STATUS_E        status,                //IN:
                                            APP_SMS_USER_DATA_T     *user_data_ptr,        //IN:
                                            MN_SMS_STORAGE_E        storage,            //IN:
                                            MN_SMS_RECORD_ID_T        record_id,            //IN:
                                            MN_SMS_ALPHABET_TYPE_E alphabet_type
                                            );

/*****************************************************************************/
//  Description : to send saved request to MN
//  Global resource dependence : none
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SaveCopyMsgReq(
                                       MN_DUAL_SYS_E            dual_sys,
                                       uint8                    *dest_addr_ptr,        //IN:
                                       uint8                    dest_len,            //IN:
                                       APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                       MN_SMS_STORAGE_E            storage,            //IN:
                                       MN_SMS_ALPHABET_TYPE_E    alphabet_type        //IN:
                                       );

/*****************************************************************************/
//  Description : to check whether it has the next SMS to send according to the 
//                head of user data
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_IsExistNextCopySMS( void );

/*****************************************************************************/
// Description : Get current max number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL uint8 Get_Read_Head_Max_Num(void);

/*****************************************************************************/
// Description : Get current max number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Read_Head_Ref_Num(void);

/*****************************************************************************/
// Description : Get current seq number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL uint8 Get_Read_Current_Seq_Num(void);
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description :mms number to bec
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void MMSNumberToBcd(uint8 *sender,uint8 *number,uint8 *len);
#endif

/*****************************************************************************/
//     Description : to send saved request to MN
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SaveMsgReq(
                                   MN_DUAL_SYS_E            dual_sys,
                                   uint8                    *dest_addr_ptr,        //IN:
                                   uint8                    dest_len,            //IN:
                                   APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                   MN_SMS_STORAGE_E            storage,            //IN:
                                   MN_SMS_ALPHABET_TYPE_E    alphabet_type        //IN:
                                   )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MO_SMS_T mo_sms = {0};
    SCI_TIME_T time = {0};
    SCI_DATE_T date = {0};

    if (PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq user_data_ptr  = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_178_112_2_18_2_53_9_252,(uint8*)"");
        return save_result;
    }
    if (MN_SMS_STORAGE_ALL == storage)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_183_112_2_18_2_53_9_253,(uint8*)"");
        return save_result;
    }
    
    if (MN_SMS_NO_STORED != storage)
    {
        SCI_MEMSET(&mo_sms, 0, sizeof(MN_SMS_MO_SMS_T));
        
        mo_sms.dest_addr_present = TRUE;
        
        if ((dest_len > 0) && (PNULL != dest_addr_ptr))
        {
            MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};

            MMISMS_GetMnCalledNum(dest_addr_ptr, dest_len, &party_number, (&mo_sms.dest_addr_t));
            
            g_mmisms_global.save_info.number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(party_number.number_type, MN_NUM_PLAN_UNKNOW);
            g_mmisms_global.save_info.number.number_len = party_number.num_len;
            SCI_MEMCPY(g_mmisms_global.save_info.number.number, party_number.bcd_num, party_number.num_len);
        }

        g_mmisms_global.save_info.folder_type = g_mmisms_global.folder_type;
        
        TM_GetSysTime(&time);
        TM_GetSysDate(&date);
        mo_sms.dcs.alphabet_type = alphabet_type;
        mo_sms.time_stamp_t.day = date.mday;
        mo_sms.time_stamp_t.hour = time.hour;
        mo_sms.time_stamp_t.minute = time.min;
        mo_sms.time_stamp_t.month = date.mon;
        mo_sms.time_stamp_t.second = time.sec;
        mo_sms.time_stamp_t.timezone = 0;    //������
        mo_sms.time_stamp_t.year = date.year - MMISMS_OFFSERT_YEAR;

        if ((MMISMS_SAVE_SMS == MMISMS_GetCurrentOperStatus()) ||
            (MMISMS_UPDATE_NOSEND_SMS == MMISMS_GetCurrentOperStatus()))
        {
            mo_sms.sc_addr_present = TRUE;
        }

#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_BOX_SENDFAIL == MMISMS_TIMERMSG_GetCurrBoxType())
        {
            mo_sms.sc_addr_present = FALSE;
        }
#endif

        MMISMS_EncodeMoSmsData( 
            dual_sys,
            &mo_sms,
            alphabet_type,
            user_data_ptr
            );
//============== MET MEX  start ===================
#ifdef MET_MEX_SUPPORT
        if (MMISMS_SEND_SAVE_SMS == MMISMS_GetCurrentOperStatus())
        {
            err_code = MNSMS_WriteMoSmsEx(dual_sys, storage, MN_SMS_MO_SR_NOT_REQUEST, &mo_sms);
        }
        else
#endif // MET_MEX_SUPPORT
//============== MET MEX  end ===================
        {
            err_code = MNSMS_WriteMoSmsEx(dual_sys, storage, MN_SMS_MO_TO_BE_SEND, &mo_sms);
        }
        
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_249_112_2_18_2_53_9_254,(uint8*)"d", err_code);
        if (ERR_MNSMS_NONE == err_code)
        {
            save_result = MMISMS_NO_ERR;
        }
        else
        {
            save_result = MMISMS_SAVE_FAIL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_261_112_2_18_2_53_9_255,(uint8*)"");
        save_result = MMISMS_NO_SPACE;
    }
    
    return (save_result);
}

/*****************************************************************************/
//     Description : to send saved request to MN
//    Global resource dependence : none
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SaveClass0MsgReq(
                                   MN_DUAL_SYS_E            dual_sys,
                                   uint8                    *ori_addr_ptr,        //IN:
                                   uint8                    ori_addr_len,            //IN:
                                   APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                   MN_SMS_STORAGE_E            storage,            //IN:
                                   MN_SMS_ALPHABET_TYPE_E    alphabet_type        //IN:
                                   )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MT_SMS_T mt_sms = {0};

    if (PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq user_data_ptr  = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_287_112_2_18_2_53_9_256,(uint8*)"");
        return save_result;
    }
    if (MN_SMS_STORAGE_ALL == storage)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_292_112_2_18_2_53_9_257,(uint8*)"");
        return save_result;
    }
    
    if (MN_SMS_NO_STORED != storage)
    {
        SCI_MEMSET(&mt_sms, 0, sizeof(MN_SMS_MT_SMS_T));
        
        if ((ori_addr_len > 0) && (PNULL != ori_addr_ptr))
        {
            g_mmisms_global.save_info.number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(g_mmisms_global.class0_msg.address.number_type, MN_NUM_PLAN_UNKNOW);
            g_mmisms_global.save_info.number.number_len = g_mmisms_global.class0_msg.address.num_len;
            SCI_MEMCPY(g_mmisms_global.save_info.number.number, 
                                  g_mmisms_global.class0_msg.address.party_num,
                                  g_mmisms_global.class0_msg.address.num_len);
            SCI_MEMCPY(&mt_sms.origin_addr_t, 
                                  &g_mmisms_global.class0_msg.address,
                                  sizeof(MN_CALLED_NUMBER_T));
        }
        
        SCI_MEMCPY(&mt_sms.time_stamp_t, 
                              &g_mmisms_global.class0_msg.time_stamp,
                              sizeof(MN_SMS_TIME_STAMP_T));

        g_mmisms_global.save_info.time = MMIAPICOM_Tm2Second(mt_sms.time_stamp_t.second,
                        mt_sms.time_stamp_t.minute,
                        mt_sms.time_stamp_t.hour,
                        mt_sms.time_stamp_t.day,
                        mt_sms.time_stamp_t.month,
                        mt_sms.time_stamp_t.year + MMISMS_MT_OFFSERT_YEAR
                        );  
                      
        mt_sms.dcs.alphabet_type = alphabet_type;

        MMISMS_EncodeMtSmsData( 
            dual_sys,
            &mt_sms,
            alphabet_type,
            user_data_ptr
            );
        
        err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, MN_SMS_MT_READED, &mt_sms);
        
        MMISMS_SetOperStatus(MMISMS_SAVE_CLASS0_SMS);

        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_329_112_2_18_2_53_9_258,(uint8*)"d", err_code);
        if (ERR_MNSMS_NONE == err_code)
        {
            save_result = MMISMS_NO_ERR;
        }
        else
        {
            save_result = MMISMS_SAVE_FAIL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_341_112_2_18_2_53_9_259,(uint8*)"");
        save_result = MMISMS_NO_SPACE;
    }
    
    return (save_result);
}

/*****************************************************************************/
//     Description : to send saved request to MN
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_UpdateMsgReq(                                            //RETURN:
                                            MN_DUAL_SYS_E            dual_sys,
                                            uint8                    *dest_addr_ptr,        //IN:
                                            uint8                    dest_len,            //IN:
                                            MN_SMS_STATUS_E        status,                //IN:
                                            APP_SMS_USER_DATA_T     *user_data_ptr,        //IN:
                                            MN_SMS_STORAGE_E        storage,            //IN:
                                            MN_SMS_RECORD_ID_T        record_id,            //IN:
                                            MN_SMS_ALPHABET_TYPE_E alphabet_type
                                            )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MO_SMS_T mo_sms = {0};
    SCI_TIME_T time = {0};
    SCI_DATE_T date = {0};
    
    if (PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq user_data_ptr  = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_371_112_2_18_2_53_9_260,(uint8*)"");
        return save_result;
    }
    if (MN_SMS_STORAGE_ALL == storage)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_376_112_2_18_2_53_9_261,(uint8*)"");
        return save_result;
    }
    
    if (MN_SMS_NO_STORED != storage)
    {
        SCI_MEMSET(&mo_sms, 0, sizeof(MN_SMS_MO_SMS_T));
        
        if (MN_SMS_FREE_SPACE != status) // if status is MN_SMS_FREE_SPACE, the message will be deleted.
        {
            if ( ( PNULL != dest_addr_ptr ) && ( 0 < dest_len ) )
            {
                MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};

                // translate the destation address to BCD code
                MMISMS_GetMnCalledNum(dest_addr_ptr, dest_len, &party_number, (&mo_sms.dest_addr_t));
            }
            
            mo_sms.dest_addr_present = TRUE;
            mo_sms.dcs.alphabet_type = alphabet_type;

            TM_GetSysTime(&time);
            TM_GetSysDate(&date);
            mo_sms.time_stamp_t.day = date.mday;
            mo_sms.time_stamp_t.hour = time.hour;
            mo_sms.time_stamp_t.minute = time.min;
            mo_sms.time_stamp_t.month = date.mon;
            mo_sms.time_stamp_t.second = time.sec;
            mo_sms.time_stamp_t.timezone = 0;    //������
            mo_sms.time_stamp_t.year = date.year - MMISMS_OFFSERT_YEAR;
            
            MMISMS_EncodeMoSmsData(
                dual_sys,
                &mo_sms,
                mo_sms.dcs.alphabet_type,
                user_data_ptr
                );
        }        
        if (MMISMS_UPDATE_NOSEND_SMS == MMISMS_GetCurrentOperStatus())
        {
            mo_sms.sc_addr_present = TRUE;
        }
        
        // call MN API function
        if (status != MN_SMS_FREE_SPACE)
        {
            err_code = MNSMS_EditMoSmsEx(dual_sys, record_id, storage, status, &mo_sms);
        }
        else
        {
            err_code = MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, status);
        }
        
        //SCI_TRACE_LOW:"MMISMS: UpdateMsgReq err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_429_112_2_18_2_53_9_262,(uint8*)"d", err_code);
        
        if (ERR_MNSMS_NONE == err_code)
        {
            save_result = MMISMS_NO_ERR;
        }
        else
        {
            save_result = MMISMS_SAVE_FAIL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: UpdateMsgReq the storage is MN_SMS_NO_STORED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_442_112_2_18_2_53_9_263,(uint8*)"");
        save_result = MMISMS_NO_SPACE;
    }
    
    return (save_result);
}

/*****************************************************************************/
//     Description : to handle ps signal of APP_MN_WRITE_SMS_CNF
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSaveSMSCnf(
                                    DPARAM    param
                                    )
{
    APP_MN_WRITE_SMS_CNF_T *sig_ptr = (APP_MN_WRITE_SMS_CNF_T *)param;
    // uint16 max_sms_num = 0;
    BOOLEAN is_success = FALSE;
    MMISMS_OPER_STATUS_E oper_status = MMISMS_GetCurrentOperStatus();
    MN_SMS_STATUS_E save_status = MN_SMS_MO_TO_BE_SEND;
    BOOLEAN  is_exist_sc_addr = FALSE;
    uint8 *dest_addr_ptr = PNULL;
    uint8 fail_id[ MMISMS_DEST_ADDR_MAX_NUM ]= { 0 };
    uint16 send_result = 0;
    uint16 dest_count = 0;
    uint8 id = 0;
    // MN_SMS_STATUS_E *status_ptr = PNULL;
    // uint16 used_sms_num = 0;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;
    MMISMS_UPDATE_INFO_T    *update_info_ptr= PNULL;
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;

    MMISMS_SEND_T           *send_info_ptr = PNULL;
    MMISMS_SEND_T           *cur_send_info_ptr = PNULL;
    BOOLEAN is_find_oper = FALSE;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    MN_SMS_STORAGE_E        sending_storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T        sending_record_id = 0;
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
   char handle_number[HANDLE_LEN] = {0};
#endif
   
    if (!sig_ptr)
    {
        return;
    }

    if(PNULL !=s_mmisms_sendlist.sendlist_head_ptr)
    {
        SCI_TRACE_LOW("mmisms1:HandleSaveSMSCnf backsend_statusSTART = %d!",s_mmisms_sendlist.sendlist_head_ptr->backsend_status);
    }

    do 
    {    
        if (MN_SMS_ME_MEM_FULL == sig_ptr->cause || MN_SMS_SIM_MEM_FULL == sig_ptr->cause)
        {
            //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSaveSMSCnf sig_ptr = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_486_112_2_18_2_53_10_264,(uint8*)"");
            if (MN_SMS_ME_MEM_FULL == sig_ptr->cause || MN_SMS_SIM_MEM_FULL == sig_ptr->cause)
            {
                send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
                if(PNULL != send_info_ptr)
                {
                    if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content)) //���ͳ����ŵ���һ��
                    {
                        // send the next message to the same destination
                        MMISMS_SendMsgBySeqNum(sig_ptr->dual_sys, GET_HEAD_MAX_NUM(&send_info_ptr->send_content), GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1, FALSE, send_info_ptr);
                    }
                    else
                    {
                        MMISMS_ClearDestAddrById();

                        OperAfterSendCnf(sig_ptr->dual_sys, TRUE, TRUE);
                    }
                }
            }
            
            ret_val = MMISMS_FINISH_OPER_ERR; 
            return;
        }        
        //SCI_TRACE_LOW:"MMISMS: HandleSaveSMSCnf record_id = %d, storage = %d, cause = %d, dual_sys = %d, oper_status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_490_112_2_18_2_53_10_265,(uint8*)"ddddd",sig_ptr->record_id, sig_ptr->storage, sig_ptr->cause,sig_ptr->dual_sys, oper_status);
    
        send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SAVING);
        if (PNULL != send_info_ptr)
        {
            if (MN_SMS_NO_STORED != send_info_ptr->cur_send_data.cur_send_storage 
                && sig_ptr->storage == send_info_ptr->cur_send_data.cur_send_storage
                && sig_ptr->record_id == send_info_ptr->cur_send_data.cur_send_record_id)
            {
                is_find_oper = TRUE;
                oper_status = send_info_ptr->cur_send_data.cur_send_oper;
            }
            if (is_find_oper)
            {            
                cur_send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING); 
                if (PNULL != cur_send_info_ptr && send_info_ptr != cur_send_info_ptr)
                {
                    if(MMISMS_NO_SEND_STATUS != cur_send_info_ptr->send_save_result.send_status)
                    {
                        oper_status = cur_send_info_ptr->cur_send_data.cur_send_oper;                
                        SCI_TRACE_LOW("mmisms MMISMS_SENDSTATUS_SAVING bu send cnf,send status %d",send_info_ptr->send_save_result.send_status);
                        MMISMS_UpdateCurSendRecordId(send_info_ptr, sig_ptr->storage, sig_ptr->record_id, TRUE);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMISMS: HandleSaveSMSCnf 11 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_508_112_2_18_2_53_10_266,(uint8*)"d",cur_send_info_ptr->cur_send_data.cur_send_record_id);
                        MMISMS_UpdateCurSendRecordId(cur_send_info_ptr, sig_ptr->storage, sig_ptr->record_id, TRUE);
                    }
                }                        
            }
        }
        send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
        if (PNULL != send_info_ptr && !is_find_oper)
        {
            if (MN_SMS_NO_STORED != send_info_ptr->cur_send_data.cur_send_storage 
                && sig_ptr->storage == send_info_ptr->cur_send_data.cur_send_storage
                && sig_ptr->record_id == send_info_ptr->cur_send_data.cur_send_record_id)
            {
                SCI_TRACE_LOW("mmisms send status %d",send_info_ptr->send_save_result.send_status);
                if(MMISMS_NO_SEND_STATUS != send_info_ptr->send_save_result.send_status)
                {
                    oper_status = send_info_ptr->cur_send_data.cur_send_oper;
                    cur_send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SAVING);
                    if (PNULL != cur_send_info_ptr && send_info_ptr != cur_send_info_ptr)
                    {
                        //SCI_TRACE_LOW:"MMISMS: HandleSaveSMSCnf 22 cur_send_info_ptr->cur_send_data.cur_send_record_id = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_525_112_2_18_2_53_10_267,(uint8*)"d",cur_send_info_ptr->cur_send_data.cur_send_record_id);
                        MMISMS_UpdateCurSendRecordId(cur_send_info_ptr, sig_ptr->storage, sig_ptr->record_id, FALSE);
                    }
                }
            }
            else
            {
                oper_status = send_info_ptr->cur_send_data.cur_send_oper; //FIXBUG: 1490994
                SCI_TRACE_LOW("MMISMS:MMISMS_HandleSaveSMSCnf sig_ptr !=send_info_ptr !");
            }
        }
    
        //update record id
        if (MMISMS_SAVE_SEND_SMS != oper_status && MMISMS_SEND_SAVE_SMS != oper_status && MMISMS_SAVE_SENDFAIL_SMS != oper_status)
        {//�ڱ��������Ķ��ŵ�ʱ��Ҫ����record id
            send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SAVING);
            if (PNULL != send_info_ptr)
            {
                if (MN_SMS_NO_STORED != send_info_ptr->cur_send_data.cur_send_storage 
                    && sig_ptr->storage == send_info_ptr->cur_send_data.cur_send_storage
                    && sig_ptr->record_id == send_info_ptr->cur_send_data.cur_send_record_id)
                {
                    is_find_oper = TRUE;
                    MMISMS_GetStorageAndRecordidForSendSms(send_info_ptr, &sending_storage, &sending_record_id);
                    if (MN_SMS_NO_STORED != sending_storage)
                    {
                        send_info_ptr->cur_send_data.cur_send_record_id = sending_record_id;
                        send_info_ptr->cur_send_data.cur_send_storage = sending_storage;
                    }
                }
            }
            send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
            if (PNULL != send_info_ptr && !is_find_oper)
            {
                MMISMS_UpdateCurSendRecordId(send_info_ptr, sig_ptr->storage, sig_ptr->record_id, FALSE);
            }
        }
        
        if (MN_SMS_OPERATE_SUCCESS == sig_ptr->cause) 
        {//����ɹ�����
            is_success = TRUE;
            SCI_TRACE_LOW("MMISMS:MMISMS_HandleSaveSMSCnf switch oper_status=%d",oper_status);
            switch (oper_status)
            {

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        case MMISMS_BT_SAVE_MT_SMS:
            
            if(MMIAPISMS_GetCurSMSIsRead())
            {
                save_status = MN_SMS_MT_READED;
            }
            else
            {
                save_status = MN_SMS_MT_TO_BE_READ;
            }

            g_mmisms_global.folder_type = MMISMS_FOLDER_BT;
            folder_type = MMISMS_FOLDER_BT;
            MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);

            MMISMS_SaveInfoToOrderByResult(
                sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                save_status, is_exist_sc_addr, MMISMS_FOLDER_BT,  oper_status,  PNULL, MMISMS_SENDSTATUS_NONE);
            
            edit_content_ptr = &g_mmisms_global.edit_content;
            
            if(1 == (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr)) )
			{
                MMIAPISMS_AddRecordIdIntoMsgHandleTableList(sig_ptr->record_id);
			}
            
            if (MMISMS_IsExistNextSMS(edit_content_ptr))
            {
                MMIAPISMS_SaveLongSmsMtNextContent(
                    sig_ptr->dual_sys,
                    dest_addr_ptr,
                    GET_HEAD_MAX_NUM(edit_content_ptr),
                    (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr) + 1),
                    (char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]),
                    (uint32)strlen((char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]))
                    );
            }
            else
            {
#if defined(MMI_SMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)|| ( defined(MAINLCD_SIZE_240X320)&&!defined(MMI_PDA_SUPPORT))
                MMISMS_ClearEditContent();
                MMISMS_ClearDestNum();
#endif
//                MMIAPISMS_AddRecordIdIntoMsgHandleTableList(sig_ptr->record_id);
                MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MSG_LIST); 
            }
            
            return;

        case MMISMS_BT_SAVE_SENDSUCC_SMS:

            save_status = MN_SMS_MO_SR_NOT_REQUEST;
            g_mmisms_global.folder_type = MMISMS_FOLDER_BT;
            folder_type = MMISMS_FOLDER_BT;
            MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);
            
            MMISMS_SaveInfoToOrderByResult(
                sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                    save_status, is_exist_sc_addr, MMISMS_FOLDER_BT,  oper_status,  PNULL, MMISMS_SENDSTATUS_SENDSUCC);

            edit_content_ptr = &g_mmisms_global.edit_content;

            if(1 == (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr)) )
			{
                MMIAPISMS_AddRecordIdIntoMsgHandleTableList(sig_ptr->record_id);
			}

            if (MMISMS_IsExistNextSMS(edit_content_ptr))
            {
                MMIAPISMS_SaveLongMoSmsNextContent(
                    MMISMS_BT_SAVE_SENDSUCC_SMS,
                    sig_ptr->dual_sys,
                    dest_addr_ptr,
                    GET_HEAD_MAX_NUM(edit_content_ptr),
                    (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr) + 1),
                    (char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]),
                    (uint32)strlen((char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]))
                    );
            }
            else
            {
#if defined(MMI_SMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)|| ( defined(MAINLCD_SIZE_240X320)&&!defined(MMI_PDA_SUPPORT))
                MMISMS_ClearEditContent();
                MMISMS_ClearDestNum();
#endif
                MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MSG_LIST); 
            }
            return;

        case MMISMS_BT_SAVE_SENDFAIL_SMS:

            save_status = MN_SMS_MO_TO_BE_SEND;
            g_mmisms_global.folder_type = MMISMS_FOLDER_BT;
            folder_type = MMISMS_FOLDER_BT;
            MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);
            is_exist_sc_addr = TRUE; 
            
            MMISMS_SaveInfoToOrderByResult(
                sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                    save_status, is_exist_sc_addr, MMISMS_FOLDER_BT,  oper_status,  PNULL, MMISMS_SENDSTATUS_SENDFAIL);

            edit_content_ptr = &g_mmisms_global.edit_content;

            if(1 == (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr)) )
			{
                MMIAPISMS_AddRecordIdIntoMsgHandleTableList(sig_ptr->record_id);
			}
            

            if (MMISMS_IsExistNextSMS(edit_content_ptr))
            {
                MMIAPISMS_SaveLongMoSmsNextContent(
                    MMISMS_BT_SAVE_SENDFAIL_SMS,
                    sig_ptr->dual_sys,
                    dest_addr_ptr,
                    GET_HEAD_MAX_NUM(edit_content_ptr),
                    (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr) + 1),
                    (char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]),
                    (uint32)strlen((char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]))
                    );
            }
            else
            {
#if defined(MMI_SMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)|| ( defined(MAINLCD_SIZE_240X320)&&!defined(MMI_PDA_SUPPORT))
                MMISMS_ClearEditContent();
                MMISMS_ClearDestNum();
#endif
                MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MSG_LIST); 
            }
            return;


        case MMISMS_BT_SAVE_NOSEND_SMS:

            save_status = MN_SMS_MO_TO_BE_SEND;
            g_mmisms_global.folder_type = MMISMS_FOLDER_BT;
            folder_type = MMISMS_FOLDER_BT;
            MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);
            
            MMISMS_SaveInfoToOrderByResult(
                sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                    save_status, is_exist_sc_addr, MMISMS_FOLDER_BT,  oper_status,  PNULL, MMISMS_SENDSTATUS_NONE);

            edit_content_ptr = &g_mmisms_global.edit_content;

            if(1 == (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr)) )
			{
                MMIAPISMS_AddRecordIdIntoMsgHandleTableList(sig_ptr->record_id);
			}

            if (MMISMS_IsExistNextSMS(edit_content_ptr))
            {
                MMIAPISMS_SaveLongMoSmsNextContent(
                    MMISMS_BT_SAVE_NOSEND_SMS,
                    sig_ptr->dual_sys,
                    dest_addr_ptr,
                    GET_HEAD_MAX_NUM(edit_content_ptr),
                    (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr) + 1),
                    (char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]),
                    (uint32)strlen((char*)(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0]))
                    );
            }
            else
            {
#if defined(MMI_SMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)|| ( defined(MAINLCD_SIZE_240X320)&&!defined(MMI_PDA_SUPPORT))
                MMISMS_ClearEditContent();
                MMISMS_ClearDestNum();
#endif
                MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MSG_LIST); 
            }
            return;
                        
#endif            
#ifdef MMI_SMS_SYN_SUPPORT
        case MMISMS_SAVE_SYN_SMS:

            if (sms_unread_flag)
            {
                save_status = MN_SMS_MT_TO_BE_READ;
            } 
            else
            {
                save_status = MN_SMS_MT_READED;
            }
            
            MMISMS_SaveInfoToOrderByResult(
                sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                    save_status, is_exist_sc_addr, folder_type,  oper_status,  PNULL, MMISMS_SENDSTATUS_NONE);

            edit_content_ptr = &g_mmisms_global.edit_content;

            if (MMISMS_IsExistNextSMS(edit_content_ptr))
            {
                MMISMS_SaveLongSmsNextContent(sig_ptr->dual_sys, dest_addr_ptr, GET_HEAD_MAX_NUM(edit_content_ptr), (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr) + 1));
            }
            else
            {
                MMISMS_ClearOperInfo();
                MMISMS_ClearEditContent();
                MMISMS_ClearDestNum();

                MMISMS_SendNextSmsRequest();
            }

            return;

            break;
#endif

    #ifdef IKEYBACKUP_SUPPORT
            case MMISMS_RESTORE_SMS:
                {
                    MMIIKB_RESTORE_ITEM_DATA_RES_T item_data_res = {0};
                    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
                    
                    if (PNULL != g_restore_param_in_ptr)
                    {
                        error_code = MMIIKB_STATUS_SUCCESS;
                        item_data_res.op_type = g_restore_param_in_ptr->op_type;
                        item_data_res.type = g_restore_param_in_ptr->type;
                        item_data_res.error_code = error_code;
                        item_data_res.suc_cnt = 1;
                        item_data_res.err_cnt = 0;
                        if (PNULL != g_restore_param_in_ptr)
                        {
                            SCI_FREE(g_restore_param_in_ptr);
                            g_restore_param_in_ptr = PNULL;
                        }
                        MMIAPIIKB_SendRestoreItemDataRes(&item_data_res);      
                    }
                    else
                    {
                        item_data_res.error_code = MMIIKB_STATUS_NO_MEMORY;
                        MMIAPIIKB_SendRestoreItemDataRes(&item_data_res);
                    }                  
                }
                if (MMISMS_MO_SEND_FAIL == g_mo_mt_type)
                {
                    is_exist_sc_addr = TRUE;
                }
                save_status = g_sms_restore_status;
                folder_type = g_sms_restore_folder_type;
                break;
    #endif
            case MMISMS_SAVE_SENDFAIL_SMS:    // save the SMS which send fail
            case MMISMS_SEND_SMS:            // only send SMS operation
            case MMISMS_UPDATE_SENDFAIL_SMS:
                if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
                {
                    folder_type = MMISMS_FOLDER_SECURITY;
                    MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);
                }   
                is_exist_sc_addr = TRUE;
                save_status = MN_SMS_MO_TO_BE_SEND;
                break;
    
            case MMISMS_SAVE_SMS:            // only save SMS operation
            case MMISMS_UPDATE_NOSEND_SMS:
                save_status = MN_SMS_MO_TO_BE_SEND;
                if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
                {
                    folder_type = MMISMS_FOLDER_SECURITY;
                    MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);
                }
                break;
    
            case MMISMS_SAVE_SEND_SMS:
                {//����֮ǰ������ŵ���������Ϣ����
                    BOOLEAN is_send_sms = FALSE;
    
                    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SAVING);
                    
                    if (PNULL == send_info_ptr)
                    {
                        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SAVE_SEND_SMS send_info_ptr = PNULL"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_624_112_2_18_2_53_10_268,(uint8*)"");
                        return;
                    }
                    //SCI_TRACE_LOW:"HandleSaveSMSCnf:MMISMS_SAVE_SEND_SMS "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_627_112_2_18_2_53_10_269,(uint8*)"");
                    is_exist_sc_addr = TRUE;
                    save_status = MN_SMS_MO_TO_BE_SEND;
    #ifndef MMI_SMS_MINI_SUPPORT
                    if (MMISMS_IsSendListEmpty())
                    {
                        MMISMS_SaveInfoToOrderByResult(
                            sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, 
                            save_status, is_exist_sc_addr, folder_type,  oper_status, send_info_ptr, MMISMS_SENDSTATUS_SENDING);                    
                    }
                    else
                    {
                        MMISMS_SaveInfoToOrderByResult(
                            sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, 
                        save_status, is_exist_sc_addr, folder_type,  oper_status, send_info_ptr, MMISMS_SENDSTATUS_WAITING);                    
                    }
    #endif
                    send_info_ptr->backsend_update_data.save_send_record_id[GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) - 1] = sig_ptr->record_id;
                    send_info_ptr->backsend_update_data.save_send_storage = sig_ptr->storage;
                    
                    if(send_info_ptr->send_content.tpdu_head.length>0 
                        && (MMISMS_HEAD_8BIT_IEI == send_info_ptr->send_content.tpdu_head.user_header_arr[MMISMS_HEAD_IEI_POS]))
                    {//������
                        if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content))  //�ǳ�����Ϣ�������ǳ�����Ϣ�����һ�����򱣴���һ��
                        {
                            save_result = MMISMS_SaveNextMessage(sig_ptr->dual_sys, send_info_ptr->send_content.dest_info.dest_list.dest_addr[send_info_ptr->send_content.dest_info.dest_list.dest_addr_id], GET_HEAD_MAX_NUM(&send_info_ptr->send_content), (uint8)(GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1), send_info_ptr);
                            if (MMISMS_NO_ERR != save_result)
                            {
                                send_info_ptr->send_content.is_need_save = FALSE;
                                if (MMISMS_SENDSAVE_NONE == send_info_ptr->backsend_update_data.save_send_result)
                                {
                                    send_info_ptr->backsend_update_data.save_send_result = MMISMS_SENDSAVE_SAVEFAIL;
                                }                            
                                is_send_sms = TRUE;
                            }
                        }
                        else
                        {
                            is_send_sms = TRUE;
                            if (MMISMS_SENDSAVE_NONE == send_info_ptr->backsend_update_data.save_send_result)
                            {
                                send_info_ptr->backsend_update_data.save_send_result = MMISMS_SENDSAVE_SAVESUCC;
                            }
                        }
                        SCI_TRACE_LOW("mmisms1:HandleSaveSMSCnf long sms");
                    }
                    else
                    {//��ͨ����
                        is_send_sms = TRUE;
                        if (MMISMS_SENDSAVE_NONE == send_info_ptr->backsend_update_data.save_send_result)
                        {
                            send_info_ptr->backsend_update_data.save_send_result = MMISMS_SENDSAVE_SAVESUCC;
                        }
                        SCI_TRACE_LOW("mmisms1:HandleSaveSMSCnf normal sms");
                    }
                    if (is_send_sms)
                    {   
                        send_info_ptr->cur_send_data.cur_send_record_id = 0;
                        send_info_ptr->cur_send_data.cur_send_storage = MN_SMS_NO_STORED;
                        if(PNULL != s_mmisms_sendlist.sendlist_head_ptr)
                        {
                           SCI_TRACE_LOW("mmisms1:HandleSaveSMSCnf backsend_status = %d!",s_mmisms_sendlist.sendlist_head_ptr->backsend_status);
                           SCI_TRACE_LOW("mmisms1:HandleSaveSMSCnf sendlist_send_num = %d!",s_mmisms_sendlist.send_num);
                        }
                        if (MMISMS_IsSendListEmpty()||MMISMS_IsAllSendListSaving())
                        {
                            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                            MMISMS_ActiveSendList();
                        }
                        else
                        {
                            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_WAITING;
                        }                   
                    }
                }
                break;
                
            case MMISMS_SEND_SAVE_SMS:        // send and save SMS operation
                { //���ͱ��洦��(���͵�ͬʱ����)               
    //============== MET MEX  start ===================
    #ifdef MET_MEX_SUPPORT
                    if(MMIMEX_PhsSaveSmsCB(sig_ptr,send_info_ptr))
                    {
                        break;
                    }
    #endif // MET_MEX_SUPPORT
    //============== MET MEX  end ===================
    
                    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
                    
                    if (PNULL == send_info_ptr)
                    {
                        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SEND_SAVE_SMS send_info_ptr = PNULL"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_710_112_2_18_2_53_10_270,(uint8*)"");
                        return;
                    }
                    //SCI_TRACE_LOW:"HandleSaveSMSCnf:resend_count is %d,send_result is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_713_112_2_18_2_53_10_271,(uint8*)"dd", send_info_ptr->resend_count,send_info_ptr->send_save_result.send_result);
                    
                    if((MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type) &&
                        (MN_SMS_STORAGE_ME == sig_ptr->storage))
                    {
                        folder_type = MMISMS_FOLDER_SECURITY;
                        MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);
                    }
                    
                    if(  ((MMISMS_SEND_FAIL == send_info_ptr->send_save_result.send_result )
                        && (send_info_ptr->resend_count < MMISMS_RESEND_MAX_COUNT) /*lint !e685 !e568*/
                        && send_info_ptr->send_content.is_resend) || 
                        (
                        MMISMS_SEND_FAIL == send_info_ptr->send_save_result.send_result 
                        && MMISMS_WAIT_FOR_WRITE_CNF_TO_RESEND == send_info_ptr->send_save_result.send_status  
                        && send_info_ptr->is_sendfail_retry))
                        
                    {
                        SCI_TRACE_LOW("mmisms sendfail resend update sms");
                        //����ʧ��ͬʱ�ط���������
                        err_code = MNSMS_UpdateSmsStatusEx(
                            sig_ptr->dual_sys, 
                            sig_ptr->storage, 
                            sig_ptr->record_id, 
                            MN_SMS_FREE_SPACE);
                        
                        //SCI_TRACE_LOW:"MMISMS: UpdateMsgReq err_code = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_733_112_2_18_2_53_10_272,(uint8*)"d", err_code);
                    }
                    else if ( MMISMS_WAIT_RESULT != send_info_ptr->send_save_result.send_result )  //�Ѿ��յ�send_cnf
                    {//�Ѿ��յ�cnf�����ͳɹ�����ʧ�ܺ��ʹ���û�е� 
                        send_info_ptr->send_save_result.save_record_id = sig_ptr->record_id;
                        send_info_ptr->send_save_result.save_storage = sig_ptr->storage;
                        send_info_ptr->send_save_result.dual_sys = sig_ptr->dual_sys;
                        
                        send_info_ptr->send_save_result.is_finished = TRUE;
                        is_exist_sc_addr = TRUE;
                        save_status = ( MMISMS_SEND_SUCC == send_info_ptr->send_save_result.send_result ) ? MN_SMS_MO_SR_NOT_REQUEST : MN_SMS_MO_TO_BE_SEND;
                        if (MMISMS_SEND_SUCC == send_info_ptr->send_save_result.send_result)
                        {
                            MMISMS_SaveInfoToOrderByResult(
                                sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                                save_status, is_exist_sc_addr,  folder_type, oper_status,  send_info_ptr, MMISMS_SENDSTATUS_SENDSUCC);
                            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDSUCC;
                        }
                        else
                        {
                            MMISMS_SaveInfoToOrderByResult(
                                sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                                save_status, is_exist_sc_addr,  folder_type, oper_status,  send_info_ptr, MMISMS_SENDSTATUS_SENDFAIL);
                            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;
                        }   
                        
                        if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content) || MMISMS_CheckNextDestAddr())
                        {//������滹�з���״̬����                        
                            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                        }
                        
                        if (!MMISMS_GetDeliverCancel())
                        {
                            MMISMS_HandleSendCnfWhenWriteCnf(send_info_ptr->send_save_result.send_status);
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"MMISMS: HandleSaveSMSCnf Deliver Cancel!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_770_112_2_18_2_53_10_273,(uint8*)"");
    #ifndef MMI_DUALMODE_ENABLE 
                            if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content))  //�ǳ�����Ϣ�������ǳ�����Ϣ�����һ�����򱣴���һ��
                            {
                                MMISMS_SetOperStatus(MMISMS_SAVE_SENDFAIL_SMS);
                                send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SENDFAIL_SMS;
                                save_result = MMISMS_SaveNextMessage(sig_ptr->dual_sys, send_info_ptr->send_content.dest_info.dest_list.dest_addr[send_info_ptr->send_content.dest_info.dest_list.dest_addr_id], GET_HEAD_MAX_NUM(&send_info_ptr->send_content), (uint8)(GET_CURRENT_SEQ_NUM(&send_info_ptr->send_content) + 1), send_info_ptr);
                                
                                if (MMISMS_NO_ERR != save_result)
                                {
                                    MMIPUB_OpenAlertSuccessWin(TXT_SMS_CANCEL_SENDING_SUCCESS);
                                    MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                                    MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
    #ifdef MMIMMS_SMS_IN_1_SUPPORT 
                                    //MMIAPIMMS_CloseEditWin();
    #else
                                    //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
    #endif
                                    MMISMS_ClearOperInfo();
                                    MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                                    MMISMS_DeleteSendList();//ȡ�����͵ĺ�ɾ������
                                }                            
                                return;
                            }
                            MMIPUB_OpenAlertSuccessWin(TXT_SMS_CANCEL_SENDING_SUCCESS);
    #ifdef MMIMMS_SMS_IN_1_SUPPORT 
                            //MMIAPIMMS_CloseEditWin();
    #else
                            //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
    #endif
    #endif
                            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                            MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                            MMISMS_ClearOperInfo();
                            MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                            MMISMS_DeleteSendList();//ȡ�����͵ĺ�ɾ������
                        }                    
                    }
                    else //��û���յ�send_cnf
                    {
                        send_info_ptr->send_save_result.is_finished = FALSE;
                        return;
                    }
                }            
                break;
            case MMISMS_SAVE_VCARD_VCAL_SMS:
                save_status = MN_SMS_MT_TO_BE_READ;
                break;
            
            case MMISMS_SAVE_CLASS0_SMS:
                is_exist_sc_addr = TRUE;
                save_status = MN_SMS_MT_READED;
                break;
    
            case MMISMS_MOVE_SMS_TO_SECURITY:
                save_status = g_mmisms_global.read_msg.status;
                folder_type = MMISMS_FOLDER_SECURITY;
                //MMISMS_SetMoveSingleFlag(FALSE);            
                if(MMISMS_BOX_SENDFAIL == g_mmisms_global.security_box_type)
                {
                    is_exist_sc_addr = TRUE;
                }
                MMISMS_SetNVFolderTypeFlag(sig_ptr->record_id,folder_type);
                MMISMS_NV_SetSmsLockState(sig_ptr->record_id,g_mmisms_global.save_info.is_locked);
                break;
    
            case MMISMS_MOVE_SMS_FROM_SECURITY:
            case MMISMS_MOVE_SMS_TO_ME:
            case MMISMS_MOVE_SMS_TO_SIM:    
            case MMISMS_COPY_SMS_TO_ME:
            case MMISMS_COPY_SMS_TO_SIM:
                save_status = g_mmisms_global.read_msg.status;
                //MMISMS_SetMoveSingleFlag(FALSE);            
                if(MMISMS_BOX_SENDFAIL == g_mmisms_global.security_box_type)
                {
                    is_exist_sc_addr = TRUE;
                }
                if(PNULL != g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index])
                {
                    if((MMISMS_MOVE_SMS_TO_SIM == oper_status) &&
                       (MN_SMS_STORAGE_ME == g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]->flag.storage))
                    {
                        MMISMS_SetNVFolderTypeFlag(g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]->record_id,folder_type);
                    }
                }
    
                MMISMS_NV_SetSmsLockState(sig_ptr->record_id,g_mmisms_global.save_info.is_locked);
                break;
                
            default: 
                //SCI_TRACE_LOW:"MMISMS: HandleSaveSMSCnf oper_status(%d) is error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_861_112_2_18_2_53_10_274,(uint8*)"d", oper_status);
                //MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);//״̬��ͬ��ʱ�򲻴洢��ֱ��ɾ��
                send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
                if (PNULL != send_info_ptr && MMISMS_SENDSTATUS_SENDING == send_info_ptr->backsend_status)
                {
                    send_info_ptr->send_save_result.save_record_id = sig_ptr->record_id;
                    send_info_ptr->send_save_result.save_storage = sig_ptr->storage;
                    send_info_ptr->send_save_result.dual_sys = sig_ptr->dual_sys;
                    
                    send_info_ptr->send_save_result.is_finished = TRUE;
                    is_exist_sc_addr = TRUE;
                    save_status = ( MMISMS_SEND_SUCC == send_info_ptr->send_save_result.send_result ) ? MN_SMS_MO_SR_NOT_REQUEST : MN_SMS_MO_TO_BE_SEND;
                    if (MMISMS_SEND_SUCC == send_info_ptr->send_save_result.send_result)
                    {
                        MMISMS_SaveInfoToOrderByResult(
                            sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                            save_status, is_exist_sc_addr,  folder_type, oper_status,  send_info_ptr, MMISMS_SENDSTATUS_SENDSUCC);
                        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDSUCC;
                    }
                    else
                    {
                        MMISMS_SaveInfoToOrderByResult(
                            sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                            save_status, is_exist_sc_addr,  folder_type, oper_status,  send_info_ptr, MMISMS_SENDSTATUS_SENDFAIL);
                        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;
                    }   
                    
                    if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content) || MMISMS_CheckNextDestAddr())
                    {//������滹�з���״̬����                        
                        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                    }
                    
                    MMISMS_HandleSendCnfWhenWriteCnf(MMISMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT);
                }
    
                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                return;
            }
            
            if ( !(oper_status == MMISMS_SEND_SAVE_SMS || oper_status == MMISMS_SAVE_SEND_SMS) &&
                (MN_SMS_FREE_SPACE != save_status)) //��ǰ����Ϊ���Ͳ�����
            {       
    //             if (PNULL == send_info_ptr)
    //             {
    //                 SCI_TRACE_LOW( "MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_UPDATE_SENDFAIL_SMS send_info_ptr = PNULL");
    //                     
    //                 if(!MMISMS_IsMoveOperation(oper_status)
    //                     && !MMISMS_IsCopyOperation(oper_status))
    //                 {
    //                     return;
    //                 }
    //             }
    
                if (MMISMS_SAVE_SENDFAIL_SMS ==  oper_status)
                {                            
                    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
                    if(PNULL != send_info_ptr)
                    {
                        MMISMS_SaveInfoToOrderByResult(
                            sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                            save_status, is_exist_sc_addr, folder_type,  oper_status,  send_info_ptr, MMISMS_SENDSTATUS_SENDFAIL);
                        send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;
                        if (MMISMS_CheckNextDestAddr())
                        {
                            send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                        }
                    }
                }
                else
                {
                    MMISMS_SaveInfoToOrderByResult(
                    sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                    save_status, is_exist_sc_addr, folder_type,  oper_status,  PNULL, MMISMS_SENDSTATUS_NONE);
                }            
        
                if(MMISMS_UPDATE_NOSEND_SMS == oper_status
                    || MMISMS_UPDATE_SENDFAIL_SMS == oper_status)
                {
                    edit_content_ptr = &g_mmisms_global.edit_content;
                    update_info_ptr = &(g_mmisms_global.update_info);
                    if ((update_info_ptr->current_num < update_info_ptr->update_num - 1) &&
                        (g_mmisms_global.save_info.is_change_save_pos))
                    {                    
                        MMISMS_SaveNextMessage( 
                            sig_ptr->dual_sys,
                            edit_content_ptr->dest_info.update_dest_addr, 
                            GET_HEAD_MAX_NUM(edit_content_ptr),
                            (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr) + 1),
                            send_info_ptr
                            );
                        update_info_ptr->current_num++;
                    }
                    else if ((update_info_ptr->current_num == update_info_ptr->update_num)
                        && (update_info_ptr->total_num > update_info_ptr->update_num))
                    {
                        // have finish update and need to delete the other SMS
                        if (!MMISMS_DeletePointedSMSById(update_info_ptr->order_id_arr[update_info_ptr->current_num]))
                        {
                            MMISMS_DisplayUpdatePrompt(TRUE);
                        }
                    }
                    else 
                    {
                        g_mmisms_global.save_info.is_change_save_pos = FALSE;
                        MMISMS_ClearSaveInfo();
                        MMISMS_DisplayUpdatePrompt(TRUE);
                        
                        MMISMS_CloseEditWin();
                        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);                
                    }      
                    return;
                }
            }
        }
        else
        {
    //============== MET MEX  start ===================
    #ifdef MET_MEX_SUPPORT
            if (MMIMEX_GetSaveSmsBox() != MMISMS_NO_OPER)
            {
                MMIMEX_SaveSmsCB(NULL,0,sig_ptr->cause);
                //process started by mex application, should not go on.
                return;
            }
    #endif // MET_MEX_SUPPORT
    //============== MET MEX  end ===================
           if (oper_status==MMISMS_SAVE_SMS)
           {
                if(MMISMSAPP_HandleSaveCnf(FALSE))
                {
                    return;
                }
           }
    
            send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
                    
            if (PNULL == send_info_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSaveSMSCnf MMISMS_SEND_SAVE_SMS send_info_ptr = PNULL"
                MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_990_112_2_18_2_53_11_275,(uint8*)"");
                return;
            }
            
            if (MMISMS_SEND_SAVE_SMS == oper_status )
            {
                if (0 == strlen((char *)send_info_ptr->send_content.dest_info.update_dest_addr))
                {
                    MMISMS_SaveUpdateAddr(FALSE);
                }
                
                MMISMS_HandleWriteError();
            }
            else if (MMISMS_SAVE_SEND_SMS == oper_status)
            {//�������ɹ��������ֱ�ӿ�����������          
                send_info_ptr->send_content.is_need_save = FALSE;
                MMISMS_AppSendSms(send_info_ptr->send_content.dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr);
            }
            else if (MMISMS_SENDSTATUS_SENDING == send_info_ptr->backsend_status)
            {
                send_info_ptr->send_save_result.save_record_id = sig_ptr->record_id;
                send_info_ptr->send_save_result.save_storage = sig_ptr->storage;
                send_info_ptr->send_save_result.dual_sys = sig_ptr->dual_sys;
                
                send_info_ptr->send_save_result.is_finished = TRUE;
                is_exist_sc_addr = TRUE;
                save_status = MN_SMS_MO_TO_BE_SEND;

				//if (MN_SMS_ME_MEM_FULL != sig_ptr->cause) //fixed on bug124840
				//{
					MMISMS_SaveInfoToOrderByResult(
						sig_ptr->dual_sys,  sig_ptr->storage, sig_ptr->record_id, 
                    save_status, is_exist_sc_addr,  folder_type, oper_status,  send_info_ptr, MMISMS_SENDSTATUS_SENDFAIL);
				//}
                
                send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDFAIL;            
                if (MMISMS_IsExistNextSMS(&send_info_ptr->send_content) || MMISMS_CheckNextDestAddr())
                {//������滹�з���״̬����                        
                    send_info_ptr->backsend_status = MMISMS_SENDSTATUS_SENDING;
                }
                
                MMISMS_HandleSendCnfWhenWriteCnf(MMISMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT);
            }
        }
        
        if ((oper_status == MMISMS_SAVE_SENDFAIL_SMS) ||(oper_status == MMISMS_SAVE_SMS ) ||
             (oper_status == MMISMS_SAVE_CLASS0_SMS))
        {
            if (MMISMS_SAVE_SENDFAIL_SMS == oper_status)
            {
                send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
            } 
            else
            {
                send_info_ptr = PNULL;
            }
                    
            if (PNULL != send_info_ptr)
            {
                edit_content_ptr = &send_info_ptr->send_content;
            }
            else 
            {
                edit_content_ptr = &g_mmisms_global.edit_content;
            }
            if(edit_content_ptr->tpdu_head.length>0 
                && (MMISMS_HEAD_8BIT_IEI == edit_content_ptr->tpdu_head.user_header_arr[MMISMS_HEAD_IEI_POS]))
            {
                // for long SMS, save the next
                if (MMISMS_IsExistNextSMS(edit_content_ptr))  //�ǳ�����Ϣ�������ǳ�����Ϣ�����һ�����򱣴���һ��
                {
                    if (MMISMS_SAVE_SENDFAIL_SMS == oper_status)
                    {
                        id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
                        dest_addr_ptr = edit_content_ptr->dest_info.dest_list.dest_addr[id];
                    }
    
                    save_result = MMISMS_SaveNextMessage(sig_ptr->dual_sys, dest_addr_ptr, GET_HEAD_MAX_NUM(edit_content_ptr), (uint8)(GET_CURRENT_SEQ_NUM(edit_content_ptr) + 1), send_info_ptr);
                    
                    if (MMISMS_NO_ERR != save_result)
                    {
                        if(MMISMS_NO_SPACE == save_result)
                        {
                            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
                        }
                        else
                        {
                            // prompt the error, and stop the save procedure.
                            MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
                            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                        }
                    }
                }
                else  //�����ŵ����һ����������һ��
                {
    //============== MET MEX  start ===================
    #ifdef MET_MEX_SUPPORT
                    if (MMIMEX_GetSaveSmsBox() != MMISMS_NO_OPER)
                    {
                        MMIMEX_SaveSmsCB(NULL, 0, sig_ptr->cause);
                        //process started by mex application, should not go on.
                        return;
                    }
    #endif // MET_MEX_SUPPORT
    //============== MET MEX  end ===================
                    if (oper_status==MMISMS_SAVE_SENDFAIL_SMS && PNULL != send_info_ptr)
                    {
    #ifndef MMI_DUALMODE_ENABLE 
                        if(MMISMS_SEND_CANCEL == send_info_ptr->send_save_result.send_result)
                        {
                            MMISMS_InitSendSaveResult(send_info_ptr);
                            MMIPUB_OpenAlertSuccessWin(TXT_SMS_CANCEL_SENDING_SUCCESS);
                            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                            MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                            MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                            MMISMS_CloseEditWin();
                        }
                        // to send the message to next destination
                        else if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
    #else
                        // to send the message to next destination
                        if (MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
    #endif
                        {
                            MMISMS_InitSendSaveResult(send_info_ptr);
                            MMISMS_AppSendSms(sig_ptr->dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr);
                            //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                            //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                        }
                        else if((MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type || MMISMS_BOX_NOSEND == send_info_ptr->cur_box_type)
                        //    &&(g_mmisms_global.update_info.current_num < g_mmisms_global.update_info.total_num) fixed bug204097
                               )//ɾ��ԭ��������Ϣ
                        {
                            send_info_ptr->mosend_update_data.is_mosend_update = TRUE;
                            send_info_ptr->mosend_update_data.mosend_update_info.current_num = 0;
                            if (MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type)
                            {
                                send_info_ptr->mosend_update_data.mosend_send_oper = MMISMS_UPDATE_SENDFAIL_SMS;
                            } 
                            else
                            {
                                send_info_ptr->mosend_update_data.mosend_send_oper = MMISMS_UPDATE_NOSEND_SMS;
                            }
    #if 0
    //no use
                            ret_val = MMISMS_AppUpdateMoSms(send_info_ptr->send_save_result.dual_sys, MMISMS_DELETE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
    #else
                            MMISMS_AppUpdateMoSms(send_info_ptr->send_save_result.dual_sys, send_info_ptr->mosend_update_data.mosend_send_oper, MN_SMS_FREE_SPACE, send_info_ptr);
    #endif
                            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                        }
                        else
                        {
                            // ��ʾ����ʧ��/����ɹ�
                            if (MMISMS_SENDSAVE_SAVESUCC == send_info_ptr->backsend_update_data.save_send_result)
                            {//��̨����ɾ����������ţ����ڳ����ŷ���ʧ��
                                MN_SMS_RECORD_ID_T update_record_id = send_info_ptr->backsend_update_data.save_send_record_id[0];
                                MMISMS_ORDER_ID_T      order_id = PNULL;
                                BOOLEAN                ret_value = FALSE;
                                
                                ret_value = MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, send_info_ptr->send_content.dual_sys, send_info_ptr->backsend_update_data.save_send_storage, update_record_id);
                                
                                if (ret_value && PNULL != order_id)
                                {                                
                                    send_info_ptr->backsend_update_data.is_backsend_update = TRUE;
                                    send_info_ptr->backsend_update_data.backsend_send_oper = MMISMS_UPDATE_BACKSEND_SMS;
                                    send_info_ptr->backsend_update_data.backsend_update_info.current_num = 0;
                                    MMISMS_SetBackSendUpdateInfo(order_id, send_info_ptr);  
    #if 0
    //no use
                                    ret_val = MMISMS_AppUpdateMoSms(send_info_ptr->send_content.dual_sys, MMISMS_UPDATE_BACKSEND_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
    #else
                                    MMISMS_AppUpdateMoSms(send_info_ptr->send_content.dual_sys, MMISMS_UPDATE_BACKSEND_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
    #endif
                                    g_mmisms_global.is_wait_update = TRUE;
                                }
                                MMISMS_ClearSaveInfo();
                                send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
                                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                                MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                                MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                                //MMK_CloseWin(MMISMS_EDIT_WIN_ID);
    #ifdef MMIMMS_SMS_IN_1_SUPPORT 
                                //MMIAPIMMS_CloseEditWin();
    #else
                                //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
    #endif
                                MMISMS_SetDeliverResendFlag(FALSE);
                                //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleSaveSMSCnf %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1127_112_2_18_2_53_11_276,(uint8*)"d", __LINE__);
                            }
                            else
                            {
                                MMISMS_ClearSaveInfo();
                                send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
                                MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                                MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                                MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                                //MMK_CloseWin(MMISMS_EDIT_WIN_ID);
    #ifdef MMIMMS_SMS_IN_1_SUPPORT 
                                //MMIAPIMMS_CloseEditWin();
    #else
                                //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
    #endif
                                MMISMS_SetDeliverResendFlag(FALSE);
                                MMISMS_ShowSendResult((uint8)send_result, fail_id);
                                MMISMS_DeleteSendList();
                                MMISMS_ClearOperInfo();
                                //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleSaveSMSCnf %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1146_112_2_18_2_53_11_277,(uint8*)"d", __LINE__);
                            }                        
                        }
                       
                    }
                    else if((MMISMS_BOX_SENDFAIL == g_mmisms_global.operation.cur_type || MMISMS_BOX_NOSEND == g_mmisms_global.operation.cur_type) &&
                            (g_mmisms_global.update_info.current_num < g_mmisms_global.update_info.total_num))//ɾ��ԭ��������Ϣ
                    {
    #if 0
    //no use
                        ret_val = MMISMS_AppUpdateMoSms(g_mmisms_global.cur_save_dual_sys, MMISMS_DELETE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
    #else
                        if(oper_status != MMISMS_SAVE_SMS)
                        { 
                            MMISMS_AppUpdateMoSms(g_mmisms_global.cur_save_dual_sys, MMISMS_DELETE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
                        }
    #endif
                        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                    }                 
                    else  //�������
                    {
                        // ��ʾ����ɹ�
                        if (!MMISMSAPP_HandleSaveCnf(TRUE))
                        {
                            MMISMS_ClearSaveInfo();
                            MMISMS_DisplayUpdatePrompt(is_success);
                            MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                            MMISMS_SetDeliverResendFlag(FALSE);
                            //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleSaveSMSCnf %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1163_112_2_18_2_53_11_278,(uint8*)"d", __LINE__);
                            MMISMS_ClearOperInfo();
                        }
                    }
                }
            }
            else   //��ͨ����Ϣ
            {
    //============== MET MEX  start ===================
    #ifdef MET_MEX_SUPPORT
                if (MMIMEX_GetSaveSmsBox() != MMISMS_NO_OPER)
                {
                    MMIMEX_SaveSmsCB(NULL, 0,sig_ptr->cause);
                    //process started by mex application, should not go on.
    
                    return;
                }
    #endif // MET_MEX_SUPPORT
    //============== MET MEX  end ===================
                if ((oper_status==MMISMS_SAVE_SENDFAIL_SMS))
                {
                    // to send the message to next destination
                    if ( MMISMS_CheckNextDestAddr() && MMISMS_MoveDestAddrId())
                    {
                        if (PNULL != send_info_ptr)
                        {
                            MMISMS_InitSendSaveResult(send_info_ptr);
                            MMISMS_AppSendSms(sig_ptr->dual_sys, send_info_ptr->send_content.is_need_save, send_info_ptr);
                            //MMISMS_UpdateSmsSendindNumber(send_info_ptr);
                            //MMK_SendMsg(MMISMS_SEND_ANIM_WIN_ID,MMISMS_MSG_UPDATE_NUMBER,PNULL);
                        }
                    }
                    else if(PNULL != send_info_ptr && ((MMISMS_BOX_SENDFAIL == send_info_ptr->cur_box_type || MMISMS_BOX_NOSEND == send_info_ptr->cur_box_type)) &&
                            (g_mmisms_global.update_info.current_num < g_mmisms_global.update_info.total_num))//ɾ��ԭ��������Ϣ
                    {
    #if 0
    // no use
                        ret_val = MMISMS_AppUpdateMoSms(send_info_ptr->send_save_result.dual_sys, MMISMS_DELETE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
    #else
                        MMISMS_AppUpdateMoSms(send_info_ptr->send_save_result.dual_sys, MMISMS_DELETE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
    #endif
                        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                    }
                    else
                    {
                        // ��ʾ����ʧ��/����ɹ�
                        MMISMS_ClearSaveInfo();
                        send_result = MMISMS_GetResultofSending(fail_id, &dest_count);
                        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                        MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
                        MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
                        //MMK_CloseWin(MMISMS_EDIT_WIN_ID);
    #ifdef MMIMMS_SMS_IN_1_SUPPORT 
                       // MMIAPIMMS_CloseEditWin();
    #else
                        //MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
    #endif
                        MMK_CloseWin(MMISMS_SENDNUMOPTION_WIN_ID);
                        MMISMS_SetDeliverResendFlag(FALSE);
                        MMISMS_ShowSendResult((uint8)send_result, fail_id);
                        MMISMS_DeleteSendList();
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleSaveSMSCnf %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1217_112_2_18_2_53_11_279,(uint8*)"d", __LINE__);
                        MMISMS_ClearOperInfo();
                    }
                }
                else if((MMISMS_BOX_SENDFAIL == g_mmisms_global.operation.cur_type || MMISMS_BOX_NOSEND == g_mmisms_global.operation.cur_type) &&
                        (g_mmisms_global.update_info.current_num < g_mmisms_global.update_info.total_num))//ɾ��ԭ��������Ϣ
                {
    #if 0
    // no use
                    ret_val = MMISMS_AppUpdateMoSms(g_mmisms_global.cur_save_dual_sys, MMISMS_DELETE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
    #else
                    if(oper_status != MMISMS_SAVE_SMS)
                    {
                        MMISMS_AppUpdateMoSms(g_mmisms_global.cur_save_dual_sys, MMISMS_DELETE_SENDFAIL_SMS, MN_SMS_FREE_SPACE, send_info_ptr);
                    }
    #endif
                    MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                }            
                else  //�������
                {
                    if (!MMISMSAPP_HandleSaveCnf(TRUE))
                    {
                        // ��ʾ����ɹ�
                        MMISMS_ClearSaveInfo();
                        MMISMS_DisplayUpdatePrompt(is_success);
                        MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
                        MMISMS_SetDeliverResendFlag(FALSE);
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleSaveSMSCnf %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1233_112_2_18_2_53_11_280,(uint8*)"d", __LINE__);
                        MMISMS_ClearOperInfo();
                    }
                }        
            }
        }
    #ifdef MMI_SMS_MOVE_SUPPORT    
        //�ƶ���Ϣ�������ɾ��ԭ��Ϣ
        else if((MMISMS_IsMoveOperation(oper_status)))
        {
            //���泤���ŵ���һ��
            if(MMISMS_IsExistNextCopySMS())
            {
                save_result = MMISMS_CopyNextMessage(
                                                     Get_Read_Head_Max_Num(),
                                                     (uint8)(Get_Read_Current_Seq_Num() + 1));
            }
            else if(MMISMS_GetMoveSingleFlag())
            {
                MMISMS_DeleteReqToMN(g_mmisms_global.operation.cur_type, MMISMS_GetCurOperationOrderId(), MMISMS_GetCurrentOperStatus());  
            }
            else
            {
                ret_val = MMISMS_FINISH_OPER_ERR; 
                break;
            }
        }
    #endif  
    #ifdef MMI_SMS_COPY_SUPPORT 
        else if(MMISMS_IsCopyOperation(oper_status))
        {
            //���泤���ŵ���һ��
            if(MMISMS_IsExistNextCopySMS())
            {
                save_result = MMISMS_CopyNextMessage(
                                                     Get_Read_Head_Max_Num(),
                                                     (uint8)(Get_Read_Current_Seq_Num() + 1));
            }
            //������һ��������������һ��
            else if(MMISMS_GetMoveAllFlag())
            {
        //            MMISMS_SetMoveSingleFlag(FALSE);
    //             g_mmisms_global.operation.cur_order_index = 0;
    //             SCI_MEMSET(g_mmisms_global.operation.cur_order_id_arr, 0, 
    //                        sizeof(g_mmisms_global.operation.cur_order_id_arr)); 
        //            MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_COPY_NEXT);   
                      MMISMSAPP_HandleCopyCnf(MMISMS_NO_ERR);
            }
            //ȡ������ֹ����
            else
            {
                      ret_val = MMISMS_FINISH_OPER_ERR; 
    				  break;         
    		}        
        }
    	#endif    
        else if(oper_status == MMISMS_SAVE_VCARD_VCAL_SMS)
        {
            SCI_TRACE_LOW("MMISMS: MMISMS_HandleSaveSMSCn receive new sms");
            MMIDEFAULT_TurnOnBackLight();
            MMISMS_ShowNewMsgPrompt(sig_ptr->dual_sys, TRUE);        
            if(0 == g_mmisms_global.save_info.max_num || vcard_vcal_cur_save_index == g_mmisms_global.save_info.max_num)
            { 
    			MMISMS_ClearSaveInfo();
                MMISMS_ClearOperInfo();
                vcard_vcal_cur_save_index = 0;
            }
        }
    }while(0);
    if(ret_val != MMISMS_NO_ERR)
    {
        SCI_TRACE_LOW("mmisms save oper_error = %d,oper_status = %d",ret_val,oper_status);
        #ifdef MMI_SMS_COPY_SUPPORT 
        if(MMISMS_IsCopyOperation(oper_status))
        {
            MMISMSAPP_HandleCopyCnf(ret_val);
        }
        #endif
        #ifdef MMI_SMS_MOVE_SUPPORT 
        if(MMISMS_IsMoveOperation(oper_status))
        {
            MMISMSAPP_HandleMoveCnf(ret_val);
        }
        #endif
    }
}

/*****************************************************************************/
//     Description : to save the SMS , called by window
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppSaveSms(MN_DUAL_SYS_E dual_sys, BOOLEAN is_send_save)
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
    uint8 total_num = 1;
    uint8 id = 0;
    uint8* dest_addr_ptr= PNULL;
    MMISMS_SEND_T           *send_info_ptr = PNULL;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    
    if (is_send_save)
    {
        send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SAVING);
        if (PNULL == send_info_ptr)
        {
            //SCI_TRACE_LOW:"MMISMS:MMISMS_AppSaveSms send_info_ptr = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1302_112_2_18_2_53_11_281,(uint8*)"");
            return ret_val;
        }
        edit_content_ptr = &send_info_ptr->send_content;
    }
    else
    {
        SCI_MEMSET(&g_mmisms_global.edit_content.tpdu_head, 0, sizeof(MN_SMS_USER_DATA_HEAD_T));
        edit_content_ptr = &g_mmisms_global.edit_content;
    }
    
    SCI_MEMSET(&g_mmisms_global.save_info , 0, sizeof(MMISMS_SAVE_INFO_T));
    g_mmisms_global.save_info.dual_sys = dual_sys;
    
    if (is_send_save)
    {
        if (PNULL != send_info_ptr)
        {
            total_num = MMISMS_SplitLongMsg(&edit_content_ptr->send_message, send_info_ptr->is_head_has_port);
        }
    }
    else
    {
        total_num = MMISMS_SplitLongMsg(&edit_content_ptr->send_message, FALSE);
    }    

    MMISMS_InitUserDataHead(total_num, 1, &edit_content_ptr->tpdu_head, send_info_ptr);
    SCI_TRACE_LOW("mmisms1:MMISMS_AppSaveSms is_send_save = %d",is_send_save);
    
    // set operation status
    if (is_send_save)
    {
        if (PNULL != send_info_ptr)
        {
            send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SEND_SMS;
        }
    }
    else
    {
        MMISMS_SetOperStatus(MMISMS_SAVE_SMS);
    }    
    
//============== MET MEX  start ===================
#ifdef MET_MEX_SUPPORT
    MMIMEX_PhsSetSmsOperStatus();      
#endif // MET_MEX_SUPPORT
//============== MET MEX  end ===================
    id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
    dest_addr_ptr = edit_content_ptr->dest_info.dest_list.dest_addr[id];
    if(PNULL != s_mmisms_sendlist.sendlist_head_ptr)
    {
        SCI_TRACE_LOW("mmisms1:MMISMS_AppSaveSms backsend_status = %d!",s_mmisms_sendlist.sendlist_head_ptr->backsend_status);
    }
    ret_val = MMISMS_SaveNextMessage(dual_sys, dest_addr_ptr, total_num, 1, send_info_ptr);

    if(MMISMS_NO_SPACE == ret_val && !is_send_save)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
    }
    
    MMISMS_CLEAR_OPER(ret_val);
    
    return (ret_val);
}

/*****************************************************************************/
//  Description : to save the SMS , called by window
//  Parameter: [In] send_info_ptr
//             [In] is_send_save
//  Author: qi.liu1
//  Note:�����һ�η����ź�ײ�Write cnf ��Ϣ��û�������ַ��𷢶��ţ�����send_info_ptrȡ�Ĳ��Ե�����
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppSaveSmsEx(MMISMS_SEND_T  *send_info_ptr, BOOLEAN is_send_save)
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
    uint8 total_num = 1;
    uint8 id = 0;
    uint8* dest_addr_ptr= PNULL;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;

    if(send_info_ptr == PNULL)
    {
        SCI_TRACE_LOW("MMISMS:MMISMS_AppSaveSmsEx send_info_ptr = PNULL!");
        return ret_val;
    }

    if (is_send_save)
    {
        //send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SAVING);
        SCI_TRACE_LOW("MMISMS:MMISMS_AppSaveSmsEx record_id = %d",send_info_ptr->cur_send_data.cur_send_record_id);
        edit_content_ptr = &send_info_ptr->send_content;
    }
    else
    {
        SCI_MEMSET(&g_mmisms_global.edit_content.tpdu_head, 0, sizeof(MN_SMS_USER_DATA_HEAD_T));
        edit_content_ptr = &g_mmisms_global.edit_content;
    }

    SCI_MEMSET(&g_mmisms_global.save_info , 0, sizeof(MMISMS_SAVE_INFO_T));
    g_mmisms_global.save_info.dual_sys = send_info_ptr->send_content.dual_sys;

    if (is_send_save)
    {
        if (PNULL != send_info_ptr)
        {
            total_num = MMISMS_SplitLongMsg(&edit_content_ptr->send_message, send_info_ptr->is_head_has_port);
            SCI_TRACE_LOW("MMISMS:MMISMS_AppSaveSmsEx send_save total_num = %d",total_num);
        }
    }
    else
    {
        total_num = MMISMS_SplitLongMsg(&edit_content_ptr->send_message, FALSE);
        SCI_TRACE_LOW("MMISMS:MMISMS_AppSaveSmsEx not_save total_num = %d",total_num);
    }

    MMISMS_InitUserDataHead(total_num, 1, &edit_content_ptr->tpdu_head, send_info_ptr);
    SCI_TRACE_LOW("MMISMS:MMISMS_AppSaveSmsEx is_send_save = %d",is_send_save);

    // set operation status
    if (is_send_save)
    {
        if (PNULL != send_info_ptr)
        {
            send_info_ptr->cur_send_data.cur_send_oper = MMISMS_SAVE_SEND_SMS;
        }
    }
    else
    {
        MMISMS_SetOperStatus(MMISMS_SAVE_SMS);
    }

//============== MET MEX  start ===================
#ifdef MET_MEX_SUPPORT
    MMIMEX_PhsSetSmsOperStatus();
#endif // MET_MEX_SUPPORT
//============== MET MEX  end ===================
    id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
    dest_addr_ptr = edit_content_ptr->dest_info.dest_list.dest_addr[id];
    if(PNULL != s_mmisms_sendlist.sendlist_head_ptr)
    {
        SCI_TRACE_LOW("MMISMS:MMISMS_AppSaveSms backsend_status = %d!",s_mmisms_sendlist.sendlist_head_ptr->backsend_status);
    }
    ret_val = MMISMS_SaveNextMessage(send_info_ptr->send_content.dual_sys, dest_addr_ptr, total_num, 1, send_info_ptr);

    if(MMISMS_NO_SPACE == ret_val && !is_send_save)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
    }

    MMISMS_CLEAR_OPER(ret_val);

    return (ret_val);
}

/*****************************************************************************/
//     Description : to save the info to order list by the result of save
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SaveInfoToOrderByResult(
                                           MN_DUAL_SYS_E            dual_sys, 
                                           MN_SMS_STORAGE_E            save_storage,
                                           MN_SMS_RECORD_ID_T        save_record_id,
                                           MN_SMS_STATUS_E            status,
                                           BOOLEAN                    is_exist_addr,
                                           MMISMS_FOLDER_TYPE_E       folder_type,
                                           MMISMS_OPER_STATUS_E oper_status,
                                           MMISMS_SEND_T *send_info_ptr,
                                           MMISMS_BACKSENDSTATUS_E send_status
                                           )
{
    MN_SMS_USER_DATA_HEAD_T save_head = {0};
    uint16 content_len = 0;//FIX BUG:1335142 �����������uint8�����ɷ��Ͷ���Assert
    wchar content[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_content[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    BOOLEAN is_ucs2 = FALSE;
    uint8 seq_num = 0;
    MMISMS_STATE_T order_info = {0};
    uint8 mmisms_status = 0;
#ifdef MMI_SMS_CHAT_SUPPORT    
    //MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MN_CALLED_NUMBER_T call_num = {0};
    MMI_TM_T time = {0};
    APP_SMS_USER_DATA_T sms_user_data_t = {0}; 
    uint16 start_pos = 0;
    int32 i = 0;
    uint8 len = 0;      
    MN_SMS_TIME_STAMP_T time_stamp = {0};
    MMISMS_BCD_NUMBER_T mms_bcd_num = {0};
#endif
    MMISMS_ORDER_ID_T current_order_id = MMISMS_GetCurOperationOrderId();

    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;

    //SCI_TRACE_LOW:"MMISMS: SaveInfoToOrderByResult(%d, %d, %d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1403_112_2_18_2_53_11_282,(uint8*)"dddd", save_storage, save_record_id, status, is_exist_addr);

    if (PNULL != send_info_ptr)
    {
        edit_content_ptr = &send_info_ptr->send_content;
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InterceptMsgContent PNULL !=  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1408_112_2_18_2_53_11_283,(uint8*)"");
    }
    else 
    {
        edit_content_ptr = &g_mmisms_global.edit_content;
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InterceptMsgContent PNULL ==  send_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1413_112_2_18_2_53_11_284,(uint8*)"");
    }
#ifdef MMI_SMS_CHAT_SUPPORT    
    g_mmisms_global.chat_add_order_id = PNULL;
#endif    
    if ( (1 == g_mmisms_global.save_info.max_num && PNULL == send_info_ptr)        
        ||( 
            (
#ifdef MMI_SMS_MOVE_SUPPORT              
			!MMISMS_IsMoveOperation(oper_status)&&
#endif			
#ifdef MMI_SMS_COPY_SUPPORT  
            !MMISMS_IsCopyOperation(oper_status)&&
#endif            
            !(MMISMS_SAVE_CLASS0_SMS == oper_status))
			&&(1 == GET_HEAD_MAX_NUM(edit_content_ptr) || 0 == GET_HEAD_MAX_NUM(edit_content_ptr))
        )
        ||
        (
             (
#ifdef MMI_SMS_MOVE_SUPPORT              
			 MMISMS_IsMoveOperation(oper_status)||
#endif			
#ifdef MMI_SMS_COPY_SUPPORT  
             MMISMS_IsCopyOperation(oper_status)||
#endif             
             (MMISMS_SAVE_CLASS0_SMS == oper_status))
			 &&(1 == Get_Read_Head_Max_Num() || 0 == Get_Read_Head_Max_Num())
        ) 
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
	  || !MMISMS_GetConcatenatedSMS()
#endif
	  ) //is normal sms
    {
        save_head.length = 0;
        seq_num = 1;
    }
    else //is long sms 
    {
        if (
#ifdef MMI_SMS_MOVE_SUPPORT            
            MMISMS_IsMoveOperation(oper_status)||
#endif            
#ifdef MMI_SMS_COPY_SUPPORT            
            MMISMS_IsCopyOperation(oper_status)||
#endif            
            (MMISMS_SAVE_CLASS0_SMS == oper_status))
        {
            //SCI_TRACE_LOW:"seq_num is %d,max_num is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1431_112_2_18_2_53_11_285,(uint8*)"dd", Get_Read_Current_Seq_Num(), Get_Read_Head_Max_Num());
            seq_num = Get_Read_Current_Seq_Num();
            if ( seq_num > Get_Read_Head_Max_Num())
            {
                seq_num = 1;
                g_mmisms_global.save_info.seq_num = 1;
            }    
            order_info.flag.is_concatenate_sms = TRUE;
            order_info.longsms_max_num = Get_Read_Head_Max_Num();
            order_info.longsms_seq_num = seq_num;
            order_info.head_ref = Get_Read_Head_Ref_Num();
        }
        else
        {
            //SCI_TRACE_LOW:"seq_num is %d,max_num is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1445_112_2_18_2_53_11_286,(uint8*)"dd", GET_CURRENT_SEQ_NUM(edit_content_ptr), GET_HEAD_MAX_NUM(edit_content_ptr));
            seq_num = GET_CURRENT_SEQ_NUM(edit_content_ptr);
            if ( seq_num > GET_HEAD_MAX_NUM(edit_content_ptr))
            {
                seq_num = 1;
                g_mmisms_global.save_info.seq_num = 1;
            }    
            order_info.flag.is_concatenate_sms = TRUE;
            order_info.longsms_max_num = GET_HEAD_MAX_NUM(edit_content_ptr);
            order_info.longsms_seq_num = seq_num;
            order_info.head_ref = Get_Current_Ref_Num(edit_content_ptr);
        }        
    }

    if(MMISMS_SAVE_VCARD_VCAL_SMS == oper_status)
    {
        if(1 == g_mmisms_global.save_info.max_num || 0 == g_mmisms_global.save_info.max_num)
        {
             save_head.length = 0;
             seq_num = 1;
        }
        else
        {
            order_info.flag.is_concatenate_sms = TRUE;
            order_info.longsms_max_num = g_mmisms_global.save_info.max_num;
			vcard_vcal_cur_save_index++;
            order_info.longsms_seq_num = vcard_vcal_cur_save_index;
            order_info.head_ref = g_mmisms_global.save_info.ref_num;
        }
    }
    if(
       0  /*lint !e506*/
#ifdef MMI_SMS_MOVE_SUPPORT
       || MMISMS_IsMoveOperation(oper_status)/*lint !e506*/
#endif
#ifdef MMI_SMS_COPY_SUPPORT
       || MMISMS_IsCopyOperation(oper_status)/*lint !e506*/
#endif       
	   || (MMISMS_SAVE_VCARD_VCAL_SMS == oper_status)
        )/*lint !e774*/
    {
            is_ucs2 = MMISMS_InterceptReadMsgContent(
                oper_status,
                seq_num,
                MMIMESSAGE_DIAPLAY_LEN,
                &content_len,
                content
                );
            g_mmisms_global.save_info.folder_type = folder_type;
    }
    else        
    {
        if((status == MN_SMS_MT_READED)
#ifdef IKEYBACKUP_SUPPORT
            && MMISMS_RESTORE_SMS != oper_status
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
         && (MMISMS_BT_SAVE_MT_SMS != oper_status )
         && (MMISMS_BT_SAVE_SENDSUCC_SMS != oper_status )
         && (MMISMS_BT_SAVE_SENDFAIL_SMS != oper_status)
         && (MMISMS_BT_SAVE_NOSEND_SMS != oper_status)
#endif
            )
        {
            is_ucs2 = MMISMS_InterceptClass0MsgContent(
                seq_num,
                MMIMESSAGE_DIAPLAY_LEN,
                &content_len,
                content
                );
        }
        else
        {
            is_ucs2 = MMISMS_InterceptMsgContent(
                seq_num,
                MMIMESSAGE_DIAPLAY_LEN,
                &content_len,
                content,
                send_info_ptr
                );
        }
    }
    
    //ת��MN StatusΪMMI Status
    if((status == MN_SMS_MT_READED) || (status == MN_SMS_MT_TO_BE_READ) ||
       (status == MN_SMS_MT_SR_READED) || (status == MN_SMS_MT_SR_TO_BE_READ))
    {
        mmisms_status = MMISMS_TranslateMNStatus2SFStatus(status, MMISMS_MT_TYPE, is_exist_addr);
    }
    else
    {
        mmisms_status = MMISMS_TranslateMNStatus2SFStatus(status, MMISMS_MO_TYPE, is_exist_addr);
    }
    
    //ΪOrder_info�ĸ�����ֵ
    order_info.flag.mo_mt_type = mmisms_status;
    order_info.flag.storage = save_storage;
    order_info.record_id = save_record_id;
    order_info.flag.is_ucs2 = is_ucs2;
    order_info.flag.dual_sys = dual_sys;
    order_info.flag.msg_type = MMISMS_TYPE_SMS;
    //����Ǳ������ֻ��ϵ�MO sms,��ȡ��ǰ��ʱ��Ϊorder_info��ʱ��
    order_info.flag.folder_type = folder_type;
    order_info.flag.is_locked = g_mmisms_global.save_info.is_locked;
    order_info.backsend_status = send_status;
#ifdef MMI_MSG_RESEND    
    if (PNULL != send_info_ptr)
    {
        if(oper_status == MMISMS_SEND_SAVE_SMS && MMISMS_MO_SEND_FAIL == order_info.flag.mo_mt_type && send_info_ptr->mosend_update_data.mosend_update_info.order_id_arr[0] != PNULL && send_info_ptr->mosend_update_data.mosend_update_info.order_id_arr[0]->flag.is_resend)
        {
            SCI_TRACE_LOW("mmisms order is_resend");
            order_info.flag.is_resend = TRUE;
        }    
    }
#endif
    if(MMISMS_FOLDER_SECURITY == folder_type
        && PNULL != current_order_id)
    {
        order_info.flag.is_search = FALSE;
        current_order_id->flag.is_search = FALSE;
    }

    if(
        (MMISMS_SAVE_CLASS0_SMS == oper_status)   /*lint !e506*/
#ifdef MMI_SMS_MOVE_SUPPORT
        || MMISMS_IsMoveOperation(oper_status)/*lint !e506*/
#endif     
#ifdef MMI_SMS_COPY_SUPPORT
        || MMISMS_IsCopyOperation(oper_status)/*lint !e506*/
#endif
       )/*lint !e774*/
    {
        order_info.time = g_mmisms_global.save_info.time;
    }
    else        
    {
        //����Ǳ������ֻ��ϵ�MO sms,��ȡ��ǰ��ʱ��Ϊorder_info��ʱ��
        if (save_storage == MN_SMS_STORAGE_ME) 
        {

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
            if ( MMISMS_BT_SAVE_MT_SMS == oper_status 
                || MMISMS_BT_SAVE_SENDSUCC_SMS == oper_status
                || MMISMS_BT_SAVE_SENDFAIL_SMS == oper_status
                || MMISMS_BT_SAVE_NOSEND_SMS == oper_status)
            {
                order_info.time = g_mmisms_global.save_info.time;
            }
            else
#endif

#ifdef IKEYBACKUP_SUPPORT    
            if (MMISMS_RESTORE_SMS == oper_status && 0 != g_sms_save_time)
            {
                order_info.time = g_sms_save_time;
                g_sms_save_time = 0;
            }
            else
#endif    
            {
                order_info.time = MMISMS_GetCurTime();
            }
            
        }
        else//����Ǳ�����sim���ϵ�MO sms,����ʱ������
        {
            order_info.time = MMISMS_GetCurTime();
        }
    }

    order_info.display_content.content_len = MIN(content_len,MMIMESSAGE_DIAPLAY_LEN);

    if(is_ucs2
#ifdef IKEYBACKUP_SUPPORT
            && MMISMS_RESTORE_SMS != oper_status
#endif
        )
    {
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    //do nothing
#else
        if(MMISMS_SAVE_CLASS0_SMS ==g_mmisms_global.operation.oper_status)
        {
            //do nothing!
        }
        else
        {
            GUI_UCS2L2UCS2B((uint8 *)temp_content,
                order_info.display_content.content_len * sizeof(wchar),
                (uint8 *)content,
                order_info.display_content.content_len * sizeof(wchar)
                );
            MMIAPICOM_Wstrncpy(
                content,
                temp_content,
                order_info.display_content.content_len);
        }
#endif
    }

    MMIAPICOM_Wstrncpy(
                       order_info.display_content.content,
                       content,
                       order_info.display_content.content_len
                       );
    if(
#ifdef MMI_SMS_MOVE_SUPPORT        
       MMISMS_IsMoveOperation(oper_status) ||
#endif       
#ifdef MMI_SMS_COPY_SUPPORT        
       MMISMS_IsCopyOperation(oper_status) ||
#endif
       MMISMS_SAVE_CLASS0_SMS == oper_status || 
       MMISMS_SAVE_VCARD_VCAL_SMS == oper_status
	   )
    {
        MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
        MN_CALLED_NUMBER_T called_number = {0};
        uint8 num_len = 0;
        uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
        MN_DUAL_SYS_E temp_dual_sys = MN_DUAL_SYS_MAX;
        if(MMISMS_SAVE_CLASS0_SMS == oper_status)
        {
            num_len = MMISMS_GetClass0OriginNum(&temp_dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
        }
        else
        { 
            num_len = MMISMS_GetOriginNum(&temp_dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
        }
        MMISMS_GetMnCalledNum(number, num_len, &party_number, &called_number);
        order_info.number.bcd_number.npi_ton = g_mmisms_global.save_info.number.npi_ton;
        order_info.number.bcd_number.number_len = party_number.num_len;
        SCI_MEMCPY(order_info.number.bcd_number.number, party_number.bcd_num, party_number.num_len);
    }
    else
    {
        uint8 addr_id = 0;
        MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
        MN_CALLED_NUMBER_T called_number = {0};

        addr_id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
        MMISMS_GetMnCalledNum(edit_content_ptr->dest_info.dest_list.dest_addr[addr_id], strlen((char*)edit_content_ptr->dest_info.dest_list.dest_addr[addr_id]), 
            &party_number, &called_number);
        order_info.number.bcd_number.npi_ton = g_mmisms_global.save_info.number.npi_ton;
        order_info.number.bcd_number.number_len = party_number.num_len;
        SCI_MEMCPY(order_info.number.bcd_number.number, party_number.bcd_num, party_number.num_len);   
    }
    
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if ( MMISMS_BT_SAVE_MT_SMS == oper_status 
        || MMISMS_BT_SAVE_SENDSUCC_SMS == oper_status
        || MMISMS_BT_SAVE_SENDFAIL_SMS == oper_status
        || MMISMS_BT_SAVE_NOSEND_SMS == oper_status)
    {
        
        order_info.flag.folder_type = MMISMS_FOLDER_BT;
        MMISMS_InsertBtSmsToOrder(&order_info);
    }
    else
#endif

    //����ֵ�õ�order_info���뵽������
    MMISMS_InsertSmsToOrder( &order_info);

#ifdef MMI_TIMERMSG_SUPPORT
    //post process timer SMS related 
    MMISMS_TIMERMSG_PostProc(&order_info);

    if (
        MMISMS_GetIsTimerMsgReplace()
                &&MMISMS_BOX_SENDFAIL == MMISMS_TIMERMSG_GetCurrBoxType()
                && (g_mmisms_global.update_info.order_id_arr[0] != PNULL
                && MMISMS_MO_SEND_FAIL ==  g_mmisms_global.update_info.order_id_arr[0]->flag.mo_mt_type)
                )
    {
        uint8 max_num = 0;
        seq_num =  GET_CURRENT_SEQ_NUM(edit_content_ptr) ;
        max_num = GET_HEAD_MAX_NUM(edit_content_ptr);
        if(seq_num == max_num || 0 == max_num)
        {
            //MMISMS_DeleteReqToMN(MMISMS_BOX_SENDFAIL,g_mmisms_global.update_info.order_id_arr[0],MMISMS_DELETE_MSG); 
            MMISMS_ORDER_ID_T update_order_id = PNULL;
            update_order_id = g_mmisms_global.update_info.order_id_arr[0];
            if(update_order_id != PNULL)
            {
                g_mmisms_global.update_info.current_num = 0;
                MMISMS_SetOperStatus(MMISMS_DELETE_MSG);
                MNSMS_UpdateSmsStatusEx(update_order_id->flag.dual_sys, update_order_id->flag.storage, update_order_id->record_id, MN_SMS_FREE_SPACE);                
                MMISMS_SetCurOrderId(0, update_order_id);
            }
            g_mmisms_global.update_info.current_num++;
                
            //MMISMS_SetIsTimerMsgReplace(FALSE);
        }
    }
    
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
    if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
#ifdef MMI_SMS_MOVE_SUPPORT        
       &&!MMISMS_IsMoveOperation(oper_status)
#endif       
#ifdef MMI_SMS_COPY_SUPPORT        
       &&!MMISMS_IsCopyOperation(oper_status) 
#endif
    )
    {
        uint8 addr_id = 0;
        MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};

        addr_id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
        MMISMS_GetMnCalledNum(edit_content_ptr->dest_info.dest_list.dest_addr[addr_id], strlen((char*)edit_content_ptr->dest_info.dest_list.dest_addr[addr_id]), 
        &party_number, &call_num);

        MMSNumberToBcd(g_mmisms_global.sender,mms_bcd_num.number,&mms_bcd_num.number_len);
    }
//     call_num.number_plan = MMIPB_GetNumberPlanFromUint8(g_mmisms_global.save_info.number.npi_ton);
//     call_num.number_type = MMIPB_GetNumberTypeFromUint8(g_mmisms_global.save_info.number.npi_ton);
//     call_num.num_len = g_mmisms_global.save_info.number.number_len;
//     SCI_MEMCPY(call_num.party_num, g_mmisms_global.save_info.number.number, g_mmisms_global.save_info.number.number_len);

    if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        //&& (MMISMS_SENDSTATUS_SENDING != order_info.backsend_status && MMISMS_SENDSTATUS_WAITING != order_info.backsend_status)
        //&& PNULL != cur_order_id
        && 0 == memcmp(call_num.party_num, mms_bcd_num.number, mms_bcd_num.number_len)
        && MMISMS_MO_DRAFT != mmisms_status
        )
    {

        if(
#ifdef MMI_SMS_MOVE_SUPPORT	            
            MMISMS_IsMoveOperation(oper_status) ||
#endif
#ifdef MMI_SMS_COPY_SUPPORT	            
            MMISMS_IsCopyOperation(oper_status) ||
#endif
		    MMISMS_SAVE_CLASS0_SMS == oper_status)
        {    
            MMISMS_ORDER_ID_T next_order_id = PNULL;  								      
            if( 
			(
#ifdef MMI_SMS_MOVE_SUPPORT
				MMISMS_IsMoveOperation(oper_status) && 
#endif
			MMISMS_MOVE_SMS_TO_SECURITY != oper_status
			) 
			|| 
			(
#ifdef MMI_SMS_COPY_SUPPORT
				   MMISMS_IsCopyOperation(oper_status) &&
#endif
			MMISMS_MOVE_SMS_TO_SECURITY != oper_status
			)
		)
            {      
                            // check whether there has the next message to read
                if (g_mmisms_global.operation.cur_order_index < MMISMS_SPLIT_MAX_NUM -1)
                {              
                    next_order_id = g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index + 1];                  
                } 
                if(PNULL == next_order_id )
                {
                     MMK_SendMsg(
                            MMISMS_SHOW_CHAT_SMS_WIN_ID,
                            MSG_SMS_FINISH_READ,
                            (ADD_DATA)g_mmisms_global.chat_oper_order_id                    
                            ); 
                }            
            }
        }
        else            
        {   
    
            time  = MMIAPICOM_Second2Tm(order_info.time);
            time_stamp.year = time.tm_year - MMISMS_OFFSERT_YEAR;
            time_stamp.month = time.tm_mon; 
            time_stamp.day = time.tm_mday;
            time_stamp.hour = time.tm_hour;
            time_stamp.minute = time.tm_min;
            time_stamp.second = time.tm_sec;
    
            SCI_MEMCPY(&sms_user_data_t.user_data_head_t,&edit_content_ptr->tpdu_head,sizeof(MN_SMS_USER_DATA_HEAD_T));
            for (i = 0; i < (int32)seq_num - 1; i++)
            {
                start_pos = start_pos + edit_content_ptr->send_message.split_sms_length[i];
            }
            len = (uint8)MIN(MMISMS_MAX_DEF_BYTE_LEN, edit_content_ptr->send_message.split_sms_length[seq_num - 1]);
    
            sms_user_data_t.user_valid_data_t.length = len;
    
            if(MN_SMS_DEFAULT_ALPHABET == edit_content_ptr->send_message.alphabet)
            {
                SCI_MEMCPY(sms_user_data_t.user_valid_data_t.user_valid_data_arr,edit_content_ptr->send_message.data + start_pos,len);
            }
            else
            {
    #if defined(__BIG_ENDIAN) || defined(__BigEndian)
            SCI_MEMCPY(
                &(sms_user_data_t.user_valid_data_t.user_valid_data_arr), 
                (edit_content_ptr->send_message.data + start_pos),
                len
                );    
    #else
            GUI_UCS2L2UCS2B(sms_user_data_t.user_valid_data_t.user_valid_data_arr,
                            len,
                            (edit_content_ptr->send_message.data + start_pos),
                            len);
    #endif            
            }
                
        	  MMISMS_ChatSetMTGlobalReadMsg(
        								  dual_sys,
        								  &sms_user_data_t,    //IN:
        								  save_storage,        //IN:
        								  save_record_id,        //IN:
        								  edit_content_ptr->send_message.alphabet,    //IN:
        								  &time_stamp,//IN:
        								  &call_num,    //IN:
										  (MMISMS_MO_MT_TYPE_E)mmisms_status
        								 ); 
#if 0  //user sms trig              
            MMK_SendMsg(
                        MMISMS_SHOW_CHAT_SMS_WIN_ID,
                        MSG_SMS_FINISH_READ,
                        (ADD_DATA)g_mmisms_global.chat_oper_order_id                    
                        ); 
#endif
        }            
    }

    
#endif

#ifdef IKEYBACKUP_SUPPORT
        //memset
        if (MMISMS_RESTORE_SMS == oper_status)
        {
            MMISMS_ClearDestNum();
        }
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
    if(g_mmisms_global.chat_oper_order_id != PNULL && order_info.record_id == g_mmisms_global.chat_oper_order_id->record_id 
        && order_info.flag.storage == g_mmisms_global.chat_oper_order_id->flag.storage 
        && order_info.flag.dual_sys == g_mmisms_global.chat_oper_order_id->flag.dual_sys 
        )
    {
        g_mmisms_global.chat_add_order_id = g_mmisms_global.chat_oper_order_id;
    }
    else
    {
        g_mmisms_global.chat_add_order_id = PNULL;
    }
#endif

    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_ADD);    
}

/*****************************************************************************/
//     Description : to save the SMS, called by window
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppUpdateMoSms( 
                                                MN_DUAL_SYS_E        dual_sys,
                                                MMISMS_OPER_STATUS_E	update_oper,	//IN:
                                                MN_SMS_STATUS_E update_status,
                                                MMISMS_SEND_T *send_info_ptr
                                               )
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
    uint8 total_num = 1;
    uint8 mosend_total_num = 0;
    uint16 i = 0;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    
    if (PNULL != send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AppUpdateMoSms send_info_ptr != PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1753_112_2_18_2_53_12_287,(uint8*)"");
        edit_content_ptr = &send_info_ptr->send_content;
        total_num = MMISMS_SplitLongMsg(&edit_content_ptr->send_message, send_info_ptr->is_head_has_port);
        
    }  
    else
    {
        edit_content_ptr = &g_mmisms_global.edit_content;
        total_num = MMISMS_SplitLongMsg(&edit_content_ptr->send_message, FALSE);
    }
    
    
    if (MMISMS_UPDATE_BACKSEND_SMS == update_oper && PNULL != send_info_ptr)
    {
        send_info_ptr->backsend_update_data.backsend_update_info.update_num = total_num;
        send_info_ptr->backsend_update_data.backsend_update_info.total_num = total_num;
    }
    else if (PNULL != send_info_ptr && send_info_ptr->mosend_update_data.is_mosend_update)
    {
        for(i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)  
        {
            if(send_info_ptr->mosend_update_data.mosend_update_info.order_id_arr[i] != PNULL)
            {
                mosend_total_num++;
            }
        }
            
        send_info_ptr->mosend_update_data.mosend_update_info.update_num = mosend_total_num; 
        send_info_ptr->mosend_update_data.mosend_update_info.total_num = mosend_total_num;
    }
    else
    {
        g_mmisms_global.update_info.update_num = total_num;
        g_mmisms_global.update_info.total_num = total_num;
    }    
    
    // init user data head for MO SMS
    MMISMS_InitUserDataHead(total_num, 1, &edit_content_ptr->tpdu_head, send_info_ptr);
    //fixed bug159006
    //MMISMS_SetOperStatus(update_oper);
    if (MMISMS_UPDATE_BACKSEND_SMS != update_oper && PNULL != send_info_ptr)
    {        
        send_info_ptr->cur_send_data.cur_send_oper = update_oper;
    }
    
    
    // update the MO SMS and the sequence from 1
    if (MMISMS_UPDATE_BACKSEND_SMS == update_oper)
    {
        ret_val = MMISMS_UpdateNextSms(
            dual_sys,  edit_content_ptr->dest_info.update_dest_addr, total_num, 1, 
            update_status, send_info_ptr, TRUE);
    }
    else
    {
        ret_val = MMISMS_UpdateNextSms(
            dual_sys,  edit_content_ptr->dest_info.update_dest_addr, total_num, 1, 
            update_status, send_info_ptr, FALSE);
    }
    MMISMS_CLEAR_OPER(ret_val);
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : to save the next SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_SaveNextMessage(
                                                MN_DUAL_SYS_E dual_sys,
                                                uint8    *dest_addr_ptr,    //IN:
                                                uint8   total_num,        //IN
                                                uint8   seq_num,            //IN: the seq number ( 1 ~ 8)
                                                MMISMS_SEND_T *send_info_ptr
                                                )
{
    APP_SMS_USER_DATA_T user_data = {0};
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_SPACE;
    uint8 addr_len = 0;
    MN_SMS_RECORD_ID_T record_id = 0;
    uint8 id = 0;
    BOOLEAN is_find_free_record = FALSE;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;

    
    if (PNULL != send_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SaveNextMessage send_info_ptr != PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_1831_112_2_18_2_53_12_288,(uint8*)"");
        edit_content_ptr = &send_info_ptr->send_content;
    }
    else
    {
        edit_content_ptr = &g_mmisms_global.edit_content;
    }
    if (total_num > 1)
    {
        if(1 == seq_num )
        {
            MMISMS_GetStoreStorageForSendLongSms(dual_sys, total_num, seq_num, &storage);
            g_mmisms_global.save_info.storage = storage;
        }
        else
        {
            storage = g_mmisms_global.save_info.storage;
            id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
            dest_addr_ptr = edit_content_ptr->dest_info.dest_list.dest_addr[id];
        }
        is_find_free_record = MMISMS_FindFreeRecordId(dual_sys, storage, &record_id);
        if(!is_find_free_record)
        {
            storage = MN_SMS_NO_STORED;
        }
    }
    else
    {
		//���ݶ��������еĴ洢λ���趨���ȴ洢��
 		MMISMS_GetStoreStorage(dual_sys, &storage, &record_id);  
    }
    
    
    if (MN_SMS_NO_STORED != storage)
    {
        //��ǰ����ĵ�������Ϣ��record id������ʶ��save cnf
        if (PNULL != send_info_ptr)
        {
            send_info_ptr->cur_send_data.cur_send_record_id = record_id;
            send_info_ptr->cur_send_data.cur_send_storage = storage;
        }
        

        //init the variable
        SCI_MEMSET(&user_data, 0, sizeof (APP_SMS_USER_DATA_T));
        // get the user data
        MMISMS_GetUserData(
            seq_num, 
            &(edit_content_ptr->send_message),
            &(edit_content_ptr->tpdu_head),
            TRUE,
            &user_data,                            //OUT:
            send_info_ptr
            );
        
        g_mmisms_global.save_info.max_num = 1;
        
        if (((user_data.user_data_head_t.length > 0) && !MMISMS_IsR8Language(edit_content_ptr->send_message.language))
            || ((user_data.user_data_head_t.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(edit_content_ptr->send_message.language)))
        {
            g_mmisms_global.save_info.max_num = user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
            g_mmisms_global.save_info.ref_num = GET_CURRENT_REF_NUM(edit_content_ptr);
        }
        
        if (PNULL != dest_addr_ptr)
        {
            addr_len = strlen((char*)dest_addr_ptr);
        }
        if(PNULL != s_mmisms_sendlist.sendlist_head_ptr)
        {
            SCI_TRACE_LOW("mmisms1:MMISMS_SaveNextMessage backsend_status = %d!",s_mmisms_sendlist.sendlist_head_ptr->backsend_status);
        }
        ret_val = SaveMsgReq( 
            dual_sys,
            dest_addr_ptr, 
            addr_len, 
            &user_data, 
            storage, 
            edit_content_ptr->send_message.alphabet 
            );
    }
    else
    {
        //prompt: no free space
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
        MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
        MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
    }
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : to save the Class0 SMS 
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_SaveClass0Msg( void )
{
    APP_SMS_USER_DATA_T user_data = {0};
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_SPACE;
    // uint8 addr_len = 0;
    MN_SMS_RECORD_ID_T record_id = 0;

    MMISMS_GetStoreStorage(g_mmisms_global.class0_msg.dual_sys, &storage, &record_id);

    if (MN_SMS_NO_STORED != storage)
    {
        g_mmisms_global.save_info.max_num = 1;
        
        user_data.user_valid_data_t.length = g_mmisms_global.class0_msg.read_content.length;
        SCI_MEMCPY(user_data.user_valid_data_t.user_valid_data_arr,
                              g_mmisms_global.class0_msg.read_content.data,
                              user_data.user_valid_data_t.length);
 
        ret_val = SaveClass0MsgReq( 
            g_mmisms_global.class0_msg.dual_sys,
            g_mmisms_global.class0_msg.address.party_num, 
            g_mmisms_global.class0_msg.address.num_len, 
            &user_data, 
            storage, 
            g_mmisms_global.class0_msg.read_content.alphabet
            );
    }
    else
    {
        //prompt: no free space
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE, MMISMS_HandleSaveNextMessageAlertWin);
        MMIPUB_CloseWaitWin(MMISMS_SEND_ANIM_WIN_ID);
        MMIPUB_CloseWaitWin(MMISMS_TIMERMSG_WAIT_WIN_ID);
    }

    return (ret_val);
}

/*****************************************************************************/
//     Description : HandleSaveNextMessageAlertWin
//    Global resource dependence : 
//  Author:wancan.you
//    Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleSaveNextMessageAlertWin(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_LOSE_FOCUS:
        recode = MMI_RESULT_TRUE;//Lose focus��ʱ�򲻹رմ���
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//     Description : to check whether there have enough spaces to save
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_HaveEnoughSpace(
                                      MN_DUAL_SYS_E dual_sys,
                                      BOOLEAN    is_new_message,    //IN:
                                      uint8        org_num            //IN:
                                      )
{
    uint8 total_num = 0;
    uint16 free_num = 0;
    BOOLEAN ret_val = FALSE;
    uint16    sim_used_num    = 0;
    uint16    sim_max_num        = 0;    
    uint16    nv_used_num    = 0;
    uint16    nv_max_num        = 0; 

    MMISMS_GetSMSMemory(dual_sys, FALSE, &sim_max_num, &sim_used_num);
    MMISMS_GetSMSMemory(dual_sys, TRUE, &nv_max_num, &nv_used_num); 
    
    if (is_new_message)
    {        
        total_num = MMISMS_SplitLongMsg(&g_mmisms_global.edit_content.send_message, FALSE);
        free_num = MMISMS_CountFreeSpace(dual_sys);
        if((total_num > (sim_max_num - sim_used_num)) &&
           (total_num > (nv_max_num - nv_used_num)))
        {
            return (ret_val); 
        }
    }
    else
    {        
        total_num = MMISMS_SplitLongMsg(&g_mmisms_global.edit_content.send_message, FALSE);
        free_num = MMISMS_CountFreeSpace(dual_sys) + org_num;
        if((total_num > (sim_max_num - sim_used_num + org_num)) &&
            (total_num > (nv_max_num - nv_used_num + org_num)))
        {
            return (ret_val); 
        }        
    }
    
    if (free_num >= total_num)
    {
        ret_val = TRUE;
    }
    if (free_num < 2 * total_num)
    {
        g_mmisms_global.edit_content.is_enough_space_backsend = TRUE;
    }
    return (ret_val);
}

/*****************************************************************************/
//     Description : encode MO SMS data
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EncodeMoSmsData(
                                   MN_DUAL_SYS_E    dual_sys,
                                   MN_SMS_MO_SMS_T            *mo_sms_ptr,        //OUT:
                                   MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN:
                                   APP_SMS_USER_DATA_T         *user_data_ptr        //IN:
                                   )
{
#if 0
// no use
    MN_SMS_USER_VALID_DATA_T data_buf_t = {0};
#endif
    
    if (PNULL == mo_sms_ptr || PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_EncodeMoSmsData mo_sms_ptr user_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2052_112_2_18_2_53_13_289,(uint8*)"");
        return;
    }
    
#if 0
// no use
    data_buf_t = user_data_ptr->user_valid_data_t;
#endif
    
    if (user_data_ptr->user_data_head_t.length > 0)
    {
        mo_sms_ptr->user_head_is_exist = TRUE;
    }
    else
    {
        mo_sms_ptr->user_head_is_exist = FALSE;
    }
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_EncodeMoSmsData user_head_is_exist = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2067_112_2_18_2_53_13_290,(uint8*)"d", mo_sms_ptr->user_head_is_exist);
    MNSMS_EncodeUserDataEx(
        dual_sys,
        mo_sms_ptr->user_head_is_exist,    
        alphabet_type,
        &user_data_ptr->user_data_head_t,
        &user_data_ptr->user_valid_data_t,
        &(mo_sms_ptr->user_data_t)
        );
}

/*****************************************************************************/
//     Description : encode MT SMS data
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EncodeMtSmsData(
                                   MN_DUAL_SYS_E    dual_sys,
                                   MN_SMS_MT_SMS_T            *mt_sms_ptr,        //OUT:
                                   MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN:
                                   APP_SMS_USER_DATA_T         *user_data_ptr        //IN:
                                   )
{
#if 0
// no use
    MN_SMS_USER_VALID_DATA_T data_buf_t = {0};
#endif
    
    if (PNULL == mt_sms_ptr || PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_EncodeMoSmsData mt_sms_ptr user_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2093_112_2_18_2_53_13_291,(uint8*)"");
        return;
    }
#if 0
// no use
    data_buf_t = user_data_ptr->user_valid_data_t;
#endif

    if (user_data_ptr->user_data_head_t.length > 0)
    {
        mt_sms_ptr->user_head_is_exist = TRUE;
    }
    else
    {
        mt_sms_ptr->user_head_is_exist = FALSE;
    }
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_EncodeMtSmsData user_head_is_exist = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2107_112_2_18_2_53_13_292,(uint8*)"d", mt_sms_ptr->user_head_is_exist);
    MNSMS_EncodeUserDataEx(
        dual_sys,
        mt_sms_ptr->user_head_is_exist,    
        alphabet_type,
        &user_data_ptr->user_data_head_t,
        &user_data_ptr->user_valid_data_t,
        &(mt_sms_ptr->user_data_t)
        );
}


/*****************************************************************************/
//     Description : to get a free space in ME or SIM
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC  void MMISMS_GetStoreStorage(
                                    MN_DUAL_SYS_E            dual_sys,
                                    MN_SMS_STORAGE_E        *storage_ptr,    //OUT:
                                    MN_SMS_RECORD_ID_T    *record_id_ptr    //OUT:
                                    )
{
//    int32 i = 0;
    int8 j = 0;
    BOOLEAN result = FALSE;
    uint16 max_num = 0;
    uint16 used_num = 0;
    MN_SMS_STORAGE_E storage[MMISMS_STORAGE_NUM] = {MN_SMS_STORAGE_ME, MN_SMS_STORAGE_SIM};
//    MN_SMS_STATUS_E *status_ptr = PNULL;
    MMISMS_SMS_SAVEPOS_TYPE_E save_pos = MMISMS_SAVEPOS_NONE;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    
    if (PNULL == storage_ptr || PNULL == record_id_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetStoreStorage storage_ptr record_id_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2139_112_2_18_2_53_13_293,(uint8*)"");
        return;
    }
    
    //���ݶ��������еĴ洢λ���趨���ȴ洢��
    save_pos = MMISMS_GetSavePos();
    if((MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type) &&
       (MMISMS_SEND_SMS != MMISMS_GetCurrentOperStatus()))
    {
        storage[0] = MN_SMS_STORAGE_ME;
        storage[1] = MN_SMS_NO_STORED;
    }
    else
    {
        if(MMISMS_SAVEPOS_SIM == save_pos)
        {
            storage[0] = MN_SMS_STORAGE_SIM;
            storage[1] = MN_SMS_STORAGE_ME;
        }
        else
        {
            storage[0] = MN_SMS_STORAGE_ME;
            storage[1] = MN_SMS_STORAGE_SIM;
        }
    }
    
    // set initial value
    (*storage_ptr) = MN_SMS_NO_STORED;
    
    for (j=0; j<MMISMS_STORAGE_NUM; j++)
    {
        // get SMS max count and used num
        MNSMS_GetSmsNumEx(dual_sys, storage[j], &max_num, &used_num);
        
        if (used_num < max_num)
        {
            break;
        }
    }
    
    if (j < MMISMS_STORAGE_NUM)
    {
        (*storage_ptr) = storage[j];

         err_code = MNSMS_FindFreeRecordIdByIndex(dual_sys, storage[j], 0, record_id_ptr);

        if (ERR_MNSMS_NONE == err_code)
        {
            result = TRUE; 
        }


        if(!result)
        {
            (*storage_ptr) = MN_SMS_NO_STORED;
        }
        
    }
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetStoreStorage storage = %d, record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2229_112_2_18_2_53_13_295,(uint8*)"dd", *storage_ptr, *record_id_ptr);
} 


/*****************************************************************************/
//     Description : to get a free space in ME or SIM
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC  void MMISMS_GetStoreStorageEx(
                                    MN_DUAL_SYS_E            dual_sys,
                                    MN_SMS_STORAGE_E        *storage_ptr,    //OUT:
                                    MN_SMS_RECORD_ID_T    *record_id1_ptr,    //OUT:
                                    MN_SMS_RECORD_ID_T    *record_id2_ptr    //OUT:
                                    )
{
//    int32 i = 0;
    int8 j = 0;
    BOOLEAN result = FALSE;
    uint16 max_num = 0;
    uint16 used_num = 0;
    MN_SMS_STORAGE_E storage[MMISMS_STORAGE_NUM] = {MN_SMS_STORAGE_ME, MN_SMS_STORAGE_SIM};
//    MN_SMS_STATUS_E *status_ptr = PNULL;
    MMISMS_SMS_SAVEPOS_TYPE_E save_pos = MMISMS_SAVEPOS_NONE;
//    uint16 index = 0;
    ERR_MNSMS_CODE_E err_code1 = ERR_MNSMS_NONE;
    ERR_MNSMS_CODE_E err_code2 = ERR_MNSMS_NONE;

    if (PNULL == storage_ptr || PNULL == record_id1_ptr || PNULL == record_id2_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetStoreStorage storage_ptr record_id_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2139_112_2_18_2_53_13_293,(uint8*)"");
        return;
    }
    
    //���ݶ��������еĴ洢λ���趨���ȴ洢��
    save_pos = MMISMS_GetSavePos();
    if((MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type) &&
       (MMISMS_SEND_SMS != MMISMS_GetCurrentOperStatus()))
    {
        storage[0] = MN_SMS_STORAGE_ME;
        storage[1] = MN_SMS_NO_STORED;
    }
    else
    {
        if(MMISMS_SAVEPOS_SIM == save_pos)
        {
            storage[0] = MN_SMS_STORAGE_SIM;
            storage[1] = MN_SMS_STORAGE_ME;
        }
        else
        {
            storage[0] = MN_SMS_STORAGE_ME;
            storage[1] = MN_SMS_STORAGE_SIM;
        }
    }
    
    // set initial value
    (*storage_ptr) = MN_SMS_NO_STORED;
    
    for (j=0; j<MMISMS_STORAGE_NUM; j++)
    {
        // get SMS max count and used num
        MNSMS_GetSmsNumEx(dual_sys, storage[j], &max_num, &used_num);
        
        if ((used_num + 1) < max_num)//��2��λ�ã�+1
        {
            break;
        }
    }
    
    if (j < MMISMS_STORAGE_NUM)
    {
        (*storage_ptr) = storage[j];
        err_code1 = MNSMS_FindFreeRecordIdByIndex(dual_sys, storage[j], 0, record_id1_ptr);
        err_code2 = MNSMS_FindFreeRecordIdByIndex(dual_sys, storage[j], 1, record_id2_ptr);
    
        if (ERR_MNSMS_NONE == err_code1 &&  ERR_MNSMS_NONE == err_code2)
        {
            result = TRUE; 
        }

        if(!result)
        {
            (*storage_ptr) = MN_SMS_NO_STORED;
        }
        
    }
    SCI_TRACE_LOW("MMISMS: MMISMS_GetStoreStorage storage = %d, record_id1 = %d, record_id2 = %d",*storage_ptr, *record_id1_ptr,*record_id2_ptr);
} 

/*****************************************************************************/
//     Description : to get a free space in ME or SIM for long sms(total_num <= space num)
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetStoreStorageForSendLongSms(
                                                 MN_DUAL_SYS_E        dual_sys,
                                                 uint8                    total_num,        //IN
                                                 uint8                  seq_num,        //IN
                                                 MN_SMS_STORAGE_E        *storage_ptr    //OUT:
                                                 )
{
    MN_SMS_STORAGE_E storage[MMISMS_STORAGE_NUM] = {MN_SMS_STORAGE_ME, MN_SMS_STORAGE_SIM};
    uint16 sms_max_num = 0;
    uint16 sms_used_num = 0;
    MMISMS_SMS_SAVEPOS_TYPE_E   save_pos = MMISMS_SAVEPOS_NONE;
    int8 j = 0;
    uint16 max_num = 0;
    uint16 used_num = 0;    
    
    if (PNULL == storage_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetStoreStorageForSendLongSms storage_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2253_112_2_18_2_53_13_296,(uint8*)"");
        return;
    }   
    //���ݶ��������еĴ洢λ���趨���ȴ洢��
    save_pos = MMISMS_GetSavePos();
	if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
    {
        storage[0] = MN_SMS_STORAGE_ME;
        storage[1] = MN_SMS_NO_STORED;
    }
	else
	{
		if(MMISMS_SAVEPOS_SIM == save_pos)
		{
			storage[0] = MN_SMS_STORAGE_SIM;
			storage[1] = MN_SMS_STORAGE_ME;
		}
		else
		{
			storage[0] = MN_SMS_STORAGE_ME;
			storage[1] = MN_SMS_STORAGE_SIM;
		}
	}
	
    (*storage_ptr) = MN_SMS_NO_STORED;	

    for (j=0; j<MMISMS_STORAGE_NUM; j++)
    {
        // get SMS max count and used num
        MNSMS_GetSmsNumEx(dual_sys, storage[j], &max_num, &used_num);
        
        if (used_num < max_num)
        {
            break;
        }
    }
    
    if (j < MMISMS_STORAGE_NUM)	
    {	
	    if (storage[0] == MN_SMS_STORAGE_SIM)
	    {
	        //no space in sim
	        MMISMS_GetSMSMemory(dual_sys, FALSE,&sms_max_num,&sms_used_num);
	        if ((total_num - seq_num +1) <= (sms_max_num - sms_used_num))
	        {
	            (*storage_ptr) = MN_SMS_STORAGE_SIM;
	        }
	        else
	        {
	            MMISMS_GetSMSMemory(dual_sys, TRUE,&sms_max_num,&sms_used_num);
	            if((total_num - seq_num +1) <= (sms_max_num - sms_used_num))
	            {
	                (*storage_ptr) = MN_SMS_STORAGE_ME;
	            }
	            else
	            {
	                (*storage_ptr) = MN_SMS_NO_STORED;
	            }
	        }
	    }
	    else if (storage[0] == MN_SMS_STORAGE_ME)
	    {
	        MMISMS_GetSMSMemory(dual_sys, TRUE,&sms_max_num,&sms_used_num);
	        if ((total_num - seq_num +1) <= (sms_max_num - sms_used_num))
	        {
	            (*storage_ptr) = MN_SMS_STORAGE_ME;
	        }
	        else
	        {
	            //no space in nv
	            MMISMS_GetSMSMemory(dual_sys, FALSE,&sms_max_num,&sms_used_num);
	            if((total_num - seq_num +1) <= (sms_max_num - sms_used_num))
	            {
	                (*storage_ptr) = MN_SMS_STORAGE_SIM;
	            }
	            else
	            {
	                (*storage_ptr) = MN_SMS_NO_STORED;
	            }
	        }
	    }
	    else
	    {
            //SCI_TRACE_LOW:"MMISMS:MMISMS_GetStoreStorageForSendLongSms 000"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2336_112_2_18_2_53_13_297,(uint8*)"");
	    }
	}
    
    //SCI_TRACE_LOW:"SMS: MMISMS_GetStoreStorageForSendLongSms storage = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2340_112_2_18_2_53_13_298,(uint8*)"d", *storage_ptr);
} 

/*****************************************************************************/
//     Description : to update the next SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_UpdateNextSms( 
                                              MN_DUAL_SYS_E           dual_sys,
                                              uint8                *dest_addr_ptr,    //IN:
                                              uint8                total_num,        //IN
                                              uint8                seq_num,        //IN: the seq number (1 ~ 8)
                                              MN_SMS_STATUS_E    status,            //IN:
                                              MMISMS_SEND_T *send_info_ptr,
                                              BOOLEAN is_backsend_update
                                              )
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_SPACE;
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    APP_SMS_USER_DATA_T user_data = {0};
    MN_SMS_RECORD_ID_T record_id = 0;
    uint8 len = 0;
    BOOLEAN is_find_free_record = FALSE;
    uint16 nv_max_num = 0;
    uint16 nv_used_num = 0;
    uint16 sim_max_num = 0;
    uint16 sim_used_num = 0;
    uint16 max_num = 0;
    uint16 used_num = 0;
#if 0
// no use
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
#endif
    MN_SMS_STORAGE_E long_sms_storage = MN_SMS_NO_STORED; 
    uint8 org_num = 0;  
    
    MMISMS_UPDATE_INFO_T *update_info_ptr = PNULL;  
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;

    g_mmisms_global.save_info.is_change_save_pos = FALSE;
    org_num = MMISMS_GetReadSMSNum();

    if (PNULL != send_info_ptr)
    {
        edit_content_ptr = &send_info_ptr->send_content;
    }
    else 
    {
        edit_content_ptr = &g_mmisms_global.edit_content;
    }

    if (is_backsend_update && PNULL != send_info_ptr)
    {
        update_info_ptr = &send_info_ptr->backsend_update_data.backsend_update_info;
    }
    else if (PNULL != send_info_ptr && send_info_ptr->mosend_update_data.is_mosend_update)
    {
        update_info_ptr = &send_info_ptr->mosend_update_data.mosend_update_info;
    }
    else
    {
        update_info_ptr = &g_mmisms_global.update_info;
    }
    
    if (((MMISMS_UPDATE_NOSEND_SMS == MMISMS_GetCurrentOperStatus()) && (update_info_ptr->current_num < MMISMS_SPLIT_MAX_NUM ) && (PNULL != update_info_ptr->order_id_arr[update_info_ptr->current_num]) && (update_info_ptr->current_num < update_info_ptr->total_num))
    || (MMISMS_UPDATE_NOSEND_SMS != MMISMS_GetCurrentOperStatus() && update_info_ptr->current_num < update_info_ptr->total_num))
    {
        // get storage and record_id from original message
        MMISMS_GetPointedSMSMemInfo( 
            update_info_ptr->order_id_arr[update_info_ptr->current_num],
            &dual_sys,
            &storage,
            &record_id
            );

        MMISMS_GetSMSMemory(dual_sys, TRUE, &nv_max_num, &nv_used_num);
        MMISMS_GetSMSMemory(dual_sys, FALSE, &sim_max_num, &sim_used_num);
        MNSMS_GetSmsNumEx(dual_sys, storage, &max_num, &used_num);

        if(MN_SMS_STORAGE_ME == storage)
        {
            if((total_num > (nv_max_num - nv_used_num + org_num)) &&
               (total_num > (sim_max_num - sim_used_num)))
            {
                storage = MN_SMS_NO_STORED;
                ret_val = MMISMS_NO_ENOUGH_SPACE;
				return ret_val;
            }
        }

        else if(MN_SMS_STORAGE_SIM == storage)
        {
            if((total_num > (nv_max_num - nv_used_num)) &&
               (total_num > (sim_max_num - sim_used_num + org_num)))
            {
                storage = MN_SMS_NO_STORED;
                ret_val = MMISMS_NO_ENOUGH_SPACE;
				return ret_val;
            }
        }

        if(total_num > (max_num - used_num + org_num))
        {
            MMISMS_GetStoreStorageForSendLongSms(dual_sys, total_num, seq_num, &long_sms_storage);
#if 0
// no use
            err_code = MNSMS_UpdateSmsStatusEx(
                                        dual_sys, 
                                        storage, 
                                        record_id, 
                                        MN_SMS_FREE_SPACE);
#else
            MNSMS_UpdateSmsStatusEx(
                                        dual_sys, 
                                        storage, 
                                        record_id, 
                                        MN_SMS_FREE_SPACE);
#endif
            g_mmisms_global.save_info.is_change_save_pos = TRUE;
            g_mmisms_global.save_info.storage = long_sms_storage;
            MMISMS_SetsmsOrder(update_info_ptr->order_id_arr[update_info_ptr->current_num]);
            return MMISMS_NO_ERR;
        }
        else
        {
            g_mmisms_global.save_info.storage = storage;
        }
    }
    else
    {
        if (total_num > 1)
        {
            if(1 == seq_num )
            {
                MMISMS_GetStoreStorageForSendLongSms(dual_sys, total_num, seq_num, &storage);
                g_mmisms_global.save_info.storage = storage;
            }
            else
            {
                storage = g_mmisms_global.save_info.storage;
            }
            is_find_free_record = MMISMS_FindFreeRecordId(dual_sys, storage, &record_id);
            if(!is_find_free_record)
            {
                storage = MN_SMS_NO_STORED;
            }
        }
        else
        {
            //find a free record id to save the message
            MMISMS_GetStoreStorage(dual_sys, &storage, &record_id);
        }
    }
    
    if (MN_SMS_NO_STORED != storage)
    {
        if (is_backsend_update && PNULL != send_info_ptr)
        {
            send_info_ptr->backsend_update_data.backsend_record_id = record_id;
            send_info_ptr->backsend_update_data.backsend_updata_storage = storage;            
        }
        else if (PNULL != send_info_ptr && send_info_ptr->mosend_update_data.is_mosend_update)
        {
            send_info_ptr->mosend_update_data.mosend_record_id = record_id;
            send_info_ptr->mosend_update_data.mosend_updata_storage = storage;
        }
                
        if (MN_SMS_FREE_SPACE != status)
        {
            MMISMS_SAVE_INFO_T    *save_info_ptr = &(g_mmisms_global.save_info);
            
            // get the user data
            MMISMS_GetUserData(
                seq_num, 
                &(edit_content_ptr->send_message),
                &(edit_content_ptr->tpdu_head),
                TRUE,
                &user_data,
                send_info_ptr
                );
            
            if (((user_data.user_data_head_t.length > 0) && !MMISMS_IsR8Language(edit_content_ptr->send_message.language))
                || ((user_data.user_data_head_t.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(edit_content_ptr->send_message.language)))
            {
                save_info_ptr->max_num = user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
                save_info_ptr->seq_num = seq_num;
                save_info_ptr->ref_num = GET_CURRENT_REF_NUM(edit_content_ptr);
            }
            else
            {
                save_info_ptr->max_num = 1;
                save_info_ptr->seq_num = 0;
                save_info_ptr->ref_num = 0;
            }
            
            if (PNULL != dest_addr_ptr)
            {
                len = strlen((char *)dest_addr_ptr);
            }
        }
        
        // send update request to MN
        ret_val = MMISMS_UpdateMsgReq(dual_sys, dest_addr_ptr, len, status, &user_data, storage, record_id, edit_content_ptr->send_message.alphabet);
    }
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : MMISMS_SetSMSSaveInfo
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSMSSaveInfo(
                                  MN_DUAL_SYS_E dual_sys,
                                  MN_CALLED_NUMBER_T *dest_num_ptr
                                  )
{
    if (PNULL == dest_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SetSMSSaveInfo dest_num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2547_112_2_18_2_53_14_299,(uint8*)"");
        return;
    }
    g_mmisms_global.save_info.dual_sys = dual_sys;
    g_mmisms_global.save_info.number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(dest_num_ptr->number_type, dest_num_ptr->number_plan);
    g_mmisms_global.save_info.number.number_len = dest_num_ptr->num_len;
    SCI_MEMCPY(g_mmisms_global.save_info.number.number, dest_num_ptr->party_num, dest_num_ptr->num_len);
}

/*****************************************************************************/
//     Description : to get the information of memory
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_GetSMSMemory(
                         MN_DUAL_SYS_E dual_sys,
                         BOOLEAN    is_nv,            //IN:
                         uint16        *max_num_ptr,    //OUT:
                         uint16        *used_num_ptr    //OUT:
                         )
{
    
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    
    if (PNULL == max_num_ptr || PNULL == used_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSMSMemory max_num_ptr used_num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2572_112_2_18_2_53_14_300,(uint8*)"");
        return;
    }
    storage = (is_nv) ? MN_SMS_STORAGE_ME : MN_SMS_STORAGE_SIM;    
    MNSMS_GetSmsNumEx(dual_sys, storage, max_num_ptr, used_num_ptr);
    
    //SCI_TRACE_LOW:"SMS: MMISMS_GetSMSMemory dual_sys=%d, is_nv = %d, max_num = %d, used_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2579_112_2_18_2_53_14_301,(uint8*)"dddd", dual_sys, is_nv, (*max_num_ptr), (*used_num_ptr));
}

/*****************************************************************************/
//     Description : to get the length according the alpha of message 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetLenByAlpha(
                                 MN_SMS_ALPHABET_TYPE_E    alpha,            //IN:
                                 uint8                     *split_len_ptr,    //OUT: the len to split message
                                 uint8                     *limit_len_ptr,    //OUT: the limit len
                                 BOOLEAN                   is_head_has_port
                                 )
{
    if (PNULL == split_len_ptr || PNULL == limit_len_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetLenByAlpha split_len_ptr num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2595_112_2_18_2_53_14_302,(uint8*)"");
        return;
    }
    // get the limited condition according to the alphabet
    switch (alpha)
    {
    case MN_SMS_UCS2_ALPHABET:  
        if (is_head_has_port)
        {
            (*limit_len_ptr) = MMISMS_VCARD_MAX_UCS2_BYTE_LEN;
            (*split_len_ptr) = MMISMS_VCARD_MAX_UCS2_SPLIT_LEN;
        }
        else
        {
            (*limit_len_ptr) = MMISMS_MAX_UCS2_BYTE_LEN;
            (*split_len_ptr) = MMISMS_MAX_UCS2_SPLIT_LEN;
        }
        break;
        
    case MN_SMS_DEFAULT_ALPHABET:
            if (is_head_has_port)
            {
                (*limit_len_ptr) = MMISMS_VCARD_MAX_DEF_BYTE_LEN;
                (*split_len_ptr) = MMISMS_VCARD_MAX_DEF_SPLIT_LEN;
            }
            else if(g_mmisms_global.is_r8)
            {
                if(g_mmisms_global.is_single_shift)
                {
                    (*limit_len_ptr) = MMISMS_R8_MAX_SINGLE_DEF_LEN;
                    (*split_len_ptr) = MMISMS_R8_MAX_SINGLE_SPLIT_LEN;
                }
                else
                {
                    (*limit_len_ptr) = MMISMS_R8_MAX_LOCK_DEF_LEN;
                    (*split_len_ptr) = MMISMS_R8_MAX_LOCK_SPLIT_LEN;
                }
            }
            else
            {
                (*limit_len_ptr) = MMISMS_MAX_DEF_BYTE_LEN;
                (*split_len_ptr) = MMISMS_MAX_DEF_SPLIT_LEN;
            }
        break;
        
    case MN_SMS_8_BIT_ALPHBET:
            if (is_head_has_port)
            {
                (*limit_len_ptr) = MMISMS_VCARD_MAX_8BIT_BYTE_LEN;
                (*split_len_ptr) = MMISMS_VCARD_MAX_8BIT_SPLIT_LEN_SAVE;
            }
            else
            {
                (*limit_len_ptr) = MMISMS_MAX_8BIT_BYTE_LEN;
                (*split_len_ptr) = MMISMS_MAX_8BIT_SPLIT_LEN;
            }
        break;
        
    default:
        //SCI_TRACE_LOW:"SMS: MMISMS_GetLenByAlpha alpha = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2654_112_2_18_2_53_14_303,(uint8*)"d", alpha);
        break;
    }
}



/*****************************************************************************/
//     Description : to get the user data which would send from sms_content_ptr
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetUserData(
                                uint8                    sms_index,        //IN: the index of SMS (1 ~ MMISMS_SPLIT_MAX_NUM) 
                                MMISMS_CONTENT_T            *sms_content_ptr, //IN: the content of need send
                                MN_SMS_USER_DATA_HEAD_T  *tpdu_head_ptr,   //IN/OUT: the head of user data 
                                BOOLEAN                  is_need_convert,
                                APP_SMS_USER_DATA_T      *user_data_ptr,    //OUT: the user data
                                MMISMS_SEND_T *send_info_ptr
                                )
{
    uint8 split_len = 0;
    uint8 limit_len = 0;
    uint8 msg_len = 0;
    uint16 start_pos = 0;
    int32 i = 0;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    
    if (PNULL == sms_content_ptr || PNULL == tpdu_head_ptr || PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetUserData sms_content_ptr tpdu_head_ptr user_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2683_112_2_18_2_53_14_304,(uint8*)"");
        return;
    }
    if (PNULL != send_info_ptr)
    {
        edit_content_ptr = &send_info_ptr->send_content;
    }
    else 
    {
        edit_content_ptr = &g_mmisms_global.edit_content;
    }
    // get the limit condition according to the alphabet
    if(MMISMS_IsR8Language(edit_content_ptr->send_message.language))       
    {
        g_mmisms_global.is_r8 = TRUE;
        g_mmisms_global.is_single_shift = edit_content_ptr->send_message.is_single_shift;
    }
    else
    {
        g_mmisms_global.is_r8 = FALSE;
        g_mmisms_global.is_single_shift = FALSE;
    }
    
    if (PNULL != send_info_ptr)
    {
        MMISMS_GetLenByAlpha(sms_content_ptr->alphabet, &split_len, &limit_len, send_info_ptr->is_head_has_port);
    }
    else
    {
        MMISMS_GetLenByAlpha(sms_content_ptr->alphabet, &split_len, &limit_len, FALSE);
    }

    g_mmisms_global.is_r8 = FALSE;
    
    if(MMISMS_IsR8Language(edit_content_ptr->send_message.language))
    {
        msg_len = (MMISMS_R8_MAX_NORMAL_HEAD_LEN < tpdu_head_ptr->length) ? split_len : limit_len;
    }
    else
    {
        msg_len = (1 < Get_Head_Max_Num(edit_content_ptr)) ? split_len : limit_len;
    }
    
    SCI_MEMSET(user_data_ptr, 0, sizeof(APP_SMS_USER_DATA_T) );
    
    // count the start pos
    //start_pos = (uint16)((sms_index - 1) * msg_len);
    for (i = 0; i < (int32)sms_index - 1; i++)
    {
        start_pos = start_pos + sms_content_ptr->split_sms_length[i];
    }
    if(start_pos > sms_content_ptr->length)
    {
        //SCI_TRACE_LOW:"SMS: MMISMS_GetUserData sms_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2736_112_2_18_2_53_14_305,(uint8*)"d", start_pos);
        return;
    }
    
    // get the length of user data
    user_data_ptr->user_valid_data_t.length = (uint8)MIN(msg_len, (sms_content_ptr->length - start_pos));
    
    if (((MN_SMS_DEFAULT_ALPHABET == sms_content_ptr->alphabet) ||
         (MN_SMS_8_BIT_ALPHBET == sms_content_ptr->alphabet))
         && (start_pos + user_data_ptr->user_valid_data_t.length > 0))
    {
        if (DEFAULT_EXT_MARK == sms_content_ptr->data[start_pos + user_data_ptr->user_valid_data_t.length - 1])
        {
            user_data_ptr->user_valid_data_t.length = user_data_ptr->user_valid_data_t.length - 1;
        }
    }
    sms_content_ptr->split_sms_length[sms_index - 1] = user_data_ptr->user_valid_data_t.length;
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetUserData: valid data len  is = %d,contect len = %d, msg len =%d = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2754_112_2_18_2_53_14_306,(uint8*)"dddd",user_data_ptr->user_valid_data_t.length ,sms_content_ptr->length,msg_len,start_pos);
    
    //copy the valid data to the param
    if(user_data_ptr->user_valid_data_t.length > MN_SMS_MAX_USER_VALID_DATA_LENGTH)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetUserData length = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2759_112_2_18_2_53_14_307,(uint8*)"d",user_data_ptr->user_valid_data_t.length);
        return;
    }
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    SCI_MEMCPY( 
        user_data_ptr->user_valid_data_t.user_valid_data_arr, 
        &(sms_content_ptr->data[start_pos]),
        user_data_ptr->user_valid_data_t.length 
        );
#else
    if ((MN_SMS_UCS2_ALPHABET == sms_content_ptr->alphabet)
        && is_need_convert
        )
    {
        GUI_UCS2B2UCS2L((uint8 *)user_data_ptr->user_valid_data_t.user_valid_data_arr,
                        user_data_ptr->user_valid_data_t.length ,
                        &(sms_content_ptr->data[start_pos]),
                        user_data_ptr->user_valid_data_t.length);
    }
    else
    {
        SCI_MEMCPY( 
            user_data_ptr->user_valid_data_t.user_valid_data_arr, 
            &(sms_content_ptr->data[start_pos]),
            user_data_ptr->user_valid_data_t.length
            );
    }
#endif 

    // get the information of user data
    // set the head of user data    
    if (0 < tpdu_head_ptr->length)
    {
        if(!MMISMS_IsR8Language(edit_content_ptr->send_message.language)
            || (MMISMS_IsR8Language(edit_content_ptr->send_message.language)
                 &&(MMISMS_R8_MAX_NORMAL_HEAD_LEN < edit_content_ptr->send_message.length)))
        {
            SET_CURRENT_SEQ_NUM(tpdu_head_ptr, sms_index);
        }
        
        if(tpdu_head_ptr->length > MN_SMS_USER_DATA_LENGTH)
        {
            //SCI_TRACE_LOW:"SMS: MMISMS_GetUserData tpdu_head_ptr->length = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2801_112_2_18_2_53_14_308,(uint8*)"d", tpdu_head_ptr->length);
            return;
        }        
        SCI_MEMCPY( 
            user_data_ptr->user_data_head_t.user_header_arr, 
            tpdu_head_ptr->user_header_arr,
            tpdu_head_ptr->length
            );
        
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetUserData: HEAD LEN is = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2810_112_2_18_2_53_14_309,(uint8*)"d",tpdu_head_ptr->length );
        
        user_data_ptr->user_data_head_t.length = tpdu_head_ptr->length;
    }
    else
    {
        user_data_ptr->user_data_head_t.length = 0;
    }
}
                    
/*****************************************************************************/
//     Description : to get a free record in ME or SIM
//    Global resource dependence : none
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMISMS_FindFreeRecordIdEx(
                                     MN_DUAL_SYS_E        dual_sys,  
                                     MN_SMS_STORAGE_E    storage,        //IN
                                     MN_SMS_RECORD_ID_T    *record_id1_ptr,    //OUT:
                                     MN_SMS_RECORD_ID_T    *record_id2_ptr    //OUT:
                                     )
{
    BOOLEAN result = FALSE;
    ERR_MNSMS_CODE_E err_code1 = ERR_MNSMS_NONE;
    ERR_MNSMS_CODE_E err_code2 = ERR_MNSMS_NONE;

    if (PNULL == record_id1_ptr || PNULL == record_id2_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_FindFreeRecordId record_id_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2838_112_2_18_2_53_14_310,(uint8*)"");
        return FALSE;
    }

    err_code1 = MNSMS_FindFreeRecordIdByIndex(dual_sys, storage, 0, record_id1_ptr);
    err_code2 = MNSMS_FindFreeRecordIdByIndex(dual_sys, storage, 1, record_id2_ptr);

    if (ERR_MNSMS_NONE == err_code1 &&  ERR_MNSMS_NONE == err_code2)
    {
        result = TRUE; 
    }

    //SCI_TRACE_LOW:"MMISMS: MMISMS_FindFreeRecordId storage = %d, record_id1 = %d ,record_id2 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2889_112_2_18_2_53_14_312,(uint8*)"dd", storage, *record_id1_ptr, *record_id2_ptr);

    return result;
}
                                 
/*****************************************************************************/
//     Description : to get a free record in ME or SIM
//    Global resource dependence : none
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMISMS_FindFreeRecordId(
                                     MN_DUAL_SYS_E        dual_sys,  
                                     MN_SMS_STORAGE_E    storage,        //IN
                                     MN_SMS_RECORD_ID_T    *record_id_ptr    //OUT:
                                     )
{
    BOOLEAN result = FALSE;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;

    if (PNULL == record_id_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_FindFreeRecordId record_id_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2838_112_2_18_2_53_14_310,(uint8*)"");
        return FALSE;
    }    
    if(dual_sys < MN_DUAL_SYS_MAX)
   {
        err_code = MNSMS_FindFreeRecordIdByIndex(dual_sys, storage, 0, record_id_ptr);
    }
    else
    {
        err_code = FALSE;
    }
    if (ERR_MNSMS_NONE == err_code)
    {
        result = TRUE; 
    }
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_FindFreeRecordId storage = %d, record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2889_112_2_18_2_53_14_312,(uint8*)"dd", storage, *record_id_ptr);

    return result;
}

/*****************************************************************************/
//     Description : to send saved request to MN
//    Global resource dependence : none
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_UpdateStatusReportMsgReq(                                            //RETURN:
                                            MN_DUAL_SYS_E            dual_sys,
                                            MN_SMS_STATUS_E        status,                //IN:
                                            MN_SMS_STORAGE_E        storage,            //IN:
                                            MN_SMS_RECORD_ID_T        record_id            //IN:
                                            )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    
    if (MN_SMS_STORAGE_ALL == storage)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_UpdateStatusReportMsgReq storage = MN_SMS_STORAGE_ALL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2910_112_2_18_2_53_14_313,(uint8*)"");
        return save_result;
    }    
    if (MN_SMS_NO_STORED != storage)
    {
        err_code = MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, status);
        
        //SCI_TRACE_LOW:"MMISMS: UpdateMsgReq err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2917_112_2_18_2_53_14_314,(uint8*)"d", err_code);
        
        if (ERR_MNSMS_NONE == err_code)
        {
            save_result = MMISMS_NO_ERR;
        }
        else
        {
            save_result = MMISMS_SAVE_FAIL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: UpdateMsgStatusReportReq the storage is MN_SMS_NO_STORED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2930_112_2_18_2_53_14_315,(uint8*)"");
    }
    
    return (save_result);
}

/*****************************************************************************/
//  Description : to saved request to MN
//  Global resource dependence : none
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_MOVESaveSms(                                            //RETURN:
                                            MMISMS_ORDER_ID_T order_id
                                            )
{
    uint16 num_len = 0;
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MN_SMS_STORAGE_E move_to_storage = MN_SMS_STORAGE_ME;
    APP_SMS_USER_DATA_T user_data = {0};
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
    uint8 total_num = 0;
    
    //SCI_TRACE_LOW:"MMISMS_MOVESaveSms:mo_mt_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_2951_112_2_18_2_53_14_316,(uint8*)"d",order_id->flag.mo_mt_type);
    SCI_MEMSET(&g_mmisms_global.save_info , 0, sizeof(MMISMS_SAVE_INFO_T));

    if( (MMISMS_MO_SEND_SUCC == order_id->flag.mo_mt_type) ||
        (MMISMS_MO_SEND_FAIL == order_id->flag.mo_mt_type) ||
        (MMISMS_MO_DRAFT == order_id->flag.mo_mt_type))
    {
        g_mmisms_global.read_msg.time = MMIAPICOM_Tm2Second(
                    g_mmisms_global.read_msg.time_stamp.second,
                    g_mmisms_global.read_msg.time_stamp.minute,
                    g_mmisms_global.read_msg.time_stamp.hour,
                    g_mmisms_global.read_msg.time_stamp.day,
                    g_mmisms_global.read_msg.time_stamp.month,
                    g_mmisms_global.read_msg.time_stamp.year + MMISMS_OFFSERT_YEAR);
    }
    else
    {
        g_mmisms_global.read_msg.time = MMIAPICOM_Tm2Second(
                    g_mmisms_global.read_msg.time_stamp.second,
                    g_mmisms_global.read_msg.time_stamp.minute,
                    g_mmisms_global.read_msg.time_stamp.hour,
                    g_mmisms_global.read_msg.time_stamp.day,
                    g_mmisms_global.read_msg.time_stamp.month,
                    g_mmisms_global.read_msg.time_stamp.year + MMISMS_MT_OFFSERT_YEAR);
    }
    g_mmisms_global.save_info.time = g_mmisms_global.read_msg.time;
    g_mmisms_global.save_info.is_locked = g_mmisms_global.read_msg.is_locked;

    if(MMISMS_MOVE_SMS_TO_SECURITY == MMISMS_GetCurrentOperStatus())
    {
        g_mmisms_global.save_info.folder_type = MMISMS_FOLDER_SECURITY;
        move_to_storage = MN_SMS_STORAGE_ME;
    }
    else if(MMISMS_MOVE_SMS_TO_ME == MMISMS_GetCurrentOperStatus())
    {
        move_to_storage = MN_SMS_STORAGE_ME;
    }
    else if(MMISMS_MOVE_SMS_TO_SIM == MMISMS_GetCurrentOperStatus())
    {
        g_mmisms_global.save_info.folder_type = MMISMS_FOLDER_NORMAL;
        g_mmisms_global.read_msg.dual_sys = MMISMS_GetCurSaveDualSys();
        move_to_storage = MN_SMS_STORAGE_SIM;
    }

    //init the variable
    SCI_MEMSET(&user_data, 0, sizeof (APP_SMS_USER_DATA_T));
    //g_mmisms_deliver.send_message = &g_mmisms_global.read_msg.read_content;
    total_num = MMISMS_SplitLongMsg( &g_mmisms_global.read_msg.read_content, FALSE);
    MMISMS_InitUserDataHead( 
        total_num, 
        1,            // current seq_num is first
        &g_mmisms_global.read_msg.head_data,
        PNULL
        );        

    MMISMS_GetReadUserData(
        1, 
        &g_mmisms_global.read_msg.read_content,
        &(g_mmisms_global.read_msg.head_data),
        FALSE,
        &user_data                            //OUT:
        );
    g_mmisms_global.save_info.storage = move_to_storage;       
    g_mmisms_global.save_info.max_num = 1;
    
    if (((user_data.user_data_head_t.length > 0) && !MMISMS_IsR8Language(g_mmisms_global.read_msg.language))
        || ((user_data.user_data_head_t.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(g_mmisms_global.read_msg.language)))
    {
        g_mmisms_global.save_info.max_num = user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
        if(MN_SMS_CONCAT_8_BIT_REF_NUM == user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_IEI_POS])
        {
            g_mmisms_global.save_info.ref_num = g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS];
        }
        else if(MN_SMS_CONCAT_16_BIT_REF_NUM == user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_IEI_POS])
        {
            g_mmisms_global.save_info.ref_num = (g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS]<<8) | 
                                                (g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS + 1]);
        }        
    }
    
    ret_val = SaveCopyMsgReq( 
                            g_mmisms_global.read_msg.dual_sys,
                            tele_num, 
                            num_len, 
                            &user_data, 
                            move_to_storage, 
                            g_mmisms_global.read_msg.read_content.alphabet
                            );
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_CopySaveSms ret_val = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3040_112_2_18_2_53_15_317,(uint8*)"d", ret_val);
    if (MMISMS_NO_ERR != ret_val)
    {
        ret_val = MMISMS_SAVE_FAIL;
    }
    
    return (ret_val);     
}

/*****************************************************************************/
//     Description : set next long sms order
//    Global resource dependence : g_mmisms_global
 //  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetsmsOrder(
                                    MMISMS_ORDER_ID_T order_id
                                    )
{
    s_mmisms_next_long_sms_ptr = order_id;
}

/*****************************************************************************/
//     Description : get next long sms order
//    Global resource dependence : g_mmisms_global
 //  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLongsmsOrder(void)
{
    return s_mmisms_next_long_sms_ptr;
}

/*****************************************************************************/
//  Description : to saved request to MN
//  Global resource dependence : none
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_CopySaveSms(                                            //RETURN:
                                            MMISMS_ORDER_ID_T order_id
                                            )
{
    uint16 num_len = 0;
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MN_SMS_STORAGE_E move_to_storage = MN_SMS_STORAGE_ME;
    APP_SMS_USER_DATA_T user_data = {0};
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
    uint8 total_num = 0;
    
    //SCI_TRACE_LOW:"MMISMS_MOVESaveSms:mo_mt_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3084_112_2_18_2_53_15_318,(uint8*)"d",order_id->flag.mo_mt_type);
    SCI_MEMSET(&g_mmisms_global.save_info , 0, sizeof(MMISMS_SAVE_INFO_T));

    if( (MMISMS_MO_SEND_SUCC == order_id->flag.mo_mt_type) ||
        (MMISMS_MO_SEND_FAIL == order_id->flag.mo_mt_type) ||
        (MMISMS_MO_DRAFT == order_id->flag.mo_mt_type))
    {
        g_mmisms_global.read_msg.time = MMIAPICOM_Tm2Second(
                    g_mmisms_global.read_msg.time_stamp.second,
                    g_mmisms_global.read_msg.time_stamp.minute,
                    g_mmisms_global.read_msg.time_stamp.hour,
                    g_mmisms_global.read_msg.time_stamp.day,
                    g_mmisms_global.read_msg.time_stamp.month,
                    g_mmisms_global.read_msg.time_stamp.year + MMISMS_OFFSERT_YEAR);
    }
    else
    {
        g_mmisms_global.read_msg.time = MMIAPICOM_Tm2Second(
                    g_mmisms_global.read_msg.time_stamp.second,
                    g_mmisms_global.read_msg.time_stamp.minute,
                    g_mmisms_global.read_msg.time_stamp.hour,
                    g_mmisms_global.read_msg.time_stamp.day,
                    g_mmisms_global.read_msg.time_stamp.month,
                    g_mmisms_global.read_msg.time_stamp.year + MMISMS_MT_OFFSERT_YEAR);
    }
    g_mmisms_global.save_info.time = g_mmisms_global.read_msg.time;
    g_mmisms_global.save_info.is_locked = g_mmisms_global.read_msg.is_locked;

    if(MMISMS_COPY_SMS_TO_SIM == MMISMS_GetCurrentOperStatus())
    {
        g_mmisms_global.save_info.folder_type = MMISMS_FOLDER_NORMAL;
        g_mmisms_global.read_msg.dual_sys = MMISMS_GetCurSaveDualSys();
        move_to_storage = MN_SMS_STORAGE_SIM;
    }

    //init the variable
    SCI_MEMSET(&user_data, 0, sizeof (APP_SMS_USER_DATA_T));
    //g_mmisms_deliver.send_message = &g_mmisms_global.read_msg.read_content;
    total_num = MMISMS_SplitLongMsg( &g_mmisms_global.read_msg.read_content, FALSE);
    MMISMS_InitUserDataHead( 
        total_num, 
        1,            // current seq_num is first
        &g_mmisms_global.read_msg.head_data,
        PNULL
        );        

    MMISMS_GetReadUserData(
        1, 
        &g_mmisms_global.read_msg.read_content,
        &(g_mmisms_global.read_msg.head_data),
        FALSE,
        &user_data                            //OUT:
        );
    g_mmisms_global.save_info.storage = move_to_storage;       
    g_mmisms_global.save_info.max_num = 1;
    
    if (((user_data.user_data_head_t.length > 0) && !MMISMS_IsR8Language(g_mmisms_global.read_msg.language))
        || ((user_data.user_data_head_t.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(g_mmisms_global.read_msg.language)))
    {
        g_mmisms_global.save_info.max_num = user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
        if(MN_SMS_CONCAT_8_BIT_REF_NUM == user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_IEI_POS])
        {
            g_mmisms_global.save_info.ref_num = g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS];
        }
        else if(MN_SMS_CONCAT_16_BIT_REF_NUM == user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_IEI_POS])
        {
            g_mmisms_global.save_info.ref_num = (g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS]<<8) | 
                                                (g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS + 1]);
        }        
    }
    
    ret_val = SaveCopyMsgReq( 
                            g_mmisms_global.read_msg.dual_sys,
                            tele_num, 
                            num_len, 
                            &user_data, 
                            move_to_storage, 
                            g_mmisms_global.read_msg.read_content.alphabet
                            );
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_CopySaveSms ret_val = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3164_112_2_18_2_53_15_319,(uint8*)"d", ret_val);
    if (MMISMS_NO_ERR != ret_val)
    {
        ret_val = MMISMS_SAVE_FAIL;
    }
    
    return (ret_val);
}

/*****************************************************************************/
//  Description : to send saved request to MN
//  Global resource dependence : none
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SaveCopyMsgReq(
                                       MN_DUAL_SYS_E            dual_sys,
                                       uint8                    *dest_addr_ptr,        //IN:
                                       uint8                    dest_len,            //IN:
                                       APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                       MN_SMS_STORAGE_E            storage,            //IN:
                                       MN_SMS_ALPHABET_TYPE_E    alphabet_type        //IN:
                                       )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MO_SMS_T mo_sms = {0};
    MN_SMS_MT_SMS_T mt_sms = {0};
    uint16 num_len = 0;
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;    

    if (PNULL == user_data_ptr || MN_SMS_STORAGE_ALL == storage)
    {
        return MMISMS_SAVE_FAIL;
    }

    num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(g_mmisms_global.read_msg.address.number_type),
                                         (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                         g_mmisms_global.read_msg.address.num_len),
                                         g_mmisms_global.read_msg.address.party_num,
                                         tele_num,
                                         (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                         );

    //get number
    if (num_len>0)
    {
        MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};

        if(MN_NUM_TYPE_ALPHANUMERIC == g_mmisms_global.read_msg.address.number_type)
        {
            SCI_MEMCPY(&mt_sms.origin_addr_t, &g_mmisms_global.read_msg.address,sizeof(MN_CALLED_NUMBER_T));
            SCI_MEMCPY(&mo_sms.dest_addr_t, &g_mmisms_global.read_msg.address,sizeof(MN_CALLED_NUMBER_T));
            
            g_mmisms_global.save_info.number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(g_mmisms_global.read_msg.address.number_type,g_mmisms_global.read_msg.address.number_plan);
            g_mmisms_global.save_info.number.number_len = num_len;
            SCI_MEMCPY(g_mmisms_global.save_info.number.number,tele_num,num_len);
        }
        else
        {
            MMISMS_GetMnCalledNum(tele_num, num_len, &party_number, (&mt_sms.origin_addr_t));
            MMISMS_GetMnCalledNum(tele_num, num_len, &party_number, (&mo_sms.dest_addr_t));
            g_mmisms_global.save_info.number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(party_number.number_type, MN_NUM_PLAN_UNKNOW);
            g_mmisms_global.save_info.number.number_len = party_number.num_len;
            SCI_MEMCPY(g_mmisms_global.save_info.number.number, party_number.bcd_num, party_number.num_len);
        }
    }

    g_mmisms_global.save_info.folder_type = g_mmisms_global.folder_type;
    
    if (MN_SMS_NO_STORED != storage)
    {   
        //save mt sms
        if(
            0   /*lint !e506*/
#ifdef MMI_SMS_COPY_SUPPORT 
           ||  (MMISMS_IsCopyOperation(MMISMS_GetCurrentOperStatus()))/*lint !e506*/
#endif       
#ifdef MMI_SMS_MOVE_SUPPORT 
           || (MMISMS_IsMoveOperation(MMISMS_GetCurrentOperStatus()))/*lint !e506*/
#endif           
           )/*lint !e774*/
        {

            SCI_TRACE_LOW("mmisms cur_type = %d,read_type = %d,read_msg.status = %d",g_mmisms_global.operation.cur_type,g_mmisms_global.read_msg.read_type,g_mmisms_global.read_msg.status);
            if( (
                (MMISMS_BOX_MT == MMISMS_GetCurOperationType()) 
#ifdef MMI_SMS_CHAT_SUPPORT
               ||(MMISMS_BOX_CHAT == MMISMS_GetCurOperationType())
#endif			   
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
               ||(MMISMS_BOX_SECURITY == MMISMS_GetCurOperationType())
#endif               
               )
               &&(MMISMS_READ_MTSMS == g_mmisms_global.read_msg.read_type))
            {
                mt_sms.dcs.alphabet_type = g_mmisms_global.read_msg.read_content.alphabet;
                SCI_MEMCPY(&mt_sms.time_stamp_t,&g_mmisms_global.read_msg.time_stamp,sizeof(MN_SMS_TIME_STAMP_T));
                MMISMS_EncodeMtSmsData( 
                    dual_sys,
                    &mt_sms,
                    alphabet_type,
                    user_data_ptr
                    );
                
                err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, g_mmisms_global.read_msg.status, &mt_sms);
            }
            //save mo sms
            else
            {
				mo_sms.sc_addr_present = TRUE;
                mo_sms.dest_addr_present = TRUE;
                mo_sms.dcs.alphabet_type = g_mmisms_global.read_msg.read_content.alphabet;
                SCI_MEMCPY(&mo_sms.time_stamp_t,&g_mmisms_global.read_msg.time_stamp,sizeof(MN_SMS_TIME_STAMP_T));
				cur_order_id = MMISMS_GetCurOperationOrderId();				
				if(MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type)
				{
					mo_sms.sc_addr_present = TRUE;
					mo_sms.sc_addr_t.num_len = 1;
				}
                MMISMS_EncodeMoSmsData( 
                    dual_sys,
                    &mo_sms,
                    alphabet_type,
                    user_data_ptr
                    );
                
                err_code = MNSMS_WriteMoSmsEx(dual_sys, storage, g_mmisms_global.read_msg.status, &mo_sms);
            }
        }        
        
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3287_112_2_18_2_53_15_320,(uint8*)"d", err_code);
        if (ERR_MNSMS_NONE == err_code)
        {
            save_result = MMISMS_NO_ERR;
        }
        else
        {
            save_result = MMISMS_SAVE_FAIL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3299_112_2_18_2_53_15_321,(uint8*)"");
        save_result = MMISMS_NO_SPACE;
    }
    
    return (save_result);
}

/*****************************************************************************/
//  Description : to copy the next SMS 
//  Global resource dependence : g_mmisms_global
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_CopyNextMessage(
                                                uint8   total_num,        //IN
                                                uint8   seq_num            //IN: the seq number ( 1 ~ 8)
                                                )
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
    APP_SMS_USER_DATA_T user_data = {0};

    MMISMS_GetReadUserData(
        seq_num, 
        &g_mmisms_global.read_msg.read_content,
        &(g_mmisms_global.read_msg.head_data),
        FALSE,
        &user_data                            //OUT:
        );
    
    ret_val = SaveCopyMsgReq( 
                            g_mmisms_global.read_msg.dual_sys,
                            PNULL, 
                            0, 
                            &user_data, 
                            g_mmisms_global.save_info.storage, 
                            g_mmisms_global.read_msg.read_content.alphabet
                            ); 
    
    //SCI_TRACE_LOW:"MMISMS_CopyNextMessage ret_val = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3335_112_2_18_2_53_16_322,(uint8*)"d", ret_val);
	return ret_val;
}

/*****************************************************************************/
//  Description : to check whether it has the next SMS to send according to the 
//                head of user data
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_IsExistNextCopySMS( void )
{
    BOOLEAN is_next_sms = FALSE;
    MN_SMS_USER_DATA_HEAD_T *head_ptr = &(g_mmisms_global.read_msg.head_data);
    uint8 i = 0;
    uint8 max_num = 0, seq_num = 0;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;
    
    MNSMS_DecodeUserDataHeadEx(dual_sys, head_ptr, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            max_num = decode_head.iei_arr[i].iei_data_t[1];
            seq_num = decode_head.iei_arr[i].iei_data_t[2];
            if (max_num > seq_num)
            {
                is_next_sms = TRUE;
                break;
            }
        }
    }
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);
    //SCI_TRACE_LOW:"SMS: IsExistNextSMS %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3370_112_2_18_2_53_16_323,(uint8*)"d", is_next_sms);
    
    return (is_next_sms);
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description :mms number to bec
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void MMSNumberToBcd(uint8 *sender,uint8 *number,uint8 *len)
{  
    if(PNULL == sender || 0 == strlen((char *)sender) || PNULL == number || PNULL == len )
    {
        return;
    }
    if(sender[0] == 0x68)
    {
        MMIAPICOM_StrToBcd( PACKED_LSB_FIRST,
                        (char *)&sender[1],
                        number);
        *len = (strlen((char *)sender) + 1)/2;                    
    }
    else
    {
        if(strlen((char *)sender) >=3 && (0x2B == sender[0]) &&
           (0x38 == sender[1]) &&
           (0x36 == sender[2]))
        {
            MMIAPICOM_StrToBcd( PACKED_LSB_FIRST,
                                (char *)&sender[3],
                                number);
            *len = (strlen((char *)&sender[3]) + 1)/2;
        }
        else
        {
            MMIAPICOM_StrToBcd( PACKED_LSB_FIRST,
                                (char *)&sender[0],
                                number);
            *len = (strlen((char *)&sender[0]) + 1)/2;                    
        }
    }
}
#endif

/*****************************************************************************/
// Description : Get current max number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Read_Head_Max_Num(void)
{
    uint8  i = 0;
    uint8 max_num = 0;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;
    
    if (((g_mmisms_global.read_msg.head_data.length > 0) && !MMISMS_IsR8Language(g_mmisms_global.read_msg.language))
        || ((g_mmisms_global.read_msg.head_data.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(g_mmisms_global.read_msg.language)))
    {
        max_num = 1;
    }
    MNSMS_DecodeUserDataHeadEx(dual_sys, &g_mmisms_global.read_msg.head_data, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            max_num = decode_head.iei_arr[i].iei_data_t[1];
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: Get_Current_Max_Num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3442_112_2_18_2_53_16_324,(uint8*)"d", max_num);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);

    return max_num;
}

/*****************************************************************************/
// Description : Get current max number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Read_Head_Ref_Num(void)
{ 
    uint8 i = 0;
    uint8 ref_num = 0;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;
    
    MNSMS_DecodeUserDataHeadEx(dual_sys, &g_mmisms_global.read_msg.head_data, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            ref_num = decode_head.iei_arr[i].iei_data_t[0];
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: Get_Current_Ref_Num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3470_112_2_18_2_53_16_325,(uint8*)"d", ref_num);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);

    return ref_num;
}

/*****************************************************************************/
// Description : Get current seq number
// Global resource dependence:
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 Get_Read_Current_Seq_Num(void)
{
    uint8 i = 0;
    uint8 seq_num = 1;
    uint8 iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;
    
    MNSMS_DecodeUserDataHeadEx(dual_sys, &g_mmisms_global.read_msg.head_data, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            seq_num = decode_head.iei_arr[i].iei_data_t[2];
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: Get_Current_Seq_Num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3498_112_2_18_2_53_16_326,(uint8*)"d", seq_num);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);

    return seq_num;
}

/*****************************************************************************/
//  Description : to get the user data which would send from sms_content_ptr
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetReadUserData(
                                    uint8                    sms_index,        //IN: the index of SMS (1 ~ MMISMS_SPLIT_MAX_NUM) 
                                    MMISMS_CONTENT_T            *sms_content_ptr, //IN: the content of need send
                                    MN_SMS_USER_DATA_HEAD_T  *tpdu_head_ptr,   //IN/OUT: the head of user data 
                                    BOOLEAN                  is_need_convert,
                                    APP_SMS_USER_DATA_T      *user_data_ptr    //OUT: the user data
                                    )
{
    uint8 split_len = 0;
    uint8 limit_len = 0;
    uint8 msg_len = 0;
    uint16 start_pos = 0;
    int32 i = 0;
    
    if (PNULL == sms_content_ptr || PNULL == tpdu_head_ptr || PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetReadUserData sms_content_ptr tpdu_head_ptr user_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3524_112_2_18_2_53_16_327,(uint8*)"");
        return;
    }    
    // get the limit condition according to the alphabet
    if(MMISMS_IsR8Language(g_mmisms_global.read_msg.language))       
    {
        g_mmisms_global.is_r8 = TRUE;
        g_mmisms_global.is_single_shift = g_mmisms_global.read_msg.is_single_shift;
    }
    else
    {
        g_mmisms_global.is_r8 = FALSE;
        g_mmisms_global.is_single_shift = FALSE;
    }
    
    MMISMS_GetLenByAlpha(sms_content_ptr->alphabet, &split_len, &limit_len, FALSE);

    g_mmisms_global.is_r8 = FALSE;
    
    if(MMISMS_IsR8Language(g_mmisms_global.read_msg.language))
    {
        msg_len = (MMISMS_R8_MAX_NORMAL_HEAD_LEN < tpdu_head_ptr->length) ? split_len : limit_len;
    }
    else
    {
        msg_len = (0 < tpdu_head_ptr->length) ? split_len : limit_len;
    }
    
    SCI_MEMSET(user_data_ptr, 0, sizeof(APP_SMS_USER_DATA_T) );
    
    // count the start pos
    //start_pos = (uint16)((sms_index - 1) * msg_len);
    for (i = 0; i < (int32)sms_index - 1; i++)
    {
        start_pos = start_pos + sms_content_ptr->split_sms_length[i];
    }
    if(start_pos > sms_content_ptr->length)
    {
        //SCI_TRACE_LOW:"SMS: MMISMS_GetReadUserData sms_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3562_112_2_18_2_53_16_328,(uint8*)"d", start_pos);
        return;
    }
    
    // get the length of user data
    user_data_ptr->user_valid_data_t.length = (uint8)MIN(msg_len, (sms_content_ptr->length - start_pos));
    
    if (((MN_SMS_DEFAULT_ALPHABET == sms_content_ptr->alphabet) ||
         (MN_SMS_8_BIT_ALPHBET == sms_content_ptr->alphabet))
         && (start_pos + user_data_ptr->user_valid_data_t.length > 0))
    {
        if (DEFAULT_EXT_MARK == sms_content_ptr->data[start_pos + user_data_ptr->user_valid_data_t.length - 1])
        {
            user_data_ptr->user_valid_data_t.length = user_data_ptr->user_valid_data_t.length - 1;
        }
    }
    sms_content_ptr->split_sms_length[sms_index - 1] = user_data_ptr->user_valid_data_t.length;
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetReadUserData: valid data len  is = %d,contect len = %d, msg len =%d = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3580_112_2_18_2_53_16_329,(uint8*)"dddd",user_data_ptr->user_valid_data_t.length ,sms_content_ptr->length,msg_len,start_pos);
    
    //copy the valid data to the param
    if(user_data_ptr->user_valid_data_t.length > MN_SMS_MAX_USER_VALID_DATA_LENGTH)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetReadUserData length = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3585_112_2_18_2_53_16_330,(uint8*)"d",user_data_ptr->user_valid_data_t.length);
        return;
    }
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    SCI_MEMCPY( 
        user_data_ptr->user_valid_data_t.user_valid_data_arr, 
        &(sms_content_ptr->data[start_pos]),
        user_data_ptr->user_valid_data_t.length 
        );
#else
    if ((MN_SMS_UCS2_ALPHABET == sms_content_ptr->alphabet)
        && is_need_convert
        )
    {
        GUI_UCS2B2UCS2L((uint8 *)user_data_ptr->user_valid_data_t.user_valid_data_arr,
                        user_data_ptr->user_valid_data_t.length ,
                        &(sms_content_ptr->data[start_pos]),
                        user_data_ptr->user_valid_data_t.length);
    }
    else
    {
        SCI_MEMCPY( 
            user_data_ptr->user_valid_data_t.user_valid_data_arr, 
            &(sms_content_ptr->data[start_pos]),
            user_data_ptr->user_valid_data_t.length
            );
    }
#endif 

    // get the information of user data
    // set the head of user data    
    if (0 < tpdu_head_ptr->length)
    {
        if(!MMISMS_IsR8Language(g_mmisms_global.read_msg.read_content.language)
            || (MMISMS_IsR8Language(g_mmisms_global.read_msg.read_content.language)
                 &&(MMISMS_R8_MAX_NORMAL_HEAD_LEN < g_mmisms_global.read_msg.read_content.length)))
        {
            SET_CURRENT_SEQ_NUM(tpdu_head_ptr, sms_index);
        }
        
        if(tpdu_head_ptr->length > MN_SMS_USER_DATA_LENGTH)
        {
            //SCI_TRACE_LOW:"SMS: MMISMS_GetReadUserData tpdu_head_ptr->length = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3627_112_2_18_2_53_16_331,(uint8*)"d", tpdu_head_ptr->length);
            return;
        }        
        SCI_MEMCPY( 
            user_data_ptr->user_data_head_t.user_header_arr, 
            tpdu_head_ptr->user_header_arr,
            tpdu_head_ptr->length
            );
        
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetReadUserData: HEAD LEN is = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_3636_112_2_18_2_53_16_332,(uint8*)"d",tpdu_head_ptr->length );
        
        user_data_ptr->user_data_head_t.length = tpdu_head_ptr->length;
    }
    else
    {
        user_data_ptr->user_data_head_t.length = 0;
    }
}

/*****************************************************************************/
//  Description : SMS������ȡ
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMISMS_GetSmsNumByStorage(MN_DUAL_SYS_E        dual_sys,      //IN
                                        MN_SMS_STORAGE_E    storage        //IN
                                        )
{
    uint16 max_sms_num = 0;
    uint16 used_sms_num = 0;

    MNSMS_GetSmsNumEx(dual_sys, storage, &max_sms_num, &used_sms_num);

    return used_sms_num;
}

//sms syn
#if defined(MMI_SMS_SYN_SUPPORT) || (defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT))
/*****************************************************************************/
//  Description : convert data string to int
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMISMS_StringToInt(uint8 *time, uint8 len)
{
    int i;
    uint16 value = 0;
    uint16 value_tmp = 0;
    uint8 len_tmp = 0;

    for (i = 0; i < len; i++)
    {
        value_tmp = time[i] - 48;

        len_tmp = len - i;

        while (--len_tmp)
        {
             value_tmp = value_tmp * 10;
        }

        value += value_tmp;
        
    }

    SCI_TRACE_LOW("MMISMS_StringToInt value = %d", value);

    return value;
}
#endif

#ifdef MMI_SMS_SYN_SUPPORT
/*****************************************************************************/
//  Description : show new sms come
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_ShowNewSms()
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 new_msg_num[6] = {0}; 
    MMI_STRING_T prompt_str = {0};
    MMI_STRING_T tmp_prompt = {0};
    uint32 time_out = 2000;
    MMI_WIN_ID_T alert_win_id = MMISMS_NEWMSG_WIN_ID;
    wchar disp_num[10] = {0};
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
    uint16 to_be_read_num = MMISMS_GetAllUnreadMsgCount();
    
    
    prompt_str.wstr_len = 0;
    prompt_str.wstr_ptr = PNULL;
    
    disp_num_len= sprintf((char *)new_msg_num, "%d", to_be_read_num);
    MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)new_msg_num, disp_num_len, disp_num_len);
    
    MMI_GetLabelTextByLang(TXT_SMS_NEW_MSG, &tmp_prompt);
    
    disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
    
    // allocate memory
    prompt_str.wstr_ptr = SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
    SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
    
    MMI_WSTRNCPY(prompt_str.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);
    
    prompt_str.wstr_len = disp_num_len;
    
    // copy the prompt string
    MMI_WSTRNCPY(
        &(prompt_str.wstr_ptr[prompt_str.wstr_len]),
        tmp_prompt.wstr_len,
        tmp_prompt.wstr_ptr, 
        tmp_prompt.wstr_len,
        tmp_prompt.wstr_len
        );
    prompt_str.wstr_len += tmp_prompt.wstr_len;
    
    
    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,PNULL);
    
    if (PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : show syn sms result
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_ShowSynSmsResult()
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 syn_msg_num[6] = {0}; 
    MMI_STRING_T prompt_str = {0};
    MMI_STRING_T tmp_prompt = {0};
    uint32 time_out = 2000;
    MMI_WIN_ID_T alert_win_id = MMISMS_SYNMSG_WIN_ID;
    wchar disp_num[10] = {0};
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
    
    
    prompt_str.wstr_len = 0;
    prompt_str.wstr_ptr = PNULL;

    if (syn_sms_total > 0 || total_unread != 0 || total_read != 0)
    {
//         disp_num_len= sprintf((char *)syn_msg_num, "%d", syn_sms_total);
//         
//         syn_sms_total = 0;
//         
//         MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)syn_msg_num, disp_num_len, disp_num_len);
//         
//         MMI_GetLabelTextByLang(TXT_SMS_SYN_MSG, &tmp_prompt);
//         
//         disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
//         
//         // allocate memory
//         prompt_str.wstr_ptr = SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
//         SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
//         
//         MMI_WSTRNCPY(prompt_str.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);
//         
//         prompt_str.wstr_len = disp_num_len;
//         
//         // copy the prompt string
//         MMI_WSTRNCPY(
//             &(prompt_str.wstr_ptr[prompt_str.wstr_len]),
//             tmp_prompt.wstr_len,
//             tmp_prompt.wstr_ptr, 
//             tmp_prompt.wstr_len,
//             tmp_prompt.wstr_len
//             );
//         prompt_str.wstr_len += tmp_prompt.wstr_len;
//         
//         MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,PNULL);

        MMI_GetLabelTextByLang(TXT_SMS_SYNC_FINISHED, &tmp_prompt);
        
        disp_str_len = tmp_prompt.wstr_len + 1;
        
        // allocate memory
        prompt_str.wstr_ptr = SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
        SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        // copy the prompt string
        MMI_WSTRNCPY(
            prompt_str.wstr_ptr,
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr, 
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
            );
        prompt_str.wstr_len += tmp_prompt.wstr_len;
        
        MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,PNULL);
    } 
    else
    {
        MMI_GetLabelTextByLang(TXT_SMS_SYN_MSG_EMPETY, &tmp_prompt);

        disp_str_len = tmp_prompt.wstr_len + 1;
        
        // allocate memory
        prompt_str.wstr_ptr = SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
        SCI_MEMSET(prompt_str.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        // copy the prompt string
        MMI_WSTRNCPY(
            prompt_str.wstr_ptr,
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr, 
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
            );
        prompt_str.wstr_len += tmp_prompt.wstr_len;
        
        MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,PNULL);

    }

    if (PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr = PNULL;
    }  
}

/*****************************************************************************/
//  Description : send request
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
PUBLIC void MMISMS_SendRequestCom(const uint8 *str_com, uint8 str_len)
{
    SCI_TRACE_LOW("SEND COMMAND: %s, %d", str_com, str_len);
    
    MMIAPIBT_SppWriteData(str_com, str_len);
    
}

/*****************************************************************************/
//  Description : save long sms content
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_SaveMsgReq(
                                           MN_DUAL_SYS_E            dual_sys,
                                           uint8                    *dest_addr_ptr,        //IN:
                                           uint8                    dest_len,            //IN:
                                           APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                           MN_SMS_STORAGE_E            storage,            //IN:
                                           MN_SMS_ALPHABET_TYPE_E    alphabet_type        //IN:
                                           )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MT_SMS_T mt_sms = {0};
//     SCI_TIME_T time = {0};
//     SCI_DATE_T date = {0};
    MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
    uint8 num_len = 0;
    uint8 i = 0;
    
    if (PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq user_data_ptr  = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_178_112_2_18_2_53_9_252,(uint8*)"");
        return save_result;
    }
    if (MN_SMS_STORAGE_ALL == storage)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq storage  = MN_SMS_STORAGE_ALL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_183_112_2_18_2_53_9_253,(uint8*)"");
        return save_result;
    }

    while(number[i++] != 32)
        num_len++;
    
    if (MN_SMS_NO_STORED != storage)
    {
        SCI_MEMSET(&mt_sms, 0, sizeof(MN_SMS_MT_SMS_T));
        
        //GUI_UCS2L2UCS2B(data_big, 4, data_litter, 4);

        MMISMS_GetMnCalledNum(number, num_len, &party_number, (&mt_sms.origin_addr_t)); 
            
        g_mmisms_global.save_info.number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(party_number.number_type, MN_NUM_PLAN_UNKNOW);
        g_mmisms_global.save_info.number.number_len = party_number.num_len;
        SCI_MEMCPY(g_mmisms_global.save_info.number.number, party_number.bcd_num, party_number.num_len);
        
        
        g_mmisms_global.save_info.folder_type = g_mmisms_global.folder_type;

        mt_sms.dcs.alphabet_type = 2;
        
        mt_sms.time_stamp_t.day = MMISMS_StringToInt(&time_date[6], 2);
        mt_sms.time_stamp_t.hour = MMISMS_StringToInt(&time_date[8], 2);
        mt_sms.time_stamp_t.minute = MMISMS_StringToInt(&time_date[10], 2);
        mt_sms.time_stamp_t.month = MMISMS_StringToInt(&time_date[4], 2);
        mt_sms.time_stamp_t.second = MMISMS_StringToInt(&time_date[12], 2);
        mt_sms.time_stamp_t.timezone = 0;    //������
        mt_sms.time_stamp_t.year = MMISMS_StringToInt(time_date, 4) - 48;  
        
   
        MMISMS_EncodeMtSmsData( 
            dual_sys,
            &mt_sms,
            alphabet_type,
            user_data_ptr
            );
      
        MMISMS_SetOperStatus(MMISMS_SAVE_SYN_SMS);

        if (total_unread != 0)
        {
            err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, MN_SMS_MT_TO_BE_READ, &mt_sms);
        } 
        else
        {
            err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, MN_SMS_MT_READED, &mt_sms);
        }
        
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_249_112_2_18_2_53_9_254,(uint8*)"d", err_code);
        if (ERR_MNSMS_NONE == err_code)
        {
            save_result = MMISMS_NO_ERR;
        }
        else
        {
            save_result = MMISMS_SAVE_FAIL;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_261_112_2_18_2_53_9_255,(uint8*)"");
        save_result = MMISMS_NO_SPACE;
    }
    
    return (save_result);
}

/*****************************************************************************/
//  Description : save long sms content
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_SaveLongSmsNextContent(MN_DUAL_SYS_E dual_sys,
                                                       uint8    *dest_addr_ptr,    //IN:
                                                       uint8   total_num,        //IN
                                                       uint8   seq_num            //IN: the seq number ( 1 ~ 8)
                                                      )
{
    APP_SMS_USER_DATA_T user_data = {0};
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MMISMS_OPER_ERR_E ret_val = MMISMS_NO_ERR;
    uint8 addr_len = 0;
    MN_SMS_RECORD_ID_T record_id = 0;
    uint8 id = 0;
    BOOLEAN is_find_free_record = FALSE;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
    int i = 0;
    

    edit_content_ptr = &g_mmisms_global.edit_content;
    
   
    if(1 == seq_num )
    {
        MMISMS_GetStoreStorageForSendLongSms(dual_sys, total_num, seq_num, &storage);
        g_mmisms_global.save_info.storage = storage;
    }
    else
    {
        storage = g_mmisms_global.save_info.storage;
        id = edit_content_ptr->dest_info.dest_list.dest_addr_id;
        dest_addr_ptr = edit_content_ptr->dest_info.dest_list.dest_addr[id];
    }
     
    is_find_free_record = MMISMS_FindFreeRecordId(dual_sys, storage, &record_id);
    
    if(!is_find_free_record)
    {
        storage = MN_SMS_NO_STORED;
    }
    
    if (MN_SMS_NO_STORED != storage)
    {
        //init the variable
        SCI_MEMSET(&user_data, 0, sizeof (APP_SMS_USER_DATA_T));
        // get the user data
        MMISMS_GetUserData(
            seq_num, 
            &(edit_content_ptr->send_message),
            &(edit_content_ptr->tpdu_head),
            TRUE,
            &user_data,                            //OUT:
            PNULL
            );
        
        g_mmisms_global.save_info.max_num = 1;
        
        if (((user_data.user_data_head_t.length > 0) && !MMISMS_IsR8Language(edit_content_ptr->send_message.language))
            || ((user_data.user_data_head_t.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(edit_content_ptr->send_message.language)))
        {
            g_mmisms_global.save_info.max_num = user_data.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
            g_mmisms_global.save_info.ref_num = GET_CURRENT_REF_NUM(edit_content_ptr);
        }
        
        if (PNULL != dest_addr_ptr)
        {
            addr_len = strlen((char*)dest_addr_ptr);
        }
        
        ret_val = MMISMS_SaveMsgReq( 
            dual_sys,
            dest_addr_ptr, 
            addr_len, 
            &user_data, 
            storage, 
            edit_content_ptr->send_message.alphabet 
            );
    }
    else
    {
        ret_val = MMISMS_NO_SPACE;
        //prompt: no free space
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE, PNULL);
    }

    return ret_val;
    
}

/*****************************************************************************/
//  Description : save long sms content
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_SaveLongSmsContent(uint8 *sms_content, uint16 con_len)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 total_num = 1;
    uint8 num_len = 0;
    uint8 i = 0;
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;

    while(number[i++] != 32)
        num_len++;


    //set message content to global
    SCI_MEMSET(&g_mmisms_global.edit_content.tpdu_head, 0, sizeof(MN_SMS_USER_DATA_HEAD_T));

    g_mmisms_global.edit_content.send_message.alphabet = 2; 
    
    SCI_MEMCPY(g_mmisms_global.edit_content.send_message.data, sms_content, con_len);

    g_mmisms_global.edit_content.send_message.length = con_len;
    
    SCI_MEMCPY(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0], number, num_len);



    SCI_MEMSET(&g_mmisms_global.save_info, 0, sizeof(MMISMS_SAVE_INFO_T));
    g_mmisms_global.save_info.dual_sys = dual_sys;

    total_num = MMISMS_SplitLongMsg(&g_mmisms_global.edit_content.send_message, FALSE);

    MMISMS_InitUserDataHead(total_num, 1, &g_mmisms_global.edit_content.tpdu_head, PNULL);

    save_result = MMISMS_SaveLongSmsNextContent(dual_sys, g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0], total_num, 1);

    return save_result;
}


/*****************************************************************************/
//  Description : save sms content
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_SaveSmsContent(uint8 *sms_content, uint16 con_len)
{
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T record_id = 0;
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

    MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};

    MN_SMS_MT_SMS_T mt_sms = {0};

    uint8 num_len = 0;
    uint8 i = 0;
    
       
    //char content_data[MMISMS_MAX_MESSAGE_LEN + 1];

    uint8 convert_data[MMISMS_MAX_MESSAGE_LEN + 1];

    uint8 save_data[MMISMS_MAX_MESSAGE_LEN + 1];

    //uint16 length = content_len * 2; 


    save_data[0] = con_len;

    while(number[i++] != 32)
        num_len++;

    //save global varites
    g_mmisms_global.edit_content.send_message.alphabet = 2;

    SCI_MEMCPY(g_mmisms_global.edit_content.send_message.data, sms_content, con_len);

    SCI_MEMCPY(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0], number, num_len);


    //get storage and record_id
    MMISMS_GetStoreStorage(dual_sys, &storage, &record_id);

    SCI_TRACE_LOW("MMISMS: storage = %d, record_id = %d", storage, record_id);

    //little to big
    GUI_UCS2B2UCS2L(convert_data, con_len, sms_content, con_len);

    //
    SCI_MEMCPY(&save_data[1], convert_data, con_len);

    if (MN_SMS_NO_STORED != storage)
    {
        
        SCI_MEMSET(&mt_sms, 0, sizeof(MN_SMS_MT_SMS_T));
        

        mt_sms.user_data_t.length = save_data[0] + 1;
        
        SCI_MEMCPY(mt_sms.user_data_t.user_data_arr, save_data, mt_sms.user_data_t.length);   

        MMISMS_GetMnCalledNum(number, num_len, &party_number, (&mt_sms.origin_addr_t));            
        
        mt_sms.dcs.alphabet_type = 2;

        mt_sms.time_stamp_t.day = MMISMS_StringToInt(&time_date[6], 2);
        mt_sms.time_stamp_t.hour = MMISMS_StringToInt(&time_date[8], 2);
        mt_sms.time_stamp_t.minute = MMISMS_StringToInt(&time_date[10], 2);
        mt_sms.time_stamp_t.month = MMISMS_StringToInt(&time_date[4], 2);
        mt_sms.time_stamp_t.second = MMISMS_StringToInt(&time_date[11], 2);
        mt_sms.time_stamp_t.timezone = 0;    //������
        mt_sms.time_stamp_t.year = MMISMS_StringToInt(time_date, 4) - 48;

        if (total_unread != 0)
        {
            err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, MN_SMS_MT_TO_BE_READ, &mt_sms);
        } 
        else
        {
            err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, MN_SMS_MT_READED, &mt_sms);
        }
        
        
        MMISMS_SetOperStatus(MMISMS_SAVE_SYN_SMS);
        
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_329_112_2_18_2_53_9_258,(uint8*)"d", err_code);
        if (ERR_MNSMS_NONE == err_code)
        {
            save_result = MMISMS_NO_ERR;
        }
        else
        {
            save_result = MMISMS_SAVE_FAIL;
        }
        
    } 
    else
    {
        //SCI_TRACE_LOW:"MMISMS: SaveMsgReq the storage is MN_SMS_NO_STORED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_341_112_2_18_2_53_9_259,(uint8*)"");
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE, PNULL);
        save_result = MMISMS_NO_SPACE;
    }

    return save_result;
    
    //
    }
/*****************************************************************************/
//  Description : load unread sms from the other android phone
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_LoadUnreadSMS()
{   
    uint8 comm[11] = {65,84,77,82,78,72,44};  //ATMRNH

    uint8 str_com[9] ={65,84,77,82,79,84,13,10,0}; //ATMROT 0d 0a

    uint8 sms_seq[2];
    
    uint8 high_byte = 0;
    
    uint8 low_byte = 0;

    uint8 bt_end[2] = {13,10};
    

    if (0 == total_unread)
    {
       MMISMS_SendRequestCom(str_com, 8);   // 
    }
    else
    {
       syn_unread = TRUE;

       if (total_unread > 10)
       {
           high_byte = (total_unread - 1)/10 + 48;
           
           low_byte = (total_unread - 1)%10 + 48;
           
           sms_seq[0] = high_byte;
           
           sms_seq[1] = low_byte;
           
           SCI_MEMCPY(&comm[7], sms_seq, 2);
           
           SCI_MEMCPY(&comm[9], bt_end, 2);
           
           MMISMS_SendRequestCom(comm, 11);
       }
       else
       {
           sms_seq[0] = total_unread - 1 + 48;
            
           SCI_MEMCPY(&comm[7], sms_seq, 1);
            
           SCI_MEMCPY(&comm[8], bt_end, 2);
            
           MMISMS_SendRequestCom(comm, 10);   //
       }
             
    } 
    
}

/*****************************************************************************/
//  Description : load have read sms from the other android phone
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_LoadReadSMS()
{
    uint8 comm[11] = {65,84,77,82,79,72,44};  //ATMROH
    
    uint8 sms_seq[2];

    uint8 high_byte = 0;

    uint8 low_byte = 0;

    uint8 bt_end[2] = {13, 10};

    if (0 != total_read)
    {
        if (total_read > 10)
        {
            high_byte = (total_read - 1)/10 + 48;

            low_byte = (total_read - 1)%10 + 48;

            sms_seq[0] = high_byte;

            sms_seq[1] = low_byte;
            
            SCI_MEMCPY(&comm[7], sms_seq, 2);

            SCI_MEMCPY(&comm[9], bt_end, 2);

            MMISMS_SendRequestCom(comm, 11);
        }
        else
        {
            sms_seq[0] = total_read - 1 + 48;
            
            SCI_MEMCPY(&comm[7], sms_seq, 1);
            
            SCI_MEMCPY(&comm[8], bt_end, 2);
            
            MMISMS_SendRequestCom(comm, 10);
        }        

    } 
    else
    {
        MMK_CloseWin(MMIBT_SYNC_SMS_WAIT_WIN_ID);

        MMISMS_ShowSynSmsResult();

        SCI_TRACE_LOW("INBOX IS NOT HAVE SMS");
    }
    //  
}

/*****************************************************************************/
//  Description : parse sms head
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_ParseSmsHead(uint8 *data)
{
    uint8 *data_ptr = PNULL;
    uint8 current_number[2];
    uint8 i = 0;
    uint8 bt_end[2] = {13, 10};

    uint8 comm_unread[11] = {65,84,77,82,78,67,44};   //ATMRNC
    uint8 sms_seq_unread[2];

    uint8 comm_read[11] = {65,84,77,82,79,67,44};     //ATMROC
    uint8 sms_seq_read[2];

    uint8 high_byte = 0;
    uint8 low_byte = 0;


    data_ptr = data;

    if (total_read > 10 || total_unread > 10)
    {
        current_number[0] = *data_ptr++;
        current_number[1] = *data_ptr++;
    } 
    else
    {
        current_number[0] = *data_ptr++;
    }

    data_ptr++;

    while(*data_ptr != 44)
        time_date[i++] = *data_ptr++;

    data_ptr++;

    i = 0;
    while(*data_ptr != 44)
        number[i++] = *data_ptr++;

    number[i] = 32;


    if (0 != total_unread)
    {   
        if (total_unread > 10)
        {
            high_byte = (total_unread - 1)/10 + 48;
            
            low_byte = (total_unread - 1)%10 + 48;
            
            sms_seq_unread[0] = high_byte;
            
            sms_seq_unread[1] = low_byte;

            SCI_MEMCPY(&comm_unread[7], sms_seq_unread, 2); 
            
            SCI_MEMCPY(&comm_unread[9], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_unread, 11);
        } 
        else
        {
            sms_seq_unread[0] = total_unread - 1 + 48;
            
            SCI_MEMCPY(&comm_unread[7], sms_seq_unread, 1); 
            
            SCI_MEMCPY(&comm_unread[8], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_unread, 10);
        }
        
    } 
    else if (0 != total_read)
    {
        if (total_read > 10)
        {
            high_byte = (total_read - 1)/10 + 48;
            
            low_byte = (total_read - 1)%10 + 48;
            
            sms_seq_read[0] = high_byte;
            
            sms_seq_read[1] = low_byte;
            
            SCI_MEMCPY(&comm_read[7], sms_seq_read, 2);
            
            SCI_MEMCPY(&comm_read[9], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_read, 11);
        }
        else
        {
            sms_seq_read[0] = total_read - 1 + 48;
            
            SCI_MEMCPY(&comm_read[7], sms_seq_read, 1); 
            
            SCI_MEMCPY(&comm_read[8], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_read, 10);
        }
        
    } 
    else if(newsms_flag)
    {
        sms_seq_unread[0] = newsms_seq;
        
        SCI_MEMCPY(&comm_unread[7], sms_seq_unread, 1); 
        
        SCI_MEMCPY(&comm_unread[8], bt_end, 2);
        
        MMISMS_SendRequestCom(comm_unread, 10);
    }  
    //
}
/*****************************************************************************/
//  Description : parse sms content
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_ParseSmsContent(uint8 *data)
{
    uint8 *data_ptr = PNULL;
    uint8 current_number[2];
    uint8 current_package;
    uint8 bt_end[2] = {13, 10};
    uint8 flag_end =0;

    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;


    SCI_TRACE_LOW("MMISMS: Enter into MMISMS_ParseSmsContent");

    data_ptr = data;
    
    if (total_read >= 10 || total_unread >= 10)
    {
        current_number[0] = *data_ptr++;
        current_number[1] = *data_ptr++;
             //current seq sms
    } 
    else
    {
        current_number[0] = *data_ptr++;
             //current seq sms
    }
    
    data_ptr++;

    current_package = *data_ptr++;
    data_ptr++;      // current sms package seq

    flag_end = *data_ptr++;
    data_ptr++;      // package is end


    if (flag_end == 48) //long sms
    {

        SCI_TRACE_LOW("MMISMS: this is long sms");

        long_sms_flag = TRUE;

        while((*data_ptr != 0x0d || *(data_ptr + 1) != 0x0a) && con_len <= MMISMS_MAX_MESSAGE_LEN)
        {  
            sms_content[con_len] = *data_ptr++;

            con_len++;
        }
        
        return;
    }
    else
    {
        while((*data_ptr != 0x0d || *(data_ptr + 1) != 0x0a) && con_len <= MMISMS_MAX_MESSAGE_LEN)
        {               
            sms_content[con_len] = *data_ptr++;

            con_len++;
        }

    }

    SCI_TRACE_LOW("MMISMS: CONTENT_LEN = %d", con_len);
    

    long_sms_flag = FALSE;

    if (syn_unread || newsms_flag)
    {
         sms_unread_flag = TRUE;              
    }
    else
    {
         sms_unread_flag = FALSE;
    }

    syn_sms_total++;

    if (con_len > 536)
    {
         con_len = 536;

         SCI_TRACE_LOW("MMISMS: SMS CONTETN length is larger than 536");
    }

    save_result = MMISMS_SaveLongSmsContent(sms_content, con_len);

    con_len = 0;

    if (MMISMS_NO_SPACE == save_result)
    {
        Syn_end = TRUE;

        syn_unread = FALSE;

        total_read = 0;

        total_unread = 0;

        syn_sms_total--;

        MMISMS_ClearEditContent();

        MMK_CloseWin(MMIBT_SYNC_SMS_WAIT_WIN_ID);

        //MMISMS_ShowSynSmsResult();

        SCI_TRACE_LOW("MMISMS: MEM is full!!!");

        return;
    }

}

/*****************************************************************************/
//  Description : send syn next sms request
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_SendNextSmsRequest()
{
    uint8 bt_end[2] = {13, 10};
    
    uint8 str_com[9] ={65,84,77,82,79,84,13,10,0}; //ATMROT 0d 0a
    
    uint8 comm_unread[11] = {65,84,77,82,78,72,44};    //ATMRNH
    uint8 sms_seq_unread[2];
    
    uint8 comm_read[11] = {65,84,77,82,79,72,44};     //ATMROH
    uint8 sms_seq_read[2];
    
    uint8 high_byte = 0;
    uint8 low_byte = 0;


    if (0 == total_unread && syn_unread)
    {
        MMISMS_SendRequestCom(str_com, 8);

        syn_unread = FALSE;

        return;
    }

    if (0 != total_unread)
    {       
        if (total_unread > 10)
        {
            high_byte = (total_unread - 1)/10 + 48;
            
            low_byte = (total_unread - 1)%10 + 48;
            
            sms_seq_unread[0] = high_byte;
            
            sms_seq_unread[1] = low_byte;
            
            SCI_MEMCPY(&comm_unread[7], sms_seq_unread, 2);
            
            SCI_MEMCPY(&comm_unread[9], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_unread, 11);
        } 
        else
        {
            sms_seq_unread[0] = total_unread - 1 + 48;
            
            SCI_MEMCPY(&comm_unread[7], sms_seq_unread, 1);
            
            SCI_MEMCPY(&comm_unread[8], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_unread, 10);
        }
        
    } 
    else if (0 != total_read)
    {
        if (total_read > 10)
        {
            high_byte = (total_read - 1)/10 + 48;
            
            low_byte = (total_read - 1)%10 + 48;
            
            sms_seq_read[0] = high_byte;
            
            sms_seq_read[1] = low_byte;
            
            SCI_MEMCPY(&comm_read[7], sms_seq_read, 2);
            
            SCI_MEMCPY(&comm_read[9], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_read, 11);
        }
        else
        {
            sms_seq_read[0] = total_read - 1 + 48;
            
            SCI_MEMCPY(&comm_read[7], sms_seq_read, 1); 
            
            SCI_MEMCPY(&comm_read[8], bt_end, 2);
            
            MMISMS_SendRequestCom(comm_read, 10);   
        }
        
    }
    else
    {
        total_read = 0;

        total_unread = 0;

        Syn_end = TRUE;

        MMK_CloseWin(MMIBT_SYNC_SMS_WAIT_WIN_ID);

        if (newsms_flag)
        {
            MMISMS_ShowNewSms();

            newsms_flag = FALSE;
        } 
        else
        {
            MMISMS_ShowSynSmsResult();
        }

        SCI_TRACE_LOW("SYN SMS END");
    }
     
}

/*****************************************************************************/
//  Description : show send result
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/

LOCAL void MMISMS_OpenSendResultWin(uint8 result)
{
    if (0 == result)
    {
        MMISMS_OpenMsgBox(TXT_COMM_SEND_SUCCEED, IMAGE_PUBWIN_SUCCESS); //success
    } 
    else
    {
        MMISMS_OpenMsgBox(TXT_COMM_SEND_FAILED, IMAGE_PUBWIN_WARNING); //fail
    }

    MMISMS_ClearEditContent();
    MMISMS_ClearDestNum();

}

/*****************************************************************************/
//  Description : syn new come sms
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL void MMISMS_SynNewComeSms()
{ 
    uint8 comm[10] = {65,84,77,82,78,72,44}; //ATMRNH

    uint8 sms_seq[1]; 

    uint8 bt_end[2] = {13, 10};
    
    sms_seq[0] = newsms_seq;
    
    SCI_MEMCPY(&comm[7], sms_seq, 1);
    
    SCI_MEMCPY(&comm[8], bt_end, 2);

    newsms_flag = TRUE;
    
    MMISMS_SendRequestCom(comm, 10);   //
}

/*****************************************************************************/
//  Description : parse bluetooth data package
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
PUBLIC void MMISMS_GetSmsData(const uint8 *buff)
{
    uint8 *data_ptr;
    uint8 i;
    uint8 at_command[5];
    uint8 command_index = 9;
    uint8 send_result = 0;
    uint8 sms_count[3];


    data_ptr = buff;

    data_ptr++;

    for (i = 0; i < 5; i++)
    {
        at_command[i] = *data_ptr++;        
    }

    SCI_TRACE_LOW("at_command: %s", at_command);

    for (i = 0; i < COMMAND_TOTAL; i++)
    {

        if(!MMIAPICOM_StrCmp(at_command, 5, command[i], 5))
        {
            command_index = i;
            
            SCI_TRACE_LOW("SMS COMMAND INDEX: %d", i);

            break;
        }
       
    }

    i = 0;
    data_ptr++; //, equal 2c

    switch (command_index)
    {
    case IMRNT:

        if (!Syn_end)
        {
            while (*data_ptr != 0x0d && *(data_ptr + 1) != 0x0a)
            {
                sms_count[i++] = *data_ptr++;
            }
            
            total_unread = MMISMS_StringToInt(sms_count, i);

            syn_sms_total = 0;
            
            SCI_TRACE_LOW("SMS total_unread: %d ", total_unread);

            if (MMIAPIBT_GetSmsSynVal())
            {
                MMISMS_LoadUnreadSMS();  
            }
            else
            {
                SCI_TRACE_LOW("MMISMS: Not Start Syn sms");
            }     
        }

    	break;

    case IMROT:

        while (*data_ptr != 0x0d && *(data_ptr + 1) != 0x0a)
        {
            sms_count[i++] = *data_ptr++;
        }
        
        total_read = MMISMS_StringToInt(sms_count, i);

        SCI_TRACE_LOW("SMS total_read = %d, syn_sms_total = %d", total_read, syn_sms_total);

        if ((total_read + syn_sms_total) > 100)
        {
            total_read = 100 - syn_sms_total;
        }

        SCI_TRACE_LOW("SMS total_read: %d ", total_read);

        MMISMS_LoadReadSMS();

        break;

    case IMRNH:        

        MMISMS_ParseSmsHead(data_ptr);

        break;

    case IMRNC:

        if (!newsms_flag)
        {
            total_unread--;
        }     

        MMISMS_ParseSmsContent(data_ptr);

        break;

    case IMROH:
    
        MMISMS_ParseSmsHead(data_ptr);
            
        break;
           
    case IMROC:

        if (!long_sms_flag)
        {
            total_read--;   //long sms is not need deincreased
        }        

        MMISMS_ParseSmsContent(data_ptr);
            
        break;

    case IMSRT:
        
            //do nothing
        break;

    case IMSDT:
        
        send_result = *data_ptr - 48;

        MMK_CloseWin(MMISMS_SEND_ANIM_WIN_ID);

        MMISMS_OpenSendResultWin(send_result);
            //alert window: sms send success or send fail
        break;

    case IMNMI:

        newsms_seq = *data_ptr;
        
        MMISMS_SynNewComeSms();

            //sys current new sms
        break;

    default:

        SCI_TRACE_LOW("SMS COMMAND INDEX: %d", command_index);

        break;
    
    }
}
#endif