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
�ļ�����:PIMdatasynctask.c
�ļ�����:taskģ��
�ļ�˵��:���taskģ��Ĺ��ܣ���Ҫ�����syncml�������ܵ�
                       ָ�ӡ�
������Ա:����
�״ο�������:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            �޸ļ�¼
  ����                   ����                �޸�ԭ��
  -----------------------------------------------------------------------------
  2004-4-11              ����                ��һ�ο������롣
  -----------------------------------------------------------------------------
  *****************************************************************************/
#include "mmi_pim_trc.h"
#ifdef WIN32
  #include "std_header.h"
#endif
  #define _PIMDATASYNCTASK_C_

  /*  ����ͷ�ļ�*/
  #include "../h/PIMDataSyncTask.h"
  #include "../h/PIMlibmem.h"
  #include "../h/PIMSyncmlComm.h"
  #include "../h/PIMcallback_handler.h"

  #include"mmi_appmsg.h"
 #include "dal_time.h"
//#include "pimsmlerr.h"

/** �궨�� **/

 typedef  unsigned char     byte;   
 
  /*  ���ݽṹ����*/

  /*  ȫ�ֱ�������*/
  extern SmlNewMapItemListPtr_t  hs_globlemapItemList; // chenxiang_map 20080325

  extern BOOLEAN g_is_pim_sync;/*lint !e526*/ //�Ƿ�����ͬ��

uint8 hs_g_sessionID = 1; /*lint !e765*/ // chenxiang 20090206
  boolean hs_already_receive=FALSE;
  //static boolean hs_need_holdon=TRUE;

  /*task���д��������Ҫ����Ҫ����*/
  task_protocol_step_type* hs_protocol_step=NULL;
  task_protocol_step_type* hs_protocol_step_priv=NULL;

  /*task��Ҫ�õ�����Ҫ����������anchor,��Ȩ��Ϣ�ȵ�*/
  task_relay_info_type*     hs_task_relay_info=NULL;

  /*task��Ҫ�õ�����Ҫ��������Ӧ�������������ɵ�status����*/
  status_cmd_queue_type*  hs_status_cmd_que=NULL;


  
  
//static SmlEncoding_t  hs_codectype=SML_XML/*SML_WBXML*/;


  /*  ���ܺ�������*/
//extern BOOLEAN  MNNV_GetIMEI(   MN_IMEI_T imei  );/*lint !e526*/

/******************************************************************* 
** ������:pim_convert_julan2str
** ������������julan�����ݽṹת��Ϊstr.
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
//static void pim_convert_julan2str(JulianType julian , char* str);

/******************************************************************* 
** ������:hs_pim_int2str
** ������������unit16�����ݽṹת��Ϊָ�����ȵ�str.
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
static void hs_pim_int2str(uint32 source, char* str,int length);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/******************************************************************* 
** ������:pim_convert_time2str
** ������������ʱ���������ݽṹת��Ϊstr.
** ��  ��: null
** ��  ��:������
** ��  ��:2007-10-23
** �汾: ver 1.0
*******************************************************************/
void hs_pim_convert_scitime2str(SCI_TM_T current_time , char* str)/*lint !e765*/
{
   char string[17];
   int i;
   hs_pim_int2str(current_time.tm_year,string,4);
   hs_pim_int2str(current_time.tm_mon,&string[4],2);
   hs_pim_int2str(current_time.tm_mday,&string[6],2);
   hs_pim_int2str(current_time.tm_hour,&string[8],2);
   hs_pim_int2str(current_time.tm_min,&string[10],2);
   hs_pim_int2str(current_time.tm_sec,&string[12],2);
   hs_pim_int2str(current_time.tm_wday,&string[14],2);
   string[16]='\0';
   for(i=0;i<17;i++)
   {
      *(str+i)=string[i];
   }
}





/******************************************************************* 
** ������:hs_Data_Sync_terminate
** �����������ս�����ͬ�����ͷ�ͬ������������
**                            ���ڴ�ռ䡣
** ��  ��: null
** ��  ��:������
** ��  ��:2007-11-4
** �汾: ver 1.0
*******************************************************************/
  void hs_Data_Sync_terminate()
  {
     // pim_sync_result sync_result={0};
      PIM_T_P  pMe = MMIPIM_GetPIM();
       //syncml_task_message:"MMIPIM  ^_^ enter syncml hs_Data_Sync_terminate!\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_144_112_2_18_3_25_43_138,(uint8*)"");
       
        g_is_pim_sync=FALSE;//add by wangxiaolin 
       
       //�ͷ�ͨѶȫ�ֱ���
       hs_syncml_Comm_Destory();

      if (NULL != hs_globlemapItemList)
      {
#ifdef HS_PIM_CACHE_MAP
        hs_mySaveCacheMap(); //  wangxiaolin 20090804 ����map ��Ϣ
#endif
        hs_smlFreeNewMapItemList(hs_globlemapItemList);

        hs_globlemapItemList = NULL;
      }
       
       //�ͷ����̿��Ʊ���
       if(NULL!=hs_protocol_step)
       	{
       hs_smlLibFree((void *) hs_protocol_step);
       hs_protocol_step=NULL;
       	}
	if(NULL!=hs_protocol_step_priv)
      {
       hs_smlLibFree((void *) hs_protocol_step_priv);
       hs_protocol_step_priv=NULL;
	}
       //�ͷŹ��̿��Ʊ���
       if(NULL!=hs_task_relay_info)
       	{
        if(NULL!=hs_task_relay_info->last_anchor)
       {
         //syncml_task_message:"MMIPIM  ^_^ syncml hs_Data_Sync_terminate! free last_anchor\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_177_112_2_18_3_25_43_139,(uint8*)"");
     	  hs_smlLibFree(((void *) hs_task_relay_info->last_anchor));
        hs_task_relay_info->last_anchor=NULL;
       }
       if(NULL!=hs_task_relay_info->next_anchor)
       {
        //syncml_task_message:"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free next_anchor\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_183_112_2_18_3_25_43_140,(uint8*)"");
     	  hs_smlLibFree(((void *) hs_task_relay_info->next_anchor));
        hs_task_relay_info->next_anchor=NULL;
       }

       
       if(NULL!=hs_task_relay_info->proxy_ip)
       {
         //syncml_task_message:"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free proxy_ip\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_191_112_2_18_3_25_43_141,(uint8*)"");
     	  hs_smlLibFree(((void *) hs_task_relay_info->proxy_ip));
        hs_task_relay_info->proxy_ip=NULL;
       }
       if(NULL!=hs_task_relay_info->proxy_port)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->proxy_port));
        hs_task_relay_info->proxy_port=NULL;
       }
       if(NULL!=hs_task_relay_info->proxy_username)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->proxy_username));
        hs_task_relay_info->proxy_username=NULL;
       }
       if(NULL!=hs_task_relay_info->proxy_password)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->proxy_password));
        hs_task_relay_info->proxy_password=NULL;
       }

       
       if(NULL!=hs_task_relay_info->syncml_pim_server_name)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_server_name));
        hs_task_relay_info->syncml_pim_server_name=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_server_phonebook_db)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_server_phonebook_db));
        hs_task_relay_info->syncml_pim_server_phonebook_db=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_server_calendar_db)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_server_calendar_db));
        hs_task_relay_info->syncml_pim_server_calendar_db=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_server_port)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_server_port));
        hs_task_relay_info->syncml_pim_server_port=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_username)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_username));
        hs_task_relay_info->syncml_pim_username=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_password)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_password));
        hs_task_relay_info->syncml_pim_password=NULL;
       }

       
       if(NULL!=hs_task_relay_info->syncml_pim_client_imei)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_client_imei));
        hs_task_relay_info->syncml_pim_client_imei=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_client_phonebook_db)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_client_phonebook_db));
        hs_task_relay_info->syncml_pim_client_phonebook_db=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_client_calendar_db)
       {
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_client_calendar_db));
        hs_task_relay_info->syncml_pim_client_calendar_db=NULL;
       }
       if(NULL!=hs_task_relay_info->syncml_pim_nextnonce)
       {
       //syncml_task_message:"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free syncml_pim_nextnonce\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_261_112_2_18_3_25_43_142,(uint8*)"");
     	  hs_smlLibFree(((void *) hs_task_relay_info->syncml_pim_nextnonce));
        hs_task_relay_info->syncml_pim_nextnonce=NULL;
       }
	//syncml_task_message:"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free hs_task_relay_info\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_265_112_2_18_3_25_43_143,(uint8*)"");
  	hs_smlLibFree(((void *) hs_task_relay_info));
       hs_task_relay_info=NULL;
       	}
	if(NULL!=hs_status_cmd_que)
	{
	//�ͷ�status queue
	if(hs_status_cmd_que->totalnumber!=0)
	{
	      //syncml_task_message:"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free hs_status_cmd_que\n"
	      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_274_112_2_18_3_25_43_144,(uint8*)"");
	      hs_myFreestatusofCQ();
	}
	hs_smlLibFree((void*)hs_status_cmd_que);
      hs_status_cmd_que=NULL;
	}
      //syncml_task_message:"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free all instance\n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_280_112_2_18_3_25_43_145,(uint8*)"");
      if (NULL != hs_pGlobalAnchor) // chenxiang_20080317
      {
        //�ͷ����е�instance
        hs_syncml_TerminateAllExitInstance();
        //�ͷ�pGlobalAnchor
        hs_syncml_Terminate();
      }

     pMe->is_init=FALSE;
     //pMe->g_sync_result=sync_result;

    //add by wangxiaolin ,reset g_sync_result 2010.09
    hs_smlLibMemset(&(pMe->g_sync_result), 0, sizeof(pim_sync_result));
     
     //reset static variables
     hs_already_receive=FALSE;
     //hs_need_holdon=TRUE;
     //syncml_task_message:"MMIPIM  ^_^leave syncml hs_Data_Sync_terminate!\n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_298_112_2_18_3_25_43_146,(uint8*)"");
  }

/******************************************************************* 
** ������:hs_Data_Sync_task
** ����������taskģ������������������ͬ���Ĺ��̡�
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_Data_Sync_task()
{
    
    /*watch the step,then do as the rule*/
    switch(*hs_protocol_step)
    {
        /*˫��ͬ���пͻ��˳�ʼ��ͬ��*/
        case STEP_CLIENT_INIT_SYNC:
        {
            //syncml_task_message:"MMIPIM  ^_^ Begin client init!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_317_112_2_18_3_25_43_147,(uint8*)"");
            hs_step_client_init_sync_process();
        }
        break;
        
        /*˫��ͬ���з�������ʼ��ͬ��*/
        case STEP_SERVER_INIT_SYNC:
        {
            //syncml_task_message:"MMIPIM  ^_^ Begin Server init!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_325_112_2_18_3_25_43_148,(uint8*)"");
            hs_step_server_init_sync_process();
        }
        break;
        
        /*˫��ͬ���пͻ��˷�����Ҫͬ��������*/
        case STEP_CLIENT_MODI:
        {
            //syncml_task_message:"MMIPIM  ^_^ Begin client modify!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_333_112_2_18_3_25_43_149,(uint8*)"");
            hs_step_client_modi_process();
        }
        break;
        
        /*˫��ͬ���з������ӵ��ͻ��˵�������Ӧ��
        ������������Ҫͬ��������*/
        case STEP_SERVER_CONF_MODI:
        {
            //syncml_task_message:"MMIPIM  ^_^ Begin Server modify!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_342_112_2_18_3_25_43_150,(uint8*)"");
            hs_step_server_conf_modi_process();
        }
        break;
        
        /*˫��ͬ���пͻ��˽��յ���������������Ӧ
        �Ϳͻ��˷���map��Ϣ*/
        case STEP_CLIENT_CONF_MAP:
        {
            //syncml_task_message:"MMIPIM  ^_^ Begin client status and map send!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_351_112_2_18_3_25_43_151,(uint8*)"");
            hs_step_client_conf_map_process();
        }
        break;
        
        /*˫��ͬ���з���������mapȷ��*/
        case STEP_SERVER_MAPACK:
        {
            //syncml_task_message:"MMIPIM  ^_^ Begin server map ack!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_359_112_2_18_3_25_43_152,(uint8*)"");
            hs_step_server_mapack_process();
        }
        break;
        
        /*ͬ���������ͷ���Դ*/
        case STEP_SYNC_OVER:
        {
             char  nv_lastanchor[MMIPIM_ANCHOR_LEN]={0};

            //syncml_task_message:"MMIPIM  ^_^ All sync operation is over,then release the resources!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_369_112_2_18_3_25_43_153,(uint8*)"");
            //save the last anchor
            //syncml_task_message:"MMIPIM  ^_^ synchornization is ok! save the last anchor!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_371_112_2_18_3_25_43_154,(uint8*)"");

            hs_smlLibStrcpy(nv_lastanchor,hs_task_relay_info->next_anchor);
            MMIPIM_SaveLastAnchor(nv_lastanchor);
            //��ǰ�汾��Ϊ�ڹرմ���ʱ��ͳһ�ͷ���Դ,Ҳ���ǽ���ͬ�����ں�ֻ����ͬ��һ��
#if 0//del by wangxiaolin   2009.11.18  ����ڴ˴��˳���ͨ�������󣬽���ͨ������ʱ���ͬ���Ѿ�������������ˢ��������
            hs_Data_Sync_terminate();
#endif
        }
        break;
        
        /*��������û�д����꣬����alert222�����������������*/
        case STEP_SERVER_MORE:
        {
            //syncml_task_message:"MMIPIM  ^_^ more data,send alert 222 for more!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_385_112_2_18_3_25_43_155,(uint8*)"");
            hs_step_server_more();
        }
        break;
        
        /*��������(�����ܷ���)*/
        default:
        {
            //syncml_task_message:"MMIPIM  *_* STEP_SYNC_ERROR!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_393_112_2_18_3_25_43_156,(uint8*)"");
            /*�����˳�ͬ��ģ��*/
#if 0//del by wangxiaolin   2009.11.18
            hs_Data_Sync_terminate();
#endif
        }
        break;		
        
    }	

    /*Data sync UI set timer,give the control to brew.*/
    //UI_Set_timer();
    return;
}

/******************************************************************* 
** ������:hs_got_free_id
** ������������Ϊinstanceһ��ֻ��4��ʵ������˵�
**                        ȫ������ʱ����Ҫ�ͷŵ�ǰ��һ����Ȼ
**                        �������·����ڴ档�˺����ҵ���ǰ��
**                        ��һ��ʵ����Ȼ���ͷ�����ͬʱ�����
**                        id����Ϊ��ǰ��id��
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_got_free_id(void)
{
    hs_task_relay_info->workspaceid++;
    if(hs_task_relay_info->workspaceid==5)
    {
          hs_task_relay_info->workspaceid=1;
    }	
     //�ͷŵ���һ��workspace���Ա����ʹ�á�	
   // hs_syncml_TerminateInstance(hs_task_relay_info->workspaceid);
     hs_syncml_TerminateAllExitInstance(); // chenxiang_20080305 �ͷ����е�workspace
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_got_free_id workspaceid= %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_429_112_2_18_3_25_43_157,(uint8*)"d", hs_task_relay_info->workspaceid);
}

/******************************************************************* 
** ������:hs_step_client_init_sync_process
** ����������taskģ�麯����client����ͬ����
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_client_init_sync_process()
{
     
    char worksapce_name[]="Hisense DS Client init";
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    PIM_T_P  pMe = MMIPIM_GetPIM();
#endif /* HS_PIM_VCAL */

     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_client_init_sync_process,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_447_112_2_18_3_25_43_158,(uint8*)"d",PIM_GetCurTime());
    hs_got_free_id(); // chenxiang_20080305 ��ͬ��ͨ�Ŵ���
    
    hs_syncml_initance(&(hs_task_relay_info->workspaceid), worksapce_name);

    /*build sync header and syncbody first parm*/
    hs_syncml_startmessage(hs_task_relay_info->workspaceid);
#ifdef HS_PIM_VCAL  //wangxiaolin  2009.9.14  
    //syncml_task_message:"MMIPIM  ^_^ Begin step_client_init_sync_process1,sync_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_455_112_2_18_3_25_43_159,(uint8*)"d",pMe->m_sync_mode);
#else /* HS_PIM_VCAL */
    //syncml_task_message:"MMIPIM  ^_^ Begin step_client_init_sync_process1"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_457_112_2_18_3_25_43_160,(uint8*)"");
#endif /* HS_PIM_VCAL */

    /*Start status cmd for two way sync server syncheader*/
    if (0 < hs_status_cmd_que->totalnumber)
    {
        hs_myFreestatusofCQ();
        //hs_syncml_StatusCmd(hs_task_relay_info->workspaceid); // chenxiang_20080308 ��ͬ��ת��ͬ��ʱ, �ڶ��η��͵�client ҲӦ�÷���status , �������ܵ�client modi �׶�
    }
    if(SYNC_TYPE_S2WAY==hs_task_relay_info->synctype)
    {
          /*Start alert cmd for initialize slow two way sync*/
          hs_syncml_AlertCmd(hs_task_relay_info->workspaceid,"201");
          //syncml_task_message:"MMIPIM  ^_^ Begin step_client_init_sync_process2"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_470_112_2_18_3_25_43_161,(uint8*)"");

    #ifdef HS_PIM_CACHE_MAP
          hs_myClearCacheMap(); //  wangxiaolin 20090804 �����ʼ��ͬ��, ��մ洢��map ��Ϣ
    #endif
    }
    else if(SYNC_TYPE_2WAY==hs_task_relay_info->synctype)
    {
          /*Start alert cmd for initialize slow two way sync*/
          hs_syncml_AlertCmd(hs_task_relay_info->workspaceid,"200");
    }
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for ����ͬ��&  ˢ��ͬ��
 else if(SYNC_TYPE_1WAYFCLI==hs_task_relay_info->synctype)
    {
        hs_syncml_AlertCmd(hs_task_relay_info->workspaceid,"202");
    }
    else if(SYNC_TYPE_RFCLI==hs_task_relay_info->synctype)
    {
        hs_syncml_AlertCmd(hs_task_relay_info->workspaceid,"203");
    }
    else if(SYNC_TYPE_1WAYFSER==hs_task_relay_info->synctype)
    {
        hs_syncml_AlertCmd(hs_task_relay_info->workspaceid,"204");
    }
    else if(SYNC_TYPE_RFSER==hs_task_relay_info->synctype)
    {
        hs_syncml_AlertCmd(hs_task_relay_info->workspaceid,"205");
    }
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
    else
    {
        // TODO: ͬ�����ʹ����� /*-wangxiaolin 2009.7.9 -*/
    }
     //syncml_task_message:"MMIPIM  ^_^ Begin step_client_init_sync_process3"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_503_112_2_18_3_25_44_162,(uint8*)"");

    /*Start put cmd for send the client's capability*/
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    if(SYNC_MODE_PB == pMe->m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    hs_syncml_PutCmd(hs_task_relay_info->workspaceid);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    }
    else
    {
        hs_syncml_PutVCALCmd(hs_task_relay_info->workspaceid);
    }
#endif /* HS_PIM_VCAL */

    /*Start get cmd for require the server's capability*/
    hs_syncml_GetCmd(hs_task_relay_info->workspaceid);
 
    /* --- End message and the syncbody last parm--- */
    hs_syncml_EndMessage(hs_task_relay_info->workspaceid);

    hs_task_relay_info->messageID++;   //count messageid
    hs_task_relay_info->cmdID=1;   //reset cmdid

   //these are for syncml core test on windows. liugang make it disabled.2005-11-7.
   //make these changed to save logs on efs in debug model.  liugang 2006-5-29.
    /* --- Print the document which was generated --- */
   #ifdef FEATURE_PIM_DEBUG
     //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_532_112_2_18_3_25_44_163,(uint8*)"");
     hs_myPrintxmlorwbxml("client_init",hs_task_relay_info->workspaceid);
     //syncml_task_message:"MMIPIM  ^_^ xml logs write complete.\n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_534_112_2_18_3_25_44_164,(uint8*)"");
   #endif
 
     //syncml_task_message:"MMIPIM  ^_^ client init data ok,begin send!time=%d \n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_537_112_2_18_3_25_44_165,(uint8*)"d",PIM_GetCurTime());
    /*---Send the data which is ready*/
    hs_syncml_Comm_SendData(hs_task_relay_info->workspaceid);
      //syncml_task_message:"MMIPIM  ^_^ client init data send ok!time=%d \n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_540_112_2_18_3_25_44_166,(uint8*)"d",PIM_GetCurTime());

    *hs_protocol_step=STEP_SERVER_INIT_SYNC;
    *hs_protocol_step_priv=STEP_SERVER_MORE;
	
}

/******************************************************************* 
** ������:hs_step_server_init_sync_process
** ����������taskģ�麯����server��Ӧ��������ͬ����
** ��  ��: null
** ��  ��:������
** ��  ��:2007-11-19
** �汾: ver 1.0
*******************************************************************/
void hs_step_server_init_sync_process()
{

    /* --- get the document which was in a file and was considered as the server init document --- */
    if(FALSE == hs_already_receive)
    {
        char worksapce_name[]="Hisense DS Server init";

        //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_server_init_sync_process 1,time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_562_112_2_18_3_25_44_167,(uint8*)"d",PIM_GetCurTime());
        hs_got_free_id();

        hs_syncml_initance(&(hs_task_relay_info->workspaceid), worksapce_name);

        //syncml_task_message:"MMIPIM  ^_^ prepare ok,begin receive!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_567_112_2_18_3_25_44_168,(uint8*)"");
        hs_syncml_Comm_RecData(hs_task_relay_info->workspaceid);
        //syncml_task_message:"MMIPIM  ^_^ server init receive ok!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_569_112_2_18_3_25_44_169,(uint8*)"");

        hs_already_receive=TRUE;
        return;
    }
    else
    {
        //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_server_init_sync_process 2 ,time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_576_112_2_18_3_25_44_170,(uint8*)"d",PIM_GetCurTime());
#ifdef FEATURE_PIM_DEBUG
        //these are for syncml core test on windows. liugang make it disabled.2005-11-7.	  
        //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_579_112_2_18_3_25_44_171,(uint8*)"");
        hs_myPrintxmlorwbxml( "server_init", hs_task_relay_info->workspaceid);
        //syncml_task_message:"MMIPIM  ^_^ xml logs write complete.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_581_112_2_18_3_25_44_172,(uint8*)"");
#endif
        //syncml_task_message:"MMIPIM  ^_^ analysis recevie data.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_583_112_2_18_3_25_44_173,(uint8*)"");
        hs_syncml_ReceiveData(hs_task_relay_info->workspaceid);
        //syncml_task_message:"MMIPIM  ^_^ analysis server init complete.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_585_112_2_18_3_25_44_174,(uint8*)"");
#if 0
        //last anchor ��Ӧ���ϣ���ҪתΪ��ͬ����ʽ         
        if((hs_task_relay_info->synctype==SYNC_TYPE_S2WAY)&&(*hs_protocol_step==STEP_CLIENT_INIT_SYNC))
        {
            *hs_protocol_step_priv  = * hs_protocol_step;
        }
#else
        if(*hs_protocol_step == STEP_SYNC_ERROR)
        {
            *hs_protocol_step_priv  = * hs_protocol_step;
        }
#endif
        else
        {
            if(*hs_protocol_step == *hs_protocol_step_priv)
            {
                
#ifdef HS_PIM_CACHE_MAP // wangxiaolin 20090804
           if(SYNC_TYPE_2WAY==hs_task_relay_info->synctype)
           {
                if (hs_myLoadCacheMap())
                {
                    hs_mySetFlagOfSendingCacheMap(TRUE);
                }
                else
                {
                    hs_mySetFlagOfSendingCacheMap(FALSE);
                }
           }
            else
            {
                hs_mySetFlagOfSendingCacheMap(FALSE);
            }

#endif /* HS_PIM_CACHE_MAP */
                *hs_protocol_step = STEP_CLIENT_MODI;

            }
        }
             
        hs_already_receive = FALSE;
       // hs_need_holdon = TRUE;
        HS_Data_Sync_PostMsgToUI( PIM_MSG_NEXT_STEP,NULL, NULL);

        return;
    }
}


/******************************************************************* 
** ������:hs_step_client_modi_process
** ����������taskģ�麯����client��server������ͬ����
**                            ��ʼ˫��ͬ�����������⣬client��server
**                            ���͸��Ĺ������ݡ�
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_client_modi_process()
{
     
    char worksapce_name[]="Hisense DS Client modify";
    PIM_T_P  pMe=MMIPIM_GetPIM();
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_client_modi_process,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_649_112_2_18_3_25_44_175,(uint8*)"d",PIM_GetCurTime());
   //syncml_task_message:"MMIPIM  ^_^ enter hs_step_client_modi_process,synctype=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_650_112_2_18_3_25_44_176,(uint8*)"d",hs_task_relay_info->synctype);
    hs_got_free_id();
	
    hs_syncml_initance(&(hs_task_relay_info->workspaceid), worksapce_name);

    /*build sync header and syncbody first parm*/
    hs_syncml_startmessage(hs_task_relay_info->workspaceid);

    /*Start status cmd for two way sync server syncheader*/
    hs_syncml_StatusCmd(hs_task_relay_info->workspaceid);

    #ifdef HS_PIM_CACHE_MAP  //add by wangxiaolin 2009.08.14
    if (hs_myGetFlagOfSendingCacheMap())
    {
         //syncml_task_message:"MMIPIM  ^_^ prepare to send cache map....."
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_664_112_2_18_3_25_44_177,(uint8*)"");
         /*Send map cmd to the server*/
        hs_syncml_MapCmd(hs_task_relay_info->workspaceid);
    }
    #endif
    /*Start sync cmd for send the client's modify*/
    hs_syncml_StartSync(hs_task_relay_info->workspaceid);

    //syncml_task_message:"MMIPIM  ^_^ workspaceid=%d,authored=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_672_112_2_18_3_25_44_178,(uint8*)"dd",hs_task_relay_info->workspaceid,hs_task_relay_info->authored);
   #ifdef HS_PIM_CACHE_MAP  //add by wangxiaolin 2010.1.11 for cache map
    if (NULL != hs_globlemapItemList) 
    {  
        /* End the sync block */
        hs_syncml_EndSync(hs_task_relay_info->workspaceid);
        hs_syncml_EndMessageWithoutFinal(hs_task_relay_info->workspaceid);
    }
    else
    {
    #endif  /*HS_PIM_CACHE_MAP*/
        if(SYNC_TYPE_S2WAY==hs_task_relay_info->synctype)
        {
             //syncml_task_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_S2WAY\n"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_685_112_2_18_3_25_44_179,(uint8*)"");
         pMe->g_sync_result.delete_is_final=TRUE;
         pMe->g_sync_result.replace_is_final=TRUE;
     #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
         hs_syncml_set_large_object_proc_flag(FALSE);
     #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
       hs_syncml_fresh_for_slow_sync(hs_task_relay_info->workspaceid);
    }
    else if(SYNC_TYPE_2WAY==hs_task_relay_info->synctype)
    {
     #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
         hs_syncml_set_large_object_proc_flag(FALSE);
     #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
         hs_syncml_modify_for_fast_sync(hs_task_relay_info->workspaceid);
    }
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for ����ͬ��&  ˢ��ͬ��
     else if(SYNC_TYPE_1WAYFCLI==hs_task_relay_info->synctype)
    {
        //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFCLI \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_703_112_2_18_3_25_44_180,(uint8*)"");
     #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
         hs_syncml_set_large_object_proc_flag(FALSE);
     #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
         hs_syncml_modify_for_fast_sync(hs_task_relay_info->workspaceid);
    }
    else if(SYNC_TYPE_RFCLI==hs_task_relay_info->synctype)
    {
         //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFCLI\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_711_112_2_18_3_25_44_181,(uint8*)"");
         pMe->g_sync_result.delete_is_final=TRUE;
         pMe->g_sync_result.replace_is_final=TRUE;
      #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
         hs_syncml_set_large_object_proc_flag(FALSE);
     #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
         hs_syncml_fresh_for_slow_sync(hs_task_relay_info->workspaceid);
    }
     else if(SYNC_TYPE_1WAYFSER==hs_task_relay_info->synctype)
    {
         //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFSER\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_721_112_2_18_3_25_44_182,(uint8*)"");
         pMe->g_sync_result.delete_is_final=TRUE;
         pMe->g_sync_result.replace_is_final=TRUE;
         pMe->g_sync_result.add_is_final=TRUE;
    }
    else if(SYNC_TYPE_RFSER==hs_task_relay_info->synctype)
    {
         //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFSER\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_728_112_2_18_3_25_44_183,(uint8*)"");
         pMe->g_sync_result.delete_is_final=TRUE;
         pMe->g_sync_result.replace_is_final=TRUE;
         pMe->g_sync_result.add_is_final=TRUE;
    }
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
    else
    {
        // TODO: ͬ�����ʹ����� /*-wangxiaolin 2009.7.9 -*/
    }
    /* End the sync block */
    hs_syncml_EndSync(hs_task_relay_info->workspaceid);
	
    /* --- End message and the syncbody last parm--- */
    //modify by wangxiaolin for multi messages in one big package 2007.12.10
    //syncml_task_message:"MMIPIM  ^_^add_is_final=%d, delete_is_final=%d,replace_is_final=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_744_112_2_18_3_25_44_184,(uint8*)"ddd",pMe->g_sync_result.add_is_final,pMe->g_sync_result.delete_is_final,pMe->g_sync_result.replace_is_final);
    if((pMe->g_sync_result.add_is_final)
        &&(pMe->g_sync_result.delete_is_final)
        &&(pMe->g_sync_result.replace_is_final))
    {
        hs_syncml_EndMessage(hs_task_relay_info->workspaceid);
    }
    else
    {
        hs_syncml_EndMessageWithoutFinal(hs_task_relay_info->workspaceid);
        }
    #ifdef HS_PIM_CACHE_MAP  //add by wangxiaolin 2010.1.11 for cache map
    }
    #endif  /*HS_PIM_CACHE_MAP*/
    
    hs_task_relay_info->messageID++;   //count messageid
    hs_task_relay_info->cmdID=1;   //reset cmdid

//these are for syncml core test on windows. liugang make it disabled.2005-11-7.	
//make these changed to save logs on efs in debug model.  liugang 2006-5-29.
    /* --- Print the document which was generated --- */
#ifdef FEATURE_PIM_DEBUG
     //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_766_112_2_18_3_25_44_185,(uint8*)"");
     hs_myPrintxmlorwbxml("client_modi",hs_task_relay_info->workspaceid);
     //syncml_task_message:"MMIPIM  ^_^ Write xml logs complete.\n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_768_112_2_18_3_25_44_186,(uint8*)"");
#endif

   //syncml_task_message:"MMIPIM  ^_^ client modi data ok,begin send!time=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_771_112_2_18_3_25_44_187,(uint8*)"d",PIM_GetCurTime());
   /*---Send the data which is ready*/
   hs_syncml_Comm_SendData(hs_task_relay_info->workspaceid);
   //syncml_task_message:"MMIPIM  ^_^ client modi data send ok!\n"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_774_112_2_18_3_25_44_188,(uint8*)"");

      
    *hs_protocol_step=STEP_SERVER_CONF_MODI;
    *hs_protocol_step_priv=STEP_SERVER_MORE;
}

/******************************************************************* 
** ������:hs_step_server_conf_modi_process
** ����������taskģ�麯����server��������client��ͬ��
**                            ���ݣ���ʼ˫��ͬ������������modify
**                            ���ݡ�
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_server_conf_modi_process()
{
    //syncml_task_message:"@@step_server_conf_modi_processf  ,start time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_792_112_2_18_3_25_44_189,(uint8*)"d",PIM_GetCurTime());
      /* --- get the document which was in a file and was considered as the server init document --- */
     if(FALSE==hs_already_receive)
     {
      char worksapce_name[]="Hisense DS Server status and modi";
	  
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_server_conf_modi_process 1,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_798_112_2_18_3_25_44_190,(uint8*)"d",PIM_GetCurTime());
      hs_got_free_id();
	  
      hs_syncml_initance(&(hs_task_relay_info->workspaceid), worksapce_name);

       //syncml_task_message:"MMIPIM  ^_^ prepare ok,begin receive!\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_803_112_2_18_3_25_44_191,(uint8*)"");
       hs_syncml_Comm_RecData(hs_task_relay_info->workspaceid);
       //syncml_task_message:"MMIPIM  ^_^ server init receive ok!\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_805_112_2_18_3_25_44_192,(uint8*)"");

       hs_already_receive=TRUE;
       return;
     }
     else
     {
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_server_conf_modi_process 2,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_812_112_2_18_3_25_44_193,(uint8*)"d",PIM_GetCurTime());
     
#ifdef FEATURE_PIM_DEBUG
         //these are for syncml core test on windows. liugang make it disabled.2005-11-7.	  
         //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_816_112_2_18_3_25_44_194,(uint8*)"");
         hs_myPrintxmlorwbxml("server_modi", hs_task_relay_info->workspaceid);
         //syncml_task_message:"MMIPIM  ^_^ xml logs write complete.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_818_112_2_18_3_25_44_195,(uint8*)"");
#endif

         //syncml_task_message:"MMIPIM  ^_^ analysis recevie data.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_821_112_2_18_3_25_44_196,(uint8*)"");
         hs_syncml_ReceiveData(hs_task_relay_info->workspaceid);
         //syncml_task_message:"MMIPIM  ^_^ analysis server conf complete.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_823_112_2_18_3_25_44_197,(uint8*)"");

#ifdef HS_PIM_CACHE_MAP
        hs_mySaveCacheMap(); // wangxiaolin 20090804 ����map ��Ϣ
#endif
        if(*hs_protocol_step==STEP_SYNC_ERROR)
        {
              *hs_protocol_step_priv  = * hs_protocol_step;
        }
        else if(*hs_protocol_step==*hs_protocol_step_priv)
        {
    #ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for ����ͬ��&  ˢ��ͬ��
             if((SYNC_TYPE_1WAYFCLI==hs_task_relay_info->synctype)
                ||(SYNC_TYPE_RFCLI==hs_task_relay_info->synctype))
            {
               //�ͻ��˷���ĵ���ͬ���������Ѿ�ȫ������
               *hs_protocol_step = STEP_SYNC_OVER;
            }
            else
            {
                *hs_protocol_step=STEP_CLIENT_CONF_MAP;
            }
    #else
           *hs_protocol_step=STEP_CLIENT_CONF_MAP;
    #endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */           
        }
        else
        {
           *hs_protocol_step_priv=*hs_protocol_step;
           *hs_protocol_step=STEP_SERVER_MORE;
        }

        hs_already_receive=FALSE;
        //hs_need_holdon=TRUE;
       //hs_pim_hold_on();
	HS_Data_Sync_PostMsgToUI( PIM_MSG_NEXT_STEP,NULL, NULL);
      //syncml_task_message:"@@step_server_conf_modi_processf  ,end time=%d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_859_112_2_18_3_25_44_198,(uint8*)"d",PIM_GetCurTime());

	return;

     }
}

/******************************************************************* 
** ������:hs_step_client_conf_map_process
** ����������taskģ�麯����server��������client��ͬ��
**                            ���ݣ���ʼ˫��ͬ������������modify
**                            ���ݡ�
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_client_conf_map_process()
{

   char worksapce_name[]="Hisense DS Client status and map";
   
    //MMIPIM_DisplayProc(dis_chr);
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_client_conf_map_process,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_881_112_2_18_3_25_44_199,(uint8*)"d",PIM_GetCurTime());
    hs_got_free_id();
	
    hs_syncml_initance(&(hs_task_relay_info->workspaceid), worksapce_name);

    /*build sync header and syncbody first parm*/
    hs_syncml_startmessage(hs_task_relay_info->workspaceid);

    /*Start status cmd for two way sync server syncheader*/
    hs_syncml_StatusCmd(hs_task_relay_info->workspaceid);

    /*Send map cmd to the server*/
    hs_syncml_MapCmd(hs_task_relay_info->workspaceid);
	
    /* --- End message and the syncbody last parm--- */
    if (NULL == hs_globlemapItemList) // chenxiang_map 20080325
    {      
         hs_syncml_EndMessage(hs_task_relay_info->workspaceid);
    }
    else
    {
         hs_syncml_EndMessageWithoutFinal(hs_task_relay_info->workspaceid);
    }
        

    hs_task_relay_info->messageID++;   //count messageid
    hs_task_relay_info->cmdID=1;   //reset cmdid

  //these are for syncml core test on windows. liugang make it disabled.2005-11-7.	
      //make these changed to save logs on efs in debug model.  liugang 2006-5-29.
    /* --- Print the document which was generated --- */
   //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_912_112_2_18_3_25_45_200,(uint8*)"");
  #ifdef FEATURE_PIM_DEBUG
     hs_myPrintxmlorwbxml("client_map",hs_task_relay_info->workspaceid);
  #endif
  //syncml_task_message:"MMIPIM  ^_^ Write xml logs complete.\n"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_916_112_2_18_3_25_45_201,(uint8*)"");

   //syncml_task_message:"MMIPIM  ^_^ client conf data ok,begin send!time=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_918_112_2_18_3_25_45_202,(uint8*)"d",PIM_GetCurTime());
   /*---Send the data which is ready*/
   hs_syncml_Comm_SendData(hs_task_relay_info->workspaceid);
   //syncml_task_message:"MMIPIM  ^_^ client conf data send ok!\n"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_921_112_2_18_3_25_45_203,(uint8*)"");

    *hs_protocol_step=STEP_SERVER_MAPACK;
    *hs_protocol_step_priv=STEP_SERVER_MORE;
}

/******************************************************************* 
** ������:hs_step_server_mapack_process
** ����������taskģ�麯����server����map ack��Ϣ��ͬ
**                         �����Խ����ˡ�
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_server_mapack_process()
{
     //char * dis_chr="server mapack process";
     short result = 0; // chenixang_20080304 ��ͬ��ͨ�Ŵ���
    //MMIPIM_DisplayProc(dis_chr);
      /* --- get the document which was in a file and was considered as the server init document --- */
     if(FALSE==hs_already_receive)
     {
       char worksapce_name[]="Hisense DS Server map ack";
	  
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_server_mapack_process  1,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_945_112_2_18_3_25_45_204,(uint8*)"d",PIM_GetCurTime());
       hs_got_free_id();
	  
       result = hs_syncml_initance(&(hs_task_relay_info->workspaceid), worksapce_name); // chenxiang_20080304
       syncml_task_message(("MMIPIM chenxiang hs_step_server_mapack_process result = 0x%4x",result)); // chenxiang_20080304

       //syncml_task_message:"MMIPIM  ^_^ prepare ok,begin receive!\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_950_112_2_18_3_25_45_205,(uint8*)"");
       hs_syncml_Comm_RecData(hs_task_relay_info->workspaceid);
       //syncml_task_message:"MMIPIM  ^_^ server map ack receive ok!\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_952_112_2_18_3_25_45_206,(uint8*)"");

       hs_already_receive=TRUE;
       return;
     }
     else
     {
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_server_mapack_process  2,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_959_112_2_18_3_25_45_207,(uint8*)"d",PIM_GetCurTime());
      #if 0
         if(TRUE==hs_need_holdon)
         {
            hs_pim_hold_on();
            hs_need_holdon=FALSE;
            return;
         }
        #endif
#ifdef FEATURE_PIM_DEBUG
         //these are for syncml core test on windows. liugang make it disabled.2005-11-7.	  
         //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_970_112_2_18_3_25_45_208,(uint8*)"");
         hs_myPrintxmlorwbxml( "server_ack", hs_task_relay_info->workspaceid);
         //syncml_task_message:"MMIPIM  ^_^ xml logs write complete.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_972_112_2_18_3_25_45_209,(uint8*)"");
#endif

         //syncml_task_message:"MMIPIM  ^_^ analysis recevie data.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_975_112_2_18_3_25_45_210,(uint8*)"");
         hs_syncml_ReceiveData(hs_task_relay_info->workspaceid);
         //syncml_task_message:"MMIPIM  ^_^ analysis server map ack complete.\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_977_112_2_18_3_25_45_211,(uint8*)"");
         
#ifdef HS_PIM_CACHE_MAP
        hs_mySaveCacheMap(); // chenxiang 20090204 ����map ��Ϣ
#endif

        if (*hs_protocol_step != STEP_SYNC_ERROR)
        {
            if(*hs_protocol_step == *hs_protocol_step_priv)
            {
             #ifdef HS_PIM_CACHE_MAP // wangxiaolin 20090804
                   hs_mySetFlagOfSendingCacheMap(FALSE);
             #endif
                    *hs_protocol_step = STEP_SYNC_OVER;
            }
            else // chenxiang_map 20080325
            {
                *hs_protocol_step_priv = *hs_protocol_step;
                *hs_protocol_step = STEP_CLIENT_CONF_MAP;
            }
            hs_already_receive=FALSE;
           //hs_need_holdon=TRUE;
            //hs_pim_hold_on();
            HS_Data_Sync_PostMsgToUI(  PIM_MSG_NEXT_STEP,NULL, NULL);
        }
     }
}

/******************************************************************* 
** ������:hs_step_server_more
** ����������taskģ�麯��������multi-packages in one message
**                             with alert 222.
** ��  ��: null
** ��  ��:������
** ��  ��:2007-11-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_server_more()
{
//    char docname[]="clientmodi";
    char worksapce_name[]="Hisense DS multi-package";
    PIM_T_P  pMe=MMIPIM_GetPIM();
     //syncml_task_message:"MMIPIM  ^_^ Begin hs_step_server_more,time=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1018_112_2_18_3_25_45_212,(uint8*)"d",PIM_GetCurTime());
    hs_got_free_id();
	
    hs_syncml_initance(&(hs_task_relay_info->workspaceid), worksapce_name);
     
    /*build sync header and syncbody first parm*/
    hs_syncml_startmessage(hs_task_relay_info->workspaceid);

    /*Start status cmd for server syncheader*/
    hs_syncml_StatusCmd(hs_task_relay_info->workspaceid);

    /*Alert cmd for more!!with alert 222*/
    hs_syncml_AlertCmd(hs_task_relay_info->workspaceid,"222");
/* modify by wangxiaolin 2007.11.10 for send  big package  */
 if((pMe->g_sync_result.add_is_final)
        &&(pMe->g_sync_result.delete_is_final)
        &&(pMe->g_sync_result.replace_is_final))
 {

    if (NULL != hs_globlemapItemList)
    {
        /*Send map cmd to the server*/
        hs_syncml_MapCmd(hs_task_relay_info->workspaceid);
    }

    /* --- End message and the syncbody last parm--- */
    hs_syncml_EndMessageWithoutFinal(hs_task_relay_info->workspaceid); 
    //hs_syncml_EndMessage(hs_task_relay_info->workspaceid);
 }
 else
 {
 #ifdef HS_PIM_CACHE_MAP  //add by wangxiaolin 2009.08.14
    if (hs_myGetFlagOfSendingCacheMap())
    {
         //syncml_task_message:"MMIPIM  ^_^ send cache map another times....."
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1051_112_2_18_3_25_45_213,(uint8*)"");
         /*Send map cmd to the server*/
        hs_syncml_MapCmd(hs_task_relay_info->workspaceid);
    }
   #endif /*HS_PIM_CACHE_MAP*/
/*Start sync cmd for send the client's modify*/
    hs_syncml_StartSync(hs_task_relay_info->workspaceid);

    //syncml_task_message:"MMIPIM  ^_^ workspaceid=%d,authored=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1058_112_2_18_3_25_45_214,(uint8*)"dd",hs_task_relay_info->workspaceid,hs_task_relay_info->authored);
#ifdef HS_PIM_CACHE_MAP  //add by wangxiaolin 2010.1.11 for cache map
   if (NULL != hs_globlemapItemList) 
    {  
        /* End the sync block */
        hs_syncml_EndSync(hs_task_relay_info->workspaceid);
        hs_syncml_EndMessageWithoutFinal(hs_task_relay_info->workspaceid);
    }
    else
    {
 #endif /*HS_PIM_CACHE_MAP*/
        if(SYNC_TYPE_S2WAY==hs_task_relay_info->synctype)
        {
         //syncml_task_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_S2WAY\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1071_112_2_18_3_25_45_215,(uint8*)"");
         hs_syncml_fresh_for_slow_sync(hs_task_relay_info->workspaceid);
    }
    else if(SYNC_TYPE_2WAY==hs_task_relay_info->synctype)
    {
         hs_syncml_modify_for_fast_sync(hs_task_relay_info->workspaceid);
    }
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for ����ͬ��&  ˢ��ͬ��
     else if(SYNC_TYPE_1WAYFCLI==hs_task_relay_info->synctype)
    {
         //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFCLI\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1081_112_2_18_3_25_45_216,(uint8*)"");
         hs_syncml_modify_for_fast_sync(hs_task_relay_info->workspaceid);
    }
    else if(SYNC_TYPE_RFCLI==hs_task_relay_info->synctype)
    {
         //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFCLI\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1086_112_2_18_3_25_45_217,(uint8*)"");
         hs_syncml_fresh_for_slow_sync(hs_task_relay_info->workspaceid);
    }
     else if(SYNC_TYPE_1WAYFSER==hs_task_relay_info->synctype)
    {
         //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFSER\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1091_112_2_18_3_25_45_218,(uint8*)"");
    }
    else if(SYNC_TYPE_RFSER==hs_task_relay_info->synctype)
    {
         //syncml_message:"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFSER\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1095_112_2_18_3_25_45_219,(uint8*)"");
    }
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
    else
    {
        // TODO: ͬ�����ʹ����� /*-wangxiaolin 2009.7.9 -*/
    }
    /* End the sync block */
    hs_syncml_EndSync(hs_task_relay_info->workspaceid);
	
    /* --- End message and the syncbody last parm--- */
    //modify by wangxiaolin for multi messages in one big package 2007.12.10
    //final ��־Ϊȫ�ֱ���������ķ��Ͳ����п���ʹ���Ѿ��ı�
    //��Ҫ�����ж�
    if((pMe->g_sync_result.add_is_final)
        &&(pMe->g_sync_result.delete_is_final)
        &&(pMe->g_sync_result.replace_is_final))
    {
        hs_syncml_EndMessage(hs_task_relay_info->workspaceid);
    }
    else
    {
        hs_syncml_EndMessageWithoutFinal(hs_task_relay_info->workspaceid);
        }
     }
  #ifdef HS_PIM_CACHE_MAP  //add by wangxiaolin 2010.1.11 for cache map
  }
  #endif  /*HS_PIM_CACHE_MAP*/
    hs_task_relay_info->messageID++;   //count messageid
    hs_task_relay_info->cmdID=1;   //reset cmdid

//these are for syncml core test on windows. liugang make it disabled.2005-11-7.	
//make these changed to save logs on efs in debug model.  liugang 2006-5-29.
    /* --- Print the document which was generated --- */
#ifdef FEATURE_PIM_DEBUG
     //syncml_task_message:"MMIPIM  ^_^ Begin Write xml logs.\n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1130_112_2_18_3_25_45_220,(uint8*)"");
     hs_myPrintxmlorwbxml("server_more",hs_task_relay_info->workspaceid);
     //syncml_task_message:"MMIPIM  ^_^ Write xml logs complete.\n"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1132_112_2_18_3_25_45_221,(uint8*)"");
#endif

   //syncml_task_message:"MMIPIM  ^_^ server more data ok,begin send!time=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1135_112_2_18_3_25_45_222,(uint8*)"d",PIM_GetCurTime());
   /*---Send the data which is ready*/
   hs_syncml_Comm_SendData(hs_task_relay_info->workspaceid);
   //syncml_task_message:"MMIPIM  ^_^ server more data send ok!time=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1138_112_2_18_3_25_45_223,(uint8*)"d",PIM_GetCurTime());

      
    *hs_protocol_step=*hs_protocol_step_priv;
    *hs_protocol_step_priv=STEP_SERVER_MORE;
}

/******************************************************************* 
** ������:hs_pim_hold_on
** ��������������һ����Ϣѭ������ϵͳ���������¼�
** ��  ��: null
** ��  ��:������
** ��  ��:2009-7-29
** �汾: ver 1.0
*******************************************************************/
void hs_pim_hold_on()
{
      //syncml_task_message:"MMIPIM  ^_^ Hold on for 0.2 second!!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMDATASYNCTASK_1154_112_2_18_3_25_45_224,(uint8*)"");
      hs_smlLibSleep( 100 );
}


/******************************************************************* 
** ������:hs_pim_int2str
** ������������unit32�����ݽṹת��Ϊָ�����ȵ�str.
** ��  ��: null
** ��  ��:������
** ��  ��:2007-10-23
** �汾: ver 1.0
*******************************************************************/
void hs_pim_int2str(uint32 source, char* str,int length)
{
   while(length>0)
   {
      *(str+length-1)=(char)(source%10+0x30);
      source=source/10;
      length--;
   }
}


/*==========================================================
 * author		: wangxiaolin      
 * function 	: HS_Data_Sync_PostMsgToUI
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/29/2009 
 ==========================================================*/
 BOOLEAN HS_Data_Sync_PostMsgToUI(
                    MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                    DPARAM              param_ptr,      //the param of the message
                    uint32              size_of_param   //the size of the param
                    )
{
    
        return MMIPIM_PostMsgToSyncWin(msg_id,param_ptr,size_of_param);
}