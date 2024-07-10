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
文件名称:PIMCommhttp.h
文件描述:HTTP通讯方式绑定支持头文件
文件说明:提供HTTP通讯方式的绑定，实现在HTTP下的通讯。
开发人员:刘刚
首次开发日期:2005-7-9
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2006-6-8              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
  /*加入此FEATURE防止头文件重复包含*/
  #ifndef   HEADER_FILE_COMM_HTTP
  #define HEADER_FILE_COMM_HTTP

  /*  包含头文件*/
  #include "PIMcomdef.h"
  #include "PIMSyncmlComm.h"
  #include "mmipim.h"
  #include "mmi_custom_define.h"
 #ifdef HS_FEATURE_COMM_HTTP_SUPPORT
 //为了方便展讯打包成库而转放到.h中,UA参数在客户后续项目中是需要修改的 by wangxiaolin ,2010.01.06
#ifdef HS_PIM_UA_SETTING  //modify by wangxiaolin 2009.12.21
#define CLIENT_NAME_UA_INFO_HS      PIM_OTHER_DEVICE_UA_INFO
#endif

#define CLIENT_NAME_UA_INFO    		PIM_DEFAULT_DEVICE_UA_INFO

  /*  数据结构声明*/

//用来指示发送、接收等通讯动作的步骤。
typedef enum
{
	SOCKET_STEP_CONNECT=0,    //下一步应该进行connect连接
	SOCKET_STEP_1,                    //下一步应该进行http header的发送或接收
	SOCKET_STEP_2,                   //下一步应该进行数据的发送或接收
	SOCKET_STEP_3                     //下一步应该通知syncml task进行下一步动作。
}Socket_Step_type;

//http层的全局变量类型。
typedef struct
{
	Socket_Step_type socket_step;    //socket 步骤
	char*                   server;             //用于POST元语和HOST元语后的内容
	char*                   serverport;                 //端口
	char*                   susername;         //服务器用户名
	char*                   spassword;         //服务器口令
	char*                   agent;              //用于socket  connect的代理服务器地址，当不使用代理时，连接的应该是servername
	char*                   agentport;        //代理服务器端口
	char*                   ausername;       //代理服务器用户名
	char*                   apassword;         //代理服务器口令
	int8                     useagent;        //标示是否使用代理服务器。
	char*                    headerbuffer;  //用来存储http header的buffer
	uint32                  usedsize;         //存储http header使用的buffer size
}http_globle_indicator_type;

#ifndef WIN32
typedef struct sci_sockaddr sci_sa;

struct sockaddr                        /* generic socket address structure */
{
  uint16 sa_family;                               /* socket address family */
  unsigned char   sa_data[14];                             /* address data */
};

#endif
  /*  全局变量声明*/

  /*  功能函数声明*/
  
  /******************************************************************* 
  ** 函数名:HS_HTTP_Prepare
  ** 功能描述：初始化HTTP 相关数据，例如服务器地址
                                  、端口。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_HTTP_Prepare(void);

  /******************************************************************* 
  ** 函数名:HS_HTTP_Release
  ** 功能描述：释放HTTP 相关数据，例如服务器地址
                                  、端口。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_HTTP_Release(void);

  /******************************************************************* 
  ** 函数名:HS_HTTP_SendData
  ** 功能描述：连接服务器，生成httpheader，并发送，
                                  然后发送想要发送的数据。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_HTTP_SendData(void);

  /******************************************************************* 
  ** 函数名:HS_HTTP_RecData
  ** 功能描述：接收返回的httpheader，然后接收header中
                                  指定的数据。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_HTTP_RecData(void);


  /******************************************************************* 
  ** 函数名:HS_HTTP_CloseConnection
  ** 功能描述：close connection.
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  void  HS_HTTP_CloseConnection(void);

 int HS_PIM_socket_select_handle(void);

  #endif

  #endif
