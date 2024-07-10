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
�ļ�����:PIMSyncmlComm.h
�ļ�����:ͨѶ�ϲ�ӿ�ģ��ͷ�ļ�
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
  -----------------------------------------------------------------------------
  *****************************************************************************/
  /*�����FEATURE��ֹͷ�ļ��ظ�����*/
  #ifndef   HEADER_FILE_SYNCML_COMM
  #define HEADER_FILE_SYNCML_COMM
  
  /*  ����ͷ�ļ�*/
#include "sci_types.h"
  
  #include"PIMcomdef.h"
  #include "PIMlibmem.h"

  #ifdef HS_FEATURE_COMM_HTTP_SUPPORT
  #include "PIMCommhttp.h"
  #endif
  #ifdef HS_FEATURE_COMM_WSP_SUPPORT
  #include "PIMCommwsp.h"
  #endif
  #ifdef HS_FEATURE_COMM_OBEX_SUPPORT
  #include "PIMCommobex.h"
  #endif

  /*  ���ݽṹ����*/

  typedef enum
  {
  	NONE_support=0,           //default
	HTTP_support,               //HTTP support
	WSP_support,                //WSP support
	OBEX_support                //OBEX support  eg. USB,UART,Blue tooth etc.
  }syncml_Comm_bearer_type;

  typedef enum
{
      XML_UNDEF = 0,
	XML_WBXML=1,
	XML_XML
}XmlCodeType;

  typedef struct  syncml_Comm_type
  {
  	char* cache;                   //buffer for globle bearer pointer to use.
	uint32 cache_length;      //buffer length
	XmlCodeType codetype;   //syncml is xml or wbxml
	struct http_globle_indicator_type* privateinfo;          //private pointer, to save the supported bearer information,such as http's server addr&port number.
	short (*prepare)();     //pointer of function.  prepare the bearer
	short (*release)();  //pointer of function. release the resources when prepare in the beginning.
	short (*bearer_SendData)();   //pointer of function. connect to server and send data.
	short (*bearer_ReceiveData)( );  //pointer of function. recevie data then release the connect.
	char * reSend_cache; // chenxiang_20080326
        uint32 reSend_cache_length; // chenxiang_20080326
  }syncml_Comm_type;

   /*  ȫ�ֱ�������*/

  /*  ���ܺ�������*/
  /******************************************************************* 
  ** ������:hs_syncml_Comm_Init
  ** ������������ʼ��ͨѶ��
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-11-14
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_Init(void);

  /******************************************************************* 
  ** ������:hs_syncml_Comm_Destory
  ** ����������ͨѶ�ͷš�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-11-14
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_Destory(void);

   /******************************************************************* 
  ** ������:hs_syncml_Comm_SendData
  ** �����������������ݽӿڡ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_SendData(short id);


  /******************************************************************* 
  ** ������:hs_syncml_Comm_ReSendData
  ** �������������·�������
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:20080326
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_ReSendData(void);
  
  /******************************************************************* 
  ** ������:hs_syncml_Comm_SendInterface
  ** �����������������ݽӿڡ��ڲ��ֻ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_SendInterface(void);

  /******************************************************************* 
  ** ������:hs_syncml_Comm_RecData
  ** �����������������ݽӿڡ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_RecData(short id);

  /******************************************************************* 
  ** ������:hs_syncml_Comm_RecInterface
  ** �����������������ݽӿڡ��ڲ��ֻ�
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_RecInterface(void);


   /******************************************************************* 
  ** ������:hs_syncml_Set_Supported_Comm
  ** ��������������ȫ�ֱ�������ֵָ�뺯����
  ** ��  ��: ���ز���״̬
  ** ��  ��:����
  ** ��  ��:2005-9-7
  ** �汾: ver 1.0
  *******************************************************************/
  short hs_syncml_Set_Supported_Comm(uint8 type);
  #endif
