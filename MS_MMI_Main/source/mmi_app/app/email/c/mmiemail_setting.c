/****************************************************************************
** File Name:      mmiemail_xxxx.c                                         *
** Author:                                                         	       *
** Date:           05/13/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/
/**---------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "window_parse.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiconnection_export.h"
#include "mmipub.h"
#include "os_api.h"
#include "guirichtext.h"
#include "mmidisplay_data.h"
#include "guiform.h"
#include "guilabel.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guisetlist.h"

#include "we_def.h"
#include "mmiemail_id.h"
#include "mmiemail_menutable.h"
#include "mmiemail_def.h"
#include "mmiemail_setting.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_utils.h"
#include "email_text.h"
#include "email_image.h"
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "mmiemail_box.h"
#include "mmiemail_net.h"
#include "mmiemail_export.h"

#ifdef MMI_WIFI_SUPPORT
#include "mmiwlan_text.h"
#include "mmiwifi_export.h"
#endif

#include "AEEEMEError.h"
#include "mmiemail_main.h"
#include "mmiemail_edit.h"
#include "guictrl_api.h"
#include "mmiphone_export.h"
#include "mmiemail_dlg.h"
#include "mmi_resource.h"
#include "mmi_default.h"
#include "mmiemail_signal.h"
/**---------------------------------------------------------------------------*
**                         Macro Declaration    							  *
**---------------------------------------------------------------------------*/
#define SETTING_ACC_MAX                    5
#define SETTING_PASSWORD_LABEL_TOP_MARGIN  5
#define SETTING_PASSWORD_LABEL_HEIGHT      22

//该尺寸下的横屏，输入法虚拟键盘打开时，导致edit高度不够，引起assert
#if defined MAINLCD_SIZE_176X220
#define SETTING_PASSWORD_LABEL_TOP_MARGIN_H 1
#define SETTING_PASSWORD_LABEL_HEIGHT_H      16
#endif

#define SETTING_PASSWORD_SHOW_STRING       L"*********************************\
*******************************************************************************\
********************************************************************************\
***************************************************************"
#define MMIEMAIL_MAX_SETTING_NUM           20


#define SETTING_ITEM_NUM_SAVE_SENT         2           
#define SETTING_ITEM_NUM_RECEIVE_LIMIT     2             
#define SETTING_ITEM_NUM_RECEIVE_NEW       2          
#define SETTING_ITEM_NUM_REPLY_MAIL        2        
#define SETTING_ITEM_NUM_SERV_TYPE         2   
#define SETTING_ITEM_NUM_SECURITY          3
#define SETTING_ITEM_NUM_DOWNLOAD_NUMBER   3
#define SETTING_ITEM_NUM_AUTHENTICATE      2   

#define SETTING_MAX_COMMON_ACC             20               
/**---------------------------------------------------------------------------*
**                         TYPE AND CONSTANT    							  *
**---------------------------------------------------------------------------*/

typedef enum
{
    SETTING_EDIT_TYPE_MAIL_ADDR,
    SETTING_EDIT_TYPE_USERNAME,
    SETTING_EDIT_TYPE_INCOMING_SERVER,
    SETTING_EDIT_TYPE_SMTP_SERVER,
    SETTING_EDIT_TYPE_SMTP_USERNAME,
    SETTING_EDIT_TYPE_SMTP_MAILNAME,
    SETTING_EDIT_TYPE_PORT,
    SETTING_EDIT_TYPE_MAX
}SETTING_EDII_TYPE_E;

typedef enum
{
    SETTING_PASSWORD_TYPE_PASSWORD,
    SETTING_PASSWORD_TYPE_SMTP_PASSWORD,
    SETTING_PASSWORD_TYPE_MAX
}SETTING_PASSWORD_TYPE_E;


typedef enum
{
    SETTING_ENTER_TYPE_COMMON_ACC,
    SETTING_ENTER_TYPE_CUSTOMIZED_ACC,
    SETTING_ENTER_TYPE_EXSIT_ACC,
    SETTING_ENTER_TYPE_MAX
}SETTING_ENTER_TYPE_E;

typedef enum
{
    SETTING_ACCOUNT_SETTINGS_PERSONALIZED,
    SETTING_ACCOUNT_SETTINGS_LOAD,
    SETTING_ACCOUNT_SETTINGS_DATA_CONNECTION,
    SETTING_ACCOUNT_SETTINGS_INCOMING_SERV,
    SETTING_ACCOUNT_SETTINGS_OUTGOING_SERV,
    SETTING_ACCOUNT_SETTINGS_MAX
}SETTING_ACCOUNT_SETTINGS_E;

typedef enum
{
    SETTING_LIST_TYPE_ONE_LINE_TEXT,
    SETTING_LIST_TYPE_ONE_LINE_RADIO,
    SETTING_LIST_TYPE_ONE_LINE_CHECK,
    SETTING_LIST_TYPE_MAX
}SETTING_LIST_TYPE_E;

typedef struct {
    wchar *          pwText;     // widget text 
    //IImage *          piImage;    // widget image
}IMAGE_STATIC_T;


typedef struct setting_account_tag
{
   EMAIL_VECTOR_T * vector_ptr;
   WE_INT32      act_cnt;
   WE_UINT32    * act_id_ptr;
}SETTING_ACCOUNT_T;



typedef struct load_setting_tag
{
   wchar        *mail_address_ptr; 
   wchar        *user_name_ptr; 
   wchar        *password_ptr; 
}LOAD_SETTING_T;

typedef struct data_connection_tag
{
   uint32   net_path; 
   uint32   net_sim[MMI_DUAL_SYS_MAX]; 
}DATA_CONNECTION_T;

typedef struct incoming_setting_tag
{
  	wchar    *incoming_server_ptr; 
   uint32   server_type; 
   uint32   port_type; 
   wchar    *port_ptr;
   uint32   secruity;
}INCOMING_SETTING_T;

typedef struct outgoing_setting_tag
{
  	wchar    *outgoing_server_ptr; 
   wchar    *user_name_ptr;
   wchar    *password_ptr;
   uint32   port_type; 
   wchar    *port_ptr;
   uint32   secruity;
   uint32   authenticate;
}OUTGOING_SETTING_T;

typedef struct personalized_setting_tag
{
  	wchar    *acc_name_ptr; 
   uint32   save_sent;
   uint32   download_num;
   uint32   receive_limit;
   uint32   receive_new;
   uint32   reply_mail;
}PERSONALIZED_SETTING_T;

typedef struct email_setting_tag
{   
   // wchar *pwszActName;
   SETTING_ACCOUNT_T *account_ptr;
   LOAD_SETTING_T *load_ptr;
   DATA_CONNECTION_T *data_conn_ptr;
   INCOMING_SETTING_T *recv_serv_ptr;
   OUTGOING_SETTING_T *send_serv_ptr;
   PERSONALIZED_SETTING_T *personalized_ptr;
   SETTING_EDII_TYPE_E edit_type;
   SETTING_PASSWORD_TYPE_E password_type;
   SETTING_ENTER_TYPE_E  enter_type;
   uint32 act_id;
   uint8  check_status[SETTING_ACC_MAX + 1];
   EMS_ServerConf *serv_conf_ptr;
}EMAIL_SETTING_T;

typedef void (*SETTING_ITEM_FUNC)(void);

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 
static EMAIL_SETTING_T s_email_setting;
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern HEmlSPMgr		g_email_sp_handle;
#ifdef MMI_WIFI_SUPPORT
extern uint32          g_email_is_send; //lint !e752
extern BOOLEAN         g_email_is_use_wifi;
#endif
extern MMIEMAIL_TRANS_TYPE_E  g_email_trans_type;
extern BOOLEAN       g_email_pdp_active;
extern EMAOPT_T      *g_email_opt_ptr;
extern BOOLEAN       g_email_is_engine_task_busy;
extern BOOLEAN           g_save_email_on_red_key;
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :send setting waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingWaitWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
/*****************************************************************************/
// 	Description: handle select email account window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectEmailAccWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description: handle system setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSystemSettingWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description: handle setting option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingOptWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );

/*****************************************************************************/
// 	Description: handle account delete window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAccountDeleteWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description: handle account setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAccountSettingWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               );

/*****************************************************************************/
// 	Description: handle setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEmailSettingWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM                param
                                        );


/*****************************************************************************/
// 	Description: handle setting password window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingPasswordWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM                param
                                                );

/*****************************************************************************/
// 	Description: handle setting edit window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingEditWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );

/*****************************************************************************/
// 	Description: handle setting download number window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDownloadNumberWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
// 	Description: handle setting receive number limit window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceiveLimitWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );


/*****************************************************************************/
// 	Description: handle setting save sent window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveSentWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
// 	Description: handle setting receive new window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceiveNewWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
// 	Description: handle setting reply with orig window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReplyMailWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );


/*****************************************************************************/
// 	Description: handle setting authenticate option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAuthenticateWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );


/*****************************************************************************/
// 	Description: handle setting server type option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleServTypeWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
// 	Description: handle setting security option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecurityWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );


/*****************************************************************************/
// 	Description: handle setting server port option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleServPortWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );


/*****************************************************************************/
// 	Description: handle setting network path option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkPathWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );


/*****************************************************************************/
// 	Description: handle email account link list option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEmailAccountLinkListWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     );

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description: handle email personalized window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePersonalizedSettingWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
// 	Description: handle email outgoing setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutgoingSettingWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
// 	Description: handle email incoming setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingSettingWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
// 	Description: handle email data connection window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDataConnectionWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
// 	Description: handle email load setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLoadSettingWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#endif /* MMI_GUI_STYLE_TYPICAL */

/*****************************************************************************/
// 	Description: handle email entry tip window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEntryEmailTipWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description: handle email wifi start query window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
#ifdef MMI_WIFI_SUPPORT
LOCAL MMI_RESULT_E HandleWifiStartQueryWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              );
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description: handle email Setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLoadSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
// 	Description: handle data Connectiong list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDataConnectionListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
// 	Description: handle Incoming Setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
// 	Description: handle Outcoming Setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutgoingSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePersonalizedSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#endif /* MMI_GUI_STYLE_TYPICAL  */

/*****************************************************************************/
// 	Description: is same account as with default_act
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIEMAIL_IsSameAccount(char *default_act, wchar *act_ptr);
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIEMAIL_SELECT_EMAIL_ACC_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleSelectEmailAccWinMsg),    
      WIN_ID(EMAIL_SELECT_EMAIL_ACC_WIN_ID),
      WIN_TITLE(TXT_EMAIL_PLS_SELECT_ACCOUNT),
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
      CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_SELECT_EMAIL_ACC_CTRL_ID),
      END_WIN
};


WINDOW_TABLE( MMIEMAIL_SYSTEM_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleSystemSettingWinMsg),    
      WIN_ID(EMAIL_SYSTEM_SETTING_WIN_ID),
      WIN_TITLE(TXT_SYSTEM_SETTING),
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
      
      CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, EMAIL_SYSTEM_SETTING_CTRL_ID),
      
      //aotodown
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_STARTUP_SCREEN_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_STARTUP_SCREEN_TITLE_LABEL_CTRL_ID,EMAIL_STARTUP_SCREEN_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_STARTUP_SCREEN_TEXT_CTRL_ID,EMAIL_STARTUP_SCREEN_FORM_CTRL_ID),
      //  
      //       //advertise
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_MAIL_STORAGE_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_MAIL_STORAGE_TITLE_LABEL_CTRL_ID,EMAIL_MAIL_STORAGE_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_MAIL_STORAGE_TEXT_CTRL_ID,EMAIL_MAIL_STORAGE_FORM_CTRL_ID),
      //       //permit rpt
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_TONE_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_TONE_TITLE_LABEL_CTRL_ID,EMAIL_TONE_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_TONE_TEXT_CTRL_ID, EMAIL_TONE_FORM_CTRL_ID),
      //       
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_TONE_VOLUME_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_TONE_VOLUME_TITLE_LABEL_CTRL_ID,EMAIL_TONE_VOLUME_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_TONE_VOLUME_TEXT_CTRL_ID, EMAIL_TONE_VOLUME_FORM_CTRL_ID),
      //       
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SLIENT_TIME_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SLIENT_TIME_TITLE_LABEL_CTRL_ID,EMAIL_SLIENT_TIME_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_SLIENT_TIME_TEXT_CTRL_ID, EMAIL_SLIENT_TIME_FORM_CTRL_ID),
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_ACCOUNT_PASSWORD_PROTECTION_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_ACCOUNT_PASSWORD_PROTECTION_TITLE_LABEL_CTRL_ID,EMAIL_ACCOUNT_PASSWORD_PROTECTION_FORM_CTRL_ID),
      //       CHILD_SETLIST_CTRL(TRUE,EMAIL_ACCOUNT_PASSWORD_PROTECTION_TEXT_CTRL_ID, EMAIL_ACCOUNT_PASSWORD_PROTECTION_FORM_CTRL_ID),
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SET_PASSWORD_PROTECTION_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,EMAIL_SET_PASSWORD_PROTECTION_TITLE_LABEL_CTRL_ID,EMAIL_SET_PASSWORD_PROTECTION_FORM_CTRL_ID),
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_STORAGE_STATISTICS_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,EMAIL_STORAGE_STATISTICS_TITLE_LABEL_CTRL_ID,EMAIL_STORAGE_STATISTICS_FORM_CTRL_ID),     
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_FLOW_STATISTICS_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,EMAIL_FLOW_STATISTICS_TITLE_LABEL_CTRL_ID,EMAIL_FLOW_STATISTICS_FORM_CTRL_ID),     
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SOFTWARE_VERSION_CHECK_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,EMAIL_SOFTWARE_VERSION_CHECK_TITLE_LABEL_CTRL_ID,EMAIL_SOFTWARE_VERSION_CHECK_FORM_CTRL_ID),
      
      //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_RESTORE_FACTORY_SETTINGS_FORM_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,EMAIL_RESTORE_FACTORY_SETTINGS_TITLE_LABEL_CTRL_ID,EMAIL_SYSTEM_SETTING_CTRL_ID),
      
      END_WIN
};


WINDOW_TABLE(MMIEMAIL_SETTING_OPT_WIN_TAB) = 
{
   WIN_FUNC((uint32)HandleSettingOptWinMsg),
      WIN_ID(EMAIL_SETTING_OPT_WIN_ID),
      WIN_STYLE(WS_HAS_TRANSPARENT),
      CREATE_POPMENU_CTRL(MENU_EMAIL_SETTING_OPT, EMAIL_SETTING_OPT_CTRL_ID),
      WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
      END_WIN   
};

WINDOW_TABLE(MMIEMAIL_ACCOUNT_DELETE_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleAccountDeleteWinMsg),
      WIN_TITLE(TXT_EMAIL_ACCOUNT_DELETE),
      WIN_ID(EMAIL_ACCOUNT_DELETE_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
      WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_COMMON_OK, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_ACCOUNT_DELETE_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE( MMIEMAIL_ACCOUNT_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleAccountSettingWinMsg),    
      WIN_ID(EMAIL_ACCOUNT_SETTING_WIN_ID),
      WIN_TITLE(TXT_ACCOUNT_SETTING),
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
      CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_ACCOUNT_SETTING_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE( MMIEMAIL_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleEmailSettingWinMsg),    
      WIN_ID(EMAIL_SETTING_WIN_ID),
      WIN_TITLE(TXT_EMAIL_TITLE),
      WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
      CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_SETTING_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE(MMIEMAIL_SETTING_PASSWORD_WIN_TAB) = 
{
   WIN_TITLE(TXT_COMMON_PASSWORD),
      WIN_FUNC((uint32)HandleSettingPasswordWinMsg),    
      WIN_ID(EMAIL_SETTING_PASSWORD_WIN_ID),
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMI_GUI_STYLE_TYPICAL
      CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,EMAIL_SETTING_PASSWROD_LABEL_CTRL_ID),
#endif
      CREATE_EDIT_PASSWORD_CTRL(EMA_USERINFO_USER_ID_LENGTH,EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID),
      END_WIN
};


WINDOW_TABLE(MMIEMAIL_SETTING_EDIT_WIN_TAB) = 
{
   WIN_TITLE(TXT_EMAIL_ACC_ADDR),
      WIN_FUNC((uint32)HandleSettingEditWinMsg),    
      WIN_ID(EMAIL_SETTING_EDIT_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
      CREATE_EDIT_TEXT_CTRL(EMA_ACCOUNT_USERINFO_NAME_LENGTH, EMAIL_SETTING_EDIT_CTRL_ID),
      END_WIN
};

WINDOW_TABLE(MMIEMAIL_DOWNLOAD_NUMBER_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleDownloadNumberWinMsg),
      WIN_TITLE(TXT_EMAIL_ONCE_DOWNLOAD_NUMBER),
      WIN_ID(EMAIL_DOWNLOAD_NUMBER_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE(MMIEMAIL_RECEIVE_LIMIT_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleReceiveLimitWinMsg),
      WIN_TITLE(TXT_EMAIL_DOWNLOADMODE),
      WIN_ID(EMAIL_RECEIVE_LIMIT_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE(MMIEMAIL_SAVE_SENT_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleSaveSentWinMsg),
      WIN_TITLE(TXT_EMAIL_SAVESENT),
      WIN_ID(EMAIL_SAVESENT_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_SAVESENT_LISTBOX_CTRL_ID),
      END_WIN
};


WINDOW_TABLE(MMIEMAIL_RECEIVE_NEW_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleReceiveNewWinMsg),
      WIN_TITLE(TXT_EMAIL_RECEIVE_ONLY_NEW_MAIL),
      WIN_ID(EMAIL_RECEIVE_NEW_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID),
      END_WIN
};


WINDOW_TABLE(MMIEMAIL_REPLY_MAIL_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleReplyMailWinMsg),
      WIN_TITLE(TXT_EMAIL_REPLY_MAIL),
      WIN_ID(EMAIL_REPLY_MAIL_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE(MMIEMAIL_AUTHENTICATE_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleAuthenticateWinMsg),
      WIN_TITLE(TXT_EMAIL_AUTHENTICATION),
      WIN_ID(EMAIL_AUTHENTICATE_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE(MMIEMAIL_SERVER_TYPE_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleServTypeWinMsg),
      WIN_TITLE(TXT_EMAIL_SERVERTYP),
      WIN_ID(EMAIL_SERV_TYPE_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_SERV_TYPE_LISTBOX_CTRL_ID),
      END_WIN
};


WINDOW_TABLE(MMIEMAIL_SECURITY_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleSecurityWinMsg),
      WIN_TITLE(TXT_COMM_SECURITY),
      WIN_ID(EMAIL_SECURITY_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_SECURITY_LISTBOX_CTRL_ID),
      END_WIN
};

WINDOW_TABLE(MMIEMAIL_SERVER_PORT_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleServPortWinMsg),
      WIN_TITLE(TXT_COMM_PORT),
      WIN_ID(EMAIL_SERV_PORT_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_SERV_PORT_LISTBOX_CTRL_ID),
      END_WIN
};


WINDOW_TABLE(MMIEMAIL_NETWORK_PATH_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleNetworkPathWinMsg),
      WIN_TITLE(TXT_EMAIL_NETWORKING_PATH),
      WIN_ID(EMAIL_NETWORK_PATH_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID),
      END_WIN
};


WINDOW_TABLE(MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleEmailAccountLinkListWinMsg),
      WIN_TITLE(TXT_EMAIL_NETWORK_ACCOUNT_SIM1),
      WIN_ID(EMAIL_ACCOUNT_LINKLIST_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
      WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
      CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_LINKLIST_LISTBOX_CTRL_ID),
      END_WIN
};

#ifdef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMIEMAIL_PERSONALIZED_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandlePersonalizedSettingWinMsg),    
      WIN_ID(EMAIL_PERSONALIZED_SETTING_WIN_ID),
      WIN_TITLE(TXT_EMAIL_PERSONALIZED_SETTING),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, STXT_RETURN, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif

      CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      
      //aotodown
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_ACC_NAME_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_ACC_NAME_TITLE_LABEL_CTRL_ID,EMAIL_ACC_NAME_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_ACC_NAME_LABEL_CTRL_ID,EMAIL_ACC_NAME_FORM_CTRL_ID),
      
      //advertise
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SEND_MAIL_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SEND_MAIL_TITLE_LABEL_CTRL_ID,EMAIL_SEND_MAIL_FORM_CTRL_ID),
      //        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_SEND_MAIL_LABEL_CTRL_ID,EMAIL_SEND_MAIL_FORM_CTRL_ID),
      //permit rpt
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SENDER_NAME_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SENDER_NAME_TITLE_LABEL_CTRL_ID,EMAIL_SENDER_NAME_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_SENDER_NAME_TEXT_CTRL_ID, EMAIL_SENDER_NAME_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SAVESENT_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SAVESENT_TITLE_LABEL_CTRL_ID,EMAIL_SAVESENT_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_SAVESENT_LABEL_CTRL_ID, EMAIL_SAVESENT_FORM_CTRL_ID),
      
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_RECEIVE_FREQUENCY_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_RECEIVE_FREQUENCY_TITLE_LABEL_CTRL_ID,EMAIL_RECEIVE_FREQUENCY_FORM_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_RECEIVE_FREQUENCY_LABEL_CTRL_ID, EMAIL_RECEIVE_FREQUENCY_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_ONCE_DOWNLOAD_NUMBER_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_ONCE_DOWNLOAD_NUMBER_TITLE_LABEL_CTRL_ID,EMAIL_ONCE_DOWNLOAD_NUMBER_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_ONCE_DOWNLOAD_NUMBER_LABEL_CTRL_ID, EMAIL_ONCE_DOWNLOAD_NUMBER_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_RECEIVE_LIMIT_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_RECEIVE_LIMIT_TITLE_LABEL_CTRL_ID,EMAIL_RECEIVE_LIMIT_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_RECEIVE_LIMIT_LABEL_CTRL_ID, EMAIL_RECEIVE_LIMIT_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_RECEIVE_ONLY_NEW_MAIL_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_RECEIVE_ONLY_NEW_MAIL_TITLE_LABEL_CTRL_ID,EMAIL_RECEIVE_ONLY_NEW_MAIL_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_RECEIVE_ONLY_NEW_MAIL_LABEL_CTRL_ID, EMAIL_RECEIVE_ONLY_NEW_MAIL_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_REPLY_MAIL_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_REPLY_MAIL_TITLE_LABEL_CTRL_ID,EMAIL_REPLY_MAIL_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_REPLY_MAIL_LABEL_CTRL_ID, EMAIL_REPLY_MAIL_FORM_CTRL_ID),
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_MAIL_AUTO_DELETE_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_MAIL_AUTO_DELETE_TITLE_LABEL_CTRL_ID,EMAIL_MAIL_AUTO_DELETE_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_MAIL_AUTO_DELETE_TEXT_CTRL_ID, EMAIL_MAIL_AUTO_DELETE_FORM_CTRL_ID),
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_MAIL_DELETE_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_MAIL_DELETE_TITLE_LABEL_CTRL_ID,EMAIL_MAIL_DELETE_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_MAIL_DELETE_TEXT_CTRL_ID, EMAIL_MAIL_DELETE_FORM_CTRL_ID),
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SIGNATURE_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SIGNATURE_TITLE_LABEL_CTRL_ID,EMAIL_SIGNATURE_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_SIGNATURE_TEXT_CTRL_ID, EMAIL_SIGNATURE_FORM_CTRL_ID),
      // 
      //       CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_NEW_MAIL_TIP_FORM_CTRL_ID,EMAIL_PERSONALIZED_SETTING_CTRL_ID),
      //       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_NEW_MAIL_TIP_TITLE_LABEL_CTRL_ID,EMAIL_NEW_MAIL_TIP_FORM_CTRL_ID),
      //       CHILD_TEXT_CTRL(TRUE,EMAIL_NEW_MAIL_TIP_TEXT_CTRL_ID, EMAIL_NEW_MAIL_TIP_FORM_CTRL_ID),
      
      END_WIN
};


WINDOW_TABLE( MMIEMAIL_OUTGOING_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleOutgoingSettingWinMsg),    
      WIN_ID(EMAIL_OUTGOING_SETTING_WIN_ID),
      WIN_TITLE(TXT_EMAIL_OUTGOING_SETTING),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, STXT_FINISH, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
      
      CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, EMAIL_OUTGOING_SETTING_CTRL_ID),
      
      //aotodown
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SMTP_SERVER_FORM_CTRL_ID,EMAIL_OUTGOING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SMTP_SERVER_TITLE_LABEL_CTRL_ID,EMAIL_SMTP_SERVER_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_SMTP_SERVER_LABEL_CTRL_ID,EMAIL_SMTP_SERVER_FORM_CTRL_ID),
      //CHILD_TEXT_CTRL(TRUE,EMA_MAX_ADDR_LEN, EMAIL_EMAIL_ADDRESS_EDIT_TEXT_CTRL_ID,EMAIL_SMTP_SERVER_FORM_CTRL_ID),
      //advertise
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SMTP_USER_NAME_FORM_CTRL_ID,EMAIL_OUTGOING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SMTP_USER_NAME_TITLE_LABEL_CTRL_ID,EMAIL_SMTP_USER_NAME_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_SMTP_USER_NAME_LABEL_CTRL_ID,EMAIL_SMTP_USER_NAME_FORM_CTRL_ID),
      //permit rpt
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SMTP_PASSWORD_FORM_CTRL_ID,EMAIL_OUTGOING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SMTP_PASSWORD_TITLE_LABEL_CTRL_ID,EMAIL_SMTP_PASSWORD_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_SMTP_PASSWORD_LABEL_CTRL_ID, EMAIL_SMTP_PASSWORD_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SMTP_PORT_FORM_CTRL_ID,EMAIL_OUTGOING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SMTP_PORT_TITLE_LABEL_CTRL_ID,EMAIL_SMTP_PORT_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_SMTP_PORT_LABEL_CTRL_ID, EMAIL_SMTP_PORT_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SMTP_SECURITY_FORM_CTRL_ID,EMAIL_OUTGOING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SMTP_SECURITY_TITLE_LABEL_CTRL_ID,EMAIL_SMTP_SECURITY_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_SMTP_SECURITY_LABEL_CTRL_ID, EMAIL_SMTP_SECURITY_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_SMTP_AUTHENTICATION_FORM_CTRL_ID,EMAIL_OUTGOING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_SMTP_AUTHENTICATION_TITLE_LABEL_CTRL_ID,EMAIL_SMTP_AUTHENTICATION_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_SMTP_AUTHENTICATION_LABEL_CTRL_ID, EMAIL_SMTP_AUTHENTICATION_FORM_CTRL_ID),
      
      END_WIN
};

WINDOW_TABLE( MMIEMAIL_INCOMING_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleIncomingSettingWinMsg),    
      WIN_ID(EMAIL_INCOMING_SETTING_WIN_ID),
      WIN_TITLE(TXT_EMAIL_INCOMING_SETTING),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_NEXT_STEP, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
      
      CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, EMAIL_INCOMING_SETTING_CTRL_ID),
      
      //aotodown
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_INCOMING_SERVER_FORM_CTRL_ID,EMAIL_INCOMING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_INCOMING_SERVER_TITLE_LABEL_CTRL_ID,EMAIL_INCOMING_SERVER_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_INCOMING_SERVER_LABEL_CTRL_ID,EMAIL_INCOMING_SERVER_FORM_CTRL_ID),
      //CHILD_TEXT_CTRL(TRUE,EMA_MAX_ADDR_LEN, EMAIL_EMAIL_ADDRESS_EDIT_TEXT_CTRL_ID,EMAIL_INCOMING_SERVER_FORM_CTRL_ID),
      //advertise
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_INCOMING_SERVER_TYPE_FORM_CTRL_ID,EMAIL_INCOMING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_INCOMING_SERVER_TYPE_TITLE_LABEL_CTRL_ID,EMAIL_INCOMING_SERVER_TYPE_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_INCOMING_SERVER_TYPE_LABEL_CTRL_ID,EMAIL_INCOMING_SERVER_TYPE_FORM_CTRL_ID),
      //permit rpt
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_INCOMING_PORT_FORM_CTRL_ID,EMAIL_INCOMING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_INCOMING_PORT_TITLE_LABEL_CTRL_ID,EMAIL_INCOMING_PORT_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_INCOMING_PORT_LABEL_CTRL_ID, EMAIL_INCOMING_PORT_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_INCOMING_SECURITY_FORM_CTRL_ID,EMAIL_INCOMING_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_INCOMING_SECURITY_TITLE_LABEL_CTRL_ID,EMAIL_INCOMING_SECURITY_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_INCOMING_SECURITY_LABEL_CTRL_ID, EMAIL_INCOMING_SECURITY_FORM_CTRL_ID),
      
      END_WIN
};

WINDOW_TABLE( MMIEMAIL_DATA_CONNECTION_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleDataConnectionWinMsg),    
      WIN_ID(EMAIL_DATA_CONNECTION_WIN_ID),
      WIN_TITLE(TXT_EMAIL_DATA_CONNECTION),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_NEXT_STEP, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
      
      CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, EMAIL_DATA_CONNECTION_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_NOTWORKING_PATH_FORM_CTRL_ID,EMAIL_DATA_CONNECTION_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_NOTWORKING_PATH_TITLE_LABEL_CTRL_ID,EMAIL_NOTWORKING_PATH_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE,EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID,EMAIL_NOTWORKING_PATH_FORM_CTRL_ID),
      //CHILD_TEXT_CTRL(TRUE,EMA_MAX_ADDR_LEN, EMAIL_EMAIL_ADDRESS_EDIT_TEXT_CTRL_ID,EMAIL_NOTWORKING_PATH_FORM_CTRL_ID),
      
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_NETWORK_ACCOUNT_SIM1_FORM_CTRL_ID,EMAIL_DATA_CONNECTION_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_NETWORK_ACCOUNT_SIM1_TITLE_LABEL_CTRL_ID,EMAIL_NETWORK_ACCOUNT_SIM1_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_NETWORK_ACCOUNT_SIM1_LABEL_CTRL_ID,EMAIL_NETWORK_ACCOUNT_SIM1_FORM_CTRL_ID),

#if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_NETWORK_ACCOUNT_SIM2_FORM_CTRL_ID,EMAIL_DATA_CONNECTION_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_NETWORK_ACCOUNT_SIM2_TITLE_LABEL_CTRL_ID,EMAIL_NETWORK_ACCOUNT_SIM2_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_NETWORK_ACCOUNT_SIM2_LABEL_CTRL_ID, EMAIL_NETWORK_ACCOUNT_SIM2_FORM_CTRL_ID),
#endif

#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_NETWORK_ACCOUNT_SIM3_FORM_CTRL_ID,EMAIL_DATA_CONNECTION_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_NETWORK_ACCOUNT_SIM3_TITLE_LABEL_CTRL_ID,EMAIL_NETWORK_ACCOUNT_SIM3_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_NETWORK_ACCOUNT_SIM3_LABEL_CTRL_ID, EMAIL_NETWORK_ACCOUNT_SIM3_FORM_CTRL_ID),
#endif

#ifdef MMI_MULTI_SIM_SYS_QUAD
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_NETWORK_ACCOUNT_SIM4_FORM_CTRL_ID,EMAIL_DATA_CONNECTION_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_NETWORK_ACCOUNT_SIM4_TITLE_LABEL_CTRL_ID,EMAIL_NETWORK_ACCOUNT_SIM4_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_NETWORK_ACCOUNT_SIM4_LABEL_CTRL_ID, EMAIL_NETWORK_ACCOUNT_SIM4_FORM_CTRL_ID),
#endif
      END_WIN
};


WINDOW_TABLE( MMIEMAIL_LOAD_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleLoadSettingWinMsg),    
      WIN_ID(EMAIL_LOAD_SETTING_WIN_ID),
      WIN_TITLE(TXT_EMAIL_LOAD_SETTING),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_NEXT_STEP, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif

      CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, EMAIL_LOAD_SETTING_CTRL_ID),
      
      //aotodown
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_EMAIL_ADDRESS_FORM_CTRL_ID,EMAIL_LOAD_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_EMAIL_ADDRESS_TITLE_LABEL_CTRL_ID,EMAIL_EMAIL_ADDRESS_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_EMAIL_ADDRESS_LABEL_CTRL_ID,EMAIL_EMAIL_ADDRESS_FORM_CTRL_ID),
      //CHILD_TEXT_CTRL(TRUE,EMA_MAX_ADDR_LEN, EMAIL_EMAIL_ADDRESS_EDIT_TEXT_CTRL_ID,EMAIL_EMAIL_ADDRESS_FORM_CTRL_ID),
      //advertise
#if 0
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_USER_NAME_FORM_CTRL_ID,EMAIL_LOAD_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_USER_NAME_TITLE_LABEL_CTRL_ID,EMAIL_USER_NAME_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_USER_NAME_LABEL_CTRL_ID,EMAIL_USER_NAME_FORM_CTRL_ID),
#endif
      //permit rpt
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,EMAIL_PASSWORD_FORM_CTRL_ID,EMAIL_LOAD_SETTING_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,EMAIL_PASSWORD_TITLE_LABEL_CTRL_ID,EMAIL_PASSWORD_FORM_CTRL_ID),
      CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,EMAIL_PASSWORD_LABEL_CTRL_ID, EMAIL_PASSWORD_FORM_CTRL_ID),
      END_WIN
};

#else                                
WINDOW_TABLE( MMIEMAIL_LOAD_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleLoadSettingListWinMsg),    
    WIN_ID(EMAIL_LOAD_SETTING_WIN_ID),
    WIN_TITLE(TXT_EMAIL_LOAD_SETTING),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_LOAD_SETTING_LIST_CTRL_ID),
    #if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_NEXT_STEP, TXT_NULL),
    #else
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    #endif
    END_WIN
};

WINDOW_TABLE( MMIEMAIL_DATA_CONNECTION_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleDataConnectionListWinMsg),    
    WIN_ID(EMAIL_DATA_CONNECTION_WIN_ID),
    WIN_TITLE(TXT_EMAIL_DATA_CONNECTION),
    #if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_NEXT_STEP, TXT_NULL),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_DATA_CONNECTION_LIST_CTRL_ID),
    END_WIN
};


WINDOW_TABLE( MMIEMAIL_INCOMING_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleIncomingSettingListWinMsg),    
    WIN_ID(EMAIL_INCOMING_SETTING_WIN_ID),
    WIN_TITLE(TXT_EMAIL_INCOMING_SETTING),
    #if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_NEXT_STEP, TXT_NULL),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_INCOMING_SETTING_LIST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIEMAIL_OUTGOING_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleOutgoingSettingListWinMsg),    
    WIN_ID(EMAIL_OUTGOING_SETTING_WIN_ID),
    WIN_TITLE(TXT_EMAIL_OUTGOING_SETTING),
    #if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, STXT_FINISH, TXT_NULL),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_OUTGOING_SETTING_LIST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIEMAIL_PERSONALIZED_SETTING_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandlePersonalizedSettingListWinMsg),    
      WIN_ID(EMAIL_PERSONALIZED_SETTING_WIN_ID),
      WIN_TITLE(TXT_EMAIL_PERSONALIZED_SETTING),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, STXT_RETURN, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_PERSONALIZED_SETTING_LIST_CTRL_ID),
      END_WIN
};
#endif  //MMI_GUI_STYLE_TYPICAL

WINDOW_TABLE( MMIEMAIL_ENTRY_EMAIL_TIP_WIN_TAB ) = 
{
   WIN_FUNC((uint32)HandleEntryEmailTipWinMsg),    
      WIN_ID(EMAIL_ENTRY_EMAIL_TIP_WIN_ID),
      WIN_TITLE(TXT_EMAIL_TITLE),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
      WIN_SOFTKEY(TXT_NULL, TXT_COMMON_START, TXT_NULL),
#else
      WIN_SOFTKEY(TXT_COMMON_START, TXT_NULL, STXT_RETURN),
#endif
      CREATE_RICHTEXT_CTRL(EMAIL_ENTRY_EMAIL_TIP_CTRL_ID),
      END_WIN
};



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: get email signature
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL int GetSignature(wchar **buff_pptr)
{
    int result = EMAIL_SUCCESS;
    SFS_HANDLE     file_handle   = 0;     //for read signature from signature file
    char *buff_ptr = PNULL;
    wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 2] = {0};
    uint32 file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;
    uint32 file_size = 0;
    int       read_size = 0;
    EMASP_PRFITEM_VAL val = {0};
    
    do 
    {   
        result = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, 
            EMASP_PRFITEM_EMAIL_CREATE_SIGNATURE, &val);
        EMA_UTILS_BREAKIF(result,"MMIEmail_Setting_GetSignature, EmailAFmMgr_DelActiveForm failed.");
        
        file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)val.pData, EMA_UTILS_STRLEN((char *)val.pData));
        file_handle = MMIAPIFMM_CreateFile(file_full_path, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE| SFS_MODE_READ, PNULL, PNULL);
        if (SFS_INVALID_HANDLE == file_handle) 
        {
            file_handle = MMIAPIFMM_CreateFile(file_full_path, SFS_MODE_CREATE_NEW | SFS_MODE_READ, PNULL, PNULL);
            EMA_UTILS_BREAKNULL(file_handle, result, "create file failed");
        }
        
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        
        buff_ptr = EMA_UTILS_MALLOC(PNULL, file_size + 1);
        EMA_UTILS_BREAKNULL(buff_ptr, result, "out of memory");
        EMA_UTILS_MEMSET(buff_ptr, 0x00, file_size +1);
        
        MMIAPIFMM_ReadFile(file_handle, buff_ptr, file_size, (uint32 *)&read_size, 0);
        
        //save buff
        *buff_pptr = EMA_UTILS_UTF8TOWSTR(PNULL, buff_ptr);
        EMA_UTILS_BREAKNULL(*buff_pptr,result,"EmailAFmMgr_Setting_GetSignature, Save buff failed."); 
    } while(0);
    
    
    EMA_UTILS_FREEIF(PNULL, buff_ptr);
    MMIAPIFMM_CloseFile(file_handle);
    
    if (result != EMAIL_SUCCESS)
    {
        EMA_UTILS_LOG_ERROR(("EmailAFmMgr_Setting_GetSignature, EmailAFmMgr_DelActiveForm failed."));
        return EMAIL_ERROR;
    } 
    
    return EMAIL_SUCCESS;
}

/*****************************************************************************/
// 	Description: get all account info
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetAccountInfo(uint32 param)
{
    EMAIL_VECTOR_T*act_vector_ptr = PNULL;
    EMAIL_VECTOR_T **act_vector_pptr = (EMAIL_VECTOR_T **)param;
    
    MMIEMAIL_VectorNew(&act_vector_ptr);
    
    if(SUCCESS != MMIEMAIL_SP_Store_GetActInfo(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),0,EMASP_ACTINFO_MAX_NUMBER,act_vector_ptr))
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_ViewActProc_Enter, MMIEMAIL_SP_Store_GetActInfo failed."));
        MMIEMAIL_VectorDelete(act_vector_ptr);
        return FALSE;      
    }
    *act_vector_pptr = act_vector_ptr;
    return TRUE;
}

/*****************************************************************************/
// 	Description: add list item
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void AppendListItem(                                    
                          wchar                     *string_ptr,
                          uint8                    string_len,
                          MMI_CTRL_ID_T            ctrl_id,
                          MMI_TEXT_ID_T           mid_softkey_id
                          )
{
    GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
    
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = STXT_RETURN;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
    GUILIST_AppendItem(ctrl_id, &item_t);      
}

/*****************************************************************************/
// 	Description: add list item ex
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void AppendListItemEx(                                    
                            wchar                     *string_ptr,
                            uint8                    string_len,
                            MMI_CTRL_ID_T            ctrl_id,
                            MMI_TEXT_ID_T           left_softkey_id,
                            uint32                   list_type
                            )
{
    GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
    
    item_data.softkey_id[0] = STXT_SELECT;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    switch (list_type)
    {
    case SETTING_LIST_TYPE_ONE_LINE_TEXT:
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
        break;
    case SETTING_LIST_TYPE_ONE_LINE_RADIO:
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
        break;
    case SETTING_LIST_TYPE_ONE_LINE_CHECK:
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
        break;
    default:
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
        break;
    }
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
    
    GUILIST_AppendItem(ctrl_id, &item_t);      
}

/*****************************************************************************/
// 	Description: set net index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL uint32 GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
   uint32               index = 0;
   EMASP_PROFILE_ITEM opt_id = EMASP_PRFITEM_USER1_PROFILE;
//   int result = EMAIL_SUCCESS;
   HStoreMgr      store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL val;
   uint32 act_id = 0;
   uint8 index_tmp = 0;
   uint32             shift = 0;
   
   act_id = s_email_setting.act_id;

   opt_id = EMASP_PRFITEM_USER1_PROFILE;

   store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
   if (PNULL == store_mgr_ptr)
   {
      EMA_UTILS_LOG_ERROR(("EmailAUIC_GetDataServiceProc_Enter, store_mgr_ptr is PNULL."));
      return FALSE;
   }
   
   if (EMAIL_SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,act_id,opt_id,&val))
   {
      EMA_UTILS_LOG_ERROR(("EmailAUIC_GetSignatureProc_Enter, MMIEMAIL_SP_Store_GetActCfgItem is Fail."));
      return FALSE;
   }

   index = (uint32)val.pData;
   EMA_UTILS_TRACE("SetNetSettingIndex, setting index is 0x%08x", index);
/*
   if(MN_DUAL_SYS_1 == dual_sys)
   {
      index = index & 0x000000FF;
   }
   else if(MN_DUAL_SYS_2 == dual_sys)
   {
      index = index & 0x0000FF00;
#ifndef WIN32
      index = index >> 8;
#else
      index = index >> 8;
#endif
   }
   */
   /*sim card net store as follow:
   xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
   sim4     sim3     sim2     sim1
   */
   shift = dual_sys * 8;
   //to get the sim card net 
   index_tmp = (uint8)(index >> shift);

   index = (uint32)index_tmp;

   EMA_UTILS_TRACE("GetNetSettingIndex,dual_sys=%d,index=%d",dual_sys,index);
   
   return index;
}

/*****************************************************************************/
// 	Description: get net index
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetNetSettingIndex(
                                              MN_DUAL_SYS_E dual_sys,
                                              uint32 act_id,
                                              uint32         index
                                              )
{
   EMASP_PROFILE_ITEM opt_id = EMASP_PRFITEM_USER1_PROFILE;
//   int result = EMAIL_SUCCESS;
   HStoreMgr      store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL val;
//   int shift = 0;
   uint32 index_tmp = 0;
   
   EMA_UTILS_TRACE("MMIMMS_Setting_SetNetSettingIndex dual_sys=%d index=%d",dual_sys,index);

   opt_id = EMASP_PRFITEM_USER1_PROFILE;

   store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
   if (PNULL == store_mgr_ptr)
   {
      EMA_UTILS_LOG_ERROR(("EmailAUIC_GetDataServiceProc_Enter, store_mgr_ptr is PNULL."));
      return;
   }

   if(EMAIL_SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,act_id,opt_id,&val))
   {
      EMA_UTILS_LOG_ERROR(("EmailAUIC_GetSignatureProc_Enter, MMIEMAIL_SP_Store_GetActCfgItem is Fail."));
      return;
   }

   index_tmp = (uint32)val.pData;

   /*sim card net store as follow:
   xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
   sim4     sim3     sim2     sim1
   */
#ifndef MMI_MULTI_SIM_SYS_SINGLE
   if(MN_DUAL_SYS_1 == dual_sys)
   {
      index_tmp = index_tmp & 0xFFFFFF00;
      index = index | index_tmp;
   }
   else if(MN_DUAL_SYS_2 == dual_sys)
   {
#ifndef WIN32
      index = index << 8;
#else
      index = index << 8;
#endif
      index_tmp = index_tmp & 0xFFFF00FF;
      index = index | index_tmp;
   }
#if defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
   else if (MN_DUAL_SYS_3 == dual_sys)
   {
       index = index << 16;
       index_tmp = index_tmp & 0xFF00FFFF;
       index = index | index_tmp;
   }
#endif

#if defined (MULTI_SIM_SYS_QUAD)
   else if (MN_DUAL_SYS_4 == dual_sys)
   {
       index = index << 24;
       index_tmp = index_tmp & 0x00FFFFFF;
       index = index | index_tmp;
   }
#endif
#else
  index_tmp = index_tmp & 0xFFFFFF00;
  index = index | index_tmp;
#endif
   val.pData = (void *)index;

   EMA_UTILS_TRACE("SetNetSettingIndex, setting index is 0x%08x", index);
   if (EMAIL_SUCCESS != MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,opt_id,&val))
   {
      EMA_UTILS_LOG_ERROR(("EmailAUIC_GetSignatureProc_Enter, MMIEMAIL_SP_Store_SetActCfgItem is Fail."));
      return;
   }
}

/*****************************************************************************/
// 	Description: create email account
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL int32 CreateEmailAccount(wchar *mail_address)
{
   HConfigMgr  cfg_mgr_ptr = PNULL;
   HStoreMgr   store_mgr_ptr = PNULL;
   EMAIL_VECTOR_T   *act_vector_ptr = PNULL;
   int total_cnt = 0;
   uint32 result = EMAIL_ERROR;
//   MMI_STRING_T size_string = {0};
   WE_INT32 act_id = 0;
   EMASP_PRFITEM_VAL val;   
   uint32 i = 0;
   
   cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(g_email_sp_handle);
   store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
   if (PNULL == cfg_mgr_ptr)
   {
      EMA_UTILS_LOG_ERROR(("MMIEmail_CreateAccount, cfg_mgr_ptr is PNULL."));
      return EMAIL_ERROR;
   }
   if (PNULL == store_mgr_ptr)
   {
      EMA_UTILS_LOG_ERROR(("MMIEmail_CreateAccount, store_mgr_ptr is PNULL."));
      return EMAIL_ERROR;
   }
   
   EMA_UTILS_TRACE("MMIEmail_CreateAccount Begin");
   
   MMIEMAIL_VectorNew(&act_vector_ptr);
   
   if(EMAIL_SUCCESS != MMIEMAIL_SP_Store_GetActInfo(store_mgr_ptr,0,EMASP_ACTINFO_MAX_NUMBER,act_vector_ptr))
   {
       EMA_UTILS_LOG_ERROR(("MMIEmail_CreateAccount, MMIEMAIL_SP_Store_GetActInfo failed."));
       return EMAIL_ERROR;      
   }
   
//    total_cnt = MMIEMAIL_VectorGetVectorNumber(act_vector_ptr);
//    if(total_cnt > 0)
//    {
//       WE_INT32 item_cnt = 0;
//       //EMASP_ACTINFO_T * act_info_ptr = PNULL;
//       for(item_cnt = 0;item_cnt < s_email_setting.account_ptr->act_cnt;item_cnt ++)
//       {
//          MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, s_email_setting.account_ptr->act_id_ptr[item_cnt], EMASP_PRFITEM_ACCOUNT_EMAIL, &val);
//          if(0 == EMA_UTILS_WSTRCMP(val.pData,(wchar *)mail_address))
//          {
//             MMIPUB_OpenAlertWarningWin(TXT_EMAIL_ACC_EXIST);
//             return EMAIL_SUCCESS;
//          }
//       }
//    }  
   
   do 
   {     
       int32 sim_i;
       total_cnt = MMIEMAIL_VectorGetVectorNumber(act_vector_ptr); 
       if(total_cnt > 0)
           result = MMIEMAIL_SP_CFG_CreateAccount(cfg_mgr_ptr,(wchar *)mail_address,(uint32*)&act_id,E_EMAILA_EXSIT_ACCOUNT);
       else
           result = MMIEMAIL_SP_CFG_CreateAccount(cfg_mgr_ptr,(wchar *)mail_address,(uint32*)&act_id,E_EMAILA_NOT_ACCOUNT);
       EMA_UTILS_BREAKIF(result, "create account failed");

       for(sim_i=MN_DUAL_SYS_1 ; sim_i < MMI_DUAL_SYS_MAX; sim_i++)
       {
           int32 index =0;
           MMICONNECTION_LINKSETTING_DETAIL_T* email_connection_setting_item_detail =PNULL;
           s_email_setting.act_id = act_id;
           index = GetNetSettingIndex(sim_i);
           email_connection_setting_item_detail =MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_i,index);
           if(MMICONNECTION_ACCOUNT_MMS ==email_connection_setting_item_detail->account_type)
           {
               for (index=0; index<MMIAPICONNECTION_GetLinkSettingNum(sim_i); index++)
               {
                   if (MMICONNECTION_ACCOUNT_BROWSER ==email_connection_setting_item_detail->account_type)
                   {
                       break;
                   }
                   else
                   {
                       email_connection_setting_item_detail =MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_i,index+1);
                   }
               }
               if ((index +1) >= MMIAPICONNECTION_GetLinkSettingNum(sim_i))
               {
                   index = 0;
                   email_connection_setting_item_detail =MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_i,index);
                   for(index=0;index < MMIAPICONNECTION_GetLinkSettingNum(sim_i);index++)
                   {
                       if(MMICONNECTION_ACCOUNT_MMS ==email_connection_setting_item_detail->account_type)
                       {
                           email_connection_setting_item_detail =MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_i,index+1);
                       }
                       else
                       {
                           break;
                       }
                   }
               }
           }
           SetNetSettingIndex(sim_i, act_id, index);
       }
      //data setting
      val.pData = (void*)s_email_setting.load_ptr->mail_address_ptr;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_ACCOUNT_EMAIL,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");

      val.pData = (void*)s_email_setting.load_ptr->mail_address_ptr;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_EMAIL_USER_ID,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");

      val.pData = (void*)s_email_setting.load_ptr->password_ptr;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_EMAIL_PASSWORD,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");

      //dataconn
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (s_email_setting.data_conn_ptr->net_path > 0)
         {
            val.pData = (void*)s_email_setting.data_conn_ptr->net_path;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_NET_PATH,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         for (i = MN_DUAL_SYS_1; i< MMI_DUAL_SYS_MAX; i++)
         {
             if (s_email_setting.data_conn_ptr->net_sim[i] > 0)
             {
                SetNetSettingIndex(i, act_id, s_email_setting.data_conn_ptr->net_sim[i]);
             }
         }
         
         //incomingserver
         val.pData = (void *)s_email_setting.recv_serv_ptr->server_type;
         result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_RECEIVE_SERVER_TYPE,&val);
         EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         
         if (s_email_setting.recv_serv_ptr->server_type == EMASP_POP3)
         {
            val.pData = (void *)s_email_setting.recv_serv_ptr->incoming_server_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_SERVER,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            
            if (s_email_setting.recv_serv_ptr->port_type > 0)
            {
               val.pData = (void *)s_email_setting.recv_serv_ptr->port_type;
               result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_PORT_DEFAULT,&val);
               EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
               
               val.pData = (void *)s_email_setting.recv_serv_ptr->port_ptr;
               result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_PORT,&val);
               EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            }
            else
            {
                if (EMASP_SECURITY_SSLTLS == s_email_setting.recv_serv_ptr->secruity
                    || EMASP_SECURITY_STARTTLS == s_email_setting.recv_serv_ptr->secruity)
                {
                    val.pData = (void*)L"995";
                }
                else
                {
                    val.pData = (void *)L"110";
                }
               result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_PORT,&val);
               EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            }
            
            if (s_email_setting.recv_serv_ptr->secruity > 0)
            {
               val.pData = (void *)s_email_setting.recv_serv_ptr->secruity;
               result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_SSL_OR_TLS,&val);
               EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            }
         }
         else if (s_email_setting.recv_serv_ptr->server_type == EMASP_IMAP4)
         {
            val.pData = (void *)s_email_setting.recv_serv_ptr->incoming_server_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_SERVER,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            
            if (s_email_setting.recv_serv_ptr->port_type > 0)
            {
               val.pData = (void *)s_email_setting.recv_serv_ptr->port_type;
               result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_PORT_DEFAULT,&val);
               EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
               
               val.pData = (void *)s_email_setting.recv_serv_ptr->port_ptr;
               result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_SERVER_PORT,&val);
               EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            }
            else
            {
                if (EMASP_SECURITY_SSLTLS == s_email_setting.recv_serv_ptr->secruity
                    || EMASP_SECURITY_STARTTLS == s_email_setting.recv_serv_ptr->secruity)
                {
                    val.pData = (void*)L"993";
                }
                else
                {
                    val.pData = (void *)L"143";
                }
                result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_SERVER_PORT,&val);
                EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            }
            
            if (s_email_setting.recv_serv_ptr->secruity > 0)
            {
               val.pData = (void *)s_email_setting.recv_serv_ptr->secruity;
               result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_SSL_OR_TLS,&val);
               EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            }
         }
         
         
         //outgoingserver
         
         val.pData = (void *)s_email_setting.send_serv_ptr->outgoing_server_ptr;
         result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_SERVER,&val);
         EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         
         if (s_email_setting.send_serv_ptr->port_type > 0)
         {
            val.pData = (void *)s_email_setting.send_serv_ptr->port_type;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_PORT_DEFAULT,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            
            val.pData = (void *)s_email_setting.send_serv_ptr->port_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_SERVER_PORT,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         else
         {
             if (EMASP_SECURITY_SSLTLS == s_email_setting.send_serv_ptr->secruity
                 || EMASP_SECURITY_STARTTLS == s_email_setting.send_serv_ptr->secruity)
             {
                 val.pData = (void *)L"465";
             }
             else
             {
                 val.pData = (void *)L"25";
             }
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_SERVER_PORT,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }

         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->user_name_ptr) > 0)
         {
             val.pData = (void *)s_email_setting.send_serv_ptr->user_name_ptr;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_USER_ID,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }

         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->password_ptr) > 0)
         {
             val.pData = (void *)s_email_setting.send_serv_ptr->password_ptr;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_PASSWORD,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         if (s_email_setting.send_serv_ptr->secruity > 0)
         {
            val.pData = (void *)s_email_setting.send_serv_ptr->secruity;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_SSL_OR_TLS,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         if (s_email_setting.send_serv_ptr->authenticate > 0)
         {
            val.pData = (void *)s_email_setting.send_serv_ptr->authenticate;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_AUTHENTICATE,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
      }
      else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         uint16         index = 0;
         BOOLEAN        is_found = 0;

         index= GUILIST_GetCurItemIndex(EMAIL_SELECT_EMAIL_ACC_CTRL_ID) - 1;
         
         for (i = MN_DUAL_SYS_1; i <MMI_DUAL_SYS_MAX; i++)
         {
             if (MMIAPIPHONE_IsSimOk(i))
             {
                 is_found = TRUE; 
                 break;
             }
         }
         
         if (is_found)
         {
             val.pData = (void*)i;
         }
         else
         {
#ifdef MMI_WIFI_SUPPORT
             val.pData = (void*)EMASP_NET_PATH_WIFI;
#else
             val.pData = (void*)MN_DUAL_SYS_1;
#endif
         }
         result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_NET_PATH,&val);
         EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");

         if(index == 8)//qq email
         { 
             val.pData = 2;
         }
         else
         {
             val.pData = (void *)(s_email_setting.serv_conf_ptr[index].uiServerType + 1);
         }
         result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_RECEIVE_SERVER_TYPE,&val);
         EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         
         if (EMA_UTILS_WSTRLEN(s_email_setting.serv_conf_ptr[index].wszPop3Server) > 0)
         {
             val.pData = (void *)s_email_setting.serv_conf_ptr[index].wszPop3Server;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_SERVER,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         if (EMA_UTILS_WSTRLEN(s_email_setting.serv_conf_ptr[index].wszPop3Port) > 0)
         {
             val.pData = (void *)s_email_setting.serv_conf_ptr[index].wszPop3Port;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_PORT,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         if (s_email_setting.serv_conf_ptr[index].bPop3SSL > 0)
         {
             val.pData = (void *)s_email_setting.serv_conf_ptr[index].bPop3SSL;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_POP3_SSL_OR_TLS,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         
         if (EMA_UTILS_WSTRLEN(s_email_setting.serv_conf_ptr[index].wszImapServer) > 0)
         {
             val.pData = (void *)s_email_setting.serv_conf_ptr[index].wszImapServer;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_SERVER,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         if (EMA_UTILS_WSTRLEN(s_email_setting.serv_conf_ptr[index].wszImapPort) > 0)
         {
             val.pData = (void *)s_email_setting.serv_conf_ptr[index].wszImapPort;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_SERVER_PORT,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         if (s_email_setting.serv_conf_ptr[index].bImapSSL > 0)
         {
             val.pData = (void *)s_email_setting.serv_conf_ptr[index].bImapSSL;
             result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_IMAP4_SSL_OR_TLS,&val);
             EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         //outgoingserver
         
         val.pData = (void *)s_email_setting.serv_conf_ptr[index].wszSmtpServer;
         result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_SERVER,&val);
         EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");

         val.pData = (void *)s_email_setting.serv_conf_ptr[index].wszSmtpPort;
         result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_SERVER_PORT,&val);
         EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");         
         
         if (s_email_setting.serv_conf_ptr[index].bSmtpSSL > 0)
         {
            val.pData = (void *)s_email_setting.serv_conf_ptr[index].bSmtpSSL;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_SSL_OR_TLS,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }
         
         if (s_email_setting.serv_conf_ptr[index].bSmtpAuth > 0)
         {
            val.pData = (void *)s_email_setting.serv_conf_ptr[index].bSmtpAuth;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SMTP_AUTHENTICATE,&val);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         }

//          if (s_email_setting.serv_conf_ptr->sim1NetType)
//          {
//             SetNetSettingIndex(MN_DUAL_SYS_1, act_id, s_email_setting.serv_conf_ptr->sim1NetType);
//          }
//          
//          if (s_email_setting.serv_conf_ptr->sim2NetType)
//          {
//             SetNetSettingIndex(MN_DUAL_SYS_2, act_id, s_email_setting.serv_conf_ptr->sim2NetType);
//          }
      }
      
      //PersonalizedSetting
      val.pData = (void*)s_email_setting.load_ptr->mail_address_ptr;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_ACCOUNT_NAME,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
      
      /* download mode */
      val.pData = (void *)EMA_DOWNLOAD_ONLY_HEADERS;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
      
      /* reply email*/
      val.pData = (void *)TRUE;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_INCLUDE_ORIGINAL,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         
      /* receive new */
      val.pData = (void *)TRUE;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
            
      /* save sent */
      val.pData = (void *)TRUE;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_SAVE_SENT,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
         
       /* size limited */
      val.pData = (void *)EMA_UTILS_WSTRDUP(PNULL, (wchar*)L"3072KB");
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_MAX_MSG_SIZE,&val);
      EMA_UTILS_FREEIF(PNULL,val.pData);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
      
      /* leave copy on server */
      val.pData = (void *)TRUE;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_RECEIVER_LEAVE_COPY,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
      
      /* default folder id */
      val.pData = (void *)EMASP_INBOX_INDEX;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_EMAIL_DEFAULTINBOXID,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
      
      /* receive unseen email */      
      val.pData = (void *)TRUE;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_RECEIVE_UNSEEN,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
      
      /* receive unseen email */      
      val.pData = (void *)TRUE;
      result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,act_id,EMASP_PRFITEM_DOWNLOAD_ATTACH,&val);
      EMA_UTILS_BREAKIF(result, "EmailAUIC_CreateActProc_Enter,add opt failed");
   } while(0);
   
   
   //EMA_UTILS_RELEASEIF(act_vector_ptr);
   MMIEMAIL_VectorDelete(act_vector_ptr);
   if(SUCCESS != result)
   {
      EMA_UTILS_TRACE("CreateEmailAccount, create account failed, the error is %d", result);
//       if (EME_NOSTORAGE == result)
//       {
//          MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
//       }
//       else
//       {
//          MMIPUB_OpenAlertFailWin(TXT_EMAIL_CREATE_NEW_ACC_FAILED);   
//       }
//      return FALSE; 
   }
   
   
   /* add by jiangdingqing 2008-2-17, pass out the actid by dwparam */
   //*((WE_UINT32*)mail_address) = (WE_UINT32)act_id;
   
   return result;
}


/*****************************************************************************/
// 	Description: setting vector free 
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SettingVectorFree(void *data_ptr)
{
   IMAGE_STATIC_T *info_ptr = (IMAGE_STATIC_T *)data_ptr;
   
   if (PNULL != info_ptr)
   {
      EMA_UTILS_FREEIF(PNULL, info_ptr->pwText);
      /*      EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE, info_ptr->piImage);*/
      //EMA_UTILS_RELEASEIF(info_ptr->piImage);
      EMA_UTILS_FREE(PNULL,info_ptr);
   }
}


#ifdef MMI_WIFI_SUPPORT

/*****************************************************************************/
// 	Description : handle wifi find AP prompt
//	Global resource dependence : none
//   Author: li.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSearchedWiFi(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E  msg_id, 
                                           DPARAM            param)
{
   MMI_RESULT_E result = MMI_RESULT_TRUE;
   
   switch(msg_id)
   {
   case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
   case MSG_CTL_OK:
   case MSG_APP_WEB:
      MMIAPIWIFI_OpenListWin();
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
// 	Description : handle wifi rigister callback function
//	Global resource dependence : none
//   Author: li.li
//	Note:
/*****************************************************************************/
LOCAL void HandleWifiMsg(MMIWIFI_NOTIFY_EVENT_E event_id,MMIWIFI_NOTIFY_PARAM_T* param_ptr)
{
   BOOLEAN is_busy = FALSE;
   uint32  trans_state = 0;

   switch(event_id)
   {
   case MMIWIFI_NOTIFY_EVENT_ON:
      break;
      
   case MMIWIFI_NOTIFY_EVENT_OFF:
      break;
      
   case MMIWIFI_NOTIFY_EVENT_CONNECT:
      if (MMK_IsOpenWin(EMAIL_NETWORK_PATH_WIN_ID))
      {
         MMI_STRING_T           item_str    ={0};
         EMASP_PRFITEM_VAL      val = {0}; 

         s_email_setting.data_conn_ptr->net_path = EMASP_NET_PATH_WIFI;
         MMI_GetLabelTextByLang(TXT_WIFI, &item_str);
         GUILABEL_SetText(EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID, &item_str, FALSE);
         
         if (s_email_setting.act_id)
         {
            val.pData = (void *)s_email_setting.data_conn_ptr->net_path;
            MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_NET_PATH,&val);
         }
         MMK_CloseWin(EMAIL_NETWORK_PATH_WIN_ID);
      }
      else if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
      {
          MMK_CreateWin((uint32*)MMIEMAIL_SETTING_WIN_TAB, PNULL);
      }
      else
      {
         if(g_email_is_send == EMA_TRANS_TYPE_SEND_EMAIL)
         {
            MMIEMAIL_CreateSendingWin(0);
            EMA_UTILS_TRACE("MMIEmail_SelectSimCard the wifi is open, use wifi to send msg");
            if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
            {
               //MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_WAITING_SENDMAIL, 0, 0);
               //MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_SEND, 0, 0);
            }
            else if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
            {
               MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RESEND, 0, 0);
            }
            else if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
            {
               MMK_PostMsg(EMAIL_BOX_WIN_ID, EVT_EMA_RESEND, 0, 0);
            }
         }
         else if (g_email_is_send == EMA_TRANS_TYPE_RETRIEVE_EMAIL)
         {
            MMIEMAIL_CreateRecvingWaitingWin(0);
            EMA_UTILS_TRACE("MMIEmail_SelectSimCard the wifi is open, use wifi to retrieve msg");
            if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
            {
               MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
            }
            else if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
            {
               MMK_PostMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
            }
            else if (MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
            {
               MMK_PostMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
            }
         }
         else if (g_email_is_send == EMA_TRANS_TYPE_SEND_RR)
         {
            EMA_UTILS_TRACE("MMIEmail_SelectSimCard the wifi is open, use wifi to send rr");
            MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_SENDREADREPORT, 0, 0);
         }
      }

      EMA_UTILS_TRACE("HandleWifiMsg recive MMIWIFI_NOTIFY_EVENT_CONNECT");
      MMIEMAIL_DeRegWifiNotify();
      break;
      
   case MMIWIFI_NOTIFY_EVENT_DISCONNECT:
      EMA_UTILS_TRACE("HandleWifiMsg recive MMIWIFI_NOTIFY_EVENT_DISCONNECT");
      MMIEMAIL_SP_TRS_IsBusy(EMA_GETTRANS_HANDLE(g_email_sp_handle), &is_busy, &trans_state);
      if (is_busy)
      {
         MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_WIFI_DISCONNECT,IMAGE_PUBWIN_QUERY,PNULL,HandleSearchedWiFi);
      }
      break;
      
   case MMIWIFI_NOTIFY_EVENT_FIND_AP:
      EMA_UTILS_TRACE("HandleWifiMsg recive MMIWIFI_NOTIFY_EVENT_FIND_AP");
      MMIEMAIL_SP_TRS_IsBusy(EMA_GETTRANS_HANDLE(g_email_sp_handle), &is_busy, &trans_state);
      if (is_busy)
      {
         MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_SEARCHED_AP,IMAGE_PUBWIN_QUERY,PNULL,HandleSearchedWiFi);
      }
      break;
      
   default:
      break;
   }
}
/*****************************************************************************/
// 	Description : register wifi notify in email
//	Global resource dependence : none
//   Author: li.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_RegWifiNotify(void)
{
    MMIWIFI_NOTIFY_INFO_T  notify_info = {0};
    notify_info.module_id = MMI_MODULE_EMAIL;
    notify_info.notify_func = HandleWifiMsg;
    
    MMIAPIWIFI_RegNotifyInfo(&notify_info);
}

/*****************************************************************************/
// 	Description : deregister wifi notify in email
//	Global resource dependence : none
//   Author: li.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_DeRegWifiNotify(void)
{
    MMIAPIWIFI_DeRegNotifyInfo(MMI_MODULE_EMAIL);
}

#endif

/*****************************************************************************/
// 	Description: handle setting option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingOptWinMsg(
                                               MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param
                                               )
{
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_SETTING_OPT_CTRL_ID;
//   uint16 cur_index = 0;
//   uint16  item_num = 0;
   MMI_MENU_GROUP_ID_T group_id = 0;
   MMI_MENU_ID_T menu_id = 0;
   
   switch (msg_id)
   {
   case MSG_FULL_PAINT:
      break;
   case MSG_OPEN_WINDOW:
      GUIMENU_GetId( ctrl_id, &group_id, &menu_id);
      if (MMIEMAIL_VectorGetVectorNumber(s_email_setting.account_ptr->vector_ptr) >= 5)
      {
         GUIMENU_SetItemVisible(ctrl_id, group_id,EMAIL_SETTING_OPT_CREATE_ACC_ID,FALSE);
         GUIMENU_SetItemGrayed(ctrl_id, group_id,EMAIL_SETTING_OPT_CREATE_ACC_ID_EX,TRUE);
      }
      else 
      {
         GUIMENU_SetItemVisible(ctrl_id, group_id,EMAIL_SETTING_OPT_CREATE_ACC_ID_EX,FALSE);
      }
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
      case EMAIL_SETTING_OPT_SEND_RECEIVE_MAIL_ID:
         MMIEMAIL_PublicProc_Retrieve();
         break;
      case EMAIL_SETTING_OPT_CREATE_ACC_ID:
         s_email_setting.enter_type = SETTING_ENTER_TYPE_COMMON_ACC;
         MMK_CreateWin((uint32 *)MMIEMAIL_SELECT_EMAIL_ACC_WIN_TAB, PNULL);
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
//  Description :handle saving email window message
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitingEmailExitWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_TRACE_LOW("mmiemail_setting.c HandleWaitingEmailExitWin, MSG_OPEN_WINDOW g_email_is_engine_task_busy = %d", g_email_is_engine_task_busy);
        break;
        
    case MSG_FULL_PAINT:
        SCI_TRACE_LOW("mmiemail_setting.c HandleWaitingEmailExitWin, MSG_FULL_PAINT g_email_is_engine_task_busy = %d", g_email_is_engine_task_busy);
        err_code = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);        
        break;
    
    default :
        SCI_TRACE_LOW("mmiemail_setting.c HandleWaitingEmailExitWin, g_email_is_engine_task_busy = %d", g_email_is_engine_task_busy);
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
// 	Description: exit setting 
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_ExitSetting(void)
{   
   if (g_email_is_engine_task_busy)
   {
        MMI_STRING_T wait_str   = {0};

        SCI_TRACE_LOW("mmiemail_setting.c g_email_is_engine_task_busy is busy, Opening waiting window");
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
        MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,EMAIL_WAITING_WIN_ID,IMAGE_NULL,
           ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleWaitingEmailExitWin);
        return;
   }
   MMIEMAIL_FreeOptPtr();
   MMIEMAIL_GetMutex();
   EMA_UTILS_TRACE("----ExitSetting begin");
   if (NULL != s_email_setting.account_ptr)
   {
      MMIEMAIL_VectorDelete(s_email_setting.account_ptr->vector_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.account_ptr->act_id_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.account_ptr);
   }
   EMA_UTILS_FREEIF(PNULL, s_email_setting.serv_conf_ptr);
   
   if (NULL != g_email_sp_handle)
   {
      EMA_UTILS_TRACE("----MMIEMAIL_SP_DeleteHandle begin");
      MMIEMAIL_SP_DeleteHandle(&g_email_sp_handle);
      EMA_UTILS_TRACE("----MMIEMAIL_SP_DeleteHandle end");
   }

   EMA_UTILS_MEMSET(&s_email_setting, 0x00, sizeof(EMAIL_SETTING_T));
   if (TRUE == g_email_pdp_active)
   {
       MMIEMAIL_PDPDeactive();
   }
#ifdef MMI_WIFI_SUPPORT
   if (TRUE == g_email_is_use_wifi)
   {
       MMIEMAIL_DeRegWifiNotify();
   }
#endif
#ifdef UI_P3D_SUPPORT
    MMI_Enable3DEffect(MMI_3D_EFFECT_EMAIL);
#endif
   MMIEmail_ThreadDisable();
   TF3RD_PRINTMEMORY();
   TF3RD_DELETEMEMORY();
   HEMSSTORE_ClearPathMgr();
   //BRWENG20_DeleteEmailTaskThread();
   //MMIEMAIL_DeleteEmailTaskThread();
   EMA_UTILS_TRACE("----ExitSetting end");
   MMIEMAIL_PutMutex();
   MMK_CloseWin(EMAIL_WAITING_WIN_ID);
}

/*****************************************************************************/
// 	Description: update setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateSettingList(MMI_CTRL_ID_T ctrl_id)
{
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   
   switch(ctrl_id)
   {
   case EMAIL_SETTING_LISTBOX_CTRL_ID:
      {
         //HStoreMgr      store_mgr_ptr = PNULL;
         IMAGE_STATIC_T *info_ptr;
         EMAIL_VECTOR_T *vector_ptr = PNULL;
         MMI_STRING_T item_str = {0};
         int32        ret = 0;
         
         do 
         {
            MMIEMAIL_VectorDelete(s_email_setting.account_ptr->vector_ptr);
            s_email_setting.account_ptr->vector_ptr = PNULL;
             
            ret = MMIEMAIL_VectorNew(&s_email_setting.account_ptr->vector_ptr);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_VectorNew failed, s_email_setting.account_ptr->vector_ptr is NULL");
            ret = MMIEMAIL_VectorNew(&vector_ptr);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_VectorNew failed, vector_ptr is NULL");
             
            ret = MMIEMAIL_SP_Store_GetActInfo(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),0,EMASP_ACTINFO_MAX_NUMBER,vector_ptr);
            EMA_UTILS_BREAKIF(ret, "UpdateSettingList--MMIEMAIL_SP_Store_GetActInfo failed");

            s_email_setting.account_ptr->act_cnt = MMIEMAIL_VectorGetVectorNumber(vector_ptr);
            
            if(s_email_setting.account_ptr->act_cnt > 0)
            {
               uint32 item_cnt = 0;
               EMASP_ACTINFO_T * act_info_ptr = PNULL;
//               wchar   *info_str_ptr = PNULL;
               
               EMA_UTILS_FREEIF(PNULL, s_email_setting.account_ptr->act_id_ptr);
               s_email_setting.account_ptr->act_id_ptr = EMA_UTILS_MALLOC(PNULL, ((uint32)s_email_setting.account_ptr->act_cnt) * sizeof(uint32));
               EMA_UTILS_BREAKNULL(s_email_setting.account_ptr->act_id_ptr, recode, "HandleEmailSettingWinMsg, malloc pMe->act_id_ptr failed");
               
               for(item_cnt = 0;item_cnt < (uint32)s_email_setting.account_ptr->act_cnt;item_cnt ++)
               {
                  MMIEMAIL_VectorGetAt(vector_ptr, item_cnt, (void **)&act_info_ptr);
                  info_ptr = (IMAGE_STATIC_T *)EMA_UTILS_MALLOC(PNULL, sizeof(IMAGE_STATIC_T));
                  EMA_UTILS_BREAKNULL(info_ptr, recode, "HandleEmailSettingWinMsg, info_ptr PNULL");
                  EMA_UTILS_MEMSET(info_ptr, 0, sizeof(IMAGE_STATIC_T));   
                  
                  info_ptr->pwText = EMA_UTILS_WSTRDUP(PNULL, act_info_ptr->act_name);
                  EMA_UTILS_BREAKNULL(info_ptr->pwText, recode, "HandleEmailSettingWinMsg, info_ptr->pwszStatic1 PNULL");
                  
                  AppendListItem(info_ptr->pwText, EMA_UTILS_WSTRLEN(info_ptr->pwText), ctrl_id, PNULL);
                  
                  MMIEMAIL_VectorInsert(s_email_setting.account_ptr->vector_ptr, (void *)info_ptr);
                  s_email_setting.account_ptr->act_id_ptr[item_cnt] = act_info_ptr->act_id;
               }
               
               MMIEMAIL_VectorSetPfnFree(s_email_setting.account_ptr->vector_ptr, SettingVectorFree);
               
               MMI_GetLabelTextByLang(TXT_EMAIL_ACCOUNT_DELETE, &item_str);
               AppendListItem(item_str.wstr_ptr, item_str.wstr_len, ctrl_id, PNULL);

                 MMI_GetLabelTextByLang(TXT_SYSTEM_SETTING, &item_str);
                 AppendListItem(item_str.wstr_ptr, item_str.wstr_len, ctrl_id, PNULL);
            }
            else
            {
                 MMI_GetLabelTextByLang(TXT_EMAIL_NO_ACCOUNT, &item_str);
                 AppendListItem(item_str.wstr_ptr, item_str.wstr_len, ctrl_id, PNULL);
            }
         } while (0);

         MMIEMAIL_VectorDelete(vector_ptr);
      }
      break;
   default:
       break;
   }
   return recode;   
}

/*****************************************************************************/
// 	Description: handle account delete query window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAccDelQueryWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                )
{
   MMI_RESULT_E        result = MMI_RESULT_TRUE;
   int32               ret = 0;
   
   switch(msg_id)
   {
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      {
         uint32 num  = 0;
         uint32 *act_id_ptr = PNULL;
         int    iSize = 0;
         int    i = 0;
         MMI_STRING_T         wait_str = {0};
         
         iSize = MMIEMAIL_VectorGetVectorNumber(s_email_setting.account_ptr->vector_ptr);
         act_id_ptr = EMA_UTILS_MALLOC(PNULL, ((uint32)iSize) * sizeof(uint32));
         EMA_UTILS_BREAKNULL(act_id_ptr, ret, "out of memory");
         EMA_UTILS_MEMSET(act_id_ptr, 0x00, ((uint32)iSize) * sizeof(uint32));
         for (i =0; i < iSize; i++)
         {
            if (s_email_setting.check_status[i])
            {
               act_id_ptr[num] = s_email_setting.account_ptr->act_id_ptr[i];
               num++;
            }
         }
         
         MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
         MMIPUB_OpenWaitWin(1, &wait_str, PNULL, PNULL, EMAIL_WAITING_WIN_ID, IMAGE_NULL,
                    ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, (MMIPUB_HANDLE_FUNC)HandleSettingWaitWinMsg);
         MMIEMAIL_SP_Store_DeleteMultiAccount(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle), act_id_ptr, num);  
         EMA_UTILS_FREE(PNULL, act_id_ptr);
      }
      MMK_CloseWin(win_id);
      break;
      
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
      MMK_CloseWin(win_id);
      break;
      
   default:
      result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
      break;
   }
   
   return result;
}

/*****************************************************************************/
// 	Description: add account data
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void AddAccDelData(void)
{
   GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
   GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
   int   iSize = 0;
   int   i = 0;
   IMAGE_STATIC_T *info_ptr = PNULL;

   item_data.softkey_id[0] = TXT_NULL;
   item_data.softkey_id[2] = STXT_RETURN;
   item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
   item_t.item_data_ptr = &item_data;

   iSize = MMIEMAIL_VectorGetVectorNumber(s_email_setting.account_ptr->vector_ptr);
   for (i= 0;  i < iSize; i++)
   {
      MMIEMAIL_VectorGetAt(s_email_setting.account_ptr->vector_ptr, i, (void **)&info_ptr);
      //AppendListItemEx(info_ptr->pwText, EMA_UTILS_WSTRLEN(info_ptr->pwText), ctrl_id, 0, 2);
      
      item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
      if (s_email_setting.check_status[i])
      {
         item_data.item_content[0].item_data.image_id= IMAGE_CHECK_SELECTED_ICON;
         item_data.softkey_id[0] = TXT_COMMON_OK;
#if !defined(TOUCH_PANEL_SUPPORT)
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
         item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_CANCEL_ICON; 
#else
         item_data.softkey_id[1] = STXT_CANCEL;  
#endif
#endif
      }
      else
      {
         item_data.item_content[0].item_data.image_id= IMAGE_CHECK_UNSELECTED_ICON;
#if !defined(TOUCH_PANEL_SUPPORT)
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
         item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_SELECTED_ICON;
#else
         item_data.softkey_id[1] = STXT_SELECT;
#endif
#endif
      }
      
      item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
      item_data.item_content[1].item_data.text_buffer.wstr_len = EMA_UTILS_WSTRLEN(info_ptr->pwText);
      item_data.item_content[1].item_data.text_buffer.wstr_ptr = info_ptr->pwText;
      
      GUILIST_AppendItem(EMAIL_ACCOUNT_DELETE_LISTBOX_CTRL_ID, &item_t);   
    }
}

/*****************************************************************************/
// 	Description: handle account delete window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAccountDeleteWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_ACCOUNT_DELETE_LISTBOX_CTRL_ID;
   uint16 selected_index = 0;
   int i =0;
   MMI_TEXT_ID_T   leftsoft_id = TXT_NULL;   // the left softkey id
   BOOLEAN     is_found = 0;
   
   switch(msg_id)
   {
   case MSG_FULL_PAINT:
      {
         MMIEMAIL_CLEAR_CLIENT(win_id); 
         MMK_SetAtvCtrl(win_id, ctrl_id);
      }
      break;
   case EVT_EMA_UPDATE_LISTBOX_TAB:
      {
         GUILIST_RemoveAllItems(ctrl_id);
         AddAccDelData();
         GUILIST_SetCurItemIndex(ctrl_id,s_email_setting.check_status[SETTING_ACC_MAX]);
      }
      MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);      
      break;

   case MSG_GET_FOCUS:
      MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); 
      break;
   case MSG_OPEN_WINDOW:
      {
         int   iSize = 0;

         iSize = MMIEMAIL_VectorGetVectorNumber(s_email_setting.account_ptr->vector_ptr);
         
         EMA_UTILS_MEMSET(&s_email_setting.check_status, 0x00, SETTING_ACC_MAX + 1);
         
         GUILIST_SetMaxItem(ctrl_id,iSize,FALSE);
         GUILIST_SetMaxSelectedItem(ctrl_id,iSize);
  
         AddAccDelData();
#if defined(MMI_PDA_SUPPORT) 
        GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
         MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
         MMK_SetAtvCtrl(win_id, ctrl_id);
      }
      break;

   case MSG_APP_OK:
   case MSG_CTL_OK:
      {
         for (i=0; i < SETTING_ACC_MAX; i++)
         {
            if (s_email_setting.check_status[i])
            {
               is_found = TRUE;
               break;
            }
         }

         if (is_found)
         {
            MMI_WIN_ID_T query_win_id = EMAIL_DELETECCOUNT_QUERY_WIN_ID;
            MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DELETE_ACCOUNT_QUAERE, IMAGE_PUBWIN_QUERY, &query_win_id, HandleAccDelQueryWinMsg);     
         }
      }
      
      break;

   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_NOTIFY_LIST_MOVEBOTTOM:
   case MSG_NOTIFY_LIST_MOVETOP:   
   case MSG_NOTIFY_LIST_MOVEDOWN:  
   case MSG_NOTIFY_LIST_MOVEUP:
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            { 
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);  
                return MMI_RESULT_TRUE;
            }
        }
#endif
      {
         selected_index = GUILIST_GetCurItemIndex(ctrl_id);
         if (MSG_CTL_MIDSK == msg_id || MSG_APP_WEB == msg_id || MSG_CTL_PENOK == msg_id)
         {
            s_email_setting.check_status[SETTING_ACC_MAX] = (char)selected_index;
            /* Switch slect and unselect status */
            s_email_setting.check_status[selected_index] = 0x01 & (~ s_email_setting.check_status[selected_index]);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
         }

         for (i=0; i < SETTING_ACC_MAX; i++)
         {
            if (s_email_setting.check_status[i])
            {
               leftsoft_id = TXT_COMMON_OK;
               break;
            }
         }
         
#if defined(MMI_PDA_SUPPORT) 
         if (TXT_NULL != leftsoft_id)
         {
                GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
         }
         else
         {
                GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
         }
         MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#else
#endif
      }
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      recode = MMI_RESULT_FALSE;
      break;
   }    
   
   return recode;
}

/*****************************************************************************/
// 	Description: 
//	Global resource dependence:
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_OpenNetSelectWin(void)
{
    MMK_CreateWin((uint32*)MMIEMAIL_DATA_CONNECTION_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle account setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAccountSettingWinMsg(
                                             MMI_WIN_ID_T        win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM                param
                                             )
{
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_ACCOUNT_SETTING_LISTBOX_CTRL_ID;
   uint16 selected_index = 0;
   int    i = 0;
   MMI_STRING_T item_str = {0};
   
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
      {
         GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE );
         //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
         
         for (i=0; i < SETTING_ACCOUNT_SETTINGS_MAX; i++)
         {
            MMI_GetLabelTextByLang(TXT_EMAIL_PERSONALIZED_SETTING + i, &item_str);
            AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, ctrl_id, 0, 0);
         }
         
         GUILIST_SetSelectedItem(ctrl_id,0,TRUE);
         MMK_SetAtvCtrl(win_id, ctrl_id);
      }
      break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
   case MSG_CTL_OK:
   case MSG_APP_OK:
      selected_index = GUILIST_GetCurItemIndex(ctrl_id);
      switch(selected_index)
      {
      case SETTING_ACCOUNT_SETTINGS_PERSONALIZED:
         MMK_CreateWin((uint32*)MMIEMAIL_PERSONALIZED_SETTING_WIN_TAB, PNULL);
         break;
      case SETTING_ACCOUNT_SETTINGS_LOAD:
         MMK_CreateWin((uint32*)MMIEMAIL_LOAD_SETTING_WIN_TAB, PNULL);
         break;
      case SETTING_ACCOUNT_SETTINGS_DATA_CONNECTION:
         MMK_CreateWin((uint32*)MMIEMAIL_DATA_CONNECTION_WIN_TAB, PNULL);
         break;
      case SETTING_ACCOUNT_SETTINGS_INCOMING_SERV:
         MMK_CreateWin((uint32*)MMIEMAIL_INCOMING_SETTING_WIN_TAB, PNULL);
         break;
      case SETTING_ACCOUNT_SETTINGS_OUTGOING_SERV:
         MMK_CreateWin((uint32*)MMIEMAIL_OUTGOING_SETTING_WIN_TAB, PNULL);
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
      break;
   default:
      recode = MMI_RESULT_FALSE;
      break;
   }
   
   return recode;
}


/*****************************************************************************/
// 	Description: handle setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEmailSettingWinMsg(
                                             MMI_WIN_ID_T        win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM                param
                                             )
{
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_SETTING_LISTBOX_CTRL_ID;
  // uint16  item_num = 0;
   uint16 selected_index = 0;
   int    iSize = 0;
   uint32          net_id    = 0;
   
   switch (msg_id)
   {
   case MSG_FULL_PAINT:
      MMIEMAIL_CLEAR_CLIENT(win_id);
//       item_num = GUILIST_GetTotalItemNum(ctrl_id);
//       if (0 == item_num)
//       {
//          GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
//       }
      MMK_SetAtvCtrl(win_id, ctrl_id);
      break;
   case EVT_EMA_UPDATE_LISTBOX_TAB:
      {
         GUILIST_RemoveAllItems(ctrl_id);
         UpdateSettingList(ctrl_id);
      }
      MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);      
      break;
   case MSG_OPEN_WINDOW:
      {
         //HStoreMgr      store_mgr_ptr = PNULL;       
         IMAGE_STATIC_T *info_ptr;
         EMAIL_VECTOR_T *vector_ptr = PNULL;
         MMI_STRING_T item_str = {0};
         int32        ret = 0;

         do
         {      
             GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE );
             s_email_setting.account_ptr = (SETTING_ACCOUNT_T *)EMA_UTILS_MALLOC(PNULL, sizeof(SETTING_ACCOUNT_T));  
             EMA_UTILS_BREAKNULL(s_email_setting.account_ptr, ret, "malloc s_email_setting.account_ptr failed");
             EMA_UTILS_MEMSET(s_email_setting.account_ptr, 0x00, sizeof(SETTING_ACCOUNT_T));

             ret = MMIEMAIL_VectorNew(&s_email_setting.account_ptr->vector_ptr);
             EMA_UTILS_BREAKIF(ret, "MMIEMAIL_VectorNew failed, s_email_setting.account_ptr->vector_ptr is NULL");
             ret = MMIEMAIL_VectorNew(&vector_ptr);
             EMA_UTILS_BREAKIF(ret, "MMIEMAIL_VectorNew failed, vector_ptr is NULL");
             
             ret = MMIEMAIL_SP_Store_GetActInfo(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),0,EMASP_ACTINFO_MAX_NUMBER,vector_ptr);
             EMA_UTILS_BREAKIF(ret, "HandleEmailSettingWinMsg, MMIEMAIL_SP_Store_GetActInfo failed");
             
             s_email_setting.account_ptr->act_cnt = MMIEMAIL_VectorGetVectorNumber(vector_ptr);
             
            if(s_email_setting.account_ptr->act_cnt > 0)
            {
               uint32 item_cnt = 0;
               EMASP_ACTINFO_T * act_info_ptr = PNULL;
//               wchar   *info_str_ptr = PNULL;
               s_email_setting.account_ptr->act_id_ptr = EMA_UTILS_MALLOC(PNULL, ((uint32)s_email_setting.account_ptr->act_cnt) * sizeof(uint32));
               EMA_UTILS_BREAKNULL(s_email_setting.account_ptr->act_id_ptr, recode, "HandleEmailSettingWinMsg, malloc pMe->act_id_ptr failed");
               for(item_cnt = 0;item_cnt < (uint32)s_email_setting.account_ptr->act_cnt;item_cnt ++)
               {   
                  MMIEMAIL_VectorGetAt(vector_ptr, item_cnt, (void **)&act_info_ptr);
                  info_ptr = (IMAGE_STATIC_T *)EMA_UTILS_MALLOC(PNULL, sizeof(IMAGE_STATIC_T));
                  EMA_UTILS_BREAKNULL(info_ptr, recode, "HandleEmailSettingWinMsg, info_ptr PNULL");
                  EMA_UTILS_MEMSET(info_ptr, 0, sizeof(IMAGE_STATIC_T));
                  info_ptr->pwText = EMA_UTILS_WSTRDUP(PNULL, act_info_ptr->act_name);
                  EMA_UTILS_BREAKNULL(info_ptr->pwText, recode, "HandleEmailSettingWinMsg, info_ptr->pwszStatic1 PNULL");
                  AppendListItem(info_ptr->pwText, EMA_UTILS_WSTRLEN(info_ptr->pwText), ctrl_id, PNULL);
                  MMIEMAIL_VectorInsert(s_email_setting.account_ptr->vector_ptr, (void *)info_ptr);
                  s_email_setting.account_ptr->act_id_ptr[item_cnt] = act_info_ptr->act_id;
               }
               MMIEMAIL_VectorSetPfnFree(s_email_setting.account_ptr->vector_ptr, SettingVectorFree);

               MMI_GetLabelTextByLang(TXT_EMAIL_ACCOUNT_DELETE, &item_str);
               AppendListItem(item_str.wstr_ptr, item_str.wstr_len, ctrl_id, PNULL);
            }
         }while(0);         

         MMI_GetLabelTextByLang(TXT_SYSTEM_SETTING, &item_str);
         AppendListItem(item_str.wstr_ptr, item_str.wstr_len, ctrl_id, PNULL);

         MMIEMAIL_VectorDelete(vector_ptr);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
         MMK_SetActiveCtrl(win_id, ctrl_id);
      }
      break;
   case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
      selected_index = GUILIST_GetCurItemIndex(ctrl_id);
      iSize = MMIEMAIL_VectorGetVectorNumber(s_email_setting.account_ptr->vector_ptr);
      if (selected_index  + 1 > iSize)
      {
         if (selected_index == iSize)
         {
            if (iSize > 0)
            {
               MMK_CreateWin((uint32*)MMIEMAIL_ACCOUNT_DELETE_WIN_TAB, PNULL);
            }
            else
            {
                if (s_email_setting.account_ptr->act_cnt > 0)
                {
                    MMIEMAIL_EntrySystemSetting();
                }
            }
         }
         else
         {
            if (s_email_setting.account_ptr->act_cnt > 0)
            {
                MMIEMAIL_EntrySystemSetting();
            }
         }
      }
      else
      {
         if (s_email_setting.account_ptr->act_cnt > 0)
         {
            EMASP_PRFITEM_VAL      val = {0}; 
            int                    i= 0;
            BOOLEAN                is_found = FALSE;
	     int32                 ret =0;
            s_email_setting.act_id = s_email_setting.account_ptr->act_id_ptr[selected_index];
            ret = MMIEMAIL_SP_CFG_SetActiveAct(EMA_GETCFG_HANDLE(g_email_sp_handle), s_email_setting.act_id);
            if (SCI_SUCCESS!=ret)
	     {
		EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetActiveAct,error"));
	     }
			
            ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_NET_PATH,&val);
	     if (SCI_SUCCESS!=ret)
	     {
		EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem,error"));
	     }
		 
	     if ((uint32)val.pData < MMI_DUAL_SYS_MAX && !MMIAPIPHONE_IsSimOk((uint32)val.pData))
            {
               for (i = MN_DUAL_SYS_1; i <MMI_DUAL_SYS_MAX; i++)
               {
                  if (MMIAPIPHONE_IsSimOk(i))
                  {
                     is_found = TRUE; 
                     break;
                  }
               }

               if (is_found)
               {
                  val.pData = (void*)i;
               }
               else
               {
#ifdef MMI_WIFI_SUPPORT
                  val.pData = (void*)EMASP_NET_PATH_WIFI;
#else
                  val.pData = (void*)MN_DUAL_SYS_1;
#endif
               }
               MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_NET_PATH,&val);
            }
         }

         MMIEMAIL_CreateMainmenuWin();
      }
      break;
	  
#if defined(MMI_PDA_SUPPORT) 
    case MSG_APP_MENU:
#endif
   case MSG_CTL_OK:
   case MSG_APP_OK:
      MMK_CreateWin((uint32 *)MMIEMAIL_SETTING_OPT_WIN_TAB, PNULL);
      break;
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
		if(MMIAPIEMAIL_GetIsPbSendEdit())
		{
		   MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc ={0};
		   MMIAPIEMAIL_SetSendVcardDocument(&pb_edit_doc);
		   MMIAPIEMAIL_SetIsPbSendEdit(FALSE);
           
		}
#endif
      if (!g_save_email_on_red_key)
      {
          if(g_email_is_engine_task_busy)
          {
              g_email_is_engine_task_busy = FALSE;
          }
            MMIEMAIL_ExitSetting();
      }
      break;
   case EVT_EMA_FACTORY_RESET:
       if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
       {
           MMK_CloseWin(EMAIL_MAIN_MENU_WIN_ID);
       }
      MMK_CloseWin(EMAIL_FACTORY_RESET_WIN_ID);
#ifdef MMI_GUI_STYLE_TYPICAL
      MMIPUB_OpenAlertSuccessWin(TXT_EMAIL_RESTORE_COMPLETE);
#endif
      MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
      break;
   case EVT_EMA_DEL_ACC_RET:
       MMK_CloseWin(EMAIL_ACCOUNT_DELETE_WIN_ID);
       MMK_CloseWin(EMAIL_WAITING_WIN_ID);
#ifdef MMI_GUI_STYLE_TYPICAL
       MMIPUB_OpenAlertSuccessWin(TXT_COMMON_DELETESUCESS);
#endif
       MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
       break;
   case EVT_EMA_RECEIVE:
      EMA_UTILS_LOG_INFO(("HandleEmailMainMenuOptWinMsg EVT_EMA_INNER_INBOX_RECEIVE"));
      switch (g_email_trans_type)
      {
      case MMIEMAIL_TRANS_MSGHEAD:
          if (param)
          {
              net_id= *((uint32*)param);
          }
          EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_RetrieveActEmailHead, the net id is %d", net_id);
          if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_RetrieveActEmailHead(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, (char *)"Inbox", net_id))
          {
              EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_RetrieveHead, MMIEMAIL_SP_TRS_RetrieveActEmailHead failed."));
              return recode;
          }
          break;
      case MMIEMAIL_TRANS_MSG:
          if (param)
          {
              net_id= *((uint32*)param);
          }
          EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_RetrieveActEmail, the net id is %d", net_id);
          if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_RetrieveActEmail(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, (char *)"Inbox", net_id))
          {
              EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_RetrieveHeadBody, MMIEMAIL_SP_TRS_RetrieveActEmail failed."));
              return FALSE;
          }
          break;
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

/*****************************************************************************/
// 	Description: get email password edit rect
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetEmailPasswordEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label)
{
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
   return GetPasswordEditBothRect(is_need_label);
#elif defined MAINLCD_SIZE_176X220
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
    if (is_need_label)
    {
        both_rect.h_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN_H + SETTING_PASSWORD_LABEL_HEIGHT_H + MMI_PASSWORD_EDIT_TOP_MARGIN_H;
        both_rect.v_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN + SETTING_PASSWORD_LABEL_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
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
}
#else
   GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
   
   both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
   if (is_need_label)
   {
      both_rect.h_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN + SETTING_PASSWORD_LABEL_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
      both_rect.v_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN + SETTING_PASSWORD_LABEL_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
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

/*****************************************************************************/
// 	Description: handle setting password message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingPasswordWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM                param
                                                     )
{
   MMI_RESULT_E err_code = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID;
   MMI_STRING_T  edit_string = {0};
   GUI_BOTH_RECT_T edit_both_rect = {0};
   EMASP_PRFITEM_VAL      val = {0}; 
   EMASP_PROFILE_ITEM     opt_id = 0;
//   wchar                  pwd_str[EMA_POP3_SERVER_PORT_LENGTH] = {0};
   MMI_STRING_T  label_string = {0};
   int32         ret = 0;
   wchar                  str_temp[EMA_ACCOUNT_USERINFO_NAME_LENGTH + 1] = {0};
   wchar forbid_char_value[]= {0x0d, 0x0a};
   
   EMA_UTILS_TRACE("HandleSettingPasswordWinMsg msg_id = 0x%x", msg_id);
   switch(msg_id)
   {
      
   case MSG_OPEN_WINDOW:
      MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
      {
         GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
         GUI_RECT_T title_rect = GetPasswordTitleRect();
         
         GUIWIN_SetTitleRect(win_id, title_rect);	
         GUIAPICTRL_SetBothRect(EMAIL_SETTING_PASSWROD_LABEL_CTRL_ID, &both_rect);
      }
#elif defined MAINLCD_SIZE_176X220//该尺寸下的横屏，输入法虚拟键盘打开时，导致edit高度不够，引起assert
{
         GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
         
         both_rect.h_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN_H;
         both_rect.v_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN;
         both_rect.h_rect.bottom = both_rect.h_rect.top + SETTING_PASSWORD_LABEL_HEIGHT_H;
         both_rect.v_rect.bottom = both_rect.v_rect.top + SETTING_PASSWORD_LABEL_HEIGHT;
         
         GUIAPICTRL_SetBothRect(EMAIL_SETTING_PASSWROD_LABEL_CTRL_ID, &both_rect);
}
#else					
      {
         GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
         
         both_rect.h_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN;
         both_rect.v_rect.top += SETTING_PASSWORD_LABEL_TOP_MARGIN;
         both_rect.h_rect.bottom = both_rect.h_rect.top + SETTING_PASSWORD_LABEL_HEIGHT;
         both_rect.v_rect.bottom = both_rect.v_rect.top + SETTING_PASSWORD_LABEL_HEIGHT;
         
         GUIAPICTRL_SetBothRect(EMAIL_SETTING_PASSWROD_LABEL_CTRL_ID, &both_rect);
      }
#endif
      IGUICTRL_SetState(MMK_GetCtrlPtr(EMAIL_SETTING_PASSWROD_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
#endif

      edit_both_rect = GetEmailPasswordEditRect(win_id, TRUE);
      GUIAPICTRL_SetBothRect(EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID, &edit_both_rect); 
      GUIEDIT_ConfigDisableChar(EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
      GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
      GUIEDIT_SetPasswordStyle(EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID, GUIEDIT_PASSWORD_STYLE_ALL);
      MMIAPISET_SetPwBg(win_id,IMAGE_COMMON_BG);
      if (SETTING_PASSWORD_TYPE_PASSWORD == s_email_setting.password_type)
      {
         GUIWIN_SetTitleTextId(win_id,TXT_COMMON_PASSWORD,FALSE);
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID,s_email_setting.load_ptr->password_ptr,EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr)); 
         }
#ifdef MMI_GUI_STYLE_TYPICAL
         GUILABEL_SetTextById(EMAIL_SETTING_PASSWROD_LABEL_CTRL_ID, TXT_EMAIL_LOAD_PASSWORD, FALSE);
#endif
      }
      else if (SETTING_PASSWORD_TYPE_SMTP_PASSWORD == s_email_setting.password_type)
      {
         GUIWIN_SetTitleTextId(win_id,TXT_COMMON_PASSWORD,FALSE);
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->password_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID,s_email_setting.send_serv_ptr->password_ptr,EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->password_ptr)); 
         }
#ifdef MMI_GUI_STYLE_TYPICAL
         GUILABEL_SetTextById(EMAIL_SETTING_PASSWROD_LABEL_CTRL_ID, TXT_COMMON_PASSWORD, FALSE);
#endif
      }

      MMK_SetAtvCtrl(win_id,EMAIL_SETTING_PASSWROD_EDIT_CTRL_ID);
      break;
      
#ifdef MMI_GUI_STYLE_TYPICAL
   case MSG_FULL_PAINT:
      MMIAPISET_SetPwBg(win_id,IMAGE_NUMBERINPUT_PIN);
      break;
#else ifdef MAINLCD_SIZE_128X160 || ifdef MAINLCD_SIZE_176X220
   case MSG_FULL_PAINT:
      MMIAPISET_SetPwBg(win_id,IMAGE_COMMON_BG);
      break;
#endif

   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      {
         EMA_UTILS_MEMSET(&edit_string, 0, sizeof(MMI_STRING_T));
         GUIEDIT_GetString( ctrl_id, &edit_string);
         //EMA_UTILS_MEMSET(edit_string.wstr_ptr + edit_string.wstr_len , 0x00, sizeof(wchar));
         EMA_UTILS_MEMCPY(str_temp, edit_string.wstr_ptr, edit_string.wstr_len * sizeof(wchar));
         if (edit_string.wstr_len > 0)
         {
            label_string.wstr_ptr = (wchar*)SETTING_PASSWORD_SHOW_STRING;
            label_string.wstr_len = edit_string.wstr_len;
         }

         if (SETTING_PASSWORD_TYPE_PASSWORD == s_email_setting.password_type)
         {
            EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->password_ptr);
            s_email_setting.load_ptr->password_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
            EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->password_ptr, ret, "out of memory");
            GUILABEL_SetText(EMAIL_PASSWORD_LABEL_CTRL_ID, &label_string,FALSE); 
            opt_id = EMASP_PRFITEM_EMAIL_PASSWORD;
         }
         else if (SETTING_PASSWORD_TYPE_SMTP_PASSWORD == s_email_setting.password_type)
         {
            EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->password_ptr);
            s_email_setting.send_serv_ptr->password_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
            EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->password_ptr, ret, "out of memory");
            GUILABEL_SetText(EMAIL_SMTP_PASSWORD_LABEL_CTRL_ID, &label_string,FALSE); 
            opt_id = EMASP_PRFITEM_SMTP_PASSWORD;
         }
   
         if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
         {
            val.pData = str_temp;
            MMIEMAIL_SP_Store_SetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle),s_email_setting.act_id,opt_id,&val);
         }
         MMK_CloseWin(win_id);
      }
      
      break;
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
      //todo 返回编辑主界面 
      MMK_CloseWin(win_id);
      break;

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        break;
        
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;   
}

/*****************************************************************************/
// 	Description: handle setting edit window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingEditWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM                param
                                                     )
{
   MMI_RESULT_E err_code = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_SETTING_EDIT_CTRL_ID;
   MMI_STRING_T  edit_string = {0};
   EMASP_PRFITEM_VAL      val = {0}; 
   EMASP_PROFILE_ITEM     opt_id = EMASP_PRFITEM_ACCOUNT_EMAIL;
   int32         ret = 0;
   wchar                  str_temp[EMA_ACCOUNT_USERINFO_NAME_LENGTH + 1] = {0};
   
   EMA_UTILS_TRACE("HandleSettingEditWinMsg msg_id = 0x%x", msg_id);
   switch(msg_id)
   {

   case MSG_OPEN_WINDOW:
      //todo 如果存在就显示主题
      if (SETTING_EDIT_TYPE_MAIL_ADDR == s_email_setting.edit_type)
      {
         GUIWIN_SetTitleTextId(win_id,TXT_EMAIL_ACC_ADDR,FALSE);
         GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
         GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_EMAIL);
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID,s_email_setting.load_ptr->mail_address_ptr,EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr)); 
         }
         GUIEDIT_SetCursorCurPos(ctrl_id, 0);
      }
      else if (SETTING_EDIT_TYPE_USERNAME == s_email_setting.edit_type)
      {
         GUIEDIT_SetTextMaxLen(ctrl_id, EMA_USERINFO_USER_ID_LENGTH, EMA_USERINFO_USER_ID_LENGTH);
         GUIWIN_SetTitleTextId(win_id,TXT_COMMON_USERNAME,FALSE);
         GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID,s_email_setting.load_ptr->user_name_ptr,EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr)); 
         } 
      }
      else if (SETTING_EDIT_TYPE_INCOMING_SERVER == s_email_setting.edit_type)
      {
         GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
         GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);
         GUIWIN_SetTitleTextId(win_id,TXT_EMAIL_SERVERNAME,FALSE);
         if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID,s_email_setting.recv_serv_ptr->incoming_server_ptr,EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr)); 
         }
      }
      else if (SETTING_EDIT_TYPE_SMTP_SERVER == s_email_setting.edit_type)
      {
         GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
         GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);
         GUIWIN_SetTitleTextId(win_id,TXT_EMAIL_SMTPSERVERNAME,FALSE);
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID,s_email_setting.send_serv_ptr->outgoing_server_ptr,EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr)); 
         }
      }
      else if (SETTING_EDIT_TYPE_SMTP_USERNAME == s_email_setting.edit_type)
      {
         GUIEDIT_SetTextMaxLen(ctrl_id, EMA_USERINFO_USER_ID_LENGTH, EMA_USERINFO_USER_ID_LENGTH);
         GUIWIN_SetTitleTextId(win_id,TXT_COMMON_USERNAME,FALSE);
         GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->user_name_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID,s_email_setting.send_serv_ptr->user_name_ptr,EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->user_name_ptr)); 
         }
      }
      else if (SETTING_EDIT_TYPE_SMTP_MAILNAME == s_email_setting.edit_type)
      {
         GUIWIN_SetTitleTextId(win_id,TXT_EMAIL_ACCOUNT_NAME,FALSE);
         GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
         GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_EMAIL);
         if (EMA_UTILS_WSTRLEN(s_email_setting.personalized_ptr->acc_name_ptr))
         {
            GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID, s_email_setting.personalized_ptr->acc_name_ptr,EMA_UTILS_WSTRLEN(s_email_setting.personalized_ptr->acc_name_ptr)); 
         }
      }
      else if (SETTING_EDIT_TYPE_PORT == s_email_setting.edit_type)
      {
         GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL); /*lint !e655*/
         GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_PURE_NUM);
         GUIEDIT_SetTextMaxLen(ctrl_id, EMA_POP3_SERVER_PORT_LENGTH, EMA_POP3_SERVER_PORT_LENGTH);
         GUIWIN_SetTitleTextId(win_id,TXT_COMM_PORT,FALSE);
         if (EMAIL_INCOMING_SETTING_WIN_ID == MMK_GetPrevWinId(win_id))
         {
            if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->port_ptr))
            {
               GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID,s_email_setting.recv_serv_ptr->port_ptr,EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->port_ptr)); 
            }
         }
         else
         {
            if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->port_ptr))
            {
               GUIEDIT_SetString(EMAIL_SETTING_EDIT_CTRL_ID,s_email_setting.send_serv_ptr->port_ptr,EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->port_ptr)); 
            }
         }
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
       {
           EMA_UTILS_MEMSET(&edit_string, 0, sizeof(MMI_STRING_T));
           GUIEDIT_GetString( ctrl_id, &edit_string);
           //EMA_UTILS_MEMSET(edit_string.wstr_ptr + edit_string.wstr_len , 0x00, sizeof(wchar));
           EMA_UTILS_MEMCPY(str_temp, edit_string.wstr_ptr, edit_string.wstr_len * sizeof(wchar));
           if (SETTING_EDIT_TYPE_MAIL_ADDR == s_email_setting.edit_type)
           {
               int result = 0;
               uint32 item_cnt = 0;
               
               result = MMIEMAIL_EditCheckFieldLegality(str_temp);
               if (result == MMIEMAIL_ERROR_INVALID_ADDR)
               {
                   MMIPUB_OpenAlertWarningWin( TXT_COMMON_MAIL_ADDR_INVALID);
                   break;
               }
               
               if (PNULL != s_email_setting.account_ptr)
               {
                   int32  rett =0;
                   for(item_cnt = 0;item_cnt < (uint32)s_email_setting.account_ptr->act_cnt;item_cnt ++)
                   {
                       rett = MMIEMAIL_SP_Store_GetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle), s_email_setting.account_ptr->act_id_ptr[item_cnt], EMASP_PRFITEM_ACCOUNT_EMAIL, &val);
                       if (EMAIL_SUCCESS != rett)
                       {
                           EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem failed!~"));
                       }
                       if((SETTING_ENTER_TYPE_EXSIT_ACC != s_email_setting.enter_type 
                           || (SETTING_ENTER_TYPE_EXSIT_ACC == s_email_setting.enter_type && s_email_setting.account_ptr->act_id_ptr[item_cnt] != s_email_setting.act_id)) 
                           && 0 == EMA_UTILS_WSTRCMP(val.pData,(wchar *)str_temp))
                       {
                           MMIPUB_OpenAlertWarningWin(TXT_EMAIL_ACC_EXIST);
                           rett = EMAIL_ERROR;
                           break;
                       }
                   }
                   EMA_UTILS_BREAKIF(rett, "TXT_EMAIL_ACC_EXIST");
               }
               EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->mail_address_ptr);
               s_email_setting.load_ptr->mail_address_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
               EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->mail_address_ptr, ret, "out of memory");
               GUILABEL_SetText(EMAIL_EMAIL_ADDRESS_LABEL_CTRL_ID, &edit_string, FALSE); 
               opt_id = EMASP_PRFITEM_ACCOUNT_EMAIL;
           }
           
           else if (SETTING_EDIT_TYPE_USERNAME == s_email_setting.edit_type)
           {
               EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->user_name_ptr);
               s_email_setting.load_ptr->user_name_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
               EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->user_name_ptr, ret, "out of memory");
               GUILABEL_SetText(EMAIL_USER_NAME_LABEL_CTRL_ID, &edit_string, FALSE); 
               opt_id = EMASP_PRFITEM_EMAIL_USER_ID;
           }
           else if (SETTING_EDIT_TYPE_INCOMING_SERVER == s_email_setting.edit_type)
           {
               EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr->incoming_server_ptr);
               s_email_setting.recv_serv_ptr->incoming_server_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
               EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->incoming_server_ptr, ret, "out of memory");
               GUILABEL_SetText(EMAIL_INCOMING_SERVER_LABEL_CTRL_ID, &edit_string, FALSE);  
               
               if (s_email_setting.recv_serv_ptr->server_type == EMASP_POP3)
               {
                   opt_id = EMASP_PRFITEM_POP3_SERVER;
               }
               else if (s_email_setting.recv_serv_ptr->server_type == EMASP_IMAP4)
               {
                   opt_id = EMASP_PRFITEM_IMAP4_SERVER;
               }
           }       
           else if (SETTING_EDIT_TYPE_SMTP_SERVER == s_email_setting.edit_type)
           {
               EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->outgoing_server_ptr);
               s_email_setting.send_serv_ptr->outgoing_server_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
               EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->outgoing_server_ptr, ret, "out of memory");
               GUILABEL_SetText(EMAIL_SMTP_SERVER_LABEL_CTRL_ID, &edit_string, FALSE);  
               opt_id = EMASP_PRFITEM_SMTP_SERVER;
           }
           else if (SETTING_EDIT_TYPE_SMTP_USERNAME == s_email_setting.edit_type)
           {
               EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->user_name_ptr);
               s_email_setting.send_serv_ptr->user_name_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
               EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->user_name_ptr, ret, "out of memory");
               GUILABEL_SetText(EMAIL_SMTP_USER_NAME_LABEL_CTRL_ID, &edit_string, FALSE);  
               opt_id = EMASP_PRFITEM_SMTP_USER_ID;
           }
           else if (SETTING_EDIT_TYPE_SMTP_MAILNAME == s_email_setting.edit_type)
           {
               EMA_UTILS_FREEIF(PNULL, s_email_setting.personalized_ptr->acc_name_ptr);
               s_email_setting.personalized_ptr->acc_name_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
               EMA_UTILS_BREAKNULL(s_email_setting.personalized_ptr->acc_name_ptr, ret, "out of memory");
               GUILABEL_SetText(EMAIL_ACC_NAME_LABEL_CTRL_ID, &edit_string, FALSE);  
               opt_id = EMASP_PRFITEM_ACCOUNT_NAME;
           }
           else if (SETTING_EDIT_TYPE_PORT == s_email_setting.edit_type)
           {
               EMASP_PRFITEM_VAL port_type_val = {0};
               GUIWIN_SetTitleTextId(win_id,TXT_COMM_PORT,FALSE);
               if (EMAIL_INCOMING_SETTING_WIN_ID == MMK_GetPrevWinId(win_id))
               {
                   EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr->port_ptr);
                   s_email_setting.recv_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
                   EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->port_ptr, ret, "out of memory");
                   GUILABEL_SetText(EMAIL_INCOMING_PORT_LABEL_CTRL_ID, &edit_string, FALSE);  
                   s_email_setting.recv_serv_ptr->port_type = EMASP_PORT_CUSTOMIZE;
                   port_type_val.pData = (void*)s_email_setting.recv_serv_ptr->port_type;
                   if (s_email_setting.recv_serv_ptr->server_type == EMASP_POP3)
                   {
                       opt_id = EMASP_PRFITEM_POP3_PORT;
                       if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                       {
                           MMIEMAIL_SP_Store_SetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT_DEFAULT,&port_type_val);
                       }
                   }
                   else if (s_email_setting.recv_serv_ptr->server_type == EMASP_IMAP4)
                   {
                       opt_id = EMASP_PRFITEM_IMAP4_SERVER_PORT;
                       if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                       {
                           MMIEMAIL_SP_Store_SetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_IMAP4_PORT_DEFAULT,&port_type_val);
                       }
                   }
               }
               else
               {
                   EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->port_ptr);
                   s_email_setting.send_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, str_temp);
                   EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->port_ptr, ret, "out of memory");
                   GUILABEL_SetText(EMAIL_SMTP_PORT_LABEL_CTRL_ID, &edit_string, FALSE);  
                   opt_id = EMASP_PRFITEM_SMTP_SERVER_PORT;
                   
                   s_email_setting.send_serv_ptr->port_type = EMASP_PORT_CUSTOMIZE;
                   port_type_val.pData = (void*)s_email_setting.send_serv_ptr->port_type;
                   if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                   {
                       MMIEMAIL_SP_Store_SetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_SMTP_PORT_DEFAULT,&port_type_val);
                   }
               }
           }
           
           if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
           {
               val.pData = str_temp;
               MMIEMAIL_SP_Store_SetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle),s_email_setting.act_id,opt_id,&val);
               if (SETTING_EDIT_TYPE_SMTP_MAILNAME == s_email_setting.edit_type)
               {
                   MMK_SendMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                   MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_TITLE, PNULL);
               }
           }
           MMK_CloseWin(win_id);
      }
      
      break;
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
      //todo 返回编辑主界面 
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;   
}

/*****************************************************************************/
// 	Description: handle setting save sent window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveSentWinMsg(
                                                      MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param
                                                      )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                  = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_SAVESENT_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      
      for (i=0; i < SETTING_ITEM_NUM_SAVE_SENT; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_YES + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_SAVESENT_LISTBOX_CTRL_ID, 0, 1);
      }
      
      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      GUILIST_SetSelectedItem(EMAIL_SAVESENT_LISTBOX_CTRL_ID, ~s_email_setting.personalized_ptr->save_sent & 0x00000001,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_SAVESENT_LISTBOX_CTRL_ID, ~s_email_setting.personalized_ptr->save_sent & 0x00000001);	
      
      MMK_SetAtvCtrl(win_id, EMAIL_SAVESENT_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index = GUILIST_GetCurItemIndex(EMAIL_SAVESENT_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_SAVESENT_LISTBOX_CTRL_ID, index, TRUE);
      s_email_setting.personalized_ptr->save_sent = (~ index) & 0x0001;
      
      MMI_GetLabelTextByLang(TXT_EMAIL_YES + index, &item_str);
      GUILABEL_SetText(EMAIL_SAVESENT_LABEL_CTRL_ID, &item_str, FALSE);
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {
         val.pData = (void*)s_email_setting.personalized_ptr->save_sent;
         MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_SAVE_SENT,&val);
      }
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting download number window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDownloadNumberWinMsg(
                                                      MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param
                                                      )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                  = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      
      for (i=0; i < SETTING_ITEM_NUM_DOWNLOAD_NUMBER; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_MAIL_NUM_25 + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID, 0, 1);
      }
      
      GUILIST_SetSelectedItem(EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID,s_email_setting.personalized_ptr->download_num,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID,s_email_setting.personalized_ptr->download_num);	
      
      MMK_SetAtvCtrl(win_id, EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index = GUILIST_GetCurItemIndex(EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_DOWNLOAD_NUMBER_LISTBOX_CTRL_ID, index, TRUE);
      s_email_setting.personalized_ptr->download_num = index;
      
      MMI_GetLabelTextByLang(TXT_EMAIL_MAIL_NUM_25 + index, &item_str);
      GUILABEL_SetText(EMAIL_ONCE_DOWNLOAD_NUMBER_LABEL_CTRL_ID, &item_str, FALSE);
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {  
         val.pData = (void*)s_email_setting.personalized_ptr->download_num;
         MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_NUMBER_LIMIT,&val);
      }
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting receive number limit window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceiveLimitWinMsg(
                                                      MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param
                                                      )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                  = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      
      for (i=0; i < SETTING_ITEM_NUM_RECEIVE_LIMIT; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_ONLYHEADER + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID, 0, 1);
      }
      
      GUILIST_SetSelectedItem(EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID,s_email_setting.personalized_ptr->receive_limit - 1,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID,s_email_setting.personalized_ptr->receive_limit - 1);	
      
      MMK_SetAtvCtrl(win_id, EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index = GUILIST_GetCurItemIndex(EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_RECEIVE_LIMIT_LISTBOX_CTRL_ID, index, TRUE);
      s_email_setting.personalized_ptr->receive_limit = index + 1;
      
      MMI_GetLabelTextByLang(TXT_EMAIL_ONLYHEADER + index, &item_str);
      GUILABEL_SetText(EMAIL_RECEIVE_LIMIT_LABEL_CTRL_ID, &item_str, FALSE);
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {
         val.pData = (void*)s_email_setting.personalized_ptr->receive_limit;
         MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE,&val);
      }
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting receive new window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceiveNewWinMsg(
                                                      MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param
                                                      )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                  = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      
      for (i=0; i < SETTING_ITEM_NUM_RECEIVE_NEW; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_RECV_FROM_NEW + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID, 0, 1);
      }
      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      GUILIST_SetSelectedItem(EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID,~s_email_setting.personalized_ptr->receive_new & 0x00000001,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID,~s_email_setting.personalized_ptr->receive_new & 0x00000001);	
      
      MMK_SetAtvCtrl(win_id, EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index = GUILIST_GetCurItemIndex(EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_RECEIVE_NEW_LISTBOX_CTRL_ID, index, TRUE);
      s_email_setting.personalized_ptr->receive_new = (~ index) & 0x0001;
      
      MMI_GetLabelTextByLang(TXT_EMAIL_RECV_FROM_NEW + index, &item_str);
      GUILABEL_SetText(EMAIL_RECEIVE_ONLY_NEW_MAIL_LABEL_CTRL_ID, &item_str, FALSE);
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {
         val.pData = (void*)s_email_setting.personalized_ptr->receive_new;
         MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL,&val);
      }
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting reply with orig window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReplyMailWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                  = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      
      for (i=0; i < SETTING_ITEM_NUM_REPLY_MAIL; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_INCLUDE_ORIG_BODY + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID, 0, 1);
      }
      
      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      GUILIST_SetSelectedItem(EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID, ~ s_email_setting.personalized_ptr->reply_mail & 0x00000001,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID, ~ s_email_setting.personalized_ptr->reply_mail & 0x00000001);	
      
      MMK_SetAtvCtrl(win_id, EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index = GUILIST_GetCurItemIndex(EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_REPLY_MAIL_LISTBOX_CTRL_ID, index, TRUE);
      s_email_setting.personalized_ptr->reply_mail =(~ index) & 0x0001;
      
      MMI_GetLabelTextByLang(TXT_EMAIL_INCLUDE_ORIG_BODY + index, &item_str);
      GUILABEL_SetText(EMAIL_REPLY_MAIL_LABEL_CTRL_ID, &item_str, FALSE);
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {
         val.pData = (void*)s_email_setting.personalized_ptr->reply_mail;
         MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_INCLUDE_ORIGINAL,&val);
      }
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting authenticate option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAuthenticateWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                  = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      
      for (i=0; i < SETTING_ITEM_NUM_AUTHENTICATE; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_NOTHING + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID, 0, 1);
      }
      
      GUILIST_SetSelectedItem(EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID,s_email_setting.send_serv_ptr->authenticate,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID,s_email_setting.send_serv_ptr->authenticate);	
      
      MMK_SetAtvCtrl(win_id, EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index = GUILIST_GetCurItemIndex(EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_AUTHENTICATE_LISTBOX_CTRL_ID, index, TRUE);
      s_email_setting.send_serv_ptr->authenticate = index;
      
      MMI_GetLabelTextByLang(TXT_EMAIL_NOTHING + index, &item_str);
      GUILABEL_SetText(EMAIL_SMTP_AUTHENTICATION_LABEL_CTRL_ID, &item_str, FALSE);
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {
         val.pData = (void*)s_email_setting.send_serv_ptr->authenticate;
         MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_SMTP_AUTHENTICATE,&val);
      }
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting security option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecurityWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                    = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   EMASP_PRFITEM_VAL      portval = {0}; 
   EMASP_PROFILE_ITEM     opt_id = 0;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_SECURITY_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      
      for (i=0; i < SETTING_ITEM_NUM_SECURITY; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_SSL_TLS + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_SECURITY_LISTBOX_CTRL_ID, 0, 1);
      }

      if (EMAIL_INCOMING_SETTING_WIN_ID == MMK_GetPrevWinId(win_id))
      {
         GUILIST_SetSelectedItem(EMAIL_SECURITY_LISTBOX_CTRL_ID,s_email_setting.recv_serv_ptr->secruity,TRUE);
         GUILIST_SetCurItemIndex(EMAIL_SECURITY_LISTBOX_CTRL_ID,s_email_setting.recv_serv_ptr->secruity);	
      }
      else
      {
         GUILIST_SetSelectedItem(EMAIL_SECURITY_LISTBOX_CTRL_ID,s_email_setting.send_serv_ptr->secruity,TRUE);
         GUILIST_SetCurItemIndex(EMAIL_SECURITY_LISTBOX_CTRL_ID,s_email_setting.send_serv_ptr->secruity);	
      }
      
      MMK_SetAtvCtrl(win_id, EMAIL_SECURITY_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index = GUILIST_GetCurItemIndex(EMAIL_SECURITY_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_SECURITY_LISTBOX_CTRL_ID, index, TRUE);
      MMI_GetLabelTextByLang(TXT_EMAIL_SSL_TLS + index, &item_str);
      if (EMAIL_INCOMING_SETTING_WIN_ID == MMK_GetPrevWinId(win_id))
      {
         s_email_setting.recv_serv_ptr->secruity = index;
         GUILABEL_SetText(EMAIL_INCOMING_SECURITY_LABEL_CTRL_ID, &item_str, FALSE);
         
         val.pData = (void*)s_email_setting.recv_serv_ptr->secruity;
         if (EMASP_POP3 == s_email_setting.recv_serv_ptr->server_type)
         {
            opt_id = EMASP_PRFITEM_POP3_SSL_OR_TLS;            
            if(EMASP_PORT_DEFAULT == s_email_setting.recv_serv_ptr->port_type)
            {
                if (EMASP_SECURITY_SSLTLS == s_email_setting.recv_serv_ptr->secruity
                    || EMASP_SECURITY_STARTTLS == s_email_setting.recv_serv_ptr->secruity)
                {
                    portval.pData = (void*)L"995";
                }
                else
                {
                    portval.pData = (void *)L"110";
                }
                MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_POP3_PORT,&portval);
            }
         }
         else if (EMASP_IMAP4 == s_email_setting.recv_serv_ptr->server_type)
         {
            opt_id = EMASP_PRFITEM_IMAP4_SSL_OR_TLS;
            if(EMASP_PORT_DEFAULT == s_email_setting.recv_serv_ptr->port_type)
            {
                 if (EMASP_SECURITY_SSLTLS == s_email_setting.recv_serv_ptr->secruity
                    || EMASP_SECURITY_STARTTLS == s_email_setting.recv_serv_ptr->secruity)
                {
                    portval.pData = (void*)L"993";
                }
                else
                {
                    portval.pData = (void *)L"143";
                }
                MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_IMAP4_SERVER_PORT,&portval);
            }
         }  
      }
      else
      {
         s_email_setting.send_serv_ptr->secruity = index;
         GUILABEL_SetText(EMAIL_SMTP_SECURITY_LABEL_CTRL_ID, &item_str, FALSE);

//          if (EMASP_SECURITY_OFF == s_email_setting.send_serv_ptr->secruity)
//          {
//              GUIFORM_SetChildDisplay(EMAIL_SMTP_AUTHENTICATION_FORM_CTRL_ID,
//                  EMAIL_SMTP_AUTHENTICATION_TITLE_LABEL_CTRL_ID,
//                  GUIFORM_CHILD_DISP_GRAY);
//          }
//          else
//          {
//              GUIFORM_SetChildDisplay(EMAIL_SMTP_AUTHENTICATION_FORM_CTRL_ID,
//                  EMAIL_SMTP_AUTHENTICATION_TITLE_LABEL_CTRL_ID,
//                  GUIFORM_CHILD_DISP_NORMAL);
//          }

         val.pData = (void*)s_email_setting.send_serv_ptr->secruity;
         opt_id = EMASP_PRFITEM_SMTP_SSL_OR_TLS;
         
         if(EMASP_PORT_DEFAULT == s_email_setting.send_serv_ptr->port_type)
         {
             if (EMASP_SECURITY_SSLTLS == s_email_setting.send_serv_ptr->secruity)
             {
                 portval.pData = (void *)L"465";
             }
            else if(EMASP_SECURITY_STARTTLS == s_email_setting.send_serv_ptr->secruity)
             {
                if (PNULL != s_email_setting.send_serv_ptr->outgoing_server_ptr && MMIEMAIL_IsSameAccount("smtp.163.com", s_email_setting.send_serv_ptr->outgoing_server_ptr))
                {
                    portval.pData = (void *)L"25";
                }
                 else
                {
                    portval.pData = (void *)L"587";
                }
             }
             else
             {
                 portval.pData = (void *)L"25";
             }
             MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_SMTP_SERVER_PORT,&portval);
         }
      }

      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {
         MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, opt_id,&val);
      }
      MMK_CloseWin(win_id);
     
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting server port option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleServPortWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                    = 0;
//   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_SERV_PORT_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。

      MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &item_str);
      AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_SERV_PORT_LISTBOX_CTRL_ID, 0, 1);

      MMI_GetLabelTextByLang(TXT_EXPORT_LOC_CUSTOM, &item_str);
      AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_SERV_PORT_LISTBOX_CTRL_ID, 0, 1);

      if (EMAIL_INCOMING_SETTING_WIN_ID == MMK_GetPrevWinId(win_id))
      {
         GUILIST_SetSelectedItem(EMAIL_SERV_PORT_LISTBOX_CTRL_ID,s_email_setting.recv_serv_ptr->port_type,TRUE);
         GUILIST_SetCurItemIndex(EMAIL_SERV_PORT_LISTBOX_CTRL_ID,s_email_setting.recv_serv_ptr->port_type);	
      }
      else
      {
         GUILIST_SetSelectedItem(EMAIL_SERV_PORT_LISTBOX_CTRL_ID,s_email_setting.send_serv_ptr->port_type,TRUE);
         GUILIST_SetCurItemIndex(EMAIL_SERV_PORT_LISTBOX_CTRL_ID,s_email_setting.send_serv_ptr->port_type);	
      }
      MMK_SetAtvCtrl(win_id, EMAIL_SERV_PORT_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index= GUILIST_GetCurItemIndex(EMAIL_SERV_PORT_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_SERV_PORT_LISTBOX_CTRL_ID, index, TRUE);

      if (0 == index)
      {
         MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &item_str);
      }
      
      if (EMAIL_INCOMING_SETTING_WIN_ID == MMK_GetPrevWinId(win_id))
      {
         if (0 == index)
         {
            GUILABEL_SetText(EMAIL_INCOMING_PORT_LABEL_CTRL_ID, &item_str, FALSE);

            s_email_setting.recv_serv_ptr->port_type = index;  
            val.pData = (void*)s_email_setting.recv_serv_ptr->port_type;

            if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
            {
               if (s_email_setting.recv_serv_ptr->server_type == EMASP_POP3)
               {
                  MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_POP3_PORT_DEFAULT,&val);

                  if (EMASP_SECURITY_SSLTLS == s_email_setting.recv_serv_ptr->secruity
                      || EMASP_SECURITY_STARTTLS == s_email_setting.recv_serv_ptr->secruity)
                  {
                      val.pData = (void*)L"995";
                  }
                  else
                  {
                      val.pData = (void *)L"110";
                  }
                  MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT,&val);
               }
               else if (s_email_setting.recv_serv_ptr->server_type == EMASP_IMAP4)
               {
                  MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_IMAP4_PORT_DEFAULT,&val);
                  if (EMASP_SECURITY_SSLTLS == s_email_setting.recv_serv_ptr->secruity
                      || EMASP_SECURITY_STARTTLS == s_email_setting.recv_serv_ptr->secruity)
                  {
                      val.pData = (void*)L"993";
                  }
                  else
                  {
                      val.pData = (void *)L"143";
                  }
                  MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SERVER_PORT,&val);
               } 
            }
         }
         else
         {
            s_email_setting.edit_type = SETTING_EDIT_TYPE_PORT;
            MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         }
      }
      else
      {
         if (0 == index)
         {
            GUILABEL_SetText(EMAIL_SMTP_PORT_LABEL_CTRL_ID, &item_str, FALSE);
            s_email_setting.send_serv_ptr->port_type = index; 
            val.pData = (void*)s_email_setting.send_serv_ptr->port_type;
            if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
            {
               MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_SMTP_PORT_DEFAULT,&val);
               if (EMASP_SECURITY_SSLTLS == s_email_setting.send_serv_ptr->secruity)
               {
                   val.pData = (void *)L"465";
               }
               else if(EMASP_SECURITY_STARTTLS == s_email_setting.send_serv_ptr->secruity)
               {
                   val.pData = (void *)L"587";
               }

               else
               {
                   val.pData = (void *)L"25";
               }
               MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_SMTP_SERVER_PORT,&val);
            }
         }
         else
         {
            s_email_setting.edit_type = SETTING_EDIT_TYPE_PORT;
            MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         }
      }

      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting server type option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleServTypeWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  index                                    = 0;
   uint32                  i = 0;
   MMI_STRING_T           item_str    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   BOOLEAN                is_change = FALSE;
   int32                  ret = 0;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_SERV_TYPE_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      for (i=0; i < SETTING_ITEM_NUM_SERV_TYPE; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_POP3 + i, &item_str);
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_SERV_TYPE_LISTBOX_CTRL_ID, 0, 1);
      }

      GUILIST_SetSelectedItem(EMAIL_SERV_TYPE_LISTBOX_CTRL_ID,s_email_setting.recv_serv_ptr->server_type - 1,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_SERV_TYPE_LISTBOX_CTRL_ID,s_email_setting.recv_serv_ptr->server_type - 1);	

      MMK_SetAtvCtrl(win_id, EMAIL_SERV_TYPE_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index= GUILIST_GetCurItemIndex(EMAIL_SERV_TYPE_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_SERV_TYPE_LISTBOX_CTRL_ID, index, TRUE);
      if (s_email_setting.recv_serv_ptr->server_type  != index + 1)
      {
         is_change = TRUE;     
      }
      s_email_setting.recv_serv_ptr->server_type  = index + 1;
      if (TRUE == is_change)
      {
          if (SETTING_ENTER_TYPE_EXSIT_ACC == s_email_setting.enter_type)
          {
              val.pData = (void*)s_email_setting.recv_serv_ptr->server_type;
              MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE,&val);
              
              if (EMASP_POP3 == s_email_setting.recv_serv_ptr->server_type)
              {
                  ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_POP3_SERVER,&val);
                  EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
                  
                  s_email_setting.recv_serv_ptr->incoming_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
                  EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->incoming_server_ptr, ret, "out of memory");
                  
                  ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT_DEFAULT,&val);
                  EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
                  
                  s_email_setting.recv_serv_ptr->port_type = (uint32)val.pData;
                  if (s_email_setting.recv_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
                  {
                      ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT,&val);
                      EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
                      
                      s_email_setting.recv_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
                      EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->port_ptr, ret, "out of memory");
                  }
                  
                  ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_POP3_SSL_OR_TLS,&val);
                  EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
                  
                  s_email_setting.recv_serv_ptr->secruity = (uint32)val.pData;
              }
              else
              {
                  ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SERVER,&val);
                  EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_SERVER failed");
                  
                  s_email_setting.recv_serv_ptr->incoming_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
                  EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->incoming_server_ptr, ret, "out of memory");
                  
                  ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_IMAP4_PORT_DEFAULT,&val);
                  EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_PORT_DEFAULT failed");
                  
                  s_email_setting.recv_serv_ptr->port_type = (uint32)val.pData;
                  if (s_email_setting.recv_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
                  {
                      ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SERVER_PORT,&val);
                      EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_SERVER failed");
                      
                      s_email_setting.recv_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
                      EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->port_ptr, ret, "out of memory");
                  }
                  
                  ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SSL_OR_TLS,&val);
                  EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
                  
                  s_email_setting.recv_serv_ptr->secruity = (uint32)val.pData;
              }

              item_str.wstr_ptr = s_email_setting.recv_serv_ptr->incoming_server_ptr;
              item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr);
              GUILABEL_SetText(EMAIL_INCOMING_SERVER_LABEL_CTRL_ID, &item_str,FALSE); 

              
              if (EMASP_PORT_DEFAULT == s_email_setting.recv_serv_ptr->port_type)
              {
                  MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &item_str);
              }
              else
              {
                  item_str.wstr_ptr = s_email_setting.recv_serv_ptr->port_ptr;
                  item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->port_ptr);
              }
              GUILABEL_SetText(EMAIL_INCOMING_PORT_LABEL_CTRL_ID, &item_str, FALSE); 
              
              MMI_GetLabelTextByLang(TXT_EMAIL_SSL_TLS + s_email_setting.recv_serv_ptr->secruity, &item_str);
              GUILABEL_SetText(EMAIL_INCOMING_SECURITY_LABEL_CTRL_ID, &item_str, FALSE);
          }
          
          MMI_GetLabelTextByLang(TXT_EMAIL_POP3 + (s_email_setting.recv_serv_ptr->server_type - 1) , &item_str);
          GUILABEL_SetText(EMAIL_INCOMING_SERVER_TYPE_LABEL_CTRL_ID, &item_str, FALSE);
      }
      
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle setting network path option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetworkPathWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16                  index = 0;
   MMI_STRING_T           item_str    ={0};
   MMI_STRING_T           item_str_temp1    ={0};
   MMI_STRING_T           item_str_temp2    ={0};
   EMASP_PRFITEM_VAL      val = {0}; 
   uint i = 0;
   int32 ret = 0;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      for (i= MN_DUAL_SYS_1;  i < MMI_DUAL_SYS_MAX; i++)
      {
         MMI_GetLabelTextByLang(TXT_EMAIL_SIM1 + i, &item_str_temp1);
         if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1 + i))
         {
             wchar               opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};

             EMA_UTILS_MEMSET(&item_str_temp2, 0x00, sizeof(item_str_temp2));
             item_str_temp2.wstr_ptr = (wchar *)opn_spn_name;//lint !e733
             EMA_UTILS_MEMSET(item_str_temp2.wstr_ptr, 0x00, sizeof(opn_spn_name));
             MMIAPIPHONE_GetSimAndServiceString(MN_DUAL_SYS_1 + i, &item_str_temp2);
         }
         else
         {
            MMI_GetLabelTextByLang(TXT_EMAIL_NO_SIM_CARD, &item_str_temp2);
         }
         
         item_str.wstr_len = item_str_temp1.wstr_len + item_str_temp2.wstr_len + 3;
         item_str.wstr_ptr = EMA_UTILS_MALLOC(PNULL, (item_str.wstr_len + 1) * sizeof(wchar));
         EMA_UTILS_BREAKNULL(item_str.wstr_ptr, ret, "out of memory");
         EMA_UTILS_MEMSET(item_str.wstr_ptr, 0x00, (item_str.wstr_len + 1) * sizeof(wchar));
         EMA_UTILS_MEMCPY(item_str.wstr_ptr, item_str_temp1.wstr_ptr, item_str_temp1.wstr_len * sizeof(wchar));
         EMA_UTILS_WSTRCAT(item_str.wstr_ptr, (wchar*)L" (");
         EMA_UTILS_MEMCPY(item_str.wstr_ptr + item_str_temp1.wstr_len + 2, item_str_temp2.wstr_ptr, item_str_temp2.wstr_len * sizeof(wchar));
         EMA_UTILS_WSTRCAT(item_str.wstr_ptr, (wchar*)L")");
         AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID, 0, 1);
         EMA_UTILS_FREEIF(PNULL, item_str.wstr_ptr);
      }
#ifdef  MMI_WIFI_SUPPORT     
      MMI_GetLabelTextByLang(TXT_WIFI, &item_str);
	  AppendListItemEx(item_str.wstr_ptr, item_str.wstr_len, EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID, 0, 1);
#endif

      GUILIST_SetSelectedItem(EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID,s_email_setting.data_conn_ptr->net_path,TRUE);
      GUILIST_SetCurItemIndex(EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID,s_email_setting.data_conn_ptr->net_path);	
      
      for (i= MN_DUAL_SYS_1;  i < MMI_DUAL_SYS_MAX; i++)
      {
         if (!MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1 + i))
         {
            GUILIST_SetItemGray(EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID, i, TRUE);
         }
      }
      
      MMK_SetAtvCtrl(win_id, EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      index= GUILIST_GetCurItemIndex(EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_NETWORK_PATH_LISTBOX_CTRL_ID, index, TRUE);

      if (index < MMI_DUAL_SYS_MAX)
      {
         s_email_setting.data_conn_ptr->net_path  = index;
         MMI_GetLabelTextByLang(TXT_EMAIL_SIM1 + s_email_setting.data_conn_ptr->net_path, &item_str_temp1);
         if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1 + index))
         {
             wchar               opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
             
             EMA_UTILS_MEMSET(&item_str_temp2, 0x00, sizeof(item_str_temp2));
             item_str_temp2.wstr_ptr = (wchar *)opn_spn_name;//lint !e733
             EMA_UTILS_MEMSET(item_str_temp2.wstr_ptr, 0x00, sizeof(opn_spn_name));
             MMIAPIPHONE_GetSimAndServiceString(s_email_setting.data_conn_ptr->net_path, &item_str_temp2);
         }
         else
         {
            MMI_GetLabelTextByLang(TXT_EMAIL_NO_SIM_CARD, &item_str_temp2);
         }
         
         item_str.wstr_len = item_str_temp1.wstr_len + item_str_temp2.wstr_len + 3;
         item_str.wstr_ptr = EMA_UTILS_MALLOC(PNULL, (item_str.wstr_len + 1) * sizeof(wchar));
         EMA_UTILS_BREAKNULL(item_str.wstr_ptr, ret, "out of memory");
         EMA_UTILS_MEMSET(item_str.wstr_ptr, 0x00, (item_str.wstr_len + 1) * sizeof(wchar));
         EMA_UTILS_MEMCPY(item_str.wstr_ptr, item_str_temp1.wstr_ptr, item_str_temp1.wstr_len * sizeof(wchar));
         EMA_UTILS_WSTRCAT(item_str.wstr_ptr, (wchar*)L" (");
         EMA_UTILS_MEMCPY(item_str.wstr_ptr + item_str_temp1.wstr_len + 2, item_str_temp2.wstr_ptr, item_str_temp2.wstr_len * sizeof(wchar));
         EMA_UTILS_WSTRCAT(item_str.wstr_ptr, (wchar*)L")");
         GUILABEL_SetText(EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID, &item_str, FALSE);
         EMA_UTILS_FREEIF(PNULL, item_str.wstr_ptr);

         if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
         {
            val.pData = (void*)s_email_setting.data_conn_ptr->net_path;
            MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_NET_PATH,&val);
         }
      }
      else
      {
#ifdef MMI_WIFI_SUPPORT
         if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
         {
            s_email_setting.data_conn_ptr->net_path  = index;
            MMI_GetLabelTextByLang(TXT_WIFI, &item_str);
            GUILABEL_SetText(EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID, &item_str, FALSE);

            if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
            {
               val.pData = (void *)s_email_setting.data_conn_ptr->net_path;
               MMIEMAIL_SP_Store_SetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),s_email_setting.act_id, EMASP_PRFITEM_NET_PATH,&val);
            }
         }
         else
         {
             MMIEMAIL_RegWifiNotify();
             MMIAPIWIFI_OpenListWin();
             break;
         }
#endif
      }
      
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
// 	Description: handle email account link list option window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEmailAccountLinkListWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
   MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
   uint16  link_index                                    = 0;
   MN_DUAL_SYS_E  dual_sim                              = MMI_DUAL_SYS_MAX;
   uint8 index                                          = 0;
   MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
   MMI_STRING_T  set_string  = {0};
   
   dual_sim = ((uint32)MMK_GetWinAddDataPtr(win_id))&0xFFFF;/*lint !e64*/
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      GUILIST_SetMaxItem(EMAIL_LINKLIST_LISTBOX_CTRL_ID, MMIEMAIL_MAX_SETTING_NUM, FALSE );
      //根据应用画出所有连接配置,连接配置在“连接管理”通过函数得到，默认配置需要mms自己记录。
      for(index = 0; index < MMIAPICONNECTION_GetLinkSettingNum(dual_sim); index++)                              //显示LIST
      {
         link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sim, index);
         if(!link_setting_ptr->is_customed_detail)
         {
            /*
            MMIMMS_AppendOneLineTextListItem(EMAIL_LINKLIST_LISTBOX_CTRL_ID,
               GUIITEM_STYLE_ONE_LINE_RADIO,
               STXT_OK,  0,  link_setting_ptr->name, 
               link_setting_ptr->name_len, (uint16)index, FALSE);
               */
            AppendListItemEx(link_setting_ptr->name, link_setting_ptr->name_len, EMAIL_LINKLIST_LISTBOX_CTRL_ID, 0, 1);
         }
         else
         {
            /*
            MMIMMS_AppendOneLineTextListItem(EMAIL_LINKLIST_LISTBOX_CTRL_ID,
               GUIITEM_STYLE_ONE_LINE_RADIO,
               STXT_OK,  0,  link_setting_ptr->name, 
               link_setting_ptr->name_len, (uint16)index, FALSE);
               */
            AppendListItemEx(link_setting_ptr->name, link_setting_ptr->name_len, EMAIL_LINKLIST_LISTBOX_CTRL_ID, 0, 1);
         }
      } 

      if(dual_sim != MN_DUAL_SYS_1)
      {
         GUIWIN_SetTitleTextId(win_id, TXT_EMAIL_NETWORK_ACCOUNT_SIM1 + (MMI_TEXT_ID_T)dual_sim, FALSE);
      }

      GUILIST_SetSelectedItem(EMAIL_LINKLIST_LISTBOX_CTRL_ID,s_email_setting.data_conn_ptr->net_sim[dual_sim],TRUE);
      GUILIST_SetCurItemIndex(EMAIL_LINKLIST_LISTBOX_CTRL_ID,s_email_setting.data_conn_ptr->net_sim[dual_sim]);	

      MMK_SetAtvCtrl(win_id, EMAIL_LINKLIST_LISTBOX_CTRL_ID);
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      //todo  根据选择的配置，设置默认email连接参数，保存全局的连接index
      link_index = GUILIST_GetCurItemIndex(EMAIL_LINKLIST_LISTBOX_CTRL_ID);
      GUILIST_SetSelectedItem(EMAIL_LINKLIST_LISTBOX_CTRL_ID, link_index, TRUE);
      link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sim, link_index);
      set_string.wstr_ptr = link_setting_ptr->name;
      set_string.wstr_len = link_setting_ptr->name_len;
      s_email_setting.data_conn_ptr->net_sim[dual_sim] = link_index;
      GUILABEL_SetText(EMAIL_NETWORK_ACCOUNT_SIM1_LABEL_CTRL_ID + (MMI_CTRL_ID_T)dual_sim, &set_string, FALSE);
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
      {
         SetNetSettingIndex(dual_sim, s_email_setting.act_id, link_index);
      }
      MMK_CloseWin(win_id);
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   }    
   
   return err_code;
}

/*****************************************************************************/
//  Description :send setting waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingWaitWinMsg(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E err_code                    = MMI_RESULT_TRUE;
//    uint16 temp_size                         = 0;    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_NULL, FALSE);
        break;
    case MSG_FULL_PAINT:
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);        
        break;
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;
    default :
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
// 	Description: handle restore factory query window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreFactoryQueryWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                )
{
   MMI_RESULT_E        result = MMI_RESULT_TRUE;
//   uint32              index = 0;
//   int                 i = 0;
   //MMIATV_SERVICE_NODE_T *cur_node_ptr = PNULL;
   
   switch(msg_id)
   {
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      {
         if (MMIEMAIL_VectorGetVectorNumber(s_email_setting.account_ptr->vector_ptr) > 0)
         {
            MMI_STRING_T  string = {0};

            MMI_GetLabelTextByLang(TXT_EMAIL_RESTORING, &string);
            MMIPUB_OpenWaitWin(1,&string,PNULL,PNULL,EMAIL_FACTORY_RESET_WIN_ID,IMAGE_NULL,
               ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)HandleSettingWaitWinMsg);

            MMIEMAIL_SP_Store_FactoryReset(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle));
         }
         else
         {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_EMAIL_RESTORE_COMPLETE);
#endif
         }
      }
      MMK_CloseWin(win_id);
      break;
      
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
      MMK_CloseWin(win_id);
      break;
      
   default:
      result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
      break;
   }
   
   return result;
}


/*****************************************************************************/
// 	Description: handle email wifi start query window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
#ifdef MMI_WIFI_SUPPORT
LOCAL MMI_RESULT_E HandleWifiStartQueryWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                )
{
   MMI_RESULT_E        result = MMI_RESULT_TRUE;
   //MMIATV_SERVICE_NODE_T *cur_node_ptr = PNULL;
   
   switch(msg_id)
   {
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
   case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
      {
         MMIEMAIL_RegWifiNotify();
         MMIAPIWIFI_OpenListWin();
         MMK_CloseWin(win_id);
      }
      break;
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
      //MMK_CloseWin(EMAIL_NETWORK_PATH_WIN_ID);
      MMK_CloseWin(win_id);
      break;
      
   default:
      result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
      break;
   }
   
   return result;
}
#endif

/*****************************************************************************/
// 	Description: handle system setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSystemSettingWinMsg(
                                                         MMI_WIN_ID_T win_id, 
                                                         MMI_MESSAGE_ID_E msg_id, 
                                                         DPARAM param
                                                         )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
   GUI_BG_T label_bg = {0};
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
//       GUIFORM_SetStyle(EMAIL_STARTUP_SCREEN_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_MAIL_STORAGE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_TONE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_TONE_VOLUME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_SLIENT_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_ACCOUNT_PASSWORD_PROTECTION_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_SET_PASSWORD_PROTECTION_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_STORAGE_STATISTICS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_FLOW_STATISTICS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
//       GUIFORM_SetStyle(EMAIL_SOFTWARE_VERSION_CHECK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_RESTORE_FACTORY_SETTINGS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      
      //从g_nv_setting_mirror读取自动下载和接收广告的配置通过LIST控件显示        
      //GUILABEL_SetTextById(EMAIL_STARTUP_SCREEN_TITLE_LABEL_CTRL_ID, TXT_EMAIL_STARTUP_SCREEN,FALSE);
//       GUILABEL_SetTextById(EMAIL_MAIL_STORAGE_TITLE_LABEL_CTRL_ID, TXT_EMAIL_MAIL_STORAGE,FALSE);
//       GUILABEL_SetTextById(EMAIL_TONE_TITLE_LABEL_CTRL_ID, TXT_TONE,FALSE);
//       GUILABEL_SetTextById(EMAIL_TONE_VOLUME_TITLE_LABEL_CTRL_ID, TXT_EMAIL_TONE_VOLUME,FALSE);
//       GUILABEL_SetTextById(EMAIL_SLIENT_TIME_TITLE_LABEL_CTRL_ID, TXT_COMMON_SLIENT_TIME,FALSE);
//       GUILABEL_SetTextById(EMAIL_ACCOUNT_PASSWORD_PROTECTION_TITLE_LABEL_CTRL_ID, TXT_EMAIL_ACCOUNT_PASSWORD_PROTECTION,FALSE);
//       GUILABEL_SetTextById(EMAIL_SET_PASSWORD_PROTECTION_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SET_PASSWORD_PROTECTION,FALSE);
//       GUILABEL_SetTextById(EMAIL_STORAGE_STATISTICS_TITLE_LABEL_CTRL_ID, TXT_EMAIL_STORAGE_STATISTICS,FALSE);
//       GUILABEL_SetTextById(EMAIL_FLOW_STATISTICS_TITLE_LABEL_CTRL_ID, TXT_COMMON_FLOW_STATISTICS,FALSE);
//       GUILABEL_SetTextById(EMAIL_SOFTWARE_VERSION_CHECK_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SOFTWARE_VERSION_CHECK,FALSE);
        GUILABEL_SetTextById(EMAIL_RESTORE_FACTORY_SETTINGS_TITLE_LABEL_CTRL_ID, TXT_COMMON_RESTORE_FACTORY_SETTINGS,FALSE);
        label_bg.bg_type = GUI_BG_COLOR;
        label_bg.color = MMI_YELLOW_COLOR;
        label_bg.shape = GUI_SHAPE_RECT;
        GUIFORM_SetBg(EMAIL_SYSTEM_SETTING_CTRL_ID,&label_bg);
      //SetRecvDropDownList();
      MMK_SetAtvCtrl(win_id, EMAIL_SYSTEM_SETTING_CTRL_ID);
      break;
      
   case MSG_CTL_OK:
   case MSG_APP_OK:
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      {
         MMI_WIN_ID_T query_win_id = EMAIL_RESTORE_FACTORY_QUERY_WIN_ID;
         MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_RESTORE_FACTORY_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, HandleRestoreFactoryQueryWinMsg);
      }
      break;

   case EVT_EMA_FACTORY_RESET:
      MMK_CloseWin(EMAIL_FACTORY_RESET_WIN_ID);
      MMK_CloseWin(EMAIL_SYSTEM_SETTING_WIN_ID);
      MMK_CloseWin(EMAIL_MAIN_MENU_WIN_ID);
#ifdef MMI_GUI_STYLE_TYPICAL
      MMIPUB_OpenAlertSuccessWin(TXT_EMAIL_RESTORE_COMPLETE);
#endif
      MMK_SendMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
      break;
      
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}

#ifdef MMI_GUI_STYLE_TYPICAL

/*****************************************************************************/
// 	Description: initialize personalized setting data
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetPersonalizedSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
   MMI_STRING_T           item_str    ={0};
//   uint32                 i= 0;
   int32                  ret = SUCCESS;
   
   do 
   {
      s_email_setting.personalized_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(PERSONALIZED_SETTING_T));
      EMA_UTILS_BREAKNULL(s_email_setting.personalized_ptr, ret, "out of memory");
      EMA_UTILS_MEMSET(s_email_setting.personalized_ptr, 0x00, sizeof(PERSONALIZED_SETTING_T));
      
      if (!is_new_acc)
      {
         store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
         EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "store_mgr_ptr is NULL");
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_ACCOUNT_NAME,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.personalized_ptr->acc_name_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.personalized_ptr->acc_name_ptr, ret, "out of memory");
         
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SAVE_SENT,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.personalized_ptr->save_sent = (uint32)val.pData;
         
//          ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_RECEIVE_FREQUENCY,&val);
//          EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
//          
//          s_email_setting.personalized_ptr->receive_frequency = (uint32)val.pData;

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_NUMBER_LIMIT,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.personalized_ptr->download_num = (uint32)val.pData;

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.personalized_ptr->receive_limit = (uint32)val.pData;


         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.personalized_ptr->receive_new = (uint32)val.pData;

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_INCLUDE_ORIGINAL,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_INCLUDE_ORIGINAL failed");
         
         s_email_setting.personalized_ptr->reply_mail = (uint32)val.pData;
      }
      else
      {
         s_email_setting.personalized_ptr->receive_limit = EMA_DOWNLOAD_ONLY_HEADERS;
         s_email_setting.personalized_ptr->receive_new = TRUE;
         s_email_setting.personalized_ptr->reply_mail = TRUE;
      }
      
      GUIFORM_SetStyle(EMAIL_ACC_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      //GUIFORM_SetStyle(EMAIL_SEND_MAIL_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      //GUIFORM_SetStyle(EMAIL_SENDER_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_SAVESENT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      //GUIFORM_SetStyle(EMAIL_RECEIVE_FREQUENCY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_ONCE_DOWNLOAD_NUMBER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_RECEIVE_LIMIT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_RECEIVE_ONLY_NEW_MAIL_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_REPLY_MAIL_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      //GUIFORM_SetStyle(EMAIL_MAIL_AUTO_DELETE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      //GUIFORM_SetStyle(EMAIL_MAIL_DELETE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      //GUIFORM_SetStyle(EMAIL_SIGNATURE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      //GUIFORM_SetStyle(EMAIL_NEW_MAIL_TIP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      
      //从g_nv_setting_mirror读取自动下载和接收广告的配置通过LIST控件显示        
      GUILABEL_SetTextById(EMAIL_ACC_NAME_TITLE_LABEL_CTRL_ID, TXT_EMAIL_ACCOUNT_NAME,FALSE);
      //GUILABEL_SetTextById(EMAIL_SEND_MAIL_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SENDEMAIL,FALSE);
      //GUILABEL_SetTextById(EMAIL_SENDER_NAME_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SENDER_NAME,FALSE);
      GUILABEL_SetTextById(EMAIL_SAVESENT_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SAVESENT,FALSE);
      //GUILABEL_SetTextById(EMAIL_RECEIVE_FREQUENCY_TITLE_LABEL_CTRL_ID, TXT_EMAIL_RECEIVE_FREQUENCY,FALSE);
      GUILABEL_SetTextById(EMAIL_ONCE_DOWNLOAD_NUMBER_TITLE_LABEL_CTRL_ID, TXT_EMAIL_ONCE_DOWNLOAD_NUMBER,FALSE);
      GUILABEL_SetTextById(EMAIL_RECEIVE_LIMIT_TITLE_LABEL_CTRL_ID, TXT_EMAIL_RECEIVE_LIMIT,FALSE);
      GUILABEL_SetTextById(EMAIL_RECEIVE_ONLY_NEW_MAIL_TITLE_LABEL_CTRL_ID, TXT_EMAIL_RECEIVE_ONLY_NEW_MAIL,FALSE);
      GUILABEL_SetTextById(EMAIL_REPLY_MAIL_TITLE_LABEL_CTRL_ID, TXT_EMAIL_REPLY_MAIL,FALSE);
      //       GUILABEL_SetTextById(EMAIL_MAIL_AUTO_DELETE_TITLE_LABEL_CTRL_ID, TXT_EMAIL_MAIL_AUTO_DELETE,FALSE);
      //       GUILABEL_SetTextById(EMAIL_MAIL_DELETE_TITLE_LABEL_CTRL_ID, TXT_EMAIL_MAIL_DELETE,FALSE);
      //       GUILABEL_SetTextById(EMAIL_SIGNATURE_TITLE_LABEL_CTRL_ID, TXT_COMMON_SIGNATURE,FALSE);
//       GUILABEL_SetTextById(EMAIL_NEW_MAIL_TIP_TITLE_LABEL_CTRL_ID, TXT_EMAIL_NEW_MAIL_TIP,FALSE);
      
      if (EMA_UTILS_WSTRLEN(s_email_setting.personalized_ptr->acc_name_ptr) > 0)
      {
         item_str.wstr_ptr = s_email_setting.personalized_ptr->acc_name_ptr;
         item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.personalized_ptr->acc_name_ptr);
         GUILABEL_SetText(EMAIL_ACC_NAME_LABEL_CTRL_ID, &item_str, FALSE); 
      }
      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      MMI_GetLabelTextByLang(TXT_EMAIL_YES + (~s_email_setting.personalized_ptr->save_sent & 0x00000001), &item_str);
      GUILABEL_SetText(EMAIL_SAVESENT_LABEL_CTRL_ID, &item_str, FALSE);

      MMI_GetLabelTextByLang(TXT_EMAIL_MAIL_NUM_25 + s_email_setting.personalized_ptr->download_num, &item_str);
      GUILABEL_SetText(EMAIL_ONCE_DOWNLOAD_NUMBER_LABEL_CTRL_ID, &item_str, FALSE);

      MMI_GetLabelTextByLang(TXT_EMAIL_ONLYHEADER + (s_email_setting.personalized_ptr->receive_limit - 1), &item_str);
      GUILABEL_SetText(EMAIL_RECEIVE_LIMIT_LABEL_CTRL_ID, &item_str, FALSE);

      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      MMI_GetLabelTextByLang(TXT_EMAIL_RECV_FROM_NEW + (~s_email_setting.personalized_ptr->receive_new & 0x00000001), &item_str);
      GUILABEL_SetText(EMAIL_RECEIVE_ONLY_NEW_MAIL_LABEL_CTRL_ID, &item_str, FALSE);

      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      MMI_GetLabelTextByLang(TXT_EMAIL_INCLUDE_ORIG_BODY + (~s_email_setting.personalized_ptr->reply_mail & 0x00000001), &item_str);
      GUILABEL_SetText(EMAIL_REPLY_MAIL_LABEL_CTRL_ID, &item_str, FALSE);
   } while (0);
}

/*****************************************************************************/
// 	Description: handle email personalized window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePersonalizedSettingWinMsg(
                                                         MMI_WIN_ID_T win_id, 
                                                         MMI_MESSAGE_ID_E msg_id, 
                                                         DPARAM param
                                                         )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
   MMI_CTRL_ID_T  ctrl_id = EMAIL_ACC_NAME_LABEL_CTRL_ID;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      SetPersonalizedSettingParam(FALSE);
      MMK_SetAtvCtrl(win_id, EMAIL_PERSONALIZED_SETTING_CTRL_ID);
      break;
      
   case MSG_CTL_OK:
   case MSG_APP_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif

      ctrl_id = MMK_GetActiveCtrlId(win_id);
      switch(ctrl_id)
      {
      case EMAIL_ACC_NAME_LABEL_CTRL_ID:
         s_email_setting.edit_type = SETTING_EDIT_TYPE_SMTP_MAILNAME;
         MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         break;
      case EMAIL_SAVESENT_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32 *)MMIEMAIL_SAVE_SENT_WIN_TAB,PNULL);
         break;      
      case EMAIL_ONCE_DOWNLOAD_NUMBER_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32 *)MMIEMAIL_DOWNLOAD_NUMBER_WIN_TAB,PNULL);
         break; 
      case EMAIL_RECEIVE_LIMIT_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32*)MMIEMAIL_RECEIVE_LIMIT_WIN_TAB, PNULL);
         break;
      case EMAIL_RECEIVE_ONLY_NEW_MAIL_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32*)MMIEMAIL_RECEIVE_NEW_WIN_TAB, PNULL);
         break;
      case EMAIL_REPLY_MAIL_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32*)MMIEMAIL_REPLY_MAIL_WIN_TAB, PNULL);
         break;
      default:
         break;
      }
      break;    
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      if (NULL != s_email_setting.personalized_ptr)
      {
          EMA_UTILS_FREEIF(PNULL, s_email_setting.personalized_ptr->acc_name_ptr);
          EMA_UTILS_FREE(PNULL, s_email_setting.personalized_ptr);
      }
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}


/*****************************************************************************/
// 	Description: initialize outgoing setting data
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetOutgoingSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
   MMI_STRING_T           item_str    ={0};
//   uint32                 i= 0;
   int32                  ret = SUCCESS;

   do 
   {
      s_email_setting.send_serv_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(OUTGOING_SETTING_T));
      EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr, ret, "out of memory");
      EMA_UTILS_MEMSET(s_email_setting.send_serv_ptr, 0x00, sizeof(OUTGOING_SETTING_T));

      if (!is_new_acc)
      {
         store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
         EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_SERVER,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->outgoing_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->outgoing_server_ptr, ret, "out of memory");

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_USER_ID,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->user_name_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->user_name_ptr, ret, "out of memory");

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_PASSWORD,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->password_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->password_ptr, ret, "out of memory");
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_PORT_DEFAULT,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.send_serv_ptr->port_type = (uint32)val.pData;
         if (s_email_setting.send_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
         {
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_SERVER_PORT,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.send_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
            EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->port_ptr, ret, "out of memory");
         }
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_SSL_OR_TLS,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->secruity = (uint32)val.pData;
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_AUTHENTICATE,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_RECEIVE_SERVER_TYPE failed");
         
         s_email_setting.send_serv_ptr->authenticate = (uint32)val.pData;    
        
      }
      
      GUIFORM_SetStyle(EMAIL_SMTP_SERVER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_SMTP_USER_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_SMTP_PASSWORD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_SMTP_PORT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_SMTP_SECURITY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_SMTP_AUTHENTICATION_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      
      //从g_nv_setting_mirror读取自动下载和接收广告的配置通过LIST控件显示        
      GUILABEL_SetTextById(EMAIL_SMTP_SERVER_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SMTPSERVERNAME,FALSE);
      GUILABEL_SetTextById(EMAIL_SMTP_USER_NAME_TITLE_LABEL_CTRL_ID, TXT_COMMON_USERNAME,FALSE);
      GUILABEL_SetTextById(EMAIL_SMTP_PASSWORD_TITLE_LABEL_CTRL_ID, TXT_COMMON_PASSWORD,FALSE);
      GUILABEL_SetTextById(EMAIL_SMTP_PORT_TITLE_LABEL_CTRL_ID, TXT_COMM_PORT,FALSE);
      GUILABEL_SetTextById(EMAIL_SMTP_SECURITY_TITLE_LABEL_CTRL_ID, TXT_COMM_SECURITY,FALSE);
      GUILABEL_SetTextById(EMAIL_SMTP_AUTHENTICATION_TITLE_LABEL_CTRL_ID, TXT_EMAIL_AUTHENTICATION,FALSE);

      if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
      {
         item_str.wstr_ptr = s_email_setting.send_serv_ptr->outgoing_server_ptr;
         item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr);
         GUILABEL_SetText(EMAIL_SMTP_SERVER_LABEL_CTRL_ID, &item_str, FALSE); 
      }

      if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->user_name_ptr) > 0)
      {
         item_str.wstr_ptr = s_email_setting.send_serv_ptr->user_name_ptr;
         item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->user_name_ptr);
         GUILABEL_SetText(EMAIL_SMTP_USER_NAME_LABEL_CTRL_ID, &item_str, FALSE); 
      }

      if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->password_ptr) > 0)
      {
         item_str.wstr_ptr = SETTING_PASSWORD_SHOW_STRING;
         item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->password_ptr);
         
         GUILABEL_SetText(EMAIL_SMTP_PASSWORD_LABEL_CTRL_ID, &item_str, FALSE);
      }

      if (EMASP_PORT_DEFAULT == s_email_setting.send_serv_ptr->port_type)
      {
         MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &item_str);
         GUILABEL_SetText(EMAIL_SMTP_PORT_LABEL_CTRL_ID, &item_str, FALSE);
      }
      else if(EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->port_ptr) > 0)
      {
         item_str.wstr_ptr = s_email_setting.send_serv_ptr->port_ptr;
         item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->port_ptr);
         GUILABEL_SetText(EMAIL_SMTP_PORT_LABEL_CTRL_ID, &item_str, FALSE); 
      }
      
      MMI_GetLabelTextByLang(TXT_EMAIL_SSL_TLS + s_email_setting.send_serv_ptr->secruity, &item_str);
      GUILABEL_SetText(EMAIL_SMTP_SECURITY_LABEL_CTRL_ID, &item_str, FALSE);

      MMI_GetLabelTextByLang(TXT_EMAIL_NOTHING + s_email_setting.send_serv_ptr->authenticate, &item_str);
      GUILABEL_SetText(EMAIL_SMTP_AUTHENTICATION_LABEL_CTRL_ID, &item_str, FALSE);
//       if (EMASP_SECURITY_OFF == s_email_setting.send_serv_ptr->secruity)
//       {
//           GUIFORM_SetChildDisplay(EMAIL_SMTP_AUTHENTICATION_FORM_CTRL_ID,
//               EMAIL_SMTP_AUTHENTICATION_TITLE_LABEL_CTRL_ID,
//               GUIFORM_CHILD_DISP_GRAY);
//       }
   } while (0);
}


/*****************************************************************************/
// 	Description: handle email outgoing setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutgoingSettingWinMsg(
                                                         MMI_WIN_ID_T win_id, 
                                                         MMI_MESSAGE_ID_E msg_id, 
                                                         DPARAM param
                                                         )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
   MMI_CTRL_ID_T  ctrl_id = EMAIL_SMTP_SERVER_LABEL_CTRL_ID;
   int32          result = 0;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      //SetRecvDropDownList();
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         SetOutgoingSettingParam(TRUE);
#if defined(MMI_PDA_SUPPORT) 
         if (0 == EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr))
         {
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
         }
#else
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_FINISH, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
#endif
      }
      else
      {
#if defined(MMI_PDA_SUPPORT) 
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif
         SetOutgoingSettingParam(FALSE);
      }
#if defined(MMI_PDA_SUPPORT) 
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
      MMK_SetAtvCtrl(win_id, EMAIL_OUTGOING_SETTING_CTRL_ID);
      break;
   case MSG_GET_FOCUS:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, STXT_FINISH, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
      }
      break;
   case MSG_CTL_OK:
   case MSG_APP_OK:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
         {            
            result = CreateEmailAccount(s_email_setting.load_ptr->mail_address_ptr);  
            if (EME_NOSTORAGE == result)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
                break;
            }
            else if (EMAIL_SUCCESS != result)
            {
                MMIPUB_OpenAlertFailWin(TXT_EMAIL_CREATE_NEW_ACC_FAILED);   
                break;
            }

            MMK_CloseWin(EMAIL_SELECT_EMAIL_ACC_WIN_ID);
            MMK_CloseWin(EMAIL_LOAD_SETTING_WIN_ID);
            MMK_CloseWin(EMAIL_DATA_CONNECTION_WIN_ID);
            MMK_CloseWin(EMAIL_INCOMING_SETTING_WIN_ID);
            if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID) && !MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID)
                && !MMK_IsOpenWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID))
            {
                int i= 0;
                MMK_CloseWin(win_id);
                result = MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)g_email_opt_ptr);
                MMIEMAIL_FreeOptPtr();
            }
            else
            {
                MMK_CloseWin(win_id);
                if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
                {
                    MMK_CreateWin((uint32*)MMIEMAIL_SETTING_WIN_TAB, PNULL);   
                }
                else
                {
                    MMK_SendMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                }       
                MMK_CloseWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID);
            }
         }
         break;
      }
#if defined(MMI_PDA_SUPPORT) 
        break;
#endif

   case MSG_APP_WEB://lint !e616 !e825
   case MSG_CTL_MIDSK://lint !e616 !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
      ctrl_id = MMK_GetActiveCtrlId(win_id);
      switch(ctrl_id)
      {
      case EMAIL_SMTP_SERVER_LABEL_CTRL_ID:
         s_email_setting.edit_type = SETTING_EDIT_TYPE_SMTP_SERVER;
         MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         break;
      case EMAIL_SMTP_USER_NAME_LABEL_CTRL_ID:
         s_email_setting.edit_type = SETTING_EDIT_TYPE_SMTP_USERNAME;
         MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         break;      
      case EMAIL_SMTP_PASSWORD_LABEL_CTRL_ID:
         s_email_setting.password_type = SETTING_PASSWORD_TYPE_SMTP_PASSWORD;
         MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_PASSWORD_WIN_TAB,PNULL);
         break; 
      case EMAIL_SMTP_PORT_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32*)MMIEMAIL_SERVER_PORT_WIN_TAB, PNULL);
         break;
      case EMAIL_SMTP_SECURITY_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32*)MMIEMAIL_SECURITY_WIN_TAB, PNULL);
         break;
      case EMAIL_SMTP_AUTHENTICATION_LABEL_CTRL_ID:
         if (EMASP_SECURITY_OFF != s_email_setting.send_serv_ptr->secruity)
         {
             MMK_CreatePubListWin((uint32*)MMIEMAIL_AUTHENTICATE_WIN_TAB, PNULL);
         }
         break;
      default:
         break;
      }
      break;   

   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->outgoing_server_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->user_name_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->password_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->port_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}

/*****************************************************************************/
// 	Description: initialize incoming setting data
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetIncomingSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
//   GUISETLIST_ITEM_INFO_T item_info  ={0};
   MMI_STRING_T           item_str    ={0};
//   uint32                 i = 0;
   int32                  ret = SUCCESS;

   do 
   {
      s_email_setting.recv_serv_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(INCOMING_SETTING_T));
      EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr, ret, "out of memory");
      EMA_UTILS_MEMSET(s_email_setting.recv_serv_ptr, 0x00, sizeof(INCOMING_SETTING_T));

      if (!is_new_acc)
      {
         store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
         EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_RECEIVE_SERVER_TYPE,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_RECEIVE_SERVER_TYPE failed");
         
         s_email_setting.recv_serv_ptr->server_type = (uint32)val.pData;    
         if (s_email_setting.recv_serv_ptr->server_type == EMASP_POP3)  
         {
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_SERVER,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.recv_serv_ptr->incoming_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
            EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->incoming_server_ptr, ret, "out of memory");
            
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT_DEFAULT,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");

            s_email_setting.recv_serv_ptr->port_type = (uint32)val.pData;
            if (s_email_setting.recv_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
            {
               ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT,&val);
               EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
               
               s_email_setting.recv_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
               EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->port_ptr, ret, "out of memory");
            }

            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_SSL_OR_TLS,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.recv_serv_ptr->secruity = (uint32)val.pData;
         }
         else if (s_email_setting.recv_serv_ptr->server_type == EMASP_IMAP4)
         {
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SERVER,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_SERVER failed");
            
            s_email_setting.recv_serv_ptr->incoming_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
            EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->incoming_server_ptr, ret, "out of memory");
            
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_PORT_DEFAULT,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_PORT_DEFAULT failed");
            
            s_email_setting.recv_serv_ptr->port_type = (uint32)val.pData;
            if (s_email_setting.recv_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
            {
               ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SERVER_PORT,&val);
               EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_SERVER failed");
               
               s_email_setting.recv_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
               EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->port_ptr, ret, "out of memory");
            }
            
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SSL_OR_TLS,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.recv_serv_ptr->secruity = (uint32)val.pData;
         }
      }
      else
      {
         s_email_setting.recv_serv_ptr->server_type = EMASP_POP3;
      }
      
      GUIFORM_SetStyle(EMAIL_INCOMING_SERVER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_INCOMING_SERVER_TYPE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_INCOMING_PORT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      GUIFORM_SetStyle(EMAIL_INCOMING_SECURITY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
      
      //从g_nv_setting_mirror读取自动下载和接收广告的配置通过LIST控件显示        
      GUILABEL_SetTextById(EMAIL_INCOMING_SERVER_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SERVERNAME,FALSE);
      GUILABEL_SetTextById(EMAIL_INCOMING_SERVER_TYPE_TITLE_LABEL_CTRL_ID, TXT_EMAIL_SERVERTYP,FALSE);
      GUILABEL_SetTextById(EMAIL_INCOMING_PORT_TITLE_LABEL_CTRL_ID, TXT_COMM_PORT,FALSE);
      GUILABEL_SetTextById(EMAIL_INCOMING_SECURITY_TITLE_LABEL_CTRL_ID, TXT_COMM_SECURITY,FALSE);

      if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
      {
         item_str.wstr_ptr = s_email_setting.recv_serv_ptr->incoming_server_ptr;
         item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr);
         GUILABEL_SetText(EMAIL_INCOMING_SERVER_LABEL_CTRL_ID, &item_str,FALSE); 
      }

      MMI_GetLabelTextByLang(TXT_EMAIL_POP3 + (s_email_setting.recv_serv_ptr->server_type - 1) , &item_str);
      GUILABEL_SetText(EMAIL_INCOMING_SERVER_TYPE_LABEL_CTRL_ID, &item_str, FALSE);

      
      if (EMASP_PORT_DEFAULT == s_email_setting.recv_serv_ptr->port_type)
      {
         MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &item_str);
         GUILABEL_SetText(EMAIL_INCOMING_PORT_LABEL_CTRL_ID, &item_str, FALSE); 
      }
      else if(EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->port_ptr) > 0)
      {
         item_str.wstr_ptr = s_email_setting.recv_serv_ptr->port_ptr;
         item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->port_ptr);
         GUILABEL_SetText(EMAIL_INCOMING_PORT_LABEL_CTRL_ID, &item_str, FALSE); 
      }

      MMI_GetLabelTextByLang(TXT_EMAIL_SSL_TLS + s_email_setting.recv_serv_ptr->secruity, &item_str);
      GUILABEL_SetText(EMAIL_INCOMING_SECURITY_LABEL_CTRL_ID, &item_str, FALSE);
      
   } while (0);
}

/*****************************************************************************/
// 	Description: handle email incoming setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingSettingWinMsg(
                                                         MMI_WIN_ID_T win_id, 
                                                         MMI_MESSAGE_ID_E msg_id, 
                                                         DPARAM param
                                                         )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
   MMI_CTRL_ID_T  ctrl_id = EMAIL_INCOMING_SERVER_LABEL_CTRL_ID;


   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         SetIncomingSettingParam(TRUE);
#if defined(MMI_PDA_SUPPORT) 
         if (0 == EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr))
         {
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
         }
#else
         if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
         {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
#endif
      }
      else
      {
#if defined(MMI_PDA_SUPPORT) 
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif
         SetIncomingSettingParam(FALSE);
      }
#if defined(MMI_PDA_SUPPORT) 
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
      MMK_SetAtvCtrl(win_id, EMAIL_INCOMING_SETTING_CTRL_ID);
      break;
      
   case MSG_GET_FOCUS:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
        if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
      }
      break;
      
   case MSG_CTL_OK://lint !e616
   case MSG_APP_OK:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
         {
            MMK_CreateWin((uint32*)MMIEMAIL_OUTGOING_SETTING_WIN_TAB, PNULL);      
         }
         break;
      }
#if defined(MMI_PDA_SUPPORT) 
        break;
#endif

   case MSG_APP_WEB://lint !e616  !e825
   case MSG_CTL_MIDSK://lint !e616  !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
      ctrl_id = MMK_GetActiveCtrlId(win_id);
      switch(ctrl_id)
      {
      case EMAIL_INCOMING_SERVER_LABEL_CTRL_ID:
         s_email_setting.edit_type = SETTING_EDIT_TYPE_INCOMING_SERVER;
         MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         break;
         
      case EMAIL_INCOMING_SERVER_TYPE_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32 *)MMIEMAIL_SERVER_TYPE_WIN_TAB,PNULL);
         break;
      case EMAIL_INCOMING_PORT_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32*)MMIEMAIL_SERVER_PORT_WIN_TAB, PNULL);
         break;
      case EMAIL_INCOMING_SECURITY_LABEL_CTRL_ID:
         MMK_CreatePubListWin((uint32*)MMIEMAIL_SECURITY_WIN_TAB, PNULL);
         break;
      default:
         break;
      }
      break;  
   case MSG_APP_CANCEL://lint !e616
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr->incoming_server_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr->port_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}

/*****************************************************************************/
// 	Description: initialize data connection data
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetDataConnectParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
//   GUISETLIST_ITEM_INFO_T item_info  ={0};
   MMI_STRING_T           item_str    ={0};
   MMI_STRING_T           item_str_temp1    ={0};
   MMI_STRING_T           item_str_temp2    ={0};
   uint32                i = 0;
   BOOLEAN               is_sim_ok = FALSE;
   MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
   int32 ret = 0;

   do 
   {
       s_email_setting.data_conn_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(DATA_CONNECTION_T));
       EMA_UTILS_BREAKNULL(s_email_setting.data_conn_ptr, ret, "out of memory");
       EMA_UTILS_MEMSET(s_email_setting.data_conn_ptr, 0x00, sizeof(DATA_CONNECTION_T));
       
       if (!is_new_acc)
       {
           store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
           EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_NET_PATH,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_ACCOUNT_EMAIL failed."));
               return;
           }
           
           s_email_setting.data_conn_ptr->net_path = (uint32)val.pData;  
           
           for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)
           {
               s_email_setting.data_conn_ptr->net_sim[i] = GetNetSettingIndex(i);
               if (s_email_setting.data_conn_ptr->net_sim[i] >= MMIAPICONNECTION_GetLinkSettingNum(i))
               {
                   s_email_setting.data_conn_ptr->net_sim[i] = 0;
                   SetNetSettingIndex(i, s_email_setting.act_id, 0);
               }
           }       
       }
       else
       {
           for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)
           {
               if (MMIAPIPHONE_IsSimOk(i))
               {
                   is_sim_ok = TRUE;
                   s_email_setting.data_conn_ptr->net_path = i; 
                   break;
               }
           }
           
           if (FALSE == is_sim_ok)
           {
               EMA_UTILS_TRACE("SetDataConnectParam, sim is not exsit");
#ifdef MMI_WIFI_SUPPORT
               s_email_setting.data_conn_ptr->net_path = EMASP_NET_PATH_WIFI; 
#else
               s_email_setting.data_conn_ptr->net_path = MN_DUAL_SYS_1; 
#endif
           }
       }
       
       GUIFORM_SetStyle(EMAIL_NOTWORKING_PATH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
       GUILABEL_SetTextById(EMAIL_NOTWORKING_PATH_TITLE_LABEL_CTRL_ID, TXT_EMAIL_NETWORKING_PATH,FALSE);
       
       for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)                                                                                                                             
       {
           GUIFORM_SetStyle(EMAIL_NETWORK_ACCOUNT_SIM1_FORM_CTRL_ID + i,GUIFORM_STYLE_UNIT);
           GUILABEL_SetTextById(EMAIL_NETWORK_ACCOUNT_SIM1_TITLE_LABEL_CTRL_ID + i, TXT_EMAIL_NETWORK_ACCOUNT_SIM1 + i,FALSE);
       }
       
       if (s_email_setting.data_conn_ptr->net_path < MMI_DUAL_SYS_MAX)
       {
           MMI_GetLabelTextByLang(TXT_EMAIL_SIM1 + s_email_setting.data_conn_ptr->net_path, &item_str_temp1);
           if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1 + s_email_setting.data_conn_ptr->net_path))
           {
               wchar               opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
               
               EMA_UTILS_MEMSET(&item_str_temp2, 0x00, sizeof(item_str_temp2));
               item_str_temp2.wstr_ptr = (wchar *)opn_spn_name;//lint !e733
               EMA_UTILS_MEMSET(item_str_temp2.wstr_ptr, 0x00, sizeof(opn_spn_name));
               MMIAPIPHONE_GetSimAndServiceString(s_email_setting.data_conn_ptr->net_path, &item_str_temp2);
           }
           else
           {
               MMI_GetLabelTextByLang(TXT_EMAIL_NO_SIM_CARD, &item_str_temp2);
           }
           
           item_str.wstr_len = item_str_temp1.wstr_len + item_str_temp2.wstr_len + 3;
           item_str.wstr_ptr = EMA_UTILS_MALLOC(PNULL, (item_str.wstr_len + 1) * sizeof(wchar));
           EMA_UTILS_BREAKNULL(item_str.wstr_ptr, ret, "out of memory");
           EMA_UTILS_MEMSET(item_str.wstr_ptr, 0x00, (item_str.wstr_len + 1) * sizeof(wchar));
           EMA_UTILS_MEMCPY(item_str.wstr_ptr, item_str_temp1.wstr_ptr, item_str_temp1.wstr_len * sizeof(wchar));
           EMA_UTILS_WSTRCAT(item_str.wstr_ptr, L" (");
           EMA_UTILS_MEMCPY(item_str.wstr_ptr + item_str_temp1.wstr_len + 2, item_str_temp2.wstr_ptr, item_str_temp2.wstr_len * sizeof(wchar));
           EMA_UTILS_WSTRCAT(item_str.wstr_ptr, L")");
           GUILABEL_SetText(EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID, &item_str, FALSE);
           EMA_UTILS_FREE(PNULL, item_str.wstr_ptr);
       }
       else
       {
           MMI_GetLabelTextByLang(TXT_WIFI, &item_str);
           GUILABEL_SetText(EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID, &item_str, FALSE);
       }
       
       for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)
       {   
           if (!MMIAPIPHONE_IsSimOk(i))
           {
               MMI_GetLabelTextByLang(TXT_EMAIL_NO_SIM_CARD, &item_str);
               GUILABEL_SetText(EMAIL_NETWORK_ACCOUNT_SIM1_LABEL_CTRL_ID + i, &item_str, FALSE);

               GUIFORM_SetChildDisplay(EMAIL_NETWORK_ACCOUNT_SIM1_FORM_CTRL_ID + i,
                   EMAIL_NETWORK_ACCOUNT_SIM1_TITLE_LABEL_CTRL_ID + i,
                   GUIFORM_CHILD_DISP_GRAY);
           }
           else
           {
               link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(i, s_email_setting.data_conn_ptr->net_sim[i]);
               item_str.wstr_ptr = link_setting_ptr->name;
               item_str.wstr_len = link_setting_ptr->name_len;
               GUILABEL_SetText(EMAIL_NETWORK_ACCOUNT_SIM1_LABEL_CTRL_ID + i, &item_str, FALSE);
           }
       }
   } while (0);
}

/*****************************************************************************/
// 	Description: handle email data connection window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDataConnectionWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
    MMI_CTRL_ID_T  ctrl_id = EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        //SetRecvDropDownList();
        if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
        {
#if defined(MMI_PDA_SUPPORT) 

#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
            SetDataConnectParam(TRUE);
        }
        else
        {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif

            SetDataConnectParam(FALSE);
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
        }
        MMK_SetAtvCtrl(win_id, EMAIL_DATA_CONNECTION_CTRL_ID);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
        {
            MMK_CreateWin((uint32*)MMIEMAIL_INCOMING_SETTING_WIN_TAB, PNULL);
            break;
        }
#if defined(MMI_PDA_SUPPORT) 
        break;
#endif

    case MSG_APP_WEB://lint !e616  !e825
    case MSG_CTL_MIDSK://lint !e616  !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        switch(ctrl_id)
        {
        case EMAIL_NOTWORKING_PATH_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32 *)MMIEMAIL_NETWORK_PATH_WIN_TAB,PNULL);
            break;
            
        case EMAIL_NETWORK_ACCOUNT_SIM1_LABEL_CTRL_ID:
            if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1))
            {
                MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
            }
            break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case EMAIL_NETWORK_ACCOUNT_SIM2_LABEL_CTRL_ID:
            if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_2))
            {
                MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_2);
            }
            break;
        case EMAIL_NETWORK_ACCOUNT_SIM3_LABEL_CTRL_ID:
#if defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
            if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_3))
            {
                MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_3);
            }
#endif
            break;
        case EMAIL_NETWORK_ACCOUNT_SIM4_LABEL_CTRL_ID:
#if defined (MULTI_SIM_SYS_QUAD)
            if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_4))
            {
                MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_4);
            }
#endif
            break;
#endif
        default:
            break;
        }
        break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            //todo 关闭窗口
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            EMA_UTILS_FREEIF(PNULL, s_email_setting.data_conn_ptr);
            break;
        default :
            err_code = MMI_RESULT_FALSE;
            break;
    } 
    return err_code;
}


/*****************************************************************************/
// 	Description: initialize load setting data
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetLoadSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr      store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL val = {0};
   MMI_STRING_T           item_str    ={0};
   int32            ret = 0;
   
   do 
   {
       s_email_setting.load_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(LOAD_SETTING_T));
       EMA_UTILS_BREAKNULL(s_email_setting.load_ptr, ret, "out of memory");
       EMA_UTILS_MEMSET(s_email_setting.load_ptr, 0x00, sizeof(LOAD_SETTING_T));
       
       if (!is_new_acc)
       {
           store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
           EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_ACCOUNT_EMAIL,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_ACCOUNT_EMAIL failed."));
               return;
           }
           
           s_email_setting.load_ptr->mail_address_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)val.pData);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->mail_address_ptr, ret, "out of memory");
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_EMAIL_USER_ID,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_EMAIL_USER_ID failed."));
               return;
           }
           
           s_email_setting.load_ptr->user_name_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)val.pData);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->user_name_ptr, ret, "out of memory");
           
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_EMAIL_PASSWORD,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_EMAIL_PASSWORD failed."));
               return;
           }
           
           s_email_setting.load_ptr->password_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)val.pData);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->password_ptr, ret, "out of memory");
       }
       else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
       {
           uint16         index = 0;
           index= GUILIST_GetCurItemIndex(EMAIL_SELECT_EMAIL_ACC_CTRL_ID) - 1;
           
           s_email_setting.load_ptr->mail_address_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)s_email_setting.serv_conf_ptr[index].wszServerName);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->mail_address_ptr, ret, "out of memory");
       }
       
       GUIFORM_SetStyle(EMAIL_EMAIL_ADDRESS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
       GUIFORM_SetStyle(EMAIL_USER_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
       GUIFORM_SetStyle(EMAIL_PASSWORD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
       
       //从g_nv_setting_mirror读取自动下载和接收广告的配置通过LIST控件显示        
       GUILABEL_SetTextById(EMAIL_EMAIL_ADDRESS_TITLE_LABEL_CTRL_ID, TXT_EMAIL_ACC_ADDR,FALSE);
       GUILABEL_SetTextById(EMAIL_USER_NAME_TITLE_LABEL_CTRL_ID, TXT_COMMON_USERNAME,FALSE);
       GUILABEL_SetTextById(EMAIL_PASSWORD_TITLE_LABEL_CTRL_ID, TXT_COMMON_PASSWORD,FALSE);
       
       if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0)
       {
           item_str.wstr_ptr = s_email_setting.load_ptr->mail_address_ptr;
           item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr);
           GUILABEL_SetText(EMAIL_EMAIL_ADDRESS_LABEL_CTRL_ID, &item_str,FALSE); 
       }
       
       if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr) > 0)
       {
           item_str.wstr_ptr = s_email_setting.load_ptr->user_name_ptr;
           item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr);
           GUILABEL_SetText(EMAIL_USER_NAME_LABEL_CTRL_ID, &item_str,FALSE);
       }
       
       if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
       {
           item_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr);
           item_str.wstr_ptr = SETTING_PASSWORD_SHOW_STRING;
           
           GUILABEL_SetText(EMAIL_PASSWORD_LABEL_CTRL_ID, &item_str,FALSE);
       }
   } while (0);
}

/*****************************************************************************/
// 	Description: handle email load setting window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLoadSettingWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T  ctrl_id = EMAIL_EMAIL_ADDRESS_LABEL_CTRL_ID;
   int32         result = 0;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:

      //SetRecvDropDownList();
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
#if defined(MMI_PDA_SUPPORT) 
        if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_FINISH, TXT_NULL, FALSE);
        }
         GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
         GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
         SetLoadSettingParam(TRUE);
      }
      else
      {
#if defined(MMI_PDA_SUPPORT) 
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif
         SetLoadSettingParam(FALSE);
      }
#if defined(MMI_PDA_SUPPORT) 
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
      MMK_SetAtvCtrl(win_id, EMAIL_LOAD_SETTING_CTRL_ID);  
      break;

   case MSG_GET_FOCUS:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0
            //&& EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr) > 0
            && EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, STXT_RETURN, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
         }
      }
      else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0
            //&& EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr) > 0
            && EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, STXT_FINISH, TXT_NULL, STXT_RETURN, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
         }
      }
      
      break;
      
   case MSG_CTL_OK:
   case MSG_APP_OK:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0
            //&& EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr) > 0
            && EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
         {
            if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
            {
               MMK_CreateWin((uint32*)MMIEMAIL_DATA_CONNECTION_WIN_TAB, PNULL);
            }
            else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
            {
               result = CreateEmailAccount(s_email_setting.load_ptr->mail_address_ptr); 
               if (EME_NOSTORAGE == result)
               {
                   MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
                   break;
               }
               else if (EMAIL_SUCCESS != result)
               {
                   MMIPUB_OpenAlertFailWin(TXT_EMAIL_CREATE_NEW_ACC_FAILED);   
                   break;
               }

               MMK_CloseWin(EMAIL_SELECT_EMAIL_ACC_WIN_ID);
               if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID) && !MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID)
                   && !MMK_IsOpenWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID))
               {
                   int i= 0;
                   MMK_CloseWin(win_id);
                   result = MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)g_email_opt_ptr);
                   MMIEMAIL_FreeOptPtr();
               }
               else
               {
                   MMK_CloseWin(win_id);
                   if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
                   {
                       MMK_CreateWin((uint32*)MMIEMAIL_SETTING_WIN_TAB, PNULL);   
                   }
                   else
                   {
                       MMK_SendMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                   }
                   MMK_CloseWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID);
               }
            }
         }
         break;
      }
#if defined(MMI_PDA_SUPPORT) 
        break;
#endif

   case MSG_APP_WEB://lint !e616  !e825
   case MSG_CTL_MIDSK://lint !e616  !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
      ctrl_id = MMK_GetActiveCtrlId(win_id);
      switch(ctrl_id)
      {
      case EMAIL_EMAIL_ADDRESS_LABEL_CTRL_ID:
         s_email_setting.edit_type = SETTING_EDIT_TYPE_MAIL_ADDR;
         MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         break;
      case EMAIL_USER_NAME_LABEL_CTRL_ID:
         s_email_setting.edit_type = SETTING_EDIT_TYPE_USERNAME;
         MMK_CreatePubEditWin((uint32*)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
         break;
      case EMAIL_PASSWORD_LABEL_CTRL_ID:
         s_email_setting.password_type = SETTING_PASSWORD_TYPE_PASSWORD;
         MMK_CreatePubEditWin((uint32*)MMIEMAIL_SETTING_PASSWORD_WIN_TAB,PNULL);
         break;
      default:
         break;
        }
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->mail_address_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->user_name_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->password_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}

#endif  //MMI_GUI_STYLE_TYPICAL

/*****************************************************************************/
// 	Description: handle select email account window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectEmailAccWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id = EMAIL_SELECT_EMAIL_ACC_CTRL_ID;
   uint16         index = 0;
   
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
      {
         uint32 i= 0;
         MMI_STRING_T string ={0};
         
         GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
         GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
         uint32              num                 = 0;
         uint32  image_id[SETTING_MAX_COMMON_ACC] =
         {
             IMAGE_EMAIL_LOGO_FOXMAIL,IMAGE_EMAIL_LOGO_GMAIL,
             IMAGE_EMAIL_LOGO_HOTMAIL,IMAGE_EMAIL_LOGO_163,IMAGE_EMAIL_LOGO_126, IMAGE_EMAIL_LOGO_SINA, 
             IMAGE_EMAIL_LOGO_SINA,IMAGE_EMAIL_LOGO_SOHU,IMAGE_EMAIL_LOGO_QQ,IMAGE_EMAIL_LOGO_139,0,0,0,0,0,0,0,0,0,0
         };
         
         item_data.softkey_id[0] = TXT_COMMON_OK;
         item_data.softkey_id[1] = TXT_NULL;
         item_data.softkey_id[2] = STXT_RETURN;
         item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
         item_t.item_data_ptr = &item_data;
         item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
         item_data.item_content[0].item_data.image_id = IMAGE_EMAIL_LOGO_USER_CUSTOM;
         
         GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE );

         MMI_GetLabelTextByLang(TXT_EMAIL_SELF_DEFINE_ACCOUNT, &string);
         item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
         item_data.item_content[1].item_data.text_buffer.wstr_len = string.wstr_len;
         item_data.item_content[1].item_data.text_buffer.wstr_ptr = string.wstr_ptr;
         GUILIST_AppendItem(ctrl_id, &item_t);

         MMIEMAIL_SP_Store_GetDefaultAccount(EMA_GETSTORE_HANDLE(g_email_sp_handle), &num, &s_email_setting.serv_conf_ptr);

         for (i=0; i<num; i++)
         {
            item_data.item_content[0].item_data.image_id = image_id[i];
            item_data.item_content[1].item_data.text_buffer.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.serv_conf_ptr[i].wszActName);
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = s_email_setting.serv_conf_ptr[i].wszActName;
            GUILIST_AppendItem(ctrl_id, &item_t);
         }
         MMK_SetAtvCtrl(win_id, ctrl_id);
      }
      break;
   case MSG_CTL_OK:
   case MSG_APP_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      index= GUILIST_GetCurItemIndex(ctrl_id);
      if (0 == index)
      {
         s_email_setting.enter_type = SETTING_ENTER_TYPE_CUSTOMIZED_ACC;
         MMK_CreateWin((uint32*)MMIEMAIL_LOAD_SETTING_WIN_TAB, PNULL);
      }
      else 
      {
         s_email_setting.enter_type = SETTING_ENTER_TYPE_COMMON_ACC;
         MMK_CreateWin((uint32*)MMIEMAIL_LOAD_SETTING_WIN_TAB, PNULL);
      }
      
      break;
    case MSG_APP_RED:
   case MSG_CTL_CANCEL:
   case MSG_APP_CANCEL:
       {
           MMK_CloseWin(win_id);
      }
      break;
   default:
      recode = MMI_RESULT_FALSE;
      break;
   }
   
   return recode;
}

/*****************************************************************************/
// 	Description: handle email entry tip window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEntryEmailTipWinMsg(
                                                       MMI_WIN_ID_T win_id, 
                                                       MMI_MESSAGE_ID_E msg_id, 
                                                       DPARAM param
                                                       )
{
   GUI_RECT_T      rect = MMITHEME_GetClientRect();
   MMI_RESULT_E    recode = MMI_RESULT_TRUE;
   GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
   MMI_CTRL_ID_T ctrl_id = EMAIL_ENTRY_EMAIL_TIP_CTRL_ID;
   
   lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
   lcd_dev_info.block_id = GUI_BLOCK_MAIN;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      {
         GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
         MMI_STRING_T string = {0}; 
         uint16    index= 0;
         
         item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
         item_data.text_type = GUIRICHTEXT_TEXT_BUF;
         
         MMI_GetLabelTextByLang(TXT_EMAIL_USE_TIP, &string);
         item_data.text_data.buf.len = string.wstr_len;
         item_data.text_data.buf.str_ptr = string.wstr_ptr;
         GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
         MMK_SetAtvCtrl(win_id, ctrl_id);
      }
      
      break;
      
   case MSG_FULL_PAINT:
      LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
      break;
      
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      MMK_CloseWin(win_id);
      break;
      
   case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_MIDSK:
   case MSG_CTL_OK:
   case MSG_APP_OK:
    {
      MMK_CreateWin((uint32*)MMIEMAIL_SELECT_EMAIL_ACC_WIN_TAB, PNULL);
    }
      break;
   case MSG_CLOSE_WINDOW:
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
       if(MMIAPIEMAIL_GetIsPbSendEdit())
       {
           MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc ={0};
           MMIAPIEMAIL_SetSendVcardDocument(&pb_edit_doc);
           MMIAPIEMAIL_SetIsPbSendEdit(FALSE);
           
       }
#endif
       if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
       {
           MMIEMAIL_ExitSetting();
       }
      break;
   default:
      recode = MMI_RESULT_FALSE;
      break;
   }
   
   return recode;
}


/*****************************************************************************/
// 	Description:
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_InitSetting(uint32 act_id)
{
    s_email_setting.act_id = act_id;
    s_email_setting.enter_type = SETTING_ENTER_TYPE_EXSIT_ACC;
}

/*****************************************************************************/
// 	Description:
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_IsNetAvailable(void)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
   if(MMIAPISET_GetFlyMode())
   {
        return FALSE;
   }

#ifdef MMI_WIFI_SUPPORT
    return TRUE;
#else
    for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)
    {
       if (MMIAPIPHONE_IsSimOk(i))
       {
           return TRUE;
       }
    }

    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description:
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_IsCurrentConfigNetAvailable(void)
{
    EMASP_PRFITEM_VAL      val = {0}; 
    MN_DUAL_SYS_E          dual_sys = MN_DUAL_SYS_1;
    uint32                 ret =0;
    ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, EMASP_PRFITEM_NET_PATH,&val);
    if (EMAIL_SUCCESS != ret)
    {
        EMA_UTILS_TRACE("MMIEMAIL_IsCurrentConfigNetAvailable, call MMIEMAIL_SP_Store_GetActCfgItem failed");
    }
    dual_sys = (uint32)val.pData;
    
    if (MMIAPIPHONE_IsSimOk(dual_sys)
#ifdef MMI_WIFI_SUPPORT
    || (dual_sys == MMI_DUAL_SYS_MAX && MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
#endif
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
// 	Description: is device ok or not
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_IsDeviceReady(BOOLEAN is_pop_alert_win)
{   
   if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
   {
        if (is_pop_alert_win)
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
       return FALSE;
   }

#ifndef NANDBOOT_SUPPORT
    {
        MMIFILE_DEVICE_E file_device = MMIAPIFMM_GetFirstValidSD();
        if (MMI_DEVICE_NUM <= file_device)
        {
            if (is_pop_alert_win)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
            }
            return FALSE;
        }
    }
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description: start setting
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_SettingStart(void)
{
   int result = EMAIL_ERROR;

   if (!MMIEMAIL_IsDeviceReady(TRUE))
   {
        return EMAIL_ERROR;
   }
       
    //弹出等待窗口，提前促使释放窗口内存
    MMIPUB_OpenTempWaitWin();
    BLKMEM_GarbageClean();
#ifdef UI_P3D_SUPPORT
   // MMI_Disable3DEffect(MMI_3D_EFFECT_EMAIL);
#endif
  
   MMIEmail_ThreadEnable();
   
   result = MMIEMAIL_SP_InitHandle(&g_email_sp_handle);
   if(EMAIL_SUCCESS != result)
   {
       if(EMAIL_NOMEMORY == result)
       {
           MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
       }
       else
       {
           EMA_UTILS_LOG_ERROR(("MMIEmail MMIEmail_Create failed"));
           MMIPUB_OpenAlertWarningWin(TXT_COMM_INIT_FAIL);
       }
       MMIEMAIL_ExitSetting();
       return result;
   }
       
   if(E_EMAILA_NOT_ACCOUNT == MMIEMAIL_SP_CFG_ExsitAct(EMA_GETCFG_HANDLE(g_email_sp_handle)))
   {
        MMIEMAIL_EntryEmailTip();
   }
   else
   {
        MMK_CreateWin((uint32 *)MMIEMAIL_SETTING_WIN_TAB, PNULL);
   }

   return result;
}

/*****************************************************************************/
// 	Description: entry email tip window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_EntryEmailTip(void)
{
    MMK_CreateWin((uint32*)MMIEMAIL_ENTRY_EMAIL_TIP_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: free g_email_opt_ptr;
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_FreeOptPtr(void)
{
    int i = 0;
    while(PNULL != g_email_opt_ptr && EMA_OPT_END != g_email_opt_ptr[i].opt_id)
    {
        EMA_UTILS_FREEIF(PNULL, g_email_opt_ptr[i].val_ptr);
        i++;
    }
    EMA_UTILS_FREEIF(PNULL, g_email_opt_ptr);
}

/*****************************************************************************/
// 	Description: entry account setting window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_EntryAccountSetting(void)
{
   s_email_setting.enter_type = SETTING_ENTER_TYPE_EXSIT_ACC;
   MMK_CreateWin((uint32*)MMIEMAIL_ACCOUNT_SETTING_WIN_TAB, PNULL);
}


/*****************************************************************************/
// 	Description: entry system setting window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_EntrySystemSetting(void)
{   
   MMK_CreateWin((uint32*)MMIEMAIL_SYSTEM_SETTING_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: entry system setting window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_SelectEmailAcc(void)
{
    MMK_CreateWin((uint32*)MMIEMAIL_SELECT_EMAIL_ACC_WIN_TAB, PNULL);
}
/*****************************************************************************/
// 	Description : reset account setting
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void  ResetAccountSettings(uint32 act_id)
{
    EMASP_MSG_OPT setting_opt[20] = {0};
    uint32    index  = 0;
    EMASP_PRFITEM_VAL val = {0};
    int32     ret = 0;

    ret = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),act_id,EMASP_PRFITEM_ACCOUNT_EMAIL,&val);
    if (EMAIL_SUCCESS != ret)
    {
        EMA_UTILS_TRACE("ResetAccountSettings, call MMIEMAIL_SP_Store_GetActCfgItem failed");
    }
    setting_opt[index].nId = EMASP_PRFITEM_ACCOUNT_NAME;
    setting_opt[index].pVal = val.pData;
    index++;

//     setting_opt[index].nId = EMASP_PRFITEM_SEND_MAIL;
//     setting_opt[index].pVal = (void*)0;
//     index++;

//     setting_opt[index].nId = EMASP_PRFITEM_SENDER_NAME;
//     setting_opt[index].pVal = (void*)val.pData;
//     index++;

    setting_opt[index].nId = EMASP_PRFITEM_SAVE_SENT;
    setting_opt[index].pVal = (void*)TRUE;
    index++;

//     setting_opt[index].nId = EMASP_PRFITEM_RECEIVE_FREQUENCY;
//     setting_opt[index].pVal = (void*)0;
//     index++;

    setting_opt[index].nId = EMASP_PRFITEM_NUMBER_LIMIT;
    setting_opt[index].pVal = (void*)0;
    index++;

    setting_opt[index].nId = EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE;
    setting_opt[index].pVal = (void*)EMA_DOWNLOAD_ONLY_HEADERS;
    index++;

    setting_opt[index].nId = EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL;
    setting_opt[index].pVal = (void*)TRUE;
    index++;

    setting_opt[index].nId = EMASP_PRFITEM_INCLUDE_ORIGINAL;
    setting_opt[index].pVal = (void*)TRUE;
    index++;

//     setting_opt[index].nId = EMASP_PRFITEM_MAIL_AUTO_DELETE;
//     setting_opt[index].pVal = (void*)0;
//     index++;

//     setting_opt[index].nId = EMASP_PRFITEM_MAIL_DELETE;
//     setting_opt[index].pVal = (void*)0;
//     index++;

//     setting_opt[index].nId = EMASP_PRFITEM_EMAIL_CREATE_SIGNATURE;
//     setting_opt[index].pVal = (void*)PNULL;
//     index++;
// 
//     setting_opt[index].nId = EMASP_PRFITEM_NEW_MAIL_TIP;
//     setting_opt[index].pVal = (void*)0;
//     index++;

    setting_opt[index].nId = EMASPMSGOPT_END;

    ret = MMIEMAIL_SP_Store_SetActCfgItemEx(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),act_id, setting_opt);
    if (EMAIL_SUCCESS != ret)
    {
        EMA_UTILS_TRACE("ResetAccountSettings, call MMIEMAIL_SP_Store_SetActCfgItemEx failed");
    }
}


/*****************************************************************************/
// 	Description : email reset settings
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_ResetSettings(void)
{
    int32 result = 0;
    EMAIL_VECTOR_T *vector_ptr = PNULL;
    int32 acc_num = 0;
    int32 i = 0;
    EMASP_ACTINFO_T * act_info_ptr = PNULL;


    do 
    {
        result = MMIEMAIL_SP_InitHandle(&g_email_sp_handle);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_ResetSettings, init sp handle failed failed");

        result = MMIEMAIL_VectorNew(&vector_ptr);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_VectorNew failed, vector_ptr is NULL");
        
        result = MMIEMAIL_SP_Store_GetActInfo(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle),0,EMASP_ACTINFO_MAX_NUMBER,vector_ptr);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_ResetSettings--MMIEMAIL_SP_Store_GetActInfo failed");


        acc_num = MMIEMAIL_VectorGetVectorNumber(vector_ptr);
        if(acc_num > 0)
        {
            for(i = 0; i < acc_num; i ++)
            {
                MMIEMAIL_VectorGetAt(vector_ptr, i, (void **)&act_info_ptr);
                ResetAccountSettings(act_info_ptr->act_id);
            }
        }
    } while (0);

    if (PNULL != vector_ptr)
    {
        MMIEMAIL_VectorDelete(vector_ptr);
    }
    
    if (PNULL != g_email_sp_handle)
    {
        MMIEMAIL_SP_DeleteHandle(&g_email_sp_handle);
    }
    return result;
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIEMAIL_AppendTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *string_1, MMI_STRING_T *string_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    if (PNULL != string_1)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_1->wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = string_1->wstr_len;
    }

    if (PNULL != string_2)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = string_2->wstr_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = string_2->wstr_len;
    }
 
    if (IMAGE_NULL != image_id)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.text_id = image_id;
    }

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIEMAIL_AppendTwoTextItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *string_1, MMI_STRING_T *string_2, uint32 user_data)
{
    MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id, string_1, string_2, IMAGE_NULL, user_data);
/*    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    if (PNULL != string_1)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_1->wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = string_1->wstr_len;
    }

    if (PNULL != string_1)
    {
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = string_2->wstr_ptr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = string_2->wstr_len;
    }

    GUILIST_AppendItem(ctrl_id, &item_t);*/
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterLoadSettingAdressWin(void)
{
    s_email_setting.edit_type = SETTING_EDIT_TYPE_MAIL_ADDR;
    MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterLoadSettingNameWin(void)
{
    s_email_setting.edit_type = SETTING_EDIT_TYPE_USERNAME;
    MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterLoadSettingPasswordWin(void)
{
    s_email_setting.password_type = SETTING_PASSWORD_TYPE_PASSWORD;
    MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_PASSWORD_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void InitLoadSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr      store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL val = {0};
   int32            ret = 0;
   
   do 
   {
       s_email_setting.load_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(LOAD_SETTING_T));
       EMA_UTILS_BREAKNULL(s_email_setting.load_ptr, ret, "out of memory");
       EMA_UTILS_MEMSET(s_email_setting.load_ptr, 0x00, sizeof(LOAD_SETTING_T));
       
       if (!is_new_acc)
       {
           store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
           EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_ACCOUNT_EMAIL,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_ACCOUNT_EMAIL failed."));
               return;
           }
           
           s_email_setting.load_ptr->mail_address_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)val.pData);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->mail_address_ptr, ret, "out of memory");
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_EMAIL_USER_ID,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_EMAIL_USER_ID failed."));
               return;
           }
           
           s_email_setting.load_ptr->user_name_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)val.pData);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->user_name_ptr, ret, "out of memory");
           
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_EMAIL_PASSWORD,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_EMAIL_PASSWORD failed."));
               return;
           }
           
           s_email_setting.load_ptr->password_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)val.pData);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->password_ptr, ret, "out of memory");
       }
       else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
       {
           uint16         index = 0;
           index= GUILIST_GetCurItemIndex(EMAIL_SELECT_EMAIL_ACC_CTRL_ID) - 1;
           
           s_email_setting.load_ptr->mail_address_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)s_email_setting.serv_conf_ptr[index].wszServerName);
           EMA_UTILS_BREAKNULL(s_email_setting.load_ptr->mail_address_ptr, ret, "out of memory");
       }
       
   } while (0);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetLoadSettingListParam(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_new_acc)
{
    MMI_STRING_T           item1_str    ={0};
    MMI_STRING_T           item2_str    ={0};
    wchar               temp_arr[1]= {0};
    uint16              current_index = 0;
    uint16              top_index = 0;

    current_index = GUILIST_GetCurItemIndex(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);
    
       
   MMIRES_GetText(TXT_EMAIL_ACC_ADDR, PNULL, &item1_str);
   if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0)
   {
       item2_str.wstr_ptr = s_email_setting.load_ptr->mail_address_ptr;
       item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr);
       MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterLoadSettingAdressWin);
   }
   else
   {
       item2_str.wstr_ptr = temp_arr;
       item2_str.wstr_len = 0;
       MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterLoadSettingAdressWin);
   }
    
   MMIRES_GetText(TXT_COMMON_PASSWORD, PNULL, &item1_str);
   if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
   {
       item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr);
       item2_str.wstr_ptr = (wchar*)SETTING_PASSWORD_SHOW_STRING;
       MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterLoadSettingPasswordWin);
   }
   else
   {
       item2_str.wstr_ptr = temp_arr;
       item2_str.wstr_len = 0;
       MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterLoadSettingPasswordWin);
   }
       
    GUILIST_SetCurItemIndex(ctrl_id, current_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
}

/*****************************************************************************/
// 	Description: handle email Setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLoadSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T  ctrl_id = EMAIL_LOAD_SETTING_LIST_CTRL_ID;
   int32         result = 0;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:

      //SetRecvDropDownList();
      
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
#if defined(MMI_PDA_SUPPORT) 
        if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_FINISH, TXT_NULL, FALSE);
        }
         GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
#ifndef MID_SOFTKEY_USE_IMG_SUPPORT
         GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#else
         GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_SELECTED_ICON, 1, FALSE);
#endif        
#endif
         InitLoadSettingParam(TRUE);
      }
      else
      {
#if defined(MMI_PDA_SUPPORT) 
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif
         InitLoadSettingParam(FALSE);
      }
#if defined(MMI_PDA_SUPPORT) 
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
      GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE);
      MMK_SetAtvCtrl(win_id, ctrl_id);  
      break;

   case MSG_GET_FOCUS:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0
            //&& EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr) > 0
            && EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, STXT_RETURN, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
         }
      }
      else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0
            //&& EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr) > 0
            && EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, STXT_FINISH, TXT_NULL, STXT_RETURN, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
#ifndef MID_SOFTKEY_USE_IMG_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#else
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_SELECTED_ICON, 1, FALSE);
#endif 
#endif
         }
      }
      
      break;
      
    case MSG_FULL_PAINT:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         SetLoadSettingListParam(ctrl_id,TRUE);
      }
      else
      {
         SetLoadSettingListParam(ctrl_id,FALSE);
      }
        break;
        
   case MSG_CTL_OK:
   case MSG_APP_OK:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->mail_address_ptr) > 0
            //&& EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->user_name_ptr) > 0
            && EMA_UTILS_WSTRLEN(s_email_setting.load_ptr->password_ptr) > 0)
         {
            if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
            {
               MMK_CreateWin((uint32*)MMIEMAIL_DATA_CONNECTION_WIN_TAB, PNULL);
            }
            else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
            {
               result = CreateEmailAccount(s_email_setting.load_ptr->mail_address_ptr); 
               if (EME_NOSTORAGE == result)
               {
                   MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
                   break;
               }
               else if (EMAIL_SUCCESS != result)
               {
                   MMIPUB_OpenAlertFailWin(TXT_EMAIL_CREATE_NEW_ACC_FAILED);   
                   break;
               }

               MMK_CloseWin(EMAIL_SELECT_EMAIL_ACC_WIN_ID);
               if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID) && !MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID)
                   && !MMK_IsOpenWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID))
               {
                   int i= 0;
                   MMK_CloseWin(win_id);
                   result = MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)g_email_opt_ptr);
                   MMIEMAIL_FreeOptPtr();
               }
               else
               {
                   MMK_CloseWin(win_id);
                   if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
                   {
                       MMK_CreateWin((uint32*)MMIEMAIL_SETTING_WIN_TAB, PNULL);   
                   }
                   else
                   {
                       MMK_SendMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                   }
                   MMK_CloseWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID);
               }
            }
         }
         break;
      }
        break;

   case MSG_APP_WEB://lint !e616  !e825
   case MSG_CTL_MIDSK://lint !e616  !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
        {
            uint32              index = 0;
            SETTING_ITEM_FUNC   item_func = PNULL;

            index = GUILIST_GetCurItemIndex(ctrl_id);

            GUILIST_GetItemData(ctrl_id, index, (uint32*)(&item_func));

            if (PNULL != item_func)
            {
                (*item_func)();
            }
        }
      break;
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->mail_address_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->user_name_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr->password_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.load_ptr);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterDataConnectPathWin(void)
{
    MMK_CreatePubListWin((uint32 *)MMIEMAIL_NETWORK_PATH_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterDataConnectSim1LinkListWin(void)
{
        if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1))
        {
            MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_1);
        }
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterDataConnectSim2LinkListWin(void)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_2))
        {
            MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_2);
        }
#endif
    return;
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterDataConnectSim3LinkListWin(void)
{
#if defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
        if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_3))
        {
            MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_3);
        }
#endif
    return;
}

LOCAL void EnterDataConnectSim4LinkListWin(void)
{
#if defined (MULTI_SIM_SYS_QUAD)
        if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_4))
        {
            MMK_CreatePubListWin((uint32*)MMIEMAIL_ACCOUNT_LINKLIST_WIN_TAB, (ADD_DATA)MN_DUAL_SYS_4);
        }
#endif
    return;
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void InitDataConnectParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
   uint32                i = 0;
   BOOLEAN               is_sim_ok = FALSE;
   int32 ret = 0;

   do 
   {
       s_email_setting.data_conn_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(DATA_CONNECTION_T));
       EMA_UTILS_BREAKNULL(s_email_setting.data_conn_ptr, ret, "out of memory");
       EMA_UTILS_MEMSET(s_email_setting.data_conn_ptr, 0x00, sizeof(DATA_CONNECTION_T));
       
       if (!is_new_acc)
       {
           store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
           EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");
           
           if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_NET_PATH,&val))
           {
               EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_ACCOUNT_EMAIL failed."));
               return;
           }
           
           s_email_setting.data_conn_ptr->net_path = (uint32)val.pData;  
           
           for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)
           {
               s_email_setting.data_conn_ptr->net_sim[i] = GetNetSettingIndex(i);
               if (s_email_setting.data_conn_ptr->net_sim[i] >= MMIAPICONNECTION_GetLinkSettingNum(i))
               {
                   s_email_setting.data_conn_ptr->net_sim[i] = 0;
                   SetNetSettingIndex(i, s_email_setting.act_id, 0);
               }
           }       
       }
       else
       {
           for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)
           {
               if (MMIAPIPHONE_IsSimOk(i))
               {
                   is_sim_ok = TRUE;
                   s_email_setting.data_conn_ptr->net_path = i; 
                   break;
               }
           }
           
           if (FALSE == is_sim_ok)
           {
               EMA_UTILS_TRACE("SetDataConnectParam, sim is not exsit");
#ifdef MMI_WIFI_SUPPORT
               s_email_setting.data_conn_ptr->net_path = EMASP_NET_PATH_WIFI; 
#else
               s_email_setting.data_conn_ptr->net_path = MN_DUAL_SYS_1; 
#endif
           }
       }
   } while (0);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetDataConnectListParam(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_new_acc)
{
    MMI_STRING_T           item1_str    ={0};
    MMI_STRING_T           item2_str    ={0};
    MMI_STRING_T           item_str_temp1    ={0};
    MMI_STRING_T           item_str_temp2    ={0};
    uint16              current_index = 0;
    uint16              top_index = 0;
    uint32                i = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
    wchar               opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
    SETTING_ITEM_FUNC linklistfun[] = 
                                                                {
                                                                        EnterDataConnectSim1LinkListWin,
                                                                        EnterDataConnectSim2LinkListWin,
                                                                        EnterDataConnectSim3LinkListWin,
                                                                        EnterDataConnectSim4LinkListWin
                                                                };

                                                                                    
        current_index = GUILIST_GetCurItemIndex(ctrl_id);
        top_index = GUILIST_GetTopItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
    
       MMIRES_GetText(TXT_EMAIL_NETWORKING_PATH, PNULL, &item1_str);
       if (s_email_setting.data_conn_ptr->net_path < MMI_DUAL_SYS_MAX)
       {
           MMI_GetLabelTextByLang(TXT_EMAIL_SIM1 + s_email_setting.data_conn_ptr->net_path, &item_str_temp1);
           if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1 + s_email_setting.data_conn_ptr->net_path))
           {

               
               EMA_UTILS_MEMSET(&item_str_temp2, 0x00, sizeof(item_str_temp2));
               item_str_temp2.wstr_ptr = (wchar *)opn_spn_name;
               EMA_UTILS_MEMSET(item_str_temp2.wstr_ptr, 0x00, sizeof(opn_spn_name));
               MMIAPIPHONE_GetSimAndServiceString(s_email_setting.data_conn_ptr->net_path, &item_str_temp2);
           }
           else
           {
               MMI_GetLabelTextByLang(TXT_EMAIL_NO_SIM_CARD, &item_str_temp2);
           }
           
           item2_str.wstr_len = item_str_temp1.wstr_len + item_str_temp2.wstr_len + 3;
           item2_str.wstr_ptr = EMA_UTILS_MALLOC(PNULL, (item2_str.wstr_len + 1) * sizeof(wchar));
           EMA_UTILS_MEMSET(item2_str.wstr_ptr, 0x00, (item2_str.wstr_len + 1) * sizeof(wchar));
           EMA_UTILS_MEMCPY(item2_str.wstr_ptr, item_str_temp1.wstr_ptr, item_str_temp1.wstr_len * sizeof(wchar));
           EMA_UTILS_WSTRCAT(item2_str.wstr_ptr, (wchar*)L" (");
           EMA_UTILS_MEMCPY(item2_str.wstr_ptr + item_str_temp1.wstr_len + 2, item_str_temp2.wstr_ptr, item_str_temp2.wstr_len * sizeof(wchar));
           EMA_UTILS_WSTRCAT(item2_str.wstr_ptr, (wchar*)L")");
           MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterDataConnectPathWin);
           EMA_UTILS_FREE(PNULL, item2_str.wstr_ptr);
       }
       else
       {
           MMIRES_GetText(TXT_WIFI, PNULL, &item2_str);
           MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterDataConnectPathWin);
       }

       for (i=MN_DUAL_SYS_1 ; i < MMI_DUAL_SYS_MAX; i++)
       {   
           MMIRES_GetText(TXT_EMAIL_NETWORK_ACCOUNT_SIM1 + i, PNULL, &item1_str);
           if (!MMIAPIPHONE_IsSimOk(i))
           {
               MMIRES_GetText(TXT_EMAIL_NO_SIM_CARD, PNULL, &item2_str);
               MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)linklistfun[i]);
               GUILIST_SetItemGray(ctrl_id,i+1,TRUE);
           }
           else
           {
               link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(i, s_email_setting.data_conn_ptr->net_sim[i]);
               item2_str.wstr_ptr = link_setting_ptr->name;
               item2_str.wstr_len = link_setting_ptr->name_len;
               MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)linklistfun[i]);
           }
       }
    GUILIST_SetCurItemIndex(ctrl_id, current_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
}

/*****************************************************************************/
// 	Description: handle data Connectiong list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDataConnectionListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
    MMI_CTRL_ID_T  ctrl_id = EMAIL_DATA_CONNECTION_LIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        //SetRecvDropDownList();
        if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
        {
#if defined(MMI_PDA_SUPPORT) 

#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
            InitDataConnectParam(TRUE);
        }
        else
        {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif

            InitDataConnectParam(FALSE);
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
        }
        GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         SetDataConnectListParam(ctrl_id,TRUE);
      }
      else
      {
         SetDataConnectListParam(ctrl_id,FALSE);
      }
        break;

        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
        {
            MMK_CreateWin((uint32*)MMIEMAIL_INCOMING_SETTING_WIN_TAB, PNULL);
            break;
        }
#if defined(MMI_PDA_SUPPORT) 
        break;
#endif

    case MSG_APP_WEB://lint !e616  !e825
    case MSG_CTL_MIDSK://lint !e616  !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif

        {
            uint32              index = 0;
            SETTING_ITEM_FUNC   item_func = PNULL;

            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, (uint32*)(&item_func));

            if (PNULL != item_func)
                {
                    (*item_func)();
                }
        }

        break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            //todo 关闭窗口
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            EMA_UTILS_FREEIF(PNULL, s_email_setting.data_conn_ptr);
            break;
        default :
            err_code = MMI_RESULT_FALSE;
            break;
    } 
    return err_code;
}                                                

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterIncomingSettingServerNameWin(void)
{
     s_email_setting.edit_type = SETTING_EDIT_TYPE_INCOMING_SERVER;
     MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterIncomingSettingServerTypeWin(void)
{
     MMK_CreatePubListWin((uint32 *)MMIEMAIL_SERVER_TYPE_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterIncomingSettingServerPortWin(void)
{
     MMK_CreatePubListWin((uint32*)MMIEMAIL_SERVER_PORT_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterIncomingSettingSecurityWin(void)
{
     MMK_CreatePubListWin((uint32*)MMIEMAIL_SECURITY_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void InitIncomingSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
   int32                  ret = SUCCESS;

   do 
   {
      s_email_setting.recv_serv_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(INCOMING_SETTING_T));
      EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr, ret, "out of memory");
      EMA_UTILS_MEMSET(s_email_setting.recv_serv_ptr, 0x00, sizeof(INCOMING_SETTING_T));

      if (!is_new_acc)
      {
         store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
         EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_RECEIVE_SERVER_TYPE,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_RECEIVE_SERVER_TYPE failed");
         
         s_email_setting.recv_serv_ptr->server_type = (uint32)val.pData;    
         if (s_email_setting.recv_serv_ptr->server_type == EMASP_POP3)  
         {
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_SERVER,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.recv_serv_ptr->incoming_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
            EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->incoming_server_ptr, ret, "out of memory");
            
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT_DEFAULT,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");

            s_email_setting.recv_serv_ptr->port_type = (uint32)val.pData;
            if (s_email_setting.recv_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
            {
               ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_PORT,&val);
               EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
               
               s_email_setting.recv_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
               EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->port_ptr, ret, "out of memory");
            }

            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_POP3_SSL_OR_TLS,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.recv_serv_ptr->secruity = (uint32)val.pData;
         }
         else if (s_email_setting.recv_serv_ptr->server_type == EMASP_IMAP4)
         {
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SERVER,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_SERVER failed");
            
            s_email_setting.recv_serv_ptr->incoming_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
            EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->incoming_server_ptr, ret, "out of memory");
            
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_PORT_DEFAULT,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_PORT_DEFAULT failed");
            
            s_email_setting.recv_serv_ptr->port_type = (uint32)val.pData;
            if (s_email_setting.recv_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
            {
               ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SERVER_PORT,&val);
               EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_IMAP4_SERVER failed");
               
               s_email_setting.recv_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
               EMA_UTILS_BREAKNULL(s_email_setting.recv_serv_ptr->port_ptr, ret, "out of memory");
            }
            
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_IMAP4_SSL_OR_TLS,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.recv_serv_ptr->secruity = (uint32)val.pData;
         }
      }
      else
      {
         s_email_setting.recv_serv_ptr->server_type = EMASP_POP3;
      }
   } while (0);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetIncomingSettingListParam(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_new_acc)
{
    MMI_STRING_T           item1_str    ={0};
    MMI_STRING_T           item2_str    ={0};
    wchar               temp_arr[1]= {0};
    uint16              current_index = 0;
    uint16              top_index = 0;

    current_index = GUILIST_GetCurItemIndex(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);

    MMIRES_GetText(TXT_EMAIL_SERVERNAME, PNULL, &item1_str);
    if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
    {
        item2_str.wstr_ptr = s_email_setting.recv_serv_ptr->incoming_server_ptr;
        item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr);
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterIncomingSettingServerNameWin);
    }
    else
    {
        item2_str.wstr_ptr = temp_arr;
        item2_str.wstr_len = 0;
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterIncomingSettingServerNameWin);
    }

    MMIRES_GetText(TXT_EMAIL_SERVERTYP, PNULL, &item1_str);
    MMI_GetLabelTextByLang(TXT_EMAIL_POP3 + (s_email_setting.recv_serv_ptr->server_type - 1) , &item2_str);
    MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterIncomingSettingServerTypeWin);

    MMIRES_GetText(TXT_COMM_PORT, PNULL, &item1_str);
    if (EMASP_PORT_DEFAULT == s_email_setting.recv_serv_ptr->port_type)
    {
        MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &item2_str);
        MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterIncomingSettingServerPortWin);
    }
    else if(EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->port_ptr) > 0)
    {
        item2_str.wstr_ptr = s_email_setting.recv_serv_ptr->port_ptr;
        item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->port_ptr);
        MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterIncomingSettingServerPortWin);
    }
    else
    {
        item2_str.wstr_ptr = temp_arr;
        item2_str.wstr_len = 0;
        MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterIncomingSettingServerPortWin);
    }
    
    MMIRES_GetText(TXT_COMM_SECURITY, PNULL, &item1_str);
    MMI_GetLabelTextByLang(TXT_EMAIL_SSL_TLS + s_email_setting.recv_serv_ptr->secruity, &item2_str);
    MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterIncomingSettingSecurityWin);
      
    GUILIST_SetCurItemIndex(ctrl_id, current_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
}

/*****************************************************************************/
// 	Description: handle Incoming Setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
   MMI_CTRL_ID_T  ctrl_id = EMAIL_INCOMING_SETTING_LIST_CTRL_ID;


   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         InitIncomingSettingParam(TRUE);
#if defined(MMI_PDA_SUPPORT) 
         if (0 == EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr))
         {
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
         }
#else
         if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
         {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
#endif
      }
      else
      {
#if defined(MMI_PDA_SUPPORT) 
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif
         InitIncomingSettingParam(FALSE);
      }
#if defined(MMI_PDA_SUPPORT) 
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
    GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE);
      MMK_SetAtvCtrl(win_id, ctrl_id);
      break;
      
   case MSG_GET_FOCUS:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
        if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
      }
      break;

    case MSG_FULL_PAINT:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         SetIncomingSettingListParam(ctrl_id,TRUE);
      }
      else
      {
         SetIncomingSettingListParam(ctrl_id,FALSE);
      }
        break;
      
   case MSG_CTL_OK://lint !e616
   case MSG_APP_OK:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.recv_serv_ptr->incoming_server_ptr) > 0)
         {
            MMK_CreateWin((uint32*)MMIEMAIL_OUTGOING_SETTING_WIN_TAB, PNULL);      
         }
         break;
      }
#if defined(MMI_PDA_SUPPORT) 
        break;
#endif

   case MSG_APP_WEB://lint !e616  !e825
   case MSG_CTL_MIDSK://lint !e616  !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
        {
            uint32              index = 0;
            SETTING_ITEM_FUNC   item_func = PNULL;

            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, (uint32*)(&item_func));

            if (PNULL != item_func)
                {
                    (*item_func)();
                }
        }

      break;  
   case MSG_APP_CANCEL://lint !e616
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr->incoming_server_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr->port_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.recv_serv_ptr);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterOutGoingSettingServerNameWin(void)
{
     s_email_setting.edit_type = SETTING_EDIT_TYPE_SMTP_SERVER;
     MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterOutGoingSettingUserNameWin(void)
{
     s_email_setting.edit_type = SETTING_EDIT_TYPE_SMTP_USERNAME;
     MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterOutGoingSettingPasswordWin(void)
{
     s_email_setting.password_type = SETTING_PASSWORD_TYPE_SMTP_PASSWORD;
     MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_PASSWORD_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterOutGoingSettingPortWin(void)
{
     MMK_CreatePubListWin((uint32*)MMIEMAIL_SERVER_PORT_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterOutGoingSettingSecurityWin(void)
{
     MMK_CreatePubListWin((uint32*)MMIEMAIL_SECURITY_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterOutGoingSettingAuthernticateWin(void)
{
    // if (EMASP_SECURITY_OFF != s_email_setting.send_serv_ptr->secruity)
     {
         MMK_CreatePubListWin((uint32*)MMIEMAIL_AUTHENTICATE_WIN_TAB, PNULL);
     }
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void InitOutgoingSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
   int32                  ret = SUCCESS;

   do 
   {
      s_email_setting.send_serv_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(OUTGOING_SETTING_T));
      EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr, ret, "out of memory");
      EMA_UTILS_MEMSET(s_email_setting.send_serv_ptr, 0x00, sizeof(OUTGOING_SETTING_T));

      if (!is_new_acc)
      {
         store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
         EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "out of memory");

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_SERVER,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->outgoing_server_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->outgoing_server_ptr, ret, "out of memory");

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_USER_ID,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->user_name_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->user_name_ptr, ret, "out of memory");

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_PASSWORD,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->password_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->password_ptr, ret, "out of memory");
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_PORT_DEFAULT,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.send_serv_ptr->port_type = (uint32)val.pData;
         if (s_email_setting.send_serv_ptr->port_type == EMASP_PORT_CUSTOMIZE)
         {
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_SERVER_PORT,&val);
            EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
            
            s_email_setting.send_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
            EMA_UTILS_BREAKNULL(s_email_setting.send_serv_ptr->port_ptr, ret, "out of memory");
         }
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_SSL_OR_TLS,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.send_serv_ptr->secruity = (uint32)val.pData;
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SMTP_AUTHENTICATE,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_RECEIVE_SERVER_TYPE failed");
         
         s_email_setting.send_serv_ptr->authenticate = (uint32)val.pData;    
        
      }
      
   } while (0);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetOutgoingSettingListParam(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_new_acc)
{
    MMI_STRING_T           item1_str    ={0};
    MMI_STRING_T           item2_str    ={0};
    wchar               temp_arr[1]= {0};
    uint16              current_index = 0;
    uint16              top_index = 0;
    uint32              i = 0;

    current_index = GUILIST_GetCurItemIndex(ctrl_id);
    top_index = GUILIST_GetTopItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);
      
    MMIRES_GetText(TXT_EMAIL_SMTPSERVERNAME, PNULL, &item1_str);
    if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
    {
        item2_str.wstr_ptr = s_email_setting.send_serv_ptr->outgoing_server_ptr;
        item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr);
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterOutGoingSettingServerNameWin);
    }
    else
    {
        item2_str.wstr_ptr = temp_arr;
        item2_str.wstr_len = 0;
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterOutGoingSettingServerNameWin);
    }
    i++;
    
      MMIRES_GetText(TXT_COMMON_USERNAME, PNULL, &item1_str);
      if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->user_name_ptr) > 0)
      {
         item2_str.wstr_ptr = s_email_setting.send_serv_ptr->user_name_ptr;
         item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->user_name_ptr);
         MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterOutGoingSettingUserNameWin);
      }
    else
    {
        item2_str.wstr_ptr = temp_arr;
        item2_str.wstr_len = 0;
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterOutGoingSettingUserNameWin);
    }

    i++;
      MMIRES_GetText(TXT_COMMON_PASSWORD, PNULL, &item1_str);
      if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->password_ptr) > 0)
      {
         item2_str.wstr_ptr = (wchar*)SETTING_PASSWORD_SHOW_STRING;
         item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->password_ptr);
         MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterOutGoingSettingPasswordWin);
      }
    else
    {
        item2_str.wstr_ptr = temp_arr;
        item2_str.wstr_len = 0;
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterOutGoingSettingPasswordWin);
    }

    i++;
      MMIRES_GetText(TXT_COMM_PORT, PNULL, &item1_str);
      if (EMASP_PORT_DEFAULT == s_email_setting.send_serv_ptr->port_type)
      {
         MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &item2_str);
         MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterOutGoingSettingPortWin);
      }
      else if(EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->port_ptr) > 0)
      {
         item2_str.wstr_ptr = s_email_setting.send_serv_ptr->port_ptr;
         item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->port_ptr);
         MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterOutGoingSettingPortWin);
      }
    else
    {
        item2_str.wstr_ptr = temp_arr;
        item2_str.wstr_len = 0;
        MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterOutGoingSettingPortWin);
    }

    i++;
    MMIRES_GetText(TXT_COMM_SECURITY, PNULL, &item1_str);
    MMI_GetLabelTextByLang(TXT_EMAIL_SSL_TLS + s_email_setting.send_serv_ptr->secruity, &item2_str);
    MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterOutGoingSettingSecurityWin);

    i++;
    MMIRES_GetText(TXT_EMAIL_AUTHENTICATION, PNULL, &item1_str);
    MMI_GetLabelTextByLang(TXT_EMAIL_NOTHING + s_email_setting.send_serv_ptr->authenticate, &item2_str);
    MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterOutGoingSettingAuthernticateWin);

    //if (EMASP_SECURITY_OFF == s_email_setting.send_serv_ptr->secruity)
   // {
   //     GUILIST_SetItemGray(ctrl_id,i,TRUE);
   // }
    
    GUILIST_SetCurItemIndex(ctrl_id, current_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
}

/*****************************************************************************/
// 	Description: handle Outcoming Setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutgoingSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
   MMI_CTRL_ID_T  ctrl_id = EMAIL_OUTGOING_SETTING_LIST_CTRL_ID;
   int32          result = 0;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      //SetRecvDropDownList();
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         InitOutgoingSettingParam(TRUE);
#if defined(MMI_PDA_SUPPORT) 
         if (0 == EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr))
         {
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
         }
#else
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_FINISH, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
         }
#endif
      }
      else
      {
#if defined(MMI_PDA_SUPPORT) 
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_RETURN, TXT_NULL, FALSE);
#endif
         InitOutgoingSettingParam(FALSE);
      }
#if defined(MMI_PDA_SUPPORT) 
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
    GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE);
      MMK_SetAtvCtrl(win_id, ctrl_id);
      break;
   case MSG_GET_FOCUS:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, FALSE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, STXT_FINISH, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
         else
         {
#if defined(MMI_PDA_SUPPORT) 
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_IKB_LAST_STEP, FALSE);
#endif
         }
      }
      break;

    case MSG_FULL_PAINT:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         SetOutgoingSettingListParam(ctrl_id,TRUE);
      }
      else
      {
         SetOutgoingSettingListParam(ctrl_id,FALSE);
      }
        break;

   case MSG_CTL_OK:
   case MSG_APP_OK:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
      {
         if (EMA_UTILS_WSTRLEN(s_email_setting.send_serv_ptr->outgoing_server_ptr) > 0)
         {            
            result = CreateEmailAccount(s_email_setting.load_ptr->mail_address_ptr);  
            if (EME_NOSTORAGE == result)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
                break;
            }
            else if (EMAIL_SUCCESS != result)
            {
                MMIPUB_OpenAlertFailWin(TXT_EMAIL_CREATE_NEW_ACC_FAILED);   
                break;
            }

            MMK_CloseWin(EMAIL_SELECT_EMAIL_ACC_WIN_ID);
            MMK_CloseWin(EMAIL_LOAD_SETTING_WIN_ID);
            MMK_CloseWin(EMAIL_DATA_CONNECTION_WIN_ID);
            MMK_CloseWin(EMAIL_INCOMING_SETTING_WIN_ID);
            if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID) && !MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID)
                && !MMK_IsOpenWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID))
            {
                int i= 0;
                MMK_CloseWin(win_id);
                result = MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)g_email_opt_ptr);
                MMIEMAIL_FreeOptPtr();
            }
            else
            {
                MMK_CloseWin(win_id);
                if (!MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
                {
                    MMK_CreateWin((uint32*)MMIEMAIL_SETTING_WIN_TAB, PNULL);   
                }
                else
                {
                    MMK_SendMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                }       
                MMK_CloseWin(EMAIL_ENTRY_EMAIL_TIP_WIN_ID);
            }
         }
         break;
      }
#if defined(MMI_PDA_SUPPORT) 
        break;
#endif

   case MSG_APP_WEB://lint !e616 !e825
   case MSG_CTL_MIDSK://lint !e616 !e825
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
        {
            uint32              index = 0;
            SETTING_ITEM_FUNC   item_func = PNULL;

            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, (uint32*)(&item_func));

            if (PNULL != item_func)
                {
                    (*item_func)();
                }
        }
      break;   

   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->outgoing_server_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->user_name_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->password_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr->port_ptr);
      EMA_UTILS_FREEIF(PNULL, s_email_setting.send_serv_ptr);
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}                                                

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterPersonalizedSettingMailNameWin(void)
{
     s_email_setting.edit_type = SETTING_EDIT_TYPE_SMTP_MAILNAME;
     MMK_CreatePubEditWin((uint32 *)MMIEMAIL_SETTING_EDIT_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterPersonalizedSettingSaveSentWin(void)
{
     MMK_CreatePubListWin((uint32 *)MMIEMAIL_SAVE_SENT_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterPersonalizedSettingDownloadNumberWin(void)
{
     MMK_CreatePubListWin((uint32 *)MMIEMAIL_DOWNLOAD_NUMBER_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterPersonalizedSettingReceiveLimitWin(void)
{
     MMK_CreatePubListWin((uint32*)MMIEMAIL_RECEIVE_LIMIT_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterPersonalizedSettingReceiveNewWin(void)
{
     MMK_CreatePubListWin((uint32*)MMIEMAIL_RECEIVE_NEW_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EnterPersonalizedSettingReplyMailWin(void)
{
     MMK_CreatePubListWin((uint32*)MMIEMAIL_REPLY_MAIL_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void InitPersonalizedSettingParam(BOOLEAN is_new_acc)
{
   HStoreMgr              store_mgr_ptr = PNULL;
   EMASP_PRFITEM_VAL      val = {0};
   int32                  ret = SUCCESS;
   
   do 
   {
      s_email_setting.personalized_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(PERSONALIZED_SETTING_T));
      EMA_UTILS_BREAKNULL(s_email_setting.personalized_ptr, ret, "out of memory");
      EMA_UTILS_MEMSET(s_email_setting.personalized_ptr, 0x00, sizeof(PERSONALIZED_SETTING_T));
      
      if (!is_new_acc)
      {
         store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
         EMA_UTILS_BREAKNULL(store_mgr_ptr, ret, "store_mgr_ptr is NULL");
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_ACCOUNT_NAME,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.personalized_ptr->acc_name_ptr = EMA_UTILS_WSTRDUP(PNULL, val.pData);
         EMA_UTILS_BREAKNULL(s_email_setting.personalized_ptr->acc_name_ptr, ret, "out of memory");
         
         
         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_SAVE_SENT,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_SERVER failed");
         
         s_email_setting.personalized_ptr->save_sent = (uint32)val.pData;
         
//          ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_RECEIVE_FREQUENCY,&val);
//          EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
//          
//          s_email_setting.personalized_ptr->receive_frequency = (uint32)val.pData;

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_NUMBER_LIMIT,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.personalized_ptr->download_num = (uint32)val.pData;

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.personalized_ptr->receive_limit = (uint32)val.pData;


         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_POP3_PORT_DEFAULT failed");
         
         s_email_setting.personalized_ptr->receive_new = (uint32)val.pData;

         ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,s_email_setting.act_id,EMASP_PRFITEM_INCLUDE_ORIGINAL,&val);
         EMA_UTILS_BREAKIF(ret, "get EMASP_PRFITEM_INCLUDE_ORIGINAL failed");
         
         s_email_setting.personalized_ptr->reply_mail = (uint32)val.pData;
      }
      else
      {
         s_email_setting.personalized_ptr->receive_limit = EMA_DOWNLOAD_ONLY_HEADERS;
         s_email_setting.personalized_ptr->receive_new = TRUE;
         s_email_setting.personalized_ptr->reply_mail = TRUE;
      }
      
   } while (0);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void SetPersonalizedSettingListParam(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_new_acc)
{
    MMI_STRING_T           item1_str    ={0};
    MMI_STRING_T           item2_str    ={0};
    wchar               temp_arr[1]= {0};
    GUILIST_POS_INFO_T pos_info = {0};

    GUILIST_GetCurPosInfo(ctrl_id, &pos_info);
    GUILIST_RemoveAllItems(ctrl_id);

    MMIRES_GetText(TXT_EMAIL_ACCOUNT_NAME, PNULL, &item1_str);
    if (EMA_UTILS_WSTRLEN(s_email_setting.personalized_ptr->acc_name_ptr) > 0)
    {
        item2_str.wstr_ptr = s_email_setting.personalized_ptr->acc_name_ptr;
        item2_str.wstr_len = EMA_UTILS_WSTRLEN(s_email_setting.personalized_ptr->acc_name_ptr);
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterPersonalizedSettingMailNameWin);
    }
    else
    {
        item2_str.wstr_ptr = temp_arr;
        item2_str.wstr_len = 0;
        MMIEMAIL_AppendTwoTextItem(ctrl_id,&item1_str,&item2_str,(uint32)EnterPersonalizedSettingMailNameWin);
    }
    
      MMIRES_GetText(TXT_EMAIL_SAVESENT, PNULL, &item1_str);
      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      MMI_GetLabelTextByLang(TXT_EMAIL_YES + (~s_email_setting.personalized_ptr->save_sent & 0x00000001), &item2_str);
      MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterPersonalizedSettingSaveSentWin);

      MMIRES_GetText(TXT_EMAIL_ONCE_DOWNLOAD_NUMBER, PNULL, &item1_str);
      MMI_GetLabelTextByLang(TXT_EMAIL_MAIL_NUM_25 + s_email_setting.personalized_ptr->download_num, &item2_str);
      MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterPersonalizedSettingDownloadNumberWin);

      MMIRES_GetText(TXT_EMAIL_RECEIVE_LIMIT, PNULL, &item1_str);
      MMI_GetLabelTextByLang(TXT_EMAIL_ONLYHEADER + (s_email_setting.personalized_ptr->receive_limit - 1), &item2_str);
      MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterPersonalizedSettingReceiveLimitWin);

      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      MMIRES_GetText(TXT_EMAIL_RECEIVE_ONLY_NEW_MAIL, PNULL, &item1_str);
      MMI_GetLabelTextByLang(TXT_EMAIL_RECV_FROM_NEW + (~s_email_setting.personalized_ptr->receive_new & 0x00000001), &item2_str);
      MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterPersonalizedSettingReceiveNewWin);

      /*ture is 1, no is 0, but the item order is "YES", "NO", so must Negated*/
      MMIRES_GetText(TXT_EMAIL_REPLY_MAIL, PNULL, &item1_str);
      MMI_GetLabelTextByLang(TXT_EMAIL_INCLUDE_ORIG_BODY + (~s_email_setting.personalized_ptr->reply_mail & 0x00000001), &item2_str);
      MMIEMAIL_AppendTwoTextOneImageItem(ctrl_id,&item1_str,&item2_str,IMAGE_COMMON_RIGHT_ARROW,(uint32)EnterPersonalizedSettingReplyMailWin);

      GUILIST_SetCurPosInfo(ctrl_id, &pos_info);
}

/*****************************************************************************/
// 	Description: handle Personalized setting list
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePersonalizedSettingListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
   MMI_RESULT_E err_code                = MMI_RESULT_TRUE;  
   MMI_CTRL_ID_T  ctrl_id = EMAIL_PERSONALIZED_SETTING_LIST_CTRL_ID;
   
   switch(msg_id)
   {
   case MSG_OPEN_WINDOW:
      InitPersonalizedSettingParam(FALSE);
      GUILIST_SetMaxItem(ctrl_id, MMIEMAIL_MAX_SETTING_NUM, FALSE);
      MMK_SetAtvCtrl(win_id, ctrl_id);
      break;

    case MSG_FULL_PAINT:
      if (s_email_setting.enter_type == SETTING_ENTER_TYPE_CUSTOMIZED_ACC || s_email_setting.enter_type == SETTING_ENTER_TYPE_COMMON_ACC)
      {
         SetPersonalizedSettingListParam(ctrl_id,TRUE);
      }
      else
      {
         SetPersonalizedSettingListParam(ctrl_id,FALSE);
      }
      break;
      
   case MSG_CTL_OK:
   case MSG_APP_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                if (s_email_setting.enter_type == SETTING_ENTER_TYPE_EXSIT_ACC)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                return MMI_RESULT_TRUE;
            }
        }
#endif
        {
            uint32              index = 0;
            SETTING_ITEM_FUNC   item_func = PNULL;

            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, (uint32*)(&item_func));

            if (PNULL != item_func)
                {
                    (*item_func)();
                }
        }
      
      break;    
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //todo 关闭窗口
      MMK_CloseWin(win_id);
      break;
   case MSG_CLOSE_WINDOW:
      if (NULL != s_email_setting.personalized_ptr)
      {
          EMA_UTILS_FREEIF(PNULL, s_email_setting.personalized_ptr->acc_name_ptr);
          EMA_UTILS_FREE(PNULL, s_email_setting.personalized_ptr);
      }
      break;
   default :
      err_code = MMI_RESULT_FALSE;
      break;
   } 
   return err_code;
}                                                


#endif  //MMI_GUI_STYLE_TYPICAL

/*****************************************************************************/
// 	Description: is same account as with default_act
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIEMAIL_IsSameAccount(char *default_act, wchar *act_ptr)
{
    char *account_ptr = PNULL;
    BOOLEAN result   = FALSE;

    account_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, act_ptr);

    result =  EMA_UTILS_STRCMP(default_act, account_ptr) == 0 ? TRUE : FALSE;

    SCI_TRACE_LOW("MMIEMAIL: MMIEMAIL_IsSameAccount = %d", result);

    EMA_UTILS_FREE(PNULL, account_ptr);

    return result;
}
/*Edit by script, ignore 34 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
