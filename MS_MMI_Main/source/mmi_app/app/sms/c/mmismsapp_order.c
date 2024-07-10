/****************************************************************************
** File Name:      mmismsapp_order.c                                       *
** Author:                                                                 *
** Date:           04/11/2004                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the SMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2004        louis.wei        Create
** 08/2006          Tracy Zhang       Modify for SP7550
** 08/2007        liming.deng      MOdify for unified message
****************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmiidle_export.h"
#include "mmipb_export.h"
#include "mmimms_export.h"
#include "mms_image.h"
#include "mmi_textfun.h"
#include "mmiudisk_export.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmisms_export.h"
#include "mmisms_order.h" 
#include "mmisms_read.h"
#include "mmisms_send.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmisms_timermsg.h"
#include "mmialarm_service.h"
#endif
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmibrw_image.h"
#include "mmiudisk_export.h"
#include "mmiota_export.h"
#include "mmi_custom_define.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmiacc_event.h"
#endif
#ifdef _ATEST_
#include "atest_ota.h"
#endif//_ATEST_
#ifdef PUSH_EMAIL_SUPPORT  //Added by yuanl  2010.8.7
#include "mmimail_export.h"
#endif /* PUSH_EMAIL_SUPPORT */
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mail_fldr.h"
#include "mail_export.h"
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
#include "mmiidle_statusbar.h"

#include "mmisms_delete.h"
#include "mmipb_app.h"
#include "mmisms_save.h"
#include "mmi_nv.h"
#ifdef MMI_PDA_SUPPORT
#include "mmipub.h"
#endif

#include "mmisms_appcontrol.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/
#define MMISMS_SMS_LOCK_FLAG 0x01
#define MMISMS_MMS_LOCK_FLAG 0x02
#define MMISMS_WAPPUSH_LOCK_FLAG 0x04
#define MMISMS_OTA_LOCK_FLAG 0x08
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#define MMISMS_MAIL_LOCK_FLAG 0x10
#endif//PUSH_EMAIL_3_IN_1_SUPPORT 

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMISMS_PORTRAIT_SUPPORT
#define SMS_LIST_SMS_PB_PHOTO_ICON 0
#define SMS_LIST_SMS_STATUS_ICON 1
#define SMS_LIST_SMS_PHONE_NAME_ICON 2
#define SMS_LIST_SMS_DATE_ICON 3
#define SMS_LIST_SMS_SIM_ICON 4
#define SMS_LIST_CONTENT_SIM_ICON 5
#else
#define SMS_LIST_SMS_STATUS_ICON 0
#define SMS_LIST_SMS_PHONE_NAME_ICON 1
#define SMS_LIST_SMS_DATE_ICON 2
#define SMS_LIST_CONTENT_SIM_ICON 3
#if defined(MAINLCD_SIZE_128X160)
#define SMS_LIST_SMS_SIM_ICON 4
#endif
#endif
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
#define SMS_BOX_DELNUM_MAX  4
#define SMS_BT_INBOX    0
#define SMS_BT_OUTBOX   1
#define SMS_BT_DRAFT    2
#define SMS_BT_SENDBOX  3
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
typedef enum
{
    MMISMS_DELMSG_TYPE_SMS,
    MMISMS_DELMSG_TYPE_MMS,
    MMISMS_DELMSG_TYPE_OTAPUSH,
    MMISMS_DELMSG_TYPE_WAPPUSH,
    MMISMS_DELMSG_TYPE_MAX
} MMISMS_DELMSG_MSGTYPE_TYPE_E;
#endif

typedef uint32 MMISMS_LOCK_FLAG_STATUS;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/       
LOCAL MMISMS_ORDER_RESULT_T s_mmisms_order; // SMS������
LOCAL MMISMS_LINKED_ORDER_T s_mmisms_linked_order;//SMS��������
//#ifdef MMI_SMS_CHAT_SUPPORT
LOCAL MMISMS_CHAT_UNIT_T  s_mmisms_chat_linked_order;
//#endif
#ifdef MMI_PDA_SUPPORT
MMISMS_DETAIL_MSGLOG_DATA_T *g_sms_match_data_ptr = PNULL;
uint16 g_match_sms_delete_num = 0;
#ifdef MMS_SUPPORT
LOCAL BOOLEAN g_match_is_exist_mms = 0;
#endif
#endif

#define MMISMS_SORT_CAHCE_BUFFER_SUPPORT

#ifdef MMISMS_SORT_CAHCE_BUFFER_SUPPORT
#define MMISMS_SORT_CAHCE_BUFFER_SIZE (50*1024)

typedef struct MMISMS_SORT_CAHCE_tag
{
    uint32 sms_state;
    wchar *wstr_ptr;
    uint16 wstr_len;
    uint16 reserved;
    struct MMISMS_SORT_CAHCE_tag *next;
}MMISMS_SORT_CAHCE_T;

LOCAL uint8 *s_mmisms_sort_cache_buffer_ptr = PNULL;
LOCAL uint32 s_mmisms_sort_cache_buffer_pos = 0;

LOCAL MMISMS_SORT_CAHCE_T *s_mmisms_sort_cache_ptr = PNULL;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS��ȫ�ֱ���

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#if defined(BT_DIALER_SUPPORT) && (defined(BT_MAP_SUPPORT) || defined(MMI_SMS_SYN_SUPPORT))
/*****************************************************************************/
//     Description : CalculateTotalNum
//    Global resource dependence :  
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void CalculateBtTotalNum(void);
#endif

/*****************************************************************************/
//     Description : set the marked message in link locked or not
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL MMISMS_LOCK_FLAG_STATUS LockMarkedMsgInLink(MMISMS_ORDER_ID_T link_head,
                                                                                                       BOOLEAN is_lock);

/*****************************************************************************/
//     Description :������Ӧ�Ĺ�������˫������(�û���ѡ����������򣺰���ʱ�䡢��ϵ�ˡ����͡�����/����)
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL MMISMS_STATE_T* SortOrderList(
                                    MMISMS_STATE_T*        head,        //IN: �����������ͷ
                                    MMISMS_ORDERTYPE_E    oreder_type  //IN: �������
                                    );

/*****************************************************************************/
//     Description : �����������ͣ��Ƚ϶���״̬��
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareStateForSort (
                                 MMISMS_ORDERTYPE_E    oreder_type,      //IN    ��Ϣ�Ƚϵ����� 
                                 MMISMS_STATE_T        *sms1_state_ptr,  //IN: SMS1 state     
                                 MMISMS_STATE_T        *sms2_state_ptr   //IN: SMS2 state
                                 );

/*****************************************************************************/
// 	Description : is same long sms part
//	Global  resource dependence : 
//  Author: liming.deng
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsSameLongSmsPart(MMISMS_STATE_T *sms1_state_ptr, MMISMS_STATE_T *sms2_state_ptr);

/*****************************************************************************/
//     Description : �Ƚ�������Ϣ�Ƿ���ͬһ�������ŵĲ�ͬ����
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CompareSmsState(                                           //RETURN: 
                              MMISMS_STATE_T        *sms1_state_ptr,   //IN: SMS1 state 
                              MMISMS_TYPE_E            type,               //IN: MO or MT
                              MMISMS_STATE_T        *sms2_state_ptr    //IN: SMS2 state
                              );
/*****************************************************************************/
//     Description : ���ҵ�ǰ�ĳ�����Ϣ���Ƿ��Ѿ������µĶ���Ϣ�����ͬ��һ��
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN HaveSamePart(                                        //RETURN: 
                             MMISMS_STATE_T        *cur_state_ptr,    //IN: current SMS state
                             MMISMS_STATE_T     *new_state_ptr  //IN: new sms state
                             );
/*****************************************************************************/
//     Description : ���ҵ�ǰ�ĳ�����Ϣ���Ƿ��Ѿ������µĶ���Ϣ�����ͬ��һ��
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN HaveRepeatPart(                                        //RETURN: 
                             MMISMS_STATE_T    *cur_state_ptr,    //IN: current SMS state
                             MMISMS_TYPE_E        type,            //IN: MO or MT
                             MMISMS_STATE_T    *new_state_ptr  //IN: new sms state
                             );

/*****************************************************************************/
//     Description : to search the name and store info by number
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void FindNameAndStoreInfoByNum(
                                     uint8                num_type,            //IN:
                                     uint8                num_plan,            //IN:
                                     uint8                num_len,            //IN:
                                     uint8                *bcd_num_ptr,        //IN:
                                     uint8                string_limit_len,    //IN:
                                     MMISMS_MO_MT_TYPE_E  mo_mt_type,
                                     MMI_STRING_T        *name_ptr            //OUT:
                                     );

/*****************************************************************************/
//  Function Name: LinkedArrayInit
//  Description  : initial linked array
//    Global resource dependence : s_mmisms_linked_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN LinkedArrayInit(uint32 unit_num);

/*****************************************************************************/
//     Description : CalculateTotalNum
//    Global resource dependence :  
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void CalculateTotalNum(void);

/*****************************************************************************/
//     Description : ��ȡ������SIM SMS��Ŀ
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL uint16 HandleMaxSimSMSNum(void);

/*****************************************************************************/
//     Function name: InsertNode()
//  Description :  ��һ���ڵ���뵽������ȥ��
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNode(BOOLEAN        is_to_be_read,
                         MMISMS_STATE_T **insert_type_head_ptr_ptr, 
                         MMISMS_STATE_T *new_state_ptr, 
                         MMISMS_TYPE_E  type,
                         MMISMS_MSG_NUM_T *total_msg_num_ptr,
                         MMISMS_MSG_NUM_T *to_be_read_msg_num);

/*****************************************************************************/
//     Function name: DeleteNode()
//  Description :  ��һ���ڵ��������ɾ����
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteNode(BOOLEAN        is_to_be_read,
                         MMISMS_STATE_T **delete_type_head_ptr_ptr, 
                         MMISMS_STATE_T *sms_state_ptr,
                         MMISMS_TYPE_E  type,
                         MMISMS_MSG_NUM_T *total_msg_num_ptr,
                         MMISMS_MSG_NUM_T *to_be_read_msg_num);

/*****************************************************************************/
//     Function name: HandleLongSMSOrderMsgNum()
//  Description :  ���������е���Ϣ��Ŀ
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleLongSMSOrderMsgNum(
                             BOOLEAN is_to_be_read,
                             BOOLEAN is_insert, 
                             MMISMS_STATE_T *sms_state_ptr,
                             MMISMS_MSG_NUM_T *total_msg_num_ptr,
                             MMISMS_MSG_NUM_T *to_be_read_msg_num);

/*****************************************************************************/
//     Function name: HandleOrderNum()
//  Description :  ���������е���Ϣ��Ŀ
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleOrderMsgNum(
                             BOOLEAN is_to_be_read,
                             BOOLEAN is_insert, 
                             MMISMS_STATE_T *sms_state_ptr,
                             MMISMS_MSG_NUM_T *total_msg_num_ptr,
                             MMISMS_MSG_NUM_T *to_be_read_msg_num);

/*****************************************************************************/
//     Function name: InsertNodeAsNormalSms
//  Description :  ��һ���ڵ���Ϊ��ͨ����Ϣ�ڵ���뵽������ȥ��
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNodeAsNormalSms(MMISMS_STATE_T** insert_type_head_ptr,
                                    MMISMS_STATE_T *new_state_ptr,
                                    MMISMS_ORDERTYPE_E sort_type);

/*****************************************************************************/
//     Function name: InsertNodeAsLongSms
//  Description :  ��һ���ڵ���Ϊ������Ϣ�ڵ���뵽������ȥ��
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNodeAsLongSms(MMISMS_STATE_T** insert_type_head_ptr,
                                  MMISMS_STATE_T *new_state_ptr,
                                  MMISMS_STATE_T* first_longsms_ptr);

/*****************************************************************************/
//     Description : Find the node have the same ref in link list
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL MMISMS_STATE_T *FindNodeInLinkByRef(MMISMS_STATE_T *link_head_ptr, 
                                          MMISMS_STATE_T *new_sms_ptr, 
                                          MMISMS_TYPE_E type );

/*****************************************************************************/
//     Description : Form List from the link list head
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN FormListFromLinkhead(MMISMS_ORDER_ID_T head_order_id, 
                                                                           MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//     Description : �Ƚ�ʱ�䡣
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateByTime(const void*   node1,
                                                                      const void*   node2);

/*****************************************************************************/
//     Description : �Ƚ϶�����ϵ�ˡ�
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateByName(const void*   node1,
                                                                          const void*   node2);

/*****************************************************************************/
//     Description : �Ƚ����⡣
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateBySubject(const void*   node1,
                                                                             const void*   node2);

/*****************************************************************************/
//     Description : �Ƚ���Ϣ���͡�
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateByType(const void*   node1,
                                                                      const void*   node2);

/*****************************************************************************/
//     Description : get sms order link delete available num
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL uint32 GetOrderLinkDeleteAvailNum(MMISMS_ORDER_ID_T order_id);

/*****************************************************************************/
//     Description : GetOrderLinkDestNum
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void GetOrderLinkDestNum(MMISMS_ORDER_ID_T order_id,MMISMS_DEST_ADDR_INFO_T *dest_info_ptr);

/*****************************************************************************/
//     Description : GetOrderLinkOriginCard
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetOrderLinkOriginCard(MMISMS_ORDER_ID_T order_id, MN_DUAL_SYS_E *dual_sys);

/*****************************************************************************/
//     Description : GetOrderLinkMMSNum
//    Global resource dependence : 
//  Author: minghu.mao
//    Note:
/*****************************************************************************/
LOCAL uint16 GetOrderLinkMMSNum(MMISMS_ORDER_ID_T order_id, wchar** num_wstr, uint16 *num_len, uint16 num);

/*****************************************************************************/
//     Description : get sms order link move available num
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
LOCAL uint32 GetOrderLinkMoveAvailNum(MMISMS_ORDER_ID_T order_id);

/*****************************************************************************/
//  Description : compare bcd num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL int MMISMS_Bcdnumcmp(char * str1, char * str2, int length);

/*****************************************************************************/
//  Description : find a valid delete node
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T FindOrderNodeIdFromLink(MMISMS_ORDER_ID_T link_order_id);

/*****************************************************************************/
//  Function Name: IsSameSMS
//  Description  : compare two sms in order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSameSMS(
                        MMISMS_STATE_T *sms1_state_ptr,
                        MMISMS_STATE_T *sms2_state_ptr
                       );
/*****************************************************************************/
//  Function Name: CompareSMSNum
//  Description  : compare two sms num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareSMSNum(
                            MMIPB_BCD_NUMBER_T  *compare_number_ptr1,
                            MMIPB_BCD_NUMBER_T *compare_number_ptr2
                           );

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : insert node to chat order
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNodeToChatOrder(
                                    MMISMS_STATE_T *new_state_ptr,
                                    MMISMS_TYPE_E type
                                    );

/*****************************************************************************/
//  Function Name: LinkedArrayAllocNode
//  Description  : Alloc linked array
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_CHAT_STATE_UNIT_T*  LinkedChatAllocNode(void);

/*****************************************************************************/
//  Function Name: SetNewChatNode
//  Description  : set new chat node
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetNewChatNode(
                       MMISMS_CHAT_STATE_UNIT_T *unit_ptr,
                       MMISMS_STATE_T *new_state_ptr
                      );

/*****************************************************************************/
//  Function Name: InsertSameNumNodeToChatOrderByTime
//  Description  : set chat order by time
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InsertSameNumNodeToChatOrderByTime(
                                              MMISMS_CHAT_STATE_UNIT_T  *first_node_ptr,
                                              MMISMS_CHAT_STATE_UNIT_T  *number_head_ptr,
                                              MMISMS_STATE_T *new_state_ptr,
                                              MMISMS_TYPE_E type
                                             );

/*****************************************************************************/
//  Function Name: AdjustChatNode
//  Description  : adjust s_mmisms_chat_linked_order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void AdjustChatNode(
                          MMISMS_CHAT_STATE_UNIT_T *unit_ptr,
                          MMISMS_STATE_T *new_state_ptr
                         );

/*****************************************************************************/
//  Description : delete node from chat order
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteNodeFromChatOrder(
                                      MMISMS_STATE_T *delete_state_ptr,
                                      MMISMS_TYPE_E type
                                     );

/*****************************************************************************/
//  Function Name: DeleteChatNode
//  Description  : delete chat node
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteChatNode(
                              MMISMS_CHAT_STATE_UNIT_T  *first_node_ptr,
                              MMISMS_CHAT_STATE_UNIT_T *head_node_ptr,
                              MMISMS_STATE_T *delete_state_ptr,
                              MMISMS_TYPE_E type
                             );

/*****************************************************************************/
//  Function Name: SortChatLinkedOrderByTime
//  Description  : adjust s_mmisms_chat_linked_order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SortChatLinkedOrderByTime(void);

/*****************************************************************************/
//  Function Name: SortChatLinkedOrderByName
//  Description  : adjust s_mmisms_chat_linked_order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SortChatLinkedOrderByName(void);

/*****************************************************************************/
//  Function Name: GetChatNodeTotalUnreadNum
//  Description  : Get ChatNode Total Unread Num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetChatNodeTotalUnreadNum(void);

/*****************************************************************************/
//  Description : Form Chat List from the link list head
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FormChatListFromLinkhead(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : get sms chat order link delete available num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetChatOrderLinkDeleteAvailNum(void);

/*****************************************************************************/
//  Description : set chat order num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetChatOrderUnitNum(void);


/*****************************************************************************/
//  Description : find whether include unread sms
//  Global resource dependence 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindUnreadSMSInChatUnit(MMISMS_CHAT_STATE_UNIT_T *cur_chat_id);
#endif

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
//     Description : Form push message List from the link list head
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN FormPushMsgListFromLinkhead(MMISMS_ORDER_ID_T head_order_id, MMI_CTRL_ID_T ctrl_id);
#endif

/*****************************************************************************/
//  Description : Get Small Icon by sim index
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSimSmallIcon(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSmallIconByStatus(MMISMS_STATE_T *sms_state_ptr);
#endif

/*****************************************************************************/
//  Function Name: LinkedArrayFreeNode
//  Description  : free linked array
//    Global resource dependence : s_mmisms_linked_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN LinkedArrayFreeNode(MMISMS_STATE_T *state_ptr);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get sms box type(exclude security)
//	Global  resource dependence : 
//  Author: liming.deng
//	Note:   
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetSmsBoxtype(MMISMS_STATE_T *sms_state_ptr)
{
    MMISMS_BOX_TYPE_E sms_box_type = MMISMS_BOX_NONE;

    if (PNULL == sms_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSmsStateBoxtype sms_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_522_112_2_18_2_53_54_574,(uint8*)"");
        return sms_box_type;
    }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if(MMISMS_FOLDER_BT == sms_state_ptr->flag.folder_type)
    {
        if(MMISMS_MO_DRAFT == sms_state_ptr->flag.mo_mt_type)
        {
            sms_box_type = MMISMS_BT_BOX_NOSEND;
        }
        else if(MMISMS_MO_SEND_FAIL == sms_state_ptr->flag.mo_mt_type)
        {
            sms_box_type = MMISMS_BT_BOX_SENDFAIL;
        }
        else if(MMISMS_MO_SEND_SUCC == sms_state_ptr->flag.mo_mt_type)
        {
            sms_box_type = MMISMS_BT_BOX_SENDSUCC;
        }
        else if ((MMISMS_MT_SR_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_SR_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_NOTIFY == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NEED_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type))
        {
            sms_box_type = MMISMS_BT_BOX_MT;
        } 
        else
        {
            SCI_TRACE_LOW("MMISMS_GetSmsStateBoxtype mo_mt_type=%d", sms_state_ptr->flag.mo_mt_type);
        }
    }
    else	
#endif    

    if (MMISMS_MO_DRAFT == sms_state_ptr->flag.mo_mt_type)
    {
        sms_box_type = MMISMS_BOX_NOSEND;
    }
    else if (MMISMS_MO_SEND_FAIL == sms_state_ptr->flag.mo_mt_type)
    {
        sms_box_type = MMISMS_BOX_SENDFAIL;
    }
    else if (MMISMS_MO_SEND_SUCC == sms_state_ptr->flag.mo_mt_type)
    {
        sms_box_type = MMISMS_BOX_SENDSUCC;
    }
    else if ((MMISMS_MT_SR_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_SR_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_NOTIFY == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NEED_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type))
    {
        sms_box_type = MMISMS_BOX_MT;
    }
    else
    {
        //SCI_TRACE_MID:"GetSmsStateBoxtype Error mo_mt_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_555_112_2_18_2_53_54_575,(uint8*)"d", sms_state_ptr->flag.mo_mt_type);
    }

    return sms_box_type;
}

/*****************************************************************************/
// 	Description : get sms state box type(include  security)
//	Global  resource dependence : 
//  Author: liming.deng
//	Note:   
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetSmsStateBoxtype(MMISMS_STATE_T *sms_state_ptr)
{
    MMISMS_BOX_TYPE_E sms_box_type = MMISMS_BOX_NONE;

    if (PNULL == sms_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSmsStateBoxtype sms_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_522_112_2_18_2_53_54_574,(uint8*)"");
        return sms_box_type;
    }
    if (MMISMS_FOLDER_SECURITY == sms_state_ptr->flag.folder_type && MMISMS_TYPE_SMS == sms_state_ptr->flag.msg_type)
    {
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        sms_box_type = MMISMS_BOX_SECURITY;
#endif
    }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    else if(MMISMS_FOLDER_BT == sms_state_ptr->flag.folder_type)
    {
        if(MMISMS_MO_DRAFT == sms_state_ptr->flag.mo_mt_type)
        {
            sms_box_type = MMISMS_BT_BOX_NOSEND;
        }
        else if(MMISMS_MO_SEND_FAIL == sms_state_ptr->flag.mo_mt_type)
        {
            sms_box_type = MMISMS_BT_BOX_SENDFAIL;
        }
        else if(MMISMS_MO_SEND_SUCC == sms_state_ptr->flag.mo_mt_type)
        {
            sms_box_type = MMISMS_BT_BOX_SENDSUCC;
        }
        else if ((MMISMS_MT_SR_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_SR_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_NOTIFY == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NEED_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type))
        {
            sms_box_type = MMISMS_BT_BOX_MT;
        } 
        else
        {
            SCI_TRACE_LOW("MMISMS_GetSmsStateBoxtype mo_mt_type=%d", sms_state_ptr->flag.mo_mt_type);
        }
    }
#endif

    else if (MMISMS_MO_DRAFT == sms_state_ptr->flag.mo_mt_type)
    {
        sms_box_type = MMISMS_BOX_NOSEND;
    }
    else if (MMISMS_MO_SEND_FAIL == sms_state_ptr->flag.mo_mt_type)
    {
        sms_box_type = MMISMS_BOX_SENDFAIL;
    }
    else if (MMISMS_MO_SEND_SUCC == sms_state_ptr->flag.mo_mt_type)
    {
        sms_box_type = MMISMS_BOX_SENDSUCC;
    }
    else if ((MMISMS_MT_SR_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_SR_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_HAVE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_TO_BE_READ == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NOT_NOTIFY == sms_state_ptr->flag.mo_mt_type)
                || (MMISMS_MT_NEED_NOT_DOWNLOAD == sms_state_ptr->flag.mo_mt_type))
    {
        sms_box_type = MMISMS_BOX_MT;
    }
    else
    {
        //SCI_TRACE_MID:"GetSmsStateBoxtype Error mo_mt_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_555_112_2_18_2_53_54_575,(uint8*)"d", sms_state_ptr->flag.mo_mt_type);
    }

    return sms_box_type;
}

/*****************************************************************************/
// 	Description : is same long sms part
//	Global  resource dependence : 
//  Author: liming.deng
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsSameLongSmsPart(MMISMS_STATE_T *sms1_state_ptr, MMISMS_STATE_T *sms2_state_ptr)
{
    if (PNULL == sms1_state_ptr || PNULL == sms2_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:IsSameLongSmsPart sms1_state_ptr sms2_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_569_112_2_18_2_53_54_576,(uint8*)"");
        return FALSE;
    }
    // the same head reference number and the same head max number
    if ((sms2_state_ptr->head_ref == sms1_state_ptr->head_ref)   //the same ref number
        &&((sms2_state_ptr->longsms_seq_num -1)/MMISMS_SPLIT_MAX_NUM == (sms1_state_ptr->longsms_seq_num -1)/MMISMS_SPLIT_MAX_NUM) //the same group
        && (sms2_state_ptr->longsms_max_num == sms1_state_ptr->longsms_max_num) //the same max number
        //&&(sms2_state_ptr->flag.is_ucs2 == (BOOLEAN)(sms1_state_ptr->flag.is_ucs2))// the same alphabet
        &&(MMISMS_GetSmsStateBoxtype(sms2_state_ptr) == MMISMS_GetSmsStateBoxtype(sms1_state_ptr))//the same type
        )
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//     Description : �Ƚ���������״̬��Ϣ���ж��Ƿ���ͬһ�������ŵĲ�ͬ���֡�
//    Global  resource dependence : 
//  Author: liming.deng
//    Note:   ref_num,max_num, bcd_number(mt) �����ȫ��ͬ�򷵻�TRUE. 
/*****************************************************************************/
LOCAL BOOLEAN CompareSmsState(                                        //RETURN: 
                              MMISMS_STATE_T    *sms1_state_ptr,    //IN: SMS1 state 
                              MMISMS_TYPE_E        type,                //IN: MO or MT
                              MMISMS_STATE_T    *sms2_state_ptr     //IN: SMS2 state
                              )
{
    BOOLEAN result = FALSE;
    
    if (PNULL == sms1_state_ptr || PNULL == sms2_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:CompareSmsState sms1_state_ptr sms2_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_600_112_2_18_2_53_54_577,(uint8*)"");
        return FALSE;
    }
    if (MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type)
    {
        //SCI_TRACE_LOW:"MMISMS:CompareSmsState MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_605_112_2_18_2_53_54_578,(uint8*)"");
        return FALSE;
    }
    
    if (  (sms1_state_ptr->flag.dual_sys == sms2_state_ptr->flag.dual_sys)
        && (1 == sms1_state_ptr->flag.is_concatenate_sms)                // concatenated
        && (sms1_state_ptr->flag.msg_type == MMISMS_TYPE_SMS))    // is SMS,not mms
        
    {
        // the same head reference number and the same head max number
        if (IsSameLongSmsPart(sms1_state_ptr, sms2_state_ptr))            
        {
            if (MMISMS_MT_TYPE == type) 
            {
                // compare the display content
                // if the display content of MT SMS is not the sender's number, the code will be changed.
                if ((sms2_state_ptr->number.bcd_number.number_len == sms1_state_ptr->number.bcd_number.number_len)
                    && (0 == memcmp(sms2_state_ptr->number.bcd_number.number, 
                    sms1_state_ptr->number.bcd_number.number, 
                    sms2_state_ptr->number.bcd_number.number_len
                    )))
                {
                    result = TRUE;
                }
            }
            else
            {
                result = TRUE;
            }            
        }
    }    
    return (result);
}

/*****************************************************************************/
//     Function name: InsertNode()
//  Description :  ��һ���ڵ���뵽������ȥ,���޸����total numֵ��
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNode(BOOLEAN        is_to_be_read,
                         MMISMS_STATE_T **insert_type_head_ptr_ptr, 
                         MMISMS_STATE_T *new_state_ptr, 
                         MMISMS_TYPE_E  type,
                         MMISMS_MSG_NUM_T *total_msg_num_ptr,
                         MMISMS_MSG_NUM_T *to_be_read_msg_num)
{
    MMISMS_STATE_T *cur_ptr = PNULL ;
    MMISMS_STATE_T *first_longsms_ptr = PNULL ;
    
    //SCI_TRACE_LOW:"InsertNode:new_state_ptr->longsms_max_num = %d,new_state_ptr->longsms_seq_num = %d,new_state_ptr->head_ref = %d,msg_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_655_112_2_18_2_53_54_579,(uint8*)"dddd",new_state_ptr->longsms_max_num, new_state_ptr->longsms_seq_num, new_state_ptr->head_ref, new_state_ptr->flag.msg_type);
    
    if (*insert_type_head_ptr_ptr == PNULL)
    {
        *insert_type_head_ptr_ptr = new_state_ptr;    //���²���Ľڵ���Ϊ�����ͷ        
        
        HandleOrderMsgNum(is_to_be_read,
                                        TRUE,
                                        new_state_ptr,
                                        total_msg_num_ptr,
                                        to_be_read_msg_num);
    }
    else
    {
        if (!new_state_ptr->flag.is_concatenate_sms) //is normal sms,insert linked array according current sort type
        {
            InsertNodeAsNormalSms(insert_type_head_ptr_ptr,new_state_ptr, MMISMS_GetOrderType());
            
            HandleOrderMsgNum(is_to_be_read,
                                            TRUE,
                                            new_state_ptr,
                                            total_msg_num_ptr,
                                            to_be_read_msg_num);
        }
        else //is long sms
        {
            cur_ptr = *insert_type_head_ptr_ptr;
            
            //cur_ptr�ƶ���ĳһ��㣬ʹҪ�����ʱ�䲻С�ڵ�ǰ��ָ��ʱ�䣬����cur_ptrδָ�����һ�����
            while(cur_ptr != PNULL)                 
            {
                if(CompareSmsState(cur_ptr, type,new_state_ptr) //��ͬref num������
                    && (!cur_ptr->flag.is_receive_all)          //����û�н�������
                    && (!HaveRepeatPart(cur_ptr,type,new_state_ptr)))//������û�к��¶���Ϣ�ظ��Ĳ���
                {
                    break;
                }
                else
                {
                    cur_ptr = cur_ptr->next_ptr;
                }
            }
            
            if (cur_ptr!=PNULL) //�ҵ�ref��ͬ��λ��,��û�н�����������û���ظ��Ĳ���
            {
                first_longsms_ptr = cur_ptr;
                InsertNodeAsLongSms(insert_type_head_ptr_ptr,new_state_ptr,first_longsms_ptr);

                HandleLongSMSOrderMsgNum(is_to_be_read,
                                                            TRUE,
                                                            new_state_ptr,
                                                            total_msg_num_ptr,
                                                            to_be_read_msg_num);
            }
            else //û���ҵ�ref��ͬ��λ�ã����ǳ������е�һ�������
            {
                cur_ptr = *insert_type_head_ptr_ptr;
                
                //cur_ptr�ƶ���ĳһ��㣬ʹҪ�����ʱ�䲻С�ڵ�ǰ��ָ��ʱ�䣬����cur_ptrδָ�����һ�����
                while(cur_ptr != PNULL)                 
                {
                    if(HaveRepeatPart(cur_ptr,type,new_state_ptr))//�������к��¶���Ϣ�ظ��Ĳ���
                    {
                        
                        MNSMS_UpdateSmsStatusEx(new_state_ptr->flag.dual_sys,new_state_ptr->flag.storage,new_state_ptr->record_id,MN_SMS_FREE_SPACE);
                        
                        SCI_TRACE_LOW("Received a same message as the part of long message,we will dismiss it");

                        LinkedArrayFreeNode(new_state_ptr);

                        return TRUE;
                    }
                    else
                    {
                        cur_ptr = cur_ptr->next_ptr;
                    }
                }

                InsertNodeAsNormalSms(insert_type_head_ptr_ptr,new_state_ptr,MMISMS_GetOrderType());
                
                HandleOrderMsgNum(is_to_be_read,
                                                TRUE,
                                                new_state_ptr,
                                                total_msg_num_ptr,
                                                to_be_read_msg_num);
            }    
        }
    }
    
    return TRUE;    
}

/*****************************************************************************/
//     Function name: InsertNodeAsNormalSms
//  Description :  ��һ���ڵ���Ϊ��ͨ����Ϣ�ڵ���뵽������ȥ��
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNodeAsNormalSms(MMISMS_STATE_T** insert_type_head_ptr,
                                    MMISMS_STATE_T *new_state_ptr,
                                    MMISMS_ORDERTYPE_E sort_type)
{
    MMISMS_STATE_T *cur_ptr = PNULL;
    
    cur_ptr = *insert_type_head_ptr;
    //cur_ptr�ƶ���ĳһ��㣬ʹҪ���������������С�ڵ�ǰ��ָ����������������cur_ptrδָ�����һ�����
    while(cur_ptr->next_ptr!=PNULL 
        && (1 == CompareStateForSort(sort_type, new_state_ptr,cur_ptr )))
    {
        cur_ptr = cur_ptr->next_ptr;
    }    
    
    //���뵽current���� (ע: �����current���棬�϶������)
    if(1 == CompareStateForSort(sort_type, new_state_ptr,cur_ptr )) 
    {
        new_state_ptr->next_ptr = cur_ptr->next_ptr;
        cur_ptr->next_ptr = new_state_ptr;
        new_state_ptr->prev_ptr = cur_ptr;
    }
    else//���뵽current�ڵ�ǰ��
    {
        if (cur_ptr->prev_ptr == PNULL) //��ǰ�ڵ�Ϊ����ͷ
        {
            new_state_ptr->next_ptr = cur_ptr;
            new_state_ptr->prev_ptr = PNULL;
            
            cur_ptr->prev_ptr = new_state_ptr;
            
            *insert_type_head_ptr = new_state_ptr;
        }
        else
        {
            new_state_ptr->prev_ptr = cur_ptr->prev_ptr;
            cur_ptr->prev_ptr->next_ptr = new_state_ptr;
            new_state_ptr->next_ptr = cur_ptr;
            cur_ptr->prev_ptr = new_state_ptr;
        }        
    }
    return TRUE;    
}

/*lint -e794*/
/*lint -e831*/
/*****************************************************************************/
//     Function name: InsertNodeAsLongSms
//  Description :  ��һ���ڵ���Ϊ������Ϣ�ڵ���뵽������ȥ��
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNodeAsLongSms(MMISMS_STATE_T **insert_type_head_ptr,
                                  MMISMS_STATE_T *new_state_ptr,
                                  MMISMS_STATE_T *first_longsms_ptr) 
{
    MMISMS_STATE_T *cur_ptr = PNULL;
    MMISMS_STATE_T *pre_longsms_ptr = PNULL;
    uint16 received_num = 0;
    
    cur_ptr = first_longsms_ptr;
    
    received_num ++; 
    
    while (cur_ptr != PNULL)
    {  
        received_num ++;
        cur_ptr = cur_ptr->next_long_sms_ptr;
    }
    
    cur_ptr = first_longsms_ptr;
    if ( (received_num == cur_ptr->longsms_max_num) 
        || (received_num == MMISMS_SPLIT_MAX_NUM) )
    {
        new_state_ptr->flag.is_receive_all = TRUE;
        while (cur_ptr != PNULL)
        {  
            cur_ptr->flag.is_receive_all = TRUE;
            cur_ptr = cur_ptr->next_long_sms_ptr;
        }        
    }
    
    cur_ptr = first_longsms_ptr;
    //��ǰseq num �����������ϵ�seq num,���·�
    if (new_state_ptr->longsms_seq_num >= cur_ptr->longsms_seq_num)
    {
        cur_ptr = first_longsms_ptr;
        while ((cur_ptr->next_long_sms_ptr != PNULL)
            &&(new_state_ptr->longsms_seq_num >= cur_ptr->longsms_seq_num))
        {
            pre_longsms_ptr = cur_ptr;
            cur_ptr = cur_ptr->next_long_sms_ptr;
        }
        
        if (new_state_ptr->longsms_seq_num < cur_ptr->longsms_seq_num)
        {
            pre_longsms_ptr->next_long_sms_ptr = new_state_ptr;
            new_state_ptr->next_long_sms_ptr = cur_ptr;
            new_state_ptr->prev_ptr = PNULL;
            new_state_ptr->next_ptr = PNULL;                        
        }
        else //put the new sms_ptr to the tail
        {
            cur_ptr->next_long_sms_ptr = new_state_ptr;
            new_state_ptr->prev_ptr = PNULL;
            new_state_ptr->next_ptr = PNULL;
            new_state_ptr->next_long_sms_ptr = PNULL;
        }
        
    }
    else
    {
        if (new_state_ptr->longsms_seq_num < cur_ptr->longsms_seq_num)
        {
            //��ǰseq num С���������ϵ�seq num, ȡ����ǰ�ڵ㣬�ŵ���������.
            new_state_ptr->prev_ptr = first_longsms_ptr->prev_ptr;
            new_state_ptr->next_ptr = first_longsms_ptr->next_ptr;
            if (first_longsms_ptr->prev_ptr != PNULL)
            {
                first_longsms_ptr->prev_ptr->next_ptr = new_state_ptr;//ԭ���ڵ㲻������ͷ
            }
            else
            {
                *insert_type_head_ptr = new_state_ptr; //ԭ���ڵ�Ϊ����ͷ
            }
            
            
            if (first_longsms_ptr->next_ptr != PNULL)
            {
                first_longsms_ptr->next_ptr->prev_ptr = new_state_ptr;
            }
            
            new_state_ptr->next_long_sms_ptr = first_longsms_ptr;
            first_longsms_ptr->prev_ptr = PNULL;
            first_longsms_ptr->next_ptr = PNULL;
        }
    }
    
    return TRUE;
}

/*lint +e794*/
/*lint +e831*/

/*****************************************************************************/
//     Description : to check whether the status is MMS
//    Global resource dependence : nsms1_state_ptr
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_MSG_TYPE_E MMISMS_GetMsgType(void)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_SMS;
    MMISMS_ORDER_ID_T cur_order_id   = PNULL;
    
    MMISMS_SetCurOrderArrIndex(0);
    cur_order_id = MMISMS_GetCurOperationOrderId();
    if (PNULL == cur_order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetMsgType cur_order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_883_112_2_18_2_53_55_580,(uint8*)"");
        return MMISMS_TYPE_MAX;
    }  
    
    msg_type = (MMISMS_MSG_TYPE_E)cur_order_id->flag.msg_type;
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetMsgType msg_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_888_112_2_18_2_53_55_581,(uint8*)"d", msg_type);
    return msg_type;
}

/*****************************************************************************/
//     Description : mark the message
//    Global resource dependence : nsms1_state_ptr
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurMsgMarked(
                                   BOOLEAN is_marked //IN:
                                   )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    MMISMS_SetCurOrderArrIndex(0);
    order_id = MMISMS_GetCurOperationOrderId();    
    if (PNULL == order_id)
    {
        return;
    }        
    cur_order_id = order_id;
    while (cur_order_id != PNULL)
    {
        cur_order_id->flag.is_marked = is_marked;
        cur_order_id = cur_order_id->next_long_sms_ptr;
    }
}

/*****************************************************************************/
//     Description : get the message is marked or not
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetCurMsgIsMarked(void )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    BOOLEAN           is_marked = FALSE;
    MMISMS_SetCurOrderArrIndex(0);
    order_id = MMISMS_GetCurOperationOrderId(); 
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetCurMsgIsMarked order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_929_112_2_18_2_53_55_582,(uint8*)"");
        return FALSE;
    }  
    is_marked = order_id->flag.is_marked;
    
    return is_marked;
}

/*****************************************************************************/
//     Description : mark all message
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAllMsgMarked(
                                   MMISMS_BOX_TYPE_E box_type,
                                   BOOLEAN           is_marked //IN:
                                   )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    
    switch(box_type) 
    {
    case MMISMS_BOX_MT:
        cur_order_id =s_mmisms_order.mt_undownloaded_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        cur_order_id =s_mmisms_order.mt_unread_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        cur_order_id =s_mmisms_order.mt_readed_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        break;
        
    case MMISMS_BOX_SENDSUCC:
        cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        break;
        
    case MMISMS_BOX_SENDFAIL:
        cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        break;
        
    case MMISMS_BOX_NOSEND:
        cur_order_id =s_mmisms_order.mo_draft_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        cur_order_id =s_mmisms_order.security_mt_undownloaded_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        cur_order_id =s_mmisms_order.security_head_ptr;
        while (cur_order_id != PNULL)
        {
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
    case MMISMS_BOX_CHAT:
        {
            MMISMS_CHAT_STATE_UNIT_T *num_ptr = PNULL;
            uint32 while_count = 0;
            uint32 total_msg_num = s_mmisms_order.max_sms_num.total_msg_num;   //fixed bug147321 ��ֹ��ѭ��

            num_ptr = s_mmisms_chat_linked_order.head_ptr;

            while(PNULL != num_ptr)
            {
                cur_order_id = num_ptr->head_ptr;
                while (cur_order_id != PNULL)
                {   
                    while_count ++;

                    cur_long_order_id = cur_order_id;
                    while (cur_long_order_id != PNULL)
                    {
                        cur_long_order_id->flag.is_marked = is_marked;
                        cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                    }
                    cur_order_id = cur_order_id->next_chat_ptr;
                    if(cur_order_id != PNULL && cur_order_id == cur_order_id->next_chat_ptr)
                    {
                        SCI_TRACE_LOW("MMISMS link array error MMISMS_SetAllMsgMarked");
                        break;
                    } 

                    if (while_count > total_msg_num)
                    {
                        SCI_TRACE_LOW("MMISMS link array error while_count exceed total_msg_num MMISMS_SetAllMsgMarked");
                        break;
                    }
                }
                num_ptr = num_ptr->next_number_ptr;
            }
        }
        break;
#endif
        
    default:
        break;
    }
}

/*****************************************************************************/
//     Description : mark point message
//    Global resource dependence : nsms1_state_ptr
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetPointedMsgMarked(
                                    MMISMS_BOX_TYPE_E box_type,
                                    MN_DUAL_SYS_E dual_sys,
                                    MN_SMS_STORAGE_E storage,
                                    uint16 index
                                    )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    BOOLEAN is_do_mark = FALSE;

    if (dual_sys >=  MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SetPointedMsgMarked dual_sys >=  MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1113_112_2_18_2_53_56_583,(uint8*)"");
        return FALSE;
    }
    //����pos_index��õ�ǰ��order_id 
    order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type, index); 
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SetPointedMsgMarked order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1120_112_2_18_2_53_56_584,(uint8*)"");
        return FALSE;
    }  

    cur_order_id = order_id;

    while (cur_order_id != PNULL)
    {
        if (MN_SMS_STORAGE_SIM == storage)
        {
            if ((storage == cur_order_id->flag.storage)
                && (dual_sys == cur_order_id->flag.dual_sys)
                && !cur_order_id->flag.is_marked)
            {
                cur_order_id->flag.is_marked = TRUE;
                is_do_mark = TRUE;
            }
        }
        else if (MN_SMS_STORAGE_ME == storage)
        {
            if ((storage == cur_order_id->flag.storage)
                && !cur_order_id->flag.is_marked)
            {
                cur_order_id->flag.is_marked = TRUE;
                is_do_mark = TRUE;
            }
        }

        cur_order_id = cur_order_id->next_long_sms_ptr;
    }

    return is_do_mark;
}

/*****************************************************************************/
//     Description : to get the storage of pointed SMS
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetPointedSMSMemInfo(                                    //RETURN:
                                           MMISMS_ORDER_ID_T    order_id,            //IN:
                                           MN_DUAL_SYS_E        *dual_sys,
                                           MN_SMS_STORAGE_E        *storage_ptr,    //OUT:
                                           MN_SMS_RECORD_ID_T    *record_id_ptr    //OUT:
                                           )
{
    BOOLEAN result = TRUE;
    
    if (PNULL == storage_ptr || PNULL == record_id_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetPointedSMSMemInfo storage_ptr record_id_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1169_112_2_18_2_53_56_585,(uint8*)"");
        return FALSE;
    }
    if (0 == s_mmisms_order.max_sms_num.total_msg_num)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetPointedSMSMemInfo 0 = s_mmisms_order.max_sms_num.total_msg_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1174_112_2_18_2_53_56_586,(uint8*)"");
        return FALSE;
    }
    
    if (order_id != PNULL)
    {
        (*storage_ptr)   = (MN_SMS_STORAGE_E)order_id->flag.storage;
        (*record_id_ptr) = (MN_SMS_RECORD_ID_T)order_id->record_id;
        (*dual_sys)         = (MN_DUAL_SYS_E)order_id->flag.dual_sys;
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetPointedSMSMemInfo: order_id is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1187_112_2_18_2_53_56_587,(uint8*)"");
        result = FALSE;
    }
    return (result);
}


/*****************************************************************************/
//     Description : get the order id by storage and record_id .
//    Global resource dependence : 
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetOrderIdByStorage(
                                          MMISMS_ORDER_ID_T         *order_id,          //OUT:
                                          MMISMS_MSG_TYPE_E         msg_type,           //IN:
                                          MN_DUAL_SYS_E                dual_sys,           //IN:
                                          MN_SMS_STORAGE_E            storage,            //IN:
                                          int32                     record_id            //IN:
                                          )
{
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id = PNULL;
    
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetOrderIdByStorage order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1210_112_2_18_2_53_56_588,(uint8*)"");
        return FALSE;
    }    
    cur_unit_id = s_mmisms_linked_order.used_unit_head_ptr;
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetOrderIdByStorage msg_type = %d,storage = %d, record_id= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1216_112_2_18_2_53_56_589,(uint8*)"ddd",msg_type, storage, record_id);
    
    while ((cur_unit_id != PNULL))
    {
        if (MMISMS_TYPE_SMS == msg_type)
        {
            if ((storage == cur_unit_id->sms_state.flag.storage) &&
                (record_id == cur_unit_id->sms_state.record_id))
            {
                if(MN_SMS_STORAGE_ME == storage)
                {
                    break;
                }
                else if((MN_SMS_STORAGE_SIM == storage) &&
                        (dual_sys == cur_unit_id->sms_state.flag.dual_sys))
                {
                    break;
                }
                else
                {
                    cur_unit_id = cur_unit_id->next_ptr;
                }
            }
            else
            {
                cur_unit_id = cur_unit_id->next_ptr;
            }            
        }
        else
        {
            if ((record_id == cur_unit_id->sms_state.record_id) 
                && (msg_type == cur_unit_id->sms_state.flag.msg_type)) //MMS,WAP PUSH,OTA���Ƚ�record id��msg type
            {
                break;
            }
            else
            {
                cur_unit_id = cur_unit_id->next_ptr;
            }            
        }        
    }
    if (cur_unit_id != PNULL)
    {
        *order_id = &(cur_unit_id->sms_state);
        return TRUE;
    }
    else
    {
        order_id = PNULL;
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetOrderIdByStorage: Can't find the order id by storage and record id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1265_112_2_18_2_53_56_590,(uint8*)"");
        return FALSE;
    }    
}

/*****************************************************************************/
//     Description : to delete SMS info from order list by storage
//    Global resource dependence : 
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DelSMSByStore(                                            //RETURN:
                                    BOOLEAN                 is_mt_sms,            //IN:
                                    MN_DUAL_SYS_E            dual_sys,           //IN:
                                    MN_SMS_STORAGE_E        storage,            //IN:
                                    MN_SMS_RECORD_ID_T        record_id            //IN:
                                    )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    MMISMS_GetOrderIdByStorage(&order_id, MMISMS_TYPE_SMS, dual_sys, storage, record_id);
    
    if (order_id != PNULL)
    {
        MMISMS_DelSmsInfoInOrder(order_id);
        MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//     Description : to delete SMS info form order list by AT
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DelSMSByAT(                                            //RETURN:
                                 MN_DUAL_SYS_E         dual_sys,
                                 BOOLEAN                 is_mt_sms,            //IN:
                                 BOOLEAN                is_saved_in_sim,    //IN:
                                 MN_SMS_RECORD_ID_T    record_id            //IN:
                                 )
{
    MN_SMS_STORAGE_E storage = MN_SMS_STORAGE_SIM;
    MN_SMS_STATUS_E  *status_ptr = PNULL;
    uint16 max_sms_num = 0;
    uint16 used_sms_num= 0;
    BOOLEAN result = FALSE;
    
    //SCI_TRACE_LOW:"MMISMS_DelSMSByAt(%d, %d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1314_112_2_18_2_53_56_591,(uint8*)"ddd", is_mt_sms, is_saved_in_sim, record_id);
    
    if (MMISMS_IsOrderOk())
    {    
        storage = (is_saved_in_sim) ? MN_SMS_STORAGE_SIM : MN_SMS_STORAGE_ME;

        MNSMS_GetSmsNumEx(dual_sys, storage, &max_sms_num, &used_sms_num);
        
        status_ptr = SCI_ALLOCAZ(max_sms_num * sizeof(MN_SMS_STATUS_E));
        if (PNULL == status_ptr)
        {
            //SCI_TRACE_LOW:"MMISMS:MMISMS_DelSMSByAT status_ptr = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1325_112_2_18_2_53_56_592,(uint8*)"");
            return FALSE;
        }
        //SCI_MEMSET(status_ptr, 0, (max_sms_num * sizeof(MN_SMS_STATUS_E)));
        
        //find next record in SIM for order
        MNSMS_GetSmsStatusEx(dual_sys, storage, &max_sms_num, status_ptr);
        
        if ( MN_SMS_FREE_SPACE != status_ptr[record_id])
        {
            
            MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, MN_SMS_FREE_SPACE);
        }
        
        result = MMISMS_DelSMSByStore(is_mt_sms, dual_sys, storage, record_id);
        
        // free the space
        SCI_FREE(status_ptr);
    }
    
    return (result);
}


/*****************************************************************************/
//     Description : translate MN status to SF status
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_MO_MT_TYPE_E MMISMS_TranslateMNStatus2SFStatus(                                    //RETURN:
                                                             MN_SMS_STATUS_E mn_status,        //IN:
                                                             MMISMS_TYPE_E type,            //IN:
                                                             BOOLEAN is_sc_addr    //IN: for MT, the parameter will be invalid
                                                             )
{
    MMISMS_MO_MT_TYPE_E sf_status = MMISMS_MT_NOT_DOWNLOAD;
    
    if (MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_TranslateMNStatus2SFStatus MMISMS_MT_TYPE != type && MMISMS_MO_TYPE != type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1363_112_2_18_2_53_56_593,(uint8*)"");
        return sf_status;
    }
    
    if (MMISMS_MT_TYPE == type)
    {
        switch (mn_status)
        {
        case MN_SMS_MT_TO_BE_READ:
            sf_status = MMISMS_MT_TO_BE_READ;
            break;
            
        case MN_SMS_MT_SR_TO_BE_READ:
            sf_status = MMISMS_MT_SR_TO_BE_READ;
            break;
            
        case MN_SMS_MT_READED:
            sf_status = MMISMS_MT_HAVE_READ;
            break;
            
        case MN_SMS_MT_SR_READED:
            sf_status = MMISMS_MT_SR_HAVE_READ;
            break;
            
        default:
            //SCI_TRACE_LOW:"SMS: MMISMS_TranslateMNStatus2SFStatus ERROR mn_status = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1388_112_2_18_2_53_56_594,(uint8*)"d", mn_status);
            break;
        }
    }
    else
    {
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMode())
        {
            if (MMISMS_BOX_NOSEND != MMISMS_TIMERMSG_GetCurrBoxType())
            {
                is_sc_addr = TRUE;
            }
        }
#endif

        switch (mn_status) 
        {
        case MN_SMS_MO_TO_BE_SEND:
            sf_status = (is_sc_addr) ? MMISMS_MO_SEND_FAIL: MMISMS_MO_DRAFT;
            break;
            
        case MN_SMS_MO_SR_NOT_REQUEST:          
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
            sf_status = MMISMS_MO_SEND_SUCC;
            break;
            
        default:
            //SCI_TRACE_LOW:"SMS: MMISMS_TranslateMNStatus2SFStatus ERROR mn_status = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1418_112_2_18_2_53_56_595,(uint8*)"d", mn_status);
            break;
        }
    }
    
    return (sf_status);
}

/*****************************************************************************/
//  Function Name: LinkedArrayInit
//  Description  : initial linked array
//    Global resource dependence : s_mmisms_linked_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN LinkedArrayInit(uint32 unit_num)
{
    uint32 i = 0;
#ifdef MMI_SMS_CHAT_SUPPORT    
    s_mmisms_chat_linked_order.uint_num = 0;
    s_mmisms_chat_linked_order.unread_unit_num = 0;
    s_mmisms_chat_linked_order.head_ptr = PNULL;
#endif

    if (unit_num > 0)
    {
        uint32 link_order_unit_size = sizeof(MMISMS_LINKED_ORDER_UNIT_T);
#ifdef MMI_SMS_CHAT_SUPPORT        
        uint32 link_chat_order_unit_size = sizeof(MMISMS_CHAT_STATE_UNIT_T);
#endif

        if (unit_num > s_mmisms_linked_order.unit_num && PNULL != s_mmisms_linked_order.head_ptr)
        {            
            SCI_FREE(s_mmisms_linked_order.head_ptr);

            s_mmisms_linked_order.head_ptr = PNULL;
        }

        if (PNULL == s_mmisms_linked_order.head_ptr)
        {
            s_mmisms_linked_order.unit_num = unit_num;

            s_mmisms_linked_order.head_ptr = SCI_ALLOC_CONST(unit_num * link_order_unit_size);
        }

#ifdef MMI_SMS_CHAT_SUPPORT
        if (PNULL != s_mmisms_chat_linked_order.head_node_ptr)
        {
            SCI_FREE(s_mmisms_chat_linked_order.head_node_ptr);
            s_mmisms_chat_linked_order.head_node_ptr = PNULL;
        }
#endif        
        SCI_MEMSET(s_mmisms_linked_order.head_ptr, 0, (unit_num * link_order_unit_size));

#ifdef MMI_SMS_CHAT_SUPPORT
        if (PNULL == s_mmisms_chat_linked_order.head_node_ptr)
        {
            s_mmisms_chat_linked_order.head_node_ptr = SCI_ALLOC_CONST(unit_num * link_chat_order_unit_size);
            SCI_MEMSET(s_mmisms_chat_linked_order.head_node_ptr, 0, (unit_num * link_chat_order_unit_size)); 
        }
#endif        
    }
    else
    { 
        //do nothing

        if ( s_mmisms_linked_order.head_ptr != PNULL)
        {
            SCI_FREE(s_mmisms_linked_order.head_ptr);
            s_mmisms_linked_order.head_ptr = PNULL;
        }
#ifdef MMI_SMS_CHAT_SUPPORT
        if ( s_mmisms_chat_linked_order.head_node_ptr != PNULL)
        {
            SCI_FREE(s_mmisms_chat_linked_order.head_node_ptr);
            s_mmisms_chat_linked_order.head_node_ptr = PNULL;
        }
#endif        
        return FALSE;
    }
    
    s_mmisms_linked_order.used_unit_head_ptr = PNULL;
    s_mmisms_linked_order.unused_unit_head_ptr = s_mmisms_linked_order.head_ptr;    
    
    for (i = 0; i< (unit_num-1); i++)
    {
        s_mmisms_linked_order.head_ptr[i].next_ptr = &(s_mmisms_linked_order.head_ptr[i+1]);
    }
    if(unit_num > 0)
    {
        s_mmisms_linked_order.head_ptr[unit_num -1].next_ptr = PNULL;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Function Name: LinkedArrayAllocNode
//  Description  : Alloc linked array
//    Global resource dependence : s_mmisms_linked_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL MMISMS_STATE_T*  LinkedArrayAllocNode(void)
{    
    MMISMS_LINKED_ORDER_UNIT_T* unit_ptr = PNULL;
    
    unit_ptr = s_mmisms_linked_order.unused_unit_head_ptr; //����δ��unit����
    
    if ( unit_ptr != PNULL)
    {
        //delete from unused link
        s_mmisms_linked_order.unused_unit_head_ptr = unit_ptr->next_ptr;
        
        //add to used link as the head
        unit_ptr->next_ptr = s_mmisms_linked_order.used_unit_head_ptr;
        
        s_mmisms_linked_order.used_unit_head_ptr = unit_ptr;
        
        return &(unit_ptr->sms_state);
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS LinkedArrayAllocNode: No array unit to alloc! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1537_112_2_18_2_53_56_596,(uint8*)"");
        return PNULL;
    }
}

/*****************************************************************************/
//  Function Name: LinkedArrayFreeNode
//  Description  : free linked array
//    Global resource dependence : s_mmisms_linked_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN LinkedArrayFreeNode(MMISMS_STATE_T *state_ptr)
{
    MMISMS_LINKED_ORDER_UNIT_T *cur_ptr = s_mmisms_linked_order.used_unit_head_ptr;
    MMISMS_LINKED_ORDER_UNIT_T *pre_ptr = PNULL;
    
    if (cur_ptr == PNULL) 
    {
        //SCI_TRACE_LOW:"MMISMS: LinkedArrayFreeNode : s_mmisms_linked_order.used_unit_head_ptr is PNULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1554_112_2_18_2_53_56_597,(uint8*)"");
        return TRUE;
    }
    else
    {    
        while((cur_ptr != PNULL)&&((&cur_ptr->sms_state) != state_ptr) )
        {
            pre_ptr = cur_ptr;
            cur_ptr = cur_ptr->next_ptr;
        }        
    }
    
    if (cur_ptr != PNULL) //�ҵ�����
    {
        //�����������������ɾ��
        if (pre_ptr != PNULL) //cur_ptr ���ǵ�һ��
        {
            pre_ptr->next_ptr =cur_ptr->next_ptr; 
            
        }
        else    //cur_ptrΪ��һ��
        {
            s_mmisms_linked_order.used_unit_head_ptr = cur_ptr->next_ptr;
        }
        
        //��������뵽δ��������
        cur_ptr->next_ptr = s_mmisms_linked_order.unused_unit_head_ptr;
        s_mmisms_linked_order.unused_unit_head_ptr = cur_ptr;
        return TRUE;        
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: LinkedArrayFreeNode : can't find the node in used link!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1586_112_2_18_2_53_56_598,(uint8*)"");
    }
    return TRUE;
}

/*****************************************************************************/
//     Function name: MMISMS_GetMaxSmsNumInOrder ()
//  Description :  �õ�s_mmisms_order�е����sms��Ŀ
//    Global resource dependence: s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetMaxSmsNumInOrder(void)
{
    return s_mmisms_order.max_sms_num.total_msg_num;
}

/*****************************************************************************/
//     Function name: MMISMS_GetUndownloadSmsNumInOrder ()
//  Description :  �õ�s_mmisms_order�е�δ���ص�mms��Ŀ
//    Global resource dependence: s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUndownloadMMSNumInOrder(void)
{
    return s_mmisms_order.total_undownloaded_num.total_msg_num;
}

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadSmsCount(MN_DUAL_SYS_E dual_sys)
{
    uint16 ret_val = 0;

    uint16 update_sms_num = 0;

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        ret_val = s_mmisms_order.to_be_read_msg_num[MMISMS_TYPE_SMS].total_msg_num;//δ��������Ŀ
        SCI_TRACE_LOW("MMISMS_GetTobeReadSmsCount sms unread count=%d",ret_val);
    }
    else //if(MN_DUAL_SYS_1 <= dual_sys && dual_sys < MMI_DUAL_SYS_MAX)
    {
        ret_val = s_mmisms_order.to_be_read_msg_num[MMISMS_TYPE_SMS].msg_num[dual_sys]; //��X δ�����ŵ���Ŀ
    }

    if (g_mmisms_global.is_need_update_order 
#ifdef MMI_SMS_CHAT_SUPPORT        
        && !g_mmisms_global.is_keylock_need_count_sms 
#endif
    )
    {
        
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        MMISMS_ORDER_ID_T cur_order_id = MMISMS_GetCurOperationOrderId();
        if(PNULL != cur_order_id && MMISMS_FOLDER_BT == cur_order_id->flag.folder_type) 
        {
           update_sms_num = MMISMS_GetUpdateBtSMSNum(dual_sys);
        }
        else
#endif
{
           update_sms_num = MMISMS_GetUpdateSMSNum(dual_sys);
}
        
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetTobeReadSmsCount ret_val=%d, update_sms_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1632_112_2_18_2_53_57_599,(uint8*)"dd", ret_val, update_sms_num);
        
        if (ret_val >= update_sms_num)
        {
           ret_val -= update_sms_num;
        }
        else
        {
           ret_val = 0;
        }
    }

    SCI_TRACE_LOW("MMISMS_GetTobeReadSmsCount sms total unread count=%d",ret_val);
    return ret_val;
}

/*****************************************************************************/
//     Description : to get the count of msg which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadMsgCount(MN_DUAL_SYS_E dual_sys, MMISMS_MSG_TYPE_E msg_type)
{
    uint16 ret_val = 0;
    uint16 update_sms_num = 0;

    SCI_TRACE_LOW("MMISMS_GetTobeReadMsgCount msg_type = %d",msg_type);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        ret_val =s_mmisms_order.to_be_read_msg_num[msg_type].total_msg_num;//δ����Ϣ��Ŀ
    }
    else //if(MN_DUAL_SYS_1 <= dual_sys && dual_sys < MMI_DUAL_SYS_MAX)
    {
        ret_val = s_mmisms_order.to_be_read_msg_num[msg_type].msg_num[dual_sys]; //��X δ����Ϣ����Ŀ
    }

    if (MMISMS_TYPE_SMS == msg_type 
        && g_mmisms_global.is_need_update_order
#ifdef MMI_SMS_CHAT_SUPPORT        
        && !g_mmisms_global.is_keylock_need_count_sms 
#endif        
        )
    {

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        MMISMS_ORDER_ID_T cur_order_id = MMISMS_GetCurOperationOrderId();
        if(PNULL != cur_order_id && MMISMS_FOLDER_BT == cur_order_id->flag.folder_type) 
        {
            update_sms_num = MMISMS_GetUpdateBtSMSNum(dual_sys);
        }
        else
#endif
        {    
            update_sms_num = MMISMS_GetUpdateSMSNum(dual_sys);
        }
        
        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetTobeReadMsgCount ret_val=%d, update_sms_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1668_112_2_18_2_53_57_600,(uint8*)"dd", ret_val, update_sms_num);
        
        if (ret_val >= update_sms_num)
        {
            ret_val -= update_sms_num;
        }
        else
        {
            ret_val = 0;
        }
    }

    SCI_TRACE_LOW("MMISMS_GetTobeReadMsgCount unread count=%d",ret_val);
    return ret_val;
}

/*****************************************************************************/
//     Description : to get the count of undownload msg
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_E dual_sys)
{
    uint16 ret_val = 0;

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        ret_val =s_mmisms_order.total_undownloaded_num.total_msg_num;//δ������Ϣ��Ŀ
    }
    else //if(MN_DUAL_SYS_1 <= dual_sys && dual_sys < MMI_DUAL_SYS_MAX)
    {
        ret_val = s_mmisms_order.total_undownloaded_num.msg_num[dual_sys]; //��Xδ������Ϣ����Ŀ
    }
        
    return ret_val;
}

/*****************************************************************************/
//     Description : to get the count of mt undownload msg
//    Global resource dependence : s_mmisms_order
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetMTUnDownloadMsgCount(MN_DUAL_SYS_E dual_sys)
{
    uint16 ret_val = 0;

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        ret_val =s_mmisms_order.total_undownloaded_num.total_msg_num;//δ������Ϣ��Ŀ
    }
    else //if(MN_DUAL_SYS_1 <= dual_sys && dual_sys < MMI_DUAL_SYS_MAX)
    {
        ret_val = s_mmisms_order.total_undownloaded_num.msg_num[dual_sys]; //��X δ������Ϣ����Ŀ
    }
        
    return ret_val;
}

/*****************************************************************************/
//     Description : to get the count of total message which is to be read(����δ��sms��mms��ota��push...)
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_E dual_sys)
{
    uint16 ret_val = 0;
    uint16 update_sms_num = 0;

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        ret_val =s_mmisms_order.total_to_be_read_msg_num.total_msg_num;//δ����Ϣ����Ŀ(�������ţ����ţ�ota��Ϣ��)
    }
    else //if(MN_DUAL_SYS_1 <= dual_sys && dual_sys < MMI_DUAL_SYS_MAX)
    {
        ret_val = s_mmisms_order.total_to_be_read_msg_num.msg_num[dual_sys]; //simX ��δ����Ϣ��Ŀ(����δ��sms��mms��ota��push...)
    }
    
    if (g_mmisms_global.is_need_update_order)
    {

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        MMISMS_ORDER_ID_T cur_order_id = MMISMS_GetCurOperationOrderId();
        if(PNULL != cur_order_id && MMISMS_FOLDER_BT == cur_order_id->flag.folder_type) 
        {
            update_sms_num = MMISMS_GetUpdateBtSMSNum(dual_sys);
        }
        else
#endif
        {    
            update_sms_num = MMISMS_GetUpdateSMSNum(dual_sys);
        }

        //SCI_TRACE_LOW:"MMISMS: MMISMS_GetTobeReadMessageCount ret_val=%d, update_sms_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1744_112_2_18_2_53_57_601,(uint8*)"dd", ret_val, update_sms_num);

        if (ret_val >= update_sms_num)
        {
            ret_val -= update_sms_num;
        }
        else
        {
            ret_val = 0;
        }
    }

    return ret_val;
}

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetFirstUnreadSmsId(void)
{
    MMISMS_ORDER_ID_T ret_val = PNULL;
    
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    cur_order_id = s_mmisms_order.mt_unread_head_ptr ;
    
    while (cur_order_id != PNULL)
    {
        if ( MMISMS_TYPE_SMS == cur_order_id->flag.msg_type )
        {
            ret_val = cur_order_id;
            break;
        }
        cur_order_id = cur_order_id->next_ptr;
    }  
    
    return ret_val;
}

/*****************************************************************************/
//     Description : get the count of msg(sms\wap push\mms\..)for widget
//    Global resource dependence : 
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetAllUnreadMsgCount(void)
{
    return MMISMS_GetTobeReadSmsCount(MN_DUAL_SYS_MAX)
           + MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS)
           + MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_PUSH)
           + MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_OTA)
           + MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS_OTA)
           ;
}

/*****************************************************************************/
//     Description : to get the count of msg(sms mms wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetFirstUnreadMsgId(void)
{
    MMISMS_ORDER_ID_T ret_val = PNULL;
    
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    cur_order_id = s_mmisms_order.mt_unread_head_ptr ;
    
    while (cur_order_id != PNULL)
    {
        if ( MMISMS_TYPE_SMS == cur_order_id->flag.msg_type 
            || MMISMS_TYPE_WAP_PUSH == cur_order_id->flag.msg_type
            || MMISMS_TYPE_MMS == cur_order_id->flag.msg_type
            || MMISMS_TYPE_MMS_OTA == cur_order_id->flag.msg_type
            || MMISMS_TYPE_WAP_OTA == cur_order_id->flag.msg_type)
        {
            ret_val = cur_order_id;
            break;
        }
        cur_order_id = cur_order_id->next_ptr;
    }  
    
    return ret_val;
}

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to get the count of msg(sms mms wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetFirstUnreadPushMsg(void)
{
    MMISMS_ORDER_ID_T ret_val = PNULL;
    
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    cur_order_id = s_mmisms_order.mt_unread_head_ptr;
    
    while (cur_order_id != PNULL)
    {
        if (MMISMS_TYPE_WAP_PUSH == cur_order_id->flag.msg_type 
            || MMISMS_TYPE_WAP_OTA == cur_order_id->flag.msg_type
            )
        {
            ret_val = cur_order_id;
            break;
        }
        cur_order_id = cur_order_id->next_ptr;
    }  
    
    return ret_val;
}
#endif

/*****************************************************************************/
//     Description : init the order variable
//    Global resource dependence :  d
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitSmsOrder(void)
{
    //SCI_TRACE_LOW:"MMISMS:MMISMS_InitSmsOrder"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_1889_112_2_18_2_53_57_602,(uint8*)"");

    if (!g_mmisms_global.is_link_init)
    {
        SCI_MEMSET(&s_mmisms_order,0,sizeof(MMISMS_ORDER_RESULT_T));
        
        MMISMS_OrderInit();

        g_mmisms_global.is_link_init = TRUE;
    }
}

/*****************************************************************************/
//     Description : CalculateTotalNum
//    Global resource dependence :  
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void CalculateTotalNum(void)
{
    uint32 i = 0;
    
    //MT����(ƴ�Ӻ�)
    s_mmisms_order.total_mt_num.total_msg_num = s_mmisms_order.total_undownloaded_num.total_msg_num +
        s_mmisms_order.total_to_be_read_msg_num.total_msg_num +
        s_mmisms_order.total_mt_readed_num.total_msg_num;

#ifdef _ATEST_
#ifdef WIN32
    ATESTOTA_GetTotalMsgNumberInInbox(s_mmisms_order.total_mt_num.total_msg_num);
#endif//WIN32
#endif//_ATEST_

    //MO����(ƴ�Ӻ�)
    s_mmisms_order.total_mo_num.total_msg_num = s_mmisms_order.total_nosend_num.total_msg_num + 
        s_mmisms_order.total_sendfail_num.total_msg_num + 
        s_mmisms_order.total_sendsucc_num.total_msg_num;

    //SIM1 ���MO����(ƴ�Ӻ�)
    s_mmisms_order.total_mo_num.total_msg_num = s_mmisms_order.total_nosend_num.total_msg_num + 
        s_mmisms_order.total_sendfail_num.total_msg_num + 
        s_mmisms_order.total_sendsucc_num.total_msg_num;

    //for security mail 
    //MT����(ƴ�Ӻ�)
    s_mmisms_order.security_total_mt_num.total_msg_num = s_mmisms_order.security_total_undownloaded_num.total_msg_num +
        s_mmisms_order.security_total_to_be_read_msg_num.total_msg_num +
        s_mmisms_order.security_total_mt_readed_num.total_msg_num;

    //MO����(ƴ�Ӻ�)
    s_mmisms_order.security_total_mo_num.total_msg_num = s_mmisms_order.security_total_nosend_num.total_msg_num + 
        s_mmisms_order.security_total_sendfail_num.total_msg_num + 
        s_mmisms_order.security_total_sendsucc_num.total_msg_num;

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
    //SIM ���MT����(ƴ�Ӻ�)
    s_mmisms_order.total_mt_num.msg_num[i] = s_mmisms_order.total_undownloaded_num.msg_num[i] +
        s_mmisms_order.total_to_be_read_msg_num.msg_num[i] +
        s_mmisms_order.total_mt_readed_num.msg_num[i];

    //SIM ���MT����(δƴ��)
    s_mmisms_order.total_mt_num.ori_msg_num[i]= s_mmisms_order.total_undownloaded_num.ori_msg_num[i] +
        s_mmisms_order.total_to_be_read_msg_num.ori_msg_num[i] +
        s_mmisms_order.total_mt_readed_num.ori_msg_num[i];

    //SIM����SMS MT����(δƴ��)
    s_mmisms_order.total_mt_num.ori_sim_sms_num[i]= s_mmisms_order.total_undownloaded_num.ori_sim_sms_num[i] +
        s_mmisms_order.total_to_be_read_msg_num.ori_sim_sms_num[i] +
        s_mmisms_order.total_mt_readed_num.ori_sim_sms_num[i];

    //SIM ���MO����(ƴ�Ӻ�)
    s_mmisms_order.total_mo_num.msg_num[i] = s_mmisms_order.total_nosend_num.msg_num[i] + 
        s_mmisms_order.total_sendfail_num.msg_num[i] + 
        s_mmisms_order.total_sendsucc_num.msg_num[i];

    //SIM ���MO����(δƴ��)
    s_mmisms_order.total_mo_num.ori_msg_num[i] = s_mmisms_order.total_nosend_num.ori_msg_num[i] + 
        s_mmisms_order.total_sendfail_num.ori_msg_num[i] + 
        s_mmisms_order.total_sendsucc_num.ori_msg_num[i];

    //SIM����SMS MO����(δƴ��)
    s_mmisms_order.total_mo_num.ori_sim_sms_num[i] = s_mmisms_order.total_nosend_num.ori_sim_sms_num[i] + 
        s_mmisms_order.total_sendfail_num.ori_sim_sms_num[i] + 
        s_mmisms_order.total_sendsucc_num.ori_sim_sms_num[i];

    //for security mail 
    //SIM ���MT����(ƴ�Ӻ�)
    s_mmisms_order.security_total_mt_num.msg_num[i] = s_mmisms_order.security_total_undownloaded_num.msg_num[i] +
        s_mmisms_order.security_total_to_be_read_msg_num.msg_num[i] +
        s_mmisms_order.security_total_mt_readed_num.msg_num[i];

    //SIM ���MT����(δƴ��)
    s_mmisms_order.security_total_mt_num.ori_msg_num[i]= s_mmisms_order.security_total_undownloaded_num.ori_msg_num[i] +
        s_mmisms_order.security_total_to_be_read_msg_num.ori_msg_num[i] +
        s_mmisms_order.security_total_mt_readed_num.ori_msg_num[i];

    //SIM����SMS MT����(δƴ��)
    s_mmisms_order.security_total_mt_num.ori_sim_sms_num[i]= s_mmisms_order.security_total_undownloaded_num.ori_sim_sms_num[i] +
        s_mmisms_order.security_total_to_be_read_msg_num.ori_sim_sms_num[i] +
        s_mmisms_order.security_total_mt_readed_num.ori_sim_sms_num[i];

    //SIM ���MO����(ƴ�Ӻ�)
    s_mmisms_order.security_total_mo_num.msg_num[i] = s_mmisms_order.security_total_nosend_num.msg_num[i] + 
        s_mmisms_order.security_total_sendfail_num.msg_num[i] + 
        s_mmisms_order.security_total_sendsucc_num.msg_num[i];

    //SIM ���MO����(δƴ��)
    s_mmisms_order.security_total_mo_num.ori_msg_num[i] = s_mmisms_order.security_total_nosend_num.ori_msg_num[i] + 
        s_mmisms_order.security_total_sendfail_num.ori_msg_num[i] + 
        s_mmisms_order.security_total_sendsucc_num.ori_msg_num[i];

    //SIM����SMS MO����(δƴ��)
    s_mmisms_order.security_total_mo_num.ori_sim_sms_num[i] = s_mmisms_order.security_total_nosend_num.ori_sim_sms_num[i] + 
        s_mmisms_order.security_total_sendfail_num.ori_sim_sms_num[i] + 
        s_mmisms_order.security_total_sendsucc_num.ori_sim_sms_num[i];
    }
}

/*****************************************************************************/
//     Function name: MMISMS_InsertSmsToOrder()
//  Description :  ����һ����Ϣ��s_mmisms_order
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertSmsToOrder (MMISMS_STATE_T *order_info_ptr)
{
    MMISMS_STATE_T **insert_type_head_ptr = PNULL;
    MMISMS_STATE_T *new_state_ptr = PNULL;
    MMISMS_STATE_T *long_sms_state_ptr = PNULL;
    MMISMS_TYPE_E type = MMISMS_NO_TYPE ;
    MMISMS_MSG_NUM_T *total_num_ptr = PNULL;
    MMISMS_ORDER_ID_T read_long_sms_id = PNULL;
    MMISMS_ORDER_ID_T first_long_sms_id = PNULL;
    MMISMS_ORDER_ID_T cur_sms_id = PNULL;
    BOOLEAN is_to_be_read = FALSE;
    uint32 total_msg_num = 0;
    uint32 i = 0;
    MMISMS_FOLDER_TYPE_E folder_type = order_info_ptr->flag.folder_type;
    
    if (PNULL == order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InsertSmsToOrder order_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2028_112_2_18_2_53_57_603,(uint8*)"");
        return FALSE;
    }

    for(i=MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        total_msg_num += (s_mmisms_order.total_mo_num.ori_msg_num[i] + s_mmisms_order.total_mt_num.ori_msg_num[i]) ;

    //������ȡ���Ʒ�Χ������ȡ
        if (i == order_info_ptr->flag.dual_sys
        && MMISMS_TYPE_SMS == order_info_ptr->flag.msg_type
        && MN_SMS_STORAGE_SIM == order_info_ptr->flag.storage
        && ((s_mmisms_order.total_mo_num.ori_sim_sms_num[i] + s_mmisms_order.total_mt_num.ori_sim_sms_num[i]) >= MMISMS_GetMaxSimSmsNum((MN_DUAL_SYS_E)i)))
        {
            //SCI_TRACE_LOW:"MMISMS_InsertSmsToOrder sim%d sms exceed(%d)!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2042_112_2_18_2_53_57_604,(uint8*)"dd", (i+1), s_mmisms_order.total_mo_num.ori_sim_sms_num[i] + s_mmisms_order.total_mt_num.ori_sim_sms_num[i]);
            return FALSE;
        }
    }
    
    if (total_msg_num >= s_mmisms_order.max_sms_num.total_msg_num)//�ռ���
    {
        //SCI_TRACE_LOW:"MMISMS_InsertSmsToOrder (%d)MEMORY FULL, RETURN!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2049_112_2_18_2_53_57_605,(uint8*)"d", total_msg_num);
        return FALSE;
    }
	
    //����LinkedArrayAllocNode����order_info_ptr�е����ݱ���������
    new_state_ptr = LinkedArrayAllocNode();
    if ( PNULL == new_state_ptr)  //no space to alloc
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2057_112_2_18_2_53_57_606,(uint8*)"");
        return FALSE;
    }
    else //�ѷ���洢�ռ䣬��������δ��������
    {
        SCI_MEMSET(new_state_ptr, 0, sizeof(MMISMS_STATE_T));
    }

    SCI_MEMCPY(new_state_ptr,order_info_ptr,sizeof(MMISMS_STATE_T));
    new_state_ptr->next_ptr = PNULL;
    new_state_ptr->prev_ptr = PNULL;
    new_state_ptr->next_chat_ptr = PNULL;
    new_state_ptr->prev_chat_ptr = PNULL;
    new_state_ptr->next_long_sms_ptr = PNULL;
    //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder new_state_ptr->flag.mo_mt_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2071_112_2_18_2_53_57_607,(uint8*)"d", new_state_ptr->flag.mo_mt_type);

    switch(new_state_ptr->flag.mo_mt_type)
    {
    case MMISMS_MT_NOT_DOWNLOAD:
    case MMISMS_MT_NOT_NOTIFY:
    case MMISMS_MT_NEED_NOT_DOWNLOAD:
        type = MMISMS_MT_TYPE;
        
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            insert_type_head_ptr = &s_mmisms_order.security_mt_undownloaded_head_ptr;
            total_num_ptr = &(s_mmisms_order.security_total_undownloaded_num);
        }
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        else if(MMISMS_FOLDER_BT == folder_type)
        {
            insert_type_head_ptr = &s_mmisms_order.bt_mt_undownloaded_head_ptr;
            total_num_ptr = &(s_mmisms_order.bt_total_undownloaded_num);
        }
#endif
        else
        {
            insert_type_head_ptr = &s_mmisms_order.mt_undownloaded_head_ptr;
            total_num_ptr = &(s_mmisms_order.total_undownloaded_num);
        }
        break;
        
    case MMISMS_MT_TO_BE_READ:
    case MMISMS_MT_SR_TO_BE_READ:
        is_to_be_read = TRUE;
        type = MMISMS_MT_TYPE;
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            read_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.security_head_ptr, new_state_ptr, MMISMS_MT_TYPE);
            if (read_long_sms_id != PNULL) //���Ѷ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //���������Ѷ������еĳ�����״̬��Ϊδ��,ʱ��ͳһΪ��һ������ĳ����ŵ�ʱ��;
                cur_sms_id = read_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MT_TO_BE_READ;
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2109_112_2_18_2_53_58_608,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0 , sizeof(MMISMS_STATE_T));
                    }

                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));
                    for(i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
                    {
                        if( cur_sms_id == g_mmisms_global.operation.cur_order_id_arr[i] )
                        {
                            g_mmisms_global.operation.cur_order_id_arr[i] = long_sms_state_ptr;
                        }
                    }
#ifdef MMI_SMS_CHAT_SUPPORT
                    DeleteNodeFromChatOrder(cur_sms_id,type);
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.security_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.security_total_mt_readed_num),
                                    (s_mmisms_order.security_to_be_read_msg_num)); //���Ѷ�������ɾ����

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;

                    InsertNode(TRUE,
                                    &(s_mmisms_order.security_mt_unread_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.security_total_to_be_read_msg_num),
                                    (s_mmisms_order.security_to_be_read_msg_num));  //���뵽δ��������
#ifdef MMI_SMS_CHAT_SUPPORT
                    InsertNodeToChatOrder(long_sms_state_ptr,type);
#endif
                }
                
                insert_type_head_ptr = &s_mmisms_order.security_mt_unread_head_ptr;                
            }
            else //���Ѷ�������û���ҵ����²����sms ref��ͬ��Ԫ��,��δ�������в���
            {
                insert_type_head_ptr = &s_mmisms_order.security_mt_unread_head_ptr;
            }    
            total_num_ptr = &(s_mmisms_order.security_total_to_be_read_msg_num);
        }
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        else if(MMISMS_FOLDER_BT == folder_type)
        {
            read_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.bt_mt_readed_head_ptr, new_state_ptr, MMISMS_MT_TYPE);
            if (read_long_sms_id != PNULL) //���Ѷ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //���������Ѷ������еĳ�����״̬��Ϊδ��,ʱ��ͳһΪ��һ������ĳ����ŵ�ʱ��;
                cur_sms_id = read_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MT_TO_BE_READ;
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2172_112_2_18_2_53_58_609,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0 , sizeof(MMISMS_STATE_T));
                    }
                    
                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));
                    for(i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
                    {
                        if( cur_sms_id == g_mmisms_global.operation.cur_order_id_arr[i] )
                        {
                            g_mmisms_global.operation.cur_order_id_arr[i] = long_sms_state_ptr;
                        }
                    }

#ifdef MMI_SMS_CHAT_SUPPORT                    
                    DeleteNodeFromChatOrder(cur_sms_id,type);
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.bt_mt_readed_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.bt_total_mt_readed_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num)); //���Ѷ�������ɾ����

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;

                    InsertNode(TRUE,
                                    &(s_mmisms_order.bt_mt_unread_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.bt_total_to_be_read_msg_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num));  //���뵽δ��������
#ifdef MMI_SMS_CHAT_SUPPORT                                    
                    InsertNodeToChatOrder(long_sms_state_ptr,type);
#endif
                }
                
                insert_type_head_ptr = &s_mmisms_order.bt_mt_unread_head_ptr;                
            }
            else //���Ѷ�������û���ҵ����²����sms ref��ͬ��Ԫ��,��δ�������в���
            {
                insert_type_head_ptr = &s_mmisms_order.bt_mt_unread_head_ptr;
            }    
            total_num_ptr = &(s_mmisms_order.bt_total_to_be_read_msg_num);
        }
#endif
        else
        {
            read_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.mt_readed_head_ptr, new_state_ptr, MMISMS_MT_TYPE);
            if (read_long_sms_id != PNULL) //���Ѷ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //���������Ѷ������еĳ�����״̬��Ϊδ��,ʱ��ͳһΪ��һ������ĳ����ŵ�ʱ��;
                cur_sms_id = read_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MT_TO_BE_READ;
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2172_112_2_18_2_53_58_609,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0 , sizeof(MMISMS_STATE_T));
                    }
                    
                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));
                    for(i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
                    {
                        if( cur_sms_id == g_mmisms_global.operation.cur_order_id_arr[i] )
                        {
                            g_mmisms_global.operation.cur_order_id_arr[i] = long_sms_state_ptr;
                        }
                    }

#ifdef MMI_SMS_CHAT_SUPPORT                    
                    DeleteNodeFromChatOrder(cur_sms_id,type);
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.mt_readed_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.total_mt_readed_num),
                                    (s_mmisms_order.to_be_read_msg_num)); //���Ѷ�������ɾ����
                    SCI_TRACE_LOW("MMISMS_InsertSmsToOrder delete readed_link to unread link!");

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;

                    InsertNode(TRUE,
                                    &(s_mmisms_order.mt_unread_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.total_to_be_read_msg_num),
                                    (s_mmisms_order.to_be_read_msg_num));  //���뵽δ��������
#ifdef MMI_SMS_CHAT_SUPPORT                                    
                    InsertNodeToChatOrder(long_sms_state_ptr,type);
#endif
                }
                
                insert_type_head_ptr = &s_mmisms_order.mt_unread_head_ptr;                
            }
            else //���Ѷ�������û���ҵ����²����sms ref��ͬ��Ԫ��,��δ�������в���
            {
                insert_type_head_ptr = &s_mmisms_order.mt_unread_head_ptr;
            }    
            total_num_ptr = &(s_mmisms_order.total_to_be_read_msg_num);
        }
        break;
        
    case MMISMS_MT_HAVE_READ:
    case MMISMS_MT_SR_HAVE_READ:
        type = MMISMS_MT_TYPE;
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            insert_type_head_ptr = &s_mmisms_order.security_head_ptr;  
            total_num_ptr = &(s_mmisms_order.security_total_mt_readed_num);
        }
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        else if(MMISMS_FOLDER_BT == folder_type)
        {
            insert_type_head_ptr = &s_mmisms_order.bt_mt_readed_head_ptr;  
            total_num_ptr = &(s_mmisms_order.bt_total_mt_readed_num);
        }
#endif
        else
        {
            insert_type_head_ptr = &s_mmisms_order.mt_readed_head_ptr;  
            total_num_ptr = &(s_mmisms_order.total_mt_readed_num);
        }
        break;
        
    case MMISMS_MO_SEND_SUCC:
        type = MMISMS_MO_TYPE;
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.security_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ���ʧ���������ҵ������²����sms ref��ͬ��Ԫ��
            {
                if(first_long_sms_id->flag.mo_mt_type == MMISMS_MO_SEND_FAIL)
    		  {
    			//�²���ĳ�����״̬��Ϊ����ʧ��
    			new_state_ptr->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;
    			insert_type_head_ptr = &s_mmisms_order.security_head_ptr; 
    			total_num_ptr = &(s_mmisms_order.security_total_sendfail_num);
    		  }
    		  else
    		  {
    			insert_type_head_ptr = &s_mmisms_order.security_head_ptr;
    			total_num_ptr = &(s_mmisms_order.security_total_sendsucc_num);
    		  }
            }
            else
            {
                insert_type_head_ptr = &s_mmisms_order.security_head_ptr;
    	         total_num_ptr = &(s_mmisms_order.security_total_sendsucc_num);
            }
        }
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        else if(MMISMS_FOLDER_BT == folder_type)
        {
            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.mo_sendfail_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ���ʧ���������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //�²���ĳ�����״̬��Ϊ����ʧ��
                new_state_ptr->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendfail_head_ptr; 
                total_num_ptr = &(s_mmisms_order.bt_total_sendfail_num);
            }
            else
            {
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendsucc_head_ptr;
                total_num_ptr = &(s_mmisms_order.bt_total_sendsucc_num);
            }       
        }
#endif
        else
        {
            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.mo_sendfail_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ���ʧ���������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //�²���ĳ�����״̬��Ϊ����ʧ��
                new_state_ptr->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;
                insert_type_head_ptr = &s_mmisms_order.mo_sendfail_head_ptr; 
                total_num_ptr = &(s_mmisms_order.total_sendfail_num);
            }
            else
            {
                insert_type_head_ptr = &s_mmisms_order.mo_sendsucc_head_ptr;
                total_num_ptr = &(s_mmisms_order.total_sendsucc_num);
            }       
        }
        break;
        
    case MMISMS_MO_SEND_FAIL:                        
        type = MMISMS_MO_TYPE; 
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.security_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ��ͳɹ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //�������ڷ��ͳɹ������еĳ�����״̬��Ϊ����ʧ��
                cur_sms_id = first_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;  
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2299_112_2_18_2_53_58_610,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0, sizeof(MMISMS_STATE_T));
                    }
                    
                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));

#ifdef MMI_SMS_CHAT_SUPPORT
                    //if (MMISMS_SENDSTATUS_SENDING != cur_sms_id->backsend_status && MMISMS_SENDSTATUS_WAITING != cur_sms_id->backsend_status)
                    {
                        DeleteNodeFromChatOrder(cur_sms_id,type);
                    }                    
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.security_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.security_total_sendsucc_num),
                                    (s_mmisms_order.security_to_be_read_msg_num)); //�ӷ��ͳɹ�������ɾ����

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;

                    InsertNode(FALSE,
                                    &(s_mmisms_order.security_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.security_total_sendfail_num),
                                    (s_mmisms_order.security_to_be_read_msg_num));    //���뵽����ʧ��������
#ifdef MMI_SMS_CHAT_SUPPORT 
                    //if (MMISMS_SENDSTATUS_SENDING != long_sms_state_ptr->backsend_status && MMISMS_SENDSTATUS_WAITING != long_sms_state_ptr->backsend_status)
                    {
                        InsertNodeToChatOrder(long_sms_state_ptr,type);
                    }                    
#endif
                }
                insert_type_head_ptr = &s_mmisms_order.security_head_ptr;
            }
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        else if(MMISMS_FOLDER_BT == folder_type)
        {
            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.bt_mo_sendsucc_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ��ͳɹ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //�������ڷ��ͳɹ������еĳ�����״̬��Ϊ����ʧ��
                cur_sms_id = first_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;  
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2361_112_2_18_2_53_58_611,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0, sizeof(MMISMS_STATE_T));
                    }
                    
                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));

#ifdef MMI_SMS_CHAT_SUPPORT
                    //if (MMISMS_SENDSTATUS_SENDING != cur_sms_id->backsend_status && MMISMS_SENDSTATUS_WAITING != cur_sms_id->backsend_status)
                    {
                        DeleteNodeFromChatOrder(cur_sms_id,type);
                    }
                    
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.bt_mo_sendsucc_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.bt_total_sendsucc_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num)); //�ӷ��ͳɹ�������ɾ����

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;

                    InsertNode(FALSE,
                                    &(s_mmisms_order.bt_mo_sendfail_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.bt_total_sendfail_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num));    //���뵽����ʧ�������� 
#ifdef MMI_SMS_CHAT_SUPPORT 
                    //if (MMISMS_SENDSTATUS_SENDING != long_sms_state_ptr->backsend_status && MMISMS_SENDSTATUS_WAITING != long_sms_state_ptr->backsend_status)
                    {
                        InsertNodeToChatOrder(long_sms_state_ptr,type);
                    }                    
#endif
                }
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendfail_head_ptr;
            }
            else 
            {
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendfail_head_ptr;
            }             
            total_num_ptr = &(s_mmisms_order.bt_total_sendfail_num);
        }
#endif
            else 
            {
                insert_type_head_ptr = &s_mmisms_order.security_head_ptr;
            }             
            total_num_ptr = &(s_mmisms_order.security_total_sendfail_num);
        }
        else
        {
            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.mo_sendsucc_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ��ͳɹ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //�������ڷ��ͳɹ������еĳ�����״̬��Ϊ����ʧ��
                cur_sms_id = first_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;  
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2361_112_2_18_2_53_58_611,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0, sizeof(MMISMS_STATE_T));
                    }
                    
                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));

#ifdef MMI_SMS_CHAT_SUPPORT
                    //if (MMISMS_SENDSTATUS_SENDING != cur_sms_id->backsend_status && MMISMS_SENDSTATUS_WAITING != cur_sms_id->backsend_status)
                    {
                        DeleteNodeFromChatOrder(cur_sms_id,type);
                    }
                    
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.mo_sendsucc_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.total_sendsucc_num),
                                    (s_mmisms_order.to_be_read_msg_num)); //�ӷ��ͳɹ�������ɾ����

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;

                    InsertNode(FALSE,
                                    &(s_mmisms_order.mo_sendfail_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.total_sendfail_num),
                                    (s_mmisms_order.to_be_read_msg_num));    //���뵽����ʧ�������� 
#ifdef MMI_SMS_CHAT_SUPPORT 
                    //if (MMISMS_SENDSTATUS_SENDING != long_sms_state_ptr->backsend_status && MMISMS_SENDSTATUS_WAITING != long_sms_state_ptr->backsend_status)
                    {
                        InsertNodeToChatOrder(long_sms_state_ptr,type);
                    }                    
#endif
                }
                insert_type_head_ptr = &s_mmisms_order.mo_sendfail_head_ptr;
            }
            else 
            {
                insert_type_head_ptr = &s_mmisms_order.mo_sendfail_head_ptr;
            }             
            total_num_ptr = &(s_mmisms_order.total_sendfail_num);
        }
        break;
        
    case MMISMS_MO_DRAFT:    
        type = MMISMS_MO_TYPE;
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            insert_type_head_ptr = &s_mmisms_order.security_head_ptr;        
            total_num_ptr = &(s_mmisms_order.security_total_nosend_num);
        }
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        else if(MMISMS_FOLDER_BT == folder_type)
        {
            insert_type_head_ptr = &s_mmisms_order.bt_mo_draft_head_ptr;
            total_num_ptr = &(s_mmisms_order.bt_total_nosend_num);
        }
#endif 
        else
        {
            insert_type_head_ptr = &s_mmisms_order.mo_draft_head_ptr;
            total_num_ptr = &(s_mmisms_order.total_nosend_num);
        }
        break;
        
    default:
        return FALSE;
    }

    if(MMISMS_FOLDER_SECURITY == folder_type)
    {
        if (is_to_be_read)
        {
            InsertNode(TRUE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.security_to_be_read_msg_num));
        }
        else
        {
            InsertNode(FALSE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.security_to_be_read_msg_num));
        }
    }
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    else if(MMISMS_FOLDER_BT == folder_type)
    {
        if (is_to_be_read)
        {
            InsertNode(TRUE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.bt_to_be_read_msg_num));
        }
        else
        {
            InsertNode(FALSE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.bt_to_be_read_msg_num));
        }
	}
#endif
    else
    {
        if (is_to_be_read)
        {
            InsertNode(TRUE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.to_be_read_msg_num));
        }
        else
        {
            InsertNode(FALSE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.to_be_read_msg_num));
        }
    }

#ifdef MMI_SMS_CHAT_SUPPORT
    //insert to chat link
    //if (MMISMS_SENDSTATUS_SENDING != new_state_ptr->backsend_status && MMISMS_SENDSTATUS_WAITING != new_state_ptr->backsend_status)
    {
        InsertNodeToChatOrder(new_state_ptr,type);
    }
    
#endif

    CalculateTotalNum();
    
    return TRUE;
}

/*****************************************************************************/
//     Function name: MMISMS_GetSmsInfoFromOrder ()
//  Description :  ����pos�����s_mmisms_order�д洢��SMS�ĸ�Ҫ��Ϣ
//    Global resource dependence :  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetSmsInfoFromOrder (
                                           MMISMS_ORDER_ID_T order_id, //[IN] Ҫ��ȡ��sms��s_mmisms_order�е������±�
                                           MMISMS_STATE_T *order_info_ptr //[OUT] Msg�ĸ�Ҫ��Ϣ
                                           )
{
    if (PNULL == order_id || PNULL == order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSmsInfoFromOrder order_id order_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2500_112_2_18_2_53_58_612,(uint8*)"");
        return FALSE;
    }    
    SCI_MEMCPY(order_info_ptr,(MMISMS_STATE_T *)order_id,sizeof(MMISMS_STATE_T));
    return TRUE;        
}

/*****************************************************************************/
//     Description : update the status info from unread to read,and order the mt sms again.
//    Global resource dependence : s_mmisms_order
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_UpdateSmsInfoInOrder(
                                           MMISMS_ORDER_ID_T      old_order_id,  
                                           MMISMS_STATE_T        *new_order_info_ptr
                                           )
{
    if (PNULL == old_order_id || PNULL == new_order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_UpdateSmsInfoInOrder old_order_id new_order_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2518_112_2_18_2_53_58_613,(uint8*)"");
        return FALSE;
    }  
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if(MMISMS_FOLDER_BT == old_order_id->flag.folder_type)
    {
        MMISMS_DelBtSmsInfoInOrder(old_order_id);
    }
    else
    {
        MMISMS_DelSmsInfoInOrder(old_order_id);
    }
#else	
    MMISMS_DelSmsInfoInOrder(old_order_id);
#endif
    MMISMS_InsertSmsToOrder(new_order_info_ptr);
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);
    return TRUE;    
}

/*****************************************************************************/
//     Description : to get the count of box
//    Global resource dependence : s_mmisms_order
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetBoxTotalNumberFromOrder(
                                               MMISMS_BOX_TYPE_E    box_type
                                               )
{
    uint16 count = 0;
    
    switch ( box_type )
    {
    case MMISMS_BOX_MT:
        count = s_mmisms_order.total_mt_num.total_msg_num;
        break;
        
    case MMISMS_BOX_SENDSUCC:
        count = s_mmisms_order.total_sendsucc_num.total_msg_num;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        count = s_mmisms_order.total_sendfail_num.total_msg_num;
        break;
        
    case MMISMS_BOX_NOSEND:
        count = s_mmisms_order.total_nosend_num.total_msg_num;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        count = s_mmisms_order.security_total_mt_num.total_msg_num + 
                s_mmisms_order.security_total_mo_num.total_msg_num;
        break; 
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
    case MMISMS_BOX_CHAT:
        count = s_mmisms_chat_linked_order.uint_num;
        break;
#endif        
        
    case MMISMS_BOX_ALL:
        count = s_mmisms_order.total_mt_num.total_msg_num + 
                s_mmisms_order.total_sendsucc_num.total_msg_num +
                s_mmisms_order.total_sendfail_num.total_msg_num +
                s_mmisms_order.total_nosend_num.total_msg_num +
                s_mmisms_order.security_total_mt_num.total_msg_num + 
                s_mmisms_order.security_total_mo_num.total_msg_num;
        break;
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    case MMISMS_BT_BOX_MT:

        count = MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_undownloaded_head_ptr) +
                MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_unread_head_ptr) +
                MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_readed_head_ptr);

        SCI_TRACE_LOW("MMISMS_GetBoxTotalNumberFromOrder: box_type = %d, count = %d", box_type, count);
        break;
    
    case MMISMS_BT_BOX_SENDFAIL:
        
        count = MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mo_sendfail_head_ptr);

        SCI_TRACE_LOW("MMISMS_GetBoxTotalNumberFromOrder: box_type = %d, count = %d", box_type, count);
        break;

    case MMISMS_BT_BOX_NOSEND:

        count = MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mo_draft_head_ptr);
        SCI_TRACE_LOW("MMISMS_GetBoxTotalNumberFromOrder: box_type = %d, count = %d", box_type, count);
        break;
        
    case MMISMS_BT_BOX_SENDSUCC:

        count = MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mo_sendsucc_head_ptr);
        SCI_TRACE_LOW("MMISMS_GetBoxTotalNumberFromOrder: box_type = %d, count = %d", box_type, count);
        break;

    case MMISMS_BT_BOX_ALL:
        count = s_mmisms_order.bt_total_mt_num.total_msg_num + 
                s_mmisms_order.bt_total_sendsucc_num.total_msg_num +
                s_mmisms_order.bt_total_sendfail_num.total_msg_num +
                s_mmisms_order.bt_total_nosend_num.total_msg_num;
        SCI_TRACE_LOW("MMISMS_GetBoxTotalNumberFromOrder: box_type = %d, count = %d", box_type, count);
        break;

#endif
    default:
        //SCI_TRACE_MID:"MMISMS_GetBoxTotalNumberFromOrder: box_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2575_112_2_18_2_53_58_614,(uint8*)"d", box_type);
        break;
    }
    
    return (count);
}

/*****************************************************************************/
//     Function name: MMISMS_DelSmsInfoInOrder ()
//  Description :  ����pos��ɾ��s_mmisms_order����Ӧ����Ϣ��
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DelSmsInfoInOrder(
                                        MMISMS_ORDER_ID_T order_id  //[IN] 
                                        )
{
    MMISMS_STATE_T **delete_type_head_ptr = PNULL;
    MMISMS_TYPE_E  type = MMISMS_NO_TYPE;
    MMISMS_STATE_T old_sms_state = {0};
    MMISMS_MSG_NUM_T *total_num_ptr = PNULL;
    BOOLEAN is_to_be_read = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;
    
    if(PNULL == order_id)
    {
        return FALSE;
    }
    
    MMISMS_GetSmsInfoFromOrder(order_id, &old_sms_state);
    folder_type = old_sms_state.flag.folder_type;

    SCI_TRACE_LOW("MMISMS_DelSmsInfoInOrder mo_mt_type=%d",folder_type);
    switch(old_sms_state.flag.mo_mt_type)
    {
    case MMISMS_MT_NOT_DOWNLOAD:
    case MMISMS_MT_NOT_NOTIFY:
    case MMISMS_MT_NEED_NOT_DOWNLOAD:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_mt_undownloaded_head_ptr;    
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_undownloaded_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.mt_undownloaded_head_ptr;    
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.total_undownloaded_num);
        }
        break;
        
    case MMISMS_MT_TO_BE_READ:
    case MMISMS_MT_SR_TO_BE_READ:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            is_to_be_read = TRUE;
            delete_type_head_ptr = &s_mmisms_order.security_mt_unread_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_to_be_read_msg_num);
        }
        else
        {
            is_to_be_read = TRUE;
            delete_type_head_ptr = &s_mmisms_order.mt_unread_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.total_to_be_read_msg_num);
        }
        break;
        
    case MMISMS_MT_HAVE_READ:
    case MMISMS_MT_SR_HAVE_READ:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_mt_readed_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.mt_readed_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.total_mt_readed_num);
        }
        break;
        
    case MMISMS_MO_SEND_SUCC:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_sendsucc_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.mo_sendsucc_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.total_sendsucc_num);
        }
        break;
        
    case MMISMS_MO_SEND_FAIL:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_sendfail_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.mo_sendfail_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.total_sendfail_num);
        }
        break;
        
    case MMISMS_MO_DRAFT:  
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_nosend_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.mo_draft_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.total_nosend_num);
        }
        break;
        
    default:
        break;
    }

#ifdef MMI_SMS_CHAT_SUPPORT
    DeleteNodeFromChatOrder(order_id,type);
#endif
    
    if(MMISMS_FOLDER_SECURITY == folder_type)
    {
        if (is_to_be_read)
        {
            DeleteNode(TRUE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.security_to_be_read_msg_num));        
        }
        else
        {
            DeleteNode(FALSE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.security_to_be_read_msg_num));
        }
    }
    else
    {
        if (is_to_be_read)
        {
            DeleteNode(TRUE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.to_be_read_msg_num));        
        }
        else
        {
            DeleteNode(FALSE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.to_be_read_msg_num));
        }
    }

    CalculateTotalNum();
        
    return TRUE;
}


/*lint -e794*/
/*lint -e831*/
/*****************************************************************************/
//     Function name: DeleteNode()
//  Description :  ��һ���ڵ��������ɾ����
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteNode(
                         BOOLEAN        is_to_be_read,
                         MMISMS_STATE_T **delete_type_head_ptr_ptr, 
                         MMISMS_STATE_T *sms_state_ptr,
                         MMISMS_TYPE_E  type,
                         MMISMS_MSG_NUM_T *total_msg_num_ptr,
                         MMISMS_MSG_NUM_T *to_be_read_msg_num)
{
    MMISMS_STATE_T *cur_ptr         = PNULL;
    MMISMS_STATE_T *cur_longsms_ptr = PNULL;
    MMISMS_STATE_T *pre_longsms_ptr = PNULL;
    
    if (PNULL == sms_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:DeleteNode sms_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2781_112_2_18_2_53_59_615,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"DeleteNode: sms_state_ptr->longsms_max_num = %d, sms_state_ptr->longsms_seq_num = %d,sms_state_ptr->head_ref = %d,msg_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2785_112_2_18_2_53_59_616,(uint8*)"dddd",sms_state_ptr->longsms_max_num, sms_state_ptr->longsms_seq_num, sms_state_ptr->head_ref, sms_state_ptr->flag.msg_type);
    
    if (PNULL == delete_type_head_ptr_ptr || PNULL == *delete_type_head_ptr_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: DeleteNode: delete_type_head_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2789_112_2_18_2_53_59_617,(uint8*)"");
    }
    else
    {
        if (sms_state_ptr->flag.is_concatenate_sms) //is long sms
        {
            //����ref��ͬ����(��ĳ�������������ͷ)
            cur_ptr = *delete_type_head_ptr_ptr;
            
            //cur_ptr�ƶ���ĳһ��㣬ʹҪ�����ʱ�䲻С�ڵ�ǰ��ָ��ʱ�䣬����cur_ptrδָ�����һ�����
            while(cur_ptr != PNULL)
            {
                MMISMS_STATE_T *temp_ptr = cur_ptr;
                if (CompareSmsState(temp_ptr, type, sms_state_ptr))
                {
                    while (temp_ptr != PNULL)
                    {
                        if ((temp_ptr->longsms_seq_num == sms_state_ptr->longsms_seq_num)
                            && (temp_ptr->record_id == sms_state_ptr->record_id)
                            && (temp_ptr->flag.storage == sms_state_ptr->flag.storage))
                        {
                            break;
                        }
                        else
                        {
                            temp_ptr = temp_ptr->next_long_sms_ptr;
                        }
                    } 
                    if (temp_ptr != PNULL)
                    {
                        break;
                    }
                }
                cur_ptr = cur_ptr->next_ptr;
            }
            
            if (cur_ptr != PNULL) //�ҵ�ref��ͬ��λ��
            {
                if (sms_state_ptr->longsms_seq_num == cur_ptr->longsms_seq_num
                    && (sms_state_ptr->record_id == cur_ptr->record_id)
                    && (sms_state_ptr->flag.storage == cur_ptr->flag.storage))  //����������
                {
                    if (cur_ptr->next_long_sms_ptr == PNULL) //����������������
                    {
                        if (cur_ptr->prev_ptr != PNULL) //��������ͷ
                        {
                            cur_ptr->prev_ptr->next_ptr = cur_ptr->next_ptr;
                        }
                        else //������ͷ
                        {
                            *delete_type_head_ptr_ptr = cur_ptr->next_ptr;
                        }
                        
                        if (cur_ptr->next_ptr != PNULL) //��������β
                        {
                            cur_ptr->next_ptr->prev_ptr = cur_ptr->prev_ptr;
                        }
                        
                        HandleOrderMsgNum(is_to_be_read,
                                                        FALSE,
                                                        sms_state_ptr,
                                                        total_msg_num_ptr,
                                                        to_be_read_msg_num);
                    }
                    else //��������������
                    {
                        cur_ptr->next_long_sms_ptr->prev_ptr = cur_ptr->prev_ptr;
                        cur_ptr->next_long_sms_ptr->next_ptr = cur_ptr->next_ptr;
                        
                        if (cur_ptr->prev_ptr != PNULL)  //��������ͷ
                        {
                            cur_ptr->prev_ptr->next_ptr = cur_ptr->next_long_sms_ptr;
                        }
                        else  //������ͷ
                        {
                            *delete_type_head_ptr_ptr = cur_ptr->next_long_sms_ptr;
                        }
                        
                        if (cur_ptr->next_ptr != PNULL) //��������β
                        {
                            cur_ptr->next_ptr->prev_ptr = cur_ptr->next_long_sms_ptr;
                        }

                        HandleLongSMSOrderMsgNum(is_to_be_read,
                                                                    FALSE,
                                                                    sms_state_ptr,
                                                                    total_msg_num_ptr,
                                                                    to_be_read_msg_num);
                    }
                }
                else //������������,�����²���
                {
                    cur_longsms_ptr = cur_ptr;
                    
                    while ((cur_longsms_ptr->next_long_sms_ptr != PNULL)
                        &&(!((sms_state_ptr->longsms_seq_num == cur_longsms_ptr->longsms_seq_num)
                        && (sms_state_ptr->record_id == cur_longsms_ptr->record_id)
                        && (sms_state_ptr->flag.storage == cur_longsms_ptr->flag.storage) )) )
                    {
                        pre_longsms_ptr = cur_longsms_ptr;
                        cur_longsms_ptr = cur_longsms_ptr->next_long_sms_ptr;
                    }
                    if (cur_longsms_ptr->longsms_seq_num == sms_state_ptr->longsms_seq_num
                        && (sms_state_ptr->record_id == cur_longsms_ptr->record_id)
                        && (sms_state_ptr->flag.storage == cur_longsms_ptr->flag.storage)) //find
                    {
                        pre_longsms_ptr->next_long_sms_ptr = cur_longsms_ptr->next_long_sms_ptr;

                        HandleLongSMSOrderMsgNum(is_to_be_read,
                                                                    FALSE,
                                                                    sms_state_ptr,
                                                                    total_msg_num_ptr,
                                                                    to_be_read_msg_num);
                        
                    }
                    else 
                    {
                        //SCI_TRACE_LOW:"MMISMS: DeleteNode :can't find the sms ptr in long sms link"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2906_112_2_18_2_53_59_618,(uint8*)"");
                        return FALSE;
                    }
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMISMS DeleteNode: there is no sms have the same ref with the to delete sms1_state_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2913_112_2_18_2_53_59_619,(uint8*)"");
                return FALSE;
            }
        }
        else //is normal sms
        {
            if (sms_state_ptr->prev_ptr == PNULL) //�������ͷ
            {
                *delete_type_head_ptr_ptr = sms_state_ptr->next_ptr;
                if (sms_state_ptr->next_ptr != PNULL)
                {
                    sms_state_ptr->next_ptr->prev_ptr =PNULL;
                }
            }
            else  //���������ͷ
            {
                sms_state_ptr->prev_ptr->next_ptr = sms_state_ptr->next_ptr;
                if (sms_state_ptr->next_ptr != PNULL)
                {
                    sms_state_ptr->next_ptr->prev_ptr = sms_state_ptr->prev_ptr;
                }
            }
            
            HandleOrderMsgNum(is_to_be_read,
                                            FALSE,
                                            sms_state_ptr,
                                            total_msg_num_ptr,
                                            to_be_read_msg_num);
        }
    }
    //�ͷű����������Դ
    SCI_MEMSET(sms_state_ptr, 0, sizeof(MMISMS_STATE_T));
    LinkedArrayFreeNode(sms_state_ptr);
    
    return TRUE;    
}

/*lint +e794*/
/*lint +e831*/

/*****************************************************************************/
//     Function name: HandleLongSMSOrderMsgNum()
//  Description :  ���������е���Ϣ��Ŀ
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleLongSMSOrderMsgNum(
                             BOOLEAN is_to_be_read,
                             BOOLEAN is_insert, 
                             MMISMS_STATE_T *sms_state_ptr,
                             MMISMS_MSG_NUM_T *total_msg_num_ptr,
                             MMISMS_MSG_NUM_T *to_be_read_msg_num)
{
    uint8 index = sms_state_ptr->flag.dual_sys;

    //SCI_TRACE_LOW:"HandleLongSMSOrderMsgNum is_to_be_read=%d,is_insert=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2966_112_2_18_2_53_59_620,(uint8*)"dd", is_to_be_read, is_insert);
    if (PNULL == total_msg_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleLongSMSOrderMsgNum total_msg_num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2969_112_2_18_2_53_59_621,(uint8*)"");
        return;
    }
    if (is_insert)
    {

        if(index < MMI_DUAL_SYS_MAX)
        {
            (total_msg_num_ptr->ori_msg_num[index])++;

            if (MMISMS_TYPE_SMS == sms_state_ptr->flag.msg_type
                && MN_SMS_STORAGE_SIM == sms_state_ptr->flag.storage)//����SIM1�϶���
            {
                (total_msg_num_ptr->ori_sim_sms_num[index])++;
            }
        }        
    }
    else
    {
        if(index < MMI_DUAL_SYS_MAX)
        {
            if(total_msg_num_ptr->ori_msg_num[index] != 0)
            { 
                (total_msg_num_ptr->ori_msg_num[index])--;
            }

            if (MMISMS_TYPE_SMS == sms_state_ptr->flag.msg_type
                && MN_SMS_STORAGE_SIM == sms_state_ptr->flag.storage)//����SIM1�϶���
            {
                if(total_msg_num_ptr->ori_sim_sms_num[index] != 0)
                {
                    (total_msg_num_ptr->ori_sim_sms_num[index])--;
                }
            }
        }        
        
    }
}

/*****************************************************************************/
//     Function name: HandleOrderNum()
//  Description :  ���������е���Ϣ��Ŀ
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleOrderMsgNum(
                             BOOLEAN is_to_be_read,
                             BOOLEAN is_insert, 
                             MMISMS_STATE_T *sms_state_ptr,
                             MMISMS_MSG_NUM_T *total_msg_num_ptr,
                             MMISMS_MSG_NUM_T *to_be_read_msg_num)
{
    uint8 index = sms_state_ptr->flag.dual_sys;
    
    //SCI_TRACE_LOW:"HandleOrderMsgNum is_to_be_read=%d,is_insert=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3017_112_2_18_2_53_59_622,(uint8*)"dd", is_to_be_read, is_insert);
    if (PNULL == total_msg_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleOrderMsgNum total_msg_num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3020_112_2_18_2_53_59_623,(uint8*)"");
        return;
    }
    SCI_TRACE_LOW("HandleOrderMsgNum sms_order record_id=%d, storage_id=%d",sms_state_ptr->record_id,sms_state_ptr->flag.storage);
    if (is_insert)
    {
        (total_msg_num_ptr->total_msg_num)++;
        
        if (is_to_be_read)
        {
            (to_be_read_msg_num[sms_state_ptr->flag.msg_type].total_msg_num)++;
             SCI_TRACE_LOW("HandleOrderMsgNum insert save_type=%d, unread_num=%d",
                            sms_state_ptr->flag.msg_type,
                            to_be_read_msg_num[sms_state_ptr->flag.msg_type].total_msg_num);
        }
        
        if(index < MMI_DUAL_SYS_MAX)
        {
            (total_msg_num_ptr->msg_num[index])++;
            (total_msg_num_ptr->ori_msg_num[index])++;

            if (MMISMS_TYPE_SMS == sms_state_ptr->flag.msg_type
                && MN_SMS_STORAGE_SIM == sms_state_ptr->flag.storage)//����SIM1�϶���
            {
                (total_msg_num_ptr->ori_sim_sms_num[index])++;
            }

            if (is_to_be_read)
            {
                (to_be_read_msg_num[sms_state_ptr->flag.msg_type].msg_num[index])++;
            }
        }
    }
    else
    {
        if(total_msg_num_ptr->total_msg_num != 0)
        { 
            (total_msg_num_ptr->total_msg_num)--;
        }
        
        if (is_to_be_read)
        {
             if(to_be_read_msg_num[sms_state_ptr->flag.msg_type].total_msg_num != 0)
             {
                (to_be_read_msg_num[sms_state_ptr->flag.msg_type].total_msg_num)--;
                SCI_TRACE_LOW("HandleOrderMsgNum delete save_type=%d, unread_num=%d",
                              sms_state_ptr->flag.msg_type,
                              to_be_read_msg_num[sms_state_ptr->flag.msg_type].total_msg_num);
             }
        }
        
        if( index < MMI_DUAL_SYS_MAX )
        {
            if(total_msg_num_ptr->msg_num[index] != 0)
            {
                (total_msg_num_ptr->msg_num[index])--;
            }
            if(total_msg_num_ptr->ori_msg_num[index] != 0)
            {
                (total_msg_num_ptr->ori_msg_num[index])--;
            }

            if (MMISMS_TYPE_SMS == sms_state_ptr->flag.msg_type
                && MN_SMS_STORAGE_SIM == sms_state_ptr->flag.storage)//����SIM1�϶���
            {

                if(total_msg_num_ptr->ori_sim_sms_num[index] != 0)
                {                
                    (total_msg_num_ptr->ori_sim_sms_num[index])--;
                }
            }

            if (is_to_be_read)
            {
                if(to_be_read_msg_num[sms_state_ptr->flag.msg_type].msg_num[index] != 0)
                {
                    (to_be_read_msg_num[sms_state_ptr->flag.msg_type].msg_num[index])--;
                }
            }
        }       
    }
}

/*****************************************************************************/
//     Function name: MMISMS_FreeSmsOrder ()
//  Description :  �ͷ�s_mmisms_order����Դ�����s_mmisms_order��
//    Global resource dependence: s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_FreeSmsOrder(void)
{
    // free the space for SMS order
    LinkedArrayInit(0);
    SCI_MEMSET(&s_mmisms_order,0,sizeof(MMISMS_ORDER_RESULT_T));

    g_mmisms_global.is_link_init = FALSE;
    return TRUE;
}

/*****************************************************************************/
//     Function name: MMISMS_LoadSmsListFromOrder ()
//  Description :  ����box_type���ɿؼ�list;
//    Global resource dependence: s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_LoadSmsListFromOrder(MMISMS_BOX_TYPE_E box_type, 
                                           MMI_CTRL_ID_T     ctrl_id)
                                           
{
    uint16 count = 0;
    uint16 list_item_num = 0;
    
    count = MMISMS_GetBoxTotalNumberFromOrder(box_type);
    //SCI_TRACE_LOW:"MMISMS: MMISMS_LoadSmsListFromOrder the count of list box is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3105_112_2_18_2_54_0_624,(uint8*)"d", count);

    list_item_num = MAX(count, 1);

    GUILIST_SetMaxItem(ctrl_id, list_item_num, TRUE);//max item 

    if (count > 0)
    {
        MMISMS_ListSmsFromOrder(box_type);
        return TRUE;        
    }
    else
    {
        GUILIST_RemoveAllItems(ctrl_id);
        #ifndef MMI_PDA_SUPPORT
        MMISMS_AppendEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);
        #endif
        return FALSE;
    }
}


/*****************************************************************************/
#ifdef MMISMS_SORT_CAHCE_BUFFER_SUPPORT
/*****************************************************************************/
//     Description : Construct sort cache
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ConstructSortCache(void)
{
    if (PNULL == s_mmisms_sort_cache_buffer_ptr)
    {
        s_mmisms_sort_cache_buffer_ptr = SCI_ALLOCA(MMISMS_SORT_CAHCE_BUFFER_SIZE);
    }

    if (PNULL != s_mmisms_sort_cache_buffer_ptr)
    {
        SCI_MEMSET(s_mmisms_sort_cache_buffer_ptr, 0x00, MMISMS_SORT_CAHCE_BUFFER_SIZE);
        s_mmisms_sort_cache_buffer_pos = 0;
        s_mmisms_sort_cache_ptr = PNULL;

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//     Description : Destroy sort cache
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void DestroySortCache(void)
{
    if (PNULL != s_mmisms_sort_cache_buffer_ptr)
    {
        SCI_FREE(s_mmisms_sort_cache_buffer_ptr);

        s_mmisms_sort_cache_buffer_ptr = PNULL;
        s_mmisms_sort_cache_buffer_pos = 0;
        s_mmisms_sort_cache_ptr = PNULL;
    }
}

/*****************************************************************************/
//     Description : Add node to sort cache
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN AddSortCacheNode(MMISMS_STATE_T *sms_state_ptr, wchar *wstr_ptr, uint16 wstr_len)
{
    MMISMS_SORT_CAHCE_T *tmp_sort_cache_ptr = s_mmisms_sort_cache_ptr;

    while (PNULL != tmp_sort_cache_ptr)
    {
        //same record
        if (tmp_sort_cache_ptr->sms_state == (uint32)sms_state_ptr)
        {
            return FALSE;
        }

        if (PNULL == tmp_sort_cache_ptr->next)
        {
            break;
        }

        tmp_sort_cache_ptr = tmp_sort_cache_ptr->next;
    }

    if (PNULL != s_mmisms_sort_cache_buffer_ptr)
    {
        uint16 node_len = sizeof(MMISMS_SORT_CAHCE_T) + wstr_len*sizeof(wchar);
        MMISMS_SORT_CAHCE_T *sort_cache_ptr = PNULL;

        if (s_mmisms_sort_cache_buffer_pos + node_len > MMISMS_SORT_CAHCE_BUFFER_SIZE)
        {
            return FALSE;
        }

        /*
        sms_state
        wstr_addr
        wstr_len
        reserved
        next_point
        wstr_data
        */
        sort_cache_ptr = (MMISMS_SORT_CAHCE_T *)((uint32)s_mmisms_sort_cache_buffer_ptr + s_mmisms_sort_cache_buffer_pos);
        sort_cache_ptr->sms_state = (uint32)sms_state_ptr;
        sort_cache_ptr->wstr_ptr = (uint32)((uint32)s_mmisms_sort_cache_buffer_ptr + s_mmisms_sort_cache_buffer_pos + sizeof(MMISMS_SORT_CAHCE_T));
        MMI_WSTRNCPY(sort_cache_ptr->wstr_ptr, wstr_len, wstr_ptr, wstr_len, wstr_len);
        sort_cache_ptr->wstr_len = wstr_len;
        sort_cache_ptr->next = PNULL;

        if (PNULL != tmp_sort_cache_ptr)
        {
            tmp_sort_cache_ptr->next = sort_cache_ptr;
        }
        else //head node
        {
            s_mmisms_sort_cache_ptr = sort_cache_ptr;
        }

        s_mmisms_sort_cache_buffer_pos += node_len;
        s_mmisms_sort_cache_buffer_pos = (s_mmisms_sort_cache_buffer_pos + 3)/4*4;// 4Bytes Align
    }

    return TRUE;
}

/*****************************************************************************/
//     Description : Find node from sort cache
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN FindSortCacheNode(MMISMS_STATE_T *sms_state_ptr, wchar **wstr_ptr, uint16 *wstr_len)
{
    MMISMS_SORT_CAHCE_T *tmp_sort_cache_ptr = s_mmisms_sort_cache_ptr;

    if (PNULL == sms_state_ptr || PNULL == wstr_ptr || 0 == wstr_len)
    {
        return FALSE;
    }

    while (PNULL != tmp_sort_cache_ptr)
    {
        BOOLEAN is_found = FALSE;
        MMISMS_STATE_T *sms_state = (MMISMS_STATE_T *)tmp_sort_cache_ptr->sms_state;

        //same record
        if (tmp_sort_cache_ptr->sms_state == (uint32)sms_state_ptr)
        {
            is_found = TRUE;
        }
        else if (sms_state->flag.msg_type == sms_state_ptr->flag.msg_type)
        {
            if (MMISMS_TYPE_SMS == sms_state->flag.msg_type)
            {
                if (sms_state->number.bcd_number.number_len == sms_state_ptr->number.bcd_number.number_len)
                {
                    if (!MMIAPIPB_BCDCmp(&sms_state->number.bcd_number, &sms_state_ptr->number.bcd_number))//Same Number
                    {
                        is_found = TRUE;
                    }
                }
            }
            else 
            {
                if (strlen(sms_state->number.sender) == strlen(sms_state_ptr->number.sender))
                {
                    if (!strncmp(sms_state->number.sender, sms_state_ptr->number.sender, strlen(sms_state->number.sender)))//Same Number
                    {
                        is_found = TRUE;
                    }
                }
            }
        }

        if (is_found)
        {
            *wstr_ptr = tmp_sort_cache_ptr->wstr_ptr;
            *wstr_len = tmp_sort_cache_ptr->wstr_len;

            return TRUE;
        }

        tmp_sort_cache_ptr = tmp_sort_cache_ptr->next;
    }

    return FALSE;
}
#endif
 /*****************************************************************************/
//  Description : sort chat message order
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SortChatMessageOrder()
{
    if (MMISMS_IsNeedSort()) //����������ͷ����ı䣬����������chat����
    {
        if(MMISMS_ORDER_BY_NAME == MMISMS_GetOrderType())
        {
            SortChatLinkedOrderByName();
        }
        else
        {
            SortChatLinkedOrderByTime();
        }
    }
        MMISMS_SetNeedSort(FALSE);
}

//     Description : to list sms from the list buffer
//    Global resource dependence : nsms1_state_ptr
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ListSmsFromOrder(MMISMS_BOX_TYPE_E box_type)
{
    MMISMS_ORDER_ID_T head_order_id = 0;    


    if (MMISMS_IsNeedSort()) //����������ͷ����ı䣬������������������
    {
#ifdef MMISMS_SORT_CAHCE_BUFFER_SUPPORT
        ConstructSortCache();
#endif
        head_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mt_undownloaded_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mt_unread_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mt_unread_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mt_readed_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mt_readed_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mo_sendsucc_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mo_sendfail_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mo_draft_head_ptr = head_order_id;
        }

        head_order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.security_mt_undownloaded_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.security_mt_unread_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.security_mt_unread_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.security_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.security_head_ptr = head_order_id;
        }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        head_order_id = s_mmisms_order.bt_mt_undownloaded_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_ORDER_BY_TIME);
            s_mmisms_order.bt_mt_undownloaded_head_ptr = head_order_id;
        }
    
        head_order_id = s_mmisms_order.bt_mt_unread_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_ORDER_BY_TIME);
            s_mmisms_order.bt_mt_unread_head_ptr = head_order_id;
        }
    
        head_order_id = s_mmisms_order.bt_mt_readed_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_ORDER_BY_TIME);
            s_mmisms_order.bt_mt_readed_head_ptr = head_order_id;
        }
    
        head_order_id = s_mmisms_order.bt_mo_sendsucc_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_ORDER_BY_TIME);
            s_mmisms_order.bt_mo_sendsucc_head_ptr = head_order_id;
        }
    
        head_order_id = s_mmisms_order.bt_mo_sendfail_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_ORDER_BY_TIME);
            s_mmisms_order.bt_mo_sendfail_head_ptr = head_order_id;
        }
    
        head_order_id = s_mmisms_order.bt_mo_draft_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_ORDER_BY_TIME);
            s_mmisms_order.bt_mo_draft_head_ptr = head_order_id;
        }
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
        if(MMISMS_ORDER_BY_NAME == MMISMS_GetOrderType())
        {
            SortChatLinkedOrderByName();
        }
        else
        {
            SortChatLinkedOrderByTime();
        }
#endif        
        MMISMS_SetNeedSort(FALSE);
#ifdef MMISMS_SORT_CAHCE_BUFFER_SUPPORT
        DestroySortCache();
#endif
    }

	if(MMISMS_ORDER_BY_NAME == MMISMS_GetOrderType())
	{
	

	}
    
    switch( box_type )
    {
    case MMISMS_BOX_MT:
        GUILIST_RemoveAllItems(MMISMS_MTBOX_LISTBOX_CTRL_ID);
        //SCI_TRACE_LOW:"MMISMS: MMISMS_ListSmsFromOrder the undownloaded sms count is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3223_112_2_18_2_54_0_625,(uint8*)"d", s_mmisms_order.total_undownloaded_num.total_msg_num);
        //SCI_TRACE_LOW:"MMISMS: MMISMS_ListSmsFromOrder the unreaded sms count is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3224_112_2_18_2_54_0_626,(uint8*)"d", s_mmisms_order.total_to_be_read_msg_num.total_msg_num);
        //SCI_TRACE_LOW:"MMISMS: MMISMS_ListSmsFromOrder the readed sms count is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3225_112_2_18_2_54_0_627,(uint8*)"d", s_mmisms_order.total_mt_readed_num.total_msg_num);
        head_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_MTBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.mt_unread_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_MTBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.mt_readed_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_MTBOX_LISTBOX_CTRL_ID);
        break;
        
    case MMISMS_BOX_SENDSUCC:
        GUILIST_RemoveAllItems(MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID);
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID);
        break;
        
    case MMISMS_BOX_SENDFAIL:
        GUILIST_RemoveAllItems(MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID);
        break;
        
    case MMISMS_BOX_NOSEND:
        GUILIST_RemoveAllItems(MMISMS_NOSENDBOX_LISTBOX_CTRL_ID);
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_NOSENDBOX_LISTBOX_CTRL_ID);
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        GUILIST_RemoveAllItems(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_SECURITYBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.security_mt_unread_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_SECURITYBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.security_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_SECURITYBOX_LISTBOX_CTRL_ID);
        break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
    case MMISMS_BOX_CHAT:
        GUILIST_RemoveAllItems(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
        FormChatListFromLinkhead(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
        break;
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    case MMISMS_BT_BOX_MT:

        GUILIST_RemoveAllItems(MMISMS_BT_MTBOX_LISTBOX_CTRL_ID);

        head_order_id = s_mmisms_order.bt_mt_undownloaded_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_BT_MTBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.bt_mt_unread_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_BT_MTBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.bt_mt_readed_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_BT_MTBOX_LISTBOX_CTRL_ID);

        break;
    case MMISMS_BT_BOX_SENDFAIL:

        GUILIST_RemoveAllItems(MMISMS_BT_SENDFAILBOX_LISTBOX_CTRL_ID);
        
        head_order_id = s_mmisms_order.bt_mo_sendfail_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_BT_SENDFAILBOX_LISTBOX_CTRL_ID);

        break;
    case MMISMS_BT_BOX_NOSEND:

        GUILIST_RemoveAllItems(MMISMS_BT_NOSENDBOX_LISTBOX_CTRL_ID);
        head_order_id = s_mmisms_order.bt_mo_draft_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_BT_NOSENDBOX_LISTBOX_CTRL_ID);

        break;
    case MMISMS_BT_BOX_SENDSUCC:

        GUILIST_RemoveAllItems(MMISMS_BT_SENDSUCCBOX_LISTBOX_CTRL_ID);
        head_order_id = s_mmisms_order.bt_mo_sendsucc_head_ptr;
        FormListFromLinkhead(head_order_id, MMISMS_BT_SENDSUCCBOX_LISTBOX_CTRL_ID);

        break;
#endif

    default:
        //SCI_TRACE_LOW:"MMISMS: MMISMS_LoadSmsListFromOrder: the box_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3276_112_2_18_2_54_0_628,(uint8*)"d", box_type);
        break;
        
    }    
}

#ifdef MAINLCD_SIZE_128X160
LOCAL THEMELIST_ITEM_STYLE_T s_sms_liststyle = {0};
#endif

/*****************************************************************************/
//     Description : Form List from the link list head
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN FormListFromLinkhead(MMISMS_ORDER_ID_T head_order_id, MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T item_info = {0};/*lint !e64*/
    // GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    // MMISMS_STATE_T sms_order_info ={0};
    MMISMS_ORDER_ID_T current_order_id = 0;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle( ctrl_id );
    
#ifdef MAINLCD_SIZE_128X160
    uint32 i = GUILIST_GetTotalItemNum(ctrl_id);
    THEMELIST_ITEM_STYLE_T* pItemStyle = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL);
    SCI_MEMCPY(&s_sms_liststyle, pItemStyle, sizeof(*pItemStyle));
    s_sms_liststyle.content[1].rect.right = MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 5;
    s_sms_liststyle.content[1].rect_focus.right = MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 5;
    s_sms_liststyle.content[2].rect.left = MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 5 + 2;
    s_sms_liststyle.content[2].rect_focus.left = MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 5 + 2;
    s_sms_liststyle.content[1].scale_right = 60;
    s_sms_liststyle.content[2].scale_left = 60;
    s_sms_liststyle.main_index = 1;
#endif

    current_order_id = head_order_id;
    
    SCI_MEMSET(&item_info, 0, sizeof(GUILIST_ITEM_T)); 
#ifdef DPHONE_SUPPORT 
#ifdef MMI_DUALSIM_SUPPORT
    item_info.item_style = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT;
#else
    item_info.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
#endif
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    item_info.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT;
#else
    item_info.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
#endif
#endif 

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMISMS_PORTRAIT_SUPPORT
    item_info.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT;
#else
    #ifdef MAINLCD_SIZE_128X160
        item_info.item_style = GUIITEM_STYLE_CUSTOMIZE;
    #else
        item_info.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL;
    #endif
#endif
#endif

    while(current_order_id != PNULL)
    {
        // append the item to the end of list
        if(MMISMS_SECURITYBOX_LISTBOX_CTRL_ID == ctrl_id)
        {
            if(MMISMS_FOLDER_SECURITY == current_order_id->flag.folder_type)
            {
                GUILIST_AppendItem(list_handle, &item_info);
#ifdef MAINLCD_SIZE_128X160
                GUILIST_SetItemStyleEx(ctrl_id, i++, &s_sms_liststyle);
#endif
            }
        }
        else
        {
            if(MMISMS_FOLDER_SECURITY != current_order_id->flag.folder_type)
            {
                GUILIST_AppendItem(list_handle, &item_info);
#ifdef MAINLCD_SIZE_128X160
                GUILIST_SetItemStyleEx(ctrl_id, i++, &s_sms_liststyle);
#endif
            }
        }
        current_order_id =current_order_id->next_ptr;
    }

    return TRUE;
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : Form Chat List from the link list head
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FormChatListFromLinkhead(MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T item_info = {0};
    MMISMS_STATE_T *current_order_id = PNULL;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle( ctrl_id );
    MMISMS_CHAT_STATE_UNIT_T *number_node = PNULL;
    
    number_node = s_mmisms_chat_linked_order.head_ptr;


    //��ÿ����������һ����Ϣ����ʱ�����µ���Ϣ��ʾ���б���
    while(PNULL != number_node)
    {
        current_order_id = number_node->head_ptr;
        if(PNULL != current_order_id)
        {
#ifdef MMISMS_PORTRAIT_SUPPORT
            if(FindUnreadSMSInChatUnit(number_node))
            {
                item_info.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON;
            }
            else
            {
                item_info.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT;
            }
#else
            item_info.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT;
#endif
            GUILIST_AppendItem(list_handle, &item_info);
        }
        number_node = number_node->next_number_ptr;
    }

    return TRUE;
}
#endif


/*****************************************************************************/
//     Description : to search the name and store info by number
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void FindNameAndStoreInfoByNum(
                                     uint8            num_type,            //IN:
                                     uint8            num_plan,            //IN:
                                     uint8            num_len,            //IN:
                                     uint8            *bcd_num_ptr,        //IN:
                                     uint8            string_limit_len,    //IN:
                                     MMISMS_MO_MT_TYPE_E mo_mt_type,
                                     MMI_STRING_T    *name_ptr            //OUT:
                                     )
{
#if 0
// no use
    BOOLEAN find_result = TRUE;        // the init value must be true.
#endif
    MMIPB_BCD_NUMBER_T pb_num = {0};

    if (PNULL == bcd_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:FindNameAndStoreInfoByNum bcd_num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3882_112_2_18_2_54_1_631,(uint8*)"");
        return;
    }    
    //SCI_TRACE_LOW:"MMISMS: FindNameAndStoreInfoByNum num_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3885_112_2_18_2_54_1_632,(uint8*)"d", num_len);
    
    if (MMIAPIPB_IsPbReady())
    {
        SCI_MEMSET(&pb_num, 0, sizeof(MMIPB_BCD_NUMBER_T));
        
        // convert to the format used in phsms1_state_ptr_book
        pb_num.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8((MN_NUMBER_TYPE_E)num_type, (MN_NUMBER_PLAN_E)num_plan); 
        pb_num.number_len = MIN(num_len, MN_MAX_ADDR_BCD_LEN);
        SCI_MEMCPY(    pb_num.number,
            bcd_num_ptr, 
            pb_num.number_len
            );        
        if (num_len != pb_num.number_len)
        {
            //SCI_TRACE_LOW:"MMISMS: FindNameAndStoreInfoByNum num_len = %d, pb_num.number_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_3901_112_2_18_2_54_1_633,(uint8*)"dd", num_len, pb_num.number_len);
        }        
#if 0
// no use
        find_result = MMIAPIPB_GetNameByNumber(&pb_num, name_ptr, string_limit_len, FALSE);
#else
        MMIAPIPB_GetNameByNumber(&pb_num, name_ptr, string_limit_len, FALSE);
#endif
    }
    
    if (0 == name_ptr->wstr_len)
    {
        uint8 tel_num[MMISMS_PBNAME_MAX_LEN] = {0};
        uint16 tel_len = 0;

        if((MMISMS_MT_TO_BE_READ == mo_mt_type)
           ||(MMISMS_MT_HAVE_READ == mo_mt_type)
           ||(MMISMS_MT_SR_HAVE_READ == mo_mt_type)
           ||(MMISMS_MT_SR_TO_BE_READ == mo_mt_type)
          )
        {
            tel_len  = MMIAPICOM_GenNetDispNumber( 
                (MN_NUMBER_TYPE_E)num_type,
                (uint8)MIN((string_limit_len >> 1), num_len),
                bcd_num_ptr,
                tel_num,
                (uint8)(string_limit_len + 2)
                );
        }
        else
        {
            tel_len  = MMIAPICOM_GenDispNumber( 
                (MN_NUMBER_TYPE_E)num_type,
                (uint8)MIN((string_limit_len >> 1), num_len),
                bcd_num_ptr,
                tel_num,
                (uint8)(string_limit_len + 2)
                );
        }

        name_ptr->wstr_len = tel_len;
        MMI_STRNTOWSTR(name_ptr->wstr_ptr,
                                    name_ptr->wstr_len,
                                    (uint8 *)tel_num,
                                    MMISMS_PBNAME_MAX_LEN,
                                    name_ptr->wstr_len);
    }
}

/*****************************************************************************/
//  Description : Get Small Icon by sim index
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSimSmallIcon(MN_DUAL_SYS_E dual_sys)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }

    return (MMI_IMAGE_ID_T)(IMAGE_SMS_SIM_CARD1+(uint32)dual_sys);
#else
    return IMAGE_SMS_SIM_CARD;
#endif
}

#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note: for search module and delete by num
/***************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSmallIconByStatus(MMISMS_STATE_T *sms_state_ptr)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;

    if (PNULL == sms_state_ptr)
    {
        return IMAGE_NULL;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    dual_sys = sms_state_ptr->flag.dual_sys;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }

    if (sms_state_ptr->flag.storage == MN_SMS_STORAGE_SIM)
    {
        return (MMI_IMAGE_ID_T)(IMAGE_SMS_SIM_CARD1+(uint32)dual_sys);
    }
    else
    {
        return (MMI_IMAGE_ID_T)(IMAGE_SMS_ME_CARD1+(uint32)dual_sys);
    }
#else
    if (sms_state_ptr->flag.storage == MN_SMS_STORAGE_SIM)
    {
        return IMAGE_SMS_SIM_CARD;
    }
    else
    {
        return IMAGE_NULL;
    }
#endif
}

#endif
/*****************************************************************************/
//     Description : to get the icon by the status
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMISMS_GetIconByStatus(MMISMS_STATE_T *sms_state_ptr)
{
    MMISMS_MSG_TYPE_E msg_type = MMISMS_TYPE_SMS;
    uint8 mo_mt_flag = (uint8)MMISMS_MT_TO_BE_READ;
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    BOOLEAN is_lock = FALSE;
    MMI_IMAGE_ID_T icon_id = IMAGE_SMS_SIM_NOT_READ_SMS_ICON;
    BOOLEAN is_saved_in_sim = FALSE;

    if (PNULL == sms_state_ptr)
    {
        return IMAGE_NULL;
    }

    msg_type = sms_state_ptr->flag.msg_type;
    mo_mt_flag = sms_state_ptr->flag.mo_mt_type;
    storage = sms_state_ptr->flag.storage;

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    if(MMISMS_TYPE_MAIL == sms_state_ptr->flag.msg_type)
    {
        is_lock = mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(sms_state_ptr->record_id));
    }
    else
#endif
    {
        is_lock = sms_state_ptr->flag.is_locked;
    }

    is_saved_in_sim = (MN_SMS_STORAGE_SIM == storage) ? TRUE : FALSE;
    
    switch(msg_type) 
    {
    case MMISMS_TYPE_SMS:
        switch (mo_mt_flag)
        {
        case MMISMS_MT_TO_BE_READ:
        case MMISMS_MT_SR_TO_BE_READ:       
            if (is_lock)
            {
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_LOCK_SIM_NOT_READ_SMS_ICON : IMAGE_SMS_LOCK_NOT_READ_SMS_ICON;
            }
            else
            {
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_SIM_NOT_READ_SMS_ICON : IMAGE_SMS_NOT_READ_SMS_ICON;
                
            }
            break;
            
        case MMISMS_MT_HAVE_READ:
        case MMISMS_MT_SR_HAVE_READ:
            if (is_lock)
            {
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_LOCK_SIM_READ_SMS_ICON : IMAGE_SMS_LOCK_READ_SMS_ICON ;
            }
            else
            {
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_SIM_READ_SMS_ICON : IMAGE_SMS_READ_SMS_ICON ;
            }
            break;
            
        case MMISMS_MO_SEND_FAIL:
#ifdef MMI_TIMERMSG_SUPPORT
            {
                MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;

                event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(sms_state_ptr);

                if (PNULL != event_fast_info_ptr)
                {
                    if (is_lock)
                    {
                        icon_id = (is_saved_in_sim) ? IMAGE_SMS_TIMER_SIM_LOCK_ICON : IMAGE_SMS_TIMER_ME_LOCK_ICON;
                    }
                    else
                    {            
                        icon_id = (is_saved_in_sim) ? IMAGE_SMS_TIMER_SIM_ICON : IMAGE_SMS_TIMER_ME_ICON;
                    }
                }
                else
                {
                    if (is_lock)
                    {
                        icon_id = (is_saved_in_sim) ? IMAGE_SMS_LOCK_SIM_SEND_FAIL_SMS_ICON : IMAGE_SMS_LOCK_SEND_FAIL_SMS_ICON;
                    }
                    else
                    {            
                        icon_id = (is_saved_in_sim) ? IMAGE_SMS_SIM_SEND_FAIL_SMS_ICON : IMAGE_SMS_SEND_FAIL_SMS_ICON;
                    }
                }
            }
#else
            if (is_lock)
            {
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_LOCK_SIM_SEND_FAIL_SMS_ICON : IMAGE_SMS_LOCK_SEND_FAIL_SMS_ICON;
            }
            else
            {            
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_SIM_SEND_FAIL_SMS_ICON : IMAGE_SMS_SEND_FAIL_SMS_ICON;
            }
#endif
            break;
            
        case MMISMS_MO_SEND_SUCC:
            if (is_lock)
            {
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_LOCK_SEND_SIM_SUCCESS_SMS_ICON : IMAGE_SMS_LOCK_SEND_SUCCESS_SMS_ICON;
            }
            else
            {
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_SEND_SIM_SUCCESS_SMS_ICON : IMAGE_SMS_SEND_SUCCESS_SMS_ICON;
            }
            break;
            
        case MMISMS_MO_DRAFT:
            if (is_lock)
            {               
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_LOCK_SIM_DRAFT_SMS_ICON : IMAGE_SMS_LOCK_DRAFT_SMS_ICON; //later need lock icon
            }
            else
            {
#ifdef MMISMS_PORTRAIT_SUPPORT
                icon_id = (is_saved_in_sim) ? IMAGE_SMS_SIM_DRAFT_SMS_ICON : IMAGE_SMS_DRAFT_SMS_ICON;
#else
                icon_id = IMAGE_SMS_SIM_DRAFT_SMS_ICON;
#endif
            }
            break;
        default:
            break;
        }
        break;
        
    case MMISMS_TYPE_MMS:
        switch (mo_mt_flag)
        {
        case MMISMS_MT_NOT_DOWNLOAD:
        case MMISMS_MT_NOT_NOTIFY:
        case MMISMS_MT_NEED_NOT_DOWNLOAD:
            icon_id = is_lock? IMAGE_SMS_LOCK_MMS_NOTIFY_NEW_ICON :IMAGE_SMS_MMS_NOTIFY_NEW_ICON;             
            break;
            
        case MMISMS_MT_TO_BE_READ:
            
            icon_id = is_lock? IMAGE_SMS_LOCK_NOT_READ_MMS_ICON :IMAGE_SMS_NOT_READ_MMS_ICON;
            break;
            
        case MMISMS_MT_HAVE_READ:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON :IMAGE_SMS_READ_MMS_ICON ; 
            break;
            
        case MMISMS_MO_SEND_SUCC:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON :IMAGE_SMS_READ_MMS_ICON; 
            break;
            
        case MMISMS_MO_SEND_FAIL:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON: IMAGE_SMS_READ_MMS_ICON; 
            break;
            
        case MMISMS_MO_DRAFT:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON:IMAGE_SMS_READ_MMS_ICON; 
            break;    
            
        default:
            SCI_TRACE_LOW("MMISMS: MMISMS_GetIconByStatus mo_mt_flag = %d", mo_mt_flag);
            break;
        }
        break;
            
    case MMISMS_TYPE_WAP_OTA:
        switch (mo_mt_flag)
        {
            
        case MMISMS_MT_TO_BE_READ:
            icon_id = is_lock? IMAGE_SMS_LOCK_WAPOTA_ICON :IMAGE_SMS_WAPOTA_ICON;
            break;
            
        case MMISMS_MT_HAVE_READ:
            icon_id = is_lock? IMAGE_SMS_LOCK_WAPOTA_READED_ICON: IMAGE_SMS_WAPOTA_READED_ICON ; 
            break;
            
        default:
            SCI_TRACE_LOW("MMISMS: MMISMS_GetIconByStatus mo_mt_flag = %d", mo_mt_flag);
            break;
        }           
        break;
                
    case MMISMS_TYPE_WAP_PUSH:
        switch (mo_mt_flag)
        {
            
        case MMISMS_MT_TO_BE_READ:
            icon_id = is_lock ? IMAGE_SMS_LOCK_WAPPUSH_ICON : IMAGE_SMS_WAPPUSH_ICON;
            break;
            
        case MMISMS_MT_HAVE_READ:
            icon_id = is_lock ? IMAGE_SMS_LOCK_WAPPUSH_READED_ICON: IMAGE_SMS_WAPPUSH_READED_ICON ; 
            break;
            
        default:
            SCI_TRACE_LOW("MMISMS: MMISMS_GetIconByStatus mo_mt_flag = %d", mo_mt_flag);
            break;
        }    
        break;
                    
    case MMISMS_TYPE_MMS_OTA:
        switch (mo_mt_flag)
        {
            
        case MMISMS_MT_TO_BE_READ:
            icon_id = is_lock? IMAGE_SMS_LOCK_MMSOTA_ICON: IMAGE_SMS_MMSOTA_ICON;
            break;
            
        case MMISMS_MT_HAVE_READ:
            icon_id = is_lock? IMAGE_SMS_LOCK_MMSOTA_READED_ICON:IMAGE_SMS_MMSOTA_READED_ICON ; 
            break;
            
        default:
            SCI_TRACE_LOW("MMISMS: MMISMS_GetIconByStatus mo_mt_flag = %d", mo_mt_flag);
            break;
        }    
        break;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    case MMISMS_TYPE_MAIL:
        switch (mo_mt_flag)
        {            
        case MMISMS_MT_TO_BE_READ:
            
            icon_id = is_lock? IMAGE_SMS_LOCK_NOT_READ_MMS_ICON :IMAGE_SMS_NOT_READ_MMS_ICON;
            break;
            
        case MMISMS_MT_HAVE_READ:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON :IMAGE_SMS_READ_MMS_ICON ; 
            break;
            
        case MMISMS_MO_SEND_SUCC:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON :IMAGE_SMS_READ_MMS_ICON; 
            break;
            
        case MMISMS_MO_SEND_FAIL:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON: IMAGE_SMS_READ_MMS_ICON; 
            break;
            
        case MMISMS_MO_DRAFT:
            icon_id = is_lock? IMAGE_SMS_LOCK_READ_MMS_ICON:IMAGE_SMS_READ_MMS_ICON; 
            break;    
            
        default:
            SCI_TRACE_LOW("MMISMS: MMISMS_GetIconByStatus mo_mt_flag = %d", mo_mt_flag);
            break;
        }
        break;
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
    default:
        break;
    }
    
    return (icon_id);
}

#ifdef MRAPP_SUPPORT
/*****************************************************************************/
//     Description : ��������type�����������SMS��������
//    Global resource dependence : 
//  Author:Tommy.yan
//    Note:
/*****************************************************************************/
PUBLIC int32 MMIMRAPP_GetSMSCountByBoxType(MMISMS_BOX_TYPE_E box_type)
{
    BOOLEAN flag = FALSE;
    int32 count = 0;
    MMISMS_ORDER_ID_T head_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id  = PNULL;
    
    //����box_type��ѭ������s_mmisms_order�е����ݡ����ض�Ӧ������ָ��id��
    SCI_TRACE_LOW("MMIMRAPP_GetSMSCountByBoxType box_type %d ", box_type); 
    switch(box_type)
    {
    case MMISMS_BOX_MT:
        if(s_mmisms_order.total_to_be_read_msg_num.total_msg_num > 0)
        {
            head_order_id = s_mmisms_order.mt_unread_head_ptr;
            flag = TRUE;
        }
        else
        {
            head_order_id = s_mmisms_order.mt_readed_head_ptr;
        }
        
        cur_order_id = head_order_id;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                count++;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        
        if(flag)
        {
            cur_order_id = s_mmisms_order.mt_readed_head_ptr;
            while (cur_order_id != PNULL)
            {
                if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
                {
                    count++;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        
        SCI_TRACE_LOW("MMIMRAPP_GetSMSCountByBoxType ount %d",count);
        return count;
        
    case MMISMS_BOX_SENDSUCC:
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        break;
        
    case MMISMS_BOX_NONE:
    case MMISMS_BOX_ALL:
#ifdef MMI_SMS_CHAT_SUPPORT  
    case MMISMS_BOX_CHAT:
#endif
        break;
        
    default:
        break;
    }
    SCI_TRACE_LOW("MMIMRAPP_GetSMSCountByBoxType head_order_id is 0x%x",head_order_id); 
    
    cur_order_id = head_order_id;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    
    SCI_TRACE_LOW("MMIMRAPP_GetSMSCountByBoxType ount %d",count);
    
    return count;
}

/*****************************************************************************/
//     Description : ��������list�б��index�����������SMS������ָ��id��
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIMRAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_TYPE_E box_type,
                                                               int32  list_index)
{
    BOOLEAN flag = FALSE;
    MMISMS_ORDER_ID_T head_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id  = PNULL;
    MMISMS_ORDER_ID_T op_order_id = PNULL;
    
    //����box_type��ѭ������s_mmisms_order�е����ݡ����ض�Ӧ������ָ��id��
    SCI_TRACE_LOW("MMIMRAPP_GetSMSLinkedArrayPtrIdInList box_type %d, list_index %d", box_type, list_index); 
    
    switch(box_type)
    {
    case MMISMS_BOX_MT:
        if(s_mmisms_order.total_to_be_read_msg_num.total_msg_num > 0)
        {
            head_order_id = s_mmisms_order.mt_unread_head_ptr;
            flag = TRUE;
        }
        else
        {
            head_order_id = s_mmisms_order.mt_readed_head_ptr;
        }
        
        op_order_id = head_order_id;
        while ((op_order_id != PNULL) && (list_index >= 0))
        {
            SCI_TRACE_LOW("MMIMRAPP msg_type %d, list_index %d, cur_order_id is 0x%x", op_order_id->flag.msg_type, list_index, op_order_id); 
            if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
            {
                list_index--;
            }
            cur_order_id = op_order_id;
            op_order_id  = cur_order_id->next_ptr;
        }
        if((list_index >= 0) && flag)
        {
            op_order_id = s_mmisms_order.mt_readed_head_ptr;
            while ((op_order_id != PNULL) && (list_index >= 0))
            {
                if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
                {
                    list_index--;
                }
                cur_order_id = op_order_id;
                op_order_id  = cur_order_id->next_ptr;
            }
        }
        
        SCI_TRACE_LOW("MMIMRAPP_GetSMSLinkedArrayPtrIdInList cur_order_id is 0x%x",cur_order_id);
        return cur_order_id;
        
    case MMISMS_BOX_SENDSUCC:
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        break;
        
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    case MMISMS_BT_BOX_MT:
        
        if((MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_unread_head_ptr)) > 0)
        {
            head_order_id = s_mmisms_order.bt_mt_unread_head_ptr;
            flag = TRUE;
        }
        else
        {
            head_order_id = s_mmisms_order.bt_mt_readed_head_ptr;
        }
        
        op_order_id = head_order_id;
        while ((op_order_id != PNULL) && (list_index >= 0))
        {
            SCI_TRACE_LOW("MMIMRAPP msg_type %d, list_index %d, cur_order_id is 0x%x", op_order_id->flag.msg_type, list_index, op_order_id); 
            if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
            {
                list_index--;
            }
            cur_order_id = op_order_id;
            op_order_id  = cur_order_id->next_ptr;
        }
        if((list_index >= 0) && flag)
        {
            op_order_id = s_mmisms_order.bt_mt_readed_head_ptr;
            while ((op_order_id != PNULL) && (list_index >= 0))
            {
                if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
                {
                    list_index--;
                }
                cur_order_id = op_order_id;
                op_order_id  = cur_order_id->next_ptr;
            }
        }
        
        SCI_TRACE_LOW("MMIMRAPP_GetSMSLinkedArrayPtrIdInList cur_order_id is 0x%x",cur_order_id);
        return cur_order_id;

    case MMISMS_BT_BOX_SENDSUCC:
        head_order_id = s_mmisms_order.bt_mo_sendsucc_head_ptr;
        break;

    case MMISMS_BT_BOX_SENDFAIL:
        head_order_id = s_mmisms_order.bt_mo_sendfail_head_ptr;
        break;

    case MMISMS_BT_BOX_NOSEND:
       
        head_order_id = s_mmisms_order.bt_mo_draft_head_ptr;
        break;
#endif	

    case MMISMS_BOX_NONE:
    case MMISMS_BOX_ALL:
#ifdef MMI_SMS_CHAT_SUPPORT  
    case MMISMS_BOX_CHAT:
#endif
        break;
        
    default:
        break;
    }
    SCI_TRACE_LOW("MMIMRAPP_GetSMSLinkedArrayPtrIdInList head_order_id is 0x%x",head_order_id);
    
    op_order_id = head_order_id;
    while ((op_order_id != PNULL && list_index >= 0))
    {
        if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
        {
            list_index--;
        }
        cur_order_id = op_order_id;
        op_order_id  = cur_order_id->next_ptr;
    }
    
    SCI_TRACE_LOW("MMIMRAPP_GetSMSLinkedArrayPtrIdInList cur_order_id is 0x%x",cur_order_id);
    
    return cur_order_id;
}
#endif

#if defined(HERO_ENGINE_SUPPORT )
PUBLIC int32 MMIHEROAPP_GetSMSCountByBoxType(MMISMS_BOX_TYPE_E box_type)
{
    BOOLEAN flag = FALSE;
    int32 count = 0;
    MMISMS_ORDER_ID_T head_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id  = PNULL;
    
    //����box_type��ѭ������s_mmisms_order�е����ݡ����ض�Ӧ������ָ��id��
    SCI_TRACE_LOW("MMIHEROAPP_GetSMSCountByBoxType box_type %d ", box_type); 
    switch(box_type)
    {
    case MMISMS_BOX_MT:
        if(s_mmisms_order.total_to_be_read_msg_num.total_msg_num > 0)
        {
            head_order_id = s_mmisms_order.mt_unread_head_ptr;
            flag = TRUE;
        }
        else
        {
            head_order_id = s_mmisms_order.mt_readed_head_ptr;
        }
        
        cur_order_id = head_order_id;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                count++;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        
        if(flag)
        {
            cur_order_id = s_mmisms_order.mt_readed_head_ptr;
            while (cur_order_id != PNULL)
            {
                if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
                {
	                count++;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        
        SCI_TRACE_LOW("MMIHEROAPP_GetSMSCountByBoxType count %d",count);
        return count;
        
    case MMISMS_BOX_SENDSUCC:
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        break;
        
    case MMISMS_BOX_NONE:
    case MMISMS_BOX_ALL:
#ifdef MMI_SMS_CHAT_SUPPORT  
    case MMISMS_BOX_CHAT:
#endif
        break;
        
    default:
        break;
    }
    SCI_TRACE_LOW("MMIHEROAPP_GetSMSCountByBoxType head_order_id is 0x%x",head_order_id); 
    
    cur_order_id = head_order_id;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    
    SCI_TRACE_LOW("MMIHEROAPP_GetSMSCountByBoxType count %d",count);
    
    return count;
}

PUBLIC MMISMS_ORDER_ID_T MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_TYPE_E box_type,
                                                               int32  list_index)
{
    BOOLEAN flag = FALSE;
    MMISMS_ORDER_ID_T head_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id  = PNULL;
    MMISMS_ORDER_ID_T op_order_id = PNULL;
    
    //����box_type��ѭ������s_mmisms_order�е����ݡ����ض�Ӧ������ָ��id��
    SCI_TRACE_LOW("MMIHEROAPP_GetSMSLinkedArrayPtrIdInList box_type %d, list_index %d", box_type, list_index); 
    
    switch(box_type)
    {
    case MMISMS_BOX_MT:
        if(s_mmisms_order.total_to_be_read_msg_num.total_msg_num > 0)
        {
            head_order_id = s_mmisms_order.mt_unread_head_ptr;
            flag = TRUE;
        }
        else
        {
            head_order_id = s_mmisms_order.mt_readed_head_ptr;
        }
        
        op_order_id = head_order_id;
        while ((op_order_id != PNULL) && (list_index >= 0))
        {
            SCI_TRACE_LOW("MMIHERO msg_type %d, list_index %d, cur_order_id is 0x%x", op_order_id->flag.msg_type, list_index, op_order_id); 
            if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
            {
                list_index--;
            }
            cur_order_id = op_order_id;
            op_order_id  = cur_order_id->next_ptr;
        }
        if((list_index >= 0) && flag)
        {
            op_order_id = s_mmisms_order.mt_readed_head_ptr;
            while ((op_order_id != PNULL) && (list_index >= 0))
            {
                if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
                {
                    list_index--;
                }
                cur_order_id = op_order_id;
                op_order_id  = cur_order_id->next_ptr;
            }
        }
        
        SCI_TRACE_LOW("MMIHEROAPP_GetSMSLinkedArrayPtrIdInList cur_order_id is 0x%x",cur_order_id);
        return cur_order_id;
        
    case MMISMS_BOX_SENDSUCC:
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        break;
        
    case MMISMS_BOX_NONE:
    case MMISMS_BOX_ALL:
#ifdef MMI_SMS_CHAT_SUPPORT  
    case MMISMS_BOX_CHAT:
#endif
        break;
        
    default:
        break;
    }
    SCI_TRACE_LOW("MMIHEROAPP_GetSMSLinkedArrayPtrIdInList head_order_id is 0x%x",head_order_id);
    
    op_order_id = head_order_id;
    while ((op_order_id != PNULL && list_index >= 0))
    {
        if(MMISMS_TYPE_SMS == op_order_id->flag.msg_type)
        {
            list_index--;
        }
        cur_order_id = op_order_id;
        op_order_id  = cur_order_id->next_ptr;
    }
    
    SCI_TRACE_LOW("MMIHEROAPP_GetSMSLinkedArrayPtrIdInList cur_order_id is 0x%x",cur_order_id);
    
    return cur_order_id;
}
#endif
/*****************************************************************************/
//     Description : ��������list�б��index��������������ָ��id��
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_TYPE_E box_type,
                                                          uint32            list_index)
{
    MMISMS_ORDER_ID_T head_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id  = PNULL;
#ifdef MMI_SMS_CHAT_SUPPORT    
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
#endif
    //����box_type��ѭ������s_mmisms_order�е����ݡ����ض�Ӧ������ָ��id��
    //SCI_TRACE_LOW:"MMISMS: list_index is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4219_112_2_18_2_54_2_634,(uint8*)"d", list_index);
    switch(box_type)
    {
    case MMISMS_BOX_MT:
        //SCI_TRACE_LOW:"MMISMS: s_mmisms_order.total_undownloaded_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4223_112_2_18_2_54_2_635,(uint8*)"d",s_mmisms_order.total_undownloaded_num.total_msg_num);
        //SCI_TRACE_LOW:"MMISMS: s_mmisms_order.to_be_read_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4224_112_2_18_2_54_2_636,(uint8*)"d",s_mmisms_order.total_to_be_read_msg_num.total_msg_num);
        
        if (list_index < s_mmisms_order.total_undownloaded_num.total_msg_num)
        {
            head_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
            break;
        }
        else
        {
            if (list_index < (s_mmisms_order.total_undownloaded_num.total_msg_num + s_mmisms_order.total_to_be_read_msg_num.total_msg_num))
            {
                head_order_id = s_mmisms_order.mt_unread_head_ptr;
                list_index = list_index -s_mmisms_order.total_undownloaded_num.total_msg_num;
                break;
            }
            else
            {
                head_order_id = s_mmisms_order.mt_readed_head_ptr;
                list_index = list_index - s_mmisms_order.total_undownloaded_num.total_msg_num - s_mmisms_order.total_to_be_read_msg_num.total_msg_num;
                break;
            }
        }        
        
    case MMISMS_BOX_SENDSUCC:
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        //head_order_id = s_mmisms_order.security_head_ptr;
        if (list_index < s_mmisms_order.security_total_undownloaded_num.total_msg_num)
        {
            head_order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;
            break;
        }
        else
        {
            if (list_index < (s_mmisms_order.security_total_undownloaded_num.total_msg_num + s_mmisms_order.security_total_to_be_read_msg_num.total_msg_num))
            {
                head_order_id = s_mmisms_order.security_mt_unread_head_ptr;
                list_index = list_index -s_mmisms_order.security_total_undownloaded_num.total_msg_num;
                break;
            }
            else
            {
                head_order_id = s_mmisms_order.security_head_ptr;
                list_index = list_index - s_mmisms_order.security_total_undownloaded_num.total_msg_num - s_mmisms_order.security_total_to_be_read_msg_num.total_msg_num;
                break;
            }
        }        
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
    case MMISMS_BOX_CHAT:
        head_chat_id = s_mmisms_chat_linked_order.head_ptr;
        break;
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)    
    case MMISMS_BT_BOX_MT:

        //SCI_TRACE_LOW:"MMISMS: s_mmisms_order.total_undownloaded_num is %d"
        SCI_TRACE_LOW("s_mmisms_order.total_undownloaded_num is %d", (MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_undownloaded_head_ptr)));
        SCI_TRACE_LOW("s_mmisms_order.bt_mt_unread_head_ptr is %d", (MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_unread_head_ptr)));
        SCI_TRACE_LOW("s_mmisms_order.bt_mt_readed_head_ptr is %d", (MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_readed_head_ptr)));
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4223_112_2_18_2_54_2_635,(uint8*)"d",s_mmisms_order.total_undownloaded_num.total_msg_num);
        //SCI_TRACE_LOW:"MMISMS: s_mmisms_order.to_be_read_num is %d"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4224_112_2_18_2_54_2_636,(uint8*)"d",s_mmisms_order.total_to_be_read_msg_num.total_msg_num);

        if (list_index < MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_undownloaded_head_ptr))
        {
            head_order_id = s_mmisms_order.bt_mt_undownloaded_head_ptr;
        }
        else
        {
            if (list_index < ((MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_undownloaded_head_ptr)) + (MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_unread_head_ptr))))
            {
                head_order_id = s_mmisms_order.bt_mt_unread_head_ptr;
                list_index = list_index - (MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_undownloaded_head_ptr));
            }
            else
            {
                head_order_id = s_mmisms_order.bt_mt_readed_head_ptr;
                list_index = list_index - (MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_undownloaded_head_ptr)) - (MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_unread_head_ptr));
            }
        }

        break;
    case MMISMS_BT_BOX_SENDFAIL:

        head_order_id = s_mmisms_order.bt_mo_sendfail_head_ptr;
        break;
    case MMISMS_BT_BOX_NOSEND:

        head_order_id = s_mmisms_order.bt_mo_draft_head_ptr;
        break;
    case MMISMS_BT_BOX_SENDSUCC:

        head_order_id = s_mmisms_order.bt_mo_sendsucc_head_ptr;
        break;
    
#endif

    default:
        //SCI_TRACE_LOW:"MMISMS: MMISMS_LoadSmsListFromOrder: the box_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4289_112_2_18_2_54_2_637,(uint8*)"d", box_type);
        break;        
    }

    //SCI_TRACE_LOW:"head_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4293_112_2_18_2_54_2_638,(uint8*)"d",head_order_id);

#ifdef MMI_SMS_CHAT_SUPPORT    
    if(MMISMS_BOX_CHAT == box_type)
    {
        uint32 while_count = 0;
        uint32 total_msg_num = s_mmisms_order.max_sms_num.total_msg_num;   //fixed bug147321 ��ֹ��ѭ��
        while ((head_chat_id != PNULL) && (list_index >0))
        {
            head_chat_id = head_chat_id->next_number_ptr;
            list_index --;
        }     
        if(PNULL != head_chat_id)
        {
            cur_order_id = head_chat_id->head_ptr;
            while(PNULL != cur_order_id)
            {
                while_count++;
                if(PNULL == cur_order_id->next_chat_ptr)
                {
                    break;
                }
                else
                {
                    cur_order_id = cur_order_id->next_chat_ptr;
                    if(cur_order_id != PNULL && cur_order_id == cur_order_id->next_chat_ptr)
                    {
                        SCI_TRACE_LOW("MMISMS link array error MMISMS_GetLinkedArrayPtrIdInList");
                        break;
                    }
                    if (while_count > total_msg_num)
                    {
                        SCI_TRACE_LOW("MMISMS link array error while_count exceed total_msg_num MMISMS_GetLinkedArrayPtrIdInList");
                        break;
                    }

                }
            }
        }
    }
    else
#endif        
    {
        cur_order_id = head_order_id;
        while ((cur_order_id != PNULL) && (list_index >0))
        {
            cur_order_id = cur_order_id->next_ptr;
            list_index --;
        }
    }
    
    //SCI_TRACE_LOW:"MMISMS:cur_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4330_112_2_18_2_54_2_639,(uint8*)"d",cur_order_id);
    return cur_order_id;
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : ��������list�б��index��������������ָ��id��
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetChatLinkedArrayPtrIdInList(uint32 list_index)
{
    MMISMS_CHAT_STATE_UNIT_T *cur_number_ptr  = PNULL;
    MMISMS_ORDER_ID_T cur_order_id  = PNULL;
    uint32 while_count = 0;
    uint32 total_msg_num = s_mmisms_order.max_sms_num.total_msg_num;   //fixed bug147321 ��ֹ��ѭ��

    //SCI_TRACE_LOW:"MMISMS: list_index is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4344_112_2_18_2_54_2_640,(uint8*)"d", list_index);

    cur_number_ptr = s_mmisms_chat_linked_order.head_ptr;

    //SCI_TRACE_LOW:"head_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4348_112_2_18_2_54_2_641,(uint8*)"d",cur_number_ptr);

    //ָ�����ڵ�
    while ((cur_number_ptr != PNULL) && (list_index >0))
    {
        cur_number_ptr = cur_number_ptr->next_number_ptr;
        list_index --;
    }
    
    //ָ�򱾺����ʱ�����½ڵ�
    if(PNULL != cur_number_ptr)
    {
        cur_order_id = cur_number_ptr->head_ptr;

        while(PNULL != cur_order_id)
        {
            while_count++;
            if(PNULL == cur_order_id->next_chat_ptr)
            {
                break;
            }
            cur_order_id = cur_order_id->next_chat_ptr;
            if(cur_order_id != PNULL && cur_order_id == cur_order_id->next_chat_ptr)
            {
                SCI_TRACE_LOW("MMISMS link array error MMISMS_GetChatLinkedArrayPtrIdInList");
                break;
            } 
            if (while_count > total_msg_num)
            {
                SCI_TRACE_LOW("MMISMS link array error while_count exceed total_msg_num MMISMS_GetChatLinkedArrayPtrIdInList");
                break;
            } 
        }
    }

    //SCI_TRACE_LOW:"MMISMS:cur_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4372_112_2_18_2_54_2_642,(uint8*)"d",cur_number_ptr);
    
    return cur_order_id;
}
#endif

/*****************************************************************************/
//     Description : ����һ����¼��δ��������������ȫ��ɾ��sms��ʱ���յ��µ�sms��
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertSmsToUnhandleOrder (const MMISMS_STATE_T *order_info_ptr)
{
    MMISMS_STATE_T *new_state_ptr = PNULL;
    
    //����ռ�
    new_state_ptr = SCI_ALLOCAZ(sizeof(MMISMS_STATE_T));
    if (PNULL == new_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InsertSmsToUnhandleOrder new_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_4390_112_2_18_2_54_2_643,(uint8*)"");
        return FALSE;
    }   
    //SCI_MEMSET(new_state_ptr, 0, sizeof(MMISMS_STATE_T));
    
    *new_state_ptr = *order_info_ptr;
    new_state_ptr->next_ptr = PNULL;
    new_state_ptr->prev_ptr = PNULL;
    new_state_ptr->next_long_sms_ptr = PNULL;
    
    //�嵽unhandle�����ͷ��
    if (s_mmisms_order.not_handled_head_ptr == PNULL)
    {
        s_mmisms_order.not_handled_head_ptr = new_state_ptr;
    }
    else
    {
        new_state_ptr->next_ptr = s_mmisms_order.not_handled_head_ptr;
        s_mmisms_order.not_handled_head_ptr->prev_ptr = new_state_ptr;
        s_mmisms_order.not_handled_head_ptr = new_state_ptr;
    }
    
    return TRUE;
}

/*****************************************************************************/
//     Description : ��δ�������Ϣ�����еļ�¼���뵽��ʽ�����С�
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertUnhandledSmsToOrder (void)
{
    MMISMS_STATE_T *cur_order_info_ptr = PNULL;
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_STATE_T *next_order_info_ptr= PNULL;
    cur_order_info_ptr = s_mmisms_order.not_handled_head_ptr;
    
    while (cur_order_info_ptr != PNULL )
    {
        if (cur_order_info_ptr->flag.mo_mt_type != MMISMS_MT_NOT_DOWNLOAD 
            &&cur_order_info_ptr->flag.mo_mt_type != MMISMS_MT_NOT_NOTIFY
            &&cur_order_info_ptr->flag.mo_mt_type != MMISMS_MT_NEED_NOT_DOWNLOAD) //not mms push
        {            
            //delete from unhandled link
            if (cur_order_info_ptr->prev_ptr == PNULL) //�������ͷ
            {
                s_mmisms_order.not_handled_head_ptr = cur_order_info_ptr->next_ptr;
                if (cur_order_info_ptr->next_ptr != PNULL)
                {
                    cur_order_info_ptr->next_ptr->prev_ptr =PNULL;
                }
            }
            else  //���������ͷ
            {
                cur_order_info_ptr->prev_ptr->next_ptr = cur_order_info_ptr->next_ptr;
                if (cur_order_info_ptr->next_ptr != PNULL)
                {
                    cur_order_info_ptr->next_ptr->prev_ptr = cur_order_info_ptr->prev_ptr;
                }            
            }
            
            //insert to handled link
            MMISMS_GetOrderIdByStorage( &order_id,
                (MMISMS_MSG_TYPE_E)cur_order_info_ptr->flag.msg_type,
                (MN_DUAL_SYS_E)cur_order_info_ptr->flag.dual_sys,
                (MN_SMS_STORAGE_E)cur_order_info_ptr->flag.storage,
                cur_order_info_ptr->record_id);
            
            if (order_id == PNULL) //������õ�order list��û���յ�����sms or mms ����Ϣ
            {    
                MMISMS_InsertSmsToOrder(cur_order_info_ptr); //������sms or mms ����Ϣ���뵽order list��ȥ
            }
            next_order_info_ptr = cur_order_info_ptr->next_ptr;
            SCI_FREE(cur_order_info_ptr);
            cur_order_info_ptr = next_order_info_ptr;
        }  
        else
        {
            cur_order_info_ptr = cur_order_info_ptr->next_ptr; 
        }
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : ��δ�������Ϣ�������ҵ�mms push����ȡ��Ȼ���δ����������ɾ����
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ReadMMSPushFromUnhandledList (void)
{
    MMISMS_STATE_T *cur_order_info_ptr = PNULL;
    BOOLEAN ret_val = FALSE;
    MMISMS_STATE_T *next_order_info_ptr = PNULL;
    cur_order_info_ptr = s_mmisms_order.not_handled_head_ptr;
    
    if (MMISMS_GetDelAllFlag() 
        #ifdef MMI_SMS_MOVE_SUPPORT
        || MMISMS_GetMoveAllFlag()
        #endif
        )
    {
        return FALSE;
    }

    MMIAPISMS_SetSavePush(FALSE);
    
    while (cur_order_info_ptr != PNULL )
    {
        if (   cur_order_info_ptr->flag.mo_mt_type == MMISMS_MT_NOT_DOWNLOAD
            || cur_order_info_ptr->flag.mo_mt_type == MMISMS_MT_NOT_NOTIFY
            || cur_order_info_ptr->flag.mo_mt_type == MMISMS_MT_NEED_NOT_DOWNLOAD) //is mms push
        {            
            //delete from unhandled link
            if (cur_order_info_ptr->prev_ptr == PNULL) //�������ͷ
            {
                s_mmisms_order.not_handled_head_ptr = cur_order_info_ptr->next_ptr;
                if (cur_order_info_ptr->next_ptr != PNULL)
                {
                    cur_order_info_ptr->next_ptr->prev_ptr =PNULL;
                }
            }
            else  //���������ͷ
            {
                cur_order_info_ptr->prev_ptr->next_ptr = cur_order_info_ptr->next_ptr;
                if (cur_order_info_ptr->next_ptr != PNULL)
                {
                    cur_order_info_ptr->next_ptr->prev_ptr = cur_order_info_ptr->prev_ptr;
                }            
            }

            MMISMS_SetOperStatus(MMISMS_READ_SMS);

            MNSMS_ReadSmsEx((MN_DUAL_SYS_E)cur_order_info_ptr->flag.dual_sys,
                (MN_SMS_STORAGE_E)cur_order_info_ptr->flag.storage,
                cur_order_info_ptr->record_id);
            next_order_info_ptr = cur_order_info_ptr->next_ptr;           
            SCI_FREE(cur_order_info_ptr);
            cur_order_info_ptr = next_order_info_ptr;
            ret_val = TRUE;
            break;
        } 
        else
        {
            cur_order_info_ptr = cur_order_info_ptr->next_ptr;  
        }
    }

    return ret_val;
}

/*****************************************************************************/
//     Description : find the node have the same ref in link list
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL MMISMS_STATE_T *FindNodeInLinkByRef(MMISMS_STATE_T *link_head_ptr, 
                                          MMISMS_STATE_T *new_sms_ptr, 
                                          MMISMS_TYPE_E type )
{
    MMISMS_STATE_T *cur_state_ptr = PNULL;
    MMISMS_ORDER_ID_T find_state_ptr = PNULL;
    
    cur_state_ptr = link_head_ptr;
    while (cur_state_ptr != PNULL)
    {
        if(CompareSmsState(cur_state_ptr, type, new_sms_ptr) 
            && (!cur_state_ptr->flag.is_receive_all)
            && (!HaveRepeatPart(cur_state_ptr, type, new_sms_ptr))
            )
        {
            find_state_ptr = cur_state_ptr;
            break;
        }
        else
        {
            cur_state_ptr = cur_state_ptr->next_ptr;
        }    
    }
    return cur_state_ptr;
}


/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MT SMS box
//    Global  resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMTSMSForDelAll(void)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    order_id = s_mmisms_order.mt_undownloaded_head_ptr;
    
    while(order_id != PNULL)//is  marked
    {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
        {
            if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        else
#endif
        if (order_id->flag.is_marked && (!order_id->flag.is_locked) )
        {
            break;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }
    if(order_id == PNULL) 
    {
        order_id = s_mmisms_order.mt_unread_head_ptr;
        
        while(order_id != PNULL)//is  marked
        {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
            if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
            {
                if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }            
            }
            else
#endif
            if (order_id->flag.is_marked && (!order_id->flag.is_locked) )
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }
        if (order_id == PNULL)
        {
            order_id = s_mmisms_order.mt_readed_head_ptr;
            while(order_id != PNULL)//is  marked
            {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
                if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
                {
                    if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
                    {
                        break;
                    }
                    else
                    {
                        order_id = order_id->next_ptr;
                    }            
                }
                else
#endif
                if (order_id->flag.is_marked && (!order_id->flag.is_locked) )
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
        }        
    }
    return order_id;    
}

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMoSMSForDelAll(MMISMS_BOX_TYPE_E box_type )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    switch(box_type)
    {
    case MMISMS_BOX_SENDSUCC:
        order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        order_id = s_mmisms_order.mo_draft_head_ptr;
        break;
        
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)    
    case MMISMS_BT_BOX_SENDSUCC:
        order_id = s_mmisms_order.bt_mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BT_BOX_SENDFAIL:
        order_id = s_mmisms_order.bt_mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BT_BOX_NOSEND:
        order_id = s_mmisms_order.bt_mo_draft_head_ptr;
        break;
#endif

    default:
        break;
    }

    while (PNULL != order_id)
    {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
        {
            if(order_id->flag.is_marked
                && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) //is  marked
                && (!mail_GetMailSendingFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) )
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        else
#endif
        if(order_id->flag.is_marked && (!order_id->flag.is_locked) ) //is  marked
        {
            break;
        }
        else
        {
            order_id = order_id->next_ptr;
        }            
    }    
    
    return order_id ;
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related Security SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecuritySMSForDelAll(void)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;

    if((MMISMS_MOVE_SMS_TO_SECURITY == g_mmisms_global.operation.oper_status)
       || (MMISMS_MOVE_SMS_FROM_SECURITY == g_mmisms_global.operation.oper_status)
#if defined(MMI_SMS_EXPORT_SUPPORT)
       || (MMISMS_EXPORT_SMS == g_mmisms_global.operation.oper_status)
#endif
       )
    {
        while(order_id != PNULL)//is  marked
        {
            if (order_id->flag.is_marked)
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }
        if(order_id == PNULL) 
        {
            order_id = s_mmisms_order.security_mt_unread_head_ptr;
            
            while(order_id != PNULL)//is  marked
            {
                if (order_id->flag.is_marked)
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
            if (order_id == PNULL)
            {
                order_id = s_mmisms_order.security_head_ptr;
                while(order_id != PNULL)//is  marked
                {
                    if (order_id->flag.is_marked)
                    {
                        break;
                    }
                    else
                    {
                        order_id = order_id->next_ptr;
                    }
                }
            }        
        }
    }
    else
    {
        while(order_id != PNULL)//is  marked
        {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
            if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
            {
                if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }            
            }
            else
#endif
            if (order_id->flag.is_marked && (!order_id->flag.is_locked))
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }
        if(order_id == PNULL) 
        {
            order_id = s_mmisms_order.security_mt_unread_head_ptr;
            
            while(order_id != PNULL)//is  marked
            {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
                if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
                {
                    if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
                    {
                        break;
                    }
                    else
                    {
                        order_id = order_id->next_ptr;
                    }            
                }
                else
#endif
                if (order_id->flag.is_marked && (!order_id->flag.is_locked))
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
            if (order_id == PNULL)
            {
                order_id = s_mmisms_order.security_head_ptr;
                while(order_id != PNULL)//is  marked
                {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
                    if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
                    {
                        if(order_id->flag.is_marked
                            && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) //is  marked
                            && (!mail_GetMailSendingFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) )
                        {
                            break;
                        }
                        else
                        {
                            order_id = order_id->next_ptr;
                        }            
                    }
                    else
#endif
                    if (order_id->flag.is_marked && (!order_id->flag.is_locked))
                    {
                        break;
                    }
                    else
                    {
                        order_id = order_id->next_ptr;
                    }
                }
            }        
        }
    }

    return order_id;    
}

//for delete all msg begin 11/02/20
/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related SMS box
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidBoxMsgForDelAll(MMISMS_BOX_TYPE_E box_type)
{
    MMISMS_ORDER_ID_T find_pos = PNULL;
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    switch(box_type)
    {
    case MMISMS_BOX_MT:
        order_id = s_mmisms_order.mt_undownloaded_head_ptr;
        find_pos = FindOrderNodeIdFromLink(order_id);       
        if (PNULL == find_pos)
        {
            order_id = s_mmisms_order.mt_unread_head_ptr;
            find_pos = FindOrderNodeIdFromLink(order_id);            
            if (PNULL == find_pos)
            {
                order_id = s_mmisms_order.mt_readed_head_ptr;
                find_pos = FindOrderNodeIdFromLink(order_id);                
            }
        }
        break;
        
    case MMISMS_BOX_SENDSUCC:
        order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        find_pos = FindOrderNodeIdFromLink(order_id);
        break;
        
    case MMISMS_BOX_SENDFAIL:
        order_id = s_mmisms_order.mo_sendfail_head_ptr;
        find_pos = FindOrderNodeIdFromLink(order_id);
        break;
        
    case MMISMS_BOX_NOSEND:
        order_id = s_mmisms_order.mo_draft_head_ptr;
        find_pos = FindOrderNodeIdFromLink(order_id);
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;
        find_pos = FindOrderNodeIdFromLink(order_id);
        if (PNULL == find_pos)
        {
            order_id = s_mmisms_order.security_mt_unread_head_ptr;
            find_pos = FindOrderNodeIdFromLink(order_id);
            if (PNULL == find_pos)
            {
                order_id = s_mmisms_order.security_head_ptr;
                find_pos = FindOrderNodeIdFromLink(order_id);
            }
        }
    	break;
#endif
    case MMISMS_BOX_ALL:
        {
            MMISMS_LINKED_ORDER_UNIT_T *linked_order_id = PNULL;

            linked_order_id = s_mmisms_linked_order.used_unit_head_ptr;
            while (PNULL != linked_order_id)
            {
                order_id = &linked_order_id->sms_state;
                if (!order_id->flag.is_locked)
                {
                    if (MMISMS_TYPE_MMS == order_id->flag.msg_type && (MMIAPIMMS_IsProcessMMSById(order_id->record_id) ||MMIAPIUDISK_UdiskIsRun() ))
                    {
                        linked_order_id = linked_order_id->next_ptr;
                    }
                    else
                    {
                        find_pos = order_id;
                        break;
                    }                    
                }
                else
                {
                    linked_order_id = linked_order_id->next_ptr;
                }
            }
        }
    	break;
        
    default:
        break;
    }
    return find_pos;
}

/*****************************************************************************/
//  Description : find a valid delete node
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T FindOrderNodeIdFromLink(MMISMS_ORDER_ID_T link_order_id)
{
    MMISMS_ORDER_ID_T find_node = PNULL;
    while (PNULL != link_order_id)
    {
        if (!link_order_id->flag.is_locked)
        {
            if (MMISMS_TYPE_MMS == link_order_id->flag.msg_type && (MMIAPIMMS_IsProcessMMSById(link_order_id->record_id) ||MMIAPIUDISK_UdiskIsRun() ))
            {
                link_order_id = link_order_id->next_ptr;
            }
            else
            {
                find_node = link_order_id;
                break;
            }            
        }
        else
        {
            link_order_id = link_order_id->next_ptr;
        }
    }
    return find_node;
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related msg type
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidTypeMsgForDelAll(MMISMS_MSG_TYPE_E msg_type)
{
    
    MMISMS_ORDER_ID_T find_pos = PNULL;
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_LINKED_ORDER_UNIT_T *linked_order_id = PNULL;

    linked_order_id = s_mmisms_linked_order.used_unit_head_ptr;
    while (PNULL != linked_order_id)
    {
        order_id = &linked_order_id->sms_state;
        if (msg_type == order_id->flag.msg_type && !order_id->flag.is_locked && MMISMS_FOLDER_SECURITY != order_id->flag.folder_type)
        {
            if (MMISMS_TYPE_MMS == order_id->flag.msg_type && (MMIAPIMMS_IsProcessMMSById(order_id->record_id) ||MMIAPIUDISK_UdiskIsRun()) )
            {
                linked_order_id = linked_order_id->next_ptr;
            }
            else
            {
                find_pos = order_id;
                break;
            }            
        }
        else
        {
            linked_order_id = linked_order_id->next_ptr;
        }
    }
    return find_pos;
}

/*****************************************************************************/
//  Description : find valid sms or mms to reply
//                  deleting the related msg type
//  Global  resource dependence : 
//  Author: rong.gu
//  Note:���ڻظ�ȫ����Ϣ������������Ƿ��п��Իظ�����Ϣ
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsHaveValidMsgToReply(void)
{    
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_LINKED_ORDER_UNIT_T *linked_order_id = PNULL;

    linked_order_id = s_mmisms_linked_order.used_unit_head_ptr;
    while (PNULL != linked_order_id)
    {
        order_id = &linked_order_id->sms_state;
        if ((MMISMS_TYPE_MMS == order_id->flag.msg_type || MMISMS_TYPE_SMS == order_id->flag.msg_type) && order_id->flag.is_marked)
        {
            if (MMISMS_TYPE_MMS == order_id->flag.msg_type && (MMIAPIMMS_IsProcessMMSById(order_id->record_id) ||MMIAPIUDISK_UdiskIsRun()) )
            {
                linked_order_id = linked_order_id->next_ptr;
            }
            else
            {
                return TRUE;
            }            
        }
        else
        {
            linked_order_id = linked_order_id->next_ptr;
        }
    }
    return FALSE;
}


/*****************************************************************************/
//  Description : get number of msg type from security box
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_MSG_TYPE_E msg_type)
{
    uint16 msg_num = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;

    order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;
    while (PNULL != order_id)
    {
        if (msg_type == order_id->flag.msg_type && MMISMS_FOLDER_SECURITY == order_id->flag.folder_type)
        {
            msg_num++;
        }
        order_id = order_id->next_ptr;
    }
    order_id = s_mmisms_order.security_mt_unread_head_ptr;
    while (PNULL != order_id)
    {
        if (msg_type == order_id->flag.msg_type && MMISMS_FOLDER_SECURITY == order_id->flag.folder_type)
        {
            msg_num++;
        }
        order_id = order_id->next_ptr;
    }
    order_id = s_mmisms_order.security_head_ptr;
    while (PNULL != order_id)
    {
        if (msg_type == order_id->flag.msg_type && MMISMS_FOLDER_SECURITY == order_id->flag.folder_type)
        {
            msg_num++;
        }
        order_id = order_id->next_ptr;
    }
    return msg_num;
}
//for delete all msg end 11/02/20


/*****************************************************************************/
//     Description : to get the record id of pointed_pos in list box
//    Global resource dependence : g_mmisms_listbuff
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC MN_SMS_RECORD_ID_T MMISMS_GetPointedPosRecordID(
                                                       MMISMS_BOX_TYPE_E box_type,
                                                       uint16    selected_index    //IN:
                                                       )
{
    MMISMS_ORDER_ID_T order_id = PNULL;

    order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type,selected_index);
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetPointedPosRecordID order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5088_112_2_18_2_54_3_644,(uint8*)"");
        return 0;
    }
    return (order_id->record_id);
}

/*****************************************************************************/
//     Description : the function is called when the order is change
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMISMS_HandleOrderChangeCallBack (void)
{
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
#ifdef MMI_SMS_CHAT_SUPPORT
    if(MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_CHATBOX_CHILD_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list����    
    }
#endif

#ifdef MMI_SMS_DELALL_SUPPORT    
    if (MMK_IsOpenWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list����    
    } 
#endif
        
    if (MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_NOSENDBOX_CHILD_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list����    
    } 

    if (MMK_IsOpenWin(MMISMS_PUSH_MESSAGE_WIN_ID))
    {
        MMK_SendMsg(MMISMS_PUSH_MESSAGE_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list����    
    } 
#endif    
    if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
    {
        MMK_SendMsg(MMISMS_MSGBOX_MAIN_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list����    
    } 

    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        if (MMIMSG_MAIN_MENU_WIN_ID!=MMK_GetFocusWinId())
        {
            MMK_SendMsg(MMIMSG_MAIN_MENU_WIN_ID,
                MMISMS_MSG_NEEDUPDATEMENU,
                PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����MENU��            
        }
        else
        {
            MMK_SendMsg(MMIMSG_MAIN_MENU_WIN_ID, 
                MMISMS_MSG_UPDATEMENU, 
                PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����MENU��
        }
    }

    if (MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_SECURITYBOX_CHILD_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list����    
    } 

#ifdef SEARCH_SUPPORT
    MMIAPISEARCH_AppUpdateInd(MMISEARCH_TYPE_SMS);
#endif

    if (MMK_IsOpenWin(MMISMS_MEMORY_STATE_WIN_ID))
    {
        if (MMISMS_MEMORY_STATE_WIN_ID!=MMK_GetFocusWinId())
        {
            MMK_SendMsg(MMISMS_MEMORY_STATE_WIN_ID,
                MMISMS_MSG_NEEDUPDATEMENU,
                PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����Memory Content��
        }
        else
        {
            MMK_SendMsg(MMISMS_MEMORY_STATE_WIN_ID, 
                MMISMS_MSG_UPDATEMENU, 
                PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����Memory Content��
        }
    }
#ifdef MMI_SMS_DELALL_SUPPORT 
    if (MMK_IsOpenWin(MMISMS_MSGDEL_ALL_WIN_ID))
    {
        MMK_SendMsg(MMISMS_MSGDEL_ALL_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list����    
    } 
#endif
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    if (MMK_IsOpenWin(MMISMS_BT_MTBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_MTBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }
    
    if (MMK_IsOpenWin(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }

    if (MMK_IsOpenWin(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }

    if (MMK_IsOpenWin(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }
#endif
    MAIN_SetMsgState(TRUE);

    return TRUE;
}

/*****************************************************************************/
//     Description : ��ȡ������SIM SMS��Ŀ
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL uint16 HandleMaxSimSMSNum(void)
{
    uint32 i = 0;
    uint16 max_sim_num = 0;
    uint16 curr_sim_num = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        max_sim_num += MMISMS_GetMaxSimSmsNum((MN_DUAL_SYS_E)i);
    }

    if (MMISMS_MAX_SYS_SIM_SMS_NUM < max_sim_num)
    {
        //SCI_TRACE_LOW:"HandleMaxSimSMSNum max_sim_num=%d exceed limit!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5210_112_2_18_2_54_4_645,(uint8*)"d", max_sim_num);

        max_sim_num = 0;
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            curr_sim_num = MMISMS_GetMaxSimSmsNum((MN_DUAL_SYS_E)i);
            if(curr_sim_num > MMISMS_MAX_SYS_SIM_SMS_NUM/MMI_DUAL_SYS_MAX)
            {
                curr_sim_num = MMISMS_MAX_SYS_SIM_SMS_NUM/MMI_DUAL_SYS_MAX;
            }
            MMISMS_SetMaxSimSmsNum((MN_DUAL_SYS_E)i, curr_sim_num);
            max_sim_num += curr_sim_num;
        }

    }
    else
    {
        max_sim_num = MMISMS_MAX_SYS_SIM_SMS_NUM;//������ڴ����
    }

    return max_sim_num;
}

/*****************************************************************************/
//     Description : ��ʼ��order list
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_OrderInit(void)
{
    // uint16 i = 0;
    uint16 max_num = 0;
    uint16 used_num = 0;
    uint16 max_sim_num = 0;
        
    s_mmisms_order.max_sms_num.total_msg_num = 0;
    
    // get the max number of SMS saved in nv
    MNSMS_GetSmsNumEx(MN_DUAL_SYS_1, MN_SMS_STORAGE_ME, &max_num, &used_num);
    s_mmisms_order.max_sms_num.total_msg_num += max_num;
    
    //��ö���Ϣ���������
    max_sim_num = HandleMaxSimSMSNum();

    s_mmisms_order.max_sms_num.total_msg_num += max_sim_num;
#ifdef MMS_SUPPORT    
    s_mmisms_order.max_sms_num.total_msg_num += MMIMMS_MAX_MMS_NUM;// get the mms num
#endif    
    s_mmisms_order.max_sms_num.total_msg_num += MMICMSBRW_MAX_ITEM_NUM; //get the push num
    s_mmisms_order.max_sms_num.total_msg_num += MMIOTA_CONFIG_INFO_MAX; //get the ota num
    
    //SCI_TRACE_LOW:"mmisms:MMISMS_OrderInit max_sms_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5259_112_2_18_2_54_4_646,(uint8*)"d", s_mmisms_order.max_sms_num.total_msg_num);
    
    LinkedArrayInit(s_mmisms_order.max_sms_num.total_msg_num); 
    
    return TRUE;
}

/*****************************************************************************/
//     Description : �����������ͣ��Ƚ϶���״̬��
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareStateForSort (
                                 MMISMS_ORDERTYPE_E    oreder_type,      //IN    ��Ϣ�Ƚϵ����� 
                                 MMISMS_STATE_T        *sms1_state_ptr,  //IN: SMS1 state     
                                 MMISMS_STATE_T        *sms2_state_ptr   //IN: SMS2 state
                                 )
{
    int32 ret_val = 0;
    MMI_STRING_T name1 = {0};
    MMI_STRING_T name2 = {0};
    MMI_STRING_T subject1 = {0};
    MMI_STRING_T subject2 = {0};
    // uint8 pinyinsn1_length = 0;
    uint8 *pinyinsn_string1_ptr; 
    // uint8 pinyinsn2_length = 0;
    uint8 *pinyinsn_string2_ptr; 
    
    wchar temp_arr1[MMIMESSAGE_DIAPLAY_LEN+1] = {0};
    wchar temp_arr2[MMIMESSAGE_DIAPLAY_LEN+1] = {0};
    uint8 temp_arr3[MMIMESSAGE_DIAPLAY_LEN*sizeof(wchar)] = {0};
    uint8 temp_arr4[MMIMESSAGE_DIAPLAY_LEN*sizeof(wchar)] = {0};
    
    pinyinsn_string2_ptr = temp_arr4;
    pinyinsn_string1_ptr = temp_arr3;
    
    if (PNULL == sms1_state_ptr || PNULL == sms2_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:CompareStateForSort sms1_state_ptr sms2_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5296_112_2_18_2_54_4_647,(uint8*)"");
        return ret_val;
    }
    
    switch(oreder_type)
    {
    case MMISMS_ORDER_BY_TIME:
        if (sms1_state_ptr->time < sms2_state_ptr->time)
        {
            ret_val = 1;
        }
        else
        {
            if (sms1_state_ptr->time > sms2_state_ptr->time )
            {
                ret_val = -1;
            }
            else
            {
                ret_val = 0;
            }
        }
        break;
        
    case MMISMS_ORDER_BY_NAME:
        name1.wstr_ptr = temp_arr1;
        name2.wstr_ptr = temp_arr2;
#ifdef MMISMS_SORT_CAHCE_BUFFER_SUPPORT
            if (!FindSortCacheNode(sms1_state_ptr, &name1.wstr_ptr, &name1.wstr_len))
            {
        MMISMS_GetNameInfo(sms1_state_ptr, &name1, MMIMESSAGE_DIAPLAY_LEN);
                name1.wstr_len = (uint16)MIN(name1.wstr_len, MMIMESSAGE_DIAPLAY_LEN);
                AddSortCacheNode(sms1_state_ptr, name1.wstr_ptr, name1.wstr_len);
            }
            if (!FindSortCacheNode(sms2_state_ptr, &name2.wstr_ptr, &name2.wstr_len))
            {
        MMISMS_GetNameInfo(sms2_state_ptr, &name2, MMIMESSAGE_DIAPLAY_LEN);
                name2.wstr_len = (uint16)MIN(name2.wstr_len, MMIMESSAGE_DIAPLAY_LEN);
                AddSortCacheNode(sms2_state_ptr, name2.wstr_ptr, name2.wstr_len);
            }
#else
            MMISMS_GetNameInfo(sms1_state_ptr, &name1, MMIMESSAGE_DIAPLAY_LEN);
            MMISMS_GetNameInfo(sms2_state_ptr, &name2, MMIMESSAGE_DIAPLAY_LEN);
            name1.wstr_len = (uint16)MIN(name1.wstr_len, MMIMESSAGE_DIAPLAY_LEN);
            name2.wstr_len = (uint16)MIN(name2.wstr_len, MMIMESSAGE_DIAPLAY_LEN);
#endif

        ret_val = MMIAPIPB_CmpStrByLetters( name1.wstr_ptr,
                                            name1.wstr_len,
                                            name2.wstr_ptr,
                                            name2.wstr_len);
        break;
        
    case MMISMS_ORDER_BY_CONTENT:
        subject1.wstr_ptr = temp_arr1;
        subject2.wstr_ptr = temp_arr2;

        subject1.wstr_len  = MIN(sms1_state_ptr->display_content.content_len, MMIMESSAGE_DIAPLAY_LEN);
        MMI_WSTRNCPY(
            subject1.wstr_ptr,
            MMIMESSAGE_DIAPLAY_LEN,
            sms1_state_ptr->display_content.content,
            MMIMESSAGE_DIAPLAY_LEN,
            subject1.wstr_len
            );
        
        subject2.wstr_len  = MIN(sms2_state_ptr->display_content.content_len, MMIMESSAGE_DIAPLAY_LEN);
        MMI_WSTRNCPY(
            subject2.wstr_ptr,
            MMIMESSAGE_DIAPLAY_LEN,
            sms2_state_ptr->display_content.content,
            MMIMESSAGE_DIAPLAY_LEN,
            subject2.wstr_len
            );

        ret_val = MMIAPIPB_CmpStrByLetters(subject1.wstr_ptr,
                                                                            subject1.wstr_len,
                                                                            subject2.wstr_ptr,
                                                                            subject2.wstr_len);
        break;
        
    case MMISMS_ORDER_BY_TYPE:
        if (sms1_state_ptr->flag.msg_type > sms2_state_ptr->flag.msg_type)
        {
            ret_val = 1;
        }
        else
        {
            if (sms1_state_ptr->flag.msg_type < sms2_state_ptr->flag.msg_type )
            {
                ret_val = -1;
            }
            else
            {
                ret_val = 0;
            }
        }
        break;

    default:
        break;
    }
    
    if ((ret_val == 0) && (MMISMS_ORDER_BY_TIME != oreder_type)) //������/����/��ϵ����ͬ�������,��Ƚ�ʱ��
    {
        if (sms1_state_ptr->time < sms2_state_ptr->time)
        {
            ret_val = 1;
        }
        else
        {
            if (sms1_state_ptr->time > sms2_state_ptr->time )
            {
                ret_val = -1;
            }
            else
            {
                ret_val = 0;
            }
        }        
    }
    return ret_val;
    
}

/*****************************************************************************/
//     Description :������Ӧ�Ĺ�������˫������(�û���ѡ����������򣺰���ʱ�䡢��ϵ�ˡ����͡�����/����)
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL MMISMS_STATE_T* SortOrderList(
                                    MMISMS_STATE_T*        head,        //IN:�����������ͷ
                                    MMISMS_ORDERTYPE_E    oreder_type  //IN:�������
                                    )
{
    MMISMS_STATE_T  *cur_ptr = PNULL;
    MMISMS_STATE_T** temp_arry = PNULL;
    uint32 i = 0, unit_num = 0;
    
    if (PNULL == head)
    {
        //SCI_TRACE_LOW:"MMISMS:SortOrderList head = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5422_112_2_18_2_54_4_648,(uint8*)"");
        return PNULL;
    }
    
    temp_arry = SCI_ALLOCAZ(s_mmisms_order.max_sms_num.total_msg_num * sizeof(MMISMS_STATE_T*) );
    if(PNULL == temp_arry)
    {
        //SCI_TRACE_LOW:"MMISMS:SortOrderList temp_arry alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5429_112_2_18_2_54_4_649,(uint8*)"");
        return PNULL;
    }
    
    //����������������
    for(cur_ptr=head; cur_ptr != PNULL; cur_ptr=cur_ptr->next_ptr)
    {
        temp_arry[i] = cur_ptr;        
        i ++;
    }
    
    unit_num = i;
    
    switch(oreder_type) 
    {
    case MMISMS_ORDER_BY_TIME:
        qsort((void*)temp_arry, unit_num, sizeof(MMISMS_STATE_T*), CompareSmsStateByTime );/*lint !e64*/
        break;
        
    case MMISMS_ORDER_BY_NAME  :
        qsort((void*)temp_arry, unit_num, sizeof(MMISMS_STATE_T*), CompareSmsStateByName);/*lint !e64*/
        break;
        
    case MMISMS_ORDER_BY_CONTENT:
        qsort((void*)temp_arry, unit_num, sizeof(MMISMS_STATE_T*), CompareSmsStateBySubject);/*lint !e64*/
        break;
        
    case MMISMS_ORDER_BY_TYPE:
        qsort((void*)temp_arry, unit_num, sizeof(MMISMS_STATE_T*), CompareSmsStateByType);/*lint !e64*/
        break;
        
    default:
        break;
        
    }
    //����������������
    for (i = 0; i< (unit_num-1); i++)
    {
        temp_arry[i]->next_ptr = temp_arry[i+1];
        temp_arry[i+1]->prev_ptr = temp_arry[i];
    }
    if(unit_num > 0)
    {
        temp_arry[unit_num-1]->next_ptr = PNULL;
    }
    temp_arry[0]->prev_ptr = PNULL;
    head = temp_arry[0];
    SCI_Free(temp_arry);
    return(head);
}

/*****************************************************************************/
//     Description :�õ�name��Ϣ��
//    Global resource dependence : none
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_GetNameInfo( MMISMS_STATE_T *sms_state_info, //IN:
                              MMI_STRING_T      *name,              //OUT:
							  uint16 max_name_len //IN:
                              )                                    
{
    if (MMISMS_TYPE_SMS  != sms_state_info->flag.msg_type )  //mms,wap push, wap ota, mms ota
    {
        if(!MMISMS_GetNameByNumberFromPB(sms_state_info->number.sender, name, max_name_len) )
        {
            name->wstr_len = SCI_STRLEN((char *)sms_state_info->number.sender);
            name->wstr_len = MIN(max_name_len,  name->wstr_len );
            MMI_STRNTOWSTR(name->wstr_ptr,
                                    name->wstr_len,
                                    (uint8 *)sms_state_info->number.sender,
                                    name->wstr_len,
                                    name->wstr_len);
        }            
    }
    else
    {
        switch(sms_state_info->flag.mo_mt_type) 
        {
        case MMISMS_MT_TO_BE_READ:
        case MMISMS_MT_HAVE_READ:
        case MMISMS_MO_SEND_FAIL:
        case MMISMS_MO_SEND_SUCC:
        case MMISMS_MT_SR_HAVE_READ:
        case MMISMS_MT_SR_TO_BE_READ:
            FindNameAndStoreInfoByNum(
                MMIPB_GetNumberTypeFromUint8(sms_state_info->number.bcd_number.npi_ton),
                MMIPB_GetNumberPlanFromUint8(sms_state_info->number.bcd_number.npi_ton),
                (uint8) (sms_state_info->number.bcd_number.number_len ),
                (uint8*) (sms_state_info->number.bcd_number.number),
                max_name_len,
                sms_state_info->flag.mo_mt_type,
                name
                );
            break;            
        case MMISMS_MO_DRAFT:
            if (sms_state_info->number.bcd_number.number_len > 0)
            {
                FindNameAndStoreInfoByNum(
                    MMIPB_GetNumberTypeFromUint8(sms_state_info->number.bcd_number.npi_ton),
                    MMIPB_GetNumberPlanFromUint8(sms_state_info->number.bcd_number.npi_ton),
                    (uint8) (sms_state_info->number.bcd_number.number_len ),
                    (uint8*) (sms_state_info->number.bcd_number.number),
                    max_name_len,
                    sms_state_info->flag.mo_mt_type,
                    name
                    );
            }
            break;            
        default:
            break;
        }        
    }
}



/*****************************************************************************/
//     Description : lock the message
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurMsgLocked(
                                   BOOLEAN is_locked //IN:
                                   )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    MMISMS_SetCurOrderArrIndex(0);
    order_id = MMISMS_GetCurOperationOrderId(); 
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_SetCurMsgLocked order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5621_112_2_18_2_54_5_650,(uint8*)"");
        return;
    }    
    cur_order_id = order_id;
    
    cur_order_id->flag.is_locked = is_locked;
    
    switch(cur_order_id->flag.msg_type)
    {
    case MMISMS_TYPE_SMS:
        while (cur_order_id != PNULL)
        {
            if (MN_SMS_STORAGE_ME == cur_order_id->flag.storage ) //sim���Ķ���Ϣ����������������
            {
                MMISMS_NV_SetSmsLockState(cur_order_id->record_id, is_locked);
            }
            cur_order_id->flag.is_locked = is_locked;
            cur_order_id = cur_order_id->next_long_sms_ptr;
        }
        break; 
        
    case MMISMS_TYPE_MMS:
        if (!MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPIMMS_SetLockFlag(cur_order_id->record_id, is_locked);
            MMIAPIMMS_SaveListInfo();
        }                
        break;
        
    case MMISMS_TYPE_WAP_PUSH:  
        if (!MMIAPIUDISK_UdiskIsRun())
        {
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_SetPushLockFlag(cur_order_id->record_id, is_locked);
            MMIAPIOTA_SavePushFileInfo();
#endif
        }        
        break;
        
    case MMISMS_TYPE_MMS_OTA:
    case MMISMS_TYPE_WAP_OTA:
        if (!MMIAPIUDISK_UdiskIsRun())
        {
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_SetOTAPUSHLockFlag(cur_order_id->record_id, is_locked);
            MMIAPIOTA_UpdateOTAFileInfo();
#endif
        }        
        break;
        
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    case MMISMS_TYPE_MAIL:  
        if (!MMIAPIUDISK_UdiskIsRun())
        {
            //����������Ϣ
            mail_SetMailLockFlag(cur_order_id->record_id, is_locked);
            //����������Ϣ
            mail_SaveMailListInfo();

        }        
        break;
#endif//PUSH_EMAIL_3_IN_1_SUPPORT 

    default:
        break;
    }
}

/*****************************************************************************/
//     Description : lock or unlock the marked message
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMarkedMsgLock(MMISMS_BOX_TYPE_E box_type,BOOLEAN is_lock)
{
    MMISMS_LOCK_FLAG_STATUS lock_flag = 0;

    switch(box_type)
    {
    case MMISMS_BOX_MT:
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.mt_undownloaded_head_ptr, is_lock);        
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.mt_unread_head_ptr, is_lock);         
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.mt_readed_head_ptr, is_lock);          
        break;
        
    case MMISMS_BOX_SENDSUCC:
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.mo_sendsucc_head_ptr, is_lock);        
        break;
        
    case MMISMS_BOX_SENDFAIL:
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.mo_sendfail_head_ptr, is_lock);
        break;
        
    case MMISMS_BOX_NOSEND:
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.mo_draft_head_ptr, is_lock);
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.security_mt_undownloaded_head_ptr, is_lock);        
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.security_mt_unread_head_ptr, is_lock); 
        lock_flag |= LockMarkedMsgInLink(s_mmisms_order.security_head_ptr, is_lock);
        break; 
#endif        
    default:
        break;
    }

    if (lock_flag & MMISMS_MMS_LOCK_FLAG)//����в���������д�뵽�ļ�
    {
        MMIAPIMMS_SaveListInfo();
//        MMIMMS_SaveFolderInfo();    
    }
    
    if (lock_flag & MMISMS_WAPPUSH_LOCK_FLAG)//�����wap push������д�뵽�ļ�
    {
    #ifdef MMI_OTA_SUPPORT
        MMIAPIOTA_SavePushFileInfo();        
    #endif
    }
    
    if (lock_flag & MMISMS_OTA_LOCK_FLAG)//�����ota��Ϣ������д�뵽�ļ�
    {   
#ifdef MMI_OTA_SUPPORT
        MMIAPIOTA_UpdateOTAFileInfo();
#endif
    } 
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    if (lock_flag & MMISMS_MAIL_LOCK_FLAG)//�����mail��Ϣ������д�뵽�ļ�
    {   
// TODO: ����Ϣ����д�������ļ� /*-sunguochen 2010.8.16 -*/
        mail_SaveMailListInfo();        
    } 
#endif//PUSH_EMAIL_3_IN_1_SUPPORT 
}

/*****************************************************************************/
//     Description : get the message is locked or not
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CurMsgIsLocked(void )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    BOOLEAN is_locked = FALSE;
    MMISMS_SetCurOrderArrIndex(0);
    order_id = MMISMS_GetCurOperationOrderId();
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_CurMsgIsLocked order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5759_112_2_18_2_54_5_651,(uint8*)"");
        return FALSE;
    }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
    {
        is_locked = mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id));
    }
    else
#endif
    is_locked = order_id->flag.is_locked;

    return is_locked; 
}
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
PUBLIC BOOLEAN MMIMAIL_CurMailIsSending(void )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    BOOLEAN is_sending = FALSE;
    MMISMS_SetCurOrderArrIndex(0);

    order_id = MMISMS_GetCurOperationOrderId();

    if(PNULL != order_id && MMISMS_TYPE_MAIL == order_id->flag.msg_type)
    {
        is_sending = mail_GetMailSendingFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id));
    }

    return is_sending; 
}
#endif

/*****************************************************************************/
// Description : get the message record id
// Global resource dependence : 
// Author:minghu.mao
// Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_CurMsgRecordId(void )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    uint32 record_id           = 0;

    MMISMS_SetCurOrderArrIndex(0);
    order_id = MMISMS_GetCurOperationOrderId(); 
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_CurMsgRecordId order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_5812_112_2_18_2_54_5_652,(uint8*)"");
        return record_id;
    }    
    record_id = order_id->record_id;
    
    return record_id; 
}

/*****************************************************************************/
//     Description : set the marked message in link locked or not
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL MMISMS_LOCK_FLAG_STATUS LockMarkedMsgInLink(MMISMS_ORDER_ID_T link_head,
                                                                                                       BOOLEAN is_lock)
{
    MMISMS_ORDER_ID_T order_id = link_head;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_LOCK_FLAG_STATUS lock_flag = 0;
    MMINV_SMS_LOCK_STATE_T lock_state = {0};
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    
    MMINV_READ(MMINV_SMS_LOCK_STATE, &lock_state, nv_value);
    
    if (MN_RETURN_SUCCESS != nv_value)
    {
        MMINV_WRITE(MMINV_SMS_LOCK_STATE, &lock_state);
    }

    while(order_id != PNULL)//������ͷ��ʼ���ұ�ǵĶ���Ϣ������������״̬
    {
        if (order_id->flag.is_marked)
        {
            switch(order_id->flag.msg_type)
            {
            case MMISMS_TYPE_SMS :
                cur_order_id = order_id;
                while (cur_order_id != PNULL)
                {     
                    cur_order_id->flag.is_marked = FALSE; //��������֮��ȥ�����
                    
                    if (cur_order_id->flag.storage == MN_SMS_STORAGE_ME)//sim���Ķ���Ϣ����������������
                    {
                        lock_state.is_locked[cur_order_id->record_id] = is_lock;
                        lock_flag |= MMISMS_SMS_LOCK_FLAG;
                        //bug 872194 mark sim���ϵĶ��ţ�����ѡ��phone�ϵĶ��ţ���ʱsim���ϵĶ����ܱ�������						
                        cur_order_id->flag.is_locked = is_lock;
                    }
                    //cur_order_id->flag.is_locked = is_lock;
                    cur_order_id = cur_order_id->next_long_sms_ptr;
                }                   
                break;
                
            case MMISMS_TYPE_MMS:
                order_id->flag.is_marked = FALSE;
                if (!MMIAPIUDISK_UdiskIsRun())
                {
                    order_id->flag.is_locked = is_lock;
                    lock_flag |= MMISMS_MMS_LOCK_FLAG;
                    MMIAPIMMS_SetLockFlag(order_id->record_id, is_lock);
                }                
                break;
                
            case MMISMS_TYPE_WAP_PUSH:
                order_id->flag.is_marked = FALSE;
                if (!MMIAPIUDISK_UdiskIsRun())
                {
                    order_id->flag.is_locked = is_lock; 
                    lock_flag |= MMISMS_WAPPUSH_LOCK_FLAG;                
#ifdef MMI_OTA_SUPPORT
                    MMIAPIOTA_SetPushLockFlag(order_id->record_id, is_lock);
                    MMIAPIOTA_SavePushFileInfo();
#endif
                }                
                break;
                
            case MMISMS_TYPE_MMS_OTA:
            case MMISMS_TYPE_WAP_OTA:
                order_id->flag.is_marked = FALSE; 
                if (!MMIAPIUDISK_UdiskIsRun())
                {
                    lock_flag |= MMISMS_OTA_LOCK_FLAG;
                    order_id->flag.is_locked = is_lock;
#ifdef MMI_OTA_SUPPORT
                    MMIAPIOTA_SetOTAPUSHLockFlag(order_id->record_id, is_lock);
#endif
                }                              
                break;

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
            case MMISMS_TYPE_MAIL:
                order_id->flag.is_marked = FALSE;                 
                if (!MMIAPIUDISK_UdiskIsRun())
                {
                    lock_flag |= MMISMS_MAIL_LOCK_FLAG;                    
                    //����״̬Ϊlock
// TODO: ������Ϣ״̬ /*-sunguochen 2010.8.16 -*/
                    mail_SetMailLockFlag(order_id->record_id, is_lock);                    
                }        
                break;
#endif//PUSH_EMAIL_3_IN_1_SUPPORT                
                
            default:
                break;
            }
        }
        order_id = order_id->next_ptr;
    }  

    if (lock_flag & MMISMS_SMS_LOCK_FLAG)
    {
        MMINV_WRITE(MMINV_SMS_LOCK_STATE, &lock_state);
    }

    return lock_flag;
}

/*****************************************************************************/
//     Description : �Ƚ�ʱ�䡣
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateByTime(const void* node1,
                                  const void* node2)
{
    int32 ret_val;
    MMISMS_STATE_T** state1 = (MMISMS_STATE_T*)(node1);/*lint !e64*/
    MMISMS_STATE_T** state2 = (MMISMS_STATE_T*)(node2);/*lint !e64*/
    
    ret_val = CompareStateForSort(MMISMS_ORDER_BY_TIME,
        *state1,
        *state2);
    return ret_val;
}

/*****************************************************************************/
//     Description : �Ƚ϶�����ϵ�ˡ�
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateByName(const void* node1,
                                  const void* node2)
{
    int32 ret_val;
    MMISMS_STATE_T** state1 = (MMISMS_STATE_T*)(node1);/*lint !e64*/
    MMISMS_STATE_T** state2 = (MMISMS_STATE_T*)(node2);/*lint !e64*/
    ret_val =CompareStateForSort(MMISMS_ORDER_BY_NAME,
        *state1,
        *state2);
    return ret_val;
}

/*****************************************************************************/
//     Description : �Ƚ����⡣
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateBySubject(const void* node1,
                                     const void* node2)
{
    int32 ret_val = 0;
    MMISMS_STATE_T** state1 = (MMISMS_STATE_T*)(node1);/*lint !e64*/
    MMISMS_STATE_T** state2 = (MMISMS_STATE_T*)(node2);/*lint !e64*/
    ret_val = CompareStateForSort(MMISMS_ORDER_BY_CONTENT,
        *state1,
        *state2);

    return ret_val;
}
/*****************************************************************************/
//     Description : �Ƚ���Ϣ���͡�
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL int32 CompareSmsStateByType(const void* node1,
                                  const void* node2)
{
    int32 ret_val = 0;
    MMISMS_STATE_T** state1 = (MMISMS_STATE_T*)(node1);/*lint !e64*/
    MMISMS_STATE_T** state2 = (MMISMS_STATE_T*)(node2);/*lint !e64*/
    ret_val =CompareStateForSort(MMISMS_ORDER_BY_TYPE,
        *state1,
        *state2);

    return ret_val;
}

/*****************************************************************************/
//     Description : ���ҵ�ǰ�ĳ�����Ϣ���Ƿ��Ѿ������µĶ���Ϣ�����ͬ��һ��
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN HaveRepeatPart(                                        //RETURN: 
                             MMISMS_STATE_T        *cur_state_ptr,    //IN: current SMS state
                             MMISMS_TYPE_E        type,            //IN: MO or MT
                             MMISMS_STATE_T     *new_state_ptr  //IN: new sms state
                             )
{
    BOOLEAN result = FALSE;
    MMISMS_STATE_T *temp_ptr = cur_state_ptr;
    
    if (CompareSmsState(temp_ptr, type, new_state_ptr))
    {
        while (temp_ptr != PNULL)
        {
            if (temp_ptr->longsms_seq_num == new_state_ptr->longsms_seq_num)
            {
                result = TRUE;
                break;                
            }
            else
            {                            
                temp_ptr = temp_ptr->next_long_sms_ptr;
            }
        }         
    }
    return result;
}

/*****************************************************************************/
//     Description : ĳ����Ϣ�Ƿ�Ϊ��ǰ����Ϣ�е�һ��
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_HaveSamePart(                                        //RETURN: 
                             MMISMS_STATE_T        *cur_state_ptr,    //IN: current SMS state
                             MMISMS_STATE_T     *new_state_ptr  //IN: new sms state
                             )
{
     BOOLEAN result = FALSE;
     result = HaveSamePart(cur_state_ptr,new_state_ptr);
     return result;
}

/*****************************************************************************/
//     Description : ���ҵ�ǰ�ĳ�����Ϣ���Ƿ��Ѿ������µĶ���Ϣ�����ͬ��һ��
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN HaveSamePart(                                        //RETURN: 
                             MMISMS_STATE_T        *cur_state_ptr,    //IN: current SMS state
                             MMISMS_STATE_T     *new_state_ptr  //IN: new sms state
                             )
{
    BOOLEAN result = FALSE;
    MMISMS_STATE_T *temp_ptr = cur_state_ptr;
    
    while (temp_ptr != PNULL)
    {
        if (IsSameSMS(temp_ptr,new_state_ptr))
        {
            result = TRUE;
            break;                
        }
        else
        {                            
            temp_ptr = temp_ptr->next_long_sms_ptr;
        }
    }
    
    if(!result)
    { 
        temp_ptr = new_state_ptr;
        while(temp_ptr)
        {
            if(IsSameSMS(cur_state_ptr,temp_ptr))
            {
                result = TRUE;
                break;
            }
            else
            {
                temp_ptr = temp_ptr->next_long_sms_ptr;
            }
        }
    }    
    return result;
}


/*****************************************************************************/
// 	Description : Update Unread SMS Order list
//	Global resource dependence : s_mmisms_order
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMISMS_UpdateUnreadOrder(void)
{
    MMISMS_STATE_T *unread_state_ptr = PNULL;
    MMISMS_ORDER_ID_T order_id[MMISMS_SPLIT_MAX_NUM] = {PNULL};

    //SCI_TRACE_LOW:"MMISMS_UpdateUnreadOrder:is_need_update_order=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6104_112_2_18_2_54_5_653,(uint8*)"d", g_mmisms_global.is_need_update_order);

    if (g_mmisms_global.is_need_update_order)
    {
        MMISMS_SetNeedSort(TRUE);

        g_mmisms_global.is_need_update_order = FALSE;

        if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
        {
            unread_state_ptr = s_mmisms_order.security_mt_unread_head_ptr;
        }
        else
        {
            unread_state_ptr = s_mmisms_order.mt_unread_head_ptr;
        }

        while(unread_state_ptr)
        {
            uint32 i = 0;
            uint32 j= 0;
            BOOLEAN is_need_update = FALSE;

            SCI_MEMSET(order_id, 0x00, (sizeof(MMISMS_ORDER_ID_T)*MMISMS_SPLIT_MAX_NUM));

            is_need_update = FALSE;

            if (MMISMS_TYPE_SMS == unread_state_ptr->flag.msg_type)//SMS
            {
                if (unread_state_ptr->flag.is_concatenate_sms)//������Ϣ
                {
                    MMISMS_ORDER_ID_T cur_order_id = PNULL;

                    cur_order_id = unread_state_ptr;

                    while (cur_order_id != PNULL)
                    {
                        if (cur_order_id->flag.is_need_update)
                        {
                            order_id[i++] = cur_order_id;
                            cur_order_id  = cur_order_id->next_long_sms_ptr;
                            is_need_update = TRUE;
                        }
                        else//��ʾ�ڶ�ȡ�����յ�������Ϣ������һ����,����Ҫ�����б�
                        {
                            is_need_update = FALSE;
                            break;
                        }
                    }
                }
                else if (unread_state_ptr->flag.is_need_update)//��ͨ����Ϣ
                {
                    order_id[i++] = unread_state_ptr;
                    is_need_update = TRUE;
                }
                else
                {
                    is_need_update = FALSE;
                }
            }

            if (is_need_update)
            {
                for (j = 0; j < i; j++)
                {
                    MMISMS_STATE_T order_info = {0};

                    MMISMS_GetSmsInfoFromOrder (
                                                order_id[j], 
                                                &order_info 
                                                );

                    order_id[j]->flag.is_need_update = FALSE;
                    order_info.flag.is_need_update = FALSE;

                    // to modify the status in the order list and order the mt sms again.
                    order_info.flag.mo_mt_type = (MMISMS_MT_TO_BE_READ == order_info.flag.mo_mt_type) ? MMISMS_MT_HAVE_READ : MMISMS_MT_SR_HAVE_READ;

                    order_info.flag.is_receive_all = FALSE;

                    MMISMS_UpdateSmsInfoInOrder(order_id[j], &order_info);

#ifdef MMI_SMS_CHAT_SUPPORT
					SetChatOrderUnitNum();
#endif

                }
//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
                MMISMS_HandleOrderChangeCallBack();
#endif
                if(MMISMS_FOLDER_NORMAL == g_mmisms_global.folder_type)
                {
                    unread_state_ptr = s_mmisms_order.mt_unread_head_ptr;
                    continue;
                }
                else if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
                {
                    unread_state_ptr = s_mmisms_order.security_mt_unread_head_ptr;
                    continue;
                }                
            }

            unread_state_ptr = unread_state_ptr->next_ptr;
        }

    }
}

/*****************************************************************************/
//     Description : get sms order link delete available num
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL uint32 GetOrderLinkDeleteAvailNum(MMISMS_ORDER_ID_T order_id)
{
    uint32 delete_sms_num = 0;

    while(order_id != PNULL)//is  marked
    {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
        {
            if (order_id->flag.is_marked 
            && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id)))
            && (!mail_GetMailSendingFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))))
            {
                if (MMIAPIUDISK_UdiskIsRun())
                { 
                    //SCI_TRACE_LOW("GetOrderLinkDeleteAvailNum udisk is running!");
                }
                else
                {
                    delete_sms_num++;
                }
            }
        }
        else
#endif
        if (order_id->flag.is_marked && (!order_id->flag.is_locked) && (!MMIAPIMMS_IsProcessMMSById(order_id->record_id)))
        {
            if (MMIAPIUDISK_UdiskIsRun() && 
                (   order_id->flag.msg_type == MMISMS_TYPE_MMS
                || order_id->flag.msg_type == MMISMS_TYPE_WAP_PUSH
                || order_id->flag.msg_type == MMISMS_TYPE_MMS_OTA
                || order_id->flag.msg_type == MMISMS_TYPE_WAP_OTA))
            { 
                //SCI_TRACE_LOW("GetOrderLinkDeleteAvailNum udisk is running!");
            }
            else
            {
                delete_sms_num++;
            }
        }
        
        order_id = order_id->next_ptr;
    }

    return delete_sms_num;
}

/*****************************************************************************/
//     Description : get sms box delete available msg num
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetSmsBoxDeleteAvailNum(MMISMS_BOX_TYPE_E box_type)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    uint32 delete_sms_num = 0;
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_GetSmsBoxDeleteAvailNum box_type = %d", box_type);

    switch(box_type)
    {
    case MMISMS_BOX_MT:
        order_id = s_mmisms_order.mt_undownloaded_head_ptr;

        delete_sms_num = GetOrderLinkDeleteAvailNum(order_id);

        order_id = s_mmisms_order.mt_unread_head_ptr;
        
        delete_sms_num += GetOrderLinkDeleteAvailNum(order_id);

        order_id = s_mmisms_order.mt_readed_head_ptr;

        delete_sms_num += GetOrderLinkDeleteAvailNum(order_id);
        break;

    case MMISMS_BOX_SENDSUCC:
        order_id = s_mmisms_order.mo_sendsucc_head_ptr;

        delete_sms_num = GetOrderLinkDeleteAvailNum(order_id);
        break;
        
    case MMISMS_BOX_SENDFAIL:
        order_id = s_mmisms_order.mo_sendfail_head_ptr;

        delete_sms_num = GetOrderLinkDeleteAvailNum(order_id);
        break;
        
    case MMISMS_BOX_NOSEND:
        order_id = s_mmisms_order.mo_draft_head_ptr;

        delete_sms_num = GetOrderLinkDeleteAvailNum(order_id);
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;

        delete_sms_num = GetOrderLinkDeleteAvailNum(order_id);

        order_id = s_mmisms_order.security_mt_unread_head_ptr;
        
        delete_sms_num += GetOrderLinkDeleteAvailNum(order_id);
        
        order_id = s_mmisms_order.security_head_ptr;

        delete_sms_num += GetOrderLinkDeleteAvailNum(order_id);
        break; 
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
    case MMISMS_BOX_CHAT:
        delete_sms_num = GetChatOrderLinkDeleteAvailNum();
        break;
#endif
        
    default:
        SCI_TRACE_LOW("MMISMS_GetSmsBoxHeaderOrderId Error box_type=%d", box_type);
        break;
    }

    //SCI_TRACE_LOW:"MMISMS_GetSmsBoxHeaderOrderId delete_sms_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6328_112_2_18_2_54_6_654,(uint8*)"d", delete_sms_num);

    return delete_sms_num;
}

/*****************************************************************************/
//     Description : GetOrderLinkMMSNum
//    Global resource dependence : 
//  Author: minghu.mao
//    Note:
/*****************************************************************************/
LOCAL uint16 GetOrderLinkMMSNum(MMISMS_ORDER_ID_T order_id, wchar** num_wstr, uint16 *num_len_ptr, uint16 num)
{
    uint32 i                                    = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2]      = {0};
    wchar number_wstr[MMISMS_PBNUM_MAX_LEN + 1] = {0};
    uint32 num_len                              = 0;
    uint16 tel_num                              = num;
    wchar *all_number_wstr                      = PNULL;
    uint16 all_number_len                       = 0;
    wchar semicolon[]                           = {';', 0};
    uint16 temp_len                             = 0;

    all_number_wstr = *num_wstr;
    all_number_len = *num_len_ptr;

    while(order_id != PNULL && (tel_num < MMIMMS_MAX_RECEIVER_NUM))
    {
        if (order_id->flag.is_marked)//is  marked
        {
            SCI_MEMSET(number, 0x00, sizeof(number));
            
            if (order_id->flag.msg_type == MMISMS_TYPE_SMS)
            {
                num_len = MMIAPICOM_GenDispNumber(
                    MMIPB_GetNumberTypeFromUint8(order_id->number.bcd_number.npi_ton),
                    MIN(MN_MAX_ADDR_BCD_LEN, order_id->number.bcd_number.number_len),
                    order_id->number.bcd_number.number,
                    number,
                    sizeof(number)
                    );
            }
            else if(order_id->flag.msg_type == MMISMS_TYPE_MMS)
            {
                num_len = strlen((char *)order_id->number.sender);
                
                for (i = 0; i < num_len; i++)
                {
                    if (MMIAPICOM_IsTelephoneNum(*(order_id->number.sender+i)))
                    {
                        number[i] = *(order_id->number.sender + i);
                    }
                    else
                    {
                        num_len = i;
                        break;
                    }
                }
                number[num_len] = 0;
            }
            
            if (0 < num_len)
            {
                tel_num++;
                temp_len =  MMIMMS_MAX_ADDRESS_LEN * MMIMMS_MAX_RECEIVER_NUM;
                num_len = MIN(num_len, MMISMS_PBNUM_MAX_LEN);
                MMI_STRNTOWSTR(number_wstr, MMISMS_PBNUM_MAX_LEN, (uint8 *)number, num_len, num_len);
                MMI_WSTRNCPY(all_number_wstr + all_number_len, temp_len, 
                    number_wstr, MMIMMS_MAX_ADDRESS_LEN, num_len);                
                all_number_len = all_number_len + num_len;
                if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
                {
                    MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                    all_number_len++;
                }                
            }
        }
        
        order_id = order_id->next_ptr;
    }
    *num_len_ptr = all_number_len;
    return tel_num;
}

/*****************************************************************************/
//     Description : GetOrderLinkDestNum
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void GetOrderLinkDestNum(MMISMS_ORDER_ID_T order_id,MMISMS_DEST_ADDR_INFO_T *dest_info_ptr)
{
    uint32 i = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint32 num_len = 0;
    if(PNULL == dest_info_ptr)
    {
        SCI_TRACE_LOW("mmisms get dest number PNULL");
        return;
    }
    while(order_id != PNULL && (dest_info_ptr->dest_list.number < MMISMS_DEST_ADDR_MAX_NUM))
    {
        if (order_id->flag.is_marked)//is  marked
        {
            SCI_MEMSET(number, 0x00, sizeof(number));

            if (order_id->flag.msg_type == MMISMS_TYPE_SMS)
            {
                num_len = MMIAPICOM_GenDispNumber(
                    MMIPB_GetNumberTypeFromUint8(order_id->number.bcd_number.npi_ton),
                    MIN(MN_MAX_ADDR_BCD_LEN, order_id->number.bcd_number.number_len),
                    order_id->number.bcd_number.number,
                    number,
                    sizeof(number)
                    );
            }
            else if(order_id->flag.msg_type == MMISMS_TYPE_MMS)
            {
                num_len = strlen((char *)order_id->number.sender);

                for (i = 0; i < num_len; i++)
                {
                    if (MMIAPICOM_IsTelephoneNum(*(order_id->number.sender+i)))
                    {
                        number[i] = *(order_id->number.sender + i);
                    }
                    else
                    {
                        num_len = i;
                        break;
                    }
                }
                number[num_len] = 0;
            }

            if (0 < num_len)
            {
                MMISMS_SetAddressToMessage(number, num_len, &dest_info_ptr->dest_list);
            }
        }

        order_id = order_id->next_ptr;
    }
}

/*****************************************************************************/
//     Description : MMISMS_GetMtBoxMarkedDestNum
//    Global resource dependence : 
//  Author: liming.deng
//    Note:dest_info_ptr [out]
/*****************************************************************************/
PUBLIC void MMISMS_GetMtBoxMarkedDestNum(MMISMS_DEST_ADDR_INFO_T *dest_info_ptr)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    if(PNULL == dest_info_ptr)
    {
        SCI_TRACE_LOW("mmisms dest_info_ptr PNULL");
        return;
    }

    if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
    {
        order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;

        GetOrderLinkDestNum(order_id,dest_info_ptr);

        if (dest_info_ptr->dest_list.number < MMISMS_DEST_ADDR_MAX_NUM)
        {
            order_id = s_mmisms_order.security_mt_unread_head_ptr;

            GetOrderLinkDestNum(order_id,dest_info_ptr);
        }

        if (dest_info_ptr->dest_list.number < MMISMS_DEST_ADDR_MAX_NUM)
        {
            order_id = s_mmisms_order.security_head_ptr;

            GetOrderLinkDestNum(order_id,dest_info_ptr);
        }
    }
    else
    {
        order_id = s_mmisms_order.mt_undownloaded_head_ptr;

        GetOrderLinkDestNum(order_id,dest_info_ptr);

        if (dest_info_ptr->dest_list.number < MMISMS_DEST_ADDR_MAX_NUM)
        {
            order_id = s_mmisms_order.mt_unread_head_ptr;

            GetOrderLinkDestNum(order_id,dest_info_ptr);
        }

        if (dest_info_ptr->dest_list.number < MMISMS_DEST_ADDR_MAX_NUM)
        {
            order_id = s_mmisms_order.mt_readed_head_ptr;

            GetOrderLinkDestNum(order_id,dest_info_ptr);
        }
    }
}

/*****************************************************************************/
//     Description : MMISMS_SetMtBoxMarkedMMSNum
//    Global resource dependence : 
//  Author: minghu.mao
//    Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMtBoxMarkedDestNum(wchar** num_wstr, uint16 *num_len)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    uint16 tel_num             = 0;

    if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
    {
        order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;

        tel_num = GetOrderLinkMMSNum(order_id, num_wstr, num_len, 0);

        if (tel_num < MMIMMS_MAX_RECEIVER_NUM)
        {
            order_id = s_mmisms_order.security_mt_unread_head_ptr;
            tel_num = GetOrderLinkMMSNum(order_id, num_wstr, num_len, tel_num);
        }
        if (tel_num < MMIMMS_MAX_RECEIVER_NUM)
        {
            order_id = s_mmisms_order.security_head_ptr;
            tel_num = GetOrderLinkMMSNum(order_id, num_wstr, num_len, tel_num);
        }
    }

    else
    {
        order_id = s_mmisms_order.mt_undownloaded_head_ptr;

        tel_num = GetOrderLinkMMSNum(order_id, num_wstr, num_len, 0);

        if (tel_num < MMIMMS_MAX_RECEIVER_NUM)
        {
            order_id = s_mmisms_order.mt_unread_head_ptr;
            tel_num = GetOrderLinkMMSNum(order_id, num_wstr, num_len, tel_num);
        }
        if (tel_num < MMIMMS_MAX_RECEIVER_NUM)
        {
            order_id = s_mmisms_order.mt_readed_head_ptr;
            tel_num = GetOrderLinkMMSNum(order_id, num_wstr, num_len, tel_num);
        }
    }
}

/*****************************************************************************/
//     Description : GetOrderLinkOriginCard
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetOrderLinkOriginCard(MMISMS_ORDER_ID_T order_id, MN_DUAL_SYS_E *dual_sys)
{
    BOOLEAN is_same_card = TRUE;

    if (PNULL == dual_sys)
    {
        //SCI_TRACE_LOW:"MMISMS:GetOrderLinkOriginCard dual_sys = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6571_112_2_18_2_54_6_655,(uint8*)"");
        return FALSE;
    }
    while (PNULL != order_id)
    {
        if (order_id->flag.is_marked)//is  marked
        {
            if (*dual_sys != order_id->flag.dual_sys && MN_DUAL_SYS_MAX != *dual_sys)
            {
                is_same_card = FALSE;
                break;
            }
            *dual_sys = (MN_DUAL_SYS_E)order_id->flag.dual_sys;
        }

        order_id = order_id->next_ptr;
    }

    return is_same_card;
}

/*****************************************************************************/
//     Description : MMISMS_GetMtBoxMarkedOriginCard
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetMtBoxMarkedOriginCard(void)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    BOOLEAN is_same_card = TRUE;
    MMISMS_ORDER_ID_T order_id = PNULL;

    order_id = s_mmisms_order.mt_undownloaded_head_ptr;

    is_same_card = GetOrderLinkOriginCard(order_id, &dual_sys);

    if (is_same_card)
    {
        order_id = s_mmisms_order.mt_unread_head_ptr;

        is_same_card = GetOrderLinkOriginCard(order_id, &dual_sys);
    }
    else
    {
        return MN_DUAL_SYS_MAX;
    }
    

    if (is_same_card)
    {
        order_id = s_mmisms_order.mt_readed_head_ptr;

        is_same_card = GetOrderLinkOriginCard(order_id, &dual_sys);
    }
    else
    {
        return MN_DUAL_SYS_MAX;
    }

    if (is_same_card)
    {
        return dual_sys;
    }

    return MN_DUAL_SYS_MAX;
}

/*****************************************************************************/
//     Description : MMISMS_GetSMSUsedAndMax
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetSMSUsedAndMax(
                                    MN_DUAL_SYS_E dual_sys,
                                    BOOLEAN    is_nv,            //IN:
                                    uint16        *max_num_ptr,    //OUT:
                                    uint16        *used_num_ptr,    //OUT:
                                    uint16        *sms_ori_used_num_ptr    //OUT:
                                    )
{
    if (PNULL == max_num_ptr || PNULL == used_num_ptr || PNULL == sms_ori_used_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetSMSUsedAndMax max_num_ptr used_num_ptr sms_ori_used_num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6651_112_2_18_2_54_7_656,(uint8*)"");
        return;
    }

    MMISMS_GetSMSMemory(dual_sys, is_nv, max_num_ptr, used_num_ptr);
    *used_num_ptr -= MMISMS_GetReadErrSmsNum(dual_sys, is_nv);

    *sms_ori_used_num_ptr = *used_num_ptr;

    if (!is_nv)
    {
        if(dual_sys >= MN_DUAL_SYS_1 && dual_sys < MMI_DUAL_SYS_MAX)  //possibility: dual_sys == MMI_DUAL_SYS_MAX ? or MN_DUAL_SYS_MAX > MMI_DUAL_SYS_MAX?
        {
            *sms_ori_used_num_ptr = s_mmisms_order.total_mo_num.ori_sim_sms_num[dual_sys] + s_mmisms_order.total_mt_num.ori_sim_sms_num[dual_sys];
        }
    }

    //SCI_TRACE_LOW:"SMS: MMISMS_GetSMSUsedAndMax dual_sys=%d, is_nv = %d, max_num = %d, used_num = %d,ori=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6669_112_2_18_2_54_7_657,(uint8*)"ddddd", dual_sys, is_nv, (*max_num_ptr), (*used_num_ptr),(*sms_ori_used_num_ptr));
}

/*****************************************************************************/
//     Description : mark point message
//    Global resource dependence : nsms1_state_ptr
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetPointedMsgNumMarked(
                                    MMISMS_BOX_TYPE_E box_type,
                                    uint8 *number,
                                    uint8 len,
                                    uint16 index
                                    )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    BOOLEAN is_do_mark = FALSE;
    uint8 mms_num[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    int32 i = 0;
    uint8 cmp_len = 0; 

    //SCI_TRACE_LOW:"MMISMS_SetPointedMsgNumMarked:index is %d,len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6690_112_2_18_2_54_7_658,(uint8*)"dd",index,len);

    //����pos_index��õ�ǰ��order_id 
    order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type, index); 
    
    if(order_id == PNULL)
    {
        return len;
    }      

    cur_order_id = order_id;

    for(i=0;i<15;i++)
    {
        //SCI_TRACE_LOW:"fengming:sender[%d] is %d,number[%d] is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6704_112_2_18_2_54_7_659,(uint8*)"dddd",i,cur_order_id->number.sender[i],i,number[i]);
    }  

    while (cur_order_id != PNULL)
    {
        //SCI_TRACE_LOW:"MMISMS_SetPointedMsgNumMarked:msg_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6709_112_2_18_2_54_7_660,(uint8*)"d",cur_order_id->flag.msg_type);

        if(cur_order_id->flag.msg_type == MMISMS_TYPE_SMS)
        {
            SCI_MEMSET(mms_num,0x0,((MMISMS_PBNUM_MAX_LEN + 2)*sizeof(uint8)));

            if(cur_order_id->number.sender[2] == 0x68)
            {
                if ((len == (cur_order_id->number.sender[0] - 1))
                    && 0 == (MMISMS_Bcdnumcmp((char *)number,(char *)&cur_order_id->number.sender[3],len))
                    && !cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = TRUE;
                    is_do_mark = TRUE;
                }
            }
            else
            {
                if ((len == cur_order_id->number.sender[0])
                    && 0 == (MMISMS_Bcdnumcmp((char *)number,(char *)&cur_order_id->number.sender[2],len))
                    && !cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = TRUE;
                    is_do_mark = TRUE;
                }
            }
        }
        else
        {
            if(cur_order_id->number.sender[0] == 0x68)
            {
                MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                  (char *)&cur_order_id->number.sender[1],
                                  mms_num);
                if ((len == (strlen((char *)cur_order_id->number.sender) + 1)/2)
                    && !(MMISMS_Bcdnumcmp((char *)number,(char *)mms_num,len))
                    && !cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = TRUE;
                    is_do_mark = TRUE;
                }
            }
            else
            {
                if((0x2B == cur_order_id->number.sender[0]) &&
                   (0x38 == cur_order_id->number.sender[1]) &&
                   (0x36 == cur_order_id->number.sender[2]))
                {
                    MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                      (char *)&cur_order_id->number.sender[3],
                                      mms_num);
                    cmp_len = (strlen((char *)&cur_order_id->number.sender[3]) + 1)/2;
                }
                else
                {
                    MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                      (char *)&cur_order_id->number.sender[0],
                                      mms_num);
                    cmp_len = (strlen((char *)cur_order_id->number.sender) + 1)/2;
                }

                if ((len == cmp_len)
                    && !(MMISMS_Bcdnumcmp((char *)number,(char *)mms_num,len))
                    && !cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = TRUE;
                    is_do_mark = TRUE;
                }
            }
        }
        cur_order_id = cur_order_id->next_long_sms_ptr;

    }

    return is_do_mark;
}

/*****************************************************************************/
//     Description : mark point message
//    Global resource dependence : nsms1_state_ptr
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetPointedMsgNumMarkedCancel(
                                    MMISMS_BOX_TYPE_E box_type,
                                    uint8 *number,
                                    uint8 len,
                                    uint16 index
                                    )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    BOOLEAN is_do_mark = FALSE;
    uint8 mms_num[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 cmp_len = 0;

    //SCI_TRACE_LOW:"MMISMS_SetPointedMsgNumMarked:index is %d,len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6803_112_2_18_2_54_7_661,(uint8*)"dd",index,len);
    
    //����pos_index��õ�ǰ��order_id 
    order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type, index); 

    if(order_id == PNULL)
    {
        return len;
    }    

    cur_order_id = order_id;

    while (cur_order_id != PNULL)
    {
        //SCI_TRACE_LOW:"MMISMS_SetPointedMsgNumMarked:msg_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6817_112_2_18_2_54_7_662,(uint8*)"d",cur_order_id->flag.msg_type);
        
        SCI_MEMSET(mms_num,0x0,((MMISMS_PBNUM_MAX_LEN + 2)*sizeof(uint8)));
    
        if(cur_order_id->flag.msg_type == MMISMS_TYPE_SMS)
        {
            if(cur_order_id->number.sender[2] == 0x68)
            {
                if ((len == (cur_order_id->number.sender[0] - 1))
                    && !(MMISMS_Bcdnumcmp((char *)number,(char *)&cur_order_id->number.sender[3],len))
                    && cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = FALSE;
                    is_do_mark = TRUE;
                }
            }
            else
            {
                if ((len == cur_order_id->number.sender[0])
                    && !(MMISMS_Bcdnumcmp((char *)number,(char *)&cur_order_id->number.sender[2],len))
                    && cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = FALSE;
                    is_do_mark = TRUE;
                }
            }
        }
        else
        {
            if(cur_order_id->number.sender[0] == 0x68)
            {
                MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                  (char *)&cur_order_id->number.sender[1],
                                  mms_num);
                if ((len == (strlen((char *)cur_order_id->number.sender) + 1)/2)
                    && !(MMISMS_Bcdnumcmp((char *)number,(char *)mms_num,len))
                    && cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = FALSE;
                    is_do_mark = TRUE;
                }
            }
            else
            {
                if((0x2B == cur_order_id->number.sender[0]) &&
                   (0x38 == cur_order_id->number.sender[1]) &&
                   (0x36 == cur_order_id->number.sender[2]))
                {
                    MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                      (char *)&cur_order_id->number.sender[3],
                                      mms_num);
                    cmp_len = (strlen((char *)&cur_order_id->number.sender[3]) + 1)/2;
                }
                else
                {
                    MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                      (char *)&cur_order_id->number.sender[0],
                                      mms_num);
                    cmp_len = (strlen((char *)&cur_order_id->number.sender[0]) + 1)/2;
                }

                if ((len == cmp_len)
                    && !(MMISMS_Bcdnumcmp((char *)number,(char *)mms_num,len))
                    && cur_order_id->flag.is_marked)
                {
                    cur_order_id->flag.is_marked = FALSE;
                    is_do_mark = TRUE;
                }
            }
        }
        cur_order_id = cur_order_id->next_long_sms_ptr;
    }

    return is_do_mark;
}

/*****************************************************************************/
//     Description : get point message number
//    Global resource dependence : nsms1_state_ptr
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetOriginNumFromOrder(
                                                MMISMS_BOX_TYPE_E box_type,
                                                uint8 *number,
                                                uint16 index)
{
    uint8 len = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    int32 i = 0;

    //����pos_index��õ�ǰ��order_id 
    order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type, index); 
    if(order_id == PNULL)
    {
        return len;
    }

    cur_order_id = order_id;

    for(i=0;i<15;i++)
    {
        //SCI_TRACE_LOW:"fengming:sender[%d] is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6918_112_2_18_2_54_7_663,(uint8*)"dd",i,cur_order_id->number.sender[i]);
    }

    if(cur_order_id->flag.msg_type == MMISMS_TYPE_SMS)
    {
        if(cur_order_id->number.sender[2] == 0x68)
        {
            len = cur_order_id->number.sender[0] - 1;
            SCI_MEMCPY( number,
                        &cur_order_id->number.sender[3],
                        len);
        }
        else
        {
            len = cur_order_id->number.sender[0];
            SCI_MEMCPY( number,
                        &cur_order_id->number.sender[2],
                        len);
        }
    }

    else
    {
        if(cur_order_id->number.sender[0] == 0x68)
        {
            MMIAPICOM_StrToBcd( PACKED_LSB_FIRST,
                                (char *)&cur_order_id->number.sender[1],
                                number);
            len = (strlen((char *)cur_order_id->number.sender) + 1)/2;
        }
        else
        {
            if((0x2B == cur_order_id->number.sender[0]) &&
               (0x38 == cur_order_id->number.sender[1]) &&
               (0x36 == cur_order_id->number.sender[2]))
            {
                MMIAPICOM_StrToBcd( PACKED_LSB_FIRST,
                                    (char *)&cur_order_id->number.sender[3],
                                    number);
                len = (strlen((char *)&cur_order_id->number.sender[3]) + 1)/2;
            }
            else
            {
                MMIAPICOM_StrToBcd( PACKED_LSB_FIRST,
                                    (char *)&cur_order_id->number.sender[0],
                                    number);
                len = (strlen((char *)cur_order_id->number.sender) + 1)/2;
            }
        }
    }

    for(i=0;i<15;i++)
    {
        //SCI_TRACE_LOW:"fengming:number[%d] is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6971_112_2_18_2_54_7_664,(uint8*)"dd",i,number[i]);
    } 

    //SCI_TRACE_LOW:"MMISMS:GetNumber len is %d,msg_type is %d,mo_mt_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6974_112_2_18_2_54_7_665,(uint8*)"ddd",len,cur_order_id->flag.msg_type,cur_order_id->flag.mo_mt_type);

    return len;
}

/*****************************************************************************/
//     Description : start from the current index, and find the valid mo sms which
//                   is stored in SIM
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMOSIMSMS(
                                MMISMS_BOX_TYPE_E box_type,
                                MN_DUAL_SYS_E dual_sys)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    switch(box_type)
    {
    case MMISMS_BOX_SENDSUCC:
        order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        order_id = s_mmisms_order.mo_draft_head_ptr;
        break;

    /*case MMISMS_BOX_SECURITY:
        order_id = s_mmisms_order.security_head_ptr;
        break;*/
        
    default:
        break;
    }

    while (PNULL != order_id)
    {
        if ((MN_SMS_STORAGE_SIM == order_id->flag.storage) &&
            (dual_sys == order_id->flag.dual_sys))
        {
            break;
        }
        else
        {
            order_id = order_id->next_ptr;
        }            
    }    

    
    return order_id ;
}

/*****************************************************************************/
//     Description : start from the current index, and find the valid mt sms which
//                   is stored in SIM
//    Global  resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMTSIMSMS(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    order_id = s_mmisms_order.mt_undownloaded_head_ptr;
    
    while(order_id != PNULL)//is  marked
    {
        if ((MN_SMS_STORAGE_SIM == order_id->flag.storage) &&
            (dual_sys == order_id->flag.dual_sys))
        {
            break;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }
    if(order_id == PNULL) 
    {
        order_id = s_mmisms_order.mt_unread_head_ptr;
        
        while(order_id != PNULL)//is  marked
        {
            if ((MN_SMS_STORAGE_SIM == order_id->flag.storage) &&
            (dual_sys == order_id->flag.dual_sys))
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }
        if (order_id == PNULL)
        {
            order_id = s_mmisms_order.mt_readed_head_ptr;
            while(order_id != PNULL)//is  marked
            {
                if ((MN_SMS_STORAGE_SIM == order_id->flag.storage) &&
                    (dual_sys == order_id->flag.dual_sys))
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
        }        
    }
    return order_id;    
}

/*****************************************************************************/
//     Description : get the message is security or not
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_FOLDER_TYPE_E MMISMS_CurMsgFolderType(void )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;
    MMISMS_SetCurOrderArrIndex(0);
    order_id = MMISMS_GetCurOperationOrderId();
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_CurMsgFolderType order_id = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_7100_112_2_18_2_54_7_666,(uint8*)"");
        return folder_type;
    }  
    folder_type = order_id->flag.folder_type;

    return folder_type; 
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecurityMoSMSForDelAll(MMISMS_BOX_TYPE_E box_type )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    switch(box_type)
    {
    case MMISMS_BOX_SENDSUCC:
        order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        while (PNULL != order_id)
        {
            if(order_id->flag.is_marked) //is  marked
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        break;
        
    case MMISMS_BOX_SENDFAIL:
        order_id = s_mmisms_order.mo_sendfail_head_ptr;
        while (PNULL != order_id)
        {
            if(order_id->flag.is_marked) //is  marked
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        break;
        
    case MMISMS_BOX_NOSEND:
        order_id = s_mmisms_order.mo_draft_head_ptr;
        while (PNULL != order_id)
        {
            if(order_id->flag.is_marked) //is  marked
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        order_id = s_mmisms_order.security_head_ptr;
        while (PNULL != order_id)
        {
            if(order_id->flag.is_marked) //is  marked
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        break;        
#endif       
    default:
        break;
    }
    return order_id ;
    
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related MT SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecurityMTSMSForDelAll(void)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    order_id = s_mmisms_order.mt_undownloaded_head_ptr;
    
    while(order_id != PNULL)//is  marked
    {
        if (order_id->flag.is_marked)
        {
            break;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }
    if(order_id == PNULL) 
    {
        order_id = s_mmisms_order.mt_unread_head_ptr;
        
        while(order_id != PNULL)//is  marked
        {
            if (order_id->flag.is_marked)
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }
        if (order_id == PNULL)
        {
            order_id = s_mmisms_order.mt_readed_head_ptr;
            while(order_id != PNULL)//is  marked
            {
                if (order_id->flag.is_marked)
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
        }        
    }
    return order_id;    
}

/*****************************************************************************/
//     Description : get sms box move available msg num
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetSmsBoxMoveAvailNum(MMISMS_BOX_TYPE_E box_type)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    uint32 move_sms_num = 0;
    
    SCI_TRACE_LOW("[MMISMS] MMISMS_GetSmsBoxMoveAvailNum box_type = %d", box_type);

    switch(box_type)
    {
    case MMISMS_BOX_MT:
        order_id = s_mmisms_order.mt_undownloaded_head_ptr;

        move_sms_num = GetOrderLinkMoveAvailNum(order_id);

        order_id = s_mmisms_order.mt_unread_head_ptr;
        
        move_sms_num += GetOrderLinkMoveAvailNum(order_id);

        order_id = s_mmisms_order.mt_readed_head_ptr;

        move_sms_num += GetOrderLinkMoveAvailNum(order_id);
        break;

    case MMISMS_BOX_SENDSUCC:
        order_id = s_mmisms_order.mo_sendsucc_head_ptr;

        move_sms_num = GetOrderLinkMoveAvailNum(order_id);
        break;
        
    case MMISMS_BOX_SENDFAIL:
        order_id = s_mmisms_order.mo_sendfail_head_ptr;

        move_sms_num = GetOrderLinkMoveAvailNum(order_id);
        break;
        
    case MMISMS_BOX_NOSEND:
        order_id = s_mmisms_order.mo_draft_head_ptr;

        move_sms_num = GetOrderLinkMoveAvailNum(order_id);
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;

        move_sms_num = GetOrderLinkMoveAvailNum(order_id);

        order_id = s_mmisms_order.security_mt_unread_head_ptr;
        
        move_sms_num += GetOrderLinkMoveAvailNum(order_id);
        
        order_id = s_mmisms_order.security_head_ptr;

        move_sms_num += GetOrderLinkMoveAvailNum(order_id);
        break;        
#endif        
    default:
        SCI_TRACE_LOW("MMISMS_GetSmsBoxHeaderOrderId Error box_type=%d", box_type);
        break;
    }

    //SCI_TRACE_LOW:"MMISMS_GetSmsBoxHeaderOrderId move_sms_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_7304_112_2_18_2_54_8_667,(uint8*)"d", move_sms_num);

    return move_sms_num;
}

/*****************************************************************************/
//     Description : get sms order link move available num
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
LOCAL uint32 GetOrderLinkMoveAvailNum(MMISMS_ORDER_ID_T order_id)
{
    uint32 delete_sms_num = 0;

    while(order_id != PNULL)//is  marked
    {
        if (order_id->flag.is_marked && (!MMIAPIMMS_IsProcessMMSById(order_id->record_id)))
        {
            if (MMIAPIUDISK_UdiskIsRun() && 
                (   order_id->flag.msg_type == MMISMS_TYPE_MMS
                || order_id->flag.msg_type == MMISMS_TYPE_WAP_PUSH
                || order_id->flag.msg_type == MMISMS_TYPE_MMS_OTA
                || order_id->flag.msg_type == MMISMS_TYPE_WAP_OTA))
            { 
                //SCI_TRACE_LOW("GetOrderLinkDeleteAvailNum udisk is running!");
            }
            else
            {
                delete_sms_num++;
            }
        }
        
        order_id = order_id->next_ptr;
    }

    return delete_sms_num;
}

/*****************************************************************************/
//     Description : set msg folder type
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurMsgFolderType(
                                MMISMS_ORDER_ID_T cur_order_id,
                                MMISMS_OPER_STATUS_E oper_status)
{
    MMISMS_STATE_T  *new_order_info_ptr = PNULL;

    new_order_info_ptr = SCI_ALLOCAZ(sizeof(MMISMS_STATE_T));
    if(PNULL == new_order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS_SetCurMsgFolderType alloc failed "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_7354_112_2_18_2_54_8_668,(uint8*)"");
        return;
    }
    //SCI_MEMSET(new_order_info_ptr,0x0,sizeof(MMISMS_STATE_T));
    SCI_MEMCPY(
        new_order_info_ptr,
        cur_order_id,
        sizeof(MMISMS_STATE_T));

    if(new_order_info_ptr->flag.is_need_update && 
    (MMISMS_MT_TO_BE_READ == new_order_info_ptr->flag.mo_mt_type))
    {
        new_order_info_ptr->flag.mo_mt_type = MMISMS_MT_HAVE_READ;
        new_order_info_ptr->flag.is_need_update = FALSE;
    }

    //SCI_TRACE_LOW:"MMISMS_SetCurMsgFolderType new_order_info_ptr = 0x%x,record_id is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_7371_112_2_18_2_54_8_669,(uint8*)"dd",new_order_info_ptr,cur_order_id->record_id);
    
    switch(oper_status)
    {
        case MMISMS_MOVE_SMS_TO_SECURITY:
#if 0  //user trigger event           
            if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) && (cur_order_id->next_long_sms_ptr == PNULL)) //PNULL ������ǰ�����һ�� 
            {
                 MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, 
                    MMISMS_MSG_UPDATELIST, 
                    PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����list���� 
            }
#endif            
            MMISMS_DelSmsInfoInOrder(cur_order_id);
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);              
            new_order_info_ptr->flag.folder_type = MMISMS_FOLDER_SECURITY;
            new_order_info_ptr->flag.is_search = FALSE;
            new_order_info_ptr->flag.is_receive_all = FALSE;
            MMISMS_InsertSmsToOrder(new_order_info_ptr);
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_SECURITY_ADD); 
            break;

        case MMISMS_MOVE_SMS_FROM_SECURITY:
            MMISMS_DelSmsInfoInOrder(cur_order_id);
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE); 
            new_order_info_ptr->flag.folder_type = MMISMS_FOLDER_NORMAL;
            new_order_info_ptr->flag.is_receive_all = FALSE;
            MMISMS_InsertSmsToOrder(new_order_info_ptr);
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_ADD); 
            break;

        default:
            break;
    }

    g_mmisms_global.nv_param.folder_type[new_order_info_ptr->record_id] = new_order_info_ptr->flag.folder_type;
    //SCI_TRACE_LOW:"MMISMS_SetCurMsgFolderType folder_type = %d,record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_7404_112_2_18_2_54_8_670,(uint8*)"dd",new_order_info_ptr->flag.folder_type,new_order_info_ptr->record_id);
    SCI_FREE(new_order_info_ptr);
    return;
}


/*****************************************************************************/
//     Description : get the used_unit_head_ptr
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_LINKED_ORDER_UNIT_T *MMISMS_GetUsedOrderId(void)
{
    return s_mmisms_linked_order.used_unit_head_ptr;
}

/*****************************************************************************/
//     Description : set search sms flag
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSearchFlag(
						 MMISMS_STATE_T *order_info
						 )
{
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id = PNULL;
    MMISMS_STATE_T *cur_state_ptr = PNULL;

    cur_unit_id = MMISMS_GetUsedOrderId();
    cur_state_ptr = &cur_unit_id->sms_state;

    order_info->flag.is_search = TRUE;

    if(0 != order_info->longsms_max_num)
    {
        while (PNULL != cur_unit_id)
        {
            if(MMISMS_CompareSearchSmsState(cur_state_ptr, order_info))
            {
                cur_state_ptr->flag.is_search = TRUE;
                cur_unit_id = cur_unit_id->next_ptr;
                cur_state_ptr = &cur_unit_id->sms_state;
            }
            else
            {
                cur_unit_id = cur_unit_id->next_ptr;
                cur_state_ptr = &cur_unit_id->sms_state;
            }    
        }
    }
    return;
}

/*****************************************************************************/
//     Description : �Ƚ���������״̬��Ϣ���ж��Ƿ���ͬһ�������ŵĲ�ͬ���֡�
//    Global  resource dependence : 
//  Author: fengming.huang
//    Note:   ref_num,max_num,�����ȫ��ͬ�򷵻�TRUE. 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CompareSearchSmsState(                                        //RETURN: 
                              MMISMS_STATE_T    *sms1_state_ptr,    //IN: SMS1 state 
                              MMISMS_STATE_T    *sms2_state_ptr     //IN: SMS2 state
                              )
{
    BOOLEAN result = FALSE;
    
    if (PNULL == sms1_state_ptr || PNULL == sms2_state_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_CompareSearchSmsState sms1_state_ptr sms2_state_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_7468_112_2_18_2_54_8_671,(uint8*)"");
        return FALSE;
    }
    
    if (  (sms1_state_ptr->flag.dual_sys == sms2_state_ptr->flag.dual_sys)
        && (1 == sms1_state_ptr->flag.is_concatenate_sms)                // concatenated
        && (sms1_state_ptr->flag.msg_type == MMISMS_TYPE_SMS))    // is SMS,not mms
        
    {
        // the same head reference number and the same head max number
        if (IsSameLongSmsPart(sms1_state_ptr, sms2_state_ptr))            
        {
            result = TRUE;
        }
    }    
    return (result);
}

/*****************************************************************************/
//  Description : mark all export message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_SetExportBoxMsgMarked(
                                   MMISMS_BOX_TYPE_E box_type,
                                   BOOLEAN           is_marked //IN:
                                   )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    uint32 mark_num = 0;
    
    switch(box_type) 
    {
    case MMISMS_BOX_MT:
        cur_order_id =s_mmisms_order.mt_unread_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        cur_order_id =s_mmisms_order.mt_readed_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        break;
        
    case MMISMS_BOX_SENDSUCC:
        cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }                
        }
        break;
        
    case MMISMS_BOX_SENDFAIL:
        cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }             
        }
        break;
        
    case MMISMS_BOX_NOSEND:
        cur_order_id =s_mmisms_order.mo_draft_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }              
        }
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        cur_order_id =s_mmisms_order.security_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }               
        }
        break;
#endif        
#ifdef CHATMODE_ONLY_SUPPORT
    case MMISMS_BOX_CHAT:   //chat include MT,send success, send fail,
        {
            MMISMS_CHAT_STATE_UNIT_T *num_ptr = PNULL;

            num_ptr = s_mmisms_chat_linked_order.head_ptr;

            while(PNULL != num_ptr)
            {
                cur_order_id = num_ptr->head_ptr;
                while (cur_order_id != PNULL)
                {
                    if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
                    {
                        mark_num++;
                        cur_long_order_id = cur_order_id;
                        while (cur_long_order_id != PNULL)
                        {
                            cur_long_order_id->flag.is_marked = is_marked;
                            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                        }
                        cur_order_id = cur_order_id->next_chat_ptr;
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_chat_ptr;
                    }
                }
                num_ptr = num_ptr->next_number_ptr;
            }
        }
        break;
#endif

    default:
        break;
    }

    return mark_num;
}

/*****************************************************************************/
//  Description : mark all export message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_SetExportAllBoxMsgMarked( 
                                    BOOLEAN is_security,
                                    BOOLEAN is_marked
                                    )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    uint32 mark_num = 0;

    //MT Box
    cur_order_id =s_mmisms_order.mt_unread_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            mark_num++;
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        else
        {
            cur_order_id = cur_order_id->next_ptr;
        }
    }
    
    cur_order_id =s_mmisms_order.mt_readed_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            mark_num++;
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        else
        {
            cur_order_id = cur_order_id->next_ptr;
        }        
    }

    //SENT Box
    cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            mark_num++;
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        else
        {
            cur_order_id = cur_order_id->next_ptr;
        }          
    }

    //OUT Box
    cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            mark_num++;
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        else
        {
            cur_order_id = cur_order_id->next_ptr;
        }         
    }

    //DRAFT Box
    cur_order_id =s_mmisms_order.mo_draft_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            mark_num++;
            cur_long_order_id = cur_order_id;
            while (cur_long_order_id != PNULL)
            {
                cur_long_order_id->flag.is_marked = is_marked;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }
            cur_order_id = cur_order_id->next_ptr;
        }
        else
        {
            cur_order_id = cur_order_id->next_ptr;
        }         
    }

    //SECURITY Box
    if(is_security)
    {
        cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }
        }

        cur_order_id =s_mmisms_order.security_head_ptr;
        while (cur_order_id != PNULL)
        {
            if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                mark_num++;
                cur_long_order_id = cur_order_id;
                while (cur_long_order_id != PNULL)
                {
                    cur_long_order_id->flag.is_marked = is_marked;
                    cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
                }
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }             
        }
    }

    return mark_num;
}

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //delete all mail from message list
/*****************************************************************************/
//     Description : get the order id by storage and record_id .
//    Global resource dependence : 
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteAllMail(MMISMS_MSG_TYPE_E msg_type)
{
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id = PNULL;
    MMISMS_LINKED_ORDER_UNIT_T *next_unit_id = PNULL;     
    MMISMS_ORDER_ID_T      order_id = PNULL;
    
    cur_unit_id = s_mmisms_linked_order.used_unit_head_ptr;    
    
    while ((cur_unit_id != PNULL))
    {
        next_unit_id = cur_unit_id->next_ptr;
        if(msg_type == cur_unit_id->sms_state.flag.msg_type)
        {
            order_id = &(cur_unit_id->sms_state);
            MMISMS_DelSmsInfoInOrder(order_id);
        }
        cur_unit_id = next_unit_id;
        
        if(cur_unit_id == NULL)
        {
            break;
        }
    }
    return TRUE;
 
}
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */


/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                exporting the related SMS box
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSMSForExportAll(
                                        MMISMS_BOX_TYPE_E box_type,
                                        BOOLEAN is_need_security)
{
    MMISMS_ORDER_ID_T order_id = PNULL;

    //unread sms    
    order_id = s_mmisms_order.mt_unread_head_ptr;
    
    while(order_id != PNULL)//is  marked
    {
        if (order_id->flag.is_marked)
        {
            return order_id;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }

    //readed sms
    order_id = s_mmisms_order.mt_readed_head_ptr;
    while(order_id != PNULL)
    {
        if (order_id->flag.is_marked)//is  marked
        {
            return order_id;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }    

    //sendsucc sms
    order_id = s_mmisms_order.mo_sendsucc_head_ptr;
    while (PNULL != order_id)
    {
        if(order_id->flag.is_marked) //is  marked
        {
            return order_id;
        }
        else
        {
            order_id = order_id->next_ptr;
        }            
    }  

    //sendfail sms
    order_id = s_mmisms_order.mo_sendfail_head_ptr;
    while (PNULL != order_id)
    {
        if(order_id->flag.is_marked) //is  marked
        {
            return order_id;
        }
        else
        {
            order_id = order_id->next_ptr;
        }            
    }    

    //nosend sms
    order_id = s_mmisms_order.mo_draft_head_ptr;
    while (PNULL != order_id)
    {
        if(order_id->flag.is_marked) //is  marked
        {
            return order_id;
        }
        else
        {
            order_id = order_id->next_ptr;
        }            
    }   

    if(is_need_security)
    {
        order_id = s_mmisms_order.security_mt_unread_head_ptr;
        
        while(order_id != PNULL)//is  marked
        {
            if (order_id->flag.is_marked)
            {
                return order_id;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }
    
        order_id = s_mmisms_order.security_head_ptr;
        while (PNULL != order_id)
        {
            if(order_id->flag.is_marked) //is  marked
            {
                return order_id;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
    }

    return order_id;
}

/*****************************************************************************/
//  Description : get export sms num
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportMarkedMsgNum(
                                   MMISMS_BOX_TYPE_E box_type
                                   )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    uint32 mark_num = 0;
    
    switch(box_type) 
    {
    case MMISMS_BOX_MT:
        cur_order_id =s_mmisms_order.mt_unread_head_ptr;
        while (cur_order_id != PNULL)
        {
            if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
               (cur_order_id->flag.is_marked)) 
            {
                mark_num++;
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        cur_order_id =s_mmisms_order.mt_readed_head_ptr;
        while (cur_order_id != PNULL)
        {
            if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
               (cur_order_id->flag.is_marked)) 
            {
                mark_num++;
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        break;
        
    case MMISMS_BOX_SENDSUCC:
        cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
        while (cur_order_id != PNULL)
        {
            if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
               (cur_order_id->flag.is_marked)) 
            {
                mark_num++;
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }                
        }
        break;
        
    case MMISMS_BOX_SENDFAIL:
        cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
        while (cur_order_id != PNULL)
        {
            if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
               (cur_order_id->flag.is_marked)) 
            {
                mark_num++;
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }             
        }
        break;
        
    case MMISMS_BOX_NOSEND:
        cur_order_id =s_mmisms_order.mo_draft_head_ptr;
        while (cur_order_id != PNULL)
        {
            if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
               (cur_order_id->flag.is_marked)) 
            {
                mark_num++;
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }              
        }
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
        while (cur_order_id != PNULL)
        {
            if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
               (cur_order_id->flag.is_marked)) 
            {
                mark_num++;
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }
        }
        cur_order_id =s_mmisms_order.security_head_ptr;
        while (cur_order_id != PNULL)
        {
            if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
               (cur_order_id->flag.is_marked)) 
            {
                mark_num++;
                cur_order_id = cur_order_id->next_ptr;
            }
            else
            {
                cur_order_id = cur_order_id->next_ptr;
            }               
        }
        break;
#endif        
    default:
        break;
    }

    return mark_num;
}

/*****************************************************************************/
//  Description : compare bcd num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL int MMISMS_Bcdnumcmp(char * str1, char * str2, int length)
{
   while (length--)
   {
      if (*str1 > *str2)
         return 1;
      if (*str1++ < *str2++)
         return -1;
   }
   return 0;
}

/*****************************************************************************/
//  Description : mark all message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurOrderMarked( 
                                    MMISMS_ORDER_ID_T  cur_order_id,
                                    BOOLEAN is_marked
                                    )
{
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    cur_long_order_id = cur_order_id;
    while (cur_long_order_id != PNULL)
    {
        cur_long_order_id->flag.is_marked = is_marked;
        cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
    }
}

/*****************************************************************************/
//  Description : mark all message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMsgMarked( 
                                    BOOLEAN is_marked
                                    )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    
    cur_order_id =s_mmisms_order.mt_undownloaded_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    cur_order_id =s_mmisms_order.mt_unread_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    cur_order_id =s_mmisms_order.mt_readed_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
        

    cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id =s_mmisms_order.mo_draft_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id =s_mmisms_order.security_mt_undownloaded_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    cur_order_id =s_mmisms_order.security_head_ptr;
    while (cur_order_id != PNULL)
    {
        cur_long_order_id = cur_order_id;
        while (cur_long_order_id != PNULL)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
}

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
                                           )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    uint32 mark_num = 0;

    //SCI_TRACE_LOW:"MMISMS_GetMarkedStorageMsgNumByDualsys:box_type = %d,storage = %d,dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_8283_112_2_18_2_54_10_672,(uint8*)"ddd",box_type,storage,dual_sys);

    switch(box_type)
    {
        case MMISMS_BOX_MT:
            cur_order_id =s_mmisms_order.mt_unread_head_ptr;
            break;

        case MMISMS_BOX_SENDSUCC:
            cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
            break;

        case MMISMS_BOX_SENDFAIL:
            cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
            break;

        case MMISMS_BOX_NOSEND:
            cur_order_id =s_mmisms_order.mo_draft_head_ptr;
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
            break;
#endif
        default:
            break;
    }
    
    switch(box_type) 
    {
        case MMISMS_BOX_MT:
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  )
                {
                    if((cur_order_id->flag.storage == storage))
                    {
                        if(((MN_SMS_STORAGE_SIM == storage) &&
                           (cur_order_id->flag.dual_sys == dual_sys)) ||
                           (MN_SMS_STORAGE_ME == storage))
                        {
                            mark_num++;
                            cur_order_id = cur_order_id->next_ptr;
                        }
                        else
                        {
                            cur_order_id = cur_order_id->next_ptr;
                        }
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            cur_order_id =s_mmisms_order.mt_readed_head_ptr;
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  )
                {
                    if((cur_order_id->flag.storage == storage))
                    {
                        if(((MN_SMS_STORAGE_SIM == storage) &&
                           (cur_order_id->flag.dual_sys == dual_sys)) ||
                           (MN_SMS_STORAGE_ME == storage))
                        {
                            mark_num++;
                            cur_order_id = cur_order_id->next_ptr;
                        }
                        else
                        {
                            cur_order_id = cur_order_id->next_ptr;
                        }
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            break;
            
        case MMISMS_BOX_SENDSUCC:
        case MMISMS_BOX_SENDFAIL:
        case MMISMS_BOX_NOSEND:
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    if((cur_order_id->flag.storage == storage))
                    {
                        if(((MN_SMS_STORAGE_SIM == storage) &&
                           (cur_order_id->flag.dual_sys == dual_sys)) ||
                           (MN_SMS_STORAGE_ME == storage))
                        {
                            mark_num++;
                            cur_order_id = cur_order_id->next_ptr;
                        }
                        else
                        {
                            cur_order_id = cur_order_id->next_ptr;
                        }
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }                
            }
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    if((cur_order_id->flag.storage == storage))
                    {
                        if(((MN_SMS_STORAGE_SIM == storage) &&
                           (cur_order_id->flag.dual_sys == dual_sys)) ||
                           (MN_SMS_STORAGE_ME == storage))
                        {
                            mark_num++;
                            cur_order_id = cur_order_id->next_ptr;
                        }
                        else
                        {
                            cur_order_id = cur_order_id->next_ptr;
                        }
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            cur_order_id =s_mmisms_order.security_head_ptr;
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    if((cur_order_id->flag.storage == storage))
                    {
                        if(((MN_SMS_STORAGE_SIM == storage) &&
                           (cur_order_id->flag.dual_sys == dual_sys)) ||
                           (MN_SMS_STORAGE_ME == storage))
                        {
                            mark_num++;
                            cur_order_id = cur_order_id->next_ptr;
                        }
                        else
                        {
                            cur_order_id = cur_order_id->next_ptr;
                        }
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }               
            }
            break;
#endif            
        default:
            break;
    }

    //SCI_TRACE_LOW:"MMISMS_GetMarkedStorageMsgNumByDualsys:mark_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_8480_112_2_18_2_54_10_673,(uint8*)"d",mark_num);

    return mark_num;
}

/*****************************************************************************/
//  Description : get move num from storage 
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetMarkedStorageMsgNum(
                                            MMISMS_BOX_TYPE_E box_type,
                                            MN_SMS_STORAGE_E storage
                                           )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    uint32 mark_num = 0;
    
    //SCI_TRACE_LOW:"MMISMS_GetMarkedStorageMsgNum:box_type = %d,storage = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_8497_112_2_18_2_54_10_674,(uint8*)"dd",box_type,storage);

    switch(box_type)
    {
        case MMISMS_BOX_MT:
            cur_order_id =s_mmisms_order.mt_unread_head_ptr;
            break;

        case MMISMS_BOX_SENDSUCC:
            cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
            break;

        case MMISMS_BOX_SENDFAIL:
            cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
            break;

        case MMISMS_BOX_NOSEND:
            cur_order_id =s_mmisms_order.mo_draft_head_ptr;
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
            break;
#endif
        default:
            break;
    }
    
    switch(box_type) 
    {
        case MMISMS_BOX_MT:
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    if((MN_SMS_STORAGE_ALL == storage) ||
                       (cur_order_id->flag.storage == storage))
                    {
                        mark_num++;
                        cur_order_id = cur_order_id->next_ptr;
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            cur_order_id =s_mmisms_order.mt_readed_head_ptr;
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  )
                {
                    if((MN_SMS_STORAGE_ALL == storage) ||
                       (cur_order_id->flag.storage == storage))
                    {
                        mark_num++;
                        cur_order_id = cur_order_id->next_ptr;
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            break;
            
        case MMISMS_BOX_SENDSUCC:
        case MMISMS_BOX_SENDFAIL:
        case MMISMS_BOX_NOSEND:
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    if((MN_SMS_STORAGE_ALL == storage) ||
                       (cur_order_id->flag.storage == storage))
                    {
                        mark_num++;
                        cur_order_id = cur_order_id->next_ptr;
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }                
            }
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    if((MN_SMS_STORAGE_ALL == storage) ||
                       (cur_order_id->flag.storage == storage))
                    {
                        mark_num++;
                        cur_order_id = cur_order_id->next_ptr;
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            cur_order_id =s_mmisms_order.security_head_ptr;
            while (cur_order_id != PNULL)
            {
                if((MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    if((MN_SMS_STORAGE_ALL == storage) ||
                       (cur_order_id->flag.storage == storage))
                    {
                        mark_num++;
                        cur_order_id = cur_order_id->next_ptr;
                    }
                    else
                    {
                        cur_order_id = cur_order_id->next_ptr;
                    }
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }               
            }
            break;
#endif            
        default:
            break;
    }

    //SCI_TRACE_LOW:"MMISMS_GetMarkedStorageMsgNum:mark_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_8654_112_2_18_2_54_11_675,(uint8*)"d",mark_num);
    
    return mark_num;
}

/*****************************************************************************/
//  Description : get move num by type  
//  Global resource dependence :
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetMarkedMsgNumByType(
                                            MMISMS_BOX_TYPE_E box_type,
                                            MMISMS_MSG_TYPE_E msg_type
                                           )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    uint32 mark_num = 0;

    //SCI_TRACE_LOW:"MMISMS_GetMarkedMsgNumByType:box_type = %d,msg_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_8671_112_2_18_2_54_11_676,(uint8*)"dd",box_type,msg_type);

    switch(box_type)
    {
        case MMISMS_BOX_MT:
            cur_order_id =s_mmisms_order.mt_unread_head_ptr;
            break;

        case MMISMS_BOX_SENDSUCC:
            cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
            break;

        case MMISMS_BOX_SENDFAIL:
            cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
            break;

        case MMISMS_BOX_NOSEND:
            cur_order_id =s_mmisms_order.mo_draft_head_ptr;
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            cur_order_id =s_mmisms_order.security_mt_unread_head_ptr;
            break;
#endif
        default:
            break;
    }
    
    switch(box_type) 
    {
        case MMISMS_BOX_MT:
            while (cur_order_id != PNULL)
            {
                if((msg_type == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    mark_num++;
                    cur_order_id = cur_order_id->next_ptr;                   
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            cur_order_id =s_mmisms_order.mt_readed_head_ptr;
            while (cur_order_id != PNULL)
            {
                if((msg_type == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  )
                {
                    mark_num++;
                    cur_order_id = cur_order_id->next_ptr;                  
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            break;
            
        case MMISMS_BOX_SENDSUCC:
        case MMISMS_BOX_SENDFAIL:
        case MMISMS_BOX_NOSEND:
            while (cur_order_id != PNULL)
            {
                if((msg_type == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    mark_num++;
                    cur_order_id = cur_order_id->next_ptr;
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }                
            }
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            while (cur_order_id != PNULL)
            {
                if((msg_type == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    mark_num++;
                    cur_order_id = cur_order_id->next_ptr;
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }
            }
            cur_order_id =s_mmisms_order.security_head_ptr;
            while (cur_order_id != PNULL)
            {
                if((msg_type == cur_order_id->flag.msg_type) &&
                   (cur_order_id->flag.is_marked)
                  ) 
                {
                    mark_num++;
                    cur_order_id = cur_order_id->next_ptr;
                }
                else
                {
                    cur_order_id = cur_order_id->next_ptr;
                }               
            }
            break;
#endif            
        default:
            break;
    }

    //SCI_TRACE_LOW:"MMISMS_GetMarkedMsgNumByType:mark_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_8788_112_2_18_2_54_11_677,(uint8*)"d",mark_num);
    
    return mark_num;
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related MO SMS box
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMoSMSForMoveAll(
                                                         MMISMS_BOX_TYPE_E box_type,
                                                         MN_SMS_STORAGE_E storage)
{
    MMISMS_ORDER_ID_T order_id = PNULL;

    switch(box_type)
    {
        case MMISMS_BOX_SENDSUCC:
            order_id = s_mmisms_order.mo_sendsucc_head_ptr;
            break;

        case MMISMS_BOX_SENDFAIL:
            order_id = s_mmisms_order.mo_sendfail_head_ptr;
            break;            

        case MMISMS_BOX_NOSEND:
            order_id = s_mmisms_order.mo_draft_head_ptr;
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            order_id = s_mmisms_order.security_head_ptr;
            break;
#endif
        default:
            break;
    }

    while (PNULL != order_id)
    {
        if((MMISMS_TYPE_SMS == order_id->flag.msg_type) &&
           (order_id->flag.is_marked) //is  marked
          )
        {
            if((MN_SMS_STORAGE_SIM == storage) &&
               (order_id->flag.storage == MN_SMS_STORAGE_SIM))
            {
                break;
            }
            else if(MN_SMS_STORAGE_ALL == storage)
            {
                if((order_id->flag.storage == MN_SMS_STORAGE_ME) ||
                   (order_id->flag.storage == MN_SMS_STORAGE_SIM) &&
                   (order_id->flag.dual_sys != MMISMS_GetCurSaveDualSys()))
                {
                    break;
                }
                else
                {
                    order_id->flag.is_marked = FALSE;
                    order_id = order_id->next_ptr;
                }
            }
            else
            {
                order_id->flag.is_marked = FALSE;
                order_id = order_id->next_ptr;
            }
        }
        else
        {
            order_id->flag.is_marked = FALSE;
            order_id = order_id->next_ptr;
        }            
    }
    
    return order_id ;
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related MT SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMTSMSForMoveAll(MN_SMS_STORAGE_E storage)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    order_id = s_mmisms_order.mt_unread_head_ptr;
    
    while(order_id != PNULL)//is  marked
    {
        if((MMISMS_TYPE_SMS == order_id->flag.msg_type) &&
           (order_id->flag.is_marked) //is  marked
          )
        {
            //�ƶ����Ƶ��ֻ�
            if((MN_SMS_STORAGE_SIM == storage) &&
               (order_id->flag.storage == MN_SMS_STORAGE_SIM))
            {
                break;
            }
            //�ƶ����Ƶ�SIM����(1)��NV�в���(2)ԭ�����Ѵ��ڵ���Ϣ���ƶ�
            else if(MN_SMS_STORAGE_ALL == storage)
            {
                if((order_id->flag.storage == MN_SMS_STORAGE_ME) ||
                   (order_id->flag.storage == MN_SMS_STORAGE_SIM) &&
                   (order_id->flag.dual_sys != MMISMS_GetCurSaveDualSys()))
                {
                    break;
                }
                else
                {
                    order_id->flag.is_marked = FALSE;
                    order_id = order_id->next_ptr;
                }
            }
            else
            {
                order_id->flag.is_marked = FALSE;
                order_id = order_id->next_ptr;
            }
        }
        else
        {
            order_id->flag.is_marked = FALSE;
            order_id = order_id->next_ptr;
        }
    }
    if (order_id == PNULL)
    {
        order_id = s_mmisms_order.mt_readed_head_ptr;
        while(order_id != PNULL)//is  marked
        {
            if((MMISMS_TYPE_SMS == order_id->flag.msg_type) &&
               (order_id->flag.is_marked) //is  marked
              )
            {
                if((MN_SMS_STORAGE_SIM == storage) &&
                   (order_id->flag.storage == MN_SMS_STORAGE_SIM))
                {
                    break;
                }
                else if(MN_SMS_STORAGE_ALL == storage)
                {
                    if((order_id->flag.storage == MN_SMS_STORAGE_ME) ||
                       (order_id->flag.storage == MN_SMS_STORAGE_SIM) &&
                       (order_id->flag.dual_sys != MMISMS_GetCurSaveDualSys()))
                    {
                        break;
                    }
                    else
                    {
                        order_id->flag.is_marked = FALSE;
                        order_id = order_id->next_ptr;
                    }
                }
                else
                {
                    order_id->flag.is_marked = FALSE;
                    order_id = order_id->next_ptr;
                }
            }
            else
            {
                order_id->flag.is_marked = FALSE;
                order_id = order_id->next_ptr;
            }
        }
    }        

    return order_id;    
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related Security SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecuritySMSForMoveAll(MN_SMS_STORAGE_E storage)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    order_id = s_mmisms_order.security_mt_unread_head_ptr;
    
    while(order_id != PNULL)//is  marked
    {
        if((MMISMS_TYPE_SMS == order_id->flag.msg_type) &&
           (order_id->flag.is_marked)//is  marked
          )
        {
            if((MN_SMS_STORAGE_SIM == storage) &&
               (order_id->flag.storage == MN_SMS_STORAGE_SIM))
            {
                break;
            }
            else if(MN_SMS_STORAGE_ALL == storage)
            {
                if((order_id->flag.storage == MN_SMS_STORAGE_ME) ||
                   (order_id->flag.storage == MN_SMS_STORAGE_SIM) &&
                   (order_id->flag.dual_sys != MMISMS_GetCurSaveDualSys()))
                {
                    break;
                }
                else
                {
                    order_id->flag.is_marked = FALSE;
                    order_id = order_id->next_ptr;
                }
            }
            else
            {
                order_id->flag.is_marked = FALSE;
                order_id = order_id->next_ptr;
            }
        }
        else
        {
            order_id->flag.is_marked = FALSE;
            order_id = order_id->next_ptr;
        }
    }
    if (order_id == PNULL)
    {
        order_id = s_mmisms_order.security_head_ptr;
        while(order_id != PNULL)//is  marked
        {
            if((MMISMS_TYPE_SMS == order_id->flag.msg_type) &&
               (order_id->flag.is_marked)//is  marked
              )
            {
                if((MN_SMS_STORAGE_SIM == storage) &&
                   (order_id->flag.storage == MN_SMS_STORAGE_SIM))
                {
                    break;
                }
                else if(MN_SMS_STORAGE_ALL == storage)
                {
                    if((order_id->flag.storage == MN_SMS_STORAGE_ME) ||
                       (order_id->flag.storage == MN_SMS_STORAGE_SIM) &&
                       (order_id->flag.dual_sys != MMISMS_GetCurSaveDualSys()))
                    {
                        break;
                    }
                    else
                    {
                        order_id->flag.is_marked = FALSE;
                        order_id = order_id->next_ptr;
                    }
                }
                else
                {
                    order_id->flag.is_marked = FALSE;
                    order_id = order_id->next_ptr;
                }
            }
            else
            {
                order_id->flag.is_marked = FALSE;
                order_id = order_id->next_ptr;
            }
        }
    }        

    return order_id;    
}

/*****************************************************************************/
//  Function Name: IsSameSMS
//  Description  : compare two sms in order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSameSMS(
                        MMISMS_STATE_T *sms1_state_ptr,
                        MMISMS_STATE_T *sms2_state_ptr
                       )
{
    //�Ƚ�record_id,storage,dual_sys
    if((sms1_state_ptr->record_id == sms2_state_ptr->record_id) &&
       (sms1_state_ptr->flag.storage == sms2_state_ptr->flag.storage) &&
       (sms1_state_ptr->flag.dual_sys == sms2_state_ptr->flag.dual_sys)
      )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :SmsBCDToPbBCD
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMISMS_SmsBCDToPbBCD(MMISMS_BCD_NUMBER_T sms_num)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};
    pb_num.number_len = sms_num.number_len;
    pb_num.npi_ton = sms_num.npi_ton;
    SCI_MEMCPY(pb_num.number,sms_num.number,MIN(sms_num.number_len,MMISMS_BCD_NUMBER_MAX_LEN));
    return pb_num;    
}

/*****************************************************************************/
//  Function Name: CompareMMSNum
//  Description  : compare two MMS num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CompareMMSNum(
                            uint8  *compare_sender_ptr1,
                            uint8 *compare_sender_ptr2
                           )
{
    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
    MN_NUMBER_TYPE_E    sender_type = MN_NUM_TYPE_UNKNOW;
                                
    BOOLEAN ret = FALSE;
    if(PNULL == compare_sender_ptr1 || PNULL == compare_sender_ptr2)
    {
        return ret;
    }
    
    if(strlen((char *)compare_sender_ptr1) > 1 && (0x2B == compare_sender_ptr1[0]))
    {
        sender_type = MN_NUM_TYPE_INTERNATIONAL;
        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
        (char*)&compare_sender_ptr1[1], 
        pb_bcd1.number);  
        pb_bcd1.number_len =  (strlen((char *)&compare_sender_ptr1[1]) + 1)/2;    
    }
    else
    {       
        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
            (char*)compare_sender_ptr1, 
            pb_bcd1.number);                    
        pb_bcd1.number_len =  (strlen((char *)compare_sender_ptr1) + 1)/2;    
    }

    pb_bcd1.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(sender_type, MN_NUM_PLAN_UNKNOW);

    sender_type = MN_NUM_TYPE_UNKNOW;
    if(strlen((char *)compare_sender_ptr2) > 1 && (0x2B == compare_sender_ptr2[0]))
    {
        sender_type = MN_NUM_TYPE_INTERNATIONAL;
        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
        (char*)&compare_sender_ptr2[1], 
        pb_bcd2.number);     
        pb_bcd2.number_len =  (strlen((char *)&compare_sender_ptr2[1]) + 1)/2;    
    }
    else
    {

        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
            (char*)compare_sender_ptr2, 
            pb_bcd2.number);   
        pb_bcd2.number_len =  (strlen((char *)compare_sender_ptr2) + 1)/2;    
    }
                                          
    pb_bcd2.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(sender_type, MN_NUM_PLAN_UNKNOW);   
    ret = CompareSMSNum(&pb_bcd1,&pb_bcd2);
    return ret;    
}

/*****************************************************************************/
//  Function Name: CompareSMSNum
//  Description  : compare two sms num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareSMSNum(
                            MMIPB_BCD_NUMBER_T  *compare_number_ptr1,
                            MMIPB_BCD_NUMBER_T *compare_number_ptr2
                           )
{
    if(0 == MMIAPIPB_BCDCmp(compare_number_ptr1,compare_number_ptr2))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : search from the current number, and find the valid index for 
//                moving the related MT SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidChatSMSMoveToSecurity(MMIPB_BCD_NUMBER_T pb_num)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};    
      
    SCI_TRACE_LOW("MMISMS_FindValidChatSMSMoveToSecurity: pb_num.number = %s",pb_num.number);

    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while ((PNULL != head_chat_id))
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return PNULL;
    }

    order_id = head_chat_id->head_ptr;  
    while(order_id != PNULL)//is  marked
    {
        if(order_id->flag.is_marked)
        {
            break;
        }
        else
        {
            order_id->flag.is_marked = FALSE;
            order_id = order_id->next_chat_ptr;
        }
    }
      
    return order_id;    
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related MT SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidChatSMSForMoveAll(MMIPB_BCD_NUMBER_T pb_num,MN_SMS_STORAGE_E storage)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};
    SCI_TRACE_LOW("MMISMS_FindValidChatSMSForMoveAll: pb_num.number = %s",pb_num.number);
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return PNULL;
    }

    order_id = head_chat_id->head_ptr;  
    while(order_id != PNULL)//is  marked
    {
        if((MMISMS_TYPE_SMS == order_id->flag.msg_type) &&
           (order_id->flag.is_marked) //is  marked
          )
        {
            //�ƶ����Ƶ��ֻ�
            if((MN_SMS_STORAGE_SIM == storage) &&
               (order_id->flag.storage == MN_SMS_STORAGE_SIM))
            {
                break;
            }
            //�ƶ����Ƶ�SIM����(1)��NV�в���(2)ԭ�����Ѵ��ڵ���Ϣ���ƶ�
            else if(MN_SMS_STORAGE_ALL == storage)///@@@@
            {
                if((order_id->flag.storage == MN_SMS_STORAGE_ME) ||
                   (order_id->flag.storage == MN_SMS_STORAGE_SIM) &&
                   (order_id->flag.dual_sys != MMISMS_GetCurSaveDualSys()))
                {
                    break;
                }
                else
                {
                    order_id->flag.is_marked = FALSE;
                    order_id = order_id->next_chat_ptr;
                }
            }
            else
            {
                order_id->flag.is_marked = FALSE;
                order_id = order_id->next_chat_ptr;
            }
        }
        else
        {
            order_id->flag.is_marked = FALSE;
            order_id = order_id->next_chat_ptr;
        }
    }
      
    return order_id;    
}

/*****************************************************************************/
//  Description : �ض�index�Ựmark�������������Ų�ͳ��
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetCurChatMarkedMsgNumByDualsys(MMIPB_BCD_NUMBER_T pb_num,MN_SMS_STORAGE_E storage,MN_DUAL_SYS_E    dual_sys)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint32 mark_num = 0;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};

    SCI_TRACE_LOW("MMISMS_GetCurChatMarkedMsgNumByDualsys: pb_num.number = %s",pb_num.number);
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return 0;
    }

    cur_order_id = head_chat_id->head_ptr;  

    while (PNULL != cur_order_id)
    {
        if(cur_order_id->flag.is_marked)
        {
            if((cur_order_id->flag.storage == storage))
            {
                if(((MN_SMS_STORAGE_SIM == storage) &&
                (cur_order_id->flag.dual_sys == dual_sys)) ||
                (MN_SMS_STORAGE_ME == storage))
                {
                    mark_num++;
                }
            }
        }               
        cur_order_id = cur_order_id->next_chat_ptr;
    }   
    
    return mark_num;
}

/*****************************************************************************/
//  Description : �ض�index�Ựmark�������������Ų�ͳ��
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetCurChatMarkedMsgNum(MMIPB_BCD_NUMBER_T pb_num,MN_SMS_STORAGE_E storage)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint32 mark_num = 0;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};
    
    SCI_TRACE_LOW("MMISMS_GetCurChatMarkedMsgNum: pb_num.number = %s",pb_num.number);
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return 0;
    }

    cur_order_id = head_chat_id->head_ptr;  

    while (PNULL != cur_order_id)
    {
        if( (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) && cur_order_id->flag.is_marked)
        {                 
            if((MN_SMS_STORAGE_ALL == storage) ||
                       (cur_order_id->flag.storage == storage))
            {
                mark_num++;
            }
        }               
        cur_order_id = cur_order_id->next_chat_ptr;
    }
    return mark_num;
}

/*****************************************************************************/
//  Description : �жϵ�ǰ������������Ƿ������ڷ��ͻ�ȴ�����Ϣ
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ChatNumberSendOrWaitIsExsit(MMIPB_BCD_NUMBER_T pb_num)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_long_order_id = PNULL;
    BOOLEAN is_sending_or_waiting = FALSE;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};

    SCI_TRACE_LOW("MMISMS_ChatNumberSendOrWaitIsExsit: pb_num.number = %s",pb_num.number);
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return FALSE;
    }

    cur_order_id = head_chat_id->head_ptr;  

    if(MMISMS_CHAT_MAX_ITEM < head_chat_id->total_num)
    {
        uint32 begin_index = head_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
        
        while((PNULL != cur_order_id) && (0 < begin_index))
        {
            cur_order_id = cur_order_id->next_chat_ptr;
            begin_index--;
        }
    }
        
    while (PNULL != cur_order_id)
    {        
        cur_long_order_id = cur_order_id->next_long_sms_ptr;
            
        while (PNULL != cur_long_order_id)
        {
            if((MMISMS_SENDSTATUS_WAITING == cur_long_order_id->backsend_status) || (MMISMS_SENDSTATUS_SENDING == cur_long_order_id->backsend_status))
            {
               is_sending_or_waiting = TRUE;
               break;
            }
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;            
        }        
        if((MMISMS_SENDSTATUS_WAITING == cur_order_id->backsend_status) || (MMISMS_SENDSTATUS_SENDING == cur_order_id->backsend_status))
        {
            is_sending_or_waiting = TRUE;
        }
        if(is_sending_or_waiting)
        {
            break;
        }
        cur_order_id = cur_order_id->next_chat_ptr;
    }
    return is_sending_or_waiting;
}
                 
/*****************************************************************************/
//  Description : insert node to chat order
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertNodeToChatOrder(
                                    MMISMS_STATE_T *new_state_ptr,
                                    MMISMS_TYPE_E type
                                    )
{
    BOOLEAN result = TRUE;
    MMISMS_CHAT_STATE_UNIT_T *new_number_ptr = PNULL;
    MMISMS_CHAT_STATE_UNIT_T  *first_number_ptr = s_mmisms_chat_linked_order.head_ptr;
    MMISMS_CHAT_STATE_UNIT_T  *first_node_ptr = PNULL;

    if(PNULL == new_state_ptr)
    {
        //SCI_TRACE_LOW:"InsertNodeToChatOrder: new_state_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9375_112_2_18_2_54_12_683,(uint8*)"");
        return FALSE;
    }

    //�ݸ���Ͱ�ȫ���䲻���������촰����
    if((MMISMS_MO_DRAFT == new_state_ptr->flag.mo_mt_type) ||
       (MMISMS_FOLDER_SECURITY == new_state_ptr->flag.folder_type) ||
       (MMISMS_TYPE_SMS != new_state_ptr->flag.msg_type &&
        MMISMS_TYPE_MMS != new_state_ptr->flag.msg_type))
    {
        //SCI_TRACE_LOW:"InsertNodeToChatOrder: mo_mt_type is %d,folder_type is %d,msg_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9386_112_2_18_2_54_12_684,(uint8*)"ddd",new_state_ptr->flag.mo_mt_type,new_state_ptr->flag.folder_type,new_state_ptr->flag.msg_type);
        return FALSE;
    }

    first_node_ptr = first_number_ptr;
    if (PNULL != first_number_ptr)
    {
        if (MMISMS_TYPE_MMS == new_state_ptr->flag.msg_type)
        {
            //�ȽϺ���
            while(PNULL != first_number_ptr)
            {
                if (MMISMS_TYPE_SMS == first_number_ptr->head_ptr->flag.msg_type)//�̲ʱȽ�
                {              
                    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
                    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
                    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
                    
                    if(strlen((char *)new_state_ptr->number.sender) > 1 && (0x2B == new_state_ptr->number.sender[0]))
                    {
                        number_type = MN_NUM_TYPE_INTERNATIONAL;
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)&new_state_ptr->number.sender[1], 
                            pb_bcd2.number);  
                        pb_bcd2.number_len =  (strlen((char *)&new_state_ptr->number.sender[1]) + 1)/2;    
                    }
                    else
                    {       
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)new_state_ptr->number.sender, 
                            pb_bcd2.number);                    
                        pb_bcd2.number_len =  (strlen((char *)new_state_ptr->number.sender) + 1)/2;    
                    }

                    pb_bcd2.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(number_type, MN_NUM_PLAN_UNKNOW);
                    
                    pb_bcd1 = MMISMS_SmsBCDToPbBCD(first_number_ptr->bcd_num);                    
                        
                    if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                    {
                        break;
                    }
                }
                else
                {                 
                    if(MMISMS_CompareMMSNum(first_number_ptr->head_ptr->number.sender,new_state_ptr->number.sender))
                    {
                        break;
                    }
                }
                first_number_ptr = first_number_ptr->next_number_ptr;
            }
        }
        else
        {
            //�ȽϺ���
            while(PNULL != first_number_ptr)
            {
                if (MMISMS_TYPE_SMS == first_number_ptr->head_ptr->flag.msg_type)
                {
                    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
                    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
                    pb_bcd1 = MMISMS_SmsBCDToPbBCD(first_number_ptr->bcd_num);
                    pb_bcd2 = MMISMS_SmsBCDToPbBCD(new_state_ptr->number.bcd_number);
                    
                    if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                    {
                        break;
                    }
                }
                else
                {
                    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
                    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
                    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;

                    if(strlen((char *)first_number_ptr->head_ptr->number.sender) > 1 && (0x2B == first_number_ptr->head_ptr->number.sender[0]))
                    {
                        number_type = MN_NUM_TYPE_INTERNATIONAL;
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)&first_number_ptr->head_ptr->number.sender[1], 
                            pb_bcd1.number);                    
                        pb_bcd1.number_len =  (strlen((char *)&first_number_ptr->head_ptr->number.sender[1]) + 1)/2;    
                    }
                    else
                    {                                  
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)first_number_ptr->head_ptr->number.sender, 
                            pb_bcd1.number);                    
                        pb_bcd1.number_len =  (strlen((char *)first_number_ptr->head_ptr->number.sender) + 1)/2;    
                    }
                    pb_bcd1.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(number_type, MN_NUM_PLAN_UNKNOW);  
                    
                    pb_bcd2 = MMISMS_SmsBCDToPbBCD(new_state_ptr->number.bcd_number); 
                        
                    if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                    {
                        break;
                    }
                }

                first_number_ptr = first_number_ptr->next_number_ptr;
            }
        }
    }

    //�ҵ�����ͬ�ĺ��룬�����Ƚ�ʱ��
    if(PNULL != first_number_ptr)
    {
        InsertSameNumNodeToChatOrderByTime(first_node_ptr,first_number_ptr,new_state_ptr,type);
        if(MMISMS_ORDER_BY_NAME == MMISMS_GetOrderType())
        {
            SortChatLinkedOrderByName();
        }
        else
        {
            SortChatLinkedOrderByTime();
        }
        GetChatNodeTotalUnreadNum();
    }
    //δ�ҵ������½�һ������ڵ�
    else
    {
        new_number_ptr = LinkedChatAllocNode();
        if(PNULL != new_number_ptr)
        {
            s_mmisms_chat_linked_order.uint_num++;
            SetNewChatNode(new_number_ptr,new_state_ptr);
            if(MMISMS_ORDER_BY_NAME == MMISMS_GetOrderType())
            {
                SortChatLinkedOrderByName();
            }
            else
            {
                SortChatLinkedOrderByTime();
            }
            GetChatNodeTotalUnreadNum();
        }
        else
        {
            result = FALSE;
        }
    }

    g_mmisms_global.chat_oper_order_id = new_state_ptr;
    
    return result;
}

/*****************************************************************************/
//  Function Name: LinkedArrayAllocNode
//  Description  : Alloc linked array
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_CHAT_STATE_UNIT_T*  LinkedChatAllocNode(void)
{    
    uint32 i = 0;

    for(i=0;i<s_mmisms_linked_order.unit_num;i++)
    {
        if(!s_mmisms_chat_linked_order.head_node_ptr[i].is_valid)
        {
            return &s_mmisms_chat_linked_order.head_node_ptr[i];
        }
    }
    return PNULL;
}

/*****************************************************************************/
//  Function Name: SetNewChatNode
//  Description  : set new chat node
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetNewChatNode(
                          MMISMS_CHAT_STATE_UNIT_T *unit_ptr,
                          MMISMS_STATE_T *new_state_ptr
                         )
{
    MMISMS_BCD_NUMBER_T bcd_num_data = {0};

    SCI_MEMSET(unit_ptr,0x0,sizeof(MMISMS_CHAT_STATE_UNIT_T));

    unit_ptr->is_valid = TRUE;
    unit_ptr->total_num++;
    unit_ptr->time = new_state_ptr->time;

    if (MMISMS_TYPE_SMS == new_state_ptr->flag.msg_type)
    {
        SCI_MEMCPY(&unit_ptr->bcd_num, &new_state_ptr->number.bcd_number, sizeof(MMISMS_BCD_NUMBER_T));
    }
    else
    {
        MMISMS_MMSNumberToBcd((char*)new_state_ptr->number.sender, bcd_num_data.number,&bcd_num_data.number_len);   
        SCI_MEMCPY(&unit_ptr->bcd_num, &bcd_num_data, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMCPY(&unit_ptr->sender, &new_state_ptr->number.sender,  (sizeof(uint8)*(MMISMS_NUMBER_MAX_LEN+1)));
    }

    AdjustChatNode(unit_ptr,new_state_ptr);
}

/*****************************************************************************/
//  Function Name: InsertSameNumNodeToChatOrderByTime
//  Description  : set chat order by time
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InsertSameNumNodeToChatOrderByTime(
                                              MMISMS_CHAT_STATE_UNIT_T  *first_node_ptr,
                                              MMISMS_CHAT_STATE_UNIT_T  *number_head_ptr,
                                              MMISMS_STATE_T *new_state_ptr,
                                              MMISMS_TYPE_E type
                                             )
{
    uint32 time = new_state_ptr->time;
    MMISMS_STATE_T *head_num_ptr = number_head_ptr->head_ptr;
    MMISMS_STATE_T *head_ptr = number_head_ptr->head_ptr;
    MMISMS_STATE_T *long_head_ptr = number_head_ptr->head_ptr;
    MMISMS_STATE_T *cur_ptr = PNULL;
    BOOLEAN is_pos_found = FALSE;
    uint16 index = 0;
    //�����ŵ����
    if(new_state_ptr->flag.is_concatenate_sms)  //�������Ȳ���ref�������ǱȽ�ʱ��
    {
		while(PNULL != head_ptr)
		{
            if(HaveSamePart(head_ptr,new_state_ptr)) //�����е�һ��
            {    
                 is_pos_found = TRUE;
                 break;
            }
            else
            {
                 head_ptr = head_ptr->next_chat_ptr;
                 index++;
                 if(head_ptr && index > number_head_ptr->total_num)
                 {
                     head_ptr->next_chat_ptr = PNULL;
				     break;                
		         }
            }
		}
    }
    
    if(!is_pos_found)  //����ҵ�����Ҫ����ʱ�����
    {   
        head_ptr = number_head_ptr->head_ptr;
        index = 0;
        while(PNULL != head_ptr)
        {
            if(time < head_ptr->time)
            {
                break;
            }
            head_ptr = head_ptr->next_chat_ptr;
            index++;
            if(head_ptr && index > number_head_ptr->total_num)
            {
                head_ptr->next_chat_ptr = PNULL;
                break;                
		    }
        }
    }

    if(PNULL == head_ptr)//�嵽�˺����б�ĩβ
    {
        number_head_ptr->time = time;
        while(PNULL != head_num_ptr)
        {
            if(PNULL == head_num_ptr->next_chat_ptr)
            {
                break;
            }
            head_num_ptr = head_num_ptr->next_chat_ptr;
        }

        //��ͨ����
        if(!new_state_ptr->flag.is_concatenate_sms)
        {
            number_head_ptr->total_num++;
            if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
            {
                number_head_ptr->unread_num++;
            }
            if(PNULL != head_num_ptr)
            {
                head_num_ptr->next_chat_ptr = new_state_ptr;
                new_state_ptr->next_chat_ptr = PNULL;
                new_state_ptr->prev_chat_ptr = head_num_ptr;
            }
        }
        //������
        else
        {
            cur_ptr = long_head_ptr;
            
            //cur_ptr�ƶ���ĳһ��㣬ʹҪ�����ʱ�䲻С�ڵ�ǰ��ָ��ʱ�䣬����cur_ptrδָ�����һ�����
            while(PNULL != cur_ptr)                 
            {

                if(HaveSamePart(cur_ptr,new_state_ptr))  //�����е�һ��
                {
                    break;
                }
                else
                {
                    cur_ptr = cur_ptr->next_chat_ptr;
                }
            }
            
            if (PNULL != cur_ptr) //�ҵ�ref��ͬ��λ�ã���û���ظ��Ĳ���
            {
                //�����Ѷ�s_mmisms_order�����˲�����������������Ϣ���ڶ�����
                //������Ϣ��seq��ԭ�����ŵ�һ����seqҪС��������Ϣ��Ϊ�˳����ŵ�ͷ�ڵ�
                if(new_state_ptr->longsms_seq_num < cur_ptr->longsms_seq_num)
                {
                    new_state_ptr->next_chat_ptr = PNULL;
                    new_state_ptr->prev_chat_ptr = cur_ptr->prev_chat_ptr;
                    if(PNULL != cur_ptr->prev_chat_ptr)
                    {
                        cur_ptr->prev_chat_ptr->next_chat_ptr = new_state_ptr;
                    }
                    else
                    {
                        number_head_ptr->head_ptr = new_state_ptr;
                    }                    
                    cur_ptr->prev_chat_ptr = PNULL;
                    cur_ptr->next_chat_ptr = PNULL;
                }
                //SCI_TRACE_LOW:"InsertSameNumNodeToChatOrderByTime: long sms not find in order!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9666_112_2_18_2_54_13_685,(uint8*)"");
            }
            else //û���ҵ�ref��ͬ��λ�ã����ǳ������е�һ�������
            {
                number_head_ptr->total_num++;
                if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
                {
                    number_head_ptr->unread_num++;
                }
                if(PNULL != head_num_ptr)
                {
                    head_num_ptr->next_chat_ptr = new_state_ptr;
                    new_state_ptr->next_chat_ptr = PNULL;
                    new_state_ptr->prev_chat_ptr = head_num_ptr;
                }
            }    
        }
    }
    else//�嵽�б��м�
    {
        //��ͨ����
        if(!new_state_ptr->flag.is_concatenate_sms)
        {
            number_head_ptr->total_num++;
            if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
            {
                number_head_ptr->unread_num++;
            }
            new_state_ptr->prev_chat_ptr = head_ptr->prev_chat_ptr;
            new_state_ptr->next_chat_ptr = head_ptr;
            if(PNULL != head_ptr->prev_chat_ptr)
            {
                head_ptr->prev_chat_ptr->next_chat_ptr = new_state_ptr;
            }
            else
            {
                number_head_ptr->head_ptr = new_state_ptr;
            }
            
            head_ptr->prev_chat_ptr = new_state_ptr;
        }
        //������
        else
        {
            cur_ptr = long_head_ptr;
            
            //cur_ptr�ƶ���ĳһ��㣬ʹҪ�����ʱ�䲻С�ڵ�ǰ��ָ��ʱ�䣬����cur_ptrδָ�����һ�����
            while(PNULL != cur_ptr)                 
            {
                if(HaveSamePart(cur_ptr,new_state_ptr)) //�����е�һ��
                {
                    break;
                }
                else
                {
                    cur_ptr = cur_ptr->next_chat_ptr;
                }
            }
            
            if (PNULL != cur_ptr) //�ҵ�ref��ͬ��λ��,��û�н�����������û���ظ��Ĳ���
            {
                //�����Ѷ�s_mmisms_order�����˲�����������������Ϣ���ڶ�����
                //������Ϣ��seq��ԭ�����ŵ�һ����seqҪС��������Ϣ��Ϊ�˳����ŵ�ͷ�ڵ�
                if(new_state_ptr->longsms_seq_num < cur_ptr->longsms_seq_num)
                {
                    new_state_ptr->next_chat_ptr = cur_ptr->next_chat_ptr;
                    new_state_ptr->prev_chat_ptr = cur_ptr->prev_chat_ptr;
                    if(PNULL != cur_ptr->prev_chat_ptr)
                    {
                        cur_ptr->prev_chat_ptr->next_chat_ptr = new_state_ptr;
                    }
                    else
                    {
                        number_head_ptr->head_ptr = new_state_ptr;
                    }
                    if(PNULL != cur_ptr->next_chat_ptr)
                    {
                        cur_ptr->next_chat_ptr->prev_chat_ptr = new_state_ptr;
                    }                    
                    cur_ptr->prev_chat_ptr = PNULL;
                    cur_ptr->next_chat_ptr = PNULL;
                }
                //SCI_TRACE_LOW:"InsertSameNumNodeToChatOrderByTime: long sms not find in order!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9748_112_2_18_2_54_13_686,(uint8*)"");
            }
            else //û���ҵ�ref��ͬ��λ�ã����ǳ������е�һ�������
            {
                number_head_ptr->total_num++;
                if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
                {
                    number_head_ptr->unread_num++;
                }
                new_state_ptr->prev_chat_ptr = head_ptr->prev_chat_ptr;
                new_state_ptr->next_chat_ptr = head_ptr;
                if(PNULL != head_ptr->prev_chat_ptr)
                {
                    head_ptr->prev_chat_ptr->next_chat_ptr = new_state_ptr;
                }
                else
                {
                    number_head_ptr->head_ptr = new_state_ptr;
                }
                head_ptr->prev_chat_ptr = new_state_ptr;
            }    
        }
    }
}

/*****************************************************************************/
//  Function Name: AdjustChatNode
//  Description  : adjust s_mmisms_chat_linked_order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void AdjustChatNode(
                          MMISMS_CHAT_STATE_UNIT_T *unit_ptr,
                          MMISMS_STATE_T *new_state_ptr
                         )
{
    uint32 new_time = new_state_ptr->time;
    MMISMS_CHAT_STATE_UNIT_T *number_node = s_mmisms_chat_linked_order.head_ptr;
    uint32 node_latest_time = 0;
    
    //�������޺���
    if(PNULL == number_node)
    {
        if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
        {
            unit_ptr->unread_num++;
        }
        unit_ptr->head_ptr = new_state_ptr;
        unit_ptr->next_number_ptr = PNULL;
        unit_ptr->prev_number_ptr = PNULL;
        s_mmisms_chat_linked_order.head_ptr = unit_ptr;
    }
    //���������������ʱ��Ƚ�
    else
    {
        node_latest_time = number_node->time;
        //ʱ�����£��嵽�����б�ĵ�һ��
        if(node_latest_time < new_time)
        {
            if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
            {
                unit_ptr->unread_num++;
            }
            unit_ptr->head_ptr = new_state_ptr;
            unit_ptr->next_number_ptr = number_node;
            unit_ptr->prev_number_ptr = PNULL;
            number_node->prev_number_ptr = unit_ptr;
            s_mmisms_chat_linked_order.head_ptr = unit_ptr;            
        }
        //����������ʱ�������������������
        else
        {
            while(PNULL != number_node)
            {
                node_latest_time = number_node->time;
                
                if(node_latest_time < new_time)
                {
                    break;
                }
                number_node = number_node->next_number_ptr;
            }
            //���뵽����β
            if(PNULL == number_node)
            {
                MMISMS_CHAT_STATE_UNIT_T *chat_node_ptr = s_mmisms_chat_linked_order.head_ptr;
                //���������һ��Ԫ��
                while(PNULL != chat_node_ptr)
                {
                    if(PNULL == chat_node_ptr->next_number_ptr)
                    {
                        break;
                    }
                    chat_node_ptr = chat_node_ptr->next_number_ptr;
                }

                if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
                {
                    unit_ptr->unread_num++;
                }
                unit_ptr->head_ptr = new_state_ptr;
                unit_ptr->next_number_ptr = PNULL;
                unit_ptr->prev_number_ptr = chat_node_ptr;
                if(PNULL != chat_node_ptr)
                {
                    chat_node_ptr->next_number_ptr = unit_ptr;
                }
            }
            //���뵽�����м�
            else
            {
                if(MMISMS_MT_TO_BE_READ == new_state_ptr->flag.mo_mt_type)
                {
                    unit_ptr->unread_num++;
                }
                unit_ptr->head_ptr = new_state_ptr;
                unit_ptr->next_number_ptr = number_node;
                unit_ptr->prev_number_ptr = number_node->prev_number_ptr;
                number_node->prev_number_ptr->next_number_ptr = unit_ptr;
                number_node->prev_number_ptr = unit_ptr;
            }
        }
    }
}

/*****************************************************************************/
//  Description : delete node from chat order
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteNodeFromChatOrder(
                                      MMISMS_STATE_T *delete_state_ptr,
                                      MMISMS_TYPE_E type
                                     )
{
    BOOLEAN result = TRUE;
    MMISMS_CHAT_STATE_UNIT_T  *first_number_ptr = s_mmisms_chat_linked_order.head_ptr;
    MMISMS_CHAT_STATE_UNIT_T  *first_node_ptr = PNULL;

    if(PNULL == delete_state_ptr)
    {
        //SCI_TRACE_LOW:"DeleteNodeFromChatOrder: delete_state_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9888_112_2_18_2_54_14_687,(uint8*)"");
        return FALSE;
    }

    if((MMISMS_MO_DRAFT == delete_state_ptr->flag.mo_mt_type) ||
       (MMISMS_FOLDER_SECURITY == delete_state_ptr->flag.folder_type) ||
       (MMISMS_TYPE_SMS != delete_state_ptr->flag.msg_type &&
        MMISMS_TYPE_MMS != delete_state_ptr->flag.msg_type))
    {
        //SCI_TRACE_LOW:"DeleteNodeFromChatOrder: mo_mt_type is %d,folder_type is %d,msg_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9898_112_2_18_2_54_14_688,(uint8*)"ddd",delete_state_ptr->flag.mo_mt_type,delete_state_ptr->flag.folder_type,delete_state_ptr->flag.msg_type);
        return FALSE;
    }

    first_node_ptr = first_number_ptr;
   if (PNULL != first_number_ptr)
    {
        if (MMISMS_TYPE_MMS == delete_state_ptr->flag.msg_type)
        {
            //�ȽϺ���
            while(PNULL != first_number_ptr)
            {
                if (MMISMS_TYPE_SMS == first_number_ptr->head_ptr->flag.msg_type)
                {
                    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
                    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
                    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
                    
                    if(strlen((char *)delete_state_ptr->number.sender) > 1 && (0x2B == delete_state_ptr->number.sender[0]))
                    {
                        number_type = MN_NUM_TYPE_INTERNATIONAL;
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)&delete_state_ptr->number.sender[1], 
                            pb_bcd2.number);  
                        pb_bcd2.number_len =  (strlen((char *)&delete_state_ptr->number.sender[1]) + 1)/2;    
                    }
                    else
                    {       
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)delete_state_ptr->number.sender, 
                            pb_bcd2.number);                    
                        pb_bcd2.number_len =  (strlen((char *)delete_state_ptr->number.sender) + 1)/2;    
                    }

                    pb_bcd2.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(number_type, MN_NUM_PLAN_UNKNOW);
                    
                    pb_bcd1 = MMISMS_SmsBCDToPbBCD(first_number_ptr->bcd_num);                    
                        
                    if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                    {
                        break;
                    }
                }
                else
                {                 
                    if(MMISMS_CompareMMSNum(first_number_ptr->head_ptr->number.sender,delete_state_ptr->number.sender))
                    {
                        break;
                    }
                }

                first_number_ptr = first_number_ptr->next_number_ptr;
            }
        }
        else
        {
            //�ȽϺ���
            while(PNULL != first_number_ptr)
            {
                if (MMISMS_TYPE_SMS == first_number_ptr->head_ptr->flag.msg_type)
                {
                    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
                    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
                    pb_bcd1 = MMISMS_SmsBCDToPbBCD(first_number_ptr->bcd_num);
                    pb_bcd2 = MMISMS_SmsBCDToPbBCD(delete_state_ptr->number.bcd_number);
                    if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                    {
                        break;
                    }
                }
                else
                {
                    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
                    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
               
                    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;

                    if(strlen((char *)first_number_ptr->head_ptr->number.sender) > 1 && (0x2B == first_number_ptr->head_ptr->number.sender[0]))
                    {
                        number_type = MN_NUM_TYPE_INTERNATIONAL;
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)&first_number_ptr->head_ptr->number.sender[1], 
                            pb_bcd1.number);                    
                        pb_bcd1.number_len =  (strlen((char *)&first_number_ptr->head_ptr->number.sender[1]) + 1)/2;    
                    }
                    else
                    {                                  
                        MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, 
                            (char*)first_number_ptr->head_ptr->number.sender, 
                            pb_bcd1.number);                    
                        pb_bcd1.number_len =  (strlen((char *)first_number_ptr->head_ptr->number.sender) + 1)/2;    
                    }
                    pb_bcd1.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(number_type, MN_NUM_PLAN_UNKNOW);                     
                    pb_bcd2 = MMISMS_SmsBCDToPbBCD(delete_state_ptr->number.bcd_number); 
                        
                    if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                    {
                        break;
                    }
                }

                first_number_ptr = first_number_ptr->next_number_ptr;
            }
        }
    }
    //�ҵ�����ͬ�ĺ��룬�����ں�������������Ϣ
    if(PNULL != first_number_ptr)
    {
        DeleteChatNode(first_node_ptr,first_number_ptr,delete_state_ptr,type);
        //��������s_mmisms_chat_linked_order
        SortChatLinkedOrderByTime();
        GetChatNodeTotalUnreadNum();
        //SCI_TRACE_LOW:"[jixin]:same num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9976_112_2_18_2_54_14_689,(uint8*)"");
    }
    //δ�ҵ������ش���
    else
    {
        result = FALSE;
        //SCI_TRACE_LOW:"[jixin]:diff num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_9982_112_2_18_2_54_14_690,(uint8*)"");
    }
    return result;
}
/*****************************************************************************/
//  Function Name: DeleteChatNode
//  Description  : delete chat node
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteChatNode(
                              MMISMS_CHAT_STATE_UNIT_T  *first_node_ptr,
                              MMISMS_CHAT_STATE_UNIT_T *head_node_ptr,
                              MMISMS_STATE_T *delete_state_ptr,
                              MMISMS_TYPE_E type
                             )
{
    BOOLEAN result = TRUE;
    uint32 compare_time = 0;
    MMISMS_STATE_T *cur_ptr         = PNULL;
    MMISMS_STATE_T *cur_longsms_ptr = PNULL;
    MMISMS_STATE_T *pre_longsms_ptr = PNULL;
    MMISMS_STATE_T *temp_ptr        = PNULL;
    MMISMS_STATE_T  *number_node_ptr = PNULL;

    if((PNULL == first_node_ptr) ||
       (PNULL == head_node_ptr) ||
       (PNULL == delete_state_ptr)
      )
    {
        //SCI_TRACE_LOW:"DeleteChatNode:input para is PNULL!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10012_112_2_18_2_54_14_691,(uint8*)"");
        return FALSE;
    }
    
    number_node_ptr = head_node_ptr->head_ptr;
        
    //����Ϣ�ӱ�����ڵ���ɾ��

    //��ͨ����
    if(!delete_state_ptr->flag.is_concatenate_sms)
    {
        while(PNULL != number_node_ptr)
        {
            if(IsSameSMS(delete_state_ptr,number_node_ptr) && (delete_state_ptr->time == number_node_ptr->time))
            {
                break;
            }
            
            number_node_ptr = number_node_ptr->next_chat_ptr;            
        }

        if(PNULL != number_node_ptr)
        {
            if (PNULL == number_node_ptr->prev_chat_ptr) //�������ͷ
            {
                //������ֻ����һ����Ϣ
                if(1 == head_node_ptr->total_num)
                {
                    SCI_MEMSET(head_node_ptr,0x0,sizeof(MMISMS_CHAT_STATE_UNIT_T));
                    s_mmisms_chat_linked_order.uint_num--;
                }
                //���ڶ����ڵ���Ϊ�������ͷ�ڵ㣬����ɾ���Ĳ��Ǳ������β�ڵ㣬��˲��ø��±����������ʱ��
                else
                {
                    cur_ptr = number_node_ptr->next_chat_ptr;
                    if(cur_ptr != PNULL)
                    {
                        cur_ptr->prev_chat_ptr = PNULL;
                        head_node_ptr->head_ptr = cur_ptr;
                        head_node_ptr->total_num--;
                    }
                    else
                    {
                        SCI_MEMSET(head_node_ptr,0x0,sizeof(MMISMS_CHAT_STATE_UNIT_T));
                        s_mmisms_chat_linked_order.uint_num--;
                    }

                    if((MMISMS_MT_TO_BE_READ == delete_state_ptr->flag.mo_mt_type) && (head_node_ptr->unread_num > 0))
                    {
                        head_node_ptr->unread_num--;
                    }
                }
            }
            else  //���������ͷ
            {
                //ɾ���ڵ�������β����������ʱ�����µĽڵ㣬����Ҫ���¸��±���������ʱ��
                if(PNULL == number_node_ptr->next_chat_ptr)
                {
                    //�µ�β�ڵ�cur_ptr
                    cur_ptr = number_node_ptr->prev_chat_ptr;
                    number_node_ptr->prev_chat_ptr = PNULL;
                    cur_ptr->next_chat_ptr = PNULL;
                    head_node_ptr->total_num--;    
                    if((MMISMS_MT_TO_BE_READ == delete_state_ptr->flag.mo_mt_type) && (head_node_ptr->unread_num > 0))
                    {
                        head_node_ptr->unread_num--;
                    }
                    //���µ�β�ڵ��е�ʱ����Ϊ�����������ʱ��
                    compare_time = cur_ptr->time;
                    if(cur_ptr->flag.is_concatenate_sms)
                    {
                        //�����������е�����ʱ��
                        cur_longsms_ptr = cur_ptr->next_long_sms_ptr;
                        while(PNULL != cur_longsms_ptr)
                        {
                            if(compare_time < cur_longsms_ptr->time)
                            {
                                compare_time = cur_longsms_ptr->time;
                            }
                            cur_longsms_ptr = cur_longsms_ptr->next_long_sms_ptr;
                        }
                    }
                    head_node_ptr->time = compare_time;
                }
                //ɾ�������м�ڵ㣬���ø���ʱ��
                else
                {
                    head_node_ptr->total_num--;    
                    if((MMISMS_MT_TO_BE_READ == delete_state_ptr->flag.mo_mt_type) && (head_node_ptr->unread_num > 0))
                    {
                        head_node_ptr->unread_num--;
                    }
                    number_node_ptr->prev_chat_ptr->next_chat_ptr = number_node_ptr->next_chat_ptr;                        
                    number_node_ptr->next_chat_ptr->prev_chat_ptr = number_node_ptr->prev_chat_ptr;
                }
            }
        }
        else//δ�ҵ�����Ϣ
        {
            SCI_TRACE_LOW("DeleteChatNode:fail record_id = %d,ref_num = %d,max_num = %d,seq_num = %d",delete_state_ptr->record_id,delete_state_ptr->head_ref,delete_state_ptr->longsms_max_num,delete_state_ptr->longsms_seq_num);
            result = FALSE;
        }
    }
    //������
    else
    {
        //����ref��ͬ����(��ĳ�������������ͷ)
        cur_ptr = number_node_ptr;
        
        //cur_ptr�ƶ���ĳһ��㣬ʹҪ�����ʱ�䲻С�ڵ�ǰ��ָ��ʱ�䣬����cur_ptrδָ�����һ�����
        while(PNULL != cur_ptr)
        {
            temp_ptr = cur_ptr;
            if (CompareSmsState(temp_ptr, type, delete_state_ptr))
            {
                while (PNULL != temp_ptr)
                {
                    if ((temp_ptr->longsms_seq_num == delete_state_ptr->longsms_seq_num)
                        && (temp_ptr->record_id == delete_state_ptr->record_id)
                        && (temp_ptr->flag.storage == delete_state_ptr->flag.storage))
                    {
                        break;
                    }
                    else
                    {
                        temp_ptr = temp_ptr->next_long_sms_ptr;
                    }
                } 
                if (PNULL != temp_ptr)
                {
                    break;
                }
            }
            cur_ptr = cur_ptr->next_chat_ptr;
        }

        if (PNULL != cur_ptr) //�ҵ�ref��ͬ��λ��
        {
            if (delete_state_ptr->longsms_seq_num == cur_ptr->longsms_seq_num
                && (delete_state_ptr->record_id == cur_ptr->record_id)
                && (delete_state_ptr->flag.storage == cur_ptr->flag.storage))  //����������
            {
                if (PNULL == cur_ptr->next_long_sms_ptr) //����������������
                {
                    if (PNULL == cur_ptr->prev_chat_ptr) //������ͷ
                    {
                        if(PNULL == cur_ptr->next_chat_ptr)//Ҳ������β�����Ǳ�����Ψһ�ڵ�
                        {
                            SCI_MEMSET(head_node_ptr,0x0,sizeof(MMISMS_CHAT_STATE_UNIT_T));
                            s_mmisms_chat_linked_order.uint_num--;
                        }
                        else
                        {
                            head_node_ptr->head_ptr = cur_ptr->next_chat_ptr;
                            cur_ptr->next_chat_ptr->prev_chat_ptr = PNULL;
                            cur_ptr->next_chat_ptr = PNULL;
                            head_node_ptr->total_num--;    
                            if((MMISMS_MT_TO_BE_READ == delete_state_ptr->flag.mo_mt_type) && (head_node_ptr->unread_num > 0))
                            {
                                head_node_ptr->unread_num--;
                            }
                        }
                    }
                    else //��������ͷ
                    {
                        if (PNULL == cur_ptr->next_chat_ptr) //������β�������ʱ��
                        {
                            MMISMS_STATE_T *new_tail_ptr = PNULL;
                            new_tail_ptr = cur_ptr->prev_chat_ptr;
                            cur_ptr->prev_chat_ptr->next_chat_ptr = PNULL;
                            cur_ptr->prev_chat_ptr = PNULL;
                            //���µ�β�ڵ��е�ʱ����Ϊ�����������ʱ��
                            compare_time = new_tail_ptr->time;
                            if(new_tail_ptr->flag.is_concatenate_sms)
                            {
                                //�����������е�����ʱ��
                                cur_longsms_ptr = new_tail_ptr->next_long_sms_ptr;
                                while(PNULL != cur_longsms_ptr)
                                {
                                    if(compare_time < cur_longsms_ptr->time)
                                    {
                                        compare_time = cur_longsms_ptr->time;
                                    }
                                    cur_longsms_ptr = cur_longsms_ptr->next_long_sms_ptr;
                                }
                            }
                            head_node_ptr->time = compare_time;
                            head_node_ptr->total_num--;    
                            if((MMISMS_MT_TO_BE_READ == delete_state_ptr->flag.mo_mt_type) && (head_node_ptr->unread_num > 0))
                            {
                                head_node_ptr->unread_num--;
                            }
                        }  
                        else //�м�ڵ�
                        {
                            cur_ptr->prev_chat_ptr->next_chat_ptr = cur_ptr->next_chat_ptr;
                            cur_ptr->next_chat_ptr->prev_chat_ptr = cur_ptr->prev_chat_ptr;
                            head_node_ptr->total_num--;    
                            if((MMISMS_MT_TO_BE_READ == delete_state_ptr->flag.mo_mt_type) && (head_node_ptr->unread_num > 0))
                            {
                                head_node_ptr->unread_num--;
                            }
                        }
                    }
                }
                else //��������������
                {
                    if (PNULL == cur_ptr->prev_chat_ptr) //������ͷ���������ŵ���һ����Ϊ����ͷ
                    {
                        if(PNULL == cur_ptr->next_chat_ptr)//Ҳ������β��ȡ���³����ŵ�����ʱ����Ϊ�����������ʱ��
                        {
                            cur_longsms_ptr = cur_ptr->next_long_sms_ptr;
                            while(PNULL != cur_longsms_ptr)
                            {
                                if(compare_time < cur_longsms_ptr->time)
                                {
                                    compare_time = cur_longsms_ptr->time;
                                }
                                cur_longsms_ptr = cur_longsms_ptr->next_long_sms_ptr;
                            }
                            head_node_ptr->time = compare_time;
                        }

                        cur_ptr->next_long_sms_ptr->next_chat_ptr = cur_ptr->next_chat_ptr;
                        cur_ptr->next_long_sms_ptr->prev_chat_ptr = cur_ptr->prev_chat_ptr;

                        if(cur_ptr->next_chat_ptr != PNULL)
                        {
                            cur_ptr->next_chat_ptr->prev_chat_ptr = cur_ptr->next_long_sms_ptr;
                        }

                        head_node_ptr->head_ptr = cur_ptr->next_long_sms_ptr;
                        cur_ptr->prev_chat_ptr = PNULL;
                        cur_ptr->next_chat_ptr = PNULL;
                    }
                    else//��������ͷ
                    {
                        if (PNULL == cur_ptr->next_chat_ptr) //������β������ʱ��
                        {
                            MMISMS_STATE_T *new_tail_longsms_ptr = PNULL;
                            new_tail_longsms_ptr = cur_ptr->next_long_sms_ptr;
                            new_tail_longsms_ptr->prev_chat_ptr = cur_ptr->prev_chat_ptr;
                            new_tail_longsms_ptr->next_chat_ptr = cur_ptr->next_chat_ptr;
                            cur_ptr->prev_chat_ptr->next_chat_ptr = new_tail_longsms_ptr;
                            cur_ptr->prev_chat_ptr = PNULL;
                            cur_ptr->next_chat_ptr = PNULL;
                            //���µ�β�ڵ��е�ʱ����Ϊ�����������ʱ��
                            if(new_tail_longsms_ptr->flag.is_concatenate_sms)
                            {
                                //�����������е�����ʱ��
                                compare_time = new_tail_longsms_ptr->time;
                                cur_longsms_ptr = new_tail_longsms_ptr->next_long_sms_ptr;
                                while(PNULL != cur_longsms_ptr)
                                {
                                    if(compare_time < cur_longsms_ptr->time)
                                    {
                                        compare_time = cur_longsms_ptr->time;
                                    }
                                    cur_longsms_ptr = cur_longsms_ptr->next_long_sms_ptr;
                                }
                            }
                            head_node_ptr->time = compare_time;
                        }
                        else//�м�ڵ�
                        {
                            cur_ptr->next_long_sms_ptr->next_chat_ptr = cur_ptr->next_chat_ptr;
                            cur_ptr->next_long_sms_ptr->prev_chat_ptr = cur_ptr->prev_chat_ptr;
                            cur_ptr->prev_chat_ptr->next_chat_ptr = cur_ptr->next_long_sms_ptr;
                            cur_ptr->next_chat_ptr->prev_chat_ptr = cur_ptr->next_long_sms_ptr;
                            cur_ptr->next_chat_ptr = PNULL;
                            cur_ptr->prev_chat_ptr = PNULL;
                        }
                    }
                }
            }
            else //������������,�����²���
            {
                cur_longsms_ptr = cur_ptr;

                //��ɾ������Ϣ��ʱ���Ǳ����������ʱ�䣬��ɾ���󱾺��������ʱ��Ϊ��ǰ�б�Ĵ���ʱ��
                if(head_node_ptr->time == delete_state_ptr->time)
                {
                    while ((PNULL != cur_longsms_ptr->next_long_sms_ptr)
                        &&(!((delete_state_ptr->longsms_seq_num == cur_longsms_ptr->longsms_seq_num)
                        && (delete_state_ptr->record_id == cur_longsms_ptr->record_id)
                        && (delete_state_ptr->flag.storage == cur_longsms_ptr->flag.storage) )) )
                    {
                        if(compare_time < cur_longsms_ptr->time)
                        {
                            compare_time = cur_longsms_ptr->time;//���ɾ���ڵ�ǰ�����ʱ��
                        }
                        pre_longsms_ptr = cur_longsms_ptr;
                        cur_longsms_ptr = cur_longsms_ptr->next_long_sms_ptr;
                    }
                    if (cur_longsms_ptr->longsms_seq_num == delete_state_ptr->longsms_seq_num
                        && (delete_state_ptr->record_id == cur_longsms_ptr->record_id)
                        && (delete_state_ptr->flag.storage == cur_longsms_ptr->flag.storage)) //find
                    {
                        //�������������ʣ����Ϣ������time
                        temp_ptr = cur_ptr->next_long_sms_ptr;
                        //��ɾ���ڵ����һ���ڵ㿪ʼ���ң������������ʱ��
                        while(PNULL != temp_ptr)
                        {
                            if(compare_time < temp_ptr->time)
                            {
                                compare_time = temp_ptr->time;
                            }
                            temp_ptr = temp_ptr->next_long_sms_ptr;
                        }

                        //��compare_time�ͱ�����ڵ��������Ϣtime���бȽϣ���ȡ����ʱ��
                        while(PNULL != cur_ptr)
                        {
                            uint32 temp_time = 0;
                            //��cur_ptr�������
                            cur_ptr = cur_ptr->next_chat_ptr;
                            
                            if(PNULL != cur_ptr)
                            {
                                temp_time = cur_ptr->time;
                                if(cur_ptr->flag.is_concatenate_sms)//������
                                {
                                    cur_longsms_ptr = cur_ptr;
                                    while (PNULL != cur_longsms_ptr)
                                    {
                                        if(cur_longsms_ptr->time > compare_time)
                                        {
                                            compare_time = cur_longsms_ptr->time;
                                        }
                                        cur_longsms_ptr = cur_longsms_ptr->next_long_sms_ptr;
                                    }
                                }
                                else//��ͨ����
                                {
                                    if(temp_time > compare_time)
                                    {
                                        compare_time = temp_time;
                                    }                                    
                                }
                                cur_ptr = cur_ptr->next_chat_ptr;
                            }
                        }
                        //���º���ڵ�time
                        head_node_ptr->time = compare_time;
                    }
                    else 
                    {
                        //SCI_TRACE_LOW:"MMISMS: DeleteChatNode :can't find the sms ptr in long sms link"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10347_112_2_18_2_54_15_692,(uint8*)"");
                        SCI_TRACE_LOW("DeleteChatNode:fail record_id = %d,ref_num = %d,max_num = %d,seq_num = %d",delete_state_ptr->record_id,delete_state_ptr->head_ref,delete_state_ptr->longsms_max_num,delete_state_ptr->longsms_seq_num);
                        return FALSE;
                    }                    
                }
                //���ø���ʱ��
                else
                {
                    //do nothing
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMISMS DeleteChatNode: there is no sms have the same ref with the to delete sms1_state_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10360_112_2_18_2_54_15_693,(uint8*)"");
            SCI_TRACE_LOW("DeleteChatNode:fail record_id = %d,ref_num = %d,max_num = %d,seq_num = %d",delete_state_ptr->record_id,delete_state_ptr->head_ref,delete_state_ptr->longsms_max_num,delete_state_ptr->longsms_seq_num);
            return FALSE;
        }            
    }

    //SCI_TRACE_LOW:"MMISMS: DeleteChatNode total_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10365_112_2_18_2_54_15_694,(uint8*)"d",s_mmisms_chat_linked_order.uint_num);

    return result;
}

/*****************************************************************************/
//  Function Name: IsSameSMS
//  Description  : compare two sms in order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSameSMS(
                        MMISMS_STATE_T *sms1_state_ptr,
                        MMISMS_STATE_T *sms2_state_ptr
                       )
{
    return IsSameSMS(sms1_state_ptr,sms2_state_ptr);
}
/*****************************************************************************/
//  Function Name: SortChatLinkedOrderByTime
//  Description  : adjust s_mmisms_chat_linked_order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SortChatLinkedOrderByTime(void)
{
    uint32 i = 0;
    MMISMS_CHAT_STATE_UNIT_T *number_node = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *head_node = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *cur_node = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *head_unread_node = PNULL;//δ������ͷָ��
    MMISMS_CHAT_STATE_UNIT_T *head_read_node = PNULL;//�Ѷ�����ͷָ��

    if(0 == s_mmisms_linked_order.unit_num)
    {
        return;
    }

    s_mmisms_chat_linked_order.head_ptr = PNULL;
    head_node = s_mmisms_chat_linked_order.head_node_ptr;

    //�������:δ����Ϣ�ĺ�������ǰ�棬�Ѷ���Ϣ�ĺ����ź��棬���Ծ���ʱ���ɴ�С����
    //δ�����Ѷ��ֱ���һ��ͷ�ڵ㣬�²������״̬ѡ�����ĳһ����
    //ȫ����ɺ󣬽�δ����β�ڵ���Ѷ���ͷ�ڵ�������
    for(i=0;i<s_mmisms_linked_order.unit_num;i++)
    {
        if(head_node[i].is_valid)
        {
            number_node = &head_node[i];

            //������������б�

            if((PNULL == head_unread_node) &&
               (0 != number_node->unread_num))
            {
                number_node->prev_number_ptr = PNULL;
                number_node->next_number_ptr = PNULL;
                head_unread_node = number_node;
            }
            else if((PNULL == head_read_node) &&
                    (0 == number_node->unread_num))
            {
                number_node->prev_number_ptr = PNULL;
                number_node->next_number_ptr = PNULL;
                head_read_node = number_node;
            }
            else
            {
                if(0 != number_node->unread_num)
                {
                    cur_node = head_unread_node;
                }
                else
                {
                    cur_node = head_read_node;                    
                }

                if(PNULL == cur_node)
                {
                    return;
                }
                
                //�嵽����ͷ
                if(number_node->time > cur_node->time)
                {
                    number_node->prev_number_ptr = PNULL;
                    cur_node->prev_number_ptr = number_node;
                    number_node->next_number_ptr = cur_node;
                    if(0 != number_node->unread_num)
                    {
                        head_unread_node = number_node;
                    }
                    else
                    {
                        head_read_node = number_node;                    
                    }                    
                }
                else
                {
                    MMISMS_CHAT_STATE_UNIT_T *tail_node = PNULL;

                    while(PNULL != cur_node)
                    {
                        if(number_node->time > cur_node->time)
                        {
                            break;
                        }
                        if(PNULL == cur_node->next_number_ptr)
                        {
                            tail_node = cur_node;
                        }
                        cur_node = cur_node->next_number_ptr;
                    }
                    //ʱ����С���嵽����β
                    if(PNULL == cur_node)
                    {
                        number_node->prev_number_ptr = tail_node;
                        number_node->next_number_ptr = PNULL;
                        if(PNULL != tail_node)
                        {
                            tail_node->next_number_ptr = number_node;
                        }
                    }
                    //�м�ڵ�
                    else
                    {
                        number_node->prev_number_ptr = cur_node->prev_number_ptr;
                        number_node->next_number_ptr = cur_node->prev_number_ptr->next_number_ptr;
                        cur_node->prev_number_ptr->next_number_ptr = number_node;
                        cur_node->prev_number_ptr = number_node;
                    }
                }
            }
        }
    }

    //�ϲ�head_unread_node��head_read_node��s_mmisms_chat_linked_order.head_ptr
    if(PNULL == head_unread_node)
    {
        if(PNULL != head_read_node)
        {
            s_mmisms_chat_linked_order.head_ptr = head_read_node;
        }
    }
    else
    {
        s_mmisms_chat_linked_order.head_ptr = head_unread_node;
        if(PNULL != head_read_node)
        {
            MMISMS_CHAT_STATE_UNIT_T *tail_node = PNULL;

            cur_node = head_unread_node;
            
            while(PNULL != cur_node)
            {
                if(PNULL == cur_node->next_number_ptr)
                {
                    tail_node = cur_node;
                }
                cur_node = cur_node->next_number_ptr;
            }      
            if(PNULL != tail_node)
            {
                tail_node->next_number_ptr = head_read_node;
                head_read_node->prev_number_ptr = tail_node;
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Function Name: SortChatLinkedOrderByName
//  Description  : adjust s_mmisms_chat_linked_order
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SortChatLinkedOrderByName(void)
{
    uint32 i = 0;
    int32 ret_val = 0;
    MMISMS_CHAT_STATE_UNIT_T *number_node = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *head_node = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *cur_node = PNULL;

    if(0 == s_mmisms_linked_order.unit_num)
    {
        return;
    }

    s_mmisms_chat_linked_order.head_ptr = PNULL;
    head_node = s_mmisms_chat_linked_order.head_node_ptr;

    for(i=0;i<s_mmisms_linked_order.unit_num;i++)
    {
        if(head_node[i].is_valid)
        {
            number_node = &head_node[i];

            //������������б�

            cur_node = s_mmisms_chat_linked_order.head_ptr;
            if(PNULL == cur_node)
            {
                number_node->prev_number_ptr = PNULL;
                number_node->next_number_ptr = PNULL;
                s_mmisms_chat_linked_order.head_ptr = number_node;
            }
            else
            {
                //�嵽����ͷ
                ret_val = CompareStateForSort(
                                              MMISMS_ORDER_BY_NAME,
                                              number_node->head_ptr,
                                              cur_node->head_ptr
                                             );
                
                if(-1 == ret_val)
                {
                    number_node->prev_number_ptr = PNULL;
                    cur_node->prev_number_ptr = number_node;
                    number_node->next_number_ptr = cur_node;
                    s_mmisms_chat_linked_order.head_ptr = number_node;
                }
                else
                {
                    MMISMS_CHAT_STATE_UNIT_T *tail_node = PNULL;

                    while(PNULL != cur_node)
                    {
                        ret_val = CompareStateForSort(
                                                      MMISMS_ORDER_BY_NAME,
                                                      number_node->head_ptr,
                                                      cur_node->head_ptr
                                                     );
                        
                        if(-1 == ret_val)
                        {
                            break;
                        }
                        if(PNULL == cur_node->next_number_ptr)
                        {
                            tail_node = cur_node;
                        }
                        cur_node = cur_node->next_number_ptr;
                    }
                    //ʱ����С���嵽����β
                    if(PNULL == cur_node)
                    {
                        number_node->prev_number_ptr = tail_node;
                        number_node->next_number_ptr = PNULL;
                        if(PNULL != tail_node)
                        {
                            tail_node->next_number_ptr = number_node;
                        }
                    }
                    //�м�ڵ�
                    else
                    {
                        number_node->prev_number_ptr = cur_node->prev_number_ptr;
                        number_node->next_number_ptr = cur_node->prev_number_ptr->next_number_ptr;
                        cur_node->prev_number_ptr->next_number_ptr = number_node;
                        cur_node->prev_number_ptr = number_node;
                    }
                }
            }
        }
    }

    return;
}
/*****************************************************************************/
//  Function Name: GetChatNodeTotalUnreadNum
//  Description  : Get ChatNode Total Unread Num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetChatNodeTotalUnreadNum(void)
{
    uint32 unread_num = 0;
    MMISMS_CHAT_STATE_UNIT_T *number_node = PNULL;

    number_node = s_mmisms_chat_linked_order.head_ptr;

    while(PNULL != number_node)
    {
        if(0 != number_node->unread_num)
        {
            unread_num++;
        }
        number_node = number_node->next_number_ptr;
    }

    s_mmisms_chat_linked_order.unread_unit_num = unread_num;

    //SCI_TRACE_LOW:"GetChatNodeTotalUnreadNum:unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10657_112_2_18_2_54_15_695,(uint8*)"d",unread_num);

    return unread_num;
}

/*****************************************************************************/
//  Function Name: MMISMS_GetChatNodeTotalUnreadNum
//  Description  : Get ChatNode Total Unread Num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetChatNodeTotalUnreadNum(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetChatNodeTotalUnreadNum:unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10669_112_2_18_2_54_15_696,(uint8*)"d",s_mmisms_chat_linked_order.unread_unit_num);
    return s_mmisms_chat_linked_order.unread_unit_num;
}


/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurChatMsgMarked(
                                       MMIPB_BCD_NUMBER_T pb_num,
                                       BOOLEAN is_marked //IN:
                                       )
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_long_order_id = PNULL;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};
    
    //SCI_TRACE_LOW:"MMISMS_SetChatMsgMarkedInList: index = %d,is_marked = %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10686_112_2_18_2_54_15_697,(uint8*)"dd",index,is_marked);
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while(PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }  
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return;
    }

    cur_order_id = head_chat_id->head_ptr;  

    while (PNULL != cur_order_id)
    {
        cur_order_id->flag.is_marked = is_marked;
        
        cur_long_order_id = cur_order_id->next_long_sms_ptr;
            
        while (PNULL != cur_long_order_id)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }        
        cur_order_id = cur_order_id->next_chat_ptr;
    }
}


/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetChatMsgMarkedInList(
                                       uint16 index,
                                       BOOLEAN is_marked //IN:
                                       )
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_long_order_id = PNULL;

    //SCI_TRACE_LOW:"MMISMS_SetChatMsgMarkedInList: index = %d,is_marked = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10686_112_2_18_2_54_15_697,(uint8*)"dd",index,is_marked);
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while ((PNULL != head_chat_id) && (index > 0))
    {
        head_chat_id = head_chat_id->next_number_ptr;
        index --;
    }     

    if(PNULL == head_chat_id)
    {
        return;
    }

    cur_order_id = head_chat_id->head_ptr;  

    while (PNULL != cur_order_id)
    {
        cur_order_id->flag.is_marked = is_marked;
        
        cur_long_order_id = cur_order_id->next_long_sms_ptr;
            
        while (PNULL != cur_long_order_id)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }        
        cur_order_id = cur_order_id->next_chat_ptr;
    }
}

/*****************************************************************************/
//  Description : Get Cur Chat SIMId
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetCurChatSIMId(MMIPB_BCD_NUMBER_T pb_num)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};
    MN_DUAL_SYS_E cur_sim_id = MMI_DUAL_SYS_MAX;
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }  
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return MMI_DUAL_SYS_MAX;
    }

    cur_order_id = head_chat_id->head_ptr;  

    if(MMISMS_CHAT_MAX_ITEM < head_chat_id->total_num)
    {
        uint32 begin_index = head_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
        
        while((PNULL != cur_order_id) && (0 < begin_index))
        {
            cur_order_id = cur_order_id->next_chat_ptr;
            begin_index--;
        }
    }
        
    while (PNULL != cur_order_id)
    {
        if (MMISMS_MO_SEND_SUCC != cur_order_id->flag.mo_mt_type
            && MMISMS_MO_SEND_FAIL != cur_order_id->flag.mo_mt_type
            && MMISMS_MO_DRAFT != cur_order_id->flag.mo_mt_type)
        {
            if (MMI_DUAL_SYS_MAX != cur_sim_id
                && cur_sim_id != (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys)
            {
                cur_sim_id = MMI_DUAL_SYS_MAX;

                break;
            }

            cur_sim_id = (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys;
        }
        
        cur_order_id = cur_order_id->next_chat_ptr;
    }

    return cur_sim_id;
}

/*****************************************************************************/
//  Description : get cur chat avail delete num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetChatAvailDeleteNum(MMIPB_BCD_NUMBER_T pb_num)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};
    uint16    delete_sms_num = 0;
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }  
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return 0;
    }

    cur_order_id = head_chat_id->head_ptr;  

    if(MMISMS_CHAT_MAX_ITEM < head_chat_id->total_num)
    {
        uint32 begin_index = head_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
        
        while((PNULL != cur_order_id) && (0 < begin_index))
        {
            cur_order_id = cur_order_id->next_chat_ptr;
            begin_index--;
        }
    }
        
    while (PNULL != cur_order_id && delete_sms_num <= MMISMS_CHAT_MAX_ITEM)
    {  
        if ( (!cur_order_id->flag.is_locked) && (!MMIAPIMMS_IsProcessMMSById(cur_order_id->record_id)))
        {
            if (MMIAPIUDISK_UdiskIsRun() && 
               (MMISMS_TYPE_MMS == cur_order_id->flag.msg_type
                || MMISMS_TYPE_WAP_PUSH == cur_order_id->flag.msg_type
                || MMISMS_TYPE_MMS_OTA == cur_order_id->flag.msg_type
                || MMISMS_TYPE_WAP_OTA == cur_order_id->flag.msg_type))
            { 
                //SCI_TRACE_LOW("GetOrderLinkDeleteAvailNum udisk is running!");
            }
            else
            {
                delete_sms_num++;
            }
        }
        cur_order_id = cur_order_id->next_chat_ptr;        
    }
    return delete_sms_num;
}

/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetLastChatMsgMarked(
                                        MMIPB_BCD_NUMBER_T pb_num,
                                        BOOLEAN is_marked //IN:
                                       )
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_long_order_id = PNULL;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0};

    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != head_chat_id)
    {
        cmp_bcd = MMISMS_SmsBCDToPbBCD(head_chat_id->bcd_num); 
        if(CompareSMSNum(&pb_num,&cmp_bcd))
        {
            break;
        }  
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    if(PNULL == head_chat_id)
    {
        return;
    }

    cur_order_id = head_chat_id->head_ptr;  

    if(MMISMS_CHAT_MAX_ITEM < head_chat_id->total_num)
    {
        uint32 begin_index = head_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
        
        while((PNULL != cur_order_id) && (0 < begin_index))
        {
            cur_order_id = cur_order_id->next_chat_ptr;
            begin_index--;
        }
    }
        
    while (PNULL != cur_order_id)
    {
        cur_order_id->flag.is_marked = is_marked;
        
        cur_long_order_id = cur_order_id->next_long_sms_ptr;
            
        while (PNULL != cur_long_order_id)
        {
            cur_long_order_id->flag.is_marked = is_marked;
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }        
        cur_order_id = cur_order_id->next_chat_ptr;
    }
}

/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_CountChatItemOrderNum(
                                        uint16 index,
                                        MMISMS_ORDER_ID_T order_id
                                       )
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_long_order_id = PNULL;
    uint16 count = 0;
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    while ((PNULL != head_chat_id) && (index > 0))
    {
        head_chat_id = head_chat_id->next_number_ptr;
        index --;
    }     

    if(PNULL == head_chat_id || PNULL == order_id)
    {
        return count;
    }

    cur_order_id = head_chat_id->head_ptr;  
        
    while (PNULL != cur_order_id)
    {
        if(IsSameLongSmsPart(order_id,cur_order_id))
        {
            cur_long_order_id = cur_order_id;			
            do
            {
                count++;
                cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
            }    
			while (PNULL != cur_long_order_id);
            break;
        }
        
        cur_order_id = cur_order_id->next_chat_ptr;
    }
	SCI_TRACE_LOW("mmisms order info %d",count);
    return count;
}

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related Chat SMS box
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidChatSMSForDelAll(void)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *num_ptr = PNULL;
    
    num_ptr = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != num_ptr)
    {
        order_id = num_ptr->head_ptr;
        
        while(PNULL != order_id)
        {
            if(order_id->flag.is_marked && (!order_id->flag.is_locked) ) //is  marked
            {
                return order_id;
            }
            else
            {
                order_id = order_id->next_chat_ptr;
            }
        }

        num_ptr = num_ptr->next_number_ptr;
    }    
    
    return order_id;
}

/*****************************************************************************/
//  Description : get sms chat order link delete available num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetChatOrderLinkDeleteAvailNum(void)
{
    uint32 delete_sms_num = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *num_ptr = PNULL;
    
    num_ptr = s_mmisms_chat_linked_order.head_ptr;

    while (PNULL != num_ptr)
    {
        order_id = num_ptr->head_ptr;
        
        while(PNULL != order_id)
        {
            if (order_id->flag.is_marked && (!order_id->flag.is_locked) && (!MMIAPIMMS_IsProcessMMSById(order_id->record_id)))
            {
                if (MMIAPIUDISK_UdiskIsRun() && 
                   (MMISMS_TYPE_MMS == order_id->flag.msg_type
                    || MMISMS_TYPE_WAP_PUSH == order_id->flag.msg_type
                    || MMISMS_TYPE_MMS_OTA == order_id->flag.msg_type
                    || MMISMS_TYPE_WAP_OTA == order_id->flag.msg_type))
                { 
                    //SCI_TRACE_LOW("GetOrderLinkDeleteAvailNum udisk is running!");
                }
                else
                {
                    delete_sms_num++;
                }
            }
            order_id = order_id->next_chat_ptr;
        }

        num_ptr = num_ptr->next_number_ptr;
    } 

    //SCI_TRACE_LOW:"GetChatOrderLinkDeleteAvailNum: delete_sms_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10847_112_2_18_2_54_16_699,(uint8*)"d",delete_sms_num);
    return delete_sms_num;
}

/*****************************************************************************/
//  Description : get sms chat order link delete available num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_CurChatMsgUnlockedNum(MMISMS_ORDER_ID_T cur_order_id)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    uint32 delete_num = 0;

    if(PNULL == cur_order_id)
    {
        return delete_num;
    }

    order_id = cur_order_id;
    
    while(PNULL != order_id)
    {
        if (!order_id->flag.is_locked)
        {
            delete_num++;
        }
        if(order_id == order_id->prev_chat_ptr)
        {
            SCI_TRACE_LOW("MMISMS_CurChatMsgUnlockedNum err");
            break;
        }
        order_id = order_id->prev_chat_ptr;
    }

    //SCI_TRACE_LOW:"MMISMS_CurChatMsgUnlockedNum: delete_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10876_112_2_18_2_54_16_700,(uint8*)"d",delete_num);

    return delete_num;
}

/*****************************************************************************/
//  Description : get number node from chat order by index
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetNumNodeByIndexInList(uint16 index)
{
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    //��ȡ������ĵ�һ����Ϣ

    //SCI_TRACE_LOW:"head_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10894_112_2_18_2_54_16_701,(uint8*)"d",head_chat_id);
    
    while ((PNULL != head_chat_id) && (index > 0))
    {
        head_chat_id = head_chat_id->next_number_ptr;
        index --;
    }     
    if(PNULL != head_chat_id)
    {
        current_order_id = head_chat_id->head_ptr;
        //������Ϣ���������������Ƹ�����ֻ��ȡ����ļ�¼
        if(MMISMS_CHAT_MAX_ITEM < head_chat_id->total_num)
        {
            uint32 begin_index = head_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
            
            while((PNULL != current_order_id) && (0 < begin_index))
            {
                current_order_id = current_order_id->next_chat_ptr;
                begin_index--;
            }
        }
    }

    return current_order_id;
}

/*****************************************************************************/
//  Description : set chat order num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetChatOrderUnitNum(void)
{
    MMISMS_STATE_T *current_order_id = PNULL;
	MMISMS_STATE_T *temp_current_order_id = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    uint32 unit_num = 0;
    uint32 unit_unread_num = 0;
    uint32 while_count = 0;
    uint32 total_msg_num = s_mmisms_order.max_sms_num.total_msg_num;   //fixed bug147321 ��ֹ��ѭ��
        
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    //��ȡ������ĵ�һ����Ϣ

    //SCI_TRACE_LOW:"head_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_10935_112_2_18_2_54_16_702,(uint8*)"d",head_chat_id);
    
    while (PNULL != head_chat_id)
    {
        uint32 num_num = 0;
        uint32 num_unread_num = 0;

        unit_num++;
        current_order_id = head_chat_id->head_ptr;
		temp_current_order_id = current_order_id;   //�����ж��Ƿ��л�����

        while(PNULL != current_order_id)
        {
            num_num++;
            while_count++;
            if(MMISMS_MT_TO_BE_READ == current_order_id->flag.mo_mt_type)
            {
                num_unread_num++;
            }


            current_order_id = current_order_id->next_chat_ptr;

			if(current_order_id != PNULL && temp_current_order_id == current_order_id->next_chat_ptr)
			{
				SCI_TRACE_LOW("MMISMS link array error SetChatOrderUnitNum");
				break;
            }  

            if (while_count > total_msg_num)
            {
                SCI_TRACE_LOW("MMISMS link array error while_count exceed total_msg_num SetChatOrderUnitNum");
				break;
            }                    
        }

        if(0 < num_unread_num)
        {
            unit_unread_num++;
        }
        head_chat_id->total_num = num_num;
        head_chat_id->unread_num = num_unread_num;
        
        head_chat_id = head_chat_id->next_number_ptr;
    }     

    s_mmisms_chat_linked_order.uint_num = unit_num;
    s_mmisms_chat_linked_order.unread_unit_num = unit_unread_num;

    return;
}

/*****************************************************************************/
//  Description : find whether include unread sms
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindUnreadSMSInChatUnit(MMISMS_CHAT_STATE_UNIT_T *cur_chat_id)
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_long_order_id = PNULL;
    
    if(PNULL == cur_chat_id)
    {
        return FALSE;
    }
    cur_order_id = cur_chat_id->head_ptr;

    if((PNULL == cur_order_id))
    {
        return FALSE;
    }
    
    if(MMISMS_CHAT_MAX_ITEM < cur_chat_id->total_num)
    {
        uint32 begin_index = cur_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
        
        while((PNULL != cur_order_id) && (0 < begin_index))
        {
            cur_order_id = cur_order_id->next_chat_ptr;
            begin_index--;
        }
    }
        
    while (PNULL != cur_order_id)
    {
        if(MMISMS_MT_TO_BE_READ == cur_order_id->flag.mo_mt_type)
        {
            return TRUE;
        }

        cur_long_order_id = cur_order_id->next_long_sms_ptr;
            
        while (PNULL != cur_long_order_id)
        {
            if(MMISMS_MT_TO_BE_READ == cur_order_id->flag.mo_mt_type)
            {
                return TRUE;
            }
            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }        
        cur_order_id = cur_order_id->next_chat_ptr;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : find whether include Chat sms Staus
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMI_SMS_CHAT_STATUS_T MMISMS_FindChatSMSNumStatusInList(uint16 index)
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_long_order_id = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMI_SMS_CHAT_STATUS_T chat_status = {0};
//    MMIPB_BCD_NUMBER_T cmp_bcd = {0};

    SCI_TRACE_LOW("mmisms MMISMS_FindChatSMSNumStatusInList index %d", index);    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;

    //��ȡ������ĵ�һ����Ϣ    
    while ((PNULL != head_chat_id) && (index > 0))
    {
        head_chat_id = head_chat_id->next_number_ptr;

        index --;
    }  

    if((PNULL == head_chat_id))
    {
        return chat_status;
    }
    
    cur_order_id = head_chat_id->head_ptr;

    if((PNULL == cur_order_id))
    {
        return chat_status;
    }

    if(MMISMS_CHAT_MAX_ITEM < head_chat_id->total_num)
    {
        uint32 begin_index = head_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
        
        while((PNULL != cur_order_id) && (0 < begin_index))
        {
            cur_order_id = cur_order_id->next_chat_ptr;
            begin_index--;
        }
    }
        
    while (PNULL != cur_order_id)
    {
        if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
        {
            if (MMISMS_MT_TO_BE_READ == cur_order_id->flag.mo_mt_type)
            {
                chat_status.is_unread_sms = TRUE;
            }

            chat_status.is_exist_sms = TRUE;
        }
        else if (MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)
        {
            if (MMISMS_MT_TO_BE_READ == cur_order_id->flag.mo_mt_type)
            {
                chat_status.is_unread_mms = TRUE;
            }

            chat_status.is_exist_mms = TRUE;
        }

        cur_long_order_id = cur_order_id->next_long_sms_ptr;
            
        while (PNULL != cur_long_order_id)
        {
            if (MMISMS_TYPE_SMS == cur_long_order_id->flag.msg_type)
            {
                if (MMISMS_MT_TO_BE_READ == cur_long_order_id->flag.mo_mt_type)
                {
                    chat_status.is_unread_sms = TRUE;
                }

                chat_status.is_exist_sms = TRUE;
            }
            else if (MMISMS_TYPE_MMS == cur_long_order_id->flag.msg_type)
            {
                if (MMISMS_MT_TO_BE_READ == cur_long_order_id->flag.mo_mt_type)
                {
                    chat_status.is_unread_mms = TRUE;
                }

                chat_status.is_exist_mms = TRUE;
            }

            cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
        }

        cur_order_id = cur_order_id->next_chat_ptr;
    }

    return chat_status;
}

/*****************************************************************************/
//  Description : �Ƚ�������Ϣ�Ƿ���ͬһ�������ŵĲ�ͬ����
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CompareSmsState(                                           //RETURN: 
                                      MMISMS_STATE_T        *sms1_state_ptr,   //IN: SMS1 state 
                                      MMISMS_TYPE_E         type,               //IN: MO or MT
                                      MMISMS_STATE_T        *sms2_state_ptr    //IN: SMS2 state
                                     )
{
    BOOLEAN result = FALSE;
    
    result = CompareSmsState(
                             sms1_state_ptr,
                             type,
                             sms2_state_ptr
                            );

    return result;
}

/*****************************************************************************/
//  Description : ��ȡ��Ϣtype(MO,MT)
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_TYPE_E MMISMS_GetMoMtType(MMISMS_ORDER_ID_T cur_order_id)
{
    MMISMS_TYPE_E type = MMISMS_NO_TYPE;
    MMISMS_MO_MT_TYPE_E mo_mt_type = cur_order_id->flag.mo_mt_type;
    
    if((MMISMS_MT_NOT_DOWNLOAD == mo_mt_type)
       || (MMISMS_MT_TO_BE_READ == mo_mt_type)
       || (MMISMS_MT_HAVE_READ == mo_mt_type)
       || (MMISMS_MT_SR_TO_BE_READ == mo_mt_type)
       || (MMISMS_MT_SR_HAVE_READ == mo_mt_type)
       || (MMISMS_MT_NOT_NOTIFY == mo_mt_type)
       || (MMISMS_MT_NEED_NOT_DOWNLOAD == mo_mt_type)
      )
    {
        type = MMISMS_MT_TYPE;
    }
    else
    {
        type = MMISMS_MO_TYPE;
    }

    return type;
}

#if 0
/*****************************************************************************/
//  Description : get unread num == 1 chat node index
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetChatUnreadIndex(void)
{
    MMISMS_CHAT_STATE_UNIT_T *num_node = s_mmisms_chat_linked_order.head_ptr;
    uint16 index = 0;
    while(PNULL != num_node)
    {
        if(1 == num_node->unread_num)
        {
            return index;
        }
        index++;
        num_node = num_node->next_number_ptr;
    }
    return 0;    
}
#endif

/*****************************************************************************/
//  Description : get chat order id(only one unread sms)
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetChatUnreadOrderId(void)
{
    MMISMS_CHAT_STATE_UNIT_T *num_node = s_mmisms_chat_linked_order.head_ptr;
    MMISMS_ORDER_ID_T return_order_id = PNULL;

    while(PNULL != num_node)
    {
        if(1 == num_node->unread_num)
        {
            return_order_id = num_node->head_ptr;
            break;
        }
        num_node = num_node->next_number_ptr;
    }

    if((PNULL != num_node)
        && (MMISMS_CHAT_MAX_ITEM < num_node->total_num)
        )
    {
        uint32 begin_index = num_node->total_num - MMISMS_CHAT_MAX_ITEM;
        
        while((PNULL != return_order_id) && (0 < begin_index))
        {
            return_order_id = return_order_id->next_chat_ptr;
            begin_index--;
        }
    }
    if(PNULL == return_order_id)
    {
        //SCI_TRACE_LOW:"mmisms : return_order_id is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11194_112_2_18_2_54_17_704,(uint8*)"");
        if(s_mmisms_chat_linked_order.head_ptr != PNULL)
        {
            return_order_id = s_mmisms_chat_linked_order.head_ptr->head_ptr;
        }
    }
    return return_order_id;
}

/*****************************************************************************/
//  Description :set exceed unread need update(max 50)
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetExceedUnreadNeedUpdate(void)
{
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    uint16 index = 0;
    //����pos_index��õ�ǰ��order_id 
    uint32 begin_index = 0;
    MMISMS_ORDER_ID_T temp_order_id = PNULL;
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;    
    head_chat_id = s_mmisms_chat_linked_order.head_ptr;
    index = GUILIST_GetCurItemIndex(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
    //��ȡ������ĵ�һ����Ϣ    
    while ((PNULL != head_chat_id) && (index > 0))   //����index��õ�ǰ�������ĵ�һ���ڵ�
    {
        head_chat_id = head_chat_id->next_number_ptr;
        index --;
    }  
    if(PNULL == head_chat_id || head_chat_id->total_num <= MMISMS_CHAT_MAX_ITEM )
    {
        return;
    }
    
    begin_index = head_chat_id->total_num - MMISMS_CHAT_MAX_ITEM;
    current_order_id = head_chat_id->head_ptr;
    //������Ϣ���������������Ƹ�����ֻ��ȡ����ļ�¼
    //��õ�ǰ��������ʵ����ʾ�ĵ�һ���ڵ�
    
    while((PNULL != current_order_id) && (0 < begin_index))
    {
        current_order_id = current_order_id->next_chat_ptr;
        begin_index--;
    }

    temp_order_id = head_chat_id->head_ptr;
    while(temp_order_id)
    {
       if(temp_order_id == current_order_id)
       {
           if(MMISMS_UPDATE_EXCEED_CHAT_SMS == MMISMS_GetCurrentOperStatus())
		   {
               g_mmisms_global.update_oper_order_id = PNULL;
               MMISMS_ClearOperInfo();
		   }
           MMK_CloseWin(MMISMS_WAITING_WIN_ID);
           return;
       }
       else
       {
           MMISMS_ORDER_ID_T cur_longsms_order_id = PNULL;
           cur_longsms_order_id = temp_order_id;
           while (cur_longsms_order_id != PNULL)
           {         
               if(MMISMS_MT_TO_BE_READ == cur_longsms_order_id->flag.mo_mt_type)
               {
                   MMI_STRING_T wait_text={0};    
                   MMISMS_SetOperStatus(MMISMS_UPDATE_EXCEED_CHAT_SMS);
                   MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &wait_text );
                   if(!MMK_IsOpenWin(MMISMS_WAITING_WIN_ID))
                   {
                       MMIPUB_OpenWaitWin(1, &wait_text, PNULL, PNULL, MMISMS_WAITING_WIN_ID, IMAGE_NULL,
                               ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, PNULL);
                   }
                   g_mmisms_global.update_oper_order_id = cur_longsms_order_id;
                   MNSMS_UpdateSmsStatusEx((MN_DUAL_SYS_E)cur_longsms_order_id->flag.dual_sys,
                       (MN_SMS_STORAGE_E)cur_longsms_order_id->flag.storage,
                       cur_longsms_order_id->record_id,
                       MN_SMS_MT_READED
                       );
                   return;
               }               
               cur_longsms_order_id = cur_longsms_order_id->next_long_sms_ptr;
           }           
       }  
       temp_order_id = temp_order_id->next_chat_ptr;
    }     
    if(MMISMS_UPDATE_EXCEED_CHAT_SMS == MMISMS_GetCurrentOperStatus())
    {
        g_mmisms_global.update_oper_order_id = PNULL;
        MMISMS_ClearOperInfo();
    }
    MMK_CloseWin(MMISMS_WAITING_WIN_ID);
}

#endif

/*****************************************************************************/
//  Description : Get Update Sms Num
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUpdateSMSNum(MN_DUAL_SYS_E dual_sys)
{
    uint32 update_sms_num = 0;

    if (g_mmisms_global.is_need_update_order)
    {
        MMISMS_STATE_T *unread_state_ptr = PNULL;

        unread_state_ptr = s_mmisms_order.mt_unread_head_ptr;

        while (PNULL != unread_state_ptr)
        {
            BOOLEAN is_need_update = FALSE;

            if (MMISMS_TYPE_SMS == unread_state_ptr->flag.msg_type
                && (dual_sys >= MMI_DUAL_SYS_MAX || unread_state_ptr->flag.dual_sys == dual_sys))
            {
                if (unread_state_ptr->flag.is_concatenate_sms)//������Ϣ
                {
                    MMISMS_ORDER_ID_T cur_order_id = PNULL;

                    cur_order_id = unread_state_ptr;

                    while (PNULL != cur_order_id)
                    {
                        if (cur_order_id->flag.is_need_update)
                        {
                            cur_order_id  = cur_order_id->next_long_sms_ptr;
                            is_need_update = TRUE;
                        }
                        else//��ʾ�ڶ�ȡ�����յ�������Ϣ������һ����,����Ҫ�����б�
                        {
                            is_need_update = FALSE;
                            break;
                        }
                    }
                }
                else if (unread_state_ptr->flag.is_need_update)//��ͨ����Ϣ
                {
                    is_need_update = TRUE;
                }
            }

            if (is_need_update)
            {
                update_sms_num++;
            }

            unread_state_ptr = unread_state_ptr->next_ptr;
        }
    }

    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetUpdateSMSNum update_sms_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11261_112_2_18_2_54_17_705,(uint8*)"d", update_sms_num);

    return update_sms_num;    
}
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : Get Update Sms Num
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadChatMsgByOrderId(MMISMS_ORDER_ID_T cur_order_id)
{
    MMISMS_CHAT_STATE_UNIT_T *head_chat_id = PNULL;
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T head_order_id = PNULL;
    uint32 i = 0;

    head_chat_id = s_mmisms_chat_linked_order.head_ptr;
                 
    //SCI_TRACE_LOW:"head_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11279_112_2_18_2_54_17_706,(uint8*)"d",head_chat_id);

    //�����Ựģʽ��������
    while (PNULL != head_chat_id)
    {
        order_id = head_chat_id->head_ptr;
        head_order_id = order_id;//����ĳ��¼�ĻỰ����ͷָ��
        while (PNULL != order_id)
        {
            if (order_id == cur_order_id)
            {
                 MMISMS_SetSMSUpdateInfo(head_order_id);
                 
                 g_mmisms_global.operation.cur_order_index = 0;
                 SCI_MEMSET(g_mmisms_global.operation.cur_order_id_arr, 0 , 
                     sizeof(g_mmisms_global.operation.cur_order_id_arr));                         
                 
                 if (head_order_id->flag.is_concatenate_sms) //is long sms
                 {
                     cur_order_id = head_order_id;
                     while (cur_order_id != PNULL)
                     {
                         MMISMS_SetCurOrderId(i,cur_order_id); //���õ�ǰorder_id��ȫ�ֱ�������
                         cur_order_id  = cur_order_id->next_long_sms_ptr;
                         i++;
                     }
                 }
                 else //is normal sms
                 {
                     MMISMS_SetCurOrderId(0,head_order_id); //���õ�ǰorder_id��ȫ�ֱ������� 
                 }  
                 
                 cur_order_id = MMISMS_GetCurOperationOrderId();

                if((0 == MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_CHAT))
                   || (PNULL == cur_order_id)
                  )
                {
                    break;
                }
                MMISMS_ReadChatMsg(cur_order_id);
                return;
            }
            order_id = order_id->next_chat_ptr;
        }
        head_chat_id = head_chat_id->next_number_ptr;
    }
#if 0
    uint16 selected_index = 0;
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 number [MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMI_STRING_T   prompt_str = {0};
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id = PNULL;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    uint32 msg_record_id = 0;
    MMISMS_ORDER_ID_T sms_state_ptr = (MMISMS_ORDER_ID_T)user_data;
    
    box_type = MMISMS_GetSmsStateBoxtype(sms_state_ptr);
    
    //to get the info of current selected item
    MMISMS_SetCurSearchOperationOrderId(box_type, sms_state_ptr);
    msg_record_id = sms_state_ptr->record_id;
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11368_112_2_18_2_54_17_707,(uint8*)"d",g_mmisms_global.operation.cur_order_index);
        MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
        // to read the pointed SMS
        MMISMS_ReadPointedSMS();
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
        if (MMIBROWSER_IsRunning())
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
            MMIAPIOTA_WapPushEnter((uint16)msg_record_id);
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
            MMIAPIOTA_OTAEnter((uint16)msg_record_id);
        }
        break;

    default: 
        break;
    }
#endif
}
#endif

#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
//     Description : ���ݶ�ʱ���ţ�������������ָ��id��
//    Global resource dependence : 
//  Author:hongbing.ju
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLinkedArrayPtrIdByTimerMsg(MMINV_SMS_TIMERMSG_T *timer_msg_ptr)
{
    MMISMS_ORDER_ID_T head_order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id  = PNULL;

    if (PNULL != timer_msg_ptr)
    {
        //����box_type��ѭ������s_mmisms_order�е����ݡ����ض�Ӧ������ָ��id��
        //SCI_TRACE_LOW:"MMISMS: sms_record_id=%d, mo_mt_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11464_112_2_18_2_54_17_708,(uint8*)"dd", timer_msg_ptr->record_id, timer_msg_ptr->flag.mo_mt_type);
        
        switch(timer_msg_ptr->flag.mo_mt_type)
        {
        case MMISMS_MT_NOT_DOWNLOAD: //δ����mms
            head_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
            break;

        case MMISMS_MT_TO_BE_READ:   //δ����Ϣ
        case MMISMS_MT_SR_TO_BE_READ://δ��״̬����
            head_order_id = s_mmisms_order.mt_readed_head_ptr;
            break;

        case MMISMS_MT_HAVE_READ:    //�Ѷ���Ϣ
        case MMISMS_MT_SR_HAVE_READ:    //�Ѷ�״̬����
            head_order_id = s_mmisms_order.mt_readed_head_ptr;
            break;

        case MMISMS_MO_SEND_SUCC:
            head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
            break;
            
        case MMISMS_MO_SEND_FAIL:
            head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
            break;
            
        case MMISMS_MO_DRAFT:
            head_order_id = s_mmisms_order.mo_draft_head_ptr;
            break;

           
        default:
            break;        
        }

        //SCI_TRACE_LOW:"head_order_id is 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11499_112_2_18_2_54_17_709,(uint8*)"d", head_order_id);

        cur_order_id = head_order_id;

        while (PNULL != cur_order_id)
        {
            if (cur_order_id->record_id == timer_msg_ptr->record_id
                && cur_order_id->flag.storage == timer_msg_ptr->flag.storage
                && cur_order_id->flag.dual_sys == timer_msg_ptr->flag.dual_sys
                && cur_order_id->flag.mo_mt_type == timer_msg_ptr->flag.mo_mt_type
                )
            {
                break;
            }

            cur_order_id = cur_order_id->next_ptr;
        }
    }
    
    //SCI_TRACE_LOW:"MMISMS:cur_order_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11518_112_2_18_2_54_17_710,(uint8*)"d", cur_order_id);

    return cur_order_id;
}

/*****************************************************************************/
//     Description : ���Timer Msg�Ƿ���Ч
//    Global resource dependence : 
//  Author:hongbing.ju
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_CheckTimerMsgValid(void)
{
    uint16 i = 0;

    for (i = 0; i < MMISMS_TIMERMSG_MAX_ITEM; i++)
    {
        BOOLEAN is_valid = FALSE;
        MMINV_SMS_TIMERMSG_T *cursor = MMISMS_TIMERMSG_GetTimerMsgByIndex(i);
        MMISMS_ORDER_ID_T  cur_order_id = s_mmisms_order.mo_sendfail_head_ptr;

        if (PNULL != cursor && cursor->flag.is_in_use)
        {
            while (PNULL != cur_order_id)
            {
                if (cur_order_id->record_id == cursor->record_id
                    && cur_order_id->flag.storage == cursor->flag.storage
                    && cur_order_id->flag.dual_sys == cursor->flag.dual_sys
                    && cur_order_id->flag.mo_mt_type == cursor->flag.mo_mt_type
                    )
                {
                    is_valid = TRUE;

                    break;
                }

                cur_order_id = cur_order_id->next_ptr;
            }

            if (!is_valid)
            {
                MMIALMSVC_UnRegService(cursor->event_id, TRUE);

                SCI_MEMSET(cursor, 0x00, sizeof(MMINV_SMS_TIMERMSG_T));
            }
        }
    }
}
#endif

#ifdef MMI_SMS_SIMBOX
/*****************************************************************************/
//  Description : Sort message
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL void SIMBOX_SortContactMessageByTime(MMISMS_SIMBOX_DATA_T   *msgLogData)
{
    MMISMS_ORDER_ID_T   temp_order_id = 0;
    uint16 i =0,j=0;

    if (msgLogData == PNULL)
    {
        return;
    }

    for (i=0;i<msgLogData->matched_count;i++)
    {
        for (j=i;j<msgLogData->matched_count;j++)
        {
            if (msgLogData->order_id_list[j] == 0)  
            {
                //error case
                return;
            }
            if (msgLogData->order_id_list[j]->time > msgLogData->order_id_list[i]->time)
            {
                temp_order_id = msgLogData->order_id_list[i];
                msgLogData->order_id_list[i] = msgLogData->order_id_list[j];
                msgLogData->order_id_list[j] = temp_order_id;
            }
        }
    }
}

LOCAL void MMISMS_CheckAndAppendSimBOX(MMISMS_ORDER_ID_T current_order_id,
										  MN_DUAL_SYS_E dual_sys,
										  MMISMS_SIMBOX_DATA_T   *msgLogData)
{

    while(current_order_id != PNULL)
    {
        if (current_order_id->flag.msg_type!=MMISMS_TYPE_SMS)       // if isn't sms,continue
        {
            current_order_id =current_order_id->next_ptr;
            continue;
        }
        //save this current_order_id   
        if (current_order_id->flag.dual_sys != dual_sys || MN_SMS_STORAGE_SIM !=current_order_id->flag.storage)
        {
            current_order_id =current_order_id->next_ptr;
            continue;        
        }
        msgLogData->order_id_list[msgLogData->matched_count]=current_order_id ;
        msgLogData->matched_count++;
        //if equal, save current_order_id
        current_order_id =current_order_id->next_ptr;
    }
    return;
}

/*****************************************************************************/
//  Description : ��������
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
PUBLIC int16 MMISMS_SIMBOX_SearchMatchedItem(MMISMS_SIMBOX_DATA_T   *msgLogData,MN_DUAL_SYS_E dual_sys)
{
    MMISMS_ORDER_ID_T current_order_id = 0;

    msgLogData->matched_count = 0;

    current_order_id = s_mmisms_order.mt_unread_head_ptr;
    MMISMS_CheckAndAppendSimBOX(current_order_id,dual_sys,msgLogData);
    current_order_id = s_mmisms_order.mt_readed_head_ptr;
    MMISMS_CheckAndAppendSimBOX(current_order_id,dual_sys,msgLogData);
    current_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
    MMISMS_CheckAndAppendSimBOX(current_order_id,dual_sys,msgLogData);
    current_order_id = s_mmisms_order.mo_sendfail_head_ptr;
    MMISMS_CheckAndAppendSimBOX(current_order_id,dual_sys,msgLogData);
    current_order_id = s_mmisms_order.mo_draft_head_ptr;
    MMISMS_CheckAndAppendSimBOX(current_order_id,dual_sys,msgLogData);    
    
    //����ʱ��ð����,ʱ����������ǰ��
    SIMBOX_SortContactMessageByTime(msgLogData);
    
    return msgLogData->matched_count;
}

/*****************************************************************************/
// 	Description : ��ȡʱ��������ʾ��WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  �������ʱ�䣬������������
/*****************************************************************************/
LOCAL BOOLEAN SIMBOX_GetTimeAndDateContent(wchar* in_str,uint16 max_len,uint32 time)
{
    SCI_DATE_T sys_data = {0};
    MMI_TM_T tm = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;
    uint8      date_str[20] = {0};

    if (in_str == PNULL)
    {
        return FALSE;
    }
    tm = MMIAPICOM_Second2Tm(time);
    TM_GetSysDate(&sys_data);

    cl_sec =MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,tm.tm_year);
    sys_sec =MMIAPICOM_Tm2Second(0,0,0,sys_data.mday,sys_data.mon,sys_data.year);

    if(cl_sec == sys_sec)       //Today ,return Time_str
    {   
        MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min, date_str,20);
        MMI_STRNTOWSTR(in_str,max_len,(const uint8*)date_str,
                    strlen((char*)date_str),strlen((char*)date_str));
    }
    else                        //use Date
    {
        MMIAPISET_FormatDateStrByDateEx(tm.tm_year, tm.tm_mon, tm.tm_mday,'/', date_str,20,FALSE);
        MMI_STRNTOWSTR(in_str,max_len,(const uint8*)date_str,(uint32)strlen((char*)date_str),(uint32)strlen((char*)date_str));
    }

    return TRUE;

}


/*****************************************************************************/
// 	Description : listitem draw callback
//	Global resource dependence :
//  Author:   
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetSIMBOXMatchItemContent(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data, BOOLEAN isSelecting)
{
    uint16 index = 0;
    uint16 num_len = 0;
    wchar  time_wstr[20] = {0};
    uint8  disp_number[MMISMS_PBNUM_MAX_LEN+2] = {0};
    wchar  disp_num_wchar[MMISMS_PBNUM_MAX_LEN+1] = {0};
    wchar  sender_wchar[MMISMS_NUMBER_MAX_LEN+1] = {0};
    MMI_STRING_T   name_str = {0};
    BOOLEAN    find_result = FALSE;
    MMI_HANDLE_T	 ctrl_handle = need_item_data_ptr->ctrl_handle;
    GUILIST_ITEM_DATA_T item_data = {0};
    MMISMS_STATE_T sms_state = *(MMISMS_STATE_T *)user_data;
    
    index = need_item_data_ptr->item_index;
    
    SCI_MEMSET(&item_data, 0, sizeof(item_data));

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

    /* �����search��־��Ҫô�Ƿǳ����ţ�Ҫô�ǳ����ŵĵ�һ�� */
    //if (sms_state.flag.is_search
        //&& sms_state.longsms_seq_num <= 1)
    {

        SIMBOX_GetTimeAndDateContent(time_wstr,20,sms_state.time);
        if (isSelecting)    //re-layout time string to left of select icon
        {
        item_data.item_content[4].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[4].item_data.text_buffer.wstr_ptr = time_wstr;
        item_data.item_content[4].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
        }
        else
        {
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = time_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
        }

        if(sms_state.flag.msg_type == MMISMS_TYPE_SMS)
        {
            item_data.item_content[0].item_data.image_id = MMISMS_GetIconByStatus(&sms_state);
#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
            item_data.item_content[0].icon_id = GetSmallIconByStatus(&sms_state);
#else
            item_data.item_content[0].icon_id = GetSimSmallIcon(sms_state.flag.dual_sys);
#endif
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

            name_str.wstr_ptr = SCI_ALLOCAZ( ( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar) );
            if(PNULL == name_str.wstr_ptr)
            {
                //SCI_TRACE_LOW:"MMIAPISMS_SetMatchItemContent name_str.wstr_ptr alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11901_112_2_18_2_54_18_711,(uint8*)"");
                return;
            }
            //SCI_MEMSET(name_str.wstr_ptr,0,( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar));

            find_result = MMIAPIPB_GetNameByNumber((MMIPB_BCD_NUMBER_T *)&sms_state.number.bcd_number, 
                &name_str, MMIPB_MAX_STRING_LEN, TRUE);
                

            if (find_result && name_str.wstr_len > 0)
            {
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
                item_data.item_content[1].item_data.text_buffer.wstr_len = name_str.wstr_len;
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

                item_data.item_content[1].item_data.text_buffer.wstr_ptr = disp_num_wchar;
                item_data.item_content[1].item_data.text_buffer.wstr_len = num_len;
            }
        }
        else
        {
            item_data.item_content[0].item_data.image_id = MMISMS_GetIconByStatus(&sms_state);
#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
            item_data.item_content[0].icon_id = GetSmallIconByStatus(&sms_state);
#else
            item_data.item_content[0].icon_id = GetSimSmallIcon(sms_state.flag.dual_sys);
#endif

            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            name_str.wstr_ptr = SCI_ALLOCAZ( ( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar) );
            if(PNULL == name_str.wstr_ptr)
            {
                //SCI_TRACE_LOW:"MMIAPISMS_SetMatchItemContent2 name_str.wstr_ptr alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11940_112_2_18_2_54_18_712,(uint8*)"");
                return;
            }
            //SCI_MEMSET(name_str.wstr_ptr,0,( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar));

            find_result = MMISMS_GetNameByNumberFromPB(sms_state.number.sender,&name_str, MMISMS_PBNAME_MAX_LEN);
            if (find_result)
            {
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
                item_data.item_content[1].item_data.text_buffer.wstr_len = name_str.wstr_len;
            }
            else
            {
                MMIAPICOM_StrToWstr((uint8 *)sms_state.number.sender, sender_wchar);
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = sender_wchar;
                item_data.item_content[1].item_data.text_buffer.wstr_len = strlen((char *)sms_state.number.sender);
            }
        }
        
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = sms_state.display_content.content;
        item_data.item_content[3].item_data.text_buffer.wstr_len = sms_state.display_content.content_len;

        GUILIST_SetItemStyle(ctrl_handle, GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL, index);

        GUILIST_SetItemData(ctrl_handle, &item_data, index);

        if (name_str.wstr_ptr != PNULL)
        {
            SCI_FREE(name_str.wstr_ptr);
        }
    }
}

#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : AppendSplitListItem
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void AppendSplitListItem(MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;
//    item_t.first_letter = first_letter;
    item_t.first_letter = '\0';
    item_t.item_data_ptr = &item_data;
    item_t.item_state |=GUIITEM_STATE_SPLIT;
    item_t.user_data = 0XFFFFFFFF;

    GUILIST_SetSplitOnside(ctrl_id, TRUE);

    GUILIST_AppendItem( ctrl_id, &item_t );

}

/*****************************************************************************/
//  Description : �Ƚ϶��ŵĺ��룬���ϵļ���Match�б�
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL void CompareAndLog(MMIPB_CONTACT_T * contact_ptr,MMISMS_ORDER_ID_T current_order_id,MMISMS_DETAIL_MSGLOG_DATA_T   *msgLogData)
{
    uint16  j=0;
    uint16  num_len = 0;
    uint16  tele_num_len = 0;
    uint8	tele_num[MMISMS_NUMBER_MAX_LEN+2]={0};

    while(current_order_id != PNULL)
    {
        if(msgLogData->matched_count >= PBDETAIL_MSG_LOG_MAX_NUM) 
        {
            SCI_TRACE_LOW("mmisms msgLogData->matched_count exceed max num");
            break;
        }
                
        if (current_order_id->flag.msg_type>=MMISMS_TYPE_MAX)       // if isn't sms,continue
        {
            current_order_id =current_order_id->next_ptr;
            continue;
        }
        //Compare number        
        for (j=0;j<MMIPB_MAX_NV_PHONE_NUM;j++)
        {
            //Compare Number,if Not equal,continue
            if (current_order_id->flag.msg_type == MMISMS_TYPE_SMS)
            {
                MMIPB_BCD_NUMBER_T current_pb_bcd = MMISMS_SmsBCDToPbBCD(current_order_id->number.bcd_number); 
                if(0 == MMIAPIPB_BcdSmartCmp(&current_pb_bcd,&(contact_ptr->number[j])))
                { 
                    //save this current_order_id   
                    msgLogData->order_id_list[msgLogData->matched_count]=current_order_id ;
                    msgLogData->matched_count++;
                    break;
                }
            }
            else 
            {
                if (current_order_id->flag.msg_type ==MMISMS_TYPE_MMS)
                {
                    SCI_MEMSET(tele_num,0x00,sizeof(tele_num));
                    //Just use compare
                    num_len = SCI_STRLEN((char *)current_order_id->number.sender);
                    if (contact_ptr->number[j].number_len == 0)
                    {
                        continue;
                    }
                    MMIAPICOM_GenDispNumber(MN_NUM_TYPE_UNKNOW,contact_ptr->number[j].number_len,
                    contact_ptr->number[j].number,tele_num,MMISMS_NUMBER_MAX_LEN+2);
                    
                    tele_num_len = SCI_STRLEN((char*)tele_num);
                    
                    num_len = MAX(num_len, tele_num_len);

                    if (num_len == 0)
                    {
                        continue;
                    }
                    if(strncmp((char*)current_order_id->number.sender,(char*)tele_num,num_len)==0)
                    { 
                        //save this current_order_id   
                        msgLogData->order_id_list[msgLogData->matched_count]=current_order_id ;
                        msgLogData->matched_count++;
                        break;
                    }
                } 
            }

        }
        //if equal, save current_order_id
        current_order_id =current_order_id->next_ptr;
    }
    return;
}


/*****************************************************************************/
//  Description : ��ȡ��һ����ѡ�е�OrderID��Index
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMISMS_GetNextSelectedOrderID(void)
{
    uint16  i=0;
    if (g_sms_match_data_ptr == PNULL)
    {
        return PBDETAIL_MSG_LOG_MAX_NUM;
    }
    g_match_sms_delete_num ++;
    for (i=g_match_sms_delete_num;i<g_sms_match_data_ptr->matched_count;i++)
    {
        if (g_sms_match_data_ptr->itemSelected[i])
        {
            return i;
        }
    }
    return PBDETAIL_MSG_LOG_MAX_NUM+1;
}

/*****************************************************************************/
//  Description : ��ȡ��һ����ѡ�е�OrderID��Index
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL uint16 MMISMS_GetFirstSelectedOrderID(void)
{
    uint16  i=0;
    if (g_sms_match_data_ptr == PNULL)
    {
        return PBDETAIL_MSG_LOG_MAX_NUM;
    }
    for (i=0;i<g_sms_match_data_ptr->matched_count;i++)
    {
        if (g_sms_match_data_ptr->itemSelected[i])
        {
            return i;
        }
    }
    return PBDETAIL_MSG_LOG_MAX_NUM+1;
}

/*****************************************************************************/
//  Description : Sort message
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL void SortContactMessageByTime(MMISMS_DETAIL_MSGLOG_DATA_T   *msgLogData)
{
    MMISMS_ORDER_ID_T   temp_order_id = 0;
    uint16 i =0,j=0;

    if (msgLogData == PNULL)
    {
        return;
    }

    for (i=0;i<msgLogData->matched_count;i++)
    {
        for (j=i;j<msgLogData->matched_count;j++)
        {
            if (msgLogData->order_id_list[j] == 0)  
            {
                //error case
                return;
            }
            if (msgLogData->order_id_list[j]->time > msgLogData->order_id_list[i]->time)
            {
                temp_order_id = msgLogData->order_id_list[i];
                msgLogData->order_id_list[i] = msgLogData->order_id_list[j];
                msgLogData->order_id_list[j] = temp_order_id;
            }
        }
    }
}



/*****************************************************************************/
//  Description : ��������
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
PUBLIC int16 MMIAPISMS_SearchMatchedItem(MMIPB_CONTACT_T * contact_ptr,MMISMS_DETAIL_MSGLOG_DATA_T   *msgLogData)
{
    MMISMS_ORDER_ID_T current_order_id = 0;

    
    
    //Get contact number
    msgLogData->matched_count = 0;

    current_order_id = s_mmisms_order.mt_unread_head_ptr;
    CompareAndLog(contact_ptr,current_order_id,msgLogData);

    current_order_id = s_mmisms_order.mt_readed_head_ptr;
    CompareAndLog(contact_ptr,current_order_id,msgLogData);
    
    current_order_id = s_mmisms_order.mo_sendfail_head_ptr;
    CompareAndLog(contact_ptr,current_order_id,msgLogData);
    
    current_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
    CompareAndLog(contact_ptr,current_order_id,msgLogData);

    //����ʱ��ð����,ʱ����������ǰ��
    SortContactMessageByTime(msgLogData);
    
    return msgLogData->matched_count;
}

/*****************************************************************************/
//  Description : SMSɾ������sms
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
PUBLIC BOOLEAN MMISMS_StartDeleteMatchMsgById(MMISMS_DETAIL_MSGLOG_DATA_T   *match_data_ptr)
{
    uint32 i = 0;
    uint32 delete_msg_num = 0;
    MMI_STRING_T wait_text={0};
    
    if (PNULL == match_data_ptr)
    {
        return FALSE;
    }
    if (0 == match_data_ptr->matched_count || PNULL == match_data_ptr->order_id_list[0])
    {
        return FALSE;
    }

    for (i = 0; i < match_data_ptr->matched_count; i++)
    {
        if (match_data_ptr->itemSelected[i]
            && !match_data_ptr->order_id_list[i]->flag.is_locked)
        {
            delete_msg_num++;
        }
    }

    if (0 == delete_msg_num)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SMS_MSG_DEL_LOCKED);

        return FALSE;
    }
#ifdef MMS_SUPPORT
    g_match_is_exist_mms = FALSE;
#endif
    g_sms_match_data_ptr = match_data_ptr;
    g_match_sms_delete_num = MMISMS_GetFirstSelectedOrderID();

    if (g_match_sms_delete_num>=match_data_ptr->matched_count)
        return FALSE;
	MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,PNULL,ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,
                        MMIPUB_SOFTKEY_NONE,PNULL);

    MMISMS_SetOperStatus(MMISMS_MATCH_DATA_DELETE);
    MMISMS_DeleteNextMatchMsgById(match_data_ptr->order_id_list[g_match_sms_delete_num]);

    return TRUE;
}

/*****************************************************************************/
//  Description : SMSɾ������sms
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteNextMatchMsgById(MMISMS_ORDER_ID_T order_id)
{
    while (MMISMS_TYPE_MMS == order_id->flag.msg_type)
    {
        if (!order_id->flag.is_locked)
        {
#ifdef MMS_SUPPORT
            g_match_is_exist_mms = TRUE;
#endif

            //Delte it 
            MMIAPIMMS_DeleteMMS(order_id->record_id,MMISMS_DELETE_ANIM_WIN_ID);
        }
        //Next
        g_match_sms_delete_num = MMISMS_GetNextSelectedOrderID();
        //if next is end
        if (g_match_sms_delete_num >= g_sms_match_data_ptr->matched_count)
        {
            MMISMS_EndDeleteMatchMsg(g_sms_match_data_ptr);
            return TRUE;
        }
        order_id = g_sms_match_data_ptr->order_id_list[g_match_sms_delete_num];
    }

    if (MMISMS_TYPE_SMS == order_id->flag.msg_type
        && !order_id->flag.is_locked)
    {
        return MMISMS_DeletePointedSMSById(order_id);
    }
    else
    {
        g_match_sms_delete_num = MMISMS_GetNextSelectedOrderID();

        if (g_match_sms_delete_num >= g_sms_match_data_ptr->matched_count)
        {
            MMISMS_EndDeleteMatchMsg(g_sms_match_data_ptr);
        }
        else
        {
            return MMISMS_DeleteNextMatchMsgById(g_sms_match_data_ptr->order_id_list[g_match_sms_delete_num]);
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : SMSɾ������sms
//  Global resource dependence : none
//  Author: 
//  Note: for search module and delete by num
/***************************************************************************/
PUBLIC void MMISMS_EndDeleteMatchMsg(MMISMS_DETAIL_MSGLOG_DATA_T   *match_data_ptr)
{   
#ifdef MMS_SUPPORT
    if (g_match_is_exist_mms)
    {
        g_match_is_exist_mms = FALSE;

        MMIAPIMMS_SaveListInfo();
    }
#endif
    MMISMS_SetOperStatus(MMISMS_NO_OPER);
    MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
    if (match_data_ptr->callback != PNULL)
    {
        match_data_ptr->callback(TRUE);
    }
    g_sms_match_data_ptr=PNULL;
    return;
}

/*****************************************************************************/
// 	Description : ��ȡʱ��������ʾ��WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  �������ʱ�䣬������������
/*****************************************************************************/
LOCAL BOOLEAN GetTimeAndDateContent(wchar* in_str,uint16 max_len,uint32 time)
{
    SCI_DATE_T sys_data = {0};
    MMI_TM_T tm = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;
    uint8      date_str[20] = {0};

    if (in_str == PNULL)
    {
        return FALSE;
    }
    tm = MMIAPICOM_Second2Tm(time);
    TM_GetSysDate(&sys_data);

    cl_sec =MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,tm.tm_year);
    sys_sec =MMIAPICOM_Tm2Second(0,0,0,sys_data.mday,sys_data.mon,sys_data.year);

    if(cl_sec == sys_sec)       //Today ,return Time_str
    {   
        MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min, date_str,20);
        MMI_STRNTOWSTR(in_str,max_len,(const uint8*)date_str,
                    strlen((char*)date_str),strlen((char*)date_str));
    }
    else                        //use Date
    {
        MMIAPISET_FormatDateStrByDateEx(tm.tm_year, tm.tm_mon, tm.tm_mday,'/', date_str,20,FALSE);
        MMI_STRNTOWSTR(in_str,max_len,(const uint8*)date_str,(uint32)strlen((char*)date_str),(uint32)strlen((char*)date_str));
    }

    return TRUE;

}


/*****************************************************************************/
// 	Description : listitem draw callback
//	Global resource dependence :
//  Author:   
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetMatchItemContent(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data, BOOLEAN isSelecting)
{
    uint16 index = 0;
    uint16 num_len = 0;
    wchar  time_wstr[20] = {0};
    uint8  disp_number[MMISMS_PBNUM_MAX_LEN+2] = {0};
    wchar  disp_num_wchar[MMISMS_PBNUM_MAX_LEN+1] = {0};
    wchar  sender_wchar[MMISMS_NUMBER_MAX_LEN+1] = {0};
    MMI_STRING_T   name_str = {0};
    BOOLEAN    find_result = FALSE;
    MMI_HANDLE_T	 ctrl_handle = need_item_data_ptr->ctrl_handle;
    GUILIST_ITEM_DATA_T item_data = {0};
    MMISMS_STATE_T sms_state = *(MMISMS_STATE_T *)user_data;
    
    index = need_item_data_ptr->item_index;
    
    SCI_MEMSET(&item_data, 0, sizeof(item_data));

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

    /* �����search��־��Ҫô�Ƿǳ����ţ�Ҫô�ǳ����ŵĵ�һ�� */
    //if (sms_state.flag.is_search
        //&& sms_state.longsms_seq_num <= 1)
    {

        GetTimeAndDateContent(time_wstr,20,sms_state.time);
        if (isSelecting)    //re-layout time string to left of select icon
        {
#if 0
        item_data.item_content[4].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[4].item_data.text_buffer.wstr_ptr = time_wstr;
        item_data.item_content[4].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
#endif
        }
        else
        {
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = time_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
        }

        if(sms_state.flag.msg_type == MMISMS_TYPE_SMS)
        {
            item_data.item_content[0].item_data.image_id = MMISMS_GetIconByStatus(&sms_state);
#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
            item_data.item_content[0].icon_id = GetSmallIconByStatus(&sms_state);
#else
            item_data.item_content[0].icon_id = GetSimSmallIcon(sms_state.flag.dual_sys);
#endif
            
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

            name_str.wstr_ptr = SCI_ALLOCAZ( ( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar) );
            if(PNULL == name_str.wstr_ptr)
            {
                //SCI_TRACE_LOW:"MMIAPISMS_SetMatchItemContent name_str.wstr_ptr alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11901_112_2_18_2_54_18_711,(uint8*)"");
                return;
            }
            //SCI_MEMSET(name_str.wstr_ptr,0,( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar));

            find_result = MMIAPIPB_GetNameByNumber((MMIPB_BCD_NUMBER_T *)&sms_state.number.bcd_number, 
                &name_str, MMIPB_MAX_STRING_LEN, TRUE);
                

            if (find_result && name_str.wstr_len > 0)
            {
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
                item_data.item_content[1].item_data.text_buffer.wstr_len = name_str.wstr_len;
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

                item_data.item_content[1].item_data.text_buffer.wstr_ptr = disp_num_wchar;
                item_data.item_content[1].item_data.text_buffer.wstr_len = num_len;
            }
        }
        else
        {
            item_data.item_content[0].item_data.image_id = MMISMS_GetIconByStatus(&sms_state);
#ifdef MMI_SMS_SIM_PHONE_DIFF_ICON
            item_data.item_content[0].icon_id = GetSmallIconByStatus(&sms_state);
#else
            item_data.item_content[0].icon_id = GetSimSmallIcon(sms_state.flag.dual_sys);
#endif

            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            name_str.wstr_ptr = SCI_ALLOCAZ( ( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar) );
            if(PNULL == name_str.wstr_ptr)
            {
                //SCI_TRACE_LOW:"MMIAPISMS_SetMatchItemContent2 name_str.wstr_ptr alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11940_112_2_18_2_54_18_712,(uint8*)"");
                return;
            }
            //SCI_MEMSET(name_str.wstr_ptr,0,( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar));

            find_result = MMISMS_GetNameByNumberFromPB(sms_state.number.sender,&name_str, MMISMS_PBNAME_MAX_LEN);
            if (find_result)
            {
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
                item_data.item_content[1].item_data.text_buffer.wstr_len = name_str.wstr_len;
            }
            else
            {
                MMIAPICOM_StrToWstr((uint8 *)sms_state.number.sender, sender_wchar);
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = sender_wchar;
                item_data.item_content[1].item_data.text_buffer.wstr_len = strlen((char *)sms_state.number.sender);
            }
        }
        
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = sms_state.display_content.content;
        item_data.item_content[3].item_data.text_buffer.wstr_len = sms_state.display_content.content_len;

        GUILIST_SetItemStyle(ctrl_handle, GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL, index);

        GUILIST_SetItemData(ctrl_handle, &item_data, index);

        if (name_str.wstr_ptr != PNULL)
        {
            SCI_FREE(name_str.wstr_ptr);
        }
    }
}

#endif

#if 0
/*****************************************************************************/
//  Description : find replace sms
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteReplaceSms(uint32 valid_num)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    BOOLEAN is_found_delete_sms = FALSE;
    uint32 i = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
        
    for(i = 0; i < valid_num; i++)
    {    
        //unread sms
        is_found_delete_sms = FALSE;
        order_id = s_mmisms_order.mt_unread_head_ptr;
        while (PNULL != order_id)
        {
            if (order_id->flag.dual_sys == dual_sys
                && order_id->flag.storage == storage
                && order_id->record_id == record_id[i])
            {
                is_found_delete_sms = TRUE;
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }

        if(!is_found_delete_sms)
        {
            //readed sms
            order_id = s_mmisms_order.mt_readed_head_ptr;
            while (PNULL != order_id)
            {
                if (order_id->flag.dual_sys == dual_sys
                    && order_id->flag.storage == storage
                    && order_id->record_id == record_id[i])
                {
                    is_found_delete_sms = TRUE;
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }    
        }
        
        if(!is_found_delete_sms)
        {                    
            order_id = s_mmisms_order.mo_sendsucc_head_ptr;
            while (PNULL != order_id)
            {
                if (order_id->flag.dual_sys == dual_sys
                    && order_id->flag.storage == storage
                    && order_id->record_id == record_id[i])
                {
                    is_found_delete_sms = TRUE;
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }  
        }
    
        //sendfail sms
        if(!is_found_delete_sms)
        {
            order_id = s_mmisms_order.mo_sendfail_head_ptr;
            while (PNULL != order_id)
            {
                if (order_id->flag.dual_sys == dual_sys
                    && order_id->flag.storage == storage
                    && order_id->record_id == record_id[i])
                {
                    is_found_delete_sms = TRUE;
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }  
        }  
    
        //nosend sms
        if(!is_found_delete_sms)
        {
            order_id = s_mmisms_order.mo_draft_head_ptr;
            while (PNULL != order_id)
            {
                if (order_id->flag.dual_sys == dual_sys
                    && order_id->flag.storage == storage
                    && order_id->record_id == record_id[i])
                {
                    is_found_delete_sms = TRUE;
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            } 
        }  

        if(!is_found_delete_sms)
        {
            order_id = s_mmisms_order.security_mt_unread_head_ptr;
            
            while(order_id != PNULL)
            {
                if (order_id->flag.dual_sys == dual_sys
                    && order_id->flag.storage == storage
                    && order_id->record_id == record_id[i])
                {
                    is_found_delete_sms = TRUE;
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
        }
        
        if(!is_found_delete_sms)
        {
            order_id = s_mmisms_order.security_head_ptr;
            while (PNULL != order_id)
            {
                if (order_id->flag.dual_sys == dual_sys
                    && order_id->flag.storage == storage
                    && order_id->record_id == record_id[i])
                {
                    is_found_delete_sms = TRUE;
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
        }
        SCI_TRACE_LOW("mmisms is_found_delete_sms = %d,dual_sys = %d,storage = %d,record_id = %d" ,is_found_delete_sms,dual_sys,storage,order_id->record_id);
        if(is_found_delete_sms)
        {
            MMISMS_DelSmsInfoInOrder(order_id);
        }
    }
    return is_found_delete_sms;
}
#endif

/*****************************************************************************/
//  Description : find replace sms
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ReplaceSms(MN_SMS_PID_E pid_e, MMISMS_STATE_T *sms_state_ptr)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    BOOLEAN is_replace = FALSE;

    if (PNULL == sms_state_ptr)
    {
        return is_replace;
    }

    //SCI_TRACE_LOW:"MMISMS: MMISMS_ReplaceSms pid_e=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11989_112_2_18_2_54_18_713,(uint8*)"d", pid_e);

    if (pid_e < MN_SMS_PID_REPLACE_ONE || pid_e > MN_SMS_PID_REPLACE_SEV)
    {
        return is_replace;
    }

    //unread sms
    order_id = s_mmisms_order.mt_unread_head_ptr;
    while (PNULL != order_id)
    {
        if (order_id->flag.dual_sys == sms_state_ptr->flag.dual_sys
            && order_id->flag.storage == sms_state_ptr->flag.storage
            && order_id->record_id == sms_state_ptr->record_id)
        {
            goto MMISMS_REPLEASESMS;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }

    //readed sms
    order_id = s_mmisms_order.mt_readed_head_ptr;
    while (PNULL != order_id)
    {
        if (order_id->flag.dual_sys == sms_state_ptr->flag.dual_sys
            && order_id->flag.storage == sms_state_ptr->flag.storage
            && order_id->record_id == sms_state_ptr->record_id)
        {
            goto MMISMS_REPLEASESMS;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }    

    //sendsucc sms
    order_id = s_mmisms_order.mo_sendsucc_head_ptr;
    while (PNULL != order_id)
    {
        if (order_id->flag.dual_sys == sms_state_ptr->flag.dual_sys
            && order_id->flag.storage == sms_state_ptr->flag.storage
            && order_id->record_id == sms_state_ptr->record_id)
        {
            goto MMISMS_REPLEASESMS;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }  

    //sendfail sms
    order_id = s_mmisms_order.mo_sendfail_head_ptr;
    while (PNULL != order_id)
    {
        if (order_id->flag.dual_sys == sms_state_ptr->flag.dual_sys
            && order_id->flag.storage == sms_state_ptr->flag.storage
            && order_id->record_id == sms_state_ptr->record_id)
        {
            goto MMISMS_REPLEASESMS;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }    

    //nosend sms
    order_id = s_mmisms_order.mo_draft_head_ptr;
    while (PNULL != order_id)
    {
        if (order_id->flag.dual_sys == sms_state_ptr->flag.dual_sys
            && order_id->flag.storage == sms_state_ptr->flag.storage
            && order_id->record_id == sms_state_ptr->record_id)
        {
            goto MMISMS_REPLEASESMS;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }   

    order_id = s_mmisms_order.security_mt_unread_head_ptr;
    
    while(order_id != PNULL)
    {
        if (order_id->flag.dual_sys == sms_state_ptr->flag.dual_sys
            && order_id->flag.storage == sms_state_ptr->flag.storage
            && order_id->record_id == sms_state_ptr->record_id)
        {
            goto MMISMS_REPLEASESMS;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }

    order_id = s_mmisms_order.security_head_ptr;
    while (PNULL != order_id)
    {
        if (order_id->flag.dual_sys == sms_state_ptr->flag.dual_sys
            && order_id->flag.storage == sms_state_ptr->flag.storage
            && order_id->record_id == sms_state_ptr->record_id)
        {
            goto MMISMS_REPLEASESMS;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }

MMISMS_REPLEASESMS:
    if (PNULL != order_id)
    {
        MMISMS_DelSmsInfoInOrder(order_id);
        MMISMS_InsertSmsToOrder(sms_state_ptr);
    
        is_replace = TRUE;
    }

    //SCI_TRACE_LOW:"MMISMS: MMISMS_ReplaceSms is_replace=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_12116_112_2_18_2_54_19_714,(uint8*)"d", is_replace);

    return is_replace;
}

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
/*****************************************************************************/
//  Description : ��������
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_FullOrder(void)
{
    uint16 total_msg_num = s_mmisms_order.max_sms_num.total_msg_num;
    MMISMS_LINKED_ORDER_UNIT_T *head_ptr = PNULL;
    MMISMS_LINKED_ORDER_UNIT_T *used_unit_head_ptr = PNULL;
    uint32 link_order_unit_size = sizeof(MMISMS_LINKED_ORDER_UNIT_T);
    BOOLEAN is_concatenated_sms = MMISMS_GetConcatenatedSMS();
    uint32 i =0;
    uint32 used_unit_num =0;

    if (!MMISMS_IsOrderOk())
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_FullOrder Order Not Ready!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_12138_112_2_18_2_54_19_715,(uint8*)"");

        return;
    }

    head_ptr = SCI_ALLOCAZ(total_msg_num * link_order_unit_size);

    if (PNULL == head_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_FullOrder No Memory!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_12147_112_2_18_2_54_19_716,(uint8*)"");

        return;
    }

//    for (i = 0; i< (link_order_unit_size - 1); i++)
//    {
//        head_ptr[i].next_ptr = &(head_ptr[i + 1]);
//    }

    used_unit_head_ptr = s_mmisms_linked_order.used_unit_head_ptr;

    while (PNULL != used_unit_head_ptr)
    {
        SCI_MEMCPY(&head_ptr[used_unit_num++].sms_state, &used_unit_head_ptr->sms_state, sizeof(MMISMS_STATE_T));

        used_unit_head_ptr = used_unit_head_ptr->next_ptr;
    }

    MMISMS_FreeSmsOrder();
    MMISMS_InitSmsOrder();

    for (i = 0; i < used_unit_num; i++)
    {
        if (is_concatenated_sms)
        {
            if (MMISMS_TYPE_SMS == head_ptr[i].sms_state.flag.msg_type
                && 0 != head_ptr[i].sms_state.longsms_max_num
                && 0 != head_ptr[i].sms_state.longsms_seq_num)//long sms
            {
                head_ptr[i].sms_state.flag.is_concatenate_sms = TRUE;
                head_ptr[i].sms_state.flag.is_receive_all = FALSE;
            }
        }
        else
        {
            head_ptr[i].sms_state.flag.is_concatenate_sms = FALSE;
        }

        MMISMS_InsertSmsToOrder(&head_ptr[i].sms_state);
    }

    MMISMS_InitOtherMessageList();

    SCI_FREE(head_ptr);
}
#endif

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : SMS���ݲ��Һ���
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindMsgForSmsBackup(uint32 index)
{
    MMISMS_LINKED_ORDER_UNIT_T *linked_order_id = PNULL;
    MMISMS_ORDER_ID_T find_pos = PNULL;  
    uint32 num = 0;

    //SCI_TRACE_LOW:"MMISMS:MMISMS_FindMsgForSmsBackup index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_12206_112_2_18_2_54_19_717,(uint8*)"d", index);
    if (index > MMISMS_GetSmsNumByStorage(MN_DUAL_SYS_1, MN_SMS_STORAGE_ME))
    {
        return PNULL;
    }

    linked_order_id = s_mmisms_linked_order.used_unit_head_ptr;
    while (num < index && PNULL != linked_order_id)
    {
        find_pos = &linked_order_id->sms_state;
        if (MMISMS_TYPE_SMS == find_pos->flag.msg_type && MN_SMS_STORAGE_ME == find_pos->flag.storage)
        {
            num++;
            if (num >= index)
            {
                break;
            }
        }
        linked_order_id = linked_order_id->next_ptr;
    }
    if (PNULL == linked_order_id)
    {
        return PNULL;
    }                      
    else
    {
        return find_pos;
    }
}

/*****************************************************************************/
//  Description : SMS��ԭ�Ƿ���ͬ�жϺ���
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: judged by the time and number
/***************************************************************************/
PUBLIC BOOLEAN MMISMS_IsExistSameSms(MMISMS_BACKUP_DATA_T *backup_data_ptr)
{
    MMISMS_LINKED_ORDER_UNIT_T *linked_order_id = PNULL;
    MMISMS_ORDER_ID_T find_pos = PNULL;
    BOOLEAN is_exist = FALSE;

    if (0 == backup_data_ptr->time)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_IsExistSameSms 0 == backup_data_ptr->time"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_12248_112_2_18_2_54_19_718,(uint8*)"");
        return FALSE;
    }
    linked_order_id = s_mmisms_linked_order.used_unit_head_ptr;
    while (PNULL != linked_order_id)
    {
        find_pos = &linked_order_id->sms_state;
        if (MMISMS_TYPE_SMS == find_pos->flag.msg_type && MN_SMS_STORAGE_ME == find_pos->flag.storage)
        { 
            if (find_pos->time == backup_data_ptr->time 
                && !strcmp((char *)&backup_data_ptr->number.number, (char *)&find_pos->number.bcd_number.number)
                && find_pos->longsms_seq_num == backup_data_ptr->longsms_seq_num //��������С���кŲ�ͬ��
                && find_pos->head_ref == backup_data_ptr->head_ref)  //�����ŵĴ����кŲ�ͬ
            {
                is_exist = TRUE;
                break;
            }
        }
        linked_order_id = linked_order_id->next_ptr;
    }
    //SCI_TRACE_LOW:"MMISMS: MMISMS_IsExistSameSms is_exist = %d, backup_data_ptr->time = %d, backup_data_ptr->record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_12265_112_2_18_2_54_19_719,(uint8*)"ddd", is_exist, backup_data_ptr->time, backup_data_ptr->record_id);
    return is_exist;
}
#endif

#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : is push message 
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_IsPushMessage(MMISMS_ORDER_ID_T cur_order_id)
{
   BOOLEAN is_push_msg = FALSE;
   if(PNULL == cur_order_id)
   {
        return FALSE;
   }
   if(MMISMS_TYPE_WAP_PUSH == cur_order_id->flag.msg_type
       || MMISMS_TYPE_MMS_OTA == cur_order_id->flag.msg_type
       || MMISMS_TYPE_WAP_OTA == cur_order_id->flag.msg_type)
   {
        is_push_msg = TRUE;
   }

   return is_push_msg;    
}

/*****************************************************************************/
//     Description : ��ȡwap push������
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL uint16 MMISMS_GetPushMsgTotalNumberFromOrder(void)
{
    uint16 count = 0;
    MMISMS_ORDER_ID_T cur_order_id = 0;    
    
    cur_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
    while(cur_order_id)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id = s_mmisms_order.mt_unread_head_ptr;
    while(cur_order_id)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id = s_mmisms_order.mt_readed_head_ptr;
    while(cur_order_id)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    
    return count;
}

/*****************************************************************************/
//     Description : list push message 
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ListPushMsgFromOrder(void)
{
    MMISMS_ORDER_ID_T head_order_id = 0;  
    if (MMISMS_IsNeedSort()) //����������ͷ����ı䣬������������������
    {
        head_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mt_undownloaded_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mt_unread_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mt_unread_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mt_readed_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mt_readed_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mo_sendsucc_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mo_sendfail_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mo_sendfail_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.mo_draft_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.mo_draft_head_ptr = head_order_id;
        }

        head_order_id = s_mmisms_order.security_mt_undownloaded_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.security_mt_undownloaded_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.security_mt_unread_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.security_mt_unread_head_ptr = head_order_id;
        }
        
        head_order_id = s_mmisms_order.security_head_ptr;
        if (head_order_id != PNULL)
        {
            head_order_id = SortOrderList(head_order_id, MMISMS_GetOrderType());
            s_mmisms_order.security_head_ptr = head_order_id;
        }

#ifdef MMI_SMS_CHAT_SUPPORT
        if(MMISMS_ORDER_BY_NAME == MMISMS_GetOrderType())
        {
            SortChatLinkedOrderByName();
        }
        else
        {
            SortChatLinkedOrderByTime();
        }
#endif        
        MMISMS_SetNeedSort(FALSE);
    }     
    GUILIST_RemoveAllItems(MMISMS_PUSHMSG_LISTBOX_CTRL_ID);
    head_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
    FormPushMsgListFromLinkhead(head_order_id, MMISMS_PUSHMSG_LISTBOX_CTRL_ID);
        
    head_order_id = s_mmisms_order.mt_unread_head_ptr;
    FormPushMsgListFromLinkhead(head_order_id, MMISMS_PUSHMSG_LISTBOX_CTRL_ID);
        
    head_order_id = s_mmisms_order.mt_readed_head_ptr;
    FormPushMsgListFromLinkhead(head_order_id, MMISMS_PUSHMSG_LISTBOX_CTRL_ID);
}

/*****************************************************************************/
//     Description : ����wap push��OTA��Ϣ
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_LoadPushMsgFromOrder(MMI_CTRL_ID_T ctrl_id)
{
    uint16 count = 0;
    uint16 list_item_num = 0;
    BOOLEAN ret = FALSE;
    count = MMISMS_GetPushMsgTotalNumberFromOrder();
    //SCI_TRACE_LOW:"MMISMS: MMISMS_LoadSmsListFromOrder the count of list box is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_12439_112_2_18_2_54_19_720,(uint8*)"d", count);

    list_item_num = MAX(count, 1);

    GUILIST_SetMaxItem(ctrl_id, list_item_num, TRUE);//max item 

    if (count > 0)
    {
        MMISMS_ListPushMsgFromOrder();    
        ret = TRUE;
    }
    else
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }
    return ret;    	
}
/*****************************************************************************/
//     Description : Form push message List from the link list head
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN FormPushMsgListFromLinkhead(MMISMS_ORDER_ID_T head_order_id, MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T item_info = {0};/*lint !e64*/
    MMISMS_ORDER_ID_T current_order_id = 0;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle( ctrl_id );
    
    current_order_id = head_order_id;    
    SCI_MEMSET(&item_info, 0, sizeof(GUILIST_ITEM_T)); 
#ifdef MMISMS_PORTRAIT_SUPPORT
    item_info.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT;
#else
    item_info.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL;
#endif

    while(current_order_id != PNULL)
    {
        if(MMISMS_IsPushMessage(current_order_id))
        {
            GUILIST_AppendItem(list_handle, &item_info);
        }

        current_order_id = current_order_id->next_ptr;
    }

    return TRUE;
}


/*****************************************************************************/
//     Description : ����push����list�б��index�����push list��index
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetPushMsgLinkedArrayPtrId(uint32 list_index)
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;    
    uint16 count = 0;
    cur_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
    while(PNULL != cur_order_id)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            if(count == list_index)
            {
                return cur_order_id;
            }
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    
    cur_order_id = s_mmisms_order.mt_unread_head_ptr;
    while(PNULL != cur_order_id)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            if(count == list_index)
            {
                return cur_order_id;
            }
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id = s_mmisms_order.mt_readed_head_ptr;
    while(PNULL != cur_order_id)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            if(count == list_index)
            {
                return cur_order_id;
            }
            count++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }    
    return cur_order_id;
}

/*****************************************************************************/
//     Description : mark all push message
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAllPushMsgMarked(
                                   BOOLEAN           is_marked //IN:
                                   )
{
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    
    cur_order_id =s_mmisms_order.mt_undownloaded_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            cur_order_id->flag.is_marked = is_marked;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    cur_order_id =s_mmisms_order.mt_unread_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            cur_order_id->flag.is_marked = is_marked;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    cur_order_id =s_mmisms_order.mt_readed_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_IsPushMessage(cur_order_id))
        {
            cur_order_id->flag.is_marked = is_marked;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
}       

/*****************************************************************************/
//     Description : get push message box delete available msg num
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetPushMsgDeleteAvailNum(void)
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint32 delete_sms_num = 0;

    if (MMIAPIUDISK_UdiskIsRun())
    {
        return 0;
    }

    cur_order_id = s_mmisms_order.mt_undownloaded_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_IsPushMessage(cur_order_id) && cur_order_id->flag.is_marked && !cur_order_id->flag.is_locked)
        {
            delete_sms_num++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id = s_mmisms_order.mt_unread_head_ptr;    
    while (cur_order_id != PNULL)
    {
        if(MMISMS_IsPushMessage(cur_order_id) && cur_order_id->flag.is_marked && !cur_order_id->flag.is_locked)
        {
            delete_sms_num++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }

    cur_order_id = s_mmisms_order.mt_readed_head_ptr;
    while (cur_order_id != PNULL)
    {
        if(MMISMS_IsPushMessage(cur_order_id) && cur_order_id->flag.is_marked && !cur_order_id->flag.is_locked)
        {
            delete_sms_num++;
        }
        cur_order_id = cur_order_id->next_ptr;
    }
    return delete_sms_num;
}
#endif

/*****************************************************************************/
//     Description : MMISMS_GetUnhandleMeSmsNumber
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetUnhandleMeSmsNumber(void)
{
    MMISMS_ORDER_ID_T not_handle_ptr = s_mmisms_order.not_handled_head_ptr; 
    uint32 sms_unhandle_num = 0;

    while(PNULL != not_handle_ptr)
    {
        if(MMISMS_TYPE_SMS == not_handle_ptr->flag.msg_type && MN_SMS_STORAGE_ME == not_handle_ptr->flag.storage )
        {
            sms_unhandle_num++;
        } 
        not_handle_ptr = not_handle_ptr->next_ptr;        
    }

    return sms_unhandle_num;
}


/*****************************************************************************/
// 	Description : MMISMS_GetUnitIDByIndex
//	Global resource dependence :
//  Author:   plum.peng
//	Note:
/*****************************************************************************/
PUBLIC MMISMS_LINKED_ORDER_UNIT_T* MMISMS_GetUnitIDByIndex(uint16 index)
{
    uint16 cur_index = 0;
    // MMI_CTRL_ID_T	 ctrl_id = MMISMS_SEARCH_LISTBOX_CTRL_ID;
    MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id = PNULL;

    cur_index = index;
    
    cur_unit_id = MMISMS_GetUsedOrderId();

    while (cur_unit_id != PNULL)
    {
        if (cur_unit_id->sms_state.flag.is_search
            && cur_unit_id->sms_state.longsms_seq_num <= 1)
        {
            if (cur_index-- == 0)
            {
                return cur_unit_id;
            }
            
        }

        cur_unit_id = cur_unit_id->next_ptr;
    }

    return PNULL;    

}


/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurDeleteData(MMISMS_OPERATE_DATA_INFO_T *op_info)
{
    MMISMS_ORDER_ID_T first_pos = PNULL;
    if(op_info->is_del_msg_type) //ɾ����Ϣ����
    {
        first_pos = MMISMS_FindValidTypeMsgForDelAll(op_info->msg_type);     
    }
    else
    {    
        if(op_info->is_del_all)
        {
            first_pos = MMISMS_FindValidBoxMsgForDelAll(op_info->box_type);
        }
        else    
        {
            switch (op_info->box_type)        
            {
            case MMISMS_BOX_SENDSUCC://ɾ���ѷ�����
            case MMISMS_BOX_SENDFAIL://ɾ��������
            case MMISMS_BOX_NOSEND://ɾ���ݸ���
                first_pos = MMISMS_FindValidMoSMSForDelAll(op_info->box_type);
                break;
                
            case MMISMS_BOX_MT://ɾ���ռ���
                first_pos = MMISMS_FindValidMTSMSForDelAll();
                break;
        #if defined (MMI_SMS_SECURITYBOX_SUPPORT)
            case MMISMS_BOX_SECURITY://ɾ����ȫ����
                first_pos = MMISMS_FindValidSecuritySMSForDelAll();
                break;        
        #endif
        #ifdef MMI_SMS_CHAT_SUPPORT
            case MMISMS_BOX_CHAT:
                first_pos = MMISMS_FindValidChatSMSForDelAll();
                break;
        #endif
        
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    case MMISMS_BT_BOX_SENDSUCC://ɾ��BT�ѷ�����
        first_pos = MMISMS_FindValidMoSMSForDelAll(op_info->box_type);
        break;
        
    case MMISMS_BT_BOX_SENDFAIL://ɾ��BT������
        first_pos = MMISMS_FindValidMoSMSForDelAll(op_info->box_type);
        break;
        
    case MMISMS_BT_BOX_NOSEND://ɾ��BT�ݸ���
        first_pos = MMISMS_FindValidMoSMSForDelAll(op_info->box_type);
        break;
        
    case MMISMS_BT_BOX_MT://ɾ��BT�ռ���
        first_pos = MMISMS_FindValidBTMTSMSForDelAll();
        break;
#endif

            default:
                break;
            }
        }
    }
        
    return first_pos;
}
#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurMoveData(MMISMS_MOVE_DATA_INFO_T *move_info_ptr,int32 *ret_val)
{
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MMISMS_ORDER_ID_T first_pos = PNULL;
    BOOLEAN is_nv = FALSE;
    uint8 sim_long_sms_num = 0 ;
    MMISMS_ORDER_ID_T next_long_sms_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    BOOLEAN is_permit_move = TRUE;
    uint16 sms_max_num = 0;
    uint16 sms_used_num = 0;
    BOOLEAN is_find_free_record = FALSE;
    MN_SMS_RECORD_ID_T record_id = 0;
    if(PNULL == move_info_ptr)
    {
        return PNULL;    
    }
    g_mmisms_global.operation.cur_type = move_info_ptr->box_type;
    MMISMS_SetOperStatus(move_info_ptr->oper_status);
    if((MMISMS_MOVE_SMS_TO_ME == move_info_ptr->oper_status) ||
       (MMISMS_MOVE_SMS_TO_SIM == move_info_ptr->oper_status)) //�ƶ�ME��SIM��
    {
        if(MMISMS_MOVE_SMS_TO_ME == move_info_ptr->oper_status)
        {
            storage = MN_SMS_STORAGE_SIM;
        }
        else if(MMISMS_MOVE_SMS_TO_SIM == move_info_ptr->oper_status)
        {
            storage = MN_SMS_STORAGE_ALL;
        }
        switch (move_info_ptr->box_type)        
        {
            case MMISMS_BOX_SENDSUCC://�ѷ�����
            case MMISMS_BOX_SENDFAIL://������
            case MMISMS_BOX_NOSEND://�ݸ���
                first_pos = MMISMS_FindValidMoSMSForMoveAll(move_info_ptr->box_type,storage);
                break;
                
            case MMISMS_BOX_MT://�ռ���
                first_pos = MMISMS_FindValidMTSMSForMoveAll(storage);
                break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
            case MMISMS_BOX_SECURITY://��ȫ����
                first_pos = MMISMS_FindValidSecuritySMSForMoveAll(storage);
                break;        
#endif                
            default:
                break;
        }        
    }
    else//�ƶ���ȫ����
    {
        switch (move_info_ptr->box_type)        
        {
            case MMISMS_BOX_SENDSUCC://�ѷ�����
            case MMISMS_BOX_SENDFAIL://������
            case MMISMS_BOX_NOSEND://�ݸ���
                first_pos = MMISMS_FindValidSecurityMoSMSForDelAll(move_info_ptr->box_type);
                break;
                
            case MMISMS_BOX_MT://�ռ���
                first_pos = MMISMS_FindValidSecurityMTSMSForDelAll();
                break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
            case MMISMS_BOX_SECURITY://��ȫ����
                first_pos = MMISMS_FindValidSecuritySMSForDelAll();
                break;        
#endif       

#ifdef MMI_SMS_CHAT_SUPPORT				
            case MMISMS_BOX_CHAT:
            {
#if 0                
                uint16 index = 0;
                MMI_CTRL_ID_T cur_ctrl_id = 0;
                cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
                index = GUILIST_GetCurItemIndex(cur_ctrl_id);
#endif                
                first_pos = MMISMS_FindValidChatSMSMoveToSecurity(move_info_ptr->pb_num);
            }
            break;
#endif
            default:
                break;
        }
    }
    
    if((MMISMS_MOVE_SMS_TO_SECURITY == move_info_ptr->oper_status) ||
       (MMISMS_MOVE_SMS_TO_ME == move_info_ptr->oper_status) ||
       (MMISMS_MOVE_SMS_TO_SIM == move_info_ptr->oper_status))
    {
        if((PNULL != first_pos) &&
           (MMISMS_TYPE_SMS == first_pos->flag.msg_type))
        {
            //�ƶ�SIM����Ϣ���ֻ�/��ȫ����
            if((MN_SMS_STORAGE_SIM == first_pos->flag.storage) &&
               ((MMISMS_MOVE_SMS_TO_ME == move_info_ptr->oper_status) ||
			    (MMISMS_MOVE_SMS_TO_SECURITY == move_info_ptr->oper_status)))
            {
				dual_sys = first_pos->flag.dual_sys;				
				is_nv = TRUE;
            }
            //�ƶ���Ϣ��SIM��
            else if(MMISMS_MOVE_SMS_TO_SIM == move_info_ptr->oper_status)
            {
                dual_sys = MMISMS_GetCurSaveDualSys();
            }
            //�ƶ�NV��Ϣ����ȫ����
			else if((MN_SMS_STORAGE_ME == first_pos->flag.storage) &&
					(MMISMS_MOVE_SMS_TO_SECURITY == move_info_ptr->oper_status))
			{
				dual_sys = first_pos->flag.dual_sys;				
				is_nv = TRUE;				
			}
            else
            {
                is_permit_move = FALSE;
            }
            if(is_permit_move)
            {
                MMISMS_GetSMSMemory(dual_sys, is_nv,&sms_max_num,&sms_used_num);
                
                if(first_pos->flag.is_concatenate_sms)//�����ţ���ѯĿ����ʿռ��Ƿ��㹻
                {
                    sim_long_sms_num++;
                    next_long_sms_ptr = first_pos->next_long_sms_ptr;

                    while(PNULL != next_long_sms_ptr)
                    {
                        sim_long_sms_num++;
                        next_long_sms_ptr = next_long_sms_ptr->next_long_sms_ptr;
                    }
                    
                    if((sim_long_sms_num > (sms_max_num - sms_used_num)) &&
                       (MN_SMS_STORAGE_SIM == first_pos->flag.storage))
                    {
                        *ret_val =  MMISMS_NO_ENOUGH_SPACE;
                        return PNULL;
                    }
                }
                else
                {
                    if(MMISMS_MOVE_SMS_TO_SIM == move_info_ptr->oper_status)
                    {
                        is_find_free_record = MMISMS_FindFreeRecordId(dual_sys, MN_SMS_STORAGE_SIM, &record_id);
                    }
                    else
                    {
                        is_find_free_record = MMISMS_FindFreeRecordId(dual_sys, MN_SMS_STORAGE_ME, &record_id);
                    }
                    if(!is_find_free_record)
                    {
  					    *ret_val =  MMISMS_NO_ENOUGH_SPACE;
  					    return PNULL;						
                    }
                }
            }
            else
            {
                *ret_val =  MMISMS_NO_ENOUGH_SPACE;
                return PNULL;
            }
        }
    }
    return first_pos;
        
}
#endif
#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurCopyData(MMISMS_COPY_DATA_INFO_T *copy_info_ptr,int32 *ret_val)
{
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MMISMS_ORDER_ID_T first_pos = PNULL;
//    BOOLEAN is_nv = FALSE;
    uint8 sim_long_sms_num = 0 ;
    MMISMS_ORDER_ID_T next_long_sms_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint16 sms_max_num = 0;
    uint16 sms_used_num = 0;
    BOOLEAN is_find_free_record = FALSE;
    MN_SMS_RECORD_ID_T record_id = 0;
    MMISMS_OPER_STATUS_E oper_status = MMISMS_NO_OPER;
    MMISMS_BOX_TYPE_E type = MMISMS_BOX_NONE;
    
    if(PNULL == copy_info_ptr)
    {
        return PNULL;    
    }
    g_mmisms_global.operation.cur_type = copy_info_ptr->box_type;
    oper_status = copy_info_ptr->oper_status;
    type = copy_info_ptr->box_type;
    if(MMISMS_COPY_SMS_TO_ME == oper_status)
    {
        storage = MN_SMS_STORAGE_SIM;
    }
    else if(MMISMS_COPY_SMS_TO_SIM == oper_status)
    {
        storage = MN_SMS_STORAGE_ALL;
    }
    switch (type)        
    {
        case MMISMS_BOX_SENDSUCC://�ѷ�����
        case MMISMS_BOX_SENDFAIL://������
        case MMISMS_BOX_NOSEND://�ݸ���
            first_pos = MMISMS_FindValidMoSMSForMoveAll(type,storage);
            break;
            
        case MMISMS_BOX_MT://�ռ���
            first_pos = MMISMS_FindValidMTSMSForMoveAll(storage);
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY://��ȫ����
            first_pos = MMISMS_FindValidSecuritySMSForMoveAll(storage);
            break;        
#endif  

#ifdef MMI_SMS_CHAT_SUPPORT
        case MMISMS_BOX_CHAT:
            {
                first_pos = MMISMS_FindValidChatSMSForMoveAll(copy_info_ptr->pb_num, storage);
            }
            break;
#endif
        
        default:
            break;
    }        

    if((MMISMS_COPY_SMS_TO_SIM == oper_status) ||
       (MMISMS_COPY_SMS_TO_ME == oper_status))
    {
        if((PNULL != first_pos) &&
           (MMISMS_TYPE_SMS == first_pos->flag.msg_type))
        {
            if(MMISMS_COPY_SMS_TO_SIM == oper_status)
            {
                dual_sys = MMISMS_GetCurSaveDualSys();
                MMISMS_GetSMSMemory(dual_sys, FALSE,&sms_max_num,&sms_used_num);
            }
            else
            {
                MMISMS_GetSMSMemory(MN_DUAL_SYS_1, TRUE,&sms_max_num,&sms_used_num);
            }
            
            if(first_pos->flag.is_concatenate_sms)//�����ţ���ѯĿ����ʿռ��Ƿ��㹻
            {
                sim_long_sms_num++;
                next_long_sms_ptr = first_pos->next_long_sms_ptr;

                while(PNULL != next_long_sms_ptr)
                {
                    sim_long_sms_num++;
                    next_long_sms_ptr = next_long_sms_ptr->next_long_sms_ptr;
                }
                
                if(sim_long_sms_num > (sms_max_num - sms_used_num))
                {
                    first_pos = PNULL;
                    *ret_val =  MMISMS_NO_ENOUGH_SPACE;
                }
            }
            else
            {
                if(MMISMS_COPY_SMS_TO_SIM == oper_status)
                {
                    is_find_free_record = MMISMS_FindFreeRecordId(dual_sys, MN_SMS_STORAGE_SIM, &record_id);
                }
                else
                {
                    is_find_free_record = MMISMS_FindFreeRecordId(dual_sys, MN_SMS_STORAGE_ME, &record_id);
                }

                if(!is_find_free_record)
                {
					first_pos = PNULL;
					*ret_val =  MMISMS_NO_ENOUGH_SPACE;
                }
            }
        }
    }
    return first_pos;    
}
#endif

#ifdef MMI_MSG_RESEND
/*****************************************************************************/
//    Description : 
//    Global resource dependence :
//    Author:Jiaoyou.wu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLatestSendFailMsg(void)
{
    MMISMS_ORDER_ID_T   handle_ptr = s_mmisms_order.mo_sendfail_head_ptr;
    MMISMS_ORDER_ID_T   order_id = PNULL;
    uint32              max_time = 0;
    
    while(PNULL != handle_ptr)
    {
#ifdef MMI_TIMERMSG_SUPPORT
        MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_table = PNULL;
        BOOLEAN  is_timer_msg = FALSE;
        
        event_fast_table = MMISMS_TIMERMSG_GetEventByOrder(handle_ptr);
        if(event_fast_table != PNULL)
        {
            is_timer_msg = TRUE;
        }
#endif        
        if(!(handle_ptr->flag.is_resend) && handle_ptr->time > max_time
            #ifdef MMI_TIMERMSG_SUPPORT
            && !is_timer_msg
            #endif
            )
        {
            order_id = handle_ptr;
            max_time = handle_ptr->time;
        }

        handle_ptr = handle_ptr->next_ptr;        
    }
    return order_id;
}
#endif

/*****************************************************************************/
//    Description : 
//    Global resource dependence :
//    Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetUndownLoadHeadOrder(void)
{
    return s_mmisms_order.mt_undownloaded_head_ptr;
}

#if defined(BT_DIALER_SUPPORT) && (defined(BT_MAP_SUPPORT) || defined(MMI_SMS_SYN_SUPPORT))
/*****************************************************************************/
//     Description : CalculateTotalNum
//    Global resource dependence :  
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void CalculateBtTotalNum(void)
{
    uint32 i = 0;
    
    s_mmisms_order.bt_total_mt_num.total_msg_num = s_mmisms_order.bt_total_undownloaded_num.total_msg_num +
        s_mmisms_order.bt_total_to_be_read_msg_num.total_msg_num +
        s_mmisms_order.bt_total_mt_readed_num.total_msg_num;

#ifdef _ATEST_
#ifdef WIN32
    ATESTOTA_GetTotalMsgNumberInInbox(s_mmisms_order.bt_total_mt_num.total_msg_num);
#endif//WIN32
#endif//_ATEST_

    s_mmisms_order.bt_total_mo_num.total_msg_num = s_mmisms_order.bt_total_nosend_num.total_msg_num + 
        s_mmisms_order.bt_total_sendfail_num.total_msg_num + 
        s_mmisms_order.bt_total_sendsucc_num.total_msg_num;

    s_mmisms_order.bt_total_mt_num.msg_num[0] = s_mmisms_order.bt_total_mt_num.total_msg_num;
#ifndef MMI_MULTI_SIM_SYS_NONE
    s_mmisms_order.bt_total_mt_num.msg_num[1] = 0;
#endif

    s_mmisms_order.bt_total_mt_num.ori_msg_num[0] = s_mmisms_order.bt_total_mt_num.total_msg_num;
#ifndef MMI_MULTI_SIM_SYS_NONE
    s_mmisms_order.bt_total_mt_num.ori_msg_num[1] = 0;
#endif

    s_mmisms_order.bt_total_mt_num.ori_sim_sms_num[0] = 0;
#ifndef MMI_MULTI_SIM_SYS_NONE
    s_mmisms_order.bt_total_mt_num.ori_sim_sms_num[1] = 0;
#endif

    s_mmisms_order.bt_total_mo_num.msg_num[0] = s_mmisms_order.bt_total_mo_num.total_msg_num;
#ifndef MMI_MULTI_SIM_SYS_NONE
    s_mmisms_order.bt_total_mo_num.msg_num[1] = 0;
#endif

    s_mmisms_order.bt_total_mo_num.ori_msg_num[0] = s_mmisms_order.bt_total_mo_num.total_msg_num;
#ifndef MMI_MULTI_SIM_SYS_NONE
    s_mmisms_order.bt_total_mo_num.ori_msg_num[1] = 0;
#endif

    s_mmisms_order.bt_total_mo_num.ori_sim_sms_num[0] = 0;
#ifndef MMI_MULTI_SIM_SYS_NONE
    s_mmisms_order.bt_total_mo_num.ori_sim_sms_num[1] = 0;
#endif

}

/*****************************************************************************/
//     Function name: MMISMS_InsertSmsToOrder()
//  Description :  ����һ����Ϣ��s_mmisms_order
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertBtSmsToOrder (MMISMS_STATE_T *order_info_ptr)
{
    MMISMS_STATE_T **insert_type_head_ptr = PNULL;
    MMISMS_STATE_T *new_state_ptr = PNULL;
    MMISMS_STATE_T *long_sms_state_ptr = PNULL;
    MMISMS_TYPE_E type = MMISMS_NO_TYPE ;
    MMISMS_MSG_NUM_T *total_num_ptr = PNULL;
    MMISMS_ORDER_ID_T read_long_sms_id = PNULL;
    MMISMS_ORDER_ID_T first_long_sms_id = PNULL;
    MMISMS_ORDER_ID_T cur_sms_id = PNULL;
    BOOLEAN is_to_be_read = FALSE;
    uint32 total_msg_num = 0;
    uint32 i = 0;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_BT;
    
    if (PNULL == order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_InsertSmsToOrder order_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2028_112_2_18_2_53_57_603,(uint8*)"");
        return FALSE;
    }

    //folder_type = order_info_ptr->flag.folder_type;

    for(i=MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        total_msg_num = MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_undownloaded_head_ptr) + MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_unread_head_ptr)
            + MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mt_readed_head_ptr) + MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mo_draft_head_ptr)
            + MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mo_sendfail_head_ptr) + MMISMS_GetBtSmsSize(&s_mmisms_order.bt_mo_sendsucc_head_ptr);
#endif
 
    //������ȡ���Ʒ�Χ������ȡ
        if (i == order_info_ptr->flag.dual_sys
        && MMISMS_TYPE_SMS == order_info_ptr->flag.msg_type
        && MN_SMS_STORAGE_SIM == order_info_ptr->flag.storage
        && (total_msg_num >= MMISMS_GetMaxSimSmsNum((MN_DUAL_SYS_E)i)))
        {
            //SCI_TRACE_LOW:"MMISMS_InsertSmsToOrder sim%d sms exceed(%d)!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2042_112_2_18_2_53_57_604,(uint8*)"dd", (i+1), s_mmisms_order.total_mo_num.ori_sim_sms_num[i] + s_mmisms_order.total_mt_num.ori_sim_sms_num[i]);
            return FALSE;
        }
    }
    
    if (total_msg_num >= s_mmisms_order.max_sms_num.total_msg_num)//�ռ���
    {
        //SCI_TRACE_LOW:"MMISMS_InsertSmsToOrder (%d)MEMORY FULL, RETURN!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2049_112_2_18_2_53_57_605,(uint8*)"d", total_msg_num);
        return FALSE;
    }
	
    //����LinkedArrayAllocNode����order_info_ptr�е����ݱ���������
    new_state_ptr = LinkedArrayAllocNode();
    if ( PNULL == new_state_ptr)  //no space to alloc
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2057_112_2_18_2_53_57_606,(uint8*)"");
        return FALSE;
    }
    else //�ѷ���洢�ռ䣬��������δ��������
    {
        SCI_MEMSET(new_state_ptr, 0, sizeof(MMISMS_STATE_T));
    }

    SCI_MEMCPY(new_state_ptr,order_info_ptr,sizeof(MMISMS_STATE_T));
    new_state_ptr->next_ptr = PNULL;
    new_state_ptr->prev_ptr = PNULL;
    new_state_ptr->next_chat_ptr = PNULL;
    new_state_ptr->prev_chat_ptr = PNULL;
    new_state_ptr->next_long_sms_ptr = PNULL;
    //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder new_state_ptr->flag.mo_mt_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2071_112_2_18_2_53_57_607,(uint8*)"d", new_state_ptr->flag.mo_mt_type);

    switch(new_state_ptr->flag.mo_mt_type)
    {
    case MMISMS_MT_NOT_DOWNLOAD:
    case MMISMS_MT_NOT_NOTIFY:
    case MMISMS_MT_NEED_NOT_DOWNLOAD:
        type = MMISMS_MT_TYPE;
        
        insert_type_head_ptr = &s_mmisms_order.bt_mt_undownloaded_head_ptr;
        total_num_ptr = &(s_mmisms_order.bt_total_undownloaded_num);
        break;
        
    case MMISMS_MT_TO_BE_READ:
    case MMISMS_MT_SR_TO_BE_READ:
        is_to_be_read = TRUE;
        type = MMISMS_MT_TYPE;
        {
            read_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.bt_mt_readed_head_ptr, new_state_ptr, MMISMS_MT_TYPE);
            if (read_long_sms_id != PNULL) //���Ѷ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //���������Ѷ������еĳ�����״̬��Ϊδ��,ʱ��ͳһΪ��һ������ĳ����ŵ�ʱ��;
                cur_sms_id = read_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MT_TO_BE_READ;
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2172_112_2_18_2_53_58_609,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0 , sizeof(MMISMS_STATE_T));
                    }
                    
                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));
                    for(i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
                    {
                        if( cur_sms_id == g_mmisms_global.operation.cur_order_id_arr[i] )
                        {
                            g_mmisms_global.operation.cur_order_id_arr[i] = long_sms_state_ptr;
                        }
                    }

#ifdef MMI_SMS_CHAT_SUPPORT                    
                    DeleteNodeFromChatOrder(cur_sms_id,type);
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.bt_mt_readed_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.bt_total_mt_readed_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num)); //���Ѷ�������ɾ����

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;

                    InsertNode(TRUE,
                                    &(s_mmisms_order.bt_mt_unread_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MT_TYPE,
                                    &(s_mmisms_order.bt_total_to_be_read_msg_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num));  //���뵽δ��������
#ifdef MMI_SMS_CHAT_SUPPORT                                    
                    InsertNodeToChatOrder(long_sms_state_ptr,type);
#endif
                }
                
                insert_type_head_ptr = &s_mmisms_order.bt_mt_unread_head_ptr;                
            }
            else //���Ѷ�������û���ҵ����²����sms ref��ͬ��Ԫ��,��δ�������в���
            {
                insert_type_head_ptr = &s_mmisms_order.bt_mt_unread_head_ptr;
            }    
            total_num_ptr = &(s_mmisms_order.bt_total_to_be_read_msg_num);

        }
        break;
        
    case MMISMS_MT_HAVE_READ:
    case MMISMS_MT_SR_HAVE_READ:
        type = MMISMS_MT_TYPE;

        insert_type_head_ptr = &s_mmisms_order.bt_mt_readed_head_ptr;  
        total_num_ptr = &(s_mmisms_order.bt_total_mt_readed_num);
        break;
        
    case MMISMS_MO_SEND_SUCC:
        type = MMISMS_MO_TYPE;
        {

            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.bt_mo_sendfail_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ���ʧ���������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //�²���ĳ�����״̬��Ϊ����ʧ��
                new_state_ptr->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendfail_head_ptr; 
                total_num_ptr = &(s_mmisms_order.bt_total_sendfail_num);
            }
            else
            {
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendsucc_head_ptr;
                total_num_ptr = &(s_mmisms_order.bt_total_sendsucc_num);
            }
 
        }
        break;
        
    case MMISMS_MO_SEND_FAIL:                        
        type = MMISMS_MO_TYPE; 
        {


            first_long_sms_id =  FindNodeInLinkByRef(s_mmisms_order.bt_mo_sendsucc_head_ptr, new_state_ptr, MMISMS_MO_TYPE);
            if (first_long_sms_id != PNULL) //�ڷ��ͳɹ��������ҵ������²����sms ref��ͬ��Ԫ��
            {
                //�������ڷ��ͳɹ������еĳ�����״̬��Ϊ����ʧ��
                cur_sms_id = first_long_sms_id;    
                while (cur_sms_id != PNULL)
                {
                    cur_sms_id->flag.mo_mt_type = MMISMS_MO_SEND_FAIL;  
                    long_sms_state_ptr = LinkedArrayAllocNode();
                    if ( PNULL == long_sms_state_ptr)  //no space to alloc
                    {
                        //SCI_TRACE_LOW:"MMISMS: MMISMS_InsertSmsToOrder no space to alloc!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2361_112_2_18_2_53_58_611,(uint8*)"");
                        return FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(long_sms_state_ptr, 0, sizeof(MMISMS_STATE_T));
                    }
                    
                    SCI_MEMCPY(long_sms_state_ptr,(MMISMS_STATE_T *)cur_sms_id,sizeof(MMISMS_STATE_T));

#ifdef MMI_SMS_CHAT_SUPPORT
                    //if (MMISMS_SENDSTATUS_SENDING != cur_sms_id->backsend_status && MMISMS_SENDSTATUS_WAITING != cur_sms_id->backsend_status)
                    {
                        DeleteNodeFromChatOrder(cur_sms_id,type);
                    }
                    
#endif
                    DeleteNode(FALSE,
                                    &(s_mmisms_order.bt_mo_sendsucc_head_ptr),
                                    cur_sms_id,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.bt_total_sendsucc_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num)); //�ӷ��ͳɹ�������ɾ����

                    cur_sms_id = long_sms_state_ptr->next_long_sms_ptr;
                    long_sms_state_ptr->next_long_sms_ptr = PNULL;
                    long_sms_state_ptr->prev_ptr = PNULL;
                    long_sms_state_ptr->next_ptr = PNULL;

                    InsertNode(FALSE,
                                    &(s_mmisms_order.bt_mo_sendfail_head_ptr),
                                    long_sms_state_ptr,
                                    MMISMS_MO_TYPE,
                                    &(s_mmisms_order.bt_total_sendfail_num),
                                    (s_mmisms_order.bt_to_be_read_msg_num));    //���뵽����ʧ�������� 
#ifdef MMI_SMS_CHAT_SUPPORT 
                    //if (MMISMS_SENDSTATUS_SENDING != long_sms_state_ptr->backsend_status && MMISMS_SENDSTATUS_WAITING != long_sms_state_ptr->backsend_status)
                    {
                        InsertNodeToChatOrder(long_sms_state_ptr,type);
                    }                    
#endif
                }
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendfail_head_ptr;
            }
            else 
            {
                insert_type_head_ptr = &s_mmisms_order.bt_mo_sendfail_head_ptr;
            }             
            total_num_ptr = &(s_mmisms_order.bt_total_sendfail_num);

        }
        break;
        
    case MMISMS_MO_DRAFT:    
        type = MMISMS_MO_TYPE;
        {

            insert_type_head_ptr = &s_mmisms_order.bt_mo_draft_head_ptr;
            total_num_ptr = &(s_mmisms_order.bt_total_nosend_num);

        }
        break;
        
    default:
        return FALSE;
    }

    {
        if (is_to_be_read)
        {
            InsertNode(TRUE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.bt_to_be_read_msg_num));
        }
        else
        {
            InsertNode(FALSE,
                            insert_type_head_ptr,
                            new_state_ptr,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.bt_to_be_read_msg_num));
        }
    }

#ifdef MMI_SMS_CHAT_SUPPORT
    //insert to chat link
    //if (MMISMS_SENDSTATUS_SENDING != new_state_ptr->backsend_status && MMISMS_SENDSTATUS_WAITING != new_state_ptr->backsend_status)
    {
        InsertNodeToChatOrder(new_state_ptr,type);
    }
    
#endif

    CalculateBtTotalNum();
    
    return TRUE;
}

/*****************************************************************************/
//     Description : update the status info from unread to read for bt sms,and order the mt sms again.
//    Global resource dependence : s_mmisms_order
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_UpdateBtSmsInfoInOrder(
                                           MMISMS_ORDER_ID_T      old_order_id,  
                                           MMISMS_STATE_T        *new_order_info_ptr
                                           )
{
    if (PNULL == old_order_id || PNULL == new_order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_UpdateSmsInfoInOrder old_order_id new_order_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_2518_112_2_18_2_53_58_613,(uint8*)"");
        return FALSE;
    }  
    MMISMS_DelBtSmsInfoInOrder(old_order_id);
    MMISMS_InsertBtSmsToOrder(new_order_info_ptr);

    return TRUE;    
}

/*****************************************************************************/
//  Function name: MMISMS_GetBtBoxMtSize ()
//  Description :  ����head_ptrȡ��BT�������Ϣ����
//  Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetBtSmsSize(
                                  MMISMS_STATE_T **head_ptr    //[IN]
                                  )
{
    MMISMS_STATE_T *cur_ptr = PNULL;
    uint16 count = 0;

    if (PNULL == head_ptr || PNULL == *head_ptr) 
    {
        SCI_TRACE_LOW("MMISMS_GetBtSmsSize head_ptr is PNULL");
    }
    else
    {
        cur_ptr = *head_ptr;

        while(cur_ptr != PNULL)
        {
            ++count;
            cur_ptr = cur_ptr ->next_ptr;
        }
    }
    
    return count;
}

#if defined(BT_DIALER_SUPPORT) && (defined(BT_MAP_SUPPORT) || defined(MMI_SMS_SYN_SUPPORT))
/*****************************************************************************/
//     Function name: MMISMS_DelSmsInfoInOrder ()
//  Description :  ??pos,??s_mmisms_order???????
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DelBtSmsInfoInOrder(
                                        MMISMS_ORDER_ID_T order_id  //[IN] 
                                        )
{
    MMISMS_STATE_T **delete_type_head_ptr = PNULL;
    MMISMS_TYPE_E  type = MMISMS_NO_TYPE;
    MMISMS_STATE_T old_sms_state = {0};
    MMISMS_MSG_NUM_T *total_num_ptr = PNULL;
    BOOLEAN is_to_be_read = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_BT;
    
    if(PNULL == order_id)
    {
        return FALSE;
    }
    
    MMISMS_GetSmsInfoFromOrder(order_id, &old_sms_state);
    folder_type = old_sms_state.flag.folder_type;

    SCI_TRACE_LOW("[MMISMS] MMISMS_DelBtSmsInfoInOrder old_sms_state.flag.mo_mt_type = %d, folder_type = %d", old_sms_state.flag.mo_mt_type, folder_type);

    switch(old_sms_state.flag.mo_mt_type)
    {
    case MMISMS_MT_NOT_DOWNLOAD:
    case MMISMS_MT_NOT_NOTIFY:
    case MMISMS_MT_NEED_NOT_DOWNLOAD:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_mt_undownloaded_head_ptr;    
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_undownloaded_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.bt_mt_undownloaded_head_ptr;    
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.bt_total_undownloaded_num);
        }
        break;
        
    case MMISMS_MT_TO_BE_READ:
    case MMISMS_MT_SR_TO_BE_READ:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            is_to_be_read = TRUE;
            delete_type_head_ptr = &s_mmisms_order.security_mt_unread_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_to_be_read_msg_num);
        }
        else
        {
            is_to_be_read = TRUE;
            delete_type_head_ptr = &s_mmisms_order.bt_mt_unread_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.bt_total_to_be_read_msg_num);
        }
        break;
        
    case MMISMS_MT_HAVE_READ:
    case MMISMS_MT_SR_HAVE_READ:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_mt_readed_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.bt_mt_readed_head_ptr;
            type = MMISMS_MT_TYPE;
            total_num_ptr = &(s_mmisms_order.bt_total_mt_readed_num);
        }
        break;
        
    case MMISMS_MO_SEND_SUCC:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_sendsucc_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.bt_mo_sendsucc_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.bt_total_sendsucc_num);
        }
        break;
        
    case MMISMS_MO_SEND_FAIL:
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_sendfail_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.bt_mo_sendfail_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.bt_total_sendfail_num);
        }
        break;
        
    case MMISMS_MO_DRAFT:  
        if(MMISMS_FOLDER_SECURITY == folder_type)
        {
            delete_type_head_ptr = &s_mmisms_order.security_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.security_total_nosend_num);
        }
        else
        {
            delete_type_head_ptr = &s_mmisms_order.bt_mo_draft_head_ptr;
            type = MMISMS_MO_TYPE;
            total_num_ptr = &(s_mmisms_order.bt_total_nosend_num);
        }
        break;
        
    default:
        break;
    }

#ifdef MMI_SMS_CHAT_SUPPORT
    DeleteNodeFromChatOrder(order_id,type);
#endif
    
    if(MMISMS_FOLDER_SECURITY == folder_type)
    {
        if (is_to_be_read)
        {
            DeleteNode(TRUE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.security_to_be_read_msg_num));        
        }
        else
        {
            DeleteNode(FALSE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.security_to_be_read_msg_num));
        }
    }
    else
    {
        if (is_to_be_read)
        {
            DeleteNode(TRUE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.bt_to_be_read_msg_num));        
        }
        else
        {
            DeleteNode(FALSE,
                            delete_type_head_ptr,
                            order_id,
                            type,
                            total_num_ptr,
                            (s_mmisms_order.bt_to_be_read_msg_num));
        }
    }

    CalculateBtTotalNum();
        
    return TRUE;
}
#endif

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related BT MT SMS box
//    Global  resource dependence : 
//  Author: Naomi Huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidBTMTSMSForDelAll(void)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    order_id = s_mmisms_order.bt_mt_undownloaded_head_ptr;
    
    while(order_id != PNULL)//is  marked
    {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
        {
            if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        else
#endif
        if (order_id->flag.is_marked && (!order_id->flag.is_locked) )
        {
            break;
        }
        else
        {
            order_id = order_id->next_ptr;
        }
    }
    if(order_id == PNULL) 
    {
        order_id = s_mmisms_order.bt_mt_unread_head_ptr;
        
        while(order_id != PNULL)//is  marked
        {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
            if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
            {
                if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }            
            }
            else
#endif
            if (order_id->flag.is_marked && (!order_id->flag.is_locked) )
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }
        }
        if (order_id == PNULL)
        {
            order_id = s_mmisms_order.bt_mt_readed_head_ptr;
            while(order_id != PNULL)//is  marked
            {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
                if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
                {
                    if(order_id->flag.is_marked && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) ) //is  marked
                    {
                        break;
                    }
                    else
                    {
                        order_id = order_id->next_ptr;
                    }            
                }
                else
#endif
                if (order_id->flag.is_marked && (!order_id->flag.is_locked) )
                {
                    break;
                }
                else
                {
                    order_id = order_id->next_ptr;
                }
            }
        }        
    }
    return order_id;    
}

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related BT MO SMS box
//    Global resource dependence : 
//  Author: Naomi Huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidBTMoSMSForDelAll(MMISMS_BOX_TYPE_E box_type )
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    
    switch(box_type)
    {
    case MMISMS_BOX_SENDSUCC:
        order_id = s_mmisms_order.mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BOX_SENDFAIL:
        order_id = s_mmisms_order.mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BOX_NOSEND:
        order_id = s_mmisms_order.mo_draft_head_ptr;
        break;
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)    
    case MMISMS_BT_BOX_SENDSUCC:
        order_id = s_mmisms_order.bt_mo_sendsucc_head_ptr;
        break;
        
    case MMISMS_BT_BOX_SENDFAIL:
        order_id = s_mmisms_order.bt_mo_sendfail_head_ptr;
        break;
        
    case MMISMS_BT_BOX_NOSEND:
        order_id = s_mmisms_order.bt_mo_draft_head_ptr;
        break;
#endif
    default:
        break;
    }

    while (PNULL != order_id)
    {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
        if(MMISMS_TYPE_MAIL == order_id->flag.msg_type)
        {
            if(order_id->flag.is_marked
                && (!mail_GetMailLockFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) //is  marked
                && (!mail_GetMailSendingFlag((uint32)mail_getMsgIdByRecordId(order_id->record_id))) )
            {
                break;
            }
            else
            {
                order_id = order_id->next_ptr;
            }            
        }
        else
#endif
        if(order_id->flag.is_marked && (!order_id->flag.is_locked) ) //is  marked
        {
            break;
        }
        else
        {
            order_id = order_id->next_ptr;
        }            
    }    
    
    return order_id ;
}
/*****************************************************************************/
// 	Description : Update BT Unread SMS Order list
//	Global resource dependence : s_mmisms_order
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMISMS_UpdateBtUnreadOrder(void)
{
    MMISMS_STATE_T *unread_state_ptr = PNULL;
    MMISMS_ORDER_ID_T order_id[MMISMS_SPLIT_MAX_NUM] = {PNULL};

    //SCI_TRACE_LOW:"MMISMS_UpdateUnreadOrder:is_need_update_order=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_6104_112_2_18_2_54_5_653,(uint8*)"d", g_mmisms_global.is_need_update_order);

    if (g_mmisms_global.is_need_update_order)
    {
        MMISMS_SetNeedSort(TRUE);

        g_mmisms_global.is_need_update_order = FALSE;

        if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
        {
            unread_state_ptr = s_mmisms_order.security_mt_unread_head_ptr;
        }
        else
        {
            unread_state_ptr = s_mmisms_order.bt_mt_unread_head_ptr;
        }

        while(unread_state_ptr)
        {
            uint32 i = 0;
            uint32 j= 0;
            BOOLEAN is_need_update = FALSE;

            SCI_MEMSET(order_id, 0x00, (sizeof(MMISMS_ORDER_ID_T)*MMISMS_SPLIT_MAX_NUM));

            is_need_update = FALSE;

            if (MMISMS_TYPE_SMS == unread_state_ptr->flag.msg_type)//SMS
            {
                if (unread_state_ptr->flag.is_concatenate_sms)//????
                {
                    MMISMS_ORDER_ID_T cur_order_id = PNULL;

                    cur_order_id = unread_state_ptr;

                    while (cur_order_id != PNULL)
                    {
                        if (cur_order_id->flag.is_need_update)
                        {
                            order_id[i++] = cur_order_id;
                            cur_order_id  = cur_order_id->next_long_sms_ptr;
                            is_need_update = TRUE;
                        }
                        else//???????????????????,???????
                        {
                            is_need_update = FALSE;
                            break;
                        }
                    }
                }
                else if (unread_state_ptr->flag.is_need_update)//?????
                {
                    order_id[i++] = unread_state_ptr;
                    is_need_update = TRUE;
                }
                else
                {
                    is_need_update = FALSE;
                }
            }

            if (is_need_update)
            {
                for (j = 0; j < i; j++)
                {
                    MMISMS_STATE_T order_info = {0};

                    MMISMS_GetSmsInfoFromOrder (
                                                order_id[j], 
                                                &order_info 
                                                );

                    order_id[j]->flag.is_need_update = FALSE;
                    order_info.flag.is_need_update = FALSE;

                    // to modify the status in the order list and order the mt sms again.
#ifdef MRAPP_SUPPORT
                    if(MMISMS_MT_TO_BE_READ == order_info.flag.mo_mt_type)
                    {
                        order_info.flag.mo_mt_type = MMISMS_MT_HAVE_READ;
                    }
                    else if(MMISMS_MT_SR_TO_BE_READ == order_info.flag.mo_mt_type)
                    {
                        order_info.flag.mo_mt_type = MMISMS_MT_SR_HAVE_READ;
                    }
#else
                    order_info.flag.mo_mt_type = (MMISMS_MT_TO_BE_READ == order_info.flag.mo_mt_type) ? MMISMS_MT_HAVE_READ : MMISMS_MT_SR_HAVE_READ;
#endif

                    order_info.flag.is_receive_all = FALSE;

                    MMISMS_UpdateBtSmsInfoInOrder(order_id[j], &order_info);
#ifdef MMI_SMS_CHAT_SUPPORT
                    SetChatOrderUnitNum();
#endif
                }

                //MMISMS_HandleOrderChangeCallBack();
                MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);

                if(MMISMS_FOLDER_NORMAL == g_mmisms_global.folder_type)
                {
                    unread_state_ptr = s_mmisms_order.mt_unread_head_ptr;
                    continue;
                }
                else if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
                {
                    unread_state_ptr = s_mmisms_order.security_mt_unread_head_ptr;
                    continue;
                }                
                else if(MMISMS_FOLDER_BT == g_mmisms_global.folder_type)
                {
                    unread_state_ptr = s_mmisms_order.bt_mt_unread_head_ptr;
                    continue;
                }
            }

            unread_state_ptr = unread_state_ptr->next_ptr;
        }
    }
}

/*****************************************************************************/
//  Description : Get Update Sms Num
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUpdateBtSMSNum(MN_DUAL_SYS_E dual_sys)
{
    uint32 update_sms_num = 0;

    if (g_mmisms_global.is_need_update_order)
    {
        MMISMS_STATE_T *unread_state_ptr = PNULL;

        unread_state_ptr = s_mmisms_order.bt_mt_unread_head_ptr;
        //unread_state_ptr = s_mmisms_order.mt_unread_head_ptr;

        while (PNULL != unread_state_ptr)
        {
            BOOLEAN is_need_update = FALSE;

            if (MMISMS_TYPE_SMS == unread_state_ptr->flag.msg_type
                && (dual_sys >= MMI_DUAL_SYS_MAX || unread_state_ptr->flag.dual_sys == dual_sys))
            {
                if (unread_state_ptr->flag.is_concatenate_sms)//????
                {
                    MMISMS_ORDER_ID_T cur_order_id = PNULL;

                    cur_order_id = unread_state_ptr;

                    while (PNULL != cur_order_id)
                    {
                        if (cur_order_id->flag.is_need_update)
                        {
                            cur_order_id  = cur_order_id->next_long_sms_ptr;
                            is_need_update = TRUE;
                        }
                        else//???????????????????,???????
                        {
                            is_need_update = FALSE;
                            break;
                        }
                    }
                }
                else if (unread_state_ptr->flag.is_need_update)//?????
                {
                    is_need_update = TRUE;
                }
            }

            if (is_need_update)
            {
                update_sms_num++;
            }

            unread_state_ptr = unread_state_ptr->next_ptr;
        }
    }

    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetUpdateSMSNum update_sms_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_ORDER_11261_112_2_18_2_54_17_705,(uint8*)"d", update_sms_num);

    return update_sms_num;    
}

/*****************************************************************************/
//     Description : delete all bt mails
//  Author:
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DeleteAllBTMail(void)
{
    MMI_STRING_T prompt_str = {0};
    
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    uint8 type = 0;
    uint16 index = 0;
    uint32 i = 0;
    uint16 msg_type=MMISMS_TYPE_MAX;
    uint16 bt_msg_num = 0;

    SCI_TRACE_LOW("[MMISMS] MMISMS_DeleteAllBTMail");
    MMIAPISMS_SetCurOperIsPush(FALSE);

    if (MMK_IsOpenWin(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID) || MMK_IsOpenWin(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID)
        || MMK_IsOpenWin(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID) || MMK_IsOpenWin(MMISMS_BT_MTBOX_CHILD_WIN_ID) 
        || (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID) && (MMIMSG_MAIN_MENU_WIN_ID == MMK_GetFocusWinId())))
    {
        if(MMIAPISMS_GetCurOperIsBoot() || MMIAPISMS_GetCurOperIsDisconnect())
        {
            MMIAPISMS_SetCurOperIsDisconnect(FALSE);
            MMIAPISMS_SetCurOperIsBoot(FALSE);

            bt_msg_num = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BT_BOX_ALL);
            SCI_TRACE_LOW("[MMISMS] bt_msg_num = %d", bt_msg_num);
            
            if (bt_msg_num > 0)
            {    
                MMISMS_SetDelAllFlag(TRUE);
                MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
                MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleDelAllBtMsgWaitWinMsg);
                MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL,0);
            }
        }
    }
}

/*****************************************************************************/
//  Description : to handle the message of delete waiting window    
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleDelAllBtMsgWaitWinMsg(
                                              MMI_WIN_ID_T        win_id,    //IN:
                                              MMI_MESSAGE_ID_E    msg_id,    //IN:
                                              DPARAM                param    //IN:
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_ORDER_ID_T first_pos = PNULL;
    MMISMS_BOX_TYPE_E bt_box_type = 0;
    MMISMS_DELMSG_MSGTYPE_TYPE_E del_msg_type = MMISMS_DELMSG_TYPE_SMS;

    BOOLEAN is_right = FALSE;
    uint16 type = 0;
    uint16 total_del_msg_num = 0;
    uint8 count = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MMISMS_MSG_DELETE_NEXT:  
        //����BT��Ϣ����ɾ��
        
        for(type = 0; type < SMS_BOX_DELNUM_MAX; type++)
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_DeleteAllBTMail MMISMS_MSG_DELETE_NEXT type = %d", type);
            switch(type)
            {
            case SMS_BT_INBOX:
                {
                    first_pos = s_mmisms_order.bt_mt_undownloaded_head_ptr;
                    if(first_pos == PNULL) 
                    {
                        first_pos = s_mmisms_order.bt_mt_unread_head_ptr;
                        if (first_pos == PNULL)
                        {
                            first_pos = s_mmisms_order.bt_mt_readed_head_ptr;

                        }
                    }

                    bt_box_type = MMISMS_BT_BOX_MT;
                }
                break;
            case SMS_BT_OUTBOX:
                {
                    first_pos = s_mmisms_order.bt_mo_sendfail_head_ptr;
                    bt_box_type = MMISMS_BT_BOX_SENDFAIL;
                }
                break;
            case SMS_BT_DRAFT:
                {
                    first_pos = s_mmisms_order.bt_mo_draft_head_ptr;
                    bt_box_type = MMISMS_BT_BOX_NOSEND;
                }
                break;
            case SMS_BT_SENDBOX:
                {
                    first_pos = s_mmisms_order.bt_mo_sendsucc_head_ptr;
                    bt_box_type = MMISMS_BT_BOX_SENDSUCC;
                }
                break;
            default:
                break;
            }

            if(PNULL != first_pos)
            {
                first_pos->flag.folder_type = MMISMS_FOLDER_BT;
                MMISMS_DeleteReqToMN(bt_box_type, first_pos, MMISMS_DELETE_MSG);
                break;
            }
            else
            {
                count++;
            }
        }

        if(count == SMS_BOX_DELNUM_MAX)
        {
            MMISMS_SetDelAllFlag(FALSE);
            MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_DeleteAllBTMail MSG_CLOSE_WINDOW");
            CalculateBtTotalNum();
            MMIAPISMS_ClearTableList();
            MMISMS_HandleUpdateListMsg();
        }        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        SCI_TRACE_LOW("[MMISMS] MMISMS_DeleteAllBTMail MSG_CTL_CANCEL");
        MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//     Description : MMISMS_HandleDelAllMsg
//  Author: naomi huang
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_HandleUpdateListMsg(void)
{
    SCI_TRACE_LOW("[MMISMS] MMISMS_HandleUpdateListMsg");

#if 0
    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        if (MMIMSG_MAIN_MENU_WIN_ID!=MMK_GetFocusWinId())
        {
            MMK_SendMsg(MMIMSG_MAIN_MENU_WIN_ID,
                MMISMS_MSG_NEEDUPDATEMENU,
                PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����MENU��            
        }
        else
        {
            MMK_SendMsg(MMIMSG_MAIN_MENU_WIN_ID, 
                MMISMS_MSG_UPDATEMENU, 
                PNULL);    //֪ͨ��Ӧ�Ĵ��ڣ�����MENU��            
        }
    }
#endif

    if (MMK_IsOpenWin(MMISMS_BT_MTBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_MTBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }
    
    if (MMK_IsOpenWin(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }

    if (MMK_IsOpenWin(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }

    if (MMK_IsOpenWin(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID))
    {
        MMK_SendMsg(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
    }
}

/*****************************************************************************/
//     Description : delete all bt mails
//  Author:
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DeleteBTMail(void)
{
    MMI_STRING_T prompt_str = {0};
    
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    uint8 type = 0;
    uint16 index = 0;
    uint32 i = 0;
    uint16 msg_type=MMISMS_TYPE_MAX;

    SCI_TRACE_LOW("[MMISMS] MMISMS_DeleteBTMail");
    MMIAPISMS_SetCurOperIsPush(FALSE);
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleDelBtMsgWaitWinMsg);
    MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL,0);
}


/*****************************************************************************/
//  Description : to handle the message of delete waiting window    
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleDelBtMsgWaitWinMsg(
                                              MMI_WIN_ID_T        win_id,    //IN:
                                              MMI_MESSAGE_ID_E    msg_id,    //IN:
                                              DPARAM                param    //IN:
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_ORDER_ID_T first_pos = PNULL;
    MMISMS_BOX_TYPE_E bt_box_type = 0;
    MMISMS_DELMSG_MSGTYPE_TYPE_E del_msg_type = MMISMS_DELMSG_TYPE_SMS;

    BOOLEAN is_right = FALSE;
    uint16 type = 0;
    uint16 total_del_msg_num = 0;
    uint8 count = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MMISMS_MSG_DELETE_NEXT:  
        //����BT��Ϣ����ɾ��
        type = MMISMS_GetCurBoxType();
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelBtMsgWaitWinMsg MMISMS_MSG_DELETE_NEXT type = %d", type);
            switch(type)
            {
            case MMISMS_BT_BOX_MT:
                {
                    first_pos = s_mmisms_order.bt_mt_undownloaded_head_ptr;
                    if(first_pos == PNULL) 
                    {
                        first_pos = s_mmisms_order.bt_mt_unread_head_ptr;
                        if (first_pos == PNULL)
                        {
                            first_pos = s_mmisms_order.bt_mt_readed_head_ptr;

                        }
                    }

                    bt_box_type = MMISMS_BT_BOX_MT;
                }
                break;
            case MMISMS_BT_BOX_SENDFAIL:
                {
                    first_pos = s_mmisms_order.bt_mo_sendfail_head_ptr;
                    bt_box_type = MMISMS_BT_BOX_SENDFAIL;
                }
                break;
            case MMISMS_BT_BOX_NOSEND:
                {
                    first_pos = s_mmisms_order.bt_mo_draft_head_ptr;
                    bt_box_type = MMISMS_BT_BOX_NOSEND;
                }
                break;
            case MMISMS_BT_BOX_SENDSUCC:
                {
                    first_pos = s_mmisms_order.bt_mo_sendsucc_head_ptr;
                    bt_box_type = MMISMS_BT_BOX_SENDSUCC;
                }
                break;
            default:
                break;
            }

            if(PNULL != first_pos)
            {
                first_pos->flag.folder_type = MMISMS_FOLDER_BT;
                MMISMS_DeleteReqToMN(bt_box_type, first_pos, MMISMS_DELETE_MSG);
                break;
            }
            else
            {
            MMISMS_SetDelAllFlag(FALSE);
            MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelBtMsgWaitWinMsg MSG_CLOSE_WINDOW");
            CalculateBtTotalNum();
            MMISMS_HandleUpdateListMsg();
        }        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelBtMsgWaitWinMsg MSG_CTL_CANCEL");
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}


/*****************************************************************************/
//     Description : delete all bt inbox mails
//  Author:
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DeleteBTInboxMail(void)
{
    MMI_STRING_T prompt_str = {0};
    
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
    uint8 type = 0;
    uint16 index = 0;
    uint32 i = 0;
    uint16 msg_type=MMISMS_TYPE_MAX;

    SCI_TRACE_LOW("[MMISMS] MMISMS_DeleteBTInboxMail");
    MMIAPISMS_SetCurOperIsPush(FALSE);
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMISMS_HandleDelBtInboxMsgWaitWinMsg);
    MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL,0);
}


/*****************************************************************************/
//  Description : to handle the message of delete waiting window    
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleDelBtInboxMsgWaitWinMsg(
                                              MMI_WIN_ID_T        win_id,    //IN:
                                              MMI_MESSAGE_ID_E    msg_id,    //IN:
                                              DPARAM                param    //IN:
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_ORDER_ID_T first_pos = PNULL;
    MMISMS_BOX_TYPE_E bt_box_type = 0;
    MMISMS_DELMSG_MSGTYPE_TYPE_E del_msg_type = MMISMS_DELMSG_TYPE_SMS;

    BOOLEAN is_right = FALSE;
    uint16 type = 0;
    uint16 total_del_msg_num = 0;
    uint8 count = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MMISMS_MSG_DELETE_NEXT:  
        //����BT��Ϣ����ɾ��
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelBtInboxMsgWaitWinMsg MMISMS_MSG_DELETE_NEXT type = %d", type);
			
			first_pos = s_mmisms_order.bt_mt_undownloaded_head_ptr;
			if(first_pos == PNULL) 
			{
				first_pos = s_mmisms_order.bt_mt_unread_head_ptr;
				if (first_pos == PNULL)
				{
					first_pos = s_mmisms_order.bt_mt_readed_head_ptr;
					
				}
			}
			
			bt_box_type = MMISMS_BT_BOX_MT;
            
            if(PNULL != first_pos)
            {
                first_pos->flag.folder_type = MMISMS_FOLDER_BT;
                MMISMS_DeleteReqToMN(bt_box_type, first_pos, MMISMS_DELETE_MSG);
                break;
            }
            else
            {
            MMISMS_SetDelAllFlag(FALSE);
            MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelBtMsgWaitWinMsg MSG_CLOSE_WINDOW");
            CalculateBtTotalNum();
            MMISMS_HandleUpdateListMsg();
        }        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        SCI_TRACE_LOW("[MMISMS] MMISMS_HandleDelBtMsgWaitWinMsg MSG_CTL_CANCEL");
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}


#endif
