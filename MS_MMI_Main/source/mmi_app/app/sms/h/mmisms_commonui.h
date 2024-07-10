/*************************************************************************
 ** File Name:      mmisms_commonui.h                                      *
 ** Author:         rong.gu                                          *
 ** Date:           2012/07/12                                            *
 ** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about receiving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2012/07/12     rong.gu      Create.                              *
*************************************************************************/
#ifndef _MMISMS_COMMONUI_H_
#define _MMISMS_COMMONUI_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "guimenu.h"
#include "guilistbox.h"
#include "guitext.h"
#include "mmi_text.h"
#include "mmisms_order.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmisms_image.h"
#include "mmisms_menutable.h"
#include "mms_image.h"
#include "guiform.h"
#include "mmipub.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define MMISMS_QUERY_STRING_MAX_NUM     (10)
#define MMISMS_ELLIPSE_STR_LEN          (3)
#define MMISMS_EMOJI_COMMOM_KEY_TIP_LEN (5)

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef struct MMI_MENU_EANBLE_GRAY_tag
{
   MMISMS_ORDER_ID_T        cur_order_id;
   uint32           marked_num;   
   //uint32           marked_sms_num;   
   //uint32           marked_mms_num;   
   MMISMS_BOX_TYPE_E         box_type;
   MMI_WIN_ID_T win_id;
   uint32 group_id;
}MMI_MENU_EANBLE_GRAY_T;

typedef enum
{
     MMISMS_APPEND_LIST_ITEM_TYPE,          //append
     MMISMS_REPLACE_LIST_ITEM_TYPE,         //replace
     MMISMS_INSERT_LIST_ITEM_TYPE,          //insert
     MMISMS_SET_LIST_ITEM_TYPE              //set list item data
} MMISMS_LIST_ITEM_TYPE_E;

typedef struct MMISMS_DELETE_DATE_INFO_tag
{
    MMISMS_OPER_ERR_E ret_value;
    uint16 all_del_msg_num;    
    BOOLEAN mms_is_sendrecv;
    BOOLEAN is_mms_del;
}MMISMS_DELETE_DATE_INFO;

typedef struct MMISMS_MOVE_DATE_INFO_tag
{
    MMISMS_OPER_ERR_E ret_value;
    uint16 all_move_msg_num;    
    BOOLEAN mms_is_sendrecv;
    BOOLEAN is_mms_move;    
}MMISMS_MOVE_RESULT_INFO;

typedef struct MMISMS_COPY_DATE_INFO_tag
{
    MMISMS_OPER_ERR_E ret_value;
    
}MMISMS_COPY_RESULT_INFO;


//sam.hua@Dec20 add data struct for select sim common api
typedef MMI_RESULT_E (* MMISMS_DEL_CALLBACK)(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr);

//sam.hua@Dec20 add data struct for select sim common api
typedef MMI_RESULT_E (* MMISMS_MOVE_CALLBACK)(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr);

typedef MMI_RESULT_E (* MMISMS_COPY_CALLBACK)(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr);


typedef struct
{    
    MMISMS_DEL_CALLBACK     del_call_back;  
    MMISMS_OPERATE_DATA_INFO_T opearte_data_info;
    uint16 all_del_msg_num;     //删除的个数
    BOOLEAN mms_is_sendrecv;  //MMS是否发送中
    BOOLEAN is_mms_del;      //是否删除了MMS
    void*  user_data;    
}MMISMS_DELWAIT_WIN_DATA;
#ifdef MMI_SMS_MOVE_SUPPORT
typedef struct
{    
    MMISMS_MOVE_CALLBACK     move_call_back;  
    MMISMS_MOVE_DATA_INFO_T move_data_info;
    uint16 all_move_msg_num;     //删除的个数
    BOOLEAN mms_is_sendrecv;  //MMS是否发送中
    BOOLEAN is_mms_move;      //是否删除了MMS
    void*  user_data;    
}MMISMS_MOVEWAIT_WIN_DATA;
#endif

#ifdef MMI_SMS_COPY_SUPPORT
typedef struct
{    
    MMISMS_COPY_CALLBACK     copy_call_back;  
    MMISMS_COPY_DATA_INFO_T copy_data_info;
    void*  user_data;    
}MMISMS_COPYWAIT_WIN_DATA;
#endif

//简单的手绘按键数据结构，其风格用常数表示
typedef enum
{
    MMISMS_EMOJI_TP_KEY_STATE_NONE, //不显示
    MMISMS_EMOJI_TP_KEY_STATE_INACTIVE, //不可用状态(gray)
    MMISMS_EMOJI_TP_KEY_STATE_ACTIVE, //正常状态
    MMISMS_EMOJI_TP_KEY_STATE_PRESS, //被按下

    MMIIM_TP_KEY_STATE_MAX,
}MMISMS_EMOJI_TP_KEY_STATE_E;

typedef struct
{
    GUI_RECT_T rect;
    wchar str[MMISMS_EMOJI_COMMOM_KEY_TIP_LEN];
    size_t str_len;
    MMISMS_EMOJI_TP_KEY_STATE_E state;
}MMISMS_EMOJI_COMMON_KEY_T;
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

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

/*****************************************************************************/
//     Description : to form the item for list_box
//    Global resource dependence : nsms1_state_ptr
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void FormListBoxItem(
                           MMISMS_STATE_T    *sms_state_ptr,     //IN:
                           MMI_CTRL_ID_T     ctrl_id,
                           uint16            index
                           );
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle mark all
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_HandleMarkAll(BOOLEAN is_mark_all,MMI_WIN_ID_T win_id,MMI_CTRL_ID_T cur_ctrl_id,MMISMS_BOX_TYPE_E cur_box_type,uint32 *marked_sms_num);
/*****************************************************************************/
//     Description : 
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_TYPE_E box_type, GUIFORM_CHILD_DISPLAY_E display_type);

#endif

/*****************************************************************************/
//     Description : handle dial menu enable
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleDialMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_t);

/*****************************************************************************/
//     Description : handle answer menu enable
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleAnswerMsgMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_t);

/*****************************************************************************/
//     Description : handle common menu enable
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleCommMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);

#ifdef MMI_SMS_MOVE_SUPPORT 
/*****************************************************************************/
//     Description : handle move menu enable
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleMoveMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT 
/*****************************************************************************/
//    Description : handle mms chat item opt
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleCopyMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);
#endif

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleDeleteMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);

/*****************************************************************************/
//    Description : handle mark menu enable gray
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSortMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);

/*****************************************************************************/
//    Description : handle mark menu enable gray
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleAddPBMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);

/*****************************************************************************/
//    Description : handle mms chat item opt
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleForwardMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);

/*****************************************************************************/
//    Description : handle mms chat item opt
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSendMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);

/*****************************************************************************/
//    Description : handle mms chat item opt
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleMarkMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//    Description : handle mark menu enable gray
//    Global resource dependence :
//    Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleChatMarkMenuEnableGray(MMI_MENU_EANBLE_GRAY_T *menu_enable_ptr);
#endif

#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT
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

/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of SMS box window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetSoftkeyAfterMarkOpera(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,BOOLEAN *is_mark_all);
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_MarkChatItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num);

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_CancelMarkChatItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num);
#endif

#ifndef MMI_GUI_STYLE_TYPICAL 
/*****************************************************************************/
//  Description : set anim in chat list
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DrawListBoxItemWithIconData(
    uint16           item_index,     //in:item索引
    uint16           content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T    ctrl_id,         //in:control id
    MMISMS_BOX_TYPE_E box_type
);
#endif

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenDelteQueryWin(MMISMS_DELETE_TYPE_E delete_type,
                                     BOOLEAN is_delete_push,
                                     MMISMS_ORDER_ID_T order_id,
                                     MMISMS_BOX_TYPE_E box_type,
                                     MMIPUB_HANDLE_FUNC handle_func);

#ifdef MMI_SMS_COPY_SUPPORT
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :set copy menu title
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCopyMenuTitle(void);
#endif
/*****************************************************************************/
//  Description : open the SMS move query window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenCopyQueryWinMsg(uint32 menu_id,
                                       MMISMS_BOX_TYPE_E box_type,
                                       uint32 marked_sms_num,
                                       MMISMS_ORDER_ID_T  cur_order_id,
                                       MMIPUB_HANDLE_FUNC handle_func
                                       );
#endif

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : open the SMS move query window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenMoveQueryWinMsg(uint32 menu_id,MMISMS_BOX_TYPE_E box_type,uint32 marked_sms_num,MMISMS_ORDER_ID_T  cur_order_id,MMIPUB_HANDLE_FUNC handle_func);
#endif

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewMsgPrompt(
                             MN_DUAL_SYS_E  dual_sys,
                             BOOLEAN    is_play_ring    //IN:
                             );

/*****************************************************************************/
//     Description : to set prompt win id
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void SetPromptWinId(MMI_HANDLE_T win_id);

#if defined(MMI_VCARD_SUPPORT)
/*****************************************************************************/
// Description : to handle the new Vcard message window
//               (参考函数:MMISMS_HandleNewMsgWin)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleNewVcardMsgWin(MMI_WIN_ID_T win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM param);
#endif

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : to handle the new Vcalendar message window
//               (参考函数:MMISMS_HandleNewMsgWin)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleNewVcalendarMsgWin(MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param);
#endif    

/*****************************************************************************/
//     Description : play new message ring
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_PlayNewSMSRing(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : set box type
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetBoxType(MMISMS_BOX_TYPE_E box_type);

PUBLIC void MMISMS_AppendListItem(
                          wchar             *string_ptr,        //IN:
                          uint8                string_len,            //IN:
                          MMI_CTRL_ID_T        ctrl_id,            //IN:
                          MMI_TEXT_ID_T        left_softkey_id,    //IN:
                          MMI_TEXT_ID_T        mid_softkey_id,    //IN:Randy add new param @10-27 
                          uint16            pos,                //IN:
                          MMISMS_LIST_ITEM_TYPE_E type
                          );

/*****************************************************************************/
//     Description : 开机初始化完时，打开等待窗口。
//    Global resource dependence : 
//  Author: liming.deng
/*****************************************************************************/
PUBLIC void MMISMS_OpenInitWaitingWindow(MMISMS_OPERATE_TYPE_E oper_type);

/*****************************************************************************/
//  Description : Open Alert Wanring Win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenAlertWarningWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//     Description : is in sms moudle window
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsInSMSWin(void);

/*****************************************************************************/
//     Description : judge whether can play new message alert ring
//    Global resource dependence : 
//  Author: bruce.chi
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsPermitPlayNewMsgRing(void);

/*****************************************************************************/
//     Description : to display send err win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DisplaySendErr(
                                  MN_DUAL_SYS_E         dual_sys,
                                  MMISMS_OPER_ERR_E    oper_err    //IN:
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


#if defined(MMI_VCARD_SUPPORT)
/*****************************************************************************/
// Description : to handle the new Vcard message window
//               (参考函数:MMISMS_HandleNewMsgWin)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
MMI_RESULT_E MMISMS_HandleNewVcardMsgWin(MMI_WIN_ID_T win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM param);
#endif

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : to handle the new Vcalendar message window
//               (参考函数:MMISMS_HandleNewMsgWin)
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
MMI_RESULT_E MMISMS_HandleNewVcalendarMsgWin(MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param);
#endif

/*****************************************************************************/
//     Description : to handle the new message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleNewMsgWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    );


/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleDelWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );

/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterDelWaitWin(MMISMS_OPERATE_DATA_INFO_T *data_info_ptr,MMISMS_DEL_CALLBACK del_call_back,void*  user_data);

/*****************************************************************************/
//     Description : 开机初始化完毕后，关闭等待窗口，并进入相应的操作。
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_CloseWaitingWin(void);
#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterMoveWaitWin(MMISMS_MOVE_DATA_INFO_T *data_info_ptr,MMISMS_MOVE_CALLBACK move_call_back,void*  user_data);
#endif


#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterCopyWaitWin(MMISMS_COPY_DATA_INFO_T *data_info_ptr,MMISMS_COPY_CALLBACK copy_call_back,void*  user_data);
#endif

#ifdef   __cplusplus
    }
#endif

#endif
