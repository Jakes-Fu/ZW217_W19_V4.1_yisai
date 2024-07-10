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
文件名称:PIMdatasynctask.h
文件描述:task模块头文件
文件说明:完成task模块的功能，主要是完成syncml整个功能的
                       指挥。
开发人员:刘刚
首次开发日期:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-4-11              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*加入此FEATURE防止头文件重复包含*/
#ifndef   HEADER_FILE_DATASYNC_TASK
  #define HEADER_FILE_DATASYNC_TASK
  /*  包含头文件*/
  #include "PIMsyncmlmgr.h"
  #include "mmipim.h"//wangzhefeng add 20090910
  #include "mmi_custom_define.h"
  /*  数据结构声明*/

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

  //版本默认的参数配置
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
  
 /*task进行传输控制需要的主要步骤*/
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

/*task同步一次的数据状态信息*/
typedef struct
{
	int allneedsend;    //一个message中共需要发送的数据条数
	int allneedreceive; //一个message中共需要接受的收据条数
	int nowsendno;      //现在发送到第几条
	int nowreceiveno;   //现在接收到第几条
	int allsendsuc;        //一次同步共发送成功的数据条数
	int allreceivesuc;   //一次同步共接收成功的数据条数
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
    char*  last_anchor;        //last anchor信息，
    char*  next_anchor;       //next anchor信息，读取于发起同步的时间
    char*  cred;                    //认证证书，一组数据，用来鉴权
    unsigned char    authored;            //通过鉴权标志。true: 通过false:未通过
    auth_type  authtype;        //auth type b64 or md5 or none.
    char*  nonce;                   //auth nonce  used by b64&md5
    char*  nextnonce;           //auth next nonce from server which is used by client for the next md5 auth. 
    short   workspaceid;                               //syncml core workspace id
    SmlEncoding_t    encodetype;                  //编解码方式xml/wbxml
    sync_type       synctype;                         //synctype,1:twoway,2:slowsync
    int8    use_proxy;            //是否使用代理服务器
    char*  proxy_ip;                //使用代理情况下代理服务器IP地址
    char*  proxy_port;            //使用代理情况下代理服务器端口
    char*  proxy_username;  //使用代理情况下代理服务器用户名
    char*  proxy_password;   //使用代理情况下代理服务器用户密码
    char*  syncml_pim_server_name;                 //SYNCML协议参数PIM服务器名称
    char*  syncml_pim_server_phonebook_db;  //SYNCML协议参数PIM服务器上地址簿数据库名称
    char*  syncml_pim_server_calendar_db;      //SYNCML协议参数PIM服务器上日程表数据库名称
    char*  syncml_pim_server_port;                  //SYNCML协议参数PIM服务器端口
    char*  syncml_pim_username;                     //SYNCML协议业务用户名--终端本机号码
    char*  syncml_pim_password;                      //SYNCML协议业务用户密码--终端在运营商处注册后得到的使用密码
    char*  syncml_pim_client_imei;                    //SYNCML协议参数PIM终端IMEI号
    char*  syncml_pim_client_phonebook_db;   //SYNCML协议参数PIM终端上地址簿数据库名称
    char*  syncml_pim_client_calendar_db;        //SYNCML协议参数PIM终端上日程表数据库名称
    char*  syncml_pim_nextnonce;                    //SYNCML协议业务用md5鉴权nextnonce值，初始化为邋"nonce"，后来自于server。
}task_relay_info_type;

  /*  全局变量声明*/

  /*  功能函数声明*/
short hs_Data_Sync_main(void);
/******************************************************************* 
** 函数名:hs_Data_Sync_init
** 功能描述：初始化task模块，进行内存分配。
** 输  出: null
** 作  者:王晓林
** 日  期:2007-9
*******************************************************************/
  short hs_Data_Sync_init(void);

/******************************************************************* 
** 函数名:hs_Data_Sync_terminate
** 功能描述：终结数据同步，释放同步过程中申请
**                            的内存空间。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
  void hs_Data_Sync_terminate(void);

/******************************************************************* 
** 函数名:hs_Data_Sync_task
** 功能描述：task模块主函数，控制整个同步的过程。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
  void hs_Data_Sync_task(void);

/******************************************************************* 
** 函数名:hs_got_free_id
** 功能描述：因为instance一共只有4个实例，因此当
**                        全部用完时，需要释放当前第一个，然
**                        后再重新分配内存。此函数找到当前的
**                        第一个实例，然后释放它，同时将这个
**                        id设置为当前的id。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_got_free_id(void);

/******************************************************************* 
** 函数名:hs_step_client_init_sync_process
** 功能描述：task模块函数，client建立同步。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_step_client_init_sync_process(void);

/******************************************************************* 
** 函数名:hs_step_server_init_sync_process
** 功能描述：task模块函数，server响应，并建立同步。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_step_server_init_sync_process(void);

/******************************************************************* 
** 函数名:hs_step_client_modi_process
** 功能描述：task模块函数，client与server建立好同步，
**                            开始双向同步动作，另外，client向server
**                            发送更改过的数据。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_step_client_modi_process(void);

/******************************************************************* 
** 函数名:hs_step_server_conf_modi_process
** 功能描述：task模块函数，server接收来自client的同步
**                            数据，开始双向同步动作，发送modify
**                            数据。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_step_server_conf_modi_process(void);

/******************************************************************* 
** 函数名:hs_step_client_conf_map_process
** 功能描述：task模块函数，server接收来自client的同步
**                            数据，开始双向同步动作，发送modify
**                            数据。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_step_client_conf_map_process(void);

/******************************************************************* 
** 函数名:hs_step_server_mapack_process
** 功能描述：task模块函数，server发送map ack消息，同
**                         步可以结束了。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_step_server_mapack_process(void);

/******************************************************************* 
** 函数名:hs_step_server_more
** 功能描述：task模块函数，用于multi-packages in one message
**                             with alert 222.
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_step_server_more(void);

/******************************************************************* 
** 函数名:hs_pim_hold_on
** 功能描述：task模块函数，用于给系统一些时间，
                                 进行efs操作。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
void hs_pim_hold_on(void);

/******************************************************************* 
** 函数名:hs_pim_return_back
** 功能描述：task模块函数，用于给系统一些时间，
                                 进行efs操作。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
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
