/*****************************************************************************
** Copyright 2021-2022 Unisoc(Shanghai) Technologies Co.,Ltd                 *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*******************************************************************************
** File Name:       ual_networkmgr.h                                           *
** Author:          haosheng.cui                                               *
** Date:             06/09/2021                                                *
** Description:    This file is used to define ual bt export api of networkmgr *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 06/09/2021           haosheng.cui           Create                           *
********************************************************************************/
#ifndef _UAL_NETWORKMGR_H_
#define _UAL_NETWORKMGR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ual_type.h"
#include "ual_cms.h"
#include "ual_tele_common.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UAL_NETWORKMGR_MODULE_ID    (0x02 << 16)// UAL_TODO: mmi module id?

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef enum
{
    UAL_NETWORKMGR_SUCCESS = 0,
    UAL_NETWORKMGR_ERROR,
    UAL_NETWORKMGR_PARAM_ERROR,
    UAL_NETWORKMGR_WIFI_PARAM_ERROR,
    UAL_NETWORKMGR_CELL_PARAM_ERROR,
    UAL_NETWORKMGR_RESULT_MAX,
}ual_networkmgr_result_e;

typedef enum
{
    UAL_NETWORKMGR_MSG_WIFI_DISCONNECT,//wifi disconnect
    UAL_NETWORKMGR_MSG_WIFI_CONNECT_UNAVAILABLE,//wifi connect but can't access the internet
    UAL_NETWORKMGR_MSG_WIFI_CONNECT_AVAILABLE,//wifi connect and can access the internet
    UAL_NETWORKMGR_MSG_CELLULAR_DATA_DISCONNECT,//cellular data disconnect
    UAL_NETWORKMGR_MSG_CELLULAR_DATA_CONNECT_AVAILABLE,//cellular data connect and can access the internet
    UAL_NETWORKMGR_MSG_END,
}ual_networkmgr_msg_e;

typedef enum
{
    UAL_NETWORKMGR_ROUTE_DEFAULT = -1,//default  UAL_TODO:不作为时间链路类型被选择，目前用于标识link prefer，后续删除
    UAL_NETWORKMGR_ROUTE_WIFI = 0,//wifi
    UAL_NETWORKMGR_ROUTE_CELLULAR,//cellular
    UAL_NETWORKMGR_ROUTE_MAX,
}ual_networkmgr_route_e;

typedef enum
{
    UAL_NETWORKMGR_LINK_WIFI_PREFER = 0,//Wifi first
    UAL_NETWORKMGR_LINK_CELLULAR_PREFER,//cellular first
	UAL_NETWORKMGR_LINK_WIFI_ONLY,//wifi only
	UAL_NETWORKMGR_LINK_CELLULAR_ONLY,//cellular only
    UAL_NETWORKMGR_LINK_MAX,
}ual_networkmgr_link_prefer_e;  //UAL_TODO:后续修改名称

typedef enum
{
    UAL_NETWORKMGR_WIFI_ROUTE_DISCONNECTED,
    UAL_NETWORKMGR_WIFI_ROUTE_CONNECT_UNAVAILABLE,
    UAL_NETWORKMGR_WIFI_ROUTE_CONNECT_AVAILABLE,
    UAL_NETWORKMGR_CELL_ROUTE_DISCONNECTED,
    UAL_NETWORKMGR_CELL_ROUTE_CONNECT_AVAILABLE,
    UAL_NETWORKMGR_ROUTE_STATE_MAX
}ual_networkmgr_state_e;

typedef struct
{
    uint32                       net_id;
    ual_networkmgr_route_e       net_route;
    ual_networkmgr_state_e       net_status;//当前网络Net id对应的网络通路状态
}ual_networkmgr_status_t;

typedef struct
{
    ual_networkmgr_status_t network_status[UAL_NETWORKMGR_ROUTE_MAX];
}ual_networkmgr_all_routes_status_t;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : get net info by route
//	Parameter: [In] route_type
//				[Out] p_net_info:当wifi或cell不是available状态时，返回wifi或
//					 cell参数错误，此时net id为无效值
//			   [Return]ual_networkmgr_result_e
//	Author: haosheng.cui
//	Note:UAL_TODO:后续此接口只用于通过route来获取wifi或cell链路net info,会将
//				  UAL_NETWORKMGR_ROUTE_DEFAULT部分分离出来,形成让用户通过模
//				  式获取当前链路信息的接口
/*****************************************************************************/
ual_networkmgr_result_e ual_networkmgr_get_net_info_by_route(ual_networkmgr_route_e route_type, ual_networkmgr_status_t * const p_net_info);

/*****************************************************************************/
//  Description : get all net info
//  Parameter: [In]
//             [Out] p_network_status
//             [Return] ual_networkmgr_result_e
//  Author: haosheng.cui
//  Note:
/*****************************************************************************/
ual_networkmgr_result_e ual_networkmgr_get_all_net_info(ual_networkmgr_all_routes_status_t *p_network_status);

/*****************************************************************************/
//  Description : setlink prefer
//  Parameter: [In] link_prefer
//             [Out]
//             [Return] ual_networkmgr_result_e
//  Author: haosheng.cui
//  Note: 获取wifi和cell链路的net info
/*****************************************************************************/
ual_networkmgr_result_e ual_networkmgr_set_link_prefer(ual_networkmgr_link_prefer_e link_prefer);

/*****************************************************************************/
//  Description : ual get link prefer
//  Parameter: [In]
//             [Out]
//             [Return] ual_networkmgr_link_prefer_e
//  Author: haosheng.cui
//  Note:
/*****************************************************************************/
ual_networkmgr_link_prefer_e ual_networkmgr_get_link_prefer(void);

/*****************************************************************************/
//  Description : 激活cellular数据端的pdp链接
//  Parameter: [In] sim_number sim卡ID
//             [Out] none
//             [Return]ual_networkmgr_result_e
//  Author: yonghua.zhang
//  Note:
/*****************************************************************************/
ual_networkmgr_result_e ual_networkmgr_cellular_data_activate(ual_tele_sim_id_e sim_number);

/*****************************************************************************/
//  Description : 去激活cellular数据端的pdp链接
//  Parameter: [In] none
//             [Out] none
//             [Return]ual_networkmgr_result_e
//  Author: yonghua.zhang
//  Note:
/*****************************************************************************/
ual_networkmgr_result_e ual_networkmgr_cellular_data_deactivate(void);

/*****************************************************************************/
//  Description: ual_networkmgr_init
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: weipeng.wang
//  Note:
/*****************************************************************************/
void ual_networkmgr_init(void);

/*****************************************************************************/
//	Description: ual_networkmgr_register
//	Parameter: [In] p_callback private call back
//	           [Out] p_handle out of phandle
//	           [Return] ual_networkmgr_result_e
//	Author: weipeng.wang
//	Note:
/*****************************************************************************/
ual_networkmgr_result_e ual_networkmgr_register(_ual_cms_client_register_callback p_callback, uint32 *p_handle);

/*****************************************************************************/
//	Description: ual_networkmgr_unregister
//	Parameter: [in] handle
//	           [Out] none
//	           [Return] true:success,false:fail
//	Author: weipeng.wang
//	Note:
/*****************************************************************************/
ual_networkmgr_result_e ual_networkmgr_unregister(uint32 handle);


#endif
