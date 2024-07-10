/***************************************************************************
** File Name:      mmisms_api.c                                            *
** Author:                                                                 *
** Date:           04/2006                                                   *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to define the API function.           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2006          Tracy Zhang       Modularize
****************************************************************************/
/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmiphone_export.h"
#include "mmi_textfun.h"
#include "mmipub.h"
#include "mmisms_export.h"
#include "mmisms_internal.h"
#include "mmisms_app.h"
#include "mmisms_id.h"
#include "mmisms_nv.h"
#include "mmisms_menutable.h"
//#include "mmisms_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmisms_delete.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmisms_send.h"
#include "mmi_appmsg.h"
#include "mmi_atc.h"
#include "mmiidle_export.h"
#include "mmisms_set.h"
#include "mmisms_read.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif
#include "mmiidle_statusbar.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmisms_timermsg.h"
#include "mmialarm_service.h"
#endif
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mmimail_export.h"
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmisms_edit.h"
#include "mmisms_save.h"

#include "mmisms_chatreadwin.h"
#endif

#include "mmiota_export.h"
#include "mmiidle_statusbar.h"
#include "mmimms_export.h"
#include "mmisms_image.h"
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "mmisms_messageboxwin.h"
#include "mmisms_pushmsgboxwin.h"
#include "mmisms_commonui.h"
#include "mmimms_id.h"
#include "mmisms_appcontrol.h"
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
#include "mmisms_voicemail.h"
#endif
#include "mmismsapp_interface.h"

#include "mmibt_id.h"
/**------------------------------------------------------------------------*
**                         EXTERN DECLARE                                  *
**-------------------------------------------------------------------------*/

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
typedef struct MAP_LIST_T{
    void    *data;       //节点数据
    uint16  len;       //数据长度
    struct MAP_LIST_T *next; //指向下个节点
} MAP_LIST_T;

LOCAL MAP_LIST_T    *s_map_list_head = PNULL;
LOCAL uint32        s_map_list_node_number = 1;
LOCAL MAP_LIST_T    *s_map_bmessage_head = PNULL;
LOCAL uint16        s_map_bmessage_node_number = 1;
LOCAL uint16        s_map_bmessage_current_node_number = 1;

LOCAL BT_MSG_LIST_T *s_message_table_head = PNULL;

LOCAL uint16        s_sync_sms_count = 0;   //记录成功同步联系人的个数
LOCAL uint8         s_send_sms_num_count = 0;  //记录要发送信息的联络人个数
LOCAL char          *s_sms_buff_ptr = PNULL;      //记录不完整的sms数据
LOCAL BOOLEAN       g_is_cur_oper_push = FALSE;
LOCAL BOOLEAN       g_is_cur_oper_connect = FALSE;
LOCAL BOOLEAN       g_is_cur_oper_disconnect = FALSE;
LOCAL BOOLEAN       g_is_cur_send_multi_sms = FALSE;
LOCAL BOOLEAN       g_is_cur_sms_read = FALSE;   // FALSE: unread, TRUE: read
LOCAL BOOLEAN       g_is_update_outbox = FALSE;  // default: false
LOCAL BOOLEAN       g_is_update_sendbox = FALSE; // default: false
LOCAL BOOLEAN       g_is_update_inbox = FALSE; // default: false
LOCAL BOOLEAN       g_is_update_draft = FALSE; // default: false
LOCAL BOOLEAN       g_is_cur_oper_boot = FALSE; // default: false
LOCAL MMI_STRING_T  p_long_sms = {0};

LOCAL MMISMS_SYNC_TYPE_E sync_type;

typedef enum _MMISMS_BT_MSG_TYPE_E
{
    MMISMS_HANDLE,
    MMISMS_TIMESTAMP,
    MMISMS_MAX
} MMISMS_BT_MSG_TYPE_E;
#endif

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
extern MMISMS_VMN_OPER_TYPE_E   g_vmn_oper_type;//查询语音信箱号是否为了通话转移
#endif

extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS的全局变量

#ifdef MMI_SMS_SYN_SUPPORT
extern BOOLEAN Syn_end;
#endif

#ifndef MMI_SYNCTOOL_SUPPORT
#define SMS_SYNC_POS_ME				'0'
#define SMS_SYNC_POS_SIM_1	        '1'
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
#if defined(TRACE_MAP_BUFFER)
#define TRACE_MAP_BUF_COUNT_BYTE         120
#endif
#define BMSG_HEADER_LEN 350
#define BMESSAGE_CONTENT_MAX_LEN             1000//220 bug570483
#define BMESSAGE_TOKEN_COUNT             15

#define MAP_LIST_LEN                      sizeof(MAP_LIST_T)
#define BT_MSG_LIST_LEN                   sizeof(BT_MSG_LIST_T)

#define FOLDERLISTING_TAGS                "folder-listing"    //域开始标签
#define FOLDERLISTING_END_TAGS            "//folder-listing"  //域结束标签

#define MSGLISTING_TAGS                   "MAP-msg-listing"   //域开始标签
#define MSGLISTING_END_TAGS               "//MAP-msg-listing" //域结束标签

#define MSGLISTINGHANDLE_TAGS             "msg handle"     //域开始标签
#define MSGLISTINGHANDLE_END_TAGS         "/>"  

#define DATETIME_TAGS                     "datetime"       //域开始标签
#define SUBJECT_TAGS                      "subject"        //域开始标签

#define SENDER_ADDRESS_TAGS               "sender_addressing"
#define SENDER_NAME_TAGS                    "sender_name"
#define RECIPIENT_ADDRESS_TAGS            "recipient_addressing"
#define RECIPIENT_NAME_TAGS                    "recipient_name"
#define SIZE_TAGS                         "size"           //域开始标签
#define READ_TAGS                         "read"
#define TYPE_TAGS                         "type"
#define COMMON_END_TAGS                   "\" "               //域结束标签
#define COMMON_END_TAGS2                  "' "               //域结束标签

#define MESSAGE_TAGS                      "BEGIN:BMSG"        //域开始标签
#define MESSAGE_END_TAGS                  "END:BMSG"          //域结束标签
#define VCARD_TAGS                        "BEGIN:VCARD"       //域开始标签
#define VCARD_END_TAGS                    "END:VCARD"         //域结束标签
#define STATUS_TAGS                       "\r\nSTATUS:"       //域结束标签
#define NAME_TAGS                         "\r\nN:"            //域开始标签
#define TEL_TAGS                          "\r\nTEL:"          //域结束标签
#define MSG_BODY_TAGS                     "BEGIN:BBODY"       //域结束标签
#define MSG_BODY_END_TAGS                 "END:BBODY"         //域结束标签
#define MSG_PDU_TAGS                      "BEGIN:MSG\r\n"     //域结束标签
#define MSG_PDU_END_TAGS                  "\r\nEND:MSG"       //域结束标签
#define MSG_CHARSET_TAGS                  "CHARSET:"          //域结束标签
#define NEW_LINE                          "\r\n"
#define UTF8_STRING                       "UTF-8"
#define UCS2_STRING                       "UCS-2"
#define BMESSAGE_TAGS                     "<MAP-event-report version=\"1.0\">"  //域开始标签
#define BMESSAGE_END_TAGS                 "<MAP-event-report>"                  //域结束标签
#define BMESSAGE_EVENT_TYPE               "event type"
#define BMESSAGE_HANDLE                   "handle"
#define BMESSAGE_MSG_TYPE                 "msg_type"
#define BMESSAGE_FOLDER                   "folder"
#define EVENT_NEW_MESSAGE                 "NewMessage"
#define EVENT_DELIVERY_SUCCESS            "DeliverySuccess"
#define EVENT_SENDING_SUCCESS             "SendingSuccess"
#define EVENT_DELIVERY_FAILURE            "DeliveryFailure"
#define EVENT_SENDING_FAILURE             "SendingFailure"
#define EVENT_MEMORY_FULL                 "MemoryFull"
#define EVENT_MEMORY_AVAILABLE            "MemoryAvailable"
#define EVENT_MESSAGE_DELETED             "MessageDeleted"
#define EVENT_MESSAGE_SHIFT               "MessageShift"
#define FOLDER_INBOX_PATH                 "telecom/msg/inbox"
#define FOLDER_OUTBOX_PATH                "telecom/msg/outbox"
#define FOLDER_SENTBOX_PATH               "telecom/msg/sent"
#define FOLDER_DRAFT_PATH                 "telecom/msg/draft"
#endif

MMISMS_INJECTION_FUNC s_p_injection_func = PNULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL MAP_LIST_T* InitList(void) ;


/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int InsertList(MAP_LIST_T *L, void* buff,uint16 buff_len);


/*******************************************************************************/
//  Description:清除线性表L中的所有元素，即释放单链表L中所有的结点，使之成为一个空表
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL void ClearList(MAP_LIST_T *pHead);

/*******************************************************************************/
//  Description:获取单链表中第i个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int GetElem(MAP_LIST_T *L, int i, void** buff, uint16 *len);

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL BT_MSG_LIST_T* InitMsgHandleTableList(void) ;

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int InsertMsgHandleTableList(BT_MSG_LIST_T *L, char* handle, BOOLEAN is_extra_msg, wchar* name, uint32 name_len);

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL BOOLEAN DeleteMsgHandleTableList(BT_MSG_LIST_T *L, MMI_CTRL_ID_T record_id);

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int AddRecordIdIntoMsgHandleTableList(BT_MSG_LIST_T *L, MMI_CTRL_ID_T record_id);

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifySMSStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifySetFoldertParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifyGetMsgListStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);


/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifyGetMessageStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InitMAPDataList(void);

/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AddDataToSMS(const char*  buff     //[in]                                      
                                        );

/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SetFolder(const char*  buff) ;

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
LOCAL void freeallmapdata(void);

/*****************************************************************************/
//  Description : check phone number
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN CheckPhoneNumberFromBuffer(
                                  char      *number,  //IN:
                                  uint32    bufferlen,      //IN:
                                  uint32    **ppdata       //OUT:
                                  );

/*****************************************************************************/
//  Description : save long sms content
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN ParseDataFromBuffer(
                                  char*            searchbuffer,         //IN:
                                  uint32           bufferlen,   //IN:
                                  char*            begin,          //IN:
                                  char*            end,            //IN:
                                  uint8**          ppdata,       //OUT:
                                  uint32*          data_len      //OUT:
                                  );

/*****************************************************************************/
//  Description : save long sms content
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN LocateDataFromBuffer(
                                  char*            searchbuffer,         //IN:
                                  uint32           bufferlen,   //IN:
                                  char*            begin,          //IN:
                                  char*            end,            //IN:
                                  uint8**          ppdata,       //OUT:
                                  uint32*          data_len      //OUT:
                                  );

/*****************************************************************************/
//  Description : save mo sms content
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMIAPISMS_SaveMoMsgReq(
                                               MMISMS_OPER_STATUS_E    oper_status,
                                               MN_DUAL_SYS_E           dual_sys,
                                               uint8                   *dest_addr_ptr,     //IN:
                                               uint8                   dest_len,           //IN:
                                               APP_SMS_USER_DATA_T     *user_data_ptr,     //IN:
                                               MN_SMS_STORAGE_E        storage,            //IN:
                                               MN_SMS_ALPHABET_TYPE_E  alphabet_type,      //IN:
                                               char                    *number,
                                               uint32                  number_len
                                               );

/*****************************************************************************/
//  Description : save mt sms content
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMIAPISMS_SaveMtMsgReq(
                                               MN_DUAL_SYS_E            dual_sys,
                                               APP_SMS_USER_DATA_T      *user_data_ptr,     //IN:
                                               MN_SMS_STORAGE_E         storage,            //IN:
                                               MN_SMS_ALPHABET_TYPE_E   alphabet_type,      //IN:
                                               char                     *number,
                                               uint32                   number_le
                                               );

/*******************************************************************************/
//  Description :Insert sms record into list
//  Parameter: buff[in]: 
//  Global resource dependence :                                
//  Author: candice chen
//  Return : 
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_InsertSMSRecord(
                                               char          *tel, 
                                               MMI_STRING_T  *p_msg_content,
                                               uint32        time
                                               );
/*******************************************************************************/
//  Description :ParseSubjectDataFromBuffer
//  Parameter: buff[in]: 
//  Global resource dependence :                                
//  Author: candice chen
//  Return : 
/*****************************************************************************/
LOCAL BOOLEAN ParseSubjectDataFromBuffer(char           *src_buff_ptr, 
                                         uint16         src_len, 
                                         uint16         content_len,
                                         MMI_STRING_T   *p_msg_content
                                         );

#endif

/***************************************************************************/
//     Description : init the SMS for ps
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/***************************************************************************/
void MMIAPISMS_InitForPS(void)
{
    MMISMS_InitForPS();
}

/***************************************************************************/
//     Description : init the SMS global
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/***************************************************************************/
void MMIAPISMS_AppInit(void)
{
    MMISMS_AppInit();
}


#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : get delete all flag
//                  TRUE:        do delete all SMS operation  
//                  FALSE:    without do delete all SMS operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_GetDelAllFlag(void)
{
    return (MMISMS_GetDelAllFlag());
}
#endif /* Reduce Code size */


/*****************************************************************************/
//     Description : to enter the window for writing SMS 
//    Global resource dependence : 
//  Author:louis.wei
//    Note:多个联系人短信编辑
/*****************************************************************************/
PUBLIC void MMIAPISMS_SendGroupSms(
                               MN_DUAL_SYS_E   dual_sys,           //IN:
                               MMI_STRING_T    *sms_init_ptr,        //IN:
                               MMISMS_DEST_ADDR_INFO_T *dest_info
                               )
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();
    
    MMIAPISMS_ExitFromSMS();

    if (MMISMS_IsOrderOk()&& MMISMS_IsMMSOrderReady())
    {
        if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
        {           
            MMISMS_SEND_GROUP_INFO_T mmisms_sendgroup_info = {0};
            uint16 length = 0;
            mmisms_sendgroup_info.dest_info = SCI_ALLOCAZ(sizeof(MMISMS_DEST_ADDR_INFO_T));
            if(PNULL == mmisms_sendgroup_info.dest_info)
            {
                return;
            }    
               
            SCI_MEMCPY((void*)mmisms_sendgroup_info.dest_info, (void*)dest_info, sizeof(MMISMS_DEST_ADDR_INFO_T));
            if(sms_init_ptr != PNULL)
            {
                length = sms_init_ptr->wstr_len;
                mmisms_sendgroup_info.sms_str.wstr_ptr = SCI_ALLOCAZ( (length + 1) * sizeof(wchar) );
                if(PNULL == mmisms_sendgroup_info.sms_str.wstr_ptr)
                {
                    SCI_FREE(mmisms_sendgroup_info.dest_info);
                    return;
                } 
                SCI_MEMCPY((void*)mmisms_sendgroup_info.sms_str.wstr_ptr , (void*)sms_init_ptr->wstr_ptr,  (length + 1) * sizeof(wchar));
                mmisms_sendgroup_info.sms_str.wstr_len = length;
            }
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, MMISMS_OpenSendGroupWinExt,(DPARAM)&mmisms_sendgroup_info,sizeof(MMISMS_SEND_GROUP_INFO_T));
        }
        else
        {
            MMISMS_SendGroupSms(dual_sys,sms_init_ptr,dest_info);
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        MMISMS_OpenInitWaitingWindow(MMISMS_WRITE_NEW_SMS);
    }
}


/*****************************************************************************/
//     Description : to enter the window for writing SMS 
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 单个联系人或无联系人短信编辑
/*****************************************************************************/
PUBLIC void MMIAPISMS_WriteNewMessage(
                               MN_DUAL_SYS_E    dual_sys,           //IN:
                               MMI_STRING_T        *sms_init_ptr,        //IN:
                               uint8            *dest_addr_ptr,        //IN:
                               uint8            dest_addr_len        //IN:
                               )
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();
    
    MMIAPISMS_ExitFromSMS();

    if (MMISMS_IsOrderOk()&& MMISMS_IsMMSOrderReady())
    {
        MMISMS_SetCurType(MMISMS_BOX_NONE); //Bug 264174 add box type 

        if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
        {           
            MMISMS_WRITE_MSG_INFO_T mmisms_write_info = {0};
            uint16 length = 0;
            mmisms_write_info.dest_addr_ptr = SCI_ALLOCAZ( dest_addr_len + 1 );
            if(PNULL == mmisms_write_info.dest_addr_ptr)
            {
                return;
            }    
               
            SCI_MEMCPY((void*)mmisms_write_info.dest_addr_ptr, (void*)dest_addr_ptr, dest_addr_len);
            mmisms_write_info.dest_addr_len = dest_addr_len;
			if(sms_init_ptr != PNULL)
			{
                length = sms_init_ptr->wstr_len;
                mmisms_write_info.sms_str.wstr_ptr = SCI_ALLOCAZ( (length + 1) * sizeof(wchar) );
                if(PNULL == mmisms_write_info.sms_str.wstr_ptr)
                {
                    SCI_FREE(mmisms_write_info.dest_addr_ptr);
                    return;
                } 
                SCI_MEMCPY((void*)mmisms_write_info.sms_str.wstr_ptr , (void*)sms_init_ptr->wstr_ptr,  (length + 1) * sizeof(wchar));
                mmisms_write_info.sms_str.wstr_len = length;
            }
            mmisms_write_info.dual_sys = dual_sys;
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, MMISMS_OpenWriteWindowExt,(DPARAM)&mmisms_write_info,sizeof(MMISMS_WRITE_MSG_INFO_T));
        }
        else
        {
#ifdef MMI_SMS_MINI_SUPPORT
            MMISMS_ClearDestNum(); 
#endif
            MMISMS_WriteNewMessage(
                dual_sys,
                sms_init_ptr,
                FALSE,
                dest_addr_ptr,
                dest_addr_len
                );
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        MMISMS_OpenInitWaitingWindow(MMISMS_WRITE_NEW_SMS);
    }
}

/*****************************************************************************/
//  Description :  set dest number for write new msg
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: interface for other module/pb
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetAddressToMessageEx(
                                               uint8            *dest_addr_ptr,        //IN:
                                               uint8            dest_addr_len        //IN:
                                               )
{
    
    if (PNULL == dest_addr_ptr || 0 == dest_addr_len)
    {
        return FALSE;
    }
    if (MMISMS_DEST_ADDR_MAX_NUM <= g_mmisms_global.edit_content.dest_info.dest_list.number)
    {
        return FALSE;
    }
    MMISMS_SetAddressToMessage(dest_addr_ptr, dest_addr_len, &g_mmisms_global.edit_content.dest_info.dest_list);
    return TRUE;
}

/*****************************************************************************/
//     Description :set address to message
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetAddressToMessage(
                                       const uint8        *num_ptr,    //IN:
                                       uint8            num_len,        //IN:
                                       uint32   *dest_list_ptr
                                       )
{
    MMISMS_SetAddressToMessage(num_ptr,num_len,(MMISMS_NUMBER_LIST_T *)dest_list_ptr);
}

/*****************************************************************************/
//     Description : to check the SMS state
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_CheckSMSState(BOOLEAN is_draw)
{
    if (MMISMS_IsOrderOk())
    {
        MAIN_SetMsgState(is_draw);
    }
}

/*****************************************************************************/
//     Description : to check the space
//    Global resource dependence : 
//  Author:louis.wei
//  Modify by: jian.ma
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSFull(void)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        if (MMISMS_CountFreeSpace(i) >0) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : to delete all SMS
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_DelAllSMS(MMISMS_BOX_TYPE_E box_type)
{
    MMIAPISMS_DeleteAllMMSDone(box_type);
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : to check the order is ok
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_IsOrderOk ( void )
{
    return MMISMS_IsOrderOk();
}

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_IsReady( void )
{
    return MMISMS_IsReady();
}

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSReady(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_IsSMSReady(dual_sys);
}


/*****************************************************************************/
//     Description : to delete SMS info form order list by AT
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPISMS_DelSMSByAT(                                            //RETURN:
                             BOOLEAN                 is_mt_sms,            //IN:
                             uint8                    pos,                //IN:
                             MN_SMS_RECORD_ID_T        record_id            //IN:
                             )
{
    BOOLEAN result = FALSE;
    
#ifdef MMI_SYNCTOOL_SUPPORT
    if(SMS_SYNC_POS_ME == pos)
    {
        result = MMISMS_DelSMSByAT(
            MN_DUAL_SYS_1,
            is_mt_sms,
            FALSE,
            record_id
            );
    }
    else if(pos >= SMS_SYNC_POS_SIM_1 && pos < (SMS_SYNC_POS_SIM_1+MMI_DUAL_SYS_MAX))
    {
        result = MMISMS_DelSMSByAT(
            (MN_DUAL_SYS_E)(pos - SMS_SYNC_POS_SIM_1),
            is_mt_sms,
            TRUE,
            record_id
            );
    }
#else
    if(0x30 == pos)
    {
        result = MMISMS_DelSMSByAT(
            MN_DUAL_SYS_1,
            is_mt_sms,
            FALSE,
            record_id
            );
    }
    else if(pos >= 0x31 && pos < (0x31 +MMI_DUAL_SYS_MAX))
    {
        result = MMISMS_DelSMSByAT(
            (MN_DUAL_SYS_E)(pos - 0x31),
            is_mt_sms,
            TRUE,
            record_id
            );
    }
#endif   

    return result;
}

//window related
/*****************************************************************************/
//     Description : to open the list of box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenSmsBox(
                          MMISMS_BOX_TYPE_E        box_type
                          )
{
    MMISMS_OpenSmsBox(box_type);
}

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMIAPISMS_ShowNewMsgPrompt(void)
{
    MMISMS_ShowNewMsgPrompt(MN_DUAL_SYS_1, FALSE);  //if is_play_ring is FALSE,we won't care the param dual_sys
}

/*****************************************************************************/
//     Description : exit from sms module
//    Global resource dependence : 
//  Author: gang.tong
//    Note: 
/*****************************************************************************/
void MMIAPISMS_ExitFromSMS(void)
{
    MMK_CloseMiddleWin(MMISMS_START_WIN_ID, MMISMS_END_WIN_ID);

    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        MMK_CloseWin(MMIMSG_MAIN_MENU_WIN_ID);
    }

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
    MMIAPIMAIL_Exit();
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
}

/*****************************************************************************/
//     Description : to open the SMS main menu window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenMainMenuWin(void)
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();

    if(MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady()) // 增加判断mms是否排序完毕
    {
        MMIAPISMS_ExitFromSMS();
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_SMS, MMISMS_OpenMainMenuWindowExt);
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        // 需要开启等待窗口    
        MMISMS_OpenInitWaitingWindow(MMISMS_OPEN_MAIN_MENU);
    }
}

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
/*****************************************************************************/
//     Description : open frequent word list for cc module
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenFreqWordListForCc(
                                     MN_DUAL_SYS_E dual_sys,
                                     uint8    number_len,    //IN:
                                     uint8    *number_str    //IN:
                                     )
{
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_OpenFreqWordListForCc len = %d, addr = %p"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_376_112_2_18_2_52_27_0,(uint8*)"dd",number_len, number_str);

    MMIAPISMS_ExitFromSMS();

    MMISMS_OpenFreqWordListForCc(dual_sys,number_len, number_str);
}
#endif

/*****************************************************************************/
//  Description : init SMS Module
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitModule(void)
{
    MMISMS_RegMenuGroup();
    MMISMS_RegWinIdNameArr();
    MMISMS_RegNv();
}

/*****************************************************************************/
//  Description : close all SMS window
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
void MMIAPISMS_CloseAllSMSWindow(void) 
{
    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        MMK_CloseMiddleWin(MMISMS_START_WIN_ID, MMISMS_END_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : open deleting SMS window
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
void MMIAPISMS_OpenDelWindow(void) 
{
    if (MMISMS_GetDelAllFlag())
    {
        MMI_STRING_T prompt_str = {0};

        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
    }
}
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
//     Description : to open the window to call the voice mail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenCallVMWin(MN_DUAL_SYS_E dual_sys)
{
    g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_IDLE_DIAL;
    MMISMS_OpenVMNumberWin(dual_sys, MMISMS_LINE1_VMN_RECORD_ID);
}
#endif
/*****************************************************************************/
#if 0 /* Reduce Code size */
/*****************************************************************************/
//Description : Get SMS Sending SIM(Not In Sending return MMI_DUAL_SYS_MAX)
//    Global resource dependence : 
//  Author: wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetSendingSIM(void)
{
    if (MMIAPISMS_IsSendingSMS())
    {
        return MMISMS_GetCurSendDualSys();
    }

    return MMI_DUAL_SYS_MAX;
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : restore factory setting
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_RestoreFactorySetting(void)
{
    MMISMS_SetValidPeriod(MMISMS_VALID_PERIOD_DEFAULT);//@zhaohui,cr95392
    MMISMS_SetStatusReportFlag(MMISMS_STATUSREPORT_FLAG_DEFAULT);
#if defined(MMI_SMSCB_SUPPORT)
    MMIAPISMSCB_RestoreFactorySetting();
#endif
    MMISMS_SetPreferredConnection(MMISMS_PREFERRED_PATH_GSM_PREFERRED);
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT 
    MMISMS_SetAutoSave(MMISMS_AUTOSAVE_FLAG_DEFAULT);
#endif
    MMISMS_SetSavePos(MMISMS_SAVEPOS_PHONE);
    MMISMS_SetOrderType(MMISMS_ORDER_BY_TIME);
    MMIAPISMS_SetDefaultPresetSms(FALSE);
    MMISMS_SetPushSmsBrowserInfo(MMISMS_BROWSERINFO_FLAG_DEFAULT);
    MMISMS_SetSMSIsExtract(MMISMS_ISEXTRACT_FLAG_DEFAULT);
    MMISMS_SetSecurityPwd();
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    MMISMS_SetAutoSignatureFlag(MMISMS_AUTO_SIGNATURE_FLAG_DEFAULT);
#endif
}

/*****************************************************************************/
//     Description : 根据record_id将一条信息从order list 中删除
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteMsgInorder(MMISMS_MSG_TYPE_E msg_type, int32 record_id)
{
    MMISMS_ORDER_ID_T      order_id = PNULL;
    BOOLEAN                ret_value = FALSE;

    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_DeleteMsgInorder : msg_type = %d,record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_483_112_2_18_2_52_27_1,(uint8*)"dd", msg_type, record_id );

    MMISMS_GetOrderIdByStorage(&order_id, msg_type, MN_DUAL_SYS_MAX, MN_SMS_NO_STORED, record_id);
    if ( PNULL == order_id )
    {
        //can't find the mms in order list by record id
        ret_value = FALSE;        
    }
    else
    {
#if 0   //user MMISMS_DATACHANGE_TYPE_DELETE trigger             
        if (MMISMS_TYPE_MMS == order_id->flag.msg_type)
        {
            if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)) 
            {
                MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容 
            }
        }
#endif
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        if(MMISMS_FOLDER_BT == order_id->flag.folder_type)
        {
            ret_value = MMISMS_DelBtSmsInfoInOrder(order_id);
        }
        else
        {
            ret_value = MMISMS_DelSmsInfoInOrder(order_id);
        }
#else
        ret_value = MMISMS_DelSmsInfoInOrder(order_id);
#endif
        if (MMISMS_IsMMSOrderReady())
        {
            //MMISMS_HandleOrderChangeCallBack();
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);              
        }
    }

    return ret_value;
}


/*****************************************************************************/
//     Description : 插入一条信息到order list
//    Global resource dependence : 
//  Author: liming.deng
//    Note:  为彩信、wap push、wap ota、mms ota模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_InsertOneMsgToOrder(MMISMS_ORDER_INFO_T *order_ptr)
{
    MMISMS_STATE_T state_info = {0};
    BOOLEAN ret_value  = FALSE;
    MMISMS_DATACHANGE_TYPE_E type = MMISMS_DATACHANGE_TYPE_MAX;
    if (PNULL == order_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_524_112_2_18_2_52_27_2,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_InsertOneMsgToOrder order->mo_mt_type = %d, mms_record_id = %d, dual_sys = %d, msg_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_528_112_2_18_2_52_27_3,(uint8*)"dddd",order_ptr->mo_mt_type, order_ptr->record_id, order_ptr->dual_sys, order_ptr->msg_type);

    state_info.flag.mo_mt_type = order_ptr->mo_mt_type;
    state_info.flag.msg_type = order_ptr->msg_type;
    state_info.flag.storage = MN_SMS_NO_STORED;
    state_info.time = order_ptr->time;
    state_info.record_id = order_ptr->record_id;
    state_info.flag.dual_sys = order_ptr->dual_sys;
    state_info.flag.is_concatenate_sms = FALSE;
    state_info.flag.is_locked = order_ptr->is_lock;
    state_info.flag.folder_type = order_ptr->folder_type;
    state_info.display_content.content_len = MIN(order_ptr->display_content.content_len, MMIMESSAGE_DIAPLAY_LEN );
    MMI_WSTRNCPY( state_info.display_content.content,
                            MMIMESSAGE_DIAPLAY_LEN,
                            order_ptr->display_content.content,
                            state_info.display_content.content_len,
                            state_info.display_content.content_len
                            );
    //SCI_TRACE_LOW:"state_info.display_content.content_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_546_112_2_18_2_52_27_4,(uint8*)"d",state_info.display_content.content_len);
    SCI_MEMCPY(&(state_info.number.sender),&(order_ptr->sender),(sizeof(uint8)*MMISMS_NUMBER_MAX_LEN));

    if (MMISMS_IsOrderOk())
    {
        ret_value = MMISMS_InsertSmsToOrder(&state_info); 
        
        //for msg delete all 11/02/18 信息全部删除使用
#ifdef MMI_SMS_DELALL_SUPPORT        
#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT      
        if (MMK_IsOpenWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID))
        {
            MMK_SendMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
        }
#else
        if (MMK_IsOpenWin(MMISMS_MSGDEL_ALL_WIN_ID))
        {
            MMK_SendMsg(MMISMS_MSGDEL_ALL_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
        }
#endif        
#endif
        if (MMISMS_IsMMSOrderReady())
        {   
            if(MMISMS_FOLDER_NORMAL == order_ptr->folder_type)
            {
                type = MMISMS_DATACHANGE_TYPE_ADD;
            }
            else
            {
                type = MMISMS_DATACHANGE_TYPE_SECURITY_ADD;
            }
            
            MMISMS_TrigSmsDataChangeEvent(type);
            MAIN_SetMsgState(TRUE);
        }
    }
    else
    {
        ret_value = MMISMS_InsertSmsToUnhandleOrder(&state_info);        
    }
#ifdef MMI_SMS_CHAT_SUPPORT
    if ((MMISMS_TYPE_MMS == order_ptr->msg_type) &&
         MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) &&
         MMISMS_FOLDER_SECURITY != order_ptr->folder_type  &&
         MMISMS_CompareMMSNum(order_ptr->sender,g_mmisms_global.sender)
                 && (order_ptr->mo_mt_type != MMISMS_MO_DRAFT))
    {
        MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MSG_MMS_UPDATE_CHAT, PNULL,0); 
    }
#endif
    
    return ret_value;
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : 更新聊天界面的彩信发送状态
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_UpdateResendMMSLabel(int32 record_id)
{
    MMI_STRING_T time_str = {0};
    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar time[20] = {0};
    uint16 i = 0;    

    for(i=0;i<MMISMS_CHAT_MAX_ITEM;i++)
    {
        if((g_mmisms_global.chat_item[i].cur_order_id != PNULL) && (g_mmisms_global.chat_item[i].is_valid) && (record_id == g_mmisms_global.chat_item[i].cur_order_id->record_id))
        {
            order_id =  g_mmisms_global.chat_item[i].cur_order_id;
            break;
        }
    }
    if(PNULL == order_id || i == MMISMS_CHAT_MAX_ITEM)
    {
        SCI_TRACE_LOW("mmisms resend order pnull");
        return;        
    }

    time_str.wstr_ptr = time;
    time_str.wstr_len = MMISMS_GetDateTimeStringByOrder(order_id, TRUE,FALSE,TRUE, time);
    
    if (MMISMS_MO_SEND_FAIL == order_id->flag.mo_mt_type 
        && order_id->flag.msg_type == MMISMS_TYPE_MMS
        && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        && MMISMS_CompareMMSNum(order_id->number.sender,g_mmisms_global.sender)
        )
    {
        wchar *str = PNULL;
        MMI_STRING_T wstr = {0};
        MMI_STRING_T fail_str = {0};
        
        if (MMISMS_TYPE_MMS == order_id->flag.msg_type
                && (MMIAPIMMS_IsProcessMMSById(order_id->record_id) || MMIAPIMMS_IsMMSInDownloadQueue(order_id->record_id)))
        {
            MMI_GetLabelTextByLang(TXT_SENDING, &fail_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_COMM_SEND_FAILED, &fail_str);
        }

        str = SCI_ALLOC_APPZ((MMISMS_CHAT_SENDFAIL_STR_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == str)
        {
            return;
        }
        wstr.wstr_ptr = str;
        MMI_WSTRNCPY(str, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN , time_str.wstr_ptr, time_str.wstr_len, MIN(time_str.wstr_len, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN));
        MMIAPICOM_Wstrcat(str, L" ");
        MMIAPICOM_Wstrncat(str, fail_str.wstr_ptr, fail_str.wstr_len);
        wstr.wstr_len = MMIAPICOM_Wstrlen(str);

        GUILABEL_SetText(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + i, &wstr, FALSE);
        SCI_FREE(str);

    }    
}


/*****************************************************************************/
//  Description : 清除发送文字接口
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearChatEditString(void)
{
    GUIEDIT_ClearAllStr(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
}
/*****************************************************************************/
//  Description : 更新聊天界面封装接口
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_UpdateChatShow(MMISMS_ORDER_INFO_T *order_info_ptr)
{
    if (PNULL == order_info_ptr)
    {
        return FALSE;
    }
    if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) &&
         MMISMS_CompareMMSNum(order_info_ptr->sender,g_mmisms_global.sender))
    {

        MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MSG_MMS_UPDATE_CHAT, PNULL,0);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :是否锁屏是否未读短信
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsKeyLockNeedCountSms(BOOLEAN is_keylock_need_count_sms)
{
    MMISMS_SetIsKeyLockNeedCountSms(is_keylock_need_count_sms);
}

/*****************************************************************************/
//  Description :是否锁屏是否未读短信
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsKeyLockNeedCountSms(void)
{
    return MMISMS_GetIsKeyLockNeedCountSms();
}
#endif

/*****************************************************************************/
//     Description : 更新信息状态
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_UpdateMsgStatusInOrder(MMISMS_ORDER_INFO_T *order_info_ptr)
{
    MMISMS_STATE_T state_info = {0};
    MMISMS_ORDER_ID_T old_order_id = PNULL;
    MMISMS_STATE_T old_state_info = {0};
    BOOLEAN ret_value = FALSE;


    if (PNULL == order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_613_112_2_18_2_52_27_5,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_UpdateMsgStatusInOrder enter: record_id = %d, dual_sys = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_617_112_2_18_2_52_27_6,(uint8*)"dd", order_info_ptr->record_id, order_info_ptr->dual_sys);

    state_info.time = order_info_ptr->time;
    state_info.record_id = order_info_ptr->record_id;    
    state_info.flag.mo_mt_type = order_info_ptr->mo_mt_type;
    state_info.flag.storage = MN_SMS_NO_STORED;
    state_info.flag.dual_sys = order_info_ptr->dual_sys;
    SCI_MEMCPY(&(state_info.number.sender),&(order_info_ptr->sender),(sizeof(uint8)*MMISMS_NUMBER_MAX_LEN));
    state_info.flag.is_locked = order_info_ptr->is_lock;
    state_info.display_content.content_len = MIN(order_info_ptr->display_content.content_len, MMIMESSAGE_DIAPLAY_LEN );
    MMI_WSTRNCPY( state_info.display_content.content,
                MMIMESSAGE_DIAPLAY_LEN,
                order_info_ptr->display_content.content,
                state_info.display_content.content_len,
                state_info.display_content.content_len
                );

    state_info.flag.msg_type = order_info_ptr->msg_type;
    MMISMS_GetOrderIdByStorage(&old_order_id, (MMISMS_MSG_TYPE_E)state_info.flag.msg_type, MN_DUAL_SYS_MAX, MN_SMS_NO_STORED, state_info.record_id);
    if (PNULL == old_order_id ) //can't find the mms in order list by record id
    {
        ret_value = FALSE;
    }
    else
    {
        MMISMS_GetSmsInfoFromOrder(old_order_id, &old_state_info);
        state_info.flag.folder_type = order_info_ptr->folder_type;
        state_info.flag.is_search = old_state_info.flag.is_search;
        if (!(MMISMS_TYPE_WAP_OTA == old_state_info.flag.msg_type && MMISMS_MT_HAVE_READ == old_state_info.flag.mo_mt_type))
        {//如果是已读ota，就不需要更新了
            //MMISMS_UpdateMMSMarkedFlag(old_order_id);
            MMISMS_UpdateSmsInfoInOrder(old_order_id,&state_info); 
            //for msg delete all 11/02/18 信息全部删除使用
#ifdef MMI_SMS_DELALL_SUPPORT            
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            if (MMK_IsOpenWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID))
            {
                MMK_SendMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
            }
#else
            if (MMK_IsOpenWin(MMISMS_MSGDEL_ALL_WIN_ID))
            {
                MMK_SendMsg(MMISMS_MSGDEL_ALL_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
            }
#endif            
#endif
            MMISMS_SetNeedSort(TRUE);
            MAIN_SetMsgState(TRUE);
        }        
        ret_value = TRUE;        
    }
#ifdef MMI_SMS_CHAT_SUPPORT
    if (MMISMS_TYPE_MMS == order_info_ptr->msg_type)
    {
        MMIAPISMS_UpdateChatShow(order_info_ptr);
    }
#endif

    return ret_value;
}

/*****************************************************************************/
// Description : 更新彩信图标
// Global resource dependence : 
// Author: minghu.mao
// Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC void MMIAPISMS_UpdateMsgStatus(void)
{
    //MMISMS_HandleOrderChangeCallBack();
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);
    MAIN_SetMsgState(TRUE);
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : 当MMS全部删除完毕后，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMMSDone (MMISMS_BOX_TYPE_E    box_type)
{
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_DeleteAllMMSDone enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_725_112_2_18_2_52_27_8,(uint8*)"");

    //删除完毕
    if (0 == MMISMS_GetBoxTotalNumberFromOrder(box_type))
    {
       MMISMS_OpenMsgBox(TXT_DELETED, IMAGE_PUBWIN_SUCCESS); 
       MMIPUB_CloseWaitWin(MMISMS_DELETE_ANIM_WIN_ID);
    }
    else
    {
        MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT);  
    }
    return TRUE;
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : 当MMS排序完成时，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_MMSOrderDone (void)
{
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_MMSOrderDone enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_747_112_2_18_2_52_28_9,(uint8*)"");
    MMISMS_SetMMSOrderReady(TRUE);

    return TRUE;
}
/*****************************************************************************/
//     Description : MMS是否已经排序完成
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_IsMMSOrderReady(void)
{
    return MMISMS_IsMMSOrderReady();
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : 当MMS初始化完成时，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC void MMIAPISMS_MMSInitDone(void)
{
    //SCI_TRACE_LOW:"MMISMS:MMIAPISMS_MMSInitDone"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_768_112_2_18_2_52_28_10,(uint8*)"");
    if (MMISMS_IsOrderOk())//短信排序完，主动触发，否则由短信触发
    {
        BOOLEAN is_open_waiting = MMK_IsOpenWin(MMISMS_WAITING_WIN_ID);//在MMISMS_HandleSortTimer会关闭MMISMS_WAITING_WIN_ID，所以需要记录

        MMISMS_HandleSortTimer(0, NULL);

        if (!is_open_waiting)//如果开启等待窗口，不需要发消息
        {
            MMISMS_HandleOrderChangeCallBack();
        }
    }
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : 当打开新彩信窗口时，按下ok键调用此函数进入收件箱。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_OpenMtBox(void)
{
    MMIAPISMS_ExitFromSMS();
    MMISMS_OpenSmsBox( MMISMS_BOX_MT );
    return TRUE;
}

/*****************************************************************************/
//     Description : to enter the window for writing SMS 
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for other module
/*****************************************************************************/
void MMIAPISMS_AnswerMessage(
                               MN_DUAL_SYS_E    dual_sys,           //IN:
                               MMI_STRING_T        *sms_init_ptr,        //IN:
                               uint8            *dest_addr_ptr,        //IN:
                               uint8            dest_addr_len        //IN:
                               )
{    
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();

    if (MMISMS_IsOrderOk()&& MMISMS_IsMMSOrderReady())
    {
        if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
        {           
            MMISMS_WRITE_MSG_INFO_T mmisms_write_info = {0};
            uint16 length = 0;
            mmisms_write_info.dest_addr_ptr = SCI_ALLOCAZ( dest_addr_len + 1 );
            if(PNULL == mmisms_write_info.dest_addr_ptr)
            {
                return;
            }    
               
            SCI_MEMCPY((void*)mmisms_write_info.dest_addr_ptr, (void*)dest_addr_ptr, dest_addr_len);
            mmisms_write_info.dest_addr_len = dest_addr_len;
			if(sms_init_ptr != PNULL)
			{
                length = sms_init_ptr->wstr_len;
                mmisms_write_info.sms_str.wstr_ptr = SCI_ALLOCAZ( (length + 1) * sizeof(wchar) );
                if(PNULL == mmisms_write_info.sms_str.wstr_ptr)
                {
                    SCI_FREE(mmisms_write_info.dest_addr_ptr);
                    return;
                } 
                SCI_MEMCPY((void*)mmisms_write_info.sms_str.wstr_ptr , (void*)sms_init_ptr->wstr_ptr,  (length + 1) * sizeof(wchar));
                mmisms_write_info.sms_str.wstr_len = length;
            }
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, MMISMS_OpenWriteWindowExt,(DPARAM)&mmisms_write_info,sizeof(MMISMS_WRITE_MSG_INFO_T));
        }
        else
        {
            MMISMS_WriteNewMessage(
                dual_sys,
                sms_init_ptr,
                FALSE,
                dest_addr_ptr,
                dest_addr_len
                );
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
    }
}

/*****************************************************************************/
//     Description : Notify MN to save mms push.
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for udisk module
/*****************************************************************************/
void MMIAPISMS_SetSavePush( BOOLEAN is_save )
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        MNSMS_SetSaveMMSPushEx(i, is_save);
    }

}

/*****************************************************************************/
//     Description : Begin read mms push for mms module.
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for udisk module
/*****************************************************************************/
BOOLEAN MMIAPISMS_ReadMmsPush( void )
{
    BOOLEAN ret_val = FALSE;

    ret_val = MMISMS_ReadMMSPushFromUnhandledList();

    return ret_val;
}

/*****************************************************************************/
//     Description : Open message main menu
//    Global resource dependence : 
//  Author:Lin.Lin
//    Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISMS_OpenMessageMainMenuWin(void)
{
    MMK_CloseWin(MMIMSG_MAIN_MENU_WIN_ID);
    MMIAPISMS_CloseAllSMSWindow();

    MMIAPISMS_OpenMainMenuWin();
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : Get the sms sort timer Id.
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISMS_GetSmsSortTimerId(void)
{
    uint8 ret_val = 0;

    ret_val = MMISMS_GetSmsSortTimerId();
    
    return ret_val;
}

/*****************************************************************************/
//  Description :Insert the mms/wap/ota into sms list. 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleSortTimer(uint8 timer_id, uint32 param)
{
    MMISMS_HandleSortTimer(timer_id, param);
}

/*****************************************************************************/
//  Description: init sms order list
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitSmsOrder(void)
{
    MMISMS_InitSmsOrder();
}

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadSmsCount(MN_DUAL_SYS_E dual_sys)
{
    uint16 ret_val = 0;
    
    ret_val = MMISMS_GetTobeReadSmsCount(dual_sys);

    return ret_val;
}

/*****************************************************************************/
//     Description : to append the info to list_box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetListCurrentItemContent(
                                                MMI_CTRL_ID_T ctrl_id,
                                                uint8 *str_len_ptr,
                                                wchar *str_ptr
                                                )
{
    MMISMS_GetListCurrentItemContent(
        ctrl_id,
        str_len_ptr,
        str_ptr
        );
}

#endif /* Reduce Code size */
/*****************************************************************************/
//  Description :clear dest number 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearDestNum(void)
{
    MMISMS_ClearDestNum();
}

#if defined(MMI_VCARD_SUPPORT) || defined(MMI_VCALENDAR_SUPPORT)
/*****************************************************************************/
// Description : to show the new Vcard prompt
//               (参考函数:MMISMS_ShowNewMsgPrompt)
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ShowNewVcardOrVcalendarPrompt(BOOLEAN is_play_ring,MMIVC_SMS_SEND_TYPE_E type)
{
    MMISMS_ShowNewVcardOrVcalendarPrompt(is_play_ring,type);
}
#endif

/*****************************************************************************/
//     Description : API for is in sms moudle window
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsInSMSWin(void)
{
    return MMISMS_IsInSMSWin();
}

/*****************************************************************************/
//  Description : MMIAPISMS_RefreshSMS
//  Global resource dependence : 
//  Author: wancna.you
//RETURN: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_RefreshSMS(MN_DUAL_SYS_E dual_sys)
{
    if (MMISMS_RefreshSMS(dual_sys))
    {
        MMISMS_InitSmsOrder();

        MMISMS_InitSmsList();

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//     Description : set default preset SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDefaultPresetSms(BOOLEAN is_clear)
{
    MMISMS_SetDefaultPresetSms(is_clear);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetMaxSimSmsNum(MN_DUAL_SYS_E dual_sys, uint8 sim_sms_num)
{
    MMISMS_SetMaxSimSmsNum(dual_sys, sim_sms_num);
}

/*****************************************************************************/
//     Description : to get the count of msg which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_E dual_sys, MMISMS_MSG_TYPE_E msg_type)
{
    return MMISMS_GetTobeReadMsgCount(dual_sys, msg_type);
}

/*****************************************************************************/
//     Description : to get the count of undownload msg
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_GetUnDownloadMsgCount(dual_sys);
}

/*****************************************************************************/
//     Description : to get the count of total message which is to be read(包括未读sms，mms，ota，push...)
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadMessageCount(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_GetTobeReadMessageCount(dual_sys);
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//    Description : to create URL option WIN for MMS mdu
//    Global resource dependence : 
//    Author:Kun.Yu
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateWinURLOpt( ADD_DATA add_data )
{
    MMISMS_Create_Win_URL_Opt( add_data );
}

/*****************************************************************************/
//     Description : get move all flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move all SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetMoveAllFlag(void)
{
    return (MMISMS_GetMoveAllFlag());
}

/*****************************************************************************/
//     Description : to get the resend count
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSBusy( void )
{
    if(MMIAPISMS_GetMoveAllFlag() ||
       MMIAPISMS_IsOrderOk())
    {
        return TRUE;
    }
    return FALSE;
    //return MMK_IsOpenWin(MMISMS_MOVE_ANIM_WIN_ID);
}

#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : Get the push sms flag.
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetPushSmsBrowserInfo(void)
{
    return MMISMS_GetPushSmsBrowserInfo();
}

/*****************************************************************************/
//     Description : handle left and right msg in show SMS window
//    Global resource dependence : 
//  Author: minghu.mao
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleLeftRightMsgInShowSmsWin(
                                                     MMI_WIN_ID_T        win_id,        //IN:
                                                     MMI_MESSAGE_ID_E    msg_id        //IN:
                                                     )
{
    HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
}

#ifdef MMI_SMS_RETRY_SUPPORT
/*****************************************************************************/
//     Description : to set the resend count
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetResendCount( 
                               BOOLEAN     is_set_resend_count
                               )
{
    MMISMS_SetDeliverResendFlag(is_set_resend_count);
}

/*****************************************************************************/
//     Description : to get the resend count
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetResendCount( void )
{
    return MMISMS_GetDeliverResendFlag();
}
#endif

/*****************************************************************************/
//     Description : Set Need Sort
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetNeedSort( BOOLEAN is_sort )
{
   MMISMS_SetNeedSort(is_sort);
   return;
}

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
//     Description : async search sms
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_StartLocalSearch(MMI_STRING_T *key_word_str,BOOLEAN is_mms_only)
{
    if (MMISMS_IsOrderOk()  && MMISMS_IsMMSOrderReady())
    {
        MMISMS_HandleSearchSms(key_word_str,is_mms_only);
    }
}

/*****************************************************************************/
// 	Description : 本地搜索退出，短信模块释放模块内部资源
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_FreeLocalSearchResource(void)
{    
    
}

/*****************************************************************************/
//     Description : stop async search sms
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_StopSearchSms(void)
{
    MMISMS_StopSearchSms();
}
#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : get win font size
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMIAPISMS_GetWinFont(void)
{
    return (MMISMS_GetWinFont());
}
#endif

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //delete all mail from message list
/*****************************************************************************/
//     Description : delete all mail from message list
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为mail模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMailMsg(void)
{

    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_DeleteAllMailMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_1193_112_2_18_2_52_29_11,(uint8*)"");

    MMISMS_DeleteAllMail(MMISMS_TYPE_MAIL);

    return TRUE;

}
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : 设置当前的字体大小
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetWinFont(GUI_FONT_T  font)
{
    MMISMS_FONT_E font_size = MMISMS_FONT_NORMAL;
    switch(font) 
    {
    case MMI_DEFAULT_BIG_FONT:
        font_size = MMISMS_FONT_BIG;
    	break;
    case MMI_DEFAULT_SMALL_FONT:
        font_size = MMISMS_FONT_SMALL;
        break;
    default:
        font_size = MMISMS_FONT_NORMAL;
        break;
    }
    MMISMS_SetFontSize(font_size);
}

#endif

/*****************************************************************************/
//  Description : response sms control
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_ResponseSmsCtl(MN_DUAL_SYS_E dual_sys,
                                                                                        MMI_STRING_T *alert_str_ptr,
                                                                                        MMI_STRING_T *display_str_ptr,
                                                                                        MN_CALLED_NUMBER_T *dest_me,
                                                                                        MN_CALLED_NUMBER_T *dest_sc,
                                                                                        BOOLEAN is_need_modified)
{
    return MMISMS_ResponseSmsCtl(dual_sys, alert_str_ptr, display_str_ptr, dest_me, dest_sc, is_need_modified);
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsSMSRefresh
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsSMSRefresh(MN_DUAL_SYS_E dual_sys, BOOLEAN is_sms_refresh)
{
    MMISMS_SetIsSMSRefresh(dual_sys, is_sms_refresh);
}

/*****************************************************************************/
//     Description : MMIAPISMS_OrderInit
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OrderInit(void)
{
    MMISMS_InitSortStatusAccordingToSetting();
    MMISMS_FreeSmsOrder();

    MMISMS_SetMMSOrderReady(FALSE);

    MAIN_SetMsgState(TRUE);
}

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_CheckTimerMSG(void)
{
#ifdef MMI_TIMERMSG_SUPPORT
    return MMISMS_TIMERMSG_CheckTimerMSG(FALSE);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : read new sms
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ReadNewSms(
                                 MMISMS_BOX_TYPE_E        box_type
                                        )
{
    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, (MMISET_PRIVACY_CALLBACK_PFUNC)MMISMS_ReadNewSmsEx, &box_type, sizeof(box_type));
}
#if 0
// no use
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void SMS_ValidatePrivacyReadNewSms(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            box_type = *(MMISMS_BOX_TYPE_E*)param_ptr;
        }
        MMISMS_ReadNewSms(box_type);
    }
}
#endif

/*****************************************************************************/
//  Description : 得到第一条未读短信号码
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPISMS_GetOriginNumByPos(                        //RETURN: the length of num_ptr
                                      MN_DUAL_SYS_E     *dual_sys_ptr,  //out
                                      uint8                *num_ptr,        //OUT:
                                      uint8                 max_num_len    //IN:                                          
                                      )
{
    uint8 len = 0;
    uint8 i = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
	//  g_mmisms_global.operation.cur_order_index = 0;
    cur_order_id = MMISMS_GetFirstUnreadMsgId();
 
    if (PNULL == cur_order_id)
    {
        *dual_sys_ptr = MN_DUAL_SYS_MAX;
        return 0;
    }

    if (PNULL == num_ptr || PNULL == dual_sys_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NewSms_GetOriginNumByPos PNULL == num_ptr || PNULL == dual_sys_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_1344_112_2_18_2_52_29_12,(uint8*)"");
        return len;
    }
    
    if (cur_order_id->flag.msg_type == MMISMS_TYPE_SMS)
    {
        len = MMIAPICOM_GenDispNumber(
            MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton),
            MIN(MN_MAX_ADDR_BCD_LEN, cur_order_id->number.bcd_number.number_len),
            cur_order_id->number.bcd_number.number,
            num_ptr,
            max_num_len
            );        
    }
    else
    {
        len = strlen((char *)cur_order_id->number.sender);
        //changed by minghu cr128630
        for (i = 0; i < len; i++)
        {
            if (MMIAPICOM_IsTelephoneNum(*(cur_order_id->number.sender+i)))
            {
                *(num_ptr + i) = *(cur_order_id->number.sender + i);
            }
            else
            {
                len = i;
                break;
            }
        }
        *(num_ptr + len) = 0;
    }
    *dual_sys_ptr = (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys;
	
    return (len);
}

/*****************************************************************************/
//  Description : 得到第一条未读短信的日期和时间
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetOriginTimeAndDate(
                                        wchar    *date_ptr,        //OUT:
                                        wchar    *time_ptr        //OUT:
                                        )
{

    MMISMS_ORDER_ID_T   cur_order_id = PNULL;

    if (PNULL == date_ptr || PNULL == time_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NewSms_GetOriginTimeAndDate PNULL == date_ptr || PNULL == time_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_1401_112_2_18_2_52_29_13,(uint8*)"");
        return;
    }
    
    cur_order_id = MMISMS_GetFirstUnreadMsgId();
    
    if (PNULL == cur_order_id)
    {
        return;
    }

    MMISMS_GetDateTimeStringByOrder(cur_order_id,TRUE,TRUE,FALSE,date_ptr);
    MMISMS_GetDateTimeStringByOrder(cur_order_id,FALSE,FALSE,TRUE,time_ptr);
    
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//     Description : to Set the status report flag
//    Global resource dependence : g_mmisms_global
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetStatusReportFlag(BOOLEAN status_report_flag)
{
    MMISMS_SetStatusReportFlag(status_report_flag);
}

/*****************************************************************************/
//     Description : to set the save position
//    Global resource dependence : g_mmisms_global
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetSavePos(MMISMS_SMS_SAVEPOS_TYPE_E pos)
{
    MMISMS_SetSavePos(pos);
}

#endif


/*****************************************************************************/
//  Description : send the SMS for other modules
//  Global resource dependence :  
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SendSmsForOther(MMISMS_SEND_DATA_T *send_data_ptr)
{
    return MMISMS_SendSmsForOther(send_data_ptr);
}

#ifdef MMI_INDIAN_SOS_SUPPORT
/*****************************************************************************/
//  Description : send one or more sms messages
//  Global resource dependence :  
//  Author:Yongli Tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MultiSendSms(MMISMS_MULTI_SEND_DATA_T *send_data_ptr)
{
    return MMISMS_MultiSendSms(send_data_ptr);
}
#endif

/*****************************************************************************/
//  Description : Select SIM API of SMS app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC	MMISMSSelectSIMWinCallback,
                                        ADD_DATA    add_data,
                                        BOOLEAN  psReady
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMISMS_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    if (psReady)
    {
        caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
    }
    else
    {
        caller_data.append_type = (uint32)MMISET_APPEND_SIM_OK;
    }
    caller_data.callback_func =MMISMSSelectSIMWinCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;

    caller_data.publist_id = MMISMS_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);

#endif
    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : is sending sms
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSendingSMS( void )
{
    if (MMISMS_GetIsSendingSms() && !MMISMS_IsSendListEmpty())
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : to handle the create select word win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateSelectWordWin(MMI_HANDLE_T inserted_ctrl)
{
    MMISMS_CreateSelectWordWin(inserted_ctrl);
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT                
/*****************************************************************************/
// 	Description : get contact info
//	Global resource dependence : 
//  Author: rong.gu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetContactInfo(
                                  uint32 handle,
                                  MMI_STRING_T    *contact_info_ptr,    //OUT:
                                  uint16        max_len                //IN:
                                  )
{
    MMISMS_GetContactInfo(handle, contact_info_ptr,max_len);
}

#if 0
/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleSendOperation(
                               MN_DUAL_SYS_E    dual_sys,
                               MMI_WIN_ID_T    win_id,    //IN:
                               BOOLEAN        is_re_send 
                               )
{
    MMISMS_HandleSendOperation(dual_sys,win_id,is_re_send);
}           
#endif

/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SendSms(MN_DUAL_SYS_E    dual_sys,BOOLEAN is_re_send)
{
    MMISMSAPP_SendSmsWithGlobalContent(dual_sys,is_re_send);
}

/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SaveAsDraft(MN_DUAL_SYS_E    dual_sys,BOOLEAN is_replace,BOOLEAN is_timer_msg)
{
    SMS_SAVE_OPERATE_DATA_T op_data={0};

    op_data.dual_sys = dual_sys;
    op_data.is_needLoadSaved = FALSE;
    op_data.is_use_global_data = TRUE;
    op_data.is_timer_msg = is_timer_msg;

#ifdef MMI_TIMERMSG_SUPPORT
    if (is_timer_msg)
    {
       if(!MMISMS_TIMERMSG_CheckTimerSend(TRUE))
       {
            return;
       }
    }
#endif    
    
    if (is_replace)
    {
        MMISMSAPP_ReplaceSms(op_data,PNULL,PNULL,PNULL);
    }
    else
    {
        MMISMSAPP_SaveSms(op_data,PNULL,PNULL,PNULL);
    }
    
}

/*****************************************************************************/
//     Description : to delete request to MN
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_DeleteReqToMN(
                            MMISMS_BOX_TYPE_E        type,        //IN:
                            MMISMS_STATE_T*       order_id,    //IN:
                            uint32    oper_status    //IN:
                            )
{
    return MMISMS_DeleteReqToMN(type,(MMISMS_ORDER_ID_T)order_id,(MMISMS_OPER_STATUS_E)oper_status);
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE

/*****************************************************************************/
//     Description : check me have enough space for save sms
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MEHaveEnoughSpace(void)
{
    return MMISMS_MEHaveEnoughSpace();
}

#endif

/*****************************************************************************/
//     Description : to save the content which user input
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetMessageContent( 
                                        uint16    length,            //IN:
                                        wchar     *content_ptr,    //IN:
                                        BOOLEAN   is_need_packet,
                                        MMISMS_SMSEDIT_CONTENT_T *message_info
                                        )
{
    MMISMS_SetMessageContent(length,content_ptr,is_need_packet,message_info);
}
#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
// 	Description : get timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsTimerMode(void)
{
    return MMISMS_GetIsTimerMode();
}

/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsTimerMode(BOOLEAN is_timer_mode)
{
    MMISMS_SetIsTimerMode(is_timer_mode);
}

/*****************************************************************************/
// 	Description : create a window according to timer msg index
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note: if index == MMISMS_TIMERMSG_MAX_ITEM create a timer msg interface with current time
//        otherwise   create a timer msg interface with timer msg info from NV
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateTimerMsgWin(void)
{
    MMISMS_EnterTimerMsgMainWin();
}

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_TIMERMSG_CheckTimerMSG(BOOLEAN is_direct)
{
    return MMISMS_TIMERMSG_CheckTimerMSG(is_direct);
}

/*****************************************************************************
// 	Description : set box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMIAPISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_TYPE_E box_type)
{
    MMISMS_TIMERMSG_SetCurrBoxType(box_type);
}
#endif

#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSendDualSys(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_SetCurSendDualSys(dual_sys);
}


/*****************************************************************************/
//     Description : to clear the id of destinations in g_mmisms_deliver
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearDestId(void)
{
    MMISMS_ClearDestId();
}
#endif
/*****************************************************************************/
//  Description : init update info
//  Global resource dependence : g_mmisms_global
//  Author:rong.gu
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitUpdateInfo(void)
{
    MMISMS_InitUpdateInfo();
}

#if 0
/*****************************************************************************/
//  Description : to set the resend flag
//  Global resource dependence : g_mmisms_deliver
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDeliverResendFlag(BOOLEAN     is_resend)
{
    MMISMS_SetDeliverResendFlag(is_resend);
}
#endif

#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description :发送Vcard短信
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcard(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_WintabSendVcard(dual_sys);
}
#endif
#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description :发送Vcalendar短信
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcalendar(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_WintabSendVcard(dual_sys);
}
#endif


/*****************************************************************************/
//     Description : to get the length according the alpha of message 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetLenByAlpha(
                                 MN_SMS_ALPHABET_TYPE_E    alpha,            //IN:
                                 uint8                     *split_len_ptr,    //OUT: the len to split message
                                 uint8                     *limit_len_ptr,    //OUT: the limit len
                                 BOOLEAN                   is_head_has_port
                                 )
{
    MMISMS_GetLenByAlpha(alpha,split_len_ptr,limit_len_ptr,is_head_has_port);
}

/*****************************************************************************/
//     Description : to set the g_mmisms_global.operation.cur_order_index
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void    MMIAPISMS_SetCurOrderArrIndex(int index)
{
    MMISMS_SetCurOrderArrIndex(index);
}

/*****************************************************************************/
//     Description : Compute Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC MMISMS_STATE_T* MMIAPISMS_GetCurOperationOrderId(void)
{
    return MMISMS_GetCurOperationOrderId();
}

#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMIAPISMS_TIMERMSG_GetCurEvent(void)
{
    return MMISMS_TIMERMSG_GetCurEvent();
}
#endif
#if 0
/*****************************************************************************/
//     Description : set dest dual sys
//    Global resource dependence : g_mmisms_deliver
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
void MMIAPISMS_SetDestDualSys( MN_DUAL_SYS_E dual_sys )
{
    MMISMS_SetDestDualSys(dual_sys);
}
#endif
/*****************************************************************************/
//     Description : open the SMS sending window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenSendingWindow(MN_DUAL_SYS_E  dual_sys,MMI_STRING_T    *prompt_ptr)
{
    MMISMS_OpenSendingWindow(dual_sys,prompt_ptr);
}

/*****************************************************************************/
//     Description : get the sms is in the draft box
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MsgIsInDraftBox(void)
{
    return MMISMS_MsgIsInDraftBox();
}

/*****************************************************************************/
//     Description : to get the destation number according the pointed position
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetPointedDestNum(                            //RETURN: 
                                        uint16     index,            //IN: start from 0
                                        uint8        *num_len_ptr,    //OUT:
                                        uint8        *num_ptr        //OUT:
                                        )
{
    return MMISMS_GetPointedDestNum(index,num_len_ptr,num_ptr);
}
#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSaveDualSys(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_SetCurSaveDualSys(dual_sys);
}
#endif

/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetCurReadMsgDualSys(void)
{
    return MMISMS_GetCurReadMsgDualSys();
}



#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsChatEditMMS(void)
{
    BOOLEAN ret = FALSE;
    if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) && (MMISMS_INSERT_INIT != MMISMS_GetChatEditInsertOpType()))
    {
        ret = TRUE;
    }
    return ret;
}
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
/*****************************************************************************/
//     Description : Set Answer SMS Flag
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsAnwserSMS(BOOLEAN is_answer)
{
    g_mmisms_global.is_answer_sms = is_answer;
}
#endif
#endif

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenPushMsgBox(void)
{
    MMIAPISMS_ExitFromSMS();
    MMISMS_EnterPushMessageWin();
}
#endif

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : SMS备份函数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_IKeyBackup(MMIIKB_SIG_BACKUP_ITEM_DATA_REQ_T *backup_data_ptr)
{
    MMISMS_IKeyBackup(backup_data_ptr);
}
/*****************************************************************************/
//  Description : SMS还原函数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_IKeyRestore(MMIIKB_SIG_RESTORE_ITEM_DATA_REQ_T *restore_data_ptr)
{
    MMISMS_IKeyRestore(restore_data_ptr);
}
#endif
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : get read msg content
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetReadMsgContent(
                                    MMISMS_READMSG_CONTENT_T *read_content_ptr      //OUT
                                    )
{
    MMISMS_GetReadMsgContent(read_content_ptr);
}
#endif

/*****************************************************************************/
//     Description : to get the amount of the destination number
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8   MMIAPISMS_GetDestCount( void )
{
    return MMISMS_GetDestCount();
}

/*****************************************************************************/
//  Description : get the edit content from global API
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:该结构在sms编辑时候用
/*****************************************************************************/
PUBLIC MMISMS_SMSEDIT_CONTENT_T * MMIAPISMS_GetEditContentFromGlobal( void )
{
    return MMISMS_GetEditContentFromGlobal();
}

/*****************************************************************************/
//  Description : Select SIM API of SMS app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISMS_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC	MMISMSSelectSIMWinCallback,
                                        ADD_DATA    add_data
                                        )
{
    return MMISMS_SelectSIMFunc(MMISMSSelectSIMWinCallback,add_data,TRUE);
}

/*****************************************************************************/
//     Description : get dest dual sys
//    Global resource dependence : 
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E    MMIAPISMS_GetDestDualSys( void )
{
    return MMISMS_GetDestDualSys();
}

/*****************************************************************************/
//     Description : get current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetCurSendDualSys(void)
{
    return MMISMS_GetCurSendDualSys();
}

/* ----------------------------------------------------------------------------
* Function Name: MMISMS_GetPBNameByNumber
* Purpose:通过电话得到电话所属姓名
* Input:  uint8 *number_ptr:电话号码指针
MMI_STRING_T *name_pt:姓名结构
* Output: BOOLEAN.
* Returns:
* Author:
* ----------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPISMS_GetNameByNumberFromPB(uint8 *number_ptr, MMI_STRING_T *name_ptr, uint16 max_name_len)
{
    return MMISMS_GetNameByNumberFromPB(number_ptr,name_ptr,max_name_len);
}

/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_EnterSmsBox(
                       MMISMS_BOX_TYPE_E        box_type
                       )
{
    MMISMS_EnterSmsBox(box_type);
}

/*****************************************************************************/
//     Description : to get the count of msg(sms mms wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetFirstUnreadMsgId(void)
{
    return MMISMS_GetFirstUnreadMsgId();
}

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to get the count of msg(wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetFirstUnreadPushMsgId(void)
{
    return MMISMS_GetFirstUnreadPushMsg();
}

/*****************************************************************************/
//     Description : open push box
//    Global resource dependence :
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenPushBox(void)
{
    MMISMS_EnterPushMessageWin();
}
#endif


/*****************************************************************************/
//     Description : get the count of msg(sms\wap push\mms\..)for widget
//    Global resource dependence : 
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetAllUnreadMsgCount()
{
    return MMISMS_GetAllUnreadMsgCount();
}

/*****************************************************************************/
//     Description : to get the save position
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_SMS_SAVEPOS_TYPE_E MMIAPISMS_GetSavePos(void)
{
    return MMISMS_GetSavePos();
}

/*****************************************************************************/
//     Description : set the SMS num of the read message
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
PUBLIC MMISMS_BOX_TYPE_E MMIAPISMS_GetCurBoxType(void)
{
    return MMISMS_GetCurBoxType();
}

/*****************************************************************************/
//  Description :sms edit text property
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetEditTextProperty(MMI_CTRL_ID_T ctrl_id)
{
    MMISMS_SetEditTextProperty(ctrl_id);
}

/*****************************************************************************/
//  Description :SmsBCDToPbBCD
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMIAPISMS_SmsBCDToPbBCD(MMISMS_BCD_NUMBER_T sms_num)
{
    return MMISMS_SmsBCDToPbBCD(sms_num);
}

/*****************************************************************************/
//  Description :Get not download Sms Image-Id 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetUndownLoadImageId(void)
{
    return IMAGE_SMS_MMS_NOTIFY_NEW_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-Sms Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadSmsImageId(void)
{
    return IMAGE_SMS_NOT_READ_SMS_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-Mms Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadMmsImageId(void)
{
    return IMAGE_SMS_NOT_READ_MMS_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-WAPPUSH Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadWAPPUSHImageId(void)
{
    return IMAGE_SMS_WAPPUSH_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-WAPOTA Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadWAPOTAImageId(void)
{
    return IMAGE_SMS_WAPOTA_ICON;
}

/*****************************************************************************/
//  Description :Get FULL_MESSAGE Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetFullMessageImageId(void)
{
    return IMAGE_SMS_TITLE_ICON_FULL_MESSAGE;
}

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:wancan.you
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetLocalSettings(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_SetLocalSettings(dual_sys);
}
#endif

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:jian.ma
//    Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMIAPISMS_NV_DelAllSmsItem(                                            //RETURN:
                                MN_SMS_RECORD_ID_T    sms_in_me_valid_num    //IN:
                                )
{
    return MMISMS_NV_DelAllSmsItem(sms_in_me_valid_num);
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :delete all SMSCB item in NV by MMI
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_NV_DelAllSmsCBItem(                                            //RETURN:
                                uint16    smscb_in_me_valid_num    //IN:
                                )
{
    return MMISMS_NV_DelAllSmsCBItem(smscb_in_me_valid_num);
}
#endif

/*****************************************************************************/
//     Description :get normal close
//    Global resource dependence :  
//  Author:rong.gu
//    Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMIAPISMS_GetNormalClose(void)
{
    return g_mmisms_global.is_sms_normal_close;
}

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:rong.gu
//    Note:
/*****************************************************************************/    
PUBLIC void MMIAPISMS_SetNormalClose(BOOLEAN is_sms_normal_close)
{
    g_mmisms_global.is_sms_normal_close = is_sms_normal_close;
}

/*****************************************************************************/
//     Description : insert string to sms edit
//    Global resource dependence : none
//  Author:
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPISMS_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             )
{
    BOOLEAN result = FALSE;

#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if(MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
        result = GUIEDIT_InsertString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, wstr_ptr, wstr_len);                                            		
    }
    MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, MSG_FULL_PAINT, PNULL);
#else
    if(MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
    {
        result = GUIEDIT_InsertString(MMISMS_EDIT_SMS_TEXT_CTRL_ID, wstr_ptr, wstr_len);                                            		
    }
    MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MSG_FULL_PAINT, PNULL);
#endif

    //SCI_TRACE_LOW:"[MMISMS ATC] MMIAPISMS_InsertMessage result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_25180_112_2_18_2_55_12_841,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//     Description :  check and show new msg prompt when exsit
//    Global resource dependence : 
//  Author:taiping.lai
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_CheckAndShowNewMsg(void)
{
    if (MMK_IsOpenWin(MMISMS_NEWMSG_WIN_ID) 
        && !MMK_IsFocusWin(MMISMS_NEWMSG_WIN_ID))
    {
        MMK_SetWinPriority(MMK_ConvertIdToHandle(MMISMS_NEWMSG_WIN_ID), WIN_ONE_LEVEL);
        MMK_WinGrabFocus(MMISMS_NEWMSG_WIN_ID);
    }
#ifdef MMS_SUPPORT
    else if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID) 
        && !MMK_IsFocusWin(MMIMMS_NEWMSG_WIN_ID))
    {
        MMK_SetWinPriority(MMK_ConvertIdToHandle(MMIMMS_NEWMSG_WIN_ID), WIN_ONE_LEVEL);
        MMK_WinGrabFocus(MMIMMS_NEWMSG_WIN_ID);
    }
#endif
}

/*****************************************************************************/
//  Description : Enter Send Option Menu
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_EnterSendOptionMenu(uint32 send_type)
{
    MMIAPISMS_ExitFromSMS();
            
    if(MMK_IsOpenWin(MMISMS_SENDNUMOPTION_WIN_ID))
    {
        MMK_CloseWin(MMISMS_SENDNUMOPTION_WIN_ID);
    }
    MMISMS_SetDestDualSys(MN_DUAL_SYS_MAX);
    if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
    {
        MMIAPISET_ValidatePrivacy(
            MMISET_PROTECT_SMS,
            (MMISET_PRIVACY_CALLBACK_PFUNC)MMISMS_EnterSendNumOptWinExt,
            &send_type,
            sizeof(send_type)); 
    }
    else
    {
        MMISMS_EnterSendNumOptWin(send_type);
    }
}

/*****************************************************************************/
//  Description : Get the push sms SL Operation
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SL_OPERATION_E MMIAPISMS_GetPushSmsSLOperation(void)
{
    return MMISMS_GetPushSmsSLOperation();
}

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ResetVMInfo(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_ResetVMInfo(dual_sys);
}
#endif

#ifdef MMS_SUPPORT
/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_ReadMMSNotify(void)
{
    MMISMS_ORDER_ID_T   cur_order_id = PNULL;
    uint16 undownloadnum = 0;
    BOOLEAN ret = FALSE;
    undownloadnum = MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX);
    if(1 == undownloadnum)
    {
        cur_order_id = MMISMS_GetUndownLoadHeadOrder();     
        if(cur_order_id != PNULL)
        {           
            MMIAPIMMS_ReadMMS(cur_order_id->record_id);
            ret = TRUE;
        }      
        else
        {
            SCI_TRACE_LOW("MMISMS no undown mms");
            ret = FALSE;
        }
    }   
    else
    {
#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT
       MMISMS_EnterSmsBox(MMISMS_BOX_CHAT);
#else
       MMISMS_EnterSmsBox(MMISMS_BOX_MT);
#endif
       ret = TRUE;
    }
    return ret;
}

/*****************************************************************************/
//    Description : 
//    Global resource dependence :
//    Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetUndownLoadHeadOrder(void)
{
    return MMISMS_GetUndownLoadHeadOrder(); 
}
#endif

#ifdef MMI_SMS_SYN_SUPPORT

/*****************************************************************************/
//  Description : syn sms by bt
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_StartSynSMS(void)
{    
    int com_len = 8;
    uint8 str_com[9] ={65,84,77,82,78,84,13,10,0};  //ATMRNT

    MMISMS_SendRequestCom(str_com, com_len);

    Syn_end = FALSE;
}

/*****************************************************************************/
//  Description : get data by bt
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetData(const uint8 *data)
{
    MMISMS_GetSmsData(data);    
}
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Description : delete all bt mails
//    Global resource dependence : 
//  Author: 
//    Note: 为mail模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMapSyncInfo(void)
{

    MMISMS_DeleteAllBTMail();

    MMIAPISMS_SetIsUpdateInbox(TRUE);
    MMIAPISMS_SetIsUpdateSendbox(TRUE);
    MMIAPISMS_SetIsUpdateOutbox(TRUE);
    MMIAPISMS_SetIsUpdateDraft(TRUE);

    return TRUE;

}

/*****************************************************************************/
//     Description : get box total order
//    Global resource dependence : 
//  Author: 
//    Note: 为mail模块提供的接口函数。
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetBoxTotalNumber()
{
    return MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BT_BOX_ALL);
}

/*****************************************************************************/
//     Description : check me have enough space for save sms
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MEHaveEnoughSpace(void)
{
    return MMISMS_MEHaveEnoughSpace();
}

/*****************************************************************************/
//     Description : Set current operation is push.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetCurOperIsPush(BOOLEAN isPush)
{
    g_is_cur_oper_push = isPush;
}

/*****************************************************************************/
//     Description : check current operation is push.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsPush(void)
{
    return g_is_cur_oper_push;
}

/*****************************************************************************/
//     Description : Set current operation is push.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/

PUBLIC void MMIAPISMS_SetCurOperIsConnect(BOOLEAN isConnect)
{
    g_is_cur_oper_connect = isConnect;
}

/*****************************************************************************/
//     Description : check current operation is push.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsConnect(void)
{
    return g_is_cur_oper_connect;
}

/*****************************************************************************/
//     Description : Set current operation is push.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetCurOperIsDisconnect(BOOLEAN isDisconnect)
{
    g_is_cur_oper_disconnect = isDisconnect;
}

/*****************************************************************************/
//     Description : check current operation is push.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsDisconnect(void)
{
    return g_is_cur_oper_disconnect;
}

/*****************************************************************************/
//     Description : Set current operation is boot.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetCurOperIsBoot(BOOLEAN isBoot)
{
    g_is_cur_oper_boot = isBoot;
}

/*****************************************************************************/
//     Description : check current operation is boot.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsBoot(void)
{
    return g_is_cur_oper_boot;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetCurSMSIsRead
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurSMSIsRead(void)
{
    return g_is_cur_sms_read;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateOutbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateOutbox(void)
{
    return g_is_update_outbox;
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateOutbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateOutbox(BOOLEAN isUpdate)
{
    g_is_update_outbox = isUpdate;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateSendbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateSendbox(void)
{
    return g_is_update_sendbox;
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateOutbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateSendbox(BOOLEAN isUpdate)
{
    g_is_update_sendbox = isUpdate;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateInbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateInbox(void)
{
    return g_is_update_inbox;
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateInbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateInbox(BOOLEAN isUpdate)
{
    g_is_update_inbox = isUpdate;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateDraft
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateDraft(void)
{
    return g_is_update_draft;
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateDraft
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateDraft(BOOLEAN isUpdate)
{
    g_is_update_draft = isUpdate;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetSendAddrCount
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPISMS_GetSendAddrCount(void)
{
    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_GetSendAddrCount count = %d", s_send_sms_num_count);
    return s_send_sms_num_count;
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetSendAddrCount
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetSendAddrCount(uint8 count)
{
    s_send_sms_num_count = count;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsSendMultiSms
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsSendMultiSms(void)
{
    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_GetIsSendMultiSms count = %d", g_is_cur_send_multi_sms);
    return g_is_cur_send_multi_sms;
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsSendMultiSms
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsSendMultiSms(BOOLEAN isSend)
{
    g_is_cur_send_multi_sms = isSend;
}

/*****************************************************************************/
//     Description : MMIAPISMS_GetCurOperStatus
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_STATUS_E MMIAPISMS_GetCurOperStatus(void)
{
    return MMISMS_GetCurrentOperStatus();
}
/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_MapDisconnect(void)
{
    MMISMS_MapDisconnect();
}

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
LOCAL void freeallmapdata(void)
{
    SCI_TRACE_LOW("mmisms map freeallmapdata");
    ClearList(s_map_list_head);
    ClearList(s_map_bmessage_head);    
    s_sync_sms_count = 0;
    s_map_list_node_number = 1;
    s_map_bmessage_node_number = 1;
    s_map_bmessage_current_node_number = 1;
    SCI_FREE(s_sms_buff_ptr);
}

/*******************************************************************************/
//  Description:清除线性表L中的所有元素，即释放单链表L中所有的结点，使之成为一个空表
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL void ClearList(MAP_LIST_T *pHead) 
{ 
    MAP_LIST_T *pNext;            //定义一个与pHead相邻节点 
    MAP_LIST_T *pHead_backup = pHead;
  
    if(pHead == PNULL) 
    { 
        return; 
    } 
	pHead = pHead->next;
	
    while(pHead!= PNULL) 
    { 
        pNext = pHead->next;//保存下一结点的指针 
        SCI_FREE(pHead->data);
        SCI_FREE(pHead); 
        pHead = pNext;      //表头下移 
    } 

	pHead_backup->next = PNULL;
} 


/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL MAP_LIST_T* InitList(void) 
{   
    MAP_LIST_T *h;  /* h 保存表头节点的指针*/

    h=(MAP_LIST_T*)SCI_ALLOC_APPZ(MAP_LIST_LEN);

    if(PNULL == h)  /*分配空间并检测*/
        return 0;

    h->data = 0;    /*把表头节点的数据域置空*/
    h->next = PNULL; /*把表头结点的链域置空*/
    h->len = 0;
    return h;
}

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int InsertList(MAP_LIST_T *L, void* buff,uint16 buff_len)
{
    MAP_LIST_T *pInsert;
    MAP_LIST_T *pHead;
    MAP_LIST_T *pTmp;  //定义一个临时链表用来存放第一个节点 

    pHead = L;
    pTmp = pHead;


    pInsert = (MAP_LIST_T*)SCI_ALLOC_APPZ(MAP_LIST_LEN);//申请一个新节点 
    
    if(PNULL == pInsert)  /*分配空间并检测*/
    {
        return 0;
    } 

    pInsert->data = SCI_ALLOC_APPZ(buff_len+1);

	if(PNULL == pInsert->data)
	{
		SCI_FREE(pInsert);
		return 0;
	}

	SCI_MEMCPY(pInsert->data,buff,buff_len);
    pInsert->len = buff_len;
    pInsert->next = PNULL;
	
    while(pHead->next != PNULL)
    {
        pHead = pHead->next;
    }

    pHead->next = pInsert; //将链表末尾节点的下一结点指向新添加的节点 

    SCI_TRACE_LOW("mmisms map: InsertList, pInsert = %x", pInsert);
    
    L = pTmp;
    return 1;
}

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL BT_MSG_LIST_T* InitMsgHandleTableList(void) 
{   
    BT_MSG_LIST_T *h;  /* h 保存表头节点的指针*/

    h=(BT_MSG_LIST_T*)SCI_ALLOC_APPZ(BT_MSG_LIST_LEN);

    if(PNULL == h)  /*分配空间并检测*/
        return 0;

    SCI_MEMSET(h->handle, 0x00, HANDLE_LEN);
    h->is_extra_msg = FALSE;
    h->record_id    = 0;
    h->next         = PNULL; /*把表头结点的链域置空*/
    h->name = PNULL;
    h->name_len = 0;

    return h;
}


/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int InsertMsgHandleTableList(BT_MSG_LIST_T *L, char* handle, BOOLEAN is_extra_msg, wchar* name, uint32 name_len)
{
    BT_MSG_LIST_T *pInsert;
    BT_MSG_LIST_T *pHead;
    BT_MSG_LIST_T *pTmp;  //定义一个临时链表用来存放第一个节点 
    
    pHead = L;
    pTmp = pHead;
    
    pInsert = (BT_MSG_LIST_T*)SCI_ALLOC_APPZ(BT_MSG_LIST_LEN);//申请一个新节点 
    
    if(PNULL == pInsert)  /*分配空间并检测*/
    {
        return 0;
    } 
    
    if( PNULL != handle)
    {
        SCI_MEMCPY(pInsert->handle, handle,HANDLE_LEN-1);
    }

    pInsert->is_extra_msg = is_extra_msg;
    
    if( name_len > 0 )
	{
		pInsert->name = SCI_ALLOC_APPZ(sizeof(wchar)*name_len +1);
		if( PNULL == pInsert->name )
        {
            return 0;
        }
		SCI_MEMSET(pInsert->name, 0x00, sizeof(wchar)*name_len +1);    
		SCI_MEMCPY(pInsert->name, name, sizeof(wchar)*name_len);
		
		pInsert->name_len = name_len;
	}
    
    pInsert->next = PNULL;
    
    while(pHead->next != PNULL)
    {
        pHead = pHead->next;
    }
    
    pHead->next = pInsert; //将链表末尾节点的下一结点指向新添加的节点 
    
    SCI_TRACE_LOW("[MMISMS]  InsertMsgHandleTableList, pInsert = %x", pInsert);
    
    L = pTmp;
    return 1;
}

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL BOOLEAN DeleteMsgHandleTableList(BT_MSG_LIST_T *L, MMI_CTRL_ID_T record_id)
{
    BT_MSG_LIST_T *pHead;
    BT_MSG_LIST_T *pTmp;  //定义一个临时链表用来存放第一个节点 
	BT_MSG_LIST_T *pNext;            //定义一个与pHead相邻节点 
	
    pHead = L;
    pTmp = pHead;
	
    if(PNULL == pHead)  /*分配空间并检测*/
    {
	SCI_TRACE_LOW("[MMISMS] DeleteMsgHandleTableList, pHead = PNULL ");
    
        return FALSE;
    } 
	
	SCI_TRACE_LOW("[MMISMS] DeleteMsgHandleTableList, record_id = %d", record_id);
	
	while(PNULL != pHead->next)
    {
        pTmp = pHead;
        pHead = pHead->next;
        if(pHead->record_id == record_id)
        {
            if(PNULL != pHead->next) 
			{ 
				pNext = pHead->next;
				SCI_FREE(pHead); 
				pTmp->next = pNext; 
            }
            return TRUE;
        }
    }

    return FALSE;
}


/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice.Chen
//  Return : 
/*****************************************************************************/
LOCAL int AddRecordIdIntoMsgHandleTableList(BT_MSG_LIST_T *L, MMI_CTRL_ID_T record_id)
{
    BT_MSG_LIST_T *pHead;

    pHead = L;

    while(pHead->next != PNULL)
    {
        pHead = pHead->next;
    }

    pHead->record_id = record_id;
    return 0;
}

/*******************************************************************************/
//  Description : Get the handle and is_extra_message information by record id
//  Parameter: 
//  Global resource dependence :                                
//  Author: Cheney.Wu
//  Return : 
/*****************************************************************************/

PUBLIC BOOLEAN MMISMS_GetHandleInfoByRecordId(
                                              MMI_CTRL_ID_T record_id,
                                              char *handle,
                                              BOOLEAN *is_extra_msg
                                              )
{
    BT_MSG_LIST_T *pHead;
    
    pHead = s_message_table_head;
    
    if((PNULL == handle) || (PNULL == pHead ))
    {
        return FALSE;
    }

    while(pHead->next != PNULL)
    {
        pHead = pHead->next;
        if(pHead->record_id == record_id)
        {
            SCI_MEMSET(handle, 0x00, sizeof(handle));
            strncpy(handle, pHead->handle, HANDLE_LEN-1);
            *is_extra_msg = pHead->is_extra_msg;
            SCI_TRACE_LOW("[MMISMS] MMISMS_GetHandleInfoByRecordId, handle_number = %s, is_extra_msg = %d", handle, *is_extra_msg);
            return TRUE;
        }
    }
    
    return FALSE;
}

/*******************************************************************************/
//  Description : Get the name information by record id
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice.Chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetNameInfoByRecordId(
                                              MMI_CTRL_ID_T record_id,
                                              wchar **name,
                                              uint32 *name_len
                                              )
{
    BT_MSG_LIST_T *pHead;
    
    pHead = s_message_table_head;
    
    if(PNULL == pHead )
    {
        return FALSE;
    }

    while(pHead->next != PNULL)
    {
        pHead = pHead->next;
//        SCI_TRACE_LOW("[MMISMS] pHead->record_id = %d, record_id = %d, handle_number = %s", pHead->record_id, record_id, handle);
        
        if( (pHead->record_id == record_id) && ( pHead->name_len > 0 ) )
        {
			if( PNULL == *name )
			{
				*name = SCI_ALLOC_APP(sizeof(wchar)*(pHead->name_len + 1));
			}
			if (PNULL == *name)
			{
				SCI_TRACE_LOW("MMISMS_GetNameInfoByRecordId alloc fail");
				return FALSE;
			}
			
			SCI_MEMSET(*name, 0x00, sizeof(wchar)*(pHead->name_len +1));    
			SCI_MEMCPY(*name, pHead->name,  sizeof(wchar)*(pHead->name_len +1));
			
			*name_len = pHead->name_len;
			
			//            SCI_TRACE_LOW("[MMISMS] handle_number = %s, is_extra_msg = %d", handle, is_extra_msg);
            return TRUE;
        }
    }
    
    return FALSE;
}


/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC int MMIAPISMS_AddRecordIdIntoMsgHandleTableList(MMI_CTRL_ID_T record_id)
{
    return AddRecordIdIntoMsgHandleTableList(s_message_table_head, record_id);
}

/*******************************************************************************/
//  Description:判断单链表是否为空
//  Parameter: 
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
LOCAL int IsTableListEmpty(BT_MSG_LIST_T *L)
{
    return(PNULL == (L->next));
}

/*******************************************************************************/
//  Description:清除线性表L中的所有元素，即释放单链表L中所有的结点，使之成为一个空表
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL void ClearTableList(BT_MSG_LIST_T *pHead) 
{ 
    BT_MSG_LIST_T *pNext;            //定义一个与pHead相邻节点 
    BT_MSG_LIST_T *pHead_backup = pHead;
  
    if(pHead == PNULL) 
    { 
        return; 
    } 
	pHead = pHead->next;
	
    while(pHead!= PNULL) 
    { 
        pNext = pHead->next;//保存下一结点的指针 
        SCI_FREE(pHead); 
        pHead = pNext;      //表头下移 
    } 

	pHead_backup->next = PNULL;
} 

#if defined(TRACE_MAP_BUFFER)
LOCAL  uint16 cache_buf_serial_index = 0;
/*****************************************************************************/
//  Description : combile path
//  Global resource dependence : 
//  Author:cheney.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CombinePath(
                         wchar      *path_ptr,          //in/out:may PNULL
                         uint16     path_len,           //in/out:may PNULL,path_ptr不为空时,此处必须填max len
                         wchar      *name_ptr,          //in/out:may PNULL,don't include suffix
                         uint16     name_len,           //in/out:may PNULL,name_ptr不为空时,此处必须填max len
                         wchar      *suffix_ptr,        //in/out:may PNULL,include dot
                         uint16     suffix_len,         //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
                         wchar      *full_path_ptr,     //out
                         uint16     *full_path_len_ptr  //out
                         )
{
    BOOLEAN result = FALSE;
    wchar   *wchar_ptr = PNULL;
    uint16  temp_len = 0;

    if (PNULL == path_ptr || PNULL == name_ptr || 0 == path_len || 0 == name_len || PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return result;
    }

    MMI_WSTRNCPY(full_path_ptr, *full_path_len_ptr , path_ptr, path_len, path_len);
    temp_len += path_len;
    wchar_ptr = (uint16 *)(full_path_ptr + temp_len);

    *wchar_ptr = MMIFILE_SLASH;
    temp_len += 1;
    
    MMI_WSTRNCPY(full_path_ptr + temp_len, *full_path_len_ptr , name_ptr, name_len, name_len);
    temp_len += name_len;

    if ((suffix_ptr != PNULL) && (suffix_len > 0))
    {
        wchar_ptr = (uint16 *)(full_path_ptr + temp_len);
        *wchar_ptr = MMIFILE_DOT;
        temp_len += 1;
        
        MMI_WSTRNCPY(full_path_ptr + temp_len, *full_path_len_ptr , suffix_ptr, suffix_len, suffix_len);
        temp_len += suffix_len;
    }

    *full_path_len_ptr = temp_len;
        
    return result;
}

/*****************************************************************************/
//  Description : backup buffer into file
//  Global resource dependence : 
//  Author:cheney.wu
//  Note: 
/*****************************************************************************/
PUBLIC void DEBUG_BackupData(void* buff,uint32 buff_len, char* token)
{
    char        prename[30+1] = {0};
    char        name[30+1] = {0};
    MMI_STRING_T    name_str_ptr;
    MMIFILE_HANDLE  file_handle;
    MMIFILE_DEVICE_E  dev    = MMI_DEVICE_SDCARD;
    wchar       path[MMIFMM_FILENAME_LEN+1]={0};
    uint16      path_length = MMIFMM_FILENAME_LEN;
    wchar       g_memo_file_suffix[]   = {'.' , 't', 'x', 't', 0 };
    wchar       g_memo_dir[]   = { 'M', 'e', 'm', 'o',  0};
    wchar       file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      file_name_len = MMIFILE_FULL_PATH_MAX_LEN;

    return;
#if 0
    sprintf(prename, "_DATA_%03d", cache_buf_serial_index);
    strcpy(name, token);
    strcat(name, prename);

    cache_buf_serial_index++;

    name_str_ptr.wstr_len = SCI_STRLEN(name);
    name_str_ptr.wstr_ptr = SCI_ALLOC((name_str_ptr.wstr_len + 1) * sizeof(wchar));
    if (name_str_ptr.wstr_ptr != PNULL)
    {
        SCI_MEMSET(name_str_ptr.wstr_ptr, 0x00, ((name_str_ptr.wstr_len + 1) * sizeof(wchar)));
        MMI_STRNTOWSTR(name_str_ptr.wstr_ptr, name_str_ptr.wstr_len, 
            (uint8*)name, name_str_ptr.wstr_len, 
            name_str_ptr.wstr_len);
    }
    else
    {
        name_str_ptr.wstr_len = 0;
    }

    dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
    MMIAPIFMM_CombineFullPath(
        MMIAPIFMM_GetDevicePath(dev), 
        MMIAPIFMM_GetDevicePathLen(dev),
        g_memo_dir, 
        MMIAPICOM_Wstrlen(g_memo_dir),
        PNULL, 
        NULL,
        path,
        &path_length
        );
    
    CombinePath(path, path_length,
        name_str_ptr.wstr_ptr, name_str_ptr.wstr_len,
        (wchar*)g_memo_file_suffix + 1, MMIAPICOM_Wstrlen(g_memo_file_suffix) -1, 
        file_name, &file_name_len);

    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
    if(SFS_INVALID_HANDLE != file_handle)
    {   
        MMIAPIFMM_WriteFile(file_handle, (const void*)buff, buff_len, &buff_len, NULL);
        
        MMIAPIFMM_CloseFile(file_handle);
    }
#endif
}

/*****************************************************************************/
//  Description : backup buffer into file
//  Global resource dependence : 
//  Author:cheney.wu
//  Note: 
/*****************************************************************************/
PUBLIC void DEBUG_MakeList(char* token)
{
    char        prename[30+1] = {0};
    char        name[30+1] = {0};
    MMI_STRING_T    name_str_ptr;
    MMIFILE_HANDLE  file_handle;
    MMIFILE_DEVICE_E  dev    = MMI_DEVICE_SDCARD;
    wchar       path[MMIFMM_FILENAME_LEN+1]={0};
    uint16      path_length = MMIFMM_FILENAME_LEN;
    wchar       g_memo_file_suffix[]   = {'.' , 't', 'x', 't', 0 };
    wchar       g_memo_dir[]   = { 'M', 'e', 'm', 'o',  0};
    wchar       file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint8       *buff = PNULL;
    
    if(cache_buf_serial_index == 0)
    {
        return;
    }

    if(NULL == token)
    {
        return;
    }

    strcpy(name, token);
    strcat(name, "_DATA_List");
    
    name_str_ptr.wstr_len = SCI_STRLEN(name);
    name_str_ptr.wstr_ptr = SCI_ALLOC((name_str_ptr.wstr_len + 1) * sizeof(wchar));
    if (name_str_ptr.wstr_ptr != PNULL)
    {
        SCI_MEMSET(name_str_ptr.wstr_ptr, 0x00, ((name_str_ptr.wstr_len + 1) * sizeof(wchar)));
        MMI_STRNTOWSTR(name_str_ptr.wstr_ptr, name_str_ptr.wstr_len, 
            (uint8*)name, name_str_ptr.wstr_len, 
            name_str_ptr.wstr_len);
    }
    else
    {
        name_str_ptr.wstr_len = 0;
    }
    
    dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
    MMIAPIFMM_CombineFullPath(
        MMIAPIFMM_GetDevicePath(dev), 
        MMIAPIFMM_GetDevicePathLen(dev),
        g_memo_dir, 
        MMIAPICOM_Wstrlen(g_memo_dir),
        PNULL, 
        NULL,
        path,
        &path_length
        );
    
    CombinePath(path, path_length,
        name_str_ptr.wstr_ptr, name_str_ptr.wstr_len,
        (wchar*)g_memo_file_suffix + 1, MMIAPICOM_Wstrlen(g_memo_file_suffix) -1, 
        file_name, &file_name_len);
    
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
    if(SFS_INVALID_HANDLE != file_handle)
    {
        uint16 index = 0;
        uint32 bytes_to_write = 0;

        bytes_to_write = cache_buf_serial_index * 36 + 4;
        buff = SCI_ALLOC(bytes_to_write);
        memset(buff, 0x00, bytes_to_write);

        for(index = 0; index <= cache_buf_serial_index; index++)
        {
            memset(name, 0x00, 31);
            strcpy(name, "&");
            strcat(name, "token");
            sprintf(prename, "_DATA_%03d", index);
            strcat(name, prename);
            strcat(name, "#");
            strcat((char*)buff, name);
        }

        bytes_to_write  = strlen((char*)buff);
        MMIAPIFMM_WriteFile(file_handle, (const void*)buff, bytes_to_write, &bytes_to_write, NULL);
        
        MMIAPIFMM_CloseFile(file_handle);
    }

    SCI_FREE(name_str_ptr.wstr_ptr);
    SCI_FREE(buff);
    cache_buf_serial_index = 0;
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_InsertMAPDataToList(void* buff,uint16 buff_len)
{
#if defined(TRACE_MAP_BUFFER)
    static uint16 list_data_cont = 0;

    SCI_TRACE_LOW("[MMIAPISMS], InsertMAPDataToList, list_data_cont = %d", list_data_cont);

    DEBUG_BackupData(buff, (uint32)buff_len, "MAPLIST");

#endif

    if(0 == InsertList(s_map_list_head, buff,buff_len))
    {
        SCI_TRACE_LOW("[MMIAPISMS] InsertMAPDataToList failed");
        return FALSE;
    }
    else
    {
#if defined(TRACE_MAP_BUFFER)
        list_data_cont ++;
#endif
        SCI_TRACE_LOW("[MMIAPISMS] InsertMAPDataToList success");
    }
    

	return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_InsertMessageDataToList(void* buff,uint16 buff_len)
{
#if defined(TRACE_MAP_BUFFER)
    uint16 len = 0;
    uint16 i;
    uint16 count = 0;

    for( i = 0; i < (buff_len/TRACE_MAP_BUF_COUNT_BYTE) +1;  i++ )
    {
        SCI_TRACE_LOW("[MMIAPISMS], MMIAPISMS_InsertMessageDataToList, i = %d", i);
        len = buff_len - i*TRACE_MAP_BUF_COUNT_BYTE;
        count = i*TRACE_MAP_BUF_COUNT_BYTE;
        
        if( len >= TRACE_MAP_BUF_COUNT_BYTE)
        {
            SCI_TRACE_BUF("[MMIAPISMS] MMIAPISMS_InsertMessageDataToList: MAP buff is :", ((char*)buff+count), TRACE_MAP_BUF_COUNT_BYTE);
        }
        else
        {
            SCI_TRACE_BUF("[MMIAPISMS] MMIAPISMS_InsertMessageDataToList: MAP buff is :", ((char*)buff+count), len);
        }
    }
#endif

    if(0 == InsertList(s_map_bmessage_head, buff,buff_len))
    {
        SCI_TRACE_LOW("[MMIAPISMS] MMIAPISMS_InsertMessageDataToList failed");
        return FALSE;
    }
    else
    {
        SCI_TRACE_LOW("[MMIAPISMS] MMIAPISMS_InsertMessageDataToList success");
    }

	return TRUE;
}

/*****************************************************************************/
//  Description :notify pb start parser s_map_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifySMSStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
    MMISMS_NotifySMSStartParser(callback);
}

/*****************************************************************************/
//  Description :notify pb start parser s_map_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifySetFolderParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
    MMISMS_NotifySetFoldertParser(callback);
}


/*****************************************************************************/
//  Description :notify pb start parser s_map_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifyGetMsgListStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
#if defined(TRACE_MAP_BUFFER)
    DEBUG_MakeList("MAPLIST");
#endif
    MMISMS_NotifyGetMsgListStartParser(callback);
}


/*****************************************************************************/
//  Description :notify pb start parser s_map_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifyGetMessageStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
#if defined(TRACE_MAP_BUFFER)
//    DEBUG_MakeList("BMSG");
#endif
    MMISMS_NotifyGetMessageStartParser(callback);
}


/*******************************************************************************/
//  Description:判断单链表是否为空
//  Parameter: 
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
LOCAL int IsListEmpty(MAP_LIST_T *L)
{
    return(PNULL == (L->next));
}

/*******************************************************************************/
//  Description:判断链表节点个数
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int ListNodeNum(MAP_LIST_T *L)
{
    MAP_LIST_T *p = L;
    int i = 0;
    while(PNULL != p->next)
    {
       i++;
       p = p->next;
    }
    return i;
}

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InitMAPDataList(void)
{				
	if(PNULL == s_map_list_head)
	{
		s_map_list_head = InitList();
	}
	else if(!IsListEmpty(s_map_list_head))
	{   
	    ClearList(s_map_list_head);
	}

	if(PNULL == s_map_list_head)
	{
		return FALSE;
	}
    s_map_list_node_number = 1;
	return TRUE;
}

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InitMAPMessageData(void)
{				
	if(PNULL == s_map_bmessage_head)
	{
		s_map_bmessage_head = InitList();
	}
	else if(!IsListEmpty(s_map_bmessage_head))
	{   
	    ClearList(s_map_bmessage_head);
	}

	if(PNULL == s_map_bmessage_head)
	{
		return FALSE;
	}
    s_map_bmessage_node_number = 1;
    s_map_bmessage_current_node_number = 1;
	return TRUE;
}

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InitMessageTableList(void)
{				
	if(PNULL == s_message_table_head)
	{
		s_message_table_head = InitMsgHandleTableList();
	}

	if(PNULL == s_message_table_head)
	{
		return FALSE;
	}
    return TRUE;
}

/*******************************************************************************/
//  Description :
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_InitMAPDataList(void)
{
    BOOLEAN ret = FALSE;

    ret = MMISMS_InitMAPDataList();

    if( TRUE == ret )
    {
        ret = MMISMS_InitMAPMessageData();
    }
    if( TRUE == ret)
    {
        ret = MMISMS_InitMessageTableList();
    }        
    return ret;
}

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_ClearTableList(void)
{
	if(PNULL == s_message_table_head)
	{
		return TRUE;
	}
	
	if(!IsTableListEmpty(s_message_table_head))
	{   
		ClearTableList(s_message_table_head);
	}
    return TRUE;
}



/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifySMSStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
    SCI_TRACE_LOW("Candicd, MMISMS_NotifySMSStartParser");
    
    if(IsListEmpty(s_map_list_head))
    {
        MMISMS_MAP_PARSER_RESULT_T result = {0};
        result.ret = MMISMS_MAP_PARSER_FINISHED;
        result.sync_count = s_sync_sms_count;
        callback(&result);
        freeallmapdata();
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifySMSStartParser buff = pnull");
    }
    else
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifySMSStartParser count = %d",ListNodeNum(s_map_list_head));
        MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_FOLDER_LIST); 
    }
}

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifySetFoldertParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
    SCI_TRACE_LOW("Candicd, MMISMS_NotifySetFoldertParser");
    
    if(IsListEmpty(s_map_list_head))
    {        
        MMISMS_MAP_PARSER_RESULT_T result = {0};
        result.ret = MMISMS_MAP_PARSER_FINISHED;
        result.sync_count = s_sync_sms_count;
        callback(&result);
        freeallmapdata();
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifySetFoldertParser buff = pnull");
    }
    else
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifySetFoldertParser count = %d",ListNodeNum(s_map_list_head));
        MMIAPICOM_SendSignalToMMITask(MMISMS_BT_SET_FOLDER); 
    }
}

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifyGetMsgListStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
    if(IsListEmpty(s_map_list_head))
    {        
        MMISMS_MAP_PARSER_RESULT_T result = {0};
        result.ret = MMISMS_MAP_PARSER_FINISHED;
        result.sync_count = s_sync_sms_count;
        callback(&result);
        freeallmapdata();
        MMISMS_CloseMapWaitWin();
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifyGetMsgListStartParser buff = pnull");
    }
    else
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifyGetMsgListStartParser count = %d",ListNodeNum(s_map_list_head));
        MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MSG_LIST); 
    }
}

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMISMS_NotifyGetMessageStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback)
{
    if(IsListEmpty(s_map_bmessage_head))
    {        
        MMISMS_MAP_PARSER_RESULT_T result = {0};
        result.ret = MMISMS_MAP_PARSER_FINISHED;
        result.sync_count = s_map_bmessage_node_number;
        callback(&result);
        freeallmapdata();
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifyGetMessageStartParser buff = pnull");
    }
    else
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_NotifyGetMessageStartParser count = %d",ListNodeNum(s_map_bmessage_head));
        MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MESSAGE); 
    }
}

/*******************************************************************************/
//  Description:获取单链表中第i个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL int GetElem(MAP_LIST_T *L, int i, void** buff, uint16 *len)
{
    int j = 1;
    MAP_LIST_T *p = L->next;

    while(p && (j < i))
    {
        p = p->next;
        ++j;
    }

    if(PNULL == p || (j > i))
    {
        SCI_TRACE_LOW("GetElem, p = PNULL");
    
        return 0;
    }
    else
    {
        SCI_TRACE_LOW("GetElem, p = %x", p);
    
        *buff = p->data;
        *len = p->len;
        return 1;
    }
}


/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseMapData(void)
{   
    char *data_ptr = PNULL;
    uint16 len = 0;
    
    GetElem(s_map_list_head, s_map_list_node_number, &data_ptr, &len);
    s_map_list_node_number++;
    return MMISMS_AddDataToSMS(data_ptr);
}

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseSetFolder(void)
{   
    char *data_ptr = PNULL;
    uint16 len = 0;
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_ParseSetFolder");
    GetElem(s_map_list_head, s_map_list_node_number, &data_ptr, &len);
    s_map_list_node_number++;
    //MMIPB_SaveBufferToFile(data_ptr);
    return SetFolder(data_ptr);
}

/*******************************************************************************/
//  Description : MMISMS_ParseMapMsgList
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseMapMsgList(void)
{   
    char *data_ptr = PNULL;
    uint16 len = 0;
    MMISMS_OPER_ERR_E retVal;

    retVal = MMISMS_GetMsgFromMapMsgList(data_ptr);
    while(MMISMS_SYSTEM_BUSY == retVal)
    {
        if(GetElem(s_map_list_head, s_map_list_node_number, &data_ptr, &len))
        {
            s_map_list_node_number++;
            
            retVal = MMISMS_GetMsgFromMapMsgList(data_ptr);
        }
        else
        {
            if(MMK_IsOpenWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID);
                MMISMS_SetMapAllowSync(TRUE);            
            }
            retVal = MMISMS_NO_ERR;
        }

    }

    if((MMISMS_NO_SPACE == retVal) || (MMISMS_BT_ERROR == retVal))
    {
        if(MMK_IsOpenWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID))
        {
            MMK_CloseWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID);
            MMISMS_SetMapAllowSync(TRUE);            
        }
    }
    return retVal;
}

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetFullMsgById(void)
{   
#ifndef WIN32
    char *data_ptr = PNULL;
    
    BT_MapGetMessage(
        data_ptr,
        TRUE,                       //BOOLEAN                   attachment,
        BT_MAP_CHARSET_UTF8,        //BtMapCharsetFlags         charset,
        BT_MAP_FRACTION_REQ_NONE    //BtMapFractionReqFlags     fraction_req
        );
#endif
//     if(MMK_IsOpenWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID))
//     {
//         MMK_CloseWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID);
//     }

    return TRUE;
}

/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_GetMsgFromMapBMsg(const char*  buff) 
{

	char    *p_searched_ptr = PNULL;
    uint32  offset_len = 0;
    char    *search_ptr = (char*)buff;
	uint32	extract_len = 0;
	char    *extract_buff_ptr = PNULL;
    MMISMS_OPER_ERR_E   ret = MMISMS_NO_ERR;
	char    *search_endTag_ptr = PNULL;
    char    *merge_buff_ptr = PNULL; 
	uint32  merge_buff_len = 0;
    char    *rn_property = "\r\n"; 
    MN_SMS_ALPHABET_TYPE_E alphabet_type = MN_SMS_8_BIT_ALPHBET;

    uint8   *name = PNULL;
    uint8   *tel = PNULL;
    uint8   *body = PNULL;
    uint8   *content = PNULL;
    uint8   *charset = PNULL;
    uint8   *status = PNULL;
    uint32  data_len = 0;

    BOOLEAN parse_status_ret = FALSE;
    BOOLEAN parse_name_ret = FALSE;
    BOOLEAN parse_tel_ret = FALSE;
    BOOLEAN parse_body_ret = FALSE;
    BOOLEAN parse_content_ret = FALSE;
    BOOLEAN parse_charset_ret = FALSE; 

    MMI_STRING_T  p_msg_content = {0};
    

    if(PNULL == buff)
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg buff = PNULL");
        return MMISMS_BT_ERROR;
    }
    
    if (PNULL != s_sms_buff_ptr)
    {
        merge_buff_len = strlen((char*)s_sms_buff_ptr) + strlen((char*)buff);
    }
    else
    {
        merge_buff_len = strlen((char*)buff);
    }
    
    //分配拼接后 所需buffer
    merge_buff_ptr = SCI_ALLOC_APP(merge_buff_len + 1);
    
    if (PNULL == merge_buff_ptr)
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg merge_buff_ptr Alloc fail!");
        
        if(PNULL != s_sms_buff_ptr)
        {
            SCI_FREE(s_sms_buff_ptr);
        }
        
        return MMISMS_BT_ERROR;
    }
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg merge_buff_len:%d", merge_buff_len);
    
    SCI_MEMSET(merge_buff_ptr, 0x00, merge_buff_len + 1);
    
    //上批缓存中有内容, 需要先将 上一批缓存中的data 与 下一批buffer中的data 拼接起来
    if (PNULL != s_sms_buff_ptr)
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg s_sms_buff_ptr has data");
        
        strncpy(merge_buff_ptr, s_sms_buff_ptr, strlen((char*)s_sms_buff_ptr));
        
        //释放上一批缓存buffer
        SCI_FREE(s_sms_buff_ptr);	
    }
    
    strncat(merge_buff_ptr, buff, strlen((char*)buff));
    
    MMI_CheckAllocatedMemInfo();
    
    
    do
    {
        /************************************************************************/
        /* whether phone stroage has free space or not                           */
        /************************************************************************/
        
        //偏移长度
        p_searched_ptr = &merge_buff_ptr[offset_len];
        
        //判断是否只剩下"\r\n"
        if (0 == strcmp(rn_property, p_searched_ptr))
        {
                SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg \r\n");
            ret = MMISMS_BT_ERROR;
            break;
        }
        
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg merge_buff_len:%d", merge_buff_len);
        
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg merge_buff_ptr:%s", merge_buff_ptr);
        
        
        /************************************************************************/
        //buffer 提取规则如下:
        //BEGIN:VCARD与END:VCARD必须配对出现
        //先查找BEGIN:VCARD，再查找END:VCARD 
        //如果BEGIN:VCARD与END:VCARD都存在,说明数据完整，可以直接对其进行解析
        //否则说明数据不完整，需要和下一批buffer的数据一起解析
        /************************************************************************/        
        {
            //找BEGIN:VCARD 
            search_ptr = strstr(p_searched_ptr, MESSAGE_TAGS);
            SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg start find MESSAGE_TAGS = %ld",search_ptr);
            
            if (PNULL != search_ptr)
            {        
                //找END:VCARD 
                search_endTag_ptr = strstr(search_ptr, MESSAGE_END_TAGS);
                SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg start find MESSAGE_END_TAGS = %ld",search_endTag_ptr);
                
                if (PNULL != search_endTag_ptr)
                {
                    
                    //begin and end 两个都找到的情况下
                    //此处使用p_searched_ptr来代替search_ptr来计算偏移量
                    extract_len = (strlen((char*)p_searched_ptr) - strlen((char*)search_endTag_ptr) + strlen((char*)MESSAGE_END_TAGS));	
                    
                    extract_buff_ptr = SCI_ALLOC_APP(extract_len + 1);
                    if (PNULL == extract_buff_ptr)
                    {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg extract_buff_ptr alloc fail");
                        ret = MMISMS_BT_ERROR;
                        break;
                    }
                    SCI_MEMSET(extract_buff_ptr, 0x00, (extract_len + 1));
                    
                    strncpy(extract_buff_ptr, p_searched_ptr, extract_len);
                    
                    if ( 0 == extract_len )
                    {
                        break;
                    }
                    // search status
                    parse_status_ret = LocateDataFromBuffer(extract_buff_ptr, extract_len, STATUS_TAGS, NEW_LINE, &status, &data_len);
                    SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg status = %s, status len = %d\n", status, data_len);
                        if( FALSE == parse_status_ret)
                    {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg status parse fail");
                        ret = MMISMS_BT_ERROR;
                        break;
                    }
                    
                    // search name
                    parse_name_ret = LocateDataFromBuffer(extract_buff_ptr, extract_len, NAME_TAGS, NEW_LINE, &name, &data_len);
                        if( FALSE == parse_name_ret)
                    {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg name parse fail");
                        ret = MMISMS_BT_ERROR;
                        break;
                    }
                    
                    // search tel
                    parse_tel_ret = LocateDataFromBuffer(extract_buff_ptr, extract_len, TEL_TAGS, NEW_LINE, &tel, &data_len);
                       if( FALSE == parse_tel_ret )
                    {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg tel parse fail");
                        ret = MMISMS_BT_ERROR;
                        break;
                    }
                    
                    // search body
                    parse_body_ret = LocateDataFromBuffer(extract_buff_ptr, extract_len, MSG_BODY_TAGS, MSG_BODY_END_TAGS, &body, &data_len);
                       if( FALSE == parse_body_ret )
                    {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg body parse fail");
                        ret = MMISMS_BT_ERROR;
                        break;
                    }
                    
                    if( TRUE == parse_body_ret )
                    {
                        uint32 content_len = 0;
                        // search content
                        parse_content_ret = LocateDataFromBuffer((char*)body, data_len, MSG_PDU_TAGS, MSG_PDU_END_TAGS, &content, &content_len);
                       if( FALSE == parse_content_ret )
                        {
                            SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg content parse fail");
                            ret = MMISMS_BT_ERROR;
                            break;
                        }
                        
                        // search charset
                        parse_charset_ret = LocateDataFromBuffer(extract_buff_ptr, extract_len, MSG_CHARSET_TAGS, NEW_LINE, &charset, &data_len);
                       if( FALSE == parse_charset_ret )
                        {
                            SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg charset parse fail");
                            ret = MMISMS_BT_ERROR;
                            break;
                        }
                        
                        if( 0 == strncmp((char*)charset, UTF8_STRING, data_len ))
                        {
                            alphabet_type =  MN_SMS_8_BIT_ALPHBET;        
                        }
                        else if ( 0 == strncmp((char*)charset, UCS2_STRING, data_len ))
                        {
                            alphabet_type =  MN_SMS_UCS2_ALPHABET;            
                        }
                        
                        if( MN_SMS_8_BIT_ALPHBET == alphabet_type )
                        {
                            p_msg_content.wstr_ptr = (wchar *)SCI_ALLOC_APP(content_len * sizeof(wchar));
                            p_msg_content.wstr_len = GUI_UTF8ToWstr(p_msg_content.wstr_ptr, content_len,
                                (const uint8*)content, content_len);
							
                            if( PNULL != p_long_sms.wstr_ptr)
							{
                                SCI_FREE(p_long_sms.wstr_ptr);
							}
                            p_long_sms.wstr_len = 0;
                            p_long_sms.wstr_ptr = (wchar *)SCI_ALLOC_APP((content_len+1) * sizeof(wchar));
                            SCI_MEMSET(p_long_sms.wstr_ptr , 0, (content_len + 1)* sizeof(wchar));
                            p_long_sms.wstr_len = GUI_UTF8ToWstr(p_long_sms.wstr_ptr, content_len,
                                (const uint8*)content, content_len);
                        }
                        
                    }

                    // MMISMS_InsertSMSRecord((char*)tel, &p_msg_content, 0);
                    offset_len += extract_len;
                    SCI_FREE(name);
                    SCI_FREE(tel);
                    SCI_FREE(body);
                    SCI_FREE(content);
                    SCI_FREE(charset);
                    continue;
                }
            }
            
            //数据不完整，需要先缓存起来 ps:处理每条buffer最后不完整的记录
            extract_len = strlen((char*)p_searched_ptr);
            
            if (PNULL != s_sms_buff_ptr)
            {
                SCI_FREE(s_sms_buff_ptr);
            }
            
            s_sms_buff_ptr = SCI_ALLOC_APP(extract_len + 1);
            
            if (PNULL == s_sms_buff_ptr)
            {
                            SCI_TRACE_LOW("MMISMS_GetMsgFromMapBMsg s_sms_buff_ptr alloc fail");
                ret = MMISMS_BT_ERROR;
                break;
            }
            
            SCI_MEMSET(s_sms_buff_ptr, 0x00, extract_len + 1);
            
            strncpy(s_sms_buff_ptr, p_searched_ptr, extract_len);
            
//            ret = MMISMS_BT_ERROR;
            break;
            //缓存数据结束
        }
        
    }    
    while(offset_len < strlen((char*)merge_buff_ptr));

	SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg, p_msg_content.wstr_len=%d", p_msg_content.wstr_len);

//        MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MESSAGE_CNF); 
    
    if( PNULL == p_msg_content.wstr_ptr ) 
        {
	SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg, p_msg_content error");
        }

    MMI_CheckAllocatedMemInfo();
    
    if (PNULL != merge_buff_ptr) /*lint !e774*/
    {
        SCI_FREE(merge_buff_ptr);
    }
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapBMsg s_map_bmessage_current_node_number = %d,ret = %d", s_map_bmessage_current_node_number-1,ret); 

    return ret;  		
}

/*******************************************************************************/
//  Description :
//  Parameter: data_ptr[in]: 
//			   data_len[in]:
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseMapBMsg(void)
{   
    char *data_ptr = PNULL;
    uint16 len = 0;
    MMISMS_OPER_ERR_E retVal;

    SCI_TRACE_LOW("MMISMS_ParseMapBMsg, s_map_bmessage_node_number = %d", s_map_bmessage_current_node_number);

    GetElem(s_map_bmessage_head, s_map_bmessage_current_node_number, &data_ptr, &len);
    s_map_bmessage_current_node_number++;

    do 
    {
        MMISMS_GetMsgFromMapBMsg(data_ptr);
        len = 0;
        retVal = GetElem(s_map_bmessage_head, s_map_bmessage_current_node_number, &data_ptr, &len);
        s_map_bmessage_current_node_number++;
    } while(len != 0);

            MMIAPICOM_SendSignalToMMITask(MMISMS_BT_GET_MESSAGE_CNF); 
            
    return retVal;

}

#ifdef WIN32
/*****************************************************************************/
//     Description : to handle the BT message of SMS box window
//     Global resource dependence : 
//     Author:Candice.Chen
//     Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SimuMAPDataToList(uint16 index)
{
    const uint8 simu_msglist_array[] = {
        0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x27, 0x31, 0x2E, 0x30, 0x27, 0x20, 0x65, 0x6E, 0x63, 0x6F, 0x64, 0x69, 0x6E, 0x67, 0x3D, 0x27, 0x75, 0x74,
        0x66, 0x2D, 0x38, 0x27, 0x20, 0x73, 0x74, 0x61, 0x6E, 0x64, 0x61, 0x6C, 0x6F, 0x6E, 0x65, 0x3D, 0x27, 0x79, 0x65, 0x73, 0x27, 0x20, 0x3F, 0x3E, 0x0A, 0x3C, 0x4D, 0x41, 0x50, 0x2D, 0x6D, 0x73,
        0x67, 0x2D, 0x6C, 0x69, 0x73, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x2E, 0x30, 0x22, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73,
        0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x36, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A,
        0x65, 0x63, 0x74, 0x3D, 0x22, 0x31, 0x30, 0x33, 0x2F, 0x36, 0x2F, 0x33, 0x30, 0xE5, 0x89, 0x8D, 0xE7, 0x94, 0xB3, 0xE8, 0xBE, 0xA6, 0xE5, 0x8F, 0xB0, 0xE7, 0x81, 0xA3, 0xE5, 0xA4, 0xA7, 0xE5,
        0x93, 0xA5, 0xE5, 0xA4, 0xA7, 0xE9, 0x9B, 0xBB, 0xE5, 0xAD, 0x90, 0xE5, 0xB8, 0xB3, 0xE5, 0x96, 0xAE, 0xE4, 0xB8, 0xA6, 0xE7, 0xAC, 0xA6, 0xE5, 0x90, 0x88, 0xE6, 0xB4, 0xBB, 0xE5, 0x8B, 0x95,
        0xE8, 0xB3, 0x87, 0xE6, 0xA0, 0xBC, 0xEF, 0xBC, 0x8C, 0xE5, 0x85, 0x8D, 0xE8, 0xB2, 0xBB, 0xE9, 0x80, 0x81, 0xE5, 0xB8, 0x82, 0xE5, 0x83, 0xB9, 0x36, 0x39, 0x39, 0xE5, 0x85, 0x83, 0xE9, 0xA6,
        0xAC, 0xE5, 0x8D, 0xA1, 0xE9, 0xBE, 0x8D, 0xE9, 0x9A, 0xA8, 0xE8, 0xBA, 0xAB, 0xE9, 0x9F, 0xB3, 0xE7, 0xAE, 0xB1, 0xEF, 0xBC, 0x8C, 0xE7, 0x94, 0xB3, 0xE8, 0xAB, 0x8B, 0xE6, 0x96, 0xB9, 0xE5,
        0xBC, 0x8F, 0xEF, 0xBC, 0x9A, 0xE5, 0xB8, 0xB3, 0xE5, 0x96, 0xAE, 0xE8, 0x83, 0x8C, 0xE9, 0x9D, 0xA2, 0xE5, 0x82, 0xB3, 0xE7, 0x9C, 0x9F, 0xE3, 0x80, 0x81, 0xE5, 0xAE, 0x98, 0xE7, 0xB6, 0xB2,
        0xE3, 0x80, 0x81, 0xE9, 0x96, 0x80, 0xE5, 0xB8, 0x82, 0xE6, 0x88, 0x96, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0xE6, 0x92, 0xA5, 0x31, 0x38, 0x38, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69,
        0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x31, 0x31, 0x54, 0x31, 0x34, 0x31, 0x34, 0x35, 0x33, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65,
        0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D,
        0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72,
        0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53,
        0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x39, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70,
        0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E,
        0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79,
        0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20,
        0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x35, 0x22,
        0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE7, 0x86, 0xB1, 0xE7, 0x88, 0x86, 0xE4, 0xBA, 0x86, 0x7E, 0xE5, 0x86, 0xB7, 0xE6, 0xB0, 0xA3, 0xE5, 0x90, 0xB9, 0xE6, 0x95, 0xB4,
        0xE5, 0xA4, 0xA9, 0x2C, 0xE5, 0x86, 0xB0, 0xE5, 0x93, 0x81, 0xE5, 0x90, 0x83, 0xE5, 0xA4, 0xAA, 0xE5, 0xA4, 0x9A, 0x2C, 0xE8, 0xBA, 0xAB, 0xE9, 0xAB, 0x94, 0x4F, 0x4B, 0xE5, 0x97, 0x8E, 0x3F,
        0xE5, 0xBF, 0xAB, 0xE5, 0x88, 0xB0, 0xE5, 0x85, 0xAD, 0xE6, 0x98, 0x9F, 0xE9, 0x9B, 0x86, 0xE4, 0xBA, 0xAB, 0xE7, 0x94, 0xA8, 0xE8, 0x85, 0xB3, 0xE5, 0xBA, 0x95, 0xE6, 0x8C, 0x89, 0xE6, 0x91,
        0xA9, 0x2C, 0x24, 0x34, 0x39, 0x39, 0xE8, 0xAE, 0x93, 0xE5, 0xB0, 0x88, 0xE6, 0xA5, 0xAD, 0xE7, 0x9A, 0x84, 0xE4, 0xBE, 0x86, 0xE5, 0x96, 0xAC, 0xE4, 0xB8, 0x80, 0xE5, 0x96, 0xAC, 0x68, 0x74,
        0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x62, 0x69, 0x74, 0x2E, 0x6C, 0x79, 0x2F, 0x31, 0x70, 0x42, 0x79, 0x58, 0x63, 0x44, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32,
        0x30, 0x31, 0x34, 0x30, 0x36, 0x31, 0x31, 0x54, 0x31, 0x30, 0x31, 0x32, 0x34, 0x36, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E,
        0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35,
        0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69,
        0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22,
        0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x37, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F,
        0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x00

    }; 
   
    const uint8 simu_msglist_array2[] = {
        0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22,
        0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E,
        0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x33, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE5, 0xAD, 0xA9, 0xE5, 0xAD, 0x90, 0xE7, 0x9A, 0x84, 0xE6, 0x9A, 0x91, 0xE5, 0x81,
        0x87, 0xE5, 0xA6, 0x82, 0xE4, 0xBD, 0x95, 0xE5, 0xAE, 0x89, 0xE6, 0x8E, 0x92, 0xEF, 0xBC, 0x9F, 0xE6, 0xBC, 0xA2, 0xE9, 0x9C, 0x96, 0xE6, 0xB0, 0x91, 0xE4, 0xBF, 0x97, 0xE8, 0xAA, 0xAA, 0xE5,
        0x94, 0xB1, 0xE8, 0x97, 0x9D, 0xE8, 0xA1, 0x93, 0xE5, 0x9C, 0x98, 0xE7, 0x9A, 0x84, 0xE8, 0xAA, 0xAA, 0xE5, 0x94, 0xB1, 0xE4, 0xB8, 0xBB, 0xE9, 0xA1, 0x8C, 0xE5, 0xA4, 0x8F, 0xE4, 0xBB, 0xA4,
        0xE7, 0x87, 0x9F, 0xEF, 0xBC, 0x8C, 0xE4, 0xBB, 0xA5, 0xE8, 0xA9, 0xBC, 0xE8, 0xAB, 0xA7, 0xE9, 0x80, 0x97, 0xE8, 0xB6, 0xA3, 0xE7, 0x9A, 0x84, 0xE6, 0x96, 0xB9, 0xE5, 0xBC, 0x8F, 0xEF, 0xBC,
        0x8C, 0xE8, 0xAE, 0x93, 0xE5, 0xAD, 0xA9, 0xE5, 0xAD, 0x90, 0xE8, 0xBC, 0x95, 0xE9, 0xAC, 0x86, 0xE5, 0x9F, 0xB9, 0xE9, 0xA4, 0x8A, 0xE5, 0x8F, 0xA3, 0xE8, 0xAA, 0xAA, 0xE8, 0x83, 0xBD, 0xE5,
        0x8A, 0x9B, 0xE5, 0x8F, 0x8A, 0xE7, 0xA9, 0xA9, 0xE5, 0x81, 0xA5, 0xE5, 0x8F, 0xB0, 0xE9, 0xA2, 0xA8, 0xE3, 0x80, 0x81, 0xE5, 0xA0, 0xB1, 0xE5, 0x90, 0x8D, 0xE8, 0xAB, 0x8B, 0xE6, 0xB4, 0xBD,
        0x28, 0x30, 0x32, 0x29, 0x38, 0x32, 0x33, 0x30, 0x31, 0x39, 0x31, 0x30, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x31, 0x30,
        0x54, 0x31, 0x39, 0x31, 0x39, 0x30, 0x32, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x38, 0x37,
        0x35, 0x37, 0x31, 0x33, 0x39, 0x30, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69,
        0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22,
        0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F,
        0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A,
        0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73,
        0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73,
        0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x32, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A,
        0x65, 0x63, 0x74, 0x3D, 0x22, 0xE5, 0x85, 0xA8, 0xE5, 0xAE, 0xB6, 0x2D, 0x50, 0x52, 0x6F, 0x75, 0x74, 0x65, 0x72, 0xEF, 0xBC, 0x9A, 0xE5, 0x96, 0xAE, 0xE8, 0x99, 0x9F, 0x36, 0x38, 0x33, 0x33,
        0x30, 0x38, 0x35, 0x31, 0x36, 0x35, 0x30, 0x37, 0x39, 0x35, 0x30, 0x20, 0x2C, 0x30, 0x36, 0x2F, 0x31, 0x30, 0xE5, 0xB7, 0xB2, 0xE5, 0x88, 0xB0, 0xE5, 0x85, 0xA8, 0xE5, 0xAE, 0xB6, 0xE7, 0xB2,
        0xBE, 0xE8, 0x8B, 0xB1, 0xE5, 0xBA, 0x97, 0x2C, 0xE8, 0xAB, 0x8B, 0xE6, 0x94, 0x9C, 0xE5, 0xB8, 0xB6, 0xE8, 0xBA, 0xAB, 0xE4, 0xBB, 0xBD, 0xE8, 0xAD, 0x89, 0xE5, 0x89, 0x8D, 0xE5, 0xBE, 0x80,
        0xE5, 0x8F, 0x96, 0xE4, 0xBB, 0xB6, 0xEF, 0xBC, 0x8C, 0x30, 0x36, 0x2F, 0x31, 0x37, 0xE5, 0xB0, 0x87, 0xE9, 0x80, 0x80, 0xE5, 0x9B, 0x9E, 0xE7, 0x89, 0xA9, 0xE6, 0xB5, 0x81, 0xE4, 0xB8, 0xAD,
        0xE5, 0xBF, 0x83, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x31, 0x30, 0x54, 0x31, 0x30, 0x31, 0x30, 0x31, 0x36, 0x22, 0x20,
        0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x34, 0x36, 0x34, 0x34, 0x32, 0x30, 0x38, 0x33, 0x34, 0x37, 0x38, 0x31,
        0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64,
        0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x36, 0x36, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73,
        0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20,
        0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E,
        0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64,
        0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x31, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x00

    };
    
    const uint8 simu_msglist_array3[] = {
        0x22, 0xE6, 0x9C, 0xAC, 0xE6, 0x9C, 0x88, 0xE5, 0xBA, 0x95, 0xE5, 0x89, 0x8D, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE3, 0x80, 0x8A, 0xE6, 0x9C, 0xAC, 0xE6, 0x9C, 0x88, 0xE4, 0xB8, 0xBB, 0xE6,
        0x89, 0x93, 0xE6, 0x98, 0x9F, 0x2D, 0xE6, 0x9E, 0x97, 0xE5, 0x87, 0xA1, 0xE3, 0x80, 0x8B, 0xE5, 0xB0, 0x88, 0xE5, 0x8D, 0x80, 0xE7, 0xAD, 0x94, 0xE9, 0x88, 0xB4, 0xEF, 0xBC, 0x8C, 0xE6, 0x8A,
        0xBD, 0xE8, 0x87, 0xAA, 0xE6, 0x8B, 0x8D, 0xE7, 0xA5, 0x9E, 0xE6, 0xA9, 0x9F, 0xEF, 0xBC, 0x81, 0xE6, 0x8E, 0xA8, 0xE8, 0x96, 0xA6, 0x38, 0x32, 0x35, 0x36, 0x33, 0x34, 0x2F, 0xE6, 0xAD, 0xB2,
        0xE6, 0x9C, 0x88, 0xE9, 0x80, 0x99, 0xE6, 0x8A, 0x8A, 0xE5, 0x88, 0x80, 0xEF, 0xBC, 0x8C, 0x38, 0x32, 0x35, 0x36, 0x33, 0x31, 0x2F, 0xE5, 0xA5, 0x97, 0xE4, 0xB8, 0x8D, 0xE4, 0xBD, 0x8F, 0xEF,
        0xBC, 0x8C, 0xE5, 0xBF, 0xAB, 0xE6, 0x92, 0xA5, 0x38, 0x30, 0x33, 0xE6, 0x8C, 0x89, 0x34, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0x28, 0xE6, 0xAF, 0x8F, 0xE7, 0xA7, 0x92, 0x30, 0x2E, 0x31, 0xE5,
        0x85, 0x83, 0x29, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x38, 0x54, 0x31, 0x34, 0x31, 0x31, 0x32, 0x36, 0x22, 0x20,
        0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F,
        0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E,
        0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22,
        0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D,
        0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65,
        0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22,
        0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74,
        0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE3, 0x80, 0x90, 0xE8, 0xB6, 0x99, 0xE7, 0xAD, 0xB1, 0xE7, 0x93,
        0x8F, 0xE3, 0x80, 0x91, 0xE8, 0xAA, 0xA0, 0xE5, 0xBF, 0x83, 0xE7, 0x9A, 0x84, 0xE6, 0x8B, 0x9C, 0xE8, 0xA8, 0x97, 0xE6, 0x82, 0xA8, 0xEF, 0xBC, 0x8C, 0xE4, 0xBB, 0x8A, 0xE5, 0xA4, 0xA9, 0xE3,
        0x80, 0x81, 0xE6, 0x98, 0x8E, 0xE5, 0xA4, 0xA9, 0xE8, 0x8B, 0xA5, 0xE6, 0x82, 0xA8, 0xE5, 0x9C, 0xA8, 0xE5, 0xAE, 0xB6, 0xE6, 0x8E, 0xA5, 0xE5, 0x88, 0xB0, 0xE3, 0x80, 0x8A, 0xE5, 0x8D, 0x97,
        0xE6, 0xB8, 0xAF, 0xE3, 0x80, 0x81, 0xE5, 0x85, 0xA7, 0xE6, 0xB9, 0x96, 0xE3, 0x80, 0x8B, 0xE5, 0xB8, 0x82, 0xE8, 0xAD, 0xB0, 0xE5, 0x93, 0xA1, 0xE6, 0xB0, 0x91, 0xE8, 0xAA, 0xBF, 0xE9, 0x9B,
        0xBB, 0xE8, 0xA9, 0xB1, 0xEF, 0xBC, 0x8C, 0xE8, 0xAB, 0x8B, 0xE8, 0x80, 0x90, 0xE5, 0xBF, 0x83, 0xE8, 0x81, 0xBD, 0xE5, 0xAE, 0x8C, 0xEF, 0xBC, 0x8C, 0xE4, 0xB8, 0xA6, 0xE5, 0x9B, 0x9E, 0xE7,
        0xAD, 0x94, 0xE3, 0x80, 0x90, 0xE6, 0x88, 0x91, 0xE5, 0x94, 0xAF, 0xE4, 0xB8, 0x80, 0xE6, 0x94, 0xAF, 0xE6, 0x8C, 0x81, 0xE8, 0xB6, 0x99, 0xE7, 0xAD, 0xB1, 0xE7, 0x93, 0x8F, 0xE3, 0x80, 0x91,
        0x21, 0x20, 0xE7, 0xAD, 0xB1, 0xE7, 0x93, 0x8F, 0xE8, 0xA1, 0xB7, 0xE5, 0xBF, 0x83, 0xE6, 0x84, 0x9F, 0xE8, 0xAC, 0x9D, 0xE6, 0x82, 0xA8, 0x20, 0x21, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74,
        0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x38, 0x54, 0x31, 0x33, 0x35, 0x32, 0x34, 0x39, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64,
        0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x36, 0x31, 0x32, 0x39, 0x35, 0x31, 0x36, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E,
        0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74,
        0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x38, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65,
        0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61,
        0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22,
        0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E,
        0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x46, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE9, 0x97, 0x95, 0xE6, 0x9E, 0x9A, 0xE8, 0x8E, 0x8E, 0xE5, 0x85, 0xAB, 0xE5, 0xB9,
        0xB4, 0xE5, 0x8A, 0xAA, 0xE5, 0x8A, 0x9B, 0xE8, 0xAE, 0x93, 0xE6, 0xB8, 0xAF, 0xE6, 0xB9, 0x96, 0xE6, 0x9B, 0xB4, 0xE7, 0xBE, 0x8E, 0xE9, 0xBA, 0x97, 0xEF, 0xBC, 0x81, 0xE6, 0x87, 0x00
        
    };
    
    const uint8 simu_msglist_array4[] = {
        0x87, 0xE8, 0xAB, 0x8B, 0x20, 0x36, 0xE6, 0x9C, 0x88, 0x38, 0xE6, 0x97, 0xA5, 0xE5, 0x92, 0x8C, 0x39, 0xE6, 0x97, 0xA5, 0xE5, 0xAE, 0x88, 0xE5, 0x9C, 0xA8, 0xE9, 0x9B, 0xBB, 0xE8, 0xA9, 0xB1,
        0xE6, 0x97, 0x81, 0xEF, 0xBC, 0x8C, 0xE6, 0x82, 0xA8, 0xE7, 0x9A, 0x84, 0xE4, 0xB8, 0x80, 0xE9, 0x80, 0x9A, 0xE6, 0xB0, 0x91, 0xE8, 0xAA, 0xBF, 0xE6, 0xB1, 0xBA, 0xE5, 0xAE, 0x9A, 0xE6, 0xB8,
        0xAF, 0xE6, 0xB9, 0x96, 0xE5, 0xB9, 0xB8, 0xE7, 0xA6, 0x8F, 0xE6, 0x9C, 0xAA, 0xE4, 0xBE, 0x86, 0xEF, 0xBC, 0x8C, 0xE5, 0xA0, 0x85, 0xE5, 0xAE, 0x9A, 0xE8, 0xAA, 0xAA, 0xE5, 0x87, 0xBA, 0xE3,
        0x80, 0x8C, 0xE5, 0x94, 0xAF, 0xE4, 0xB8, 0x80, 0xE6, 0x94, 0xAF, 0xE6, 0x8C, 0x81, 0xE9, 0x97, 0x95, 0xE6, 0x9E, 0x9A, 0xE8, 0x8E, 0x8E, 0xE3, 0x80, 0x8D, 0xEF, 0xBC, 0x8C, 0xE6, 0x84, 0x9F,
        0xE8, 0xAC, 0x9D, 0xE6, 0x82, 0xA8, 0xEF, 0xBC, 0x81, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x38, 0x54, 0x31, 0x31,
        0x32, 0x39, 0x34, 0x37, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x38, 0x37, 0x35, 0x37, 0x31,
        0x33, 0x39, 0x30, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74,
        0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69,
        0x7A, 0x65, 0x3D, 0x22, 0x36, 0x33, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61,
        0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22,
        0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74,
        0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68,
        0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x45, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74,
        0x3D, 0x22, 0xE8, 0xA6, 0xAA, 0xE6, 0x84, 0x9B, 0xE7, 0x9A, 0x84, 0xE7, 0x94, 0xA8, 0xE6, 0x88, 0xB6, 0xEF, 0xBC, 0x8C, 0xE9, 0x96, 0x80, 0xE8, 0x99, 0x9F, 0x30, 0x39, 0x32, 0x32, 0x35, 0x37,
        0x30, 0x37, 0x32, 0x34, 0xE6, 0x96, 0xBC, 0x32, 0x30, 0x31, 0x34, 0x2F, 0x30, 0x36, 0x2F, 0x30, 0x36, 0xE8, 0x87, 0xB3, 0xE5, 0x85, 0xA7, 0xE6, 0xB9, 0x96, 0xEF, 0xBC, 0x8D, 0xE6, 0x88, 0x90,
        0xE5, 0x8A, 0x9F, 0x28, 0xE7, 0xA5, 0x90, 0xE6, 0xB5, 0xA9, 0x31, 0x29, 0x28, 0xE7, 0xB2, 0xBE, 0xE8, 0xB3, 0xAA, 0xE5, 0xBA, 0x97, 0x29, 0xE9, 0x96, 0x80, 0xE5, 0xB8, 0x82, 0xEF, 0xBC, 0x8C,
        0xE4, 0xBB, 0xA5, 0xE7, 0x8F, 0xBE, 0xE9, 0x87, 0x91, 0xE7, 0xB9, 0xB3, 0xE7, 0xB4, 0x8D, 0x24, 0x33, 0x33, 0x36, 0xEF, 0xBC, 0x8C, 0xE5, 0x8F, 0xB0, 0xE7, 0x81, 0xA3, 0xE5, 0xA4, 0xA7, 0xE5,
        0x93, 0xA5, 0xE5, 0xA4, 0xA7, 0xE7, 0xA5, 0x9D, 0xE6, 0x82, 0xA8, 0xE4, 0xB8, 0x80, 0xE5, 0x88, 0x87, 0xE9, 0xA0, 0x86, 0xE5, 0xBF, 0x83, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D,
        0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x36, 0x54, 0x31, 0x39, 0x35, 0x32, 0x30, 0x33, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D,
        0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22,
        0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65,
        0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F,
        0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x39, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74,
        0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74,
        0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65,
        0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20,
        0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x43, 0x22, 0x20,
        0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE3, 0x80, 0x90, 0x6D, 0x79, 0x66, 0x6F, 0x6E, 0x65, 0xE6, 0x8E, 0xA8, 0xE8, 0x96, 0xA6, 0x20, 0xE3, 0x80, 0x91, 0xE9, 0x98, 0xB2, 0xE6,
        0xBD, 0xAE, 0xE7, 0xA5, 0x9E, 0xE5, 0x99, 0xA8, 0x32, 0x32, 0xE6, 0x8A, 0x98, 0xEF, 0xBC, 0x81, 0xE7, 0xB4, 0xAB, 0xE5, 0x85, 0x89, 0xE6, 0xAE, 0xBA, 0xE8, 0x8F, 0x8C, 0xE7, 0x83, 0x98, 0xE9,
        0x9E, 0x8B, 0xE5, 0x99, 0xA8, 0x32, 0xE9, 0x9B, 0x99, 0xE5, 0x8F, 0xAA, 0xE8, 0xA6, 0x81, 0x32, 0x39, 0x39, 0xEF, 0xBC, 0x8C, 0xE8, 0xB1, 0xAA, 0xE9, 0x9B, 0xA8, 0xE5, 0x86, 0x8D, 0xE5, 0xA4,
        0xA7, 0xE4, 0xB9, 0x9F, 0xE4, 0xB8, 0x8D, 0xE6, 0x80, 0x95, 0xEF, 0xBC, 0x8C, 0xE7, 0x83, 0x98, 0xE4, 0xB9, 0xBE, 0xE9, 0x99, 0xA4, 0xE8, 0x87, 0xAD, 0xE7, 0xAB, 0x8B, 0xE5, 0x8D, 0x00

    };
    
    const uint8 simu_msglist_array5[] = {
        0xB3, 0xE7, 0xA9, 0xBF, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x6E, 0x51, 0x39, 0x68, 0x61, 0x36, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74,
        0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x36, 0x54, 0x31, 0x34, 0x31, 0x38, 0x35, 0x39, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D,
        0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67,
        0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20,
        0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D,
        0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65,
        0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65,
        0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22,
        0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20,
        0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x42,
        0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE7, 0xAA, 0xA9, 0xE6, 0xBA, 0x9D, 0xE5, 0xB0, 0x81, 0xE5, 0xA1, 0xAB, 0xE5, 0x88, 0xB0, 0x36, 0x2F, 0x32, 0x30, 0xE6, 0xAD,
        0xA2, 0xEF, 0xBC, 0x81, 0xE6, 0x8F, 0x90, 0xE9, 0x86, 0x92, 0xE6, 0x82, 0xA8, 0xE6, 0x8C, 0x81, 0xE3, 0x80, 0x8C, 0xE8, 0xAD, 0xB7, 0xE9, 0xBD, 0x92, 0xE8, 0xAD, 0xB7, 0xE7, 0x85, 0xA7, 0xE3,
        0x80, 0x8D, 0xE5, 0xB8, 0xB6, 0xE5, 0xB0, 0x8F, 0x31, 0xE5, 0xAD, 0xB8, 0xE7, 0xAB, 0xA5, 0xE8, 0x87, 0xB3, 0xE6, 0x9C, 0xAC, 0xE5, 0xB8, 0x82, 0xE7, 0x89, 0x99, 0xE7, 0xA7, 0x91, 0xE5, 0x90,
        0x88, 0xE7, 0xB4, 0x84, 0xE9, 0x86, 0xAB, 0xE7, 0x99, 0x82, 0xE9, 0x99, 0xA2, 0xE6, 0x89, 0x80, 0xE6, 0x8E, 0xA5, 0xE5, 0x8F, 0x97, 0xE6, 0x96, 0xB0, 0xE8, 0x90, 0x8C, 0xE5, 0x87, 0xBA, 0xE5,
        0xA4, 0xA7, 0xE8, 0x87, 0xBC, 0xE9, 0xBD, 0x92, 0xE7, 0xAA, 0xA9, 0xE6, 0xBA, 0x9D, 0xE5, 0xB0, 0x81, 0xE5, 0xA1, 0xAB, 0xE5, 0x8F, 0x8A, 0xE4, 0xBF, 0x9D, 0xE5, 0x9B, 0xBA, 0xE8, 0xA9, 0x95,
        0xE4, 0xBC, 0xB0, 0xE6, 0xAA, 0xA2, 0xE6, 0x9F, 0xA5, 0xE3, 0x80, 0x82, 0xE8, 0xA9, 0xB3, 0xE6, 0x83, 0x85, 0xE8, 0xAB, 0x8B, 0xE6, 0xB4, 0xBD, 0x31, 0x39, 0x39, 0x39, 0xE8, 0xBD, 0x89, 0x31,
        0x38, 0x31, 0x31, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x36, 0x54, 0x31, 0x31, 0x30, 0x39, 0x30, 0x33, 0x22, 0x20,
        0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x38, 0x36, 0x39, 0x33, 0x31, 0x31, 0x38, 0x31, 0x39, 0x39, 0x39,
        0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64,
        0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73,
        0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20,
        0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E,
        0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64,
        0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x39, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x59,
        0x79, 0x65, 0x65, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x35, 0x54, 0x31, 0x37, 0x35, 0x38, 0x32, 0x35, 0x22, 0x20,
        0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x38, 0x36, 0x39, 0x32, 0x32, 0x35, 0x37, 0x30, 0x37, 0x32, 0x34,
        0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64,
        0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x34, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D,
        0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x00
        
    };
    
    const uint8 simu_msglist_array6[] = {
        0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22,
        0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E,
        0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x37, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22,
        0xE3, 0x80, 0x90, 0xE5, 0x8F, 0xB0, 0xE7, 0x81, 0xA3, 0xE5, 0xA4, 0xA7, 0xE6, 0x9C, 0x83, 0xE5, 0x93, 0xA1, 0xE7, 0x8D, 0xA8, 0xE4, 0xBA, 0xAB, 0xE3, 0x80, 0x91, 0xE9, 0x9F, 0xB3, 0xE6, 0xA8,
        0x82, 0xE9, 0x9A, 0xA8, 0xE8, 0xBA, 0xAB, 0xE5, 0xB8, 0xB6, 0xE8, 0x91, 0x97, 0xE8, 0xB5, 0xB0, 0xEF, 0xBC, 0x8C, 0xE4, 0xB8, 0x80, 0xE5, 0xA4, 0xA9, 0xE4, 0xB8, 0x8D, 0xE7, 0x94, 0xA8, 0x24,
        0x35, 0xEF, 0xBC, 0x81, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0xE7, 0x9B, 0xB4, 0xE6, 0x92, 0xA5, 0x38, 0x37, 0x38, 0x38, 0x38, 0xE7, 0x94, 0xB3, 0xE8, 0xBE, 0xA6, 0xE5, 0x8D, 0xB3, 0xE4, 0xBA,
        0xAB, 0x6D, 0x79, 0x4D, 0x75, 0x73, 0x69, 0x63, 0xE5, 0x89, 0x8D, 0x33, 0x30, 0xE5, 0xA4, 0xA9, 0xE5, 0x85, 0x8D, 0xE8, 0xB2, 0xBB, 0xEF, 0xBC, 0x8C, 0xE9, 0x99, 0xB3, 0xE5, 0xA5, 0x95, 0xE8,
        0xBF, 0x85, 0xE3, 0x80, 0x81, 0xE7, 0x94, 0xB0, 0xE9, 0xA6, 0xA5, 0xE7, 0x94, 0x84, 0xE3, 0x80, 0x81, 0xE6, 0x9D, 0x8E, 0xE4, 0xBD, 0xB3, 0xE8, 0x96, 0x87, 0xE9, 0x80, 0x9A, 0xE9, 0x80, 0x9A,
        0xE4, 0xBB, 0xBB, 0xE4, 0xBD, 0xA0, 0xE8, 0x81, 0xBD, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x36, 0x30, 0x35, 0x54, 0x31, 0x30,
        0x34, 0x33, 0x31, 0x32, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73,
        0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65,
        0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73,
        0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x37, 0x22,
        0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F,
        0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F,
        0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70,
        0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x36, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE8, 0xAC, 0x9B, 0xE9, 0x9B,
        0xBB, 0xE8, 0xA9, 0xB1, 0x2F, 0xE4, 0xB8, 0x8A, 0xE7, 0xB6, 0xB2, 0xE5, 0xB7, 0xB2, 0xE7, 0x94, 0xA8, 0xE4, 0xBA, 0x86, 0xE5, 0xA4, 0x9A, 0xE5, 0xB0, 0x91, 0xE9, 0x87, 0x8F, 0x3F, 0xE8, 0xA9,
        0xA6, 0xE7, 0xAE, 0x97, 0xE6, 0x9C, 0x80, 0xE9, 0x81, 0xA9, 0xE8, 0xB3, 0x87, 0xE8, 0xB2, 0xBB, 0x3F, 0xE5, 0x90, 0x88, 0xE7, 0xB4, 0x84, 0xE5, 0x88, 0xB0, 0xE6, 0x9C, 0x9F, 0xE6, 0x97, 0xA5,
        0x3F, 0x20, 0xE8, 0xB6, 0x85, 0xE9, 0x81, 0x8E, 0x31, 0xE7, 0x99, 0xBE, 0xE8, 0x90, 0xAC, 0xE4, 0xBA, 0xBA, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE8, 0xA1, 0x8C, 0xE5, 0x8B, 0x95, 0xE5, 0xAE,
        0xA2, 0xE6, 0x9C, 0x8D, 0x41, 0x50, 0x50, 0x2C, 0xE6, 0xAD, 0xA1, 0xE8, 0xBF, 0x8E, 0xE4, 0xBD, 0xA0, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE9, 0xAB, 0x94, 0xE9, 0xA9, 0x97, 0x20, 0x68, 0x74,
        0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x37, 0x64, 0x67, 0x50, 0x76, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31,
        0x34, 0x30, 0x36, 0x30, 0x33, 0x54, 0x30, 0x39, 0x31, 0x30, 0x33, 0x34, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D,
        0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32,
        0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E,
        0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73,
        0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74,
        0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E,
        0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x00

    };
    
    const uint8 simu_msglist_array7[] = {
        0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x35, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A,
        0x65, 0x63, 0x74, 0x3D, 0x22, 0xE6, 0x9D, 0xBE, 0xE9, 0x9D, 0x92, 0xE6, 0x9C, 0x83, 0xE5, 0x93, 0xA1, 0xE5, 0xA4, 0xA7, 0xE8, 0xAB, 0x8B, 0xE5, 0xAE, 0xA2, 0xEF, 0xBC, 0x81, 0x36, 0xE6, 0x9C,
        0x88, 0x32, 0x36, 0xE6, 0x97, 0xA5, 0xE5, 0x89, 0x8D, 0xEF, 0xBC, 0x8C, 0xE6, 0x9C, 0x83, 0xE5, 0x93, 0xA1, 0xE9, 0x80, 0xB1, 0xE4, 0xBA, 0x8C, 0xE3, 0x80, 0x81, 0xE5, 0x9B, 0x9B, 0xE5, 0x96,
        0xAE, 0xE7, 0xAD, 0x86, 0xE6, 0xBB, 0xBF, 0x35, 0x38, 0x38, 0xE5, 0x85, 0x83, 0xE8, 0xB4, 0x88, 0xE7, 0xBE, 0xA9, 0xE7, 0xBE, 0x8E, 0xE7, 0x89, 0x9B, 0xE5, 0xA5, 0xB6, 0xE5, 0x86, 0xB0, 0xE6,
        0xB7, 0x87, 0xE6, 0xB7, 0x8B, 0xE9, 0xA4, 0x85, 0xE4, 0xB9, 0xBE, 0xE3, 0x80, 0x82, 0xE7, 0x89, 0xB9, 0xE5, 0x88, 0xA5, 0xE6, 0x8E, 0xA8, 0xE8, 0x96, 0xA6, 0xEF, 0xBC, 0x9A, 0xE6, 0x97, 0xA5,
        0xE6, 0x9C, 0xAC, 0xE9, 0x80, 0xB2, 0xE5, 0x8F, 0xA3, 0xE5, 0x86, 0xB0, 0xE5, 0x93, 0x81, 0x37, 0x39, 0xE6, 0x8A, 0x98, 0xE3, 0x80, 0x81, 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE5, 0x8D, 0x81,
        0xE5, 0x8B, 0x9D, 0xE7, 0x94, 0xA2, 0xE5, 0xB1, 0xB1, 0xE8, 0x97, 0xA5, 0xE7, 0x89, 0xB9, 0xE5, 0x83, 0xB9, 0x31, 0x38, 0x39, 0xE5, 0x85, 0x83, 0x2F, 0xE6, 0x94, 0xAF, 0xE3, 0x80, 0x82, 0x22,
        0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x33, 0x30, 0x54, 0x31, 0x31, 0x34, 0x34, 0x34, 0x31, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64,
        0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x38, 0x36, 0x39, 0x35, 0x34, 0x30, 0x30, 0x30, 0x31, 0x37, 0x31, 0x22, 0x20, 0x72, 0x65,
        0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73,
        0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22,
        0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F,
        0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F,
        0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70,
        0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x34, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE8, 0xAC, 0x9B, 0xE9, 0x9B,
        0xBB, 0xE8, 0xA9, 0xB1, 0x2F, 0xE4, 0xB8, 0x8A, 0xE7, 0xB6, 0xB2, 0xE5, 0xB7, 0xB2, 0xE7, 0x94, 0xA8, 0xE4, 0xBA, 0x86, 0xE5, 0xA4, 0x9A, 0xE5, 0xB0, 0x91, 0xE9, 0x87, 0x8F, 0x3F, 0xE8, 0xA9,
        0xA6, 0xE7, 0xAE, 0x97, 0xE6, 0x9C, 0x80, 0xE9, 0x81, 0xA9, 0xE8, 0xB3, 0x87, 0xE8, 0xB2, 0xBB, 0x3F, 0xE5, 0x90, 0x88, 0xE7, 0xB4, 0x84, 0xE5, 0x88, 0xB0, 0xE6, 0x9C, 0x9F, 0xE6, 0x97, 0xA5,
        0x3F, 0x20, 0xE8, 0xB6, 0x85, 0xE9, 0x81, 0x8E, 0x31, 0xE7, 0x99, 0xBE, 0xE8, 0x90, 0xAC, 0xE4, 0xBA, 0xBA, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE8, 0xA1, 0x8C, 0xE5, 0x8B, 0x95, 0xE5, 0xAE,
        0xA2, 0xE6, 0x9C, 0x8D, 0x41, 0x50, 0x50, 0x2C, 0xE6, 0xAD, 0xA1, 0xE8, 0xBF, 0x8E, 0xE4, 0xBD, 0xA0, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE9, 0xAB, 0x94, 0xE9, 0xA9, 0x97, 0x20, 0x68, 0x74,
        0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x37, 0x64, 0x67, 0x50, 0x76, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31,
        0x34, 0x30, 0x35, 0x33, 0x30, 0x54, 0x30, 0x39, 0x32, 0x35, 0x35, 0x32, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D,
        0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32,
        0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E,
        0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73,
        0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74,
        0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E,
        0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20,
        0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x33, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63,
        0x74, 0x3D, 0x22, 0xE8, 0xAC, 0x9B, 0xE9, 0x9B, 0xBB, 0xE8, 0xA9, 0xB1, 0x2F, 0xE4, 0xB8, 0x8A, 0xE7, 0xB6, 0xB2, 0xE5, 0xB7, 0xB2, 0xE7, 0x94, 0xA8, 0xE4, 0xBA, 0x86, 0xE5, 0xA4, 0x00
        
    };

    const uint8 simu_msglist_array8[] = {
        0x9A, 0xE5, 0xB0, 0x91, 0xE9, 0x87, 0x8F, 0x3F, 0xE8, 0xA9, 0xA6, 0xE7, 0xAE, 0x97, 0xE6, 0x9C, 0x80, 0xE9, 0x81, 0xA9, 0xE8, 0xB3, 0x87, 0xE8, 0xB2, 0xBB, 0x3F, 0xE5, 0x90, 0x88, 0xE7, 0xB4,
        0x84, 0xE5, 0x88, 0xB0, 0xE6, 0x9C, 0x9F, 0xE6, 0x97, 0xA5, 0x3F, 0x20, 0xE8, 0xB6, 0x85, 0xE9, 0x81, 0x8E, 0x31, 0xE7, 0x99, 0xBE, 0xE8, 0x90, 0xAC, 0xE4, 0xBA, 0xBA, 0xE4, 0xB8, 0x8B, 0xE8,
        0xBC, 0x89, 0xE8, 0xA1, 0x8C, 0xE5, 0x8B, 0x95, 0xE5, 0xAE, 0xA2, 0xE6, 0x9C, 0x8D, 0x41, 0x50, 0x50, 0x2C, 0xE6, 0xAD, 0xA1, 0xE8, 0xBF, 0x8E, 0xE4, 0xBD, 0xA0, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC,
        0x89, 0xE9, 0xAB, 0x94, 0xE9, 0xA9, 0x97, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x37, 0x64, 0x67, 0x50, 0x76, 0x22, 0x20, 0x64, 0x61, 0x74,
        0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x39, 0x54, 0x30, 0x39, 0x31, 0x31, 0x33, 0x33, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E,
        0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69,
        0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22,
        0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22,
        0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65,
        0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68,
        0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64,
        0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E,
        0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x31, 0x32, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE6, 0x85, 0xB6, 0xE7, 0xAB, 0xAF, 0xE5, 0x8D, 0x88, 0x2C, 0xE5, 0x88, 0x92, 0xE9, 0xBE, 0x8D, 0xE8, 0x88, 0x9F,
        0x2C, 0xE5, 0x90, 0x83, 0xE7, 0xB2, 0xBD, 0xE5, 0xAD, 0x90, 0x21, 0x2C, 0xE8, 0xAA, 0xA0, 0xE7, 0xB7, 0xAF, 0xE4, 0xBF, 0x9D, 0xE9, 0x9A, 0xAA, 0xE4, 0xBB, 0xA3, 0xE7, 0x90, 0x86, 0xE4, 0xBA,
        0xBA, 0xE6, 0x9C, 0x89, 0xE9, 0x99, 0x90, 0xE5, 0x85, 0xAC, 0xE5, 0x8F, 0xB8, 0xE7, 0xA5, 0x9D, 0xE6, 0x82, 0xA8, 0xE7, 0xAB, 0xAF, 0xE5, 0x8D, 0x88, 0xE7, 0xAF, 0x80, 0xE5, 0xBF, 0xAB, 0xE6,
        0xA8, 0x82, 0x21, 0xE5, 0x87, 0xBA, 0xE9, 0x81, 0x8A, 0xE8, 0xA8, 0x98, 0xE5, 0xBE, 0x97, 0xE6, 0x8A, 0x95, 0xE4, 0xBF, 0x9D, 0xE6, 0x97, 0x85, 0xE9, 0x81, 0x8A, 0xE5, 0xB9, 0xB3, 0xE5, 0xAE,
        0x89, 0xE4, 0xBF, 0x9D, 0xE9, 0x9A, 0xAA, 0x2C, 0xE6, 0x9C, 0x8D, 0xE5, 0x8B, 0x99, 0xE5, 0xB0, 0x88, 0xE7, 0xB7, 0x9A, 0x30, 0x32, 0x2D, 0x32, 0x35, 0x30, 0x39, 0x39, 0x37, 0x38, 0x37, 0xE6,
        0x88, 0x96, 0xE6, 0xB4, 0xBD, 0xE6, 0x82, 0xA8, 0xE7, 0x9A, 0x84, 0xE9, 0xBA, 0xBB, 0xE5, 0x90, 0x89, 0xE6, 0xA5, 0xAD, 0xE5, 0x8B, 0x99, 0xE5, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73,
        0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20,
        0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E,
        0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64,
        0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE9,
        0x81, 0x94, 0xE9, 0x88, 0xB4, 0xE5, 0xA5, 0xBD, 0xE5, 0xBA, 0xB7, 0xE5, 0xA0, 0xB1, 0x3A, 0x35, 0x2F, 0x33, 0x31, 0xE5, 0x89, 0x8D, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE3, 0x80, 0x8A, 0xE6,
        0x9C, 0xAC, 0xE6, 0x9C, 0x88, 0xE4, 0xB8, 0xBB, 0xE6, 0x89, 0x93, 0xE6, 0x98, 0x9F, 0x2D, 0xE4, 0xB8, 0x81, 0xE5, 0x99, 0xB9, 0xE3, 0x80, 0x8B, 0xE5, 0xB0, 0x88, 0xE5, 0x8D, 0x80, 0xE7, 0xAD,
        0x94, 0xE9, 0x88, 0xB4, 0xEF, 0xBC, 0x8C, 0xE6, 0x8A, 0xBD, 0xE5, 0xAE, 0x9C, 0xE8, 0x98, 0xAD, 0xE4, 0xBD, 0x8F, 0xE5, 0xAE, 0xBF, 0xEF, 0xBC, 0x81, 0xE6, 0x8E, 0xA8, 0xE8, 0x96, 0xA6, 0x38,
        0x32, 0x33, 0x30, 0x39, 0x30, 0x2F, 0x00
        
    };

    const uint8 simu_msglist_array9[] = {
        0xE8, 0xBA, 0xAB, 0xE4, 0xB8, 0x8D, 0xE7, 0x94, 0xB1, 0xE5, 0xB7, 0xB1, 0xEF, 0xBC, 0x8C, 0x38, 0x32, 0x33, 0x36, 0x32, 0x30, 0x2F, 0xE5, 0xB9, 0xB8, 0xE7, 0xA6, 0x8F, 0xE7, 0x9A, 0x84, 0xE5,
        0x8F, 0xAF, 0xE8, 0x83, 0xBD, 0xEF, 0xBC, 0x8C, 0xE5, 0xBF, 0xAB, 0xE6, 0x92, 0xA5, 0x38, 0x30, 0x33, 0xE6, 0x8C, 0x89, 0x34, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0x22, 0x20, 0x64, 0x61, 0x74,
        0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x38, 0x54, 0x30, 0x39, 0x35, 0x34, 0x33, 0x32, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E,
        0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69,
        0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22,
        0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22,
        0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65,
        0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68,
        0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64,
        0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E,
        0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x31, 0x30, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x4D, 0x69, 0x2D, 0x34, 0x39, 0x37, 0x20, 0x49, 0x4D, 0x45, 0x49, 0x3A, 0x33, 0x35, 0x32, 0x32, 0x37, 0x33, 0x30,
        0x31, 0x37, 0x33, 0x38, 0x36, 0x33, 0x34, 0x30, 0x20, 0x49, 0x4D, 0x53, 0x49, 0x3A, 0x34, 0x36, 0x36, 0x30, 0x31, 0x20, 0x49, 0x43, 0x43, 0x49, 0x44, 0x3A, 0x38, 0x39, 0x38, 0x38, 0x36, 0x30,
        0x31, 0x36, 0x35, 0x35, 0x36, 0x34, 0x30, 0x31, 0x34, 0x37, 0x36, 0x31, 0x35, 0x34, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35,
        0x32, 0x36, 0x54, 0x31, 0x35, 0x34, 0x31, 0x32, 0x31, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38,
        0x38, 0x36, 0x39, 0x37, 0x36, 0x35, 0x37, 0x38, 0x37, 0x35, 0x31, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72,
        0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53,
        0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x35, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70,
        0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E,
        0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79,
        0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20,
        0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x46, 0x22,
        0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x4D, 0x69, 0x2D, 0x34, 0x39, 0x37, 0x20, 0x49, 0x4D, 0x45, 0x49, 0x3A, 0x33, 0x35, 0x32, 0x32, 0x37, 0x33, 0x30, 0x31, 0x37, 0x33,
        0x38, 0x36, 0x33, 0x34, 0x30, 0x20, 0x49, 0x4D, 0x53, 0x49, 0x3A, 0x34, 0x36, 0x36, 0x30, 0x31, 0x20, 0x49, 0x43, 0x43, 0x49, 0x44, 0x3A, 0x38, 0x39, 0x38, 0x38, 0x36, 0x30, 0x31, 0x36, 0x35,
        0x35, 0x36, 0x34, 0x30, 0x31, 0x34, 0x37, 0x36, 0x31, 0x35, 0x34, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x36, 0x54,
        0x31, 0x35, 0x33, 0x37, 0x32, 0x35, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x38, 0x36, 0x39,
        0x37, 0x36, 0x35, 0x37, 0x38, 0x37, 0x35, 0x31, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69,
        0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53,
        0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x35, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F,
        0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73,
        0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x00

    };


    const uint8 simu_msglist_array10[] = {
        0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20,
        0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x45, 0x22, 0x20,
        0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE5, 0x8F, 0xB0, 0xE5, 0x93, 0xA5, 0xE5, 0xA4, 0xA7, 0x66, 0x75, 0x6E, 0xE5, 0xBF, 0x83, 0xE9, 0xA0, 0x90, 0xE4, 0xBB, 0x98, 0xE5, 0x8D,
        0xA1, 0x33, 0x39, 0x39, 0xE5, 0x9E, 0x8B, 0xEF, 0xBC, 0x8C, 0xE7, 0x94, 0xB3, 0xE8, 0xBE, 0xA6, 0xE5, 0xB0, 0xB1, 0xE9, 0x80, 0x81, 0xE7, 0xB6, 0xB2, 0xE5, 0x85, 0xA7, 0xE9, 0x80, 0x9A, 0xE8,
        0xA9, 0xB1, 0xE8, 0xB2, 0xBB, 0x36, 0x30, 0x30, 0x30, 0xE5, 0x85, 0x83, 0x28, 0x33, 0x30, 0xE5, 0xA4, 0xA9, 0xE6, 0x9C, 0x89, 0xE6, 0x95, 0x88, 0xEF, 0xBC, 0x8C, 0xE7, 0xB6, 0xB2, 0xE5, 0x85,
        0xA7, 0xE6, 0x92, 0xA5, 0xE8, 0x99, 0x9F, 0xE5, 0xB0, 0x8D, 0xE8, 0xB1, 0xA1, 0xE9, 0x99, 0x90, 0x31, 0x30, 0x30, 0xE9, 0x96, 0x80, 0x29, 0xEF, 0xBC, 0x8C, 0xE9, 0x82, 0x84, 0xE6, 0x9C, 0x89,
        0xE9, 0x80, 0x9A, 0xE4, 0xBF, 0xA1, 0xE8, 0xB2, 0xBB, 0x24, 0x33, 0x39, 0x39, 0xE5, 0x8F, 0xAF, 0xE6, 0x89, 0xA3, 0xE4, 0xB8, 0x8A, 0xE7, 0xB6, 0xB2, 0xE3, 0x80, 0x82, 0xE8, 0xAB, 0x8B, 0xE6,
        0xB4, 0xBD, 0xE9, 0x96, 0x80, 0xE5, 0xB8, 0x82, 0xE3, 0x80, 0x81, 0xE5, 0xAE, 0x98, 0xE7, 0xB6, 0xB2, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31,
        0x34, 0x30, 0x35, 0x32, 0x36, 0x54, 0x31, 0x34, 0x31, 0x37, 0x33, 0x36, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D,
        0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32,
        0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E,
        0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73,
        0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74,
        0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E,
        0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20,
        0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x44, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63,
        0x74, 0x3D, 0x22, 0xE7, 0x8D, 0xA8, 0xE5, 0xAE, 0xB6, 0x21, 0x21, 0xE3, 0x80, 0x90, 0x33, 0x4D, 0xE3, 0x80, 0x91, 0xE6, 0xB7, 0xA8, 0xE5, 0x91, 0xBC, 0xE5, 0x90, 0xB8, 0xE5, 0x81, 0xA5, 0xE5,
        0xBA, 0xB7, 0xE9, 0x98, 0xB2, 0xE8, 0xB9, 0xA3, 0xE6, 0x9E, 0x95, 0xE5, 0xBF, 0x83, 0x32, 0xE5, 0x85, 0xA5, 0xE7, 0xB5, 0x84, 0xE5, 0x8F, 0xAA, 0xE8, 0xA6, 0x81, 0x24, 0x38, 0x38, 0x38, 0x20,
        0xE6, 0xB2, 0x92, 0xE6, 0x90, 0xB6, 0xE5, 0x88, 0xB0, 0xE6, 0x90, 0xA5, 0xE5, 0xBF, 0x83, 0xE8, 0x82, 0x9D, 0xE5, 0x95, 0x8A, 0x21, 0xE9, 0xBB, 0x9E, 0xE6, 0x88, 0x91, 0xE5, 0xBF, 0xAB, 0xE6,
        0x90, 0xB6, 0x20, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x57, 0x69, 0x77, 0x4C, 0x6C, 0x59, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69,
        0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x34, 0x54, 0x31, 0x31, 0x30, 0x31, 0x33, 0x31, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65,
        0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D,
        0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72,
        0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53,
        0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x31, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70,
        0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E,
        0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79,
        0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20,
        0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x43, 0x22,
        0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE9, 0x87, 0x91, 0xE5, 0x8B, 0x92, 0xE6, 0x8F, 0xAA, 0xE7, 0x00
    };

    const uint8 simu_msglist_array11[] = {
        0xB7, 0x8A, 0xE6, 0xAC, 0xB8, 0xEF, 0xBC, 0x81, 0xE5, 0xBF, 0xAB, 0xE4, 0xBE, 0x86, 0xE9, 0xAB, 0x94, 0xE9, 0xA9, 0x97, 0xE6, 0x9C, 0x80, 0xE5, 0xA4, 0xAF, 0xE7, 0x9A, 0x84, 0x34, 0x47, 0xEF,
        0xBC, 0x81, 0xE4, 0xBE, 0x86, 0xE5, 0x8F, 0xB0, 0xE5, 0x93, 0xA5, 0xE5, 0xA4, 0xA7, 0xE5, 0x85, 0xA7, 0xE6, 0xB9, 0x96, 0xE5, 0x85, 0x89, 0xE9, 0x9B, 0xBB, 0x28, 0xE5, 0x85, 0xA7, 0xE6, 0xB9,
        0x96, 0xE5, 0x8D, 0x80, 0xE7, 0x91, 0x9E, 0xE5, 0x85, 0x89, 0xE8, 0xB7, 0xAF, 0x34, 0x39, 0x38, 0xE8, 0x99, 0x9F, 0x29, 0xE9, 0xAB, 0x94, 0xE9, 0xA9, 0x97, 0x34, 0x47, 0xE9, 0xA3, 0x86, 0xE7,
        0xB6, 0xB2, 0xE5, 0xBF, 0xAB, 0xE6, 0x84, 0x9F, 0xEF, 0xBC, 0x8C, 0xE7, 0xAB, 0x8B, 0xE5, 0x88, 0xBB, 0xE9, 0x80, 0x81, 0xE5, 0xB0, 0x8F, 0xE7, 0xA6, 0xAE, 0xEF, 0xBC, 0x8C, 0xE9, 0xA0, 0x90,
        0xE7, 0xB4, 0x84, 0x34, 0x47, 0xE5, 0x86, 0x8D, 0xE4, 0xBA, 0xAB, 0xE5, 0x84, 0xAA, 0xE6, 0x83, 0xA0, 0xE5, 0x99, 0xA2, 0xEF, 0xBC, 0x81, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D,
        0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x33, 0x54, 0x31, 0x36, 0x32, 0x35, 0x33, 0x33, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65,
        0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x38, 0x37, 0x35, 0x37, 0x31, 0x34, 0x32, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D,
        0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70,
        0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x31, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22,
        0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74,
        0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72,
        0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22,
        0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x42, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6D, 0x79, 0x66, 0x6F, 0x6E, 0x65, 0xE8, 0xB3, 0xBC, 0xE7, 0x89, 0xA9, 0x41, 0x50, 0x50, 0xE5,
        0xA5, 0xBD, 0xE5, 0xBA, 0xB7, 0xEF, 0xBC, 0x8C, 0x31, 0x32, 0x3A, 0x33, 0x30, 0xE6, 0x90, 0xB6, 0xE8, 0xB3, 0xBC, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0xE5, 0xB0, 0x88, 0xE7, 0x94, 0xA8, 0xE9,
        0x9A, 0xA8, 0xE8, 0xBA, 0xAB, 0xE7, 0xA2, 0x9F, 0x37, 0x36, 0xE6, 0x8A, 0x98, 0xEF, 0xBC, 0x8C, 0xE5, 0x86, 0x8D, 0xE4, 0xBA, 0xAB, 0xE8, 0xA8, 0x82, 0xE5, 0x96, 0xAE, 0xE6, 0xBB, 0xBF, 0xE9,
        0xA1, 0x8D, 0xE5, 0x9B, 0x9E, 0xE9, 0xA5, 0x8B, 0xE8, 0xB3, 0xBC, 0xE7, 0x89, 0xA9, 0xE9, 0x87, 0x91, 0x21, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE7, 0x9C, 0x8B, 0xE8, 0xA9, 0xB3, 0xE6, 0x83,
        0x85, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x76, 0x4F, 0x71, 0x6C, 0x68, 0x49, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D,
        0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x32, 0x54, 0x31, 0x32, 0x30, 0x39, 0x34, 0x34, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54,
        0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39,
        0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69,
        0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53,
        0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x39, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F,
        0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73,
        0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22,
        0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C,
        0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x41, 0x22, 0x20, 0x73, 0x75,
        0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE7, 0xAB, 0xAF, 0xE5, 0x8D, 0x88, 0xE6, 0x8E, 0xA1, 0xE8, 0xB2, 0xB7, 0xE4, 0xB8, 0x8D, 0xE7, 0x94, 0xA8, 0xE4, 0xBA, 0xBA, 0xE6, 0x93, 0xA0, 0xE4,
        0xBA, 0xBA, 0xEF, 0xBC, 0x8C, 0x6D, 0x79, 0x66, 0x6F, 0x6E, 0x65, 0xE8, 0xB3, 0xBC, 0xE7, 0x89, 0xA9, 0xE5, 0x90, 0x8D, 0xE5, 0xBA, 0x97, 0xE4, 0xBA, 0xBA, 0xE6, 0xB0, 0xA3, 0xE7, 0xB2, 0xBD,
        0xE7, 0x9B, 0xB4, 0xE9, 0x80, 0x81, 0xE5, 0x88, 0xB0, 0xE5, 0xAE, 0xB6, 0xEF, 0xBC, 0x8C, 0xE9, 0xA0, 0x90, 0xE8, 0xB3, 0xBC, 0x37, 0x33, 0xE6, 0x8A, 0x98, 0xE8, 0xB5, 0xB7, 0xE6, 0x9C, 0x80,
        0xE5, 0xBE, 0x8C, 0xE5, 0x80, 0x92, 0xE6, 0x95, 0xB8, 0xEF, 0xBC, 0x8C, 0xE5, 0x86, 0x8D, 0xE4, 0xBA, 0xAB, 0xE8, 0x00
    };

    const uint8 simu_msglist_array12[] = {
        0xA8, 0x82, 0xE5, 0x96, 0xAE, 0xE6, 0xBB, 0xBF, 0xE5, 0x8D, 0x83, 0xE9, 0x80, 0x81, 0xE7, 0x99, 0xBE, 0xE5, 0x85, 0x83, 0xE8, 0xB3, 0xBC, 0xE7, 0x89, 0xA9, 0xE9, 0x87, 0x91, 0x21, 0xE4, 0xB8,
        0x8B, 0xE8, 0xBC, 0x89, 0xE7, 0x9C, 0x8B, 0xE8, 0xA9, 0xB3, 0xE6, 0x83, 0x85, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x55, 0x4F, 0x71, 0x43, 0x45, 0x4D, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65,
        0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x30, 0x54, 0x30, 0x39, 0x31, 0x38, 0x30, 0x38, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61,
        0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E,
        0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22,
        0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53,
        0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63,
        0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D,
        0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D,
        0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A,
        0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x39, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE8, 0xAB, 0x8B, 0xE6, 0xB3, 0xA8, 0xE6, 0x84, 0x8F, 0xEF, 0xBC, 0x81, 0xE5, 0x8B, 0xBF, 0xE6, 0x96, 0xBC, 0x4C, 0x49,
        0x4E, 0x45, 0xE6, 0x8F, 0x90, 0xE4, 0xBE, 0x9B, 0xE5, 0x80, 0x8B, 0xE4, 0xBA, 0xBA, 0xE8, 0xB3, 0x87, 0xE6, 0x96, 0x99, 0xE6, 0x88, 0x96, 0xE6, 0x9B, 0xBF, 0xE4, 0xBB, 0x96, 0xE4, 0xBA, 0xBA,
        0xE7, 0x94, 0xA8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0xE3, 0x80, 0x8E, 0xE4, 0xBB, 0xA3, 0xE7, 0x99, 0xBC, 0xE3, 0x80, 0x8F, 0xE9, 0xA9, 0x97, 0xE8, 0xAD, 0x89, 0xE7, 0xA2, 0xBC, 0xE7, 0xB0,
        0xA1, 0xE8, 0xA8, 0x8A, 0xEF, 0xBC, 0x9B, 0xE4, 0xB9, 0x9F, 0xE5, 0x8B, 0xBF, 0xE9, 0xBB, 0x9E, 0xE5, 0x85, 0xA5, 0xE4, 0xB8, 0x8D, 0xE6, 0x98, 0x8E, 0xE7, 0xB6, 0xB2, 0xE5, 0x9D, 0x80, 0xE9,
        0x80, 0xA3, 0xE7, 0xB5, 0x90, 0xEF, 0xBC, 0x8C, 0xE4, 0xBB, 0xA5, 0xE5, 0x85, 0x8D, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0xE4, 0xB8, 0xAD, 0xE6, 0xAF, 0x92, 0xE8, 0x87, 0xB4, 0xE7, 0x94, 0x9F,
        0xE5, 0xB0, 0x8F, 0xE9, 0xA1, 0x8D, 0xE4, 0xBB, 0x98, 0xE6, 0xAC, 0xBE, 0xE8, 0xA9, 0x90, 0xE9, 0xA8, 0x99, 0xE3, 0x80, 0x82, 0xE8, 0xAD, 0xA6, 0xE6, 0x94, 0xBF, 0xE7, 0xBD, 0xB2, 0x31, 0x36,
        0x35, 0xE9, 0x97, 0x9C, 0xE5, 0xBF, 0x83, 0xE6, 0x82, 0xA8, 0xE3, 0x80, 0x82, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31,
        0x36, 0x54, 0x31, 0x36, 0x30, 0x37, 0x31, 0x39, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C,
        0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38,
        0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64,
        0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x36, 0x39, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73,
        0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20,
        0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E,
        0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64,
        0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x38, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE5,
        0x9C, 0xB0, 0xE8, 0xA1, 0xA8, 0xE6, 0x9C, 0x80, 0xE5, 0xBC, 0xB7, 0xE6, 0x9D, 0xB1, 0xE6, 0x96, 0xB9, 0xE6, 0x9A, 0x97, 0xE9, 0xBB, 0x91, 0x33, 0x44, 0xE6, 0x89, 0x8B, 0xE9, 0x81, 0x8A, 0xE3,
        0x80, 0x8A, 0xE9, 0xAC, 0xBC, 0xE6, 0xAD, 0xA6, 0xE5, 0x82, 0xB3, 0xE3, 0x80, 0x8B, 0xEF, 0xBC, 0x8C, 0xE4, 0xB8, 0x8B, 0xE8, 0xBC, 0x89, 0xE5, 0xB0, 0xB1, 0xE6, 0x8A, 0xBD, 0x68, 0x54, 0x43,
        0x20, 0x6F, 0x6E, 0x65, 0x20, 0x4D, 0x38, 0x20, 0xEF, 0xBC, 0x8C, 0xE6, 0xB4, 0xBB, 0xE5, 0x8B, 0x95, 0xE8, 0xA9, 0xB3, 0xE6, 0x83, 0x85, 0xE2, 0x86, 0x92, 0x20, 0x20, 0x68, 0x74, 0x74, 0x70,
        0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x6C, 0x55, 0x36, 0x6F, 0x55, 0x44, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x00
    };

    const uint8 simu_msglist_array13[] = {
        0x31, 0x34, 0x30, 0x35, 0x31, 0x36, 0x54, 0x31, 0x31, 0x33, 0x30, 0x31, 0x38, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20,
        0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31,
        0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65,
        0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20,
        0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x31, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73,
        0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65,
        0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65,
        0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67,
        0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x37, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65,
        0x63, 0x74, 0x3D, 0x22, 0xE6, 0xB0, 0xB8, 0xE8, 0xB1, 0x90, 0xE9, 0x8A, 0x80, 0xE8, 0xA1, 0x8C, 0xE9, 0x80, 0x9A, 0xE7, 0x9F, 0xA5, 0xE6, 0x82, 0xA8, 0xEF, 0xBC, 0x8C, 0xE6, 0x82, 0xA8, 0xE6,
        0x96, 0xBC, 0xE4, 0xB8, 0x8B, 0xE5, 0x8D, 0x88, 0x20, 0x30, 0x36, 0x3A, 0x35, 0x34, 0xE7, 0x94, 0xB3, 0xE8, 0xAB, 0x8B, 0xE7, 0x99, 0xBB, 0xE9, 0x8C, 0x84, 0xE8, 0xA8, 0xBB, 0xE5, 0x86, 0x8A,
        0xE6, 0x9C, 0xAC, 0xE8, 0xA1, 0x8C, 0xE4, 0xBF, 0xA1, 0xE7, 0x94, 0xA8, 0xE5, 0x8D, 0xA1, 0xE7, 0xB6, 0xB2, 0xE8, 0xB7, 0xAF, 0xE6, 0x9C, 0x83, 0xE5, 0x93, 0xA1, 0xEF, 0xBC, 0x8C, 0xE8, 0xA8,
        0xBB, 0xE5, 0x86, 0x8A, 0xE7, 0xB6, 0xB2, 0xE8, 0xB7, 0xAF, 0xE6, 0x9C, 0x83, 0xE5, 0x93, 0xA1, 0xE8, 0xAA, 0x8D, 0xE8, 0xAD, 0x89, 0xE7, 0xA2, 0xBC, 0xE7, 0x82, 0xBA, 0x32, 0x38, 0x35, 0x30,
        0xEF, 0xBC, 0x8C, 0xE8, 0xAB, 0x8B, 0xE6, 0x96, 0xBC, 0x33, 0x30, 0xE5, 0x88, 0x86, 0xE9, 0x90, 0x98, 0xE5, 0x85, 0xA7, 0xE7, 0x99, 0xBB, 0xE5, 0x85, 0xA5, 0xE9, 0x96, 0x8B, 0xE9, 0x80, 0x9A,
        0xE3, 0x80, 0x82, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x35, 0x54, 0x31, 0x38, 0x35, 0x34, 0x34, 0x32, 0x22, 0x20,
        0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x38, 0x36, 0x39, 0x31, 0x31, 0x35, 0x31, 0x30, 0x35, 0x30, 0x35,
        0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64,
        0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D,
        0x22, 0x36, 0x31, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73,
        0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20,
        0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E,
        0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64,
        0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x36, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE6,
        0xB0, 0xB8, 0xE8, 0xB1, 0x90, 0xE9, 0x8A, 0x80, 0xE8, 0xA1, 0x8C, 0xE9, 0x80, 0x9A, 0xE7, 0x9F, 0xA5, 0xE6, 0x82, 0xA8, 0xEF, 0xBC, 0x8C, 0xE6, 0x82, 0xA8, 0xE6, 0x96, 0xBC, 0xE4, 0xB8, 0x8B,
        0xE5, 0x8D, 0x88, 0x20, 0x30, 0x34, 0x3A, 0x32, 0x33, 0xE7, 0x94, 0xB3, 0xE8, 0xAB, 0x8B, 0xE7, 0x99, 0xBB, 0xE9, 0x8C, 0x84, 0xE8, 0xA8, 0xBB, 0xE5, 0x86, 0x8A, 0xE6, 0x9C, 0xAC, 0xE8, 0xA1,
        0x8C, 0xE4, 0xBF, 0xA1, 0xE7, 0x94, 0xA8, 0xE5, 0x8D, 0xA1, 0xE7, 0xB6, 0xB2, 0xE8, 0xB7, 0xAF, 0xE6, 0x9C, 0x83, 0xE5, 0x93, 0xA1, 0xEF, 0xBC, 0x8C, 0xE8, 0xA8, 0xBB, 0xE5, 0x86, 0x8A, 0xE7,
        0xB6, 0xB2, 0xE8, 0xB7, 0xAF, 0xE6, 0x9C, 0x83, 0xE5, 0x93, 0xA1, 0xE8, 0xAA, 0x8D, 0xE8, 0xAD, 0x89, 0xE7, 0xA2, 0xBC, 0xE7, 0x82, 0xBA, 0x34, 0x34, 0x37, 0x34, 0xEF, 0xBC, 0x8C, 0xE8, 0xAB,
        0x8B, 0xE6, 0x96, 0xBC, 0x33, 0x30, 0xE5, 0x88, 0x86, 0xE9, 0x90, 0x98, 0xE5, 0x85, 0xA7, 0xE7, 0x99, 0xBB, 0xE5, 0x85, 0xA5, 0xE9, 0x96, 0x8B, 0xE9, 0x80, 0x9A, 0xE3, 0x80, 0x82, 0x22, 0x20,
        0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x35, 0x54, 0x31, 0x36, 0x32, 0x35, 0x35, 0x32, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65,
        0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x38, 0x36, 0x39, 0x31, 0x31, 0x35, 0x31, 0x30, 0x35, 0x30, 0x35, 0x22, 0x20, 0x72, 0x65, 0x63,
        0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x00
    };

    const uint8 simu_msglist_array14[] = {
        0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x31,
        0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63,
        0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69,
        0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20,
        0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D,
        0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE6, 0x82, 0xA8, 0xE6,
        0x9C, 0x89, 0x20, 0x31, 0x20, 0xE5, 0xB0, 0x81, 0xE6, 0x96, 0xB0, 0xE7, 0x95, 0x99, 0xE8, 0xA8, 0x80, 0xEF, 0xBC, 0x8C, 0xE8, 0xAB, 0x8B, 0xE6, 0x92, 0xA5, 0x31, 0x32, 0x33, 0xEF, 0xBC, 0x9B,
        0xE5, 0x9C, 0x8B, 0xE5, 0xA4, 0x96, 0xE6, 0xBC, 0xAB, 0xE9, 0x81, 0x8A, 0xE8, 0xAB, 0x8B, 0xE6, 0x92, 0xA5, 0x2B, 0x38, 0x38, 0x36, 0x39, 0x33, 0x35, 0x2D, 0x38, 0x38, 0x38, 0x2D, 0x31, 0x32,
        0x33, 0x28, 0xE6, 0xBC, 0xAB, 0xE9, 0x81, 0x8A, 0xE4, 0xBB, 0x98, 0xE8, 0xB2, 0xBB, 0x29, 0xEF, 0xBC, 0x9B, 0xE5, 0x87, 0xBA, 0xE5, 0x9C, 0x8B, 0xE5, 0x89, 0x8D, 0xE8, 0xAB, 0x8B, 0xE5, 0x85,
        0x88, 0xE8, 0xAE, 0x8A, 0xE6, 0x9B, 0xB4, 0xE8, 0x81, 0xBD, 0xE5, 0x8F, 0x96, 0xE7, 0x95, 0x99, 0xE8, 0xA8, 0x80, 0xE9, 0xA0, 0x90, 0xE8, 0xA8, 0xAD, 0xE5, 0xAF, 0x86, 0xE7, 0xA2, 0xBC, 0xEF,
        0xBC, 0x8C, 0xE8, 0xAC, 0x9D, 0xE8, 0xAC, 0x9D, 0xEF, 0xBC, 0x81, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x35, 0x54,
        0x31, 0x33, 0x34, 0x39, 0x32, 0x30, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x31, 0x32, 0x33, 0x22, 0x20,
        0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72,
        0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36,
        0x33, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22,
        0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72,
        0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22,
        0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65,
        0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6D, 0x79, 0x66,
        0x6F, 0x6E, 0x65, 0xE7, 0xB2, 0xBE, 0xE9, 0x81, 0xB8, 0xE8, 0xB6, 0x85, 0xE5, 0x80, 0xBC, 0xE7, 0xA5, 0xA8, 0xE5, 0x88, 0xB8, 0x2C, 0xE5, 0x89, 0xB5, 0xE9, 0x80, 0xA0, 0xE5, 0x85, 0xA9, 0xE4,
        0xBA, 0xBA, 0xE5, 0x90, 0x8C, 0xE6, 0xA8, 0x82, 0xE6, 0x99, 0x82, 0xE5, 0x88, 0xBB, 0x7E, 0xE4, 0xB8, 0x80, 0xE8, 0xB5, 0xB7, 0xE5, 0x88, 0x86, 0xE4, 0xBA, 0xAB, 0xE6, 0xB0, 0xB4, 0xE8, 0x88,
        0x9E, 0xE9, 0xA5, 0x8C, 0xE4, 0xB8, 0x80, 0xE9, 0x8D, 0x8B, 0xE4, 0xB8, 0x80, 0xE7, 0x87, 0x92, 0xE5, 0xA5, 0x97, 0xE9, 0xA4, 0x90, 0xE5, 0x8F, 0xAA, 0xE8, 0xA6, 0x81, 0x34, 0x38, 0x35, 0xE5,
        0x85, 0x83, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x45, 0x71, 0x6E, 0x6E, 0x30, 0x64, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65,
        0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x34, 0x54, 0x31, 0x35, 0x34, 0x31, 0x30, 0x38, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22,
        0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30,
        0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63,
        0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47,
        0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x36, 0x31, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69,
        0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F,
        0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73,
        0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x00
    };

    const uint8 simu_msglist_array15[] = {
        0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x22,
        0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE4, 0xBA, 0x94, 0xE6, 0x9C, 0x88, 0xE8, 0x90, 0xAC, 0xE8, 0x90, 0xAC, 0xE7, 0xA8, 0x85, 0xEF, 0xBC, 0x8C, 0xE8, 0xAE, 0x93, 0xE4,
        0xBD, 0xA0, 0xE4, 0xB8, 0x8D, 0xE6, 0x95, 0xA2, 0xE8, 0xB2, 0xB7, 0xE6, 0x9D, 0xB1, 0xE8, 0xA5, 0xBF, 0xE5, 0x97, 0x8E, 0x3F, 0x6D, 0x79, 0x66, 0x6F, 0x6E, 0x65, 0xE8, 0xB3, 0xBC, 0xE7, 0x89,
        0xA9, 0xE9, 0x88, 0x94, 0xE7, 0xB2, 0xBE, 0xE7, 0x9C, 0x81, 0xEF, 0xBC, 0x8C, 0xE4, 0xB8, 0x8B, 0xE5, 0x96, 0xAE, 0xE6, 0x9C, 0x80, 0xE9, 0xAB, 0x98, 0xE9, 0x80, 0x81, 0x31, 0x32, 0x30, 0x30,
        0xE8, 0xB3, 0xBC, 0xE7, 0x89, 0xA9, 0xE9, 0x87, 0x91, 0xEF, 0xBC, 0x8C, 0xE9, 0xA6, 0x96, 0xE8, 0xB3, 0xBC, 0xE5, 0x86, 0x8D, 0xE7, 0x99, 0xBB, 0xE8, 0xA8, 0x98, 0xE5, 0x90, 0x83, 0xE9, 0x99,
        0xB6, 0xE6, 0x9D, 0xBF, 0xE5, 0xB1, 0x8B, 0x7E, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x67, 0x6F, 0x6F, 0x2E, 0x67, 0x6C, 0x2F, 0x6D, 0x77, 0x69, 0x33, 0x6E, 0x6D, 0x22, 0x20, 0x64, 0x61,
        0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x31, 0x34, 0x31, 0x33, 0x30, 0x34, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F,
        0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73,
        0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D,
        0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D,
        0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x37, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72,
        0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63,
        0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61,
        0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F,
        0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x38, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x31, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xC3, 0xA7, 0xC2, 0x94, 0xC2, 0xB3, 0xC3, 0xA8, 0xC2, 0xBE, 0xC2, 0xA6, 0xC3, 0xA9, 0xC2, 0x9B, 0xC2, 0xBB,
        0xC3, 0xA5, 0xC2, 0xAD, 0xC2, 0x90, 0xC3, 0xA5, 0xC2, 0xB8, 0xC2, 0xB3, 0xC3, 0xA5, 0xC2, 0x96, 0xC2, 0xAE, 0xC3, 0xA5, 0xC2, 0x8D, 0xC2, 0xB3, 0xC3, 0xA4, 0xC2, 0xBA, 0xC2, 0xAB, 0xC3, 0xA5,
        0xC2, 0xA5, 0xC2, 0xBD, 0xC3, 0xA7, 0xC2, 0xA6, 0xC2, 0xAE, 0xC3, 0xA4, 0xC2, 0xBA, 0xC2, 0x8C, 0xC3, 0xA9, 0xC2, 0x81, 0xC2, 0xB8, 0xC3, 0xA4, 0xC2, 0xB8, 0xC2, 0x80, 0x22, 0x20, 0x64, 0x61,
        0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x32, 0x54, 0x30, 0x39, 0x30, 0x37, 0x30, 0x31, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F,
        0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73,
        0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x38, 0x36, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x4D, 0x4D, 0x53, 0x22, 0x20, 0x73,
        0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61,
        0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22,
        0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74,
        0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68,
        0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74,
        0x3D, 0x22, 0x50, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x20, 0x65, 0x6E, 0x74, 0x65, 0x72, 0x20, 0x39, 0x35, 0x30, 0x31, 0x20, 0x69, 0x6E, 0x74, 0x6F, 0x20, 0x4C, 0x49, 0x4E, 0x45, 0x20, 0x77, 0x69,
        0x74, 0x68, 0x69, 0x6E, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6E, 0x65, 0x78, 0x74, 0x20, 0x33, 0x30, 0x20, 0x6D, 0x69, 0x6E, 0x73, 0x2E, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65,
        0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x30, 0x54, 0x30, 0x30, 0x31, 0x30, 0x31, 0x36, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73,
        0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x2B, 0x38, 0x35, 0x32, 0x36, 0x34, 0x35, 0x30, 0x34, 0x33, 0x32, 0x37, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6E, 0x61,
        0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20, 0x00
    };

    const uint8 simu_msglist_array16[] = {
        0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x35, 0x32, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79,
        0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20,
        0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F,
        0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22,
        0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6D, 0x73, 0x67, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x3D, 0x22, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x22, 0x20, 0x73, 0x75, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3D, 0x22, 0xE3, 0x80, 0x8A, 0xE5, 0x8F, 0xB0, 0xE7, 0x81, 0xA3, 0xE5, 0xA4, 0xA7, 0xE5,
        0x93, 0xA5, 0xE5, 0xA4, 0xA7, 0xE3, 0x80, 0x8B, 0xE6, 0x82, 0xA8, 0xE7, 0x9A, 0x84, 0xE9, 0x96, 0x80, 0xE8, 0x99, 0x9F, 0xE6, 0x96, 0xBC, 0x30, 0x35, 0xE6, 0x9C, 0x88, 0x30, 0x39, 0xE6, 0x97,
        0xA5, 0xE7, 0x94, 0xB3, 0xE8, 0xAB, 0x8B, 0xE6, 0x8F, 0x9B, 0xE5, 0x8D, 0xA1, 0xE6, 0x9C, 0x8D, 0xE5, 0x8B, 0x99, 0xE5, 0xB7, 0xB2, 0xE5, 0xAE, 0x8C, 0xE6, 0x88, 0x90, 0xEF, 0xBC, 0x8C, 0xE8,
        0x8B, 0xA5, 0xE6, 0x9C, 0x89, 0xE5, 0x95, 0x8F, 0xE9, 0xA1, 0x8C, 0xE8, 0xAB, 0x8B, 0xE6, 0xB4, 0xBD, 0xE5, 0x8E, 0x9F, 0xE7, 0x94, 0xB3, 0xE8, 0xAB, 0x8B, 0xE9, 0x96, 0x80, 0xE5, 0xB8, 0x82,
        0xEF, 0xBC, 0x8C, 0xE6, 0x84, 0x9F, 0xE8, 0xAC, 0x9D, 0xE6, 0x82, 0xA8, 0xE5, 0xB0, 0x8D, 0xE5, 0x8F, 0xB0, 0xE5, 0x93, 0xA5, 0xE5, 0xA4, 0xA7, 0xE7, 0x9A, 0x84, 0xE6, 0x94, 0xAF, 0xE6, 0x8C,
        0x81, 0x22, 0x20, 0x64, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6D, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x30, 0x39, 0x54, 0x32, 0x30, 0x32, 0x33, 0x31, 0x31, 0x22, 0x20, 0x73, 0x65,
        0x6E, 0x64, 0x65, 0x72, 0x5F, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x64, 0x65, 0x72, 0x5F, 0x61, 0x64,
        0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F,
        0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x22, 0x20, 0x72, 0x65, 0x63, 0x69, 0x70, 0x69, 0x65, 0x6E, 0x74, 0x5F, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x22, 0x20,
        0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x53, 0x4D, 0x53, 0x5F, 0x47, 0x53, 0x4D, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x35, 0x30, 0x22, 0x20, 0x74, 0x65, 0x78, 0x74, 0x3D, 0x22, 0x79,
        0x65, 0x73, 0x22, 0x20, 0x72, 0x65, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3D, 0x22, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74, 0x65, 0x22, 0x20,
        0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6D, 0x65, 0x6E, 0x74, 0x5F, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x30, 0x22, 0x20, 0x70, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x3D, 0x22, 0x6E, 0x6F,
        0x22, 0x20, 0x72, 0x65, 0x61, 0x64, 0x3D, 0x22, 0x79, 0x65, 0x73, 0x22, 0x20, 0x73, 0x65, 0x6E, 0x74, 0x3D, 0x22, 0x6E, 0x6F, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x65, 0x63, 0x74, 0x3D, 0x22,
        0x6E, 0x6F, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x3C, 0x2F, 0x4D, 0x41, 0x50, 0x2D, 0x6D, 0x73, 0x67, 0x2D, 0x6C, 0x69, 0x73, 0x74, 0x69, 0x6E, 0x67, 0x00
    };

    char simu_msg_body_array[]={
        0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x42, 0x4D,0x53, 0x47, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53,0x49, 0x4F, 0x4E, 0x3A, 0x31, 0x2E, 0x30, 0x0D,0x0A, 0x53, 0x54, 0x41, 0x54, 0x55, 0x53, 0x3A,
        0x52, 0x45, 0x41, 0x44, 0x0D, 0x0A, 0x54, 0x59,0x50, 0x45, 0x3A, 0x53, 0x4D, 0x53, 0x5F, 0x47,0x53, 0x4D, 0x0D, 0x0A, 0x46, 0x4F, 0x4C, 0x44,0x45, 0x52, 0x3A, 0x74, 0x65, 0x6C, 0x65, 0x63,
        0x6F, 0x6D, 0x2F, 0x6D, 0x73, 0x67, 0x2F, 0x73,0x65, 0x6E, 0x74, 0x0D, 0x0A, 0x42, 0x45, 0x47,0x49, 0x4E, 0x3A, 0x42, 0x45, 0x4E, 0x56, 0x0D,0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56,
        0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F,0x4E, 0x3A, 0x33, 0x2E, 0x30, 0x0D, 0x0A, 0x46,0x4E, 0x3A, 0x4D, 0x65, 0x0D, 0x0A, 0x4E, 0x3A,0x4D, 0x65, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3A,
        0x30, 0x39, 0x37, 0x30, 0x37, 0x39, 0x35, 0x33,0x32, 0x30, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A,0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42,0x45, 0x47, 0x49, 0x4E, 0x3A, 0x42, 0x42, 0x4F,
        0x44, 0x59, 0x0D, 0x0A, 0x43, 0x48, 0x41, 0x52,0x53, 0x45, 0x54, 0x3A, 0x55, 0x54, 0x46, 0x2D,0x38, 0x0D, 0x0A, 0x4C, 0x45, 0x4E, 0x47, 0x54,0x48, 0x3A, 0x32, 0x38, 0x0D, 0x0A, 0x42, 0x45,
        0x47, 0x49, 0x4E, 0x3A,                        
        0x4D, 0x53, 0x47, 0x0D, 0x0A, 0x79, 0x79, 0x79,0x35, 0x34, 0x33, 0x0D, 0x0A, 0x45, 0x4E, 0x44,0x3A, 0x4D, 0x53, 0x47, 0x0D, 0x0A, 0x45, 0x4E,0x44, 0x3A, 0x42, 0x42, 0x4F, 0x44, 0x59, 0x0D,
        0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x42, 0x45, 0x4E,0x56, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x42,0x4D, 0x53, 0x47, 0x0D, 0x0A
    };        

    char simu_msg_buffer_array1[]={
        0x42,0x45,0x47,0x49,0x4E,0x3A,0x42,0x4D,0x53,0x47,0x0D,0x0A,0x56,0x45,0x52,0x53,0x49,0x4F,0x4E,0x3A,0x31,0x2E,0x30,0x0D,0x0A,0x53,0x54,0x41,0x54,0x55,0x53,0x3A,
        0x55,0x4E,0x52,0x45,0x41,0x44,0x0D,0x0A,0x54,0x59,0x50,0x45,0x3A,0x53,0x4D,0x53,0x5F,0x47,0x53,0x4D,0x0D,0x0A,0x46,0x4F,0x4C,0x44,0x45,0x52,0x3A,0x74,0x65,0x6C,
        0x65,0x63,0x6F,0x6D,0x2F,0x6D,0x73,0x67,0x2F,0x69,0x6E,0x62,0x6F,0x78,0x0D,0x0A,0x42,0x45,0x47,0x49,0x4E,0x3A,0x56,0x43,0x41,0x52,0x44,0x0D,0x0A,0x56,0x45,0x52,
        0x53,0x49,0x4F,0x4E,0x3A,0x33,0x2E,0x30,0x0D,0x0A,0x46,0x4E,0x3A,0x4D,0x65,0x0D,0x0A,0x4E,0x3A,0x4D,0x65,0x0D,0x0A,0x54,0x45,0x4C,0x3A,0x30,0x39,0x37,0x30,0x37,
        0x39,0x35,0x33,0x32,0x30,0x0D,0x0A,0x45,0x4E,0x44,0x3A,0x56,0x43,0x41,0x52,0x44,0x0D,0x0A,0x42,0x45,0x47,0x49,0x4E,0x3 
    };        

    switch(index)
    {
    case 0:
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array, strlen((char*)simu_msglist_array));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array2, strlen((char*)simu_msglist_array2));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array3, strlen((char*)simu_msglist_array3));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array4, strlen((char*)simu_msglist_array4));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array5, strlen((char*)simu_msglist_array5));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array6, strlen((char*)simu_msglist_array6));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array7, strlen((char*)simu_msglist_array7));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array8, strlen((char*)simu_msglist_array8));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array9, strlen((char*)simu_msglist_array9));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array10, strlen((char*)simu_msglist_array10));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array11, strlen((char*)simu_msglist_array11));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array12, strlen((char*)simu_msglist_array12));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array13, strlen((char*)simu_msglist_array13));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array14, strlen((char*)simu_msglist_array14));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array15, strlen((char*)simu_msglist_array15));
        MMIAPISMS_InsertMAPDataToList((void*)simu_msglist_array16, strlen((char*)simu_msglist_array16));
        break;
    
    case 1:
        freeallmapdata();
        MMIAPISMS_InsertMessageDataToList(simu_msg_body_array, strlen(simu_msg_buffer_array1));
        break;

    case 2:
        {
#if defined(TRACE_MAP_BUFFER)
    MMIFILE_HANDLE list_handle = SFS_INVALID_HANDLE;
    MMIFILE_HANDLE content_handle = SFS_INVALID_HANDLE;
    uint8* list_data = NULL;
    uint8* p_curr_list_data = NULL;
    uint8* map_data = NULL;
    uint32 bytes_to_read;
    wchar *MAP_Data_List = L"E:\\memo\\MAPLIST_DATA_List.txt";
    uint8* orig_file_name = NULL;
    uint32 map_file_len = 0;
    wchar  map_file[128] = {0};
    MMIFILE_DEVICE_E  dev    = MMI_DEVICE_SDCARD;
    wchar       path[MMIFMM_FILENAME_LEN+1]={0};
    uint16      path_length = MMIFMM_FILENAME_LEN;
    wchar       g_memo_file_suffix[]   = {'.' , 't', 'x', 't', 0 };
    wchar       g_memo_dir[]   = { 'M', 'e', 'm', 'o',  0};
    wchar       file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      file_name_len = MMIFILE_FULL_PATH_MAX_LEN;

    list_handle = MMIAPIFMM_CreateFile(MAP_Data_List, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL); /*lint !e655*/
    if(0 == list_handle)
    {
        return;
    }
    //读取版本信息
    list_data = SCI_ALLOC_APP(2048*5);
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(list_handle, list_data, 2048*5, &bytes_to_read, NULL))
    {
        MMIAPIFMM_CloseFile(list_handle);
        return;
    }

    dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
    MMIAPIFMM_CombineFullPath(
        MMIAPIFMM_GetDevicePath(dev), 
        MMIAPIFMM_GetDevicePathLen(dev),
        g_memo_dir, 
        MMIAPICOM_Wstrlen(g_memo_dir),
        PNULL, 
        NULL,
        path,
        &path_length
        );

    // make file list
    p_curr_list_data = list_data;
    LocateDataFromBuffer(p_curr_list_data, 18, "&", "#", &orig_file_name, &file_name_len);
    p_curr_list_data += (file_name_len + 2);

    map_data = SCI_ALLOC_APP(2048);
    do 
    {

        memset(map_file, 0x00, 128);
        
        MMI_STRNTOWSTR(map_file, 128, (const uint8*)orig_file_name, file_name_len, file_name_len);

        map_file_len = MMIAPICOM_Wstrlen(map_file);

        CombinePath(path, path_length,
            map_file, map_file_len,
            (wchar*)g_memo_file_suffix + 1, MMIAPICOM_Wstrlen(g_memo_file_suffix) -1, 
            file_name, &file_name_len);

        SCI_MEMSET(map_data, 0x00, 2048);
        content_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL); /*lint !e655*/
        
        if(0 != content_handle)
        {
            //读取版本信息
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(content_handle, map_data, 2048, &bytes_to_read, NULL))
            {
                MMIAPISMS_InsertMAPDataToList(map_data, bytes_to_read);
            }
            MMIAPIFMM_CloseFile(content_handle);
        }

        SCI_FREE(orig_file_name);
        orig_file_name = NULL;
        // make file list
        LocateDataFromBuffer(p_curr_list_data, 18, "&", "#", &orig_file_name, &file_name_len);
        p_curr_list_data += (file_name_len + 2);

    } while(NULL != orig_file_name);

    MMIAPIFMM_CloseFile(list_handle);

    SCI_FREE(list_data);
    SCI_FREE(map_data);
#endif
        }
        break;
    default:
        break;
    }

    return TRUE;

}
#endif

/*******************************************************************************/
//  Description :Insert sms record into list
//  Parameter: buff[in]: 
//  Global resource dependence :                                
//  Author: candice chen
//  Return : 
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_InsertSMSRecord(char *tel, MMI_STRING_T *subject, uint32 time)
{
    MMISMS_OPER_ERR_E   ret = MMISMS_BT_ERROR;

    if( PNULL != subject->wstr_ptr)
    {
        switch( MMIAPISMS_GetCurBoxType() )
        {
        case MMISMS_BT_BOX_MT:
            SCI_TRACE_LOW("MMISMS_BT_BOX_MT, tel = %s, datetime %x ", tel, time);
            if( PNULL != tel)
                {
            ret  = MMIAPISMS_SaveLongMtSmsContent(tel, subject, time);
                }
            break;
            
        case MMISMS_BT_BOX_SENDSUCC:
            SCI_TRACE_LOW("MMISMS_BT_BOX_SENDSUCC, tel = %s, datetime %x ", tel, time);
            if( PNULL != tel)
                {
            ret  = MMIAPISMS_SaveLongMoSmsContent(MMISMS_BT_SAVE_SENDSUCC_SMS, tel, subject, time);
                }
            break;
            
        case MMISMS_BT_BOX_SENDFAIL:
            SCI_TRACE_LOW("MMISMS_BT_BOX_SENDFAIL, tel = %s, datetime %x ", tel, time);
            if( PNULL != tel)
                {
            ret  = MMIAPISMS_SaveLongMoSmsContent(MMISMS_BT_SAVE_SENDFAIL_SMS, tel, subject, time);
                }
            break;
            
        case MMISMS_BT_BOX_NOSEND:
            SCI_TRACE_LOW("MMISMS_BT_BOX_NOSEND, tel = %s, datetime %x ", tel, time);
            if ( PNULL != tel )
                {
            ret  = MMIAPISMS_SaveLongMoSmsContent(MMISMS_BT_SAVE_NOSEND_SMS, tel, subject, time);
                }
            else
                {
            ret  = MMIAPISMS_SaveLongMoSmsContent(MMISMS_BT_SAVE_NOSEND_SMS, "", subject, time);
                }
            break;
            
        default:
            break;
        }
    }
    return ret;
}

/*******************************************************************************/
//  Description :ParseSubjectDataFromBuffer
//  Parameter: buff[in]: 
//  Global resource dependence :                                
//  Author: candice chen
//  Return : 
/*****************************************************************************/
LOCAL BOOLEAN ParseSubjectDataFromBuffer(
                                         char           *src_buff_ptr, 
                                         uint16         src_len, 
                                         uint16         max_content_len,
                                         MMI_STRING_T   *p_msg_content
                                         )
{
    uint16 i = 0, src_buf_index = 0;
    uint32 unicode_len;
    uint32 content_len = (uint32)max_content_len;
    uint16 *content_buff_ptr = NULL;
    char   *curr_buf = src_buff_ptr;
    uint8  *section_buf = PNULL;

    
    if(NULL != curr_buf)
    {
        if(PNULL != strstr(curr_buf, "&#"))
        {
            if(content_len == 0)
            {
                content_len = src_len;
            }

            if(content_len > 0)
            {
                content_buff_ptr = SCI_ALLOC_APP((content_len + 1) * 2);
                p_msg_content->wstr_ptr = content_buff_ptr;
                SCI_MEMSET(content_buff_ptr, 0x00, (content_len + 1) * 2);
            }

            if(NULL != content_buff_ptr)
            {
                section_buf = SCI_ALLOC_APP(10);

                for(src_buf_index = 0; src_buf_index < src_len; src_buf_index++, i++)
                {
                    if(i == content_len)
                    {
                        break;
                    }

                    curr_buf = &src_buff_ptr[src_buf_index];
                    if('&' == curr_buf[0])
                    {
                        SCI_MEMSET(section_buf, 0x00, 10);
                        LocateDataFromBuffer(curr_buf, 6, "&", ";", &section_buf, &unicode_len);

                        if('#' == section_buf[0])
                        {
                            src_buf_index+=(1+unicode_len);
                            content_buff_ptr[i] = atoi((char*)&section_buf[1]);
                        }
                        else
                        {
                            if(PNULL != strstr((char*)section_buf, "amp"))
                            {
                                content_buff_ptr[i] = (uint16)'&';
                            }
                            else if(PNULL != strstr((char*)section_buf, "lt"))
                            {
                                content_buff_ptr[i] = (uint16)'<';
                            }
                            else if(PNULL != strstr((char*)section_buf, "gt"))
                            {
                                content_buff_ptr[i] = (uint16)'>';
                            }
                            else if(PNULL != strstr((char*)section_buf, "quota"))
                            {
                                content_buff_ptr[i] = (uint16)'\"';
                            }
                            src_buf_index+=(1+unicode_len);
                        }
                    }
                    else
                    {
                        content_buff_ptr[i] = curr_buf[0];
                    }
                    
                    if(i > (content_len+1))
                    {
                        break;
                    }
                }
                p_msg_content->wstr_len = i;
                SCI_FREE(section_buf);
            }
        }
        else
        {
//            if(content_len == 0)
            {
                content_len = src_len * 2 + 1;
            }

            if(content_len > 0)
            {
                content_buff_ptr = SCI_ALLOC_APP((content_len + 10) * 2);
                p_msg_content->wstr_ptr = content_buff_ptr;
            }

            if(NULL != content_buff_ptr)
            {
                SCI_MEMSET(content_buff_ptr, 0x00, ((content_len + 10) * 2));
                p_msg_content->wstr_len = GUI_UTF8ToWstr(content_buff_ptr, src_len, (const uint8*)src_buff_ptr, src_len);
            }
        }
    
        return TRUE;
    }
    return FALSE;
}

/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_GetMsgFromMapMsgList(const char*  buff) 
{

	char    *p_searched_ptr = PNULL;
    uint32  offset_len = 0;
    char    *search_ptr = (char*)buff;
	uint32	extract_len = 0;
	char    *extract_buff_ptr = PNULL;
    MMISMS_OPER_ERR_E   ret = MMISMS_BT_ERROR;
	char    *search_endTag_ptr = PNULL;
    char    *merge_buff_ptr = PNULL; 
	uint32  merge_buff_len = 0;
    char    *rn_property = "\r\n"; 
    uint16  *content = PNULL;
    uint8   *msg_handle = PNULL;
    uint8   *datetime = PNULL;
    uint8   *subject = PNULL;
    uint8   *tel = PNULL;
    uint8   *is_read = PNULL;
    uint8 *phone_number = PNULL;
    uint8 *name = PNULL;
    uint32 name_len = 0;

    uint32  data_len = 0;
    uint16  content_len = 0;
    uint32  time = 0;
    BOOLEAN is_extra_msg = FALSE;
    BOOLEAN parse_ret = FALSE;

    MMI_STRING_T  p_msg_content = {0};
    MMI_STRING_T  p_msg_name = {0};
    

    if (PNULL != s_sms_buff_ptr)
    {
        if(PNULL == buff)
        {
            merge_buff_len = strlen((char*)s_sms_buff_ptr);
        }
        else
        {
            merge_buff_len = strlen((char*)s_sms_buff_ptr) + strlen((char*)buff);
        }
    }
    else
    {
        if(PNULL == buff)
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList buff = PNULL");
            return MMISMS_SYSTEM_BUSY;
        }
        merge_buff_len = strlen((char*)buff);
    }
    
    //分配拼接后 所需buffer
    merge_buff_ptr = SCI_ALLOC_APP(merge_buff_len + 1);
    
    if (PNULL == merge_buff_ptr)
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList merge_buff_ptr Alloc fail!");
        
        if(PNULL != s_sms_buff_ptr)
        {
            SCI_FREE(s_sms_buff_ptr);
        }
        
        return MMISMS_BT_ERROR;
    }
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList merge_buff_len:%d", merge_buff_len);
    
    SCI_MEMSET(merge_buff_ptr, 0x00, merge_buff_len + 1);
    
    //上批缓存中有内容, 需要先将 上一批缓存中的data 与 下一批buffer中的data 拼接起来
    if (PNULL != s_sms_buff_ptr)
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList s_sms_buff_ptr has data");
        
        strncpy(merge_buff_ptr, s_sms_buff_ptr, strlen((char*)s_sms_buff_ptr));
        
        //释放上一批缓存buffer
        SCI_FREE(s_sms_buff_ptr);	
    }
    
    if(PNULL != buff)
    {
        strncat(merge_buff_ptr, buff, strlen((char*)buff));
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList buff: %s", buff);
    }
    
    MMI_CheckAllocatedMemInfo();
    
    do
    {
        /************************************************************************/
        /* whether phone stroage has free space or not                           */
        /************************************************************************/
        if( !MMIAPISMS_MEHaveEnoughSpace())
        {
            ret = MMISMS_NO_SPACE;
            break;
        }
        
        
        //偏移长度
        p_searched_ptr = &merge_buff_ptr[offset_len];
        
        //判断是否只剩下"\r\n"
        if (0 == strcmp(rn_property, p_searched_ptr))
        {
            ret = MMISMS_NO_ERR;
            break;
        }
        
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList merge_buff_len:%d", merge_buff_len);

        /************************************************************************/
        //buffer 提取规则如下:
        //BEGIN:VCARD与END:VCARD必须配对出现
        //先查找BEGIN:VCARD，再查找END:VCARD 
        //如果BEGIN:VCARD与END:VCARD都存在,说明数据完整，可以直接对其进行解析
        //否则说明数据不完整，需要和下一批buffer的数据一起解析
        /************************************************************************/        
		
        search_ptr = strstr(p_searched_ptr, MSGLISTINGHANDLE_TAGS);
        if(PNULL != search_ptr)
        {
            search_endTag_ptr = strstr(search_ptr+1, MSGLISTINGHANDLE_END_TAGS);
            SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList search_ptr: %s", search_ptr);
            SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList search_next_ptr: %s", search_endTag_ptr);

            if (PNULL != search_endTag_ptr)
			{
				
				//begin and end 两个都找到的情况下
				//此处使用p_searched_ptr来代替search_ptr来计算偏移量
				extract_len = (strlen((char*)p_searched_ptr) - strlen((char*)search_endTag_ptr) +  strlen((char*)MSGLISTINGHANDLE_END_TAGS));	
				
				extract_buff_ptr = SCI_ALLOC_APP(extract_len + 1);
				if (PNULL == extract_buff_ptr)
				{
					SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList extract_buff_ptr alloc fail");
					ret = MMISMS_BT_ERROR;
					break;
				}
				SCI_MEMSET(extract_buff_ptr, 0x00, (extract_len + 1));
				
				strncpy(extract_buff_ptr, p_searched_ptr, extract_len);

                p_searched_ptr = extract_buff_ptr;
				
				if ( 0 == extract_len )
				{
					break;
				}
				
                // ********* Message Handle Section Start *********
				parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, MSGLISTINGHANDLE_TAGS, COMMON_END_TAGS, &msg_handle, &data_len);
                if (PNULL == msg_handle)
                {
                    SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList msg_handle alloc fail");
                    ret = MMISMS_BT_ERROR;
                    break;
                }
                p_searched_ptr+=data_len;
                // ********* Message Handle Section End *********

				
                // ********* Size Section Start *********
                parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, SIZE_TAGS, COMMON_END_TAGS, &subject, &data_len);
                if(  (TRUE == parse_ret) && (PNULL != subject))
                {
                    content_len = (uint16)atoi((char*)subject);
                    SCI_FREE(subject);
                    subject = NULL;
                }
                else
                {
                    SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList size parse fail");
                }

                // ********* Subject Section Start *********
				parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, SUBJECT_TAGS, COMMON_END_TAGS, &subject, &data_len);
                if (PNULL == subject)
                {
                    parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, "bject", COMMON_END_TAGS, &subject, &data_len);
                }

                if(PNULL != subject)
                {
                    // Convert subject string from html/utf-8 encoding to wstring
                    ParseSubjectDataFromBuffer((char*)subject, data_len, content_len, &p_msg_content);
                }
                p_searched_ptr+=data_len;
                // ********* subject Section End *********
                
                // ********* datetime Section Start *********
				parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, DATETIME_TAGS, COMMON_END_TAGS, &datetime, &data_len);
                if( (TRUE == parse_ret) && (PNULL != datetime))
                {
                    uint8 day, hour, minute, month, second, year;
                    day         = (uint8)MMISMS_StringToInt(&datetime[6],  2);
                    hour        = (uint8)MMISMS_StringToInt(&datetime[9],  2);
                    minute      = (uint8)MMISMS_StringToInt(&datetime[11], 2);
                    month       = (uint8)MMISMS_StringToInt(&datetime[4],  2);
                    second      = (uint8)MMISMS_StringToInt(&datetime[13], 2);
                    year        = (uint8)(MMISMS_StringToInt(datetime, 4) - MMISMS_MT_OFFSERT_YEAR);
                    time = MMIAPICOM_Tm2Second(
                        second,
                        minute,
                        hour,
                        day,
                        month,
                        year + MMISMS_MT_OFFSERT_YEAR);
                }
                else
                {
                    SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList datetime parse fail");
                }
                // ********* datetime Section End *********
                
                
                // ********* tel Section Start *********
                if( MMISMS_BT_BOX_MT == MMIAPISMS_GetCurBoxType() )
				{

                parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, SENDER_ADDRESS_TAGS, COMMON_END_TAGS, &tel, &data_len);
                    if( FALSE == parse_ret)
                {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList tel parse fail");
                        ret = MMISMS_BT_ERROR;
                    break;
                }
                if (0 == data_len)
                {
						if( PNULL != tel)
						{
                    SCI_FREE(tel);
						}
                    parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, RECIPIENT_ADDRESS_TAGS, COMMON_END_TAGS, &tel, &data_len);
						if( FALSE == parse_ret)
						{
                            SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList tel parse fail");
                                                ret = MMISMS_BT_ERROR;
							break;
						}
					}				
                }
                else
                {
                    parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, RECIPIENT_ADDRESS_TAGS, COMMON_END_TAGS, &tel, &data_len);
                    if( (FALSE == parse_ret)  &&  (MMISMS_BT_BOX_NOSEND != MMIAPISMS_GetCurBoxType()) )
                    {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList tel parse fail");
                        break;
                    }
                }

                if( (PNULL != tel ) && (MMISMS_BT_BOX_NOSEND != MMIAPISMS_GetCurBoxType()))
                    {
                                    parse_ret = CheckPhoneNumberFromBuffer((char*)tel, data_len, (uint32**)(&phone_number));
                    if( FALSE == parse_ret)
                    {
                        SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList CheckPhoneNumberFromBuffer fail");
                        break;
                    }
                    }
                // ********* tel Section End *********

                // ********* name Section Start *********
					if( MMISMS_BT_BOX_MT == MMIAPISMS_GetCurBoxType() )
					{
						parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, SENDER_NAME_TAGS, COMMON_END_TAGS, &name, &name_len);
						if( FALSE == parse_ret)
						{
							SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList name parse fail");
							ret = MMISMS_BT_ERROR;
							break;
						}
                    }
					else
                    {
						parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, RECIPIENT_NAME_TAGS, COMMON_END_TAGS, &name, &name_len);
						if( FALSE == parse_ret)
						{
							SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList name parse fail");
							ret = MMISMS_BT_ERROR;
							break;
						}
                    }

                if(PNULL != name)
                {
                    // Convert name string from html/utf-8 encoding to wstring
                    ParseSubjectDataFromBuffer((char*)name, name_len, name_len, &p_msg_name);
                }
                // ********* name Section End *********

                // ********* status Section Start *********
                if(MMISMS_BT_BOX_MT == MMIAPISMS_GetCurBoxType() )
                {
                    parse_ret = ParseDataFromBuffer(p_searched_ptr, extract_len, READ_TAGS, COMMON_END_TAGS, &is_read, &data_len);
                    if(0 == strncmp((char*)is_read, "yes", data_len ))  //read
                    {
                        g_is_cur_sms_read = TRUE;
                    }
                    else
                    {
                        g_is_cur_sms_read = FALSE;   
                    }
                }
                // ********* status Section End *********

                if( content_len >= 255 )
                {
                    is_extra_msg = TRUE;
                }
                else
                {
                    if((content_len > p_msg_content.wstr_len))
                    {
                        is_extra_msg = TRUE;
                }
                else
                {
                        is_extra_msg = FALSE;
                    }
                }
                
        SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList, msg_handle = %s, content_len=%d, is_extra_msg = %d", msg_handle, content_len, is_extra_msg);

                InsertMsgHandleTableList(s_message_table_head, (char*)msg_handle, is_extra_msg, p_msg_name.wstr_ptr, p_msg_name.wstr_len);
                if(PNULL == subject)
                {
                    ret = MMISMS_SYSTEM_BUSY;
                }
                else
                {
                    ret = MMISMS_InsertSMSRecord((char*)phone_number, &p_msg_content, time);
					SCI_TRACE_LOW("MMISMS_InsertSMSRecord ret = %d", ret);
                }

                s_sync_sms_count++;

                SCI_FREE(datetime);
                SCI_FREE(subject);
                SCI_FREE(content);
                SCI_FREE(tel);
                SCI_FREE(msg_handle);
                SCI_FREE(p_msg_content.wstr_ptr);
                
                offset_len += extract_len;
                p_searched_ptr = &merge_buff_ptr[offset_len];
				extract_len = strlen((char*)p_searched_ptr);

					if( MMISMS_NO_SPACE == ret)
					{
						SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList MMISMS_NO_SPACE");
						MMISMS_CloseMapWaitWin();
					}
					else
					{
						ret = MMISMS_NO_ERR;
					}
			}
            else
            {
                SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList PNULL == search_endTag_ptr");
            }
		}
        else
        {
            SCI_TRACE_LOW("MMISMS_GetMsgFromMapMsgList PNULL == search_ptr");
        }

        {
            extract_len = strlen((char*)p_searched_ptr);
            
            if (PNULL != s_sms_buff_ptr)
            {
                SCI_FREE(s_sms_buff_ptr);
            }
            
            if((PNULL == search_ptr) && (PNULL == search_endTag_ptr))
            {
                if (PNULL != strstr(p_searched_ptr, MSGLISTING_TAGS))
                {
                    ret = MMISMS_NO_ERR;
                    MMISMS_CloseMapWaitWin();
                    break;
                }
            }

            s_sms_buff_ptr = SCI_ALLOC_APP(extract_len + 1);
            
            if (PNULL == s_sms_buff_ptr)
            {
                ret = MMISMS_BT_ERROR;
                break;
            }
            
            SCI_MEMSET(s_sms_buff_ptr, 0x00, extract_len + 1);
            
            strncpy(s_sms_buff_ptr, p_searched_ptr, extract_len);
            
            if((PNULL == search_ptr) || (PNULL == search_endTag_ptr))
            {
                ret = MMISMS_SYSTEM_BUSY;
            }
            break;
		}
		
    } while(offset_len < strlen((char*)merge_buff_ptr));
    
    MMI_CheckAllocatedMemInfo();
    
    
    if (PNULL != merge_buff_ptr) /*lint !e774*/
    {
        SCI_FREE(merge_buff_ptr);
    }
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_GetMsgFromMapMsgList s_sync_sms_count = %d,ret = %d", s_sync_sms_count,ret); 

    return ret;  		
}

/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E SetFolder(const char*  buff) 
{

	char*   p_searched_ptr = PNULL;
    uint32  offset_len = 0;
    char*   search_ptr = (char*)buff;
	uint32	extract_len = 0;
	char*	extract_buff_ptr = PNULL;
    MMISMS_OPER_ERR_E   ret = MMISMS_BT_ERROR;
	char*	search_endTag_ptr = PNULL;
    char*	merge_buff_ptr = PNULL; 
	uint32  merge_buff_len = 0;
    char*   rn_property = "\r\n"; 

    if(PNULL == buff)
    {
        SCI_TRACE_LOW("[MMISMS] SetFolder param error");
        return MMISMS_BT_ERROR;
    }
    
    if (PNULL != s_sms_buff_ptr)
    {
        merge_buff_len = strlen((char*)s_sms_buff_ptr) + strlen((char*)buff);
    }
    else
    {
        merge_buff_len = strlen((char*)buff);
    }

    //分配拼接后 所需buffer
	merge_buff_ptr = SCI_ALLOC_APP(merge_buff_len + 1);

	if (PNULL == merge_buff_ptr)
	{
		SCI_TRACE_LOW("[MMISMS] SetFolder merge_buff_ptr Alloc fail!");
        
        if(PNULL != s_sms_buff_ptr)
        {
	        SCI_FREE(s_sms_buff_ptr);
        }

		return MMISMS_BT_ERROR;
	}

	SCI_TRACE_LOW("[MMISMS] SetFolder merge_buff_len:%d", merge_buff_len);

	SCI_MEMSET(merge_buff_ptr, 0x00, merge_buff_len + 1);

	//上批缓存中有内容, 需要先将 上一批缓存中的data 与 下一批buffer中的data 拼接起来
    if (PNULL != s_sms_buff_ptr)
	{
		SCI_TRACE_LOW("[MMISMS] SetFolder s_vcard_buff_ptr has data");

	    strncpy(merge_buff_ptr, s_sms_buff_ptr, strlen((char*)s_sms_buff_ptr));

        //释放上一批缓存buffer
	    SCI_FREE(s_sms_buff_ptr);	
    }

    
	strncat(merge_buff_ptr, buff, strlen((char*)buff));

    MMI_CheckAllocatedMemInfo();


    do
    {
		/************************************************************************/
		/* whether phone stroage has free space or not                           */
		/************************************************************************/
        if( !MMIAPISMS_MEHaveEnoughSpace())
        {
            ret = MMISMS_NO_SPACE;
            break;
        }


		//偏移长度
		p_searched_ptr = &merge_buff_ptr[offset_len];
      
		//判断是否只剩下"\r\n"
		if (0 == strcmp(rn_property, p_searched_ptr))
		{
            ret = MMISMS_BT_ERROR;
            break;
		}
        
        SCI_TRACE_LOW("[MMISMS] SetFolder merge_buff_len:%d", merge_buff_len);
        
        SCI_TRACE_LOW("[MMISMS] SetFolder merge_buff_ptr:%s", merge_buff_ptr);

        
        /************************************************************************/
        //buffer 提取规则如下:
        //BEGIN:VCARD与END:VCARD必须配对出现
        //先查找BEGIN:VCARD，再查找END:VCARD 
        //如果BEGIN:VCARD与END:VCARD都存在,说明数据完整，可以直接对其进行解析
        //否则说明数据不完整，需要和下一批buffer的数据一起解析
        /************************************************************************/        
        {
            //找BEGIN:VCARD 
			search_ptr = strstr(p_searched_ptr, FOLDERLISTING_TAGS);
            SCI_TRACE_LOW("SetFolder start find FOLDERLISTING_TAGS = %ld",search_ptr);
   
			if (PNULL != search_ptr)
			{        
                //找END:VCARD 
                search_endTag_ptr = strstr(search_ptr, FOLDERLISTING_END_TAGS);
				SCI_TRACE_LOW("SetFolder start find FOLDERLISTING_END_TAGS = %ld",search_endTag_ptr);

                if (PNULL != search_endTag_ptr)
        		{
        		    //begin and end 两个都找到的情况下
					//此处使用p_searched_ptr来代替search_ptr来计算偏移量
        			extract_len = (strlen((char*)p_searched_ptr) - strlen((char*)search_endTag_ptr) + strlen((char*)FOLDERLISTING_END_TAGS));	

                    extract_buff_ptr = SCI_ALLOC_APP(extract_len + 1);

                    if (PNULL == extract_buff_ptr)
                    {
                        SCI_TRACE_LOW("SetFolder extract_buff_ptr alloc fail");

                        ret = MMISMS_BT_ERROR;
                        break;
                    }

                    SCI_MEMSET(extract_buff_ptr, 0x00, (extract_len + 1));

                    strncpy(extract_buff_ptr, p_searched_ptr, extract_len);

                    SCI_FREE(extract_buff_ptr);
                    
                    if(MMISMS_BT_ERROR != ret)
                    {
                        ret = MMISMS_BT_ERROR;
                        break;
                    }
        		}

			}
           
            //数据不完整，需要先缓存起来 ps:处理每条buffer最后不完整的记录
            extract_len = strlen((char*)p_searched_ptr);

			if (PNULL != s_sms_buff_ptr)
			{
				SCI_FREE(s_sms_buff_ptr);
			}

			s_sms_buff_ptr = SCI_ALLOC_APP(extract_len + 1);

			if (PNULL == s_sms_buff_ptr)
			{
                ret = MMISMS_BT_ERROR;
                break;
			}

			SCI_MEMSET(s_sms_buff_ptr, 0x00, extract_len + 1);

			strncpy(s_sms_buff_ptr, p_searched_ptr, extract_len);

            ret = MMISMS_BT_ERROR;
            break;
            //缓存数据结束
        }

        
    }    
    while(offset_len < strlen((char*)merge_buff_ptr));

    MMI_CheckAllocatedMemInfo();


	if (PNULL != merge_buff_ptr) /*lint !e774*/
	{
		SCI_FREE(merge_buff_ptr);
	}
    
    SCI_TRACE_LOW("[MMISMS] SetFolder s_sync_contact_count = %d,ret = %d", s_sync_sms_count,ret); 

    return ret;  		
}


/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AddDataToSMS(const char*  buff     //[in]                                      
                                        ) 
{

	char*   p_searched_ptr = PNULL;
    uint32  offset_len = 0;
    char*   search_ptr = (char*)buff;
	uint32	extract_len = 0;
	char*	extract_buff_ptr = PNULL;
    MMISMS_OPER_ERR_E   ret = MMISMS_BT_ERROR;
	char*	search_endTag_ptr = PNULL;
    char*	merge_buff_ptr = PNULL; 
	uint32  merge_buff_len = 0;
    char*   rn_property = "\r\n"; 

    if(PNULL == buff)
    {
        SCI_TRACE_LOW("[MMISMS] MMISMS_AddDataToSMS param error");
        return MMISMS_BT_ERROR;
    }
    
    if (PNULL != s_sms_buff_ptr)
    {
        merge_buff_len = strlen((char*)s_sms_buff_ptr) + strlen((char*)buff);
    }
    else
    {
        merge_buff_len = strlen((char*)buff);
    }

    //分配拼接后 所需buffer
	merge_buff_ptr = SCI_ALLOC_APP(merge_buff_len + 1);

	if (PNULL == merge_buff_ptr)
	{
		SCI_TRACE_LOW("[MMISMS] MMISMS_AddDataToSMS merge_buff_ptr Alloc fail!");
        
        if(PNULL != s_sms_buff_ptr)
        {
	        SCI_FREE(s_sms_buff_ptr);
        }

		return MMISMS_BT_ERROR;
	}

	SCI_TRACE_LOW("[MMISMS] MMISMS_AddDataToSMS merge_buff_len:%d", merge_buff_len);

	SCI_MEMSET(merge_buff_ptr, 0x00, merge_buff_len + 1);

	//上批缓存中有内容, 需要先将 上一批缓存中的data 与 下一批buffer中的data 拼接起来
    if (PNULL != s_sms_buff_ptr)
	{
		SCI_TRACE_LOW("[MMISMS] MMISMS_AddDataToSMS s_vcard_buff_ptr has data");

	    strncpy(merge_buff_ptr, s_sms_buff_ptr, strlen((char*)s_sms_buff_ptr));

        //释放上一批缓存buffer
	    SCI_FREE(s_sms_buff_ptr);	
    }

    
	strncat(merge_buff_ptr, buff, strlen((char*)buff));

    MMI_CheckAllocatedMemInfo();


    do
    {
		/************************************************************************/
		/* whether phone stroage has free space or not                           */
		/************************************************************************/
        if( !MMIAPISMS_MEHaveEnoughSpace())
        {
            ret = MMISMS_NO_SPACE;
            break;
        }


		//偏移长度
		p_searched_ptr = &merge_buff_ptr[offset_len];
      
		//判断是否只剩下"\r\n"
		if (0 == strcmp(rn_property, p_searched_ptr))
		{
            ret = MMISMS_BT_ERROR;
            break;
        }
        
        SCI_TRACE_LOW("[MMISMS] MMISMS_AddDataToSMS merge_buff_len:%d", merge_buff_len);
        
        SCI_TRACE_LOW("[MMISMS] MMISMS_AddDataToSMS merge_buff_ptr:%s", merge_buff_ptr);

        
        /************************************************************************/
        //buffer 提取规则如下:
        //BEGIN:VCARD与END:VCARD必须配对出现
        //先查找BEGIN:VCARD，再查找END:VCARD 
        //如果BEGIN:VCARD与END:VCARD都存在,说明数据完整，可以直接对其进行解析
        //否则说明数据不完整，需要和下一批buffer的数据一起解析
        /************************************************************************/        
        {
            //找BEGIN:VCARD 
			search_ptr = strstr(p_searched_ptr, FOLDERLISTING_TAGS);
            SCI_TRACE_LOW("MMISMS_AddDataToSMS start find FOLDERLISTING_TAGS = %ld",search_ptr);
   
			if (PNULL != search_ptr)
			{        
                //找END:VCARD 
                search_endTag_ptr = strstr(search_ptr, FOLDERLISTING_END_TAGS);
				SCI_TRACE_LOW("MMISMS_AddDataToSMS start find FOLDERLISTING_END_TAGS = %ld",search_endTag_ptr);

                if (PNULL != search_endTag_ptr)
        		{
        		    //begin and end 两个都找到的情况下
					//此处使用p_searched_ptr来代替search_ptr来计算偏移量
        			extract_len = (strlen((char*)p_searched_ptr) - strlen((char*)search_endTag_ptr) + strlen((char*)FOLDERLISTING_END_TAGS));	

                    extract_buff_ptr = SCI_ALLOC_APP(extract_len + 1);

                    if (PNULL == extract_buff_ptr)
                    {
                        SCI_TRACE_LOW("MMISMS_AddDataToSMS extract_buff_ptr alloc fail");

                        ret = MMISMS_BT_ERROR;
                        break;
                    }

                    SCI_MEMSET(extract_buff_ptr, 0x00, (extract_len + 1));

                    strncpy(extract_buff_ptr, p_searched_ptr, extract_len);

                    SCI_FREE(extract_buff_ptr);
                    
                    if(MMISMS_BT_ERROR != ret)
                    {
                        ret = MMISMS_BT_ERROR;
                        break;
                    }
        		}

			}
           
            //数据不完整，需要先缓存起来 ps:处理每条buffer最后不完整的记录
            extract_len = strlen((char*)p_searched_ptr);

			if (PNULL != s_sms_buff_ptr)
			{
				SCI_FREE(s_sms_buff_ptr);
			}

			s_sms_buff_ptr = SCI_ALLOC_APP(extract_len + 1);

			if (PNULL == s_sms_buff_ptr)
			{
                ret = MMISMS_BT_ERROR;
                break;
			}

			SCI_MEMSET(s_sms_buff_ptr, 0x00, extract_len + 1);

			strncpy(s_sms_buff_ptr, p_searched_ptr, extract_len);

            ret = MMISMS_BT_ERROR;
            break;
            //缓存数据结束
        }

        
    }    
    while(offset_len < strlen((char*)merge_buff_ptr));

    MMI_CheckAllocatedMemInfo();


	if (PNULL != merge_buff_ptr) /*lint !e774*/
	{
		SCI_FREE(merge_buff_ptr);
	}
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_AddDataToSMS s_sync_contact_count = %d,ret = %d", s_sync_sms_count,ret); 

    return ret;  		
}

/*****************************************************************************/
//  Description : parse data from buffer
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN ParseDataFromBuffer(
                                  char      *searchbuffer,  //IN:
                                  uint32    bufferlen,      //IN:
                                  char      *begin,         //IN:
                                  char      *end,           //IN:
                                  uint8     **ppdata,         //OUT:
                                  uint32    *data_len       //OUT:
                                  )
{
    char* tempBuffer = PNULL;
    char* tempEndBuffer = PNULL;

    /* Reset data len */
    *data_len = 0;
    
    // search begin
    if (PNULL != searchbuffer)
    {
        tempBuffer = strstr(searchbuffer, begin);
    }
    else
    {
        return FALSE;
    }
    
    if (PNULL != tempBuffer)
    {
        while((tempBuffer[0] != '\"') && (tempBuffer[0] != '\''))
        {
            tempBuffer++;
        }

        if(strstr(end, COMMON_END_TAGS))
        {
            if(tempBuffer[0] == '\'')
            {
                end = COMMON_END_TAGS2;
            }
        }

        tempBuffer++;
        
        // search end
        tempEndBuffer = strstr(tempBuffer, end);
        
        if (PNULL != tempEndBuffer)
        {
            *data_len = strlen((char*)tempBuffer) - strlen((char*)tempEndBuffer);
            if( *data_len <= bufferlen)
            {
                if(PNULL == *ppdata)
                {
                    *ppdata = SCI_ALLOC_APP(*data_len + 1);
                }
                if (PNULL == *ppdata)
                {
                    SCI_TRACE_LOW("ParseDataFromBuffer alloc fail");
                    return FALSE;
                }
                SCI_MEMSET(*ppdata, 0x00, (*data_len + 1));
                
                strncpy((char*)*ppdata, tempBuffer, *data_len);
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : parse data from buffer
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN LocateDataFromBuffer(
                                  char      *searchbuffer,  //IN:
                                  uint32    bufferlen,      //IN:
                                  char      *begin,         //IN:
                                  char      *end,           //IN:
                                  uint8     **ppdata,         //OUT:
                                  uint32    *data_len       //OUT:
                                  )
{
    char* tempBuffer = PNULL;
    char* tempEndBuffer = PNULL;

    /* Reset data len */
    *data_len = 0;
    
    // search begin
    if (PNULL != searchbuffer)
    {
        tempBuffer = strstr(searchbuffer, begin);
    }
    else
    {
        return FALSE;
    }
    
    if (PNULL != tempBuffer)
    {
        // search end
        tempEndBuffer = strstr(tempBuffer+1, end);
        
        if (PNULL != tempEndBuffer)
        {
            *data_len = strlen((char*)tempBuffer) - strlen((char*)tempEndBuffer) - strlen((char*)begin);
			if( *data_len <= bufferlen)
			{
                if(PNULL == *ppdata)
                {
                    *ppdata = SCI_ALLOC_APP(*data_len + 1);
                }
                if (PNULL == *ppdata)
                {
                    SCI_TRACE_LOW("ParseDataFromBuffer alloc fail");
                    return FALSE;
                }
                SCI_MEMSET(*ppdata, 0x00, (*data_len + 1));

                strncpy((char*)*ppdata, tempBuffer+strlen((char*)begin), *data_len);
			}
			else
			{
				return FALSE;
			}
        }
        else
        {
            return FALSE;
        }
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description : check phone number
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN CheckPhoneNumberFromBuffer(
                                  char      *number,  //IN:
                                  uint32    bufferlen,      //IN:
                                  uint32    **ppdata       //OUT:
                                  )
{
    char* tempBuffer = PNULL;
    char* tempEndBuffer = PNULL;
    uint32 i = 0;
    uint32 length = 0;

    // search begin
    if (PNULL != number)
    {
                if(PNULL == *ppdata)
                {
                    *ppdata = SCI_ALLOC_APP(bufferlen + 1);
                }
                if (PNULL == *ppdata)
                {
                    SCI_TRACE_LOW("CheckPhoneNumberFromBuffer alloc fail");
                    return FALSE;
                }
                SCI_MEMSET(*ppdata, 0x00, (bufferlen + 1));

        for( i = 0; i < bufferlen; i ++)
            {
                if( 0 != strncmp(&number[i], " ", 1))
                    {
                        strncpy((char*)(*ppdata)+length, &number[i], 1);
                        length ++;
                    }
                    
            }
                    SCI_TRACE_LOW("CheckPhoneNumberFromBuffer *ppdata = %s, number = %d", *ppdata, number);
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : save mt long sms content
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongMtSmsContent(
                                                        char            *number,
                                                        MMI_STRING_T    *subject,
                                                        uint32          time
                                                        )
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 total_num = 1;
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;
    uint32  number_len = strlen(number);

    //set message content to global
    SCI_MEMSET(&g_mmisms_global.edit_content.tpdu_head, 0, sizeof(MN_SMS_USER_DATA_HEAD_T));

    g_mmisms_global.edit_content.send_message.alphabet = MN_SMS_UCS2_ALPHABET; 
    
    SCI_MEMCPY(g_mmisms_global.edit_content.send_message.data, subject->wstr_ptr, subject->wstr_len*2);

    g_mmisms_global.edit_content.send_message.length = subject->wstr_len*2;
    
    SCI_MEMCPY(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0], number, number_len);


    SCI_MEMSET(&g_mmisms_global.save_info, 0, sizeof(MMISMS_SAVE_INFO_T));
    g_mmisms_global.save_info.dual_sys = dual_sys;
    g_mmisms_global.save_info.time = time;

    total_num = MMISMS_SplitLongMsg(&g_mmisms_global.edit_content.send_message, FALSE);

    MMISMS_InitUserDataHead(total_num, 1, &g_mmisms_global.edit_content.tpdu_head, PNULL);

    save_result = MMIAPISMS_SaveLongSmsMtNextContent(dual_sys, g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0], total_num, 1, number, number_len);

    return save_result;
}

/*****************************************************************************/
//  Description : save mt long sms content
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongSmsMtNextContent(
                                                           MN_DUAL_SYS_E dual_sys,
                                                           uint8    *dest_addr_ptr,    //IN:
                                                           uint8    total_num,         //IN
                                                           uint8    seq_num,           //IN: the seq number ( 1 ~ 8)
                                                           char     *number,
                                                           uint32   number_len
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
        
        ret_val = MMIAPISMS_SaveMtMsgReq( 
            dual_sys,
            &user_data, 
            storage, 
            edit_content_ptr->send_message.alphabet,
            number,
            number_len
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
//  Description : save mt sms content
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMIAPISMS_SaveMtMsgReq(
                                               MN_DUAL_SYS_E            dual_sys,
                                               APP_SMS_USER_DATA_T      *user_data_ptr,     //IN:
                                               MN_SMS_STORAGE_E         storage,            //IN:
                                               MN_SMS_ALPHABET_TYPE_E   alphabet_type,      //IN:
                                               char                     *number,
                                               uint32                   number_len
                                               )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MT_SMS_T mt_sms = {0};
    MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
    
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
        SCI_MEMSET(&mt_sms, 0, sizeof(MN_SMS_MT_SMS_T));
        
        MMISMS_GetMnCalledNum((uint8*)number, number_len, &party_number, (&mt_sms.origin_addr_t)); 
            
        g_mmisms_global.save_info.number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(party_number.number_type, MN_NUM_PLAN_UNKNOW);
        g_mmisms_global.save_info.number.number_len = party_number.num_len;
        SCI_MEMCPY(g_mmisms_global.save_info.number.number, party_number.bcd_num, party_number.num_len);
        
        
        g_mmisms_global.save_info.folder_type = g_mmisms_global.folder_type;

        mt_sms.dcs.alphabet_type = alphabet_type;
        
        MMISMS_EncodeMtSmsData( 
            dual_sys,
            &mt_sms,
            alphabet_type,
            user_data_ptr
            );
      
        MMISMS_SetOperStatus(MMISMS_BT_SAVE_MT_SMS);

        if (g_is_cur_sms_read)
        {
            err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, MN_SMS_MT_READED, &mt_sms);
        }
        else
        {
            err_code = MNSMS_WriteMtSmsEx(dual_sys, storage, MN_SMS_MT_TO_BE_READ, &mt_sms);
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
//  Description : save mo long sms content
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongMoSmsContent(
                                                        MMISMS_OPER_STATUS_E    oper_status,
                                                        char                    *number,
                                                        MMI_STRING_T            *subject,
                                                        uint32                  time
                                                        )
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 total_num = 1;
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_ERR;
    uint32  number_len = strlen(number);
    
    //set message content to global
    SCI_MEMSET(&g_mmisms_global.edit_content.tpdu_head, 0, sizeof(MN_SMS_USER_DATA_HEAD_T));
    
    g_mmisms_global.edit_content.send_message.alphabet = MN_SMS_UCS2_ALPHABET; 
    
    SCI_MEMCPY(g_mmisms_global.edit_content.send_message.data, subject->wstr_ptr, subject->wstr_len*2);
    
    g_mmisms_global.edit_content.send_message.length = subject->wstr_len*2;
    
    g_mmisms_global.edit_content.dest_info.dest_list.dest_addr_id = 0;
    
    SCI_MEMCPY(g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0], number, number_len);
    
    SCI_MEMSET(&g_mmisms_global.save_info, 0, sizeof(MMISMS_SAVE_INFO_T));
    g_mmisms_global.save_info.dual_sys = dual_sys;
    g_mmisms_global.save_info.time = time;

    total_num = MMISMS_SplitLongMsg(&g_mmisms_global.edit_content.send_message, FALSE);
    
    MMISMS_InitUserDataHead(total_num, 1, &g_mmisms_global.edit_content.tpdu_head, PNULL);
    
    save_result = MMIAPISMS_SaveLongMoSmsNextContent(oper_status, dual_sys, g_mmisms_global.edit_content.dest_info.dest_list.dest_addr[0], total_num, 1, number, number_len);
    
    return save_result;
}

/*****************************************************************************/
//  Description : save mo long sms content
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongMoSmsNextContent(
                                                            MMISMS_OPER_STATUS_E     oper_status,
                                                            MN_DUAL_SYS_E            dual_sys,
                                                            uint8                    *dest_addr_ptr,    //IN:
                                                            uint8                    total_num,        //IN
                                                            uint8                    seq_num,           //IN: the seq number ( 1 ~ 8)
                                                            char                     *number,
                                                            uint32                   number_len
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
        
        ret_val = MMIAPISMS_SaveMoMsgReq(
            oper_status,
            dual_sys,
            dest_addr_ptr, 
            addr_len, 
            &user_data, 
            storage, 
            edit_content_ptr->send_message.alphabet,
            number,
            number_len
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
//  Description : save mo sms content
//  Global resource dependence : none
//  Author: bingjie.chen
//  Note: 
/***************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMIAPISMS_SaveMoMsgReq(
                                               MMISMS_OPER_STATUS_E     oper_status,
                                               MN_DUAL_SYS_E            dual_sys,
                                               uint8                    *dest_addr_ptr,        //IN:
                                               uint8                    dest_len,              //IN:
                                               APP_SMS_USER_DATA_T      *user_data_ptr,        //IN:
                                               MN_SMS_STORAGE_E         storage,               //IN:
                                               MN_SMS_ALPHABET_TYPE_E   alphabet_type,         //IN:
                                               char                     *number,
                                               uint32                   number_len
                                               )
{
    MMISMS_OPER_ERR_E save_result = MMISMS_NO_SC_ADDRESS;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    MN_SMS_MO_SMS_T mo_sms = {0};    
    MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
    
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
        
        if ((number_len > 0) && (PNULL != number))
        {
            MMISMS_GetMnCalledNum(dest_addr_ptr, dest_len, &party_number, (&mo_sms.dest_addr_t));
            
            g_mmisms_global.save_info.number.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(party_number.number_type, MN_NUM_PLAN_UNKNOW);
            g_mmisms_global.save_info.number.number_len = party_number.num_len;
            SCI_MEMCPY(g_mmisms_global.save_info.number.number, party_number.bcd_num, party_number.num_len);
        }
        
        g_mmisms_global.save_info.folder_type = MMISMS_FOLDER_BT;
        
        mo_sms.dcs.alphabet_type = alphabet_type;
        mo_sms.dcs.class_is_present = FALSE;
        mo_sms.time_format_e = MN_SMS_TP_VPF_RELATIVE_FORMAT;
        
        MMISMS_EncodeMoSmsData( 
            dual_sys,
            &mo_sms,
            alphabet_type,
            user_data_ptr
            );
        
        MMISMS_SetOperStatus(oper_status);
        
        if( MMISMS_BT_SAVE_NOSEND_SMS == oper_status || MMISMS_BT_SAVE_SENDFAIL_SMS == oper_status )
        {
            err_code = MNSMS_WriteMoSmsEx(dual_sys, storage, MN_SMS_MO_TO_BE_SEND, &mo_sms);
        }
        else if (MMISMS_BT_SAVE_SENDSUCC_SMS == oper_status )
        {
            err_code = MNSMS_WriteMoSmsEx(dual_sys, storage, MN_SMS_MO_SR_NOT_REQUEST, &mo_sms);
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
//  Description : MMIAPISMS_ComposeMAPPushBody
//  Global resource dependence : none
//  Author: Naomi Huang 
//  Note: 
/***************************************************************************/
PUBLIC char* MMIAPISMS_ComposeMAPPushBody(
                                           uint8 *dest_number, uint16 dest_number_len,
                                           uint8 *msg_body,    uint16 msg_body_len,
                                           MN_SMS_ALPHABET_TYPE_E alphabet
                                           )
{
    char temp[20] = {0};
    char* push_msg = PNULL;
    char* msg_body_in_utf8 = PNULL;
    uint16 current_body_len = 0;
    uint16 max_len = BMSG_HEADER_LEN + msg_body_len + dest_number_len;
    
    // Need to check if the message len definition
    push_msg = SCI_ALLOC_APP(max_len*2);//bug570483 memory overwrite
    msg_body_in_utf8 = SCI_ALLOC_APP(max_len);
    SCI_TRACE_LOW("BT_MapPushMessage max_len = %d", max_len);

    if(PNULL != push_msg)
    {
        SCI_MEMSET(push_msg, 0, max_len*2);
    }
    else
    {
        return 0;
    }

    if(PNULL != msg_body_in_utf8)
    {
        SCI_MEMSET(msg_body_in_utf8, 0, max_len);
        if(MN_SMS_UCS2_ALPHABET == alphabet)
        {
            GUI_WstrToUTF8((uint8*)msg_body_in_utf8, max_len, (uint16*)msg_body, msg_body_len);
        }
        else
        {
            strcpy(msg_body_in_utf8, (char*)msg_body);
        }
        current_body_len = strlen(msg_body_in_utf8);
    }
    else
    {
        return 0;
    }

    sprintf(push_msg, "BEGIN:BMSG\r\n");
    strcat(push_msg, "VERSION:1.0\r\n");
    strcat(push_msg, "STATUS:UNREAD\r\n");
    strcat(push_msg, "TYPE:SMS_GSM\r\n");
    strcat(push_msg, "FOLDER:telecom/msg/outbox\r\n");
    strcat(push_msg, "BEGIN:VCARD\r\n");
    strcat(push_msg, "VERSION:2.1\r\n");
    strcat(push_msg, "N:\r\n");
    strcat(push_msg, "TEL:\r\n");
    strcat(push_msg, "END:VCARD\r\n");
    strcat(push_msg, "BEGIN:BENV\r\n");
    strcat(push_msg, "BEGIN:VCARD\r\n");
    strcat(push_msg, "VERSION:2.1\r\n");
    strcat(push_msg, "N:\r\n");
    strcat(push_msg, "TEL:");
    strcat(push_msg, (char*)dest_number);
    strcat(push_msg, "\r\n");

    SCI_TRACE_LOW("BT_MapPushMessage dest_number = %s", dest_number);

    strcat(push_msg, "END:VCARD\r\n");
    strcat(push_msg, "BEGIN:BENV\r\n");
    strcat(push_msg, "BEGIN:BBODY\r\n");
    strcat(push_msg, "ENCODING:8BIT\r\n");  //ENCODING:G-UCS2
    strcat(push_msg, "CHARSET:UTF-8\r\n");  //CHARSET:UTF-8
    strcat(push_msg, "LANGUAGE:ENGLISH\r\n"); //LANGUAGE:ENGLISH,CHINESE
    sprintf(temp, "LENGTH:%d\r\n", 22+msg_body_len);  //22+message content,include BEGIN:MSG,END:MSG and \r\n,these three field is 22bytes

    strcat(push_msg, temp); 
    strcat(push_msg, "BEGIN:MSG\r\n");

    SCI_TRACE_LOW("BT_MapPushMessage msg_body_in_utf8: = %s", msg_body_in_utf8);

    strcat(push_msg, (char*)msg_body_in_utf8);     
    strcat(push_msg, "\r\n");
    strcat(push_msg, "END:MSG\r\n");
    strcat(push_msg, "END:BBODY\r\n");
    strcat(push_msg, "END:BENV\r\n");
    strcat(push_msg, "END:BENV\r\n");
    strcat(push_msg, "END:BMSG\r\n");
    
    SCI_TRACE_LOW("BT_MapPushMessage data = %x", push_msg);
    SCI_TRACE_LOW("BT_MapPushMessage length = %d", strlen(push_msg));
    SCI_TRACE_LOW("MAP:MMIAPISMS_ComposeMAPPushBody push_message_ptr=%s.",push_msg);

    if (PNULL != msg_body_in_utf8)//bug570483 memory leak
    {
        SCI_FREE(msg_body_in_utf8);
        msg_body_in_utf8 = PNULL;
    }

    //return strlen(push_message_ptr);
    return push_msg;
}

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_FreeAllMapData(void)
{
    freeallmapdata();
}

/*****************************************************************************/
//  Description : MMIAPISMS_HandleServerMsgData
//  Global resource dependence : none
//  Author: Naomi Huang 
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_HandleServerMsgData(void* buff,uint16 buff_len)
{
    MMI_WIN_ID_T    alert_win_id = SMS_EVENT_WARNING_WIN_ID;
    char *event_report = PNULL;
    char *pch;
    char *token[BMESSAGE_TOKEN_COUNT+1] = {0};
    uint8 event_type = 0;
    char *handle = PNULL;
    char *folder = PNULL;
    char *msg_type = PNULL;
    char *temp = PNULL;
    uint16 i;
    uint16 seek = 0;
    uint16 event_report_len = 0;
#if 0//defined(TRACE_MAP_BUFFER)
    uint16 len = 0;
    uint16 count = 0;
#endif

    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleServerMsgData buff_len = %d", buff_len);
    event_report_len = MIN(buff_len, BMESSAGE_CONTENT_MAX_LEN);
    event_report = SCI_ALLOC_APP(event_report_len);
    if(PNULL != event_report)
    {
        SCI_MEMSET(event_report, 0, event_report_len);
    }
    else
    {
        SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleServerMsgData have no memory to alloc!!");
        return 0;
    }
    temp = (char*) buff;
    SCI_MEMCPY(event_report,temp,event_report_len);
    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleServerMsgData ALLOC event_report_len = %d",event_report_len);
    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleServerMsgData buff 1 addr = 0x%x", buff);

#if 0//defined(TRACE_MAP_BUFFER)
    for( i = 0; i < (buff_len/TRACE_MAP_BUF_COUNT_BYTE) +1;  i++ )
    {
        SCI_TRACE_LOW("[MMISMS] TRACE_MAP_BUF_COUNT_BYTE, i = %d", i);
        len = buff_len - i*TRACE_MAP_BUF_COUNT_BYTE;
        count = i*TRACE_MAP_BUF_COUNT_BYTE;

        if( len >= TRACE_MAP_BUF_COUNT_BYTE)
        {
            SCI_TRACE_BUF("[MMISMS] buff is :", (( BT_MAP_MNS_SERVER_DATA_IND_T * )event_report+count), TRACE_MAP_BUF_COUNT_BYTE);
        }
        else
        {
            SCI_TRACE_BUF("[MMISMS] buff is :", (( BT_MAP_MNS_SERVER_DATA_IND_T * )event_report+count), len);
        }
    }
#endif
    
    SCI_TRACE_LOW("[MMISMS] buff_len = %d", buff_len);
    
    for(i = 0; i < BMESSAGE_TOKEN_COUNT; i++)
    {
        token[i] = "";
    }

    i = 0;
    pch = strtok(event_report, "\"");
    
    while(pch != PNULL)
    {
        SCI_TRACE_LOW("[MMISMS] token = %s\n", pch);
        token[i] = pch;
        i++;
        pch = strtok(PNULL, "\"");
    }

    for(seek = 0; seek < BMESSAGE_TOKEN_COUNT; seek++)
    {
        if((strstr(token[seek], BMESSAGE_EVENT_TYPE) != PNULL))
        {
            SCI_TRACE_LOW("[MMISMS] event type = %s\n", token[seek+1]);

            if (strcmp(token[seek+1], EVENT_NEW_MESSAGE) == 0)
            {
                event_type = MMISMS_MAP_EVENT_NEW_MESSAGE;
            }
            else if (strcmp(token[seek+1], EVENT_DELIVERY_SUCCESS) == 0)
            {
                event_type = MMISMS_MAP_EVENT_DELIVERY_SUCCESS;
            }
            else if (strcmp(token[seek+1], EVENT_SENDING_SUCCESS) == 0)
            {
                event_type = MMISMS_MAP_EVENT_SENDING_SUCCESS;
            }
            else if (strcmp(token[seek+1], EVENT_DELIVERY_FAILURE) == 0)
            {
                event_type = MMISMS_MAP_EVENT_DELIVERY_FAILURE;
            }
            else if (strcmp(token[seek+1], EVENT_SENDING_FAILURE) == 0)
            {
                event_type = MMISMS_MAP_EVENT_SENDING_FAILURE;
            }
            else if (strcmp(token[seek+1], EVENT_MEMORY_FULL) == 0)
            {
                event_type = MMISMS_MAP_EVENT_MEMORY_FULL;
            }
            else if (strcmp(token[seek+1], EVENT_MEMORY_AVAILABLE) == 0)
            {
                event_type = MMISMS_MAP_EVENT_MEMORY_AVAILABLE;
            }
            else if (strcmp(token[seek+1], EVENT_MESSAGE_DELETED) == 0)
            {
                event_type = MMISMS_MAP_EVENT_MESSAGE_DELETED;
            }
            else if (strcmp(token[seek+1], EVENT_MESSAGE_SHIFT) == 0)
            {
                event_type = MMISMS_MAP_EVENT_MESSAGE_SHIFT;
            }
        }

        if((strstr(token[seek], BMESSAGE_HANDLE) != PNULL))
        {
            SCI_TRACE_LOW("[MMISMS] handle = %s\n", token[seek+1]);
            handle = token[seek+1];
        }

        if((strstr(token[seek], BMESSAGE_FOLDER) != PNULL))
        {
            SCI_TRACE_LOW("[MMISMS] folder = %s\n", token[seek+1]);
            folder = token[seek+1];
        }

        if((strstr(token[seek], BMESSAGE_MSG_TYPE) != PNULL))
        {
            SCI_TRACE_LOW("[MMISMS] msg type = %s\n", token[seek+1]);
            msg_type = token[seek+1];
        }
    }

    if( MMK_IsOpenWin(MMISMS_BT_MTBOX_WIN_ID))
    {
        MMK_CloseWin(MMISMS_BT_MTBOX_WIN_ID);
    }
    else if( MMK_IsOpenWin(MMISMS_MOBOX_WIN_ID))
    {
        MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
    }
    else if( MMK_IsOpenWin(MMISMS_BT_SENDSUCCBOX_WIN_ID))
    {
        MMK_CloseWin(MMISMS_BT_SENDSUCCBOX_WIN_ID);
    }

    switch(event_type)
    {
    case MMISMS_MAP_EVENT_NEW_MESSAGE:
        {
			SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_NEW_MESSAGE");
			MMIAPISMS_ShowNewBTMsgPrompt();
            if( TRUE == MMISMS_GetMapAllowSync() )
            {
                MMIAPISMS_SetIsUpdateInbox(TRUE);
            }
            /*
            if(MMISMS_BT_BOX_MT == MMISMS_GetCurBoxType())
            {
                MMK_SendMsg(MMISMS_BT_MTBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            */
        }
        break;

    case MMISMS_MAP_EVENT_DELIVERY_SUCCESS:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_DELIVERY_SUCCESS");
            MMIAPISMS_SetIsUpdateOutbox(TRUE);
            MMIAPISMS_SetIsUpdateSendbox(TRUE);
            MMISMS_SetMapAllowSync(TRUE);
            
            if(MMISMS_BT_BOX_SENDSUCC == MMISMS_GetCurBoxType())
            {
                MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            else if(MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurBoxType())
            {
                MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_DELIVERY_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        break;
    case MMISMS_MAP_EVENT_SENDING_SUCCESS:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_SENDING_SUCCESS");
            if(MMIAPISMS_GetIsSendMultiSms())
            {
                if(MMIAPISMS_GetSendAddrCount() == 0)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_SENDING_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_SENDING_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            MMIAPISMS_SetIsUpdateSendbox(TRUE);
            if( (TRUE == MMISMS_GetMapAllowSync() ) && (MMISMS_BT_BOX_SENDSUCC == MMISMS_GetCurBoxType()) )
            {
                MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
    case MMISMS_MAP_EVENT_DELIVERY_FAILURE:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_DELIVERY_FAILURE");
            MMIAPISMS_SetIsUpdateOutbox(TRUE);
            MMIAPISMS_SetIsUpdateSendbox(TRUE);
            MMISMS_SetMapAllowSync(TRUE);
            
            if(MMISMS_BT_BOX_SENDSUCC == MMISMS_GetCurBoxType())
            {
                MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            else if(MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurBoxType())
            {
                MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_DELIVERY_FAILURE,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        break;
    case MMISMS_MAP_EVENT_SENDING_FAILURE:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_SENDING_FAILURE");
            if(MMIAPISMS_GetIsSendMultiSms())
            {
                if(MMIAPISMS_GetSendAddrCount() == 0)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_SENDING_FAILURE,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_SENDING_FAILURE,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            MMIAPISMS_SetIsUpdateOutbox(TRUE);
            if( (TRUE == MMISMS_GetMapAllowSync() ) && (MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurBoxType()) )
            {
                MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }            
            
        }
        break;
    case MMISMS_MAP_EVENT_MEMORY_FULL:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_MEMORY_FULL");
        }
        break;
    case MMISMS_MAP_EVENT_MEMORY_AVAILABLE:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_MEMORY_AVAILABLE");
        }
        break;
    case MMISMS_MAP_EVENT_MESSAGE_DELETED:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_MESSAGE_DELETED");
            
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_MSE_MSG_DELETED,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            
            if(PNULL != folder)
            {
                if(strcmp(folder, FOLDER_INBOX_PATH) == 0)
                {
                    if( MMK_IsOpenWin(MMISMS_BT_MTBOX_WIN_ID))
                    {
                        MMK_CloseWin(MMISMS_BT_MTBOX_WIN_ID);
                    }
                    MMIAPISMS_SetIsUpdateInbox(TRUE);
                    MMK_SendMsg(MMISMS_BT_MTBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
                else if(strcmp(folder, FOLDER_OUTBOX_PATH) == 0)
                {
                    if( MMK_IsOpenWin(MMISMS_MOBOX_WIN_ID))
                    {
                        MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
                    }                    
                    MMIAPISMS_SetIsUpdateOutbox(TRUE);
                    MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
                else if(strcmp(folder, FOLDER_SENTBOX_PATH) == 0)
                {
                    if( MMK_IsOpenWin(MMISMS_BT_SENDSUCCBOX_WIN_ID))
                    {
                        MMK_CloseWin(MMISMS_BT_SENDSUCCBOX_WIN_ID);
                    }
                    MMIAPISMS_SetIsUpdateSendbox(TRUE);
                    MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
                else if(strcmp(folder, FOLDER_DRAFT_PATH) == 0)
                {
                    if( MMK_IsOpenWin(MMISMS_MOBOX_WIN_ID))
                    {
                        MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
                    }
                    MMIAPISMS_SetIsUpdateDraft(TRUE);
                    MMK_SendMsg(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
            }
        }
        break;
    case MMISMS_MAP_EVENT_MESSAGE_SHIFT:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_MAP_EVENT_MESSAGE_SHIFT");
            MMIAPISMS_SetIsUpdateOutbox(TRUE);
            MMIAPISMS_SetIsUpdateSendbox(TRUE);
            MMISMS_SetMapAllowSync(TRUE);
            
            if(MMISMS_BT_BOX_SENDSUCC == MMISMS_GetCurBoxType())
            {
                MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            else if(MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurBoxType())
            {
                MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
    default:
        break;
    }

    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleServerMsgData buff 2 addr = 0x%x", buff);
    if (PNULL != event_report)
    {
        SCI_FREE(event_report);
        event_report = PNULL;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC BT_MSG_LIST_T* MMIAPISMS_GetMessageTableList(void)
{
    return s_message_table_head;
}

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ShowNewBTMsgPrompt(void)
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
    
    MMI_GetLabelTextByLang(TXT_SMS_NEW_BT_MSG, &tmp_prompt);
    
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
    
    
    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);
    
    if (PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr = PNULL;
    }
	
}

/*****************************************************************************/
// Description : MMIAPISMS_HandleBTMapRegister
// Global resource dependence : 
// Author: Naomi Huang
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleBTMapRegister(void)
{
#ifndef WIN32
    BT_STATUS       reg_notify_ret = BT_ERROR;

    reg_notify_ret = BT_MapSetNotifyRegistration(TRUE);
    SCI_TRACE_LOW("[MMISMS] reg_notify_ret = %d", reg_notify_ret);


    if(BT_PENDING != reg_notify_ret)
    {
        MMISMS_OpenAlertWarningWin(TXT_BT_NOTIFY_REGISTER_FAIL);    
    }
#endif
}

/*****************************************************************************/
//     Description : delete all bt mails
//    Global resource dependence : 
//  Author: 
//    Note: 为mail模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_HandleMsgDeleteStatus(void)
{
    MN_SMS_RECORD_ID_T record_id = 0;
    MMI_WIN_ID_T  query_win_id = MMISMS_DELETE_ANIM_WIN_ID;

    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleMsgDeleteStatus");
    record_id = g_mmisms_global.operation.cur_order_id_arr[g_mmisms_global.operation.cur_order_index]->record_id;

    SCI_TRACE_LOW("[MMISMS] record_id = %d", record_id);
    DeleteMsgHandleTableList(s_message_table_head, record_id);
    
    MMISMS_DelBtSmsInfoInOrder(MMISMS_GetCurOperationOrderId());
    MMISMS_SetNVFolderTypeFlag(record_id,MMISMS_FOLDER_BT);

    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);

    if (MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID))
    {
        SCI_TRACE_LOW("[MMISMS] MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID)");
        MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL,0);
    }
    else
    {
        MMISMS_SetDelAllFlag(FALSE);
        //全部删除完毕，将未处理链表中的sms插入到相应的链表中
        MMISMS_InsertUnhandledSmsToOrder();
        //MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
        MMISMS_HandleOrderChangeCallBack(); //删除完毕刷新list
        MMIAPIALM_CheckAndStartDelayedAlarm();
    }

    return TRUE;

}

/*****************************************************************************/
//     Description : MMIAPISMS_HandleMsgReadStatus
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleMsgReadStatus(void)
{
    MMI_WIN_ID_T  query_win_id = MMISMS_QUERY_WIN_ID;
    MMI_WIN_ID_T  conn_win_id = MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID;

    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleMsgReadStatus");
    if(MMK_IsOpenWin(MMISMS_QUERY_WIN_ID))
    {
        SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleMsgReadStatus MMK_IsOpenWin(MMISMS_QUERY_WIN_ID)");
        MMIPUB_CloseQuerytWin(&query_win_id);
    }
    
    if(MMK_IsOpenWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID))
    {
        SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleMsgReadStatus MMK_IsOpenWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID)");
        MMIPUB_CloseQuerytWin(&conn_win_id);
    }

    MMISMS_UpdateBtUnreadOrder();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMI_STRING_T MMIAPISMS_GetLongMessageData(void)
{
    return p_long_sms;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_ClearLongMessageData(void)
{
	
	SCI_TRACE_LOW("[MMISMS] MMIAPISMS_ClearLongMessageData");
	
    if( PNULL != p_long_sms.wstr_ptr)
    {
        SCI_FREE(p_long_sms.wstr_ptr)
    }
    p_long_sms.wstr_len = 0;
}

/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
PUBLIC void MMIAPISMS_MapAbortClient(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    MMI_STRING_T                wait_text = {0};
    ret_val = BT_MapAbortClient();
    
    if(BT_SUCCESS == ret_val)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text); 
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleBtWaitWinMsg); 
        MMIPUB_SetWinSoftkey(MMIBT_WAIT_WIN_ID, TXT_NULL, TXT_NULL, TRUE);
        MMK_CloseWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : MMIAPISMS_HandleSetMsgStatusAbort
//  Global resource dependence : none
//  Author: Naomi Huang
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_HandleSetMsgStatusAbort(BT_MAP_RESP_CODE_T resp_code)
{
    MMI_WIN_ID_T  delete_win_id = MMISMS_DELETE_ANIM_WIN_ID;
    MMI_WIN_ID_T  alert_win_id = SMS_EVENT_WARNING_WIN_ID;
    MMI_STRING_T prompt_str = {0};

    SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleSetMsgStatusAbort fail reason is 0x%x.", resp_code);
    
    if(MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID))
    {
        SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleSetMsgStatusAbort MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID)");
        MMIPUB_CloseQuerytWin(&delete_win_id);
    }

    if(MMK_IsOpenWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID))
    {
        SCI_TRACE_LOW("[MMISMS] MMIAPISMS_HandleSetMsgStatusAbort MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID)");
        MMK_CloseWin(MMISMS_SMS_WAIT_BT_CONNECTION_WIN_ID);
    }

    if(BT_MRC_SERVICE_UNAVAILABLE == resp_code || BT_MRC_PRECONDITION_FAILED == resp_code)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_LIST_NOT_SYNC,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        SCI_TRACE_LOW("[MMISMS] MMISMS_GetCurBoxType = %d", MMISMS_GetCurBoxType());
        if(MMISMS_BT_BOX_SENDSUCC == MMISMS_GetCurBoxType())
        {
            MMISMS_SetMapAllowSync(TRUE);
            MMIAPISMS_SetIsUpdateSendbox(TRUE);
            MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
        else if(MMISMS_BT_BOX_SENDFAIL == MMISMS_GetCurBoxType())
        {
            MMISMS_SetMapAllowSync(TRUE);
            MMIAPISMS_SetIsUpdateOutbox(TRUE);
            MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
        else if(MMISMS_BT_BOX_MT == MMISMS_GetCurBoxType())
        {
            MMISMS_SetMapAllowSync(TRUE);
            MMIAPISMS_SetIsUpdateInbox(TRUE);
            MMK_SendMsg(MMISMS_BT_MTBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
        else if(MMISMS_BT_BOX_NOSEND == MMISMS_GetCurBoxType())
        {
            MMISMS_SetMapAllowSync(TRUE);
            MMIAPISMS_SetIsUpdateDraft(TRUE);
            MMK_SendMsg(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DELETE_FAILURE,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_SYNC_TYPE_E MMIAPISMS_GetSMSSyncType(void)
{
    return sync_type;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_E type)
{
    if( (type >= MMISMS_SYNC_TYPE_NONE ) && (type <= MMISMS_SYNC_TYPE_MAP ) )
	{
		sync_type = type;
	}
}
#endif

/*****************************************************************************/
//  Description: 注入函数
//  Parameter: [In] p_func   //外部模块向mmi sms注入的函数
//             [Out] none
//             [Return] BOOLEAN
//  Author: yang.wang3
//  Note:外部模块可调用此接口注入函数，p_func在MMISMSAPP_SendSmsWithData接口中会被调用
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_Inject_Function(MMISMS_INJECTION_FUNC p_func)
{
    BOOLEAN        ret              = TRUE;

    SCI_TRACE_LOW("MMISMSAPP_Inject_Function Enter!");

    if(PNULL == p_func)
    {
        SCI_TRACE_LOW("MMISMSAPP_Inject_Function p_func is PNULL!");
        return FALSE;
    }

    if(PNULL != s_p_injection_func)
    {
        SCI_TRACE_LOW("MMISMSAPP_Inject_Function have injected func!");
        return FALSE;
    }

    s_p_injection_func = p_func;
    SCI_TRACE_LOW("MMISMSAPP_Inject_Function injection_func:%0x",p_func);
    SCI_TRACE_LOW("MMISMSAPP_Inject_Function Exit,ret:%d",ret);
    return ret;
}

/*****************************************************************************/
//  Description:取消注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] BOOLEAN
//  Author: yang.wang3
//  Note:外部模块可调用此接口取消注入函数，调用完此接口后，MMISMSAPP_SendSmsWithData中将不会再调用注入函数
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_Uninject_Function(void)
{
    BOOLEAN  ret = TRUE;
    SCI_TRACE_LOW("MMISMSAPP_Uninject_Function Enter!");
    if(PNULL == s_p_injection_func)
    {
        SCI_TRACE_LOW("MMISMSAPP_Uninject_Function s_p_injection_func is PNULL!");
        return FALSE;
    }
    s_p_injection_func = PNULL;
    SCI_TRACE_LOW("MMISMSAPP_Uninject_Function Exit!");
    return ret;
}

/*****************************************************************************/
//  Description: get注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] injection_func    //注入函数
//  Author: yang.wang3
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_INJECTION_FUNC MMISMSAPP_Get_Injection_Func(void)
{
    MMISMS_INJECTION_FUNC p_func = PNULL;
    SCI_TRACE_LOW("MMISMSAPP_Get_Injection_Func Enter!");
    p_func = s_p_injection_func;
    SCI_TRACE_LOW("MMISMSAPP_Get_Injection_Func p_func:%0x",p_func);
    return p_func;
}

