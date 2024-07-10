/****************************************************************************
** File Name:      mmiset_callwin.c                                             *
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

#define _MMISET_CALLWIN_C_


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
#include "mmiidle_statusbar.h"
#include "guiform.h"
#include "guisetlist.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guidropdownlist.h"
//#include "mmieng.h"
#include "mmieng_export.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
#include "mmiset_menutable.h"

#ifdef ASP_SUPPORT
#include "mmiasp_text.h"
#endif
#ifdef DPHONE_SUPPORT
#include "mmiset_menutable.h"
#endif
#include "mmisms_export.h"
#include "mmi_appmsg.h"
#include "mmiconnection_export.h"
#include "guirichtext.h"
#include "mmifmm_export.h"
#include "mmicc_text.h"
#include "guistatusbar.h"
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif
#include "mmimms_export.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_internal.h"
#include "mmiidle_export.h"
#include "mmimp3_export.h"
#include "guictrl_api.h"
#include "guitext.h"
#ifndef MONOCHROME_LCD_SUPPORT 
#include "guires.h"
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
#include "mmipb_text.h"
#endif
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
#include "mmisms_voicemail.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMISET_CALL_BAR_OPEN,
    MMISET_CALL_BAR_CLOSE,
    MMISET_CALL_BAR_CANCEL_ALL,
    MMISET_CALL_BAR_MODIFY_PWD,
    MMISET_CALL_BAR_MAX
}MMISET_CALL_BAR_E;
typedef enum
{
    MMISET_CALL_CLIR_INVOCATION_E,
        MMISET_CALL_CLIR_SUPPRESSION_E,
        MMISET_CALL_CLIR_NETWORK_DECIDE_E,
        MMISET_CALL_CLIR_MAX
}MMISET_CLL_CLIR_E;

typedef enum
{
    NETWORK_TYPE_DM_E,          // 2G/3G自动双模优先3G
    NETWORK_TYPE_DM_G_E,        // 2G/3G自动双模优先2G
    NETWORK_TYPE_GPRS_E,        // 2g单模
    NETWORK_TYPE_TD_E,          // 3g单模
    NETWORK_TYPE_LTE_E,         // 4g单模
    NETWORK_TYPE_GPRS_3G_LTE_E, // 2G/3G/4G自动多模优先4G
    NETWORK_TYPE_GPRS_3G_LTE_TD_E, // 2G/3G/4G自动多模优先3G
    NETWORK_TYPE_GPRS_3G_LTE_GPRS_E, // 2G/3G/4G自动多模优先2G
    NETWORK_TYPE_GPRS_LTE_E,    // 2G/4G自动双模4G优先   
    NETWORK_TYPE_GPRS_LTE_G_E,  // 2G/4G自动双模2G优先
    NETWORK_TYPE_3G_LTE_E,      // 3G/4G自动双模4G优先
    NETWORK_TYPE_3G_LTE_TD,     // 3G/4G自动双模3G优先
    NETWORK_TYPE_MAX
}NETWORK_TYPE_E;

#ifdef MMI_CSP_SUPPORT
typedef enum
{
    CSP_MENU_NULL,
    CSP_MENU_CF, //call fowarding
    CSP_MENU_CFU, // cf unconditional
    //CSP_MENU_CFC, //cf conditional including cfb, cfnry, cfnrc
    CSP_MENU_CFB, // cf busy
    CSP_MENU_CFNRY, // cf no reply
    CSP_MENU_CFNRC, // cf no reachable
    CSP_MENU_CB, //call barring
    CSP_MENU_BAOC, //cb outgoing
    CSP_MENU_BAIC, // cb incomming
    CSP_MENU_BICROAM, //cb roaming
    CSP_MENU_BOIC, //cb international outgoing
    CSP_MENU_BOIC_EXHC, //boic except outgoing to home plmn 
    CSP_MENU_MAX
}CSP_MENU_SS_TYPE_E;

typedef enum
{
    CSP_MENU_OPER_NULL,
    CSP_MENU_OPER_ACTIVE,
    CSP_MENU_OPER_ACTIVE_WITH_PB,
    CSP_MENU_OPER_ACTIVE_WITH_VM,
    CSP_MENU_OPER_ACTIVE_WITH_NO,
    CSP_MENU_OPER_CANCEL,
    CSP_MENU_OPER_INTEROGATE,
    CSP_MENU_OPER_CANCEL_ALL,
    CSP_MENU_OPER_MODIFY_PASSWORD,
    CSP_MENU_OPER_MAX
}CSP_MENU_SS_OPER_E;
#endif


//@maggie add end   for cr79064



#define   MMISET_SEARCHNETWORK_SPACE_TIME   3000    //两次尝试切网间的空歇时间，3秒
//@maggie add begin for cr79064
#define     MMISET_SET_MAX_NETWORK_TYPE             4//@zhaohui modify for cr94108
#define       MMISET_SETRAT_MAX_NUM     7//尝试切网的次数

#define MMISET_FULLSCREEN_CLIENT_BOTTOM  (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMISET_FULLSCREEN_CLIENT_RIGHT   (MMI_MAINSCREEN_WIDTH - 1)

//多国语言下，大字体显示，18/20号字体

#define     MMISET_LABEL_HEIGH                      MMI_DEFAULT_FONT_HEIGHT
#define     MMISET_SS_INTERROGATE_NAME_BOTTOM       (MMISET_LABEL_HEIGH + MMI_CLIENT_RECT_TOP)
#define     MMISET_SS_INTERROGATE_STATUS_BOTTOM     (MMISET_SS_INTERROGATE_NAME_BOTTOM + MMISET_LABEL_HEIGH)
#define     MMISET_SS_INTERROGATE_NUMBER_BOTTOM     (MMISET_SS_INTERROGATE_STATUS_BOTTOM + MMISET_LABEL_HEIGH)

#define MMISET_USSD_SELECT_SIM_MAX_NUM        MMI_DUAL_SYS_MAX
// vt set record path
#define MMISET_VT_RECORD_HEIGHT 25
//rv & da
#define MMISET_VT_RECORD_RV_DA_LABEL_LEFT 10
#define MMISET_VT_RECORD_RV_DA_LABEL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_DA_LABEL_TOP (MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT + 5)
#define MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM (MMISET_VT_RECORD_RV_DA_LABEL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_RV_DA_DL_LEFT 10
#define MMISET_VT_RECORD_RV_DA_DL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_DA_DL_TOP (MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM + 10)
#define MMISET_VT_RECORD_RV_DA_DL_BOTTOM (MMISET_VT_RECORD_RV_DA_DL_TOP + MMISET_VT_RECORD_HEIGHT)
//da
#define MMISET_VT_RECORD_DA_LABEL_LEFT 10
#define MMISET_VT_RECORD_DA_LABEL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_DA_LABEL_TOP (MMISET_VT_RECORD_RV_DA_DL_BOTTOM + 10)
#define MMISET_VT_RECORD_DA_LABEL_BOTTOM (MMISET_VT_RECORD_DA_LABEL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_DA_DL_LEFT 10
#define MMISET_VT_RECORD_DA_DL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_DA_DL_TOP (MMISET_VT_RECORD_DA_LABEL_BOTTOM + 10)
#define MMISET_VT_RECORD_DA_DL_BOTTOM (MMISET_VT_RECORD_DA_DL_TOP + MMISET_VT_RECORD_HEIGHT)
//rv
#define MMISET_VT_RECORD_RV_LABEL_LEFT 10
#define MMISET_VT_RECORD_RV_LABEL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_LABEL_TOP (MMISET_VT_RECORD_DA_DL_BOTTOM + 10)
#define MMISET_VT_RECORD_RV_LABEL_BOTTOM (MMISET_VT_RECORD_RV_LABEL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_RV_DL_LEFT 10
#define MMISET_VT_RECORD_RV_DL_RIGHT (MMI_MAINSCREEN_WIDTH - 10)
#define MMISET_VT_RECORD_RV_DL_TOP (MMISET_VT_RECORD_RV_LABEL_BOTTOM + 10)
#define MMISET_VT_RECORD_RV_DL_BOTTOM (MMISET_VT_RECORD_RV_DL_TOP + MMISET_VT_RECORD_HEIGHT)

#define MMISET_VT_RECORD_DL_NUM 2 //dropdownlist的下拉数目“话机”“存储卡”

//vt local video setting
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_LEFT MMISET_VT_RECORD_RV_DA_LABEL_LEFT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_RIGHT MMISET_VT_RECORD_RV_DA_LABEL_RIGHT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_TOP MMISET_VT_RECORD_RV_DA_LABEL_TOP
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_BOTTON MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM

#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_LEFT MMISET_VT_RECORD_RV_DA_DL_LEFT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_RIGHT MMISET_VT_RECORD_RV_DA_DL_RIGHT
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_TOP MMISET_VT_RECORD_RV_DA_DL_TOP
#define MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_BOTTOM MMISET_VT_RECORD_RV_DA_DL_BOTTOM

#define MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM MMISET_VT_RECORD_DL_NUM

#define MMISET_VT_REPLACE_PIC_SIZE_MAX (32*1024)
#define MMISET_VT_REPLACE_PIC_WIDTH_MAX 320
#define MMISET_VT_REPLACE_PIC_HEIGHT_MAX 240

#define MMISET_PLMN_DIGITAL_MAX_LEN             (6)
#define MMISET_PLMN_PRIORITY_MAX_LEN             (3)
#define MMISET_CALLSET_ITEM_NUM_MAX             (3)

#ifdef MMI_PREFER_PLMN_SUPPORT
//记录当前操作的是哪一条plmn，主要用于优选网络列表中的属性修改。。。
LOCAL uint16 s_prefer_network_item = 0;
//记录优选网络列表更新的list
LOCAL MN_PHONE_PLMN_SEL_LIST_T *s_update_prefer_list_ptr = PNULL; 
#endif
#ifdef MMI_MULTISIM_COLOR_SUPPORT

#define SIM_COLOR_BUTTON_RECT_HEIGHT   44       //Button height
#define SIM_COLOR_BUTTON_RECT_WIDTH    49       //Button height
#define SIM_COLOR_BUTTON_FORM_HEIGHT   50       //button form height
#define SIM_COLOR_BUTTON_LINE_COUNT     2
typedef struct 
{
    MMI_CTRL_ID_T   button_line_form_id[SIM_COLOR_BUTTON_LINE_COUNT];
    MMI_CTRL_ID_T   button_ctrl_id[MAX_SIM_COLOR_NUM];
}SIMCOLOR_CTRL_LIST_T;
#endif

#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define SET_MINUTE_REMINDER_LABLE_TOP_MARGIN     1
#define SET_MINUTE_REMINDER_LABLE_HEIGHT     14
#else
#define SET_MINUTE_REMINDER_LABLE_TOP_MARGIN     5
#define SET_MINUTE_REMINDER_LABLE_HEIGHT     22
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT
#define DEFAULT_BTN_BORDER  {1,MMI_GRAY_COLOR,GUI_BORDER_NONE}
#define SELECTED_BTN_BORDER {3,GUI_RGB2RGB565(255, 100, 0),GUI_BORDER_SOLID}
#endif

#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMISET_MINUTE_REMINDER_REMAIN_TOP_MARGIN  1
#define MMISET_MINUTE_REMINDER_REMAIN_HEIGHT      22
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern MN_PHONE_PLMN_DETAIL_T  *g_plmn_detail_list_ptr;//选择网络的列表
extern MMISET_SS_PWD_INFO_T g_mmiset_ss_pwd;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
extern MMISET_SS_STATUS_T g_mmiset_ss_status;/*lint !e752*/
#endif
#endif
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*lint -esym(552,g_vmn_oper_type) */
PUBLIC MMISMS_VMN_OPER_TYPE_E  g_vmn_oper_type = 0; /*lint !e64*/  //查询语音信箱号是否为了通话转移

#ifdef MMI_DUALMODE_ENABLE 
LOCAL BOOLEAN is_rat_switching = FALSE;
//LOCAL MN_GMMREG_RAT_E s_current_sel_rat = MN_GMMREG_RAT_GPRS;//@$discuss,是否需要加宏？
#endif

#ifdef MMI_VOLTE_SUPPORT
LOCAL uint8 wait_timer_id = 0;

#ifdef MMI_MULTI_SIM_SYS_DUAL
#define VOLTE_MAX_ITEMS  2
#else
#define VOLTE_MAX_ITEMS  1
#endif
#endif

LOCAL BOOLEAN s_allow_rat_switching = TRUE;

LOCAL BOOLEAN s_allow_set_prefer_plmn = TRUE;

LOCAL BOOLEAN s_is_wizard_cfnrc = FALSE;
LOCAL uint16 s_re_switchrat_count = 0;//切网重试的次数
LOCAL uint8 s_searchnetwork_space_timer_id = 0;//重试切网间隔时间的Timer
LOCAL MN_PHONE_PLMN_STATUS_E s_plmn_status_before_manu_search = PLMN_NORMAL_GSM_GPRS_BOTH;//@fen.xie MS00174570
#ifdef MMI_IP_CALL_SUPPORT
LOCAL uint16 s_ipnumber_list_cur_id = 0;
LOCAL BOOLEAN s_is_add_ipnumber = FALSE; // true添加，false修改
#endif
LOCAL MMISET_CALL_BAR_E s_call_bar_operation = MMISET_CALL_BAR_MAX;
LOCAL MN_DUAL_SYS_E s_cur_dual_sys = 0;
LOCAL MMI_CTRL_ID_T s_set_divert_bar_wating_hideid_ctrl_id = MMISET_CALLDIVERT_FORM_CTRL_ID;
LOCAL uint8 g_call_reminder_time[CC_CALL_REMINDER_TIME_MAX] = 
{
    CC_CALL_REMINDER_TIME_OFF,
    CC_CALL_REMINDER_TIME_50S,
    CC_CALL_REMINDER_TIME_55S,
    CC_CALL_REMINDER_TIME_60S
};

/* cr00185468: User can change wallpaper number freely */
//MMI_IMAGE_ID_T g_fixed_pic_wallpaper[MMISET_WALLPAPER_FIXED_PIC_MAX_NUM] = {0};

#ifdef VT_SUPPORT
//vt replace 
LOCAL const MMI_TEXT_ID_T s_replace_title_text_id[MMISET_VT_REPLACE_MAX] = {TXT_SET_VT_REPLACE_LOCAL_IMAGE, TXT_SET_VT_REPLACE_REMOTE_IMAGE};
LOCAL const MMISET_VT_REPLACE_MEDIA_TYPE_E s_replace_media_type[MMISET_VT_REPLACE_MAX] = {MMISET_VT_REPLACE_MEDIA_TYPE_IMAGE, MMISET_VT_REPLACE_MEDIA_TYPE_IMAGE};
//LOCAL MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
#endif

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
LOCAL BOOLEAN s_is_cta_deactive_ps[MMI_DUAL_SYS_MAX] = {0};
LOCAL uint8 s_cta_deactive_ps_timer = 0;
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL int32 s_dualsys_cfnrc_win_close_time = 0;
LOCAL int32 s_dualsys_cfnrc_success_num = 0;
LOCAL BOOLEAN  s_is_action_dualsys_cfnrc  = FALSE; //记录是否正在进行双卡无法到达转移
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT
//Ctrl ID list
LOCAL const SIMCOLOR_CTRL_LIST_T simColor_CtrlID=
{
    {
        MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID,  //BTN_form_id
        MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID,
     },
    {                                               //BTN_ctrl_id[MAX_SIM_COLOR_NUM]    
        MMISET_SIM_COLOR_BTN_COLOR1_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR2_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR3_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR4_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR5_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR6_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR7_CTRL_ID,
        MMISET_SIM_COLOR_BTN_COLOR8_CTRL_ID,
    },
};

LOCAL uint16  s_current_selected_index = 0;
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

                                              

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
/*****************************************************************************/
//  Description : ImplementStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementStandbyChanged(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : AppendMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendMultiSysTypeList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : to handle multi sys time window msg
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMultiSysTypeWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : to handle sim active type when all sim is set to on type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMajorSimWhenAllSimOnWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        );

/*****************************************************************************/
//  Description : to handle answer with original sim type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnwerWithOriginalSimWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        );

/*****************************************************************************/
//  Description : 处理SIM SET NAME WIN MESSAGE
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimNameWindow(
                                           MMI_WIN_ID_T          win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E         msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           );

#ifdef MMI_MULTISIM_COLOR_SUPPORT
/*****************************************************************************/
//  Description :  Reset button border 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  ResetSimColorButtonBorder(MN_DUAL_SYS_E   dual_sys);

/*****************************************************************************/
//  Description :  Color button on click
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ColorButtonOnClick(MMI_WIN_ID_T  win_id,MN_DUAL_SYS_E   dual_sys,BOOLEAN is_selected);

/*****************************************************************************/
//  Description : Init ctrl settings
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E InitEditSimColorWindow(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : 处理SIM SET Color WIN MESSAGE
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimColorWindow(
                                           MMI_WIN_ID_T          win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E         msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           );
#endif

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
//  Description : open dual sim set cfnrc window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenDualsysCFNRCWin(void);

/*****************************************************************************/
//  Description : 处理dual sim set cfnrc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCWindow(
                                            MMI_WIN_ID_T          win_id,   // 窗口的ID
                                            MMI_MESSAGE_ID_E        msg_id,     // 窗口的内部消息ID
                                            DPARAM              param       // 相应消息的参数
                                            );

/*****************************************************************************/
//  Description : open cfnrc win
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenCFNRCWin(void);
#endif

/*****************************************************************************/
//  Description : 处理triple sim set cfnrc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMultiSimSettingWindow(
                                              MMI_WIN_ID_T          win_id,     // 窗口的ID
                                              MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                              DPARAM                param       // 相应消息的参数
                                              );

/*****************************************************************************/
//  Description : AppendPinSelectListItem
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendPinSelectListItem(MMISET_SIM_SELECT_TYPE_E select_type, MMI_CTRL_ID_T ctrl_id);
#endif

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : to handle CTA Multi Sys Standby window msg
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCTAMultiSysStandbyWindow(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
/*****************************************************************************/
//  Description : AppendCTAMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendCTAMultiSysTypeList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : ImplementCTAStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementCTAStandbyChanged(BOOLEAN * is_standby_ptr, BOOLEAN is_power_select);



/*****************************************************************************/
//  Description : ImplementCTASendSIMNotReadInd
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void ImplementCTASendSIMNotReadInd(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Standby 等待窗口
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCTAStandbyWaitingWindow (
                                           MMI_WIN_ID_T      win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           );
#endif
#endif




                                                                                                                                                                                          
/*****************************************************************************/
//  Description : to handle divert to voice box
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertVoiceWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );


/*****************************************************************************/
//  Description : to open bar settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void OpenBarSettingsWindow(void);

//@zhaohui add
/*****************************************************************************/
//  Description : to handle select divert call
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
#ifdef VT_SUPPORT
                                              
LOCAL MMI_RESULT_E HandleSelectCallTypeGeneralWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );                                            
#endif

/*****************************************************************************/
//  Description : set call diver voice or num title
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetCallDivertVoiceOrNumTitle(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : to handle open num divert call
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOpenNumDivertWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : get voice mail address
//  Global resource dependence : 
//  Author:bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetVoiceMailAddress(MN_DUAL_SYS_E dual_sys, MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type, MN_VOICE_MAIL_TYPE_E voice_mail_type);

/*****************************************************************************/
//  Description : to handle divert to phone number
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertNumberWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPopBarWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/                                             
LOCAL MMI_RESULT_E  HandleSelectPopWaitingWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/                                             
LOCAL MMI_RESULT_E  HandleSelectPopDivertWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallWaitingtSelectPopWindow(void);    


/*****************************************************************************/
//  Description : to handle call divert/bar/wating/hide id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallDivertWindow(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          );

/*****************************************************************************/
//  Description : handle call bar window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/                                                   
LOCAL MMI_RESULT_E  HandleCallBarWindow(
                                                          MMI_WIN_ID_T    win_id, 
                                                          MMI_MESSAGE_ID_E   msg_id, 
                                                          DPARAM             param
                                                          );
                                                          

/*****************************************************************************/
// Description : to handle set call hide id window
// Global resource dependence : 
// Author:wancan.you
// Note: MMISET_SET_CALL_HIDE_ID_WIN_TAB
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallHideIdWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM            param
                                              ); 
                                              
/*****************************************************************************/
//  Description : to handle input call bar password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBarPasswordWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );  
                                            
                                                                                       
/*****************************************************************************/
//  Description : to handle register new call bar password again
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordAgainWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       );  
                                                       
/*****************************************************************************/
//  Description : to select network
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSelectLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLockLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetSelectLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetLockLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallFowardVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallBarVisibleSimNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide cf cb menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSSMenuItemVisible(MN_DUAL_SYS_E dual_sys, CSP_MENU_SS_TYPE_E ss_type, CSP_MENU_SS_OPER_E ss_oper);

/*****************************************************************************/
// Description : set line lock
// Global resource dependence : 
// Author: samboo.shen
// Note:
/*****************************************************************************/
LOCAL void  SetLineLock(void);

/*****************************************************************************/
//  Description : to handle select line msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           );  

/*****************************************************************************/
// 	Description : to handle lock line msg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLockLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E 	msg_id,
                                           DPARAM 		        param
                                           );
#endif //MMI_CSP_SUPPORT

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
//  Description : handle network setting window msg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkSettingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle prefer network window msg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispPreferNetworkListWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
// 	Description : HandleDeletePreferPlmnQueryWindow
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeletePreferPlmnQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );

/*****************************************************************************/
//  Description : handle prefer network options menu msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkOptionMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlmnListDispWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCreatePlmnItemWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkPriorityChangeWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetWaitingWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : InitPlmnListCtrl
//  Global resource dependence : 
//  Author:michael.shi
//  Note: InitPlmnListCtrl
/*****************************************************************************/
LOCAL void InitPlmnListCtrl(MMI_CTRL_ID_T ctrl_id,uint16 max_item_num);

#if 0
/*****************************************************************************/
//  Description : is plmn already exists in prefer network list.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN isPlmnExistsAlready(MN_DUAL_SYS_E dual_sys,MN_PLMN_T plmn);
#endif

/*****************************************************************************/
//  Description : get prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetCurPreferNetworkIndex(void);

/*****************************************************************************/
//  Description : get prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL void SetCurPreferNetworkIndex(uint16 index);

/*****************************************************************************/
//  Description : isPlmnValid ,
//  Global resource dependence : 
//  Author:michael.shi
//  Note: if plmn is in list ,return FALSE; if plmn is not in list ,return TRUE;
/*****************************************************************************/
LOCAL BOOLEAN isPlmnValid(MN_DUAL_SYS_E dual_sys, MN_PLMN_T plmn);
#endif
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : list ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleListIpNumberWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
                                             


/*****************************************************************************/
//  Description : modify ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyIPNumberWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
//  Description : append ip number item to listbox
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItemToList(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMI_CTRL_ID_T ctrl_id
                                    );

/*****************************************************************************/
//  Description : append listbox item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItem(
                              MMI_CTRL_ID_T                 ctrl_id,
                              const MMISET_IP_NUMBER_INFO_T *ip_info_ptr,
                              uint16                        pos,
                              BOOLEAN                       is_update
                              );

/*****************************************************************************/
//  Description : handle ip number option menu window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );
#if defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : handle ip number add option menu window
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberAddOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );
#endif
#endif
/*****************************************************************************/
//  Description : 处理分钟提示音
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMinuteReminderWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );
/*****************************************************************************/
//  Description : 自定义分钟提示音窗口
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMinuteReminderEditWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );


/*****************************************************************************/
//  Description : 在进行手动选网中，网络连接窗口的处理函数
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleManualSelNetworkConnectWindow(
                                                       MMI_WIN_ID_T       win_id,   // 窗口的ID
                                                       MMI_MESSAGE_ID_E   msg_id,   // 窗口的内部消息ID
                                                       DPARAM             param     // 相应消息的参数
                                                       );

/*****************************************************************************/
//  Description : to handle remind user will select network through manual win msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelNetManualWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
#ifdef MMI_DUALMODE_ENABLE


/*****************************************************************************/
//  Description : to select network method
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkMethodWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );
#endif

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : to handle VT call setting window message
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTSettingsWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : to handle VT call fallback setting window message
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTFallbackSettingsWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   );

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTReplaceImageSettingsWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       );

/*****************************************************************************/
//  Description : 判断替代图片格式是否有效
//  Global resource dependence : none
//  Author: samboo
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsReplaceImageValid(MULTIM_SELECT_RETURN_T * multim_select_ptr);

/*****************************************************************************/
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 );

/*****************************************************************************/
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoOptWinMsg
(
 MMI_WIN_ID_T        win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 );
/*****************************************************************************/
//  Description : 设置VT录音存储位置
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTRecordPathSetWinMsg(
                                               MMI_WIN_ID_T        win_id,
                                               MMI_MESSAGE_ID_E    msg_id,
                                               DPARAM              param
                                               );

/*****************************************************************************/
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetVTRecordPathSettings(MMIFILE_DEVICE_E *device_table_ptr);


/*****************************************************************************/
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTRecordPathSettings(MMIFILE_DEVICE_E rvda_device, 
                                    MMIFILE_DEVICE_E da_device,
                                    MMIFILE_DEVICE_E rv_device);

/*****************************************************************************/
//  Description : to handle vt one time ring setting win
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTOneTimeRingSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : AppendOneTimeRingListItem
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/ 
LOCAL void AppendOneTimeRingListItem(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : SetOneTimeRing
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/
LOCAL void SetOneTimeRing(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 设置VT本地视频位置
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTLocalVideoSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           );

#if 0//bug337002
/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SetVTLocalVideoSettings(void);

/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTLocalVideoSettings(void);
#endif

#if 1//bug337002
/*****************************************************************************/
//  Description : to handle set Local Video
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTLocalVideoSetSelectWinMsg(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : create screensaver window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMISETAPI_CreateRemoteShutdownCameraSetWin(void);
#endif
#endif  

/*****************************************************************************/
//  Description : 处理自动重拨
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoRedialWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void MMISET_AppendSpeedDialDelayListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               );

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void SetSpeedDialDelay(
                              MMI_CTRL_ID_T     ctrl_id
                              );

/*****************************************************************************/
//  Description : 延时拨号处理
//  Global resource dependence : 
//  Author:yiwen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSpeedDialWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  );

/*****************************************************************************/
//  Description : to HandleSetCallSetOtherWindow
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSetCallSetOtherWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );  
                                               
                                          
/*****************************************************************************/
// Description : to handle set fly mode window msg idle
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeWindow(
                                        MMI_WIN_ID_T      win_id,
                                        MMI_MESSAGE_ID_E  msg_id,
                                        DPARAM            param
                                        );
                                        

/*****************************************************************************/
// Description : to handle set fly mode window msg
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeSetWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E  msg_id,
                                           DPARAM            param
                                           );
//@maggie add begin for cr79064
/*****************************************************************************/
//  Description : HandleNetworkTypeWindow
//  Global resource dependence : 
//  Author:maggie.ren
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkTypeWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
                                            
#ifdef MMI_VOLTE_SUPPORT
LOCAL MMI_RESULT_E  HandleSetVoLTEWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#if defined(MMI_GUI_STYLE_TYPICAL)
/*****************************************************************************/
//  Description : handle set any key and reject mode win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnykeyRejectWindow(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                );
#endif

/*****************************************************************************/
//  Description : handle set connect prompt remind win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConnectPromptWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );

#if 0
/*****************************************************************************/
//  Description : InsertOptMenuNodeByLabel
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InsertOptMenuNodeByLabel(
                                    MMI_CTRL_ID_T      ctrl_id,        //控件id
                                    MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                                    uint16             node_id,        //节点编号
                                    uint16             parent_node_id, //父节点编号
                                    uint16             index           //位置
                                    );                                                 
 #endif
 
/*****************************************************************************/
//  Description : 语音信箱号码界面打开时，等待界面不允许返回
//  Global resource dependence : 
//  Author: kelly.li
//  Note:2006-02-13
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVMForDivertWaitingWindow(
                                                  MMI_WIN_ID_T      win_id,     // 窗口的ID
                                                  MMI_MESSAGE_ID_E     msg_id,  // 窗口的内部消息ID
                                                  DPARAM                param       // 相应消息的参数
                                                  ); 
#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
//  Description : 翻盖接听
//  Global resource dependence : 
//  Author:jibin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFlipAnswerWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              ); 
#endif             
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : Is this ip number active
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
BOOLEAN IsActiveIPNumber(MN_DUAL_SYS_E dual_sys,uint16 cur_index);
#endif
/*****************************************************************************/
//  Description : to select no answer time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoAnswerTimeWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );   
                                             
/*****************************************************************************/
//     Description : add fly mode set list to list 
// Global resource dependence : 
//  Author: kelly.li
// Note:
/*****************************************************************************/
LOCAL void AppendFlyModeSetListItem(
                                    MMI_CTRL_ID_T      ctrl_id
                                    );  
                                    

/*****************************************************************************/
//     Description : replace some struct of the  GUILIST_ITEM_T
// Global resource dependence : 
//  Author: Qiang.Zhang
// Note:
/*****************************************************************************/
LOCAL void SetCallSetOthersItemCheck(
                        MMI_CTRL_ID_T ctrl_id,
                        BOOLEAN is_check,
                        uint16 index
                       );

#ifndef MMI_MULTI_SIM_SYS_SINGLE /* lint_lai */
/********************************************************************************
 NAME:      Stk Forbid Input Chars
 DESCRIPTION:
 AUTHOR: Michael.Shi
 DATE:      
********************************************************************************/
LOCAL void SetForbidInputChar(MMI_CTRL_ID_T ctrl_id);
#endif  


/*****************************************************************************/
//  Description : to handle interrogate ss status report
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllCFReportWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param);
/*****************************************************************************/
//  Description : to handle select popo win message
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/                                             
LOCAL MMI_RESULT_E  HandleCallSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                  
/*****************************************************************************/
// Description : to handle set fly mode window msg when open phone
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeOpenPhoneWindow(
                                                 MMI_WIN_ID_T      win_id,
                                                 MMI_MESSAGE_ID_E  msg_id,
                                                 DPARAM            param
                                                 );                                             
/*****************************************************************************/
//  Description : to handle plmn list
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlmnListWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );                                             
                                             
#if 0 //!defined(MMI_MULTI_SIM_SYS_SINGLE)

/*****************************************************************************/
//  Description :
//  Global resource dependence :                                
//      Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowHelpWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );
#endif

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
/*****************************************************************************/
//  Description : handle set major sim win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWizardMajorSimSelWindow(
                                                 MMI_WIN_ID_T          win_id,  // 窗口的ID
                                                 MMI_MESSAGE_ID_E       msg_id,     // 窗口的内部消息ID
                                                 DPARAM                 param       // 相应消息的参数
                                                 );

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WizardCFNRCSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectWinCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);


/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DispatchBySelectType(MN_DUAL_SYS_E dual_sys, MMISET_SIM_SELECT_TYPE_E select_type);
#endif                                             
/*****************************************************************************/
//  Description : to handle register new call bar password
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       );   
                                                       
/*****************************************************************************/
//  Description : GetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL NETWORK_TYPE_E GetNetworkType(MN_DUAL_SYS_E  dual_sys);  

#ifdef MMI_DUALMODE_ENABLE 
/*****************************************************************************/
//  Description : MMISET_AppendNetworkTypeListItem
//  Global resource dependence : 
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void AppendNetworkTypeListItem(
                                     MMI_CTRL_ID_T        ctrl_id,
                                     MN_DUAL_SYS_E  dual_sys
                                     );

/*****************************************************************************/
//  Description : MMISET_SetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void SetNetworkType(//@zhaohui modify for cr94108
                          MN_DUAL_SYS_E  dual_sys,
                          NETWORK_TYPE_E network_type, //用户的选择
                          BOOLEAN is_resend,        //是否是失败后的重发
                          BOOLEAN only_write_nv,
                          BOOLEAN is_reset_factorysetting
                          );
LOCAL void MMIAPISET_SetTestMode(MN_DUAL_SYS_E dual_sys, MN_PHONE_POWER_TEST_MODE_IND network_type);
#endif 

/********************************************************************************
NAME:           ResetReSwitchRatCount
DESCRIPTION:    
PARAM IN:       None
PARAM OUT:      
AUTHOR:     zhaohui
DATE:           2009.1.5
********************************************************************************/
LOCAL void ResetReSwitchRatCount(void);

/*****************************************************************************/
//  Description :网络搜索的窗口，判断是否是在切网状态
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsinSwitchRatState(void);



/*****************************************************************************/
//  Description :显示设置优先网络结果 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetPreferRatResultDisp(BOOLEAN result);

/*****************************************************************************/
//  Description : open call divert pop window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void OpenSetCallDivertWin(MN_DUAL_SYS_E    dual_sys);   

#if defined(MAINLCD_SIZE_176X220) || defined(MAINLCD_SIZE_128X160)
/*****************************************************************************/
// 	Description : 设置输入密码窗口的背景色，image 和title
//	Global resource dependence : 
//  Author: chunjie liu
//	Note:
/*****************************************************************************/
LOCAL void SetPwBackground(
                                 MMI_WIN_ID_T    win_id, //窗口的ID
                                 MMI_CTRL_ID_T   ctrl_id,   //the id of control
                                 BOOLEAN is_need_label
                                 );
/*****************************************************************************/
// 	Description : GetPinPukEditRect
//	Global resource dependence : 
//  Author: chunjie liu
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetMinuteReminderEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label);
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
// 	Description : Is Set DualSys MSISDN 
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSetDualSysMSISDN(void);

/*****************************************************************************/
//  Description : Handle Enter Edit MSISDN QueryWin
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterEditMSISDNQueryWin(
                                                MMI_WIN_ID_T       win_id,    
                                                MMI_MESSAGE_ID_E      msg_id,    
                                                DPARAM                param      
                                             );
#if 0
/*****************************************************************************/
// 	Description : to handle dualsys call divert help function, display the help infor
//	Global resource dependence : 
//   Author:  dave.ruan
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCHelpWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param);
#endif
#endif
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/


//开机设置飞行模式窗口
WINDOW_TABLE( MMISET_FLY_MODE_OPEN_PHONE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFlyModeOpenPhoneWindow ),    
        WIN_ID( MMISET_FLY_MODE_OPEN_PHONE_WIN_ID ),
        WIN_TITLE( TXT_SET_FLY_MODE ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_FLY_MODE_OPEN_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

// the window for call hide id
WINDOW_TABLE( MMISET_SET_CALL_HIDE_ID_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetCallHideIdWindow ),    
        WIN_ID( MMISET_SET_CALL_HIDE_ID_WIN_ID ),
        WIN_TITLE(TXT_SET_HIDE_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_CALL_HIDE_ID_MENU_CTRL_ID),
        END_WIN
};



WINDOW_TABLE( MMISET_SET_OPEN_DIVERT_WIN_TAB ) =  //语音信箱还是电话号码
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleOpenNumDivertWindow ),    
    WIN_ID( MMISET_SET_OPEN_DIVERT_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SET_SS_DIVERT_OPT, MMISET_VOICE_NUM_CTRL_ID),
    END_WIN
};



// the window for divert voice box
WINDOW_TABLE( MMISET_SET_DIVERT_VOICE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDivertVoiceWindow ),    
        WIN_ID( MMISET_SET_DIVERT_VOICE_WIN_ID ),
        WIN_TITLE( TXT_SET_DIVERT_VOICE ),
        CREATE_EDIT_PHONENUM_CTRL(MMISET_VOCIE_MAX_NUM,MMISET_SET_DIVERT_VOICE_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for divert phone number
WINDOW_TABLE( MMISET_SET_DIVERT_NUMBER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDivertNumberWindow ),    
    WIN_ID( MMISET_SET_DIVERT_NUMBER_WIN_ID ),
    WIN_TITLE( TXT_SET_DIVERT_NUMBER ),
    CREATE_EDIT_PHONENUM_CTRL(MMISET_VOCIE_MAX_NUM, MMISET_SET_DIVERT_NUMBER_CTRL_ID),
    WIN_SOFTKEY(STXT_MAIN_PB, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input call bar password
WINDOW_TABLE( MMISET_SET_BAR_PASSWORD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBarPasswordWindow ),    
        WIN_ID( MMISET_SET_BAR_PASSWORD_WIN_ID ),
        WIN_TITLE( TXT_INPUT_PASSWORD ),
        CREATE_EDIT_PASSWORD_CTRL(MMISET_BAR_PASSWORD_MAX_LEN,MMISET_SET_BAR_PASSWORD_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),    
        END_WIN
};

// the window for register new call bar password
WINDOW_TABLE( MMISET_SET_BAR_CHANGE_PW_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleRegisterNewBarPasswordWindow ),    
    WIN_ID( MMISET_SET_BAR_CHANGE_PW_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_PASSWORD_CTRL(MMISET_BAR_PASSWORD_MAX_LEN,MMISET_SET_BAR_CHANGE_PW_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for register new call bar password
WINDOW_TABLE( MMISET_SET_BAR_CHANGE_PW_AGAIN_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleRegisterNewBarPasswordAgainWindow ),    
    WIN_ID( MMISET_SET_BAR_CHANGE_PW_AGAIN_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_PASSWORD_CTRL(MMISET_BAR_PASSWORD_MAX_LEN,MMISET_SET_BAR_CHANGE_PW_AGAIN_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for select network
WINDOW_TABLE( MMISET_SET_SELECT_NETWORK_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectNetworkWindow ),    
    WIN_ID( MMISET_SET_SELECT_NETWORK_WIN_ID ),
    WIN_TITLE( TXT_NETWORK_SELECT ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SELECT_NETWORK_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
};

#ifdef MMI_PREFER_PLMN_SUPPORT
// the window for network setting
WINDOW_TABLE( MMISET_SET_NETWORK_SETTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNetworkSettingWindow ),    
    WIN_ID( MMISET_SET_NETWORK_SETTING_WIN_ID ),
    WIN_TITLE( TXT_NETWORK_SETTING ),
    CREATE_MENU_CTRL(MENU_SET_NETWORK_SETTING, MMISET_SET_NETWORK_SETTING_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for display prefer plmn list.
WINDOW_TABLE( MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleDispPreferNetworkListWinMsg ),    
    WIN_ID( MMISET_SET_PREFER_NETWORK_WIN_ID ),
    WIN_TITLE( TXT_SET_PREFER_NETWORK ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_NETWORK_PREFER_PLMN_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for prefer network list options menu
WINDOW_TABLE(MMISET_PREFER_NETWORK_OPT_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePreferNetworkOptionMsg ),    
    WIN_ID( MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SET_PREFER_NETWORK_OPT, MMISET_SET_PREFER_NETWORK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
//the window for prefer network list options menu
WINDOW_TABLE(MMISET_PREFER_NETWORK_LONGPENOK_OPT_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePreferNetworkOptionMsg ),    
    WIN_ID( MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SET_PREFER_NETWORK_LONGPENOK_OPT, MMISET_SET_PREFER_NETWORK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

//the window for display all plmn list .
WINDOW_TABLE(MMISET_NETWORK_PLMN_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePlmnListDispWinMsg ),    
    WIN_ID( MMISET_NETWORK_PLMN_LIST_WIN_ID ),
    WIN_TITLE( TXT_SET_NETWORK_PLMN_LIST ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMISET_NETWORK_PLMN_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for create plmn item.
WINDOW_TABLE(MMISET_NETWORK_NEW_PLMN_ITEM_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCreatePlmnItemWinMsg ),    
    WIN_ID( MMISET_NETWORK_CREATE_PLMN_ITEM_WIN_ID ),
    WIN_TITLE( TXT_NEW ), // new

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PLMN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PLMN_FORM1_CTRL_ID,MMISET_NETWORK_PLMN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_NETWORK_PLMN_LABEL_CTRL_ID,MMISET_NETWORK_PLMN_FORM1_CTRL_ID),
        CHILD_EDIT_DIGITAL_CTRL(TRUE,MMISET_PLMN_DIGITAL_MAX_LEN,MMISET_NETWORK_PLMN_EDIT_CTRL_ID,MMISET_NETWORK_PLMN_FORM1_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PLMN_FORM2_CTRL_ID,MMISET_NETWORK_PLMN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_NETWORK_PLMN_PRIORITY_LABEL_CTRL_ID,MMISET_NETWORK_PLMN_FORM2_CTRL_ID),
        CHILD_EDIT_DIGITAL_CTRL(TRUE,MMISET_PLMN_PRIORITY_MAX_LEN,MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID,MMISET_NETWORK_PLMN_FORM2_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for create plmn item.
WINDOW_TABLE(MMISET_NETWORK_CHANGE_PRIORITY_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePreferNetworkPriorityChangeWinMsg ),    
    WIN_ID( MMISET_NETWORK_PRIORITY_CHANGE_WIN_ID ),
    WIN_TITLE( TXT_COMMON_PRIORITY ), // new

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PRIORITY_CHANGE_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_NETWORK_PRIORITY_CHANGE_FORM1_CTRL_ID,MMISET_NETWORK_PRIORITY_CHANGE_FORM_CTRL_ID),
        CHILD_EDIT_DIGITAL_CTRL(TRUE,MMISET_PLMN_PRIORITY_MAX_LEN,MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID,MMISET_NETWORK_PRIORITY_CHANGE_FORM1_CTRL_ID),
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

//@Lin.Lin add for set ip number, start
// the window for list ip number
#ifdef MMI_IP_CALL_SUPPORT
WINDOW_TABLE( MMISET_LIST_IPNUMBER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleListIpNumberWindow ),    
        WIN_ID( MMISET_LIST_IPNUMBER_WIN_ID ),
        WIN_TITLE( TXT_SET_IP_NUMBER ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_LIST_IPNUMBER_CTRL_ID),
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        END_WIN
};


// the window for modify ip number
WINDOW_TABLE( MMISET_MODIFY_IPNUMBER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleModifyIPNumberWindow ),    
    WIN_ID( MMISET_MODIFY_IPNUMBER_WIN_ID ),
    WIN_TITLE( TXT_SET_IP_NUMBER ),
    CREATE_EDIT_PHONENUM_CTRL(MMISET_IPNUMBER_MAX_LEN,MMISET_MODIFY_IPNUMBER_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
// 分钟提示音窗口
WINDOW_TABLE( MMISET_SET_MINUTE_REMINDER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetMinuteReminderWindow ),
    WIN_ID( MMISET_SET_MINUTE_REMINDER_WIN_ID ),
    WIN_TITLE( TXT_SET_CALL_TIME_REMINDER ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_MINUTE_REMINDER_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
//分钟提示音时间自定义窗口
WINDOW_TABLE(MMISET_SET_MINUTE_REMINDER_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleSetMinuteReminderEditWindow),  
    WIN_ID( MMISET_SET_MINUTE_REMINDER_EDIT_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SET_CALL_TIME_REMINDER),
    
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID),
    CREATE_EDIT_DIGITAL_CTRL(2, MMISET_SET_MINUTE_REMINDER_EDITBOX_CTRL_ID),
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

//提示用户即将进行手动选网 
WINDOW_TABLE(MMISET_SEL_NET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleSelNetManualWinMsg ),    
        WIN_ID(MMISET_SEL_NET_WIN_ID),
        WIN_TITLE(TXT_NETWORK_SELECT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMISET_SEL_NET_MANUAL_CTRL_ID),
        END_WIN
};



#ifdef MMI_DUALMODE_ENABLE

#ifdef VT_SUPPORT
// the window for VT call setting 
WINDOW_TABLE( MMISET_SET_VT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTSettingsWindow ),    
        WIN_ID( MMISET_SET_VT_SETTINGS_WIN_ID ),
        WIN_TITLE(TXT_SET_VIDEOPHONE),
        CREATE_MENU_CTRL(MENU_SET_VT_OPT, MMISET_CALL_VT_SETTINGS_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for VT call fallback setting 
WINDOW_TABLE( MMISET_SET_VT_FALLBACK_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTFallbackSettingsWindow ),    
        WIN_ID( MMISET_SET_VT_FALLBACK_WIN_ID ),
        WIN_TITLE(TXT_SET_VT_FALLBACK_HANDLE),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_CALL_VT_FALLBACK_SETTINGS_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// the window for VT call resplace image setting 
WINDOW_TABLE( MMISET_SET_VT_REPLACE_IMAGE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTReplaceImageSettingsWindow ),
        WIN_ID( MMISET_SET_VT_REPLACE_IMAGE_WIN_ID ),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
        WIN_TITLE(TXT_SET_VT_REPLACE_IMAGE),
        //    CREATE_RICHTEXT_CTRL(MMIVT_REPLACE_IMAGE_RICHTEXT_CTRL_ID),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_OPTION_SPECIFY, TXT_NULL, STXT_RETURN),
        END_WIN
};

// 预览自定义图片窗口
WINDOW_TABLE( MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_TAB ) = 
{
    WIN_ID( MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_ID),
    WIN_TITLE( TXT_SET_VT_REPLACE_IMAGE ),
    WIN_FUNC( (uint32)HandleReviewCustomPhotoWinMsg),
    CREATE_ANIM_CTRL(MMISET_VT_REPLACE_REVIEW_PHOTO_ANIM_CTRL_ID,MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};


WINDOW_TABLE( MMISET_VT_REPLACE_REVIEW_PHOTO_OPT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_ID(MMISET_VT_REPLACE_REVIEW_PHOTO_OPT_WIN_ID),
        WIN_TITLE(TXT_NULL),
        WIN_FUNC( (uint32)HandleReviewCustomPhotoOptWinMsg),
        CREATE_MENU_CTRL(MENU_SET_VT_CUSTOM_PHOTO_OPT, MMISET_VT_CUSTOM_PHOTO_OPT_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

//设置VT录制存储位置窗口
WINDOW_TABLE( MMISET_VT_RECORD_PATH_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleVTRecordPathSetWinMsg), 
        WIN_ID( MMISET_VT_RECORD_PATH_WIN_ID ),
        WIN_TITLE(TXT_SET_SPCIFY_SAVE_PATH),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),

        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_VT_RECORD_FORM_CTRL_ID),
        //RV & DA
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID,MMISET_VT_RECORD_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_RECORD_RV_DA_LABEL_CTRL_ID,MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID, MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID),
        
        //RV
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_VT_RECORD_RV_FORM_CTRL_ID,MMISET_VT_RECORD_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_RECORD_RV_LABEL_CTRL_ID,MMISET_VT_RECORD_RV_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID, MMISET_VT_RECORD_RV_FORM_CTRL_ID),
        
        //DA
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_VT_RECORD_DA_FORM_CTRL_ID,MMISET_VT_RECORD_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_RECORD_DA_LABEL_CTRL_ID,MMISET_VT_RECORD_DA_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID, MMISET_VT_RECORD_DA_FORM_CTRL_ID),
        WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
        END_WIN
};

// the window for VT one time ring setting
WINDOW_TABLE( MMISET_SET_VT_ONE_TIME_RING_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVTOneTimeRingSettingsWindow ),    
    WIN_ID( MMISET_SET_VT_ONE_TIME_RING_WIN_ID ),
    WIN_TITLE(TXT_SET_VT_ONE_TIME_RING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_VT_ONE_TIME_RING_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    END_WIN
};

//设置VT本地视频窗口
WINDOW_TABLE( MMISET_SET_VT_LOCAL_VIDEO_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleVTLocalVideoSetWinMsg), 
    WIN_ID( MMISET_VT_LOCAL_VIDEO_WIN_ID ),
    WIN_TITLE(TXT_SET_VT_LOCAL_VIDEO),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID),
    //对方关闭摄像头
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID),
#if 1//bug337002
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,FALSE,MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
#else
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID, MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID),
               
    WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
#endif
    END_WIN
};
#if 1//bug337002
WINDOW_TABLE( MMISET_SET_REMOTESHUTDOWNCAMERASET_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleVTLocalVideoSetSelectWinMsg ),    
        WIN_ID( MMISET_SET_REMOTESHUTDOWNCAMERASET_WIN_ID ),        
        WIN_TITLE( TXT_COMM_REMOTE_CAMERA_OFF ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_VT_REMOTE_SHUTDOWN_CAMERA_SET_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};
#endif
#endif
WINDOW_TABLE(MMISET_NETWORK_METHOD_WIN_TAB) = 
{ 
    WIN_FUNC( (uint32)HandleSelectNetworkMethodWindow ),
        WIN_ID( MMISET_SET_NETWORK_METHOD_WIN_ID ),
        WIN_TITLE( TXT_SET_NETWORK_METHOD ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_NETWORK_METHOD_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};
#endif



// 自动重拨窗口
WINDOW_TABLE( MMISET_SET_AUTO_REDIAL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetAutoRedialWindow ),    
        WIN_ID( MMISET_SET_AUTO_REDAIL_WIN_ID ),
        WIN_TITLE( TXT_SET_AUTO_REDAIL ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_AUTO_REDIAL_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

// 延时拨号
WINDOW_TABLE( MMISET_SET_SPEED_DIAL_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetSpeedDialWindow ),
        WIN_ID( MMISET_SET_SPEED_DIAL_WIN_ID ),
        WIN_TITLE( TXT_SPEED_DIAL ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SPEED_DIAL_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#if !defined MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISET_SET_CALL_OTHER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetCallSetOtherWindow ),    
        WIN_ID( MMISET_SET_Call_Other_WIN_ID ),
        WIN_TITLE(TXT_TOOLS_OTHERS),
#if defined(MMI_PDA_SUPPORT) 
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
#else
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_OTHER_SETTING_CTRL_ID),
        END_WIN
};
#else
WINDOW_TABLE( MMISET_SET_CALL_OTHER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetCallSetOtherWindow ),    
        WIN_ID( MMISET_SET_Call_Other_WIN_ID ),
        WIN_TITLE(TXT_TOOLS_OTHERS),
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_IP_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_IP_KEY_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_IP_FORM_CTRL_ID),//ip

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_CALL_MINUTE_REMINDER_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID),//语音通话每分钟提示音
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID),//语音通话每分钟提示音
        
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_AUTO_CONNECT_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_AUTO_CONNECT_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VIBRATE_TIP_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_VIBRATE_TIP_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID),

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_ANSWER_MODE_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_ANSWER_MODE_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_ANSWER_MODE_FORM_CTRL_ID),
#endif

#ifdef MV_SUPPORT 
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_MAGIC_VOICE_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_MAGIC_VOICE_LIST_CTRL_ID,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID),
#endif
#ifdef TTS_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VOICE_REPORT_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_VOICE_REPORT_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID),
#endif
#if 0//def TOUCH_PANEL_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_VIBRATE_TP_REPORT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_VIBRATE_TP_LABEL_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TP_REPORT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_VIBRATE_TP_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_VIBRATE_TP_REPORT_FORM_CTRL_ID),
#endif
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_FLY_MODE_SELECT_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID,MMISET_CALL_OTHER_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_FLY_MODE_SELECT_ON_POWER_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID,MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID),
#endif
        END_WIN
};
#endif

#ifdef MMI_CSP_SUPPORT
//the window for select line
WINDOW_TABLE( MMISET_SELECT_LINE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectLineWindow ),    
        WIN_ID( MMISET_LINE_SELECT_WIN_ID ),
        WIN_TITLE( TXT_SET_LINE_SELECT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_LINE_SELECT_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

WINDOW_TABLE( MMISET_LOCK_LINE_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleLockLineWindow ),    
        WIN_ID( MMISET_LOCK_LINE_WIN_ID ),
        WIN_TITLE( TXT_SET_LINE_LOCK ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_LOCK_LINE_LIST_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#endif //MMI_CSP_SUPPORT


//设置飞行模式窗口
WINDOW_TABLE( MMISET_FLY_MODE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFlyModeWindow ),    
        WIN_ID( MMISET_FLY_MODE_WIN_ID ),
        WIN_TITLE( TXT_SET_FLY_MODE ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_FLY_MODE_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

WINDOW_TABLE( MMISET_FLY_MODE_SET_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFlyModeSetWindow ),    
        WIN_ID( MMISET_FLY_MODE_SET_WIN_ID ),
        WIN_TITLE( TXT_SET_FLY_MODE ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_FLY_MODE_SET_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
        END_WIN
};

#ifdef MMI_VOLTE_SUPPORT
WINDOW_TABLE(MMISET_VOLTE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSetVoLTEWindow),
    WIN_ID(MMISET_SET_VOLTE_WIN_ID),
    WIN_TITLE( TXT_LIST_SET_VOLTE ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_VOLTE_LIST_CTRL_ID),
    END_WIN	
};
#endif

//@maggie add begin for cr79064
WINDOW_TABLE( MMISET_NETWORK_TYPE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNetworkTypeWindow ),    
        WIN_ID( MMISET_SET_NETWORK_TYPE_WIN_ID ),
        WIN_TITLE( TXT_NETWORK_TYPE ),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_NETWORK_TYPE_CTRL_ID),
        END_WIN
};

#if defined(MMI_GUI_STYLE_TYPICAL)
//设置接听拒接模式窗口
WINDOW_TABLE( MMISET_SET_ANYKEY_REJECT_MODE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetAnykeyRejectWindow ),
    WIN_ID( MMISET_SET_ANYKEY_REJECT_MODE_WIN_ID ),
    WIN_TITLE( TXT_SET_ANYKEY_REJECT_MODE ),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SET_ANYKEY_REJECT_MODE_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
};
#endif

//接通提示音窗口
WINDOW_TABLE( MMISET_SET_CONNECT_PROMPT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetConnectPromptWindow ),
        WIN_ID( MMISET_SET_CONNECT_PROMPT_WIN_ID ),
        WIN_TITLE( TXT_SET_CONNECT_PROMPT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_CONNECT_PROMPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};


#ifdef MMI_IP_CALL_SUPPORT
WINDOW_TABLE( MMISET_IPNUMBER_ACTIVE_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleIpNumberOptionWindow),
        WIN_ID(MMISET_IPNUMBER_OPTION_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_IPNUMBER_ACTIVE_OPT,MMISET_IPNUMBER_OPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE( MMISET_IPNUMBER_DEACTIVE_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleIpNumberOptionWindow),
        WIN_ID(MMISET_IPNUMBER_OPTION_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_IPNUMBER_DEAVTIVE_OPT,MMISET_IPNUMBER_OPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

#if defined(MMI_PDA_SUPPORT)
WINDOW_TABLE( MMISET_IPNUMBER_ADD_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleIpNumberAddOptionWindow),
        WIN_ID(MMISET_IPNUMBER_ADD_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_IPNUMBER_ADD_OPT,MMISET_IPNUMBER_ADD_OPT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};
#endif
#endif

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
// the window for set time
WINDOW_TABLE( MMISET_SET_SIM_NAME_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditSimNameWindow ),    
        WIN_ID( MMISET_SET_SIM_NAME_WIN_ID ),
        WIN_TITLE( TXT_SET_SIM_NAME ),
#if !defined MMI_GUI_STYLE_TYPICAL 
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        CREATE_EDIT_TEXT_CTRL(0,MMISET_SET_SIM_NAME_CTRL_ID),
        
        END_WIN
};

WINDOW_TABLE( MMISET_SET_MULTISYS_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_SIM_ENABLE_SEL ),
    WIN_FUNC( (uint32) HandleSetMultiSysTypeWindow),    
    WIN_ID( MMISET_SET_MULTISYS_TYPE_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_DAULSYS_TYPE_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_NULL),
    WIN_FUNC( (uint32)HandleSetMajorSimWhenAllSimOnWindow ),    
        WIN_ID( MMISET_SET_DUALSYS_SIM_ACTIVE_TYPE_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_DUALSYS_SIM_ACTIVE_TYPE_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_SET_REPLY_BY_ORIGINAL_SIM ),
        WIN_FUNC( (uint32)HandleSetAnwerWithOriginalSimWindow ),    
        WIN_ID( MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_CTRL_ID),
        END_WIN
};
/*	//Sam.hua	use SelectSimFunc 
//显示使用SIM卡拨出的选项                                                
WINDOW_TABLE( MMIUSSD_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUSSDSIMSelectWinMsg),    
        WIN_ID(MMISET_USSD_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SIM_SEL),
#if defined(MMI_PDA_SUPPORT) 
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_USSD_SELECT_SIM__CTRL_ID),
        END_WIN
};



//设置向导中无应答转移设置选卡
WINDOW_TABLE( MMISET_WIZARD_CFNRC_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleWizardCFNRCSIMSelectWinMsg),    
        WIN_ID(MMISET_WIZARD_CFNRC_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SET_DIVERTS_NOT_REACHABLE),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_WIZARD_CFNRC_SELECT_SIM__CTRL_ID),
        END_WIN
};

*/

WINDOW_TABLE( MMISET_MULTI_SIM_SETTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleMultiSimSettingWindow ),    
        WIN_ID( MMISET_SET_MULTI_SYS_SETTING_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL 
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_MULTI_SIM_SETTING_CTRL_ID),
#else
        CREATE_MENU_CTRL(MENU_SET_MULTI_SIM_SETTING, MMISET_SET_MULTI_SIM_SETTING_CTRL_ID),
#endif
        END_WIN
};

WINDOW_TABLE( MMISET_DUAL_MAJOR_SIM_SEL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleWizardMajorSimSelWindow ),    
        WIN_ID( MMISET_DUAL_MAJOR_SIM_SEL_WIN_ID ),
        WIN_TITLE(TXT_SET_SIM_NAME),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_SET_WIZARD_MAJOR_SIM_SEL, MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID),
        END_WIN
};

#ifdef MMI_MULTISIM_COLOR_SUPPORT
// the window for set sim color window
WINDOW_TABLE( MMISET_SET_SIM_COLOR_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditSimColorWindow ),    
    WIN_ID( MMISET_SET_SIM_COLOR_WIN_ID ),
    WIN_TITLE( TXT_SET_SIM_COLOR ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SIM_COLOR_SIM_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SIM_COLOR_BTN_FORM_CTRL_ID,MMISET_SIM_COLOR_SIM_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID,MMISET_SIM_COLOR_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR1_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR2_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR3_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR4_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE1_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID,MMISET_SIM_COLOR_BTN_FORM_CTRL_ID),    
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR5_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR6_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR7_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,PNULL,MMISET_SIM_COLOR_BTN_COLOR8_CTRL_ID,MMISET_SIM_COLOR_BTN_LINE2_FORM_CTRL_ID),
    
    END_WIN
};
#endif
// add for dual sys end
#endif //!defined(MMI_MULTI_SIM_SYS_SINGLE)

#ifdef FLIP_PHONE_SUPPORT
// 翻盖接听窗口
WINDOW_TABLE( MMISET_SET_FLIP_ANSWER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetFlipAnswerWindow ),
        WIN_ID( MMISET_SET_FLIP_ANSWER_WIN_ID ),
        WIN_TITLE( TXT_SET_FLIP_ANSWER ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_FLIP_ANSWER_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif

// the window for select network plmn list
WINDOW_TABLE( MMISET_SET_PLMN_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePlmnListWindow ),    
        WIN_ID( MMISET_SET_PLMN_LIST_WIN_ID ),
        WIN_TITLE( TXT_SET_MANUAL_SEL ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_PLMN_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
        END_WIN
};


// the window for select no anwer time
WINDOW_TABLE( MMISET_SET_NO_ANSWER_TIME_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNoAnswerTimeWindow ),    
    WIN_ID( MMISET_SET_NO_ANSWER_TIME_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE( TXT_NO_REPLY_TIME ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_NO_ANSWER_TIME_CTRL_ID),
    END_WIN
};
// the window for interrogate all call foward report
WINDOW_TABLE( MMISET_SS_ALL_REPORT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleAllCFReportWindow ),    
        WIN_ID( MMISET_SS_ALL_REPORT_WIN_ID ),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
        WIN_TITLE( TXT_SET_QUERY_STATUS ),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_RICHTEXT_CTRL(MMISET_SS_ALL_REPORT_CTRL_ID),
        END_WIN
};



WINDOW_TABLE( MMISET_SELECT_POP_DIVERT_WIN_TAB ) =  //ADDED BY MARY
{
    //CLEAR_LCD,
    WIN_FUNC( (uint32)HandleSelectPopDivertWindow),    
    WIN_ID( MMISET_SLECT_POP_DIVERT_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_DIVERTS),
    WIN_STYLE(WS_HAS_TRANSPARENT),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_POPMENU_CALLSETTING_DIVERT_ID),
    END_WIN
};


WINDOW_TABLE( MMISET_SELECT_POP_WAITING_WIN_TAB ) =  //ADDED BY MARY
{
    //CLEAR_LCD,
    WIN_FUNC( (uint32)HandleSelectPopWaitingWindow),    
    WIN_ID( MMISET_SLECT_POP_WAITING_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_WAITING),
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_POPMENU_CALLSETTING_WAITING_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_SELECT_POP_BAR_WIN_TAB ) =  //ADDED BY MARY
{
    //CLEAR_LCD,
    WIN_FUNC( (uint32)HandleSelectPopBarWindow),    
    WIN_ID( MMISET_SLECT_POP_BAR_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_BAR),
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_POPMENU_CALLSETTING_BAR_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_CALL_BAR_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallBarWindow ),    
    WIN_ID( MMISET_SET_CALL_BAR_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_BAR),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_BAR_MENU_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_CALL_BAR_OPT, MMISET_CALL_BAR_MENU_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISET_SET_CALL_DIVERT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallDivertWindow ),    
    WIN_ID( MMISET_SET_CALL_DIVERT_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_DIVERTS),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALL_DIVERT_MENU_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_CALL_DIVERTS_OPT, MMISET_CALL_DIVERT_MENU_CTRL_ID),
#endif
    END_WIN
};

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
WINDOW_TABLE( MMISET_CTA_MULTI_SYS_STANDBY_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SIM_ENABLE_SEL ),
    WIN_FUNC( (uint32) HandleCTAMultiSysStandbyWindow),    
    WIN_ID( MMISET_SET_CTA_MULTISYS_STANDBY_WIN_ID ),

#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_OK),
#else
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CTA_MULTI_SYS_STANDBY_CTRL_ID),
    END_WIN
};
#endif

//@zhaohui add start
//the window for divert or restrict(menu:"open,close,inquire"  for audio call)

#ifdef VT_SUPPORT //modify by mary

WINDOW_TABLE( MMISET_SET_DIVERT_AUDIOORVIDEO_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSelectCallTypeGeneralWindow ),    
        WIN_ID( MMISET_SET_SELECT_DIVERT_WIN_ID ),
        WIN_TITLE(TXT_SET_CALL_DIVERTS),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#else
        CREATE_MENU_CTRL(MENU_SET_DIVERT_CALLS_OPT, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#endif
        END_WIN
};

WINDOW_TABLE( MMISET_SET_BAR_AUDIOORVIDEO_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSelectCallTypeGeneralWindow ),    
        WIN_ID( MMISET_SET_SELECT_DIVERT_WIN_ID ),
        WIN_TITLE(TXT_SET_CALL_BAR),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#else
        CREATE_MENU_CTRL(MENU_SET_BARRING_CALLS_OPT, MMISET_SET_CALL_MENU_EX_CTRL_ID),
#endif
        END_WIN
};

#endif




WINDOW_TABLE( MMISET_SET_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallSettingsWindow ),    
        WIN_ID( MMISET_SET_CALL_WIN_ID ),
        WIN_TITLE(TXT_SETTINGS_CALL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if !defined MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_CALL_MENU_CTRL_ID),
#else
        CREATE_MENU_CTRL(MENU_SET_CALL_OPT, MMISET_SET_CALL_MENU_CTRL_ID),
#endif
        END_WIN
};

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
#if 0
//使用说明窗口
WINDOW_TABLE( MMISET_SET_DUALSYS_DIVERT_HELP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleDualsysCFNRCHelpWinMsg ),    
    WIN_ID( MMISET_SET_DUALSY_DIVERT_HELP_WIN_ID ),
    WIN_TITLE(TXT_HELP),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
    CREATE_TEXT_CTRL(MMISET_HELP_TEXT_CTRL_ID),
    END_WIN
}; 
#endif
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISET_SET_DIVERT_NUMBER_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
}


#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCallSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_IMAGE_ID_T image_id,   //icon
                                 uint32         menu_id   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

     //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

     //icon
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = menu_id;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : InitPdaCallSettingsCtrl
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallSettingsCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_CALL_MENU_CTRL_ID;

    uint16 list_max = 7;
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    list_max++;
#endif

#ifndef SUBLCD_SIZE_NONE
    list_max++;
#endif

#ifdef VT_SUPPORT
    list_max++;
#endif

    GUILIST_SetMaxItem(ctrl_id, list_max, FALSE );
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
#if defined(MMI_MULTI_SIM_SYS_DUAL)
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DUAL_SIM_SETTING, IMAGE_NULL, ID_SET_MULTI_SIM_SETTING);
#else
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_MULTI_SIM_SETTING, IMAGE_NULL, ID_SET_MULTI_SIM_SETTING);
#endif
#endif
#ifndef SUBLCD_SIZE_NONE
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_FLIP_ANSWER, IMAGE_NULL, ID_SET_FLIP_ANSWER);
#endif
#ifdef MMI_CSP_SUPPORT
    if(GetCallFowardVisibleSimNum(PNULL, 0) > 0)
#endif //MMI_CSP_SUPPORT        
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_CALL_DIVERTS, IMAGE_NULL, ID_SET_CALL_DIVERTS);
    }    
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_CALL_WAITING, IMAGE_NULL, ID_SET_CALL_WAITING);
#ifdef MMI_CSP_SUPPORT
    if(GetCallBarVisibleSimNum(PNULL, 0) > 0)
#endif //MMI_CSP_SUPPORT        
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_CALL_BAR, IMAGE_NULL, ID_SET_CALL_BAR);
    }
#ifdef VT_SUPPORT
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_VIDEOPHONE, IMAGE_NULL, ID_SET_VIDEOPHONE);
#endif
#ifdef MMI_CSP_SUPPORT
    if(GetSelectLineVisibleSimNum(PNULL, 0) > 0)
#endif
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_LINE_SELECT, IMAGE_NULL, ID_SET_LINE_SELECT);
    }
#ifdef MMI_CSP_SUPPORT
    if(GetLockLineVisibleSimNum(PNULL, 0) > 0)
#endif
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_LINE_LOCK, IMAGE_NULL, ID_SET_LINE_LOCK);
    }
    //MMI_CSP_SUPPORT
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_HIDE_ID, IMAGE_NULL, ID_SET_HIDE_ID);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_TOOLS_OTHERS, IMAGE_NULL, ID_SET_OTHER);
}

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
     MMISET_SIM_SELECT_TYPE_E    select_type =   0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_CALL_MENU_CTRL_ID;
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   
    if (sim_num == 1)//cr227781 maryxiao
    {
        s_cur_dual_sys = (MN_DUAL_SYS_E)sim_sys; 
    }  
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaCallSettingsCtrl();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:           
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);

        switch (menu_id)
        {
#ifndef MMI_MULTI_SIM_SYS_SINGLE     
        case ID_SET_MULTI_SIM_SETTING:
            MMK_CreateWin((uint32 *)MMISET_MULTI_SIM_SETTING_WIN_TAB, PNULL);
            break;  
#endif
        case ID_SET_CALL_DIVERTS:
            MMIAPISET_OpenDivertSettingsWindow();
            break;
            
        case ID_SET_CALL_BAR:
            OpenBarSettingsWindow();
            break;
            
        case ID_SET_CALL_WAITING:
            MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLWATINGT_FORM_CTRL_ID);
            select_type = MMISET_CALL_WAITING;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                CreateCallWaitingtSelectPopWindow();
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;
#ifdef VT_SUPPORT
#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_VIDEOPHONE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_WIN_TAB,PNULL);        
            break;
#endif
#endif   
#ifdef MMI_CSP_SUPPORT
        case ID_SET_LINE_SELECT:
            MMIAPISET_SetLineInfo();
            break;

        case ID_SET_LINE_LOCK: //line lock
            //verify pin2 before open line lock setting win
            SetLineLock();
            break;            
#endif


	/* +qingfeng.dong */
        case ID_SET_NETWORK://ID_SET_NETWORK
            select_type = MMISET_SET_NETWORK;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMIAPISET_OpenSelNetWorkWin((MN_DUAL_SYS_E)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;

#ifndef SUBLCD_SIZE_NONE
        case ID_SET_FLIP_ANSWER:
            MMIAPISET_OpenUssdSelectWin(MMISET_FLIP_ANSWER);
            break;
#endif

#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_NETSETTING:
            MMIAPISET_OpenSelNetWorkWin((MN_DUAL_SYS_E)sim_sys);
            break;
#endif

        case ID_SET_FLY_MODE:
#ifdef MCARE_V31_SUPPORT
            if(MCareV31_Exit_Confirm())
            {
                return MMI_RESULT_FALSE;		
            }
#endif
            MMK_CreateWin((uint32*)MMISET_FLY_MODE_SET_WIN_TAB,PNULL);
            break;
	/* -qingfeng.dong */
			
        case ID_SET_HIDE_ID:
            select_type = MMISET_CALL_HIDE_ID;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMK_CreatePubListWin((uint32*)MMISET_SET_CALL_HIDE_ID_WIN_TAB, (ADD_DATA)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;

        case ID_SET_OTHER:
            MMK_CreateWin((uint32*)MMISET_SET_CALL_OTHER_WIN_TAB, PNULL);
            break;
            
  
        default:
            break;
        }
        break;
        
    case MSG_GET_FOCUS:
         index = GUILIST_GetCurItemIndex(ctrl_id); 
         GUILIST_RemoveAllItems(ctrl_id);
         InitPdaCallSettingsCtrl();
         GUILIST_SetCurItemIndex(ctrl_id, index); 
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
/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_MENU_GROUP_ID_T         group_id    =   0;
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
     MMISET_SIM_SELECT_TYPE_E    select_type =   0;
    // MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   
    if (sim_num == 1)//cr227781 maryxiao
    {
        s_cur_dual_sys = (MN_DUAL_SYS_E)sim_sys; 
    }    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_GetId(MMISET_SET_CALL_MENU_CTRL_ID,&group_id,&menu_id);
        
        MMK_SetAtvCtrl(win_id, MMISET_SET_CALL_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#ifdef MMI_CSP_SUPPORT
        {
            MMI_CTRL_ID_T ctrl_id = MMISET_SET_CALL_MENU_CTRL_ID;
            uint32 group = MENU_SET_CALL_OPT;
            
            if(GetCallFowardVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_CALL_DIVERTS, FALSE);
            
            if(GetCallBarVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_CALL_BAR, FALSE);
            }
            if(GetSelectLineVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_LINE_SELECT, FALSE);
            }
            if(GetLockLineVisibleSimNum(PNULL, 0) > 0)
            {
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_LINE_LOCK, FALSE);
            }
        }     
#endif        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMISET_SET_CALL_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
#ifndef MMI_MULTI_SIM_SYS_SINGLE     
        case ID_SET_MULTI_SIM_SETTING:
            MMK_CreateWin((uint32 *)MMISET_MULTI_SIM_SETTING_WIN_TAB, PNULL);
            break;  
#endif
           
        case ID_SET_CALL_DIVERTS:
            MMIAPISET_OpenDivertSettingsWindow();
            break;
            
        case ID_SET_CALL_BAR:
            OpenBarSettingsWindow();
            break;
            
        case ID_SET_CALL_WAITING:
            MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLWATINGT_FORM_CTRL_ID);
            select_type = MMISET_CALL_WAITING;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if(sim_num == 1)
            {
                CreateCallWaitingtSelectPopWindow();
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;
#ifdef VT_SUPPORT
#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_VIDEOPHONE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_WIN_TAB,PNULL);        
            break;
#endif
#endif   
#ifdef MMI_CSP_SUPPORT
        case ID_SET_LINE_SELECT:
            MMIAPISET_SetLineInfo();
            break;

        case ID_SET_LINE_LOCK: //line lock
            //verify pin2 before open line lock setting win
            SetLineLock();
            break;            
#endif

	/* +qingfeng.dong */
        case ID_SET_NETWORK://ID_SET_NETWORK
            select_type = MMISET_SET_NETWORK;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMIAPISET_OpenSelNetWorkWin((MN_DUAL_SYS_E)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;

#ifdef MMI_DUALMODE_ENABLE
        case ID_SET_NETSETTING:
            MMIAPISET_OpenSelNetWorkWin(sim_sys);
            break;
#endif

        case ID_SET_FLY_MODE:
#ifdef MCARE_V31_SUPPORT
            if(MCareV31_Exit_Confirm())
            {
                return MMI_RESULT_FALSE;	///具体的return值不同，和添加出的函数相关
            }
#endif
            MMK_CreateWin((uint32*)MMISET_FLY_MODE_SET_WIN_TAB,PNULL);
            break;
	/* -qingfeng.dong */
			
        case ID_SET_HIDE_ID:
            select_type = MMISET_CALL_HIDE_ID;
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMK_CreatePubListWin((uint32*)MMISET_SET_CALL_HIDE_ID_WIN_TAB, (ADD_DATA)sim_sys);
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
            else
            {
                MMIAPISET_OpenUssdSelectWin(select_type);
            }
#endif
            break;
#ifdef MMI_IP_CALL_SUPPORT
#ifdef DPHONE_SUPPORT 
        case ID_SET_IP_CALL:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        select_type = MMISET_IP_CALL;
        
        //判断是否存在两张卡同时开启，不是则启动active sim
        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_num)
        {
            MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)sim_sys);
        }
        else
        {
            MMIAPISET_OpenUssdSelectWin(select_type);
        }
#else
        MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
#endif
        break;
#endif
        case ID_SET_MINUTE_REMINDER://分钟提示音
            MMK_CreateWin((uint32*)MMISET_SET_MINUTE_REMINDER_WIN_TAB,PNULL);
            break;

        case ID_SET_SPEED_DIAL://延时拨号
            MMK_CreateWin((uint32*)MMISET_SET_SPEED_DIAL_WIN_TAB,PNULL);
            break;

        case ID_SET_AUTO_REDIAL://自动重播
            MMK_CreateWin((uint32*)MMISET_SET_AUTO_REDIAL_WIN_TAB,PNULL);
            break;
            
        case ID_SET_ANYKEY_REJECT_MODE:
            MMK_CreateWin((uint32*)MMISET_SET_ANYKEY_REJECT_MODE_WIN_TAB,PNULL);
            break;
#else
        case ID_SET_OTHER:
            MMK_CreateWin((uint32*)MMISET_SET_CALL_OTHER_WIN_TAB, PNULL);
            break;
#endif 
            
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


#ifdef MULTI_SIM_GSM_CTA_SUPPORT
/*****************************************************************************/
//  Description : Open CTA MULTI SIM Standby Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenCTAMultiSysStandbyWin(BOOLEAN is_power_select)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (MMIAPIPHONE_GetSimExistedNum(PNULL, 0) > 1)
    {
        if (is_power_select
            || (MMIAPIPB_IsPbReady()
            && MMIAPISMS_IsOrderOk()))
        {
            MMK_CreatePubListWin((uint32*)MMISET_CTA_MULTI_SYS_STANDBY_WIN_TAB, (ADD_DATA)is_power_select);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        }
    }
    else
#endif
    {
        MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);

        if (is_power_select)
        {
            uint32 i = 0;
            MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIAPIPHONE_GetSimExistedStatus(i) && !(e_dualsys_setting & (0x01 << i)))
                {
                    ImplementCTASendSIMNotReadInd((MN_DUAL_SYS_E)i);
                }
            }

            // 打开正常开机窗口
            MMIAPIPHONE_OpenNormalStartUpWin();
        }
    }
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : to handle CTA Multi Sys Standby window msg
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCTAMultiSysStandbyWindow(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    uint32 i = 0;
    uint16 cur_selection = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl = MMISET_SET_CTA_MULTI_SYS_STANDBY_CTRL_ID;
    LOCAL BOOLEAN s_is_standby[MMI_DUAL_SYS_MAX] = {0};
    LOCAL MMISET_DUALSYS_TYPE_E old_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    uint32 sim_power_select = (uint32)MMK_GetWinAddDataPtr(win_id);
    BOOLEAN is_power_select = (BOOLEAN)sim_power_select;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        old_dualsys_setting = e_dualsys_setting;
        SCI_MEMSET(s_is_standby, 0x00, sizeof(s_is_standby));
        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (e_dualsys_setting & (0x01 << i))
            {
                s_is_standby[i] = TRUE;
            }
        }
        MMK_SetAtvCtrl(win_id, list_ctrl);
        break;

    case MSG_FULL_PAINT:
        AppendCTAMultiSysTypeList(list_ctrl);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            BOOLEAN is_has_selected = FALSE;

            //get cur selection
            cur_selection = GUILIST_GetCurItemIndex(list_ctrl);

            s_is_standby[cur_selection] = !s_is_standby[cur_selection];

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (s_is_standby[i])
                {
                    is_has_selected = TRUE;
                    break;
                }
            }

            if (!is_has_selected)
            {
                s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                MMIPUB_OpenAlertWarningWin( TXT_SET_STANDBY_MODE_ALERT);
            }
            else
            {
                if (!ImplementCTAStandbyChanged(s_is_standby, is_power_select))
                {
                    s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                }
            }
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if (is_power_select)
        {
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIAPIPHONE_GetSimExistedStatus(i) && !(e_dualsys_setting & (0x01 << i)))
                {
                    ImplementCTASendSIMNotReadInd((MN_DUAL_SYS_E)i);
                }
            }

            MAIN_UpdateIdleSignalState();
            MMIAPISET_SetIsQueryCfu(FALSE, MMI_DUAL_SYS_MAX);

            MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);

            // 打开正常开机窗口
            MMIAPIPHONE_OpenNormalStartUpWin();
        }
        else if (old_dualsys_setting != e_dualsys_setting)
        {
            MMI_STRING_T wait_text = {0};
            //进入等待界面
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                s_is_cta_deactive_ps[i] = TRUE;

                MMIAPIPHONE_DeactivePS((MN_DUAL_SYS_E)i);
            }

            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMISET_SET_CTA_MULTISYS_STANDBY_WAITING_WIN_ID,IMAGE_NULL,
                                            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleCTAStandbyWaitingWindow);
        }

        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : AppendCTAMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendCTAMultiSysTypeList(MMI_CTRL_ID_T ctrl_id)
{
    uint32 i = 0;
    uint16 cur_selection = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T status_image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T sim_image_id = IMAGE_NULL;
    MMISET_SIM_NAME_T sim_str = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={0};
    MMI_IMAGE_ID_T no_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMI_IMAGE_ID_T gray_sim_img_id[MMI_DUAL_SYS_MAX] = {0};

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        sim_img_id[i]      = IMAGE_IDLE_PROMPT_SIM1      + (i-MN_DUAL_SYS_1);    //shoule IMAGE_IDLE_PROMPT_NO_SIM2 == IMAGE_IDLE_PROMPT_NO_SIM1+1.atc
        no_sim_img_id[i]   = IMAGE_IDLE_PROMPT_NO_SIM1   + (i-MN_DUAL_SYS_1);
        gray_sim_img_id[i] = IMAGE_IDLE_PROMPT_GRAY_SIM1 + (i-MN_DUAL_SYS_1);
    }

    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE);
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        SCI_MEMSET(&sim_str, 0x00, sizeof(sim_str));
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

        if (e_dualsys_setting & (0x01 << i))
        {
            status_image_id = IMAGE_CHECK_SELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_CLOSE;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_OK;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                sim_image_id = sim_img_id[i];
            }
            else
            {
                sim_image_id = no_sim_img_id[i];
            }
        }
        else
        {
            status_image_id = IMAGE_CHECK_UNSELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_OPEN;
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)TXT_COMMON_OK;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                sim_image_id = gray_sim_img_id[i];
            }
            else
            {
                sim_image_id = no_sim_img_id[i];
            }
        }

        sim_str = MMIAPISET_GetSimName(i);
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = sim_image_id;
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = MIN(GUILIST_STRING_MAX_NUM, sim_str.wstr_len);

        item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)sim_str.wstr_arr;

        item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[2].item_data.image_id = status_image_id;

        GUILIST_AppendItem(ctrl_id, &item_t);
    }

    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
 }

/*****************************************************************************/
//  Description : ImplementCTAStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementCTAStandbyChanged(BOOLEAN * is_standby_ptr, BOOLEAN is_power_select)
{
    uint32 i = 0;
    BOOLEAN is_allow_set = FALSE;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMISET_DUALSYS_TYPE_E standby_mode = 0;

    //SCI_ASSERT(PNULL != is_standby_ptr);
   if(PNULL == is_standby_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert ImplementCTAStandbyChanged"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3086_112_2_18_2_50_22_66,(uint8*)"");
        return FALSE;
    } 
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (is_standby_ptr[i])
        {
            standby_mode |= (0x01 << i);
        }
    }

    if (e_dualsys_setting != standby_mode)
    {
        if (is_power_select
            ||(MMIAPIPB_IsPbReady()
            && MMIAPISMS_IsOrderOk()))
        {
            is_allow_set = TRUE;
        }

        if (!is_allow_set)
        {
            return FALSE;
        }

        //记录用户自己设置的情况
        MMISET_SetMultiSysSetting(standby_mode);

        MAIN_UpdateIdleSignalState();
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : ImplementCTASendSIMNotReadInd
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void ImplementCTASendSIMNotReadInd(MN_DUAL_SYS_E dual_sys)
{
    //send sim_not_ready
    APP_MN_SIM_NOT_READY_IND_T *ind_ptr = PNULL;
    
    SCI_CREATE_SIGNAL(
        ind_ptr,
        APP_MN_SIM_NOT_READY_IND,
        sizeof(APP_MN_SIM_NOT_READY_IND_T),
        P_MN);
    
   // SCI_ASSERT(ind_ptr != PNULL);
   if(PNULL == ind_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert ImplementCTASendSIMNotReadInd"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3138_112_2_18_2_50_22_67,(uint8*)"");
        return;
    }     
    ind_ptr->reason = MNSIM_ERROR_INSERTED_ERROR;
    ind_ptr->dual_sys = dual_sys;
    
    SCI_SEND_SIGNAL((xSignalHeaderRec*)ind_ptr, P_APP);
}

/*****************************************************************************/
//  Description : Standby 等待窗口
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCTAStandbyWaitingWindow (
                                           MMI_WIN_ID_T      win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_RED:
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
//  Description : Handle Implement CTA Standby Mode Async Timer
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleImplementCTAStandbyModeAsyncTimer(
                               uint8    timer_id,
                               uint32   param
                               )
{
    if (s_cta_deactive_ps_timer == timer_id)
    {
        uint32 i =0;
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (MMIAPIPHONE_GetSimExistedStatus(i) && !(e_dualsys_setting & (0x01 << i)))
            {
                ImplementCTASendSIMNotReadInd((MN_DUAL_SYS_E)i);
            }
        }

        s_cta_deactive_ps_timer = 0;

        MMIAPISMS_OrderInit();

        //MMIAPIPB_SetAllSIMNotInit();

        MAIN_UpdateIdleSignalState();
        MMIAPISET_SetIsQueryCfu(FALSE, MMI_DUAL_SYS_MAX);

        MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);

#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif

        MMK_CloseWin(MMISET_SET_CTA_MULTISYS_STANDBY_WAITING_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : Implement CTA Standby Mode
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ImplementCTAStandbyModeAsync(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    s_is_cta_deactive_ps[dual_sys] = FALSE;

    if (MMK_IsOpenWin(MMISET_SET_CTA_MULTISYS_STANDBY_WAITING_WIN_ID))
    {
        uint32 i = 0;
        BOOLEAN s_is_cta_deactive_finished = TRUE;

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (s_is_cta_deactive_ps[i])
            {
                s_is_cta_deactive_finished = FALSE;

                break;
            }
        }

        if (s_is_cta_deactive_finished)
        {
            if (0 != s_cta_deactive_ps_timer)
            {
                MMK_StopTimer(s_cta_deactive_ps_timer);
                s_cta_deactive_ps_timer = 0;
            }

            s_cta_deactive_ps_timer = MMK_CreateTimerCallback(3000, HandleImplementCTAStandbyModeAsyncTimer, NULL, FALSE);
        }
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : to open call settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenCallSettingsWindow(void)
{
     MMK_CreateWin((uint32*)MMISET_SET_CALL_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : to open Divert settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDivertSettingsWindow(void)
{
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
#ifdef VT_SUPPORT
    uint32 win_id  = MMISET_CALLDIVERT_FORM_CTRL_ID;
#endif

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }
    
    MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLDIVERT_FORM_CTRL_ID);
    select_type = MMISET_CALL_FORWARD;
#ifdef MMI_CSP_SUPPORT
    sim_num = GetCallFowardVisibleSimNum(&sim_sys, 1);
#else
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
#endif
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
#ifdef VT_SUPPORT
        MMK_CreateWin((uint32*)MMISET_SET_DIVERT_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);   
#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_WIN_TAB, (ADD_DATA)sim_sys);

#endif
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}

/*****************************************************************************/
//  Description : to open bar settings main menu window
//  Global resource dependence : none
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void OpenBarSettingsWindow(void)
{
    MMISET_SIM_SELECT_TYPE_E    select_type = 0;

    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
#ifdef VT_SUPPORT
    uint32 win_id = MMISET_CALLBAR_FORM_CTRL_ID;
#endif

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }
    MMISET_SetDivertBarWatingHideidCtrlId(MMISET_CALLBAR_FORM_CTRL_ID);
    select_type = MMISET_CALL_BAR;

#ifdef MMI_CSP_SUPPORT
    sim_num = GetCallFowardVisibleSimNum(&sim_sys, 1);
#else
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
#endif
    
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
#ifdef VT_SUPPORT
        MMK_CreateWin((uint32*)MMISET_SET_BAR_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);    
#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_BAR_WIN_TAB, (ADD_DATA)sim_sys);
#endif
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}


#ifdef VT_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitCallDivertList
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitVTCallSelectCallTypeList(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_CALL_MENU_EX_CTRL_ID;

    GUILIST_SetMaxItem(ctrl_id, 3, FALSE );

    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_CALL_AUDIO, IMAGE_NULL, ID_SET_DIVERTS_AUDIO);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_VIDEOCALL, IMAGE_NULL, ID_SET_DIVERTS_VIDEO);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_AUDIOANDVIDEO_CALL, IMAGE_NULL, ID_SET_DIVERTS_AUDIOANDVIDEO);
}
#endif
/*****************************************************************************/
//  Description : handle select call type window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSelectCallTypeGeneralWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    uint32              wind_id    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_CALL_MENU_EX_CTRL_ID;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
#if defined MMI_GUI_STYLE_TYPICAL
            if(wind_id == MMISET_CALLDIVERT_FORM_CTRL_ID)
            {
                GUIWIN_SetTitleTextId(MMISET_SET_SELECT_DIVERT_WIN_ID, TXT_SET_CALL_DIVERTS, TRUE);
            }
            else
            {
                GUIWIN_SetTitleTextId(MMISET_SET_SELECT_DIVERT_WIN_ID, TXT_SET_CALL_BAR, TRUE);
            }    
#else
            InitVTCallSelectCallTypeList();
#endif
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
      case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
            //GUIMENU_GetId(MMISET_SET_CALL_MENU_CTRL_ID,&group_id_3,&menu_id_3);
            if(wind_id == MMISET_CALLDIVERT_FORM_CTRL_ID)
            {
                MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_WIN_TAB,(ADD_DATA)ctrl_id);
            }
            else
            {
                MMK_CreateWin((uint32*)MMISET_SET_CALL_BAR_WIN_TAB,(ADD_DATA)ctrl_id);
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
//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

LOCAL void InitDivertVoiceNumcontrol(MMI_WIN_ID_T    win_id)
{
      MMI_STRING_T label_name  = {0};
      MMI_CTRL_ID_T             called_fuc_id   =  MMK_GetActiveCtrlId(MMISET_SET_CALL_DIVERT_WIN_ID);

      switch(called_fuc_id)
      {
        case MMISET_DIVERT_UNCONDITION_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_ALL_CALLS,FALSE);
            break;
        case MMISET_DIVERT_BUSY_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_WHEN_BUSY,FALSE);
            break;
        case MMISET_DIVERT_NOANSWER_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_NO_ANSWER,FALSE);
            break;
        case MMISET_DIVERT_UNRECHED_LABEL_CTRL_ID:
            GUIWIN_SetTitleTextId(win_id,TXT_SET_DIVERTS_NOT_REACHABLE,FALSE);
            break;
        default:
            break;
      }
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERT_VOICE, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_VOICE_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERT_NUMBER, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_NUM_LABEL_CTRL_ID, &label_name, FALSE);

    
}
/*****************************************************************************/
//  Description : set call diver voice or num title
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void SetCallDivertVoiceOrNumTitle(MMI_CTRL_ID_T ctrl_id)
{
    MMI_CTRL_ID_T      menu_ctrl_id = MMISET_VOICE_NUM_CTRL_ID;
    MMI_STRING_T     title_text_Str = {0};
    GUIMENU_TITLE_INFO_T title_info = {0};
    SET_ID_MENU_E           menu_id = ID_SET_DIVERTS_ALL_CALLS;
    MMI_TEXT_ID_T           text_id = TXT_NULL;
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T    group_id = 0;

    GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
#else
    uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
	// bug 171530
#ifdef MMI_CSP_SUPPORT
	MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
	if (!(MMISET_IsAlsValid(dual_sys) 
	    && IsSSMenuItemVisible(dual_sys, CSP_MENU_CFB, CSP_MENU_OPER_NULL)
		&& IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRY, CSP_MENU_OPER_NULL) 
		&& IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRC, CSP_MENU_OPER_NULL))
		&& index > 0)
	{
		index++;
	}
#endif
    menu_id = menu_id + index;
#endif
    title_info.is_static = TRUE;
    title_info.group_id = MENU_SET_SS_DIVERT_OPT; 
    switch (menu_id)
    {
    case ID_SET_DIVERTS_ALL_CALLS://?????
        text_id = TXT_SET_DIVERTS_ALL_CALLS;
        break;

#ifdef MMI_CSP_SUPPORT
    case ID_SET_DIVERTS_ALL_CONDITIONAL://????
        text_id = TXT_SET_DIVERTS_ALL_CONDITIONAL;
        break;
#endif
        
    case ID_SET_DIVERTS_WHEN_BUSY://?????
        text_id = TXT_SET_DIVERTS_WHEN_BUSY;
        break;
        
    case ID_SET_DIVERTS_NO_ANSWER://?????
        text_id = TXT_SET_DIVERTS_NO_ANSWER;
        break;
        
    case ID_SET_DIVERTS_NOT_REACHABLE://??????
        text_id = TXT_SET_DIVERTS_NOT_REACHABLE;
        break;
        
    default:
        //do nothing
        break;
    }
    MMI_GetLabelTextByLang(text_id, &title_text_Str);
    title_info.title_ptr = &title_text_Str;
    GUIMENU_SetMenuTitleEx(menu_ctrl_id, &title_info);

}
/*****************************************************************************/
//  Description : to do num divert
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOpenNumDivertWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    //MMI_CTRL_ID_T ctrl_id = MMISET_VOICE_NUM_CTRL_ID;
    MMI_MENU_GROUP_ID_T            group_id  =   0;
    MMI_MENU_ID_T                   menu_id  =   0;
    MMI_RESULT_E                     recode  =   MMI_RESULT_TRUE;
    //uint8                       record_id  =   0;
    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type  =   MN_MULTI_SUBSCRIBER_MAX;
    MN_VOICE_MAIL_TYPE_E    voice_mail_type  =   MN_VOICE_MAIL;
    //MMI_STRING_T                wait_text  =   {0};
    uint32                        dual_sys   =   s_cur_dual_sys;
    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //InitDivertVoiceNumcontrol(win_id);
        MMK_SetAtvCtrl(win_id, MMISET_VOICE_NUM_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        SetCallDivertVoiceOrNumTitle(MMISET_CALL_DIVERT_MENU_CTRL_ID);
#ifdef MMI_CSP_SUPPORT                    
        {
            BOOLEAN is_voicemail_visible = TRUE;
            BOOLEAN is_number_visible = TRUE;
            //MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T item_id = 0;
            CSP_MENU_SS_TYPE_E ss_type = CSP_MENU_NULL;
            GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID,&group_id,&item_id);
            switch (item_id)
            {
                case ID_SET_DIVERTS_ALL_CALLS:
                    ss_type = CSP_MENU_CFU;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;

#ifdef MMI_CSP_SUPPORT
                case ID_SET_DIVERTS_ALL_CONDITIONAL:
                    ss_type = CSP_MENU_CFB; //?cfb
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;                    
#endif

                case ID_SET_DIVERTS_WHEN_BUSY:
                    ss_type = CSP_MENU_CFB;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;
                    
                case ID_SET_DIVERTS_NO_ANSWER:
                    ss_type = CSP_MENU_CFNRY;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;
                    
                case ID_SET_DIVERTS_NOT_REACHABLE:
                    ss_type = CSP_MENU_CFNRC;
                    is_voicemail_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_VM);
                    is_number_visible = IsSSMenuItemVisible( dual_sys,  ss_type, CSP_MENU_OPER_ACTIVE_WITH_NO);
                    break;

                default:
                    //do nothing
                    break;
                    
            }
            GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_OPT, ID_SET_SS_DIVERT_VOICE, is_voicemail_visible);
            GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_OPT, ID_SET_SS_DIVERT_NUMBER, is_number_visible);            

            if (MMISET_IsAlsValid(dual_sys))
            {
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE_VMN, FALSE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE1_VMN, TRUE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE2_VMN, TRUE);
            }
            else
            {
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE_VMN, TRUE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE1_VMN, FALSE);
                GUIMENU_SetItemVisible(MMISET_VOICE_NUM_CTRL_ID, MENU_SET_SS_DIVERT_VOICE_OPT, ID_SET_SS_LINE2_VMN, FALSE);
            }
        }
#endif //MMI_CSP_SUPPORT           
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_VOICE_NUM_CTRL_ID, &group_id, &menu_id);       
        //record dual_sys type
        MMIAPISET_SetSysForCf(dual_sys);/*lint !e64*/
        
        switch (menu_id)
        {
        case ID_SET_SS_LINE_VMN:
            //record_id = 1;
			line_type = MN_MULTI_SUBSCRIBER_ID_1;
			voice_mail_type = MN_VOICE_MAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //?sim??????????
            GetVoiceMailAddress(dual_sys, line_type, g_vmn_oper_type);/*lint !e64*/
            break;
            
#ifdef MMI_CSP_SUPPORT        
        case ID_SET_SS_LINE1_VMN:
            //record_id = 1;
            line_type = MN_MULTI_SUBSCRIBER_ID_1;
			voice_mail_type = MN_VOICE_MAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type,  voice_mail_type);/*lint !e64*/
            break;
            
        case ID_SET_SS_LINE2_VMN:
            //record_id = 2;
            line_type = MN_MULTI_SUBSCRIBER_ID_2;
			voice_mail_type = MN_VOICE_MAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type,  voice_mail_type);/*lint !e64*/
            break;
#endif //   MMI_CSP_SUPPORT 
            
        case ID_SET_SS_FAX_VMN:
            //record_id = 3;
            line_type = MN_MULTI_SUBSCRIBER_ID_1;
			voice_mail_type = MN_VOICE_MAIL_FAX;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type, voice_mail_type);/*lint !e64*/
            break;
            
        case ID_SET_SS_DATA_VMN:
            //record_id = 4;
            line_type = MN_MULTI_SUBSCRIBER_ID_1;
			voice_mail_type = MN_VOICE_MAIL_EMAIL;
            g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_CALL_DIVERT;
            //从sim卡里读取语音信箱号码
            GetVoiceMailAddress(dual_sys, line_type,  voice_mail_type);/*lint !e64*/

            break;
            
		case ID_SET_SS_DIVERT_NUMBER:
            {
                OpenSetCallDivertWin(dual_sys);               
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
//  Description : get voice mail address
//  Global resource dependence : 
//  Author:bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetVoiceMailAddress(MN_DUAL_SYS_E dual_sys, MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type, MN_VOICE_MAIL_TYPE_E voice_mail_type)
{
	MMI_STRING_T wait_text = {0};
	ERR_MNSMS_CODE_E result = ERR_MNSMS_NONE;

    SCI_TRACE_LOW("GetVoiceMailAddress,dual_sys = %d, line_type= %d, voice_mail_type= %d", dual_sys, line_type, voice_mail_type);	
	
	result = MNSMS_GetVoiMessAddrEx(dual_sys, line_type, voice_mail_type);/*lint !e64*/
	
	//进入等待界面
	if (ERR_MNSMS_NONE == result)
	{
		MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
		MMIPUB_OpenWaitWin(1, &wait_text, PNULL, PNULL, MAIN_WAIT_WIN_ID, IMAGE_NULL,
                           ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleVMForDivertWaitingWindow);
	} 
}

/*****************************************************************************/
//  Description : create call divert pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallDivertSelectPopWindow(MMI_CTRL_ID_T called_func_id)
{
    MMK_CreatePubListWin((uint32 *)MMISET_SELECT_POP_DIVERT_WIN_TAB, (ADD_DATA)called_func_id);
}

/*****************************************************************************/
//  Description : create call bar pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallBarSelectPopWindow(MMI_CTRL_ID_T called_func_id)
{
    MMK_CreatePubListWin((uint32 *)MMISET_SELECT_POP_BAR_WIN_TAB,(ADD_DATA)MMISET_POPMENU_CALLSETTING_BAR_ID);
}
#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitCallDivertList
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitCallDivertList(void)
{
    MMI_CTRL_ID_T      ctrl_id = MMISET_CALL_DIVERT_MENU_CTRL_ID;
    uint16        max_item_num = 5;
    // bug 171530
#ifdef MMI_CSP_SUPPORT    
    MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
    BOOLEAN is_cfu_visible = TRUE;
    BOOLEAN is_cfc_visible = TRUE; //All conditional call forwarding
    BOOLEAN is_cfb_visible = TRUE;
    BOOLEAN is_cfnry_visible = TRUE;
    BOOLEAN is_cfnrc_visible = TRUE;
    BOOLEAN is_cancel_all_visible = TRUE;

    if (dual_sys < MMI_DUAL_SYS_MAX)
    {                
        if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFU, CSP_MENU_OPER_NULL))
        {
            is_cfu_visible = TRUE;
        }
        else
        {
            is_cfu_visible = FALSE;
        }

        if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFB, CSP_MENU_OPER_NULL))
        {
            is_cfb_visible = TRUE;
        }
        else
        {
            is_cfb_visible = FALSE;
        }

        if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRY, CSP_MENU_OPER_NULL))
        {
            is_cfnry_visible = TRUE;
        }
        else
        {
            is_cfnry_visible = FALSE;
        }

        if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRC, CSP_MENU_OPER_NULL))
        {
            is_cfnrc_visible = TRUE;
        }
        else
        {
            is_cfnrc_visible = FALSE;
        }

        if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CF, CSP_MENU_OPER_CANCEL_ALL))
        {
            is_cancel_all_visible = TRUE;
        }
        else
        {
            is_cancel_all_visible = FALSE;
        }

        //??????,??????,???????????,??????????
        if (MMISET_IsAlsValid(dual_sys) && is_cfb_visible && is_cfnry_visible && is_cfnrc_visible)     
        {
            is_cfc_visible = TRUE;
            is_cfb_visible = FALSE;
            is_cfnry_visible = FALSE;
            is_cfnrc_visible = FALSE;
            max_item_num = 3;
        }                
        else
        {
            is_cfc_visible = FALSE;
        }               
    }
#endif            
    
    GUILIST_SetMaxItem(ctrl_id, max_item_num, FALSE);

#ifdef MMI_CSP_SUPPORT
    if (is_cfu_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_ALL_CALLS, IMAGE_NULL, ID_SET_DIVERTS_ALL_CALLS);
    }
#ifdef MMI_CSP_SUPPORT
    if (is_cfc_visible)
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_ALL_CONDITIONAL, IMAGE_NULL, ID_SET_DIVERTS_ALL_CONDITIONAL); 
    }        
#endif 
#ifdef MMI_CSP_SUPPORT
    if (is_cfb_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_WHEN_BUSY, IMAGE_NULL, ID_SET_DIVERTS_WHEN_BUSY);
    } 
#ifdef MMI_CSP_SUPPORT
    if (is_cfnry_visible)
#endif 
    {
    	AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_NO_ANSWER, IMAGE_NULL, ID_SET_DIVERTS_NO_ANSWER);
    }
#ifdef MMI_CSP_SUPPORT
    if (is_cfnrc_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_NOT_REACHABLE, IMAGE_NULL, ID_SET_DIVERTS_NOT_REACHABLE);
    }
#ifdef MMI_CSP_SUPPORT
    if (is_cancel_all_visible)
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_DIVERTS_CANCEL_ALL, IMAGE_NULL, ID_SET_DIVERTS_CANCEL_ALL);
    }
}

/*****************************************************************************/
//  Description : InitCallBarList
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitCallBarList(void)
{
    MMI_CTRL_ID_T  ctrl_id = MMISET_CALL_BAR_MENU_CTRL_ID;
#ifdef MMI_CSP_SUPPORT    
    MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
#endif 

    GUILIST_SetMaxItem(ctrl_id, 7, FALSE);   
#ifdef MMI_CSP_SUPPORT
    if ((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BAOC, CSP_MENU_OPER_NULL))
#endif         
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_ALL_MO_CALLS, IMAGE_NULL, ID_SET_BAR_ALL_MO_CALLS);
    }
#ifdef MMI_CSP_SUPPORT
    if ((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BAIC, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_ALL_MT_CALLS, IMAGE_NULL, ID_SET_BAR_ALL_MT_CALLS);
    }
#ifdef MMI_CSP_SUPPORT
    if ((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BICROAM, CSP_MENU_OPER_NULL))
#endif 
    {
    	AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_MT_CALLS_ROAM, IMAGE_NULL, ID_SET_BAR_MT_CALLS_ROAM);
    }
#ifdef MMI_CSP_SUPPORT
    if ((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_MO_CALLS_INTL, IMAGE_NULL, ID_SET_BAR_MO_CALLS_INTL);
    }
#ifdef MMI_CSP_SUPPORT
    if ((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC_EXHC, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_MO_CALLS_INTL_ROAM, IMAGE_NULL, ID_SET_BAR_MO_CALLS_INTL_ROAM);
    }
#ifdef MMI_CSP_SUPPORT
    if ((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_CANCEL_ALL, IMAGE_NULL, ID_SET_BAR_CANCEL_ALL);
    }
#ifdef MMI_CSP_SUPPORT
    if ((dual_sys < MMI_DUAL_SYS_MAX) && IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_NULL))
#endif 
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_BAR_CHANGE_PW, IMAGE_NULL, ID_SET_BAR_CHANGE_PW);
    }

}
#endif
/*****************************************************************************/
//  Description : handle call divert window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallDivertWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E                recode      =  MMI_RESULT_TRUE;
#if defined MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T         group_id    =  0;
#endif
    MMI_MENU_ID_T               menu_id     =  0;
    MMI_CTRL_ID_T               ctrl_id     =  MMISET_CALL_DIVERT_MENU_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
#if !defined MMI_GUI_STYLE_TYPICAL
            InitCallDivertList();
#endif
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id, STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
#if defined MMI_GUI_STYLE_TYPICAL           
#ifdef MMI_CSP_SUPPORT
            if (s_cur_dual_sys < MMI_DUAL_SYS_MAX)
            {
                MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
                BOOLEAN is_cfu_visible = TRUE;
                BOOLEAN is_cfc_visible = TRUE; //All conditional call forwarding
                BOOLEAN is_cfb_visible = TRUE;
                BOOLEAN is_cfnry_visible = TRUE;
                BOOLEAN is_cfnrc_visible = TRUE;
                BOOLEAN is_cancel_all_visible = TRUE;
                uint32 group = MENU_SET_CALL_DIVERTS_OPT;
                
                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFU, CSP_MENU_OPER_NULL))
                {
                    is_cfu_visible = TRUE;
                }
                else
                {
                    is_cfu_visible = FALSE;
                }

                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFB, CSP_MENU_OPER_NULL))
                {
                    is_cfb_visible = TRUE;
                }
                else
                {
                    is_cfb_visible = FALSE;
                }

                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRY, CSP_MENU_OPER_NULL))
                {
                    is_cfnry_visible = TRUE;
                }
                else
                {
                    is_cfnry_visible = FALSE;
                }

                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CFNRC, CSP_MENU_OPER_NULL))
                {
                    is_cfnrc_visible = TRUE;
                }
                else
                {
                    is_cfnrc_visible = FALSE;
                }

                if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CF, CSP_MENU_OPER_CANCEL_ALL))
                {
                    is_cancel_all_visible = TRUE;
                }
                else
                {
                    is_cancel_all_visible = FALSE;
                }

                //??????,??????,???????????,??????????
                if (MMISET_IsAlsValid(dual_sys) && is_cfb_visible && is_cfnry_visible && is_cfnrc_visible)     
                {
                    is_cfc_visible = TRUE;
                    is_cfb_visible = FALSE;
                    is_cfnry_visible = FALSE;
                    is_cfnrc_visible = FALSE;
                }                
                else
                {
                    is_cfc_visible = FALSE;
                }

                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_ALL_CALLS,  is_cfu_visible); 
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_ALL_CONDITIONAL,  is_cfc_visible);                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_WHEN_BUSY,  is_cfb_visible);                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_NO_ANSWER,  is_cfnry_visible);                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_NOT_REACHABLE,  is_cfnrc_visible);                                    
                GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_DIVERTS_CANCEL_ALL,  is_cancel_all_visible);
              
            }
#endif
#endif
            break;
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
#if !defined MMI_GUI_STYLE_TYPICAL
            GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &menu_id);
#else
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
#endif
            if (menu_id == ID_SET_DIVERTS_CANCEL_ALL)
            {
                MMISET_CloseAllCallForwarding(s_cur_dual_sys);
            }
            else
            {
                CreateCallDivertSelectPopWindow(ctrl_id);
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
//  Description : handle call bar window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallBarWindow(
                                          MMI_WIN_ID_T    win_id, 
                                          MMI_MESSAGE_ID_E   msg_id, 
                                          DPARAM             param
                                          )
{
    MMI_RESULT_E          recode = MMI_RESULT_TRUE;
#if defined MMI_GUI_STYLE_TYPICAL
    MMI_MENU_GROUP_ID_T group_id = 0;
#endif
    MMI_MENU_ID_T        menu_id = 0;
    MMI_CTRL_ID_T        ctrl_id = MMISET_CALL_BAR_MENU_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if !defined MMI_GUI_STYLE_TYPICAL
        InitCallBarList();
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_SELECT, TXT_NULL, STXT_RETURN, FALSE);
#if defined MMI_GUI_STYLE_TYPICAL
#ifdef MMI_CSP_SUPPORT
        if (s_cur_dual_sys < MMI_DUAL_SYS_MAX)
        {
            MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
            BOOLEAN is_visible = TRUE;
            uint32 group = MENU_SET_CALL_BAR_OPT;
            
            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BAOC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_ALL_MO_CALLS, is_visible);

            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BAIC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_ALL_MT_CALLS, is_visible);

            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BICROAM, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_MT_CALLS_ROAM, is_visible);

            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_MO_CALLS_INTL, is_visible);

            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_BOIC_EXHC, CSP_MENU_OPER_NULL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_MO_CALLS_INTL_ROAM, is_visible);

            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_CANCEL_ALL))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_CANCEL_ALL, is_visible);

            if (IsSSMenuItemVisible(dual_sys, CSP_MENU_CB, CSP_MENU_OPER_MODIFY_PASSWORD))
            {
                is_visible = TRUE;
            }
            else
            {
                is_visible = FALSE;
            }
            GUIMENU_SetItemVisible(ctrl_id, group, ID_SET_BAR_CHANGE_PW, is_visible);
            
        }
#endif  
#endif
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &menu_id);
#else
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
#endif
        if (menu_id == ID_SET_BAR_CHANGE_PW)
        {
            if (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) > 0)
            {
                s_call_bar_operation = MMISET_CALL_BAR_MODIFY_PWD;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
        }
        else if (menu_id == ID_SET_BAR_CANCEL_ALL)
        {
            
            if (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) > 0)
            {
                s_call_bar_operation = MMISET_CALL_BAR_CANCEL_ALL;
                MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
        }
        else
        {
            CreateCallBarSelectPopWindow(ctrl_id);
            
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
//  Description : set call divert/bar/wating/hide id ctrl id
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetDivertBarWatingHideidCtrlId(MMI_CTRL_ID_T ctri_id)
{
    //SCI_TRACE_LOW:"MMISET_SetDivertBarWatingHideidCtrlId ctrl_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3766_112_2_18_2_50_23_68,(uint8*)"d", ctri_id);
    
    s_set_divert_bar_wating_hideid_ctrl_id = ctri_id;
}

/*****************************************************************************/
//  Description : Get call divert/bar/wating/hide id ctrl id
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMISET_GetDivertBarWatingHideidCtrlId(void)
{
    //SCI_TRACE_LOW:"MMISET_GetDivertBarWatingHideidCtrlId ctrl_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_3777_112_2_18_2_50_23_69,(uint8*)"d", s_set_divert_bar_wating_hideid_ctrl_id);
    
    return s_set_divert_bar_wating_hideid_ctrl_id;
}

/*****************************************************************************/
//  Description : handle call bar pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPopBarWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id = MMISET_POPMENU_CALLSETTING_BAR_ID;
    uint16             sim_num = 0;
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL,0);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
        {
            BOOLEAN is_open_visible  = TRUE;
            BOOLEAN is_close_visible = TRUE;
            BOOLEAN is_query_visible = TRUE;
#ifdef MMI_CSP_SUPPORT            
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T item_id = 0;
            MN_DUAL_SYS_E dual_sys = s_cur_dual_sys;
            CSP_MENU_SS_TYPE_E ss_type = CSP_MENU_NULL;
            
            GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID, &group_id, &item_id);
            switch (item_id)
            {
                case ID_SET_DIVERTS_ALL_CALLS:
                    ss_type = CSP_MENU_CFU;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;

                case ID_SET_DIVERTS_ALL_CONDITIONAL:
                    ss_type = CSP_MENU_CFB;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;                    
                    
                case ID_SET_DIVERTS_WHEN_BUSY:
                    ss_type = CSP_MENU_CFB;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;
                    
                case ID_SET_DIVERTS_NO_ANSWER:
                    ss_type = CSP_MENU_CFNRY;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;
                    
                case ID_SET_DIVERTS_NOT_REACHABLE:
                    ss_type = CSP_MENU_CFNRC;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;

                default:
                    //do nothing
                    break;
                    
            }

#endif //MMI_CSP_SUPPORT   
            if (is_open_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_OPEN, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if (is_close_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if (is_query_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_SET_QUERY_STATUS, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
        }
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16  current_index = 0;
            uint32  menu_id = ID_SET_CALL_BAR_OPEN;

            current_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
            menu_id += current_index;

            if (sim_num > 0)
            {
                switch (menu_id)
                {               
                case ID_SET_CALL_BAR_OPEN:
                    s_call_bar_operation = MMISET_CALL_BAR_OPEN;
                    MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
                    MMK_CloseWin(win_id);
                    break;
                    
                case ID_SET_CALL_BAR_CLOSE:
                    s_call_bar_operation = MMISET_CALL_BAR_CLOSE;
                    MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_PASSWORD_WIN_TAB, (ADD_DATA)s_cur_dual_sys);
                    MMK_CloseWin(win_id);
                    break;
                    
                case ID_SET_CALL_BAR_STATUS:
                    MMISET_InterrogateStatus(s_cur_dual_sys);//for cr226576 maryxiao
                    MMK_CloseWin(win_id);
                    break;
                    
                default:
                    break;
                }
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
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

/*****************************************************************************/
//  Description : init divert waiting barring call control
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/

LOCAL void InitCallDivertControl(void)
{
    MMI_STRING_T label_name  = {0};
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_ALL_CALLS, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_UNCONDITION_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_WHEN_BUSY, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_BUSY_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NO_ANSWER, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_NOANSWER_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NOT_REACHABLE, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_UNRECHED_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_DIVERTS_CANCEL_ALL, &label_name);
    GUILABEL_SetText(MMISET_DIVERT_CANCELALL_LABEL_CTRL_ID, &label_name, FALSE);
}

#if 0
/*****************************************************************************/
//  Description : InitCallWaitingControl
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InitCallWaitingControl(void)
{
    
    MMI_STRING_T label_name  = {0};
    
    MMI_GetLabelTextByLang(TXT_OPEN, &label_name);
    GUILABEL_SetText(MMISET_WAITING_ACTIVE_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
    GUILABEL_SetText(MMISET_WAITING_QUERY_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_QUERY_STATUS, &label_name);
    GUILABEL_SetText(MMISET_WAITING_QUERY_LABEL_CTRL_ID, &label_name, FALSE);
}

/*****************************************************************************/
//  Description : InitCallBarControl
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InitCallBarControl(void)
{
    MMI_STRING_T label_name  = {0};
    
    MMI_GetLabelTextByLang(TXT_BAOC, &label_name);
    GUILABEL_SetText(MMISET_BAR_ALLOUTGOING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_BAIC, &label_name);
    GUILABEL_SetText(MMISET_BAR_ALLINCOMING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_BICROAM, &label_name);
    GUILABEL_SetText(MMISET_BAR_ROAMINGINCOMING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_BOIC, &label_name);
    GUILABEL_SetText(MMISET_BAR_INTERNATIONALCALLING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_BAR_MO_CALLS_INTL_ROAM, &label_name);
    GUILABEL_SetText(MMISET_BAR_INTLROAMING_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_BAR_CANCEL_ALL, &label_name);
    GUILABEL_SetText(MMISET_BAR_CANCELALL_LABEL_CTRL_ID, &label_name, FALSE);
    
    MMI_GetLabelTextByLang(TXT_SET_BAR_CHANGE_PW, &label_name);
    GUILABEL_SetText(MMISET_BAR_CHANGEPASSWORD_LABEL_CTRL_ID, &label_name, FALSE);
}
#endif

/*****************************************************************************/
//  Description : open call divert pop window
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void OpenSetCallDivertWin(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_MULTI_SIM_SYS_DUAL          /* lint_lai */   
    MMIPB_CONTACT_T     local_num                 =  {0};
    uint8      sim_num[MMIPB_MAX_STRING_LEN + 2]  =  {0};
    wchar sim_num_wstr[MMIPB_MAX_STRING_LEN + 2]  =  {0};
    uint8                                num_len  =  {0};
    BOOLEAN                         is_pb_msisdn  =  FALSE;           
    MN_DUAL_SYS_E                   another_sys   =  MN_DUAL_SYS_MAX;
        
    another_sys = (dual_sys == MN_DUAL_SYS_1) ? MN_DUAL_SYS_2 : MN_DUAL_SYS_1;
    is_pb_msisdn = MMIAPIPB_ReadMsisdnContact(another_sys, &local_num);
#endif             
    //record dual_sys type
    MMK_CreatePubEditWin((uint32*)MMISET_SET_DIVERT_NUMBER_WIN_TAB,(ADD_DATA)dual_sys);
            
#ifdef MMI_MULTI_SIM_SYS_DUAL  
    if (is_pb_msisdn && s_is_wizard_cfnrc)
    {
        num_len = MMIAPIPB_BCDNumberToString(&(local_num.number[0]), (uint8)MMIPB_MAX_STRING_LEN + 2, sim_num);
//      num_len = MMIAPICOM_GenDispNumber(
//                                        MMIPB_GetNumberTypeFromUint8(local_num.number_t[0].npi_ton), 
//                                        local_num.number_t[0].number_len, 
//                                        local_num.number_t[0].number,
//                                        sim_num,
//                                        MMIPB_MAX_STRING_LEN + 2
//                                        );

        num_len = MIN(num_len, MMISET_VOCIE_MAX_NUM);                    
        MMI_STRNTOWSTR(sim_num_wstr, MMISET_VOCIE_MAX_NUM, sim_num, num_len, num_len);
        GUIEDIT_SetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID, sim_num_wstr, num_len);
    }
#endif                
}


/*****************************************************************************/
//  Description : handle call divert pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectPopDivertWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id = MMISET_POPMENU_CALLSETTING_DIVERT_ID;
    uint16             sim_num = 0;
    //   MMI_CTRL_ID_T ctrl_id = (MMI_CTRL_ID_T) (uint32)MMK_GetWinAddDataPtr(win_id);
    sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL,0);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
        {
            BOOLEAN      is_open_visible = TRUE;
            BOOLEAN     is_close_visible = TRUE;
            BOOLEAN     is_query_visible = TRUE;
#ifdef MMI_CSP_SUPPORT            
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T        item_id = 0;
            MN_DUAL_SYS_E       dual_sys = s_cur_dual_sys;
            CSP_MENU_SS_TYPE_E   ss_type = CSP_MENU_NULL;
            
            GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID, &group_id, &item_id);
            switch (item_id)
            {
                case ID_SET_DIVERTS_ALL_CALLS:
                    ss_type = CSP_MENU_CFU;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;

                case ID_SET_DIVERTS_ALL_CONDITIONAL:
                    ss_type = CSP_MENU_CFB;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;                    
                    
                case ID_SET_DIVERTS_WHEN_BUSY:
                    ss_type = CSP_MENU_CFB;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;
                    
                case ID_SET_DIVERTS_NO_ANSWER:
                    ss_type = CSP_MENU_CFNRY;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;
                    
                case ID_SET_DIVERTS_NOT_REACHABLE:
                    ss_type = CSP_MENU_CFNRC;
                    is_open_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_ACTIVE);
                    is_close_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_CANCEL);
                    is_query_visible = IsSSMenuItemVisible(dual_sys, ss_type, CSP_MENU_OPER_INTEROGATE);
                    break;

                default:
                    //do nothing
                    break;
            }

#endif //MMI_CSP_SUPPORT   
            if (is_open_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_OPEN, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if (is_close_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
            if (is_query_visible)/*lint !e774*/
            {
                MMIAPISET_AppendListItemByTextId(TXT_SET_QUERY_STATUS, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
            }
        }        
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16  current_index = 0;
            uint32  menu_id  = ID_SET_CALL_DIVERT_OPEN;

            current_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
            menu_id += current_index;

            if (sim_num > 0)
            {
                switch (menu_id)
                {
                    case ID_SET_CALL_DIVERT_OPEN:    
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT 
                        MMK_CreateWin((uint32*)MMISET_SET_OPEN_DIVERT_WIN_TAB, PNULL);        
#else   
                        MMIAPISET_SetSysForCf(s_cur_dual_sys);/*lint !e64*/
                        OpenSetCallDivertWin(s_cur_dual_sys);
#endif
                        MMK_CloseWin(win_id);
                        break;                      
                    case ID_SET_CALL_DIVERT_CLOSE:
                        MMISET_CloseCallForwarding(s_cur_dual_sys);/*lint !e64*/
                        MMK_CloseWin(win_id);
                        break;
                        
                    case ID_SET_CALL_DIVERT_STATUS:
                        MMISET_InterrogateStatus(s_cur_dual_sys);//for cr226576
                        MMK_CloseWin(win_id);
                        break;
                        
                    default:
                        break;
                }
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
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

/*****************************************************************************/
//  Description : handle call waiting pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectPopWaitingWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id = MMISET_POPMENU_CALLSETTING_WAITING_ID;
    uint16             sim_num = 0;
    sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL, 0);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(menu_ctrl_id, MMISET_CALLSET_ITEM_NUM_MAX, FALSE);
        MMIAPISET_AppendListItemByTextId(TXT_OPEN, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
        MMIAPISET_AppendListItemByTextId(STXT_CANCEL, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
        MMIAPISET_AppendListItemByTextId(TXT_SET_QUERY_STATUS, STXT_OK, menu_ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16  current_index = 0;
            uint32  menu_id = ID_SET_CALL_WAITING_OPEN;

            current_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
            menu_id += current_index;

            if (sim_num > 0)
            {
                switch (menu_id)
                { 
                case ID_SET_CALL_WAITING_OPEN:
                    MMISET_StartCallWaitOrBar(s_cur_dual_sys);/*lint !e64*/
                    MMK_CloseWin(win_id);
                    break;
                    
                case ID_SET_CALL_WAITING_CLOSE:
                    MMISET_CloseCallWaitOrBar(s_cur_dual_sys);
                    MMK_CloseWin(win_id);
                    break;
                    
                case ID_SET_CALL_WAITING_STATUS:   
                    MMISET_InterrogateStatus(s_cur_dual_sys);//for cr226576
                    MMK_CloseWin(win_id);
                    break;
                    
                default:
                    break;
                }
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
                MMK_CloseWin(win_id);
            }
            
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMK_CloseWin(MMISET_COM_SELECT_SIM_WIN_ID);
#endif
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


#if 0
/*****************************************************************************/
//  Description : InsertOptMenuNodeByLabel
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InsertOptMenuNodeByLabel(
                                    MMI_CTRL_ID_T      ctrl_id,        //控件id
                                    MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                                    uint16             node_id,        //节点编号
                                    uint16             parent_node_id, //父节点编号
                                    uint16             index           //位置
                                    )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  
#endif
/*****************************************************************************/
//  Description : create call waiting pop window
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallWaitingtSelectPopWindow(void)
{
    MMK_CreatePubListWin((uint32 *)MMISET_SELECT_POP_WAITING_WIN_TAB, PNULL);
}

/*****************************************************************************/
// Description : to handle set call hide id window
// Global resource dependence : 
// Author:wancan.you
// Note: MMISET_SET_CALL_HIDE_ID_WIN_TAB
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallHideIdWindow(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id,
                                              DPARAM            param
                                              )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    MN_CALL_CLIR_E      clir_type       =   MN_CALL_CLIR_NETWORK_DECIDE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_CALL_HIDE_ID_MENU_CTRL_ID;
    uint16              cur_selection   =   0;    
    uint32              dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_CALL_CLIR_MAX, FALSE);
        MMIAPISET_AppendListItemByTextId(TXT_SET_HIDEID,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_DISPLAY_ID,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_DISPLAY_ID_BY_NETWORK,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        clir_type = MMIAPISET_GetClirType(dual_sys);/*lint !e64*/
        
        switch (clir_type)
        {
        case MN_CALL_CLIR_SUPPRESSION:
            cur_selection = MMISET_CALL_CLIR_SUPPRESSION_E;
            break;
            
        case MN_CALL_CLIR_INVOCATION:
            cur_selection = MMISET_CALL_CLIR_INVOCATION_E;
            break;
            
        case MN_CALL_CLIR_NETWORK_DECIDE:
            cur_selection = MMISET_CALL_CLIR_NETWORK_DECIDE_E;
            break;
            
        default:
            cur_selection = MMISET_CALL_CLIR_NETWORK_DECIDE_E;
            break;
        }
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        
        switch (cur_selection)
        {
        case MMISET_CALL_CLIR_SUPPRESSION_E:
            clir_type = MN_CALL_CLIR_SUPPRESSION;
            break;
            
        case MMISET_CALL_CLIR_INVOCATION_E:
            clir_type = MN_CALL_CLIR_INVOCATION;
            break;
            
        case MMISET_CALL_CLIR_NETWORK_DECIDE_E:
            clir_type = MN_CALL_CLIR_NETWORK_DECIDE;
            break;
            
        default:
            clir_type = MN_CALL_CLIR_NETWORK_DECIDE;
            break;
        }
        
        MMISET_SetClirType(dual_sys, clir_type);/*lint !e64*/
        
        //success 
        //MMIPUB_OpenAlertSuccessWin(TXT_SET_SETTING_ACTIVATED);
        MMK_CloseWin(win_id);
        break;
        
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_APP_CANCEL:
#endif
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
//  Description : to handle divert to voice box
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertVoiceWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    MMI_STRING_T        voice_number    =   {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BG_T        edit_bg = {0};
    GUI_BORDER_T    edit_border = {0};
    
    SCI_MEMSET(&voice_number, 0, sizeof(MMI_STRING_T));
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
        GUIAPICTRL_SetBothRect(MMISET_SET_DIVERT_VOICE_CTRL_ID, &edit_both_rect);

        //set edit single
        GUIEDIT_SetStyle(MMISET_SET_DIVERT_VOICE_CTRL_ID,GUIEDIT_STYLE_SINGLE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(MMISET_SET_DIVERT_VOICE_CTRL_ID,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(MMISET_SET_DIVERT_VOICE_CTRL_ID,&edit_border);

        //set font
        //GUIEDIT_SetFontColor(MMISET_SET_DIVERT_VOICE_CTRL_ID,MMI_BLACK_COLOR);

        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_DIVERT_VOICE_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_DIVERT_VOICE_CTRL_ID);
        break;
        
 #ifndef MMIPUBWIN_PDASTYLE_SUPPORT 
    case MSG_FULL_PAINT:
         MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_DIVERT);
        break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get voice box number
        GUIEDIT_GetString(MMISET_SET_DIVERT_VOICE_CTRL_ID,&voice_number);
        if (0 != voice_number.wstr_len)
        {
            if (MMIAPICOM_IsValidNumberStringEx(&voice_number))
            {
                uint32 i = 0;
                BOOLEAN is_valid = TRUE;

                for (i = 0; i < voice_number.wstr_len; i++)
                {
                    if ('*' == voice_number.wstr_ptr[i]
                        || '#' == voice_number.wstr_ptr[i]
                        || 'p' == voice_number.wstr_ptr[i]
                        || 'w' == voice_number.wstr_ptr[i]
                        || 'P' == voice_number.wstr_ptr[i]
                        || 'W' == voice_number.wstr_ptr[i])
                    {
                        is_valid = FALSE;

                        break;
                    }
                }

                if (is_valid)
                {
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
                    MMI_SMS_VMN_T  cur_vm_info = {0};
                    cur_vm_info = MMISMS_GetCurVmInfo();
#ifndef MMI_GUI_STYLE_TYPICAL
                    SaveVMAddr2SimByType(&cur_vm_info,MMIAPISET_GetSysForCf(),&voice_number,MMIAPISET_GetVMRecordId()); 
#endif
#endif
                    MMISET_CallForwarding(MMIAPISET_GetSysForCf(),&voice_number); 
			    }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
        }
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


/*****************************************************************************/
//  Description : to handle divert to phone number
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDivertNumberWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    uint8                   num_len    =   0,tele_num[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
    MMI_RESULT_E             recode    =   MMI_RESULT_TRUE;
    MMI_STRING_T       voice_number    =   {0};
    MMIPB_BCD_NUMBER_T    bcd_number   =   {0};
    uint32                  dual_sys   =   (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_BOTH_RECT_T   edit_both_rect   =   {0};
    GUI_BG_T                 edit_bg   =   {0};
    GUI_BORDER_T         edit_border   =   {0};

    SCI_MEMSET(&voice_number, 0, sizeof(MMI_STRING_T));
    SCI_MEMSET(&bcd_number, 0, sizeof(MMIPB_BCD_NUMBER_T));
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id);
        GUIAPICTRL_SetBothRect(MMISET_SET_DIVERT_NUMBER_CTRL_ID, &edit_both_rect);

        //set edit single
        GUIEDIT_SetStyle(MMISET_SET_DIVERT_NUMBER_CTRL_ID,GUIEDIT_STYLE_SINGLE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(MMISET_SET_DIVERT_NUMBER_CTRL_ID,&edit_bg);

        //+CR226335
        #ifdef MONOCHROME_LCD_SUPPORT 
        #else
        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(MMISET_SET_DIVERT_NUMBER_CTRL_ID,&edit_border);
        #endif
        //-CR226335
        
        //set font
        //GUIEDIT_SetFontColor(MMISET_SET_DIVERT_NUMBER_CTRL_ID,MMI_BLACK_COLOR);

        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_DIVERT_NUMBER_CTRL_ID, 0, 1, STXT_MAIN_PB, TXT_COMMON_OK, PNULL);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_DIVERT_NUMBER_CTRL_ID);
        break;
 
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_DIVERT);
        break;
#endif

#if 0
//#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            GUIEDIT_GetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID, &voice_number);	
        
            if (0 == voice_number.wstr_len)
            {    
                MMIPUB_SetWinSoftkey(win_id, STXT_MAIN_PB, STXT_RETURN, TRUE);
            }
            else
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, TXT_DELETE, TRUE);
            }
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get voice box number
        GUIEDIT_GetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID, &voice_number);
        if (0 == voice_number.wstr_len)//enter phone booke
        {
            //调用phonebooke接口提供的单选列表
            // MMIAPIPB_OpenNumberList(MMIPB_LIST_FOR_SMS,MMIPB_LIST_RADIO,1,PNULL);
            MMIAPIPB_OpenMainWin(
                                 MMIPB_ALL_CONTACT,//
                                 MMIPB_LIST_RADIO, //IN: 号码列表的类型
                                 MMIPB_LIST_FIELD_NUMBER,
                                 1,//如果是多选时，此项限制了多选的最大个数
                                 PNULL,
                                 SelectContactCallback
                                 );
        }
        else if (MMIAPICOM_IsValidNumberStringEx(&voice_number))//enter call forwarding
        {
            uint32 i = 0;
            BOOLEAN is_valid = TRUE;

            for (i = 0; i < voice_number.wstr_len; i++)
            {
                if ('*' == voice_number.wstr_ptr[i]
                    || '#' == voice_number.wstr_ptr[i]
                    || 'p' == voice_number.wstr_ptr[i]
                    || 'w' == voice_number.wstr_ptr[i]
                    || 'P' == voice_number.wstr_ptr[i]
                    || 'W' == voice_number.wstr_ptr[i])
                {
                    is_valid = FALSE;

                    break;
                }
            }

            if (is_valid)
            {
                MMISET_CallForwarding(dual_sys, &voice_number);/*lint !e64*/
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        }
        break;
        
    case MMI_SELECT_CONTACT_CNF:
        //get select phone number
        if (0 < MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param))
        {
            wchar tele_num_wstr[MMISET_PHONE_NUM_MAX_LEN + 2] = {0};
            
            MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &bcd_number,MMIPB_BCD_NUMBER_MAX_LEN,  0);       
            //MMIAPIPB_GetDetailInSelectedList(PNULL,&bcd_number,PNULL,0,PNULL,PNULL);
            //generate display number
            num_len = MMIAPIPB_BCDNumberToString(&bcd_number, MMISET_PHONE_NUM_MAX_LEN+2, tele_num);
//             num_type = MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton);
//             num_len = MMIAPICOM_GenDispNumber(num_type,
//                 bcd_number.number_len,
//                 bcd_number.number, 
//                 tele_num,
//                 MMISET_PHONE_NUM_MAX_LEN + 2);

            num_len = MIN(num_len, MMISET_PHONE_NUM_MAX_LEN);
            
            MMI_STRNTOWSTR( tele_num_wstr, MMISET_PHONE_NUM_MAX_LEN, tele_num, num_len, num_len );
            
            //set phone number editbox
            GUIEDIT_SetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID, tele_num_wstr, num_len);
        }
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


/*****************************************************************************/
//  Description :open voice message divert call number window
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDivertCallWin(uint8   mail_len,
                                        uint8*  voice_mail_ptr
                                        )
{       
    if (0 != mail_len)//是否有语音信箱号码
    {
        wchar mail_wstr[ MMISMS_PBNUM_MAX_LEN + 1 ] = {0};

        mail_len = MIN(mail_len, MMISMS_PBNUM_MAX_LEN);
        
        MMI_STRNTOWSTR( mail_wstr, MMISMS_PBNUM_MAX_LEN, voice_mail_ptr, mail_len, mail_len );
        
        MMK_CreatePubEditWin((uint32*)MMISET_SET_DIVERT_VOICE_WIN_TAB,PNULL);
        
        // set the edit box init param
        GUIEDIT_SetString(
            MMISET_SET_DIVERT_VOICE_CTRL_ID,
            mail_wstr,
            mail_len);
        //SCI_TRACE_LOW:"HandleGetVoiMessAddrCnf:enter divert voice box!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_4491_112_2_18_2_50_25_70,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW("HandleSetMainMenuWindow:enter divert voice box and no number!");
        MMIPUB_OpenAlertWarningWin(TXT_SMS_BOX_EMPTY);
    }
}



/*****************************************************************************/
//  Description : to handle input call bar password
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBarPasswordWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMI_STRING_T   old_password_value = {0};
    uint16 sim_num = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id);
#endif              
        GUIAPICTRL_SetBothRect(MMISET_SET_BAR_PASSWORD_CTRL_ID, &edit_both_rect);
        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_BAR_PASSWORD_CTRL_ID,0,MMISET_BAR_PASSWORD_MAX_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMISET_SET_BAR_PASSWORD_CTRL_ID);
        break;
        
 #ifndef MMIPUBWIN_PDASTYLE_SUPPORT 
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        // get the value about old password
        GUIEDIT_GetString(MMISET_SET_BAR_PASSWORD_CTRL_ID,&old_password_value);

        SCI_MEMSET(g_mmiset_ss_pwd.old_pwd, 0x00, sizeof(g_mmiset_ss_pwd.old_pwd));

        MMI_WSTRNTOSTR((uint8 *)g_mmiset_ss_pwd.old_pwd,
                       MMISET_BAR_PASSWORD_MAX_LEN,
                       old_password_value.wstr_ptr,
                       old_password_value.wstr_len,
                       old_password_value.wstr_len);

        //SCI_TRACE_LOW:"HandleBarPasswordWindow s_call_bar_operation=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_4556_112_2_18_2_50_25_71,(uint8*)"d", s_call_bar_operation);

        sim_num = MMIAPIPHONE_GetSimAvailableNum(PNULL, 0);

        switch (s_call_bar_operation)
        {
        case MMISET_CALL_BAR_OPEN:
            if (sim_num > 0)
            {
                MMISET_StartCallWaitOrBar(dual_sys);/*lint !e64*/
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            break;
            
        case MMISET_CALL_BAR_CLOSE:
            if (sim_num > 0)
            {
                MMISET_CloseCallWaitOrBar(dual_sys);/*lint !e64*/
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            break;

        case MMISET_CALL_BAR_CANCEL_ALL:
            if (sim_num > 0)
            {
                MMISET_CloseAllCallBar(dual_sys);/*lint !e64*/
            }
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            break;

        // change call-bar password
        case MMISET_CALL_BAR_MODIFY_PWD:
            if (sim_num > 0)
            {
                MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_CHANGE_PW_WIN_TAB, (ADD_DATA)dual_sys);
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

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);      
        break;
        
    case MSG_APP_FLIP:
    case MSG_APP_RED:
        recode = MMI_RESULT_FALSE;
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}



/*****************************************************************************/
//  Description : to handle register new call bar password
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_STRING_T    title       =   {0};
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
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
        GUIAPICTRL_SetBothRect(MMISET_SET_BAR_CHANGE_PW_CTRL_ID, &edit_both_rect);

       //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_BAR_CHANGE_PW_CTRL_ID,0,MMISET_BAR_PASSWORD_MAX_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMI_GetLabelTextByLang(TXT_INPUT_NEW_BAR_PW, &title);   
        GUIWIN_SetTitleText( win_id, title.wstr_ptr, title.wstr_len, FALSE);
        MMK_SetAtvCtrl(win_id,MMISET_SET_BAR_CHANGE_PW_CTRL_ID);
        break;
        
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        MMK_CreatePubEditWin((uint32*)MMISET_SET_BAR_CHANGE_PW_AGAIN_WIN_TAB, (ADD_DATA)dual_sys);
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
//  Description : to handle register new call bar password again
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRegisterNewBarPasswordAgainWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_STRING_T    title       =   {0};
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMI_STRING_T   new_password_value = {0};
    MMI_STRING_T   new_password_again_value = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
         MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(MMISET_SET_BAR_CHANGE_PW_AGAIN_CTRL_ID, &edit_both_rect);

       //set left softkey
        GUIEDIT_SetSoftkey(MMISET_SET_BAR_CHANGE_PW_AGAIN_CTRL_ID,0,MMISET_BAR_PASSWORD_MAX_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMI_GetLabelTextByLang(TXT_INPUT_NEW_BAR_PW_AGAIN, &title);   
        GUIWIN_SetTitleText( win_id, title.wstr_ptr, title.wstr_len, FALSE);
        MMK_SetAtvCtrl(win_id,MMISET_SET_BAR_CHANGE_PW_AGAIN_CTRL_ID);
        break;
        
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:        
        // get the value about new password
        GUIEDIT_GetString(MMISET_SET_BAR_CHANGE_PW_AGAIN_CTRL_ID,&new_password_value);

        // get the value about new password again
        GUIEDIT_GetString(MMISET_SET_BAR_CHANGE_PW_CTRL_ID,&new_password_again_value);

        if(MMIAPICOM_StrCompare(&new_password_value, &new_password_again_value))
        {
            SCI_MEMSET(g_mmiset_ss_pwd.new_pwd, 0x00, sizeof(g_mmiset_ss_pwd.new_pwd));
            SCI_MEMSET(g_mmiset_ss_pwd.new_pwd_two, 0x00, sizeof(g_mmiset_ss_pwd.new_pwd_two));

            MMI_WSTRNTOSTR((uint8 *)g_mmiset_ss_pwd.new_pwd,
                                        MMISET_BAR_PASSWORD_MAX_LEN,
                                        new_password_value.wstr_ptr,
                                        new_password_value.wstr_len,
                                        new_password_value.wstr_len);

            MMI_WSTRNTOSTR((uint8 *)g_mmiset_ss_pwd.new_pwd_two,
                                        MMISET_BAR_PASSWORD_MAX_LEN,
                                        new_password_again_value.wstr_ptr,
                                        new_password_again_value.wstr_len,
                                        new_password_again_value.wstr_len);

            MMISET_ChangeCallBarPassWord(dual_sys);/*lint !e64*/
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);

            //清空password
            GUIEDIT_ClearAllStr(MMISET_SET_BAR_CHANGE_PW_CTRL_ID);
            
            //close input new pin again window
            MMK_CloseWin(win_id);
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
// Description : open select network win
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-21
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSelNetWorkWin(MN_DUAL_SYS_E dual_sys)
{
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    {
#if defined(MMI_PREFER_PLMN_SUPPORT) && defined MMI_GUI_STYLE_TYPICAL
        MMK_CreateWin((uint32*)MMISET_SET_NETWORK_SETTING_WIN_TAB,(ADD_DATA)dual_sys);
#else
#ifdef MMI_DUALMODE_ENABLE
        MMK_CreatePubListWin((uint32*)MMISET_NETWORK_METHOD_WIN_TAB,(ADD_DATA)dual_sys);
#else
        MMK_CreatePubListWin((uint32*)MMISET_SET_SELECT_NETWORK_WIN_TAB,(ADD_DATA)dual_sys);
#endif
#endif
    }
}

/*****************************************************************************/
// Description : open set network win
// Global resource dependence : 
// Author: mary.xiao
// Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSetNetWorkWin(void)
{
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint32 sim_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    select_type = MMISET_SET_NETWORK;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
#if defined(MMI_PREFER_PLMN_SUPPORT) && defined MMI_GUI_STYLE_TYPICAL
        MMK_CreateWin((uint32*)MMISET_SET_NETWORK_SETTING_WIN_TAB,(ADD_DATA)sim_sys);
#else
#ifdef MMI_DUALMODE_ENABLE
        MMK_CreatePubListWin((uint32*)MMISET_NETWORK_METHOD_WIN_TAB,(ADD_DATA)sim_sys);
#else
        MMK_CreatePubListWin((uint32*)MMISET_SET_SELECT_NETWORK_WIN_TAB,(ADD_DATA)sim_sys);
#endif
#endif
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// Description : open set prefer network win
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPreferNetWorkWin(void)
{
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint32 sim_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    select_type = MMISET_SET_PREFER_NETWORK;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB, (ADD_DATA)sim_sys);
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}
#endif

/*****************************************************************************/
//  Description : to select network
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    uint16 cur_selection = 0;
    uint16 no_seltction = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MN_PHONE_SELECT_PLMN_T  sel_net_mode    =   {MN_PHONE_PLMN_SELECT_MANUAL_MODE, 0};/*lint !e785*/
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_SELECT_NETWORK_CTRL_ID,MMISET_NETWORK_SEL_MAX_ITEM, FALSE );//max item 2
        MMISET_AppendSelectNetworkListItem(dual_sys,MMISET_SET_SELECT_NETWORK_CTRL_ID);/*lint !e64*/

        GUILIST_SetOwnSofterKey( MMISET_SET_SELECT_NETWORK_CTRL_ID, TRUE );
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_SELECT_NETWORK_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_GET_FOCUS:
        MNPHONE_GetPlmnSelectModeEx(dual_sys, &sel_net_mode);/*lint !e64*/
        switch (sel_net_mode.select_mode)
        {
        case MN_PHONE_PLMN_SELECT_AUTO_MODE:
            cur_selection = 0;
            no_seltction = 1;
            break;
            
        case MN_PHONE_PLMN_SELECT_MANUAL_MODE:
            cur_selection = 1;
            no_seltction = 0;
            break;
            
        default:
            //SCI_TRACE_LOW:"HandleSelectNetworkWindow: select network mode is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_4923_112_2_18_2_50_26_72,(uint8*)"d",sel_net_mode.select_mode);
            break;
        }
        GUILIST_SetSelectedItem( MMISET_SET_SELECT_NETWORK_CTRL_ID, no_seltction, FALSE );
        GUILIST_SetSelectedItem( MMISET_SET_SELECT_NETWORK_CTRL_ID, cur_selection, TRUE );
        break;
        
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            MMISET_SetSelectNetworkMode(dual_sys,MMISET_SET_SELECT_NETWORK_CTRL_ID);/*lint !e64*/
            #if !defined MMI_GUI_STYLE_TYPICAL
                MMK_CloseWin( win_id );
            #endif
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            // 释放保存网络列表信息的指针
            if (PNULL != g_plmn_detail_list_ptr)
            {
                SCI_FREE(g_plmn_detail_list_ptr);
                g_plmn_detail_list_ptr = PNULL;
            }
            
            MMK_CloseWin( win_id );      
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }   
    return recode;
}
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : 获得未使用的IPNumber ID
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
uint8 GetNotUsedIPNumberId(MN_DUAL_SYS_E dual_sys)
{
    MMISET_IP_NUMBER_INFO_T     ip_number_info  =   {0};
    int16                       i               =   0;
    
    MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);
    
    for ( i = 0; i < MMISET_IPNUMBER_MAX_NUM; i++ )
    {
        if(0 == strlen( (char*)ip_number_info.number[i] ))
        {
            return i;
        }
    }

    return MMISET_IPNUMBER_MAX_NUM;
}

/*****************************************************************************/
//  Description : list ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleListIpNumberWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    uint32                      dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_LIST_IPNUMBER_CTRL_ID;
    MMISET_IP_NUMBER_INFO_T     ip_number_info  =   {0};/*lint !e64*/
    uint16                      cur_index       =   0;
    uint8                       origin_active_id    =   0;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_IPNUMBER_MAX_NUM, FALSE );//max item 5
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        MMISET_GetIPNumberInfo(dual_sys, &ip_number_info);/*lint !e64*/
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        AppendIpNumberItemToList(dual_sys,ctrl_id);/*lint !e64*/
        GUILIST_SetSelectedItem(ctrl_id, ip_number_info.active_id, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
        s_ipnumber_list_cur_id = GUILIST_GetCurItemIndex(ctrl_id);
        if(!IsActiveIPNumber(dual_sys, s_ipnumber_list_cur_id))/*lint !e64*/
        {
            MMK_CreateWin((uint32*)MMISET_IPNUMBER_ACTIVE_OPT_WIN_TAB, (ADD_DATA)dual_sys);
        }
        else
        {
            MMK_CreateWin((uint32*)MMISET_IPNUMBER_DEACTIVE_OPT_WIN_TAB, (ADD_DATA)dual_sys);
        }
        break;
#if defined(MMI_PDA_SUPPORT)
    case MSG_APP_MENU:
        MMK_CreateWin((uint32*)MMISET_IPNUMBER_ADD_OPT_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        s_ipnumber_list_cur_id = GUILIST_GetCurItemIndex(ctrl_id);
        if(!IsActiveIPNumber(dual_sys, s_ipnumber_list_cur_id))/*lint !e64*/
        {
            MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);/*lint !e64*/
            origin_active_id = ip_number_info.active_id;
            
            if(origin_active_id != s_ipnumber_list_cur_id)
            {
                ip_number_info.active_id = s_ipnumber_list_cur_id; 
                MMISET_SetIPNumberInfo(&ip_number_info);
                GUILIST_SetSelectedItem( MMISET_LIST_IPNUMBER_CTRL_ID, origin_active_id, FALSE );
                GUILIST_SetSelectedItem( MMISET_LIST_IPNUMBER_CTRL_ID, s_ipnumber_list_cur_id, TRUE );               
            }
        }
        else
        {
            MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);/*lint !e64*/
            ip_number_info.active_id = MMISET_IPNUMBER_MAX_NUM; //means ip number invalid
            
            MMISET_SetIPNumberInfo(&ip_number_info);
            GUILIST_SetSelectedItem( MMISET_LIST_IPNUMBER_CTRL_ID, s_ipnumber_list_cur_id, FALSE );
        }
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
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
//  Description : Is this ip number active
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
BOOLEAN IsActiveIPNumber(MN_DUAL_SYS_E dual_sys,uint16 cur_index)
{
    MMISET_IP_NUMBER_INFO_T     ipnumber_info   =   {0};/*lint !e64*/
    
    MMISET_GetIPNumberInfo(dual_sys, &ipnumber_info);
    
    return (ipnumber_info.active_id == cur_index);
}

/*****************************************************************************/
//  Description : append ip number item to listbox
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItemToList(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMI_CTRL_ID_T ctrl_id
                                    )
{
    MMISET_IP_NUMBER_INFO_T     ip_number_info  =   {0};/*lint !e64*/
    int16                       i               =   0;
    
    MMISET_GetIPNumberInfo(dual_sys,&ip_number_info);
    
    for ( i = 0; i < MMISET_IPNUMBER_MAX_NUM; i++ )
    {
        if(0 < strlen( (char*)ip_number_info.number[i] ))
        {
            AppendIpNumberItem( ctrl_id, &ip_number_info, i, FALSE );
        }
    }
}

/*****************************************************************************/
//  Description : append listbox item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendIpNumberItem(
                              MMI_CTRL_ID_T                 ctrl_id,
                              const MMISET_IP_NUMBER_INFO_T *ip_info_ptr,
                              uint16                        pos,
                              BOOLEAN                       is_update
                              )
{
    uint8                   number_len  =   0;
    GUILIST_ITEM_T          item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data   =   {0};/*lint !e64*/
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
    wchar                   temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    
    //SCI_ASSERT( pos < MMISET_IPNUMBER_MAX_NUM );
   if(pos >= MMISET_IPNUMBER_MAX_NUM)
   {
        //SCI_TRACE_LOW:"mmisetting_assert AppendIpNumberItem pos %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_5100_112_2_18_2_50_26_73,(uint8*)"d", pos);
        return;
    }     
#if !defined MMI_GUI_STYLE_TYPICAL
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
       
    if ( pos == ip_info_ptr->active_id )
    {
        image_id = IMAGE_RADIO_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_RADIO_UNSELECTED_ICON;
    }
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    number_len = strlen( (char*)ip_info_ptr->number[pos] );
    
    item_data.item_content[0].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, number_len );
    
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
    
    MMI_STRNTOWSTR( temp_wstr,
        GUILIST_STRING_MAX_NUM,
        (uint8*)ip_info_ptr->number[pos], 
        item_data.item_content[0].item_data.text_buffer.wstr_len,
        item_data.item_content[0].item_data.text_buffer.wstr_len );

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;
#else
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if ( pos == ip_info_ptr->active_id )
    {
        image_id = IMAGE_RADIO_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_RADIO_UNSELECTED_ICON;
    }
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = image_id;
    
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    number_len = strlen( (char*)ip_info_ptr->number[pos] );
    
    item_data.item_content[1].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, number_len );
    
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_wstr;
    
    MMI_STRNTOWSTR( temp_wstr,
        GUILIST_STRING_MAX_NUM,
        (uint8*)ip_info_ptr->number[pos], 
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        item_data.item_content[1].item_data.text_buffer.wstr_len );
#endif
    
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
//  Description : handle ip number option menu window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E                recode              =   MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T          group_id           =   0;
    MMI_MENU_ID_T                menu_id            =   0;
    MMISET_IP_NUMBER_INFO_T     ipnumber_info       =   {0};/*lint !e64*/
    uint8                       origin_active_id    =   0;
    uint32                      dual_sys            =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T               list_id         =   MMISET_LIST_IPNUMBER_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_IPNUMBER_OPT_CTRL_ID;
    ipnumber_info.dual_sys = dual_sys;/*lint !e64*/
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16  total_num = 0;

            total_num = GUILIST_GetTotalItemNum(list_id);

            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

            if(0 == total_num)
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ACTIVE, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_DEACTIVE, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_MODIFY, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_DEL, TRUE);
            }
            else if(MMISET_IPNUMBER_MAX_NUM == total_num)
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ADD, TRUE);
            }

            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_IPNUMBER_ACTIVE:
            {
                MMISET_GetIPNumberInfo(dual_sys,&ipnumber_info);/*lint !e64*/
                origin_active_id = ipnumber_info.active_id;
                
                if(origin_active_id != s_ipnumber_list_cur_id)
                {
                    ipnumber_info.active_id = s_ipnumber_list_cur_id; 
                    MMISET_SetIPNumberInfo(&ipnumber_info);
                    GUILIST_SetSelectedItem( list_id, origin_active_id, FALSE );
                    GUILIST_SetSelectedItem( list_id, GUILIST_GetCurItemIndex(list_id), TRUE );               
                }
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);           
                MMK_CloseWin(win_id);
            }
            break;
            
        case ID_SET_IPNUMBER_DEACTIVE:
            {
                MMISET_GetIPNumberInfo(dual_sys,&ipnumber_info);/*lint !e64*/
                ipnumber_info.active_id = MMISET_IPNUMBER_MAX_NUM; //means ip number invalid
                
                MMISET_SetIPNumberInfo(&ipnumber_info);
                GUILIST_SetSelectedItem( list_id, GUILIST_GetCurItemIndex(list_id), FALSE );
                
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);           
                MMK_CloseWin(win_id);
            }
            break;

        case ID_SET_IPNUMBER_MODIFY:
            s_is_add_ipnumber = FALSE;
            MMK_CreatePubEditWin((uint32*)MMISET_MODIFY_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
            MMK_CloseWin(win_id);
            break;
            
        case ID_SET_IPNUMBER_ADD:
            s_is_add_ipnumber = TRUE;
            MMK_CreatePubEditWin((uint32*)MMISET_MODIFY_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
            MMK_CloseWin(win_id);
            break;

        case ID_SET_IPNUMBER_DEL:
            {
                uint16                      index = 0;
                uint16                      i = 0;
                MMISET_IP_NUMBER_INFO_T     ip_info = {0};

                index = GUILIST_GetCurItemIndex(MMISET_LIST_IPNUMBER_CTRL_ID);

                if (index < MMISET_IPNUMBER_MAX_NUM)
                {
                    MMISET_GetIPNumberInfo(dual_sys, &ip_info);
 
                    if(ip_info.active_id >= index)
                    {
                        ip_info.active_id--;
                    }

                    for(i = index; i < MMISET_IPNUMBER_MAX_NUM - 1; i++)
                    {
                        SCI_MEMCPY(ip_info.number[i], ip_info.number[i + 1], (MMISET_IPNUMBER_MAX_LEN + 1));
                    }

                    SCI_MEMSET(ip_info.number[MMISET_IPNUMBER_MAX_NUM - 1], 0, (MMISET_IPNUMBER_MAX_LEN + 1));

                    MMISET_SetIPNumberInfo(&ip_info);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
                }

                MMK_CloseWin(win_id);
            }
            break;
        default:
            //SCI_PASSERT(FALSE,("HandleIpNumberOptionWindow menu_id = %d", menu_id));  
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

#if defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : handle ip number add option menu window
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIpNumberAddOptionWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E                recode              =   MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T          group_id           =   0;
    MMI_MENU_ID_T                menu_id            =   0;
    MMISET_IP_NUMBER_INFO_T     ipnumber_info       =   {0};/*lint !e64*/
    //uint8                       origin_active_id    =   0;
    uint32                      dual_sys            =   (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T               list_id         =   MMISET_LIST_IPNUMBER_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_IPNUMBER_ADD_OPT_CTRL_ID;
    ipnumber_info.dual_sys = dual_sys;/*lint !e64*/
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16  total_num = 0;

            total_num = GUILIST_GetTotalItemNum(list_id);

            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

            if(MMISET_IPNUMBER_MAX_NUM == total_num)
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ADD, TRUE);
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, ID_SET_IPNUMBER_ADD, FALSE);
            }

            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {          
        case ID_SET_IPNUMBER_ADD:
            s_is_add_ipnumber = TRUE;
            MMK_CreatePubEditWin((uint32*)MMISET_MODIFY_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
            MMK_CloseWin(win_id);
            break;
        default:
            //SCI_PASSERT(FALSE,("HandleIpNumberOptionWindow menu_id = %d", menu_id));  
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_LOSE_FOCUS:
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
//  Description : modify ip number window
//  Global resource dependence : 
//  Author:lin.lin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleModifyIPNumberWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    uint32              dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_BG_T            edit_bg         =   {0};
    GUI_BORDER_T        edit_border     =   {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //@fen.xie MS00186982
        GUIEDIT_SetStyle(MMISET_MODIFY_IPNUMBER_CTRL_ID, GUIEDIT_STYLE_SINGLE);
        
        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(MMISET_MODIFY_IPNUMBER_CTRL_ID,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(MMISET_MODIFY_IPNUMBER_CTRL_ID,&edit_border);

        //set font
        //GUIEDIT_SetFontColor(MMISET_MODIFY_IPNUMBER_CTRL_ID,MMI_BLACK_COLOR);

        {
            MMISET_IP_NUMBER_INFO_T     ip_info                                     =   {0};/*lint !e64*/
            wchar                       number_wstr[MMISET_IPNUMBER_MAX_NUM + 1]    =   {0};
            uint16                      num_len                                     =   0;
            GUI_BOTH_RECT_T edit_both_rect = {0};

            edit_both_rect = MMIAPISET_GetPwEditRect(win_id );//MS00199646
            GUIAPICTRL_SetBothRect(MMISET_MODIFY_IPNUMBER_CTRL_ID, &edit_both_rect);

            MMISET_GetIPNumberInfo(dual_sys, &ip_info);/*lint !e64*/
            
            if(s_is_add_ipnumber)
            {
                uint8 not_used_id = GetNotUsedIPNumberId(dual_sys);
                num_len = (uint16)strlen((char*)ip_info.number[not_used_id]);
                MMI_STRNTOWSTR( number_wstr, MMISET_IPNUMBER_MAX_LEN, (uint8*)ip_info.number[not_used_id], num_len, num_len );
            }
            else
            {
                num_len = (uint16)strlen((char*)ip_info.number[s_ipnumber_list_cur_id]);
                MMI_STRNTOWSTR( number_wstr, MMISET_IPNUMBER_MAX_LEN, (uint8*)ip_info.number[s_ipnumber_list_cur_id], num_len, num_len );
            }
            GUIEDIT_SetString(MMISET_MODIFY_IPNUMBER_CTRL_ID, number_wstr, num_len);
        }
        
        //set left softkey
        GUIEDIT_SetSoftkey(MMISET_MODIFY_IPNUMBER_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        
        MMK_SetAtvCtrl(win_id, MMISET_MODIFY_IPNUMBER_CTRL_ID);
        break;
        
 #ifndef MMIPUBWIN_PDASTYLE_SUPPORT 
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_IP);
        break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T                string_info     =   {0};
            MMISET_IP_NUMBER_INFO_T     ip_info         =   {0};/*lint !e64*/
            
            MMISET_GetIPNumberInfo(dual_sys,&ip_info);/*lint !e64*/
            GUIEDIT_GetString(MMISET_MODIFY_IPNUMBER_CTRL_ID, &string_info);
            if(0 != string_info.wstr_len)            
            {
                uint32 i = 0;
                BOOLEAN is_input_valid = TRUE;
                if(s_is_add_ipnumber)
                {
                    uint8 not_used_id = GetNotUsedIPNumberId(dual_sys);
                    
                    SCI_MEMSET(ip_info.number[not_used_id], 0, (MMISET_IPNUMBER_MAX_LEN + 1));
                    
                    MMI_WSTRNTOSTR( (uint8*)ip_info.number[not_used_id], MMISET_IPNUMBER_MAX_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len);
                    
                    for (i = 0; i < string_info.wstr_len; i++)
                    {
                        if (ip_info.number[not_used_id][i] < '0' || ip_info.number[not_used_id][i] > '9')
                        {
                            is_input_valid = FALSE;
                            break;
                        }
                    }
                }
                else
                {
                    SCI_MEMSET(ip_info.number[s_ipnumber_list_cur_id], 0, (MMISET_IPNUMBER_MAX_LEN + 1));
                    
                    MMI_WSTRNTOSTR( (uint8*)ip_info.number[s_ipnumber_list_cur_id], MMISET_IPNUMBER_MAX_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len);
                    
                    for (i = 0; i < string_info.wstr_len; i++)
                    {
                        if (ip_info.number[s_ipnumber_list_cur_id][i] < '0' || ip_info.number[s_ipnumber_list_cur_id][i] > '9')
                        {
                            is_input_valid = FALSE;
                            break;
                        }
                    }
                }
                
                if (is_input_valid)
                {
                    MMISET_SetIPNumberInfo(&ip_info);
                    //AppendIpNumberItemToList(dual_sys, MMISET_LIST_IPNUMBER_CTRL_ID);/*lint !e64*/
                    //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                }
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
/*****************************************************************************/
//  Description : 处理分钟提示音
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMinuteReminderWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_MINUTE_REMINDER_CTRL_ID;
    MMI_STRING_T label_name  = {0};
    
    uint16 sel_item_index = 0;
    uint8 call_reminder_time = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_CALL_REMINDER_TIME_MAX, FALSE);//max item 5
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_50S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_55S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SET_TIME_60S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_EXPORT_LOC_CUSTOM, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);     
        MMK_SetAtvCtrl(win_id,ctrl_id);
        
        switch(MMIAPISET_GetMinuteReminderTime())
        {
        case CC_CALL_REMINDER_TIME_OFF:
            sel_item_index = MMISET_CALL_REMINDER_TIME_OFF;
            break;
            
        case CC_CALL_REMINDER_TIME_50S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_50S;
            break;
            
        case CC_CALL_REMINDER_TIME_55S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_55S;
            break;
            
        case CC_CALL_REMINDER_TIME_60S:
            sel_item_index =MMISET_CALL_REMINDER_TIME_60S;
            break;
            
        default :
            sel_item_index = MMISET_CALL_REMINDER_TIME_OTHER;
            break;
        }
        
        GUILIST_SetCurItemIndex(ctrl_id, sel_item_index);
        GUILIST_SetSelectedItem(ctrl_id, sel_item_index, TRUE);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_OK:
    case MSG_APP_WEB:
        sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
        switch (sel_item_index)
        {
        case MMISET_CALL_REMINDER_TIME_OFF:        
        case MMISET_CALL_REMINDER_TIME_50S:
        case MMISET_CALL_REMINDER_TIME_55S: 
        case MMISET_CALL_REMINDER_TIME_60S:
            call_reminder_time = g_call_reminder_time[sel_item_index];
            MMISET_SetMinuteReminderTime(call_reminder_time);

            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                switch(MMIAPISET_GetMinuteReminderTime())
                {
                case CC_CALL_REMINDER_TIME_OFF:
                    MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
                    break;
                    
                case CC_CALL_REMINDER_TIME_50S:
                    MMI_GetLabelTextByLang(TXT_SET_TIME_50S, &label_name);
                    break;
                    
                case CC_CALL_REMINDER_TIME_55S:
                    MMI_GetLabelTextByLang(TXT_SET_TIME_55S, &label_name);
                    break;
                    
                case CC_CALL_REMINDER_TIME_60S:
                    MMI_GetLabelTextByLang(TXT_SET_TIME_60S, &label_name);
                    break;
                    
                default :
                    MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &label_name);
                    break;
                }
                        
            GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID, &label_name, FALSE);
            MMK_CloseWin( win_id );
            break;
            
        case MMISET_CALL_REMINDER_TIME_OTHER:
            MMK_CreatePubEditWin((uint32*)MMISET_SET_MINUTE_REMINDER_EDIT_WIN_TAB,PNULL);
            break;
            
        default:
            break;
        }
        break;
    //根据用户自定义输入的时间值确定是否选中默认项
    case MSG_GET_FOCUS:
        switch(MMIAPISET_GetMinuteReminderTime())
        {
        case CC_CALL_REMINDER_TIME_OFF:
            sel_item_index = MMISET_CALL_REMINDER_TIME_OFF;
            break;
            
        case CC_CALL_REMINDER_TIME_50S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_50S;
            break;
            
        case CC_CALL_REMINDER_TIME_55S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_55S;
            break;
            
        case CC_CALL_REMINDER_TIME_60S:
            sel_item_index = MMISET_CALL_REMINDER_TIME_60S;
            break;
            
        default :
            sel_item_index = MMISET_CALL_REMINDER_TIME_OTHER;
            MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &label_name);
                GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID, &label_name, FALSE);
            break;
        }
       
        GUILIST_SetSelectedItem(ctrl_id, sel_item_index, TRUE);
       

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
/*****************************************************************************/
//  Description : 自定义分钟提示音窗口
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMinuteReminderEditWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id =   MMISET_SET_MINUTE_REMINDER_EDITBOX_CTRL_ID;
    MMI_STRING_T time_number = {0};
    wchar num_wchar_str[3] = {0};
    char num_char_str[3] = {0};
    uint32 call_reminder_time = 0;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BG_T edit_bg = {0};
    GUI_BORDER_T edit_border = {0};

    time_number.wstr_ptr = num_wchar_str;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN;
            both_rect.v_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + SET_MINUTE_REMINDER_LABLE_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + SET_MINUTE_REMINDER_LABLE_HEIGHT;

            GUIAPICTRL_SetBothRect(MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID, &both_rect);
        }
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        
        GUILABEL_SetTextById(MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID,TXT_SET_MINUTE_REMINDER_TIME,FALSE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color = MMI_WHITE_COLOR;
        edit_bg.shape = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(ctrl_id,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(ctrl_id,&edit_border);
        
        {
            edit_both_rect = MMITHEME_GetWinClientBothRect(win_id);

            edit_both_rect.h_rect.left = edit_both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;

            edit_both_rect.h_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN + SET_MINUTE_REMINDER_LABLE_HEIGHT + MMI_NUMBER_EDIT_TOP_MARGIN;
            edit_both_rect.v_rect.top += SET_MINUTE_REMINDER_LABLE_TOP_MARGIN + SET_MINUTE_REMINDER_LABLE_HEIGHT + MMI_NUMBER_EDIT_TOP_MARGIN;

            edit_both_rect.h_rect.bottom = edit_both_rect.h_rect.top + MMI_NUMBER_EDIT_HEIGHT;
            edit_both_rect.h_rect.right = edit_both_rect.h_rect.right - MMI_NUMBER_EDIT_HEIGHT;
            edit_both_rect.v_rect.bottom = edit_both_rect.v_rect.top + MMI_NUMBER_EDIT_HEIGHT;
            edit_both_rect.v_rect.right = edit_both_rect.v_rect.right - MMI_NUMBER_EDIT_HEIGHT;

        }
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);
        
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);
        
        call_reminder_time = MMIAPISET_GetMinuteReminderTime();
        if (0 != call_reminder_time)
        {
            MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
            MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
            time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
            GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
        }
        else
        {
            GUIEDIT_SetString(ctrl_id, PNULL, 0);
        }

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_GET_FOCUS:
        call_reminder_time = MMIAPISET_GetMinuteReminderTime();
        if (0 != call_reminder_time)
        {
            MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
            MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
            time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
            GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
        }
        else
        {
            GUIEDIT_SetString(ctrl_id, PNULL, 0);
        }

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_EDITBOX_UPDATE_STRNUM:
        GUIEDIT_GetString(ctrl_id, &time_number);
        
        MMIAPICOM_WstrToNum(time_number.wstr_ptr, time_number.wstr_len, &call_reminder_time);     
        if (call_reminder_time > CC_CALL_REMINDER_TIME_60S)
        {
            time_number.wstr_ptr = num_wchar_str;
            call_reminder_time = CC_CALL_REMINDER_TIME_60S;
            MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
            MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
            time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
            GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id, &time_number);
        
        MMIAPICOM_WstrToNum(time_number.wstr_ptr, time_number.wstr_len, &call_reminder_time);        

        if (0 != call_reminder_time)
        {
            MMISET_SetMinuteReminderTime(call_reminder_time); 
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INPUT_ERROR);
        }
        break;
 
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:  
        MMK_CloseWin(win_id);
        break;

#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    case MSG_FULL_PAINT:
#if !defined(MAINLCD_SIZE_176X220) && !defined(MAINLCD_SIZE_128X160)
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_IP);
#else
        SetPwBackground(win_id,ctrl_id,TRUE);
#endif
        break;
#endif

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : deal with signal of APP_MN_DISPLAY_PLMN_IND
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleDisplayPlmnInd(MN_DUAL_SYS_E dual_sys)
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    if (!MMK_CreateWin((uint32 *)MMISET_SEL_NET_WIN_TAB, (ADD_DATA)dual_sys))
    {
        result = MMI_RESULT_FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 在进行手动选网中，网络连接窗口的处理函数
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleManualSelNetworkConnectWindow(
                                                       MMI_WIN_ID_T       win_id,     // 窗口的ID
                                                       MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                       DPARAM             param       // 相应消息的参数
                                                       )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED:
        //SCI_TRACE_LOW("MMISET_HandleSelNetworkConnectWindow:received MSG_APP_RED and abort plmn search!");
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
//  Description : to handle remind user will select network through manual win msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelNetManualWinMsg(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    GUI_RECT_T              rect             =  MMITHEME_GetClientRect();
    MMI_RESULT_E            recode           = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO        lcd_dev_info     =  {GUI_LCD_0, GUI_BLOCK_0};
    uint32                  dual_sys         =  (uint32)MMK_GetWinAddDataPtr(win_id);
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN; //@$discuss,8800用的是MMI_DEFAULT_MAINLCD_BLOCK，此处直接引用了6800中的block_id是否会有问题？
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);;

            both_rect.h_rect.top = both_rect.v_rect.top = MMISET_SS_INTERROGATE_STATUS_BOTTOM;
            both_rect.h_rect.bottom = both_rect.h_rect.top + (MMISET_SS_INTERROGATE_NUMBER_BOTTOM - MMISET_SS_INTERROGATE_STATUS_BOTTOM);
            both_rect.v_rect.bottom = both_rect.v_rect.top + (MMISET_SS_INTERROGATE_NUMBER_BOTTOM - MMISET_SS_INTERROGATE_STATUS_BOTTOM);

            GUIAPICTRL_SetBothRect(MMISET_SEL_NET_MANUAL_CTRL_ID, &both_rect);
        }
        //set soft key
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);
        GUILABEL_SetTextById(MMISET_SEL_NET_MANUAL_CTRL_ID,TXT_SET_MANUAL_SEL,FALSE );
        break;
        
    case MSG_FULL_PAINT:
        //fill rect 
        LCD_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //进入手动选网
        //the list of all plmn
        //@YONGWEI.HE
#ifdef MMI_DUALMODE_ENABLE 
        //  MNPHONE_ListPLMNEx(dual_sys, s_current_sel_rat);
        MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS_AND_3G);/*lint !e64*/  //cr149167
#else
        MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS);/*lint !e64*/
#endif
        
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_NONE,
                                                                HandleManualSelNetworkConnectWindow);

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

#ifdef MMI_DUALMODE_ENABLE
/*****************************************************************************/
//  Description : open NetWorkType win
//  Global resource dependence : 
//  Author: panvs.zhong
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenNetWorkTypeWin(void)
{    
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint32 sim_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    select_type = MMISET_SET_NETWORK_TYPE;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_NETWORK_TYPE_WIN_TAB, (ADD_DATA)sim_sys);
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}
#ifdef MMI_VOLTE_SUPPORT
PUBLIC void MMIAPISET_OpenVoLTEWin(void)
{    
    MMISET_SIM_SELECT_TYPE_E select_type = 0;
    uint32 sim_num = 0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    select_type = MMISET_SET_VOLTE_STATUS;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreatePubListWin((uint32*)MMISET_VOLTE_WIN_TAB,(ADD_DATA)sim_sys);
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#endif
}
#endif

#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTSettingsWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_MENU_GROUP_ID_T         group_id    =   0;
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_CALL_VT_SETTINGS_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_CALL_VT_SETTINGS_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_VT_FALLBACK_HANDLE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_FALLBACK_WIN_TAB,PNULL);
            break;
            
        case ID_SET_VT_SPCIFY_SAVE_PATH:
            MMK_CreateWin((uint32*)MMISET_VT_RECORD_PATH_WIN_TAB,PNULL);
            break;
            
        case ID_SET_VT_REPLACE_IMAGE:
            MMK_CreateWin((uint32*)MMISET_SET_VT_REPLACE_IMAGE_WIN_TAB,PNULL);
            break;  
            
        case ID_SET_VT_LOCAL_VIDEO:
            MMK_CreateWin((uint32*)MMISET_SET_VT_LOCAL_VIDEO_WIN_TAB,PNULL);
            break;
            
        case ID_SET_VT_ONE_TIME_RING:
            MMK_CreateWin((uint32*)MMISET_SET_VT_ONE_TIME_RING_WIN_TAB,PNULL);
            break;            
            
        default:
            //do nothing
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
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTFallbackSettingsWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_CALL_VT_FALLBACK_SETTINGS_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_VT_FALLBACK_MAX, FALSE );//max item 3
        MMISET_AppendFallbackListItem(ctrl_id);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SetSelectFallback(ctrl_id);/*lint !e64*/
        //MMIPUB_OpenAlertSuccessWin( TXT_SET_SUCCESS);
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

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTReplaceImageSettingsWindow(
                                                       MMI_WIN_ID_T    win_id, 
                                                       MMI_MESSAGE_ID_E   msg_id, 
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID;
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16 sel_item_index = 0;
            //从nv中获得替代文件信息
            MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
            MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
            
            sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_SetMaxItem(ctrl_id, MMISET_VT_REPLACE_MAX, FALSE );
            GUILIST_RemoveAllItems(ctrl_id);
            
            {
                GUILIST_ITEM_T item_t = {0};/*lint !e64*/
                GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
                uint32 index = 0;
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
                item_t.item_data_ptr = &item_data;
                
                
                for (index = 0; index < MMISET_VT_REPLACE_MAX; index++)
                {
                    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
                    item_data.item_content[0].item_data.text_id = s_replace_title_text_id[index];
                    
                    if (s_replace_file_info[index].is_valid && (s_replace_file_info[index].file_name_len > 0))
                    {
                        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                        item_data.item_content[1].item_data.text_buffer.wstr_len = s_replace_file_info[index].file_name_len;
                        item_data.item_content[1].item_data.text_buffer.wstr_ptr = s_replace_file_info[index].file_name;
                    }
                    else
                    {
                        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
                        item_data.item_content[1].item_data.text_id = TXT_SET_VT_REPLACE_DEFAULT;
                    }   
                    GUILIST_AppendItem( ctrl_id, &item_t );
                }
            }
            GUILIST_SetCurItemIndex(ctrl_id, sel_item_index);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        {
            MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
            
            uint16 sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            
            //从nv中获得替代文件信息
            MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
            if (s_replace_file_info[sel_item_index].is_valid && (s_replace_file_info[sel_item_index].file_name_len > 0))
            {
                if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                    break;
                }
                else if (MMIAPIENG_GetIQModeStatus())
                {
                    MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
                    break;
                }
                
                MMK_CreateWin((uint32 *)MMISET_VT_REPLACE_REVIEW_PHOTO_WIN_TAB, PNULL);
            }
            else
            {
                uint32 time_out = 500;
                MMIAPIOCR_ClosePreviewWinOpen();
                //显示请稍候
                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                MMK_PostMsg(win_id, MSG_MULTIM_WAITING, NULL, 0);                
            }
        }
        //MMK_CloseWin(win_id);
        break;
        
    case MSG_MULTIM_WAITING:
        {
            uint16 sel_item_index = 0;
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PICTURE_JPG,0,MMISET_VT_REPLACE_PIC_SIZE_MAX,0,PNULL,0,0};
            sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            //select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = s_replace_media_type[sel_item_index];
            MMIPUB_CloseAlertWin();
            select_info.win_id = win_id;
            MMIAPIFMM_OpenSelectPictureWin(&select_info);
        }
        break;
        
    case MSG_MULTIM_SELECTED_RETURN:
        {
            MULTIM_SELECT_RETURN_T * multim_select_ptr = PNULL;
            
           // SCI_ASSERT(param != 0);
	   if(param == 0)
	   {
	        //SCI_TRACE_LOW:"mmisetting_assert HandleVTReplaceImageSettingsWindow"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6023_112_2_18_2_50_28_74,(uint8*)"");
	        break;
	    } 
            multim_select_ptr = (MULTIM_SELECT_RETURN_T *)param;
            switch (multim_select_ptr->file_type)
            {
            case MMIFMM_FILE_TYPE_PICTURE:
                if (multim_select_ptr->file_name_len > MMIFILE_FULL_PATH_MAX_LEN)
                {
                    return MMI_RESULT_FALSE;
                }
                else if (!IsReplaceImageValid(multim_select_ptr))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                }
                else
                {
                    MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
                    int16 list_index = GUILIST_GetCurItemIndex(ctrl_id);
                    //SCI_ASSERT(list_index < MMISET_VT_REPLACE_MAX);
			if(list_index >= MMISET_VT_REPLACE_MAX)
			{
			    //SCI_TRACE_LOW:"mmisetting_assert HandleVTReplaceImageSettingsWindow list_index %d"
			    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6045_112_2_18_2_50_28_75,(uint8*)"d", list_index);
			    break;
			}                     
                    //从nv中获得替代文件信息
                    MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
                    
                    s_replace_file_info[list_index].is_valid = TRUE;
                    s_replace_file_info[list_index].file_name_len = MIN((multim_select_ptr->file_name_len), MMIFILE_FULL_PATH_MAX_LEN);
                    SCI_MEMSET(s_replace_file_info[list_index].file_name, 0 , sizeof(s_replace_file_info[list_index].file_name));
                    MMIAPICOM_Wstrncpy(s_replace_file_info[list_index].file_name, multim_select_ptr->file_name_ptr, s_replace_file_info[list_index].file_name_len);
                    s_replace_file_info[list_index].file_size = multim_select_ptr->file_size;
                    s_replace_file_info[list_index].media_type = MMISET_VT_REPLACE_MEDIA_TYPE_IMAGE;
                    s_replace_file_info[list_index].photo_type = multim_select_ptr->pic_type;
                    //存入nv中
                    MMISET_WriteReplaceInfoToNv((MMISET_VT_REPLACE_FILE_INFO_T*)&s_replace_file_info);
                    
                }
                break;
                
            case MMIFMM_FILE_TYPE_MUSIC:
                break;
                
            case MMIFMM_FILE_TYPE_MOVIE:  
                break;
                
            default:
                //SCI_PASSERT(0,("msg_id %d",msg_id));
                break;
            }
        }
        
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

/*****************************************************************************/
//  Description : 判断替代图片格式是否有效
//  Global resource dependence : none
//  Author: samboo
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsReplaceImageValid(MULTIM_SELECT_RETURN_T * multim_select_ptr)
{
    BOOLEAN is_valid = FALSE;
    uint16 pic_width = 0;
    uint16 pic_height = 0;
    
    if (PNULL == multim_select_ptr)
    {
        //SCI_TRACE_LOW:"IsReplaceImageValid : null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6103_112_2_18_2_50_28_76,(uint8*)"");
    }
    else
    {
        GUIANIM_GetImgWidthHeight(&pic_width, &pic_height, multim_select_ptr->file_name_ptr, multim_select_ptr->file_name_len);
        //SCI_TRACE_LOW:"IsReplaceImageValid width:%d height:%d size:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6108_112_2_18_2_50_28_77,(uint8*)"ddd", pic_width, pic_height, multim_select_ptr->file_size);
        //替代图片分辨率不能大于320*240，文件大小不能超过32k 格式必须是jpeg
        is_valid = (pic_width <= MMISET_VT_REPLACE_PIC_WIDTH_MAX)
            &&(pic_height <= MMISET_VT_REPLACE_PIC_HEIGHT_MAX)
            &&(multim_select_ptr->file_size <= MMISET_VT_REPLACE_PIC_SIZE_MAX)
            &&(GUIANIM_TYPE_IMAGE_JPG == multim_select_ptr->pic_type);
    }
    return is_valid;
}


/*****************************************************************************/
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoWinMsg
(
 MMI_WIN_ID_T        win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 )
{
    MMI_RESULT_E                result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISET_VT_REPLACE_REVIEW_PHOTO_ANIM_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        //set anim control param
        {
            MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
            uint16 index = GUILIST_GetCurItemIndex(MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID);
            
            //从nv中获得替代文件信息
            MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
            MMIAPIFMM_SetPreviewPicParam(FALSE,(wchar *)s_replace_file_info[index].file_name,
                (uint16)s_replace_file_info[index].file_name_len,
                ctrl_id);
            GUIWIN_SetTitleText(win_id, s_replace_file_info[index].file_name, s_replace_file_info[index].file_name_len, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        {                                                      
            GUI_RECT_T rect = {0};
            GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0}; 
            lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
            lcd_dev_info.block_id = GUI_BLOCK_MAIN;
            rect.left = 0;
            rect.top = MMI_TITLE_HEIGHT;
            rect.right = MMISET_FULLSCREEN_CLIENT_RIGHT;
            rect.bottom = MMISET_FULLSCREEN_CLIENT_BOTTOM;
            LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR);
            
        }    
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        MMK_CreateWin((uint32 *) MMISET_VT_REPLACE_REVIEW_PHOTO_OPT_WIN_TAB, PNULL);
        MMK_CloseWin(win_id);
        break;
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
//  Description : 预览自定义图片窗口
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReviewCustomPhotoOptWinMsg
(
 MMI_WIN_ID_T        win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 )
{
    uint32          time_out = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISET_VT_CUSTOM_PHOTO_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T          group_id = 0;
    MMI_MENU_ID_T                menu_id = 0;
    
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
        GUIMENU_GetId(ctrl_id, 
            &group_id, 
            &menu_id
            );
        switch(menu_id)
        {
        case ID_SET_VT_CUSTOM_PHOTO_MODIFY:
            //显示请稍候
            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            MMK_PostMsg(win_id, MSG_MULTIM_WAITING, NULL, 0);
            break;
            
        case ID_SET_VT_CUSTOM_PHOTO_DEL:
            {
                uint16 list_index = GUILIST_GetCurItemIndex(MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID);
                MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
                
                //从nv中获得替代文件信息
                MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
                s_replace_file_info[list_index].is_valid = FALSE;
                s_replace_file_info[list_index].file_name_len = 0;
                s_replace_file_info[list_index].file_size = 0;
                SCI_MEMSET(s_replace_file_info[list_index].file_name, 0 , sizeof(s_replace_file_info[list_index].file_name));
                MMISET_WriteReplaceInfoToNv((MMISET_VT_REPLACE_FILE_INFO_T*)&s_replace_file_info);
                MMK_CloseWin(win_id);
            }
            
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_MULTIM_WAITING:
            {
                MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PICTURE_JPG,0,0,0,PNULL,0,0};
                uint16 list_index = GUILIST_GetCurItemIndex(MMISET_CALL_VT_REPLACE_IMAGE_LIST_CTRL_ID);
                MMISET_VT_REPLACE_FILE_INFO_T s_replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
                
                //从nv中获得替代文件信息
                MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)s_replace_file_info);
                MMK_CloseWin(win_id);
                MMIPUB_CloseAlertWin();
                select_info.win_id = MMISET_SET_VT_REPLACE_IMAGE_WIN_ID;
                select_info.ticked_file_wstr = s_replace_file_info[list_index].file_name;
                select_info.ticked_file_len = s_replace_file_info[list_index].file_name_len;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);      
            break;
            
        case MSG_CLOSE_WINDOW:
            //        MMK_FreeWinAddData(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : MMIAPISET_OpenVTSeting
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/
#ifdef VT_SUPPORT
PUBLIC void MMIAPISET_OpenVTSeting(void)
{
    if(MMK_IsOpenWin(MMISET_SET_VT_SETTINGS_WIN_ID) )
    {
        MMK_CloseWin(MMISET_SET_VT_SETTINGS_WIN_ID);
    }
    MMK_CreateWin((uint32*)MMISET_SET_VT_WIN_TAB, PNULL);
}
#endif
/*****************************************************************************/
//  Description : 设置VT录制位置
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTRecordPathSetWinMsg(
                                               MMI_WIN_ID_T        win_id,
                                               MMI_MESSAGE_ID_E    msg_id,
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    const MMI_CTRL_ID_T rv_da_dl_id = MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID;
    const MMI_CTRL_ID_T rv_dl_id = MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID;
    const MMI_CTRL_ID_T da_dl_id = MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID;
    GUI_RECT_T          dropdownlist_rect1 = {MMISET_VT_RECORD_RV_DA_DL_LEFT,MMISET_VT_RECORD_RV_DA_DL_TOP,MMISET_VT_RECORD_RV_DA_DL_RIGHT,MMISET_VT_RECORD_RV_DA_DL_BOTTOM};
    GUI_RECT_T          dropdownlist_rect2 = {MMISET_VT_RECORD_RV_DL_LEFT,MMISET_VT_RECORD_RV_DL_TOP,MMISET_VT_RECORD_RV_DL_RIGHT,MMISET_VT_RECORD_RV_DL_BOTTOM};
    GUI_RECT_T          dropdownlist_rect3 = {MMISET_VT_RECORD_DA_DL_LEFT,MMISET_VT_RECORD_DA_DL_TOP,MMISET_VT_RECORD_DA_DL_RIGHT,MMISET_VT_RECORD_DA_DL_BOTTOM};
    static MMIFILE_DEVICE_E s_set_device_arr[MMI_DEVICE_NUM] = {0};
    uint16 cur_rvda_ind = 0;
    uint16 cur_da_ind = 0;
    uint16 cur_rv_ind = 0;
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMISET_VT_RECORD_RV_DA_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMISET_VT_RECORD_RV_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMISET_VT_RECORD_DA_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        GUIDROPDOWNLIST_SetRect(rv_da_dl_id,&dropdownlist_rect1);
        GUIDROPDOWNLIST_SetRect(rv_dl_id,&dropdownlist_rect2);
        GUIDROPDOWNLIST_SetRect(da_dl_id,&dropdownlist_rect3);
        {
            GUI_RECT_T lab_rect = {MMISET_VT_RECORD_RV_DA_LABEL_LEFT,MMISET_VT_RECORD_RV_DA_LABEL_TOP,MMISET_VT_RECORD_RV_DA_LABEL_RIGHT,MMISET_VT_RECORD_RV_DA_LABEL_BOTTOM};
            GUILABEL_SetRect(MMISET_VT_RECORD_RV_DA_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {MMISET_VT_RECORD_RV_LABEL_LEFT,MMISET_VT_RECORD_RV_LABEL_TOP,MMISET_VT_RECORD_RV_LABEL_RIGHT,MMISET_VT_RECORD_RV_LABEL_BOTTOM};
            GUILABEL_SetRect(MMISET_VT_RECORD_RV_LABEL_CTRL_ID, &lab_rect, FALSE);
        }        
        {
            GUI_RECT_T lab_rect = {MMISET_VT_RECORD_DA_LABEL_LEFT,MMISET_VT_RECORD_DA_LABEL_TOP,MMISET_VT_RECORD_DA_LABEL_RIGHT,MMISET_VT_RECORD_DA_LABEL_BOTTOM};
            GUILABEL_SetRect(MMISET_VT_RECORD_DA_LABEL_CTRL_ID, &lab_rect, FALSE);
        } 
        
        SetVTRecordPathSettings(s_set_device_arr);
        MMK_SetAtvCtrl(win_id,rv_da_dl_id);
        break;
        
    case MSG_FULL_PAINT:
        
        break;
            
    case MSG_APP_OK:
    case MSG_CTL_OK:
           
        cur_rvda_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID);
        cur_da_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID);
        cur_rv_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID);
        if(cur_rvda_ind < MMI_DEVICE_NUM && cur_da_ind < MMI_DEVICE_NUM && cur_rv_ind < MMI_DEVICE_NUM)
        {
            SaveVTRecordPathSettings(s_set_device_arr[cur_rvda_ind], s_set_device_arr[cur_da_ind], s_set_device_arr[cur_rv_ind]);
        }
        {
            
            //MMIPUB_OpenAlertSuccessWin( TXT_SET_SUCCESS);
        }
        
        MMK_CloseWin(win_id);
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
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetVTRecordPathSettings(MMIFILE_DEVICE_E *device_table_ptr)
{
    uint16 rvda_cur_index = 0;
    uint16 da_cur_index = 0;
    uint16 rv_cur_index = 0;
    uint32 i = 0;
    //MMI_TEXT_ID_T record_path_text[MMISET_VT_RECORD_DL_NUM] = {TXT_COMMON_UDISK, TXT_COMMON_SD_CARD};
    MMI_STRING_T  item[MMI_DEVICE_NUM] ={0};
    uint32 record_item_num = 0;
    //MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD; 
    MMIFILE_DEVICE_E rvda_user_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E da_user_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E rv_user_dev = MMI_DEVICE_SDCARD;
    
//    if(MMIAPISD_IsSDExist(&fmm_dev))
//    {
//        record_item_num = MMISET_VT_RECORD_DL_NUM;
//    }
//    else
//    {
//        record_item_num = 1;
//     }
    
    GUILABEL_SetTextById(MMISET_VT_RECORD_RV_DA_LABEL_CTRL_ID,TXT_CONECTED_MENU_RECORD_RV_DA,FALSE);
    GUILABEL_SetTextById(MMISET_VT_RECORD_DA_LABEL_CTRL_ID,TXT_CONECTED_MENU_RECORD_DA,FALSE);
    GUILABEL_SetTextById(MMISET_VT_RECORD_RV_LABEL_CTRL_ID,TXT_CONECTED_MENU_RECORD_RV,FALSE);
    
    //get effect item
    rvda_user_dev = MMIAPISET_GetVTRecordRVDAPath();
    da_user_dev = MMIAPISET_GetVTRecordDAPath();
    rv_user_dev = MMIAPISET_GetVTRecordRVPath();
    {
        for (i = MMI_DEVICE_UDISK; i< MMI_DEVICE_NUM; i++)
        {
            if(MMIAPIFMM_GetDeviceTypeStatus(i))
            {
                MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(i),&item[record_item_num]);
                if(device_table_ptr != PNULL)
                {
                      device_table_ptr[record_item_num] = i;
                }
                if(rvda_user_dev == i)
                {
                    rvda_cur_index = record_item_num;
                }
                if(da_user_dev == i)
                {
                    da_cur_index = record_item_num;
                }
                if(rv_user_dev == i)
                {
                    rv_cur_index = record_item_num;
                }
                record_item_num++;
            }
        }
    }
    
    
    //rv&da
    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID,item,record_item_num);
    
    //da
    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID,item,record_item_num);
    
    //rv
    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID,item,record_item_num);
    
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_RECORD_RV_DA_DROPDOWN_CTRL_ID,rvda_cur_index);
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_RECORD_DA_DROPDOWN_CTRL_ID,da_cur_index);
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_RECORD_RV_DROPDOWN_CTRL_ID,rv_cur_index);

#if 1
    if(!MMIAPIFMM_GetDeviceTypeStatus(rvda_user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            MMISET_SetVTRecordRVDAPath(device_table_ptr[rvda_cur_index]);
        }
    }
    if(!MMIAPIFMM_GetDeviceTypeStatus(da_user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            MMISET_SetVTRecordRVDAPath(device_table_ptr[da_cur_index]);
        }
    }
    if(!MMIAPIFMM_GetDeviceTypeStatus(rv_user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            MMISET_SetVTRecordRVDAPath(device_table_ptr[rv_cur_index]);
        }
    }
#else
    if(!MMIAPIFMM_GetDeviceTypeStatus(user_dev))
    {
        if(device_table_ptr != PNULL)
        {
            SaveVTRecordPathSettings(device_table_ptr[cur_index], device_table_ptr[cur_index], device_table_ptr[cur_index]);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : 初始化VT录制位置设置中的控件
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTRecordPathSettings(MMIFILE_DEVICE_E rvda_device, 
                                    MMIFILE_DEVICE_E da_device,
                                    MMIFILE_DEVICE_E rv_device)
{
    MMISET_SetVTRecordRVDAPath(rvda_device);
    MMISET_SetVTRecordDAPath(da_device);
    MMISET_SetVTRecordRVPath(rv_device);
}

/*****************************************************************************/
//  Description : to handle vt one time ring setting win
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTOneTimeRingSettingsWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_VT_ONE_TIME_RING_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        AppendOneTimeRingListItem(ctrl_id);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        SetOneTimeRing(ctrl_id);/*lint !e64*/
        MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
        MMK_CloseWin( win_id );//bug337284
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

/*****************************************************************************/
//  Description : AppendOneTimeRingListItem
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/ 
LOCAL void AppendOneTimeRingListItem(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_selection = 0;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current line
    cur_selection = (uint16)MMIAPISET_GetVTOneTimeRingOption();
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

}  

/*****************************************************************************/
//  Description : SetOneTimeRing
//  Global resource dependence : 
//  Author: samboo
//  Note:
/*****************************************************************************/
LOCAL void SetOneTimeRing(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN cur_selection = (BOOLEAN)GUILIST_GetCurItemIndex(ctrl_id);
    
    MMINV_WRITE(MMINV_SET_VT_ONE_TIME_RING_OPTION, &cur_selection);

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
}
#if 1//bug337002
PUBLIC void SetRemoteShutdownCameraParam(void)
{
    uint16 cur_index = 0;
    MMI_TEXT_ID_T remote_shutdown_camera_text[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] = {TXT_SET_VT_DISPLAY_REMOTE_REPLACE_IMAGE, TXT_SET_VT_DISPLAY_REMOTE_SENDING_IMAGE};
    MMI_STRING_T  remote_shutdown_camera_item ={0};

    GUIFORM_SetStyle(MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
    //remote shut down camera option
    GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,TXT_COMM_REMOTE_CAMERA_OFF,FALSE);

    cur_index = (uint16)MMIAPISET_GetVTRemoteShutdownCameraOption();
    //GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID,remote_shutdown_camera_text[cur_index],FALSE);
    MMI_GetLabelTextByLang(remote_shutdown_camera_text[cur_index],&remote_shutdown_camera_item);
    GUILABEL_SetText(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID, &remote_shutdown_camera_item, FALSE);
}
#endif

/*****************************************************************************/
//  Description : 设置VT本地视频位置
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTLocalVideoSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //GUI_RECT_T              client_rect = MMI_CLIENT_RECT;
    //LOCAL MMI_CTRL_ID_T s_active_ctrl_id = 0;
#if 1//bug337002
    MMI_CTRL_ID_T ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID;
#else
    const MMI_CTRL_ID_T label_ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID;
    const MMI_CTRL_ID_T dropdownlist_ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID;
    GUI_RECT_T label_rect = {MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_LEFT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_TOP,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_RIGHT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_BOTTON};
    GUI_RECT_T dropdownlist_rect = {MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_LEFT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_TOP,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_RIGHT,MMSET_VT_REMOTE_SHUTDOWN_CAMERA_DL_BOTTOM};
#endif

    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
#if 1//bug337002
#if !defined(MMI_GUI_STYLE_TYPICAL) 
        GUIFORM_SetType(MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        SetRemoteShutdownCameraParam();
        MMK_SetAtvCtrl(win_id,MMISET_VT_LOCAL_SETTING_FORM_CTRL_ID);
#else
        GUIFORM_SetStyle(MMISET_VT_LOCAL_SETTING_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUILABEL_SetRect(label_ctrl_id, &label_rect, FALSE);
        GUIDROPDOWNLIST_SetRect(dropdownlist_ctrl_id, &dropdownlist_rect);
        SetVTLocalVideoSettings();
        MMK_SetAtvCtrl(win_id,dropdownlist_ctrl_id);
#endif
        break;

    case MSG_FULL_PAINT:

        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#if 1//bug337002
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }
        
        switch (ctrl_id)
        {
        case MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID:
        case MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID:
            MMISETAPI_CreateRemoteShutdownCameraSetWin();
            break;
        default:
            SCI_TRACE_LOW("HandleVTLocalVideoSetWinMsg ctrl_id = %d",ctrl_id);
            break;

        }

#else
        SaveVTLocalVideoSettings();
        {

            //MMIPUB_OpenAlertSuccessWin( TXT_SET_SUCCESS);
        }

        MMK_CloseWin(win_id);
#endif
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

#if 0//bug337002
/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SetVTLocalVideoSettings(void)
{
    uint16 cur_index = 0;
    uint32 i = 0;
    MMI_TEXT_ID_T remote_shutdown_camera_text[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] = {TXT_SET_VT_DISPLAY_REMOTE_REPLACE_IMAGE, TXT_SET_VT_DISPLAY_REMOTE_SENDING_IMAGE};
    MMI_STRING_T  remote_shutdown_camera_item[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] ={0};
    uint32 remote_shutdown_camera_num = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM;

    //remote shut down camera option
    GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_CTRL_ID,TXT_COMM_REMOTE_CAMERA_OFF,FALSE);

    for (i=0; i<remote_shutdown_camera_num; i++)
    {
        MMI_GetLabelTextByLang(remote_shutdown_camera_text[i],&remote_shutdown_camera_item[i]);
    }

    GUIDROPDOWNLIST_AppendItemArray(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID,remote_shutdown_camera_item,remote_shutdown_camera_num);
    
    cur_index = (uint16)MMIAPISET_GetVTRemoteShutdownCameraOption();
    GUIDROPDOWNLIST_SetCurItemIndex(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID,cur_index);
}

/*****************************************************************************/
//  Description : 初始化VT本地视频设置中的控件
//  Global resource dependence : none
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL void SaveVTLocalVideoSettings(void)
{
    uint16 cur_ind = 0;

    cur_ind = GUIDROPDOWNLIST_GetCurItemIndex(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_DROPDOWN_CTRL_ID);
    MMIAPISET_SetVTRemoteShutdownCameraOption((MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION)cur_ind);
}
#endif
#if 1//bug337002
/*****************************************************************************/
//  Description : to handle set Local Video
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVTLocalVideoSetSelectWinMsg(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T remote_shutdown_camera_text[MMISET_VT_REMOTE_SHUTDOWN_CAMERA_NUM] = {TXT_SET_VT_DISPLAY_REMOTE_REPLACE_IMAGE, TXT_SET_VT_DISPLAY_REMOTE_SENDING_IMAGE};
    MMI_CTRL_ID_T ctrl_id = MMISET_VT_REMOTE_SHUTDOWN_CAMERA_SET_CTRL_ID;
    uint16 cur_index = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
        MMIAPISET_AppendListItemByTextIdExt(remote_shutdown_camera_text[0],TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMIAPISET_AppendListItemByTextIdExt(remote_shutdown_camera_text[1],TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMK_SetAtvCtrl(win_id,ctrl_id);   
        break;
        
    case MSG_FULL_PAINT:
        cur_index = (uint16)MMIAPISET_GetVTRemoteShutdownCameraOption();
        GUILIST_SetSelectedItem( ctrl_id, cur_index, TRUE );      
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);     
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_index = (uint16)GUILIST_GetCurItemIndex(ctrl_id);
        MMIAPISET_SetVTRemoteShutdownCameraOption((MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION)cur_index);

        GUILABEL_SetTextById(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_LABEL_SET_CTRL_ID,remote_shutdown_camera_text[cur_index],FALSE);

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : create screensaver window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMISETAPI_CreateRemoteShutdownCameraSetWin(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_SET_REMOTESHUTDOWNCAMERASET_WIN_TAB,PNULL);
}
#endif

#endif //end VT_SUPPORT
/*****************************************************************************/
//  Description : to select network method
//  Global resource dependence : s_cur_dual_sys
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectNetworkMethodWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    MMI_RESULT_E                recode              =   MMI_RESULT_TRUE;
#ifndef _WIN32 /* lint_lai */
    MN_GMMREG_RAT_E rat =  MN_GMMREG_RAT_GPRS;
    MN_PLMN_T select_plmn = {0,0,2};
#endif 
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    uint16 cur_selection = 0;
    uint16 no_seltction = 0;
    MN_PHONE_SELECT_PLMN_T sel_net_mode ={MN_PHONE_PLMN_SELECT_MANUAL_MODE, 0};/*lint !e785*/
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_NETWORK_METHOD_CTRL_ID,MMISET_NETWORK_SEL_MAX_ITEM, FALSE );//max item 2
        MMISET_AppendSelectNetworkListItem(dual_sys,MMISET_SET_NETWORK_METHOD_CTRL_ID);/*lint !e64*/

        GUILIST_SetOwnSofterKey( MMISET_SET_NETWORK_METHOD_CTRL_ID, TRUE );

        MMK_SetAtvCtrl(win_id, MMISET_SET_NETWORK_METHOD_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_GET_FOCUS:
        MNPHONE_GetPlmnSelectModeEx(dual_sys, &sel_net_mode);/*lint !e64*/
        switch (sel_net_mode.select_mode)
        {
        case MN_PHONE_PLMN_SELECT_AUTO_MODE:
            cur_selection = 0;
            no_seltction = 1;
            break;
            
        case MN_PHONE_PLMN_SELECT_MANUAL_MODE:
            cur_selection = 1;
            no_seltction = 0;
            break;
            
        default:
            //SCI_TRACE_LOW:"HandleSelectNetworkMethodWindow: select network mode is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6687_112_2_18_2_50_29_78,(uint8*)"d",sel_net_mode.select_mode);
            break;
        }

        GUILIST_SetSelectedItem( MMISET_SET_NETWORK_METHOD_CTRL_ID, no_seltction, FALSE );
        GUILIST_SetSelectedItem( MMISET_SET_NETWORK_METHOD_CTRL_ID, cur_selection, TRUE );
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID);
        switch (cur_selection)
        {
        //自动选网
        case 0:
#ifndef _WIN32
            if (MMIAPIPHONE_IsNetworkSelecting(dual_sys))
            {
                MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
                break;
            }
            
            rat = MNPHONE_GetCurrentRATEx(dual_sys);
            MMIAPIPHONE_GetCurPLMN(&select_plmn, dual_sys);
            if (MN_RETURN_SUCCESS != MNPHONE_SelectPLMNEx(dual_sys, FALSE, select_plmn, rat))
            {
                //SCI_TRACE_LOW:"HandleSelectNetworkMethodWindow: MNPHONE_SelectPLMN fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6717_112_2_18_2_50_29_79,(uint8*)"");
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                break;
            }
            
            //进入网络连接界面
            MMISET_OpenNetworkConnectWin(dual_sys,
                                         MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                         TXT_NETWORK_CONNECT,
                                         MMIPUB_SOFTKEY_NONE,
                                         MMISET_HandleSelNetworkConnectWindow);
#else
            GUILIST_SetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID, cur_selection);
            MMK_CloseWin(win_id);
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif 
            break;
            
        //手动选网
        case 1:
#ifndef _WIN32
            if (MMIAPIPHONE_IsNetworkSelecting(dual_sys))
            {
                MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
                break;
            }
            
            // s_current_sel_rat = MNPHONE_GetConfiguredRAT();/*lint !e64*/  //cr149167
            if (MN_RETURN_SUCCESS != MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS_3G_LTE))//@zhaohui,cr106675
            {
                //SCI_TRACE_LOW:"HandleSelectNetworkMethodWindow: MNPHONE_ListPLMN fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6748_112_2_18_2_50_29_80,(uint8*)"");
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                break;
            }
            
            //@fen.xie MS00174570:保存手动搜网前的plmn status
            s_plmn_status_before_manu_search = MMIAPIPHONE_GetPlmnStatus(dual_sys);
            
            //MNPHONE_SelectPLMN(TRUE,select_plmn,MN_GMMREG_RAT_GPRS);    
            //进入网络连接界面
            MMISET_OpenNetworkConnectWin(dual_sys,
                                         MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                         TXT_NETWORK_CONNECT,
                                         MMIPUB_SOFTKEY_ONE,
                                         MMISET_HandleQuitSelNetworkConnectWindow);
#else
            GUILIST_SetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID, cur_selection);
            MMK_CloseWin(win_id);
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif  
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
//  Description : 判断是否满足提示"切换双模后再试"搜网的情况
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPISET_IsPromptRetry(MN_DUAL_SYS_E  dual_sys)
{
    NETWORK_TYPE_E network_type = GetNetworkType(dual_sys);
    
    if (MMK_IsOpenWin(MMISET_SET_NETWORK_METHOD_WIN_ID))
    {
        uint16                          cur_selection       =   0;
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_METHOD_CTRL_ID);
        
        if((PLMN_NO_SERVICE == s_plmn_status_before_manu_search) /*手动搜网前已经是"无网络"*/
            && ((NETWORK_TYPE_TD_E == network_type) || (NETWORK_TYPE_GPRS_E == network_type) || (NETWORK_TYPE_LTE_E == network_type))//单模下
            && (1 == cur_selection)//手动搜网
            )
        {
            return TRUE;
        }
    }
    
    return FALSE;
}
#endif

/*****************************************************************************/
//  Description : 处理自动重拨
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoRedialWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;
    BOOLEAN         is_open         =   FALSE;
    MMI_CTRL_ID_T   ctrl_id         =   MMISET_SET_AUTO_REDIAL_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_AUTO_REDIAL_CTRL_ID, MMISET_AUTODIAL_MAX_ITEM, FALSE );//max item 3
        
        //add items to listbox
        MMIAPISET_AppendListItemByTextId(TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        //MMIAPISET_AppendListItemByTextId(TXT_SET_DELETE_BLACKLIST, STXT_OK, ctrl_id);
        
        if (MMIAPISET_GetIsAutoRedail())
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
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_AUTO_REDIAL_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get the selected item
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        
       // SCI_ASSERT((MMISET_AUTODIAL_MAX_ITEM > cur_selection));
	if(MMISET_AUTODIAL_MAX_ITEM <= cur_selection)
	{
	    //SCI_TRACE_LOW:"mmisetting_assert HandleSetAutoRedialWindow cur_selection %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6869_112_2_18_2_50_29_81,(uint8*)"d", cur_selection);
	    break;
	}         
        switch (cur_selection)
        {
        case 0:
            is_open = TRUE;
            break;
            
        case 1:
            is_open = FALSE;
            break;
            //case 2:
            //    MMIAPICC_ResetBlacklist();
            //    break;
            
        default:
            break;
        }
        MMIAPISET_SetIsAutoRedial(is_open);
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
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

LOCAL  uint32   s_set_speeddial_delay;
#define MMISET_SPEDDIAL_DELAY_NUM   9
/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void MMISET_AppendSpeedDialDelayListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               )
{
    uint32      cur_selection=0;
    
    MN_RETURN_RESULT_E          return_val    =   MN_RETURN_FAILURE;
    MMINV_READ(MMINV_SET_SPEEDIAL_DELAY_TIME, &cur_selection, return_val);

    //add items to listbox
    
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_1_SECOND,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_2_SECONDS,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_3_SECONDS,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_SPEED_DIAL_TIME_4S,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_5_SECONDS,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_6_SECONDS,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_SPEED_DIAL_TIME_7S,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_SPEED_DIAL_TIME_8S,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection/1000, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection/1000);
    
}

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
PUBLIC void MMISET_SetSpeedDialDelay(uint32 time)
{
    s_set_speeddial_delay = time;
}

/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
PUBLIC uint32 MMIAPISET_GetSpeedDialDelay()
{
    return s_set_speeddial_delay;
}


/************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
LOCAL void SetSpeedDialDelay(
                              MMI_CTRL_ID_T     ctrl_id
                              )
{
    uint16   cur_selection = 0;
    uint32   delay_time[] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    //SCI_ASSERT((0 <= cur_selection) && (MMISET_SPEDDIAL_DELAY_NUM > cur_selection));
    if(MMISET_SPEDDIAL_DELAY_NUM <= cur_selection)
    {
        //SCI_TRACE_LOW:"mmisetting_assert SetSpeedDialDelay cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_6974_112_2_18_2_50_30_82,(uint8*)"d", cur_selection);
        return;
    }     
    s_set_speeddial_delay = delay_time[cur_selection];

    //  SetBackLightInfo(backlight_info);
    MMINV_WRITE(MMINV_SET_SPEEDIAL_DELAY_TIME,&s_set_speeddial_delay);
}

/*****************************************************************************/
//  Description : 延时拨号处理
//  Global resource dependence : 
//  Author:yiwen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSpeedDialWindow(
                                                  MMI_WIN_ID_T    win_id, 
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_SPEED_DIAL_CTRL_ID,MMISET_SPEDDIAL_DELAY_NUM, FALSE );//max item 2
        MMISET_AppendSpeedDialDelayListItem(MMISET_SET_SPEED_DIAL_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMISET_SET_SPEED_DIAL_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        SetSpeedDialDelay(MMISET_SET_SPEED_DIAL_CTRL_ID);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
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

#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : SetVoiceReportParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetVoiceReportParam(void)
{
    
    uint32 loop = 0;
    uint16 cur_selection = 0;
    BOOLEAN                     is_tts_open = FALSE;
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_VOICE_REPORT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_CALL_TTS, &label_name);
    GUILABEL_SetText(MMISET_VOICE_REPORT_LABEL_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_VOICE_REPORT_EDIT_CTRL_ID, text_id[loop]);
    }

    //get current tts setting info
    is_tts_open = MMIAPISET_GetTTSInfo();
    //the status is open
    if (is_tts_open)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
	GUISETLIST_SetTitleTextId(MMISET_VOICE_REPORT_EDIT_CTRL_ID,TXT_SET_CALL_TTS);
    GUISETLIST_SetCurIndex(MMISET_VOICE_REPORT_EDIT_CTRL_ID, cur_selection);

    GUISETLIST_SetCtrlState(MMISET_VOICE_REPORT_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574

}
#endif

#ifdef TTS_SUPPORT 
/*****************************************************************************/
//  Description : SaveVoiceReportParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveVoiceReportParam(void)
{
    uint16      cur_selection = 0;
    BOOLEAN     is_tts_open = FALSE;
    
    //get the selected item
    cur_selection = GUISETLIST_GetCurIndex(MMISET_VOICE_REPORT_EDIT_CTRL_ID);
    
    // set the global virable
    if ( 0 == cur_selection)
    {
        is_tts_open = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_SetTTSInfo cur_selection = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_7090_112_2_18_2_50_30_83,(uint8*)"d",cur_selection);
        is_tts_open = FALSE;
    }
    
    //write the status of TTS into NV
    MMINV_WRITE(MMINV_SET_TTS_ON_OFF,&is_tts_open);
}
#endif

/*****************************************************************************/
//  Description : SaveVibrateTipParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveVibrateTipParam(void)
{
    uint16      cur_selection   =   0;
    BOOLEAN     is_open         =   FALSE;
    
    //get the selected item
    cur_selection = GUISETLIST_GetCurIndex(MMISET_VIBRATE_TIP_EDIT_CTRL_ID);
    
    switch (cur_selection)
    {
    case 0:
        is_open = TRUE;
        break;
        
    case 1:
        is_open = FALSE;
        break;
        
    default:
        break;
    }
    MMISET_SetIsConnectPrompt(is_open);
}

/*****************************************************************************/
//  Description : SaveAuTORedialParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveAuTORedialParam(void)
{
    uint16 cur_selection = 0;
    BOOLEAN     is_open         =   FALSE;
    cur_selection = GUISETLIST_GetCurIndex(MMISET_AUTO_CONNECT_EDIT_CTRL_ID);
    
    switch (cur_selection)
    {
    case 0:
        is_open = TRUE;
        break;
        
    case 1:
        is_open = FALSE;
        break;
        
    default:
        break;
    }
    MMIAPISET_SetIsAutoRedial(is_open); 
}

#ifdef MV_SUPPORT 
/*****************************************************************************/
//  Description : SaveMagicParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SaveMagicParam(void)
{
    uint16 cur_selection = 0;
    cur_selection = GUISETLIST_GetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID);
    if(cur_selection == 0)
    {
     MMIAPISET_SetIsMagicVoice(TRUE);
    }
    else
    {
        MMIAPISET_SetIsMagicVoice(FALSE);
    }
}
#endif




/*****************************************************************************/
//  Description : init call setting other control
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
#ifdef MV_SUPPORT 
LOCAL void SetMagicParam(void)
{
    uint32 loop = 0;
    MMI_STRING_T label_name  = {0};
    uint16 cur_selection = 0;
    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };
#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_MAGIC_VOICE_LIST_CTRL_ID,ALIGN_LEFT);
#endif

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_CC_MAGICVOICE, &label_name);
    GUILABEL_SetText(MMISET_MAGIC_VOICE_LABEL_CTRL_ID, &label_name, FALSE);

#if !defined MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_MAGIC_VOICE_LIST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    MMI_GetLabelTextByLang(TXT_CC_MAGICVOICE_PARAM_SET, &label_name);
    GUILABEL_SetText(MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID, &label_name, FALSE);
        
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_MAGIC_VOICE_LIST_CTRL_ID, text_id[loop]);
    }
    
#if !defined(MMI_PDA_SUPPORT) 
    if(MMIAPISET_GetIsMagicVoice())
    {
        cur_selection = 0;
        GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                GUIFORM_CHILD_DISP_NORMAL
                );  
    }
    else
    {
        cur_selection = 1;
        GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                GUIFORM_CHILD_DISP_GRAY
                );          
    }
	GUISETLIST_SetTitleTextId(MMISET_MAGIC_VOICE_LIST_CTRL_ID,TXT_CC_MAGICVOICE_PARAM_SET);
    GUISETLIST_SetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_MAGIC_VOICE_LIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574
#endif
}
#endif

/*****************************************************************************/
//  Description : SetSetCallOtherLabelParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetSetCallOtherLabelParam(void)
{
    MMI_STRING_T label_name  = {0};
    
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_IP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_IP_NUMBER, &label_name);
    GUILABEL_SetText(MMISET_IP_KEY_LABEL_CTRL_ID, &label_name, FALSE);
    
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_ANSWER_MODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_ANYKEY_REJECT_MODE, &label_name);
    GUILABEL_SetText(MMISET_ANSWER_MODE_LABEL_CTRL_ID, &label_name, FALSE);        
#endif
}

/*****************************************************************************/
//  Description : SetAutoRedailParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetAutoRedailParam(void)
{

    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_AUTO_CONNECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_AUTO_REDAIL, &label_name);
    GUILABEL_SetText(MMISET_AUTO_CONNECT_LABEL_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_AUTO_CONNECT_EDIT_CTRL_ID, text_id[loop]);
    }

    if (MMIAPISET_GetIsAutoRedail())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    GUISETLIST_SetTitleTextId(MMISET_AUTO_CONNECT_EDIT_CTRL_ID,TXT_SET_AUTO_REDAIL);
    GUISETLIST_SetCurIndex(MMISET_AUTO_CONNECT_EDIT_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_AUTO_CONNECT_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574

}

/*****************************************************************************/
//  Description : SetVibrateTipParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetVibrateTipParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
    TXT_OPEN,
    TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_VIBRATE_TIP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_CONNECT_PROMPT, &label_name);
    GUILABEL_SetText(MMISET_VIBRATE_TIP_LABEL_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_VIBRATE_TIP_EDIT_CTRL_ID, text_id[loop]);
    }

    if(MMIAPISET_GetIsConnectPrompt())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    GUISETLIST_SetTitleTextId(MMISET_VIBRATE_TIP_EDIT_CTRL_ID,TXT_SET_CONNECT_PROMPT);
    GUISETLIST_SetCurIndex(MMISET_VIBRATE_TIP_EDIT_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_VIBRATE_TIP_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//for cr226574
}


/*****************************************************************************/
//  Description : SetMinuteReminderParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetMinuteReminderParam(void)
{
    MMI_STRING_T label_name  = {0};
    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_MINUTE_REMINDER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_CALL_TIME_REMINDER, &label_name);
    GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_CTRL_ID, &label_name, FALSE);
    switch(MMIAPISET_GetMinuteReminderTime())
    {
    case CC_CALL_REMINDER_TIME_OFF:
        MMI_GetLabelTextByLang(TXT_CLOSE, &label_name);
        break;
        
    case CC_CALL_REMINDER_TIME_50S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_50S, &label_name);
        break;
        
    case CC_CALL_REMINDER_TIME_55S:
       MMI_GetLabelTextByLang(TXT_SET_TIME_55S, &label_name);
        break;
        
    case CC_CALL_REMINDER_TIME_60S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_60S, &label_name);
        break;
        
    default :
         MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &label_name);
        break;
    }
    
    GUILABEL_SetText(MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID, &label_name, FALSE);
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitPdaCallOtherSettingsCtrl
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallOtherSettingsCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_OTHER_SETTING_CTRL_ID;
    MMI_STRING_T text_str  = {0};
    MMI_STRING_T text_str_null  = {0};
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    MMISET_ANYKEY_REJECT_SET_T  mode_info = {0};
#endif
#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
    BOOLEAN is_fly_mode_on = FALSE;
    BOOLEAN is_flymode_openphone = FALSE;
#endif
    uint8   list_menu_count =5;
    wchar temp_wchar[12 + 1]={0};
    
#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
    list_menu_count = list_menu_count+2;
#endif

#ifdef MMI_AUTO_RECORD_SUPPORT
    list_menu_count = list_menu_count+3;
#endif

#ifdef MMI_IP_CALL_SUPPORT
    list_menu_count += 1;
#endif

    GUILIST_SetMaxItem(ctrl_id, list_menu_count, FALSE );
    
#ifdef MMI_IP_CALL_SUPPORT
    //ip setting
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_IP_NUMBER,text_str,IMAGE_NULL,MMISET_IP_KEY_LABEL_CTRL_ID);
#endif

    //call time reminder
    switch(MMIAPISET_GetMinuteReminderTime())
    {
    case CC_CALL_REMINDER_TIME_OFF:
        MMI_GetLabelTextByLang(TXT_CLOSE, &text_str);
        break;
        
    case CC_CALL_REMINDER_TIME_50S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_50S, &text_str);
        break;
        
    case CC_CALL_REMINDER_TIME_55S:
       MMI_GetLabelTextByLang(TXT_SET_TIME_55S, &text_str);
        break;
        
    case CC_CALL_REMINDER_TIME_60S:
        MMI_GetLabelTextByLang(TXT_SET_TIME_60S, &text_str);
        break;
        
    default :
        {
            char temp_char[4 + 1] = {0};
            MMI_STRING_T second_str = {0};
            uint32 temp_char_len = 0;
            text_str.wstr_ptr = temp_wchar;
            
            sprintf((char*)temp_char, "%d ", MMIAPISET_GetMinuteReminderTime());
            temp_char_len = strlen((char*)temp_char);
            text_str.wstr_len = MIN((sizeof(temp_wchar)/sizeof(wchar) - 1), temp_char_len);
            MMI_STRNTOWSTR(text_str.wstr_ptr, text_str.wstr_len, (uint8 *)temp_char, text_str.wstr_len, text_str.wstr_len);

            MMI_GetLabelTextByLang(TXT_SECOND, &second_str);

            text_str.wstr_len += MIN((sizeof(temp_wchar)/sizeof(wchar) - text_str.wstr_len - 1), second_str.wstr_len);
            MMIAPICOM_Wstrncat(text_str.wstr_ptr, second_str.wstr_ptr, MIN((sizeof(temp_wchar)/sizeof(wchar) - text_str.wstr_len - 1), second_str.wstr_len));
        }
        break;
    } 
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_CALL_TIME_REMINDER,text_str,IMAGE_NULL,MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID);

    //auto redial
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_AUTO_REDAIL,text_str_null,MMIAPISET_GetIsAutoRedail()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_AUTO_CONNECT_EDIT_CTRL_ID);

    //vibrate tip
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_CONNECT_PROMPT,text_str_null,MMIAPISET_GetIsConnectPrompt()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_VIBRATE_TIP_EDIT_CTRL_ID);

    //call reject mode
#if 0
	//pda remove this function
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_ANYKEY_REJECT_MODE,text_str_null,IMAGE_COMMON_RIGHT_ARROW,MMISET_ANSWER_MODE_LABEL_CTRL_ID);
#endif

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    mode_info = MMIAPISET_GetAnykeyRejectMode();
    AppendPdaSettingsItem2Line(ctrl_id, TXT_SET_SMS_REPLY, text_str_null, mode_info.is_sms_reply_open?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON, MMISET_ANSWER_MODE_LABEL_CTRL_ID);
#endif

#ifdef TTS_SUPPORT
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_CALL_TTS,text_str_null,MMIAPISET_GetTTSInfo()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_VOICE_REPORT_EDIT_CTRL_ID);
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
    is_fly_mode_on = MMIAPISET_GetFlyMode();

    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_FLY_MODE,text_str_null,is_fly_mode_on?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID);

    is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();

    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &text_str);

    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_FLY_MODE,text_str,is_flymode_openphone?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID);
#endif
#ifdef MMI_AUTO_RECORD_SUPPORT
    AppendPdaSettingsItem2Line(ctrl_id,TXT_SET_AUTO_RECORD,text_str_null,MMIAPISET_GetAutoRecordSetting()?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON,MMISET_CALL_AUTO_RECORD_CTRL_ID);
#endif
}
 
/*****************************************************************************/
//  Description : 通话设置中的others的设置窗口
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallSetOtherWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_OTHER_SETTING_CTRL_ID;
    MMI_MENU_ID_T               menu_id     =   0;
    uint16 index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            InitPdaCallOtherSettingsCtrl();
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

    case MSG_GET_FOCUS:
        index = GUILIST_GetCurItemIndex(ctrl_id); 
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaCallOtherSettingsCtrl();
        GUILIST_SetCurItemIndex(ctrl_id, index); 
        break;
    case MSG_FULL_PAINT:
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);

        switch (menu_id)
        {
#ifdef MMI_IP_CALL_SUPPORT
        case MMISET_IP_KEY_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            {
                uint16 sim_sys = MN_DUAL_SYS_1;
                uint32 sim_num = 0;

                sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   

                if (0 == sim_num)
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if (1 == sim_num)
                {
                    MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)sim_sys);
                }
                else
                {
                    MMIAPISET_OpenUssdSelectWin(MMISET_IP_CALL);
                }
            }
#else
            MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
#endif
            break;
#endif

        case MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID:
            MMK_CreateWin((uint32*)MMISET_SET_MINUTE_REMINDER_WIN_TAB,PNULL);
            break;

        case MMISET_AUTO_CONNECT_EDIT_CTRL_ID:
			{
                MMIAPISET_SetIsAutoRedial((BOOLEAN)!MMIAPISET_GetIsAutoRedail());
                SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetIsAutoRedail(),index);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
			break;
            
        case MMISET_VIBRATE_TIP_EDIT_CTRL_ID:
			{
                MMISET_SetIsConnectPrompt((BOOLEAN)!MMIAPISET_GetIsConnectPrompt());
                SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetIsConnectPrompt(),index);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}
            break;
            
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        case MMISET_ANSWER_MODE_LABEL_CTRL_ID:
            {
                MMISET_ANYKEY_REJECT_SET_T  mode_info = {0};
                BOOLEAN is_sms_reply_open = FALSE;
                
                mode_info = MMIAPISET_GetAnykeyRejectMode();
                is_sms_reply_open = !(mode_info.is_sms_reply_open);
                SetCallSetOthersItemCheck(ctrl_id, is_sms_reply_open, index);
                mode_info.is_sms_reply_open = mode_info.is_sms_reply_open ? FALSE:TRUE;
                MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&mode_info);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
            
#ifdef MV_SUPPORT
        case MMISET_MAGIC_VOICE_LABEL_CTRL_ID:
            MMIAPISET_SetIsMagicVoice((BOOLEAN)!MMIAPISET_GetIsMagicVoice());
            SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetIsMagicVoice(),index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
            

        case MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID:
            MMIAPICC_OpenMagicVoiceSetMenu();
            break;
#endif
#ifdef TTS_SUPPORT
        case MMISET_VOICE_REPORT_EDIT_CTRL_ID:
            MMISET_SetTTS((BOOLEAN)!MMIAPISET_GetTTSInfo());
            SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetTTSInfo(),index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
        case MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID:
            {
                uint32 i  = 0;
                BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();

                if (!is_fly_mode_on)
                {
                    uint32 dual_sys =0;
                    BOOLEAN is_ps_ready = TRUE;
                    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                    {
                        if ((e_dualsys_setting & (0x01 << dual_sys))
                            && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                        {
                            is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                            if (!is_ps_ready)
                            {
                                break;
                            }
                        }
                    }

                    if (is_ps_ready)
                    {
                        if (MMISET_GetIsQueryOpenFlyMode())
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                            break;
                        }
                        else
                        {
                            is_fly_mode_on = TRUE;

                            MMIAPISET_SetFlyMode(is_fly_mode_on);

                            MMIAPIPHONE_DeactivePsAccordingToSetting();
                            SetCallSetOthersItemCheck(ctrl_id,is_fly_mode_on,index);
                            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        
                        break;
                    }
                }
                else
                {
                    BOOLEAN is_ps_deactive = TRUE;

                    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                        {
                            is_ps_deactive = FALSE;
                            break;
                        }
                    }
            
                    if (is_ps_deactive)
                    {
                        is_fly_mode_on = FALSE;
                    
                        MMIAPISET_SetFlyMode(is_fly_mode_on);
                    
                        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                    
                        // 激活协议栈
                        MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                        MMIAPIMMS_RestartAutodownMMS();
                        SetCallSetOthersItemCheck(ctrl_id,is_fly_mode_on,index);
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        break;
                    }
                }
            }
            break;

        case MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID:
            {
                BOOLEAN is_flymode_openphone = FALSE;
                is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
                SetCallSetOthersItemCheck(ctrl_id,!is_flymode_openphone,index);
                is_flymode_openphone = !is_flymode_openphone;
                MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
			
#ifdef MMI_AUTO_RECORD_SUPPORT
        case MMISET_CALL_AUTO_RECORD_CTRL_ID:
            MMIAPISET_SetAutoRecordSetting((BOOLEAN)!MMIAPISET_GetAutoRecordSetting());
            SetCallSetOthersItemCheck(ctrl_id,MMIAPISET_GetAutoRecordSetting(),index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;

#endif

        default:
            break;

        }
        break;

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
     case MSG_PROMPTWIN_OK:
        {
            BOOLEAN is_fly_mode_on = TRUE;

            MMIPUB_CloseQuerytWin(PNULL);
            
            MMISET_StopPSApplication();
            
            MMIAPISET_SetFlyMode(is_fly_mode_on);
            
            MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);

#ifdef DCD_SUPPORT                      
            MMIAPIDCD_HandFlyModeSetChange();
#endif
            
            MMIAPIPHONE_DeactivePsAccordingToSetting();
            
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
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
    return recode;
}
#else

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
/*****************************************************************************/
//  Description : SetFlyModeParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyModeParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
        TXT_OPEN,
        TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &label_name);
    GUILABEL_SetText(MMISET_FLY_MODE_SELECT_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID, text_id[loop]);
    }

    //the status is open
    if (is_fly_mode_on)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUISETLIST_SetTitleTextId(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID,TXT_SET_FLY_MODE);
    GUISETLIST_SetCurIndex(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID, cur_selection);

    GUISETLIST_SetCtrlState(MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}

/*****************************************************************************/
//  Description : SetFlyModePowerOnParam
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyModePowerOnParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    BOOLEAN is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
    MMI_STRING_T label_name  = {0};

    uint32 text_id[2]=
    {
        TXT_OPEN,
        TXT_CLOSE,
    };

    GUIFORM_SetStyle(MMISET_CALL_OTHER_SET_FLY_MODE_SELECT_POWER_ON_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &label_name);
    GUILABEL_SetText(MMISET_FLY_MODE_SELECT_ON_POWER_CTRL_ID, &label_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID, text_id[loop]);
    }

    //the status is open
    if (is_flymode_openphone)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUISETLIST_SetTitleTextId(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID,TXT_SET_SELECT_ON_POWER);
    GUISETLIST_SetCurIndex(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID, cur_selection);

    GUISETLIST_SetCtrlState(MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCallSetOtherWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_IP_KEY_LABEL_CTRL_ID;
/*#if !defined(MMI_MULTI_SIM_SYS_SINGLE)    
    MMISET_SIM_SELECT_TYPE_E    select_type =   0;
#endif*/
    
    MMIMAINMENU_TYPE_E    menu_style = MMIAPISET_GetMainmenuStyle();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetSetCallOtherLabelParam();
        SetMinuteReminderParam();
        SetAutoRedailParam();
        SetVibrateTipParam();
#ifdef MV_SUPPORT 
        if(
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
            MMIMAINMENU_GO_SLIDE_E == menu_style
#endif
#ifdef MENU_SWEETS_SUPPORT
            || MMIMAINMENU_SWEETS_E  == menu_style
#endif
            )
        {
            GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_FORM_CTRL_ID,
            MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
            GUIFORM_CHILD_DISP_HIDE);
        }
        else
        {
            GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_FORM_CTRL_ID,
            MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
            GUIFORM_CHILD_DISP_NORMAL);
            SetMagicParam();
        }
#endif
#ifdef TTS_SUPPORT
        SetVoiceReportParam();
#endif
        
#if 0//def TOUCH_PANEL_SUPPORT
        SetVibrateTPParam();
#endif
#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
        SetFlyModeParam();
        SetFlyModePowerOnParam();
#endif
#ifdef MMI_IP_CALL_SUPPORT
        MMK_SetAtvCtrl(win_id, MMISET_IP_KEY_LABEL_CTRL_ID);
#endif
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
        break;

    case MSG_GET_FOCUS:
        SetMinuteReminderParam();
        break;
        
    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        switch(ctrl_id)
        {
#ifdef MV_SUPPORT 
        case MMISET_MAGIC_VOICE_LIST_CTRL_ID:
            if(GUISETLIST_GetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID) == 0)
            {

                GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                        MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                        GUIFORM_CHILD_DISP_NORMAL
                        );
            }
            else
            {
                GUIFORM_SetChildDisplay(MMISET_CALL_OTHER_SET_MAGIC_VOICE_FORM_CTRL_ID,
                        MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID,
                        GUIFORM_CHILD_DISP_GRAY
                        );  
            }
            SaveMagicParam();
            break;
#endif
        case MMISET_AUTO_CONNECT_EDIT_CTRL_ID:
            SaveAuTORedialParam();
            break;

        case MMISET_VIBRATE_TIP_EDIT_CTRL_ID:
            SaveVibrateTipParam();
            break;

#ifdef TTS_SUPPORT
        case MMISET_VOICE_REPORT_EDIT_CTRL_ID:
            SaveVoiceReportParam();
            break;
#endif
#if 0//def TOUCH_PANEL_SUPPORT
        case MMISET_VIBRATE_TP_EDIT_CTRL_ID:
            SaveVibrateTPParam();
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
        case MMISET_FLY_MODE_SELECT_EDIT_CTRL_ID:
            {
                uint32 i  = 0;
                BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();
            
                if (!is_fly_mode_on)
                {
                    uint32 dual_sys =0;
                    BOOLEAN is_ps_ready = TRUE;
                    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                    {
                        if ((e_dualsys_setting & (0x01 << dual_sys))
                            && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                        {
                            is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                            if (!is_ps_ready)
                            {
                                break;
                            }
                        }
                    }

                    if (is_ps_ready)
                    {
                        if (MMISET_GetIsQueryOpenFlyMode())
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                            break;
                        }
                        else
                        {
                            is_fly_mode_on = TRUE;

                            MMIAPISET_SetFlyMode(is_fly_mode_on);

                            MMIAPIPHONE_DeactivePsAccordingToSetting();
                            
                            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        
                        break;
                    }
                }
                else
                {
                    BOOLEAN is_ps_deactive = TRUE;

                    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                        {
                            is_ps_deactive = FALSE;
                            break;
                        }
                    }
            
                    if (is_ps_deactive)
                    {
                        is_fly_mode_on = FALSE;
                    
                        MMIAPISET_SetFlyMode(is_fly_mode_on);
                    
                        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                    
                        // 激活协议栈
                        MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                        MMIAPIMMS_RestartAutodownMMS();
                    
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        break;
                    }
                }
            }
            break;

        case MMISET_FLY_MODE_SELECT_ON_POWER_EDIT_CTRL_ID:
            {
                BOOLEAN is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
            
                is_flymode_openphone = !is_flymode_openphone;

                MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);

                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
        default:
            break;
        }
        break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        if(msg_id==MSG_CTL_PENOK)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        switch (ctrl_id)
        {
#ifdef MMI_IP_CALL_SUPPORT
        case MMISET_IP_KEY_LABEL_CTRL_ID:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            {
                uint16 sim_sys = MN_DUAL_SYS_1;
                uint32 sim_num = 0;

                sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);   

                if (0 == sim_num)
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if (1 == sim_num)
                {
                    MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)sim_sys);
                }
                else
                {
                    MMIAPISET_OpenUssdSelectWin(MMISET_IP_CALL);
                }
            }
#else
            MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
#endif
            break;
#endif

        case MMISET_CALL_MINUTE_REMINDER_LABEL_EDIT_CTRL_ID:
            MMK_CreateWin((uint32*)MMISET_SET_MINUTE_REMINDER_WIN_TAB,PNULL);
            break;

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        case MMISET_ANSWER_MODE_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_ANYKEY_REJECT_MODE_WIN_TAB,PNULL);
            break;
#endif

#ifdef MV_SUPPORT
        case MMISET_MAGIC_VOICE_EDIT_LABEL_CTRL_ID:
#if !defined(MMI_PDA_SUPPORT) 
            if(GUISETLIST_GetCurIndex(MMISET_MAGIC_VOICE_LIST_CTRL_ID) == 0)
#endif
            {
                MMIAPICC_OpenMagicVoiceSetMenu();
            }
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MULTI_SIM_GSM_CTA_SUPPORT)
     case MSG_PROMPTWIN_OK:
        {
            BOOLEAN is_fly_mode_on = TRUE;

            MMIPUB_CloseQuerytWin(PNULL);
            
            MMISET_StopPSApplication();
            
            MMIAPISET_SetFlyMode(is_fly_mode_on);
            
            MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);

#ifdef DCD_SUPPORT                      
            MMIAPIDCD_HandFlyModeSetChange();
#endif
            
            MMIAPIPHONE_DeactivePsAccordingToSetting();
            
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
#endif

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
// Description : to handle set fly mode window msg
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeWindow(
                                        MMI_WIN_ID_T      win_id,
                                        MMI_MESSAGE_ID_E  msg_id,
                                        DPARAM            param
                                        )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on  =   FALSE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_FLY_MODE_LIST_CTRL_ID;
    uint16              cur_selection   =   0;
    uint32  i = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_fly_mode_on = MMIAPISET_GetFlyMode();
        GUILIST_SetMaxItem(MMISET_FLY_MODE_LIST_CTRL_ID,2, FALSE );
        
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_fly_mode_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_fly_mode_on);
        MMK_SetAtvCtrl(win_id,MMISET_FLY_MODE_LIST_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        is_fly_mode_on = MMIAPISET_GetFlyMode();
        
        if (is_fly_mode_on != cur_selection)
        {
            if(0 == cur_selection)
            {
                BOOLEAN is_ps_deactive = TRUE;

                for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                    {
                        is_ps_deactive = FALSE;
                        break;
                    }
                }
                if (is_ps_deactive)
                {
                    is_fly_mode_on = (BOOLEAN)cur_selection;
                    
                    MMIAPISET_SetFlyMode(is_fly_mode_on);
                    
                    MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                    
                    // 激活协议栈
                    MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                    MMIAPIMMS_RestartAutodownMMS();
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    MMK_CloseWin(win_id);
                    break;
                }
            }
            else
            {
                uint32 dual_sys =MN_DUAL_SYS_1;
                BOOLEAN is_ps_ready = TRUE;
                MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                {
                    if ((e_dualsys_setting & (0x01 << dual_sys))
                        && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                    {
                        is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                        if (!is_ps_ready)
                        {
                            break;
                        }
                    }
                }
            
                if (is_ps_ready)
                {
                    if (MMISET_GetIsQueryOpenFlyMode())
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        break;
                    }
                    else
                    {
                        is_fly_mode_on = (BOOLEAN)cur_selection;
                        
                        MMIAPISET_SetFlyMode(is_fly_mode_on);

                        MMIAPIPHONE_DeactivePsAccordingToSetting();
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    MMK_CloseWin(win_id);
                    break;
                }
            }
        }
        
        //success 
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        
        MMISET_StopPSApplication();
        
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        is_fly_mode_on = (BOOLEAN)cur_selection;
        
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        
        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);


#ifdef DCD_SUPPORT                      
        MMIAPIDCD_HandFlyModeSetChange();
#endif
        
        MMIAPIPHONE_DeactivePsAccordingToSetting();
        
        //success 
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}


/*****************************************************************************/
// Description : to handle set fly mode window msg
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeSetWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E  msg_id,
                                           DPARAM            param
                                           )
{
    MMI_RESULT_E        recode                  =   MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on          =   FALSE;
    MMI_CTRL_ID_T       ctrl_id                 =   MMISET_FLY_MODE_SET_CTRL_ID;
    static uint16       cur_index               =   0;   
    BOOLEAN             is_flymode_openphone    =   FALSE;   
    uint32  i = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        cur_index = 0;
        break;
        
    case MSG_FULL_PAINT:
        AppendFlyModeSetListItem(ctrl_id);  
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB: 
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        switch(cur_index) 
        {
        case 0:
            is_fly_mode_on = MMIAPISET_GetFlyMode();
            
            if (!is_fly_mode_on)
            {
                uint32 dual_sys =0;
                BOOLEAN is_ps_ready = TRUE;
                MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                {
                    if ((e_dualsys_setting & (0x01 << dual_sys))
                        && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                    {
                        is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                        if (!is_ps_ready)
                        {
                            break;
                        }
                    }
                }

                if (is_ps_ready)
                {
                    if (MMISET_GetIsQueryOpenFlyMode())
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        break;
                    }
                    else
                    {
                        is_fly_mode_on = TRUE;
                        MMIAPISET_SetFlyMode(is_fly_mode_on);

                        MMIAPIPHONE_DeactivePsAccordingToSetting();
                        
                        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    break;
                }
            }
            else
            {
                BOOLEAN is_ps_deactive = TRUE;
                for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                    {
                        is_ps_deactive = FALSE;
                        break;
                    }
                }
            
                if (is_ps_deactive)
                {
                    is_fly_mode_on = FALSE;
                    
                    MMIAPISET_SetFlyMode(is_fly_mode_on);
                    
                    MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                    
                    // 激活协议栈
                    MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                    MMIAPIMMS_RestartAutodownMMS();
                    
                    MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                    break;
                }
                break;

            }
            break;
        case 1:   
            is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
            
            is_flymode_openphone = !is_flymode_openphone;
            MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
            
        default:
            break; 
        }
        break;
        
        case MSG_APP_CANCEL:    
        case MSG_CTL_CANCEL:              
            MMK_CloseWin( win_id );
            break;
            
        case MSG_PROMPTWIN_OK:
            MMIPUB_CloseQuerytWin(PNULL);
            
            MMISET_StopPSApplication();
            
            is_fly_mode_on = TRUE;
            MMIAPISET_SetFlyMode(is_fly_mode_on);
            
            MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);


#ifdef DCD_SUPPORT                      
            MMIAPIDCD_HandFlyModeSetChange();
#endif
            
            MMIAPIPHONE_DeactivePsAccordingToSetting();
            
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
            
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

#if defined (MODEM_SUPPORT_SPRD)
LOCAL NETWORK_TYPE_E GetNetworkTypeBySelectIndex(uint16 cur_sel_item)
{
    NETWORK_TYPE_E network_type = 0;

    SCI_TRACE_LOW("[MMISET] GetNetworkTypeBySelectIndex cur_sel_item = %d", cur_sel_item);
    switch (cur_sel_item)
    {
        case 0:
            network_type = MN_PHONE_POWER_TD_LTE_FIVE_MODE_ONE_ACTIVE;
            break;

        case 1:
            network_type = MN_PHONE_POWER_WCDMA_AND_GSM;
            break;

        case 2:
            network_type = MN_PHONE_POWER_GSM_ONLY;
            break;

        default:
            break;
    }

    return network_type;
}
#endif
#ifdef MODEM_SUPPORT_SPRD
LOCAL uint16 GetSelectIndexByNetworkType(MN_PHONE_POWER_TEST_MODE_IND network_type)
{
    uint16 cur_sel_item = 0;

    SCI_TRACE_LOW("[MMISET] GetSelectIndexByNetworkType network_type = %d", network_type);
    switch (network_type)
    {
        case MN_PHONE_POWER_TD_LTE_FIVE_MODE_ONE_ACTIVE:
            cur_sel_item = 0;
            break;

        case MN_PHONE_POWER_WCDMA_AND_GSM:
            cur_sel_item = 1;
            break;

        case MN_PHONE_POWER_GSM_ONLY:
            cur_sel_item = 2;
            break;

        default:
            break;
    }

    return cur_sel_item;
}
#endif
PUBLIC void MMIAPIENG_SHOWVOLTE_WINDOW(void)
{
	#ifdef MMI_VOLTE_SUPPORT
#ifdef MODEM_SUPPORT_SPRD
    MMIAPISET_OpenVoLTEWin();
#endif
	#endif
}

#if defined (MMI_VOLTE_SUPPORT)
LOCAL void HandleWaitWinTimer(uint8 timer_id, uint32 param)
{
    if (wait_timer_id == timer_id)
    {
        if (MMK_IsOpenWin( MAIN_WAIT_WIN_ID ))
        {
            MMIDEFAULT_SetBackLight(TRUE);
            MMK_CloseWin(MAIN_WAIT_WIN_ID);
        }
        wait_timer_id = 0;		   
    }
}

LOCAL void ShowWaitingWin(void)
{
    MMI_STRING_T wait_text = {0};

    if (MMK_IsOpenWin(MAIN_WAIT_WIN_ID))
    {
        MMK_CloseWin(MAIN_WAIT_WIN_ID);
    }
    MMI_GetLabelTextByLang(TXT_NOTE_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1, &wait_text, PNULL, PNULL, MAIN_WAIT_WIN_ID, IMAGE_NULL,
                       ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, NULL);
    wait_timer_id = MMK_CreateTimerCallback(1000, HandleWaitWinTimer, PNULL, FALSE);	

    return;
}

//PUBLIC BOOLEAN MMIAPICC_IsInState(CC_CALL_STATE_FOR_USING_E call_state);

LOCAL MMI_RESULT_E  HandleSetVoLTEWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{

    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    GUILIST_ITEM_T      item_t          =   {0};
    GUILIST_ITEM_DATA_T item_data       =   {0};
    uint16              v_space         =   0;
    GUI_BG_T            bg              =   {0};
    MMI_STRING_T        temp_str        =   {0};
    uint32              dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id); 
    uint8               volte_info      =   0;
    uint16              cur_selection   =   0;
    uint16              no_seltction    =   0;
    BOOLEAN             is_call_state   =   FALSE;
    BOOLEAN             is_volte_state  =   FALSE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(MMISET_SET_VOLTE_LIST_CTRL_ID, VOLTE_MAX_ITEMS, FALSE );//max item 2
            MMISET_AppendVoLTEOptionItem(dual_sys, MMISET_SET_VOLTE_LIST_CTRL_ID);/*lint !e64*/
            
            GUILIST_SetOwnSofterKey( MMISET_SET_VOLTE_LIST_CTRL_ID, TRUE );
            
            MMK_SetAtvCtrl(win_id, MMISET_SET_VOLTE_LIST_CTRL_ID);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            break;
            
        case MSG_GET_FOCUS:
            volte_info = MMIAPIPHONE_GetVolteInfo();
            if (volte_info & (1 << dual_sys))
            {
                cur_selection = 0;
                no_seltction = 1;
            }
            else
            {
                cur_selection = 1;
                no_seltction = 0;
            }
            GUILIST_SetSelectedItem( MMISET_SET_VOLTE_LIST_CTRL_ID, no_seltction, FALSE );
            GUILIST_SetSelectedItem( MMISET_SET_VOLTE_LIST_CTRL_ID, cur_selection, TRUE );
            
            break;

        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
             cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_VOLTE_LIST_CTRL_ID);
             //(MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_1) == 0)
             is_call_state = MMIAPICC_IsInState(CC_IN_CALL_STATE);
             is_volte_state = MMIAPIPHONE_GetVolteStatus((MN_DUAL_SYS_E)dual_sys);
             SCI_TraceLow("fzb HandleSetVoLTEWindow is_call_state(%d)  is_volte_state %d ",is_call_state,is_volte_state);
             if (is_call_state)
             {
                 //MMIPUB_OpenAlertWarningWin(qtn_in_call_unavailable);
             }
             else
             {
                 if (0 == cur_selection)
                 {
                     MMIAPIPHONE_SetVolteInfo(dual_sys, TRUE); 
                 }
                 else
                 {
                     MMIAPIPHONE_SetVolteInfo(dual_sys, FALSE); 
                 }

                 ShowWaitingWin();
                 MMIAPIPHONE_SetIdleVolteState((MN_DUAL_SYS_E) dual_sys);
             }
             
             break;
             
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        case MSG_CLOSE_WINDOW:
             MMK_CloseWin(win_id);
             break;
             
        default:
             recode = MMI_RESULT_FALSE;
             break;
    }
    
    return recode;
}
#endif

//@maggie add begin for cr79064
/*****************************************************************************/
//  Description : HandleNetworkTypeWindow
//  Global resource dependence : 
//  Author:maggie.ren
//  Note: yongwei.he modify 2007-07-18
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkTypeWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{//@zhaohui modify for cr94108
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
#ifdef MMI_DUALMODE_ENABLE 
    uint16 cur_sel_item = 0;
    uint8 testmode[4] =
    {
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID
    };
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    MN_PHONE_POWER_TEST_MODE_IND network_type = 0;/*lint !e64*/

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_NETWORK_TYPE_CTRL_ID, MMISET_SET_MAX_NETWORK_TYPE,FALSE);//max item 4
        AppendNetworkTypeListItem(MMISET_SET_NETWORK_TYPE_CTRL_ID, dual_sys);

        GUILIST_SetOwnSofterKey( MMISET_SET_NETWORK_TYPE_CTRL_ID, TRUE );

        MMK_SetAtvCtrl(win_id,MMISET_SET_NETWORK_TYPE_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        network_type = GetNetworkType(dual_sys);
#ifdef MODEM_SUPPORT_SPRD
        //set selected item
#ifndef WIN32
        MNPHONE_GetTestModeEx(testmode);
#endif
        cur_sel_item = GetSelectIndexByNetworkType(testmode[dual_sys]);
        GUILIST_SetSelectedItem(MMISET_SET_NETWORK_TYPE_CTRL_ID, cur_sel_item, TRUE);
#elif defined (PLATFORM_UWS6121E)
        //set selected item
        GUILIST_SetSelectedItem(MMISET_SET_NETWORK_TYPE_CTRL_ID, network_type, TRUE);
#else
    #error
#endif

        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_sel_item = GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_TYPE_CTRL_ID); 

#if defined (MODEM_SUPPORT_SPRD)
        network_type = GetNetworkTypeBySelectIndex(cur_sel_item);
        MMIAPISET_SetTestMode(dual_sys, network_type);
#elif defined (MODEM_SUPPORT_RDA)
        SetNetworkType((NETWORK_TYPE_E)cur_sel_item, FALSE, FALSE, FALSE);
#else
    #error
#endif
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
#endif
    
    return recode;    
}

#if defined(MMI_GUI_STYLE_TYPICAL)
/*****************************************************************************/
//  Description : handle set any key and reject mode win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnykeyRejectWindow(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_SET_ANYKEY_REJECT_MODE_CTRL_ID;  
    uint16                      selected_num    =   0;
    MMISET_ANYKEY_REJECT_SET_T  mode_info       =   {0};       
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_SET_ANYKEY_REJECT_MAX_ITEM, FALSE );//max item 2
        GUILIST_SetMaxSelectedItem(ctrl_id,MMISET_SET_ANYKEY_REJECT_MAX_ITEM);
        MMISET_AppendAnykeyRejectListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //get current selected items
        selected_num = GUILIST_GetCurItemIndex(ctrl_id);
        mode_info = MMIAPISET_GetAnykeyRejectMode();
        
        switch (selected_num)
        {
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        case 0:
            mode_info.is_sms_reply_open = mode_info.is_sms_reply_open ? FALSE:TRUE;
            break;
            
        case 1:
            mode_info.is_anykey_answer = mode_info.is_anykey_answer ?  FALSE:TRUE;
            break;
#else
        case 0:
            mode_info.is_anykey_answer = mode_info.is_anykey_answer ?  FALSE:TRUE;
            break;
#endif
            
        default:
            //SCI_TRACE_LOW:"HandleSetAnykeyRejectWindow: selected item is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_8587_112_2_18_2_50_34_84,(uint8*)"d",selected_num);
            break;
        }
        
        MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&mode_info);
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //save anykey reject mode info
            MMISET_SetAnykeyRejectMode(ctrl_id);
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_APP_RED:
            //save anykey reject mode info
            MMISET_SetAnykeyRejectMode(ctrl_id);
            recode = MMI_RESULT_FALSE;
            break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
        case MSG_APP_FLIP:
            //save anykey reject mode info
            MMIDEFAULT_HandleFlipKey(FALSE);
            MMISET_SetAnykeyRejectMode(ctrl_id);
            recode = MMI_RESULT_FALSE;
            break;
#endif            
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;  
}
#endif

/*****************************************************************************/
//  Description : handle set connect prompt remind win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConnectPromptWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id  =   MMISET_SET_CONNECT_PROMPT_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMISET_AppendOpenCloseListItem(ctrl_id,MMIAPISET_GetIsConnectPrompt);       
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_WEB:
    case MSG_APP_OK:

        MMIAPISET_SetByOpenCloseList(ctrl_id, MMIAPISET_GetActiveSim(), MMISET_SetIsConnectPrompt);
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
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


/*****************************************************************************/
//  Description : to open ussd select  window
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenUssdSelectWin(MMISET_SIM_SELECT_TYPE_E select_type)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)

    MMISET_APPEND_SIM_TYPE_E append_type = MMISET_APPEND_SYS_OK;

    switch((MMISET_SIM_SELECT_TYPE_E)select_type) 
    {
    case MMISET_MP3_PLAYER_CALL_RING:
    case MMISET_MULTIM_MUSIC_CALL_RING:
    //case MMISET_IP_CALL:
    case MMISET_FLIP_ANSWER:
        append_type = MMISET_APPEND_SIM_ALL;
        break;

    case MMISET_SET_PIN:
        append_type = MMISET_APPEND_PIN1_PERMIT;
        break;

    case MMISET_SET_PIN2_MODIFY:
#ifdef FDN_SUPPORT
    case MMISET_SET_FDN:
#endif
        append_type = MMISET_APPEND_SIM_OK;
        break;
        
    case MMISET_MULTI_SIM_NAME:
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    case MMISET_MULTI_SIM_COLOR:
#endif    
        append_type = MMISET_APPEND_SIM_ALL;
        break;
        
    default:
        append_type = MMISET_APPEND_SYS_OK;
        break;
    }

    MMISET_SelectSIMFunc(SIMSelectWinCallback, append_type, (ADD_DATA)select_type);
#endif
}

/*****************************************************************************/
// Description : 设置飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-8
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_OpenSetFlyMode(void)
{
    MMK_CreateWin((uint32*)MMISET_FLY_MODE_WIN_TAB,PNULL);
    return MMISET_FLY_MODE_WIN_ID;
}

///*****************************************************************************/
////    Description : MMISET_EnterMultiSimSettingMenu
////    Global resource dependence : 
////  Author:
////    Note: 
///*****************************************************************************/
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
PUBLIC void MMISET_EnterMultiSimSettingMenu(void)
{
    if (MMK_IsOpenWin(MMISET_SET_MULTI_SYS_SETTING_WIN_ID))
    {
        MMK_WinGrabFocus(MMISET_SET_MULTI_SYS_SETTING_WIN_ID);

    }
    else
    {
        MMK_CreateWin ((uint32 *)MMISET_MULTI_SIM_SETTING_WIN_TAB,PNULL);
    }
}
#endif

/*****************************************************************************/
//  Description : 语音信箱号码界面打开时，等待界面不允许返回
//  Global resource dependence : 
//  Author: kelly.li
//  Note:2006-02-13
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVMForDivertWaitingWindow(
                                                  MMI_WIN_ID_T      win_id,     // 窗口的ID
                                                  MMI_MESSAGE_ID_E     msg_id,  // 窗口的内部消息ID
                                                  DPARAM                param       // 相应消息的参数
                                                  )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED:
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

#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
//  Description : 翻盖接听
//  Global resource dependence : 
//  Author:jibin
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFlipAnswerWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    uint32                      dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    uint16                      cur_selection   =   0;
    MMISET_FLIP_ANSWER_TYPE_T   flip_answer     =   {0};/*lint !e64*/
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_SET_FLIP_ANSWER_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_FLIP_ANSWER_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        
        //add items to listbox
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        if (MMIAPISET_GetIsFlipAnswer(dual_sys))/*lint !e64*/
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
        
        //        MMISET_AppendOpenCloseListItem(MMISET_SET_FLIP_ANSWER_CTRL_ID,MMIAPISET_GetIsFlipAnswer);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_FLIP_ANSWER_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16 i = 0;

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                flip_answer.is_open[i] = MMIAPISET_GetIsFlipAnswer((MN_DUAL_SYS_E)i);
            }

            //get the selected item
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            
            //SCI_ASSERT((MMISET_OPEN_CLOSE_ITEM > cur_selection));
		if(MMISET_OPEN_CLOSE_ITEM <= cur_selection)
		{
		    //SCI_TRACE_LOW:"mmisetting_assert HandleSetFlipAnswerWindow cur_selection %d"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_8908_112_2_18_2_50_35_85,(uint8*)"d", cur_selection);
		    break;
		} 
            
            flip_answer.is_open[dual_sys] = (cur_selection == 0? TRUE:FALSE);
            
            MMISET_SetIsFlipAnswer(&flip_answer);
            //        MMISET_SetByOpenCloseList(MMISET_SET_FLIP_ANSWER_CTRL_ID, dual_sys, MMISET_SetIsFlipAnswer);
            
            //success!
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            
            MMK_CloseWin( win_id );
        }
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
//  Description : 提示飞行模式
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AlertFlyMode(void)
{
    MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST);
}

/*****************************************************************************/
//  Description : to handle plmn list
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlmnListWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,MMISET_SET_PLMN_LIST_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SelectNetwork(dual_sys, MMISET_SET_PLMN_LIST_CTRL_ID);/*lint !e64*/
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

                                             
/*****************************************************************************/
//  Description : to select no answer time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoAnswerTimeWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    MMI_STRING_T    voice_number    =   {0};
    uint32          dual_sys        =   (uint32)MMK_GetWinAddDataPtr(win_id);
    
    SCI_MEMSET(&voice_number, 0, sizeof(MMI_STRING_T));
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_NO_ANSWER_TIME_CTRL_ID,MMISET_NO_ANSWER_TIME_MAX_ITEM, FALSE );//max item 6
        MMISET_AppendNoAnswerTimeListItem(MMISET_SET_NO_ANSWER_TIME_CTRL_ID);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_NO_ANSWER_TIME_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (MMK_IsOpenWin(MMISET_SET_DIVERT_NUMBER_WIN_ID))
        {
            //get phone number
            GUIEDIT_GetString(MMISET_SET_DIVERT_NUMBER_CTRL_ID, &voice_number);
        }
        else
        {
            //get voice box number
            GUIEDIT_GetString(MMISET_SET_DIVERT_VOICE_CTRL_ID, &voice_number);
        }
        
        if (0 == voice_number.wstr_len)//no phone number
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
            MMK_CloseWin( win_id ); 
        }
        else if (MMIAPICOM_IsValidNumberStringEx(&voice_number))//enter call forwarding
        {   
            uint32 i = 0;
            BOOLEAN is_valid = TRUE;

            for (i = 0; i < voice_number.wstr_len; i++)
            {
                if ('*' == voice_number.wstr_ptr[i]
                    || '#' == voice_number.wstr_ptr[i]
                    || 'p' == voice_number.wstr_ptr[i]
                    || 'w' == voice_number.wstr_ptr[i]
                    || 'P' == voice_number.wstr_ptr[i]
                    || 'W' == voice_number.wstr_ptr[i])
                {
                    is_valid = FALSE;

                    break;
                }
            }

            if (is_valid)
            {
                MMISET_StartCallForwarding(dual_sys, &voice_number);/*lint !e64*/
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        }
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




/*****************************************************************************/
//     Description : add fly mode set list to list 
// Global resource dependence : 
//  Author: kelly.li
// Note:
/*****************************************************************************/
LOCAL void AppendFlyModeSetListItem(
                                    MMI_CTRL_ID_T      ctrl_id
                                    )
{
    GUILIST_ITEM_T          item_t                  =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data               =   {0};/*lint !e64*/
    MMI_STRING_T            fly_string              =   {0};  
    MMI_IMAGE_ID_T          image_id                =   IMAGE_NULL;
    MMI_TEXT_ID_T           fly_text_id[2]          =   {TXT_ACTIVE_OPERATIVE,TXT_SET_SELECT_ON_POWER};
    uint32                  i                       =   0;
    uint16                  cur_selection           =   0;
    BOOLEAN                 is_fly_mode_on          =   FALSE;
    BOOLEAN                 is_flymode_openphone    =   FALSE;
    
    GUILIST_SetMaxItem(ctrl_id,2, FALSE );   
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SELECT;
    item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
    is_fly_mode_on = MMIAPISET_GetFlyMode();
    is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
    
    for (i = 0; i < 2; i++)
    {
        switch(i)
        {
        case 0:
            if (is_fly_mode_on)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
            }
            break;
            
        case 1:
            if (is_flymode_openphone)
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
            }
            break;
            
        default:
            break;
        }//end switch
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = image_id;
        
        
        MMI_GetLabelTextByLang(fly_text_id[i], &fly_string);
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = fly_string.wstr_len;  
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;
        
        //         MMIAPICOM_Wstrncpy( item_data.item_content[1].item_data.text_buffer.wstr,
        //                             fly_string.wstr_ptr,
        //                             item_data.item_content[1].item_data.text_buffer.wstr_len );
        
        //item_data.item_content[1].item_data.text_buffer.is_ucs2 = fly_string.is_ucs2;
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }//end for
    
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}


/*****************************************************************************/
//     Description : replace some struct of the  GUILIST_ITEM_T
// Global resource dependence : 
//  Author: Qiang.Zhang
// Note:
/*****************************************************************************/
LOCAL void SetCallSetOthersItemCheck(
                        MMI_CTRL_ID_T ctrl_id,
                        BOOLEAN is_check,
                        uint16 index
					   )
{
    const GUILIST_ITEM_T* item_t = PNULL;

    item_t = GUILIST_GetItemPtrByIndex(ctrl_id, index);

    item_t->item_data_ptr->item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_t->item_data_ptr->item_content[1].item_data.text_id = is_check?IMAGE_CHECK_SELECTED_ICON:IMAGE_CHECK_UNSELECTED_ICON;

    item_t->item_data_ptr->softkey_id[0] = is_check ? STXT_CANCEL : STXT_SELECT;

    GUILIST_ReplaceItem(ctrl_id, item_t, index);
}
 
 /*****************************************************************************/
 //  Description : to handle interrogate ss status report
 //  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleAllCFReportWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set softkey
        MMK_SetAtvCtrl(win_id,MMISET_SS_ALL_REPORT_CTRL_ID);
        GUIRICHTEXT_SetFocusMode(MMISET_SS_ALL_REPORT_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
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

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : ImplementStandbyChanged
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImplementStandbyChanged(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    // BOOLEAN is_changed = FALSE;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMISET_DUALSYS_TYPE_E standby_mode = 0;

    //SCI_ASSERT(PNULL != is_standby_ptr);
   if(PNULL == is_standby_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert ImplementStandbyChanged"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_9243_112_2_18_2_50_36_86,(uint8*)"");
        return FALSE;
    } 

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (is_standby_ptr[i])
        {
            standby_mode |= (0x01 << i);
        }
    }

    if (e_dualsys_setting != standby_mode)
    {
        if (MMIAPISET_GetFlyMode())//如果是飞行模式只切换待机模式
        {
            //记录用户自己设置的情况
            MMISET_SetMultiSysSetting(standby_mode);
            
            MAIN_UpdateIdleSignalState();
        }
        else
        {
            BOOLEAN is_allow_set = TRUE;

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (!(e_dualsys_setting & (0x01 << i)) && is_standby_ptr[i])
                {
                    if (!MMIAPIPHONE_GetIsPsDeactiveCnf(i))
                    {
                        is_allow_set = FALSE;
                        break;
                    }
                }
            }

            if (!is_allow_set)
            {
                return FALSE;
            }

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if ((e_dualsys_setting & (0x01 << i))
                    && MMIAPIPHONE_GetSimExistedStatus(i)                    
                    && !is_standby_ptr[i])
                {
                    if (!MMIAPIPHONE_GetIsPsReady(i))
                    {
                        is_allow_set = FALSE;
                        break;
                    }
                }
            }

            if (!is_allow_set)
            {
                return FALSE;
            }

            MMIAPISET_SetIsQueryCfu(FALSE, dual_sys);

            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if ((e_dualsys_setting & (0x01 << i)) && !is_standby_ptr[i])//关闭待机
                {
                    /* pwr on -> pwr off */
#ifdef MMI_GPRS_SUPPORT
                    MMIAPIPDP_ResetPdp(i);
                    MMIAPIPHONE_SetGPRSDetachEx(i);
#endif

                    MMIAPIPHONE_PowerOffPs(i);
                    MAIN_SetIdleForwardState(i,FALSE);
                    MAIN_SetIdleRoamingState(i,FALSE);
                    MMIAPIPHONE_SetIdleGprsState(i);
#ifdef QQ_SUPPORT
                    MMIAPIQQ_UpdateSimSelect(i);
#endif
                }
                else if (!(e_dualsys_setting & (0x01 << i)) 
                            && MMIAPIPHONE_GetSimExistedStatus(i)
                            && is_standby_ptr[i])//开启待机
                {
                    /* pwr off -> pwr on */
                    MMIAPIPHONE_PowerOnPsByUsim(i);
                }
                MMIAPIPHONE_SetIdleGprsState(i);
            }

            //记录用户自己设置的情况
            MMISET_SetMultiSysSetting(standby_mode);

#ifdef MMI_GPRS_SUPPORT
            MMIAPIPHONE_SetGPRSAttachMode();

            if (!MMIAPICONNECTION_isGPRSNeedAutoAttach())
            {
                for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(is_standby_ptr[i] && !MMIAPIPDP_PsIsPdpLinkExistEx(i))
                    {
                        MMIAPIPHONE_SetGPRSDetachEx((MN_DUAL_SYS_E)i);
                    }
                }   
            }
            else
            {
                MMIAPIPHONE_SetGPRSAttach();
            }
#endif/*MMI_GPRS_SUPPORT*/

            if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
            {
                BOOLEAN is_first = TRUE;
                //目前不支持零待机，无待机卡，则当前有且仅有一个PS启动。
                //遍历所有设置为待机且无SIM卡的卡槽，启动第一个遍历到的PS，其他均关闭PS
                for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
                {
                    if ((standby_mode & (0x01 << i))
                        && !MMIAPIPHONE_GetSimExistedStatus(i))
                    {
                        if (is_first) // 启动第一个遍历到的无SIM卡卡槽PS
                        {
                            MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                            is_first = FALSE;
                            continue; //继续搜索剩下的设置待机且无SIM卡的卡槽，并关闭相应的ps
                        }
                        
                        MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                    }
                }
            }
            else
            {
                for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
                {
                    if ((standby_mode & (0x01 << i))
                        && !MMIAPIPHONE_GetSimExistedStatus(i))//关闭不存在SIM卡的PS
                    {
                        MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                    }
                }
            }

            MAIN_UpdateIdleSignalState();
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : AppendMultiSysTypeList
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AppendMultiSysTypeList(MMI_CTRL_ID_T ctrl_id)
{
    uint32 i = 0;
    uint16 cur_selection = 0;
    uint16 topItemIndex = 0;
    BOOLEAN is_fly_mode_on = FALSE;
    BOOLEAN is_flymode_openphone =   FALSE;
    MMI_STRING_T fly_string =  {0};
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T status_image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T sim_image_id = IMAGE_NULL;
    MMISET_SIM_NAME_T sim_str = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    MMI_STRING_T network_str = {0};
    MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={0};
    MMI_IMAGE_ID_T no_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMI_IMAGE_ID_T gray_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    is_fly_mode_on = MMIAPISET_GetFlyMode();
    is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        sim_img_id[i] = IMAGE_IDLE_PROMPT_SIM1 + (i-MN_DUAL_SYS_1);    //shoule IMAGE_IDLE_PROMPT_NO_SIM2 == IMAGE_IDLE_PROMPT_NO_SIM1+1.atc
        no_sim_img_id[i] = IMAGE_IDLE_PROMPT_NO_SIM1 + (i-MN_DUAL_SYS_1);
        gray_sim_img_id[i] = IMAGE_IDLE_PROMPT_GRAY_SIM1 + (i-MN_DUAL_SYS_1);
    }

    cur_selection = GUILIST_GetCurItemIndex(ctrl_id); 
    topItemIndex = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX+2, FALSE);
#if !defined MMI_GUI_STYLE_TYPICAL 
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
#else
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;
#endif
    item_t.item_data_ptr = &item_data;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

        if (e_dualsys_setting & (0x01 << i))
        {
            status_image_id = IMAGE_CHECK_SELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_CLOSE;
#ifdef MMI_GUI_STYLE_MINISCREEN	
	     item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)IMAGE_NULL;
#else
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
#endif
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                if (is_fly_mode_on)
                {
                    sim_image_id = gray_sim_img_id[i];
                }
                else
                {
                    sim_image_id = sim_img_id[i];
                }
            }
            else
            {
                status_image_id = IMAGE_CHECK_UNSELECTED_ICON;
                sim_image_id = no_sim_img_id[i];
            }
        }
        else
        {
            status_image_id = IMAGE_CHECK_UNSELECTED_ICON;

            item_data.softkey_id[LEFT_BUTTON] = TXT_OPEN;
#ifdef MMI_GUI_STYLE_MINISCREEN	
	     item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)IMAGE_NULL;
#else			
            item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
#endif
            item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

            if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                sim_image_id = gray_sim_img_id[i];
            }
            else
            {
                sim_image_id = no_sim_img_id[i];
            }
        }

        if (is_fly_mode_on)
        {
            status_image_id = IMAGE_CHECK_UNSELECTED_ICON;
            item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
        }

        sim_str = MMIAPISET_GetSimName(i);
#if !defined MMI_GUI_STYLE_TYPICAL      
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_len = MIN(GUILIST_STRING_MAX_NUM, sim_str.wstr_len);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;

        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = status_image_id;
        
        SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
        network_str.wstr_ptr = (wchar *)opn_spn_name;
        SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
        
        MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);

        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_len = network_str.wstr_len;  
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;

#else
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = status_image_id;

        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = sim_image_id;
        
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_len = MIN(GUILIST_STRING_MAX_NUM, sim_str.wstr_len);
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;

        SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
        network_str.wstr_ptr = (wchar *)opn_spn_name;
        SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
        
        MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);

        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data.text_buffer.wstr_len = network_str.wstr_len;  
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;
#endif
        GUILIST_AppendItem(ctrl_id, &item_t);

        if (is_fly_mode_on)
        {
            GUILIST_SetItemGray(ctrl_id, i, TRUE);
        }
        else
        {
            GUILIST_SetItemGray(ctrl_id, i, FALSE);
        }

        if (!MMIAPIPHONE_GetSimExistedStatus(i))
        {
            GUILIST_SetItemGray(ctrl_id, i, TRUE);
        }
    }

    if (is_fly_mode_on)
    {
        sim_image_id = IMAGE_CHECK_SELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
    }
    else
    {
        sim_image_id = IMAGE_CHECK_UNSELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
    }
#if !defined MMI_GUI_STYLE_TYPICAL       
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    
    MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &fly_string);

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = fly_string.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = sim_image_id;
    
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id = TXT_NULL;  

    GUILIST_AppendItem(ctrl_id,&item_t);

    if (is_flymode_openphone)
    {
        sim_image_id = IMAGE_CHECK_SELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
    }
    else
    {
        sim_image_id = IMAGE_CHECK_UNSELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
    }
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;

    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &fly_string);  
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = fly_string.wstr_len;  
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = sim_image_id;        
    
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id = TXT_NULL;  
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = sim_image_id;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = IMAGE_SECMENU_ICON_CALLSET_FLYMODE;        
    MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &fly_string);

    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = fly_string.wstr_len;
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;
    GUILIST_AppendItem(ctrl_id,&item_t);

    if (is_flymode_openphone)
    {
        sim_image_id = IMAGE_CHECK_SELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_CANCEL;
    }
    else
    {
        sim_image_id = IMAGE_CHECK_UNSELECTED_ICON;
        item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_SELECT;
    }
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = sim_image_id;
    
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = IMAGE_SECMENU_ICON_CALLSET_FLYMODE;        
    MMI_GetLabelTextByLang(TXT_SET_SELECT_ON_POWER, &fly_string);   

    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = fly_string.wstr_len;  
    
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = fly_string.wstr_ptr;
#endif
    GUILIST_AppendItem(ctrl_id,&item_t);
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);    
    GUILIST_SetTopItemIndex(ctrl_id, topItemIndex);
 }

/*****************************************************************************/
//  Description : to handle dual sys time window msg
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMultiSysTypeWindow(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    uint32 i = 0;
    uint16 cur_selection = 0;
    BOOLEAN is_ps_deactive = TRUE;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on  =   FALSE;
    BOOLEAN             is_flymode_openphone    =   FALSE;
    MMI_CTRL_ID_T list_ctrl = MMISET_SET_DAULSYS_TYPE_CTRL_ID;
    LOCAL BOOLEAN s_is_standby[MMI_DUAL_SYS_MAX] = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    uint32 dual_sys = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(s_is_standby, 0x00, sizeof(s_is_standby));
        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (e_dualsys_setting & (0x01 << i))
            {
                s_is_standby[i] = TRUE;
            }
        }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)  
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
#endif
        AppendMultiSysTypeList(list_ctrl);

        MMK_SetAtvCtrl(win_id, list_ctrl);
        break;

    case MSG_FULL_PAINT:
        AppendMultiSysTypeList(list_ctrl);
        GUILIST_ValidCurItemPos(list_ctrl);        
        break;

#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    case MSG_GET_FOCUS:  
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;
#endif 

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            BOOLEAN is_has_selected = FALSE;

            is_fly_mode_on = MMIAPISET_GetFlyMode();
            //get cur selection
            cur_selection = GUILIST_GetCurItemIndex(list_ctrl);
            SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_fly_mode_on = %d, cur_selection = %d", is_fly_mode_on, cur_selection);

            if(cur_selection == MMI_DUAL_SYS_MAX)
            {
#ifdef MCARE_V31_SUPPORT
				if(MCareV31_Exit_Confirm())
				{
					return MMI_RESULT_FALSE;		
				}
#endif
                if (!is_fly_mode_on)
                {
                    BOOLEAN is_ps_ready = TRUE;
                    
#ifdef BLUETOOTH_SUPPORT
                    if(MMIAPIBT_IsOpenFileWin())
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
                        return MMI_RESULT_FALSE;
                    } 
#endif

                    for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                    {
                        if ((e_dualsys_setting & (0x01 << dual_sys))
                            && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                        {
                            is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);

                            if (!is_ps_ready)
                            {
                                break;
                            }
                        }
                    }

                    SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_ps_ready = %d", is_ps_ready);

                    if (is_ps_ready)
                    {
                        if (MMISET_GetIsQueryOpenFlyMode())
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                            break;
                        }
                        else
                        {
                            is_fly_mode_on = TRUE;
                            MMIAPISET_SetFlyMode(is_fly_mode_on);

                            MMIAPIPHONE_DeactivePsAccordingToSetting();
     //                       SetCallSetOthersItemCheck(list_ctrl,is_fly_mode_on,cur_selection);
                            SCI_TRACE_LOW("HandleSetMultiSysTypeWindow Set");
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    
                        break;
                    }
                }
                else
                {
                    is_ps_deactive = TRUE;
                    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
                        {
                            is_ps_deactive = FALSE;
                            break;
                        }
                    }

                    SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_ps_deactive = %d i = %d", is_ps_deactive, i);

                    if (is_ps_deactive)
                    {
                        is_fly_mode_on = FALSE;
                    
                        MMIAPISET_SetFlyMode(is_fly_mode_on);
                    
                        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
                    
                        // 激活协议栈
                        MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
                        MMIAPIMMS_RestartAutodownMMS();
    //                    SetCallSetOthersItemCheck(list_ctrl,is_fly_mode_on,cur_selection);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                        break;
                    }
                }
            }
            else if(cur_selection == MMI_DUAL_SYS_MAX+1)
            {
               // BOOLEAN  is_fly_on = !is_fly_mode_on;
                
                is_flymode_openphone = MMIAPISET_GetFlyModeSetPowerOn();
   //             SetCallSetOthersItemCheck(list_ctrl, is_fly_on, cur_selection);
                is_flymode_openphone = !is_flymode_openphone;
                MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_flymode_openphone);
            }
            else
            {
                s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if (s_is_standby[i])
                    {
                        is_has_selected = TRUE;
                        break;
                    }
                }

                SCI_TRACE_LOW("HandleSetMultiSysTypeWindow is_has_selected = %d i = %d standby = %d", is_has_selected, i, s_is_standby[cur_selection]);

                if (!is_has_selected || (1 == MMIAPIPHONE_GetSimExistedNum(PNULL, 0) && 1 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0)))
                {
                    s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                    MMIPUB_OpenAlertWarningWin(TXT_SET_STANDBY_MODE_ALERT);
                }
                else
                {
                    if (!ImplementStandbyChanged(s_is_standby,(MN_DUAL_SYS_E)cur_selection))
                    {
                        s_is_standby[cur_selection] = !s_is_standby[cur_selection];

                        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
                    }
                }
            }
        }

		AppendMultiSysTypeList(list_ctrl);
		GUILIST_ValidCurItemPos(list_ctrl);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

     case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        
        MMISET_StopPSApplication();
        
        is_fly_mode_on = TRUE;
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        
        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);

#ifdef DCD_SUPPORT                      
        MMIAPIDCD_HandFlyModeSetChange();
#endif
        
        MMIAPIPHONE_DeactivePsAccordingToSetting();
        
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : to handle sim active type when all sim is set to on type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAnwerWithOriginalSimWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        )
{
    
    MMI_RESULT_E            result                                                      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           list_ctrl                                                   =   MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_CTRL_ID;
    int32                   i                                                           =   0;
    BOOLEAN                 is_set                                                      =   FALSE;
    uint16                  list_index_array[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX]  =   {0};
    uint16                  sel_index_num                                               =   0;
    BOOLEAN                 is_set_array[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX]       =  {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //init list string
        GUILIST_SetMaxItem(list_ctrl, MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX, FALSE );//max item 4
        GUILIST_SetMaxSelectedItem(list_ctrl, MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX);
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_CALL, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_MESSAGE, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#else
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_SMS, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#ifdef MMS_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_MMS, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
#endif
#endif

   #if 0
    MMIAPISET_AppendListItemByTextId(TXT_SET_ORIGIN_REPLY_WAP, STXT_SELECT, list_ctrl, GUIITEM_STYLE_ONE_LINE_CHECK);
   #endif
        
        //set active list item
        for (i = 0; i < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX; i++)
        {
            is_set = MMIAPISET_GetAnswerWithOriginalSimType(i);
            GUILIST_SetSelectedItem(list_ctrl, (uint16)i, is_set);
        }
        MMK_SetAtvCtrl(win_id, list_ctrl);
        break;

    case MSG_APP_OK:
#if !defined(MMI_PDA_SUPPORT) 
    case MSG_CTL_OK:
#endif
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        //save selection
        sel_index_num = GUILIST_GetSelectedItemIndex(list_ctrl, list_index_array, sizeof(list_index_array));
        
        for (i = 0; i < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX; i++)
        {
            is_set_array[i] = FALSE;
        }
        
        for (i = 0; i < sel_index_num; i++)
        {
            is_set_array[list_index_array[i]] = TRUE;
        }

        MMISET_SetAnswerWithOriginalSimType(is_set_array);
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
//  Description : to handle answer with original sim type
//  Global resource dependence : 
//  Author:bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMajorSimWhenAllSimOnWindow(
                                                        MMI_WIN_ID_T       win_id, 
                                                        MMI_MESSAGE_ID_E   msg_id, 
                                                        DPARAM             param
                                                        )
{
    uint16                              cur_selection       =   0;
    MMI_RESULT_E                        result              =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       list_ctrl           =   MMISET_SET_DUALSYS_SIM_ACTIVE_TYPE_CTRL_ID;
    MMISET_MAJOR_SIM_SEL_TYPE_E         e_major_sim_type    =   0;
    uint32                              e_major_sim_func    =   0;
        
    MMI_STRING_T                        title               =   {0};
    BOOLEAN                             is_open             =   FALSE;
    uint32 item_data = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        e_major_sim_func = (uint32)MMK_GetWinAddDataPtr(win_id);
       // SCI_ASSERT(e_major_sim_func < MMISET_MAJOR_SIM_SEL_FUNC_MAX);
	   if(e_major_sim_func >= MMISET_MAJOR_SIM_SEL_FUNC_MAX)
	   {
	        //SCI_TRACE_LOW:"mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_9968_112_2_18_2_50_38_87,(uint8*)"d", e_major_sim_func);
	        return MMI_RESULT_FALSE;
	    }         
        //init list string
        if (e_major_sim_func == MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL)
        {
            //speed dial is special, because it won't use manual select function
            GUILIST_SetMaxItem(list_ctrl, MMISET_MAJOR_SIM_SEL_TYPE_MAX - 1, FALSE );//max item 2
            MMIAPISET_AppendMultiSimName(list_ctrl, TXT_COMMON_OK, MMISET_MAJOR_SIM_SEL_TYPE_MAX - 1, MMISET_APPEND_SIM_ALL);
        }
        else
        {
            GUILIST_SetMaxItem(list_ctrl, MMISET_MAJOR_SIM_SEL_TYPE_MAX, FALSE );//max item 3
            MMIAPISET_AppendMultiSimName(list_ctrl, TXT_COMMON_OK, MMISET_MAJOR_SIM_SEL_TYPE_MAX - 1, MMISET_APPEND_SIM_ALL);

            MMIAPISET_AppendListItemByTextId(TXT_SET_PROMPT_TO_SEL, TXT_COMMON_OK, list_ctrl, GUIITEM_STYLE_ONE_LINE_RADIO);
        }
        
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        
        MMK_SetAtvCtrl(win_id, list_ctrl);
        
        //set active list item
        e_major_sim_type = MMIAPISET_GetMajorSim(e_major_sim_func);
        GUILIST_SetCurItemIndex(list_ctrl, (uint16)e_major_sim_type);
        GUILIST_SetSelectedItem(list_ctrl, (uint16)e_major_sim_type, TRUE);
        
        is_open = FALSE;
        if (MMK_IsOpenWin(MMISET_SET_MULTI_SYS_SETTING_WIN_ID))
        {
           // GUIMENU_GetId(MMISET_SET_MULTI_SIM_SETTING_CTRL_ID, &group_id,&menu_id);
            is_open = TRUE;
        }
        if (MMK_IsOpenWin(MMISET_DUAL_MAJOR_SIM_SEL_WIN_ID))
        {
            // GUIMENU_GetId(MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID, &group_id,&menu_id);
            is_open = TRUE;
        }
        
        title.wstr_len = 0;
        if (is_open)
        {
            switch (e_major_sim_func)
            {
            case MMISET_MAJOR_SIM_SEL_FUNC_CALL:
                MMI_GetLabelTextByLang(TXT_COMM_CALL, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_SMS:
                MMI_GetLabelTextByLang(TXT_SET_SMS_SIM_SEL, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_MMS:
                MMI_GetLabelTextByLang(TXT_MMS, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_WAP:
                MMI_GetLabelTextByLang(TXT_COMMUNICATION_BROWSER, &title);
                break;
                
            case MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL:
                MMI_GetLabelTextByLang(TXT_COMMON_SPEEDDIAL, &title);
                break;
            case MMISET_MAJOR_SIM_SEL_FUNC_STREAMING:
                MMI_GetLabelTextByLang(TXT_SET_STREAMING, &title);
                break;
            default:
                break;
            }//end switch
        }//end if (is_open)
        
        if (title.wstr_len > 0)
        {
            GUIWIN_SetTitleText(win_id, 
                title.wstr_ptr, 
                title.wstr_len, 
                //title.is_ucs2, 
                FALSE);
        }
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        e_major_sim_func = (uint32)MMK_GetWinAddDataPtr(win_id);
        //SCI_ASSERT(e_major_sim_func < MMISET_MAJOR_SIM_SEL_FUNC_MAX);
	   if(e_major_sim_func >= MMISET_MAJOR_SIM_SEL_FUNC_MAX)
	   {
	        //SCI_TRACE_LOW:"mmisetting_assert HandleSetMajorSimWhenAllSimOnWindow e_major_sim_func %d"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10058_112_2_18_2_50_38_88,(uint8*)"d", e_major_sim_func);
	        break;
	    }         
        //save selection
        cur_selection = GUILIST_GetCurItemIndex(list_ctrl);
        if(GUILIST_IsCurrItemInvalid(list_ctrl))
        {
            break;
        }
        
        e_major_sim_type = MMISET_GetMajorSimSelType(e_major_sim_func);
        //SCI_TRACE_LOW:"mmiset:HandleSetMajorSimWhenAllSimOnWindow, cur_select=%d, e_major_sim_func=%d, e_major_sim_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10069_112_2_18_2_50_38_89,(uint8*)"ddd", cur_selection, e_major_sim_func, e_major_sim_type);

        GUILIST_GetItemData(list_ctrl, cur_selection, &item_data);
        if(item_data < MMI_DUAL_SYS_MAX)
        {
            if (item_data != e_major_sim_type)
            {
                //save user selection
                MMISET_SetMajorSimSelType(e_major_sim_func, (MMISET_MAJOR_SIM_SEL_TYPE_E)item_data);
            }
            else
            {
                //do nothing
            }
            //alert success!
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else
        {
            //SCI_TRACE_LOW:"HandleSetMajorSimWhenAllSimOnWindow,dual_sys[%d]>=MMI_DUAL_SYS_MAX"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10088_112_2_18_2_50_38_90,(uint8*)"d",item_data);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/********************************************************************************
 NAME:      Stk Forbid Input Chars
 DESCRIPTION:
 AUTHOR: Michael.Shi
 DATE:      
********************************************************************************/
LOCAL void SetForbidInputChar(MMI_CTRL_ID_T ctrl_id)
{
    wchar forbid_char_value[]= {0x0d, 0x0a};

    GUIEDIT_ConfigDisableChar(ctrl_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
}

#ifdef MMI_MULTISIM_COLOR_SUPPORT
/*****************************************************************************/
//  Description :  Reset button border 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  ResetSimColorButtonBorder(MN_DUAL_SYS_E   dual_sys)
{
    uint16 index = 0;
    GUI_BORDER_T   btn_border = DEFAULT_BTN_BORDER;/*lint !e572*/
    GUI_BORDER_T   btn_Selected_border = SELECTED_BTN_BORDER;/*lint !e572*/

    for (index=0;index<MAX_SIM_COLOR_NUM;index++)
    {
        GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[index],&btn_border,FALSE);
    }
    //Set used button border
    GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[MMISET_GetSimColorIndex(dual_sys)],&btn_Selected_border,FALSE);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :  Color button on click
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetBtnIndexByCtrlID(MMI_CTRL_ID_T ctrl_id,uint16* index)
{
    uint16 i = 0;

    for (i=0;i<MAX_SIM_COLOR_NUM;i++)
    {
        if (ctrl_id == simColor_CtrlID.button_ctrl_id[i])
        {
            *index = i;
            return TRUE;
        }
    }

    return FALSE;

}

/*****************************************************************************/
//  Description :  Color button on click
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ColorButtonOnClick(MMI_WIN_ID_T  win_id,MN_DUAL_SYS_E   dual_sys,BOOLEAN is_selected)
{
    MMI_CTRL_ID_T   ctrl_id=0;
    uint16          index = 0;
    GUI_BORDER_T   btn_border = DEFAULT_BTN_BORDER;/*lint !e572*/
    GUI_BORDER_T   btn_Selected_border = SELECTED_BTN_BORDER;/*lint !e572*/
    
   //Get active ctrl id
    ctrl_id= MMK_GetActiveCtrlId(win_id);

//  if (ctrl_id is color button id
    if (GetBtnIndexByCtrlID(ctrl_id,&index))
    {
        if (is_selected)
        {
            MMISET_SetSimColorByIndex(dual_sys,index);  //Set color
            ResetSimColorButtonBorder(dual_sys);        //Reset color button border
        }
        else
        {
            GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_border,FALSE);            
            s_current_selected_index = index;
            GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_Selected_border,FALSE);
        }
        return MMI_RESULT_TRUE;
    }

    return MMI_RESULT_FALSE;
}   //ColorButtonOnClick

/*****************************************************************************/
//  Description :  init sim color win 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E InitEditSimColorWindow(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys)
{
    GUI_BG_T btn_bg = {0};
    GUIFORM_CHILD_HEIGHT_T      col_height = {0};
    uint16          i=0,index = 0;
    
    GUIFORM_IsSlide(MMISET_SIM_COLOR_SIM_FORM_CTRL_ID,FALSE);        
    GUIFORM_SetStyle(MMISET_SIM_COLOR_SIM_FORM_CTRL_ID,GUIFORM_STYLE_NORMAL);
    
    btn_bg.bg_type=GUI_BG_COLOR;

    for (i=0;i<SIM_COLOR_BUTTON_LINE_COUNT;i++)
    {
        col_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        col_height.add_data = SIM_COLOR_BUTTON_FORM_HEIGHT;
        GUIFORM_SetChildHeight(MMISET_SIM_COLOR_BTN_FORM_CTRL_ID, simColor_CtrlID.button_line_form_id[i], &col_height);
        //Don't use Form border control 
        GUIFORM_PermitChildBorder(simColor_CtrlID.button_line_form_id[i], FALSE);
    }

    //Set button height and bg color
    for (index=0;index<MAX_SIM_COLOR_NUM;index++)  
        {
            btn_bg.color  =MMISET_GetSimColorByIndex(index);
            GUIBUTTON_SetBg(simColor_CtrlID.button_ctrl_id[index],&btn_bg);   //Set BG
            col_height.type     = GUIFORM_CHILD_HEIGHT_FIXED;
            col_height.add_data = SIM_COLOR_BUTTON_RECT_HEIGHT;
            GUIFORM_SetChildHeight(simColor_CtrlID.button_line_form_id[index/4], simColor_CtrlID.button_ctrl_id[index], &col_height);
            IGUICTRL_SetState(MMK_GetCtrlPtr(simColor_CtrlID.button_ctrl_id[index]), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
        }    
        
    ResetSimColorButtonBorder(dual_sys);    //Reset button border
    
    return MMI_RESULT_TRUE;
}   //InitEditSimColorWindow

/*****************************************************************************/
//  Description :  Handle key move events for select sim color win
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ColorButtonOnkeySelect(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys)
{    
    if (s_current_selected_index<MAX_SIM_COLOR_NUM)
    {
        MMISET_SetSimColorByIndex(dual_sys,s_current_selected_index); 
    }

     return TRUE;
}


/*****************************************************************************/
//  Description :  Handle key move events for select sim color win
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ColorButtonOnkeyMove(MMI_WIN_ID_T win_id,MN_DUAL_SYS_E dual_sys,MMI_MESSAGE_ID_E msg_id)
{    
    
    uint16         index = 0;
    GUI_BORDER_T   btn_border = DEFAULT_BTN_BORDER;/*lint !e572*/
    GUI_BORDER_T   btn_Selected_border = SELECTED_BTN_BORDER;/*lint !e572*/
    MMI_CTRL_ID_T   ctrl_id=0;

    
    ctrl_id= MMK_GetActiveCtrlId(win_id);
    if (GetBtnIndexByCtrlID(ctrl_id,&index))
    {
        GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_border,FALSE);            
        switch (msg_id)
        {
            case MSG_APP_LEFT:
                if (s_current_selected_index>0)
                {
                    s_current_selected_index --;
                }
                break;
            case MSG_APP_RIGHT:
                if (s_current_selected_index<MAX_SIM_COLOR_NUM-1)
                {
                    s_current_selected_index ++;
                }
                break;
            case MSG_APP_UP:
                if (SIM_COLOR_BTN_NUM_ONE_LINE<=s_current_selected_index && 
                    s_current_selected_index<MAX_SIM_COLOR_NUM)
                {
                    s_current_selected_index = s_current_selected_index-SIM_COLOR_BTN_NUM_ONE_LINE;
                 }
                break;
            case MSG_APP_DOWN:
                if (s_current_selected_index<SIM_COLOR_BTN_NUM_ONE_LINE)
                {
                    s_current_selected_index = s_current_selected_index+SIM_COLOR_BTN_NUM_ONE_LINE;
                 }
                break;
            default:
                break;
        }

        if(s_current_selected_index<MAX_SIM_COLOR_NUM)
        {
            GUIBUTTON_SetBorder(simColor_CtrlID.button_ctrl_id[s_current_selected_index],&btn_Selected_border,FALSE);
            MMK_SetAtvCtrl(win_id,simColor_CtrlID.button_ctrl_id[s_current_selected_index]);
            return TRUE;
        }
              
    }
    return FALSE;
}

/*****************************************************************************/
//  Description :  HOOK Key events 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E SimColorWinHookFunc(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result      = MMI_RESULT_FALSE;
    MN_DUAL_SYS_E   dual_sys    = (MN_DUAL_SYS_E)((uint32)MMK_GetWinAddDataPtr(win_id));

    switch(msg_id)
    {
        case MSG_APP_UP:
        case MSG_APP_LEFT:
        case MSG_APP_DOWN:
        case MSG_APP_RIGHT:
        {
            if(ColorButtonOnkeyMove(win_id,dual_sys,msg_id))
            {
                result = MMI_RESULT_TRUE;
            }
        }
        break;
        

    default:
        break;
    }
    
    return  result;
}

/*****************************************************************************/
//  Description : 处理SIM SET Color WIN MESSAGE
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimColorWindow(
                                           MMI_WIN_ID_T         win_id,    // 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                           DPARAM               param       // 相应消息的参数
                                           )
{
    MMI_RESULT_E            err_code    = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E           dual_sys    = (MN_DUAL_SYS_E)((uint32)MMK_GetWinAddDataPtr(win_id));
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetTitleTextId(win_id,TXT_SET_SIM_COLOR, FALSE);
        InitEditSimColorWindow(win_id,dual_sys);     //Init ctrl settings
        MMK_SetAtvCtrl(win_id,simColor_CtrlID.button_ctrl_id[MMISET_GetSimColorIndex(dual_sys)]);
        s_current_selected_index = MMISET_GetSimColorIndex(dual_sys);
		MMK_SetWinHookFunc(win_id,SimColorWinHookFunc);

        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT            
        ColorButtonOnClick(win_id,dual_sys,FALSE);            //Click is select
#else
        ColorButtonOnClick(win_id,dual_sys,FALSE);            //Click is select
#endif
        MMK_UpdateScreen();
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:

        if (ColorButtonOnkeySelect(win_id,dual_sys))
        {
            MMK_CloseWin(win_id); //Close win in TK mode
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);        
        break;
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;

}   //HandleEditSimColorWindow

#endif
/*****************************************************************************/
//  Description : 处理SIM SET NAME WIN MESSAGE
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSimNameWindow(
                                           MMI_WIN_ID_T          win_id,     // 窗口的ID
                                           MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                           DPARAM                param       // 相应消息的参数
                                           )
{
    MN_DUAL_SYS_E           dual_sys        =   0;
    MMI_RESULT_E            err_code        =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           edit_ctrl_id    =   MMISET_SET_SIM_NAME_CTRL_ID;
    MMISET_SIM_NAME_T       sim_name        =   {0};
    MMI_STRING_T            edit_string     =   {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetTextMaxLen(edit_ctrl_id,MMISET_MAX_SIM_NAME_LEN/sizeof(wchar), MMISET_MAX_SIM_NAME_LEN);
        
        dual_sys = (uint32)(MMK_GetWinAddDataPtr(win_id));
        
        sim_name = MMIAPISET_GetSimName(dual_sys);
        
        GUIEDIT_SetString(
            edit_ctrl_id,
            sim_name.wstr_arr,
            sim_name.wstr_len
            );

        SetForbidInputChar(edit_ctrl_id);
#if !defined(MMI_PDA_SUPPORT) 
        GUIEDIT_SetSoftkey(edit_ctrl_id, 0, 1, TXT_NULL, TXT_COMMON_OK, PNULL);
        GUIEDIT_SetSoftkey(edit_ctrl_id, 2, 1, STXT_RETURN, TXT_COMMON_CLEAR, PNULL);
#endif
        MMK_SetAtvCtrl(win_id, edit_ctrl_id);
        break;
        
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        //get the content which the user input
        SCI_MEMSET( &edit_string, 0, sizeof(edit_string) );
        GUIEDIT_GetString(edit_ctrl_id, &edit_string);
        //SCI_ASSERT(edit_string.wstr_len <= MMISET_MAX_SIM_NAME_LEN);
	if(edit_string.wstr_len > MMISET_MAX_SIM_NAME_LEN)
	{
	    //SCI_TRACE_LOW:"mmisetting_assert HandleEditSimNameWindow %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10453_112_2_18_2_50_39_91,(uint8*)"d", edit_string.wstr_len);
	    break;
	}         
        if (edit_string.wstr_len != 0)
        {
            
            sim_name.wstr_len = edit_string.wstr_len;
            MMIAPICOM_Wstrncpy( sim_name.wstr_arr, edit_string.wstr_ptr, sim_name.wstr_len );
            //sim_name.is_ucs2 = edit_string.is_ucs2;
            
            dual_sys = (uint32)(MMK_GetWinAddDataPtr(win_id));
            MMISET_SetSimName(dual_sys, &sim_name);
            
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);           
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMM_INPUT_NULL);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        // close the edit window
        MMK_CloseWin(win_id);        
        break;
        
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
    
}

/*****************************************************************************/
//  Description : handle set major sim win msg
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWizardMajorSimSelWindow(
                                                 MMI_WIN_ID_T          win_id,     // 窗口的ID
                                                 MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                                 DPARAM                param       // 相应消息的参数
                                                 )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO            lcd_dev_info    =   {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    BOOLEAN                     is_in_wizard    =   FALSE;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    is_in_wizard = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:           
        if (is_in_wizard)
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_MAIN_SETTING,TXT_NULL,TXT_NEXT_STEP,FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);
        }
        MMK_SetAtvCtrl(win_id, MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID);
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMISET_DUAL_MAJOR_SIM_SEL_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_CALL_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_CALL);
            break;
            
        case ID_SET_SMS_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_SMS);
            break;
            
        case ID_SET_MMS_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_MMS);
            break;
            
        case ID_SET_BROWSER_SIM_SEL:
            //SCI_TRACE_LOW:"mmiset: HandleMultiSimSettingWindow wap"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10546_112_2_18_2_50_39_92,(uint8*)"");
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_WAP);
            break;
            
        case ID_SET_SPEED_DIAL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL);
            break;
            
            
        case ID_SET_STREAMING_SIM_SEL:
            MMK_CreateWin((uint32 *)MMISET_SET_MAJOR_SIM_WHEN_ALLSIM_ON_TAB, (ADD_DATA)MMISET_MAJOR_SIM_SEL_FUNC_STREAMING);
            break;
            
        case ID_SET_ANSWER_USING_ORIGINAL_SIM:
            MMK_CreateWin((uint32 *)MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB, (ADD_DATA)PNULL);
            break;
            
        default:
            //SCI_PASSERT(0,("HandleWizardMajorSimSelWindow menu_id = %d", menu_id));
            break;
        }
        break;
        
        case MSG_APP_CANCEL:     
        case MSG_CTL_CANCEL:
#ifdef MMIPB_SIMDN_SUPPORT
            if (is_in_wizard)
            {
                MMIAPIPB_OpenMSISDNWin(is_in_wizard, FALSE);
            }
#endif
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;      
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E WizardCFNRCSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;

#if defined(MMI_MULTI_SIM_SYS_DUAL)
    if (isReturnOK)
    {
        s_is_wizard_cfnrc = TRUE;
#ifdef  MMI_SMS_VOICE_MAIL_SUPPORT       
        MMK_CreateWin((uint32*)MMISET_SET_OPEN_DIVERT_WIN_TAB,(ADD_DATA)dual_sys);
#else
        MMIAPISET_SetSysForCf(dual_sys);/*lint !e64*/
        OpenSetCallDivertWin((MN_DUAL_SYS_E)dual_sys);
#endif
    }
    else
    {
        s_is_wizard_cfnrc = FALSE;
    }
#endif 
    return (recode);

}


/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectWinCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32  select_type = (uint32)(MMK_GetWinAddDataPtr(MMK_GetFocusWinId()));

    if (isReturnOK)
    {
        s_cur_dual_sys = (MN_DUAL_SYS_E)((uint32)dual_sys);

        DispatchBySelectType((MN_DUAL_SYS_E)s_cur_dual_sys, select_type);
        if (MMISET_CALL_WAITING == select_type) //if MMISET_CALL_WAITING,don't close window
        {
            return MMI_RESULT_FALSE;
        }
        
        return (recode);
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DispatchBySelectType(MN_DUAL_SYS_E dual_sys, MMISET_SIM_SELECT_TYPE_E select_type)
{
#ifdef VT_SUPPORT
    uint32 win_id = MMISET_CALLDIVERT_FORM_CTRL_ID;
#endif

    switch (select_type) 
    {
    case MMISET_CALL_FORWARD:
#ifdef VT_SUPPORT
        MMK_CreateWin((uint32*)MMISET_SET_DIVERT_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);

#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_DIVERT_WIN_TAB, (ADD_DATA)dual_sys);
#endif
        break;
        
    case MMISET_CALL_BAR:
#ifdef VT_SUPPORT
        win_id = MMISET_CALLBAR_FORM_CTRL_ID;
        MMK_CreateWin((uint32*)MMISET_SET_BAR_AUDIOORVIDEO_CALL_WIN_TAB, (ADD_DATA)win_id);
#else
        MMK_CreateWin((uint32*)MMISET_SET_CALL_BAR_WIN_TAB, (ADD_DATA)dual_sys);
#endif
        break;
        
    case MMISET_CALL_WAITING:
        CreateCallWaitingtSelectPopWindow();
        break;
        
    case MMISET_CALL_HIDE_ID:
        MMK_CreatePubListWin((uint32*)MMISET_SET_CALL_HIDE_ID_WIN_TAB, (ADD_DATA)dual_sys);
        break;
        
    case MMISET_SET_NETWORK:
        MMIAPISET_OpenSelNetWorkWin(dual_sys);
        break;
        
    case MMISET_SET_NETWORK_TYPE:
        MMK_CreateWin((uint32*)MMISET_NETWORK_TYPE_WIN_TAB,(ADD_DATA)dual_sys);
        break;
        
#ifdef MMI_VOLTE_SUPPORT
    case MMISET_SET_VOLTE_STATUS:
        MMK_CreatePubListWin((uint32*)MMISET_VOLTE_WIN_TAB,(ADD_DATA)dual_sys);
        break;
#endif        

#ifdef MMI_PREFER_PLMN_SUPPORT
    case MMISET_SET_PREFER_NETWORK:
        MMK_CreateWin((uint32*)MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
    case MMISET_IP_CALL:
        MMK_CreateWin((uint32*)MMISET_LIST_IPNUMBER_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif
#ifdef FLIP_PHONE_SUPPORT        
    case MMISET_FLIP_ANSWER:
        MMK_CreateWin((uint32*)MMISET_SET_FLIP_ANSWER_WIN_TAB, (ADD_DATA)dual_sys);
        break;
#endif       
#ifdef MMI_CSP_SUPPORT        
    case MMISET_LINE_SELECT:
        MMK_CreatePubListWin((uint32*)MMISET_SELECT_LINE_WIN_TAB,(ADD_DATA)dual_sys);
        break;

    case MMISET_LINE_LOCK:
        MMIAPIPHONE_SetCurHandlingPinSimID( dual_sys );
        MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_VERIFY, MNSIM_PIN2, (uint32)MMISET_LOCK_LINE_WIN_TAB, FALSE);        
        //MMK_CreatePubListWin((uint32*)MMISET_LOCK_LINE_WIN_TAB,(ADD_DATA)dual_sys);
        break;      
#endif //MMI_CSP_SUPPORT   


#ifdef MMI_AUDIO_PLAYER_SUPPORT        
    case MMISET_MP3_PLAYER_CALL_RING:
        MMIAPIMP3_SetMp3AsCallRing(dual_sys);
        break; 
#endif

    case MMISET_SET_PIN:
        MMISET_OpenPinWin(dual_sys);
        break;
        
    case MMISET_SET_PIN2_MODIFY:
        MMISET_OpenPin2Win(dual_sys);
        break;
        
#ifdef FDN_SUPPORT

    case MMISET_SET_FDN:
        MMIAPIFDN_SetCurSIM(dual_sys);
        MMISET_OpenFDNWin(dual_sys);
        break;

#endif
    case MMISET_MULTI_SIM_NAME:
        MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)dual_sys);
        break;
        
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    case MMISET_MULTI_SIM_COLOR:
        MMK_CreatePubFormWin((uint32*)MMISET_SET_SIM_COLOR_WIN_TAB,(ADD_DATA)dual_sys);
        //MMK_CreateWin((uint32*)MMISET_SET_SIM_COLOR_WIN_TAB,(ADD_DATA)dual_sys);
        break;
#endif        
    default:
        //SCI_TRACE_LOW:"mmiset_wintab.c:DispatchBySelectType, dual_sys = %d, select_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_10802_112_2_18_2_50_40_93,(uint8*)"dd", dual_sys, select_type);
        break;
    }
}

/*****************************************************************************/
//  Description : INIT CONTROL
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitMultiSimSettingControl(void)
{
    MMI_STRING_T label_name  = {0};
    MMI_GetLabelTextByLang(TXT_SIM_ENABLE_SEL, &label_name);
    GUILABEL_SetText(MMISET_MUTISIM_IDLEMODE_LABEL_CTRL_ID, &label_name, FALSE);
    MMI_GetLabelTextByLang(TXT_SET_REPLY_BY_ORIGINAL_SIM, &label_name);
    GUILABEL_SetText(MMISET_MUTISIM_SIM_REPLY_LABEL_CTRL_ID, &label_name, FALSE);
    MMI_GetLabelTextByLang(TXT_SET_SIM_NAME, &label_name);
    GUILABEL_SetText(MMISET_MUTISIM_SET_NAME_LABEL_CTRL_ID, &label_name, FALSE);
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : InitPdaMultiSimSettingCtrl
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMultiSimSettingCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_MULTI_SIM_SETTING_CTRL_ID;
    uint32 i = 3;
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    ++i;
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        i += 1;//"setting" and "help" menu is 2
    }
#endif

    GUILIST_SetMaxItem(ctrl_id, i, FALSE );
    
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SIM_ENABLE_SEL, IMAGE_COMMON_RIGHT_ARROW, ID_SET_MULTISYS_SIM_ONOFF_TYPE);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_REPLY_BY_ORIGINAL_SIM, IMAGE_COMMON_RIGHT_ARROW, ID_SET_ANSWER_USING_ORIGINAL_SIM);
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_SIM_NAME, IMAGE_NULL, ID_SET_SIM_NAME);
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_SET_SIM_COLOR, IMAGE_NULL, ID_SET_SIM_COLOR);
#endif    
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        AppendPdaCallSettingsItem1Line(ctrl_id, TXT_DUAL_SYS_CFNRC, IMAGE_NULL, ID_SET_DUALSIM_CALL_DIVERT);
        //AppendPdaCallSettingsItem1Line(ctrl_id, TXT_HELP, IMAGE_NULL, ID_SET_DUALSIM_CALL_DIVERT_HELP);
    }
#endif    

}

/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMultiSimSettingWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_MENU_ID_T             menu_id = 0;
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_MULTI_SIM_SETTING_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined(MMI_MULTI_SIM_SYS_DUAL)
        GUIWIN_SetTitleTextId(win_id,TXT_SET_DUAL_SIM_SETTING,FALSE);
#else
        GUIWIN_SetTitleTextId(win_id,TXT_SET_MULTI_SIM_SETTING,FALSE);
#endif
        InitPdaMultiSimSettingCtrl(); 
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_GET_FOCUS:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);
        switch (menu_id)
        {
        case ID_SET_MULTISYS_SIM_ONOFF_TYPE:
            //MMIAPISET_OpenMultiSimSettingsWindow();
            MMIAPISET_OpenMultiSimStandbySettingsWindow();
            break;

        case ID_SET_ANSWER_USING_ORIGINAL_SIM:
            MMK_CreatePubListWin((uint32 *)MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB, (ADD_DATA)PNULL);
            break;
            
        case ID_SET_SIM_NAME:
            MMIAPISET_OpenUssdSelectWin(MMISET_MULTI_SIM_NAME);
            break;
#ifdef MMI_MULTISIM_COLOR_SUPPORT            
        case ID_SET_SIM_COLOR:
            MMIAPISET_OpenUssdSelectWin(MMISET_MULTI_SIM_COLOR);
            break;
#endif            
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
        case ID_SET_DUALSIM_CALL_DIVERT:
            MMIAPISET_SetDualSIMCFNRC(MMISET_SET_DUALSYS_CFNRC_WIN_ID);//CFNRC win
            break;
#if 0            
        case ID_SET_DUALSIM_CALL_DIVERT_HELP:
            MMK_CreateWin((uint32 *)MMISET_SET_DUALSYS_DIVERT_HELP_WIN_TAB, NULL);
            break;
#endif
#endif            
        default:
            break;
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_WEB:
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
/*****************************************************************************/
//  Description : to handle call setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMultiSimSettingWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_MENU_GROUP_ID_T       group_id = 0;
    MMI_MENU_ID_T             menu_id = 0;
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
 /*       InitMultiSimSettingControl();
        GUIWIN_SetTitleTextId(win_id,TXT_SET_DUAL_SIM_SETTING,FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_MUTISIM_IDLEMODE_LABEL_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, TXT_NULL, STXT_RETURN, FALSE);*/
#if defined(MMI_MULTI_SIM_SYS_DUAL)
        GUIWIN_SetTitleTextId(win_id,TXT_SET_DUAL_SIM_SETTING,FALSE);
#else
        GUIWIN_SetTitleTextId(win_id,TXT_SET_MULTI_SIM_SETTING,FALSE);
#endif

        if (MMIAPISET_GetMultiSysSettingNum(NULL, 0) > 1)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_SET_MULTI_SIM_SETTING, ID_SET_MAJORSIM_SELECT, FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_SET_MULTI_SIM_SETTING, ID_SET_MAJORSIM_SELECT, TRUE);
        }
        MMK_SetAtvCtrl(win_id, MMISET_SET_MULTI_SIM_SETTING_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
    case MSG_GET_FOCUS:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMISET_SET_MULTI_SIM_SETTING_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_SET_MULTISYS_SIM_ONOFF_TYPE:
            //MMIAPISET_OpenMultiSimSettingsWindow();
            MMIAPISET_OpenMultiSimStandbySettingsWindow();
            break;

        case ID_SET_ANSWER_USING_ORIGINAL_SIM:
            MMK_CreatePubListWin((uint32 *)MMISET_SET_ANSWER_WITH_ORIGINAL_SIM_TAB, (ADD_DATA)PNULL);
            break;
            
        case ID_SET_SIM1_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
            break;

        case ID_SET_SIM2_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_2);
            break;

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case ID_SET_SIM3_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_3);
            break;

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case ID_SET_SIM4_NAME:
            MMK_CreatePubEditWin((uint32 *)MMISET_SET_SIM_NAME_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_4);
            break;
#endif/* MMI_MULTI_SIM_SYS_QUAD */
#endif/*  MMI_MULTI_SIM_SYS_QUAD ||  MMI_MULTI_SIM_SYS_TRI*/

        default:
            break;
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_WEB:
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

/*****************************************************************************/
//  Description : to open multi sys call settings standby menu window
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMultiSimStandbySettingsWindow(void)
{
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    if ((MMIAPIPB_IsPbReady()
        && MMIAPISMS_IsOrderOk()))
    {
        MMK_CreatePubListWin((uint32*)MMISET_CTA_MULTI_SYS_STANDBY_WIN_TAB, (ADD_DATA)FALSE);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
    }
#else
    if (MMK_IsOpenWin(MMISET_SET_MULTISYS_TYPE_WIN_ID))
    {
        MMK_WinGrabFocus(MMISET_SET_MULTISYS_TYPE_WIN_ID);
    }
    else
    {
        MMK_CreatePubListWin((uint32*)MMISET_SET_MULTISYS_TYPE_WIN_TAB, PNULL);
    }
#endif
}

/*****************************************************************************/
//  Description : to open multi sys call settings main menu window
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMultiSimSettingsWindow(void)
{
    MMISET_EnterMultiSimSettingMenu();
}

/*****************************************************************************/
//  Description : AppendPinSelectListItem
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendPinSelectListItem(MMISET_SIM_SELECT_TYPE_E select_type, MMI_CTRL_ID_T ctrl_id)
{
    int32                   i           =   0;
    MMISET_SIM_NAME_T       sim_str     =   {0};
    MN_DUAL_SYS_E           dual_sys    =   MN_DUAL_SYS_1;
    GUILIST_ITEM_T          item_t      =   {0};
    GUILIST_ITEM_DATA_T     item_data   =   {0};
    SIM_STATUS_E            sim_status  =   SIM_STATUS_OK;
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
    
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE);//max item 2
    
    item_t.item_style  = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)TXT_COMMON_OK;
    item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        switch(select_type)
        {
        case MMISET_SET_PIN:
            sim_status = MMIAPIPHONE_GetSimStatus(i);
            if ((SIM_STATUS_OK == sim_status || SIM_STATUS_REGISTRATION_FAILED == sim_status) && MNSIM_IsPin1EnableEx(i))
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            break;
            
        default:
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
            break;
        }
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = image_id;
        
        dual_sys = (MN_DUAL_SYS_E)i;
        sim_str = MMIAPISET_GetSimName(dual_sys);
        
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;
        
        //         MMIAPICOM_Wstrncpy( item_data.item_content[1].item_data.text_buffer.wstr,
        //             sim_str.wstr_arr,
        //             item_data.item_content[1].item_data.text_buffer.wstr_len);
        
        //item_data.item_content[1].item_data.text_buffer.is_ucs2 = sim_str.is_ucs2;
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
}
#endif  //MMI_MULTI_SIM_SYS_SINGLE

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
//  Description : open dual sim set cfnrc window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenDualsysCFNRCWin(void)
{
    MMI_STRING_T    connect_net_text    =   {0};
    
    s_dualsys_cfnrc_win_close_time = 0;
    s_dualsys_cfnrc_success_num = 0;
    s_is_action_dualsys_cfnrc = TRUE;
    MMI_GetLabelTextByLang(TXT_NETWORK_CONNECT, &connect_net_text);
    
    MMIPUB_OpenWaitWin(
        1,
        &connect_net_text,
        PNULL,
        PNULL,
        MMISET_SET_DUALSYS_CFNRC_WIN_ID,
        NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_ONE,
        HandleDualsysCFNRCWindow
        );
}

/*****************************************************************************/
//  Description : close dual sim set cfnrc window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseDualsysCFNRCWin(MN_DUAL_SYS_E dual_sys, BOOLEAN  is_action_success, MN_SS_ERROR_CODE_E  error_code)
{
    if (s_is_action_dualsys_cfnrc)
    {
        if (is_action_success)
        {
            s_dualsys_cfnrc_success_num++;
        }
        else //failed notify which SIM error, and check SIM or CFNRC whether support
        {
            MMI_TEXT_ID_T sim_text_id[] = {
                                                                    TXT_SIM_SEL_SIM1,
                                                                    TXT_SIM_SEL_SIM2,
                                                                    TXT_SIM_SEL_SIM3,
                                                                    TXT_SIM_SEL_SIM4, 
                                                                 }; 
            MMI_STRING_T  sim_string ={0};
            MMI_STRING_T  prompt_text_string = {0};
            MMI_STRING_T  prompt_string = {0};
            wchar blank_wchar[] = {0x20,0x00};
            MN_DUAL_SYS_E another_sim_index = MMI_DUAL_SYS_MAX;
            MMI_STRING_T  another_sim_string ={0};
            MMI_STRING_T  setting_string ={0};
            
            s_dualsys_cfnrc_win_close_time = 0;
            s_dualsys_cfnrc_success_num = 0;
            s_is_action_dualsys_cfnrc = FALSE;
            
            another_sim_index = (MN_DUAL_SYS_1 == dual_sys)? MN_DUAL_SYS_2: MN_DUAL_SYS_1;
            
            MMI_GetLabelTextByLang(sim_text_id[dual_sys], &sim_string);
            MMI_GetLabelTextByLang(TXT_SET_DUALSIM_DIVERT_FAIL_PROMPT, &prompt_text_string);
            MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &setting_string);
            
            if(MN_SS_ERROR_CALL_BARRED == error_code
                ||MN_SS_ERROR_UNEXPECTED_DATA_VALUE == error_code)
            {
                MMI_STRING_T  msisdn_string ={0};
                
                MMI_GetLabelTextByLang(TXT_PB_MSISDN, &msisdn_string);
                MMI_GetLabelTextByLang(sim_text_id[another_sim_index], &another_sim_string);
                
                prompt_string.wstr_ptr = SCI_ALLOCAZ((sim_string.wstr_len + prompt_text_string.wstr_len + another_sim_string.wstr_len + msisdn_string.wstr_len + setting_string.wstr_len+ 5 ) * sizeof(wchar));
                if(PNULL == prompt_string.wstr_ptr) 
                {
                    return;
                }
                prompt_string.wstr_len = sim_string.wstr_len + prompt_text_string.wstr_len + another_sim_string.wstr_len + msisdn_string.wstr_len + setting_string.wstr_len+4;
                
                MMIAPICOM_Wstrncpy(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, prompt_text_string.wstr_ptr, prompt_text_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, another_sim_string.wstr_ptr, another_sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, msisdn_string.wstr_ptr, msisdn_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, setting_string.wstr_ptr, setting_string.wstr_len);

                //Alert "SIM1 Set Failed, Please check SIM2 Own Number Setting"
                MMIPUB_OpenQueryWinByTextPtr(&prompt_string, PNULL, IMAGE_PUBWIN_QUERY, PNULL, HandleEnterEditMSISDNQueryWin);
            }
            else
            {
                MMI_STRING_T  cfnrc_string ={0};
                uint32 time_period = 0;
                
                MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NOT_REACHABLE, &cfnrc_string);
                
                prompt_string.wstr_ptr = SCI_ALLOCAZ((sim_string.wstr_len + prompt_text_string.wstr_len + sim_string.wstr_len + cfnrc_string.wstr_len + setting_string.wstr_len + 5 ) * sizeof(wchar));
                if(PNULL == prompt_string.wstr_ptr) 
                {
                    return;
                }
                prompt_string.wstr_len = sim_string.wstr_len*2 + prompt_text_string.wstr_len + cfnrc_string.wstr_len + setting_string.wstr_len +4;

                MMIAPICOM_Wstrncpy(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, prompt_text_string.wstr_ptr, prompt_text_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, sim_string.wstr_ptr, sim_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, cfnrc_string.wstr_ptr, cfnrc_string.wstr_len);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, blank_wchar, 1);
                MMIAPICOM_Wstrncat(prompt_string.wstr_ptr, setting_string.wstr_ptr, setting_string.wstr_len);
                
                //Alert "SIM1 set failed, Please Check SIM1 Call Divert unreachable setting"
                MMIPUB_OpenAlertWinByTextPtr(&time_period, &prompt_string, PNULL, IMAGE_PUBWIN_WARNING,
                                                 PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            
            SCI_FREE(prompt_string.wstr_ptr);
            MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
            return;
        }
        
        s_dualsys_cfnrc_win_close_time++;
        
        //sim1注册完毕后sim2开始注册，避免注册时间间隔太短，注册失败的情况
        if (1 == s_dualsys_cfnrc_win_close_time)
        {
            if (MMISET_RegisterDualsysCFNRC(MN_DUAL_SYS_2))
            {
                //SCI_TRACE_LOW:"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin,True"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11324_112_2_18_2_50_41_94,(uint8*)"");
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
            //SCI_TRACE_LOW:"mmiset_wintab.c:MMISET_CloseDualsysCFNRCWin:s_dualsys_cfnrc_win_close_time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11330_112_2_18_2_50_41_95,(uint8*)"d", s_dualsys_cfnrc_win_close_time);
        }
        //SCI_TRACE_LOW:"mmiset:CloseDualsysCFNRCWin, s_dualsys_cfnrc_win_close_time=%d, action_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11332_112_2_18_2_50_41_96,(uint8*)"dd", s_dualsys_cfnrc_win_close_time,is_action_success);
        
        if (s_dualsys_cfnrc_win_close_time >= MMI_DUAL_SYS_MAX)
        {
            MMK_CloseWin(MMISET_SET_DUALSYS_CFNRC_WIN_ID);
            
            if (s_dualsys_cfnrc_success_num >= MMI_DUAL_SYS_MAX)
            {
                MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
            s_dualsys_cfnrc_win_close_time = 0;
            s_dualsys_cfnrc_success_num = 0;
            s_is_action_dualsys_cfnrc = FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISET_CloseDualsysCFNRCWin: is not dual sys, do not handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11353_112_2_18_2_50_41_97,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 处理dual sim set cfnrc
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCWindow(
                                            MMI_WIN_ID_T          win_id,     // 窗口的ID
                                            MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                            DPARAM                param       // 相应消息的参数
                                            )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MNSS_RelActivingServiceEx(g_mmiset_ss_status.dual_sys);
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open cfnrc win
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL void OpenCFNRCWin(void)
{
#if defined(MMI_MULTI_SIM_SYS_DUAL)
    //judge whether dual sys all call read
    if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
    {
        //judge whether dual sys msisdn all ready
        if (MMIAPIPB_IsPbReady())
        {
            if (MMISET_RegisterDualsysCFNRC(MN_DUAL_SYS_1))
            {
                OpenDualsysCFNRCWin();
            }
            else
            {
                //alert fail
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
        }
        else
        {
            //alert set pb msisdn first
            MMIPUB_OpenAlertWarningWin(TXT_SET_PB_MSISDN);
        }
    }
    else
    {
        //alert sim not ready
        MMIPUB_OpenAlertWarningWin(TXT_SIM_ISNOT_READY);
    }
#endif
}
#endif


#ifdef MMI_DUALMODE_ENABLE 
/*****************************************************************************/
//  Description : MMISET_AppendNetworkTypeListItem
//  Global resource dependence : 
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void AppendNetworkTypeListItem(
                                     MMI_CTRL_ID_T        ctrl_id,
                                     MN_DUAL_SYS_E  dual_sys
                                     )
{
    
    uint16              cur_sel_item = 0;
    MN_PHONE_POWER_TEST_MODE_IND      network_type = 0;/*lint !e64*/
    uint8 testmode[4] =
    {
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID
    };
    //add items to listbox
    /*
    #ifdef PLATFORM_SC7702
    MMIAPISET_AppendListItemByTextId(TXT_SET_W_PREFER, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);    
    #else
    MMIAPISET_AppendListItemByTextId(TXT_SET_TD_PREFER, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);    
    #endif
    MMIAPISET_AppendListItemByTextId(TXT_COMM_GSM_PREFER, STXT_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);   
    MMIAPISET_AppendListItemByTextId(TXT_SET_GSM, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);//@zhaohui modify for cr94108
    #ifdef PLATFORM_SC7702
    MMIAPISET_AppendListItemByTextId(TXT_SET_WCDMA, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    #else
    MMIAPISET_AppendListItemByTextId(TXT_SET_TDSCDMA, TXT_COMMON_OK,  ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    #endif
    */
    MMIAPISET_AppendListItemByTextId(TXT_SET_GPRS_3G_LTE_MODE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO); 
    MMIAPISET_AppendListItemByTextId(TXT_SET_2G_3G_MODE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_2G_MODE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#ifndef WIN32
    MNPHONE_GetTestModeEx(testmode);
#endif
    network_type = testmode[dual_sys];
    cur_sel_item = GetSelectIndexByNetworkType(network_type);
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_sel_item, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_sel_item);
    
}
#endif
/*****************************************************************************/
//  Description : GetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
PUBLIC NETWORK_TYPE_E GetNetworkType(MN_DUAL_SYS_E  dual_sys)//@zhaohui modify for cr94108
{ 
    NETWORK_TYPE_E                  net_type    =   0;/*lint !e64*/
#ifdef MMI_DUALMODE_ENABLE 
    MN_GMMREG_RAT_E        gmmreg_rat  =   MN_GMMREG_RAT_GPRS;
    
#ifndef _WIN32
    gmmreg_rat = MNPHONE_GetConfiguredRATEx(dual_sys);//user selected rat
#endif
    //SCI_TRACE_LOW:"MMISET_GetNetworkType gmmreg_rat is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11467_112_2_18_2_50_41_98,(uint8*)"d",gmmreg_rat);
    switch(gmmreg_rat) 
    {
    case MN_GMMREG_RAT_GPRS:
        net_type = NETWORK_TYPE_GPRS_E;
        break;
        
    case MN_GMMREG_RAT_3G:
        net_type = NETWORK_TYPE_TD_E;
        break;
        
    case MN_GMMREG_RAT_LTE:
        net_type = NETWORK_TYPE_LTE_E;
        break;
        
    case MN_GMMREG_RAT_GPRS_AND_3G:
#ifndef _WIN32
        if(MN_GMMREG_RAT_GPRS == MNPHONE_GetConfiguredDMPreferRATEx(dual_sys))
        {
            net_type = NETWORK_TYPE_DM_G_E;
        }
        else
#endif
        {
            net_type = NETWORK_TYPE_DM_E;
        }
        break;
        
    case MN_GMMREG_RAT_GPRS_3G_LTE:
        net_type = NETWORK_TYPE_GPRS_3G_LTE_E;
        break;
        
    default:
        break;
    }
    
#endif
    return net_type;
}
#ifdef MMI_DUALMODE_ENABLE 
/*****************************************************************************/
//  Description : MMIAPISET_SetNetworkType
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetNetworkType(MN_GMMREG_RAT_E rat)
{
    NETWORK_TYPE_E network_type = NETWORK_TYPE_DM_E;

    //SCI_TRACE_LOW:"MMIAPISET_SetNetworkType rat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11507_112_2_18_2_50_41_99,(uint8*)"d", rat);

    switch (rat)
    {
    case MN_GMMREG_RAT_GPRS:
        network_type = NETWORK_TYPE_GPRS_E;
        break;

    case MN_GMMREG_RAT_GPRS_AND_3G:
        network_type = NETWORK_TYPE_DM_E;
        break;

    case MN_GMMREG_RAT_3G:
        network_type = NETWORK_TYPE_TD_E;
        break;

    default:
        break;
    }

    SetNetworkType(MN_DUAL_SYS_1, network_type, FALSE, FALSE, FALSE);
}



/*****************************************************************************/
//  Description : MMISET_SetNetworkType
//  Global resource dependence : none
//  Author: maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void SetNetworkType(//@zhaohui modify for cr94108
                          MN_DUAL_SYS_E  dual_sys,
                          NETWORK_TYPE_E network_type, //用户的选择
                          BOOLEAN is_resend,        //是否是失败后的重发
                          BOOLEAN only_write_nv,
                          BOOLEAN is_reset_factorysetting
                          )
{       
    MN_PHONE_RAT_INFO_T  rat_info = {0};/*lint !e64*/
    NETWORK_TYPE_E  last_network_type  = GetNetworkType(dual_sys);
    NETWORK_TYPE_E  cur_network_type = NETWORK_TYPE_GPRS_E;
    ERR_MNPHONE_CODE_E  ret  = ERR_MNPHONE_WRITE_ME_FAILURE;//@zhaohui,cr106675
    LOCAL MN_PHONE_RAT_INFO_T s_rat_info_resend = {0};/*lint !e64*/
    LOCAL NETWORK_TYPE_E s_network_type_resend = NETWORK_TYPE_GPRS_E;
    
    //cur_selection_netwok    =   GUILIST_GetCurItemIndex(MMISET_SET_NETWORK_TYPE_CTRL_ID);
    if(!is_resend)
    {
        //SCI_TRACE_LOW:"SetNetworkType cur_selection_netwoktype is %d, only_write_nv is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11553_112_2_18_2_50_41_100,(uint8*)"dd",network_type, only_write_nv);
#ifndef _WIN32
        switch(network_type) 
        {
        case NETWORK_TYPE_DM_E:
            cur_network_type = NETWORK_TYPE_DM_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_AND_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_3G;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;

        case NETWORK_TYPE_DM_G_E:
            cur_network_type = NETWORK_TYPE_DM_G_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_AND_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_GPRS;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;     
            
        case NETWORK_TYPE_GPRS_E:
            cur_network_type = NETWORK_TYPE_GPRS_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
            
        case NETWORK_TYPE_TD_E:
            cur_network_type = NETWORK_TYPE_TD_E;
            rat_info.rat = MN_GMMREG_RAT_3G;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_UNKNOWN;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
        
        case NETWORK_TYPE_GPRS_3G_LTE_E:
            cur_network_type = NETWORK_TYPE_GPRS_3G_LTE_E;
            rat_info.rat = MN_GMMREG_RAT_GPRS_3G_LTE;
            rat_info.dm_prefer_rat = MN_GMMREG_RAT_LTE;
            //ret =     MNPHONE_SetRAT(rat_info,FALSE,FALSE);
            break;
        default:
            
            break;
        }
#endif  
    }
    else
    {
        rat_info.rat = s_rat_info_resend.rat;
        rat_info.dm_prefer_rat = s_rat_info_resend.dm_prefer_rat;
        cur_network_type = s_network_type_resend;
    }
    
    //SCI_TRACE_LOW:"SetNetworkType %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11598_112_2_18_2_50_41_101,(uint8*)"dd",cur_network_type, last_network_type);
    

    
    if (cur_network_type != last_network_type)
    {
#ifdef MMI_DUALMODE_ENABLE
        //avoid frequently switch network
        if (!s_allow_rat_switching)
        {    
            MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
            //not use continue, for we should not allow switch if busy
            return;
        }
#endif
        //note: it's necessary to set gprs attach mode before setting rat.
        MMIAPIPHONE_SetGPRSAttachMode();
        
#ifndef _WIN32
        {
            
#ifndef MMI_NET_TYPE_SET_DEFAULT
            if (MMIAPIPHONE_IsSimCardOk(dual_sys))
            {
                ret = MNPHONE_SetRATEx(dual_sys, rat_info, FALSE, only_write_nv);
                SCI_TRACE_LOW("SetNetworkType, dual_sys=%d, ret= %d, only_write_nv = %d", dual_sys, ret, only_write_nv);
            }
#else
            ret = MNPHONE_SetRATEx(MN_DUAL_SYS_1, rat_info, FALSE, only_write_nv);
            SCI_TRACE_LOW("SetNetworkType,  ret= %d, only_write_nv = %d", ret, only_write_nv);
#endif
        }
#endif

        if (!only_write_nv)
        {
            if(ERR_MNPHONE_NO_ERR == ret)/*lint !e774 */
            {
                if (!is_resend) //设置成功之后，如果是首次发送，记录下发送的参数
                {
                    s_rat_info_resend.rat = rat_info.rat;
                    s_rat_info_resend.dm_prefer_rat = rat_info.dm_prefer_rat;
                    s_network_type_resend = cur_network_type;
                }
                
                MMISET_SetSwitchRatState(TRUE);
                MMIAPISET_SetIsQueryCfu(FALSE, MMI_DUAL_SYS_MAX);
                
                if(!is_reset_factorysetting)
                {
                    //进入网络连接界面
                    MMISET_OpenNetworkConnectWin(dual_sys,
                                                 MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID,
                                                 TXT_PROCESS_SEARCHING_NETWORK,
                                                 MMIPUB_SOFTKEY_NONE,
                                                 MMISET_HandleWaitSearchNetworkWindow);
                }
            }
            else
            {
#ifdef MMI_DUALMODE_ENABLE
                s_allow_rat_switching = TRUE;
#endif
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
        }
    }
    else
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_PLMN_SELECT_COMPLETE);        
#endif
    }
    
}
#endif
#ifdef MMI_DUALMODE_ENABLE 
LOCAL void MMIAPISET_SetTestMode(MN_DUAL_SYS_E dual_sys, MN_PHONE_POWER_TEST_MODE_IND network_type)
{
    BOOLEAN  ret  = FALSE;//@zhaohui,cr106675
    LOCAL MN_PHONE_RAT_INFO_T s_rat_info_resend = {0};/*lint !e64*/
    uint8 testmode[4] =
    {
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID
    };
#ifndef WIN32
    MNPHONE_GetTestModeEx(testmode);
#endif

    if (network_type != testmode[dual_sys])
    {
#ifdef MMI_DUALMODE_ENABLE
        //avoid frequently switch network
        if (!s_allow_rat_switching)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
            return;
        }
#endif
        //note: it's necessary to set gprs attach mode before setting rat.
        MMIAPIPHONE_SetGPRSAttachMode();
#ifndef _WIN32
        testmode[dual_sys] = network_type;
        ret = MNPHONE_SetTestMode(testmode[0], testmode[1], dual_sys);
        if (TRUE == ret)/*lint !e774 */
        {
            MMISET_SetSwitchRatState(TRUE);
            MMINV_WRITE(MMINV_SET_NETWORK_TYPE_MODE, &testmode);
        }
        else
        {
#ifdef MMI_DUALMODE_ENABLE
            s_allow_rat_switching = TRUE;
#endif
            MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
        }
#endif
    }
    else
    {
        SCI_TRACE_LOW("[MMISET] MMIAPISET_SetTestMode Network type not change.");
    }
}

PUBLIC void MMIAPISET_SetTestModeEx(MN_DUAL_SYS_E dual_sys, uint32 network_type)
{
    MMIAPISET_SetTestMode(dual_sys,network_type);
}

/*****************************************************************************/
// Description : reset network when resume factory setting
// Global resource dependence : 
// Author:kun.yu
// Note: 2009-12-28
/*****************************************************************************/
PUBLIC void MMISET_ResetNetWorkType(BOOLEAN only_write_nv)
{
    uint8 testmode[4] =
    {
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID
    };
#ifdef MMI_4G_SUPPORT
        testmode[0] = MN_PHONE_POWER_TD_LTE_FIVE_MODE_ONE_ACTIVE;
        testmode[1] = MN_PHONE_POWER_TD_LTE_FIVE_MODE_ONE_ACTIVE;
#else
        testmode[0] = MN_PHONE_POWER_WCDMA_AND_GSM;
        testmode[1] = MN_PHONE_POWER_WCDMA_AND_GSM;
#endif
    MMINV_WRITE(MMINV_SET_NETWORK_TYPE_MODE, &testmode);
    return;
}

/*****************************************************************************/
// Description : set switch rat state
// Global resource dependence : 
// Author:kun.yu
// Note: 2010-02-04
/*****************************************************************************/
PUBLIC void MMISET_SetSwitchRatState(BOOLEAN state)
{
    is_rat_switching = state;
}

/*****************************************************************************/
// Description : get switch rat state
// Global resource dependence : 
// Author:kun.yu
// Note: 2010-02-04
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSwitchRatState(void)
{
    return is_rat_switching;
}
#endif


/*****************************************************************************/
//  Description : 网络选择后，等待搜网成功,这中间只响应关机键
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleWaitSearchNetworkWindow(
                                                         MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                         MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                         DPARAM             param       // 相应消息的参数
                                                         )
{
    //@zhaohui modify for delete timer
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    static BOOLEAN s_has_closewin_msg = FALSE;//记录是否有退出窗口的消息，当允许退出时，立即响应该消息

    SCI_TRACE_LOW("MMISET_HandleWaitSearchNetworkWindow, win_id = %d, msg_id = %d",win_id,msg_id);

    switch (msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_APP_RED:   
#ifdef MMI_DUALMODE_ENABLE
        s_allow_rat_switching = FALSE;
#endif
        if(0 != s_searchnetwork_space_timer_id)//在等待重新尝试搜网的间隔内，可响应退出
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            s_has_closewin_msg = TRUE;
            MMK_CloseWin(win_id);
        }   
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
        
    case MSG_SET_SPACE_FOR_RESTART_SETRAT:
        if (s_has_closewin_msg)
        {
            s_has_closewin_msg = FALSE;
            MMK_CloseWin(win_id);
        }
        else
        {   
            s_searchnetwork_space_timer_id = MMK_CreateWinTimer(
                MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID,
                MMISET_SEARCHNETWORK_SPACE_TIME, 
                FALSE
                );
        }
        s_allow_rat_switching = TRUE;

        break;
#ifdef MMI_DUALMODE_ENABLE 
    case MSG_TIMER:
        if (s_searchnetwork_space_timer_id == *(uint8*)param)
        {
            s_searchnetwork_space_timer_id = 0;
            MMK_StopTimer(s_searchnetwork_space_timer_id);           
            SetNetworkType(s_cur_dual_sys, NETWORK_TYPE_DM_E, TRUE, FALSE, FALSE);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif
    case MSG_CLOSE_WINDOW:
#ifdef MMI_DUALMODE_ENABLE 
        MMISET_SetSwitchRatState(FALSE);
#endif
        if(0 != s_searchnetwork_space_timer_id)
        {
            s_allow_rat_switching = TRUE;
        }
        MMK_StopTimer(s_searchnetwork_space_timer_id);
        s_searchnetwork_space_timer_id = 0;
        s_has_closewin_msg = FALSE;
        ResetReSwitchRatCount();
        result = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);        
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description :关闭网络搜索的窗口
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_CloseSearchNetworkWin(void)
{
    //关闭网络搜索窗口
    MMK_CloseWin(MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID);
}

/*****************************************************************************/
//  Description :判断窗口MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID是否打开
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SearchNetworkWinisOpen(void)
{
    return MMK_IsOpenWin(MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID);
}
/*****************************************************************************/
//  Description :显示搜网结果 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SearchNetworkResultDisp(BOOLEAN   result)
{
    
    if (MMIAPISET_SearchNetworkWinisOpen())
    {         
        if (result)
        {
            //SCI_TRACE_LOW:"ResultDisp success: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11802_112_2_18_2_50_42_102,(uint8*)"d", result);
            MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        }
        else
        {
            //SCI_TRACE_LOW:"ResultDisp fail: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11807_112_2_18_2_50_42_103,(uint8*)"d", result);
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        }
        MMIAPISET_CloseSearchNetworkWin();
    }
}

/*****************************************************************************/
//  Description :显示设置优先网络结果 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetPreferRatResultDisp(BOOLEAN result)
{
#ifdef MMI_DUALMODE_ENABLE 
    MMISET_SetSwitchRatState(FALSE);
#endif

    s_allow_rat_switching = TRUE;

    if (MMIAPISET_SearchNetworkWinisOpen())
    {         
        if (result)
        {
            //SCI_TRACE_LOW:"ResultDisp success: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11827_112_2_18_2_50_42_104,(uint8*)"d", result);
            MMIPUB_OpenAlertSuccessWin(TXT_PLMN_SELECT_COMPLETE);
        }
        else
        {
            //SCI_TRACE_LOW:"ResultDisp fail: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11832_112_2_18_2_50_42_105,(uint8*)"d", result);
            MMIPUB_OpenAlertWarningWin(TXT_PLMN_SELECT_FAIL);
        }
        MMIAPISET_CloseSearchNetworkWin();
    }   
}


/*****************************************************************************/
//  Description : 是否包含回車
//  Global resource dependence : 
//  Author: bruce.chi
//  Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsIncludeReturnChar(
                                             const wchar    * name_ptr, 
                                             uint16           wstr_len
                                             )
{/*lint !e18*/
    uint8       invalid_char_r    =   '\r'; //换行
    uint8       invalid_char_n    =   '\n'; //回车
    uint16      i               =   0;    
    
    if (name_ptr == PNULL || wstr_len == 0)
    {
        return FALSE;
    }
    
    for(i=0; i<wstr_len; i++)
    {
        
        if ((*(name_ptr+i) == invalid_char_n) || (*(name_ptr+i) == invalid_char_r))
        {
            return TRUE;
        }
    }
    
    return FALSE;
}




/*****************************************************************************/
//  Description : open wizard cfnrc win
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_OpenWizardCFNRCWin(void)
{
#ifdef MMI_MULTI_SIM_SYS_DUAL
    MMISET_SelectSIMFunc(WizardCFNRCSIMSelectCallback, MMISET_APPEND_SYS_OK ,PNULL);
#endif
    return FALSE;
    
}

/*****************************************************************************/
//  Description : is open dual sys CFNRC
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsOpenDualSysCfnrc(void)
{
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    return s_is_action_dualsys_cfnrc;
#else
    return FALSE;
#endif
}





/*****************************************************************************/
//  Description : MMIAPISET_HandlePreferRatRsp
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandlePreferRatRspResult(
                                                       BOOLEAN rsp_result
                                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"MMIAPISET_HandlePreferRatRsp rsp_result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11914_112_2_18_2_50_42_106,(uint8*)"d", rsp_result);

    if (rsp_result)//设置成功
    {
        SetPreferRatResultDisp(rsp_result);
        MMIAPIPHONE_SetGPRSAttachMode();
#ifdef MMI_GPRS_SUPPORT
        MMIAPIPDP_ResetPdp(MN_DUAL_SYS_1);
#endif
    }

    if (MMIAPISET_SearchNetworkWinisOpen())
    {  
        MMIDEFAULT_TurnOnBackLight();

        if (!rsp_result)//设置失败
        {
            if (s_re_switchrat_count < MMISET_SETRAT_MAX_NUM)//切网失败，最多允许重试7次
            {
                s_re_switchrat_count ++;
                MMK_SendMsg(MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID, MSG_SET_SPACE_FOR_RESTART_SETRAT, PNULL);
            }   
            else
            {
                //模式切换失败
                // MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                //MMIAPISET_CloseSearchNetworkWin();
                SetPreferRatResultDisp(rsp_result);
            }
        }
    }
    else
    {
#ifdef MMI_DUALMODE_ENABLE 
        MMISET_SetSwitchRatState(FALSE);
#endif
       s_allow_rat_switching = TRUE; 
    }
    
    s_allow_rat_switching = TRUE; 

    return (result);
    
}

/*****************************************************************************/
//  Description :网络搜索的窗口，判断是否是在切网状态
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsinSwitchRatState(void)
{
    if (0 != s_searchnetwork_space_timer_id)
    {
        //SCI_TRACE_LOW:"IsinSwitchRatState is TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11964_112_2_18_2_50_42_107,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"IsinSwitchRatState is FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_11969_112_2_18_2_50_42_108,(uint8*)"");
        return FALSE;
    }    
}

/********************************************************************************
NAME:           ResetReSwitchRatCount
DESCRIPTION:    
PARAM IN:       None
PARAM OUT:      
AUTHOR:     zhaohui
DATE:           2009.1.5
********************************************************************************/
LOCAL void ResetReSwitchRatCount(void)
{
    s_re_switchrat_count = 0;
}

/*****************************************************************************/
//  Description :MMIAPISET_IsWizardCfnrc
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsWizardCfnrc(void)
{
    return s_is_wizard_cfnrc;
}





/*****************************************************************************/
// Description : 开机设置飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-8
/*****************************************************************************/
PUBLIC void MMIAPISET_SetFlyModeOpenPhone(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_FLY_MODE_OPEN_PHONE_WIN_TAB,PNULL);
}

/*****************************************************************************/
// Description : to handle set fly mode window msg when open phone
// Global resource dependence : 
// Author:Ming.Song
// Note: 2007-8-8
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFlyModeOpenPhoneWindow(
                                                 MMI_WIN_ID_T      win_id,
                                                 MMI_MESSAGE_ID_E  msg_id,
                                                 DPARAM            param
                                                 )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    BOOLEAN             is_fly_mode_on  =   FALSE;
    MMI_CTRL_ID_T       ctrl_id         =   MMISET_FLY_MODE_OPEN_LIST_CTRL_ID;
    uint16              cur_selection   =   0;    
#if !defined(MMI_PDA_SUPPORT)
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_BOTH_RECT_T win_rect = MMITHEME_GetFullScreenBothRect();
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_fly_mode_on = MMIAPISET_GetFlyMode();
        GUILIST_SetMaxItem(ctrl_id,2, FALSE );
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_fly_mode_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_fly_mode_on);
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#if !defined(MMI_PDA_SUPPORT)
    case MSG_FULL_PAINT:
        if (!MMITHEME_IsMainScreenLandscape())
        {
            GUI_FillRect(&lcd_dev_info, win_rect.v_rect, MMI_BLACK_COLOR);
        }
        else
        {
            GUI_FillRect(&lcd_dev_info, win_rect.h_rect, MMI_BLACK_COLOR);
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        
        is_fly_mode_on = (BOOLEAN)cur_selection;
        
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_FLYMODE_FINISH);
#if 0//for StartupCtrl
        MMIAPIPHONE_NormalInitAfterPwd();
#endif
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}





/*****************************************************************************/
//  Description :get voice mail address record id
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetVMRecordId(void)
{
    MMI_MENU_GROUP_ID_T          group_id   =   0;
    MMI_MENU_ID_T                menu_id    =   0;
    uint8                        record_id  =   0; 
    
    if (!MMK_IsOpenWin(MMISET_SET_OPEN_DIVERT_WIN_ID))
    {
        return record_id;
    }
    
    //to get the menu id
    GUIMENU_GetId( MMISET_VOICE_NUM_CTRL_ID, &group_id, &menu_id);           
    switch ( menu_id )
    {
    case ID_SET_SS_LINE_VMN:
        record_id = 1;
        break;
#ifdef MMI_CSP_SUPPORT    
    case ID_SET_SS_LINE1_VMN:   
        record_id = 1;
        break;          
    case ID_SET_SS_LINE2_VMN:
        record_id = 2;
        break;
#endif //MMI_CSP_SUPPORT        
    case ID_SET_SS_FAX_VMN:
        record_id = 3;
        break;
    case ID_SET_SS_DATA_VMN:
        record_id = 4;
        break;          
    default:
        break;
    }
    
    return record_id;
}




/*****************************************************************************/
//  Description : open network connect win
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenNetworkConnectWin(MN_DUAL_SYS_E dual_sys,
                                        MMI_WIN_ID_T win_id,
                                        MMI_TEXT_ID_T text_id,
                                        MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                                        MMIPUB_HANDLE_FUNC      handle_func     //handle function
                                    )
{
    MMI_STRING_T connect_net_text = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
    MMI_STRING_T sim_string_ptr = {0};
#endif

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    MMI_GetLabelTextByLang(text_id, &connect_net_text);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    sim_name = MMIAPISET_GetSimName(dual_sys);

    sim_string_ptr.wstr_len = sim_name.wstr_len;
    sim_string_ptr.wstr_ptr = sim_name.wstr_arr;

    MMIPUB_OpenWaitWin(2,&sim_string_ptr,&connect_net_text,PNULL,win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,softkey_style,handle_func);
#else
    MMIPUB_OpenWaitWin(1,&connect_net_text,PNULL,PNULL,win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,softkey_style,handle_func);
#endif
}

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
//  Description : to handle select line msg
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM               param
                                           )
{
    MMI_RESULT_E        recode      =   MMI_RESULT_TRUE;
    uint32              dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_LINE_SELECT_LIST_CTRL_ID,MMISET_MAX_LINE, FALSE );//max item 2
        MMISET_AppendLineListItem(dual_sys,MMISET_LINE_SELECT_LIST_CTRL_ID);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,MMISET_LINE_SELECT_LIST_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16 cur_selection = 0;
            MMI_CTRL_ID_T ctrl_id = MMISET_LINE_SELECT_LIST_CTRL_ID;
            //get the selected item
            GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
            if(!MMISET_SetSelectLine(dual_sys,(MMISET_LINE_TYPE_E)cur_selection))
            {
                //fail
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            }
        }
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

/*****************************************************************************/
// 	Description : to handle lock line msg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLockLineWindow(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E 	msg_id,
                                           DPARAM 		        param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_LOCK_LINE_LIST_CTRL_ID;
    LOCAL MN_DUAL_SYS_E s_dual_sys = MN_DUAL_SYS_1;
    uint16  cur_selection = 0;
    BOOLEAN is_lock_line = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );//max item 2
        s_dual_sys = MMIAPIPHONE_GetCurHandlingPinSimID(); //???????,???pin2???????????
        //add items to listbox
        MMIAPISET_AppendListItemByTextId( TXT_CLOSE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId( TXT_OPEN, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        
        //get current line
        cur_selection = (MMISET_GetLineLockStatus(s_dual_sys) == MMISET_LINE_IS_LOCK)?1:0;
        
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex( ctrl_id);
        if( 1 == cur_selection )
        {
            is_lock_line = TRUE;
        }
        if(MMI_RESULT_TRUE == MMISET_LockLine(s_dual_sys, is_lock_line))
        {
            //??????
            MMI_STRING_T wait_text = {0};
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
        break;

    case MSG_GET_FOCUS:
        cur_selection = (MMISET_GetLineLockStatus(s_dual_sys) == MMISET_LINE_IS_LOCK)?1:0;
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
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

/*****************************************************************************/
// Description : set line lock
// Global resource dependence : 
// Author: samboo.shen
// Note:
/*****************************************************************************/
LOCAL void  SetLineLock(void)
{
   // ????????
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMISET_SIM_SELECT_TYPE_E select_type = 0;

    if (MMIAPISET_GetFlyMode())
    {
        MMIPHONE_AlertSimNorOKStatus();
        return ;
    }

    select_type = MMISET_LINE_LOCK;
    sim_num = MMIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_LOCK_LINE_WIN_TAB, (ADD_DATA)sim_sys);
    }
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
    }
#else
    if (MMIAPISET_GetFlyMode())
    {
        MMIPHONE_AlertSimNorOKStatus();
        return ;
    }
    
    if (MMIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
    {
        MMK_CreateWin((uint32*)MMISET_LOCK_LINE_WIN_TAB,PNULL);
        
    }
    else
    {
        MMIPHONE_AlertSimNorOKStatus();
    }
#endif
    
    return;
}
/*****************************************************************************/
// Description : ????
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_SetLineInfo(void)
{
    MMI_WIN_ID_T                    win_id      =   0;
    
    // ????????
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMISET_SIM_SELECT_TYPE_E select_type = 0;

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return win_id;
    }

    select_type = MMISET_LINE_SELECT;
    sim_num = GetSelectLineVisibleSimNum(&sim_sys, 1);//MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMISET_SELECT_LINE_WIN_TAB, (ADD_DATA)sim_sys);
        win_id = MMISET_LINE_SELECT_WIN_ID;
    }
    else
    {
        MMIAPISET_OpenUssdSelectWin(select_type);
        win_id = MMISET_COM_SELECT_SIM_WIN_ID;
    }
#else
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return win_id;
    }
    
    if (MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
    {
        MMK_CreateWin((uint32*)MMISET_SELECT_LINE_WIN_TAB,PNULL);
        
        win_id = MMISET_LINE_SELECT_WIN_ID;
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
#endif
    
    return win_id;
}
/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSelectLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN result = FALSE;
#ifdef MMI_CSP_SUPPORT    
    if(MMISET_IsAlsValid(dual_sys))
    {
        if(!MMISET_IsCspValid(dual_sys, 1))
        {
            result = TRUE;
        }
        else
        {
            MMISET_LINE_LOCK_STATUS_E line_lock_status = MMISET_GetLineLockStatus(dual_sys);
            result = !(MMISET_LINE_LOCK_INVALID == line_lock_status);
        }
    }
    else
    {
        result = FALSE;
    }
#endif //MMI_CSP_SUPPORT
    return result;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLockLineVisibleByDualSys(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN result = FALSE;
#ifdef MMI_CSP_SUPPORT        
    if(MMISET_IsAlsValid(dual_sys))
    {
        //if csp2 not valid, hide line lock option(because csp2 dynamic file save the line lock flag)
        if(!MMISET_IsCspValid(dual_sys, 1))
        {
            result = FALSE;
        }
        else
        {
            MMISET_LINE_LOCK_STATUS_E line_lock_status = MMISET_GetLineLockStatus(dual_sys);
            result = !(MMISET_LINE_LOCK_INVALID == line_lock_status);
        }
    }
    else
    {
        result = FALSE;
    }
#endif //MMI_CSP_SUPPORT    
    return result;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetSelectLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (IsSelectLineVisibleByDualSys(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetLockLineVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (IsLockLineVisibleByDualSys(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallFowardVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (IsSSMenuItemVisible(i, CSP_MENU_CF, CSP_MENU_OPER_NULL))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide als menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCallBarVisibleSimNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (IsSSMenuItemVisible(i, CSP_MENU_CB, CSP_MENU_OPER_NULL))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : according to cphs, csp & als data in sim, to judge display or hide cf cb menu item
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSSMenuItemVisible(MN_DUAL_SYS_E dual_sys, CSP_MENU_SS_TYPE_E ss_type, CSP_MENU_SS_OPER_E ss_oper)
{
    BOOLEAN result = TRUE;

#ifdef MMI_CSP_SUPPORT
    if (MMISET_IsAlsValid(dual_sys))
    {
        MN_BEARER_CAPABILITY_TYPE_E line = 0;
        if (MMISET_IsCspValid(dual_sys, 1))
        {
            line = (MN_BEARER_CAPABILITY_TYPE_E)MMIAPISET_GetCallLineInfo(dual_sys); //if csp2 valid, get line from sim
        }
        else
        {
            line = MN_BEARER_CAPABILITY_LINE_1;//if csp2 invalid, select line1
        }
    
        switch(ss_type)
        {
            case CSP_MENU_CF:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert(dual_sys, line); 
                            break;

                        case CSP_MENU_OPER_CANCEL_ALL:
                            result = MNSIM_CPHS_CallOffering_CancelAllVoiceDivert(dual_sys, line);
                            break; 

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_CFU:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_CFB:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;                    
                    }
                }
                break;

            case CSP_MENU_CFNRY:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;                    
                    }
                }
                break;

            case CSP_MENU_CFNRC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Activate(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_ACTIVE_WITH_PB:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Phonebook(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_VM:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_VoicemailNumber(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE_WITH_NO:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Number(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Cancel(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;                    
                    }
                }
                break;

            case CSP_MENU_CB:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_BarringOfCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL_ALL:
                            result = MNSIM_CPHS_CallRestriction_ClearAllBarrings(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_MODIFY_PASSWORD:
                            result = MNSIM_CPHS_CallRestriction_ChangeBarringPassword(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_BAOC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_OutgoingCalls_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;
                    }
                }
                break;

            case CSP_MENU_BAIC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_IncomingCalls_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            case CSP_MENU_BICROAM:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            case CSP_MENU_BOIC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_AllInternationalCalls_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            case CSP_MENU_BOIC_EXHC:
                {
                    switch(ss_oper)
                    {
                        case CSP_MENU_OPER_NULL:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_ACTIVE:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_Activate(dual_sys, line);
                            break;

                        case CSP_MENU_OPER_CANCEL:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_Cancel(dual_sys, line);
                            break;
                            
                        case CSP_MENU_OPER_INTEROGATE:
                            result = MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_GetStatus(dual_sys, line);
                            break;

                        default:
                            result = TRUE;
                            break;

                    }
                }
                break;

            default:
                result = TRUE;
                break;
        }
    }
    else
#endif //MMI_CSP_SUPPORT        
    {
        result = TRUE;
    }

    SCI_TRACE_LOW("MMISET IsSSMenuItemVisible dual_sys:%d, type:%d, oper:%d,result:%d", dual_sys, ss_type, ss_oper, result);    
    return result;
}
#endif //MMI_CSP_SUPPORT


#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
//  Description : handle network setting window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkSettingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_NETWORK_SETTING_CTRL_ID;

    //SCI_TRACE_LOW:"HandleNetworkSettingWindow,msg_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12182_112_2_18_2_50_43_109,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
        MMK_SetAtvCtrl(win_id,MMISET_SET_NETWORK_SETTING_CTRL_ID);
        break;        
       
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_MENU_GROUP_ID_T     group_id     = 0;
            MMI_MENU_ID_T             menu_id   = 0;
            GUIMENU_GetId( MMISET_SET_NETWORK_SETTING_CTRL_ID, &group_id, &menu_id);     
            
            switch (menu_id)
            {   
            case ID_SET_NETWORK_SETTING_SEL_MENU: /// net work selection                    
                MMK_CreateWin((uint32*)MMISET_SET_SELECT_NETWORK_WIN_TAB, (ADD_DATA)dual_sys);
                break;

            case ID_SET_NETWORK_SETTING_PREFER_MENU: ///  偏好网络
                MMK_CreateWin((uint32*)MMISET_SET_DISP_PREFER_NETWORK_WIN_TAB, (ADD_DATA)dual_sys);
                break;

            default:
                break;
            }
        }
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

/*****************************************************************************/
//  Description : handle display prefer network list window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDispPreferNetworkListWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    MMI_CTRL_ID_T ctrl_id = MMISET_NETWORK_PREFER_PLMN_LIST_CTRL_ID;
    
    //静态变量，在list窗口FULL PAINT的时候需要更新list的数据。.
    static MN_PHONE_PLMN_SEL_LIST_T prefer_network_list = {0};

    //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,msg=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12247_112_2_18_2_50_43_110,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
            GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);

#if !defined MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

            MMK_SendMsg(win_id, MSG_SET_LOAD_PREFER_PLMN_LIST, PNULL);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break; 
        
    case MSG_SET_LOAD_PREFER_PLMN_LIST:
        {
            uint16 max_list_num =0;
            
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);
            max_list_num = prefer_network_list.plmn_validate_num;
            
            //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,max_list_num"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12272_112_2_18_2_50_43_111,(uint8*)"",max_list_num);

            InitPlmnListCtrl(ctrl_id, max_list_num);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
    
            if( PNULL != need_item_data_ptr )
            {
                index = need_item_data_ptr->item_index;                    
                MMIAPIPHONE_AppendPreferPlmnList(dual_sys, ctrl_id, prefer_network_list.plmn_arr, index);
            }
        }
        break;            
        
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
            uint16 total_item = GUILIST_GetTotalItemNum(ctrl_id);
            
            //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,total_item_num=%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12304_112_2_18_2_50_43_112,(uint8*)"d",total_item);
            
            if (total_item > 0)
            {
                uint16 cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);

                SetCurPreferNetworkIndex(cur_item_index);
            }

            MMK_CreateWin((uint32*)MMISET_PREFER_NETWORK_OPT_MENU_WIN_TAB, (ADD_DATA)dual_sys);
        }
        break;

#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_LIST_LONGOK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16 total_item = GUILIST_GetTotalItemNum(ctrl_id);
            
            //SCI_TRACE_LOW:"HandleDispPreferNetworkListWinMsg,total_item_num=%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12325_112_2_18_2_50_43_113,(uint8*)"d",total_item);
            
            if (total_item > 0)
            {
                uint16 cur_item_index = GUILIST_GetCurItemIndex(ctrl_id);

                SetCurPreferNetworkIndex(cur_item_index);
            }

            MMK_CreateWin((uint32*)MMISET_PREFER_NETWORK_LONGPENOK_OPT_MENU_WIN_TAB, (ADD_DATA)dual_sys);
        }
        break;
#endif
#endif
        
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

/*****************************************************************************/
// 	Description : HandleDeletePreferPlmnQueryWindow
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeletePreferPlmnQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMISET_PREFER_NET_DEL_QUERY_WIN_ID;
    MN_PHONE_PLMN_SEL_LIST_T prefer_network_list = {0};
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {  
            //删除的执行!!!!
            uint32 i = 0;
            uint16 cur_prefer_net = GetCurPreferNetworkIndex();

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

            if (PNULL != win_info_ptr)
            {
                dual_sys = (MN_DUAL_SYS_E)win_info_ptr->user_data;
            }

            //SCI_TRACE_LOW:"HandleDeletePreferPlmnQueryWindow,OK_Delete_Item= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12387_112_2_18_2_50_43_114,(uint8*)"d",cur_prefer_net);
            
            //关闭询问窗口
            MMIPUB_CloseQuerytWin(&query_win_id);                
            
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);

            if (cur_prefer_net < prefer_network_list.plmn_validate_num)
            {
                if(prefer_network_list.plmn_max_num - 1 == cur_prefer_net ) //恰好是最后一项
                {
                    SCI_MEMSET(&prefer_network_list.plmn_arr[cur_prefer_net],0x00,sizeof(MN_PLMN_T));
                }
                else
                {
                    for(i = cur_prefer_net; i < prefer_network_list.plmn_validate_num;i++)
                    {
                        SCI_MEMCPY(&prefer_network_list.plmn_arr[i], &prefer_network_list.plmn_arr[i+1], sizeof(MN_PLMN_T));
                    }
                    SCI_MEMSET(&prefer_network_list.plmn_arr[prefer_network_list.plmn_validate_num - 1],0x00,sizeof(MN_PLMN_T));
                }
                prefer_network_list.plmn_validate_num--;
                
                //SCI_TRACE_LOW:"HandleDeletePreferPlmnQueryWindow,Delete Plmn: mcc=%d,mnc=%d,mnc_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12411_112_2_18_2_50_43_115,(uint8*)"ddd",prefer_network_list.plmn_arr[cur_prefer_net].mcc,prefer_network_list.plmn_arr[cur_prefer_net].mnc,prefer_network_list.plmn_arr[cur_prefer_net].mnc_digit_num);

                MMIAPISET_SavePreferNetworkList(dual_sys,&prefer_network_list);
            }
        }

        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : handle prefer network options menu msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkOptionMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_PREFER_NETWORK_OPT_MENU_CTRL_ID;
    MN_PHONE_PLMN_SEL_LIST_T prefer_network_list = {0};
        
    MMI_WIN_ID_T query_win_id = MMISET_PREFER_NET_DEL_QUERY_WIN_ID;

    //SCI_TRACE_LOW:"HandlePreferNetworkOptionMsg,msg_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12449_112_2_18_2_50_43_116,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);
            
            //SCI_TRACE_LOW:"HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12457_112_2_18_2_50_43_117,(uint8*)"dd",prefer_network_list.plmn_validate_num, prefer_network_list.plmn_max_num);
             
            if (prefer_network_list.plmn_validate_num == prefer_network_list.plmn_max_num)//plmn list full
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list
            if ( 0 == prefer_network_list.plmn_validate_num )
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list or prefer list is only one
            if ( prefer_network_list.plmn_validate_num <= 1)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#endif
#endif
            }
            
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;

    case MSG_GET_FOCUS:
        {
            prefer_network_list = MMIAPISET_LoadPreferNetworkList(dual_sys);
            
             //SCI_TRACE_LOW:"HandlePreferNetworkOptionMsg,total_list_num=%d,plmn_max_num=%d"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12497_112_2_18_2_50_43_118,(uint8*)"dd",prefer_network_list.plmn_validate_num, prefer_network_list.plmn_max_num);
             
            if (prefer_network_list.plmn_validate_num == prefer_network_list.plmn_max_num)//plmn list full
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_ADD_PREFER_NETWORK_FROM_LIST,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CREATE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list
            if ( 0 == prefer_network_list.plmn_validate_num )
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_DELETE_PREFER_NETWORK,TRUE);
#endif
#endif
            }

            //no prefer list or prefer list is only one
            if ( prefer_network_list.plmn_validate_num <= 1)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#ifdef MMI_PDA_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
                MMIAPICOM_EnableGrayed(win_id,MENU_SET_PREFER_NETWORK_LONGPENOK_OPT,ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,TRUE);
#endif
#endif
            }
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T menu_id = 0;
            GUIMENU_GetId( ctrl_id, &group_id, &menu_id);     
            
            switch (menu_id)
            {   
            case ID_SET_ADD_PREFER_NETWORK_FROM_LIST:
                {
                    MMK_CreateWin((uint32*)MMISET_NETWORK_PLMN_LIST_WIN_TAB, (ADD_DATA)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

            case ID_SET_CREATE_PREFER_NETWORK:
                {
                    MMK_CreatePubFormWin((uint32*)MMISET_NETWORK_NEW_PLMN_ITEM_WIN_TAB, (ADD_DATA)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

            case ID_SET_CHANGE_PREFER_NETWORK_PRIORITY:
                {
                    MMK_CreatePubFormWin((uint32*)MMISET_NETWORK_CHANGE_PRIORITY_WIN_TAB, (ADD_DATA)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

             case ID_SET_DELETE_PREFER_NETWORK:
                {                            
                    // 弹出delete提示窗口
                    MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDeletePreferPlmnQueryWindow, (uint32)dual_sys);
                    MMK_CloseWin(win_id);
                }
                break;

            default:
                break;
            }
        }
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

/*****************************************************************************/
//  Description : HandlePlmnListDispWinMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlmnListDispWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    MMI_CTRL_ID_T ctrl_id = MMISET_NETWORK_PLMN_LIST_CTRL_ID;
    
    //SCI_TRACE_LOW:"HandlePlmnListDispWinMsg,msg_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12603_112_2_18_2_50_44_119,(uint8*)"d",msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
            uint16 max_item_num = MMIAPIPHONE_GetPlmnListNum();    
            
            GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
            
            InitPlmnListCtrl(ctrl_id,max_item_num);
            
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;        
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
    
            if( PNULL != need_item_data_ptr )
            {
                index = need_item_data_ptr->item_index;
                MMIAPIPHONE_AppendPlmnList(dual_sys, ctrl_id,index);
            }
        }
        break;
       
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            //选中的某个item 需要加入偏好网络的列表中。
            uint16 item_data = 0;                    
            MN_PLMN_T plmn_data = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);

            uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            item_data = cur_index+1;

            // 获取这个item项，插入优选网络列表
            //如下接口可以获取mcc/mnc。
            MMIAPIPHONE_GetPlmnInfoByIndex(&plmn_data,item_data);  

            if((prefer_network.plmn_validate_num >= prefer_network.plmn_max_num)
                 ||(!isPlmnValid(dual_sys,plmn_data)))
            {
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_SET_PREFER_NETWORK_ADD_ERROR);
            }
            else
            {
                MMK_CloseWin(win_id);
                
                prefer_network.plmn_arr[prefer_network.plmn_validate_num].mcc = plmn_data.mcc;
                prefer_network.plmn_arr[prefer_network.plmn_validate_num].mnc = plmn_data.mnc;
                prefer_network.plmn_arr[prefer_network.plmn_validate_num].mnc_digit_num = plmn_data.mnc_digit_num;
                prefer_network.plmn_validate_num++;

                //SCI_TRACE_LOW:"HandlePlmnListDispWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12668_112_2_18_2_50_44_120,(uint8*)"ddd",plmn_data.mcc,plmn_data.mnc,plmn_data.mnc_digit_num);

                MMIAPISET_SavePreferNetworkList(dual_sys, &prefer_network);
            }
        }
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

/*****************************************************************************/
//  Description : HandleCreatePlmnItemWinMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCreatePlmnItemWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    const wchar plmn_info[] = L"MCC/MNC"; //不需要引入字串，该字段显示具有通用性。

    //SCI_TRACE_LOW:"HandleCreatePlmnItemWinMsg,msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12702_112_2_18_2_50_44_121,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T label_text  = {0};
            MMI_STRING_T plmn_priority_text = {0};
            MMI_STRING_T priority_text = {0};
            uint8 buf[4] = {0};               
            wchar plmn_priority[4] ={0};               
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = prefer_network.plmn_validate_num + 1;
            uint8 priority_range[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            uint16 priority_range_len = 0;
            wchar temp_ch1 = L'(';
            wchar temp_ch2 = L')';

            MMIAPICOM_Int2Str(prefer_net_num, buf, 4);
            MMI_STRNTOWSTR(plmn_priority, 4, buf, 4, 4);

            plmn_priority_text.wstr_ptr = plmn_priority;
            plmn_priority_text.wstr_len = sizeof(plmn_priority)/sizeof(wchar);

            label_text.wstr_ptr = (wchar *)plmn_info;
            label_text.wstr_len = sizeof(plmn_info)/sizeof(wchar);

            GUILABEL_SetText(MMISET_NETWORK_PLMN_LABEL_CTRL_ID, &label_text, FALSE); 

            MMI_GetLabelTextByLang(TXT_COMMON_PRIORITY, &priority_text);

            sprintf((char *)priority_range, "1-%d", prefer_net_num);
            priority_range_len = strlen((char *)priority_range);

            label_text.wstr_len = (priority_text.wstr_len + 1 + priority_range_len + 1);
            label_text.wstr_ptr = SCI_ALLOC_APPZ((label_text.wstr_len + 1)*sizeof(wchar));

            if (PNULL == label_text.wstr_ptr)
            {
                break;
            }

            MMI_WSTRNCPY(label_text.wstr_ptr, label_text.wstr_len, priority_text.wstr_ptr, priority_text.wstr_len, priority_text.wstr_len);
            MMI_WSTRNCPY(label_text.wstr_ptr + priority_text.wstr_len, label_text.wstr_len, &temp_ch1, 1, 1);
            MMI_STRNTOWSTR(label_text.wstr_ptr + priority_text.wstr_len + 1, label_text.wstr_len, priority_range, priority_range_len, priority_range_len);
            MMI_WSTRNCPY(label_text.wstr_ptr + priority_text.wstr_len + 1 + priority_range_len, label_text.wstr_len, &temp_ch2, 1, 1);

            GUILABEL_SetText(MMISET_NETWORK_PLMN_PRIORITY_LABEL_CTRL_ID, &label_text, FALSE);

            SCI_FREE(label_text.wstr_ptr);

            GUIEDIT_SetStyle(MMISET_NETWORK_PLMN_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetStyle(MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            //没有列表的时候，不需要设置优先级
            if (0 == prefer_network.plmn_validate_num)
            {
                GUIFORM_SetChildDisplay(MMISET_NETWORK_PLMN_FORM_CTRL_ID, MMISET_NETWORK_PLMN_FORM2_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            }
            else
            {
                GUIFORM_SetStyle(MMISET_NETWORK_PLMN_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
                GUIFORM_SetStyle(MMISET_NETWORK_PLMN_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
            }

            //优先级
            GUIEDIT_SetString(MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID, plmn_priority_text.wstr_ptr, plmn_priority_text.wstr_len);

            MMK_SetAtvCtrl(win_id, MMISET_NETWORK_PLMN_EDIT_CTRL_ID);
        }
        break;

    case MSG_CTL_MIDSK:
    //case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            // 将编辑好的plmn item加入偏好网络列表中。
            MMI_STRING_T edit_str = {0};
            MMI_STRING_T priority_str = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = prefer_network.plmn_validate_num + 1;
            MN_PLMN_T plmn = {0};
            uint8 buf[MMISET_PLMN_DIGITAL_MAX_LEN + 1] = {0};               
            char mcc_str[4] = {0};
            char mnc_str[4] = {0};
            char priority_num[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            uint16 plmn_priority = 0;
            
            GUIEDIT_GetString(MMISET_NETWORK_PLMN_EDIT_CTRL_ID, &edit_str);
            GUIEDIT_GetString(MMISET_NETWORK_PLMN_PRIORITY_CODE_EDIT_CTRL_ID, &priority_str);
            
            MMI_WSTRNTOSTR(buf, MMISET_PLMN_DIGITAL_MAX_LEN, edit_str.wstr_ptr, MMISET_PLMN_DIGITAL_MAX_LEN, edit_str.wstr_len);

            strncpy(mcc_str, (char*)buf, 3);
            strcpy(mnc_str, (char*)&(buf[3]));
            
            plmn.mcc = atoi(mcc_str);
            plmn.mnc = atoi(mnc_str);

            MMI_WSTRNTOSTR(priority_num, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_ptr, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_len); /*lint !e64*/
            plmn_priority = atoi(priority_num);

            if((edit_str.wstr_len < MMISET_PLMN_DIGITAL_MAX_LEN -1) //mnc len may be 2 or 3
              ||(prefer_network.plmn_validate_num >= prefer_network.plmn_max_num)
              ||(!isPlmnValid(dual_sys,plmn) ))
            {                   
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_SET_PREFER_NETWORK_ADD_ERROR);
            }
            else if (plmn_priority > prefer_net_num || plmn_priority < 1)
            {
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_SET_PREFER_NETWORK_ADD_ERROR);
            }
            else
            {
                uint32 i = 0;
                MN_PHONE_PLMN_SEL_LIST_T tmp_prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);

                plmn.mnc_digit_num = edit_str.wstr_len - 3;

                for (i = 0; i < plmn_priority - 1; i++)/*lint !e574*/
                {
                    tmp_prefer_network.plmn_arr[i] = prefer_network.plmn_arr[i];
                }
                tmp_prefer_network.plmn_arr[i].mcc = plmn.mcc;
                tmp_prefer_network.plmn_arr[i].mnc = plmn.mnc;
                tmp_prefer_network.plmn_arr[i].mnc_digit_num = plmn.mnc_digit_num;

                i += 1;

                for (; i < prefer_net_num; i++)
                {
                    tmp_prefer_network.plmn_arr[i] = prefer_network.plmn_arr[i - 1];
                }
                tmp_prefer_network.plmn_validate_num++;
                
                //SCI_TRACE_LOW:"HandleCreatePlmnItemWinMsg,Add Plmn: mcc=%d,mnc=%d,mnc_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12842_112_2_18_2_50_44_122,(uint8*)"ddd",plmn.mcc,plmn.mnc,plmn.mnc_digit_num);
                
                MMIAPISET_SavePreferNetworkList(dual_sys, &tmp_prefer_network);

                MMK_CloseWin(win_id);
            }
        }
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

/*****************************************************************************/
//  Description : handle plmn list display window msg.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetworkPriorityChangeWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);    
    uint16 cur_index = GetCurPreferNetworkIndex();
    uint32 index_priority = 0; //大于0
    
    //SCI_TRACE_LOW:"HandlePreferNetworkPriorityChangeWinMsg,msg = 0x%x,cur_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12879_112_2_18_2_50_44_123,(uint8*)"dd",msg_id,cur_index);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T title_text  = {0};
            MMI_STRING_T plmn_priority_text = {0};
            MMI_STRING_T priority_text = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = prefer_network.plmn_validate_num;
            uint8 buf[4] = {0};
            wchar plmn_priority[4] ={0};
            uint8 priority_range[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            uint16 priority_range_len = 0;
            wchar temp_ch1 = L'(';
            wchar temp_ch2 = L')';

            index_priority = cur_index + 1;

            MMIAPICOM_Int2Str(index_priority, buf, 4);               
            MMI_STRNTOWSTR(plmn_priority, 4, buf, 4, 4);               

            plmn_priority_text.wstr_ptr = plmn_priority;
            plmn_priority_text.wstr_len = sizeof(plmn_priority)/sizeof(wchar);

            GUIEDIT_SetStyle(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            //优先级
            GUIEDIT_SetString(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, plmn_priority_text.wstr_ptr, plmn_priority_text.wstr_len);

            MMI_GetLabelTextByLang(TXT_COMMON_PRIORITY, &priority_text);

            sprintf((char *)priority_range, "1-%d", prefer_net_num);
            priority_range_len = strlen((char *)priority_range);

            title_text.wstr_len = (priority_text.wstr_len + 1 + priority_range_len + 1);
            title_text.wstr_ptr = SCI_ALLOC_APPZ((title_text.wstr_len + 1)*sizeof(wchar));

            if (PNULL == title_text.wstr_ptr)
            {
                break;
            }

            MMI_WSTRNCPY(title_text.wstr_ptr, title_text.wstr_len, priority_text.wstr_ptr, priority_text.wstr_len, priority_text.wstr_len);
            MMI_WSTRNCPY(title_text.wstr_ptr + priority_text.wstr_len, title_text.wstr_len, &temp_ch1, 1, 1);
            MMI_STRNTOWSTR(title_text.wstr_ptr + priority_text.wstr_len + 1, title_text.wstr_len, priority_range, priority_range_len, priority_range_len);
            MMI_WSTRNCPY(title_text.wstr_ptr + priority_text.wstr_len + 1 + priority_range_len, title_text.wstr_len, &temp_ch2, 1, 1);

            GUIWIN_SetTitleText(win_id, title_text.wstr_ptr, title_text.wstr_len, FALSE);

            SCI_FREE(title_text.wstr_ptr);

            GUIEDIT_SetStyle(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            MMK_SetAtvCtrl(win_id, MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID);
        }
        break;
     
    case MSG_CTL_MIDSK:
    //case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T priority_str = {0};
            char priority_num[MMISET_PLMN_PRIORITY_MAX_LEN + 1] = {0};
            MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);
            uint32 prefer_net_num = MIN(prefer_network.plmn_validate_num, prefer_network.plmn_max_num);
            MN_PLMN_T plmn_temp = {0};
            
            GUIEDIT_GetString(MMISET_NETWORK_PRIORITY_CHANGE_EDIT_CTRL_ID, &priority_str);

            MMI_WSTRNTOSTR(priority_num, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_ptr, MMISET_PLMN_PRIORITY_MAX_LEN, priority_str.wstr_len);/*lint !e64*/
            index_priority = atoi(priority_num);

            //SCI_TRACE_LOW:"HandlePreferNetworkPriorityChangeWinMsg,index_priority=%d,cur_index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_12955_112_2_18_2_50_44_124,(uint8*)"dd",index_priority,cur_index);

            if (index_priority == cur_index + 1) //优先级没有调整
            {
                //clsoe option menu and current win.
                MMK_CloseWin(MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID);
                MMK_CloseWin(win_id);
                // success !
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
            else if (index_priority > prefer_net_num || index_priority < 1)
            {
                //提示失败的窗口
                MMIPUB_OpenAlertWarningWin(TXT_INPUT_ERROR);
            }
            else if( index_priority - 1 < cur_index )
            {
                uint32 i = 0;

                SCI_MEMCPY(&plmn_temp, &prefer_network.plmn_arr[cur_index], sizeof(MN_PLMN_T));

                for (i = cur_index; i > index_priority -1; i--)
                {
                    SCI_MEMCPY(&prefer_network.plmn_arr[i], &prefer_network.plmn_arr[i - 1], sizeof(MN_PLMN_T));
                }                        
                SCI_MEMCPY(&prefer_network.plmn_arr[index_priority-1], &plmn_temp, sizeof(MN_PLMN_T));

                MMIAPISET_SavePreferNetworkList(dual_sys, &prefer_network);
            }
            else if (index_priority - 1 > cur_index)
            {
                uint32 i = 0;

                SCI_MEMCPY(&plmn_temp, &prefer_network.plmn_arr[cur_index], sizeof(MN_PLMN_T));

                for (i = cur_index; i < index_priority - 1; i++)
                {
                    SCI_MEMCPY(&prefer_network.plmn_arr[i], &prefer_network.plmn_arr[i + 1], sizeof(MN_PLMN_T));
                }                        
                SCI_MEMCPY(&prefer_network.plmn_arr[index_priority - 1], &plmn_temp, sizeof(MN_PLMN_T));

                MMIAPISET_SavePreferNetworkList(dual_sys, &prefer_network);
            }
        }
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

/*****************************************************************************/
//  Description : HandlePreferNetWaitingWinMsg
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreferNetWaitingWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"HandlePreferNetChangeWaitingWinMsg,msg=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13026_112_2_18_2_50_45_125,(uint8*)"d",msg_id);

    switch(msg_id)
    {        
    case MSG_APP_CANCEL:
        s_allow_set_prefer_plmn = FALSE;
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : InitPlmnListCtrl
//  Global resource dependence : 
//  Author:michael.shi
//  Note: InitPlmnListCtrl
/*****************************************************************************/
LOCAL void InitPlmnListCtrl(MMI_CTRL_ID_T ctrl_id,uint16 max_item_num)
{
    uint32 i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16 index = GUILIST_GetCurItemIndex(ctrl_id);

    GUILIST_RemoveAllItems(ctrl_id);

    //需要动态创建plmn列表数，以节省memory!!
    GUILIST_SetMaxItem( ctrl_id, max_item_num, TRUE); 

    item_t.item_style         = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t.item_data_ptr  = &item_data;
    
    if (max_item_num)
    {            
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
    }
    else
    {
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
    }
    
    for ( i = 0; i < max_item_num; i++ )
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }       

    GUILIST_SetCurItemIndex(ctrl_id,0);
}

/*****************************************************************************/
//  Description : get prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetCurPreferNetworkIndex(void)
{
    return s_prefer_network_item;
}

/*****************************************************************************/
//  Description : set prefer network list num.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
LOCAL void SetCurPreferNetworkIndex( uint16 index)
{
    s_prefer_network_item = index;
}

/*****************************************************************************/
//  Description : isPlmnValid ,
//  Global resource dependence : 
//  Author:michael.shi
//  Note: if plmn is in list ,return FALSE; if plmn is not in list ,return TRUE;
/*****************************************************************************/
LOCAL BOOLEAN isPlmnValid(MN_DUAL_SYS_E dual_sys, MN_PLMN_T plmn)
{
    uint32 i=0;
    MN_PHONE_PLMN_SEL_LIST_T prefer_network = MMIAPISET_LoadPreferNetworkList(dual_sys);

    for(i = 0; i < prefer_network.plmn_validate_num; i++)    
    {
        //某些特殊的case可以加快循环体的判断结果
        if( (plmn.mcc == prefer_network.plmn_arr[i].mcc )
            &&(plmn.mnc == prefer_network.plmn_arr[i].mnc) )
        {
            return FALSE;
        }
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : MMISET_PreferNetworkListInit
//  Global resource dependence : 
//  Author:michael.shi
//  Note: call l4 MN API:MNPHONE_GetPreferredPlmnListEx
/*****************************************************************************/
PUBLIC MN_PHONE_PLMN_SEL_LIST_T MMIAPISET_LoadPreferNetworkList(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_PLMN_SEL_LIST_T return_prefer_list = {0};

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return (return_prefer_list);
    }
    
    MNPHONE_GetPreferredPlmnListEx(dual_sys, MN_PHONE_PLMN_LIST_UPLMN, &return_prefer_list);
    
    //SCI_TRACE_LOW:"LoadMNPreferNetworkList,sim=%d,prefer_valid_num=%d,prefer_max_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13131_112_2_18_2_50_45_126,(uint8*)"ddd",dual_sys,return_prefer_list.plmn_validate_num,return_prefer_list.plmn_max_num);
    
    return (return_prefer_list);
}

/*****************************************************************************/
//  Description : MMIAPISET_SavePreferNetworkList
//  Global resource dependence : 
//  Author:michael.shi
//  Note: call l4 MN API : MMIAPISET_SavePreferNetworkList
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SavePreferNetworkList(
                                   MN_DUAL_SYS_E   dual_sys,
                                   MN_PHONE_PLMN_SEL_LIST_T   *prefer_network   
                                   )
{
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T wait_win_id =  MMISET_PREFER_CHANGE_WAIT_WIN_ID;
    uint16 prefer_list_mem_size = sizeof(MN_PHONE_PLMN_SEL_LIST_T);

    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == prefer_network)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("MMIAPISET_SavePreferNetworkList, s_allow_set_prefer_plmn= %d ", s_allow_set_prefer_plmn);
    if(!s_allow_set_prefer_plmn)
    {
        MMIPUB_OpenAlertWaitingWin(TXT_COMMON_WAITING);
        return FALSE;
    }
		

    if(PNULL == s_update_prefer_list_ptr)
    {
        s_update_prefer_list_ptr = SCI_ALLOC_APP(prefer_list_mem_size);
    }
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);    
    
    MNPHONE_SetPreferredPlmnListEx(dual_sys, prefer_network);

    //保留本次更新的列表，以确认SIM是否可以更新成功。
    if(PNULL != s_update_prefer_list_ptr)
    {
        SCI_MEMSET(s_update_prefer_list_ptr,0x00,prefer_list_mem_size);
        SCI_MEMCPY(s_update_prefer_list_ptr,prefer_network,prefer_list_mem_size);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_SavePreferNetworkList ERROR ! s_update_prefer_list_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13171_112_2_18_2_50_45_127,(uint8*)"");
    }

    //打开waiting的等待窗口
    MMIPUB_OpenWaitWin(1,
                            &prompt_str,
                            PNULL,
                            PNULL,
                            wait_win_id,
                            IMAGE_NULL,
                            ANIM_PUBWIN_WAIT,
                            WIN_ONE_LEVEL,
                            MMIPUB_SOFTKEY_ONE,
                            HandlePreferNetWaitingWinMsg);

    
    //SCI_TRACE_LOW:"MMIAPISET_SavePreferNetworkList,dual_sys=%d,prefer_valid_num=%d,prefer_max_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13188_112_2_18_2_50_45_128,(uint8*)"ddd",dual_sys,prefer_network->plmn_validate_num,prefer_network->plmn_max_num);

    return TRUE;
}

/*****************************************************************************/
//  Description : MMIAPISET_HandlePreferNetworkListCnf
//  Global resource dependence : 
//  Author:michael.shi
//  Note:  MMIAPISET_HandlePreferNetworkListCnf
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_HandlePreferNetworkListCnf(APP_MN_PREFER_PLMN_CHANGE_CNF_T* prefer_cnf)
{    
    BOOLEAN is_update_success = FALSE;

    uint32 cmplen = sizeof(prefer_cnf->new_prefer_plmn_list.plmn_validate_num) 
        + sizeof(prefer_cnf->new_prefer_plmn_list.plmn_max_num) + sizeof(MN_PLMN_T) * prefer_cnf->new_prefer_plmn_list.plmn_validate_num;

    //if( (PNULL != s_update_prefer_list_ptr)
    //    &&( 0 == memcmp(&(prefer_cnf->new_prefer_plmn_list),s_update_prefer_list_ptr,/*sizeof(MN_PHONE_PLMN_SEL_LIST_T)*/cmplen)))
    {
        is_update_success = TRUE;
    }

    //SCI_TRACE_LOW:"MMIAPISET_HandlePreferNetworkListCnf,sim=%d,update_sucess=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALLWIN_13207_112_2_18_2_50_45_129,(uint8*)"dd",prefer_cnf->dual_sys,is_update_success);

    //close wating window
    MMK_CloseWin(MMISET_PREFER_CHANGE_WAIT_WIN_ID);
    s_allow_set_prefer_plmn = TRUE;

    //close priority change window
    MMK_CloseWin(MMISET_NETWORK_PRIORITY_CHANGE_WIN_ID);

    //关闭菜单窗口
    MMK_CloseWin(MMISET_PREFER_NETWORK_OPT_MENU_WIN_ID);                

    if(prefer_cnf->is_ok && is_update_success)
    {
        //弹出提示成功的窗口
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
    }
    else
    {    
        //dispaly failed window        
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
    
    if(PNULL != s_update_prefer_list_ptr)
    {
        SCI_FREE(s_update_prefer_list_ptr);
        s_update_prefer_list_ptr = PNULL;
    }

    if (MMK_IsOpenWin(MMISET_SET_PREFER_NETWORK_WIN_ID))
    {
        MMK_SendMsg(MMISET_SET_PREFER_NETWORK_WIN_ID, MSG_SET_LOAD_PREFER_PLMN_LIST, PNULL);
    }

    return TRUE;
}
#endif
#if defined(MAINLCD_SIZE_176X220) || defined(MAINLCD_SIZE_128X160)
/*****************************************************************************/
// 	Description : 设置输入密码窗口的背景色，image 和title
//	Global resource dependence : 
//  Author: chunjie liu
//	Note:
/*****************************************************************************/
LOCAL void SetPwBackground(
                                 MMI_WIN_ID_T    win_id, //窗口的ID
                                 MMI_CTRL_ID_T   ctrl_id,   //the id of control
                                 BOOLEAN is_need_label
                                 )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T bg_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T  both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MONOCHROME_LCD_SUPPORT 
#else
    GUI_POINT_T dis_point = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;	
    //GUI_BOTH_RECT_T  both_rect = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
#endif 
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    if(MMITHEME_IsMainScreenLandscape())
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
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_NUMBERINPUT_IP, win_id);

    //设置image id
    IMG_EnableTransparentColor(TRUE);
    GUIRES_DisplayImg(PNULL,
            &bg_rect,
            &bg_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);

    edit_both_rect = GetMinuteReminderEditRect(win_id, is_need_label);

    dis_point.x = (bg_rect.right + bg_rect.left - icon_width) / 2;
    dis_point.y = (bg_rect.bottom + edit_both_rect.v_rect.bottom - icon_height) / 2;
    GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_NUMBERINPUT_IP,
            &lcd_dev_info);
    
    IMG_EnableTransparentColor(FALSE); 
#endif 
}

/*****************************************************************************/
// 	Description : GetPinPukEditRect
//	Global resource dependence : 
//  Author: chunjie liu
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetMinuteReminderEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label)
{
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
	return GetPasswordEditBothRect(is_need_label);
#else
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
    if (is_need_label)
    {
        both_rect.h_rect.top += MMISET_MINUTE_REMINDER_REMAIN_TOP_MARGIN + MMISET_MINUTE_REMINDER_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMISET_MINUTE_REMINDER_REMAIN_TOP_MARGIN + MMISET_MINUTE_REMINDER_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
    }
    else
    {
        both_rect.h_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
    }

    both_rect.h_rect.bottom = both_rect.h_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.h_rect.right = both_rect.h_rect.right - MMI_PASSWORD_EDIT_LEFT;
    both_rect.v_rect.bottom = both_rect.v_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.v_rect.right = both_rect.v_rect.right - MMI_PASSWORD_EDIT_LEFT;

    return both_rect;
#endif 
		
}
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
// 	Description : Is Set DualSys MSISDN 
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSetDualSysMSISDN(void)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    BOOLEAN is_dual_sys_msisdn = TRUE;
    
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        MMIPB_BCD_NUMBER_T sim_number = {0};
        
        MMIAPIPB_GetMSISDNumberArrayBySim(&(sim_number), dual_sys, 0);
        if(0 == sim_number.number_len)
        {
            is_dual_sys_msisdn = FALSE;
            break;
        }
    }
    
    return is_dual_sys_msisdn;
}

/*****************************************************************************/
// 	Description : Set DualSIM CFNRC
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDualSIMCFNRC(MMI_HANDLE_T win_id)
{
    BOOLEAN is_need_close_msisdn_win = TRUE;
    
    if(MMK_IsOpenWin(MMISET_SET_MULTI_SYS_SETTING_WIN_ID))
    {
        if(IsSetDualSysMSISDN())
        {
            OpenCFNRCWin(); 
        }
        else
        {
            if(!MMK_IsOpenWin(win_id))//MSISDN WIN
            {
                MMIAPIPB_OpenMSISDNWin(FALSE, TRUE);
            }
            else
            {
                //in case close it outside
                is_need_close_msisdn_win = FALSE;
                //alert set pb msisdn first
                MMIPUB_OpenAlertWarningWin(TXT_SET_PB_MSISDN);
            }
        }
    }
    
    return is_need_close_msisdn_win;
}

/*****************************************************************************/
//  Description : Handle Enter Edit MSISDN QueryWin
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterEditMSISDNQueryWin(
                                                MMI_WIN_ID_T       win_id,    
                                                MMI_MESSAGE_ID_E      msg_id,    
                                                DPARAM                param      
                                             )
{
    MMI_RESULT_E result =  MMI_RESULT_TRUE;
    

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMIAPIPB_OpenMSISDNWin(FALSE,TRUE);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;

    }
    return result;
}
#if 0
/*****************************************************************************/
// 	Description : to handle dualsys call divert help function, display the help infor
//	Global resource dependence : 
//   Author:  dave.ruan
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDualsysCFNRCHelpWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T help_string_ptr = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMI_GetLabelTextByLang(TXT_SET_DUAL_HELP, &help_string_ptr);
        GUITEXT_SetString(MMISET_HELP_TEXT_CTRL_ID,help_string_ptr.wstr_ptr,help_string_ptr.wstr_len,FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_HELP_TEXT_CTRL_ID);  
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
// 	Description : Set DualSIM standby mode
//	Global resource dependence : 
//   Author: yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDualSIMStandby(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys)
{
    ImplementStandbyChanged(is_standby_ptr, dual_sys);
}

#endif
/*Edit by script, ignore 56 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
