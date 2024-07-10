
/***************************************************************************
** File Name:      mmidcdmain.h                                                *
** Author:                                                                 *
** Date:           20/09/2007                                              *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 09/2007        violent.wei      Create
****************************************************************************/
#ifndef  _MMIDCD_MAIN_H_    
#define  _MMIDCD_MAIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"


#ifdef MMI_ENABLE_DCD
#include "mmidcd_xml_parse.h"
#include "mmidcd_http.h"
#include "mmidcd_file.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


#define DCD_DEBUG_ENABLE
#define MODULE_DCD     0x21

#ifdef DCD_DEBUG_ENABLE
#define DCD_TRACE_LOW( _format_string )   SCI_TRACE_LOW _format_string
#else
#define DCD_TRACE_LOW( _format_string )  
#endif

#define MMIDCD_ALLOC(size)   MMIDCD_AllocMem(size, __FILE__, __LINE__)
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/               
#define MMIDCD_MAX_PAGE_NUM                     15//最多15个频道
#define MMIDCD_MAX_ITEM_NUM_PER_PAGE            15//10//每个频道最多10条新闻

/*GUI_888_color_to_565_color*/
#define MMIDCD_NO_READ_COLOR    0XF4A3
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum _DCD_STATUS_E
{
    DCD_NULL_STATUS=0,
    DCD_IDLE_STATUS=1,
    DCD_XML_STATUS=2,
    DCD_MIME_STATUS,
    DCD_MAX_STATUS
}DCD_STATE_E;

typedef struct
{
    BOOLEAN is_deleted;
    BOOLEAN is_readed;
}MMIDCD_ITEM_FLAG;
/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DECLARE                          *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCD_RegMenuGroup(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:violent.wei
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_RegNv(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_InitModule(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:zhuo.wang
//	Note:
/*****************************************************************************/
PUBLIC void *MMIDCD_AllocMem(
                             uint32         mem_len,
                             const char     *file_ptr, 
                             uint32         line
                             );
/*****************************************************************************/
// Description : set xmldoc 
// Global resource dependence : None
//  Author:zhuo.wang
// Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_FreeMem(  void    *mem_ptr);

/*****************************************************************************/
// 	Description : Deactivate PDP context
//	Global resource dependence : none
//  Author: zhuo.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCD_DeactivatePDPcontext(void);
/*****************************************************************************/
//  Description :get current mcc
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
//PUBLIC void MMIDCD_SetCurMCC(uint32 mcc, uint32 mnc);
/*****************************************************************************/
//  Description :is mcc net or not
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsMccNetwork(void);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC void DCD_InitStart(void);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC uint32  MMIDCD_GetNextUpdateTimeSpace(void);//added by @jun.hu
/****************************************************************************/
//  Description: 设置更新失败的原因代码....
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/11/11
/****************************************************************************/
PUBLIC void  MMIDCD_SetLatestUpdateState(MMIDCD_ERROR_CODE_E error_code);
/****************************************************************************/
//  Description: 返回DCD上次更新状态的失败码
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/13
/****************************************************************************/
PUBLIC MMIDCD_ERROR_CODE_E  MMIDCD_GetLatestUpdateState(void);
/****************************************************************************/
//  Description: 设置信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC void  MMIDCD_SetSigDispatchFun(DCD_SigDispatch dcd_sig);
/****************************************************************************/
//  Description: 获取信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC DCD_SigDispatch  MMIDCD_GetSigDispatchFun(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description: 设置XmlDoc
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC void  MMIDCD_SetXmlDoc_Ptr(xmlDocPtr xmldoc_ptr);
/*****************************************************************************/
//  Description :MMIDCD_IsIdle
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDCD_IsIdle(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: z.f.for dcd_sim
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_NetWorkIsValid(void);
/*****************************************************************************/
//  Description :  whether or not to invoke synchronization
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsWapPushUpdate(void);

/*****************************************************************************/
//  Description :  whether or not to invoke synchronization 
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsSynchronize(DCD_SYNC_PKG_T *sync_pkg_ptr);

/*****************************************************************************/
//  Description :  whether or not to invoke synchronization 
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsRetryUpdate(void);

/*****************************************************************************/
//  Description :  dcd send sig to mmi
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_Sendsig(uint16 signal_code,uint16 para);

/*****************************************************************************/
//  Description :  whether dcd is in national roming!
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsInRoaming(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:byte.guo
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_RestoreFactorySetting(void);
/*****************************************************************************/
// Description : DCD sim error indication
// Global resource dependence : 
// Author:Zhuo Wang
// Note: 
/*****************************************************************************/
PUBLIC void  MMIDCD_SimErrorInd(void);
/*****************************************************************************/
// 	Description : 检查是否能进行DCD的更新动作,返回检查的结果....
//	Global resource dependence : none
//  Author: jun.hu
//	Note: 2009/11/12
/*****************************************************************************/
PUBLIC MMIDCD_ERROR_CODE_E MMIDCD_GetCheckForUpdateResult(void);
/*****************************************************************************/
// 	Description : 设置启动重试机制的标记....
//	Global resource dependence : none
//  Author: jun.hu
//	Note: 2009/11/12
/*****************************************************************************/
PUBLIC void MMIDCD_SetErrorRetryFlag(BOOLEAN retry_flag);
/*****************************************************************************/
//  Description :  dcd error retry update default
//  Global resource dependence : 
//  Author:
//  Note:  disable error update
/*****************************************************************************/
PUBLIC void MMIDCD_ErrorRetrySetDefault(void);
/*****************************************************************************/
//  Description :  whether or not to invoke synchronization
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_SetWapPushUpdate(BOOLEAN is_wappush);
/*****************************************************************************/
// 	Description :...
//	Global resource dependence : none
//  Author: jun.hu
//	Note: 2009/11/12
/*****************************************************************************/
PUBLIC void MMIDCD_ShowDefaultXml(void);
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
                                        );
/*****************************************************************************/
//  Description :Initialize Notify Event
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_InitNotifyEvent(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 启动TTL timer和retry timer
/*****************************************************************************/
PUBLIC void MMIDCD_StartDCDTimer(MMIDCD_REQ_TYPE_E req_type, uint32 second);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 恢复retry timer及相关信息
/*****************************************************************************/
PUBLIC void MMIDCD_SetDefaultRetryTime(void);
/****************************************************************************/
//  Description: 设置更新的类型...
//  Global resource dependence
//  Auther: jun.hu
//  Note: 
/****************************************************************************/
PUBLIC void  MMIDCD_SetLatestReqType(MMIDCD_REQ_TYPE_E latest_req_type);
/****************************************************************************/
//  Description: 获取当前正在处理的类型...
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/13
/****************************************************************************/
PUBLIC MMIDCD_REQ_TYPE_E  MMIDCD_GetCurProcessReqType(void);
#ifdef __cplusplus
    }
#endif

#endif /*#ifdef MMI_ENABLE_DCD*/   
#endif /*#ifdef _MMIDCD_MAIN_H_*/ 
