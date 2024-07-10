
/*****************************************************************************
** File Name:      mmidcd.c                                                  *
** Author:                                                                   *
** Date:           09/2007                                                   *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    											                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2007        violent.wei      Create
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_dcd_trc.h"
#ifdef MMI_ENABLE_DCD	//!宏定义开头
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmidcd_main.h"
#include "mmidcd_export.h"
#include "mmidcd_id.h"
#include "mmidcd_nv.h"
#include "http_api.h"
#include "mmidcd_xml_parse.h"
#include "mmidcd_file.h"
#include "mmidcd_http.h"
#include "mmidcd_wintab.h"
#include "mmipb_export.h"
#include "mmiidle_export.h"
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "mmiphone_export.h"
#include "mmidcd_text.h"
#include "mmidcd_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h" 
#include "mmidcd_export.h"
#include "mmi_appmsg.h"
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
#define MMIDCD_MEMORY_NEEDED_SIZE     1000000 //3000000 //jun.hu 2010.2.3 改为1M
#define CHINA_MCC_CODE                0x01CC
#define CHINA_MNC_CODE                0x00
#define DCD_UPDATE_TIMES_MAX          3
#define DCD_TRACE_LOG_OFFSET          20 
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//static uint32   s_mcc = CHINA_MCC_CODE;
//static uint32   s_mnc = CHINA_MNC_CODE;

static uint32   s_update_times  = 0;
static uint32   s_ttl_second    = 0;    
static uint32   s_retry_minutes = 5;

LOCAL SCI_TIMER_PTR s_ttl_timer = NULL; 
LOCAL SCI_TIMER_PTR s_retry_timer = NULL;
static uint32   s_retry_times  = 0;
static uint32   s_retry_seconds = 300;

static MMIDCD_ERROR_CODE_E s_dcd_error       = DCD_ERROR_NULL;
static MMIDCD_REQ_TYPE_E   s_latest_req_type = HTTP_REQ_TYPE_NULL;
static DCD_STATE_E         s_dcd_status      = DCD_NULL_STATUS;
static DCD_SigDispatch     s_SigDispatch     = PNULL;

static BOOLEAN  s_svr_update      = FALSE;
static BOOLEAN  s_is_error_retry  = FALSE; 
//static BOOLEAN  is_show_default   = FALSE;

static BOOLEAN  s_dcd_is_updating    = FALSE; //DCD是否在更新中
static BOOLEAN  s_update_is_cancling = FALSE; // DCD 是否在取消更新中...

static xmlDocPtr               s_mmiidle_xml_doc_ptr = {0}; 
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


static const char msg_str[][64]=
{
   "OUT OF DCD MSG",
    "HTTP_SIG_INIT_CNF",	    
    "HTTP_SIG_CLOSE_CNF",
    "HTTP_SIG_GET_REQ",
    "HTTP_SIG_GET_CNF",
    "HTTP_SIG_ERROR_IND",
    "HTTP_SIG_NEED_AUTH_IND",
    "HTTP_SIG_NEED_AUTH_RSP",
    "HTTP_SIG_HEADER_IND",
    "HTTP_SIG_HEADER_RSP",
    "HTTP_SIG_DATA_IND",
    "HTTP_SIG_CANCEL_REQ",
    "HTTP_SIG_CANCEL_CNF",
    "HTTP_SIG_POST_REQ",
    "HTTP_SIG_POST_CNF",
    "HTTP_SIG_HEAD_REQ",
    "HTTP_SIG_HEAD_CNF",
    "HTTP_SIG_NET_PROG_IND",
    "HTTP_SIG_REDIRECT_IND",
    "HTTP_SIG_REQUEST_ID_IND", //19

    "DCD_SIG_INIT_REQ",
    "DCD_SIG_INIT_CNF",
    "DCD_SIG_PDP_ACTIVED_CNF",
    "DCD_SIG_PDP_DEACTIVED_CNF",
    "DCD_SIG_DL_CANCLE_REQ",
    "DCD_SIG_XML_DL_REQ",
    "DCD_SIG_XML_DL_CNF",    
    "DCD_SIG_MIME_DL_REQ",
    "DCD_SIG_MIME_DL_CNF",
    "DCD_SIG_DL_OVER_IND",
    "DCD_SIG_DEFALUT_RETRY_TIMER_IND",
    "DCD_SIG_ERROR_IND"
    

};

static const char dcd_state_str[][64]=
{
    "DCD_NULL_STATUS",
    "DCD_IDLE_STATUS",
    "DCD_XML_STATUS",
    "DCD_MIME_STATUS",
    "DCD_MAX_STATUS"
};

/*must match SIM_STATUS_E struct*/
LOCAL const MMI_TEXT_ID_T s_sim_status_string[SIM_STATUS_MAX + 1] = 
{
    TXT_NULL,                               //SIM_STATUS_OK,
    TXT_SIM_REJECTED,               //SIM_STATUS_REJECTED,
    TXT_SIM_REGISTRATION_FAILED,//SIM_STATUS_REGISTRATION_FAILED,
    TXT_PIN_BLOCKED,                //SIM_STATUS_PIN_BLOCKED
    TXT_NOT_SUPPORT_SIM,	//SIM_STATUS_NOT_SUPPORT_SIM, //@zhaohui,cr109170                                                            
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK_BLOCKED,
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK2_BLOCKED,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    TXT_INSERT_SIM,                   //SIM_STATUS_NO_SIM,
#else
    TXT_INSERT_SIM_AND_INFO, //SIM_STATUS_NO_SIM,
#endif
    TXT_PROCESS_SEARCHING_NETWORK, //SIM_NOT_INITED
    TXT_NULL                                //SIM_STATUS_MAX
};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN DCD_ActivatePDPcontext(void);
/*****************************************************************************/
// Description : 
// Global resource dependence : None
// Author: 
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DCD_DeactivatePDPcontext(void);

/*****************************************************************************/
// Description : 
// Global resource dependence : None
// Author:  
// Note: 
/*****************************************************************************/
LOCAL void DcdHandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);
/*****************************************************************************/
//  Description :get current mcc
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
LOCAL void DCD_SigDispatchReg(DCD_SigDispatch SigDispatch);
/*****************************************************************************/
//  Description : 对Ctxt_id 的唯一性进行判断处理
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:  
/*****************************************************************************/
LOCAL void DeleteRepeatCtxtId(xmlDocPtr xml_doc_ptr);
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:zhoufeng
//  Note: modified by jun.hu 
/*****************************************************************************/
LOCAL BOOLEAN ParseDCDXMLFile(BOOLEAN is_default_parse);
/*****************************************************************************/
//  Description :开机解析函数, 此时还没有去更新，所以不需要比较两个xml 文件...
//  Global resource dependence : none
//  Author: jun.hu 
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN ParseDCDXMLFileWhenPowerOn(BOOLEAN is_default_parse);
/*****************************************************************************/
//  Description : PS/CS Notify Event Callback function  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void MMIDCD_NotifyEventCallback(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event, uint32 param);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:jun.hu
//  Note: 指DCD是否在IDLE状态,
/*****************************************************************************/
LOCAL BOOLEAN IsDCDInIdleState(void);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: jun.hu
//  Note:
/****************************************************************************/
PUBLIC uint32  MMIDCD_GetNextUpdateTimeSpace(void)
{
    if(s_is_error_retry)
        return s_retry_seconds;
    
    return s_ttl_second;
}
/****************************************************************************/
//  Description: 设置更新的类型...
//  Global resource dependence
//  Auther: jun.hu
//  Note: 
/****************************************************************************/
PUBLIC void  MMIDCD_SetLatestReqType(MMIDCD_REQ_TYPE_E latest_req_type)
{
    s_latest_req_type = latest_req_type;
}
/****************************************************************************/
//  Description: 设置更新失败的原因代码....
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/11/11
/****************************************************************************/
PUBLIC void  MMIDCD_SetLatestUpdateState(MMIDCD_ERROR_CODE_E error_code)
{
   s_dcd_error = error_code;
}
/****************************************************************************/
//  Description: 返回DCD上次更新状态的失败码
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/13
/****************************************************************************/
PUBLIC MMIDCD_ERROR_CODE_E  MMIDCD_GetLatestUpdateState(void)
{
    //SCI_TRACE_LOW:"[MMIDCD]: MMIDCD_GetLatestUpdateState; dcd latest error_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_233_112_2_18_2_12_49_152,(uint8*)"d",s_dcd_error);
    return s_dcd_error;
}
/****************************************************************************/
//  Description: 设置信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC void  MMIDCD_SetSigDispatchFun(DCD_SigDispatch dcd_sig)
{
    s_SigDispatch = dcd_sig;
}
/****************************************************************************/
//  Description: 获取信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC DCD_SigDispatch  MMIDCD_GetSigDispatchFun(void)
{
    return s_SigDispatch;
}
/****************************************************************************/
//  Description: 设置XmlDoc
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC void  MMIDCD_SetXmlDoc_Ptr(xmlDocPtr xmldoc_ptr)
{
    DeleteRepeatCtxtId(xmldoc_ptr);
    
    s_mmiidle_xml_doc_ptr = xmldoc_ptr;
}
/****************************************************************************/
//  Description: 获取XmlDoc
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC xmlDocPtr MMIAPIDCD_GetXmlDoc_Ptr(void)
{
     return s_mmiidle_xml_doc_ptr;
}
/*****************************************************************************/
//  Description :  dcd error retry update default
//  Global resource dependence : 
//  Author:
//  Note:  disable error update
/*****************************************************************************/
PUBLIC void MMIDCD_ErrorRetrySetDefault(void)
{
     MMIDCD_SetErrorRetryFlag(FALSE);
     s_retry_minutes =5;	//first error retry time:5 min
     s_update_times=0;	
}
/****************************************************************************/
//  Description: 获取当前正在处理的类型...
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/13
/****************************************************************************/
PUBLIC MMIDCD_REQ_TYPE_E  MMIDCD_GetCurProcessReqType(void)
{
    return s_latest_req_type;
}
/*****************************************************************************/
//  Description :  for trace msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL  uint32  DCD_MsgId(uint32 Msgid)
{
   uint32 ret = 0;
   
    if((Msgid >= HTTP_SIG_INIT_CNF)&&(Msgid <= HTTP_SIG_REQUEST_ID_IND))
    {
        ret = (Msgid - HTTP_SIG_INIT_CNF+1); 
    }
    else if((Msgid >= DCD_SIG_INIT_REQ)&&(Msgid <= DCD_SIG_ERROR_IND))
    {
        ret = (Msgid - DCD_SIG_INIT_REQ + DCD_TRACE_LOG_OFFSET); //msg_str offset
    }	
    else
    {
        //DCD_TRACE_LOW:"[MMIDCD] OUT OF RANGE OF DCD MSG"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_306_112_2_18_2_12_49_153,(uint8*)"");
	 ret = 0;	
    }
	return ret;
}

/*****************************************************************************/
//  Description :MMIDCD_IsIdle
//  Global resource dependence :
//  Author:
//  Note: DCD是否在空闲中，供外部调用,
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDCD_IsIdle(void)
{
  return (!s_dcd_is_updating);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:jun.hu
//  Note: 指DCD是否在IDLE状态,
/*****************************************************************************/
LOCAL BOOLEAN IsDCDInIdleState(void)
{
  return (s_dcd_status == DCD_IDLE_STATUS);
}
/*****************************************************************************/
//  Description :  dcd send sig to mmi
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_Sendsig(uint16 signal_code,uint16 para)
{
    DCD_SigDispatch dcd_sigfunc = MMIDCD_GetSigDispatchFun();
    if(PNULL != dcd_sigfunc)
    {
        dcd_sigfunc(signal_code,para);
    }
} 
/*****************************************************************************/
// 	Description : 检查是否能进行DCD的更新动作,返回检查的结果....
//	Global resource dependence : none
//  Author: jun.hu
//	Note: 2009/11/12
/*****************************************************************************/
PUBLIC MMIDCD_ERROR_CODE_E MMIDCD_GetCheckForUpdateResult(void)
{

    MMIDCD_ERROR_CODE_E result = DCD_ERROR_NULL;
    
    // 如果是在飞行模式....
    if(MMIAPISET_GetFlyMode())
    {
        //SCI_TRACE_LOW:"[MMIDCD]: DCD phone is in fly mode now"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_352_112_2_18_2_12_50_154,(uint8*)"");
        return DCD_ERROR_FLY_MODE;
    }
    // 卡的状态的检查.....
    if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MMIAPIDCD_GeSettingInfo()->sim_slected))
    {
        //SCI_TRACE_LOW:"[MMIDCD]:  DCD sim card is not ok"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_358_112_2_18_2_12_50_155,(uint8*)"");
        return DCD_ERROR_SIM_FAIL;
    }
    //  如果U盘在使用中.....
    if(MMIAPIUDISK_UdiskIsRun())
    {    
        //SCI_TRACE_LOW:"[MMIDCD]:  DCD u disk is running"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_364_112_2_18_2_12_50_156,(uint8*)"");
        // 记录下失败原因....//旧的流程是把它作为DCD_ERROR_NO_MEMORY的原因...
        MMIDCD_SetLatestUpdateState(DCD_ERROR_NO_MEMORY);
        return DCD_ERROR_UDISK_USING;
    }

 // 空间不足时也要发起更新的请求，需要让服务器知道有发起的动作....
    // 需要的空间的检查....
    if(MMI_GetFreeSpace(MMIAPIFMM_GetDefaultDisk()) < (MMIDCD_MEMORY_NEEDED_SIZE >> 10))
    {
        //SCI_TRACE_LOW:"[MMIDCD]:  DCD mem size is not enough!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_374_112_2_18_2_12_50_157,(uint8*)"");
        // 记录下失败原因...
        MMIDCD_SetLatestUpdateState(DCD_ERROR_NO_MEMORY);
        return DCD_ERROR_NO_MEMORY;
    } 

    if(!MMIDCD_IsMccNetwork())
    {
        //SCI_TRACE_LOW:"[MMIDCD]:  current network is not cmcc "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_382_112_2_18_2_12_50_158,(uint8*)"");
        return DCD_ERROR_NOT_CMCC;
    }
    // 网络状态的检查...
    if(!MMIAPIPHONE_IsCSAvail(MMIAPIDCD_GeSettingInfo()->sim_slected))
    {
        //SCI_TRACE_LOW:"[MMIDCD]:  network is not valid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_388_112_2_18_2_12_50_159,(uint8*)"");
        // 记录下失败原因...
        MMIDCD_SetLatestUpdateState(DCD_ERROR_INVALID_NETWORK);
        return DCD_ERROR_INVALID_NETWORK;
    }
    // 如果DCD的服务是关闭的....
    if(!MMIAPIDCD_DcdIsOpen())
    {  
        //SCI_TRACE_LOW:"[MMIDCD]:  DCD service is not open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_396_112_2_18_2_12_50_160,(uint8*)"");
        return DCD_ERROR_SET_CLOSE;
    }
    // DCD是否在空闲中的检查....
    if(!MMIDCD_IsIdle())
    {    
        //SCI_TRACE_LOW:"[MMIDCD]:  DCD is busy now"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_402_112_2_18_2_12_50_161,(uint8*)"");
        return DCD_ERROR_IS_BUSY;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDCD]:  DCD status is ok now!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_407_112_2_18_2_12_50_162,(uint8*)"");
    }
    
    return result;
}
/*****************************************************************************/
// 	Description : 设置启动重试机制的标记....
//	Global resource dependence : none
//  Author: jun.hu
//	Note: 2009/11/12
/*****************************************************************************/
PUBLIC void MMIDCD_SetErrorRetryFlag(BOOLEAN retry_flag)
{
    s_is_error_retry = retry_flag;
}
/*****************************************************************************/
// 	Description :...
//	Global resource dependence : none
//  Author: jun.hu
//	Note: 2009/11/12
/*****************************************************************************/
PUBLIC void MMIDCD_ShowDefaultXml(void)
{
//    is_show_default=TRUE;
    //DCD_UpdateUI(TRUE, TRUE, FALSE);
    ParseDCDXMLFile(TRUE);
    MMIDCD_UpdateFrame(FALSE);
}
/*****************************************************************************/
//  Description :DCD_Idle_ProcessMsg
//  Global resource dependence :
//  Author:
//  Note: DCD_Idle_ProcessMsg
/*****************************************************************************/
PUBLIC void DCD_Idle_ProcessMsg(uint16 msg_id,void *param)   
{
    //DCD_TRACE_LOW:"[MMIDCD]:DCD_Idle_ProcessMsg enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_437_112_2_18_2_12_50_163,(uint8*)"");
    
    switch (msg_id) 
    {
        case DCD_SIG_XML_DL_REQ: 
            {
                MMIDCD_REQ_TYPE_E req_type = HTTP_REQ_TYPE_NULL;
                //s_dcd_status=DCD_XML_STATUS;    	
                req_type = (MMIDCD_REQ_TYPE_E)((MmiAppCopyMsgS*)param) ->data;         
                MMIDCD_SetReqType(req_type);
            }
            break;
        
        case DCD_SIG_MIME_DL_CNF:                        
            MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId());    
            break;	
        
        case DCD_SIG_PDP_ACTIVED_CNF:
            //DCD_TRACE_LOW:"[MMIDCD]: http init !!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_455_112_2_18_2_12_50_164,(uint8*)"");
            s_dcd_status = DCD_XML_STATUS;    		
            MMIDCD_SetReqCommonPara(MMIAPICOM_GetUserAgent()); 
            Http_DownloadXMLPre(MODULE_DCD);     
            break;
        
        case DCD_SIG_DL_CANCLE_REQ:
            //DCD_TRACE_LOW:"[MMIDCD]: DCD_SIG_DL_CANCLE_REQ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_462_112_2_18_2_12_50_165,(uint8*)"");
            MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId());             
            break;

        case HTTP_SIG_CLOSE_CNF:    
            DCD_DeactivatePDPcontext();
            break;

        default:
	      //DCD_TRACE_LOW:"[MMIDCD]: default enter"
	      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_471_112_2_18_2_12_50_166,(uint8*)"");
            break;
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_XML_ProcessMsg(uint16 msg_id, void *param)   
{
    //DCD_TRACE_LOW:"[MMIDCD]:DCD_XML_ProcessMsg enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_481_112_2_18_2_12_50_167,(uint8*)"");
    
    switch (msg_id) 
    {
    case HTTP_SIG_INIT_CNF:
        {
            MMI_HTTP_INIT_CNF_SIG_T*   http_recv_sig_ptr=PNULL;
            //SCI_TRACE_LOW:"[MMIDCD]: http init cnf and xml download start!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_488_112_2_18_2_12_50_168,(uint8*)"");
            http_recv_sig_ptr = (MMI_HTTP_INIT_CNF_SIG_T*)param;
            if(PNULL != http_recv_sig_ptr)
            {
                Http_DownloadXML(http_recv_sig_ptr->context_id);  /*send req to http to get xml file  and save the context_id !!*/
            }
        }
        break;
        
    case HTTP_SIG_REQUEST_ID_IND:
        /*remmber the id, for use cancel*/
        {
            MMI_HTTP_REQ_ID_IND_SIG_T*   http_recv_sig_ptr=PNULL;
            http_recv_sig_ptr = (MMI_HTTP_REQ_ID_IND_SIG_T*)param;
            if(PNULL != http_recv_sig_ptr)
            { 
                DCD_HttpSetCurRequtestId(http_recv_sig_ptr->request_id);
            }
        }
        break;
        
        
    case HTTP_SIG_ERROR_IND:
        {
            MMI_HTTP_ERROR_SIGNAL_T*   http_err_sig_ptr=PNULL;
            MMIDCD_ERROR_CODE_E http_error=DCD_ERROR_NULL;
            http_err_sig_ptr = (MMI_HTTP_ERROR_SIGNAL_T*)param;
            http_error = Http_ErrorHandler(http_err_sig_ptr->result);//modofied by @jun.hu, 2009/8/13
            DCD_Sendsig(DCD_SIG_ERROR_IND,http_error);
        }
        break;
        
        
    case HTTP_SIG_HEADER_IND:
        break;
        
#ifdef DCD_BUFFER_INTERPRETE           
    case HTTP_SIG_DATA_IND:
        break;	 
#endif     
        
    case HTTP_SIG_GET_CNF:       /*when afte xml-parser, 1st time build the need download data req list:s_dcd_http_req_list_needed (image/gif/mm)  **/
        {
            Http_FreeXmlReqList();   
            s_dcd_status=DCD_MIME_STATUS;		
            DCD_Sendsig(DCD_SIG_XML_DL_CNF,HTTP_REQ_TYPE_NULL);
            /*maybe here need parser xml file!!!!!!!!*/
            
        } 
        
        break;
        
    case DCD_SIG_DL_CANCLE_REQ:
        MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId());    /*http deinit*/            
        break;
        
    case HTTP_SIG_CANCEL_CNF:
        {
            HTTP_CLOSE_CNF_SIG_T *signal = (HTTP_CLOSE_CNF_SIG_T*)((uint32)param-sizeof(xSignalHeaderRec));
            if(NULL != signal && (DCD_HttpGetCurSessionId() == signal->context_id))
            {
                MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId()); 
            }
        }
        break;
    case HTTP_SIG_CLOSE_CNF:  
        {
            HTTP_CLOSE_CNF_SIG_T *signal = (HTTP_CLOSE_CNF_SIG_T*)((uint32)param-sizeof(xSignalHeaderRec));
            if(NULL != signal && (DCD_HttpGetCurSessionId() == signal->context_id))
            {
                DCD_DeactivatePDPcontext();
                DCD_HttpSetCurSessionId(0);
            }
        }
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//  Description :DCD_Idle_ProcessMsg
//  Global resource dependence :
//  Author:
//  Note: DCD_Idle_ProcessMsg
/*****************************************************************************/
PUBLIC void DCD_MIME_ProcessMsg(uint16 msg_id,void *param)   
{
    //DCD_TRACE_LOW:"[MMIDCD]:DCD_MIME_ProcessMsg enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_575_112_2_18_2_12_50_169,(uint8*)"");
    
    switch (msg_id) 
    {
    case DCD_SIG_MIME_DL_REQ:	
        {
            Http_ContentDownloadPre();
            Http_ContentDownloadProcess();        /*this function support buffer parser to parser mime buffer */ 
            //SCI_TRACE_MID:"MMIDCD Http_ContentDownloadProcess finish time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_583_112_2_18_2_12_50_170,(uint8*)"d", SCI_GetTickCount());
        }
        break; 
        
    case HTTP_SIG_REQUEST_ID_IND:
        /*remmber the id, for use cancel*/
        {
            MMI_HTTP_REQ_ID_IND_SIG_T*   http_recv_sig_ptr=PNULL;
            http_recv_sig_ptr = (MMI_HTTP_REQ_ID_IND_SIG_T*)param;
            if(PNULL != http_recv_sig_ptr)
            {
                DCD_HttpSetCurRequtestId(http_recv_sig_ptr->request_id);
            }
        }
        break;
        
#ifdef DCD_BUFFER_INTERPRETE  
    case HTTP_SIG_HEADER_IND:
        
        break;          
        
    case HTTP_SIG_DATA_IND:
        {
            MMI_HTTP_CONTENT_IND_SIG_T*  http_recv_sig_ptr=PNULL;
            http_recv_sig_ptr = (MMI_HTTP_CONTENT_IND_SIG_T*)param;
            if(PNULL != http_recv_sig_ptr)
            {
                Http_ContentInterpreteProcess((uint8*)http_recv_sig_ptr->buffer,http_recv_sig_ptr->content_len);		/*this function support buffer parser to parser mime buffer */ 	                	                
            }
        }            
        
        break;	 
#endif     
        
    case HTTP_SIG_GET_CNF:    
        if(Http_ContentInterpreteProcess())
        {
            Http_ContentDownloadProcess();  
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDCD]: HTTP_SIG_GET_CNF, parse fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_624_112_2_18_2_12_50_171,(uint8*)"");
            DCD_Sendsig(DCD_SIG_ERROR_IND,DCD_ERROR_WRONG_DATA);     
        }
        break;
        
    case DCD_SIG_MIME_DL_CNF:                        /*when pop null,  get the message and deinit http*/
        MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId());    /*http deinit*/
        //s_dcd_status=DCD_IDLE_STATUS;
        break;		
        
    case HTTP_SIG_ERROR_IND:
        {
            MMI_HTTP_ERROR_SIGNAL_T*   http_err_sig_ptr=PNULL;
            MMIDCD_ERROR_CODE_E http_error=DCD_ERROR_NULL;
            http_err_sig_ptr = (MMI_HTTP_ERROR_SIGNAL_T*)param;
            http_error = Http_ErrorHandler(http_err_sig_ptr->result);//modofied by @jun.hu, 2009/8/13
            DCD_Sendsig(DCD_SIG_ERROR_IND,http_error);
        }
        break;
        
    case DCD_SIG_DL_CANCLE_REQ:
        MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId());    /*http deinit*/      
        break;
        
    case HTTP_SIG_CANCEL_CNF:
        {
            HTTP_CLOSE_CNF_SIG_T *signal = (HTTP_CLOSE_CNF_SIG_T*)((uint32)param);
            if(NULL != signal && (DCD_HttpGetCurSessionId() == signal->context_id))
            {
                MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId()); 
            }
        }
        break;
    case HTTP_SIG_CLOSE_CNF:  
        {
            HTTP_CLOSE_CNF_SIG_T *signal = (HTTP_CLOSE_CNF_SIG_T*)((uint32)param);
            if(NULL != signal && (DCD_HttpGetCurSessionId() == signal->context_id))
            {
                DCD_DeactivatePDPcontext();
                DCD_HttpSetCurSessionId(0);
            }
        }
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// Description : DCD NULL status handler
// Global resource dependence : 
// Author:Zhuo Wang
// Note: 
/*****************************************************************************/
PUBLIC void DCD_Null_ProcessMsg(uint16 msg_id,void *param)   
{
	//DCD_TRACE_LOW:"[MMIDCD]:DCD_Null_ProcessMsg enter"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_679_112_2_18_2_12_50_172,(uint8*)"");
	switch (msg_id) 
	{
	case DCD_SIG_INIT_REQ:  
		DCD_Sendsig(DCD_SIG_INIT_CNF,HTTP_REQ_TYPE_NULL);
		break;
		
	case DCD_SIG_INIT_CNF:
		s_dcd_status=DCD_IDLE_STATUS;
		break;        
	default:
		break;
	}
    
}

/*****************************************************************************/
// Description : DCD NULL status handler
// Global resource dependence : 
// Author:Zhuo Wang
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DCD_AppMsgHandler(uint16 msg_id,void *param)
{
	BOOLEAN ret = TRUE;
	
	//DCD_TRACE_LOW:"[MMIDCD]: msg %s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_703_112_2_18_2_12_50_173,(uint8*)"s",msg_str[DCD_MsgId(msg_id)]);
	DCD_TRACE_LOW(("[MMIDCD]: msg %s",dcd_state_str[s_dcd_status-DCD_NULL_STATUS]));/*lint !e656*/
	switch(s_dcd_status)
	{
	case DCD_IDLE_STATUS:
		DCD_Idle_ProcessMsg(msg_id,param);   
		break;
	case DCD_XML_STATUS:
		DCD_XML_ProcessMsg(msg_id,param);   
		break;
	case DCD_MIME_STATUS:
		DCD_MIME_ProcessMsg(msg_id,param);             
		break;
	case DCD_NULL_STATUS:
		DCD_Null_ProcessMsg(msg_id,param);   
		break;
	default: 
		break;
	}
	
	return ret;
	
}
/*****************************************************************************/
// Description : DCD sim error indication
// Global resource dependence : 
// Author:Zhuo Wang
// Note: 
/*****************************************************************************/
PUBLIC void  MMIDCD_SimErrorInd(void)
{
	MMI_TEXT_ID_T 	text_id = TXT_NULL;
	SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(MMIAPIDCD_GeSettingInfo()->sim_slected);
	//SCI_TRACE_LOW:"[MMIDCD]: MMIDCD_SimErrorInd(),  sim_status= %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_733_112_2_18_2_12_50_174,(uint8*)"d",sim_status);
    if(sim_status <= SIM_STATUS_MAX)
    {
        text_id = s_sim_status_string[sim_status];
    }	
	if (text_id != TXT_NULL)
	{
		MMIPUB_OpenAlertWarningWin(text_id);
	}   
	
}

/*****************************************************************************/
// Description :  dcd handle http msg function!
// Global resource dependence : 
// Author:Zhuo Wang
// Note: modified by jun.hu 2009/11/12
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleDCDProcessMsg(
                                        PWND app_ptr,
                                        uint16 msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};

    //SCI_TRACE_LOW:"HandleDCDProcessMsg() enter msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_759_112_2_18_2_12_50_175,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
       case DCD_SIG_XML_DL_REQ:        
        {
            MMIDCD_ERROR_CODE_E error_type = DCD_ERROR_NULL;
            
            error_type = MMIDCD_GetCheckForUpdateResult();	
            s_dcd_is_updating = TRUE;
            //SCI_TRACE_LOW:"HandleDCDProcessMsg() DCD_SIG_XML_DL_REQ error_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_769_112_2_18_2_12_50_176,(uint8*)"d",error_type);
            MMIDCD_SetLatestUpdateState(error_type);
            TM_GetSysDate(&sys_date);
            TM_GetSysTime(&sys_time);
            
            MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_year=sys_date.year;
            MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_mday=sys_date.mday;   
            MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_mon=sys_date.mon;
            
            MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_hour=sys_time.hour;
            MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_min=sys_time.min;
            MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_sec = sys_time.sec;
            MMIAPIDCD_WriteSettingToNv();

            if((PNULL != param) && (error_type!=DCD_ERROR_IS_BUSY))
            {
                s_latest_req_type=((MmiAppCopyMsgS*)param)->data;       /*lint !e64*/     
            }
            
            if(DCD_ERROR_NULL == error_type)
            {
                // 让动画控件动起来...... 
                MMIDCD_SetAnimateCtrlState(MMIDCD_ANIM_PLAY);           
     
                MMIDCDFILE_SetUpDateNewItemsCount(0); //设置更新获得的新条目数....      
#ifndef WIN32
                result=DCD_ActivatePDPcontext();   
                if(!result)
                {
                    //SCI_TRACE_LOW:"[MMIDCD]:dcd pdp active fail!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_798_112_2_18_2_12_50_177,(uint8*)"");
                    DCD_Sendsig(DCD_SIG_ERROR_IND,DCD_ERROR_INVALID_NETWORK);
                    result = MMI_RESULT_FALSE;             
                }
#else
                DCD_Sendsig(DCD_SIG_PDP_ACTIVED_CNF,HTTP_REQ_TYPE_NULL);      
#endif        
            }
            else
            {
                if(error_type != DCD_ERROR_IS_BUSY)
                {
                    DCD_Sendsig(DCD_SIG_ERROR_IND,error_type);
                }				
                result = MMI_RESULT_FALSE;				            
            }	 	
        }
        break;
        
    case HTTP_SIG_INIT_CNF:
        {
            MMI_HTTP_INIT_CNF_SIG_T*   http_recv_sig_ptr=PNULL;
            uint16 http_result=0;
            http_recv_sig_ptr = (MMI_HTTP_INIT_CNF_SIG_T*)param;
            if(PNULL == http_recv_sig_ptr)
            {
                break;
            }
            http_result=http_recv_sig_ptr->result;
#ifndef WIN32         
            if(http_result!=0)
            {
                //SCI_TRACE_LOW:"[MMIDCD]: http init cnf and error!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_830_112_2_18_2_12_50_178,(uint8*)"");
                DCD_Sendsig(DCD_SIG_ERROR_IND,DCD_ERROR_INVALID_NETWORK);
                result=MMI_RESULT_FALSE;
            }
#endif
            //SCI_TRACE_LOW:"[MMIDCD]: http init cnf and ok!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_835_112_2_18_2_12_50_179,(uint8*)"");
        }
        break;
    case DCD_SIG_INIT_REQ:        
    case HTTP_SIG_ERROR_IND:
    case HTTP_SIG_HEADER_IND:    
    case HTTP_SIG_REQUEST_ID_IND:
    case HTTP_SIG_DATA_IND:
    case HTTP_SIG_GET_CNF:
    case HTTP_SIG_CANCEL_CNF:		
	 break;	

    case HTTP_SIG_CLOSE_CNF:
        break;
        
    /*增加 cancel update流程保护*/
    case DCD_SIG_PDP_ACTIVED_CNF:
        if((MMK_IsOpenWin(MMIDCD_STOP_UPDATING_WAITING_WIN_ID)) || (MMK_IsOpenWin(MMIDCD_RESET_FACTORY_WAITING_WIN_ID)))
        {
            DCD_DeactivatePDPcontext();
            result=MMI_RESULT_FALSE;			
        }
        break;

    case DCD_SIG_DL_CANCLE_REQ:
        s_update_is_cancling = TRUE;	
        s_dcd_error = DCD_ERROR_NULL;		
		break;

    case DCD_SIG_PDP_DEACTIVED_CNF:	
        {
             if(!MMIDCD_IsIdle())
            {
                s_dcd_status=DCD_IDLE_STATUS;	
                DCD_Sendsig(DCD_SIG_DL_OVER_IND, HTTP_REQ_TYPE_NULL);
            }
        }
        break;  
    case DCD_SIG_MIME_DL_REQ:
	 break;      

    case DCD_SIG_INIT_CNF:
        break;
        
    case DCD_SIG_XML_DL_CNF:            
        {
            BOOLEAN parse_result=FALSE;
            //DCD_TRACE_LOW:"[MMIDCD]: xml download is end!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_882_112_2_18_2_12_51_180,(uint8*)"");
            //is_show_default=FALSE;
            //parse_result = DCD_UpdateUI(TRUE,TRUE, FALSE);  
            parse_result = ParseDCDXMLFile(FALSE);
            MMIDCD_UpdateFrame(FALSE);
            if(parse_result)
            {
                DCD_Sendsig(DCD_SIG_MIME_DL_REQ,HTTP_REQ_TYPE_NULL);                        
            }
            else
            {
                DCD_Sendsig(DCD_SIG_ERROR_IND,DCD_ERROR_NO_XML);                        
            }
            MMIDCD_IndicationShow(s_latest_req_type,DCD_ERROR_NULL,FALSE);
            result=MMI_RESULT_FALSE;
        }
        break;
        
    case DCD_SIG_MIME_DL_CNF:
        s_dcd_error = DCD_ERROR_NULL; //added by @jun.hu, 2009/8/13
        MMIDCD_ErrorRetrySetDefault();
        MMIDCD_SetDefaultRetryTime();
        break;
   case DCD_SIG_DEFALUT_RETRY_TIMER_IND:
        //DCD_TRACE_LOW:"[MMIDCD]: receive DCD_SIG_DEFALUT_RETRY_TIMER_IND!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_906_112_2_18_2_12_51_181,(uint8*)"");
        MMIDCD_SetDefaultRetryTime();
        break;   
    case DCD_SIG_ERROR_IND:
        {
            MMIDCD_ERROR_CODE_E  error_type = (MMIDCD_ERROR_CODE_E)(((MmiAppCopyMsgS*)param) ->data);            
            
            MMIDCD_SetErrorRetryFlag(MMIDCD_IndicationShow(s_latest_req_type,error_type,FALSE)); 
            
            MMIDCD_SetLatestUpdateState(error_type);
            MMIDCD_SetUpdateStopFlag(FALSE);
            if(error_type == DCD_ERROR_UDISK_USING)
            {
                s_dcd_error = DCD_ERROR_NO_MEMORY;
            }
            else if(error_type< DCD_ERROR_XML_FORMAT)
            {
                s_dcd_error = DCD_ERROR_INVALID_NETWORK;
            }
            else			
            {
                s_dcd_error=error_type;
            }
            
            MMIDCD_HttpSessionClose(DCD_HttpGetCurSessionId());    /*http deinit*/          
            
            MMIDCD_SetAnimateCtrlState(MMIDCD_ANIM_STOP); // for cr180348
            //is_show_default=FALSE; 
            //s_dcd_status      = DCD_IDLE_STATUS;
            //s_dcd_is_updating = FALSE;
            if(MMIAPIDCD_GeSettingInfo()->is_auto_update && s_is_error_retry)
            {
                MMIDCD_StartDCDTimer(HTTP_REQ_TYPE_RETRY, 300);
            }
        }
        result = MMI_RESULT_FALSE;
        break;
        
    case DCD_SIG_DL_OVER_IND:
        //is_show_default   = FALSE;
        s_dcd_status      = DCD_IDLE_STATUS;
        s_dcd_is_updating = FALSE;
        MMIDCD_SetUpdateStopFlag(FALSE);
        
        if(MMIAPIDCD_GeSettingInfo()->is_auto_update && 0 != s_ttl_second && DCD_ERROR_NULL == s_dcd_error)
        {
            MMIDCD_StartDCDTimer(HTTP_REQ_TYPE_TTL, s_ttl_second);
        }
 
        MMIDCD_SetResetFinishedInd();		
        MMIDCD_SetAnimateCtrlState(MMIDCD_ANIM_STOP); 
        MMIDCD_UpdateFrame(TRUE);    
        if(!s_update_is_cancling)
        {
            MMIDCD_ShowUpdateComplete(s_latest_req_type, s_dcd_error);
        }
        else
        {   // 恢复变量.....
            s_update_is_cancling = FALSE;
        }

        if(MMIAPIDCD_GeSettingInfo()->is_dcd_update_log_on)
        {
            MMIDCDFILE_WriteUpdateInfo(s_latest_req_type, s_dcd_error);
        }
        // over 消息收到后处理结束了，不需要再交给各个状态去处理....
        return MMI_RESULT_TRUE;
        //break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }


    if(result == MMI_RESULT_TRUE)
    { 
        result = DCD_AppMsgHandler( msg_id,param);		
    }

    //SCI_TRACE_LOW:"HandleDCDProcessMsg()  exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_985_112_2_18_2_12_51_182,(uint8*)"");

    return (result);
}

/*****************************************************************************/
//  Description : 对Ctxt_id 的唯一性进行判断处理
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:  
//频道ID    : 频道ID 和频道项ID 互相不影响，因此不去判断
//频道项ID  : 仅判断频道内部的频道项，为了速度不考虑和其他频道的频道项比较
//内容ID    : 内容ID 重复的时候不去下载就好了，没有必要去比较
/*****************************************************************************/
LOCAL void DeleteRepeatCtxtId(xmlDocPtr xml_doc_ptr)
{
    DCD_FEED_T* feed_ptr_src = PNULL;
    DCD_FEED_T* feed_ptr_dst = PNULL;    
    DCD_FEED_T* feed_ptr_pre = PNULL;
    DCD_ENTRY_T *entry_ptr_src = PNULL;
    DCD_ENTRY_T *entry_ptr_dst = PNULL;
    DCD_ENTRY_T *entry_ptr_pre = PNULL;    
    uint16 feed_num  = 0; // 频道数
    uint16 entry_num  = 0; // 频道项数
    uint16 i =0;
    uint16 j =0;
    uint16 k =0;
    
    if (PNULL == xml_doc_ptr)
    {
        return;
    }

    //删除重复频道
    feed_num  = MMIDCD_GetRealFeedNum(xml_doc_ptr); // 频道数
    
    for(i = 0; i < feed_num; i++)
    {
        //当前频道
        feed_ptr_src = MMIDCD_GetFeedByIndex(xml_doc_ptr->feed_list_ptr, i);
        
        if(PNULL != feed_ptr_src)
        {
            for(j = feed_num-1; j>i; j--)
            {
                //要比较的频道
                feed_ptr_dst = MMIDCD_GetFeedByIndex(xml_doc_ptr->feed_list_ptr, j);
                
                if(PNULL != feed_ptr_dst)
                {
                    if(0 == strcmp(feed_ptr_src->ctxt_id, feed_ptr_dst->ctxt_id))
                    {
                        //SCI_TRACE_MID:"[MMIDCD] feed Ctxt_Id repeat = %s"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1035_112_2_18_2_12_51_183,(uint8*)"s", feed_ptr_dst->ctxt_id);

                        //删除重复的频道
                        feed_ptr_pre = MMIDCD_GetFeedByIndex(xml_doc_ptr->feed_list_ptr, j-1);
                        
                        if(PNULL != feed_ptr_pre)
                        {
                            feed_ptr_pre->next_ptr = feed_ptr_dst->next_ptr;
                        }

                        MMIDCD_FeedFree(feed_ptr_dst);
                        feed_num--;
                    }
                }
            }
        }
    }


    //删除重复频道项
    feed_num  = MMIDCD_GetRealFeedNum(xml_doc_ptr); // 频道数
    
    for(k = 0; k < feed_num; k++)
    {
        entry_num = MMIDCD_GetRealEntryNum(xml_doc_ptr, k);            
        for(i = 0; i < entry_num; i++)
        {
            entry_ptr_src = MMIDCD_GetEntryByIndex(xml_doc_ptr, k, i);
            if(PNULL != entry_ptr_src)
            {
                for(j = entry_num-1; j>i; j--)
                {
                    //要比较的频道项
                    entry_ptr_dst = MMIDCD_GetEntryByIndex(xml_doc_ptr, k, j);
                    
                    if(PNULL != entry_ptr_dst)
                    {
                        if(0 == strcmp(entry_ptr_src->ctxt_id, entry_ptr_dst->ctxt_id))
                        {
                            //SCI_TRACE_MID:"[MMIDCD] entry Ctxt_Id repeat = %s"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1074_112_2_18_2_12_51_184,(uint8*)"s", entry_ptr_dst->ctxt_id);

                            //删除重复的频道项
                            entry_ptr_pre = MMIDCD_GetEntryByIndex(xml_doc_ptr, k, j-1);
                            if(PNULL != entry_ptr_pre)
                            {
                                entry_ptr_pre->next_ptr = entry_ptr_dst->next_ptr;
                            }

                            MMIDCD_EntryFree(entry_ptr_dst);
                            entry_num--;
                        }
                    }
                }
            }
        }
    }
}    

/*****************************************************************************/
//  Description :开机解析函数, 此时还没有去更新，所以不需要比较两个xml 文件...
//  Global resource dependence : none
//  Author: jun.hu 
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN ParseDCDXMLFileWhenPowerOn(BOOLEAN is_default_parse)
{
    xmlDocPtr            xml_doc_ptr = PNULL;
    XML_USERDATA         *xml_user_data_ptr = PNULL;
    DCD_GLOBAL_META_T    *gobal_meta_ptr = PNULL;
    wchar       wchar_xml_file_path_ptr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};

    uint16    path_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN   result   = FALSE;

    //SCI_TRACE_MID:"[MMIDCD]:ParseDCDXMLFileWhenPowerOn enter!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1107_112_2_18_2_12_51_185,(uint8*)"");

    // 解析前的准备.....
    if(PNULL != s_mmiidle_xml_doc_ptr)
    {
        MMIDCD_XMLDataFree(FALSE);  //对当前的xml文件对应内存影像数据的释放...
        s_mmiidle_xml_doc_ptr = PNULL; 
    }

    if(!MMIDCDFILE_GetFilePathForRead(wchar_xml_file_path_ptr , &path_len, MMIDCD_INDEX_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1118_112_2_18_2_12_51_186,(uint8*)"");
    }

    
    if(MMIDCD_XmlParse((uint16*)wchar_xml_file_path_ptr, FALSE))
    {
        /* when parse suceessful ! */
        //SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse() case xml file parse return TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1125_112_2_18_2_12_51_187,(uint8*)"");
        
        xml_user_data_ptr = MMIDCD_GetXMLData(FALSE);
        
        xml_doc_ptr =  &(xml_user_data_ptr ->sync_pkg);

        if(0 < MMIDCD_GetFeedNum(xml_doc_ptr))
        {
            MMIDCDFILE_ReadItemsState();
            result = TRUE;
        }
        else// 如果解析后的频道数为0， 则启用默认的预制项....
        {
            result = FALSE;
        }
    }
    // 如果解析失败或者解析后发现频道数为0， 则起用默认的DCD项目....
    if(!result)
    {   
        //SCI_TRACE_MID:"[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1144_112_2_18_2_12_51_188,(uint8*)"");
        MMIDCD_XMLDataFree(FALSE);
        
        MMIDCDFFS_ParseDefaultXMLFile();
        xml_user_data_ptr = MMIDCD_GetXMLData(FALSE);
        
        xml_doc_ptr =  &(xml_user_data_ptr ->sync_pkg);                               
        
        MMIDCD_Init_XML_Node_State();
        
    }
    
    // 解析结束了以后，记下解析形成的链表...
    MMIDCD_SetXmlDoc_Ptr(xml_doc_ptr); 
    
    if(PNULL != xml_doc_ptr)
    {
        gobal_meta_ptr = &(xml_doc_ptr->meta.global_meta);
        s_ttl_second = gobal_meta_ptr->global_meta_ttl;
        //SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse:global_meta_ttl= %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1163_112_2_18_2_12_51_189,(uint8*)"d",s_ttl_second);
    }
    
    //SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse() exit!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1166_112_2_18_2_12_51_190,(uint8*)"");

    return result;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:zhoufeng
//  Note: modified by jun.hu 
/*****************************************************************************/
LOCAL BOOLEAN ParseDCDXMLFile(BOOLEAN is_default_parse)
{
    xmlDocPtr             xml_doc_ptr            = PNULL;
    XML_USERDATA          *xml_user_data_ptr     = PNULL;
    DCD_GLOBAL_META_T     *gobal_meta_ptr        = PNULL;
    xmlDocPtr             older_xml_ptr          = PNULL;
    XML_USERDATA          *old_xml_user_data_ptr = PNULL;
    BOOLEAN   result                             = FALSE;
    BOOLEAN   parse_old_fail                     = TRUE;
    uint16    path_len                           = MMIFILE_FULL_PATH_MAX_LEN;

    wchar     wchar_xml_file_path_ptr[MMIFILE_FILE_NAME_MAX_LEN + 1]     = {0};
    wchar     wchar_old_xml_file_path_ptr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
	
    BOOLEAN parse_xml_result = FALSE;
	
    //SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse() enter!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1190_112_2_18_2_12_51_191,(uint8*)"");

    // 解析前的准备.....
    if(PNULL != s_mmiidle_xml_doc_ptr)
    {
        MMIDCD_XMLDataFree(FALSE);  //对当前的xml文件对应内存影像数据的释放...
        s_mmiidle_xml_doc_ptr = PNULL; 
    }

    if(!MMIDCDFILE_GetFilePathForRead(wchar_xml_file_path_ptr , &path_len, MMIDCD_INDEX_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1201_112_2_18_2_12_51_192,(uint8*)"");
    }

    if(is_default_parse)
    {
        MMIDCD_Init_XML_Node_State(); 
        //SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse() case default xml parse!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1207_112_2_18_2_12_51_193,(uint8*)"");
        MMIDCD_XMLDataFree(FALSE);
        
        MMIDCDFFS_ParseDefaultXMLFile();
        xml_user_data_ptr = MMIDCD_GetXMLData(FALSE);
        
        xml_doc_ptr =  &(xml_user_data_ptr ->sync_pkg); 
        //read xml node state
        // 这里旧的流程有问题，上面已经清掉了，下面还从文件里面读，解析默认的不需要从文件里读...
        //ReadXMLNodeFlag(xml_file_path_ptr);
        result = TRUE;
    }
    else
    {
		parse_xml_result = MMIDCD_XmlParse((uint16*)wchar_xml_file_path_ptr, FALSE);
		if(parse_xml_result)
       	{
			/* when parse suceessful ! */
			//SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse() case xml file parse return TRUE"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1225_112_2_18_2_12_51_194,(uint8*)"");
			
			xml_user_data_ptr = MMIDCD_GetXMLData(FALSE);
			
			xml_doc_ptr =  &(xml_user_data_ptr ->sync_pkg);
			
			/*added by jun.hu 2009/10/30 for 修改解析流程......., begin+*/
			MMIDCDFILE_ReadItemsState();
			// 再解析旧的文件......
			// 因为DCD每次下载更新前会把XML重命名为old.xml文件保存下来...
			if(MMIDCDFILE_GetFilePathForRead(wchar_old_xml_file_path_ptr , &path_len, MMIDCD_OLD_FILE_PATH))
			{
				MMIDCD_XMLDataFree(TRUE);
				if(MMIDCD_XmlParse(wchar_old_xml_file_path_ptr, TRUE))
				{
					parse_old_fail = FALSE;
					old_xml_user_data_ptr = MMIDCD_GetXMLData(TRUE);
					if(PNULL != old_xml_user_data_ptr)
					{
						older_xml_ptr =  &(old_xml_user_data_ptr ->sync_pkg);
						// 下面的这个判断可以去掉，它们都是获取的有效地址.....
						//if((PNULL != xml_doc_ptr) && (PNULL != older_xml_ptr))
						{   
							MMIDCDFILE_CmpTwoXmlFile(xml_doc_ptr, older_xml_ptr);
						}
					}
				}
				else
				{
					//SCI_TRACE_LOW:"[MMIDCD] local xml file parse fail"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1254_112_2_18_2_12_51_195,(uint8*)"");
				}
			}
			else
			{
				//SCI_TRACE_LOW:"[MMIDCD]MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1259_112_2_18_2_12_51_196,(uint8*)"");
			}
			if(parse_old_fail) 
			{
				// 如果旧的文件没有解析成功，则新的文件中的所有频道项都计入新的频道项数......
				MMIDCDFILE_SetUpDateNewItemsCount(MMIDCDFILE_GetXMLItemsCount(xml_doc_ptr));
				MMIDCD_Init_XML_Node_State(); //for cr171363
			}
			MMIDCD_XMLDataFree(TRUE);
			/*added by jun.hu 2009/10/30 for 修改解析流程......., end-*/
			
			result = TRUE;
       	}
		else
		{
			/*when parse fail*/
			//SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse() case xml file parse return FALSE"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1275_112_2_18_2_12_52_197,(uint8*)"");
			
			//deleted by jun.hu 2009/10/31 for 这里旧的流程先时清掉，再往里面写,这样会冲掉旧的文件里的数据....
			//WriteXMLNodeFlag(MMIDCDFFS_GetXmlFilePath());
			
			MMIDCD_XMLDataFree(FALSE);
			//这个地方解析失败的话直接从资源读数据来显示默认信息
			
			// 此时取更新前的文件，并解析之
			MMIDCDFILE_RenameXMLFile(MMIDCD_OLD_FILE_PATH, MMIDCD_INDEX_FILE_PATH);
			if( MMIDCD_XmlParse((uint16*)wchar_xml_file_path_ptr, FALSE))
			{
				xml_user_data_ptr = MMIDCD_GetXMLData(FALSE);
				xml_doc_ptr =  &(xml_user_data_ptr ->sync_pkg);
				
				//SCI_TRACE_MID:"[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return TRUE"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1290_112_2_18_2_12_52_198,(uint8*)"");
				
				MMIDCDFILE_ReadItemsState();
			}
			else
			{
				//SCI_TRACE_MID:"[MMIDCD]:MMIAPIDCD_Parse() case old xml file DCDXML_ParseXMLFile return FALSE"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1296_112_2_18_2_12_52_199,(uint8*)"");
			}
			
		}
		// 如果解析后的频道数为0， 则启用默认的预制项....
		if(0 == MMIDCD_GetFeedNum(xml_doc_ptr))
		{
			//SCI_TRACE_MID:"[MMIDCD]:has parse fail or pasrse success but has zero feed, so use default xml"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1303_112_2_18_2_12_52_200,(uint8*)"");
			MMIDCD_XMLDataFree(FALSE);

			MMIDCDFFS_ParseDefaultXMLFile();
			xml_user_data_ptr = MMIDCD_GetXMLData(FALSE);

			xml_doc_ptr =  &(xml_user_data_ptr ->sync_pkg);                               

			MMIDCD_Init_XML_Node_State();
		}
    }
    

    // 解析结束了以后，记下解析形成的链表...
    MMIDCD_SetXmlDoc_Ptr(xml_doc_ptr); 
    
    if(PNULL != xml_doc_ptr)
    {
        gobal_meta_ptr = &(xml_doc_ptr->meta.global_meta);
        s_ttl_second = gobal_meta_ptr->global_meta_ttl;
        //SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse:global_meta_ttl= %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1323_112_2_18_2_12_52_201,(uint8*)"d",s_ttl_second);
    }
    	  
    //SCI_TRACE_MID:"[MMIDCD]:MMIDCD_Parse() exit!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1326_112_2_18_2_12_52_202,(uint8*)"");

    return result;
}



/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:zhuowang
//	Note: modified by jun.hu 2009.12.7 1: 规范变量的定量，使用wchar, 2:调整流程等
//        2009/12/19 jun.hu 修改对文件名的操作
/*****************************************************************************/
PUBLIC void MMIDCD_RestoreFactorySetting(void)
{
    MMIFILE_ERROR_E file_error = SFS_NO_ERROR;
    wchar     wchar_xml_file_path_ptr[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16    full_path_len = MMIFILE_FULL_PATH_MAX_LEN; 

    /*delete index.xml file*/
    if(!MMIDCDFILE_GetFilePathForRead(wchar_xml_file_path_ptr , &full_path_len, MMIDCD_INDEX_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1347_112_2_18_2_12_52_203,(uint8*)"");
    }
    file_error = MMIAPIFMM_DeleteFile(wchar_xml_file_path_ptr, PNULL);
    if(SFS_NO_ERROR != file_error)
    {
        //SCI_TRACE_LOW:"[MMIDCD]: the index.xml file delete fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1352_112_2_18_2_12_52_204,(uint8*)"");
    }
    	
    
    /*delete local.xml file*/	
    SCI_MEMSET(wchar_xml_file_path_ptr, 0, ((MMIFILE_FULL_PATH_MAX_LEN)*sizeof(wchar)));				
    if(!MMIDCDFILE_GetFilePathForRead(wchar_xml_file_path_ptr , &full_path_len, MMIDCD_LOCAL_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1360_112_2_18_2_12_52_205,(uint8*)"");
    }
    
    file_error = MMIAPIFMM_DeleteFile(wchar_xml_file_path_ptr,PNULL);
    if(SFS_NO_ERROR != file_error)
    {
        //SCI_TRACE_LOW:"[MMIDCD]: the local.xml file delete fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1366_112_2_18_2_12_52_206,(uint8*)"");
    }
    

    // delete old.xml file jun.hu 2009/12/7 for cr164172..
    SCI_MEMSET(wchar_xml_file_path_ptr,0,((MMIFILE_FULL_PATH_MAX_LEN)*sizeof(wchar)));	
    if(!MMIDCDFILE_GetFilePathForRead(wchar_xml_file_path_ptr , &full_path_len, MMIDCD_OLD_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1374_112_2_18_2_12_52_207,(uint8*)"");
    }
    file_error = MMIAPIFMM_DeleteFile(wchar_xml_file_path_ptr, PNULL);
    if(SFS_NO_ERROR != file_error)
    {
        //SCI_TRACE_LOW:"[MMIDCD]: the old.xml file delete fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1379_112_2_18_2_12_52_208,(uint8*)"");
    }
    
    /* must first update xml and then delete the local files!! */    
    MMIDCD_ShowDefaultXml();                 
    MMIDCDFILE_DeleteLocalPictures();
    MMIDCDFILE_FindUseablePictures();

    MMIDCD_SetSettingDefault();
    MMIDCD_HandleDCDSwitch(MMIAPIDCD_DcdIsOpen(), TRUE);
    
    
}
/*****************************************************************************/
// 	Description : open input homepage window of dcd
//	Global resource dependence : none
//  Author: 
//	Note: jun.hu 2009/9/9 修改了此函数.........
/*****************************************************************************/
LOCAL BOOLEAN DCD_ActivatePDPcontext(void)
{
    BOOLEAN return_val = FALSE;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    uint8 setting_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[MMIDCD]:DCD_ActivatePDPcontext enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1403_112_2_18_2_12_52_209,(uint8*)"");
    
    app_info.app_handler = MMI_MODULE_DCD;
    app_info.dual_sys = MMIAPIDCD_GeSettingInfo()->sim_slected;
    setting_index = MMIDCD_Setting_GetNetSettingIndex(app_info.dual_sys);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(app_info.dual_sys, setting_index);
    if(PNULL != setting_item_ptr)
    {
        app_info.auth_type = setting_item_ptr->auth_type;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.priority = 3; //added by jun.hu 11.14 for给它设一个优先级.....
        app_info.ps_service_rat = MN_UNSPECIFIED;

        app_info.handle_msg_callback = DcdHandlePDPMsg;
        app_info.ps_service_type = BROWSER_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;

        return_val = MMIAPIPDP_Active(&app_info);
    }
    
    return return_val;    
	
}


/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DCD_DeactivatePDPcontext(void)
{
    //SCI_TRACE_LOW:"[MMIDCD]:DCD_DeactivatePDPcontext enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1438_112_2_18_2_12_52_211,(uint8*)"");
#ifndef WIN32	
    MMIAPIPDP_Deactive(MMI_MODULE_DCD);
#endif
    DCD_Sendsig(DCD_SIG_PDP_DEACTIVED_CNF,HTTP_REQ_TYPE_NULL);
    return TRUE;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:zhuo.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCD_DeactivatePDPcontext(void)
{
    DCD_DeactivatePDPcontext();
}

/*****************************************************************************/
// 	Description : save url in latest play info
//	Global resource dependence : none
//  Author: 有问题
//	Note:
/*****************************************************************************/
LOCAL void DcdHandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    //SCI_TRACE_LOW:"[MMIDCD]:DcdHandlePDPMsg enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1465_112_2_18_2_12_52_212,(uint8*)"");
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
		//added by jun.hu for cr180280, DCD更新1小时死机的问题....
		if(IsDCDInIdleState())
		{
			if(MMIPDP_RESULT_SUCC == msg_ptr->result)
			{
				DCD_HttpSetCurNetId((uint32)msg_ptr->nsapi);
				//SCI_TRACE_LOW:"[MMIDCD]:dcd  pdp active success!!!!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1475_112_2_18_2_12_52_213,(uint8*)"");
				DCD_Sendsig(DCD_SIG_PDP_ACTIVED_CNF,HTTP_REQ_TYPE_NULL);
			}
			else
			{
				//SCI_TRACE_LOW:"[MMIDCD]:dcd pdp active fail!!!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1480_112_2_18_2_12_52_214,(uint8*)"");
				DCD_Sendsig(DCD_SIG_ERROR_IND,DCD_ERROR_INVALID_NETWORK);
			}
		}
		else
		{
			//SCI_TRACE_LOW:"[MMIDCD]:dcd  is busy , but accapt MMIPDP_ACTIVE_CNF message!!status = %d "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1486_112_2_18_2_12_52_215,(uint8*)"d", s_dcd_status);
		}
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        {
            //SCI_TRACE_LOW:"[MMIDCD]:dcd  pdp deactive is come!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1492_112_2_18_2_12_52_216,(uint8*)"");
            //DCD_Sendsig(DCD_SIG_PDP_DEACTIVED_CNF,HTTP_REQ_TYPE_NULL);		   
        }		   
        break;
        
    case MMIPDP_DEACTIVE_IND:
        {        
            //SCI_TRACE_LOW:"[MMIDCD]:dcd  MMIPDP_DEACTIVE_IND msg is come!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1499_112_2_18_2_12_52_217,(uint8*)"");
            DCD_Sendsig(DCD_SIG_ERROR_IND,DCD_ERROR_INVALID_NETWORK);
        }
        break;
        
    default:        
        break;
    }
    //SCI_TRACE_LOW:"[MMIDCD]:DcdHandlePDPMsg exit!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1507_112_2_18_2_12_52_218,(uint8*)"");
}
/*****************************************************************************/
// Description : set xmldoc 
// Global resource dependence : None
// Author:  zhuo.wang
// Note: 
/*****************************************************************************/
PUBLIC void *MMIDCD_AllocMem(
                             uint32         mem_len,
                             const char     *file_ptr, 
                             uint32         line
                             )
{
    return SCI_MallocApp (mem_len,file_ptr,line);
}


/*****************************************************************************/
// Description : set xmldoc 
// Global resource dependence : None
// Author: zhuo.wang
// Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_FreeMem(void *mem_ptr)
{
    SCI_FREE(mem_ptr);	   
}

//在收到sim相关信号时<调用>!!!
/*****************************************************************************/
// 	Description : 是否存在网络
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_NetWorkIsValid(void)
{
    // 卡状态的检查
    if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MMIAPIDCD_GeSettingInfo()->sim_slected))
    {
        //SCI_TRACE_LOW:"[MMIDCD]: MMIDCD_NetWorkIsValid sim is not ok"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1542_112_2_18_2_12_52_219,(uint8*)"");
        return FALSE;
    }
    // 移动联通的检查
    if(!MMIDCD_IsMccNetwork())
    {
        //SCI_TRACE_LOW:"[MMIDCD]: MMIDCD_NetWorkIsValid net type is not ok"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1548_112_2_18_2_12_52_220,(uint8*)"");
        return FALSE;
    }
    // 网络状态的检查...
    if(!MMIAPIPHONE_IsCSAvail(MMIAPIDCD_GeSettingInfo()->sim_slected))
    {
        //SCI_TRACE_LOW:"[MMIDCD]:  MMIDCD_NetWorkIsValid net is not valid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1554_112_2_18_2_12_52_221,(uint8*)"");
        
        return FALSE;
    }

    return TRUE;

}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: z.f.for dcd_sim
//	Note:
/*****************************************************************************/
LOCAL void MMIDCD_SigSend(uint16 sig_code,uint16 para)
{
    MmiAppCopyMsgS   *sendSignal = PNULL;
    MmiCreateSignal(sig_code,sizeof(MmiAppCopyMsgS), (MmiSignalS**)&sendSignal);
    sendSignal ->data =para;
    MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
}

/*****************************************************************************/
//  Description :get current mcc and mnc
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
//PUBLIC void MMIDCD_SetCurMCC(uint32 mcc , uint32 mnc)
//{
//    SCI_TRACE_LOW("[MMIDCD]:Current net mcc is: %d; mnc is: %d ", mcc, mnc);
////    s_mcc = mcc;
//    s_mnc = mnc;		
//}


/*****************************************************************************/
//  Description :is mcc net or not
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsMccNetwork(void)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MN_PLMN_T select_plmn = {0};

    dual_sys = (MN_DUAL_SYS_E)(MMIAPIDCD_GeSettingInfo()->sim_slected);
    MMIAPIPHONE_GetCurPLMN(&select_plmn, dual_sys);
    //SCI_TRACE_LOW:"[MMIDCD]:Current net mnc is: %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1598_112_2_18_2_12_52_222,(uint8*)"d", select_plmn.mnc);
     return (select_plmn.mnc == CHINA_MNC_CODE);	
}

/*****************************************************************************/
//  Description :get current mcc
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
LOCAL void DCD_SigDispatchReg(DCD_SigDispatch SigDispatch)
{
    if(PNULL != SigDispatch)
    {
        MMIDCD_SetSigDispatchFun(SigDispatch);
    }
}

/****************************************************************************/
//   Description : DCD task init handler
//   Global resource dependence : 
//   Author:Zhuo.Wang
//   Note: first set some param, parser default xml file in fs
/****************************************************************************/
PUBLIC void DCD_InitStart(void)
{
    MMIDCD_XMLDataFree(FALSE);
    DCD_SigDispatchReg(MMIDCD_SigSend);
    
    MMIDCD_SetErrorRetryFlag(FALSE);
    MMIDCD_SetWapPushUpdate(FALSE);
    MMIDCD_SetDefaultRetryTime();
    s_dcd_is_updating = FALSE;

    MMIDCDFILE_SetUpDateNewItemsCount(0);// jun.hu 11/2 初始化更新中获得的新条目数...
    
    //DCD_UpdateUI(TRUE,FALSE,FALSE); 
    ParseDCDXMLFileWhenPowerOn(FALSE);
    MMIDCD_SetData();
    
    DCD_Sendsig(DCD_SIG_INIT_REQ,HTTP_REQ_TYPE_NULL);   
    
}
/*****************************************************************************/
//  Description :  whether or not to invoke synchronization
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsWapPushUpdate(void)
{
    return s_svr_update;	    
}

/*****************************************************************************/
//  Description :  whether or not to invoke synchronization
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_SetWapPushUpdate(BOOLEAN is_wappush)
{
    s_svr_update= is_wappush;	    
}

/*****************************************************************************/
//  Description :  whether or not to invoke synchronization 
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsSynchronize(DCD_SYNC_PKG_T *sync_pkg_ptr)
{
	DCD_GLOBAL_META_T     *gobal_meta_ptr = PNULL;
	BOOLEAN result=FALSE;		
	
    if (PNULL == sync_pkg_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:PNULL != sync_pkg_ptr!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1666_112_2_18_2_12_52_223,(uint8*)"");
        return result;
    }
	gobal_meta_ptr = &(sync_pkg_ptr->meta.global_meta);
	
	//SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_IsSynchronize: at present dcd ttl=%d\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1671_112_2_18_2_12_52_224,(uint8*)"d",s_ttl_second);
	
	if(gobal_meta_ptr->global_meta_ttl == 0) //zero means disable dcd ttl update!!
	{
		//SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_IsSynchronize:dcd disable dcd ttl update!!\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1675_112_2_18_2_12_52_225,(uint8*)"");
		s_ttl_second=0;	   
		result = FALSE;
	}
	else if(s_ttl_second <= 60)
	{
		s_ttl_second=gobal_meta_ptr->global_meta_ttl;
		//SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_IsSynchronize:ttl update is coming, resume next ttl: %d\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1682_112_2_18_2_12_52_226,(uint8*)"d",s_ttl_second);
		result = TRUE;            	   
	}    
	else
	{
		s_ttl_second -= 60;
	}
	return result;
}

/*****************************************************************************/
//  Description :  whether or not to invoke synchronization 
//  Global resource dependence : the entry exist
//  Author:
//  Note: is_error_retry: is or not error retry
//              s_retry_minutes:  error retry time stamp
//             s_update_times:   error retry  update times
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsRetryUpdate(void)
{
    BOOLEAN result=FALSE;
    if(s_is_error_retry)
    {
        if(s_retry_minutes == 1)// 间隔为5分钟
        {
            s_update_times++; 
            if(s_update_times < 4)
            {	  
                s_retry_minutes=5+5*s_update_times;
            }
            else
            {
                s_retry_minutes=120;
            }				
            //SCI_TRACE_LOW:"[MMIDCD]:DCD error retry update is the %d times, next timer is %d min !!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1714_112_2_18_2_12_52_227,(uint8*)"dd",s_update_times, s_retry_minutes );
            result = TRUE;
        }
        else
        {
            s_retry_minutes--;     
            result = FALSE;
        }
    }
    
    return result;
    
}

/*****************************************************************************/
//  Description :  whether dcd is in national roming!
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsInRoaming(void)
{
	
	BOOLEAN result=FALSE;	
	MMIPHONE_ROAMING_STATUS_E status=MMIPHONE_ROAMING_STATUS_NO_ROMING;	
	status=MMIAPIPHONE_GetRoamingStatus(MMIAPIDCD_GeSettingInfo()->sim_slected);
	if(MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING == status)
	{
		result =TRUE;		
	}
	//SCI_TRACE_LOW:"[MMIDCD]: phone is in roaming status: %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1742_112_2_18_2_12_52_228,(uint8*)"d", status);
	return result;			 
	
}

/*****************************************************************************/
//  Description : Get dcd net setting homepage
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
PUBLIC char* MMIAPIDCD_MAIN_GetNetHomePage(void)
{
    char* homepage_ptr = PNULL;
    uint8 setting_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    
    setting_index = MMIDCD_Setting_GetNetSettingIndex(MMIAPIDCD_GeSettingInfo()->sim_slected);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIAPIDCD_GeSettingInfo()->sim_slected,setting_index);
    if(PNULL != setting_item_ptr)
    {
        homepage_ptr = (char* )setting_item_ptr->homepage;
    }

    return homepage_ptr;
}
/*****************************************************************************/
//  Description : PS/CS Notify Event Callback function  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void MMIDCD_NotifyEventCallback(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMIDCD]MMIDCD_NotifyEventCallback dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1772_112_2_18_2_12_53_229,(uint8*)"dd", dual_sys, notify_event);

    switch(notify_event)
    {
    case MMIPHONE_NOTIFY_EVENT_PS_AVAIL:
        if ((s_latest_req_type == HTTP_REQ_TYPE_START) || (s_latest_req_type == HTTP_REQ_TYPE_TTL)
            || (HTTP_REQ_TYPE_SVR == s_latest_req_type) || (HTTP_REQ_TYPE_RETRY == s_latest_req_type))
        {
            if ((MMIDCD_GetLatestUpdateState()==DCD_ERROR_INVALID_NETWORK)&& MMIDCD_IsIdle())
            {
                //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_NotifyEventCallback,  start retry update now! "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1782_112_2_18_2_12_53_230,(uint8*)"");
                MMIDCD_UpdateStart(HTTP_REQ_TYPE_RETRY);
            }
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description :Initialize Notify Event
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_InitNotifyEvent(void)
{
    MMIPHONE_NOTIFY_INFO_T notify_info = {0};

    notify_info.module_id = MMI_MODULE_DCD;
    notify_info.notify_func = MMIDCD_NotifyEventCallback;
    MMIAPIPHONE_RegisterNotifyInfo(&notify_info);

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:retry timer到时后的处理函数
/*****************************************************************************/
void MMIDCD_RetryCallBack(uint32 state)
{
    //SCI_TRACE_LOW:"[MMIDCD]:DCD Retry start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1813_112_2_18_2_12_53_231,(uint8*)"");
    MMIDCD_UpdateStart(HTTP_REQ_TYPE_RETRY);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:TTL timer到时后的处理函数
/*****************************************************************************/
void MMIDCD_TTLUpdateCallBack(uint32 state)
{
    //SCI_TRACE_LOW:"[MMIDCD]:DCD TTL start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1823_112_2_18_2_12_53_232,(uint8*)"");
    MMIDCD_UpdateStart(HTTP_REQ_TYPE_TTL);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 启动TTL timer和retry timer
/*****************************************************************************/
PUBLIC void MMIDCD_StartDCDTimer(MMIDCD_REQ_TYPE_E req_type, uint32 second)
{
    if(0 != second)
    {
        //SCI_TRACE_LOW:"[MMIDCD] start timer, req_type = %d, second = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1835_112_2_18_2_12_53_233,(uint8*)"dd", req_type, second);
	         
	 if(HTTP_REQ_TYPE_TTL == req_type)
	 {
	     //SCI_TRACE_LOW:"[MMIDCD] start ttl timer begin, timer_id = %d"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1839_112_2_18_2_12_53_234,(uint8*)"d", s_ttl_timer);
	     if(0 == s_ttl_timer)
            {
                s_ttl_timer = SCI_CreateTimer("ttlupdate_timer", (TIMER_FUN)MMIDCD_TTLUpdateCallBack,  0,
                                                                   second * 1000, SCI_NO_ACTIVATE);
                SCI_ActiveTimer(s_ttl_timer);
            }
            else
            {
                if (SCI_IsTimerActive(s_ttl_timer))
                {
                    SCI_DeactiveTimer(s_ttl_timer);
                }
                SCI_ChangeTimer(s_ttl_timer, (TIMER_FUN)MMIDCD_TTLUpdateCallBack, second * 1000);
                SCI_ActiveTimer(s_ttl_timer);
            }
	     //SCI_TRACE_LOW:"[MMIDCD] start ttl timer end, timer_id = %d"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1855_112_2_18_2_12_53_235,(uint8*)"d", s_ttl_timer);
	 }
	 else if(HTTP_REQ_TYPE_RETRY == req_type)
        {
            //SCI_TRACE_LOW:"[MMIDCD] start retry timer begin, timer_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1859_112_2_18_2_12_53_236,(uint8*)"d", s_retry_timer);
            if(0 == s_retry_timer)
            {
	         s_retry_timer = SCI_CreateTimer("retryupdate_timer", (TIMER_FUN)MMIDCD_RetryCallBack,  0,
                                                                        second * 1000, SCI_NO_ACTIVATE);
                SCI_ActiveTimer(s_retry_timer);
            }
            else
            {
                if(s_retry_times < 3)
                {
                    s_retry_times++; 
                    s_retry_seconds =300+300*s_retry_times;
                }
                else
                {
                    s_retry_seconds = 120 * 60;
                }
                //SCI_TRACE_LOW:"[MMIDCD]:DCD error retry update is the %d times, next timer is %d second !!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1877_112_2_18_2_12_53_237,(uint8*)"dd",s_retry_times, s_retry_seconds);
		  SCI_ChangeTimer(s_retry_timer, (TIMER_FUN)MMIDCD_RetryCallBack, s_retry_seconds * 1000);
                SCI_ActiveTimer(s_retry_timer);
            }
	     //SCI_TRACE_LOW:"[MMIDCD] start retry timer end, timer_id = %d"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1881_112_2_18_2_12_53_238,(uint8*)"d", s_retry_timer);
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 恢复retry timer及相关信息
/*****************************************************************************/
PUBLIC void MMIDCD_SetDefaultRetryTime(void)
{
    s_retry_times = 0;
    s_retry_seconds = 300;
    //SCI_TRACE_LOW:"[MMIDCD] MMIDCD_SetDefaultRetryTime enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1894_112_2_18_2_12_53_239,(uint8*)"");
    if (NULL != s_retry_timer)
    {
        //SCI_TRACE_LOW:"[MMIDCD] MMIDCD_SetDefaultRetryTime, delete s_retry_timer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_MAIN_1897_112_2_18_2_12_53_240,(uint8*)"");
        SCI_DeleteTimer(s_retry_timer);
        s_retry_timer = NULL;
    }
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_ENABLE_DCD*/    


