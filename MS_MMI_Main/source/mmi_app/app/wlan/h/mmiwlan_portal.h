/*****************************************************************************
** File Name:      mmiwlan_portal.h                                        *
** Author:         wancan.you                                               *
** Date:           2011.03.15                                               *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011.03.15     wancan.you            Create                                    *
******************************************************************************/
#ifndef  _MMIWLAN_PORTAL_H_
#define  _MMIWLAN_PORTAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWLAN_PORTAL_UPDATE_FILE_SIZE_MAX         (1024)
#define MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX     (20)
#define MMIWIFI_PORTAL_COMMAND_LEN_MAX  (20)
#define MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX  (40)
#define MMIWIFI_PORTAL_VERSION_LEN_MAX  (20)


#define MMIWIFI_TAG_VERSION    "Version"
#define MMIWIFI_TAG_SERVICE_PROVIDER    "Serviceprovider"
#define MMIWIFI_TAG_ACTIVE    "Active"
#define MMIWIFI_TAG_MODITY_PASSWORD    "Modifypassword"
#define MMIWIFI_TAG_RESET_PASSWORD    "Resetpassword"
#define MMIWIFI_TAG_PORTAL_MYFORM    "Myform"
#define MMIWIFI_TAG_LOGINFORM    "Loginform"



#define MMIWIFI_PORTAL_VERSION_INFO    "1.0.0"
#define MMIWIFI_PORTAL_SERVICE_PROVIDER_CHINA_MOBILE    "10086"
#define MMIWIFI_PORTAL_COMMAND_ACTIVE_STRING    "KTWLAN"
#define MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD_STRING    "CZWLANMM"
#define MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD_STRING    "XGWLANMM"
#define MMIWIFI_PORTAL_MYFORM    "NAME=\"myForm\""
#define MMIWIFI_PORTAL_LOGINFORM    "NAME=\"loginform\""

#define MMIWIFI_PORTAL_UPDATE_OK            (0)
#define MMIWIFI_PORTAL_UPDATE_ERROR_FILE    (-1)
#define MMIWIFI_PORTAL_UPDATE_ERROR_UPDATED (-2)    


/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIWIFI_PORTAL_LOGIN_SUCCESS,//login_res=0
    MMIWIFI_PORTAL_LOGIN_USER_ERROR,//login_res=1
    MMIWIFI_PORTAL_LOGIN_USER_ABNORMAL_STATE,//login_res=2
    MMIWIFI_PORTAL_LOGIN_PWD_ERROR,//login_res=3
    MMIWIFI_PORTAL_LOGIN_IP_NOT_MATCH,//login_res=7
    MMIWIFI_PORTAL_LOGIN_AC_NOT_MATCH,//login_res=8
    MMIWIFI_PORTAL_LOGIN_PORTAL_REJECT,//login_res=15
    MMIWIFI_PORTAL_LOGIN_PORTALING,//login_res=17
    MMIWIFI_PORTAL_LOGIN_USER_CONFLICT,//login_res=26
    MMIWIFI_PORTAL_LOGIN_UNPROTAL_EXPIRED,//login_res=40
    MMIWIFI_PORTAL_LOGIN_FAILED,//other
    MMIWIFI_PORTAL_LOGIN_MAX
}MMIWIFI_PORTAL_LOGIN_RES_E;

typedef enum
{
    MMIWIFI_PORTAL_OFFLINE_SUCCESS,//offline_res=0
    MMIWIFI_PORTAL_OFFLINE_IP_NOT_MATCH,//offline_res=7
    MMIWIFI_PORTAL_OFFLINE_AC_NOT_MATCH,//offline_res=9
    MMIWIFI_PORTAL_OFFLINE_AUTO_OFFLINE,//offline_res=11
    MMIWIFI_PORTAL_OFFLINE_REJECT,//offline_res=18
    MMIWIFI_PORTAL_OFFLINE_FAILED,//other
    MMIWIFI_PORTAL_OFFLINE_MAX
}MMIWIFI_PORTAL_OFFLINE_RES_E;

typedef enum
{
    MMIWLAN_PORTAL_STATUS_DISCONNECTED,
    MMIWLAN_PORTAL_STATUS_OPENING,
    MMIWLAN_PORTAL_STATUS_OPENED,
    MMIWLAN_PORTAL_STATUS_SCANING,
    MMIWLAN_PORTAL_STATUS_SCANED,
    MMIWLAN_PORTAL_STATUS_CONNECTING,
    MMIWLAN_PORTAL_STATUS_CONNECTED,
    MMIWLAN_PORTAL_STATUS_DISCONNECTING,
    MMIWLAN_PORTAL_STATUS_PORTALING,
    MMIWLAN_PORTAL_STATUS_PORTALED,
    MMIWLAN_PORTAL_STATUS_PORTAL_FAILED,
    MMIWLAN_PORTAL_STATUS_LOGOUTING,
    MMIWLAN_PORTAL_STATUS_MAX
}MMIWLAN_PORTAL_STATUS_E;

typedef enum
{
    MMIWLAN_PORTAL_WIN_LOGIN,
    MMIWLAN_PORTAL_WIN_SETTINGS,
    MMIWLAN_PORTAL_WIN_STATUS,

    MMIWLAN_PORTAL_WIN_MAX
}MMIWLAN_PORTAL_WIN_E;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
typedef enum
{
    MMI_PORTAL_SMS_ACTIVE,
    MMI_PORTAL_SMS_RESET_PASSWORD,
    MMI_PORTAL_SMS_MODIFY_PASSWORD,
    MMI_PORTAL_SMS_MAX,
}MMI_PORTAL_TYPE_E;
#endif

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct MMIWIFI_PORTAL_CONFIG_T_
{
    wchar user_name[MMIWLAN_PORTAL_USER_NAME_MAX_LEN + 1];
    uint16 user_name_len;
    wchar password[MMIWLAN_PORTAL_PASSWORD_MAX_LEN + 1];
    uint16 password_len;
    BOOLEAN is_remember_password;
    BOOLEAN is_auto_login;    
    BOOLEAN is_notify;    
}MMIWIFI_PORTAL_CONFIG_T;

typedef struct MMIWIFI_PORTAL_CONTEXT_T_
{
    uint8 status_timer_id;
    uint32 status_time;
    MMIWLAN_PORTAL_STATUS_E portal_status;
    MMIWLAN_PORTAL_WIN_E win_type;
    wchar  sms_content_wchar[MMIWLAN_PORTAL_SMS_CONTENT_MAX_LEN + 1];
}MMIWIFI_PORTAL_CONTEXT_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : Open Portal Status Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalStatusWin(void);

/****************************************************************************/
//  Description : Start Portal Status Timer
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_StartPortalStatusTimer(void);

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_WlanPortalRequestCancel(void);

/****************************************************************************/
//  Description : Open Portal Help Win
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalHelpWin(MMI_TEXT_ID_T text_id, MMI_TEXT_ID_T title_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_Update(void);

/*****************************************************************************/
//  Description : Net ConnectEx
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_WlanPortalNetConnect(void);

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIFI_HandleHttpMsg(uint16 msg_id, DPARAM param);

/****************************************************************************/
//  Description : Handle Portal Login
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_HandlePortalLogin(MMIWIFI_PORTAL_LOGIN_RES_E login_res);

/****************************************************************************/
//  Description : Handle Portal Offline
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_HandlePortalOffline(MMIWIFI_PORTAL_OFFLINE_RES_E offline_res);

/*****************************************************************************/
//  Description : Portal Offline
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_PortalOffline(void);

/****************************************************************************/
//  Description : get portal status
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC MMIWLAN_PORTAL_STATUS_E MMIWIFI_GetPortalStatus(void);

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_WlanPortalNetDisconnect(void);

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_WlanPortalRequestClose(void);

/****************************************************************************/
//  Description : Notify Portal Failed
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_NotifyPortalFailed(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : Get Portal Config
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetPortalConfig(MMIWIFI_PORTAL_CONFIG_T *portal_cfg_ptr);

/*****************************************************************************/
//  Description : Set Portal Config
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetPortalConfig(MMIWIFI_PORTAL_CONFIG_T *portal_cfg_ptr);

/****************************************************************************/
//  Description : Open Portal Login Win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalLoginWin(void);

/****************************************************************************/
//  Description : Open Portal Settings win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_OpenPortalSettingsWin(void);

/****************************************************************************/
//  Description : Open Portal Status Win
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_OpenPortalStatusWin(void);

/****************************************************************************/
//  Description : Auto Login CMCC
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_CMCCAutoLogin(void);

/****************************************************************************/
//  Description : Log off
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_CMCCLogoff(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
