/*****************************************************************************
** File Name:      mmiphone_network_name.c                                          *
** Author:                                                                   *
** Date:           03/26/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe network name string info, include SPN ,PNN,OPL, ONS string        *
*                          and NITZ and so on
一、关于PNN的显示：
1.PNN存在SIM卡里面，初始化、调用、读取都是通过异步消息来更新到MMI，用全局量记录
2.PNN是OPN，替代原来的OPN显示，优先于局端来的short name和full name

1.SIM卡初始化完成SIM_READY_CNF上来后读取PNN，读取后用全局量保存，一直不变，下次SIM READY CNF上来后再次读取更新
2.Netname Ready只是上报SPN/ONS/SPDI可用的消息，与PNN无关
3.在update network name时就调用保存PNN的全局量，每次LAC更新后查看PNN index是否变化，变化则刷新显示OPN
4.如果PNN存在，而OPL不存在，默认显示PNN的第一条
5.PNN和OPL在SIM ready之后就可以读取了
6.读取PNN和OPL是异步的，同时发消息不影响队列

二、关于CPHS中 ONS 和shortONS的显示:
Enhanced Operator Name String (EONS)
Common PCN Handset Specification(CPHS) (Version 4.2)
PCN:personal communication network，个人通信网
CPHS的network name包括ons和short ons
1.在底层CPHS的几个消息接口：
APP_MN_CPHS_READY_IND
APP_MN_CPHS_INFO_IND
APP_MN_OPER_NAME_IND
2.ONS和short ONS两种方式获取:同步直接读取;异步获取OPER_NAME;我们目前使用同步接口直接获取

三、关于NITZ:
每次NITZ上来会主动更新network name显示（发送full paint消息到WIN ID(idle,widget)？）；每次更新network name显示的时候都去check NITZ部分

1.存储full name或者short name的目的是防止网络本来有名字的，刷新后没有，显示不全的问题
2.short name和full name同时存在时优先显示、存储short name
3.short name和full name还是放在network name全局量里面的OPN里面，在update运营商字符串调用
4.如果full name和short name都不存在？清除所有显示？还是判断PLMN一致情况下再删除

四、所有OPN的显示问题优先级如下：PNN > CPHS > NITZ > default(系统默认)
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/26/2012       dave.ruan          Create                              *
******************************************************************************/
#ifndef _MMIPHONE_NETWORK_NAME_C_
#define _MMIPHONE_NETWORK_NAME_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#ifdef MMI_NETWORK_NAME_SUPPORT
#include "mmiidle_subwintab.h"
#include "mmiidle_statusbar.h"
#include "mmiidle_export.h"
#include "mmi_id.h"
#include "mmipub.h"
#include "mmiset_export.h"
#include "gui_ucs2b_converter.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#define strncasecmp(a,b,n)	strnicmp(a,b,n)
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
LOCAL MMIPHONE_NETWORK_NAME_T   s_network_name[MMI_DUAL_SYS_MAX]= {0};      //记录ACI+SPN+OPN(PNN-NITZ-ONS)
LOCAL MMIPHONE_PNN_T   s_pnn_network_name[MMI_DUAL_SYS_MAX]= {0};       //记录PNN的内容

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//  Description : 注册sim卡插拔消息的callback
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void RegPlugSimCallBack(void);

/*****************************************************************************/
//     Description : Handle SIM Plug Notify PNN handle
//    Global resource dependence : 
//  Author: dave.ruan
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyPNNFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif

/*****************************************************************************/
//  Description :Get Curr SIM PLMN Network Name Num
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is an asynchronism function
/*****************************************************************************/
LOCAL BOOLEAN GetPNNContent(
                            MN_DUAL_SYS_E    dual_sys,
                            MNSIM_SERVICE_TYPE_PH2_E mn_file_name,
                            uint8  record_id
                            );

/*****************************************************************************/
//  Description :Get Curr SPN content
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is a synchronization function
/*****************************************************************************/
LOCAL BOOLEAN GetSPNContent(
                            MN_DUAL_SYS_E    dual_sys
                            );

#if defined(MMI_SIM_ICON_SUPPORT) 
/*****************************************************************************/
//  Description :Get Curr SPN Icon content
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is an asynchronism function
/*****************************************************************************/
LOCAL BOOLEAN GetSPNIContent(
                            MN_DUAL_SYS_E    dual_sys
                            );
#endif

/*****************************************************************************/
//  Description :Get Curr SIM ONS content
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:ons is operator name  string "6F14"Or "6F18"
//          this is a synchronization function
/*****************************************************************************/
LOCAL BOOLEAN GetONSContent(
                            MN_DUAL_SYS_E    dual_sys
                            );

/*****************************************************************************/
//  Description :use OPL to get PNN index 
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:select PNN index from operator PLMN list
/*****************************************************************************/
LOCAL uint8  GetPNNIndexByOPL(
                           MN_DUAL_SYS_E    dual_sys,
                           MN_PLMN_T           cur_plmn,
                           uint16  lac
                           );

/*****************************************************************************/
//  Description : set the PNN/PNNI  in to MMI
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetPLMNNetworkName(
                                                MN_DUAL_SYS_E   dual_sys,       //IN:
                                                MN_SIM_RECORD_FILE_BUF_U  *record_file_buf,  //in:
                                                MNSIM_SERVICE_TYPE_PH2_E  mn_file_name    //IN
                                                );

/*****************************************************************************/
//  Description :Is Curr Service Plmn In Spdi
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:if service PLMN is in spdi, SPN should display when roaming 
/*****************************************************************************/
LOCAL BOOLEAN IsServicePlmnInSpdi(
                           MN_DUAL_SYS_E    dual_sys,
                           MN_PLMN_T           cur_plmn
                           );

/*****************************************************************************/
//  Description :Convert OPN Content To Wchar 
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:Convert OPN String , priority is PNN-NITZ-ONS(CPHS)-Default
/*****************************************************************************/
LOCAL uint16 ConvertOPNToWchar(                                           //RETURN: 
                                    MN_OPN_DCS_E    opn_dcs,   //IN: string type
                                    uint8   opn_len,    //IN
                                    uint8* opn,     //IN
                                    wchar* opn_string    //OUT
                                    );

/*****************************************************************************/
//  Description :Select OPN String
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:select OPN String from PNN-NITZ-ONS(CPHS)-Default
/*****************************************************************************/
LOCAL void SelectOPNString(
                         MN_DUAL_SYS_E dual_sys,
                         MMI_STRING_T* opn_string
                         );

/*---------------------------------------------------------------------------*/
/*                          RESERVED FOR WIN_TAB DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//  Description : 注册sim卡插拔消息的callback
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL void RegPlugSimCallBack(void)
{
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
    
    sim_plug_notify_info.module_id = MMI_MODULE_COMMON;//use temp
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifyPNNFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
}

/*****************************************************************************/
//     Description : Handle SIM Plug Notify PNN handle
//    Global resource dependence : 
//  Author: dave.ruan
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyPNNFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    MMIPHONE_PNN_T* pnn_info =  MMIPHONE_GetPNNInfo();

    SCI_TRACE_LOW("[MMIPHONE]: HandleSIMPlugNotifyPNNFunc dual_sys=%d, notify_event=%d",dual_sys,notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        //no care
        break;
        
    //if PNN memory is not NULL,free them
    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        if (PNULL != pnn_info[dual_sys].opl_ptr )
        {
            SCI_FREE(pnn_info[dual_sys].opl_ptr);
        }
        if (PNULL != pnn_info[dual_sys].pnn_ptr )
        {
            SCI_FREE(pnn_info[dual_sys].pnn_ptr);
        }
#if defined(MMI_SIM_ICON_SUPPORT) 
        if (PNULL != pnn_info[dual_sys].pnni_ptr )
        {
            SCI_FREE(pnn_info[dual_sys].pnni_ptr);
        }
#endif        
        if (PNULL != pnn_info[dual_sys].infonum_ptr )
        {
            SCI_FREE(pnn_info[dual_sys].infonum_ptr);
        }
        break;

    default:
        break;
    }
}
#endif

/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_NETWORK_NAME_T* MMIPHONE_GetNetworkNameInfo(void)
{
    return s_network_name;
}

/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_PNN_T* MMIPHONE_GetPNNInfo(void)
{
    return s_pnn_network_name;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETNAME_READY_IND
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready index, get the SPN SPNI ONS .etc netname that can read derectly
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleNetNameReadyInd(
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE; 
    APP_MN_NETNAME_READY_IND_T netname_ready_ind = *((APP_MN_NETNAME_READY_IND_T *)param);
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();

    if(MMI_DUAL_SYS_MAX <= netname_ready_ind.dual_sys)
    {
        return result;
    }

    // 保存net name files ready flag
    network_name[netname_ready_ind.dual_sys].is_nw_name_files_ready= TRUE;
    //get SPN
    GetSPNContent(netname_ready_ind.dual_sys);
    //get SPN Icon
#if defined(MMI_SIM_ICON_SUPPORT) 
    GetSPNIContent(netname_ready_ind.dual_sys);
#endif

    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready flag, get the ONS .etc netname that can read derectly
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleCPHSReadyInd(
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE; 
    APP_MN_CPHS_READY_IND_T cphs_ready_ind = *((APP_MN_CPHS_READY_IND_T *)param);
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();

    if(MMI_DUAL_SYS_MAX <= cphs_ready_ind.dual_sys)
    {
        return result;
    }
    
    SCI_TRACE_LOW("[mmiphone]:HandleCPHSReadyInd  dual_sys =%d, is_cphs_exist = %d",
                     cphs_ready_ind.dual_sys, cphs_ready_ind.is_cphs_exist);

    // 保存CPHS ready flag
    network_name[cphs_ready_ind.dual_sys].is_cphs_exist = cphs_ready_ind.is_cphs_exist;
    //get Operator Network String if cphs exist
    if(network_name[cphs_ready_ind.dual_sys].is_cphs_exist)
    {
        GetONSContent(cphs_ready_ind.dual_sys);
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_RECORD_NUM_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready index
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMRecordNumCnf(
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE; 
    APP_MN_SIM_RECORD_NUM_CNF_T sim_record_num_cnf = *((APP_MN_SIM_RECORD_NUM_CNF_T *)param);
    MMIPHONE_PNN_T* pnn_network_name = MMIPHONE_GetPNNInfo();
    uint8  read_first_record_id = 1;//first id is not 0
    
    SCI_TRACE_LOW("[mmiphone]:HandleSIMRecordNumCnf  is_ok = %d, dual_sys =%d, max_num = %d, mn_file_name = %d",
                     sim_record_num_cnf.is_ok, sim_record_num_cnf.dual_sys, sim_record_num_cnf.max_num, sim_record_num_cnf.mn_file_name);

    if(MMI_DUAL_SYS_MAX <= sim_record_num_cnf.dual_sys)
    {
        return result;
    }
	
    // 保存PNN/OPL/PNNI num
    switch(sim_record_num_cnf.mn_file_name)
    {
    case MNSIM_PH2_OPL:
        if(0 < sim_record_num_cnf.max_num && sim_record_num_cnf.is_ok)//sim read ok
        {
            pnn_network_name[sim_record_num_cnf.dual_sys].is_opl_read_ok = sim_record_num_cnf.is_ok;
            pnn_network_name[sim_record_num_cnf.dual_sys].opl_num= sim_record_num_cnf.max_num;
            pnn_network_name[sim_record_num_cnf.dual_sys].is_opl_empty = FALSE;
            
            if(PNULL == pnn_network_name[sim_record_num_cnf.dual_sys].opl_ptr)
            {
                //alloc memory of the max_num size
                pnn_network_name[sim_record_num_cnf.dual_sys].opl_ptr = SCI_ALLOCAZ(sizeof(MN_SIM_OPL_ENTRY_T)*sim_record_num_cnf.max_num);
            }
            
            if(PNULL == pnn_network_name[sim_record_num_cnf.dual_sys].opl_ptr)
            {
                return result;
            }
            //get OPL items from first record ID
            GetPNNContent(sim_record_num_cnf.dual_sys, sim_record_num_cnf.mn_file_name, read_first_record_id);
        }
        else
        {
            pnn_network_name[sim_record_num_cnf.dual_sys].is_opl_empty = TRUE;
        }
        break;
        
    case MNSIM_PH2_PNN:
        if(0 < sim_record_num_cnf.max_num && sim_record_num_cnf.is_ok)//sim read ok
        {
            pnn_network_name[sim_record_num_cnf.dual_sys].pnn_num= sim_record_num_cnf.max_num;
            
            if(PNULL == pnn_network_name[sim_record_num_cnf.dual_sys].pnn_ptr)
            {
                //alloc memory of the max_num size
                pnn_network_name[sim_record_num_cnf.dual_sys].pnn_ptr = SCI_ALLOCAZ(sizeof(MN_APP_NET_NAME_INFO_T)*sim_record_num_cnf.max_num);
            }
            
            if(PNULL == pnn_network_name[sim_record_num_cnf.dual_sys].pnn_ptr)
            {
                return result;
            }

            //get  PNN items from first record ID
            GetPNNContent(sim_record_num_cnf.dual_sys, sim_record_num_cnf.mn_file_name, read_first_record_id);
        }
        break;
        
#if defined(MMI_SIM_ICON_SUPPORT) 
    case MNSIM_PH2_PNN_ICON:
        if(0 < sim_record_num_cnf.max_num && sim_record_num_cnf.is_ok)
        {
            pnn_network_name[sim_record_num_cnf.dual_sys].pnni_num= sim_record_num_cnf.max_num;
            
            if(PNULL == pnn_network_name[sim_record_num_cnf.dual_sys].pnni_ptr)
            {
                //alloc memory of the max_num size
                pnn_network_name[sim_record_num_cnf.dual_sys].pnni_ptr = SCI_ALLOCAZ(sizeof(MN_SIM_ICON_INFO_T)*sim_record_num_cnf.max_num);
            }
            
            if(PNULL == pnn_network_name[sim_record_num_cnf.dual_sys].pnni_ptr)
            {
                return result;
            }

            //get PNN ICON items from first record ID
            GetPNNContent(sim_record_num_cnf.dual_sys, sim_record_num_cnf.mn_file_name, read_first_record_id);
        }
        break;
#endif

    case MNSIM_PH2_INFONUM:
        if(0 < sim_record_num_cnf.max_num && sim_record_num_cnf.is_ok)
        {
            pnn_network_name[sim_record_num_cnf.dual_sys].infonum_num= sim_record_num_cnf.max_num;
            //TODO:all execution reserved in INFONUM
        }
        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_RECORD_READ_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:SIM record read confirm
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMRecordReadCnf(
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE; 
    MMIPHONE_PNN_T*  pnn_info =  MMIPHONE_GetPNNInfo();
    APP_MN_SIM_RECORD_READ_CNF_T read_pnn_cnf = *((APP_MN_SIM_RECORD_READ_CNF_T *)param);
    
    SCI_TRACE_LOW("[mmiphone]:HandleSIMRecordReadCnf  is_ok = %d, dual_sys =%d, record_id = %d, mn_file_name = %d",
                                 read_pnn_cnf.is_ok, read_pnn_cnf.dual_sys, read_pnn_cnf.record_id, read_pnn_cnf.mn_file_name);

    if(MMI_DUAL_SYS_MAX <= read_pnn_cnf.dual_sys)
    {
        return result;
    }
    
    //record_id is from 1
    switch(read_pnn_cnf.mn_file_name)
    {
    case MNSIM_PH2_OPL:
        if(PNULL != pnn_info[read_pnn_cnf.dual_sys].opl_ptr && read_pnn_cnf.is_ok)
        {
            // 保存OPL item
            if(pnn_info[read_pnn_cnf.dual_sys].opl_num >= read_pnn_cnf.record_id)
            {
                pnn_info[read_pnn_cnf.dual_sys].opl_ptr[read_pnn_cnf.record_id -1].lac_1 = read_pnn_cnf.buf.opl_entry.lac_1;
                pnn_info[read_pnn_cnf.dual_sys].opl_ptr[read_pnn_cnf.record_id -1].lac_2 = read_pnn_cnf.buf.opl_entry.lac_2;
                pnn_info[read_pnn_cnf.dual_sys].opl_ptr[read_pnn_cnf.record_id -1].pnn_index = read_pnn_cnf.buf.opl_entry.pnn_index;
                SCI_MEMCPY(&pnn_info[read_pnn_cnf.dual_sys].opl_ptr[read_pnn_cnf.record_id -1].plmn,
                                        &read_pnn_cnf.buf.opl_entry.plmn, 
                                        sizeof(MN_PLMN_T));
            }
            
            //read all opl item, record id is from 1
            if(read_pnn_cnf.record_id < pnn_info[read_pnn_cnf.dual_sys].opl_num)
            {
                pnn_info[read_pnn_cnf.dual_sys].is_opl_read_ok = FALSE;
                //get next OPL item
                GetPNNContent(read_pnn_cnf.dual_sys,read_pnn_cnf.mn_file_name, read_pnn_cnf.record_id + 1);
            }
            else
            {
                //read ok
                pnn_info[read_pnn_cnf.dual_sys].is_opl_read_ok = TRUE;
            }
        }
        else
        {
            pnn_info[read_pnn_cnf.dual_sys].is_opl_read_ok = FALSE;
        }
        break;

     case MNSIM_PH2_PNN:
        if(0 < pnn_info[read_pnn_cnf.dual_sys].pnn_num && read_pnn_cnf.is_ok)
        {
            if(pnn_info[read_pnn_cnf.dual_sys].pnn_num >= read_pnn_cnf.record_id)
            {
                // 保存PNN item
                pnn_info[read_pnn_cnf.dual_sys].pnn_ptr[read_pnn_cnf.record_id -1].is_full_name_flag = read_pnn_cnf.buf.pnn_entry.is_full_name_flag;
                pnn_info[read_pnn_cnf.dual_sys].pnn_ptr[read_pnn_cnf.record_id -1].is_short_name_flag = read_pnn_cnf.buf.pnn_entry.is_short_name_flag;
                if(pnn_info[read_pnn_cnf.dual_sys].pnn_ptr[read_pnn_cnf.record_id -1].is_full_name_flag)
                {
                    SCI_MEMCPY(&pnn_info[read_pnn_cnf.dual_sys].pnn_ptr[read_pnn_cnf.record_id -1].full_nt_name,
                                            &read_pnn_cnf.buf.pnn_entry.full_nt_name, 
                                            sizeof(MN_NETWORK_NAME_T));
                }
                
                if(pnn_info[read_pnn_cnf.dual_sys].pnn_ptr[read_pnn_cnf.record_id -1].is_short_name_flag)
                {
                    SCI_MEMCPY(&pnn_info[read_pnn_cnf.dual_sys].pnn_ptr[read_pnn_cnf.record_id -1].short_nt_name,
                                            &read_pnn_cnf.buf.pnn_entry.short_nt_name, 
                                            sizeof(MN_NETWORK_NAME_T));
                }
            }
            
            if(read_pnn_cnf.record_id < pnn_info[read_pnn_cnf.dual_sys].pnn_num)
            {
                pnn_info[read_pnn_cnf.dual_sys].is_pnn_read_ok = FALSE;
                //get next PNN item
                GetPNNContent(read_pnn_cnf.dual_sys,read_pnn_cnf.mn_file_name, read_pnn_cnf.record_id + 1);
            }
            else
            {
                PHONE_SERVICE_STATUS_T* cur_service_status = MMIAPIPHONE_GetServiceStatus();
                
                //read ok
                pnn_info[read_pnn_cnf.dual_sys].is_pnn_read_ok = TRUE;
                //if read ok, set PNN 
                MMIPHONE_SetPNNWithLac(read_pnn_cnf.dual_sys, 
                                                                cur_service_status[read_pnn_cnf.dual_sys].plmn, 
                                                                cur_service_status[read_pnn_cnf.dual_sys].lac);
            }
        }
        else
        {
            pnn_info[read_pnn_cnf.dual_sys].is_pnn_read_ok = FALSE;
        }
        break;

#if defined(MMI_SIM_ICON_SUPPORT) 
    case MNSIM_PH2_PNN_ICON:
        if(0 < pnn_info[read_pnn_cnf.dual_sys].pnni_num && read_pnn_cnf.is_ok)
        {
            if(pnn_info[read_pnn_cnf.dual_sys].pnn_num >= read_pnn_cnf.record_id)
            {
                pnn_info[read_pnn_cnf.dual_sys].pnni_ptr[read_pnn_cnf.record_id -1].icon_tag = read_pnn_cnf.buf.pnni_entry.icon_tag;
                pnn_info[read_pnn_cnf.dual_sys].pnni_ptr[read_pnn_cnf.record_id -1].qualifier = read_pnn_cnf.buf.pnni_entry.qualifier;
                SCI_MEMCPY(&pnn_info[read_pnn_cnf.dual_sys].pnni_ptr[read_pnn_cnf.record_id -1].icon_link,
                                        &read_pnn_cnf.buf.pnni_entry.icon_link, 
                                        sizeof(MN_SIM_ICON_LINK_T));
            }

             if(read_pnn_cnf.record_id < pnn_info[read_pnn_cnf.dual_sys].pnni_num)
            {
                pnn_info[read_pnn_cnf.dual_sys].is_pnni_read_ok = FALSE;
                //get next PNN ICON item
                GetPNNContent(read_pnn_cnf.dual_sys,read_pnn_cnf.mn_file_name, read_pnn_cnf.record_id + 1);
            }
            else
            {
                //read ok
                pnn_info[read_pnn_cnf.dual_sys].is_pnni_read_ok = TRUE;
            }
        }
        else
        {
            pnn_info[read_pnn_cnf.dual_sys].is_pnni_read_ok = FALSE;
        }
        break;
#endif

    case MNSIM_PH2_INFONUM:
        if(PNULL != pnn_info[read_pnn_cnf.dual_sys].infonum_ptr&& read_pnn_cnf.is_ok )
        {
            //TODO:all execution reserved in INFONUM
        }
        else
        {
            pnn_info[read_pnn_cnf.dual_sys].is_infonum_read_ok = FALSE;
        }
        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_BINARY_READ_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:read SPNI confirm
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMBinaryReadCnf(
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE; 
    APP_MN_SIM_BINARY_READ_CNF_T spni_cnf = *((APP_MN_SIM_BINARY_READ_CNF_T *)param);
#if defined(MMI_SIM_ICON_SUPPORT) 
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
#endif

    if(MMI_DUAL_SYS_MAX <= spni_cnf.dual_sys)
    {
        return result;
    }
    
    SCI_TRACE_LOW("[mmiphone]:HandleSIMBinaryReadCnf  is_ok = %d, dual_sys =%d",spni_cnf.is_ok, spni_cnf.dual_sys);
    
#if defined(MMI_SIM_ICON_SUPPORT) 
    network_name[spni_cnf.dual_sys].is_spni_ok = spni_cnf.is_ok;
    // 保存SPNI content
    if(network_name[spni_cnf.dual_sys].is_spni_ok)
    {
        SCI_MEMCPY(&network_name[spni_cnf.dual_sys].spni, &spni_cnf.buf.spni, sizeof(MN_SIM_ICON_INFO_T));
    }
#endif

    return (result);
}

/*****************************************************************************/
//  Description :Get Curr SIM PLMN Network Name Num
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is an asynchronism function
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetPNNum(
                           MN_DUAL_SYS_E    dual_sys
                           )
{
    BOOLEAN result  = FALSE;
    MNSIM_SERVICE_TYPE_PH2_E mn_file_name[] =
    {
        MNSIM_PH2_OPL,//first get OPL max_num
        MNSIM_PH2_PNN,
        MNSIM_PH2_PNN_ICON,
        MNSIM_PH2_INFONUM
    };
    uint8  i = 0;
    uint8  total_sim_file_num = 0;
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return result;
    }
    
    total_sim_file_num = sizeof(mn_file_name)/sizeof(MNSIM_SERVICE_TYPE_PH2_E);
    
    for(i = 0; i < total_sim_file_num; i++)
    {
        if(ERR_MNDATAMAG_NO_ERR == MNSIM_GetSimFileRecordNumEx(dual_sys,mn_file_name[i]))
        {
            result = TRUE;
        }

    }
    
    return (result);
}

/*****************************************************************************/
//  Description :Get Curr SIM PLMN Network Name Num
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is an asynchronism function
/*****************************************************************************/
LOCAL BOOLEAN GetPNNContent(
                            MN_DUAL_SYS_E    dual_sys,
                            MNSIM_SERVICE_TYPE_PH2_E mn_file_name,
                            uint8  record_id
                            )
{
    BOOLEAN result  = FALSE;
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return result;
    }
#ifndef UAL_TELE_SUPPORT
    if(ERR_MNDATAMAG_NO_ERR == MNSIM_GetSimFileRecordEntryEx(dual_sys,mn_file_name,record_id))
    {
        result = TRUE;
    }
#endif
    return (result);
}

/*****************************************************************************/
//  Description :Get Curr SPN content
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is a synchronization function
/*****************************************************************************/
LOCAL BOOLEAN GetSPNContent(
                            MN_DUAL_SYS_E    dual_sys
                            )
{
    BOOLEAN result  = FALSE;
    MNSIM_SPN_T spn  = {0};
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return result;
    }

    if (MN_RETURN_SUCCESS == MNSIM_GetSPNEx(dual_sys,&spn))
    {
        uint8 spn_len = MN_SPN_ID_NUM_LEN;

        //get length and real SPN str
        MMIAPICOM_ConvSIMStr2PhoneAlpha((uint8*)spn.id_num,&spn_len, MN_SPN_ID_NUM_LEN);
        SCI_TRACE_LOW("GetSPNContent display_condition = %d, spn_len = %d, id_num[0] =%x", 
                                        spn.display_condition, spn_len, spn.id_num[0]);
        
        network_name[dual_sys].display_condition = spn.display_condition;
        network_name[dual_sys].spn_len= spn_len;
        SCI_MEMSET(network_name[dual_sys].spn, 0, MN_SPN_ID_NUM_LEN);
        
        if((0 < spn_len) && ((MN_SPN_ID_NUM_LEN + 1) > spn_len) && (NULL != spn.id_num))//Bug 350354
        {
            network_name[dual_sys].is_spn_supported = TRUE;
            SCI_MEMCPY(network_name[dual_sys].spn, spn.id_num, MN_SPN_ID_NUM_LEN);
        }
        else
        {
            network_name[dual_sys].is_spn_supported = FALSE;
            SCI_MEMSET(network_name[dual_sys].spn, 0, MN_SPN_ID_NUM_LEN);
        }
        
        result = TRUE;
    }
    
    return (result);
}

#if defined(MMI_SIM_ICON_SUPPORT) 
/*****************************************************************************/
//  Description :Get Curr SPN Icon content
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is an asynchronism function
/*****************************************************************************/
LOCAL BOOLEAN GetSPNIContent(
                            MN_DUAL_SYS_E    dual_sys
                            )
{
    BOOLEAN result  = FALSE;
    MNSIM_SERVICE_TYPE_PH2_E mn_file_name = MNSIM_PH2_SPN_ICON;
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return result;
    }
    
    if(ERR_MNDATAMAG_NO_ERR == MNSIM_GetSimBinaryFileEx(dual_sys,mn_file_name))
    {
        result = TRUE;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description :Get Curr SIM ONS content
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:ons is operator name  string "6F14"Or "6F18"
//          this is a synchronization function
/*****************************************************************************/
LOCAL BOOLEAN GetONSContent(
                            MN_DUAL_SYS_E    dual_sys
                            )
{
    BOOLEAN result  = FALSE;
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
    MN_OPER_NAME_T  ons_string = {0};

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return result;
    }
    //get ONS
    if(ERR_MNDATAMAG_NO_ERR == MNSIM_GetOperatorNameEx(dual_sys,&ons_string))
    {
        uint16 ons_len = 0;
        
        network_name[dual_sys].ons_dcs = MN_OPN_DCS_8BIT;
        ons_len = SCI_STRLEN((char*)ons_string.buf);
        network_name[dual_sys].ons_len  = MIN(ons_len, ons_string.length);
#ifdef WIN32    //show on Simulator
    {
        uint8 offset = 0;
        uint8 pnn_string_8bit[] = {"SPRDPNN"};
        uint8 pnn_string_7bit[8] = {0};
        int16 pnn_len = 8;

        SCI_Pack8bitsTo7bits((uint8 *)pnn_string_8bit,
                                                7, 
                                                (uint8 *)pnn_string_7bit,
                                                (int16 *)&pnn_len, 
                                                &offset);
    }
#else
#endif
        SCI_MEMSET(network_name[dual_sys].ons, 0x00, MAX_ONSHORTFORM_LEN);
        SCI_MEMCPY(network_name[dual_sys].ons, ons_string.buf, network_name[dual_sys].ons_len);
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description :use OPL to get PNN index 
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:select PNN index from operator PLMN list
/*****************************************************************************/
LOCAL uint8  GetPNNIndexByOPL(
                           MN_DUAL_SYS_E    dual_sys,
                           MN_PLMN_T           cur_plmn,
                           uint16  lac
                           )
{
    MMIPHONE_PNN_T* pnn_info =  MMIPHONE_GetPNNInfo();
    uint8  pnn_index = 0;
    uint8  i = 0;
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return pnn_index;
    }
    
    if(0 == pnn_info[dual_sys].opl_num)
    {
        return pnn_index;
    }
    //pnn record id from 1
    for(i = 0; i < pnn_info[dual_sys].opl_num; i++)
    {
        //is PLMN same
        if((pnn_info[dual_sys].opl_ptr[i].plmn.mcc == cur_plmn.mcc)
        &&(pnn_info[dual_sys].opl_ptr[i].plmn.mnc == cur_plmn.mnc)
        &&(pnn_info[dual_sys].opl_ptr[i].plmn.mnc_digit_num == cur_plmn.mnc_digit_num))
        {
            //is lac in the lac_1~lac_2 rank OR lac== lac_1== lac_2
            if((lac > pnn_info[dual_sys].opl_ptr[i].lac_1 && lac < pnn_info[dual_sys].opl_ptr[i].lac_2)
                ||(lac == pnn_info[dual_sys].opl_ptr[i].lac_1 && lac == pnn_info[dual_sys].opl_ptr[i].lac_2))
            {
                pnn_index = pnn_info[dual_sys].opl_ptr[i].pnn_index;
                break;
            }
        }
    }
    
    SCI_TRACE_LOW("[mmiphone]:GetPNNIndexByOPL pnn_index = %d, dual_sys = %d, lac = %d, lac_1 = %d, lac_2 =%d", pnn_index, lac, dual_sys, pnn_info[dual_sys].opl_ptr[i].lac_1, pnn_info[dual_sys].opl_ptr[i].lac_2);

    return (pnn_index);
}

/*****************************************************************************/
//  Description : set the PNN/PNNI  in to MMI
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetPLMNNetworkName(
                                                MN_DUAL_SYS_E   dual_sys,       //IN:
                                                MN_SIM_RECORD_FILE_BUF_U  *record_file_buf,  //in:
                                                MNSIM_SERVICE_TYPE_PH2_E  mn_file_name    //IN
                                                )
{
    BOOLEAN  result = TRUE;
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return result;
    }
    if(PNULL == record_file_buf)
    {
        return result;
    }
    
    switch(mn_file_name)
    {
    case MNSIM_PH2_PNN:
        /*The MS shall be free to choose one of these names depending upon its own characteristics and/or limitations, 
            such as those of its display*/
        if (record_file_buf->pnn_entry.is_full_name_flag)
        {
            //if (record_file_buf.pnn_entry.full_nt_name.network_name_exist)    
            {
                network_name[dual_sys].pnn_dcs = (MN_OPN_DCS_E)record_file_buf->pnn_entry.full_nt_name.code_scheme;
                network_name[dual_sys].pnn_len = MIN(record_file_buf->pnn_entry.full_nt_name.length,MAX_ONSHORTFORM_LEN);
                network_name[dual_sys].is_add_ci = record_file_buf->pnn_entry.full_nt_name.add_ci;
                SCI_MEMCPY(network_name[dual_sys].pnn, 
                                        record_file_buf->pnn_entry.full_nt_name.name, 
                                        network_name[dual_sys].pnn_len);
            }
        }
        else if (record_file_buf->pnn_entry.is_short_name_flag)
        {
            //if (record_file_buf.pnn_entry.short_nt_name.network_name_exist) //L4 not set
            {
                network_name[dual_sys].pnn_dcs = (MN_OPN_DCS_E)record_file_buf->pnn_entry.short_nt_name.code_scheme;
                network_name[dual_sys].pnn_len = MIN(record_file_buf->pnn_entry.short_nt_name.length,MAX_ONSHORTFORM_LEN);
                network_name[dual_sys].is_add_ci = record_file_buf->pnn_entry.short_nt_name.add_ci;
                SCI_MEMCPY(network_name[dual_sys].pnn, 
                                        record_file_buf->pnn_entry.short_nt_name.name, 
                                        network_name[dual_sys].pnn_len);
            }
        }
        else 
        {
            network_name[dual_sys].pnn_dcs = 0;
            network_name[dual_sys].pnn_len = 0;
            network_name[dual_sys].is_add_ci = FALSE;
            SCI_MEMSET(network_name[dual_sys].pnn, 0, MAX_ONSHORTFORM_LEN);
            result = FALSE;
        }
        break;

#if defined(MMI_SIM_ICON_SUPPORT) 
    case MNSIM_PH2_PNN_ICON:
        SCI_MEMCPY(&network_name[dual_sys].pnni, 
                                &record_file_buf->pnni_entry, 
                                sizeof(MN_SIM_ICON_INFO_T));
        break;
#endif
    default:
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set the PNN/PNNI  in to MMI
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_SetPNNWithLac(
                                                MN_DUAL_SYS_E   dual_sys,       //IN:
                                                MN_PLMN_T cur_plmn,                 //IN
                                                uint16  lac         //IN
                                                )
{
    BOOLEAN  result = TRUE;
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
    MMIPHONE_PNN_T* pnn_info =  MMIPHONE_GetPNNInfo();
    uint8 pnn_index = 0;    //record id is from 1
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return result;
    }
    //pnn is null, return 
    if(0 ==  pnn_info[dual_sys].pnn_num)
    {
        return result;
    }
    
    SCI_TRACE_LOW("[mmiphone]:SetPNNWithLac pnn_num = %d, lac = %d, network_name[ %d ].lac = %d", pnn_info[dual_sys].pnn_num, lac, dual_sys, network_name[dual_sys].lac);
    //is lac available and changed
    if(0 != lac && (network_name[dual_sys].lac != lac))
    {
        //record id is from 1, so -1 when use it
        if(pnn_info[dual_sys].is_opl_empty &&(0 < pnn_info[dual_sys].pnn_num))
        {
            pnn_index = 1; //opl is null, use default record id is 1
        }
        else
        {
            pnn_index = GetPNNIndexByOPL(dual_sys, cur_plmn, lac);
        }
        
        //pnn index is diff use new pnn_index
        if(pnn_index != 0 &&(pnn_info[dual_sys].curr_pnn_index != pnn_index) 
            && (pnn_index <= pnn_info[dual_sys].pnn_num))
        {
            MN_SIM_RECORD_FILE_BUF_U pnn_sim_record_buf ={0};
            //is pnn ok
            if(pnn_info[dual_sys].is_pnn_read_ok && (PNULL != pnn_info[dual_sys].pnn_ptr))
            {
                SCI_MEMCPY(&pnn_sim_record_buf.pnn_entry, 
                                        &pnn_info[dual_sys].pnn_ptr[pnn_index - 1],
                                        sizeof(MN_APP_NET_NAME_INFO_T));
                
                if(SetPLMNNetworkName(dual_sys, &pnn_sim_record_buf, MNSIM_PH2_PNN))
                {
                    //set idle string
                    MAIN_SetIdleNetWorkName();
                    //remember cur LAC(location area code)
                    network_name[dual_sys].lac = lac;
                    //remember current PNN index, in case update in high frequency
                    pnn_info[dual_sys].curr_pnn_index = pnn_index;
                }
            }

            //set pnn icon
#if defined(MMI_SIM_ICON_SUPPORT) 
            if(pnn_info[dual_sys].is_pnni_read_ok && (PNULL != pnn_info[dual_sys].pnni_ptr))
            {
                SCI_MEMCPY(&pnn_sim_record_buf.pnni_entry, 
                                        &pnn_info[dual_sys].pnni_ptr[pnn_index - 1],
                                        sizeof(MN_SIM_ICON_INFO_T));
                
                if(SetPLMNNetworkName(dual_sys, &pnn_sim_record_buf, MNSIM_PH2_PNN_ICON))
                {
                    //set idle string
                    MAIN_SetIdleNetWorkName();
                    //remember cur LAC(location area code)
                    network_name[dual_sys].lac = lac;
                    //remember current PNN index, in case update in high frequency
                    pnn_info[dual_sys].curr_pnn_index = pnn_index;
                }
            }
#endif
        }
    }

    
    return (result);
}
PUBLIC void MMIAPIPHONE_SyncSimSPN(MN_DUAL_SYS_E dual_sys)
{
    if (!GetSPNContent(dual_sys))
    {
        SCI_TRACE_LOW("MMIAPIPHONE_GetSimSPN failed.");
    }
}
/*****************************************************************************/
//  Description :Is Curr Service Plmn In Spdi
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:if service PLMN is in spdi, SPN should display when roaming 
/*****************************************************************************/
LOCAL BOOLEAN IsServicePlmnInSpdi(
                           MN_DUAL_SYS_E    dual_sys,
                           MN_PLMN_T           cur_plmn
                           )
{
    MNSIM_SPDI_T   spdi_map = {0};//cid601020
    BOOLEAN result  = FALSE;
    uint8  i = 0;

    MNSIM_GetSPDIEx(dual_sys, &spdi_map);
    
    if(0 == spdi_map.num)
    {
        return result;
    }
    
    for(i = 0; i < spdi_map.num; i++)
    {
        if(spdi_map.plmn_arr[i].present)
        {
            if((spdi_map.plmn_arr[i].plmn.mcc == cur_plmn.mcc)
            &&(spdi_map.plmn_arr[i].plmn.mnc == cur_plmn.mnc)
            &&(spdi_map.plmn_arr[i].plmn.mnc_digit_num == cur_plmn.mnc_digit_num))
            {
                result = TRUE;
                break;
            }
        }
    }
    
    SCI_TRACE_LOW("[mmiphone]:IsServicePlmnInSpdi spdi_map.num = %d result = %d", spdi_map.num, result);
    
    return (result);
}

/*****************************************************************************/
//  Description :Convert OPN Content To Wchar 
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:Convert OPN String , priority is PNN-NITZ-ONS(CPHS)-Default
/*****************************************************************************/
LOCAL uint16 ConvertOPNToWchar(                                           //RETURN: 
                                    MN_OPN_DCS_E    opn_dcs,   //IN: string type
                                    uint8   opn_len,    //IN
                                    uint8* opn,     //IN
                                    wchar* opn_string    //OUT
                                    )  
{
    uint16   string_len = 0;
    
    if(PNULL == opn  || PNULL == opn_string)
    {
        return string_len;
    }
    
    switch (opn_dcs)
    {
    case MN_OPN_DCS_UCS2:
        {
            //UCS2字符格式
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            MMI_WSTRNCPY(
                opn_string,
                MAX_ONSHORTFORM_LEN,
                (wchar *)opn,
                MAX_ONSHORTFORM_LEN,
                opn_len/sizeof(wchar)
                );
#else
            GUI_UCS2L2UCS2B((uint8 *)opn_string,
                                        opn_len,
                                        (uint8 *)opn,
                                        opn_len);
#endif        

            string_len = opn_len/sizeof(wchar);
        }
        break;
        
    default:
        //8 Bit 标准 Ascii 字符格式
        string_len = MMIAPICOM_Default2Wchar(opn, opn_string, opn_len);
        break;
    }
    
    return (string_len);
}

/*****************************************************************************/
//  Description :Select OPN String
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:select OPN String from PNN-ONS(CPHS)-NITZ-Default
/*****************************************************************************/
LOCAL void SelectOPNString(
                         MN_DUAL_SYS_E dual_sys,
                         MMI_STRING_T* opn_string
                         )
{
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
    PHONE_SERVICE_STATUS_T* cur_service_status = MMIAPIPHONE_GetServiceStatus();

    
    if(PNULL == opn_string)
    {
        return;
    }
    
    SCI_TRACE_LOW("[mmiphone]:SelectOPNString dual_sys = %d, pnn_len = %d, ons_len = %d, opn_len = %d, isRoaming =%d",
                                    dual_sys, network_name[dual_sys].pnn_len, network_name[dual_sys].ons_len,
                                    network_name[dual_sys].opn_len, cur_service_status[dual_sys].isRoaming);
    
    if(0 < network_name[dual_sys].pnn_len) //PLMN network name
    {
        opn_string->wstr_len = ConvertOPNToWchar(
                                                        network_name[dual_sys].pnn_dcs,
                                                        network_name[dual_sys].pnn_len,
                                                        (uint8*) network_name[dual_sys].pnn,
                                                        opn_string->wstr_ptr);
    }
    else if(0 < network_name[dual_sys].ons_len && !cur_service_status[dual_sys].isRoaming) //ONS from CPHS, behind R4
    {
        opn_string->wstr_len = ConvertOPNToWchar(
                                                        network_name[dual_sys].ons_dcs,
                                                        network_name[dual_sys].ons_len,
                                                        (uint8*) network_name[dual_sys].ons,
                                                        opn_string->wstr_ptr);
    }
    else if(0 < network_name[dual_sys].opn_len) //NITZ
    {
        opn_string->wstr_len = ConvertOPNToWchar(
                                                        network_name[dual_sys].opn_dcs,
                                                        network_name[dual_sys].opn_len,
                                                        (uint8*) network_name[dual_sys].opn,
                                                        opn_string->wstr_ptr);
    }
    else     //use default
    {
        opn_string->wstr_len = MAX_ONSHORTFORM_LEN;//MMIPHONE_MAX_OPER_NAME_LEN

        MMIAPIPHONE_GenPLMNDisplay(
                                        MMIAPIPHONE_GetTDOrGsm(dual_sys),
                                        &cur_service_status[dual_sys].plmn,
                                        opn_string->wstr_ptr,
                                        &opn_string->wstr_len,
                                        FALSE);
    }
}

/*****************************************************************************/
//  Description : Get Network Name String
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:network name string include ACI SPN OPN three part
/*****************************************************************************/
PUBLIC void MMIPHONE_GetNetworkNameString( 
                                          MN_DUAL_SYS_E   dual_sys,
                                          MMI_STRING_T  *  network_name_ptr
                                          )
{
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
    PHONE_SERVICE_STATUS_T* cur_service_status = MMIAPIPHONE_GetServiceStatus();
    MMISET_LANGUAGE_TYPE_E  language_type;  
    uint8  display_condition  = 0;
    MMI_STRING_T aci_string = {0}; 
    MMI_STRING_T opn_string = {0};
    MMI_STRING_T spn_string = {0};
    wchar aci[MAX_ACI_INFO_LEN + 1] = {0};
    wchar opn[MAX_ONSHORTFORM_LEN + 1] = {0};
    wchar spn[MN_SPN_ID_NUM_LEN + 1] = {0};
    
    if (PNULL == network_name_ptr || PNULL == network_name_ptr->wstr_ptr)
    {
        return;
    }
    
	MMIAPISET_GetLanguageType(&language_type);
    display_condition  = network_name[dual_sys].display_condition;
    
    SCI_TRACE_LOW("[mmiphone]:GetNetworkNameString dual_sys = %d, is_aci = %d, is_spn_support = %d, spn_len = %d, display_condition = %d, isRoaming =%d",
            dual_sys, network_name[dual_sys].is_add_ci, network_name[dual_sys].is_spn_supported,
            network_name[dual_sys].spn_len,display_condition,cur_service_status[dual_sys].isRoaming);

    SCI_TRACE_LOW("[mmiphone]:GetNetworkNameString dual_sys = %d, pnn_len = %d, ons_len = %d, opn_len = %d, isRoaming =%d",
                                    dual_sys, network_name[dual_sys].pnn_len, network_name[dual_sys].ons_len,
                                    network_name[dual_sys].opn_len, cur_service_status[dual_sys].isRoaming);
    //get aci string
    if (network_name[dual_sys].is_add_ci)
    {
        aci_string.wstr_ptr = aci;
        MMIPHONE_GetAciString(&cur_service_status[dual_sys], &aci_string, MAX_ACI_INFO_LEN);
    }
    
    opn_string.wstr_ptr = (wchar *)opn;
    spn_string.wstr_ptr = (wchar *)spn;
    
    if (network_name[dual_sys].is_spn_supported && (0 < network_name[dual_sys].spn_len)
        && (!(cur_service_status[dual_sys].isRoaming) || (IsServicePlmnInSpdi(dual_sys, cur_service_status[dual_sys].plmn))))
    {
        // (i) When registered on the HPLMN, or one of the PLMN Identifications used for Service Provider Name display
        //  The SP Name shall be displayed;(TS 22101-910)
        MMIAPICOM_ParseSimStr(network_name[dual_sys].spn, 
                                                        network_name[dual_sys].spn_len,
                                                        &spn_string,
                                                        MN_SPN_ID_NUM_LEN);

        // (ii) Display of the PLMN Name is an operator's option by setting the appropriate fields in the USIM
        // (i.e. the Service Provider name shall be displayed either in parallel to the PLMN Name or instead of
        // the PLMN Name). display OPN by display_condition
       /* if((0 < network_name[dual_sys].spn_len) && (0 == (display_condition & 0x01)))
        {
            //display_condition bit1 = 0:display of registered PLMN NOT Required when registered PLMN is either HPLMN 
            //or a PLMN in the Service Provider PLMNN List (see EFSPDI)(TS 51011-4f0)
            //OPN stay NONE
        }
        else*/
        {
            //display_condition bit1 = 1: display of registered PLMN required when registered PLMN is either HPLMN 
            //or a PLMN in the Service Provider PLMN List
            if ((0 < network_name[dual_sys].pnn_len) || (0 < network_name[dual_sys].opn_len) || (0 < network_name[dual_sys].ons_len) )//bug 211640
            {
                SelectOPNString(dual_sys, &opn_string);

            }
        }
    }
    else
    {
        // (i)  When registered on neither the HPLMN, nor one of the PLMN Identifications used for Service Provider Name display:
        // The PLMN name(OPN) shall be displayed;
        SelectOPNString(dual_sys, &opn_string);

        // (ii) Display of the SP Name is an operator's option by setting the appropriate fields in the USIM.
        if((network_name[dual_sys].is_spn_supported ) && (0 == (display_condition & 0x02)))
        {
            //display_condition bit2 = 0: display of the service provider name is Required when registered PLMN is neither HPLMN nor a PLMN
            // in the service provider PLMN list.
            MMIAPICOM_ParseSimStr(network_name[dual_sys].spn, 
                                                        network_name[dual_sys].spn_len,
                                                        &spn_string,
                                                        MN_SPN_ID_NUM_LEN);
        }
        else
        {
            //display_condition bit2=1: display of the service provider name is NOT Required when registered PLMN is neither HPLMN 
            //nor a PLMN in the service provider PLMN list(see EFSPDI).
            //SPN stay NONE
        }
    }

    if (((460 == cur_service_status[dual_sys].plmn.mcc) &&
        (
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
            (MMISET_LANGUAGE_SIMP_CHINESE == language_type) ||
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
            (MMISET_LANGUAGE_TRAD_CHINESE == language_type) ||
#endif
         ((11 == cur_service_status[dual_sys].plmn.mnc) && (MMISET_LANGUAGE_ENGLISH == language_type)))))
    {
        network_name_ptr->wstr_len = MMIPHONE_MAX_OPER_NAME_LEN;
        MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys),
                                   &cur_service_status[dual_sys].plmn,
                                   network_name_ptr->wstr_ptr,
                                   &network_name_ptr->wstr_len,
                                   FALSE);
    }
    else 
    {
        MMIPHONE_CatAciOpnSpnString(&aci_string, &opn_string, &spn_string, network_name_ptr);
    }   
    
}

void TransformGidToStr(uint8       *gid_ptr,         // in the BCD array
                       int32        gid_length,      // in the length of BCD array
                       uint8        *tel_str,        // out the telephone number string
                       uint8       *tel_str_len_ptr  // out, the telephone number length
                       )
{
    int32        i, j;
    uint8        temp;
    uint8       str_len = 0;

    if (MN_SPN_ID_NUM_LEN < gid_length)
    {
        gid_length = MN_SPN_ID_NUM_LEN;
    }

    for(i = 0; i < gid_length; i++)
    {
        if (0xff == gid_ptr[i])
        {
            break;
        }
    
        for(j = 0; j < 2; j++)
        {
            if(0 == j)
            {
                temp = ((*gid_ptr & 0xf0) >> 4);
            }
            else
            {
                temp = (uint8)(*gid_ptr & 0x0f);
            }

            str_len++;

            switch(temp)
            {
                case 10:
                    *tel_str++ = 'A';
                    break;
                case 11:
                    *tel_str++ = 'B';
                    break;
                case 12:
                    *tel_str++ = 'C';
                    break;
                case 13:
                    *tel_str++ = 'D';
                    break;
                case 14:
                    *tel_str++ = 'E';
                    break;
                case 15:
                    *tel_str++ = 'F';
                    break;
                default:
                    *tel_str++ = (uint8)(temp + '0');
                    break;
            }
        }

        gid_ptr++;
    }

    if(NULL != tel_str_len_ptr)
    {
        *tel_str_len_ptr = str_len;
    }

    return;
}

#if defined (MODEM_SUPPORT_SPRD)
PUBLIC BOOLEAN MMIPHONE_CheckGIDString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_gid_string)
{
    BOOLEAN is_same_gid = FALSE;
    uint8   *gid1 = NULL;
    char gid_origin[2*MNSIM_GID_CONTENT_LEN + 1] = {0};
    char tstr[2*MNSIM_GID_CONTENT_LEN + 1] = {0};
    uint8 origin_len = 0;
    uint8 tstr_len = 0;
    uint8 gid1_len = 0;

    //获取GID参数接口需要再次确认，避免异步时差影响apn匹配。
    #ifndef WIN32
    gid1 = (uint8 *)SCI_ALLOC(sizeof(uint8) * MNSIM_GID_CONTENT_LEN);
    if (PNULL == gid1)
    {
        return FALSE;
    }
    if (ERR_MNDATAMAG_NO_ERR != MNSIM_GetGidEx(dual_sys, MNSIM_GID1, gid1, &gid1_len))
    {
        SCI_FREE(gid1);
        return FALSE;
    }
    #endif
    if ((0 == gid1_len) || (PNULL == res_gid_string->wstr_ptr) || (0 == res_gid_string->wstr_len))
    {
        SCI_FREE(gid1);
        return FALSE;
    }

    MMIAPICOM_WstrToStr(res_gid_string->wstr_ptr, gid_origin);
    origin_len = strlen(gid_origin);
    TransformGidToStr(gid1, gid1_len, tstr, &tstr_len);
    SCI_TRACE_LOW("gid1= %x, gid1_len=%d",gid1, gid1_len);
    SCI_TRACE_LOW("gid_origin: %s, origin_len=%d, gid.number: %s, real gid.len=%d", gid_origin, origin_len, tstr, tstr_len);
    #ifndef WIN32
    if (0 == strncasecmp(tstr, gid_origin, origin_len))
    {
        is_same_gid = TRUE;
    }
    else
    {
        is_same_gid = FALSE;
    }
    #endif

    SCI_FREE(gid1);

    return is_same_gid;
}
#elif defined (MODEM_SUPPORT_RDA)
PUBLIC BOOLEAN MMIPHONE_CheckGIDString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_gid_string)
{
    BOOLEAN is_same_gid = FALSE;
    MN_SIM_EF_MAPPING_GID_T gid1 = {0};
    char gid_origin[64] = {0};
    char tstr[64] = {0};
    uint8 origin_len = 0;
    uint8 tstr_len = 0;
    
    MNSIM_GetGidData(dual_sys, MNSIM_GID1, &gid1);
    if ((0 == gid1.len) || (PNULL == res_gid_string->wstr_ptr) || (0 == res_gid_string->wstr_len))
    {
        return FALSE;
    }

    SCI_TRACE_BUF("[szp]gid1 tele_num.number:", gid1.gid, gid1.len);
    MMIAPICOM_WstrToStr(res_gid_string->wstr_ptr, gid_origin);
    origin_len = strlen(gid_origin);
    TransformGidToStr(gid1.gid, gid1.len, tstr, &tstr_len);
    SCI_TRACE_LOW("[szp]gid_origin: %s, origin_len=%d, gid.number: %s, real gid.len=%d", gid_origin, origin_len, tstr, tstr_len);
    if ((origin_len == tstr_len) && (0 == strncasecmp(tstr, gid_origin, tstr_len)))
    {
        is_same_gid = TRUE;
    }
    else
    {
        is_same_gid = FALSE;
    }

    return is_same_gid;
}

#endif
PUBLIC BOOLEAN MMIPHONE_CheckPNNString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_pnn_string)
{
    BOOLEAN is_same_pnn = FALSE;
    MMI_STRING_T pnn_string = {0};
    wchar pnn[MN_SPN_ID_NUM_LEN + 1] = {0};
    MMIPHONE_PNN_T* pnn_info =  MMIPHONE_GetPNNInfo();
    
    pnn_string.wstr_ptr = (wchar *)pnn;
    if (NULL != pnn_info[dual_sys].pnn_ptr)
    {
        if (TRUE == pnn_info[dual_sys].pnn_ptr->is_full_name_flag)
        {
            MMIAPICOM_StrToWstr(pnn_info[dual_sys].pnn_ptr->full_nt_name.name, pnn_string.wstr_ptr);
            pnn_string.wstr_len = pnn_info[dual_sys].pnn_ptr->full_nt_name.length;
        }
        else if (TRUE == pnn_info[dual_sys].pnn_ptr->is_short_name_flag)
        {
            MMIAPICOM_StrToWstr(pnn_info[dual_sys].pnn_ptr->short_nt_name.name, pnn_string.wstr_ptr);
            pnn_string.wstr_len = pnn_info[dual_sys].pnn_ptr->short_nt_name.length;
        }
        
        if (TRUE == MMIAPICOM_StrSmartCompare(&pnn_string, res_pnn_string, TRUE, TRUE))
        {
            is_same_pnn = TRUE;
        }
        else
        {
            is_same_pnn = FALSE;
        }
    }
    
    return is_same_pnn;
}

PUBLIC BOOLEAN MMIPHONE_CheckIMSIString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_imsi_string)
{
    //需要考虑xx以及-,范围check
    MN_IMSI_T imsi = {0};
    char tstr[64] = {0};
    char imsir_origin[64] = {0};
    uint8 index = 0;
    char* string1 = PNULL;
    char* string2 = PNULL;
    uint8 wstr_len = 0;
    
    if ((PNULL == res_imsi_string->wstr_ptr) || (0 == res_imsi_string->wstr_len))
    {
        return FALSE;
    }
    
    memset(tstr,0,64);
    MMIAPICOM_WstrToStr(res_imsi_string->wstr_ptr, imsir_origin);
    string1 = imsir_origin;
    
    MNPHONE_GetImsiEx(dual_sys, &imsi);
    SCI_TRACE_BUF("[szp]imsi.number:", imsi.imsi_val, imsi.imsi_len);
#ifndef WIN32
    TransformBcdToStr(imsi.imsi_val,imsi.imsi_len, (uint8 *)tstr);
#endif /* WIN32 */
    string2 = &tstr[1];
    wstr_len = strlen(string1);
    SCI_TRACE_LOW("[szp]imsi.number: %s, wstr_len:%d", tstr, wstr_len);
    for (index=0; index < wstr_len; index++)
    {
        if (('x' == *string1) || ('X' == *string1) || (*string1 == *string2))
        {
            string1++;
            string2++;
            SCI_TRACE_LOW("MMIPHONE_CheckIMSIRString leave %d %d", *string1, *string2);
            continue;
        }
        else
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

PUBLIC BOOLEAN MMIPHONE_CheckIMSIRString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_imsir_string)
{
    //需要考虑xx以及-,范围check
    MN_IMSI_T imsi = {0};
    char tstr[64] = {0};
    char imsir_origin[64] = {0};
    char imsi_min[64] = {0};
    char imsi_max[64] = {0};
    uint8 index = 0;
    char* cstring1 = PNULL;
    char* cstring2 = PNULL;
    char* cstring3 = PNULL;
    uint8 str_len = 0;    
    uint32 imsi_num = 0;
    uint32 imsi_min_num = 0;
    uint32 imsi_max_num = 0;
    
    if ((PNULL == res_imsir_string->wstr_ptr) || (0 == res_imsir_string->wstr_len))
    {
        return FALSE;
    }

    memset(tstr,0,64);
    memset(imsi_min,0,64);
    memset(imsi_max,0,64);
    memset(imsir_origin,0,64);

    MMIAPICOM_WstrToStr(res_imsir_string->wstr_ptr, imsir_origin);
    
    MNPHONE_GetImsiEx(dual_sys, &imsi);
    SCI_TRACE_BUF("[szp]imsi.number:", imsi.imsi_val, imsi.imsi_len);
#ifndef WIN32
    TransformBcdToStr(imsi.imsi_val,imsi.imsi_len, (uint8 *)tstr);
#endif /* WIN32 */
    cstring2 = &tstr[1];
    str_len = strlen(cstring2);
    SCI_TRACE_LOW("[szp]imsi.number: %s, str_len:%d", cstring2, str_len);
    cstring1 = strtok(imsir_origin, "-");
    if (NULL == cstring1)
    {
        return FALSE;
    }
    
    SCI_STRCPY(imsi_min,cstring1);
    cstring1 = imsi_min;
    cstring3 = strtok(NULL, "-");
    if (NULL == cstring3)
    {
        return FALSE;
    }
    
    SCI_STRCPY(imsi_max,cstring3);
    cstring3 = imsi_max;
    
    for (index=0; index < str_len; index++)
    {
        if (('\0' != *cstring1) && ('\0' != *cstring2) && ('\0' != *cstring3) 
            && (*cstring1 == *cstring2) && (*cstring2 == *cstring3))
        {
            cstring1++;
            cstring2++;
            cstring3++;
            SCI_TRACE_LOW("MMIPHONE_CheckIMSIRString leave %d %d %d", *cstring1, *cstring2, *cstring3);
            continue;
        }
        else
        {
            MMIAPICON_ConvertStringtoNumber(cstring1, strlen(cstring1), &imsi_min_num);
            MMIAPICON_ConvertStringtoNumber(cstring2, strlen(cstring2), &imsi_num);
            MMIAPICON_ConvertStringtoNumber(cstring3, strlen(cstring3), &imsi_max_num);

            SCI_TRACE_LOW("[szp]imsi_min_num = %d, imsi_num = %d, imsi_max = %d.",imsi_min_num, imsi_num, imsi_max_num);
            
            if ((imsi_num < imsi_min_num) || (imsi_num > imsi_max_num))
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Check if has the same SPN String
//  Global resource dependence : none
//  Author: CBK
//  Note:res spn VS sim spn
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_CheckSPNString( 
                                       MN_DUAL_SYS_E   dual_sys,
                                       MMI_STRING_T  * inSpn
                                       )
{
    MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
    MMI_STRING_T spn_string = {0};
    wchar spn[MN_SPN_ID_NUM_LEN + 1] = {0};
    
    if ( PNULL == inSpn )
    {
        return FALSE;
    }
	SCI_TRACE_LOW("AutoAdapting:[mmiphone]:MMIPHONE_CheckSPNString dual_sys = %d, spn_len = %d",
                                    dual_sys, inSpn->wstr_len);   
    SCI_TRACE_LOW("AutoAdapting:[mmiphone]:MMIPHONE_CheckSPNString is_spn_supported = %d, sim_spn_len = %d sim_spn=%s",
                                    network_name[dual_sys].is_spn_supported, network_name[dual_sys].spn_len, network_name[dual_sys].spn);   

    spn_string.wstr_ptr = (wchar *)spn;

    // (i) When registered on the HPLMN, or one of the PLMN Identifications used for Service Provider Name display
    //  The SP Name shall be displayed;(TS 22101-910)
	if (0 < network_name[dual_sys].spn_len)
	{
        MMIAPICOM_ParseSimStr(network_name[dual_sys].spn, 
                              network_name[dual_sys].spn_len,
                              &spn_string,
                              MN_SPN_ID_NUM_LEN);
        if (!MMIAPICOM_StrSmartCompare(inSpn, &spn_string, TRUE, TRUE))
        {
            return FALSE;
        }
	}
	else if (network_name[dual_sys].spn_len == 0 && inSpn->wstr_len != 0)
	{      
		return FALSE;
	}
	
    return TRUE; // always true except need compare(no spn sim need pass this check)
}

#endif/*MMI_NETWORK_NAME_SUPPORT*/

/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitNetworkNameInfo(void)
{
    MMIPHONE_NV_NETWORK_NAME_T* nitz_nv_network_name = MMIPHONE_GetNVNetworkNameInfo();
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    
#ifdef  MMI_NETWORK_NAME_SUPPORT
    SCI_MEMSET(&s_network_name, 0, sizeof(s_network_name));
    SCI_MEMSET(&s_pnn_network_name, 0, sizeof(s_pnn_network_name));
#ifdef SIM_PLUG_IN_SUPPORT    
    RegPlugSimCallBack();
#endif
#endif

    //set OPN from NV 
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(nitz_nv_network_name[i].is_opn_exist)
        {
        #ifdef  MMI_NETWORK_NAME_SUPPORT
            s_network_name[i].opn_dcs = nitz_nv_network_name[i].opn_dcs;
            s_network_name[i].opn_len = MIN(nitz_nv_network_name[i].opn_len,MAX_ONSHORTFORM_LEN);
            SCI_MEMCPY(s_network_name[i].opn, nitz_nv_network_name[i].opn, s_network_name[i].opn_len);
        #else
            PHONE_SERVICE_STATUS_T* cur_service_status = MMIAPIPHONE_GetServiceStatus();
            
            cur_service_status[i].is_opn_exist = nitz_nv_network_name[i].is_opn_exist;
            cur_service_status[i].opn_dcs= nitz_nv_network_name[i].opn_dcs;
            cur_service_status[i].opn_len= MIN(nitz_nv_network_name[i].opn_len,MAX_ONSHORTFORM_LEN);
            SCI_MEMCPY(cur_service_status[i].opn, nitz_nv_network_name[i].opn, cur_service_status[i].opn_len);
        #endif   
        }
    }
}

#endif/*_MMIPHONE_NETWORK_NAME_C_*/
