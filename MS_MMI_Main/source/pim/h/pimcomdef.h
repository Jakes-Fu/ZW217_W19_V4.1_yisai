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
文件名称:PIMcomdef.h
文件描述:基本定义，用于定义一些基本的数据类型。
文件说明:基本数据类型和feature定义头文件。
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
  #ifndef   HEADER_FILE_COMDEF
  #define  HEADER_FILE_COMDEF
  /*  包含头文件*/
 // #include "Pim.h"

  /*  开关定义声明*/

/* 支持WBXML编码开关 */
#define __HS_SML_WBXML__
/* 支持XML编码 开关*/
#define __HS_SML_XML__

/* 支持 Sub DTD extensions */
#define __HS_USE_EXTENSIONS__

/* 使用Metainformation DTD  */
#define __HS_USE_METINF__
/* 使用 Device Info DTD  */
#define __HS_USE_DEVINF__

/*支持syncml1.1开关*/
#define __HS_SML_1_1__

/*支持syncml1.2开关*/
//#define __SML_1_2__

/*网络承载方式的支持*/
#define HS_FEATURE_COMM_HTTP_SUPPORT   //we support http binding.
//#define HS_FEATURE_COMM_WSP_SUPPORT    //wsp binding wil be supported later!
//#define HS_FEATURE_COMM_OBEX_SUPPORT   //obex binding will be supported later!

/* which of the following optional commands should be included ? */
#define HS_ADD_SEND

#define HS_COPY_SEND
#define HS_COPY_RECEIVE

#define HS_ATOMIC_SEND
#define HS_ATOMIC_RECEIVE

#define HS_SEQUENCE_SEND
#define HS_SEQUENCE_RECEIVE

#define HS_EXEC_SEND

#define HS_SEARCH_SEND

#define HS_MAP_RECEIVE

#define HS_RESULT_RECEIVE

#define HS_EXEC_RECEIVE

#define HTTPLOGNAME  "httplogs"  
  /*  数据结构声明*/
/** 
 * PCDATA - types of synchronization data which SyncML supports
 **/
typedef enum {
  SML_PCDATA_UNDEFINED = 0,
  SML_PCDATA_STRING,                   // String type
  SML_PCDATA_OPAQUE,                   // Opaque type
  SML_PCDATA_EXTENSION,                // Extention type - specified by PcdataExtension_t
  SML_PCDATA_CDATA                     // XML CDATA type   
} SmlPcdataType_t;


/**
 * PCDATA - types of extensions for PCData elements
 */
typedef enum {
  SML_EXT_UNDEFINED = 0,
  SML_EXT_METINF, // Meta Information
  SML_EXT_DEVINF, // Device Information
  SML_EXT_LAST,    // last codepage, needed for loops!
  SML_EXT_MAX=254
} SmlPcdataExtension_t;


/** 
 * PCDATA - into this structure SyncML wraps the synchronization data itself
 **/ 
typedef struct sml_pcdata_s {
  SmlPcdataType_t       contentType;   // The type of data which a PCDATA structure contains
  SmlPcdataExtension_t  extension;     // PCData Extension type
  long             length;        // length of the data in this PCDATA structure
  void*             content;       // Pointer to the data itself
} *SmlPcdataPtr_t, SmlPcdata_t;      

/* generic list of PCData elements */
typedef struct sml_pcdata_list_s {
	SmlPcdataPtr_t	data;
	struct sml_pcdata_list_s *next;
} *SmlPcdataListPtr_t, SmlPcdataList_t;
  
  /*  全局变量声明*/

  /*  功能函数声明*/

#endif
