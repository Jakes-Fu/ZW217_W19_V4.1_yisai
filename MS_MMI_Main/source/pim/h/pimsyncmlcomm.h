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
文件名称:PIMSyncmlComm.h
文件描述:通讯上层接口模块头文件
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
  -----------------------------------------------------------------------------
  *****************************************************************************/
  /*加入此FEATURE防止头文件重复包含*/
  #ifndef   HEADER_FILE_SYNCML_COMM
  #define HEADER_FILE_SYNCML_COMM
  
  /*  包含头文件*/
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

  /*  数据结构声明*/

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

   /*  全局变量声明*/

  /*  功能函数声明*/
  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_Init
  ** 功能描述：初始化通讯。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_Init(void);

  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_Destory
  ** 功能描述：通讯释放。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_Destory(void);

   /******************************************************************* 
  ** 函数名:hs_syncml_Comm_SendData
  ** 功能描述：发送数据接口。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_SendData(short id);


  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_ReSendData
  ** 功能描述：重新发送数据
  ** 输  出: 返回操作状态
  ** 作  者:陈翔
  ** 日  期:20080326
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_ReSendData(void);
  
  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_SendInterface
  ** 功能描述：发送数据接口。内部轮回
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_SendInterface(void);

  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_RecData
  ** 功能描述：接收数据接口。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_RecData(short id);

  /******************************************************************* 
  ** 函数名:hs_syncml_Comm_RecInterface
  ** 功能描述：接收数据接口。内部轮回
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Comm_RecInterface(void);


   /******************************************************************* 
  ** 函数名:hs_syncml_Set_Supported_Comm
  ** 功能描述：设置全局变量，付值指针函数。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-9-7
  ** 版本: ver 1.0
  *******************************************************************/
  short hs_syncml_Set_Supported_Comm(uint8 type);
  #endif
