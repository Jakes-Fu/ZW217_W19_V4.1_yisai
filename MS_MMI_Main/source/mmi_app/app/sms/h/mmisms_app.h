/****************************************************************************
** File Name:      mmisms_app.h                                            *
** Author:                                                                 *
** Date:           04/11/2004                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the SMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME                DESCRIPTION                            *
** 04/2004        louis.wei         Create
** 08/2006          Tracy Zhang        Modify for 7550
** 
****************************************************************************/
#ifndef  _MMISMS_APP_H_    
#define  _MMISMS_APP_H_   
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mn_type.h"
#include "mmisms_nv.h"
#include "sci_types.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipb_export.h"
#include "mmisms_internal.h"
#include "mmi_signal_ext.h"
#include "mmicom_time.h"
#include "mmi_appmsg.h"
#include "mmivc_common.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmisms_timermsg.h"
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmifmm_export.h" 
#endif
#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
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
#define        MMISMS_MAX_DEF_BYTE_LEN        160   // the length of default normal SMS
#define        MMISMS_MAX_UCS2_BYTE_LEN    140   // the length of ucs2 normal SMS
#define     MMISMS_MAX_8BIT_BYTE_LEN    140   // the length of 8bit normal SMS
#define        MMISMS_MAX_DEF_SPLIT_LEN    153   // the length of default split-SMS   
#define        MMISMS_MAX_UCS2_SPLIT_LEN    134   // the length of UCS2 split-SMS
#define     MMISMS_MAX_8BIT_SPLIT_LEN   134   // the length of 8bit split-SMS   
#define     MMISMS_HEAD_8BIT_LENGTH               5   // the length of SMS head
#define     MMISMS_HEAD_16BIT_LENGTH               6   // the length of SMS head
#define     MMISMS_HEAD_8BIT_IEI             0x00  // the indicate of SMS 
#define     MMISMS_HEAD_8BIT_IEL             0x03  // the length of SMS indicate
#define     MMISMS_HEAD_IEI_POS         0     // the pos of SMS indicate
#define        MMISMS_HEAD_IEL_POS         1     // the pos of SMS indicate length
#define     MMISMS_HEAD_REF_NUM_POS      2     // the pos of reference number
#define     MMISMS_HEAD_8BIT_MAX_NUM_POS      3     // the pos of max number
#define     MMISMS_HEAD_8BIT_SEQ_NUM_POS        4     // the pos sequence number 
#define     MMISMS_HEAD_16BIT_MAX_NUM_POS        4
#define     MMISMS_HEAD_16BIT_SEQ_NUM_POS        5 
#define     MMISMS_MAX_MESSAGE_LEN      (MMISMS_SPLIT_MAX_NUM * MMISMS_DEFAULT_CHAR_LEN)
#define     MMISMS_MAX_UCS2_MESSAGE_LEN (MMISMS_SPLIT_MAX_NUM * MMISMS_MAX_UCS2_SPLIT_LEN)

#define     MMISMS_HEAD_LENGTH_FOR_PORT            6     // the length of SMS head
#define     MMISMS_HEAD_IEI_FOR_PORT             0x05  // the indicate of SMS 
#define     MMISMS_HEAD_IEL_FOR_PORT             0x04  // the length of SMS indicate
#define     MMISMS_HEAD_IEI_POS_FOR_PORT         0     // the pos of SMS indicate
#define        MMISMS_HEAD_IEL_POS_FOR_PORT         1     // the pos of SMS indicate length
#define     MMISMS_HEAD_SRC_PORTH_POS   2     // the pos of src port(high 8bit)
#define     MMISMS_HEAD_SRC_PORTL_POS      3     // the pos of srct port(low 8bit)
#define     MMISMS_HEAD_DST_PORTH_POS   4     // the pos of dest port(high 8bit)
#define     MMISMS_HEAD_DST_PORTL_POS      5     // the pos of dest port(low 8bit)
#define     MMISMS_VCARD_PORT_NUM       0x23F4
#define     MMISMS_VCAL_PORT_NUM       0x23F5
#define     MMISMS_SMART_MESSAGE_PORT_NUM       0x1581

//for SMS R8
#define     MMISMS_R8_LOCK_DEFAULT_CHAR_LEN        155   // the length of default lock R8 SMS


#define     MMISMS_R8_SINGLE_DEFAULT_CHAR_LEN     152   // the length of default single R8 SMS

#define     MMISMS_R8_MAX_LOCK_DEF_LEN          155
#define     MMISMS_R8_MAX_LOCK_SPLIT_LEN        149

    

#define     MMISMS_R8_MAX_SINGLE_DEF_LEN          152
#define     MMISMS_R8_MAX_SINGLE_SPLIT_LEN      146

#define     MMISMS_R8_LOCK_HEAD_LEN                 5       // the length of R8 with all 1 byte character
#define     MMISMS_R8_SINGLE_HEAD_LEN               8       // the length of R8 include 2 bytes character
#define     MMISMS_R8_HEAD_IEL                                    1  
#define     MMISMS_R8_MESSAGE_LEN       (MMISMS_SPLIT_MAX_NUM * MMISMS_R8_LOCK_DEFAULT_CHAR_LEN)    
#define     MMISMS_R8_HEAD_LEN                          3
#define     MMISMS_R8_MAX_NORMAL_HEAD_LEN       6

//other macro define
#define MMISMS_PBNAME_MAX_LEN					MMIPB_MAX_STRING_LEN
#define MMISMS_MAX_USERNUMBER_LEN                MMISMS_PBNUM_MAX_LEN
#define MMISMS_PBMAIL_MAX_LEN                           MMIPB_MAX_MAIL_LEN
#define MMISMS_MIN_USERNUMBER_LEN                3
#define MMISMS_VMNUM_MAX_LEN                    20  

#define MMISMS_INVAILD_INDEX                    0xFFFF    
#define MMISMS_INVAILD_LISTBUFF_POS                0xFF
#define MMISMS_STRNUM_MAX_LEN                    40
#define    MMISMS_PROMPT_INFO_LEN                    40
#define MMISMS_UPDATE_LISTBUFF_STATUS            0xFE

//#define  MMISMS_DAY_TIME                   (24*3600)
#define MMISMS_MINUTE_TIME                    60
        
#define MMISMS_RECIEVER_NUM_MAX             20
// the macro for order
// to indicate the flag of sort result in the SIM/NV
#define MMISMS_ORDERED_NO           0x00
#define MMISMS_ORDERED_SIM            0x02
#define MMISMS_ORDERED_ME            0x01
#define MMISMS_ORDERED_SMS            0x03
#define MMISMS_TYPE_FLAG_OFFSET     4
#define MMISMS_MO_FLAG              0x0F

//define the valid period
#define MMISMS_VALID_PERIOD_3600        3600
#define MMISMS_VALID_PERIOD_21600        21600
#define MMISMS_VALID_PERIOD_86400        86400
#define MMISMS_VALID_PERIOD_259200        259200
#define MMISMS_VALID_PERIOD_604800        604800
#define MMISMS_VALID_PERIOD_MAX            50000000
#define MMISMS_VALID_PERIOD_DEFAULT        50000000

//define the send mode
#define MMISMS_SEND_MODE_DEFAULT    0
#define MMISMS_SEND_MODE_VOCAL      1
#define MMISMS_SEND_MODE_PAGER      2
#define MMISMS_SEND_MODE_EMAIL      3
#define MMISMS_SEND_MODE_ERMES      4
#define MMISMS_SEND_MODE_X400       5
#define MMISMS_SEND_MODE_FAX        6

#define MMISMS_AUTOSAVE_FLAG_DEFAULT        MMINV_SMS_SETTING_SAVE

#ifdef ORANGE_SUPPORT
#define MMISMS_STATUSREPORT_FLAG_DEFAULT    TRUE
#else
#define MMISMS_STATUSREPORT_FLAG_DEFAULT    FALSE
#endif

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
#define    MMISMS_AUTO_SIGNATURE_FLAG_DEFAULT    FALSE
#endif

#define MMISMS_REPLY_PATH_FLAG_DEFAULT    FALSE

#define MMISMS_BROWSERINFO_FLAG_DEFAULT    TRUE
#define MMISMS_ISEXTRACT_FLAG_DEFAULT    TRUE

#define MMISMS_IS_CONCATENATED_SMS_DEFAULT    TRUE


#define MMISMS_ALPHABET_MANAGEMENT_DEFAULT MMISMS_ALPHABET_MANAGEMENT_AUTO

#define MMISMS_SL_OPERATION_DEFAULT MMISMS_SL_OPERATION_PROMPT

#define        MMISMS_3SECOND        3000

#define        MMISMS_OFFSERT_YEAR    MMICOM_DATE_MIN_YEAR
#define        MMISMS_MT_OFFSERT_YEAR    2000

#define        DEFAULT_EXT_MARK    0x1b

#define     MMISMS_UNVALID_ORDER_ID 0xffff

// max resend count
#ifdef MMI_DUALMODE_ENABLE
#define MMISMS_RESEND_MAX_COUNT            3    // 1 // �Զ��ط�����
#else
#define MMISMS_RESEND_MAX_COUNT            0 // �Զ��ط�����
#endif

// max send path resend count
#define MMISMS_SEND_PATH_RESEND_COUNT            1    // ������ѡ������·���ʧ�ܵ��ط�����

#define    MMISMS_DEFAULT_WORD_NUM            5
#define MMISMS_VALID_DATA_LEN_DIV        2
#define MMISMS_NV_DEFAULT_REFNUM        0
// the char macro
#define MMISMS_NUMBER_0                    '0'
#define MMISMS_NUMBER_9                    '9'
#define MMISMS_NUMBER_PLUS              '+'

#define HANDLE_LEN 17

// translate the struct from MMI_PARTY_NUM_T to MN_CALL_NUM_T
#define MMIPARTYNUM_2_MNCALLEDNUM( _MMI_PARTY_NUM, _MN_CALL_NUM_PTR )                            \
        {                                                                                        \
            _MN_CALL_NUM_PTR->number_type = _MMI_PARTY_NUM->number_type;                            \
            _MN_CALL_NUM_PTR->num_len      = MIN( _MMI_PARTY_NUM->num_len, MN_MAX_ADDR_BCD_LEN );    \
            SCI_MEMCPY(                                                                            \
                        (void*)_MN_CALL_NUM_PTR->party_num,                                        \
                        (void*)_MMI_PARTY_NUM->bcd_num ,                                            \
                        _MN_CALL_NUM_PTR->num_len                                                \
                        );                                                                        \
        }

// get the sequence number form the message head
#define     GET_CURRENT_REF_NUM(_edit_content_ptr) \
            Get_Current_Ref_Num(_edit_content_ptr)
//tpdu_head.user_header_arr[MMISMS_HEAD_REF_NUM_POS]


// get the sequence number form the message head
#define     GET_CURRENT_SEQ_NUM(_edit_content_ptr) \
            Get_Current_Seq_Num(_edit_content_ptr)    
//tpdu_head.user_header_arr[MMISMS_HEAD_SEQ_NUM_POS]

// set the sequence number 
#define     SET_CURRENT_SEQ_NUM( _head_ptr, _seq_num )\
            Set_Current_Seq_Num(_head_ptr, _seq_num)

//tpdu_head.user_header_arr[MMISMS_HEAD_SEQ_NUM_POS] = _SEQ_NUM

// get the max number form the message head
#define     GET_HEAD_MAX_NUM(_send_info_ptr)\
            Get_Head_Max_Num(_send_info_ptr)

//tpdu_head.user_header_arr[MMISMS_HEAD_MAX_NUM_POS]


// to set the user data head
#define SET_HEAD_VAR( _head_ptr, _ref_num, _max_num, _seq_num )                        \
        Set_Longsms_Head_Var(_head_ptr, _ref_num, _max_num, _seq_num)

#define SET_HEAD_R8_VAR( _head_ptr, _language, _single_shift_flag )               \
        Set_SmsR8_Head_Var(_head_ptr, _language, _single_shift_flag)

/*
        {                                                                            \
            _head_ptr->length                                  = MMISMS_HEAD_LENGTH;    \
            _head_ptr->user_header_arr[ MMISMS_HEAD_IEI_POS ] = MMISMS_HEAD_IEI;    \
            _head_ptr->user_header_arr[ MMISMS_HEAD_IEL_POS ] = MMISMS_HEAD_IEL;    \
            _head_ptr->user_header_arr[ MMISMS_HEAD_REF_NUM_POS ] = _ref_num;        \
            _head_ptr->user_header_arr[ MMISMS_HEAD_MAX_NUM_POS ] = _max_num;        \
            _head_ptr->user_header_arr[ MMISMS_HEAD_SEQ_NUM_POS ] = _seq_num;        \
        }
*/

#define MMISMS_STORAGE_NUM          2

#define MMISMS_CLEAR_OPER(_ret_val)                \
    if (MMISMS_NO_ERR != _ret_val )                \
    {                                            \
        MMISMS_SetOperStatus(MMISMS_NO_OPER);    \
    }

#define MMISMS_RETRY_WAIT_TIMER     5000 

#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef LOW_MEMORY_SUPPORT
#define MMISMS_CHAT_MAX_ITEM 50
#else
#define MMISMS_CHAT_MAX_ITEM 100
#endif
#endif

#define MMISMS_SET_SIMMODE_NAME_LEN 16

#define  MMISMS_SENDLIST_MAX_NUM 20
#define  MMISMS_SENDSMS_SINGLE_TIME 60*1000

#define MMISMS_CHAT_SENDFAIL_STR_MAX_LEN 40
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef unsigned UTF16;
#define UNICODE_IS_SURROGATE(ch)  (0xD800 <= (ch) && (ch) <= 0xDFFF)
#define UNICODE_IS_HIGH_SURROGATE(ch) (0xD800 <=(ch) && (ch) <= 0xDBFF)
#define UNICODE_IS_LOW_SURROGATE(ch)  (0xDC00 <=(ch) && (ch) <= 0xDFFF)
#define UNICODE_TO_HIGH_SURROGATE(ch)  (0xD800 - (0x10000 >> 10) + ((ch) >> 10))
#define UNICODE_TO_LOW_SURROGATE(ch)  (0xDC00 + ((ch) & 0x3FF))
#define DECODE_SURROGATE_PAIR(hi,lo) ((hi - 0xD800) * 0x400) + (lo - 0xDC00) + 0x10000

typedef enum
{
    MMISMS_NO_ERR,              // API run successfully, no error 
    MMISMS_NO_SC_ADDRESS,       // API run failure, user must write default SC_Address or input SC_Address while send message
    MMISMS_FDN_SCA_FAILED,//FDN FAILED
    MMISMS_FDN_DEST_NUM_FAILED,
    MMISMS_NO_DEST_ADDRESS,     // API run failure, user must input default destination_address or input destination address while send message
    MMISMS_EMPTY_BOX,
    MMISMS_DEL_FAIL,
    MMISMS_NO_ENOUGH_SPACE, 
    MMISMS_NO_SPACE,
    MMISMS_SAVE_FAIL,
    MMISMS_FINISH_OPER,
    MMISMS_FINISH_OPER_ERR,
    MMISMS_SYSTEM_BUSY,
    MMISMS_CALLING_BUSY,
    MMISMS_SENDSMS_FAIL,
    MMISMS_SENDLIST_FULL,
    MMISMS_SMS_READ_FAIL,
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_BT_ERROR,
#endif
    MMISMS_MAX_ERR
} MMISMS_OPER_ERR_E;

typedef enum
{
    MMISMS_DATACHANGE_TYPE_ADD,   //save������������������,��copy,move
    MMISMS_DATACHANGE_TYPE_SECURITY_ADD,  
    MMISMS_DATACHANGE_TYPE_NEW_MT,
    MMISMS_DATACHANGE_TYPE_DELETE, //delete���������ɾ��һ������
    MMISMS_DATACHANGE_TYPE_UPDATE,//update�������������һ����Ϣ
    MMISMS_DATACHANGE_TYPE_MAX,    
}MMISMS_DATACHANGE_TYPE_E;

typedef enum 
{
    MMISMS_NO_OPER,                    // no operation
    MMISMS_ORDER_MTSMS,                // order NV MT SMS
    MMISMS_ORDER_MOSMS,                // sort NV MO SMS
    MMISMS_DELETE_OTHER,            // delete SMS for the other module
    MMISMS_DELETE_MSG,                // delete SMS operation
    MMISMS_UPDATE_SMS,                // update SMS operation
    MMISMS_UPDATE_OTHER,            // update the SMS in accord the result of IVR
    MMISMS_SEND_OTHER,
    MMISMS_READ_SMS,                // read SMS operation
    MMISMS_ORDER_SMS,               //SMS Order
    MMISMS_SAVE_SMS,                // only save SMS operation
    MMISMS_SEND_SMS,                // only send SMS operation
    MMISMS_SAVE_SENDFAIL_SMS,        // save the SMS which send fail
    MMISMS_SAVE_SENDSUCC_SMS,        // save the SMS which send succ
    MMISMS_SEND_SAVE_SMS,            // send and save SMS operation
    MMISMS_SAVE_SEND_SMS,            // save the leave SMS then send SMS to other destination
    MMISMS_UPDATE_BACKSEND_SMS,
    MMISMS_UPDATE_SENDFAIL_SMS,
    MMISMS_UPDATE_NOSEND_SMS,        // update the no send SMS
    MMISMS_SAVE_CLASS0_SMS,
    MMISMS_DELETE_STATUSREPORT_SMS,
    MMISMS_MOVE_SMS_TO_SECURITY,
    MMISMS_MOVE_SMS_FROM_SECURITY,
    MMISMS_MOVE_SMS_TO_SIM,
    MMISMS_MOVE_SMS_TO_ME,
    MMISMS_COPY_SMS_TO_SIM,
    MMISMS_COPY_SMS_TO_ME,
    MMISMS_ASYNC_SEARCH_SMS,
    MMISMS_READ_MATCH_SMS,
#if defined(MMI_SMS_EXPORT_SUPPORT)
    MMISMS_EXPORT_SMS,
#endif
    MMISMS_READ_FOR_TIMERMSG,
    MMISMS_DELETE_SENDFAIL_SMS,
#ifdef MMI_SMS_CHAT_SUPPORT   
    MMISMS_READ_CHAT_NUM_SMS,
    MMISMS_UPDATE_EXCEED_CHAT_SMS,        
#endif
#ifdef IKEYBACKUP_SUPPORT   
    MMISMS_BACKUP_SMS,
    MMISMS_RESTORE_SMS,
#endif
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    MMISMS_READ_SMS_NOT_SHOW,
#endif
    MMISMS_SAVE_VCARD_VCAL_SMS,
#ifdef MMI_SMS_SYN_SUPPORT
	 MMISMS_SAVE_SYN_SMS,
#endif    
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
	 MMISMS_BT_SAVE_MT_SMS,
	 MMISMS_BT_SAVE_SENDSUCC_SMS,
	 MMISMS_BT_SAVE_SENDFAIL_SMS,
	 MMISMS_BT_SAVE_NOSEND_SMS,
     MMISMS_BT_SEND_SAVE_SMS,
#endif
    MMISMS_MATCH_DATA_DELETE,
    MMISMS_READ_RESEND_SMS
}MMISMS_OPER_STATUS_E;

typedef enum
{
    MMISMS_NO_TYPE,
    MMISMS_MO_TYPE,        // MO type message
    MMISMS_MT_TYPE        // MT type message    
} MMISMS_TYPE_E;

typedef enum
{
    MMISMS_READ_NONE,
    MMISMS_READ_MOSMS,                // READ MO SMS
    MMISMS_READ_MTSMS,                // READ MT SMS    
    MMISMS_READ_STATUSREPORT        // READ STATUS REPORT
} MMISMS_READ_TYPE_E;

typedef enum
{
    MMISMS_ORDER_BY_TIME    = 0,
    MMISMS_ORDER_BY_NAME    = 1,
    MMISMS_ORDER_BY_CONTENT = 2,
    MMISMS_ORDER_BY_TYPE    = 3,
    MMISMS_ORDER_TYPE_MAX
} MMISMS_ORDERTYPE_E;

typedef enum
{
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_PREFERRED_PATH_GPRS_PREFERRED,
    MMISMS_PREFERRED_PATH_GSM_ONLY,
    MMISMS_PREFERRED_PATH_GPRS_ONLY,
    MMISMS_PREFERRED_PATH_MAX
} MMISMS_PREFERRED_PATH_E;//SMS ������ѡ������

typedef enum
{
    MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET,
    MMISMS_ALPHABET_MANAGEMENT_8BIT,
    MMISMS_ALPHABET_MANAGEMENT_UCS2,
    MMISMS_ALPHABET_MANAGEMENT_AUTO,
    MMISMS_ALPHABET_MANAGEMENT_MAX
} MMISMS_ALPHABET_MANAGEMENT_E;

typedef enum
{
    MMISMS_READ_MSG,
    MMISMS_OPEN_POPMENU,
    MMISMS_POPMENU_FORWARD
}MMI_SMS_OPERATION_T;

typedef enum
{
    MMISMS_FONT_BIG    = 0,
    MMISMS_FONT_NORMAL    = 1,
    MMISMS_FONT_SMALL = 2,
    MMISMS_FONT_MAX
} MMISMS_FONT_E;

typedef enum
{
    MMISMS_DELETE_SINGLE    = 0,
    MMISMS_DELETE_PART    = 1,
    MMISMS_DELETE_ALL = 2,
    MMISMS_DELETE_MAX
} MMISMS_DELETE_TYPE_E;


typedef enum
{
    MMISMS_MAIN_MENU_NODE_NULL,
#ifdef MMI_SMS_CHAT_SUPPORT        
    MMISMS_MAIN_MENU_NODE_CHAT,
#endif    
    MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG, 
    MMISMS_MAIN_MENU_NODE_INBOX,
    MMISMS_MAIN_MENU_NODE_OUTBOX,
    MMISMS_MAIN_MENU_NODE_DRAFTBOX,
    MMISMS_MAIN_MENU_NODE_SENTBOX,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    MMISMS_MAIN_MENU_NODE_SECURITYBOX,  
#endif    
    MMISMS_MAIN_MENU_NODE_FREQWORD,
#if defined(MMI_SMSCB_SUPPORT)
    MMISMS_MAIN_MENU_NODE_BROADCAST,    //С���㲥�˵���
#endif
    MMISMS_MAIN_MENU_NODE_VOICEMAIL,    //��������
    MMISMS_MAIN_MENU_NODE_SETTING,
    MMISMS_MAIN_MENU_NODE_MEMSTATE,
#ifdef MMI_SMS_SIMBOX    
    MMISMS_MAIN_MENU_NODE_SIMBOX,    //SIMBOX
#endif    
    MMISMS_MAIN_MENU_NODE_MMS_SEARCH,
    MMISMS_MAIN_MENU_NODE_MAX
}MMISMS_MAIN_MENU_NODE_E;

typedef enum
{
    //������ѡ���
    MMISMS_SEARCH_MENU_CALL_ITEM_ID,
    MMISMS_SEARCH_MENU_ANSWER_ITEM_ID,
    MMISMS_SEARCH_MENU_FORWARD_ITEM_ID,
    MMISMS_SEARCH_MENU_DELBOX_ITEM_ID,
    MMISMS_SEARCH_MENU_SEARCH_OPEN_ID,
    MMISMS_SEARCH_MENU_ANSWER_SMS_ITEM_ID,
    MMISMS_SEARCH_MENU_ANSWER_MMS_ITEM_ID,
    MMISMS_SEARCH_MENU_AUDIO_CALL_ITEM_ID,
#ifdef VT_SUPPORT
    MMISMS_SEARCH_MENU_VIDEO_CALL_ITEM_ID,
#endif
    MMISMS_SEARCH_MENU_IPCALL_ITEM_ID,
    
}MMISMS_SEARCH_OPTION_MENU_NODE_E;

#ifdef MMI_SMS_CHAT_SUPPORT
typedef enum
{
    MMISMS_CHAT_INDEX_NEW,
    MMISMS_CHAT_INDEX_SAME_LONG_SMS,
    MMISMS_CHAT_INDEX_SAME_SMS,
    MMISMS_CHAT_INDEX_MAX
} MMISMS_CHAT_INDEX_E;
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
typedef enum
{
    MMISMS_MAP_EVENT_NEW_MESSAGE,
    MMISMS_MAP_EVENT_DELIVERY_SUCCESS,
    MMISMS_MAP_EVENT_SENDING_SUCCESS,
    MMISMS_MAP_EVENT_DELIVERY_FAILURE,
    MMISMS_MAP_EVENT_SENDING_FAILURE,
    MMISMS_MAP_EVENT_MEMORY_FULL,
    MMISMS_MAP_EVENT_MEMORY_AVAILABLE,
    MMISMS_MAP_EVENT_MESSAGE_DELETED,
    MMISMS_MAP_EVENT_MESSAGE_SHIFT,
    MMISMS_MAP_EVENT_MAX
} MMISMS_MAP_EVENT_E;
#endif

typedef struct MMISMS_LINKED_ORDER_UNIT_T_tag
{
    struct MMISMS_LINKED_ORDER_UNIT_T_tag     *next_ptr;        //��һ����Ԫ��ָ��
    MMISMS_STATE_T                            sms_state;        //����Ԫ�ľ�������
}MMISMS_LINKED_ORDER_UNIT_T;

typedef struct
{
    uint16 msg_num[MMI_DUAL_SYS_MAX];          //simX ��Ϣ��
    uint16 ori_msg_num[MMI_DUAL_SYS_MAX];          //simX ��Ϣ��(δƴ��)
    uint16 ori_sim_sms_num[MMI_DUAL_SYS_MAX];          //simX SMS��(δƴ��)
    uint16 total_msg_num;          //sim1��sim2��Ϣ��
}MMISMS_MSG_NUM_T;

typedef struct
{
    uint32                                unit_num;                //����Ԫ����Ŀ
    MMISMS_LINKED_ORDER_UNIT_T             *head_ptr;                //����Ԫ���׵�ַ
    MMISMS_LINKED_ORDER_UNIT_T            *unused_unit_head_ptr;    //δ������Ԫ�׵�ַ
    MMISMS_LINKED_ORDER_UNIT_T            *used_unit_head_ptr;    //��������Ԫ�׵�ַ
}MMISMS_LINKED_ORDER_T;

//#ifdef MMI_SMS_CHAT_SUPPORT
typedef struct MMISMS_CHAT_STATE_UNIT_T_tag
{
    BOOLEAN is_valid;   //�ڵ��Ƿ���Ч
	uint32  total_num;  //�ú����������Ϣ�����������ż����һ��
	uint32  unread_num; //�ú���δ����Ϣ����
	uint32  time;       //���һ����Ϣ��ʱ��
	MMISMS_BCD_NUMBER_T bcd_num;
    uint8   sender[MMISMS_NUMBER_MAX_LEN+1];//added for mms...
	MMISMS_STATE_T  *head_ptr;  //������ĵ�һ����Ϣ
	struct MMISMS_CHAT_STATE_UNIT_T_tag  *next_number_ptr;  //��һ������ĵ�һ����Ϣ
	struct MMISMS_CHAT_STATE_UNIT_T_tag  *prev_number_ptr;  //ǰһ������ĵ�һ����Ϣ
}MMISMS_CHAT_STATE_UNIT_T;

typedef struct
{
    uint32  uint_num;  //�������
    uint32  unread_unit_num;    //δ���Ự�������
    MMISMS_CHAT_STATE_UNIT_T  *head_ptr;  //�б��е�һ����¼�����ʱ��������¼
    MMISMS_CHAT_STATE_UNIT_T  *head_node_ptr;   //��һ���ڵ�
}MMISMS_CHAT_UNIT_T;
//#endif

typedef struct 
{
    MMISMS_MSG_NUM_T max_sms_num;                //sms�������    
    MMISMS_MSG_NUM_T total_undownloaded_num;//����δ���ص�mms������
    MMISMS_MSG_NUM_T total_to_be_read_msg_num;    //δ����Ϣ��Ŀ(����δ��sms��mms��ota��push...)
    MMISMS_MSG_NUM_T to_be_read_msg_num[MMISMS_TYPE_MAX];    //δ����Ϣ��Ŀ
    MMISMS_MSG_NUM_T total_mt_readed_num;        //�����Ѷ�MT number
    MMISMS_MSG_NUM_T total_mt_num;                  //MT SMS����
    MMISMS_MSG_NUM_T total_mo_num;                //����MO sms����
    MMISMS_MSG_NUM_T total_sendsucc_num;            //���ͳɹ�sms����
    MMISMS_MSG_NUM_T total_sendfail_num;            //����ʧ��sms����
    MMISMS_MSG_NUM_T total_nosend_num;           //�ݸ���sms����
    MMISMS_STATE_T *mt_undownloaded_head_ptr;  //δ��������    
    MMISMS_STATE_T *mt_unread_head_ptr;        //δ������
    MMISMS_STATE_T *mt_readed_head_ptr;        //�Ѷ�����
    MMISMS_STATE_T *mo_sendsucc_head_ptr;        //���ͳɹ�����
    MMISMS_STATE_T *mo_sendfail_head_ptr;        //����ʧ������
    MMISMS_STATE_T *mo_draft_head_ptr;            //�ݸ�������
    MMISMS_STATE_T *not_handled_head_ptr;        //δ��������

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_MSG_NUM_T bt_total_undownloaded_num;//����δ���ص�mms������
    MMISMS_MSG_NUM_T bt_total_to_be_read_msg_num;    //δ����Ϣ��Ŀ(����δ��sms��mms��ota��push...)
    MMISMS_MSG_NUM_T bt_to_be_read_msg_num[MMISMS_TYPE_MAX];    //δ����Ϣ��Ŀ
    MMISMS_MSG_NUM_T bt_total_mt_readed_num;        //�����Ѷ�MT number
    MMISMS_MSG_NUM_T bt_total_mt_num;                  //MT SMS����
    MMISMS_MSG_NUM_T bt_total_mo_num;                //����MO sms����
    MMISMS_MSG_NUM_T bt_total_sendsucc_num;            //���ͳɹ�sms����
    MMISMS_MSG_NUM_T bt_total_sendfail_num;            //����ʧ��sms����
    MMISMS_MSG_NUM_T bt_total_nosend_num;           //�ݸ���sms����
	
    MMISMS_STATE_T *bt_mt_undownloaded_head_ptr;  //δ��������    
    MMISMS_STATE_T *bt_mt_unread_head_ptr;        //δ������
    MMISMS_STATE_T *bt_mt_readed_head_ptr;        //�Ѷ�����
    MMISMS_STATE_T *bt_mo_sendsucc_head_ptr;        //���ͳɹ�����
    MMISMS_STATE_T *bt_mo_sendfail_head_ptr;        //����ʧ������
    MMISMS_STATE_T *bt_mo_draft_head_ptr;            //�ݸ�������
#endif

    MMISMS_MSG_NUM_T security_total_undownloaded_num;//����δ���ص�mms������
    MMISMS_MSG_NUM_T security_total_to_be_read_msg_num;    //δ����Ϣ��Ŀ(����δ��sms��mms��ota��push...)
    MMISMS_MSG_NUM_T security_to_be_read_msg_num[MMISMS_TYPE_MAX];    //δ����Ϣ��Ŀ
    MMISMS_MSG_NUM_T security_total_mt_readed_num;        //�����Ѷ�MT number
    MMISMS_MSG_NUM_T security_total_mt_num;                  //MT SMS����
    MMISMS_MSG_NUM_T security_total_mo_num;                //����MO sms����
    MMISMS_MSG_NUM_T security_total_sendsucc_num;            //���ͳɹ�sms����
    MMISMS_MSG_NUM_T security_total_sendfail_num;            //����ʧ��sms����
    MMISMS_MSG_NUM_T security_total_nosend_num;           //��ȫ������sms����
    MMISMS_STATE_T *security_mt_undownloaded_head_ptr;  //δ��������    
    MMISMS_STATE_T *security_mt_unread_head_ptr;        //δ������
    MMISMS_STATE_T *security_head_ptr;            //��ȫ����������    
}MMISMS_ORDER_RESULT_T;

typedef struct _APP_SMS_USER_DATA_T
{
    MN_SMS_USER_DATA_HEAD_T  user_data_head_t;     // the head structure
    MN_SMS_USER_VALID_DATA_T user_valid_data_t;    // the used valid data structure
} APP_SMS_USER_DATA_T;

typedef struct 
{
    MMISMS_READ_TYPE_E        read_type;        // the message type(MO or MT)
    MN_SMS_USER_DATA_HEAD_T    head_data;        // head of SMS
    MN_SMS_TIME_STAMP_T        time_stamp;        // 
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    MN_CALLED_NUMBER_T        sc_addr_t;        //if reply path is TRUE,use sc_addr from MT SMS
#endif
    MN_CALLED_NUMBER_T        address;
    uint8                    sms_num;
    MMISMS_CONTENT_T        read_content;    // the data of MO and MT 
    MN_DUAL_SYS_E           dual_sys;
    MMISMS_R8_LANGUAGE_E  language; 
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    BOOLEAN reply_path_is_exist;
#endif
    BOOLEAN is_lock_shift;
    BOOLEAN is_single_shift;
    APP_SMS_USER_DATA_T ascii_vaild;
    MMISMS_FOLDER_TYPE_E folder_type;
    MN_SMS_STATUS_E    status; 
    MMISMS_OPER_STATUS_E     oper_status; 
    BOOLEAN is_locked;
    uint32 time;
    MMISMS_MO_MT_TYPE_E mo_mt_type;
    BOOLEAN  class_is_present;  // whether use specified class
    MN_SMS_CLASS_E      sms_class;         // the SMS class type
}MMISMS_READ_MSG_T;

typedef struct 
{
    BOOLEAN       is_get_set_vm_addr_cnf;   //��sim���������������ʱ���Ƿ񷵻���Ϣ
    BOOLEAN       is_set_vm_addr_success;   //��sim���������������ʱ���Ƿ�ɹ�            
}MMISMS_SET_VM_ADDR_STATUS_T;

//the status of voice message info 
typedef struct
{
    BOOLEAN  is_line1_new_vm;  //��·1�Ƿ����µ�������Ϣ
    BOOLEAN  is_line2_new_vm;  //��·2�Ƿ����µ�������Ϣ
}MMISMS_VM_INFO_T;

typedef struct
{
    MMISMS_OPER_STATUS_E     oper_status;    // the status of operation
    MMISMS_BOX_TYPE_E         cur_type;        // the type of current SMS type
    MMISMS_ORDER_ID_T         cur_order_id_arr[MMISMS_SPLIT_MAX_NUM];// the pos in the g_mmisms_order
    uint8                   cur_order_index;
}MMISMS_OPER_T;

typedef struct
{
    MN_DUAL_SYS_E       dual_sys;
    MN_SMS_STORAGE_E    storage;     // the storage for long sms 
    uint16               ref_num;     // the reference number
    uint8               max_num;     // the max number
    uint8               seq_num;     // the sequence number
    MMISMS_BCD_NUMBER_T number;      // add for display the number
    MMISMS_FOLDER_TYPE_E folder_type;
    BOOLEAN is_locked;
    BOOLEAN is_change_save_pos;
    uint32 time;    
}MMISMS_SAVE_INFO_T;


typedef struct
{
    MMINV_SMS_REFERENCE_NUM_T   ref_num;
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT    
    MMINV_SMS_AUTOSAVE_T        auto_save_setting;
#endif
    BOOLEAN                        status_report_flag;
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    BOOLEAN                     reply_path_flag;
#endif
    MMINV_SMS_VALID_PERIOD_T    vaild_period;
    MMISMS_PREFERRED_PATH_E   preferred_connection;
    MMISMS_SL_OPERATION_E   browser_info_sl_operation;    
    BOOLEAN                        browser_info_flag;
    BOOLEAN                        is_extract_flag;
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
    BOOLEAN                        is_concatenated_sms;
#endif
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    MMISMS_ALPHABET_MANAGEMENT_E alpha_management;
#endif
    MMISMS_FOLDER_TYPE_E   folder_type[MMINV_MAX_SMS_RECORDS];    
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    uint8                       sim_mode[MMI_DUAL_SYS_MAX];
#endif
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    /*auto signature*/
    BOOLEAN                        auto_signature_flag;
#endif
}MMISMS_NV_PARAM_T;

typedef struct _MMI_SMS_VMN_T 
{
    uint8                  record_id;
    MMI_PARTY_NUMBER_T     line1_vmn_info;
#ifdef MMI_SMS_VOICE_MAIL_DISPLAY_NAME_SUPPORT
    uint8   alpha_len;
    uint8   alpha_id[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN];    //the alpha id of the dialling number
#endif
    MMI_PARTY_NUMBER_T     line2_vmn_info;
    MMI_PARTY_NUMBER_T     fax_vmn_info;
    MMI_PARTY_NUMBER_T     data_vmn_info;
} MMI_SMS_VMN_T;

typedef enum
{
    MMISMS_LINE1_VMN_RECORD_ID = 1,  //����ps��ֵ������mmi��Ҫ�޸ġ�
    MMISMS_LINE2_VMN_RECORD_ID = 2,
    MMISMS_FAX_VMN_RECORD_ID = 3,
    MMISMS_DATA_VMN_RECORD_ID = 4,
    MMISMS_VMN_TYPE_MAX        
} MMISMS_VMN_RECORD_ID_E;

typedef struct 
{
    uint8    total_num;
    uint8    current_num;
    uint8    update_num;
    BOOLEAN is_sendsucc;
    MMISMS_ORDER_ID_T    order_id_arr[MMISMS_SPLIT_MAX_NUM];
}MMISMS_UPDATE_INFO_T;

typedef struct
{
    MN_DUAL_SYS_E dual_sys;
    uint16 mode;
    BOOLEAN alpha_id_present;    // whether the alpha id exist
    MN_SMS_SUPPORT_LANG_T    alpha_id_t;    // the alpha id struct
    BOOLEAN tp_sc_addr_present;    // whether the sc addrress exist 
    MN_CALLED_NUMBER_T tp_sc_addr_t;    // the sc addrress struct
    BOOLEAN tp_pid_present;    // if the protocol id exist
    MN_SMS_PID_E tp_pid;    // the protocol id value
    BOOLEAN tp_vp_present;    // whether validity period exist
    MN_SMS_TIME_SECOND_T vp_second;     // the validity value express by second
}MMISMS_SET_SIMMODE_T;

typedef struct
{
    wchar  search_str[MMISMS_SEARCH_MAX_LEN + 1];
    uint16  search_len;
    MMISMS_LINKED_ORDER_UNIT_T     *search_ptr; 
    MMISMS_ORDER_ID_T next_long_order_id;
}MMISMS_SEARCH_T;

#ifdef MMI_SMS_CHAT_SUPPORT
typedef struct
{
    BOOLEAN is_valid;
    MMISMS_ORDER_ID_T cur_order_id;
    MMI_CTRL_ID_T parent_ctrl_id;
    BOOLEAN split_sms_enable[MMISMS_SPLIT_MAX_NUM];
    uint16 split_sms_length[MMISMS_SPLIT_MAX_NUM];
}MMISMS_CHAT_ITEM_T;
#endif

typedef enum
{
    MMISMS_WAIT_RESULT,
    MMISMS_SEND_SUCC,
    MMISMS_SEND_FAIL,
    MMISMS_SEND_CANCEL
}MMISMS_SEND_RESULT_E;

typedef enum
{
    MMISMS_SENDSAVE_NONE,
    MMISMS_SENDSAVE_SAVEFAIL_NOSPACE,
    MMISMS_SENDSAVE_SAVEFAIL,
    MMISMS_SENDSAVE_SAVESUCC,
}MMISMS_SENDSAVE_RESULT_E;

typedef enum
{
    MMISMS_NO_SEND_STATUS,
    MMISMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT,
    MMISMS_WAIT_FOR_WRITE_CNF_TO_RESEND,
    MMISMS_LONG_SMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT,
    MMISMS_NO_RESEND_WAIT_FOR_WRITE_CNF_TO_SEND_NEXT,
    MMISMS_LONG_SMS_WAIT_FOR_WRITE_CNF_TO_SEND_NEXTPART
}MMISMS_SEND_STATUS_E;

typedef enum
{
    MMISMS_SENDLIST_NONE,
    MMISMS_SENDLIST_SAVING,
    MMISMS_SENDLIST_SENDBEGIN,
    MMISMS_SENDLIST_SENDING,
    MMISMS_SENDLIST_SENDCNF,
    MMISMS_SENDLIST_WRITECNF,
    MMISMS_SENDLIST_SENDEND,
    MMISMS_SENDLIST_MAX
}MMISMS_SENDLIST_STATUS_E;

typedef struct 
{
    MN_DUAL_SYS_E   dual_sys;
    BOOLEAN                    is_finished;    // TRUE: the two procedure are finished.
    MMISMS_SEND_RESULT_E    send_result;
    MMISMS_SEND_STATUS_E    send_status;    
    MN_SMS_STORAGE_E        save_storage;
    MN_SMS_RECORD_ID_T        save_record_id;    
} MMISMS_SEND_SAVE_RESULT_T;

//�������ڷ��͵��������ŵ�order��Ϣ�����ڷ�����ɺ�ɾ��
typedef struct 
{
    BOOLEAN  is_backsend_update;

    MN_SMS_RECORD_ID_T save_send_record_id[MMISMS_SPLIT_MAX_NUM];    //��̨�����ȱ���Ķ��ŵ�id
    MN_SMS_STORAGE_E save_send_storage;                              //��̨�����ȱ���Ķ��ŵ�λ��
    MMISMS_SENDSAVE_RESULT_E save_send_result;                       //��̨�����ȱ���Ķ��ű�����
    
    MN_SMS_STORAGE_E  backsend_updata_storage;                       //��̨�����ȱ���Ķ��ŵ�ǰɾ�����������ŵ�λ��
    MN_SMS_RECORD_ID_T backsend_record_id;                           //��̨�����ȱ���Ķ��ŵ�ǰɾ�����������ŵ�id
    MMISMS_OPER_STATUS_E backsend_send_oper;                         //��̨�����ȱ���Ķ��ŵ�ǰ����
    MMISMS_UPDATE_INFO_T backsend_update_info;                       //��̨�����ȱ���Ķ��ŵ�ǰɾ�����������ŵ������Ϣ
} MMISMS_BACKSEND_UPDATE_T;

//����mo��order��Ϣ�����ڲݸ���ͷ�������ŷ��ͳɹ���ɾ����������
typedef struct 
{
    BOOLEAN  is_mosend_update;    
    MN_SMS_STORAGE_E  mosend_updata_storage;
    MN_SMS_RECORD_ID_T mosend_record_id; 
    MMISMS_OPER_STATUS_E mosend_send_oper;
    MMISMS_UPDATE_INFO_T mosend_update_info;
} MMISMS_MOSEND_UPDATE_T;

//���浱ǰ�����Ͷ��ŵĲ�������Ϣ������Ϣ���ڷ��Ͳ�����������savecnf��oper׷��
typedef struct 
{
    uint8 total_num;                       //��ǰ��Ϣ�������� 
    uint8 seq_num;                         //��ǰ���͵ڼ���   
    MN_SMS_STORAGE_E cur_send_storage;     //��ǰ���Ͷ��ſ��ܱ����λ�ã�����λ�ã�sim , me or ������ ����send cnf״̬
    MN_SMS_RECORD_ID_T cur_send_record_id; //��ǰ���ͷ��Ͷ��ſ��ܵ�record id������send cnf��֤״̬   
    MMISMS_OPER_STATUS_E cur_send_oper;    //��ǰ���淢�͵Ĳ���״̬�����ڱ������ʶ�����
} MMISMS_CURSEND_INFO_T;

//SMS struct which need send
typedef struct MMISMS_SEND_T_tag
{            
    BOOLEAN is_deliver_cancel;        
    uint8 resend_count;
    MMISMS_SEND_SAVE_RESULT_T send_save_result;

    MMISMS_SMSEDIT_CONTENT_T send_content;          //���͵����� 

    uint32 start_send_tickcount;                         //��ʼ����ʱ�䣬���ڳ�ʱ����
    
    MMISMS_CURSEND_INFO_T cur_send_data;             //��ǰ���͵Ķ�����Ϣ
                      
    MMISMS_BACKSEND_UPDATE_T backsend_update_data;   //��̨�����ȱ���Ķ�����Ϣ��ţ�����ɾ��������ɺ�ķ��������

    MMISMS_BOX_TYPE_E         cur_box_type;          //���ں�̨���ͷ�����ɿ��Խ������������䣬���Ա���֪�������÷��͵�sms����������
    MMISMS_BACKSENDSTATUS_E backsend_status;         //������Ϣ���͵Ľ׶Σ����ݲ�ͬ�Ľ׶�����ͬ�Ĳ���
    
    MMISMS_MOSEND_UPDATE_T mosend_update_data;       //������ݸ��䷢�Ͷ��ű�������

    BOOLEAN is_need_rpt;
    BOOLEAN is_force_rpt;
    BOOLEAN is_head_has_port;                        //����������ģ�鷢�Ͷ���Ϣʱ�����Ƿ��ж˿�
    BOOLEAN is_sendfail_retry;                          //�Զ���̨�ط�
    SMS_SEND_CALLBACK send_callback;                 //���ͻص�����������ģ�鷢�Ͷ���Ϣʹ��
    struct MMISMS_SEND_T_tag *next;
}MMISMS_SEND_T;

//������Ϣ�ṹ����
typedef struct 
{
    BOOLEAN is_sending_sms;    
    uint32 send_num;
    MMISMS_SEND_T *sendlist_head_ptr;
} MMISMS_SENDINFO_T;

#ifdef IKEYBACKUP_SUPPORT
//for i key backup
typedef struct 
{    
    uint8               longsms_seq_num;   //for compare //bug 146105
    uint16              head_ref;          //for compare //the reference number of SMS
    uint32 time;                           //for compare
    MMISMS_BCD_NUMBER_T number;            //for compare
    MN_DUAL_SYS_E  dual_sys;               //for restore
    MN_SMS_RECORD_ID_T record_id;          //for back up
    MN_SMS_STATUS_E   status;              //for restore
    MN_SMS_STORAGE_E  storage;             //for restore
    MMISMS_FOLDER_TYPE_E folder_type;
    MMISMS_MO_MT_TYPE_E mo_mt_type;
    MN_SMS_U_T sms_t_u;                    //data
} MMISMS_BACKUP_DATA_T;
#endif

//���浱ǰ���������ݵ���Ϣ������˽�����ص�ʹ��
typedef struct 
{
   MN_DUAL_SYS_E    dual_sys;
   MMI_STRING_T     sms_str;      
   uint8            *dest_addr_ptr;      
   uint8            dest_addr_len;     
} MMISMS_WRITE_MSG_INFO_T;

typedef struct 
{
   MN_DUAL_SYS_E    dual_sys;
   MMI_STRING_T     sms_str;      
   MMISMS_DEST_ADDR_INFO_T *dest_info;   
} MMISMS_SEND_GROUP_INFO_T;

typedef struct _MMI_SMS_GLOBAL_T
{
    BOOLEAN                    is_sim_order_ok[MMI_DUAL_SYS_MAX];    //SIM��SMS�Ƿ� ORDER ready
    BOOLEAN                    is_sim_ready[MMI_DUAL_SYS_MAX];     //SIM���Ƿ�ready
    BOOLEAN                 is_nv_order_ok;      // �����Ƿ�ok

    BOOLEAN                    is_mms_ready;        //MMS�Ƿ�ready
    uint8                    update_status_num;
    uint8                    err_sms_num[MMI_DUAL_SYS_MAX];   // sim ���Ͽ�����sms���������
    uint8                    me_err_sms_num;        // �ֻ��Ͽ�����sms���������

    MMISMS_SMSEDIT_CONTENT_T edit_content;    

    MMISMS_READ_MSG_T        read_msg;                 // the data of read sms
    MMISMS_OPER_T            operation;              // the operation status    
    MMISMS_SAVE_INFO_T      save_info;             // the info of save 
    MMISMS_UPDATE_INFO_T    update_info;
    MMISMS_NV_PARAM_T         nv_param;
    MMISMS_READ_MSG_T   class0_msg;
    BOOLEAN                 is_need_sort;
    BOOLEAN                 is_need_load_saved;//�Ƿ���Ҫ���ر����sms
    BOOLEAN                 is_need_update_order;

    BOOLEAN                 is_link_init;
    BOOLEAN                 is_send_fail;
    BOOLEAN                 is_reinit[MMI_DUAL_SYS_MAX];
    MMISMS_ORDERTYPE_E        cur_order_type;     //��ǰ������ʽ
    MN_DUAL_SYS_E           cur_voice_mail_dual_sys;
    MN_DUAL_SYS_E           cur_send_dual_sys;
    MN_DUAL_SYS_E           cur_save_dual_sys;
#ifdef JAVA_SUPPORT
    BOOLEAN                 is_comming_sms_java_running; //use for exit vm when push is over.
#endif
    BOOLEAN                 is_wait_update;
    MMI_SMS_OPERATION_T         handle_ok_msg;
    BOOLEAN                 is_r8;
    BOOLEAN                 is_single_shift;
    MMISMS_FOLDER_TYPE_E    folder_type;
    MMISMS_BOX_TYPE_E   security_box_type;    
    MMISMS_SEARCH_T     search_msg;
    MMISMS_FONT_E       font_size;
    BOOLEAN                 is_delete_same_num;    
    MMISMS_OPER_STATUS_E    move_oper_status;

#ifdef MMI_SMS_CHAT_SUPPORT
    BOOLEAN                 is_chat_mode;
    MMISMS_CHAT_ITEM_T      chat_item[MMISMS_CHAT_MAX_ITEM];
    MMISMS_BCD_NUMBER_T     chat_number;
    uint8                   sender[MMISMS_NUMBER_MAX_LEN+1];//added for mms...
    MMISMS_ORDER_ID_T       chat_oper_order_id;
    MMISMS_ORDER_ID_T       chat_add_order_id;
    BOOLEAN                 is_del_single;
    BOOLEAN                 is_keylock_need_count_sms;  //�����Ƿ�֧��ͳ���Ѷ�sms        
    uint32                  del_index;
    MMISMS_ORDER_ID_T       update_oper_order_id;    
#endif
#ifdef MMI_TIMERMSG_SUPPORT
    BOOLEAN is_timer_msg_replace;
    BOOLEAN is_timer_mode;
    MMISMS_TIMERMSG_NODE curr_timer_mode_node;
    BOOLEAN is_timer_msg;
    MMISMS_TIMERMSG_NODE curr_timer_msg_node;
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMISMS_SET_SIMMODE_T set_mode;
#endif
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    BOOLEAN is_answer_sms;
#endif
    BOOLEAN is_save_draft;
    BOOLEAN is_sms_normal_close;
}MMISMS_GLOBAL_T;

typedef struct
{
    unsigned code; //unicode
    wchar data; //mapping id
    MMI_TEXT_ID_T text_id; //�ı�id
    MMI_IMAGE_ID_T image_id; //ͼƬid
} MMISMS_EMOJI_INFO_T;

typedef struct
{
    MMISMS_EMOJI_INFO_T item[MMISMS_MAX_UCS2_MESSAGE_LEN];
    uint16 length;

}MMISMS_EMOJI_DISPLAY_CONTENT_T;

#ifdef BT_MAP_SUPPORT
typedef enum _MMISMS_MAP_PARSER_RESULT_E
{
    MMISMS_MAP_PARSER_FINISHED,
    MMISMS_MAP_PARSER_NO_SPACE,
    MMISMS_MAP_PARSER_CANCELED   
}MMISMS_MAP_PARSER_RESULT_E;
typedef struct _MMISMS_MAP_PARSER_RESULT_T
{
    MMISMS_MAP_PARSER_RESULT_E ret;
    uint16 sync_count;
}MMISMS_MAP_PARSER_RESULT_T;
#endif


#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)

typedef struct BT_MSG_LIST_T
{
    char            handle[HANDLE_LEN];       //�ڵ�����
    MMI_CTRL_ID_T   record_id;
    BOOLEAN         is_extra_msg;
    wchar    *name;
    uint32  name_len;
    struct BT_MSG_LIST_T *next; //ָ���¸��ڵ�
} BT_MSG_LIST_T;
#endif


#ifdef BT_MAP_SUPPORT
typedef void(*MMISMS_MAP_PARSER_CALLBACK_FUNC)(MMISMS_MAP_PARSER_RESULT_T *result);
#endif

/*****************************************************************************/
//     Description : init the order variable
//    Global resource dependence :  
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_InitSmsOrder(void);

/*****************************************************************************/
//     Description : to get the flag of auto_save
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
MMINV_SMS_AUTOSAVE_T MMISMS_GetAutoSave( void );

/*****************************************************************************/
//     Description : to update the status info form STATUS_TO_BE_READ to STATUS_READ
//                to modify the status in the order list and order the mt sms again.
//    Global resource dependence : g_mmisms_order
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_UpdateSmsInfoInOrder(
                                            MMISMS_ORDER_ID_T             old_order_id,  
                                            MMISMS_STATE_T        *new_order_info_ptr
                                          );
/*****************************************************************************/
//  Description : for cr00116469 @tzw
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenMainMenuWindowExt(void);
/*****************************************************************************/
//     Description : to Set the status report setting
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_SetAutoSave (
                         MMINV_SMS_AUTOSAVE_T    is_auto_save
                         );

/*****************************************************************************/
//     Description : to ready read SMS
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void    MMISMS_ReadyReadSms( void);


/*****************************************************************************/
//     Description : to get the type of MT message 
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
MMISMS_READ_TYPE_E MMISMS_GetMtType(void);

/*****************************************************************************/
//     Description : to check whether the status is SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
void MMISMS_GetPointedStatus(
                             BOOLEAN    *is_mms_ptr,    //OUT: TRUE, is mms
                             uint8        *msg_status_ptr,//OUT: SMS_SF_SMS_TO_BE_READ, SMS_SF_REPORT_TO_BE_READ, and etc
                             uint16        pos_index        //IN:
                             );

/*****************************************************************************/
//     Description : to get the message type
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_MSG_TYPE_E MMISMS_GetMsgType(void);

/*****************************************************************************/
//     Description : mark the message
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurMsgMarked(
                                   BOOLEAN is_marked //IN:
                                   );

/*****************************************************************************/
//     Description : get the message is marked or not
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetCurMsgIsMarked(void );

/*****************************************************************************/
//     Description : mark all message
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAllMsgMarked(
                                   MMISMS_BOX_TYPE_E box_type,
                                   BOOLEAN           is_marked //IN:
                                   );

/*****************************************************************************/
//     Description : to set the global operation status
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetOperStatus(
                                    MMISMS_OPER_STATUS_E    oper_status
                                    );

/*****************************************************************************/
//     Description : to set the global CurOrderId
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void    MMISMS_SetCurOrderId(
                                 int index,
                                 MMISMS_ORDER_ID_T order_id
                                 );

/*****************************************************************************/
//     Description : to set the g_mmisms_global.operation.cur_order_index
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void    MMISMS_SetCurOrderArrIndex(
                                       int index                        
                                       );

/*****************************************************************************/
//     Description : to set the global Current Type
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
void    MMISMS_SetCurType(
                         MMISMS_BOX_TYPE_E box_type
                         );
/*****************************************************************************/
//     Description : to get the global operation type
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E  MMISMS_GetCurOperationType(void);

/*****************************************************************************/
//     Description : to check the oper of update is ok
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_IsUpdateOk( void );


/*****************************************************************************/
//     Description : to set the update info
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_SetSMSUpdateInfo( 
                              MMISMS_ORDER_ID_T order_id
                            );

/*****************************************************************************/
//     Description : to display the err code of save
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_DisplayPrompByErr(
                              MMISMS_OPER_ERR_E        err_code,    //IN:
                              MMI_TEXT_ID_T            label_id    //IN:
                              );



/*****************************************************************************/
//     Description : to init the global
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_InitSendSaveResult(MMISMS_SEND_T *send_info_ptr);

/*****************************************************************************/
//     Description : to get the count of box
//    Global resource dependence : g_mmisms_order
//  Author:louis.wei
//    Note:
/*****************************************************************************/
uint16    MMISMS_GetBoxCount(
                            MMISMS_BOX_TYPE_E    box_type
                            );

/*****************************************************************************/
//     Description : to get the current oper status 
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
MMISMS_OPER_STATUS_E MMISMS_GetCurrentOperStatus( void );

/*****************************************************************************/
//     Description : �õ���ǰ������ʽ
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDERTYPE_E MMISMS_GetOrderType(void);

/*****************************************************************************/
//     Description : ���õ�ǰ������ʽ
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetOrderType (MMISMS_ORDERTYPE_E  order_type);

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : ���õ�ǰ�������С
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetFontSize (MMISMS_FONT_E  font_size);

/*****************************************************************************/
//  Description : �õ���ǰ�������С
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_FONT_E MMISMS_GetFontSize(void);
#endif

/*****************************************************************************/
//     Description : �Ƿ���Ҫ��������
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsNeedSort(void);

/*****************************************************************************/
//     Description : ������Ҫ��������
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetNeedSort(BOOLEAN is_sort);

/*****************************************************************************/
//     Description : to clear the global variable
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_ClearOperInfo(void);

/*****************************************************************************/
//     Description : to get the status by pos
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
uint8 MMISMS_Getstatus_flagByPos(                //RETURN:
                            uint16    pos        //IN: 
                            );


/*****************************************************************************/
//     Description : to delete SMS info from order list by storage
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
BOOLEAN MMISMS_DelSMSByStore(                                            //RETURN:
                             BOOLEAN                 is_mt_sms,            //IN:
                             MN_DUAL_SYS_E            dual_sys,           //IN:
                             MN_SMS_STORAGE_E        storage,            //IN:
                             MN_SMS_RECORD_ID_T        record_id            //IN:
                             );

/*****************************************************************************/
//     Description : to get the flag of status report
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_GetStatusReportFlag(void);

/*****************************************************************************/
//     Description : to Set the status report flag
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
void MMISMS_SetStatusReportFlag(
                                BOOLEAN    status_report_flag
                                );

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
/*****************************************************************************/
//     Description : allow to add auto signature
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_AllowAutoSignature(void);

/*****************************************************************************/
//     Description : to Set the sms auto signature flag
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAutoSignatureFlag(
                                       BOOLEAN    auto_signature_flag
                                       );

/*****************************************************************************/
//     Description : to get the flag of sms auto signature
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetAutoSignatureFlag(void);

/*****************************************************************************/
//     Description : to get the flag of sms auto signature content
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetAutoSignatureContent(wchar *signature_str_ptr);

/*****************************************************************************/
//     Description : to Set the sms auto signature content
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAutoSignatureContent(
                                       wchar    *signature_str_ptr, //OUT:
                                       uint8    signature_str_len //IN:
                                       );
#endif
/*****************************************************************************/
//     Description : to get the msd status
//    Global resource dependence : g_mmisms_is_msd, 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
BOOLEAN    MMISMS_GetMSDStatus(void);

/*****************************************************************************/
//     Description : to set the msd status
//    Global resource dependence : g_mmisms_is_msd, 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMISMS_SetMSDStatus(BOOLEAN is_msd);

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
/*****************************************************************************/
//  Description : to get the flag of reply path
//  Global resource dependence : g_mmisms_global
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetReplyPathFlag(void);

/*****************************************************************************/
//  Description : to Set the status reply path
//  Global resource dependence : g_mmisms_global
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetReplyPathFlag(
                                    BOOLEAN    reply_path_flag
                                    );
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
/*****************************************************************************/
//  Description : to get the sim mode
//  Global resource dependence : g_mmisms_global
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetSIMModeFlag(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : to set the sim mode
//  Global resource dependence : g_mmisms_global
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSIMModeFlag(MN_DUAL_SYS_E dual_sys, uint8 mode);
#endif

/*****************************************************************************/
//     Description : to get the information of memory
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_GetSMSMemory(
                         MN_DUAL_SYS_E    dual_sys,
                         BOOLEAN        is_nv,            //IN:
                         uint16            *max_num_ptr,    //OUT:
                         uint16            *used_num_ptr    //OUT:
                         );

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : Set Empty to the list box   
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMS_AppendEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,        //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: for different position, the empty string is different.
                             MMI_TEXT_ID_T    left_softkey_id    //IN:
                             );
#endif

/*****************************************************************************/
//     Description : Set Empty to the list box   
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMS_SetEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,        //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: for different position, the empty string is different.
                             MMI_TEXT_ID_T    left_softkey_id    //IN:
                             );

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : display the listbox style
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_SetListBoxTitleStyle(
                                 MMI_CTRL_ID_T    ctrl_id,    //IN:
                                 BOOLEAN        have_items    //IN:
                                 );
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : get the SMS num of the read message
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
uint8 MMISMS_GetReadSMSNum(void);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : set the SMS num of the read message
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
void MMISMS_SetReadSMSNum(
                          uint8 sms_num
                          );
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : to get the send path
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC MN_SMS_PATH_E MMISMS_GetSendPath(void);

/*****************************************************************************/
//     Description : to get the preferred connection
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_PREFERRED_PATH_E MMISMS_GetPreferredConnection(void);

/*****************************************************************************/
//     Description : to set the preferred connection
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetPreferredConnection(MMISMS_PREFERRED_PATH_E preferred_connection);

/*****************************************************************************/
//     Description : to get the save position
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
MMISMS_SMS_SAVEPOS_TYPE_E MMISMS_GetSavePos(void);

/*****************************************************************************/
//     Description : to set the save position
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
void MMISMS_SetSavePos(
                        MMISMS_SMS_SAVEPOS_TYPE_E    pos
                        );

/*****************************************************************************/
//     Description : Compute Current Order Id according to record_id,storage, cur_type
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  change the g_mmisms_global.operation.cur_order_id;
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurOperationOrderId(void);

/*****************************************************************************/
//     Description : set Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC void MMISMS_SetCurOperationOrderId(MMISMS_BOX_TYPE_E box_type, uint32 index);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : set the first to be read sms as Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC void MMISMS_SetFirstUnreadSmsToGlobal(void);
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : ��ʼ��ȫ�ֱ���g_mmisms_global��
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  ��g_mmisms_global��0��
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InitGlobalInfo(void);

/*****************************************************************************/
//     Description : ����g_mmisms_global.is_sim_ready or is_nv_ready
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetSmsOrderReady (
                                        MN_DUAL_SYS_E           dual_sys,
                                        MN_SMS_STORAGE_E        storage,
                                        BOOLEAN                    is_order_ready
                                        );

/*****************************************************************************/
//     Description : to display the update prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_DisplayUpdatePrompt( 
                                        BOOLEAN    is_succ        //IN:
                               );


/*****************************************************************************/
//     Description : to update the next SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_UpdateNextSms( 
                                                MN_DUAL_SYS_E       dual_sys,
                                                uint8                *dest_addr_ptr,    //IN:
                                                uint8                total_num,        //IN
                                                uint8                seq_num,        //IN: the seq number (1 ~ 8)
                                                MN_SMS_STATUS_E        status,           //IN:
                                                MMISMS_SEND_T *send_info_ptr,
                                                BOOLEAN is_backsend_update
                                             );


/*****************************************************************************/
//     Description : to get a free space in ME or SIM for long sms(total_num <= space num)
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetStoreStorageForSendLongSms(
                                       MN_DUAL_SYS_E        dual_sys, 
                                       uint8                total_num,        //IN
                                       uint8                seq_num,        //IN
                                       MN_SMS_STORAGE_E        *storage_ptr    //OUT:
                                       );

/*****************************************************************************/
//     Description : to get a free space in ME or SIM
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetStoreStorage(
                                   MN_DUAL_SYS_E            dual_sys,  
                                   MN_SMS_STORAGE_E        *storage_ptr,    //OUT:
                                     MN_SMS_RECORD_ID_T    *record_id_ptr    //OUT:
                                  );

/*****************************************************************************/
//     Description : to get a free space in ME or SIM
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetStoreStorageEx(
                                   MN_DUAL_SYS_E            dual_sys,  
                                   MN_SMS_STORAGE_E        *storage_ptr,    //OUT:
                                   MN_SMS_RECORD_ID_T    *record_id1_ptr,    //OUT:
                                   MN_SMS_RECORD_ID_T    *record_id2_ptr    //OUT:  
                                  );



/*****************************************************************************/
//     Description : to get a free record in ME or SIM
//    Global resource dependence : none
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC  BOOLEAN  MMISMS_FindFreeRecordId(
                                     MN_DUAL_SYS_E            dual_sys,  
                                     MN_SMS_STORAGE_E    storage,        //IN
                                    MN_SMS_RECORD_ID_T    *record_id_ptr    //OUT:
                                    );

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
                                     );

/*****************************************************************************/
//     Description : set the SMS num of the read message
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetCurBoxType(void);

/*****************************************************************************/
//     Description : Clear the save info in g_mmisms_global
//    Global resource dependence :  
//  Author: liming.deng
//    Note:
/*****************************************************************************/    
PUBLIC void MMISMS_ClearSaveInfo(void);

/*****************************************************************************/
//     Description : get g_mmisms_global.is_mms_ready 
//    Global resource dependence : 
//  Author:liming.deng  
//    Note: �õ�ȫ�ֱ���g_mmisms_global.is_mms_ready��ֵ
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsMMSOrderReady(void);


/*****************************************************************************/
//     Description : set g_mmisms_global.is_mms_ready 
//    Global resource dependence : 
//  Author:liming.deng  
//    Note: ��MMS�����������ʱ����ȫ�ֱ���g_mmisms_global.is_mms_ready��
/*****************************************************************************/
PUBLIC void MMISMS_SetMMSOrderReady(BOOLEAN is_mms_ready);


/*****************************************************************************/
//     Description : get current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetCurVoiceMailDualSys(void);


/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetCurVoiceMailDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetCurSaveDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetCurSendDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : get current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetCurSendDualSys(void);

/*****************************************************************************/
//     Description : get current read msg dual sys
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetCurReadMsgDualSys(void);

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadSmsCountEx(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadSmsCount(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetFirstUnreadSmsId(void);

/*****************************************************************************/
//     Description : to get the count of mms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetFirstUnreadMmsId(void);

/*****************************************************************************/
//     Description : to get the count of msg(sms mms wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetFirstUnreadMsgId(void);

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to get the count of msg(sms mms wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetFirstUnreadPushMsg(void);
#endif

/*****************************************************************************/
//     Description : get the count of msg(sms\wap push\mms\..)for widget
//    Global resource dependence : 
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetAllUnreadMsgCountEx(void);

/*****************************************************************************/
//     Description : get the count of msg(sms\wap push\mms\..)for widget
//    Global resource dependence : 
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetAllUnreadMsgCount(void);

/*****************************************************************************/
//     Description : to clear the all destinations in g_mmisms_deliver
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ClearDestNum( void );

#ifdef MMI_SMS_MINI_SUPPORT
/*****************************************************************************/
//     Description : to clear the all sms content edit
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void    MMISMS_ClearEditContent( void );
#endif

#if defined(MMI_VCARD_SUPPORT) || defined(MMI_VCALENDAR_SUPPORT)
/*****************************************************************************/
// Description : to show the new Vcard prompt
//               (�ο�����:MMISMS_ShowNewMsgPrompt)
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewVcardOrVcalendarPrompt(BOOLEAN is_play_ring,MMIVC_SMS_SEND_TYPE_E type);
#endif



/*****************************************************************************/
// Description : to send the SMS for Vcard
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
MMISMS_OPER_ERR_E MMISMS_AppSendSmsForVcardAndVcal(
                                                   MN_DUAL_SYS_E   dual_sys,
                                                   MMI_STRING_T    *sms_init_ptr,
                                                   BOOLEAN is_vcard);

/*****************************************************************************/
//     Description : get current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetCurSaveDualSys(void);

/*****************************************************************************/
//     Description : lock the message
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurMsgLocked(BOOLEAN is_locked);

/*****************************************************************************/
//     Description : lock or unlock the marked message
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMarkedMsgLock(MMISMS_BOX_TYPE_E box_type,BOOLEAN is_lock);

/*****************************************************************************/
//     Description : get the message is locked or not
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CurMsgIsLocked(void );
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC BOOLEAN MMIMAIL_CurMailIsSending(void )
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/15/2011 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_CurMailIsSending(void );
#endif

/*****************************************************************************/
// Description : get the message record id
// Global resource dependence : 
// Author:minghu.mao
// Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_CurMsgRecordId(void );

/*****************************************************************************/
//  Description : init sort status according setting
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_InitSortStatusAccordingToSetting(void);



#ifdef JAVA_SUPPORT
/*****************************************************************************/
//     Description : is MT for java
//    Global resource dependence : 
//  Author: kelly.li
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsMtForJava(
                            MN_SMS_USER_HEAD_T         user_head_ptr_t,
                            int32                    *des_port_ptr,
                            int32                    *org_port_ptr
                            );
#endif

/*****************************************************************************/
//  Description : MMISMS_RefreshSMS
//  Global resource dependence : 
//  Author: wancan.you
//RETURN: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_RefreshSMS(MN_DUAL_SYS_E dual_sys);

#if 0
/*****************************************************************************/
//     Description : set is need load saved sms
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetIsNeedLoadSaved(BOOLEAN is_need_load_saved);

/*****************************************************************************/
//     Description : get is need load saved sms
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsNeedLoadSaved(void);
#endif

/*****************************************************************************/
//     Description : set is send fail sms
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetIsSendFail(BOOLEAN is_send_fail);

/*****************************************************************************/
//     Description : get is send fail sms
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsSendFail(void);

/*****************************************************************************/
//     Description : convet sms content to display content
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ConvertSmsContentToDisplayContent(
                                                    MMISMS_SMSDISPLAY_CONTENT_T *display_content_ptr,//out
                                                    uint8 *message_content_ptr,
                                                    uint16 content_len,
                                                    MN_SMS_ALPHABET_TYPE_E alphabet
                                                );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetMaxSimSmsNum(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMaxSimSmsNum(MN_DUAL_SYS_E dual_sys, uint8 sim_sms_num);

/*****************************************************************************/
//     Description : to init the factory setting of security pwd
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSecurityPwd(void);

/*****************************************************************************/
//     Description : set Current Search Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC void MMISMS_SetCurSearchOperationOrderId(
                                        MMISMS_BOX_TYPE_E box_type,
                                        MMISMS_ORDER_ID_T order_id);

/*****************************************************************************/
//     Description : MMISMS_GetIsNeedReSort
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsNeedReSort(void);

/*****************************************************************************/
//     Description : MMISMS_SetIsNeedReSort
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetIsNeedReSort(BOOLEAN is_need_re_sort);

/*****************************************************************************/
//     Description : MMISMS_GetIsSMSRefresh
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsSMSRefresh(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : MMISMS_SetIsSMSRefresh
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetIsSMSRefresh(MN_DUAL_SYS_E dual_sys, BOOLEAN is_sms_refresh);

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetConcatenatedSMS(
                                BOOLEAN is_concatenated_sms
                                );

/*****************************************************************************/
//     Description : to Get the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetConcatenatedSMS(void);
#endif

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
/*****************************************************************************/
//     Description : to Set the alphabet management
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAlphabetManagement(
                                MMISMS_ALPHABET_MANAGEMENT_E alpha_management
                                );

/*****************************************************************************/
//     Description : to Get the alphabet management
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ALPHABET_MANAGEMENT_E MMISMS_GetAlphabetManagement(void);
#endif
#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to set the global operation status
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void    MMISMS_SetMoveOperStatus(
                                        MMISMS_OPER_STATUS_E    oper_status
                                        );
#endif
/*****************************************************************************/
//  Description : to get the current oper status 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_STATUS_E MMISMS_GetCurrentMoveOperStatus( void );

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related MO SMS box
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMoSMSForMoveAll(
                                                         MMISMS_BOX_TYPE_E box_type,
                                                         MN_SMS_STORAGE_E storage
                                                        );

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related MT SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMTSMSForMoveAll(MN_SMS_STORAGE_E storage);

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related Security SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecuritySMSForMoveAll(MN_SMS_STORAGE_E storage);

/*****************************************************************************/
//  Description : get move num from storage by dual_sys
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetMarkedStorageMsgNumByDualsys(
                                           MMISMS_BOX_TYPE_E box_type,
                                           MN_SMS_STORAGE_E storage,
                                           MN_DUAL_SYS_E    dual_sys
                                           );
#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : is move operation 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsMoveOperation(MMISMS_OPER_STATUS_E oper_status);
#endif

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : is copy operation 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsCopyOperation(MMISMS_OPER_STATUS_E oper_status);
#endif

/*****************************************************************************/
//  Description : set Current operation order
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMISMS_SetGlobalOperationOrderId(MMISMS_ORDER_ID_T order_id);

#if 0
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : set Current Order Id
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMISMS_SetChatCurOperationOrderId(uint32 index);
#endif
#endif

#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
//     Description : set Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC void MMISMS_SetCurOperationOrderIdByTimerMsg(MMINV_SMS_TIMERMSG_T *timer_msg_ptr);
#endif


#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : get read msg content
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetReadMsgContent(
                                    MMISMS_READMSG_CONTENT_T *read_content_ptr      //OUT
                                    );
#endif                              

/*****************************************************************************/
//     Description : open the SMS sending window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSendingWindow(
                             MN_DUAL_SYS_E  dual_sys,
                             MMI_STRING_T    *prompt_ptr        //IN:
                             );
                             
/*****************************************************************************/
//  Description : init update info
//  Global resource dependence : g_mmisms_global
//  Author:rong.gu
//  Note:  
/*****************************************************************************/
PUBLIC void MMISMS_InitUpdateInfo(void);

/*****************************************************************************/
//     Description : Set Readed SMS to global
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveReadedSmsToGlobal(
                                 MN_DUAL_SYS_E                  dual_sys,
                                 MMISMS_READ_TYPE_E          msg_type,                //IN:
                                 APP_SMS_USER_DATA_T         *new_user_data_ptr,    //IN: the user data of new message 
                                 MN_CALLED_NUMBER_T          *address_ptr,            //IN:
                                 MN_SMS_ALPHABET_TYPE_E      alphabet_type            //IN: the alphabet type of new message
                                 );

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : SMS���ݺ���
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMISMS_IKeyBackup(MMIIKB_SIG_BACKUP_ITEM_DATA_REQ_T *backup_data_ptr);

/*****************************************************************************/
//  Description : SMS��ԭ����
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMISMS_IKeyRestore(MMIIKB_SIG_RESTORE_ITEM_DATA_REQ_T *restore_data_ptr);

/*****************************************************************************/
//  Description : SMS���ݲ��Һ���
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindMsgForSmsBackup(uint32 index);

/*****************************************************************************/
//  Description : SMS��ԭ�Ƿ���ͬ�жϺ���
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: judged by the time and number
/***************************************************************************/
PUBLIC BOOLEAN MMISMS_IsExistSameSms(MMISMS_BACKUP_DATA_T *backup_data_ptr);
#endif
/*****************************************************************************/
//     Description : to get the icon by the status
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMISMS_GetIconByStatus(MMISMS_STATE_T *sms_state_ptr);

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//  Description : set Current Order Id
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMISMS_SetPushMsgCurOperationOrderId(uint32 index);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : SMSɾ������sms
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
PUBLIC BOOLEAN MMISMS_StartDeleteMatchMsgById(MMISMS_DETAIL_MSGLOG_DATA_T   *match_data_ptr);

/*****************************************************************************/
//  Description : SMSɾ������sms
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteNextMatchMsgById(MMISMS_ORDER_ID_T order_id);
/*****************************************************************************/
//  Description : SMSɾ������sms
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
PUBLIC void MMISMS_EndDeleteMatchMsg(MMISMS_DETAIL_MSGLOG_DATA_T   *match_data_ptr);
#endif

/*****************************************************************************/
//  Description :mms number to bec
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_MMSNumberToBcd(char *sender,uint8 *number,uint8 *len);

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetLocalSettings(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_FOLDER_TYPE_E MMISMS_GetFoldType(void);

/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetFoldType(MMISMS_FOLDER_TYPE_E folder_type);

/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetSecurityBoxType(void);

/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSecurityBoxType(MMISMS_BOX_TYPE_E   security_box_type);

/*****************************************************************************/
//     Description : to get the origin number form MT SMS
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetClass0OriginNum(                        //RETURN: the length of num_ptr
                                 MN_DUAL_SYS_E *dual_sys_ptr,
                                 uint8            *num_ptr,        //OUT:
                                 uint8            max_num_len        //IN:
                                 );

/*****************************************************************************/
//     Description : to check the space
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_CountFreeSpace(MN_DUAL_SYS_E dual_sys);
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : to check the space
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetGlobalChatItem(MMISMS_CHAT_ITEM_T *chat_item,uint16 item_num);
#endif

/*****************************************************************************/
// 	Description :  Set Folder Type Manager
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetNVFolderTypeFlag(
                                int32 order_id,
                                MMISMS_FOLDER_TYPE_E folder_type);

/*****************************************************************************/
//  Description : get edit content info
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SMSEDIT_CONTENT_T* MMISMS_GetEditContentInfoPtr(void);

/*****************************************************************************/
//  Description : Set edit content info
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetEditContentInfo(MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr);

/*****************************************************************************/
//     Description : Compute Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetOperationOrderId(uint16 index);

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : Set chat order id
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetChatOperOrderID(MMISMS_ORDER_ID_T order_id);

/*****************************************************************************/
//  Description : get chat order id
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetChatOperOrderID(void);

/*****************************************************************************/
//  Description : get chat order id
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetChatAddOrderID(MMISMS_ORDER_ID_T order_id);

/*****************************************************************************/
//  Description : get chat order id
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetChatAddOrderID(void);
#endif

/*****************************************************************************/
//     Description : to get the type of MT message 
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_READ_MSG_T MMISMS_GetReadResult(void);

/*****************************************************************************/
// 	Description :  Write Folder Type Manager
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_WriteFolderType(void);

/*****************************************************************************/
//     Description : Compute Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_InitCurOperationOrderId(void);

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
/*****************************************************************************/
// 	Description : set is answer sms 
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetIsAnswerSms(BOOLEAN is_answer_sms);
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description :Set global chat number
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetGlobalChatNumber(MMISMS_BCD_NUMBER_T     chat_number,uint8 *sender);

/*****************************************************************************/
//  Description :�Ƿ������Ƿ�δ������
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetIsKeyLockNeedCountSms(BOOLEAN is_keylock_need_count_sms);

/*****************************************************************************/
//  Description :�Ƿ������Ƿ�δ������
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsKeyLockNeedCountSms(void);
#endif

/*****************************************************************************/
//  Description :set need update info
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetNeedUpdateInfo(void);

/*****************************************************************************/
//  Description : enter single sms win
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterShowSmsWin(MMISMS_READ_MSG_T read_msg);

/*****************************************************************************/
//     Description : to open the window to show voice mailbox number of line1 ,line2
//                 fax and data
//    Global resource dependence : 
//  Author:kelly.li
//    Note: 2006-02-10
/*****************************************************************************/
PUBLIC void MMISMS_OpenVMNumberWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type);

/*****************************************************************************/
//     Description : Set readed SMS to window
//    Global resource dependence : g_mmisms_global
//  Author: rong gu
//    Note:��ȡ��ʾ����
/*****************************************************************************/
PUBLIC void MMISMS_GetReadSmsDispNum(
                                         MMISMS_READ_MSG_T *read_msg_ptr, //in
                                         wchar *num_string_ptr,//out
                                         uint16 *num_string_len //out
                                     );

/*****************************************************************************/
//     Description : Set readed SMS to window
//    Global resource dependence : g_mmisms_global
//  Author: rong gu
//    Note:��ȡʱ��
/*****************************************************************************/
PUBLIC void MMISMS_GetReadSmsDispTime(
                                         MMISMS_READ_MSG_T *read_msg_ptr, //in
                                         wchar *time_string_ptr,//out
                                         uint16 *time_string_len //out
                                     );

/*****************************************************************************/
//     Description : Set readed SMS to window
//    Global resource dependence : g_mmisms_global
//  Author: rong gu
//    Note:��ȡ��ʾ����
/*****************************************************************************/
PUBLIC void MMISMS_GetReadSmsDispContent(
                                         MMISMS_READ_MSG_T *read_msg_ptr, //in
                                         wchar *content_string_ptr,//out
                                         uint16 *content_string_len //out
                                     );

/*****************************************************************************/
//     Description : convet sms content to display content
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ConvertValidDataToDispStr(
                                                     wchar *display_content_ptr,//out
                                                     uint16 *display_content_len,//OUT:
                                                     uint8 *message_valid_data, //in
                                                     uint16 convert_len,//ת���ַ��ĳ���
                                                     MN_SMS_ALPHABET_TYPE_E alphabet, // in
                                                     MMISMS_R8_LANGUAGE_E language, //in                                                     
                                                     BOOLEAN is_lock_shift,//lang Ϊr8ʱ��Ч
                                                     BOOLEAN is_single_shift//lang Ϊr8ʱ��Ч
                                                    );                                     

/*****************************************************************************/
//     Description : MMISMS_GetUnhandleMeSmsNumber
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_E type);

#ifdef MMI_MSG_RESEND
/*****************************************************************************/
//     Description :
//    Global resource dependence :
//  Author: Jiaoyou.wu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetResendFlag(void);

/*****************************************************************************/
//     Description :
//    Global resource dependence :
//  Author: Jiaoyou.wu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetResendFlag(BOOLEAN is_resend);
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/******************************************************************************/
// Description: MMIAPISMS_CreateMapSyncTimer
// Global resource dependence: 
// Author: 
// Note: ��ʼ��ʱ��ʱ��
/******************************************************************************/
PUBLIC void MMIAPISMS_CreateMapSyncTimer(void);

/******************************************************************************/
// Description: MMIAPISMS_StopMapSyncTimer
// Global resource dependence: 
// Author: Candice Chen
// Note: ֹͣ��ʱ��ʱ��
/******************************************************************************/
PUBLIC void MMIAPISMS_StopMapSyncTimer(void);

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: Candice Chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_MapDisconnect(void);

/*****************************************************************************/
//  Description : MMIAPISMS_InsertMAPDataToList
//  Global resource dependence :                                
//  Author: Candice Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_InsertMAPDataToList(void* buff,uint16 buff_len);

/*****************************************************************************/
//  Description : MMIAPISMS_InsertMessageDataToList
//  Global resource dependence :                                
//  Author: Candice Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_InsertMessageDataToList(void* buff,uint16 buff_len);

/*****************************************************************************/
//  Description :notify pb start parser s_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifySMSStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);

/*****************************************************************************/
//  Description :notify pb start parser s_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifySetFolderParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);

/*****************************************************************************/
//  Description :notify pb start parser s_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifyGetMsgListStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);

/*****************************************************************************/
//  Description :notify pb start parser s_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_NotifyGetMessageStartParser(MMISMS_MAP_PARSER_CALLBACK_FUNC callback);

/*******************************************************************************/
//  Description : MMIAPISMS_InitMAPDataList
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_InitMAPDataList(void);

/*******************************************************************************/
//  Description : ������ͷ�ڵ�ĵ���������ͷ���
//  Parameter: 
//  Global resource dependence :                                
//  Author: 
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_ClearTableList(void);


/*****************************************************************************/
//  Description : save long sms content
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveBTMsgReq(
                                                MN_DUAL_SYS_E            dual_sys,
                                                uint8                    *dest_addr_ptr,        //IN:
                                                uint8                    dest_len,            //IN:
                                                APP_SMS_USER_DATA_T         *user_data_ptr,        //IN:
                                                MN_SMS_STORAGE_E            storage,            //IN:
                                                MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN:
                                                char* number,
                                                uint32 number_len,
                                                char* content,
                                                uint32 content_len
                                                );

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
                                              );

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
                                              );

/*****************************************************************************/
//  Description : convert data string to int
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMISMS_StringToInt(uint8 *time, uint8 len);

/*******************************************************************************/
//  Description : MMISMS_ParseMapData
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseMapData(void);

/*******************************************************************************/
//  Description : MMISMS_ParseSetFolder
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseSetFolder(void);

/*******************************************************************************/
//  Description : MMISMS_ParseMapMsgList
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseMapMsgList(void);

/*******************************************************************************/
//  Description : MMISMS_ParseMapBMsg
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_ParseMapBMsg(void);

/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_GetMsgFromMapMsgList(const char*  buff);

/*****************************************************************************/
//  Description : save long sms MT content
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongMtSmsContent(
                                                        char            *number,
                                                        MMI_STRING_T    *subject,
                                                        uint32          time
                                                        );

/*****************************************************************************/
//  Description : save long sms MO content
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongMoSmsContent(
                                                        MMISMS_OPER_STATUS_E    oper_status,
                                                        char                    *number,
                                                        MMI_STRING_T            *subject,
                                                        uint32                  time
                                                        );

/*****************************************************************************/
//  Description : save mt long sms content
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongSmsMtNextContent(
                                                            MN_DUAL_SYS_E   dual_sys,
                                                            uint8           *dest_addr_ptr,    //IN:
                                                            uint8           total_num,         //IN
                                                            uint8           seq_num,           //IN: the seq number ( 1 ~ 8)
                                                            char            *number,
                                                            uint32          number_len
                                                            );


/*****************************************************************************/
//  Description : save long sms content
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMIAPISMS_SaveLongMoSmsNextContent(
                                                            MMISMS_OPER_STATUS_E     oper_status,
                                                            MN_DUAL_SYS_E            dual_sys,
                                                            uint8                    *dest_addr_ptr,    //IN:
                                                            uint8                    total_num,         //IN
                                                            uint8                    seq_num,           //IN: the seq number ( 1 ~ 8)
                                                            char                     *number,
                                                            uint32                   number_len
                                                            );


#ifdef WIN32
/*****************************************************************************/
//     Description : to handle the BT message of SMS box window
//     Global resource dependence : 
//  Author: Candice Chen
//     Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SimuMAPDataToList(uint16 index);
#endif

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
                                           );

/*******************************************************************************/
//  Description : �������ĩβ���һ��Ԫ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPISMS_FreeAllMapData(void);

/*****************************************************************************/
//  Description : to show the new message prompt
//  Global resource dependence : 
//  Author: Candice Chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ShowNewBTMsgPrompt(void);

/*****************************************************************************/
// Description : MMIAPISMS_HandleBTMapRegister
// Global resource dependence : 
// Author: Naomi Huang
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleBTMapRegister(void);

/*****************************************************************************/
//  Description : MMIAPISMS_HandleServerMsgData
//  Global resource dependence : none
//  Author: Naomi Huang 
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_HandleServerMsgData(void* buff,uint16 buff_len);

/*******************************************************************************/
//  Description : �������ĩβ���һ��Ԫ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
PUBLIC int MMIAPISMS_AddRecordIdIntoMsgHandleTableList(MMI_CTRL_ID_T record_id);

/*****************************************************************************/
//  Description : MMIAPISMS_GetMessageTableList
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC BT_MSG_LIST_T* MMIAPISMS_GetMessageTableList(void);

/*****************************************************************************/
//  Description : MMISMSAPI_GetCurrentHandle
//  Global resource dependence : none
//  Author: Candice Chen
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMISMSAPI_GetCurrentHandle(char* handle);

/*******************************************************************************/
//  Description : �������ĩβ���һ��Ԫ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: Candice Chen
//  Return : 
/*****************************************************************************/
 PUBLIC BOOLEAN MMIAPISMS_GetTableListDate(BT_MSG_LIST_T *L, char* handle, char* datetime);

/*****************************************************************************/
//     Description : MMIAPISMS_GetCurOperStatus
//    Global resource dependence : 
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_STATUS_E  MMIAPISMS_GetCurOperStatus(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC MMI_STRING_T MMIAPISMS_GetLongMessageData(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: candice chen
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_ClearLongMessageData(void);

#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT) && defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
/******************************************************************************/
// Description: MMIAPISMS_CreateSmsSyncTimer
// Global resource dependence: 
// Author: 
// Note: ��ʼ��ʱ��ʱ��
/******************************************************************************/
PUBLIC void MMIAPISMS_CreateSmsSyncTimer(void);

/******************************************************************************/
// Description: MMIAPISMS_StopSmsSyncTimer
// Global resource dependence: 
// Author: 
// Note: ֹͣ��ʱ��ʱ��
/******************************************************************************/
PUBLIC void MMIAPISMS_StopSmsSyncTimer(void);

#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
