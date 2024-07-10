/******************************************************************************
                                   ����WCDMA  ����ҵ��ģ��
        ����3G  ������𽥳��죬������ҵ�������Ϊ������
        �ƶ��ն˵����㣬�𽥳�Ϊ���ɻ�ȱ��һ���֡���
        ģ����ƻ���OMA  ��������Ļ���syncml Э������ݺ���
        �˹���ʵ���ڶ�ƽ̨�������ϵͳ�е�PIMҵ��Ŀ��
        ��Ҫ�ﵽ����ƽ̨�Ŀ���ֲ�����õĹ����ȶ���ģ�顣
        ��ģ���ʵ�ַ�Ϊ��������:
        1.ʵ��Data sync����
        2.ʵ��Device Management����
        3.ʵ�ֶ�ƽ̨��ƽ����ֲ
        ģ�鿪���ο�OMA��Ա��ͬ������SRT_c��
        Ŀǰģ��ƻ�����֧��OMA syncML Э��1.1


        ��Ȩ����:�������Ȩ���ں���ͨ�ţ�����һ�з���Ȩ��
        ������Ա:����
        ��������:2005��4��11�ա�
******************************************************************************/


/******************************************************************************
�ļ�����:PIMSyncmlComm.c
�ļ�����:ͨѶ�ϲ�ӿ�ģ���ļ�
�ļ�˵��:��ҪΪsyncml task�ṩ��Ӧ��ͨѶ�ӿڣ��ṩͳһ��
                       �ӿڷ�װ��ʹ�����ϲ㿴��ͨѶ��һ���ģ�����
                       �ӵײ���������ΪHTTP/WSP/OBEX�����֣�����OBEX��
                       ��ΪIrda/Bluetooth/USB�����֡���Ӧ��ͬ��ƽ̨����
                       FEATURE�����ƣ�ʵ�ֲ�ͬƽ̨�Ĳ�����
������Ա:����
�״ο�������:2005-9-7
******************************************************************************/


/******************************************************************************
                                                            �޸ļ�¼
  ����                   ����                �޸�ԭ��
  -----------------------------------------------------------------------------
  2004-9-7              ����                ��һ�ο������롣
  2005-11-14          ����                ����ͨѶ������
  2006-6-8              ����                ȫ����д���룬ȥ�������Ļ��ơ�
  -----------------------------------------------------------------------------
  *****************************************************************************/
  /*  ����ͷ�ļ�*/
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

  /*  ���ݽṹ����*/
  
  /*  ȫ�ֱ�������*/

uint32 hs_resend_count = 0; // chenxiang_20080326
 
syncml_Comm_type* hs_global_Comm_Ptr=NULL;

extern task_relay_info_type*     hs_task_relay_info;
extern http_globle_indicator_type* hs_http_globle;
  /*  ���ܺ�������*/

  /*  ���ܺ���*/

  /******************************************************************* 
  ** ������:hs_syncml_Comm_Init
  ** ������������ʼ��ͨѶ��
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-11-14
  ** �汾: ver 1.0
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

       //PDP�����
	return hs_global_Comm_Ptr->prepare();/*lint !e746 */
  }

  /******************************************************************* 
  ** ������:hs_syncml_Comm_Destory
  ** ����������ͨѶ�ͷš�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-11-14
  ** �汾: ver 1.0
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
  ** ������:hs_syncml_Comm_SendData
  ** �����������������ݽӿڡ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
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
  ** ������:hs_syncml_Comm_ReSendData
  ** �������������·�������
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:20080326
  ** �汾: ver 1.0
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
  ** ������:hs_syncml_Comm_SendInterface
  ** �����������������ݽӿڡ��ڲ��ֻ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_SendInterface()
  {
      hs_resend_count = 0; // chenxiang_20080326
  	//syncml_comm_message:"MMIPIM  *_*use the bearer interface to send another times"
  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_208_112_2_18_3_25_56_237,(uint8*)"");
  	return hs_global_Comm_Ptr->bearer_SendData();/*lint !e746 */
  }

  /******************************************************************* 
  ** ������:hs_syncml_Comm_RecData
  ** �����������������ݽӿڡ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
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
  ** ������:hs_syncml_Comm_RecInterface
  ** �����������������ݽӿڡ��ڲ��ֻ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_RecInterface()
  {
  	//syncml_comm_message:"MMIPIM  *_*use the bearer interface to receive another times"
  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLCOMM_245_112_2_18_3_25_56_241,(uint8*)"");
  	return hs_global_Comm_Ptr->bearer_ReceiveData();/*lint !e746 */
  }
  
  /******************************************************************* 
  ** ������:hs_syncml_Set_Supported_Comm
  ** ��������������ȫ�ֱ�������ֵָ�뺯����
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
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
