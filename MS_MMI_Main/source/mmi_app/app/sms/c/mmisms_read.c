/****************************************************************************
** File Name:      mmisms_read.c                                            *
** Author:                                                                  *
** Date:           8/23/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe reading SMS.               *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2007        liming.deng      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_read.h"
#include "mmisms_order.h"
#include "mmisms_export.h"
#include "mmi_signal_ext.h"
#include "mmisms_app.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "mmipub.h"
#include "mmipb_export.h"
#include "mmimms_export.h"
#include "mmisms_send.h"
#include "mmiidle_export.h"
#include "mmisms_receive.h"
#include "mmisms_edit.h"
#include "mmk_timer.h"
#include "mmipb_text.h"
#include "mmistk_export.h"
#ifdef MMI_OTA_SUPPORT
#include "mmiota_export.h"
#endif
#include "mmidm_export.h"
#include "mmisms_set.h"
#include "mmisms_delete.h"
#include "gui_ucs2b_converter.h"
#include "mmiudisk_export.h"
#include "mmisms_save.h"
#include "mmimms_text.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmisms_timermsg.h"
#endif
#ifdef PUSH_EMAIL_SUPPORT  //Added by yuanl  2010.8.7
#include "mmimail_export.h"
#endif /* PUSH_EMAIL_SUPPORT */

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mail_fldr.h"
#include "mail_export.h"
#endif /* PUSH_EMAIL_SUPPORT */

#ifdef MMIWIDGET_SUPPORT
#include "mmiwidget.h"
#include"mmiwidget_id.h"
#endif
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
#include "guistatusbar_scrollmsg.h"
#endif
#include "mmiidle_statusbar.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"  
#include "mmiota_id.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mmisms_commonui.h"
#include "mmisms_filecontrol.h"
#include "mmisms_appcontrol.h"
#include "mmismsapp_interface.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define MMISMS_PROMPT_STR_LEN           52
#define MMISMS_SORT_TIME                1000

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8 s_mmisms_sort_timer_id = 0;
#ifdef SEARCH_SUPPORT
LOCAL MMISMS_ORDER_ID_T s_cur_match_order_id;
LOCAL BOOLEAN s_is_stop_async_search_sms;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T   g_mmisms_global;  // SMS��ȫ�ֱ���
#ifdef IKEYBACKUP_SUPPORT   
extern MMISMS_BACKUP_DATA_T *g_sms_backup_data_ptr;//����ȫ�ֱ���
extern MMIIKB_BACKUP_ITEM_DATA_REQ_T *g_backup_item_data_req_ptr;
extern BOOLEAN g_sms_is_backup;
#endif

#ifdef WIN32
extern ERR_MNSMS_CODE_E MNSMS_WriteMeSmsSupportedEx(
                                             MN_DUAL_SYS_E  dual_sys,       //IN:
                                             BOOLEAN        is_me_support   //IN: set if me is support
                                             );
#endif

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : Start Check if MMS/OTA/WAP is ready timer
//  Global resource dependence :
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
LOCAL void StartSmsSortTimer(void);
#endif /* Reduce Code size */

#if 0
// no use
/*****************************************************************************/
//  Description : stop Check if MMS/OTA/WAP is ready  timer
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
LOCAL void StopSmsSortTimer(void);
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Description : GetBtSMSBoxTypeByReadSMSCnf
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_BOX_TYPE_E GetBtSMSBoxTypeByReadSMSCnf(APP_MN_READ_SMS_CNF_T *sig_ptr);
#endif

/*****************************************************************************/
//     Description : to handle the read SMS err
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void ReadSmsErr( 
                      MMISMS_ORDER_ID_T order_id
                      );

/*****************************************************************************/
//     Description : to assemble the status report by the formate
//    Global resource dependence : g_mmisms_global.read_msg
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void AssembleStatusReportToGlobal(
                                        MN_SMS_STATUS_REPORT_T    *status_report_ptr
                                        );

/*****************************************************************************/
//     Description : get ref num from head
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL uint8 GetSeqNumFromUserHead(
                                  MN_SMS_USER_DATA_HEAD_T    *user_data_head_ptr    //IN:
                                  );
                                  
/*****************************************************************************/
//     Description : set string to read content
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendString2ReadContent(
                                       uint16    str_len,    //IN:
                                       uint8    *str_ptr,    //IN:
                                       MN_SMS_ALPHABET_TYPE_E msg_dcs,//IN:
                                       MMISMS_READ_MSG_T        *read_msg_ptr
                                       );

/*****************************************************************************/
//     Description : to check whether the status  is to be read
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN  MtIsNeedUpdateReadStatus(
                                        MMISMS_ORDER_ID_T order_id
                                        );

/*****************************************************************************/
//     Description : to check whether there have content
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN    IsExistContent( void );


/*****************************************************************************/
//     Description : handle MT SMS order
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleMtSMSOrder(
                            MN_DUAL_SYS_E       dual_sys,       //IN:
                            MN_SMS_STORAGE_E    storage,        //IN:
                            MN_SMS_STATUS_E     status,            //IN:
                            MN_SMS_RECORD_ID_T  record_id,        //IN:
                            MN_SMS_MT_SMS_T        *mt_sms_ptr        //IN:
                            );

/*****************************************************************************/
//     Description : handle MT SMS status report order
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleSROrder(
                         MN_DUAL_SYS_E       dual_sys,       //IN:
                         MN_SMS_STORAGE_E    storage,        //IN:
                         MN_SMS_STATUS_E     status,            //IN:
                         MN_SMS_RECORD_ID_T  record_id,        //IN:
                         MN_SMS_STATUS_REPORT_T     *mt_sr_ptr        //IN:
                         );

/*****************************************************************************/
//     Description : handle MO SMS order
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleMoSMSOrder(
                            MN_DUAL_SYS_E       dual_sys,       //IN:
                            MN_SMS_STORAGE_E    storage,        //IN:
                            MN_SMS_STATUS_E     status,            //IN:
                            MN_SMS_RECORD_ID_T  record_id,        //IN:
                            MN_SMS_MO_SMS_T        *mo_sms_ptr        //IN:
                            );

/*****************************************************************************/
//     Description : to read next record SMS in storage
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSmsForOrder(    //RETURN: TRUE, the SMS stored in the storage is ready
                              MN_DUAL_SYS_E         dual_sys,
                              MN_SMS_STORAGE_E        storage,    //IN:
                              MN_SMS_RECORD_ID_T    record_id    //IN: the start id
                              );

/*****************************************************************************/
//     Description : to read sms for order.
//    Global resource dependence : g_mmisms_order
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MultiSimReadSmsForOrder(APP_MN_READ_SMS_CNF_T* sig_ptr );

/*****************************************************************************/
//     Description : append string to contact information string
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
LOCAL void AppendString2Contact(
                                MMI_STRING_T    *conv_str_ptr,    //IN/OUT:
                                BOOLEAN            is_ucs2,        //IN:
                                uint16            length,            //IN:
                                uint8            *org_str_ptr,    //IN:
                                uint16            max_len            //IN:
                                );

/*****************************************************************************/
//     Description : GetSMSBoxTypeByReadSMSCnf
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_BOX_TYPE_E GetSMSBoxTypeByReadSMSCnf(APP_MN_READ_SMS_CNF_T *sig_ptr);

#ifdef MMI_MSG_RESEND
/*****************************************************************************/
//  Description : 
//  Global resource dependence : g_mmisms_global
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void Handle_ReadResendSms(DPARAM param);
#endif

#ifdef SEARCH_SUPPORT 
/*****************************************************************************/
//  Description : 
//  Global resource dependence : g_mmisms_global
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSmsSearchMatchItem(MMISMS_ORDER_ID_T sms_state_ptr);

/*****************************************************************************/
//     Description : search string in sms 
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchStrInReadMsg(
							 MMISMS_READ_MSG_T read_msg
							 );

							 
/*****************************************************************************/
//     Description : search string in sms 
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
 /*****************************************************************************/
LOCAL BOOLEAN SearchStrInMMSMsg(
                                       MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id

                                     );

/*****************************************************************************/
//     Description : init search sms flag
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
LOCAL void InitSearchFlag(
        					 MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id
        					 );

/*****************************************************************************/
//     Description : search string in push 
//    Global resource dependence : 
//  Author: minghu.mao
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchStrInPushMsg(MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id);
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
//     Description : handle search sms
//    Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSearchSms(MMI_STRING_T* search_string_ptr,BOOLEAN is_mms_only)
{
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id = PNULL;

    if(PNULL == search_string_ptr || PNULL == search_string_ptr->wstr_ptr)
    {
        return ;
    }
    
    cur_unit_id = MMISMS_GetUsedOrderId();
    InitSearchFlag(cur_unit_id);

    if(PNULL != cur_unit_id)
    {
        SCI_MEMSET(g_mmisms_global.search_msg.search_str, 0, sizeof(g_mmisms_global.search_msg.search_str));
    
        MMI_WSTRNCPY(g_mmisms_global.search_msg.search_str,
                                MMISMS_SEARCH_MAX_LEN,
                                search_string_ptr->wstr_ptr,
                                search_string_ptr->wstr_len,
                                MIN(search_string_ptr->wstr_len, MMISMS_SEARCH_MAX_LEN));

        g_mmisms_global.search_msg.search_len = search_string_ptr->wstr_len;

        MMISMS_SetOperStatus(MMISMS_ASYNC_SEARCH_SMS);
        if (is_mms_only)
        {
            MMISMS_SearchMMS(cur_unit_id);
        }
        else
        {
            MMISMS_SearchMain(cur_unit_id);
        }
    }
    else
    {
        MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_SMS);
    }
}

/*****************************************************************************/
//     Description : stop async search sms
//    Global resource dependence : 
//  Author: tao.xue
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_StopSearchSms(void)
{
    s_is_stop_async_search_sms  = TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_HandleSearchNextSms(void)
{
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_ptr = PNULL;
    
    if (s_is_stop_async_search_sms )
    {
        MMISMS_ClearOperInfo();
        MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_SMS);
        s_is_stop_async_search_sms  = FALSE;
    }
    else
    {
        MMISMS_SetOperStatus(MMISMS_ASYNC_SEARCH_SMS);
        cur_unit_ptr = g_mmisms_global.search_msg.search_ptr;
#if defined(WIN32)
        SCI_Sleep(10);
#endif
        MNSMS_ReadSmsEx(
                (MN_DUAL_SYS_E)cur_unit_ptr->sms_state.flag.dual_sys, 
                (MN_SMS_STORAGE_E)cur_unit_ptr->sms_state.flag.storage, 
                cur_unit_ptr->sms_state.record_id 
                );
    }
}

/*****************************************************************************/
//     Description : read search sms
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SearchMain(
								 MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id
								 )
{
    //Search first sms
    while ((cur_unit_id != PNULL))
    {
        if(MMISMS_FOLDER_SECURITY == cur_unit_id->sms_state.flag.folder_type)
        {
            cur_unit_id = cur_unit_id->next_ptr;
        }
        else if((MMISMS_TYPE_SMS == cur_unit_id->sms_state.flag.msg_type) &&
            ((cur_unit_id->sms_state.longsms_max_num == 0) ||
            (cur_unit_id->sms_state.longsms_max_num != 0) &&
            (cur_unit_id->sms_state.longsms_seq_num == 1)))
        {
            if (MMISMS_SENDSTATUS_SENDING == cur_unit_id->sms_state.backsend_status)
            {
                SCI_TRACE_LOW("MMISMS sending sms not included");
                cur_unit_id = cur_unit_id->next_ptr;
            }
            else
            {
                break;
            }
        }
        else if(MMISMS_TYPE_MMS == cur_unit_id->sms_state.flag.msg_type)
        {//���� ���ҽ�����һ��
            g_mmisms_global.search_msg.search_ptr = cur_unit_id;
            g_mmisms_global.search_msg.next_long_order_id = g_mmisms_global.search_msg.search_ptr->sms_state.next_long_sms_ptr;
            //����������ӽ�������
            if(!MMIAPIUDISK_UdiskIsRun() && SearchStrInMMSMsg(cur_unit_id))
            {//��������
                if(!SaveSmsSearchMatchItem(&g_mmisms_global.search_msg.search_ptr->sms_state))
                {
                    cur_unit_id = PNULL;
                    break;
                }
                g_mmisms_global.search_msg.search_ptr->sms_state.flag.is_search = TRUE;
            }

            cur_unit_id = cur_unit_id->next_ptr;

        }  
        else if (MMISMS_TYPE_WAP_PUSH == cur_unit_id->sms_state.flag.msg_type
            || MMISMS_TYPE_WAP_OTA == cur_unit_id->sms_state.flag.msg_type)
        {
            g_mmisms_global.search_msg.search_ptr = cur_unit_id;
            g_mmisms_global.search_msg.next_long_order_id = g_mmisms_global.search_msg.search_ptr->sms_state.next_long_sms_ptr;
            //����������ӽ�������
            if(!MMIAPIUDISK_UdiskIsRun() && SearchStrInPushMsg(cur_unit_id))
            {//��������
                if(!SaveSmsSearchMatchItem(&g_mmisms_global.search_msg.search_ptr->sms_state))
                {
                    cur_unit_id = PNULL;
                    break;
                }
                
                g_mmisms_global.search_msg.search_ptr->sms_state.flag.is_search = TRUE;
            }

            cur_unit_id = cur_unit_id->next_ptr;
        }
        else
        {
            cur_unit_id = cur_unit_id->next_ptr;
        }
    }

    if(PNULL != cur_unit_id)
    {
        g_mmisms_global.search_msg.search_ptr = cur_unit_id;
        g_mmisms_global.search_msg.next_long_order_id = g_mmisms_global.search_msg.search_ptr->sms_state.next_long_sms_ptr;
#if 0
        MNSMS_ReadSmsEx(
                (MN_DUAL_SYS_E)cur_unit_id->sms_state.flag.dual_sys, 
                (MN_SMS_STORAGE_E)cur_unit_id->sms_state.flag.storage, 
                cur_unit_id->sms_state.record_id 
                );
#endif
        {
            uint16               sig_size = 0;  
            APP_MN_SEARCH_NEXT_SMS_REQ_T    *sig_ptr = PNULL;
    
            sig_size = sizeof(APP_MN_SEARCH_NEXT_SMS_REQ_T);
            SCI_CREATE_SIGNAL(sig_ptr,APP_MN_SEARCH_NEXT_SMS_CNF,sig_size,P_APP);   /*lint !e64*/
            SCI_SEND_SIGNAL(sig_ptr,P_APP);     
        }
    }
    else
    {
        MMISMS_ClearOperInfo();
        MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_SMS);
    }

    return;
}

/*****************************************************************************/
//     Description : read search sms
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SearchMMS( MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id)
{
    //Search first sms
    while ((cur_unit_id != PNULL))
    {
        if(MMISMS_TYPE_MMS == cur_unit_id->sms_state.flag.msg_type)
        {//���� ���ҽ�����һ��
            g_mmisms_global.search_msg.search_ptr = cur_unit_id;
            g_mmisms_global.search_msg.next_long_order_id = g_mmisms_global.search_msg.search_ptr->sms_state.next_long_sms_ptr;
            //����������ӽ�������
            if(!MMIAPIUDISK_UdiskIsRun() && SearchStrInMMSMsg(cur_unit_id))
            {//��������
                if(!SaveSmsSearchMatchItem(&g_mmisms_global.search_msg.search_ptr->sms_state))
                {
                    cur_unit_id = PNULL;
                    break;
                }
                g_mmisms_global.search_msg.search_ptr->sms_state.flag.is_search = TRUE;
            }

            cur_unit_id = cur_unit_id->next_ptr;

        }  
        else
        {
            cur_unit_id = cur_unit_id->next_ptr;
        }
    }

    MMISMS_ClearOperInfo();
    MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_SMS);
    return;
}

/*****************************************************************************/
//     Description : search string in sms 
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchStrInReadMsg(
							 MMISMS_READ_MSG_T read_msg
							 )
{
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint16 num_len = 0;
    wchar pb_name[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MMI_STRING_T num_name = {0};   
    BOOLEAN find_result = FALSE;

    if (0 != read_msg.address.num_len)
    {
        num_len = MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)(read_msg.address.number_type),
                                                                        (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                        read_msg.address.num_len),
                                                                        read_msg.address.party_num,
                                                                        tele_num,
                                                                        (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                                                        );

        //search number
        SCI_MEMSET(pb_name, 0x00, sizeof(pb_name));
        num_name.wstr_ptr = pb_name;
        num_name.wstr_len = MIN(num_len, MMISMS_PBNAME_MAX_LEN);

        MMI_STRNTOWSTR(num_name.wstr_ptr,
                                    MMISMS_PBNAME_MAX_LEN,
                                    (uint8 *)tele_num,
                                    num_len,
                                    num_name.wstr_len);
#if 0
        if (MMIAPICOM_Wstrnstr(num_name.wstr_ptr,
                                            num_name.wstr_len,
                                            g_mmisms_global.search_msg.search_str,
                                            g_mmisms_global.search_msg.search_len
                                            ))
        {
            return TRUE;
        }
#endif
        SCI_MEMSET(pb_name, 0x00, sizeof(pb_name));
        num_name.wstr_len = 0;
        num_name.wstr_ptr = pb_name;
        
        find_result = MMISMS_GetNameByNumberFromPB(tele_num, &num_name, MMISMS_PBNAME_MAX_LEN);

        //search name from PB
        if(find_result)
        {
            if (MMIAPICOM_WstrnstrExt(num_name.wstr_ptr,
                                   num_name.wstr_len,
                                   g_mmisms_global.search_msg.search_str,
                                   g_mmisms_global.search_msg.search_len,
                                   FALSE
                                   ))
            {
                return TRUE;
            }
        }
    }

    //search msg content
    if(0 != read_msg.read_content.length)
    {
        uint16 convert_ucs2_msg_len = 0;
        wchar *convert_ucs2_msg_ptr = PNULL;

        convert_ucs2_msg_ptr = SCI_ALLOCAZ((MMISMS_MAX_UCS2_MESSAGE_LEN + 1)*sizeof(wchar));

        if (PNULL == convert_ucs2_msg_ptr)
        {
            return FALSE;
        }

        if (MN_SMS_DEFAULT_ALPHABET == read_msg.read_content.alphabet)
        {
            convert_ucs2_msg_len = MIN(read_msg.read_content.length,MMISMS_MAX_UCS2_MESSAGE_LEN);
            
            convert_ucs2_msg_len = MMIAPICOM_Default2Wchar((uint8 *)read_msg.read_content.data,
                                                            convert_ucs2_msg_ptr,
                                                            convert_ucs2_msg_len
                                                            );
        }
        else if (MN_SMS_8_BIT_ALPHBET == read_msg.read_content.alphabet)
        {
            convert_ucs2_msg_len = MIN(read_msg.read_content.length,MMISMS_MAX_UCS2_MESSAGE_LEN);
            
            MMI_STRNTOWSTR(
                                            convert_ucs2_msg_ptr,
                                            convert_ucs2_msg_len,
                                            (uint8 *)read_msg.read_content.data,
                                            convert_ucs2_msg_len,
                                            convert_ucs2_msg_len
                                            );
        }
        else
        {
            convert_ucs2_msg_len = MIN(read_msg.read_content.length/sizeof(wchar),MMISMS_MAX_UCS2_MESSAGE_LEN);
        
            //SCI_TRACE_LOW:"SearchStrInReadMsg dst_len = %d count = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_551_112_2_18_2_52_51_97,(uint8*)"dd", convert_ucs2_msg_len, read_msg.read_content.length);
            
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            MMI_MEMCPY( 
                        (uint8 *)convert_ucs2_msg_ptr,
                        MMISMS_MAX_UCS2_MESSAGE_LEN * sizeof(wchar),
                        read_msg.read_content.data,
                        read_msg.read_content.length,
                        convert_ucs2_msg_len * sizeof(wchar)
                        );
#else
        GUI_UCS2L2UCS2B((uint8 *)convert_ucs2_msg_ptr,
                        convert_ucs2_msg_len * sizeof(wchar),
                        read_msg.read_content.data,
                        convert_ucs2_msg_len * sizeof(wchar)
                        );
#endif
        }

        if(MMIAPICOM_WstrnstrExt(
                              convert_ucs2_msg_ptr, 
                              convert_ucs2_msg_len, 
                              g_mmisms_global.search_msg.search_str, 
                              g_mmisms_global.search_msg.search_len,
                              FALSE))
        {
            SCI_FREE(convert_ucs2_msg_ptr);

            return TRUE;
        }

        SCI_FREE(convert_ucs2_msg_ptr);
    }
    
    return FALSE;
}

/*****************************************************************************/
//     Description : search string in sms 
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchStrInMMSMsg(
							 MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id
							 )
{
    uint16 num_len = 0;
    wchar pb_name[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MMI_STRING_T num_name = {0};   
    BOOLEAN find_result = FALSE;
    wchar convert_ucs2_msg[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    uint16 convert_ucs2_msg_len = 0;

    num_name.wstr_len  = 0;
    num_name.wstr_ptr = pb_name;

    if (0 == g_mmisms_global.search_msg.search_len)
    {
        return FALSE;
    }

    num_len = strlen((char *)cur_unit_id->sms_state.number.sender);

    if(0 != num_len)
    {
        //search number
        SCI_MEMSET(pb_name, 0x00, sizeof(pb_name));
        num_name.wstr_ptr = pb_name;
        num_name.wstr_len = MIN(num_len, MMISMS_PBNAME_MAX_LEN);

        MMI_STRNTOWSTR(num_name.wstr_ptr,
                                    MMISMS_PBNAME_MAX_LEN,
                                    (uint8 *)cur_unit_id->sms_state.number.sender,
                                    num_len,
                                    num_name.wstr_len);
#if 0
        if (MMIAPICOM_Wstrnstr(num_name.wstr_ptr,
                                            num_name.wstr_len,
                                            g_mmisms_global.search_msg.search_str,
                                            g_mmisms_global.search_msg.search_len
                                            ))
        {
            return TRUE;
        }
#endif
        //search name from pb
        SCI_MEMSET(pb_name, 0x00, sizeof(pb_name));
        num_name.wstr_len = 0;
        num_name.wstr_ptr = pb_name;

        find_result = MMISMS_GetNameByNumberFromPB(cur_unit_id->sms_state.number.sender, &num_name, MMISMS_PBNAME_MAX_LEN);

        if(find_result)
        {
            if (MMIAPICOM_WstrnstrExt(num_name.wstr_ptr,
                                                num_name.wstr_len,
                                                g_mmisms_global.search_msg.search_str,
                                                g_mmisms_global.search_msg.search_len,
                                                FALSE
                                                ))
            {
                return TRUE;
            }
        }
    }

    //search msg short content
    if (0 != cur_unit_id->sms_state.display_content.content_len)
    {
        convert_ucs2_msg_len = MIN(cur_unit_id->sms_state.display_content.content_len,MMIMESSAGE_DIAPLAY_LEN);
    
        //SCI_TRACE_LOW:"SearchStrInMMSMsg dst_len = %d count = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_657_112_2_18_2_52_51_98,(uint8*)"dd", convert_ucs2_msg_len, cur_unit_id->sms_state.display_content.content_len);
        
        MMI_WSTRNCPY( 
                                convert_ucs2_msg,
                                MMIMESSAGE_DIAPLAY_LEN,
                                (wchar *)cur_unit_id->sms_state.display_content.content,
                                cur_unit_id->sms_state.display_content.content_len,
                                convert_ucs2_msg_len
                                );

        if(MMIAPICOM_WstrnstrExt(
                            convert_ucs2_msg, 
                            convert_ucs2_msg_len, 
                            g_mmisms_global.search_msg.search_str, 
                            g_mmisms_global.search_msg.search_len,
                            FALSE))
        {
            return TRUE;
        }
    }

#ifdef MMISEARCH_SUPPORT_ONLYMMS
    //Search the number of sender
    {
        wchar   sender[MMISMS_NUMBER_MAX_LEN + 1] = {0};
        int16   sender_len = 0;

        MMIAPICOM_StrToWstr(cur_unit_id->sms_state.number.sender, sender);
        sender_len = MMIAPICOM_Wstrlen(sender);
        sender_len = MIN(sender_len, MMISMS_NUMBER_MAX_LEN);

        if(MMIAPICOM_WstrnstrExt((wchar *)sender, sender_len, g_mmisms_global.search_msg.search_str, g_mmisms_global.search_msg.search_len, FALSE))
        {
            return TRUE;
        }
    }

    //Search mms total conent
    if (MMIAPIMMS_SearchMMSContent(cur_unit_id->sms_state.record_id,
        g_mmisms_global.search_msg.search_str,
        g_mmisms_global.search_msg.search_len))
        {
            return TRUE;
        }
    
#endif    
    return FALSE;
}

/*****************************************************************************/
//     Description : search string in push 
//    Global resource dependence : 
//  Author: minghu.mao
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchStrInPushMsg(MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id)
{
    uint16 num_len = 0;
    wchar pb_name[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MMI_STRING_T num_name = {0};   
    BOOLEAN find_result = FALSE;
    wchar convert_ucs2_msg[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    uint16 convert_ucs2_msg_len = 0;

    num_name.wstr_len  = 0;
    num_name.wstr_ptr = pb_name;

    if (0 == g_mmisms_global.search_msg.search_len)
    {
        return FALSE;
    }

    num_len = strlen((char *)cur_unit_id->sms_state.number.sender);

    if(0 != num_len)
    {

        //search number
        SCI_MEMSET(pb_name, 0x00, sizeof(pb_name));
        num_name.wstr_ptr = pb_name;
        num_name.wstr_len = MIN(num_len, MMISMS_PBNAME_MAX_LEN);

        MMI_STRNTOWSTR(num_name.wstr_ptr, MMISMS_PBNAME_MAX_LEN,
            (uint8 *)cur_unit_id->sms_state.number.sender, num_len, num_name.wstr_len);
#if 0
        if (MMIAPICOM_Wstrnstr(num_name.wstr_ptr, num_name.wstr_len,
            g_mmisms_global.search_msg.search_str,
            g_mmisms_global.search_msg.search_len))
        {
            return TRUE;
        }
#endif
        //search name from pb
        SCI_MEMSET(pb_name, 0x00, sizeof(pb_name));
        num_name.wstr_len = 0;
        num_name.wstr_ptr = pb_name;

        find_result = MMISMS_GetNameByNumberFromPB(cur_unit_id->sms_state.number.sender, &num_name, MMISMS_PBNAME_MAX_LEN);

        if(find_result)
        {
            if (MMIAPICOM_WstrnstrExt(num_name.wstr_ptr, num_name.wstr_len,
                g_mmisms_global.search_msg.search_str,
                g_mmisms_global.search_msg.search_len,
                FALSE))
            {
                return TRUE;
            }
        }
    }

    //search msg content
    if (0 != cur_unit_id->sms_state.display_content.content_len)
    {
        convert_ucs2_msg_len = MIN(cur_unit_id->sms_state.display_content.content_len,MMIMESSAGE_DIAPLAY_LEN);
    
        //SCI_TRACE_LOW:"SearchStrInPushMsg dst_len = %d count = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_746_112_2_18_2_52_51_99,(uint8*)"dd", convert_ucs2_msg_len, cur_unit_id->sms_state.display_content.content_len);
        
        MMI_WSTRNCPY(convert_ucs2_msg, MMIMESSAGE_DIAPLAY_LEN,
            (wchar *)cur_unit_id->sms_state.display_content.content,
            cur_unit_id->sms_state.display_content.content_len,
            convert_ucs2_msg_len);

        if(MMIAPICOM_WstrnstrExt(convert_ucs2_msg, convert_ucs2_msg_len, 
            g_mmisms_global.search_msg.search_str, 
            g_mmisms_global.search_msg.search_len,
            FALSE))
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//     Description : init search sms flag
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
LOCAL void InitSearchFlag(
        					 MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id
        					 )
{
    while(PNULL != cur_unit_id)
    {
        cur_unit_id->sms_state.flag.is_search = FALSE;

        cur_unit_id = cur_unit_id->next_ptr;
    }

    return;
}

/*****************************************************************************/
//     Description : open search match SMS cnf
//    Global resource dependence : 
//  Author: tao.xue
//    Note: reading finished and etc
/*****************************************************************************/
LOCAL void SMS_OpenMatchedSmsCnf(void)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    uint8 num_len = 0;
    uint8 number [MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_READ_MSG_T read_msg = {0};
    if(PNULL == s_cur_match_order_id)
    {
        return;
    }
    box_type = MMISMS_GetSmsStateBoxtype(s_cur_match_order_id);
    dual_sys = s_cur_match_order_id->flag.dual_sys;

    switch (box_type) 
    {
    case MMISMS_BOX_MT:
    case MMISMS_BOX_SENDSUCC:
        MMISMS_ClearOperInfo();
        //MMISMS_SetReadedSms2Window(box_MMISMS_EnterShowSmsWintype);
        
        read_msg = MMISMS_GetReadResult();
        MMISMS_EnterShowSmsWin(read_msg);    
        break;
                        
    case MMISMS_BOX_SENDFAIL:
    case MMISMS_BOX_NOSEND:
        {
            MMI_STRING_T msg_content = {0};

            // set the init SMS content
            MMISMS_GetSMSContent(&msg_content);

            num_len = MMISMS_GetOriginNum(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

            // open edit window
            MMISMS_WriteNewMessage(
                MN_DUAL_SYS_MAX,
                &msg_content,
                FALSE,
                number,
                num_len
                );

#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_BOX_SENDFAIL == box_type)
            {
                MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;
                MMISMS_ORDER_ID_T order_id = PNULL;

                MMISMS_SetCurOrderArrIndex(0);
                order_id = MMISMS_GetCurOperationOrderId();

                event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);

                if (PNULL != event_fast_info_ptr)
                {
                    MMISMS_SetIsTimerMode(TRUE);

                    MMISMS_TIMERMSG_SetCurEvent(event_fast_info_ptr);
                }
            }
#endif

            if (PNULL != msg_content.wstr_ptr)
            {
                SCI_FREE(msg_content.wstr_ptr);
                msg_content.wstr_ptr = PNULL;
            }
        }
        break;
        
    default:
        break;
    }
    if(MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID)) 
    {
        MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    }
}

/*****************************************************************************/
//     Description : open match msg in SMS list
//    Global resource dependence : 
//  Author: tao.xue
//    Note: 
/*****************************************************************************/
LOCAL void SMS_OpenMatchedItem(uint32 user_data)
{
    MMI_STRING_T   prompt_str = {0};
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    uint32 msg_record_id = 0;

    s_cur_match_order_id = (MMISMS_ORDER_ID_T)user_data;
    if(PNULL == s_cur_match_order_id)
    {
        return;
    }
    
    box_type = MMISMS_GetSmsStateBoxtype(s_cur_match_order_id);
    
    //to get the info of current selected item
    MMISMS_SetCurSearchOperationOrderId(box_type, s_cur_match_order_id);
    msg_record_id = s_cur_match_order_id->record_id;
    switch(MMISMS_GetMsgType())
    {
    case MMISMS_TYPE_SMS:
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
            break;
        }
#endif
        //SCI_TRACE_LOW:"HandleOkMsgInSmsList index is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_896_112_2_18_2_52_51_100,(uint8*)"d",g_mmisms_global.operation.cur_order_index);
        MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
        // to read the pointed SMS
        {
            MMISMS_ORDER_ID_T cur_order_id = MMISMS_GetCurOperationOrderId(); 
            ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
            if(PNULL == cur_order_id)
            {
                return;
            }
            //׼��ȫ�ֱ���
            SCI_MEMSET(&g_mmisms_global.read_msg, 0, sizeof(MMISMS_READ_MSG_T) );
            g_mmisms_global.operation.oper_status = MMISMS_READ_MATCH_SMS;
                                
            err_code = MNSMS_ReadSmsEx(
                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, 
                (MN_SMS_STORAGE_E)cur_order_id->flag.storage, 
                cur_order_id->record_id 
                );
            if(ERR_MNSMS_NONE != err_code)
            {
                MMIPUB_CloseWaitWin(MMISMS_READSMS_WAIT_WIN_ID);
            }
        }
        break;
        
    case MMISMS_TYPE_MMS:
		//���cr MS00150274,U��ʹ�ù����в��ܿ�MMS
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
        if (MMIAPIBROWSER_IsRunning())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MMS_WEB_IN_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#endif
#endif
        MMISMS_ClearOperInfo();
        switch(box_type) 
        {                
            case MMISMS_BOX_MT:
            case MMISMS_BOX_SENDSUCC:
            case MMISMS_BOX_SENDFAIL:
                //if (order_id != PNULL)
                {
                    MMIAPIMMS_ReadMMS((uint32)msg_record_id);
                }                    
                //MMIAPIMMS_OpMMS((uint32)order_id->record_id, MMIMMS_OP_PREVIEW, win_id);
                break;
            case MMISMS_BOX_NOSEND:
                //MMIAPIMMS_OpMMS((uint32)order_id->record_id, MMIMMS_OP_EDIT, win_id);
                //if (order_id != PNULL)
                {
                    MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT);
                    MMIAPIMMS_DraftEditMMS((uint32)msg_record_id);
                }                    
                break;
            default:
                break;
        }
        //�������Ҽ��л������ţ������ؼ���ص��˸ոն����Ž��� cr117926
        if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
        {
            MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
        }
        if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
        {
            MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
        }
        if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
        {
            MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
        }
        break;
        
    case MMISMS_TYPE_WAP_PUSH:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            MMISMS_ClearOperInfo();                
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_WapPushEnter((uint16)msg_record_id);
#endif
            //�������Ҽ��л������ţ������ؼ���ص��˸ոն����Ž��� cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
            }
        }
        break;
        
    case MMISMS_TYPE_WAP_OTA:
    case MMISMS_TYPE_MMS_OTA:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {              
            MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_OTAEnter((uint16)msg_record_id);
#endif
            //�������Ҽ��л������ţ������ؼ���ص��˸ոն����Ž��� cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
            {
                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
            }
        }
        break;

    default: 
        break;
    }
}
/*****************************************************************************/
// 	Description : MMISMS_LoadSearchRstItem
//	Global resource dependence :
//  Author:   plum.peng
//	Note:
/*****************************************************************************/
LOCAL void SMS_SetMatchItemData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data)
{
    uint16 index = 0;
    uint16 display_content_index = 0;
    uint16 num_len = 0;
    uint8  time_len = 0;
    char   time_str[20] = {0};
    wchar  time_wstr[20] = {0};
    SCI_DATE_T   today_t = {0};
    uint8  disp_number[MMISMS_PBNUM_MAX_LEN+2] = {0};
    wchar  disp_num_wchar[MMISMS_PBNUM_MAX_LEN+1] = {0};
    wchar  sender_wchar[MMISMS_NUMBER_MAX_LEN+1] = {0};
    MMI_STRING_T   name_str = {0};
    BOOLEAN    find_result = FALSE;
    MMI_CTRL_ID_T	 ctrl_id = need_item_data_ptr->ctrl_id;
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_TM_T data_tm = {0};
    MMISMS_STATE_T sms_state = *(MMISMS_STATE_T *)user_data;
    
    index = need_item_data_ptr->item_index;
    
    SCI_MEMSET(&item_data, 0, sizeof(item_data));
    
    /* �����search��־��Ҫô�Ƿǳ����ţ�Ҫô�ǳ����ŵĵ�һ�� */
    if (sms_state.flag.is_search
        && sms_state.longsms_seq_num <= 1)
    {
        item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
        item_data.softkey_id[MIDDLE_BUTTON]    = TXT_VIEW;
        item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
        
        //box type
        item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[display_content_index].item_data.image_id = MMISMS_GetIconByStatus(&sms_state);
        display_content_index++;
        
        //name or number
        item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        name_str.wstr_ptr = SCI_ALLOCAZ( ( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar) );
        
        if (PNULL == name_str.wstr_ptr)
        {
            return ;
        }
        
        if(sms_state.flag.msg_type == MMISMS_TYPE_SMS)
        {
            find_result = MMIAPIPB_GetNameByNumber((MMIPB_BCD_NUMBER_T *)&sms_state.number.bcd_number, 
                &name_str, MMIPB_MAX_STRING_LEN, TRUE);
                
            if (find_result && name_str.wstr_len > 0)
            {
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = name_str.wstr_len;
            }
            else
            {
                num_len = MMIAPICOM_GenDispNumber(
                    MMIPB_GetNumberTypeFromUint8(sms_state.number.bcd_number.npi_ton),
                    MIN(MN_MAX_ADDR_BCD_LEN, sms_state.number.bcd_number.number_len),
                    sms_state.number.bcd_number.number,
                    disp_number,
                    MMISMS_NUMBER_MAX_LEN+2
                    );        

                MMIAPICOM_StrToWstr((uint8 *)disp_number, disp_num_wchar);

                item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = disp_num_wchar;
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = num_len;
            }
        }
        else
        {
            find_result = MMISMS_GetNameByNumberFromPB(sms_state.number.sender,&name_str, MMISMS_PBNAME_MAX_LEN);

            if (find_result && name_str.wstr_len > 0)
            {
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = name_str.wstr_len;
            }
            else
            {
                MMIAPICOM_StrToWstr((uint8 *)sms_state.number.sender, sender_wchar);
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = sender_wchar;
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = strlen((char *)sms_state.number.sender);
            }
        }
        display_content_index++;
        
/*        //card index
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        item_data.item_content[display_content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        if (MMISMS_MO_DRAFT != sms_state.flag.mo_mt_type)
        {
            if (sms_state.flag.dual_sys<MMI_DUAL_SYS_MAX )
            {
                item_data.item_content[display_content_index].item_data.image_id = MMIAPISET_GetSimIcon(sms_state.flag.dual_sys); 
            }
            else
            {
                item_data.item_content[display_content_index].item_data.image_id = (MMI_IMAGE_ID_T)0;
            }
        }
        else
        {
            if (MN_SMS_STORAGE_SIM == sms_state.flag.storage)
            {
                if (sms_state.flag.dual_sys<MMI_DUAL_SYS_MAX )
                {
                    item_data.item_content[display_content_index].item_data.image_id = MMIAPISET_GetSimIcon(sms_state.flag.dual_sys); 
                }
                else
                {
                    item_data.item_content[display_content_index].item_data.image_id = (MMI_IMAGE_ID_T)0;
                }
            }
        }
#endif
*/
        //date&time
        SCI_MEMSET(time_str, 0x00, sizeof(time_str));
        SCI_MEMSET(time_wstr, 0x00, sizeof(time_wstr));
            
        /* �õ�ʱ�� */
        data_tm = MMIAPICOM_Second2Tm(sms_state.time);
        time_len = sprintf((char*)time_str, "%02d/%02d %02d:%02d",
                        data_tm.tm_mon,
                        data_tm.tm_mday,
                        data_tm.tm_hour,
                        data_tm.tm_min);

        MMI_STRNTOWSTR(
                time_wstr,
                time_len,
                (uint8 *)time_str,
                time_len,
                time_len
                );

        /* �õ�ϵͳʱ�� */
        TM_GetSysDate(&today_t);
    
        if (today_t.year == data_tm.tm_year && today_t.mon == data_tm.tm_mon
                && today_t.mday == data_tm.tm_mday)
        {
            item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

            /* ֻ��ʾСʱ������ */
            item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = &(time_wstr[6]);
            item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = time_len - 6;
        }
        else
        {
            item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = time_wstr;

            /* ֻ��ʾ�·ݣ����� */
            item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = 5;
        }
        display_content_index++;
        
        //sms content
        item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = sms_state.display_content.content;
        item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = sms_state.display_content.content_len;

        GUILIST_SetItemData(ctrl_id, &item_data, index);

        if (name_str.wstr_ptr != PNULL)
        {
            SCI_FREE(name_str.wstr_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : g_mmisms_global
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSmsSearchMatchItem(MMISMS_ORDER_ID_T sms_state_ptr)
{
    MMISEARCH_DATA_T match_data = {0};
    MMISEARCH_MATCH_ITEM_T* matched_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(sizeof(MMISEARCH_MATCH_ITEM_T));
    
    if (PNULL != matched_item_ptr)
    {
        matched_item_ptr[0].match_item_type = MMISEARCH_TYPE_SMS;
        matched_item_ptr[0].match_item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2;
        matched_item_ptr[0].user_data = (uint32)sms_state_ptr;
        matched_item_ptr[0].open_pfunc = SMS_OpenMatchedItem;
        matched_item_ptr[0].set_data_pfunc = SMS_SetMatchItemData;

        match_data.match_item_ptr = matched_item_ptr;
        match_data.match_num = 1;
        if (!MMIAPISEARCH_SaveMatchItems(match_data))
        {
            MMIAPISEARCH_IstyleStop();
            SCI_FREE(matched_item_ptr);
            return FALSE;
        }

        SCI_FREE(matched_item_ptr);
        return TRUE;
    }
    return FALSE;
}
#endif
/*****************************************************************************/
//     Description : to handle the read SMS err
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void ReadSmsErr( 
                      MMISMS_ORDER_ID_T order_id //IN:
                      )
{
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if(MMISMS_FOLDER_BT == order_id->flag.folder_type)
    {
        MMISMS_DelBtSmsInfoInOrder(order_id);
    }
    else
    {
        MMISMS_DelSmsInfoInOrder(order_id);
    }
#else
    MMISMS_DelSmsInfoInOrder(order_id);
#endif
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
}

/*****************************************************************************/
//     Description : Set Readed SMS to global
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveReadedSmsToGlobal(
                                 MN_DUAL_SYS_E               dual_sys,
                                 MMISMS_READ_TYPE_E       msg_type,                //IN:
                                 APP_SMS_USER_DATA_T      *new_user_data_ptr,    //IN: the user data of new message 
                                 MN_CALLED_NUMBER_T       *address_ptr,            //IN:
                                 MN_SMS_ALPHABET_TYPE_E   alphabet_type            //IN: the alphabet type of new message
                                 )
{
#if 0
// no use
    MN_SMS_USER_VALID_DATA_T data_buf_t = {0};
#endif
    MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};
    BOOLEAN is_need_lost_prompt = FALSE;
    BOOLEAN is_need_last_lost_prompt = FALSE;
    // int32 i = 0;
    
    if (PNULL == new_user_data_ptr || PNULL == address_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:SaveReadedSmsToGlobal new_user_data_ptr address_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1272_112_2_18_2_52_52_101,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMISMS: SaveReadedSmsToGlobal alphabet_type = %d, msg_type = %d, dual_sys = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1276_112_2_18_2_52_52_102,(uint8*)"ddd", alphabet_type, msg_type, dual_sys);
    
    if ((MMISMS_READ_MOSMS == msg_type) || (MMISMS_READ_MTSMS == msg_type))
    {
        // limit the head length in a valid range.
        new_user_data_ptr->user_data_head_t.length = MIN(MN_SMS_MAX_USER_VALID_DATA_LENGTH, new_user_data_ptr->user_data_head_t.length);
        
        if (MN_SMS_DEFAULT_ALPHABET == alphabet_type) 
        {
            // because default alphabet table is different from ascii alphabet table, 
            // and in our system just display ascii character, we must convert the string.
            
            // store the user data to another space.

            MMISMS_DecodeUserDataHead(&(new_user_data_ptr->user_data_head_t), &user_head_ptr_t);
            g_mmisms_global.read_msg.language = MMISMS_JudgeR8Language(&user_head_ptr_t);
            MMISMS_DecodeR8Header(
                        &user_head_ptr_t,
                        &g_mmisms_global.read_msg.is_lock_shift,
                        &g_mmisms_global.read_msg.is_single_shift);

            MMISMS_FreeUserDataHeadSpace(&user_head_ptr_t);

            //SCI_TRACE_LOW:"MMISMSR8:SaveReadedSmsToGlobal language is %d,is_lock_shift is %d,is_single_shift is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1303_112_2_18_2_52_52_103,(uint8*)"ddd",g_mmisms_global.read_msg.language,g_mmisms_global.read_msg.is_lock_shift,g_mmisms_global.read_msg.is_single_shift);

/*
           if(MMISMS_R8_LANGUAGE_NONE == g_mmisms_global.read_msg.language)
            {
                new_user_data_ptr->user_valid_data_t.length = MMIAPICOM_Default2Ascii(
                    data_buf_t.user_valid_data_arr,
                    new_user_data_ptr->user_valid_data_t.user_valid_data_arr,
                    data_buf_t.length
                    );
            }
*/
        }
#ifdef MMI_SMS_CHAT_SUPPORT
        if(!MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) || (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) && MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID))) //���������Ķ������
#endif        
        {
            if (g_mmisms_global.operation.cur_order_index < MMISMS_SPLIT_MAX_NUM -1)
            {
                uint8 cur_order_index = g_mmisms_global.operation.cur_order_index;

                cur_order_index++;

                if (PNULL == g_mmisms_global.operation.cur_order_id_arr[cur_order_index])
                {
                    is_need_last_lost_prompt = TRUE;
                }
            }
            is_need_lost_prompt = TRUE;
        }        
        
        // assemble the SMS message, and save in g_mmisms_global.read_msg, may be a long message.
        MMISMS_AssembleSmsToGlobal(
            dual_sys,
            msg_type,
            new_user_data_ptr,  
            address_ptr,
            alphabet_type,
            &(g_mmisms_global.read_msg),
            is_need_lost_prompt,
            is_need_last_lost_prompt
            );
#ifdef IKEYBACKUP_SUPPORT
        //set readed msg to edit content for save order info
        if(MMISMS_RESTORE_SMS == MMISMS_GetCurrentOperStatus())
        {
            uint8 num_len = 0;
            uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
            MN_DUAL_SYS_E dual_sys_temp = MN_DUAL_SYS_MAX;
            
            MMISMS_ClearDestNum();
            num_len = MMISMS_GetOriginNum(&dual_sys_temp, number, MMISMS_PBNUM_MAX_LEN + 2);
            MMISMS_SetAddressToMessage(number, num_len, &g_mmisms_global.edit_content.dest_info.dest_list);
            SCI_MEMCPY(&g_mmisms_global.edit_content.tpdu_head, &g_mmisms_global.read_msg.head_data, sizeof(MN_SMS_USER_DATA_HEAD_T));
            SCI_MEMCPY(&g_mmisms_global.edit_content.send_message, &g_mmisms_global.read_msg.read_content, sizeof(MMISMS_CONTENT_T));
            g_mmisms_global.save_info.max_num = 1;
            if(g_mmisms_global.edit_content.tpdu_head.length > 0)
            {
                g_mmisms_global.save_info.max_num = g_mmisms_global.edit_content.tpdu_head.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
            }            
        }
#endif
    }
}

/*****************************************************************************/
//     Description : to assemble the SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_AssembleSmsToGlobal(
                               MN_DUAL_SYS_E              dual_sys,
                               MMISMS_READ_TYPE_E          msg_type,                //IN:
                               APP_SMS_USER_DATA_T        *new_user_data_ptr,    //IN: the user data of new message 
                               MN_CALLED_NUMBER_T          *address_ptr,            //IN:
                               MN_SMS_ALPHABET_TYPE_E     msg_dcs,                //IN: the dcs of new message
                               MMISMS_READ_MSG_T        *read_msg_ptr,
                               BOOLEAN is_need_lost_prompt,
                               BOOLEAN is_need_last_lost_prompt
                               )
{
    BOOLEAN app_result   = TRUE;
    uint16 refnum_in_save = 0;
    uint16 refnum_in_new = 0;
    uint8 seqnum_in_save = 0;
    uint8 seqnum_in_new = 0;
    int16  i = 0;
    uint16 prompt_len = 0;
    wchar prompt_str[MMISMS_PROMPT_STR_LEN + 1] = {0}; //(......)
    MMI_STRING_T sms_lost_promt = {0};
    
    if (PNULL == new_user_data_ptr || PNULL == address_ptr || PNULL == read_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AssembleSmsToGlobal new_user_data_ptr address_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1402_112_2_18_2_52_53_104,(uint8*)"");
        return;
    }
    
    refnum_in_save = MMISMS_GetRefNumFromUserHead(&(read_msg_ptr->head_data));
    refnum_in_new  = MMISMS_GetRefNumFromUserHead(&(new_user_data_ptr->user_data_head_t));

    //SCI_TRACE_LOW:"MMISMS_AssembleSmsToGlobal refnum_in_new = %d,refnum_in_save = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1409_112_2_18_2_52_53_105,(uint8*)"dd",refnum_in_new,refnum_in_save);
    
    // compare two ref num
    if (!((read_msg_ptr->head_data.length > 0)
        && (refnum_in_new == refnum_in_save)
        && (dual_sys == read_msg_ptr->dual_sys)
        && !MMISMS_IsR8Language(read_msg_ptr->language)) &&
        !((read_msg_ptr->head_data.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN)
        && (refnum_in_new == refnum_in_save)
        && (dual_sys == read_msg_ptr->dual_sys)
        && MMISMS_IsR8Language(read_msg_ptr->language))
        )
    {
        // the new message is first message
        SCI_MEMSET( &(read_msg_ptr->read_content), 0, sizeof(MMISMS_CONTENT_T) );     
        SCI_MEMSET( &(read_msg_ptr->address), 0, sizeof(MN_CALLED_NUMBER_T) );
        SCI_MEMSET( &(read_msg_ptr->time_stamp), 0, sizeof(MN_SMS_TIME_STAMP_T) );
    }

    // add the new message to the global variable g_mmisms_global        
    read_msg_ptr->read_content.alphabet = msg_dcs;

    // the ref num is same, indicate the SMS message is a long message.
    seqnum_in_save = GetSeqNumFromUserHead(&(read_msg_ptr->head_data));
    seqnum_in_new  = GetSeqNumFromUserHead(&(new_user_data_ptr->user_data_head_t));
    //SCI_TRACE_LOW:"MMISMS_AssembleSmsToGlobal seqnum_in_save = %d,seqnum_in_new = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1434_112_2_18_2_52_53_106,(uint8*)"dd",seqnum_in_save,seqnum_in_new);


    // compare the sequence number between the two message
    if (((0 != (seqnum_in_new - 1)%MMISMS_SPLIT_MAX_NUM) && seqnum_in_new > seqnum_in_save + 1)
        && is_need_lost_prompt
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
        && MMISMS_GetConcatenatedSMS()
#endif
       )
    {
        MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
        
        // indicate the part of a long message has been lost.
        // may be more than one SMS.
        // must display the lost prompt in the long message.
        for (i=seqnum_in_save+1; i<seqnum_in_new; i++)
        {
            if (MN_SMS_UCS2_ALPHABET != read_msg_ptr->read_content.alphabet
                && MMIAPICOM_IsASCIIString(sms_lost_promt.wstr_ptr, sms_lost_promt.wstr_len))//ascii
            {
                uint8 temp_prompt_str[MMISMS_PROMPT_STR_LEN + 1] = {0};
                uint8 temp_default_str[(MMISMS_PROMPT_STR_LEN + 1) * 2] = {0};
                uint16 default_len = 0;

                sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LEN);

                MMI_WSTRNTOSTR(temp_prompt_str, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_ptr, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_len);

                default_len = MMIAPICOM_Ascii2default(temp_prompt_str, temp_default_str, sms_lost_promt.wstr_len);

                app_result = AppendString2ReadContent(default_len, temp_default_str, read_msg_ptr->read_content.alphabet, read_msg_ptr);
            }
            else
            {
                SCI_MEMSET(prompt_str, 0, sizeof(prompt_str));

                sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LEN);

                // copy default prompt string to a buffer
                prompt_len = sms_lost_promt.wstr_len;

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                MMI_MEMCPY(  (uint8 *)&prompt_str,
                             MMISMS_PROMPT_STR_LEN * sizeof(wchar),
                             (uint8 *)sms_lost_promt.wstr_ptr,
                             prompt_len * sizeof(wchar),
                             prompt_len * sizeof(wchar)
                             );
#else
                GUI_UCS2L2UCS2B((uint8 *)&prompt_str,
                                MMISMS_PROMPT_STR_LEN * sizeof(wchar),
                                (const uint8 *)sms_lost_promt.wstr_ptr,
                                prompt_len * sizeof(wchar));
#endif
                app_result = AppendString2ReadContent(prompt_len*sizeof(wchar), (uint8 *)prompt_str, read_msg_ptr->read_content.alphabet, read_msg_ptr);
            }
            
            if (!app_result)
            {
                // if add the prompt will extend the string length, there must be some error.
                //SCI_TRACE_LOW:"MMISMS: AssembleSms new_seq = %d, save_seq = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1495_112_2_18_2_52_53_107,(uint8*)"dd", seqnum_in_new, seqnum_in_save);
                break;
            }
        } 
    }        
        
    read_msg_ptr->sms_num++;
    // the following will copy the new message to the global variable.
    // note: memory overwrite.
    app_result = AppendString2ReadContent(
        new_user_data_ptr->user_valid_data_t.length,
        new_user_data_ptr->user_valid_data_t.user_valid_data_arr,
        msg_dcs,
        read_msg_ptr
        );
    
    read_msg_ptr->read_type = msg_type;
    read_msg_ptr->dual_sys  = dual_sys;
    read_msg_ptr->read_content.language = read_msg_ptr->language;
    
    if (new_user_data_ptr->user_data_head_t.length > 0)
    {
        // save the user head
        read_msg_ptr->head_data = new_user_data_ptr->user_data_head_t;
    }
    
    // save address
    read_msg_ptr->address = *address_ptr;

    if (is_need_last_lost_prompt
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
        && MMISMS_GetConcatenatedSMS()
#endif
        )
    {
        uint8 max_num = 0;

        max_num  = MMISMS_GetMaxNumFromUserHead(&(new_user_data_ptr->user_data_head_t));

        if ((0 != (seqnum_in_new % MMISMS_SPLIT_MAX_NUM)) &&  seqnum_in_new < max_num)
        {
            uint16 cur_pos = seqnum_in_new / MMISMS_SPLIT_MAX_NUM + 1;
            uint8 loop_num = MIN(max_num , cur_pos * MMISMS_SPLIT_MAX_NUM);
        
            MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);

            for (i = seqnum_in_new; i < loop_num; i++)
            {
                if (MN_SMS_UCS2_ALPHABET != read_msg_ptr->read_content.alphabet
                    && MMIAPICOM_IsASCIIString(sms_lost_promt.wstr_ptr, sms_lost_promt.wstr_len))//ascii
                {
                    uint8 temp_prompt_str[MMISMS_PROMPT_STR_LEN + 1] = {0};
                    uint8 temp_default_str[(MMISMS_PROMPT_STR_LEN + 1) * 2] = {0};
                    uint16 default_len = 0;

                    sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LEN);

                    MMI_WSTRNTOSTR(temp_prompt_str, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_ptr, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_len);

                    default_len = MMIAPICOM_Ascii2default(temp_prompt_str, temp_default_str, sms_lost_promt.wstr_len);

                    app_result = AppendString2ReadContent(default_len, temp_default_str, read_msg_ptr->read_content.alphabet,read_msg_ptr);
                }
                else
                {
                    SCI_MEMSET(prompt_str, 0, sizeof(prompt_str));

                    sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LEN);

                    // copy default prompt string to a buffer
                    prompt_len = sms_lost_promt.wstr_len;

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                    MMI_MEMCPY(  (uint8 *)&prompt_str,
                                 MMISMS_PROMPT_STR_LEN * sizeof(wchar),
                                 (uint8 *)sms_lost_promt.wstr_ptr,
                                 prompt_len * sizeof(wchar),
                                 prompt_len * sizeof(wchar)
                                 );
#else
                    GUI_UCS2L2UCS2B((uint8 *)&prompt_str,
                                    MMISMS_PROMPT_STR_LEN * sizeof(wchar),
                                    (const uint8 *)sms_lost_promt.wstr_ptr,
                                    prompt_len * sizeof(wchar));
#endif
                    app_result = AppendString2ReadContent(prompt_len*sizeof(wchar), (uint8 *)prompt_str, read_msg_ptr->read_content.alphabet,read_msg_ptr);
                }
                
                if (!app_result)
                {
                    // if add the prompt will extend the string length, there must be some error.
                    //SCI_TRACE_LOW:"MMISMS: AssembleSms new_seq = %d, save_seq = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1585_112_2_18_2_52_53_108,(uint8*)"dd", seqnum_in_new, seqnum_in_save);
                    break;
                }
            }
        }
    }
    
}

/*****************************************************************************/
//     Description : get ref num from head
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL uint8 GetSeqNumFromUserHead(
                                  MN_SMS_USER_DATA_HEAD_T    *user_data_head_ptr    //IN:
                                  )
{
	uint8 seq_num = 0;

    if (PNULL == user_data_head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetSeqNumFromUserHead user_data_head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1606_112_2_18_2_52_53_109,(uint8*)"");
        return 0;
    }

    if(MN_SMS_CONCAT_8_BIT_REF_NUM == user_data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
    {
        seq_num = user_data_head_ptr->user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS];
    }
    else if(MN_SMS_CONCAT_16_BIT_REF_NUM == user_data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
    {
        seq_num = user_data_head_ptr->user_header_arr[MMISMS_HEAD_16BIT_SEQ_NUM_POS];
    }
	return seq_num;
}

/*****************************************************************************/
//     Description : get max num from head
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetMaxNumFromUserHead(
                                  MN_SMS_USER_DATA_HEAD_T    *user_data_head_ptr    //IN:
                                  )
{
    uint8 max_num = 0;

    if (PNULL == user_data_head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetMaxNumFromUserHead user_data_head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1633_112_2_18_2_52_53_110,(uint8*)"");
        return 0;
    }

    if(MN_SMS_CONCAT_8_BIT_REF_NUM == user_data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
    {
        max_num = user_data_head_ptr->user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
    }
    else if(MN_SMS_CONCAT_16_BIT_REF_NUM == user_data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
    {
        max_num = user_data_head_ptr->user_header_arr[MMISMS_HEAD_16BIT_MAX_NUM_POS];
    }

    return max_num;
}

/*****************************************************************************/
//     Description : set string to read content
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendString2ReadContent(                        //RETURN: TRUE, indicate the whole string can be copyed to the global variable
                                       uint16    str_len,    //IN:
                                       uint8    *str_ptr,    //IN:
                                       MN_SMS_ALPHABET_TYPE_E msg_dcs,//IN:
                                       MMISMS_READ_MSG_T        *read_msg_ptr
                                       )
{
    MMISMS_CONTENT_T *sms_content_ptr= PNULL;
    uint16 length = 0;
    BOOLEAN result = TRUE;
    
    if (PNULL == str_ptr || PNULL == read_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AppendString2ReadContent str_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1666_112_2_18_2_52_53_111,(uint8*)"");
        return FALSE;
    }
    if (0 < str_len)
    {
        // save the string to the global variable.
        sms_content_ptr = &(read_msg_ptr->read_content);
        
        if (sms_content_ptr->length + str_len <= MMISMS_MAX_MESSAGE_LEN)
        {
            length = str_len;
        }
        else
        {
            length = MMISMS_MAX_MESSAGE_LEN - sms_content_ptr->length;
            //SCI_TRACE_LOW:"MMISMS: AppendString2ReadContent read_content_length = %d, append_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1682_112_2_18_2_52_53_112,(uint8*)"dd", sms_content_ptr->length, length);
            
            // indicate the part of the string is copyed to the global variable.
            result = FALSE;
        }
        
        if(sms_content_ptr->sms_num < MMISMS_SPLIT_MAX_NUM)
        {
            sms_content_ptr->split_sms_length[sms_content_ptr->sms_num] = length;
            sms_content_ptr->alphabet_ori[sms_content_ptr->sms_num] = msg_dcs;
            sms_content_ptr->sms_num++;
        }
        
        //SCI_TRACE_LOW:"AppendString2ReadContent len is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1695_112_2_18_2_52_53_113,(uint8*)"d",sms_content_ptr->length);
        SCI_MEMCPY(
            &(sms_content_ptr->data[sms_content_ptr->length]),
            str_ptr, 
            length
            );
        
        sms_content_ptr->length += length;
    }
    
    return (result);
}

/*****************************************************************************/
//     Description : to assemble the status report by the formate
//    Global resource dependence : g_mmisms_global.read_msg
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL void AssembleStatusReportToGlobal(
                                        MN_SMS_STATUS_REPORT_T    *status_report_ptr
                                        )
{
    MMI_STRING_T status_str = {0};
    MMI_STRING_T status_report_str = {0};
    MMISMS_READ_MSG_T        *read_msg_ptr = &(g_mmisms_global.read_msg);
    wchar cr_wstr = MMIAPICOM_GetCRCh();
    uint16 cr_wstr_len = 1;
    wchar *prompt_str = PNULL;
    uint16 prompt_str_len = 0;
    
    if (PNULL == status_report_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AssembleStatusReportToGlobal status_report_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1726_112_2_18_2_52_53_114,(uint8*)"");
        return;
    }
    SCI_MEMSET(&status_str, 0, sizeof(MMI_STRING_T));
    // clear read msg space
    SCI_MEMSET(
        read_msg_ptr,
        0,
        sizeof(MMISMS_READ_MSG_T)
        );
    
    // address and time
    read_msg_ptr->address     = status_report_ptr->dest_addr_t;
    read_msg_ptr->time_stamp = status_report_ptr->tp_dt;

    // prompt
    MMI_GetLabelTextByLang(
        TXT_SMS_STATUSREPORT, 
        &status_report_str
        );

    // status
    MMI_GetLabelTextByLang(((MMI_TEXT_ID_T)TXT_SMS_SR_SUCC + (SMS_MMI_TEXT_ID_E)status_report_ptr->tp_status), 
        &status_str
        );

    read_msg_ptr->read_content.alphabet = MN_SMS_UCS2_ALPHABET;

    prompt_str_len = status_report_str.wstr_len + cr_wstr_len + status_str.wstr_len;

    prompt_str = SCI_ALLOCA((prompt_str_len + 1)*sizeof(wchar));
    if (PNULL == prompt_str)
    {
        return;
    }

    SCI_MEMSET(prompt_str, 0x00, ((prompt_str_len + 1)*sizeof(wchar)));

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    MMI_WSTRNCPY(
        prompt_str,
        status_report_str.wstr_len,
        status_report_str.wstr_ptr,
        status_report_str.wstr_len,
        status_report_str.wstr_len
        );
#else
    GUI_UCS2L2UCS2B((uint8 *)prompt_str,
                    status_report_str.wstr_len * sizeof(wchar),
                    (const uint8 *)status_report_str.wstr_ptr,
                    status_report_str.wstr_len * sizeof(wchar));
#endif

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    MMI_WSTRNCPY(
        prompt_str + status_report_str.wstr_len,
        cr_wstr_len,
        &cr_wstr,
        cr_wstr_len,
        cr_wstr_len
        );
#else
    GUI_UCS2L2UCS2B((uint8 *)(prompt_str + status_report_str.wstr_len),
                    cr_wstr_len * sizeof(wchar),
                    (const uint8 *)&cr_wstr,
                    cr_wstr_len * sizeof(wchar));
#endif

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    MMI_WSTRNCPY(
        prompt_str + status_report_str.wstr_len + 1,
        status_str.wstr_len,
        status_str.wstr_ptr,
        status_str.wstr_len,
        status_str.wstr_len
        );
#else
    GUI_UCS2L2UCS2B((uint8 *)(prompt_str + status_report_str.wstr_len + 1),
                    status_str.wstr_len * sizeof(wchar),
                    (const uint8 *)status_str.wstr_ptr,
                    status_str.wstr_len * sizeof(wchar));
#endif

    AppendString2ReadContent(prompt_str_len*sizeof(wchar), (uint8 *)prompt_str, MN_SMS_UCS2_ALPHABET, &(g_mmisms_global.read_msg));
    
    read_msg_ptr->read_type = MMISMS_READ_STATUSREPORT;

    SCI_FREE(prompt_str);
}

/*****************************************************************************/
//     Description : to check whether the status is to be read
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MtIsNeedUpdateReadStatus(                    //RETURN: TRUE, need update 
                                 MMISMS_ORDER_ID_T order_id
                                 )
{
    BOOLEAN ret_val = FALSE;
    MMISMS_STATE_T order_info = {0};
    
    MMISMS_GetSmsInfoFromOrder(order_id,&order_info);    
    
    // if the SMS content is not read, the state will be changed.
    if (MMISMS_MT_TO_BE_READ == order_info.flag.mo_mt_type||
        MMISMS_MT_SR_TO_BE_READ == order_info.flag.mo_mt_type
        )
    {
        ret_val = TRUE;
    }

    return (ret_val);
}

/*****************************************************************************/
//     Description : to check whether there have content
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsExistContent( void )
{
    BOOLEAN is_exist_content = FALSE;
    
    if( g_mmisms_global.read_msg.read_content.length > 0)
    {
        is_exist_content = TRUE;
    }
#ifdef SEARCH_SUPPORT
    s_is_stop_async_search_sms = FALSE;
#endif
    return (is_exist_content);
}

/*****************************************************************************/
//     Description : to read the SMS called by window
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_AppReadSms( 
                              MMISMS_ORDER_ID_T    order_id    //IN:
                              )
{
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    uint16 max_sms_num = 0;
    MMISMS_STATE_T order_info = {0};
    MMISMS_OPER_STATUS_E oper_status = MMISMS_GetCurrentOperStatus();
    ERR_MNSMS_CODE_E mn_err_code = ERR_MNSMS_NONE;
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
	char handle_number[HANDLE_LEN] = {0};
	BOOLEAN is_extra_msg = FALSE;
    BOOLEAN ret_val = FALSE;
#endif
    
    max_sms_num=MMISMS_GetMaxSmsNumInOrder();
    
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AppReadSms order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1875_112_2_18_2_52_53_115,(uint8*)"");
        return FALSE;
    }    
    if ((MMISMS_READ_SMS != oper_status)
#if defined(MMI_SMS_EXPORT_SUPPORT)
        && (MMISMS_EXPORT_SMS != oper_status)
#endif
#ifdef MMI_SMS_COPY_SUPPORT
        && !MMISMS_IsCopyOperation(oper_status)
#endif
#ifdef MMI_SMS_MOVE_SUPPORT
        && !MMISMS_IsMoveOperation(oper_status)
#endif        
#ifdef MMI_SMS_CHAT_SUPPORT
        && (MMISMS_READ_CHAT_NUM_SMS != MMISMS_GetCurrentOperStatus())
#endif
        && (MMISMS_READ_FOR_TIMERMSG != MMISMS_GetCurrentOperStatus())
       )
    {
        MMISMS_ReadyReadSms();
    }
    
    if(
        1
#ifdef MMI_SMS_COPY_SUPPORT
       && !MMISMS_IsCopyOperation(oper_status) /*lint !e506*/
#endif

#ifdef MMI_SMS_MOVE_SUPPORT
       && !MMISMS_IsMoveOperation(oper_status) /*lint !e506*/
#endif

#if defined(MMI_SMS_EXPORT_SUPPORT)
       && (MMISMS_EXPORT_SMS != MMISMS_GetCurrentOperStatus())
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
       && (MMISMS_READ_CHAT_NUM_SMS != MMISMS_GetCurrentOperStatus())
#endif
#ifdef MMI_TIMERMSG_SUPPORT
       && (MMISMS_READ_FOR_TIMERMSG != MMISMS_GetCurrentOperStatus())
#endif	   
	   ) /*lint !e774*/
    {
        MMISMS_SetOperStatus(MMISMS_READ_SMS);
    }
    
    // get the status of this message 
    MMISMS_GetSmsInfoFromOrder(order_id,&order_info);
    
    storage = (MN_SMS_STORAGE_E)order_info.flag.storage;    
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_AppReadSms order_id = %d, max_sms_num = %d, record_id = %d, storage = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1912_112_2_18_2_52_54_116,(uint8*)"dddd", order_id, max_sms_num, order_info.record_id, storage);
    if (order_info.flag.dual_sys >=  MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AppReadSms order_info.flag.dual_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1915_112_2_18_2_52_54_117,(uint8*)"");
        return FALSE;
    }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if( MMISMS_FOLDER_BT == order_info.flag.folder_type )
    {
		MMIAPISMS_ClearLongMessageData();

		cur_order_id = MMISMS_GetCurOperationOrderId();
		if((0 == MMISMS_GetBoxTotalNumberFromOrder(MMISMS_GetCurBoxType()))
			|| (PNULL == cur_order_id)
			)
		{
			return FALSE;
		}
		
		ret_val = MMISMS_GetHandleInfoByRecordId(cur_order_id->record_id, handle_number, &is_extra_msg);

        if( (TRUE == ret_val ) && (TRUE == is_extra_msg ))
        {
            if( TRUE == MMISMS_GetMapAllowSync())
            {
	            MMISMS_SetMapAllowSync(FALSE);
            }
           else
            {
              return FALSE;
            }
        }
    }
#endif

    mn_err_code = MNSMS_ReadSmsEx(
        (MN_DUAL_SYS_E)order_info.flag.dual_sys, 
        (MN_SMS_STORAGE_E)order_info.flag.storage, 
        order_info.record_id 
        );
    if (ERR_MNSMS_NONE != mn_err_code)
    {

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        if(MMISMS_FOLDER_BT == order_id->flag.folder_type)
        {
            MMISMS_DelBtSmsInfoInOrder(order_id);
        }
        else
        {
            MMISMS_DelSmsInfoInOrder(order_id);
        }
#else    
        MMISMS_DelSmsInfoInOrder(order_id);
#endif

        MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AppReadSms order_info.flag.dual_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1915_112_2_18_2_52_54_117,(uint8*)"");
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : GetSMSBoxTypeByReadSMSCnf
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_BOX_TYPE_E GetSMSBoxTypeByReadSMSCnf(APP_MN_READ_SMS_CNF_T *sig_ptr)
{
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    BOOLEAN is_exist_sc_addr = FALSE;
    MMISMS_MO_MT_TYPE_E mo_mt_type = MMISMS_MO_DRAFT;
    
    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetSMSBoxTypeByReadSMSCnf sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1937_112_2_18_2_52_54_118,(uint8*)"");
        return box_type;
    }
    switch (sig_ptr->status)
    {
    //sort the MT SMS
    case MN_SMS_MT_READED:                   
    case MN_SMS_MT_TO_BE_READ:
        box_type = MMISMS_BOX_MT;
        break;
        
    case MN_SMS_MT_SR_TO_BE_READ:
    case MN_SMS_MT_SR_READED:
        box_type = MMISMS_BOX_MT;
        break;
        
    case MN_SMS_MO_TO_BE_SEND:
    case MN_SMS_MO_SR_NOT_REQUEST:          
    case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
    case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
    case MN_SMS_MO_SR_RECEIVED_AND_STORED:
        if (sig_ptr->sms_t_u.mo_sms_t.sc_addr_present && sig_ptr->sms_t_u.mo_sms_t.sc_addr_t.num_len > 0)
        {
            is_exist_sc_addr = TRUE;
        }
        else
        {
            is_exist_sc_addr = FALSE;
        }

        mo_mt_type = MMISMS_TranslateMNStatus2SFStatus(sig_ptr->status, MMISMS_MO_TYPE, is_exist_sc_addr);

        if (MMISMS_MO_SEND_SUCC == mo_mt_type)
        {
            box_type = MMISMS_BOX_SENDSUCC;
        }
        else if (MMISMS_MO_SEND_FAIL == mo_mt_type)
        {
            box_type = MMISMS_BOX_SENDFAIL;
        }
        else if (MMISMS_MO_DRAFT == mo_mt_type)
        {
            box_type = MMISMS_BOX_NOSEND;
        }
        break;
        
    default:
        break;
    }

    //SCI_TRACE_LOW:"MMI_SMS:GetSMSBoxTypeByReadSMSCnf box_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1987_112_2_18_2_52_54_119,(uint8*)"d", box_type);

    return box_type;
}


LOCAL BOOLEAN SMSisReadOperHasCnfHandler(MMISMS_OPER_STATUS_E op_stat)
{
    BOOLEAN ret = FALSE;
#ifdef MMI_SMS_CHAT_SUPPORT   
    if( MMISMS_READ_CHAT_NUM_SMS == op_stat)
    {
        ret = TRUE;
    }
#endif        
#ifdef MMI_TIMERMSG_SUPPORT
    if( MMISMS_READ_FOR_TIMERMSG == op_stat)
    {
        ret = TRUE;
    }
#endif     
#ifdef MMI_SMS_EXPORT_SUPPORT
    if( MMISMS_EXPORT_SMS ==op_stat)
    {
        ret = TRUE;
    }
#endif

    if( MMISMS_READ_SMS == op_stat)
    {
        ret = TRUE;
    }

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    if( MMISMS_READ_SMS_NOT_SHOW == op_stat)
    {
        ret = TRUE;
    }
#endif

    return ret;
}

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
// Description : handle keybackup case of reading operation
// Global resource dependence : 
// Author:sam.hua
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckAndHandleKeyBackupReadSMSCnf(APP_MN_READ_SMS_CNF_T *sig_ptr)
{
    if (PNULL== sig_ptr)
    {
        return TRUE;
    }

    if(PNULL != g_sms_backup_data_ptr
        && PNULL != g_backup_item_data_req_ptr
        && g_sms_is_backup
        && sig_ptr->record_id == g_sms_backup_data_ptr->record_id
        && sig_ptr->storage == g_sms_backup_data_ptr->storage)
    {//bake up sms
        MMIIKB_BACKUP_ITEM_DATA_RES_T item_data_res = {0};
        uint16 backed_num = 0;
        uint16 sms_num = 0;

        g_sms_backup_data_ptr->dual_sys = sig_ptr->item_sys;
        g_sms_backup_data_ptr->status = sig_ptr->status;
        //SCI_TRACE_LOW:"SMS: MMISMS_HandleUserReadSMSCnf g_sms_backup_data_ptr->time = %d, g_sms_backup_data_ptr->record_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2049_112_2_18_2_52_54_123,(uint8*)"dd", g_sms_backup_data_ptr->time, g_sms_backup_data_ptr->record_id);
        //memset 
        if (MN_SMS_MO_TO_BE_SEND == sig_ptr->status && !sig_ptr->sms_t_u.mo_sms_t.sc_addr_present)
        {
            //SCI_TRACE_LOW:"SMS: MMISMS_HandleUserReadSMSCnf sig_ptr->sms_t_u.mo_sms_t.sc_addr_t.num_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2054_112_2_18_2_52_54_124,(uint8*)"d",sig_ptr->sms_t_u.mo_sms_t.sc_addr_t.num_len);
            if (0 < sig_ptr->sms_t_u.mo_sms_t.sc_addr_t.num_len)
            {//for testing
                SCI_MEMSET(&sig_ptr->sms_t_u.mo_sms_t.sc_addr_t, 0, sizeof(MN_CALLED_NUMBER_T));
            }
        }
        SCI_MEMCPY(&g_sms_backup_data_ptr->sms_t_u, &sig_ptr->sms_t_u, sizeof(MN_SMS_U_T));

        item_data_res.op_type = g_backup_item_data_req_ptr->op_type;
        item_data_res.type = g_backup_item_data_req_ptr->type;
        item_data_res.begin = g_backup_item_data_req_ptr->begin;
        item_data_res.number = 1;
        item_data_res.buf_ptr = g_sms_backup_data_ptr;
        item_data_res.len = sizeof(MMISMS_BACKUP_DATA_T);
        item_data_res.is_need_free = FALSE;

        backed_num = g_backup_item_data_req_ptr->number;
        sms_num = MMISMS_GetSmsNumByStorage(MN_DUAL_SYS_1, MN_SMS_STORAGE_ME);
        sms_num -= MMISMS_GetUnhandleMeSmsNumber(); 
        if (++backed_num >= sms_num)
        {
            item_data_res.is_finished = TRUE;
            g_sms_is_backup = FALSE;
            MMISMS_InsertUnhandledSmsToOrder();
        }
        else
        {
            item_data_res.is_finished = FALSE;
        }
        item_data_res.error_code = MMIIKB_STATUS_SUCCESS;
        MMIAPIIKB_SendBackupItemDataRes(&item_data_res);
        SCI_FREE(g_sms_backup_data_ptr);
        g_sms_backup_data_ptr = PNULL;
        SCI_FREE(g_backup_item_data_req_ptr);
        g_backup_item_data_req_ptr = PNULL;
        return TRUE;
    }
    return FALSE;
}
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// Description : handle search sms case of reading operation
// Global resource dependence : 
// Author:sam.hua
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckAndHandleSearchReadSMSCnf(APP_MN_READ_SMS_CNF_T *sig_ptr)
{
    BOOLEAN is_exist_read_err = FALSE;
    APP_SMS_USER_DATA_T ascii_vaild = {0};
    MMISMS_ORDER_ID_T next_order_id = PNULL  ;
    BOOLEAN is_search = FALSE;

    if (PNULL== sig_ptr)
    {
        return TRUE;
    }

    if (s_is_stop_async_search_sms )
    {
        MMISMS_ClearOperInfo();
        MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_SMS);
        s_is_stop_async_search_sms  = FALSE;
        return TRUE ;
    }

    switch (sig_ptr->status)
    {
    case MN_SMS_MT_TO_BE_READ:
    case MN_SMS_MT_READED:
        MMISMS_DecodeUserHeadAndContent(sig_ptr->item_sys,
            sig_ptr->sms_t_u.mt_sms_t.user_head_is_exist,
            &sig_ptr->sms_t_u.mt_sms_t.dcs,
            &sig_ptr->sms_t_u.mt_sms_t.user_data_t,
            &ascii_vaild
            );
        // add the readed SMS to global
        MMISMS_SaveReadedSmsToGlobal(
            sig_ptr->item_sys,
            MMISMS_READ_MTSMS,
            &ascii_vaild,  
            &(sig_ptr->sms_t_u.mt_sms_t.origin_addr_t),
            sig_ptr->sms_t_u.mt_sms_t.dcs.alphabet_type
            );

        g_mmisms_global.read_msg.dual_sys    = sig_ptr->item_sys;
        MMISMS_CorrectTimeStamp(&sig_ptr->sms_t_u.mt_sms_t.time_stamp_t, MMISMS_MT_TYPE);
        g_mmisms_global.read_msg.time_stamp  = sig_ptr->sms_t_u.mt_sms_t.time_stamp_t;
        //SCI_TRACE_LOW:"sms:g_mmisms_global.read_msg.dual_sys = %d, g_mmisms_global.operation.cur_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2124_112_2_18_2_52_54_125,(uint8*)"dd",g_mmisms_global.read_msg.dual_sys, g_mmisms_global.operation.cur_type);
        break;
        
    case MN_SMS_MO_TO_BE_SEND :
    case MN_SMS_MO_SR_NOT_REQUEST :
    case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
    case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
    case MN_SMS_MO_SR_RECEIVED_AND_STORED:
        // is an MO SMS message.
        MMISMS_DecodeUserHeadAndContent(sig_ptr->item_sys,
            sig_ptr->sms_t_u.mo_sms_t.user_head_is_exist,
            &sig_ptr->sms_t_u.mo_sms_t.dcs,
            &sig_ptr->sms_t_u.mo_sms_t.user_data_t,
            &ascii_vaild
            );
        
        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2140_112_2_18_2_52_54_126,(uint8*)"d", ascii_vaild.user_data_head_t.length);
        
        // add the readed SMS to global
        MMISMS_SaveReadedSmsToGlobal(
            sig_ptr->item_sys,
            MMISMS_READ_MOSMS,
            &ascii_vaild,  
            &(sig_ptr->sms_t_u.mo_sms_t.dest_addr_t),
            sig_ptr->sms_t_u.mo_sms_t.dcs.alphabet_type
            );
        g_mmisms_global.read_msg.dual_sys = sig_ptr->item_sys;
#if 0
        //Add by huangfengming for cr150400
        time  = MMIAPICOM_Second2Tm(order_info.time);
        g_mmisms_global.read_msg.time_stamp.year = time.tm_year;
        g_mmisms_global.read_msg.time_stamp.month = time.tm_mon;
        g_mmisms_global.read_msg.time_stamp.day = time.tm_mday;
        g_mmisms_global.read_msg.time_stamp.hour = time.tm_hour;
        g_mmisms_global.read_msg.time_stamp.minute = time.tm_min;
        g_mmisms_global.read_msg.time_stamp.second = time.tm_sec;
#endif
        break;
        
    case MN_SMS_MT_SR_READED:
    case MN_SMS_MT_SR_TO_BE_READ:            
        AssembleStatusReportToGlobal(&sig_ptr->sms_t_u.report_sms_t);
        break;
        
    default:
        is_exist_read_err = TRUE;
        break;  
    }


    if(PNULL != g_mmisms_global.search_msg.next_long_order_id)
    {
        next_order_id = g_mmisms_global.search_msg.next_long_order_id;
        MNSMS_ReadSmsEx(
            (MN_DUAL_SYS_E)next_order_id->flag.dual_sys, 
            (MN_SMS_STORAGE_E)next_order_id->flag.storage, 
            next_order_id->record_id 
            );

        g_mmisms_global.search_msg.next_long_order_id = next_order_id->next_long_sms_ptr;
    }
    else
    {
        is_search = SearchStrInReadMsg(g_mmisms_global.read_msg);
        
        if(is_search)
        {
            SaveSmsSearchMatchItem(&g_mmisms_global.search_msg.search_ptr->sms_state);
            MMISMS_SetSearchFlag(&g_mmisms_global.search_msg.search_ptr->sms_state);
        }

        if ( g_mmisms_global.search_msg.search_ptr->next_ptr != PNULL )
        {
            SCI_MEMSET(&g_mmisms_global.read_msg,0x0,sizeof(MMISMS_READ_MSG_T));

            MMISMS_SearchMain( g_mmisms_global.search_msg.search_ptr->next_ptr );
        }
        else
        {
            MMISMS_ClearOperInfo();
            MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_SMS);
        }
    }
    return TRUE;
}
#endif

#ifdef MET_MEX_SUPPORT
/*****************************************************************************/
// Description : handle mex read case of reading operation
// Global resource dependence : 
// Author:sam.hua
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckAndHandleMexReadCnf(BOOLEAN result)
{
    MMISMS_CONTENT_T *read_msg_ptr = PNULL;

    read_msg_ptr = SCI_ALLOCAZ(sizeof(MMISMS_CONTENT_T));

    if (PNULL == read_msg_ptr)
    {
        return TRUE;
    }

    SCI_MEMCPY(read_msg_ptr, &g_mmisms_global.read_msg.read_content, sizeof(MMISMS_CONTENT_T));

    MMIMEX_ReadSmsContentCB((void *)read_msg_ptr,0,FALSE);

    if(read_msg_ptr->length == 0xFEDC) //mex
    {
        SCI_FREE(read_msg_ptr);
        //process started by mex application, should not go on.
        return TRUE;
    }

    SCI_FREE(read_msg_ptr);
    return FALSE;
}

#endif

/*****************************************************************************/
// Description : handle mms push case of reading operation
// Global resource dependence : 
// Author:sam.hua
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckAndHandleMMSPushMSGReadCnf(APP_MN_READ_SMS_CNF_T *sig_ptr)
{
    MMIVC_SMS_SEND_TYPE_E send_type = MMIVC_SMS_SEND_NORMAL;
    APP_SMS_USER_DATA_T ascii_vaild = {0};
    BOOLEAN             is_new_mms = FALSE;
    ERR_MNSMS_CODE_E    err_code = ERR_MNSMS_NONE;
    APP_MMS_USER_DATA_T mms_user_data_t = {0};
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint16 num_len = 0;
    uint8 sc_tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint16 sc_num_len = 0;
   

    if (sig_ptr == PNULL)
    {
        return TRUE;
    }
    
    // is an MT SMS message
    MNSMS_DecodeUserDataEx(
        sig_ptr->item_sys,        
        sig_ptr->sms_t_u.mt_sms_t.user_head_is_exist,
        &sig_ptr->sms_t_u.mt_sms_t.dcs,
        &sig_ptr->sms_t_u.mt_sms_t.user_data_t,
        &ascii_vaild.user_data_head_t,
        &ascii_vaild.user_valid_data_t
        );     

    send_type = MMISMS_IsMtForVcard(sig_ptr->item_sys, &ascii_vaild);

    if(MMIVC_SMS_SEND_NORMAL == send_type)
    {
        err_code = MNSMS_IsSupportMMSEx(
            sig_ptr->item_sys,
            ascii_vaild.user_data_head_t.length,
            ascii_vaild.user_data_head_t.user_header_arr,
            &is_new_mms
            );
        
        if ((ERR_MNSMS_NONE == err_code) && (is_new_mms))  //is mms push
        {
            mms_user_data_t.user_data_head_t.length = MIN(ascii_vaild.user_data_head_t.length, 40);
            SCI_MEMCPY(
                mms_user_data_t.user_data_head_t.user_header_arr,
                ascii_vaild.user_data_head_t.user_header_arr,
                mms_user_data_t.user_data_head_t.length
                );
            
            //SCI_TRACE_LOW:"MMISMS: OperateMtIndAsMmsPush mms head length = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2240_112_2_18_2_52_54_127,(uint8*)"d", mms_user_data_t.user_data_head_t.length);
            
            mms_user_data_t.user_valid_data_t = ascii_vaild.user_valid_data_t;
            
            if (sig_ptr->sms_t_u.mt_sms_t.origin_addr_t.num_len > 0)//get number
            {
                num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(sig_ptr->sms_t_u.mt_sms_t.origin_addr_t.number_type),
                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                    sig_ptr->sms_t_u.mt_sms_t.origin_addr_t.num_len),
                    sig_ptr->sms_t_u.mt_sms_t.origin_addr_t.party_num,
                    tele_num,
                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                    );
            }

            if (sig_ptr->sms_t_u.mt_sms_t.sc_addr_t.num_len > 0)//get number
            {
                sc_num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(sig_ptr->sms_t_u.mt_sms_t.sc_addr_t.number_type),
                                                                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                    sig_ptr->sms_t_u.mt_sms_t.sc_addr_t.num_len),
                                                                    sig_ptr->sms_t_u.mt_sms_t.sc_addr_t.party_num,
                                                                    sc_tele_num,
                                                                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                                                    );
            }

            //call the mms interface
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_DecodePush(sig_ptr->item_sys, &mms_user_data_t,tele_num,num_len, sc_tele_num,sc_num_len);
#endif
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
            MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MMS);
#endif
            MMISMS_ReadMMSPushFromUnhandledList(); //����һ�������û�з���false  
            // call MN API to update SMS status.
            MNSMS_UpdateSmsStatusEx(sig_ptr->item_sys, sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
            return TRUE;    
        }
    }
    return FALSE;
}

/*****************************************************************************/
// Description : check and update sms status if needed
// Global resource dependence : 
// Author:sam.hua
//    Note: 
/*****************************************************************************/
LOCAL void SMS_CheckandUpdateSMSReadFlag(MMISMS_ORDER_ID_T cur_order_id,
                                        APP_MN_READ_SMS_CNF_T *sig_ptr
#if defined(MMI_SMS_MOVE_SUPPORT) ||defined(MMI_SMS_COPY_SUPPORT)
                                        ,MMISMS_OPER_STATUS_E oper_status
#endif
                                        )
{
#ifdef MMI_SMS_CHAT_SUPPORT
        BOOLEAN is_need_update = FALSE;
#endif
        MMISMS_STATE_T order_info = {0};

    if (cur_order_id==PNULL||sig_ptr == PNULL)
    {
        return;
    }

    MMISMS_GetSmsInfoFromOrder(cur_order_id, &order_info);

#ifdef MMI_SMS_CHAT_SUPPORT
    if(((MMISMS_READ_CHAT_NUM_SMS == MMISMS_GetCurrentOperStatus())
        && (MMISMS_MT_TO_BE_READ == order_info.flag.mo_mt_type)
        && (0 == memcmp(sig_ptr->sms_t_u.mt_sms_t.origin_addr_t.party_num,
            cur_order_id->number.bcd_number.number, 
            cur_order_id->number.bcd_number.number_len
            )))
      )
    {
        is_need_update = TRUE;
    }
#endif

    // check whether this message need to update the status
    if ((MtIsNeedUpdateReadStatus(cur_order_id)
        && (sig_ptr->storage == order_info.flag.storage)
        && (sig_ptr->record_id == order_info.record_id)
#ifdef MMI_SMS_MOVE_SUPPORT            
        && !MMISMS_IsMoveOperation(oper_status)
#endif
#ifdef MMI_SMS_COPY_SUPPORT            
        && !MMISMS_IsCopyOperation(oper_status)
#endif            
#if defined(MMI_SMS_EXPORT_SUPPORT)
        && (MMISMS_EXPORT_SMS != MMISMS_GetCurrentOperStatus())
#endif
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
        && (MMISMS_READ_SMS_NOT_SHOW != MMISMS_GetCurrentOperStatus())
#endif
		)
#ifdef MMI_SMS_CHAT_SUPPORT
        || is_need_update
#endif
       )
    {
        MN_SMS_STATUS_E        update_status = MN_SMS_MT_READED;
        MN_SMS_STORAGE_E    storage = (MN_SMS_STORAGE_E)order_info.flag.storage;
        
        // according to the current status, set the update status.
        update_status = (MMISMS_MT_TO_BE_READ) == order_info.flag.mo_mt_type ? MN_SMS_MT_READED : MN_SMS_MT_SR_READED;
        
        // to modify the status from to_be_read to READ
        MNSMS_UpdateSmsStatusEx((MN_DUAL_SYS_E)order_info.flag.dual_sys,
            storage,
            order_info.record_id,
            update_status
            );
        g_mmisms_global.update_status_num++;

        g_mmisms_global.is_need_update_order = TRUE;

        cur_order_id->flag.is_need_update = TRUE;

    }
}


/*****************************************************************************/
//     Description : to handle the PS signal of APP_MN_READ_SMS_CNF when the user 
//                is reading SMS
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_HandleUserReadSMSCnf( 
                                        DPARAM param 
                                        )
{
    APP_MN_READ_SMS_CNF_T *sig_ptr = (APP_MN_READ_SMS_CNF_T *)param;
    MMISMS_ORDER_ID_T next_order_id = PNULL  ;
    BOOLEAN is_exist_read_err = FALSE;

    APP_SMS_USER_DATA_T ascii_vaild = {0};
    MMISMS_STATE_T order_info = {0};
    MMISMS_ORDER_ID_T cur_order_id = 0;
//    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    MMI_TM_T time = {0};
#if defined(MMI_SMS_MOVE_SUPPORT) ||defined(SEARCH_SUPPORT) ||defined(MMI_SMS_COPY_SUPPORT)
    MMISMS_OPER_STATUS_E oper_status = MMISMS_GetCurrentOperStatus();   
#endif    
    MMISMS_OPER_ERR_E oper_error = MMISMS_NO_ERR;

    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleUserReadSMSCnf sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2022_112_2_18_2_52_54_120,(uint8*)"");
        oper_error = MMISMS_FINISH_OPER_ERR;
        return;
    }

    if (sig_ptr->item_sys >=  MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleUserReadSMSCnf sig_ptr->item_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2028_112_2_18_2_52_54_121,(uint8*)"");
        MMIPUB_CloseWaitWin(MMISMS_READSMS_WAIT_WIN_ID);
        oper_error = MMISMS_FINISH_OPER_ERR;
        return;
    }
    
    //SCI_TRACE_LOW:"SMS: MMISMS_HandleUserReadSMSCnf dual_sys = %d, cause = %d, status = %d, record_id = %d, storage = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2034_112_2_18_2_52_54_122,(uint8*)"ddddd",sig_ptr->item_sys, sig_ptr->cause, sig_ptr->status, sig_ptr->record_id, sig_ptr->storage);

#ifdef IKEYBACKUP_SUPPORT
    if (CheckAndHandleKeyBackupReadSMSCnf(sig_ptr)) //Handle Backup read cnf
    {
        return;
    }
#endif

#ifdef MMI_MSG_RESEND
        else if (MMISMS_READ_RESEND_SMS == MMISMS_GetCurrentOperStatus())
        {
            Handle_ReadResendSms(param);
            return;
        }
#endif
#ifdef SEARCH_SUPPORT
    if(MMISMS_ASYNC_SEARCH_SMS == oper_status)    
    {
        CheckAndHandleSearchReadSMSCnf(sig_ptr);
        return;
    }
#endif

    if (MN_SMS_OPERATE_SUCCESS == sig_ptr->cause 
        && ( (MN_SMS_MT_TO_BE_READ == sig_ptr->status) || (MN_SMS_MT_READED == sig_ptr->status)))
    {
        if (CheckAndHandleMMSPushMSGReadCnf(sig_ptr))
        {
            return;
        }
    }

    do 
    {
        // the read procedure is success
         cur_order_id = MMISMS_GetCurOperationOrderId();
    
        if (PNULL == cur_order_id || sig_ptr->record_id != cur_order_id->record_id)
        {
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    		SCI_TRACE_LOW("mmisms cur_order_id read error");
            oper_error = MMISMS_FINISH_OPER_ERR;
            break;
        }
    
        //MN is busy
        if(MN_SMS_MN_BUSY == sig_ptr->cause)
        {
            MMISMS_OpenMsgBox(TXT_SMS_BUSY, IMAGE_PUBWIN_WARNING);
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);   
            oper_error = MMISMS_SYSTEM_BUSY;
            break;
        }
    
        //SMS has free
        if ((MN_SMS_SIM_SAVE_STATUS_FREE == sig_ptr->cause) ||
            (MN_SMS_FREE_SPACE == sig_ptr->status))
        {
            MMISMS_OpenMsgBox(TXT_SMS_READ_ERR, IMAGE_PUBWIN_WARNING);
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        if(MMISMS_FOLDER_BT == cur_order_id->flag.folder_type)
        {
            MMISMS_DelBtSmsInfoInOrder(cur_order_id);
        }
        else
        {
            MMISMS_DelSmsInfoInOrder(cur_order_id);
        }
#else		
            SCI_TRACE_LOW("MMISMS_HandleUserReadSMSCnf DelSmsInfo record_id=%d,store_id=%d",cur_order_id->record_id,cur_order_id->flag.storage);
            MMISMS_DelSmsInfoInOrder(cur_order_id);
#endif

            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);    
            oper_error = MMISMS_SMS_READ_FAIL;
            break;
        }
    
        //read success
        if (MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
        {    
    
            MMISMS_GetSmsInfoFromOrder(cur_order_id, &order_info);
    
            g_mmisms_global.update_info.total_num++;
            if(MMISMS_SPLIT_MAX_NUM <= g_mmisms_global.update_info.total_num)
            {
                g_mmisms_global.update_info.total_num = MMISMS_SPLIT_MAX_NUM;
            }
    
            //If need update sms status..
            SMS_CheckandUpdateSMSReadFlag(cur_order_id,
                                                                      sig_ptr
#if defined(MMI_SMS_MOVE_SUPPORT) ||defined(MMI_SMS_COPY_SUPPORT)            
                                                                     ,oper_status
#endif            
            );    
    
            switch (sig_ptr->status)
            {
            case MN_SMS_MT_TO_BE_READ:
            case MN_SMS_MT_READED:
                MMISMS_DecodeUserHeadAndContent(sig_ptr->item_sys,
                    sig_ptr->sms_t_u.mt_sms_t.user_head_is_exist,
                    &sig_ptr->sms_t_u.mt_sms_t.dcs,
                    &sig_ptr->sms_t_u.mt_sms_t.user_data_t,
                    &ascii_vaild
                    );
    #ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
                if (MN_SMS_MT_TO_BE_READ == sig_ptr->status && sig_ptr->record_id == MMISMS_GetScrollmsgRecordId())
                {
                    GUISTBSCROLLMSG_SetIsValid(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MESSAGE, FALSE);
                }
    #endif
    
                if((
                    FALSE       /*lint !e506*/
    #ifdef MMI_SMS_MOVE_SUPPORT
                   ||  MMISMS_IsMoveOperation(oper_status)/*lint !e506*/
    #endif
    #ifdef MMI_SMS_COPY_SUPPORT
                   ||  MMISMS_IsCopyOperation(oper_status)/*lint !e506*/
    #endif
                   )
                   && (cur_order_id->longsms_seq_num == 1))/*lint !e774*/
                {
                    if(MMISMS_HEAD_8BIT_LENGTH == ascii_vaild.user_data_head_t.length)
                    {
                        g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS] = 0;
                    }
    
                    else if(MMISMS_HEAD_16BIT_LENGTH == ascii_vaild.user_data_head_t.length)
                    {
                        g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS] = 0;
                        g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS + 1] = 0;
                    }
                }
               
                
                SCI_MEMCPY(&g_mmisms_global.read_msg.ascii_vaild,&ascii_vaild,sizeof(APP_SMS_USER_DATA_T));
                
                //SCI_TRACE_LOW:"MMISMS_HandleUserReadSMSCnf seq num is %d,max num is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2513_112_2_18_2_52_55_131,(uint8*)"dd",ascii_vaild.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS],ascii_vaild.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS]);
                
                // add the readed SMS to global
                MMISMS_SaveReadedSmsToGlobal(
                    sig_ptr->item_sys,
                    MMISMS_READ_MTSMS,
                    &ascii_vaild,  
                    &(sig_ptr->sms_t_u.mt_sms_t.origin_addr_t),
                    sig_ptr->sms_t_u.mt_sms_t.dcs.alphabet_type
                    );
                g_mmisms_global.read_msg.status = sig_ptr->status;            
                g_mmisms_global.read_msg.dual_sys    = sig_ptr->item_sys;
                MMISMS_CorrectTimeStamp(&sig_ptr->sms_t_u.mt_sms_t.time_stamp_t, MMISMS_MT_TYPE);
                g_mmisms_global.read_msg.time_stamp  = sig_ptr->sms_t_u.mt_sms_t.time_stamp_t;
    #ifdef MMI_SMS_CHAT_SUPPORT
                if(MN_SMS_MT_TO_BE_READ == sig_ptr->status)
                {
                    g_mmisms_global.read_msg.mo_mt_type = MMISMS_MT_TO_BE_READ;
                }
                else if(MN_SMS_MT_READED == sig_ptr->status)
                {
                    g_mmisms_global.read_msg.mo_mt_type = MMISMS_MT_HAVE_READ;
                }
    #endif
    #ifdef MMI_SMS_REPLY_PATH_SUPPORT
                g_mmisms_global.read_msg.reply_path_is_exist = sig_ptr->sms_t_u.mt_sms_t.reply_path_is_exist;
    
                if (g_mmisms_global.read_msg.reply_path_is_exist)
                {
                    g_mmisms_global.read_msg.sc_addr_t = sig_ptr->sms_t_u.mt_sms_t.sc_addr_t;
                }
    #endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
            if(MMISMS_FOLDER_BT == cur_order_id->flag.folder_type)
            {
                time  = MMIAPICOM_Second2Tm(order_info.time);
                g_mmisms_global.read_msg.time_stamp.year = time.tm_year - MMISMS_OFFSERT_YEAR;
                g_mmisms_global.read_msg.time_stamp.month = time.tm_mon;
                g_mmisms_global.read_msg.time_stamp.day = time.tm_mday;
                g_mmisms_global.read_msg.time_stamp.hour = time.tm_hour;
                g_mmisms_global.read_msg.time_stamp.minute = time.tm_min;
                g_mmisms_global.read_msg.time_stamp.second = time.tm_sec;
                g_mmisms_global.read_msg.time = order_info.time;
//                g_mmisms_global.read_msg.folder_type = MMISMS_FOLDER_BT;
            }
#endif

#ifdef DROPDOWN_NOTIFY_SHORTCUT
                SCI_TRACE_LOW("MMISMS_HandleUserReadSMSCnf read_msg.status=%d,read_msg.read_type=%d",g_mmisms_global.read_msg.status,g_mmisms_global.read_msg.read_type);
                //�������δ�����Ѷ���֪ͨNotification (SMS and Notification Connection)
                if((MN_SMS_MT_TO_BE_READ == g_mmisms_global.read_msg.status)&&
                    (MMISMS_READ_MTSMS == g_mmisms_global.read_msg.read_type))
                {
                    MMIEVENT_INFO_T event_info = {0};
                    MMISMS_NEW_SMS_INFO_T sms_info = {0};
                    event_info.event_type = SMS_READSMS_EVENT;
                    event_info.param_ptr = &sms_info;
                    sms_info.storage = cur_order_id->flag.storage;
                    sms_info.record_id = cur_order_id->record_id;
                    sms_info.cur_chat_order_id = cur_order_id;
                    SCI_TRACE_LOW("SMSNotify: trigger readed sms event! record_id=%d",sms_info.record_id);
                    MMIFW_TriggerEvent(&event_info);
                }
#endif
                //SCI_TRACE_LOW:"sms:g_mmisms_global.read_msg.dual_sys = %d, g_mmisms_global.operation.cur_type = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2545_112_2_18_2_52_55_132,(uint8*)"dd",g_mmisms_global.read_msg.dual_sys, g_mmisms_global.operation.cur_type);
                break;
                
            case MN_SMS_MO_TO_BE_SEND :
            case MN_SMS_MO_SR_NOT_REQUEST :
            case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
            case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
            case MN_SMS_MO_SR_RECEIVED_AND_STORED:
                // is an MO SMS message.
                MMISMS_DecodeUserHeadAndContent(sig_ptr->item_sys,
                    sig_ptr->sms_t_u.mo_sms_t.user_head_is_exist,
                    &sig_ptr->sms_t_u.mo_sms_t.dcs,
                    &sig_ptr->sms_t_u.mo_sms_t.user_data_t,
                    &ascii_vaild
                    );
    
                if((
                    FALSE   /*lint !e506*/
    #ifdef MMI_SMS_MOVE_SUPPORT
                   || MMISMS_IsMoveOperation(oper_status)/*lint !e506*/
    #endif
    #ifdef MMI_SMS_COPY_SUPPORT
                   || MMISMS_IsCopyOperation(oper_status)/*lint !e506*/
    #endif
                   )
                   && (cur_order_id->longsms_seq_num == 1))/*lint !e774*/
                {
                    if(MMISMS_HEAD_8BIT_LENGTH == ascii_vaild.user_data_head_t.length)
                    {
                        g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS] = 0;
                    }
    
                    else if(MMISMS_HEAD_16BIT_LENGTH == ascii_vaild.user_data_head_t.length)
                    {
                        g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS] = 0;
                        g_mmisms_global.read_msg.head_data.user_header_arr[MMISMS_HEAD_REF_NUM_POS + 1] = 0;
                    }
                }
                
                SCI_MEMCPY(&g_mmisms_global.read_msg.ascii_vaild,&ascii_vaild,sizeof(APP_SMS_USER_DATA_T));            
                
                //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUserReadSMSCnf head_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2579_112_2_18_2_52_55_133,(uint8*)"d", ascii_vaild.user_data_head_t.length);
                
                // add the readed SMS to global
                MMISMS_SaveReadedSmsToGlobal(
                    sig_ptr->item_sys,
                    MMISMS_READ_MOSMS,
                    &ascii_vaild,  
                    &(sig_ptr->sms_t_u.mo_sms_t.dest_addr_t),
                    sig_ptr->sms_t_u.mo_sms_t.dcs.alphabet_type
                    );
                g_mmisms_global.read_msg.status = sig_ptr->status;
                g_mmisms_global.read_msg.dual_sys = sig_ptr->item_sys;
                //Add by huangfengming for cr150400
                time  = MMIAPICOM_Second2Tm(order_info.time);
                g_mmisms_global.read_msg.time_stamp.year = time.tm_year - MMISMS_OFFSERT_YEAR;
                g_mmisms_global.read_msg.time_stamp.month = time.tm_mon;
                g_mmisms_global.read_msg.time_stamp.day = time.tm_mday;
                g_mmisms_global.read_msg.time_stamp.hour = time.tm_hour;
                g_mmisms_global.read_msg.time_stamp.minute = time.tm_min;
                g_mmisms_global.read_msg.time_stamp.second = time.tm_sec;
                g_mmisms_global.read_msg.time = order_info.time;
    #ifdef MMI_SMS_CHAT_SUPPORT
                if(MN_SMS_MO_TO_BE_SEND == sig_ptr->status)
                {
                    g_mmisms_global.read_msg.mo_mt_type = MMISMS_MO_SEND_FAIL;
                }
                else
                {
                    g_mmisms_global.read_msg.mo_mt_type = MMISMS_MO_SEND_SUCC;
                }
    #endif
                break;
                
            case MN_SMS_MT_SR_READED:
            case MN_SMS_MT_SR_TO_BE_READ:            
                AssembleStatusReportToGlobal(&sig_ptr->sms_t_u.report_sms_t);
                break;
                
            default:
                is_exist_read_err = TRUE;
                break;  
            }
        }
        
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if(MMISMS_FOLDER_BT == cur_order_id->flag.folder_type)
    {
        g_mmisms_global.read_msg.folder_type = MMISMS_FOLDER_BT;
    }
#endif

        // check whether there has the next message to read
        if (g_mmisms_global.operation.cur_order_index < MMISMS_SPLIT_MAX_NUM -1)
        {
            g_mmisms_global.operation.cur_order_index ++;
            next_order_id = MMISMS_GetCurOperationOrderId();  //������MMISMS_GetCurOperationOrderId
        }
        
        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleUserReadSMSCnf: the next_order_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2630_112_2_18_2_52_55_134,(uint8*)"d", next_order_id);
        
        if ( next_order_id != PNULL )   //Get Next message for long sms
        {
            if ( is_exist_read_err)
            {
                //ɾ�����ò����ڵ�,ͬʱҪע��next_pos
                ReadSmsErr(cur_order_id);
                //return???
            }
            if(ERR_MNSMS_NONE != MNSMS_ReadSmsEx(
                (MN_DUAL_SYS_E)next_order_id->flag.dual_sys, 
                (MN_SMS_STORAGE_E)next_order_id->flag.storage, 
                (MN_SMS_RECORD_ID_T)next_order_id->record_id 
                ))
            {
                oper_error = MMISMS_SMS_READ_FAIL;
            }
            break;
        }
        
        // indicate the whole SMS message, sp6ecially long message, is finished reading.
        if ( is_exist_read_err )
        {
            ReadSmsErr( cur_order_id);
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);   
            if (IsExistContent())
            {
    //============== MET MEX  start ===================        
    #ifdef MET_MEX_SUPPORT
                if(CheckAndHandleMexReadCnf(FALSE))
                {
                    break;
                }
    #endif  
            }
    //============== MET MEX  end ===================
            MMISMS_OpenMsgBox(TXT_SMS_READ_ERR, IMAGE_PUBWIN_WARNING);
            oper_error = MMISMS_SMS_READ_FAIL;
            break;
        }
    
        //finish get msg content and callback read result
        MMISMS_SetCurOrderArrIndex(0);
    
    #ifdef MMI_SMS_COPY_SUPPORT        
        //do Copy operation
        if(MMISMS_IsCopyOperation(oper_status))
        {
            g_mmisms_global.security_box_type = GetSMSBoxTypeByReadSMSCnf(sig_ptr);
    		g_mmisms_global.read_msg.is_locked = cur_order_id->flag.is_locked;
            g_mmisms_global.read_msg.status = sig_ptr->status;
            MMISMS_CopySaveSms(MMISMS_GetCurOperationOrderId());
            MMISMS_SetMoveSingleFlag(TRUE);    
            break;
        }
    #endif
    #ifdef MMI_SMS_MOVE_SUPPORT        
        //do MOVE operation
        if(MMISMS_IsMoveOperation(oper_status))
        {
            g_mmisms_global.security_box_type = GetSMSBoxTypeByReadSMSCnf(sig_ptr);
    		g_mmisms_global.read_msg.is_locked = cur_order_id->flag.is_locked;
            g_mmisms_global.read_msg.status = sig_ptr->status;
            MMISMS_MOVESaveSms(MMISMS_GetCurOperationOrderId());
            MMISMS_SetMoveSingleFlag(TRUE);        
            break;
        }        
    #endif            
    
        if (SMSisReadOperHasCnfHandler(MMISMS_GetCurrentOperStatus()))
        {
            next_order_id = MMISMS_GetCurOperationOrderId();

#ifdef MMI_SMS_CHAT_SUPPORT            
            g_mmisms_global.chat_oper_order_id = next_order_id;
#endif
            MMISMSAPP_HandleReadCnf(next_order_id->record_id, (MN_SMS_STORAGE_E)next_order_id->flag.storage,TRUE);
            break;
        }     
        
    #ifdef SEARCH_SUPPORT
        if (MMISMS_READ_MATCH_SMS == MMISMS_GetCurrentOperStatus())
        {
            SMS_OpenMatchedSmsCnf();
            break;
        }
    #endif
    //============== MET MEX  start ===================        
    #ifdef MET_MEX_SUPPORT
        if(CheckAndHandleMexReadCnf(TRUE))
        {
            break;
        }
    #endif
    //============== MET MEX  end =================== 
        break;
    }while(FALSE) ; 

    if(oper_error != MMISMS_NO_ERR)
    {
	    SCI_TRACE_LOW("mmisms oper_error = %d",oper_error);
        g_mmisms_global.operation.cur_order_index = 0;
        next_order_id = MMISMS_GetCurOperationOrderId();

#ifdef MMI_SMS_CHAT_SUPPORT
        g_mmisms_global.chat_oper_order_id = next_order_id;
#endif	    
        if(SMSisReadOperHasCnfHandler(MMISMS_GetCurrentOperStatus()))
        {
            //use head order id if long sms
            MMISMSAPP_HandleReadCnf(next_order_id->record_id, (MN_SMS_STORAGE_E)next_order_id->flag.storage,FALSE);
            return;            
        }	    
#ifdef MMI_SMS_COPY_SUPPORT       
        if(MMISMS_IsCopyOperation(oper_status))
        {
            MMISMSAPP_HandleCopyCnf(oper_error);
            return;
        }
#endif
#ifdef MMI_SMS_MOVE_SUPPORT        
        if(MMISMS_IsMoveOperation(oper_status))
        {
            MMISMSAPP_HandleMoveCnf(oper_error);
            return;            
        }
#endif
    }
}

/*****************************************************************************/
//     Description : get ref num from head
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetRefNumFromUserHead(
                                          MN_SMS_USER_DATA_HEAD_T    *user_data_head_ptr    //IN:
                                          )
{
    uint16 ref_num = 0;

    if (PNULL == user_data_head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetRefNumFromUserHead user_data_head_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2910_112_2_18_2_52_56_135,(uint8*)"");
        return 0;
    }

    if(MN_SMS_CONCAT_8_BIT_REF_NUM == user_data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
    {
        ref_num = user_data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS];
    }
    else if(MN_SMS_CONCAT_16_BIT_REF_NUM == user_data_head_ptr->user_header_arr[MMISMS_HEAD_IEI_POS])
    {
        ref_num = (user_data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS] << 8)
                      |(user_data_head_ptr->user_header_arr[MMISMS_HEAD_REF_NUM_POS+1]);
    }
    
    return ref_num;
}

/*****************************************************************************/
//     Description : to handle the PS signal of APP_MN_READ_SMS_CNF
//    Global resource dependence : 
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_HandleReadSmsCnf(DPARAM param)
{
    MN_DUAL_SYS_E dual_sys = ((APP_MN_READ_SMS_CNF_T*)param)->item_sys;
    MN_SMS_STORAGE_E storage = ((APP_MN_READ_SMS_CNF_T*)param)->storage;
    MN_SMS_STATUS_E status = ((APP_MN_READ_SMS_CNF_T*)param)->status;

    //SCI_TRACE_LOW:"SMS:MMISMS_HandleReadSmsCnf, record_id=%d, storage=%d, status=%d, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2936_112_2_18_2_52_56_136,(uint8*)"dddd", ((APP_MN_READ_SMS_CNF_T*)param)->record_id, storage, ((APP_MN_READ_SMS_CNF_T*)param)->status, dual_sys);
    
    if (!MMISMS_IsOrderOk() || (MMISMS_GetIsSMSRefresh(dual_sys) && MN_SMS_STORAGE_SIM == storage))
    {
        if (MN_SMS_MT_TO_BE_READ == status || MN_SMS_MT_READED == status)
        {
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
        }

        if (MMISMS_IsOrderOk() && (MN_SMS_STORAGE_SIM == storage) && MMISMS_GetIsSMSRefresh(dual_sys))//���� refresh�����������Ϣ
        {
            return;
        }

        MMISMS_HandleSMSOrder(param);
    }
    else
    {
        // indicate the init procedure is finished.
        MMISMS_HandleUserReadSMSCnf(param);
    }
}

/*****************************************************************************/
//     Description : Init Other Message List(MMS/OTA/Push...)
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitOtherMessageList(void)
{
    // BOOLEAN ret = FALSE;
    uint32 i = 0;

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        g_mmisms_global.is_sim_order_ok[i] = TRUE;
        g_mmisms_global.is_reinit[i] = FALSE;
    }

    MMISMS_SetOperStatus(MMISMS_NO_OPER);
    
    if (MMISMS_GetIsNeedReSort())
    {
        MMISMS_SetIsNeedReSort(FALSE);

        MMISMS_RefreshSMS(MMI_DUAL_SYS_MAX);

        MMISMS_InitSmsList();

        return;
    }

//    if (MMISMS_ORDER_BY_NAME == MMISMS_GetOrderType())
//    {
//
//    }

    //SCI_TRACE_LOW:"MMISMS: Sms Order OK! time=%ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_2985_112_2_18_2_52_56_137,(uint8*)"d", SCI_GetTickCount());

    g_mmisms_global.is_nv_order_ok = TRUE;
    
#ifdef MMI_OTA_SUPPORT   
    MMIAPIOTA_InitOTAPUSH();
#endif

    MMISMS_InsertUnhandledSmsToOrder();//���洢��δ���������еĶ���Ϣ���뵽��Ӧ��������ȥ��
    // ����������ϣ���mms���뵽������
    MMIAPIMMS_ListInit(); //MMS ��������ʼ,��mms��Ϣ���뵽order list��      
//    if (!ret) //��ʱmms��wap��û��׼����
//    {
//        StartSmsSortTimer();
//    }
//    else
//    {
//        StopSmsSortTimer();
//3 in 1 add mail list
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        MAIL_ListInit();
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
//
        MMISMS_ReadMMSPushFromUnhandledList(); //��ȡmms push ��Ϣ������mmsģ��
        MMISMS_CloseWaitingWin();
        MAIN_SetMsgState(TRUE);
//    }    
        
#if 0 //def MMIWIDGET_SUPPORT
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
        {
            if (0 < MMISMS_GetAllUnreadMsgCount())
            {
                MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_NEWSMS_ID, FALSE, TRUE);
            }
        }
#endif

    MMIAPIDM_SendSelfRegisterSMS(MMIAPISET_GetActiveSim());

#ifdef PUSH_EMAIL_SUPPORT  //Added by yuanl  2010.8.7
    MMIAPIMAIL_SetSmsOk(TRUE);
#endif /* PUSH_EMAIL_SUPPORT */

#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMsgValid();

    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    if (0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_SMS) //δ������
        && 0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS)//δ������
        )
    {
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
    }
    else
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
    }
    if (0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_PUSH)//δ��push��Ϣ
        && 0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS_OTA)//δ��MMS OTA
        && 0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_OTA)//δ��WAP OTA
        )
    {
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_NEW_PUSH);
    }
    else
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_PUSH);
    }
#else
    if (0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_SMS) //δ������
        && 0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS)//δ������
        && 0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_PUSH)//δ��push��Ϣ
        && 0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS_OTA)//δ��MMS OTA
        && 0 == MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_OTA)//δ��WAP OTA
        )
    {
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
    }
    else
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
    }
#endif

    if (!MMIAPISMS_IsSMSFull()//������
        && !MMIAPIMMS_IsMMSFull()//������
        ) 
    {
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_MESSAGE_FULL);
    }
    else
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_MESSAGE_FULL);
    }

#if defined(MMS_SUPPORT)
    if (0 == MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)) 
    {
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_NEW_MMS_NOTIFY);
    }
    else
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MMS_NOTIFY);
    }
#endif

#endif

}
/*****************************************************************************/
//     Description : to init SMS in SIM by order after received the SMS_READY_IND
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitSmsList(void)
{
    uint32 i = 0;
    BOOLEAN is_sim_order_ok = TRUE;
    BOOLEAN result = FALSE;
    
    //SCI_TRACE_LOW:"mmisms:MMISMS_InitSmsList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3079_112_2_18_2_52_56_138,(uint8*)"");
        
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(!(g_mmisms_global.is_sim_order_ok[i]))
        {
            is_sim_order_ok = FALSE;
            break;
        }
    }

    if(is_sim_order_ok && (g_mmisms_global.is_nv_order_ok))
    {
        //˵�����еĶ��Ѿ��ź���
        return;
    }
   
#ifdef WIN32
    MNSMS_WriteMeSmsSupportedEx(MN_DUAL_SYS_1, TRUE); 
#endif

    do
    {
        result = ReadSmsForOrder(MN_DUAL_SYS_1, MN_SMS_STORAGE_ME, 0 );
        if(!result)
        {
            break;
        }        

        // From Dual_SYS_1 polling,if  one order not OK, break directly!!      
        for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
        {
            result = ReadSmsForOrder((MN_DUAL_SYS_E)i, MN_SMS_STORAGE_SIM, 0 );
            if( !result )
            {
                break;
            }
        }
    }while(0);

    if(result)
    {
        MMISMS_InitSmsOrder();
        MMISMS_InitOtherMessageList();
    }
        
    //get the voice message waiting indicator flags
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        MNSMS_GetVoiMessFlagEx((MN_DUAL_SYS_E)i); 
    }
}

/*****************************************************************************/
//     Description : to read next record SMS in storage
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSmsForOrder(                                    //RETURN: TRUE, the SMS stored in the storage is ready
                              MN_DUAL_SYS_E         dual_sys,
                              MN_SMS_STORAGE_E        storage,    //IN:
                              MN_SMS_RECORD_ID_T    record_id    //IN: the start id
                              ) 
{
    BOOLEAN result = FALSE;
    uint16 max_sms_num = 0;
    uint16 used_sms_num= 0;
    int32 i = 0;
    MN_SMS_STATUS_E *status_ptr = PNULL;
    
    if (dual_sys >=  MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISMS:ReadSmsForOrder dual_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3149_112_2_18_2_52_56_139,(uint8*)"");
        return FALSE;
    }
    
    if (MN_SMS_STORAGE_SIM == storage)
    {
        if (g_mmisms_global.is_sim_order_ok[dual_sys])
        {
            return TRUE;
        }
    }
    
    if (MN_SMS_STORAGE_ME == storage)
    {
        if (g_mmisms_global.is_nv_order_ok)
        {
            //����Ҫ��������Ѿ��ź���
            return TRUE;
        }
    }
    
    if ((MN_SMS_STORAGE_ME == storage) || (MN_SMS_STORAGE_SIM == storage))
    {
        ERR_MNSMS_CODE_E sms_err_code = ERR_MNSMS_NONE;

        MNSMS_GetSmsNumEx(dual_sys, storage, &max_sms_num, &used_sms_num);
        SCI_TRACE_LOW("watchsms: ReadSmsForOrder max_sms_num= %d, used_sms_num= %d",max_sms_num,used_sms_num);

        if(0 != max_sms_num)
        {
            status_ptr = SCI_ALLOCAZ(max_sms_num * sizeof(MN_SMS_STATUS_E));
            if (PNULL == status_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS: ReadSmsForOrder status_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3179_112_2_18_2_52_56_140,(uint8*)"");
                return FALSE;
            }
        }

        
        sms_err_code = MNSMS_GetSmsStatusEx(
                                                                            dual_sys,
                                                                            storage, 
                                                                            &max_sms_num, 
                                                                            status_ptr
                                                                            );

        if ((MN_SMS_STORAGE_SIM == storage)
            && (ERR_MNSMS_SIM_NOT_READY == sms_err_code)
            && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
            //SCI_TRACE_LOW:"ReadSmsForOrder SIM NOT Ready"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3194_112_2_18_2_52_56_141,(uint8*)"");

            if(0 != max_sms_num)//MNSMS_GetSmsNumEx is Error, Temp for order OK
            {
                SCI_FREE(status_ptr);
            }

            return FALSE;
        }
        
        for (i=record_id; i<max_sms_num; i++)
        {

          if(0 != max_sms_num)
          {
            if (MN_SMS_FREE_SPACE != status_ptr[i])
            {
                //get the first valid SMS
                MMISMS_SetOperStatus(MMISMS_ORDER_SMS);

                //SCI_TRACE_LOW:"SMS ReadSmsForOrder storage=%d, record_id=%d, max_sms_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3208_112_2_18_2_52_56_142,(uint8*)"ddd", storage, i, max_sms_num);
                MNSMS_ReadSmsEx(dual_sys, storage, (MN_SMS_RECORD_ID_T )i);
                break;
            }
          }
        }

        if(0 != max_sms_num)
        {
            SCI_FREE(status_ptr);
        }
        
        if (i == max_sms_num)
        {
            // indicate no SMS from record_id stored in the storage
            
            //SCI_TRACE_LOW:"SMS: ReadSmsForOrder the storage(%d) is ready, record_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3220_112_2_18_2_52_56_143,(uint8*)"dd", storage, record_id);
            
            
            if (MN_SMS_STORAGE_SIM == storage)
            {
                g_mmisms_global.is_sim_order_ok[dual_sys] = TRUE;
            }
            else if (MN_SMS_STORAGE_ME == storage)
            {
                g_mmisms_global.is_nv_order_ok = TRUE;
            }
            
            result = TRUE;
        }    
    }
    else
    {
        result = FALSE;
    }
    return (result);
}

/*****************************************************************************/
//     Description : to read sms for order.
//    Global resource dependence : g_mmisms_order
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MultiSimReadSmsForOrder(APP_MN_READ_SMS_CNF_T* sig_ptr )
{
    BOOLEAN result = FALSE;
    uint32 i = 0;

    if (MN_SMS_STORAGE_ME == sig_ptr->storage)
    {
        result = ReadSmsForOrder(sig_ptr->item_sys, MN_SMS_STORAGE_ME, (MN_SMS_RECORD_ID_T)(sig_ptr->record_id+1));

        if (result)//ME�������
        {
            for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
            {
                if (g_mmisms_global.is_reinit[i])//refresh
                {
                    result = ReadSmsForOrder((MN_DUAL_SYS_E)i, MN_SMS_STORAGE_SIM, 0);

                    if (!result)
                    {
                        break;
                    }
                }
            }
        }
    }
    else
    {
        if(!g_mmisms_global.is_reinit[sig_ptr->item_sys])
        {
            return result;
        }

        result = ReadSmsForOrder(sig_ptr->item_sys, MN_SMS_STORAGE_SIM, (MN_SMS_RECORD_ID_T)(sig_ptr->record_id+1));

        if (result)//��SIM��������������
        {
            result = ReadSmsForOrder(sig_ptr->item_sys, MN_SMS_STORAGE_ME, 0);
        }

        if (result)//ME�������
        {
            for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
            {
                if ((sig_ptr->item_sys != i) && g_mmisms_global.is_reinit[i])//refresh
                {
                    result = ReadSmsForOrder((MN_DUAL_SYS_E)i, MN_SMS_STORAGE_SIM, 0);

                    if (!result)
                    {
                        break;
                    }
                }
            }
        }
    }

    return result;
}

/*****************************************************************************/
//     Description : to handle the signal APP_MN_READ_SMS_CNF
//    Global resource dependence : g_mmisms_order
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_HandleSMSOrder(                //RETURN: 
                                  DPARAM param    //IN:
                            )
{
    APP_MN_READ_SMS_CNF_T* sig_ptr = (APP_MN_READ_SMS_CNF_T*)param;
    APP_SMS_USER_DATA_T sms_user_data_t = {0};
    BOOLEAN result = FALSE;
    MMIVC_SMS_SEND_TYPE_E send_type = MMIVC_SMS_SEND_NORMAL;
    APP_SMS_USER_DATA_T ascii_vaild = {0};
    

    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_HandleSMSOrder sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3322_112_2_18_2_52_56_144,(uint8*)"");
        return FALSE;
    }

    MMISMS_InitSmsOrder();
    
    SCI_MEMSET(&sms_user_data_t, 0, sizeof (APP_SMS_USER_DATA_T));
    
    if (MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
    {
        switch (sig_ptr->status)
        {
            //sort the MT SMS
        case MN_SMS_MT_READED:                   
        case MN_SMS_MT_TO_BE_READ:

        send_type = MMISMS_OperateMtIndForVcardOrVcal(
        sig_ptr->item_sys, 
        &ascii_vaild,
        sig_ptr->storage,
        sig_ptr->record_id,
        sig_ptr->sms_t_u.mt_sms_t
        );

        if(MMIVC_SMS_SEND_NORMAL != send_type)
        {
            //SCI_TRACE_LOW:"MMISMS_HandleUserReadSMSCnf:send_type is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3348_112_2_18_2_52_56_145,(uint8*)"d",send_type);
            return FALSE;
        }
            HandleMtSMSOrder(
                sig_ptr->item_sys,
                sig_ptr->storage,
                sig_ptr->status,
                sig_ptr->record_id,
                &(sig_ptr->sms_t_u.mt_sms_t)
                );
            break;
            
        case MN_SMS_MT_SR_TO_BE_READ:
        case MN_SMS_MT_SR_READED:
            HandleSROrder(
                sig_ptr->item_sys,
                sig_ptr->storage,
                sig_ptr->status,
                sig_ptr->record_id,
                &(sig_ptr->sms_t_u.report_sms_t)
                );
            break;
            
        case MN_SMS_MO_TO_BE_SEND:
        case MN_SMS_MO_SR_NOT_REQUEST:          
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
            HandleMoSMSOrder(
                sig_ptr->item_sys,
                sig_ptr->storage,
                sig_ptr->status,
                sig_ptr->record_id,
                &(sig_ptr->sms_t_u.mo_sms_t)
                );
            break;
            
        default:
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_HandleSMSOrder sig_ptr->cause is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3391_112_2_18_2_52_57_146,(uint8*)"d", sig_ptr->cause);
        if( MN_SMS_STORAGE_SIM == sig_ptr->storage )
        {
            g_mmisms_global.err_sms_num[sig_ptr->item_sys]++; //simN�϶�ȡ�����sms������1
        }
        else
        {
            g_mmisms_global.me_err_sms_num ++; //�ֻ��϶�ȡ�����sms������1
        }
    }

    result = MultiSimReadSmsForOrder(sig_ptr);
    
    // set SMS state to idle screen
    if (result)
    {
        MMISMS_InitOtherMessageList();
        MAIN_SetMsgState(TRUE);
#ifdef MMI_MSG_RESEND
        MMISMS_ResendDelayed();
#endif
    }
    
    return (result);
}

/*****************************************************************************/
//     Description : handle MT SMS order
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleMtSMSOrder(
                            MN_DUAL_SYS_E       dual_sys, 
                            MN_SMS_STORAGE_E    storage,        //IN:
                            MN_SMS_STATUS_E     status,            //IN:
                            MN_SMS_RECORD_ID_T  record_id,        //IN:
                            MN_SMS_MT_SMS_T        *mt_sms_ptr        //IN:
                            )
{
    uint32 sc_time = 0;
    APP_SMS_USER_DATA_T sms_user_data_t = {0};
    MMISMS_STATE_T insert_order_info = {0};
    uint8 content [MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) + 1] = {0};
    uint8 content_len = 0;
    uint8 num_len = 0;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    BOOLEAN is_new_mms = FALSE;
    MMIVC_SMS_SEND_TYPE_E send_type = MMIVC_SMS_SEND_NORMAL;

    MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
//    uint16 content_r8 [MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) + 1] = {0};
    BOOLEAN is_lock_shift = FALSE;
    BOOLEAN is_single_shift = FALSE;    
     
    if (PNULL == mt_sms_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleMtSMSOrder mt_sms_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3444_112_2_18_2_52_57_147,(uint8*)"");
        return;
    }    
    MMISMS_DecodeUserHeadAndContent(
                                    dual_sys,
                                    mt_sms_ptr->user_head_is_exist,
                                    &(mt_sms_ptr->dcs),
                                    &(mt_sms_ptr->user_data_t),
                                    &sms_user_data_t
                                    );
    
    err_code = MNSMS_IsSupportMMSEx(
        dual_sys,
        sms_user_data_t.user_data_head_t.length,
        sms_user_data_t.user_data_head_t.user_header_arr,
        &is_new_mms
        );

    send_type = MMISMS_IsMtForVcard(dual_sys, &sms_user_data_t);

    MMISMS_CorrectTimeStamp(&mt_sms_ptr->time_stamp_t, MMISMS_MT_TYPE);
   
    // translate the time to second
    sc_time = MMIAPICOM_Tm2Second(mt_sms_ptr->time_stamp_t.second,
        mt_sms_ptr->time_stamp_t.minute,
        mt_sms_ptr->time_stamp_t.hour,
        mt_sms_ptr->time_stamp_t.day,
        mt_sms_ptr->time_stamp_t.month,
        mt_sms_ptr->time_stamp_t.year + MMISMS_MT_OFFSERT_YEAR
        );         
    
    insert_order_info.flag.is_ucs2 = (MN_SMS_UCS2_ALPHABET == mt_sms_ptr->dcs.alphabet_type) ? 1 : 0;
    insert_order_info.flag.dual_sys = dual_sys;
    insert_order_info.record_id = record_id;
    insert_order_info.time = sc_time;    
    insert_order_info.flag.storage   = storage;
    insert_order_info.flag.msg_type = MMISMS_TYPE_SMS;
    if(MN_SMS_STORAGE_ME == storage)
    {
        insert_order_info.flag.folder_type = g_mmisms_global.nv_param.folder_type[record_id];
    }
    else
    {
        insert_order_info.flag.folder_type = MMISMS_FOLDER_NORMAL;
    }    
    if (MN_SMS_STORAGE_ME == storage )
    {
        insert_order_info.flag.is_locked = MMISMS_NV_GetSmsIsLocked(record_id);    
    }
    else
    {
        insert_order_info.flag.is_locked = FALSE;
    }
//smsdecode to do         
    //add for display sms content

    MMISMS_DecodeUserDataHead(&sms_user_data_t.user_data_head_t, &user_head_ptr_t);
    language = MMISMS_JudgeR8Language(&user_head_ptr_t);
    MMISMS_DecodeR8Header(&user_head_ptr_t,&is_lock_shift,&is_single_shift);

    //SCI_TRACE_LOW:"MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3506_112_2_18_2_52_57_148,(uint8*)"ddd",language,is_lock_shift,is_single_shift);



    if(MN_SMS_UCS2_ALPHABET == mt_sms_ptr->dcs.alphabet_type)
    {
        content_len = MIN(MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar), sms_user_data_t.user_valid_data_t.length); 
    }
    else
    {
        content_len = MIN(MMIMESSAGE_DIAPLAY_LEN, sms_user_data_t.user_valid_data_t.length); 
    }
    
    MMISMS_ConvertValidDataToDispStr(insert_order_info.display_content.content,
                                     (uint16*)&insert_order_info.display_content.content_len,
                                     sms_user_data_t.user_valid_data_t.user_valid_data_arr,
                                     content_len,
                                     mt_sms_ptr->dcs.alphabet_type,
                                     language,
                                     is_lock_shift,
                                     is_single_shift
                                     );
    
    MMISMS_FreeUserDataHeadSpace(&user_head_ptr_t);

    content_len = 0;
    SCI_MEMSET(content, 0, sizeof(content));
    // get display content
   // num_len = MIN(mt_sms_ptr->origin_addr_t.num_len, (MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) - 2));
    num_len = MIN(mt_sms_ptr->origin_addr_t.num_len, MN_MAX_ADDR_BCD_LEN);//converity
    // the first two bytes are the type and plan
    content[0] = (uint8)mt_sms_ptr->origin_addr_t.number_type;
    content[1] = (uint8)mt_sms_ptr->origin_addr_t.number_plan;
    
    SCI_MEMCPY(
        &(content[2]),
        mt_sms_ptr->origin_addr_t.party_num,
        num_len
        );
    
    content_len = num_len + 2;
    
    insert_order_info.number.bcd_number.number_len= MIN((content_len-2), MMISMS_BCD_NUMBER_MAX_LEN);
    insert_order_info.number.bcd_number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8( (MN_NUMBER_TYPE_E)content[0], (MN_NUMBER_PLAN_E)content[1] );   
    SCI_MEMCPY(insert_order_info.number.bcd_number.number, &(content[2]), insert_order_info.number.bcd_number.number_len);

    //SCI_TRACE_LOW:"HandleMtSMSOrder:err_code=%d,is_new_mms=%d,send_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3568_112_2_18_2_52_57_150,(uint8*)"ddd", err_code, is_new_mms, send_type);

    if ((ERR_MNSMS_NONE == err_code) && (is_new_mms) && (MMIVC_SMS_SEND_NORMAL == send_type)) //vcard��ʱ����һ��sms��ȡ
    {
        insert_order_info.flag.mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
        MMISMS_InsertSmsToUnhandleOrder(&insert_order_info);
    }
    else
    {
        insert_order_info.flag.mo_mt_type = MMISMS_TranslateMNStatus2SFStatus(status, MMISMS_MT_TYPE, TRUE); 
        if (((sms_user_data_t.user_data_head_t.length != 0) && !MMISMS_IsR8Language(language)) ||
             ((sms_user_data_t.user_data_head_t.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) && MMISMS_IsR8Language(language)))
        {
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
            if (MMISMS_GetConcatenatedSMS())
#endif
            {
                insert_order_info.flag.is_concatenate_sms = TRUE;
            }
            if(MN_SMS_CONCAT_8_BIT_REF_NUM == sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_IEI_POS])
            {
                insert_order_info.longsms_max_num = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
                insert_order_info.longsms_seq_num = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS];
                insert_order_info.head_ref = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_REF_NUM_POS];
                //SCI_TRACE_LOW:"MMISMS: HandleMtSMSOrder: head_len = %d, ref_num = 0x%X, seq_num = 0x%X"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3595_112_2_18_2_52_57_151,(uint8*)"ddd", sms_user_data_t.user_data_head_t.length,sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_REF_NUM_POS],sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS]);
            }
            else if(MN_SMS_CONCAT_16_BIT_REF_NUM == sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_IEI_POS])
            {
                insert_order_info.longsms_max_num = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_16BIT_MAX_NUM_POS];
                insert_order_info.longsms_seq_num = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_16BIT_SEQ_NUM_POS];
                insert_order_info.head_ref = (sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_REF_NUM_POS] << 8) 
                                                        | (sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_REF_NUM_POS+1]);
                //SCI_TRACE_LOW:"MMISMS: HandleMtSMSOrder: head_len = %d, ref_num = 0x%X, seq_num = 0x%X"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3606_112_2_18_2_52_57_152,(uint8*)"ddd", sms_user_data_t.user_data_head_t.length,sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_REF_NUM_POS],sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_16BIT_SEQ_NUM_POS]);
            }
        }
        
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        if(MMISMS_FOLDER_BT == insert_order_info.flag.folder_type)
        {
            MMISMS_InsertBtSmsToOrder(&insert_order_info);
        }
        else
#endif
{
        MMISMS_InsertSmsToOrder(&insert_order_info);
}

    }    
}

/*****************************************************************************/
//     Description : handle MT SMS status report order
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleSROrder(
                         MN_DUAL_SYS_E              dual_sys,       //IN:
                         MN_SMS_STORAGE_E            storage,        //IN:
                         MN_SMS_STATUS_E            status,            //IN:
                         MN_SMS_RECORD_ID_T         record_id,        //IN:
                         MN_SMS_STATUS_REPORT_T     *mt_sr_ptr        //IN:
                         )
{
    uint32 sc_time = 0;
    MMISMS_STATE_T insert_order_info = {0};
    wchar content [MMIMESSAGE_DIAPLAY_LEN+1] = {0};
    uint8 content_len = 0;
    uint8 num_len = 0;
    MMI_STRING_T status_str = {0};
    uint8 number[MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) + 1] = {0};
    
    if (PNULL == mt_sr_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSROrder mt_sr_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3636_112_2_18_2_52_57_153,(uint8*)"");
        return;
    }

    MMISMS_CorrectTimeStamp(&mt_sr_ptr->tp_scts, MMISMS_MT_TYPE);
   
    // translate the time to second
    sc_time = MMIAPICOM_Tm2Second(mt_sr_ptr->tp_scts.second,
        mt_sr_ptr->tp_scts.minute,
        mt_sr_ptr->tp_scts.hour,
        mt_sr_ptr->tp_scts.day,
        mt_sr_ptr->tp_scts.month,
        mt_sr_ptr->tp_scts.year + MMISMS_MT_OFFSERT_YEAR
        );     

    insert_order_info.flag.dual_sys = dual_sys;
    insert_order_info.record_id = record_id;
    insert_order_info.time = sc_time;    
    insert_order_info.flag.storage   = storage;
    insert_order_info.flag.mo_mt_type = MMISMS_TranslateMNStatus2SFStatus(status, MMISMS_MT_TYPE, TRUE); 
    insert_order_info.flag.msg_type = MMISMS_TYPE_SMS;
    if (MN_SMS_STORAGE_ME == storage )
    {
        insert_order_info.flag.is_locked = MMISMS_NV_GetSmsIsLocked(record_id);    
    }
    else
    {
        insert_order_info.flag.is_locked = FALSE;
    }

    // prompt
    MMI_GetLabelTextByLang(
        TXT_SMS_STATUSREPORT, 
        &status_str
        );

    content_len = MIN(MMIMESSAGE_DIAPLAY_LEN, status_str.wstr_len);
    
    MMI_WSTRNCPY(content,
                            MMIMESSAGE_DIAPLAY_LEN,
                            status_str.wstr_ptr,
                            status_str.wstr_len,
                            content_len);

    insert_order_info.display_content.content_len = MIN(MMIMESSAGE_DIAPLAY_LEN, content_len);    
    MMI_WSTRNCPY(insert_order_info.display_content.content,
                                MMIMESSAGE_DIAPLAY_LEN,
                                content,
                                MMIMESSAGE_DIAPLAY_LEN,
                                insert_order_info.display_content.content_len);

    insert_order_info.flag.is_ucs2 = TRUE;

    // get display content
    //num_len = MIN(mt_sr_ptr->dest_addr_t.num_len, (MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) - 2));
    num_len = MIN(mt_sr_ptr->dest_addr_t.num_len, MN_MAX_ADDR_BCD_LEN);//converity
    
    // the first two bytes are the type and plan
    number[0] = (uint8)mt_sr_ptr->dest_addr_t.number_type;
    number[1] = (uint8)mt_sr_ptr->dest_addr_t.number_plan;
    
    SCI_MEMCPY(
        &(number[2]), 
        mt_sr_ptr->dest_addr_t.party_num, 
        num_len
        ); 
    
    content_len = num_len + 2;
    
    insert_order_info.number.bcd_number.number_len= MIN((content_len-2), MMISMS_BCD_NUMBER_MAX_LEN);
    insert_order_info.number.bcd_number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8( (MN_NUMBER_TYPE_E)number[0], (MN_NUMBER_PLAN_E)number[1] );   
    SCI_MEMCPY(insert_order_info.number.bcd_number.number, &(number[2]), insert_order_info.number.bcd_number.number_len);
    
    MMISMS_InsertSmsToOrder(&insert_order_info);
}

/*****************************************************************************/
//     Description : handle MO SMS order
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleMoSMSOrder(
                            MN_DUAL_SYS_E       dual_sys,       //IN:
                            MN_SMS_STORAGE_E    storage,        //IN:
                            MN_SMS_STATUS_E     status,            //IN:
                            MN_SMS_RECORD_ID_T  record_id,        //IN:
                            MN_SMS_MO_SMS_T        *mo_sms_ptr        //IN:
                            )
{
    BOOLEAN is_exist_addr = FALSE;
    BOOLEAN is_exist_sc_addr = FALSE;
    uint8 is_ucs2 = 0;
    uint8 content_len = 0;
    uint32 sc_time = 0;
    uint8 content[MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) + 1] = {0};   
    APP_SMS_USER_DATA_T sms_user_data_t = {0};    
    MMISMS_STATE_T order_info = {0};
    uint8 num_len = 0;

    MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
    //uint16 content_r8 [MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) + 1] = {0};
    BOOLEAN is_lock_shift = FALSE;
    BOOLEAN is_single_shift = FALSE;      
    
    if (PNULL == mo_sms_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleMoSMSOrder mo_sms_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3741_112_2_18_2_52_57_154,(uint8*)"");
        return;
    }
    SCI_MEMSET(&order_info, 0, sizeof(MMISMS_STATE_T));
        
    MMISMS_DecodeUserHeadAndContent(dual_sys,
        mo_sms_ptr->user_head_is_exist,
        &(mo_sms_ptr->dcs),
        &(mo_sms_ptr->user_data_t),
        &sms_user_data_t
        );
    
    if (MN_SMS_STORAGE_ME == storage)
    {
        MMISMS_CorrectTimeStamp(&mo_sms_ptr->time_stamp_t, MMISMS_MO_TYPE);

        // translate the time to second
        sc_time = MMIAPICOM_Tm2Second(mo_sms_ptr->time_stamp_t.second,
            mo_sms_ptr->time_stamp_t.minute,
            mo_sms_ptr->time_stamp_t.hour,
            mo_sms_ptr->time_stamp_t.day,
            mo_sms_ptr->time_stamp_t.month,
            (mo_sms_ptr->time_stamp_t.year) + MMISMS_OFFSERT_YEAR
            );
    }
    else
    {
        sc_time = MMISMS_GetCurTime();
    }
    
    //SCI_TRACE_LOW:"MMISMS: HandleMoSMSOrder head_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3771_112_2_18_2_52_57_155,(uint8*)"d", sms_user_data_t.user_data_head_t.length);
    
    is_ucs2 = (MN_SMS_UCS2_ALPHABET == mo_sms_ptr->dcs.alphabet_type) ? 1: 0;
    
       
    
    order_info.flag.is_ucs2 = is_ucs2;
    order_info.flag.storage = storage;
    order_info.record_id = record_id;    
    order_info.time = sc_time;
    order_info.flag.dual_sys = dual_sys;
    order_info.flag.msg_type = MMISMS_TYPE_SMS;

    if(MN_SMS_STORAGE_ME == storage)
    {
        order_info.flag.folder_type = g_mmisms_global.nv_param.folder_type[record_id];
    }
    else
    {
        order_info.flag.folder_type = MMISMS_FOLDER_NORMAL;
    }
    //SCI_TRACE_LOW:"HandleMoSMSOrder folder_type = %d,record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3792_112_2_18_2_52_57_156,(uint8*)"dd",order_info.flag.folder_type,record_id);
    if (MN_SMS_STORAGE_ME == storage )
    {
        order_info.flag.is_locked = MMISMS_NV_GetSmsIsLocked(record_id);    
    }
    else
    {
        order_info.flag.is_locked = FALSE;
    }
//smsdecode to do    
    MMISMS_DecodeUserDataHead(&sms_user_data_t.user_data_head_t, &user_head_ptr_t);
    language = MMISMS_JudgeR8Language(&user_head_ptr_t);
    MMISMS_DecodeR8Header(&user_head_ptr_t,&is_lock_shift,&is_single_shift);

    //SCI_TRACE_LOW:"MMISMSR8:InsertNewMtToOrder language is %d,is_lock_shift is %d,is_single_shift is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3807_112_2_18_2_52_57_157,(uint8*)"ddd",language,is_lock_shift,is_single_shift);

    if(MN_SMS_UCS2_ALPHABET == mo_sms_ptr->dcs.alphabet_type)
    {
        content_len = MIN(MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar), sms_user_data_t.user_valid_data_t.length); 
    }
    else
    {
        content_len = MIN(MMIMESSAGE_DIAPLAY_LEN, sms_user_data_t.user_valid_data_t.length); 
    }
    MMISMS_ConvertValidDataToDispStr((wchar *)order_info.display_content.content,
                                     (uint16*)&order_info.display_content.content_len,
                                     sms_user_data_t.user_valid_data_t.user_valid_data_arr,
                                     content_len,
                                     mo_sms_ptr->dcs.alphabet_type,
                                     language,
                                     is_lock_shift,
                                     is_single_shift
                                     );
    
    MMISMS_FreeUserDataHeadSpace(&user_head_ptr_t);

    is_exist_addr = ( mo_sms_ptr->dest_addr_t.num_len > 0 ) ? TRUE : FALSE;
    
    if (is_exist_addr)
    {
        SCI_MEMSET(content, 0, sizeof(content));
        content_len = 0;

        // get display content
        //num_len = MIN(mo_sms_ptr->dest_addr_t.num_len, (MMIMESSAGE_DIAPLAY_LEN * sizeof(wchar) - 2));
        num_len = MIN(mo_sms_ptr->dest_addr_t.num_len, MN_MAX_ADDR_BCD_LEN);//converity
        
        // the first two bytes are the type and plan
        content[0] = (uint8)mo_sms_ptr->dest_addr_t.number_type;
        content[1] = (uint8)mo_sms_ptr->dest_addr_t.number_plan;
        
        SCI_MEMCPY(
                    &(content[2]), 
                    mo_sms_ptr->dest_addr_t.party_num,
                    num_len);
        
        content_len = num_len + 2;
        
        order_info.number.bcd_number.number_len= MIN((content_len-2), MMISMS_BCD_NUMBER_MAX_LEN);
        
        order_info.number.bcd_number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8( (MN_NUMBER_TYPE_E)content[0], (MN_NUMBER_PLAN_E)content[1] );
        
        SCI_MEMCPY(order_info.number.bcd_number.number, &(content[2]), order_info.number.bcd_number.number_len);
    }
    
    if (((mo_sms_ptr->user_head_is_exist) && 
        (sms_user_data_t.user_data_head_t.length > 0) &&
        !MMISMS_IsR8Language(language)) ||
        ((mo_sms_ptr->user_head_is_exist) && 
        (sms_user_data_t.user_data_head_t.length > MMISMS_R8_MAX_NORMAL_HEAD_LEN) &&
        MMISMS_IsR8Language(language)))
    {
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
        if (MMISMS_GetConcatenatedSMS())
#endif
        {
            order_info.flag.is_concatenate_sms = TRUE;
        }
        order_info.longsms_max_num = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_MAX_NUM_POS];
        order_info.longsms_seq_num = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_8BIT_SEQ_NUM_POS];
        order_info.head_ref = sms_user_data_t.user_data_head_t.user_header_arr[MMISMS_HEAD_REF_NUM_POS];
    }
    
    if (mo_sms_ptr->sc_addr_present && mo_sms_ptr->sc_addr_t.num_len > 0)
    {
        is_exist_sc_addr = TRUE;
    }
    else
    {
        is_exist_sc_addr = FALSE;
    }

    //SCI_TRACE_LOW:"HandleMoSMSOrder sc_addr_present is %d,len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3903_112_2_18_2_52_58_159,(uint8*)"dd",mo_sms_ptr->sc_addr_present,mo_sms_ptr->sc_addr_t.num_len);
    
    // get the related sf_status
    order_info.flag.mo_mt_type = MMISMS_TranslateMNStatus2SFStatus(status, MMISMS_MO_TYPE, is_exist_sc_addr);
    
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if(MMISMS_FOLDER_BT == order_info.flag.folder_type)
    {
        MMISMS_InsertBtSmsToOrder(&order_info);
    }
    else
#endif
{
    MMISMS_InsertSmsToOrder(&order_info );
}
}

#if 0
/*****************************************************************************/
//     Description : to check whether the status  is to be read
//    Global resource dependence : none
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadPointedSMS(void)
{
    MMISMS_ORDER_ID_T cur_order_id = MMISMS_GetCurOperationOrderId(); 
    //׼��ȫ�ֱ���
    MMISMS_ClearOperInfo();
    MMISMS_ReadyReadSms();
    //��ʼ��ȡ
    MMISMS_AppReadSms(cur_order_id);    
}
#endif

/* ----------------------------------------------------------------------------
* Function Name: MMISMS_GetPBNameByNumber
* Purpose:ͨ���绰�õ��绰��������
* Input:  uint8 *number_ptr:�绰����ָ��
MMI_STRING_T *name_pt:�����ṹ
* Output: BOOLEAN.
* Returns:
* Author:
* ----------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMISMS_GetNameByNumberFromPB(uint8 *number_ptr, MMI_STRING_T *name_ptr, uint16 max_name_len)
{
    MMIPB_BCD_NUMBER_T bcd_num = {0};/*lint !e64*/
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    uint32 num_len = 0;
    uint8 real_phonenum[MMISMS_PBNUM_MAX_LEN + 1] = {0};
    BOOLEAN is_found = FALSE;    
    
    if (PNULL == name_ptr || PNULL == number_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetNameByNumberFromPB name_ptr number_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3943_112_2_18_2_52_58_160,(uint8*)"");
        return FALSE;
    }    
    SCI_MEMSET(&party_num, 0, sizeof(party_num));
    SCI_MEMSET(real_phonenum, 0, sizeof(real_phonenum));
    
    num_len = (SCI_STRLEN((char *)(number_ptr)) > MMISMS_PBNUM_MAX_LEN)? MMISMS_PBNUM_MAX_LEN : SCI_STRLEN((char *)(number_ptr));
    
    SCI_MEMCPY(real_phonenum, number_ptr, num_len);/* parasoft-suppress BD-RES-INVFREE "���жϼ��" */
    
    MMIAPICOM_GenPartyNumber(real_phonenum, strlen((char *)real_phonenum), &party_num);
    
    //��ʼ��MMIPB_BCD_NUMBER_T bcd_num
    SCI_MEMSET(&bcd_num, 0, sizeof(bcd_num));
    
    bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW );
    bcd_num.number_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
    
    MMI_MEMCPY(bcd_num.number, MMIPB_BCD_NUMBER_MAX_LEN, party_num.bcd_num, MN_MAX_ADDR_BCD_LEN, bcd_num.number_len);
    
    //���ҵ绰����¼
    is_found = MMIAPIPB_GetNameByNumber(&bcd_num, name_ptr, max_name_len, FALSE); //MMIAPIPB_FindAlphaAndGroupByNumber(name_ptr, PNULL, PNULL, &bcd_num, PNULL, FALSE);
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetNameByNumberFromPB is_found = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3966_112_2_18_2_52_58_161,(uint8*)"d",is_found);
    
    //special, the name is zero
    if (is_found && (name_ptr->wstr_len == 0))
    {
        is_found = FALSE;
    }
    
    return is_found;
}

/*****************************************************************************/
//     Description : to set the update info
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSMSUpdateInfo( 
                                    MMISMS_ORDER_ID_T    order_id
                                    )
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    int32 i = 0;
    
    SCI_MEMSET(((void*)&g_mmisms_global.update_info),
        0,
        sizeof(MMISMS_UPDATE_INFO_T));
    cur_order_id = order_id;
    if (order_id->flag.is_concatenate_sms) //is long sms
    {
        while (cur_order_id != PNULL)
        {
            if (i >=  MMISMS_SPLIT_MAX_NUM)
            {
                //SCI_TRACE_LOW:"MMISMS:MMISMS_SetSMSUpdateInfo i >=  MMISMS_SPLIT_MAX_NUM"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_3998_112_2_18_2_52_58_162,(uint8*)"");
                break;
            }
            g_mmisms_global.update_info.order_id_arr[i] = cur_order_id;
            i ++;
            cur_order_id = cur_order_id->next_long_sms_ptr;
        }
    }
    else
    {
        g_mmisms_global.update_info.order_id_arr[0] = order_id;
    }    
}

/*****************************************************************************/
//     Description : get contact information from phone_book
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
PUBLIC void MMISMS_GetContactInfo(
                                  uint32         handle,
                                  MMI_STRING_T    *contact_info_ptr,    //OUT:
                                  uint16        max_len                //IN:
                                  )
{
    MMI_TEXT_ID_T name_label = TXT_NULL;
    MMI_TEXT_ID_T number_label = TXT_NULL;
    MMI_STRING_T name_str     = {0};
    MMI_STRING_T label_str    = {0};
    MMIPB_BCD_NUMBER_T bcd_number   = {0};
    uint8 tele_len     = 0;
    uint8 tele_num[MMISMS_NUMBER_MAX_LEN + 2] = {0};
    BOOLEAN is_selected = FALSE;
    MMI_STRING_T email_str  = {0};
    MMI_TEXT_ID_T email_label = TXT_NULL;
    
    if (PNULL == contact_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetContactInfo contact_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4035_112_2_18_2_52_58_163,(uint8*)"");
        return;
    }
    
    // allocate memory 
    name_str.wstr_ptr = SCI_ALLOCAZ((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
    if (PNULL == name_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetContactInfo name_str.wstr_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4043_112_2_18_2_52_58_164,(uint8*)"");
        return;
    }
    email_str.wstr_ptr = SCI_ALLOCAZ((MMISMS_PBMAIL_MAX_LEN+1)*sizeof(wchar));
    if (PNULL == email_str.wstr_ptr)
    {
        SCI_FREE(name_str.wstr_ptr);
        name_str.wstr_ptr = PNULL;
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetContactInfo email_str.wstr_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4051_112_2_18_2_52_58_165,(uint8*)"");
        return;
    }
    
    // get the detail information from the phone_book
    //if (MMIAPIPB_GetDetailInSelectedList(&name_str, &bcd_number, PNULL, 0, &name_label, &number_label))
    is_selected = MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)handle,&bcd_number,MMIPB_BCD_NUMBER_MAX_LEN, 0);
    is_selected = is_selected | MMIAPIPB_GetSelectName((MMIPB_HANDLE_T)handle,&name_str, MMISMS_PBNAME_MAX_LEN, 0);
    is_selected = is_selected | MMIAPIPB_GetSelectEmail((MMIPB_HANDLE_T)handle,&email_str, MMISMS_PBMAIL_MAX_LEN, 0);
    if( is_selected)
    {
        // get the name label
	 name_label = TXT_NAME;
        number_label = TXT_PB_ITEM_DETAIL_MOBILE_PHONE;
        email_label = TXT_PB_ITEM_DETAIL_EMAIL;
        MMI_GetLabelTextByLang(name_label, &label_str);
        
        // the alphabet which the whole string mapped is depended on the label's and the name's
        contact_info_ptr->wstr_len  = 0;
        
        // copy the label and string of the name to the string
        AppendString2Contact(
            contact_info_ptr,
            TRUE,
            label_str.wstr_len*sizeof(wchar),
            (uint8 *)label_str.wstr_ptr,
            max_len
            );
        AppendString2Contact(
            contact_info_ptr,
            FALSE, 
            1,
            (uint8 *)(":"),
            max_len
            );
        
        AppendString2Contact(
            contact_info_ptr,
            TRUE,
            name_str.wstr_len*sizeof(wchar),
            (uint8 *)name_str.wstr_ptr,
            max_len
            );
        AppendString2Contact(
            contact_info_ptr,
            FALSE, 
            1,
            (uint8 *)(" "),
            max_len
            );
        
        // get the number label
        SCI_MEMSET(&label_str,0x0, sizeof(MMI_STRING_T));        
        MMI_GetLabelTextByLang(number_label, &label_str);

        if(label_str.wstr_len != 0)
        {
            // copy the label to string
            AppendString2Contact(
                contact_info_ptr,
                TRUE,
                label_str.wstr_len*sizeof(wchar),
                (uint8 *)label_str.wstr_ptr,
                max_len
                );
            AppendString2Contact(
                contact_info_ptr,
                FALSE, 
                1,
                (uint8 *)(":"),
                max_len
                );
            
            // convert number to string
            tele_len = MMIAPICOM_GenDispNumber( 
                MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton), 
                MIN(MMISMS_NUMBER_MAX_BCD_LEN, bcd_number.number_len), 
                bcd_number.number,  
                tele_num,
                MMISMS_NUMBER_MAX_LEN + 2
                );
            // copy the number to the string
            AppendString2Contact(
                contact_info_ptr,
                FALSE,
                tele_len,
                tele_num,
                max_len
                );
            AppendString2Contact(
                contact_info_ptr,
                FALSE, 
                1,
                (uint8 *)(" "),
                max_len
                );
        }

        else
        {
            //get email label
            MMI_GetLabelTextByLang(email_label, &label_str);

            // copy the label to string
            AppendString2Contact(
                contact_info_ptr,
                TRUE,
                label_str.wstr_len*sizeof(wchar),
                (uint8 *)label_str.wstr_ptr,
                max_len
                );
            AppendString2Contact(
                contact_info_ptr,
                FALSE, 
                1,
                (uint8 *)(":"),
                max_len
                );

            AppendString2Contact(
                contact_info_ptr,
                TRUE,
                email_str.wstr_len*sizeof(wchar),
                (uint8 *)email_str.wstr_ptr,
                max_len
                );
            AppendString2Contact(
                contact_info_ptr,
                FALSE, 
                1,
                (uint8 *)(" "),
                max_len
                );
        }
    }
    
    SCI_FREE(name_str.wstr_ptr);
    name_str.wstr_ptr = PNULL;    
    SCI_FREE(email_str.wstr_ptr);
    email_str.wstr_ptr = PNULL;
}

/*****************************************************************************/
//     Description : append string to contact information string
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
LOCAL void AppendString2Contact(
                                MMI_STRING_T    *conv_str_ptr,    //IN/OUT:
                                BOOLEAN            is_ucs2,        //IN:
                                uint16            length,            //IN:
                                uint8            *org_str_ptr,    //IN:
                                uint16            max_len            //IN:
                                )
{
    uint16 copy_len = 0;
    
    if (PNULL == conv_str_ptr || PNULL == org_str_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AppendString2Contact conv_str_ptr org_str_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4209_112_2_18_2_52_58_166,(uint8*)"");
        return;
    }
    
    if (!is_ucs2)
    {
        // copy to the end of contact information string
        copy_len = MIN(max_len-conv_str_ptr->wstr_len, length);

        MMI_STRNTOWSTR(&(conv_str_ptr->wstr_ptr[conv_str_ptr->wstr_len]),
                                    copy_len,
                                    (uint8 *)org_str_ptr,
                                    copy_len,
                                    copy_len);
    }
    else
    {
        copy_len = MIN(max_len-conv_str_ptr->wstr_len, (int32)(length/sizeof(wchar)));

        /*
        MMI_WSTRNCPY(&(conv_str_ptr->wstr_ptr[conv_str_ptr->wstr_len]),
                                copy_len,
                                (wchar *)org_str_ptr,
                                copy_len,
                                copy_len);
        */
        MMI_MEMCPY((uint8 *)&(conv_str_ptr->wstr_ptr[conv_str_ptr->wstr_len]),
                          copy_len*sizeof(wchar),
                          org_str_ptr,
                          copy_len*sizeof(wchar),
                          copy_len*sizeof(wchar));
    }

    conv_str_ptr->wstr_len += copy_len;
}

/*****************************************************************************/
//     Description : to get the user number form SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: num_arr_ptr[arr_size - 1][max_char_num + 1]
/*****************************************************************************/
PUBLIC uint8  MMISMS_GetSMSUseNumber(                                //RETURN: 
                                     uint8          *num_arr_ptr,    //OUT:
                                     uint8          arr_size,        //IN:
                                     uint8          max_char_num    //IN: must < MMISMS_MAX_USERNUMBER_LEN
                                     )
{
    uint8 addr[ MMISMS_MAX_USERNUMBER_LEN + 2 ] = {0};
    uint8 addr_num  = 0;
    uint8 num_len   = 0;
    uint8 step_len  = 0;
    BOOLEAN is_ucs2 = FALSE;
    int16 i = 0;
    uint16 offset = 0;
    MMISMS_READ_MSG_T *read_msg_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    if (PNULL == num_arr_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSMSUseNumber num_arr_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4267_112_2_18_2_52_58_167,(uint8*)"");
        return 0;
    }
    
    read_msg_ptr = &(g_mmisms_global.read_msg);
    
    // first get the address string
    num_len = MMISMS_GetOriginNum(
        &dual_sys,
        addr,
        MMISMS_MAX_USERNUMBER_LEN + 2
        );
    
    // set address string to array
    if (num_len >= MMISMS_MIN_USERNUMBER_LEN)
    {
        num_len = MIN(num_len, max_char_num);
        SCI_MEMCPY( 
            num_arr_ptr, 
            addr, 
            num_len 
            );
        addr_num++;
        offset = addr_num * (MMISMS_MAX_USERNUMBER_LEN + 1);
        if ( addr_num == arr_size )
        {
            return addr_num;
        }           
    }
    
    num_len = 0;
    
    //whether the message is ucs2
    is_ucs2 = ( MN_SMS_UCS2_ALPHABET == read_msg_ptr->read_content.alphabet ) ? TRUE : FALSE;
    
    // the step len according by the alphabet of the message
    step_len = ( is_ucs2 ) ? 1 : 0 ;
    
    for (i=0; i<read_msg_ptr->read_content.length; i=(i+step_len+1))
    {
        // check  whether the data is char of number
        if ( ( !is_ucs2 || ( read_msg_ptr->read_content.data[i] == 0x00 ) ) 
            && ( ((read_msg_ptr->read_content.data[i+step_len] >= MMISMS_NUMBER_0) && (read_msg_ptr->read_content.data[i+step_len] <= MMISMS_NUMBER_9))
                    ||((read_msg_ptr->read_content.data[i+step_len]== MMISMS_NUMBER_PLUS)&&((read_msg_ptr->read_content.data[i+step_len+1+step_len] >= MMISMS_NUMBER_0) && (read_msg_ptr->read_content.data[i+step_len+1+step_len] <= MMISMS_NUMBER_9)))))
        {
            // save the char which fulfill the conditions
            *(num_arr_ptr+offset+num_len) = read_msg_ptr->read_content.data[i+step_len];
            num_len ++;
            
            // whether the length has arrvered the limited condition
            if ( num_len == max_char_num )
            {
                addr_num++;
                offset  = addr_num * ( MMISMS_MAX_USERNUMBER_LEN + 1 );
                num_len = 0;
            }
        }
        else
        {
            // whether the length has arrvered the limited condition
            if (num_len >= MMISMS_MIN_USERNUMBER_LEN)
            {
                addr_num++;
                offset  = addr_num * ( MMISMS_MAX_USERNUMBER_LEN + 1 );
            }
            
            num_len = 0;
        }
        
        // whether the length has arrvered the limited condition
        if ( addr_num == arr_size )
        {
            return addr_num;
        }         
    }
    
    // maybe the number in the last part of the SMS content
    // whether the length has arrived the limited condition
    if ( num_len >= MMISMS_MIN_USERNUMBER_LEN )
    {
        addr_num++;
    }        
    
    return (addr_num);
}

/*****************************************************************************/
//     Description : to get the origin time and date form MT SMS
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetOriginTimeAndDate(
                                        wchar    *date_ptr,        //OUT:
                                        wchar    *time_ptr        //OUT:
                                        )
{
    MN_SMS_TIME_STAMP_T    time_stamp  = (g_mmisms_global.read_msg.time_stamp);
    MMI_TM_T mmi_time_stamp = {0};
    if (PNULL == date_ptr || PNULL == time_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetOriginTimeAndDate date_ptr time_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4390_112_2_18_2_52_59_168,(uint8*)"");
        return ;
    }
    
    mmi_time_stamp.tm_mday = time_stamp.day;
    mmi_time_stamp.tm_hour = time_stamp.hour;
    mmi_time_stamp.tm_min = time_stamp.minute;
    mmi_time_stamp.tm_mon = time_stamp.month;
    mmi_time_stamp.tm_sec = time_stamp.second;

    if ((MMISMS_BOX_SENDSUCC == MMISMS_GetCurOperationType())
        || (MMISMS_BOX_SENDFAIL == MMISMS_GetCurOperationType())
        || (MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        || (MMISMS_BT_BOX_SENDSUCC == MMISMS_GetCurOperationType())
        || (MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurOperationType())
#endif
	)
    {
        mmi_time_stamp.tm_year =  time_stamp.year + MMICOM_DATE_MIN_YEAR;
    }
    else
    {
        mmi_time_stamp.tm_year =  time_stamp.year + MMISMS_MT_OFFSERT_YEAR;
    }

    MMISMS_GetDateTimeStringByStamp(&mmi_time_stamp,TRUE,TRUE,FALSE,date_ptr);    
    MMISMS_GetDateTimeStringByStamp(&mmi_time_stamp,FALSE,FALSE,TRUE,time_ptr);
}


/*****************************************************************************/
//     Description : to get number of the error sms
//    Global resource dependence : g_mmisms_global
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint8  MMISMS_GetReadErrSmsNum(MN_DUAL_SYS_E dual_sys, BOOLEAN is_nv)
{
    if(is_nv)
    {
        return g_mmisms_global.me_err_sms_num;
    }
    else
    {
        return g_mmisms_global.err_sms_num[dual_sys];
    }
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : Start Check if MMS/OTA/WAP is ready timer
//  Global resource dependence :
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
LOCAL void StartSmsSortTimer(void)
{
    if (0 == s_mmisms_sort_timer_id)
    {
        s_mmisms_sort_timer_id = MMK_CreateTimerCallback(MMISMS_SORT_TIME, MMIAPISMS_HandleSortTimer, NULL, TRUE);
    }
    else
    {
        //SCI_TRACE_LOW:"StartSmsSortTimer: the timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4432_112_2_18_2_52_59_169,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : Get the sms sort timer Id.
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetSmsSortTimerId(void)
{
    return s_mmisms_sort_timer_id;
}

/*****************************************************************************/
//  Description :Insert the mms/wap/ota into sms list. 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSortTimer(uint8 timer_id, uint32 param)
{
//ͬ����MMS LIST��������̲���Ҫ�ˡ�

//    BOOLEAN ret = FALSE;
//    if (!MMISMS_IsMMSOrderReady()) //������ų�ʼ���Ѿ���ɣ��򽫲��Ų��뵽order��ȥ
//    {
//        ret = MMIAPIMMS_ListInit(FALSE); //MMS ��������ʼ,��mms��Ϣ���뵽order list��      
//    }
//    if (ret)
//    {
//        StopSmsSortTimer(); //ȫ��������Ϻ�ر�timer.
//        MMISMS_ReadMMSPushFromUnhandledList(); //��ȡmms push ��Ϣ������mmsģ��
//        MMISMS_CloseWaitingWin();
//        MAIN_SetMsgState(TRUE);
//    }
}

#endif /* Reduce Code size */

#if 0 
// no use
/*****************************************************************************/
//  Description : stop Check if MMS/OTA/WAP is ready  timer
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
LOCAL void StopSmsSortTimer(void)
{
    if (0 < s_mmisms_sort_timer_id)
    {
        MMK_StopTimer(s_mmisms_sort_timer_id);
        s_mmisms_sort_timer_id = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"StopSmsSortTimer: the timer has stop!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_4482_112_2_18_2_52_59_170,(uint8*)"");
    }
}
#endif

/*****************************************************************************/
//     Description : decode and modify sms data
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_DecodeUserHeadAndContent(
                                     MN_DUAL_SYS_E           dual_sys,           //IN:
                                     BOOLEAN                 user_head_is_exist, //IN:
                                     MN_SMS_DCS_T            *dcs_ptr_t,         //IN:
                                     MN_SMS_USER_DATA_T      *user_data_ptr_t,     //IN:
                                     APP_SMS_USER_DATA_T     *decode_data_ptr     //OUT:
                                     )
{
    uint8 temp_valid_data[MN_SMS_MAX_USER_VALID_DATA_LENGTH] = {0};
    BOOLEAN is_long_sms_16bit_ref = FALSE;
    BOOLEAN is_long_sms_8bit_ref = FALSE;
    BOOLEAN is_vcard_ref = FALSE;
    BOOLEAN is_java_ref = FALSE;
    BOOLEAN is_mms_ref  = FALSE;
#ifdef MOBILE_VIDEO_SUPPORT	
    BOOLEAN is_mv_sms_16bit_ref = FALSE;
#endif

    if(dual_sys >= MN_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("mmisms decode dual_sys error, dual_sys = %d", dual_sys);
        return;
    }
        
    MNSMS_DecodeUserDataEx(
        dual_sys,
        user_head_is_exist,
        dcs_ptr_t,
        user_data_ptr_t,
        &(decode_data_ptr->user_data_head_t),
        &(decode_data_ptr->user_valid_data_t)
        );
    
    is_long_sms_16bit_ref = (decode_data_ptr->user_valid_data_t.length >= decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1)                    // ��СΪ��Ϣͷ���ֵĳ���
        && ((0x06 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[0])    //header length
            && (MN_SMS_CONCAT_16_BIT_REF_NUM == decode_data_ptr->user_valid_data_t.user_valid_data_arr[1])    // ��ʾΪ16bit reference number
            && (0x04 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[2]));
    
#ifdef MOBILE_VIDEO_SUPPORT	
    is_mv_sms_16bit_ref = (decode_data_ptr->user_valid_data_t.length >= decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1)                    // ��СΪ��Ϣͷ���ֵĳ���
        && ((0x06 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[0])    //header length
            && (MN_SMS_APP_PORT_16_BIT_ADDR == decode_data_ptr->user_valid_data_t.user_valid_data_arr[1])    // ��ʾΪ16bit reference number
            && (0x04 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[2]));
#endif

    is_long_sms_8bit_ref = (decode_data_ptr->user_valid_data_t.length >= decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1)                    // ��СΪ��Ϣͷ���ֵĳ���
        && ((0x05 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[0])     //header length
            && (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_data_ptr->user_valid_data_t.user_valid_data_arr[1])    // ��ʾΪ8bit reference number
            && (0x03 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[2]));

    is_vcard_ref = (decode_data_ptr->user_valid_data_t.length >= decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1)                    // ��СΪ��Ϣͷ���ֵĳ���
        && ((MMISMS_HEAD_IEI_FOR_PORT == decode_data_ptr->user_valid_data_t.user_valid_data_arr[1])
            && (MMISMS_HEAD_IEL_FOR_PORT == decode_data_ptr->user_valid_data_t.user_valid_data_arr[2])
            && (MMISMS_VCARD_PORT_NUM == ((decode_data_ptr->user_valid_data_t.user_valid_data_arr[3] << 8) | (decode_data_ptr->user_valid_data_t.user_valid_data_arr[4])))
            && (0x00 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[5])
            && (0x00 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[6]));

    is_java_ref = (decode_data_ptr->user_valid_data_t.length >= decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1)                    // ��СΪ��Ϣͷ���ֵĳ���
        && (((6 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[0])
                    &&(MN_SMS_APP_PORT_16_BIT_ADDR == decode_data_ptr->user_valid_data_t.user_valid_data_arr[1]))//16Bit Address
                || 
                ((4 == decode_data_ptr->user_valid_data_t.user_valid_data_arr[0])
                    &&(MN_SMS_APP_PORT_8_BIT_ADDR == decode_data_ptr->user_valid_data_t.user_valid_data_arr[1])));//8Bit Address
    is_mms_ref = (decode_data_ptr->user_valid_data_t.length >= decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1)                    // ��СΪ��Ϣͷ���ֵĳ���
        && (((0x0B == decode_data_ptr->user_valid_data_t.user_valid_data_arr[0])
                    &&(MN_SMS_APP_PORT_16_BIT_ADDR == decode_data_ptr->user_valid_data_t.user_valid_data_arr[1])));//16Bit Address
    //������������   
    // �����ǳ�����Ϣ������û��������Ӧ�ı�־λ��������һ�����Ϣ��
    if (!user_head_is_exist 
        && (is_long_sms_16bit_ref 
        || is_long_sms_8bit_ref 
        || is_vcard_ref 
        || is_java_ref
        || is_mms_ref
#ifdef MOBILE_VIDEO_SUPPORT			
        || is_mv_sms_16bit_ref
#endif
                ))
    {
        MN_SMS_USER_VALID_DATA_T    temp_data = {0};

        //save header
        SCI_MEMCPY(
                                &decode_data_ptr->user_data_head_t,
                                &decode_data_ptr->user_valid_data_t.user_valid_data_arr[0],
                                (decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1)
                                );
        
        temp_data.length = decode_data_ptr->user_valid_data_t.length - (decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1);
        SCI_MEMCPY(
            temp_data.user_valid_data_arr,
            &(decode_data_ptr->user_valid_data_t.user_valid_data_arr[decode_data_ptr->user_valid_data_t.user_valid_data_arr[0] + 1]),
            temp_data.length
            );
        
        decode_data_ptr->user_valid_data_t = temp_data;
    }
    
    //if is UCS2 code,and valid data length is odd, delete the first byte data
    if (MN_SMS_UCS2_ALPHABET == dcs_ptr_t->alphabet_type 
        && (0 != decode_data_ptr->user_valid_data_t.length % MMISMS_VALID_DATA_LEN_DIV))
    {
        //Store the valid data
        SCI_MEMCPY(temp_valid_data,decode_data_ptr->user_valid_data_t.user_valid_data_arr,decode_data_ptr->user_valid_data_t.length);
        decode_data_ptr->user_valid_data_t.length--;
        
        //re- store the valid data.delete the first byte or last byte.
        SCI_MEMSET(decode_data_ptr->user_valid_data_t.user_valid_data_arr, 0,sizeof(decode_data_ptr->user_valid_data_t.user_valid_data_arr));
        SCI_MEMCPY(decode_data_ptr->user_valid_data_t.user_valid_data_arr,temp_valid_data,decode_data_ptr->user_valid_data_t.length);
    }
    
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : to read the SMS chat box
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ChatSetMTGlobalReadMsg(
                                          MN_DUAL_SYS_E           dual_sys,
                                          APP_SMS_USER_DATA_T        *user_data_ptr,    //IN:
                                          MN_SMS_STORAGE_E        storage,        //IN:
                                          MN_SMS_RECORD_ID_T        record_id,        //IN:
                                          MN_SMS_ALPHABET_TYPE_E    alphabet_type,    //IN:
                                          MN_SMS_TIME_STAMP_T        *time_stamp_ptr,//IN:
                                          MN_CALLED_NUMBER_T        *org_addr_ptr,    //IN:
                                          MMISMS_MO_MT_TYPE_E mo_mt_type
                                         )
{


    SCI_MEMSET(&g_mmisms_global.read_msg,0x0,sizeof(MMISMS_READ_MSG_T));
    g_mmisms_global.read_msg.mo_mt_type = mo_mt_type;                
    SCI_MEMCPY(&g_mmisms_global.read_msg.ascii_vaild,user_data_ptr,sizeof(APP_SMS_USER_DATA_T));
    
    // add the readed SMS to global
    MMISMS_SaveReadedSmsToGlobal(
        dual_sys,
        MMISMS_READ_MTSMS,
        user_data_ptr,  
        org_addr_ptr,
        alphabet_type
        );
           
    g_mmisms_global.read_msg.dual_sys = dual_sys;
    
    SCI_MEMCPY(
               &g_mmisms_global.read_msg.time_stamp,
               time_stamp_ptr,
               sizeof(MN_SMS_TIME_STAMP_T)
              );

    return;
}

/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CalcChatItemSplitLenth(uint32  index)
{
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;
    uint16 i = 0;
    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    chat_item = &g_mmisms_global.chat_item[index];

    if(g_mmisms_global.operation.cur_order_id_arr[0] != PNULL)
	{
        max_num = g_mmisms_global.operation.cur_order_id_arr[0]->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
	}
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;
    
    while( i < MMISMS_SPLIT_MAX_NUM && g_mmisms_global.operation.cur_order_id_arr[i] != PNULL)
    {
        seq_num = g_mmisms_global.operation.cur_order_id_arr[i]->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
        seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num;         
        index = seq_num - 1;
        chat_item->split_sms_length[index] = g_mmisms_global.read_msg.read_content.split_sms_length[index];
        chat_item->split_sms_enable[i] = TRUE;
        i++;    
    }
    for(i = 0; i < max_num; i++)
    {   
        if(0 == chat_item->split_sms_length[i])
        {
            chat_item->split_sms_length[i] = sms_lost_promt.wstr_len; 
            chat_item->split_sms_enable[i] = FALSE;
        }        
    }    
}

/*****************************************************************************/
//  Description : �����Ķ�������һ����ͬ��������
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetChatLongSmsItemContent(
                                               uint32  index,
                                               MMISMS_ORDER_ID_T order_id,
                                               MMI_STRING_T orig_content,
                                               MMI_STRING_T new_insert_content,
                                               MMI_STRING_T *string_ptr //out
                                               )
{
    int32 i = 0;
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    uint16 pos = 0;
    uint16 prompt_len = 0;

    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;

    uint16 split_pos_before_len = 0;
    uint16 split_pos_after_len = 0;
    uint16 split_pos_after_pos = 0;
    
    if (PNULL == order_id)
    {
        return FALSE;
    }
     
    chat_item = &g_mmisms_global.chat_item[index];

    //SCI_TRACE_LOW:"MMISMS_SetChatLongSmsItemContet:seq_len is %d,ref_num is %d,max is %d,seq is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_5245_112_2_18_2_53_0_177,(uint8*)"dddd",chat_item->split_sms_length[order_id->longsms_seq_num],order_id->longsms_max_num,order_id->longsms_seq_num);
    max_num = order_id->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;
    seq_num = order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
    seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num; 
            
    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    SCI_TRACE_LOW("MMISMS_SetChatLongSmsItemContent:orig_content.wstr_len is %d",orig_content.wstr_len);
    SCI_TRACE_LOW("MMISMS_SetChatLongSmsItemContent:split[0] is %d,split[1] is %d,split[2] is %d,split[3] is %d",chat_item->split_sms_length[0],chat_item->split_sms_length[1],chat_item->split_sms_length[2],chat_item->split_sms_length[3]);
    if(orig_content.wstr_len == 0)  //��ǰ�����ŵ�һ���������Ϣ
    {                    
        for(i = 0; i < max_num; i++)
        {
            if(i + 1 == seq_num)
            {               
                MMIAPICOM_Wstrncpy(
                &string_ptr->wstr_ptr[pos],
                new_insert_content.wstr_ptr,
                new_insert_content.wstr_len
                );
                pos += new_insert_content.wstr_len;                         
                chat_item->split_sms_length[i] = new_insert_content.wstr_len;
                chat_item->split_sms_enable[i] = TRUE;
            }
            else
            {
                if (MN_SMS_UCS2_ALPHABET != g_mmisms_global.read_msg.read_content.alphabet
                && MMIAPICOM_IsASCIIString(sms_lost_promt.wstr_ptr, sms_lost_promt.wstr_len))//ascii
                {
                    uint8 temp_prompt_str[MMISMS_PROMPT_STR_LEN + 1] = {0};
                    uint16 default_len = 0;
                    
                    sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LEN);
                    
                    MMI_WSTRNTOSTR(temp_prompt_str, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_ptr, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_len);
                    
                    prompt_len = sms_lost_promt.wstr_len;
                    
                    default_len = MMIAPICOM_Default2Wchar(temp_prompt_str, &string_ptr->wstr_ptr[pos], sms_lost_promt.wstr_len);
                }
                else
                {
                    MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_ptr,
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_len);                
                }
                pos += sms_lost_promt.wstr_len;  
                chat_item->split_sms_length[i] = sms_lost_promt.wstr_len;
                chat_item->split_sms_enable[i] = FALSE;
            }
        } 
    }
    else
    {
        for(i = 0; i < seq_num - 1; i++)
        {
            split_pos_before_len += chat_item->split_sms_length[i];  
        }  
    
        split_pos_after_pos = split_pos_before_len + sms_lost_promt.wstr_len;            
        
        for(i = seq_num; i < max_num; i++)
        {
            split_pos_after_len += chat_item->split_sms_length[i];  
        } 
        
        if(split_pos_before_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            split_pos_before_len,
            orig_content.wstr_ptr,
            split_pos_before_len,
            split_pos_before_len);
            pos += split_pos_before_len;
        }
        
        MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
        new_insert_content.wstr_len,
        new_insert_content.wstr_ptr,
        new_insert_content.wstr_len,
        new_insert_content.wstr_len);
        pos += new_insert_content.wstr_len;
        chat_item->split_sms_length[seq_num - 1] = new_insert_content.wstr_len;
        
        if(split_pos_after_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            orig_content.wstr_len,
            orig_content.wstr_ptr + split_pos_after_pos,
            split_pos_after_len,
            split_pos_after_len);
            pos += split_pos_after_len;        
        }
    }
    
    string_ptr->wstr_len = pos;
    return TRUE;
}
#endif

/*****************************************************************************/
//     Description : handle open search msg in SMS list
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: include reading finished and etc
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenMatchedItem(uint32 user_data)
{
    MMISMS_ORDER_ID_T cur_order_id = (MMISMS_ORDER_ID_T)user_data;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    uint32 msg_record_id = 0;
    if(PNULL == cur_order_id)
	{
		return;
	}
    box_type = MMISMS_GetSmsStateBoxtype(cur_order_id);
    
    //to get the info of current selected item
    MMISMS_SetCurSearchOperationOrderId(box_type, cur_order_id);
    msg_record_id = cur_order_id->record_id;
    switch(MMISMS_GetMsgType())
    {
    case MMISMS_TYPE_SMS:
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
            break;
        }
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
        MMISMS_ReadChatMsgByOrderId(cur_order_id);        
#endif

        break;
        
    case MMISMS_TYPE_MMS:
		//���cr MS00150274,U��ʹ�ù����в��ܿ�MMS
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
        if (MMIAPIBROWSER_IsRunning())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MMS_WEB_IN_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#endif
#endif
        MMISMS_ClearOperInfo();

        switch(box_type) 
        {                
        case MMISMS_BOX_MT:
        case MMISMS_BOX_SENDSUCC:
        case MMISMS_BOX_SENDFAIL:
            //if (order_id != PNULL)
            {
                MMIAPIMMS_ReadMMS((uint32)msg_record_id);
            }                    
            //MMIAPIMMS_OpMMS((uint32)order_id->record_id, MMIMMS_OP_PREVIEW, win_id);
            break;
        case MMISMS_BOX_NOSEND:
            //MMIAPIMMS_OpMMS((uint32)order_id->record_id, MMIMMS_OP_EDIT, win_id);
            //if (order_id != PNULL)
            {
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT);
                MMIAPIMMS_DraftEditMMS((uint32)msg_record_id);
            }                    
            break;
        default:
            break;
        }
        break;
        
    case MMISMS_TYPE_WAP_PUSH:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            MMISMS_ClearOperInfo();         
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_WapPushEnter((uint16)msg_record_id);
#endif
        }
        break;
        
    case MMISMS_TYPE_WAP_OTA:
    case MMISMS_TYPE_MMS_OTA:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {              
            MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_OTAEnter((uint16)msg_record_id);
#endif
        }
        break;

    default: 
        break;
    }
}

#ifdef MMI_MSG_RESEND
/*****************************************************************************/
//  Description : 
//  Global resource dependence : g_mmisms_global
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void Handle_ReadResendSms(DPARAM param)
{
    APP_MN_READ_SMS_CNF_T *sig_ptr = (APP_MN_READ_SMS_CNF_T *)param;
//    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MMISMS_ORDER_ID_T next_order_id = PNULL  ;
    uint8 num_len = 0;
    APP_SMS_USER_DATA_T ascii_vaild = {0};
    MMI_STRING_T msg_content = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 number [MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;

    if (PNULL == param)
    {
        return;
    }

    SCI_TRACE_LOW("Handle_ReadResendSms");

    MMISMS_DecodeUserHeadAndContent(sig_ptr->item_sys, sig_ptr->sms_t_u.mo_sms_t.user_head_is_exist, &sig_ptr->sms_t_u.mo_sms_t.dcs, &sig_ptr->sms_t_u.mo_sms_t.user_data_t, &ascii_vaild);

    // add the readed SMS to global
    MMISMS_SaveReadedSmsToGlobal(sig_ptr->item_sys, MMISMS_READ_MOSMS, &ascii_vaild, &(sig_ptr->sms_t_u.mo_sms_t.dest_addr_t), sig_ptr->sms_t_u.mo_sms_t.dcs.alphabet_type);
    
    if (g_mmisms_global.operation.cur_order_index < MMISMS_SPLIT_MAX_NUM -1)
    {
        g_mmisms_global.operation.cur_order_index ++;
        next_order_id = MMISMS_GetCurOperationOrderId();  //������MMISMS_GetCurOperationOrderId
    }

    if(next_order_id != PNULL)
    {
        MNSMS_ReadSmsEx((MN_DUAL_SYS_E)next_order_id->flag.dual_sys, (MN_SMS_STORAGE_E)next_order_id->flag.storage, next_order_id->record_id );
    }
    else
    {
        MMISMS_ClearOperInfo();
        // set the init SMS content
        MMISMS_GetSMSContent(&msg_content);
        if (msg_content.wstr_len != 0)
        {
            MMISMS_SetMessageContent(msg_content.wstr_len, msg_content.wstr_ptr, TRUE, &g_mmisms_global.edit_content);
        
            num_len = MMISMS_GetOriginNum(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
            MMISMS_ClearDestNum();
        
            MMISMS_SetAddressToMessage(number, num_len, &g_mmisms_global.edit_content.dest_info.dest_list);
        
            MMISMS_SetSaveFlag(TRUE);
            MMISMS_SetCurSendDualSys(dual_sys);
            MMISMS_SetSMSUpdateInfo(g_mmisms_global.operation.cur_order_id_arr[0]);
            send_err_code = MMISMS_WintabSendSMS(TRUE);
        }
        else
        {
            SCI_TRACE_LOW("Handle_ReadResendSms Read error");
        }
    }
}
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Description : GetBtSMSBoxTypeByReadSMSCnf
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMISMS_BOX_TYPE_E GetBtSMSBoxTypeByReadSMSCnf(APP_MN_READ_SMS_CNF_T *sig_ptr)
{
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    BOOLEAN is_exist_sc_addr = FALSE;
    MMISMS_MO_MT_TYPE_E mo_mt_type = MMISMS_MO_DRAFT;
    
    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetSMSBoxTypeByReadSMSCnf sig_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1937_112_2_18_2_52_54_118,(uint8*)"");
        return box_type;
    }
    switch (sig_ptr->status)
    {

    //sort the MT SMS
    case MN_SMS_MT_READED:                   
    case MN_SMS_MT_TO_BE_READ:
        box_type = MMISMS_BT_BOX_MT;
        break;
        
    case MN_SMS_MT_SR_TO_BE_READ:
    case MN_SMS_MT_SR_READED:
        box_type = MMISMS_BT_BOX_MT;
        break;
        
    case MN_SMS_MO_TO_BE_SEND:
    case MN_SMS_MO_SR_NOT_REQUEST:          
    case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
    case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
    case MN_SMS_MO_SR_RECEIVED_AND_STORED:
        SCI_TRACE_LOW("[MMISMS] sc_addr_present = %d,  num_len =%d", sig_ptr->sms_t_u.mo_sms_t.sc_addr_present, sig_ptr->sms_t_u.mo_sms_t.sc_addr_t.num_len);

		if( MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurOperationType() )
        {
            is_exist_sc_addr = TRUE;
        }
        else if( MMISMS_BT_BOX_NOSEND == MMISMS_GetCurOperationType() )
        {
            is_exist_sc_addr = FALSE;
        }

        mo_mt_type = MMISMS_TranslateMNStatus2SFStatus(sig_ptr->status, MMISMS_MO_TYPE, is_exist_sc_addr);

        if (MMISMS_MO_SEND_SUCC == mo_mt_type)
        {
            box_type = MMISMS_BT_BOX_SENDSUCC;
        }
        else if (MMISMS_MO_SEND_FAIL == mo_mt_type)
        {
            box_type = MMISMS_BT_BOX_SENDFAIL;
        }
        else if (MMISMS_MO_DRAFT == mo_mt_type)
        {
            box_type = MMISMS_BT_BOX_NOSEND;
        }
        break;
        
    default:
        break;
    }

    //SCI_TRACE_LOW:"MMI_SMS:GetSMSBoxTypeByReadSMSCnf box_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_1987_112_2_18_2_52_54_119,(uint8*)"d", box_type);

    return box_type;
}
#endif