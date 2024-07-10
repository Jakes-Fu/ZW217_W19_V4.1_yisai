/****************************************************************************
** File Name:      mmiset_security.c                                             *
** Author:                                                                   *
** Date:           15/08/2006                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2006        kelly.li         create
******************************************************************************/

#define _MMISET_SECURITY_C_


/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_type.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmi_nv.h"
#include "guimsgbox.h"
#include "mmk_timer.h"
#if defined(MMI_MSD_SUPPORT)
#include "mmimsd_export.h"
#endif
#include "mmiphone_export.h"
#include "mmiset_security.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmiset_wintab.h"
#include "mmiset_func.h"
#include "mmiset_text.h"
#include "mmicl_export.h"
#include "mmisms_text.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmipb_export.h"//@zhaohui,cr103407
#include "mmifdn_export.h"
#ifdef ENGTD_SUPPORT
//#include "mmiengtd_win.h"//MS00216980 cheney
#include "mmiengtd_export.h"
#endif
#include "guiform.h"
#include "guisetlist.h"
#include "guiedit.h"
#include "guilabel.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef ASP_SUPPORT
#include "mmiasp_text.h"
#endif
#ifdef DPHONE_SUPPORT
#include "mmiset_menutable.h"
#endif

#ifdef MMI_PDA_SUPPORT
#include "mmi_appmsg.h"
#endif
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
#include "mmikl_export.h"
#endif
#endif
#include "mmi_appmsg.h"
#include "mmiset_menutable.h"
#include "mmiidle_subwintab.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "mmiset_image.h"
#include "guictrl_api.h"
#include "guires.h"
#ifdef ENGTD_SUPPORT
#include "mmieng_win.h"
#endif
#ifdef MMI_SIM_LOCK_SUPPORT
#include "guitext.h"
#endif
#include "mmiap_appmain.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_LIST_STRNUM_MAX_LEN 40

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

PUBLIC uint8   g_mmiset_3seconds_timer_id =0;
PUBLIC MMISET_PWD_OPERATE_TYPE_E   g_mmiset_pwd_type = MMISET_PWD_SET; //the password type of power on

#if defined(MMI_BLACKLIST_SUPPORT)
LOCAL MMIPB_BCD_NUMBER_T *s_new_blacklist_num_ptr = PNULL;
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
    MMISMS_PDA_PIN1_STATUS,
    MMISMS_PDA_PIN1_CHANGE,
    MMISMS_PDA_PIN1_SETTINGS_MAX    
}MMISMS_PDA_PIN1_SETTINGS_TYPE_E;

typedef enum
{
    MMISMS_PDA_SECURITY_SETTINGS_INIT,    
    MMISMS_PDA_SECURITY_SETTINGS_PING,
    MMISMS_PDA_SECURITY_SETTINGS_PING2,
    MMISMS_PDA_SECURITY_SETTINGS_PHONE_LOCK,
    MMISMS_PDA_SECURITY_SETTINGS_CELLPHONE_PASSWORD,
    MMISMS_PDA_SECURITY_SETTINGS_PRAVICY,
    MMISMS_PDA_SECURITY_SETTINGS_AUTOKEYPAD,
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT    
    #ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
    MMISMS_PDA_SECURITY_SETTINGS_KEYLOCK_PSW,
    #endif
#endif    
    MMISMS_PDA_SECURITY_SETTINGS_ENDKEY_LOCK,
    MMISMS_PDA_SECURITY_SETTINGS_GAURD_LOCK,
    MMISMS_PDA_SECURITY_SETTINGS_FIXDIAL_NUMBER,

#ifdef CMCC_UI_STYLE
    MMISMS_PDA_SECURITY_SETTINGS_UPLMN,
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    MMISMS_PDA_SECURITY_SETTINGS_BLACKLIST,                
#endif
    MMISMS_PDA_SECURITY_SETTINGS_MAX    
}MMISMS_PDA_SECURITY_SETTINGS_TYPE_E;

#define MMISECURITY_LIST_STRNUM_MAX_LEN 40
#endif /* MMI_GUI_STYLE_TYPICAL */
#ifdef MMI_SIM_LOCK_SUPPORT
#ifdef WIN32
/*{
    MN_PHONE_COMPANY_ID_E      company_id;
    uint32                     SIM_lock_status;
    uint32                     max_num_trials;
    uint32                     time_interval;
    uint32                     time_multiplier;
    NETWORK_LOCKS_T            network_locks;
    NETWORK_SUBSET_LOCKS_T     network_subset_locks;
    SP_LOCKS_T                 SP_locks;
    CORPORATE_LOCKS_T          corporate_locks;
    uint32                     dummy1;        //reserve for future
    uint32                     dummy2;        //reserve for future
    uint32                     dummy3;        //reserve for future
    uint32                     dummy4;        //reserve for future
    uint32                     encrypted_CRC[4];
}*/
static MN_SIM_LOCK_CUSTOMIZE_DATA_T  s_sim_lock_customize_data =
{
    MN_PHONE_COMPANY_UNKNOWN,
    0x1F, //SIM_lock_status 0000 0000 0001 1111
    0x0A, //max_num_trials 10
    0, //time_interval
    0x02, //time_multiplier
    {  3,    //numLocks
        0,  //reserved
        {001, 01, 02, 002, 02, 02 , 002, 02, 02}},//network_locks
    {3, 0, {460, 0, 2, 4, 5, 460, 0, 2, 4, 5, 460, 0, 2, 4, 5}}, //network_subset_locks
    {3, 0, {460, 0, 2, 1, 0, 460,1, 2, 2, 0, 460, 2, 2, 3, 0}}, //SP_locks
    {3, 0, {460, 0, 2, 1, 1, 460,1, 2, 2, 2, 460, 2, 2, 3, 3}}, //corporate_locks
};
/*{
    uint32                     SIM_lock_status;
    uint32                     nck_trials;
    uint32                     nck_unlock_time;
    uint32                     nsck_trials;
    uint32                     nsck_unlock_time;
    uint32                     spck_trials;
    uint32                     spck_unlock_time;
    uint32                     cck_trials;
    uint32                     cck_unlock_time;
    uint32                     pck_trials;
    uint32                     pck_unlock_time;
    uint32                     dummy1;        //reserve for future
    uint32                     dummy2;        //reserve for future
    uint32                     dummy3;        //reserve for future
    uint32                     dummy4;        //reserve for future
    USER_LOCKS_T               user_locks;
    uint32                     encrypted_CRC[4];
}
*/
static MN_SIM_LOCK_USER_DATA_T       s_sim_lock_user_data =
{
    0x1F, //SIM_lock_status 0000 0000 0001 1111
    0, //nck_trials
    0,
    0, //nsck_trials
    0,
    0, //spck_trials
    0,
    0, //cck_trials
    0,
    0, //pck_trials
    0,
    0,
    0,
    0,
    0,
    {
      3, {8, 0x21,0x43,0x65,0x87,0x09,0x21,0x43,0x65, 8, 0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11, 8, 0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,}  //USER_LOCKS_T
    },
    0,
    0,
    0,
    0,
};
#endif
#define COLON_CHAR     0x3A
#define  TEMP_BUF_LEN  32
#define  SIM_LOCKED_NULL_STR_LEN  17
#define  NETWORK_LOCKED_STR_LEN  16
#define  NETWORK_SUBSET_LOCKED_STR_LEN  23
#define  SP_LOCKED_STR_LEN  11
#define  CORPORATE_LOCKED_STR_LEN  18
#define  USER_LOCKED_STR_LEN  13
//#define MMI_SIM_NETWORK_LOCK                1
//#define MMI_SIM_NETWORK_SUBSET_LOCK   (1 << 1)
//#define MMI_SIM_SP_LOCK               (1 << 2)
//#define MMI_SIM_CORPORATE_LOCK        (1 << 3)
//#define MMI_SIM_USER_LOCK             (1 << 4)

#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern MMISET_PWD_OPERATE_TYPE_E    g_mmiset_pwd_type;
#if defined(MMI_BLACKLIST_SUPPORT)
extern MMISET_BLACKLIST_T *g_blacklist_ptr;
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle input new privacy protect password confirm win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPrivacyPwdCnf(
                                               MMI_WIN_ID_T      win_id,
                                               MMI_MESSAGE_ID_E     msg_id,
                                               DPARAM               param
                                               );


/*****************************************************************************/
//  Description : handle input privacy protect win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputPrivacyProtectPwd(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );
/*****************************************************************************/
//     Description : 添加锁屏开启时间到radio list的item
// Global resource dependence : 
//  Author: apple.zhang
// Note:
/*****************************************************************************/
LOCAL void AppendAutoKeylockTimeListItem(MMI_CTRL_ID_T      ctrl_id);


/*****************************************************************************/
//  Description : full paint privacy protect list
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void RefreshPrivacyProtectListItem(
                                         MMI_CTRL_ID_T      ctrl_id
                                         );



/*****************************************************************************/
//  Description : to handle select privacy protect item win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPrivacyProtectWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     );

/*****************************************************************************/
//  Description : handle validate privacy protect password win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleValidatePrivacyProtectPwd(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );

/*****************************************************************************/
//  Description : to handle security setting power on password menu
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePwdMenuWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/*****************************************************************************/
//  Description : to handle security setting input old power on password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputPwdWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );


/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OperatePowerOnPwd(
                             MMISET_PWD_OPERATE_TYPE_E  operate_type
                             );

/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           );

/*****************************************************************************/
//  Description : 处理确认新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdCnfWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM                param
                                              );

/*****************************************************************************/
//  Description : handle input new privacy protect password win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPrivacyPwdWindow(
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  DPARAM                param
                                                  );

#if defined(MMI_MSD_SUPPORT)
/*****************************************************************************/
//  Description : to handle security setting msd menu
//  Global resource dependence : 
//  Author:wangxf
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdMenuWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

#endif

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : to handle security setting fdn menu
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFDNMenuWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );
#endif

/*****************************************************************************/
//  Description : to handle set fdn state
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFDNStateWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );



/*****************************************************************************/
//  Description : 设定自动锁键盘开启时间
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL void SetAudoKeyLockTime(MMI_CTRL_ID_T     ctrl_id);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : to handle security setting blacklist menu
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistMenuWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : Set Blacklist type 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistTypeParam(void);

/*****************************************************************************/
//  Description : set backlight setlist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistSetlist(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle blacklist type win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistTypeWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : save all audio setting
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeAudio(void);

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : save all video setting
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeVideo(void);
#endif

/*****************************************************************************/
//  Description : save all message setting
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeMessage(void);

/*****************************************************************************/
//  Description : handle blacklist win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBlackListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: 窗口的ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: 窗口的内部消息ID
                                            DPARAM                 param        //IN: 相应消息的参数
                                            );

/*****************************************************************************/
//  Description : update list
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void UpdateBlackList(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           );

/*****************************************************************************/
//  Description : append list
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void AppendBlacklistItem(
                                MMI_CTRL_ID_T             ctrl_id,         //要设置的列表控件ID号 (输入参数)
                                uint16                    pos,
                                BOOLEAN                   is_update
                                );

/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          );
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :InitPdaFDNSettingsListCtrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaFDNSettingsListCtrl(MN_DUAL_SYS_E dual_sys);	

#if defined(MMI_MSD_SUPPORT)
/*****************************************************************************/
//  Description :InitPdaFDNSettingsListCtrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMSDSettingsListCtrl(void);
#endif
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void HandleBlacklistMenuMsg(
                                 MMI_MENU_ID_T menu_id,    //IN: 被选中的菜单项
                                 MMI_CTRL_ID_T ctrl_id
                                 );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlackListItmeOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          );
#endif

/*****************************************************************************/
//  Description : Set Blacklist info
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void InitBlacklistInfo(void);

/*****************************************************************************/
//  Description : handle blacklist new win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNewBlacklistWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : handle blacklist edit win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBlacklistWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : Save Blacklist info
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveBlacklistInfo(
                                uint16 index,
                                BOOLEAN is_new
                               );

/*****************************************************************************/
//  Description : Save Blacklist info
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistInfo(
                            uint16 index,
                            MMIPB_BCD_NUMBER_T *sender_ptr);

/*****************************************************************************/
//  Description : to handle the blacklist of delete query window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle the blacklist of delete all query window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelAllQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle the blacklist of delete window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteBlacklistItem(
                                             uint16 pos
                                             );

/*****************************************************************************/
//  Description : to handle the blacklist of delete all window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteAllBlacklistItem(void);

/*****************************************************************************/
//  Description : set detail from PB  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistDetailFromPB(MMIPB_HANDLE_T handle);

#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
//  Description : set black list from cl 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromCL(void);
#endif

/*****************************************************************************/
//  Description : set list from PB  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromPB(MMIPB_HANDLE_T handle);

/*****************************************************************************/
// 	Description : alert of the result when add a number into blacklist
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddNumToBlacklist(
                                MMI_STRING_T mmi_pb_name,
                                MMIPB_BCD_NUMBER_T pb_bcd      
                                );

/*****************************************************************************/
//  Description : to handle add to blacklist query window    
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddToBlackListQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle  add blacklist open setting query window
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistAddQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
#endif

/*****************************************************************************/
// 	Description : to handle security setting list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecuritySetWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
/*****************************************************************************/
// 	Description : to handle security setting pin1 menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePin1MenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );
/*****************************************************************************/
// 	Description : eanble pin prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnablePin1Window(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
/*****************************************************************************/
// 	Description : modify pin prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyPin1Window(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
// 	Description : 修改pin1
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void ModifyPin1(MN_DUAL_SYS_E     dual_sys);

/*****************************************************************************/
// 	Description : get is item number of selected privacy protect
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL uint8 GetPrivacyProtecdItemNum(void);

/*****************************************************************************/
// 	Description : 添加安全设置的选项到text list的item
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void AppendSecuritySetListItem(
                                     MMI_CTRL_ID_T	    ctrl_id
                                     );

/*****************************************************************************/
// 	Description : 开启pin1
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void EnablePin1(MN_DUAL_SYS_E               dual_sys);

/*****************************************************************************/
// 	Description : disable pin prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDisablePin1Window(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : 关闭pin1
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DisablePin1(MN_DUAL_SYS_E  dual_sys);

/*****************************************************************************/
// 	Description : to handle security setting pin2 menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
/*LOCAL MMI_RESULT_E  HandlePin2MenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );*/

/*****************************************************************************/
// 	Description : modify pin2 prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyPin2Window(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
/*****************************************************************************/
// 	Description : to handle red key lock
//	Global resource dependence : 
//  Author:apple.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetRedKeyLockWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
//  Description :Init SIMLock Settings List Ctrl
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void  InitSIMLockSettingsListCtrl(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : to handle simlock setting menu
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockSettingMenuWindow(
                                                                                                       MMI_WIN_ID_T    win_id, 
                                                                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                                                                       DPARAM             param
                                                                                                       );

/*****************************************************************************/
//  Description :open reset sim lock switch menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void OpenSIMLockSwitchWin(void);

/********************************************************************************
//   NAME: GetSIMLockInfoStr
//   DESCRIPTION:   Get SIM Lock Info Str
//   PARAM IN:      sim_lock_info_buf - 
                        buf_len - 
//   PARAM OUT:SIM Lock info string len
//   AUTHOR: dave.ruan
//   Note: 
********************************************************************************/
LOCAL int GetSIMLockInfoStr(uint8** sim_lock_info_buf, uint32* buf_len);

/*****************************************************************************/
// 	Description :  handle SIM Lock info win
//	Global resource dependence : 
//    Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSIMLockInfoMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMLockSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
// 	Description : to handle security setting SIM Lock menu
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockMenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );                 
                                         
/*****************************************************************************/
// 	Description : Is SIMLock On of input type 
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  IsSIMLockOn(
                                uint32 unlock_type      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                );

#ifndef MMI_GUI_STYLE_TYPICAL 
/*****************************************************************************/
// 	Description : Append SIMLock Switch List Item
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void AppendSIMLockSwitchListItem(
                                                MMI_CTRL_ID_T	    ctrl_id,
                                                BOOLEAN is_need_replace
                                                );
#else
/*****************************************************************************/
// 	Description : to init sim lock control 
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void InitSIMLockMenuControl(MMI_WIN_ID_T win_id, uint32 dual_sys);
#endif

/*****************************************************************************/
// 	Description : to update sim lock edit control 
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void UpdateSIMLockEditControl(MMI_CTRL_ID_T ctrl_id);
#endif                                         

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : append security settings 2 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/                           
LOCAL void AppendPdaSecuritySettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data,   //用户数据
                                 BOOLEAN is_need_replace
                                 );

/*****************************************************************************/
//  Description : init security settings list ctrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/                                       
LOCAL void  InitPdaSecuritySettingsListCtrl(BOOLEAN is_need_replace);      

/*****************************************************************************/
// 	Description : init auto key lock  list
//	Global resource dependence : 
//  Author:rong.gu
//	Note: 
/*****************************************************************************/                                      
LOCAL void  InitPdaSecurityAutoKeyLockListCtrl(void);    


#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
/*****************************************************************************/
//  Description : handle auto key lock set
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/   
LOCAL MMI_RESULT_E  HandleSetAutoKeyLockWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

#endif    
#endif
/*****************************************************************************/
//  Description : append security settings 1 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaSecuritySettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 );
                                 

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
/******************************************************************************/
//  Description : Create key lock psw settting window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void OpenKLPswSettingEnterWin(void);

/******************************************************************************/
//  Description : Create key lock psw settting window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void OpenKLPswSettingSwitchOffWin(void);

/******************************************************************************/
//  Description : Create key lock psw settting window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void OpenKLPswSettingChangeWin(void);

/******************************************************************************/
//  Description : handle kl password setting.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLPswSettingWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        );
#endif

#endif

#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
/*****************************************************************************/
// 	Description : Power on password input window softkey
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN PwdInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      );

/*****************************************************************************/
// 	Description : Make Emergency Call
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void MakeEmergencyCall(MMI_STRING_T *str_info);
#endif

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/



//the window for validate privacy password
WINDOW_TABLE( MMISET_VALIDATE_PRIVACY_PWD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleValidatePrivacyProtectPwd),    
        WIN_ID( MMISET_VALIDATE_PRIVACY_PWD_WIN_ID ),
        WIN_TITLE( TXT_INPUT_PHONE_PASSWORD ),//cr227782 maryxiao
        CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_VALIDATE_PRIVACY_PWD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};


//the window for set privacy protect item win
WINDOW_TABLE( MMISET_SET_PRIVACY_PROTECT_ITEM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32) HandleSelectPrivacyProtectWindow),   
        WIN_ID( MMISET_SET_PRIVACY_PROTECT_ITEM_WIN_ID ),
        WIN_TITLE( TXT_SET_SET_PRIVACY_PROTECT ),
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SET_PRIVACY_PROTECT_ITEM_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the window for input privacy protect password win
WINDOW_TABLE( MMISET_SET_PRIVACY_PROTECT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputPrivacyProtectPwd),    
        WIN_ID( MMISET_INPUT_PRIVACY_PROTRCT_PWD_WIN_ID ),
        WIN_TITLE( TXT_INPUT_PHONE_PASSWORD ),//cr227782 maryxiao
        CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_INPUT_PRIVACY_PROTRCT_PWD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for security setting power on password menu
WINDOW_TABLE( MMISET_SET_SECURITY_PWD_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePwdMenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_PWD_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_SET_POWERON_PWD_OPT, MMISET_SET_SECURITY_PWD_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input old power on password
WINDOW_TABLE( MMISET_SET_SECURITY_PWD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputPwdWindow ),    
        WIN_ID( MMISET_SET_SECURITY_PWD_WIN_ID ),
        WIN_TITLE( TXT_INPUT_PHONE_PASSWORD ), //cr227782 maryxiao
        CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_SET_SECURITY_PWD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the window for input new privacy protect password
WINDOW_TABLE( MMISET_PRIVACY_PROTECT_NEW_PWD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputNewPrivacyPwdWindow ),    
        WIN_ID( MMISET_PRIVACY_PROTECT_NEW_PWD_WIN_ID ),
        WIN_TITLE( TXT_INPUT_NEW_PWD ),
        CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_PRIVACY_PROTECT_NEW_PWD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//the window for input new privacy protect password confirm
WINDOW_TABLE( MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputNewPrivacyPwdCnf ),  
        WIN_ID( MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_WIN_ID ),
        WIN_TITLE( TXT_INPUT_NEW_PWD_CNF ),
        CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for input new power on password
WINDOW_TABLE( MMISET_SET_SECURITY_NEW_PWD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputNewPwdWindow ),    
        WIN_ID( MMISET_SET_SECURITY_NEW_PWD_WIN_ID ),
        WIN_TITLE( TXT_INPUT_NEW_PWD ),
        CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_SET_SECURITY_NEW_PWD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for input new power on password confirm
WINDOW_TABLE( MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleInputNewPwdCnfWindow ),    
        WIN_ID( MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_ID ),
        WIN_TITLE( TXT_INPUT_NEW_PWD_CNF ),
        CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_SET_SECURITY_NEW_PWD_CNF_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#ifdef DPHONE_SUPPORT 
#if defined(MMI_MSD_SUPPORT)
// the window for securtiy setting msd menu
WINDOW_TABLE( MMISET_SET_SECURITY_MSD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdMenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_MSD_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_MSD_SET_OPT, MMISET_SET_SECURITY_MSD_CTRL_ID),
    END_WIN
};
#endif
//the window for securtiy setting fdn menu

#ifdef FDN_SUPPORT
WINDOW_TABLE( MMISET_SET_SECURITY_FDN_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFDNMenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_FDN_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_FDN_SET_OPT, MMISET_SET_SECURITY_FDN_CTRL_ID),
    END_WIN
};
#endif
#else
// the window for securtiy setting msd menu
#ifndef MMI_GUI_STYLE_TYPICAL 
#if defined(MMI_MSD_SUPPORT)
WINDOW_TABLE( MMISET_SET_SECURITY_MSD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdMenuWindow ),    
        WIN_ID( MMISET_SET_SECURITY_MSD_WIN_ID ),
        WIN_TITLE(TXT_MSD_MSD_FUN),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SECURITY_MSD_FORM_CTRL_ID),
      END_WIN
};
#endif

#ifdef FDN_SUPPORT

//the window for securtiy setting fdn menu
WINDOW_TABLE( MMISET_SET_SECURITY_FDN_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFDNMenuWindow ),    
        WIN_ID( MMISET_SET_SECURITY_FDN_WIN_ID ),
        WIN_TITLE(TXT_SET_FDN),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SECURITY_FDN_FORM_CTRL_ID),
        END_WIN
};
#endif
#else
#if defined(MMI_MSD_SUPPORT)
WINDOW_TABLE( MMISET_SET_SECURITY_MSD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdMenuWindow ),    
        WIN_ID( MMISET_SET_SECURITY_MSD_WIN_ID ),
        WIN_TITLE(TXT_MSD_MSD_FUN),
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
        //CREATE_MENU_CTRL(MENU_MSD_SET_OPT, MMISET_SET_SECURITY_MSD_CTRL_ID),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_MSD_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_MSD_CHILD_FORM_CTRL_ID,MMISET_SECURITY_MSD_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_MSD_LABEL_CTRL_ID,MMISET_SECURITY_MSD_CHILD_FORM_CTRL_ID),//防盗功能
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_MSD_OPENCLOSE_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_MSD_CHILD_FORM_CTRL_ID),//开启关闭
#else
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_MSD_OPENCLOSE_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_MSD_CHILD_FORM_CTRL_ID),//开启关闭
#endif        
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_MSD_MODIFY_CHILD_FORM_CTRL_ID,MMISET_SECURITY_MSD_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_MODIFY_MODIFY_PASSWORD_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_MSD_MODIFY_CHILD_FORM_CTRL_ID),//修改密码
        END_WIN
};
#endif

#ifdef FDN_SUPPORT
//the window for securtiy setting fdn menu
WINDOW_TABLE( MMISET_SET_SECURITY_FDN_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFDNMenuWindow ),    
        WIN_ID( MMISET_SET_SECURITY_FDN_WIN_ID ),
        WIN_TITLE(TXT_SET_FDN),
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_FDN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_FDN_CHILD_FORM_CTRL_ID,MMISET_SECURITY_FDN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_SET_FDN_LABEL_CTRL_ID,MMISET_SECURITY_FDN_CHILD_FORM_CTRL_ID),//FDN
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_FDN__EDIT_LABEL_CTRL_ID,MMISET_SECURITY_FDN_CHILD_FORM_CTRL_ID),//开启关闭
#else
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_FDN__EDIT_LABEL_CTRL_ID,MMISET_SECURITY_FDN_CHILD_FORM_CTRL_ID),//开启关闭
#endif

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_FDN_LIST_CHILD_FORM_CTRL_ID,MMISET_SECURITY_FDN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_FDN_LIST_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_FDN_LIST_CHILD_FORM_CTRL_ID),//LIST

        END_WIN
};
#endif


#endif 
#endif 
// the window for fdn open/close
WINDOW_TABLE( MMISET_SET_FDN_STATE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetFDNStateWindow ),    
        WIN_ID( MMISET_SET_FDN_STATE_WIN_ID ),
        WIN_TITLE( TXT_SET_FDN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_FDN_STATE_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for input old power on password when power on
WINDOW_TABLE( MMIPHONE_POWER_ON_PWD_WIN_TAB ) = 
{
    WIN_PRIO( WIN_LOWEST_LEVEL ),
    WIN_FUNC( (uint32)HandleInputPwdWindow ),    
    WIN_ID( MMISET_SET_POWER_ON_PWD_WIN_ID ),
    WIN_TITLE( TXT_INPUT_PHONE_PASSWORD ),//cr227782 maryxiao
    CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_SET_SECURITY_PWD_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),                        
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                        
#endif
    END_WIN
};

#if defined(MMI_BLACKLIST_SUPPORT)
// the window for set blacklist
WINDOW_TABLE( MMISET_SET_SECURITY_BLACKLIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleBlacklistMenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_BLACKLIST_WIN_ID ),
    WIN_TITLE( TXT_SET_BLACKLIST ),
    CREATE_MENU_CTRL(MENU_BLACKLIST_SET_OPT, MMISET_SET_BLACKLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

#ifndef MMI_GUI_STYLE_TYPICAL
//the window for set blacklist type
#if defined(MMI_BLACKLIST_SUPPORT)
WINDOW_TABLE( MMISET_SET_BLACKLIST_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleBlacklistTypeWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_BLACKLIST_SETTING),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),

    END_WIN
};
#endif
#else
#if defined(MMI_BLACKLIST_SUPPORT)
//the window for set blacklist type
WINDOW_TABLE( MMISET_SET_BLACKLIST_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleBlacklistTypeWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_BLACKLIST_SETTING),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_TYPE_AUDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID),

#ifdef VT_SUPPORT
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_TYPE_VEDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_TYPE_MESSAGE_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID,MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID),

    END_WIN
};
#endif
#endif

//the window for blacklist win
WINDOW_TABLE( MMISET_SET_BLACKLIST_LIST_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SET_BLACKLIST_LIST ),
    WIN_FUNC((uint32) HandleSetBlackListWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_LIST_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISET_SET_BLACKLIST_LIST_CTRL_ID),
    END_WIN
};

//the window for blacklist opt win
WINDOW_TABLE( MMISET_SET_BLACKLIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBlacklistOptWinMsg),
    WIN_ID(MMISET_SET_BLACKLIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BLACKLIST_OPT,MMISET_SET_BLACKLIST_LIST_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//the window for new blacklist 
WINDOW_TABLE( MMISET_SET_BLACKLIST_NEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNewBlacklistWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_NEW_WIN_ID ),
    WIN_TITLE(TXT_SET_NEW_BLACKLIST),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN),
    #else        
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_PHONE_NUM_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    END_WIN
};

//the window for new blacklist 
WINDOW_TABLE( MMISET_SET_BLACKLIST_EDIT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleEditBlacklistWindow ),    
    WIN_ID( MMISET_SET_BLACKLIST_EDIT_WIN_ID ),
    WIN_TITLE(TXT_SET_EDIT_BLACKLIST),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_PHONE_NUM_MAX_LEN,MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID),
    END_WIN
};


#ifdef MMI_PDA_SUPPORT
//the window for longpower opt win
WINDOW_TABLE( MMISET_SET_BLACKLIST_ITEM_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBlackListItmeOptWinMsg),
    WIN_ID(MMISET_BLACKLISTITEM_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BLACKLIST_ITEM_OPTION,MMISET_BLACKLIST_ITEM_OPT_CTRL_ID),
    END_WIN
};
#endif
#endif //#if defined(MMI_BLACKLIST_SUPPORT)

// the window for security setting
#ifdef DPHONE_SUPPORT 
WINDOW_TABLE( MMISET_SET_SECURITY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSecuritySetWindow ),    
    WIN_ID( MMISET_SET_SECURITY_WIN_ID ),
    WIN_TITLE( TXT_COMM_SECURITY_SETTING ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMISET_SET_SECURITY_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    END_WIN
};
#else
#ifdef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISET_SET_SECURITY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSecuritySetWindow ),    
    WIN_ID( MMISET_SET_SECURITY_WIN_ID ),
    WIN_TITLE( TXT_COMM_SECURITY_SETTING ),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_PIN_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_PIN_LABEL_CTRL_ID,MMISET_SECURITY_SET_PIN_FORM_CTRL_ID),//PIN

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_PIN2_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_MODIFYPIN2_LABEL_CTRL_ID,MMISET_SECURITY_SET_PIN2_FORM_CTRL_ID),//MODIFY PIN2

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_PHONE_LOCK_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_PHONELOCK_LABEL_CTRL_ID,MMISET_SECURITY_SET_PHONE_LOCK_FORM_CTRL_ID),//phone lock
#ifdef MMI_PDA_SUPPORT
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_PHONELOCK_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SET_PHONE_LOCK_FORM_CTRL_ID),//phone lock
#else
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_PHONELOCK_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SET_PHONE_LOCK_FORM_CTRL_ID),//phone lock
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_PHONE_MODIFYPW_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_MODIFY_PHONELOCK_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SET_PHONE_MODIFYPW_FORM_CTRL_ID),//phone lock

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_PRIVACY_LOCK_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_PRIVACY_LABEL_CTRL_ID,MMISET_SECURITY_SET_PRIVACY_LOCK_FORM_CTRL_ID),//PRIVACY PROTECTION
#ifdef MMI_PDA_SUPPORT
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_PRIVACY_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SET_PRIVACY_LOCK_FORM_CTRL_ID),//PRIVACY PROTECTION for cr 226245
#else
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_PRIVACY_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SET_PRIVACY_LOCK_FORM_CTRL_ID),//PRIVACY PROTECTION for cr 226245
#endif
#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT//自动锁屏时间移动到显示设置中
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_AUTO_KEYPADLOCK_LABEL_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
    CHILD_SETLIST_CTRL(TRUE,MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_LOCK_SCREEN_BYEND_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_LOCK_BYEND_LABEL_CTRL_ID,MMISET_SECURITY_SET_LOCK_SCREEN_BYEND_FORM_CTRL_ID),//LOCK SCREEN BY END KEY
    CHILD_SETLIST_CTRL(TRUE,MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID,MMISET_SECURITY_SET_LOCK_SCREEN_BYEND_FORM_CTRL_ID),//LOCK SCREEN BY END KEY
 
#if defined(MMI_MSD_SUPPORT)
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_GUARDLOCK_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_GUARDLOCK_LABEL_CTRL_ID,MMISET_SECURITY_SET_GUARDLOCK_FORM_CTRL_ID),//GUARDLOCK
#endif

#ifdef FDN_SUPPORT
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_FDN_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_FDN_LABEL_CTRL_ID,MMISET_SECURITY_SET_FDN_FORM_CTRL_ID),//FDN
#endif

#ifdef CMCC_UI_STYLE
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_UPLMN_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_UPLMN_LABEL_CTRL_ID,MMISET_SECURITY_SET_UPLMN_FORM_CTRL_ID),//FDN
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_BLACKLIST_FORM_CTRL_ID,MMISET_SECURITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SECURITY_BLACKLIST_LABEL_CTRL_ID,MMISET_SECURITY_SET_BLACKLIST_FORM_CTRL_ID),//FDN
#endif
    END_WIN
};

#else
WINDOW_TABLE( MMISET_SET_SECURITY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSecuritySetWindow ),    
    WIN_ID( MMISET_SET_SECURITY_WIN_ID ),
    WIN_TITLE( TXT_COMM_SECURITY_SETTING ),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SECURITY_FORM_CTRL_ID),
    END_WIN
};
#endif /* MMI_GUI_STYLE_TYPICAL */
#endif 
// the window for securtiy setting pin1 menu
WINDOW_TABLE( MMISET_SET_SECURITY_PIN1_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePin1MenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_PIN1_WIN_ID ),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
#ifdef DPHONE_SUPPORT 
    CREATE_MENU_CTRL(MENU_SET_PIN1_OPT, MMISET_SET_SECURITY_PIN1_CTRL_ID),
#else	
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_SECURITY_PIN1_CTRL_ID),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_PIN1_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_PIN1_CHILD_FORM_CTRL_ID,MMISET_PIN1_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_PIN1_LABEL_CTRL_ID,MMISET_PIN1_CHILD_FORM_CTRL_ID),//PIN1
    #ifndef MMI_GUI_STYLE_TYPICAL 
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_PIN1_EDIT_LABEL_CTRL_ID,MMISET_PIN1_CHILD_FORM_CTRL_ID),//PIN1
    #else
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_PIN1_EDIT_LABEL_CTRL_ID,MMISET_PIN1_CHILD_FORM_CTRL_ID),//PIN1
    #endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_MODIFY_PIN1_CHILD_FORM_CTRL_ID,MMISET_PIN1_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_MODIFY_PIN1_EDIT_LABEL_CTRL_ID,MMISET_MODIFY_PIN1_CHILD_FORM_CTRL_ID),//PIN
#endif
#endif 
    END_WIN
};

// the window for enable pin1
WINDOW_TABLE( MMISET_SET_ENABLE_PIN1_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleEnablePin1Window ),    
    WIN_ID( MMISET_SET_ENABLE_PIN1_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MSGBOX_CTRL(GUIMSGBOX_TEXT_VCENTER|GUIMSGBOX_TEXT_HCENTER|GUIMSGBOX_STYLE_CUSTOMED,MMISET_SET_ENABLE_PIN1_MSGBOX_CTRL_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for modify pin1
WINDOW_TABLE( MMISET_SET_MODIFY_PIN1_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleModifyPin1Window ),    
    WIN_ID( MMISET_SET_MODIFY_PIN1_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MSGBOX_CTRL(GUIMSGBOX_TEXT_VCENTER|GUIMSGBOX_TEXT_HCENTER|GUIMSGBOX_STYLE_CUSTOMED,MMISET_SET_MODIFY_PIN1_MSGBOX_CTRL_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT ),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for disable pin1
WINDOW_TABLE( MMISET_SET_DISABLE_PIN1_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleDisablePin1Window ),    
    WIN_ID( MMISET_SET_DISABLE_PIN1_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MSGBOX_CTRL(GUIMSGBOX_TEXT_VCENTER|GUIMSGBOX_TEXT_HCENTER|GUIMSGBOX_STYLE_CUSTOMED,MMISET_SET_DISABLE_PIN1_MSGBOX_CTRL_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT ),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for securtiy setting pin2 menu
/*WINDOW_TABLE( MMISET_SET_SECURITY_PIN2_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePin2MenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_PIN2_WIN_ID ),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SET_PIN2_OPT, MMISET_SET_SECURITY_PIN2_CTRL_ID),
    END_WIN
};*/

// the window for modify pin2
WINDOW_TABLE( MMISET_SET_SECURITY_PIN2_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleModifyPin2Window ),    
    WIN_ID( MMISET_SET_MODIFY_PIN2_WIN_ID ),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MSGBOX_CTRL(GUIMSGBOX_TEXT_VCENTER|GUIMSGBOX_TEXT_HCENTER|GUIMSGBOX_STYLE_CUSTOMED,MMISET_SET_MODIFY_PIN2_MSGBOX_CTRL_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT ),
    END_WIN
};

// the window for set auto board key
WINDOW_TABLE( MMISET_SET_RED_KEYLOCK_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetRedKeyLockWindow ),    
        WIN_ID( MMISET_SET_RED_KEYLOCK_WIN_ID ),
        WIN_TITLE( TXT_SET_RED_KEYLOCK ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_RED_KEYLOCK_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#ifdef MMI_SIM_LOCK_SUPPORT
/*		//Sam.hua	use SelectSimFunc 
//显示使用SIM卡拨出的选项                                                
WINDOW_TABLE( MMISET_SIMLOCK_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSIMLockSIMSelectWinMsg),    
        WIN_ID(MMISET_SET_SECURITY_SIM_LOCK_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SIM_SEL),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SECURITY_SIM_LOCK_SELECT_SIM_CTRL_ID),
        END_WIN
};
*/
//the window for securtiy setting fdn menu
WINDOW_TABLE( MMISET_SIM_LOCK_SETTING_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSIMLockSettingMenuWindow ),    
        WIN_ID( MMISET_SET_SECURITY_SIM_LOCK_WIN_ID ),
        WIN_TITLE(TXT_SET_SIM_LOCK),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SECURITY_SIMLOCK_MENU_FORM_CTRL_ID),
        END_WIN
};

#ifndef MMI_GUI_STYLE_TYPICAL 
//the window for SIM Lock Switch menu
WINDOW_TABLE( MMISET_SIM_LOCK_SWITCH_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32) HandleSIMLockMenuWindow),   
    WIN_ID( MMISET_SET_SECURITY_SIM_LOCK_ACTION_WIN_ID),
    WIN_TITLE( TXT_SET_SIM_LOCK_SWITCH ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#else
// the window for SIM Lock setting  menu
WINDOW_TABLE( MMISET_SIM_LOCK_SWITCH_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSIMLockMenuWindow ),    
    WIN_ID( MMISET_SET_SECURITY_SIM_LOCK_ACTION_WIN_ID ),
    WIN_TITLE(TXT_SET_SIM_LOCK),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
#ifdef DPHONE_SUPPORT 
    CREATE_MENU_CTRL(MENU_SET_SECURITY_SIMLOCK, MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
#else	
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
    //network
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SIM_LOCK_NETWORK_FORM_CTRL_ID,MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_SIM_LOCK_NETWORK_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_NETWORK_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_SIM_LOCK_NETWORK_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_NETWORK_FORM_CTRL_ID),
    //network subset
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_FORM_CTRL_ID,MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_FORM_CTRL_ID),
    //Service Provider
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SIM_LOCK_SP_FORM_CTRL_ID,MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_SIM_LOCK_SP_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_SP_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_SIM_LOCK_SP_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_SP_FORM_CTRL_ID),
    //corporate
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SIM_LOCK_CORPORATE_FORM_CTRL_ID,MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_SIM_LOCK_CORPORATE_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_CORPORATE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_SIM_LOCK_CORPORATE_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_CORPORATE_FORM_CTRL_ID),
    //user
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SIM_LOCK_USER_FORM_CTRL_ID,MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_SIM_LOCK_USER_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_USER_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_SECURITY_SIM_LOCK_USER_EDIT_LABEL_CTRL_ID,MMISET_SECURITY_SIM_LOCK_USER_FORM_CTRL_ID),
#endif 
    END_WIN
};
#endif

WINDOW_TABLE(MMISET_SHOW_SIM_LOCK_INFO_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleShowSIMLockInfoMsg),
    WIN_ID(MMISET_SET_SECURITY_SIM_LOCK_INFO_WIN_ID),
    WIN_TITLE(TXT_SET_SIM_LOCK_INFO),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif    
    CREATE_TEXT_CTRL(MMISET_SECURITY_SIM_LOCK_INFO_FORM_CTRL_ID),
    END_WIN
};

#endif
#if !defined (MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT) && !defined(MMI_GUI_STYLE_TYPICAL)
// the window for set auto board key
WINDOW_TABLE( MMISET_SET_AUTOKEYLOCK_WIN_TAB ) = 
{
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetAutoKeyLockWindow ),    
        WIN_ID( MMISET_SET_AUTOKEYLOCK_WIN_ID ),
#ifdef TOUCH_PANEL_SUPPORT
        WIN_TITLE(TXT_SET_AUTO_LOCK_SCREEN),
#else
        WIN_TITLE(TXT_SET_AUTO_BOADRKEY),
#endif
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_AUTOKEYLOCK_ITEM_CTRL_ID),        
        END_WIN
};
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
WINDOW_TABLE(MMIKC_PSW_INPUT_WINTAB) =
{
    WIN_FUNC((uint32)MMIAPIKL_HandleKLPswInputWinMsg),
//    WIN_HIDE_STATUS,
    WIN_ID(MMIKL_PSW_INPUT_WIN_ID),
//    WIN_STYLE( WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG ),
//    CREATE_ANIM_CTRL(MMIKL_MAIN_ANIM_BG_ID,MMIKL_CLOCK_DISP_WIN_ID),
//    WIN_SOFTKEY(TXT_IDLE_UNLOCK,TXT_NULL,TXT_NULL),
    END_WIN
};

WINDOW_TABLE( MMISET_KL_PSW_SETTING_WIN_TAB ) = 
{
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleKLPswSettingWinMsg),    
        WIN_ID( MMIKL_PSW_SETTING_WIN_ID ),
        WIN_TITLE( TXT_SET_KL_PSW_LOCK ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SECURITY_KL_PSW_LIST_CTRL_ID),    
        END_WIN
};

typedef void (*KL_PSW_INPUT_FUNC_T)(void);

LOCAL const uint32 s_kl_psw_func_arr[2] = 
{
    (uint32)OpenKLPswSettingSwitchOffWin,
    (uint32)OpenKLPswSettingChangeWin
};

#endif

#endif

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : blacklist select contact callback
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void AddBlackListSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_BLACKLIST_NEW_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);      
}

/*****************************************************************************/
// 	Description : edit blacklist select contact callback
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void EditBlackListSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_BLACKLIST_EDIT_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);      
}

/*****************************************************************************/
// 	Description : set blacklist select contact callback
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void SetBlackListSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);      
}
#endif

/*****************************************************************************/
// 	Description : modify pin prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyPin1Window(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        // 开启窗口定时器
        break;
        
    case MSG_GET_FOCUS:
//        MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : modify pin2 prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyPin2Window(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    GUI_RECT_T          image_rect = MMI_PROMPT_IMAGE_RECT,text_rect = MMI_PROMPT_TEXT_RECT;
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_STRING_T        msg_str = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        // 开启窗口定时器
        if (g_mmiset_3seconds_timer_id != 0)
        {
            MMK_StopTimer(g_mmiset_3seconds_timer_id);
            g_mmiset_3seconds_timer_id = 0;
        }
        g_mmiset_3seconds_timer_id = MMK_CreateTimer(MMI_3SECONDS, FALSE);
        MMI_GetLabelTextByLang(TXT_SUCCESS,&msg_str);
        
        GUIMSGBOX_SetParam(MMISET_SET_MODIFY_PIN2_MSGBOX_CTRL_ID,
            0,
            GUIMSGBOX_IMAGE_BMP,
            IMAGE_PUBWIN_SUCCESS,
            0,
            &image_rect,
            &msg_str,
            &text_rect,
            TXT_NULL,
            STXT_EXIT);
        
        MMK_SetAtvCtrl(win_id, MMISET_SET_MODIFY_PIN2_MSGBOX_CTRL_ID);
        break;
        
    case MSG_GET_FOCUS:
//        MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_TIMER:
        if (g_mmiset_3seconds_timer_id == *((uint8 *) param))
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
// 	Description : disable pin prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDisablePin1Window(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_GET_FOCUS:
//        MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
// 	Description : eanble pin prompt window
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnablePin1Window(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    /*// 开启窗口定时器
    g_mmiset_3seconds_timer_id = MMK_CreateTimer(MMI_3SECONDS, FALSE);
    
      MMI_GetLabelTextByLang(TXT_SUCCESS,&msg_str);
      
        GUIMSGBOX_SetParam(MMISET_SET_ENABLE_PIN1_MSGBOX_CTRL_ID,
        0,
        GUIMSGBOX_IMAGE_BMP,
        IMAGE_PROMPT_ICON_SUCCESS,
        &image_rect,
        &msg_str,
        &text_rect,
        TXT_NULL,
        STXT_EXIT);
        
          MMK_SetAtvCtrl(win_id, MMISET_SET_ENABLE_PIN1_MSGBOX_CTRL_ID);
        */		
        break;
        
    case MSG_GET_FOCUS:
//        MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
        break;
        
        /*   case MSG_TIMER:
        if (g_mmiset_3seconds_timer_id == *((uint8 *) param))
        {
        MMK_CloseWin(win_id);
        }
        break;
        */
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : to handle security setting pin2 menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
/*LOCAL MMI_RESULT_E  HandlePin2MenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_MENU_GROUP_ID_T          group_id = 0;
    MMI_MENU_ID_T				 menu_id = 0;
    MMI_RESULT_E				recode = MMI_RESULT_TRUE;
    uint32                       dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_SET_SECURITY_PIN2_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_SET_SECURITY_PIN2_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_PIN_MODIFY:
            if (MMIAPIPHONE_IsSimCardOk(dual_sys))
            {
                MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
                MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_CHANGE,MNSIM_PIN2,(uint32)MMISET_SET_MODIFY_PIN2_WIN_TAB, TRUE);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}*/

/*****************************************************************************/
// 	Description : to INITIAL SECURITY CONTROL
//	Global resource dependence : 
//  Author:MARYXIAO
//	Note: 
/*****************************************************************************/
LOCAL void InitSecurityLabel(void)
{
    MMI_STRING_T label_name  = {0};

    GUIFORM_SetStyle(MMISET_SECURITY_SET_PIN_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_PIN1, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_PIN_LABEL_CTRL_ID, &label_name, FALSE);

    GUIFORM_SetStyle(MMISET_SECURITY_SET_PIN2_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SERCURITY_MODIFY_PIN2, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_MODIFYPIN2_LABEL_CTRL_ID, &label_name, FALSE);

    GUIFORM_SetStyle(MMISET_SECURITY_SET_PHONE_MODIFYPW_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SERCURITY_MODIFY_PHONE_PASSWORD, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_MODIFY_PHONELOCK_EDIT_LABEL_CTRL_ID, &label_name, FALSE);

#if defined(MMI_MSD_SUPPORT)
    GUIFORM_SetStyle(MMISET_SECURITY_SET_GUARDLOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_MSD_MSD_FUN, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_GUARDLOCK_LABEL_CTRL_ID, &label_name, FALSE);
#endif

#ifdef FDN_SUPPORT
    GUIFORM_SetStyle(MMISET_SECURITY_SET_FDN_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_FDN, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_FDN_LABEL_CTRL_ID, &label_name, FALSE);
#endif

#ifdef CMCC_UI_STYLE
    GUIFORM_SetStyle(MMISET_SECURITY_SET_UPLMN_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_COMM_UPLMN_SETTING, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_UPLMN_LABEL_CTRL_ID, &label_name, FALSE);
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    GUIFORM_SetStyle(MMISET_SECURITY_SET_BLACKLIST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_BLACKLIST, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_BLACKLIST_LABEL_CTRL_ID, &label_name, FALSE);    
#endif
}
/*****************************************************************************/
// 	Description : to initial privacy protection list
//	Global resource dependence : 
//  Author:maryxiao
//	Note: 
/*****************************************************************************/
LOCAL void SetPrivacyProtectionList(void)
{

 	MMI_STRING_T label_name  = {0};

        GUIFORM_SetStyle(MMISET_SECURITY_SET_PRIVACY_LOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	MMI_GetLabelTextByLang(TXT_SET_SET_PRIVACY_PROTECT, &label_name);
	GUILABEL_SetText(MMISET_SECURITY_PRIVACY_LABEL_CTRL_ID, &label_name, FALSE);

    if(GetPrivacyProtecdItemNum()>0)
    {
	    MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
	    GUILABEL_SetText(MMISET_SECURITY_PRIVACY_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }
    else
    {
	    MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
	    GUILABEL_SetText(MMISET_SECURITY_PRIVACY_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }
}
/*****************************************************************************/
// 	Description : to initial phone lock list
//	Global resource dependence : 
//  Author:maryxiao
//	Note: 
/*****************************************************************************/
LOCAL void SetPhoneLockList(void)
{
	MMI_STRING_T label_name  = {0};

        GUIFORM_SetStyle(MMISET_SECURITY_SET_PHONE_LOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	MMI_GetLabelTextByLang(TXT_SERCURITY_PHONE_LOC, &label_name);
	GUILABEL_SetText(MMISET_SECURITY_PHONELOCK_LABEL_CTRL_ID, &label_name, FALSE);

 


    if(MMIAPISET_GetPowerOnPwdStatus())
    {
	    MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
	    GUILABEL_SetText(MMISET_SECURITY_PHONELOCK_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }
    else
    {
	    MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
	    GUILABEL_SetText(MMISET_SECURITY_PHONELOCK_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }
          
}

/*****************************************************************************/
// 	Description : to initial auto lock keypad list
//	Global resource dependence : 
//  Author:maryxiao
//	Note: 
/*****************************************************************************/
LOCAL void SetAutoLockKeyPadList(void)
{
	uint32 loop = 0;
	uint16 cur_selection = 0;
	MMI_STRING_T label_name  = {0};

	uint32 text_id[]=
	{
                TXT_CLOSE,
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
#else
                TXT_SET_TIME_5S,
#endif
                TXT_SET_TIME_15S,
                TXT_SET_TIME_30S,
                TXT_COMM_1_MINUTE,
                TXT_COMM_5_MINUTES
	};
	
    GUIFORM_SetStyle(MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUISETLIST_SetFontAlign(MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);

    #ifdef TOUCH_PANEL_SUPPORT
    MMI_GetLabelTextByLang(TXT_SET_AUTO_LOCK_SCREEN, &label_name);
    #else
    MMI_GetLabelTextByLang(TXT_SET_AUTO_BOADRKEY, &label_name);
    #endif
    GUILABEL_SetText(MMISET_SECURITY_AUTO_KEYPADLOCK_LABEL_CTRL_ID, &label_name, FALSE);
	for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
	    GUISETLIST_AddItemById(MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID, text_id[loop]);
	}

    switch (MMIAPISET_GetAutoBoardKey())
    {
    case MMISET_KEYLOCK_CLOSE:
        cur_selection = (uint16)MMISET_KEYLOCK_SELECT_CLOSE;
        break;
    
#if !defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
    case MMISET_KEYLOCK_5_SEC:
        cur_selection = (uint16)MMISET_KEYLOCK_SELECT_5_SEC;
        break;
#endif            
    case MMISET_KEYLOCK_15_SEC:
        cur_selection = (uint16)MMISET_KEYLOCK_SELECT_15_SEC;
        break;
    
    case MMISET_KEYLOCK_30_SEC:
        cur_selection = (uint16)MMISET_KEYLOCK_SELECT_30_SEC;
        break;
    
    case MMISET_KEYLOCK_1_MIN:
        cur_selection = (uint16)MMISET_KEYLOCK_SELECT_1_MIN;
        break;
        
    case MMISET_KEYLOCK_5_MIN:
        cur_selection = (uint16)MMISET_KEYLOCK_SELECT_5_MIN;
        break;
    default:
        SCI_TRACE_LOW("AppendAutoKeylockTimeListItem:keylock time is error!");
        break;
    }
    #ifdef TOUCH_PANEL_SUPPORT
    GUISETLIST_SetTitleTextId(MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID,TXT_SET_AUTO_LOCK_SCREEN);
    #else
    GUISETLIST_SetTitleTextId(MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID,TXT_SET_AUTO_BOADRKEY);
    #endif
    GUISETLIST_SetCurIndex(MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//forcr226574

}

/*****************************************************************************/
// 	Description : to initial auto lock keypad list
//	Global resource dependence : 
//  Author:maryxiao
//	Note: 
/*****************************************************************************/
LOCAL void SetLockScreenByEndKeyList(void)
{
	uint32 loop = 0;
	uint16 cur_selection = 0;
	MMI_STRING_T label_name  = {0};

	uint32 text_id[2]=
	{
    	TXT_COMMON_ENABLE_ON,
    	TXT_CLOSE,
	};
	
    GUIFORM_SetStyle(MMISET_SECURITY_SET_LOCK_SCREEN_BYEND_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	MMI_GetLabelTextByLang(TXT_SET_RED_KEYLOCK, &label_name);
	GUILABEL_SetText(MMISET_SECURITY_LOCK_BYEND_LABEL_CTRL_ID, &label_name, FALSE);
//	GUISETLIST_SetFontAlign(MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID, ALIGN_LVMIDDLE);
	for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
		GUISETLIST_AddItemById(MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID, text_id[loop]);
	}
    if(MMIAPISET_GetRedKeyLock())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    GUISETLIST_SetTitleTextId(MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID,TXT_SET_RED_KEYLOCK);
    GUISETLIST_SetCurIndex(MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//forcr226574
}


/*****************************************************************************/
// 	Description : to save autolock keypad
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/
LOCAL void SaveAutoLockKeyPadList(void)
{
    uint16                       cur_selection          =   0;
    MMISET_KEYLOCK_TYPE_E        auto_boardkey_time     =   MMISET_KEYLOCK_CLOSE;
    
    SCI_MEMSET(&auto_boardkey_time, 0, sizeof(MMISET_KEYLOCK_TYPE_E));
    
    //get the selected item
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID);
    
    if(MMISET_KEYLOCK_MAX_ITEM <= cur_selection)
    {
        //SCI_TRACE_LOW:"mmiset cur_selection = %d exceed MMISET_KEYLOCK_MAX_ITEM "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_1955_112_2_18_2_51_54_310,(uint8*)"d",cur_selection);
        return;
    }
    switch (cur_selection)
    {
    case MMISET_KEYLOCK_SELECT_CLOSE:
        auto_boardkey_time = MMISET_KEYLOCK_CLOSE;
        break;
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
#else
    case MMISET_KEYLOCK_SELECT_5_SEC:
        auto_boardkey_time = MMISET_KEYLOCK_5_SEC;
        break;
#endif        
        
    case MMISET_KEYLOCK_SELECT_15_SEC:
        auto_boardkey_time = MMISET_KEYLOCK_15_SEC;
        break;
        
    case MMISET_KEYLOCK_SELECT_30_SEC:
        auto_boardkey_time = MMISET_KEYLOCK_30_SEC;
        break;
        
    case MMISET_KEYLOCK_SELECT_1_MIN:
        auto_boardkey_time = MMISET_KEYLOCK_1_MIN;
        break;
        
    case MMISET_KEYLOCK_SELECT_5_MIN:
        auto_boardkey_time = MMISET_KEYLOCK_5_MIN;
        break;
    default:
        break;
    }
    MMISET_SetAutoBoardKey(auto_boardkey_time);    
}
/*****************************************************************************/
// 	Description : set power on password
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
/*PUBLIC void MMIAPISET_SetPowerOnPwdStatus(BOOLEAN is_open)
{
    MMISET_POWER_ON_PWD_T   phone_code = {0};
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    SCI_MEMSET(&phone_code,0,sizeof(MMISET_POWER_ON_PWD_T));
    
    //init power on password
    MMINV_READ(MMINV_SET_POWER_ON_PWD_INFO,&phone_code,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&phone_code,0,sizeof(MMISET_POWER_ON_PWD_T));
        MMINV_WRITE(MMINV_SET_POWER_ON_PWD_INFO,&phone_code);
    }
    phone_code.is_enable = is_open;
    MMINV_WRITE(MMINV_SET_POWER_ON_PWD_INFO,&phone_code);

}*/


/*****************************************************************************/
// 	Description : to save LockScreenByEndKey
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/
LOCAL void SaveLockScreenByEndKeyList(void)
{
    uint16                       cur_selection          =   0;
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID);
    if(cur_selection == 0)
    {
        MMIAPISET_SetRedKeyLock(TRUE);
    }
    else
    {
        MMIAPISET_SetRedKeyLock(FALSE);
    }
}


/*****************************************************************************/
// 	Description : to handle security setting list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
#ifdef DPHONE_SUPPORT 
LOCAL MMI_RESULT_E  HandleSecuritySetWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_SELECT_TYPE_E    select_type = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint32 sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,MMISET_SET_SECURITY_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        AppendSecuritySetListItem(MMISET_SET_SECURITY_CTRL_ID);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //get the selected item
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_SECURITY_CTRL_ID);
        switch (cur_selection)
        {
        case MMISET_SECURITY_PIN_CODE:
            {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint16 dual_sys = MN_DUAL_SYS_1;
                uint32 set_pin_num = MMIAPIPHONE_GetPermitSetPin1Num(&dual_sys,1);
                
                select_type = MMISET_SET_PIN;
                
                if (set_pin_num > 1)
                {
                    MMIAPISET_OpenUssdSelectWin(select_type);
                }
                else
                {
                    MMISET_OpenPinWin((MN_DUAL_SYS_E)dual_sys);
                }            
#else
                MMISET_OpenPinWin(MN_DUAL_SYS_1);
#endif
            }
            break;
            
        case MMISET_SECURITY_PIN2_CODE:     
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            select_type = MMISET_SET_PIN2_MODIFY;
            
            //判断是否存在两张卡同时开启，不是则启动active sim
            if (sim_ok_num > 1)
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            } 
            else
            {
                MMISET_OpenPin2Win((MN_DUAL_SYS_E)sim_ok);
            }
#else
            MMISET_OpenPin2Win(MN_DUAL_SYS_1);
#endif
            break;
            
        case MMISET_SECURITY_PRIVACY:
            MMK_CreatePubEditWin((uint32*)MMISET_SET_PRIVACY_PROTECT_WIN_TAB,PNULL);
            break;
            
#ifndef LCD_SLIDE_SUPPORT
#ifdef DPHONE_SUPPORT 
#else
        case MMISET_SECURITY_KEYLOCK:
            MMK_CreateWin((uint32*)MMISET_SET_AUTO_BAORADKEY_WIN_TAB,PNULL);
            break;
#endif 
        case MMISET_SECURITY_RED_KEYLOCK:
            MMK_CreateWin((uint32*)MMISET_SET_RED_KEYLOCK_WIN_TAB,PNULL);
            break;
#endif

        case MMISET_SECURITY_POWER_ON_CODE:
            if (MMISET_IsSetPowerOnPwd())//?ü??ê?·?ò??-éè??
            {
                MMK_CreateWin((uint32*)MMISET_SET_SECURITY_PWD_MENU_WIN_TAB,PNULL);
            }
            else
            {
                g_mmiset_pwd_type = MMISET_PWD_SET;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);
            }
            break;
            
#if defined(MMI_MSD_SUPPORT)
        case MMISET_SECURITY_MSD:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (sim_ok_num > 0)
            {
                MMK_CreateWin((uint32*)MMISET_SET_SECURITY_MSD_WIN_TAB,PNULL);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
#else
            if (MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1))
            {
                MMK_CreateWin((uint32*)MMISET_SET_SECURITY_MSD_WIN_TAB,PNULL);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
#endif
            break;
#endif

#ifdef FDN_SUPPORT

        case MMISET_SECURITY_FDN:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            select_type = MMISET_SET_FDN;
            
            //?D??ê?·?′??úá????¨í?ê±?a??￡?2?ê??ò???ˉactive sim
             if (sim_ok_num > 1)
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
            else
            {
                MMISET_OpenFDNWin((MN_DUAL_SYS_E)sim_ok);
            }
#else
            MMISET_OpenFDNWin(MN_DUAL_SYS_1);
#endif
            break;
#endif

 //MS00216980 cheney
#ifdef ENGTD_SUPPORT
    case MMISET_SECURITY_UPLMN:
        MMIENGTD_OpenUPLMNSettingWin();
        break;
#endif 

#ifdef CMCC_UI_STYLE
    case MMISET_SECURITY_UPLMN_LABEL_CTRL_ID:
#ifdef ENGTD_SUPPORT
        MMIENGTD_OpenUPLMNSettingWin();
#endif
        break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
        case MMISET_BLACKLIST:
            if(MMISET_InitBlacklistParam())
            {
                MMISET_GetBlackList();
                MMK_CreateWin((uint32*)MMISET_SET_SECURITY_BLACKLIST_WIN_TAB,PNULL);
            }
            break;
#endif

        default:
            SCI_TRACE_LOW("HandleSecuritySetWindow: cur_selection %d is error!",cur_selection);
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#else

#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
/*****************************************************************************/
//  Description : handle auto key lock set
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoKeyLockWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_AUTOKEYLOCK_ITEM_CTRL_ID;
    uint16 index = 0;
    MMISET_KEYLOCK_TYPE_E auto_boardkey_time = MMISET_KEYLOCK_CLOSE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaSecurityAutoKeyLockListCtrl();        
        switch (MMIAPISET_GetAutoBoardKey())
        {
        case MMISET_KEYLOCK_CLOSE:
            index = MMISET_KEYLOCK_SELECT_CLOSE;
            break;
#if defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
#else
        case MMISET_KEYLOCK_5_SEC:
            index = MMISET_KEYLOCK_SELECT_5_SEC;
            break;
#endif            
        case MMISET_KEYLOCK_15_SEC:
            index = MMISET_KEYLOCK_SELECT_15_SEC;
            break;
            
        case MMISET_KEYLOCK_30_SEC:
            index = MMISET_KEYLOCK_SELECT_30_SEC;
            break;
            
        case MMISET_KEYLOCK_1_MIN:
            index = MMISET_KEYLOCK_SELECT_1_MIN;
            break;
            
        case MMISET_KEYLOCK_5_MIN:
            index = MMISET_KEYLOCK_SELECT_5_MIN;
            break;
            
        default:
            SCI_TRACE_LOW("AppendAutoKeylockTimeListItem:keylock time is error!");
            break;
        }
        GUILIST_SetSelectedItem( ctrl_id, index, TRUE );
        GUILIST_SetCurItemIndex(ctrl_id, index);
        
        MMK_SetAtvCtrl(win_id,MMISET_AUTOKEYLOCK_ITEM_CTRL_ID);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        index = GUILIST_GetCurItemIndex(ctrl_id);
        switch(index)
        {
        case MMISET_KEYLOCK_SELECT_CLOSE:
            auto_boardkey_time = MMISET_KEYLOCK_CLOSE;
            break;
#if defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
#else
        case MMISET_KEYLOCK_SELECT_5_SEC:
            auto_boardkey_time = MMISET_KEYLOCK_5_SEC;
            break;
#endif            
            
        case MMISET_KEYLOCK_SELECT_15_SEC:
            auto_boardkey_time = MMISET_KEYLOCK_15_SEC;
            break;
            
        case MMISET_KEYLOCK_SELECT_30_SEC:
            auto_boardkey_time = MMISET_KEYLOCK_30_SEC;
            break;
            
        case MMISET_KEYLOCK_SELECT_1_MIN:
            auto_boardkey_time = MMISET_KEYLOCK_1_MIN;
            break;
            
        case MMISET_KEYLOCK_SELECT_5_MIN:
            auto_boardkey_time = MMISET_KEYLOCK_5_MIN;
            break;
        default:
            break;
        }
        
    MMISET_SetAutoBoardKey(auto_boardkey_time);
    /* Modify by michael on 3/23/2012 for NEWMS00183957 : 设置完timer后立即使其生效 */
    MMIDEFAULT_StartSettingLCDBackLightTimer();
    /************************Modify over*************************/
    MMK_CloseWin( win_id );
    break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
}

#endif

/*****************************************************************************/
//  Description : append security settings radio line settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaSecuritySettingsRadioLine(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : append settings 2 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaSecuritySettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data, //用户数据
                                 BOOLEAN is_need_replace
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[MMISECURITY_LIST_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_len = 0;
    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        MMIAPICOM_Wstrncpy(temp_wstr,
                                          text_str.wstr_ptr,
                                          text_str.wstr_len);
        temp_len = text_str.wstr_len;
    }
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    }
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }
    
    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;

   //softkey
#ifndef MMI_PDA_SUPPORT
    if(image_id == IMAGE_CHECK_SELECTED_ICON)
        {
            item_data.softkey_id[0] = STXT_CANCEL;
            item_data.softkey_id[2] = STXT_RETURN;
        }
    else
        {
            item_data.softkey_id[0] = STXT_SELECT;
            item_data.softkey_id[2] = STXT_RETURN;            
        }
#endif

    if(is_need_replace)
    {
        uint16 index = 0;
        index = GUILIST_GetCurItemIndex(ctrl_id); 
        GUILIST_ReplaceItem(ctrl_id,&item_t,index);      
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }    
    
}

/*****************************************************************************/
// 	Description : init auto key lock  list
//	Global resource dependence : 
//  Author:rong.gu
//	Note: 
/*****************************************************************************/
LOCAL void  InitPdaSecurityAutoKeyLockListCtrl(void)
{
    uint8 i = 0;
    MMI_CTRL_ID_T       ctrl_id = MMISET_AUTOKEYLOCK_ITEM_CTRL_ID;
    uint32 text_arr_id[]=
    {
    	TXT_CLOSE,
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
#else
        TXT_SET_TIME_5S,
#endif        
        TXT_SET_TIME_15S,
        TXT_SET_TIME_30S,
    	TXT_COMM_1_MINUTE,
        TXT_COMM_5_MINUTES
    };
    uint32 text_arr_size = 0;
    
    text_arr_size = sizeof(text_arr_id) / sizeof(uint32);
    GUILIST_SetMaxItem(ctrl_id, text_arr_size, FALSE );   

    
    for(i = 0; i < text_arr_size; i++)
    {
    AppendPdaSecuritySettingsRadioLine(            
    ctrl_id,
    text_arr_id[i],
    (uint32)0);
    }
//end    
}

/*****************************************************************************/
// 	Description : init security settings list
//	Global resource dependence : 
//  Author:rong.gu
//	Note: 
/*****************************************************************************/
LOCAL void  InitPdaSecuritySettingsListCtrl(BOOLEAN is_need_replace)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMISET_SECURITY_FORM_CTRL_ID;
    MMI_STRING_T  text_str = {0};
#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
    MMI_TEXT_ID_T  text_id = TXT_NULL;

     uint32 text_arr_id[]=
     {
        TXT_CLOSE,
#if defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
#else
        TXT_SET_TIME_5S,
#endif
        TXT_SET_TIME_15S,
        TXT_SET_TIME_30S,
        TXT_COMM_1_MINUTE,
        TXT_COMM_5_MINUTES
     };
#endif

    uint16 index = 0;
    uint16 focus_index = 0;

    if(is_need_replace)
    { 
        focus_index = GUILIST_GetCurItemIndex(ctrl_id); 
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_SECURITY_SETTINGS_MAX, FALSE );   
    }

    if(!is_need_replace)
    {
    AppendPdaSecuritySettingsItem1Line(            
    ctrl_id,
    TXT_SET_PIN1,
    (uint32)MMISMS_PDA_SECURITY_SETTINGS_PING);    
    }
    index++;

    if(!is_need_replace) 
    {
    AppendPdaSecuritySettingsItem1Line(            
    ctrl_id,
    TXT_SERCURITY_MODIFY_PIN2,
    (uint32)MMISMS_PDA_SECURITY_SETTINGS_PING2);
    }
    index++;

    if( (is_need_replace && index == focus_index) || !is_need_replace)
    {
        flag = MMIAPISET_GetPowerOnPwdStatus();
        if(flag)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }      
        AppendPdaSecuritySettingsItem2Line(            
        ctrl_id,
        TXT_SERCURITY_PHONE_LOC,
        text_str,
        image_id,
        (uint32)MMISMS_PDA_SECURITY_SETTINGS_PHONE_LOCK,
        is_need_replace);
    }
    index++;

    if(!is_need_replace)
    {
    AppendPdaSecuritySettingsItem1Line(            
    ctrl_id,
    TXT_SERCURITY_MODIFY_PHONE_PASSWORD,
    (uint32)MMISMS_PDA_SECURITY_SETTINGS_CELLPHONE_PASSWORD);
    }
    index++;

    if( (is_need_replace && index == focus_index) || !is_need_replace)
    {
        if(GetPrivacyProtecdItemNum()>0)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        } 
        AppendPdaSecuritySettingsItem2Line(            
        ctrl_id,
        TXT_SET_SET_PRIVACY_PROTECT,
        text_str,
        image_id,
        (uint32)MMISMS_PDA_SECURITY_SETTINGS_PRAVICY,
        is_need_replace);
    }
    index++; 
#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
    if((is_need_replace && index == focus_index) ||!is_need_replace)
    {
    switch (MMIAPISET_GetAutoBoardKey())
    {
    case MMISET_KEYLOCK_CLOSE:
        text_id = text_arr_id[MMISET_KEYLOCK_SELECT_CLOSE];
        break;
    
#if defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
#else        
    case MMISET_KEYLOCK_5_SEC:
        text_id = text_arr_id[MMISET_KEYLOCK_SELECT_5_SEC];
        break;
#endif    
    case MMISET_KEYLOCK_15_SEC:
        text_id = text_arr_id[MMISET_KEYLOCK_SELECT_15_SEC];
        break;
    
    case MMISET_KEYLOCK_30_SEC:
        text_id = text_arr_id[MMISET_KEYLOCK_SELECT_30_SEC];
        break;
    
    case MMISET_KEYLOCK_1_MIN:
        text_id = text_arr_id[MMISET_KEYLOCK_SELECT_1_MIN];
        break;
        
    case MMISET_KEYLOCK_5_MIN:
        text_id = text_arr_id[MMISET_KEYLOCK_SELECT_5_MIN];
        break;
        
    default:
        SCI_TRACE_LOW("AppendAutoKeylockTimeListItem:keylock time is error!");
        break;
    }

    MMI_GetLabelTextByLang(text_id , &text_str);     
    AppendPdaSecuritySettingsItem2Line(            
    ctrl_id,
#ifdef TOUCH_PANEL_SUPPORT
    TXT_SET_AUTO_LOCK_SCREEN,
#else
    TXT_SET_AUTO_BOADRKEY,
#endif    
    text_str,
#ifdef TOUCH_PANEL_SUPPORT
    IMAGE_COMMON_RIGHT_ARROW,
#else
    PNULL,
#endif
    (uint32)MMISMS_PDA_SECURITY_SETTINGS_AUTOKEYPAD,
    is_need_replace);
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    }
    index++; 
#endif
        
#ifndef MMI_PDA_SUPPORT
    if((is_need_replace && index == focus_index) || !is_need_replace)
    {
        if( MMIAPISET_GetRedKeyLock())
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        } 
        AppendPdaSecuritySettingsItem2Line(            
        ctrl_id,
        TXT_SET_RED_KEYLOCK,
        text_str,
        image_id,
        (uint32)MMISMS_PDA_SECURITY_SETTINGS_ENDKEY_LOCK,
        is_need_replace);
    }
    index++;
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
    if((is_need_replace && index == focus_index) ||!is_need_replace)
    {
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    if(MMIAPISET_IsKLPswOn())/* renyt */
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    } 
    AppendPdaSecuritySettingsItem2Line(            
    ctrl_id,
    TXT_SET_KL_PSW_LOCK,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_SECURITY_SETTINGS_KEYLOCK_PSW,
    is_need_replace);
    }
    index++;
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
#endif
#endif

#if defined(MMI_MSD_SUPPORT)
    if(!is_need_replace )
    {
        AppendPdaSecuritySettingsItem1Line(            
        ctrl_id,
        TXT_MSD_MSD_FUN,
        (uint32)MMISMS_PDA_SECURITY_SETTINGS_GAURD_LOCK);
    }
    index++;
#endif

#ifdef FDN_SUPPORT
    if(!is_need_replace )
    {
        AppendPdaSecuritySettingsItem1Line(            
        ctrl_id,
        TXT_SET_FDN,
        (uint32)MMISMS_PDA_SECURITY_SETTINGS_FIXDIAL_NUMBER);
    }
    index++;
#endif
    
#if 0
    if(!is_need_replace )
    {
        AppendPdaSecuritySettingsItem1Line(ctrl_id, TXT_COMM_UPLMN_SETTING, (uint32)MMISMS_PDA_SECURITY_SETTINGS_UPLMN);
    }
    index++;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    if(!is_need_replace )
    {
        AppendPdaSecuritySettingsItem1Line(            
        ctrl_id,
        TXT_SET_BLACKLIST,
        (uint32)MMISMS_PDA_SECURITY_SETTINGS_BLACKLIST);
    }
#endif
}

/*****************************************************************************/
// 	Description :  security settings list
//	Global resource dependence : 
//  Author:rong.gu
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecuritySetWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    //uint16          cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_SECURITY_FORM_CTRL_ID;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_SELECT_TYPE_E    select_type = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint32 sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
    uint16 dual_sys = MN_DUAL_SYS_1;
    uint32 set_pin_num = MMIAPIPHONE_GetPermitSetPin1Num(&dual_sys,1);
#endif
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:                 
        InitPdaSecuritySettingsListCtrl(FALSE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
    case MSG_SET_BLACKLIST_LIST_UPDATE:        
        InitPdaSecuritySettingsListCtrl(TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint16 index = 0;
            uint32 user_data = 0;
            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &user_data);
            switch((MMISMS_PDA_SECURITY_SETTINGS_TYPE_E)user_data)
            {
            case MMISMS_PDA_SECURITY_SETTINGS_PING:
#ifndef MMI_MULTI_SIM_SYS_SINGLE             
                
                select_type = MMISET_SET_PIN;
                
                if (set_pin_num > 1)
                {
                    MMIAPISET_OpenUssdSelectWin(select_type);
                }
                else
                {
                    MMISET_OpenPinWin((MN_DUAL_SYS_E)dual_sys);
                }            
#else
                MMISET_OpenPinWin(MN_DUAL_SYS_1);
#endif
                break;
                
            case MMISMS_PDA_SECURITY_SETTINGS_PING2:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                select_type = MMISET_SET_PIN2_MODIFY;
                
                //判断是否存在两张卡同时开启，不是则启动active sim
                if (sim_ok_num > 1)
                {
                    MMIAPISET_OpenUssdSelectWin(select_type);
                } 
                else
                {
                    MMISET_OpenPin2Win((MN_DUAL_SYS_E)sim_ok);
                }
#else
                MMISET_OpenPin2Win(MN_DUAL_SYS_1);
#endif   
                break;
            case MMISMS_PDA_SECURITY_SETTINGS_PHONE_LOCK:
                if (MMISET_IsSetPowerOnPwd())//密码是否已经设置
                {
                    if (MMIAPISET_GetPowerOnPwdStatus())
                    {
                        g_mmiset_pwd_type = MMISET_PWD_CLOSE;
                        MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
                    }
                    else
                    {
                        g_mmiset_pwd_type = MMISET_PWD_OPEN;
                        MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
                    }
                }
                else
                {
                    g_mmiset_pwd_type = MMISET_PWD_SET;
                    MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);                
                }
                break;
            case MMISMS_PDA_SECURITY_SETTINGS_CELLPHONE_PASSWORD:
                g_mmiset_pwd_type = MMISET_PWD_MODIFY;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
                break;
                
            case MMISMS_PDA_SECURITY_SETTINGS_PRAVICY:
                MMK_CreatePubEditWin((uint32*)MMISET_SET_PRIVACY_PROTECT_WIN_TAB,PNULL);//for cr 226245
                break;

#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
            case MMISMS_PDA_SECURITY_SETTINGS_AUTOKEYPAD:
                MMK_CreatePubListWin((uint32*)MMISET_SET_AUTOKEYLOCK_WIN_TAB,PNULL);
                break;
#endif
            case MMISMS_PDA_SECURITY_SETTINGS_ENDKEY_LOCK:
            {
                BOOLEAN flag = !MMIAPISET_GetRedKeyLock();
                MMIAPISET_SetRedKeyLock(flag);
                MMK_PostMsg(win_id, MSG_SET_BLACKLIST_LIST_UPDATE, PNULL, NULL);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
                break;
            }
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
                
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
            case MMISMS_PDA_SECURITY_SETTINGS_KEYLOCK_PSW:
                OpenKLPswSettingEnterWin();
                break;
#endif
#endif
                
#if defined(MMI_MSD_SUPPORT)
        case MMISMS_PDA_SECURITY_SETTINGS_GAURD_LOCK:    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (sim_ok_num > 0)
            {
                MMK_CreateWin((uint32*)MMISET_SET_SECURITY_MSD_WIN_TAB,PNULL);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
#else
            if (MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1))
            {
                MMK_CreateWin((uint32*)MMISET_SET_SECURITY_MSD_WIN_TAB,PNULL);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
#endif
            break;
#endif

            case MMISMS_PDA_SECURITY_SETTINGS_FIXDIAL_NUMBER:
#ifdef FDN_SUPPORT 
              
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                select_type = MMISET_SET_FDN;
                
                //判断是否存在两张卡同时开启，不是则启动active sim
                if (sim_ok_num > 1)
                {
                    MMIAPISET_OpenUssdSelectWin(select_type);
                }
                else
                {
                    MMISET_OpenFDNWin((MN_DUAL_SYS_E)sim_ok);
                }
#else
                MMISET_OpenFDNWin(MN_DUAL_SYS_1);
                    
#endif  
#endif                
                break;
                
#ifdef CMCC_UI_STYLE
            case MMISMS_PDA_SECURITY_SETTINGS_UPLMN:
#ifdef ENGTD_SUPPORT
                MMIENGTD_OpenUPLMNSettingWin();
#endif
                break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
            case MMISMS_PDA_SECURITY_SETTINGS_BLACKLIST:
                if(MMISET_InitBlacklistParam())
                {
                    MMISET_GetBlackList();
                    MMK_CreateWin((uint32*)MMISET_SET_SECURITY_BLACKLIST_WIN_TAB,PNULL);
                }
                break;
#endif

            default:
                break;
            }
            break;
        }
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
   }
   
   return recode;
}
#else
LOCAL MMI_RESULT_E  HandleSecuritySetWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    //uint16          cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_CALL_DUAL_SIM_LABEL_CTRL_ID;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_SELECT_TYPE_E    select_type = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint32 sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
    uint16 dual_sys = MN_DUAL_SYS_1;
    uint32 set_pin_num = MMIAPIPHONE_GetPermitSetPin1Num(&dual_sys,1);
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    #ifdef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMISET_SECURITY_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        GUISETLIST_SetFontAlign(MMISET_DISPLAY_MAINMENU_LABEL_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
    #endif
        InitSecurityLabel();
        SetPrivacyProtectionList();
        SetLockScreenByEndKeyList();
#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
        SetAutoLockKeyPadList();
#endif
        SetPhoneLockList();
        MMK_SetAtvCtrl(win_id,MMISET_SECURITY_PIN_LABEL_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        SetPhoneLockList();
        SetPrivacyProtectionList();
        break;
        
    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
#ifndef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
        if(ctrl_id == MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID)
        {
            SaveAutoLockKeyPadList();
        }
        else 
#endif
        if(ctrl_id == MMISET_SECURITY_LOCK_BYEND_SETLIST_CTRL_ID)
        {
            SaveLockScreenByEndKeyList();
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT    
    case MSG_APP_OK:
    case MSG_CTL_OK:
#endif    
        ctrl_id= MMK_GetActiveCtrlId(win_id);
		if(msg_id==MSG_CTL_PENOK)
		{
			ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
		}
        switch (ctrl_id)
        {
            case MMISET_SECURITY_PIN_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE             
            
            select_type = MMISET_SET_PIN;
            
            if (set_pin_num > 1)
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
            else
            {
                MMISET_OpenPinWin((MN_DUAL_SYS_E)dual_sys);
            }            
#else
            MMISET_OpenPinWin(MN_DUAL_SYS_1);
#endif
            break;

        case MMISET_SECURITY_MODIFYPIN2_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        select_type = MMISET_SET_PIN2_MODIFY;
        
        //判断是否存在两张卡同时开启，不是则启动active sim
        if (sim_ok_num > 1)
        {
            MMIAPISET_OpenUssdSelectWin(select_type);
        } 
        else
        {
            MMISET_OpenPin2Win((MN_DUAL_SYS_E)sim_ok);
        }
#else
        MMISET_OpenPin2Win(MN_DUAL_SYS_1);
#endif                
        break;

    case MMISET_SECURITY_PHONELOCK_EDIT_LABEL_CTRL_ID:
        if (MMISET_IsSetPowerOnPwd())//密码是否已经设置
        {
            if (MMIAPISET_GetPowerOnPwdStatus())
            {
                g_mmiset_pwd_type = MMISET_PWD_CLOSE;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
            }
            else
            {
                g_mmiset_pwd_type = MMISET_PWD_OPEN;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
            }
        }
        else
        {
            g_mmiset_pwd_type = MMISET_PWD_SET;
            MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);                
        }
        break;

    case MMISET_SECURITY_MODIFY_PHONELOCK_EDIT_LABEL_CTRL_ID:
        g_mmiset_pwd_type = MMISET_PWD_MODIFY;
        MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
        break;
            
    case MMISET_SECURITY_PRIVACY_EDIT_LABEL_CTRL_ID:
        MMK_CreatePubEditWin((uint32*)MMISET_SET_PRIVACY_PROTECT_WIN_TAB,PNULL);//for cr 226245
        break;

#if defined(MMI_MSD_SUPPORT)
    case MMISET_SECURITY_GUARDLOCK_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (sim_ok_num > 0)
        {
            MMK_CreateWin((uint32*)MMISET_SET_SECURITY_MSD_WIN_TAB,PNULL);
        }
        else
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
#else
        if (MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1))
        {
            MMK_CreateWin((uint32*)MMISET_SET_SECURITY_MSD_WIN_TAB,PNULL);
        }
        else
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
#endif
        break;
#endif

#ifdef FDN_SUPPORT

    case MMISET_SECURITY_FDN_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        select_type = MMISET_SET_FDN;
        
        //判断是否存在两张卡同时开启，不是则启动active sim
         if (sim_ok_num > 1)
        {
            MMIAPISET_OpenUssdSelectWin(select_type);
        }
        else
        {
            MMISET_OpenFDNWin((MN_DUAL_SYS_E)sim_ok);
        }
#else
        MMISET_OpenFDNWin(MN_DUAL_SYS_1);
#endif
        break;
#endif

 //MS00216980 cheney
#ifdef ENGTD_SUPPORT
    case MMISET_SECURITY_UPLMN:
        MMIENGTD_OpenUPLMNSettingWin();
        break;
#endif			

#if defined(MMI_BLACKLIST_SUPPORT)
    case MMISET_SECURITY_BLACKLIST_LABEL_CTRL_ID:
        if(MMISET_InitBlacklistParam())
        {
            MMISET_GetBlackList();
            MMK_CreateWin((uint32*)MMISET_SET_SECURITY_BLACKLIST_WIN_TAB,PNULL);
        }
        break;
#endif

    default:
        break;

    }
    break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
   }

    return recode;
}
#endif 
/*****************************************************************************/
//  Description : append security settings 1 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaSecuritySettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

#ifndef MMI_PDA_SUPPORT
    item_data.softkey_id[0] = STXT_SELECT;
    item_data.softkey_id[2] = STXT_RETURN;  
#endif

    GUILIST_AppendItem(ctrl_id, &item_t);
}

#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : to INITIAL PIN1 CONTROL 
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/
LOCAL void InitPin1PdaControl(MMI_WIN_ID_T win_id, uint32 dual_sys, BOOLEAN is_need_replace)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_SECURITY_PIN1_CTRL_ID;
    MMI_STRING_T text_str = {0};
    uint16 index = 0;
    uint16 focus_index = 0;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
#endif

    if(is_need_replace)
    { 
        focus_index = GUILIST_GetCurItemIndex(ctrl_id); 
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_PIN1_SETTINGS_MAX, FALSE );   
    }

    if( (is_need_replace && index == focus_index) || !is_need_replace)
    {
        if (MNSIM_IsPin1EnableEx(dual_sys))
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }

        AppendPdaSecuritySettingsItem2Line(            
        ctrl_id,
        TXT_SET_PIN1,
        text_str,
        image_id,
        (uint32)MMISMS_PDA_PIN1_STATUS,
        is_need_replace);
    }
    index++; 

    if(!is_need_replace)
    {
        AppendPdaSecuritySettingsItem1Line(
        ctrl_id,
        TXT_SERCURITY_MODIFY_PIN,
        (uint32)MMISMS_PDA_PIN1_CHANGE);
    }
    index++;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    sim_name = MMIAPISET_GetSimName(dual_sys);
    GUIWIN_SetTitleText(win_id, sim_name.wstr_arr, sim_name.wstr_len, FALSE);
#else
    GUIWIN_SetTitleTextId(win_id,TXT_SET_PIN1,FALSE);
#endif
}
#else
/*****************************************************************************/
// 	Description : to INITIAL PIN1 CONTROL 
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/
LOCAL void InitPin1Control(MMI_WIN_ID_T win_id, uint32 dual_sys)
{
    MMI_STRING_T label_name  = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
#endif

    GUIFORM_SetStyle(MMISET_PIN1_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);    
    MMI_GetLabelTextByLang(TXT_SET_PIN1, &label_name);
    GUILABEL_SetText(MMISET_PIN1_LABEL_CTRL_ID, &label_name, FALSE);   
    
    
    if (MNSIM_IsPin1EnableEx(dual_sys))
    {
        
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_PIN1_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
       
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_PIN1_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    sim_name = MMIAPISET_GetSimName(dual_sys);
    GUIWIN_SetTitleText(win_id, sim_name.wstr_arr, sim_name.wstr_len, FALSE);
#else
    GUIWIN_SetTitleTextId(win_id,TXT_SET_PIN1,FALSE);
#endif

    GUIFORM_SetStyle(MMISET_MODIFY_PIN1_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SERCURITY_MODIFY_PIN, &label_name);
	GUILABEL_SetText(MMISET_MODIFY_PIN1_EDIT_LABEL_CTRL_ID, &label_name, FALSE);

}
#endif
/*****************************************************************************/
// 	Description : to handle security setting pin1 menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
#ifdef DPHONE_SUPPORT 
LOCAL MMI_RESULT_E  HandlePin1MenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_MENU_GROUP_ID_T          group_id = 0;
    MMI_MENU_ID_T                menu_id = 0;
    MMI_RESULT_E                 recode = MMI_RESULT_TRUE;
    uint32                       dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_SET_SECURITY_PIN1_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_SET_SECURITY_PIN1_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_PIN_OPEN:          
            EnablePin1(dual_sys);/*lint !e64*/
            break;
            
        case ID_SET_PIN_MODIFY:          
            ModifyPin1(dual_sys);/*lint !e64*/
            break;
            
        case ID_SET_PIN_CLOSE:         
            DisablePin1(dual_sys);/*lint !e64*/
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#else
#ifndef MMI_GUI_STYLE_TYPICAL
LOCAL MMI_RESULT_E  HandlePin1MenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_SECURITY_PIN1_CTRL_ID;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPin1PdaControl(win_id, dual_sys, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_GET_FOCUS:
    case MSG_SET_BLACKLIST_LIST_UPDATE:
        InitPin1PdaControl(win_id, dual_sys, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT    
    case MSG_CTL_OK:
#endif
        {
            uint16 index = 0;
            uint32 user_data = 0;
            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &user_data);

            switch((MMISMS_PDA_PIN1_SETTINGS_TYPE_E)user_data)
            {
            case MMISMS_PDA_PIN1_STATUS:
                if(MNSIM_IsPin1EnableEx(dual_sys))
                {
                    DisablePin1(dual_sys);
                }
                else
                {
                    EnablePin1(dual_sys);
                }
                break;

            case MMISMS_PDA_PIN1_CHANGE:
                ModifyPin1(dual_sys);
                break;

            default:
                break;
            }
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
#else
LOCAL MMI_RESULT_E  HandlePin1MenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    //MMI_MENU_GROUP_ID_T          group_id = 0;
    //MMI_MENU_ID_T				 menu_id = 0;
    MMI_RESULT_E				 recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                ctrl_id = MMISET_PIN1_EDIT_LABEL_CTRL_ID;
    MMI_STRING_T                 label_string  = {0};
    //    MMI_STRING_T                 label_name  = {0};
    uint32                       dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        #ifndef MMI_GUI_STYLE_TYPICAL 
        GUIFORM_SetType(MMISET_PIN1_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        #endif
        InitPin1Control(win_id, dual_sys);
        MMK_SetAtvCtrl(win_id, MMISET_PIN1_EDIT_LABEL_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_FULL_PAINT:
        if (MNSIM_IsPin1EnableEx(dual_sys))
        {
            MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_string);
            GUILABEL_SetText(MMISET_PIN1_EDIT_LABEL_CTRL_ID, &label_string, FALSE);  
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_CLOSE, &label_string);
            GUILABEL_SetText(MMISET_PIN1_EDIT_LABEL_CTRL_ID, &label_string, FALSE); 
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT    
    case MSG_CTL_OK:
#endif    
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(msg_id==MSG_CTL_PENOK)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        if(ctrl_id == MMISET_MODIFY_PIN1_EDIT_LABEL_CTRL_ID)
        {
            ModifyPin1(dual_sys);/*lint !e64*/
        }
        else if(ctrl_id == MMISET_PIN1_EDIT_LABEL_CTRL_ID)
        {
            if(MNSIM_IsPin1EnableEx(dual_sys))
            {
                DisablePin1(dual_sys);
            }
            else
            {
                EnablePin1(dual_sys);
            }
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
#endif
#endif 
/*****************************************************************************/
// 	Description : get is item number of selected privacy protect
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL uint8 GetPrivacyProtecdItemNum(void)
{
    uint16 i =0;
    MMISET_PRIVACY_PROTECT_T privacy_protect_info = {0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    //get if is flip answer
    MMINV_READ(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        privacy_protect_info.protected_item_num = 0;
        for ( i = 0; i < MMISET_PROTECT_MAX; i++)
        {
            privacy_protect_info.protected_item[i] = FALSE;
        }		
        
        MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info);
    }
    
    return (privacy_protect_info.protected_item_num);
}

/*****************************************************************************/
// 	Description : 添加安全设置的选项到text list的item
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void AppendSecuritySetListItem(
                                     MMI_CTRL_ID_T	    ctrl_id
                                     )
{
    GUILIST_ITEM_T          item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0};/*lint !e64*/
    uint16  cur_selection = 0;
    uint8   privacy_selected_item = 0;
    MMI_STRING_T security_string = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MMI_TEXT_ID_T security_text_id[MMISET_SECURITY_MAX_NUM] =
    {TXT_SET_PIN1,
    TXT_SET_PIN2,
    TXT_SET_SET_PRIVACY_PROTECT,
#ifndef LCD_SLIDE_SUPPORT    
#ifdef DPHONE_SUPPORT 
#else
#ifdef MMI_PDA_SUPPORT
    TXT_SET_AUTO_LOCK_SCREEN,
#else
    TXT_SET_AUTO_BOADRKEY,
#endif    
#endif 
    TXT_SET_RED_KEYLOCK,
#endif
    TXT_POWER_ON_PWD,
#if defined(MMI_MSD_SUPPORT)
    TXT_MSD_MSD_FUN,
#endif
#ifdef FDN_SUPPORT    
    TXT_SET_FDN,
#endif
    //MS00216980 cheney
    TXT_COMM_UPLMN_SETTING,
#if defined(MMI_BLACKLIST_SUPPORT)
    TXT_SET_BLACKLIST,
#endif
    };
    uint32 i = 0;
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 j = MN_DUAL_SYS_1;
    #endif

    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    
    GUILIST_SetMaxItem(MMISET_SET_SECURITY_CTRL_ID,MMISET_SECURITY_MAX_NUM, FALSE );//max item 7
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)TXT_COMMON_OK;
    item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
    for (i = 0; i < MMISET_SECURITY_MAX_NUM; i++)
    {
        switch(i)
        {
        case MMISET_SECURITY_PIN_CODE:

#ifndef MMI_MULTI_SIM_SYS_SINGLE  
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
            for(j = MN_DUAL_SYS_1; j < MMI_DUAL_SYS_MAX; j++)
            {
                if (MMIAPIPHONE_GetSimExistedStatus(j) && MNSIM_IsPin1EnableEx(j))
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                    break;
                }
            }           
#else
            if (MMIAPIPHONE_GetSimExistedStatus(MMIAPISET_GetActiveSim()) && MNSIM_IsPin1EnableEx(MMIAPISET_GetActiveSim()))//PIN1码启动,left icon显示启动  //dual sys later
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
#endif
            break;
            
        case MMISET_SECURITY_PIN2_CODE:
#ifdef DPHONE_SUPPORT 
            image_id = IMAGE_CHECK_UNSELECTED_ICON; //IMAGE_SELECT_TICK_OFF_PIC
#else
            image_id = IMAGE_CONTROL_LIST_CHECK_PIN_ICON;
#endif
            break;
            
        case MMISET_SECURITY_PRIVACY:
            privacy_selected_item = GetPrivacyProtecdItemNum();
            if (privacy_selected_item > 0)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
            
#ifndef LCD_SLIDE_SUPPORT	
#ifdef DPHONE_SUPPORT 
#else
        case MMISET_SECURITY_KEYLOCK:
            if (MMISET_KEYLOCK_CLOSE != MMIAPISET_GetAutoBoardKey())
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
#endif            
            
        case MMISET_SECURITY_RED_KEYLOCK:
            if (MMIAPISET_GetRedKeyLock())
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
#endif
            
        case MMISET_SECURITY_POWER_ON_CODE:
            if (MMIAPISET_GetPowerOnPwdStatus())
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
            
#if defined(MMI_MSD_SUPPORT)
        case MMISET_SECURITY_MSD:
            if (MMIAPIMSD_IsMsdStartup())
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
#endif
            
#ifdef FDN_SUPPORT
        case MMISET_SECURITY_FDN:
            {
                BOOLEAN is_fdn_enable = FALSE;
                MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;                
                uint32 dual_sys = MN_DUAL_SYS_1;
                for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                {
                   fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);
                   if(fdn_context_ptr->is_fdn_enable)
                    {
                        is_fdn_enable=fdn_context_ptr->is_fdn_enable;
                        break;
                    }
                }
                if (is_fdn_enable)
                {
                    image_id = IMAGE_CHECK_SELECTED_ICON;
                }
                else
                {
                    image_id = IMAGE_CHECK_UNSELECTED_ICON;
                }
            }
            break;
#endif  
           case MMISET_SECURITY_UPLMN:
	     //@MS00216980 cheney  UPLMN is only user for cmcc test
	     if(!MMIAPIENG_GetCMCCTestFlag())
	     {
	    	 continue;
	     }
	     image_id = IMAGE_CHECK_UNSELECTED_ICON;
            break;

#if defined(MMI_BLACKLIST_SUPPORT)
        case MMISET_BLACKLIST:
            if (MMIAPISET_GetIsBlacklistAudioCall() ||
                MMIAPISET_GetIsBlacklistVedioCall() ||
                MMIAPISET_GetIsBlacklistMessage())
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }            
            break;
#endif
            
        default:
            //SCI_TRACE_LOW:"AppendSecuritySetListItem:mode=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_3523_112_2_18_2_51_57_311,(uint8*)"d",i);
            return;
        }
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = image_id;
        
        MMI_GetLabelTextByLang(security_text_id[i], &security_string);
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = security_string.wstr_len;  
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = security_string.wstr_ptr;
//         MMIAPICOM_Wstrncpy( item_data.item_content[1].item_data.text_buffer.wstr,
//             security_string.wstr_ptr,
//             item_data.item_content[1].item_data.text_buffer.wstr_len );
        
        //item_data.item_content[1].item_data.text_buffer.is_ucs2 = security_string.is_ucs2;
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
// 	Description : 开启pin1
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void EnablePin1(MN_DUAL_SYS_E     dual_sys)
{
    if (MNSIM_IsPin1EnableEx(dual_sys))//PIN1码启动
    {
        MMIPUB_OpenAlertWarningWin(TXT_PIN1_ENABLED);
    }
    else
    {
        if (MMIAPIPHONE_IsPermitSetPin1(dual_sys))
        {
            MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
            MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_ENABLE,MNSIM_PIN1,(uint32)MMISET_SET_ENABLE_PIN1_WIN_TAB, TRUE);
        }
        else
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
    }
}

/*****************************************************************************/
// 	Description : 修改pin1
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void ModifyPin1(MN_DUAL_SYS_E     dual_sys)
{
    if (MNSIM_IsPin1EnableEx(dual_sys))//PIN1码启动
    {
        if (MMIAPIPHONE_IsPermitSetPin1(dual_sys))
        {
            MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
            MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_CHANGE,MNSIM_PIN1,(uint32)MMISET_SET_MODIFY_PIN1_WIN_TAB, TRUE);
        }
        else
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ENABLE_PIN1);
    }
}

/*****************************************************************************/
// 	Description : 关闭pin1
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DisablePin1(MN_DUAL_SYS_E  dual_sys)
{
    if (MNSIM_IsPin1EnableEx(dual_sys))//PIN1码启动
    {
        if (MMIAPIPHONE_IsPermitSetPin1(dual_sys))
        {
            MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
            MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_DISABLE,MNSIM_PIN1,(uint32)MMISET_SET_DISABLE_PIN1_WIN_TAB, TRUE);
        }
        else
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_PIN1_DISABLED);
    }
}

/*****************************************************************************/
// 	Description : add privacy protect list item 
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendPrivacyProtectListItem(
                                                MMI_CTRL_ID_T	    ctrl_id
                                                )
{
    uint16                      i =0;		
    MMISET_PRIVACY_PROTECT_T    privacy_protect_info = {0};
    MN_RETURN_RESULT_E			return_value = MN_RETURN_FAILURE;
    
    //add items to list box
    MMIAPISET_AppendListItemByTextId(STXT_MAIN_CL,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(STXT_MAIN_MESSAGE,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#ifdef  CAMERA_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_ENTERTAIMENT_CAMERA,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#endif
    
    MMIAPISET_AppendListItemByTextId(STXT_MAIN_PB,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    
    
    MMIAPISET_AppendListItemByTextId(TXT_EXPLORER,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#ifdef EBOOK_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_COMM_EBOOK,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#endif
#ifdef CALENDAR_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_TOOLS_CALENDAR,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#endif
    MMIAPISET_AppendListItemByTextId(TXT_UDISK_UDISK,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#ifdef MMI_AUDIO_PLAYER_SUPPORT   
    MMIAPISET_AppendListItemByTextId(TXT_SET_SHORTCUT_MP3,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_VIDEOPLAYER,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#endif
#endif
#ifdef ASP_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_MMIASP_ANSWER_PHONE,STXT_SELECT,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
#endif

    //get privacy protect info and change list icon
    MMINV_READ(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info,return_value);
    
    for (i = 0; i < MMISET_PROTECT_MAX; i ++ )
    {
        if (privacy_protect_info.protected_item[i])
        {
            GUILIST_SetSelectedItem(ctrl_id,i,TRUE);
        }
    }
}


/*****************************************************************************/
// 	Description : set privacy protect information
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetPrivacyProtectInfo(
                                         MMI_CTRL_ID_T	    ctrl_id
                                         )
{
    uint16						i = 0;
    uint16						selected_num = 0;
    uint16 						selected_item[MMISET_PROTECT_MAX] = {0};
    MMISET_PRIVACY_PROTECT_T    privacy_protect_info = {0};
    
    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,selected_item,MMISET_PROTECT_MAX);
    
    for ( i = 0; i < MMISET_PROTECT_MAX; i++)
    {
        privacy_protect_info.protected_item[i] = FALSE;
    }
    
    //get select privacy item from ctrl
    for (i=0; i<selected_num; i++)
    {
        if (selected_item[i] < MMISET_PROTECT_MAX)
        {
            privacy_protect_info.protected_item[selected_item[i]] = TRUE;
        }
    }
    //save privacy protect info into nv
    privacy_protect_info.protected_item_num = selected_num;	
    MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info);
}

/*****************************************************************************/
// 	Description : if privacy protect password correct
//	Global resource dependence : 
//  Author:kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCorrectPrivacyProtectPwd(
                                                 const wchar *pwd_ptr,
                                                 uint16       pwd_len
                                                 )
{
    return MMISET_IsCorrectPowerOnPwd(pwd_ptr, pwd_len);//cr227408 maryxiao
}

/*****************************************************************************/
// 	Description : 设置开机密码
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetPowerOnPwd(
                                 const wchar *pwd_ptr,
                                 uint16     pwd_len
                                 )
                                 
{
    MMISET_POWER_ON_PWD_T   phone_code = {0};
//    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    if(PNULL == pwd_ptr)
    {
        //SCI_TRACE_LOW:"mmiset pwd_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_3731_112_2_18_2_51_58_312,(uint8*)"");
        return;
    }

    //read power on password from
    InitPhonePwd(&phone_code);
    
    phone_code.pwd_len = (uint8)(pwd_len & 0x00FF);
    MMI_WSTRNTOSTR((uint8 *)phone_code.pwd,MMISET_POWER_ON_PWD_MAX_LEN,
        pwd_ptr,pwd_len,pwd_len);
    //write power on password into NV
    MMINV_WRITE(MMINV_SET_POWER_ON_PWD_INFO,&phone_code);
}

/*****************************************************************************/
// 	Description : set privacy protect password
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetPrivacyProtectPwd(
                                        const wchar *pwd_ptr,
                                        uint16     pwd_len
                                        )
                                        
{
    uint8							pwd_num[MMISET_PRIVACY_PROTECT_PWD_MAX_LEN] = {0};
    MMISET_PRIVACY_PROTECT_PWD_T	privacy_code = {0};
    MN_RETURN_RESULT_E				return_value = MN_RETURN_FAILURE;
    
    if(PNULL == pwd_ptr)
    {
        //SCI_TRACE_LOW:"mmiset pwd_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_3761_112_2_18_2_51_58_313,(uint8*)"");
        return;
    }
    SCI_MEMSET(&privacy_code,0,sizeof(MMISET_PRIVACY_PROTECT_PWD_T));
    
    //read privacy protect password from nv
    MMINV_READ(MMINV_SET_PRIVACY_PROTECT_PWD_INFO,&privacy_code,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        sprintf((char*)pwd_num, "%s" , MMISET_PRIVACY_PROTECT_INIT_PWD);
        privacy_code.pwd_len = strlen((char*)pwd_num);

        if(privacy_code.pwd_len >= MMISET_PRIVACY_PROTECT_PWD_MAX_LEN)
        {
            privacy_code.pwd_len = MMISET_PRIVACY_PROTECT_PWD_MAX_LEN;            
        }
        SCI_MEMCPY(privacy_code.pwd, pwd_num, privacy_code.pwd_len);
        MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_PWD_INFO,&privacy_code);
    }
    
    privacy_code.pwd_len = (uint8)(pwd_len & 0x00FF);

    MMI_WSTRNTOSTR( (uint8 *)privacy_code.pwd,MMISET_PRIVACY_PROTECT_PWD_MAX_LEN,
        pwd_ptr,pwd_len,pwd_len);

    //write privacy protect password into NV
    MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_PWD_INFO,&privacy_code);
}
/*****************************************************************************/
// 	Description : reset the privacy protect mode 
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC  void MMISET_ResetPrivacyProtectInfo(void)
{
    uint16						i =0;
    MMISET_PRIVACY_PROTECT_T   privacy_protect_info = {0};
    
    privacy_protect_info.protected_item_num = 0;
    for ( i = 0; i < MMISET_PROTECT_MAX; i++)
    {
        privacy_protect_info.protected_item[i] = FALSE;
    }		
    MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info);	
}

/*****************************************************************************/
// 	Description : reset the privacy protect password 
//	Global resource dependence : 
//  Author: fen.xie
//	Note:for MS00179453
/*****************************************************************************/
PUBLIC  void MMISET_ResetPrivacyProtectPwd(void)
{
    uint8                           pwd_num[MMISET_PRIVACY_PROTECT_PWD_MAX_LEN] =   {0};
    MMISET_PRIVACY_PROTECT_PWD_T    code_info = {0};
    
    sprintf((char*)pwd_num, "%s" , MMISET_PRIVACY_PROTECT_INIT_PWD);
    code_info.pwd_len = strlen((char*)pwd_num);
    if(code_info.pwd_len >= MMISET_PRIVACY_PROTECT_PWD_MAX_LEN)
    {
        code_info.pwd_len = MMISET_PRIVACY_PROTECT_PWD_MAX_LEN;           
    }
    SCI_MEMCPY(code_info.pwd, pwd_num, code_info.pwd_len);  
    
    MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_PWD_INFO,&code_info);
}

/*****************************************************************************/
// 	Description : 开机密码是否已经开启
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetPowerOnPwdStatus(void)
{
    MMISET_POWER_ON_PWD_T   phone_code = {0};
//    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    InitPhonePwd(&phone_code);
    
    return (phone_code.is_enable);
}



/*****************************************************************************/
// 	Description : 开机密码出厂设置
//	Global resource dependence : 
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetPowerOnPwdStatus(void)//@zhaohui,cr95392
{
    MMISET_POWER_ON_PWD_T   phone_code = {0};
    uint8                   pwd_num[MMISET_PRIVACY_PROTECT_PWD_MAX_LEN] =   {0};
    
    SCI_MEMSET(&phone_code,0,sizeof(MMISET_POWER_ON_PWD_T));
    sprintf((char*)pwd_num, "%s" , MMISET_PRIVACY_PROTECT_INIT_PWD);
    phone_code.pwd_len = strlen((char*)pwd_num);

    if(phone_code.pwd_len >= MMISET_PRIVACY_PROTECT_PWD_MAX_LEN)
    {
        phone_code.pwd_len = MMISET_PRIVACY_PROTECT_PWD_MAX_LEN;
    }
          
    SCI_MEMCPY(phone_code.pwd, pwd_num, phone_code.pwd_len);  
    MMINV_WRITE(MMINV_SET_POWER_ON_PWD_INFO,&phone_code);
}

/*****************************************************************************/
// 	Description : 开机密码是否正确
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCorrectPowerOnPwd(
                                          const wchar *pwd_ptr,
                                          uint16     pwd_len
                                          )
{
    BOOLEAN                 result = FALSE;
    MMISET_POWER_ON_PWD_T   phone_code = {0};
//    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    if(PNULL == pwd_ptr)
    {
         //SCI_TRACE_LOW:"mmiset pwd_ptr== PNULL"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_3879_112_2_18_2_51_58_314,(uint8*)"");
         return result;
    }
    //read power on password from
    InitPhonePwd(&phone_code);
    
    if( pwd_len == phone_code.pwd_len
        && 0 == MMIAPICOM_WstrStrncmp( pwd_ptr, (uint8 *)phone_code.pwd, pwd_len ) )
    {
        result = TRUE;
    }

    //result = MMIAPICOM_CompareTwoTeleNum((uint8*)pwd_ptr,(uint8)pwd_len,(uint8*)phone_code.pwd,(uint8)phone_code.pwd_len);
    
    return (result);
}

/*****************************************************************************/
// 	Description : to handle set flip answer
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetRedKeyLockWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_RED_KEYLOCK_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMISET_AppendOpenCloseListItem(MMISET_SET_RED_KEYLOCK_CTRL_ID,MMIAPISET_GetRedKeyLock);
        MMK_SetAtvCtrl(win_id,MMISET_SET_RED_KEYLOCK_CTRL_ID);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT    
    case MSG_CTL_OK:
#endif    
    case MSG_APP_WEB:
    case MSG_APP_OK:
        MMIAPISET_SetByOpenCloseList(MMISET_SET_RED_KEYLOCK_CTRL_ID, MMIAPISET_GetActiveSim(), MMIAPISET_SetRedKeyLock);
        
        //success!
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
//  Description :open reset sim lock switch menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSIMLockSwitchWin(void)
{
    MMK_CreateWin((uint32*)MMISET_SIM_LOCK_SETTING_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :Init SIMLock Settings List Ctrl
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void  InitSIMLockSettingsListCtrl(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    
    GUILIST_SetMaxItem(ctrl_id, 2, FALSE );       
    
    AppendPdaSettingsItem1Line(            
                                                        ctrl_id,
                                                        TXT_SET_SIM_LOCK_INFO,
                                                        PNULL);
    
    AppendPdaSettingsItem1Line(            
                                                        ctrl_id,
                                                        TXT_SET_SIM_LOCK_SWITCH,
                                                        PNULL);

    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
}

/*****************************************************************************/
//  Description : to handle simlock setting menu
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockSettingMenuWindow(
                                                                                                       MMI_WIN_ID_T    win_id, 
                                                                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                                                                       DPARAM             param
                                                                                                       )
{

   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = MMISET_SECURITY_SIMLOCK_MENU_FORM_CTRL_ID;
   
   switch(msg_id)
   {
    case MSG_OPEN_WINDOW:
        InitSIMLockSettingsListCtrl(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        break;

    case MSG_GET_FOCUS:
        InitSIMLockSettingsListCtrl(ctrl_id);
        break;
  
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:              
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint16 index = 0;
            index = GUILIST_GetCurItemIndex(ctrl_id);
            if(0 == index)
            {
                MMK_CreateWin((uint32*)MMISET_SHOW_SIM_LOCK_INFO_WIN_TAB, PNULL);
            } 
            else if(1 == index)
            {
                OpenSIMLockSwitchWin();
            }              
        }
        break;

       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
           MMK_CloseWin(win_id);
           break;
           
       default:
           recode = MMI_RESULT_FALSE;
           break;
   }
   
   return recode;
}

/*****************************************************************************/
//  Description :open reset sim lock switch menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void OpenSIMLockSwitchWin(void)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_SIM_LOCK_SWITCH_WIN_TAB, (ADD_DATA)sim_sys);
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMISET_SelectSIMFunc(SIMLockSIMSelectCallback, MMISET_APPEND_SYS_OK,PNULL);
    }
#endif
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMLockSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    if(isReturnOK)
    {
        MMK_CreateWin((uint32*)MMISET_SIM_LOCK_SWITCH_WIN_TAB,(ADD_DATA)dual_sys);
    }

    return (recode);
}

/********************************************************************************
//   NAME: GetSIMLockInfoStr
//   DESCRIPTION:   Get SIM Lock Info Str
//   PARAM IN:      sim_lock_info_buf - 
                        buf_len - 
//   PARAM OUT:SIM Lock info string len
//   AUTHOR: dave.ruan
//   Note: 
********************************************************************************/
LOCAL int GetSIMLockInfoStr(uint8** sim_lock_info_buf, uint32* buf_len)
{
    MN_SIM_LOCK_CUSTOMIZE_DATA_T* sim_lock_customize_data = PNULL;
    MN_SIM_LOCK_STATUS_T *sim_lock_status= MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    MN_SIM_LOCK_USER_DATA_T* sim_lock_user_data = PNULL;
    uint32 offset = 0;
    uint32 i = 0;
#if !defined(WIN32) && !defined(PRODUCT_DM)
    BOOLEAN is_get_data_ok = FALSE;
#endif
    
    sim_lock_customize_data = SCI_ALLOCAZ(sizeof(MN_SIM_LOCK_CUSTOMIZE_DATA_T));
    if(PNULL == sim_lock_customize_data)
    {
        return NULL;
    }
    sim_lock_user_data = SCI_ALLOCAZ(sizeof(MN_SIM_LOCK_USER_DATA_T));
    if(PNULL == sim_lock_user_data)
    {
        return NULL;
    }
    
#ifdef WIN32
    SCI_MEMCPY(sim_lock_customize_data, &s_sim_lock_customize_data,sizeof(MN_SIM_LOCK_CUSTOMIZE_DATA_T));
    SCI_MEMCPY(sim_lock_user_data, &s_sim_lock_user_data,sizeof(MN_SIM_LOCK_USER_DATA_T));
    sim_lock_status->network_locked = TRUE;
    sim_lock_status->network_subset_locked = TRUE;
    sim_lock_status->sp_locked = TRUE;    
    sim_lock_status->corporate_locked = TRUE;
    sim_lock_status->user_locked = TRUE;    
#else
#ifdef PRODUCT_DM
    {
        if(sim_lock_status->network_locked)
        {
            sim_lock_customize_data->SIM_lock_status |= MMI_SIM_NETWORK_LOCK;
            for (i =0; i < MAX_PERSONALISATIONS; i++)
            {
                LOCK_CONTENT_INFO_T lock_content_info= {0};
                SIM_LOCK_KEY_T  lock_key = {0};
                
                if(MN_SIM_LOCK_NO_ERROR == MNPHONE_ReadLockContent(MMI_SIM_NETWORK_LOCK, lock_key, i, &lock_content_info))
                {
                    sim_lock_customize_data->network_locks.locks[i] = lock_content_info.network_lock;
                }
                else
                {
                    sim_lock_customize_data->network_locks.numLocks = i;
                    break;
                }
            }
        }

        if(sim_lock_status->network_subset_locked)
        {
            sim_lock_customize_data->SIM_lock_status |= MMI_SIM_NETWORK_SUBSET_LOCK;
            for (i =0; i < MAX_PERSONALISATIONS; i++)
            {
                LOCK_CONTENT_INFO_T lock_content_info= {0};
                SIM_LOCK_KEY_T  lock_key = {0};
                
                if(MN_SIM_LOCK_NO_ERROR == MNPHONE_ReadLockContent(MMI_SIM_NETWORK_SUBSET_LOCK, lock_key, i, &lock_content_info))
                {
                    sim_lock_customize_data->network_subset_locks.locks[i] = lock_content_info.network_subset_lock;
                }
                else
                {
                    sim_lock_customize_data->network_subset_locks.numLocks = i;
                    break;
                }
            }
        }
        
        if(sim_lock_status->sp_locked)
        {
            sim_lock_customize_data->SIM_lock_status |= MMI_SIM_SP_LOCK;
            for (i =0; i < MAX_PERSONALISATIONS; i++)
            {
                LOCK_CONTENT_INFO_T lock_content_info= {0};
                SIM_LOCK_KEY_T  lock_key = {0};
                
                if(MN_SIM_LOCK_NO_ERROR == MNPHONE_ReadLockContent(MMI_SIM_SP_LOCK,lock_key, i, &lock_content_info))
                {
                    sim_lock_customize_data->SP_locks.locks[i] = lock_content_info.sp_lock;
                }
                else
                {
                    sim_lock_customize_data->SP_locks.numLocks = i;
                    break;
                }
            }
        }

        if(sim_lock_status->corporate_locked)
        {
            sim_lock_customize_data->SIM_lock_status |= MMI_SIM_CORPORATE_LOCK;
            for (i =0; i < MAX_PERSONALISATIONS; i++)
            {
                LOCK_CONTENT_INFO_T  lock_content_info= {0};
                SIM_LOCK_KEY_T  lock_key = {0};
                
                if(MN_SIM_LOCK_NO_ERROR == MNPHONE_ReadLockContent(MMI_SIM_CORPORATE_LOCK,lock_key, i, &lock_content_info))
                {
                    sim_lock_customize_data->corporate_locks.locks[i] = lock_content_info.corporate_lock;
                }
                else
                {
                    sim_lock_customize_data->corporate_locks.numLocks = i;
                    break;
                }
            }
        }
   
        if(sim_lock_status->user_locked)
        {
            sim_lock_customize_data->SIM_lock_status |= MMI_SIM_USER_LOCK;
            for (i =0; i < MAX_PERSONALISATIONS; i++)
            {
                LOCK_CONTENT_INFO_T lock_content_info= {0};
                SIM_LOCK_KEY_T  lock_key = {0};
                
                if(MN_SIM_LOCK_NO_ERROR == MNPHONE_ReadLockContent(MMI_SIM_USER_LOCK,lock_key, i, &lock_content_info))
                {
                    sim_lock_user_data->user_locks.locks[i] = lock_content_info.sim_lock;
                }
                else
                {
                    sim_lock_user_data->user_locks.numLocks = i;
                    break;
                }
            }
        }
     
    }
#else
    is_get_data_ok = MNPHONE_GetXMLLocks(sim_lock_customize_data, sim_lock_user_data);

    if(!is_get_data_ok)
    {
        return -1;
    }
#endif    
#endif

    if(PNULL == buf_len)
    {
        return -1;
    }
    
    if(0 == sim_lock_customize_data->SIM_lock_status)
    {
        *sim_lock_info_buf = SCI_ALLOCA(SIM_LOCKED_NULL_STR_LEN);
        if(PNULL == *sim_lock_info_buf)
        {
            return -1;
        }
 
        *buf_len = SIM_LOCKED_NULL_STR_LEN;
        MMI_MEMCPY(*sim_lock_info_buf, *buf_len, "<SIM Locked NULL>", SIM_LOCKED_NULL_STR_LEN, SIM_LOCKED_NULL_STR_LEN);
        offset = SIM_LOCKED_NULL_STR_LEN;
        SCI_TRACE_LOW("GetSIMLockInfoStr SIM_lock_status offset = %d",offset);
        return offset;
    }
    else 
    {
        uint32 temp_buf_len = 0;
        uint32 temp_str_len = 0;

        if(sim_lock_status->network_locked)//
        {
            //"PLMNi: 460 1  2  (CR)" 4+1+1+1 + 3+1  +3+3 +1 =18
            temp_buf_len += 18 *sim_lock_customize_data->network_locks.numLocks + SIM_LOCKED_NULL_STR_LEN + 2;
        }

        if (sim_lock_status->network_subset_locked)//()
        {
            //"PLMNi: 460 2  2  (CR) network subset: (CR) 2 3 (CR)" 4+1+1+1 +3+1 +3+3 +1 +14+1 +2+1 = 36
            temp_buf_len += 36 *sim_lock_customize_data->network_subset_locks.numLocks + NETWORK_SUBSET_LOCKED_STR_LEN + 2;
        }

        if (sim_lock_status->sp_locked) //
        {
            //"PLMNi: 460 3  2  (CR) sp: (CR) 2 3 (CR)" 4+1+1+1 +3+1 +3+3 +1 +4+1 +2+1 = 26
            temp_buf_len += 26 *sim_lock_customize_data->SP_locks.numLocks + SP_LOCKED_STR_LEN + 2;
        }

        if (sim_lock_status->corporate_locked) //
        {
            //"PLMNi: 460 3  2  (CR) SP: (CR) 2 (CR)Corporate: 3 (CR)" 4+1+1+1 +3+1 +3+3 +1 +4+1 +2 +11 + 2 +1 = 39
            temp_buf_len += 39 *sim_lock_customize_data->corporate_locks.numLocks + CORPORATE_LOCKED_STR_LEN + 2;
        }
        
        if (sim_lock_status->user_locked) //
        {
            //"IMSI: 123456789012345678  (CR)" 5 + 18 +1 = 24
            temp_buf_len += 24 *sim_lock_user_data->user_locks.numLocks + NETWORK_LOCKED_STR_LEN + 2;
        }

        *sim_lock_info_buf = SCI_ALLOCA(temp_buf_len);
        if(PNULL == *sim_lock_info_buf)
        {
            return -1;
        }
        *buf_len = temp_buf_len;
        SCI_MEMSET(*sim_lock_info_buf, 0x0, temp_buf_len);

        ///////NETWORK_LOCKED/////////////
        if (sim_lock_status->network_locked) //
        {
            uint8 temp_buf[TEMP_BUF_LEN] = {0};
            
            MMI_MEMCPY(*sim_lock_info_buf, temp_buf_len, "<Network Locked>", NETWORK_LOCKED_STR_LEN, NETWORK_LOCKED_STR_LEN);
            offset += NETWORK_LOCKED_STR_LEN;
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;

            for(i = 0; i < sim_lock_customize_data->network_locks.numLocks; i++)
            {
                //PLMN: 
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "PLMN", 4, 4);
                offset += 4;
                MMIAPICOM_Int2Str(i+1, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = COLON_CHAR;   //:
                offset += 1;
                (*sim_lock_info_buf)[offset] = BL_CHAR;    //blank char
                offset += 1;

                // MCC
                MMIAPICOM_Int2Str(sim_lock_customize_data->network_locks.locks[i].mcc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;

                //MNC
                MMIAPICOM_Int2Str(sim_lock_customize_data->network_locks.locks[i].mnc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;
                //digital num
                MMIAPICOM_Int2Str(sim_lock_customize_data->network_locks.locks[i].mnc_digit_num, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                
                (*sim_lock_info_buf)[offset] = CR_CHAR;
                offset += 1;
            }
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;
        }

        SCI_TRACE_LOW("GetSIMLockInfoStr network_locks offset = %d",offset);
        //////////////NETWORK_SUBSET_LOCKED/////////////
        if (sim_lock_status->network_subset_locked) //
        {
            uint8 temp_buf[TEMP_BUF_LEN] = {0};
            
            MMI_MEMCPY(*sim_lock_info_buf+ offset, temp_buf_len, "<Network Subset Locked>", NETWORK_SUBSET_LOCKED_STR_LEN, NETWORK_SUBSET_LOCKED_STR_LEN);
            offset += NETWORK_SUBSET_LOCKED_STR_LEN;
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;

            for(i = 0; i < sim_lock_customize_data->network_subset_locks.numLocks; i++)
            {
                //PLMN: 
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "PLMN", 4, 4);
                offset += 4;
                MMIAPICOM_Int2Str(i +1, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = COLON_CHAR;   //:
                offset += 1;
                (*sim_lock_info_buf)[offset] = BL_CHAR;    //blank char
                offset += 1;

                // MCC
                MMIAPICOM_Int2Str(sim_lock_customize_data->network_subset_locks.locks[i].plmn.mcc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;

                //MNC
                MMIAPICOM_Int2Str(sim_lock_customize_data->network_subset_locks.locks[i].plmn.mnc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;
                 
                //digital num
                MMIAPICOM_Int2Str(sim_lock_customize_data->network_subset_locks.locks[i].plmn.mnc_digit_num, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                
                (*sim_lock_info_buf)[offset] = CR_CHAR;
                offset += 1;
                //network subset
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "network subset: ", 16, 16);
                offset += 16;

                MMIAPICOM_Int2Str(sim_lock_customize_data->network_subset_locks.locks[i].network_subset[0], temp_buf, 2);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, 1, 1);
                offset += 1;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;
                MMIAPICOM_Int2Str(sim_lock_customize_data->network_subset_locks.locks[i].network_subset[1], temp_buf, 2);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, 1, 1);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += 1;

                (*sim_lock_info_buf)[offset] = CR_CHAR;
                offset += 1;
            }
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;
        }
        
        SCI_TRACE_LOW("GetSIMLockInfoStr network_subset_locks offset = %d",offset);
        ///////////SP LOCK/////////////
        if (sim_lock_status->sp_locked) //
        {
            uint8 temp_buf[TEMP_BUF_LEN] = {0};
            
            MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "<SP Locked>", SP_LOCKED_STR_LEN, SP_LOCKED_STR_LEN);
            offset += SP_LOCKED_STR_LEN;
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;

            for(i = 0; i < sim_lock_customize_data->SP_locks.numLocks; i++)
            {
                //PLMN: 
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "PLMN", 4, 4);
                offset += 4;
                MMIAPICOM_Int2Str(i+1, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = COLON_CHAR;   //:
                offset += 1;
                (*sim_lock_info_buf)[offset] = BL_CHAR;    //blank char
                offset += 1;

                // MCC
                MMIAPICOM_Int2Str(sim_lock_customize_data->SP_locks.locks[i].plmn.mcc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;

                //MNC
                MMIAPICOM_Int2Str(sim_lock_customize_data->SP_locks.locks[i].plmn.mnc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;
                
                //digital num
                MMIAPICOM_Int2Str(sim_lock_customize_data->SP_locks.locks[i].plmn.mnc_digit_num, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;
                
                //SP
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "sp: ", 4, 4);
                offset += 4;
                MMIAPICOM_Int2Str(sim_lock_customize_data->SP_locks.locks[i].sp, temp_buf, 3);//sp 00~EF
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;

                (*sim_lock_info_buf)[offset] = CR_CHAR;
                offset += 1;
            }
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;
        }

        SCI_TRACE_LOW("GetSIMLockInfoStr SP_locks offset = %d",offset);
        ///////////corporate_locked///////////
        if (sim_lock_status->corporate_locked) //
        {
            uint8 temp_buf[TEMP_BUF_LEN] = {0};
            
            MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "<Corporate Locked>", CORPORATE_LOCKED_STR_LEN, CORPORATE_LOCKED_STR_LEN);
            offset += CORPORATE_LOCKED_STR_LEN;
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;

            for(i = 0; i < sim_lock_customize_data->corporate_locks.numLocks; i++)
            {
                //PLMN: 
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "PLMN", 4, 4);
                offset += 4;
                MMIAPICOM_Int2Str(i+1, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = COLON_CHAR;   //:
                offset += 1;
                (*sim_lock_info_buf)[offset] = BL_CHAR;    //blank char
                offset += 1;

                // MCC
                MMIAPICOM_Int2Str(sim_lock_customize_data->corporate_locks.locks[i].plmn.mcc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;

                //MNC
                MMIAPICOM_Int2Str(sim_lock_customize_data->corporate_locks.locks[i].plmn.mnc, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;
                
                //digital num
                MMIAPICOM_Int2Str(sim_lock_customize_data->corporate_locks.locks[i].plmn.mnc_digit_num, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = CR_CHAR;
                offset += 1;
                
                //SP
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "SP: ", 4, 4);
                offset += 4;
                MMIAPICOM_Int2Str(sim_lock_customize_data->corporate_locks.locks[i].sp, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;
                (*sim_lock_info_buf)[offset] = BL_CHAR;
                offset += 1;
                
                //corporate
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "Corporate: ", 11, 11);
                offset += 11;
                MMIAPICOM_Int2Str(sim_lock_customize_data->corporate_locks.locks[i].corporate, temp_buf, 3);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len, temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;

                (*sim_lock_info_buf)[offset] = CR_CHAR;
                offset += 1;
            }
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;
        }

        SCI_TRACE_LOW("GetSIMLockInfoStr corporate_locks offset = %d",offset);
        //////////user_locked//////////
        if (sim_lock_status->user_locked) //
        {
            uint8 temp_buf[TEMP_BUF_LEN] = {0};
            
            MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "<User Locked>", USER_LOCKED_STR_LEN, USER_LOCKED_STR_LEN);
            offset += USER_LOCKED_STR_LEN;
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;

            for(i = 0; i < sim_lock_user_data->user_locks.numLocks; i++)
            {
                //IMSI
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, "IMSI:", 5, 5);
                offset += 5;
                MMIAPICOM_BcdLfToStr( sim_lock_user_data->user_locks.locks[i].imsi_val, 8*2, (char*)temp_buf);
                temp_str_len = SCI_STRLEN((char*)temp_buf);
                MMI_MEMCPY(*sim_lock_info_buf + offset, temp_buf_len, temp_buf, temp_str_len,temp_str_len);
                SCI_MEMSET(temp_buf, 0x00, TEMP_BUF_LEN);
                offset += temp_str_len;

                (*sim_lock_info_buf)[offset] = CR_CHAR;
                offset += 1;
            }
            (*sim_lock_info_buf)[offset] = CR_CHAR;
            offset += 1;
        }
    }

    SCI_TRACE_LOW("GetSIMLockInfoStr user_locks offset = %d",offset);
    return (offset);
}

/*****************************************************************************/
// 	Description :  handle SIM Lock info win
//	Global resource dependence : 
//    Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowSIMLockInfoMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8* str_buf = PNULL;
    wchar* wstr = PNULL;
    uint32 str_buf_len = 0;      
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text.wstr_len = GetSIMLockInfoStr(&str_buf, &str_buf_len);
        wstr = SCI_ALLOCA(str_buf_len * sizeof(wchar));
        if(PNULL == wstr)
        {
            break;
        }
        SCI_MEMSET(wstr, 0, str_buf_len * sizeof(wchar));
        text.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, str_buf_len, (uint8*)str_buf, text.wstr_len, text.wstr_len );
        GUITEXT_SetString(MMISET_SECURITY_SIM_LOCK_INFO_FORM_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_SECURITY_SIM_LOCK_INFO_FORM_CTRL_ID);
        SCI_FREE(str_buf);
        SCI_FREE(wstr);
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : Is SIMLock On of input type 
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  IsSIMLockOn(
                                uint32 unlock_type      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                )
{
    MN_SIM_LOCK_STATUS_T *sim_lock_status= MMIAPIPHONE_GetSIMUnlockConfigDataPtr();

    if(unlock_type&MMI_SIM_NETWORK_LOCK)
    {
        return sim_lock_status->network_locked;
    }
    else if(unlock_type&MMI_SIM_NETWORK_SUBSET_LOCK)
    {
        return sim_lock_status->network_subset_locked;
    }
    else if(unlock_type&MMI_SIM_SP_LOCK)
    {
        return sim_lock_status->sp_locked;
    }
    else if(unlock_type&MMI_SIM_CORPORATE_LOCK)
    {
        return sim_lock_status->corporate_locked;
    }
    else if(unlock_type&MMI_SIM_USER_LOCK)
    {
        return sim_lock_status->user_locked;
    }
    else
    {
        return sim_lock_status->network_locked;
    }
}

#ifdef DPHONE_SUPPORT 
/*****************************************************************************/
// 	Description : to handle security setting SIM Lock menu
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockMenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_MENU_GROUP_ID_T          group_id = 0;
    MMI_MENU_ID_T                menu_id = 0;
    MMI_RESULT_E                 recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys =  (uint32)MMK_GetWinAddDataPtr(win_id);
    MN_SIM_LOCK_STATUS_T *sim_lock_status= MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_SECURITY_SIM_LOCK_NETWORK:          
            MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_NETWORK_LOCK,dual_sys,sim_lock_status->network_locked);
            break;
            
        case ID_SET_SECURITY_SIM_LOCK_NETWORK_SUBSET:
            MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_NETWORK_SUBSET_LOCK,dual_sys,sim_lock_status->network_locked);
            break;
            
        case ID_SET_SECURITY_SIM_LOCK_SP:         
            MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_SP_LOCK,dual_sys,sim_lock_status->network_locked);
            break;
            
        case ID_SET_SECURITY_SIM_LOCK_CORPORATE:         
            MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_CORPORATE_LOCK,dual_sys,sim_lock_status->network_locked);
            break;
            
        case ID_SET_SECURITY_SIM_LOCK_USER:         
            MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_USER_LOCK,dual_sys,sim_lock_status->network_locked);
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#else
#ifndef MMI_GUI_STYLE_TYPICAL 
/*****************************************************************************/
// 	Description : Append SIMLock Switch List Item
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void AppendSIMLockSwitchListItem(
                                                MMI_CTRL_ID_T	    ctrl_id,
                                                BOOLEAN is_need_replace
                                                )
{
    uint16 index =0;	
    uint16 focus_index = 0;
    MMI_STRING_T  text_str = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MN_SIM_LOCK_STATUS_T *sim_lock_status= PNULL;
    
    if(MMIAPIPHONE_UpdateSIMUnlockData())//update SIM Lock data from PS
    {
        sim_lock_status= MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPHONE]InitSIMLockMenuControl get sim lock data fail!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_4159_112_2_18_2_51_58_315,(uint8*)"");
        return;
    }
    
    if(is_need_replace)
    {
        focus_index = GUILIST_GetCurItemIndex(ctrl_id); 
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id, MMI_SIM_LOCK_TYPE_NUM, FALSE );//max item 5 sim locks
    }
    //add items to list box
    if((is_need_replace && index == focus_index) || !is_need_replace )
    {
        if (sim_lock_status->network_locked)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }      
        AppendPdaSecuritySettingsItem2Line(ctrl_id,TXT_SET_SIM_LOCK_NETWORK,text_str, image_id, (uint32)index, is_need_replace);
    }
    index++;
    
    if((is_need_replace && index == focus_index) || !is_need_replace )
    {
        if (sim_lock_status->network_subset_locked)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }      
        AppendPdaSecuritySettingsItem2Line(ctrl_id,TXT_SET_SIM_LOCK_NETWORK_SUBSET,text_str, image_id, (uint32)index, is_need_replace);
    }
    index++;
    
    if((is_need_replace && index == focus_index) || !is_need_replace )
    {
        if (sim_lock_status->sp_locked)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }      
        AppendPdaSecuritySettingsItem2Line(ctrl_id,TXT_SET_SIM_LOCK_SP,text_str, image_id, (uint32)index, is_need_replace);
    }
    index++;
    
    if((is_need_replace && index == focus_index) || !is_need_replace )
    {
        if (sim_lock_status->corporate_locked)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }      
        AppendPdaSecuritySettingsItem2Line(ctrl_id,TXT_SET_SIM_LOCK_CORPORATE,text_str, image_id, (uint32)index, is_need_replace);
    }
    index++;
    
    if((is_need_replace && index == focus_index) || !is_need_replace )
    {
        if (sim_lock_status->user_locked)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
           image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }      
        AppendPdaSecuritySettingsItem2Line(ctrl_id,TXT_SET_SIM_LOCK_USER,text_str, image_id, (uint32)index, is_need_replace);
    }
    index++;
    
}

/*****************************************************************************/
//  Description : to Handle SIM Lock Menu switch Window
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockMenuWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T ctrl_id = MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendSIMLockSwitchListItem(ctrl_id, FALSE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
        AppendSIMLockSwitchListItem(ctrl_id, TRUE);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32 sim_lock_type = 1;
            uint16 index = 0;
            BOOLEAN is_curr_simlock_type_locked = FALSE;
            
            index = GUILIST_GetCurItemIndex(ctrl_id);
            
            sim_lock_type  = sim_lock_type<< index;
            is_curr_simlock_type_locked = IsSIMLockOn( sim_lock_type);
            MMIAPIPHONE_HandleSIMLockSettingMenuFlow(sim_lock_type, dual_sys, is_curr_simlock_type_locked);
        }
        break;
        
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_APP_RED:
        recode = MMI_RESULT_FALSE;
        break;
        
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        recode = MMI_RESULT_FALSE;
        break;
#endif        
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#else
/*****************************************************************************/
// 	Description : to handle security setting SIM Lock menu
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockMenuWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E				 recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                ctrl_id = MMISET_SECURITY_SIM_LOCK_FORM_CTRL_ID;
    uint32                       dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    MN_SIM_LOCK_STATUS_T *sim_lock_status= MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitSIMLockMenuControl(win_id, dual_sys);
        MMK_SetAtvCtrl(win_id, MMISET_SECURITY_SIM_LOCK_NETWORK_EDIT_LABEL_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_FULL_PAINT:
        UpdateSIMLockEditControl(ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(msg_id==MSG_CTL_PENOK)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        switch(ctrl_id)
        {
         case MMISET_SECURITY_SIM_LOCK_NETWORK_EDIT_LABEL_CTRL_ID:
            {
                MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_NETWORK_LOCK,dual_sys,sim_lock_status->network_locked);
            }
            break;
            
         case MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_EDIT_LABEL_CTRL_ID:
            {
                MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_NETWORK_SUBSET_LOCK,dual_sys,sim_lock_status->network_subset_locked);
            }
            break;
            
         case MMISET_SECURITY_SIM_LOCK_SP_EDIT_LABEL_CTRL_ID:
            {
                MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_SP_LOCK,dual_sys,sim_lock_status->sp_locked);
            }
            break;
            
         case MMISET_SECURITY_SIM_LOCK_CORPORATE_EDIT_LABEL_CTRL_ID:
            {
                MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_CORPORATE_LOCK,dual_sys,sim_lock_status->corporate_locked);
            }
            break;
            
         case MMISET_SECURITY_SIM_LOCK_USER_EDIT_LABEL_CTRL_ID:
            {
                MMIAPIPHONE_HandleSIMLockSettingMenuFlow(MMI_SIM_USER_LOCK,dual_sys,sim_lock_status->user_locked);
            }
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
// 	Description : to init sim lock control 
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void InitSIMLockMenuControl(MMI_WIN_ID_T win_id, uint32 dual_sys)
{
    MMI_STRING_T network_label_name  = {0};
    MMI_STRING_T networ_subset_label_name  = {0};
    MMI_STRING_T sp_label_name  = {0};
    MMI_STRING_T corporate_label_name  = {0};
    MMI_STRING_T user_label_name  = {0};
    MMI_STRING_T label_name  = {0};

    MN_SIM_LOCK_STATUS_T *sim_lock_status= PNULL;
    if(MMIAPIPHONE_UpdateSIMUnlockData())//update SIM Lock data from PS
    {
        sim_lock_status= MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPHONE]InitSIMLockMenuControl get sim lock data fail!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_4159_112_2_18_2_51_58_315,(uint8*)"");
        return;
    }
    //network
    GUIFORM_SetStyle(MMISET_SECURITY_SIM_LOCK_NETWORK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SIM_LOCK_NETWORK, &network_label_name);
    GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_LABEL_CTRL_ID, &network_label_name, FALSE);  
    if (sim_lock_status->network_locked)
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //network subset
    GUIFORM_SetStyle(MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SIM_LOCK_NETWORK_SUBSET, &networ_subset_label_name);
    GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_LABEL_CTRL_ID, &networ_subset_label_name, FALSE);  
    if (sim_lock_status->network_subset_locked)
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //Service Provider
    GUIFORM_SetStyle(MMISET_SECURITY_SIM_LOCK_SP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SIM_LOCK_SP, &sp_label_name);
    GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_SP_LABEL_CTRL_ID, &sp_label_name, FALSE);  
    if (sim_lock_status->sp_locked)
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_SP_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_SP_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //corporate
    GUIFORM_SetStyle(MMISET_SECURITY_SIM_LOCK_CORPORATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SIM_LOCK_CORPORATE, &corporate_label_name);
    GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_CORPORATE_LABEL_CTRL_ID, &corporate_label_name, FALSE);  
    if (sim_lock_status->corporate_locked)
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_CORPORATE_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_CORPORATE_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //user
    GUIFORM_SetStyle(MMISET_SECURITY_SIM_LOCK_USER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SIM_LOCK_USER, &user_label_name);
    GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_USER_LABEL_CTRL_ID, &user_label_name, FALSE);  
    if (sim_lock_status->user_locked)
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_USER_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_USER_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
}
#endif

/*****************************************************************************/
// 	Description : to update sim lock edit control 
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void UpdateSIMLockEditControl(MMI_CTRL_ID_T ctrl_id)
{
    MMI_STRING_T label_name  = {0};
    MN_SIM_LOCK_STATUS_T *sim_lock_status= PNULL;
    
    if(MMIAPIPHONE_UpdateSIMUnlockData())//update sim Lock data from PS layer
    {
        sim_lock_status = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPHONE]UpdateSIMLockEditControl get sim lock data fail!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_4249_112_2_18_2_51_59_316,(uint8*)"");
        return;
    }

    //network
    if (sim_lock_status->network_locked)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //network subset
    if (sim_lock_status->network_subset_locked)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_NETWORK_SUBSET_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //Service Provider
    if (sim_lock_status->sp_locked)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_SP_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_SP_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //corporate
    if (sim_lock_status->corporate_locked)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_CORPORATE_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_CORPORATE_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
    //user
    if (sim_lock_status->user_locked)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_USER_EDIT_LABEL_CTRL_ID, &label_name, FALSE);  
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_SIM_LOCK_USER_EDIT_LABEL_CTRL_ID, &label_name, FALSE); 
    }
}

#endif
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
/*****************************************************************************/
// 	Description : make a judgement that if key lock password is on.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsKLPswOn(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    BOOLEAN is_on = FALSE;

    MMINV_READ(MMINV_SET_KL_PSW_ON, &is_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = FALSE;
        MMINV_WRITE(MMINV_SET_KL_PSW_ON, &is_on);
    }
    return is_on;
}

/*****************************************************************************/
// 	Description : Set key lock password on or off.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKLPswOnOff(BOOLEAN is_on)
{
    MMINV_WRITE(MMINV_SET_KL_PSW_ON, &is_on);
}

/*****************************************************************************/
// 	Description : Get key lock password from NV.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetKLPsw(uint8 *psw)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint8 password[MMI_KL_PSW_LEN + 1] = {0};
    MMINV_READ(MMINV_SET_KL_PSW_CODE, psw, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_KL_PSW_CODE, password);
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : Save key lock password to NV.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKLPsw(uint8 *psw)
{
    MMINV_WRITE(MMINV_SET_KL_PSW_CODE, psw);    
}

/*****************************************************************************/
// 	Description : 开机密码出厂设置
//	Global resource dependence : 
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetKLPsw(void)
{
    uint8 psw1[MMI_KL_PSW_LEN] = {0};

    MMIAPISET_SetKLPswOnOff(FALSE);
    MMIAPISET_SetKLPsw(psw1);
}


/*****************************************************************************/
// 	Description : Open key lock password setting windows.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenKLPswSettingWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_KL_PSW_SETTING_WIN_TAB, PNULL);
}

/******************************************************************************/
//  Description : Create key lock psw settting window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void OpenKLPswSettingEnterWin(void)
{
    MMK_CreateWin((uint32*)MMIKC_PSW_INPUT_WINTAB, (ADD_DATA)MMI_KL_OPEN_PSW_INPUT_ENTER);
}

/******************************************************************************/
//  Description : Create key lock psw settting window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void OpenKLPswSettingSwitchOffWin(void)
{
    MMK_CreateWin((uint32*)MMIKC_PSW_INPUT_WINTAB, (ADD_DATA)MMI_KL_OPEN_PSW_INPUT_SWITCHOFF);
}

/******************************************************************************/
//  Description : Create key lock psw settting window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void OpenKLPswSettingChangeWin(void)
{
    MMK_CreateWin((uint32*)MMIKC_PSW_INPUT_WINTAB, (ADD_DATA)MMI_KL_OPEN_PSW_INPUT_CHANGE);
}

LOCAL void  InitKLPswSettingListCtrl(void)
{
#if 0
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    BOOLEAN flag = FALSE;
    MMI_STRING_T  text_str = {0};
    MMI_TEXT_ID_T  text_id = TXT_NULL;
#endif    
    MMI_CTRL_ID_T       ctrl_id = MMISET_SECURITY_KL_PSW_LIST_CTRL_ID;

#if 0
     uint32 text_arr_id[6]=
     {
        TXT_CLOSE,
        TXT_SET_TIME_5S,
        TXT_SET_TIME_15S,
        TXT_SET_TIME_30S,
        TXT_COMM_1_MINUTE,
        TXT_COMM_5_MINUTES
     };
#endif

    GUILIST_SetMaxItem(ctrl_id, 4, FALSE );   

    AppendPdaSecuritySettingsItem1Line(            
    ctrl_id,
    TXT_SET_KL_PSW_TURN_OFF,
    (uint32)0);

    AppendPdaSecuritySettingsItem1Line(            
    ctrl_id,
    TXT_SET_KL_PSW_CHANGE,
    (uint32)0);
#if 0
   switch (MMIAPISET_GetAutoBoardKey())
    {
    case MMISET_KEYLOCK_CLOSE:
        text_id = text_arr_id[0];
        break;
    
    case MMISET_KEYLOCK_5_SEC:
        text_id = text_arr_id[1];
        break;
    
    case MMISET_KEYLOCK_15_SEC:
        text_id = text_arr_id[2];
        break;
    
    case MMISET_KEYLOCK_30_SEC:
        text_id = text_arr_id[3];
        break;
    
    case MMISET_KEYLOCK_1_MIN:
        text_id = text_arr_id[4];
        break;
        
    case MMISET_KEYLOCK_5_MIN:
        text_id = text_arr_id[5];
        break;
        
    default:
        SCI_TRACE_LOW("AppendAutoKeylockTimeListItem:keylock time is error!");
        break;
    }

    MMI_GetLabelTextByLang(text_id , &text_str);     
    AppendPdaSecuritySettingsItem2Line(            
            ctrl_id,
            TXT_SET_KL_PSW_WAITING_TIME,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)0
            );
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 


    flag = MMIAPISET_GetPowerOnPwdStatus();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSecuritySettingsItem2Line(            
    ctrl_id,
    TXT_SET_KL_PSW_ERASE_DATA,
    text_str,
    image_id,
    (uint32)0);
#endif    
}

/******************************************************************************/
//  Description : handle kl password setting.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLPswSettingWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SECURITY_KL_PSW_LIST_CTRL_ID;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitKLPswSettingListCtrl();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_GET_FOCUS:
        GUILIST_RemoveAllItems(ctrl_id);
        InitKLPswSettingListCtrl();
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint16 index = 0;
            index = GUILIST_GetCurItemIndex(ctrl_id);
            if(s_kl_psw_func_arr[index] != PNULL)
            {
                ((KL_PSW_INPUT_FUNC_T)(s_kl_psw_func_arr[index]))();
            }
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#endif                    
#endif
/*****************************************************************************/
//  Description : open security setting window
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterSecuritySettingWin(void)
{
    MMK_CreateWin((uint32*)MMISET_SET_SECURITY_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : handle validate privacy protect password win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleValidatePrivacyProtectPwd(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    MMI_STRING_T                old_pwd_value   =   {0};  
    MMI_RESULT_E                result          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_VALIDATE_PRIVACY_PWD_CTRL_ID;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMISET_PRIVACY_CALLBACK_DATA* p_call_back_data = (MMISET_PRIVACY_CALLBACK_DATA*)MMK_GetWinAddDataPtr(win_id);
    static BOOLEAN s_pwd_is_validated = FALSE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
        MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMISET_PRIVACY_PROTECT_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(ctrl_id,&old_pwd_value);
        
        if (PRIVACY_PROTECT_PWD_LEN_IS_IN_RANGE(old_pwd_value.wstr_len))
        {
            //密码是否正确          
            if (MMISET_IsCorrectPrivacyProtectPwd( old_pwd_value.wstr_ptr,old_pwd_value.wstr_len))
            {
                s_pwd_is_validated = TRUE;
                MMIAPISET_SetIsNeedValidatePrivacy(FALSE);
                 if (PNULL!= p_call_back_data && PNULL != p_call_back_data->call_back_func)
                {
                    p_call_back_data->call_back_func(TRUE, p_call_back_data->param_ptr);
                }
                MMK_CloseWin(win_id);
            }
            else
            {
                //提示密码错误!
                MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                
                //清空password
                GUIEDIT_ClearAllStr(ctrl_id);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        s_pwd_is_validated = TRUE;
        if (PNULL!= p_call_back_data && PNULL != p_call_back_data->call_back_func)
        {
            p_call_back_data->call_back_func(FALSE, p_call_back_data->param_ptr);
        }
		//防止隐私保护mp3时，cancel隐私保护界面，mp3的信息没有释放 bug142106  Bug 177888
		
        if(MMIMP3_PLAYER_STATE_NONE == MMIAPIMP3_GetPlayerState())		
        {		
            MMIAP_DestroyAPInfo();
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (!s_pwd_is_validated)
        {
            if (PNULL!= p_call_back_data && PNULL != p_call_back_data->call_back_func)
            {
                p_call_back_data->call_back_func(FALSE, p_call_back_data->param_ptr);
            }
        }
        
        if (PNULL != p_call_back_data)
        {
            if (PNULL != p_call_back_data->param_ptr)
            {
                SCI_FREE(p_call_back_data->param_ptr);
            }
            SCI_FREE(p_call_back_data);
        }
        MMIAPISET_ResetValidatePrivacyType();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open validate privacy password window
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_OpenVailidatePwdWin(
                                       MMISET_PRIVACY_CALLBACK_DATA* p_call_back_data
                                       )
{
     //此处进行判断，如果有一个密码验证窗口，则关闭，并且调用其call_back，处理失败情形
    if (MMK_IsOpenWin(MMISET_VALIDATE_PRIVACY_PWD_WIN_ID))
    {
        MMK_CloseWin(MMISET_VALIDATE_PRIVACY_PWD_WIN_ID);
    }
    
    MMK_CreatePubEditWin((uint32*)MMISET_VALIDATE_PRIVACY_PWD_WIN_TAB,(ADD_DATA)p_call_back_data);
}

/*****************************************************************************/
//     Description : 添加锁屏开启时间到radio list的item
// Global resource dependence : 
//  Author: apple.zhang
// Note:
/*****************************************************************************/
LOCAL void AppendAutoKeylockTimeListItem(MMI_CTRL_ID_T      ctrl_id)
{
    uint16                      cur_selection   =   0;
    MMISET_KEYLOCK_TYPE_E       type = MMIAPISET_GetAutoBoardKey();
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_5S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_15S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_30S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_COMM_1_MINUTE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_COMM_5_MINUTES, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current keylock info   
    switch (type)
    {
    case MMISET_KEYLOCK_CLOSE:
        cur_selection = MMISET_KEYLOCK_SELECT_CLOSE;
        break;
        
#if defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
#else        
    case MMISET_KEYLOCK_5_SEC:
        cur_selection = MMISET_KEYLOCK_SELECT_5_SEC;
        break;
#endif

    case MMISET_KEYLOCK_15_SEC:
        cur_selection = MMISET_KEYLOCK_SELECT_15_SEC;
        break;
        
    case MMISET_KEYLOCK_30_SEC:
        cur_selection = MMISET_KEYLOCK_SELECT_30_SEC;
        break;
        
    case MMISET_KEYLOCK_1_MIN:
        cur_selection = MMISET_KEYLOCK_SELECT_1_MIN;
        break;
    case MMISET_KEYLOCK_5_MIN:
        cur_selection = MMISET_KEYLOCK_SELECT_5_MIN;
        break;
    default:
        //SCI_TRACE_LOW:"AppendAutoKeylockTimeListItem:keylock type is %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_4720_112_2_18_2_52_0_317,(uint8*)"d",type);
        break;
    }    
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
/*****************************************************************************/
//  Description : 设定自动锁键盘开启时间
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL void SetAudoKeyLockTime(MMI_CTRL_ID_T     ctrl_id)
{
    uint16                       cur_selection          =   0;
    MMISET_KEYLOCK_TYPE_E        auto_boardkey_time     =   MMISET_KEYLOCK_CLOSE;
    
    SCI_MEMSET(&auto_boardkey_time, 0, sizeof(MMISET_KEYLOCK_TYPE_E));
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    //SCI_ASSERT( (MMISET_KEYLOCK_MAX_ITEM > cur_selection));
   if(MMISET_KEYLOCK_MAX_ITEM <= cur_selection)
   {
        //SCI_TRACE_LOW:"mmisetting_assert SetAudoKeyLockTime cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_4747_112_2_18_2_52_0_318,(uint8*)"d", cur_selection);
        return;
    }     
    switch (cur_selection)
    {
        case MMISET_KEYLOCK_SELECT_CLOSE:
            auto_boardkey_time = MMISET_KEYLOCK_CLOSE;
            break;
            
#if defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
#else        
        case MMISET_KEYLOCK_SELECT_5_SEC:
            auto_boardkey_time = MMISET_KEYLOCK_5_SEC;
            break;
#endif

        case MMISET_KEYLOCK_SELECT_15_SEC:
            auto_boardkey_time = MMISET_KEYLOCK_15_SEC;
            break;
            
        case MMISET_KEYLOCK_SELECT_30_SEC:
            auto_boardkey_time = MMISET_KEYLOCK_30_SEC;
            break;
            
        case MMISET_KEYLOCK_SELECT_1_MIN:
            auto_boardkey_time = MMISET_KEYLOCK_1_MIN;
            break;
            
        case MMISET_KEYLOCK_SELECT_5_MIN:
            auto_boardkey_time = MMISET_KEYLOCK_5_MIN;
            break;
            
        default:
            break;
    }
    MMISET_SetAutoBoardKey(auto_boardkey_time);
}

/*****************************************************************************/
//  Description : full paint privacy protect list
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void RefreshPrivacyProtectListItem(
                                         MMI_CTRL_ID_T      ctrl_id
                                         )
{
    uint16          i =0,cur_selection = 0;     
    uint16          selected_num = 0;
    uint16          selected_item[MMISET_PROTECT_MAX] = {0};
    
    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,selected_item,MMISET_PROTECT_MAX);
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    
    if (0 == cur_selection && 0 == selected_num)
    {
        GUILIST_SetSelectedItem( ctrl_id,cur_selection, TRUE );
    }
    
    //get selected item info from control
    for ( i = 0; i < selected_num; i++ )
    {
        GUILIST_SetSelectedItem( ctrl_id,selected_item[i],TRUE );
    }
    
    for ( i = 0; i < selected_num; i++ )
    {
        if ( cur_selection == selected_item[i] )
        {
            GUILIST_SetSelectedItem( ctrl_id, cur_selection, FALSE );
            break;
        }
        else
        {
            GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);
        }
    }
}
/*****************************************************************************/
//  Description : to handle select privacy protect item win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPrivacyProtectWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_PRIVACY_PROTECT_ITEM_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_PROTECT_MAX, FALSE );//max item 9
        GUILIST_SetMaxSelectedItem(ctrl_id,MMISET_PROTECT_MAX);
        MMISET_AppendPrivacyProtectListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // MMK_CreateWin((uint32*)MMISET_INPUT_PRIVACY_PROTRCT_PWD_WIN_TAB,PNULL);
        break;
        
    case MSG_SET_CANCEL_SET_PRIVACY_PROTECT:
        RefreshPrivacyProtectListItem(ctrl_id);
        break; 
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMISET_SetPrivacyProtectInfo(ctrl_id);
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_APP_RED:
        MMISET_SetPrivacyProtectInfo(ctrl_id);
        recode = MMI_RESULT_FALSE;
        break;
        
    case MSG_CLOSE_WINDOW:
        MMISET_SetPrivacyProtectInfo(ctrl_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        MMISET_SetPrivacyProtectInfo(ctrl_id);
        recode = MMI_RESULT_FALSE;
        break;
#endif        
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#if defined(MMI_MSD_SUPPORT)
/*****************************************************************************/
//  Description : to INITIAL MSD CONTROL
//  Global resource dependence : 
//  Author:MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL void InitMsdControl(void)
{
    MMI_STRING_T label_name  = {0};
    #ifndef MMI_PDA_SUPPORT
    GUIFORM_SetStyle(MMISET_SECURITY_MSD_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    #endif
    MMI_GetLabelTextByLang(TXT_COMMON_STATE, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_MSD_LABEL_CTRL_ID, &label_name, FALSE);
    #ifndef MMI_PDA_SUPPORT    
    GUIFORM_SetStyle(MMISET_SECURITY_MSD_MODIFY_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    #endif
    MMI_GetLabelTextByLang(TXT_COMMON_MODIFY_PASSWORD , &label_name);
    GUILABEL_SetText(MMISET_SECURITY_MODIFY_MODIFY_PASSWORD_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetMsdStatusList(void)
{

    MMI_STRING_T label_name  = {0};

    if(MMIAPIMSD_IsMsdStartup())
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_MSD_OPENCLOSE_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_MSD_OPENCLOSE_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }
}
#endif//#if defined(MMI_MSD_SUPPORT)

/*****************************************************************************/
//  Description : to handle security setting msd menu
//  Global resource dependence : 
//  Author:wangxf
//  Note: 
/*****************************************************************************/
#ifdef DPHONE_SUPPORT
#if defined(MMI_MSD_SUPPORT)
LOCAL MMI_RESULT_E  HandleMsdMenuWindow(
   MMI_WIN_ID_T    win_id, 
   MMI_MESSAGE_ID_E   msg_id, 
   DPARAM             param
   )
{
   MMI_MENU_GROUP_ID_T          group_id   =   0;
   MMI_MENU_ID_T                menu_id    =   0;
   MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
   MMI_CTRL_ID_T               ctrl_id     =   MMISET_SET_SECURITY_MSD_CTRL_ID;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
       MMK_SetAtvCtrl(win_id, ctrl_id);
       GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
       break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
   case MSG_CTL_OK:
   case MSG_APP_OK:
   case MSG_APP_WEB:
       GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
       switch (menu_id)
       {
       case ID_MSD_SETUP:
           MMIAPIMSD_OpenSetupInputPswWin();
           break;
           
       case ID_MSD_CLOSE:
           MMIAPIMSD_OpenCloseInputPswWin();
           break;
           
       case ID_MSD_MODIFY_PSW:
           MMIAPIMSD_ModPswInputPswWin();
           break;
           
       default:
           break;
       }
       break;
       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
           MMK_CloseWin(win_id);
           break;
           
       default:
           recode = MMI_RESULT_FALSE;
           break;
   }
   
   return recode;
}
#endif//#if defined(MMI_MSD_SUPPORT)
#else
#if !defined MMI_GUI_STYLE_TYPICAL
PUBLIC void AppendPdaSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;
#ifndef MMI_PDA_SUPPORT
    item_data.softkey_id[0] = STXT_SELECT;
    item_data.softkey_id[2] = STXT_RETURN;  
#endif
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void AppendPdaSettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[MMI_LIST_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_len = 0;
    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        MMIAPICOM_Wstrncpy(temp_wstr,
                                          text_str.wstr_ptr,
                                          text_str.wstr_len);
        temp_len = text_str.wstr_len;
    }

    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
	 item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    }
//    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }
    
    //anim
    item_data.item_content[1].item_data_type  = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
#ifndef MMI_PDA_SUPPORT
    if(image_id == IMAGE_CHECK_SELECTED_ICON)
    {
        item_data.softkey_id[0] = STXT_CANCEL;
        item_data.softkey_id[2] = STXT_RETURN;
    }
    else
    {
        item_data.softkey_id[0] = STXT_SELECT;
        item_data.softkey_id[2] = STXT_RETURN;        
    }
#endif
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description :InitPdaFDNSettingsListCtrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaFDNSettingsListCtrl(MN_DUAL_SYS_E dual_sys)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    MMI_CTRL_ID_T       ctrl_id = MMISET_SECURITY_FDN_FORM_CTRL_ID;
    MMI_STRING_T  text_str = {0};
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL; 
    uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    
    GUILIST_SetMaxItem(ctrl_id, 2, FALSE );       

    fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys); 

    if(fdn_context_ptr->is_fdn_enable)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_FDN_MODE,
    text_str,
    image_id,
    (uint32)0);
    
    AppendPdaSettingsItem1Line(            
    ctrl_id,
    TXT_FDN_NUM,
    (uint32)0);

    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
}

#if defined(MMI_MSD_SUPPORT)
/*****************************************************************************/
//  Description :InitPdaFDNSettingsListCtrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMSDSettingsListCtrl(void)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMISET_SECURITY_MSD_FORM_CTRL_ID;
    MMI_STRING_T  text_str = {0};

    GUILIST_SetMaxItem(ctrl_id, 2, FALSE );   
    
    flag = MMIAPIMSD_IsMsdStartup();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_COMMON_STATE,
    text_str,
    image_id,
    (uint32)0);
    
    AppendPdaSettingsItem1Line(            
    ctrl_id,
    TXT_COMMON_MODIFY_PASSWORD ,
    (uint32)0);

}
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description :InitPdaBlackLIstSettingsListCtrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaBlackLIstSettingsListCtrl(void)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID;
    MMI_STRING_T  text_str = {0};

    GUILIST_SetMaxItem(ctrl_id, 3, FALSE );   
    
    flag =  MMIAPISET_GetIsBlacklistAudioCall();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_BLACKLIST_TYPE_AUDIOCALL,
    text_str,
    image_id,
    (uint32)0);

#ifdef VT_SUPPORT
    flag =  MMIAPISET_GetIsBlacklistVedioCall();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_BLACKLIST_TYPE_VEDIOCALL,
    text_str,
    image_id,
    (uint32)0);
#endif


    flag = MMIAPISET_GetIsBlacklistMessage();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }      
    AppendPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_BLACKLIST_TYPE_MESSAGE,
    text_str,
    image_id,
    (uint32)0);
}
#endif

#if defined(MMI_MSD_SUPPORT)
LOCAL MMI_RESULT_E  HandleMsdMenuWindow(
   MMI_WIN_ID_T    win_id, 
   MMI_MESSAGE_ID_E   msg_id, 
   DPARAM             param
   )
{

   MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
   MMI_CTRL_ID_T               ctrl_id     =   MMISET_SECURITY_MSD_FORM_CTRL_ID;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
       InitPdaMSDSettingsListCtrl();
       MMK_SetAtvCtrl(win_id, ctrl_id);
       break;
  
    case MSG_GET_FOCUS:
    case MSG_SET_BLACKLIST_LIST_UPDATE:
        {
            uint16 index = GUILIST_GetCurItemIndex(ctrl_id);        
            GUILIST_RemoveAllItems(ctrl_id);
            InitPdaMSDSettingsListCtrl();
            GUILIST_SetCurItemIndex(ctrl_id,index);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
   case MSG_APP_OK:
   case MSG_CTL_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint16 index = 0;
            index = GUILIST_GetCurItemIndex(ctrl_id);
            if(0 == index)
            {
                if(MMIAPIMSD_IsMsdStartup())
                {
                     MMIAPIMSD_OpenCloseInputPswWin();
                }
                else
                {
                     MMIAPIMSD_OpenSetupInputPswWin();
                }
            }
            else if(1 == index)
            {
                MMIAPIMSD_ModPswInputPswWin();
            }        
        }
        break;

       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
           MMK_CloseWin(win_id);
           break;
           
       default:
           recode = MMI_RESULT_FALSE;
           break;
   }
   
   return recode;
}
#endif//#if defined(MMI_MSD_SUPPORT)
#else
#if defined(MMI_MSD_SUPPORT)
LOCAL MMI_RESULT_E  HandleMsdMenuWindow(
   MMI_WIN_ID_T    win_id, 
   MMI_MESSAGE_ID_E   msg_id, 
   DPARAM             param
   )
{

   MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
   MMI_CTRL_ID_T               ctrl_id     =   MMISET_SECURITY_MSD_OPENCLOSE_EDIT_LABEL_CTRL_ID;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
       InitMsdControl();
       MMK_SetAtvCtrl(win_id, ctrl_id);
       GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
       break;

   case MSG_FULL_PAINT:
       SetMsdStatusList();
       break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
   case MSG_APP_OK:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(msg_id==MSG_CTL_PENOK)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        switch(ctrl_id)
        {
         case MMISET_SECURITY_MODIFY_MODIFY_PASSWORD_EDIT_LABEL_CTRL_ID:
             MMIAPIMSD_ModPswInputPswWin();
             break;
         case MMISET_SECURITY_MSD_OPENCLOSE_EDIT_LABEL_CTRL_ID:
            if(MMIAPIMSD_IsMsdStartup())
            {
                 MMIAPIMSD_OpenCloseInputPswWin();
            }
            else
            {
                 MMIAPIMSD_OpenSetupInputPswWin();
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
           
       default:
           recode = MMI_RESULT_FALSE;
           break;
   }
   
   return recode;
}
#endif//#if defined(MMI_MSD_SUPPORT)
#endif
#endif 
/*****************************************************************************/
//  Description : to inital FDN Control
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitFDNControl(void)
{
    MMI_STRING_T label_name  = {0};
    #ifndef MMI_PDA_SUPPORT
    GUIFORM_SetStyle(MMISET_SECURITY_FDN_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    #endif
    MMI_GetLabelTextByLang(TXT_SET_FDN, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_SET_FDN_LABEL_CTRL_ID, &label_name, FALSE);
    #ifndef MMI_PDA_SUPPORT
    GUIFORM_SetStyle(MMISET_SECURITY_FDN_LIST_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    #endif
    MMI_GetLabelTextByLang(TXT_FDN_NUM, &label_name);
    GUILABEL_SetText(MMISET_SECURITY_FDN_LIST_EDIT_LABEL_CTRL_ID, &label_name, FALSE);   
}


LOCAL void SetFDNStatusList(MN_DUAL_SYS_E dual_sys)
{

    MMI_STRING_T label_name  = {0};
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
    
    fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys); 

    if(fdn_context_ptr->is_fdn_enable)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_ENABLE_ON, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_FDN__EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        GUILABEL_SetText(MMISET_SECURITY_FDN__EDIT_LABEL_CTRL_ID, &label_name, FALSE);
    }


}

/*****************************************************************************/
//  Description : to handle security setting fdn menu
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
#ifdef DPHONE_SUPPORT 
LOCAL MMI_RESULT_E  HandleFDNMenuWindow(
   MMI_WIN_ID_T    win_id, 
   MMI_MESSAGE_ID_E   msg_id, 
   DPARAM             param
   )
{
   MMI_MENU_GROUP_ID_T group_id = 0;
   MMI_MENU_ID_T menu_id = 0;
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = MMISET_SET_SECURITY_FDN_CTRL_ID;
   MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
       MMK_SetAtvCtrl(win_id, ctrl_id);
       GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
       break;
       
   case MSG_APP_OK:
   case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
   case MSG_CTL_OK:
       GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
       switch (menu_id)
       {
       case ID_FDN_STATE:
           /*打开FDN状态窗口,可使能/关闭FDN*/
           if (!MMIAPIPB_IsPbReady(FALSE))
           {
               MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
           }
           else
           {
               MMK_CreateWin((uint32*)MMISET_SET_FDN_STATE_WIN_TAB, (ADD_DATA)dual_sys);
           }
           break;
           
       case ID_FDN_NUM://固定号码
           if (!MMIAPIPB_IsPbReady(FALSE))
           {
               MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
           }
           else
           {
               //MMK_CreateWin((uint32*)MMIPB_FDN_LIST_WIN_TAB, (ADD_DATA)dual_sys);
               MMIAPIPB_OpenFDNContactListWin(dual_sys);
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
           
       default:
           recode = MMI_RESULT_FALSE;
           break;
   }
   
   return recode;
}
#else
#ifdef FDN_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL 
LOCAL MMI_RESULT_E  HandleFDNMenuWindow(
   MMI_WIN_ID_T    win_id, 
   MMI_MESSAGE_ID_E   msg_id, 
   DPARAM             param
   )
{

   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = MMISET_SECURITY_FDN_FORM_CTRL_ID;
   MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;    
    
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
       InitPdaFDNSettingsListCtrl(dual_sys);
       MMK_SetAtvCtrl(win_id, ctrl_id);        
       break;

    case MSG_GET_FOCUS:
    case MSG_SET_BLACKLIST_LIST_UPDATE:
         InitPdaFDNSettingsListCtrl(dual_sys);
        break;
  
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:              
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint16 index = 0;
            index = GUILIST_GetCurItemIndex(ctrl_id);
            if(0 == index)
            {
                    /*打开FDN状态窗口,可使能/关闭FDN*/
                   if (!MMIAPIPB_IsPbReady())
                   {
                       MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                   }
                   else
                   {
                       fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys); 
                      // MMK_CreateWin((uint32*)MMISET_SET_FDN_STATE_WIN_TAB, (ADD_DATA)dual_sys);
                       if(fdn_context_ptr->is_fdn_enable)
                       {
                            fdn_context_ptr->oper_type = MMIFDN_DISABLE_OPER;
                            if (!fdn_context_ptr->is_fdn_enable)
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_FDN_DEACTIVATED);
                            }
                            else
                            {
                                MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
                                MMIAPIPHONE_HandlePinAndPukFlow(
                                    MNSIM_PIN_VERIFY,
                                    MNSIM_PIN2,
                                    (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
                                    TRUE);
                            }
                       }
                       else
                       {
                             fdn_context_ptr->oper_type = MMIFDN_ENABLE_OPER;
                             if (fdn_context_ptr->is_fdn_enable)
                             {
                                //重复操作，直接调用成功窗口显示
                                MMIPUB_OpenAlertWarningWin(TXT_FDN_ACTIVATED);
                             }
                             else
                             {
                                MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
                                MMIAPIPHONE_HandlePinAndPukFlow(
                                    MNSIM_PIN_VERIFY,
                                    MNSIM_PIN2,
                                    (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
                                    TRUE);
                             }                  
                        }
                   }
            } 
            else if(1 == index)
            {
                if (!MMIAPIPB_IsPbReady())
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                }
                else
                {
                    //MMK_CreateWin((uint32*)MMIPB_FDN_LIST_WIN_TAB, (ADD_DATA)dual_sys);
                    MMIAPIPB_OpenFDNContactListWin(dual_sys);
                }
            }              
        }
        break;
        
       case MSG_FDN_PIN2_SUCCESS:
            fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);
            switch (fdn_context_ptr->oper_type)
            {
            case MMIFDN_ENABLE_OPER:
                //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService true"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5569_112_2_18_2_52_1_319,(uint8*)"");
#if defined (MODEM_SUPPORT_SPRD)
                err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, TRUE);//使能FDN
#elif defined (MODEM_SUPPORT_RDA)
                err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, TRUE, P_APP);//使能FDN
#else
    #error
#endif

break;
        
            case MMIFDN_DISABLE_OPER:
                //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService false"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5574_112_2_18_2_52_1_320,(uint8*)"");
#if defined (MODEM_SUPPORT_SPRD)
                err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, FALSE);//关闭FDN
#elif defined (MODEM_SUPPORT_RDA)
                err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, FALSE, P_APP);//关闭FDN
#else
    #error
#endif
                break;
       
           default:
               break;
           }
           //SCI_TRACE_LOW:"HandleSetFDNStateWindow err_code=%d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5581_112_2_18_2_52_1_321,(uint8*)"d", err_code);
    
            if (ERR_MNDATAMAG_NO_ERR != err_code)//不存在
            {
                MMIPUB_OpenAlertWarningWin( TXT_FDN_NOT_EXIST);
                MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
            }            
           break;      
       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
           MMK_CloseWin(win_id);
           break;
           
       default:
           recode = MMI_RESULT_FALSE;
           break;
   }
   
   return recode;
}
#else
LOCAL MMI_RESULT_E  HandleFDNMenuWindow(
   MMI_WIN_ID_T    win_id, 
   MMI_MESSAGE_ID_E   msg_id, 
   DPARAM             param
   )
{

   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = MMISET_SECURITY_FDN__EDIT_LABEL_CTRL_ID;
   MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;    
    
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
       InitFDNControl();
       MMK_SetAtvCtrl(win_id, ctrl_id);
       GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
       break;

   case MSG_FULL_PAINT: 
          SetFDNStatusList(dual_sys);
       break;
       
   
   case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
   case MSG_CTL_OK:
   case MSG_APP_OK:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(msg_id==MSG_CTL_PENOK)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
       switch (ctrl_id)
       {
       case MMISET_SECURITY_FDN__EDIT_LABEL_CTRL_ID:
           /*打开FDN状态窗口,可使能/关闭FDN*/
           if (!MMIAPIPB_IsPbReady())
           {
               MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
           }
           else
           {
               fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys); 
              // MMK_CreateWin((uint32*)MMISET_SET_FDN_STATE_WIN_TAB, (ADD_DATA)dual_sys);
               if(fdn_context_ptr->is_fdn_enable)
               {
                    fdn_context_ptr->oper_type = MMIFDN_DISABLE_OPER;
                    if (!fdn_context_ptr->is_fdn_enable)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_FDN_DEACTIVATED);
                    }
                    else
                    {
                        MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
                        MMIAPIPHONE_HandlePinAndPukFlow(
                            MNSIM_PIN_VERIFY,
                            MNSIM_PIN2,
                            (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
                            TRUE);
                    }
               }
               else
               {
                     fdn_context_ptr->oper_type = MMIFDN_ENABLE_OPER;
                     if (fdn_context_ptr->is_fdn_enable)
                     {
                        //重复操作，直接调用成功窗口显示
                        MMIPUB_OpenAlertWarningWin(TXT_FDN_ACTIVATED);
                     }
                     else
                     {
                        MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
                        MMIAPIPHONE_HandlePinAndPukFlow(
                            MNSIM_PIN_VERIFY,
                            MNSIM_PIN2,
                            (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
                            TRUE);
                     }                  
                }
           }
       
           break;            
       case MMISET_SECURITY_FDN_LIST_EDIT_LABEL_CTRL_ID://固定号码
           if (!MMIAPIPB_IsPbReady())
           {
               MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
           }
           else
           {
              // MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
               //MMK_CreateWin((uint32*)MMIPB_FDN_LIST_WIN_TAB, (ADD_DATA)dual_sys);
               MMIAPIPB_OpenFDNContactListWin(dual_sys);
           }
           break;          
       default:
           break;
       }
        break;

       case MSG_FDN_PIN2_SUCCESS:
            fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);
            switch (fdn_context_ptr->oper_type)
            {
            case MMIFDN_ENABLE_OPER:
                //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService true"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5712_112_2_18_2_52_2_322,(uint8*)"");
                err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, TRUE);//使能FDN
                break;
        
            case MMIFDN_DISABLE_OPER:
                //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService false"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5717_112_2_18_2_52_2_323,(uint8*)"");
                err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, FALSE);//关闭FDN
                break;
       
           default:
               break;
           }
           //SCI_TRACE_LOW:"HandleSetFDNStateWindow err_code=%d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5724_112_2_18_2_52_2_324,(uint8*)"d", err_code);
    
            if (ERR_MNDATAMAG_NO_ERR != err_code)//不存在
            {
                MMIPUB_OpenAlertWarningWin( TXT_FDN_NOT_EXIST);
                MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
            }            
           break;      
       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
           MMK_CloseWin(win_id);
           break;
           
       default:
           recode = MMI_RESULT_FALSE;
           break;
   }

   
   return recode;
}
#endif
#endif 
#endif
/*****************************************************************************/
//  Description : AppendFDNStateListItem
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void AppendFDNStateListItem(MMI_CTRL_ID_T ctrl_id)
{
   //add items to listbox
   MMIAPISET_AppendListItemByTextId(TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
   MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
}

/*****************************************************************************/
//  Description : to handle set fdn state
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFDNStateWindow(
    MMI_WIN_ID_T    win_id, 
    MMI_MESSAGE_ID_E   msg_id, 
    DPARAM             param
    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    uint16 cur_selection = 0;
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
    
    fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys); 
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        GUILIST_SetMaxItem(MMISET_SET_FDN_STATE_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE);//max item 2
        
        AppendFDNStateListItem(MMISET_SET_FDN_STATE_CTRL_ID);

        if (fdn_context_ptr->is_fdn_enable)
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }

        //set selected item
        GUILIST_SetSelectedItem(MMISET_SET_FDN_STATE_CTRL_ID, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(MMISET_SET_FDN_STATE_CTRL_ID, cur_selection);

        GUILIST_SetOwnSofterKey(MMISET_SET_FDN_STATE_CTRL_ID, TRUE);
        
        MMK_SetAtvCtrl(win_id, MMISET_SET_FDN_STATE_CTRL_ID);
        break;
        
    case MSG_GET_FOCUS:
        if (fdn_context_ptr->is_fdn_enable)
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }

        //set selected item
        GUILIST_SetSelectedItem(MMISET_SET_FDN_STATE_CTRL_ID, cur_selection, TRUE);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
        if (!MMIAPIPB_IsPbReady())
        {
            MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
            break;
        }
        
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_FDN_STATE_CTRL_ID);
        
        if (0 == cur_selection) //开启
        {
            fdn_context_ptr->oper_type = MMIFDN_ENABLE_OPER;
            if (fdn_context_ptr->is_fdn_enable)
            {
                //重复操作，直接调用成功窗口显示
                MMIPUB_OpenAlertWarningWin(TXT_FDN_ACTIVATED);
            }
            else
            {
                MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
                MMIAPIPHONE_HandlePinAndPukFlow(
                    MNSIM_PIN_VERIFY,
                    MNSIM_PIN2,
                    (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
                    TRUE);
            }
        }
        else//关闭
        {
            fdn_context_ptr->oper_type = MMIFDN_DISABLE_OPER;
            if (!fdn_context_ptr->is_fdn_enable)
            {
                MMIPUB_OpenAlertWarningWin(TXT_FDN_DEACTIVATED);
            }
            else
            {
                MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);
                MMIAPIPHONE_HandlePinAndPukFlow(
                    MNSIM_PIN_VERIFY,
                    MNSIM_PIN2,
                    (uint32)MMISET_SET_FDN_STATE_WIN_TAB,
                    TRUE);
            }
        }
        break;
        
    case MSG_FDN_PIN2_SUCCESS:
        switch (fdn_context_ptr->oper_type)
        {
        case MMIFDN_ENABLE_OPER:
            //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService true"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5869_112_2_18_2_52_2_325,(uint8*)"");
#if defined (MODEM_SUPPORT_SPRD)
            err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, TRUE);//使能FDN
#elif defined (MODEM_SUPPORT_RDA)
            err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, TRUE, P_APP);//使能FDN
#else
    #error
#endif

            break;
            
        case MMIFDN_DISABLE_OPER:
            //SCI_TRACE_LOW:"start PHONEBOOK_SetFDNService false"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5874_112_2_18_2_52_2_326,(uint8*)"");
#if defined (MODEM_SUPPORT_SPRD)
            err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, FALSE);//关闭FDN
#elif defined (MODEM_SUPPORT_RDA)
            err_code = PHONEBOOK_SetFDNServiceEx(dual_sys, FALSE, P_APP);//关闭FDN
#else
    #error
#endif
            break;
            
        default:
            //SCI_PASSERT(FALSE, ("fdn_context_ptr->oper_type=%d", fdn_context_ptr->oper_type));
            break;
        }
        
        //SCI_TRACE_LOW:"HandleSetFDNStateWindow err_code=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_5883_112_2_18_2_52_2_327,(uint8*)"d", err_code);
        
        if (ERR_MNDATAMAG_NO_ERR != err_code)//不存在
        {
            MMIPUB_OpenAlertWarningWin( TXT_FDN_NOT_EXIST);
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle security setting power on password menu
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePwdMenuWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_MENU_GROUP_ID_T          group_id   =   0;
    MMI_MENU_ID_T                menu_id    =   0;
    MMI_RESULT_E                result      =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_SET_SECURITY_PWD_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMISET_SET_SECURITY_PWD_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_PWD_OPEN:
            g_mmiset_pwd_type = MMISET_PWD_OPEN;
            if (MMIAPISET_GetPowerOnPwdStatus())
            {
                MMIPUB_OpenAlertWarningWin(TXT_PWD_ENABLED);
            }
            else
            {
                MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
            }
            break;
            
        case ID_SET_PWD_MODIFY:
            g_mmiset_pwd_type = MMISET_PWD_MODIFY;
            MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
            break;
            
        case ID_SET_PWD_CLOSE:
            g_mmiset_pwd_type = MMISET_PWD_CLOSE;
            if (MMIAPISET_GetPowerOnPwdStatus())
            {
                MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_PWD_WIN_TAB,PNULL);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_PWD_DISABLED);
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
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
/*****************************************************************************/
// 	Description : Power on password input window softkey
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN PwdInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
    BOOLEAN  result = FALSE;
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    MMI_STRING_T str_info = {0};

    //set string info
    str_info.wstr_ptr = str_ptr;
    str_info.wstr_len = str_len;

    is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MN_DUAL_SYS_1);//开机密码界面，未启动SIM卡，输入卡1即可
    if ((is_input_emergency_call) &&
        (str_len == emergency_call_len))
    {
        *text_id_ptr = TXT_SOS;
        result = TRUE;
    }
    else if (str_len >= MMISET_POWER_ON_PWD_MIN_LEN)
    {
        *text_id_ptr = TXT_COMMON_OK;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Make Emergency Call
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void MakeEmergencyCall(MMI_STRING_T *str_info)
{
    uint8 *emergency_call_ptr = PNULL;

    if (PNULL == str_info || PNULL == str_info->wstr_ptr || 0 == str_info->wstr_len)
    {
        return;
    }

    emergency_call_ptr = (uint8 *)SCI_ALLOCAZ(str_info->wstr_len + 1);
    if (PNULL == emergency_call_ptr)
    {
        return;
    }

    MMI_WSTRNTOSTR(emergency_call_ptr,
                                str_info->wstr_len,
                                str_info->wstr_ptr,
                                str_info->wstr_len,
                                str_info->wstr_len);

    MMIAPICC_MakeCall(
                                        MN_DUAL_SYS_1,
                                        emergency_call_ptr,
                                        (uint8)str_info->wstr_len,
                                        PNULL,
                                        PNULL,
                                        MMIAPICC_GetCallSimType(MN_DUAL_SYS_1),
                                        CC_CALL_NORMAL_CALL,
                                        PNULL
                                        );

    SCI_FREE(emergency_call_ptr);
    emergency_call_ptr = PNULL;
}
#endif
#ifndef MMI_SECURITYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : to handle security setting input old power on password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputPwdWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_STRING_T old_pwd_value = {0};  
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T edit_both_rect = {0};
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
#endif
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif
        GUIAPICTRL_SetBothRect(MMISET_SET_SECURITY_PWD_CTRL_ID, &edit_both_rect);

        if ((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
        {
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
            MMIPHONE_PowerOnPs(MN_DUAL_SYS_1); //开机密码界面，未启动SIM卡，启动卡1即可

            //set left softkey
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PwdInputWinSoftkey);
#else
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
#endif

#if defined(MMI_PDA_SUPPORT) && defined(MMIPUBWIN_PDASTYLE_SUPPORT)
            // PUBEDIT负责softkey的显示，PDA下需要禁止右键
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,2,0,TXT_NULL,TXT_NULL,PNULL);
#else
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,2,1,TXT_NULL,TXT_COMMON_CLEAR,PNULL);
#endif
        }
        else
        {
            //set left softkey
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        }
        MMK_SetAtvCtrl(win_id, MMISET_SET_SECURITY_PWD_CTRL_ID);
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示,输入开机密码
        MMISUB_IsPermitUpdate(TRUE);
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_PWD,PNULL);
#endif
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif
        break;

#if 0
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T str_info = {0};
            GUIEDIT_GetString(MMISET_SET_SECURITY_PWD_CTRL_ID, &str_info);	

            if ((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
            {            
                is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MN_DUAL_SYS_1);
                if ((is_input_emergency_call) &&
                    (str_info.wstr_len == emergency_call_len))
                {    
                    MMIPUB_SetWinSoftkey(win_id, TXT_SOS, TXT_NULL,TRUE);
                }
                else if (str_info.wstr_len >= MMISET_POWER_ON_PWD_MIN_LEN)
                {
                    MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, TXT_NULL,TRUE);
                }
                else
                {
                    MMIPUB_SetWinSoftkey(win_id, TXT_NULL, TXT_NULL,TRUE);
                }
            }
        }
        break;
#endif
#endif
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
#endif
        // get the value about PIN
        GUIEDIT_GetString(MMISET_SET_SECURITY_PWD_CTRL_ID,&old_pwd_value);
        
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
        is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&old_pwd_value, &emergency_call_len, MN_DUAL_SYS_1);

        if (((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
            && (is_input_emergency_call)
            && (old_pwd_value.wstr_len == emergency_call_len))
        {
            MakeEmergencyCall(&old_pwd_value);
        }
        else if (MSG_APP_GREEN != msg_id
               && MSG_APP_CALL2 != msg_id
               && MSG_APP_CALL3 != msg_id
               && MSG_APP_CALL4 != msg_id)
#endif
        {
            if (PHONE_CODE_LEN_IS_IN_RANGE(old_pwd_value.wstr_len))
            {
                //密码是否正确
                if (MMISET_IsCorrectPowerOnPwd(old_pwd_value.wstr_ptr, old_pwd_value.wstr_len))
                {
                    //刷新小屏的显示,关闭输入开机密码
#ifndef SUBLCD_SIZE_NONE                    
                    MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PWD,PNULL);
#endif

#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
                    if ((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
                    {
                        MMIPHONE_PowerOffPs(MN_DUAL_SYS_1); //开机密码界面，未启动SIM卡，启动卡1即可
                    }
#endif

                    OperatePowerOnPwd(g_mmiset_pwd_type);
                    MMK_CloseWin(win_id);
                }
                else
                {
                    //提示密码错误!
                    MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                    
                    //清空password
                    GUIEDIT_ClearAllStr(MMISET_SET_SECURITY_PWD_CTRL_ID);
                }
            }
        }

        break;
    case MSG_APP_CANCEL:    
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
        if((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
        {
            break;
        }
        else
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CLOSE_WINDOW:        
        //刷新小屏的显示,关闭输入开机密码
#ifndef SUBLCD_SIZE_NONE        
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PWD,PNULL);
#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#else
/*****************************************************************************/
//  Description : to handle security setting input old power on password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputPwdWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_STRING_T old_pwd_value = {0};  
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T edit_both_rect = {0};
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
#endif
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif
        GUIAPICTRL_SetBothRect(MMISET_SET_SECURITY_PWD_CTRL_ID, &edit_both_rect);

        if ((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
        {
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
            MMIPHONE_PowerOnPs(MN_DUAL_SYS_1); //开机密码界面，未启动SIM卡，启动卡1即可

            //set left softkey
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PwdInputWinSoftkey);
#else
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
#endif

#if defined(MMI_PDA_SUPPORT) && defined(MMIPUBWIN_PDASTYLE_SUPPORT)
            // PUBEDIT负责softkey的显示，PDA下需要禁止右键
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,2,0,TXT_NULL,TXT_NULL,PNULL);
#else
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,2,1,TXT_NULL,TXT_DELETE,PNULL);
#endif
        }
        else
        {
            //set left softkey
            GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        }
        
        MMK_SetAtvCtrl(win_id, MMISET_SET_SECURITY_PWD_CTRL_ID);
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示,输入开机密码
        MMISUB_IsPermitUpdate(TRUE);
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_PWD,PNULL);
#endif
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif
        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
#endif
        // get the value about PIN
        GUIEDIT_GetString(MMISET_SET_SECURITY_PWD_CTRL_ID,&old_pwd_value);
        
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
        is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&old_pwd_value, &emergency_call_len, MN_DUAL_SYS_1);

        if (((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
            && (is_input_emergency_call)
            && (old_pwd_value.wstr_len == emergency_call_len))
        {
            MakeEmergencyCall(&old_pwd_value);
        }
        else if (MSG_APP_GREEN != msg_id
               && MSG_APP_CALL2 != msg_id
               && MSG_APP_CALL3 != msg_id
               && MSG_APP_CALL4 != msg_id)
#endif
        {
            if (PHONE_CODE_LEN_IS_IN_RANGE(old_pwd_value.wstr_len))
            {
                //密码是否正确
#ifndef SUBLCD_SIZE_NONE
                    //刷新小屏的显示,关闭输入开机密码
                    MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PWD,PNULL);
#endif
#ifdef MMI_POWER_ON_PWD_SOS_SUPPORT
                    if ((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
                    {
                        MMIPHONE_PowerOffPs(MN_DUAL_SYS_1); //开机密码界面，未启动SIM卡，启动卡1即可
                    }
#endif

                    OperatePowerOnPwd(g_mmiset_pwd_type);
                    //MMK_CloseWin(win_id);
               
            }
        }

        break;
    case MSG_APP_CANCEL:    
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
        if((MMISET_PWD_NORMAL == g_mmiset_pwd_type)||(MMISET_PWD_OTHER == g_mmiset_pwd_type))
        {
            break;
        }
        else
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CLOSE_WINDOW:  
#ifndef SUBLCD_SIZE_NONE       
        //刷新小屏的显示,关闭输入开机密码
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PWD,PNULL);
#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif

#ifndef MMI_SECURITYSET_MINI_SUPPORT

/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OperatePowerOnPwd(
                             MMISET_PWD_OPERATE_TYPE_E  operate_type
                             )
{
    switch (operate_type)
    {
    case MMISET_PWD_OPEN:
        //enable password
        MMISET_SetPowerOnPwdStatus(TRUE);
        //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        break;
        
    case MMISET_PWD_MODIFY:
        MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);
        break;
        
    case MMISET_PWD_CLOSE:
        //diable password
        MMISET_SetPowerOnPwdStatus(FALSE);
        //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        break;
        
    case MMISET_PWD_NORMAL:
        MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_BOOTPWD_FINISH);
#if 0//for StartupCtrl
        if (MMIAPISET_GetFlyModeSetPowerOn())
        {
            MMIAPISET_SetFlyModeOpenPhone();
        }
        else
        {
            MMIAPIPHONE_NormalInitAfterPwd();
        }
#endif
        break;
        
    case MMISET_PWD_OTHER:
        break;
        
    default:
        //SCI_TRACE_LOW:"OperatePowerOnPwd: operate_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_6277_112_2_18_2_52_3_328,(uint8*)"d",operate_type);
        break;
    }
}
#else
/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OperatePowerOnPwd(
                             MMISET_PWD_OPERATE_TYPE_E  operate_type
                             )
{
    MMI_STRING_T        old_pwd_value   =   {0};
    BOOLEAN             is_pwd_correct  =   FALSE;
    GUIEDIT_GetString(MMISET_SET_SECURITY_PWD_CTRL_ID,&old_pwd_value);
    is_pwd_correct = MMISET_IsCorrectPowerOnPwd(old_pwd_value.wstr_ptr, old_pwd_value.wstr_len);
    if((MMISET_PWD_MODIFY != operate_type) && is_pwd_correct)
    {
        switch (operate_type)
        {
        case MMISET_PWD_OPEN:
            //enable password
            MMISET_SetPowerOnPwdStatus(TRUE);
            MMK_CloseWin(MMISET_SET_SECURITY_PWD_WIN_ID);
            //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            break;
        
        case MMISET_PWD_MODIFY:
#ifdef MMI_PDA_SUPPORT
            MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);
#else
            MMK_CreateWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);
#endif
            break;
        
        case MMISET_PWD_CLOSE:
            //diable password
            MMISET_SetPowerOnPwdStatus(FALSE);
            MMK_CloseWin(MMISET_SET_SECURITY_PWD_WIN_ID);
            //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            break;
        
        case MMISET_PWD_NORMAL:
            MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_BOOTPWD_FINISH);
#if 0//for StartupCtrl
            if (MMIAPISET_GetFlyModeSetPowerOn())
            {
                MMIAPISET_SetFlyModeOpenPhone();
            }
            else
            {
                MMIAPIPHONE_NormalInitAfterPwd();
            }
#endif
            break;
        
        case MMISET_PWD_OTHER:  
            break;
        default:
            //SCI_TRACE_LOW:"OperatePowerOnPwd: operate_type %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_6277_112_2_18_2_52_3_328,(uint8*)"d",operate_type);
            break;
        } 
    }
    else
    {
        if(MMISET_PWD_MODIFY == operate_type)
        {
#ifdef MMI_PDA_SUPPORT
            MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);
#else
            MMK_CreateWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_WIN_TAB,PNULL);
#endif
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                    
            //清空password
            GUIEDIT_ClearAllStr(MMISET_SET_SECURITY_PWD_CTRL_ID);
            //close input new pwd and input new pwd again window
            //MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_WIN_ID);
            //MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_ID);
        }
    }
}
#endif
/*****************************************************************************/
//  Description : handle input privacy protect password win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputPrivacyProtectPwd(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    //    int                                                 sel            =   0;
   // MMI_MENU_GROUP_ID_T         group_id        =   0;
   // MMI_MENU_ID_T               menu_id         =   0;
    MMI_STRING_T                old_pwd_value   =   {0};  
    MMI_RESULT_E                result          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_INPUT_PRIVACY_PROTRCT_PWD_CTRL_ID;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
    //sel = (int )MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMISET_PRIVACY_PROTECT_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        #ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        #endif
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(ctrl_id,&old_pwd_value);
        
        if (PRIVACY_PROTECT_PWD_LEN_IS_IN_RANGE(old_pwd_value.wstr_len))
        {
            //密码是否正确          
            if (MMISET_IsCorrectPrivacyProtectPwd( old_pwd_value.wstr_ptr,old_pwd_value.wstr_len))
            {

                MMK_CloseWin(win_id); //for cr226245
                MMK_CreateWin((uint32*)MMISET_SET_PRIVACY_PROTECT_ITEM_WIN_TAB,PNULL);
            }
            else
            {
                //提示密码错误!
                MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                //清空password
                GUIEDIT_ClearAllStr(ctrl_id);
            }
        }
        break;
        
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
//  Description : handle input new privacy protect password win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPrivacyPwdWindow(
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E     msg_id,
                                                  DPARAM               param
                                                  )
{   
    MMI_STRING_T        pwd_value       =   {0};  
    MMI_STRING_T        pwd_value_cnf   =   {0};  
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_PRIVACY_PROTECT_NEW_PWD_CTRL_ID;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMISET_PRIVACY_PROTECT_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about pwd
        GUIEDIT_GetString(ctrl_id,&pwd_value);
        if (PRIVACY_PROTECT_PWD_LEN_IS_IN_RANGE(pwd_value.wstr_len))
        {
            MMK_CreateWin((uint32*)MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_WIN_TAB,PNULL);
        }
        break;
        
    case MSG_SET_NEW_PRIVACY_PWD_AGAIN:
        // get the value about new pwd again
        GUIEDIT_GetString(MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_CTRL_ID,&pwd_value_cnf);
        
        // get the value about new pwd
        GUIEDIT_GetString(ctrl_id,&pwd_value);
        
        //比较两个pwd是否相同
        if( MMIAPICOM_StrCompare( &pwd_value, &pwd_value_cnf ) )
            //if (MMIAPICOM_CompareTwoTeleNum((uint8*)pwd_value_cnf.str_ptr,(uint8)pwd_value_cnf.str_len,(uint8*)pwd_value.str_ptr,(uint8)pwd_value.str_len))
        {
            //set privacy protect  password
            MMISET_SetPrivacyProtectPwd( pwd_value.wstr_ptr,pwd_value.wstr_len);
            
            //success!
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            
            //close input new pwd and input new pwd again window
            MMK_CloseWin(MMISET_PRIVACY_PROTECT_NEW_PWD_WIN_ID);
            MMK_CloseWin(MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_WIN_ID);
        }
        else
        {
            //提示错误窗口,两次输入的pwd不一致
            MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);
            
            //清空password
            GUIEDIT_ClearAllStr(ctrl_id);
            
            //close input new pwd again window
            MMK_CloseWin(MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_WIN_ID);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
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
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           )
{   
    MMI_STRING_T        pwd_value       =   {0};  
    MMI_STRING_T        pwd_value_cnf   =   {0};  
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    lcd_dev_info.lcd_id     =    GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   =    GUI_BLOCK_MAIN;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);//@fen.xie MS00175496:MMISET_POWER_ON_PWD_MIN_LEN
	  MMK_SetAtvCtrl(win_id,MMISET_SET_SECURITY_NEW_PWD_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif    
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID,&pwd_value);
        
        if (PHONE_CODE_LEN_IS_IN_RANGE(pwd_value.wstr_len))
        {
            MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_TAB,PNULL);
        }
        break;
        
    case MSG_SET_NEW_PWD_AGAIN:
        // get the value about new pwd again
        GUIEDIT_GetString(MMISET_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,&pwd_value_cnf);
        
        // get the value about new pwd
        GUIEDIT_GetString(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID,&pwd_value);
        
        //比较两个pwd是否相同
        if( MMIAPICOM_StrCompare( &pwd_value, &pwd_value_cnf ) )
            //if (MMIAPICOM_CompareTwoTeleNum((uint8*)pwd_value_cnf.str_ptr,(uint8)pwd_value_cnf.str_len,(uint8*)pwd_value.str_ptr,(uint8)pwd_value.str_len))
        {
            //set power on password
            MMISET_SetPowerOnPwd( pwd_value.wstr_ptr, pwd_value.wstr_len);
            
            if (MMISET_PWD_SET == g_mmiset_pwd_type)
            {
               // MMK_CreateWin((uint32*)MMISET_SET_SECURITY_PWD_MENU_WIN_TAB,PNULL);
                OperatePowerOnPwd(MMISET_PWD_OPEN);
            }
            else
            {
                //success!
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
            
            //close input new pwd and input new pwd again window
            MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_WIN_ID);
            MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_ID);
        }
        else
        {
            //提示错误窗口,两次输入的pwd不一致
            MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);
            
            //清空password
            GUIEDIT_ClearAllStr(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID);
            
            //close input new pwd again window
            MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_ID);
            
            MMK_SetAtvCtrl(win_id,MMISET_SET_SECURITY_NEW_PWD_CTRL_ID);
        }
        break;
        
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

#else
/*****************************************************************************/
//  Description : 处理输入新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           )
{   
    MMI_STRING_T        pwd_value       =   {0};  
    MMI_STRING_T        pwd_value_cnf   =   {0};  
    MMI_STRING_T        old_pwd_value   =   {0};
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    lcd_dev_info.lcd_id     =    GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   =    GUI_BLOCK_MAIN;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);//@fen.xie MS00175496:MMISET_POWER_ON_PWD_MIN_LEN
        MMK_SetAtvCtrl(win_id,MMISET_SET_SECURITY_NEW_PWD_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif    
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID,&pwd_value);
        
        if (PHONE_CODE_LEN_IS_IN_RANGE(pwd_value.wstr_len))
        {
            MMK_CreatePubEditWin((uint32*)MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_TAB,PNULL);
        }
        break;
        
    case MSG_SET_NEW_PWD_AGAIN:
        // get the value about new pwd again
        GUIEDIT_GetString(MMISET_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,&pwd_value_cnf);
        
        // get the value about new pwd
        GUIEDIT_GetString(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID,&pwd_value);

        GUIEDIT_GetString(MMISET_SET_SECURITY_PWD_CTRL_ID,&old_pwd_value);

        if (MMISET_IsCorrectPowerOnPwd(old_pwd_value.wstr_ptr, old_pwd_value.wstr_len))
        {       
        //比较两个pwd是否相同
            if( MMIAPICOM_StrCompare( &pwd_value, &pwd_value_cnf ) )
                //if (MMIAPICOM_CompareTwoTeleNum((uint8*)pwd_value_cnf.str_ptr,(uint8)pwd_value_cnf.str_len,(uint8*)pwd_value.str_ptr,(uint8)pwd_value.str_len))
            {
                //set power on password
                MMISET_SetPowerOnPwd( pwd_value.wstr_ptr, pwd_value.wstr_len);
            
                if (MMISET_PWD_SET == g_mmiset_pwd_type)
                {
                   // MMK_CreateWin((uint32*)MMISET_SET_SECURITY_PWD_MENU_WIN_TAB,PNULL);
                    OperatePowerOnPwd(MMISET_PWD_OPEN);
                }
                else
                {
                    //success!
                    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                }
            
                //close input new pwd and input new pwd again window
                MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_WIN_ID);
                MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_ID);
                MMK_CloseWin(MMISET_SET_SECURITY_PWD_WIN_ID);
            }
            else
            {
                //提示错误窗口,两次输入的pwd不一致
                MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);
            
                //清空password
                GUIEDIT_ClearAllStr(MMISET_SET_SECURITY_NEW_PWD_CTRL_ID);
            
                //close input new pwd again window
                MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_ID);
            
                MMK_SetAtvCtrl(win_id,MMISET_SET_SECURITY_NEW_PWD_CTRL_ID);
            }
        }
        else
        {
                                //提示密码错误!
                MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                    
                    //清空password
                GUIEDIT_ClearAllStr(MMISET_SET_SECURITY_PWD_CTRL_ID);
                                //close input new pwd and input new pwd again window
                MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_WIN_ID);
                MMK_CloseWin(MMISET_SET_SECURITY_NEW_PWD_CNF_WIN_ID);
                MMK_SetAtvCtrl(MMISET_SET_SECURITY_PWD_WIN_ID,MMISET_SET_SECURITY_PWD_CTRL_ID);

        }
            break;
        
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
#endif
/*****************************************************************************/
//  Description : get password edit rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMIAPISET_GetPwEditRect(MMI_WIN_ID_T win_id)
{
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
    GUI_BOTH_RECT_T both_rect = {0};
    CAF_RECT_T rect = {0, 24, 127, 40};

    both_rect.h_rect = rect;
    both_rect.v_rect = rect;
#else   
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
    both_rect.h_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
    both_rect.v_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
    both_rect.h_rect.bottom = both_rect.h_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.h_rect.right = both_rect.h_rect.right - MMI_PASSWORD_EDIT_LEFT;
    both_rect.v_rect.bottom = both_rect.v_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.v_rect.right = both_rect.v_rect.right - MMI_PASSWORD_EDIT_LEFT;
#endif 
    return both_rect;
}
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
/*****************************************************************************/
//  Description : 设置密码输入框的TITLE 区域
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPwTitleRect(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T title_rect = {0, 0, 127, 15};

    GUIWIN_SetTitleRect(win_id, title_rect);    
}
/*****************************************************************************/
//  Description : 获取Dphone 密码输入区域
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMIAPISET_GetPwEditRectForDphone(void)
{
    GUI_BOTH_RECT_T both_rect = {0};
    CAF_RECT_T rect = {0, 24, 127, 40};

    both_rect.h_rect = rect;
    both_rect.v_rect = rect;
    return both_rect;
}
#endif 

/*****************************************************************************/
//  Description : 设置输入密码窗口的背景色，image 和title
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPwBg(
                              MMI_WIN_ID_T      win_id, //窗口的ID
                              MMI_IMAGE_ID_T    image_id
                              )
{
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T          bg_rect         =   MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T  both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MONOCHROME_LCD_SUPPORT 
#else
    GUI_POINT_T         dis_point       =    {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;     
    GUI_BOTH_RECT_T edit_both_rect = {0};
#endif 
    lcd_dev_info.lcd_id     =   GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   =   GUI_BLOCK_MAIN;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    return ;   
#endif

    if(MMITHEME_IsMainScreenLandscape())     /*lint !e527 */
    {
        bg_rect = both_client_rect.h_rect;
    }
    else
    {
        bg_rect = both_client_rect.v_rect;
    }
#ifdef MONOCHROME_LCD_SUPPORT 
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
#else
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);

    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, image_id, win_id);
    
    //设置image id
    IMG_EnableTransparentColor(TRUE);    
    GUIRES_DisplayImg(PNULL,
        &bg_rect,
        &bg_rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);

    edit_both_rect = MMIAPISET_GetPwEditRect(win_id);

    dis_point.x = (bg_rect.right + bg_rect.left - icon_width) / 2;
    dis_point.y = (bg_rect.bottom + edit_both_rect.v_rect.bottom - icon_height) / 2;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        image_id,
        &lcd_dev_info);
    
    IMG_EnableTransparentColor(FALSE); 
#endif 
}
/*****************************************************************************/
//  Description : handle input new privacy protect password confirm win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPrivacyPwdCnf(
                                               MMI_WIN_ID_T      win_id,
                                               MMI_MESSAGE_ID_E  msg_id,
                                               DPARAM                param
                                               )
{   
    MMI_STRING_T       pwd_cnf_value    =   {0};  
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_PRIVACY_PROTECT_NEW_PWD_CNF_CTRL_ID;
    GUI_BOTH_RECT_T edit_both_rect = {0};
     
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMISET_PRIVACY_PROTECT_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about privacy protect password
        GUIEDIT_GetString(ctrl_id,&pwd_cnf_value);
        
        if (PRIVACY_PROTECT_PWD_LEN_IS_IN_RANGE(pwd_cnf_value.wstr_len))
        {
            //send the new privacy password again to input new privacy password window
            MMK_SendMsg(MMISET_PRIVACY_PROTECT_NEW_PWD_WIN_ID,MSG_SET_NEW_PRIVACY_PWD_AGAIN,PNULL);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMISET_PRIVACY_PROTECT_NEW_PWD_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 处理确认新的开机密码窗口信息
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputNewPwdCnfWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM                param
                                              )
{   
    MMI_STRING_T       pwd_cnf_value    =   {0};  
    MMI_RESULT_E        result          =   MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(MMISET_SET_SECURITY_NEW_PWD_CNF_CTRL_ID, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,0,MMISET_POWER_ON_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMISET_SET_SECURITY_NEW_PWD_CNF_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif    
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(MMISET_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,&pwd_cnf_value);
        
        if (PHONE_CODE_LEN_IS_IN_RANGE(pwd_cnf_value.wstr_len))
        {
            //send the new pin again to input new pin window
            MMK_SendMsg(MMISET_SET_SECURITY_NEW_PWD_WIN_ID,MSG_SET_NEW_PWD_AGAIN,PNULL);
        }
        break;
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
//  Description : open msg box
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMsgBox(uint32 win_tab, BOOLEAN is_special_func)
{
    if (is_special_func)
    {
        //@zhaohui modified for fdn
        if (win_tab == (uint32)MMISET_SET_FDN_STATE_WIN_TAB)
        {
#ifdef DPHONE_SUPPORT 
            MMK_SendMsg(MMISET_SET_FDN_STATE_WIN_ID, MSG_FDN_PIN2_SUCCESS, PNULL);
#else            
            MMK_SendMsg(MMISET_SET_SECURITY_FDN_WIN_ID, MSG_FDN_PIN2_SUCCESS, PNULL);
#endif 
        }
#if defined(FDN_SUPPORT)
        else if (win_tab == MMIAPIPB_GetDailNumberListWinTab())
        {
            MMIAPIPB_Pin2VerifyCnf();      
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
#endif
        else
        {
            MMI_WIN_ID_T alert_win_id = MMISET_SET_ENABLE_PIN1_WIN_ID;
            #ifdef MMI_PDA_SUPPORT
            if((uint32)MMISET_SET_ENABLE_PIN1_WIN_TAB == win_tab)
            {
                //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_OPEN,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
            }
            else if((uint32)MMISET_SET_DISABLE_PIN1_WIN_TAB == win_tab)
            {
                //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_CLOSE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else if((uint32)MMISET_SET_MODIFY_PIN1_WIN_TAB == win_tab)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_MODIFY,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }            
            #else
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            #endif
            
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
    }
    else
    {
        MMK_CreateWin((uint32*)win_tab,PNULL);
    }       
}



/*****************************************************************************/
//  Description : get if set security password win open
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSetSecurityWinOpen(void)
{
    return(MMK_IsOpenWin(MMISET_SET_POWER_ON_PWD_WIN_ID));  
}

/*****************************************************************************/
//  Description : open input power on password win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPowerOnPwdWin(void)
{
    MMK_CreatePubEditWin((uint32*)MMIPHONE_POWER_ON_PWD_WIN_TAB,PNULL);
}

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : Open FDN Win
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenFDNWin(MN_DUAL_SYS_E dual_sys)
{
    if (MMIAPIPHONE_IsSimCardOk(dual_sys))
    {
        MMIAPIFDN_SetCurSIM(dual_sys);
        if (!MMIAPIPB_IsPbReady())
        {
            MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        }
#if 0
        else if(!MMIAPIFDN_IsFdnExist(dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_FACILITY_NOT_SUPPORTED);
        }
#endif
        else
        {
            MMK_CreateWin((uint32*)MMISET_SET_SECURITY_FDN_WIN_TAB, (ADD_DATA)dual_sys);
        }
    }
    else if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SIM);
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
}
#endif
//#endif

/*****************************************************************************/
//  Description : to open pin win
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenPinWin( MN_DUAL_SYS_E dual_sys )
{
    if(MMIAPIENG_GetCallBarring()) //for mokeytest
    {
        //do nothing
    }
    else if (MMIAPIPHONE_IsPermitSetPin1(dual_sys))
    {
        MMK_CreateWin((uint32*)MMISET_SET_SECURITY_PIN1_WIN_TAB, (ADD_DATA)dual_sys);
    }
    else if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SIM);
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
}

/*****************************************************************************/
//  Description : to open pin2 win
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenPin2Win( MN_DUAL_SYS_E dual_sys )
{
    if(MMIAPIENG_GetCallBarring()) //for mokeytest
    {
        //do nothing
    }
    else if (MMIAPIPHONE_IsSimCardOk(dual_sys))/*lint !e64*/
    {
        MMIAPIPHONE_SetCurHandlingPinSimID(dual_sys);/*lint !e64*/
        MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_CHANGE,MNSIM_PIN2,(uint32)MMISET_SET_SECURITY_PIN2_WIN_TAB, TRUE);
    }
    else if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SIM);
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : to handle security setting blacklist menu
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistMenuWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
   MMI_MENU_GROUP_ID_T          group_id   =   0;
   MMI_MENU_ID_T                menu_id    =   0;
   MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
   MMI_CTRL_ID_T               ctrl_id     =   MMISET_SET_BLACKLIST_CTRL_ID;
   
   switch(msg_id)
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
       GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
       switch (menu_id)
       {
       case ID_SET_BLACKLIST_LIST:
           MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_LIST_WIN_TAB,PNULL);
           break;
           
       case ID_SET_BLACKLIST_SETTING:
           MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_TYPE_WIN_TAB,PNULL);
           break;
           
       default:
           break;
       }
       break;
       
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMISET_FreeBlacklistParam();
        break;
       
   default:
       recode = MMI_RESULT_FALSE;
       break;
   }
   
   return recode;
}

/*****************************************************************************/
//  Description : Set Blacklist type 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistTypeParam(void)
{
    MMI_STRING_T type_audio_call = {0};
    MMI_STRING_T type_video_call = {0};
    MMI_STRING_T type_message = {0};
    
    MMI_GetLabelTextByLang(TXT_SET_BLACKLIST_TYPE_AUDIOCALL, &type_audio_call);
	GUILABEL_SetText(MMISET_SET_BLACKLIST_TYPE_AUDIO_CTRL_ID, &type_audio_call, FALSE);

    MMI_GetLabelTextByLang(TXT_SET_BLACKLIST_TYPE_VEDIOCALL, &type_video_call);
	GUILABEL_SetText(MMISET_SET_BLACKLIST_TYPE_VEDIO_CTRL_ID, &type_video_call, FALSE);

    MMI_GetLabelTextByLang(TXT_SET_BLACKLIST_TYPE_MESSAGE, &type_message);
	GUILABEL_SetText(MMISET_SET_BLACKLIST_TYPE_MESSAGE_CTRL_ID, &type_message, FALSE);  
}

/*****************************************************************************/
//  Description : set backlight setlist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistSetlist(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    uint32 loop = 0;
    GUI_BG_T  bg_id = {0};
    uint32 text_id[]=
	{
		TXT_OPEN,
		TXT_CLOSE,
    };
    BOOLEAN is_block = FALSE;
    
    bg_id.img_id = IMAGE_THEME_BG;

    GUISETLIST_SetFontAlign(ctrl_id, ALIGN_LVMIDDLE);
    
	for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
	    GUISETLIST_AddItemById(ctrl_id, text_id[loop]);
	}

    switch(ctrl_id)
    {
        case MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID:
            is_block = MMIAPISET_GetIsBlacklistAudioCall();
            break;

#ifdef VT_SUPPORT
        case MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID:
            is_block = MMIAPISET_GetIsBlacklistVedioCall();
            break;
#endif

        case MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID:
            is_block = MMIAPISET_GetIsBlacklistMessage();
            break;

        default:
            break;
    }
    
    if(is_block)
    {
        index = 0;
    }
    else
    {
        index = 1;
    }
    
    GUISETLIST_SetBackGround(ctrl_id, &bg_id);
    GUISETLIST_SetCurIndex(ctrl_id, index );
    GUISETLIST_SetCtrlState(ctrl_id, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}


#ifndef MMI_GUI_STYLE_TYPICAL
#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : handle blacklist type win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistTypeWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID;
    uint16 index = 0;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            InitPdaBlackLIstSettingsListCtrl();
            MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
        case MSG_SET_BLACKLIST_LIST_UPDATE:
        index = GUILIST_GetCurItemIndex(ctrl_id);        
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaBlackLIstSettingsListCtrl();
        GUILIST_SetCurItemIndex(ctrl_id,index);  
        break;    
        case MSG_CTL_OK:
        case MSG_APP_OK:

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:                
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
          {
            index = GUILIST_GetCurItemIndex(ctrl_id);
            if(0 == index)
            {
                   MMISET_SetIsBlacklistAudioCall((BOOLEAN)!MMIAPISET_GetIsBlacklistAudioCall());
            }
            #ifdef VT_SUPPORT
            else if(1 == index)
            {
                    MMISET_SetIsBlacklistVedioCall((BOOLEAN)!MMIAPISET_GetIsBlacklistVedioCall());
            }
            #endif
            else
            {
                 MMISET_SetIsBlacklistMessage((BOOLEAN)!MMIAPISET_GetIsBlacklistMessage());
            }
            MMK_PostMsg(win_id, MSG_SET_BLACKLIST_LIST_UPDATE, PNULL, 0);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
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
#endif
#else
/*****************************************************************************/
//  Description : handle blacklist type win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistTypeWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_BLACKLIST_CTRL_ID;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            #ifndef MMI_PDA_SUPPORT
            GUIFORM_SetStyle(MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef VT_SUPPORT
            GUIFORM_SetStyle(MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
            GUIFORM_SetStyle(MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID,GUIFORM_STYLE_UNIT);
            #else
            GUIFORM_SetType(MMISET_SET_BLACKLIST_TYPE_FROM_CTRL_ID,GUIFORM_TYPE_TP);
            GUISETLIST_SetFontAlign(MMISET_SET_BLACKLIST_TYPE_FORM_AUDIO_CTRL_ID,ALIGN_LVMIDDLE);
            GUISETLIST_SetFontAlign(MMISET_SET_BLACKLIST_TYPE_FORM_VEDIO_CTRL_ID,ALIGN_LVMIDDLE);
            GUISETLIST_SetFontAlign(MMISET_SET_BLACKLIST_TYPE_FORM_MESSAGE_CTRL_ID,ALIGN_LVMIDDLE);
            #endif
            SetBlacklistTypeParam();

            SetBlacklistSetlist(MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID);
#ifdef VT_SUPPORT
            SetBlacklistSetlist(MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID);
#endif
            SetBlacklistSetlist(MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID);
            
            MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID);
            break;

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        #ifndef MMI_PDA_SUPPORT
        case MSG_CTL_OK:
        case MSG_APP_OK:            
        #endif
            ctrl_id= MMK_GetActiveCtrlId(win_id);
            if(msg_id==MSG_CTL_PENOK)
            {
                ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            }
            switch (ctrl_id)
            {
                case MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID:
                    SaveBlacklistTypeAudio();
                    break;

    #ifdef VT_SUPPORT
                case MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID:
                    SaveBlacklistTypeVideo();
                    break;
    #endif

                case MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID:
                    SaveBlacklistTypeMessage();
                    break;

                default:
                    break;
            }
            break;
            
        case MSG_CTL_SETLIST_SWITCH:
            ctrl_id= MMK_GetActiveCtrlId(win_id);
            switch (ctrl_id)
            {
                case MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID:
                    SaveBlacklistTypeAudio();
                    break;

        #ifdef VT_SUPPORT
                case MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID:
                    SaveBlacklistTypeVideo();
                    break;
        #endif

                case MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID:
                    SaveBlacklistTypeMessage();
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
#endif

/*****************************************************************************/
//  Description : save all audio setting
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeAudio(void)
{
    uint32 cur_selection = 0;
    BOOLEAN is_block = FALSE;
    
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_BLACKLIST_TYPE_SET_AUDIO_CTRL_ID); 
    is_block = (BOOLEAN)!cur_selection;
    MMISET_SetIsBlacklistAudioCall(is_block);
}

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : save all video setting
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeVideo(void)
{
    uint32 cur_selection = 0;
    BOOLEAN is_block = FALSE;
    
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_BLACKLIST_TYPE_SET_VEDIO_CTRL_ID); 
    is_block = (BOOLEAN)!cur_selection;
    MMISET_SetIsBlacklistVedioCall(is_block);
}
#endif

/*****************************************************************************/
//  Description : save all message setting
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void SaveBlacklistTypeMessage(void)
{
    uint32 cur_selection = 0;
    BOOLEAN is_block = FALSE;
    
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SET_BLACKLIST_TYPE_SET_MESSAGE_CTRL_ID); 
    is_block = (BOOLEAN)!cur_selection;
    MMISET_SetIsBlacklistMessage(is_block);
}

/*****************************************************************************/
//  Description : handle blacklist win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBlackListWindow(
                                            MMI_WIN_ID_T        win_id,     //IN: 窗口的ID
                                            MMI_MESSAGE_ID_E     msg_id,     //IN: 窗口的内部消息ID
                                            DPARAM                 param        //IN: 相应消息的参数
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_BLACKLIST_LIST_CTRL_ID;
    uint16 index = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        UpdateBlackList(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;

    case MSG_SET_BLACKLIST_LIST_UPDATE:
        UpdateBlackList(ctrl_id);
        index = *(uint16*)param;
        GUILIST_SetCurItemIndex(ctrl_id, index);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;


   #ifdef MMI_PDA_SUPPORT 
   case MSG_CTL_LIST_LONGOK:  
        MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_ITEM_OPT_WIN_TAB,PNULL);        
        break;
   #endif  
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_OPT_WIN_TAB,PNULL);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(0 == g_blacklist_ptr->num)
        {
            break;
        }
        else
        {
            MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_EDIT_WIN_TAB,PNULL);
        }
        break;

    case MMI_SELECT_CONTACT_CNF:
        SetBlacklistFromPB((MMIPB_HANDLE_T)param);
        UpdateBlackList(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0); 
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;
#ifdef MMI_PDA_SUPPORT        
    case MMI_CL_MULTI_SELECT_LIST_RETURN:
        SetBlacklistFromCL();
        UpdateBlackList(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, 0); 
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;        
#endif        
    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
      
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : update list
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void UpdateBlackList(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           )
{
    int32 i = 0;

    MMISET_GetBlackList();

    GUILIST_RemoveAllItems(ctrl_id);

    GUILIST_SetMaxItem( ctrl_id, MMISET_BLACKLIST_MAX_NUM, FALSE );//max item 10
    
    for ( i = g_blacklist_ptr->num; i > 0; i-- )
    {
        AppendBlacklistItem(
                            ctrl_id,
                            i - 1,
                            FALSE
                            );
    }
}

/*****************************************************************************/
//  Description : append list
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void AppendBlacklistItem(
                                MMI_CTRL_ID_T             ctrl_id,         //要设置的列表控件ID号 (输入参数)
                                uint16                    pos,
                                BOOLEAN                   is_update
                                )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 list_index = 0;
    MMI_STRING_T str_num = {0};
    uint16 name_len = 0;

    item_t.item_data_ptr = &item_data;

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;

    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_IMAGE_ID;

    item_data.item_content[list_index].item_data.image_id = IMAGE_SET_BLACKLIST;

    list_index++;

    //name
    name_len = MIN(g_blacklist_ptr->blacklist[pos].name_len,MMIPB_NAME_MAX_LEN);
    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;    
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = name_len;
    
    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = g_blacklist_ptr->blacklist[pos].name;

    list_index++;

    //number
    str_num.wstr_len = MIN(g_blacklist_ptr->blacklist[pos].num_len,MMISET_PHONE_NUM_MAX_LEN);

    item_data.item_content[list_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[list_index].item_data.text_buffer.wstr_len = str_num.wstr_len;

    item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = g_blacklist_ptr->blacklist[pos].num;

    list_index++;    
    
    //设置功能软键
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );      
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );      
    }
}

/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlacklistOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_BLACKLIST_LIST_OPT_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;

    //SCI_TRACE_LOW:"HandleBlacklistOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_7496_112_2_18_2_52_5_329,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(g_blacklist_ptr->num == 0)
        {
            MMIAPICOM_EnableGrayed(win_id, MENU_BLACKLIST_OPT,ID_SET_BLACKLIST_EDIT,TRUE);
            MMIAPICOM_EnableGrayed(win_id, MENU_BLACKLIST_OPT,ID_SET_BLACKLIST_DEL,TRUE);
            MMIAPICOM_EnableGrayed(win_id, MENU_BLACKLIST_OPT,ID_SET_BLACKLIST_DELALL,TRUE);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        HandleBlacklistMenuMsg(menu_id,ctrl_id);
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

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBlackListItmeOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          )
{

    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_BLACKLIST_ITEM_OPT_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            const GUILIST_ITEM_T* list_item_ptr = PNULL;
            MMI_STRING_T  title_text = {0};
            list_item_ptr = GUILIST_GetItemPtrByIndex( MMISET_SET_BLACKLIST_LIST_CTRL_ID,GUILIST_GetCurItemIndex( MMISET_SET_BLACKLIST_LIST_CTRL_ID));
            if((PNULL !=list_item_ptr )&& (PNULL !=list_item_ptr->item_data_ptr))
            {
                title_text = list_item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
                GUIMENU_SetMenuTitle(&title_text,  ctrl_id);
            }
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

     case MSG_APP_WEB:
     case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
         {
            GUIMENU_GROUP_T *cur_group_ptr = PNULL;
            uint16 item_cnt =0;
        
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        
            //读取静态菜单数据个数
            cur_group_ptr = MMI_GetMenuGroup(group_id);
            if (PNULL != cur_group_ptr)
            {
                item_cnt = cur_group_ptr->item_count;
            }
        
            if((ID_SET_BLACKLIST_DEL == menu_id)&&(item_cnt ==1))//只有唯一的菜单--删除
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistDelQueryWinMsg);
                MMK_CloseWin(win_id);
            }
            else
            {
                HandleBlacklistMenuMsg(menu_id,ctrl_id);
                MMK_CloseWin(win_id);
            }
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
#endif
/*****************************************************************************/
//  Description : to handle the blacklist option window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void HandleBlacklistMenuMsg(
                                 MMI_MENU_ID_T menu_id,    //IN: 被选中的菜单项
                                 MMI_CTRL_ID_T ctrl_id
                                 )
{
    uint32 user_data = 0;
    
    switch (menu_id)
    {
    case ID_SET_BLACKLIST_NEW:                 
        if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
        }
        else  if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
                &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif            
                &&!MMIAPISET_GetIsBlacklistMessage()
                )
        {
            user_data = ID_SET_BLACKLIST_NEW;
            MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_OPEN_BLACKLIST_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistAddQueryWinMsg,(uint32)user_data);
        }
        else
        {
            MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_NEW_WIN_TAB, (ADD_DATA)ctrl_id);
        }
        break;
        
    case ID_SET_BLACKLIST_EDIT:                     
        MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_EDIT_WIN_TAB,PNULL);
        break;
        
    case ID_SET_BLACKLIST_DEL:                 
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistDelQueryWinMsg);
        break;
        
    case ID_SET_BLACKLIST_CONTACT:            
        if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
        }
        else if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
                &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif            
                &&!MMIAPISET_GetIsBlacklistMessage()
                )
        {
            user_data = ID_SET_BLACKLIST_CONTACT;
            MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_OPEN_BLACKLIST_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistAddQueryWinMsg,(uint32)user_data);
        }
        else
        {
            /*MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_CHECK,
                                MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,
                                PNULL
                                );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    SetBlackListSelectContactCallback
                    );	
        }
        break;
        
    case ID_SET_BLACKLIST_DELALL:             
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_ALL_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleBlacklistDelAllQueryWinMsg);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : handle blacklist new win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNewBlacklistWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T edit_string = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        #ifndef MMI_PDA_SUPPORT
        GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        #else
        GUIFORM_SetType(MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID,GUIFORM_TYPE_TP);
        #endif
        InitBlacklistInfo();
        #ifndef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID,GUIFORM_TYPE_TP);        
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_MAIN_PB, STXT_RETURN, FALSE);
        #endif
        MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID);
        break;
#ifndef MMI_PDA_SUPPORT    
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
        }
        else
        {
            if(SaveBlacklistInfo(g_blacklist_ptr->num,TRUE))
            {
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);            
                MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&g_blacklist_ptr->num, sizeof(uint16));
                MMK_CloseWin(win_id);
            }
        }
        break;
  
    
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
       /* MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_RADIO,
                                1,
                                PNULL
                                );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    AddBlackListSelectContactCallback
                    );	
        break;
#else
     case MSG_APP_WEB:
     case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
                }
                else
                {
                    if(SaveBlacklistInfo(g_blacklist_ptr->num,TRUE))
                    {
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);            
#endif
                        MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&g_blacklist_ptr->num, sizeof(uint16));
                        MMK_CloseWin(win_id);
                    }
                }

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
               /* MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_RADIO,
                                1,
                                PNULL
                                );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_RADIO, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_WHOLE,
                        1,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddBlackListSelectContactCallback
                        );	
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                break;           
            default:
                break;
            }
        }
        break;
#endif
    
    case MMI_SELECT_CONTACT_CNF:
        // get the number from the PB
        if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
        {
            SetBlacklistDetailFromPB((MMIPB_HANDLE_T)param);
        }
		break;
        
    case MSG_CTL_CANCEL:
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, &edit_string);
		
		if(edit_string.wstr_len == 0)
        {
            // close the edit window
            MMK_CloseWin(win_id);			
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        break;

   case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

   case MSG_PROMPTWIN_CANCEL: 
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
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
//  Description : handle blacklist edit win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBlacklistWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    uint16 pos = 0;
    MMIPB_BCD_NUMBER_T *sender_ptr = ( MMIPB_BCD_NUMBER_T * )MMK_GetWinAddDataPtr(win_id);
    BOOLEAN is_new = FALSE;
    MMI_STRING_T edit_string = {0};

    if(PNULL == sender_ptr) 
    {
        index = GUILIST_GetCurItemIndex(MMISET_SET_BLACKLIST_LIST_CTRL_ID);
        pos = g_blacklist_ptr->num - index - 1;
    }
    else
    {
        is_new = TRUE;
        pos = g_blacklist_ptr->num;
    }
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMISET_SET_BLACKLIST_SET_NUM_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);        
        SetBlacklistInfo(pos,sender_ptr);
        #ifndef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMISET_SET_BLACKLIST_NEW_FROM_CTRL_ID,GUIFORM_TYPE_TP);        
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_MAIN_PB, STXT_RETURN, FALSE);
        #endif
        MMK_SetAtvCtrl(win_id, MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
	//case MSG_APP_MENU:
        if(SaveBlacklistInfo(pos,is_new))
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);            
#endif
            MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&index, sizeof(uint16));
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif
       /* MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_RADIO,
                                1,
                                PNULL
                                );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditBlackListSelectContactCallback
                    );	
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;

            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;

            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        
    case MMI_SELECT_CONTACT_CNF:
        // get the number from the PB
        if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
        {
            SetBlacklistDetailFromPB((MMIPB_HANDLE_T)param);
        }
		break;
        
    case MSG_CTL_CANCEL:
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, &edit_string);
		
		if(edit_string.wstr_len == 0)
        {
            // close the edit window
            MMK_CloseWin(win_id);			
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }
        break;

   case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

   case MSG_PROMPTWIN_CANCEL: 
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(!MMK_IsOpenWin(MMISET_SET_BLACKLIST_LIST_WIN_ID))
        {
            MMISET_FreeBlacklistParam();
            if(PNULL != s_new_blacklist_num_ptr)
            {
                SCI_FREE(s_new_blacklist_num_ptr);
                s_new_blacklist_num_ptr = PNULL;
            }
        }
        break;
            
    default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : Set Blacklist info
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void InitBlacklistInfo(void)
{
    wchar name[MMIPB_NAME_MAX_LEN + 1] = {0};
    uint16 name_len = 0;
    wchar num[MMISET_PHONE_NUM_MAX_LEN + 1] = {0};
    uint16 num_len = 0;
    MMI_STRING_T str_name = {0};
    MMI_STRING_T str_num = {0};    
    
    MMI_GetLabelTextByLang(TXT_NAME, &str_name);
	GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID, &str_name, FALSE);

    MMI_GetLabelTextByLang(TXT_COMMON_NUMBER, &str_num);
	GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID, &str_num, FALSE);

    GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID, name, name_len);
    GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, num, num_len);

    return;
}

/*****************************************************************************/
//  Description : Save Blacklist info
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveBlacklistInfo(
                                uint16 index,
                                BOOLEAN is_new
                               )
{
    MMI_STRING_T str_name = {0};
    MMI_STRING_T str_num = {0}; 
    BOOLEAN result = FALSE;
    uint16 name_len = 0;    
    uint16 num_len = 0;
    
    GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,&str_name);
    GUIEDIT_GetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID,&str_num);

    if(0 == str_num.wstr_len)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_INPUT_NUM);
        result = FALSE;
    }
    else if(!MMIAPICOM_IsValidNumberStringEx(&str_num))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        result = FALSE;        
    }
    else
    {
        uint16 j=0;
        
        for(j=0; j<g_blacklist_ptr->num;j++)
        {
            uint32 cmp_count =0;
            cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,str_num.wstr_len);
            if(!MMIAPICOM_Wstrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, str_num.wstr_ptr, cmp_count)
            &&( j != index))//for edit blacklist and modfy nothing
            {
                MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
                return FALSE;
            }
        }

        name_len = MIN(str_name.wstr_len,MMIPB_NAME_MAX_LEN);
        g_blacklist_ptr->blacklist[index].name_len = name_len;
        MMI_WSTRNCPY(
                     g_blacklist_ptr->blacklist[index].name,
                     MMIPB_NAME_MAX_LEN,
                     str_name.wstr_ptr,
                     name_len,
                     name_len);

        num_len = MIN(str_num.wstr_len,MMISET_PHONE_NUM_MAX_LEN);
        g_blacklist_ptr->blacklist[index].num_len = num_len;
        MMI_WSTRNCPY(
                     g_blacklist_ptr->blacklist[index].num,
                     MMISET_PHONE_NUM_MAX_LEN,
                     str_num.wstr_ptr,
                     num_len,
                     num_len);        

        if(is_new)
        {
            g_blacklist_ptr->num++;
        }

        MMISET_SetBlackList(g_blacklist_ptr);
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Set Blacklist info
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlacklistInfo(
                            uint16 index,
                            MMIPB_BCD_NUMBER_T *sender_ptr
                            )
{
    MMI_STRING_T str_name = {0};
    MMI_STRING_T str_num = {0};    
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0}; 
    uint8 max_len = 0;
    uint8 tele_len = 0 ;
    wchar num[MMISET_PHONE_NUM_MAX_LEN] = {0};
    BOOLEAN find_result = FALSE;
    MMI_STRING_T name_str = {0};
    wchar str_ptr[MMIPB_MAX_STRING_LEN + 1] = {0};
    
    MMI_GetLabelTextByLang(TXT_NAME, &str_name);
	GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NAME_LABEL_CTRL_ID, &str_name, FALSE);

    MMI_GetLabelTextByLang(TXT_COMMON_NUMBER, &str_num);
	GUILABEL_SetText(MMISET_SET_BLACKLIST_NEW_NUM_LABEL_CTRL_ID, &str_num, FALSE);

    if(PNULL == sender_ptr)
    {
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID, g_blacklist_ptr->blacklist[index].name, g_blacklist_ptr->blacklist[index].name_len);
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, g_blacklist_ptr->blacklist[index].num, g_blacklist_ptr->blacklist[index].num_len);
    }
    else
    {
        if(PNULL != s_new_blacklist_num_ptr)
        {
            SCI_MEMSET((uint8 *)str_ptr, 0x00, sizeof(str_ptr));
            name_str.wstr_len = 0;
            name_str.wstr_ptr = str_ptr;
            find_result = MMIAPIPB_GetNameByNumber(s_new_blacklist_num_ptr, &name_str, MMIPB_MAX_STRING_LEN, FALSE);
            if(find_result)
            {
                GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID,name_str.wstr_ptr,name_str.wstr_len);                   
            }
        }

        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, sender_ptr->number_len);
        tele_len = MMIAPIPB_BCDNumberToString(sender_ptr, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//         tele_len = MMIAPICOM_GenDispNumber( 
//                                             MMIPB_GetNumberTypeFromUint8(sender_ptr->npi_ton), 
//                                             max_len, 
//                                             sender_ptr->number,  
//                                             tele_num,
//                                             MMISET_PHONE_NUM_MAX_LEN + 2
//                                             );

        tele_len = MIN(MMISET_PHONE_NUM_MAX_LEN, tele_len);

        MMI_STRNTOWSTR(
                       num, 
                       tele_len, 
                       tele_num, 
                       tele_len, 
                       tele_len
                       );
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, num, tele_len);
    }

    return;
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete query window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
	uint16 index = 0;
    uint16 real_index = 0;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(MMISET_SET_BLACKLIST_LIST_CTRL_ID);
        real_index = g_blacklist_ptr->num - index - 1;
        DeleteBlacklistItem(real_index);
#ifndef MMI_PDA_SUPPORT
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&index, sizeof(uint16));
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete all query window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistDelAllQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        DeleteAllBlacklistItem();
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);            
        MMK_PostMsg(MMISET_SET_BLACKLIST_LIST_WIN_ID, MSG_SET_BLACKLIST_LIST_UPDATE, (DPARAM)&index, sizeof(uint16));
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteBlacklistItem(
                                             uint16 pos
                                             )
{
    uint32    i = 0;
    MN_RETURN_RESULT_E return_val = MN_RETURN_SUCCESS;
    
    if(pos > (g_blacklist_ptr->num - 1))
    {
        return MN_RETURN_FAILURE;
    }
    
    //将删除位置以后的所有项前移
    for (i=pos+1; i<g_blacklist_ptr->num; i++)
    {
        //g_blacklist_ptr->blacklist[i-1] = g_blacklist_ptr->blacklist[i];
        SCI_MEMCPY(
                   &g_blacklist_ptr->blacklist[i-1],
                   &g_blacklist_ptr->blacklist[i],
                   sizeof(MMISET_BLACKLIST_ITEM_T));
    }
    
    g_blacklist_ptr->num--;
    
    MMISET_SetBlackList(g_blacklist_ptr);
    
    return return_val;
}

/*****************************************************************************/
//  Description : to handle the blacklist of delete all window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E DeleteAllBlacklistItem(void)
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_SUCCESS;
    
    g_blacklist_ptr->num = 0;

    SCI_MEMSET(g_blacklist_ptr,0x00,sizeof(MMISET_BLACKLIST_T));
    
    MMISET_SetBlackList(g_blacklist_ptr);
    
    return return_val;
}

/*****************************************************************************/
//  Description : set info from PB  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistDetailFromPB(MMIPB_HANDLE_T handle)
{
    BOOLEAN return_val = TRUE;
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    wchar num[MMISET_PHONE_NUM_MAX_LEN + 1] = {0};
    MMI_STRING_T name_str = {0};

    name_str.wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN+1)*sizeof(wchar));
    if(PNULL == name_str.wstr_ptr)
    {
        return FALSE;
    }

    SCI_MEMSET(name_str.wstr_ptr,0x00,(MMIPB_NAME_MAX_LEN+1)*sizeof(wchar));    
    MMIAPIPB_GetSelectName(handle, &name_str,MMIPB_NAME_MAX_LEN,0);
    
    if(0 == name_str.wstr_len)//name is empty, clean the privous name string
    {
        GUIEDIT_ClearAllStr(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID);
    }
    else
    {
        GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NAME_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len);
    }
        
	MMIAPIPB_GetSelectNumber(handle, &number,MMIPB_BCD_NUMBER_MAX_LEN, 0);
    max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, number.number_len);
    tele_len = MMIAPIPB_BCDNumberToString(&number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//     tele_len = MMIAPICOM_GenDispNumber( 
//                                         MMIPB_GetNumberTypeFromUint8(number.npi_ton), 
//                                         max_len, 
//                                         number.number,  
//                                         tele_num,
//                                         MMISET_PHONE_NUM_MAX_LEN + 2
//                                         );
    
    MMI_STRNTOWSTR(
                   num, 
                   MMISET_PHONE_NUM_MAX_LEN + 2, 
                   tele_num, 
                   MMISET_PHONE_NUM_MAX_LEN + 2, 
                   tele_len
                   );
        
    GUIEDIT_SetString(MMISET_SET_BLACKLIST_NEW_NUM_CTRL_ID, num, tele_len);

    SCI_FREE(name_str.wstr_ptr);
    name_str.wstr_ptr = PNULL;
    
    return return_val;
}
#ifdef MMI_PDA_SUPPORT 

LOCAL uint32 GetNextClIndex(uint32 cur_index)
{   
    uint32 i,index = 0;

    for(i=0;i<MMICL_RECORD_MAX_NUM;i++)
    {
      if (MMIAPICL_SelectedListIsValid(i))
        {
            if (index == cur_index)
            {
                return i;
            }
            index ++;
        }
    }
    return MMICL_RECORD_MAX_NUM;
}

/*****************************************************************************/
//  Description : set detail from CL  
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromCL(void)
{
    BOOLEAN return_val = TRUE;
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    MMI_STRING_T name_str = {0};
    uint16 count = MMIAPICL_GetSelectedItemCount();
    int32 i = 0;
    BOOLEAN once_duplicated= FALSE;
    wchar name_wstr[MMIPB_NAME_MAX_LEN+1]={0};
    uint32 cl_index = 0;

    if ((MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num) < count)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_BEYOND_MAX_NUM);
        return FALSE;
    }
    cl_index= 0;
    for (i=0; i<MIN(MMISET_BLACKLIST_MAX_NUM, count); i++)
    {
        uint16 j= 0;
        BOOLEAN is_duplicate = FALSE;
        uint16 total_num = 0;

        cl_index = GetNextClIndex(i);
        if(cl_index>=MMICL_RECORD_MAX_NUM)
        {
            break;
        }
        
        SCI_MEMSET(name_wstr,0x00,(MMIPB_NAME_MAX_LEN+1)*sizeof(wchar));   
        SCI_MEMSET(&name_str,0,sizeof(name_str));
        name_str.wstr_ptr = name_wstr;
        // read the phone number from phonebook one by one
        SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));
        
        MMIAPICL_GetSelectPhonenum(&number,cl_index);

        MMIAPICL_GetSelectPhoneName(&name_str,cl_index,MMIPB_NAME_MAX_LEN,&number);

        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, number.number_len);
        tele_len = MMIAPIPB_BCDNumberToString(&number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
        for(j=0; j<g_blacklist_ptr->num;j++)
        {
            uint32 cmp_count =0;
            cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,tele_len);
            if(!MMIAPICOM_WstrStrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, tele_num, cmp_count))
            {
                is_duplicate =TRUE;
                once_duplicated = TRUE;
                //MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
                //break;
                break;
            }
        }
        if(!is_duplicate)
        {
            total_num = g_blacklist_ptr->num;
            g_blacklist_ptr->blacklist[total_num].name_len = name_str.wstr_len;
            
            MMI_WSTRNCPY(
                         g_blacklist_ptr->blacklist[total_num].name,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_ptr,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_len
                         );

            g_blacklist_ptr->blacklist[total_num ].num_len = tele_len;
            MMI_STRNTOWSTR(
                           g_blacklist_ptr->blacklist[total_num].num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_len
                           );
            g_blacklist_ptr->num++;
            MMISET_SetBlackList(g_blacklist_ptr);
        }
    }
    if (once_duplicated)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
    }
       
    return return_val;
}
#endif
/*****************************************************************************/
//  Description : set list from PB  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBlacklistFromPB(MMIPB_HANDLE_T handle)
{
    BOOLEAN return_val = TRUE;
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    MMI_STRING_T name_str = {0};
    uint16 count = MMIAPIPB_GetSelectCount(handle);
    int32 i = 0;
    
    if ((MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num) < count)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_BEYOND_MAX_NUM);
        return FALSE;
    }

    name_str.wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN+1)*sizeof(wchar));
    if(PNULL == name_str.wstr_ptr)
    {
        return FALSE;
    }
    
        for (i=0; i<MIN(MMISET_BLACKLIST_MAX_NUM, count); i++)
    {
        uint16 j= 0;
        BOOLEAN is_duplicate = FALSE;
        uint16 total_num = 0;

        // read the phone number from phonebook one by one
        SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));

        MMIAPIPB_GetSelectNumber(handle, &number,MMIPB_BCD_NUMBER_MAX_LEN, i);

        SCI_MEMSET(name_str.wstr_ptr,0x00,(MMIPB_NAME_MAX_LEN+1)*sizeof(wchar)); 
        MMIAPIPB_GetNameByNumber(&number,
                                  &name_str,
                                  MMIPB_NAME_MAX_LEN,
                                  FALSE  //use default name,"no name"
                                  );
        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, number.number_len);
        tele_len = MMIAPIPB_BCDNumberToString(&number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//         tele_len = MMIAPICOM_GenDispNumber( 
//                                             MMIPB_GetNumberTypeFromUint8(number.npi_ton), 
//                                             max_len, 
//                                             number.number,  
//                                             tele_num,
//                                             MMISET_PHONE_NUM_MAX_LEN + 2
//                                             );

		if(0 == tele_len)
		{
			if(PNULL != name_str.wstr_ptr)
			{
				SCI_FREE(name_str.wstr_ptr);
			}
			MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
			return FALSE;
		}//add by yongsheng.wang
        for(j=0; j<g_blacklist_ptr->num;j++)
        {
            uint32 cmp_count =0;
            cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,tele_len);
            if(!MMIAPICOM_WstrStrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, tele_num, cmp_count))
            {
                is_duplicate =TRUE;
                MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
                break;
            }
        }
        if(!is_duplicate)
        {
            total_num = g_blacklist_ptr->num;
            g_blacklist_ptr->blacklist[total_num].name_len = name_str.wstr_len;
            
            MMI_WSTRNCPY(
                         g_blacklist_ptr->blacklist[total_num].name,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_ptr,
                         MMIPB_NAME_MAX_LEN,
                         name_str.wstr_len
                         );

            g_blacklist_ptr->blacklist[total_num ].num_len = tele_len;
            MMI_STRNTOWSTR(
                           g_blacklist_ptr->blacklist[total_num].num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_num, 
                           MMISET_PHONE_NUM_MAX_LEN + 2, 
                           tele_len
                           );
            g_blacklist_ptr->num++;
            MMISET_SetBlackList(g_blacklist_ptr);
        }
    }

    if(PNULL != name_str.wstr_ptr)
    {
        SCI_FREE(name_str.wstr_ptr);
    }
        
    return return_val;
}

/*****************************************************************************/
//  Description : open blacklit edit win  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenBlacklistEditWin(MMIPB_BCD_NUMBER_T *orginal_addr_ptr)
{
    if (PNULL == orginal_addr_ptr)
    {
        return;
    }

    s_new_blacklist_num_ptr = SCI_ALLOCA(sizeof(MMIPB_BCD_NUMBER_T));
    if(PNULL == s_new_blacklist_num_ptr)
    {
        return;
    }
    
    SCI_MEMSET(
               s_new_blacklist_num_ptr,
               0x00,
               sizeof(MMIPB_BCD_NUMBER_T)
               );

    SCI_MEMCPY(
               s_new_blacklist_num_ptr,
               orginal_addr_ptr,
               sizeof(MMIPB_BCD_NUMBER_T)
               );
    
    MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_EDIT_WIN_TAB,(ADD_DATA)s_new_blacklist_num_ptr);
}

/*****************************************************************************/
//  Description : add blacklist from external,such as PB  
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 调用这个接口的时候,所有的加入黑名单询问、提示都需要调用者自己处理
/*****************************************************************************/
PUBLIC MMISET_BLACKLIST_ERROR_TYPE_E MMIAPISET_AddBlacklistFromExt(
                                                                   MMI_STRING_T * blacklist_name_ptr,
                                                                   MMIPB_BCD_NUMBER_T *pb_num_ptr      
                                                                   )
{
    MMISET_BLACKLIST_ERROR_TYPE_E result = MMISET_BLACKLIST_ERROR_MAX;
    uint16 name_len = 0;    
    uint16 num_len = 0;
    uint16 index = 0;//g_blacklist_ptr->num
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 tele_len = 0;
    BOOLEAN is_need_free = FALSE;
   
    if (PNULL == blacklist_name_ptr ||PNULL == pb_num_ptr)
    {
        //SCI_TRACE_LOW:"MMISET: MMIAPISET_AddBlacklistFromExt,pb_name_ptr ||pb_num_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SECURITY_8514_112_2_18_2_52_7_330,(uint8*)"");
        return MMISET_BLACKLIST_ERROR_PARAM_NULL;
    }
    if(PNULL ==g_blacklist_ptr)
    {
        is_need_free = TRUE;
    }
    if(MMISET_InitBlacklistParam())
    {
        MMISET_GetBlackList();
        if(PNULL !=g_blacklist_ptr)
        {
            if (MMISET_BLACKLIST_MAX_NUM <= g_blacklist_ptr->num)
            {
                if(is_need_free)
                {
                    MMISET_FreeBlacklistParam();  
                } 
                return MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM;
            }
			index = g_blacklist_ptr->num;  
        }
        
            
        tele_len = MMIAPIPB_BCDNumberToString(pb_num_ptr, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//         tele_len = MMIAPICOM_GenDispNumber( 
//                                         MMIPB_GetNumberTypeFromUint8(pb_num_ptr->npi_ton), 
//                                         pb_num_ptr->number_len, 
//                                         pb_num_ptr->number,  
//                                         tele_num,
//                                         MMISET_PHONE_NUM_MAX_LEN + 2
//                                         );
        /*load from pb,do not need check num if valid*/                                    
        if(0 == pb_num_ptr->number_len)
        {
            result = MMISET_BLACKLIST_ERROR_NUM_EMPTY;
        }
        else
        {
            if(PNULL !=g_blacklist_ptr)
            {
                uint16 i= 0;              
                for(i=0; i<g_blacklist_ptr->num;i++)
                {
                    uint32 count =0;
                    count = (uint32)MAX(g_blacklist_ptr->blacklist[i].num_len,tele_len);
                    if(!MMIAPICOM_WstrStrncmp(g_blacklist_ptr->blacklist[i].num, tele_num, count))
                    {
                        if(is_need_free)
                        {
                            MMISET_FreeBlacklistParam();  
                        } 
                        return MMISET_BLACKLIST_ERROR_SAME_NUM;
                    }
                }
                
                name_len = MIN(blacklist_name_ptr->wstr_len,MMIPB_NAME_MAX_LEN);
                g_blacklist_ptr->blacklist[index].name_len = name_len;
                MMI_WSTRNCPY(
                             g_blacklist_ptr->blacklist[index].name,
                             MMIPB_NAME_MAX_LEN,
                             blacklist_name_ptr->wstr_ptr,
                             name_len,
                             name_len);

                num_len = MIN(tele_len,MMISET_PHONE_NUM_MAX_LEN);
                g_blacklist_ptr->blacklist[index].num_len = num_len;
                MMI_STRNTOWSTR(
                                            g_blacklist_ptr->blacklist[index].num, 
                                            MMISET_PHONE_NUM_MAX_LEN, 
                                            tele_num, 
                                            num_len, 
                                            num_len);   

                g_blacklist_ptr->num++;
                MMISET_SetBlackList(g_blacklist_ptr);
                
                UpdateBlackList(MMISET_SET_BLACKLIST_LIST_CTRL_ID);
                result = MMISET_BLACKLIST_ERROR_NONE;
            }           
        }
        
        if(is_need_free)
        {
            MMISET_FreeBlacklistParam();  
        } 
    }
    return result;
}

/*****************************************************************************/
//  Description : add blacklist,such as from CL,sms,mms
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 内部封装了MMIAPISET_AddBlacklistFromExt等接口；加入
//        黑名单时的询问、提示等操作已处理，调用者无需另外处理
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_AddBlacklist(
                                MMI_STRING_T mmi_pb_name,
                                MMIPB_BCD_NUMBER_T pb_bcd      
                                )
{
    MMI_WIN_ID_T query_win_id = MMISET_ADD_BLACKLISTQUERY_WIN_ID;
	
    //add to blacklist
    if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
        &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif            
        &&!MMIAPISET_GetIsBlacklistMessage()
        )
    {
        MMISET_BLACKLIST_NAME_NUM_ITEM_T *black_name_num_ptr = PNULL;

        black_name_num_ptr = SCI_ALLOC_APPZ(sizeof(MMISET_BLACKLIST_NAME_NUM_ITEM_T));
        if (PNULL == black_name_num_ptr)
        {
            return FALSE;
        }
        
        black_name_num_ptr->pb_bcd.npi_ton = pb_bcd.npi_ton;
        black_name_num_ptr->pb_bcd.number_len = pb_bcd.number_len;
        SCI_MEMCPY( 
            black_name_num_ptr->pb_bcd.number,  
            pb_bcd.number, 
            black_name_num_ptr->pb_bcd.number_len 
            );
        black_name_num_ptr->mmi_pb_name.wstr_len = mmi_pb_name.wstr_len;
        black_name_num_ptr->mmi_pb_name.wstr_ptr = SCI_ALLOC_APPZ((MMIPB_NAME_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == black_name_num_ptr->mmi_pb_name.wstr_ptr)
        {
		    SCI_FREE(black_name_num_ptr);

            return FALSE;
        }
        SCI_MEMCPY( 
            black_name_num_ptr->mmi_pb_name.wstr_ptr,  
            mmi_pb_name.wstr_ptr, 
            MMIPB_NAME_MAX_LEN
            );

		MMK_CloseWin(query_win_id);

        MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_ADD_TO_BLACKLIST_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,HandleAddToBlackListQueryWinMsg, (uint32)black_name_num_ptr);
        return FALSE;
    }
    else
    {
        return AddNumToBlacklist(mmi_pb_name, pb_bcd);
    }
}

/*****************************************************************************/
// 	Description : alert of the result when add a number into blacklist
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddNumToBlacklist(
                            MMI_STRING_T mmi_pb_name,
                            MMIPB_BCD_NUMBER_T pb_bcd      
                            )
{
    MMI_RESULT_E    ret = FALSE;
    MMISET_BLACKLIST_ERROR_TYPE_E add_blacklist_result = MMISET_BLACKLIST_ERROR_MAX;
       
    add_blacklist_result = MMIAPISET_AddBlacklistFromExt(&mmi_pb_name,&pb_bcd);      
    
    if(MMISET_BLACKLIST_ERROR_NONE == add_blacklist_result)
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        ret = TRUE;
    }
    else if(MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM == add_blacklist_result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
        ret = FALSE;
    }
    else if(MMISET_BLACKLIST_ERROR_SAME_NUM == add_blacklist_result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
        ret = FALSE;
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        ret = FALSE;
    }

    return ret;

}

/*****************************************************************************/
//  Description : to handle add to blacklist query window    
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddToBlackListQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *win_info_ptr = PNULL;
    MMISET_BLACKLIST_NAME_NUM_ITEM_T *black_name_num_ptr = PNULL;
    win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        black_name_num_ptr  = (MMISET_BLACKLIST_NAME_NUM_ITEM_T *)win_info_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:        
        MMIAPISET_SetAllBlacklistSetting(TRUE);
        if (PNULL != black_name_num_ptr)
        {
            if (AddNumToBlacklist(black_name_num_ptr->mmi_pb_name, black_name_num_ptr->pb_bcd))
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != black_name_num_ptr)
        {
            if (black_name_num_ptr->mmi_pb_name.wstr_ptr)
            {
                SCI_FREE(black_name_num_ptr->mmi_pb_name.wstr_ptr);
            }

            SCI_FREE(black_name_num_ptr);
        }
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : to handle  add blacklist open setting query window
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBlacklistAddQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        if(!MMIAPISET_GetIsBlacklistAudioCall()
#ifdef VT_SUPPORT
        &&!MMIAPISET_GetIsBlacklistVedioCall()
#endif            
        &&!MMIAPISET_GetIsBlacklistMessage()
        )
        {
            MMIAPISET_SetAllBlacklistSetting(TRUE);
        }
        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if ((win_info_ptr != PNULL) && ((uint32 )(win_info_ptr->user_data) != NULL))
        {
            if( ID_SET_BLACKLIST_NEW == win_info_ptr->user_data)
            {
                MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_NEW_WIN_TAB, PNULL);
            }
            else if(ID_SET_BLACKLIST_CONTACT== win_info_ptr->user_data)
            {
               /* MMIAPIPB_OpenNumberList(
                        MMIPB_LIST_FOR_SMS,
                        MMIPB_LIST_CHECK,
                        MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,
                        PNULL
                        );*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    SetBlackListSelectContactCallback
                    );	
            }
            else
            {
                /*do nothing*/
            }
        }
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if ((win_info_ptr != PNULL) && ((uint32 )(win_info_ptr->user_data) != NULL))
        {
            if( ID_SET_BLACKLIST_NEW == win_info_ptr->user_data)
            {
                MMK_CreateWin((uint32 *)MMISET_SET_BLACKLIST_NEW_WIN_TAB, PNULL);
            }
            else if(ID_SET_BLACKLIST_CONTACT== win_info_ptr->user_data)
            {
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    MMISET_BLACKLIST_MAX_NUM - g_blacklist_ptr->num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    SetBlackListSelectContactCallback
                    );	
            }
            else
            {
                /*do nothing*/
            }
        }
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : delete blacklist,such as from CL,sms,mms
//  Global resource dependence : 
//  Author:sam.hua
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_DeleteBlacklist(MMIPB_BCD_NUMBER_T pb_bcd)
{

    int32 i,j=0;
    uint8 tele_len =0,max_len =0;
    uint8 tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    BOOLEAN is_need_free =FALSE;
    
    if(PNULL ==g_blacklist_ptr)
    {
        is_need_free = TRUE;
    }
    if(MMISET_InitBlacklistParam())
    {
        MMISET_GetBlackList();

        if(PNULL !=g_blacklist_ptr)
        {
            if (MMISET_BLACKLIST_MAX_NUM <= g_blacklist_ptr->num)
            {
                if(is_need_free)
                {
                    MMISET_FreeBlacklistParam();  
                } 
                return FALSE;
            }  
        }

        max_len = MIN(MMISET_PHONE_NUM_MAX_LEN, pb_bcd.number_len);
        tele_len = MMIAPIPB_BCDNumberToString(&pb_bcd, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);

        if(PNULL !=g_blacklist_ptr)
        {
            for(j=0; j<g_blacklist_ptr->num;j++)
            {
                uint32 cmp_count =0;
                cmp_count = (uint32)MAX(g_blacklist_ptr->blacklist[j].num_len,tele_len);
                if(!MMIAPICOM_WstrStrncmp((const wchar *)g_blacklist_ptr->blacklist[j].num, tele_num, cmp_count))
                {
                    break;
                }
            }
            
            for (i=j+1; i<g_blacklist_ptr->num; i++)
            {
                //g_blacklist_ptr->blacklist[i-1] = g_blacklist_ptr->blacklist[i];
                SCI_MEMCPY(
                           &g_blacklist_ptr->blacklist[i-1],
                           &g_blacklist_ptr->blacklist[i],
                           sizeof(MMISET_BLACKLIST_ITEM_T));
            }
            
            g_blacklist_ptr->num--;
            
            MMISET_SetBlackList(g_blacklist_ptr);

        }
        
        if(is_need_free)
        {
            MMISET_FreeBlacklistParam();  
        }  
    }

    return TRUE;
}

#endif//#if defined(MMI_BLACKLIST_SUPPORT)

#ifdef FDN_SUPPORT

/*****************************************************************************/
// 	Description : open fdn window
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFDNWindow(void)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_SELECT_TYPE_E    select_type = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint32 sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

    select_type = MMISET_SET_FDN;
    
    //判断是否存在两张卡同时开启，不是则启动active sim
     if (sim_ok_num > 1)
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
    else
    {
        MMISET_OpenFDNWin((MN_DUAL_SYS_E)sim_ok);
    }
#else
        MMISET_OpenFDNWin(MN_DUAL_SYS_1);
#endif

}
#endif

/*****************************************************************************/
// 	Description : MMISET_SetAutoLockKeyPadList
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetAutoLockKeyPadList(void)
{
    SetAutoLockKeyPadList();
}

/*****************************************************************************/
// 	Description : MMISET_SaveAutoKeyPadList
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SaveAutoKeyPadList(void)
{
    SaveAutoLockKeyPadList();
}
/*Edit by script, ignore 31 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
