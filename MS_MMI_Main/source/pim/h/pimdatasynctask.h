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
�ļ�����:PIMdatasynctask.h
�ļ�����:taskģ��ͷ�ļ�
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
/*�����FEATURE��ֹͷ�ļ��ظ�����*/
#ifndef   HEADER_FILE_DATASYNC_TASK
  #define HEADER_FILE_DATASYNC_TASK
  /*  ����ͷ�ļ�*/
  #include "PIMsyncmlmgr.h"
  #include "mmipim.h"//wangzhefeng add 20090910
  #include "mmi_custom_define.h"
  /*  ���ݽṹ����*/

  #define  HS_DEVICE_INFO_CTTYPE                 "text/vcard"
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
  #define  HS_DEVICE_INFO_CTTYPE_VCAL                 "text/vcalendar"
#endif /* HS_PIM_VCAL */
  #define  HS_DEVICE_INFO_DATATYPE_CHR    "chr"
  #define  HS_DEVICE_INFO_DATATYPE_INT     "int"
  #define  HS_DEVICE_INFO_DATATYPE_BOOL  "bool"
  #define  HS_DEVICE_INFO_DATATYPE_BIN     "bin"
  #define  HS_DEVICE_INFO_DATATYPE_DTAETIME   "datetime"
  #define  HS_DEVICE_INFO_DATATYPE_PHONENUM  "phonenum"
  #define  HS_DEVICE_INFO_DEVID                    "004400-15-202000-0"
  #define  HS_DEVICE_INFO_DEVTYP                  "phone"
  #define  HS_DEVICE_INFO_DISPNAME              "Addressbook"
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
#define  HS_DEVICE_INFO_DEVTYP_VCAL               "Schedule"
#define  HS_DEVICE_INFO_DISPNAME_VCAL              "Schedule"
#endif /* HS_PIM_VCAL */
  #define  HS_DEVICE_INFO_FWV                       "20060101"
  
  #define  HS_DEVICE_INFO_HWV                       "1.0"
  
  #ifdef HS_PIM_UA_SETTING 
  #define  DEVICE_INFO_MAN_HS                       PIM_OTHER_DEVICE_INFO_MAN
  #define  DEVICE_INFO_OEM_HS                       PIM_OTHER_DEVICE_INFO_OEM
  #define  DEVICE_INFO_MOD_HS                       PIM_OTHER_DEVICE_INFO_MOD
  #endif

  //�汾Ĭ�ϵĲ�������
  #define  DEVICE_INFO_MAN                       PIM_DEFAULT_DEVICE_INFO_MAN
  #define  DEVICE_INFO_OEM                       PIM_DEFAULT_DEVICE_INFO_OEM
  #define  DEVICE_INFO_MOD                       PIM_DEFAULT_DEVICE_INFO_MOD

  
  #define  HS_DEVICE_INFO_MAXGUIDESIZE      "2"
  #define  HS_DEVICE_INFO_MAXID                    "1000"
  #define  HS_DEVICE_INFO_MAXMEM                 "50000"

  
  #define  HS_DEVICE_INFO_SOURCEREF            "./phonebook"
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
  #define  HS_DEVICE_INFO_SOURCEREF_VCAL            "./schedule"
#endif /* HS_PIM_VCAL */
  #define  HS_DEVICE_INFO_SWV                        "1.0.0"
  #define  HS_DEVICE_INFO_SYNCTYPE_TWOWAY   "1"   //support of two-way sync
  #define  HS_DEVICE_INFO_SYNCTYPE_STWOWAY  "2"  //support of slow two-way sync
  #define  HS_DEVICE_INFO_SYNCTYPE_SERVALERT "7"  //support of server alerted sync
  #define  HS_DEVICE_INFO_VERCT_VCARD               "2.1"   //VerCT
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
  #define  HS_DEVICE_INFO_VERCT_VCAL               "1.0"   //VerCT
#endif /* HS_PIM_VCAL */
  #define  HS_DEVICE_INFO_VERDTD                         "1.1"
  #define  HS_DEVICE_INFO_XNAM                            "nothing"
  #define  HS_DEVICE_INFO_XVAL                             "1"
  
 /*task���д��������Ҫ����Ҫ����*/
  typedef enum
{
	STEP_CLIENT_INIT_SYNC=0,      //client sync initialization
	STEP_SERVER_INIT_SYNC,        //server sync initialization
	STEP_CLIENT_MODI,             //client send modified package
	STEP_SERVER_CONF_MODI,     //server send status and server modified package
	STEP_CLIENT_CONF_MAP,       //client send status and map
	STEP_SERVER_MAPACK,         //server map acknowledgement
	STEP_SERVER_MORE,             //server info without <final/> flag,so we should send alert 222 for more.
	STEP_SYNC_OVER,                   //all step over, release the resources,then set step to client init.
	STEP_SYNC_ERROR
}task_protocol_step_type; 

/*taskͬ��һ�ε�����״̬��Ϣ*/
typedef struct
{
	int allneedsend;    //һ��message�й���Ҫ���͵���������
	int allneedreceive; //һ��message�й���Ҫ���ܵ��վ�����
	int nowsendno;      //���ڷ��͵��ڼ���
	int nowreceiveno;   //���ڽ��յ��ڼ���
	int allsendsuc;        //һ��ͬ�������ͳɹ�����������
	int allreceivesuc;   //һ��ͬ�������ճɹ�����������
}percentInfo_type;
#if 0
/*authentication type base64 or md5 or nothing*/
typedef enum
{
      AUTH_DUMMY=0,
	AUTH_B64,
	AUTH_MD5
}auth_type;
#endif

#if 0//del by wangxiaolin  2009.7.16
/*supported sync type*/
typedef enum
{
   SYNC_TYPE_DUMMY=0,          //0:dummy
   SYNC_TYPE_2WAY,                // 1: two way sync
   SYNC_TYPE_S2WAY,              // 2: slow two way sync
   SYNC_TYPE_1WAYFCLI,        // 3: one way sync from client only
   SYNC_TYPE_RFCLI,               // 4: refresh sync from client only
   SYNC_TYPE_1WAYFSER,       // 5: one way sync from server only
   SYNC_TYPE_RFSER,              // 6: refresh sync from server only
   SYNC_TYPE_SERALERT        // 7: server alerted sync
}sync_type;
#endif

typedef struct
{
    unsigned int sessionID;       //session ID
    unsigned int messageID;     //messageID
    unsigned int cmdID;           //command ID
    char*  last_anchor;        //last anchor��Ϣ��
    char*  next_anchor;       //next anchor��Ϣ����ȡ�ڷ���ͬ����ʱ��
    char*  cred;                    //��֤֤�飬һ�����ݣ�������Ȩ
    unsigned char    authored;            //ͨ����Ȩ��־��true: ͨ��false:δͨ��
    auth_type  authtype;        //auth type b64 or md5 or none.
    char*  nonce;                   //auth nonce  used by b64&md5
    char*  nextnonce;           //auth next nonce from server which is used by client for the next md5 auth. 
    short   workspaceid;                               //syncml core workspace id
    SmlEncoding_t    encodetype;                  //����뷽ʽxml/wbxml
    sync_type       synctype;                         //synctype,1:twoway,2:slowsync
    int8    use_proxy;            //�Ƿ�ʹ�ô��������
    char*  proxy_ip;                //ʹ�ô�������´��������IP��ַ
    char*  proxy_port;            //ʹ�ô�������´���������˿�
    char*  proxy_username;  //ʹ�ô�������´���������û���
    char*  proxy_password;   //ʹ�ô�������´���������û�����
    char*  syncml_pim_server_name;                 //SYNCMLЭ�����PIM����������
    char*  syncml_pim_server_phonebook_db;  //SYNCMLЭ�����PIM�������ϵ�ַ�����ݿ�����
    char*  syncml_pim_server_calendar_db;      //SYNCMLЭ�����PIM���������ճ̱����ݿ�����
    char*  syncml_pim_server_port;                  //SYNCMLЭ�����PIM�������˿�
    char*  syncml_pim_username;                     //SYNCMLЭ��ҵ���û���--�ն˱�������
    char*  syncml_pim_password;                      //SYNCMLЭ��ҵ���û�����--�ն�����Ӫ�̴�ע���õ���ʹ������
    char*  syncml_pim_client_imei;                    //SYNCMLЭ�����PIM�ն�IMEI��
    char*  syncml_pim_client_phonebook_db;   //SYNCMLЭ�����PIM�ն��ϵ�ַ�����ݿ�����
    char*  syncml_pim_client_calendar_db;        //SYNCMLЭ�����PIM�ն����ճ̱����ݿ�����
    char*  syncml_pim_nextnonce;                    //SYNCMLЭ��ҵ����md5��Ȩnextnonceֵ����ʼ��Ϊ��"nonce"����������server��
}task_relay_info_type;

  /*  ȫ�ֱ�������*/

  /*  ���ܺ�������*/
short hs_Data_Sync_main(void);
/******************************************************************* 
** ������:hs_Data_Sync_init
** ������������ʼ��taskģ�飬�����ڴ���䡣
** ��  ��: null
** ��  ��:������
** ��  ��:2007-9
*******************************************************************/
  short hs_Data_Sync_init(void);

/******************************************************************* 
** ������:hs_Data_Sync_terminate
** �����������ս�����ͬ�����ͷ�ͬ������������
**                            ���ڴ�ռ䡣
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
  void hs_Data_Sync_terminate(void);

/******************************************************************* 
** ������:hs_Data_Sync_task
** ����������taskģ������������������ͬ���Ĺ��̡�
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
  void hs_Data_Sync_task(void);

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
void hs_got_free_id(void);

/******************************************************************* 
** ������:hs_step_client_init_sync_process
** ����������taskģ�麯����client����ͬ����
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_client_init_sync_process(void);

/******************************************************************* 
** ������:hs_step_server_init_sync_process
** ����������taskģ�麯����server��Ӧ��������ͬ����
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_server_init_sync_process(void);

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
void hs_step_client_modi_process(void);

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
void hs_step_server_conf_modi_process(void);

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
void hs_step_client_conf_map_process(void);

/******************************************************************* 
** ������:hs_step_server_mapack_process
** ����������taskģ�麯����server����map ack��Ϣ��ͬ
**                         �����Խ����ˡ�
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_server_mapack_process(void);

/******************************************************************* 
** ������:hs_step_server_more
** ����������taskģ�麯��������multi-packages in one message
**                             with alert 222.
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_step_server_more(void);

/******************************************************************* 
** ������:hs_pim_hold_on
** ����������taskģ�麯�������ڸ�ϵͳһЩʱ�䣬
                                 ����efs������
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_pim_hold_on(void);

/******************************************************************* 
** ������:hs_pim_return_back
** ����������taskģ�麯�������ڸ�ϵͳһЩʱ�䣬
                                 ����efs������
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
void hs_pim_return_back(void);

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
                    );

#endif
