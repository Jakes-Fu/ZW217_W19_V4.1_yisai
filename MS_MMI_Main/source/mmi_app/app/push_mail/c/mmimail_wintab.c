/****************************************************************************
** File Name:      mmimail_wintab.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MMIMAIL_WINTAB_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_app_push_mail_trc.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "mmimail_internal.h"
#include "mmimail_wintab.h"
#include "mmimail_edit.h"
#include "mmimail_read.h"
#include "mmimail_text.h"
#include "mmimail_image.h"
#include "mmimail_id.h"
#include "mmimail_menutable.h"
#include "mail_main.h"
#include "mail_export.h"
#include "mail_fldr.h"
#include "mail_setting.h"
#include "mail_file.h"
#include "guitab.h"
#include "guilistbox.h"
#include "guitext.h"
#include "guires.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "comm_memory.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_image.h"
#include "mmiidle_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmicc_export.h"
#include "mmipub.h"
#include "mmifmm_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmivt_export.h"
#include "mmi_default.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef STREAMING_HS_SUPPORT
#include "mmismp_export.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

#include "mmisms_export.h"
#include "mmipic_edit.h"
#include "mmisd_export.h"
#include "mmiudisk_export.h"
#include "mail_network.h"
#include "guictrl_api.h"
#include "mmiphone_export.h"
#include "mmisms_image.h"

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
#include "mmisms_app.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMAIL_MENUWIN_TIME                1000

#define MMIMAIL_WELCOME_CONTENT_LEN         300 //欢迎界面内容最大长度

#define MMIMAIL_SET_OPEN_CLOSE_ITEM         2
#define MMIMAIL_SET_AUTO_CHANGE_ITEM        3
#define MMIMAIL_SET_SIZE_LIMIT_ITEM         5

#define MMIMAIL_TIME_STR_LEN                10 //时间字符串的长度

#define MMIMAIL_SIZE_100K_BYTE              (100 * 1024)

#define MMIMAIL_CMCC_MCC_NUM                460
#define MMIMAIL_CMCC_MNC_NUM_00             00
#define MMIMAIL_CMCC_MNC_NUM_02             02
#define MMIMAIL_CMCC_MNC_NUM_07             07

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//邮箱主菜单menu id
typedef enum
{
    MMIMAIL_MAIN_MENU_NODE_NULL,
    MMIMAIL_MAIN_MENU_NODE_WRITE_NEW_MAIL, 
    MMIMAIL_MAIN_MENU_NODE_INBOX,
    MMIMAIL_MAIN_MENU_NODE_OUTBOX,
    MMIMAIL_MAIN_MENU_NODE_SENTBOX,
    MMIMAIL_MAIN_MENU_NODE_DRAFTBOX,
    MMIMAIL_MAIN_MENU_NODE_MAX
}MMIMAIL_MAIN_MENU_NODE_E;

typedef enum
{
    MMIMAIL_APPEND_LIST_ITEM_TYPE,          //append
    MMIMAIL_REPLACE_LIST_ITEM_TYPE,         //replace
    MMIMAIL_INSERT_LIST_ITEM_TYPE,          //insert
    MMIMAIL_SET_LIST_ITEM_TYPE              //set list item data
}MMIMAIL_LIST_ITEM_TYPE_E;

//帐号编辑界面item枚举
typedef enum
{
    MMIMAIL_ACCOUNT_NAME_LABEL,             //帐号名称标签
    MMIMAIL_ACCOUNT_NAME_EDIT,              //帐号名称内容
    MMIMAIL_ACCOUNT_ADDR_LABEL,             //帐号地址标签
    MMIMAIL_ACCOUNT_ADDR_EDIT,              //帐号地址内容
    MMIMAIL_ACCOUNT_UERNAME_LABEL,          //用户名称标签
    MMIMAIL_ACCOUNT_UERNAME_EDIT,           //用户名称内容
    MMIMAIL_ACCOUNT_PASSWORD_LABEL,         //密码标签
    MMIMAIL_ACCOUNT_PASSWORD_EDIT          //密码内容
}MMIMAIL_EDIT_ITEM_E;

//静音时段编辑界面item枚举
typedef enum
{
    MMIMAIL_EDIT_SILENT_START_TIME_LABEL,   //开始时间标签
    MMIMAIL_EDIT_SILENT_START_TIME_EDIT,    //开始时间内容
    MMIMAIL_EDIT_SILENT_END_TIME_LABEL,     //结束时间标签
    MMIMAIL_EDIT_SILENT_END_TIME_EDIT      //结束时间内容
}MMIMAIL_EDIT_SILENT_TIME_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL const MMI_TEXT_ID_T s_sim_status_string[SIM_STATUS_MAX + 1] = 
{
    TXT_NULL,                       //SIM_STATUS_OK,
    TXT_SIM_REJECTED,               //SIM_STATUS_REJECTED,
#ifndef MMI_IDLE_MINI_SUPPORT
    TXT_SIM_REGISTRATION_FAILED,//SIM_STATUS_REGISTRATION_FAILED,
#else
    TXT_SIM_REGISTRATION_FAILED_MINI,//SIM_STATUS_REGISTRATION_FAILED,
#endif
    
    TXT_PIN_BLOCKED,                //SIM_STATUS_PIN_BLOCKED
    TXT_NOT_SUPPORT_SIM,            //SIM_STATUS_NOT_SUPPORT_SIM,                                                            
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK_BLOCKED,
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK2_BLOCKED,
    TXT_NO_SIM,                     //SIM_STATUS_NO_SIM,
    TXT_PROCESS_SEARCHING_NETWORK,  //SIM_NOT_INITED
    TXT_NULL                        //SIM_STATUS_MAX
};

MMI_APPLICATION_T               g_mmimail_app; //MAIL的实体

LOCAL MMIMAIL_MANAGE_INFO_T     s_mmimail_info = {0}; //MAIL的全局变量
LOCAL BOOLEAN                   s_mmimail_is_sms_ok = FALSE; //标记短信模块是否准备就绪

LOCAL wchar * s_acc_file_ptr = PNULL;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern BOOLEAN GPIO_CheckHeadsetStatus(void);
extern PUBLIC BOOLEAN MMK_IsNeedCloseWin( 
                                  MMI_HANDLE_T win_handle
                                  );

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 打开欢迎界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_OpenWelcomeWin(void);

/*****************************************************************************/
// 	Description : 打开邮箱列表界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_OpenMailListWin(void);

/*****************************************************************************/
//	Description : 进入相应的邮箱
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_EnterMailBox(
                                       MMIMAIL_BOX_TYPE_E        box_type
                                       );

/*****************************************************************************/
// 	Description : 进入编辑界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterEditWin(MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type, uint32 record_id);

/*****************************************************************************/
// 	Description : 进入设置界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterSettingWin(void);

/*****************************************************************************/
// 	Description : 进入阅读界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterReadWin(uint32 record_id);

/*****************************************************************************/
// 	Description : 进入应用检查提示
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterAppCheck(void);

/*****************************************************************************/
// Description : 判断是否是中国移动的SIM卡
// Global resource dependence:
// Author: yuanl
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsCmccSimCard(void);

/*****************************************************************************/
// 	Description : 是否有保存的本地帐号
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsHaveLocalAccount(void);

/*****************************************************************************/
// 	Description : 获取指定帐号的邮箱信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_GetAccountMailBoxInfo(
                                                        MMIMAIL_ACCOUNT_INFO_T *account_info, //in
                                                        MMIMAIL_BOX_INFO_T     *mailbox_info  //out
                                                      );
                                                      
/*****************************************************************************/
// 	Description : 获取资费信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_GetChargeInfo(void);

/*****************************************************************************/
//	Description : 打开等待初始化窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_OpenInitWaitingWindow(void);

/*****************************************************************************/
//	Description : 关闭等待初始化窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CloseInitWaitingWindow(BOOLEAN is_success);

/*****************************************************************************/
//	Description : 等待获取帐号配置信息窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitGetAccountWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       );

/*****************************************************************************/
//	Description : 是否覆盖现有帐号窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOverwriteQueryWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       );

/*****************************************************************************/
//	Description : 是否删除原有帐号窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelExistAccountQueryWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       );

/*****************************************************************************/
//	Description : 等待初始化窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInitWaitingWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       );
                                   
/*****************************************************************************/
//	Description : 删除邮件询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            );

/*****************************************************************************/
//	Description : Query delete all func
//	Global resource dependence : none
//	Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllInCurBoxQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            );

/*****************************************************************************/
//	Description : 删除邮件等待处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );
                                    
/*****************************************************************************/
//	Description : 处理mail线程发来的消息
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailTaskMsg(
                               PWND        app_ptr,
                               uint16      msg_id,
                               DPARAM      param
                               );

/*****************************************************************************/
//	Description : 退出邮箱
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_Exit(void);

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
/*==========================================================
 * author		: liangshun      
 * function 	: LOCAL BOOLEAN MMIMAIL_NeedDispDelOpt(MMIMAIL_BOX_TYPE_E  box_type)
 * decr		: 判断是否需要在菜单中显示"删除"选项
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/13/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_NeedDispDelOpt(MMIMAIL_BOX_TYPE_E  box_type);
#endif

/*****************************************************************************/
// 	Description : 欢迎界面
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailWelcomeWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );

/*****************************************************************************/
// 	Description : 提示开通信息界面
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailApplyAlertWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );

/*****************************************************************************/
// 	Description : 邮箱列表界面主处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailMainMenuWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );

/*****************************************************************************/
// 	Description : 邮箱列表界面选项菜单处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailMainMenuOptWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );
                                               
/*****************************************************************************/
//	Description : 邮箱主界面处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailBoxMainWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
/*****************************************************************************/
//	Description : 收件箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInBoxChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 发件箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutBoxChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 已发件箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSentBoxChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );
                                      
/*****************************************************************************/
//	Description : 草稿箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDraftChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//  Description : 处理邮件列表界面MSG_GET_FOCUS消息
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void HandleMailListGetFocusMsg(
                                                MMIMAIL_BOX_TYPE_E  box_type, 
                                                MMI_CTRL_ID_T       ctrl_id,
                                                BOOLEAN             *need_update_flag, //in & out
                                                BOOLEAN             *have_items //in & out
                                             );

/*****************************************************************************/
//  Description : 处理邮件列表界面 MMISMS_MSG_UPDATELIST 消息
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void HandleMailListUpdateMsg(
                                            MMIMAIL_BOX_TYPE_E  box_type, 
                                            MMI_CTRL_ID_T       ctrl_id,
                                            BOOLEAN             *have_items //in & out
                                         );

/*****************************************************************************/
//  Description : 处理邮件列表界面 标记/取消标记 操作
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void HandleMailListMarkOperation(
                                            MMIMAIL_BOX_TYPE_E  box_type, 
                                            MMI_CTRL_ID_T       ctrl_id,
                                            DPARAM              param
                                         );

/*****************************************************************************/
//	Description : 处理中间键消息，进入邮件浏览界面
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void HandleMailListOkMsg(
                                MMI_WIN_ID_T         win_id,
                                MMI_MESSAGE_ID_E     msg_id,
                                MMI_CTRL_ID_T        ctrl_id,
                                MMIMAIL_BOX_TYPE_E   box_type
                                );
                                
/*****************************************************************************/
//	Description : 处理邮箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMailBoxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 );

/*****************************************************************************/
//	Description : 处理收件箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleInboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 );

/*****************************************************************************/
//	Description : 处理发件箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleOutboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 );

/*****************************************************************************/
//	Description : 处理已发件箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSentboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 );                                 
                                         
/*****************************************************************************/
//	Description : 收件箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//	Description : 发件箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//	Description : 发件箱选项菜单处理函数，针对正在发送中的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:只有取消发送一个菜单项
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutboxMenuOptExtWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//	Description : 已发件箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSentboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//	Description : 草稿箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDraftboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
                                          
/*****************************************************************************/
//  Description : 显示欢迎界面信息
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void ShowWelcomeInfo(
                                    BOOLEAN         is_disp_charge_info, //是否显示资费信息
                                    MMI_STRING_T    *charge_info         //资费信息
                                 );

/*****************************************************************************/
//	Description : 启动检查是否需要更新菜单内容的定时器
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void StartMailMenuWinTimer(
                                  MMI_WIN_ID_T  win_id  //in
                                  );

/*****************************************************************************/
//	Description : 停止检查是否需要更新菜单内容的定时器
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void StopMailMenuWinTimer(void);

/*****************************************************************************/
//	Description : 添加tab页和子窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows( void );

/*****************************************************************************/
// 	Description : 创建邮箱列表界面主菜单
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void CreateMailMainMenu(
                                     MMI_WIN_ID_T   win_id,
                                     MMI_CTRL_ID_T  ctrl_id
                                     );

/*****************************************************************************/
//	Description : 更新邮箱列表界面主菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void UpdateMailMainMenu(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id                             
                             );
                                     
/*****************************************************************************/
// 	Description : 获取邮件数量信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void SetMailNumState2String(
                              MMI_STRING_T        *string_ptr,    //OUT:
                              MMIMAIL_BOX_TYPE_E   box_type       //IN:
                              );
                              
/*****************************************************************************/
//	Description : 进入相应的邮箱
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void EnterMailCorrespondingMenu(
                                     MMIMAIL_MAIN_MENU_NODE_E node_id
                                     );
                                          
/*****************************************************************************/
//	Description : 开始读取邮件，通知mail线程解码
//	Global resource dependence : none
//	Author:yuanl
//	Note: 从邮件列表界面进入读取过程
/*****************************************************************************/
LOCAL void StartReadMail(
                            MMIMAIL_BOX_TYPE_E box_type,
                            MMIMAIL_OPERATION_T oper_type, 
                            MMI_CTRL_ID_T ctrl_id //邮件列表控件id
                            );

/*****************************************************************************/
//	Description : 进入邮箱
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_Enter(MAIL_ENTER_TYPE_E   enter_type, DPARAM param);

/*****************************************************************************/
//	Description : 保存帐号配置信息
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SaveAccountConfig(void);
                                  
/*****************************************************************************/
// 	Description : 获取默认帐号指定邮箱邮件总数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL uint16 MMIMAIL_GetBoxTotalNumber(
                                                     MMIMAIL_BOX_TYPE_E    box_type
                                                 );

/*****************************************************************************/
// 	Description : 获取默认帐号收件箱未读邮件数目
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL uint16 MMIMAIL_GetInboxBoxUnreadNum(void);

/*****************************************************************************/
//	Description : 根据邮箱类型生成列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadMailList(
                                            MMIMAIL_BOX_TYPE_E  box_type,
                                            MMI_CTRL_ID_T       ctrl_id
                                           );
                                           
/*****************************************************************************/
//	Description : 获取当前邮箱邮件链表指针头
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMIMAIL_MAIL_SUMMERY_T  *MMIMAIL_GetMailListHead(
                                                                MMIMAIL_BOX_TYPE_E  box_type
                                                              );

/*****************************************************************************/
//	Description : 获取与list_index对应的邮件指针
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMIMAIL_MAIL_SUMMERY_T  *MMIMAIL_GetCurMailPtr(
                                                            MMIMAIL_BOX_TYPE_E  box_type,
                                                            uint16              list_index
                                                           );
                                                           
/*****************************************************************************/
//	Description : 根据邮件数量添加列表项
//	Global resource dependence : none
//	Author:yuanl
//	Note:此处为空项，等显示时再获取列表实际显示内容
/*****************************************************************************/
LOCAL void MMIMAIL_AppendAllItems(
                                            MMI_CTRL_ID_T       ctrl_id,
                                            MMIMAIL_BOX_TYPE_E  box_type
                                          );

/*****************************************************************************/
//	Description : 设置列表项信息
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_AppendListItem(
                              wchar                    *string_ptr,
                              uint16                    string_len,
                              MMI_CTRL_ID_T             ctrl_id,
                              MMI_TEXT_ID_T             left_softkey_id,
                              MMI_TEXT_ID_T             mid_softkey_id,
                              uint16                    pos,
                              GUIITEM_STYLE_E           item_style,
                              MMIMAIL_LIST_ITEM_TYPE_E  type
                              );
                              
/*****************************************************************************/
//	Description : 设置空列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,         //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: 
                             MMI_TEXT_ID_T    left_softkey_id  //IN:
                             );

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//
/*****************************************************************************/
//  Description : 根据邮件状态获取显示图标
//  Global resource dependence : none
//  Author:yuanl
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T MMIMAIL_GetIconIdByState( MMIMAIL_STATE_E  state, BOOLEAN is_locked);
#else
/*****************************************************************************/
//	Description : 根据邮件状态获取显示图标
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T MMIMAIL_GetIconIdByState( MMIMAIL_STATE_E  state);
#endif

/*****************************************************************************/
//	Description : 生成列表内容
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_FormListBoxItem(
                                               MMIMAIL_MAIL_SUMMERY_T    *mail_ptr,
                                               MMI_CTRL_ID_T     ctrl_id,
                                               uint16            index
                                           );
                             
/*****************************************************************************/
//	Description : 设置列表项数据
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetListItemData(
                                            MMIMAIL_BOX_TYPE_E box_type, 
                                            MMI_CTRL_ID_T      ctrl_id,
                                            uint16             index
                                           );
                                         
/*****************************************************************************/
//	Description : 获取当前焦点子窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_WIN_ID_T MMIMAIL_GetCurFocusChildBoxWinId(void);

/*****************************************************************************/
//	Description : 获取当前焦点列表控件id
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T MMIMAIL_GetCurActiveListCtrlId(void);

/*****************************************************************************/
//	Description : 获取当前邮件是否被标记
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_GetCurMailIsMarked(void );

/*****************************************************************************/
//	Description : 标记当前邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurMailMarked(
                                   BOOLEAN is_marked
                                   );

/*****************************************************************************/
//	Description : 标记指定邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetMailMarked(
                                           MMIMAIL_BOX_TYPE_E   box_type,
                                           uint16               index,               
                                           BOOLEAN              is_marked
                                         );

/*****************************************************************************/
//	Description : 标记当前邮箱所有邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetAllMailMarked(
                                    MMIMAIL_BOX_TYPE_E  box_type,
                                    BOOLEAN is_marked
                                    );

/*==========================================================
 * author		: liangshun      
 * function 	: LOCAL BOOLEAN MMIMAIL_HasMailBeenMarked(MMIMAIL_BOX_TYPE_E  box_type )
 * decr		: 判断是否有邮件被标记
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/13/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_HasMailBeenMarked(MMIMAIL_BOX_TYPE_E  box_type );

/*****************************************************************************/
//	Description : 获取是否全部邮件已被标记
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsAllMailMarked(MMIMAIL_BOX_TYPE_E  box_type );
                                    
/*****************************************************************************/
//	Description : 获取当前邮箱删除邮件数目
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL uint32 MMIMAIL_GetMailboxDeleteAvailNum(MMIMAIL_BOX_TYPE_E box_type);

/*****************************************************************************/
//	Description : 删除完毕回调函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL  BOOLEAN MMIMAIL_HandleListChangeCallBack (void);

/*****************************************************************************/
//	Description : 处理MSG_MAIL_RETRATTACH_RESULT消息
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_HandleRetrAttachResult(mail_retr_result_t  *retr_result);

/*****************************************************************************/
//	Description : 检查邮件密码完毕处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CheckPwFinished(BOOLEAN is_success);

/*****************************************************************************/
//	Description : 删除邮件完毕处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_DelFinished(BOOLEAN is_success);

/*****************************************************************************/
//	Description : 恢复出厂设置结束处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ResetFinished(BOOLEAN is_success);

/*****************************************************************************/
//  Description : 邮件下载进度更新
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ProgressUpdateCallBack(mail_ProgressInfo *progress_info);

/*****************************************************************************/
//	Description : 手动发起接收邮件操作
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ReceiveMail(void);

/*****************************************************************************/
//	Description : 邮件中是否有接收邮件地址
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsHaveReceiver(MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr);

/*****************************************************************************/
//	Description : 是否允许修改密码
//	Global resource dependence : none
//	Author: yuanl
//	Note: 企业版帐号允许修改密码，个人用户不能修改密码
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsPermitModifyPassword(void);

/*****************************************************************************/
//	Description : 加载帐号列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadAccountList(
                                            MMI_CTRL_ID_T               ctrl_id,
                                            MMIMAIL_LIST_ITEM_TYPE_E    type
                                           );

/*****************************************************************************/
// 	Description : 系统设置菜单处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailSysSettingMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );

/*****************************************************************************/
// 	Description : 帮助界面处理函数
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailHelpWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );
                                             
/*****************************************************************************/
//	Description : 邮箱帐号设置处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAccountWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 设置帐号选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAccountOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : 帐号编辑处理函数
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAccountEditWinMsg(
                                                 MMI_WIN_ID_T       win_id,  
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                               );

/*****************************************************************************/
//	Description : 帐号名称编辑界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditAccountNameWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//	Description : 帐号密码编辑界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditAccountPwWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                             
/*****************************************************************************/
//	Description : 输入密码界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailInputPwWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                             
/*****************************************************************************/
//	Description : 编辑选项设置处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetEditOptionWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 设置转发/回复是否包含原邮件正文处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetIncOrgWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 设置是否添加签名处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetSignWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 编辑签名界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditSignWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                             
/*****************************************************************************/
//  Description : 设置静音时段处理函数
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailSilentTimeWinMsg(
                                                 MMI_WIN_ID_T       win_id,  
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );

/*****************************************************************************/
//  Description : 设置静音时段时间编辑界面
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditSilentTimeWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
//	Description : 附件下载选项设置处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccDlOptWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );
                                        
/*****************************************************************************/
//	Description : 设置附件自动下载处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccAutoDlWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );
                                      
/*****************************************************************************/
//	Description : 设置附件自动格式转换处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccAutoChangeWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 设置附件大小限制处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccSizeLimitWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 删除所有邮件询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            );

/*****************************************************************************/
//	Description : 恢复出厂设置，删除本地帐号及所有邮件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ResetFTSetting(MMI_WIN_ID_T win_id);
                                            
/*****************************************************************************/
//	Description : 恢复出厂设置等待处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     );

/*****************************************************************************/
//	Description : 恢复出厂设置询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailResetQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            );

/*****************************************************************************/
//	Description : 注销139邮箱询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailLogoutQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            );

/*==========================================================
 * Function     : MMIMail_NewMailArrival
 * Author	    : sunguochen      
 * Description : 新邮件到达处理
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/10/2010 
 ==========================================================*/
PUBLIC void MMIMail_NewMailArrival(mail_retr_result_t *retr_result);


/*==========================================================
 * Function     : MMIMAIL_checkAccount
 * Author	    : sunguochen      
 * Description : 检查账号情况 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/11/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_checkAccount(void);



/*==========================================================
 * Function     : MMIMAIL_checkAccount
 * Author	    : sunguochen      
 * Description : 开通手机邮箱
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/11/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_applyAccount(void);

/*==========================================================
 * Function     : MMIMAIL_logOutAccount
 * Author	    : sunguochen      
 * Description : 开通手机邮箱
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/11/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_logOutAccount(void);


/*==========================================================
 * Function     : MMIMAIL_NewArrivalMsgNum
 * Description : 新邮件个数-如果进入邮箱，清空为0
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  10/15/2010 
 ==========================================================*/
LOCAL int MMIMAIL_NewArrivalMsgNum(void);


/*==========================================================
 * Function     : MMIMAIL_SetNewArrivalMsgNum
 * Description : 新邮件个数-如果进入邮箱，清空为0
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  10/15/2010 
 ==========================================================*/
LOCAL void MMIMAIL_SetNewArrivalMsgNum(int num);

/*****************************************************************************/
//  Description : hand file list
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL void HandleCopyAccToFolderWinMsg(MMIFMM_DATA_INFO_T *list_data_ptr, uint32 index);

/*****************************************************************************/
//  Description : email send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MailSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
//欢迎界面
WINDOW_TABLE( MMIMAIL_WELCOMEWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailWelcomeWinMsg),    
    WIN_SOFTKEY(TXT_YES, TXT_NULL, TXT_COMM_NO),
    WIN_ID( MMIMAIL_WELCOME_WIN_ID ),
	WIN_TITLE(TXT_MAIL_WELCOME),
    CREATE_TEXT_CTRL(MMIMAIL_WELCOME_TEXT_CTRL_ID),
    END_WIN
};

//提示已发送开通请求界面
WINDOW_TABLE( MMIMAIL_OPENSERVICE_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailApplyAlertWinMsg),    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),
    WIN_ID( MMIMAIL_OPEN_SERVICE_WIN_ID ),
	WIN_TITLE(TXT_MAIL_PUSHMAIL),
    CREATE_TEXT_CTRL(MMIMAIL_WELCOME_TEXT_CTRL_ID),
    END_WIN
};

//邮箱列表界面
WINDOW_TABLE( MMIMAIL_MAINWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailMainMenuWinMsg),    
    WIN_STATUSBAR,
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    WIN_ID( MMIMAIL_MAIN_MENU_WIN_ID ),
	WIN_TITLE(TXT_MAIL_PUSHMAIL),
    END_WIN
};

//邮箱列表界面选项菜单
WINDOW_TABLE( MMIMAIL_MAINMENU_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMailMainMenuOptWinMsg),
    WIN_ID(MMIMAIL_MAINMENU_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_MAINMENU_OPT,MMIMAIL_MAINMENU_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//系统设置
WINDOW_TABLE( MMIMAIL_SYS_SETTING_MENU_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSysSettingMenuWinMsg),    
    WIN_ID( MMIMAIL_SYS_SETTING_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_MENU_CTRL(MENU_MAIL_SYS_SETTING, MMIMAIL_SYS_SETTING_MENU_CTRL_ID),
    END_WIN
};

//帮助界面
WINDOW_TABLE( MMIMAIL_HELP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailHelpWinMsg),    
    WIN_ID( MMIMAIL_HELP_WIN_ID ),
    WIN_TITLE(TXT_HELP),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIMAIL_HELP_TEXT_CTRL_ID),
    END_WIN
};

//邮箱主界面
WINDOW_TABLE(MMIMAIL_MAILBOX_MAIN_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleMailBoxMainWindow),    
    WIN_ID( MMIMAIL_MAILBOX_MAIN_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#ifdef MMI_GUI_STYLE_TYPICAL
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMIMAIL_MAILBOX_TAB_CTRL_ID),
#else
	CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMIMAIL_MAILBOX_TAB_CTRL_ID),
#endif
    END_WIN
};

//收件箱
WINDOW_TABLE( MMIMAIL_INBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleInBoxChildWinMsg),    
    WIN_ID( MMIMAIL_INBOX_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_INBOX_LISTBOX_CTRL_ID),
    END_WIN
};

//发件箱
WINDOW_TABLE( MMIMAIL_OUTBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleOutBoxChildWinMsg),    
    WIN_ID( MMIMAIL_OUTBOX_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_OUTBOX_LISTBOX_CTRL_ID),
    END_WIN
};

//已发邮件
WINDOW_TABLE( MMIMAIL_SENTBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSentBoxChildWinMsg),    
    WIN_ID( MMIMAIL_SENTBOX_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_SENTBOX_LISTBOX_CTRL_ID),
    END_WIN
};

//草稿箱
WINDOW_TABLE( MMIMAIL_DRAFTBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleDraftChildWinMsg),    
    WIN_ID( MMIMAIL_DRAFTBOX_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_DRAFTBOX_LISTBOX_CTRL_ID),
    END_WIN
};

//收件箱选项菜单
WINDOW_TABLE( MMIMAIL_INBOX_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInboxMenuOptWinMsg),
    WIN_ID(MMIMAIL_INBOX_MENU_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_INBOX_OPT,MMIMAIL_INBOX_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//发件箱选项菜单
WINDOW_TABLE( MMIMAIL_OUTBOX_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleOutboxMenuOptWinMsg),
    WIN_ID(MMIMAIL_OUTBOX_MENU_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_OUTBOX_OPT,MMIMAIL_OUTBOX_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//发件箱选项菜单，针对正在发送的邮件
WINDOW_TABLE( MMIMAIL_OUTBOX_OPT_EXT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleOutboxMenuOptExtWinMsg),
    WIN_ID(MMIMAIL_OUTBOX_MENU_OPT_EXT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_OUTBOX_OPT_EXT,MMIMAIL_OUTBOX_OPT_EXT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//已发件箱选项菜单
WINDOW_TABLE( MMIMAIL_SENTBOX_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSentboxMenuOptWinMsg),
    WIN_ID(MMIMAIL_SENTBOX_MENU_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_SENTBOX_OPT,MMIMAIL_SENTBOX_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//草稿箱选项菜单
WINDOW_TABLE( MMIMAIL_DRAFTBOX_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDraftboxMenuOptWinMsg),
    WIN_ID(MMIMAIL_DRAFTBOX_MENU_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_DRAFTBOX_OPT,MMIMAIL_DRAFTBOX_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//邮箱帐号设置
WINDOW_TABLE( MMIMAIL_SET_ACCOUNT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSetAccountWinMsg),    
    WIN_ID( MMIMAIL_SET_ACCOUNT_WIN_ID ),
	WIN_TITLE(TXT_MAIL_ACCOUNT_SETTING),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMAIL_SET_ACCOUNT_LISTBOX_CTRL_ID),
    END_WIN
};

//邮箱帐号设置选项菜单
WINDOW_TABLE( MMIMAIL_SET_ACCOUNT_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetAccountOptWinMsg),
    WIN_ID(MMIMAIL_SET_ACCOUNT_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_SET_ACCOUNT_OPT,MMIMAIL_SET_ACCOUNT_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//邮箱帐号编辑界面
WINDOW_TABLE( MMIMAIL_ACCOUNT_EDIT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAccountEditWinMsg),
    WIN_ID(MMIMAIL_ACCOUNT_EDIT_WIN_ID),
	WIN_TITLE(TXT_MAIL_EDIT_ACCOUNT),
    WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIMAIL_SET_RICHTEXT_CTRL_ID),
    END_WIN
};

//帐号名称编辑界面
WINDOW_TABLE(MMIMAIL_EDIT_ACCOUNT_NAME_TAB) = 
{
    WIN_TITLE(TXT_MAIL_ACCOUNT_NAME),
    WIN_FUNC((uint32)HandleMailEditAccountNameWinMsg),    
    WIN_ID(MMIMAIL_EDIT_ACCOUNT_NAME_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIMAIL_MAX_ACC_NAME_LEN, MMIMAIL_EDIT_TEXT_CTRL_ID),
    END_WIN
};

//帐号密码编辑界面
WINDOW_TABLE(MMIMAIL_EDIT_ACCOUNT_PW_TAB) = 
{
    WIN_TITLE(TXT_COMMON_PASSWORD),
    WIN_FUNC((uint32)HandleMailEditAccountPwWinMsg),    
    WIN_ID(MMIMAIL_EDIT_ACCOUNT_PW_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PASSWORD_CTRL(MMIMAIL_MAX_PASSWORD_LEN, MMIMAIL_EDIT_TEXT_CTRL_ID),
    END_WIN
};

//输入密码界面
WINDOW_TABLE(MMIMAIL_INPUT_PASSWORD_WIN_TAB) = 
{
    WIN_TITLE(TXT_MAIL_INPUT_PASSWORD),
    WIN_FUNC((uint32)HandleMailInputPwWinMsg),    
    WIN_ID(MMIMAIL_INPUT_PW_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PASSWORD_CTRL(MMIMAIL_MAX_PASSWORD_LEN, MMIMAIL_EDIT_TEXT_CTRL_ID),
    END_WIN
};

//编辑选项设置窗口
WINDOW_TABLE( MMIMAIL_SET_EDIT_OPTION_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSetEditOptionWinMsg),    
    WIN_ID( MMIMAIL_SET_EDIT_OPTION_WIN_ID ),
	WIN_TITLE(TXT_MAIL_EDIT_OPTION),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN ),
    CREATE_MENU_CTRL(MENU_MAIL_EDIT_OPT_SETTING, MMIMAIL_SET_MENU_CTRL_ID),
    END_WIN
};

//转发/回复是否包含原邮件正文
WINDOW_TABLE( MMIMAIL_SET_INC_ORG_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSetIncOrgWinMsg),    
    WIN_ID( MMIMAIL_SET_INC_ORG_WIN_ID ),
	WIN_TITLE(TXT_MAIL_INCLUDE_ORIGINAL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMAIL_SET_LISTBOX_CTRL_ID),
    END_WIN
};

//转发/回复是否包含原邮件正文
WINDOW_TABLE( MMIMAIL_SET_SIGN_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSetSignWinMsg),    
    WIN_ID( MMIMAIL_SET_SIGN_WIN_ID ),
	WIN_TITLE(TXT_COMMON_SIGNATURE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMAIL_SET_LISTBOX_CTRL_ID),
    END_WIN
};

//编辑签名窗口
WINDOW_TABLE(MMIMAIL_EDIT_SIGN_TAB) = 
{
    WIN_TITLE(TXT_MAIL_EDIT_SIGN),
    WIN_FUNC((uint32)HandleMailEditSignWinMsg),    
    WIN_ID(MMIMAIL_EDIT_SIGN_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MAIL_SIGNATURE_LEN, MMIMAIL_EDIT_TEXT_CTRL_ID),
    END_WIN
};

//设置静音时段
WINDOW_TABLE( MMIMAIL_SET_SILENT_TIME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMailSilentTimeWinMsg),
    WIN_ID(MMIMAIL_SILENT_TIME_WIN_ID),
	WIN_TITLE(TXT_COMMON_SLIENT_TIME),
    WIN_SOFTKEY(STXT_SAVE,TXT_EDIT,STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIMAIL_SET_RICHTEXT_CTRL_ID),
    END_WIN
};

//设置静音时段时间的编辑界面
WINDOW_TABLE( MMIMAIL_EDIT_SILENT_TIME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMailEditSilentTimeWinMsg ),    
    WIN_ID( MMIMAIL_EDIT_SILENT_TIME_WIN_ID ),
    WIN_TITLE( TXT_COMMON_SLIENT_TIME ),
    CREATE_EDIT_TIME_CTRL(MMIMAIL_SET_TIMEEDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//附件下载选项菜单
WINDOW_TABLE( MMIMAIL_SET_ACC_DL_OPT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSetAccDlOptWinMsg),    
    WIN_ID( MMIMAIL_SET_ACC_DL_OPT_WIN_ID ),
	WIN_TITLE(TXT_MAIL_ATTACH_DL_OPTION),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN ),
    CREATE_MENU_CTRL(MENU_MAIL_ACC_DL_SETTING, MMIMAIL_SET_MENU_CTRL_ID),
    END_WIN
};

//设置附件自动下载界面
WINDOW_TABLE( MMIMAIL_SET_ACC_AUTO_DL_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSetAccAutoDlWinMsg),    
    WIN_ID( MMIMAIL_SET_ACC_AUTO_DL_WIN_ID ),
	WIN_TITLE(TXT_COMMON_AUTO_DOWNLOAD),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMAIL_SET_LISTBOX_CTRL_ID),
    END_WIN
};

//设置附件自动格式转换界面
WINDOW_TABLE( MMIMAIL_SET_ACC_AUTO_CHANGE_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSetAccAutoChangeWinMsg),    
    WIN_ID( MMIMAIL_SET_ACC_AUTO_CHANGE_WIN_ID ),
	WIN_TITLE(TXT_MAIL_AUTO_FORMAT_CHANGE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMAIL_SET_LISTBOX_CTRL_ID),
    END_WIN
};

//设置附件大小限制界面
WINDOW_TABLE( MMIMAIL_SET_ACC_SIZE_LIMIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailSetAccSizeLimitWinMsg),    
    WIN_ID( MMIMAIL_SET_ACC_SIZE_LIMIT_WIN_ID ),
	WIN_TITLE(TXT_MAIL_SIZE_LIMIT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMAIL_SET_LISTBOX_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : Pushmail初始化
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_Init(void)
{
    SCI_MEMSET(&s_mmimail_info, 0, sizeof(MMIMAIL_MANAGE_INFO_T));

    g_mmimail_app.ProcessMsg = HandleMailTaskMsg; 
    
}

/*****************************************************************************/
//	Description : 模块初始化
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_InitModule(void)
{
    MMIMAIL_RegMenuGroup();
    MMIMAIL_RegWinIdNameArr();   
}

/*****************************************************************************/
// 	Description : Pushmail 入口
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_Enter(MAIL_ENTER_TYPE_E   enter_type, DPARAM param)
{
    BOOLEAN is_start_success = FALSE;

    if (!MMIMAIL_EnterAppCheck())
    {
        return FALSE;
    }

    s_mmimail_info.enter_type = enter_type;
    s_mmimail_info.enter_param = param;

    /* Save User param */
    if (MAIL_ENTER_NEWMAIL == enter_type && PNULL != param)
    {
        uint16 param_len = 0;

        param_len = SCI_STRLEN((char *)param);
        param_len = MIN(param_len, MMIMAIL_MAX_ADDRESS_LEN);

        s_mmimail_info.enter_param = SCI_ALLOC_APP(param_len + 1);
        if (PNULL == s_mmimail_info.enter_param)
        {
            return FALSE;
        }

        SCI_MEMSET(s_mmimail_info.enter_param, 0, param_len + 1);
        SCI_MEMCPY(s_mmimail_info.enter_param, (char *)param, param_len);
        param = s_mmimail_info.enter_param;
    }

    if (!MMIMAIL_IsReady())
    {
        if (!MMIMAIL_IsHaveLocalAccount())
        {
            MMI_STRING_T    waiting_text1 = {0};
            MMI_STRING_T    waiting_text2 = {0};

            if(mail_SettinginfoRecved())//已经获取账户信息
            {
                //未开通--提醒用户是否开通
                MMIMAIL_OpenWelcomeWin();
                return FALSE;            
            }

            //短信模块未准备好，发送短信肯定失败，提示用户稍候再试
            if (!s_mmimail_is_sms_ok)
            {
                MMIMAIL_OpenMsgBox(TXT_MAIL_SIM_NOT_READY, IMAGE_PUBWIN_WARNING);
                return FALSE;
            }
            
            //提示正在获取帐号信息，进入等待界面
            MMI_GetLabelTextByLang(TXT_MAIL_GETTING_ACCOUNT_CONFIG, &waiting_text1);
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &waiting_text2);
            MMIPUB_OpenWaitWin(2, &waiting_text1, &waiting_text2, PNULL, MMIMAIL_WAIT_GET_ACCOUNT_WIN_ID, IMAGE_NULL,
                ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_CUSTOMER, HandleWaitGetAccountWinMsg);

            //发送获取帐号信息短信
            MMIMAIL_checkAccount();
            return FALSE;
        }

        if (mail_isCardSwitch())
        {
            MMI_STRING_T    waiting_text1 = {0};
            MMI_STRING_T    waiting_text2 = {0};
            MMI_WIN_ID_T    query_win = 0;
            
            if(mail_SettinginfoRecved())//已经获取账户信息
            {
                if(mail_ExistTmpSettinginfo())
                {
                    //新卡已开通--提醒用户替换新账户
                    query_win = MMIMAIL_OVERWRITE_QUERY_WIN_ID;
                    MMIPUB_OpenQueryWinByTextId(TXT_MAIL_OVERWRITE_EXIST_ACCOUNT, IMAGE_PUBWIN_QUERY, &query_win, HandleOverwriteQueryWinMsg);
                }
                else
                {
                    //新卡未开通开通--提醒用户删除并开通
                    query_win = MMIMAIL_DEL_EXIST_ACCOUNT_QUERY_WIN_ID;
                    MMIPUB_OpenQueryWinByTextId(TXT_MAIL_DEL_EXIST_ACCOUNT, IMAGE_PUBWIN_QUERY, &query_win, HandleDelExistAccountQueryWinMsg);
                }
                
                return FALSE;
            }        

            //短信模块未准备好，发送短信肯定失败，提示用户稍候再试
            if (!s_mmimail_is_sms_ok)
            {
                MMIMAIL_OpenMsgBox(TXT_MAIL_SIM_NOT_READY, IMAGE_PUBWIN_WARNING);
                return FALSE;
            }

            //COMM_TRACE:"Mail:MMIAPIMAIL_Enter change sim and no new settings"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1677_112_2_18_2_48_28_417,(uint8*)"");
            //提示正在获取帐号信息，进入等待界面
            MMI_GetLabelTextByLang(TXT_MAIL_GETTING_ACCOUNT_CONFIG, &waiting_text1);
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &waiting_text2);
            MMIPUB_OpenWaitWin(2, &waiting_text1, &waiting_text2, PNULL, MMIMAIL_WAIT_GET_ACCOUNT_WIN_ID, IMAGE_NULL,
                ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_CUSTOMER, HandleWaitGetAccountWinMsg);

            //发送获取帐号信息短信
            MMIMAIL_checkAccount();
            return FALSE;
        }
    }

    is_start_success = MMIMAIL_Enter(enter_type, param);

    if (!is_start_success && MAIL_ENTER_NEWMAIL == enter_type && PNULL != s_mmimail_info.enter_param)
    {
        SCI_FREE(s_mmimail_info.enter_param);
        s_mmimail_info.enter_param = PNULL;
    }

    return is_start_success;
}

/*****************************************************************************/
//	Description : 退出手机邮箱
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_Exit(void)
{
    if ((!MMK_IsOpenWin(MMIMAIL_MAIN_MENU_WIN_ID)) 
        && (!MMK_IsOpenWin(MMIMAIL_MAILBOX_MAIN_WIN_ID)))
    {
        BOOLEAN is_need_close = MMK_IsNeedCloseWin(MMIMSG_MAIN_MENU_WIN_ID);
        
        if ((!MMIMAIL_IsSmsOpen()) //短信主窗口未打开
            || !is_need_close) //短信主窗口已打开但是正在关闭
        {
            //邮箱主界面和邮件列表界面都不存在的情况下，需要单独调用释放函数
            MMIMAIL_Exit();
        }
    }
    
    return MMK_CloseMiddleWin(MMIMAIL_START_WIN_ID, MMIMAIL_END_WIN_ID);
}

/*****************************************************************************/
// 	Description : 设置资费信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SetChargeInfo(MMI_STRING_T *charge_info)
{
    BOOLEAN is_get_success = FALSE;

    if (MMK_IsOpenWin(MMIMAIL_WELCOME_WIN_ID))
    {
        MMK_SendMsg(MMIMAIL_WELCOME_WIN_ID, MSG_MAIL_UPDATE_CHARGE_INFO, (DPARAM)charge_info);
    }
    
    return is_get_success;
}

/*****************************************************************************/
// 	Description : 获取帐号信息回调
//	Global resource dependence : none
//  Author:yuanl
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_GetAccountCallback(
                                                    BOOLEAN is_open, //当前卡是否开通了139邮箱
                                                    BOOLEAN is_white_sim //当前卡是否是白卡(即欠费卡)
                                                    )
{
    MMI_WIN_ID_T query_win = MMIMAIL_OVERWRITE_QUERY_WIN_ID;
    
    if (MMK_IsOpenWin(MMIMAIL_WAIT_GET_ACCOUNT_WIN_ID))
    {
        //关闭等待窗口
        MMK_CloseWin(MMIMAIL_WAIT_GET_ACCOUNT_WIN_ID);
        
        if(is_white_sim)
        {
            //COMM_TRACE:"Mail: MMIAPIMAIL_GetAccountCallback cur sim is a white sim card!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1757_112_2_18_2_48_28_418,(uint8*)"");
            MMIMAIL_OpenMsgBox(TXT_MAIL_CHECK_SIM_STATE, IMAGE_PUBWIN_WARNING);
        }
        else
        {
            if (is_open)
            {
                //已经开通
                if (MMIMAIL_IsHaveLocalAccount())
                {
                    //提示用户是否覆盖现有帐号
                    MMIPUB_OpenQueryWinByTextId(TXT_MAIL_OVERWRITE_EXIST_ACCOUNT, IMAGE_PUBWIN_QUERY, &query_win, HandleOverwriteQueryWinMsg);
                }
                else
                {
                    //保存配置信息
                    MMIMAIL_SaveAccountConfig();
                
                    //重新进入邮箱
                    MMIMAIL_Enter(s_mmimail_info.enter_type, s_mmimail_info.enter_param);
                }
            }
            else
            {
                if (MMIMAIL_IsHaveLocalAccount())
                {
                    //先提示删除旧帐号再开通
                    query_win = MMIMAIL_DEL_EXIST_ACCOUNT_QUERY_WIN_ID;
                    MMIPUB_OpenQueryWinByTextId(TXT_MAIL_DEL_EXIST_ACCOUNT, IMAGE_PUBWIN_QUERY, &query_win, HandleDelExistAccountQueryWinMsg);
                }
                else
                {
                    //进入开通流程
                    MMIMAIL_OpenWelcomeWin();
                }
            }
        }
    }
    else
    {
        if (is_open)
        {
            //已经开通
            if (MMIMAIL_IsHaveLocalAccount() == FALSE)
            {
                //保存配置信息
                MMIMAIL_SaveAccountConfig();
            }
        }        
    }
    
    return;
}

/*****************************************************************************/
//  Description : 如果没有新邮件了则关闭新邮件提示窗口
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_CloseNewMailPromptWin(void)
{
    uint16 unread_num = 0;
    
    unread_num = MMIMAIL_GetInboxBoxUnreadNum();
    
	if ((0 == unread_num) 
        && (MMK_IsOpenWin(MMIMAIL_NEW_MAIL_ALERT_WIN_ID)))
    {
        MMK_CloseWin(MMIMAIL_NEW_MAIL_ALERT_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : 是否有未读邮件
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsHaveUnreadMail(void)
{
    uint16 unread_num = 0;
    BOOLEAN is_have = FALSE;

    unread_num = MMIMAIL_GetInboxBoxUnreadNum();

    //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_IsHaveUnreadMail: unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1839_112_2_18_2_48_28_419,(uint8*)"d", unread_num);
    
	if (unread_num > 0) 
    {
        is_have = TRUE;
    }

	return is_have;
}

/*****************************************************************************/
//  Description : 邮件是否满
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsMailFull(void)
{
    BOOLEAN is_full = FALSE;

    is_full = mail_IsMailBoxFull();
    
	return is_full;
}

/*****************************************************************************/
//  Description : 设置短信模块准备就绪
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_SetSmsOk(BOOLEAN is_sms_ok)
{
    //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SetSmsOk: is_sms_ok = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1868_112_2_18_2_48_28_420,(uint8*)"d", is_sms_ok);
    s_mmimail_is_sms_ok = is_sms_ok;
}

/*****************************************************************************/
//	Description : 邮件是否准备就绪
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsReady( void )
{
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_IsReady: return = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1878_112_2_18_2_48_28_421,(uint8*)"d", s_mmimail_info.is_ready);
    return s_mmimail_info.is_ready;
}

/*****************************************************************************/
//	Description : 获取邮箱启动类型
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MAIL_ENTER_TYPE_E MMIMAIL_GetOpenType( void )
{
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetOpenType: return = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1888_112_2_18_2_48_28_422,(uint8*)"d", s_mmimail_info.enter_type);
    return s_mmimail_info.enter_type;
}

/*****************************************************************************/
//	Description : 获取邮箱启动参数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC DPARAM MMIMAIL_GetOpenParam( void )
{
    if (PNULL != s_mmimail_info.enter_param)
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetOpenParam: return = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1900_112_2_18_2_48_28_423,(uint8*)"d", s_mmimail_info.enter_param);
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetOpenParam: return 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1904_112_2_18_2_48_28_424,(uint8*)"");
    }
    return s_mmimail_info.enter_param;
}

/*****************************************************************************/
// 	Description : 获取当前默认帐号ID
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMAIL_GetDefAccountId(void)
{
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetDefAccountId: return = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_1915_112_2_18_2_48_28_425,(uint8*)"d", s_mmimail_info.def_account.accountid);
    return s_mmimail_info.def_account.accountid;
}

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC MMIMAIL_ACCOUNT_INFO_T* MMIMAIL_GetDefAccountInfo(void)
 * decr		: 获取指向当前默认账户信息的结构指针
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/16/2011 
 ==========================================================*/
PUBLIC MMIMAIL_ACCOUNT_INFO_T* MMIMAIL_GetDefAccountInfo(void)
{
    return &s_mmimail_info.def_account;
}

/*****************************************************************************/
// 	Description : 获取当前默认帐号邮箱信息指针
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_BOX_INFO_T * MMIMAIL_GetDefAccountMailboxInfo(void)
{
    return &(s_mmimail_info.def_account_box_info);
}

/*****************************************************************************/
// 	Description : 获取当前正在阅读的邮件
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MAIL_MESSAGE_T * MMIMAIL_GetCurDispMail(void)
{
    return s_mmimail_info.cur_disp_mail;
}

/*****************************************************************************/
// 	Description : 设置当前正在阅读的邮件
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurDispMail(MAIL_MESSAGE_T *mail_ptr)
{
    s_mmimail_info.cur_disp_mail = mail_ptr;
}

/*****************************************************************************/
// 	Description : 获取当前所在邮箱
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_BOX_TYPE_E MMIMAIL_GetCurBoxType(void)
{
    return s_mmimail_info.cur_box_type;
}

/*****************************************************************************/
// 	Description : 设置当前所在邮箱
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurBoxType(MMIMAIL_BOX_TYPE_E box_type)
{
    s_mmimail_info.cur_box_type = box_type;
}

/*****************************************************************************/
// 	Description : 获取当前浏览邮件的record id
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMAIL_GetCurDispMailRecordId(void)
{
    return s_mmimail_info.cur_disp_record_id;
}

/*****************************************************************************/
// 	Description : 设置当前浏览邮件的record id
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurDispMailRecordId(uint32 record_id)
{
    s_mmimail_info.cur_disp_record_id = record_id;
}

/*****************************************************************************/
// 	Description : 获取当前打开编辑窗口的类型
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_OPEN_EDIT_WIN_TYPE_E MMIMAIL_GetOpenEditwinType(void)
{
    return s_mmimail_info.open_editwin_type;
}

/*****************************************************************************/
// 	Description : 设置当前打开编辑窗口的类型
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetOpenEditwinType(MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type)
{
    s_mmimail_info.open_editwin_type = open_type;
}

/*****************************************************************************/
//	Description : 获取当前正在操作的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_MAIL_SUMMERY_T *MMIMAIL_GetCurOperationMail(void)
{
    return s_mmimail_info.cur_oper_mail;
}

/*****************************************************************************/
//	Description : 设置当前正在操作的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurOperationMail(MMIMAIL_MAIL_SUMMERY_T * mail_ptr)
{
    s_mmimail_info.cur_oper_mail = mail_ptr;
}

/*****************************************************************************/
//	Description : 获取当前邮件是否是通过id发送的
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsSendingById(void)
{
    return s_mmimail_info.is_mail_sendby_id;
}

/*****************************************************************************/
//	Description : 设置当前邮件是通过id发送的
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetSendingById(BOOLEAN is_sendby_id)
{
    s_mmimail_info.is_mail_sendby_id = is_sendby_id;
}

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC void MMIMAIL_SetDelAccountOperFlag(BOOLEAN is_del_account_oper_by_bomb)
 * decr		: 设置通过炸弹消息删除账户操作的标志位
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/18/2011 
 ==========================================================*/
PUBLIC void MMIMAIL_SetDelAccountOperFlag(BOOLEAN is_del_account_oper_by_bomb)
{
    s_mmimail_info.is_del_account_oper_by_bomb = is_del_account_oper_by_bomb;
}

/*****************************************************************************/
//	Description : 获取当前操作邮件对应在列表中的index
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMAIL_GetCurOperationMailIndex(MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr)
{
    MMIMAIL_MAIL_SUMMERY_T *head_ptr = PNULL;
    MMIMAIL_MAIL_SUMMERY_T *cur_ptr = PNULL;
    uint16 list_index = 0;

    if (PNULL != cur_mail_ptr)
    {
        head_ptr = MMIMAIL_GetMailListHead(MMIMAIL_GetCurBoxType());
        
        cur_ptr = head_ptr;
        while (cur_ptr != cur_mail_ptr)
        {
            if (PNULL != cur_ptr)
            {
                cur_ptr = cur_ptr->next_ptr;
                list_index ++;
            }
            else
            {
                break;    
            }
        }
    }
    
    return list_index;
}

/*****************************************************************************/
//	Description : 设置当前正在操作的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:通过列表的index设置
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurOperationMailByIndex(MMIMAIL_BOX_TYPE_E box_type, uint32 index)
{
    MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr = PNULL;
    
    cur_mail_ptr = MMIMAIL_GetCurMailPtr(box_type,index);
    
    s_mmimail_info.cur_oper_mail = cur_mail_ptr;
}

/*****************************************************************************/
// 	Description : 获取邮件状态
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_STATE_E MMIMAIL_GetMailState(MMIMAIL_MAIL_SUMMERY_T *mail_ptr)
{
    MMIMAIL_STATE_E state = MMIMAIL_STATE_NULL;
    
    if (PNULL != mail_ptr)
    {
        state = mail_ptr->state;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetMailState: mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2121_112_2_18_2_48_28_426,(uint8*)"");
    }
    
    return state;
}

/*****************************************************************************/
// 	Description : 设置邮件状态
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetMailState(
                                        MMIMAIL_MAIL_SUMMERY_T *mail_ptr, 
                                        MMIMAIL_STATE_E state
                                      )
{
    if (PNULL != mail_ptr)
    {
        mail_ptr->state = state;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_SetMailState: mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2142_112_2_18_2_48_28_427,(uint8*)"");
    }
}

/*****************************************************************************/
// 	Description : 是否需要排序
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsNeedSort(void)
{
    return s_mmimail_info.is_need_sort;
}

/*****************************************************************************/
// 	Description : 设置是否需要排序
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetNeedSort(BOOLEAN is_need_sort)
{
    s_mmimail_info.is_need_sort = is_need_sort;
}

/*****************************************************************************/
//	Description : 打开询问是否删除窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_OpenDelteQueryWin(void)
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 delete_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T tmp_prompt = {0};
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;
    uint32 delete_mail_num = 0;

    if (s_mmimail_info.marked_mail_num > 0)
    {
        delete_mail_num = MMIMAIL_GetMailboxDeleteAvailNum(MMIMAIL_GetCurBoxType());

        MMI_GetLabelTextByLang(TXT_DELETE, &prompt_str1);
        
        disp_num_len = sprintf((char *)delete_msg_num, "%ld", delete_mail_num);

        MMI_STRNTOWSTR(disp_num, disp_num_len, (uint8 *)delete_msg_num, disp_num_len, disp_num_len);

        if (delete_mail_num > 1)
        {
            MMI_GetLabelTextByLang(TXT_MAIL_MAIL_ITEMS, &tmp_prompt);
        }
        else if (delete_mail_num == 1)
        {
            MMI_GetLabelTextByLang(TXT_MAIL_MAIL_ITEM, &tmp_prompt);
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_OpenDelteQueryWin: delete_mail_num = 0,no need delete!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2200_112_2_18_2_48_29_428,(uint8*)"");
            return;
        }

        // the total string wstr_len
        disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
        
        // allocate memory
        prompt_str2.wstr_ptr = SCI_ALLOC_APP((disp_str_len+1)*sizeof(wchar));
        SCI_MEMSET(prompt_str2.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        // copy the num
        MMI_WSTRNCPY(prompt_str2.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);

        prompt_str2.wstr_len = disp_num_len;
        
        // copy the prompt string
        MMI_WSTRNCPY(
            &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]),
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr, 
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
            );
        prompt_str2.wstr_len += tmp_prompt.wstr_len;

        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWinMsg );
        if (PNULL != prompt_str2.wstr_ptr)
        {
            SCI_FREE(prompt_str2.wstr_ptr);
            prompt_str2.wstr_ptr = PNULL;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MAIL_DEL_CUR_MAIL, &prompt_str1);
        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWinMsg );
    }
}

/*****************************************************************************/
//	Description : Query delete all
//	Global resource dependence : none
//	Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_OpenDelteAllQueryWin(void)
{
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_ALL_QUERY_WIN_ID;
    uint32 delete_mail_num = 0;

    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
    }

    delete_mail_num = MMIMAIL_GetBoxTotalNumber(MMIMAIL_GetCurBoxType());
    if (0 == delete_mail_num)
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_OpenDelteQueryWin: delete_mail_num = 0,no need delete!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2257_112_2_18_2_48_29_429,(uint8*)"");
        return;
    }

    MMIPUB_OpenQueryWinByTextId(TXT_DELALL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelAllInCurBoxQueryWinMsg);
}

/*****************************************************************************/
//	Description : 增加空列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_AppendEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,         //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: 
                             MMI_TEXT_ID_T    left_softkey_id  //IN:
                             )
{
    MMI_STRING_T empty_str = {0};

    MMI_GetLabelTextByLang(empty_txt_id, &empty_str);
    MMIMAIL_AppendListItem(
                    empty_str.wstr_ptr,
                    empty_str.wstr_len,
                    ctrl_id,
                    left_softkey_id,
                    TXT_NULL,
                    0,
                    GUIITEM_STYLE_ONE_LINE_TEXT,
                    MMIMAIL_APPEND_LIST_ITEM_TYPE
                    );
}

/*****************************************************************************/
// 	Description : 减少默认帐号收件箱未读邮件数目
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_DecreaseInboxBoxUnreadNum(void)
{
    if (s_mmimail_info.def_account_box_info.total_inbox_unread_num > 0)
    {
        s_mmimail_info.def_account_box_info.total_inbox_unread_num--;
    }
}

/*****************************************************************************/
//	Description : 打开消息提示框
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_OpenMsgBox( 
                       MMI_TEXT_ID_T     text_id,    //IN:
                       MMI_IMAGE_ID_T    image_id    //IN:
                       )
{
    if (MMK_IsOpenWin(MMIMAIL_MSGBOX_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_MSGBOX_WIN_ID);
    }

    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
//	Description : 获取与record id对应的邮件指针
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_MAIL_SUMMERY_T  *MMIMAIL_GetMailPtrByRecordId(
                                                            uint32 record_id
                                                           )
{
    MMIMAIL_MAIL_SUMMERY_T *head_ptr = PNULL;
    MMIMAIL_MAIL_SUMMERY_T *cur_ptr = PNULL;
    MMIMAIL_BOX_TYPE_E  box_type = MMIMAIL_BOX_NONE;
    
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetMailPtrByRecordId: record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2330_112_2_18_2_48_29_430,(uint8*)"d", record_id);
    
    for (box_type = MMIMAIL_INBOX; box_type < MMIMAIL_BOX_MAX; box_type++)
    {
        head_ptr = MMIMAIL_GetMailListHead(box_type);
        
        cur_ptr = head_ptr;
        while (cur_ptr != PNULL)
        {
            if (cur_ptr->record_id == record_id)
            {
                MMIMAIL_SetCurBoxType(box_type); //保存当前所在邮箱信息
                return cur_ptr;
            }
            cur_ptr = cur_ptr->next_ptr;
        }
    }
    
    return PNULL;
}

/*****************************************************************************/
//  Description : 判断短信模块是否启动
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsSmsOpen(void)
{
    BOOLEAN result = FALSE;
    
    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        result = TRUE;
    }
    
    //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_IsSmsOpen: is_sms_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2364_112_2_18_2_48_29_431,(uint8*)"d", result);
    
    return result;
}

/*****************************************************************************/
// 	Description : 打开欢迎界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_OpenWelcomeWin(void)
{
    return MMK_CreateWin((uint32 *)MMIMAIL_WELCOMEWIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : 打开邮箱列表界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_OpenMailListWin(void)
{
    if (MMIMAIL_IsReady())
    {
        MMK_CreateWin((uint32 *)MMIMAIL_MAINWIN_TAB, PNULL);
    }
    else
    {
        MMIMAIL_OpenInitWaitingWindow();
        
        //获取默认帐号的邮箱信息
        MMIMAIL_GetAccountMailBoxInfo(&s_mmimail_info.def_account, &s_mmimail_info.def_account_box_info);
    }
    return TRUE;
}

/*****************************************************************************/
//	Description : 进入相应的邮箱
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_EnterMailBox(
                                       MMIMAIL_BOX_TYPE_E        box_type
                                       )
{
    if (MMIMAIL_IsReady())
    {
        MMK_CreateParentWin((uint32 *)MMIMAIL_MAILBOX_MAIN_WIN_TAB, PNULL);    
        MMK_CreateChildWin(MMIMAIL_MAILBOX_MAIN_WIN_ID, (uint32 *)MMIMAIL_INBOX_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMIMAIL_MAILBOX_MAIN_WIN_ID, (uint32 *)MMIMAIL_OUTBOX_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMIMAIL_MAILBOX_MAIN_WIN_ID, (uint32 *)MMIMAIL_SENTBOX_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMIMAIL_MAILBOX_MAIN_WIN_ID, (uint32 *)MMIMAIL_DRAFTBOX_CHILD_WIN_TAB, NULL);
        switch(box_type)
        {
        case MMIMAIL_INBOX:
            MMK_SetChildWinFocus(MMIMAIL_MAILBOX_MAIN_WIN_ID, MMIMAIL_INBOX_CHILD_WIN_ID);
            break;
            
        case MMIMAIL_OUTBOX:
            MMK_SetChildWinFocus(MMIMAIL_MAILBOX_MAIN_WIN_ID, MMIMAIL_OUTBOX_CHILD_WIN_ID);
            break;
            
        case MMIMAIL_SENTBOX:
            MMK_SetChildWinFocus(MMIMAIL_MAILBOX_MAIN_WIN_ID, MMIMAIL_SENTBOX_CHILD_WIN_ID);
            break;
            
        case MMIMAIL_DRAFTBOX:
            MMK_SetChildWinFocus(MMIMAIL_MAILBOX_MAIN_WIN_ID, MMIMAIL_DRAFTBOX_CHILD_WIN_ID);
            break;
            
        default:
            MMK_SetChildWinFocus(MMIMAIL_MAILBOX_MAIN_WIN_ID, MMIMAIL_INBOX_CHILD_WIN_ID);
            break;
         }

         MMIMAIL_SetCurBoxType(box_type);
    }
    else
    {
        MMIMAIL_OpenInitWaitingWindow();
        
        //获取默认帐号的邮箱信息
        MMIMAIL_GetAccountMailBoxInfo(&s_mmimail_info.def_account, &s_mmimail_info.def_account_box_info);
    }
}

/*****************************************************************************/
// 	Description : 进入编辑界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterEditWin(MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type, uint32 record_id)
{
    if (MMIMAIL_IsReady())
    {
        if (MMIMAIL_OPEN_TYPE_NEW_MAIL == open_type)
        {
            MMIAPIMAIL_OpenEditWin(MMIMAIL_OPEN_TYPE_NEW_MAIL, (void *)record_id);
            if (s_mmimail_info.enter_param)
            {
                SCI_FREE(s_mmimail_info.enter_param);
                s_mmimail_info.enter_param = PNULL;
            }
        }
        else
        {
            MMIAPIMAIL_ReadMail(record_id);
        }
    }
    else
    {
        MMIMAIL_OpenInitWaitingWindow();
        
        //获取默认帐号的邮箱信息
        MMIMAIL_GetAccountMailBoxInfo(&s_mmimail_info.def_account, &s_mmimail_info.def_account_box_info);
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : 进入设置界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterSettingWin(void)
{
    if (MMIMAIL_IsReady())
    {
        MMK_CreateWin((uint32 *)MMIMAIL_SYS_SETTING_MENU_WIN_TAB, PNULL);
    }
    else
    {
        MMIMAIL_OpenInitWaitingWindow();
        
        //获取默认帐号的邮箱信息
        MMIMAIL_GetAccountMailBoxInfo(&s_mmimail_info.def_account, &s_mmimail_info.def_account_box_info);
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : 进入阅读界面
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterReadWin(uint32 record_id)
{
    if (MMIMAIL_IsReady())
    {
        MMIAPIMAIL_ReadMail(record_id);
    }
    else
    {
        MMIMAIL_OpenInitWaitingWindow();
        
        //获取默认帐号的邮箱信息
        MMIMAIL_GetAccountMailBoxInfo(&s_mmimail_info.def_account, &s_mmimail_info.def_account_box_info);
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : 进入应用检查提示
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EnterAppCheck(void)
{
	MMI_TEXT_ID_T 	text_id = TXT_NULL;
	SIM_STATUS_E    sim_status = MMIAPIPHONE_GetSimStatus(MMIAPISET_GetActiveSim());

	text_id = s_sim_status_string[sim_status]; //SIM卡状态检查
	if (TXT_NULL != text_id)
    {
        MMIMAIL_OpenMsgBox(text_id, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }

    if (!MMIMAIL_IsCmccSimCard() && !MMIAPISET_GetFlyMode()) //非中国移动SIM卡且非飞行模式
    {
        MMIMAIL_OpenMsgBox(TXT_MAIL_NONE_CMCC_SIMCARD, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }

    if (MMIAPISET_GetFlyMode()) //飞行模式下无帐号或换卡后都不进入应用
    {
        uint32 acc_num = mail_GetAccountNum();

        if ((0 == acc_num) || mail_isCardSwitch())
        {
            MMIMAIL_OpenMsgBox(TXT_MAIL_NOT_AVAILABLE, IMAGE_PUBWIN_WARNING);
            return FALSE;
        }
    }
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************/
// Description : 判断是否是中国移动的SIM卡
// Global resource dependence:
// Author: yuanl
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsCmccSimCard(void)
{
    BOOLEAN                     is_cmcc = FALSE;
    PHONE_SERVICE_STATUS_T      service_status= {0};   

    //get single card mnc value
    MMIAPIPHONE_GetCurServiceStatus(MMIAPISET_GetActiveSim(),&service_status);
    if( (MMIMAIL_CMCC_MCC_NUM == service_status.plmn.mcc) &&
        ((MMIMAIL_CMCC_MNC_NUM_00 == service_status.plmn.mnc )
        ||(MMIMAIL_CMCC_MNC_NUM_02 == service_status.plmn.mnc )
        ||(MMIMAIL_CMCC_MNC_NUM_07 == service_status.plmn.mnc ))
        )
    {
        is_cmcc = TRUE;
    }
    else
    {
        is_cmcc = FALSE;
    }
        
    return(is_cmcc);
}

/*****************************************************************************/
// 	Description : 是否有保存的本地帐号
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsHaveLocalAccount(void)
{
    BOOLEAN is_have_account = FALSE;

    //调用接口
    is_have_account = mail_ExistLocalAccount();
    
    return is_have_account;
}

/*****************************************************************************/
// 	Description : 获取默认帐号信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_GetDefAccount(
                                                MMIMAIL_ACCOUNT_INFO_T *account_info //out
                                            )
{
    BOOLEAN is_get_success = FALSE;

    if(PNULL == account_info)
    {
        //SCI_TRACE_LOW:"MMIMAIL_GetDefAccount account_info is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2613_112_2_18_2_48_29_432,(uint8*)"");
        return FALSE;
    }
    
    //调用接口获取帐号信息
    SCI_MEMSET(account_info, 0, sizeof(MMIMAIL_ACCOUNT_INFO_T));

    is_get_success = mail_GetDefAccount(account_info);

    return is_get_success;
}

/*****************************************************************************/
// 	Description : 获取指定帐号的邮箱信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_GetAccountMailBoxInfo(
                                                        MMIMAIL_ACCOUNT_INFO_T *account_info, //in
                                                        MMIMAIL_BOX_INFO_T     *mailbox_info  //out
                                                      )
{
    BOOLEAN is_get_success = FALSE;

    if(PNULL == account_info ||PNULL == mailbox_info)
    {
        //SCI_TRACE_LOW:"MMIMAIL_GetDefAccount account_info OR mailbox_info is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2638_112_2_18_2_48_29_433,(uint8*)"");
        return FALSE;
    }
    
    //读取全部邮件信息到全局变量
    //异步处理，读取完毕后关闭等待窗口
    mail_GetlHeadInfoStart(account_info->accountid, (void *)mailbox_info);
    
    is_get_success = TRUE;
    
    return is_get_success;
}

/*****************************************************************************/
// 	Description : 获取资费信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_GetChargeInfo(void)
{
    BOOLEAN is_get_success = FALSE;

    //发送短信或命令获取资费信息
    
    return is_get_success;
}

/*****************************************************************************/
//	Description : 删除所有标记的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_DelAllMarkedMsg( 
                      MMIMAIL_BOX_TYPE_E    box_type
                      )
{
    mail_StartDelMail();
}

/*****************************************************************************/
//	Description : 删除选中邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_OperateDelorDelAll(void)
{
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T win_id = MMIMAIL_DEL_WAITING_WIN_ID;

    //U盘使用过程中不能删除邮件
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
    	return;
    }
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandleDelWaitWinMsg);

    if (0 == s_mmimail_info.marked_mail_num)
    {
        //如果没有标记的邮件，则删除当前选中的邮件
        MMIMAIL_SetCurMailMarked(TRUE);
    }
    MMK_PostMsg(win_id, MSG_MAIL_DELETE_NEXT, PNULL,0);
}

/*****************************************************************************/
//	Description : 退出邮箱
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_Exit(void)
{

    LOCAL MMIMAIL_BOX_INFO_T      def_account_box_info = {0};
    
    SCI_MEMSET(&def_account_box_info, 0, sizeof(MMIMAIL_BOX_INFO_T));
    memcpy(&def_account_box_info, &(s_mmimail_info.def_account_box_info), sizeof(MMIMAIL_BOX_INFO_T));

    //add by lixu
    mail_fldrMgrQuit(&def_account_box_info);
    
    SCI_MEMSET(&s_mmimail_info, 0, sizeof(MMIMAIL_MANAGE_INFO_T));

    //打印mail未释放的内存
    comm_memory_unfree(); 

    //用户已经进入手机邮箱，认为已知新邮件
    MMIMAIL_SetNewArrivalMsgNum(0);    
    return;
}

/*****************************************************************************/
//	Description : 打开等待初始化窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_OpenInitWaitingWindow(void)
{
    MMI_STRING_T waiting_text = {0};

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &waiting_text);
    
    MMIPUB_OpenWaitWin(1, &waiting_text, PNULL, PNULL, MMIMAIL_INIT_WAITING_WIN_ID, IMAGE_NULL,
        ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleInitWaitingWinMsg);
}

/*****************************************************************************/
//	Description : 关闭等待初始化窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CloseInitWaitingWindow(BOOLEAN is_success)
{
    if(MMK_IsOpenWin(MMIMAIL_INIT_WAITING_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_INIT_WAITING_WIN_ID);

        if (is_success)
        {
            s_mmimail_info.is_ready = TRUE;

            //根据启动类型进入不同的界面
            if (MAIL_ENTER_MAINMENU == s_mmimail_info.enter_type)
            {
                MMIMAIL_OpenMailListWin();
            }
            else if (MAIL_ENTER_INBOX == s_mmimail_info.enter_type)
            {
                MMIMAIL_EnterMailBox(MMIMAIL_INBOX);
            }
            else if (MAIL_ENTER_NEWMAIL == s_mmimail_info.enter_type)
            {
                //进入新建邮件界面
                MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_NEW_MAIL, (uint32)s_mmimail_info.enter_param);
            }
            else if (MAIL_ENTER_REPLYMAIL == s_mmimail_info.enter_type)
            {
                //进入回复邮件界面
                MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_REPLY, (uint32)s_mmimail_info.enter_param);
            }
            else if (MAIL_ENTER_FORWARDMAIL == s_mmimail_info.enter_type)
            {
                //进入转发编辑界面
                MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_FORWARD_ALL, (uint32)s_mmimail_info.enter_param);
            }
            else if (MAIL_ENTER_EDITMAIL == s_mmimail_info.enter_type)
            {
                //进入编辑原始邮件界面
                MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_ORIGINAL, (uint32)s_mmimail_info.enter_param);
            }
            else if (MAIL_ENTER_SETTINGWIN == s_mmimail_info.enter_type)
            {
                //进入设置界面
                MMIMAIL_EnterSettingWin();
            }
            else if (MAIL_ENTER_READWIN == s_mmimail_info.enter_type)
            {
                //进入阅读界面
                MMIMAIL_EnterReadWin((uint32)s_mmimail_info.enter_param);
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_CloseInitWaitingWindow: s_mmimail_info.enter_type = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2798_112_2_18_2_48_30_434,(uint8*)"d", s_mmimail_info.enter_type);
            }
        }
        else
        {
            s_mmimail_info.enter_type = MAIL_ENTER_NULL;
            MMIMAIL_OpenMsgBox(TXT_COMM_INIT_FAIL, IMAGE_PUBWIN_WARNING);
        }
    }

    if (MAIL_ENTER_NEWMAIL == s_mmimail_info.enter_type && s_mmimail_info.enter_param)
    {
        SCI_FREE(s_mmimail_info.enter_param);
        s_mmimail_info.enter_param = PNULL;
    }
}

/*****************************************************************************/
//	Description : 等待获取帐号配置信息窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitGetAccountWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    static uint8 wait_timer_id = 0;
    uint32 time_out = 60*1000;//最多超时等待1分钟

    //COMM_TRACE:"MMIMAIL==> HandleWaitGetAccountWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2829_112_2_18_2_48_30_435,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_RETURN, FALSE);
        wait_timer_id = MMK_CreateWinTimer(win_id, time_out, FALSE);//3 
        break;

    case MSG_TIMER:
        if(*(uint8*)param == wait_timer_id)//解决MS00225681
        {
            MMIPUB_OpenAlertWarningWin(TXT_MAIL_GET_ACCOUNT_CONFIG_FAIL);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(wait_timer_id);
        wait_timer_id = 0;
        break;
        
//    case MSG_APP_OK:
//    case MSG_APP_WEB:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//	Description : 是否覆盖现有帐号窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOverwriteQueryWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //COMM_TRACE:"MMIMAIL==> HandleOverwriteQueryWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2885_112_2_18_2_48_30_436,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        //先删除旧卡所有帐号及所有邮件
        MMIMAIL_ResetFTSetting(MMIMAIL_OVERWRITE_WAITING_WIN_ID);    

        //关闭询问窗口
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//	Description : 是否删除原有帐号窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelExistAccountQueryWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param        // 相应消息的参数
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //COMM_TRACE:"MMIMAIL==> HandleDelExistAccountQueryWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2924_112_2_18_2_48_30_437,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        //先删除旧卡所有帐号及所有邮件
        MMIMAIL_ResetFTSetting(MMIMAIL_DEL_EXIST_ACCOUNT_WAIT_WIN_ID);    

        //关闭询问窗口
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        //不删除旧帐号，直接进入开通界面
        MMIMAIL_OpenWelcomeWin();
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}
/*****************************************************************************/
//	Description : 等待初始化窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInitWaitingWinMsg(
                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                       DPARAM             param       // 相应消息的参数
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    //COMM_TRACE:"MMIMAIL==> HandleInitWaitingWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_2964_112_2_18_2_48_30_438,(uint8*)"d", msg_id);

    switch (msg_id)
    {
//    case MSG_APP_OK:
//    case MSG_APP_WEB:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);

        if (MMK_IsOpenWin(MMIMAIL_EDIT_ACCOUNT_PW_WIN_ID))
        {
            //帐号编辑界面，修改密码后同步密码窗口
            //退出同步界面，直接关闭等待窗口即可
        }
        else if (MMK_IsOpenWin(MMIMAIL_SET_ACCOUNT_WIN_ID))
        {
            uint32 acc_num = mail_GetAccountNum();

            if (1 == acc_num)
            {
                //当前只有一个帐号，且和服务器同步密码时退出则同时退出邮箱
                MMIAPIMAIL_Exit();
            }
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//	Description : 删除邮件询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;    

    //COMM_TRACE:"MMIMAIL==> HandleDelQueryWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3017_112_2_18_2_48_30_439,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        MMIMAIL_OperateDelorDelAll();
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//	Description : Query delete all func
//	Global resource dependence : none
//	Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllInCurBoxQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //COMM_TRACE:"MMIMAIL==> HandleDelAllInCurBoxQueryWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3054_112_2_18_2_48_30_440,(uint8*)"d", msg_id);

    switch (msg_id)
    {

    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        MMIMAIL_SetAllMailMarked(MMIMAIL_GetCurBoxType(), TRUE);
        MMIMAIL_OperateDelorDelAll();
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//	Description : 删除邮件等待处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T cur_ctrl_id = MMIMAIL_GetCurActiveListCtrlId();

    //COMM_TRACE:"MMIMAIL==> HandleDelWaitWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3093_112_2_18_2_48_30_441,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MSG_MAIL_DELETE_NEXT: 
        MMIMAIL_DelAllMarkedMsg(MMIMAIL_GetCurBoxType());
        break;

    case MSG_CLOSE_WINDOW:
        //去掉所有邮件的选中标记
        s_mmimail_info.marked_mail_num = 0;
        if (!MMK_IsOpenWin(MMIMAIL_READ_WIN_ID) && MMK_IsOpenWin(MMIMAIL_MAILBOX_MAIN_WIN_ID))
        {
            GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
            MMIMAIL_SetAllMailMarked(MMIMAIL_GetCurBoxType(), FALSE);
        }
        if (MMK_IsOpenWin(MMIMAIL_READ_WIN_ID))
        {
            MMK_CloseWin(MMIMAIL_READ_WIN_ID);
        }
        MMIMAIL_HandleListChangeCallBack(); //删除完毕刷新list
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        mail_CancleDelMail(); //通知mail线程取消删除操作
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//	Description : 处理mail线程发来的消息
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailTaskMsg(
                               PWND        app_ptr,
                               uint16      msg_id,
                               DPARAM      param
                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    BOOLEAN             is_success = FALSE;
    MAIL_MESSAGE_T      *mail_ptr = PNULL;
    mail_retr_result_t  *retr_result = NULL;
    MAIL_RESULT_INFO_T *signal_ptr = (MAIL_RESULT_INFO_T*)param;
    
    if(PNULL == app_ptr)
    {
        //SCI_TRACE_LOW:"HandleMailTaskMsg app_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3155_112_2_18_2_48_31_442,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    //COMM_TRACE:"MMIMAIL==> HandleMailTaskMsg: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3159_112_2_18_2_48_31_443,(uint8*)"d", msg_id);
    
    switch ( msg_id )
    {
    case MSG_MAIL_CLOSE_WAITING_WIN:
        if (PNULL != param)
        {
        	is_success = signal_ptr.is_success;
        	MMIMAIL_CloseInitWaitingWindow(is_success);
        }
        break;

    case MSG_MAIL_CHECK_PASSWD_RESULT:
        if (PNULL != param)
        {
        	is_success = signal_ptr.is_success;
        	MMIMAIL_CheckPwFinished(is_success);
        }
        break;
        
    case MSG_MAIL_FINISH_READ:
        if (PNULL != param)
        {
            mail_ptr = (MAIL_MESSAGE_T *)*((MAIL_MESSAGE_T **)param);
            MMIMAIL_ReadFinished(mail_ptr);
        }
        break;
    
    case MSG_MAIL_DELETE_FINISH:
        if (PNULL != param)
        {
        	is_success = signal_ptr.is_success;
        	MMIMAIL_DelFinished(is_success);
        }
        break;
    
    case MSG_MAIL_LIST_CHANGE:
        MMIMAIL_HandleListChangeCallBack();
        break;

    case MSG_MAIL_RETRATTACH_RESULT:
        if (PNULL != param)
        {
            retr_result = (mail_retr_result_t *)(*((mail_retr_result_t **)param));
            MMIMAIL_HandleRetrAttachResult(retr_result);
        }
        break;

    case MSG_MAIL_RETRMAIL_RESULT:
        if (PNULL != param)
        {
            retr_result = (mail_retr_result_t *)(*((mail_retr_result_t **)param));
            if(mail_GetAutoRecvingState() == TRUE)
            {
                MMIMail_NewMailArrival(retr_result);
                mail_SetAutoRecvingState(FALSE);
                comm_free(retr_result);
            }
            else
            {
                retr_result = (mail_retr_result_t *)(*((mail_retr_result_t **)param));
                MMIMAIL_HandleRetrAttachResult(retr_result);            
            }           
        }
        break;

    case MSG_MAIL_RECV_MAIL_PROGRESS:
        if (PNULL != param)
        {
            mail_ProgressInfo *progress_info = (mail_ProgressInfo*)(*((mail_ProgressInfo **)param));

            //更新邮件下载进度
            MMIMAIL_ProgressUpdateCallBack(progress_info);
        }
        break;

    case MSG_MAIL_SAVE_FINISH:
        if (PNULL != param)
        {
            is_success = signal_ptr.is_success;
            MMIMAIL_SaveFinished(is_success);
            
            //更新列表
            MMIMAIL_HandleListChangeCallBack();
        }
        break;
    
    case MSG_MAIL_SEND_FINISH:
        if (PNULL != param)
        {
            is_success = signal_ptr.is_success;       
        }
        else
        {
            is_success = TRUE;
        }
        MMIMAIL_SendFinished(is_success);
        break;

    case MSG_MAIL_RESET_FACTORY_OVER:
        if (PNULL != param)
        {
            is_success = signal_ptr.is_success; 
            MMIMAIL_ResetFinished(is_success);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return ( result );
}

/*****************************************************************************/
// 	Description : 欢迎界面
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailWelcomeWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    info = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);

    //COMM_TRACE:"MMIMAIL==> HandleMailWelcomeWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3288_112_2_18_2_48_31_444,(uint8*)"d", msg_id);

	switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(MMIMAIL_WELCOME_TEXT_CTRL_ID, &both_client_rect);

        //获取资费信息
        MMIMAIL_GetChargeInfo();
        //等获取到资费信息后再更新显示内容
        //ShowWelcomeInfo(FALSE, PNULL);
        MMI_GetLabelTextByLang(TXT_MAIL_CHARGE_INFO, &info);
        ShowWelcomeInfo(TRUE, &info);
        MMK_SetAtvCtrl(win_id, MMIMAIL_WELCOME_TEXT_CTRL_ID);
        break;

    case MSG_MAIL_UPDATE_CHARGE_INFO:
        if (PNULL != param)
        {
            MMI_STRING_T    *charge_info = PNULL;
            
            charge_info = (MMI_STRING_T *)param;
            ShowWelcomeInfo(TRUE, charge_info);
        }
        break;
        
    case MSG_CTL_OK:
        //调用接口执行开通操作
        MMIMAIL_applyAccount();

        //清空收到配置信息的标记位
        mail_RecvedSettinginfoReset();
        
        //打开新的提示窗口
        MMK_CreateWin((uint32 *)MMIMAIL_OPENSERVICE_WIN_TAB, PNULL);

        MMK_CloseWin(win_id);
        break;
        
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
    	break;
        
	default:
    	result = MMI_RESULT_FALSE;
    	break;
    }
	return (result);
}

/*****************************************************************************/
// 	Description : 提示开通信息界面
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailApplyAlertWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    text_str = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);

    //COMM_TRACE:"MMIMAIL==> HandleMailApplyAlertWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3352_112_2_18_2_48_31_445,(uint8*)"d", msg_id);

	switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(MMIMAIL_WELCOME_TEXT_CTRL_ID, &both_client_rect);
        MMI_GetLabelTextByLang(TXT_MAIL_CONFIG_REQUEST,&text_str);
        GUITEXT_SetString(MMIMAIL_WELCOME_TEXT_CTRL_ID, text_str.wstr_ptr, text_str.wstr_len , FALSE);
        MMK_SetAtvCtrl(win_id, MMIMAIL_WELCOME_TEXT_CTRL_ID);        
        break;
       
    case MSG_CTL_OK:
        //重新进入邮箱
        MMIMAIL_Enter(s_mmimail_info.enter_type, s_mmimail_info.enter_param);
        
        MMK_CloseWin(win_id);
        break;
        
	default:
    	result = MMI_RESULT_FALSE;
    	break;
    }
	return (result);
}

/*****************************************************************************/
// 	Description : 邮箱列表界面主处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailMainMenuWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_MAINMENU_CTRL_ID;
    uint16 node_id = 0;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUI_RECT_T title_rect = {0};

    //COMM_TRACE:"MMIMAIL==> HandleMailMainMenuWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3393_112_2_18_2_48_31_446,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (0 == mail_FileFreeSpace()) //U盘剩余空间不足
        {
            MMIMAIL_OpenMsgBox(TXT_MAIL_NO_MEMORY, IMAGE_PUBWIN_WARNING);
        }
        CreateMailMainMenu(win_id, ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        title_rect.left = 0;
        title_rect.top = MMI_STATUSBAR_HEIGHT;
        title_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
        title_rect.bottom = MMI_STATUSBAR_HEIGHT + MMI_TITLE_HEIGHT - 1;
        GUIWIN_SetTitleRect(win_id, title_rect);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMK_CreateWin((uint32 *)MMIMAIL_MAINMENU_OPT_MENU_WIN_TAB,PNULL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        EnterMailCorrespondingMenu((MMIMAIL_MAIN_MENU_NODE_E)node_id);
        break;
        
    case MSG_MAIL_NEEDUPDATEMENU:
        s_need_update = TRUE;
        break;
        
    case MSG_GET_FOCUS:    
        if (s_mmimail_info.is_def_account_change)
        {
            s_mmimail_info.is_def_account_change = FALSE;
            
            //重新加载当前帐号邮件列表
            s_mmimail_info.enter_type = MAIL_ENTER_MAINMENU;
            
            MMIMAIL_OpenInitWaitingWindow();
            
            //获取默认帐号的邮箱信息
            MMIMAIL_GetAccountMailBoxInfo(&s_mmimail_info.def_account, &s_mmimail_info.def_account_box_info);
        }
        
        if (s_need_update)
        {
            UpdateMailMainMenu(win_id, ctrl_id);
            s_need_update = FALSE; //更新菜单后将静态变量置为初始值
        }
        break;

    case MSG_MAIL_UPDATE_MENU:
        UpdateMailMainMenu(win_id, ctrl_id);
        StartMailMenuWinTimer(win_id);
        s_need_update = TRUE;
        break;

    case MSG_LOSE_FOCUS:
        StopMailMenuWinTimer();
        break;
        
    case MSG_TIMER:
        if (s_need_update)
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); //刷新窗口
            s_need_update = FALSE;
        }
        break; 
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIMAIL_Exit();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : 邮箱列表界面选项菜单处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailMainMenuOptWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;

    //COMM_TRACE:"MMIMAIL==> HandleMailMainMenuOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3497_112_2_18_2_48_31_447,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIMAIL_MAINMENU_OPT_MENU_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MMIMAIL_MAINMENU_OPT_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_SYS_SETTING_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SYS_SETTING_MENU_WIN_TAB, PNULL);
            break;
            
        case MMIMAIL_MENU_HELP_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_HELP_WIN_TAB, PNULL);
            break;
//sunguochen add for test
        case MMIMAIL_MENU_SEND_ITEM_ID:

            mail_Send_test();
            break;
            
        case MMIMAIL_MENU_RECV_ITEM_ID:
            
            mail_retrieve_test();
            break;            
//end
        default:
            //SCI_TRACE_LOW:"MMIMail HandleMailMainMenuOptWinMsg menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3532_112_2_18_2_48_31_448,(uint8*)"d", menu_id);
            break;
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//	Description : 邮箱主界面处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailBoxMainWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T child_win_id = 0;

    //COMM_TRACE:"MMIMAIL==> HandleMailBoxMainWindow, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3563_112_2_18_2_48_31_449,(uint8*)"d", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, MMIMAIL_MAILBOX_TAB_CTRL_ID);

            AddTabsAndChildWindows();
            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_MAIL_READ:
            child_win_id = MMIMAIL_GetCurFocusChildBoxWinId();
            MMK_SendMsg(child_win_id, MSG_MAIL_READ, PNULL);
            break;

        case MSG_MAIL_FINISH_READ:
            child_win_id = MMIMAIL_GetCurFocusChildBoxWinId();
            MMK_SendMsg(child_win_id, MSG_MAIL_FINISH_READ, PNULL);
            break;
        
        case MSG_MAIL_UPDATE_LIST:        
            child_win_id = MMIMAIL_GetCurFocusChildBoxWinId();

            if (MMK_IsOpenWin(child_win_id))
            {
                MMK_SendMsg(child_win_id, MSG_MAIL_UPDATE_LIST, PNULL);
            }
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseParentWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            if (!MMK_IsOpenWin(MMIMAIL_MAIN_MENU_WIN_ID))
            {
                //邮箱列表主界面不存在的情况下，退出139邮箱
                MMIMAIL_Exit();
            }
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//	Description : 收件箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInBoxChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_INBOX_LISTBOX_CTRL_ID;
    MMIMAIL_BOX_TYPE_E box_type = MMIMAIL_INBOX;
    //uint16          max_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    LOCAL BOOLEAN   s_need_update = FALSE;    
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //COMM_TRACE:"MMIMAIL==> HandleInBoxChildWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3634_112_2_18_2_48_31_450,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        s_need_update = FALSE;
        
        GUIAPICTRL_SetBothRect(ctrl_id,&list_both_rect);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

        // load items to list box
        have_items = MMIMAIL_LoadMailList(box_type, ctrl_id);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMIMail HandleInBoxChildWinMsg PNULL == need_item_data_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3666_112_2_18_2_48_32_451,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
    
            MMIMAIL_SetListItemData(box_type, ctrl_id, need_item_data_ptr->item_index);
        }
        break;
        
    case MSG_MAIL_NEEDUPDATELIST:
        s_need_update = TRUE;
        break;

    case MSG_GET_FOCUS:    
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        HandleMailListGetFocusMsg(box_type, ctrl_id, &s_need_update, &have_items);
        break;

    case MSG_MAIL_UPDATE_LIST:
        HandleMailListUpdateMsg(box_type, ctrl_id, &have_items);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_MAIL_FINISH_READ:
    case MSG_KEYUP_GREEN:
    case MSG_KEYLONG_GREEN:
        //if ( have_items ) //邮件为空时可以操作选项菜单中的新建邮件和接收
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_MAIL_READ:
        if (have_items && (0 == s_mmimail_info.marked_mail_num))
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_LIST_MARK_ITEM:   
        HandleMailListMarkOperation(box_type, ctrl_id, param);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIMAIL_MAILBOX_MAIN_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        MMIMAIL_SetAllMailMarked(box_type, FALSE);
        //如果已经没有未读信息了，就关闭新邮件的提示窗口
    	MMIAPIMAIL_CloseNewMailPromptWin();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 发件箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutBoxChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_OUTBOX_LISTBOX_CTRL_ID;
    MMIMAIL_BOX_TYPE_E box_type = MMIMAIL_OUTBOX;
    //uint16          max_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    LOCAL BOOLEAN   s_need_update = FALSE;    
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //COMM_TRACE:"MMIMAIL==> HandleOutBoxChildWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3766_112_2_18_2_48_32_452,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        s_need_update = FALSE;
        
        GUIAPICTRL_SetBothRect(ctrl_id,&list_both_rect);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

        // load items to list box
        have_items = MMIMAIL_LoadMailList(box_type, ctrl_id);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMIMail HandleOutBoxChildWinMsg PNULL == need_item_data_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3798_112_2_18_2_48_32_453,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
            
            MMIMAIL_SetListItemData(box_type, ctrl_id, need_item_data_ptr->item_index);
        }
        break;
        
    case MSG_MAIL_NEEDUPDATELIST:
        s_need_update = TRUE;
        break;

    case MSG_GET_FOCUS:    
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        HandleMailListGetFocusMsg(box_type, ctrl_id, &s_need_update, &have_items);
        break;

    case MSG_MAIL_UPDATE_LIST:
        HandleMailListUpdateMsg(box_type, ctrl_id, &have_items);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_MAIL_FINISH_READ:
    case MSG_KEYUP_GREEN:
    case MSG_KEYLONG_GREEN:
        if ( have_items )
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_MAIL_READ:
        if (have_items && (0 == s_mmimail_info.marked_mail_num))
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_LIST_MARK_ITEM:   
        HandleMailListMarkOperation(box_type, ctrl_id, param);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIMAIL_MAILBOX_MAIN_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        MMIMAIL_SetAllMailMarked(box_type, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 已发件箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSentBoxChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SENTBOX_LISTBOX_CTRL_ID;
    MMIMAIL_BOX_TYPE_E box_type = MMIMAIL_SENTBOX;
    //uint16          max_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    LOCAL BOOLEAN   s_need_update = FALSE;    
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //COMM_TRACE:"MMIMAIL==> HandleSentBoxChildWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3896_112_2_18_2_48_32_454,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        s_need_update = FALSE;
        
        GUIAPICTRL_SetBothRect(ctrl_id,&list_both_rect);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

        // load items to list box
        have_items = MMIMAIL_LoadMailList(box_type, ctrl_id);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMIMail HandleSentBoxChildWinMsg PNULL == need_item_data_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_3928_112_2_18_2_48_32_455,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
            MMIMAIL_SetListItemData(box_type, ctrl_id, need_item_data_ptr->item_index);
        }
        break;
        
    case MSG_MAIL_NEEDUPDATELIST:
        s_need_update = TRUE;
        break;

    case MSG_GET_FOCUS:    
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        HandleMailListGetFocusMsg(box_type, ctrl_id, &s_need_update, &have_items);
        break;

    case MSG_MAIL_UPDATE_LIST:
        HandleMailListUpdateMsg(box_type, ctrl_id, &have_items);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_MAIL_FINISH_READ:
    case MSG_KEYUP_GREEN:
    case MSG_KEYLONG_GREEN:
        if ( have_items )
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_MAIL_READ:
        if (have_items && (0 == s_mmimail_info.marked_mail_num))
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_LIST_MARK_ITEM:   
        HandleMailListMarkOperation(box_type, ctrl_id, param);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIMAIL_MAILBOX_MAIN_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        MMIMAIL_SetAllMailMarked(box_type, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 草稿箱子窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDraftChildWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
    MMIMAIL_BOX_TYPE_E box_type = MMIMAIL_DRAFTBOX;
    //uint16          max_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    LOCAL BOOLEAN   s_need_update = FALSE;    
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //COMM_TRACE:"MMIMAIL==> HandleDraftChildWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4025_112_2_18_2_48_32_456,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        s_need_update = FALSE;
        
        GUIAPICTRL_SetBothRect(ctrl_id,&list_both_rect);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

        // load items to list box
        have_items = MMIMAIL_LoadMailList(box_type, ctrl_id);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMIMail HandleDraftChildWinMsg PNULL == need_item_data_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4057_112_2_18_2_48_32_457,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
            MMIMAIL_SetListItemData(box_type, ctrl_id, need_item_data_ptr->item_index);
        }
        break;
        
    case MSG_MAIL_NEEDUPDATELIST:
        s_need_update = TRUE;
        break;

    case MSG_GET_FOCUS:    
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        HandleMailListGetFocusMsg(box_type, ctrl_id, &s_need_update, &have_items);
        break;

    case MSG_MAIL_UPDATE_LIST:
        HandleMailListUpdateMsg(box_type, ctrl_id, &have_items);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_MAIL_FINISH_READ:
    case MSG_KEYUP_GREEN:
    case MSG_KEYLONG_GREEN:
        if ( have_items )
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_MAIL_READ:
        if (have_items && (0 == s_mmimail_info.marked_mail_num))
        {
            HandleMailListOkMsg(
                win_id,
                msg_id,
                ctrl_id,
                box_type
                );
        }
        break;

    case MSG_CTL_LIST_MARK_ITEM:   
        HandleMailListMarkOperation(box_type, ctrl_id, param);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIMAIL_MAILBOX_MAIN_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        s_mmimail_info.marked_mail_num = 0;
        MMIMAIL_SetAllMailMarked(box_type, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//  Description : 处理邮件列表界面MSG_GET_FOCUS消息
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void HandleMailListGetFocusMsg(
                                                MMIMAIL_BOX_TYPE_E  box_type, 
                                                MMI_CTRL_ID_T       ctrl_id,
                                                BOOLEAN             *need_update_flag, //in & out
                                                BOOLEAN             *have_items //in & out
                                             )
{
    uint16 i = 0;
    uint16 selected_index = 0;
    
    if (MMIMAIL_GetCurBoxType() != box_type) //从其他信箱切换回来
    {
        //清除掉原来的标记
        s_mmimail_info.marked_mail_num = 0;
        for (i = 0; i< GUILIST_GetTotalItemNum(ctrl_id); i++)
        {
            GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
        }
        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
        MMIMAIL_SetAllMailMarked(box_type, FALSE);

        //设置当前操作的邮箱类型
        MMIMAIL_SetCurBoxType(box_type);

        //设置邮箱列表菜单选中项为当前邮箱
        if (MMK_IsOpenWin(MMIMAIL_MAIN_MENU_WIN_ID))
        {
            //box_type的值与MMIMAIL_MAIN_MENU_NODE_INBOX - MMIMAIL_MAIN_MENU_NODE_WRITE_NEW_MAIL的值相同
            GUIMENU_SetFirstAndSelectedItem(0, (uint16)box_type,MMIMAIL_MAINMENU_CTRL_ID);
        }

        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        *have_items = MMIMAIL_LoadMailList( box_type, ctrl_id );
        if (*have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }
        GUILIST_SetCurItemIndex(ctrl_id, selected_index);
        *need_update_flag = FALSE;
    }
    else
    {
        if (*need_update_flag || MMIMAIL_IsNeedSort())
        {
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            *have_items = MMIMAIL_LoadMailList( box_type, ctrl_id );
            if (*have_items)
            {            
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            *need_update_flag = FALSE;
        }
        else
        {
            //在邮件浏览界面返回，可能在浏览界面上下切换了当前邮件
            if (PNULL != MMIMAIL_GetCurOperationMail())
            {
                //获取当前操作邮件在列表中的显示位置
                selected_index = MMIMAIL_GetCurOperationMailIndex(MMIMAIL_GetCurOperationMail());
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            }

        }
    }
    GUILIST_ValidCurItemPos(ctrl_id);
}

/*****************************************************************************/
//  Description : 处理邮件列表界面 MMISMS_MSG_UPDATELIST 消息
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void HandleMailListUpdateMsg(
                                            MMIMAIL_BOX_TYPE_E  box_type, 
                                            MMI_CTRL_ID_T       ctrl_id,
                                            BOOLEAN             *have_items
                                         )
{
    uint16  selected_index = 0;
    uint16  total_num = 0;
    uint16  top_index = 0;
    uint16  page_num = 0;
    
    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
    total_num = GUILIST_GetTotalItemNum(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    page_num = GUILIST_GetPageItemNum(ctrl_id);

    *have_items = MMIMAIL_LoadMailList(box_type, ctrl_id);
    if (*have_items)
    {            
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
    }
    else
    {
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
    }

    if (total_num < GUILIST_GetTotalItemNum(ctrl_id)) //有增加新信息
    {
        if((top_index == 0) && (selected_index != (page_num - 1)))
        {
            GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
            GUILIST_SetTopItemIndex(ctrl_id, top_index);
        }
        else
        {
            GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
            GUILIST_SetTopItemIndex(ctrl_id, top_index + 1);
        }
    }

    else
    {
        GUILIST_SetCurItemIndex(ctrl_id, selected_index);
        GUILIST_SetTopItemIndex(ctrl_id, top_index);
    }
}

/*****************************************************************************/
//  Description : 处理邮件列表界面 标记/取消标记 操作
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void HandleMailListMarkOperation(
                                            MMIMAIL_BOX_TYPE_E  box_type, 
                                            MMI_CTRL_ID_T       ctrl_id,
                                            DPARAM              param
                                         )
{
    GUILIST_MARK_ITEM_T *list_param_ptr = PNULL;
    
    //to get the info of current selected item
    list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

    MMIMAIL_SetCurOperationMailByIndex(box_type, list_param_ptr->item_idx);

    if (ctrl_id == list_param_ptr->ctrl_id)
    {
        if (list_param_ptr->had_maked)
        {
            MMIMAIL_MAIL_SUMMERY_T * cur_mail_ptr = MMIMAIL_GetCurOperationMail();

            if (PNULL == cur_mail_ptr)
            {
                //SCI_TRACE_LOW:"MMIMail HandleDraftChildWinMsg PNULL == cur_mail_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4292_112_2_18_2_48_33_458,(uint8*)"");
                return ;
            }
            if (MMIMAIL_STATE_SENDING != MMIMAIL_GetMailState(cur_mail_ptr))
            {
                MMIMAIL_SetCurMailMarked(TRUE);
                s_mmimail_info.marked_mail_num ++;                
            }
            else
            {
                //发送中的邮件去掉选中标记
                GUILIST_SetSelectedItem(ctrl_id, list_param_ptr->item_idx, FALSE);
                MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, 0);
            }
        }
        else
        {
            MMIMAIL_SetCurMailMarked(FALSE);
            s_mmimail_info.marked_mail_num --;
            // 如果标记数为零，关闭标记状态
            if (0 == s_mmimail_info.marked_mail_num)
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            }
        }
    }
}

/*****************************************************************************/
//	Description : 处理中间键消息，进入邮件浏览界面
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void HandleMailListOkMsg(
                                MMI_WIN_ID_T         win_id,
                                MMI_MESSAGE_ID_E     msg_id,
                                MMI_CTRL_ID_T        ctrl_id,
                                MMIMAIL_BOX_TYPE_E   box_type
                                )
{
    uint16 selected_index = 0;
    //MMI_STRING_T   prompt_str = {0};
    //MMI_MENU_ID_T menu_id = 0;
    //GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    //MMI_CTRL_ID_T           cur_ctrl_id = 0;
    //uint16                  cur_index = 0;

    //COMM_TRACE:"MMIMAIL==> HandleMailListOkMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4338_112_2_18_2_48_33_459,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_MAIL_READ:
    {
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        if (MMIMAIL_STATE_SENDING == MMIMAIL_GetMailState(MMIMAIL_GetCurMailPtr(box_type, selected_index)))
        {
            //正在发送的邮件按中间键打开选项菜单，不允许查看或编辑邮件
            MMK_SendMsg(win_id, MSG_APP_OK, 0);
        }
        else
        {
            StartReadMail(box_type, MMIMAIL_OPER_OPEN_READ_WIN, ctrl_id);
        }
        break;
    }

    case MSG_CTL_OK:
    case MSG_APP_OK:
       //保存当前操作类型为打开选项菜单
       SetHandleOkMenuOperation(MMIMAIL_OPER_OPEN_POPMENU);
       
       //保存当前正在操作的邮件
       selected_index = GUILIST_GetCurItemIndex(ctrl_id);
       MMIMAIL_SetCurOperationMailByIndex(box_type, selected_index);

       MMK_PostMsg(win_id, MSG_MAIL_FINISH_READ, PNULL,0);
       break;

    case MSG_MAIL_FINISH_READ:
        if(MMK_IsOpenWin(MMIMAIL_READ_WAITING_WIN_ID)) 
        {
            MMK_CloseWin(MMIMAIL_READ_WAITING_WIN_ID);
        }
        
        if (MMIMAIL_OPER_OPEN_READ_WIN == GetHandleOkMenuOperation())
        {
            if (0 == s_mmimail_info.marked_mail_num)
            {
                MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = MMIMAIL_GetCurOperationMail();
                
                //打开阅读邮件界面
                if (PNULL != cur_mail_ptr)
                {
                    MMIAPIMAIL_OpenReadWin(cur_mail_ptr->record_id);
                }
                else
                {
                    //COMM_TRACE:"HandleMailListOkMsg: Get current operating mail fail!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4391_112_2_18_2_48_33_460,(uint8*)"");
                }
            }
            
            SetHandleOkMenuOperation(MMIMAIL_OPER_NULL);
        }
        else if(MMIMAIL_OPER_OPEN_EDIT_WIN == GetHandleOkMenuOperation())
        {
            MAIL_MESSAGE_T *whole_mail_ptr = PNULL;
            MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type = MMIMAIL_GetOpenEditwinType();
            
            whole_mail_ptr = MMIMAIL_GetCurDispMail();
            
            //打开编辑邮件界面
            MMIAPIMAIL_OpenEditWin(open_type, whole_mail_ptr);
        }
        else
        {
            SetHandleOkMenuOperation(MMIMAIL_OPER_NULL);

            //打开选项菜单
            switch (box_type)
            {
            case MMIMAIL_INBOX:    
                MMK_CreateWin((uint32 *)MMIMAIL_INBOX_OPT_MENU_WIN_TAB,PNULL);
                break;
                
            case MMIMAIL_OUTBOX:
                if (MMIMAIL_STATE_SENDING == MMIMAIL_GetMailState(MMIMAIL_GetCurOperationMail()))
                {
                    //当前邮件为正在发送状态，则选项菜单只提供取消发送操作
                    MMK_CreateWin((uint32 *)MMIMAIL_OUTBOX_OPT_EXT_MENU_WIN_TAB,PNULL);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMIMAIL_OUTBOX_OPT_MENU_WIN_TAB,PNULL);
                }
                break;
                
            case MMIMAIL_SENTBOX:
                MMK_CreateWin((uint32 *)MMIMAIL_SENTBOX_OPT_MENU_WIN_TAB,PNULL);
                break;
                
            case MMIMAIL_DRAFTBOX:
                MMK_CreateWin((uint32 *)MMIMAIL_DRAFTBOX_OPT_MENU_WIN_TAB,PNULL);
                break;
                
            default:
                break;
            }
        }

        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//	Description : 处理邮箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMailBoxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    uint16 i = 0;
    uint16 cur_index = 0;
    MMI_CTRL_ID_T cur_ctrl_id = 0;
    MMIMAIL_BOX_TYPE_E cur_box_type = MMIMAIL_BOX_NONE;
    MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr = PNULL;
    
    cur_box_type = MMIMAIL_GetCurBoxType();
    cur_ctrl_id = MMIMAIL_GetCurActiveListCtrlId();
    cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
    //cur_mail_ptr = MMIMAIL_GetCurMailPtr(cur_box_type, cur_index);
    cur_mail_ptr = MMIMAIL_GetCurOperationMail();
    
    switch (menu_id)
    {
    case MMIMAIL_MENU_MARK_ONE_ITEM_ID:
        if (PNULL == cur_mail_ptr)
        {
            //SCI_TRACE_LOW:"MMIMail HandleMailBoxOptMenuId PNULL == cur_mail_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4477_112_2_18_2_48_33_461,(uint8*)"");
            return FALSE;
        }
        if (!MMIMAIL_GetCurMailIsMarked())
        {
            GUILIST_SetTextListMarkable(cur_ctrl_id, TRUE);
            GUILIST_SetMaxSelectedItem(cur_ctrl_id, MMIMAIL_GetBoxTotalNumber(cur_box_type));
            if (MMIMAIL_STATE_SENDING != MMIMAIL_GetMailState(cur_mail_ptr))
            {
                GUILIST_SetSelectedItem(cur_ctrl_id, cur_index, TRUE);
                MMIMAIL_SetCurMailMarked(TRUE);
                s_mmimail_info.marked_mail_num ++;
            }
        }
        MMK_CloseWin(win_id);
        break;
                
    case MMIMAIL_MENU_CANCEL_MARK_ONE_ITEM_ID:
        if (MMIMAIL_GetCurMailIsMarked())
        {
            GUILIST_SetSelectedItem(cur_ctrl_id, cur_index, FALSE);
            MMIMAIL_SetCurMailMarked(FALSE);
            s_mmimail_info.marked_mail_num --;
            
            if (s_mmimail_info.marked_mail_num == 0)
            {
                GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
            }
        }
        MMK_CloseWin(win_id);
        break;
        
    case MMIMAIL_MENU_MARK_ALL_ITEM_ID:
        if (PNULL == cur_mail_ptr)
        {
            //SCI_TRACE_LOW:"MMIMail HandleMailBoxOptMenuId PNULL == cur_mail_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4512_112_2_18_2_48_33_462,(uint8*)"");
            return FALSE;
        }
        GUILIST_SetTextListMarkable(cur_ctrl_id, TRUE);
        GUILIST_SetMaxSelectedItem(cur_ctrl_id, MMIMAIL_GetBoxTotalNumber(cur_box_type));
        for (i = 0; i< GUILIST_GetTotalItemNum(cur_ctrl_id); i++)
        {
            cur_mail_ptr = MMIMAIL_GetCurMailPtr(cur_box_type, i);
            if ((PNULL != cur_mail_ptr)
                && (MMIMAIL_STATE_SENDING != MMIMAIL_GetMailState(cur_mail_ptr)))
            {
                GUILIST_SetSelectedItem(cur_ctrl_id, i, TRUE);
                MMIMAIL_SetMailMarked(cur_box_type, i, TRUE);
                s_mmimail_info.marked_mail_num ++;
            }
        }

        MMK_CloseWin(win_id);
        break;
        
    case MMIMAIL_MENU_CANCEL_MARK_ALL_ITEM_ID:
        for (i = 0; i< GUILIST_GetTotalItemNum(cur_ctrl_id); i++)
        {
            GUILIST_SetSelectedItem(cur_ctrl_id, i, FALSE);
        }
        s_mmimail_info.marked_mail_num = 0;
        GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
        MMIMAIL_SetAllMailMarked(cur_box_type, FALSE);
        MMK_CloseWin(win_id);
        break;

    case MMIMAIL_MENU_DEL_ITEM_ID:
        MMIMAIL_OpenDelteQueryWin();        
        MMK_CloseWin( win_id );
        break;

    case MMIMAIL_MENU_DEL_ALL_ITEM_ID:
        MMIMAIL_OpenDelteAllQueryWin();        
        MMK_CloseWin( win_id );
        break;

    case MMIMAIL_MENU_REPLY_ITEM_ID:
    case MMIMAIL_MENU_REPLY_ALL_ITEM_ID:
    case MMIMAIL_MENU_FORWARD_CONTENT_ITEM_ID:
    case MMIMAIL_MENU_SERVER_FORWARD_ACC_ITEM_ID:
    case MMIMAIL_MENU_SERVER_FORWARD_ALL_ITEM_ID:
    case MMIMAIL_MENU_NEW_MAIL_ITEM_ID:
    case MMIMAIL_MENU_RECEIVE_ITEM_ID:
        result = HandleInboxOptMenuId(win_id, menu_id);
        break;

    case MMIMAIL_MENU_OPT_CANCEL_SEND_ITEM_ID:
    case MMIMAIL_MENU_OPT_SEND_ITEM_ID:
    case MMIMAIL_MENU_OPT_EDIT_ITEM_ID:
        result = HandleOutboxOptMenuId(win_id, menu_id);
        break;
        
    case MMIMAIL_MENU_OPT_FORWARD_ITEM_ID:
    case MMIMAIL_MENU_OPT_RESEND_ITEM_ID:
        result = HandleSentboxOptMenuId(win_id, menu_id);
        break;

    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//	Description : 处理收件箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleInboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_type = MMIMAIL_OPEN_TYPE_DEFAULT;
    
    switch (menu_id)
    {
    case MMIMAIL_MENU_REPLY_ITEM_ID:
    case MMIMAIL_MENU_REPLY_ALL_ITEM_ID:
    case MMIMAIL_MENU_FORWARD_CONTENT_ITEM_ID:
    case MMIMAIL_MENU_SERVER_FORWARD_ACC_ITEM_ID:
    case MMIMAIL_MENU_SERVER_FORWARD_ALL_ITEM_ID:
        if (MMIMAIL_MENU_REPLY_ITEM_ID == menu_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_REPLY;
        }
        else if (MMIMAIL_MENU_REPLY_ALL_ITEM_ID == menu_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_REPLY_ALL;
        }
        else if (MMIMAIL_MENU_FORWARD_CONTENT_ITEM_ID == menu_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_FORWARD_CONTENT;
        }
        else if (MMIMAIL_MENU_SERVER_FORWARD_ACC_ITEM_ID == menu_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ACC;
        }
        else if (MMIMAIL_MENU_SERVER_FORWARD_ALL_ITEM_ID == menu_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ALL;
        }
        
        //保存当前操作为打开编辑窗口
        MMIMAIL_SetOpenEditwinType(open_type);

        //开始读取邮件内容
        StartReadMail(MMIMAIL_GetCurBoxType(), MMIMAIL_OPER_OPEN_EDIT_WIN, MMIMAIL_GetCurActiveListCtrlId());  

        //关闭选项菜单
        MMK_CloseWin(win_id);
        break;

    case MMIMAIL_MENU_NEW_MAIL_ITEM_ID:
        //直接打开编辑邮件界面
        MMIAPIMAIL_OpenEditWin(MMIMAIL_OPEN_TYPE_NEW_MAIL, PNULL);
        
        //关闭选项菜单
        MMK_CloseWin(win_id);
        break;
        
    case MMIMAIL_MENU_RECEIVE_ITEM_ID:
        MMK_CloseWin(win_id);
        MMIMAIL_ReceiveMail();
        break;

    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//	Description : 处理发件箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleOutboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr = PNULL;
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_type = MMIMAIL_OPEN_TYPE_DEFAULT;
    
    cur_mail_ptr = MMIMAIL_GetCurOperationMail();

    if ( NULL == cur_mail_ptr)
    {
        //COMM_TRACE:"MMIMAIL==> HandleOutboxOptMenuId: current operation mail is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4670_112_2_18_2_48_34_463,(uint8*)"");
        return TRUE;
    }

    switch (menu_id)
    {
    case MMIMAIL_MENU_OPT_CANCEL_SEND_ITEM_ID:
        MMIAPIMAIL_CancelSendById(cur_mail_ptr->record_id);
        MMK_CloseWin(win_id);
        break;
        
    case MMIMAIL_MENU_OPT_SEND_ITEM_ID:
        if (MMIMAIL_IsHaveReceiver(cur_mail_ptr))
        {
            MMIAPIMAIL_SendMailById(cur_mail_ptr->record_id);

        }
        else
        {
            //保存打开编辑窗口的类型
            open_type = MMIMAIL_OPEN_TYPE_ORIGINAL;
            MMIMAIL_SetOpenEditwinType(open_type);

            //开始读取邮件内容
            StartReadMail(MMIMAIL_GetCurBoxType(), MMIMAIL_OPER_OPEN_EDIT_WIN, MMIMAIL_GetCurActiveListCtrlId());  
        }
        MMK_CloseWin(win_id);
        break;
        
    case MMIMAIL_MENU_OPT_EDIT_ITEM_ID:
        //保存打开编辑窗口的类型
        open_type = MMIMAIL_OPEN_TYPE_ORIGINAL;
        MMIMAIL_SetOpenEditwinType(open_type);

        //开始读取邮件内容
        StartReadMail(MMIMAIL_GetCurBoxType(), MMIMAIL_OPER_OPEN_EDIT_WIN, MMIMAIL_GetCurActiveListCtrlId());  

        MMK_CloseWin(win_id);
        break;
        
    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//	Description : 处理已发件箱选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSentboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_MENU_ID_T      menu_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr = PNULL;
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_type = MMIMAIL_OPEN_TYPE_DEFAULT;
    
    cur_mail_ptr = MMIMAIL_GetCurOperationMail();
    
    if (PNULL == cur_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleSentboxOptMenuId PNULL == cur_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4735_112_2_18_2_48_34_464,(uint8*)"");
        return FALSE;
    }
    
    switch (menu_id)
    {
    case MMIMAIL_MENU_OPT_FORWARD_ITEM_ID:
    case MMIMAIL_MENU_OPT_RESEND_ITEM_ID:
        if (MMIMAIL_MENU_OPT_FORWARD_ITEM_ID == menu_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_FORWARD_ALL;
        }        
        else if (MMIMAIL_MENU_OPT_RESEND_ITEM_ID == menu_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_ORIGINAL;
        }
        //保存打开编辑窗口的类型
        MMIMAIL_SetOpenEditwinType(open_type);
        
        //开始读取邮件内容
        StartReadMail(MMIMAIL_GetCurBoxType(), MMIMAIL_OPER_OPEN_EDIT_WIN, MMIMAIL_GetCurActiveListCtrlId());  

        MMK_CloseWin(win_id);
        break;

    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC BOOLEAN MMIMAIL_CurMailIsLocked(void )
 * decr		: 判断当前邮件是否被锁定
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/10/2010 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_CurMailIsLocked(void )
{
    return mail_GetMailLockFlag(MMIMAIL_GetCurOperationMail()->record_id);
}

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC BOOLEAN MMIMAIL_IsSendingCurMail(void )
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/15/2011 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_IsSendingCurMail(void )
{
    return mail_GetMailSendingFlag(MMIMAIL_GetCurOperationMail()->record_id);
}

/*==========================================================
 * author		: liangshun      
 * function 	: LOCAL BOOLEAN MMIMAIL_NeedDispDelOpt(MMIMAIL_BOX_TYPE_E  box_type)
 * decr		: 判断是否需要在菜单中显示"删除"选项
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/13/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_NeedDispDelOpt(MMIMAIL_BOX_TYPE_E  box_type)
{
    if(MMIMAIL_HasMailBeenMarked(box_type))
    {
        if(0 < MMIMAIL_GetMailboxDeleteAvailNum(box_type))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if(MMIMAIL_CurMailIsLocked() || MMIMAIL_IsSendingCurMail())
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
}
#endif

/*****************************************************************************/
//	Description : 灰化选项菜单处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_EnableGrayed(
                                        MMI_WIN_ID_T        win_id,
                                        MMIMAIL_BOX_TYPE_E  box_type
                                        )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    
    if (MMIMAIL_INBOX == box_type)
    {
        group_id = MENU_MAIL_INBOX_OPT;
        
        //收件箱邮件为空时只能使用新建邮件和接收选项菜单
        if (0 == MMIMAIL_GetBoxTotalNumber(box_type))
        {
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_REPLY_ITEM_ID, TRUE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_REPLY_ALL_ITEM_ID, TRUE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_FORWARD_CONTENT_ITEM_ID, TRUE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_SERVER_FORWARD_ACC_ITEM_ID, TRUE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_SERVER_FORWARD_ALL_ITEM_ID, TRUE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ITEM_ID, TRUE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_MARK_ITEM_ID, TRUE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ALL_ITEM_ID, TRUE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_REPLY_ITEM_ID, FALSE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_REPLY_ALL_ITEM_ID, FALSE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_FORWARD_CONTENT_ITEM_ID, FALSE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_SERVER_FORWARD_ACC_ITEM_ID, FALSE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_SERVER_FORWARD_ALL_ITEM_ID, FALSE);
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
            if(MMIMAIL_NeedDispDelOpt(box_type))
            {
                MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ITEM_ID, FALSE);
                MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ALL_ITEM_ID, FALSE);
            }
            else
            {
                MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ITEM_ID, TRUE);
                MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ALL_ITEM_ID, TRUE);
            }
#else
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ITEM_ID, FALSE);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ALL_ITEM_ID, FALSE);
#endif
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_MARK_ITEM_ID, FALSE);
        }
    }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    else
    {
        switch(box_type)
        {
            case MMIMAIL_OUTBOX:
                group_id = MENU_MAIL_OUTBOX_OPT;
                break;
                
            case MMIMAIL_SENTBOX:
                group_id = MENU_MAIL_SENTBOX_OPT;
                break;
                
            case MMIMAIL_DRAFTBOX:
                group_id = MENU_MAIL_DRAFTBOX_OPT;
                break;
                
            default:
                break;
        }

        if(MMIMAIL_NeedDispDelOpt(box_type))
        {
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ITEM_ID, FALSE);

        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_DEL_ITEM_ID, TRUE);
        }
    }
#endif

    group_id = MENU_MAIL_MARK_OPT;
    if (MMIMAIL_GetCurMailIsMarked())
    {
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_MARK_ONE_ITEM_ID, TRUE);            
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_CANCEL_MARK_ONE_ITEM_ID, FALSE);            
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_CANCEL_MARK_ALL_ITEM_ID, FALSE);            
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_MARK_ONE_ITEM_ID, FALSE);            
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_CANCEL_MARK_ONE_ITEM_ID, TRUE);            
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_CANCEL_MARK_ALL_ITEM_ID, TRUE);            
    }

    //如果全部邮件被标记则灰化标记全部菜单项
    if (MMIMAIL_IsAllMailMarked(box_type))
    {
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_MARK_ALL_ITEM_ID, TRUE);            
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_MARK_ALL_ITEM_ID, FALSE);            
    }
}

/*****************************************************************************/
//	Description : 收件箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_INBOX_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;

    //COMM_TRACE:"MMIMAIL==> HandleInboxMenuOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4956_112_2_18_2_48_34_465,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMAIL_EnableGrayed(win_id, MMIMAIL_INBOX);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIMAIL_OperateDelorDelAll();
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        if (!HandleMailBoxOptMenuId(win_id, menu_id))
        {
            //COMM_TRACE:"MMIMAIL==> HandleInboxMenuOptWinMsg: menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_4982_112_2_18_2_48_34_466,(uint8*)"d", menu_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//	Description : 发件箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_OUTBOX_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;

    //COMM_TRACE:"MMIMAIL==> HandleOutboxMenuOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5015_112_2_18_2_48_34_467,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMAIL_EnableGrayed(win_id, MMIMAIL_OUTBOX);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIMAIL_OperateDelorDelAll();
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        if (!HandleMailBoxOptMenuId(win_id, menu_id))
        {
            //COMM_TRACE:"MMIMAIL==> HandleOutboxMenuOptWinMsg: menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5041_112_2_18_2_48_34_468,(uint8*)"d", menu_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//	Description : 发件箱选项菜单处理函数，针对正在发送中的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:只有取消发送一个菜单项
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutboxMenuOptExtWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_OUTBOX_OPT_EXT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;

    //COMM_TRACE:"MMIMAIL==> HandleOutboxMenuOptExtWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5074_112_2_18_2_48_34_469,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIMAIL_OperateDelorDelAll();
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        if (!HandleMailBoxOptMenuId(win_id, menu_id))
        {
            //COMM_TRACE:"MMIMAIL==> HandleOutboxMenuOptExtWinMsg: menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5099_112_2_18_2_48_34_470,(uint8*)"d", menu_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//	Description : 已发件箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSentboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_SENTBOX_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;

    //COMM_TRACE:"MMIMAIL==> HandleSentboxMenuOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5132_112_2_18_2_48_34_471,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMAIL_EnableGrayed(win_id, MMIMAIL_SENTBOX);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIMAIL_OperateDelorDelAll();
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        if (!HandleMailBoxOptMenuId(win_id, menu_id))
        {
            //COMM_TRACE:"MMIMAIL==> HandleSentboxMenuOptWinMsg: menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5158_112_2_18_2_48_34_472,(uint8*)"d", menu_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//	Description : 草稿箱选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDraftboxMenuOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_DRAFTBOX_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;

    //COMM_TRACE:"MMIMAIL==> HandleDraftboxMenuOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5191_112_2_18_2_48_35_473,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMAIL_EnableGrayed(win_id, MMIMAIL_DRAFTBOX);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIMAIL_OperateDelorDelAll();
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        if (!HandleMailBoxOptMenuId(win_id, menu_id))
        {
            //COMM_TRACE:"MMIMAIL==> HandleDraftboxMenuOptWinMsg: menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5217_112_2_18_2_48_35_474,(uint8*)"d", menu_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//  Description : 显示欢迎界面信息
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void ShowWelcomeInfo(
                                    BOOLEAN         is_disp_charge_info, //是否显示资费信息
                                    MMI_STRING_T    *charge_info         //资费信息
                                 )
{
    wchar           unicode_cr = 0x0d;
    MMI_STRING_T    cr_s = {0};
    MMI_STRING_T    text_s = {0};
    uint16          uint16_str[MMIMAIL_WELCOME_CONTENT_LEN] = {0};
    uint16          uint16_str_len = 0;
    MMI_STRING_T    tmp_str = {0};

    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;

    //业务介绍  
    MMI_GetLabelTextByLang(TXT_MAIL_INTRODUCE,&tmp_str);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &tmp_str);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    //资费信息
    if (is_disp_charge_info)
    {
        if ((PNULL != charge_info))
        {
            if ((PNULL != charge_info->wstr_ptr) && (charge_info->wstr_len > 0))
            {
                MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, charge_info);
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> ShowWelcomeInfo: charge_info->wstr_ptr = %d, charge_info->wstr_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5270_112_2_18_2_48_35_475,(uint8*)"dd",charge_info->wstr_ptr, charge_info->wstr_len);
            }
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> ShowWelcomeInfo: charge_info is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5275_112_2_18_2_48_35_476,(uint8*)"");
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MAIL_GET_INFOMATION,&tmp_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &tmp_str);
    }
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    //提示是否开通139邮箱业务
    MMI_GetLabelTextByLang(TXT_MAIL_ALERT_APPLY,&tmp_str);
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &tmp_str);

    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    if (MMIMAIL_WELCOME_CONTENT_LEN <= uint16_str_len)//memory overlay
    {
        //SCI_TRACE_LOW:"MMIMail ShowWelcomeInfo MMIMAIL_WELCOME_CONTENT_LEN <= uint16_str_len"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5294_112_2_18_2_48_35_477,(uint8*)"");
        return ;
    }
    
    GUITEXT_SetString(MMIMAIL_WELCOME_TEXT_CTRL_ID, text_s.wstr_ptr, text_s.wstr_len , FALSE);
}

/*****************************************************************************/
//	Description : 启动检查是否需要更新菜单内容的定时器
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void StartMailMenuWinTimer(
                                  MMI_WIN_ID_T  win_id  //in
                                  )
{
    if (0 == s_mmimail_info.mainmenu_win_timer_id)
    {
        s_mmimail_info.mainmenu_win_timer_id = MMK_CreateWinTimer(win_id, MMIMAIL_MENUWIN_TIME, TRUE);
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> StartMailMenuWinTimer: the mainmenu win timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5315_112_2_18_2_48_35_478,(uint8*)"");
    }
}

/*****************************************************************************/
//	Description : 停止检查是否需要更新菜单内容的定时器
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void StopMailMenuWinTimer(void)
{
    if (0 < s_mmimail_info.mainmenu_win_timer_id)
    {
        MMK_StopTimer(s_mmimail_info.mainmenu_win_timer_id);
        s_mmimail_info.mainmenu_win_timer_id = 0;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> StopMailMenuWinTimer: the mainmenu win timer has stop!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5332_112_2_18_2_48_35_479,(uint8*)"");
    }
}

/*****************************************************************************/
//	Description : 添加tab页和子窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows( void )
{
    MMI_STRING_T str_text = {0};
    
    GUITAB_SetTabRotative(MMIMAIL_MAILBOX_TAB_CTRL_ID,TRUE);
    //GUITAB_SetState(MMIMAIL_MAILBOX_TAB_CTRL_ID, GUITAB_STATE_NONEED_SCROLL, TRUE);
    //GUITAB_SetState(MMIMAIL_MAILBOX_TAB_CTRL_ID, GUITAB_STATE_ALIGN_MIDDLE, TRUE);

    GUITAB_SetMaxItemNum(MMIMAIL_MAILBOX_TAB_CTRL_ID, 4);

    MMI_GetLabelTextByLang(TXT_INBOX , &str_text);
    GUITAB_AppendSimpleItem(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        &str_text,
        IMAGE_SMS_INBOX_TAB_ON_ICON,
        IMAGE_SMS_INBOX_TAB_OFF_ICON);
    GUITAB_AddChildWindow(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        MMIMAIL_INBOX_CHILD_WIN_ID, 
        0);

    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &str_text);
    GUITAB_AppendSimpleItem(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        &str_text,
        IMAGE_SMS_SENDFAILBOX_TAB_ON_ICON,
        IMAGE_SMS_SENDFAILBOX_TAB_OFF_ICON);
    GUITAB_AddChildWindow(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        MMIMAIL_OUTBOX_CHILD_WIN_ID,
        1);
    
    MMI_GetLabelTextByLang(TXT_MAIL_SENTBOX, &str_text);
    GUITAB_AppendSimpleItem(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        &str_text,
        IMAGE_SMS_SENDSUCCBOX_TAB_ON_ICON,
        IMAGE_SMS_SENDSUCCBOX_TAB_OFF_ICON);
    GUITAB_AddChildWindow(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        MMIMAIL_SENTBOX_CHILD_WIN_ID, 
        2);

    MMI_GetLabelTextByLang(TXT_MAIL_DRAFT, &str_text);
    GUITAB_AppendSimpleItem(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        &str_text,
        IMAGE_SMS_DRAFTBOX_TAB_ON_ICON,
        IMAGE_SMS_DRAFTBOX_TAB_OFF_ICON);
    GUITAB_AddChildWindow(
        MMIMAIL_MAILBOX_TAB_CTRL_ID,
        MMIMAIL_DRAFTBOX_CHILD_WIN_ID,
        3);

    switch (s_mmimail_info.cur_box_type)
    {
    case MMIMAIL_INBOX:
        GUITAB_SetCurSel(MMIMAIL_MAILBOX_TAB_CTRL_ID, 0);
        break;

    case MMIMAIL_OUTBOX:
        GUITAB_SetCurSel(MMIMAIL_MAILBOX_TAB_CTRL_ID, 1);
        break;

    case MMIMAIL_SENTBOX:
        GUITAB_SetCurSel(MMIMAIL_MAILBOX_TAB_CTRL_ID, 2);
        break;
    
    case MMIMAIL_DRAFTBOX:
        GUITAB_SetCurSel(MMIMAIL_MAILBOX_TAB_CTRL_ID, 3);
        break;

    default:
        GUITAB_SetCurSel(MMIMAIL_MAILBOX_TAB_CTRL_ID, 0);
        break;
    }
}

/*****************************************************************************/
// 	Description : 创建邮箱列表界面主菜单
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void CreateMailMainMenu(
                                     MMI_WIN_ID_T   win_id,
                                     MMI_CTRL_ID_T  ctrl_id
                                     )
{
    GUI_BOTH_RECT_T both_rect_ptr = {0};
    MMI_STRING_T tstring = {0};
    MMI_STRING_T nstring = {0};
    MMI_STRING_T num_status = {0};
    uint16 i = 0;
    wchar number_string[MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1] = {0};
    MMI_STRING_T menu_string = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    
    both_rect_ptr.h_rect.left = 0;
    both_rect_ptr.h_rect.top = MMI_CLIENT_RECT_TOP;
    both_rect_ptr.h_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    both_rect_ptr.h_rect.bottom = MMI_CLIENT_RECT_BOTTOM;
    both_rect_ptr.v_rect.left = 0;
    both_rect_ptr.v_rect.top = MMI_CLIENT_RECT_TOP;
    both_rect_ptr.v_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    both_rect_ptr.v_rect.bottom = MMI_CLIENT_RECT_BOTTOM;

    if (MMK_IsIncludeStatusBar(win_id))
    {
        both_rect_ptr.h_rect.top = both_rect_ptr.h_rect.top + MMI_STATUSBAR_HEIGHT;
        both_rect_ptr.v_rect.top = both_rect_ptr.v_rect.top + MMI_STATUSBAR_HEIGHT;
    }

    num_status.wstr_ptr = number_string;
    menu_string.wstr_ptr = SCI_ALLOC_APP((MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1) * sizeof(wchar));
    menu_string.wstr_len = MMIMAIL_MAX_MEMSTATE_LABEL_LEN;
    SCI_MEMSET(menu_string.wstr_ptr, 0, ((MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1 ) * sizeof(wchar)));
    
    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect_ptr, win_id, ctrl_id, GUIMENU_STYLE_THIRD);

    //set title
    MMI_GetLabelTextByLang(TXT_MAIL_PUSHMAIL, &tstring);
    GUIMENU_SetMenuTitle(&tstring,ctrl_id);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_OPTION, TXT_NULL, STXT_RETURN);

    //insert write new mail
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    MMI_GetLabelTextByLang(TXT_MAIL_NEW, &nstring);
    node_item.item_text_ptr = &nstring;
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_WRITE;
    GUIMENU_InsertNode(i++, MMIMAIL_MAIN_MENU_NODE_WRITE_NEW_MAIL, 0, &node_item, ctrl_id);

    //insert inbox 
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_INBOX, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_INBOX);
    MMIAPICOM_MergeTwoString(
                        &nstring,
                        &num_status,
                        &menu_string,
                        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
                        );
    node_item.item_text_ptr = &menu_string;
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_INBOX;
    GUIMENU_InsertNode(i++, MMIMAIL_MAIN_MENU_NODE_INBOX, 0, &node_item, ctrl_id);

    //insert outbox
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_OUTBOX);
    MMIAPICOM_MergeTwoString(
                        &nstring,
                        &num_status,
                        &menu_string,
                        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
                        );
    node_item.item_text_ptr = &menu_string;
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_OUTBOX;
    GUIMENU_InsertNode(i++, MMIMAIL_MAIN_MENU_NODE_OUTBOX, 0, &node_item, ctrl_id);

    //insert sentbox
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_MAIL_SENTBOX, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_SENTBOX);
    MMIAPICOM_MergeTwoString(
                        &nstring,
                        &num_status,
                        &menu_string,
                        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
                      );
    node_item.item_text_ptr = &menu_string;
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_SENT;
    GUIMENU_InsertNode(i++, MMIMAIL_MAIN_MENU_NODE_SENTBOX, 0, &node_item, ctrl_id);

    //insert draft box
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET( menu_string.wstr_ptr, 0, ((MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1 ) * sizeof(wchar)));
    MMI_GetLabelTextByLang(TXT_MAIL_DRAFT, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_DRAFTBOX);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
        );
    node_item.item_text_ptr = &menu_string;
    node_item.select_icon_id = IMAGE_SECMENU_ICON_MESSAGE_DRAFTBOX;
    GUIMENU_InsertNode(i++, MMIMAIL_MAIN_MENU_NODE_DRAFTBOX, 0, &node_item, ctrl_id);

    if (PNULL != menu_string.wstr_ptr)
    {
        SCI_FREE(menu_string.wstr_ptr);
        menu_string.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//	Description : 更新邮箱列表界面主菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void UpdateMailMainMenu(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id                             
                             )
{
    MMI_STRING_T nstring = {0};
    MMI_STRING_T num_status = {0};
    uint16 i = 0;
    wchar number_string[MMIMAIL_MAX_MEMSTATE_LABEL_LEN + 1] = {0};
    MMI_STRING_T menu_string = {0};
    uint16 menu_wstr_len = (MMIMAIL_MAX_MEMSTATE_LABEL_LEN+1)*sizeof(wchar);
    
    num_status.wstr_ptr = number_string;
    menu_string.wstr_ptr = SCI_ALLOC_APP(menu_wstr_len);
    menu_string.wstr_len = MMIMAIL_MAX_MEMSTATE_LABEL_LEN;
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);

    //update new mail
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    MMI_GetLabelTextByLang(TXT_MAIL_NEW, &nstring);
    GUIMENU_ReplaceNode( i++, MMIMAIL_MAIN_MENU_NODE_WRITE_NEW_MAIL, 0, &nstring, ctrl_id );

    //update inbox
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_INBOX, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_INBOX);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
        );
    GUIMENU_ReplaceNode(i++, MMIMAIL_MAIN_MENU_NODE_INBOX, 0, &menu_string, ctrl_id);

    //update outbox
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_OUTBOX);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
        );
    GUIMENU_ReplaceNode(i++, MMIMAIL_MAIN_MENU_NODE_OUTBOX, 0, &menu_string, ctrl_id);

    //update sentbox
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_MAIL_SENTBOX, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_SENTBOX);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
        );
    GUIMENU_ReplaceNode(i++, MMIMAIL_MAIN_MENU_NODE_SENTBOX, 0, &menu_string, ctrl_id);

    //update draftbox
    SCI_MEMSET(&nstring, 0, sizeof(nstring));
    SCI_MEMSET((uint8 *)menu_string.wstr_ptr, 0, menu_wstr_len);
    MMI_GetLabelTextByLang(TXT_MAIL_DRAFT, &nstring);
    SetMailNumState2String(&num_status, MMIMAIL_DRAFTBOX);
    MMIAPICOM_MergeTwoString(
        &nstring,
        &num_status,
        &menu_string,
        MMIMAIL_MAX_MEMSTATE_LABEL_LEN
        );
    GUIMENU_ReplaceNode(i++, MMIMAIL_MAIN_MENU_NODE_DRAFTBOX, 0, &menu_string, ctrl_id);
    
    if (PNULL != menu_string.wstr_ptr)
    {
        SCI_FREE(menu_string.wstr_ptr);
        menu_string.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
// 	Description : 获取邮件数量信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void SetMailNumState2String(
                              MMI_STRING_T        *string_ptr,    //OUT:
                              MMIMAIL_BOX_TYPE_E   box_type       //IN:
                              )
{
    int16 total_num = 0;
    int16 unread_num = 0;
    uint8 num_state[10] = {0};

    if (MMIMAIL_INBOX == box_type)
    {
        total_num = MMIMAIL_GetBoxTotalNumber(box_type);
        unread_num = MMIMAIL_GetInboxBoxUnreadNum();

        string_ptr->wstr_len = sprintf((char *)num_state, "(%d/%d)", unread_num, total_num);
    }
    else
    {
        total_num = MMIMAIL_GetBoxTotalNumber(box_type);
        string_ptr->wstr_len = sprintf((char *)num_state, "(%d)", total_num);
    }

    MMI_STRNTOWSTR(string_ptr->wstr_ptr, string_ptr->wstr_len, (uint8 *)num_state, string_ptr->wstr_len, string_ptr->wstr_len);
}

/*****************************************************************************/
//	Description : 进入相应的邮箱
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void EnterMailCorrespondingMenu(
                                     MMIMAIL_MAIN_MENU_NODE_E node_id
                                     )
{
    switch (node_id)
    {
    case MMIMAIL_MAIN_MENU_NODE_WRITE_NEW_MAIL:
        MMIAPIMAIL_OpenEditWin(MMIMAIL_OPEN_TYPE_NEW_MAIL, PNULL);
        break;

    case MMIMAIL_MAIN_MENU_NODE_INBOX:
        MMIMAIL_EnterMailBox( MMIMAIL_INBOX);
        break;

    case MMIMAIL_MAIN_MENU_NODE_OUTBOX:
        MMIMAIL_EnterMailBox( MMIMAIL_OUTBOX );
        break;

    case  MMIMAIL_MAIN_MENU_NODE_SENTBOX:
        MMIMAIL_EnterMailBox( MMIMAIL_SENTBOX );
        break;
        
    case MMIMAIL_MAIN_MENU_NODE_DRAFTBOX:
        MMIMAIL_EnterMailBox( MMIMAIL_DRAFTBOX);
        break;

    default:
        //COMM_TRACE:"MMIMAIL==> EnterMailCorrespondingMenu node_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5683_112_2_18_2_48_35_480,(uint8*)"d", node_id);
        break;
    }
}

/*****************************************************************************/
//	Description : 设置当前按OK键的操作类型
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void SetHandleOkMenuOperation(
                                          MMIMAIL_OPERATION_T prev_operation
                                          )
{
    s_mmimail_info.ok_operation = prev_operation;
    return;
}

/*****************************************************************************/
//	Description : 获取之前按OK键的操作类型
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_OPERATION_T GetHandleOkMenuOperation(void)
{
    return s_mmimail_info.ok_operation;
}

/*****************************************************************************/
//	Description : 开始读取邮件，通知mail线程解码
//	Global resource dependence : none
//	Author:yuanl
//	Note: 从邮件列表界面进入读取过程
/*****************************************************************************/
LOCAL void StartReadMail(
                            MMIMAIL_BOX_TYPE_E box_type,
                            MMIMAIL_OPERATION_T oper_type, 
                            MMI_CTRL_ID_T ctrl_id //邮件列表控件id
                            )
{
    MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = PNULL;
    uint16          selected_index = 0;
    
    //检查控件id的有效性
    if(0 == ctrl_id)
    {
        //SCI_TRACE_LOW:"StartReadMail ctrl_id is 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5725_112_2_18_2_48_36_481,(uint8*)"");
        return;
    }
    
    //U盘使用过程中不能读邮件
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
    	return;
    }

    //保存当前操作类型
    SetHandleOkMenuOperation(oper_type);
    
    //保存当前正在操作的邮件
    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
    MMIMAIL_SetCurOperationMailByIndex(box_type, selected_index);
    
    cur_mail_ptr = MMIMAIL_GetCurOperationMail();
    
    //通知mail线程开始读取邮件
    MMIAPIMAIL_ReadMail(cur_mail_ptr->record_id);

    return;
}

/*****************************************************************************/
//	Description : 进入邮箱
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_Enter(MAIL_ENTER_TYPE_E   enter_type, DPARAM param)
{
    BOOLEAN is_start_success = FALSE;
    BOOLEAN is_need_password = FALSE;
    uint32  record_id = 0xffff;
    
    //进入手机邮箱时显示版本号
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_Enter: %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5762_112_2_18_2_48_36_482,(uint8*)"s", MAIL_HS_VERSION);
        
    if (!MMIMAIL_IsHaveLocalAccount())
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_Enter: no local account!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5766_112_2_18_2_48_36_483,(uint8*)"");
        return FALSE;
    }

    if (!MMIMAIL_IsReady())
    {
        //获取默认帐号信息
        MMIMAIL_GetDefAccount(&s_mmimail_info.def_account);

        //判断当前帐号是否需要输入密码
        is_need_password = mail_AccountNeedPassword(MMIMAIL_GetDefAccountId());
        if (is_need_password)
        {
            MMK_CreateWin((uint32 *)MMIMAIL_INPUT_PASSWORD_WIN_TAB,PNULL);
            return FALSE;
        }
        
        mail_fldrMgrInit();
    }
    
    if (MAIL_ENTER_MAINMENU == enter_type)//enter from mainmenu
    {
        //进入默认帐号邮箱列表
        MMIMAIL_OpenMailListWin();
        
        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_INBOX == enter_type)//enter mail box
    {
        //进入默认帐号 收件箱列表
        MMIMAIL_EnterMailBox(MMIMAIL_INBOX);

        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_NEWMAIL == enter_type)//enter editwin
    {
        //进入新建邮件界面
        MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_NEW_MAIL, (uint32)param);

        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_REPLYMAIL == enter_type)//enter editwin
    {
        //进入回复邮件界面
        record_id = (uint32)param;
        MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_REPLY, record_id);

        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_FORWARDMAIL == enter_type)//enter editwin
    {
        //进入转发编辑界面
        record_id = (uint32)param;
        MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_FORWARD_ALL, record_id);

        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_EDITMAIL == enter_type)//enter editwin
    {
        //进入编辑原始邮件界面
        record_id = (uint32)param;
        MMIMAIL_EnterEditWin(MMIMAIL_OPEN_TYPE_ORIGINAL, record_id);

        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_SETTINGWIN == enter_type)//enter setting win
    {
        //进入设置界面
        MMIMAIL_EnterSettingWin();

        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_READWIN == enter_type) //enter read win
    {
        //进入阅读界面
        record_id = (uint32)param;
        MMIMAIL_EnterReadWin(record_id);

        is_start_success = TRUE;
    }
    else if (MAIL_ENTER_SMS == enter_type)//enter from sms
    {
        
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_Enter: enter type [%d] is invalid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5852_112_2_18_2_48_36_484,(uint8*)"d", enter_type);
    }

    //用户已经进入手机邮箱，认为已知新邮件
    MMIMAIL_SetNewArrivalMsgNum(0);

    return is_start_success;
}

/*****************************************************************************/
//	Description : 保存帐号配置信息
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SaveAccountConfig(void)
{
    mail_WriteAllSettings();
}

/*****************************************************************************/
// 	Description : 获取默认帐号指定邮箱邮件总数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL uint16 MMIMAIL_GetBoxTotalNumber(
                                                     MMIMAIL_BOX_TYPE_E    box_type
                                                 )
{
    uint16 count = 0;
    
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    MMIMAIL_MAIL_SUMMERY_T *mail_list = MMIMAIL_GetMailListHead(box_type);

    while(mail_list != PNULL)
    {
        if(!mail_GetMailSecurityFlag(mail_list->record_id))//不在安全信箱中的邮件才会计数
        {
            count++;
        }
        mail_list = mail_list->next_ptr;
    }    
#else
    switch ( box_type )
    {
    case MMIMAIL_INBOX:
        count = s_mmimail_info.def_account_box_info.total_inbox_num;
        break;
        
    case MMIMAIL_OUTBOX:
        count = s_mmimail_info.def_account_box_info.total_outbox_num;
        break;
        
    case MMIMAIL_SENTBOX:
        count = s_mmimail_info.def_account_box_info.total_sentbox_num;
        break;
        
    case MMIMAIL_DRAFTBOX:
        count = s_mmimail_info.def_account_box_info.total_draftbox_num;
        break;
        
    default:
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetBoxTotalNumber: box_type = %d is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5911_112_2_18_2_48_36_485,(uint8*)"d", box_type);
        break;
    }
#endif
    
    return (count);
}

/*****************************************************************************/
// 	Description : 获取默认帐号收件箱未读邮件数目
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL uint16 MMIMAIL_GetInboxBoxUnreadNum(void)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    uint16 unread_num = 0;
    MMIMAIL_MAIL_SUMMERY_T *mail_list = s_mmimail_info.def_account_box_info.inbox_mail_list;
    while(mail_list != PNULL)
    {
        if((!mail_GetMailReadFlag(mail_list->record_id))&&(!mail_GetMailSecurityFlag(mail_list->record_id)))//不在安全信箱中的未读邮件才会计数
        {
            unread_num++;
        }
        mail_list = mail_list->next_ptr;
    }    
#else
    uint16 unread_num = s_mmimail_info.def_account_box_info.total_inbox_unread_num;
#endif
    
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetInboxBoxUnreadNum: unread_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5940_112_2_18_2_48_36_486,(uint8*)"d", unread_num);
    
    return unread_num;
}

/*****************************************************************************/
//	Description : 根据邮箱类型生成列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadMailList(
                                            MMIMAIL_BOX_TYPE_E  box_type, 
                                            MMI_CTRL_ID_T       ctrl_id
                                           )
                                           
{
    uint16 count = 0;
    
    count = MMIMAIL_GetBoxTotalNumber(box_type);
    
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_LoadMailList the count of list box is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_5959_112_2_18_2_48_36_487,(uint8*)"d", count);

    GUILIST_RemoveAllItems(ctrl_id);
    
    if (count > 0)
    {
        if (MMIMAIL_IsNeedSort())
        {
            MMIMAIL_SetNeedSort(FALSE);
            mail_InboxSort();
        }
        GUILIST_SetMaxItem(ctrl_id, count, TRUE);
        MMIMAIL_AppendAllItems(ctrl_id, box_type);
        return TRUE;        
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id, 1, FALSE);
        if (MMIMAIL_INBOX == box_type)
        {
            //收件箱邮件为空时可以操作选项菜单中的新建邮件和接收
            MMIMAIL_AppendEmptyStr2List(ctrl_id, TXT_BLANK, STXT_OPTION);
        }
        else
        {
            MMIMAIL_AppendEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);
        }
        return FALSE;
    }
}

/*****************************************************************************/
//	Description : 获取当前邮箱邮件链表指针头
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMIMAIL_MAIL_SUMMERY_T  *MMIMAIL_GetMailListHead(
                                                                MMIMAIL_BOX_TYPE_E  box_type
                                                              )
{
    MMIMAIL_MAIL_SUMMERY_T   *mail_list = PNULL;
    
    switch( box_type )
    {
    case MMIMAIL_INBOX:
        mail_list = s_mmimail_info.def_account_box_info.inbox_mail_list;
        break;
        
    case MMIMAIL_OUTBOX:
        mail_list = s_mmimail_info.def_account_box_info.outbox_mail_list;
        break;
        
    case MMIMAIL_SENTBOX:
        mail_list = s_mmimail_info.def_account_box_info.sentbox_mail_list;
        break;
        
    case MMIMAIL_DRAFTBOX:
        mail_list = s_mmimail_info.def_account_box_info.draftbox_mail_list;
        break;
        
    default:
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetMailListHead: the box_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6019_112_2_18_2_48_36_488,(uint8*)"d", box_type);
        break;
    }   

    return mail_list;
}

/*****************************************************************************/
//	Description : 获取与list_index对应的邮件指针
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMIMAIL_MAIL_SUMMERY_T  *MMIMAIL_GetCurMailPtr(
                                                            MMIMAIL_BOX_TYPE_E  box_type,
                                                            uint16              list_index
                                                           )
{
    MMIMAIL_MAIL_SUMMERY_T *head_ptr = PNULL;
    MMIMAIL_MAIL_SUMMERY_T *cur_ptr = PNULL;
    
    head_ptr = MMIMAIL_GetMailListHead(box_type);
    
    cur_ptr = head_ptr;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT    
    while(cur_ptr != PNULL)
    {
        if((cur_ptr->item_index == list_index) && (!mail_GetMailSecurityFlag(cur_ptr->record_id)))//获取不在安全信箱中的邮件指针
        {
            break;
        }
        cur_ptr = cur_ptr->next_ptr;
    }
#else
    while ((cur_ptr != PNULL) && (list_index >0))
    {
        cur_ptr = cur_ptr->next_ptr;
        list_index --;
    }
#endif
    return cur_ptr;
}

/*****************************************************************************/
//	Description : 根据邮件数量添加列表项
//	Global resource dependence : none
//	Author:yuanl
//	Note:此处为空项，等显示时再获取列表实际显示内容
/*****************************************************************************/
LOCAL void MMIMAIL_AppendAllItems(
                                            MMI_CTRL_ID_T       ctrl_id,
                                            MMIMAIL_BOX_TYPE_E  box_type
                                          )
{  
    MMIMAIL_MAIL_SUMMERY_T   *mail_list = PNULL;
    GUILIST_ITEM_T          item_info = {0};
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
    uint32 item_index = 0;
#endif
    
    mail_list = MMIMAIL_GetMailListHead(box_type);

    item_info.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
    
    while (PNULL != mail_list)
    {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
        if(!mail_GetMailSecurityFlag(mail_list->record_id))//安全信箱中的邮件不能添加到139邮箱的列表中
        {
            mail_list->item_index = item_index;
            GUILIST_AppendItem(ctrl_id, &item_info);
            item_index++;
        }
        mail_list = mail_list->next_ptr;
#else
        GUILIST_AppendItem(ctrl_id, &item_info);
        mail_list = mail_list->next_ptr;
#endif
    }

    return;
}

/*****************************************************************************/
//	Description : 设置列表项信息
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_AppendListItem(
                              wchar                    *string_ptr,
                              uint16                    string_len,
                              MMI_CTRL_ID_T             ctrl_id,
                              MMI_TEXT_ID_T             left_softkey_id,
                              MMI_TEXT_ID_T             mid_softkey_id,
                              uint16                    pos,
                              GUIITEM_STYLE_E           item_style,
                              MMIMAIL_LIST_ITEM_TYPE_E  type
                              )
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    if(PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMIMAIL_AppendListItem string_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6118_112_2_18_2_48_36_489,(uint8*)"");
        return;
    }
    
    item_t.item_style = item_style;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;

    switch( type )
    {
    case MMIMAIL_APPEND_LIST_ITEM_TYPE:
        GUILIST_AppendItem(ctrl_id, &item_t);
        break;
        
    case MMIMAIL_REPLACE_LIST_ITEM_TYPE:
        GUILIST_ReplaceItem(ctrl_id, &item_t, pos);
        break;
        
    case MMIMAIL_INSERT_LIST_ITEM_TYPE:
        GUILIST_InsertItem(ctrl_id, &item_t, pos);
        break;
        
    case MMIMAIL_SET_LIST_ITEM_TYPE:
        GUILIST_SetItemData(ctrl_id,&item_data, pos);
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//	Description : 设置空列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,         //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: 
                             MMI_TEXT_ID_T    left_softkey_id  //IN:
                             )
{
    MMI_STRING_T empty_str = {0};

    MMI_GetLabelTextByLang(empty_txt_id, &empty_str);
    MMIMAIL_AppendListItem(
                    empty_str.wstr_ptr,
                    empty_str.wstr_len,
                    ctrl_id,
                    left_softkey_id,
                    TXT_NULL,
                    0,
                    GUIITEM_STYLE_ONE_LINE_TEXT,
                    MMIMAIL_SET_LIST_ITEM_TYPE
                    );
}

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
/*****************************************************************************/
//	Description : 根据邮件状态获取显示图标
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T MMIMAIL_GetIconIdByState( MMIMAIL_STATE_E  state, BOOLEAN is_locked)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

    switch (state)
    {
        case MMIMAIL_STATE_READ:
            image_id = is_locked ? IMAGE_MAIL_LOCK_READ_ICON : IMAGE_MAIL_READ_ICON;
            break;
            
        case MMIMAIL_STATE_UNREAD:
            image_id = is_locked ? IMAGE_MAIL_LOCK_UNREAD_ICON : IMAGE_MAIL_UNREAD_ICON;
            break;
            
        case MMIMAIL_STATE_READ_WITH_ACC:
            image_id = is_locked ? IMAGE_MAIL_LOCK_READ_WITH_ACC_ICON : IMAGE_MAIL_READ_WITH_ACC_ICON;
            break;
            
        case MMIMAIL_STATE_UNREAD_WITH_ACC:
            image_id = is_locked ? IMAGE_MAIL_LOCK_UNREAD_WITH_ACC_ICON : IMAGE_MAIL_UNREAD_WITH_ACC_ICON;
            break;
            
        case MMIMAIL_STATE_SERVER_REJECT:
            image_id = is_locked ? IMAGE_MAIL_LOCK_SERVER_REJECT_ICON : IMAGE_MAIL_SERVER_REJECT_ICON;
            break;
                       
        case MMIMAIL_STATE_SEND_FAIL:
            image_id = is_locked ? IMAGE_MAIL_LOCK_SEND_FAIL_ICON : IMAGE_MAIL_SEND_FAIL_ICON;
            break;
                       
        case MMIMAIL_STATE_WAIT_SEND:
            image_id = is_locked ? IMAGE_MAIL_LOCK_WAIT_SEND_ICON : IMAGE_MAIL_WAIT_SEND_ICON;
            break;

        case MMIMAIL_STATE_SENDING:
            image_id = is_locked ? IMAGE_MAIL_LOCK_SENDING_ICON : IMAGE_MAIL_SENDING_ICON;
            break;
            
        case MMIMAIL_STATE_SENT:
            image_id = is_locked ? IMAGE_MAIL_LOCK_SENT_ICON : IMAGE_MAIL_SENT_ICON;
            break;
            
        case MMIMAIL_STATE_SENT_WITH_ACC:
            image_id = is_locked ? IMAGE_MAIL_LOCK_SENT_WITH_ACC_ICON : IMAGE_MAIL_SENT_WITH_ACC_ICON;
            break;
            
        case MMIMAIL_STATE_DRAFT:
            image_id = is_locked ? IMAGE_MAIL_LOCK_DRAFT_ICON : IMAGE_MAIL_DRAFT_ICON;
            break;
            
        case MMIMAIL_STATE_DRAFT_WITH_ACC:
            image_id = is_locked ? IMAGE_MAIL_LOCK_DRAFT_WITH_ACC_ICON : IMAGE_MAIL_DRAFT_WITH_ACC_ICON;
            break;
            
        default:
            break;
    }

    return image_id;
}
#else
/*****************************************************************************/
//	Description : 根据邮件状态获取显示图标
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T MMIMAIL_GetIconIdByState( MMIMAIL_STATE_E  state)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

    switch (state)
    {
        case MMIMAIL_STATE_READ:
            image_id = IMAGE_MAIL_READ_ICON;
            break;
            
        case MMIMAIL_STATE_UNREAD:
            image_id = IMAGE_MAIL_UNREAD_ICON;
            break;
            
        case MMIMAIL_STATE_READ_WITH_ACC:
            image_id = IMAGE_MAIL_READ_WITH_ACC_ICON;
            break;
            
        case MMIMAIL_STATE_UNREAD_WITH_ACC:
            image_id = IMAGE_MAIL_UNREAD_WITH_ACC_ICON;
            break;
            
        case MMIMAIL_STATE_SERVER_REJECT:
            image_id = IMAGE_MAIL_SERVER_REJECT_ICON;
            break;
                       
        case MMIMAIL_STATE_SEND_FAIL:
            image_id = IMAGE_MAIL_SEND_FAIL_ICON;
            break;
                       
        case MMIMAIL_STATE_WAIT_SEND:
            image_id = IMAGE_MAIL_WAIT_SEND_ICON;
            break;

        case MMIMAIL_STATE_SENDING:
            image_id = IMAGE_MAIL_SENDING_ICON;
            break;
            
        case MMIMAIL_STATE_SENT:
            image_id = IMAGE_MAIL_SENT_ICON;
            break;
            
        case MMIMAIL_STATE_SENT_WITH_ACC:
            image_id = IMAGE_MAIL_SENT_WITH_ACC_ICON;
            break;
            
        case MMIMAIL_STATE_DRAFT:
            image_id = IMAGE_MAIL_DRAFT_ICON;
            break;
            
        case MMIMAIL_STATE_DRAFT_WITH_ACC:
            image_id = IMAGE_MAIL_DRAFT_WITH_ACC_ICON;
            break;
            
        default:
            break;
    }

    return image_id;
}
#endif

/*****************************************************************************/
//	Description : 生成列表内容
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_FormListBoxItem(
                                               MMIMAIL_MAIL_SUMMERY_T    *mail_ptr,
                                               MMI_CTRL_ID_T     ctrl_id,
                                               uint16            index
                                           )
{
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T subject = {0};
    MMI_STRING_T name = {0}; 
    MMI_STRING_T time = {0};
    uint8 time_string[MMIMAIL_MAX_LIST_DISP_LEN] = {0};
    MMI_TM_T mail_time = {0};
    SCI_DATE_T cur_date = {0};
    uint16 list_index = 0;
    uint16 copy_len = 0;
    wchar temp_arr1[MMIMAIL_MAX_LIST_DISP_LEN + 1] = {0};
    wchar temp_arr2[MMIMAIL_MAX_LIST_DISP_LEN + 1] = {0};
    wchar temp_arr3[MMIMAIL_MAX_LIST_DISP_LEN + 1] = {0};
    
    subject.wstr_ptr = temp_arr1;
    time.wstr_ptr    = temp_arr2;
    name.wstr_ptr    = temp_arr3;
    
    //显示邮件地址
    copy_len = MMIAPICOM_Wstrlen(mail_ptr->addr);
    copy_len = MIN(copy_len, MMIMAIL_MAX_LIST_DISP_LEN);
    MMIAPICOM_Wstrncpy(name.wstr_ptr, mail_ptr->addr, copy_len);
    name.wstr_len = MMIAPICOM_Wstrlen(name.wstr_ptr);
    
    //获取标题
    copy_len = MMIAPICOM_Wstrlen(mail_ptr->subject);
    copy_len = MIN(copy_len, MMIMAIL_MAX_LIST_DISP_LEN);
    MMIAPICOM_Wstrncpy(subject.wstr_ptr, mail_ptr->subject, copy_len);
    subject.wstr_len = MMIAPICOM_Wstrlen(subject.wstr_ptr);
    
    //生成时间字符串
    TM_GetSysDate(&cur_date);
    mail_time = MMIAPICOM_Second2Tm(mail_ptr->time);
    if ((mail_time.tm_mday == cur_date.mday)
        && (mail_time.tm_mon == cur_date.mon) 
        && (mail_time.tm_year == cur_date.year))
    {
        time.wstr_len = sprintf((char *)time_string, "%d:%02d", mail_time.tm_hour, mail_time.tm_min);
    }
    else
    {
        time.wstr_len = sprintf((char *)time_string, "%d/%d", mail_time.tm_mon, mail_time.tm_mday);
    }

    MMI_STRNTOWSTR(time.wstr_ptr,
                    MMIMAIL_MAX_LIST_DISP_LEN,
                    (uint8 *)time_string,
                    time.wstr_len,
                    time.wstr_len
                    );

    //设置邮件状态图标
    list_index = 0;
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
    item_data.item_content[list_index].item_data.image_id = MMIMAIL_GetIconIdByState(mail_ptr->state, mail_GetMailLockFlag(mail_ptr->record_id));
#else
    item_data.item_content[list_index].item_data.image_id = MMIMAIL_GetIconIdByState(mail_ptr->state);
#endif

    //设置发件人信息
    list_index++;
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = name.wstr_len;
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = name.wstr_ptr;

    //设置时间信息
    list_index++;
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = time.wstr_len;
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = time.wstr_ptr;

    //设置标题信息
    list_index++;
    item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = subject.wstr_len;
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;

    //设置软键
    item_data.softkey_id[LEFT_BUTTON]      = STXT_OPTION;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;

    GUILIST_SetItemData( ctrl_id, &item_data, index );
}

/*****************************************************************************/
//	Description : 设置列表项数据
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetListItemData(
                                            MMIMAIL_BOX_TYPE_E box_type, 
                                            MMI_CTRL_ID_T      ctrl_id,
                                            uint16             index
                                           )
{
    MMIMAIL_MAIL_SUMMERY_T   *cur_mail_ptr = PNULL;
    
    cur_mail_ptr = MMIMAIL_GetCurMailPtr(box_type, index);
    
    if (cur_mail_ptr != PNULL)
    {  
        MMIMAIL_FormListBoxItem(            
                                cur_mail_ptr,
                                ctrl_id,
                                index
                                );        

        if (cur_mail_ptr->is_marked)
        {
            GUILIST_SetTextListMarkable(ctrl_id, TRUE);
            GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
        }
    }
    else
    {
        MMIMAIL_SetEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);        
    }
}

/*****************************************************************************/
//	Description : 获取当前焦点子窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_WIN_ID_T MMIMAIL_GetCurFocusChildBoxWinId(void)
{
    MMI_WIN_ID_T cur_child_winid = 0;

    switch(MMIMAIL_GetCurBoxType())
    {
        case MMIMAIL_INBOX:
            cur_child_winid = MMIMAIL_INBOX_CHILD_WIN_ID;
            break;
            
        case MMIMAIL_OUTBOX:
            cur_child_winid = MMIMAIL_OUTBOX_CHILD_WIN_ID;
            break;
            
        case MMIMAIL_SENTBOX:
            cur_child_winid = MMIMAIL_SENTBOX_CHILD_WIN_ID;
            break;
            
        case MMIMAIL_DRAFTBOX:
            cur_child_winid = MMIMAIL_DRAFTBOX_CHILD_WIN_ID;
            break;
            
        default:
            break;
    }

    return cur_child_winid;
}

/*****************************************************************************/
//	Description : 获取当前焦点列表控件id
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T MMIMAIL_GetCurActiveListCtrlId(void)
{
    MMI_CTRL_ID_T cur_list_ctrlid = 0;

    switch (MMIMAIL_GetCurBoxType())
    {
        case MMIMAIL_INBOX:
            cur_list_ctrlid = MMIMAIL_INBOX_LISTBOX_CTRL_ID;
            break;
            
        case MMIMAIL_OUTBOX:
            cur_list_ctrlid = MMIMAIL_OUTBOX_LISTBOX_CTRL_ID;
            break;
            
        case MMIMAIL_SENTBOX:
            cur_list_ctrlid = MMIMAIL_SENTBOX_LISTBOX_CTRL_ID;
            break;
            
        case MMIMAIL_DRAFTBOX:
            cur_list_ctrlid = MMIMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
            break;
            
        default:
            break;
    }

    return cur_list_ctrlid;
}

/*****************************************************************************/
//	Description : 获取当前邮件是否被标记
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_GetCurMailIsMarked(void )
{   
    MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = PNULL;
    BOOLEAN           is_marked = FALSE;
    
    cur_mail_ptr = MMIMAIL_GetCurOperationMail();
    
    if (PNULL != cur_mail_ptr)
    {
        is_marked = cur_mail_ptr->is_marked;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetCurMailIsMarked: cur_mail_ptr is null !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6525_112_2_18_2_48_37_490,(uint8*)"");
    }
    
    return is_marked;
}

/*****************************************************************************/
//	Description : 标记当前邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurMailMarked(
                                   BOOLEAN is_marked
                                   )
{
    MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = MMIMAIL_GetCurOperationMail();
        
    if (PNULL != cur_mail_ptr)
    {
        cur_mail_ptr->is_marked = is_marked;
        mail_setDelFlag(cur_mail_ptr->record_id, is_marked);
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_SetCurMailMarked: cur_mail_ptr is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6548_112_2_18_2_48_37_491,(uint8*)"");
    }
}

/*****************************************************************************/
//	Description : 标记指定邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetMailMarked(
                                           MMIMAIL_BOX_TYPE_E   box_type,
                                           uint16               index,               
                                           BOOLEAN              is_marked
                                         )
{   
    MMIMAIL_MAIL_SUMMERY_T   *mail_ptr = PNULL;

    //获取当前邮箱邮件链表头
    mail_ptr = MMIMAIL_GetCurMailPtr(box_type, index);

    if (PNULL != mail_ptr)
    {
        mail_ptr->is_marked = is_marked;
        mail_setDelFlag(mail_ptr->record_id, is_marked);
    }
    
    return;
}

/*****************************************************************************/
//	Description : 标记当前邮箱所有邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetAllMailMarked(
                                    MMIMAIL_BOX_TYPE_E  box_type,
                                    BOOLEAN is_marked
                                    )
{
    MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr = PNULL;
    
    cur_mail_ptr = MMIMAIL_GetMailListHead(box_type);
    
    while (cur_mail_ptr != PNULL)
    {
        cur_mail_ptr->is_marked = is_marked;
        mail_setDelFlag(cur_mail_ptr->record_id, is_marked);
        cur_mail_ptr = cur_mail_ptr->next_ptr;
    }

    return;
}

/*==========================================================
 * author		: liangshun      
 * function 	: LOCAL BOOLEAN MMIMAIL_HasMailBeenMarked(MMIMAIL_BOX_TYPE_E  box_type )
 * decr		: 判断是否有邮件被标记
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/13/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_HasMailBeenMarked(MMIMAIL_BOX_TYPE_E  box_type )
{   
    MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = PNULL;
    BOOLEAN     has_mail_been_marked = FALSE;
    
    cur_mail_ptr = MMIMAIL_GetMailListHead(box_type);
    
    while (cur_mail_ptr != PNULL)
    {
        if(cur_mail_ptr->is_marked)
        {
            has_mail_been_marked = TRUE;
            break;
        }
        cur_mail_ptr = cur_mail_ptr->next_ptr;
    }
    
    return has_mail_been_marked;
}

/*****************************************************************************/
//	Description : 获取是否全部邮件已被标记
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsAllMailMarked(MMIMAIL_BOX_TYPE_E  box_type )
{   
    MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = PNULL;
    BOOLEAN     is_all_marked = FALSE;
    uint16      total_num = MMIMAIL_GetBoxTotalNumber(box_type);
    uint16      count = 0;
    
    cur_mail_ptr = MMIMAIL_GetMailListHead(box_type);
    
    while ((cur_mail_ptr != PNULL) && (cur_mail_ptr->is_marked))
    {
        count++;
        cur_mail_ptr = cur_mail_ptr->next_ptr;
    }

    if ((0 != total_num) && (total_num == count))
    {
        is_all_marked = TRUE;
    }
    
    return is_all_marked;
}

/*****************************************************************************/
//	Description : 获取当前邮箱删除邮件数目
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL uint32 MMIMAIL_GetMailboxDeleteAvailNum(MMIMAIL_BOX_TYPE_E box_type)
{
    MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr = PNULL;
    uint32 delete_mail_num = 0;

    if (MMIAPIUDISK_UdiskIsRun())
    {
        //U盘使用中不能删除邮件
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
    }
    else
    {
        cur_mail_ptr = MMIMAIL_GetMailListHead(box_type);
        
        while (PNULL != cur_mail_ptr)
        {
            if (cur_mail_ptr->is_marked)
            {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
                if(!mail_GetMailLockFlag(cur_mail_ptr->record_id)
                && !mail_GetMailSendingFlag(cur_mail_ptr->record_id))
#endif
                delete_mail_num++;
            }
            
            cur_mail_ptr = cur_mail_ptr->next_ptr;
        }
    }
    
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetMailboxDeleteAvailNum delete_mail_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6687_112_2_18_2_48_38_492,(uint8*)"d", delete_mail_num);

    return delete_mail_num;
}

/*****************************************************************************/
//	Description : 删除完毕回调函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL  BOOLEAN MMIMAIL_HandleListChangeCallBack (void)
{
    //如果邮箱窗口开启，则通知邮箱更新列表
    if (MMK_IsOpenWin(MMIMAIL_MAILBOX_MAIN_WIN_ID))
    {
        MMK_SendMsg(MMIMAIL_MAILBOX_MAIN_WIN_ID, 
            MSG_MAIL_UPDATE_LIST, 
            PNULL); 
    } 

    //如果邮箱列表主菜单开启，则通知其更新菜单显示，邮件数量信息改变
    if (MMK_IsOpenWin(MMIMAIL_MAIN_MENU_WIN_ID))
    {
        if (MMIMAIL_MAIN_MENU_WIN_ID != MMK_GetFocusWinId())
        {
            //邮箱列表主菜单非焦点窗口时，先设置其需要更新列表状态
            MMK_SendMsg(MMIMAIL_MAIN_MENU_WIN_ID,
                MSG_MAIL_NEEDUPDATEMENU,
                PNULL);
        }
        else
        {
            MMK_SendMsg(MMIMAIL_MAIN_MENU_WIN_ID, 
                MSG_MAIL_UPDATE_MENU, 
                PNULL);
        }
    }

    //更新icon区显示
    MAIN_SetMsgState(TRUE);

    return TRUE;
}

/*****************************************************************************/
//	Description : 处理MSG_MAIL_RETRATTACH_RESULT消息
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_HandleRetrAttachResult(mail_retr_result_t  *retr_result)
{
    //下载附件或手动接收邮件界面存在时才提示用户接收结果，否则有时会莫名其妙的多弹出一些提示窗口
    if (MMK_IsOpenWin(MMIMAIL_DL_ACCESSORY_WAITING_WIN_ID)
        || MMK_IsOpenWin(MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID))
    {
    //关闭下载附件等待窗口
    if (MMK_IsOpenWin(MMIMAIL_DL_ACCESSORY_WAITING_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_DL_ACCESSORY_WAITING_WIN_ID);
    }

    //关闭接收邮件进度窗口
    if (MMK_IsOpenWin(MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID);
    }
    
    if (PNULL != retr_result)
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_HandleRetrAttachResult: retr_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6754_112_2_18_2_48_38_493,(uint8*)"d", retr_result->retr_result);
        switch (retr_result->retr_result)
        {
        case MAIL_RETR_SUCCESS:
            MMIMAIL_OpenMsgBox(TXT_COMPLETE, IMAGE_PUBWIN_SUCCESS);
            break;

        case MAIL_RETR_ERR_NOSPACE:
            MMIMAIL_OpenMsgBox(TXT_MAIL_NO_MEMORY, IMAGE_PUBWIN_WARNING);
            break;            
        case MAIL_RETR_ERR_BOXFULL:
            MMIMAIL_OpenMsgBox(TXT_MAIL_FULLSPACE, IMAGE_PUBWIN_WARNING);
            break;
            
        case MAIL_RETR_ERR_SENDING:
            MMIMAIL_OpenMsgBox(TXT_MAIL_SENDING, IMAGE_PUBWIN_WARNING);
            break;

        case MAIL_RETR_ERR_TOLARGE:
            MMIMAIL_OpenMsgBox(TXT_MAIL_ACC_TOO_LARGE, IMAGE_PUBWIN_WARNING);
            break;

        case MAIL_RETR_ERR_RETRING:
            MMIMAIL_OpenMsgBox(TXT_COMMON_RECEIVING, IMAGE_PUBWIN_WARNING);
            break;
            
        default:
            MMIMAIL_OpenMsgBox(TXT_COMMON_RECEIVE_FAIL, IMAGE_PUBWIN_WARNING);
            break;
        }

        comm_free(retr_result);/*lint !e774*/
        }    
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_HandleRetrAttachResult: MMIMAIL_DL_ACCESSORY_WAITING_WIN_ID and MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6790_112_2_18_2_48_38_494,(uint8*)"");
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_HandleRetrAttachResult: retr_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6791_112_2_18_2_48_38_495,(uint8*)"d", retr_result->retr_result);
        if(MMK_IsFocusWin(MMIMAIL_READ_ACCESSORY_WIN_ID))//added by liangshun 20110114 for 附件下载完成后刷新附件列表窗口的softkey
        {
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_HandleRetrAttachResult Full Paint ACCESSORY WIN!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6794_112_2_18_2_48_38_496,(uint8*)"");
            MMK_SendMsg(MMIMAIL_READ_ACCESSORY_WIN_ID, MSG_GET_FOCUS, PNULL);
            MMK_SendMsg(MMIMAIL_READ_ACCESSORY_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }
}

/*****************************************************************************/
//	Description : 检查邮件密码完毕处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CheckPwFinished(BOOLEAN is_success)
{
    if (MMK_IsOpenWin(MMIMAIL_CHECK_PW_WAITING_WIN_ID))
    {
        MMIPUB_CloseWaitWin(MMIMAIL_CHECK_PW_WAITING_WIN_ID);
        
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: is_success = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6811_112_2_18_2_48_38_497,(uint8*)"d", is_success);
        //密码正确则进入邮箱，否则提示密码错误
        if (is_success)
        {
            if (MMK_IsOpenWin(MMIMAIL_EDIT_ACCOUNT_PW_WIN_ID))
            {
                //帐号编辑界面，进行密码修改同步操作
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: edit password window"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6818_112_2_18_2_48_38_498,(uint8*)"");
                MMK_PostMsg(MMIMAIL_EDIT_ACCOUNT_PW_WIN_ID, MSG_MAIL_CHECK_PASSWD_RESULT, (DPARAM)&is_success, sizeof(BOOLEAN));
            }
            else if (MMK_IsOpenWin(MMIMAIL_SET_ACCOUNT_WIN_ID))
            {
                //切换帐号
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: switch account window"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6824_112_2_18_2_48_38_499,(uint8*)"");
                MMK_PostMsg(MMIMAIL_SET_ACCOUNT_WIN_ID, MSG_MAIL_CHECK_PASSWD_RESULT, PNULL, 0);
            }
            else
            {
                //进入应用
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: enter app"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6830_112_2_18_2_48_38_500,(uint8*)"");
                MMIMAIL_Enter(s_mmimail_info.enter_type, s_mmimail_info.enter_param);
            }
        }
        else
        {
            uint32 acc_num = mail_GetAccountNum();

            //MMIMAIL_OpenMsgBox(TXT_KL_PSW_WRONG_PSW, IMAGE_PUBWIN_WARNING);
            MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                                        
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: account num = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6841_112_2_18_2_48_38_501,(uint8*)"d", acc_num);
            
            if (MMK_IsOpenWin(MMIMAIL_EDIT_ACCOUNT_PW_WIN_ID))
            {
                //帐号编辑界面，进行密码修改同步操作
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: edit password window"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6846_112_2_18_2_48_38_502,(uint8*)"");
                MMK_PostMsg(MMIMAIL_EDIT_ACCOUNT_PW_WIN_ID, MSG_MAIL_CHECK_PASSWD_RESULT, (DPARAM)&is_success, sizeof(BOOLEAN));
            }            
            else if (MMK_IsOpenWin(MMIMAIL_SET_ACCOUNT_WIN_ID) && (1 == acc_num))
            {
                //当前只有一个帐号且密码输入错误
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: switch account window"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6852_112_2_18_2_48_38_503,(uint8*)"");
                MMIAPIMAIL_Exit();
            }
            else
            {
                //进入应用
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_CheckPwFinished: enter app"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6858_112_2_18_2_48_38_504,(uint8*)"");
            }
        }
    }
}

/*****************************************************************************/
//	Description : 删除邮件完毕处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_DelFinished(BOOLEAN is_success)
{
    if (MMK_IsOpenWin(MMIMAIL_SET_ACCOUNT_WIN_ID))
    {
        if (s_mmimail_info.is_del_account_oper_by_bomb)
        {
            //恢复标记位
            s_mmimail_info.is_del_account_oper_by_bomb = FALSE;
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_DelFinished: Account del finish by Bomb!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6876_112_2_18_2_48_38_505,(uint8*)"");
            //在139邮箱任意窗口中收到炸弹消息后需要退出手机邮箱
            //否则139邮箱列表显示会有问题
            MMIAPIMAIL_Exit();
        }
        else
        {
            //删除帐号界面调用的删除操作，不需要提示消息
            MMK_PostMsg(MMIMAIL_SET_ACCOUNT_WIN_ID, MSG_MAIL_DEL_ACCOUNT_FINISH, 0, 0);
        }
        return;
    }
    
    if (s_mmimail_info.is_logout_oper) //注销邮箱引起的删除邮件操作
    {
        uint16 account_num = mail_GetAccountNum();
        uint16 i = 0;

        //删除本地所有帐号
        for (i = 0; i < account_num; i++)
        {
            //每次执行完之后,mail_settings.account_num都会减1,所以需要循环删除最前面的账户
            //否则不是所有账户都能删除
            mail_DeleteAccountByIndex(0);//i->0 changed by liangshun 20110422
        }
    }
    
    if (MMK_IsOpenWin(MMIMAIL_DEL_WAITING_WIN_ID))
    {
        MMIPUB_CloseWaitWin(MMIMAIL_DEL_WAITING_WIN_ID);
        
        //邮件列表界面或邮件阅读界面调用的删除操作
        if (is_success)
        {
            MMIMAIL_OpenMsgBox(TXT_COMPLETE, IMAGE_PUBWIN_SUCCESS);
        }
        else
        {
            MMIMAIL_OpenMsgBox(TXT_COMMON_DEL_FAIL, IMAGE_PUBWIN_WARNING);
        }
    }

    if (s_mmimail_info.is_logout_oper)
    {
        //恢复标记位
        s_mmimail_info.is_logout_oper = FALSE;
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_DelFinished: Account logoff finish!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6922_112_2_18_2_48_38_506,(uint8*)"");
        //退出手机邮箱
        MMIAPIMAIL_Exit();
    }
    else if (s_mmimail_info.is_del_account_oper_by_bomb)
    {
        //恢复标记位
        s_mmimail_info.is_del_account_oper_by_bomb = FALSE;
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_DelFinished: Account del finish by Bomb!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_6930_112_2_18_2_48_38_507,(uint8*)"");
        //在139邮箱任意窗口中收到炸弹消息后需要退出手机邮箱
        //否则139邮箱列表显示会有问题
        MMIAPIMAIL_Exit();
   }
    return;
}

/*****************************************************************************/
//	Description : 恢复出厂设置结束处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ResetFinished(BOOLEAN is_success)
{
    if (MMK_IsOpenWin(MMIMAIL_OVERWRITE_WAITING_WIN_ID))
    {
        //删除旧帐号及邮件结束
        
        //保存新帐号配置
        MMIMAIL_SaveAccountConfig();

        //关闭等待窗口
        MMK_CloseWin(MMIMAIL_OVERWRITE_WAITING_WIN_ID);
        
        //重新进入邮箱
        MMIMAIL_Enter(s_mmimail_info.enter_type, s_mmimail_info.enter_param);
        return;
    }

    if (MMK_IsOpenWin(MMIMAIL_DEL_EXIST_ACCOUNT_WAIT_WIN_ID))
    {
        //删除旧帐号及邮件结束

        //关闭等待窗口
        MMK_CloseWin(MMIMAIL_DEL_EXIST_ACCOUNT_WAIT_WIN_ID);
        
        //进入开通流程
        MMIMAIL_OpenWelcomeWin();
        return;
    }

    //关闭等待窗口
    if (MMK_IsOpenWin(MMIMAIL_RESET_WAITING_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_RESET_WAITING_WIN_ID);
    }

    //提示已完成或操作失败
    if (is_success)
    {
        MMIMAIL_OpenMsgBox(TXT_COMPLETE, IMAGE_PUBWIN_WARNING);
    }
    else
    {
        MMIMAIL_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
    }
    
    //退出手机邮箱
    MMIAPIMAIL_Exit();
    
}

/*****************************************************************************/
//  Description : 处理邮件接收进度
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailProgressWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);    
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
        
    case MSG_APP_RED:    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //取消接收邮件
        mail_CancelRetringManual();
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 邮件下载进度更新
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ProgressUpdateCallBack(mail_ProgressInfo *progress_info)
{
    MMI_WIN_ID_T    progress_win_id = MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID;
    uint16          cur_progress = 0;
    uint16          total_progress = 0;

    if (PNULL != progress_info)
    {
        cur_progress = progress_info->msgIndex;
        total_progress = progress_info->totNumOfMsg;
        
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_ProgressUpdateCallBack: cur_progress = %d, total_progress = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7041_112_2_18_2_48_38_508,(uint8*)"dd", cur_progress, total_progress);
        
        if(!MMK_IsOpenWin(progress_win_id))
        {
            MMIPUB_OpenProgressWinByTextId(TXT_MAIL_RECV_MAIL_WAIT, &progress_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleMailProgressWinMsg);
        }
        
        MMIPUB_SetProgressTotalSize(&progress_win_id, total_progress);
        MMIPUB_UpdateProgressBar(&progress_win_id, cur_progress);
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_ProgressUpdateCallBack: progress_info is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7053_112_2_18_2_48_38_509,(uint8*)"");
    }
}

/*****************************************************************************/
//	Description : 手动发起接收邮件操作
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ReceiveMail(void)
{
    uint16          acc_id = MMIMAIL_GetDefAccountId();
    MMI_WIN_ID_T    progress_win_id = MMIMAIL_RECV_MAIL_PROGRESS_WIN_ID;

#ifdef WIN32
    mail_retrieve_test(); //该函数可以在模拟器环境下接收邮件
#else    
    mail_retrieveMsgManual(acc_id);

    //创建接收邮件进度窗口
    MMIPUB_OpenProgressWinByTextId(TXT_MAIL_RECV_MAIL_WAIT, &progress_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleMailProgressWinMsg);
    MMIPUB_SetProgressTotalSize(&progress_win_id, 0);
#endif
}

/*****************************************************************************/
//	Description : 邮件中是否有接收邮件地址
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsHaveReceiver(MMIMAIL_MAIL_SUMMERY_T  *cur_mail_ptr)
{
    BOOLEAN is_have = FALSE;
    //MAIL_MESSAGE_T *whole_mail_ptr = PNULL;

    if(PNULL == cur_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMAIL_IsHaveReceiver cur_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7088_112_2_18_2_48_38_510,(uint8*)"");
        return FALSE;
    }

    if (MMIAPICOM_Wstrlen(cur_mail_ptr->addr) > 0)
    {
        is_have = TRUE;
    }

    return is_have;
}

/*****************************************************************************/
//	Description : 是否允许修改密码
//	Global resource dependence : none
//	Author: yuanl
//	Note: 企业版帐号允许修改密码，个人用户不能修改密码
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsPermitModifyPassword(void)
{
    //判断是否是企业版帐号
    return mail_AccountIsGroupAccount(s_mmimail_info.cur_edit_account.accountid);

}

/*****************************************************************************/
//	Description : 加载帐号列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadAccountList(
                                            MMI_CTRL_ID_T               ctrl_id,
                                            MMIMAIL_LIST_ITEM_TYPE_E    type
                                           )
                                           
{
    uint16              i = 0;
    uint16              account_num = 0;
    MMIMAIL_ACCOUNT_INFO_T account_info = {0};
    uint16              def_acc_id = 0;
    uint16              cur_sel = 0;
    BOOLEAN             have_items = FALSE;
    
    account_num = mail_GetAccountNum();

    if (account_num > 0)
    {
        for (i = 0; i < account_num; i++)
        {
            mail_GetAccountByIndex(i, &account_info);
            if (MMIAPICOM_Wstrlen(account_info.name) > 0)
            {
                MMIMAIL_AppendListItem(
                                account_info.name,
                                MMIAPICOM_Wstrlen(account_info.name),
                                ctrl_id,
                                STXT_OPTION,
                                TXT_MAIL_SET_DEFAULT,
                                i,
                                GUIITEM_STYLE_ONE_LINE_RADIO,
                                type
                                );
            }
            else
            {
                MMIMAIL_AppendListItem(
                                account_info.addr,
                                MMIAPICOM_Wstrlen(account_info.addr),
                                ctrl_id,
                                STXT_OPTION,
                                TXT_MAIL_SET_DEFAULT,
                                i,
                                GUIITEM_STYLE_ONE_LINE_RADIO,
                                type
                                );
            }
        }

        def_acc_id = mail_GetDefaultAccount();

        if ((MAIL_INVALD_ACCOUNT_ID != def_acc_id) 
            && (def_acc_id < (account_num + MAIL_ACCOUNT_ID_START)))
        {
            cur_sel = def_acc_id - MAIL_ACCOUNT_ID_START;
            
            //set select item
            GUILIST_SetSelectedItem(ctrl_id,cur_sel,TRUE);

            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_sel);
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_LoadAccountList: def_acc_id = %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7178_112_2_18_2_48_39_511,(uint8*)"d", def_acc_id);
            cur_sel = 0;
            
            //set select item
            GUILIST_SetSelectedItem(ctrl_id,cur_sel,TRUE);

            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_sel);
            
            mail_SetDefAccount(cur_sel + MAIL_ACCOUNT_ID_START);
        }

        have_items = TRUE;
    }
    else
    {
        GUILIST_RemoveAllItems(ctrl_id);
        MMIMAIL_AppendEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);
    }

    return have_items;
}

/*****************************************************************************/
// 	Description : 系统设置菜单处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailSysSettingMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SYS_SETTING_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T   menu_id = 0;
    MMI_STRING_T    prompt_str = {0};
    MMI_WIN_ID_T    query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_ACCOUNT_SETTING_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_ACCOUNT_WIN_TAB, PNULL);
            break;

        case MMIMAIL_MENU_EDIT_OPTION_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_EDIT_OPTION_WIN_TAB, PNULL);
            break;

        case MMIMAIL_MENU_SILENT_TIME_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_SILENT_TIME_WIN_TAB, PNULL);
            break;
            
        case MMIMAIL_MENU_ATTACH_DL_OPTION_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_ACC_DL_OPT_WIN_TAB, PNULL);
            break;

        case MMIMAIL_MENU_DEL_ALL_MAILS_ITEM_ID:
            MMI_GetLabelTextByLang(TXT_MAIL_DEL_ALL_LOCAL_MAILS, &prompt_str);
            MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelAllQueryWinMsg );
            break;
            
        case MMIMAIL_MENU_RESET_FACTORY_SETTING_ITEM_ID:
            MMI_GetLabelTextByLang(TXT_COMMON_RESTORE_FACTORY_SETTINGS, &prompt_str);
            MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleMailResetQueryWinMsg );
            break;

        case MMIMAIL_MENU_LOGOUT_ITEM_ID:
            MMI_GetLabelTextByLang(TXT_MAIL_LOGOUT, &prompt_str);
            MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleMailLogoutQueryWinMsg );
            break;
            
        default:
            break;
        }
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (MAIL_ENTER_SETTINGWIN == MMIMAIL_GetOpenType())
        {
            MMIAPIMAIL_Exit();
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : 帮助界面处理函数
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailHelpWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T   ctrl_id = MMIMAIL_HELP_TEXT_CTRL_ID;
    MMI_STRING_T    content = {0};
    
	switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
        MMI_GetLabelTextByLang(TXT_MAIL_HELP_CONTENT, &content);
        GUITEXT_SetString(ctrl_id, content.wstr_ptr, content.wstr_len , FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
    	break;
        
	default:
    	result = MMI_RESULT_FALSE;
    	break;
    }
	return (result);
}

/*****************************************************************************/
//	Description : 提示帐号为空提示框处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlertAccEmptyWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        //退出手机邮箱
        MMIAPIMAIL_Exit();
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}

/*****************************************************************************/
//	Description : 删除帐号等待处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAccountWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIMAIL_HandleListChangeCallBack(); //删除完毕刷新list
        break;

    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//	Description : 邮箱帐号设置处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAccountWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_ACCOUNT_LISTBOX_CTRL_ID;
    uint16          cur_sel = 0;
    uint16          cur_item_index = 0;
    uint16          account_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    MMI_WIN_ID_T    query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        account_num = mail_GetAccountNum();
        account_num = MAX(account_num, 1);
        GUILIST_SetMaxItem(ctrl_id, account_num, FALSE);

        //加载帐号列表
        have_items = MMIMAIL_LoadAccountList(ctrl_id, MMIMAIL_APPEND_LIST_ITEM_TYPE);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

        s_mmimail_info.cur_sel_account_idx = GUILIST_GetCurItemIndex(ctrl_id);
        
        s_mmimail_info.is_account_num_change = FALSE;
        s_mmimail_info.is_account_name_change = FALSE;
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
        if (s_mmimail_info.is_account_name_change)
        {
            //更新帐号列表
            have_items = MMIMAIL_LoadAccountList(ctrl_id, MMIMAIL_REPLACE_LIST_ITEM_TYPE);
            if (have_items)
            {            
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
            GUILIST_SetCurItemIndex(ctrl_id, s_mmimail_info.cur_sel_account_idx);

        	s_mmimail_info.is_account_name_change = FALSE;
        }
        else if (s_mmimail_info.is_account_num_change)
        {
            //重新加载帐号列表
            GUILIST_RemoveAllItems(ctrl_id);
            
            account_num = mail_GetAccountNum();
            account_num = MAX(account_num, 1);
            GUILIST_SetMaxItem(ctrl_id, account_num, FALSE);

            //加载帐号列表
            have_items = MMIMAIL_LoadAccountList(ctrl_id, MMIMAIL_APPEND_LIST_ITEM_TYPE);

            if (have_items)
            {            
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
            s_mmimail_info.cur_sel_account_idx = GUILIST_GetCurItemIndex(ctrl_id);

        	s_mmimail_info.is_account_num_change = FALSE;
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (have_items)
        {
            s_mmimail_info.cur_sel_account_idx = GUILIST_GetCurItemIndex(ctrl_id);
            MMK_CreateWin((uint32 *)MMIMAIL_SET_ACCOUNT_OPT_WIN_TAB, PNULL);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (have_items)
        {
            //设为默认
            GUILIST_GetSelectedItemIndex(ctrl_id, &cur_sel, 1);
            cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            if (cur_sel != cur_item_index)
            {
                uint32 acc_id = cur_item_index + MAIL_ACCOUNT_ID_START;
                BOOLEAN is_need_password = FALSE;

                //COMM_TRACE:"MMIMAIL==> HandleSetAccountWinMsg: current select account id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7500_112_2_18_2_48_39_512,(uint8*)"d", acc_id);

                //检查切换的帐号是否需要输入密码
                is_need_password = mail_AccountNeedPassword(acc_id);
                if (is_need_password)
                {
                    MMK_CreateWin((uint32 *)MMIMAIL_INPUT_PASSWORD_WIN_TAB, (ADD_DATA)acc_id);
                }
                else
                {
                    GUILIST_SetSelectedItem(ctrl_id, cur_sel, FALSE);
                    GUILIST_SetSelectedItem(ctrl_id, cur_item_index, TRUE);
                    mail_SetDefAccount(cur_item_index + MAIL_ACCOUNT_ID_START);

                    MMK_PostMsg(ctrl_id, MSG_CTL_PAINT, 0, 0);
                    MMIMAIL_OpenMsgBox(TXT_SET_SUCCESS, IMAGE_PUBWIN_SUCCESS);//提示设置成功
                }
            }
            else
            {
                MMIMAIL_OpenMsgBox(TXT_SET_SUCCESS, IMAGE_PUBWIN_SUCCESS);//提示设置成功
            }
        }
        break;

    case MSG_MAIL_CHECK_PASSWD_RESULT:
        //COMM_TRACE:"MMIMAIL==> HandleSetAccountWinMsg: receive MSG_MAIL_CHECK_PASSWD_RESULT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7526_112_2_18_2_48_39_513,(uint8*)"");
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_sel, 1);
        cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_SetSelectedItem(ctrl_id, cur_sel, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, cur_item_index, TRUE);
        mail_SetDefAccount(cur_item_index + MAIL_ACCOUNT_ID_START);

        MMK_PostMsg(ctrl_id, MSG_CTL_PAINT, 0, 0);
        MMIMAIL_OpenMsgBox(TXT_SET_SUCCESS, IMAGE_PUBWIN_SUCCESS);//提示设置成功
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_MAIL_DEL_ACCOUNT_FINISH:
        //删除邮件结束后，删除当前帐号
        mail_DeleteAccountByIndex(s_mmimail_info.cur_sel_account_idx);
        s_mmimail_info.is_account_num_change = TRUE; //更新状态

        //关闭等待窗口
        if (MMK_IsOpenWin(MMIMAIL_DEL_WAITING_WIN_ID))
        {
            MMIPUB_CloseWaitWin(MMIMAIL_DEL_WAITING_WIN_ID);
        }

        //如果没有帐号则退出邮箱
        account_num = mail_GetAccountNum();
        if (0 == account_num)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAIL_ACCOUNT_IS_EMPTY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleAlertAccEmptyWinMsg);
        }
        else
        {
            //删除之后切换了默认帐号
            uint32 acc_id = mail_GetDefaultAccount();
            BOOLEAN is_need_password = mail_AccountNeedPassword(acc_id);

            if (is_need_password)
            {
                MMK_CreateWin((uint32 *)MMIMAIL_INPUT_PASSWORD_WIN_TAB, (ADD_DATA)acc_id);
            }
        }
        break;
        
    case MSG_PROMPTWIN_OK:
    {
        MMI_WIN_ID_T    wait_win_id = MMIMAIL_DEL_WAITING_WIN_ID;
        MMI_STRING_T    prompt_str = {0};
        MMIMAIL_ACCOUNT_INFO_T  account_info = {0};
        
        //关闭提示框
        MMIPUB_CloseQuerytWin(&query_win_id);
        
        //打开等待窗口,等待删除当前帐号及所有邮件
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
        
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,wait_win_id,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE, HandleDelAccountWaitWinMsg);

        
        //先删除当前帐号所有邮件
        mail_GetAccountByIndex(s_mmimail_info.cur_sel_account_idx, &account_info);
        mail_delAccountAllMail(account_info.accountid);
        
        break;
    }
    
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    case MSG_CLOSE_WINDOW:
    {
        MMIMAIL_ACCOUNT_INFO_T cur_info = {0};
        
        //获取当前默认帐号信息
        MMIMAIL_GetDefAccount(&cur_info);
        if (cur_info.accountid != s_mmimail_info.def_account.accountid)
        {
            //默认帐号改变
            s_mmimail_info.def_account = cur_info;

            //记录默认帐号改变状态
            s_mmimail_info.is_def_account_change = TRUE;
        }
        break;
    }
    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 设置帐号选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAccountOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_SET_ACCOUNT_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_WIN_ID_T query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;
    MMI_STRING_T prompt_str = {0};

    //COMM_TRACE:"MMIMAIL==> HandleSetAccountOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7640_112_2_18_2_48_40_514,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_OPT_EDIT_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_ACCOUNT_EDIT_WIN_TAB, PNULL);
            break;
                    
        case MMIMAIL_MENU_DEL_ITEM_ID:
            MMI_GetLabelTextByLang(TXT_MAIL_DEL_ACC_ALERT, &prompt_str);
            MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL );
            break;
            
        case MMIMAIL_MENU_SET_DEF_ACCOUNT_ITEM_ID:
            MMK_PostMsg(MMIMAIL_SET_ACCOUNT_WIN_ID, MSG_APP_WEB, 0, 0);
            break;
            
        case MMIMAIL_MENU_SYNC_ACC_SET_ITEM_ID:
            
            break;

        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//	Description : 加载帐号名称
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddAccountName(
                                MMI_CTRL_ID_T       ctrl_id,
                                uint16              *index_ptr,
                                BOOLEAN             is_add, //TRUE: 增加节点，FALSE:修改节点
                                MMIMAIL_ACCOUNT_INFO_T   *account_info
                                )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              len = 0;
    MMI_STRING_T        tmp_str = {0};

    if ((PNULL == account_info) || (PNULL == index_ptr))
    {
        return;
    }
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    if (is_add)
    {
        //label
        MMI_GetLabelTextByLang(TXT_MAIL_ACCOUNT_NAME, &tmp_str);
        item_data.text_data.buf.str_ptr = tmp_str.wstr_ptr;
        item_data.text_data.buf.len    = tmp_str.wstr_len;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    
    //edit
    len = MMIAPICOM_Wstrlen(account_info->name);
    if (len > 0)
    {
        item_data.text_data.buf.str_ptr = account_info->name;
        item_data.text_data.buf.len    = len;
    }
    else
    {
        //帐号名称为空时，使用帐号地址作为默认的帐号名称
        item_data.text_data.buf.str_ptr = account_info->addr;
        item_data.text_data.buf.len    = MMIAPICOM_Wstrlen(account_info->addr);
    }
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    if (is_add)
    {
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    else
    {
        GUIRICHTEXT_ModifyItem(ctrl_id, *index_ptr, &item_data);
    }
}                                

/*****************************************************************************/
//	Description : 加载帐号地址
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddAccountAddr(
                                MMI_CTRL_ID_T       ctrl_id,
                                uint16              *index_ptr,
                                BOOLEAN             is_add, //TRUE: 增加节点，FALSE:修改节点
                                MMIMAIL_ACCOUNT_INFO_T   *account_info
                                )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    MMI_STRING_T        tmp_str = {0};

    if ((PNULL == account_info) || (PNULL == index_ptr))
    {
        return;
    }
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    if (is_add)
    {
        //label
        MMI_GetLabelTextByLang(TXT_MAIL_ACCOUNT_ADDR, &tmp_str);
        item_data.text_data.buf.str_ptr = tmp_str.wstr_ptr;
        item_data.text_data.buf.len    = tmp_str.wstr_len;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    
    //edit
    item_data.text_data.buf.str_ptr = account_info->addr;
    item_data.text_data.buf.len    = MMIAPICOM_Wstrlen(account_info->addr);
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    if (is_add)
    {
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    else
    {
        GUIRICHTEXT_ModifyItem(ctrl_id, *index_ptr, &item_data);
    }
}     

/*****************************************************************************/
//	Description : 加载用户名称,即地址别名
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddAccountUserName(
                                MMI_CTRL_ID_T       ctrl_id,
                                uint16              *index_ptr,
                                BOOLEAN             is_add, //TRUE: 增加节点，FALSE:修改节点
                                MMIMAIL_ACCOUNT_INFO_T   *account_info
                                )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    MMI_STRING_T        tmp_str = {0};
    uint16              len = 0;

    if ((PNULL == account_info) || (PNULL == index_ptr))
    {
        return;
    }
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    if (is_add)
    {
        //label
        MMI_GetLabelTextByLang(TXT_COMMON_USERNAME, &tmp_str);
        item_data.text_data.buf.str_ptr = tmp_str.wstr_ptr;
        item_data.text_data.buf.len    = tmp_str.wstr_len;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    
    //edit
    len = MMIAPICOM_Wstrlen(account_info->addr_name);
    if (len > 0)
    {
        item_data.text_data.buf.str_ptr = account_info->addr_name;
        item_data.text_data.buf.len    = len;
    }
    else
    {
        //用户别名为空时，使用帐号地址作为默认的帐号别名
        item_data.text_data.buf.str_ptr = account_info->addr;
        item_data.text_data.buf.len    = MMIAPICOM_Wstrlen(account_info->addr);
    }
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    if (is_add)
    {
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    else
    {
        GUIRICHTEXT_ModifyItem(ctrl_id, *index_ptr, &item_data);
    }
}                                

/*****************************************************************************/
//	Description : 加载帐号密码
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddAccountPassword(
                                MMI_CTRL_ID_T       ctrl_id,
                                uint16              *index_ptr,
                                BOOLEAN             is_add, //TRUE: 增加节点，FALSE:修改节点
                                MMIMAIL_ACCOUNT_INFO_T   *account_info
                                )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    MMI_STRING_T        tmp_str   = {0};
    wchar               *password = L"****************";
    
    if ((PNULL == account_info) || (PNULL == index_ptr))
    {
        return;
    }
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    if (is_add)
    {
        //label
        MMI_GetLabelTextByLang(TXT_COMMON_PASSWORD, &tmp_str);
        item_data.text_data.buf.str_ptr = tmp_str.wstr_ptr;
        item_data.text_data.buf.len    = tmp_str.wstr_len;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    
    //edit
    item_data.text_data.buf.str_ptr = password; //account_info->password;
    item_data.text_data.buf.len    = MMIAPICOM_Wstrlen(account_info->password);
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    if (is_add)
    {
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    else
    {
        GUIRICHTEXT_ModifyItem(ctrl_id, *index_ptr, &item_data);
    }
}                                

/*****************************************************************************/
//	Description : 加载帐号编辑项目
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_LoadAccountEditItem(
                                            MMI_CTRL_ID_T       ctrl_id,
                                            BOOLEAN             is_add //TRUE: 增加节点，FALSE:修改节点
                                            )
                                           
{
    uint16              index = 0;
    
    if (is_add)
    {
        mail_GetAccountByIndex(s_mmimail_info.cur_sel_account_idx, &s_mmimail_info.cur_edit_account);

        AddAccountName(ctrl_id, &index, is_add, &(s_mmimail_info.cur_edit_account));
        AddAccountAddr(ctrl_id, &index, is_add, &(s_mmimail_info.cur_edit_account));
        AddAccountUserName(ctrl_id, &index, is_add, &(s_mmimail_info.cur_edit_account));
        AddAccountPassword(ctrl_id, &index, is_add, &(s_mmimail_info.cur_edit_account));
    }
    else
    {
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &index);
        //只能编辑帐号名称和密码
        switch (index)
        {
        case MMIMAIL_ACCOUNT_NAME_EDIT:
            AddAccountName(ctrl_id, &index, is_add, &(s_mmimail_info.cur_edit_account));
            break;

        case MMIMAIL_ACCOUNT_PASSWORD_EDIT:
            AddAccountPassword(ctrl_id, &index, is_add, &(s_mmimail_info.cur_edit_account));
            break;

        default:
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_LoadAccountEditItem: focus item is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_7940_112_2_18_2_48_40_515,(uint8*)"d", index);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : 帐号编辑处理函数
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAccountEditWinMsg(
                                                 MMI_WIN_ID_T       win_id,  
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                               )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_RICHTEXT_CTRL_ID;
    uint16          focus_index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMAIL_LoadAccountEditItem(ctrl_id, TRUE);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED);
        GUIRICHTEXT_SetBorderColor(ctrl_id, MMI_GRAY_WHITE_COLOR);
        GUIRICHTEXT_SetFocusBgColor(ctrl_id, MMI_DARK_GRAY_COLOR);
        GUIRICHTEXT_SetFocusItem(ctrl_id, MMIMAIL_ACCOUNT_NAME_EDIT);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_EDIT, STXT_RETURN, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id); 
        break;

    case MSG_GET_FOCUS:
        MMIMAIL_LoadAccountEditItem(ctrl_id, FALSE);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        //保存编辑内容
        mail_SetAccountInfo(&(s_mmimail_info.cur_edit_account));
        MMK_CloseWin(win_id);
        break;

    case MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_KEY:
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &focus_index);
        if ((MMIMAIL_ACCOUNT_NAME_EDIT == focus_index)
            || ((MMIMAIL_ACCOUNT_PASSWORD_EDIT == focus_index)
            && (MMIMAIL_IsPermitModifyPassword())))
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_EDIT, STXT_RETURN, TRUE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_NULL, STXT_RETURN, TRUE);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &focus_index);
        switch (focus_index)
        {
        case MMIMAIL_ACCOUNT_NAME_EDIT:
            MMK_CreateWin((uint32 *)MMIMAIL_EDIT_ACCOUNT_NAME_TAB,PNULL);
            break;
            
        case MMIMAIL_ACCOUNT_PASSWORD_EDIT:
            if (MMIMAIL_IsPermitModifyPassword())
            {
                MMK_CreateWin((uint32 *)MMIMAIL_EDIT_ACCOUNT_PW_TAB,PNULL);
            }
            break;

        default:
            break;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
       
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }

    
    return err_code;
}

/*****************************************************************************/
//	Description : 帐号名称编辑界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditAccountNameWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_TEXT_CTRL_ID;
    MMI_STRING_T    tmp_str = {0};
    uint16          len = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        len = MMIAPICOM_Wstrlen(s_mmimail_info.cur_edit_account.name);
        if (len > 0)
        {
            GUIEDIT_SetString(ctrl_id, s_mmimail_info.cur_edit_account.name, len);
        }
        else
        {
            len = MMIAPICOM_Wstrlen(s_mmimail_info.cur_edit_account.addr);
            GUIEDIT_SetString(ctrl_id, s_mmimail_info.cur_edit_account.addr, len);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIEDIT_GetString(ctrl_id, &tmp_str);
        len = sizeof(s_mmimail_info.cur_edit_account.name);
        SCI_MEMSET(s_mmimail_info.cur_edit_account.name, 0, len);
        len = MIN(len, tmp_str.wstr_len);
        MMIAPICOM_Wstrncpy(s_mmimail_info.cur_edit_account.name, (const wchar *)tmp_str.wstr_ptr, len);

        s_mmimail_info.is_account_name_change = TRUE;

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//	Description : 帐号密码编辑界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditAccountPwWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_TEXT_CTRL_ID;
    MMI_STRING_T    tmp_str = {0};
    uint16          len = 0;
	BOOLEAN         is_disp_im_icon = TRUE;
	BOOLEAN         is_disp_num_info = TRUE;
	MMI_STRING_T    waiting_text = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    	GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);
        GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
        GUIEDIT_SetPasswordStyle(ctrl_id, GUIEDIT_PASSWORD_STYLE_ALL);
        len = MMIAPICOM_Wstrlen(s_mmimail_info.cur_edit_account.password);
        GUIEDIT_SetString(ctrl_id, s_mmimail_info.cur_edit_account.password, len);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIEDIT_GetString(ctrl_id, &tmp_str);
        
        //发送验证密码请求
        mail_CheckPassWord(s_mmimail_info.cur_edit_account.accountid, tmp_str.wstr_ptr, tmp_str.wstr_len);
        
        //打开等待窗口
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &waiting_text);
        
        MMIPUB_OpenWaitWin(1, &waiting_text, PNULL, PNULL, MMIMAIL_CHECK_PW_WAITING_WIN_ID, IMAGE_NULL,
            ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleInitWaitingWinMsg);
        
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_MAIL_CHECK_PASSWD_RESULT:
    {
        BOOLEAN is_success = *((BOOLEAN *)param);
        
        //COMM_TRACE:"MMIMAIL==> HandleMailEditAccountPwWinMsg: receive MSG_MAIL_CHECK_PASSWD_RESULT, is_success = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_8149_112_2_18_2_48_41_516,(uint8*)"d", is_success);
        
        //密码同步成功则更新，否则保持不变
        if (is_success)
        {
            GUIEDIT_GetString(ctrl_id, &tmp_str);
            //COMM_TRACE:"MMIMAIL==> HandleMailEditAccountPwWinMsg: tmp_str.wstr_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_8155_112_2_18_2_48_41_517,(uint8*)"d", tmp_str.wstr_len);
        len = sizeof(s_mmimail_info.cur_edit_account.password);
        SCI_MEMSET(s_mmimail_info.cur_edit_account.password, 0, len);
        len = MIN(len, tmp_str.wstr_len);
        MMIAPICOM_Wstrncpy(s_mmimail_info.cur_edit_account.password, (const wchar *)tmp_str.wstr_ptr, len);
        }
        
        MMK_CloseWin(win_id);
        break;
    }
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//	Description : 输入密码界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailInputPwWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_TEXT_CTRL_ID;
    MMI_STRING_T    tmp_str = {0};
    MMI_STRING_T    waiting_text = {0};
    //uint16          len = 0;
	BOOLEAN         is_disp_im_icon = TRUE;
	BOOLEAN         is_disp_num_info = TRUE;
	uint32          add_data = 0;
	LOCAL uint32   acc_id = MAIL_INVALD_ACCOUNT_ID; //要输入密码的帐号id

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    	GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);
        GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
        GUIEDIT_SetPasswordStyle(ctrl_id, GUIEDIT_PASSWORD_STYLE_ALL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        add_data = (uint32)MMK_GetWinAddDataPtr(win_id);
        if (add_data >= MAIL_ACCOUNT_ID_START)
        {
            acc_id = add_data;
        }
        else
        {
            acc_id = MMIMAIL_GetDefAccountId();
        }
        //COMM_TRACE:"MMIMAIL==> HandleMailInputPwWinMsg: add_data = %d, acc_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_8210_112_2_18_2_48_41_518,(uint8*)"dd", add_data, acc_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIEDIT_GetString(ctrl_id, &tmp_str);
        
        //发送验证密码请求
        mail_CheckPassWord(acc_id, tmp_str.wstr_ptr, tmp_str.wstr_len);
        
        //打开等待窗口
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &waiting_text);
        
        MMIPUB_OpenWaitWin(1, &waiting_text, PNULL, PNULL, MMIMAIL_CHECK_PW_WAITING_WIN_ID, IMAGE_NULL,
            ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleInitWaitingWinMsg);
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        
        if (MMK_IsOpenWin(MMIMAIL_SET_ACCOUNT_WIN_ID))
        {
            uint32 acc_num = mail_GetAccountNum();
            
            if (1 == acc_num)
            {
                //当前只有一个帐号，但是不输入密码的话直接退出邮箱
                MMIAPIMAIL_Exit();
            }
        }
        
        break;
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//	Description : 编辑选项设置处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetEditOptionWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T       menu_id = 0;
   
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_EDIT_OPT_INC_ORG_CONT_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_INC_ORG_WIN_TAB, PNULL);
            break;

        case MMIMAIL_MENU_EDIT_OPT_SIGN_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_SIGN_WIN_TAB, PNULL);
            break;

        default:
            break;
        }
        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 设置转发/回复是否包含原邮件正文处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetIncOrgWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_LISTBOX_CTRL_ID;
    uint16          cur_selection = 0;
    BOOLEAN         is_open = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIMAIL_SET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextId( TXT_YES, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_COMM_NO, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

        is_open = mail_GetReplyWithOriginalCont();
        if (is_open)
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
        if (0 == cur_selection)
        {
            is_open = TRUE;
        }
        else
        {
            is_open = FALSE;
        }
        mail_SetReplyWithOriginalCont(is_open);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 设置是否添加签名处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetSignWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_LISTBOX_CTRL_ID;
    uint16          cur_selection = 0;
    BOOLEAN         is_open = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIMAIL_SET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

        is_open = mail_GetAddSignature();
        if (is_open)
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
        if (0 == cur_selection)
        {
            is_open = TRUE;
            MMK_CreateWin((uint32 *)MMIMAIL_EDIT_SIGN_TAB, PNULL);
        }
        else
        {
            is_open = FALSE;
        }
        mail_SetAddSignature(is_open);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 编辑签名界面处理函数
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditSignWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_TEXT_CTRL_ID;
    wchar           *sign_text = PNULL;
    MMI_STRING_T    tmp_str = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        sign_text = mail_GetSignature_text();
        if (PNULL != sign_text)
        {
            GUIEDIT_SetString(ctrl_id, sign_text, MMIAPICOM_Wstrlen(sign_text));
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIEDIT_GetString(ctrl_id, &tmp_str);
        mail_SetSignature_text(tmp_str.wstr_ptr);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//	Description : 设置静音时段
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddSilentTime(
                            MMI_CTRL_ID_T                   ctrl_id,
                            uint16                          *index_ptr,
                            SCI_TIME_T                      *time,  //开始时间
                            MMIMAIL_EDIT_SILENT_TIME_TYPE_E type,//时间类型            
                            BOOLEAN                         is_add  //TRUE: 增加节点，FALSE:修改节点
                            )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    //uint16              len = 0;
    MMI_STRING_T        tmp_str = {0};
    char                time_str[MMIMAIL_TIME_STR_LEN + 1] = {0};
    wchar               time_wstr[MMIMAIL_TIME_STR_LEN + 1] = {0};

    if(PNULL == index_ptr ||PNULL == time)
    {
        //SCI_TRACE_LOW:"AddSilentTime index_ptr || time is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_8528_112_2_18_2_48_41_519,(uint8*)"");
        return;
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    if (is_add)
    {
        //label
        if (MMIMAIL_EDIT_SILENT_START_TIME == type)
        {
            MMI_GetLabelTextByLang(TXT_START_TIME, &tmp_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_MAIL_END_TIME, &tmp_str);
        }
        item_data.text_data.buf.str_ptr = tmp_str.wstr_ptr;
        item_data.text_data.buf.len    = tmp_str.wstr_len;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    
    //edit
    sprintf(time_str, "%02d:%02d", time->hour, time->min);
    MMIAPICOM_StrToWstr((const uint8 *)time_str, time_wstr);
    item_data.text_data.buf.str_ptr = time_wstr;
    item_data.text_data.buf.len    = MMIAPICOM_Wstrlen(time_wstr);
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    if (is_add)
    {
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, index_ptr);
    }
    else
    {
        GUIRICHTEXT_ModifyItem(ctrl_id, *index_ptr, &item_data);
    }
}                                

/*****************************************************************************/
//	Description : 设置静音时段
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void SetSilentTime(
                            MMI_CTRL_ID_T       ctrl_id,
                            BOOLEAN             is_add //TRUE: 增加节点，FALSE:修改节点
                            )
                                           
{
    uint16              index = 0;
    SCI_TIME_T          *start_time = &(s_mmimail_info.cur_edit_silent_start_time);
    SCI_TIME_T          *end_time = &(s_mmimail_info.cur_edit_silent_end_time);
    
    if (is_add)
    {
        mail_GetSilenceDuration(start_time, end_time);
        
        AddSilentTime(ctrl_id, &index, start_time, MMIMAIL_EDIT_SILENT_START_TIME, is_add);
        AddSilentTime(ctrl_id, &index, end_time, MMIMAIL_EDIT_SILENT_END_TIME, is_add);
    }
    else
    {
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &index);
        switch (index)
        {
        case MMIMAIL_EDIT_SILENT_START_TIME_EDIT:
            AddSilentTime(ctrl_id, &index, start_time, MMIMAIL_EDIT_SILENT_START_TIME, is_add);
            break;

        case MMIMAIL_EDIT_SILENT_END_TIME_EDIT:
            AddSilentTime(ctrl_id, &index, end_time, MMIMAIL_EDIT_SILENT_END_TIME, is_add);
            break;

        default:
            //COMM_TRACE:"MMIMAIL==> SetSilentTime: focus item is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_8605_112_2_18_2_48_41_520,(uint8*)"d", index);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : 显示当前静音时间
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void DisplaySilentTime(
                                 MMI_CTRL_ID_T     ctrl_id
                                 )
{
    SCI_TIME_T	                *cur_time = PNULL;
	GUIEDIT_TIME_STYLE_E		time_style = GUIEDIT_TIME_STYLE_NULL;
	MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    //获取时间类型
    time_type = MMIAPISET_GetTimeDisplayType();
	if (MMISET_TIME_12HOURS == time_type)
    {
    	time_style = GUIEDIT_TIME_STYLE_12;
    }
	else
    {
    	time_style = GUIEDIT_TIME_STYLE_24;
    }
    
    //设置时间显示风格
	GUIEDIT_SetTimeStyle(ctrl_id,PNULL,&time_style,PNULL,FALSE);
    GUIEDIT_SetFont(ctrl_id, MMI_SET_TIME_FONT);

    if (MMIMAIL_EDIT_SILENT_START_TIME == s_mmimail_info.cur_edit_time_type)
    {
        cur_time = &(s_mmimail_info.cur_edit_silent_start_time);
    }
    else
    {
        cur_time = &(s_mmimail_info.cur_edit_silent_end_time);
    }
    
	GUIEDIT_SetTime(ctrl_id, cur_time->hour, cur_time->min, 0);
}

/*****************************************************************************/
//  Description : 设置静音时间窗口的背景
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void SetSilentTimeBackground(
                                 uint32         icon_id,
                                 MMI_WIN_ID_T   win_id
                                 )
{
	GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T          bg_rect         =   {0};
	GUI_POINT_T         dis_point       =   {0};

    bg_rect.left = 0;
    bg_rect.top = MMI_TITLE_HEIGHT;
    bg_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    bg_rect.bottom = MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;

	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;

	IMG_EnableTransparentColor(TRUE); 
	GUIRES_DisplayImg(PNULL,
        &bg_rect,
        &bg_rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);

    dis_point.x = MMI_TIME_IMAGE_LEFT;
    dis_point.y = MMI_TIME_IMAGE_TOP;
    GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            icon_id,
            &lcd_dev_info);
    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : 保存输入的静音时间
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSilentTime(
                      MMI_CTRL_ID_T     ctrl_id
                      )
{
    uint8           hour        =   0;
    uint8           minute      =   0;
    MMISET_TIME_T   time        =   {0};
    SCI_TIME_T      cur_time    =   {0};
    BOOLEAN         result      =   TRUE;
    
    //获取编辑的时间
    GUIEDIT_GetTime(ctrl_id,&hour,&minute,PNULL);
    
    time.time_hour = hour;
    time.time_minute = minute;
    
    //检查时间是否有效
    if (MMIAPICOM_CheckTimeIsValid(time))
    {
        cur_time.sec = 0;
        cur_time.min = minute;
        cur_time.hour = hour;

        if (MMIMAIL_EDIT_SILENT_START_TIME == s_mmimail_info.cur_edit_time_type)
        {
            s_mmimail_info.cur_edit_silent_start_time = cur_time;
        }
        else
        {
            s_mmimail_info.cur_edit_silent_end_time = cur_time;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"SaveSilentTime:hour is %d,minute is %d is invalid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_8728_112_2_18_2_48_42_521,(uint8*)"dd",hour,minute);
        result = FALSE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 设置静音时段处理函数
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailSilentTimeWinMsg(
                                                 MMI_WIN_ID_T       win_id,  
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_RICHTEXT_CTRL_ID;
    uint16          focus_index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetSilentTime(ctrl_id, TRUE);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED);
        GUIRICHTEXT_SetBorderColor(ctrl_id, MMI_GRAY_WHITE_COLOR);
        GUIRICHTEXT_SetFocusBgColor(ctrl_id, MMI_DARK_GRAY_COLOR);
        GUIRICHTEXT_SetFocusItem(ctrl_id, MMIMAIL_EDIT_SILENT_START_TIME_EDIT);
        MMK_SetAtvCtrl(win_id, ctrl_id); 
        break;

    case MSG_GET_FOCUS:
        SetSilentTime(ctrl_id, FALSE);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        //保存编辑内容
        mail_SetSilenceDuration(&(s_mmimail_info.cur_edit_silent_start_time),
                                &(s_mmimail_info.cur_edit_silent_end_time));
        MMK_CloseWin(win_id);
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &focus_index);
        switch (focus_index)
        {
        case MMIMAIL_EDIT_SILENT_START_TIME_EDIT:
            s_mmimail_info.cur_edit_time_type = MMIMAIL_EDIT_SILENT_START_TIME;
            MMK_CreateWin((uint32 *)MMIMAIL_EDIT_SILENT_TIME_WIN_TAB,PNULL);
            break;
            
        case MMIMAIL_EDIT_SILENT_END_TIME_EDIT:
            s_mmimail_info.cur_edit_time_type = MMIMAIL_EDIT_SILENT_END_TIME;
            MMK_CreateWin((uint32 *)MMIMAIL_EDIT_SILENT_TIME_WIN_TAB,PNULL);
            break;

        default:
            break;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
       
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }

    
    return err_code;
}

/*****************************************************************************/
//  Description : 设置静音时段时间编辑界面
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditSilentTimeWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E	    	recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIMAIL_SET_TIMEEDIT_CTRL_ID;
    //SCI_TIME_T	        	time    = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        DisplaySilentTime(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        SetSilentTimeBackground(IMAGE_SET_TIME_ICON, win_id);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (SaveSilentTime(ctrl_id))//时间有效
        {
            MMK_CloseWin(win_id);
        }
        else
        {      
            MMIMAIL_OpenMsgBox(TXT_INVALID_TIME, IMAGE_PUBWIN_WARNING);
        }
        break;
                
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);      
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//	Description : 附件下载选项设置处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccDlOptWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T       menu_id = 0;
   
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_ACC_AUTO_DL_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_ACC_AUTO_DL_WIN_TAB, PNULL);
            break;

        case MMIMAIL_MENU_ACC_AUTO_CHANGE_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_ACC_AUTO_CHANGE_WIN_TAB, PNULL);
            break;
            
        case MMIMAIL_MENU_ACC_SIZE_LIMIT_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_SET_ACC_SIZE_LIMIT_WIN_TAB, PNULL);
            break;

        default:
            break;
        }
        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 设置附件自动下载处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccAutoDlWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_LISTBOX_CTRL_ID;
    uint16          cur_selection = 0;
    BOOLEAN         is_open = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIMAIL_SET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

        if (mail_GetAttachAutoDownload())
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
        if (0 == cur_selection)
        {
            is_open = TRUE;
        }
        else
        {
            is_open = FALSE;
        }
        mail_SetAttachAutoDownload(is_open);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 设置附件自动格式转换处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccAutoChangeWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_LISTBOX_CTRL_ID;
    uint16          cur_selection = 0;
    //BOOLEAN         is_open = FALSE;
    MAIL_CHANGE_FORMAT_E    format = MAIL_NO_CHANGE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIMAIL_SET_AUTO_CHANGE_ITEM, FALSE );//max item 3
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_NO_CHANGE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_CHANGE_TO_PNG, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_CHANGE_TO_JPG, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

        //获取当前设置项
        format = mail_GetAttachDownMode();

        cur_selection = (uint16)format;
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);

        format = (MAIL_CHANGE_FORMAT_E)cur_selection;
        
        //设置当前设置项
        mail_SetAttachDownMode(format);
        
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 设置附件大小限制处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailSetAccSizeLimitWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_SET_LISTBOX_CTRL_ID;
    uint16          cur_selection = 0;
    //BOOLEAN         is_open = FALSE;
    int             size = 0;
    int             num = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIMAIL_SET_SIZE_LIMIT_ITEM, FALSE );
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_SIZE_100K, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_SIZE_200K, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_SIZE_300K, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_SIZE_400K, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_MAIL_SIZE_500K, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

        size = mail_GetAttachDownloadLimit();
        num = size / MMIMAIL_SIZE_100K_BYTE;

        if (num > 0)
        {
            cur_selection = num - 1;
        }
        else
        {
            cur_selection = 0;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
        num = cur_selection + 1;
        size = num * MMIMAIL_SIZE_100K_BYTE;
        mail_SetAttachDownloadLimit(size);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 删除所有邮件等待处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MSG_CLOSE_WINDOW:
        MMIMAIL_HandleListChangeCallBack(); //删除完毕刷新list
        break;

    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //mail_CancleDelMail(); //通知mail线程取消删除操作
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//	Description : 删除所有邮件询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    //MMI_MENU_GROUP_ID_T group_id = 0;
    //MMI_MENU_ID_T       menu_id = 0;
    MMI_STRING_T        prompt_str = {0};
    MMI_WIN_ID_T        wait_win_id = MMIMAIL_DEL_WAITING_WIN_ID;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:      
        //U盘使用过程中不能删除邮件
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
        	break;
        }

        //打开等待窗口
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
        
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,wait_win_id,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleDelAllWaitWinMsg);

        //通知mail线程删除所有帐号的所有邮件
        mail_delAllAccountAllMail();
        
        //关闭询问窗口
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        //关闭询问窗口
        MMK_CloseWin(win_id);
        
        if (s_mmimail_info.is_logout_oper)
        {
            uint16 account_num = mail_GetAccountNum();
            uint16 i = 0;
            
            s_mmimail_info.is_logout_oper = FALSE; //恢复注销发起的删除标记位
            
            //只删除帐号
            for (i = 0; i < account_num; i++)
            {
                //每次执行完之后,mail_settings.account_num都会减1,所以需要循环删除最前面的账户
                //否则不是所有账户都能删除
                mail_DeleteAccountByIndex(0);//i->0 changed by liangshun 20110422
            }

            //退出手机邮箱
            MMIAPIMAIL_Exit();
        }

        if (MSG_APP_RED == msg_id)
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
                
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//	Description : 恢复出厂设置，删除本地帐号及所有邮件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_ResetFTSetting(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T prompt_str = {0};
    
    //U盘使用过程中不能删除邮件
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
    	return;
    }

    //打开等待窗口
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleResetWaitWinMsg);

    //通知mail线程删除整个mail目录
    mail_resetFactory();
}

/*****************************************************************************/
//	Description : 恢复出厂设置等待处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        break;

    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//	Description : 恢复出厂设置询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailResetQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:    
        MMIMAIL_ResetFTSetting(MMIMAIL_RESET_WAITING_WIN_ID);
        
        //关闭询问窗口
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
// 	Description : 提示注销成功界面
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailLogoutSuccessWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    prompt_str = {0};
    MMI_WIN_ID_T    query_win_id = MMIMAIL_DEL_QUERY_WIN_ID;
    
	switch (msg_id) 
    {
    case MSG_CLOSE_WINDOW:
        //询问是否删除本地邮件
        MMI_GetLabelTextByLang(TXT_MAIL_DEL_ALL_LOCAL_MAILS, &prompt_str);
        MMIPUB_OpenQueryWinByTextPtr(&prompt_str, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelAllQueryWinMsg );

        s_mmimail_info.is_logout_oper = TRUE; //记录注销发起的删除标记
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    
	default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
    	break;
    }
	return (result);
}

/*****************************************************************************/
//	Description : 注销139邮箱询问窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailLogoutQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM              param      //IN:
                                            )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:    
        if (MMIMAIL_logOutAccount()) //发送注销短信
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,
                                TXT_MAIL_LOGOUT_ALERT_SUCCESS,
                                TXT_NULL,IMAGE_PUBWIN_SUCCESS,
                                PNULL,
                                PNULL,
                                MMIPUB_SOFTKEY_ONE,
                                HandleMailLogoutSuccessWinMsg);
        }
        else
        {
            MMIMAIL_OpenMsgBox(TXT_MAIL_LOGOUT_ALERT_FAIL, IMAGE_PUBWIN_WARNING);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

//following is mail notify function

#define MMIMAIL_RING_LEN             5000
#define MMIMAIL_VIBRA_TIMEROUT       2000


LOCAL uint8    s_play_mail_vibrator_timer_id = 0;  //play mail vibrator timer id
LOCAL uint8    g_mail_ring_timer_id              = 0;//mail 铃声的截取





LOCAL MMI_RESULT_E  MMIMAIL_HandleNewMsgWin(
                                           MMI_WIN_ID_T      win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        //  左软键是进入，右软键是退出
#ifdef MMI_KEY_LOCK_SUPPORT
        if (TRUE == MMIAPIKL_IsPhoneLocked() 
#ifdef BROWSER_SUPPORT_DORADO
            || TRUE == MMIAPIBROWSER_IsRunning()
#endif
#ifdef VT_SUPPORT
            ||TRUE == MMIAPIVT_IsVtConnecting()
#endif
            )
        {
            //SCI_TRACE_LOW:"Mail:MMIMAIL_HandleNewMsgWin do not allow in mail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9454_112_2_18_2_48_44_522,(uint8*)"");
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
#endif
        {
            MMIPUB_SetWinSoftkey(win_id, STXT_SELECT, STXT_EXIT, FALSE);
        }
        err_code = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //todo 进入收件箱
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {         
            //open mail inbox
            //MMIMAIL_EnterMailBox(MMIMAIL_INBOX);
            MMIAPIMAIL_Enter(MAIL_ENTER_INBOX, PNULL);
        }
        MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CLOSE_WINDOW:
        //todo 停止响铃
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
        err_code = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default :
        err_code = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }    
    
    return err_code;
}


/*==========================================================
 * Function     : MailRingPlayCallBack
 * Author	    : sunguochen      
 * Description : 邮件响铃回调
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/10/2010 
 ==========================================================*/
LOCAL void MailRingPlayCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9505_112_2_18_2_48_44_523,(uint8*)"d",result);
    
    if (MMISRVAUD_REPORT_RESULT_NOT_SUPPORT == result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
    }
    else if ((MMISRVAUD_REPORT_RESULT_ERROR == result))
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_PLAY_ERROR);
    }
    
    if (0 != s_play_mail_vibrator_timer_id)
    {
        MMK_StopTimer(s_play_mail_vibrator_timer_id);
        s_play_mail_vibrator_timer_id = 0;
    }
    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_MSG);    
}





/*==========================================================
 * Function     : MMIAPIMail_HandleTimerForRing
 * Author	    : sunguochen      
 * Description : 播放mail铃声定时时间到处理函数
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/10/2010 
 ==========================================================*/
PUBLIC void MMIAPIMail_HandleTimerForRing(uint8  timer_id, uint32 param)
{
    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_MSG);
    MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
    if (0 != g_mail_ring_timer_id)
    {
        MMK_StopTimer(g_mail_ring_timer_id);
        g_mail_ring_timer_id = 0;
    }
    if (0 != s_play_mail_vibrator_timer_id)
    {
        MMK_StopTimer(s_play_mail_vibrator_timer_id);
        s_play_mail_vibrator_timer_id = 0;
    }
}



/*==========================================================
 * Function     : MMIMAIL_PlayMailSound
 * Author	    : sunguochen      
 * Description : 播放邮件到达通知消息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/10/2010 
 ==========================================================*/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
PUBLIC void MMIMAIL_PlayMailSound(void)
{
    MMISET_MSG_RING_TYPE_E      ring_type = MMISET_MSG_RING;
    MN_DUAL_SYS_E             dual_sys  = MN_DUAL_SYS_1;

    MMIDEFAULT_SetBackLight(TRUE);

    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_MSG);
    MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);

    if (0 != s_play_mail_vibrator_timer_id)
    {
        MMK_StopTimer(s_play_mail_vibrator_timer_id);
        s_play_mail_vibrator_timer_id = 0;
    }

    if(mail_IsSilenceDuration() == TRUE)
    {
        //SCI_TRACE_LOW:"MAIL: MMIMAIL_PlayMailSound now silence mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9583_112_2_18_2_48_44_524,(uint8*)"");
        return;
    }
    
    //有新的需要播放铃声，把原来的播放铃声先停掉。
    ring_type = (MMISET_MSG_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(dual_sys, MSG_RING_TYPE);
    
    if (!MMIDEFAULT_IsPermitPlayNewMsgRing())
    {
        if(MMISET_MSG_RING == ring_type || MMISET_MSG_VIBRA_AND_RING == ring_type)
        {
            ring_type = MMISET_MSG_VIBRA;
        }
    }

    if (MMIAPICC_IsVideoCall())
    {
        //SCI_TRACE_LOW:"MAIL: PlaymailRing vt mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9600_112_2_18_2_48_44_525,(uint8*)"");
        MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, MMIMAIL_VIBRA_TIMEROUT, 0, 1, PNULL);
    }    
    else if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) && !MMIAPIUDISK_UdiskIsRun())
    {
        //SCI_TRACE_LOW:"MAIL: PlaymailRing normal call mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9605_112_2_18_2_48_44_526,(uint8*)"");
        MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG_IN_CALL,MailRingPlayCallBack);
    }
    else 
    {
        //SCI_TRACE_LOW:"MAIL: PlaymailRing ring_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9610_112_2_18_2_48_44_527,(uint8*)"d", ring_type);

#ifdef JAVA_SUPPORT
        if(MMIAPIJAVA_IsFocusJAVARunWin()
#ifdef STREAMING_HS_SUPPORT //
            ||MMIAPISMP_IsStreamPlaying()
#endif // STREAMING_HS_SUPPORT
            )
        {
            ring_type = MMISET_MSG_VIBRA;
        }
#endif
#ifdef WRE_SUPPORT
        if(WRE_IsFocusRunWin())
        {
                ring_type = MMISET_MSG_VIBRA;
        }
#endif
        switch( ring_type )
        {
        case MMISET_MSG_RING:

            MMIAPISET_PlayRing(dual_sys, FALSE, 0,1, MMISET_RING_TYPE_MSG, MailRingPlayCallBack);
#ifdef FEA_NEVER  //去除插入耳机启动震动，创建铃声定时器
            if (0 != g_mail_ring_timer_id)
            {
                MMK_StopTimer(g_mail_ring_timer_id);
                g_mail_ring_timer_id = 0;
            }
            g_mail_ring_timer_id = MMK_CreateTimerCallback(MMIMAIL_RING_LEN, MMIAPIMail_HandleTimerForRing, NULL, FALSE);
#else
            if (GPIO_CheckHeadsetStatus())     //耳机插入时启动振动
            {
                if ( 0 == s_play_mail_vibrator_timer_id)
                {
                    MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_MSG);
                    s_play_mail_vibrator_timer_id = MMK_CreateTimerCallback(MMIMAIL_RING_LEN, MMIAPIMail_HandleTimerForRing, NULL, FALSE);
                }
            }
#endif /*  */
            break;
            
        case MMISET_MSG_VIBRA:
            
            if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_MSG))
            {
                MMIAPISET_StopAllVibrator();
                if ( 0 == s_play_mail_vibrator_timer_id)
                {
                    MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_MSG);
                    s_play_mail_vibrator_timer_id = MMK_CreateTimerCallback(MMIMAIL_RING_LEN, MMIAPIMail_HandleTimerForRing, NULL, FALSE);
                }
            }
            break;
            
        case MMISET_MSG_SILENT:
            
            break;
            
        case MMISET_MSG_VIBRA_AND_RING:
            
#ifdef FEA_NEVER
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_MSG))
        {
            if ( 0 == MMIAPIENVSET_GetCurModeOptValue(dual_sys, MSG_RING_VOL) )//静音的时候，设置震动
            {
                MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, MMIMAIL_VIBRA_TIMEROUT, 0, 1, PNULL);
            }
            else
            {
                
                MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG, MailRingPlayCallBack);
                if (0 != g_mail_ring_timer_id)
                {
                    MMK_StopTimer(g_mail_ring_timer_id);
                    g_mail_ring_timer_id = 0;
                }
                MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_MSG);
                g_mail_ring_timer_id = MMK_CreateTimerCallback(MMIMAIL_RING_LEN, MMIAPIMail_HandleTimerForRing, NULL, FALSE);
            }
        }
#else 
            if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_MSG))
            {
                if ( 0 == MMIAPIENVSET_GetCurModeOptValue(dual_sys, MSG_RING_VOL) )
                {
                    MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, MMIMAIL_VIBRA_TIMEROUT, 0, 1, PNULL);
                }
                else
                {                    
                    MMIAPISET_PlayRing(dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_MSG, MailRingPlayCallBack);
                    if ( 0 == s_play_mail_vibrator_timer_id)
                    {
                        MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_MSG);
                        s_play_mail_vibrator_timer_id = MMK_CreateTimerCallback(MMIMAIL_RING_LEN, MMIAPIMail_HandleTimerForRing, NULL, FALSE);
                    }
                }
            }
#endif
            break;
            
        default:
            break;
        }
    }
}


LOCAL int new_msg_num = 0;

/*==========================================================
 * Function     : MMIMAIL_NewArrivalMsgNum
 * Description : 新邮件个数-如果进入邮箱，清空为0
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/10/2010 
 ==========================================================*/
LOCAL int MMIMAIL_NewArrivalMsgNum(void)
{
    return new_msg_num;
}


/*==========================================================
 * Function     : MMIMAIL_SetNewArrivalMsgNum
 * Description : 新邮件个数-如果进入邮箱，清空为0
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/10/2010 
 ==========================================================*/
LOCAL void MMIMAIL_SetNewArrivalMsgNum(int num)
{
    new_msg_num = num;
}


/*==========================================================
 * Function     : MMIMail_NewMailArrival
 * Author	    : sunguochen      
 * Description : 新邮件到达处理
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/10/2010 
 ==========================================================*/
PUBLIC void MMIMail_NewMailArrival(mail_retr_result_t *retr_result)
{

    MMI_STRING_T recv_str           = {0};
    MMI_STRING_T mails_str          = {0};
    MMI_STRING_T notify_str         = {0};  
    char mail_numstr[5]                = {0};
    wchar mail_num_wstr[5]          = {0};    
    uint32  time                            = 0;
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T alert_win_id       = MMIMAIL_NEW_MAIL_ALERT_WIN_ID;
    int total_new_msg_num = 0;
    if(retr_result->retr_result == MAIL_RETR_ERR_NOSPACE)
    {
        MMIDEFAULT_SetBackLight(TRUE);
        //SCI_TRACE_LOW:"MMIMail_NewMailArrival No space "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9769_112_2_18_2_48_45_528,(uint8*)"");
        MMIMAIL_OpenMsgBox(TXT_MAIL_INSUFFICIENT_MEMORY, IMAGE_PUBWIN_WARNING);
        return;
    }

    if(retr_result->retr_result == MAIL_RETR_ERR_BOXFULL)
    {
        MMIDEFAULT_SetBackLight(TRUE);
        //SCI_TRACE_LOW:"MMIMail_NewMailArrival box is full"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9777_112_2_18_2_48_45_529,(uint8*)"");
        MMIMAIL_OpenMsgBox(TXT_MAIL_BOX_FULL, IMAGE_PUBWIN_WARNING);
        return;
    }
    
    if(retr_result->retr_num == 0)
    {
        //SCI_TRACE_LOW:"MMIMail_NewMailArrival recv 0 mail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9784_112_2_18_2_48_45_530,(uint8*)"");
        return;
    }
    
    //更新icon区显示
    MAIN_SetMsgState(TRUE);

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    //在手机邮箱和短信界面-只振铃-不显示提示窗
    if (MMIMAIL_IsReady() || MMIMAIL_IsSmsOpen())
#else
    //在手机邮箱界面-只振铃-不显示提示窗
    if (MMIMAIL_IsReady())
#endif
    {
        //SCI_TRACE_LOW:"MMIMail_NewMailArrival in mail box "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9799_112_2_18_2_48_45_531,(uint8*)"");
        MMIMAIL_PlayMailSound();
        return;

    }

#ifdef BROWSER_SUPPORT
    else if (MMIAPIBROWSER_IsActive())
    {
        win_priority = WIN_ONE_LEVEL;
    }
#endif
#ifdef VT_SUPPORT
    else if (MMIAPIVT_IsVtConnecting())
    {
        win_priority = WIN_ONE_LEVEL;
    }
#endif
    else if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        win_priority = WIN_ONE_LEVEL;
    }

#ifdef MBBMS_SUPPORT
    else if (MMIAPIMTV_IsMTVRunning())
    {
        win_priority = WIN_ONE_LEVEL;
    }
#endif

    MMIDEFAULT_SetBackLight(TRUE);

    MMI_GetLabelTextByLang(TXT_MAIL_RECEIVE, &recv_str);
    MMI_GetLabelTextByLang(TXT_MAIL_MAIL_ITEM, &mails_str);

    total_new_msg_num = retr_result->retr_num + MMIMAIL_NewArrivalMsgNum();
    MMIMAIL_SetNewArrivalMsgNum(total_new_msg_num);
    sprintf(mail_numstr, " %d ", total_new_msg_num);

    MMIAPICOM_StrToWstr((uint8 *)mail_numstr, mail_num_wstr);

    notify_str.wstr_len = MMIAPICOM_Wstrlen(mail_num_wstr) + recv_str.wstr_len + mails_str.wstr_len ;

    notify_str.wstr_ptr = SCI_ALLOC((notify_str.wstr_len + 1)*sizeof(wchar) );

    memset(notify_str.wstr_ptr, 0, (notify_str.wstr_len + 1)*sizeof(wchar) );

    MMIAPICOM_Wstrncat(notify_str.wstr_ptr , recv_str.wstr_ptr, recv_str.wstr_len);
    MMIAPICOM_Wstrcat(notify_str.wstr_ptr , mail_num_wstr);
    MMIAPICOM_Wstrncat(notify_str.wstr_ptr, mails_str.wstr_ptr, mails_str.wstr_len);    
    
    if (MMK_IsOpenWin(MMIMAIL_NEW_MAIL_ALERT_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_NEW_MAIL_ALERT_WIN_ID);
    }
    //SCI_TRACE_LOW:"MMIMail_NewMailArrival "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9854_112_2_18_2_48_45_532,(uint8*)"");

    //MMIPUB_OpenAlertWinByTextPtr(&time,&prompt_str,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMIMAIL_HandleNewMsgWin);
    MMIPUB_OpenAlertWinByTextPtr(&time,&notify_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMIMAIL_HandleNewMsgWin);

    MMIMAIL_PlayMailSound();

    SCI_FREE(notify_str.wstr_ptr);

}

/*****************************************************************************/
//  Description : email send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MailSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm)
{//dm 不需要做任何处理，直接结束流程
    BOOLEAN result = TRUE;

    if (!is_succss)
    {
        MMIAPIMAIL_GetAccountCallback(FALSE, TRUE);
    }
    return result;
}

/*==========================================================
 * Function     : MMIMAIL_checkAccount
 * Author	    : sunguochen      
 * Description : 检查账号情况 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/11/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_checkAccount(void)
{
    MMISMS_SEND_DATA_T send_data = {0};
    MMI_STRING_T    message_content = {0};

    message_content.wstr_ptr = MMIMAIL_CHECK_MSG;
    message_content.wstr_len = MMIAPICOM_Wstrlen(message_content.wstr_ptr);
    send_data.sms_content_ptr = &message_content;
    send_data.dst_port = 0;
    send_data.src_port = 0;
    send_data.dest_addr_ptr = (uint8 *)MMIMAIL_CHECK_NUM;
    send_data.dest_addr_len = strlen(MMIMAIL_CHECK_NUM);
    send_data.dual_sys = MN_DUAL_SYS_1;
    send_data.is_need_packet = TRUE;
    send_data.send_callback = MailSMSSendCnfCallback;

    return MMIAPISMS_SendSmsForOther(&send_data);
}

/*==========================================================
 * Function     : MMIMAIL_checkAccount
 * Author	    : sunguochen      
 * Description : 开通手机邮箱
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/11/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_applyAccount(void)
{
    MMISMS_SEND_DATA_T send_data = {0};
    MMI_STRING_T    message_content = {0};

    message_content.wstr_ptr = MMIMAIL_APPLY_MSG;
    message_content.wstr_len = MMIAPICOM_Wstrlen(message_content.wstr_ptr);
    send_data.sms_content_ptr = &message_content;
    send_data.dst_port = 0;
    send_data.src_port = 0;
    send_data.dest_addr_ptr = (uint8 *)MMIMAIL_APPLY_NUM;
    send_data.dest_addr_len = strlen(MMIMAIL_APPLY_NUM);
    send_data.dual_sys = MN_DUAL_SYS_1;
    send_data.is_need_packet = TRUE;
    send_data.send_callback = MailSMSSendCnfCallback;

    return MMIAPISMS_SendSmsForOther(&send_data);
}

/*==========================================================
 * Function     : MMIMAIL_logOutAccount
 * Author	    : sunguochen      
 * Description : 开通手机邮箱
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/11/2010 
 ==========================================================*/
LOCAL BOOLEAN MMIMAIL_logOutAccount(void)
{
    MMISMS_SEND_DATA_T send_data = {0};
    MMI_STRING_T    message_content = {0};

    message_content.wstr_ptr = MMIMAIL_LOGOUT_MSG;
    message_content.wstr_len = MMIAPICOM_Wstrlen(message_content.wstr_ptr);
    send_data.sms_content_ptr = &message_content;
    send_data.dst_port = 0;
    send_data.src_port = 0;
    send_data.dest_addr_ptr = (uint8 *)MMIMAIL_APPLY_NUM;
    send_data.dest_addr_len = strlen(MMIMAIL_APPLY_NUM);
    send_data.dual_sys = MN_DUAL_SYS_1;
    send_data.is_need_packet = TRUE;
    send_data.send_callback = MailSMSSendCnfCallback;

    return MMIAPISMS_SendSmsForOther(&send_data);
}

/*****************************************************************************/
// 	Description : handle waiting  win msg  for  saving  as while coping file
//	Global resource dependence : 
//  Author: robert.wang
//  Date: 09-8-20
//	Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleCopyAccWaitWinMsg(
                                       MMI_WIN_ID_T        win_id,   //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in  
                                       DPARAM              param    //in
                                       )
{

    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    BOOLEAN                     copy_result=FALSE;
    
    switch( msg_id)
    {
        case MSG_MAIL_COPY_ACC_CNF:
            copy_result=*(BOOLEAN *)param;

            //SCI_TRACE_LOW:"HandleCopyAccWaitWinMsg  copy_result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_9981_112_2_18_2_48_45_533,(uint8*)"d",copy_result);
            
            if(!copy_result)
            {
                MMIPUB_OpenAlertWarningWin(TXT_MMI_SAVEFAILED);                
#ifdef PIC_EDITOR_SUPPORT				
                MMIAPIPIC_ExitFromPicEditor(); 
#endif
                return (result);
            }
            else
            {
                //add save successfully prompt	
                MMIPUB_OpenAlertSuccessWin( TXT_COMM_SAVE_SUCCESS);        
            }

            MMK_CloseWin(win_id);
            break;
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
            break;
        default:
           result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : 拷贝附件到本地目录
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void CopyAccToFolder(
                                wchar     *acc_full_path, //附件的全路径，包含文件名
                                wchar     *select_foler_full_path   //选中目录
                                  )
{
    uint32          time_out = 0;
    uint32          file_size = 0;
    SFS_DATE_T      data = {0};    
    SFS_TIME_T      time = {0}; 
    BOOLEAN         ret_value = TRUE;
    uint16          acc_full_path_len = MMIAPICOM_Wstrlen(acc_full_path);
    uint16          select_foler_full_path_len = MMIAPICOM_Wstrlen(select_foler_full_path);
    MMI_WIN_ID_T    wait_win_id = MMIMAIL_COPY_FILE_WAIT_WIN_ID;
    uint16          device_name[MMIFILE_DEVICE_NAME_MAX_LEN] = {0};
    uint16          device_len=0;

    uint16          dir_name[MMIFILE_DIR_NAME_MAX_LEN] = {0};
    uint16          dir_len=0;            
    
    uint16          new_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16          new_path_len = MMIFILE_FULL_PATH_MAX_LEN ;
    
    //uint32          free_high_word = 0;
    //uint32          free_low_word = 0;

    BOOLEAN         disk_result = FALSE;
    //BOOLEAN         is_udisk = FALSE;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    uint32          i = 0;

    //SCI_TRACE_LOW:"select_foler_full_path = "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10045_112_2_18_2_48_45_534,(uint8*)"");
    for(i=0; i<select_foler_full_path_len; i++)
    {
        //SCI_TRACE_LOW:"%d:%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10048_112_2_18_2_48_45_535,(uint8*)"dd",i, select_foler_full_path[i]);
    }

    MMIAPIFMM_GetFileInfo(acc_full_path, acc_full_path_len, &file_size, &data, &time);
    //SCI_TRACE_LOW:"file_size = "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10052_112_2_18_2_48_45_536,(uint8*)"", file_size);
    MMIAPIFMM_SplitFullPath(acc_full_path, acc_full_path_len, device_name, &device_len, dir_name, &dir_len, new_full_path, &new_path_len);

    select_foler_full_path[select_foler_full_path_len] = MMIFILE_SLASH;

    MMI_WSTRNCPY(select_foler_full_path + select_foler_full_path_len + 1,new_path_len,
                new_full_path,new_path_len,
                new_path_len);        
            
    select_foler_full_path_len = select_foler_full_path_len+1+new_path_len;     
    device_type = MMIAPIFMM_GetDeviceTypeByPath(select_foler_full_path, MMIFILE_DEVICE_LEN);

    disk_result = MMIAPIFMM_IsDevEnoughSpace(disk_result, device_type);
    if(disk_result)
    {

        if(!MMIAPIFMM_IsFileExist(select_foler_full_path,select_foler_full_path_len))
        {
            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleCopyAccWaitWinMsg);

            ret_value=MMIAPIFMM_CopyOneFile(acc_full_path, acc_full_path_len, \
                 select_foler_full_path, select_foler_full_path_len, wait_win_id, MSG_MAIL_COPY_ACC_CNF);

            //如果失败，直接关闭等待窗口
            if(!ret_value)        
            {
                 MMK_CloseWin(wait_win_id);
            }
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_FILE_EXISTED,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);   
    }
    
    //SCI_TRACE_LOW:"MMIAPIFMM_CopyOneFile ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10091_112_2_18_2_48_45_537,(uint8*)"d",ret_value);
  }
/*****************************************************************************/
//  Description : hand file list
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
#ifdef JIAOYOUWU_TEMP
LOCAL MMI_RESULT_E  HandleCopyAccToFolderWinMsg (
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM      param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    wchar          selected_folder[MMIFMM_PATHNAME_LEN]= {0};
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //only process web key
        MMIAPIFMM_GetCurrentPathName(selected_folder, MMIFILE_FULL_PATH_MAX_LEN);
    	CopyAccToFolder(s_acc_file_ptr, selected_folder);

        MMIAPIFMM_Exit();
        break;
	default:
    	result = MMI_RESULT_FALSE;
    	break;
    }
    return (result);
}

#else
LOCAL void HandleCopyAccToFolderWinMsg(MMIFMM_DATA_INFO_T *list_data_ptr, uint32 index)
{
    wchar file_name_ptr[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  full_name_len = 0;

    //获取文件全路经
    MMI_WSTRNCPY(file_name_ptr, MMIFILE_FULL_PATH_MAX_LEN, list_data_ptr->pathname, list_data_ptr->pathname_len, list_data_ptr->pathname_len);

    //插入"\"符
    full_name_len = list_data_ptr->pathname_len;
    *(file_name_ptr + full_name_len) = MMIFILE_SLASH;
    full_name_len++;

    MMI_WSTRNCPY(file_name_ptr + full_name_len, MMIFILE_FULL_PATH_MAX_LEN - full_name_len, list_data_ptr->data[index]->filename_ptr, list_data_ptr->data[index]->filename_len, list_data_ptr->data[index]->filename_len);

    CopyAccToFolder(s_acc_file_ptr, file_name_ptr);
}



#endif

/*****************************************************************************/
//  Description : 拷贝附件到本地目录
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyAccToFolder(
                                  wchar     *acc_full_path, //附件的全路径，包含文件名
                                  uint16    full_path_len   //全路径长度
                                  )
{
    //uint16   current_index =0;
    uint32   i = 0;
    uint16   len = 0;
    //MMIFILE_FILE_INFO_T file_info = {0};
    //MMIFMM_SOFTKEY_TEXT_T sk_text_id = {STXT_OK, TXT_NULL, STXT_RETURN};
    wchar root_path[3] = {'D', ':', 0};
    MMIFMM_OPENWIN_INFO_T openwin_info = {0};
    MMIFILE_DEVICE_E dev       = MMI_DEVICE_UDISK;
    wchar*root_path_ptr = PNULL;
    
    //SCI_TRACE_LOW:"MMIMAIL==> MMIAPIFMM_CopyAccToFolder: Enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10167_112_2_18_2_48_45_538,(uint8*)"");

    if (PNULL == s_acc_file_ptr)
    {
        s_acc_file_ptr = comm_alloc(MMIFMM_PATHNAME_LEN*sizeof(wchar));//mod by liangshun 20110315 for 解决附件有时保存失败的问题
        if (PNULL == s_acc_file_ptr)
        {
            //SCI_TRACE_LOW:"MMIMAIL==> MMIAPIFMM_CopyAccToFolder: alloc s_selected_file_ptr fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10174_112_2_18_2_48_45_539,(uint8*)"");
            return;
        }
    }
    
    //拷贝全路径
    len = MIN(full_path_len, MMIFMM_PATHNAME_LEN);
    memset(s_acc_file_ptr, 0, MMIFMM_PATHNAME_LEN*sizeof(wchar));//added by liangshun 20110315 for 解决附件有时保存失败的问题
    MMIAPICOM_Wstrncpy(s_acc_file_ptr, acc_full_path, len);

    //SCI_TRACE_LOW:"s_acc_file_ptr = "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10184_112_2_18_2_48_45_540,(uint8*)"");
    for(i=0; i<len; i++)
    {
        //SCI_TRACE_LOW:"%d:%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10187_112_2_18_2_48_45_541,(uint8*)"dd",i, s_acc_file_ptr[i]);
    }

    dev = MMI_DEVICE_SDCARD;

    do
    {
        if(MMIAPIFMM_GetDeviceTypeStatus(dev))
        {
            root_path_ptr = MMIAPIFMM_GetDevicePath(dev);
            break;
        }
    }while(++dev < MMI_DEVICE_NUM);
    if (PNULL == root_path_ptr)
    {
        dev = MMI_DEVICE_UDISK;

        if(MMIAPIFMM_GetDeviceTypeStatus(dev))
        {
            root_path_ptr = MMIAPIFMM_GetDevicePath(dev);
        }
    }

    if (PNULL != root_path_ptr)
    {
        root_path[0] = *root_path_ptr;
    }

    openwin_info.path_ptr = root_path;
    openwin_info.path_len = MMIAPICOM_Wstrlen(root_path);
    openwin_info.filename = PNULL;
    openwin_info.file_len = 0;
    openwin_info.is_need_display = TRUE;
    openwin_info.find_suffix_type = MMIFMM_UNKNOW_TYPE;
    openwin_info.callback.callback_web = PNULL;
    openwin_info.callback.callback_can = PNULL;
    openwin_info.callback.callback_ok = HandleCopyAccToFolderWinMsg;

    openwin_info.sk_text_id.leftsoft_id = STXT_SAVE;
    openwin_info.sk_text_id.middlesoft_id = TXT_NULL;
    openwin_info.sk_text_id.rightsoft_id = STXT_RETURN;
    openwin_info.privacy_protect = FALSE;

    MMIAPIFMM_OpenExplorerWin(&openwin_info);

    #if 0
    if (PNULL == s_copy_del_path_data_ptr)
    {
        s_copy_del_path_data_ptr = SCI_ALLOC_APP(sizeof(MMIFMM_COPY_DEL_PATH_DATA_INFO_T));
        if (PNULL == s_copy_del_path_data_ptr)
        {
            //SCI_TRACE_LOW:"MMIMAIL==> MMIAPIFMM_CopyAccToFolder: alloc s_copy_del_path_data_ptr fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10239_112_2_18_2_48_46_542,(uint8*)"");
            return;
        }
    }
    SCI_MEMSET(s_copy_del_path_data_ptr, 0, sizeof(MMIFMM_COPY_DEL_PATH_DATA_INFO_T));

    //拷贝全路径
    len = MIN(full_path_len, MMIFMM_PATHNAME_LEN);
    MMIAPICOM_Wstrncpy(s_copy_del_path_data_ptr->cur_pathname, acc_full_path, len);

    //解析全路径
    MMIAPIFMM_GetFileInfoFormFullPath((const wchar *)acc_full_path,
                                        full_path_len,
                                        &file_info);
                                        
    //备份原始文件路径
    MMIAPICOM_Wstrncpy(s_copy_del_path_data_ptr->user_scr_path,file_info.device_name,file_info.device_name_len);
    s_copy_del_path_data_ptr->user_scr_path[file_info.device_name_len] = MMIFILE_COLON;
    s_copy_del_path_data_ptr->user_scr_path[file_info.device_name_len + 1] = MMIFILE_SLASH;
    
    MMIAPICOM_Wstrncat(s_copy_del_path_data_ptr->user_scr_path,file_info.dir_name,file_info.dir_name_len);
    len = MMIAPICOM_Wstrlen((const wchar *)s_copy_del_path_data_ptr->user_scr_path);
    s_copy_del_path_data_ptr->user_scr_path[len] = MMIFILE_SLASH;

    //赋初始盘符信息
    if (PNULL == s_fmm_path_ptr)
    {
        s_fmm_path_ptr = SCI_ALLOC_APP(sizeof(MMIFMM_CURRENT_WIN_PATH_T));
    }
    SCI_MEMSET(s_fmm_path_ptr, 0, sizeof(MMIFMM_CURRENT_WIN_PATH_T));
    s_fmm_path_ptr->current_device = MMI_DEVICE_UDISK;
    #endif
 }

/*****************************************************************************/
//  Description : 释放拷贝附件时分配的临时空间
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_FreeAccTmpSpace(void)
{    
    //SCI_TRACE_LOW:"MMIMAIL==> MMIAPIFMM_FreeAccTmpSpace: Enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10279_112_2_18_2_48_46_543,(uint8*)"");
    
    if (PNULL != s_acc_file_ptr)
    {
        SCI_FREE(s_acc_file_ptr);
    }
    #if 0
    if (PNULL != s_fmm_path_ptr)
    {
        SCI_FREE(s_fmm_path_ptr);
    }
    #endif
}

/*****************************************************************************/
//  Description : 激活PDP
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAILPDP_Active(uint32 account_id, MMIPDP_MSG_CALLBACK callback_func)
{
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T setting_detail_ptr = {0};
    BOOLEAN result = FALSE;
    
    comm_NetGetSetting(account_id, &setting_detail_ptr);

    app_info.app_handler = mail_AppHandler( );
    app_info.apn_ptr = (char*)setting_detail_ptr.apn;
    app_info.user_name_ptr = (char*)setting_detail_ptr.username;
    app_info.psw_ptr = (char*)setting_detail_ptr.password;
    app_info.dual_sys = (MN_DUAL_SYS_E)MMIAPISET_GetActiveSim();
    app_info.ps_service_type = BROWSER_E;
    app_info.ps_service_rat = MN_TD_PREFER;
    app_info.priority = 3;
    app_info.handle_msg_callback = callback_func;
    app_info.storage = MN_GPRS_STORAGE_ALL;

    //COMM_TRACE:"Mail:MMIMAILPDP_Active apn:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10315_112_2_18_2_48_46_544,(uint8*)"s", app_info.apn_ptr);
    
    // The call back function will be triggered immediatelly in some situations, set the flag to avoid the timing issue
    result = MMIAPIPDP_Active(&app_info);
    
    return result;
}

/*****************************************************************************/
//  Description : 去激活PDP
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAILPDP_Deactive(uint32 app_handler)
{
    return MMIAPIPDP_Deactive(app_handler);
}

/*****************************************************************************/
//	Description : 邮件应用是否处于最上层
//	Global resource dependence : none
//	Author:jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsActive(void)
{
    BOOLEAN ret = FALSE;
    MMI_WIN_ID_T win_id = MMIMAIL_START_WIN_ID;

    for (win_id = MMIMAIL_START_WIN_ID + 1; win_id <  MMIMAIL_END_WIN_ID; win_id++)
    {
        if (MMK_IsFocusWin(win_id))
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : insert string to sms edit
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             )
{
    BOOLEAN result = FALSE;

 
    if(MMK_IsOpenWin(MMIMAIL_EDIT_RECVER_LIST_WIN_ID))
    {
        result = GUIEDIT_InsertString(MMIMAIL_EDIT_RECVER_LIST_CTRL_ID, wstr_ptr, wstr_len);
        MMK_SendMsg(MMIMAIL_EDIT_RECVER_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    else if (MMK_IsOpenWin(MMIMAIL_EDIT_SUBJECT_WIN_ID))
    {
        result = GUIEDIT_InsertString(MMIMAIL_EDIT_TEXT_CTRL_ID, wstr_ptr, wstr_len); 
        MMK_SendMsg(MMIMAIL_EDIT_SUBJECT_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    else if (MMK_IsOpenWin(MMIMAIL_EDIT_CONTENT_WIN_ID))
    {
        result = GUIEDIT_InsertString(MMIMAIL_EDIT_TEXT_CTRL_ID, wstr_ptr, wstr_len); 
        MMK_SendMsg(MMIMAIL_EDIT_CONTENT_WIN_ID, MSG_FULL_PAINT, PNULL);
    }

    //SCI_TRACE_LOW:"[MMISMS ATC] MMIAPIMAIL_InsertMessage result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_WINTAB_10383_112_2_18_2_48_46_545,(uint8*)"d", result);

    return result;
}



/*Edit by script, ignore 29 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
