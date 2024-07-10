/*****************************************************************************
** File Name:      mmiconnection_export.h                                            *
** Author:         ryan.xu
** Date:           2007/2/1                                            *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe CONNECTION *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/2/1       ryan.xu	       Create
******************************************************************************/

#ifndef _MMICONNECTION_H_
#define _MMICONNECTION_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"
#include "mmi_signal_ext.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/		
#ifdef LOW_MEMORY_SUPPORT_32X16
#define MMICONNECTION_MAX_SETTING_NUM			    3              //Low Memory���õ��������3
#else
#define MMICONNECTION_MAX_SETTING_NUM			    8             //���õ��������8
#endif
#define MMICONNECTION_MAX_LINKMODENAME_LEN		    28			//���ӷ�ʽ�趨������󳤶�2008.1.10 modify
#ifdef MMI_HTTP_PROXY_IP_ONLY_SUPPORT
#define MMICONNECTION_MAX_GATEWAY_LEN			    15		    //���ص�ַ����󳤶�
#else
#ifdef LOW_MEMORY_SUPPORT
#define MMICONNECTION_MAX_GATEWAY_LEN			    64		    //���ص�ַ����󳤶�
#else
#define MMICONNECTION_MAX_GATEWAY_LEN			    80		    //���ص�ַ����󳤶�(�ο����ܻ�Ϊ80)
#endif
#endif
#define MMICONNECTION_MAX_USERNAME_LEN		            30          //�û�������󳤶�
#define MMICONNECTION_MAX_PASSWORD_LEN		            30          //�������󳤶�
#define MMICONNECTION_MAX_APN_LEN				     30        //��������󳤶�102
#ifdef LOW_MEMORY_SUPPORT
#define MMICONNECTION_HOMEPAGE_MAX_URL_LEN		     128			//3216URL����󳤶�,�ο�ֵ128
#else
#define MMICONNECTION_HOMEPAGE_MAX_URL_LEN		     512			//URL����󳤶�,�ο�ֵ128
#endif
#define MMICONNECTION_MAX_DNS_LEN                               15               //��ѡdns�ִ��ĳ���
#define MMICONNECTION_MAX_PORT_LEN			            5
#define MMICONNECTION_MAX_PORT_VALUE			     0XFFFF

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMICONNECTION_NOTIFY_EVENT_DEL_ITEM,
    MMICONNECTION_NOTIFY_EVENT_EDIT_ITEM,
    MMICONNECTION_NOTIFY_EVENT_MAX
}MMICONNECTION_NOTIFY_EVENT_E;

typedef enum
{
    MMICONNECTION_ACCOUNT_BROWSER,	
    MMICONNECTION_ACCOUNT_MMS,
    MMICONNECTION_ACCOUNT_JAVA,
    MMICONNECTION_ACCOUNT_DCD,
    MMICONNECTION_ACCOUNT_CBS,
    MMICONNECTION_ACCOUNT_IMS,
    MMICONNECTION_ACCOUNT_IA,
    MMICONNECTION_ACCOUNT_SUPL,
    MMICONNECTION_ACCOUNT_XCAP,
    MMICONNECTION_ACCOUNT_SOS,
    MMICONNECTION_ACCOUNT_MAX
}MMICONNECTION_ACCOUNT_TYPE_E;

typedef enum
{
    MMICONNECTION_MVNOTYPE_NONE,
    MMICONNECTION_MVNOTYPE_SPN,
    MMICONNECTION_MVNOTYPE_GID,
	MMICONNECTION_MVNOTYPE_PNN,
    MMICONNECTION_MVNOTYPE_IMSI,
	MMICONNECTION_MVNOTYPE_IMSIR,
	MMICONNECTION_MVNOTYPE_MAX
}MMICONNECTION_MVNO_TYPE_E;

#ifdef IPVERSION_SUPPORT_V4_V6
typedef enum
{
    MMICONNECTION_IP_V4,
    MMICONNECTION_IP_V6,
    MMICONNECTION_IP_V4V6,
    MMICONNECTION_IP_MAX
}MMICONNECTION_IP_TYPE_E;
#endif

typedef enum
{
    MMI_PDP_CTX_TYPE_IMS,
    MMI_PDP_CTX_TYPE_IA,
    MMI_PDP_CTX_TYPE_SOS,
    MMI_PDP_CTX_TYPE_NUM,
    MMI_PDP_CTX_TYPE_INVALID = 0xFFFFFFFF
}MMI_INTERNAL_PDP_CTX_TYPE;

typedef enum
{
    MMICONNECTION_ACCESS_EDITABLE,	
    MMICONNECTION_ACCESS_READONLY,
    MMICONNECTION_ACCESS_MAX,
}MMICONNECTION_ACCESS_OPTION_E;

typedef struct
{
    uint8 access_type;								//0 ��ʾwap1.2, 1��ʾwap2.0
    MN_PCO_AUTH_TYPE_E auth_type;                                                               //��������0:PAP��1:CHAP:
    uint16 port;										//�˿ں�
    wchar name[MMICONNECTION_MAX_LINKMODENAME_LEN + 1];		//�������õ�����
    uint8 name_len;									//�������õ����ֳ���
    uint8 gateway[MMICONNECTION_MAX_GATEWAY_LEN + 1];		//���ص�ַ
    uint8 gateway_len;								//���ص�ַ����
    uint8 username[MMICONNECTION_MAX_USERNAME_LEN + 1];	//�û���
    uint8 username_len;								//�û�������
    uint8 password[MMICONNECTION_MAX_PASSWORD_LEN + 1];	//����
    uint8 password_len;								//���볤��
    uint8 apn[MMICONNECTION_MAX_APN_LEN + 1];				//�����
    uint8 apn_len;									//����㳤��
    uint8 homepage[MMICONNECTION_HOMEPAGE_MAX_URL_LEN + 1];	//��ҳ��ַ������MMS����Ϊ��������
    uint16 homepage_len;								//��ҳ��ַ����
    uint8 dns[MMICONNECTION_MAX_DNS_LEN + 1];
    uint8 dns_len;									//����㳤��
    BOOLEAN is_customed_detail;                         //�Ƿ��ƶ�������
    BOOLEAN priority;                                   //��Ŀ���Ƿ���и����ȼ�TRUE:high;FALSE:low
    BOOLEAN is_default_unchange;                                                                  //�Ƿ���Ĭ��δ�޸���Ŀ
    uint16 list_index;//��Ĭ���б��е�λ��
    uint16 hplmn;                                                                  //hpmn
    MN_PLMN_T plmn;									//plmn for autoadapting
    MMICONNECTION_ACCOUNT_TYPE_E account_type;      //account_type
    uint16 uiorder;
    uint8 spn[MMICONNECTION_HOMEPAGE_MAX_URL_LEN + 1];
    uint16 spn_len;		  
#ifdef IPVERSION_SUPPORT_V4_V6
    MMICONNECTION_IP_TYPE_E ip_type;   //ip type
#endif
    BOOLEAN visible;
}MMICONNECTION_LINKSETTING_DETAIL_T;//�������õĽṹ


typedef struct
{
    uint16 mcc;                                                                   //PLMN MCC
    uint16 mnc;                                                                   //PLMN MNC
    uint16 port;										//�˿�
    uint8 access_type;								//0 ��ʾwap1.2, 1��ʾwap2.0
    MN_PCO_AUTH_TYPE_E auth_type;                                                               //��������0:PAP��1:CHAP:
    MMICONNECTION_ACCOUNT_TYPE_E account_type;                //account_type
    uint32 name_index;
    const char *gateway;
    const char *username;
    const char *password;
    const char *apn;
    const char *homepage;
}MMICONNECTION_LINKSETTING_ITEM_T;//�������õĽṹ

typedef struct
{
    uint16 uiorder;
    uint16 mcc;                                                                   //PLMN MCC
    uint16 mnc;                                                                   //PLMN MNC
    uint16 port;										//�˿�
    uint8 access_type;								//0 ��ʾwap1.2, 1��ʾwap2.0
    MMICONNECTION_ACCESS_OPTION_E access_option;                                                //0x00:�ɱ༭��0x01:ֻ��
    MN_PCO_AUTH_TYPE_E auth_type;                                                               //��������0:PAP��1:CHAP:
    MMICONNECTION_ACCOUNT_TYPE_E account_type;                //account_type
    wchar *name;
    uint16 name_len;
    const char *gateway;
    uint16 gateway_len;
    const char *dns1;
    uint16 dns1_len;
    const char *dns2;
    uint16 dns2_len;
    const char *username;
    uint16 username_len;
    const char *password;
    uint16 password_len;
    const char *apn;
    uint16 apn_len;
    const char *homepage;
    uint16 homepage_len;
    const char *mvno_value;
    uint16 mvno_value_len;
#ifdef IPVERSION_SUPPORT_V4_V6
    MMICONNECTION_IP_TYPE_E ip_type;   //ip type
#endif
    MMICONNECTION_MVNO_TYPE_E mvno_type;
    uint8 visible;
    uint8 roaming;
#ifdef IPVERSION_SUPPORT_V4_V6	
    MMICONNECTION_IP_TYPE_E roaming_protocol;
#endif
}MMICONNECTION_RES_ITEM_T;//�������õĽṹ


typedef struct
{
    wchar* name_ptr;
    MMI_TEXT_ID_T name_text_id;
}MMICONNECTION_NAME_T;

typedef struct
{
    MMICONNECTION_RES_ITEM_T *res_item_ptr;
    uint16 res_num;
    BOOLEAN is_res_data;
}MMICONNECTION_RES_INFO_T;

typedef struct
{
    MMICONNECTION_LINKSETTING_DETAIL_T setting_detail[MMI_DUAL_SYS_MAX][MMICONNECTION_MAX_SETTING_NUM];	//�����������õ�����
    uint8 settinglink_num[MMI_DUAL_SYS_MAX];
    uint8 invisible_apn_num[MMI_DUAL_SYS_MAX];
}MMICONNECTION_SETTING_T;//���õ���Ϣ

typedef struct
{
    BOOLEAN is_sim_ready_status;
    BOOLEAN bIsPNNAPNExist;
}MMICONNECTION_PNNAPN_EXIST_T;

typedef struct
{
    uint8 index;
    BOOLEAN is_valid;
}MMICONNECTION_TYPEINDEX_T;
typedef struct
{
    MMICONNECTION_TYPEINDEX_T internet_index[MMI_DUAL_SYS_MAX];
    MMICONNECTION_TYPEINDEX_T xcap_index[MMI_DUAL_SYS_MAX];
    MMICONNECTION_TYPEINDEX_T ims_index[MMI_DUAL_SYS_MAX];
    MMICONNECTION_TYPEINDEX_T ia_index[MMI_DUAL_SYS_MAX];
    MMICONNECTION_TYPEINDEX_T sos_index[MMI_DUAL_SYS_MAX];
}MMICONNECTION_APNTYPEINDEX_T;

typedef struct
{
    MN_DUAL_SYS_E dual_sys;//setting_detail����Ӧ��SIM id
    uint8 item_index;   //setting_detail �е�ǰSIM ����Ӧ��item id
}MMICONNECTION_SETTING_DETAIL_ITEM_T;//���õ���Ϣ

typedef void (*MMICONNECTION_NOTIFY_FUNC)(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys);

typedef struct 
{
    uint32 module_id;
    MMICONNECTION_NOTIFY_FUNC notify_func;
}MMICONNECTION_NOTIFY_INFO_T;

typedef struct 	MMICONNECTION_NOTIFY_LIST_T_tag
{
    MMICONNECTION_NOTIFY_INFO_T notify_info;
    struct MMICONNECTION_NOTIFY_LIST_T_tag *next;
}MMICONNECTION_NOTIFY_LIST_T;

//GPRS Attach mode
typedef enum 
{
    MMICONNECTION_GPRS_ATTACH_ONNEED,
    MMICONNECTION_GPRS_ALWAYS,
#ifdef MMI_DUALMODE_ENABLE
    MMICONNECTION_GPRS_ALWAYS_LC,//low cost(energy saving)
#endif
#ifdef MMI_GPRS_SWITCH_SUPPORT
    MMICONNECTION_GPRS_TURNED_OFF,
#endif
    MMICONNECTION_GPRS_SETTINGS_MAX
}MMICONNECTION_GPRS_SETTINGS_E;

#ifdef MMI_GPRS_SWITCH_SUPPORT
typedef enum 
{
    MMICONNECTION_SETTING_GPRS_SWITCH_ON,
    MMICONNECTION_SETTING_GPRS_SWITCH_OFF,
    MMICONNECTION_SETTING_GPRS_SWITCH_MAX
}MMICONNECTION_SETTING_GPRS_SWITCH_E;
#endif   


#ifdef MMI_GPRS_SUPPORT
typedef void (*MMICONNECTION_CALLBACK_FUNC)(MN_DUAL_SYS_E dual_sys, uint32 index);
#endif

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
typedef enum _MMICONNECTION_SETTING_DATA_ROAMING_E
{
    MMICONNECTION_SETTING_DATA_ROAMING_ENABLE,
    MMICONNECTION_SETTING_DATA_ROAMING_DOMESTIC_ENABLE = MMICONNECTION_SETTING_DATA_ROAMING_ENABLE,
    MMICONNECTION_SETTING_DATA_ROAMING_DISABLE,
    MMICONNECTION_SETTING_DATA_ROAMING_MAX
}MMICONNECTION_SETTING_DATA_ROAMING_E;
#ifndef WIN32//fen.xie
#define MMICONNECTION_SETTING_DATA_ROAMING_DEFAULT MMICONNECTION_SETTING_DATA_ROAMING_DISABLE
#else
#define MMICONNECTION_SETTING_DATA_ROAMING_DEFAULT MMICONNECTION_SETTING_DATA_ROAMING_ENABLE
#endif
#endif
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  FUNCTION:      MMIAPICONNECTION_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:        ryan.xu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_InitModule(void);

/*****************************************************************************/
// 	Description : Register connection menu group
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_RegMenuGroup(void);
/*****************************************************************************/
// 	Description : open connection menu
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_OpenMenuWin(void);
/*****************************************************************************/
// 	Description : close connection menu
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_CloseMenuWin(void);

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : open setting list window of connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_OpenSettingListWin(void);
#endif

/*****************************************************************************/
// 	Description : init connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_Init(void);

/*****************************************************************************/
// 	Description : change th ip address from uint32 type to string, 
//	Global resource dependence :
//  Author: bei.wu 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_IpNumToStr(uint32 ip_num, uint8* ip_str);

/*****************************************************************************/
// 	Description : change th ip address string to uint32 type, 
//	Global resource dependence :
//  Author: bei.wu 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_IpStrToNum(uint8* ip_str, uint32* ip_num);

/*****************************************************************************/
// 	Description : Get link setting item by index
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMICONNECTION_LINKSETTING_DETAIL_T* MMIAPICONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_E dual_sys, uint8 index);
PUBLIC MMICONNECTION_APNTYPEINDEX_T *MMIAPICONNECTION_GetApnTypeIndex(MN_DUAL_SYS_E dual_sys);
PUBLIC MMICONNECTION_PNNAPN_EXIST_T *MMIAPICONNECTION_GetPNNAPNExist(MN_DUAL_SYS_E dual_sys);
PUBLIC void MMIAPICONNECTION_SetPNNAPNExist(MN_DUAL_SYS_E dual_sys, BOOLEAN bIsPNNAPNExist, BOOLEAN is_sim_ready_status);
/*****************************************************************************/
// 	Description : Get link setting num
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICONNECTION_GetLinkSettingNum(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set homepage by index
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_SetHomepage(
                                        MN_DUAL_SYS_E dual_sys,
                                         uint8 index,
                                         const MMI_STRING_T *string_ptr
                                         );

/*****************************************************************************/
// 	Description : ����һ��LIST
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_AddNewItem(
                                        MN_DUAL_SYS_E dual_sys,
                                        const MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr
                                        );

/*****************************************************************************/
// 	Description : ���ԭ��һ��LIST������
//	Global resource dependence : none
//  Author: maryxiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_ReplaceItem(
                                         MN_DUAL_SYS_E dual_sys,
                                         uint16 list_index,
                                         const MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr
                                         );

/*****************************************************************************/
// 	Description : ע��
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_RegisterNotifyInfo(MMICONNECTION_NOTIFY_INFO_T *notify_info_ptr);

/*****************************************************************************/
// 	Description : ����
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_UnregisterNotifyInfo(uint32 module_id);

/*****************************************************************************/
// 	Description : �����б��޸�����
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_ChangeSettingListLanguage(void);

/*****************************************************************************/
//  Description : Open GPRS Setting Window
//  Global resource dependence : 
//  Author: Ying.Xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_OpenGprsWin(void);

/*****************************************************************************/
//  Description : Get GPRS Attach mode (always or manual)
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMICONNECTION_GPRS_SETTINGS_E MMIAPICONNECTION_GetGPRSStatus(void);

/*****************************************************************************/
//  Description : Set GPRS Attach mode (always or manual)
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_SetGPRSStatus(MMICONNECTION_GPRS_SETTINGS_E gprs_settings);

/*****************************************************************************/
//  Description : Reset Factory Settings
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_ResetFactorySetting(BOOLEAN is_clean_user_data);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_isGPRSNeedAutoAttach(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_isGPRSSwitchedOff(void);

/*****************************************************************************/
// 	Description : Get UI list index by apn list index 
//	Global resource dependence : none
//  Author: zhongping.shi
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICONNECTION_GetListIndexByAPNIndex(MN_DUAL_SYS_E dual_sys, uint16 apn_index);
/*****************************************************************************/
// 	Description : Get apn list index by UI list index
//	Global resource dependence : none
//  Author: zhongping.shi
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICONNECTION_GetAPNIndexByListIndex(MN_DUAL_SYS_E dual_sys, uint16 list_index);

#ifdef MMI_GPRS_SWITCH_SUPPORT
/*****************************************************************************/
// 	Description : Set GPRS switch Settings.
//	Global resource dependence : none
//    Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_E gprs_switch_settings);

/*****************************************************************************/
// 	Description : Get GPRS switch Settings.
//	Global resource dependence : none
//    Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMICONNECTION_SETTING_GPRS_SWITCH_E MMIAPICONNECTION_GetGPRSSwitchStatus(void);

#endif
/*****************************************************************************/
//  Description :MMISet_OpenThemeWin
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SHOWVOLTE_WINDOW(void);


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function  Declare                                                      */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

PUBLIC void MMIAPISET_SaveDataSimToNV(MN_DUAL_SYS_E dual_sys);
PUBLIC uint8 MMIAPISET_GetDataSimFromNV(void);
/*****************************************************************************/
// 	Description : MMIAPICONNECTION_InsertStr
//	Global resource dependence :
//  Author: bei.wu 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_InsertStr(                                            //RETURN:
                             wchar *wstr_ptr,
				 uint16 wstr_len
                             );

/*****************************************************************************/
// 	Description : MMIAPICONNECTION_InsertStr
//	Global resource dependence :
//  Author: shuyan.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_CONNECTIONGATEWAY_DNS_InsertStr(
 				uint8 *gateway_ptr                                                                //input gateway string
                            );

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function  Declare                                                      */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifdef MMI_GPRS_SUPPORT

/*****************************************************************************/
//  Description : Auto Adapting Network account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_AutoAdapting(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Auto Adapting specified account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_AdaptingAccount(MN_DUAL_SYS_E dual_sys, MMICONNECTION_ACCOUNT_TYPE_E account_type);

/*****************************************************************************/
//  Description : Get account default index
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICONNECTION_GetAccountDefaultIndex(MN_DUAL_SYS_E dual_sys,
                                                                                                        MMICONNECTION_ACCOUNT_TYPE_E account_type);

/*****************************************************************************/
// 	Description : create Linklist Win for mms,browser,dcd,java
//	Global resource dependence : none
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_TYPE_E module_type, uint8 index,
                                               MN_DUAL_SYS_E dual_sys, MMICONNECTION_CALLBACK_FUNC callback_func);

/*****************************************************************************/
// 	Description : check avalid data account without gateway IP for SNTP of current SIM
//	Global resource dependence : s_connection_setting_info
//    Author: dave.ruan
//	Note:item_num_ptr total derect link item number;
//             detail_item:setting detail,include dual_sys and item_index
//             dual_sys sim index
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_GetCurSIMNoProxyAccountIndex(MN_DUAL_SYS_E   dual_sys, uint8* item_num,MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item,uint8 max_item_num );

/*****************************************************************************/
// 	Description : check avalid data account without gateway IP for SNTP
//	Global resource dependence : s_connection_setting_info
//  Author: dave.ruan
//	Note:item_num_ptr total derect link item number;
//             detail_item:setting detail,include dual_sys and item_index
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_GetNoProxyAccountIndex(uint8* item_num,MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item, uint8 max_item_num);
    //////////////////////////////////////////////////////////////////////////
    // added by feng.xiao
    #ifdef DATA_ROAMING_SUPPORT
    /*****************************************************************************/
    // 	Description : get data roaming setting status
    //	Global resource dependence : none
    //  Author: feng.xiao
    //	Note:
    /*****************************************************************************/
    PUBLIC MMICONNECTION_SETTING_DATA_ROAMING_E MMAPIICONNECTION_Setting_GetDataRoamingStatus(void);
    /*****************************************************************************/
    // 	Description : set data roaming setting status
    //	Global resource dependence : none
    //  Author: feng.xiao
    //	Note:
    /*****************************************************************************/
    PUBLIC void MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_E data_raoming_status);
    /*****************************************************************************/
    // 	Description : whether activated pdp link exist when data roaming
    //	Global resource dependence : none
    //  Author: feng.xiao
    //	Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPICONNECTION_IsRoamingActivedPdpLinkExist(void);
    #endif


#endif

/****************************************************************************/
//Description : set pdp context first before detach gprs
//Params: [In] dual_sys: sim index
//        [Out] None
//        [Return] None
//Author:
//Note:
/****************************************************************************/
PUBLIC void MMIAPICONNECTION_SetPdpContext(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// Description : Get if is Support domestic roaming menu
// Global resource dependence:
// Author:cheney.wu
// Note:
/*****************************************************************************/
PUBLIC uint8 MMICONNECTION_IsSupportDomesticRoaming(void);

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT
    
#define 	MMIConnection_RegMenuGroup		        MMIAPICONNECTION_RegMenuGroup

#define 	MMIConnection_OpenMenuWin		        MMIAPICONNECTION_OpenMenuWin

#define 	MMIConnection_CloseMenuWin		        MMIAPICONNECTION_CloseMenuWin

#ifdef MMI_GPRS_SUPPORT
#define 	MMICONNECTION_OpenSettingListWin	MMIAPICONNECTION_OpenSettingListWin
#endif

#define 	MMICONNECTION_IpNumToStr		        MMIAPICONNECTION_IpNumToStr

#define 	MMICONNECTION_IpStrToNum		        MMIAPICONNECTION_IpStrToNum

#define 	MMICONNECTION_GetLinkSettingItemByIndex		MMIAPICONNECTION_GetLinkSettingItemByIndex

#define 	MMICONNECTION_GetLinkSettingNum		MMIAPICONNECTION_GetLinkSettingNum

#define 	MMICONNECTION_SetHomepage		        MMIAPICONNECTION_SetHomepage

#define 	MMICONNECTION_AddNewItem		        MMIAPICONNECTION_AddNewItem

#define 	MMICONNECTION_ReplaceItem		        MMIAPICONNECTION_ReplaceItem

#define 	MMICONNECTION_RegisterNotifyInfo		MMIAPICONNECTION_RegisterNotifyInfo

#define 	MMICONNECTION_UnregisterNotifyInfo	MMIAPICONNECTION_UnregisterNotifyInfo

#define 	MMICONNECTION_ChangeSettingListLanguage		MMIAPICONNECTION_ChangeSettingListLanguage

#define 	MMICONNECTION_OpenGprsWin		        MMIAPICONNECTION_OpenGprsWin

#define 	MMICONNECTION_GetGPRSStatus		MMIAPICONNECTION_GetGPRSStatus

#define 	MMICONNECTION_SetGPRSStatus		MMIAPICONNECTION_SetGPRSStatus

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
