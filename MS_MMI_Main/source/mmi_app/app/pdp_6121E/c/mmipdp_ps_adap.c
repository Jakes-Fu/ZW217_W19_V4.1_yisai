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
#include "sci_api.h"//pdp_ut_test
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "socket_api.h"
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
	MN_PLMN_T play_hplmn = {0};					//for poland carrier "PLAY"
    
    if(0==link_info_ptr)
    {
        return FALSE;
    }
    //SCI_TRACE_LOW:"PDP MANAGER SetQos pdp_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_69_112_2_18_2_44_57_109,(uint8*)"d",link_info_ptr->pdp_id);
    if(ERR_MNGPRS_NO_ERR!=(err=MNGPRS_SetQosProfileEx(link_info_ptr->dual_sys,
        param_used,
        link_info_ptr->pdp_id,3,0)))
//        link_info_ptr->reliability,
//        link_info_ptr->peak_throughput)))
    {
        //SCI_TRACE_LOW:"PDP MANAGER SetQos MNGPRS_SetQosProfileEx FAIL,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_76_112_2_18_2_44_57_110,(uint8*)"d",err);
        return FALSE;
    }
	//for Poland carrier "PLAY", bug511283
	MMIAPIPHONE_GetSimHPLMN(&play_hplmn, link_info_ptr->dual_sys);
	if( play_hplmn.mcc == 260 && ( play_hplmn.mnc == 6 || play_hplmn.mnc == 98 ) ){
		SCI_TRACE_LOW("SetQos for PLAY, sim.mcc=%d, sim.mnc=%d",play_hplmn.mcc, play_hplmn.mnc);
		link_info_ptr->qos_info.traffic_class = 0;
		link_info_ptr->qos_info.sdu_error_ratio[0] = 0;
		link_info_ptr->qos_info.sdu_error_ratio[1] = 0;
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

#ifdef IPVERSION_SUPPORT_V4_V6
    //step2 set pdp context
    if (MMICONNECTION_IP_V4 == link_info.ip_type)
    {
        if(ERR_MNGPRS_NO_ERR!=
            (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IP",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0)))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
            return FALSE;
        }
    } 
    else if (MMICONNECTION_IP_V6 == link_info.ip_type)
    {
        if(ERR_MNGPRS_NO_ERR!=
            (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IPV6",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0)))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
            return FALSE;
        }
    } 
    else
    {
        if(ERR_MNGPRS_NO_ERR!=
            (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IPV4V6",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0)))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
            return FALSE;
        }
    }
#else

	if(ERR_MNGPRS_NO_ERR!=
            (err=MNGPRS_SetPdpContextEx(link_info.dual_sys,context_param,link_info.pdp_id,(uint8 *)"IP",(uint8 *)link_info.apn_ptr,pdp_addr,0,0,0)))
    {
         //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextEx FAIL,%d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_149_112_2_18_2_44_57_114,(uint8*)"d",err);
         return FALSE;
    }

#endif
    
    //step3 set pdp username and psw
    if(link_info.user_name_ptr !=0 && link_info.psw_ptr !=0)//FIX192346,上下层检测兼容
    {
#ifdef CIPHER_AUTH_SUPPORT
        if (MN_PAP_CHAP == link_info.auth_type)
        {
            if (FALSE == link_info.reqChapForPapOrChap)
            {
                err=MNGPRS_SetPdpContextPcoCipherEx(link_info.dual_sys,
                                                    link_info.pdp_id,
                                                    (uint8*)link_info.user_name_ptr,
                                                    (uint8*)link_info.psw_ptr,
                                                    MN_PAP);
            }
            else
            {
                err=MNGPRS_SetPdpContextPcoCipherEx(link_info.dual_sys,
                                                    link_info.pdp_id,
                                                    (uint8*)link_info.user_name_ptr,
                                                    (uint8*)link_info.psw_ptr,
                                                    MN_CHAP);
            }
        }
        else
        {
            err=MNGPRS_SetPdpContextPcoCipherEx(link_info.dual_sys,
                                                link_info.pdp_id,
                                                (uint8*)link_info.user_name_ptr,
                                                (uint8*)link_info.psw_ptr,
                                                link_info.auth_type);
        }
        if(ERR_MNGPRS_NO_ERR!=err)
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_PsActivePdp, MNGPRS_SetPdpContextPcoEx FAIL,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_PS_ADAP_164_112_2_18_2_44_57_115,(uint8*)"d",err);
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
            return FALSE;
        }
#endif
    }
    //step4 active pdp
    if(ERR_MNGPRS_NO_ERR != (err=MNGPRS_ActivatePdpContextEx(
        link_info.dual_sys,
        FALSE,
        pdp_id_array,
        link_info.ps_service_type,
        link_info.ps_service_rat
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
    ERR_MNGPRS_CODE_E err_code_e = ERR_MNGPRS_NO_ERR;
    
    if(0==link_info_ptr)
    {
        return 0;
    }
    //added by Tong Yongli. According to Coverity CID 16080
    err_code_e = MNGPRS_ReadPdpContextStateEx(link_info_ptr->dual_sys,&ret_state_ptr,&pdp_context_num);
    SCI_TRACE_LOW("PDP MANAGER RequestPdpId err_code_e=%d, pdp_context_num=%d", err_code_e, pdp_context_num);
    if(err_code_e != ERR_MNGPRS_NO_ERR)
    {
        return 0;
    }
    //从大到小分配pdp id，以避免同AT的冲突
#ifdef MMI_GCF_SUPPORT
    for (i=1; i<=pdp_context_num; i++)//temply add for the GCF6531 Case27.22.7.10.1/1 NSAPI5
#else
    for (i=pdp_context_num; i>=1; i--)
#endif
    {
        if (MN_CONTEXT_DEACTIVATED == ret_state_ptr[i-1].pdp_state)
        {
            pdp_id = ret_state_ptr[i-1].pdp_id;
            SCI_TRACE_LOW("PDP MANAGER RequestPdpId pdp_id=%d, i=%d", pdp_id, i);
            break;
        }
    }
    SCI_FREE(ret_state_ptr);
    if (pdp_id>MN_GPRS_MAX_PDP_CONTEXT_COUNT || 0 == pdp_id)
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
                if(pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
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
                if(pdp_context_arr_ptr[i].pdp_state == MN_CONTEXT_ACTIVATED)
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
