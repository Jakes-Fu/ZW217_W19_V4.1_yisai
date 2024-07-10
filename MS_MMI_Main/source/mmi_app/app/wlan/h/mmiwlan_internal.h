/*****************************************************************************
** File Name:      mmiwlan_internal.h                                        *
** Author:         Jiaoyou.wu                                                *
** Date:           2010.11.30                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010.11.30     Jiaoyou.wu            Create                               *
******************************************************************************/
#ifndef  _MMIWLAN_INTERNAL_H_    
#define  _MMIWLAN_INTERNAL_H_

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

#define MMIWIFI_USER_NAME_MAX_LEN                (15)
#define MMIWIFI_PASSWORD_MAX_LEN                 (10)
#define MMIWIFI_TSTRINGISNULL(string)            (PNULL == (string).wstr_ptr || 0 == (string).wstr_len)
#define PROXY_PASSWORD_LEN                      (10)
#define PROXY_PORT_LEN                          (5)
#define PROXY_NAME_LEN                          (10)
#define MMIWIFI_URL_MAX_LEN                 (128)
#define PROXY_PORT_MAX_LEN                  (MMIWIFI_URL_MAX_LEN)
#define MMIWIFI_MAX_REJECTLIST_NUM  (10)
#define MMIWIFI_MAX_LIST_NUM                20
#define MMIWIFI_MAX_TRUSTED_LIST_NUM        (MMIWIFI_MAX_PROFILE_NUM - MMIWIFI_MAX_LIST_NUM)

#define MMIWLAN_PORTAL_USER_NAME_MAX_LEN                (64)
#define MMIWLAN_PORTAL_PASSWORD_MAX_LEN                 (20)
#define MMIWLAN_PORTAL_SMS_CONTENT_MAX_LEN                 (60)

#define MMIWIFI_CACL_PACKET_TIME_RANGE      30   //µ•Œª∑÷÷”
#define MMIWIFI_CACL_PACKET_MAX_NUM         1000

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum MMI_AUTO_CONNECTING_CHOICE_E_
{
    MMI_AUTO_CONNECTING_CHOICE_ON,
    MMI_AUTO_CONNECTING_CHOICE_OFF,
    
    MMI_AUTO_CONNECTING_CHOICE_MAX,
}MMI_AUTO_CONNECTING_CHOICE_E;

typedef enum MMI_GPRSTOWLAN_RESELECT_E_
{
    MMI_GPRSTOWLAN_ALWAYS_INQUIRY,
    MMI_GPRSTOWLAN_AUTO_RESELCET,
    MMI_GPRSTOWLAN_MANUAL_RESELCET   
}MMI_GPRSTOWLAN_RESELECT;

typedef enum MMI_AP_RESELECT_E_
{
    MMI_AP_AUTO_RESELCET,
    MMI_AP_MANUAL_RESELCET   
}MMI_AP_RESELECT;

typedef enum MMI_NOWLAN_PROMPT_E_
{
    MMI_NOWLAN_SHOW_PROMPT,
    MMI_NOWLAN_NOT_SHOW_PROMPT,
}MMI_NOWLAN_PROMPT;

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct MMIWIFI_ACCOUNT_INFO_T_
{
    wchar user_name[MMIWIFI_USER_NAME_MAX_LEN + 1];
    wchar user_psw[MMIWIFI_PASSWORD_MAX_LEN + 1];
}MMIWIFI_ACCOUNT_INFO_T;

typedef struct MMIWIFI_PROXY_PASSWORD_E_
{
    wchar proxy_password[PROXY_PASSWORD_LEN + 1];
}MMIWIFI_PROXY_PASSWORD_E;

typedef struct MMIWIFI_PROXY_NAME_E_
{
    wchar proxy_name[PROXY_NAME_LEN + 1];
}MMIWIFI_PROXY_NAME_E;

typedef struct MMIWIFI_PROXY_REJECTLIST_E_
{
    char proxy_rejectlist[MMIWIFI_MAX_REJECTLIST_NUM][MMIWIFI_URL_MAX_LEN + 1];
}MMIWIFI_PROXY_REJECTLIST_E;

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
