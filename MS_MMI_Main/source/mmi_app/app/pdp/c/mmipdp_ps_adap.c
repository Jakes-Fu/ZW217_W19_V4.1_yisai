/****************************************************************************************
** File Name:      mmipdp_ps_adap.c                                                     *
** Author:         juan.zhang                                                           *
** Date:           2009.5.27                                                            *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:                                                                         *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.5        juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_pdp_trc.h"
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
#ifdef WIN32
#include "std_header.h"
#endif
#include "mn_api.h"
#include "mmipdp_internal.h"
#ifdef WIN32
#include "sig_code.h"
#endif
#include "sci_api.h"//PDP_UT_TEST
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "socket_api.h"
#include "mmiphone_export.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPDP_DEACTIVE_PROTECT_TIMOUT  40000
#define CIPHER_AUTH_SUPPORT
/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN SetQos(MMIPDP_LINK_INFO_T *link_info_ptr);
LOCAL uint32 RequestPdpId(MMIPDP_LINK_INFO_T *link_info_ptr);

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//Description : SetQos
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN SetQos(MMIPDP_LINK_INFO_T *link_info_ptr)
{
    BOOLEAN             param_used[3]   = {TRUE,TRUE,TRUE};
    ERR_MNGPRS_CODE_E   err = ERR_MNGPRS_NO_ERR;
    
    if(0==link_info_ptr)
    {
        return FALSE;
    }
    //SCI_TRACE_LOW:"PDP MANAGER SetQos pdp_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_69_112_2_18_2_44_57_109,(uint8*)"d",link_info_ptr->pdp_id);
    if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_SetQosProfileEx(link_info_ptr->dual_sys,
        param_used,
        link_info_ptr->pdp_id,
        link_info_ptr->reliability,
        link_info_ptr->peak_throughput)))
    {
        //SCI_TRACE_LOW:"PDP MANAGER SetQos MNGPRS_SetQosProfileEx FAIL,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_76_112_2_18_2_44_57_110,(uint8*)"d",err);
        return FALSE;
    }
    if(ERR_MNGPRS_NO_ERR != 
        (err=MNGPRS_SetExtReqQosProfileEx(link_info_ptr->dual_sys,&link_info_ptr->qos_info)))
    {
        //SCI_TRACE_LOW:"PDP MANAGER SetQos MNGPRS_SetExtReqQosProfileEx FAIL,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_82_112_2_18_2_44_57_111,(uint8*)"d",err);
        return FALSE;
    }
    return TRUE;
}

/****************************************************************************/
//Description : PS Adapter: to active pdp
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsActivePdp(uint32 link_id)
{
#ifdef MMI_GPRS_SUPPORT
#ifndef WIN32    
    MMIPDP_LINK_INFO_T      link_info = {0};/*lint !e64*/
    BOOLEAN                 context_param[7] = {TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
    MN_GPRS_PDP_ADDR_T      pdp_addr = {0}; 
    uint32                  pdp_id_array[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    uint32                  pdp_id = 0;
    ERR_MNGPRS_CODE_E       err = ERR_MNGPRS_NO_ERR;
#endif
    uint16       i = 0;
    if(0==link_id||link_id>MMIPDP_MAX_LINK_NUM)
    {
        return FALSE;
    }
#ifdef WIN32
	{
		APP_MN_GPRS_EXT_T    *sig_ptr = PNULL;
		uint16				 sig_size = 0;

		MMIPDP_SetLinkPdpId(link_id, 1);
		
		sig_size = sizeof(APP_MN_GPRS_EXT_T);
		SCI_CREATE_SIGNAL(sig_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,sig_size,19); /*lint !e64*/
		sig_ptr->dual_sys = MN_DUAL_SYS_1;
		sig_ptr->result = MN_GPRS_ERR_SUCCESS;
		sig_ptr->pdp_id = 1;
		sig_ptr->nsapi = 5;
		SCI_SEND_SIGNAL(sig_ptr,SCI_IDENTIFY_THREAD());
		return TRUE;
	}
#else
    
    MMIPDP_GetLinkInfo(link_id,&link_info);
    
    pdp_id = RequestPdpId(&link_info);
    if(0==pdp_id)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp RequestPdpId fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_131_112_2_18_2_44_57_112,(uint8*)"");
        return FALSE;
    }
    link_info.pdp_id = pdp_id;
    link_info.qos_info.pdp_id = pdp_id;
    MMIPDP_SetLinkPdpId(link_id, pdp_id);
    pdp_id_array[0] = link_info.pdp_id;
    
    //step1 set qos
    if(!SetQos(&link_info))
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp set qos fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_142_112_2_18_2_44_57_113,(uint8*)"");
        return FALSE;
    }
    //step2 set pdp context
     SCI_TRACE_LOW("[MMI_STK] link_info.apn_ptr:%s",link_info.apn_ptr);
    for(i=0;i<strlen(link_info.apn_ptr);i++)
    	{
    	     SCI_TRACE_LOW("[MMI_STK] link_info.apn_ptr[%d]:%x",i,link_info.apn_ptr[i]);
    	}

    SCI_TRACE_LOW("PDP MANAGER MMIPDP_PsActivePdp, ip_type=%d", link_info.ip_type);
    
#ifdef IPVERSION_SUPPORT_V4_V6
    //step2 set pdp context
    if (MMICONNECTION_IP_V4V6 == link_info.ip_type)
    {
    	/* fix compile error, modified by zhigang.peng 2019-07-15 */
        if(ERR_MNGPRS_NO_ERR!=
            (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IPV4V6",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0,MN_GPRS_REQUEST_TYPE_INIT,0,0, FALSE)))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
            return FALSE;
        }
    } 
    else if (MMICONNECTION_IP_V4 == link_info.ip_type)
    {
        if(ERR_MNGPRS_NO_ERR!=
            (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IP",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0,MN_GPRS_REQUEST_TYPE_INIT,0,0, FALSE)))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
            return FALSE;
        }
    } 
    else
    {
        if(ERR_MNGPRS_NO_ERR!=
            (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IPV6",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0,MN_GPRS_REQUEST_TYPE_INIT,0,0, FALSE)))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
            return FALSE;
        }
    }
#else
    if(ERR_MNGPRS_NO_ERR!=
        (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IP",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0,MN_GPRS_REQUEST_TYPE_INIT,0,0, FALSE)))
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
        return FALSE;
    }
#endif

    //step3 set pdp username and psw
    if(link_info.user_name_ptr !=0 && link_info.psw_ptr !=0)//FIX192346,ÉÏÏÂ²ã¼ì²â¼æÈÝ
    {
#ifdef CIPHER_AUTH_SUPPORT
        SCI_TRACE_LOW("link_info.auth_type = %d\n",link_info.auth_type);
        if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_SetPdpContextPcoCipherEx(
            link_info.dual_sys,
            link_info.pdp_id,
            (uint8*)link_info.user_name_ptr,
            (uint8*)link_info.psw_ptr,
            link_info.auth_type)
            ))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextPcoEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_164_112_2_18_2_44_57_115,(uint8*)"d",err);
            SCI_TRACE_LOW("Fail, err: 0x%x",err);
            return FALSE;
        }
#else
        if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_SetPdpContextPcoEx(
            link_info.dual_sys,
            link_info.pdp_id,
            (uint8*)link_info.user_name_ptr,
            (uint8*)link_info.psw_ptr)
            ))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextPcoEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_175_112_2_18_2_44_57_116,(uint8*)"d",err);
            SCI_TRACE_LOW("Fail 2, err: 0x%x",err);
            return FALSE;
        }
#endif
    }
    //step4 active pdp
        SCI_TRACE_LOW("MMIPDP_PsActivePdp, call ExEx");
        //MNGPRS_SWDatatEx(link_info.dual_sys);
        if(ERR_MNGPRS_NO_ERR != (err = MNGPRS_ActivatePdpContextExEx(
            link_info.dual_sys, 
            FALSE, 
            pdp_id_array, 
            link_info.ps_service_type, 
            link_info.ps_service_rat, 
            link_id
            )))
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_ActivatePdpContextEx FAIL:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_189_112_2_18_2_44_57_117,(uint8*)"d",err);
        return FALSE;
    }
    return TRUE;
#endif
#else
    return FALSE;
#endif
}

/****************************************************************************/
//Description : RequestPdpId
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL uint32 RequestPdpId(MMIPDP_LINK_INFO_T *link_info_ptr)
{
#ifdef MMI_GPRS_SUPPORT
    uint32                  pdp_id = 0;
    MN_GPRS_CON_STATE_T     *ret_state_ptr = 0;
    uint32                  pdp_context_num = 0;
    uint32                  i = 0;
    BOOLEAN        is_test_sim = FALSE;//add for bip case
    BOOLEAN        is_already_exist = FALSE;//add for bip case
    MN_GPRS_PDP_DYNAMIC_PARAM_T dynamic_param = {0};
    if (PNULL == link_info_ptr)
    {
        return 0;
    }
    MNGPRS_ReadPdpContextStateEx(link_info_ptr->dual_sys,&ret_state_ptr,&pdp_context_num);

    /*
    T117 feature phone doesn't support AT command. Failed to use the asigned pdp id(11) to active PDP. 
    So the allocation order change from small to large (0~11).
    */
    for(i=1;i<=pdp_context_num;i++)
    {
        if ((MN_CONTEXT_ACTIVATED == ret_state_ptr[i-1].pdp_state) && (9 != ret_state_ptr[i-1].pdp_id) && (11 != ret_state_ptr[i-1].pdp_id))
        {
            if (ERR_MNGPRS_NO_ERR == MNGPRS_ReadPdpContextDynamicParamEx(link_info_ptr->dual_sys, ret_state_ptr[i-1].pdp_id, &dynamic_param))
            {
                if (0 == strncasecmp(link_info_ptr->apn_ptr, dynamic_param.pdp_info->apn, strlen(link_info_ptr->apn_ptr)))
                {
                    pdp_id = ret_state_ptr[i-1].pdp_id;
                    is_already_exist = TRUE;
                    SCI_TRACE_LOW("RequestPdpId: pdp_id=%d, is_already_exist=%d, setting_item_ptr=%s, pdp_info=%s.",
                                  ret_state_ptr[i-1].pdp_id, is_already_exist, link_info_ptr->apn_ptr,dynamic_param.pdp_info->apn);
                    break;
                }
                else
                {
                    SCI_TRACE_LOW("RequestPdpId: pdp_id=%d, setting_item_ptr=%s, pdp_info=%s.",
                                  ret_state_ptr[i-1].pdp_id, link_info_ptr->apn_ptr,dynamic_param.pdp_info->apn);
                }
            }
        }
    }

    if (FALSE == is_already_exist)
    {
        for (i=1; i<=pdp_context_num; i++)//temply add for the GCF6531 Case27.22.7.10.1/1 NSAPI5
		{
			if ((MN_CONTEXT_DEACTIVATED == ret_state_ptr[i-1].pdp_state) && 
                (9 != ret_state_ptr[i-1].pdp_id) && 
                (11 != ret_state_ptr[i-1].pdp_id))
			{
				pdp_id = ret_state_ptr[i-1].pdp_id;
				break;
			}
		}
	}
    if(dynamic_param.pdp_info)
    {
         SCI_TRACE_LOW( "[MMIPDP] RequestPdpId dynamic_param.pdp_info=%p", dynamic_param.pdp_info );
         SCI_FREE(dynamic_param.pdp_info );
    }
    SCI_FREE(ret_state_ptr);
    if (pdp_id > MN_GPRS_MAX_PDP_CONTEXT_COUNT || 0 == pdp_id)
    {
        //SCI_TRACE_LOW:"PDP MANAGER RequestPdpId fail pdp_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_228_112_2_18_2_44_57_118,(uint8*)"d", pdp_id);
        pdp_id = 0;
    }
    //SCI_TRACE_LOW:"PDP MANAGER RequestPdpId pdp_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_231_112_2_18_2_44_57_119,(uint8*)"d", pdp_id);
    return pdp_id;
#else
    return 0;
#endif
}

/****************************************************************************/
//Description : PS Adapter: to deactive pdp
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsDeactivePdp(uint32 link_id)
{
#ifdef MMI_GPRS_SUPPORT
#ifndef WIN32
    MMIPDP_LINK_INFO_T      link_info = {0};/*lint !e64*/
    uint32                  pdp_id_array[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    ERR_MNGPRS_CODE_E       err = ERR_MNGPRS_NO_ERR;
#endif
    
    if(0==link_id||link_id>MMIPDP_MAX_LINK_NUM)
    {
        return FALSE;
    }
#ifdef WIN32
	{
		APP_MN_GPRS_DEACTIVATE_CNF_T    *sig_ptr = PNULL;
		uint16							sig_size = 0;
		
		sig_size = sizeof(APP_MN_GPRS_DEACTIVATE_CNF_T);
		SCI_CREATE_SIGNAL(sig_ptr,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,sig_size,19); /*lint !e64*/
		sig_ptr->dual_sys = MN_DUAL_SYS_1;
		sig_ptr->result = MN_GPRS_ERR_SUCCESS;
		sig_ptr->pdp_num = 1;
		sig_ptr->pdp_id[0] = 1;
		SCI_SEND_SIGNAL(sig_ptr,SCI_IDENTIFY_THREAD());
//		MMIPDP_GetLinkInfo(link_id,&link_info);
//		link_info.deactive_protect_timer = MMIPDP_CreatTimer(MMIPDP_DEACTIVE_PROTECT_TIMOUT);
//		MMIPDP_SetLinkDeactiveTimer(link_id,link_info.deactive_protect_timer);
		return TRUE;
	}
#else
    MMIPDP_GetLinkInfo(link_id,&link_info);
    pdp_id_array[0] = link_info.pdp_id;
    link_info.deactive_protect_timer = MMIPDP_CreatTimer(MMIPDP_DEACTIVE_PROTECT_TIMOUT);
    MMIPDP_SetLinkDeactiveTimer(link_id,link_info.deactive_protect_timer);
    //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsDeactivePdp MMIPDP_CreatTimer:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_277_112_2_18_2_44_57_120,(uint8*)"d",link_info.deactive_protect_timer);
    if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_DeactivatePdpContextEx(link_info.dual_sys,FALSE,pdp_id_array)))
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsDeactivePdp MNGPRS_DeactivatePdpContextEx fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_280_112_2_18_2_44_57_121,(uint8*)"");
        return FALSE;
    }
    return TRUE;
#endif
#else
    return FALSE;
#endif
}

/****************************************************************************/
//Description : get the flow rate 
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsGetFlowRate(uint32 *send_flowrate_ptr, uint32 *recv_flowrate_ptr)
{
#ifdef MMI_GPRS_SUPPORT
    if(0==send_flowrate_ptr||0==recv_flowrate_ptr)
    {
        return FALSE;
    }
    return (0==sci_getsockthroughput(send_flowrate_ptr,recv_flowrate_ptr));
#else
    return FALSE;
#endif
}

/****************************************************************************/
//Description : gprs attach
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsGprsAttach(              
                                   MN_DUAL_SYS_E dual_sys,
                                   MN_PHONE_GPRS_ATTACH_TYPE_E mn_attach_type
                                   )
{
#ifdef MMI_GPRS_SUPPORT
    if(dual_sys>=MN_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsGprsAttach, input error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_319_112_2_18_2_44_57_122,(uint8*)"");
        return FALSE;
    }
#ifdef WIN32
    {
        APP_MN_GPRS_ATTACH_RESULT_T     *sig_ptr = PNULL;
        uint16                          sig_size = 0;
        
        sig_size = sizeof(APP_MN_GPRS_ATTACH_RESULT_T);
        SCI_CREATE_SIGNAL(sig_ptr,APP_MN_GPRS_ATTACH_RESULT,sig_size,19); /*lint !e64*/
        sig_ptr->dual_sys = MN_DUAL_SYS_1;
        sig_ptr->attach_type = MN_PHONE_GPRS_ATTACH;
        sig_ptr->is_attach_ok = TRUE;
        sig_ptr->err_cause = 0;
        SCI_SEND_SIGNAL(sig_ptr,SCI_IDENTIFY_THREAD());
        return TRUE;
    }
#else
    MNPHONE_GprsAttachEx(dual_sys,mn_attach_type);
    return TRUE;
#endif
#else
    return FALSE;
#endif
}

/****************************************************************************/
//Description : To Check if any Actived Pdp Link Exist
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsIsActivedPdpLinkExist(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_GPRS_SUPPORT
    MN_GPRS_CON_STATE_T *pdp_context_arr_ptr = 0;
    uint32              pdp_context_num = 0;
    uint32              i = 0;
    BOOLEAN             is_exist = FALSE;
    
    if(dual_sys>=MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, input error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_359_112_2_18_2_44_57_123,(uint8*)"");
        return FALSE;
    }
    
    if(ERR_MNGPRS_NO_ERR == 
        MNGPRS_ReadPdpContextStateEx(dual_sys,&pdp_context_arr_ptr,&pdp_context_num))
    {
        if(pdp_context_arr_ptr!=0)
        {
            for(i=0; i<pdp_context_num; i++)
            {
                if ((pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED) &&
                    (9 != pdp_context_arr_ptr[i].pdp_id) && 
                    (11 != pdp_context_arr_ptr[i].pdp_id))
                {
                    is_exist = TRUE;
                    break;
                }
            }
            SCI_FREE(pdp_context_arr_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, pdp_context_arr_ptr error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_380_112_2_18_2_44_57_124,(uint8*)"");
            return FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, MNGPRS_ReadPdpContextStateEx error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_386_112_2_18_2_44_57_125,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsActivedPdpLinkExist, is_exist:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_389_112_2_18_2_44_57_126,(uint8*)"d",is_exist);
    return is_exist;
#else
    return FALSE;
#endif
}

/****************************************************************************/
//Description : To Check if any Pdp Link Exist
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_PsIsPdpLinkExist(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_GPRS_SUPPORT
    MN_GPRS_CON_STATE_T *pdp_context_arr_ptr = 0;
    uint32              pdp_context_num = 0;
    uint32              i = 0;
    BOOLEAN             is_exist = FALSE;
    
    if(dual_sys>=MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsPdpLinkExist, input error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_410_112_2_18_2_44_57_127,(uint8*)"");
        return FALSE;
    }
    
    if(ERR_MNGPRS_NO_ERR == 
        MNGPRS_ReadPdpContextStateEx(dual_sys,&pdp_context_arr_ptr,&pdp_context_num))
    {
        if(pdp_context_arr_ptr!=0)
        {
            for(i=0; i<pdp_context_num; i++)
            {
                SCI_TRACE_LOW("MMIPDP_PsIsPdpLinkExist:[%d], pdp_id=%d, pdp_state=%d", i, pdp_context_arr_ptr[i].pdp_id, pdp_context_arr_ptr[i].pdp_state);
                //don't judge pdp id (1) (9) and (11), pdp id (1) is the default pdp, it's always exist. So don't need to judge pdp id (1).
                if ((1 != pdp_context_arr_ptr[i].pdp_id) &&
                    (9 != pdp_context_arr_ptr[i].pdp_id) &&
                    (11 != pdp_context_arr_ptr[i].pdp_id) &&
                    (pdp_context_arr_ptr[i].pdp_state != MN_CONTEXT_DEACTIVATED))
                {
                    is_exist = TRUE;
                    break;
                }
            }
            SCI_FREE(pdp_context_arr_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsPdpLinkExist, pdp_context_arr_ptr error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_431_112_2_18_2_44_57_128,(uint8*)"");
            return FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsPdpLinkExist, MNGPRS_ReadPdpContextStateEx error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_437_112_2_18_2_44_57_129,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsIsPdpLinkExist, is_exist:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_440_112_2_18_2_44_57_130,(uint8*)"d",is_exist);
    return is_exist;
#else
    return FALSE;
#endif
}
#endif
