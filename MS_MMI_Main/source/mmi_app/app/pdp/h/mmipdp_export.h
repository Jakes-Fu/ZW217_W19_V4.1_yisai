/*****************************************************************************
** File Name:      mmipdp_export.h                                              *
** Author:         juan.zhang                                                *
** Date:           11/02/2009                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/02/2009     juan.zhang          Create                                 *
******************************************************************************/

#ifndef _MMIPDP_API_H_
#define _MMIPDP_API_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_api.h"
#include "mn_type.h"
#ifndef ADULT_WATCH_SUPPORT
#include "mmiconnection_export.h"
#endif
#ifdef ADULT_WATCH_SUPPORT
#include "ual_tele.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MMIPDP_APP_MSG_INVALID,
    MMIPDP_ACTIVE_REQ,          
    MMIPDP_ACTIVE_CNF,
    MMIPDP_DEACTIVE_REQ,
    MMIPDP_DEACTIVE_CNF,
    MMIPDP_DEACTIVE_IND,
    MMIPDP_SERVICE_CHANGE_IND,
    MMIPDP_SUSPEND_IND,
    MMIPDP_RESUME_IND,
    MMIPDP_ACTIVE_TIMOUT_IND,
    MMIPDP_REACTIVE_TIMER_IND,
    MMIPDP_PS_ACTIVE_CNF,
    MMIPDP_PS_DEACTIVE_CNF,
    MMIPDP_PS_DEACTIVE_IND
} MMIPDP_APP_MSG_E;

typedef enum
{
    MMIPDP_RESULT_SUCC,
    MMIPDP_RESULT_FAIL,
#ifndef GPRSMPDP_SUPPORT
    MMIPDP_RESULT_AT_IS_ON, //正在使用AT拨号
#endif
    MMIPDP_RESULT_DIFFER_DUALSYS,
    MMIPDP_RESULT_TIMEOUT,
    MMIPDP_RESULT_NOT_PERMIT, //不允许使用网络，比如飞行模式
#ifdef DATA_ROAMING_SUPPORT
    MMIPDP_RESULT_NOT_PERMIT_ROAMING, //漫游模式关闭网络(added by feng.xiao)
#endif
    MMIPDP_RESULT_FDN_NOT_PERMIT, //不允许使用网络，FDN only
}MMIPDP_RESULT_E;

typedef enum
{
    MMIPDP_LINK_STATE_DEACTIVED,
    MMIPDP_LINK_STATE_ACTIVED,
    MMIPDP_LINK_STATE_DEACTIVING,
    MMIPDP_LINK_STATE_ACTIVING,
    MMIPDP_LINK_STATE_MAX
}MMIPDP_LINK_STATE_E;

typedef enum
{
    MMIPDP_NOTIFY_EVENT_ACTIVED,
    MMIPDP_NOTIFY_EVENT_DEACTIVED,
    MMIPDP_NOTIFY_EVENT_ATTACH,
    MMIPDP_NOTIFY_EVENT_DETTACH,
    MMIPDP_NOTIFY_EVENT_MAX
}MMIPDP_NOTIFY_EVENT_E;

typedef enum
{
    MMIPDP_INTERFACE_GPRS, /*GPRS链路*/
    MMIPDP_INTERFACE_WIFI, /*WiFi链路*/
    MMIPDP_INTERFACE_MAX,    
} MMIPDP_INTERFACE_E;
/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

typedef struct
{
    uint32              app_handler;
    uint32              nsapi;   /*原来的值是uint8,为了支持wifi，扩展为uint32*/
    uint32              pdp_id;
    MMIPDP_APP_MSG_E    msg_id;
    MMIPDP_INTERFACE_E  ps_interface; //数据承载的接口
    MMIPDP_RESULT_E     result;
}MMIPDP_CNF_INFO_T;

typedef void (*MMIPDP_MSG_CALLBACK)(MMIPDP_CNF_INFO_T *msg_ptr);//msg_ptr is freed in PDP MGR, app must NOT free
typedef void (*MMIPDP_RELEASE_ALL_CALLBACK)(void);
typedef void (*MMIPDP_NOTIFY_FUNC)(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E notify_event);

#ifdef ADULT_WATCH_SUPPORT
#ifdef IPVERSION_SUPPORT_V4_V6
typedef enum
{
    MMIPDP_IP_V4,
    MMIPDP_IP_V6,
    MMIPDP_IP_V4V6,
    MMIPDP_IP_MAX
}MMIPDP_IP_TYPE_E;
#endif

#define MMIPDP_LINKSETTINGS_MAX_LEN		            30          //用户名的最大长度

typedef struct
{
    uint8 username[MMIPDP_LINKSETTINGS_MAX_LEN + 1];	//用户名
    uint8 password[MMIPDP_LINKSETTINGS_MAX_LEN + 1];	//密码
    uint8 apn[MMIPDP_LINKSETTINGS_MAX_LEN + 1];				//接入点
    MN_PCO_AUTH_TYPE_E auth_type;
#ifdef IPVERSION_SUPPORT_V4_V6
    MMIPDP_IP_TYPE_E ip_type;   //ip type
#endif
}MMIPDP_LINKSETTINGS_DETAIL_T;//连接设置的结构

typedef struct
{
    uint8 index;
    BOOLEAN is_valid;
}MMIPDP_TYPE_INDEX_T;

typedef struct
{
    MMIPDP_TYPE_INDEX_T internet_index[UAL_SIM_NUMBER_MAX];
    MMIPDP_TYPE_INDEX_T xcap_index[UAL_SIM_NUMBER_MAX];
}MMIPDP_APN_TYPE_INDEX_T;
#endif

typedef struct
{
    uint32                          app_handler;        //app handler
    char                            *apn_ptr;           //apn string
    char                            *user_name_ptr;     //user name string
    char                            *psw_ptr;           //password string
    MN_DUAL_SYS_E                   dual_sys;           //dual sim choice
    MN_PS_SERVICE_TYPE_E            ps_service_type;
    MN_PS_SERVICE_RAT_E         ps_service_rat;
    MN_PCO_AUTH_TYPE_E auth_type;
    MN_GPRS_EXT_QOS_INFO_T          qos_info;           //QOS info for qos setting
    MN_GPRS_RELIABILITY_TYPE_E      reliability;        //for MNGPRS_SetQosProfile
    MN_GPRS_PEAK_THROUGHPUT_TYPE_E  peak_throughput;    //for MNGPRS_SetQosProfile
    uint32                          priority;           //the smaller the number, the higher the priority
    uint8                           active_timer_id;    //used by pdpmanager,APP should set it to zero when activing input
    MMIPDP_MSG_CALLBACK             handle_msg_callback;//callback handle for dealing pdp manager message
    MMIPDP_INTERFACE_E              ps_interface;          //数据承载的接口               
    MN_GPRS_STORAGE_E      storage;
#ifdef IPVERSION_SUPPORT_V4_V6
#ifdef ADULT_WATCH_SUPPORT
	MMIPDP_IP_TYPE_E                ip_type;
#else
    MMICONNECTION_IP_TYPE_E        ip_type;
#endif
#endif
}MMIPDP_ACTIVE_INFO_T;

typedef struct 
{
    uint32 total_send;  //总发送流量
    uint32 total_recv;  //总接收流量    
    uint32 last_send;   //上次连接发送流量
    uint32 last_recv;   //上次连接接收流量
}MMIPDP_FLOWRATE_T;


typedef struct 
{
    uint32                 app_handler;         
    MMIPDP_MSG_CALLBACK    handle_msg_callback; /*app处理pdp消息的回调函数*/
    MMIPDP_LINK_STATE_E    e_app_status;         
} MMIPDP_WIFI_APPINFO_T;

typedef struct 
{
    uint32  dwWifiRegAppList;
} MMIPDP_MEM_T;

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description : GetLinkSettingItemByIndex
//  Global resource dependence :
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC MMIPDP_LINKSETTINGS_DETAIL_T* MMIAPIPDP_GetLinkSettingItemByIndex(MN_DUAL_SYS_E dual_sys, uint8 index);
/*****************************************************************************/
//  Description : Get APN type index
//  Global resource dependence :
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC MMIPDP_APN_TYPE_INDEX_T* MMIAPIPDP_GetApnTypeIndex(MN_DUAL_SYS_E dual_sys);
#endif
/****************************************************************************/
//Description : Active pdp link req
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_Active(MMIPDP_ACTIVE_INFO_T *app_info_ptr);

/****************************************************************************/
//Description : Deactive pdp link req
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_Deactive(uint32 app_handler);

/****************************************************************************/
//Description : Get Link State
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC MMIPDP_LINK_STATE_E MMIAPIPDP_GetLinkState(uint32 app_handler);

/****************************************************************************/
//Description : pdp app initiation
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_AppInit (void);

/****************************************************************************/
//Description : Get flowrate infomation
//Global resource dependence : 
//Author: juan.zhang
//Note: 注意，在多pdp的情况下，获取的上次连接的流量是错误的，原因是下层的流量
//      计算放在socket那边计算的，无法区分各个链路的流量。需要下层修改计算方法
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_GetFlowrate(MMIPDP_FLOWRATE_T *flowrate_ptr, MN_DUAL_SYS_E dual_sys);

/****************************************************************************/
//Description : Get flowrate infomation
//Global resource dependence : 
//Author: juan.zhang
//Note: 注意，在多pdp的情况下，获取的上次连接的流量是错误的，原因是下层的流量
//      计算放在socket那边计算的，无法区分各个链路的流量。需要下层修改计算方法
/****************************************************************************/
PUBLIC void MMIAPIPDP_ClearFlowrate(MN_DUAL_SYS_E dual_sys);

/****************************************************************************/
//Description : To release all pdp
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_ReleaseAll(MMIPDP_RELEASE_ALL_CALLBACK release_all_cb);

/****************************************************************************/
//Description : To Subscribe the notify event
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_SubscribeEvent(
                                     uint32                 module_id,
                                     MMIPDP_NOTIFY_FUNC     notify_func,//[IN]callback on notify
                                     uint32                 notify_event_num,//[IN]notify event number
                                     MMIPDP_NOTIFY_EVENT_E  *notify_event_arr_ptr //[IN]notify events
                                     );

/****************************************************************************/
//Description : To Unsubscribe the notify event
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_UnsubscribeEvent(
                                     uint32                 module_id
                                     );

/****************************************************************************/
//Description : To reset pdp
//Global resource dependence : 
//Author: juan.zhang
//Note: 此函数只能在底层PDP链路释放，但又不是经过MMIPDP释放时调用，以此通知MMIPDP
//改变状态。比如手动将网络从G网切换到TD，底层自行将网络接口释放了，而MMI就需要调用
//此函数复位MMIPDP的状态。
/****************************************************************************/
PUBLIC void MMIAPIPDP_ResetPdp(MN_DUAL_SYS_E dual_sys);

/****************************************************************************/
//Description : To Unsubscribe the notify event
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_HandleIdleEvent(void);

/*****************************************************************************/
// Description : set last pdp event
// Global resource dependence : 
// Author: 
// Note: /*@CR255469 modify 2011.08.23*/
/*****************************************************************************/
PUBLIC void MMIAPIPDP_SetPdpLastEvent(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E last_pdp_event);

/*****************************************************************************/
// Description : 当网络类型发生变化的时候，更新数据业务图标显示
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIAPIPDP_SetGprsInfoWhenRatSwitch( void );

/****************************************************************************/
//Description : To Check if any Actived Pdp Link Exist
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsActivedPdpLinkExist(void);

/*+ @CR255469 2011.08.23*/
/****************************************************************************/
//Description : To Check if Actived Pdp Link Exist with the sim
//Global resource dependence : 
//Author: fen.xie
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsActivedPdpLinkExistEx(MN_DUAL_SYS_E dual_sys);
/*- @CR255469 2011.08.23*/

/****************************************************************************/
//Description : To Check if any Pdp Link Exist
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsPdpLinkExist(void);

/****************************************************************************/
//Description : To Check if any Pdp Link Exist with the sim
//Global resource dependence : 
//Author: fen.xie
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsPdpLinkExistEx(MN_DUAL_SYS_E dual_sys);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
/****************************************************************************/
//Description : MMIPDP_IsDifferdualsysActivedPdpLinkExist
//Global resource dependence : 
//Author: kun.yu
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_IsDifferdualsysActivedPdpLinkExist(MN_DUAL_SYS_E dual_sys);
#endif
/*****************************************************************************/
// Description : Get last pdp event
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC MMIPDP_NOTIFY_EVENT_E MMIAPIPDP_GetPdpLastEvent(MN_DUAL_SYS_E dual_sys); /*@CR255469 modify 2011.08.23*/

/*****************************************************************************/
// Description : MMIAPIPDP_WifiActiveCnf
// Global resource dependence : 
// Author: George.Liu 01526
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIPDP_WifiActiveCnf(MMIPDP_RESULT_E eResult);

/*****************************************************************************/
// Description : MMIAPIPDP_WifiDeactiveInd
// Global resource dependence : 
// Author: George.Liu 01526
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIPDP_WifiDeactiveInd(void);

/****************************************************************************/
//Description : Indicate Service change
//Global resource dependence : 
//Author: Jiaoyouwu
//Note:
/****************************************************************************/
PUBLIC void MMIAPIPDP_WifiServiceChangeInd(void);

/****************************************************************************/
//Description : Indicate Service change
//Global resource dependence : 
//Author: Jiaoyouwu
//Note:
/****************************************************************************/
PUBLIC void MMIAPIPDP_GPRSServiceChangeInd(void);

/****************************************************************************/
//Description : Get PS Service SIM(Not In PDP return MMI_DUAL_SYS_MAX)
//Global resource dependence : 
//Author: wancan.you
//Note: 
/****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPDP_GetPdpLinkSIM(void);

/****************************************************************************/
//Description : set gprs retach(you should set pdp context first before detach gprs)
//Params: [In] dual_sys: sim index
//        [Out] None
//        [Return] None
//Author:
//Note:
/****************************************************************************/
PUBLIC void MMIAPIPDP_GprsRetach(MN_DUAL_SYS_E dual_sys);

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	MMIPDP_WifiActiveCnf	   		MMIAPIPDP_WifiActiveCnf

#define 	MMIPDP_WifiDeactiveInd   			MMIAPIPDP_WifiDeactiveInd

#define 	MMIPDP_WifiServiceChangeInd			MMIAPIPDP_WifiServiceChangeInd

#define 	MMIPDP_GPRSServiceChangeInd			MMIAPIPDP_GPRSServiceChangeInd

#endif //MMI_FUNC_COMPATIBLE_SUPPORT


#ifdef   __cplusplus
    }
#endif


#endif 

