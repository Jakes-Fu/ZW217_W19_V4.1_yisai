/***************************************************************************
** File Name:      mmisms_internal.h                                       *
** Author:                                                                 *
** Date:           04/2006                                                   *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the SMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2006          Tracy Zhang       Modularize
****************************************************************************/
#ifndef  _MMISMS_INTERNAL_H_    
#define  _MMISMS_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmisms_export.h"
#include "mmisms_app.h"
#include "mn_type.h"
#include "mmi_event_api.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#define SMSAPP_SRV_SUPPORT

 
#ifdef __cplusplus
    extern "C"
    {
#endif

#ifdef CMCC_UI_STYLE
#define MMI_SMS_SIMBOX
#endif


/*****************************************************************************/
//     Description : save the message to the draft box
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveAsDraft(MN_DUAL_SYS_E dual_sys);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : init the SMS global
//    Global resource dependence : g_mmisms_app, 
//                               g_mmisms_global,
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_AppInit(void);

/*****************************************************************************/
//     Description : init the SMS for PS
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitForPS(void);



/*****************************************************************************/
//     Description : get delete all flag
//                  TRUE:        do delete all SMS operation  
//                  FALSE:    without do delete all SMS operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_GetDelAllFlag(void);

/*****************************************************************************/
//     Description : to enter the window for writing SMS 
//    Global resource dependence : 
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_WriteNewMessage(
                            MN_DUAL_SYS_E   dual_sys,           //IN:
                            MMI_STRING_T    *sms_init_ptr,        //IN:
                            BOOLEAN            need_load_saved,    //IN:    TRUE, load the saved content
                            uint8            *dest_addr_ptr,        //IN:
                            uint8            dest_addr_len        //IN:
                            );

/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_WriteNewMessageEx(
    MN_DUAL_SYS_E   dual_sys,           //IN:
    MMI_STRING_T    *sms_init_ptr,        //IN:
    BOOLEAN            need_load_saved,    //IN:    TRUE, load the saved content
    uint8            *dest_addr_ptr,        //IN:
    uint8            dest_addr_len        //IN:
);

/*****************************************************************************/
//     Description : to send group sms
//    Global resource dependence :
//  Author:rong.gu
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_SendGroupSms(
    MN_DUAL_SYS_E   dual_sys,           //IN:
    MMI_STRING_T    *sms_init_ptr,        //IN:
    MMISMS_DEST_ADDR_INFO_T *dest_info
);

/*****************************************************************************/
//     Description : to send group sms
//    Global resource dependence :
//  Author:rong.gu
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_SendGroupSmsEx(
    MN_DUAL_SYS_E   dual_sys,           //IN:
    MMI_STRING_T    *sms_init_ptr,        //IN:
    MMISMS_DEST_ADDR_INFO_T *dest_info
);
/*****************************************************************************/
//  Description : to enter the window for edit SMS
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_EditSMSbyOrderID(MMISMS_ORDER_ID_T order_id);

/*****************************************************************************/
//  Description : to enter the window for edit SMS
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_ForwardSMSbyOrderID(MMISMS_ORDER_ID_T order_id);


/*****************************************************************************/
//     Description : to check the order is ok
//    Global resource dependence : g_mmisms_order
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_IsOrderOk( void );

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_IsReady(void);

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_IsSMSReady(MN_DUAL_SYS_E dual_sys);


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
                                 );

//window related
/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
MMI_RESULT_E MMISMS_HandleWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );

/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
//lzk mark
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
MMI_RESULT_E MMISMS_HandleBtWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );
#endif
/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
MMI_RESULT_E MMISMS_HandleSendWaitWinMsg(
                                         MMI_WIN_ID_T        win_id,    //IN:
                                         MMI_MESSAGE_ID_E    msg_id,    //IN:
                                         DPARAM                param    //IN:
                                         );


#if 0
//#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to open the window to set the save position
//    Global resource dependence : 
//  Author: Liqing Peng
//    Note: 
/*****************************************************************************/
void MMISMS_OpenSetSavePosWin(void);
//#endif
/*****************************************************************************/
//     Description : to open set sc win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMISMS_OpenSetSCWin(MN_DUAL_SYS_E dual_sys);

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
/******************* **********************************************************/
//     Description : to open set auto signature win
//    Global resource dependence : 
//  Author:bown.zhang
//    Note: 
/*****************************************************************************/
void MMISMS_OpenSetAutoSignatureWin(void);
#endif
#endif
                                                                                
/*****************************************************************************/
//     Description : to open set vaild period win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
//void MMISMS_OpenSetVPWin( void );

/*****************************************************************************/
//  Description : read new sms ex
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ReadNewSmsEx(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : read new sms
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
void MMISMS_ReadNewSms(
                       MMISMS_BOX_TYPE_E        box_type
                       );

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_ShowNewMsgPrompt(
                             MN_DUAL_SYS_E  dual_sys,
                             BOOLEAN    is_play_ring    //IN:
                             );



#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
/*****************************************************************************/
//     Description : open frequent word list for cc module
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMS_OpenFreqWordListForCc(
                                  MN_DUAL_SYS_E dual_sys,
                                  uint8    number_len,    //IN:
                                  uint8    *number_str    //IN:
                                  );
#endif

/*****************************************************************************/
//     Description : to check the space
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
uint16 MMISMS_CountFreeSpace(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : set the flag of playing new SMS ring
//    Global resource dependence : 
//  Author: Tracy Zhang
/*****************************************************************************/
void MMISMS_SetNewSMSRingFlag(
                              BOOLEAN    is_need_play    //IN:
                              );

/*****************************************************************************/
//     Description : get the flag of playing new SMS ring
//    Global resource dependence : 
//  Author: Tracy Zhang
/*****************************************************************************/
BOOLEAN MMISMS_GetNewSMSRingFlag(void);

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to open the window to set the preferred connection
//    Global resource dependence : 
//  Author: Liqing Peng
//    Note: 
/*****************************************************************************/
void MMISMS_OpenSetPreferredConnection(void);
#endif

/*****************************************************************************/
#if 0 /* Reduce Code size */
/*****************************************************************************/
//    Description : to create URL option WIN
//    Global resource dependence : 
//    Author:Kun.Yu
//    Note: 
/*****************************************************************************/
void MMISMS_Create_Win_URL_Opt( ADD_DATA add_data );
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : get dual sys when new msg arrive
//    Global resource dependence : s_dual_new_sms
//  Author: bown.zhang
//    Note:
/*****************************************************************************/
MN_DUAL_SYS_E MMISMS_GetSimIdOfNewMsg(void);

/*****************************************************************************/
//     Description : set dual sys when new msg arrive
//    Global resource dependence : s_dual_new_sms
//  Author: bown.zhang
//    Note:
/*****************************************************************************/
void MMISMS_SetSimIdOfNewMsg(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :set extract flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void  MMISMS_SetSMSIsExtract(BOOLEAN is_parser);

/*****************************************************************************/
//  Description : Set the push sms browser info
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetPushSmsBrowserInfo(BOOLEAN flag);

/*****************************************************************************/
//  Description : Get the push sms SL Operation
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SL_OPERATION_E MMISMS_GetPushSmsSLOperation(void);

/*****************************************************************************/
//  Description : Set the push sms browser info
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetPushSmsSLOperation(MMISMS_SL_OPERATION_E oper);

/*****************************************************************************/
//     Description : handle left and right msg in show SMS window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void HandleLeftRightMsgInShowSmsWin(
                                          MMI_WIN_ID_T        win_id,        //IN:
                                          MMI_MESSAGE_ID_E    msg_id        //IN:
                                          );

/*****************************************************************************/
//     Description : get move all flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move all SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetMoveAllFlag(void);


/*****************************************************************************/
//     Description : Get time display string
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetTimeDispString(                                        //RETURN: the length of time string
                              MN_SMS_TIME_STAMP_T    *sc_time_ptr,    //IN:
                              wchar                    *time_ptr        //OUT:
                              );

/*****************************************************************************/
//     Description : is sim not initial 
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSimNotInited(void);

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsForbitNewMsgWin(BOOLEAN is_lockwin_show_prompt);

/*****************************************************************************/
//  Description : phonebook callback
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMISMS_HandlePBEventFunc(MMIEVENT_PB_EVENT_E event);




/*****************************************************************************/
//  Description : Select SIM API of SMS app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC MMISMSSelectSIMWinCallback,
                                        ADD_DATA       add_data,
                                        BOOLEAN psReady
                                        );

/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  MultiSendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);


#if defined(MMI_SMS_DELALL_SUPPORT)
/*****************************************************************************/
// 	Description : to enter the del all msg window
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterDelAllWin(void);
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :open settings tab win
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSettingTabWin(void);
#endif
/*****************************************************************************/
//  Description : create setting win func
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CreateSmsSettingWin(void);

/*****************************************************************************/
//  Description : create setting win func
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CreateBrowserSettingWin(void);

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : create setting win func
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CreateServiceLoadingOperationSettingWin(MMI_CTRL_ID_T ctrl_id);
#endif

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
//  Description : open voice mail win
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenVoiceMailWin(void);

#endif

/*****************************************************************************/
//  Description : close edit win
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_CloseEditWin(void);

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
/*****************************************************************************/
//  Description : to save the sim MN
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveSIMModeToMN(void);
#endif

#ifdef MMI_SMS_FIX_SEND_SIM

PUBLIC BOOLEAN MMISMS_SetFixSIMSendingSIMSYS(uint32 sim_sys);

PUBLIC uint32 MMISMS_GetFixSIMSendingSIMSYS(void);

PUBLIC void MMISMS_SetFixSIMSendingOnOFF(BOOLEAN is_onoff);

PUBLIC BOOLEAN MMISMS_GetFixSIMSendingOnOFF(void);
#endif

#ifdef MMI_SMS_SIMBOX

#define MMISMS_SIM_BOX_MAX_COUNT    100     //暂定为100，应该是从底层获取

typedef struct
{
    uint16              matched_count;
    uint16              last_matched_count;
    MMISMS_ORDER_ID_T   order_id_list[MMISMS_SIM_BOX_MAX_COUNT];
}MMISMS_SIMBOX_DATA_T;


/*****************************************************************************/
// 	Description : listitem draw callback
//	Global resource dependence :
//  Author:   
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetSIMBOXMatchItemContent(
        GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data, BOOLEAN isSelecting);

/*****************************************************************************/
//  Description : 搜索短信
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
PUBLIC int16 MMISMS_SIMBOX_SearchMatchedItem(MMISMS_SIMBOX_DATA_T   *msgLogData,MN_DUAL_SYS_E dual_sys);
        
#endif

/*****************************************************************************/
//  Description : to handle the create select word win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CreateSelectWordWin(MMI_HANDLE_T ctrl_handle);


PUBLIC void MMISMS_EnterNewSmsNewWin(void);

/*****************************************************************************/
//  Description : Enter Send Option Menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSendNumOptWin(uint32 send_type);

/*****************************************************************************/
//  Description : Enter Send Option Menu
//  Global resource dependence : 
//  Author:yongli.tong
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSendNumOptWinExt(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//     Description : to enter send prompt win
//    Global resource dependence : 
//  Author: rong gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSendPromptWin(ADD_DATA data_ptr);


/*****************************************************************************/
//     Description : enter phiz win
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenPhizWin(MMI_HANDLE_T ctrl_handle);

/*****************************************************************************/
//     Description : enter emoji win
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenEmojiWin(void);

/*****************************************************************************/
//     Description : MMISMS_SetMessageContent
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMessageContent( 
                                     uint16    length,            //IN:
                                     wchar     *content_ptr,    //IN:
                                     BOOLEAN   is_need_packet,
                                     MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr
                                     );


/*****************************************************************************/
//     Description : to open write SMS word window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterWordListWin(void);


/*****************************************************************************/
//  Description : Get the push sms browser info
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetPushSmsBrowserInfo(void);


/*****************************************************************************/
//     Description : enter sms chat read win
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSMSChatReadWin(MMISMS_ORDER_ID_T cur_order_id);

/*****************************************************************************/
//  Description :Get is save draft
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsWaitUpdate(void);


/*****************************************************************************/
//     Description : to open the list of box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSmsBox(
                       MMISMS_BOX_TYPE_E box_type
                       );

/*****************************************************************************/
//  Description :get extract flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetSMSIsExtract(void);


/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_EnterEditSMSWinByOrderID(MMISMS_ORDER_ID_T order_id);

/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_EnterEditSMSWinForForward(MMISMS_ORDER_ID_T order_id);

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
// Description : start calllog pbap sync process
// Global resource dependence : 
// Author: Cheney.Wu
// Note: 
/*****************************************************************************/
PUBLIC void StartMapMessageSync(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : Handle BTMapConnectSuccess
// Global resource dependence : 
// Author: Cheney.Wu
// Note: 
/*****************************************************************************/
PUBLIC void HandleBTMapSetFolderSuccess(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : Handle BTMapConnectSuccess
// Global resource dependence : 
// Author: Cheney.Wu
// Note: 
/*****************************************************************************/
PUBLIC void HandleBTMapConnectSuccess(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : Handle BTMsgSyncCfn
// Global resource dependence : 
// Author: Cheney.Wu
// Note: 
/*****************************************************************************/
PUBLIC void HandleBTMsgSyncCfn(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : BT SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_BtSendItem();

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  Candice.Chen
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleConnectWaitingMsg(
                                                  MMI_WIN_ID_T         win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  );
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
