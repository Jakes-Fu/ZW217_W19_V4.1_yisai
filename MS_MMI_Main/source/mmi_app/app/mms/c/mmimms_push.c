/****************************************************************************
** File Name:      mmimms_push.c                                           *
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
#include "mmimms_push.h"
#include "mmimms_control_main.h"
#include "mmi_appmsg.h"
#include "mmimms_file.h"
#include "mMmsApi.h"
#include "mmimms_setting.h"
#include "mmiota_export.h"
#include "mmimms_net.h"
#include "mmimms_text.h"
#ifdef PUSH_EMAIL_SUPPORT//Added by sunlu  2010.5.7
#include "mail_export.h" 
#endif /*PUSH_EMAIL_SUPPORT */
#ifdef _ATEST_
#include "atest_ota.h"
#endif//_ATEST_
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mmi_modu_main.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
extern MMIMMS_LISTINFO_T s_mms_list;
uint8 *mms_sc_address_ptr = PNULL;
MN_DUAL_SYS_E     s_push_dual_sys = MN_DUAL_SYS_MAX;      //整理MMS库的相关接口函数后可以去掉此变量
#ifdef WIN32
const uint8  s_mms_push_info[] = {
                                    0x01,0x02,0x00,
                                    0x8c,0x82, //mms type
                                    0x98,0x31,0x32,0x33,0x34,0x00, //transaction id
                                    0x8D,0x90,  //version
                                    0x89,0x17,
                                    0x80,0x31,0x33,0x39,0x31,0x38,0x36,0x35,0x33,0x31,0x33,0x37,0x2f,0x54,0x59,0x50,
                                    0x45,0x3d,0x50,0x4c,0x4d,0x4e,0x00,
                                    //0x96,0x06,0xea,0x61,0x62,0x63,0x64,0x00, //subject
                                    0x96,0x08,0xea,0xe6,0xb5,0x8b,0xe8,0xaf,0x95,0x00, //subject
                                    0x8a,0x80, //message class
                                    0x8E,0x03,0x03,0x20,0x00, //message size
                                    0x88,0x05,0x81,0x03,0x01,0x51,0x80, //expire
                                    0x83,0x6d,0x6d,0x73,0x2e,0x73,0x6d,0x69,0x6c,0x00 //content location
                                    };

const uint8  s_mms_delivery_report[] = {
                                    0x01,0x02,0x00,
                                    0x8c,0x86, //mms type
                                    0x8D,0x90,  //version
                                    0x8B,0x31,0x32,0x00, //msg id
                                    0x97,0x31,0x32,0x33,0x2f,0x54,0x59,0x50,0x45,0x3d,0x50,0x4c,0x4d,0x4e,0x00,//to
                                    0x85, 0x04, 0x37,0xa0,0xa1,0x53,//data
                                    0x95,0x82// mms atatus
                                    }; 

   
    
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 提取指定的unitvar的值 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_PushSMSGetUnitVarValue(uint8* pdu_ptr, uint8* offset_ptr)
{ 
	uint32 	ret = 0;
	uint8 	unitvar[128]={0};
	uint8 	i = 0;
	uint8* 	content = PNULL;

	content = pdu_ptr;
	
	*offset_ptr = 0;
	
	while(content[*offset_ptr] > 0x80)
	{
		unitvar[*offset_ptr] = content[*offset_ptr];
		(*offset_ptr)++;
	}
	
	unitvar[*offset_ptr] = content[*offset_ptr];
	
	while(i <= (*offset_ptr))
	{
		if(unitvar[i] > 0x80)
		{
			ret += (uint32)((unitvar[i] & 0x7f) << (((*offset_ptr)-i)*7));
		}
		else
		{
			ret += (uint32)unitvar[i];
		}
		i++;
	}
	(*offset_ptr)++;

	return ret;
}

/*****************************************************************************/
//  Description : 查找PUSH消息对应的AP 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIPUSH_AP_E MMIMMS_PushDispatch(uint8 *pushpdu_ptr, uint32 pdu_len)
{
	MMIPUSH_AP_E 	ret = MMIPUSH_NOT_AP;
    uint8			headerlen_offset = 0;
    uint32			headerlen = 0;
    uint8			*application_id_ptr = PNULL;
	uint32			i = 0;

	if(PNULL != pushpdu_ptr && pdu_len >= 4 && pushpdu_ptr[1]==0x06)
	{
		ret = MMIPUSH_ANY_AP;	
		headerlen = MMIMMS_PushSMSGetUnitVarValue(&(pushpdu_ptr[2]), (uint32)(&headerlen_offset));
        for (i=3; i<headerlen+3; i++)
        {
            if (0xaf == pushpdu_ptr[i]) 
            {
                application_id_ptr = &pushpdu_ptr[i];
                SCI_TRACE_LOW("MMIMMS_PushDispatch: 0xaf == pushpdu_ptr[i]");
                break;
            }
        }
		
		if(PNULL != application_id_ptr && (application_id_ptr-(&(pushpdu_ptr[2])))<(int32)headerlen)
		{
			if(*(application_id_ptr+1) >= 0x80)
			{
				ret = (MMIPUSH_AP_E)(*(application_id_ptr+1)-0x80);
			}
			else
			{
				// 	字符串形式的Application ID，
				// 	只有Encoding Version小于1.2
				// 	才会出现，暂时不予考虑
			}
		}
	}
	return ret;
}

/*****************************************************************************/
//  Description : 回调函数，主要用于push消息处理 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC int MMIMMS_HandleMMSPushCallback(int status,void *prompt,int status_value)
{
    MMS_NOTIFICATION_IND_MSG_T *msg = NULL;
    static MMIMMS_MSG_RECEIVED_T   received_para = {0};
    int32                   record_id = 0;
    uint32                  current_second = MMIAPICOM_GetCurTime();
    MMISMS_MO_MT_TYPE_E     mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
//////////////////////////////////////////////////////////////////////////
// modified by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
    MMIMMS_SETTING_RETRIEVE_MODE_E retrieve_mode = MMIMMS_SETTING_RETRIEVE_MODE_DEFER;
#else
    MMIMMS_SETTING_RETRIEVE_E retrieve_mode = MMIMMS_SETTING_RETRIEVE_AUTO;
#endif
    MMIMMS_EDIT_DOCUMENT_T * reportdoc_ptr = PNULL;
    MMI_STRING_T            report_string = {0};
    wchar                  read_time[MMIMMS_MAX_SUBJECT_LEN] = {0};
    MMI_TM_T             tm = {0};
    MMI_STRING_T           to_str = {0};
    MMI_STRING_T           message_id_str = {0};
    MMI_STRING_T           message_wstr = {0};
    MMI_STRING_T           read_time_str = {0};
    char str[MMIMMS_MAX_SUBJECT_LEN] = {0};
    const wchar     r_wstr[] = {'\r', 0 };
    uint16          r_wstr_len = MMIAPICOM_Wstrlen(r_wstr);
    uint16          time_zone = 8;
    MMISMS_ORDER_INFO_T* list_item_ptr = PNULL;
    MMIMMS_PUSH_FILE_T *push_file_ptr = PNULL;  
    MMIMMS_ERROR_E save_error = MMIMMS_NO_ERROR;
    
    //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_297_112_2_18_2_38_54_368,(uint8*)"d", status);
    switch(status)
    {
    case MMS_PUSH_TYPE_DELIVERY_IND:
        //可在此处向用户弹出对话框，"用户已收到你的彩信"
        //参数prompt就是告诉用户接收成功的字符串,
        msg = (MMS_NOTIFICATION_IND_MSG_T *)prompt;
        //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback THE MSG->de_status is%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_304_112_2_18_2_38_54_369,(uint8*)"d",msg->de_status);
        SCI_MEMSET((char *)&received_para, 0 , sizeof(MMIMMS_MSG_RECEIVED_T));
        received_para.type = MMS_PUSH_TYPE_DELIVERY_IND;
        received_para.push_status = (MMIMMS_PUSH_RECV_E)msg->de_status;        
        if (msg->de_to)
        {
            uint16 temp_len = strlen(msg->de_to);
            temp_len = MIN(MMIMMS_MAX_ADDRESS_LEN, temp_len);
            strncpy((char *)received_para.receiver, msg->de_to, temp_len);
        }
        else
        {
            received_para.receiver[0] = 0;
        }
        MMIMMS_ControlToUIMsg(MSG_MMS_RECEIVED,&received_para);
        break;
    case MMS_PUSH_TYPE_READORIG_IND://阅读报告，彩信一致性测试的阅读报告
        {
            //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback MMS_PUSH_TYPE_READORIG_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_322_112_2_18_2_38_54_370,(uint8*)"");
            msg = (MMS_NOTIFICATION_IND_MSG_T *)prompt;
            reportdoc_ptr = MMIMMS_CreateEditDocument();
            if (PNULL == reportdoc_ptr)
            {
                break;
            }
            MMS_CreateNewSlide(reportdoc_ptr->editbody_ptr);

            //发件人
            if (msg->de_to && msg->de_to[0])
            {
                reportdoc_ptr->edit_to.wstr_len = SCI_STRLEN(msg->de_to);
                reportdoc_ptr->edit_to.wstr_ptr = MMIMMS_ALLOC((reportdoc_ptr->edit_to.wstr_len + 1) * sizeof(wchar));
                if (PNULL != reportdoc_ptr->edit_to.wstr_ptr)
                {
                    SCI_MEMSET(reportdoc_ptr->edit_to.wstr_ptr,0,((reportdoc_ptr->edit_to.wstr_len + 1) * sizeof(wchar)));
                    MMI_STRNTOWSTR(reportdoc_ptr->edit_to.wstr_ptr,reportdoc_ptr->edit_to.wstr_len,(uint8 *)msg->de_to,reportdoc_ptr->edit_to.wstr_len,reportdoc_ptr->edit_to.wstr_len);
                }                
            }
            //主题 主题不要放进阅读报告里面
//            if (msg->subject && msg->subject[0])
//            {
//                len = *(uint16*)msg->subject;
//                len = MIN(len/2,MMIMMS_MAX_SUBJECT_LEN);
//                reportdoc_ptr->edit_subject.wstr_ptr = MMIMMS_ALLOC(MMIMMS_MAX_SUBJECT_LEN * sizeof(wchar));
//                SCI_MEMSET(reportdoc_ptr->edit_subject.wstr_ptr, 0, (MMIMMS_MAX_SUBJECT_LEN * sizeof(wchar)));
//                reportdoc_ptr->edit_subject.wstr_len = len;
//                SCI_MEMCPY(reportdoc_ptr->edit_subject.wstr_ptr,msg->subject+2,len*2);
//            } 

            MMIMMS_SetDefaultHeadValue(reportdoc_ptr);
            reportdoc_ptr->message_class = MMS_MESSAGE_CLASS_TYPE_PERSONAL;
            reportdoc_ptr->delivery_report = MMS_DELIVERY_REPORT_NO;
            reportdoc_ptr->read_report = MMS_READ_REPORT_NO;
            //阅读报告内容
            tm = MMIAPICOM_Second2Tm(msg->de_date + time_zone * MMIMMS_1_HOUR_SECONDS - MMIMMS_SECOND_1970_TO_1980);
            _snprintf((char*)str, (MMIMMS_MAX_SUBJECT_LEN-1),
                "%04d%s%02d%s%02d%s%02d:%02d:%02d",
                tm.tm_year,  "-",  tm.tm_mon,  "-",  tm.tm_mday,  "  ", tm.tm_hour, tm.tm_min, tm.tm_sec);
            MMIAPICOM_StrToWstr((uint8 *)str,read_time);
            
            MMI_GetLabelTextByLang(TXT_COMMON_TO,&to_str);
            //MMI_GetLabelTextByLang(TXT_INPUT_SUBJECT,&subject_str);
            MMI_GetLabelTextByLang(TXT_MMS_DETAIL_MESSAGEID,&message_id_str);
            MMI_GetLabelTextByLang(TXT_MMS_READREPORT_TIME,&read_time_str);

            report_string.wstr_len += to_str.wstr_len + r_wstr_len + reportdoc_ptr->edit_to.wstr_len + r_wstr_len;
            //report_string.wstr_len += subject_str.wstr_len + r_wstr_len + reportdoc_ptr->edit_subject.wstr_len + r_wstr_len;
            report_string.wstr_len += message_id_str.wstr_len + r_wstr_len + SCI_STRLEN(msg->de_msgid) + r_wstr_len;
            report_string.wstr_len += read_time_str.wstr_len + r_wstr_len;
            report_string.wstr_len += MMIAPICOM_Wstrlen(read_time);

            report_string.wstr_ptr = (wchar*)MMIMMS_ALLOC((report_string.wstr_len+1) * sizeof(wchar));            
            if (PNULL == report_string.wstr_ptr)
            {
                break;
            }
            SCI_MEMSET(report_string.wstr_ptr, 0, ((report_string.wstr_len+1) * sizeof(wchar)));
            MMIAPICOM_Wstrncpy(report_string.wstr_ptr,to_str.wstr_ptr,to_str.wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,reportdoc_ptr->edit_to.wstr_ptr,reportdoc_ptr->edit_to.wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
            
//            if(reportdoc_ptr->edit_subject.wstr_ptr)
//            {
//                MMIAPICOM_Wstrncat(report_string.wstr_ptr,subject_str.wstr_ptr,subject_str.wstr_len);
//                MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
//                MMIAPICOM_Wstrncat(report_string.wstr_ptr,reportdoc_ptr->edit_subject.wstr_ptr,reportdoc_ptr->edit_subject.wstr_len);
//                MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
//            }            
            
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,message_id_str.wstr_ptr,message_id_str.wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
            message_wstr.wstr_ptr = (wchar *)MMIMMS_ALLOC((SCI_STRLEN(msg->de_msgid) + 1)*sizeof(wchar));
            if (PNULL == message_wstr.wstr_ptr)
            {
                MMIMMS_FREE(report_string.wstr_ptr);
                break;
            }
            message_wstr.wstr_len = SCI_STRLEN(msg->de_msgid);
            SCI_MEMSET(message_wstr.wstr_ptr, 0, ((message_wstr.wstr_len + 1)*sizeof(wchar)));
            MMI_STRNTOWSTR(message_wstr.wstr_ptr, message_wstr.wstr_len, (uint8 *)msg->de_msgid, message_wstr.wstr_len, message_wstr.wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,message_wstr.wstr_ptr,message_wstr.wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);

            MMIAPICOM_Wstrncat(report_string.wstr_ptr,read_time_str.wstr_ptr,read_time_str.wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,r_wstr,r_wstr_len);
            MMIAPICOM_Wstrncat(report_string.wstr_ptr,read_time,MMIAPICOM_Wstrlen(read_time));
            
            report_string.wstr_len = MMIAPICOM_Wstrlen(report_string.wstr_ptr);
            MMIMMS_AddText(reportdoc_ptr,&report_string);
            MMIMMS_FREE(message_wstr.wstr_ptr);
            MMIMMS_FREE(report_string.wstr_ptr);

            //保存
            save_error = MMIMMS_SaveMMS(s_push_dual_sys, MMISMS_MT_TO_BE_READ, reportdoc_ptr);
            MMIMMS_DestroyEditDocument(reportdoc_ptr);
            received_para.type = MMS_PUSH_TYPE_READORIG_IND;
            received_para.push_status = MMIMMS_PUSH_NEW;
            if (MMIMMS_NO_ERROR == save_error)
            {
                MMIMMS_ControlToUIMsg(MSG_MMS_RECEIVED,&received_para);
            }            
        }
        break;
    case MMS_PUSH_TYPE_NOTIFICATION_IND:
        {
            
            //此时参数prompt为MMS url，status_value为url的长度。
            //可在此处向用户弹出对话框，"某用户向您发了一封彩信，是否现在接收!"
            //将这个值取得，赋到状态机中，就可以使用MMS_RecvMms去接收MMS了。或者将此URL存在文件中
            //且放入到未接收邮箱中去。
            msg = (MMS_NOTIFICATION_IND_MSG_T *)prompt;
            if(MMIMMS_Setting_GetAdvertising() || (msg && 0x81 != msg->message_class_value)) //MMS广告判断 
            {
                msg = (MMS_NOTIFICATION_IND_MSG_T *)prompt;
                received_para.type = MMS_PUSH_TYPE_NOTIFICATION_IND;
                if(MMIMMS_IsMaxNum())
                {
                    received_para.push_status = MMIMMS_PUSH_NEW_FULLNUM;
                    MMIMMS_ResponceMMS(s_push_dual_sys,msg,MMS_STATUS_VALUE_DEFERRED);
                }
                else if(!MMIMMS_IsEnoughSpace(MMIMMS_RESERVE_HEAD_SIZE))//save mms push space
                {
                    received_para.push_status = MMIMMS_PUSH_NEW_FULLSPACE;
                    MMIMMS_ResponceMMS(s_push_dual_sys,msg,MMS_STATUS_VALUE_DEFERRED);
                }
                else
                {
                    //////////////////////////////////////////////////////////////////////////
                    // modified by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
                    retrieve_mode = MMIMMS_Setting_GetCurRetrieveMode(s_push_dual_sys);
                    if (MMIMMS_SETTING_RETRIEVE_MODE_DENY == retrieve_mode)
                    {
                        MMIMMS_ResponceMMS(s_push_dual_sys, msg, MMS_STATUS_VALUE_REJECTED);
                    }
                    else
#endif
                    {
                        //保存PUSH与LIST
                        if(s_push_dual_sys >= MN_DUAL_SYS_MAX)
                        {
                            //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback error s_push_dual_sys >= MN_DUAL_SYS_MAX"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_471_112_2_18_2_38_54_371,(uint8*)"");
                            //s_push_dual_sys值异常，程序出现问题，仍掉异常PUSH。
                            break;
                        }

                        list_item_ptr = s_mms_list.list_head_ptr;
                        push_file_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
                        if (PNULL == push_file_ptr)
                        {
                            //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback read push error alloc failed!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_480_112_2_18_2_38_54_372,(uint8*)"");
                            break;
                        }
                        while (PNULL != list_item_ptr)
                        {
                            if(    MMISMS_MT_NOT_DOWNLOAD == list_item_ptr->mo_mt_type
                                || MMISMS_MT_NOT_NOTIFY == list_item_ptr->mo_mt_type
                                || MMISMS_MT_NEED_NOT_DOWNLOAD == list_item_ptr->mo_mt_type)
                            {                            
                                SCI_MEMSET((uint8 *)push_file_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
                                if (MMIMMS_ReadPush(push_file_ptr, list_item_ptr->record_id))
                                {
                                    if (PNULL != msg->mms_url && !strcmp(msg->mms_url, (char*)push_file_ptr->url))
                                    {
                                        SCI_FREE(push_file_ptr);
                                        push_file_ptr = PNULL;
                                        return 1;
                                    }
                                }
                                else
                                {
                                    //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback read push error!"
                                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_501_112_2_18_2_38_54_373,(uint8*)"");
                                }                         
                            }
                         
                            list_item_ptr = list_item_ptr->next;
                        }
                        SCI_FREE(push_file_ptr);
                        push_file_ptr = PNULL;
                    //////////////////////////////////////////////////////////////////////////
                    // added by feng.xiao
                    #ifdef DATA_ROAMING_SUPPORT
                        if (MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE == retrieve_mode)
                        {
                            mo_mt_type = MMISMS_MT_NOT_NOTIFY;
                        }
                        else if (MMIMMS_SETTING_RETRIEVE_MODE_DEFER == retrieve_mode)
                        {
                            mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
                        }
                    #else
                    retrieve_mode = MMIMMS_Setting_GetRetrieveMode();
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD
                    if(MMIPHONE_ROAMING_STATUS_NO_ROMING != MMIAPIPHONE_GetRoamingStatus(s_push_dual_sys))
                    {
                        retrieve_mode = MMIMMS_Setting_GetRoamingRetrieveMode();
                    }
#endif
#ifdef MET_MEX_SUPPORT
                        if (MMIMEX_IsSysRunning())  // 如果MEX在运行可能网络冲突,禁止自动接受彩信
                        {
                            MMIMEX_SmsReceivePromt();
                            retrieve_mode = MMIMMS_SETTING_RETRIEVE_MANUAL;
                        }
                        else if (MMIMEX_ConflictPromt())
                        {
                            retrieve_mode = MMIMMS_SETTING_RETRIEVE_MANUAL;
                        }
#endif
                        if(MMIMMS_SETTING_RETRIEVE_AUTO == retrieve_mode)
                        {
                            mo_mt_type = MMISMS_MT_NOT_NOTIFY;
                        }
                        else
                        {
                            mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
                        }
                    #endif
                        if (mms_sc_address_ptr != PNULL)
                        {
                            record_id = MMIMMS_SavePush(s_push_dual_sys,mo_mt_type, msg, mms_sc_address_ptr);
                            SCI_FREE(mms_sc_address_ptr);
                            mms_sc_address_ptr = PNULL;
                        }
                        else
                        {
                            record_id = MMIMMS_SavePush(s_push_dual_sys,mo_mt_type, msg, PNULL);
                        }

                        if (0 > record_id)
                        {
                            //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback push is in black lists"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_555_112_2_18_2_38_54_374,(uint8*)"");
                            MMIMMS_ResponceMMS(s_push_dual_sys,msg,MMS_STATUS_VALUE_DEFERRED);
                            break;
                        }
                        else if(0 == record_id)
                        {
                            //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback push save fail"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_561_112_2_18_2_38_54_375,(uint8*)"");
                            //保存出错PUSH内容丢失,比如文件夹或文件到达上限无法创建
                            received_para.push_status = MMIMMS_PUSH_SFS_ERROR;
                            MMIMMS_ResponceMMS(s_push_dual_sys,msg,MMS_STATUS_VALUE_DEFERRED);
                        }
                        else if(MMIMMS_IsExpire(current_second,(uint32)msg->expiry_time))   //to be 是否能以服务器的时间判断
                        {
                            received_para.push_status = MMIMMS_PUSH_EXPIRED;
                            MMIMMS_ResponceMMS(s_push_dual_sys,msg,MMS_STATUS_VALUE_EXPIRED);
                        }
                        else
                        {  
                            received_para.record_id = record_id;
                        //////////////////////////////////////////////////////////////////////////
                        // modified by feng.xiao
                        #ifdef DATA_RAOMING_SUPPORT
                            if (MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE == retrieve_mode)
                        #else
                            if(MMIMMS_SETTING_RETRIEVE_AUTO == retrieve_mode)
                        #endif
                            {
                                received_para.push_status = MMIMMS_PUSH_NEW_NOT_NOTIFY;
                                //加入自动下载链表
                                MMIMMS_AddAutoList(s_push_dual_sys,record_id, MMIMMS_SENDRECV_RECVING);
                                //触发自动下载
                                MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
                            }
                            else
                            {
                                //SCI_TRACE_LOW:"[MMIMMS]:HandleMMSPushCallback retrieve manual mode"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PUSH_590_112_2_18_2_38_54_376,(uint8*)"");
                                MMIMMS_ResponceMMS(s_push_dual_sys,msg,MMS_STATUS_VALUE_DEFERRED);
                                received_para.push_status = MMIMMS_PUSH_NEW;
                        	}
                        }
                    }
                }
                
                MMIMMS_ControlToUIMsg(MSG_MMS_RECEIVED,&received_para);
            }
        }
        break;
        
    case MMS_PUSH_TYPE_UNKNOW:
        //可以扔了。
        break;
    default :
        break;
    }
    return 1;
}

/*****************************************************************************/
//  Description : 判断是否过期
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsExpire(
                              uint32   date_second,
                              uint32   expiry_second
                              )
{
    BOOLEAN return_val = FALSE;
    uint32 current_second = 0;
    uint32 expiry_time = 0;

    current_second = MMIAPICOM_GetCurTime();
    expiry_time = date_second + expiry_second;
    
    if(expiry_second > 0 
        &&current_second > expiry_time)
    {
        return_val = TRUE;
    }

    return return_val;
}

PUBLIC void MMS_TestPushRcv(void)
{
#ifdef WIN32
    s_push_dual_sys = MN_DUAL_SYS_1;
    
    MMS_ParsePushMsg(
                    (uint8 *)s_mms_push_info,
                    sizeof(s_mms_push_info),
                    0, 
                    MMIMMS_HandleMMSPushCallback
                    );/*lint !e64*/
#endif
}

PUBLIC void MMS_TestDeliveryReport(void)
{ 
#ifdef WIN32
    MMS_ParsePushMsg(
                    (uint8 *)s_mms_delivery_report,
                    sizeof(s_mms_delivery_report),
                    0, 
                    MMIMMS_HandleMMSPushCallback
                    );/*lint !e64*/
#endif
}
