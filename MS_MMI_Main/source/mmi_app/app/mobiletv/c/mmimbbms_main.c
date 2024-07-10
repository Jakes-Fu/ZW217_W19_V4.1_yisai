/*****************************************************************************
** File Name:      mmimbbms.c                                             *
** Author:                                                                   *
** Date:           2009-6                                                     *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv window table            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008       lisa.lin     Create                                     *
*****************************************************************************/

#define _MMIMBBMS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#ifdef MBBMS_SUPPORT
#include "mmimbbms_main.h"
#include "mmimbbms_wintab.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmipub.h"
#include "mbbms_auth.h"        
#include "mbbms_sub.h"        
#include "mmimtv_text.h"
#include "mmimtv_nv.h"
#include "mbbms_sg_api.h"
#include "mbbms_service.h"
#include "mtv_api.h"

#include "guilistbox.h"
#include "guiform.h"
#include "mmimtv_id.h"
#include "mmimtv_main.h"
#include "mmimtv_data.h"
#include "mmimtv_osd_display.h"
#include "mmimtv_image.h"
#include "mmimtv_internal.h"
#include "mmipdp_export.h"
#include "mmi_appmsg.h"
#include "mua_cmd.h"
#include "mmiphone_export.h"
#ifdef WIN32
#include "mbbms_api_dummy.h"
#endif
#include "Mua_cmd.h"

#include "mbbms_sg_common.h"
#include "mmiconnection_export.h"
#include "mmialarm_service.h"
#include "mmischedule_export.h"
#include "mmiphone_export.h"
#include "mmieng_win.h"
#include "mmiidle_export.h"
#include "mmk_msg.h"
#include "gui_ucs2b_converter.h"
#include "guires.h"
#include "chng_freq.h"
#include "freq_cfg.h"
//#include "mmiaudio_ctrl.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "mmi_default.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmk_app.h"
#include "mmifmm_export.h"
#include "mmipub.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


LOCAL MMIMBBMS_SUB_UPDATE_FROM_TYPE_E       s_sub_update_type = MMIMBBMS_SUB_UPDATE_FROM_NONE;   //record subscription update type
LOCAL MMIMBBMS_GBA_MRK_FROM_TYPE_E          s_gba_operation_type = MMIMBBMS_GBA_MRK_FROM_NONE;
LOCAL MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E  s_sg_operation_type = MMIMBBMS_SG_SEARCH_PARSE_FROM_NONE;
LOCAL MMIMBBMS_SUBSCRIBE_FROM_TYPE_E        s_subscribe_type = MMIMBBMS_SUBSCRIBE_FROM_NONE;
LOCAL MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T  s_curr_operation_purchase = {0};

LOCAL BOOLEAN   s_is_sub_update_ok= FALSE;

LOCAL uint8     s_currency_type = 0; 
LOCAL uint8     s_lang_type = 0;  
LOCAL BOOLEAN   s_is_mbbms_open = FALSE;    //标志MBBMS模块是否已开启

LOCAL MBBMS_Service_Request_t       *s_subscrible_request_ptr = PNULL;
LOCAL MBBMS_Service_Request_t       *s_uns_subscrible_request_ptr = PNULL;
//LOCAL MBBMS_Msk_Request_t           *s_msk_request_ptr = PNULL;
LOCAL MBBMS_Account_Inquiry_t       *s_account_inquiry_request_ptr = PNULL;
LOCAL MBBMS_Account_Inquiry_Res_t   *s_account_inquiry_res_ptr = PNULL; //套餐订购查询后返回的数据结构，其中包含已订购套餐信息
LOCAL SG_LIST_PTR_T                 s_serviceIdList_head_ptr = PNULL;

LOCAL MMIMBBMS_PLAY_INFO      s_mbbms_play_info = {0};
LOCAL MMIMBBMS_FLOW_INFO_T    s_curr_flow_info = {MMIMBBMS_FLOW_NONE,MMIMBBMS_FLOW_STATE_IDLE};
LOCAL MMIMBBMS_SETTING_INFO_T g_mbbms_setting_info = {0};

LOCAL const uint16            reminder_time[MMIMBBMS_REMAINDER_MAX] = {0, 2, 5, 10};
LOCAL BOOLEAN                 s_is_mbbms_exiting = FALSE;   //正在退出MBBMS
LOCAL MMIFILE_DEVICE_E        s_sgfile_location = MMI_DEVICE_UDISK;
LOCAL BOOLEAN                 s_is_init_finished = FALSE; //初始化是否完成

//mbbms2.0
LOCAL MTV_SERVICE_MODE_E     s_service_mode = MBBMS_SERVICE_MODE_CMMB;
LOCAL MN_DUAL_SYS_E          s_dual_sys = MN_DUAL_SYS_1;
LOCAL MTVSERVICE_PDPCALLBACK s_mtvservice_pdp_callback = PNULL;
LOCAL MMIMBBMS_BG_LAYER_INFO_T s_bg_layer_info = {0};

LOCAL BOOLEAN   s_is_update_from_city_name = FALSE;                 //是否使用按照城市更新SG标识
LOCAL uint8     s_city_name[MMIMBBMS_MAX_CITY_NAME_LENS+1] = {0};   //手动更新输入的城市名字
LOCAL uint16    s_city_name_len = 0;
LOCAL BOOLEAN  s_stopplay_flag = FALSE;
//标识SG是否发生变化
LOCAL anyURI s_sgdd_id = {0};
 
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : to handle alert window closed message
//	Global resource dependence : 
//  Author:lisa.lin
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMemAlertWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param);

/*****************************************************************************/
// 	Description : Update sg welcome information
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_UpdateWelcomeInfo(void);

/*****************************************************************************/
// Description : set sg filter
// Global resource dependence :
// Author:lisa.lin
// Note: 
/*****************************************************************************/
/*MBBMS Play Related end */
LOCAL void MMIMBBMS_SetSGFilter(void);

/*****************************************************************************/
//     Description : Init MBBMS when starting mobile TV
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_Init(void);

/*****************************************************************************/
//     Description : Set MBBMS GBA operation type
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetGBAOperationType(MMIMBBMS_GBA_MRK_FROM_TYPE_E gba_operation_type);

/*****************************************************************************/
//     Description : Get  MBBMS GBA operation type
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL MMIMBBMS_GBA_MRK_FROM_TYPE_E MMIMBBMS_GetGBAOperationType(void);

/*****************************************************************************/
// Description : operate failed, need update subscription
// Global resource dependence : none
// Author: xin.li
// Note:订购/退订操作失败, 且返回错误类型SUB_UNCLEAR时，
//      需要更新订购关系表来与服务器状态进行同步
/*****************************************************************************/
LOCAL void MMIMBBMS_HandleNeedUpdateSubscription(MBBMS_MSK_SUBSCRIBER_TYPE_E operation_type);

/*****************************************************************************/
// 	Description : Get Name from name list according to language type
//	Global resource dependence :
//  Author: 
//	Note:
//should check index
/*****************************************************************************/
LOCAL void MMIMBBMS_GetNameFromNameList(
                            SG_LIST_PTR_T name_list_head, //in
                            uint8   lang_type_code, //in
                            uint8** name_string,
                            uint32* string_len_ptr
                            );
							
/*****************************************************************************/
// 	Description : Get Purchase Item name by index
//	Global resource dependence :
//  Author: 
//	Note:
//should check index
/*****************************************************************************/
LOCAL uint8* MMIMBBMS_GetPurchaseNameByIndex(
						SG_LIST_PTR_T purchaseindex_list_head,//in
						int32 index,	//in		
						uint8   lang_type_code,
						uint32* string_len
						);
						
/*****************************************************************************/
// 	Description : Get price from price list according to language type
//	Global resource dependence :
//  Author: 
//	Note: @check for s_currency_type
/*****************************************************************************/
LOCAL SG_MONETARY_PRICE_T* MMIMBBMS_GetPriceInfoFromPriceList(
                                                              SG_LIST_PTR_T price_list_head, //in
                                                              uint8   sel_currency  //in
                                                              );

/*****************************************************************************/
// 	Description : Get the price of purchase item  by index
//	Global resource dependence :
//  Author: 
//	Note:
//should check index
/*****************************************************************************/
LOCAL void MMIMBBMS_GetPurchasePriceByIndex(
									SG_LIST_PTR_T purchaseindex_list_head,//in
									int32 index,	//in		
									uint8   currency, //in
									float* price,  //out
									MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E *subscriptionType,  //out
									uint8           *period               //out订购周期      
									);
									
/*****************************************************************************/
// 	Description : Get whether purchase is ordered or not
//	Global resource dependence :
//  Author: 
//	Note:
//should check index
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetPurchaseOrderStatusByIndex(
						SG_LIST_PTR_T purchaseindex_list_head,   
						uint32 index 
						);


/*****************************************************************************/
//     Description : get the current purchase index in richtext ctrl list
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_DisplayServiceInPurchase(SG_LIST_PTR_T service_idref_list_head, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//     Description : get the current purchase index in richtext ctrl list
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_DisplayContentInPurchase(SG_LIST_PTR_T content_idref_list_head, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//     Description : mbbms play service according to service id
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_PlayService(anyURI contentId);

/*****************************************************************************/
//     Description : mbbms play content according to service id
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_PlayContent(anyURI contentId);

/*****************************************************************************/
//  Description : Get MBBMS play-from type
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
LOCAL MMIMBBMS_PLAY_FROM_TYPE_E MMIMBBMS_GetPlayFromType(void);

/*****************************************************************************/
// 	Description : set need play next flag
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SetNeedPlayNextFlag(BOOLEAN is_need_play_next);

/*****************************************************************************/
// 	Description : update CMMB play status
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SetCmmbPlayStatus(MMIMTV_PLAY_STATUS_TYPE_E play_status);

/*****************************************************************************/
// 	Description : reset current service info, service_id etc.
//	Global resource dependence : none
//  Author: xin.li
//	Note: 重置当前service信息
/*****************************************************************************/
LOCAL void ResetCurServiceInfo(void);

/*****************************************************************************/
//     Description : mbbms subscription table update before play 
//    Global resource dependence : none
//     Description : check whether service or content is ordered befor play
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_CheckServiceOrContentOrderStatusBeforePlay(void);

/*****************************************************************************/
//     Description : whether content is ordered
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsContentOrdered(anyURI contentId);

/*****************************************************************************/
//     Description : whether purchase item is ordered
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsPurchaseItemOrdered(anyURI purchaseItem);

/*****************************************************************************/
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
// Description : Get NAF address and port
// Global resource dependence :
// Author: lisa.lin
// Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_GetNAFAddr(uint8* naf_addr,uint8* naf_addr_len,uint32 *port);

/*****************************************************************************/
//  Description : get selected date
//  Global resource dependence :
//  Author:plum.peng
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_GetNewRemindTime(
                                     SG_DATETIME_T* pSrcTime, 
                                     uint16 old_remindertime,
                                     uint16 new_remindertime,
                                     MMI_TM_T* pDstTime);

/*****************************************************************************/
// 	Description : set MBBMS open status
//	Global resource dependence :
//  Author:xin.li
//	Note: 设置MBBMS开启状态
/*****************************************************************************/
LOCAL void MMIMBBMS_SetMBBMSOpenStatus(BOOLEAN is_open);

/*****************************************************************************/
//     Description : Set MBBMS Init Flag, if during init process, then set it TRUE
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetSGOperationFromType(MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E sg_search_from_type);

/*****************************************************************************/
//     Description : Init MBBMS when starting mobile TV
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E  MMIMBBMS_GetSGOperationFromType(void);

/*****************************************************************************/
// 	Description : content is not ready
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
LOCAL MMIMBBMS_CONTENT_TIME_TYPE_E MMIMBBMS_CheckContentPlayTime(anyURI* pIndex);

/*****************************************************************************/
// 	Description : Whether content is expired
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/

LOCAL BOOLEAN MMIMBBMS_IsContentExpired(anyURI* pIndex);

/*****************************************************************************/
//     Description : whether sg files exist or not 
// 	Description : parse sg ok
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SGParseOK(void);

/*****************************************************************************/
// Description : parse sg fail 
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void SGParseFailed(void);

/*****************************************************************************/
// Description : mbbms active progress failed
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void MBBMSActiveFailed(void);

/*****************************************************************************/
// 	Description : sg search failed
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SGSearchFailed(void);

/*****************************************************************************/
//  Description : start active mbbms process
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_StartActiveMBBMS(void);

/*****************************************************************************/
// Description : start GBA operation
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_StartGBAOperation(MMIMBBMS_GBA_MRK_FROM_TYPE_E from_type);

/*****************************************************************************/
// Description : play
// Global resource dependence : none
// Author: xin.li
// Note: 发起播放
/*****************************************************************************/
LOCAL void MMIMBBMS_Play(uint32 frequency_no, uint32 service_id);

/*****************************************************************************/
// Description : handle gba operation failed
// Global resource dependence : none
// Author: xin.li
//Note:
/*****************************************************************************/
LOCAL void GBAOperationFailed(void);

/*****************************************************************************/
// Description : handle gba operation OK
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void GBAOperationOK(void);

/*****************************************************************************/
// 	Description : handle mrk operation failed
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
//LOCAL void MMIMBBMS_MRKOperationFailed(void);

/*****************************************************************************/
// 	Description : handle subscription update ok
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SubTableUpateOK(void);

/*****************************************************************************/
// 	Description : handle subscription update fail
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SubTableUpateFailed(void);


/*****************************************************************************/
// 	Description : start account inquir process
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_AccoutInquiry(void);

/*****************************************************************************/
// 	Description : start account inquir process
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_CloseAllMiddleWin(void);

/*****************************************************************************/
// 	Description : handle gba request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGBATestRequestWinMsg( 
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           );


/*****************************************************************************/
//     Description : 
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetServiceWap(void);

/*****************************************************************************/
//  Description : get wap setting status
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetServiceWapSetting(void);

/*****************************************************************************/
//     Description : Set MBBMS NAF addr
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetNAFAddr(void);

/*****************************************************************************/
//  Description : Get NAF Setting Status(setting failed or ok) 
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetNAFSettingStatus(void);

/*****************************************************************************/
// 	Description : handle exit wait win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleExitWaitWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : whether current service is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsServiceAudioTyle(anyURI id);

/*****************************************************************************/
// 	Description : whether current content is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsContentAudioTyle(anyURI id);

/*****************************************************************************/
// 	Description : whether current cmmb service is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCmmbServiceAudioTyle(uint32 index);

//mbbms2.0
/*****************************************************************************/
// Description : MTVSERVICE_CALLBACK实现
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_ServiceCallback(MBBMS_SERVICE_INFO_T *service_info_ptr);

/*****************************************************************************/
// Description : save MTVSERVICE pdp callback
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void SaveServicePDPCallback(MTVSERVICE_PDPCALLBACK service_pdp_callback);

/*****************************************************************************/
// Description : get saved MTVSERVICE pdp callback
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL MTVSERVICE_PDPCALLBACK GetServicePDPCallback(void);

/*****************************************************************************/
// Description : start play failed
// Global resource dependence : none
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void StartPlayFailed(MBBMS_SERVICE_RET_E service_ret);

/*****************************************************************************/
// Description : stop play failed
// Global resource dependence : none
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void StopPlayFailed(MBBMS_SERVICE_RET_E service_ret);

/*****************************************************************************/
// 	Description : set condition while playing audio service
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void SetAudioServiceCondition(void);

/*****************************************************************************/
// 	Description : restore condition
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void RestoreAudioServiceCondition(void);

/*****************************************************************************/
// 	Description : handle play ok
//	Global resource dependence : none
//  Author: xin.li
//	Note: 异步确认消息
/*****************************************************************************/
LOCAL void HandlePlayAck(MBBMS_SERVICE_RESULT_E result);

/*****************************************************************************/
// 	Description : handle rssi change msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleRssiChangedMsg(uint32 rssi_value);

/*****************************************************************************/
// 	Description : handle play_av_ind msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleAVIndMsg(uint32 param);

/*****************************************************************************/
// 	Description : handle emergency broadcast msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleEmergencyBroadcastMsg(MTVSERVICE_CMMB_BROADCAST_IND_T *eb_sig_ptr);

/*****************************************************************************/
// 	Description : handle ca_not_granted msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleCANotGrantedMsg(void);

/*****************************************************************************/
// 	Description : handle dra_ind msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleDRAIndMsg(void);

/*****************************************************************************/
// 	Description : handle bad_signal msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleBadSignalMsg(void);

/*****************************************************************************/
// 	Description : handle signal_recovered msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleSignalRecoveredMsg(void);

/*****************************************************************************/
// 	Description : handle relocking msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleRelockingMsg(void);

/*****************************************************************************/
// 	Description : handle play_error_ind msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandlePlayErrorIndMsg(void);

/*****************************************************************************/
// 	Description : get text id by returned error code
//	Global resource dependence : none
//  Author: xin.li
//	Note: 根据网络返回错误码获取对应的文本ID，TURE-获取成功， FALSE-获取失败
/*****************************************************************************/
LOCAL BOOLEAN GetTextIdByReturnCode(uint32          err_code,       //in
                                    MMI_TEXT_ID_T   *text_id_ptr    //out
                                    );

/*****************************************************************************/
// 	Description : check whether description resource is valid
//	Global resource dependence : none
//  Author: xin.li
//	Note: 检查网络提示语信息是否有效
/*****************************************************************************/
LOCAL BOOLEAN CheckDescriptionResValid(void *data_ptr,              //in
                                       MBBMS_SERVICE_FUNC_E type    //in
                                       );

/*****************************************************************************/
// 	Description : display error tip by returned description
//	Global resource dependence :
//  Author: xin.li
//	Note: 根据网络下发的提示语进行提示
/*****************************************************************************/
LOCAL void DisplayErrorTipByDescription(uint8   *description_ptr,
                                        uint16  len
                                        );

/*****************************************************************************/
// 	Description : send signal to task
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SendSignalToTask(uint16 sig_code, void *signal_data_ptr, uint32 task_id);

/*****************************************************************************/
//     Description : set softkey of sd card insert alert win
//    Global resource dependence : none
//    Author:  xiaoxiang.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleInsertSDCardAlertWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );


/*****************************************************************************/
// 	Description : 设置当前紧急广播等级信息
//	Global resource dependence :
//  Author: xiaoxiang.huang
//	Note: 
/*****************************************************************************/
LOCAL void MMIMTV_SetEbStopPlayFlag(BOOLEAN stopplayflag);

/*****************************************************************************/
//  Description : HandleSubscribeMainflowFinish
//  Global resource dependence : none
//  Author: xiaoxiang.huang
//  Note:
/*****************************************************************************/
void HandleSubscribeMainFlowFinish(DPARAM param,MMIMBBMS_FLOW_STATE_E curr_status);

/*****************************************************************************/
//  Description : HandleUnSubscribeMainflowFinish
//  Global resource dependence : none
//  Author: xiaoxiang.huang
//  Note:
/*****************************************************************************/
void HandleUnsubscribeMainFlowFinish(DPARAM param,MMIMBBMS_FLOW_STATE_E curr_status);
//mbbms2.0 end

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : copy anyURI
// Global resource dependence :
// Author: 
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_Copy2AnyURI(
                                 const anyURI *src_ptr, // [IN]
                                 anyURI *dst_ptr // [OUT]
                                 )
{
    if(PNULL == src_ptr || PNULL == dst_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_Copy2AnyURI src_ptr:0x%x dst_ptr:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_678_112_2_18_2_39_27_7,(uint8*)"dd", src_ptr, dst_ptr);
        return;
    }
    
    if(src_ptr->string_len > SG_MAX_URI_LEN)
    {
        //free firsts
        if(PNULL != dst_ptr->anyURI_ptr)
        {
            SCI_FREE(dst_ptr->anyURI_ptr);
        }
        
        dst_ptr->anyURI_ptr =  SCI_ALLOC_APP(src_ptr->string_len +1);
        if(PNULL == dst_ptr->anyURI_ptr)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_Copy2AnyURI alloc memory fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_693_112_2_18_2_39_27_8,(uint8*)"");
            return;
        }

        SCI_MEMSET(dst_ptr->anyURI_ptr, 0x0, src_ptr->string_len +1);
        SCI_MEMCPY(dst_ptr->anyURI_ptr,src_ptr->anyURI_ptr,src_ptr->string_len);
    }else
    {
        //clear original data in arr
        SCI_MEMSET(dst_ptr->anyURI_arr, 0x0, SG_MAX_URI_LEN);
        SCI_MEMCPY(dst_ptr->anyURI_arr,src_ptr->anyURI_arr,src_ptr->string_len);        
    }
    dst_ptr->string_len= src_ptr->string_len;
}

/*****************************************************************************/
// 	Description : whether MBBMS app is opened
//	Global resource dependence :
//  Author:xin.li
//	Note: MBBMS应用是否已开启
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsMBBMSOpen(void)
{
    return s_is_mbbms_open;
}

/*****************************************************************************/
//     Description :set all kinds of flow state
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_TYPE_E flow_type,MMIMBBMS_FLOW_STATE_E status)
{
    if(MMIMBBMS_FLOW_MAX <= flow_type)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetFlowInfo MMIMBBMS_FLOW_MAX <= flow_type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_725_112_2_18_2_39_27_9,(uint8*)"");
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_MAX <= status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetFlowInfo MMIMBBMS_FLOW_STATE_MAX <= status"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_731_112_2_18_2_39_27_10,(uint8*)"");
        return;
    }
    
    s_curr_flow_info.flow_type=flow_type;
    s_curr_flow_info.status=status;
    
}
/*****************************************************************************/
//     Description :get all kinds of flow state
//    Global resource dependence : none
//  Author:lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_FLOW_INFO_T* MMIMBBMS_GetFlowInfo(void)
{
      return &s_curr_flow_info;
}

/*****************************************************************************/
//     Description : Stop All flow related with http
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_StopFlow(void)
{
    //MBBMS_GBA_RETURN_E  gba_ret_val = GBA_RET_OK;
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_INVALID;
    BOOLEAN             recode = TRUE;
    
    //SCI_TRACE_LOW:"MMIMBBMS_StopFlow curr flow status=%d,curr_flow type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_758_112_2_18_2_39_27_11,(uint8*)"dd",s_curr_flow_info.status,s_curr_flow_info.flow_type);
    if(MMIMBBMS_FLOW_STATE_IDLE==s_curr_flow_info.status) 
    {
    	   if(!s_is_mbbms_exiting)
           {
               MMIMBBMS_DeactivePDPContext();
           }    
           
           return FALSE;
           
    }else if(MMIMBBMS_FLOW_STATE_STOPPING==s_curr_flow_info.status)
    {
        return TRUE;    
    }
    
    switch(s_curr_flow_info.flow_type)
    {
    case MMIMBBMS_FLOW_SG_MANUAL_UPDATE:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SG_MANUAL_UPDATE, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_StopManualUpdateSG();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_StopParseServiceGuide ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_781_112_2_18_2_39_27_12,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break; 
        
    case MMIMBBMS_FLOW_SG_PARSE:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SG_PARSE, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_StopParseServiceGuide();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_StopParseServiceGuide ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_799_112_2_18_2_39_27_13,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break;
        
    case MMIMBBMS_FLOW_ESG_PARSE:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_ESG_PARSE, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_StopParseESG();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_StopParseESG ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_817_112_2_18_2_39_27_14,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break;
        
    case MMIMBBMS_FLOW_MBBMS_ACTIVE:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_MBBMS_ACTIVE, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_DeactiveMBBMS();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_DeactiveMBBMS ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_835_112_2_18_2_39_27_15,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break;

    case MMIMBBMS_FLOW_PLAY:
        {
            //if audio service, restore condition
            RestoreAudioServiceCondition();
            
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_PLAY, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_Stop();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_Stop ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_856_112_2_18_2_39_27_16,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break;
        
    case MMIMBBMS_FLOW_SG_GET_COMPLEX:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SG_GET_COMPLEX, MMIMBBMS_FLOW_STATE_STOPPING);
            MBBMSSG_StopGetComplexInfo();
        }
        break;
        
    case MMIMBBMS_FLOW_SUBTABLE_UPDATE:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SUBTABLE_UPDATE, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_MBMSStopSubUpdate();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopSubUpdate ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_881_112_2_18_2_39_27_17,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break;
        
    case MMIMBBMS_FLOW_SUBSCRIBE:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SUBSCRIBE, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_MBMSStopServiceRequest();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopServiceRequest ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_899_112_2_18_2_39_27_18,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }	
        }
        break;
        
    case MMIMBBMS_FLOW_UNSUBSCRIBE:
        //case MMIMBBMS_FLOW_MSK_REQUEST:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_UNSUBSCRIBE, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_MBMSStopUnsubRequest();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopUnsubRequest ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_918_112_2_18_2_39_28_19,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }		
        }
        break;

    case MMIMBBMS_FLOW_GBA_OPERATION:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_GBA_OPERATION, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_MBMSStopGBA();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopGBA ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_936_112_2_18_2_39_28_20,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break;
        
    case MMIMBBMS_FLOW_ACCOUNT_INQUERY:
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_ACCOUNT_INQUERY, MMIMBBMS_FLOW_STATE_STOPPING);
            service_ret = MTVSERVICE_MBMSStopAccountQuiry();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopFlow MTVSERVICE_MBMSStopAccountQuiry ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_954_112_2_18_2_39_28_21,(uint8*)"d", service_ret);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                
                if(s_is_mbbms_exiting)
                {
                    //操作失败将无异步消息，同步退出
                    //MMIMBBMS_Clear();
                    recode = FALSE;
                }
            }
        }
        break;

    default:
        recode = FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//     Description : Set MBBMS subscription update type
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_TYPE_E sub_update_type)
{
    if (MMIMBBMS_SUB_UPDATE_FROM_TYPE_MAX <= sub_update_type)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetSubUpdateType MMIMBBMS_SUB_UPDATE_FROM_TYPE_MAX <= sub_update_type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_983_112_2_18_2_39_28_22,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_SetSubUpdateType sub_update_type = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_987_112_2_18_2_39_28_23,(uint8*)"d",sub_update_type);
    s_sub_update_type = sub_update_type;
}

/*****************************************************************************/
//     Description : Init MBBMS when starting mobile TV
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_SUB_UPDATE_FROM_TYPE_E MMIMBBMS_GetSubUpdateType(void)
{
    return s_sub_update_type;
}

/*****************************************************************************/
//     Description : Set MBBMS GBA operation type
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetGBAOperationType(MMIMBBMS_GBA_MRK_FROM_TYPE_E gba_operation_type)
{
    if (MMIMBBMS_GBA_MRK_FROM_TYPE_MAX <= gba_operation_type)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetGBAOperationType MMIMBBMS_GBA_MRK_FROM_TYPE_MAX <= gba_operation_type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1008_112_2_18_2_39_28_24,(uint8*)"");
        return;
    }
    s_gba_operation_type = gba_operation_type;
}

/*****************************************************************************/
//     Description : Get  MBBMS GBA operation type
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL MMIMBBMS_GBA_MRK_FROM_TYPE_E MMIMBBMS_GetGBAOperationType(void)
{
    return s_gba_operation_type;
}

/*****************************************************************************/
//  Description : set subscribe from type
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_TYPE_E type)
{
    if(type >= MMIMBBMS_SUBSCRIBE_FROM_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetSubscribeFromType  type >= MMIMBBMS_SUBSCRIBE_FROM_TYPE_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1031_112_2_18_2_39_28_25,(uint8*)"");
        return;
    }
    
    s_subscribe_type = type;
}

/*****************************************************************************/
//  Description : get subscribe from type
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_SUBSCRIBE_FROM_TYPE_E MMIMBBMS_GetSubscribeFromType(void)
{
    return s_subscribe_type;
}

/*****************************************************************************/
//     Description : Set mbbms play-from type
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_TYPE_E play_from_type)
{
    if (MMIMBBMS_PLAY_FROM_MAX <= play_from_type)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetPlayFromType MMIMBBMS_PLAY_FROM_MAX <= play_from_type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1055_112_2_18_2_39_28_26,(uint8*)"");
        return;
    }
    s_mbbms_play_info.from_type = play_from_type;
}

/*****************************************************************************/
//     Description : Get MBBMS play-from type
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL MMIMBBMS_PLAY_FROM_TYPE_E MMIMBBMS_GetPlayFromType(void)
{
    return s_mbbms_play_info.from_type;
}

/*****************************************************************************/
//     Description : Set Current play uri
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurrPlayUri(anyURI uri)
{
    MMIMBBMS_Copy2AnyURI(&uri, &(s_mbbms_play_info.uri));
}

/*****************************************************************************/
//     Description : Set Current play uri
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeCurrPlayUri(void)
{
    MBBMSSG_FreeTypeAnyURI(&(s_mbbms_play_info.uri));
}

/*****************************************************************************/
//     Description : Init MBBMS when starting mobile TV
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_Init(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_Init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1094_112_2_18_2_39_28_27,(uint8*)"");

#ifndef WIN32
    //升频
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(),(CHNG_FREQ_INDEX_E)FREQ_INDEX_APP_HIGH);
#endif
    
    //禁掉冲突资源, 背景播放、3D等
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_MTV);
#endif 
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_MTV, FALSE);
    
    //系统变量初始化
    s_is_sub_update_ok = FALSE;
    s_is_mbbms_exiting = FALSE;
    
    s_subscrible_request_ptr = PNULL;
    s_uns_subscrible_request_ptr = PNULL;
    s_account_inquiry_request_ptr = PNULL;
    s_serviceIdList_head_ptr = PNULL;
    
    s_sub_update_type = MMIMBBMS_SUB_UPDATE_FROM_NONE;
    s_gba_operation_type = MMIMBBMS_GBA_MRK_FROM_NONE;
    s_sg_operation_type = MMIMBBMS_SG_SEARCH_PARSE_FROM_NONE;
    SetMSKUpdateStatus((uint32)MMIMBBMS_FLOW_STATE_IDLE);
    s_subscribe_type = MMIMBBMS_SUBSCRIBE_FROM_NONE;

#ifdef WIN32
    s_is_init_finished = TRUE;
#else
    s_is_init_finished = FALSE;
#endif
    
    //init current flow info
    s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
    s_curr_flow_info.status = MMIMBBMS_FLOW_STATE_IDLE;
    //init play info
    s_mbbms_play_info.cmmb_play_status = MMIMTV_PLAY_NONE;
    s_mbbms_play_info.is_need_play_next = FALSE;
    
    SCI_MEMSET(&s_curr_operation_purchase,0x00,sizeof(MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T));
    SCI_MEMSET(&s_mbbms_play_info,0x00,sizeof(MMIMBBMS_PLAY_INFO));
    
    //init settting info etc.
    MMIMBBMS_InitSettingInfo();
    MMIMBBMS_InitContentNotifyInfo();
    
    //set sg filter
    MMIMBBMS_SetSGFilter();
}

/*****************************************************************************/
//     Description : Activate PDP context for mbbms
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ActivePDPContext(MTVSERVICE_PDP_ACTIVE_T *active_info_ptr)
{
    BOOLEAN              return_val = FALSE;
    uint8                setting_index = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MBMSPDP_CNF_INFO_T   mbbms_pdp_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    
    if(PNULL == active_info_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_ActivePDPContext  active_info_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1160_112_2_18_2_39_28_28,(uint8*)"");
        return FALSE;
    }
    
    //保存MTVSERVICE_PDPCALLBACK
    SaveServicePDPCallback(active_info_ptr->mtvservice_pdpcallback);
    
    setting_index = MMIMBBMS_GetNetSettingIndex(active_info_ptr->dual_id);    
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(active_info_ptr->dual_id, setting_index);
    if(PNULL!=setting_item_ptr)
    {
        app_info.app_handler = MMI_MODULE_MTV;
        app_info.auth_type = setting_item_ptr->auth_type;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.dual_sys = active_info_ptr->dual_id;
        app_info.priority = 3;
        app_info.ps_service_rat = MN_UNSPECIFIED;
        app_info.ps_service_type = BROWSER_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;
        
        //SCI_TRACE_LOW:"MMIMBBMS_ActivePDPContext:apn=%s,username=%s,psw=%s,dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1182_112_2_18_2_39_28_29,(uint8*)"sssd",app_info.apn_ptr,app_info.user_name_ptr, app_info.psw_ptr,app_info.dual_sys);
        
        app_info.handle_msg_callback = MMIMBBMS_HandlePDPMsg;

        return_val = MMIAPIPDP_Active(&app_info);
    }
    
    if(!return_val)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_ActivePDPContext  MMIAPIPDP_Active failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1191_112_2_18_2_39_28_30,(uint8*)"");
        //PDP激活同步失败，亦通知MTVSERVICE
        mbbms_pdp_info.msg_id = MBMSPDP_ACTIVE_CNF;
        mbbms_pdp_info.result = MBMSPDP_RESULT_FAIL;
        active_info_ptr->mtvservice_pdpcallback(&mbbms_pdp_info);
    }
    
#ifdef WIN32
    //dummy pdp active_cnf msg to MTVSERVICE
    mbbms_pdp_info.msg_id = MBMSPDP_ACTIVE_CNF;
    mbbms_pdp_info.result = MBMSPDP_RESULT_SUCC;
    active_info_ptr->mtvservice_pdpcallback(&mbbms_pdp_info);
#endif

    return return_val;
}


/*****************************************************************************/
//     Description : Deactivate PDP context
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_DeactivePDPContext(void)
{
    BOOLEAN ret = FALSE;
    
    ret = MMIAPIPDP_Deactive(MMI_MODULE_MTV);
    //SCI_TRACE_LOW:"MMIMBBMS_DeactivePDPContext ret=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1218_112_2_18_2_39_28_31,(uint8*)"d",ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : mmi handle pdp msg 
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    MBMSPDP_CNF_INFO_T      mbbms_pdp_info = {0};
    MTVSERVICE_PDPCALLBACK  mtvservice_pdp_callback = GetServicePDPCallback();
    
    if(PNULL == msg_ptr
        || PNULL == mtvservice_pdp_callback)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandlePDPMsg msg_ptr:0x%x mtvservice_pdp_callback:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1235_112_2_18_2_39_28_32,(uint8*)"dd", msg_ptr, mtvservice_pdp_callback);
        return;
    }
    
    if(!MMIMBBMS_IsMBBMSOpen())
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandlePDPMsg:mbbms is not open,ignore msg =%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1241_112_2_18_2_39_28_33,(uint8*)"d", msg_ptr->msg_id);
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandlePDPMsg: msg_id=%d, result=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1245_112_2_18_2_39_28_34,(uint8*)"dd", msg_ptr->msg_id, msg_ptr->result);
    
    //cr238546, 正常情况下mtvservice层不应该收到PDP DEACTIVE_CNF消息
    if(msg_ptr->msg_id == MMIPDP_DEACTIVE_CNF)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandlePDPMsg: ignore MMIPDP_DEACTIVE_CNF msg!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1250_112_2_18_2_39_28_35,(uint8*)"");
        return;
    }
    
    mbbms_pdp_info.app_handler = msg_ptr->app_handler;
    mbbms_pdp_info.nsapi       = msg_ptr->nsapi;
    mbbms_pdp_info.msg_id      = (MBMSPDP_APP_MSG_E)msg_ptr->msg_id;
    switch(msg_ptr->result)
    {
    case MMIPDP_RESULT_SUCC:
        mbbms_pdp_info.result = MBMSPDP_RESULT_SUCC;
        break;
        
    case MMIPDP_RESULT_FAIL:
        mbbms_pdp_info.result = MBMSPDP_RESULT_FAIL;
        break;
        
    case MMIPDP_RESULT_TIMEOUT:
        mbbms_pdp_info.result = MBMSPDP_RESULT_TIMEOUT;
        break;
        
    default:
        mbbms_pdp_info.result = MBMSPDP_RESULT_FAIL;
        break;
    }
    
    //send to mtvservice via callback
    mtvservice_pdp_callback(&mbbms_pdp_info);
}

/*****************************************************************************/
// Description : start GBA operation
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_StartGBAOperation(MMIMBBMS_GBA_MRK_FROM_TYPE_E from_type)
{
    //whether card support mtv or not
    if(!MUACMD_MBBMS_IsEnabled())
    {   
        //SCI_TRACE_LOW:"MMIMBBMS_StartGBAOperation:card not support mtv\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1289_112_2_18_2_39_28_36,(uint8*)"");
        return;
    }
    
    if(MMIMBBMS_GBA_MRK_FROM_ENG_MODE != from_type)
    {
        //whether gba is ready or not
        if(!MUACMD_IsGBANeeded())   
        {        
            //SCI_TRACE_LOW:"MMIMBBMS_StartGBAOperation:gba is ready\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1298_112_2_18_2_39_28_37,(uint8*)"");
            return;
        }
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_StartGBAOperation:start gba init process start!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1303_112_2_18_2_39_28_38,(uint8*)"");
    if(MMIMBBMS_FLOW_STATE_IDLE != s_curr_flow_info.status)
    {
        //流程不允许并发
        //SCI_TRACE_LOW:"MMIMBBMS_StartGBAOperation flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1307_112_2_18_2_39_28_39,(uint8*)"");
        return;
    }
    
    // to check whether naf is setted
    if((!MMIMBBMS_GetNAFSettingStatus()) ||(!MMIMBBMS_GetServiceWapSetting()))
    {
        GBAOperationFailed();
        return;
    }
    
    //set gba operation from type
    MMIMBBMS_SetGBAOperationType(from_type);
    //SCI_TRACE_LOW:"MMIMBBMS_StartGBAOperation gba from type =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1320_112_2_18_2_39_28_40,(uint8*)"d",from_type);
    
    //start GBA
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_GBA_OPERATION, MMIMBBMS_FLOW_STATE_STARTING);
    if(MBBMS_SERVICE_FUNC_OK != MTVSERVICE_StartGBA())
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartGBAOperation  MTVSERVICE_StartGBA return fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1326_112_2_18_2_39_28_41,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_GBA_OPERATION_FAILED);
    }
}

/*****************************************************************************/
// Description : play
// Global resource dependence : none
// Author: xin.li
// Note: 发起播放
/*****************************************************************************/
LOCAL void MMIMBBMS_Play(uint32 frequency_no, uint32 service_id)
{
    uint32 old_freq_no = 0;
    uint32 old_service_id = 0;
    
    //SCI_TRACE_LOW:"[MMIMBBMS]: MMIMBBMS_Play freq_no = %d service_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1341_112_2_18_2_39_28_42,(uint8*)"dd", frequency_no, service_id);
    
    //记录旧信息, 用以判断是否需发起播放
    old_freq_no     = s_mbbms_play_info.frequency_no;
    old_service_id  = s_mbbms_play_info.service_id;
    
    //保存播放信息
    s_mbbms_play_info.frequency_no    = frequency_no;
    s_mbbms_play_info.service_id      = service_id;
    
    //切换网络播放时，需要把信号强度置零
    MMIMTV_SaveTVSignalLevel(0);
    MAIN_SetTVSignalState();
    
    //设置播放标志
    MMIMBBMS_SetIsNeedPlayService(TRUE);
    
    //open player window
    if(MMK_IsOpenWin(MMIMTV_MAIN_WIN_ID))
    {
        //player win is already open
        if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
        {
            //player win is in focus
            //SCI_TRACE_LOW:"[MMIMBBMS]: MMIMBBMS_Play MMIMTV_MAIN_WIN_ID is in focus"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1365_112_2_18_2_39_28_43,(uint8*)"");
            
            if(old_freq_no == s_mbbms_play_info.frequency_no 
                && old_service_id == s_mbbms_play_info.service_id
                && MMIMTV_PLAY_PLAYING == s_mbbms_play_info.cmmb_play_status)
            {
                MMIMBBMS_SetIsNeedPlayService(FALSE);
            }
            
            if(MMIMBBMS_IsNeedPlayService())
            {
                MMIMBBMS_SetIsNeedPlayService(FALSE);
                
                //连接等待场景
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_PLAY_CONNECTING);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                
                //启动播放主流程
                MMIMBBMS_StartPlay();
                
            }
            else
            {
                //显示频道号
                if(OSD_IDLE == MMIMTV_GetFocusOSDScene() || OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
                {
                    MMIMTV_DisplayServiceNumber();
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMBBMS]: MMIMBBMS_Play  MMK_WinGrabFocus(MMIMTV_MAIN_WIN_ID)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1398_112_2_18_2_39_29_44,(uint8*)"");
            //当前的频道已在播放，不需要再连接, 显示频道号
            if(old_freq_no == s_mbbms_play_info.frequency_no 
                && old_service_id == s_mbbms_play_info.service_id
                && MMIMTV_PLAY_PLAYING == s_mbbms_play_info.cmmb_play_status)
            {
                MMIMBBMS_SetIsNeedPlayService(FALSE);
            }
            
            //player win is open but not in focus, grab focus
            MMK_WinGrabFocus(MMIMTV_MAIN_WIN_ID);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMBBMS]: MMIMBBMS_Play  MMIMTV_OpenMTVPlayer()"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1413_112_2_18_2_39_29_45,(uint8*)"");
        MMIMTV_OpenMTVPlayerForMBBMS();
    }
    
    //设置OSD场景数据
    MMIMTV_SetPreOSDScene(OSD_IDLE);
}

/*****************************************************************************/
//  Description : start manual update SG
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartManualUpdateSG(MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E from_type)
{
    uint8   setting_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr = PNULL;
    MBBMS_SERVICE_RET_E wap_ret = MBBMS_SERVICE_FUNC_OK;
    MBBMS_SERVICE_RET_E sg_ret = MBBMS_SERVICE_FUNC_OK;
    
    //SCI_TRACE_LOW:"MMIMBBMS_StartManualUpdateSG from type=%d service_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1432_112_2_18_2_39_29_46,(uint8*)"dd", from_type, MMIMBBMS_GetServiceMode());
    if(MMIMBBMS_FLOW_STATE_IDLE != s_curr_flow_info.status)
    {
        //流程不允许并发
        //SCI_TRACE_LOW:"MMIMBBMS_StartManualUpdateSG flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1436_112_2_18_2_39_29_47,(uint8*)"");
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_NETWORK_BUSY);
        return;
    }
	
    MMIMBBMS_CheckSGDD();   
    MMIMBBMS_SetSGOperationFromType(from_type);
    
    //start update via service mode
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        setting_index = MMIMBBMS_GetNetSettingIndex(MMIMBBMS_GetCurSelectSim());        
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIMBBMS_GetCurSelectSim(), setting_index);
        
        //Set SG wap address
        //SCI_TRACE_LOW:"MMIMBBMS_StartManualUpdateSG gateway =%s,port=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1451_112_2_18_2_39_29_48,(uint8*)"sd",setting_item_ptr->gateway,setting_item_ptr->port);
        wap_ret = MTVSERVICE_SGSetWap((char*)setting_item_ptr->gateway, setting_item_ptr->port);
        //Set SG URI
        sg_ret = MTVSERVICE_SetSGURI(g_mbbms_setting_info.sg_domain_info);//uri
        
        if(MBBMS_SERVICE_FUNC_OK != wap_ret
            || MBBMS_SERVICE_FUNC_OK != sg_ret)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_StartManualUpdateSG connection setting failed wap_ret=%d, sg_ret=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1459_112_2_18_2_39_29_49,(uint8*)"dd", wap_ret, sg_ret);
            SGSearchFailed();
            return;
        }
        
        //if search sg via city, set area first
        if(MMIMBBMS_IsCityUpdateSG())
        {
            uint8       *city_name = PNULL;
            uint16      len = 0;
            MBBMS_SERVICE_RET_E  error_code = MBBMS_SERVICE_FUNC_OK;
            
            MMIMBBMS_SetCityUpdateSGFlag(FALSE);
            
            //set city name
            city_name = MMIMBBMS_GetUpdateCityName(&len);	
            error_code = MTVSERVICE_SetSGAreaName(city_name, len);
            //SCI_TRACE_LOW:"MMIMBBMS_StartManualUpdateSG MTVSERVICE_SetSGAreaName name_len=%d error_code = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1476_112_2_18_2_39_29_50,(uint8*)"dd", len, error_code);
        }
        
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SG_MANUAL_UPDATE, MMIMBBMS_FLOW_STATE_STARTING);
        if(MBBMS_SERVICE_FUNC_OK != MTVSERVICE_ManualUpdateSG(NULL, NULL))
        {
            //SCI_TRACE_LOW:"MMIMBBMS_StartManualUpdateSG  MTVSERVICE_ManualUpdateSG return fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1482_112_2_18_2_39_29_51,(uint8*)"");
            SGSearchFailed();
        }
    }
    else
    {
        //cmmb mode update
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SG_MANUAL_UPDATE, MMIMBBMS_FLOW_STATE_STARTING);
        if(MBBMS_SERVICE_FUNC_OK != MTVSERVICE_ManualUpdateSG(MTVSERVICE_SEARCH_PROG_ESG, NULL))
        {
            //SCI_TRACE_LOW:"MMIMBBMS_StartManualUpdateSG  MTVSERVICE_ManualUpdateSG return fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1492_112_2_18_2_39_29_52,(uint8*)"");
            SGSearchFailed();
        }
    } 
}


/*****************************************************************************/
//     Description : Set service(including gba, mrk, msk,open,pause,cancel,resume, subscrible,unsubscrible)wap addr,
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetServiceWap(void)
{
    
    uint8 setting_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T*  setting_item_ptr = PNULL;
    
    setting_index = MMIMBBMS_GetNetSettingIndex(MMIMBBMS_GetCurSelectSim());
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIMBBMS_GetCurSelectSim(), setting_index);
    
    //set wap address
    MBBMS_Set_WAP((char*)setting_item_ptr->gateway, setting_item_ptr->port);
}

/*****************************************************************************/
//  Description : get wap setting status
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetServiceWapSetting(void)
{
    char    wap_addr[MMICONNECTION_MAX_GATEWAY_LEN] = {0};
    uint16  port = 0;
    BOOLEAN ret_val = FALSE;
    
#ifdef WIN32
    return TRUE;
#endif
    
    ret_val = MBBMS_Get_WAP(wap_addr,  &port);
    
    //SCI_TRACE_LOW:"MMIMBBMS_GetServiceWapSetting wap_addr =%s ,ret_val  =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1532_112_2_18_2_39_29_53,(uint8*)"sd",wap_addr,ret_val);
    if((ret_val) && (0 !=strlen(wap_addr)))
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }
    
    
    return ret_val;
}
/*****************************************************************************/
//     Description : Set MBBMS NAF addr
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetNAFAddr(void)
{
    
    uint8   naf_addr[MMIMBBMS_MAX_NAF_ADDR_LEN]={0};  
    uint8   naf_addr_len=0;
    uint32 naf_port=0;
    
    MMIMBBMS_GetNAFAddr(naf_addr,&naf_addr_len,&naf_port);	  
    //set naf address,set port param to 0
    MBBMS_Set_NAF((char*)naf_addr, 0); 
}

/*****************************************************************************/
//  Description : Get NAF Setting Status(setting failed or ok) 
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetNAFSettingStatus(void)
{
    uint8   naf_addr[MMIMBBMS_MAX_NAF_ADDR_LEN]={0};    
    uint16  naf_port =0;
    BOOLEAN ret_val = FALSE;
    
#ifdef WIN32
    return TRUE;
#endif
    
    ret_val = MBBMS_Get_NAF((char*)naf_addr,&naf_port);
    
    //SCI_TRACE_LOW:"MMIMBBMS_GetNAFSettingStatus naf addr =%s ,ret_val  =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1577_112_2_18_2_39_29_54,(uint8*)"sd",naf_addr,ret_val);
    
    if((ret_val) && (0 != strlen((char*)naf_addr) ))
    {
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }
    
    return ret_val;
    
}

/*****************************************************************************/
//  Description : handle gba operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleGBAOperationSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    mbbms_gba_data_t     *gba_res_ptr = PNULL;
    MMI_TEXT_ID_T        text_id = TXT_NULL;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleGBAOperationSignal sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1604_112_2_18_2_39_29_55,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleGBAOperationSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1609_112_2_18_2_39_29_56,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleGBAOperationSignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1613_112_2_18_2_39_29_57,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        MMIMBBMS_SetGBAOperationType(MMIMBBMS_GBA_MRK_FROM_NONE);
        
        gba_res_ptr = (mbbms_gba_data_t*)sig_data_ptr->data_ptr;
        MBBMS_GBA_Free_Data(gba_res_ptr);
        MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
        
        if(s_is_mbbms_exiting)
        {
            MMIMBBMS_Clear();
        }
        else
        {
            //NOTHING TO DO
        }
        
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status)
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_GBA:
            //主流程结束
            //SCI_TRACE_LOW:"MMIMBBMS_HandleGBAOperationSignal subtype MBBMS_SERVICE_GBA"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1639_112_2_18_2_39_29_58,(uint8*)"");
            gba_res_ptr = (mbbms_gba_data_t*)sig_data_ptr->data_ptr;
            
            if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result)
            {
                GBAOperationOK();
            }
            else
            {
                if(MMK_IsOpenWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID);
                }
                
                if(CheckDescriptionResValid(gba_res_ptr, MBBMS_SERVICE_GBA))
                {
                    DisplayErrorTipByDescription((uint8*)gba_res_ptr->description, 255);
                }
                else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
                {
                    MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    GBAOperationFailed();
                }
                
                MMIMBBMS_SetGBAOperationType(MMIMBBMS_GBA_MRK_FROM_NONE);
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE,MMIMBBMS_FLOW_STATE_IDLE);
            }
            
            MBBMS_GBA_Free_Data(gba_res_ptr);
            break;
            
        case MBBMS_SERVICE_PDP_ACTIVE:
            //PDP子流程，提示待主流程完毕后
            //SCI_TRACE_LOW:"MMIMBBMS_HandleGBAOperationSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1675_112_2_18_2_39_29_59,(uint8*)"");
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleGBAOperationSignal  subtype error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1679_112_2_18_2_39_29_60,(uint8*)"");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleGBAOperationSignal  cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1685_112_2_18_2_39_29_61,(uint8*)"");
    }
}

/*****************************************************************************/
// Description : handle stop gba operation signal
// Global resource dependence : none
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopGBAOperationSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    //mbbms_gba_data_t     *gba_res_ptr = PNULL;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopGBAOperationSignal sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1700_112_2_18_2_39_29_62,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopGBAOperationSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1705_112_2_18_2_39_29_63,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    MMIMBBMS_SetGBAOperationType(MMIMBBMS_GBA_MRK_FROM_NONE);
    
    if(MMK_IsOpenWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID);
    }
    //MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
    
    if (s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
        return;
    }
}

/*****************************************************************************/
//     Description :Enter into main window
//    Global resource dependence : none
//    Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterIntoMainWindow(void)
{
    BOOLEAN bRet = FALSE;
    
    //SCI_TRACE_LOW:"MMIMBBMS_EnterIntoMainWindow\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1731_112_2_18_2_39_29_64,(uint8*)"");
    
    //enter into main window (open).
    bRet = MMIMBBMS_OpenMainInterfaceWin();   
    
    if (!bRet)
    {
        return;
    }
    
    //close init window
#ifdef WIN32
    SCI_Sleep(200);
#endif
    
    //StartUpWin正常关闭
    MMIMBBMS_SetStartUpWinCloseFlag(FALSE);
    MMK_CloseWin(MMIMBBMS_STARTUP_WIN_ID);    
    
}

/*****************************************************************************/
// Description : operate failed, need update subscription
// Global resource dependence : none
// Author: xin.li
// Note:订购/退订操作失败, 且返回错误类型SUB_UNCLEAR时，
//      需要更新订购关系表来与服务器状态进行同步
/*****************************************************************************/
LOCAL void MMIMBBMS_HandleNeedUpdateSubscription(MBBMS_MSK_SUBSCRIBER_TYPE_E operation_type)
{
    MMI_STRING_T  string_hint = {0};
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleNeedUpdateSubscription operation_type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1761_112_2_18_2_39_29_65,(uint8*)"d",operation_type);
    
    switch(operation_type)
    {
    case MBBMS_SUB_SERVICE_REQUEST:
    case MBBMS_SUB_SERVICE_UNSUB:
        MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_UNCLEAR);
        break;

    default:
        break;
    }
    
    MMI_GetLabelTextByLang(TXT_MBBMS_NEED_UPDATE_SUBSCRIPTION, &string_hint);
    
    //询问是否更新订购关系表
    MMIMBBMS_OpenSubscriptionUpdateQueryWinByTextPtr(&string_hint);
	
    if(MMK_IsOpenWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID))
        MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);
	
    if(MMK_IsOpenWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID))
        MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);
    
    MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
#if 0	
    MMK_CloseWin(MMIMBBMS_SUBSCRIBE_TYPE_WIN_ID);
    MMK_CloseWin(MMIMBBMS_PURCHASEITEM_LIST_WIN_ID);
#endif	
}

/*****************************************************************************/
//  Description : HandleSubscribeMainflowFinish
//  Global resource dependence : none
//  Author: xiaoxiang.huang
//  Note:
/*****************************************************************************/
void HandleSubscribeMainFlowFinish(DPARAM param,MMIMBBMS_FLOW_STATE_E curr_status)
{
    anyURI         globalItemID = {0};
    MMI_STRING_T   purchase_item_str = {0};	
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    MBBMSSG_PURCHASE_ITEM_T             item_info = {0};
    MBBMS_Service_Request_Res_t         *sub_res_ptr = PNULL;
    MBBMS_Purchase_Data_Reference_Service_t *data_ref_ptr = PNULL;
    MMI_TEXT_ID_T        text_id = TXT_NULL;
    char           char_price[10] = {0};
    wchar          wchar_price[10] = {0};
    MMI_STRING_T   hint_str0 = {0};
    MMI_STRING_T   hint_str1 = {0};
    MMI_STRING_T   hint_str2 = {0};
    MMI_STRING_T   hint_str3 = {0};
    //MMI_STRING_T   hint_str4 = {0};
    //MMI_STRING_T   hint_str5 = {0};
    //MMI_STRING_T   hint_str6 = {0};
    //MMI_STRING_T   str_price = {0};
    wchar          wchar_hint1[40] = {0};
    wchar          wchar_hint3[40] = {0};
    //wchar          wchar_hint4[40] = {0};
    //wchar          wchar_hint6[40] = {0};
    anyURI         itemID = {0};
    //anyURI         globalItemID = {0};
    wchar          wchar_name[60] = {0};
    uint16         lens = 0;
    MMI_STRING_T  prompt_str={0};	
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"HandleSubscribeMainFlowFinish sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1827_112_2_18_2_39_30_66,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"[MMIMBBMS]HandleSubscribeMainFlowFinish:curr_status= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1831_112_2_18_2_39_30_67,(uint8*)"d",curr_status);
	
	sub_res_ptr = (MBBMS_Service_Request_Res_t*)(sig_data_ptr->data_ptr);

	if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result)
	{
		//subscribe success
		uint8  global_id_len =0;
    
		//SCI_TRACE_LOW:"[MMIMBBMS]HandleSubscribeMainFlowFinish:Subscrible successfully!\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1840_112_2_18_2_39_30_68,(uint8*)"");
    
		MMI_GetLabelTextByLang(TXT_MBBMS_SUB_SUCCESS, &hint_str0);
    
		data_ref_ptr = s_subscrible_request_ptr->data_ref_ptr;
    
		global_id_len = strlen((char*)data_ref_ptr->gloabl_id_ref);
    
		if(SG_MAX_URI_LEN > global_id_len)
		{              
			SCI_MEMCPY(globalItemID.anyURI_arr, data_ref_ptr->gloabl_id_ref, global_id_len);            
		}
		else
		{
			globalItemID.anyURI_ptr =  SCI_ALLOC_APP(global_id_len+1);
            if(PNULL == globalItemID.anyURI_ptr)
            {
                //SCI_TRACE_LOW:"HandleSubscribeMainFlowFinish alloc mem fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1857_112_2_18_2_39_30_69,(uint8*)"");
                return;
            }
            
            SCI_MEMSET(globalItemID.anyURI_ptr, 0x0, (global_id_len+1));
			SCI_MEMCPY(globalItemID.anyURI_ptr, data_ref_ptr->gloabl_id_ref, global_id_len);
		}
    
		globalItemID.string_len = global_id_len;        
    
		MBBMSSG_GetPurchaseItemId(globalItemID, &itemID);       
    
		MBBMSSG_GetPurchaseItem(itemID, &item_info);     
    
		MMIMBBMS_GetNameWchar(item_info.NameList, wchar_name, sizeof(wchar_name) - 1);
    
		purchase_item_str.wstr_ptr = wchar_name;
		purchase_item_str.wstr_len = MMIAPICOM_Wstrlen(wchar_name);
    
		MBBMSSG_FreePurchaseItem(&item_info);
		MBBMSSG_FreeTypeAnyURI(&itemID);
    
		hint_str1.wstr_ptr = wchar_hint1;
		MMIAPICOM_MergeTwoString(&hint_str0, &purchase_item_str, &hint_str1, 40);
		hint_str1.wstr_len = MMIAPICOM_Wstrlen(wchar_hint1);
    
		MMI_GetLabelTextByLang(TXT_MBBMS_PURCHASEITEM, &hint_str2);
    
		hint_str3.wstr_ptr = wchar_hint3;
		MMIAPICOM_MergeTwoString(&hint_str1, &hint_str2, &hint_str3, 40);
		hint_str1.wstr_len = MMIAPICOM_Wstrlen(wchar_hint3);

	#if 0                
	 sprintf(char_price, "%f", data_ref_ptr->price);
 
	 lens = strlen(char_price);
 
	 while (lens > 0 && char_price[--lens] == '0')
	 {
		 ;
	 }
 
	 if (char_price[lens] == '.')
	 {
		 char_price[lens+2] = '\0';
	 }
	 else
	 {
		 char_price[lens+1] = '\0';
	 }
 
	 MMIAPICOM_StrToWstr((uint8*)char_price, wchar_price);
	 str_price.wstr_ptr = wchar_price;
	 str_price.wstr_len = MMIAPICOM_Wstrlen(wchar_price);
 
	 hint_str4.wstr_ptr = wchar_hint4;
	 MMIAPICOM_MergeTwoString(&hint_str3, &str_price, &hint_str4, 40);
	 hint_str4.wstr_len = MMIAPICOM_Wstrlen(wchar_hint4);
 
	 MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRIBE_OK_HINT_TEXT4, &hint_str5);
 
	 hint_str6.wstr_ptr = wchar_hint6;
	 MMIAPICOM_MergeTwoString(&hint_str4, &hint_str5, &hint_str6, 40);
	 hint_str6.wstr_len = MMIAPICOM_Wstrlen(wchar_hint6);
 
	 if(PNULL != globalItemID.anyURI_ptr)
	 {
		 SCI_FREE(globalItemID.anyURI_ptr);
	 }
	#endif                
      /*  if(curr_status == MMIMBBMS_FLOW_STATE_STOPPING)
        {
			MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRIBE_CANCEL_FAIL, &prompt_str);
		    MMIPUB_OpenAlertTextWinByTextPtr(PNULL,
			    &prompt_str,
			    PNULL,
			    PNULL,
			    MMIPUB_SOFTKEY_ONE,
			    PNULL);
		#ifdef WIN32
		    SCI_Sleep(200);
		#endif			
        }*/
		MMIPUB_OpenAlertTextWinByTextPtr(PNULL,
			&hint_str3,
			PNULL,
			PNULL,
			MMIPUB_SOFTKEY_ONE,
			PNULL);        
    
		#ifdef WIN32
		SCI_Sleep(500);
		#endif
    
        /*由于2.0规范中已订购套餐信息是从查询的返回消息的结构体中获取的
        所以当订购或者退订以后我们没有发起重新查询之前是无法得到最新
        的订购信息的，这个时候如果直接返回原来界面显示结果是错误的，
        而直接后台查询的话因为查询有一定的时间，显示也不友好，所以先退出*/
        MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
        MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        
        //update display
        MMIMBBMS_FreeProgramList();
        MMIMBBMS_FreeServiceIndex();
        MMK_PostMsg(MMIMBBMS_SEARCH_RESULT_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
        MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
        MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
        MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
        MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
     }
	 else
	 {
        if(curr_status == MMIMBBMS_FLOW_STATE_STARTING)//MMIMBBMS_FLOW_STATE_STOPPING时，订购失败不提示
        {
		 //subscrbe failed
		 //SCI_TRACE_LOW:"MMIMBBMS HandleSubscribeMainFlowFinish:Subscrible  failed !\n"
		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_1973_112_2_18_2_39_30_70,(uint8*)"");
		 MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
         MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_NONE);
		 MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);
     
		 if(CheckDescriptionResValid(sub_res_ptr, MBBMS_SERVICE_SUBSCRIBE))
		 {
			 DisplayErrorTipByDescription(sub_res_ptr->description, sub_res_ptr->description_len);
		 }
		 else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
		 {
			 MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
		 }
		 else
		 {
			 MMIMBBMS_DisplayCommonTip(TXT_MBBMS_SUBSCRIBE_FAILED, IMAGE_PUBWIN_WARNING);
		 }
        }else
        {
            //NOTHING TO DO
		}
	 }
 
	 MBBMS_Free_Service_Request_Res_Ptr(sub_res_ptr);
	 MBBMS_Free_Service_Request_Ptr(s_subscrible_request_ptr);
	 s_subscrible_request_ptr = PNULL;
}
/*****************************************************************************/
//  Description : handle Subscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleSubscribeSignal(DPARAM param)
{   
    mbbms_gba_data_t     *gba_res_ptr = PNULL;   
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2011_112_2_18_2_39_30_71,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2016_112_2_18_2_39_30_72,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2020_112_2_18_2_39_30_73,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_NONE);
        
        if(MBBMS_SERVICE_SUBSCRIBE == sig_data_ptr->subtype)
        {
            HandleSubscribeMainFlowFinish(param,MMIMBBMS_FLOW_STATE_STOPPING);        
        }
        else if(MBBMS_SERVICE_GBA == sig_data_ptr->subtype)
        {
            gba_res_ptr = (mbbms_gba_data_t*)sig_data_ptr->data_ptr;
            MBBMS_GBA_Free_Data(gba_res_ptr);
        }
        else
        {
            //NOTHING TO DO
        }
        
        if(s_uns_subscrible_request_ptr)
            MBBMS_Free_Service_Request_Ptr(s_uns_subscrible_request_ptr);
        s_subscrible_request_ptr = PNULL;
		
        if(MMK_IsOpenWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID))
            MMK_CloseWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID);		     
        if(MMK_IsOpenWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID))
            MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);
        
        if(s_is_mbbms_exiting)
        {
            MMIMBBMS_Clear();     
        }
        else
        {
            //NOTHING TO DO
        }
        
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status )
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_SUBSCRIBE:
            //主流程结束
            //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_SUBSCRIBE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2065_112_2_18_2_39_30_74,(uint8*)"");
            HandleSubscribeMainFlowFinish(param,MMIMBBMS_FLOW_STATE_STARTING);
            break;
             
        case MBBMS_SERVICE_PDP_ACTIVE:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2070_112_2_18_2_39_30_75,(uint8*)"");
            break;
            
        case MBBMS_SERVICE_GBA:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal subtype MBBMS_SERVICE_GBA"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2074_112_2_18_2_39_30_76,(uint8*)"");
            gba_res_ptr = (mbbms_gba_data_t*)sig_data_ptr->data_ptr;
            //NOTHING TO DO
            MBBMS_GBA_Free_Data(gba_res_ptr);
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal  subtype error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2081_112_2_18_2_39_30_77,(uint8*)"");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleSubscribeSignal  cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2087_112_2_18_2_39_30_78,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : handle stop subscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopSubscribeSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    //MBBMS_Service_Request_Res_t *sub_res_ptr = PNULL;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopSubscribeSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2102_112_2_18_2_39_30_79,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2107_112_2_18_2_39_30_80,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_NONE);
    
    MBBMS_Free_Service_Request_Ptr(s_subscrible_request_ptr);
    s_subscrible_request_ptr = PNULL;
	
    if(MMK_IsOpenWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID))
        MMK_CloseWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID);    
    if(MMK_IsOpenWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID))
        MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);    
    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();     
    }
}
/*****************************************************************************/
//  Description : HandleUnSubscribeMainflowFinish
//  Global resource dependence : none
//  Author: xiaoxiang.huang
//  Note:
/*****************************************************************************/
void HandleUnsubscribeMainFlowFinish(DPARAM param,MMIMBBMS_FLOW_STATE_E curr_status)
{
    anyURI  itemID = {0};
    anyURI  globalItemID = {0};
    wchar   wchar_name[MMIMBBMS_PURCHASE_NAME_MAX_LEN+1] = {0};
    MBBMSSG_PURCHASE_ITEM_T item_info = {0};
    MMI_STRING_T  prompt_str = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    mbbms_gba_data_t            *gba_res_ptr = PNULL;
    MBBMS_Service_Request_Res_t *unsub_res_ptr = PNULL;
    MBBMS_SERVICE_INFO_T        *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    unsub_res_ptr = (MBBMS_Service_Request_Res_t*)(sig_data_ptr->data_ptr);
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"HandleUnsubscribeMainFlowFinish: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2143_112_2_18_2_39_30_81,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"[MMIMBBMS]HandleUnsubscribeMainFlowFinish:start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2147_112_2_18_2_39_30_82,(uint8*)"");
	
    if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result)
	{
     //unsubscribe success
     MMI_STRING_T  prompt_str1 = {0};   
     MMI_STRING_T  prompt_str2 = {0};   
     uint32        purchase_index = 0;
     MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr = PNULL;
     MBBMS_Purchase_Table_Info_t* purchase_table_info_ptr = PNULL;	
     MBBMSSG_PURCHASE_ITEM_T* purchase_item_ptr = PNULL;
     uint32 anyURI_len = 0;
     anyURI   global_purchase_item_Id = {0};//套餐标识
     SG_ERROR_E purchase_item_error_code = SG_ERROR_NONE;
     anyURI   itemId = {0};//套餐分片标识 
     MBBMS_Account_Inquiry_Res_t* account_inquiry_res_ptr = MMIMBBMS_GetAccountInquiryRes();              

     MMI_GetLabelTextByLang(TXT_MBBMS_UNSUB_SUCCESS, &prompt_str1);
     MMI_GetLabelTextByLang(TXT_MBBMS_PURCHASEITEM_FOR_UNSUB, &prompt_str2);
     
     SCI_MEMSET(wchar_name, 0, sizeof(wchar_name));
     
     MMIAPICOM_Wstrncpy(wchar_name, prompt_str1.wstr_ptr, 
         prompt_str1.wstr_len);
     purchase_index = MMIMBBMS_GetCurrPurchaseIndex();    
     purchase_table_info_ptr = MMIMBBMS_GetPurchasedItemByIndex(purchase_index, account_inquiry_res_ptr);
      //SCI_TRACE_LOW:"[MMIMBBMS]HandleUnsubscribeMainFlowFinish purchase_table_info_ptr = 0x%x\n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2173_112_2_18_2_39_31_83,(uint8*)"d", purchase_table_info_ptr);
     if(PNULL != purchase_table_info_ptr)
     {
         purchase_item_ptr = (MBBMSSG_PURCHASE_ITEM_T*)
             STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_item, 0);            
         //如果查询返回信息中套餐分片信息或者套餐详细信息中一项为空，则从SG中查询	
         if(PNULL == purchase_item_ptr)
         {
             anyURI_len = 0;
             SCI_MEMSET(&global_purchase_item_Id, 0, sizeof(anyURI));
             global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];	     
             while(0 != global_purchase_item_Id.anyURI_arr[anyURI_len] && anyURI_len < (SG_MAX_URI_LEN - 1))
             {
                 anyURI_len++;
                 global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];
             }
             //SCI_TRACE_LOW:"[MMIMBBMS]HandleUnsubscribeMainFlowFinish anyURI_len = %d\n"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2189_112_2_18_2_39_31_84,(uint8*)"d", anyURI_len);
             if(0 == anyURI_len)
             {
                 //获取到的套餐标识为空
                 MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);	
                 return;
             }
             global_purchase_item_Id.anyURI_ptr = PNULL;
             global_purchase_item_Id.string_len = anyURI_len;
             MBBMSSG_GetPurchaseItemId(global_purchase_item_Id, &itemId);
         	
             purchase_item_error_code = MBBMSSG_GetPurchaseItem(itemId, &item_info);
             if(SG_ERROR_NONE != purchase_item_error_code)	
             {
                 MBBMSSG_FreePurchaseItem(&item_info);
                 MBBMSSG_FreeTypeAnyURI(&itemId);
                 MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);
                 //SCI_TRACE_LOW:"[MMIMBBMS]HandleUnsubscribeMainFlowFinish purchase_item_error_code =%dn"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2207_112_2_18_2_39_31_85,(uint8*)"d",purchase_item_error_code);
                 return;            
             }
             purchase_item_ptr = &item_info;	
         }   
         MMIMBBMS_GetNameWchar(purchase_item_ptr->NameList, wchar_name + prompt_str1.wstr_len,
             (MMIMBBMS_PURCHASE_NAME_MAX_LEN - prompt_str1.wstr_len));        
        
     }
     MMIAPICOM_Wstrncpy(wchar_name + MMIAPICOM_Wstrlen(wchar_name), 
         prompt_str2.wstr_ptr, 
         prompt_str2.wstr_len);   

     MBBMSSG_FreePurchaseItem(&item_info);
     MBBMSSG_FreeTypeAnyURI(&itemId);
     MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);	
     
     prompt_str.wstr_ptr = wchar_name;
     prompt_str.wstr_len = MMIAPICOM_Wstrlen(wchar_name);                  
#if 0                
    MMI_STRING_T hint_ok={0};
    uint8        global_id_len =0;
    MBBMS_Purchase_Item_t *purchase_item_ptr = PNULL;
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal:UnSubscrible successfully!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2231_112_2_18_2_39_31_86,(uint8*)"");
    purchase_item_ptr = unsub_res_ptr->purchase_item_ptr;
    global_id_len = strlen((char*)purchase_item_ptr->global_id_ref);
    
    if(SG_MAX_URI_LEN > global_id_len)
    {
        SCI_MEMCPY(globalItemID.anyURI_arr, purchase_item_ptr->global_id_ref,global_id_len);    
        
    }
    else
    {
        globalItemID.anyURI_ptr =  SCI_ALLOC_APP(global_id_len+1);
        if(PNULL == globalItemID.anyURI_ptr)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal alloc mem fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2245_112_2_18_2_39_31_87,(uint8*)"");
            return;
        }
        
        SCI_MEMSET(globalItemID.anyURI_ptr, 0x0, (global_id_len+1));
        SCI_MEMCPY(globalItemID.anyURI_ptr,purchase_item_ptr->global_id_ref,global_id_len);
    }
    
    globalItemID.string_len = global_id_len;        
    
    // SCI_STRCPY(globalItemID.anyURI_arr, unsub_res_ptr->purchase_item_ptr->global_id_ref);
    // globalItemID.string_len = strlen(globalItemID.anyURI_arr);
    MBBMSSG_GetPurchaseItemId(globalItemID, &itemID);
    
    MBBMSSG_GetPurchaseItem(itemID, &item_info);
    
    MMIMBBMS_GetNameWchar(item_info.NameList, wchar_name, 
        sizeof(wchar_name) - 1);
    
    prompt_str.wstr_ptr = wchar_name;
    MBBMSSG_FreePurchaseItem(&item_info);
    MBBMSSG_FreeTypeAnyURI(&itemID);
    
    //should change to purchase item name 
    MMI_GetLabelTextByLang(TXT_MBBMS_UNSUBSCRIBE_OK_HINT, &hint_ok);
    
    MMIAPICOM_Wstrcat(wchar_name, hint_ok.wstr_ptr);
    prompt_str.wstr_len = MMIAPICOM_Wstrlen(wchar_name);
    
    if(PNULL != globalItemID.anyURI_ptr)
    {
        SCI_FREE(globalItemID.anyURI_ptr );            
    }
#endif                
  /*   if(curr_status == MMIMBBMS_FLOW_STATE_STOPPING)
     {
		MMI_GetLabelTextByLang(TXT_MBBMS_UNSUBSCRIBE_CANCEL_FAIL, &prompt_str);
		MMIPUB_OpenAlertTextWinByTextPtr(PNULL,
			&prompt_str,
			PNULL,
			PNULL,
			MMIPUB_SOFTKEY_ONE,
			PNULL);
		#ifdef WIN32
		    SCI_Sleep(200);
		#endif
	
     }*/

	MMIPUB_OpenAlertTextWinByTextPtr(
		PNULL, 
		&prompt_str,
		PNULL, 
		PNULL, 
		MMIPUB_SOFTKEY_ONE,
		PNULL
		);

	#ifdef WIN32
	SCI_Sleep(500);
	#endif

	 


     /*由于2.0规范中已订购套餐信息是从查询的返回消息的结构体中获取的
     所以当订购或者退订以后我们没有发起重新查询之前是无法得到最新
     的订购信息的，这个时候如果直接返回原来界面显示结果是错误的，
     而直接后台查询的话因为查询有一定的时间，显示也不友好，所以先退出*/
     MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
     MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);
     MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
     
     //update display
     MMIMBBMS_FreeProgramList();
     MMIMBBMS_FreeServiceIndex();
     MMK_PostMsg(MMIMBBMS_SEARCH_RESULT_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
     MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
     MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
     MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
     MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);     
   }
   else
   {
       if(curr_status == MMIMBBMS_FLOW_STATE_STARTING)   
       {
		   //unsubscribe failed
		   //SCI_TRACE_LOW:"[MMIMBBMS]HandleUnsubscribeMainFlowFinish: unsubscrible failed!\n"
		   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2332_112_2_18_2_39_31_88,(uint8*)"");
		   MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
		   MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);
       
		   if(CheckDescriptionResValid(unsub_res_ptr, MBBMS_SERVICE_UNSUBSCRIBE))
		   {
			   DisplayErrorTipByDescription(unsub_res_ptr->description, unsub_res_ptr->description_len);
		   }
		   else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
		   {
			   MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
		   }
		   else
		   {
			   MMIMBBMS_DisplayCommonTip(TXT_MBBMS_UNSUBSCRIBE_FAILED, IMAGE_PUBWIN_WARNING);
		   }
	   }else
	   {
	       // NOTHING TO DO
	   }
   }
   
   MBBMS_Free_Service_Request_Res_Ptr(unsub_res_ptr);
   MBBMS_Free_Service_Request_Ptr(s_uns_subscrible_request_ptr);   
   s_uns_subscrible_request_ptr = PNULL;   
}
/*****************************************************************************/
//  Description : handle unsubscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleUnSubscribeSignal(DPARAM param)
{
    mbbms_gba_data_t            *gba_res_ptr = PNULL;
    MBBMS_SERVICE_INFO_T        *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2369_112_2_18_2_39_31_89,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2374_112_2_18_2_39_31_90,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2378_112_2_18_2_39_31_91,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        
        if(MBBMS_SERVICE_UNSUBSCRIBE == sig_data_ptr->subtype)
        {
            HandleUnsubscribeMainFlowFinish(param,MMIMBBMS_FLOW_STATE_STOPPING);         
        }
        else if(MBBMS_SERVICE_GBA == sig_data_ptr->subtype)
        {
            gba_res_ptr = (mbbms_gba_data_t*)sig_data_ptr->data_ptr;
            MBBMS_GBA_Free_Data(gba_res_ptr);
        }
        else
        {
            //NOTHING TO DO
        }
        if(s_uns_subscrible_request_ptr)
            MBBMS_Free_Service_Request_Ptr(s_uns_subscrible_request_ptr);
        s_uns_subscrible_request_ptr = PNULL;
        if(MMK_IsOpenWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID))
            MMK_CloseWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID);		
        if(MMK_IsOpenWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID))
            MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);
        if(s_is_mbbms_exiting)
        {  
            MMIMBBMS_Clear();            
        }
        else
        {
            //NOTHING TO DO
        }
        return;
    }
    
    if( MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status)
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_UNSUBSCRIBE:
            //主流程结束
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_UNSUBSCRIBE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2418_112_2_18_2_39_31_92,(uint8*)"");
            HandleUnsubscribeMainFlowFinish(param,MMIMBBMS_FLOW_STATE_STARTING); 
            break;

        case MBBMS_SERVICE_PDP_ACTIVE:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2423_112_2_18_2_39_31_93,(uint8*)"");
            break;

        case MBBMS_SERVICE_GBA:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal subtype MBBMS_SERVICE_GBA"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2427_112_2_18_2_39_31_94,(uint8*)"");
            gba_res_ptr = (mbbms_gba_data_t*)sig_data_ptr->data_ptr;
            //NOTHING TO DO
            MBBMS_GBA_Free_Data(gba_res_ptr);
            break;

        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal subtype error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2434_112_2_18_2_39_31_95,(uint8*)"");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleUnSubscribeSignal  cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2440_112_2_18_2_39_31_96,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : handle stop unsubscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopUnSubscribeSignal(DPARAM param)
{
    //MBBMS_Service_Request_Res_t *unsub_res_ptr = PNULL;
    MBBMS_SERVICE_INFO_T        *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopUnSubscribeSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2455_112_2_18_2_39_31_97,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopUnSubscribeSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2460_112_2_18_2_39_31_98,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    
    MBBMS_Free_Service_Request_Ptr(s_uns_subscrible_request_ptr);    
    s_uns_subscrible_request_ptr = PNULL;
    if(MMK_IsOpenWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID))
        MMK_CloseWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID);
    if(MMK_IsOpenWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID))
        MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);
    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();            
    }
}

/*****************************************************************************/
//  Description : handle Account inquiry signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleAccountInquirySignal(DPARAM param)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    MMIMBBMS_RESULT open_result = MMIMBBMS_MAX;
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleAccountInquirySignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2488_112_2_18_2_39_31_99,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleAccountInquirySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2493_112_2_18_2_39_31_100,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleAccountInquirySignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2497_112_2_18_2_39_31_101,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        
        s_account_inquiry_res_ptr = (MBBMS_Account_Inquiry_Res_t*)sig_data_ptr->data_ptr;
        MBBMS_Free_Account_Query_Res_Ptr(s_account_inquiry_res_ptr);
        s_account_inquiry_res_ptr = PNULL;
        
        MBBMS_Free_Account_Query_Ptr(s_account_inquiry_request_ptr);
        s_account_inquiry_request_ptr = PNULL;
        
        MMK_CloseWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID);
        if(s_is_mbbms_exiting)
        {
            MMIMBBMS_Clear();
        }
        else
        {
            //NOTHING TO DO
        }		
        return;
    }
    
    if( MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status )
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_ACCOUNT_QUIRY:
            //主流程结束
            //SCI_TRACE_LOW:"MMIMBBMS_HandleAccountInquirySignal subtype MBBMS_SERVICE_ACCOUNT_QUIRY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2525_112_2_18_2_39_31_102,(uint8*)"");
            s_account_inquiry_res_ptr = (MBBMS_Account_Inquiry_Res_t*)sig_data_ptr->data_ptr;
            
            if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result)
            {
                //account quiry success
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                
                //open_result = MMIMBBMS_OpenSubscribeMainWin();          
                open_result = MMIMBBMS_OpenSubscribeTabMainWin();
                if (MMIMBBMS_OK == open_result)
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_MBBMS_ACCOUNT_INQUIRY_OK);        
                    
                    MMK_CloseWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID);
                }
            }
            else
            {
                //account quiry failed
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                MMK_CloseWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID);
                
                if(CheckDescriptionResValid(s_account_inquiry_res_ptr, MBBMS_SERVICE_ACCOUNT_QUIRY))
                {
                    DisplayErrorTipByDescription(s_account_inquiry_res_ptr->description, s_account_inquiry_res_ptr->description_len);
                }
                else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
                {
                    MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    MMIMBBMS_DisplayCommonTip(TXT_MBBMS_ACCOUNT_INQUIRY_FAILED, IMAGE_PUBWIN_WARNING);
                }
            }
            
            MBBMS_Free_Account_Query_Ptr(s_account_inquiry_request_ptr);
            s_account_inquiry_request_ptr = PNULL;
            break;
            
        case MBBMS_SERVICE_PDP_ACTIVE:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleAccountInquirySignal subtype MBBMS_SERVICE_PDP_ACTIVE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2567_112_2_18_2_39_31_103,(uint8*)"");
            break;

        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleAccountInquirySignal subtype error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2571_112_2_18_2_39_31_104,(uint8*)"");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleAccountInquirySignal  cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2577_112_2_18_2_39_31_105,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : handle stop Account inquiry signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopAccountInquirySignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopAccountInquirySignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2591_112_2_18_2_39_31_106,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopAccountInquirySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2596_112_2_18_2_39_31_107,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    
    MBBMS_Free_Account_Query_Ptr(s_account_inquiry_request_ptr);
    s_account_inquiry_request_ptr = PNULL;
    
    MMK_CloseWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID);
    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
    }
}

/*****************************************************************************/
//     Description : Get Name from name list according to language type
//    Global resource dependence :
//  Author: 
//    Note:
//should check index
/*****************************************************************************/
LOCAL void MMIMBBMS_GetNameFromNameList(
                                        SG_LIST_PTR_T name_list_head, //in
                                        uint8   lang_type_code, //in
                                        uint8** name_string,
                                        uint32* string_len_ptr
                                        )
{
    SG_NODE_T* temp=PNULL;
    languageString* name_ptr=NULL;    
    
    if(PNULL == name_list_head)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetNameFromNameList: name_list_head is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2627_112_2_18_2_39_31_108,(uint8*)"");
        return;
    }
    
    temp = name_list_head->next_ptr;
    
    while(temp)
    {
        name_ptr =(languageString*)temp->data;
        if(PNULL!=name_ptr) //&&(lang_type==name_ptr->lang_type))
        {
            *string_len_ptr = name_ptr->string_len;
            if( SG_MAX_LANGUAGE_CONTENT_LEN < name_ptr->string_len)
            {
                *name_string = name_ptr->content_string_ptr;
                return;
            }
            else
            {
                *name_string = name_ptr->string_arr;
                return;
            }
        }
        
        temp=temp->next_ptr;
    }    
    
    *name_string = PNULL;
    *string_len_ptr = 0;
    
    return;
}

/*****************************************************************************/
//     Description : Get Purchase Item name by index
//    Global resource dependence :
//  Author: 
//    Note:
//should check index
/*****************************************************************************/
LOCAL uint8* MMIMBBMS_GetPurchaseNameByIndex(
                                             SG_LIST_PTR_T purchaseindex_list_head,//in
                                             int32   index,	//in		
                                             uint8   lang_type_code,
                                             uint32* string_len)
{
    uint8* name_ptr = PNULL;
    MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr= PNULL;
    
    if(PNULL == purchaseindex_list_head)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetPurchaseNameByIndex: purchaseindex_list_head is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2676_112_2_18_2_39_32_109,(uint8*)"");
        return PNULL;
    }
    
    purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)STL_LinkListGetNodeDataByIdx(purchaseindex_list_head,index);    
    
    if((PNULL!=purchase_index_ptr)&&(PNULL!=purchase_index_ptr->NameList))
    {
        MMIMBBMS_GetNameFromNameList(purchase_index_ptr->NameList,lang_type_code,
            &name_ptr, string_len);
        
        return name_ptr;
    }
    
    *string_len = 0;
    
    return PNULL;
}


/*****************************************************************************/
//     Description : Get price from price list according to language type
//    Global resource dependence :
//  Author: 
//    Note: @check for s_currency_type
/*****************************************************************************/
LOCAL SG_MONETARY_PRICE_T* MMIMBBMS_GetPriceInfoFromPriceList(
                                                              SG_LIST_PTR_T price_list_head, //in
                                                              uint8   sel_currency  //in
                                                              )
{
    SG_NODE_T* temp =PNULL;
    SG_MONETARY_PRICE_T* price_ptr=PNULL;    
    
    if(PNULL == price_list_head)
    {
        return PNULL;
    }
    
    temp = price_list_head->next_ptr;
    
    if (temp != PNULL)
    {
        price_ptr =(SG_MONETARY_PRICE_T*)temp->data;
        
        return price_ptr;
    }    
    return PNULL;     
    
}

/*****************************************************************************/
//     Description : Get the price of purchase item  by index
//    Global resource dependence :
//  Author: 
//    Note:
//should check index
/*****************************************************************************/
LOCAL void MMIMBBMS_GetPurchasePriceByIndex(
                                            SG_LIST_PTR_T purchaseindex_list_head,//in
                                            int32 index,    //in        
                                            uint8   currency, //in
                                            float* price,  //out
                                            MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E *subscriptionType,  //out
                                            uint8           *period  //out
                                            )
{
    
    MBBMSSG_PURCHASEITEM_INDEX_T *purchase_index_ptr = PNULL;    
    MBBMSSG_PURCHASE_DETAIL_T      purchase_detail =  {0};
    SG_MONETARY_PRICE_T*            price_info_ptr = PNULL;
    
    if(PNULL == purchaseindex_list_head)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetPurchasePriceByIndex: purchaseindex_list_head is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2746_112_2_18_2_39_32_110,(uint8*)"");
        return;
    }
    
    purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)STL_LinkListGetNodeDataByIdx(purchaseindex_list_head,index);    
    
    if(PNULL!=purchase_index_ptr)
    {        
        //get  detail info purchase
        MBBMSSG_GetPurchaseDetail(purchase_index_ptr->index, &purchase_detail);    
        
        *subscriptionType = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail.PriceInfo.subscriptionType;
        *period = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail.PriceInfo.period;
        
        if(PNULL!= purchase_detail.PriceInfo.MonetaryPriceList)
        {
            
            price_info_ptr = MMIMBBMS_GetPriceInfoFromPriceList(
                purchase_detail.PriceInfo.MonetaryPriceList,
                currency);
            
            if(PNULL!=price_info_ptr)
            {
                *price =price_info_ptr->price;
            }
            else
            {
                //not find specific currcency type;
                //SCI_TRACE_LOW:"MMIMBBMS_GetPurchasePriceByIndex not find price info according to currency type"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2774_112_2_18_2_39_32_111,(uint8*)"");
            }
        }
        
        MBBMSSG_FreePurchaseDetail(&purchase_detail);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetPurchasePriceByIndex.....get node failed!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2782_112_2_18_2_39_32_112,(uint8*)"");
    }
    
}


/*****************************************************************************/
//     Description : Get whether purchase is ordered or not
//    Global resource dependence :
//  Author: 
//    Note:
//should check index
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetPurchaseOrderStatusByIndex(
                                                     SG_LIST_PTR_T purchaseindex_list_head,   
                                                     uint32 index 
                                                     )
{
    
    MBBMSSG_PURCHASEITEM_INDEX_T *purchase_index_ptr = PNULL;    
    BOOLEAN is_ordered=FALSE;
    
    if(PNULL == purchaseindex_list_head)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetPurchaseOrderStatusByIndex: purchaseindex_list_head is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2804_112_2_18_2_39_32_113,(uint8*)"");
        return;
    }
    
    if(index>=purchaseindex_list_head->node_cnt)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetPurchaseOrderStatusByIndex: index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2810_112_2_18_2_39_32_114,(uint8*)"");
        return FALSE;
    }
    
    purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)STL_LinkListGetNodeDataByIdx(purchaseindex_list_head,index);    
    
    if(PNULL!=purchase_index_ptr)
    {
        //according to purchaseitem id to get purchase order status
        return  MMIMBBMS_IsPurchaseItemOrdered(purchase_index_ptr->index);
        
    }
    
    //SCI_TRACE_LOW:"[MMIMBBMS] Get purchase item order status=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2823_112_2_18_2_39_32_115,(uint8*)"d",is_ordered);
    
    return is_ordered;
    
}

/*****************************************************************************/
// Description : 逐个获取套餐信息并显示
// Global resource dependence :
// Author: 
// Note:
//should check index
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayAllPurchaseItem(uint32 purchase_index, MMI_CTRL_ID_T ctrl_id)
{
    int index=0;
    int purchase_num=0;
    anyURI globalItemID = {0};
    SG_ERROR_E ret = SG_ERROR_NONE;
    BOOLEAN    is_subscribed = FALSE;
    SG_LIST_PTR_T purchaseIndex_list_head_ptr = PNULL;
    MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T  purchase_display_info={0};
    uint8 *purchase_name =PNULL; //purchase name,UTF8 format
    MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr= PNULL;
    uint32 string_len = 0;
    
    ret = MBBMSSG_GetPurchaseItemIndex(&purchaseIndex_list_head_ptr);
    
    if(SG_ERROR_NONE !=ret)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchaseItem:Get Purchase Item failed\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2851_112_2_18_2_39_32_116,(uint8*)"");
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        return;
    }
    
    purchase_num = STL_LinkListGetNodeDataCount(purchaseIndex_list_head_ptr);
    if(0==purchase_num)
    {
        //套餐列表为空
        //SCI_TRACE_LOW:"---no purchaseitem\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2860_112_2_18_2_39_32_117,(uint8*)"");
        //should handle @plum
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        return ;
    }            
    
    GUILIST_SetMaxItem(ctrl_id, purchase_num, FALSE);
    
    //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchaseItem total purchase num=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2868_112_2_18_2_39_32_118,(uint8*)"d",purchase_num);
    
    //Get every purchase item 
    for(index=0; index<purchase_num; index++)
    {
        purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)
            STL_LinkListGetNodeDataByIdx(purchaseIndex_list_head_ptr,index);
        
        if (PNULL == purchase_index_ptr)
        {
            continue;
        }
        
        MBBMSSG_GetGlobalPurchaseItemtId(purchase_index_ptr->index, &globalItemID);
        ret = MBBMSSG_IsPurchaseItemSubscribed(purchase_index_ptr->index, &is_subscribed);
        
        // 本地套餐包含所有套餐 , 删除 MMIMBBMS_IsLocalPurchaseItem(&globalItemID) == TRUE
        if ( !((MMIMBBMS_SUBSCRIPTE_TYPE_LOCAL_PURCHASE == purchase_index && (!is_subscribed))
            || ((MMIMBBMS_SUBSCRIPTE_TYPE_PURCHASED == purchase_index) && MMIMBBMS_IsNationalPurchaseItem(&globalItemID)
            &&( ret == SG_ERROR_NONE) &&  is_subscribed)))
        {
            MBBMSSG_FreeTypeAnyURI(&globalItemID);
            continue;
        }
        MBBMSSG_FreeTypeAnyURI(&globalItemID);
        SCI_MEMSET(&purchase_display_info,0,sizeof(MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T));
        //get purchase name of every purchase                       
        purchase_name= MMIMBBMS_GetPurchaseNameByIndex(
            purchaseIndex_list_head_ptr,
            index,
            s_lang_type, &string_len);
        
        if(PNULL==purchase_name)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchaseItem purchase name is null\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2902_112_2_18_2_39_32_119,(uint8*)"");
            continue;
        }
        
        SCI_MEMSET(purchase_display_info.purchase_name, 0, sizeof(purchase_display_info.purchase_name));
        
        string_len = MIN(string_len,MMIMBBMS_PURCHASE_NAME_MAX_LEN);
        
        SCI_MEMCPY(purchase_display_info.purchase_name,purchase_name,string_len);
        
        //get purchase order time,there is no such field in sg,should check
        
        //get purchase order status
        purchase_display_info.is_ordered= MMIMBBMS_GetPurchaseOrderStatusByIndex(purchaseIndex_list_head_ptr,index);
        
        MMIMBBMS_GetPurchasePriceByIndex(
            purchaseIndex_list_head_ptr, 
            index,  
            s_currency_type, 
            &purchase_display_info.price, 
            &purchase_display_info.scription_type,
            &purchase_display_info.period);
        //get purchase period        
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchaseItem purchase_display_info.period = %d, purchase_display_info.scription_type = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2925_112_2_18_2_39_32_120,(uint8*)"dd", purchase_display_info.period, purchase_display_info.scription_type);
        
        //add an Purchase item to list or richtext @plum
        MMIMBBMS_AddPurchaseItemToListboxCtrl(ctrl_id, &purchase_display_info);
        
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchaseItem complete add purchase item\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2932_112_2_18_2_39_32_121,(uint8*)"");
    MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
    
}

/*****************************************************************************/
//     Description : MMIMBBMS_IsNationalPurchaseItem
//    Global resource dependence : none
//  Author: plum.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsLocalPurchaseItem(anyURI* pGlobalPurchaseItemID)
{
    uint8* str_buf = PNULL;
    
    if(PNULL == pGlobalPurchaseItemID)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_IsLocalPurchaseItem: pGlobalPurchaseItemID is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_2947_112_2_18_2_39_32_122,(uint8*)"");
        return;
    }
    
    if ( SG_MAX_URI_LEN > pGlobalPurchaseItemID->string_len )
    {
        str_buf = pGlobalPurchaseItemID->anyURI_arr;
    }
    else
    {
        str_buf = pGlobalPurchaseItemID->anyURI_ptr;
    }
    
    if(( '0' == str_buf[0]) && ( '0' == str_buf[1] ))
    {
        
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//     Description : MMIMBBMS_IsNationalPurchaseItem
//    Global resource dependence : none
//  Author:  plum.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsNationalPurchaseItem(anyURI* pGlobalPurchaseItemID)
{
    //uint8* str_buf = PNULL;
    
    return TRUE;
    /*
    if (pGlobalPurchaseItemID->string_len < SG_MAX_URI_LEN)
    {
    str_buf = pGlobalPurchaseItemID->anyURI_arr;
    }
    else
    {
    str_buf = pGlobalPurchaseItemID->anyURI_ptr;
    }
    
      if (str_buf[0] == '0' && str_buf[1] == '0' && str_buf[2] == '\0')
      {
      
        return TRUE;
        }
        
          return FALSE;
    */
}

/*****************************************************************************/
//  Description : set/update current operating purchase info via global index
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurrPurchaseDisplayInfo(uint32 index)
{
    uint32 str_len = 0;
    uint8* purchase_name_ptr = PNULL;
    SG_ERROR_E err_code =  SG_ERROR_NONE;
    SG_LIST_PTR_T purchaseIndex_list_head_ptr=PNULL;
    
    err_code=MBBMSSG_GetPurchaseItemIndex(&purchaseIndex_list_head_ptr);
    
    if(SG_ERROR_NONE != err_code)
    {
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        //SCI_TRACE_LOW:"MMIMBBMS_SetCurrPurchaseDisplayInfo:Get purchase item index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3014_112_2_18_2_39_32_123,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(&s_curr_operation_purchase,0,sizeof(MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T));
    SCI_MEMSET(s_curr_operation_purchase.purchase_name,0,sizeof(s_curr_operation_purchase.purchase_name));
    
    s_curr_operation_purchase.index = index;    
    s_curr_operation_purchase.currency=s_currency_type;
    
    purchase_name_ptr=MMIMBBMS_GetPurchaseNameByIndex(
        purchaseIndex_list_head_ptr,
        index,
        s_lang_type, &str_len );
    
    if (PNULL == purchase_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetCurrPurchaseDisplayInfo:purchase_name_ptr is null\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3031_112_2_18_2_39_32_124,(uint8*)"");
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        return;
    }
    
    str_len = MIN(str_len,MMIMBBMS_PURCHASE_NAME_MAX_LEN);
    
    SCI_MEMCPY(s_curr_operation_purchase.purchase_name,purchase_name_ptr, str_len);
    
    MMIMBBMS_GetPurchasePriceByIndex(
        purchaseIndex_list_head_ptr,
        index, 
        s_currency_type, 
        &s_curr_operation_purchase.price, 
        &s_curr_operation_purchase.scription_type,
        &s_curr_operation_purchase.period);
    
    s_curr_operation_purchase.is_ordered = MMIMBBMS_GetPurchaseOrderStatusByIndex(purchaseIndex_list_head_ptr, index);
    MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
    
}


/*****************************************************************************/
//     Description : get current purchase order status
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_GetCurrPurchaseOrdereStatus(void)
{
    return s_curr_operation_purchase.is_ordered;
}

/*****************************************************************************/
//     Description : get the current purchase index in richtext ctrl list
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetCurrPurchaseIndex(void)
{
    return s_curr_operation_purchase.index;
}

/*****************************************************************************/
//     Description : get the current purchase name
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8* MMIMBBMS_GetCurrPurchaseName(void)
{
    return s_curr_operation_purchase.purchase_name;
}

/*****************************************************************************/
//     Description : get the current purchase name
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC float MMIMBBMS_GetCurrPurchasePrice(void)
{
    
    return s_curr_operation_purchase.price;
}

/*****************************************************************************/
//     Description : get the current purchase subscription type
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E MMIMBBMS_GetCurrPurchaseSubscriptionType(void)
{
    
    return s_curr_operation_purchase.scription_type;
}

/*****************************************************************************/
//     Description : get the current purchase subscription type
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetCurrPurchaseCurrency(void)
{
    return s_curr_operation_purchase.currency;
}

/*****************************************************************************/
//     Description : display service in purchase
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_DisplayServiceInPurchase(SG_LIST_PTR_T service_idref_list_head, MMI_CTRL_ID_T ctrl_id)
{
    anyURI*    service_idref_ptr=PNULL;
    SG_LIST_PTR_T temp =service_idref_list_head;    
    
    if(PNULL == temp)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceInPurchase: temp is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3121_112_2_18_2_39_32_125,(uint8*)"");
        return;
    }
    
    temp = temp->next_ptr;
    while(temp)
    {
        service_idref_ptr = (anyURI*)(temp->data);
        if(PNULL!=service_idref_ptr)
        {
            MBBMSSG_SERVICE_T service={0};
            
            MBBMSSG_GetService(*service_idref_ptr, &service);
            //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceInPurchase service.NameList =0x%x, len=%dn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3135_112_2_18_2_39_32_126,(uint8*)"dd", service.NameList,STL_LinkListGetNodeDataCount(service.NameList));
            if(STL_LinkListGetNodeDataCount(service.NameList)>0)
            {
                uint32 string_len = 0;
                uint8* service_name_ptr=PNULL;
                
                MMIMBBMS_GetNameFromNameList(service.NameList, s_lang_type,
                    &service_name_ptr, &string_len);
                
                if (PNULL!= service_name_ptr)
                {
                    MMIMBBMS_AddPurchaseContentToRchtextCtrl(ctrl_id,
                        service_name_ptr, string_len);
                }
            }
            
            MBBMSSG_FreeService(&service);
        }
        
        temp=temp->next_ptr;
    }
    
}


/*****************************************************************************/
//     Description : display content in purchase
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_DisplayContentInPurchase(SG_LIST_PTR_T content_idref_list_head, MMI_CTRL_ID_T ctrl_id)
{
    anyURI*    content_idref_ptr=PNULL;    
    SG_NODE_T* temp =content_idref_list_head;    
    
    if(PNULL == temp)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayContentInPurchase: temp is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3171_112_2_18_2_39_33_127,(uint8*)"");
        return;
    }
    
    temp = temp->next_ptr;
    while(temp)
    {
        content_idref_ptr = (anyURI*)(temp->data);
        if(PNULL!=content_idref_ptr)
        {
            MBBMSSG_CONTENT_T  content={0};
            
            MBBMSSG_GetContent(*content_idref_ptr, &content);
            
            if(0 < STL_LinkListGetNodeDataCount(content.NameList))            
            {
                uint32 string_len = 0;
                uint8* content_name_ptr=PNULL;
                
                MMIMBBMS_GetNameFromNameList(content.NameList, s_lang_type,
                    &content_name_ptr, &string_len);
                
                //@plum,add item to richtext
                if (PNULL != content_name_ptr)
                {
                    MMIMBBMS_AddPurchaseContentToRchtextCtrl(ctrl_id,
                        content_name_ptr, string_len);
                }
            }
            
            MBBMSSG_FreeContent(&content);
        }
        temp=temp->next_ptr;
    }
    
    return;
}

/*****************************************************************************/
//     Description : to display all services or contents in purchase item to richtext
//    Global resource dependence : none
//  Author: 
//    Note: 显示套餐详情，（套餐名、包含的所有频道或节目、订购时间、资费）
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayServiceOrContentInPurchase(MMI_CTRL_ID_T ctrl_id)
{
    wchar  wchar_name[MMIMBBMS_PURCHASE_NAME_MAX_LEN+1] = {0};
    uint32 purchase_index = 0;
    uint32 str_len = 0;
    MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr = PNULL;
    MBBMSSG_PURCHASE_ITEM_T    purchase_item = {0};
    uint8 *purchase_name =PNULL; //purchase name,UTF8 format
    MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T  purchase_display_info={0};
    
    SG_ERROR_E err_code = SG_ERROR_NONE;
    SG_LIST_PTR_T purchaseIndex_list_head_ptr=PNULL;
    uint8 *purchase_description_ptr =PNULL; //purchase name,UTF8 format    
    uint32 description_str_len = 0;	
    MMI_STRING_T mbbms_string_description = {0};   
    MMI_STRING_T mbbms_string_description1 = {0};
    MMI_STRING_T mbbms_string_description2 = {0};    
    wchar temp_wchar_des[GUILIST_STRING_MAX_NUM+1] = {0};
    uint8 temp_char_des[GUILIST_STRING_MAX_NUM+1] = {0};
    wchar temp_wchar_des2[GUILIST_STRING_MAX_NUM+1] = {0};    
    
    err_code=MBBMSSG_GetPurchaseItemIndex(&purchaseIndex_list_head_ptr);
    
    if( SG_ERROR_NONE != err_code)
    {
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchase:Get purchase item index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3239_112_2_18_2_39_33_128,(uint8*)"");
        return;
    }
    
    //get current purchase index
    purchase_index=MMIMBBMS_GetCurrPurchaseIndex();    
    //get purchase index node
    purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)STL_LinkListGetNodeDataByIdx(
        purchaseIndex_list_head_ptr,
        purchase_index);
    
    if(PNULL!=purchase_index_ptr)
    {
        //get purchase item
        MBBMSSG_GetPurchaseItem(purchase_index_ptr->index, &purchase_item);
        
        MMIMBBMS_GetNameWchar(purchase_item.NameList, wchar_name, 
            sizeof(wchar_name) - 1);
        
        MMIMBBMS_AddPurchaseDetailHead(wchar_name, ctrl_id);
        
        //to show service ref list                      
        if((PNULL!=purchase_item.ServiceIdRefList)
            &&(STL_LinkListGetNodeDataCount(purchase_item.ServiceIdRefList)>0))
        {
            MMIMBBMS_DisplayServiceInPurchase(purchase_item.ServiceIdRefList, ctrl_id);
        }	
        //to show purchase description
        if(PNULL != purchase_item.DescriptionList)
        {
            MMIMBBMS_GetNameFromNameList(purchase_item.DescriptionList, s_lang_type, 
                &purchase_description_ptr, &description_str_len);
            if(PNULL != purchase_description_ptr && 0 != description_str_len)
            {
               
               MMI_GetLabelTextByLang(TXT_MBBMS_PURCHASE_DESCRIPTION, &mbbms_string_description1);
               //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description1.wstr_len =%d\n"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3275_112_2_18_2_39_33_129,(uint8*)"d", mbbms_string_description1.wstr_len);
                mbbms_string_description2.wstr_len = GUI_UTF8ToWstr(temp_wchar_des,
                                                                        (uint16)GUILIST_STRING_MAX_NUM,
                                                                        purchase_description_ptr, 
                                                                        (uint16)description_str_len);

               mbbms_string_description2.wstr_ptr = temp_wchar_des;
               //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description2.wstr_len =%d\n"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3282_112_2_18_2_39_33_130,(uint8*)"d", mbbms_string_description2.wstr_len);

               memset(temp_wchar_des2, 0, sizeof(temp_wchar_des2));
               mbbms_string_description.wstr_ptr = temp_wchar_des2;
               MMIAPICOM_MergeTwoString(&mbbms_string_description1, &mbbms_string_description2, &mbbms_string_description, GUILIST_STRING_MAX_NUM);
               //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchase purchase mbbms_string_description.wstr_len =%d\n"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3287_112_2_18_2_39_33_131,(uint8*)"d", mbbms_string_description.wstr_len);

               description_str_len = GUI_WstrToUTF8(temp_char_des,
                                                (uint16)GUILIST_STRING_MAX_NUM,
                                                mbbms_string_description.wstr_ptr, 
                                                (uint16)mbbms_string_description.wstr_len);     
               MMIMBBMS_AddPurchaseContentToRchtextCtrl(ctrl_id, temp_char_des, description_str_len);
            }            
#if 0            
            if(PNULL != purchase_description_ptr)
            {
                MMIMBBMS_AddPurchaseContentToRchtextCtrl(ctrl_id, purchase_description_ptr, description_str_len);
            }
#endif            
        }        	
        //to show content ref list
        if((PNULL!=purchase_item.ContentIdRefList)
            &&(STL_LinkListGetNodeDataCount(purchase_item.ContentIdRefList)>0))
        {
            MMIMBBMS_DisplayContentInPurchase(purchase_item.ContentIdRefList, ctrl_id);
        }
        //get purchase name of every purchase                       
        purchase_name= MMIMBBMS_GetPurchaseNameByIndex(
            purchaseIndex_list_head_ptr,
            purchase_index,
            s_lang_type, &str_len);
        
        SCI_MEMSET(purchase_display_info.purchase_name, 0, sizeof(purchase_display_info.purchase_name));
        
        str_len = MIN(str_len,MMIMBBMS_PURCHASE_NAME_MAX_LEN);
        
        SCI_MEMCPY(purchase_display_info.purchase_name,purchase_name, str_len);
        
        //get purchase order time,there is no such field in sg,should check
        
        //get purchase order status
        purchase_display_info.is_ordered= MMIMBBMS_GetPurchaseOrderStatusByIndex(purchaseIndex_list_head_ptr,purchase_index);
        
        //get purchase cost info
        MMIMBBMS_GetPurchasePriceByIndex(
            purchaseIndex_list_head_ptr, 
            purchase_index,  
            s_currency_type, 
            &purchase_display_info.price, 
            &purchase_display_info.scription_type,
            &purchase_display_info.period);      
        
        MMIMBBMS_AddPurchaseDetailTail(&purchase_display_info, ctrl_id);
        
        MBBMSSG_FreePurchaseItem(&purchase_item);
    }
    
    MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
    return;
}

/*****************************************************************************/
//  Description : subscribe  purchase item via index
//  Global resource dependence : none
//  Author:  xin.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_SubscriblePurchaseItem(void)
{
    
    MBBMSSG_PURCHASEITEM_INDEX_T    *purchase_index_ptr=PNULL;
    MBBMS_Purchase_Data_Reference_Service_t *data_ref_ptr=PNULL;
    anyURI  globalPurchaseItemId={0};    
    uint32  purchase_index=0;
    uint8   str_len = MBBMS_IDREF_LEN;    
    
    SG_ERROR_E      err_code = SG_ERROR_NONE;
    SG_LIST_PTR_T   purchaseIndex_list_head_ptr=PNULL;
    MBBMS_SERVICE_RET_E request_ret = MBBMS_SERVICE_FUNC_OK;
    
    err_code = MBBMSSG_GetPurchaseItemIndex(&purchaseIndex_list_head_ptr);
    
    if(SG_ERROR_NONE != err_code)
    {
        //SCI_TRACE_LOW:"MBBMSSG_GetPurchaseItemIndex:Get purchase item index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3364_112_2_18_2_39_33_132,(uint8*)"");
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);        
        return FALSE;
    }
    
    purchase_index=MMIMBBMS_GetCurrPurchaseIndex();
    //get purchase index info
    purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)STL_LinkListGetNodeDataByIdx(purchaseIndex_list_head_ptr, purchase_index);
    
    if(PNULL == purchase_index_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SubscriblePurchaseItem: get purchase info error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3375_112_2_18_2_39_33_133,(uint8*)"");
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        return FALSE;
    }
    
    if( PNULL != s_subscrible_request_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SubscriblePurchaseItem: s_subscrible_request_ptr  is not null\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3382_112_2_18_2_39_33_134,(uint8*)"");
        MBBMS_Free_Service_Request_Ptr(s_subscrible_request_ptr);
        
        s_subscrible_request_ptr = PNULL;
    }
    
    s_subscrible_request_ptr = (MBBMS_Service_Request_t*)SCI_ALLOC_APP(sizeof(MBBMS_Service_Request_t));
    if(PNULL == s_subscrible_request_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SubscriblePurchaseItem: s_subscrible_request_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3391_112_2_18_2_39_33_135,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(s_subscrible_request_ptr, 0, sizeof(MBBMS_Service_Request_t));
    
    s_subscrible_request_ptr->request_id_valid = FALSE;        
    s_subscrible_request_ptr->data_ref_ptr = (MBBMS_Purchase_Data_Reference_Service_t*)SCI_ALLOC_APP(sizeof(MBBMS_Purchase_Data_Reference_Service_t));
    if(PNULL == s_subscrible_request_ptr->data_ref_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SubscriblePurchaseItem: data_ref_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3400_112_2_18_2_39_33_136,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET(s_subscrible_request_ptr->data_ref_ptr, 0, sizeof(MBBMS_Purchase_Data_Reference_Service_t));
    
    data_ref_ptr = s_subscrible_request_ptr->data_ref_ptr;
    
    data_ref_ptr->have_id_ref   = FALSE;
    data_ref_ptr->have_price    = TRUE;
    data_ref_ptr->price         = (double)MMIMBBMS_GetCurrPurchasePrice();
    data_ref_ptr->next          = PNULL;
    //get global index
    SCI_MEMSET(&globalPurchaseItemId, 0, sizeof(anyURI));
    MBBMSSG_GetGlobalPurchaseItemtId(purchase_index_ptr->index, &globalPurchaseItemId);
    //should copy global id    
    
    str_len = MIN(str_len,SG_MAX_URI_LEN);
    
    if(PNULL == globalPurchaseItemId.anyURI_ptr)
    {
        SCI_MEMCPY(data_ref_ptr->gloabl_id_ref, globalPurchaseItemId.anyURI_arr, str_len);        
    }
    else
    {
        SCI_MEMCPY(data_ref_ptr->gloabl_id_ref, globalPurchaseItemId.anyURI_ptr, str_len);        
    }
    //SCI_TRACE_LOW:"MBBMSSG_GetGlobalPurchaseItemtId gloabl_id=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3427_112_2_18_2_39_33_137,(uint8*)"s",data_ref_ptr->gloabl_id_ref);
       
   //should confirm here
   request_ret = MTVSERVICE_ServiceRequest(s_subscrible_request_ptr);
   //SCI_TRACE_LOW:"MBBMS_Service_Request return = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3431_112_2_18_2_39_33_138,(uint8*)"d", request_ret);
   if(MBBMS_SERVICE_FUNC_OK != request_ret)    
   {
       //should free global purchase item ptr here
       //SCI_TRACE_LOW:"MMIMBBMS_SubscriblePurchaseItem:Service Request error\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3435_112_2_18_2_39_33_139,(uint8*)"");
       MBBMSSG_FreeTypeAnyURI(&globalPurchaseItemId);
       MBBMS_Free_Service_Request_Ptr(s_subscrible_request_ptr);
       MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
       s_subscrible_request_ptr = PNULL;
       
       return FALSE;
   }
    
    MBBMSSG_FreeTypeAnyURI(&globalPurchaseItemId);
    MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
    return TRUE;
}

/*****************************************************************************/
//     Description : whether sd card is inserted and service is availbale
//    Global resource dependence : none
//  Author:  lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_SDandOpenCondition(void)
{
    if(!MUACMD_MBBMS_IsEnabled())
    {
        //提示用户插入SD卡
        MMIMBBMS_OpenInsertSDCardAlertWin();
        return FALSE;
    }
    
    //SCI_TRACE_LOW:" MMIMBBMS_SDandOpenCondition OK!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3462_112_2_18_2_39_33_140,(uint8*)"");
    return TRUE;
}

/*****************************************************************************/
//     Description : set softkey of sd card insert alert win
//    Global resource dependence : none
//    Author:  xiaoxiang.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleInsertSDCardAlertWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
}

PUBLIC  void MMIMBBMS_OpenInsertSDCardAlertWin(void)
{
    MMIPUB_OpenAlertTextWinByTextId(PNULL,TXT_MBBMS_SD_CARD_MISSING,PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,MMIMBBMS_HandleInsertSDCardAlertWinMsg);
    
}

/*****************************************************************************/
// 	Description : to handle alert window closed message
//	Global resource dependence : 
//  Author:lisa.lin
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSGBackgroundParseWaitingWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
        
    case MSG_APP_RED:	 	
    case MSG_APP_CANCEL: 
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle manual update sg signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleManualUpdateSGSignal(DPARAM param)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    MBBMS_Subscription_Update_Res_t *sub_update_res_ptr = PNULL;
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3537_112_2_18_2_39_33_141,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3542_112_2_18_2_39_33_142,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3546_112_2_18_2_39_33_143,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        MMK_CloseWin(MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID);
        
        if (s_is_mbbms_exiting)
        {
            //该流程结束只需关注STOP_CNF消息
            //MMIMBBMS_Clear();
        }
        else
        {
            //NOTHING TO DO
        }
        
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal  cur_service_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3563_112_2_18_2_39_33_144,(uint8*)"d", MMIMBBMS_GetServiceMode());
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status)
    {
        //mbbms mode include subtype:MBBMS_SERVICE_UPDATE_SG/MBBMS_SERVICE_PARSE_SG/MBBMS_SERVICE_UPDATE_TABLE
        //cmmb  mode include subtype:CMMB_SEARCH_PROG/MBBMS_SERVICE_MANUAL_SG(for cmmb esg)
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_MANUAL_SG:
            //主流程结束
            //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_MANUAL_SG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3572_112_2_18_2_39_34_145,(uint8*)"");
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
            MMIMBBMS_SetSGOperationFromType(MMIMBBMS_SG_SEARCH_PARSE_FROM_NONE);
            
            if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
            {
                MMIMBBMS_UpdateWelcomeInfo();
            }
            
            //refresh main win when manual update sg finish
            MMIMBBMS_FreeProgramList();
            MMIMBBMS_FreeServiceIndex();
            MMIMBBMS_RefreshMainInterface();
            MMK_CloseWin(MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID);
            break;
            
        case MBBMS_SERVICE_UPDATE_SG:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_UPDATE_SG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3589_112_2_18_2_39_34_146,(uint8*)"");
            if(MTVSERVICE_RST_SG_RCV_FAIL == sig_data_ptr->result
                || MTVSERVICE_RST_PDP_FAIL == sig_data_ptr->result)
            {
                //提示信号问题导致更新失败
                MMIMBBMS_DisplayCommonTip(TXT_MBBMS_BAD_SIGNAL, IMAGE_PUBWIN_WARNING);
            }
            else
            {
                //NOTHING TO DO
            }
            break;
            
        case MBBMS_SERVICE_PARSE_SG:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_PARSE_SG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3603_112_2_18_2_39_34_147,(uint8*)"");
            if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result
                || MTVSERVICE_RST_NO_DATA == sig_data_ptr->result
                || MTVSERVICE_RST_INCOMPLETE_DATA == sig_data_ptr->result)
            {
                //NOTHING TO DO
            }
            else if(MTVSERVICE_RST_NO_MEM == sig_data_ptr->result
                || MTVSERVICE_RST_NEED_UPDATE_SG == sig_data_ptr->result
                || MTVSERVICE_RST_NO_SPACE == sig_data_ptr->result)
            {
                //提示“内存不足，请尝试手动更新”
                if(MTVSERVICE_RST_NO_MEM == sig_data_ptr->result)
                {
                    MMIMBBMS_DisplayCommonTip(TXT_COMMON_NO_MEMORY, IMAGE_PUBWIN_WARNING);
                }
                else if(MTVSERVICE_RST_NEED_UPDATE_SG == sig_data_ptr->result)
                {
                    MMIMBBMS_DisplayCommonTip(TXT_MBBMS_SG_RETRY_UPDATE, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    MMIMBBMS_DisplayCommonTip(TXT_MBBMS_NO_SPACE, IMAGE_PUBWIN_WARNING);
                }
            }
            else
            {
                //NOTHING TO DO
            }
            break;
            
        case MBBMS_SERVICE_UPDATE_TABLE:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal subtype MBBMS_SERVICE_UPDATE_TABLE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3635_112_2_18_2_39_34_148,(uint8*)"");
            sub_update_res_ptr = (MBBMS_Subscription_Update_Res_t*)sig_data_ptr->data_ptr;
            
            if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result)
            {
                //提示成功
                MMIMBBMS_DisplayCommonTip(TXT_MBBMS_UPDATE_SUCCESS, IMAGE_PUBWIN_SUCCESS);
            }
            else
            {
                if(CheckDescriptionResValid(sub_update_res_ptr, MBBMS_SERVICE_UPDATE_TABLE))
                {
                    DisplayErrorTipByDescription(sub_update_res_ptr->response_ptr->description, sub_update_res_ptr->response_ptr->description_len);
                }
                else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
                {
                    MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    if(MTVSERVICE_RST_PDP_FAIL == sig_data_ptr->result)
                    {
                        //提示信号问题导致更新失败
                        MMIMBBMS_DisplayCommonTip(TXT_MBBMS_BAD_SIGNAL, IMAGE_PUBWIN_WARNING);
                    }
                    else
                    {
                        //NOTHING TO DO
                    }
                }
            }
            
            MBBMS_Free_Subupdate_Res_Ptr(sub_update_res_ptr);
            break;
            
        case CMMB_SEARCH_PROG:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal subtype CMMB_SEARCH_PROG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3671_112_2_18_2_39_34_149,(uint8*)"");
            //NOTHING TO DO
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal  subtype error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3676_112_2_18_2_39_34_150,(uint8*)"");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleManualUpdateSGSignal  cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3682_112_2_18_2_39_34_151,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : handle stop manual update sg signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopManualUpdateSGSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopManualUpdateSGSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3696_112_2_18_2_39_34_152,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopManualUpdateSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3701_112_2_18_2_39_34_153,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    MMIMBBMS_SetSGOperationFromType(MMIMBBMS_SG_SEARCH_PARSE_FROM_NONE);
    MMK_CloseWin(MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID);
    
    if (s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
    }
}

/*****************************************************************************/
//  Description : handle parse sg signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleParseSGSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleParseSGSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3723_112_2_18_2_39_34_154,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleParseSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3728_112_2_18_2_39_34_155,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    //关闭后台解析保护窗口
    MMK_CloseWin(MMIMBBMS_SG_PARSE_WAITING_WIN_ID);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleParseSGSignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3735_112_2_18_2_39_34_156,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        
        if(s_is_mbbms_exiting)
        {
            //该流程结束只需关注STOP_CNF消息
            //MMIMBBMS_Clear();
        }
        else
        {
            //NOTHING TO DO
        }
        
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status)
    {
        if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result
            || MTVSERVICE_RST_INCOMPLETE_DATA == sig_data_ptr->result)
        {  
            SGParseOK();
        }
        else    
        {
            SGParseFailed();
            
            //提示“内存不足，请尝试手动更新”
            if(MTVSERVICE_RST_NO_MEM == sig_data_ptr->result
                || MTVSERVICE_RST_NEED_UPDATE_SG == sig_data_ptr->result
                || MTVSERVICE_RST_NO_SPACE == sig_data_ptr->result)
            {
                if(MTVSERVICE_RST_NO_MEM == sig_data_ptr->result)
                {
                    MMIMBBMS_DisplayCommonTip(TXT_COMMON_NO_MEMORY, IMAGE_PUBWIN_WARNING);
                }
                else if(MTVSERVICE_RST_NEED_UPDATE_SG == sig_data_ptr->result)
                {
                    MMIMBBMS_DisplayCommonTip(TXT_MBBMS_SG_RETRY_UPDATE, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    MMIMBBMS_DisplayCommonTip(TXT_MBBMS_NO_SPACE, IMAGE_PUBWIN_WARNING);
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleParseSGSignal cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3784_112_2_18_2_39_34_157,(uint8*)"");
    }
    
    return;
}

/*****************************************************************************/
// 	Description : handle stop parse sg signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopParseSGSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopParseSGSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3800_112_2_18_2_39_34_158,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopParseSGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3805_112_2_18_2_39_34_159,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    
    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
        return;
    }
}

/*****************************************************************************/
// 	Description : handle parse cmmb esg signal 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleParseESGSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleParseESGSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3826_112_2_18_2_39_34_160,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleParseESGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3831_112_2_18_2_39_34_161,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    //关闭后台解析保护窗口
    MMK_CloseWin(MMIMBBMS_SG_PARSE_WAITING_WIN_ID);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleParseESGSignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3838_112_2_18_2_39_34_162,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        
        if(s_is_mbbms_exiting)
        {
            //该流程结束只需关注STOP_CNF消息
            //MMIMBBMS_Clear();
        }
        else
        {
            s_is_init_finished = TRUE;
        }
        
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status)
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_PARSE_ESG:
            //解析ESG流程完成
            //SCI_TRACE_LOW:"MMIMBBMS_HandleParseESGSignal:  subtype MBBMS_SERVICE_PARSE_ESG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3860_112_2_18_2_39_34_163,(uint8*)"");
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
            s_is_init_finished = TRUE;
            MMIMBBMS_RefreshMainInterface();
            break;
            
        default:
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleParseESGSignal cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3872_112_2_18_2_39_34_164,(uint8*)"");
    }
    
    return;
}

/*****************************************************************************/
// 	Description : handle stop parse cmmb esg signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopParseESGSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopParseESGSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3888_112_2_18_2_39_34_165,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopParseESGSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3893_112_2_18_2_39_34_166,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    
    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
        return;
    }
    else
    {
        s_is_init_finished = TRUE;
    }
}

/*****************************************************************************/
// 	Description : handle active mbbms signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleActiveMBBMSSignal(DPARAM param)
{
    MMI_TEXT_ID_T        text_id = TXT_NULL;
    mbbms_gba_data_t     *gba_res_ptr = PNULL;
    MBBMS_Subscription_Update_Res_t *sub_update_res_ptr = PNULL;
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;

    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3921_112_2_18_2_39_34_167,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3926_112_2_18_2_39_34_168,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
    	if(sig_data_ptr->type == MBBMS_SERVICE_ACTIVE && sig_data_ptr->subtype == MBBMS_SERVICE_ACTIVE)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal:  cur flow is stopping"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3932_112_2_18_2_39_34_169,(uint8*)"");
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        
            if (s_is_mbbms_exiting)
            {
                //该流程结束只需关注STOP_CNF消息
                //MMIMBBMS_Clear();
            }
            else
            {
            //NOTHING TO DO
            }
    	}else
    	{
            //NOTHING TO D0
		}
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status)
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_UPDATE_SG:
        case MBBMS_SERVICE_PARSE_SG:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_UPDATE_SG/MBBMS_SERVICE_PARSE_SG"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3957_112_2_18_2_39_34_170,(uint8*)"");
            if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result
                || MTVSERVICE_RST_SG_NOT_UPDATED == sig_data_ptr->result
                || MTVSERVICE_RST_NO_DATA == sig_data_ptr->result
                || MTVSERVICE_RST_INCOMPLETE_DATA == sig_data_ptr->result)
            {
                //NOTHING TO DO
            }
            else if(MTVSERVICE_RST_PDP_FAIL == sig_data_ptr->result
                    || MTVSERVICE_RST_SG_RCV_FAIL == sig_data_ptr->result)
            {
                //CMCC Dlg02 获取业务指南失败提示
                MMIMBBMS_DisplayCommonTip(TXT_MBBMS_BAD_SIGNAL, IMAGE_PUBWIN_WARNING);
            }
            else if(MTVSERVICE_RST_NO_MEM == sig_data_ptr->result
                    || MTVSERVICE_RST_NEED_UPDATE_SG == sig_data_ptr->result
                    || MTVSERVICE_RST_NO_SPACE == sig_data_ptr->result)
            {
                //提示“内存不足，请尝试手动更新”
                if(MTVSERVICE_RST_NO_MEM == sig_data_ptr->result)
                {
                    MMIMBBMS_DisplayCommonTip(TXT_COMMON_NO_MEMORY, IMAGE_PUBWIN_WARNING);
                }
                else if(MTVSERVICE_RST_NEED_UPDATE_SG == sig_data_ptr->result)
                {
                    MMIMBBMS_DisplayCommonTip(TXT_MBBMS_SG_RETRY_UPDATE, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    MMIMBBMS_DisplayCommonTip(TXT_MBBMS_NO_SPACE, IMAGE_PUBWIN_WARNING);
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal  subtype result error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3991_112_2_18_2_39_34_171,(uint8*)"");
            }
            break;
            
        case MBBMS_SERVICE_UPDATE_TABLE:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_UPDATE_TABLE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_3996_112_2_18_2_39_34_172,(uint8*)"");
            sub_update_res_ptr = (MBBMS_Subscription_Update_Res_t*)sig_data_ptr->data_ptr;
            
            if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result)
            {
                s_is_sub_update_ok = TRUE;
            }
            else
            {
                s_is_sub_update_ok = FALSE;
                
                if(CheckDescriptionResValid(sub_update_res_ptr, MBBMS_SERVICE_UPDATE_TABLE))
                {
                    DisplayErrorTipByDescription(sub_update_res_ptr->response_ptr->description, sub_update_res_ptr->response_ptr->description_len);
                }
                else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
                {
                    MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    if(MTVSERVICE_RST_PDP_FAIL == sig_data_ptr->result
                        || MTVSERVICE_RST_SUB_HTTP_FAIL == sig_data_ptr->result)
                    {
                        //CMCC Dlg03 获取订购关系表失败提示
                        MMIMBBMS_DisplayCommonTip(TXT_MBBMS_BAD_SIGNAL, IMAGE_PUBWIN_WARNING);
                    }
                    else
                    {
                        //NOTHING TO DO
                    }
                }
            }
            
            MBBMS_Free_Subupdate_Res_Ptr(sub_update_res_ptr);
            break;
            
        case MBBMS_SERVICE_GBA:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_GBA"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4034_112_2_18_2_39_35_173,(uint8*)"");
            gba_res_ptr = (mbbms_gba_data_t*)sig_data_ptr->data_ptr;
            
            if(MTVSERVICE_RST_RET_OK != sig_data_ptr->result)
            {
                if(CheckDescriptionResValid(gba_res_ptr, MBBMS_SERVICE_GBA))
                {
                    DisplayErrorTipByDescription((uint8*)gba_res_ptr->description, 255);
                }
                else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
                {
                    MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    //NOTHING TO DO
                }
            }
            
            MBBMS_GBA_Free_Data(gba_res_ptr);
            break;
            
        case MBBMS_SERVICE_ACTIVE:
            //主流程结束
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal subtype MBBMS_SERVICE_ACTIVE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4058_112_2_18_2_39_35_174,(uint8*)"");
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
            s_is_init_finished = TRUE;
            
            //refresh main win
            MMIMBBMS_FreeProgramList();
            MMIMBBMS_FreeServiceIndex();
            MMIMBBMS_RefreshMainInterface();
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal  subtype error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4069_112_2_18_2_39_35_175,(uint8*)"");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal  cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4075_112_2_18_2_39_35_176,(uint8*)"");
    }
}

/*****************************************************************************/
// 	Description : handle stop active mbbms signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopActiveMBBMSSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopActiveMBBMSSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4089_112_2_18_2_39_35_177,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopActiveMBBMSSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4094_112_2_18_2_39_35_178,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    
    if (s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
        return;
    }
}

/*****************************************************************************/
// Description : mbbms active progress failed
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void MBBMSActiveFailed(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS: MBBMSActiveFailed"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4111_112_2_18_2_39_35_179,(uint8*)"");
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    
    s_is_init_finished = TRUE;
    MMIMBBMS_RefreshMainInterface();
}

/*****************************************************************************/
// Description : handle update subscription table signal
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleUpdateSubTableSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    MBBMS_Subscription_Update_Res_t *sub_update_res_ptr = PNULL;
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateSubTableSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4131_112_2_18_2_39_35_180,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateSubTableSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4136_112_2_18_2_39_35_181,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateSubTableSignal:  cur flow is stopping"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4140_112_2_18_2_39_35_182,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_NONE);
        
        sub_update_res_ptr = (MBBMS_Subscription_Update_Res_t*)sig_data_ptr->data_ptr;
        MBBMS_Free_Subupdate_Res_Ptr(sub_update_res_ptr);
        
        MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
        MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
        
        if(s_is_mbbms_exiting)
        {
            MMIMBBMS_Clear();
        }
        else
        {
            //NOTHING TO DO
        }
        
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status)
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_UPDATE_TABLE:
            //主流程结束
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateSubTableSignal subtype MBBMS_SERVICE_UPDATE_TABLE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4168_112_2_18_2_39_35_183,(uint8*)"");
            sub_update_res_ptr = (MBBMS_Subscription_Update_Res_t*)sig_data_ptr->data_ptr;
            
            if(MTVSERVICE_RST_RET_OK == sig_data_ptr->result)
            {
                SubTableUpateOK();
            }
            else
            {
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
                s_is_sub_update_ok = FALSE;
                
                MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
                if(CheckDescriptionResValid(sub_update_res_ptr, MBBMS_SERVICE_UPDATE_TABLE))
                {
                    DisplayErrorTipByDescription(sub_update_res_ptr->response_ptr->description, sub_update_res_ptr->response_ptr->description_len);
                }
                else if(GetTextIdByReturnCode(sig_data_ptr->result, &text_id))
                {
                    MMIMBBMS_DisplayCommonTip(text_id, IMAGE_PUBWIN_WARNING);
                }
                else
                {
                    SubTableUpateFailed();
                }
                
                MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_NONE);
            }
            
            MBBMS_Free_Subupdate_Res_Ptr(sub_update_res_ptr);
            break;
            
        case MBBMS_SERVICE_PDP_ACTIVE:
            //PDP子流程，提示待主流程完毕后
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateSubTableSignal subtype MBBMS_SERVICE_PDP_ACTIVE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4202_112_2_18_2_39_35_184,(uint8*)"");
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleActiveMBBMSSignal  subtype error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4206_112_2_18_2_39_35_185,(uint8*)"");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateSubTableSignal  cur flow status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4212_112_2_18_2_39_35_186,(uint8*)"");
    }
}

/*****************************************************************************/
// Description : handle stop update subscription table signal
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopUpdateSubTableSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    //MBBMS_Subscription_Update_Res_t *sub_update_res_ptr = PNULL;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopUpdateSubTableSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4227_112_2_18_2_39_35_187,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopUpdateSubTableSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4232_112_2_18_2_39_35_188,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    s_is_sub_update_ok = FALSE;
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_NONE);
    
    MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
    MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
    
    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
        return;
    }
}

/*****************************************************************************/
// 	Description : handle start play signal
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStartPlaySignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4258_112_2_18_2_39_35_189,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4263_112_2_18_2_39_35_190,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_STOPPING == s_curr_flow_info.status)
    {
        if (s_is_mbbms_exiting)
        {
            if(MBBMS_SERVICE_PLAY == sig_data_ptr->type && MBBMS_SERVICE_PLAY == sig_data_ptr->subtype)
            {
                //该流程结束只需关注STOP_CNF消息
                //MMIMBBMS_Clear();
            }
        }
        else
        {
            //NOTHING TODO
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal is not exiting while MMIMBBMS_FLOW_STATE_STOPPING"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4278_112_2_18_2_39_35_191,(uint8*)"");
        }
        
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_STARTING == s_curr_flow_info.status
        || MMIMBBMS_FLOW_STATE_IDLE == s_curr_flow_info.status)
    {
        switch(sig_data_ptr->subtype)
        {
        case MBBMS_SERVICE_PDP_ACTIVE:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PDP_ACTIVE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4290_112_2_18_2_39_35_192,(uint8*)"");
            break;
            
        case MBBMS_SERVICE_GBA:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_GBA"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4294_112_2_18_2_39_35_193,(uint8*)"");
            break;
            
        case MBBMS_SERVICE_GETMSK:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_GETMSK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4298_112_2_18_2_39_35_194,(uint8*)"");
            break;
            
        case MBBMS_SERVICE_PLAY:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4302_112_2_18_2_39_35_195,(uint8*)"");
            //主流程结束
            //MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
            HandlePlayAck(sig_data_ptr->result);
            break;
            
            
        //以下是播放过程中可能的IND消息，如信号强度、紧急广播等
        case MBBMS_SERVICE_RSSI_CHANGED_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_RSSI_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4311_112_2_18_2_39_35_196,(uint8*)"");
            HandleRssiChangedMsg(sig_data_ptr->param);
            break;
            
        case MBBMS_SERVICE_PLAY_ERROR_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_ERROR_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4316_112_2_18_2_39_35_197,(uint8*)"");
            HandlePlayErrorIndMsg();
            break;
            
        case MBBMS_SERVICE_PLAY_AV_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_AV_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4321_112_2_18_2_39_35_198,(uint8*)"");
            HandleAVIndMsg(sig_data_ptr->param);
            break;
            
        case MBBMS_SERVICE_EMERGENCY_BROADCAST_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_EMERGENCY_BROADCAST_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4326_112_2_18_2_39_35_199,(uint8*)"");
            HandleEmergencyBroadcastMsg((MTVSERVICE_CMMB_BROADCAST_IND_T*)sig_data_ptr->data_ptr);
            break;
            
        case MBBMS_SERVICE_CA_NOT_GRANTED_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_CA_NOT_GRANTED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4331_112_2_18_2_39_35_200,(uint8*)"");
            HandleCANotGrantedMsg();
            break;
            
        case MBBMS_SERVICE_PLAY_DRA_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_PLAY_DRA_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4336_112_2_18_2_39_35_201,(uint8*)"");
            HandleDRAIndMsg();
            break;
            
        case MBBMS_SERVICE_BAD_SIGNAL_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_BAD_SIGNAL_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4341_112_2_18_2_39_35_202,(uint8*)"");
            HandleBadSignalMsg();
            break;
            
        case MBBMS_SERVICE_SIGNAL_RECOVERED_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_SIGNAL_RECOVERED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4346_112_2_18_2_39_35_203,(uint8*)"");
            HandleSignalRecoveredMsg();
            break;
            
        case MBBMS_SERVICE_SIGNAL_RELOCKING_IND:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype MBBMS_SERVICE_SIGNAL_RELOCKING_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4351_112_2_18_2_39_35_204,(uint8*)"");
            HandleRelockingMsg();
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal  subtype ignored!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4356_112_2_18_2_39_35_205,(uint8*)"");
            break;
        }
    }
}

/*****************************************************************************/
// 	Description : set condition while playing audio service
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void SetAudioServiceCondition(void)
{
    if(MMIMBBMS_IsCurrentServiceAudioTyle())
    {
        //to do 开音频广播背景图
        
        //音频广播允许关闭背光
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        
        //音频节目降频节电
#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
    }
}

/*****************************************************************************/
// 	Description : restore condition
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void RestoreAudioServiceCondition(void)
{
    if(MMIMBBMS_IsCurrentServiceAudioTyle())
    {
        //关音频广播背景图
        //to do
        
        //音频节目停止后，恢复背光、升频
        MMIDEFAULT_TurnOnBackLight(); 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(),(CHNG_FREQ_INDEX_E)FREQ_INDEX_APP_HIGH);
#endif
    }
}

/*****************************************************************************/
// 	Description : handle play ok
//	Global resource dependence : none
//  Author: xin.li
//	Note: 异步确认消息
/*****************************************************************************/
LOCAL void HandlePlayAck(MBBMS_SERVICE_RESULT_E result)
{
    //SCI_TRACE_LOW:"MMIMBBMS HandlePlayAck result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4409_112_2_18_2_39_35_206,(uint8*)"d", result);
    
    //close play connecting scene
    if(OSD_PLAY_CONNECTING == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        MMIMTV_StartStbDisableTimer();
    }
    
    if(MTVSERVICE_RST_RET_OK == result)
    {
        //play ok
        SetCmmbPlayStatus(MMIMTV_PLAY_PLAYING);
        
        if(MMIMBBMS_IsCurrentServiceAudioTyle())
        {
            //create audio service bg layer
            MMIMBBMS_CreateBgOSDLayer();
            
            //set conditon
            SetAudioServiceCondition();
        }
        
        //CMMB模式下清流播放成功时保存频道信息，包括network_id/service_id
        {
            uint32  net_id = MMIMBBMS_GetPlayInfo()->frequency_no;
            uint32  service_id = MMIMBBMS_GetPlayInfo()->service_id;
            
            if(MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode())
            {
                MMIMBBMS_SaveFreeServiceInfoToFile(net_id, service_id);
            }
        }
    }
    else
    {
        //play failed
        SetCmmbPlayStatus(MMIMTV_PLAY_STOPPED);
        
        //play error tip
        MMIMTV_SetLastOSDScene();
        MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
        
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
    }
}

/*****************************************************************************/
// 	Description : handle play_error_ind msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandlePlayErrorIndMsg(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS HandlePlayErrorIndMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4466_112_2_18_2_39_36_207,(uint8*)"");
    
    //播放过程中超时错误，原因无信号或无节目
    SetCmmbPlayStatus(MMIMTV_PLAY_STOPPED);
    
    //play error tip
    MMIMTV_SetLastOSDScene();
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_SIGNAL_ERROR);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_SIGNAL_ERROR);
    
    if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
    {
        MMIMTV_SetPreOSDScene(OSD_IDLE);
    }
}

/*****************************************************************************/
// 	Description : handle rssi change msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleRssiChangedMsg(uint32 rssi_value)
{
    uint8   signal_level = 0;
    
    //SCI_TRACE_LOW:"MMIMBBMS HandleRssiChangedMsg rssi_value = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4493_112_2_18_2_39_36_208,(uint8*)"d", rssi_value);
    
    //calculate signal level 
    signal_level = MMIMTV_CalcTVSignalLevel(rssi_value);
    //save level value
    MMIMTV_SaveTVSignalLevel(signal_level);
    
    //update win status bar
    MAIN_SetTVSignalState();
}

/*****************************************************************************/
// 	Description : handle play_av_ind msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleAVIndMsg(uint32 param)
{
    uint32 av_ind = 0;
    uint32 video_ind = 0;
    uint32 audio_ind = 0;
    
    av_ind = param;
    video_ind = av_ind >> 16;
    audio_ind = av_ind & 0x0ffff;
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStartPlaySignal subtype av_ind  video_ind=%d, audio_ind=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4518_112_2_18_2_39_36_209,(uint8*)"dd", video_ind, audio_ind);
}

/*****************************************************************************/
// 	Description : 设置当前紧急广播等级信息
//	Global resource dependence :
//  Author: xiaoxiang.huang
//	Note: 
/*****************************************************************************/
LOCAL void MMIMTV_SetEbStopPlayFlag(BOOLEAN stopplayflag)
{
	s_stopplay_flag = stopplayflag;
}

/*****************************************************************************/
// 	Description : 获取当前紧急广播等级信息
//	Global resource dependence :
//  Author: xiaoxiang.huang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_GetEbStopPlayFlag(void)
{
	return s_stopplay_flag;
}
/*****************************************************************************/
// 	Description : handle emergency broadcast msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 紧急广播消息处理
/*****************************************************************************/
LOCAL void HandleEmergencyBroadcastMsg(MTVSERVICE_CMMB_BROADCAST_IND_T *eb_sig_ptr)
{
    uint32  eb_grade = 0;
    MTV_STIMI_BROADCAST_U    bc_data = {0};
	BOOLEAN is_stopplay_success = TRUE;
    
    //SCI_TRACE_LOW:"MMIMBBMS HandleEmergencyBroadcastMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4548_112_2_18_2_39_36_210,(uint8*)"");
    
    if (CMMB_GetEmergBroadcast(eb_sig_ptr->data_type, eb_sig_ptr->data_len, &bc_data))
    {
        eb_sig_ptr->data_ptr = (uint8*)&(bc_data.msg_content);
        //已达到上限
        if(MMIMTV_EB_MAX_NUMBER == MMIMTV_GetEBListNumber())
        {
            //MMIMTV_EBSortByTime();
            if(MMIMBBMS_ParseEmergencyBroadcastData(eb_sig_ptr, &eb_grade))
            { 
				
                //记录来源场景用于返回
                if(OSD_PUB != MMIMTV_GetFocusOSDScene())
                {
                    //s_source_osd = MMIMTV_GetFocusOSDScene();
                    MMIMTV_SetSourceOSDScene(MMIMTV_GetFocusOSDScene());
                }
                else
                {
                    //s_source_osd = OSD_SHORTCUT_ICON;
                    MMIMTV_SetSourceOSDScene(OSD_SHORTCUT_ICON);
                }
                
                //删除最旧的一条紧急广播信息
                MMIMTV_DeleteEBNodeViaIndex(MMIMTV_EB_MAX_NUMBER - 1);

                //设置PUB_WIN返回的场景
                if(eb_grade < 3)
                {
                    MMIMTV_SetPreOSDScene(OSD_EB_DETAIL); //提示空间不足后,返回 OSD_EB_DETAIL 场景
                }
                else
                {
                    MMIMTV_SetPreOSDScene(OSD_READ_EB_QUERY);   //保存当前被打断的场景 
                    MAIN_SetMsgState(TRUE);
                }
                
                //提示已达到最大条数，如当前场景包含编辑框则不提示
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_EB_FULL);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_EB_FULL);
                
                if(eb_grade < 3)
                {
                    //MBBMS 1,2级紧急广播要停止当前播放, 并在返回后重新发起播放
                    is_stopplay_success = MMIMBBMS_StopPlay();
                    if(is_stopplay_success)
                        MMIMTV_SetEbStopPlayFlag(TRUE); 
                }

                //保存紧急广播信息
                MMIMTV_WriteEBFile();
            }
        }
        else
        {
            if(MMIMBBMS_ParseEmergencyBroadcastData(eb_sig_ptr, &eb_grade))
            {
				
                //记录来源场景用于返回
                if(OSD_PUB != MMIMTV_GetFocusOSDScene())
                {
                    //s_source_osd = MMIMTV_GetFocusOSDScene();
                    MMIMTV_SetSourceOSDScene(MMIMTV_GetFocusOSDScene());
                }
                else
                {
                    //s_source_osd = OSD_SHORTCUT_ICON;
                    MMIMTV_SetSourceOSDScene(OSD_SHORTCUT_ICON);
                }
                
                if(eb_grade < 3)
                { 
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                    //MMIMTV_SetLastOSDScene();
                    
                    MMIMTV_SetFocusOSDScene(OSD_EB_DETAIL);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                }
                else
                {
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);                       
                    MMIMTV_SetFocusOSDScene(OSD_READ_EB_QUERY);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE,PNULL);
                    
                    MAIN_SetMsgState(TRUE);
                }
                
                if(eb_grade < 3)
                {
                    //MBBMS 1,2级紧急广播要停止当前播放, 并在返回后重新发起播放
                    is_stopplay_success = MMIMBBMS_StopPlay();
                    if(is_stopplay_success)
                        MMIMTV_SetEbStopPlayFlag(TRUE);  
                }

                //保存紧急广播信息
                MMIMTV_WriteEBFile();
            }
        }
    }
    CMMB_FreeBroadcastMsg(&bc_data);
}

/*****************************************************************************/
// 	Description : handle ca_not_granted msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleCANotGrantedMsg(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS HandleCANotGrantedMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4660_112_2_18_2_39_36_211,(uint8*)"");
    
    //CA卡异常、未授权等
    SetCmmbPlayStatus(MMIMTV_PLAY_STOPPED);
    
    //ca error tip
    MMIMTV_SetLastOSDScene();
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_CA_UNAUTHORIZED);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_CA_UNAUTHORIZED);
    
    if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
    {
        MMIMTV_SetPreOSDScene(OSD_IDLE);
    }
}

/*****************************************************************************/
// 	Description : handle dra_ind msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleDRAIndMsg(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS HandleDRAIndMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4685_112_2_18_2_39_36_212,(uint8*)"");
    
    //音频广播标志
    //s_mtv_current_para.is_playing_broadcast = TRUE;
    
}


/*****************************************************************************/
// 	Description : handle bad_signal msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleBadSignalMsg(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS HandleBadSignalMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4699_112_2_18_2_39_36_213,(uint8*)"");
    
    //TV信号弱
    SetCmmbPlayStatus(MMIMTV_PLAY_WAITING);
    
    //bad signal tip
    MMIMTV_SetLastOSDScene();
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_SIGNAL_WEAK);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_SIGNAL_WEAK);
    
    if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
    {
        MMIMTV_SetPreOSDScene(OSD_IDLE);
    }
}

/*****************************************************************************/
// 	Description : handle signal_recovered msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleSignalRecoveredMsg(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS HandleSignalRecoveredMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4724_112_2_18_2_39_36_214,(uint8*)"");
    
    //TV信号已恢复
    SetCmmbPlayStatus(MMIMTV_PLAY_PLAYING);
    
    //signal recovered tip
    MMIMTV_SetLastOSDScene();
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_SIGNAL_RECOVER);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_SIGNAL_RECOVER);
    
    if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
    {
        MMIMTV_SetPreOSDScene(OSD_IDLE);
    }
}

/*****************************************************************************/
// 	Description : handle relocking msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleRelockingMsg(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS HandleRelockingMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4749_112_2_18_2_39_36_215,(uint8*)"");
    
    //重新发起播放
    
    //to do
}

/*****************************************************************************/
// 	Description : handle stop play signal
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopPlaySignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStopPlaySignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4766_112_2_18_2_39_36_216,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStopPlaySignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4771_112_2_18_2_39_36_217,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);
    
    //主流程完成
    if(MBBMS_SERVICE_STOP == sig_data_ptr->subtype)
    {
        //update flow status
        SetCmmbPlayStatus(MMIMTV_PLAY_STOPPED);
        if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type)
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        }
        
        if (s_is_mbbms_exiting)
        {
            MMIMBBMS_Clear();
            return;
        }
        
        //check need play flag
        if(MMIMBBMS_GetPlayInfo()->is_need_play_next)
        {
            SetNeedPlayNextFlag(FALSE);
            MMIMBBMS_StartPlay();
        }
        else
        {
            if(OSD_PLAY_CONNECTING == MMIMTV_GetFocusOSDScene())
            {
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_IDLE);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            }
        }
    }
}

/*****************************************************************************/
//     Description : MMIMBBMS_HandleSGGetComplex
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleSGGetComplex(DPARAM param)
{
    SG_LIST_PTR_T* pList = PNULL;
    MBBMS_SGAPI_INFO_T *search_result = (MBBMS_SGAPI_INFO_T*)param;
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleSGGetComplex: err_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4816_112_2_18_2_39_36_218,(uint8*)"d", search_result->err_code);
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);

    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
        return;
    }
    
    pList = search_result->data_ptr;
    /* 如果搜索等待窗口被覆盖,则不能马上打开搜索结果窗口 */
    if (!MMK_IsFocusWin(MMIMBBMS_SEARCH_WAIT_WIN_ID))
    {
        MMK_PostMsg(MMIMBBMS_SEARCH_WAIT_WIN_ID, MSG_APP_MBBMS_WAIT_GET_FOCUS, 
            NULL, NULL);
        
        return;
    }
    
    if((SG_ERROR_NONE == search_result->err_code) && (PNULL!= pList )) //success  
    {  
        MMIMBBMS_OpenSearchResultMainWin((SG_LIST_PTR_T)(*pList));
    }
    else
    {
        MMIMBBMS_OpenSearchResultMainWin((SG_LIST_PTR_T)PNULL);
    }
    
    return;
}

/*****************************************************************************/
//     Description : MMIMBBMS_HandleSGStopComplex
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleSGStopComplex(DPARAM param)
{
    MBBMS_SGAPI_INFO_T *search_result = (MBBMS_SGAPI_INFO_T*)param;
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleSGStopComplex: err_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4855_112_2_18_2_39_37_219,(uint8*)"d", search_result->err_code);
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    MMIMBBMS_FreeComplexInfo();
    MMK_CloseWin(MMIMBBMS_SEARCH_WAIT_WIN_ID);
    
    if(s_is_mbbms_exiting)
    {
        MMIMBBMS_Clear();
    }
    
    return;
}

/*****************************************************************************/
//  Description : handle MSK update signal
//  Global resource dependence : none
//  Author: xiaoxiang.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleMSKupdateSignal(DPARAM param)
{
    MBBMS_SERVICE_INFO_T *sig_data_ptr = (MBBMS_SERVICE_INFO_T*)param;
    
    if(PNULL == sig_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleMSKupdateSignal: sig_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4878_112_2_18_2_39_37_220,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandleMSKupdateSignal: type=%d, subtype=%d, result=%d, cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4883_112_2_18_2_39_37_221,(uint8*)"ddddd", sig_data_ptr->type, sig_data_ptr->subtype, sig_data_ptr->result, s_curr_flow_info.status, s_curr_flow_info.flow_type);

	if(MBBMS_SERVICE_PLAY == sig_data_ptr->subtype)
		SetMSKUpdateStatus((uint32)MMIMBBMS_FLOW_STATE_STARTING);
	else if(MBBMS_SERVICE_CA_NOT_GRANTED_IND == sig_data_ptr->subtype)
		SetMSKUpdateStatus((uint32)MMIMBBMS_FLOW_STATE_STOPPING);		

}
/*****************************************************************************/
//     Description : whether the service to be played is encryted
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsServiceEncrypted(anyURI serviceId)
{
    
    BOOLEAN is_encrypted=FALSE;
    SG_ERROR_E ret = SG_ERROR_NONE;
    
    ret =MBBMSSG_IsServiceEncrypted(serviceId,&is_encrypted);
    
    if(SG_ERROR_NONE ==ret)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_IsServiceEncrypted Service encryption status =%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4905_112_2_18_2_39_37_222,(uint8*)"d",is_encrypted);
        return is_encrypted;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_IsServiceEncrypted Service encryption status = FALSE\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4910_112_2_18_2_39_37_223,(uint8*)"");
        return FALSE;
    }
    
}

/*****************************************************************************/
//     Description : whether the Content to be played is encryted
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsContentEncrypted(anyURI contentId)
{
    MBBMSSG_CONTENT_T content={0};
    BOOLEAN ret = FALSE;    
    
    //search service according to content id
    if(SG_ERROR_NONE ==MBBMSSG_GetContent(contentId, &content))
    {        
        ret=MMIMBBMS_IsServiceEncrypted(content.serviceIdRef);        
    }
    else
    {
        ret = FALSE;
    }
    
    MBBMSSG_FreeContent(&content);
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsContentEncrypted content encryted status  =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4937_112_2_18_2_39_37_224,(uint8*)"d",ret);
    
    return ret;
}



/*****************************************************************************/
//     Description : whether purchase item is ordered
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsPurchaseItemOrdered(anyURI purchaseItem)
{
    BOOLEAN is_ordered=FALSE;
    SG_ERROR_E    error_code = SG_ERROR_NONE;
    
    error_code=MBBMSSG_IsPurchaseItemSubscribed(purchaseItem,&is_ordered);
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsPurchaseItemOrdered error_code=%d,is_ordered=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4955_112_2_18_2_39_37_225,(uint8*)"dd",error_code,is_ordered);
    
    if(SG_ERROR_NONE==error_code)
    {
        return is_ordered;
        
    }
    else
    {
        return FALSE;
    }
    
}

/*****************************************************************************/
//     Description : whether the service is  ordered
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsServiceOrdered(anyURI serviceId)
{
    SG_LIST_PTR_T purchase_itemlist_head_ptr=PNULL;    
    SG_LIST_PTR_T purchase_itemlist_node_ptr=PNULL; 
    MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr=PNULL;
    BOOLEAN ret=FALSE;
    SG_ERROR_E err_code = SG_ERROR_NONE;
    
    //get all purchase items which include the Service
    err_code = MBBMSSG_GetPurchaseItemByService(serviceId, &purchase_itemlist_head_ptr);
    purchase_itemlist_node_ptr = purchase_itemlist_head_ptr;
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsServiceOrdered get purchase result=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4985_112_2_18_2_39_37_226,(uint8*)"d",err_code);
    if(SG_ERROR_NONE== err_code)
    {                
        if(0==STL_LinkListGetNodeDataCount(purchase_itemlist_head_ptr))
        {
            //SCI_TRACE_LOW:"MMIMBBMS_IsServiceOrdered:not found purchase which contain this service\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4990_112_2_18_2_39_37_227,(uint8*)"");
            ret =FALSE;
            
        }
        else
        {
            purchase_itemlist_node_ptr = purchase_itemlist_node_ptr->next_ptr;
            while(purchase_itemlist_node_ptr)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_IsServiceOrdered purchase_itemlist_head_ptr=0x%x\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_4999_112_2_18_2_39_37_228,(uint8*)"d",purchase_itemlist_node_ptr);
                purchase_index_ptr=(MBBMSSG_PURCHASEITEM_INDEX_T *)(purchase_itemlist_node_ptr->data);
                if(PNULL == purchase_index_ptr)
                {
                    //SCI_TRACE_LOW:"MMIMBBMS_IsServiceOrdered purchase_index_ptr is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5003_112_2_18_2_39_37_229,(uint8*)"");
                    break;
                }
                
                if(MMIMBBMS_IsPurchaseItemOrdered(purchase_index_ptr->index))
                {
                    ret= TRUE;
                    //SCI_TRACE_LOW:"MMIMBBMS_IsServiceOrdered :Purchase ordered\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5010_112_2_18_2_39_37_230,(uint8*)"");
                    break;
                }    
                purchase_itemlist_node_ptr =purchase_itemlist_node_ptr->next_ptr;                
            }
            
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_IsServiceOrdered:Get Purchase Item by sevice failded\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5020_112_2_18_2_39_37_231,(uint8*)"");
        ret =FALSE;
    }
    
    MBBMSSG_FreePurchaseItemByService(&purchase_itemlist_head_ptr);
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsServiceOrdered:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5026_112_2_18_2_39_37_232,(uint8*)"d",ret);
    
    return ret;
    
}

/*****************************************************************************/
//     Description : whether content is ordered or not
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsContentOrdered(anyURI contentId)
{
    
    MBBMSSG_CONTENT_T content={0};
    BOOLEAN ret = FALSE;    
    
    //search service according to content id
    if(SG_ERROR_NONE ==MBBMSSG_GetContent(contentId, &content))
    {
        
        ret=MMIMBBMS_IsServiceOrdered(content.serviceIdRef);
        
    }
    else
    {
        ret = FALSE;
        
    }
    
    MBBMSSG_FreeContent(&content);
    
    return ret;
    
    
}

/*****************************************************************************/
//     Description : check whether service or content is ordered befor play
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_CheckServiceOrContentOrderStatusBeforePlay(void)
{    
    BOOLEAN ret=FALSE;
    //check current playing service or content order status
    switch(MMIMBBMS_GetPlayFromType())
    {
    case MMIMBBMS_PLAY_FROM_SERVICE:
        //get current service id
        ret= MMIMBBMS_IsServiceOrdered(s_mbbms_play_info.uri);
        
        break;
    case MMIMBBMS_PLAY_FROM_CONTENT:
        //get current content id
        ret= MMIMBBMS_IsContentOrdered(s_mbbms_play_info.uri);
        break;
        
    default:            
        break;
    }
    
    return ret;
}

/*****************************************************************************/
//     Description : mbbms play service according to service id
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_PlayService(anyURI serviceId)
{
    //play from service
    if(MMIMBBMS_IsServiceEncrypted(serviceId))
    {
        BOOLEAN is_free=FALSE;
        
        //check whether support mtv and open status
        if(!MMIMBBMS_SDandOpenCondition())
        {
            MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
            MMIMBBMS_FreeCurrPlayUri();
            return;
        }
        //check whether service is free or not
        MBBMSSG_GetFreeStatusByServiceId(serviceId,&is_free);
        if(is_free)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_PlayService is  encrypted but free,then play direct\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5111_112_2_18_2_39_37_233,(uint8*)"");
            MMIMBBMS_EnterPlayprocess();
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMBBMS_PlayService is  encrypted but not free\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5116_112_2_18_2_39_37_234,(uint8*)"");
            
            //check whether service subscribled
            if(MMIMBBMS_CheckServiceOrContentOrderStatusBeforePlay())
            {
                
                //SCI_TRACE_LOW:"MMIMBBMS_PlayService:service is orderd and then check gba \n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5122_112_2_18_2_39_37_235,(uint8*)"");
                MMIMBBMS_EnterPlayprocess();
            }    
            else
            {
                //SCI_TRACE_LOW:"MMIMBBMS_PlayService:service is not orderd"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5127_112_2_18_2_39_37_236,(uint8*)"");
                MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
                
                //show subscription alert win 
                if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
                {
                    //SCI_TRACE_LOW:"MMIMBBMS_OpenSubscriblePromptWin MMIMTV_MAIN_WIN_ID is focus"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5133_112_2_18_2_39_37_237,(uint8*)"");
                    //从播放界面切换频道的，需要在播放区域显示未订购提示
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                    MMIMTV_SetFocusOSDScene(OSD_SUBSCRIBLE_QUERY);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                }
                else
                {
                    MMIMBBMS_OpenSubscriblePromptWin();
                }
            }    
        }
        
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_PlayService: service is not encrypted and play directly\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5149_112_2_18_2_39_37_238,(uint8*)"");
        MMIMBBMS_EnterPlayprocess();                
    }
}

/*****************************************************************************/
//     Description : 节目时间未到 或 已过期 提示窗口处理函数
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleContentAlertWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    LOCAL BOOLEAN is_need_play = TRUE;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
        is_need_play = TRUE;
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        is_need_play = FALSE;
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        //SCI_TRACE_LOW:"MMIMBBMS HandleContentAlertWinMsg close win"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5181_112_2_18_2_39_37_239,(uint8*)"");
        if(is_need_play)
        {
            MMIMBBMS_EnterPlayprocess();
        }
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}

/*****************************************************************************/
//     Description : mbbms play content according to service id
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_PlayContent(anyURI contentId)
{
    MMIMBBMS_CONTENT_TIME_TYPE_E check_ret = MMIMBBMS_CONTENT_TIME_NOT_START;
    MBBMSSG_CONTENT_T content={0};
    MMI_TEXT_ID_T  text_id = TXT_NULL;
    MMI_WIN_ID_T   alert_win_id = MMIMBBMS_CONTENT_TIME_ALERT_WIN_ID;
    
    //search service according to content id
    if(SG_ERROR_NONE!=MBBMSSG_GetContent(contentId, &content))
    {
        //SCI_TRACE_LOW:"MMIMBBMS_PlayContent get content  error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5209_112_2_18_2_39_37_240,(uint8*)"");
        MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
        MMIMBBMS_FreeCurrPlayUri();
        return;
    }
    
    //check content play time
    check_ret = MMIMBBMS_CheckContentPlayTime(&contentId);

    //play from service
    if(MMIMBBMS_IsServiceEncrypted(content.serviceIdRef))
    {
        BOOLEAN is_free=FALSE;
        
        //check whether support mtv and open status
        if(!MMIMBBMS_SDandOpenCondition())
        {
            MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
            MMIMBBMS_FreeCurrPlayUri();
            MBBMSSG_FreeContent(&content);
            return;
        }
        //check whether service is free or not
        MBBMSSG_GetFreeStatusByServiceId(content.serviceIdRef,&is_free);
        
        if(is_free)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_PlayContent is  encrypted but free,then play direct\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5236_112_2_18_2_39_37_241,(uint8*)"");
            if(MMIMBBMS_CONTENT_TIME_OK == check_ret)
            {
                MMIMBBMS_EnterPlayprocess();
            }
            else
            {
                if(MMIMBBMS_CONTENT_TIME_NOT_START == check_ret)
                {
                    text_id = TXT_MBBMS_CONTENT_PLAY_NOT_START;
                }
                else
                {
                    text_id = TXT_MBBMS_CONTENT_PLAY_EXPIRED;
                }
                
                //提示后播放
                MMIPUB_OpenAlertWinByTextId(PNULL,
                    text_id,
                    TXT_NULL,
                    IMAGE_PUBWIN_WARNING,
                    &alert_win_id,
                    PNULL,
                    MMIPUB_SOFTKEY_ONE,
                    HandleContentAlertWinMsg);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMBBMS_PlayService is  encrypted but not free\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5265_112_2_18_2_39_37_242,(uint8*)"");
            
            //check whether service subscribled
            if(MMIMBBMS_CheckServiceOrContentOrderStatusBeforePlay())
            {
                
                //SCI_TRACE_LOW:"MMIMBBMS_PlayContent:service is orderd and then check gba \n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5271_112_2_18_2_39_37_243,(uint8*)"");
                if(MMIMBBMS_CONTENT_TIME_OK == check_ret)
                {
                    MMIMBBMS_EnterPlayprocess();
                }
                else
                {
                    if(MMIMBBMS_CONTENT_TIME_NOT_START == check_ret)
                    {
                        text_id = TXT_MBBMS_CONTENT_PLAY_NOT_START;
                    }
                    else
                    {
                        text_id = TXT_MBBMS_CONTENT_PLAY_EXPIRED;
                    }
                    
                    //提示后播放
                    MMIPUB_OpenAlertWinByTextId(PNULL,
                        text_id,
                        TXT_NULL,
                        IMAGE_PUBWIN_WARNING,
                        &alert_win_id,
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        HandleContentAlertWinMsg);
                }
            }    
            else
            {
                //SCI_TRACE_LOW:"MMIMBBMS_PlayContent:service is not orderd"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5300_112_2_18_2_39_37_244,(uint8*)"");
                MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
                
                //show subscription alert win 
                if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
                {
                    //SCI_TRACE_LOW:"MMIMBBMS_OpenSubscriblePromptWin MMIMTV_MAIN_WIN_ID is focus"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5306_112_2_18_2_39_37_245,(uint8*)"");
                    //从播放界面切换频道的，需要在播放区域显示未订购提示
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                    MMIMTV_SetFocusOSDScene(OSD_SUBSCRIBLE_QUERY);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                }
                else
                {
                    MMIMBBMS_OpenSubscriblePromptWin();
                }
            }    
        }
        
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_PlayContent: service is not encrypted and play directly\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5322_112_2_18_2_39_37_246,(uint8*)"");
        
        if(MMIMBBMS_CONTENT_TIME_OK == check_ret)
        {
            MMIMBBMS_EnterPlayprocess();        
        }
        else
        {
            if(MMIMBBMS_CONTENT_TIME_NOT_START == check_ret)
            {
                text_id = TXT_MBBMS_CONTENT_PLAY_NOT_START;
            }
            else
            {
                text_id = TXT_MBBMS_CONTENT_PLAY_EXPIRED;
            }
            
            //提示后播放
            MMIPUB_OpenAlertWinByTextId(PNULL,
                text_id,
                TXT_NULL,
                IMAGE_PUBWIN_WARNING,
                &alert_win_id,
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                HandleContentAlertWinMsg);
        }
    }
    
    MBBMSSG_FreeContent(&content);
    
    
}

/*****************************************************************************/
//     Description : mbbms enter play interface
//    Global resource dependence : none
//  Author: lisa lin
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterPlay(anyURI id)
{
    BOOLEAN is_audio_service = FALSE;

    MMIMBBMS_OpenWatchConnectingWin();
    
    MMIMBBMS_SetCurrPlayUri(id);
    
    if(MMIMBBMS_PLAY_FROM_SERVICE == MMIMBBMS_GetPlayFromType())
    {
        is_audio_service = MMIMBBMS_IsServiceAudioTyle(id);
        
#ifdef ENG_SUPPORT
        if(is_audio_service
            && !MMIAPIENG_IsAudioServiceEnable())
        {
            s_mbbms_play_info.is_audio_type = FALSE;
            ResetCurServiceInfo();
            
            MMIMBBMS_StopPlay();        
            MMIMBBMS_DisplayCommonError(TXT_MBBMS_UNSUPPORT_AUDIO_SERVICE);

            //SCI_TRACE_LOW:"MMIMBBMS_EnterPlay service cannot play audio service!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5382_112_2_18_2_39_38_247,(uint8*)"");
            return;
        }
#endif
        MMIMBBMS_PlayService(id);
    }
    else
    {
        is_audio_service = MMIMBBMS_IsContentAudioTyle(id);
        
        //play from content,get the corresponding service according to content  
#ifdef ENG_SUPPORT
        if(is_audio_service
            && !MMIAPIENG_IsAudioServiceEnable())
        {
            s_mbbms_play_info.is_audio_type = FALSE;
            ResetCurServiceInfo();
            
            MMIMBBMS_StopPlay();        
            MMIMBBMS_DisplayCommonError(TXT_MBBMS_UNSUPPORT_AUDIO_SERVICE);

            //SCI_TRACE_LOW:"MMIMBBMS_EnterPlay content cannot play audio service!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5403_112_2_18_2_39_38_248,(uint8*)"");
            return;
        }
#endif
        MMIMBBMS_PlayContent(id);
    }
}

/*****************************************************************************/
// 	Description : enter cmmb mode play
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterCMMBModePlay(MMIMBBMS_CMMB_PROG_ID_T *prog_t_ptr)
{
    //SCI_TRACE_LOW:"MMIMBBMS_EnterCMMBModePlay  frequency_no=%d, service_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5417_112_2_18_2_39_38_249,(uint8*)"dd", prog_t_ptr->net_id, prog_t_ptr->service_id);
    
    if(PNULL == prog_t_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_EnterCMMBModePlay prog_t_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5421_112_2_18_2_39_38_250,(uint8*)"");
        return;
    }
    
    MMIMBBMS_OpenWatchConnectingWin();
    
    //save curr cmmb prog id
    s_mbbms_play_info.cmmb_id.net_id     = prog_t_ptr->net_id;
    s_mbbms_play_info.cmmb_id.service_id = prog_t_ptr->service_id;
    
    //check audio type
    s_mbbms_play_info.is_audio_type = MMIMBBMS_IsCmmbServiceAudioTyle(MMIMBBMS_GetPlayingServiceIndex());
    
#ifdef ENG_SUPPORT
    if(s_mbbms_play_info.is_audio_type
        && !MMIAPIENG_IsAudioServiceEnable())
    {
        s_mbbms_play_info.is_audio_type = FALSE;
        ResetCurServiceInfo();
        
        MMIMBBMS_StopPlay();
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_UNSUPPORT_AUDIO_SERVICE);
        
        //SCI_TRACE_LOW:"MMIMBBMS_EnterCMMBModePlay cannot play audio service!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5444_112_2_18_2_39_38_251,(uint8*)"");
        return;
    }
#endif
    MMIMBBMS_EnterPlayprocess();
}

/*****************************************************************************/
// Description : Get NAF address and port
// Global resource dependence :
// Author: lisa.lin
// Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_GetNAFAddr(uint8* naf_addr,uint8* naf_addr_len,uint32* port)           
{
    SG_LIST_PTR_T   serviceIdList_head_ptr = PNULL;
    SG_ERROR_E      error_code = SG_ERROR_NONE;    
    anyURI*         uri_ptr = PNULL;
    int             service_num = 0;
    uint8           temp[MMIMBBMS_MAX_NAF_ADDR_LEN]={0};
    
    if(PNULL == naf_addr
        || PNULL == naf_addr_len
        || PNULL == port)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetNAFAddr naf_addr:0x%d, naf_addr_len:0x%x, port:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5467_112_2_18_2_39_38_252,(uint8*)"ddd", naf_addr, naf_addr_len, port);
        return;
    }
    
    error_code=MBBMSSG_GetServiceIndex(&serviceIdList_head_ptr);
    service_num =STL_LinkListGetNodeDataCount(serviceIdList_head_ptr);
    SCI_MEMSET(temp,0x00,sizeof(temp));
    
    //SCI_TRACE_LOW:"MMIMBBMS_GetNAFAddr error_code =%d, service_num=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5475_112_2_18_2_39_38_253,(uint8*)"dd",error_code,service_num);
    
    if((SG_ERROR_NONE==error_code)&&( 0 < service_num))        
    {
        int i=0;
        
        for(i=0;i<service_num;i++)
        {
            MBBMSSG_SERVICE_INDEX_T *service_index_ptr=PNULL;
            MBBMSSG_ACCESS_T access={0};
            
            service_index_ptr=(MBBMSSG_SERVICE_INDEX_T*)STL_LinkListGetNodeDataByIdx(serviceIdList_head_ptr, i);
            if(PNULL == service_index_ptr)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_GetNAFAddr service_index_ptr is null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5489_112_2_18_2_39_38_254,(uint8*)"");
                return;
            }
            
            error_code=MBBMSSG_GetAccess(service_index_ptr->index, &access);
            
            if((SG_ERROR_NONE == error_code )&&(PNULL!=access.KeyManagementSystemList))
            {
                //find naf address
                SG_KEY_MAN_SYS_T  *sg_key_info_ptr=PNULL;
                uint8  str_len=0;
                
                //SCI_TRACE_LOW:"MMIMBBMS_GetNAFAddr get access\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5501_112_2_18_2_39_38_255,(uint8*)"");
                
                sg_key_info_ptr=(SG_KEY_MAN_SYS_T*)STL_LinkListGetNodeDataByIdx(access.KeyManagementSystemList, 0);
                
                uri_ptr =&(sg_key_info_ptr->PermissionIssuerURI);
                
                if(PNULL==uri_ptr->anyURI_ptr)
                {
                    uint16 ori_len = strlen((char*)uri_ptr->anyURI_arr);
                    
                    str_len=MIN(MMIMBBMS_MAX_NAF_ADDR_LEN, ori_len);                
                    strncpy((char*)temp,(char*)(uri_ptr->anyURI_arr),str_len);
                }
                else
                {
                    str_len=MIN(MMIMBBMS_MAX_NAF_ADDR_LEN,uri_ptr->string_len);        
                    strncpy((char*)temp,(char*)(uri_ptr->anyURI_ptr),str_len);
                }        
                
                *naf_addr_len =str_len;
                
                //SCI_TRACE_LOW:"MMIMBBMS_GetNAFAddr temp =%s\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5522_112_2_18_2_39_38_256,(uint8*)"s",temp);
                
                if(strstr((char*)temp,"http://"))
                {
                    *naf_addr_len = *naf_addr_len -7;
                    strncpy((char*)naf_addr,(char*)(temp+7),*naf_addr_len);
                }
                else
                {
                    strncpy((char*)naf_addr,(char*)temp,*naf_addr_len);
                }                
                naf_addr[*naf_addr_len]='\0';
                MBBMSSG_FreeAccess(&access);
                break;
            }
            
            MBBMSSG_FreeAccess(&access);
        }
        
        if(i==service_num)
        {
            //doesn't find naf address        
            //SCI_TRACE_LOW:"MMIMBBMS_GetNAFAddr doesn't find"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5544_112_2_18_2_39_38_257,(uint8*)"");
            *naf_addr_len=0;
            naf_addr[*naf_addr_len]='\0';
        }
        
        *port=0;
    }
    else
    {
        *naf_addr_len=0;
        naf_addr[*naf_addr_len]='\0';
        *port=0;
    }
    
    MBBMSSG_FreeServiceIndex(&serviceIdList_head_ptr);
    //SCI_TRACE_LOW:"MMIMBBMS_GetNAFAddr naf addr =%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5559_112_2_18_2_39_38_258,(uint8*)"s",naf_addr);
}


/*****************************************************************************/
// Description : Set sg filter
// Global resource dependence :
// Author: lisa.lin
// Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_SetSGFilter(void)
{
    MBBMSSG_FILTER_T sg_filter={0};
    SCI_DATE_T    date={0};
    int i=0;
    
    //get system date and set sg search filter for 3 days
    TM_GetSysDate(&date);
    
    sg_filter.startTime.years=(uint32)date.year;
    sg_filter.startTime.months=(uint32)date.mon;
    sg_filter.startTime.days=(uint32)date.mday;
    sg_filter.startTime.hours=0;
    sg_filter.startTime.minutes=0;
    sg_filter.startTime.seconds=0;
    
    //select three days 
    for(i=0;i<3;i++)
    {
        MMIAPIALM_IncreaseDateByOne(&date);
    }
    
    sg_filter.endTime.years=(uint32)date.year;
    sg_filter.endTime.months=(uint32)date.mon;
    sg_filter.endTime.days=(uint32)date.mday;
    sg_filter.endTime.hours=0;
    sg_filter.endTime.minutes=0;
    sg_filter.endTime.seconds=0;    
    
    //Set SG Filter
    MBBMSSG_SetFilter(sg_filter);
}

/*****************************************************************************/
// Description : Enter into mbbms 
// Global resource dependence :
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterMBBMS(void)
{
    
    //start init process
    {
        MMI_WIN_ID_T alert_win_id = MMIMBBMS_MEMFULL_ALERT_WIN_ID;           
        MMIFILE_DEVICE_E dev  = MMI_DEVICE_UDISK;

        dev = MMIAPIFMM_GetFirstValidDevice();
        if((dev >= MMI_DEVICE_NUM))
        {
            //无可用存储器
            //SCI_TRACE_LOW:"MMIMBBMS_EnterMBBMS both sd and usdisk doesn't exist\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5615_112_2_18_2_39_38_259,(uint8*)"");
            s_sgfile_location = MMI_DEVICE_NUM;
            MMIPUB_OpenAlertWarningWin(TXT_MBBMS_NO_MEDIA);          
            return;
        }

        if(SFS_NO_ERROR != MMIAPIFMM_GetSuitableFileDev(dev, MMIMBBMS_MEMORY_NEEDED_SIZE, &dev))
        {
            //未找到满足条件的存储器
            //SCI_TRACE_LOW:"MMIMBBMS_EnterMBBMS udisk no space  K\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5624_112_2_18_2_39_38_260,(uint8*)"");
            MMIPUB_OpenAlertWinByTextId(PNULL,
                TXT_MBBMS_NO_SPACE,
                TXT_NULL,
                IMAGE_PUBWIN_WARNING,
                &alert_win_id,
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                HandleMemAlertWinMsg);      
            return;
        }
        else
        {
            s_sgfile_location = dev;
        }
        
        MMIMBBMS_InitProcess();
    }
}

/*****************************************************************************/
// 	Description : to handle alert window closed message
//	Global resource dependence : 
//  Author:lisa.lin
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMemAlertWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
        
    case MSG_TIMER:
        {
            if (win_info_ptr->timer_id == *(uint8*)param)
            {
                MMIMBBMS_InitProcess();
                MMK_CloseWin(win_id);
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }
        break;
    case MSG_APP_CANCEL:	
        MMIMBBMS_InitProcess();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        //用户红键退出,需要关闭V、释放资源
        //MMIMBBMS_CloseCMMB();
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
    
}

/*****************************************************************************/
// Description : 获取MBBMS业务初始化参数
// Global resource dependence :
// Author: lisa.lin
// Note: 
/*****************************************************************************/
LOCAL void GetServiceInitPara(
                              MBBMS_Service_Init_Para_T *init_ptr //out
                              )
{
    wchar   *device_ptr = PNULL;
    uint16  device_len = 0;        
    wchar   full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint8   setting_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T*  setting_item_ptr = PNULL;
    
    if(PNULL == init_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_Init: init_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5711_112_2_18_2_39_38_261,(uint8*)"");
        return;
    }
    
    //service mode
    init_ptr->mode = MMIMBBMS_GetServiceMode();
    
    //SIM no
    init_ptr->sim_no = MMIMBBMS_GetCurSelectSim();
    device_ptr = MMIAPIFMM_GetDevicePath(s_sgfile_location);
    device_len = MMIAPIFMM_GetDevicePathLen(s_sgfile_location); 
    if((PNULL == device_ptr) || (0 == device_len))
    {
        //SCI_TRACE_LOW:"MMIMBBMS_Init: no u disk and sd card\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5724_112_2_18_2_39_38_262,(uint8*)"");
        device_ptr = (wchar*)MMIAPIFMM_GetDevicePath(MMIAPIFMM_GetDefaultDisk());
        device_len = MMIAPIFMM_GetDevicePathLen(MMIAPIFMM_GetDefaultDisk()); 
    }
    
    MMIAPIFMM_CombineFullPath(
        device_ptr, 
        device_len,
        MMIMULTIM_DIR_SYSTEM, 
        MMIMULTIM_DIR_SYSTEM_LEN,
        PNULL, 
        0,
        full_path_name,
        &full_path_len
        );
    
    SCI_MEMCPY(init_ptr->sg_path, (uint8*)full_path_name, full_path_len*2);
    init_ptr->sg_path_len = full_path_len*2;
    
    //cmmb esg path
    SCI_MEMCPY(init_ptr->cmmb_esg_path, (uint8*)full_path_name, full_path_len*2);
    init_ptr->cmmb_esg_len = full_path_len*2;
    
    //SG URL
    strncpy((char*)(init_ptr->sg_uri), g_mbbms_setting_info.sg_domain_info, M_MMIMBBMS_MAX_SG_DOMAIN_LENS);
    init_ptr->sg_uri_len = strlen(g_mbbms_setting_info.sg_domain_info);
    
    //wap_addr
    setting_index = MMIMBBMS_GetNetSettingIndex(init_ptr->sim_no);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(init_ptr->sim_no, setting_index);
    strncpy((char*)(init_ptr->wap_addr), (char*)(setting_item_ptr->gateway), (setting_item_ptr->gateway_len));
    init_ptr->wap_port = setting_item_ptr->port;
    
}

/*****************************************************************************/
// Description :mbbms init process
// Global resource dependence :
// Author: lisa.lin
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_InitProcess(void)
{
    uint32  sim_num = 0;
    uint16  sim_sys  = MN_DUAL_SYS_1;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    //SCI_TRACE_LOW:"MMIMBBMS_InitProcess enter  TickCount=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5769_112_2_18_2_39_38_263,(uint8*)"d", SCI_GetTickCount());
    
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_MTV, FALSE);
    
    //获取可用SIM数目
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    
    if(0 == sim_num)    //没有可用的SIM卡
    {
        MMIMBBMS_SetServiceMode(MBBMS_SERVICE_MODE_CMMB);
        
        MMIMBBMS_SetStartUpWinCloseFlag(TRUE);
        MMIMBBMS_OpenStartUpWin();
    }
    else if(1 == sim_num)   //只有一张卡使用，则直接将该卡作为参数
    {
        dual_sys = (MN_DUAL_SYS_E)sim_sys;
        MMIMBBMS_SetCurSelectSim(dual_sys);
        
        MMIMBBMS_SetServiceMode(MBBMS_SERVICE_MODE_MBBMS);
        
        MMIMBBMS_SetStartUpWinCloseFlag(TRUE);
        MMIMBBMS_OpenStartUpWin();
    }
    else
    {
        //多张SIM卡，则弹出选卡界面
        MMIMBBMS_OpenSimSelectWin();
    }
}

/*****************************************************************************/
// 	Description : handle startup win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIMBBMS_HandleStartUpWinMsg( 
                                                 MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E            result   = MMI_RESULT_TRUE;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info =    {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    uint16                  form_space = MMIMBBMS_FORM_VERTICAL_SPACE;
	uint16	tv_logo_height = 0;
	uint16	progress_height = 0;
	uint16  text_hight = 0;
	uint16  form_display_height = 0;
	//uint16  ver_top = 0;	
	GUI_RECT_T form_rect= {0};	
	GUI_POINT_T		display_point = {0};  
	uint16 lcd_width = 0;
	uint16 lcd_height = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_FONT_T    font = MMI_DEFAULT_NORMAL_FONT;
            MMI_STRING_T  welcome_info ={0};
            wchar         welcome_wchar[M_MMIMBBMS_MAX_WELCOME_INFO_LENS+1] = {0};
            const MMIMBBMS_SETTING_INFO_T   *setting_info_ptr = PNULL;

            //set form not support slide
            GUIFORM_IsSlide(MMIMBBMS_STARTUP_FORM_CTRL_ID,FALSE);
            
            setting_info_ptr = MMIMBBMS_GetConstSettingInfo();
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
            MAIN_SetTVSignalState();
            
            //welcome info str
            if (setting_info_ptr->welcome_info_len > 0)
            {
                GUI_UTF8ToWstr(welcome_wchar, M_MMIMBBMS_MAX_WELCOME_INFO_LENS,
                    setting_info_ptr->sg_welcome_info, 
                    setting_info_ptr->welcome_info_len);
                welcome_info.wstr_ptr = welcome_wchar;
                welcome_info.wstr_len = MMIAPICOM_Wstrlen(welcome_wchar);
                //SCI_TRACE_LOW:"MMIMBBMS HandleStartUpWinMsg wchar welcome_info_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5848_112_2_18_2_39_39_264,(uint8*)"d", welcome_info.wstr_len);
            }
            else
            {
                //set default sg welcome info    
                MMI_GetLabelTextByLang(TXT_MBBMS_WELCOME, &welcome_info);
            }
            GUITEXT_SetFont(MMIMBBMS_TEXT_START_CTRL_ID, &font, PNULL);
            GUITEXT_SetAlign(MMIMBBMS_TEXT_START_CTRL_ID, ALIGN_HMIDDLE);
            GUITEXT_SetString(MMIMBBMS_TEXT_START_CTRL_ID,welcome_info.wstr_ptr,welcome_info.wstr_len,FALSE);
            
            //tv logo
            data_info.img_id = IMAGE_MBBMS_TV_LOGO;
            control_info.is_ctrl_id = TRUE;
            control_info.ctrl_id = MMIMBBMS_ANIM_START_CTRL_ID;
            control_info.ctrl_ptr = PNULL;
            display_info.is_syn_decode = TRUE;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
            
            //progress bar
            data_info.anim_id = AINM_MTV_PROGRESS;
            control_info.is_ctrl_id = TRUE;
            control_info.ctrl_id = MMIMBBMS_ANIM_PROGRESS_CTRL_ID;
            control_info.ctrl_ptr = PNULL;
            display_info.is_syn_decode = TRUE;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
			
			//set start win  display
			GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);			
			form_rect.left = 0;
			form_rect.right = lcd_width-1;
        	GUIRES_GetImgWidthHeight(PNULL,&tv_logo_height,IMAGE_MBBMS_TV_LOGO,win_id);		
        	GUIRES_GetImgWidthHeight(PNULL,&progress_height,AINM_MTV_PROGRESS,win_id);	
			text_hight =GUI_GetStringHeight(font,welcome_info.wstr_ptr,welcome_info.wstr_len);		
			form_display_height =tv_logo_height + form_space + progress_height + form_space + text_hight; //form 中显示部分的高度	
			//设置显示区域上半部空白是下半部空白的一半
			form_rect.top = (lcd_height - MMI_STATUSBAR_HEIGHT - form_display_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)/3 + MMI_STATUSBAR_HEIGHT;
			if(0 > form_rect.top)
			form_rect.top = MMI_STATUSBAR_HEIGHT;	
			
			form_rect.bottom = lcd_height- MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1;
   			GUIFORM_SetRect(MMIMBBMS_STARTUP_FORM_CTRL_ID,&form_rect);    
            
            GUIFORM_SetSpace(MMIMBBMS_STARTUP_FORM_CTRL_ID,&form_space,&form_space);
            MMK_SetAtvCtrl(win_id, MMIMBBMS_STARTUP_FORM_CTRL_ID);
            MMITHEME_UpdateRect();
            //SCI_TRACE_LOW:"MMIMBBMS_OpenStartUpWin MSG_OPEN_WINDOW TickCount=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5896_112_2_18_2_39_39_265,(uint8*)"d", SCI_GetTickCount());
            
            MMK_PostMsg(win_id, MSG_MBBMS_INIT_IND, PNULL, NULL);
        }
            break;      
    case MSG_MBBMS_INIT_IND:
        {
            MBBMS_Service_Init_Para_T       init_para = {0};
            MMIMTV_InitCMMB();//需要先初始化CMMB，然后再初始化MBBMS          
            //mmimbbms init
            MMIMBBMS_SetMBBMSOpenStatus(TRUE);
            MMIMBBMS_Init();
            
            //MTVSERVICE同步初始化
            GetServiceInitPara(&init_para);
            //SCI_TRACE_LOW:"MMIMBBMS_InitProcess MTVSERVICE_Init  before TickCount=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5911_112_2_18_2_39_39_266,(uint8*)"d", SCI_GetTickCount());
            MTVSERVICE_Init(&init_para, MMIMBBMS_ServiceCallback);    
#ifdef ENG_SUPPORT            
            if(MMIAPIENG_IsInterfaceTestingEnable())
	        {
                MTVSERVICE_SetMskTestMode(TRUE);
	        } 
#endif             
            //SCI_TRACE_LOW:"MMIMBBMS_InitProcess MTVSERVICE_Init  after TickCount=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5919_112_2_18_2_39_39_267,(uint8*)"d", SCI_GetTickCount());
            //MMIMTV_InitCMMB();
            
            //Start Parse SG
            MMIMBBMS_StartParseSG(MMIMBBMS_SG_SEARCH_PARSE_FROM_PRE_INIT);
        }
        break;
        
    case MSG_FULL_PAINT:
			GUIRES_DisplayImg(&display_point,
				PNULL,
				PNULL,
				win_id,
				IMAGE_COMMON_BG,
				MMITHEME_GetDefaultLcdDev());

        //SCI_TRACE_LOW:"MMIMBBMS_OpenStartUpWin MSG_FULL_PAINT TickCount=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5935_112_2_18_2_39_39_268,(uint8*)"d", SCI_GetTickCount());
        break;
        
    case MSG_APP_CANCEL:		
    case MSG_CTL_CANCEL:
        {
            MMIMBBMS_ExitMBBMS();
            break;
        }
        
    case MSG_APP_RED:
        MMIMBBMS_ExitMBBMS();
        result = MMI_RESULT_FALSE;
        break;
        
    case MSG_CLOSE_WINDOW:
        //非正常关闭StartUpWin，需执行MBBMS退出流程
        if(MMIMBBMS_IsStartUpWinAbnormalClose())
        {
            //SCI_TRACE_LOW:"[MMIMBBMS]: HandleStartUpWinMsg abnormal colsed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5954_112_2_18_2_39_39_269,(uint8*)"");
            MMIMBBMS_ExitMBBMS();
        }
        
        //SCI_TRACE_LOW:"[MMIMBBMS]: close mbbms init window\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5958_112_2_18_2_39_39_270,(uint8*)"");
        break;

    default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    
    return (result);
}

/*****************************************************************************/
// Description :Exit MBBMS
// Global resource dependence :
// Author: lisa.lin
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_ExitMBBMS(void)
{
    MMISRV_HANDLE_T audio_handle = MMIMTV_GetAudioHandle();
    
    //SCI_TRACE_LOW:"MMIMBBMS_ExitMBBMS start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5979_112_2_18_2_39_39_271,(uint8*)"");
    if(!MMIMTV_IsVRunning())
    {
        //app is already closed
        //SCI_TRACE_LOW:"MMIMBBMS_ExitMBBMS app is already closed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5983_112_2_18_2_39_39_272,(uint8*)"");
        return;
    }
    
    s_is_mbbms_exiting= TRUE;
    
    MMINV_WRITE(MMINV_MBBMS_SETTING_INFO,&g_mbbms_setting_info);
    
    if(!MMIMBBMS_StopFlow())
    {
        //if no service is working, exit directly
        //SCI_TRACE_LOW:"MMIMBBMS_ExitMBBMS no service is working\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_5994_112_2_18_2_39_39_273,(uint8*)"");
        MMIMBBMS_Clear();
    }
    else
    {
        MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MBBMS_PRE_EXIT_MSG,PNULL,0);
    }

    if(audio_handle > 0)
    {
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
        MMIMTV_SetAudioHandle(0);
    }
}

/*****************************************************************************/
// 	Description : init network notify info
//	Global resource dependence :
//  Author: xin.li
//	Note: 初始化网络通知信息，网络状态变化时通知MTV
/*****************************************************************************/
PUBLIC void MMIMTV_InitNetworkNotifyInfo(void)
{
    MMIPHONE_NOTIFY_INFO_T notify_info = {0};
    
    notify_info.module_id = MMI_MODULE_MTV;
    notify_info.notify_func = MMIMBBMS_NotifyEventCallback;
    MMIAPIPHONE_RegisterNotifyInfo(&notify_info);
}

/*****************************************************************************/
// 	Description : notify callback
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_NotifyEventCallback(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event, uint32 param)
{
#ifdef MBBMS_SUPPORT
    //SCI_TRACE_LOW:"[MMIMTV]: NotifyEventCallback dual_sys=%d, notify_event=%d, param=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6030_112_2_18_2_39_39_274,(uint8*)"ddd", dual_sys, notify_event, param);

    //无网恢复有网时，CMMB模式转换到MBBMS模式
   if(MMIPHONE_NOTIFY_EVENT_CS_AVAIL == notify_event)
    {
        if(MMIMBBMS_IsMBBMSOpen()
            && (MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode()))
        {
            //记录来源SIM卡
            MMIMBBMS_SetCurSelectSim(dual_sys);        
            MMIMBBMS_OpenSwitchToMBBMSQueryWin();
        }
    }
#endif
}

/*****************************************************************************/
//  Description : Get mbbms net setting index 
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMBBMS_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    uint8 index = 0;
    
    if(MN_DUAL_SYS_1 == dual_sys)
    {
        index = g_mbbms_setting_info.link_setting.sim1_index;
    }
    else
    {
        index =g_mbbms_setting_info.link_setting.sim2_index;
    }
    return index;
}


/*****************************************************************************/
//     Description : get mbbms setting info
//    Global resource dependence : 
//  Author: plum.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetSettingInfo(MN_DUAL_SYS_E dual_sys,
                                    uint16* notify_type_ptr,
                                    uint16* nerwork_index_ptr, 
                                    MMI_STRING_T* sg_ptr,
                                    MMI_STRING_T* naf_ptr)
{
    if(PNULL == notify_type_ptr
        || PNULL == nerwork_index_ptr
        || PNULL == sg_ptr
        || PNULL == naf_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetSettingInfo notify_type_ptr:0x%x, nerwork_index_ptr:0x%x, sg_ptr:0x%x, naf_ptr:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6082_112_2_18_2_39_39_275,(uint8*)"dddd", notify_type_ptr, nerwork_index_ptr, sg_ptr, naf_ptr);
        return;
    }

    *notify_type_ptr       = g_mbbms_setting_info.time_remainder;
    // *nerwork_index_ptr = g_mbbms_setting_info.gateway_index;
    if(MN_DUAL_SYS_1 == dual_sys)
    {
        *nerwork_index_ptr = g_mbbms_setting_info.link_setting.sim1_index;
    }
    else
    {
        *nerwork_index_ptr = g_mbbms_setting_info.link_setting.sim2_index;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_GetSettingInfo sg_domain_info = %s "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6097_112_2_18_2_39_39_276,(uint8*)"s", g_mbbms_setting_info.sg_domain_info);
    
    MMIAPICOM_StrToWstr((uint8*)g_mbbms_setting_info.sg_domain_info, sg_ptr->wstr_ptr);
    sg_ptr->wstr_len = strlen(g_mbbms_setting_info.sg_domain_info);
    
    MMIAPICOM_StrToWstr((uint8*)g_mbbms_setting_info.sg_naf_addr, naf_ptr->wstr_ptr);
    naf_ptr->wstr_len = strlen(g_mbbms_setting_info.sg_naf_addr);
    
    return;
}

/*****************************************************************************/
//  Description : MMIMBBMS_InitContentNotifyInfo
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_InitContentNotifyInfo(void)
{
    MMI_TM_T dstTime = {0};
    uint16 remindertime = 0;
    BOOLEAN event_flag = FALSE;
    SCI_TM_T systime = {0};
    SCI_TM_T rel_systime = {0};
    SCI_DATE_T tomDate = {0};
    SG_DATETIME_T almTime = {0};    
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
    BOOLEAN bret = FALSE;
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = PNULL;
	
    if(TRUE == MMIMBBMS_ReadAlarmContentFile())
    {
         cursor = MMIMBBMS_GetAlarmContentNodeViaIndex(0);
	}
	else
	{
        //SCI_TRACE_LOW:"MMIMBBMS_InitContentNotifyInfo no alarm content file"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6131_112_2_18_2_39_39_277,(uint8*)"");
        return ;
	}
    
    remindertime = MMIMBBMS_GetRemindTime(g_mbbms_setting_info.time_remainder);
    if (0 == remindertime)
    {
        /* 关闭提醒 */
        return;
    }
    
    TM_GetTime(&rel_systime);
    
    //SCI_TRACE_LOW:"MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6148_112_2_18_2_39_39_278,(uint8*)"ddddd", rel_systime.tm_year,rel_systime.tm_mon,rel_systime.tm_mday,rel_systime.tm_hour,rel_systime.tm_min);
    
    MMIMBBMS_CancelAllAlarm();
    
    // 循环读取所有设置了节目提醒的节目
    while (cursor != PNULL)
    {
        systime = rel_systime;
        
        //SCI_TRACE_LOW:"MMIMBBMS_InitContentNotifyInfo  %d:%d:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6159_112_2_18_2_39_39_279,(uint8*)"ddd", cursor->starttime.hours,cursor->starttime.minutes,cursor->starttime.seconds);
        
        if (systime.tm_hour < cursor->starttime.hours
            || (systime.tm_hour == cursor->starttime.hours
            && systime.tm_min < cursor->starttime.minutes)
            || (systime.tm_hour == cursor->starttime.hours
            && systime.tm_min == cursor->starttime.minutes
            && systime.tm_sec < cursor->starttime.seconds))
        {
            systime.tm_hour = cursor->starttime.hours;
            systime.tm_min  = cursor->starttime.minutes;
            systime.tm_sec  = cursor->starttime.seconds;
            
        }
        else
        {
            /* 得到第二天的日期 */
            MMIMBBMS_GetSelectedDate(&tomDate, 1);
            
            systime.tm_year = tomDate.year;
            systime.tm_mon  = tomDate.mon;
            systime.tm_mday = tomDate.mday;
            
            systime.tm_hour = cursor->starttime.hours;
            systime.tm_min  = cursor->starttime.minutes;
            systime.tm_sec  = cursor->starttime.seconds;
        }
        
        almTime.years = systime.tm_year;
        almTime.months = systime.tm_mon;
        almTime.days = systime.tm_mday;
        
        almTime.hours = systime.tm_hour;
        almTime.minutes = systime.tm_min;
        almTime.seconds = systime.tm_sec;
        
        //SCI_TRACE_LOW:"MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6199_112_2_18_2_39_39_280,(uint8*)"ddddd", almTime.years,almTime.months,almTime.days,almTime.hours,almTime.minutes);
        
        MMIMBBMS_GetNewRemindTime(&almTime, 
            0, remindertime, &dstTime);
        
        content_event_info.event_type = ALMSVC_EVENT_TV;
        content_event_info.event_time.year = dstTime.tm_year;
        content_event_info.event_time.mon = dstTime.tm_mon;
        content_event_info.event_time.mday = dstTime.tm_mday;
        content_event_info.event_time.hour = dstTime.tm_hour;
        content_event_info.event_time.min = dstTime.tm_min;
        content_event_info.event_time.sec = dstTime.tm_sec;
        content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMIMBBMS_ContentNotifyCallback;
        
        //SCI_TRACE_LOW:"MMIMBBMS_InitContentNotifyInfo %d-%d-%d %d:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6217_112_2_18_2_39_39_281,(uint8*)"ddddd", content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,content_event_info.event_time.hour,content_event_info.event_time.min);
        
        //SCI_TRACE_LOW:"MMIMBBMS_InitContentNotifyInfo before update cursor->event_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6219_112_2_18_2_39_39_282,(uint8*)"d", cursor->event_id);
        
        if (cursor->event_id > 0)
        {
            /* 最后一个参数表示是否立即启动ALM SERVICE， 放到循环后统一开启 */
            bret = MMIALMSVC_UpdateServcice(&content_event_info, cursor->event_id, TRUE, TRUE);
            
            if (!bret)
            {
                cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
            }
        }
        else
        {
            cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
        }
        
        //SCI_TRACE_LOW:"MMIMBBMS_InitContentNotifyInfo after update cursor->event_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6236_112_2_18_2_39_39_283,(uint8*)"d", cursor->event_id);
        
        /* 最后一个参数表示是否立即启动ALM SERVICE， 放到循环后统一开启 */
        //event_id = MMIALMSVC_RegService(&content_event_info, TRUE, FALSE);
        
        //cursor->event_id = event_id;
        
        event_flag = TRUE;
        
        cursor = cursor->content_next;
    }
    
    /* 如果存在节目提醒，则启动ALM SERVICE */
    if (event_flag)
    {
        MMIALMSVC_SetAlmService();
    }
    
    return;
}

/*****************************************************************************/
//  Description : MMIMBBMS_CancelAllAlarm
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_CancelAllAlarm(void)
{
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = MMIMBBMS_GetAlarmContentNodeViaIndex(0);
    
    //循环读取所有设置了节目提醒的节目
    while (cursor != PNULL)
    {
        MMIALMSVC_UnRegService(cursor->event_id, TRUE);
        cursor->event_id = 0;
        
        cursor = cursor->content_next;
    }
    
    return;
}

/*****************************************************************************/
//     Description : MMIMBBMS_UpdateAllContentAlarm
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_UpdateAllContentAlarm(uint16 old_time, uint16 new_time)
{
    MMI_TM_T dstTime = {0};
    BOOLEAN event_flag = FALSE;
    SCI_TM_T systime = {0};
    SCI_DATE_T tomDate = {0};
    SG_DATETIME_T almTime = {0};
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = MMIMBBMS_GetAlarmContentNodeViaIndex(0);
    
    TM_GetTime(&systime);
    
    // 循环读取所有设置了节目提醒的节目
    while (cursor != PNULL)
    {
        if (systime.tm_hour < cursor->starttime.hours
            || (systime.tm_hour == cursor->starttime.hours
            && systime.tm_min < cursor->starttime.minutes)
            || (systime.tm_hour == cursor->starttime.hours
            && systime.tm_min == cursor->starttime.minutes
            && systime.tm_sec < cursor->starttime.seconds))
        {
            systime.tm_hour = cursor->starttime.hours;
            systime.tm_min  = cursor->starttime.minutes;
            systime.tm_sec  = cursor->starttime.seconds;
        }
        else
        {
            /* 得到第二天的日期 */
            MMIMBBMS_GetSelectedDate(&tomDate, 1);
            
            systime.tm_year = tomDate.year;
            systime.tm_mon  = tomDate.mon;
            systime.tm_mday = tomDate.mday;
            
            systime.tm_hour = cursor->starttime.hours;
            systime.tm_min  = cursor->starttime.minutes;
            systime.tm_sec  = cursor->starttime.seconds;
        }
        
        almTime.years = systime.tm_year;
        almTime.months = systime.tm_mon;
        almTime.days = systime.tm_mday;
        
        almTime.hours = systime.tm_hour;
        almTime.minutes = systime.tm_min;
        almTime.seconds = systime.tm_sec;
        
        //SCI_TRACE_LOW:"MMIMBBMS_UpdateAllContentAlarm %d-%d-%d %d:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6333_112_2_18_2_39_40_284,(uint8*)"ddddd", almTime.years,almTime.months,almTime.days,almTime.hours,almTime.minutes);
        
        MMIMBBMS_GetNewRemindTime(&almTime, 
            0, new_time, &dstTime);
        
        content_event_info.event_type = ALMSVC_EVENT_TV;
        content_event_info.event_time.year = dstTime.tm_year;
        content_event_info.event_time.mon = dstTime.tm_mon;
        content_event_info.event_time.mday = dstTime.tm_mday;
        content_event_info.event_time.hour = dstTime.tm_hour;
        content_event_info.event_time.min = dstTime.tm_min;
        content_event_info.event_time.sec = dstTime.tm_sec;
        content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMIMBBMS_ContentNotifyCallback;
        
        //SCI_TRACE_LOW:"MMIMBBMS_UpdateAllContentAlarm %d-%d-%d %d:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6351_112_2_18_2_39_40_285,(uint8*)"ddddd", content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,content_event_info.event_time.hour,content_event_info.event_time.min);
        
        //SCI_TRACE_LOW:"before update cursor->event_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6353_112_2_18_2_39_40_286,(uint8*)"d", cursor->event_id);
        
        if (cursor->event_id > 0)
        {
            /* 最后一个参数表示是否立即启动ALM SERVICE， 放到循环后统一开启 */
            MMIALMSVC_UpdateServcice(&content_event_info, cursor->event_id, TRUE, TRUE);
        }
        else
        {
            cursor->event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
        }
        
        //SCI_TRACE_LOW:"after update cursor->event_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6365_112_2_18_2_39_40_287,(uint8*)"d", cursor->event_id);
        
        event_flag = TRUE;
        
        cursor = cursor->content_next;
    }
    
    /* 如果存在节目提醒，则启动ALM SERVICE */
    if (event_flag)
    {
        MMIALMSVC_SetAlmService();
    }
    
    return;
}

/*****************************************************************************/
//     Description : save mbbms setting info
//    Global resource dependence : 
//  Author: plum.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SaveSettingInfo(MN_DUAL_SYS_E dual_sys,
                                     uint16 notify_type,
                                     uint16 network_index, 
                                     wchar* pSGDomain,
                                     wchar* pNafAddr)
{
    uint16 remindertime = 0;
    
    if (g_mbbms_setting_info.time_remainder != notify_type)
    {
        // 调整提醒闹钟
        
        /* 得到节目提醒时间类型 */
        remindertime = MMIMBBMS_GetRemindTime(notify_type);
        
        if (0 == remindertime)
        {
            MMIMBBMS_CancelAllAlarm();
        }
        else if (MMIMBBMS_REMAINDER_OFF == g_mbbms_setting_info.time_remainder)
        {
            /* 调用MMIMBBMS_InitContentNotifyInfo之前要先将提醒类型设置为新的值 */            
            g_mbbms_setting_info.time_remainder = (MMIMBBMS_TIME_REMAINDER_E)notify_type;
            MMIMBBMS_InitContentNotifyInfo();
        }
        else
        {
            MMIMBBMS_UpdateAllContentAlarm(MMIMBBMS_GetRemindTime(g_mbbms_setting_info.time_remainder), 
                remindertime);
        }
        
        // 保存新的提醒时间
        g_mbbms_setting_info.time_remainder = (MMIMBBMS_TIME_REMAINDER_E)notify_type;
    }
    
    
    if(MN_DUAL_SYS_1==dual_sys)
    {
        g_mbbms_setting_info.link_setting.sim1_index=network_index;    
    }
    else
    {
        g_mbbms_setting_info.link_setting.sim2_index=network_index;
    }
    
    SCI_MEMSET(g_mbbms_setting_info.sg_domain_info, 0, sizeof(g_mbbms_setting_info.sg_domain_info));
    if (PNULL != pSGDomain)
    {
        MMIAPICOM_WstrToStr(pSGDomain, (uint8*)g_mbbms_setting_info.sg_domain_info);
    }
    
    SCI_MEMSET(g_mbbms_setting_info.sg_naf_addr, 0, sizeof(g_mbbms_setting_info.sg_naf_addr));
    if (PNULL != pNafAddr)
    {
        MMIAPICOM_WstrToStr(pNafAddr, (uint8*)g_mbbms_setting_info.sg_naf_addr);
    }
    
    //modify service wap addr
    MMIMBBMS_SetServiceWap();
    
    return;
}

 
/*****************************************************************************/
//  Description : register content alarm
//  Global resource dependence : 
//  Author: plum.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_RegisterContentAlarm(MMIMBBMS_ALARM_CONTENT_INFO_T* pContentInfo)
{
    MMI_TM_T dstTime = {0};
    uint16 remindertime = 0;
    uint16 event_id = 0;
    SCI_TM_T systime  = {0};
    SG_DATETIME_T almTime = {0};
    SCI_DATE_T tomDate = {0};
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
    MMI_TEXT_ID_T text_id =  0;

    if(PNULL == pContentInfo)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_RegisterContentAlarm pContentInfo is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6466_112_2_18_2_39_40_288,(uint8*)"");
        return;
    }
    
    remindertime = MMIMBBMS_GetRemindTime(g_mbbms_setting_info.time_remainder);
    if ( 0 == remindertime)
    {
        /* 已关闭提醒 */
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MBBMS_CONTENT_REGISTER_NOTIFY_INFO_NONE,TXT_NULL,IMAGE_PUBWIN_WARNING, 
            PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,MMIMBBMS_HandleAddorCancelAlarmServiceAlertWinMsg);       
        return;
    }
    
    TM_GetTime(&systime);
    
    if (systime.tm_hour < pContentInfo->starttime.hours
        || (systime.tm_hour == pContentInfo->starttime.hours
        && systime.tm_min < pContentInfo->starttime.minutes)
        || (systime.tm_hour == pContentInfo->starttime.hours
        && systime.tm_min == pContentInfo->starttime.minutes
        && systime.tm_sec < pContentInfo->starttime.seconds))
    {
        systime.tm_hour = pContentInfo->starttime.hours;
        systime.tm_min  = pContentInfo->starttime.minutes;
        systime.tm_sec  = pContentInfo->starttime.seconds;
    }
    else
    {
        /* 得到第二天的日期 */
        MMIMBBMS_GetSelectedDate(&tomDate, 1);
        
        systime.tm_year = tomDate.year;
        systime.tm_mon  = tomDate.mon;
        systime.tm_mday = tomDate.mday;
        
        systime.tm_hour = pContentInfo->starttime.hours;
        systime.tm_min  = pContentInfo->starttime.minutes;
        systime.tm_sec  = pContentInfo->starttime.seconds;
    }
    
    almTime.years = systime.tm_year;
    almTime.months = systime.tm_mon;
    almTime.days = systime.tm_mday;
    
    almTime.hours = systime.tm_hour;
    almTime.minutes = systime.tm_min;
    almTime.seconds = systime.tm_sec;
    
    MMIMBBMS_GetNewRemindTime(&almTime, 
        0, remindertime, &dstTime);
    
    content_event_info.event_type = ALMSVC_EVENT_TV;
    content_event_info.event_time.year = dstTime.tm_year;
    content_event_info.event_time.mon = dstTime.tm_mon;
    content_event_info.event_time.mday = dstTime.tm_mday;
    content_event_info.event_time.hour = dstTime.tm_hour;
    content_event_info.event_time.min = dstTime.tm_min;
    content_event_info.event_time.sec = dstTime.tm_sec;
    content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMIMBBMS_ContentNotifyCallback;
    
    event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);
    
    pContentInfo->event_id = event_id;
    
    //SCI_TRACE_LOW:"MMIMBBMS_RegisterContentAlarm %d-%d-%d %d:%d remindertime = %d event_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6532_112_2_18_2_39_40_289,(uint8*)"ddddddd",dstTime.tm_year, dstTime.tm_mon, dstTime.tm_mday,dstTime.tm_hour, dstTime.tm_min, remindertime, event_id);
    
    if (MMIMBBMS_REMAINDER_TWO_MIN == g_mbbms_setting_info.time_remainder)
    {
        text_id = TXT_MBBMS_CONTENT_REGISTER_NOTIFY_INFO_2;
    }
    else if (MMIMBBMS_REMAINDER_FIVE_MIN == g_mbbms_setting_info.time_remainder)
    {
        text_id = TXT_MBBMS_CONTENT_REGISTER_NOTIFY_INFO_5;
    }
    else if (MMIMBBMS_REMAINDER_TEN_MIN == g_mbbms_setting_info.time_remainder)
    {
        text_id = TXT_MBBMS_CONTENT_REGISTER_NOTIFY_INFO_10;    
    }
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_SUCCESS, 
        PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,MMIMBBMS_HandleAddorCancelAlarmServiceAlertWinMsg);     
    return;
}


/*****************************************************************************/
//  Description : get selected date
//  Global resource dependence :
//  Author:plum.peng
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_GetNewRemindTime(SG_DATETIME_T* pSrcTime, uint16 old_remindertime,
                                     uint16 new_remindertime, MMI_TM_T* pDstTime)
{
    uint32     uSeconds = 0; 
    
    if(PNULL == pSrcTime
        || PNULL == pDstTime)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetNewRemindTime pSrcTime:0x%x, pDstTime:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6564_112_2_18_2_39_40_290,(uint8*)"dd", pSrcTime, pDstTime);
        return;
    }
    
    uSeconds =  MMIAPICOM_Tm2Second(pSrcTime->seconds,
        pSrcTime->minutes,
        pSrcTime->hours,
        pSrcTime->days,
        pSrcTime->months,
        pSrcTime->years);
    
    uSeconds += 60 * old_remindertime - 60 * new_remindertime;
    
    *pDstTime = MMIAPICOM_Second2Tm(uSeconds);
    
    return;
    
}

/*****************************************************************************/
// Description : get index of service
// Global resource dependence :
// Author:plum.peng
// Note: data structure of node is MBBMSSG_SERVICE_INDEX_T
/*****************************************************************************/
PUBLIC SG_ERROR_E MMIMBBMS_GetServiceIndex(
                                           SG_LIST_PTR_T *serviceIdList_head_ptr)
{
    SG_ERROR_E errcode = SG_ERROR_NONE;
    
    if (PNULL == s_serviceIdList_head_ptr)
    {
        errcode = MBBMSSG_GetServiceIndex(serviceIdList_head_ptr);
        s_serviceIdList_head_ptr = *serviceIdList_head_ptr;
    }
    else
    {
        *serviceIdList_head_ptr = s_serviceIdList_head_ptr;
    }
    
    return errcode;
}

/*****************************************************************************/
// Description : free index of service
// Global resource dependence :
// Author:plum.peng
// Note: data structure of node is MBBMSSG_SERVICE_INDEX_T
/*****************************************************************************/
PUBLIC SG_ERROR_E MMIMBBMS_FreeServiceIndex(void)
{
    MBBMSSG_FreeServiceIndex(&s_serviceIdList_head_ptr);
    
    s_serviceIdList_head_ptr = PNULL;
    
    return SG_ERROR_NONE;
}

/*****************************************************************************/
//     Description : init mbbms setting info 
//    Global resource dependence : none
//  Author: lisa lin
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_InitSettingInfo(void)
{
    MN_RETURN_RESULT_E   ret_value = MN_RETURN_FAILURE;
    
    SCI_MEMSET(&g_mbbms_setting_info,0x00,sizeof(MMIMBBMS_SETTING_INFO_T));
    
    MMINV_READ(MMINV_MBBMS_SETTING_INFO, &g_mbbms_setting_info, ret_value);
    if(MN_RETURN_SUCCESS != ret_value)
    {
        //MMI_STRING_T  welcome_info_str ={0};
        
        //SCI_TRACE_LOW:"MMIMBBMS_InitSettingInfo read nv failed\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6633_112_2_18_2_39_40_291,(uint8*)"");
        SCI_MEMSET(&g_mbbms_setting_info,0,sizeof(MMIMBBMS_SETTING_INFO_T));
        
        g_mbbms_setting_info.link_setting.sim1_index=MMIMBBMS_SETTING_DEFAULT_INDEX;
        g_mbbms_setting_info.link_setting.sim2_index=MMIMBBMS_SETTING_DEFAULT_INDEX; 
        
        g_mbbms_setting_info.time_remainder=MMIMBBMS_REMAINDER_OFF;
        //add default sg domain and naf address
        strncpy(g_mbbms_setting_info.sg_domain_info,(char*)MMIMBBMS_DEFAULT_SG_DOMAIN,M_MMIMBBMS_MAX_SG_DOMAIN_LENS);
        strncpy(g_mbbms_setting_info.sg_naf_addr,(char*)MMIMBBMS_DEFAULT_NAF_ADDR,M_MMIMBBMS_MAX_NAF_ADDR_LENS);
        
        MMINV_WRITE(MMINV_MBBMS_SETTING_INFO,&g_mbbms_setting_info);
    }
}

/*****************************************************************************/
//     Description : Restore MBBMS setting info 
//    Global resource dependence : 
//  Author: lisa lin
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_RestoreFactorySetting(void)
{
    MMIMBBMS_SETTING_INFO_T  mbbms_setting_info = {0};
    //MMI_STRING_T  welcome_info_str = {0};
    
    SCI_MEMSET(&mbbms_setting_info,0,sizeof(MMIMBBMS_SETTING_INFO_T));  
    
    
    mbbms_setting_info.link_setting.sim1_index = MMIMBBMS_SETTING_DEFAULT_INDEX;
    mbbms_setting_info.link_setting.sim2_index = MMIMBBMS_SETTING_DEFAULT_INDEX;
    
    mbbms_setting_info.time_remainder = MMIMBBMS_REMAINDER_OFF;
    
    //should add default sg domain address and naf address    
    strncpy(mbbms_setting_info.sg_domain_info,(char*)MMIMBBMS_DEFAULT_SG_DOMAIN,M_MMIMBBMS_MAX_SG_DOMAIN_LENS);
    strncpy(mbbms_setting_info.sg_naf_addr,(char*)MMIMBBMS_DEFAULT_NAF_ADDR,M_MMIMBBMS_MAX_NAF_ADDR_LENS);
    
    MMINV_WRITE(MMINV_MBBMS_SETTING_INFO, &mbbms_setting_info); 
}

/*****************************************************************************/
// Description : enter play proccess
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterPlayprocess(void)
{
    uint32 frequency_no = 0;
    uint16 service_id = 0;
    uint16 service_index = 0;

    //SCI_TRACE_LOW:"MMIMBBMS_EnterPlayprocess curr flow status=%d,curr_flow type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6682_112_2_18_2_39_40_292,(uint8*)"dd",s_curr_flow_info.status,s_curr_flow_info.flow_type);
    if(!MMIMBBMS_IsPermitCMMBPlay())
    {
        //SCI_TRACE_LOW:"MMIMBBMS_EnterPlayprocess is not permit play!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6685_112_2_18_2_39_40_293,(uint8*)"");
        MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
        
        if(OSD_PLAY_CONNECTING == MMIMTV_GetFocusOSDScene())
        {
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        return;
    }
    
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        switch(MMIMBBMS_GetPlayFromType())
        {
        case MMIMBBMS_PLAY_FROM_SERVICE:
            {
                MBBMSSG_ACCESS_T access ={0};
                
                MBBMSSG_GetAccess(s_mbbms_play_info.uri, &access);
                frequency_no = access.CMMBPara.frequencyNo;
                service_id = access.CMMBPara.serviceid;
                
                //SCI_TRACE_LOW:"[MMIMBBMS]:MMIMBBMS_EnterPlayprocess   frequency_no = %d service_id = %d uri:%s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6710_112_2_18_2_39_40_294,(uint8*)"dds", frequency_no, service_id, s_mbbms_play_info.uri.anyURI_arr);
                
                service_index = MMIMBBMS_GetServiceIndexByServiceID(&(s_mbbms_play_info.uri));
                MMIMBBMS_SetServiceIdex(service_index);
                
                MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
                MMIMBBMS_Play(frequency_no, service_id);
                
                MBBMSSG_FreeAccess(&access);
                break;
            }            
        case MMIMBBMS_PLAY_FROM_CONTENT:
            {
                MBBMSSG_CONTENT_T content = {0};
                MBBMSSG_ACCESS_T  access  = {0};
                
                if(SG_ERROR_NONE==MBBMSSG_GetContent( s_mbbms_play_info.uri, &content))
                {
                    MBBMSSG_GetAccess(content.serviceIdRef, &access);
                    frequency_no = access.CMMBPara.frequencyNo;
                    service_id = access.CMMBPara.serviceid;
                    //SCI_TRACE_LOW:"[MMIMBBMS]:MMIMBBMS_EnterPlayprocess frequency_no = %d service_id = %d uri:%s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6732_112_2_18_2_39_40_295,(uint8*)"dds",frequency_no, service_id, s_mbbms_play_info.uri.anyURI_arr);
                    
                    service_index = MMIMBBMS_GetServiceIndexByServiceID(&(content.serviceIdRef));
                    MMIMBBMS_SetServiceIdex(service_index);
                    
                    MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
                    MMIMBBMS_Play(frequency_no, service_id);
                    
                    MBBMSSG_FreeAccess(&access);
                }
                else
                {
                    MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
                }
                
                MBBMSSG_FreeContent(&content);
                
                break;
            }
        default:
            MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_EnterPlayprocess cmmb service mode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6758_112_2_18_2_39_40_296,(uint8*)"");
        MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
        
        //CMMB mode service play by net_id and service_id
        frequency_no = s_mbbms_play_info.cmmb_id.net_id;
        service_id   = s_mbbms_play_info.cmmb_id.service_id;
        MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
        MMIMBBMS_Play(frequency_no, service_id);
    }
    
}



/*****************************************************************************/
// 	Description : set MBBMS open status
//	Global resource dependence :
//  Author:xin.li
//	Note: 设置MBBMS开启状态
/*****************************************************************************/
LOCAL void MMIMBBMS_SetMBBMSOpenStatus(BOOLEAN is_open)
{
    s_is_mbbms_open = is_open;
}

/*****************************************************************************/
//  Description : start subscribe process
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartSubscribeProcess(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_StartSubscribeProcess\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6787_112_2_18_2_39_40_297,(uint8*)"");
    if(MMIMBBMS_FLOW_STATE_IDLE != MMIMBBMS_GetFlowInfo()->status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartSubscribeProcess flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6790_112_2_18_2_39_40_298,(uint8*)"");
        return;
    }
    
    MMIMBBMS_OpenSubcribleRequestWin();
    if(MMIMBBMS_SubscriblePurchaseItem())
    {
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SUBSCRIBE, MMIMBBMS_FLOW_STATE_STARTING);
    }
    else
    {
        //start subscrible failed
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_SUBSCRIBE_FAILED);
        MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);
    }       
}

/*****************************************************************************/
//  Description : start unsubscribe process
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartUnSubscribeProcess(void)
{
    
    //SCI_TRACE_LOW:"MMIMBBMS_StartUnSubscribeProcess\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6814_112_2_18_2_39_40_299,(uint8*)"");
    if(MMIMBBMS_FLOW_STATE_IDLE != MMIMBBMS_GetFlowInfo()->status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartUnSubscribeProcess flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6817_112_2_18_2_39_40_300,(uint8*)"");
        return;
    }
    
    MMIMBBMS_OpenUnSubcribleRequestWin();
    if(MMIMBBMS_UnSubscriblePurchaseItemNew())
    {
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_UNSUBSCRIBE, MMIMBBMS_FLOW_STATE_STARTING);
    }
    else
    {    
        //start unsubscribe failed
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_UNSUBSCRIBE_FAILED);
        MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);
    }
}

/*****************************************************************************/
// 	Description : start update subscription table
//	Global resource dependence : none
//  Author: xin.li
//	Note: 更新订购关系表
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartUpdateSubscriptionTable(MMIMBBMS_SUB_UPDATE_FROM_TYPE_E from_type)
{
    
    //SCI_TRACE_LOW:"MMIMBBMS_StartUpdateSubscriptionTable from type=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6841_112_2_18_2_39_41_301,(uint8*)"d",from_type);
    
    if(MMIMBBMS_FLOW_STATE_IDLE != MMIMBBMS_GetFlowInfo()->status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartUpdateSubscriptionTable flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6845_112_2_18_2_39_41_302,(uint8*)"");
        return;
    }
    
    if((!MMIMBBMS_GetNAFSettingStatus()) || (!MMIMBBMS_GetServiceWapSetting()))
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartUpdateSubscriptionTable connection setting error \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6851_112_2_18_2_39_41_303,(uint8*)"");
        SubTableUpateFailed();
        return;
    }
    //set from flag
    //MMIMBBMS_SetSubUpdateType(from_type); 
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SUBTABLE_UPDATE, MMIMBBMS_FLOW_STATE_STARTING);
    if(MBBMS_SERVICE_FUNC_OK != MTVSERVICE_SubUpdate())
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartUpdateSubscriptionTable  MTVSERVICE_SubUpdate return fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6861_112_2_18_2_39_41_304,(uint8*)"");
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        MMIMBBMS_DisplayCommonError(TXT_UPDATE_FAIL);
    }
}

/*****************************************************************************/
// Description : start play failed
// Global resource dependence : none
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void StartPlayFailed(MBBMS_SERVICE_RET_E service_ret)
{
    //reset flow info
    if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type)
    {
        if(MBBMS_SERVICE_FUNC_BUSY != service_ret)
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
            SetCmmbPlayStatus(MMIMTV_PLAY_STOPPED);
        }
    }
    
    if(OSD_PLAY_CONNECTING == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
    }
    
    //提示播放错误
    MMIMTV_SetLastOSDScene();
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
}

/*****************************************************************************/
// Description : stop play failed
// Global resource dependence : none
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void StopPlayFailed(MBBMS_SERVICE_RET_E service_ret)
{
    //reset flow info
    if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type)
    {
        if(MBBMS_SERVICE_FUNC_BUSY != service_ret)
        {
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
            SetCmmbPlayStatus(MMIMTV_PLAY_STOPPED);
        }
    }
    
    if(OSD_PLAY_CONNECTING == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
    }
}

/*****************************************************************************/
// Description : start play flow
// Global resource dependence : none
// Author: xiaoxiang.huang
// Note: return result of operation with MTVSERVICE_Stop and MTVSERVICE_Play 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_StartPlay(void)
{
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_OK;
    uint32  frequency_no = MMIMBBMS_GetPlayInfo()->frequency_no;
    uint32  service_id   = MMIMBBMS_GetPlayInfo()->service_id;	
    
    //SCI_TRACE_LOW:"MMIMBBMS_StartPlay  frequency_no=%d, service_id=%d cur_flow_type=%d cur_flow_status=%d cmmb_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6934_112_2_18_2_39_41_305,(uint8*)"ddddd", frequency_no, service_id, MMIMBBMS_GetFlowInfo()->flow_type, MMIMBBMS_GetFlowInfo()->status, MMIMBBMS_GetPlayInfo()->cmmb_play_status);
    
    if(MMIMBBMS_FLOW_STATE_IDLE == MMIMBBMS_GetFlowInfo()->status)
    {
        //no flow running, start play directly
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_PLAY, MMIMBBMS_FLOW_STATE_STARTING);
        SetCmmbPlayStatus(MMIMTV_PLAY_WAITING);
        service_ret = MTVSERVICE_Play(frequency_no, service_id);
        if(MBBMS_SERVICE_FUNC_OK != service_ret)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_StartPlay  MTVSERVICE_Play return = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6944_112_2_18_2_39_41_306,(uint8*)"d", service_ret);
            StartPlayFailed(service_ret);
            return FALSE;			
        }
    }
    else if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type)
    {
        //current play flow
        if(MMIMBBMS_FLOW_STATE_STARTING == MMIMBBMS_GetFlowInfo()->status)
        {
            //if audio service, restore conditon
            RestoreAudioServiceCondition();
            
            //play flow is running, stop first
            MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_PLAY, MMIMBBMS_FLOW_STATE_STOPPING);
            SetCmmbPlayStatus(MMIMTV_PLAY_STOPPING);
            service_ret = MTVSERVICE_Stop();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_StopPlay  MTVSERVICE_Stop return fail "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6963_112_2_18_2_39_41_307,(uint8*)"");
                StopPlayFailed(service_ret);
				return FALSE;	
            }
            else
            {
                SetNeedPlayNextFlag(TRUE);
            }
        }
        else if(MMIMBBMS_FLOW_STATE_STOPPING == MMIMBBMS_GetFlowInfo()->status)
        {
            //play flow is stopping, set need play flag only
            SetNeedPlayNextFlag(TRUE);
        }
        else
        {
            //NOTHING TO DO
            //SCI_TRACE_LOW:"MMIMBBMS_StartPlay  status error in flow MMIMBBMS_FLOW_PLAY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6980_112_2_18_2_39_41_308,(uint8*)"");
        }
    }
    else
    {
        //NOTHING TO DO
        //SCI_TRACE_LOW:"MMIMBBMS_StartPlay  flow type and status error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_6986_112_2_18_2_39_41_309,(uint8*)"");
    }
	return TRUE;
}

/****************************************************************************/
// Description : stop play
// Global resource dependence : none
// Author: xiaoxiang.huang
// Note: return result of operation with MTVSERVICE_Stop 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_StopPlay(void)
{
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_OK;
    
    //SCI_TRACE_LOW:"MMIMBBMS_StopPlay  cur_flow_status=%d, cur_flow_type=%d cmmb_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7000_112_2_18_2_39_41_310,(uint8*)"ddd", MMIMBBMS_GetFlowInfo()->status, MMIMBBMS_GetFlowInfo()->flow_type, MMIMBBMS_GetPlayInfo()->cmmb_play_status);
    
    //restore audio service flag
    if(MMIMBBMS_IsCurrentServiceAudioTyle())
    {
        //s_mbbms_play_info.is_audio_type = FALSE;
        MMIMBBMS_ReleaseBgOSDLayer();
    }
    
    if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type
        && MMIMBBMS_FLOW_STATE_STARTING == MMIMBBMS_GetFlowInfo()->status)
    {
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_PLAY, MMIMBBMS_FLOW_STATE_STOPPING);
        SetCmmbPlayStatus(MMIMTV_PLAY_STOPPING);
        //if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
        {		
            //set quick_stop_flag
            s_mbbms_play_info.is_quick_stop_play = TRUE;
            s_mbbms_play_info.is_received_quick_ack = FALSE;
        }
        service_ret = MTVSERVICE_Stop();
        if(MBBMS_SERVICE_FUNC_OK != service_ret)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_StopPlay  MTVSERVICE_Stop return = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7023_112_2_18_2_39_41_311,(uint8*)"d", service_ret);
            StopPlayFailed(service_ret);
            //if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
            {			
                //set quick_stop_flag
                s_mbbms_play_info.is_quick_stop_play = FALSE;
                s_mbbms_play_info.is_received_quick_ack = FALSE;
            }	
            return FALSE;
        }
        
        //if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
        {
            //等待播放异步停止完成
#ifndef WIN32
            while(!s_mbbms_play_info.is_received_quick_ack)
            {
                SCI_Sleep(50);
            }
#endif
            //SCI_TRACE_LOW:"MMIMBBMS_StopPlay  cmmb stop complete!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7043_112_2_18_2_39_41_312,(uint8*)"");

            s_mbbms_play_info.is_quick_stop_play = FALSE;
            s_mbbms_play_info.is_received_quick_ack = FALSE;
        }	
    }
	return TRUE;
}

/****************************************************************************/
// Description : quit stop play
// Global resource dependence : none
// Author: xin.li
// Note: 快速停止播放，先停止CMMB播放，再停MBBMS相关流程
/*****************************************************************************/
PUBLIC void MMIMBBMS_QuickStopPlay(void)
{
    BOOLEAN stopplayresult = FALSE;
    //SCI_TRACE_LOW:"MMIMBBMS_QuickStopPlay  cur_flow_status=%d, cur_flow_type=%d cmmb_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7060_112_2_18_2_39_41_313,(uint8*)"ddd", MMIMBBMS_GetFlowInfo()->status, MMIMBBMS_GetFlowInfo()->flow_type, MMIMBBMS_GetPlayInfo()->cmmb_play_status);
#if 0    
    if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type)
    {
        //set quick_stop_flag
        s_mbbms_play_info.is_quick_stop_play = TRUE;
        s_mbbms_play_info.is_received_quick_ack = FALSE;
        
        MMIMBBMS_StopPlay();
        
        //等待播放异步停止完成
#ifndef WIN32
        while(!s_mbbms_play_info.is_received_quick_ack)
        {
            SCI_Sleep(50);
        }
#endif
        //SCI_TRACE_LOW:"MMIMBBMS_QuickStopPlay  complete!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7077_112_2_18_2_39_41_314,(uint8*)"");
        
        s_mbbms_play_info.is_quick_stop_play = FALSE;
        s_mbbms_play_info.is_received_quick_ack = FALSE;
        //resume play flag
        s_mbbms_play_info.is_need_resume_play = TRUE;
    }
#else  
        stopplayresult = MMIMBBMS_StopPlay();
        if(stopplayresult)
        {
            s_mbbms_play_info.is_need_resume_play = TRUE;        
            MMIMBBMS_GetPlayInfo()->is_need_play_next = FALSE;
		}		
#endif
}

/*****************************************************************************/
//  Description : start sg parse process
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartParseSG(MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E from_type)
{
    //SCI_TRACE_LOW:"MMIMBBMS_StartParseSG process from type=%d MMIMBBMS_TickCount=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7100_112_2_18_2_39_41_315,(uint8*)"dd",from_type,SCI_GetTickCount());

    MMIMBBMS_CheckSGDD();      
    //MMIMBBMS_SetSGOperationFromType(from_type);
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SG_PARSE, MMIMBBMS_FLOW_STATE_STARTING);
    if(MBBMS_SERVICE_FUNC_OK != MTVSERVICE_ParseServiceGuide())
    {
        SGParseFailed();
    }
    
    return;
}

/*****************************************************************************/
//  Description : start parse cmmb esg, cmmb mode only
//  Global resource dependence : none
//  Author: xin.li
//  Note: only available for cmmb mode
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartParseESG(void)
{
    uint16  main_service_count = 0;
    uint16  net_id = 0;
    MTVSERVICE_PROGRAM_LIST_T prog_list_t = {0};
    MTVSERVICE_PROGRAM_T      *prog_arry_ptr = PNULL;
    
    main_service_count = GUILIST_GetTotalItemNum(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID);
    //SCI_TRACE_LOW:"MMIMBBMS_StartParseESG main_service_count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7125_112_2_18_2_39_41_316,(uint8*)"d", main_service_count);
    
    if(0 == main_service_count)
    {
        //all service list is null, unable to parse esg
        s_is_init_finished = TRUE;
        MMIMBBMS_RefreshMainInterface();
        
        return;
    }
    
    if(MMIMBBMS_FLOW_STATE_IDLE != MMIMBBMS_GetFlowInfo()->status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartParseESG flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7138_112_2_18_2_39_41_317,(uint8*)"");
        return;
    }
    
    //get cmmb network_id
    MMIMBBMS_GetProgramList(&prog_list_t);
    prog_arry_ptr = prog_list_t.list_ptr;
    net_id = prog_arry_ptr[0].net_id;
    //SCI_TRACE_LOW:"MMIMBBMS_StartParseESG net_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7146_112_2_18_2_39_41_318,(uint8*)"d", net_id);
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_ESG_PARSE, MMIMBBMS_FLOW_STATE_STARTING);
    if(MBBMS_SERVICE_FUNC_OK != MTVSERVICE_ParseESG(net_id))
    {
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
        s_is_init_finished = TRUE;
        MMIMBBMS_RefreshMainInterface();
    }
}

/*****************************************************************************/
//  Description : start active mbbms process
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_StartActiveMBBMS(void)
{
	
    //SCI_TRACE_LOW:"MMIMBBMS_StartActiveMBBMS"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7164_112_2_18_2_39_41_319,(uint8*)"");

    if(MMIMBBMS_FLOW_STATE_IDLE != MMIMBBMS_GetFlowInfo()->status)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartActiveMBBMS flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7168_112_2_18_2_39_41_320,(uint8*)"");
        return;
    }
	
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_MBBMS_ACTIVE, MMIMBBMS_FLOW_STATE_STARTING);
    if(MBBMS_SERVICE_FUNC_OK != MTVSERVICE_ActiveMBBMS())         
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartActiveMBBMS MTVSERVICE_ActiveMBBMS return fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7175_112_2_18_2_39_41_321,(uint8*)"");
        MBBMSActiveFailed();
    }
}

/*****************************************************************************/
//     Description : Set MBBMS Init Flag, if during init process, then set it TRUE
//    Global resource dependence : none
//  Author: lisa
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_SetSGOperationFromType(MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E sg_search_from_type)
{
    s_sg_operation_type = sg_search_from_type;
}

/*****************************************************************************/
//  Description : Init MBBMS when starting mobile TV
//  Global resource dependence : none
//  Author: lisa
//  Note:
/*****************************************************************************/
LOCAL MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E MMIMBBMS_GetSGOperationFromType(void)
{
    return s_sg_operation_type;
}
/*****************************************************************************/
//  Description : Display common error
//  Global resource dependence : none
//  Author: lisa
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayCommonError(MMI_TEXT_ID_T text_id)
{
    MMIPUB_OpenAlertWarningWin(text_id);        
    
    //StartUpWin正常关闭
    MMIMBBMS_SetStartUpWinCloseFlag(FALSE);
    MMK_CloseWin(MMIMBBMS_STARTUP_WIN_ID);

    //close subscrible request win
    MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);
    //close unsubscrible request win
    MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);        
    //close watch connecting win
    MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
    //close sg update request win
    MMK_CloseWin(MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID);
    //close subscription update request win
    MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID);
    
    return;
}

/*****************************************************************************/
// 	Description : Display common tip via text
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayCommonTip(MMI_TEXT_ID_T text_id, MMI_IMAGE_ID_T image_id)
{
    MMIPUB_OpenAlertWinByTextId(PNULL,
        text_id,
        TXT_NULL,
        image_id,
        PNULL,
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL);
}

/*****************************************************************************/
//     Description : the last subscription update status
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMBBMS_GetSubscriptionUpdateStatus(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_GetSubscriptionUpdateStatus = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7249_112_2_18_2_39_41_322,(uint8*)"d",s_is_sub_update_ok);
    return s_is_sub_update_ok;
    
}
/*****************************************************************************/
// 	Description : Is content strat playing
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsContentExpired(anyURI* pIndex)
{
    SCI_DATE_T date_t = {0};
    SCI_TIME_T time_t = {0};
    SG_DATETIME_T nowTime = {0};  
    MBBMSSG_CONTENT_T content_info = {0};
    BOOLEAN ret = FALSE;
    
    if(PNULL == pIndex)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_IsContentExpired pIndex is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7267_112_2_18_2_39_41_323,(uint8*)"");
        return ret;
    }

    if(SG_ERROR_NONE!=MBBMSSG_GetContent(*pIndex, &content_info))
    	   return ret;
    
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsContentExpired content start time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7281_112_2_18_2_39_41_324,(uint8*)"dddddd",content_info.StartTime.years,content_info.StartTime.months,content_info.StartTime.days,content_info.StartTime.hours,content_info.StartTime.minutes,content_info.StartTime.seconds);
    
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsContentExpired content end time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7290_112_2_18_2_39_41_325,(uint8*)"dddddd",content_info.EndTime.years,content_info.EndTime.months,content_info.EndTime.days,content_info.EndTime.hours,content_info.EndTime.minutes,content_info.EndTime.seconds);
    
    TM_GetSysDate(&date_t);
    TM_GetSysTime(&time_t);
    
    nowTime.years = date_t.year;
    nowTime.months= date_t.mon;
    nowTime.days = date_t.mday;
    
    nowTime.hours = time_t.hour;
    nowTime.minutes = time_t.min;
    nowTime.seconds = time_t.sec;
    
    if (MBBMSSG_CompareDateTime(nowTime, content_info.StartTime) < 0 ) 
    {
        //the current time is before content valid period
        ret= FALSE;
    }    
    else if(MBBMSSG_CompareDateTime(nowTime, content_info.EndTime) > 0 ) 
    {
        ret = TRUE;
    }
    else 
    {
        // the current time is during content valid period,then compare hours,minutes and seconds
        if(nowTime.hours > content_info.EndTime.hours)
        {
            ret = TRUE;
        }
        else if (nowTime.hours < content_info.EndTime.hours)
        {
            ret = FALSE;
        }
        else
        {
            if(nowTime.minutes > content_info.EndTime.minutes)
            {
                ret = TRUE;
            }
            else if(nowTime.minutes < content_info.EndTime.minutes)
            {
                ret =FALSE;
            }
            else
            {
                if(nowTime.seconds> content_info.EndTime.seconds)
                {
                    ret = TRUE;
                }
                else if(nowTime.seconds <=content_info.EndTime.seconds)
                {
                    ret = FALSE;
                }
                
            }
            
        }		
        
    }        
    
    MBBMSSG_FreeContent(&content_info);
    //SCI_TRACE_LOW:"MMIMBBMS_IsContentExpired ret =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7351_112_2_18_2_39_42_326,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
// 	Description : Is content strat playing
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
LOCAL MMIMBBMS_CONTENT_TIME_TYPE_E MMIMBBMS_CheckContentPlayTime(anyURI* pIndex)
{
    SCI_DATE_T date_t = {0};
    SCI_TIME_T time_t = {0};
    SG_DATETIME_T nowTime = {0};
    MBBMSSG_CONTENT_T content_info = {0};
    MMIMBBMS_CONTENT_TIME_TYPE_E ret = MMIMBBMS_CONTENT_TIME_NOT_START;
    
    if(PNULL == pIndex)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_CheckContentPlayTime pIndex is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7369_112_2_18_2_39_42_327,(uint8*)"");
        return ret;
    }
    
    if(SG_ERROR_NONE!=MBBMSSG_GetContent(*pIndex, &content_info))
    {
        //SCI_TRACE_LOW:"MMIMBBMS_CheckContentPlayTime SG_ERROR_NONE!=MBBMSSG_GetContent"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7375_112_2_18_2_39_42_328,(uint8*)"");
        return ret;
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_CheckContentPlayTime content start time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7385_112_2_18_2_39_42_329,(uint8*)"dddddd",content_info.StartTime.years,content_info.StartTime.months,content_info.StartTime.days,content_info.StartTime.hours,content_info.StartTime.minutes,content_info.StartTime.seconds);
    
    //SCI_TRACE_LOW:"MMIMBBMS_CheckContentPlayTime content end time year =%d, month=%d,date =%d,hour =%d,min=%d, sec=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7393_112_2_18_2_39_42_330,(uint8*)"dddddd",content_info.EndTime.years,content_info.EndTime.months,content_info.EndTime.days,content_info.EndTime.hours,content_info.EndTime.minutes,content_info.EndTime.seconds);
    TM_GetSysDate(&date_t);
    TM_GetSysTime(&time_t);
    
    nowTime.years = date_t.year;
    nowTime.months= date_t.mon;
    nowTime.days = date_t.mday;
    
    nowTime.hours = time_t.hour;
    nowTime.minutes = time_t.min;
    nowTime.seconds = time_t.sec;
    
    
    if (MBBMSSG_CompareDateTime(nowTime, content_info.StartTime) < 0 ) 
    {
        //节目未开始
        ret = MMIMBBMS_CONTENT_TIME_NOT_START;
    }    
    else if(MBBMSSG_CompareDateTime(nowTime, content_info.EndTime) > 0 ) 
    {
        //节目过期
        ret = MMIMBBMS_CONTENT_TIME_EXPIRED;
    }
    else 
    {
        ret = MMIMBBMS_CONTENT_TIME_OK;
    }    
    
    MBBMSSG_FreeContent(&content_info);
    
    //SCI_TRACE_LOW:"MMIMBBMS_CheckContentPlayTime ret =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7423_112_2_18_2_39_42_331,(uint8*)"d",ret);
    
    return ret;
}


/*****************************************************************************/
// 	Description : close mbbms and release resource
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_Clear(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_Clear enter\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7435_112_2_18_2_39_42_332,(uint8*)"");
    
    MBBMSSG_ClearFilter();
    
    //set mbbms is open to false
    MMIMBBMS_SetMBBMSOpenStatus(FALSE);
    
    //mbbms2.0 exit
    MTVSERVICE_CMMBClose();
    MTVSERVICE_MBMSExit();
    
    //释放资源
    MMIMBBMS_CloseCMMB();
    MMIMBBMS_DeactivePDPContext();
    MMIMBBMS_ClearFavoriteServiceList();
    MMIMBBMS_ClearAlarmContentList();
    
    MMIMBBMS_CloseAllMiddleWin();
    //close main interface win
    MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID);
    //StartUpWin正常关闭
    MMIMBBMS_SetStartUpWinCloseFlag(FALSE);
    MMK_CloseWin(MMIMBBMS_STARTUP_WIN_ID);
    MMK_CloseWin(MMIMBBMS_EXIT_WAIT_WIN_ID);
    
    s_is_mbbms_exiting = FALSE;
    
    //关闭状态栏TV信号显示
    MAIN_SetTVSignalFalse();
    
    //解禁冲突资源
#ifdef UI_P3D_SUPPORT
    MMI_Enable3DEffect(MMI_3D_EFFECT_MTV);
#endif    
    //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_MTV);
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_MTV, TRUE);
    //恢复频率
#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
}

/*****************************************************************************/
// 	Description : sg search failed
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SGSearchFailed(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS SGSearchFailed sg search from type =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7483_112_2_18_2_39_42_333,(uint8*)"d", MMIMBBMS_GetSGOperationFromType());
    
    //s_is_sg_search_ok= FALSE;
    switch(MMIMBBMS_GetSGOperationFromType())
    {
    case MMIMBBMS_SG_SEARCH_PARSE_FROM_MAINMENU:
        //MBBMS2.0规范中要求因为移动信号弱导致更新失败需明确提示 		
        //刷新完成，成功或者失败均返回list列表
        if(MMK_IsOpenWin(MMIMBBMS_UPDATE_FROM_CITY_MAIN_WIN_ID))
        {
            MMK_CloseWin(MMIMBBMS_UPDATE_FROM_CITY_MAIN_WIN_ID);
        }
        if (MMK_IsOpenWin(MMIMBBMS_UPDATE_MAIN_WIN_ID))
        {
            MMK_CloseWin(MMIMBBMS_UPDATE_MAIN_WIN_ID);
        }
        
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_SG_UPDATE_FAILED);
        break;

    default:
        break;
    }
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    MMIMBBMS_SetSGOperationFromType(MMIMBBMS_SG_SEARCH_PARSE_FROM_NONE);
}

/*****************************************************************************/
// 	Description : parse sg ok
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SGParseOK(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS: SGParseOK!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7517_112_2_18_2_39_42_334,(uint8*)"");
    
    if(s_is_mbbms_exiting)
    {
        //SCI_TRACE_LOW:"MMIMBBMS SGParseOK: mbbms exiting"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7521_112_2_18_2_39_42_335,(uint8*)"");
        return;
    }
    
    //set naf addr after sg parsed
    //MMIMBBMS_SetNAFAddr();
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        MMIMBBMS_UpdateWelcomeInfo();
    }
    
    if(MMK_IsOpenWin(MMIMBBMS_SWITCH_WAIT_WIN_ID))
    {
        //if switching to MBBMS mode, need close wait win
        MMK_CloseWin(MMIMBBMS_SWITCH_WAIT_WIN_ID);
    }
    
    //open main win and folw forward
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    if(!MMK_IsOpenWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID))
    {
        MMIMBBMS_EnterIntoMainWindow();
    }
    else
    {
        //while swithing to MBBMS, main_interface_win is already opened
        MMIMBBMS_FreeProgramList();
        MMIMBBMS_FreeServiceIndex();
        MMIMBBMS_RefreshMainInterface();
    }
    
    //mbbms2.0 active mbbms
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        MMIMBBMS_StartActiveMBBMS();
    }
    else
    {
        //cmmb mode, start parse esg
        MMIMBBMS_StartParseESG();
    }
}

/*****************************************************************************/
//  Description :sg parse failed 
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SGParseFailed(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS: SGParseFailed"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7570_112_2_18_2_39_42_336,(uint8*)"");
    
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        MMIMBBMS_UpdateWelcomeInfo();
    }
    
    if(MMK_IsOpenWin(MMIMBBMS_SWITCH_WAIT_WIN_ID))
    {
        //if switching to MBBMS mode, need close wait win
        MMK_CloseWin(MMIMBBMS_SWITCH_WAIT_WIN_ID);
    }
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    if(!MMK_IsOpenWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID))
    {
        MMIMBBMS_EnterIntoMainWindow();
    }
    else
    {
        //while swithing to MBBMS, main_interface_win is already opened
        MMIMBBMS_FreeProgramList();
        MMIMBBMS_FreeServiceIndex();
        MMIMBBMS_RefreshMainInterface();
    }
    
    //mbbms2.0 active mbbms
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        MMIMBBMS_StartActiveMBBMS();
    }
    else
    {
        //cmmb mode, start parse esg
        MMIMBBMS_StartParseESG();
    }
}

/*****************************************************************************/
// Description : handle gba operation failed
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void GBAOperationFailed(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS GBAOperationFailed GBA operation type =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7614_112_2_18_2_39_42_337,(uint8*)"d",MMIMBBMS_GetGBAOperationType());
    
    switch(MMIMBBMS_GetGBAOperationType())
    {
    case MMIMBBMS_GBA_MRK_FROM_SUBSCRIBLE:
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_SUBSCRIBE_FAILED);   
        break;
        
    case MMIMBBMS_GBA_MRK_FROM_UNSUBSCRIBLE:
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_UNSUBSCRIBE_FAILED);  
        break;
        
    case MMIMBBMS_GBA_MRK_FROM_ENG_MODE:
        MMK_CloseWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID);
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_GBA_OPERATION_FAILED);  
        break;
        
    default: 
        //SCI_TRACE_LOW:"MMIMBBMS GBAOperationFailed:GBA operation type error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7632_112_2_18_2_39_42_338,(uint8*)"");
        MMK_CloseWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID);
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_GBA_OPERATION_FAILED);  
        break;
    }    
    
    MMIMBBMS_SetGBAOperationType(MMIMBBMS_GBA_MRK_FROM_NONE);
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE,MMIMBBMS_FLOW_STATE_IDLE);
}

/*****************************************************************************/
// Description : handle gba operation OK
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL void GBAOperationOK(void)
{ 
    //MMIMBBMS_GBA_MRK_FROM_TYPE_E mrk_operation_type = MMIMBBMS_GetGBAOperationType();
    
    //SCI_TRACE_LOW:"MMIMBBMS GBAOperationOK:Operation type =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7650_112_2_18_2_39_42_339,(uint8*)"d",MMIMBBMS_GetGBAOperationType());
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    MMIMBBMS_SetGBAOperationType(MMIMBBMS_GBA_MRK_FROM_NONE); 
	
    if(MMK_IsOpenWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID);
    }
    
    MMIMBBMS_DisplayCommonError(TXT_MBBMS_GBA_OPERATION_OK);
}

/*****************************************************************************/
// 	Description : handle subscription update ok
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SubTableUpateOK(void)
{	
    MMIMBBMS_SUB_UPDATE_FROM_TYPE_E  subupdate_type = MMIMBBMS_GetSubUpdateType();
    
    //SCI_TRACE_LOW:"MMIMBBMS SubTableUpateOK subscription type=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7671_112_2_18_2_39_42_340,(uint8*)"d",subupdate_type);
    
    MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_NONE);   
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    s_is_sub_update_ok = TRUE;
    
    switch(subupdate_type)
    {
    case MMIMBBMS_SUB_UPDATE_FROM_PURCHASE_MANAGE:
        { 
            MMIMBBMS_OpenSubscribeTabMainWin();
            MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
            break;
        }
        
    case MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY:
        {
            MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
            MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_GetObjectType());               
            
            break;
        }
        
    case MMIMBBMS_SUB_UPDATE_FROM_UNCLEAR:
        {
            MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIMBBMS SubTableUpateOK:subscription update type error\n "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7701_112_2_18_2_39_42_341,(uint8*)"");
        break;
    }
    
    //update main window
    MMIMBBMS_FreeProgramList();
    MMIMBBMS_FreeServiceIndex();
    MMIMBBMS_RefreshMainInterface();
}

/*****************************************************************************/
// 	Description : handle subscription update fail
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SubTableUpateFailed(void)
{
    
    //SCI_TRACE_LOW:"MMIMBBMS SubTableUpateFailed from type =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7718_112_2_18_2_39_42_342,(uint8*)"d",MMIMBBMS_GetSubUpdateType());
    
    MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_NONE, MMIMBBMS_FLOW_STATE_IDLE);
    
    s_is_sub_update_ok = FALSE;
    
    switch(MMIMBBMS_GetSubUpdateType())
    {
    case MMIMBBMS_SUB_UPDATE_FROM_PURCHASE_MANAGE:
    case MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY:
    case MMIMBBMS_SUB_UPDATE_FROM_UNCLEAR:
        {         
            MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_NONE);
            MMIMBBMS_DisplayCommonError(TXT_UPDATE_FAIL);
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIMBBMS SubTableUpateOK:subscription update type error\n "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7736_112_2_18_2_39_42_343,(uint8*)"");
        break;
    }
}

/*****************************************************************************/
// 	Description : start account inquir process
//	Global resource dependence : none
//  Author:lisa.lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartAccoutInquiryProcess(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_StartAccoutInqueryProcess"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7747_112_2_18_2_39_42_344,(uint8*)"");
    if(MMIMBBMS_FLOW_STATE_IDLE != MMIMBBMS_GetFlowInfo()->status)
    {
        //流程不允许并发
        //SCI_TRACE_LOW:"MMIMBBMS flow status is not MMIMBBMS_FLOW_STATE_IDLE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7751_112_2_18_2_39_42_345,(uint8*)"");
        return;
    }
    
    if(MMIMBBMS_AccoutInquiry())
    {
        MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_ACCOUNT_INQUERY, MMIMBBMS_FLOW_STATE_STARTING);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_StartAccoutInquiryProcess  MMIMBBMS_AccoutInquiry return fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7761_112_2_18_2_39_42_346,(uint8*)"");
        MMIMBBMS_DisplayCommonError(TXT_MBBMS_ACCOUNT_INQUIRY_FAILED);
        MMK_CloseWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID);
    }
    
//     //set pdp type to account inquery
//     MMIMBBMS_SetPdpActivateType(MMIMBBMS_FLOW_ACCOUNT_INQUERY);
//     if(!MMIMBBMS_ActivePDPContext(MN_DUAL_SYS_1))
//     {
//         MMIMBBMS_HandlePDPActivateFailed();
//     }
    
}

/*****************************************************************************/
// 	Description :Account inquiry
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_AccoutInquiry(void)
{
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_OK;
    
    if(PNULL != s_account_inquiry_request_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AccoutInquiry: s_account_inquiry_request_ptr is  not null\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7785_112_2_18_2_39_42_347,(uint8*)"");
        MBBMS_Free_Account_Query_Ptr(s_account_inquiry_request_ptr);
        s_account_inquiry_request_ptr=PNULL;
    }
    
    s_account_inquiry_request_ptr = (MBBMS_Account_Inquiry_t *)SCI_ALLOC_APP(sizeof(MBBMS_Account_Inquiry_t));
    if(PNULL == s_account_inquiry_request_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AccoutInquiry: s_account_inquiry_request_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7793_112_2_18_2_39_42_348,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET(s_account_inquiry_request_ptr,0,sizeof(MBBMS_Account_Inquiry_t));
    
    s_account_inquiry_request_ptr->request_id_valid = FALSE; 
    s_account_inquiry_request_ptr->request_id = 0;
    s_account_inquiry_request_ptr->inquiry_type = MBBMS_ACCOUNT_PURCHASEITEM;
    
    //SCI_TRACE_LOW:"MMIMBBMS_AccoutInquiry: inquiry type =%d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7803_112_2_18_2_39_42_349,(uint8*)"d", s_account_inquiry_request_ptr->inquiry_type);
    
    service_ret = MTVSERVICE_AccountQuiry(s_account_inquiry_request_ptr);
    //SCI_TRACE_LOW:"MBBMS_Account_Inquiry return = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7806_112_2_18_2_39_42_350,(uint8*)"d", service_ret);
    if(MBBMS_SERVICE_FUNC_OK != service_ret)
    {
        MBBMS_Free_Account_Query_Ptr(s_account_inquiry_request_ptr);
        s_account_inquiry_request_ptr = PNULL;
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : close all middle win
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_CloseAllMiddleWin(void)
{
    
    MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);	 //watch connecting win
    MMK_CloseWin(MMIMBBMS_HELP_INFO_WIN_ID);	//help info wintab
    
    //purchase item management window id
    MMK_CloseWin(MMIMBBMS_PURCHASE_ORDER_WIN_ID);		//purchase order window id
    MMK_CloseWin(MMIMBBMS_PURCHASEITEM_DETAIL_WIN_ID);	//purchase item detail win id
    MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
#if 0    
    MMK_CloseWin(MMIMBBMS_PURCHASEITEM_LIST_WIN_ID);		//purchase item list window id
    MMK_CloseWin(MMIMBBMS_SUBSCRIBE_TYPE_WIN_ID);		//subscribe type window id
#endif	
    
    MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);
    MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);
    MMK_CloseWin(MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID);    
    // search channel
    MMK_CloseWin(MMIMBBMS_KEYWORD_EDIT_WIN_ID);	//
    //	MMK_CloseWin(MMIMBBMS_CHANNEL_LIST_WIN_ID);	//
    MMK_CloseWin(MMIMBBMS_SEARCH_MAIN_WIN_ID);		//
    
    MMK_CloseWin(MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_ID);		//
    MMK_CloseWin(MMIMBBMS_SEARCH_RESULT_WIN_ID);		//
    MMK_CloseWin(MMIMBBMS_SETTING_WIN_ID);		//
    
    // program list
    MMK_CloseWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID);		//
    MMK_CloseWin(MMIMBBMS_PROGRAM_CHILD_WIN_ID);		//
    MMK_CloseWin(MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_ID);		//
    MMK_CloseWin(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID);		//
    MMK_CloseWin(MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_ID);		//
    MMK_CloseWin(MMIMBBMS_SG_DNS_EDIT_WIN_ID);	//
    
    MMK_CloseWin(MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_SEARCH_WAIT_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_POPMENU_WIN_ID);	//pop menu window
    MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_FS_POPMENU_WIN_ID);	//pop menu window
    MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_FC_POPMENU_WIN_ID);	//pop menu window
    
    MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_QUERY_WIN_ID);	  //
    
    MMK_CloseWin(MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_GBA_TEST_REQUEST_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_SWITCH_MBBMS_QUERY_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_MEMFULL_ALERT_WIN_ID);
} 
/*****************************************************************************/
// 	Description : handle gba request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGBATestRequestWinMsg( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case  MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
        MMIMBBMS_StartGBAOperation(MMIMBBMS_GBA_MRK_FROM_ENG_MODE);
        break;
        
    case MSG_APP_CANCEL:	
    //case MSG_APP_RED:
        MMIMBBMS_StopFlow();
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }    
    return (result);
}

/*****************************************************************************/
// 	Description : start gba test
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartGBATest(void)
{
    MMI_STRING_T wait_text ={0};
    
    if(MMIMBBMS_IsMBBMSOpen())
    { 
        /*add for init not finished*/
        if(!MMIMBBMS_DisplayInitWaitingWin())
        {
            return;
        }
        
        SCI_MEMSET(&wait_text,0,sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_MBBMS_GBA_TESTING, &wait_text);
        
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIMBBMS_GBA_TEST_REQUEST_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleGBATestRequestWinMsg);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_MBBMS_GBA_TEST_ALERT);        		
        
    }
}

/*****************************************************************************/
// 	Description : Update sg welcome information
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_UpdateWelcomeInfo(void)
{
    uint32  welcome_str_len = 0;
    uint8*  welcome_str_ptr = PNULL;
    //uint32  i=0;
    
    welcome_str_ptr = MBBMSSG_GetWelcomeInfo(&welcome_str_len);
    //SCI_TRACE_LOW:"MMIMBBMS: MMIMBBMS_UpdateWelcomeInfo welcome_str_ptr = 0x%x, str len =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7947_112_2_18_2_39_43_351,(uint8*)"dd", welcome_str_ptr, welcome_str_len);
    
    if((PNULL != welcome_str_ptr) && (0 != welcome_str_len))
    {
        SCI_MEMSET(g_mbbms_setting_info.sg_welcome_info, 0, M_MMIMBBMS_MAX_WELCOME_INFO_LENS);
        
        welcome_str_len = MIN(welcome_str_len,M_MMIMBBMS_MAX_WELCOME_INFO_LENS);
        
        //save welcome info
        SCI_MEMCPY(g_mbbms_setting_info.sg_welcome_info,welcome_str_ptr,welcome_str_len);
        g_mbbms_setting_info.welcome_info_len = welcome_str_len;
        
        //SCI_TRACE_LOW:"MMIMBBMS_UpdateWelcomeInfo byte1 = 0x%x, byte2 =0x%x ,byte3= 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7960_112_2_18_2_39_43_352,(uint8*)"ddd", g_mbbms_setting_info.sg_welcome_info[0],g_mbbms_setting_info.sg_welcome_info[1],g_mbbms_setting_info.sg_welcome_info[2]);
    } 
    
    if(welcome_str_ptr != PNULL)
    {
        SCI_Free(welcome_str_ptr);
    }
}

/*****************************************************************************/
//     Description : the init process(sg search,sg parse,subscription update,gba,mrk) is finished
//    Global resource dependence : none
//  Author: lisa.lin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMBBMS_GetInitStatus(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_GetInitStatus = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7975_112_2_18_2_39_43_353,(uint8*)"d",s_is_init_finished);
    return s_is_init_finished;
}

/*****************************************************************************/
//  Description : set init status flag
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetInitStatus(BOOLEAN is_finish)
{
    s_is_init_finished = is_finish;
}

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: plum.peng
// Note:
/*****************************************************************************/
PUBLIC uint16  MMIMBBMS_GetRemindTime(uint32 index)
{
    if (index >= MMIMBBMS_REMAINDER_MAX)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetRemindTime index error =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_7996_112_2_18_2_39_43_354,(uint8*)"d", index);
        return 0;
    }
    
    return reminder_time[index];
}

/*****************************************************************************/
// Description : MMIMBBMS_GetSettingInfo
// Global resource dependence : none
// Author: plum.peng
// Note:
/*****************************************************************************/
PUBLIC const MMIMBBMS_SETTING_INFO_T*  MMIMBBMS_GetConstSettingInfo(void)
{
    return &g_mbbms_setting_info;
}


/*****************************************************************************/
// 	Description : handle exit wait win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleExitWaitWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case  MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_NULL, TRUE);	
        break;
        
    case MSG_APP_CANCEL:	
    case MSG_APP_RED:
        
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }    
    return (result);
}

/*****************************************************************************/
// 	Description : MMIMBBMS_GetPlayInfo
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC  MMIMBBMS_PLAY_INFO*  MMIMBBMS_GetPlayInfo(void)
{
    return &s_mbbms_play_info;
}

/*****************************************************************************/
// 	Description : set need play next flag
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SetNeedPlayNextFlag(BOOLEAN is_need_play_next)
{
    s_mbbms_play_info.is_need_play_next = is_need_play_next;
}

/*****************************************************************************/
// 	Description : update CMMB play status
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SetCmmbPlayStatus(MMIMTV_PLAY_STATUS_TYPE_E play_status)
{
    s_mbbms_play_info.cmmb_play_status = play_status;
}

/*****************************************************************************/
// 	Description : reset current service info, service_id etc.
//	Global resource dependence : none
//  Author: xin.li
//	Note: 重置当前service信息
/*****************************************************************************/
LOCAL void ResetCurServiceInfo(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS ResetCurServiceInfo  cur service_id =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8076_112_2_18_2_39_43_355,(uint8*)"d", s_mbbms_play_info.service_id);
    
    //将当前service_id重置为INVALID
    s_mbbms_play_info.service_id = MMIMBBMS_INVALID_SERVICE_ID;
}

/*****************************************************************************/
// 	Description : MMIMBBMS_GetPlayInfo
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_PreExitCallback(void)
{
    MMI_STRING_T  wait_text = {0};

    //SCI_TRACE_LOW:"MMIMBBMS_PreExitCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8090_112_2_18_2_39_43_356,(uint8*)"");
    
    SCI_MEMSET(&wait_text,0,sizeof(MMI_STRING_T));
    MMI_GetLabelTextByLang(TXT_MBBMS_EXIT_WAITING, &wait_text);
    
    //service working and open wait win
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIMBBMS_EXIT_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleExitWaitWinMsg);
}

/*****************************************************************************/
// 	Description : whether current service is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsServiceAudioTyle(anyURI id)
{
    BOOLEAN     is_audio_type = FALSE;
    SG_ERROR_E  error_code  = SG_ERROR_NONE;
    MBBMSSG_SERVICE_T       service_node = {0};
    MBBMS_SERVICE_TYPE_E    service_type = 0;
    
    error_code = MBBMSSG_GetService(id, &service_node);
    if(SG_ERROR_NONE == error_code)
    {
        if(PNULL != service_node.servicetypeList
            && PNULL != service_node.servicetypeList->next_ptr
            && PNULL != service_node.servicetypeList->next_ptr->data)
        {
            service_type = (MBBMS_SERVICE_TYPE_E)(*(uint8*)service_node.servicetypeList->next_ptr->data);
            //SCI_TRACE_LOW:"MMIMBBMS_IsServiceAudioTyle servicetype = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8119_112_2_18_2_39_43_357,(uint8*)"d", service_type);
            if(MBBMS_SERVICE_TYPE_AUDIO == service_type)
            {
                is_audio_type = TRUE;
            }
        }
    }
    MBBMSSG_FreeService(&service_node);
    
    s_mbbms_play_info.is_audio_type = is_audio_type;
    return is_audio_type;
}

/*****************************************************************************/
// 	Description : whether current content is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsContentAudioTyle(anyURI id)
{
    BOOLEAN     is_audio_type = FALSE;
    SG_ERROR_E  error_code  = SG_ERROR_NONE;
    MBBMSSG_CONTENT_T content = {0};
    
    error_code = MBBMSSG_GetContent(id, &content);
	if(SG_ERROR_NONE == error_code)
	{
        //SCI_TRACE_LOW:"MMIMBBMS_IsCurrentContentAudioTyle"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8145_112_2_18_2_39_43_358,(uint8*)"");
        is_audio_type = MMIMBBMS_IsServiceAudioTyle(content.serviceIdRef);
	}
    MBBMSSG_FreeContent(&content);

    s_mbbms_play_info.is_audio_type = is_audio_type;
    return is_audio_type;
}

/*****************************************************************************/
// 	Description : whether current cmmb service is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCmmbServiceAudioTyle(uint32 index)
{
    BOOLEAN is_audio_type = FALSE;
    MTVSERVICE_PROGRAM_LIST_T prog_list_t = {0};
    MTVSERVICE_PROGRAM_T      *prog_arry_ptr = PNULL;
    MTV_STIMI_SERVICE_CLASS_E service_type = MTV_STIMI_SERVICE_CLASEE_TV;
    
    MMIMBBMS_GetProgramList(&prog_list_t);
    prog_arry_ptr = prog_list_t.list_ptr;
    
    if(index < prog_list_t.total_count 
        && PNULL != prog_arry_ptr)
    {
        if(PNULL != prog_arry_ptr[index].servicetype_ptr)
        {
            service_type = (MTV_STIMI_SERVICE_CLASS_E)*(prog_arry_ptr[index].servicetype_ptr);
            if(MTV_STIMI_SERVICE_CLASEE_AUDIO_BC == service_type)
            {
                is_audio_type = TRUE;
            }
        }
    }
    
    return is_audio_type;
}

/*****************************************************************************/
// 	Description : whether mbbms current playing service is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsCurrentServiceAudioTyle(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_IsCurrentServiceAudioTyle : %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8190_112_2_18_2_39_43_359,(uint8*)"d", s_mbbms_play_info.is_audio_type);
    return s_mbbms_play_info.is_audio_type;
}

/*****************************************************************************/
// Description : whether permit enter cmmb play process
// Global resource dependence : 
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsPermitCMMBPlay(void)
{
    BOOLEAN is_permit = FALSE;
    
    //MBBMS所属的主要窗口不在Focus, 则认为当前App是非Focus状态的
    if(MMK_IsFocusWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_CONTENT_TIME_ALERT_WIN_ID)
        || MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_PROGRAM_CHILD_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_SEARCH_RESULT_WIN_ID)
        || MMK_IsFocusWin(MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_ID))
    {
        is_permit = TRUE;
    }

    //SCI_TRACE_LOW:"MMIMBBMS_IsMBBMSAppFocous is_focus = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8219_112_2_18_2_39_43_360,(uint8*)"d", is_permit);
    return is_permit;
}

//mbbms2.0 start
/*****************************************************************************/
// Description : get current service mode
// Global resource dependence : 
// Author: xin.li
// Note: 两种模式MBBMS_SERVICE_MODE_CMMB/MBBMS_SERVICE_MODE_MBBMS
/*****************************************************************************/
PUBLIC MTV_SERVICE_MODE_E MMIMBBMS_GetServiceMode(void)
{
    return s_service_mode;
}

/*****************************************************************************/
// Description : set current service mode
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetServiceMode(MTV_SERVICE_MODE_E mode)
{
    //SCI_TRACE_LOW:"MMIMBBMS_SetServiceMode mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8239_112_2_18_2_39_43_361,(uint8*)"d", mode);
    
    s_service_mode = mode;
}

/*****************************************************************************/
// Description : MTVSERVICE_CALLBACK实现
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_ServiceCallback(MBBMS_SERVICE_INFO_T *service_info_ptr)
{
    uint16  sig_code = service_info_ptr->type;
    
    
    //快速停止播放ACK
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        //MBBMS模式下subtype为MBBMS_SERVICE_STOP_CMMB
        if(MBBMS_SERVICE_STOP == service_info_ptr->type
            && MBBMS_SERVICE_STOP_CMMB == service_info_ptr->subtype)
        {
            if(s_mbbms_play_info.is_quick_stop_play)
            {
                s_mbbms_play_info.is_received_quick_ack = TRUE;
            }
        }
    }
    else
    {
        //CMMB模式下直接停播放，无MBBMS_SERVICE_STOP_CMMB消息
        if(MBBMS_SERVICE_STOP == service_info_ptr->type
            && MBBMS_SERVICE_STOP == service_info_ptr->subtype)
        {
            if(s_mbbms_play_info.is_quick_stop_play)
            {
                s_mbbms_play_info.is_received_quick_ack = TRUE;
            }
        }
    }
    
    SendSignalToTask(sig_code, (void*)service_info_ptr, P_APP);
}

/*****************************************************************************/
// Description : save MTVSERVICE pdp callback
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void SaveServicePDPCallback(MTVSERVICE_PDPCALLBACK service_pdp_callback)
{
    s_mtvservice_pdp_callback = service_pdp_callback;
}

/*****************************************************************************/
// Description : get saved MTVSERVICE pdp callback
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL MTVSERVICE_PDPCALLBACK GetServicePDPCallback(void)
{
    return s_mtvservice_pdp_callback;
}

/*****************************************************************************/
// 	Description : send signal to task
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SendSignalToTask(uint16 sig_code, void *signal_data_ptr, uint32 task_id)
{
    MMIMBBMS_SERVICE_SIG_T *sig_to_mmi_ptr = PNULL;
    uint32 result = 0;
    
    //SCI_TRACE_LOW("[MMIMBBMS]: SendSignalToTask task id=%d,sig_code=%d\n ",task_id,sig_code);
    if(task_id == P_APP )
    {
        sig_to_mmi_ptr = (MMIMBBMS_SERVICE_SIG_T*)SCI_ALLOC(sizeof(MMIMBBMS_SERVICE_SIG_T));
        if(PNULL == sig_to_mmi_ptr)
        {
            //SCI_TRACE_LOW:"MMIMBBMS SendSignalToTask sig_to_mmi_ptr alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8315_112_2_18_2_39_43_362,(uint8*)"");
            return;
        }
        
        sig_to_mmi_ptr->SignalCode = sig_code;
       	sig_to_mmi_ptr->SignalSize = sizeof(MMIMBBMS_SERVICE_SIG_T);
       	sig_to_mmi_ptr->Sender = SCI_IdentifyThread();
       	sig_to_mmi_ptr->data = *(MBBMS_SERVICE_INFO_T *)signal_data_ptr;
       	result = SCI_SendSignal((xSignalHeader)sig_to_mmi_ptr, task_id);
    } 
}

/*****************************************************************************/
// Description : 逐个获取已订购套餐信息并显示
// Global resource dependence :
// Author: 
// Note:
//should check index
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayAllPurchasedItem(MMI_CTRL_ID_T ctrl_id, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr)
{
    int index=0;
    int purchase_num=0;
    MBBMS_Purchase_Table_Info_t* purchase_table_info_ptr = PNULL;
    MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T  purchase_display_info={0};
    uint8 *purchase_name_ptr =PNULL; //purchase name,UTF8 format
    MBBMSSG_PURCHASE_DETAIL_T* purchase_detail_ptr = PNULL;
    MBBMSSG_PURCHASE_ITEM_T* purchase_item_ptr = PNULL;
    SG_MONETARY_PRICE_T*            price_info_ptr = PNULL;
    MBBMSSG_PURCHASE_ITEM_T item_info = {0};//查询返回purchaseitem为空时需要通过SG接口获取iteminfo
    MBBMSSG_PURCHASE_DETAIL_T purDetailItem = {0};//查询返回purchaseitem为空时需要通过SG接口获取iteminfo
    anyURI   global_purchase_item_Id = {0};//套餐标识
    anyURI   itemId = {0};//套餐分片标识
    uint32 string_len = 0;
    uint32 anyURI_len = 0;
    SG_ERROR_E purchase_item_error_code = SG_ERROR_NONE;
    SG_ERROR_E purchase_detail_error_code = SG_ERROR_NONE;
    
    if(PNULL == account_inquiry_res_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchasedItem  --  account_inquiry_res_ptr = NULL.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8353_112_2_18_2_39_44_363,(uint8*)"");
        return;	  
    }

    purchase_num = MMIMBBMS_GetAllPurchasedItemNum(account_inquiry_res_ptr);
    if(0==purchase_num)
    {
        //套餐列表为空
        //SCI_TRACE_LOW:"---MMIMBBMS_DisplayAllPurchasedItem no purchaseitem\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8361_112_2_18_2_39_44_364,(uint8*)"");
        return ;
    }            
    
    GUILIST_SetMaxItem(ctrl_id, purchase_num, FALSE);
    
    //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchasedItem total purchase num=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8367_112_2_18_2_39_44_365,(uint8*)"d",purchase_num);
    
    //Get every purchase item 
    for(index=0; index<purchase_num; index++)
    {
        purchase_table_info_ptr = MMIMBBMS_GetPurchasedItemByIndex(index, account_inquiry_res_ptr);
        if(PNULL == purchase_table_info_ptr)
        {
            //套餐信息表结点为空
            //SCI_TRACE_LOW:"---MMIMBBMS_DisplayAllPurchasedItem purchase_table_info_ptr = PNULL\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8376_112_2_18_2_39_44_366,(uint8*)"");
            continue;
        }
        
        purchase_item_ptr = (MBBMSSG_PURCHASE_ITEM_T*)STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_item, 0);     
        purchase_detail_ptr = (MBBMSSG_PURCHASE_DETAIL_T*)STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_data, 0);    
        
        //如果查询返回信息中套餐分片信息或者套餐详细信息中一项为空，则从SG中查询	
        if(PNULL == purchase_item_ptr || PNULL == purchase_detail_ptr)
        {
            anyURI_len = 0;
            
            SCI_MEMSET(&global_purchase_item_Id, 0, sizeof(anyURI));
            global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];	     
            while(0 != global_purchase_item_Id.anyURI_arr[anyURI_len] && anyURI_len < (SG_MAX_URI_LEN - 1))
            {
                anyURI_len++;
                global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];
            }
	     //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchasedItem anyURI_len = %d\n"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8395_112_2_18_2_39_44_367,(uint8*)"d", anyURI_len);
            if(0 == anyURI_len)
            {
                //获取到的套餐标识为空
                continue;
            }
            global_purchase_item_Id.anyURI_ptr = PNULL;
            global_purchase_item_Id.string_len = anyURI_len;
            MBBMSSG_GetPurchaseItemId(global_purchase_item_Id, &itemId);
			
            purchase_item_error_code = MBBMSSG_GetPurchaseItem(itemId, &item_info);
            purchase_detail_error_code= MBBMSSG_GetPurchaseDetail(itemId, &purDetailItem);
            if(SG_ERROR_NONE != purchase_item_error_code || SG_ERROR_NONE != purchase_detail_error_code)	
            {
                MBBMSSG_FreePurchaseItem(&item_info);
                MBBMSSG_FreeTypeAnyURI(&itemId);
                MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);
                //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchasedItem purchase_item_error_code =%d, purchase_detail_error_code =%dn"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8413_112_2_18_2_39_44_368,(uint8*)"dd",purchase_item_error_code,purchase_detail_error_code);
                continue;            
            }
            purchase_item_ptr = &item_info;	
            purchase_detail_ptr = &purDetailItem;	 
        }
        SCI_MEMSET(&purchase_display_info,0,sizeof(MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T));
        //get purchase name of every purchase                       
        MMIMBBMS_GetNameFromNameList(purchase_item_ptr->NameList, s_lang_type, &purchase_name_ptr, &string_len);
		
        if(PNULL==purchase_name_ptr)
        {
            MBBMSSG_FreePurchaseItem(&item_info);
            MBBMSSG_FreeTypeAnyURI(&itemId);
            MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);
            //SCI_TRACE_LOW:"MMIMBBMS_DisplayAllPurchasedItem purchase name is null\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8428_112_2_18_2_39_44_369,(uint8*)"");
            continue;
        }
        SCI_MEMSET(purchase_display_info.purchase_name, 0, sizeof(purchase_display_info.purchase_name));
        
        string_len = MIN(string_len,MMIMBBMS_PURCHASE_NAME_MAX_LEN);
        
        SCI_MEMCPY(purchase_display_info.purchase_name, purchase_name_ptr, string_len);
        
        
        //get purchase order status
        purchase_display_info.is_ordered= TRUE;
        
        //get purchase cost info
        purchase_display_info.scription_type = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail_ptr->PriceInfo.subscriptionType;
        if(PNULL != purchase_detail_ptr->PriceInfo.MonetaryPriceList)  
        {
            price_info_ptr = MMIMBBMS_GetPriceInfoFromPriceList(purchase_detail_ptr->PriceInfo.MonetaryPriceList, 0);
            if(PNULL!=price_info_ptr)
            {
                purchase_display_info.price = price_info_ptr->price;
            }
        }
        //get purchase period
        purchase_display_info.period = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail_ptr->PriceInfo.period;
        
        MMIMBBMS_AddPurchaseItemToListboxCtrl(ctrl_id, &purchase_display_info);
        MBBMSSG_FreePurchaseItem(&item_info);
        MBBMSSG_FreeTypeAnyURI(&itemId);
        MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);		
        
    }
}

/*****************************************************************************/
//     Description : to display all services or contents in purchased item to richtext
//    Global resource dependence : none
//  Author: 
//    Note: 显示套餐详情，（套餐名、包含的所有频道或节目、订购时间、资费）
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayServiceOrContentInPurchasedItem(MMI_CTRL_ID_T ctrl_id, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr)
{
    uint32 purchased_item_index = 0;
    MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T  purchase_display_info={0};
    uint8 *purchase_name_ptr =PNULL; //purchase name,UTF8 format
    uint8 *purchase_description_ptr =PNULL; //purchase name,UTF8 format    
    MBBMSSG_PURCHASE_DETAIL_T* purchase_detail_ptr = PNULL;
    MBBMSSG_PURCHASE_ITEM_T* purchase_item_ptr = PNULL;
    SG_MONETARY_PRICE_T*            price_info_ptr = PNULL;
    wchar  wchar_name[MMIMBBMS_PURCHASE_NAME_MAX_LEN+1] = {0};
    uint32 string_len = 0;
    uint32 description_str_len = 0;
    MBBMS_Purchase_Table_Info_t* purchase_table_info_ptr = PNULL;	
    MBBMSSG_PURCHASE_ITEM_T item_info = {0};//查询返回purchaseitem为空时需要通过SG接口获取iteminfo
    MBBMSSG_PURCHASE_DETAIL_T purDetailItem = {0};//查询返回purchaseitem为空时需要通过SG接口获取iteminfo
    anyURI   global_purchase_item_Id = {0};//套餐标识
    anyURI   itemId = {0};//套餐分片标识    
    uint32 anyURI_len = 0;
    SG_ERROR_E purchase_item_error_code = SG_ERROR_NONE;
    SG_ERROR_E purchase_detail_error_code = SG_ERROR_NONE;
    MMI_STRING_T mbbms_string_description = {0};   
    MMI_STRING_T mbbms_string_description1 = {0};
    MMI_STRING_T mbbms_string_description2 = {0};    
    wchar temp_wchar_des[GUILIST_STRING_MAX_NUM+1] = {0};
    uint8 temp_char_des[GUILIST_STRING_MAX_NUM+1] = {0};
    wchar temp_wchar_des2[GUILIST_STRING_MAX_NUM+1] = {0};
	


    if(PNULL == account_inquiry_res_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem  --  account_inquiry_res_ptr = NULL.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8497_112_2_18_2_39_44_370,(uint8*)"");
        return;	  
    }

	  	
    //get current purchase index
    purchased_item_index=MMIMBBMS_GetCurrPurchaseIndex();    
    //get purchase index node

    purchase_table_info_ptr = MMIMBBMS_GetPurchasedItemByIndex(purchased_item_index, account_inquiry_res_ptr);
    if(PNULL == purchase_table_info_ptr)
    {
        //套餐信息表结点为空
        //SCI_TRACE_LOW:"---MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_table_info_ptr = PNULL\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8510_112_2_18_2_39_44_371,(uint8*)"");
        return;
    }

    //purchase_item_ptr = (MBBMSSG_PURCHASE_ITEM_T*)purchase_table_info_ptr->purchase_item;
    //purchase_detail_ptr = (MBBMSSG_PURCHASE_DETAIL_T*)purchase_table_info_ptr->purchase_data;     
	
	
    //if(PNULL == purchase_detail_ptr || PNULL == purchase_detail_ptr)
    {
        anyURI_len = 0;
        SCI_MEMSET(&global_purchase_item_Id, 0, sizeof(anyURI));
        global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];	     
        while(0 != global_purchase_item_Id.anyURI_arr[anyURI_len] && anyURI_len < (SG_MAX_URI_LEN - 1))
        {
            anyURI_len++;
            global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];
        }
        if(0 == anyURI_len)
        {
            //获取到的套餐标识为空
            return;
        }
        global_purchase_item_Id.anyURI_ptr = PNULL;
        global_purchase_item_Id.string_len = anyURI_len;
        MBBMSSG_GetPurchaseItemId(global_purchase_item_Id, &itemId);
			
        purchase_item_error_code = MBBMSSG_GetPurchaseItem(itemId, &item_info);
        purchase_detail_error_code= MBBMSSG_GetPurchaseDetail(itemId, &purDetailItem);
        if(SG_ERROR_NONE != purchase_item_error_code || SG_ERROR_NONE != purchase_detail_error_code)	
        {
            MBBMSSG_FreePurchaseItem(&item_info);
            MBBMSSG_FreeTypeAnyURI(&itemId);
            MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);
            //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_item_error_code =%d, purchase_detail_error_code =%dn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8545_112_2_18_2_39_44_372,(uint8*)"dd",purchase_item_error_code,purchase_detail_error_code);
            return;            
        }
        purchase_item_ptr = &item_info;	
        purchase_detail_ptr = &purDetailItem;	 
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase_item_ptr=0x%x, purchase_detail_ptr=0x%x.n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8553_112_2_18_2_39_44_373,(uint8*)"dd",purchase_item_ptr,purchase_detail_ptr);
    if(PNULL == purchase_item_ptr || PNULL == purchase_detail_ptr)
    {
        purchase_item_ptr = (MBBMSSG_PURCHASE_ITEM_T*)
            STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_item, 0);     
        purchase_detail_ptr = (MBBMSSG_PURCHASE_DETAIL_T*)
            STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_data, 0);       
    }
    if(PNULL == purchase_item_ptr || PNULL == purchase_detail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem null in local and account inquiry return.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8563_112_2_18_2_39_44_374,(uint8*)"");
        return;
    }
    //get purchase item
    if(PNULL == purchase_item_ptr->NameList)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem  --  purchase_item_ptr->NameList = NULL.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8569_112_2_18_2_39_44_375,(uint8*)"");
        return;		
    }
    MMIMBBMS_GetNameWchar(purchase_item_ptr->NameList, wchar_name, 
        sizeof(wchar_name) - 1);
        
    MMIMBBMS_AddPurchaseDetailHead(wchar_name, ctrl_id);
        
    //to show service ref list            
    //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem ServiceIdRefList =0x%x, len=%dn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8579_112_2_18_2_39_44_376,(uint8*)"dd", purchase_item_ptr->ServiceIdRefList,STL_LinkListGetNodeDataCount(purchase_item_ptr->ServiceIdRefList));
    if((PNULL != purchase_item_ptr->ServiceIdRefList)
        &&(STL_LinkListGetNodeDataCount(purchase_item_ptr->ServiceIdRefList) > 0))
    {
        MMIMBBMS_DisplayServiceInPurchase(purchase_item_ptr->ServiceIdRefList, ctrl_id);
    }
    //to show purchase description
    if(PNULL != purchase_item_ptr->DescriptionList)
    {
        MMIMBBMS_GetNameFromNameList(purchase_item_ptr->DescriptionList, s_lang_type, 
            &purchase_description_ptr, &description_str_len);
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase description description_str_len =%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8590_112_2_18_2_39_44_377,(uint8*)"d", description_str_len);
        if(PNULL != purchase_description_ptr && 0 != description_str_len)
        {
           
           MMI_GetLabelTextByLang(TXT_MBBMS_PURCHASE_DESCRIPTION, &mbbms_string_description1);
           //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description1.wstr_len =%d\n"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8595_112_2_18_2_39_44_378,(uint8*)"d", mbbms_string_description1.wstr_len);
            mbbms_string_description2.wstr_len = GUI_UTF8ToWstr(temp_wchar_des,
                                                                    (uint16)GUILIST_STRING_MAX_NUM,
                                                                    purchase_description_ptr, 
                                                                    (uint16)description_str_len);

           mbbms_string_description2.wstr_ptr = temp_wchar_des;
           //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description2.wstr_len =%d\n"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8602_112_2_18_2_39_44_379,(uint8*)"d", mbbms_string_description2.wstr_len);

           memset(temp_wchar_des2, 0, sizeof(temp_wchar_des2));
           mbbms_string_description.wstr_ptr = temp_wchar_des2;
           MMIAPICOM_MergeTwoString(&mbbms_string_description1, &mbbms_string_description2, &mbbms_string_description, GUILIST_STRING_MAX_NUM);
           //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase mbbms_string_description.wstr_len =%d\n"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8607_112_2_18_2_39_44_380,(uint8*)"d", mbbms_string_description.wstr_len);

           description_str_len = GUI_WstrToUTF8(temp_char_des,
                                            (uint16)GUILIST_STRING_MAX_NUM,
                                            mbbms_string_description.wstr_ptr, 
                                            (uint16)mbbms_string_description.wstr_len);     
           MMIMBBMS_AddPurchaseContentToRchtextCtrl(ctrl_id, temp_char_des, description_str_len);
        }
    }
        
    //to show content ref list
    if((PNULL != purchase_item_ptr->ContentIdRefList)
        &&(STL_LinkListGetNodeDataCount(purchase_item_ptr->ContentIdRefList) > 0))
    {
        MMIMBBMS_DisplayContentInPurchase(purchase_item_ptr->ContentIdRefList, ctrl_id);
    }
		
    SCI_MEMSET(&purchase_display_info,0,sizeof(MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T));
    //get purchase name of every purchase                       
    MMIMBBMS_GetNameFromNameList(purchase_item_ptr->NameList, s_lang_type, &purchase_name_ptr, &string_len);
		
    SCI_MEMSET(purchase_display_info.purchase_name, 0, sizeof(purchase_display_info.purchase_name));
        
    string_len = MIN(string_len,MMIMBBMS_PURCHASE_NAME_MAX_LEN);

    if(PNULL==purchase_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DisplayServiceOrContentInPurchasedItem purchase name is null\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8634_112_2_18_2_39_44_381,(uint8*)"");
    }
    else
    {
        SCI_MEMCPY(purchase_display_info.purchase_name, purchase_name_ptr, string_len);
    }
        

    //get purchase order status
    purchase_display_info.is_ordered= TRUE;
        
    //get purchase cost info		
    purchase_display_info.scription_type = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail_ptr->PriceInfo.subscriptionType;
		
    if(PNULL != purchase_detail_ptr->PriceInfo.MonetaryPriceList)  
    {
        price_info_ptr = MMIMBBMS_GetPriceInfoFromPriceList(purchase_detail_ptr->PriceInfo.MonetaryPriceList, 0);
        if(PNULL!=price_info_ptr)
        {
            purchase_display_info.price = price_info_ptr->price;
        }
    }
    purchase_display_info.period = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail_ptr->PriceInfo.period;
        
    MMIMBBMS_AddPurchaseDetailTail(&purchase_display_info, ctrl_id);
	
    MBBMSSG_FreePurchaseItem(&item_info);
    MBBMSSG_FreeTypeAnyURI(&itemId);
    MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);	

}

/*****************************************************************************/
// Description :获取套餐查询返回的数据指针
// Global resource dependence :
// Author: 
// Note:
//should check index
/*****************************************************************************/
PUBLIC MBBMS_Account_Inquiry_Res_t* MMIMBBMS_GetAccountInquiryRes(void)
{
    return s_account_inquiry_res_ptr;
}

/*****************************************************************************/
// Description : 释放套餐查询返回的结构体指针
// Global resource dependence :
// Author: 
// Note:
//should check index
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeAccountInquiryRes(void)
{
    if(PNULL != s_account_inquiry_res_ptr)
    {
        MBBMS_Free_Account_Query_Res_Ptr(s_account_inquiry_res_ptr);
        s_account_inquiry_res_ptr = PNULL;
    }
}

/*****************************************************************************/
//     Description : set current purchase display info
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurrPurchasedItemDisplayInfo(uint32 index, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr)
{
    //SG_LIST_PTR_T purchasedItem_list_head_ptr = PNULL;
    //SG_LIST_PTR_T purchasedDetail_list_head_ptr = PNULL;
    uint8 *purchase_name_ptr =PNULL; //purchase name,UTF8 format
    MBBMSSG_PURCHASE_DETAIL_T* purchase_detail_ptr = PNULL;
    MBBMSSG_PURCHASE_ITEM_T* purchase_item_ptr = PNULL;
    SG_MONETARY_PRICE_T*            price_info_ptr = PNULL;
    uint32 string_len = 0;
    MBBMS_Purchase_Table_Info_t* purchase_table_info_ptr = PNULL;	
    MBBMSSG_PURCHASE_ITEM_T item_info = {0};//查询返回purchaseitem为空时需要通过SG接口获取iteminfo
    MBBMSSG_PURCHASE_DETAIL_T purDetailItem = {0};//查询返回purchaseitem为空时需要通过SG接口获取iteminfo
    anyURI   global_purchase_item_Id = {0};//套餐标识
    anyURI   itemId = {0};//套餐分片标识    
    uint32 anyURI_len = 0;
    SG_ERROR_E purchase_item_error_code = SG_ERROR_NONE;
    SG_ERROR_E purchase_detail_error_code = SG_ERROR_NONE;
    


    if(PNULL == account_inquiry_res_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetCurrPurchasedItemDisplayInfo  --  account_inquiry_res_ptr = NULL.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8716_112_2_18_2_39_45_382,(uint8*)"");
        return;	  
    }
    
    SCI_MEMSET(&s_curr_operation_purchase,0,sizeof(MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T));
    SCI_MEMSET(s_curr_operation_purchase.purchase_name,0,sizeof(s_curr_operation_purchase.purchase_name));
	
    s_curr_operation_purchase.index = index;    
    s_curr_operation_purchase.currency=s_currency_type;
	

    purchase_table_info_ptr = MMIMBBMS_GetPurchasedItemByIndex(index, account_inquiry_res_ptr);
    if(PNULL == purchase_table_info_ptr)
    {
        //套餐信息表结点为空
        //SCI_TRACE_LOW:"---MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_table_info_ptr = PNULL\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8731_112_2_18_2_39_45_383,(uint8*)"");
        return;
    }
    
    purchase_item_ptr = (MBBMSSG_PURCHASE_ITEM_T*)STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_item, 0); 
    purchase_detail_ptr = (MBBMSSG_PURCHASE_DETAIL_T*)STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_data, 0);
    
    //SCI_TRACE_LOW:"MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_item_ptr=0x%x, purchase_detail_ptr=0x%x.n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8739_112_2_18_2_39_45_384,(uint8*)"dd",purchase_item_ptr,purchase_detail_ptr);
    
    if(PNULL == purchase_item_ptr || PNULL == purchase_detail_ptr)
    {
        anyURI_len = 0;
        SCI_MEMSET(&global_purchase_item_Id, 0, sizeof(anyURI));
        global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];	     
        while(0 != global_purchase_item_Id.anyURI_arr[anyURI_len] && anyURI_len < (SG_MAX_URI_LEN - 1))
        {
            anyURI_len++;
            global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];
        }
        if(0 == anyURI_len)
        {
            //获取到的套餐标识为空
            return;
        }
        global_purchase_item_Id.anyURI_ptr = PNULL;
        global_purchase_item_Id.string_len = anyURI_len;
        MBBMSSG_GetPurchaseItemId(global_purchase_item_Id, &itemId);
			
        purchase_item_error_code = MBBMSSG_GetPurchaseItem(itemId, &item_info);
        purchase_detail_error_code= MBBMSSG_GetPurchaseDetail(itemId, &purDetailItem);
        if(SG_ERROR_NONE != purchase_item_error_code || SG_ERROR_NONE != purchase_detail_error_code)	
        {
            MBBMSSG_FreePurchaseItem(&item_info);
            MBBMSSG_FreeTypeAnyURI(&itemId);
            MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);
            //SCI_TRACE_LOW:"MMIMBBMS_SetCurrPurchasedItemDisplayInfo purchase_item_error_code =%d, purchase_detail_error_code =%dn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8768_112_2_18_2_39_45_385,(uint8*)"dd",purchase_item_error_code,purchase_detail_error_code);
            return;            
        }
        purchase_item_ptr = &item_info;	
        purchase_detail_ptr = &purDetailItem;	 
    }

    MMIMBBMS_GetNameFromNameList(purchase_item_ptr->NameList, s_lang_type, &purchase_name_ptr, &string_len);
		
    if(PNULL != purchase_name_ptr)
    {
        string_len = MIN(string_len,MMIMBBMS_PURCHASE_NAME_MAX_LEN);
        SCI_MEMCPY(s_curr_operation_purchase.purchase_name,purchase_name_ptr, string_len);
    }
      
    //get purchase order status
    s_curr_operation_purchase.is_ordered= TRUE;
        
    //get purchase cost info
     
    s_curr_operation_purchase.scription_type = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail_ptr->PriceInfo.subscriptionType;
    s_curr_operation_purchase.period = (MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E)purchase_detail_ptr->PriceInfo.period;
    if(PNULL != purchase_detail_ptr->PriceInfo.MonetaryPriceList)  
    {
        price_info_ptr = MMIMBBMS_GetPriceInfoFromPriceList(purchase_detail_ptr->PriceInfo.MonetaryPriceList, 0);
        if(PNULL!=price_info_ptr)
        {
            s_curr_operation_purchase.price = price_info_ptr->price;
        }
    }
    
}


/*****************************************************************************/
// 	Description : get text id by returned error code
//	Global resource dependence : none
//  Author: xin.li
//	Note: 根据返回错误码获取对应的文本ID，TURE-获取成功， FALSE-获取失败
/*****************************************************************************/
LOCAL BOOLEAN GetTextIdByReturnCode(uint32          err_code,       //in
                                    MMI_TEXT_ID_T   *text_id_ptr    //out
                                    )
{
    BOOLEAN  is_code_valid = TRUE;
    
    //只有网络侧返回的错误码(目前301-431), 且此错误码已定义对应显示文本, 才是有效的
    
    switch(err_code)
    {
    case MTVSERVICE_RST_DEVICE_AUTH_FAIL:
        *text_id_ptr = TXT_MBBMS_DEVICE_AUTHENTICATION_FAILED;
        break;
        
    case MTVSERVICE_RST_PURCHASE_UNKNOWN:
        *text_id_ptr = TXT_MBBMS_PURCHASE_ITEM_UNKNOWN;
        break;
        
    case MTVSERVICE_RST_SERVER_ERR:
        *text_id_ptr = TXT_MBBMS_SERVER_ERROR;
        break;
        
    case MTVSERVICE_RST_MALFORM_MSG:
        *text_id_ptr = TXT_MBBMS_MALFORMED_MESSAGE_ERROR;
        break;
        
    case MTVSERVICE_RST_NO_SUBSCRIPTION:
        *text_id_ptr = TXT_MBBMS_NO_SUBSCRIPTION;
        break;
        
    case MTVSERVICE_RST_NOT_PERMITTED:
        *text_id_ptr = TXT_MBBMS_OPERATION_NOT_PERMITTED;
        break;
        
    case MTVSERVICE_RST_REQUEST_UNAVAILABLE:
        *text_id_ptr = TXT_MBBMS_REQUESTED_SERVICE_UNAVAILABLE;
        break;
        
    case MTVSERVICE_RST_REQUEST_DONE:
        *text_id_ptr = TXT_MBBMS_REQUESTED_SERVICE_ACCEPTED;
        break;
        
    case MTVSERVICE_RST_INVALID_REQUEST:
        *text_id_ptr = TXT_MBBMS_INVALID_REQUEST;
        break;
        
    case MTVSERVICE_RST_WRONG_KEY_INFO:
        *text_id_ptr = TXT_MBBMS_DELIVERY_WRONG_KEY_INFORMATION;
        break;
        
    case MTVSERVICE_RST_ALREADY_USE:
        *text_id_ptr = TXT_MBBMS_ALREADY_IN_USE;
        break;
        
    case MTVSERVICE_RST_NO_MATCH_FRAG:
        *text_id_ptr = TXT_MBBMS_NO_MATCHING_FRAGMENT;
        break;
        
    case MTVSERVICE_RST_SUBSCRIBED:
        *text_id_ptr = TXT_MBBMS_NOW_SUBSCRIBED;
        break;
        
    case MTVSERVICE_RST_INSUFF_CHARGE:
        *text_id_ptr = TXT_MBBMS_INSUFFICIENT_CHARGE;
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIMBBMS GetTextIdByReturnCode invalid err_code = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8873_112_2_18_2_39_45_386,(uint8*)"d", err_code);
        is_code_valid = FALSE;
        break;
    }

    return is_code_valid;
}

/*****************************************************************************/
// 	Description : check whether description resource is valid
//	Global resource dependence : none
//  Author: xin.li
//	Note: 检查网络提示语信息是否有效
/*****************************************************************************/
LOCAL BOOLEAN CheckDescriptionResValid(void *data_ptr,              //in
                                       MBBMS_SERVICE_FUNC_E type    //in
                                       )
{
    MBBMS_Subscription_Update_Res_t *sub_update_res_ptr = PNULL;
    mbbms_gba_data_t                *gba_res_ptr = PNULL;
    MBBMS_Service_Request_Res_t     *sub_res_ptr = PNULL;
    MBBMS_Service_Request_Res_t     *unsub_res_ptr = PNULL;
    MBBMS_Account_Inquiry_Res_t     *account_inquiry_res_ptr = PNULL;
    BOOLEAN is_valid = FALSE;
    
    //to do need check description len>0
    //SCI_TRACE_LOW:"MMIMBBMS CheckDescriptionResValid  type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8897_112_2_18_2_39_45_387,(uint8*)"d", type);
    switch(type)
    {
    case MBBMS_SERVICE_UPDATE_TABLE:
        sub_update_res_ptr = (MBBMS_Subscription_Update_Res_t*)data_ptr;
        if(PNULL != sub_update_res_ptr)
        {
            if(PNULL != sub_update_res_ptr->response_ptr
                && sub_update_res_ptr->response_ptr->description_len > 0)
            {
                is_valid = TRUE;
            }
        }
        break;
        
    case MBBMS_SERVICE_GBA:
        gba_res_ptr = (mbbms_gba_data_t*)data_ptr;
        if(PNULL != gba_res_ptr)
        {
            is_valid = TRUE;
        }
        break;
        
    case MBBMS_SERVICE_SUBSCRIBE:
        sub_res_ptr = (MBBMS_Service_Request_Res_t*)data_ptr;
        if(PNULL != sub_res_ptr
            && sub_res_ptr->description_len > 0)
        {
            is_valid = TRUE;
        }
        break;
        
    case MBBMS_SERVICE_UNSUBSCRIBE:
        unsub_res_ptr = (MBBMS_Service_Request_Res_t*)data_ptr;
        if(PNULL != unsub_res_ptr
            && unsub_res_ptr->description_len > 0)
        {
            is_valid = TRUE;
        }
        break;
        
    case MBBMS_SERVICE_ACCOUNT_QUIRY:
        account_inquiry_res_ptr = (MBBMS_Account_Inquiry_Res_t*)data_ptr;
        if(PNULL != account_inquiry_res_ptr
            && account_inquiry_res_ptr->description_len > 0)
        {
            is_valid = TRUE;
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIMBBMS CheckDescriptionResValid Input type error!  type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8948_112_2_18_2_39_45_388,(uint8*)"d", type);
        break;
    }
    
    return is_valid;
}

/*****************************************************************************/
// 	Description : display error tip by returned description
//	Global resource dependence :
//  Author: xin.li
//	Note: 根据网络下发的提示语进行提示
/*****************************************************************************/
LOCAL void DisplayErrorTipByDescription(uint8   *description_ptr,
                                        uint16  len
                                        )
{
    wchar   wchar_des[MMIMBBMS_COMMON_MAX_LENS +1] = {0};
    uint32  min_len = 0;
    uint32  real_len = 0;
    MMI_STRING_T text_info = {0};
    
    min_len = MIN(MMIMBBMS_COMMON_MAX_LENS, len);
    
    real_len = GUI_UTF8ToWstr(wchar_des,
                    real_len,
                    description_ptr,
                    real_len);
    
    text_info.wstr_ptr = wchar_des;
    text_info.wstr_len = real_len;
    //SCI_TRACE_LOW:"MMIMBBMS DisplayErrorTipByDescription len=%d, min_len=%d, real_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_8977_112_2_18_2_39_45_389,(uint8*)"ddd", len, min_len, real_len);
    
    if(real_len > 0)
    {
        MMIPUB_OpenAlertWinByTextPtr(PNULL,
            &text_info,
            PNULL,
            IMAGE_PUBWIN_WARNING,
            PNULL,
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
    }
}

/*****************************************************************************/
// 	Description : 创建背景OSD层，用以显示音频广播背景图片等
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_CreateBgOSDLayer(void)
{
    UILAYER_CREATE_T        create_info = {0};
    UILAYER_RESULT_E        result = UILAYER_RESULT_SUCCESS;
    UILAYER_APPEND_BLT_T    append_info = {0};
    GUI_RECT_T full_rect = {0};
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    
    //SCI_TRACE_LOW:"MMIMBBMS CreateBgOSDLayer is_bg_layer_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9005_112_2_18_2_39_45_390,(uint8*)"d", s_bg_layer_info.is_bg_layer_open);
    
    if(!MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMIMBBMS CreateBgOSDLayer player win is not focus!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9009_112_2_18_2_39_45_391,(uint8*)"");
        return;
    }
    
    if(!UILAYER_IsMultiLayerEnable())
    {
        //SCI_TRACE_LOW:"MMIMBBMS CreateBgOSDLayer multi-layer not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9015_112_2_18_2_39_45_392,(uint8*)"");
        return;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    //SCI_TRACE_LOW:"MMIMBBMS CreateBgOSDLayer Logic lcd_width=%d, lcd_height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9020_112_2_18_2_39_45_393,(uint8*)"dd", lcd_width, lcd_height);
    
    //create info
    create_info.lcd_id          = GUI_MAIN_LCD_ID;
    create_info.owner_handle    = MMIMTV_MAIN_WIN_ID;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = MMIMTV_LCD_WIDTH;
    create_info.height          = MMIMTV_LCD_HEIGHT;
    create_info.is_bg_layer     = TRUE; //is bg layer
    create_info.is_static_layer = TRUE;
    
    //create layer
    result = UILAYER_CreateLayer(&create_info, &s_bg_layer_info.layer_dev_handle);
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_bg_layer_info.is_bg_layer_open = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS CreateBgOSDLayer UILAYER_CreateLayer return error:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9040_112_2_18_2_39_45_394,(uint8*)"d", result);
        s_bg_layer_info.is_bg_layer_open = FALSE;
        return;
    }
    
    //append BltLayer
    append_info.lcd_dev_info = s_bg_layer_info.layer_dev_handle;
    append_info.layer_level  = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_info);
    
    //layer color_key or alpha
    UILAYER_SetLayerColorKey(&s_bg_layer_info.layer_dev_handle, TRUE, MMIMTV_TRANSPARENT_COLOR);
    UILAYER_SetLayerAlpha(&s_bg_layer_info.layer_dev_handle, MMIMTV_GetTransparencyValue(MMIMTV_GetTransparency()));
    
    //display bg image and audio service tip
    full_rect = MMITHEME_GetFullScreenRect();
    UILAYER_ClearRect(&s_bg_layer_info.layer_dev_handle, full_rect);
    //IMG_EnableTransparentColor(TRUE);
    MMIMTV_DisplayAudioServiceTip(&s_bg_layer_info.layer_dev_handle);
}

/*****************************************************************************/
// 	Description : 获取背景层设备信息
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* MMIMBBMS_GetBgOSDLayerDev(void)
{
    return &s_bg_layer_info.layer_dev_handle;
}

/*****************************************************************************/
// 	Description : 背景层是否创建
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsBgOSDLayerOpen(void)
{
    return s_bg_layer_info.is_bg_layer_open;
}

/*****************************************************************************/
// 	Description : 销毁背景OSD层
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_ReleaseBgOSDLayer(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS ReleaseBgOSDLayer is_bg_layer_open=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9085_112_2_18_2_39_46_395,(uint8*)"d", s_bg_layer_info.is_bg_layer_open);
    
    if(!s_bg_layer_info.is_bg_layer_open)
    {
        return;
    }
    
    UILAYER_RELEASELAYER(&s_bg_layer_info.layer_dev_handle);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&s_bg_layer_info.layer_dev_handle);
}

/*****************************************************************************/
//  Description : set city name for update
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetUpdateCityName(MMI_STRING_T *text_ptr)
{
    if(PNULL != text_ptr)
    {
        s_city_name_len = 0;
        SCI_MEMSET(s_city_name, 0x00, sizeof(s_city_name));
        
        s_city_name_len = GUI_WstrToUTF8(s_city_name, MMIMBBMS_MAX_CITY_NAME_LENS, 
            text_ptr->wstr_ptr, text_ptr->wstr_len);
        
        //SCI_TRACE_LOW:"MMIMBBMS_SetUpdateCityName: wstr_len=%d utf8_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9110_112_2_18_2_39_46_396,(uint8*)"dd", text_ptr->wstr_len, s_city_name_len);
    }
}

/*****************************************************************************/
//  Description : GET city name for update
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIMBBMS_GetUpdateCityName(uint16 *city_name_len)
{
    *city_name_len = s_city_name_len;
    return s_city_name;
}

/*****************************************************************************/
//  Description : set city update SG flag
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCityUpdateSGFlag(BOOLEAN is_city_update)
{
    s_is_update_from_city_name = is_city_update;
}

/*****************************************************************************/
//  Description : whether update SG via city
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsCityUpdateSG(void)
{
    return s_is_update_from_city_name;
}

/*****************************************************************************/
//  Description : UnsubscriblePurchaseItem according to index in listbox for mbbms2.0
//  Global resource dependence : none
//  Author:  yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_UnSubscriblePurchaseItemNew(void)
{
    //MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr=PNULL;
    MBBMS_Purchase_Data_Reference_Service_t *data_ref_ptr=PNULL;
    //anyURI globalPurchaseItemId={0};    
    uint32 purchase_index=0;
    uint8 str_len=MBBMS_IDREF_LEN;
    //SG_ERROR_E err_code = SG_ERROR_NONE;
    //SG_LIST_PTR_T purchaseIndex_list_head_ptr=PNULL;
    MBBMS_SERVICE_RET_E request_ret = MBBMS_SERVICE_FUNC_OK;
    MBBMS_Purchase_Table_Info_t* purchase_table_info_ptr = PNULL;
    MBBMS_Account_Inquiry_Res_t* account_inquiry_res_ptr = MMIMBBMS_GetAccountInquiryRes();

    	
    if(PNULL == account_inquiry_res_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_UnSubscriblePurchaseItemNew account_inquiry_res_ptr = PNULL\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9162_112_2_18_2_39_46_397,(uint8*)"");
        return FALSE;
    }
    purchase_index = MMIMBBMS_GetCurrPurchaseIndex();
	
    purchase_table_info_ptr = MMIMBBMS_GetPurchasedItemByIndex(purchase_index, account_inquiry_res_ptr);
    if(PNULL == purchase_table_info_ptr)
    {
        //套餐信息表结点为空
        //SCI_TRACE_LOW:"---MMIMBBMS_UnSubscriblePurchaseItemNew purchase_table_info_ptr = PNULL\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9171_112_2_18_2_39_46_398,(uint8*)"");
        return FALSE;
    }
    
    if(PNULL != s_uns_subscrible_request_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_UnSubscriblePurchaseItemNew: s_uns_subscrible_request_ptr  is not null\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9177_112_2_18_2_39_46_399,(uint8*)"");
        MBBMS_Free_Service_Request_Ptr(s_uns_subscrible_request_ptr);
        
        s_uns_subscrible_request_ptr=PNULL;
    }
    
    s_uns_subscrible_request_ptr = (MBBMS_Service_Request_t*)SCI_ALLOC_APP(sizeof(MBBMS_Service_Request_t));
    if(PNULL == s_uns_subscrible_request_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_UnSubscriblePurchaseItemNew s_uns_subscrible_request_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9186_112_2_18_2_39_46_400,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(s_uns_subscrible_request_ptr,0,sizeof(MBBMS_Service_Request_t));
    
    s_uns_subscrible_request_ptr->request_id_valid = FALSE;        
    s_uns_subscrible_request_ptr->data_ref_ptr=(MBBMS_Purchase_Data_Reference_Service_t*)SCI_ALLOC_APP(sizeof(MBBMS_Purchase_Data_Reference_Service_t));
    if(PNULL == s_uns_subscrible_request_ptr->data_ref_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_UnSubscriblePurchaseItemNew data_ref_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9195_112_2_18_2_39_46_401,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET(s_uns_subscrible_request_ptr->data_ref_ptr,0,sizeof(MBBMS_Purchase_Data_Reference_Service_t));
    
    data_ref_ptr=s_uns_subscrible_request_ptr->data_ref_ptr;
    
    data_ref_ptr->next=PNULL;
     
    
    SCI_MEMCPY(data_ref_ptr->gloabl_id_ref,purchase_table_info_ptr->global_purchase_item_id,str_len);        
    
    request_ret = MTVSERVICE_UnsubRequest(s_uns_subscrible_request_ptr);
    //SCI_TRACE_LOW:"MMIMBBMS_UnSubscriblePurchaseItemNew MBBMS_Unsubscribe_Request return = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9209_112_2_18_2_39_46_402,(uint8*)"d", request_ret);
    if(MBBMS_SERVICE_FUNC_OK != request_ret)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_UnSubscriblePurchaseItemNew: service request error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9212_112_2_18_2_39_46_403,(uint8*)"");
        MBBMS_Free_Service_Request_Ptr(s_uns_subscrible_request_ptr);
        s_uns_subscrible_request_ptr=PNULL;
        return FALSE;
    }
    
    return TRUE;
    
}

/*****************************************************************************/
// Description :获取已订购套餐结点个数
// Global resource dependence :
// Author: yexiong.liu
// Note:
//should check index
/*****************************************************************************/
PUBLIC uint16 MMIMBBMS_GetAllPurchasedItemNum(MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr)
{
    MBBMS_Purchase_Table_Info_t *purchase_cur = NULL;
    uint16 node_count = 0;
    
    if(PNULL == account_inquiry_res_ptr)
    {
        return 0;	  
    }
    
    purchase_cur = account_inquiry_res_ptr->purchase_info_ptr;
    while(PNULL != purchase_cur)
    {
        node_count++;
        purchase_cur = purchase_cur->next;
    }
    
    return node_count;
}

/*****************************************************************************/
// Description :获取已订购套餐指定结点指针
// Global resource dependence :
// Author: yexiong.liu
// Note:
//should check index
/*****************************************************************************/
PUBLIC MBBMS_Purchase_Table_Info_t* MMIMBBMS_GetPurchasedItemByIndex(uint16 index, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr)
{
    MBBMS_Purchase_Table_Info_t *purchase_cur = NULL;
    uint16 current_node_index = 0;
    
    if(PNULL == account_inquiry_res_ptr)
    {
        return PNULL;	  
    }
    
    if(!(index < MMIMBBMS_GetAllPurchasedItemNum(account_inquiry_res_ptr)))
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetPurchasedItemByIndex  --  index error.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9264_112_2_18_2_39_46_404,(uint8*)"");
        return PNULL;
    }
    
    purchase_cur = account_inquiry_res_ptr->purchase_info_ptr;
    while(PNULL != purchase_cur)
    {
        if(index == current_node_index)
        {
            return purchase_cur;
        }
        current_node_index++;
        purchase_cur = purchase_cur->next;
    }
    //SCI_TRACE_LOW:"MMIMBBMS_GetPurchasedItemByIndex  --  purchase_cur  NULL.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9278_112_2_18_2_39_46_405,(uint8*)"");
    return purchase_cur;
}

/*****************************************************************************/
//  Description : check sgdd
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_CheckSGDD(void)
{
    MN_RETURN_RESULT_E   ret_value = MN_RETURN_FAILURE;
    SCI_MEMSET(&s_sgdd_id,0x00,sizeof(anyURI));

    MMINV_READ(MMINV_MBBMS_SGDD_INFO, &s_sgdd_id, ret_value);
    if(MN_RETURN_SUCCESS != ret_value)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_CheckSGDD read nv from init failed\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9294_112_2_18_2_39_46_406,(uint8*)"");
        MBBMSSG_GetSGDDId(&s_sgdd_id);
        SCI_MEMSET(&s_sgdd_id,0,sizeof(anyURI));
        MMINV_WRITE(MMINV_MBBMS_SGDD_INFO,&s_sgdd_id);     
    }
    else
    {
        anyURI sgdd_id = {0};
        MBBMSSG_GetSGDDId(&sgdd_id);
        //SCI_TRACE_LOW:"MMIMBBMS_CheckSGDD nv old url = %s, len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9303_112_2_18_2_39_46_407,(uint8*)"sd", s_sgdd_id.anyURI_arr, s_sgdd_id.string_len);
        //SCI_TRACE_LOW:"MMIMBBMS_CheckSGDD nv new url = %s, len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9304_112_2_18_2_39_46_408,(uint8*)"sd", sgdd_id.anyURI_arr, sgdd_id.string_len);
        if(!MMIMBBMS_IndexCompare(&sgdd_id, &s_sgdd_id))
        {
            MMIMBBMS_ClearFavoriteServiceList();
            s_sgdd_id = sgdd_id;
            MMINV_WRITE(MMINV_MBBMS_SGDD_INFO,&s_sgdd_id);    
        }
    }    
}

/*****************************************************************************/
//  Description : get current sim select
//  Global resource dependence :
//  Author: xin.li
//  Note: 获取当前选择的SIM卡
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIMBBMS_GetCurSelectSim(void)
{
    return s_dual_sys;
}

/*****************************************************************************/
//  Description : set current sim select
//  Global resource dependence :
//  Author: xin.li
//  Note: 设置当前选择的SIM卡
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurSelectSim(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys > MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_SetCurSelectSim dual_sys error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9331_112_2_18_2_39_46_409,(uint8*)"");
    }
    
    //SCI_TRACE_LOW:"MMIMBBMS_SetCurSelectSim, dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_MAIN_9334_112_2_18_2_39_46_410,(uint8*)"d", dual_sys);
    
    s_dual_sys = dual_sys;
}


//mbbms2.0 end

#endif
