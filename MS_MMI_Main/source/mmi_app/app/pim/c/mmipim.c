/******************************************************************************
                                   海信TD-SCDMA  自主业务模块
        本模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现PIM业务。 模块开发参考OMA成员共同开发的
        SRT_c  及海信WCDMA PIM功能模块。 目前模块计划开发支持OMA 
        syncML 协议1.1

        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:王晓林
        创建日期:2007年9月22日。
******************************************************************************/


/******************************************************************************
文件名称:mmipim.c
文件描述:PIM应用头文件。
文件说明:应用注册初始化以及对其他应用提供接口。
开发人员:王晓林
首次开发日期:2007-9.22
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2007-09-22            王晓林                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
#include "mmi_app_pim_trc.h"
#ifdef WIN32
  #include "std_header.h"
#endif
  #define _MMIPIM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipim.h"
#include "mmipim_wintab.h"
#include "Mmipim_id.h"
#include "Mmipim_nv.h"
#include "PIMlibmem.h"
#include "Mmk_app.h"
#include "mmi_appmsg.h"
#include "mn_type.h"
#include "mmi_nv.h"
//#include "mmiwap.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
//#include <stdarg.h>
#include "mmi_modu_main.h"
#include "mmiphone_export.h"
#include "guilabel.h"
//#include "Mmk_type.h"
#include "mmipdp_export.h"
#include "Vcalendar_parse.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmipim_wintab.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

  /**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
 /**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
wchar hint_note[100] = {0};

MMI_STRING_T pim_note_s={(wchar*)hint_note,0};

PIM_T g_pim;

BOOLEAN g_is_pim_sync=FALSE;//是否正在同步
MMI_APPLICATION_T   g_mmipim_app;    // pim应用的实体

#define PDP_MAX_RETRY 0






  /*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
  
  /*****************************************************************************/
//  Description : 处理PS的消息
//  Global resource dependence : none
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandlePIMPsMsg 
(
    PWND                app_ptr, 
    uint16              msg_id, 
    DPARAM              param
);



#define PEER_MAX_STR_SIZE 250

//extern  void Enter_LOG_Flag(  BOOLEAN b_log    );

#if 0//del by wangxiaolin   2009.12.20
static char msg[PEER_MAX_STR_SIZE];
static char  msg_log[PEER_MAX_STR_SIZE];
#endif
//#ifndef WIN32

//#define DebugPrint   SCI_TRACE_MID
//#define DebugPrintLine   SCI_TRACE_MID

/*#define DebugPrint(msg) \
{\
msg[PEER_MAX_STR_SIZE-1] = 0;\
Enter_LOG_Flag(1);\
SCI_TraceMid(msg);\
Enter_LOG_Flag(0);\
}*/

/*#define DebugPrintLine(msg)   \
{\
msg[PEER_MAX_STR_SIZE-1] = 0;\
Enter_LOG_Flag(1);\
SCI_TraceLow(msg);\
Enter_LOG_Flag(0);\
}*/
//#endif 

#ifdef WIN32
#define vsnprintf _vsnprintf
#endif
  
  /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : MMIPIM_GetPIM
//  Global resource dependence : g_pim
//  Author: wangxiaolin
//  Note: 为其他文件(比如网络承载)调用设置提供接口
/*****************************************************************************/
PUBLIC PIM_T_P  MMIPIM_GetPIM(void)
{
    return (&g_pim);
}

PUBLIC PIM_GPRS_STATE_E  MMIPIM_GetPIMGprsState(void)
{
    return (g_pim.gprs_state);
}

/*****************************************************************************/
//  FUNCTION:    pim_notify_dm_happen  
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note: notify pim that dm will running
/*****************************************************************************/
PUBLIC void pim_notify_dm_happen(void)
{
       MMIPIM_PostMsgToSyncWin(PIM_MSG_STOP,NULL, NULL);
}


/*****************************************************************************/
//  Description : MMIPIM_GetpISocket
//  Global resource dependence : g_pim
//  Author: wangxiaolin
//  Note: 为了避免调用时候对结构体的其它成员造成更改，
//尽量做到用什么调什么
/*****************************************************************************/
PUBLIC int* MMIPIM_GetpISocket(void)
{
   return (&g_pim.m_pISocket);
}



 /*****************************************************************************/
//  Description : 初始化模块用到的全局变量
//  Global resource dependence : g_pim
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIM_InitGlobleRef(void)
{
      MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;

      SmlEncoding_t  nv_sync_encode_type=0;
      auth_type nv_sync_auth_type=0;
      proxy_setting_info_type nv_proxy_setting={0};
      server_setting_info_type nv_server_setting={0};
      pim_sync_result sync_result={0};
      net_setting_info_type nv_net_setting = {0}; // chenxiang 20071101 pim_setting
      wap_url_info_type  nv_url_setting = {0}; // chenxiang 20071101 pim_setting
#ifdef HS_PIM_UA_SETTING
      uint32 nv_ua_setting = 0;
      MMIPIM_UA_INFO_T ua_info = {0};
#endif
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
    sync_type nv_sync_type=SYNC_TYPE_2WAY;
#endif

    const net_setting_info_type	nv_sync_net_info = {"CMWAP","",""};
    const proxy_setting_info_type nv_sync_proxy_info = {1, "10.0.0.172", "80", "", ""};
    //MS00216980 cheney
    const wap_url_info_type nv_sync_url_info = {"http://pim1.monternet.com/kt.wml", "http://pim1.monternet.com/cx.wml", "http://pim1.monternet.com/zf.wml"};

    
    //SCI_TRACE_LOW:"enter MMIPIM MMIPIM_InitGlobleRef,time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_212_112_2_18_2_46_55_0,(uint8*)"d",PIM_GetCurTime());
     //get auth type from NV
    MMINV_READ(MMI_PIM_AUTH_TYPE,&nv_sync_auth_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        nv_sync_auth_type = AUTH_B64;
        MMINV_WRITE(MMI_PIM_AUTH_TYPE,&nv_sync_auth_type);
    }

     //get encode type from NV
    MMINV_READ(MMI_PIM_ENCODE_TYPE,&nv_sync_encode_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        nv_sync_encode_type = SML_WBXML;
        MMINV_WRITE(MMI_PIM_ENCODE_TYPE,&nv_sync_encode_type);
    }
	

     //get proxy setting from NV
    MMINV_READ(MMINV_PIM_PROXY_INFO,&nv_proxy_setting,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        hs_smlLibMemcpy(&nv_proxy_setting,&nv_sync_proxy_info,sizeof(proxy_setting_info_type));
        MMINV_WRITE(MMINV_PIM_PROXY_INFO,&nv_proxy_setting);
    }
     //MS00216980 cheney
     //get server setting from NV
    MMIPIM_GetServerConfig(&nv_server_setting);

     //get net setting from NV // chenxiang 20071101 pim_setting
    MMINV_READ(MMINV_PIM_NET_INFO,&nv_net_setting,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        hs_smlLibMemcpy(&nv_net_setting,&nv_sync_net_info,sizeof(net_setting_info_type));
        MMINV_WRITE(MMINV_PIM_NET_INFO,&nv_net_setting);
    }
	
     //get url setting from NV // chenxiang 20071101 pim_setting
    MMINV_READ(MMINV_PIM_WAP_URL_INFO, &nv_url_setting, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        hs_smlLibMemcpy(&nv_url_setting, &nv_sync_url_info, sizeof(wap_url_info_type));
        MMINV_WRITE(MMINV_PIM_WAP_URL_INFO, &nv_url_setting);
    }
#ifdef HS_PIM_UA_SETTING
   //MS00216980 cheney
    MMINV_READ(MMINV_PIM_UA_TYPE, &nv_ua_setting, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        nv_ua_setting = PIM_UA_TYPE_DEFAULT;
        MMINV_WRITE(MMINV_PIM_UA_TYPE, &nv_ua_setting);
    }
    MMINV_READ(MMINV_PIM_UA_INFO, &ua_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIPIM_GetUAInfo(PIM_UA_TYPE_DEFAULT, &ua_info);
        MMINV_WRITE(MMINV_PIM_UA_INFO, &ua_info);
    }
#endif

#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
    MMINV_READ(MMINV_PIM_SYNC_TYPE, &nv_sync_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        nv_sync_type = SYNC_TYPE_2WAY;
        MMINV_WRITE(MMINV_PIM_SYNC_TYPE, &nv_sync_type);
    }
 #endif   
    hs_smlLibMemset(&sync_result,0,sizeof(pim_sync_result));

    g_pim.g_sync_result = sync_result;
    g_pim.m_psocket_select_timer = PNULL;
    g_pim.gprs_state = PIM_GPRS_DEACTIVE_OK;
    g_pim.m_pISocket = -1;
    g_pim.is_proxy = nv_proxy_setting.use_proxy;
    g_pim.m_authtype = nv_sync_auth_type;
    g_pim.m_sml_encoding_type = nv_sync_encode_type;
    g_pim.m_proxy_setting = nv_proxy_setting;
    g_pim.m_server_setting = nv_server_setting;
    g_pim.m_net_setting = nv_net_setting; // chenxiang 20071101 pim_setting
    g_pim.m_url_setting = nv_url_setting; // chenxiang 20071101 pim_setting
#ifdef HS_PIM_UA_SETTING
    g_pim.m_ua_type = nv_ua_setting; // chenxiang 20080318
#endif
    g_pim.is_init=FALSE; 

#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
    g_pim.m_sync_type=nv_sync_type;
 #endif
#if 0//del by wangxiaolin  2009.9.18
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    g_pim.m_sync_mode = SYNC_MODE_MAX; //同步类型0:电话簿1:日程 
#endif /* HS_PIM_VCAL */
#endif
}
/*****************************************************************************/
//  Description : MMIPIM_IsPimInitialized
//  Global resource dependence : g_pim
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIM_IsPimInitialized(void)
{
   return g_pim.is_init;

}

/*==========================================================
 * author		: wangxiaolin       
 * function 	: HandlePIMPdpMsg
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/19/2009 
 ==========================================================*/
LOCAL void HandlePIMPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    PIM_T_P  pMe = MMIPIM_GetPIM	();
    //SCI_ASSERT(PNULL != msg_ptr);/*assert verified*/
    if(PNULL == msg_ptr)
        return;
    
    //SCI_TRACE_LOW:"MMIPIM:HandlePIMPdpMsg enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_332_112_2_18_2_46_55_1,(uint8*)"");
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            //SCI_TRACE_LOW:"MMIPIM:pim  pdp active success!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_338_112_2_18_2_46_55_2,(uint8*)"");
            if((PIM_GPRS_ACTIVE_OK != pMe->gprs_state)&&(MMIPIM_IsSyncProcing()))
            {
                  pMe->gprs_state = PIM_GPRS_ACTIVE_OK;
                  pMe->m_netid = msg_ptr->nsapi;  /* get net interface ID, cr166376, @yifeng.wang */
  		     MMIPIM_PostMsgToSyncWin( MSG_PDP_ACTIVE_SUCCESS,NULL, NULL);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIPIM:pim pdp active fail!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_348_112_2_18_2_46_55_3,(uint8*)"");
             pMe->gprs_state = PIM_GPRS_PDP_REJECT;
             MMIPIM_PostMsgToSyncWin( MSG_PDP_ACTIVE_FAIL,NULL, NULL);
        }
        
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        {
            //SCI_TRACE_LOW:"MMIPIM:pim  pdp deactive is over!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_357_112_2_18_2_46_55_4,(uint8*)"");
             if((PIM_GPRS_DEACTIVE_OK != pMe->gprs_state)&&(!MMIPIM_IsSyncProcing()))
             {
                  pMe->gprs_state = PIM_GPRS_DEACTIVE_OK;
                  pMe->m_netid = 0;   /* reset net interface ID, cr166376, @yifeng.wang */
             }
        }		   
        break;
        
    case MMIPDP_DEACTIVE_IND:
        //SCI_TRACE_LOW:"MMIPIM:pim  MMIPDP_DEACTIVE_IND msg is come!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_367_112_2_18_2_46_55_5,(uint8*)"");
        // MMIPDP_DEACTIVE_IND 表示网络主动断开连接，PDP 已经去激活
        // 清除GPRS 激活标志, @yifeng.wang。此处不能直接终止，否则可能导致重入
        // 其他地方会对标志进行查询判断
         if((PIM_GPRS_DEACTIVE_OK != pMe->gprs_state)&&(!MMIPIM_IsSyncProcing()))
         {
              pMe->gprs_state = PIM_GPRS_DEACTIVE_OK;
              pMe->m_netid = 0;   /* reset net interface ID, cr166376, @yifeng.wang */
         }
        break;
        
    default:        
        break;
    }
    //SCI_TRACE_LOW:"MMIPIM:HandlePIMPdpMsg exit!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_381_112_2_18_2_46_55_6,(uint8*)"");
}


/*=====================================================================

  FUNCTION :  MMIPIM_ConnectNetwork
  DESC        : pdp 激活
  
  INPUT:        none
  RETURN:      
  AUTHER:      wangxiaolin
  OTHERS:      

======================================================================*/
void MMIPIM_ConnectNetwork(int* presult)
{


    MMIPDP_ACTIVE_INFO_T app_info = {0};
    PIM_T_P  pMe=MMIPIM_GetPIM();
    MN_DUAL_SYS_E dual_sys=MMIPIM_GetSim();
    
    app_info.app_handler = MMI_MODULE_PIM;
    //app_info.auth_type = MN_PAP;
    app_info.apn_ptr = pMe->m_net_setting.apn;
    app_info.user_name_ptr =pMe->m_net_setting.username;
    app_info.psw_ptr = pMe->m_net_setting.password;
    app_info.dual_sys = dual_sys;
    app_info.priority = 4;//3
    app_info.ps_service_rat = MN_UNSPECIFIED;
    app_info.ps_service_type = BROWSER_E;
    app_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef MMIPIM_PDP_OLD_STYLE  //Added by wangxiaolin   2009.12.19 for 展讯PDP管理方式
    app_info.handle_msg_callback = HandlePIMPsMsg;/*lint !e64 */
#else
    app_info.handle_msg_callback = HandlePIMPdpMsg;/*lint !e64 */
#endif /*end of  MMIPIM_PDP_OLD_STYLE */
    
    //SCI_TRACE_LOW:"enter MMIPIM_ConnectNetwork()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_417_112_2_18_2_46_55_7,(uint8*)"");
	
    switch(pMe->gprs_state)
    {
    case PIM_GPRS_DEACTIVE_OK:
    case PIM_GPRS_PDP_REJECT:
    case PIM_GPRS_MAX:
        {

            //SCI_TRACE_LOW:"MMIPIM_GPRS_DEACTIVE_OK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_426_112_2_18_2_46_55_8,(uint8*)"");
#ifndef WIN32
           if(!MMIAPIPDP_Active(&app_info))       
            {
                //SCI_TRACE_LOW:"MMIPIM MNGPRS_SetAndActivePdpContext failure!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_433_112_2_18_2_46_55_9,(uint8*)"");
                *presult =-1;
                return ;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIPIM  MNGPRS_SetAndActivePdpContext ok!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_439_112_2_18_2_46_55_10,(uint8*)"");
                *presult = 0;
                pMe->gprs_state = PIM_GPRS_ACTIVING;
            }
#else   //FOR WIN32 Simulator
              *presult = -1;
              pMe->gprs_state = PIM_GPRS_ACTIVE_OK;
             return;
#endif

        }
        break;/*lint !e527 */

       case PIM_GPRS_ACTIVE_OK:
        //SCI_TRACE_LOW:"MMIPIM_GPRS_ACTIVE_OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_453_112_2_18_2_46_55_11,(uint8*)"");
        *presult = 1;
        break;
        
    case PIM_GPRS_ACTIVING:
    case PIM_GPRS_DEACTIVEING:
        //SCI_TRACE_LOW:"==PIM_GPRS_DEACTIVEING "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_459_112_2_18_2_46_55_12,(uint8*)"");
        *presult = 0;
        break;
    default:
        //SCI_ASSERT(FALSE);/*assert verified*/
         *presult = -1;
        break;

    }
   return;
}

/*****************************************************************************/
//  Description : 处理Gprs激活的消息
//  Global resource dependence : none
//  Author: wangxiaolin
//  Note: 
//  Data:2007.10.17
/*****************************************************************************/
#ifdef MMIPIM_PDP_OLD_STYLE  //Added by wangxiaolin   2009.12.19 for 展讯平台老的PDP管理方式
LOCAL void PIM_ProcessGprsActiveCnf(
    DPARAM param
)
{
    int pdp_result=0;
    static int pdp_active_times = 0;//pdp active times
    APP_MN_GPRS_T *signal_ptr = (APP_MN_GPRS_T*)param;
    PIM_T_P  pMe = MMIPIM_GetPIM();

    //SCI_ASSERT(PNULL != param);/*assert verified*/
    if(PNULL == param)
        return;
    //SCI_TRACE_LOW:"enter MMIPIM PIM_ProcessGprsActiveCnf()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_489_112_2_18_2_46_55_13,(uint8*)"");
    
   	if((MN_GPRS_ERR_SUCCESS == signal_ptr->result)||(MN_GPRS_ERR_SAME_PDP_CONTEXT == signal_ptr->result))
	{
		pMe->gprs_state = PIM_GPRS_ACTIVE_OK;
		pdp_active_times = 0;
		MMIPIM_PostMsgToSyncWin( MSG_PDP_ACTIVE_SUCCESS,NULL, NULL);
		return;
	}
	else if(MN_GPRS_ERR_ACTIVE_REJCET == signal_ptr->result)
	{
		pMe->gprs_state = PIM_GPRS_PDP_REJECT;
	}
	else
	{
		pMe->gprs_state = PIM_GPRS_DEACTIVE_OK;
	}
	pdp_active_times ++;
    
	if(pdp_active_times > PDP_MAX_RETRY)//active pdp 2 times mostly
	{
		pdp_active_times = 0;

		pMe->gprs_state = PIM_GPRS_PDP_REJECT;
             MMIPIM_PostMsgToSyncWin( MSG_PDP_ACTIVE_FAIL,NULL, NULL);
	       return;
	}

      //SCI_TRACE_LOW:"MMIPIM PIM_ProcessGprsActiveCnf, try to connect network again!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_517_112_2_18_2_46_55_14,(uint8*)"");

	MMIPIM_ConnectNetwork(&pdp_result);
    return;
}
#endif /* MMIPIM_PDP_OLD_STYLE */

/*****************************************************************************/
//  Description : 处理PS的消息
//  Global resource dependence : none
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandlePIMPsMsg 
(
    PWND    app_ptr, 
    uint16  msg_id, 
    DPARAM  param
)
{
    MMI_RESULT_E  recode       = MMI_RESULT_FALSE;
#ifdef MMIPIM_PDP_OLD_STYLE  //Added by wangxiaolin   2009.12.19 for 展讯平台老的PDP管理方式
    PIM_T_P  pMe = MMIPIM_GetPIM();
    
   //SCI_TRACE_LOW:"enter MMIPIM HandlePIMPsMsg()"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_540_112_2_18_2_46_55_15,(uint8*)"");
 if(TRUE==pMe->is_init)
 {
    switch(msg_id)
    {
    case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
       //SCI_TRACE_LOW:"enter MMIPIM HandlePIMPsMsg(),pdp_actived_cnf"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_546_112_2_18_2_46_55_16,(uint8*)"");
 /*Start of  wangzhefeng 2009.10.27 for 修改pim与彩信并发死机问题 */
    if((PIM_GPRS_ACTIVE_OK != pMe->gprs_state)&&(MMIPIM_IsSyncProcing()))
  /*End   of  wangzhefeng 2009.10.27 for 修改pim与彩信并发死机问题 */
    {
           PIM_ProcessGprsActiveCnf(param);
    }

          
        break;

    case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
     //SCI_TRACE_LOW:"enter MMIPIM HandlePIMPsMsg(),DEACTIVATE_PDP_CONTEXT_CNF"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_558_112_2_18_2_46_55_17,(uint8*)"");
/*Start of  wangxiaolin 2009.11.2 for 彩信并发死机问题 */
         if((PIM_GPRS_DEACTIVE_OK != pMe->gprs_state)&&(!MMIPIM_IsSyncProcing()))
/*End   of  wangxiaolin 2009.11.2 for 彩信并发死机问题 */
         {
                pMe->gprs_state = PIM_GPRS_DEACTIVE_OK;
         }
   
        break;

    case APP_MN_DEACTIVATE_PDP_CONTEXT_IND:
        //SCI_TRACE_LOW:"enter MMIPIM HandlePIMPsMsg(),APP_MN_DEACTIVATE_PDP_CONTEXT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_567_112_2_18_2_46_55_18,(uint8*)"");
 /*Start of  wangxiaolin 2009.11.2 for 彩信并发死机问题 */
         if((PIM_GPRS_DEACTIVE_OK != pMe->gprs_state)&&(!MMIPIM_IsSyncProcing()))
/*End   of  wangxiaolin 2009.11.2 for 彩信并发死机问题 */
        pMe->gprs_state = PIM_GPRS_DEACTIVE_OK;
  
        break;

    default:
        recode = MMI_RESULT_FALSE;
      
        break;

    }
 }
#endif /* MMIPIM_PDP_OLD_STYLE */
    return recode;
}
  
 /*****************************************************************************/
//  Description : 初始化Pim的全局变量、回调函数等
//  Global resource dependence : 
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
  void MMIPIM_InitApp 
(
	void
)
{
    MMIPIM_InitGlobleRef(); // chenxiang_20080407
    g_mmipim_app.ProcessMsg     = HandlePIMPsMsg; 	//设置处理协议栈消息的回调函数
    g_mmipim_app.component_type = CT_APPLICATION; 
    MMIPIM_InitPbStatus();
#ifdef HS_PIM_VCAL  //Added by wangzhefeng  2009.9.4
    MMIPIM_InitVCALStatus();
#endif /* HS_PIM_VCAL */

}


 /*****************************************************************************/
//  Description : 号簿管家入口函数
//  Global resource dependence : 
//  Author: 王晓林
//  Note:
/*****************************************************************************/
PUBLIC void MMIPIM_Main(PIM_ENTRY_TYPE entry_type)
 {
    uint32  sim_num = 0;
    uint16  sim_ok=0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);
    
     if (sim_num>0)
        {
/*Start of  wangzhefeng 2009.10.30 for 在飞行模式下，不进入号簿管家 */
        if(MMIAPISET_GetFlyMode())
        {
            //飞行模式，不能进入，返回
            MMIAPISET_AlertFlyMode();
            return;
        }
/*End   of  wangzhefeng 2009.10.30 for 在飞行模式下，不进入号簿管家 */
          //首先关闭PIM业务的相关窗口,防止重入
          MMK_CloseMiddleWin(MMIPIM_MAINMENU_WIN_ID, MMIPIM_END_WIN_ID);

#ifdef PIM_MEMORY_LEAK_DEBUG // chenxiang_memory 20080314
          PrintLeakLink(); // chenxiang_memory 20080312
#endif
          // MMIPIM_InitGlobleRef(); chenxiang_20080407
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.18
          if(PIM_ENTRY_FROM_PB == entry_type)
          {
                g_pim.m_sync_mode =SYNC_MODE_PB;
          }
          else
          {
                g_pim.m_sync_mode =SYNC_MODE_MAX;
          }
#endif /* HS_PIM_VCAL */
          MMIPIM_OpenMainMenuWin(entry_type);
    }
    else
    {
        MMIAPIPHONE_AlertSimNorOKStatus();

    }
 }


/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIPIM_InitModule(void)
{
    MMIPIM_RegWinIdNameArr();
    MMIPIM_RegMenuGroup();
    MMIPIM_RegNv(); 

}

/*****************************************************************************/
//  FUNCTION:      MMIPIM_DisplayProc 
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   暂时用来显示空 ，刷新第二个标签
/*****************************************************************************/
PUBLIC void MMIPIM_DisplayProc(void)
{
        hs_smlLibMemset((char *)pim_note_s.wstr_ptr,0,sizeof(pim_note_s.wstr_ptr));
      	 pim_note_s.wstr_len=(uint16)MMIAPICOM_Wstrlen(pim_note_s.wstr_ptr);

	 GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID1, &pim_note_s, TRUE);
        
}

/*****************************************************************************/
//  FUNCTION:      MMIPIM_GenDisplayChr
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   当前显示用的是Lable 控件，不能使用资源文件
//后期考虑修改。且不能使用Strcat函数
/*****************************************************************************/
PUBLIC void MMIPIM_GenDisplayChr(wchar * display_chr1,uint16 chr1_len,
                                      wchar * display_chr2,
                                      uint16 chr2_len,
                                      wchar * display_chr3,
                                      uint16 chr3_len,
                                      wchar * display_chr4, // chenxiang_pim_ui 20080306
                                      uint16 chr4_len
                                      )
{
       int  i=0;  
        hs_smlLibMemset((char *)pim_note_s.wstr_ptr,0,70);
        
        MMIAPICOM_Wstrncpy(pim_note_s.wstr_ptr,display_chr1,chr1_len);
        if (NULL != display_chr2)
        {
            for(i=0;i<chr2_len;i++)
            {
               // pim_note_s.str_ptr[chr1_len+2*i]=0x0;
                pim_note_s.wstr_ptr[chr1_len+i]=display_chr2[i];
            }
        }

        if (NULL != display_chr3)
        {
            for(i=0;i<chr3_len;i++)
            {
               // pim_note_s.str_ptr[chr1_len+chr2_len+2*i]=0x0;
                pim_note_s.wstr_ptr[chr1_len+chr2_len+i]=display_chr3[i];
            }
        }
        
        if (NULL != display_chr4)
        {
            for(i=0;i<chr4_len;i++)
            {
               // pim_note_s.str_ptr[chr1_len+chr2_len+2*i]=0x0;
                pim_note_s.wstr_ptr[chr1_len+chr2_len+chr3_len+i]=display_chr4[i];
            }
        }
        
      	 pim_note_s.wstr_len=(uint16)(chr1_len+chr2_len+chr3_len+chr4_len);
}


/*****************************************************************************/
//  FUNCTION:      MMIPIM_GetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIM_GetServerConfig(server_setting_info_type * p_server_setting)
{
  BOOLEAN result=FALSE;
  BOOLEAN test_flag = FALSE;
  MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
  const server_setting_info_type nv_sync_syncml_info = {"./contact", "./calendar", PIM_SRV_ADDR_URL, "80", "", ""};
  const server_setting_info_type nv_sync_syncml_info_test = {"./contact", "./calendar", PIM_SRV_ADDR_URL_TEST, "80", "", ""};
  //SCI_ASSERT(PNULL != p_server_setting);/*assert verified*/
  if(PNULL == p_server_setting)
        return  FALSE;
  
  test_flag = MMIAPIENG_GetCMCCTestFlag();

  //get server setting from NV
  MMINV_READ(MMI_PIM_SERVER_INFO,p_server_setting,return_value);  

    //MS00216980 cheney 
   if(MN_RETURN_SUCCESS != return_value)
   {
		if(test_flag)
		{
		      hs_smlLibMemcpy(p_server_setting,&nv_sync_syncml_info_test,sizeof(server_setting_info_type));
		}
		else
		{
	      		hs_smlLibMemcpy(p_server_setting,&nv_sync_syncml_info,sizeof(server_setting_info_type));
		}
	      MMINV_WRITE(MMI_PIM_SERVER_INFO,p_server_setting);
	      result=TRUE;
   }

   if((test_flag && strcmp(p_server_setting->addr, nv_sync_syncml_info_test.addr)!= 0)||
   	                       (!test_flag && strcmp(p_server_setting->addr, nv_sync_syncml_info.addr)!= 0))
   {
   	//SCI_TRACE_LOW:"[MMIPIM]  test flag is not consistent with pim config"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_769_112_2_18_2_46_56_19,(uint8*)"");
   }
   return result;
}

/*****************************************************************************/
//  FUNCTION:      MMIPIM_SetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIPIM_SetServerConfig(server_setting_info_type * p_server_setting)
{
   //SCI_ASSERT(PNULL != p_server_setting);/*assert verified*/
   
  //Set server config to NV
   MMI_WriteNVItem(MMI_PIM_SERVER_INFO,p_server_setting);
  return ;
}


/*****************************************************************************/
//  FUNCTION:      MMIPIM_IsSyncProcing
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note: 判断pim是否正在同步
/*****************************************************************************/
BOOLEAN MMIPIM_IsSyncProcing(void)
{
   return g_is_pim_sync;
}


/*==========================================================
 * author		: wangxiaolin      
 * function 	: MMIPIM_PostMsgToSyncWin
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/29/2009 
 ==========================================================*/
PUBLIC BOOLEAN MMIPIM_PostMsgToSyncWin(
                    MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                    DPARAM              param_ptr,      //the param of the message
                    uint32              size_of_param   //the size of the param
                    )
{
    
    	//if(MMIPIM_IsSyncProcing())
    	if(MMK_IsOpenWin(MMIPIM_SYNC_BEGIN_WIN_ID))
	{
		return MMK_PostMsg(MMIPIM_SYNC_BEGIN_WIN_ID, msg_id,param_ptr, size_of_param);
	}
       else
       {
             return FALSE;     
       }
}

/*==========================================================
 * author		: wangxiaolin      
 * function 	: public uint32 PIM_GetCurTime(void)
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  8/28/2009 
 ==========================================================*/
PUBLIC uint32 PIM_GetCurTime(void)
{

#if 0//del by wangxiaolin  2009.9.10
    SCI_TIME_T sys_time = {0};
    uint32      second = 0;
    
    TM_GetSysTime(&sys_time);
     second =   sys_time.hour*3600+sys_time.min*60+sys_time.sec;
     return ( second );   
#endif

     return  (SCI_GetTickCount());   
}


PUBLIC void MMIPIM_SaveLastAnchor(char * nv_lastanchor)
{
    if (PNULL == nv_lastanchor)
    {
        return;
    }
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
    if(SYNC_MODE_PB == MMIPIM_GetPIM()->m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    MMINV_WRITE(MMIPIM_LAST_ANCHOR,nv_lastanchor);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
    }
    else
    {
        MMINV_WRITE(MMIPIM_VCAL_LAST_ANCHOR,nv_lastanchor);
    }
#endif /* HS_PIM_VCAL */
}

PUBLIC void  MMIPIM_DeactivePDP(void)
{
#if 0//Added by wangxiaolin  2009.9.3
             MNGPRS_ResetAndDeactivePdpContext();
#else
             MMIAPIPDP_Deactive(MMI_MODULE_PIM);
#endif//Added by wangxiaolin  2009.9.3
}
