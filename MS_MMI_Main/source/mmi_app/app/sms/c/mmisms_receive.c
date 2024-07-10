/****************************************************************************
** File Name:      mmisms_receive.c                                         *
** Author:                                                                  *
** Date:           8/23/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe receiving SMS.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 08/2007        liming.deng      Create                                   *
**                                                                          *
****************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_receive.h"
#include "window_parse.h"
#include "guitext.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiidle_export.h"
#include "tb_dal.h"
#include "mmipb_export.h"

#include "mmistk_export.h"
#include "mmi_atc.h"
#include "mmiphone_export.h"
#include "mmi_textfun.h"
#include "guilistbox.h"
#include "mmimms_export.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmi_default.h"
#include "mmi_nv.h"
//#include "nvitem.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmipub.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
//#include "sci_types.h"
#include "mmisms_order.h"
#include "mmisms_read.h"
#include "mmivcard_export.h"
#include "mmiudisk_export.h"
#include "mmidm_export.h"
#include "mmisms_save.h"
#include "mmisms_set.h"
#include "gui_ucs2b_converter.h"
#include "mmisms_send.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
#include "mmisms_voicemail.h"
#endif
#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
//#include "mmiset.h"
#include "mmiset_export.h"

#ifdef _ATEST_
#include "atest_ota.h"
#endif//_ATEST_

#ifdef MOBILE_VIDEO_SUPPORT
#include "mv_sms_api.h"
#endif
#ifdef MMI_VCALENDAR_SUPPORT
#include "Mmivcalendar.h"
#endif

#ifdef SCREENSAVER_SUPPORT
#include "mmiss_export.h"  
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#include "met_mex_app_export.h"
#endif

#ifdef MRAPP_SUPPORT
#include "mmimrapp_export.h"
#endif
#ifdef ZDT_SMS_SUPPORT
#include "zdt_app.h"
#endif
#include "mmisms_showsmswin.h"
#include "mmisms_commonui.h"
#define MMISMS_STATUS_REPORT_LEN    30
#ifdef MMI_OTA_SUPPORT
#include "mmiota_export.h"
#endif
#include "mmisms_appcontrol.h"
#include "mmi_common.h"
#include "mmienvset_export.h" 
#ifdef ADULT_WATCH_SUPPORT
#include "mmismsapp_interface.h"
#ifdef DROPDOWN_NOTIFY_SHORTCUT
#include "watch_notifycenter.h"
#endif
#endif

#ifdef HERO_ENGINE_SUPPORT 
#include "heroEngineExport.h"
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL VCARD_LONG_SMS_T *s_long_vcardsms_ptr = PNULL;//记录Vcard
#ifdef MMI_VCALENDAR_SUPPORT
LOCAL VCARD_LONG_SMS_T *s_long_vcalsms_ptr = PNULL;
#endif


#if defined MMI_STATUSBAR_SCROLLMSG_SUPPORT || defined DROPDOWN_NOTIFY_SHORTCUT
typedef struct
{
    MN_DUAL_SYS_E dual_sys;
    MN_SMS_STORAGE_E storage;
    BOOLEAN is_concatenate_sms;
    uint8 longsms_max_num;
    uint8 longsms_seq_num;
    uint16 head_ref;
    MMISMS_READ_TYPE_E msg_type;
    MMISMS_READ_MSG_T read_msg;
    uint32 record_id;
}MMI_STATUSBAR_SCROLLMSG_T;

MMI_STATUSBAR_SCROLLMSG_T s_sms_statusbar_scrollmsg = {0};
#endif

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
BOOLEAN g_is_sms_vm_header = FALSE;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T  		   g_mmisms_global; // SMS的全局变量
#ifdef IKEYBACKUP_SUPPORT
extern BOOLEAN g_sms_is_backup;
#endif

#ifdef SXH_APPLE_SUPPORT
// extern BOOLEAN Apple_Get_ps_mode(void);
#include "apple_export.h"  /*lint !e746*/
#endif
/*****************************************************************************/
//     Description : to insert new message to order list
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNewMtToOrder(                                                //RETURN:
                              MN_DUAL_SYS_E                dual_sys,            //IN:
                              MN_SMS_STORAGE_E            storage,            //IN:
                              MN_SMS_RECORD_ID_T            record_id,            //IN:
                              MN_SMS_STATUS_E                status,                //IN:
                              APP_SMS_USER_DATA_T         *user_data_ptr,     //IN:
                              MN_SMS_MT_SMS_T *mt_sms_ptr,//IN:
                              BOOLEAN                     is_mms_push,        //IN:
                              BOOLEAN                    is_unhandle_order
                              );

/*****************************************************************************/
//     Description : operate MT ind as mms
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN OperateMtIndAsMmsPush(                                    //RETURN: TRUE, indicate the operation is finished
                                    MN_DUAL_SYS_E       dual_sys,
                                    APP_SMS_USER_DATA_T    *user_data_ptr,    //IN:
                                    MN_SMS_STORAGE_E    storage,        //IN:
                                    MN_SMS_RECORD_ID_T    record_id,        //IN:
                                    MN_SMS_MT_SMS_T *mt_sms_ptr//IN:
                                    );

/*****************************************************************************/
// Description : receive Vcard over sms
// Global resource dependence :s_long_vcardsms_ptr
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_RecvVcardOrVcalSms(
                                     MN_DUAL_SYS_E         dual_sys, 
                                     APP_SMS_USER_DATA_T   *sms_ptr,
                                     MN_SMS_TIME_STAMP_T   *time_stamp_ptr,
                                     MMIVC_SMS_SEND_TYPE_E send_type
                                     );
#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : save received Vcard data over sms
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_SaveVcardSms(
                               uint8   *name_ptr,
                               uint8   *buf_ptr,
                               uint16   buf_len
                               );
#endif
/*****************************************************************************/
// Description : 获得vCard文件名称(名称由SMS+接收时间字串组成)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void GetVcardFileNameOverSms(
                                   MN_SMS_TIME_STAMP_T  *sc_time_ptr,
                                   uint8                *name_ptr
                                   );

/*****************************************************************************/
// Description : Get SM receive time stamp
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL uint32 GetSmsReceiveTimeStamp(void);


#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : to show the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void ShowMsgContent(
                          MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                          uint16                    msg_len,            //IN:
                          MMISMS_READ_MSG_T          *read_msg_ptr,       //IN:
                          MN_SMS_TIME_STAMP_T        *sc_time_ptr,        //IN:
                          MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                          MMISMS_R8_LANGUAGE_E    language,
#ifdef MMI_SMS_CHAT_SUPPORT                          
                          BOOLEAN                    is_chat_mode,
                          BOOLEAN                    is_need_time,
#endif                          
                          MMI_STRING_T                *string_ptr            //OUT:
                          );
#endif

/*****************************************************************************/
//     Description : Get transmitter disply string
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void GetTransmitterDispString(
                                    MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                    MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                    MMI_STRING_T            *string_ptr            //OUT:
                                    );

/*****************************************************************************/
// Description : save received Vcard data over sms
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL VCARD_LONG_SMS_T* InitLongVcardsmsStruct(void);

/*****************************************************************************/
//     Description : operate MT ind as SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: except class 0
/*****************************************************************************/
LOCAL BOOLEAN OperateMtIndAsSms(                                        //RETURN: TRUE, indicate the operation is finished
                                MN_DUAL_SYS_E           dual_sys,
                                APP_SMS_USER_DATA_T        *user_data_ptr,    //IN:
                                MN_SMS_STORAGE_E        storage,        //IN:
                                MN_SMS_RECORD_ID_T        record_id,        //IN:
                                MN_SMS_MT_SMS_T *mt_sms_ptr//IN:
                                );

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : save received Vcalendar data over sms
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_SaveVcalendarSms(
                               uint8   *name_ptr,
                               uint8   *buf_ptr,
                               uint16   buf_len
                               );
#endif

/*****************************************************************************/
//     Description : to handle the new message window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleNewReportWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
#if defined(JAVA_SUPPORT_MYRIAD)

/*****************************************************************************/
// Description : init long sms struct.
// Global resource dependence : s_long_myriadsms_ptr
// Author:huangns
// Company:Amoi
// Return: void*, point to MYRIAD_LONG_SMS_T struct
// Note: 2011/11/03
/*****************************************************************************/
LOCAL MYRIAD_LONG_SMS_T* InitLongsmsStruct(void)
{
    uint32 i = 0;
    MYRIAD_LONG_SMS_T* long_myriadsms_ptr = PNULL;

    SCI_TRACE_LOW(("mmijava_export.c: Call MMIAPIJAVA_InitLongsmsStruct()"));

    long_myriadsms_ptr = (MYRIAD_LONG_SMS_T *)SCI_ALLOCA(sizeof(MYRIAD_LONG_SMS_T));

    if (PNULL != long_myriadsms_ptr)
    {
        SCI_MEMSET(long_myriadsms_ptr, 0, sizeof(MYRIAD_LONG_SMS_T));

        for(i = 0;i < MYRIAD_MAX_LONG_SMS_NUM; i++)
        {
            long_myriadsms_ptr->myriad_sms[i].seq_id = MYRIAD_SMS_INVAILD_INDEX;
        }
    }
    return long_myriadsms_ptr;
}

/*****************************************************************************/
//     Description : operate MT ind as for java
//    Global resource dependence : 
/*****************************************************************************/
LOCAL MYRIAD_SMS_ERROR_ID_E RetrieveSmsContentForMyriad(                                    //RETURN: 
                                                                    APP_SMS_USER_DATA_T    *user_data_ptr,    //IN:
                                                                    MN_SMS_USER_HEAD_T*  user_head_ptr,        //IN:
                                                                    MN_SMS_STORAGE_E    storage,        //IN:
                                                                    MN_SMS_RECORD_ID_T    record_id,        //IN:
                                                                    MN_SMS_MT_SMS_T        sms_t,
                                                                    int32 des_port,
                                                                    int32 org_port
                                                                    )
{
    BOOLEAN ret = FALSE;
    //MYRIAD_SMS_ERROR_ID_E sms_error_id = MYRIAD_SMS_ERROR_NO;
    uint8 iei_num = 0;
    int32 i = 0, index = -1;
    int32 ref_num = 0, seq_num = 0,max_num = 0;
    BOOLEAN is_long_sms = FALSE;

    SCISmsMessage*     sci_sms_java_ptr = PNULL;
    MYRIAD_SMS_CONTENT_T *valid_date_ptr = PNULL;
    uint8 limit_bytes = 0;
    uint8 split_bytes = 0;
    uint8* data_ptr = PNULL;
    uint8 receive_num = 1;
    uint8 sms_index[MYRIAD_MAX_LONG_SMS_NUM]={0};
    int32 empty_pos = -1;

    //SCI_ASSERT(PNULL != user_data_ptr && PNULL != user_head_ptr);
    if(PNULL == user_data_ptr || PNULL == user_head_ptr)
    {
        return MYRIAD_SMS_ERROR_ALLOC;
    }

    
    iei_num = user_head_ptr->iei_num;
    for (i = 0; i <iei_num; i++)
    {
        if((user_head_ptr->iei_arr[i].iei == MN_SMS_CONCAT_8_BIT_REF_NUM)
            ||(user_head_ptr->iei_arr[i].iei == MN_SMS_CONCAT_16_BIT_REF_NUM))
        {
            index = i;
            break;
        }
        else
        {
            index = -1;
        }
    }
        
    if (index != -1)  
    {
        if (user_head_ptr->iei_arr[index].iei == MN_SMS_CONCAT_8_BIT_REF_NUM)
        {
            ref_num = user_head_ptr->iei_arr[index].iei_data_t[0];
            max_num = user_head_ptr->iei_arr[index].iei_data_t[1];
            seq_num = user_head_ptr->iei_arr[index].iei_data_t[2];    
        }
        else if (user_head_ptr->iei_arr[index].iei == MN_SMS_CONCAT_16_BIT_REF_NUM)
        {
            ref_num = user_head_ptr->iei_arr[index].iei_data_t[0];
            ref_num += user_head_ptr->iei_arr[index].iei_data_t[1];
            max_num = user_head_ptr->iei_arr[index].iei_data_t[2];
            seq_num = user_head_ptr->iei_arr[index].iei_data_t[3];
        }
    }
    else
    {
       
        max_num = 1;
        ref_num  = record_id;
        seq_num = 1;
    }

    if (1 == max_num)
    {
        is_long_sms = FALSE;
    }
    else
    {
        is_long_sms = TRUE;
    }
        //set CPL_SmsMessage info
    //设置基本信息
    MMISMS_GetLenByAlpha(sms_t.dcs.alphabet_type, &split_bytes, &limit_bytes,TRUE);  
    sci_sms_java_ptr = (SCISmsMessage*)SCI_ALLOC_APP(sizeof(SCISmsMessage));

    if(PNULL == sci_sms_java_ptr)
    {
        return MYRIAD_SMS_ERROR_ALLOC;
    }
    SCI_MEMSET(sci_sms_java_ptr, 0, sizeof(SCISmsMessage));       

    MMIAPICOM_GenDispNumber(    
                            sms_t.origin_addr_t.number_type, 
                            MIN(MN_MAX_ADDR_BCD_LEN, sms_t.origin_addr_t.num_len),
                            sms_t.origin_addr_t.party_num,
                            sci_sms_java_ptr->addr,
                            MYRIAD_PHONE_NUM_MAX_SIZE  
                        );
    sci_sms_java_ptr->dstPort = des_port;
    sci_sms_java_ptr->srcPort = org_port;

    sci_sms_java_ptr->timestamp = (uint64)MMIAPICOM_Tm2Second(
                                            sms_t.time_stamp_t.second, 
                                            sms_t.time_stamp_t.minute, 
                                            sms_t.time_stamp_t.hour, 
                                            sms_t.time_stamp_t.day, 
                                            sms_t.time_stamp_t.month, 
                                            sms_t.time_stamp_t.year + MMISMS_MT_OFFSERT_YEAR
                                        );
    sci_sms_java_ptr->seq_id = ref_num;
    sci_sms_java_ptr->seq_idx = seq_num;
    sci_sms_java_ptr->seq_len = max_num; 
    sci_sms_java_ptr->encoding = sms_t.dcs.alphabet_type;
    
    data_ptr = (uint8*)SCI_ALLOC_APP(user_data_ptr->user_valid_data_t.length+1);
    if(PNULL == data_ptr)
    {
        SCI_FREE(sci_sms_java_ptr);
        return MYRIAD_SMS_ERROR_ALLOC;
    }
    SCI_MEMSET(data_ptr,0,user_data_ptr->user_valid_data_t.length + 1);
    SCI_MEMCPY(data_ptr,user_data_ptr->user_valid_data_t.user_valid_data_arr, user_data_ptr->user_valid_data_t.length);

  
    if (is_long_sms)
    {
        LOCAL MYRIAD_LONG_SMS_T* s_long_myriadsms_ptr = PNULL;
        if (PNULL == s_long_myriadsms_ptr)
        {
            s_long_myriadsms_ptr = InitLongsmsStruct();

            if (PNULL == s_long_myriadsms_ptr)
            {
                SCI_FREE(data_ptr);
                SCI_FREE(sci_sms_java_ptr)
                return MYRIAD_SMS_ERROR_ALLOC;
            }
        }
        
        for(i = 0;i < MYRIAD_MAX_LONG_SMS_NUM;i++)
        {
            if (ref_num == s_long_myriadsms_ptr->myriad_sms[i].seq_id)
            {
                if (seq_num != s_long_myriadsms_ptr->myriad_sms[i].seq_idx)
                {
                    sms_index[receive_num] = i;
                    receive_num++;
                }
                else
                {
                    
                    SCI_FREE(data_ptr);
                    SCI_FREE(sci_sms_java_ptr)
                    return MYRIAD_SMS_ERROR_REPEAT;
                }
            }
        }

        
        if (receive_num < max_num)
        {
            
            for(i = 0;i < MYRIAD_MAX_LONG_SMS_NUM;i++)
            {
                if (s_long_myriadsms_ptr->myriad_sms[i].timestamp == 0)
                {
                    empty_pos = i;
                    break;
                }
            }
            
            if ((empty_pos < MYRIAD_MAX_LONG_SMS_NUM) && (empty_pos != -1))
            {
                sci_sms_java_ptr->length = user_data_ptr->user_valid_data_t.length;
                sci_sms_java_ptr->data = data_ptr;
                SCI_MEMCPY(&(s_long_myriadsms_ptr->myriad_sms[empty_pos]),
                    sci_sms_java_ptr,
                    sizeof(SCISmsMessage));
            }
            //SCI_FREE(data_ptr);
            SCI_FREE(sci_sms_java_ptr);
            return MYRIAD_SMS_ERROR_NO;
        }
        else
        {
            
            int32 j = 1;
            int32 copy_len = 0;
            valid_date_ptr = (MYRIAD_SMS_CONTENT_T*)SCI_ALLOC_APP(sizeof(MYRIAD_SMS_CONTENT_T));

            if(PNULL == valid_date_ptr)
            {
                if(PNULL != data_ptr)
                {
                    SCI_FREE(data_ptr)
                }
                if(PNULL != sci_sms_java_ptr)
                {
                    SCI_FREE(sci_sms_java_ptr)
                }
                return MYRIAD_SMS_ERROR_ALLOC;
            }
            SCI_MEMSET(valid_date_ptr, 0, sizeof(MYRIAD_SMS_CONTENT_T));

            while(j <= receive_num)
            {
                if(MYRIAD_SMS_CONTENT_MAX_LEN <= valid_date_ptr->length)
                {
                    valid_date_ptr->length = MYRIAD_SMS_CONTENT_MAX_LEN;
                    break;
                }
                for(i = 1;i < receive_num;i++)
                {
                    if (s_long_myriadsms_ptr->myriad_sms[sms_index[i]].seq_idx== j)
                    {
                        copy_len = MIN(MYRIAD_SMS_CONTENT_MAX_LEN - valid_date_ptr->length, s_long_myriadsms_ptr->myriad_sms[sms_index[i]].length);
                        
                        SCI_MEMCPY(valid_date_ptr->data + valid_date_ptr->length, s_long_myriadsms_ptr->myriad_sms[sms_index[i]].data, copy_len);
                        
                        valid_date_ptr->length += copy_len;
                        
                        SCI_FREE(s_long_myriadsms_ptr->myriad_sms[sms_index[i]].data);
                        
                        SCI_MEMSET(&(s_long_myriadsms_ptr->myriad_sms[sms_index[i]]), 0, sizeof(SCISmsMessage));
                        
                        s_long_myriadsms_ptr->myriad_sms[sms_index[i]].seq_id = MYRIAD_SMS_INVAILD_INDEX;
                        
                        break;
                    }
                    else if (seq_num == j)
                    {
                        copy_len = MIN(MYRIAD_SMS_CONTENT_MAX_LEN - valid_date_ptr->length, user_data_ptr->user_valid_data_t.length);
                        
                        SCI_MEMCPY(valid_date_ptr->data + valid_date_ptr->length, data_ptr,copy_len);
                        
                        valid_date_ptr->length += copy_len;
                                                
                        break;
                    }     
                }
                j++;
            }    
            sci_sms_java_ptr->data = valid_date_ptr->data;
            sci_sms_java_ptr->length = valid_date_ptr->length;
            sci_sms_java_ptr->seq_idx = 1;
            sci_sms_java_ptr->seq_len = 1; 
        }
        
        
        for(i = 0;i < MYRIAD_MAX_LONG_SMS_NUM; i++)
        {
            if (MYRIAD_SMS_INVAILD_INDEX != s_long_myriadsms_ptr->myriad_sms[i].seq_id)
            {
                break;
            }
        }

        if (MYRIAD_MAX_LONG_SMS_NUM == i)
        {
            SCI_FREE(s_long_myriadsms_ptr);
            s_long_myriadsms_ptr = PNULL;
        }
        else
        {
            SCI_TRACE_LOW("RetrieveSmsContentForMyriad exist vcard sms");
        }
    }
    else
    {
        sci_sms_java_ptr->data = data_ptr;
        sci_sms_java_ptr->length = user_data_ptr->user_valid_data_t.length;
    }
    
    ret = MMIAPIJAVA_RetrieveSmsContent(sci_sms_java_ptr);
    SCI_FREE(data_ptr);
    SCI_FREE(sci_sms_java_ptr);  
    if(PNULL != valid_date_ptr)
    {	
        SCI_FREE(valid_date_ptr);
    }

    return MYRIAD_SMS_ERROR_NO;
}

/*****************************************************************************/
// Description : operate MT ind as for Myriad
// Global resource dependence :
// Author:huangns
// Company:Amoi
// Note: 20111103
/*****************************************************************************/
LOCAL BOOLEAN OperateMtIndForMyriad(
                                   MN_DUAL_SYS_E         dual_sys,                                  
                                   APP_SMS_USER_DATA_T   *user_data_ptr,
                                   MN_SMS_STORAGE_E       storage,
                                   MN_SMS_RECORD_ID_T     record_id,
                                   MN_SMS_MT_SMS_T        sms_t)
{
    BOOLEAN is_finished = TRUE;
    BOOLEAN is_new_java = FALSE;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    int32 des_port = 0,org_port = 0;

    if(PNULL == user_data_ptr)
    {
        return FALSE;
    }
    
    MNSMS_DecodeUserDataHeadEx(
                        dual_sys,
                        &user_data_ptr->user_data_head_t,
                        &user_head_ptr_t
                        );
    
    // 判断是否为java sms
    is_new_java = MMISMS_IsMtForJava(user_head_ptr_t,&des_port,&org_port);
    if (is_new_java)
    {    
        RetrieveSmsContentForMyriad(
                        user_data_ptr,
                        &user_head_ptr_t,
                        storage,
                        record_id,
                        sms_t,
                        des_port,
                        org_port);
        
        // call MN API to update SMS status.
        MNSMS_UpdateSmsStatusEx(dual_sys,storage, record_id, MN_SMS_FREE_SPACE);
        is_finished = TRUE;
    }
    else
    {
        is_finished = FALSE;
    }
    SCI_TRACE_LOW("OperateMtIndForMyriad: is_finished = %d is_new_java %d",is_finished,is_new_java);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);
    return is_finished;
}

#endif
//END @huangns_20111103
#ifdef JAVA_SUPPORT_IA
/*****************************************************************************/
// 	Description : to add the java data to global, and inform jvm
//	Global resource dependence : g_mmisms_javadata
//  Author: kfyy.ye
//      @Params:    (in)java_data_ptr - pointer to the java data to set
//      @Return:    None
//	Note:
/*****************************************************************************/
LOCAL void AddJavaSMSData(MMISMS_JAVA_DATA_T* java_data_ptr)
{
	MMISMS_JAVA_GLOBAL_T*   	java_global_ptr = MMIAPIJAVA_GetSmsJavaData();
	MMISMS_JAVA_DATA_T*     	tmp_data_ptr    = PNULL;
	SMS_CALLBACK              	javasms_func    = PNULL;
	SMS_Message					javamessage = {0};

    if (PNULL == java_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AddJavaSMSData java_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_299_112_2_18_2_53_2_178,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMISMS:AddJavaSMSData java_data_ptr = is_long =%d,total_num=%d,cur_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_303_112_2_18_2_53_2_179,(uint8*)"ddd",java_data_ptr->is_long,java_data_ptr->total_num,java_data_ptr->cur_num);


    // insert the data to global
    if (!java_data_ptr->is_long || java_data_ptr->cur_num == 1)
    {
        // new data, need to insert to TAIL
        if (java_global_ptr->java_data_root == PNULL)
        {
            java_global_ptr->java_data_root = java_data_ptr;
        }
        else
        {
            tmp_data_ptr = java_global_ptr->java_data_root;
            while (tmp_data_ptr->next_ptr != PNULL)
            {
                tmp_data_ptr = tmp_data_ptr->next_ptr;
            }
            tmp_data_ptr->next_ptr = java_data_ptr;

            // update new data
            java_data_ptr->previous_ptr = tmp_data_ptr;
        }
    }

    // inform java virtual machine to process the new mt sms
    if (!java_data_ptr->is_long || java_data_ptr->cur_num == java_data_ptr->total_num)
    {
        MMIAPIJAVA_CheckIfJavaPort(java_data_ptr->dst_port, &javasms_func, TRUE);
        if (PNULL == javasms_func)
        {
            //SCI_TRACE_LOW:"MMISMS:AddJavaSMSData javasms_func = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_334_112_2_18_2_53_2_180,(uint8*)"");
            return;
        }
        javamessage.binaryData = java_data_ptr->binaryData;
        javamessage.binaryDataLength = java_data_ptr->binaryDataLength;
        javamessage.dstPort = java_data_ptr->dst_port;
        javamessage.isTimeStampAvailable = java_data_ptr->isTimeStampAvailable;
        javamessage.srcPort = java_data_ptr->src_port;
        javamessage.textData = java_data_ptr->textData;
        javamessage.textDataLength = java_data_ptr->textDataLength;
        javamessage.timeStampHigh = java_data_ptr->timeStampHigh;
        javamessage.timeStampLow = java_data_ptr->timeStampLow;
        javamessage.type = java_data_ptr->type;
        javamessage.url = (char *)java_data_ptr->addr_buf;
        javamessage.urlLength =java_data_ptr->addr_len;
        javasms_func(JAVA_NEW_MT_IND, (int)&javamessage);
       
        // data is processed by jvm, remove it from global
        if (java_data_ptr->previous_ptr != PNULL)
        {
            java_data_ptr->previous_ptr->next_ptr = java_data_ptr->next_ptr;
        }
        else
        {
            java_global_ptr->java_data_root = java_data_ptr->next_ptr;
            
        }
        if (java_data_ptr->next_ptr != PNULL)
        {
            java_data_ptr->next_ptr->previous_ptr = java_data_ptr->previous_ptr;
        }

        // and delete the data
        if (PNULL != java_data_ptr->textData)
        {
            SCI_FREE(java_data_ptr->textData);
        }
        if (PNULL != java_data_ptr->binaryData)
        {
            SCI_FREE(java_data_ptr->binaryData);
        }
        SCI_FREE(java_data_ptr);

    }
}
/*****************************************************************************/
//     Description : operate MT ind as for java
//    Global resource dependence : 
//  Author: kelly.li
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN OperateMtIndForJava(                                    //RETURN: 
                                                                    MN_DUAL_SYS_E dual_sys,
                                                                    APP_SMS_USER_DATA_T    *user_data_ptr,    //IN:
                                                                    MN_SMS_STORAGE_E    storage,        //IN:
                                                                    MN_SMS_RECORD_ID_T    record_id,        //IN:
                                                                    MN_SMS_MT_SMS_T        sms_t
                                                                    )
{
    BOOLEAN is_finished = TRUE;
    BOOLEAN is_new_java = FALSE;
//    CPL_SmsMessage new_java_info = {0};
    #ifdef JAVA_SUPPORT_IA
    MMISMS_JAVA_GLOBAL_T*   java_global_data_ptr = MMIAPIJAVA_GetSmsJavaData();
    MMISMS_JAVA_DATA_T      *java_data_ptr   = PNULL;    
    uint8 	                limit_bytes = 0;
	uint8	                split_bytes = 0;
    char *                  data_ptr = PNULL;
    #endif
    uint8 *valid_date_ptr = PNULL;
    uint8 iei_num = 0;
    int32 i = 0, index = -1;
    int32 des_port = 0,org_port = 0;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
     int32 seq_id = 0, seq_indx = 0,seq_len = 0;
	 //int32 is_cbs = 0;
    // uint8 addr[ MMISMS_MAX_USERNUMBER_LEN + 2 ] = {0};
    // uint16 num_len = 0;
    
    if (PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: OperateMtIndForJava: user_data_ptr = PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_413_112_2_18_2_53_2_181,(uint8*)"");
        return FALSE;
    }

    MNSMS_DecodeUserDataHeadEx(
        dual_sys,
        &user_data_ptr->user_data_head_t,
        &user_head_ptr_t
        );

    // 判断是否为java sms
    is_new_java = MMISMS_IsMtForJava(user_head_ptr_t,&des_port,&org_port);
    
    //SCI_TRACE_LOW:"MMISMS: OperateMtIndForJava: is_new_java_sms = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_426_112_2_18_2_53_2_182,(uint8*)"d",is_new_java);
    
    if (is_new_java)
    {    
        valid_date_ptr = SCI_ALLOCAZ(user_data_ptr->user_valid_data_t.length);
        if (PNULL == valid_date_ptr)
        {
            //SCI_TRACE_LOW:"MMISMS: OperateMtIndForJava: valid_date_ptr alloc fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_433_112_2_18_2_53_2_183,(uint8*)"");
            return FALSE;
        }
        //SCI_MEMSET(valid_date_ptr, 0, user_data_ptr->user_valid_data_t.length);
        
        SCI_MEMCPY(valid_date_ptr,user_data_ptr->user_valid_data_t.user_valid_data_arr,user_data_ptr->user_valid_data_t.length);
        
        iei_num = user_head_ptr_t.iei_num;
        for (i = 0; i <iei_num; i++)
        {
            //SCI_TRACE_LOW:"MMISMS: OperateMtIndForJava: iei_arr[%d].iei=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_443_112_2_18_2_53_2_184,(uint8*)"dd",i,user_head_ptr_t.iei_arr[i].iei);
            if((user_head_ptr_t.iei_arr[i].iei == MN_SMS_CONCAT_8_BIT_REF_NUM)
                ||(user_head_ptr_t.iei_arr[i].iei == MN_SMS_CONCAT_16_BIT_REF_NUM))
            {
                index = i;
                break;
            }
            else
            {
                index = -1;
            }
        }
        
        if (index != -1)    //该sms 为长短信
        {
            if (user_head_ptr_t.iei_arr[index].iei == MN_SMS_CONCAT_8_BIT_REF_NUM)
            {
                seq_id = user_head_ptr_t.iei_arr[index].iei_data_t[0];
                seq_len = user_head_ptr_t.iei_arr[index].iei_data_t[1];
                seq_indx = user_head_ptr_t.iei_arr[index].iei_data_t[2];    
            }
            else if (user_head_ptr_t.iei_arr[index].iei == MN_SMS_CONCAT_16_BIT_REF_NUM)
            {
                seq_id = user_head_ptr_t.iei_arr[index].iei_data_t[0];
                seq_id += user_head_ptr_t.iei_arr[index].iei_data_t[1];
                seq_len = user_head_ptr_t.iei_arr[index].iei_data_t[2];
                seq_indx = user_head_ptr_t.iei_arr[index].iei_data_t[3];
            }
        }
        else
        {
            //不是长短信
            seq_len = 1;
            seq_id  = record_id;
            seq_indx = 1;
        }
        
        //set CPL_SmsMessage info
#ifdef JAVA_SUPPORT_IA        
        
        java_data_ptr = java_global_data_ptr->java_data_root;
        while ((PNULL != java_data_ptr)&&(java_data_ptr->dst_port != des_port)&&(java_data_ptr->src_port != org_port)&&(seq_id != java_data_ptr->ref_num))
        {
            java_data_ptr = java_data_ptr->next_ptr;
        }            
       
        MMISMS_GetLenByAlpha(sms_t.dcs.alphabet_type, &split_bytes, &limit_bytes, TRUE);  
        if (PNULL == java_data_ptr)
        {
            java_data_ptr = (MMISMS_JAVA_DATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_JAVA_DATA_T));
            if (PNULL == java_data_ptr)
            {
                SCI_FREE(valid_date_ptr);
                //SCI_TRACE_LOW:"MMISMS:OperateMtIndForJava java_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_496_112_2_18_2_53_2_185,(uint8*)"");
                return FALSE;
            }

            //SCI_MEMSET(java_data_ptr, 0, sizeof(MMISMS_JAVA_DATA_T));       

            java_data_ptr->addr_len = MMIAPICOM_GenDispNumber(    
                                sms_t.origin_addr_t.number_type, 
                                MIN(MN_MAX_ADDR_BCD_LEN, sms_t.origin_addr_t.num_len),
                                sms_t.origin_addr_t.party_num,
                                java_data_ptr->addr_buf,
                                MMISMS_JAVA_ADDR_MAX_LEN
                            );
            java_data_ptr->dst_port = des_port;
            java_data_ptr->is_cbs = FALSE;
            if (1 == seq_len)
            {
                java_data_ptr->is_long = FALSE;
            }
            else
            {
                java_data_ptr->is_long = TRUE;
            }

            MMISMS_CorrectTimeStamp(&sms_t.time_stamp_t, MMISMS_MT_TYPE);

            java_data_ptr->isTimeStampAvailable = TRUE;
            java_data_ptr->src_port = org_port;
            java_data_ptr->timeStampHigh = 0;
            java_data_ptr->timeStampLow = (int32)MMIAPICOM_Tm2Second(
                                            sms_t.time_stamp_t.second, 
                                            sms_t.time_stamp_t.minute, 
                                            sms_t.time_stamp_t.hour, 
                                            sms_t.time_stamp_t.day, 
                                            sms_t.time_stamp_t.month, 
                                            sms_t.time_stamp_t.year + MMISMS_MT_OFFSERT_YEAR
                                        );
            java_data_ptr->total_num = seq_len;       

            if (java_data_ptr->is_long)
            {
            
                data_ptr = SCI_ALLOCAZ(split_bytes*seq_len+1);
                if(PNULL == data_ptr)
                {
                    SCI_FREE(valid_date_ptr);
                    return FALSE;
                }
                
                //SCI_MEMSET(data_ptr,0,(split_bytes*seq_len+1)); 
                SCI_MEMCPY(data_ptr,user_data_ptr->user_valid_data_t.user_valid_data_arr,user_data_ptr->user_valid_data_t.length);
                java_data_ptr->ref_num   = seq_id;   
                java_data_ptr->cur_num  = 0;  
            
            }  
            else
            {
                data_ptr = SCI_ALLOCAZ(user_data_ptr->user_valid_data_t.length+1);//CR227692 maryxiao
                if(PNULL == data_ptr)
                {
                    SCI_FREE(valid_date_ptr);
                    return FALSE;
                }
                //SCI_MEMSET(data_ptr,0,user_data_ptr->user_valid_data_t.length+1);//CR227692 maryxiao
                SCI_MEMCPY(data_ptr,user_data_ptr->user_valid_data_t.user_valid_data_arr,user_data_ptr->user_valid_data_t.length);

            }
            
            //SCI_TRACE_LOW:"MMISMS: OperateMtIndForJava: sms_t.dcs.alphabet_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_564_112_2_18_2_53_2_186,(uint8*)"d",sms_t.dcs.alphabet_type);

            if (MN_SMS_UCS2_ALPHABET == sms_t.dcs.alphabet_type)
            {
                java_data_ptr->type = MN_SMS_UCS2_ALPHABET;
                java_data_ptr->textData = data_ptr;
                java_data_ptr->textDataLength = user_data_ptr->user_valid_data_t.length;//maryxiao
                //SCI_TRACE_LOW:"MMISMS: OperateMtIndForJava: MN_SMS_UCS2_ALPHABET user_data_ptr->user_valid_data_t.length  %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_571_112_2_18_2_53_2_187,(uint8*)"d",user_data_ptr->user_valid_data_t.length);
            }
            else
            {
                java_data_ptr->type = MN_SMS_8_BIT_ALPHBET;
                java_data_ptr->binaryData = data_ptr;
                java_data_ptr->binaryDataLength = user_data_ptr->user_valid_data_t.length;//maryxiao
                //SCI_TRACE_LOW:"MMISMS: OperateMtIndForJava: user_data_ptr->user_valid_data_t.length = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_578_112_2_18_2_53_2_188,(uint8*)"d",user_data_ptr->user_valid_data_t.length);
            }
        }
        else
        {

            if (MN_SMS_UCS2_ALPHABET == java_data_ptr->type)
            {
                data_ptr = java_data_ptr->textData;
                java_data_ptr->textDataLength += user_data_ptr->user_valid_data_t.length;
                //SCI_TRACE_LOW:"second:MMISMS: OperateMtIndForJava: MN_SMS_UCS2_ALPHABET user_data_ptr->user_valid_data_t.length  %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_588_112_2_18_2_53_2_189,(uint8*)"d",user_data_ptr->user_valid_data_t.length);
            }
            else
            {
                data_ptr = java_data_ptr->binaryData;
                java_data_ptr->binaryDataLength += user_data_ptr->user_valid_data_t.length;
                //SCI_TRACE_LOW:"second:MMISMS: OperateMtIndForJava: user_data_ptr->user_valid_data_t.length = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_594_112_2_18_2_53_2_190,(uint8*)"d",user_data_ptr->user_valid_data_t.length);
            }
            
            //SCI_TRACE_LOW:"seq_indx,split_bytesseq_len =%d,%d,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_597_112_2_18_2_53_2_191,(uint8*)"ddd",seq_len,seq_indx,split_bytes,user_data_ptr->user_valid_data_t.length);
            SCI_MEMCPY(data_ptr + (seq_indx-1)*split_bytes,user_data_ptr->user_valid_data_t.user_valid_data_arr,user_data_ptr->user_valid_data_t.length);

        }

        if (java_data_ptr->is_long)
        {        
            java_data_ptr->cur_num ++;
            //SCI_TRACE_LOW:"seq_indx,split_bytesseq_len =%d,%d,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_605_112_2_18_2_53_2_192,(uint8*)"ddd",seq_len,seq_indx,split_bytes,user_data_ptr->user_valid_data_t.length);
        }

        // add the data to global
		AddJavaSMSData(java_data_ptr);      
        //SCI_FREE(data_ptr);  无需在这个地方释放，在函数 AddJavaSMSData会进行释放的

        //SCI_FREE(java_data_ptr);     
#endif
        
        is_finished = TRUE;

        MNSMS_UpdateSmsStatusEx(dual_sys,storage, record_id, MN_SMS_FREE_SPACE);
        
        SCI_FREE(valid_date_ptr);
    }
    else
    {
        is_finished = FALSE;
    }

    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);
    
    return (is_finished);
}
#endif

#ifdef ZDT_SMS_SUPPORT
LOCAL BOOLEAN  UCS_Big_To_Little(
                             uint8*  src_ptr, //[IN]  Stringμ??′????
                             uint16  src_len, //[IN]  ?′Stringμ?3¤?è￡?byteêy
                             uint8*  dest_ptr
                             )
{
    uint8 tmp;
    uint16 i = 0;
    uint16 j = 0;
    while ( src_len > 1 )
    {
        tmp = src_ptr[j++];
        dest_ptr[i++] = src_ptr[j++];
        dest_ptr[i++] = tmp;
        src_len -= 2;
    }
    return TRUE;
}

LOCAL BOOLEAN  Utf8_To_Utf16be(
                             uint8*  src_ptr, //[IN]
                             uint16  src_len, //[IN]  
                             uint8*  dest_ptr
                             )
{
    uint8 *data_ptr = PNULL;
    uint16 data_len = src_len*2;
    data_ptr = SCI_ALLOC_APPZ(data_len+1);
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
    MMIAPICOM_DefaultToWstr(src_ptr, data_ptr, data_len);
    UCS_Big_To_Little(data_ptr,data_len,dest_ptr);
    SCI_FREE(data_ptr);
    return TRUE;
}

/*****************************************************************************/
// Description : init long sms struct.
// Global resource dependence : s_long_zdtsms_ptr
// Author:huangns
// Company:Amoi
// Return: void*, point to ZDT_LONG_SMS_T struct
// Note: 2011/11/03
/*****************************************************************************/
LOCAL ZDT_LONG_SMS_T* ZdtInitLongsmsStruct(void)
{
    uint32 i = 0;
    ZDT_LONG_SMS_T* long_zdtsms_ptr = PNULL;

    SCI_TRACE_LOW(("mmijava_export.c: Call MMIAPIJAVA_ZdtInitLongsmsStruct()"));

    long_zdtsms_ptr = (ZDT_LONG_SMS_T *)SCI_ALLOCA(sizeof(ZDT_LONG_SMS_T));

    if (PNULL != long_zdtsms_ptr)
    {
        SCI_MEMSET(long_zdtsms_ptr, 0, sizeof(ZDT_LONG_SMS_T));

        for(i = 0;i < ZDT_MAX_LONG_SMS_NUM; i++)
        {
            long_zdtsms_ptr->zdt_sms[i].seq_id = ZDT_SMS_INVAILD_INDEX;
        }
    }
    return long_zdtsms_ptr;
}


BOOLEAN ZdtRetrieveSmsContent(ZdtSmsMessage* msg)
{
    BOOLEAN                is_finished = FALSE;
    uint8 *                  data_ptr = PNULL;
    wchar *                  wdata_ptr = PNULL;
    uint32                  data_len = 0;
    uint8                iei_num = 0;
    int32                i = 0, index = -1;
    MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};
    uint8               addr[ MMISMS_MAX_USERNUMBER_LEN + 2 ] = {0};
    uint16              num_len = 0;
    BOOLEAN is_ucs2 = FALSE;
    MN_SMS_ALPHABET_TYPE_E    alphabet_type = 0;
    
    SCI_ASSERT(PNULL != msg);

    num_len = strlen(msg->addr);
    SCI_MEMCPY(addr,msg->addr,num_len);
    alphabet_type = msg->encoding;
       
    if ((MN_SMS_DEFAULT_ALPHABET == alphabet_type) ||
        (MN_SMS_8_BIT_ALPHBET == alphabet_type))
    {
        //data_len = msg->length;
        //data_ptr = SCI_ALLOC_APPZ(data_len+1);
		
        //if(PNULL == data_ptr)
        //{
        //    return FALSE;
        //}
        //
        //data_len = MMIAPICOM_Default2Ascii(
        //    msg->data,
        //    (wchar *)data_ptr,
        //    msg->length
        //    );
        //uint8 *msg_data_ptr = PNULL;
        uint16 wchat_len;
        data_len = msg->length*2;
        data_ptr = SCI_ALLOC_APPZ(data_len+1);
        if(PNULL == data_ptr)
        {
            return FALSE;
        }
        is_ucs2 = TRUE;
        wchat_len = MMIAPICOM_DefaultToWstr(msg->data, data_ptr, msg->length);
        wchat_len = wchat_len*2>data_len?data_len:wchat_len*2;
        UCS_Big_To_Little(data_ptr,wchat_len,msg->data);//短信纯数字或者英文utf8 统一转为双字节utf16be
        msg->length = wchat_len;
            
    }
    else
    {
        data_ptr = SCI_ALLOC_APPZ(msg->length+1);
        if(PNULL == data_ptr)
        {
            return FALSE;
        }
        is_ucs2 = TRUE;
        UCS_Big_To_Little(msg->data,msg->length,data_ptr);
        data_len = msg->length;
    }
    
    ZDT_YXTrace("MT SMS: alphabet_type = %d,num_len=%d,data_len =%d,is_ucs2=%d",alphabet_type,num_len,data_len,is_ucs2);
    ZDT_YXTrace("MT SMS: Num=%s",addr);

    MMIZDT_SMS_Rcv_IsValid(MN_DUAL_SYS_1,addr,num_len,data_ptr,data_len,is_ucs2);
    SCI_FREE(data_ptr);
    return (is_finished);
}
/*****************************************************************************/
//     Description : operate MT ind as for java
//    Global resource dependence : 
/*****************************************************************************/
LOCAL ZDT_SMS_ERROR_ID_E RetrieveSmsContentForZdt(                                    //RETURN: 
                                                        APP_SMS_USER_DATA_T    *user_data_ptr,    //IN:
                                                        MN_SMS_USER_HEAD_T*  user_head_ptr,        //IN:
                                                        MN_SMS_STORAGE_E    storage,        //IN:
                                                        MN_SMS_RECORD_ID_T    record_id,        //IN:
                                                        MN_SMS_MT_SMS_T        *sms_t,
                                                        int32 des_port,
                                                        int32 org_port
                                                        )
{
    BOOLEAN ret = FALSE;
    //ZDT_SMS_ERROR_ID_E sms_error_id = ZDT_SMS_ERROR_NO;
    uint8 iei_num = 0;
    int32 i = 0, index = -1;
    int32 ref_num = 0, seq_num = 0,max_num = 0;
    BOOLEAN is_long_sms = FALSE;

    ZdtSmsMessage*     sci_sms_zdt_ptr = PNULL;
    ZDT_SMS_CONTENT_T *valid_date_ptr = PNULL;
    uint8 limit_bytes = 0;
    uint8 split_bytes = 0;
    uint8* data_ptr = PNULL;
    uint8 receive_num = 1;
    uint8 sms_index[ZDT_MAX_LONG_SMS_NUM]={0};
    int32 empty_pos = -1;

    //SCI_ASSERT(PNULL != user_data_ptr && PNULL != user_head_ptr);
    if(PNULL == user_data_ptr || PNULL == user_head_ptr)
    {
        return ZDT_SMS_ERROR_ALLOC;
    }

    
    iei_num = user_head_ptr->iei_num;
    for (i = 0; i <iei_num; i++)
    {
        if((user_head_ptr->iei_arr[i].iei == MN_SMS_CONCAT_8_BIT_REF_NUM)
            ||(user_head_ptr->iei_arr[i].iei == MN_SMS_CONCAT_16_BIT_REF_NUM))
        {
            index = i;
            break;
        }
        else
        {
            index = -1;
        }
    }
        
    if (index != -1)  
    {
        if (user_head_ptr->iei_arr[index].iei == MN_SMS_CONCAT_8_BIT_REF_NUM)
        {
            ref_num = user_head_ptr->iei_arr[index].iei_data_t[0];
            max_num = user_head_ptr->iei_arr[index].iei_data_t[1];
            seq_num = user_head_ptr->iei_arr[index].iei_data_t[2];    
        }
        else if (user_head_ptr->iei_arr[index].iei == MN_SMS_CONCAT_16_BIT_REF_NUM)
        {
            ref_num = user_head_ptr->iei_arr[index].iei_data_t[0];
            ref_num += user_head_ptr->iei_arr[index].iei_data_t[1];
            max_num = user_head_ptr->iei_arr[index].iei_data_t[2];
            seq_num = user_head_ptr->iei_arr[index].iei_data_t[3];
        }
    }
    else
    {
       
        max_num = 1;
        ref_num  = record_id;
        seq_num = 1;
    }
    ZDT_YXTrace("RetrieveSmsContentForZdt max_num=%d,ref_num=%d,seq_num=%d,alphabet_type=%d",max_num,ref_num,seq_num,sms_t->dcs.alphabet_type);
    if (1 == max_num)
    {
        is_long_sms = FALSE;
    }
    else
    {
        is_long_sms = TRUE;
    }
        //set CPL_SmsMessage info
    //设置基本信息
    MMISMS_GetLenByAlpha(sms_t->dcs.alphabet_type, &split_bytes, &limit_bytes,TRUE);  
    sci_sms_zdt_ptr = (ZdtSmsMessage*)SCI_ALLOC_APP(sizeof(ZdtSmsMessage));

    if(PNULL == sci_sms_zdt_ptr)
    {
        return ZDT_SMS_ERROR_ALLOC;
    }
    SCI_MEMSET(sci_sms_zdt_ptr, 0, sizeof(ZdtSmsMessage));       

    MMIAPICOM_GenDispNumber(    
                            sms_t->origin_addr_t.number_type, 
                            MIN(MN_MAX_ADDR_BCD_LEN, sms_t->origin_addr_t.num_len),
                            sms_t->origin_addr_t.party_num,
                            sci_sms_zdt_ptr->addr,
                            ZDT_PHONE_NUM_MAX_SIZE  
                        );
    sci_sms_zdt_ptr->dstPort = des_port;
    sci_sms_zdt_ptr->srcPort = org_port;

    sci_sms_zdt_ptr->timestamp = (uint64)MMIAPICOM_Tm2Second(
                                            sms_t->time_stamp_t.second, 
                                            sms_t->time_stamp_t.minute, 
                                            sms_t->time_stamp_t.hour, 
                                            sms_t->time_stamp_t.day, 
                                            sms_t->time_stamp_t.month, 
                                            sms_t->time_stamp_t.year + MMISMS_MT_OFFSERT_YEAR
                                        );
    sci_sms_zdt_ptr->seq_id = ref_num;
    sci_sms_zdt_ptr->seq_idx = seq_num;
    sci_sms_zdt_ptr->seq_len = max_num; 
    sci_sms_zdt_ptr->encoding = sms_t->dcs.alphabet_type;
    if ((MN_SMS_DEFAULT_ALPHABET == sci_sms_zdt_ptr->encoding) || (MN_SMS_8_BIT_ALPHBET == sci_sms_zdt_ptr->encoding))
    {
        data_ptr = (uint8*)SCI_ALLOC_APP(user_data_ptr->user_valid_data_t.length*2+1);
    }
    else
    {
        data_ptr = (uint8*)SCI_ALLOC_APP(user_data_ptr->user_valid_data_t.length+1);
    }
    if(PNULL == data_ptr)
    {
        SCI_FREE(sci_sms_zdt_ptr);
        return ZDT_SMS_ERROR_ALLOC;
    }
    SCI_MEMSET(data_ptr,0,user_data_ptr->user_valid_data_t.length + 1);
    SCI_MEMCPY(data_ptr,user_data_ptr->user_valid_data_t.user_valid_data_arr, user_data_ptr->user_valid_data_t.length);

    ZDT_YXTrace("RetrieveSmsContentForZdt is_long_sms=%d,addr=%s,length=%d",is_long_sms,sci_sms_zdt_ptr->addr,user_data_ptr->user_valid_data_t.length);
    if (is_long_sms)
    {
        LOCAL ZDT_LONG_SMS_T* s_long_zdtsms_ptr = PNULL;
        if (PNULL == s_long_zdtsms_ptr)
        {
            s_long_zdtsms_ptr = ZdtInitLongsmsStruct();

            if (PNULL == s_long_zdtsms_ptr)
            {
                SCI_FREE(data_ptr);
                SCI_FREE(sci_sms_zdt_ptr);
                return ZDT_SMS_ERROR_ALLOC;
            }
        }
        
        for(i = 0;i < ZDT_MAX_LONG_SMS_NUM;i++)
        {
            if (ref_num == s_long_zdtsms_ptr->zdt_sms[i].seq_id)
            {
                if (seq_num != s_long_zdtsms_ptr->zdt_sms[i].seq_idx)
                {
                    sms_index[receive_num] = i;
                    receive_num++;
                }
                else
                {
                    
                    SCI_FREE(data_ptr);
                    SCI_FREE(sci_sms_zdt_ptr);
                    return ZDT_SMS_ERROR_REPEAT;
                }
            }
        }

        
        if (receive_num < max_num)
        {
            for(i = 0;i < ZDT_MAX_LONG_SMS_NUM;i++)
            {
                if (s_long_zdtsms_ptr->zdt_sms[i].timestamp == 0)
                {
                    empty_pos = i;
                    break;
                }
            }
            
            if ((empty_pos < ZDT_MAX_LONG_SMS_NUM) && (empty_pos != -1))
            {
                sci_sms_zdt_ptr->length = user_data_ptr->user_valid_data_t.length;
                sci_sms_zdt_ptr->data = data_ptr;
                SCI_MEMCPY(&(s_long_zdtsms_ptr->zdt_sms[empty_pos]),
                    sci_sms_zdt_ptr,
                    sizeof(ZdtSmsMessage));
            }
            //SCI_FREE(data_ptr);
            SCI_FREE(sci_sms_zdt_ptr);
            return ZDT_SMS_ERROR_NO;
        }
        else
        {
            
            int32 j = 1;
            int32 copy_len = 0;
            uint8 *sms_long_data = PNULL;
            uint32 sms_long_data_max_length = MMISMS_MAX_DEF_SPLIT_LEN * max_num;
            valid_date_ptr = (ZDT_SMS_CONTENT_T*)SCI_ALLOC_APP(sizeof(ZDT_SMS_CONTENT_T));
            if ((MN_SMS_DEFAULT_ALPHABET == sci_sms_zdt_ptr->encoding) || (MN_SMS_8_BIT_ALPHBET == sci_sms_zdt_ptr->encoding))
            {
                sms_long_data_max_length = MMISMS_MAX_DEF_SPLIT_LEN * max_num*2;//需要转为unicode 所以需要2倍空间
            }
            sms_long_data = (uint8 *) SCI_ALLOC_APPZ(sms_long_data_max_length+1);

            if(PNULL == valid_date_ptr || PNULL == sms_long_data)
            {
                if(PNULL != data_ptr)
                {
                    SCI_FREE(data_ptr)
                }
                if(PNULL != sci_sms_zdt_ptr)
                {
                    SCI_FREE(sci_sms_zdt_ptr)
                }
                if(PNULL != valid_date_ptr)
                {
                    SCI_FREE(valid_date_ptr)
                }
                if(PNULL != sms_long_data)
                {
                    SCI_FREE(sms_long_data)
                }
                return ZDT_SMS_ERROR_ALLOC;
            }
            SCI_MEMSET(valid_date_ptr, 0, sizeof(ZDT_SMS_CONTENT_T));
            valid_date_ptr->data = sms_long_data;

            while(j <= receive_num)
            {
                if(sms_long_data_max_length <= valid_date_ptr->length)
                {
                    valid_date_ptr->length = sms_long_data_max_length;
                    break;
                }
                for(i = 1;i < receive_num;i++)
                {
                    if (s_long_zdtsms_ptr->zdt_sms[sms_index[i]].seq_idx== j)
                    {
                        copy_len = MIN(sms_long_data_max_length - valid_date_ptr->length, s_long_zdtsms_ptr->zdt_sms[sms_index[i]].length);
                        
                        SCI_MEMCPY(valid_date_ptr->data + valid_date_ptr->length, s_long_zdtsms_ptr->zdt_sms[sms_index[i]].data, copy_len);
                        
                        valid_date_ptr->length += copy_len;
                        
                        SCI_FREE(s_long_zdtsms_ptr->zdt_sms[sms_index[i]].data);
                        
                        SCI_MEMSET(&(s_long_zdtsms_ptr->zdt_sms[sms_index[i]]), 0, sizeof(ZdtSmsMessage));
                        
                        s_long_zdtsms_ptr->zdt_sms[sms_index[i]].seq_id = ZDT_SMS_INVAILD_INDEX;
                        
                        break;
                    }
                    else if (seq_num == j)
                    {
                        copy_len = MIN(sms_long_data_max_length - valid_date_ptr->length, user_data_ptr->user_valid_data_t.length);
                        
                        SCI_MEMCPY(valid_date_ptr->data + valid_date_ptr->length, data_ptr,copy_len);
                        
                        valid_date_ptr->length += copy_len;
                                                
                        break;
                    }     
                }
                j++;
            }    
            sci_sms_zdt_ptr->data = valid_date_ptr->data;
            sci_sms_zdt_ptr->length = valid_date_ptr->length;
            sci_sms_zdt_ptr->seq_idx = 1;
            sci_sms_zdt_ptr->seq_len = 1; 
            sci_sms_zdt_ptr->max_length = sms_long_data_max_length;
        }
        
        
        for(i = 0;i < ZDT_MAX_LONG_SMS_NUM; i++)
        {
            if (ZDT_SMS_INVAILD_INDEX != s_long_zdtsms_ptr->zdt_sms[i].seq_id)
            {
                break;
            }
        }

        if (ZDT_MAX_LONG_SMS_NUM == i)
        {
            SCI_FREE(s_long_zdtsms_ptr);
            s_long_zdtsms_ptr = PNULL;
        }
        else
        {
            SCI_TRACE_LOW("RetrieveSmsContentForZdt exist vcard sms");
        }
    }
    else
    {
        sci_sms_zdt_ptr->data = data_ptr;
        sci_sms_zdt_ptr->length = user_data_ptr->user_valid_data_t.length;
    }
    
    ret = ZdtRetrieveSmsContent(sci_sms_zdt_ptr);
    ZDT_YXTrace("RetrieveSmsContentForZdt length=%d",sci_sms_zdt_ptr->length);
    ZDT_YXTraceU8(sci_sms_zdt_ptr->data,sci_sms_zdt_ptr->length);
    YX_Net_Send_SMS(sci_sms_zdt_ptr->data, sci_sms_zdt_ptr->length, &sci_sms_zdt_ptr->addr);
    
    SCI_FREE(data_ptr);
    SCI_FREE(sci_sms_zdt_ptr);  
    if(PNULL != valid_date_ptr)
    {	
        SCI_FREE(valid_date_ptr->data);
        SCI_FREE(valid_date_ptr);
    }

    return ZDT_SMS_ERROR_NO;
}

/*****************************************************************************/
// Description : operate MT ind as for Zdt
// Global resource dependence :
// Author:huangns
// Company:Amoi
// Note: 20111103
/*****************************************************************************/
LOCAL BOOLEAN OperateMtIndForZdt(
                                   MN_DUAL_SYS_E         dual_sys,                                  
                                   APP_SMS_USER_DATA_T   *user_data_ptr,
                                   MN_SMS_STORAGE_E       storage,
                                   MN_SMS_RECORD_ID_T     record_id,
                                   MN_SMS_MT_SMS_T        *sms_t)
{
    BOOLEAN is_finished = TRUE;
    BOOLEAN is_new_java = FALSE;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    int32 des_port = 0,org_port = 0;

    if(PNULL == user_data_ptr)
    {
        return FALSE;
    }
    
    MNSMS_DecodeUserDataHeadEx(
                        dual_sys,
                        &user_data_ptr->user_data_head_t,
                        &user_head_ptr_t
                        );
    
    RetrieveSmsContentForZdt(
                    user_data_ptr,
                    &user_head_ptr_t,
                    storage,
                    record_id,
                    sms_t,
                    des_port,
                    org_port);
        
    // call MN API to update SMS status.
    MNSMS_UpdateSmsStatusEx(dual_sys,storage, record_id, MN_SMS_FREE_SPACE);
    is_finished = TRUE;
    SCI_TRACE_LOW("OperateMtIndForZdt: is_finished = %d is_new_java %d",is_finished,is_new_java);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);
    return is_finished;
}

#endif
/*****************************************************************************/
// Description : save received Vcard data over sms
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL VCARD_LONG_SMS_T* InitLongVcardsmsStruct(void)
{
    uint32 i = 0;
    VCARD_LONG_SMS_T *long_vcardsms_ptr = PNULL;
    
    //SCI_TRACE_LOW:"SMS: InitLongVcardsmsStruct()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_641_112_2_18_2_53_2_193,(uint8*)"");

    long_vcardsms_ptr = (VCARD_LONG_SMS_T *)SCI_ALLOCAZ(sizeof(VCARD_LONG_SMS_T));

    if (PNULL != long_vcardsms_ptr)
    {
        //SCI_MEMSET(long_vcardsms_ptr, 0, sizeof(VCARD_LONG_SMS_T));

        for(i = 0;i < MMIVC_MAX_LONG_SMS_NUM; i++)
        {
            long_vcardsms_ptr->vcard_sms[i].ref_num = MMISMS_INVAILD_INDEX;
            long_vcardsms_ptr->vcard_sms[i].is_valid = FALSE;
        }
    }

    return long_vcardsms_ptr;
}

/*****************************************************************************/
//     Description : operate MT ind as SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: except class 0
/*****************************************************************************/
LOCAL BOOLEAN OperateMtIndAsSms(                                        //RETURN: TRUE, indicate the operation is finished
                                MN_DUAL_SYS_E           dual_sys,
                                APP_SMS_USER_DATA_T        *user_data_ptr,    //IN:
                                MN_SMS_STORAGE_E        storage,        //IN:
                                MN_SMS_RECORD_ID_T        record_id,        //IN:
                                MN_SMS_MT_SMS_T *mt_sms_ptr//IN:
                                )
{
    MN_SMS_USER_DATA_HEAD_T *read_head_ptr = PNULL; 
    MN_SMS_USER_DATA_HEAD_T *current_head_ptr = PNULL; 
    BOOLEAN is_finished = TRUE;
#if 0  
// no use
    MN_SMS_STATUS_E status = MN_SMS_MT_TO_BE_READ;  
#endif
    BOOLEAN is_replace = TRUE;
    MMISMS_SEND_T           *send_info_ptr = PNULL;
    BOOLEAN is_find_oper = FALSE;
    MN_SMS_STORAGE_E        sending_storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T        sending_record_id = 0;
#ifdef MMI_SMS_CHAT_SUPPORT
    //BOOLEAN is_num_compare = FALSE;
    MMIPB_BCD_NUMBER_T mt_num = {0};

    BOOLEAN is_num_compare = FALSE;
#endif

    if (PNULL == user_data_ptr || PNULL == mt_sms_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:OperateMtIndAsSms user_data_ptr mt_sms_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_687_112_2_18_2_53_3_194,(uint8*)"");

        return FALSE;
    }

#ifdef MMI_SMS_CHAT_SUPPORT
    mt_num.number_len = mt_sms_ptr->origin_addr_t.num_len;
    mt_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(mt_sms_ptr->origin_addr_t.number_type,MN_NUM_PLAN_UNKNOW);
    SCI_MEMCPY(
               mt_num.number,
               mt_sms_ptr->origin_addr_t.party_num,
               mt_num.number_len
               ); 

    if(0 == MMIAPIPB_BcdSmartCmp(&mt_num,(MMIPB_BCD_NUMBER_T *)&g_mmisms_global.chat_number))
    {
        is_num_compare = TRUE;
    }
#endif
   
    //SCI_TRACE_LOW:"MMISMS:OperateMtIndAsSms storage = %d, record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_724_112_2_18_2_53_3_195,(uint8*)"dd", storage, record_id);

    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SAVING);
    if (PNULL != send_info_ptr)
    {
        if (MN_SMS_NO_STORED != send_info_ptr->cur_send_data.cur_send_storage 
            && storage == send_info_ptr->cur_send_data.cur_send_storage
            && record_id == send_info_ptr->cur_send_data.cur_send_record_id)
        {
            is_find_oper = TRUE;
            MMISMS_GetStorageAndRecordidForSendSms(send_info_ptr, &sending_storage, &sending_record_id);
            if (MN_SMS_NO_STORED != storage)
            {
                send_info_ptr->cur_send_data.cur_send_record_id = sending_record_id;
                send_info_ptr->cur_send_data.cur_send_storage = sending_storage;
            }
        }
    }
    send_info_ptr = MMISMS_GetSendInfoFromSendLIst(MMISMS_SENDSTATUS_SENDING);
    if (PNULL != send_info_ptr && !is_find_oper)
    {
        MMISMS_UpdateCurSendRecordId(send_info_ptr, storage, record_id, FALSE);
    }

    //SCI_TRACE_LOW:" MMISMS:OperateMtIndAsSms storage = %d, record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_748_112_2_18_2_53_3_196,(uint8*)"dd", storage, record_id );
    if (MMISMS_IsOrderOk())  //排序完成
    {
        //当前状态为删除所有收件箱短消息，则将此时收到的短消息放入到未处理链表中，
        //全部删除完毕时将未处理链表中的短消息放入相应的链表中
        if (MMISMS_GetDelAllFlag()
#ifdef MMI_SMS_MOVE_SUPPORT            
            || MMISMS_GetMoveAllFlag()
#endif            
#ifdef MMI_SMS_CHAT_SUPPORT 
            || ((MMISMS_READ_CHAT_NUM_SMS == MMISMS_GetCurrentOperStatus())
                && MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
                && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
               )
#endif
#ifdef IKEYBACKUP_SUPPORT
            || g_sms_is_backup
#endif
#ifdef MMI_PDA_SUPPORT
            ||(MMISMS_GetCurrentOperStatus() ==MMISMS_MATCH_DATA_DELETE)
#endif
            )
        {
            is_replace = InsertNewMtToOrder(
                dual_sys,
                storage, 
                record_id,
                MN_SMS_MT_TO_BE_READ,
                user_data_ptr,
                mt_sms_ptr,
                FALSE,
                TRUE
                );           
        }
        else
        {
            // insert this new message to order list
            is_replace = InsertNewMtToOrder(
                dual_sys,
                storage, 
                record_id,
                MN_SMS_MT_TO_BE_READ,
                user_data_ptr,
                mt_sms_ptr,
                FALSE,
                FALSE
                );
        }       

        current_head_ptr = &(user_data_ptr->user_data_head_t);
        read_head_ptr     = &(g_mmisms_global.read_msg.head_data);
        
        //SCI_TRACE_LOW:"MMISMS: OperateMtIndAsSms head length = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_798_112_2_18_2_53_3_197,(uint8*)"d", read_head_ptr->length);
        //SCI_TRACE_LOW:"MMISMS: OperateMtIndAsSms ref = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_799_112_2_18_2_53_3_198,(uint8*)"d", MMISMS_GetRefNumFromUserHead(read_head_ptr));
        
        if (((MMISMS_NO_OPER == MMISMS_GetCurrentOperStatus())
             && (MMISMS_SHOWMSG_WIN_ID == MMK_GetFocusWinId())
             && (MMISMS_READ_MTSMS == MMISMS_GetMtType())
             && (read_head_ptr->length > 0)
             && (current_head_ptr->length > 0)
             && (MMISMS_GetRefNumFromUserHead(current_head_ptr) == MMISMS_GetRefNumFromUserHead(read_head_ptr))
             && !MMISMS_IsR8Language(g_mmisms_global.read_msg.language))
             ||((MMISMS_NO_OPER == MMISMS_GetCurrentOperStatus()) 
             && (MMISMS_SHOWMSG_WIN_ID == MMK_GetFocusWinId()) 
             && (MMISMS_READ_MTSMS == MMISMS_GetMtType())  
             && (read_head_ptr->length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) 
             && (current_head_ptr->length > MMISMS_R8_MAX_NORMAL_HEAD_LEN)
             && (MMISMS_GetRefNumFromUserHead(current_head_ptr) == MMISMS_GetRefNumFromUserHead(read_head_ptr)
             && MMISMS_IsR8Language(g_mmisms_global.read_msg.language))
             )) 
        {
            //SCI_TRACE_LOW:"MMISMS: OperateMtIndAsSms need not update window"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_817_112_2_18_2_53_3_199,(uint8*)"");
        }
        else
        {
            MMIEVENT_INFO_T event_info = {0};
            MMISMS_NEW_SMS_INFO_T sms_info = {0};
            event_info.event_type = SMS_NEWSMS_EVENT;
            event_info.param_ptr = &sms_info;
            sms_info.dual_sys = dual_sys;
            sms_info.storage = storage;
            sms_info.record_id = record_id;
            sms_info.mo_mt_type = MMISMS_MT_HAVE_READ;
#ifdef MMI_SMS_CHAT_SUPPORT     
            sms_info.cur_chat_order_id = g_mmisms_global.chat_oper_order_id;
#endif
            SCI_MEMCPY(&sms_info.user_data,user_data_ptr,sizeof(APP_SMS_USER_DATA_T));
            SCI_MEMCPY(&sms_info.mt_sms,mt_sms_ptr,sizeof(MN_SMS_MT_SMS_T));
            SCI_TRACE_LOW("mmisms trigger new sms event");
            MMIFW_TriggerEvent(&event_info);
        }
    }
    else //排序未完成
    {
        is_replace = InsertNewMtToOrder(
            dual_sys,
            storage, 
            record_id,
            MN_SMS_MT_TO_BE_READ,
            user_data_ptr,
            mt_sms_ptr,
            FALSE,
            TRUE
            );
    }
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_NEW_MT); 
    if (is_replace)
    {                
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SMS_UPDATED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    
    return (is_finished);
}

#ifdef SXH_APPLE_SUPPORT
//	extern uint32 g_apple_ps_mode;
void AppleOperateMtIndAsSms(										//RETURN: TRUE, indicate the operation is finished
                                                        MN_DUAL_SYS_E			 dual_sys,
                                                        APP_SMS_USER_DATA_T 	   *user_data_ptr,	  //IN:
                                                        MN_SMS_STORAGE_E		storage,		//IN:
                                                        MN_SMS_RECORD_ID_T		  record_id,		//IN:
                                                        MN_SMS_MT_SMS_T *mt_sms_ptr//IN:
                                                        )
{
    if (PNULL == user_data_ptr || PNULL == mt_sms_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:" MMISMS:OperateMtIndAsSms storage = %d, record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_902_112_2_18_2_53_3_200,(uint8*)"dd",storage, record_id );

    if (MMISMS_IsOrderOk())  //排序完成
    {
        //当前状态为删除所有收件箱短消息，则将此时收到的短消息放入到未处理链表中，
        //全部删除完毕时将未处理链表中的短消息放入相应的链表中
        if (MMISMS_GetDelAllFlag()) 
        {
            InsertNewMtToOrder(
                dual_sys,
                storage, 
                record_id,
                MN_SMS_MT_TO_BE_READ,
                user_data_ptr,
                mt_sms_ptr,
                FALSE,
                TRUE
                );	   
        }
        else
        {
            // insert this new message to order list
            InsertNewMtToOrder(
                dual_sys,
                storage, 
                record_id,
                MN_SMS_MT_TO_BE_READ,
                user_data_ptr,
                mt_sms_ptr,
                FALSE,
                FALSE
                );
        }
    }
    else //排序未完成
    {
        InsertNewMtToOrder(
            dual_sys,
            storage, 
            record_id,
            MN_SMS_MT_TO_BE_READ,
            user_data_ptr,
            mt_sms_ptr,
            FALSE,
            TRUE
            );
    }
}
#endif
/*****************************************************************************/
//     Description : to insert new message to order list
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNewMtToOrder(                                                //RETURN:
                              MN_DUAL_SYS_E                dual_sys,            //IN:
                              MN_SMS_STORAGE_E            storage,            //IN:
                              MN_SMS_RECORD_ID_T            record_id,            //IN:
                              MN_SMS_STATUS_E                status,                //IN:
                              APP_SMS_USER_DATA_T         *user_data_ptr,     //IN:
                              MN_SMS_MT_SMS_T *mt_sms_ptr,//IN:
                              BOOLEAN                     is_mms_push,        //IN:
                              BOOLEAN                    is_unhandle_order
                              )
{
    uint8 is_ucs2 = FALSE;
    uint32 sc_time = 0;
    MMISMS_STATE_T insert_order_info = {0};
    uint8 mmisms_status = 0;    //inserted SMS status
    uint16 num_len = 0;
    MN_SMS_USER_DATA_HEAD_T *data_head_ptr =  &(user_data_ptr->user_data_head_t);
    uint8 content_len = 0;
//    uint8 content [MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) + 1] = {0};
    MN_SMS_ALPHABET_TYPE_E alpha = MN_SMS_DEFAULT_ALPHABET;
    MN_SMS_TIME_STAMP_T *sc_time_ptr = PNULL;
    MN_CALLED_NUMBER_T *orginal_addr_ptr = PNULL;
    MN_SMS_PID_E pid_e = MN_SMS_PID_DEFAULT_TYPE;
    BOOLEAN is_replace = FALSE;
    MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
//    uint16 content_r8 [MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) + 1] = {0};
    BOOLEAN is_lock_shift = FALSE;
    BOOLEAN is_single_shift = FALSE;
    
    if (PNULL == mt_sms_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:InsertNewMtToOrder mt_sms_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_987_112_2_18_2_53_3_201,(uint8*)"");
        return FALSE;
    }

    alpha = mt_sms_ptr->dcs.alphabet_type;
    sc_time_ptr = &mt_sms_ptr->time_stamp_t;
    orginal_addr_ptr = &mt_sms_ptr->origin_addr_t;
    pid_e = mt_sms_ptr->pid_e;

    //SCI_TRACE_LOW:"MMISMS: InsertNewMtToOrder(%d,%d, %d, %d), pid_e=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_996_112_2_18_2_53_3_202,(uint8*)"ddddd", dual_sys, storage, record_id, status, pid_e);
#ifdef MMI_SMS_REPLY_PATH_SUPPORT    
    SCI_TRACE_LOW("MMISMS: InsertNewMtToOrder reply_path_is_exist = %d, scaLen=%d", mt_sms_ptr->reply_path_is_exist, mt_sms_ptr->sc_addr_t.num_len);
    SCI_TRACE_BUF("MMISMS: InsertNewMtToOrder reply sca:", g_mmisms_global.read_msg.sc_addr_t.party_num, sizeof(g_mmisms_global.read_msg.sc_addr_t.party_num));
#endif    

    is_ucs2 = (MN_SMS_UCS2_ALPHABET == alpha) ? TRUE : FALSE;

    MMISMS_CorrectTimeStamp(sc_time_ptr, MMISMS_MT_TYPE);
    
    // translate the time to second
    sc_time = MMIAPICOM_Tm2Second( 
        sc_time_ptr->second,
        sc_time_ptr->minute,
        sc_time_ptr->hour,
        sc_time_ptr->day,
        sc_time_ptr->month,
        (sc_time_ptr->year) + MMISMS_MT_OFFSERT_YEAR
        );
    
    // get display content
    //if (PNULL != orginal_addr_ptr)
    {
        num_len = MIN(orginal_addr_ptr->num_len, MMISMS_BCD_NUMBER_MAX_LEN);
        
        insert_order_info.number.bcd_number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(
            (MN_NUMBER_TYPE_E)orginal_addr_ptr->number_type,
            (MN_NUMBER_PLAN_E)orginal_addr_ptr->number_plan
            );
/*        
        if (MN_NUM_TYPE_ALPHANUMERIC == orginal_addr_ptr->number_type)
        {
            num_len = MMIAPICOM_Default2Ascii(
                orginal_addr_ptr->party_num, 
                insert_order_info.number.bcd_number.number, 
                num_len
                );
        }

        else
*/
        {
            SCI_MEMCPY(
                insert_order_info.number.bcd_number.number,
                orginal_addr_ptr->party_num,
                num_len
                );
        }             
        insert_order_info.number.bcd_number.number_len = num_len;
    }
    
//smsdecode to do  

    if (MN_SMS_DEFAULT_ALPHABET == alpha)
    {
        MMISMS_DecodeUserDataHead(&(user_data_ptr->user_data_head_t), &user_head_ptr_t);
        language = MMISMS_JudgeR8Language(&user_head_ptr_t);
        MMISMS_DecodeR8Header(&user_head_ptr_t,&is_lock_shift,&is_single_shift);
    }
    
    if (MN_SMS_MT_SR_TO_BE_READ != status)
    {
        if(MN_SMS_UCS2_ALPHABET == alpha)
        {
            content_len = MIN(MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar), user_data_ptr->user_valid_data_t.length); 
        }
        else
        {
            content_len = MIN(MMIMESSAGE_DIAPLAY_LEN, user_data_ptr->user_valid_data_t.length); 
        }
        MMISMS_ConvertValidDataToDispStr((wchar *)insert_order_info.display_content.content,
                                         (uint16*)&insert_order_info.display_content.content_len,
                                         user_data_ptr->user_valid_data_t.user_valid_data_arr,
                                         content_len,
                                         alpha,
                                         language,
                                         is_lock_shift,
                                         is_single_shift
                                         );
    }
    
    if (MN_SMS_DEFAULT_ALPHABET == alpha)
    {
        MMISMS_FreeUserDataHeadSpace(&user_head_ptr_t);
    }
        
    //SCI_TRACE_LOW:"MMISMS: InsertNewMtToOrder head_len = %d, ref_num = 0x%X, seq_num = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_1119_112_2_18_2_53_3_206,(uint8*)"ddd", data_head_ptr->length,data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS],data_head_ptr->user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS]);
    
    // get the related sf_status
    if (is_mms_push)
    {
        mmisms_status = MMISMS_MT_NOT_DOWNLOAD;
        insert_order_info.flag.msg_type = MMISMS_TYPE_WAP_PUSH;
    }
    else
    {
        mmisms_status = MMISMS_TranslateMNStatus2SFStatus(status, MMISMS_MT_TYPE, TRUE);
        insert_order_info.flag.msg_type = MMISMS_TYPE_SMS;
    }
    
    insert_order_info.flag.storage = storage;
    insert_order_info.flag.mo_mt_type = mmisms_status;
    insert_order_info.flag.is_ucs2 = is_ucs2;
    insert_order_info.record_id = record_id;
    insert_order_info.time = sc_time;
    insert_order_info.flag.dual_sys = dual_sys;


    if (MN_SMS_MT_SR_TO_BE_READ == status) //not status report
    {
        insert_order_info.flag.is_concatenate_sms = FALSE;
    }
    else if ((PNULL != data_head_ptr) &&
             (((data_head_ptr->length != 0) && !MMISMS_IsR8Language(language)) ||
             ((data_head_ptr->length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(language))))/*lint !e774*/ 
    {
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
        if (MMISMS_GetConcatenatedSMS())
#endif
        {
            insert_order_info.flag.is_concatenate_sms = TRUE;
        }

        if(MN_SMS_CONCAT_8_BIT_REF_NUM == data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
        {
            insert_order_info.longsms_max_num = data_head_ptr->user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
            insert_order_info.longsms_seq_num = data_head_ptr->user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS];
            insert_order_info.head_ref = data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS];
        }
        else if(MN_SMS_CONCAT_16_BIT_REF_NUM == data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
        {
            insert_order_info.longsms_max_num = data_head_ptr->user_header_arr[MMISMS_HEAD_16BIT_MAX_NUM_POS];
            insert_order_info.longsms_seq_num = data_head_ptr->user_header_arr[MMISMS_HEAD_16BIT_SEQ_NUM_POS];
            insert_order_info.head_ref = (data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS] << 8) 
                                                    | (data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS+1]) ;
        }
    }
//可以作为长短信的短信内容 s_sms_statusbar_scrollmsg.read_msg.read_content
#if defined MMI_STATUSBAR_SCROLLMSG_SUPPORT || defined DROPDOWN_NOTIFY_SHORTCUT
    if (!is_unhandle_order)
    {
        BOOLEAN is_same_sms = FALSE;
        BOOLEAN is_need_swap = FALSE;
        uint16 cur_index = 0;
        int32 i = 0;
        int32 start_pos = 0;
        BOOLEAN is_need_assemble_prompt =  TRUE;
        s_sms_statusbar_scrollmsg.is_concatenate_sms = insert_order_info.flag.is_concatenate_sms;
        s_sms_statusbar_scrollmsg.msg_type = MMISMS_READ_MTSMS;

        if (s_sms_statusbar_scrollmsg.is_concatenate_sms)
        {
            if (s_sms_statusbar_scrollmsg.dual_sys == dual_sys
                && s_sms_statusbar_scrollmsg.longsms_max_num == insert_order_info.longsms_max_num
                && s_sms_statusbar_scrollmsg.head_ref == insert_order_info.head_ref
                && ((s_sms_statusbar_scrollmsg.longsms_seq_num - 1)/MMISMS_SPLIT_MAX_NUM == (insert_order_info.longsms_seq_num - 1)/MMISMS_SPLIT_MAX_NUM)
                )
            {
          if(insert_order_info.longsms_seq_num > 0)
          {
               cur_index = (insert_order_info.longsms_seq_num - 1)%MMISMS_SPLIT_MAX_NUM;
          }
		  else
		  {
		       cur_index = 0;
		  }

                if (s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index] > 0
                    && s_sms_statusbar_scrollmsg.read_msg.read_content.length >= s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index]
                    && s_sms_statusbar_scrollmsg.read_msg.read_content.sms_num > 0
                    && s_sms_statusbar_scrollmsg.read_msg.sms_num > 0)
                {
                    for (i = 0; i < cur_index; i++)
                    {
                        start_pos += s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[i];
                    }

                    if ((cur_index == s_sms_statusbar_scrollmsg.read_msg.sms_num) && (cur_index + 1) == s_sms_statusbar_scrollmsg.longsms_max_num )
                    {
                        is_need_swap = FALSE;
                    }
                    else
                    {
                        is_need_swap = TRUE;
                    }

                    if (s_sms_statusbar_scrollmsg.read_msg.read_content.length - start_pos >= 0)
                    {
                        SCI_MEMCPY(s_sms_statusbar_scrollmsg.read_msg.read_content.data + start_pos,
                                            s_sms_statusbar_scrollmsg.read_msg.read_content.data + start_pos + s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index],
                                            s_sms_statusbar_scrollmsg.read_msg.read_content.length - start_pos - s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index]);

                        SCI_MEMSET(s_sms_statusbar_scrollmsg.read_msg.read_content.data + (s_sms_statusbar_scrollmsg.read_msg.read_content.length - s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index]),
                                            0x00,
                                            s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index]);
                        s_sms_statusbar_scrollmsg.read_msg.read_content.length -=  s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index];
                        s_sms_statusbar_scrollmsg.read_msg.read_content.sms_num --;
                    }

                    if (MMISMS_SPLIT_MAX_NUM - 1 == cur_index)
                    {
                        s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index] = 0;
                        s_sms_statusbar_scrollmsg.read_msg.read_content.alphabet_ori[cur_index] = MN_SMS_DEFAULT_ALPHABET;
                    }
                    else
                    {
                        for (i = cur_index; i < MMISMS_SPLIT_MAX_NUM - 1; i++)
                        {
                            s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[i] = s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[i + 1];
                            s_sms_statusbar_scrollmsg.read_msg.read_content.alphabet_ori[i] = s_sms_statusbar_scrollmsg.read_msg.read_content.alphabet_ori[i + 1];
                        }
                    }
                }

                is_same_sms = TRUE;
            }
        }

        s_sms_statusbar_scrollmsg.dual_sys = dual_sys;
        s_sms_statusbar_scrollmsg.storage = storage;
        s_sms_statusbar_scrollmsg.record_id = insert_order_info.record_id;
        s_sms_statusbar_scrollmsg.longsms_seq_num = insert_order_info.longsms_seq_num;
        s_sms_statusbar_scrollmsg.longsms_max_num = insert_order_info.longsms_max_num;
        s_sms_statusbar_scrollmsg.head_ref = insert_order_info.head_ref;

        if (!is_same_sms)
        {
            SCI_MEMSET(&s_sms_statusbar_scrollmsg.read_msg, 0x00, sizeof(s_sms_statusbar_scrollmsg.read_msg));
        }

        for(i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
        {
            if(s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[i] != 0)
            {
                is_need_assemble_prompt = FALSE;
                break;
            }
        }
        
        MMISMS_AssembleSmsToGlobal(s_sms_statusbar_scrollmsg.dual_sys,
                                                        s_sms_statusbar_scrollmsg.msg_type,
                                                        user_data_ptr,
                                                        orginal_addr_ptr,
                                                        alpha,
                                                        &s_sms_statusbar_scrollmsg.read_msg,
                                                        is_need_assemble_prompt,
                                                        is_need_assemble_prompt
                                                        );

        if (is_need_swap
            && s_sms_statusbar_scrollmsg.read_msg.read_content.sms_num > 0
            && s_sms_statusbar_scrollmsg.read_msg.read_content.length >= s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index])
        {
            MMISMS_CONTENT_T *read_content_ptr = PNULL;

            read_content_ptr = SCI_ALLOCAZ(sizeof(MMISMS_CONTENT_T));

            if (PNULL == read_content_ptr)
            {
                return FALSE;
            }

            SCI_MEMCPY(read_content_ptr, &s_sms_statusbar_scrollmsg.read_msg.read_content, sizeof(s_sms_statusbar_scrollmsg.read_msg.read_content));
            start_pos = 0;
            for (i = 0; i < cur_index; i++)
            {
                start_pos += s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[i];
            }

            SCI_MEMCPY(s_sms_statusbar_scrollmsg.read_msg.read_content.data + start_pos,
                                read_content_ptr->data + (s_sms_statusbar_scrollmsg.read_msg.read_content.length - read_content_ptr->split_sms_length[read_content_ptr->sms_num - 1]),
                                read_content_ptr->split_sms_length[read_content_ptr->sms_num - 1]);

            s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[cur_index] = s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[read_content_ptr->sms_num - 1];
            s_sms_statusbar_scrollmsg.read_msg.read_content.alphabet_ori[cur_index] = s_sms_statusbar_scrollmsg.read_msg.read_content.alphabet_ori[read_content_ptr->sms_num - 1];

            SCI_MEMCPY(s_sms_statusbar_scrollmsg.read_msg.read_content.data + start_pos + read_content_ptr->split_sms_length[read_content_ptr->sms_num - 1],
                                read_content_ptr->data + start_pos,
                                (s_sms_statusbar_scrollmsg.read_msg.read_content.length - read_content_ptr->split_sms_length[read_content_ptr->sms_num - 1] - start_pos));

            for (i = cur_index + 1; i < MIN(s_sms_statusbar_scrollmsg.longsms_max_num,MMISMS_SPLIT_MAX_NUM); i++)
            {
                s_sms_statusbar_scrollmsg.read_msg.read_content.split_sms_length[i] = read_content_ptr->split_sms_length[i - 1];
                s_sms_statusbar_scrollmsg.read_msg.read_content.alphabet_ori[i] = read_content_ptr->alphabet_ori[i - 1];
            }

            SCI_FREE(read_content_ptr);
        }
    }
#endif

    is_replace = MMISMS_ReplaceSms(pid_e, &insert_order_info);

    if (!is_replace)
    {
        if (is_unhandle_order)
        {
            MMISMS_InsertSmsToUnhandleOrder(&insert_order_info);
        }
        else
        {
            MMISMS_InsertSmsToOrder(&insert_order_info);  //trigger new sms
            MMISMS_SetNeedSort(TRUE);
        }
    }

#ifdef _ATEST_
#ifdef WIN32
    ATESTOTA_ProcessPressKeySerialByReceiveOta();  // added by Mingxuan.Zhu
#endif//WIN32
#endif//_ATEST_

    return is_replace;
}

/*****************************************************************************/
//     Description : operate MT ind as mms
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN OperateMtIndAsMmsPush(                                    //RETURN: TRUE, indicate the operation is finished
                                    MN_DUAL_SYS_E       dual_sys,
                                    APP_SMS_USER_DATA_T    *user_data_ptr,    //IN:
                                    MN_SMS_STORAGE_E    storage,        //IN:
                                    MN_SMS_RECORD_ID_T    record_id,        //IN:
                                    MN_SMS_MT_SMS_T *mt_sms_ptr//IN:
                                    )
{
    BOOLEAN is_finished = TRUE;
    BOOLEAN is_new_mms    = FALSE;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    APP_MMS_USER_DATA_T mms_user_data_t = {0};
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint16 num_len = 0;
    uint8 sc_tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint16 sc_num_len = 0;
    MN_CALLED_NUMBER_T *org_addr_ptr = PNULL;
    MN_CALLED_NUMBER_T *sc_addr_ptr = PNULL;
    
    if (PNULL == user_data_ptr || PNULL == mt_sms_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:OperateMtIndAsMmsPush user_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_1368_112_2_18_2_53_4_207,(uint8*)"");

        return FALSE;
    }

    org_addr_ptr = &mt_sms_ptr->origin_addr_t;
    sc_addr_ptr = &mt_sms_ptr->sc_addr_t;

    // 判断是否为mms
    err_code = MNSMS_IsSupportMMSEx(dual_sys,
        user_data_ptr->user_data_head_t.length,
        user_data_ptr->user_data_head_t.user_header_arr,
        &is_new_mms
        );

    //dont receive MMS,fixbug:1331369
	if(( ERR_MNSMS_NONE == err_code)&&(TRUE==is_new_mms))
	{
	    SCI_TRACE_LOW("MMISMS: OperateMtIndAsMmsPush is new MMS");
        return TRUE;
	}
    //SCI_TRACE_LOW:"MMISMS: OperateMtIndAsMmsPush err_code=%d, is_new_mms=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_1382_112_2_18_2_53_4_208,(uint8*)"dd", err_code, is_new_mms);
    
    if ( ( ERR_MNSMS_NONE == err_code) && (is_new_mms) )
    {    
        if (MMISMS_GetDelAllFlag()
#ifdef MMI_SMS_MOVE_SUPPORT            
            || MMISMS_GetMoveAllFlag()
#endif            
            || !MMISMS_IsMMSOrderReady()
            || MMIAPIUDISK_UdiskIsRun())
        {
            //插入到未处理连表中 //later
            InsertNewMtToOrder(
                dual_sys,
                storage, 
                record_id,
                MN_SMS_MT_TO_BE_READ,
                user_data_ptr,
                mt_sms_ptr,
                TRUE,
                TRUE
                );             

            is_finished = TRUE;
        }
        else
        {
            mms_user_data_t.user_data_head_t.length = MIN(user_data_ptr->user_data_head_t.length, 40);
            SCI_MEMCPY(
                mms_user_data_t.user_data_head_t.user_header_arr,
                user_data_ptr->user_data_head_t.user_header_arr,
                mms_user_data_t.user_data_head_t.length
                );
            
            //SCI_TRACE_LOW:"MMISMS: OperateMtIndAsMmsPush mms head length = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_1414_112_2_18_2_53_4_209,(uint8*)"d", mms_user_data_t.user_data_head_t.length);
            
            mms_user_data_t.user_valid_data_t = user_data_ptr->user_valid_data_t;
            
            if (org_addr_ptr->num_len > 0)//get number
            {
                num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(org_addr_ptr->number_type),
                                                                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                    org_addr_ptr->num_len),
                                                                    org_addr_ptr->party_num,
                                                                    tele_num,
                                                                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                                                    );
            }

            if (sc_addr_ptr->num_len > 0)//get number
            {
                sc_num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(sc_addr_ptr->number_type),
                                                                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                    sc_addr_ptr->num_len),
                                                                    sc_addr_ptr->party_num,
                                                                    sc_tele_num,
                                                                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                                                    );
            }

            //call the mms interface
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_DecodePush(dual_sys, &mms_user_data_t,tele_num,num_len,sc_tele_num,sc_num_len);
#endif            
            // call MN API to update SMS status.
            if(MN_SMS_NO_STORED != storage)
            {
                MNSMS_UpdateSmsStatusEx(dual_sys,storage, record_id, MN_SMS_FREE_SPACE);
            }
            is_finished = TRUE;
        }
    }
    else
    {
        is_finished = FALSE;
    }
    
    return (is_finished);
}

#if defined MMI_STATUSBAR_SCROLLMSG_SUPPORT || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : get read msg content
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetReadMsgContent(
                                    MMISMS_READMSG_CONTENT_T *read_content_ptr      //OUT
                                    )
{
    MMI_STRING_T num_name = {0};
//    uint16         nIndex = 0;
    //uint16         convert_msg_len = 0;
    uint8           time_len = 0;
//    wchar          time_string[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    char           time[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    uint8          date[MMISET_DATE_STR_LEN] = {0};
    uint8          time_str[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
//    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    //wchar    *string = PNULL;
//    wchar content_str[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
//    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID;
//    uint16 content_len = 0;    
    uint16 year = 0;
    //param
    MN_SMS_ALPHABET_TYPE_E    alphabet = 0;            //IN:
    //uint16                    msg_len = 0;            //IN:
    uint8                     *msg_data_ptr = PNULL;        //IN:
    MN_SMS_TIME_STAMP_T        *sc_time_ptr = PNULL;        //IN:
    MN_CALLED_NUMBER_T        *orginal_addr_ptr = PNULL;    //IN:
    MMISMS_READ_MSG_T        *read_msg_ptr = &s_sms_statusbar_scrollmsg.read_msg;


    if (PNULL == read_content_ptr)
    {
        return;
    }

    //param
    alphabet =    read_msg_ptr->read_content.alphabet;
    //msg_len =    read_msg_ptr->read_content.length;
    msg_data_ptr =    read_msg_ptr->read_content.data;
    sc_time_ptr = &(read_msg_ptr->time_stamp);
    orginal_addr_ptr =    &(read_msg_ptr->address);

    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));

    //Add item sender name or No.
    num_name.wstr_ptr = SCI_ALLOCAZ((MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar));
    if(PNULL == num_name.wstr_ptr)
    {

        return;
    }
    //SCI_MEMSET((uint8 *)num_name.wstr_ptr, 0, (MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar));
    GetTransmitterDispString(
        alphabet,
        orginal_addr_ptr,
        &num_name
        );
    
    if (PNULL != read_content_ptr->name_string.wstr_ptr)
    {
        read_content_ptr->name_string.wstr_len = MMIAPICOM_CopyStr2Buf(read_content_ptr->name_string.wstr_ptr, 
            &num_name, read_content_ptr->name_string.wstr_len);
    }

    if(MMISMS_READ_MTSMS == read_msg_ptr->read_type)
    {
        year = sc_time_ptr->year + MMISMS_MT_OFFSERT_YEAR;
    }

    else
    {
        year = sc_time_ptr->year + MMISMS_OFFSERT_YEAR;
    }
    MMIAPISET_FormatDateStrByDateStyle(year,sc_time_ptr->month, sc_time_ptr->day,
        '/',date,MMISET_DATE_STR_LEN);
    MMIAPISET_FormatTimeStrByTime(sc_time_ptr->hour, sc_time_ptr->minute,time_str,MMISET_TIME_STR_12HOURS_LEN + 1);
    //Add item SMS time
    sprintf(time, "%s %s",date,time_str);

    time_len = strlen( time );

    if (PNULL != read_content_ptr->time_string.wstr_ptr)
    {
        MMI_STRNTOWSTR( 
                read_content_ptr->time_string.wstr_ptr,
                read_content_ptr->time_string.wstr_len,
                (const uint8 *)time,
                read_content_ptr->time_string.wstr_len,
                read_content_ptr->time_string.wstr_len
                );
    }

    //Add item SMS content
    //convert_msg_len = msg_len;

    MMISMS_GetReadSmsDispContent(read_msg_ptr,read_content_ptr->content_string.wstr_ptr,&read_content_ptr->content_string.wstr_len);

    SCI_FREE(num_name.wstr_ptr);
    num_name.wstr_ptr = PNULL;


}


#endif



#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : to show the content of message
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void ShowMsgContent(
                          MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                          uint16                    msg_len,            //IN:
                          MMISMS_READ_MSG_T          *read_msg_ptr,   
                          MN_SMS_TIME_STAMP_T        *sc_time_ptr,        //IN:
                          MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                          MMISMS_R8_LANGUAGE_E    language,
#ifdef MMI_SMS_CHAT_SUPPORT                          
                          BOOLEAN                    is_chat_mode,
                          BOOLEAN                    is_need_time,
#endif                          
                          MMI_STRING_T                *string_ptr            //OUT:
                          )
{
    MMI_STRING_T num_name = {0};
    uint16 copy_offset = 0;
    //uint16 convert_msg_len = 0;
    uint8 time_len = 0;
    wchar time[40] = {0};
    wchar cr_ch = MMIAPICOM_GetCRCh();
    uint8 cr_len = 1;
    //wchar *content_r8_ptr = PNULL;
    uint16 display_content_len = 0;
    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));
    
    if (PNULL == orginal_addr_ptr)
    {

        //SCI_TRACE_LOW:"MMISMS:ShowMsgContent msg_data_ptr orginal_addr_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2145_112_2_18_2_53_6_216,(uint8*)"");
        return;
    }
    
    // change original address to display string
    // the address will be the name stored in phonebook
    // currently, if the address string is ucs2, then the whole string will be ucs2.
    // if you just display phone number, please modify the following function.
    // and modify the priority of the whole string. 

#ifdef MMI_SMS_CHAT_SUPPORT
    if(!is_chat_mode)
#endif        
    {
        num_name.wstr_ptr = SCI_ALLOC_APP((MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar));
        SCI_MEMSET((uint8 *)num_name.wstr_ptr, 0, (MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar));
        GetTransmitterDispString(
            alphabet,
            orginal_addr_ptr,
            &num_name
            );        
    }

#ifdef MMI_SMS_CHAT_SUPPORT 
    if(is_need_time)
#endif
    {
        time_len = MMISMS_GetTimeDispString(sc_time_ptr, time);
    }
    
    // the length of SMS content will be too large to allocate memory, so we just calculate the length.
    // in first line, display address
    // in second line, display time
    // and then display the SMS content.
    
    // in order to allocate memory for string, calculate the string length.


#ifdef MMI_SMS_CHAT_SUPPORT
    if(is_chat_mode)
    {
        if (0 < time_len)
        {
            string_ptr->wstr_len = time_len + cr_len + MMISMS_R8_MESSAGE_LEN;
        }
        else
        {
            string_ptr->wstr_len = MMISMS_R8_MESSAGE_LEN;
        }        
    }
    else
#endif 
    {
        if (0 < time_len)
        {
            string_ptr->wstr_len = num_name.wstr_len + cr_len + time_len + cr_len + MMISMS_R8_MESSAGE_LEN;
        }
        else
        {
            string_ptr->wstr_len = num_name.wstr_len + cr_len + MMISMS_R8_MESSAGE_LEN;
        }
    }
    
    
    // allocate memory
    SCI_TRACE_LOW("MMISMS: ShowMsgContent alloc length = %d",string_ptr->wstr_len);
    string_ptr->wstr_ptr = SCI_ALLOC_APP((string_ptr->wstr_len+1)*sizeof(wchar));
    SCI_MEMSET((uint8 *)string_ptr->wstr_ptr, 0x00, ((string_ptr->wstr_len+1)*sizeof(wchar)));
    
    copy_offset = 0;

#ifdef MMI_SMS_CHAT_SUPPORT
    if(!is_chat_mode)
#endif        
    {
        // address
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            num_name.wstr_len,
            num_name.wstr_ptr,
            num_name.wstr_len,
            num_name.wstr_len
            );
        
        copy_offset += num_name.wstr_len;
        // CR
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            cr_len,
            &cr_ch,
            cr_len,
            cr_len
            );
        
        copy_offset += cr_len;
    }
    
    if (0 < time_len)
    {
        // time
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            time_len,
            time,
            time_len,
            time_len
            );
        
        copy_offset += time_len;
        // CR
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            cr_len,
            &cr_ch,
            cr_len,
            cr_len
            );
        copy_offset += cr_len;
    }
    
    // message content

    MMISMS_GetReadSmsDispContent(read_msg_ptr,&(string_ptr->wstr_ptr[copy_offset]),&display_content_len);
    string_ptr->wstr_len =  (uint16)MMIAPICOM_Wstrlen(string_ptr->wstr_ptr);
        
    //SCI_TRACE_LOW:"MMISMS: ShowMsgContent length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2332_112_2_18_2_53_6_219,(uint8*)"d", string_ptr->wstr_len);

    if (PNULL != num_name.wstr_ptr)
    {
        SCI_FREE(num_name.wstr_ptr);
        num_name.wstr_ptr = PNULL;
    }
    
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
     
    return;
}
#endif

/*****************************************************************************/
//     Description : Get time display string
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetTimeDispString(                                        //RETURN: the length of time string
                              MN_SMS_TIME_STAMP_T    *sc_time_ptr,    //IN:
                              wchar                    *time_ptr        //OUT:
                              )
{
    uint8 time_tmp[20] = {0};
    uint8 time_len = 0;
    
    if (PNULL == time_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetTimeDispString time_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2359_112_2_18_2_53_6_220,(uint8*)"");
        return 0;
    }
    if (PNULL != sc_time_ptr)
    {
        time_len = MMIAPICOM_TranslateTime(
            sc_time_ptr,
            time_tmp
            );
        
        MMI_STRNTOWSTR(
            time_ptr,
            time_len,
            (const uint8 *)time_tmp,
            time_len,
            time_len
            );
    }
    
    return (time_len);
}

/*****************************************************************************/
//     Description : Get transmitter display string
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void GetTransmitterDispString(
                                    MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                    MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                    MMI_STRING_T            *string_ptr            //OUT:
                                    )
{
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 2] = {0};
#if 0
// no use
    uint16 num_len = 0;
#endif
    wchar pb_name[MMISMS_PBNAME_MAX_LEN + 2] = {0};
    MMI_STRING_T num_name = {0};
    BOOLEAN find_result = FALSE;
    
    if (PNULL == orginal_addr_ptr || PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetTransmitterDispString orginal_addr_ptr string_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2399_112_2_18_2_53_6_221,(uint8*)"");
        return;
    }
    
    num_name.wstr_len  = 0;
    num_name.wstr_ptr = pb_name;
    
    if (orginal_addr_ptr->num_len > 0)
    {
        //indicate the phonebook info of the address is not saved.
        //SCI_TRACE_LOW:"MMISMS: GetTransmitterDispString number_type = %d, number_plan = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2410_112_2_18_2_53_6_222,(uint8*)"dd",orginal_addr_ptr->number_type, orginal_addr_ptr->number_plan);
        
#if 0
// no use
        num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(orginal_addr_ptr->number_type),
                                                                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                    orginal_addr_ptr->num_len),
                                                                    orginal_addr_ptr->party_num,
                                                                    tele_num,
                                                                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                                                    );
#else
        MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(orginal_addr_ptr->number_type),
                                                                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                    orginal_addr_ptr->num_len),
                                                                    orginal_addr_ptr->party_num,
                                                                    tele_num,
                                                                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                                                    );

#endif

        find_result = MMISMS_GetNameByNumberFromPB(tele_num,&num_name, MMISMS_PBNAME_MAX_LEN);    
        
        if ((!find_result) 
            || (0 == num_name.wstr_len))
        {
            num_name.wstr_len = strlen((char *)tele_num);

            num_name.wstr_len = MIN(MMISMS_PBNAME_MAX_LEN, num_name.wstr_len);

            MMI_STRNTOWSTR(num_name.wstr_ptr,
                                        num_name.wstr_len,
                                        (const uint8 *)tele_num,
                                        num_name.wstr_len,
                                        num_name.wstr_len);

        }
    }
    else
    {
        num_name.wstr_len  = 0;
    }
    
    string_ptr->wstr_len  = num_name.wstr_len;

    MMI_WSTRNCPY(
        string_ptr->wstr_ptr,
        MMISMS_PBNAME_MAX_LEN,
        num_name.wstr_ptr,
        MMISMS_PBNAME_MAX_LEN,
        string_ptr->wstr_len
        );
}
#ifdef HERO_ENGINE_SUPPORT
#undef HERO_LOG
#define HERO_LOG(x)

#ifndef MIN
#define MIN(x, y)           (((x) < (y))?(x):(y))
#endif

#define HERO_APP_PDU_MAX_ITEM  (10)

#define HERO_APP_PDU_IND_LIST_MAX_COUNT  (15)

typedef struct tagHeroAppPudIndItemSt{
    unsigned short msgRef;
	unsigned short count;
	APP_MN_SMS_IND_T *pudIndList[HERO_APP_PDU_IND_LIST_MAX_COUNT];
}heroAppPudIndItemSt;

typedef struct tagHeroAppPduIndDataSt{
	heroAppPudIndItemSt item[HERO_APP_PDU_MAX_ITEM];
}heroAppPduIndDataSt;

static heroAppPduIndDataSt gHeroAppPduIndList = {0};

typedef struct tagHeroAppAsynHandleMsgSt{
	int timerId;
	int isNotify;
	APP_MN_SMS_IND_T pudData;
}heroAppAsynHandleMsgSt;

BOOL hero_mmi_msg_new_sms_ind(unsigned char *pContent, int nLen, char *pNum, int type);

static void hero_app_asyn_pdu_timer_cb(void *p)
{
	heroAppAsynHandleMsgSt *pData = (heroAppAsynHandleMsgSt*)p;
	HERO_LOG(("hero_app_asyn_pdu_timer_cb pData = %d", pData));
	if(pData != NULL)
	{
		hero_delete_timer(pData->timerId);

		HERO_LOG(("hero_app_asyn_pdu_timer_cb pData->isNotify = %d", pData->isNotify));
		if(pData->isNotify)
		{
			MMISMS_HandleSMSMTInd(&pData->pudData);
		}
		else
		{
			APP_MN_SMS_IND_T *sig_ptr = &pData->pudData;
			BOOLEAN is_saved_in_sim = sig_ptr->storage != MN_SMS_STORAGE_ME;

			HERO_LOG(("hero_app_asyn_pdu_timer_cb sig_ptr->record_id = %d, sig_ptr->storage = %d, is_saved_in_sim = %d", sig_ptr->record_id, sig_ptr->storage, is_saved_in_sim));
	#if 0
			MMISMS_DelSMSByAT(sig_ptr->dual_sys, TRUE, is_saved_in_sim, sig_ptr->record_id);
	#else
		#ifdef _USE_OLD_L4_API
			MNSMS_UpdateSmsStatus(sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
		#else
			MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
		#endif
	#endif
		}

		SCI_FREE(pData);
	}
}

static void hero_app_free_pdu_ind_list(unsigned short msgRef, int isNotify)
{
	int index = 0;
	heroAppPudIndItemSt *pduItem = &gHeroAppPduIndList.item[msgRef%HERO_APP_PDU_MAX_ITEM];

	for(index = 0; index < HERO_APP_PDU_IND_LIST_MAX_COUNT; index++)
	{
		APP_MN_SMS_IND_T *pduIndData = pduItem->pudIndList[index];
		HERO_LOG(("hero_app_free_pdu_ind_list pduIndData = %d, msgRef = %d, index = %d", pduIndData, msgRef, index));
		if(pduIndData != NULL)
		{
			heroAppAsynHandleMsgSt *pData = (heroAppAsynHandleMsgSt*)SCI_ALLOCA(sizeof(heroAppAsynHandleMsgSt));
			if(pData != NULL)
			{
				memset(pData, 0, sizeof(heroAppAsynHandleMsgSt));

				pData->isNotify = isNotify;
				pData->timerId = hero_create_timerId();
				if(pData->timerId != 0)
				{
					memcpy(&pData->pudData, pduIndData, sizeof(APP_MN_SMS_IND_T));
					hero_start_timer(pData->timerId, ((index == 0) ? 10 : index * 200), hero_app_asyn_pdu_timer_cb, (void*)pData);
				}
				else
				{
					SCI_FREE(pData);
				}
			}
			SCI_FREE(pduIndData);
		}
	}
	memset(pduItem, 0, sizeof(heroAppPudIndItemSt));
}

static char *hero_app_assemble_pdu(heroAppPudIndItemSt *pduItem, int *contentLen, char *numberBuff, int numberBufSize)
{
	int index = 0;
	int copyPos = 0;
	int contentBufLen = 0;
	char *tempContentBuf = NULL;
	uint8 tempUcsBuf[MN_SMS_MAX_USER_VALID_DATA_LENGTH<<1] = {0};

	contentBufLen = (pduItem->count * MN_SMS_MAX_USER_VALID_DATA_LENGTH + 1) << 1;
	tempContentBuf = (char*)SCI_ALLOCA(contentBufLen);
	HERO_LOG(("hero_app_assemble_pdu contentBufLen = %d, tempContentBuf = %d", contentBufLen, tempContentBuf));
	if(tempContentBuf == NULL)
	{
		return NULL;
	}
	memset(tempContentBuf, 0, contentBufLen);

	for(index = 0; index < HERO_APP_PDU_IND_LIST_MAX_COUNT; index++)
	{
		APP_MN_SMS_IND_T *sig_ptr = pduItem->pudIndList[index];
		if(sig_ptr != NULL)
		{
			int copyLen = 0;
			int dataSize = 0;
			MN_SMS_USER_VALID_DATA_T *data = NULL;
			MN_CALLED_NUMBER_T *number = NULL;
			APP_SMS_USER_DATA_T sms_user_data_t = {0};

			MMISMS_DecodeUserHeadAndContent(sig_ptr->dual_sys, 
				sig_ptr->sms_t.user_head_is_exist, &(sig_ptr->sms_t.dcs), &(sig_ptr->sms_t.user_data_t), &sms_user_data_t);
			data = &sms_user_data_t.user_valid_data_t;
			number = &sig_ptr->sms_t.origin_addr_t;

			memset(numberBuff, 0, numberBufSize);
			MMIAPICOM_GenDispNumber(number->number_type,
				MIN(numberBufSize, number->num_len),
				number->party_num,
				numberBuff,
				numberBufSize);

			memset(tempUcsBuf, 0, sizeof(tempUcsBuf));
			if(sig_ptr->sms_t.dcs.alphabet_type == MN_SMS_DEFAULT_ALPHABET)
			{
				uint16 dataLen = 0;
				uint8 user_data_arr[MN_SMS_MAX_USER_VALID_DATA_LENGTH] = {0};

				SCI_MEMSET(user_data_arr, '\0', sizeof(user_data_arr));
				dataLen = MMIAPICOM_Default2Ascii(data->user_valid_data_arr, user_data_arr, data->length);

				MMIAPICOM_StrToWstr(user_data_arr, (wchar*)tempUcsBuf);
				hr_str_convert_endian(tempUcsBuf);
			}
			else
			{
				if(sig_ptr->sms_t.dcs.alphabet_type == MN_SMS_8_BIT_ALPHBET)
				{
					GUI_UTF8ToWstr((wchar*)tempUcsBuf, sizeof(tempUcsBuf), data->user_valid_data_arr, data->length);
				}
				else
				{
					memcpy(tempUcsBuf, data->user_valid_data_arr, data->length);
				}
			}

			dataSize = MMIAPICOM_Wstrlen(tempUcsBuf) * 2;
			copyLen = MIN(contentBufLen - copyPos, dataSize);
			memcpy(tempContentBuf + copyPos, tempUcsBuf, copyLen);

			copyPos += copyLen;
		}
	}

	////
	if(contentLen != NULL)
	{
		*contentLen = copyPos;
	}
	return tempContentBuf;
}

PUBLIC void HeroApp_MMISMS_HandleSMSMTInd(DPARAM param)
{
	do
	{
	    APP_SMS_USER_DATA_T sms_user_data_t = {0};
		APP_MN_SMS_IND_T *sig_ptr = (APP_MN_SMS_IND_T *)param;

		if(sig_ptr == NULL)
		{
			break;
		}
		MMISMS_DecodeUserHeadAndContent(sig_ptr->dual_sys,
			sig_ptr->sms_t.user_head_is_exist, &(sig_ptr->sms_t.dcs), &(sig_ptr->sms_t.user_data_t), &sms_user_data_t);

		if(sms_user_data_t.user_data_head_t.length > 0)
		{
			uint8 ref = 0;
			uint8 longsms_max_num = 0;
			uint8 longsms_seq_num = 0;
			uint8 segmentsIndex = 0;
			heroAppPudIndItemSt *pduItem = NULL;
			APP_MN_SMS_IND_T *tempPduData = NULL;
			MN_SMS_USER_DATA_HEAD_T *data_head_ptr = &sms_user_data_t.user_data_head_t;

			if(MN_SMS_CONCAT_8_BIT_REF_NUM == data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
			{
				ref = data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS];
				longsms_max_num = data_head_ptr->user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
				longsms_seq_num = data_head_ptr->user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS];
			}
			else if(MN_SMS_CONCAT_16_BIT_REF_NUM == data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
			{
				ref = data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS];
				longsms_max_num = data_head_ptr->user_header_arr[MMISMS_HEAD_16BIT_MAX_NUM_POS];
				longsms_seq_num = data_head_ptr->user_header_arr[MMISMS_HEAD_16BIT_SEQ_NUM_POS];
			}
			else
			{
				MMISMS_HandleSMSMTInd(param);
				break;
			}

			HERO_LOG(("ref = %d", ref));
			pduItem = &gHeroAppPduIndList.item[ref%HERO_APP_PDU_MAX_ITEM];
			if(pduItem->msgRef != ref)
			{
				hero_app_free_pdu_ind_list(ref, 1);
				pduItem->msgRef = ref;
			}
			pduItem->count++;
			HERO_LOG(("hero_app_handle_new_sms pduItem->count = %d", pduItem->count));

			tempPduData = SCI_ALLOCA(sizeof(APP_MN_SMS_IND_T));
			if(tempPduData == NULL)
			{
				break;
			}
			memset(tempPduData, 0, sizeof(APP_MN_SMS_IND_T));
			segmentsIndex = MIN(longsms_seq_num, HERO_APP_PDU_IND_LIST_MAX_COUNT) - 1;
			pduItem->pudIndList[segmentsIndex] = tempPduData;
			memcpy(tempPduData, sig_ptr, sizeof(APP_MN_SMS_IND_T));

			HERO_LOG(("longsms_seq_num = %d, longsms_max_num = %d, pduItem->count = %d", longsms_seq_num, longsms_max_num, pduItem->count));
			if(pduItem->count >= longsms_max_num)
			{
				char *tempContentBuf = NULL;
				do
				{
					int contentLen = 0;
					char numberBuf[64] = {0};

					tempContentBuf = hero_app_assemble_pdu(pduItem, &contentLen, numberBuf, sizeof(numberBuf));
					if(tempContentBuf == NULL)
					{
						hero_app_free_pdu_ind_list(ref, 1);
						break;
					}
					if(hero_mmi_msg_new_sms_ind((unsigned char*)tempContentBuf, contentLen, numberBuf, HR_ENCODE_UNICODE))
					{
						hero_app_free_pdu_ind_list(ref, 0);
					}
					else
					{
						hero_app_free_pdu_ind_list(ref, 1);
					}
				}while(0);
				if(tempContentBuf != NULL)
				{
					SCI_FREE(tempContentBuf);
				}
			}
			break;
		}

		MMISMS_HandleSMSMTInd(param);
	}while(0);
}
#endif

/*****************************************************************************/
//     Description : to handle the signal APP_MN_SMS_IND
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSMSMTInd( 
                                  DPARAM    param    //IN:
                                  )
{    
    APP_MN_SMS_IND_T *sig_ptr = (APP_MN_SMS_IND_T *)param;
    APP_SMS_USER_DATA_T sms_user_data_t = {0};  
#if defined(MMI_BLACKLIST_SUPPORT)
    MMIPB_BCD_NUMBER_T orig_num = {0};
#endif
    MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};
#ifdef MRAPP_SUPPORT
    int32 result = 0;
#endif
    
#ifdef HERO_ENGINE_SUPPORT
    int32 hr_result = 0;
#endif
    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSMSMTInd sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2473_112_2_18_2_53_6_223,(uint8*)"");
        return;
    }

#ifdef SFR_SUPPORT
    if (MMISFR_SMSGetRunningStatus(sig_ptr->dual_sys))
    {
        SCI_TRACE_LOW("MMISMS:MMISMS_HandleSMSMTInd Self Reg Cnf");
        if(MMISFR_SmsIsSelfRegCnfInfo(param))
        {
            BOOLEAN is_saved_in_sim = sig_ptr->storage != MN_SMS_STORAGE_ME;
            //删除短信
            MMISMS_DelSMSByAT(
                sig_ptr->dual_sys,
                TRUE,
                is_saved_in_sim,
                sig_ptr->record_id);
            SCI_TRACE_LOW("MMISMS:MMISMS_HandleSMSMTInd Self Reg Del SFR SMS");
            return;
        }
    }
#endif

      //MMISMS_ModifyNum(sig_ptr);  
    SCI_MEMSET( &sms_user_data_t, 0x00, sizeof( APP_SMS_USER_DATA_T ) );

#if defined(TTS_SUPPORT_YOUNGTONE)
		{
			extern int yte_tts_handle_sms_ind ( void * param );
			if ( yte_tts_handle_sms_ind(param ) == 1 )
			{
				return;
			}
		}
#endif
    
    //SCI_TRACE_LOW:"MMISMS: HandleSMSMTInd user_head_is_exist = %d, record_id = %d, storage = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2480_112_2_18_2_53_6_224,(uint8*)"ddd", sig_ptr->sms_t.user_head_is_exist, sig_ptr->record_id, sig_ptr->storage);

#if defined(MMI_BLACKLIST_SUPPORT)
    //if num in blacklist,block and delete the message
#ifdef MET_MEX_SUPPORT
	if (1 == MMIMex_Phs_HookResult(MEX_MSG_ID_SMS_DELIVER_MSG_IND, param, 0, NULL, MEX_HOOK_MSG_FROM_MMITASK))
	{
        BOOLEAN is_saved_in_sim = sig_ptr->storage != MN_SMS_STORAGE_ME;
        mex_log(1, "hook SMS MT");

        //删除短信
        MMISMS_DelSMSByAT(
            sig_ptr->dual_sys,
            TRUE,
            is_saved_in_sim,
            sig_ptr->record_id);
        //mex application registered to hook this message

        return;
	}
#ifdef MET_MEX_AQ_SUPPORT
        if (1==MMIMex_Aq_SmsWall(sig_ptr->dual_sys,&sig_ptr->sms_t.origin_addr_t))
        {
            //删除短信
            MMISMS_DelSMSByAT(
                sig_ptr->dual_sys,
                TRUE,
                (sig_ptr->storage != MN_SMS_STORAGE_ME),
                sig_ptr->record_id); 
			//mexsystem already  do the next
            return;
        }
#endif
        //判断是否正在运行全屏应用
        if(MMIMEX_IsSysRunning())
        {
            //显示新短信提示
            MMIMEX_SmsReceivePromt();
        }
#endif

    orig_num.number_len = sig_ptr->sms_t.origin_addr_t.num_len;
    orig_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(sig_ptr->sms_t.origin_addr_t.number_type, MN_NUM_PLAN_UNKNOW );
    SCI_MEMCPY(&orig_num.number,&sig_ptr->sms_t.origin_addr_t.party_num,orig_num.number_len);  

    if(MMIAPISET_IsSMSBlacklist(&orig_num))
    {
        MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys,sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
        return;
    }
#endif
    
    // need call MN API to decode user head and data.
    MMISMS_DecodeUserHeadAndContent(
        sig_ptr->dual_sys,
        sig_ptr->sms_t.user_head_is_exist,
        &(sig_ptr->sms_t.dcs),
        &(sig_ptr->sms_t.user_data_t),
        &sms_user_data_t
        );
#ifdef HERO_ENGINE_SUPPORT
	hr_result = MMIHEROAPP_hrSMSfilter(sig_ptr->sms_t.dcs.alphabet_type, &sms_user_data_t.user_valid_data_t, &sig_ptr->sms_t.origin_addr_t); /*lint !e718 !e18*/
	if(hr_result == 0 || hr_result == -1)
	{
#ifdef _USE_OLD_L4_API
		MNSMS_UpdateSmsStatus(sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE );
#else
		MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE );
#endif	
		return;
	}
#endif

#ifdef MRAPP_SUPPORT
	result = MMIMRAPP_dsmSMSfilter(sig_ptr->sms_t.dcs.alphabet_type, &sms_user_data_t.user_valid_data_t, &sig_ptr->sms_t.origin_addr_t); /*lint !e718 !e18*/
	if(result == 0 || result == -1)
	{
#ifdef _USE_OLD_L4_API
		MNSMS_UpdateSmsStatus(sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE );
#else
		MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE );
#endif	
		return;//is mrp configure files
	}
#endif

#ifdef MOBILE_VIDEO_SUPPORT
    CM_IsMvSms(&sms_user_data_t);
#endif

    #ifdef JAVA_SUPPORT_SUN
    #ifndef _MSC_VER
	/*检查是不是JAVA短消息*/
    if(APPSMS_IsAppSms_CallBack(sig_ptr->dual_sys,&sms_user_data_t)){
        APPSMS_RecvSms_CallBack(&(sig_ptr->sms_t));
        if(MN_SMS_NO_STORED != sig_ptr->storage)
        {
            MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys,sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
        }
        return;
    }
    #endif
    #endif

#ifdef WRE_SUPPORT
	{
            extern BOOLEAN  WRESMS_RecvSms_CallBack(MN_DUAL_SYS_E dual_sys,uint32 alphabet,MN_CALLED_NUMBER_T * origin_addr,APP_SMS_USER_DATA_T *sms_user_data_t);

            if(TRUE == WRESMS_RecvSms_CallBack(sig_ptr->dual_sys,sig_ptr->sms_t.dcs.alphabet_type,&sig_ptr->sms_t.origin_addr_t,&sms_user_data_t))
            {
                MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys,sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
                return;
            }
	}
#endif	

#if defined(OD_GAME_SUPPORT)
	 {
	 	 extern  BOOLEAN od_game_unlock_game_by_sms(MN_DUAL_SYS_E dual_sys,uint32 alphabet,MN_CALLED_NUMBER_T * origin_addr,APP_SMS_USER_DATA_T *sms_user_data_t);
		 if(od_game_unlock_game_by_sms(sig_ptr->dual_sys,sig_ptr->sms_t.dcs.alphabet_type,&sig_ptr->sms_t.origin_addr_t,&sms_user_data_t))
		 {
        #ifdef _USE_OLD_L4_API
				 MNSMS_UpdateSmsStatus(sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE );
        #else
				 MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE );
        #endif  
			 return;
		 }	  
	 }
#endif
    if( (sig_ptr->sms_t.dcs.class_is_present) &&
        (MN_SMS_CLASS_0 == sig_ptr->sms_t.dcs.sms_class) )
    {

        BOOLEAN is_finished = FALSE;    

#ifdef ZDT_PLAT_YX_SUPPORT
		//add by bao for del broad sms
        if(1)
        {
            MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys,sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
            return;
        }
#endif
        if (0 == sms_user_data_t.user_data_head_t.length
            || (sig_ptr->dual_sys != g_mmisms_global.class0_msg.dual_sys)
            || (0 != memcmp(&g_mmisms_global.class0_msg.address,&sig_ptr->sms_t.origin_addr_t,sizeof(MN_CALLED_NUMBER_T)))
            || (MMISMS_GetRefNumFromUserHead(&sms_user_data_t.user_data_head_t) != MMISMS_GetRefNumFromUserHead(&g_mmisms_global.class0_msg.head_data))
            || g_mmisms_global.class0_msg.read_content.sms_num > MMISMS_SPLIT_MAX_NUM
            || (g_mmisms_global.class0_msg.read_content.length + sms_user_data_t.user_valid_data_t.length > (MMISMS_SPLIT_MAX_NUM * MMISMS_DEFAULT_CHAR_LEN))
        )
        {
            SCI_MEMSET(&g_mmisms_global.class0_msg,0x0,sizeof(MMISMS_READ_MSG_T));
        }

        g_mmisms_global.class0_msg.read_type = MMISMS_READ_MTSMS;
        g_mmisms_global.class0_msg.dual_sys = sig_ptr->dual_sys;
        SCI_MEMCPY(&g_mmisms_global.class0_msg.time_stamp,&sig_ptr->sms_t.time_stamp_t,sizeof(MN_SMS_TIME_STAMP_T));
        SCI_MEMCPY(&g_mmisms_global.class0_msg.head_data,&sms_user_data_t.user_data_head_t,sizeof(MN_SMS_USER_DATA_HEAD_T));
        SCI_MEMCPY(&g_mmisms_global.class0_msg.address,&sig_ptr->sms_t.origin_addr_t,sizeof(MN_CALLED_NUMBER_T));
        g_mmisms_global.class0_msg.read_content.alphabet = sig_ptr->sms_t.dcs.alphabet_type;
        g_mmisms_global.class0_msg.class_is_present = sig_ptr->sms_t.dcs.class_is_present;
        g_mmisms_global.class0_msg.sms_class = sig_ptr->sms_t.dcs.sms_class;
        g_mmisms_global.class0_msg.read_content.sms_num++;
        SCI_MEMCPY(&g_mmisms_global.class0_msg.read_content.data[g_mmisms_global.class0_msg.read_content.length],
                   sms_user_data_t.user_valid_data_t.user_valid_data_arr,
                   sms_user_data_t.user_valid_data_t.length);

        g_mmisms_global.class0_msg.read_content.length += sms_user_data_t.user_valid_data_t.length;

        MMISMS_DecodeUserDataHead(&(sms_user_data_t.user_data_head_t), &user_head_ptr_t);
        g_mmisms_global.class0_msg.language = MMISMS_JudgeR8Language(&user_head_ptr_t);
        MMISMS_DecodeR8Header(
                &user_head_ptr_t,
                &g_mmisms_global.class0_msg.is_lock_shift,
                &g_mmisms_global.class0_msg.is_single_shift);

        MMISMS_FreeUserDataHeadSpace(&user_head_ptr_t);

        if (0 == sms_user_data_t.user_data_head_t.length
            || (MMISMS_GetMaxNumFromUserHead(&sms_user_data_t.user_data_head_t) == g_mmisms_global.class0_msg.read_content.sms_num))
        {
            MMI_STRING_T            num_name ={0}; 
            wchar                   name_str[MMISMS_PBNAME_MAX_LEN +1] = {0};
            MMISMS_SetSimIdOfNewMsg(sig_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMIDEFAULT_TurnOnBackLight();
            
            num_name.wstr_ptr = name_str;
            GetTransmitterDispString(                   //fixed 146484
                MN_SMS_DEFAULT_ALPHABET, //no use para
                &g_mmisms_global.class0_msg.address,
                &num_name);
            if('_' == num_name.wstr_ptr[0] && '@' == num_name.wstr_ptr[1])
            {
                SCI_MEMSET(&g_mmisms_global.class0_msg.address,0x00,sizeof(MN_CALLED_NUMBER_T));
            }
            MMISMS_EnterShowClass0Win(g_mmisms_global.class0_msg);
        }

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
        is_finished = MMISMS_IsVoiceMailMessage(sig_ptr->dual_sys,
                                                                        &sms_user_data_t,
                                                                        sig_ptr->sms_t.dcs,
                                                                        sig_ptr->storage,
                                                                        sig_ptr->record_id);
        if (is_finished)
        {
            MMISMS_UpdateVMFlagState();

            g_is_sms_vm_header = FALSE;
        }
#endif        
    }
    else
    {
        BOOLEAN is_finished = FALSE;
#ifdef ZDT_SMS_SUPPORT
        BOOLEAN is_zdt_sms = FALSE;
        is_finished = OperateMtIndForZdt(
            sig_ptr->dual_sys,
            &sms_user_data_t,
            sig_ptr->storage,
            sig_ptr->record_id,
            &sig_ptr->sms_t
            );
        if(is_finished)
        {
            is_zdt_sms = TRUE;
        }
#endif
        // the SMS class is not 0, then must followed the operate step
        // 1 operate as java
        // 2 operate as mms
        // 3 operate as SMS
         is_finished = MMIAPIDM_HandleSmsMtInd(
                                                 sig_ptr->dual_sys,
                                                 sig_ptr->sms_t.origin_addr_t.party_num,
                                                 sig_ptr->sms_t.origin_addr_t.num_len,
                                                 sms_user_data_t
                                                 ); 
        if (is_finished)
        {
            if(MN_SMS_NO_STORED != sig_ptr->storage)
            {
                MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys,sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
            }
        }
//BEGIN @huangns 20111103  add for myriad java
#if defined(JAVA_SUPPORT_MYRIAD)
        is_finished = OperateMtIndForMyriad(
            sig_ptr->dual_sys,
            &sms_user_data_t,
            sig_ptr->storage,
            sig_ptr->record_id,
            sig_ptr->sms_t
            );
#endif
//END @huangns_20111103z
#ifdef JAVA_SUPPORT_IA
        if (!is_finished)
        {
            is_finished = OperateMtIndForJava(
                sig_ptr->dual_sys,
                &sms_user_data_t,
                sig_ptr->storage,
                sig_ptr->record_id,
                sig_ptr->sms_t
                );
        }
#endif     
        if (!is_finished)

        {
            MMIVC_SMS_SEND_TYPE_E send_type = MMIVC_SMS_SEND_NORMAL;
            send_type = MMISMS_OperateMtIndForVcardOrVcal(
                sig_ptr->dual_sys,
                &sms_user_data_t,
                sig_ptr->storage,
                sig_ptr->record_id,
                sig_ptr->sms_t
                );
            if(MMIVC_SMS_SEND_NORMAL != send_type)
            {
                is_finished = TRUE;
            }
        }
        
        if (!is_finished)
        {
            is_finished = OperateMtIndAsMmsPush(
                sig_ptr->dual_sys,
                &sms_user_data_t,
                sig_ptr->storage,
                sig_ptr->record_id,
                &sig_ptr->sms_t
                );
        }
        
        if (!is_finished)
        {
            is_finished = MMISMS_IsSmartMessage(sig_ptr->dual_sys,
                                                                            &sms_user_data_t,
                                                                            sig_ptr->storage,
                                                                            sig_ptr->record_id);
        }
        
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
        if (!is_finished)
        {
            is_finished = MMISMS_IsVoiceMailMessage(sig_ptr->dual_sys,
                                                                            &sms_user_data_t,
                                                                            sig_ptr->sms_t.dcs,
                                                                            sig_ptr->storage,
                                                                            sig_ptr->record_id);
        }
#endif
        
        if (!is_finished)
        {
            is_finished = OperateMtIndAsSms(
                sig_ptr->dual_sys,
                &sms_user_data_t,
                sig_ptr->storage,
                sig_ptr->record_id,
                &sig_ptr->sms_t
                );
        }
    }
    //Add for CR 103433
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim((sig_ptr->dual_sys)));    
#endif
}

/*****************************************************************************/
//     Description : to handle the new message window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleNewReportWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
     case MSG_CLOSE_WINDOW:
         MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
         MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);
         MMIENVSET_StopOnetimeVibTimer();
         MMIAPIENVSET_SetVibStatus(FALSE);    
        // MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);    
         recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
         break;             
         
     default:
         recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
         break;   
    }
    
    return (recode);        
}

/*****************************************************************************/
//     Description : to handle the signal APP_MN_SMS_STATUS_REPORT_IND
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSMSReportInd( 
                                      DPARAM    param    //IN:
                                      )
{
    APP_MN_SMS_STATUS_REPORT_IND_T *sig_ptr = (APP_MN_SMS_STATUS_REPORT_IND_T*)param;    
    // APP_SMS_USER_DATA_T sms_user_data_t = {0};  
    MMI_STRING_T            status_str = {0};
    MMI_STRING_T            status_str1 = {0};
#if 0
// no use
    MMISMS_OPER_ERR_E result = MMISMS_MAX_ERR;
#endif
    MMI_WIN_ID_T alert_win_id = MMISMS_STATUE_REPORT_WIN_ID;
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    uint32 time_out = 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    
    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSMSReportInd sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2736_112_2_18_2_53_7_225,(uint8*)"");
        return;
    }
    if (MMISMS_IsOrderOk())
    {
        MMISMS_SetNewSMSRingFlag(TRUE);
        MMIDEFAULT_TurnOnBackLight();
        MMISMS_PlayNewSMSRing(sig_ptr->dual_sys);

        // status
        status_str.wstr_ptr = SCI_ALLOC_APP((MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar));
        SCI_MEMSET((uint8 *)status_str.wstr_ptr, 0, ((MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar)));
        GetTransmitterDispString(
            MN_SMS_UCS2_ALPHABET,
            &(sig_ptr->status_report.dest_addr_t),
            &status_str
            );

        SCI_TRACE_LOW("MMISMS_HandleSMSReportInd tp_status=%d", sig_ptr->status_report.tp_status);

        if (MN_SMS_RECEIVED_BY_SME == sig_ptr->status_report.tp_status
            || MN_SMS_NOT_CONFIRM_RECEIVED == sig_ptr->status_report.tp_status
            || MN_SMS_SM_REPLACED_BY_SC == sig_ptr->status_report.tp_status
            || MN_SMS_SEND_LATER == sig_ptr->status_report.tp_status)
        {
            text_id = TXT_SMS_SR_SUCC;
        }
        else
        {
            text_id = TXT_COMM_SEND_FAILED;
        }

        MMI_GetLabelTextByLang(text_id, &status_str1);

        if(MMK_IsOpenWin(alert_win_id))
        {
            MMK_CloseWin(alert_win_id);
        }

        if(MMISMS_IsForbitNewMsgWin(FALSE))
        {
            win_priority = WIN_LOWEST_LEVEL;
        }
        else
        {
            win_priority = WIN_ONE_LEVEL;
        }

        MMIPUB_OpenAlertWinByTextPtr(&time_out,&status_str,&status_str1,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,MMISMS_HandleNewReportWin);

        //Set Pubwin Title
        {
            MMI_STRING_T title_str = {0};

            MMI_GetLabelTextByLang(TXT_SMS_STATUSREPORT, &title_str);

            MMIPUB_SetWinTitleText(alert_win_id, &title_str, FALSE);
        }

#if 0
// no use
        //MMISMS_SetOperStatus(update_oper);
        result = MMISMS_UpdateStatusReportMsgReq(
                                sig_ptr->dual_sys,
                                MN_SMS_FREE_SPACE,
                                sig_ptr->storage,
                                sig_ptr->record_id);
#else
        //MMISMS_SetOperStatus(update_oper);
        MMISMS_UpdateStatusReportMsgReq(
                                sig_ptr->dual_sys,
                                MN_SMS_FREE_SPACE,
                                sig_ptr->storage,
                                sig_ptr->record_id);

#endif
        if(PNULL != status_str.wstr_ptr)
        {
            SCI_FREE(status_str.wstr_ptr);
            status_str.wstr_ptr = PNULL;
        }

        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSMSReportInd dual_sys is %d,storage is %d,record_id is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2798_112_2_18_2_53_7_226,(uint8*)"ddd",sig_ptr->dual_sys,sig_ptr->storage,sig_ptr->record_id);
    }
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Add for CR 103433
    MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim((sig_ptr->dual_sys)));   
#endif
}



/*****************************************************************************/
//     Description : Set readed SMS to window
//    Global resource dependence : g_mmisms_global
//  Author: rong gu
//    Note:获取时间
/*****************************************************************************/
PUBLIC void MMISMS_GetReadSmsDispTime(
                                         MMISMS_READ_MSG_T *read_msg_ptr, //in
                                         wchar *time_string_ptr,//out
                                         uint16 *time_string_len //out
                                     )
{
    MN_SMS_TIME_STAMP_T    *time_stamp_ptr = PNULL;
    char           time[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    uint8          date[MMISET_DATE_STR_LEN] = {0};
    uint8          time_str[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    uint8           time_len = 0;
    uint16 year = 0;
    if(PNULL == read_msg_ptr || PNULL == time_string_ptr)
    {
        return;
    }
    time_stamp_ptr = &(read_msg_ptr->time_stamp);
    if((MMISMS_READ_MTSMS == read_msg_ptr->read_type)
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        && (MMISMS_FOLDER_BT != g_mmisms_global.read_msg.folder_type)
#endif
        )
    {
        year = time_stamp_ptr->year + MMISMS_MT_OFFSERT_YEAR;
    }    
    else  //MO time 
    {
        year = time_stamp_ptr->year + MMISMS_OFFSERT_YEAR;
    }
    MMIAPISET_FormatDateStrByDateStyle(year,time_stamp_ptr->month, time_stamp_ptr->day,
        '/',date,MMISET_DATE_STR_LEN);
    MMIAPISET_FormatTimeStrByTime(time_stamp_ptr->hour, time_stamp_ptr->minute,time_str,MMISET_TIME_STR_12HOURS_LEN + 1);
    //Add item SMS time
    sprintf(time, "%s %s",date,time_str);
    
    time_len = strlen( time );
    
    MMI_STRNTOWSTR( 
            time_string_ptr,
            time_len,
            (const uint8 *)time,
            time_len,
            time_len
            );	
    
    *time_string_len = time_len;
}
/*****************************************************************************/
//     Description : Set readed SMS to window
//    Global resource dependence : g_mmisms_global
//  Author: rong gu
//    Note:获取显示号码
/*****************************************************************************/
PUBLIC void MMISMS_GetReadSmsDispNum(
                                         MMISMS_READ_MSG_T *read_msg_ptr, //in
                                         wchar *num_string_ptr,//out
                                         uint16 *num_string_len //out
                                     )
{    
    MMI_STRING_T            num_name = {0};        
    if(PNULL == read_msg_ptr || PNULL == num_string_ptr)
    {
        return;
    }
        
    num_name.wstr_ptr = num_string_ptr;
    GetTransmitterDispString(
        MN_SMS_DEFAULT_ALPHABET, //no use para
        &read_msg_ptr->address,
        &num_name
        );
    *num_string_len = num_name.wstr_len;
}
/*****************************************************************************/
//     Description : Set readed SMS to window
//    Global resource dependence : g_mmisms_global
//  Author: rong gu
//    Note:获取显示内容
/*****************************************************************************/
PUBLIC void MMISMS_GetReadSmsDispContent(
                                         MMISMS_READ_MSG_T *read_msg_ptr, //in
                                         wchar *content_string_ptr,//out
                                         uint16 *content_string_len //out
                                     )
{
    uint32 i = 0;
    uint16 offset = 0;
    uint16 data_offset = 0;
    uint16 single_sms_len = 0;
    if(PNULL == read_msg_ptr || PNULL == content_string_ptr || PNULL == content_string_len)
    {
        return;
    }        

    if(g_mmisms_global.class0_msg.class_is_present && MN_SMS_CLASS_0 == g_mmisms_global.class0_msg.sms_class)
    {
        MMISMS_ConvertValidDataToDispStr(content_string_ptr,
            content_string_len,
            read_msg_ptr->read_content.data,
            MIN(MMISMS_SPLIT_MAX_NUM*MMISMS_DEFAULT_CHAR_LEN,read_msg_ptr->read_content.length),//MMISMS_R8_MESSAGE_LEN
            read_msg_ptr->read_content.alphabet,
            read_msg_ptr->language,
            read_msg_ptr->is_lock_shift,
            read_msg_ptr->is_single_shift
            );
    }
    else
    {
        for (i = 0; i < MIN(read_msg_ptr->read_content.sms_num, MMISMS_SPLIT_MAX_NUM); i++)
        {
            if (0 != read_msg_ptr->read_content.split_sms_length[i])
            {
                SCI_TRACE_LOW("mmisms split_sms_length %d",read_msg_ptr->read_content.split_sms_length[i]);
                MMISMS_ConvertValidDataToDispStr(content_string_ptr + offset,
                    &single_sms_len,
                    &read_msg_ptr->read_content.data[data_offset],
                    MIN(MMISMS_MAX_DEF_BYTE_LEN,read_msg_ptr->read_content.split_sms_length[i]),  //修改coverity错误防止数组越界，原参数为MMISMS_R8_MESSAGE_LEN yongsheng.wang
                    read_msg_ptr->read_content.alphabet_ori[i],
                    read_msg_ptr->language,
                    read_msg_ptr->is_lock_shift,
                    read_msg_ptr->is_single_shift
                    );
                offset += single_sms_len;
                data_offset += read_msg_ptr->read_content.split_sms_length[i];              
            }
        }
        *content_string_len = offset;
    }
}
/*****************************************************************************/
// Description :Is Smart Message
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSmartMessage( 
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                APP_SMS_USER_DATA_T *user_data_ptr,
                                                                                MN_SMS_STORAGE_E storage,
                                                                                MN_SMS_RECORD_ID_T record_id
                                                                                )
{
    uint8 iei_num = 0;
    int32 i = 0;
    int32 index = -1;
    int32 des_port_num = 0;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    BOOLEAN is_smart_message = FALSE;
    
    MNSMS_DecodeUserDataHeadEx(
        dual_sys,
        &user_data_ptr->user_data_head_t,
        &user_head_ptr_t);

    // Check whether Smart Message
    iei_num = user_head_ptr_t.iei_num;
    index = -1;
    for (i = 0; i < iei_num; i++)
    {
        if( MN_SMS_APP_PORT_16_BIT_ADDR == user_head_ptr_t.iei_arr[i].iei)
        {
            index = i;
            des_port_num = (user_head_ptr_t.iei_arr[i].iei_data_t[0] << 8) | (user_head_ptr_t.iei_arr[i].iei_data_t[1]);
            break;
        }
    }
    SCI_TRACE_LOW("SMS: MMISMS_IsSmartMessage: index = %d  port = %d ", index, des_port_num);

    if (-1 != index && MMISMS_SMART_MESSAGE_PORT_NUM == des_port_num )
    {
        is_smart_message = TRUE;
    }

    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);

    if (is_smart_message)
    {
        if (MN_SMS_NO_STORED != storage)
        {
            MNSMS_UpdateSmsStatusEx(dual_sys,storage, record_id, MN_SMS_FREE_SPACE);
        }

        MMIDEFAULT_TurnOnBackLight();
        //MMIAPISMS_PlayNewSMSRing(dual_sys);

        MMIPUB_OpenAlertWarningWin(SS_CONTENT_NOT_SUPPORTED);
    }

    return is_smart_message;
}


#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
// Description :Is Voice Mail Message
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsVoiceMailMessage( 
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                APP_SMS_USER_DATA_T *user_data_ptr,
                                                                                MN_SMS_DCS_T dcs,
                                                                                MN_SMS_STORAGE_E storage,
                                                                                MN_SMS_RECORD_ID_T record_id
                                                                                )
{
    uint8 iei_num = 0;
    int32 i = 0;
    int32 index = -1;
    int32 vm_count = 0;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    BOOLEAN is_vm_message = FALSE;

    SCI_TRACE_LOW("SMS: MMISMS_IsVoiceMailMessage: msg_is_waiting = %d wait_type = %d is_active_wait_ind=%d", dcs.msg_is_waiting, dcs.wait_type, dcs.is_active_wait_ind);

    if (dcs.msg_is_waiting)
    {
        MNSMS_DecodeUserDataHeadEx(
            dual_sys,
            &user_data_ptr->user_data_head_t,
            &user_head_ptr_t);

        // Check whether Voice Message
        iei_num = user_head_ptr_t.iei_num;
        index = -1;
        for (i = 0; i < iei_num; i++)
        {
            if (MN_SMS_SPEC_MESS_IND == user_head_ptr_t.iei_arr[i].iei)
            {
                index = i;
                vm_count = (user_head_ptr_t.iei_arr[i].iei_data_t[0] << 8);
                break;
            }
        }
        SCI_TRACE_LOW("SMS: MMISMS_IsVoiceMailMessage: index = %d vm_count = %d ", index, vm_count);

        is_vm_message = TRUE;

        MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);
    }

    if (is_vm_message)
    {
        MN_SMS_VMW_IND_FLAG_T vmw_flag = {0};
        uint8 line = MMIAPISET_GetCallLineInfo(dual_sys);

        if (MN_SMS_NO_STORED != storage)
        {
            MNSMS_UpdateSmsStatusEx(dual_sys,storage, record_id, MN_SMS_FREE_SPACE);
        }

        switch(dcs.wait_type)
        {
        case MN_SMS_VOICE_MSG_WAIT:
            {
                if(1 == line)
                {
                
#if defined (MODEM_SUPPORT_SPRD)
                    vmw_flag.line2_status = dcs.is_active_wait_ind;
#elif defined(MODEM_SUPPORT_RDA)
                    vmw_flag.line2_waiting_count = dcs.is_active_wait_ind;
#endif
                }
                else if(0 == line)
                {
#if defined (MODEM_SUPPORT_SPRD)
                    vmw_flag.line1_status = dcs.is_active_wait_ind;
#elif defined(MODEM_SUPPORT_RDA)
                    vmw_flag.line1_waiting_count = dcs.is_active_wait_ind;
#endif
                }
            }
            break;

        case MN_SMS_FAX_MSG_WAIT:
            vmw_flag.is_datafax_flag_present = TRUE;
#if defined (MODEM_SUPPORT_SPRD)
            vmw_flag.fax_status = dcs.is_active_wait_ind;
#elif defined(MODEM_SUPPORT_RDA)
            vmw_flag.fax_waiting_count = dcs.is_active_wait_ind;
#endif
            break;

        case MN_SMS_EMAIL_MSG_WAIT:
            vmw_flag.is_datafax_flag_present = TRUE;
#if defined (MODEM_SUPPORT_SPRD)
            vmw_flag.data_status = dcs.is_active_wait_ind;
#elif defined(MODEM_SUPPORT_RDA)
            vmw_flag.data_waiting_count = dcs.is_active_wait_ind;
#endif
            break;

        case MN_SMS_OTHER_MSG_WAIT:
            return TRUE;

        default:
            return FALSE;
        }

        if (dcs.is_active_wait_ind)//Set VM Flag
        {
            if (0 == vm_count)
            {
                vm_count = 1;
            }
        }
        else //Clear VM flag
        {
            vm_count = 0;
        }

        //临时注调，需要后期改正
//        vmw_flag.msg_waiting_count = vm_count;

        g_is_sms_vm_header = TRUE;

        MMISMS_SetVMInfo(dual_sys, &vmw_flag);
    }

    return is_vm_message;
}
#endif


/*****************************************************************************/
// Description :
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMIVC_SMS_SEND_TYPE_E MMISMS_IsMtForVcard( 
                                                 MN_DUAL_SYS_E         dual_sys,                                  
                                                 APP_SMS_USER_DATA_T   *user_data_ptr)
{
    uint8 iei_num = 0;
    int32 i = 0;
    int32 index = -1;
    int32 des_port_num = 0;
    MMIVC_SMS_SEND_TYPE_E send_type = MMIVC_SMS_SEND_NORMAL;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    
    //SCI_TRACE_LOW:"SMS: MMISMS_IsMtForVcard()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2855_112_2_18_2_53_7_227,(uint8*)"");
    MNSMS_DecodeUserDataHeadEx(
        dual_sys,
        &user_data_ptr->user_data_head_t,
        &user_head_ptr_t);
    // Check whether Vcard sms
    iei_num = user_head_ptr_t.iei_num;
    index = -1;
    for (i = 0;i < iei_num;i++)
    {
        if( MN_SMS_APP_PORT_16_BIT_ADDR == user_head_ptr_t.iei_arr[i].iei)
        {
            index = i;
            des_port_num = (user_head_ptr_t.iei_arr[i].iei_data_t[0] << 8) | (user_head_ptr_t.iei_arr[i].iei_data_t[1]);
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: MMISMS_IsMtForVcard: index = %d  port = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2872_112_2_18_2_53_7_228,(uint8*)"dd", index, des_port_num);
    if (-1 != index && MMISMS_VCARD_PORT_NUM == des_port_num )
    {
        send_type =  MMIVC_SMS_SEND_VCARD;
    }
    else if (-1 != index && MMISMS_VCAL_PORT_NUM == des_port_num ) //no need vcalendar macro
    {
        send_type =  MMIVC_SMS_SEND_VCALENDAR;
    }

    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);

    return send_type;
}

/*****************************************************************************/
//     Description : to set the user data head
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_VcardOrVcalInitUserDataHead(
                                    uint8 max_num,        //IN:
                                    uint8 seq_num,        //IN: the seq number (1 ~ 8)
                                    MN_SMS_USER_DATA_HEAD_T  *data_head_ptr  //OUT: the user data head
                                    )
{
    MMINV_SMS_REFERENCE_NUM_T ref_num = 0;

    if (PNULL == data_head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InitUserDataHead data_head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1570_112_2_18_2_53_19_368,(uint8*)"");
        return;
    }
    if (0 == max_num || max_num > MMISMS_SPLIT_MAX_NUM)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InitUserDataHead 0 >= max_num || max_num > MMISMS_SPLIT_MAX_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1575_112_2_18_2_53_19_369,(uint8*)"");
        return;
    }
    if (0 == seq_num || seq_num > max_num)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InitUserDataHead 0 >= seq_num || seq_num > max_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1580_112_2_18_2_53_19_370,(uint8*)"");
        return;
    }    
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_InitUserDataHead(%d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SEND_1584_112_2_18_2_53_19_371,(uint8*)"dd", max_num, seq_num);

    SCI_MEMSET( data_head_ptr, 0, sizeof(MN_SMS_USER_DATA_HEAD_T) );
    
    if ( max_num > 1 )
    {
        if (1 == seq_num)
        {
            // get the refer number
            ref_num = g_mmisms_global.nv_param.ref_num ;
            g_mmisms_global.save_info.ref_num = ref_num;
            MMISMS_SetRefNumToNV( (MMINV_SMS_REFERENCE_NUM_T)( ref_num + 1 ) );
        }
        else
        {
            ref_num = g_mmisms_global.nv_param.ref_num;
            if(ref_num != 0)
            {
                ref_num--;
            }
            g_mmisms_global.save_info.ref_num = ref_num;
        }
        
        // set the head of user data according to spec
        SET_HEAD_VAR( data_head_ptr, ref_num,  max_num, seq_num );               
    } 
}

/*****************************************************************************/
// Description : operate MT ind as for Vcard or Vcal
// Global resource dependence :
// Author:rong.gu
// Note: 
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_RecvVcardOrVcalAsNormalSms(
                                     MN_DUAL_SYS_E         dual_sys, 
                                     MN_SMS_STORAGE_E       storage, 
                                     MN_SMS_RECORD_ID_T	    record_id,
                                     MN_SMS_MT_SMS_T        sms_t,
                                     APP_SMS_USER_DATA_T   *sms_ptr,   //vcard,vcal
                                     MMIVC_SMS_SEND_TYPE_E send_type
                                     )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MT_SMS_T mt_sms = {0};
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0}; 
    MN_SMS_USER_DATA_HEAD_T user_data_head = {0}; //普通sms的头
    int32 index = -1; 
    int32 i = 0;
    uint8 iei_num = 0; 
    uint8 max_num = 0;
    uint8 seq_num = 0;
    MN_SMS_STORAGE_E store_storage = MN_SMS_STORAGE_ME;
    MN_SMS_RECORD_ID_T new_record_id = 0;
    uint16 start_pos = 0;
    if (PNULL == sms_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveMsgReq user_data_ptr  = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SAVE_287_112_2_18_2_53_9_256,(uint8*)"");
        return MMISMS_SAVE_FAIL;
    }    
    
    SCI_MEMSET(&mt_sms, 0, sizeof(MN_SMS_MT_SMS_T));
    
    if (sms_t.origin_addr_t.num_len > 0)
    {
        g_mmisms_global.save_info.number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(sms_t.origin_addr_t.number_type, MN_NUM_PLAN_UNKNOW);
        g_mmisms_global.save_info.number.number_len = sms_t.origin_addr_t.num_len;
        SCI_MEMCPY(g_mmisms_global.save_info.number.number, 
                              sms_t.origin_addr_t.party_num,
                              sms_t.origin_addr_t.num_len);
    }

      
    
    SCI_MEMCPY(&mt_sms,&sms_t,sizeof(MN_SMS_MT_SMS_T));
    SCI_MEMSET(&mt_sms.user_data_t,0,sizeof(MN_SMS_USER_DATA_T));
    
    //MMISMS_InitUserDataHead(total_num, 1, &edit_content_ptr->tpdu_head, send_info_ptr);
    MMISMS_GetStoreStorage(MN_DUAL_SYS_1,&store_storage,&new_record_id);

    MNSMS_DecodeUserDataHeadEx(
        dual_sys, 
        &sms_ptr->user_data_head_t,
        &user_head_ptr_t);

    index = -1;
    iei_num = user_head_ptr_t.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM  == user_head_ptr_t.iei_arr[i].iei ||
            MN_SMS_CONCAT_16_BIT_REF_NUM == user_head_ptr_t.iei_arr[i].iei )
        {
            index = i;
            break;
        }
    }
    if(index >= 0)
    {
        SCI_TRACE_LOW("MMISMS:sms index found");
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == user_head_ptr_t.iei_arr[index].iei)
        {
            max_num = user_head_ptr_t.iei_arr[index].iei_data_t[1];
            seq_num = user_head_ptr_t.iei_arr[index].iei_data_t[2];    
        }
        else if (MN_SMS_CONCAT_16_BIT_REF_NUM == user_head_ptr_t.iei_arr[index].iei)
        {
            max_num  = user_head_ptr_t.iei_arr[index].iei_data_t[2];
            seq_num  = user_head_ptr_t.iei_arr[index].iei_data_t[3];
        }
        
        SCI_TRACE_LOW("MMISMS: max_num = %d,seq_num = %d",max_num,seq_num);
        g_mmisms_global.save_info.max_num = max_num;
        g_mmisms_global.save_info.seq_num = seq_num;
        MMISMS_VcardOrVcalInitUserDataHead(max_num,seq_num,&user_data_head);                 
    }
    else
    {
        sms_t.user_head_is_exist = 0;
		mt_sms.user_head_is_exist = 0;
    }

    if(1 == seq_num || 0 == seq_num)
    {
        SCI_MEMSET(&g_mmisms_global.read_msg,0x0,sizeof(MMISMS_READ_MSG_T));
    }
    
    SCI_MEMCPY(&g_mmisms_global.read_msg.address,&sms_t.origin_addr_t,sizeof(MN_CALLED_NUMBER_T));
    g_mmisms_global.read_msg.read_content.alphabet = sms_t.dcs.alphabet_type;
    g_mmisms_global.read_msg.read_content.length = sms_ptr->user_valid_data_t.length;
    if(sms_t.dcs.alphabet_type == MN_SMS_8_BIT_ALPHBET)
    {
        SCI_MEMCPY(&g_mmisms_global.read_msg.ascii_vaild.user_valid_data_t.user_valid_data_arr,sms_ptr->user_valid_data_t.user_valid_data_arr,sms_ptr->user_valid_data_t.length);
        g_mmisms_global.read_msg.ascii_vaild.user_valid_data_t.length = sms_ptr->user_valid_data_t.length;
    }
    else
    {    
        if(seq_num > 0 && seq_num <= MMISMS_SPLIT_MAX_NUM)
        {
            g_mmisms_global.read_msg.read_content.split_sms_length[seq_num - 1] = sms_ptr->user_valid_data_t.length;
            for (i = 0;i < seq_num - 1;i++)
            {
                start_pos += g_mmisms_global.read_msg.read_content.split_sms_length[i];
            }
            SCI_MEMCPY(&g_mmisms_global.read_msg.read_content.data[start_pos],sms_ptr->user_valid_data_t.user_valid_data_arr,sms_ptr->user_valid_data_t.length);
        }   
        else
        {
            SCI_MEMCPY(&g_mmisms_global.read_msg.read_content.data,sms_ptr->user_valid_data_t.user_valid_data_arr,sms_ptr->user_valid_data_t.length);
        }
    }
     
    MNSMS_EncodeUserDataEx(
        dual_sys,
        sms_t.user_head_is_exist,    
        sms_t.dcs.alphabet_type,
        &user_data_head,
        &sms_ptr->user_valid_data_t,
        &mt_sms.user_data_t
        );

    err_code = MNSMS_WriteMtSmsEx(dual_sys, store_storage, MN_SMS_MT_TO_BE_READ, &mt_sms);
    
    MMISMS_SetOperStatus(MMISMS_SAVE_VCARD_VCAL_SMS);

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

    if(max_num == seq_num && MN_SMS_NO_STORED != store_storage)
    {
        SCI_TRACE_LOW("MMISMS:update sms record id = %d",record_id);
        MNSMS_UpdateSmsStatusEx(dual_sys,storage,record_id,MN_SMS_FREE_SPACE);
    }
    return (save_result);
}
/*****************************************************************************/
// Description : operate MT ind as for Vcard
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMIVC_SMS_SEND_TYPE_E MMISMS_OperateMtIndForVcardOrVcal(
                                                               MN_DUAL_SYS_E         dual_sys,                                  
                                                               APP_SMS_USER_DATA_T   *user_data_ptr,
                                                               MN_SMS_STORAGE_E       storage,
                                                               MN_SMS_RECORD_ID_T     record_id,
                                                               MN_SMS_MT_SMS_T        sms_t)
{
    BOOLEAN is_finished = TRUE;
    MMIVC_SMS_SEND_TYPE_E send_type = MMIVC_SMS_SEND_NORMAL;
#ifdef WIN32
    //    APP_SMS_USER_DATA_T test_data = {{ 0x05,
    //                                       0x05,0x04,0x23,0xf4,0x00,0x00
    //                                     },
    //                                     { 0x5f,
    //                                       0x42,0x45,0x47,0x49,0x4e,0x3a,0x56,0x43,0x41,0x52,
    //                                       0x44,0x0d,0x0a,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e,
    //                                       0x3a,0x32,0x2e,0x31,0x0d,0x0a,0x4e,0x3b,0x43,0x48,
    //                                       0x41,0x52,0x53,0x45,0x54,0x3d,0x55,0x54,0x46,0x2d,
    //                                       0x38,0x3a,0xe9,0x83,0x91,0xe5,0x9b,0xbd,0xe5,0xb8,
    //                                       0x85,0x0d,0x0a,0x54,0x45,0x4c,0x3b,0x50,0x52,0x45,
    //                                       0x46,0x3b,0x56,0x4f,0x49,0x43,0x45,0x3a,0x2b,0x38,
    //                                       0x36,0x31,0x33,0x37,0x38,0x34,0x31,0x30,0x37,0x31,
    //                                       0x35,0x36,0x0d,0x0a,0x45,0x4e,0x44,0x3a,0x56,0x43,
    //                                       0x41,0x52,0x44,0x0d,0x0a
    //                                     }
    //                                    };  
    //    user_data_ptr = &test_data;
#endif

    if (PNULL == user_data_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_OperateMtIndForVcardOrVcal user_data_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2923_112_2_18_2_53_7_229,(uint8*)"");
        return send_type;
    }
    // 判断是否为Vcard sms
    send_type = MMISMS_IsMtForVcard(dual_sys, user_data_ptr);
    //SCI_TRACE_LOW:"SMS: OperateMtIndForVcard: send_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2928_112_2_18_2_53_7_230,(uint8*)"d",send_type);
    if (MMIVC_SMS_SEND_NORMAL != send_type)
    {    
        if(
            0 //lint !e774
#ifdef   MMI_VCARD_SUPPORT 
            || MMIVC_SMS_SEND_VCARD == send_type 
#endif               
#ifdef   MMI_VCALENDAR_SUPPORT          
            || MMIVC_SMS_SEND_VCALENDAR == send_type    //lint !e506
#endif            
            )//lint !e506
        {
            MMISMS_RecvVcardOrVcalSms(dual_sys, user_data_ptr, &sms_t.time_stamp_t,send_type); 
    
            // call MN API to update SMS status.
            if(MN_SMS_NO_STORED != storage)
            {
                MNSMS_UpdateSmsStatusEx(dual_sys,storage,record_id,MN_SMS_FREE_SPACE);
            }
            is_finished = TRUE;
        }
        else
        {
			if(MMISMS_IsOrderOk())
			{
				MMISMS_RecvVcardOrVcalAsNormalSms(dual_sys,storage,record_id, sms_t, user_data_ptr,send_type);                       
			}
            is_finished = TRUE;    
        }
    }
    else
    {
        is_finished = FALSE;
    }
    //SCI_TRACE_LOW:"SMS: OperateMtIndForVcard: is_finished = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2944_112_2_18_2_53_7_231,(uint8*)"d",is_finished);
    return send_type;
}

/*****************************************************************************/
// Description : receive Vcard over sms
// Global resource dependence :s_long_vcardsms_ptr
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_RecvVcardOrVcalSms(
                                     MN_DUAL_SYS_E         dual_sys, 
                                     APP_SMS_USER_DATA_T   *sms_ptr,
                                     MN_SMS_TIME_STAMP_T   *time_stamp_ptr,
                                     MMIVC_SMS_SEND_TYPE_E send_type
                                     )
{
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    VCARD_LONG_SMS_DATA_T vcard_data = {0};
    uint8 receive_num = 1;
    uint8 iei_num = 0;
    uint8 sms_index[MMIVC_MAX_LONG_SMS_NUM]={0};
    uint8 data_length = 0;
    uint32 i = 0;
    uint8 j = 1;
    int32 index = -1;
    uint16 ref_num = 0;
    uint8 max_num = 0;
    uint8 seq_num = 0;
    uint32 current_time = 0;
    int32 empty_pos = -1;
    uint8 name_arr[VCARD_NAME_MAX_LEN +1] = {0};
    VCARD_LONG_SMS_T *send_type_ptr =PNULL;
    
    if (PNULL == sms_ptr || PNULL == time_stamp_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:OperateMtIndAsSms sms_ptr time_stamp_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2978_112_2_18_2_53_7_232,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_2981_112_2_18_2_53_7_233,(uint8*)"");
    MNSMS_DecodeUserDataHeadEx(
        dual_sys, 
        &sms_ptr->user_data_head_t,
        &user_head_ptr_t);
    index   = -1;
    iei_num = user_head_ptr_t.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM  == user_head_ptr_t.iei_arr[i].iei ||
            MN_SMS_CONCAT_16_BIT_REF_NUM == user_head_ptr_t.iei_arr[i].iei )
        {
            index = i;
            break;
        }
    }

    if(MMIVC_SMS_SEND_VCARD == send_type)
    {
        send_type_ptr = s_long_vcardsms_ptr;
    }
#ifdef MMI_VCALENDAR_SUPPORT
    else if(MMIVC_SMS_SEND_VCALENDAR == send_type)
    {
        send_type_ptr = s_long_vcalsms_ptr;
    }
#endif
    //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms  index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3008_112_2_18_2_53_7_234,(uint8*)"d", index);
    //是长短信
    if (index != -1)
    {
        if (PNULL == send_type_ptr)//指针为空
        {
            send_type_ptr = InitLongVcardsmsStruct();//申请内存

            if (PNULL == send_type_ptr) //申请不到
            {
                goto RECV_RETURN;
            }

            if(MMIVC_SMS_SEND_VCARD == send_type)
            {
               s_long_vcardsms_ptr= send_type_ptr ;
            }
#ifdef MMI_VCALENDAR_SUPPORT
            else if(MMIVC_SMS_SEND_VCALENDAR == send_type)
            {
                s_long_vcalsms_ptr=send_type_ptr ;
            }
#endif
        }

        //获取长短信相关信息
        data_length = sms_ptr->user_valid_data_t.length;
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == user_head_ptr_t.iei_arr[index].iei)
        {
            ref_num = user_head_ptr_t.iei_arr[index].iei_data_t[0];
            max_num = user_head_ptr_t.iei_arr[index].iei_data_t[1];
            seq_num = user_head_ptr_t.iei_arr[index].iei_data_t[2];    
        }
        else if (MN_SMS_CONCAT_16_BIT_REF_NUM == user_head_ptr_t.iei_arr[index].iei)
        {
            ref_num  = user_head_ptr_t.iei_arr[index].iei_data_t[0];
            ref_num  = ref_num << 8;
            ref_num += user_head_ptr_t.iei_arr[index].iei_data_t[1];
            max_num  = user_head_ptr_t.iei_arr[index].iei_data_t[2];
            seq_num  = user_head_ptr_t.iei_arr[index].iei_data_t[3];
        }
        //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms ref_num=%d,max_num=%d,seq_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3050_112_2_18_2_53_7_235,(uint8*)"ddd",ref_num, max_num, seq_num);
        //重复判断
        for(i = 0;i < MMIVC_MAX_LONG_SMS_NUM;i++)
        {
            //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms ref_num=%d,max_num=%d,seq_num=%d,vcard_sms[i].ref_num=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3055_112_2_18_2_53_7_236,(uint8*)"dddd",ref_num, max_num, seq_num,send_type_ptr->vcard_sms[i].ref_num);
            if (ref_num == send_type_ptr->vcard_sms[i].ref_num)
            {
                if (seq_num != send_type_ptr->vcard_sms[i].seq_num)
                {
                    sms_index[receive_num] = i;
                    receive_num++;
                }
                else
                {
                    //已经保存有同样的短信
                    //SCI_TRACE_LOW:"MMISMS_RecvVcardOrVcalSms the same seq sms"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3066_112_2_18_2_53_7_237,(uint8*)"");
                    goto RECV_RETURN;
                }
            }
        }
        //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms receive_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3071_112_2_18_2_53_7_238,(uint8*)"d", receive_num);
        //长短信未到齐
        if (receive_num < max_num)
        {
            //搜索空白位置，保存此条信息
            for(i = 0;i < MMIVC_MAX_LONG_SMS_NUM;i++)
            {
                if (send_type_ptr->vcard_sms[i].recv_time == 0)
                {
                    empty_pos = i;
                    break;
                }
            }
            //清理超时短信:如果五分钟内不能来齐，则丢弃
            current_time = GetSmsReceiveTimeStamp();
            //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms i=%d,current_time=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3086_112_2_18_2_53_7_239,(uint8*)"dd", i,current_time);
            if(i == MMIVC_MAX_LONG_SMS_NUM)
            {
                for(i = 0;i < MMIVC_MAX_LONG_SMS_NUM;i++)
                {
                    if (((send_type_ptr->vcard_sms[i].recv_time + 5*MMISMS_MINUTE_TIME < current_time)
                        ||(send_type_ptr->vcard_sms[i].recv_time > current_time))
                        &&(ref_num != send_type_ptr->vcard_sms[i].ref_num))
                    {
                        SCI_MEMSET(&(send_type_ptr->vcard_sms[i]),0,sizeof(VCARD_SMS_T));
                        send_type_ptr->vcard_sms[i].ref_num = MMISMS_INVAILD_INDEX;
                        send_type_ptr->vcard_sms[sms_index[i]].is_valid = FALSE;
                        empty_pos = i;
                        //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms i=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3099_112_2_18_2_53_7_240,(uint8*)"d", i);
                        break;
                    }
                }
            }
            //将收到的信息保存到全局变量
            //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms empty_pos=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3105_112_2_18_2_53_7_241,(uint8*)"d", empty_pos);
            if ((empty_pos < MMIVC_MAX_LONG_SMS_NUM) && (empty_pos != -1))
            {
                //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms write global var,seq_num=%d,ref_num=%d,max_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3108_112_2_18_2_53_7_242,(uint8*)"ddd",seq_num,ref_num,max_num);
                send_type_ptr->vcard_sms[empty_pos].recv_time = current_time;
                send_type_ptr->vcard_sms[empty_pos].seq_num   = seq_num;
                send_type_ptr->vcard_sms[empty_pos].ref_num   = ref_num;
                send_type_ptr->vcard_sms[empty_pos].max_num   = max_num;
                send_type_ptr->vcard_sms[empty_pos].length    = data_length;
                send_type_ptr->vcard_sms[empty_pos].is_valid    = TRUE;
                SCI_MEMCPY(send_type_ptr->vcard_sms[empty_pos].data_arr,
                    sms_ptr->user_valid_data_t.user_valid_data_arr,
                    data_length);
            }
            goto RECV_RETURN;
        }
        else
        {
            //长短信已到齐，合并长短信
            //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms write vcf file"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3124_112_2_18_2_53_8_243,(uint8*)"");
            j = 1;
            while(j <= receive_num)
            {
                for(i = 1;i < receive_num;i++)
                {
                    if (send_type_ptr->vcard_sms[sms_index[i]].seq_num == j)
                    {
                        SCI_MEMCPY(vcard_data.data+vcard_data.length,
                                            send_type_ptr->vcard_sms[sms_index[i]].data_arr,
                                            send_type_ptr->vcard_sms[sms_index[i]].length);
                        vcard_data.length += send_type_ptr->vcard_sms[sms_index[i]].length;
                        SCI_MEMSET(&(send_type_ptr->vcard_sms[sms_index[i]]),
                                            0, sizeof(VCARD_SMS_T));

                        send_type_ptr->vcard_sms[sms_index[i]].ref_num = MMISMS_INVAILD_INDEX;
                        send_type_ptr->vcard_sms[sms_index[i]].is_valid = FALSE;
                        break;
                    }
                    else if (seq_num == j)
                    {
                        SCI_MEMCPY(vcard_data.data+vcard_data.length,
                            sms_ptr->user_valid_data_t.user_valid_data_arr,
                            sms_ptr->user_valid_data_t.length);
                        vcard_data.length += sms_ptr->user_valid_data_t.length;
                        break;
                    }    
                }
                j++;
            }            

            //检查是否还有未组装的Vcard
            for(i = 0;i < MMIVC_MAX_LONG_SMS_NUM; i++)
            {
                if (send_type_ptr->vcard_sms[i].is_valid)
                {
                    break;
                }
            }

            if (MMIVC_MAX_LONG_SMS_NUM == i)//没有未组装的Vcard
            {
                SCI_FREE(send_type_ptr);
                send_type_ptr = PNULL;
                if(MMIVC_SMS_SEND_VCARD == send_type)
                {
                   s_long_vcardsms_ptr= PNULL ;
                }
#ifdef MMI_VCALENDAR_SUPPORT
                else if(MMIVC_SMS_SEND_VCALENDAR == send_type)
                {
                    s_long_vcalsms_ptr=PNULL ;
                }
#endif
                //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms free memory"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3178_112_2_18_2_53_8_244,(uint8*)"");
            }
            else
            {
                //SCI_TRACE_LOW:"SMS: MMISMS_RecvVcardOrVcalSms exist vcard sms"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3182_112_2_18_2_53_8_245,(uint8*)"");
            }
        }
    }
    else
    {   //不是长短信
        SCI_MEMCPY(vcard_data.data,
            sms_ptr->user_valid_data_t.user_valid_data_arr,
            sms_ptr->user_valid_data_t.length);
        vcard_data.length = sms_ptr->user_valid_data_t.length;
    }

    GetVcardFileNameOverSms(time_stamp_ptr, name_arr);

#if defined MMI_VCARD_SUPPORT
    if(MMIVC_SMS_SEND_VCARD == send_type)
    {
        MMISMS_SaveVcardSms(name_arr, vcard_data.data, vcard_data.length);
    }
    else
#endif
#ifdef MMI_VCALENDAR_SUPPORT
    if(MMIVC_SMS_SEND_VCALENDAR == send_type)
    {
        //SCI_TRACE_LOW:"SMS: MMISMS_SaveVcalendarSms send_type=%d, vcard_data.length=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3206_112_2_18_2_53_8_246,(uint8*)"dd",send_type,vcard_data.length);
        MMISMS_SaveVcalendarSms(name_arr, vcard_data.data, vcard_data.length);
    }
#endif
    //MMISMS_SetNewSMSRingFlag(TRUE); 
    MMIDEFAULT_TurnOnBackLight();
   // MMISMS_PlayNewSMSRing(dual_sys);
    //如果屏保被点亮中，则关闭屏保窗口
    if (MMK_IsOpenWin(MMIMAIN_SHOWTIME_WIN_ID))
    {
        MMK_CloseWin(MMIMAIN_SHOWTIME_WIN_ID);
    }    
    
RECV_RETURN:
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);
    return;
}

/*****************************************************************************/
// Description : 获得vCard文件名称(名称由SMS+接收时间字串组成)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void GetVcardFileNameOverSms(MN_SMS_TIME_STAMP_T  *sc_time_ptr, uint8 *name_ptr)
{
    if (PNULL == sc_time_ptr || PNULL == name_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetVcardFileNameOverSms sc_time_ptr name_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3232_112_2_18_2_53_8_247,(uint8*)"");
        return;
    }
    sprintf((char*)name_ptr, "SMS_%02d%02d%02d%02d%02d.vcf",
        sc_time_ptr->year, sc_time_ptr->month, sc_time_ptr->day,
        sc_time_ptr->hour, sc_time_ptr->minute);

    return;
}
#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : save received Vcard data over sms
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_SaveVcardSms(
                               uint8   *name_ptr,
                               uint8   *buf_ptr,
                               uint16   buf_len
                               )
{

    VCARD_ERROR_E  save_ret = VCARD_FILE_ERROR;
    wchar          *name_wstr_ptr = PNULL;
   
    if (PNULL == name_ptr || PNULL == buf_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SaveVcardSms name_ptr buf_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3255_112_2_18_2_53_8_248,(uint8*)"");
        return;
    }
    
    if (buf_len > 0)
    {
        name_wstr_ptr = SCI_ALLOCA(sizeof(wchar) *(MMIFILE_FILE_NAME_MAX_LEN +1));
        if(name_wstr_ptr != PNULL)
        {
            SCI_MEMSET(name_wstr_ptr, 0x00, sizeof(wchar) *(MMIFILE_FILE_NAME_MAX_LEN +1));
            MMI_STRNTOWSTR(name_wstr_ptr, MMIFILE_FILE_NAME_MAX_LEN, name_ptr, MMIFILE_FILE_NAME_MAX_LEN, strlen((char*)name_ptr));
            save_ret = MMIAPIVCard_SaveVCardByData(name_wstr_ptr, buf_ptr, buf_len);
        }

        if (VCARD_NO_ERROR == save_ret)
        {
            MMIAPISMS_ShowNewVcardOrVcalendarPrompt(TRUE,MMIVC_SMS_SEND_VCARD);
            MMK_SendMsg(MMISMS_NEW_VCARD_MSG_WIN_ID,
                MSG_SMS_VCARD_FILE_NAME,
                name_wstr_ptr);
            
        }
        else
        {
            //MMIPUB_OpenAlertWarningWin(TXT_MMS_TEXT_UNSUPORTED_MESSAGE);
            MMIAPIPB_OpenVcardContactWin(buf_ptr, buf_len);                
        }  
        if(name_wstr_ptr != PNULL)
        {
            SCI_FREE(name_wstr_ptr);
        }
    }

    return;
}
#endif
#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : save received Vcalendar data over sms
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_SaveVcalendarSms(
                               uint8   *name_ptr,
                               uint8   *buf_ptr,
                               uint16   buf_len
                               )
{

    if (PNULL == name_ptr || PNULL == buf_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SaveVcardSms name_ptr buf_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3280_112_2_18_2_53_8_249,(uint8*)"");
        return;
    }
    if (buf_len > 0)
    {
        MMIVCAL_SaveVCalendarByData(VIRTUAL_WIN_ID, MSG_VCALENDAR_WRITE_SMS_CNF, buf_ptr, buf_len);
    }

    return;
}
#endif

#if defined(MMI_VCARD_SUPPORT) || defined(MMI_VCALENDAR_SUPPORT)
/*****************************************************************************/
// Description : to show the new Vcard prompt
//               (参考函数:MMISMS_ShowNewMsgPrompt)
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewVcardOrVcalendarPrompt(BOOLEAN is_play_ring,MMIVC_SMS_SEND_TYPE_E type)
{
    MMI_STRING_T prompt_str = {0};
    uint32 time_out = 0;
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T alert_win_id = MMISMS_NEW_VCARD_MSG_WIN_ID;
    MMIPUB_HANDLE_FUNC  handle_func = PNULL;
    //SCI_TRACE_LOW:"MMISMS_ShowNewVcardOrVcalendarPrompt type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3305_112_2_18_2_53_8_250,(uint8*)"d",type);
#ifdef MMI_VCALENDAR_SUPPORT
    if(MMIVC_SMS_SEND_VCALENDAR == type)
    {
        alert_win_id =MMISMS_NEW_VCALENDAR_MSG_WIN_ID;
        MMI_GetLabelTextByLang(TXT_SMS_RECEIVE_VCALENDAR_MSG, &prompt_str);
        handle_func=(MMIPUB_HANDLE_FUNC)MMISMS_HandleNewVcalendarMsgWin;
    }
#ifdef MMI_VCARD_SUPPORT
    else// if(MMIVC_SMS_SEND_VCARD == type)
    {
        alert_win_id =MMISMS_NEW_VCARD_MSG_WIN_ID;
        MMI_GetLabelTextByLang(TXT_SMS_RECEIVE_VCARD_MSG, &prompt_str);
        handle_func=(MMIPUB_HANDLE_FUNC)MMISMS_HandleNewVcardMsgWin;
    }
#endif
#else
#ifdef MMI_VCARD_SUPPORT
    alert_win_id =MMISMS_NEW_VCARD_MSG_WIN_ID;
    MMI_GetLabelTextByLang(TXT_SMS_RECEIVE_VCARD_MSG, &prompt_str);
    handle_func=(MMIPUB_HANDLE_FUNC)MMISMS_HandleNewVcardMsgWin;
#endif
#endif   
    //SCI_TRACE_LOW:"SMS: MMISMS_ShowNewVcardOrVcalendarPrompt"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_RECEIVE_3324_112_2_18_2_53_8_251,(uint8*)"");
    MMISMS_SetNewSMSRingFlag(is_play_ring);
    if (MMK_IsFocusWin(alert_win_id))
    {
        // play the ring
        //has played ring when new msg received MMK_SendMsg(alert_win_id, MSG_SMS_PLAY_RING, PNULL); 
    }
    else
    {
        if (MMK_IsOpenWin(alert_win_id))
        {
            MMK_WinGrabFocus(alert_win_id);
        }
        else
        {
#ifdef SCREENSAVER_SUPPORT
            if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
            {
                MMIAPIIDLESS_CloseScreenSaver();
            }
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())        
            {
                win_priority = WIN_ONE_LEVEL;
                if(MMIVC_SMS_SEND_VCARD==type)
                {
#if defined(MMI_VCARD_SUPPORT)
                    MMIAPIKL_SetNewVcardStatus();//设置新Vcard状态
#endif
                }
#ifdef MMI_VCALENDAR_SUPPORT
                else if(MMIVC_SMS_SEND_VCALENDAR==type)
                {
                    MMIAPIKL_SetNewVcalendarStatus();
                }
#endif
            }
            else
#endif
            {
                win_priority = WIN_LOWEST_LEVEL;
            }
            MMIPUB_OpenAlertWinByTextPtr(&time_out,
                &prompt_str,
                PNULL,
                IMAGE_PUBWIN_NEWMSG,
                &alert_win_id,
                &win_priority,
                MMIPUB_SOFTKEY_CUSTOMER,
                handle_func);
        }
    }
}
#endif

/*****************************************************************************/
// Description : Get SM receive time stamp
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL uint32 GetSmsReceiveTimeStamp(void)
{
    return (TM_GetTotalSeconds() + (365*23+366*7)*24*3600);
    /*1970年1月1日零点到1999年12月31日23点59分59秒以来经过的秒数*/
}

#if defined MMI_STATUSBAR_SCROLLMSG_SUPPORT || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : get scroll msg record id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetScrollmsgRecordId(void)
{
    return s_sms_statusbar_scrollmsg.record_id;
}
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : to show the content of message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowMsgContent(
                                  MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                  uint16                    msg_len,            //IN:
                                  MMISMS_READ_MSG_T                     *read_msg_ptr,        //IN:
                                  MN_SMS_TIME_STAMP_T        *sc_time_ptr,        //IN:
                                  MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                  MMISMS_R8_LANGUAGE_E    language,
                                  BOOLEAN                    is_chat_mode,
                                  BOOLEAN                    is_need_time,
                                  MMI_STRING_T                *string_ptr            //OUT:
                                  )
{
    ShowMsgContent(
                   alphabet,
                   msg_len,
                   read_msg_ptr,
                   sc_time_ptr,            
                   orginal_addr_ptr,
                   language,
                   is_chat_mode,
                   is_need_time,
                   string_ptr
                  );    
}
#endif
#if defined MMI_STATUSBAR_SCROLLMSG_SUPPORT || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : MMISMS_GetmsgResult
//  Global resource dependence : 
//  Author:
//  Note:获取短信read msg
/*****************************************************************************/
PUBLIC MMISMS_READ_MSG_T MMISMS_GetmsgResult(void)
{
    return s_sms_statusbar_scrollmsg.read_msg;
}
#endif
