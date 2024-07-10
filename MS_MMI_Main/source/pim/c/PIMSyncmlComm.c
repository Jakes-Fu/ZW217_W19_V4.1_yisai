/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMSyncmlComm.c
文件描述:通讯上层接口模块文件
文件说明:主要为syncml task提供相应的通讯接口，提供统一的
                       接口封装，使其在上层看来通讯是一样的，但是
                       从底层来看，分为HTTP/WSP/OBEX三部分，其中OBEX又
                       分为Irda/Bluetooth/USB三部分。对应不同的平台，有
                       FEATURE来控制，实现不同平台的操作。
开发人员:刘刚
首次开发日期:2005-9-7
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-9-7              刘刚                第一次开发编码。
  2005-11-14          刘刚                整合通讯方法。
  2006-6-8              刘刚                全部重写代码，去除繁复的机制。
  -----------------------------------------------------------------------------
  *****************************************************************************/
  /*  包含头文件*/
#include "mmi_pim_trc.h"
#ifdef WIN32
  #include "std_header.h"
#endif
  #define _PIMSYNCMLCOMM_C_
  
  #include "../h/PIMSyncmlComm.h"
  #include "../h/PIMCommhttp.h"
  #include "../h/PIMCommwsp.h"
  #include "../h/PIMCommobex.h"
  #include "../h/PIMsmlerr.h"
  #include "../h/PIMdatasynctask.h"

   #include"../h/mmipim.h"

  /*  数据结构声明*/
  
  /*  全局变量声明*/

uint32 hs_resend_count = 0; // chenxiang_20080326
 
syncml_Comm_type* hs_global_Comm_Ptr=NULL;

extern task_relay_info_type*     hs_task_relay_info;
extern http_globle_indicator_type* hs_http_globle;
  /*  功能函数声明*/

  /*  功能函数*/

  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_Init
  ** 功能描述：初始化通讯。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_Init()
  {
  	hs_global_Comm_Ptr=(syncml_Comm_type*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(syncml_Comm_type));
       if(NULL==hs_global_Comm_Ptr)
       {
             return HS_SML_ERR_NOT_ENOUGH_SPACE;
       }
	hs_smlLibMemset(hs_global_Comm_Ptr,0,sizeof(syncml_Comm_type));
	if(HS_SML_ERR_OK!=hs_syncml_Set_Supported_Comm(HTTP_support))
	{
		hs_smlLibFree(hs_global_Comm_Ptr);
		hs_global_Comm_Ptr=NULL;
		return HS_SML_ERR_A_COMM_ERROR;
	}
       hs_global_Comm_Ptr->reSend_cache = NULL; // chenxiang_20080326
       hs_global_Comm_Ptr->reSend_cache_length = 0; // chenxiang_20080326
	hs_global_Comm_Ptr->codetype=(XmlCodeType)hs_task_relay_info->encodetype;

       //PDP激活处理
	return hs_global_Comm_Ptr->prepare();/*lint !e746 */
  }

  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_Destory
  ** 功能描述：通讯释放。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_Destory()
  {
  	if(NULL!=hs_global_Comm_Ptr)
  	{
            // chenxiang_20090326
            if (NULL != hs_global_Comm_Ptr->reSend_cache)
            {
                hs_smlLibFree(hs_global_Comm_Ptr->reSend_cache);
                hs_global_Comm_Ptr->reSend_cache_length = 0;
                hs_global_Comm_Ptr->reSend_cache = NULL;
            }
  		hs_global_Comm_Ptr->release();/*lint !e746 */
		hs_smlLibFree(hs_global_Comm_Ptr);
		hs_global_Comm_Ptr=NULL;
  	}
	return HS_SML_ERR_OK;
  }
  
  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_SendData
  ** 功能描述：发送数据接口。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_SendData(short id)
  {
      hs_resend_count = 0; // chenxiang_20080326
       /* Lock the workspace for reading the assembled SyncML document */
	//syncml_comm_message:"MMIPIM  *_*hs_smlLockReadBuffer"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_138_112_2_18_3_25_55_233,(uint8*)"");
       //hs_smlLockReadBuffer(id, &(unsigned char*)(hs_global_Comm_Ptr->cache), &(long)(hs_global_Comm_Ptr->cache_length));
       hs_smlLockReadBuffer(id, (unsigned char*)(&(hs_global_Comm_Ptr->cache)), (long *)(&(hs_global_Comm_Ptr->cache_length)));/*lint !e64*/
    // chenxiang_20080326
    if (NULL != hs_global_Comm_Ptr->reSend_cache)
    {
        hs_smlLibFree(hs_global_Comm_Ptr->reSend_cache);
        hs_global_Comm_Ptr->reSend_cache = NULL;
    }
    hs_global_Comm_Ptr->reSend_cache_length = hs_global_Comm_Ptr->cache_length;
    
    hs_global_Comm_Ptr->reSend_cache = hs_smlLibMalloc(__FILE__, __LINE__, hs_global_Comm_Ptr->reSend_cache_length);    
    if(NULL==hs_global_Comm_Ptr->reSend_cache)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(hs_global_Comm_Ptr->reSend_cache,0, hs_global_Comm_Ptr->reSend_cache_length);

    hs_smlLibMemcpy(hs_global_Comm_Ptr->reSend_cache, hs_global_Comm_Ptr->cache, hs_global_Comm_Ptr->reSend_cache_length);

	/*use the bearer  interface*/
	//syncml_comm_message:"MMIPIM  *_*use the bearer interface to send."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_159_112_2_18_3_25_55_234,(uint8*)"");
#ifdef HS_FEATURE_COMM_HTTP_SUPPORT
	hs_http_globle->socket_step=SOCKET_STEP_CONNECT;
#endif
       return hs_global_Comm_Ptr->bearer_SendData();/*lint !e746 */
  }




  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_ReSendData
  ** 功能描述：重新发送数据
  ** 输  出: 返回操作状态
  ** 作  者:陈翔
  ** 日  期:20080326
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_ReSendData(void) //chenxiang_20090326
  {
    hs_resend_count++; // chenxiang_20080326
    //syncml_comm_message:"MMIPIM  *_*  hs_syncml_Comm_ReSendData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_180_112_2_18_3_25_55_235,(uint8*)"");
    if (NULL != hs_global_Comm_Ptr->reSend_cache)
    {
        hs_global_Comm_Ptr->cache = hs_global_Comm_Ptr->reSend_cache;
        hs_global_Comm_Ptr->cache_length = hs_global_Comm_Ptr->reSend_cache_length;
	/*use the bearer  interface*/
	//syncml_comm_message:"MMIPIM  *_*use the bearer interface to send."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_186_112_2_18_3_25_56_236,(uint8*)"");
#ifdef HS_FEATURE_COMM_HTTP_SUPPORT
	hs_http_globle->socket_step=SOCKET_STEP_CONNECT;
#endif
       return hs_global_Comm_Ptr->bearer_SendData();/*lint !e746 */
     }
    return 0;
  }



  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_SendInterface
  ** 功能描述：发送数据接口。内部轮回
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_SendInterface()
  {
      hs_resend_count = 0; // chenxiang_20080326
  	//syncml_comm_message:"MMIPIM  *_*use the bearer interface to send another times"
  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_208_112_2_18_3_25_56_237,(uint8*)"");
  	return hs_global_Comm_Ptr->bearer_SendData();/*lint !e746 */
  }

  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_RecData
  ** 功能描述：接收数据接口。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_RecData(short id)
  {
  	//syncml_comm_message:"MMIPIM  *_*Begin receive data!"
  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_222_112_2_18_3_25_56_238,(uint8*)"");
      /* Lock the workspace for reading the assembled SyncML document */
	//syncml_comm_message:"MMIPIM  *_*hs_smlLockWriteBuffer"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_224_112_2_18_3_25_56_239,(uint8*)"");
       hs_smlLockWriteBuffer(id, (unsigned char*)(&(hs_global_Comm_Ptr->cache)), (long *)(&(hs_global_Comm_Ptr->cache_length)));/*lint !e64*/

   #ifdef HS_FEATURE_COMM_HTTP_SUPPORT
	hs_http_globle->socket_step=SOCKET_STEP_1;
   #endif
    /*use the bearer  interface*/
	//syncml_comm_message:"MMIPIM  *_*use the bearer interface to receive."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_231_112_2_18_3_25_56_240,(uint8*)"");
       return hs_global_Comm_Ptr->bearer_ReceiveData();/*lint !e746 */
  }

  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_RecInterface
  ** 功能描述：接收数据接口。内部轮回
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_RecInterface()
  {
  	//syncml_comm_message:"MMIPIM  *_*use the bearer interface to receive another times"
  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_245_112_2_18_3_25_56_241,(uint8*)"");
  	return hs_global_Comm_Ptr->bearer_ReceiveData();/*lint !e746 */
  }
  
  /******************************************************************* 
  ** 函数名:hs_syncml_Set_Supported_Comm
  ** 功能描述：设置全局变量，付值指针函数。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Set_Supported_Comm(uint8 type)
  {
  	switch(type)
  	{
  		#ifdef HS_FEATURE_COMM_HTTP_SUPPORT
  		case HTTP_support:
		{
			hs_global_Comm_Ptr->cache=NULL;
			hs_global_Comm_Ptr->cache_length=0;
			hs_global_Comm_Ptr->privateinfo=NULL;
			hs_global_Comm_Ptr->prepare=HS_HTTP_Prepare;
			hs_global_Comm_Ptr->release=HS_HTTP_Release;
			hs_global_Comm_Ptr->bearer_SendData=HS_HTTP_SendData;
			hs_global_Comm_Ptr->bearer_ReceiveData=HS_HTTP_RecData;
  		}break;
		#endif
		
		#ifdef HS_FEATURE_COMM_WSP_SUPPORT
		case WSP_support:
		{
			hs_global_Comm_Ptr->cache=NULL;
			hs_global_Comm_Ptr->cache_length=0;
			hs_global_Comm_Ptr->privateinfo=NULL;
			hs_global_Comm_Ptr->prepare=HS_WSP_Prepare;
			hs_global_Comm_Ptr->release=HS_WSP_Release;
			hs_global_Comm_Ptr->bearer_SendData=HS_WSP_SendData;
			hs_global_Comm_Ptr->bearer_ReceiveData=HS_WSP_RecData;
		}break;
		#endif

		#ifdef HS_FEATURE_COMM_OBEX_SUPPORT
		case OBEX_support:
		{
			hs_global_Comm_Ptr->cache=NULL;
			hs_global_Comm_Ptr->cache_length=0;
			hs_global_Comm_Ptr->privateinfo=NULL;
			hs_global_Comm_Ptr->prepare=HS_OBEX_Prepare;
			hs_global_Comm_Ptr->release=HS_OBEX_Release;
			hs_global_Comm_Ptr->bearer_SendData=HS_OBEX_SendData;
			hs_global_Comm_Ptr->bearer_ReceiveData=HS_OBEX_RecData;
		}break;
		#endif

		default:
		{
			hs_global_Comm_Ptr->cache=NULL;
			hs_global_Comm_Ptr->cache_length=0;
			hs_global_Comm_Ptr->privateinfo=NULL;
			hs_global_Comm_Ptr->prepare=NULL;
			hs_global_Comm_Ptr->release=NULL;
			hs_global_Comm_Ptr->bearer_SendData=NULL;
			hs_global_Comm_Ptr->bearer_ReceiveData=NULL;
			return HS_SML_ERR_WRONG_PARAM;
		}break;/*lint !e527*/
  	}
	return HS_SML_ERR_OK;
  }
