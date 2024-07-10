/*************************************************************************
 ** File Name:      mmisms_order.h                                       *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about ordering sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/8/23      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMS_ORDER_H_
#define _MMISMS_ORDER_H_
#define SMSAPP_SRV_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmisms_export.h"
#include "mmisms_app.h"
#include "guilistbox.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
typedef struct
{
    BOOLEAN is_unread_sms;
    BOOLEAN is_unread_mms;
    BOOLEAN is_exist_sms;
    BOOLEAN is_exist_mms;
}MMI_SMS_CHAT_STATUS_T;
#endif

typedef struct MMISMS_OPERATE_DATA_INFO_tag
{
    BOOLEAN is_del_msg_type;
    BOOLEAN is_del_all;
    MMISMS_BOX_TYPE_E    box_type;
    MMISMS_MSG_TYPE_E    msg_type;
}MMISMS_OPERATE_DATA_INFO_T;
#ifdef MMI_SMS_MOVE_SUPPORT
typedef struct MMISMS_MOVE_DATA_INFO_tag
{
    MMISMS_BOX_TYPE_E    box_type;
    MMISMS_OPER_STATUS_E    oper_status;   //move 类型
#ifdef MMI_SMS_CHAT_SUPPORT	    

    MMIPB_BCD_NUMBER_T pb_num;
#endif    
}MMISMS_MOVE_DATA_INFO_T;
#endif
#ifdef MMI_SMS_COPY_SUPPORT
typedef struct MMISMS_COPY_DATA_INFO_tag
{
    MMISMS_BOX_TYPE_E    box_type;
    MMISMS_OPER_STATUS_E    oper_status;   //move 类型
#ifdef MMI_SMS_CHAT_SUPPORT	    
    
    MMIPB_BCD_NUMBER_T pb_num;
#endif    
}MMISMS_COPY_DATA_INFO_T;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get sms box type(exclude security)
//	Global  resource dependence : 
//  Author: liming.deng
//	Note:   
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetSmsBoxtype(MMISMS_STATE_T *sms_state_ptr);

/*****************************************************************************/
//     Function name: MMISMS_InsertMsgToOrder()
//  Description :  插入一条信息到g_mmisms_order
//    Global resource dependence:  g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertSmsToOrder (MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************/
//     Function name: MMISMS_GetMsgInfoFromOrder ()
//  Description :  根据pos，获得g_mmisms_order中存储的SMS的概要信息
//    Global resource dependence :  g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetSmsInfoFromOrder (
                                           MMISMS_ORDER_ID_T order_id, //[IN] 要读取的sms
                                           MMISMS_STATE_T *order_info_ptr //[OUT] Msg的概要信息
                                           );

/*****************************************************************************/
//     Function name: MMISMS_GetBoxTotalNumberFromOrder ()
//  Description :  根据信箱类型，获得该类型信息的总条目
//    Global resource dependence:  g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetBoxTotalNumberFromOrder (MMISMS_BOX_TYPE_E box_type);

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//     Function name: MMISMS_GetBtBoxMtSize ()
//  Description :  根据head_ptr取得BT传输的信息数量
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetBtSmsSize(
                                  MMISMS_STATE_T **head_ptr    //[IN]
                                  );

/*****************************************************************************/
//     Description : delete all bt sync sms.
//  Author: Naomi Huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DeleteAllBTMail(void );

/*****************************************************************************/
//     Description : delete all bt sync sms.
//  Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DeleteBTMail(void );

/*****************************************************************************/
//     Description : delete all bt inbox mails
//  Author:
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DeleteBTInboxMail(void);

#endif

/*****************************************************************************/
//     Function name: MMISMS_DelMsgInfoInOrder ()
//  Description :  根据pos，删除g_mmisms_order中相应的信息。
//    Global resource dependence:  g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DelSmsInfoInOrder(
                                        MMISMS_ORDER_ID_T order_id  //[IN] sms在g_mmisms_order中的数组下标
                                        );

/*****************************************************************************/
//     Function name: MMISMS_FreeSmsOrder ()
//  Description :  释放g_mmisms_order的资源，清空g_mmisms_order。
//    Global resource dependence: g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_FreeSmsOrder (void);

/*****************************************************************************/
//     Function name: MMISMS_GetMaxSmsNumInOrder ()
//  Description :  得到g_mmisms_order中的最大sms数目
//    Global resource dependence: g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetMaxSmsNumInOrder(void);

/*****************************************************************************/
//     Function name: MMISMS_GetUndownloadSmsNumInOrder ()
//  Description :  得到s_mmisms_order中的未下载的mms数目
//    Global resource dependence: s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUndownloadMMSNumInOrder(void);

/*****************************************************************************/
//     Description : to get the next position of mt sms 
//    Global resource dependence : g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurMtNextPos(MMISMS_ORDER_ID_T order_id);

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
                                           );
/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MT SMS box
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMTSMSForDelAll( void );

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MT SMS box
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMoSMSForDelAll(MMISMS_BOX_TYPE_E box_type );

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)   
/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related BT MT SMS box
//    Global  resource dependence : 
//  Author: Naomi Huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidBTMTSMSForDelAll(void);

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related BT MO SMS box
//    Global resource dependence : 
//  Author: Naomi Huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidBTMoSMSForDelAll(MMISMS_BOX_TYPE_E box_type );

/*****************************************************************************/
//  Description : to handle the message of bt delete waiting window    
//  Global resource dependence : 
//  Author: Naomi Huang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleDelAllBtMsgWaitWinMsg(
                                              MMI_WIN_ID_T        win_id,    //IN:
                                              MMI_MESSAGE_ID_E    msg_id,    //IN:
                                              DPARAM                param    //IN:
                                              );

/*****************************************************************************/
//  Description : to handle the message of bt delete waiting window    
//  Global resource dependence : 
//  Author: Naomi Huang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleDelBtMsgWaitWinMsg(
                                              MMI_WIN_ID_T        win_id,    //IN:
                                              MMI_MESSAGE_ID_E    msg_id,    //IN:
                                              DPARAM                param    //IN:
                                              );

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
                                              );

/*****************************************************************************/
//  Description : MMIAPISMS_DelBtMsgByBoxType
//  Global resource dependence : none
//  Author: Naomi Huang
//  Note: 
/***************************************************************************/
//PUBLIC void MMIAPISMS_DelBtMsgByBoxType(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//     Description : MMISMS_HandleUpdateListMsg
//  Author: Naomi Huang
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_HandleUpdateListMsg(void);
#endif

/*****************************************************************************/
//     Description : translate MN status to SF status
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_MO_MT_TYPE_E MMISMS_TranslateMNStatus2SFStatus(                                    //RETURN:
                                                             MN_SMS_STATUS_E mn_status,        //IN:
                                                             MMISMS_TYPE_E type,            //IN:
                                                             BOOLEAN is_sc_addr    //IN: for MT, the parameter will be invalid
                                                             );

/*****************************************************************************/
//     Description : to list sms from the list buffer
//    Global resource dependence : none
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ListSmsFromOrder(MMISMS_BOX_TYPE_E box_type);

 /*****************************************************************************/
//  Description : sort chat message order
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SortChatMessageOrder();
#ifdef MRAPP_SUPPORT
/*****************************************************************************/
//     Description : 根据信箱type，获得信箱中SMS的条数。
//    Global resource dependence : 
//  Author:Tommy.yan
//    Note:
/*****************************************************************************/
PUBLIC int32 MMIMRAPP_GetSMSCountByBoxType(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//     Description : 根据信箱list列表的index，获得信箱中SMS的链表指针id。
//    Global resource dependence : 
//  Author:Toomy.yan
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIMRAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_TYPE_E box_type,
                                                                     int32  list_index);
#endif

#ifdef HERO_ENGINE_SUPPORT 
PUBLIC int32 MMIHEROAPP_GetSMSCountByBoxType(MMISMS_BOX_TYPE_E box_type);

PUBLIC MMISMS_ORDER_ID_T MMIHEROAPP_GetSMSLinkedArrayPtrIdInList(MMISMS_BOX_TYPE_E box_type,
                                                                     int32  list_index);
#endif
/*****************************************************************************/
//     Description : 根据信箱list列表的index，获得信箱的链表指针id。
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_TYPE_E box_type,
                                                          uint32            list_index);


/*****************************************************************************/
//     Description : 插入一条记录到未处理链表。仅用于全部删除sms的时候，收到新的sms。
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertSmsToUnhandleOrder (const MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************/
//     Description : 将未处理短消息链表中的记录插入到正式链表当中。
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertUnhandledSmsToOrder (void);


/*****************************************************************************/
//     Description : 从未处理短消息链表中找到mms push并读取，然后从未处理连表中删除。
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ReadMMSPushFromUnhandledList (void);

/*****************************************************************************/
//     Function name: MMISMS_LoadSmsListFromOrder ()
//  Description :  根据box_type生成控件list;
//    Global resource dependence: g_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_LoadSmsListFromOrder(MMISMS_BOX_TYPE_E box_type, 
                                           MMI_CTRL_ID_T     ctrl_id);

/*****************************************************************************/
//     Description : to delete SMS info from order list by storage
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetOrderIdByStorage(
                                          MMISMS_ORDER_ID_T         *order_id,          //OUT:
                                          MMISMS_MSG_TYPE_E         msg_type,           //IN:
                                          MN_DUAL_SYS_E                dual_sys,           //IN:
                                          MN_SMS_STORAGE_E            storage,            //IN:
                                          int32                     record_id            //IN:
                                          );

/*****************************************************************************/
//     Description : to get the record id of pointed_pos in list box
//    Global resource dependence : s_mmisms_order
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MN_SMS_RECORD_ID_T MMISMS_GetPointedPosRecordID(//RETURN:
                                                MMISMS_BOX_TYPE_E box_type, //IN:
                                                uint16    selected_index    //IN:
                                                );

/*****************************************************************************/
//     Description : the function is called when the order is change
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMISMS_HandleOrderChangeCallBack (void);

/*****************************************************************************/
//     Description : 初始化order list
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMISMS_OrderInit(void);

#if 0
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC  void MMISMS_SetListItemData(MMISMS_BOX_TYPE_E box_type, 
                                    MMI_CTRL_ID_T     ctrl_id,
                                    uint16            index
                                    );
#endif

/*****************************************************************************/
// 	Description : Update Unread SMS Order list
//	Global resource dependence : s_mmisms_order
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMISMS_UpdateUnreadOrder(void);

/*****************************************************************************/
//     Description : get sms box delete available msg num
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetSmsBoxDeleteAvailNum(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//     Description : MMISMS_GetMtBoxMarkedDestNum
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetMtBoxMarkedDestNum(MMISMS_DEST_ADDR_INFO_T *dest_info_ptr);

/*****************************************************************************/
//     Description : MMISMS_SetMtBoxMarkedMMSNum
//    Global resource dependence : 
//  Author: minghu.mao
//    Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMtBoxMarkedDestNum(wchar** num_wstr, uint16 *num_len);

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
                                    );

/*****************************************************************************/
//     Description : to get the count of msg which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadMsgCountEx(MN_DUAL_SYS_E dual_sys, MMISMS_MSG_TYPE_E msg_type);

/*****************************************************************************/
//     Description : to get the count of msg which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadMsgCount(MN_DUAL_SYS_E dual_sys, MMISMS_MSG_TYPE_E msg_type);

/*****************************************************************************/
//     Description : to get the count of undownload msg
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : to get the count of total message which is to be read(包括未读sms，mms，ota，push...)
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : to get the count of mt undownload msg
//    Global resource dependence : s_mmisms_order
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetMTUnDownloadMsgCount(MN_DUAL_SYS_E dual_sys);

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
                                                                                 );

/*****************************************************************************/
//     Description : MMISMS_GetMtBoxMarkedOriginCard
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetMtBoxMarkedOriginCard(void);

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
                                    );

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
                                    );

/*****************************************************************************/
//     Description : get point message number
//    Global resource dependence : nsms1_state_ptr
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetOriginNumFromOrder(
                                                MMISMS_BOX_TYPE_E box_type,
                                                uint8 *number,
                                                uint16 index);

/*****************************************************************************/
//     Description : start from the current index, and find the valid mo sms which
//                   is stored in SIM
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMOSIMSMS(
                                MMISMS_BOX_TYPE_E box_type,
                                MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : start from the current index, and find the valid mt sms which
//                   is stored in SIM
//    Global  resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMTSIMSMS(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get sms state box type
//	Global  resource dependence : 
//  Author: liming.deng
//	Note:   
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetSmsStateBoxtype(MMISMS_STATE_T *sms_state_ptr);

/*****************************************************************************/
//     Description : get the message is security or not
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_FOLDER_TYPE_E MMISMS_CurMsgFolderType(void );

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related Security SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecuritySMSForDelAll(void);

//for delete all msg begin 11/02/20
/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related SMS box
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidBoxMsgForDelAll(MMISMS_BOX_TYPE_E type);

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related msg type
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidTypeMsgForDelAll(MMISMS_MSG_TYPE_E type);

/*****************************************************************************/
//  Description : get number of msg type from security box
//  Global  resource dependence : 
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetMsgNumFromSecurityBoxByType(MMISMS_MSG_TYPE_E msg_type);

/*****************************************************************************/
//  Description : find valid sms or mms to reply
//                  deleting the related msg type
//  Global  resource dependence : 
//  Author: rong.gu
//  Note:对于回复全部信息的情况，查找是否有可以回复的信息
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsHaveValidMsgToReply(void);

//for delete all msg end 11/02/20

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecurityMoSMSForDelAll(MMISMS_BOX_TYPE_E box_type );

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MT SMS box
//    Global  resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSecurityMTSMSForDelAll(void);

/*****************************************************************************/
//     Description : get sms box move available msg num
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetSmsBoxMoveAvailNum(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************/
//     Description : set msg folder type
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurMsgFolderType(
                                MMISMS_ORDER_ID_T cur_order_id,
                                MMISMS_OPER_STATUS_E oper_status);

/*****************************************************************************/
//     Description : start from the current index, and find the valid mo sms which
//                   is stored in SIM
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMOSIMSMS(
                                MMISMS_BOX_TYPE_E box_type,
                                MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : start from the current index, and find the valid mt sms which
//                   is stored in SIM
//    Global  resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidMTSIMSMS(MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
//     Description : get the used_unit_head_ptr
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_LINKED_ORDER_UNIT_T *MMISMS_GetUsedOrderId(void);


/*****************************************************************************/
//  Description : Get Update Sms Num
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUpdateSMSNum(MN_DUAL_SYS_E dual_sys);

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //delete all mail from message list
/*****************************************************************************/
//     Description : get the order id by storage and record_id .
//    Global resource dependence : 
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteAllMail(MMISMS_MSG_TYPE_E msg_type);
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
/*****************************************************************************/
//     Description : set search sms flag
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetSearchFlag(
						 MMISMS_STATE_T *order_info
						 );

/*****************************************************************************/
// 	Description : get sms state box type
//	Global  resource dependence : 
//  Author: liming.deng
//	Note:   
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetSmsStateBoxtype(MMISMS_STATE_T *sms_state_ptr);

/*****************************************************************************/
//     Description : 比较两条短信状态信息，判断是否是同一条长短信的不同部分。
//    Global  resource dependence : 
//  Author: fengming.huang
//    Note:   ref_num,max_num,如果完全相同则返回TRUE. 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CompareSearchSmsState(                                        //RETURN: 
                              MMISMS_STATE_T    *sms1_state_ptr,    //IN: SMS1 state 
                              MMISMS_STATE_T    *sms2_state_ptr     //IN: SMS2 state
                              );

/*****************************************************************************/
//  Description : mark all export message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_SetExportBoxMsgMarked(
                                   MMISMS_BOX_TYPE_E box_type,
                                   BOOLEAN           is_marked //IN:
                                   );

/*****************************************************************************/
//  Description : mark all export message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_SetExportAllBoxMsgMarked( 
                                    BOOLEAN is_security,
                                    BOOLEAN is_marked
                                    );

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                exporting the related SMS box
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidSMSForExportAll(
                                        MMISMS_BOX_TYPE_E box_type,
                                        BOOLEAN is_need_security);

/*****************************************************************************/
//  Description : get export sms num
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportMarkedMsgNum(
                                   MMISMS_BOX_TYPE_E box_type
                                   );

/*****************************************************************************/
//  Description : mark all message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMsgMarked( 
                                BOOLEAN is_marked
                               );

/*****************************************************************************/
//  Description : get move num from storage 
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetMarkedStorageMsgNum(
                                            MMISMS_BOX_TYPE_E box_type,
                                            MN_SMS_STORAGE_E storage
                                           );

/*****************************************************************************/
//  Description : get move num by type  
//  Global resource dependence :
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetMarkedMsgNumByType(
                                            MMISMS_BOX_TYPE_E box_type,
                                            MMISMS_MSG_TYPE_E msg_type
                                           );

/*****************************************************************************/
//     Description :得到name信息。
//    Global resource dependence : none
//  Author:liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_GetNameInfo( 
                              MMISMS_STATE_T *sms_state_info, //IN:
                              MMI_STRING_T      *name,          //OUT:
                              uint16 max_name_len //IN:
                             );

/*****************************************************************************/
//  Description :SmsBCDToPbBCD
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMISMS_SmsBCDToPbBCD(MMISMS_BCD_NUMBER_T sms_num);

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
                           );

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : find whether include Chat sms Staus
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMI_SMS_CHAT_STATUS_T MMISMS_FindChatSMSNumStatusInList(uint16 index);

/*****************************************************************************/
//  Description : 判断当前的聊天号码中是否有正在发送或等待的信息
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
BOOLEAN MMISMS_ChatNumberSendOrWaitIsExsit(MMIPB_BCD_NUMBER_T pb_num);

/*****************************************************************************/
//  Function Name: MMISMS_GetChatNodeTotalUnreadNum
//  Description  : Get ChatNode Total Unread Num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetChatNodeTotalUnreadNum(void);

/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetChatMsgMarkedInList(
                                       uint16 index,
                                       BOOLEAN is_marked //IN:
                                       );

/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurChatMsgMarked(
                                       MMIPB_BCD_NUMBER_T pb_num,
                                       BOOLEAN is_marked //IN:
                                       );

/*****************************************************************************/
//  Description : Get Cur Chat SIMId
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMISMS_GetCurChatSIMId(MMIPB_BCD_NUMBER_T pb_num);

/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetChatAvailDeleteNum(MMIPB_BCD_NUMBER_T pb_num);

/*****************************************************************************/
//  Description : mark the message
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetLastChatMsgMarked(
                                        MMIPB_BCD_NUMBER_T pb_num,
                                        BOOLEAN is_marked //IN:
                                       );

/*****************************************************************************/
//  Description : get sms chat order link delete available num
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_CurChatMsgUnlockedNum(MMISMS_ORDER_ID_T cur_order_id);

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                  deleting the related Chat SMS box
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidChatSMSForDelAll(void);

/*****************************************************************************/
//  Description : 根据信箱list列表的index，获得信箱的链表指针id。
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetChatLinkedArrayPtrIdInList(uint32 list_index);

/*****************************************************************************/
//  Description : get number node from chat order by index
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetNumNodeByIndexInList(uint16 index);

/*****************************************************************************/
//  Description : 比较两条信息是否是同一条长短信的不同部分
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CompareSmsState(                                           //RETURN: 
                                     MMISMS_STATE_T        *sms1_state_ptr,   //IN: SMS1 state 
                                     MMISMS_TYPE_E         type,               //IN: MO or MT
                                     MMISMS_STATE_T        *sms2_state_ptr    //IN: SMS2 state
                                     );

/*****************************************************************************/
//  Description : 获取信息type(MO,MT)
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_TYPE_E MMISMS_GetMoMtType(MMISMS_ORDER_ID_T cur_order_id);

/*****************************************************************************/
//  Description :某条信息是否为当前的信息中的一个
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_HaveSamePart(                                        //RETURN: 
                             MMISMS_STATE_T        *cur_state_ptr,    //IN: current SMS state
                             MMISMS_STATE_T     *new_state_ptr  //IN: new sms state
                             );
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
                       );

/*****************************************************************************/
//  Description : get unread num == 1 chat node index
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetChatUnreadIndex(void);

/*****************************************************************************/
//  Description : get chat order id(only one unread sms)
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetChatUnreadOrderId(void);

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related MT SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidChatSMSMoveToSecurity(MMIPB_BCD_NUMBER_T pb_num);

/*****************************************************************************/
//  Description : start from the current index, and find the valid index for 
//                moving the related MT SMS box
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_FindValidChatSMSForMoveAll(MMIPB_BCD_NUMBER_T pb_num,MN_SMS_STORAGE_E storage);

/*****************************************************************************/
//  Description :set exceed unread need update(max 50)
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetExceedUnreadNeedUpdate(void);

/*****************************************************************************/
//  Description :count chat order num
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_CountChatItemOrderNum(
                                        uint16 index,
                                        MMISMS_ORDER_ID_T order_id
                                       );
#endif

#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
//     Description : 根据定时短信，获得信箱的链表指针id。
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLinkedArrayPtrIdByTimerMsg(MMINV_SMS_TIMERMSG_T *timer_msg_ptr);

/*****************************************************************************/
//     Description : 检查Timer Msg是否有效
//    Global resource dependence : 
//  Author:hongbing.ju
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_CheckTimerMsgValid(void);
#endif

/*****************************************************************************/
//  Description : replace sms
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ReplaceSms(MN_SMS_PID_E pid_e, MMISMS_STATE_T *sms_state_ptr);

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
/*****************************************************************************/
//  Description : 重新排序
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_FullOrder(void);
#endif

#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : is push message 
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_IsPushMessage(MMISMS_ORDER_ID_T cur_order_id);

/*****************************************************************************/
//     Description : get push message box delete available msg num
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetPushMsgDeleteAvailNum(void);

/*****************************************************************************/
//     Description : 获取wap push的数量
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL uint16 MMISMS_GetPushMsgTotalNumberFromOrder(void);

/*****************************************************************************/
//     Description : 加载wap push，OTA信息
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_LoadPushMsgFromOrder(MMI_CTRL_ID_T     ctrl_id);
/*****************************************************************************/

/*****************************************************************************/
//     Description : mark all message
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetAllPushMsgMarked(
                                   BOOLEAN           is_marked //IN:
                                   );

/*****************************************************************************/
//     Description : 根据push信箱list列表的index，获得push list的index
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetPushMsgLinkedArrayPtrId(uint32 list_index);
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC  void MMISMS_SetPushMsgListItemData(
                                    MMI_CTRL_ID_T     ctrl_id,
                                    uint16            index
                                    );
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : 特定index会话mark的条数，长短信不统计
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetCurChatMarkedMsgNumByDualsys(MMIPB_BCD_NUMBER_T pb_num,MN_SMS_STORAGE_E storage,MN_DUAL_SYS_E    dual_sys);
/*****************************************************************************/
//  Description : 特定index会话mark的条数，长短信不统计
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetCurChatMarkedMsgNum(MMIPB_BCD_NUMBER_T pb_num,MN_SMS_STORAGE_E storage);
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获取下一个被选中的OrderID的Index
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMISMS_GetNextSelectedOrderID(void);
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : Get Update Sms Num
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadChatMsgByOrderId(MMISMS_ORDER_ID_T cur_order_id);
#endif

/*****************************************************************************/
//     Description : MMISMS_GetUnhandleMeSmsNumber
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetUnhandleMeSmsNumber(void);

#ifdef MMI_MSG_RESEND
/*****************************************************************************/
//    Description : 
//    Global resource dependence :
//    Author:Jiaoyou.wu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetLatestSendFailMsg(void);
#endif

/*****************************************************************************/
//  Description : mark all message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurOrderMarked( 
                                    MMISMS_ORDER_ID_T  cur_order_id,
                                    BOOLEAN is_marked
                                    );

/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurDeleteData(MMISMS_OPERATE_DATA_INFO_T *op_info);

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurMoveData(MMISMS_MOVE_DATA_INFO_T *op_info,int32 *ret_val);
#endif

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : start from the current index, and find the valid index for 
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetCurCopyData(MMISMS_COPY_DATA_INFO_T *copy_info_ptr,int32 *ret_val);
#endif

/*****************************************************************************/
// 	Description : MMISMS_GetUnitIDByIndex
//	Global resource dependence :
//  Author:   plum.peng
//	Note:
/*****************************************************************************/
PUBLIC MMISMS_LINKED_ORDER_UNIT_T* MMISMS_GetUnitIDByIndex(uint16 index);

/*****************************************************************************/
//    Description : 
//    Global resource dependence :
//    Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMISMS_GetUndownLoadHeadOrder(void);

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//  Description : Get Update Sms Num
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetUpdateBtSMSNum(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Insert Bt Sms To Order
//  Global  resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InsertBtSmsToOrder (MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************/
// 	Description : Update BT Unread SMS Order list
//	Global resource dependence : s_mmisms_order
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMISMS_UpdateBtUnreadOrder(void);

/*****************************************************************************/
//     Description : update the status info from unread to read for bt sms,and order the mt sms again.
//    Global resource dependence : s_mmisms_order
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_UpdateBtSmsInfoInOrder(
                                           MMISMS_ORDER_ID_T      old_order_id,  
                                           MMISMS_STATE_T        *new_order_info_ptr
                                           );

/*****************************************************************************/
//     Function name: MMISMS_DelBtSmsInfoInOrder ()
//  Description :  ??pos,??s_mmisms_order???????
//    Global resource dependence:  s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DelBtSmsInfoInOrder(
                                        MMISMS_ORDER_ID_T order_id  //[IN] 
                                        );

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_SAVE_H_
