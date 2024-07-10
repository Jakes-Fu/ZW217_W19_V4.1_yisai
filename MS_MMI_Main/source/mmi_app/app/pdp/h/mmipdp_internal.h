/*****************************************************************************
** File Name:      mmipdp_internal.h                                         *
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

#ifndef _MMIPDP_INTERNAL_H_
#define _MMIPDP_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmipdp_export.h"
#include "mmipdp_list.h"
#ifdef ADULT_WATCH_SUPPORT
#include "mmiconnection_export.h"
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
#ifndef GPRSMPDP_SUPPORT
#define MMIPDP_MAX_LINK_NUM	1
#else
#define MMIPDP_MAX_LINK_NUM	2//MN_GPRS_MAX_PDP_CONTEXT_COUNT
#endif
/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
	//MMIPDP_LINK_STATE_E state;
	//uint32 pdp_id;
	BOOLEAN is_busy;
	uint32 link_id;
	//MMIPDP_APP_INFO_T info;
}MMIPDP_FSM_T;

typedef struct 
{
    //BOOLEAN				is_used;
    MMIPDP_LINK_STATE_E		state;
    MN_PCO_AUTH_TYPE_E auth_type;
    uint32					pdp_id;
    uint8					nsapi;
    char					*apn_ptr;
    char					*user_name_ptr;
    char					*psw_ptr;
    MN_DUAL_SYS_E			dual_sys;
    MN_PS_SERVICE_TYPE_E	ps_service_type;
    MN_PS_SERVICE_RAT_E     ps_service_rat;
    MN_GPRS_EXT_QOS_INFO_T  qos_info;
    MN_GPRS_RELIABILITY_TYPE_E       reliability;
    MN_GPRS_PEAK_THROUGHPUT_TYPE_E   peak_throughput;
    MMIPDP_FSM_T			*fsm_ptr;
    uint8					deactive_protect_timer;
    uint8					reactive_timer_id;
    uint8					reactive_count;
    MMIPDP_LIST_HANDLE		using_list;
#ifdef IPVERSION_SUPPORT_V4_V6
    MMICONNECTION_IP_TYPE_E        ip_type;
#endif
}MMIPDP_LINK_INFO_T;

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
//Description : get the link info
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC void MMIPDP_GetLinkInfo(uint32 link_id, MMIPDP_LINK_INFO_T *link_info_ptr);

/****************************************************************************/
//Description : PS Adapter: to active pdp
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsActivePdp(uint32 link_id);
/****************************************************************************/
//Description : get the flow rate 
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsGetFlowRate(uint32 *send_flowrate_ptr, uint32 *recv_flowrate_ptr);

/****************************************************************************/
//Description : PS Adapter: to deactive pdp
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsDeactivePdp(uint32 link_id);

PUBLIC uint8 MMIPDP_CreatTimer(uint32 timeout);

PUBLIC void MMIPDP_StopTimer(uint8 *timer_id_ptr);

PUBLIC void MMIPDP_SetLinkDeactiveTimer(uint32 link_id, uint8 timer_id);

PUBLIC void MMIPDP_SetLinkPdpId(uint32 link_id, uint32 pdp_id);

PUBLIC BOOLEAN MMIPDP_PsGprsAttach(              
								   MN_DUAL_SYS_E dual_sys,
								   MN_PHONE_GPRS_ATTACH_TYPE_E mn_attach_type
									);

PUBLIC BOOLEAN MMIPDP_PsIsActivedPdpLinkExist(MN_DUAL_SYS_E dual_sys);

PUBLIC BOOLEAN MMIPDP_PsIsPdpLinkExist(MN_DUAL_SYS_E dual_sys);

/****************************************************************************/
//Description : to handle the pdp manager timer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIPDP_HandleTimer(uint8 timer_id, uint32 param);

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : is permit active dp
//  Global resource dependence : 
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPDP_IsFDNPermitActivePdp(MN_DUAL_SYS_E dual_sys);
#endif

#ifdef   __cplusplus
    }
#endif


#endif 

