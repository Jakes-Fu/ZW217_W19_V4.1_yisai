/****************************************************************************
** File Name:      mmimbbms_main.h                                            *
** Author:         *.*		                                                   *
** Date:           2009-6-25                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe window and control id.     *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 6/2009        lisa.lin           Create
****************************************************************************/
#ifdef MBBMS_SUPPORT
#ifndef _MMIMBBMS_MAIN_H_
#define _MMIMBBMS_MAIN_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mbbms_auth.h"
#include "mbbms_sg_api.h"
#include "mbbms_sub.h"
#include "mbbms_service.h"

#include "Mn_type.h"
#include "mmipdp_export.h"
#include "mmk_type.h"
#include "mmimtv_data.h"
#include "mmimtv_main.h"
#include "Mn_type.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

#ifdef WIN32
//#define MBBMS_TEST	
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMBBMS_SERVICE_NAME_MAX_LEN       MMIMTV_MAX_NAME_LEN
#define MMIMBBMS_SERVICE_DETAIL_MAX_LEN     100
#define MMIMBBMS_CONTENT_NAME_MAX_LEN       MMIMBBMS_SERVICE_NAME_MAX_LEN

#define MMIMBBMS_START_TIME_MAX_LEN         20
#define M_MMI_MBBMS_MAX_DAY_NUM             3
#define MMIMBBMS_MAX_NAF_ADDR_LEN	        60
#define M_MMI_MBBMS_MAX_CONTENT_TYPE_NUM    7
#define M_MMIMBBMS_MAX_SG_DOMAIN_LENS       80
#define M_MMIMBBMS_MAX_NAF_ADDR_LENS        80
#define MMIMBBMS_PURCHASE_NAME_MAX_LEN      255

#define M_MMIMBBMS_MAX_WELCOME_INFO_LENS    255
#define MMIMBBMS_COMMON_MAX_LENS            255

#define MMIMBBMS_SETTING_DEFAULT_INDEX      0

#define MMIMBBMS_DEFAULT_SG_DOMAIN          "http://sg.mbbms.chinamobile.com:80/?requesttype=sg_retrieve" 
#define MMIMBBMS_DEFAULT_NAF_ADDR	        "sg.mbbms.chinamobile.com:80"

#define MMIMBBMS_MAX_DATE_STR_LENS          11

#define MMIMBBMS_MEMORY_NEEDED_SIZE         2100000

#define MMIMBBMS_INVALID_SERVICE_ID         0xFFFFFFFF


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/   
typedef enum
{
    MMIMBBMS_FLOW_NONE,	   	
	MMIMBBMS_FLOW_SG_MANUAL_UPDATE,
	MMIMBBMS_FLOW_SG_PARSE,
    MMIMBBMS_FLOW_ESG_PARSE,
    MMIMBBMS_FLOW_SG_GET_COMPLEX,

    //mbbms2.0 new
    MMIMBBMS_FLOW_MBBMS_ACTIVE,
    MMIMBBMS_FLOW_PLAY,
	
	MMIMBBMS_FLOW_SUBSCRIBE,
	MMIMBBMS_FLOW_UNSUBSCRIBE,
	MMIMBBMS_FLOW_SUBTABLE_UPDATE,
	
	MMIMBBMS_FLOW_GBA_OPERATION,
	MMIMBBMS_FLOW_ACCOUNT_INQUERY,

    MMIMBBMS_FLOW_MAX
}MMIMBBMS_FLOW_TYPE_E;

typedef enum
{
	MMIMBBMS_FLOW_STATE_IDLE,
	MMIMBBMS_FLOW_STATE_STARTING,	
	MMIMBBMS_FLOW_STATE_STOPPING,
    
    MMIMBBMS_FLOW_STATE_MAX
}MMIMBBMS_FLOW_STATE_E;

typedef struct
{
	MMIMBBMS_FLOW_TYPE_E	flow_type;	
	MMIMBBMS_FLOW_STATE_E   status;

}MMIMBBMS_FLOW_INFO_T;

//search sg entry type
typedef enum
{
	MMIMBBMS_SG_SEARCH_PARSE_FROM_NONE,
    MMIMBBMS_SG_SEARCH_PARSE_FROM_PRE_INIT, //初始进入先解析旧SG
	MMIMBBMS_SG_SEARCH_PARSE_FROM_MAINMENU,  //used for mbbms 2.0(in mbbms2.0 add a menu for update SG and sub)
	MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_MAX
	
}MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E;

typedef enum
{
    MMIMBBMS_OBJECT_FROM_NONE,
    MMIMBBMS_OBJECT_FROM_ALL_SERVICE,
    MMIMBBMS_OBJECT_FROM_FAVORITE_SERVICE,    
    MMIMBBMS_OBJECT_FROM_FAVORITE_CONTENT,
    MMIMBBMS_OBJECT_FROM_PROGROM_LIST,
    MMIMBBMS_OBJECT_FROM_PROGROM_DETAIL,
    MMIMBBMS_OBJECT_FROM_SEARCH_RESULT_OPT,
    MMIMBBMS_OBJECT_FROM_SEARCH_RESULT,
    MMIMBBMS_OBJECT_FROM_PROGRAM_LIST_OPT,
    
    MMIMBBMS_OBJECT_FROM_TYPE_MAX
    
}MMIMBBMS_OBJECT_FROM_TYPE_E;

typedef enum
{
    MMIMBBMS_SUBSCRIBE_FROM_NONE,
    MMIMBBMS_SUBSCRIBE_FROM_PLAY,

    MMIMBBMS_SUBSCRIBE_FROM_TYPE_MAX
}MMIMBBMS_SUBSCRIBE_FROM_TYPE_E;

typedef enum
{
	MMIMBBMS_SUB_UPDATE_FROM_NONE,    
    MMIMBBMS_SUB_UPDATE_FROM_PURCHASE_MANAGE, 
    MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY,
    MMIMBBMS_SUB_UPDATE_FROM_UNCLEAR, //业务或套餐 操作失败需更新订购关系表
       
	MMIMBBMS_SUB_UPDATE_FROM_TYPE_MAX
}MMIMBBMS_SUB_UPDATE_FROM_TYPE_E;

typedef enum
{
	MMIMBBMS_GBA_MRK_FROM_NONE,
    MMIMBBMS_GBA_MRK_FROM_SUBSCRIBLE, //GBA/MRK  Operation after subscrible with GBA error
    MMIMBBMS_GBA_MRK_FROM_UNSUBSCRIBLE,//GBA/MRK Operation after unsubscrible with GBA error
//	MMIMBBMS_GBA_MRK_FROM_AFTER_PLAY,   //GBA error form play error
 	MMIMBBMS_GBA_MRK_FROM_ENG_MODE,		//GBA test from eng mode
 	
	MMIMBBMS_GBA_MRK_FROM_TYPE_MAX
}MMIMBBMS_GBA_MRK_FROM_TYPE_E;

typedef enum
{
	MMIMBBMS_PLAY_FROM_SERVICE,    //enter play process from service
	MMIMBBMS_PLAY_FROM_CONTENT,	//enter play procsee from content
	
	MMIMBBMS_PLAY_FROM_MAX
}MMIMBBMS_PLAY_FROM_TYPE_E;

typedef enum
{
    MMIMBBMS_CONTENT_TIME_NOT_START,
    MMIMBBMS_CONTENT_TIME_EXPIRED,
    MMIMBBMS_CONTENT_TIME_OK,

    MMIMBBMS_CONTENT_TIME_MAX
}MMIMBBMS_CONTENT_TIME_TYPE_E;

typedef struct  MMIMBBMS_CMMB_PROG_ID_T_tag
{
    uint32  net_id;
    uint32  service_id;
} MMIMBBMS_CMMB_PROG_ID_T;

typedef struct
{
    anyURI	 uri;                       //mbbms keyword of anyURI type
    MMIMBBMS_CMMB_PROG_ID_T cmmb_id;    //cmmb keyword  

    uint32   frequency_no;              //frequency for mbbms mode, and net_id for cmmb mode
    uint32   service_id;                //service id

    MMIMTV_PLAY_STATUS_TYPE_E cmmb_play_status;

    BOOLEAN  is_need_play_next;         //whether need play next service while stop ok
    //BOOLEAN  is_encrypt;
    BOOLEAN  is_audio_type;

    BOOLEAN  is_quick_stop_play;
    BOOLEAN  is_received_quick_ack;
    BOOLEAN  is_need_resume_play;       //whether need resume play

    MMIMBBMS_PLAY_FROM_TYPE_E from_type;
}MMIMBBMS_PLAY_INFO;

/*remainder of Content starting*/
typedef enum
{
	MMIMBBMS_REMAINDER_OFF =0,
	MMIMBBMS_REMAINDER_TWO_MIN,      //two minutes before program starting
	MMIMBBMS_REMAINDER_FIVE_MIN,
	MMIMBBMS_REMAINDER_TEN_MIN,

	MMIMBBMS_REMAINDER_MAX
}MMIMBBMS_TIME_REMAINDER_E;

typedef struct _MMIMBBMS_LINK_SETTING_T_
{
    uint8 sim1_index;				//for sim1
    uint8 sim2_index;                       //for sim2
	
}MMIMBBMS_LINK_SETTING_T;

//MBBMS setting info
typedef struct _MMIMBBMS_SETTING_INFO_T_
{ 	
    MMIMBBMS_LINK_SETTING_T link_setting;	
    MMIMBBMS_TIME_REMAINDER_E  time_remainder;    
    char    sg_domain_info[M_MMIMBBMS_MAX_SG_DOMAIN_LENS];
    char    sg_naf_addr[M_MMIMBBMS_MAX_NAF_ADDR_LENS];  
    uint8   sg_welcome_info[M_MMIMBBMS_MAX_WELCOME_INFO_LENS];
    uint32  welcome_info_len;
	
}MMIMBBMS_SETTING_INFO_T;

//mbbms2.0
typedef struct MMIMBBMS_SERVICE_SIG_T_tag
{
    SIGNAL_VARS                 //signal
    MBBMS_SERVICE_INFO_T  data; //operate status  
}MMIMBBMS_SERVICE_SIG_T;

/*this struct defined just for subscription management*/
typedef struct MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T_tag
{
	uint32  index; 					        //index in richtext ctrl
	uint8   purchase_name[MMIMBBMS_PURCHASE_NAME_MAX_LEN+1]; //purchase name,UTF8 format
	float   price;  					    //price info
	uint32  currency;  				        //currency type
	BOOLEAN is_ordered;			            //order status
    uint8   period;                         //订购周期 
	MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E scription_type;
}MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T;

typedef struct MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T_tag
{
    wchar       service_name[MMIMBBMS_SERVICE_NAME_MAX_LEN +1];     //频道名

    BOOLEAN     is_exist_content;                                   //节目是否有效
    wchar       content_info[MMIMBBMS_SERVICE_NAME_MAX_LEN +1];     //最近的节目信息
    wchar       time_info[MMIMBBMS_START_TIME_MAX_LEN + 1];         //节目起始时间

    BOOLEAN     is_collected;                                       //是否加入收藏
    BOOLEAN     forfree;            // 免费指示(免费:TRUE, 收费:FALSE)
    BOOLEAN     is_ordered;
    BOOLEAN     is_encrypted;
}MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T;

typedef struct MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T_tag
{
    wchar       content_name[MMIMBBMS_SERVICE_NAME_MAX_LEN +1];     //节目名
    wchar       start_time[MMIMBBMS_START_TIME_MAX_LEN +1];         //起始时间
    wchar       start_date[MMIMBBMS_START_TIME_MAX_LEN +1];         //起始日期    
    wchar       service_name[MMIMBBMS_SERVICE_NAME_MAX_LEN +1];     //节目对应的频道
    BOOLEAN     is_set_alarm;                                       //是否设置了节目提醒
}MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T;

typedef struct MMIMBBMS_BG_LAYER_INFO_T_tag 
{
    BOOLEAN             is_bg_layer_open;
    GUI_LCD_DEV_INFO    layer_dev_handle;
}MMIMBBMS_BG_LAYER_INFO_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : init mbbms setting info 
// Global resource dependence : none
// Author: lisa.lin
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_InitSettingInfo(void);

/*****************************************************************************/
//  Description : start manual update SG 
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartManualUpdateSG(MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E from_type);

/*****************************************************************************/
// Description : copy anyURI
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_Copy2AnyURI(
                                 const anyURI *src_ptr, // [IN]
                                 anyURI *dst_ptr // [OUT]
                                 );

/*****************************************************************************/
// 	Description : whether MBBMS app is opened
//	Global resource dependence :
//  Author:xin.li
//	Note: MBBMS应用是否已开启
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsMBBMSOpen(void);

/*****************************************************************************/
// 	Description :set all kinds of flow state
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_TYPE_E flow_type, MMIMBBMS_FLOW_STATE_E status);

/*****************************************************************************/
// 	Description :get all kinds of flow state
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_FLOW_INFO_T* MMIMBBMS_GetFlowInfo(void);
/*****************************************************************************/
// 	Description : Stop All flow related with http
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIMBBMS_StopFlow(void);

/*****************************************************************************/
// 	Description : mbbms init process
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_InitProcess(void);

/*****************************************************************************/
// 	Description : handle startup win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIMBBMS_HandleStartUpWinMsg( 
                                                 MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 );

/*****************************************************************************/
// 	Description :Enter into main window
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterIntoMainWindow(void);

/*****************************************************************************/
// 	Description : Handle MRK Operation signal
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
//PUBLIC void MMIMBBMS_HandleMRKOperationResSignal(DPARAM param);

/*****************************************************************************/
// 	Description : Handle MRK Stop Operation signal
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleMRKStopOperationResSignal(DPARAM param);

/*****************************************************************************/
//  Description : get mbbms setting info
//  Global resource dependence : 
//  Author: plum.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetSettingInfo(MN_DUAL_SYS_E dual_sys,
					  uint16* notify_type_ptr,
                                     uint16* nerwork_index_ptr, 
                                     MMI_STRING_T* sg_ptr,
                                     MMI_STRING_T* naf_ptr);

/*****************************************************************************/
//  Description : save mbbms setting info
//  Global resource dependence : 
//  Author: plum.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SaveSettingInfo(MN_DUAL_SYS_E dual_sys,
					  uint16 notify_type,
                                     uint16 nerwork_index, 
                                     wchar* pSGDomain,
                                     wchar* pNafAddr);

/*****************************************************************************/
// Description : get index of service
// Global resource dependence :
// Author: plum.peng
// Note: data structure of node is MBBMSSG_SERVICE_INDEX_T
/*****************************************************************************/
PUBLIC SG_ERROR_E MMIMBBMS_GetServiceIndex(SG_LIST_PTR_T *serviceIdList_head_ptr);

/*****************************************************************************/
// Description : free index of service
// Global resource dependence :
// Author:plum.peng
// Note: data structure of node is MBBMSSG_SERVICE_INDEX_T
/*****************************************************************************/
PUBLIC SG_ERROR_E MMIMBBMS_FreeServiceIndex(void);

/*****************************************************************************/
// 	Description : set/update current operating purchase info via global index
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurrPurchaseDisplayInfo(uint32 index);

/*****************************************************************************/
// 	Description : handle enter into purchase item list window
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_GetCurrPurchaseOrdereStatus(void);

/*****************************************************************************/
// 	Description : get the current purchase index in richtext ctrl list
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetCurrPurchaseIndex(void);

/*****************************************************************************/
// 	Description : get the current purchase name
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIMBBMS_GetCurrPurchaseName(void);

/*****************************************************************************/
// 	Description : get the current purchase name
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC float MMIMBBMS_GetCurrPurchasePrice(void);

/*****************************************************************************/
// 	Description : get the current purchase subscription type
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E MMIMBBMS_GetCurrPurchaseSubscriptionType(void);

/*****************************************************************************/
// 	Description : get the current purchase subscription type
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetCurrPurchaseCurrency(void);

/*****************************************************************************/
//  Description :wheterh the purchase is local or not
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsLocalPurchaseItem(anyURI* pGlobalPurchaseItemID);

/*****************************************************************************/
//  Description : whether the purchase is national or not
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsNationalPurchaseItem(anyURI* pGlobalPurchaseItemID);

/*****************************************************************************/
// 	Description : to display all services or contents in purchase item to richtext
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayServiceOrContentInPurchase(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : Display all purchase item
//	Global resource dependence :
//  Author: 
//	Note:
//should check index
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayAllPurchaseItem(uint32 purchase_index, MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
// 	Description : Subscrible  PurchaseItem
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_SubscriblePurchaseItem(void);

/*****************************************************************************/
// 	Description : Unsubscrible PurchaseItem
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_UnSubscriblePurchaseItem(void);

/*****************************************************************************/
// 	Description : whether sd card is inserted and service is availbale
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_SDandOpenCondition(void);

/*****************************************************************************/
// 	Description : open sd card insert alert win
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenInsertSDCardAlertWin(void);

/*****************************************************************************/
//  Description : handle manual update sg signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleManualUpdateSGSignal(DPARAM param);

/*****************************************************************************/
//  Description : handle Stop manual update sg signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopManualUpdateSGSignal(DPARAM param);

/*****************************************************************************/
//  Description : handle MSK update signal
//  Global resource dependence : none
//  Author: xiaoxiang.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleMSKupdateSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle parse sg signal 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleParseSGSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle stop sg parse signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopParseSGSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle parse cmmb esg signal 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleParseESGSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle stop parse cmmb esg signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopParseESGSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle active mbbms signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleActiveMBBMSSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle stop active mbbms signal
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopActiveMBBMSSignal(DPARAM param);

/*****************************************************************************/
// Description : handle update subscription table signal
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleUpdateSubTableSignal(DPARAM param);

/*****************************************************************************/
// Description : handle stop update subscription table signal
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopUpdateSubTableSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle gba operation signal
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleGBAOperationSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle stop gba operation signal
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopGBAOperationSignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle start play signal
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStartPlaySignal(DPARAM param);

/*****************************************************************************/
// 	Description : handle stop play signal
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopPlaySignal(DPARAM param);

/*****************************************************************************/
//  Description : handle Subscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleSubscribeSignal(DPARAM param);

/*****************************************************************************/
//  Description : handle stop subscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopSubscribeSignal(DPARAM param);

/*****************************************************************************/
//  Description : handle unsubscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleUnSubscribeSignal(DPARAM param);

/*****************************************************************************/
//  Description : handle stop unsubscribe operation signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopUnSubscribeSignal(DPARAM param);

/*****************************************************************************/
//  Description : handle Account inquiry signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleAccountInquirySignal(DPARAM param);

/*****************************************************************************/
//  Description : handle stop Account inquiry signal
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStopAccountInquirySignal(DPARAM param);

/*****************************************************************************/
//  Description : MMIMBBMS_HandleSGGetComplex
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleSGGetComplex(DPARAM param);

/*****************************************************************************/
//  Description : MMIMBBMS_HandleSGStopComplex
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleSGStopComplex(DPARAM param);

/*****************************************************************************/
// 	Description : enter play
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterPlay(anyURI id);

/*****************************************************************************/
// 	Description : enter cmmb mode play
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterCMMBModePlay(MMIMBBMS_CMMB_PROG_ID_T *prog_t_ptr);

/*****************************************************************************/
// 	Description : Create a purchase item to richtext
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddPurchaseItemToListboxCtrl(
                MMI_CTRL_ID_T                ctrl_id,
                MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T* purchase_display_info_ptr);

/*****************************************************************************/
//  Description : set subscribe from type
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_TYPE_E type);

/*****************************************************************************/
//  Description : get subscribe from type
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_SUBSCRIBE_FROM_TYPE_E MMIMBBMS_GetSubscribeFromType(void);

/*****************************************************************************/
// 	Description : Set mbbms play-from type
//	Global resource dependence : none
//  Author: lisa
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_TYPE_E play_from_type);

/*****************************************************************************/
// 	Description : Set Current play uri
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurrPlayUri(anyURI uri);

/*****************************************************************************/
// 	Description : Free Current play uri
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeCurrPlayUri(void);

/*****************************************************************************/
// 	Description : Start MBBMS
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/

PUBLIC void MMIMBBMS_EnterMBBMS(void);

/*****************************************************************************/
// Description :Exit MBBMS
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_ExitMBBMS(void);

/*****************************************************************************/
// 	Description : init network notify info
//	Global resource dependence :
//  Author: xin.li
//	Note: 初始化网络通知信息，网络状态变化时通知MTV
/*****************************************************************************/
PUBLIC void MMIMTV_InitNetworkNotifyInfo(void);

/*****************************************************************************/
// 	Description : notify callback
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_NotifyEventCallback(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event, uint32 param);

/*****************************************************************************/
// 	Description : Add purchase content to richtext control
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/

PUBLIC void MMIMBBMS_AddPurchaseContentToRchtextCtrl(
                MMI_CTRL_ID_T ctrl_id,
                uint8*        service_name_ptr,
                uint32 string_len);

/*****************************************************************************/
//  Description : whether the service is  ordered
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsServiceOrdered(anyURI serviceId);

/*****************************************************************************/
// 	Description : whether the service to be played is encryted
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsServiceEncrypted(anyURI serviceId);

/*****************************************************************************/
// 	Description : whether the Content to be played is encryted
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsContentEncrypted(anyURI contentId);

/*****************************************************************************/
// 	Description : Restor factory settings
//	Global resource dependence : Restore MBBMS setting info t
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_RestoreFactorySetting(void);

/*****************************************************************************/
//  Description : register content alarm
//  Global resource dependence : 
//  Author: plum.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_RegisterContentAlarm(MMIMBBMS_ALARM_CONTENT_INFO_T* pContentInfo);

/*****************************************************************************/
// 	Description : MMIMBBMS_ContentNotifyCallback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ContentNotifyCallback(void);

/*****************************************************************************/
//  Description : MMIMBBMS_InitContentNotifyInfo
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_InitContentNotifyInfo(void);

 /*****************************************************************************/
 //  Description : MMIMBBMS_CancelAllAlarm
 //  Global resource dependence : 
 //  Author: 
 //  Note:
 /*****************************************************************************/
 PUBLIC void MMIMBBMS_CancelAllAlarm(void);

 /*****************************************************************************/
//  Description : MMIMBBMS_UpdateAllContentAlarm
//  Global resource dependence : 
//  Author: plum.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_UpdateAllContentAlarm(uint16 old_time, uint16 new_time);

/*****************************************************************************/
//  Description : start sg parse process
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartParseSG(MMIMBBMS_SG_SEARCH_PARSE_FROM_TYPE_E from_type);

/*****************************************************************************/
//  Description : start parse cmmb esg, cmmb mode only
//  Global resource dependence : none
//  Author: xin.li
//  Note: only available for cmmb mode
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartParseESG(void);

/*****************************************************************************/
// 	Description : start subscribe process
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartSubscribeProcess(void);

/*****************************************************************************/
// 	Description : start unsubscribe process
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartUnSubscribeProcess(void);

/*****************************************************************************/
// 	Description : start update subscription table
//	Global resource dependence : none
//  Author: xin.li
//	Note: 更新订购关系表
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartUpdateSubscriptionTable(MMIMBBMS_SUB_UPDATE_FROM_TYPE_E from_type);

/*****************************************************************************/
// Description : start play flow
// Global resource dependence : none
// Author: xin.li
// Note: 播放主流程
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_StartPlay(void);

/****************************************************************************/
// Description : stop play
// Global resource dependence : none
// Author: xin.li
// Note: 停止播放
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_StopPlay(void);

/****************************************************************************/
// Description : quit stop play
// Global resource dependence : none
// Author: xin.li
// Note: 快速停止播放，先停止CMMB播放，再停MBBMS相关流程
/*****************************************************************************/
PUBLIC void MMIMBBMS_QuickStopPlay(void);

/*****************************************************************************/
// 	Description : Display common error
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayCommonError(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
// 	Description : Display common tip via text
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayCommonTip(MMI_TEXT_ID_T text_id, MMI_IMAGE_ID_T image_id);

/*****************************************************************************/
//  Description : the last subscription update status
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMBBMS_GetSubscriptionUpdateStatus(void);

/*****************************************************************************/
// 	Description : mbbms kill threads and clear other parameters
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_Clear(void);

/*****************************************************************************/
//  Description : set subscription update type
//  Global resource dependence : none
//  Author: lisa
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_TYPE_E sub_update_type);

/*****************************************************************************/
//  Description :get subscription update type
//  Global resource dependence : none
//  Author: lisa
//  Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_SUB_UPDATE_FROM_TYPE_E MMIMBBMS_GetSubUpdateType(void);

/*****************************************************************************/
// 	Description : start account inquir process
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartAccoutInquiryProcess(void);

/*****************************************************************************/
// 	Description : start gba test for eng mode
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_StartGBATest(void);

/*****************************************************************************/
//  Description : the init process(sg search,sg parse,subscription update,gba,mrk) is finished
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_GetInitStatus(void);

/*****************************************************************************/
//  Description : set init status flag
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetInitStatus(BOOLEAN is_finish);

/*****************************************************************************/
// 	Description : Activate PDP context for mbbms
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ActivePDPContext(MTVSERVICE_PDP_ACTIVE_T *active_info_ptr);

/*****************************************************************************/
//  Description : Deactivate PDP context
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_DeactivePDPContext(void);

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: plum.peng
// Note:
/*****************************************************************************/
PUBLIC uint16  MMIMBBMS_GetRemindTime(uint32 index);

/*****************************************************************************/
// Description : MMIMBBMS_GetSettingInfo
// Global resource dependence : none
// Author: plum.peng
// Note:
/*****************************************************************************/
PUBLIC const MMIMBBMS_SETTING_INFO_T*  MMIMBBMS_GetConstSettingInfo(void);

/*****************************************************************************/
//  Description : Get mbbms net setting index 
//  Global resource dependence : none
//  Author: lisa.lin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMBBMS_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIMBBMS_GetPlayInfo
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC  MMIMBBMS_PLAY_INFO*  MMIMBBMS_GetPlayInfo(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_GetPlayInfo
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_PreExitCallback(void);

/*****************************************************************************/
// 	Description : set stop msk type flag
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetMskStopFromType(BOOLEAN is_from_enterplay);

/*****************************************************************************/
// 	Description : set stop msk type flag
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsMskStopFromEnterPlay(void);

/*****************************************************************************/
// 	Description : whether mbbms current playing service is audio type
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsCurrentServiceAudioTyle(void);

/*****************************************************************************/
// Description : enter play proccess
// Global resource dependence : 
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_EnterPlayprocess(void);

/*****************************************************************************/
// Description : whether permit enter cmmb play process
// Global resource dependence : 
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsPermitCMMBPlay(void);

/*****************************************************************************/
// Description : get current service mode
// Global resource dependence : 
// Author: xin.li
// Note: 两种模式MBBMS_SERVICE_MODE_CMMB/MBBMS_SERVICE_MODE_MBBMS
/*****************************************************************************/
PUBLIC MTV_SERVICE_MODE_E MMIMBBMS_GetServiceMode(void);

/*****************************************************************************/
// Description : set current service mode
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetServiceMode(MTV_SERVICE_MODE_E mode);
/*****************************************************************************/
// 	Description : handle Account Inquiry request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenAccountInquiryRequestWin(void);

/*****************************************************************************/
// Description : 逐个获取已订购套餐信息并显示
// Global resource dependence :
// Author: 
// Note:
//should check index
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayAllPurchasedItem(MMI_CTRL_ID_T ctrl_id, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr);

/*****************************************************************************/
//  Description : to display all services or contents in purchased item to richtext
//  Global resource dependence : none
//  Author: 
//  Note: 显示套餐详情，（套餐名、包含的所有频道或节目、订购时间、资费）
/*****************************************************************************/
PUBLIC void MMIMBBMS_DisplayServiceOrContentInPurchasedItem(MMI_CTRL_ID_T ctrl_id, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr);

/*****************************************************************************/
// Description :获取套餐查询返回的数据指针
// Global resource dependence :
// Author: 
// Note:
//should check index
/*****************************************************************************/
PUBLIC MBBMS_Account_Inquiry_Res_t* MMIMBBMS_GetAccountInquiryRes(void);


/*****************************************************************************/
// Description : 释放套餐查询返回的结构体指针
// Global resource dependence :
// Author: 
// Note:
//should check index
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeAccountInquiryRes(void);

/*****************************************************************************/
//  Description : set current purchase display info
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurrPurchasedItemDisplayInfo(uint32 index, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr);

/*****************************************************************************/
//  Description : GET city name for update
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIMBBMS_GetUpdateCityName(uint16 *city_name_len);

/*****************************************************************************/
//  Description : set city name for update
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetUpdateCityName(MMI_STRING_T *text_ptr);

/*****************************************************************************/
//  Description : set city update SG flag
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCityUpdateSGFlag(BOOLEAN is_city_update);

/*****************************************************************************/
//  Description : whether update SG via city
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsCityUpdateSG(void);

/*****************************************************************************/
// Description :获取已订购套餐结点个数
// Global resource dependence :
// Author: yexiong.liu
// Note:
//should check index
/*****************************************************************************/
PUBLIC uint16 MMIMBBMS_GetAllPurchasedItemNum(MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr);

/*****************************************************************************/
// Description :获取已订购套餐指定结点指针
// Global resource dependence :
// Author: yexiong.liu
// Note:
//should check index
/*****************************************************************************/
PUBLIC MBBMS_Purchase_Table_Info_t* MMIMBBMS_GetPurchasedItemByIndex(uint16 index, MBBMS_Account_Inquiry_Res_t*  account_inquiry_res_ptr);

/*****************************************************************************/
//     Description : UnsubscriblePurchaseItem according to index in listbox for mbbms2.0
//    Global resource dependence : none
//  Author:  yexiong.liu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_UnSubscriblePurchaseItemNew(void);

/*****************************************************************************/
// 	Description : 创建背景OSD层，用以显示音频广播背景图片等
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_CreateBgOSDLayer(void);

/*****************************************************************************/
// 	Description : 获取背景层设备信息
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* MMIMBBMS_GetBgOSDLayerDev(void);

/*****************************************************************************/
// 	Description : 背景层是否创建
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsBgOSDLayerOpen(void);

/*****************************************************************************/
// 	Description : 销毁背景OSD层
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_ReleaseBgOSDLayer(void);

/*****************************************************************************/
// 	Description : 获取当前紧急广播等级信息
//	Global resource dependence :
//  Author: xiaoxiang.huang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_GetEbStopPlayFlag(void);

/*****************************************************************************/
//  Description : check sgdd
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_CheckSGDD(void);

/*****************************************************************************/
//  Description : get current sim select
//  Global resource dependence :
//  Author: xin.li
//  Note: 获取当前选择的SIM卡
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIMBBMS_GetCurSelectSim(void);

/*****************************************************************************/
//  Description : set current sim select
//  Global resource dependence :
//  Author: xin.li
//  Note: 设置当前选择的SIM卡
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetCurSelectSim(MN_DUAL_SYS_E dual_sys);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
    

#endif  /* _MMIMBBMS_MAIN_H_ */
#endif
