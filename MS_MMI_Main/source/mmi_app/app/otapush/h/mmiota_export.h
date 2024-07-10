/*****************************************************************************
** File Name:      mmiota_export.h                                                *
** Author:         minghu.mao                                                          *
** Date:           12/2008                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe PUSH OTA protocol tables              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008       minghu.mao         Create 
** 12/2008       minghu.mao        Modify
******************************************************************************/

#ifndef _MMIOTA_API_H_    
#define _MMIOTA_API_H_   


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmisms_export.h"
#if defined(OMA_CP_SEC_SUPPORT)
#include "mmisms_app.h"
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
**                         MOCRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define		MMIOTA_CONFIG_INFO_MAX					MMICMSBRW_MAX_ITEM_NUM
#define		MMIOTA_SETTING_MAX_NAME_LEN		        48		               //名称最大长度    
#define		MMIOTA_SETTING_MAX_LINKMODENAME_LEN		28			           //连接方式设定名的最大长度   
#define		MMIOTA_SETTING_MAX_GATEWAY_LEN			21	                   //网关地址的最大长度
#define     MMIOTA_SETTING_MAX_DNS_LEN              15
#define		MMIOTA_SETTING_MAX_USERNAME_LEN		    32                     //用户名的最大长度
#define		MMIOTA_SETTING_MAX_PASSWORD_LEN		    20                     //密码的最大长度
#define		MMIOTA_SETTING_MAX_APN_LEN				MN_GPRS_MAX_APN_LEN    //接入点的最大长度102
#define		MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN	    512		               //URL的最大长度,参考值512

#define     MMIOTA_WAP_SI_NAME_MAX_LEN       MMIOTA_SETTING_MAX_NAME_LEN               //WAP PUSH名称最长长度
#define     MMIOTA_WAP_SI_HERF_MAX_LEN       MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN       //WAP PUSH URL最长长度
#define     MMIOTA_WAP_SI_CONTENT_MAX_LEN    100

#define     MMIOTA_BOOKMARK_NAME_MAX_LEN     MMIOTA_SETTING_MAX_NAME_LEN               //书签名称最长长度
#define     MMIOTA_BOOKMARK_URL_MAX_LEN      MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN       //书签URL最长长度
    
/**--------------------------------------------------------------------------*
**                         STRUCT DEFINITION                                 *
**--------------------------------------------------------------------------*/
typedef enum{                                  //Setting type define
    MMIOTA_MMS_SETTING = 0,                    //MMS setting
    MMIOTA_BRW_SETTING,                        //wap setting
    MMIOTA_BRW_BOOKMARK,                       //Bookmark setting
    MMIOTA_BRW_WAP_SI,                         //SI push
    MMIOTA_BRW_WAP_CO,
    MMIOTA_BRW_WAP_SL,
    MMIOTA_SETTING_MAX
}MMIOTA_SETTING_TYPE_E;

typedef enum{                                 
    MMIOTA_SI_SIGNAL_ACTION_NONE = 0,          //no nothing and delete
    MMIOTA_SI_SIGNAL_ACTION_LOW,               //low
    MMIOTA_SI_SIGNAL_ACTION_MEDIUM,            //default normal
    MMIOTA_SI_SIGNAL_ACTION_HIGH,              //high
    MMIOTA_SI_SIGNAL_ACTION_DELETE,            //delete
    MMIOTA_SI_SIGNAL_ACTION_MAX
}MMIOTA_SI_SIGNAL_ACTION_E;

typedef enum{                                 
    MMIOTA_SL_EXECUTE_ACTION_LOW = 0,          //no nothing and delete
    MMIOTA_SL_EXECUTE_ACTION_HIGH,             //low
    MMIOTA_SL_EXECUTE_ACTION_CACHE,            //default normal
    MMIOTA_SL_EXECUTE_ACTION_MAX
}MMIOTA_SL_EXECUTE_ACTION_E;

typedef struct {                             
    uint16              len;               
    wchar               *value; 
}MMIOTA_SETTING_WSTR_VALUE_T;

typedef struct {                             
    uint16              len;               
    uint8               *value;    
}MMIOTA_SETTING_VALUE_T;

typedef struct 
{    
    uint8			    bookmark_name[MMIOTA_BOOKMARK_NAME_MAX_LEN + 1];		 //书签设置的名字
    uint16              bookmark_name_len;
    uint8			    bookmark_url[MMIOTA_BOOKMARK_URL_MAX_LEN + 1];	         //书签URL
    uint16              bookmark_url_len;
}MMIOTA_BOOKMARK_T,*P_MMIOTA_BOOKMARK_T;                                     //书签设置的结构

typedef struct
{    
    uint8 			    si_name[MMIOTA_WAP_SI_NAME_MAX_LEN + 1];	             //si name
    uint16              si_name_len;
    uint8				si_herf[MMIOTA_WAP_SI_HERF_MAX_LEN + 1];	             //si URL
    uint16              si_herf_len;
    uint16              si_content[MMIOTA_WAP_SI_CONTENT_MAX_LEN + 1];           //push内容
    uint16              si_content_len;
    uint8 			    si_id[MMIOTA_WAP_SI_NAME_MAX_LEN + 1];	             //si name
    uint16              si_id_len;
    uint8 			    si_created_time[MMIOTA_WAP_SI_NAME_MAX_LEN + 1];	             //si created time
    uint16              si_created_time_len;
    uint8 			    si_expired_time[MMIOTA_WAP_SI_NAME_MAX_LEN + 1];	             //si expired time
    uint16              si_expired_time_len;
    MMIOTA_SI_SIGNAL_ACTION_E signal_action;
 }MMIOTA_WAP_SI_T,*P_MMIOTA_WAP_SI_T;  

typedef struct
{    
    uint8 			    sl_name[MMIOTA_WAP_SI_NAME_MAX_LEN + 1];	             //si name
    uint16              sl_name_len;
    uint8				sl_herf[MMIOTA_WAP_SI_HERF_MAX_LEN + 1];	             //si URL
    uint16              sl_herf_len;
    uint16              sl_service[MMIOTA_WAP_SI_CONTENT_MAX_LEN + 1];           //push内容
    uint16              sl_service_len;
    MMIOTA_SL_EXECUTE_ACTION_E execute_action;
 }MMIOTA_WAP_SL_T,*P_MMIOTA_WAP_SL_T;

typedef struct
{    
    uint8 			    co_name[MMIOTA_WAP_SI_NAME_MAX_LEN + 1];	             //si name
    uint16              co_name_len;
    uint8				co_herf[MMIOTA_WAP_SI_HERF_MAX_LEN + 1];	             //si URL
    uint16              co_herf_len;
    uint16              co_content[MMIOTA_WAP_SI_CONTENT_MAX_LEN + 1];           //push内容
    uint16              co_content_len;
 }MMIOTA_WAP_CO_T,*P_MMIOTA_WAP_CO_T;    

typedef struct mmiota_setting_tag
{
    MMIOTA_SETTING_TYPE_E  setting_type;                                    //设置类型
    uint8           	   access_type;				                        //0 表示wap1.2, 1表示wap2.0
    MN_PCO_AUTH_TYPE_E auth_type;                                                               //鉴定类型0:PAP，1:CHAP:
    int32      		       port;						                    //端口号
    uint8 		           name[MMIOTA_SETTING_MAX_NAME_LEN + 1];	            //连接设置的名字
    uint16                 name_len;
    uint8			       gateway[MMIOTA_SETTING_MAX_GATEWAY_LEN + 1];	        //网关地址
    uint16                 gateway_len;
    uint8			       dnsaddress[MMIOTA_SETTING_MAX_DNS_LEN + 1];	        //DNS地址
    uint16                 dnsaddress_len;
    uint8			       username[MMIOTA_SETTING_MAX_USERNAME_LEN + 1];	     //用户名
    uint16                 username_len;
    uint8		       	   password[MMIOTA_SETTING_MAX_PASSWORD_LEN + 1];	     //密码
    uint16                 password_len;
    uint8			       apn[MMIOTA_SETTING_MAX_APN_LEN + 1];		             //接入点
    uint16                 apn_len;
    uint8			       homepage[MMIOTA_SETTING_HOMEPAGE_MAX_URL_LEN + 1];	 //主页网址，对于MMS，此为彩信中心
    uint16                 homepage_len;
    MMIOTA_WAP_SI_T        wap_si     ;                                      //si
    MMIOTA_WAP_SL_T        wap_sl     ;                                      //sl
    MMIOTA_WAP_CO_T        wap_co     ;                                      //co
    MMIOTA_BOOKMARK_T      bookmark;                                         //书签
    struct mmiota_setting_tag       *next;
}MMIOTA_SETTING_T,*P_MMIOTA_SETTING_T;                                       //连接设置的结构

#ifdef MMI_OTA_SUPPORT
/**--------------------------------------------------------------------------*
**ota 回调定义                                                               *
**--------------------------------------------------------------------------*/  
typedef BOOLEAN (* MMIOTA_CBF) (P_MMIOTA_SETTING_T  mmi_setting_ptr, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys);

#if defined(OMA_CP_SEC_SUPPORT)
typedef struct{
    MN_DUAL_SYS_E dual_sys;
    BOOLEAN needCheckPin;
    int sec;
    uint8 mac[20*2+1];
    uint8 *wbxml;  
    uint32 wbxml_length;
}OTA_SEC_INFO_T;
typedef struct{
    uint8 *push_data_ptr;
    uint32 push_data_len;
    uint8 send_num[MMISMS_PBNAME_MAX_LEN+1];	
    MMIOTA_CBF provision_setting;	
    OTA_SEC_INFO_T security_info;
}OTA_WIN_DATA_T;
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         MMIAPIOTA_ParseWBXML
* Function:     PUSH WBXML解析主函数
* Author:       minghu.mao
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN  MMIAPIOTA_ParseWBXML(const uint8  *push_data_ptr, uint32 push_data_Len,MMIOTA_CBF provision_setting, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :ota push callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_AddOTAPUSH(MMIOTA_SETTING_T  *mmi_setting_ptr, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :inbox enter ota
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_OTAEnter(uint16 id);

/*****************************************************************************/
//  Description :delete ota push
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_DeleteOTAPUSH(int16 del_index);

/*****************************************************************************/
//  Description :set locked flag
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_SetOTAPUSHLockFlag(uint32 record_id,BOOLEAN is_locked);

/*****************************************************************************/
//  Description :update ota file info
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_UpdateOTAFileInfo(void);

/*****************************************************************************/
//  Description :init ota push
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_InitOTAPUSH(void);

/*****************************************************************************/
//  Description : delete ota by id
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_DelOTAByID(int32 id,int32 (*cbf)(BOOLEAN result,void* param),void *param);

/*****************************************************************************/
//  Description : get ota total number
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPIOTA_GetOTATotalNum(void);

/*****************************************************************************/
//  Description :open ota read win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_EnterconfigInfoWinMsg(void);

/*****************************************************************************/
//  Description :wap push enter
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIOTA_WapPushEnter(uint16 ID);

/*****************************************************************************/
//  Description :load wap push into sms list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_LoadWapPushListToSms(void);

/*****************************************************************************/
//  Description :delete ota push by id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_DelPushByID(uint16 ID,int32 (*cbf)(BOOLEAN result,void* param),void *param);

/*****************************************************************************/
//  Description :get flag lock
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_GetPushLockFlag(uint32 record_id);

/*****************************************************************************/
//  Description :set lock flag
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_SetPushLockFlag(uint32 record_id,BOOLEAN is_locked);

/*****************************************************************************/
//  Description :out save push file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_SavePushFileInfo(void);

/*****************************************************************************/
//  Description :get wap push total num
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIOTA_GetPushTotalNum(void);

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//  Description : set ota security flag
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_MoveSecurityOTA(uint32 record_id,
                                         BOOLEAN is_to_security);

/*****************************************************************************/
//  Description : set push security flag
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_MoveSecurityPush(uint32 record_id,
                                          BOOLEAN is_to_security);
#endif


/*****************************************************************************/
//  Description : is in ota moudle window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_IsInOTAWin(void);

#endif

#ifdef MMI_OTA_SUPPORT
/*****************************************************************************/
//  Description : Decode ota push 
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIOTA_DecodePush(
                                     MN_DUAL_SYS_E dual_sys, 
                                     APP_MMS_USER_DATA_T *sms_ptr,
                                     uint8      *sender_ptr,
                                     uint32     sender_len,
                                     uint8      *sc_sender_ptr,
                                     uint32     sc_sender_len
                                     );
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif

