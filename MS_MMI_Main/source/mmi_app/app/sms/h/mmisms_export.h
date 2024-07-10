/***************************************************************************
** File Name:      mmisms_api.h                                            *
** Author:                                                                 *
** Date:           09/22/2004                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the SMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 09/2004        louis.wei        Create
** 04/2006          Tracy Zhang       Modularize
****************************************************************************/
#ifndef  _MMISMS_EXPORT_H_    
#define  _MMISMS_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#include "mmk_type.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipb_export.h"
#include "mmivc_common.h"
#include "mmi_event_api.h"
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmiacc_event.h"
#endif
//#include "mmisms_app.h"
#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
#include "bt_abs.h"
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

#define SMSAPP_SRV_SUPPORT

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define     MMISMS_DEFAULT_CHAR_LEN     153   // the length of default split SMS
#define MMIMESSAGE_DIAPLAY_LEN        14
#define MMISMS_PBNUM_MAX_LEN		( MMIPB_BCD_NUMBER_MAX_LEN << 1 )
#define MMISMS_NUMBER_MAX_LEN   40     //baokun change 20 to 40
#define  MMISMS_NUMBER_MAX_BCD_LEN    20 //baokun change 10 to 20

#define MMISMS_BCD_NUMBER_MAX_LEN /*10*/20

#define MMISMS_MAX_SC_NUMBER_LEN  20
        
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
#define MMISMS_MAX_SIGNATURE_NUMBER_LEN  20
#endif

#define MMISMS_SEARCH_MAX_LEN       20

#ifdef MMI_ETWS_SUPPORT
#define CB_MSG_TIME_MAX_LEN                   24                                  //小区广播消息接受时间最大长度
#define CB_PWS_MCC 466
#endif

typedef BOOLEAN (* SMS_SEND_CALLBACK) (BOOLEAN is_succss, DPARAM parm);

#define PBDETAIL_MSG_LOG_MAX_NUM 200
#ifdef MMI_MMS_LIST_DISPLAY_MULTI_PEOPLE
#define MMISMS_SENDER_DISPLAY_MAX             20
#define MMISMS_SENDER_DISPLAY_STRING_MAX      100
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 
/*外部模块注入的函数定义*/
typedef BOOLEAN (*MMISMS_INJECTION_FUNC) (uint32 event, void* p_param, uint32 param_size);

typedef enum
{
     MMISMS_VMN_OPER_TYPE_CALL_DIVERT,       //setting中查询呼叫转移     MMISMS_VMN_OPER_TYPE_CALL_DIVERT,       //setting中查询呼叫转移
     MMISMS_VMN_OPER_TYPE_SMS_VM_SETTING,    //sms中VMN设置
     MMISMS_VMN_OPER_TYPE_IDLE_DIAL,         //idle下快捷拨号
     MMISMS_VMN_OPER_TYPE_MAX
}MMISMS_VMN_OPER_TYPE_E;
        
typedef enum
{
    MMISMS_BOX_NONE,
    MMISMS_BOX_MT,
    MMISMS_BOX_SENDSUCC,
    MMISMS_BOX_SENDFAIL,
    MMISMS_BOX_NOSEND,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    MMISMS_BOX_SECURITY,
#endif    
    MMISMS_BOX_ALL,
//#ifdef MMI_SMS_CHAT_SUPPORT    
    MMISMS_BOX_CHAT,
//#endif
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_BT_BOX_MT,
    MMISMS_BT_BOX_SENDSUCC,
    MMISMS_BT_BOX_SENDFAIL,
    MMISMS_BT_BOX_NOSEND,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    MMISMS_BT_BOX_SECURITY,
#endif    
    MMISMS_BT_BOX_ALL,
#ifdef MMI_SMS_CHAT_SUPPORT    
    MMISMS_BT_BOX_CHAT
#endif
#endif
}MMISMS_BOX_TYPE_E;

typedef enum
{
    MMISMS_OPERATE_NONE,
    MMISMS_WRITE_NEW_SMS,
    MMISMS_OPEN_BOX,
    MMISMS_VIEW_MEMORY,
    MMISMS_OPEN_MAIN_MENU
}MMISMS_OPERATE_TYPE_E;

//@cr36401 jian.ma change for mms
typedef struct _APP_MMS_USER_DATA_HEAD_T
{
    uint8 length;                                          // the length
    uint8 user_header_arr [ 40 ]; // the data buf 
} APP_MMS_USER_DATA_HEAD_T;
//use for mms message struct
typedef struct _APP_MMS_USER_DATA_T
{
    APP_MMS_USER_DATA_HEAD_T  user_data_head_t;     // the head sturcture
    MN_SMS_USER_VALID_DATA_T  user_valid_data_t;    // the used valid data sructure
} APP_MMS_USER_DATA_T;

typedef struct 
{
    uint8    content_len;
    wchar    content[MMIMESSAGE_DIAPLAY_LEN + 1];//部分内容或者主题
}MMISMS_SMSDISPLAY_CONTENT_T;
typedef struct
{
    uint8   number;             // the number of dest addr
    uint8   dest_addr_id;        // the index of dest_addr
    uint8   dest_addr[ MMISMS_DEST_ADDR_MAX_NUM ][MMISMS_NUMBER_MAX_LEN+1]; // the destination address                                
}MMISMS_NUMBER_LIST_T;

//读取短信内容
typedef struct
{
    MMI_STRING_T time_string;   //时间
    MMI_STRING_T name_string;    //来自
    MMI_STRING_T content_string;     //内容
}MMISMS_READMSG_CONTENT_T;

typedef struct 
{
    uint16                    failed_count;
    MMISMS_NUMBER_LIST_T    dest_list;
    uint8                    update_dest_addr[MMISMS_NUMBER_MAX_LEN+2];
}MMISMS_DEST_ADDR_INFO_T;

typedef enum
{
    MMISMS_R8_LANGUAGE_NONE = 0,
    MMISMS_R8_LANGUAGE_TURKISH,
    MMISMS_R8_LANGUAGE_SPANISH,
    MMISMS_R8_LANGUAGE_PORTUGUESE,
    MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE,
    MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH,
    MMISMS_R8_LANGUAGE_LATIN,
    MMISMS_R8_LANGUAGE_UNICODE
}MMISMS_R8_LANGUAGE_E;

typedef struct 
{
    uint16                    length;                                    // the length of data
    uint16                    split_sms_length[MMISMS_SPLIT_MAX_NUM];
    MN_SMS_ALPHABET_TYPE_E  alphabet;                                // the type of data
    MN_SMS_ALPHABET_TYPE_E  alphabet_ori[MMISMS_SPLIT_MAX_NUM];                                // the type of data
    MMISMS_R8_LANGUAGE_E language;
    uint8 sms_num;
    BOOLEAN is_single_shift;    
    uint8                    data[MMISMS_SPLIT_MAX_NUM * MMISMS_DEFAULT_CHAR_LEN];    // the data of user inputting
}MMISMS_CONTENT_T;

//短信编辑相关信息
typedef struct 
{
    BOOLEAN is_need_save;
    BOOLEAN is_vacard_vcal;                          // 是否vcar或者是vcal
    BOOLEAN is_enough_space_backsend;                //不够保存后台短信，那就不保存，发送就可以了
#ifdef SFR_SUPPORT
    BOOLEAN is_sfr_sms;
#endif
    //是否需要保存，默认需要，除非空间不足了
    BOOLEAN is_resend;
    MN_DUAL_SYS_E dual_sys;                //卡
    MMISMS_DEST_ADDR_INFO_T dest_info;     //收件人信息     
    MN_SMS_USER_DATA_HEAD_T tpdu_head;     //头信息       
    MMISMS_CONTENT_T send_message;         //内容
} MMISMS_SMSEDIT_CONTENT_T;

typedef MMISMS_SMSDISPLAY_CONTENT_T MMISMS_MMSDISPLAY_CONTENT_T;

typedef enum
{
    MMISMS_TYPE_SMS         = 0,
    MMISMS_TYPE_MMS         = 1,
    MMISMS_TYPE_WAP_PUSH = 2,
    MMISMS_TYPE_MMS_OTA  = 3,
    MMISMS_TYPE_WAP_OTA  = 4,
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    MMISMS_TYPE_MAIL     = 5,
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
    MMISMS_TYPE_MAX
} MMISMS_MSG_TYPE_E;   //当前的消息类型占3位.

typedef enum mmisms_mo_mt_type_tag
{
    MMISMS_MT_NOT_DOWNLOAD, //未下载mms
    MMISMS_MT_TO_BE_READ,   //未读信息             
    MMISMS_MT_HAVE_READ,    //已读信息                    
    MMISMS_MT_SR_TO_BE_READ,//未读状态报告            
    MMISMS_MT_SR_HAVE_READ,    //已读状态报告                
    MMISMS_MO_SEND_SUCC,    //已发信息                    
    MMISMS_MO_SEND_FAIL,    //发送失败信息                    
    MMISMS_MO_DRAFT,        //未发送信息
    MMISMS_MT_NOT_NOTIFY, //未上报mms
    MMISMS_MT_NEED_NOT_DOWNLOAD, //彩信下载失败和不重复下载，NO_SPACE，INVALIDPERIOD，INVALID_URL，NOT_FOUND
} MMISMS_MO_MT_TYPE_E;   //当前的消息类型占4位，如果需要添加类型则需要增加位数。


typedef enum
{
    MMISMS_FOLDER_NORMAL = 0,
    MMISMS_FOLDER_SECURITY,
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
	MMISMS_FOLDER_BT,
#endif
    MMISMS_FOLDER_MAX
} MMISMS_FOLDER_TYPE_E;   //当前的文件夹类型占3位.

typedef enum
{
    MMISMS_SAVEPOS_NONE,
    MMISMS_SAVEPOS_SIM,
    MMISMS_SAVEPOS_PHONE,
    MMISMS_SAVEPOS_MAX
}MMISMS_SMS_SAVEPOS_TYPE_E;

typedef enum
{
    MMISMS_SETTING_PATH_DEFAULT,
    MMISMS_SETTING_PATH_CUSTOM,
    MMISMS_SETTING_PATH_TYPE_MAX
}MMISMS_SETTING_PATH_TYPE;

typedef struct tag_MMISMS_BCD_NUMBER_T
{
    uint8   number_len;                         // number数组的BYTE个数
    uint8   npi_ton;                            // 具体意义请查看GSM 04.08
    uint8   number[MMISMS_BCD_NUMBER_MAX_LEN];    // 具体意义请查看GSM 04.08 
} MMISMS_BCD_NUMBER_T;

//sms status used for back send orderlist ui
typedef enum
{
    MMISMS_SENDSTATUS_NONE,
    MMISMS_SENDSTATUS_SAVING,
    MMISMS_SENDSTATUS_WAITING,
    MMISMS_SENDSTATUS_SENDING,
    MMISMS_SENDSTATUS_SENDFAIL,
    MMISMS_SENDSTATUS_SENDSUCC,
    MMISMS_SENDSTATUS_MAX
}MMISMS_BACKSENDSTATUS_E;

#ifdef MMIMMS_SMS_IN_1_SUPPORT
//编辑彩信时候进行的选项菜单操作的类型（插入音乐，插入图片，插入视频...)
typedef enum
{
    MMISMS_INSERT_INIT,
    MMISMS_INSERT_MUSIC,  //插入音乐文件
    MMISMS_INSERT_PIC,    //插入图片文件
    MMISMS_INSERT_VIDEO,  //插入视频文件
    MMISMS_INSERT_ADD_ATTACH, //附件
    
}MMISMS_CHAT_INSERT_TYPE;  

typedef enum
{
    MMISMS_SELECT_WORD_SMS_MMS_EDIT = 0,  //短信编辑进入
    MMISMS_SELECT_WORD_CHAT_EDIT,   //聊天室进入
    MMISMS_SELECT_WORD_TYPE_MAX
} MMISMS_SELECT_WORD_TYPE_E;   
#endif

typedef enum
{
    MMISMS_SL_OPERATION_ALWAYS,
    MMISMS_SL_OPERATION_PROMPT,
    MMISMS_SL_OPERATION_NEVER,
    MMISMS_SL_OPERATION_MAX,
}MMISMS_SL_OPERATION_E;

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
typedef enum
{
    MMISMS_SYNC_TYPE_NONE,
    MMISMS_SYNC_TYPE_SPP,        
    MMISMS_SYNC_TYPE_MAP
}MMISMS_SYNC_TYPE_E;
#endif

typedef struct MMISMS_STATE_T_tag
{
    uint16                    head_ref;                // the reference number of SMS
    uint8                     longsms_max_num;       //长短消息的最大sms数目;
    uint8                     longsms_seq_num;        //长短消息的每条sms的sequence number
    uint32                    time;                    // the sc timer
    int32                     record_id;                // the record_id of SMS,MMS,OTA OR WAP PUSH
#ifdef DROPDOWN_NOTIFY_SHORTCUT
    uint32                    notify_handle;
#endif
    struct 
    {
        uint32               storage                :2;
        uint32                 is_ucs2                :1;
        uint32               mo_mt_type             :4;
        uint32               is_concatenate_sms        :1;    
        uint32                 is_receive_all         :1;
        uint32                 msg_type               :3;//信息的类型
        uint32                 dual_sys               :3;//双卡信息
        uint32               is_marked              :1;//标记信息
        uint32               is_locked              :1;
        uint32              is_need_update      :1;//是否需要更新状态
        uint32              folder_type             :3;//文件夹类型        
        uint32              is_search                   :1;	
        uint32              is_resend                   :1;	
    }  flag;
    
    union
    {
        MMISMS_BCD_NUMBER_T        bcd_number; //sms
        uint8                      sender[MMISMS_NUMBER_MAX_LEN+1]; //mms,wap push,wap ota,mms ota    
    } number;

    MMISMS_SMSDISPLAY_CONTENT_T    display_content;
    MMISMS_BACKSENDSTATUS_E        backsend_status;  //status of sms back send
    struct MMISMS_STATE_T_tag     *next_ptr;
    struct MMISMS_STATE_T_tag     *prev_ptr;
    struct MMISMS_STATE_T_tag     *next_chat_ptr;
    struct MMISMS_STATE_T_tag     *prev_chat_ptr;
    struct MMISMS_STATE_T_tag     *next_long_sms_ptr;    

}MMISMS_STATE_T;

typedef struct MMISMS_ORDER_INFO_T_tag
{
    uint32                          time;               // 时间(sec) 
    int32                           record_id;          // record id;
    MMISMS_MO_MT_TYPE_E             mo_mt_type;         // 已读，未读
    MMISMS_MSG_TYPE_E               msg_type;           // 信息类型
    MN_DUAL_SYS_E                   dual_sys;           // 双卡参数
    MMISMS_MMSDISPLAY_CONTENT_T     display_content;    // 显示的内容/主题
    uint8                           sender[MMISMS_NUMBER_MAX_LEN+1];
    BOOLEAN                     	is_lock;
    MMISMS_FOLDER_TYPE_E            folder_type;//信箱类型
    uint32                          file_size;          //对应存储条目的大小
    int8                            textimage_order;
    struct MMISMS_ORDER_INFO_T_tag  *next;
}MMISMS_ORDER_INFO_T;

typedef BOOLEAN (*SMSAPI_VALIDATESECURITY_CALLBACK)(uint32 validate_result);
//other module send sms info struct,for java,vacard,msd,dm,email,etc..
typedef struct MMI_BINARY_SMS_tag
{
   BOOLEAN        is_binary;
   char           *binary_data;
   uint16         binary_length;
}MMI_BINARY_SMS_T;

typedef struct MMISMS_SEND_DATA_T_tag
{
    MN_DUAL_SYS_E  dual_sys;  
    BOOLEAN        is_need_packet;       //是否压缩发送，如果只是发送短消息可以压缩，使用应用不压缩，防止服务器端 不识别
    uint32         src_port;
    uint32         dst_port;
    uint8          *dest_addr_ptr;
    uint8          dest_addr_len; 
    MMI_BINARY_SMS_T     binary_sms_data;    
    MMI_STRING_T   *sms_content_ptr;
    BOOLEAN is_force_rpt;
    SMS_SEND_CALLBACK send_callback;    //回调函数，当发送有结果send_cnf的时候调用该函数
#ifdef SFR_SUPPORT
    BOOLEAN is_sfr_sms;
#endif
} MMISMS_SEND_DATA_T;

#ifdef MMI_INDIAN_SOS_SUPPORT
typedef struct MMISMS_MULTI_SEND_DATA_T_tag
{
    MN_DUAL_SYS_E  dual_sys;  
    uint16 dest_num_len;/*actual number length*/
    BOOLEAN is_need_packet;
    uint8 dest_num[ MMISET_SOS_CONTACTS_MAX_NUM ][MMISMS_NUMBER_MAX_LEN+1];/*contact's number*/
    MMI_STRING_T   *sms_content_ptr;
    SMS_SEND_CALLBACK send_callback;    //????,??????send_cnf????????
} MMISMS_MULTI_SEND_DATA_T;
#endif

typedef MMISMS_STATE_T* MMISMS_ORDER_ID_T;    //链表指针数据类型

#ifdef MMI_PDA_SUPPORT
//for delete match data 
#define     PBDETAIL_MSG_LOG_MAX_NUM    200

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef void (*pSmsCallFunc)(BOOLEAN result);

typedef struct
{
    uint16              matched_count;
    uint16              last_matched_count;
    MMISMS_ORDER_ID_T   order_id_list[PBDETAIL_MSG_LOG_MAX_NUM];
    MMIPB_CONTACT_T *   contact_ptr;
    BOOLEAN             isSelecting;
    BOOLEAN             itemSelected[PBDETAIL_MSG_LOG_MAX_NUM];
    MMI_HANDLE_T        msg_select_win_handle;
    pSmsCallFunc        callback;               
}MMISMS_DETAIL_MSGLOG_DATA_T;
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
// 	Description : get contact info
//	Global resource dependence : 
//  Author: rong.gu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetContactInfo(uint32       handle,
                                     MMI_STRING_T *contact_info_ptr, 
                                     uint16       max_len);

#if 0
/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleSendOperation(MN_DUAL_SYS_E dual_sys,MMI_WIN_ID_T win_id,  BOOLEAN is_re_send);    
#endif
                               
/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SaveAsDraft(MN_DUAL_SYS_E    dual_sys,BOOLEAN is_replace,BOOLEAN is_timer_msg); 

/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SendSms(MN_DUAL_SYS_E    dual_sys,BOOLEAN is_re_send); 


/*****************************************************************************/
//     Description : to delete request to MN
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_DeleteReqToMN(MMISMS_BOX_TYPE_E        type, MMISMS_STATE_T *order_id, uint32    oper_status);

/*****************************************************************************/
// 	Description : get timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsTimerMode(void);                            

/*****************************************************************************/
// 	Description : Set timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsTimerMode(BOOLEAN is_timer_mode);

/*****************************************************************************/
//  Description : to handle the create select word win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateSelectWordWin(MMI_HANDLE_T inserted_ctrl);



/*****************************************************************************
// 	Description : set box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMIAPISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//     Description : to save the content which user input
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetGloblalMsgContent( uint16    length,wchar     *content_ptr);
/*****************************************************************************/
// 	Description : create a window according to timer msg index
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note: if index == MMISMS_TIMERMSG_MAX_ITEM create a timer msg interface with current time
//        otherwise   create a timer msg interface with timer msg info from NV
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateTimerMsgWin(void);

#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSendDualSys(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//     Description : to clear the id of destinations in g_mmisms_deliver
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearDestId(void);
#endif


/*****************************************************************************/
//  Description : init update info
//  Global resource dependence : g_mmisms_global
//  Author:rong.gu
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitUpdateInfo(void);

#if 0
/*****************************************************************************/
//  Description : to set the resend flag
//  Global resource dependence : g_mmisms_deliver
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDeliverResendFlag(BOOLEAN     is_resend);
#endif
#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description :发送Vcard短信
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcard(MN_DUAL_SYS_E dual_sys);
#endif

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description :发送Vcalendar短信
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcalendar(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : to set the g_mmisms_global.operation.cur_order_index
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void    MMIAPISMS_SetCurOrderArrIndex(int index);

/*****************************************************************************/
//     Description : Compute Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC MMISMS_STATE_T* MMIAPISMS_GetCurOperationOrderId(void);
#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMIAPISMS_TIMERMSG_GetCurEvent(void);
#endif
/*****************************************************************************/
//     Description : get dest dual sys
//    Global resource dependence : g_mmisms_deliver
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E    MMIAPISMS_GetDestDualSys( void );

#if 0
/*****************************************************************************/
//     Description : set dest dual sys
//    Global resource dependence : g_mmisms_deliver
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDestDualSys( MN_DUAL_SYS_E dual_sys );
#endif

/*****************************************************************************/
//     Description : open the SMS sending window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenSendingWindow(MN_DUAL_SYS_E  dual_sys,MMI_STRING_T    *prompt_ptr);

/*****************************************************************************/
//     Description : to get the destation number according the pointed position
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetPointedDestNum(uint16 index, uint8 *num_len_ptr, uint8 *num_ptr);

/*****************************************************************************/
//     Description : to get the length according the alpha of message 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetLenByAlpha(MN_SMS_ALPHABET_TYPE_E alpha, uint8 *split_len_ptr,  uint8 *limit_len_ptr ,BOOLEAN  is_head_has_port);      

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_TIMERMSG_CheckTimerMSG(BOOLEAN is_direct);

#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSaveDualSys(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : get current read msg dual sys
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetCurReadMsgDualSys(void);
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************/
//     Description : init the SMS for ps
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/***************************************************************************/
void MMIAPISMS_InitForPS(void);

/***************************************************************************/
//     Description : init the SMS global
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/***************************************************************************/
void MMIAPISMS_AppInit(void);


#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : get delete all flag
//                  TRUE:        do delete all sms operation  
//                  FALSE:    without do delete all sms operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_GetDelAllFlag(void);
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : to enter the window for writing sms 
//    Global resource dependence : 
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMIAPISMS_WriteNewMessage(
                               MN_DUAL_SYS_E    dual_sys,           //IN:
                               MMI_STRING_T        *sms_init_ptr,        //IN:
                               uint8            *dest_addr_ptr,        //IN:
                               uint8            dest_addr_len        //IN:
                               );

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
                               );

/*****************************************************************************/
//     Description : set address to message
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetAddressToMessage(
                                       const uint8        *num_ptr,    //IN:
                                       uint8            num_len,        //IN:
                                       uint32   *dest_list_ptr
                                       );

/*****************************************************************************/
//  Description :  set dest number for write new msg
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: interface for other module/pb
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetAddressToMessageEx(
                                               uint8            *dest_addr_ptr,        //IN:
                                               uint8            dest_addr_len        //IN:
                                               );

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
                               );

/*****************************************************************************/
//     Description : to check the sms state
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_CheckSMSState( BOOLEAN    is_draw );

/*****************************************************************************/
//     Description : to check the space
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSFull( void );

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : to delete all sms
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_DelAllSMS( 
                         MMISMS_BOX_TYPE_E    box_type
                         );
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : to check the order is ok
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_IsOrderOk( void );

/*****************************************************************************/
//     Description : to check the sms is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_IsReady( void );

#ifdef MMI_SYNCTOOL_SUPPORT
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
                             );
#endif

//window related
/*****************************************************************************/
//     Description : to open the list of box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenSmsBox(
                          MMISMS_BOX_TYPE_E        box_type
                          );

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMIAPISMS_ShowNewMsgPrompt(void);

/*****************************************************************************/
//     Description : to open the sms main menu window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenMainMenuWin(void);

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
                                     );
#endif

/*****************************************************************************/
//  Description : init SMS Module
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitModule(void);

/*****************************************************************************/
//  Description : close all SMS window
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
void MMIAPISMS_CloseAllSMSWindow(void);

/*****************************************************************************/
//  Description : open deleting SMS window
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
void MMIAPISMS_OpenDelWindow(void);

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
//     Description : to open the window to call the voice mail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenCallVMWin(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : check the flag of sending SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSendingSMS(void);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//Description : Get SMS Sending SIM(Not In Sending return MMI_DUAL_SYS_MAX)
//    Global resource dependence : 
//  Author: wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetSendingSIM(void);
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : restore factory setting
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_RestoreFactorySetting(void);

/*****************************************************************************/
//     Description : to open the voice mail main window
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenVoicemailWin(void);

/*****************************************************************************/
//     Description : 将信息从order list 中删除
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteMsgInorder(MMISMS_MSG_TYPE_E msg_type, int32 record_id);

/*****************************************************************************/
//     Description : 插入一条信息到order list
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_InsertOneMsgToOrder(MMISMS_ORDER_INFO_T *order_ptr);

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : 更新聊天界面的彩信发送状态
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_UpdateResendMMSLabel(int32 record_id);

/*****************************************************************************/
//  Description : 清除发送文字接口
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearChatEditString(void);

/*****************************************************************************/
//  Description : 更新聊天界面封装接口
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_UpdateChatShow(MMISMS_ORDER_INFO_T *order_info_ptr);


/*****************************************************************************/
//  Description :是否锁屏是否未读短信
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsKeyLockNeedCountSms(BOOLEAN is_keylock_need_count_sms);

/*****************************************************************************/
//  Description :是否锁屏是否未读短信
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsKeyLockNeedCountSms(void);
#endif

/*****************************************************************************/
//     Description : 更新信息状态
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_UpdateMsgStatusInOrder(MMISMS_ORDER_INFO_T *order_info_ptr);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : 当MMS全部删除完毕后，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMMSDone(MMISMS_BOX_TYPE_E box_type);
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : 当MMS排序完成时，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_MMSOrderDone(void);

/*****************************************************************************/
//     Description : MMS是否已经排序完成
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_IsMMSOrderReady(void);

/*****************************************************************************/
//     Description : 当MMS初始化完成时，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC void MMIAPISMS_MMSInitDone(void);

/*****************************************************************************/
//     Description : Notify MN to save mms push.
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for udisk module
/*****************************************************************************/
void MMIAPISMS_SetSavePush( BOOLEAN is_save );

/*****************************************************************************/
//     Description : Begin read mms push for mms module.
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for udisk module
/*****************************************************************************/
BOOLEAN MMIAPISMS_ReadMmsPush( void );

/*****************************************************************************/
//     Description : 当打开新彩信窗口时，按下ok键调用此函数进入收件箱。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_OpenMtBox(void);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : Get the sms sort timer Id.
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISMS_GetSmsSortTimerId(void);

/*****************************************************************************/
//  Description :Insert the mms/wap/ota into sms list. 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleSortTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description: init sms order list
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitSmsOrder(void);

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadSmsCount(MN_DUAL_SYS_E dual_sys);

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
                                                );

#endif /* Reduce Code size */

/*****************************************************************************/
//  Description :clear dest number 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearDestNum(void);

#if defined(MMI_VCARD_SUPPORT) || defined(MMI_VCALENDAR_SUPPORT)
/*****************************************************************************/
// Description : to show the new Vcard prompt
//               (参考函数:MMISMS_ShowNewMsgPrompt)
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ShowNewVcardOrVcalendarPrompt(BOOLEAN  is_play_ring,MMIVC_SMS_SEND_TYPE_E type );
#endif

/*****************************************************************************/
//     Description : Open message main menu
//    Global resource dependence : 
//  Author:Lin.Lin
//    Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISMS_OpenMessageMainMenuWin(void);

/*****************************************************************************/
//     Description : API for is in sms moudle window
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsInSMSWin(void);

/*****************************************************************************/
//     Description : exit from sms module
//    Global resource dependence : 
//  Author: gang.tong
//    Note: 
/*****************************************************************************/
void MMIAPISMS_ExitFromSMS(void);

/*****************************************************************************/
//  Description : MMIAPISMS_RefreshSMS
//  Global resource dependence : 
//  Author: wancan.you
//RETURN: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_RefreshSMS(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : get sum size of all sms nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
uint32 MMIAPISMS_GetSumNvSize(void);

/*****************************************************************************/
//     Description : set default preset SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDefaultPresetSms(BOOLEAN is_clear);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetMaxSimSmsNum(MN_DUAL_SYS_E dual_sys, uint8 sim_sms_num);

/*****************************************************************************/
//     Description : to get the count of msg which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_E dual_sys, MMISMS_MSG_TYPE_E msg_type);

/*****************************************************************************/
//     Description : to get the count of undownload msg
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : to get the count of total message which is to be read(包括未读sms，mms，ota，push...)
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadMessageCount(MN_DUAL_SYS_E dual_sys);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//    Description : to create URL option WIN for MMS mdu
//    Global resource dependence : 
//    Author:Kun.Yu
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateWinURLOpt( ADD_DATA add_data );

#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : to get the resend count
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSBusy( void );

/*****************************************************************************/
// Description : 更新彩信图标
// Global resource dependence : 
// Author: minghu.mao
// Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC void MMIAPISMS_UpdateMsgStatus(void);

/*****************************************************************************/
//  Description : Get the push sms flag.
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetPushSmsBrowserInfo(void);

/*****************************************************************************/
//     Description : handle left and right msg in show SMS window
//    Global resource dependence : 
//  Author: minghu.mao
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleLeftRightMsgInShowSmsWin(
                                                     MMI_WIN_ID_T        win_id,        //IN:
                                                     MMI_MESSAGE_ID_E    msg_id        //IN:
                                                     );

#ifdef MMI_SMS_RETRY_SUPPORT
/*****************************************************************************/
//     Description : to set the resend count
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetResendCount( 
                               BOOLEAN     is_set_resend_count
                               );

/*****************************************************************************/
//     Description : to get the resend count
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetResendCount( void );
#endif

/*****************************************************************************/
//     Description : Set Need Sort
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetNeedSort( BOOLEAN is_sort );

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //delete all mail from message list
/*****************************************************************************/
//     Description : delete all mail from message list
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为mail模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMailMsg(void);
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
#ifdef SEARCH_SUPPORT
/*****************************************************************************/
//     Description : async search sms
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_StartLocalSearch(MMI_STRING_T *key_word_str,BOOLEAN is_mms_only);

/*****************************************************************************/
// 	Description : 本地搜索退出，短信模块释放模块内部资源
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_FreeLocalSearchResource(void);

/*****************************************************************************/
//     Description : stop async search sms
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_StopSearchSms(void);

#endif

/*****************************************************************************/
//     Description : insert string to sms edit
//    Global resource dependence : none
//  Author:
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPISMS_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             );

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : 设置当前的字体大小
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetWinFont(GUI_FONT_T  font);

/*****************************************************************************/
//  Description : get win font size
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMIAPISMS_GetWinFont(void);
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
                                                                                        BOOLEAN is_need_modified);

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsSMSRefresh
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsSMSRefresh(MN_DUAL_SYS_E dual_sys, BOOLEAN is_sms_refresh);

/*****************************************************************************/
//     Description : MMIAPISMS_OrderInit
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OrderInit(void);

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_CheckTimerMSG(void);

/*****************************************************************************/
//  Description : read new sms
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
void MMIAPISMS_ReadNewSms(
                          MMISMS_BOX_TYPE_E        box_type
                          );
#ifdef MMI_PDA_SUPPORT
PUBLIC void MMISMS_OpenVoiceMailWin(void);
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
                                      );

/*****************************************************************************/
//  Description : 得到第一条未读短信的日期和时间
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetOriginTimeAndDate(
                                        wchar    *date_ptr,        //OUT:
                                        wchar    *time_ptr        //OUT:
                                        );


#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//     Description : to Set the status report flag
//    Global resource dependence : g_mmisms_global
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetStatusReportFlag(BOOLEAN status_report_flag);

/*****************************************************************************/
//     Description : to set the save position
//    Global resource dependence : g_mmisms_global
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetSavePos(MMISMS_SMS_SAVEPOS_TYPE_E pos);
#endif

/*****************************************************************************/
//  Description : send the SMS for other modules
//  Global resource dependence :  
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SendSmsForOther(MMISMS_SEND_DATA_T *send_data_ptr);

#ifdef MMI_INDIAN_SOS_SUPPORT
/*****************************************************************************/
//  Description : send one or more sms messages
//  Global resource dependence :  
//  Author:Yongli Tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MultiSendSms(MMISMS_MULTI_SEND_DATA_T *send_data_ptr);
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
                                        );

                                 
#ifdef MMIMMS_SMS_IN_1_SUPPORT                                        
/*****************************************************************************/
// 	Description : get contact info
//	Global resource dependence : 
//  Author: rong.gu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetContactInfo(uint32       handle,
                                     MMI_STRING_T *contact_info_ptr, 
                                     uint16       max_len);
#if 0
/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleSendOperation(MN_DUAL_SYS_E dual_sys,MMI_WIN_ID_T win_id,  BOOLEAN is_re_send);    
#endif                               
/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SaveAsDraft(MN_DUAL_SYS_E    dual_sys,BOOLEAN is_replace,BOOLEAN is_timer_msg); 

/*****************************************************************************/
//     Description : to delete request to MN
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_DeleteReqToMN(MMISMS_BOX_TYPE_E        type, MMISMS_STATE_T *order_id, uint32    oper_status);

/*****************************************************************************/
// 	Description : get timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsTimerMode(void);                            

/*****************************************************************************/
// 	Description : Set timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsTimerMode(BOOLEAN is_timer_mode);


/*****************************************************************************
// 	Description : set box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMIAPISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//     Description : to save the content which user input
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetGloblalMsgContent( uint16    length,wchar     *content_ptr);
/*****************************************************************************/
// 	Description : create a window according to timer msg index
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note: if index == MMISMS_TIMERMSG_MAX_ITEM create a timer msg interface with current time
//        otherwise   create a timer msg interface with timer msg info from NV
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateTimerMsgWin(void);

#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSendDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : to clear the id of destinations in g_mmisms_deliver
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearDestId(void);
#endif


/*****************************************************************************/
//  Description : init update info
//  Global resource dependence : g_mmisms_global
//  Author:rong.gu
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitUpdateInfo(void);
#if 0
/*****************************************************************************/
//  Description : to set the resend flag
//  Global resource dependence : g_mmisms_deliver
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDeliverResendFlag(BOOLEAN     is_resend);
#endif
#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description :发送Vcard短信
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcard(MN_DUAL_SYS_E dual_sys);
#endif

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description :发送Vcalendar短信
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcalendar(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : to set the g_mmisms_global.operation.cur_order_index
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void    MMIAPISMS_SetCurOrderArrIndex(int index);

/*****************************************************************************/
//     Description : Compute Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC MMISMS_STATE_T* MMIAPISMS_GetCurOperationOrderId(void);
#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMIAPISMS_TIMERMSG_GetCurEvent(void);
#endif
#if 0
/*****************************************************************************/
//     Description : to add the phone number to the global variable
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDestNum(const uint8 *num_ptr, uint8  num_len);
#endif

/*****************************************************************************/
//     Description : get dest dual sys
//    Global resource dependence : g_mmisms_deliver
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E    MMIAPISMS_GetDestDualSys( void );

#if 0
/*****************************************************************************/
//     Description : set dest dual sys
//    Global resource dependence : g_mmisms_deliver
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDestDualSys( MN_DUAL_SYS_E dual_sys );
#endif

/*****************************************************************************/
//     Description : open the SMS sending window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenSendingWindow(MN_DUAL_SYS_E  dual_sys,MMI_STRING_T    *prompt_ptr);

/*****************************************************************************/
//     Description : to get the destation number according the pointed position
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetPointedDestNum(uint16 index, uint8 *num_len_ptr, uint8 *num_ptr);

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_TIMERMSG_CheckTimerMSG(BOOLEAN is_direct);

#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSaveDualSys(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : get current read msg dual sys
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetCurReadMsgDualSys(void);

/*****************************************************************************/
//     Description : get the sms is in the draft box
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MsgIsInDraftBox(void);

/*****************************************************************************/
//     Description : check me have enough space for save sms
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MEHaveEnoughSpace(void);

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsChatEditMMS(void);
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
/*****************************************************************************/
//     Description : Set Answer SMS Flag
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsAnwserSMS(BOOLEAN is_answer);
#endif

#endif

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : SMS备份函数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_IKeyBackup(MMIIKB_SIG_BACKUP_ITEM_DATA_REQ_T *backup_data_ptr);

/*****************************************************************************/
//  Description : SMS还原函数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_IKeyRestore(MMIIKB_SIG_RESTORE_ITEM_DATA_REQ_T *restore_data_ptr);
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
                                    );
#endif

/*****************************************************************************/
//     Description : to get the amount of the destination number
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8   MMIAPISMS_GetDestCount( void );

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_IsReady( void );

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSReady(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : get the edit content from global API
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:该结构在sms编辑时候用
/*****************************************************************************/
PUBLIC MMISMS_SMSEDIT_CONTENT_T * MMIAPISMS_GetEditContentFromGlobal(void);

/*****************************************************************************/
//  Description : Select SIM API of SMS app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISMS_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC	MMISMSSelectSIMWinCallback,
                                        ADD_DATA    add_data
                                        );

/*****************************************************************************/
//     Description : get current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetCurSendDualSys(void);

/* ----------------------------------------------------------------------------
* Function Name: MMISMS_GetPBNameByNumber
* Purpose:通过电话得到电话所属姓名
* Input:  uint8 *number_ptr:电话号码指针
MMI_STRING_T *name_pt:姓名结构
* Output: BOOLEAN.
* Returns:
* Author:
* ----------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPISMS_GetNameByNumberFromPB(uint8 *number_ptr, MMI_STRING_T *name_ptr, uint16 max_name_len);

/*****************************************************************************/
//     Description : get dual sys when new msg arrive
//    Global resource dependence : s_dual_new_sms
//  Author: bown.zhang
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetSimIdOfNewMsg(void);

/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_EnterSmsBox(MMISMS_BOX_TYPE_E  box_type);

/*****************************************************************************/
//     Description : to get the count of msg(sms mms wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetFirstUnreadMsgId(void);


#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to get the count of msg(wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetFirstUnreadPushMsgId(void);

/*****************************************************************************/
//     Description : open push box
//    Global resource dependence :
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenPushBox(void);
#endif

/*****************************************************************************/
//     Description : get the count of msg(sms\wap push\mms\..)for widget
//    Global resource dependence : 
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetAllUnreadMsgCount(void);

/*****************************************************************************/
//     Description : to get the save position
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_SMS_SAVEPOS_TYPE_E MMIAPISMS_GetSavePos(void);

/*****************************************************************************/
//     Description : set the SMS num of the read message
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
PUBLIC MMISMS_BOX_TYPE_E MMIAPISMS_GetCurBoxType(void);

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenPushMsgBox(void);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : Search MSG LIST
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC int16 MMIAPISMS_SearchMatchedItem(MMIPB_CONTACT_T * contact_ptr, MMISMS_DETAIL_MSGLOG_DATA_T *msgLogData);

/*****************************************************************************/
// 	Description : listitem draw callback
//	Global resource dependence :
//  Author:   
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetMatchItemContent(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data,BOOLEAN isSelecting);


#endif
/*****************************************************************************/
//     Description : handle open search msg in SMS list
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: include reading finished and etc
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenMatchedItem(uint32 user_data);

/*****************************************************************************/
//  Description :sms edit text property
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetEditTextProperty(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description :SmsBCDToPbBCD
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMIAPISMS_SmsBCDToPbBCD(MMISMS_BCD_NUMBER_T sms_num);


/*****************************************************************************/
//  Description : Enter Send Option Menu
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_EnterSendOptionMenu(uint32 send_type);


/*****************************************************************************/
//     Description : play new message ring
//    Global resource dependence : 
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_PlayNewSMSRing(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :Get not download Sms Image-Id 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetUndownLoadImageId(void);

/*****************************************************************************/
//  Description :Get Not-Read-Sms Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadSmsImageId(void);

/*****************************************************************************/
//  Description :Get Not-Read-Mms Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadMmsImageId(void);

/*****************************************************************************/
//  Description :Get Not-Read-WAPPUSH Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadWAPPUSHImageId(void);

/*****************************************************************************/
//  Description :Get Not-Read-WAPOTA Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadWAPOTAImageId(void);

/*****************************************************************************/
//  Description :Get FULL_MESSAGE Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetFullMessageImageId(void);

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetLocalSettings(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:jian.ma
//    Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMIAPISMS_NV_DelAllSmsItem(                                            //RETURN:
                                MN_SMS_RECORD_ID_T    sms_in_me_valid_num    //IN:
                                );

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :delete all SMSCB item in NV by MMI
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_NV_DelAllSmsCBItem(                                            //RETURN:
                                uint16    smscb_in_me_valid_num    //IN:
                                );
#endif

/*****************************************************************************/
//     Description :get normal close
//    Global resource dependence :  
//  Author:rong.gu
//    Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMIAPISMS_GetNormalClose(void);

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:rong.gu
//    Note:
/*****************************************************************************/    
PUBLIC void MMIAPISMS_SetNormalClose(BOOLEAN is_sms_normal_close);

/*****************************************************************************/
//     Description :  check and show new msg prompt when exsit
//    Global resource dependence : 
//  Author:taiping.lai
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_CheckAndShowNewMsg(void);

/*****************************************************************************/
//     Description :  check and show new msg prompt when exsit
//    Global resource dependence : 
//  Author:taiping.lai
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_CheckAndShowNewMsg(void);

#if MMI_SMSCB_SUPPORT
/******************************************************************************************************************/
//
//
//                        小区广播模块对外接口函数声明
//
//
/******************************************************************************************************************/

/***************************************************************************/
//     Description : 初始化小区广播接口函数(在手机初始化时调用)
//    Global resource dependence : none
//  Author:肖凡
//    Note:
/***************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPISMSCB_Init(MN_DUAL_SYS_E dual_sys);

//window related
/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenMainMenu(void);

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void);

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenSettingWin(void);

/*****************************************************************************/
//     Description : MMIAPISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_OpenCBList(void);

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : MMIAPISMSCB_GetETWSAlertFlag
//    Global resource dependence :
//  Author: Yongli Tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetETWSAlertFlag(void);

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_IsETWSAlert(void);

/*****************************************************************************/
//  Description : judge ETWS alert status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_IsCMASChannelAlert(void);

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetCallStatus
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetEMCallStatus(BOOLEAN is_emer_call);

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetEMCallStatus
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetEMCallStatus(void);

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedRedisplay
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsNeedRedisplay(void);

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsNeedRedisplay(BOOLEAN is_need_redisplay);

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsNeedEnable(void);

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsNeedEnable(BOOLEAN is_need_enable);

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsETWSDisplay
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsETWSDisplay(void);
#endif

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set SMS CB Status
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SwitchSmsCBStatus(BOOLEAN cb_enabled);
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_HandleCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleCBMsgInd(DPARAM param);       //收到小区广播信号处理

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_HandleMarcoCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleMarcoCBMsgInd(DPARAM param);       //收到小区广播信号处理

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleLteWarningMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleLteWarningMsgInd(DPARAM param);       //收到小区广播信号处理

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description : Get Unread SMS CB Number
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetUnreadSMSCBNum(void);

/*****************************************************************************/
//  Description : Get Unread SMS CB Channel
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBChannel(wchar *name_ptr, uint16 max_len);

/*****************************************************************************/
//  Description : Get Unread SMS CB Time
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBTime(wchar *time_ptr, uint16 max_len);
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetScrollItemNewSMSCB(void);
#endif
#endif
/*****************************************************************************/
//  Description : Get the push sms SL Operation
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SL_OPERATION_E MMIAPISMS_GetPushSmsSLOperation(void);

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ResetVMInfo(MN_DUAL_SYS_E dual_sys);
#endif

#ifdef MMS_SUPPORT
/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_ReadMMSNotify(void);
#endif

/*****************************************************************************/
//    Description : 
//    Global resource dependence :
//    Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetUndownLoadHeadOrder(void);

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Description : check me have enough space for save sms
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MEHaveEnoughSpace(void);

/*****************************************************************************/
//     Description : check current operation,
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsPush(void);

/*****************************************************************************/
//     Description : check current operation,
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetCurOperIsPush(BOOLEAN isPush);

/*****************************************************************************/
//     Description : Set current operation is connect.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetCurOperIsConnect(BOOLEAN isConnect);

/*****************************************************************************/
//     Description : check current operation is connect.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsConnect(void);

/*****************************************************************************/
//     Description : Set current operation is boot.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetCurOperIsBoot(BOOLEAN isBoot);

/*****************************************************************************/
//     Description : check current operation is boot.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsBoot(void);

/*****************************************************************************/
//     Description : Set current operation is disconnect.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetCurOperIsDisconnect(BOOLEAN isDisconnect);

/*****************************************************************************/
//     Description : check current operation is disconnect.
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurOperIsDisconnect(void);

/*****************************************************************************/
//     Description : MMIAPISMS_GetCurSMSIsRead
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetCurSMSIsRead(void);

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateOutbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateOutbox(void);

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateOutbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateOutbox(BOOLEAN isUpdate);

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateSendbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateSendbox(void);

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateOutbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateSendbox(BOOLEAN isUpdate);

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateInbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateInbox(void);

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateInbox
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateInbox(BOOLEAN isUpdate);

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsUpdateDraft
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsUpdateDraft(void);

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsUpdateDraft
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsUpdateDraft(BOOLEAN isUpdate);

/*****************************************************************************/
//     Description : MMIAPISMS_GetSendAddrCount
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPISMS_GetSendAddrCount(void);

/*****************************************************************************/
//     Description : MMIAPISMS_SetSendAddrCount
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetSendAddrCount(uint8 count);

/*****************************************************************************/
//     Description : MMIAPISMS_GetIsSendMultiSms
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsSendMultiSms(void);

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsSendMultiSms
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsSendMultiSms(BOOLEAN isSend);

/*****************************************************************************/
//     Description : MMIAPISMS_HandleMsgDeleteStatus

//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_HandleMsgDeleteStatus(void);

/*****************************************************************************/
//     Description : MMIAPISMS_HandleMsgReadStatus
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleMsgReadStatus(void);

/*****************************************************************************/
//  Description : MMIAPISMS_HandleSetMsgStatusAbort
//  Global resource dependence : none
//  Author: Naomi Huang
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_HandleSetMsgStatusAbort(BT_MAP_RESP_CODE_T resp_code);

/*****************************************************************************/
//     Description : delete all bt mails
//    Global resource dependence : 
//  Author: 
//    Note: 为sms模块提供的接口函数
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMapSyncInfo(void);

/*****************************************************************************/
//     Description : get box total order
//    Global resource dependence : 
//  Author: 
//    Note: 为mail模块提供的接口函数。
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetBoxTotalNumber();

/*****************************************************************************/
//  Description : Registers the MAP push client
//  Global resource dependence :                                
//  Author: Candice.Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MapRegisterClient(void);

/*****************************************************************************/
//  Description : Close MAP Waiting win
//  Global resource dependence : 
//  Author: Candice Chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CloseMapWaitWin(void);

/******************************************************************************/
// Description: MMISMS_SetMapAllowSync
// Global resource dependence: 
// Author: naomi huang
// Note: 
/******************************************************************************/
PUBLIC void MMISMS_SetMapAllowSync(BOOLEAN allow_sync);

/******************************************************************************/
// Description: MMISMS_GetMapAllowSync
// Global resource dependence: 
// Author: naomi huang
// Note: 
/******************************************************************************/
PUBLIC BOOLEAN MMISMS_GetMapAllowSync(void);

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_MapDisconnect(void);

/*****************************************************************************/
//  Description : MMIAPISMS_HandleServerMsgData
//  Global resource dependence : none
//  Author: Naomi Huang 
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_HandleServerMsgData(void* buff,uint16 buff_len);
/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
PUBLIC void MMIAPISMS_MapAbortClient(void);
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT) && defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_SYNC_TYPE_E MMIAPISMS_GetSMSSyncType(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_E type);
#endif

/*****************************************************************************/
// 	Description : is sc addr valid(only number & '+')
//	Global resource dependence : 
//  Author:louis.wei
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IsSCAddrValid(MMI_STRING_T *num_str_ptr);

/*****************************************************************************/
//     Description : to set the sc addr called by window
//    Global resource dependence : none
//  Author:louis.wei
//    Note:设置信息服务中心号码
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_AppSetSCAddr(                    //RETURN:
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
                                      MMISMS_SET_SIMMODE_T *set_mode_ptr,
#endif
                                      MN_DUAL_SYS_E   dual_sys,
                                      uint8  *sc_addr,    //IN:
                                      uint8  addr_len        //IN:
                                      );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/


/******************************#W317 Public #*********************************/
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//record the source of the preset list win
typedef enum
{
    MMISMS_PRESETFROM_NONE,
    MMISMS_PRESETFROM_CHAT,        
    MMISMS_PRESETFROM_NORMAL,
    MMISMS_PRESETFROM_CALL
}MMISMS_PRESETFROM_E;

typedef struct 
{
    wchar         preset_buff[255];  //preset  word buff
    uint16         preset_len;
}MMISMS_PRESET_BUFF_T;
/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Set Preset list win enter From
//  Parameter: [In] enum
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetPresetFrom(MMISMS_PRESETFROM_E s_presetfrom);

/*****************************************************************************/
//  Description : Get Preset list win enter From
//  Parameter: [In] None
//             [Out] enum
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_PRESETFROM_E MMISMS_GetPresetFrom();

/*****************************************************************************/
//  Description : save SMS contact info
//  Parameter: [In] phone number
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetNumberForSend(MMISMS_DEST_ADDR_INFO_T s_send_dest_list);

/*****************************************************************************/
//  Description : Get SMS contact info
//  Parameter: [In] None
//             [Out] contact info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_DEST_ADDR_INFO_T* MMISMS_GetNumberForSend();

/*****************************************************************************/
//  Description : Save send message info
//  Parameter: [In] message info buff&len
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetPresetForSend(wchar* preset_buff, uint16 preset_len);

/*****************************************************************************/
//  Description :Get send message info
//  Parameter: [In] None
//             [Out] message info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_PRESET_BUFF_T MMISMS_GetPresetForSend ();

/*****************************************************************************/
//  Description :Open note win
//  Parameter: [In] win_id
//             [In] text_id
//             [In] userFunc
//             [Out] NULL 
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenNoteWin(
                       MMI_WIN_ID_T            win_id,  
                       MMI_TEXT_ID_T           text_id,    
                       PROCESSMSG_FUNC    userFunc
                       );
/*****************************************************************************/
//  Description :Open full screen note win
//  Parameter: [In] win_id
//             [In] text_id
//             [In] image_id
//             [In] userFunc
//             [Out] message info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenFullScreenNoteWin( 
                       MMI_WIN_ID_T          win_id,
                       MMI_TEXT_ID_T         text_id,   
                       MMI_IMAGE_ID_T       image_id,  
                       PROCESSMSG_FUNC   userFunc
                       );
/*****************************************************************************/
//  Description :Open waiting win
//  Parameter: [In] win_id
//             [In] image_id
//             [In] image_id
//             [In] userFunc
//             [Out] message info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenFullScreenWaitingWin( 
                       MMI_WIN_ID_T       win_id,
                       MMI_TEXT_ID_T      text_id,  
                       MMI_IMAGE_ID_T    image_id,  
                       PROCESSMSG_FUNC userFunc
                       );
/*****************************************************************************/
//  Description :Open delete query win
//  Parameter: [In] win_id
//             [In] text_id
//             [In] left_image_id
//             [In] right_image_id
//             [In] userFunc
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenFullScreenQueryWin(
                       MMI_WIN_ID_T       win_id,
                       MMI_TEXT_ID_T      text_id,    
                       MMI_IMAGE_ID_T    left_image_id,  
                       MMI_IMAGE_ID_T    right_image_id, 
                       PROCESSMSG_FUNC userFunc);

/*****************************************************************************/
//  Description : OTA升级前，将SMS从NV保存到文件
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_OTASaveSMSToFile(void);

//PUBLIC BOOLEAN MMISMS_DeleteAllNvSMS(void);
/*****************************************************************************/
//  Description : OTA升级后，将SMS文件保存到NV
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_OTASaveFileToNv(void);

//PUBLIC BOOLEAN MMISMS_DeleteSMSFile(uint16 event_id);
/*****************************************************************************/
//  Description : OTA升级后，删除所有的短信文件
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMISMS_OTADelSMSFile(void);

/*****************************************************************************/
//     Description : MMISMS_GetEmojiItemNum
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetEmojiItemNum(void);

/*****************************************************************************/
//  Description: 注入函数
//  Parameter: [In] p_func   //外部模块向mmi sms注入的函数
//             [Out] none
//             [Return] BOOLEAN
//  Author: yang.wang3
//  Note:外部模块可调用此接口注入函数
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_Inject_Function(MMISMS_INJECTION_FUNC p_func);

/*****************************************************************************/
//  Description:取消注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] BOOLEAN
//  Author: yang.wang3
//  Note:外部模块可调用此接口取消注入函数
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_Uninject_Function(void);

/*****************************************************************************/
//  Description: get注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] injection_func    //注入函数
//  Author: yang.wang3
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_INJECTION_FUNC MMISMSAPP_Get_Injection_Func(void);

#ifdef __cplusplus
    }
#endif

#endif

