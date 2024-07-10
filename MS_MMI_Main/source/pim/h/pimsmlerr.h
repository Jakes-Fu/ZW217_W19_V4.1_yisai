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
文件名称:PIMsmlerr.h
文件描述:异常信息头文件
文件说明:工作过程中产生异常的返回所定义的异常信息。
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
  #ifndef   HEADER_FILE_SMLERR
  #define  HEADER_FILE_SMLERR
  
  /*  包含头文件*/

  /*  数据结构声明*/
  /**
 * No error, success code
 **/
#define HS_SML_ERR_OK 0x00				    // OK	




/**
 * SyncML Common Error Codes
 **/

// general errors
#define HS_SML_ERR_UNSPECIFIC       0x10      // unspecific error
#define HS_SML_ERR_NOT_ENOUGH_SPACE 0x11      // not enough memory to perform this operation
#define HS_SML_ERR_WRONG_USAGE      0x13      // function was called in wrong context

// wrong parameters
#define HS_SML_ERR_WRONG_PARAM      0x20      // wrong parameter
#define HS_SML_ERR_INVALID_SIZE     0x21      // param has an invalid size
#define HS_SML_ERR_INVALID_HANDLE   0x22      // if handle is invalid/unknown
#define HS_SML_ERR_INVALID_OPTIONS  0x23      // unkown or unallowed options


/**
 * SyncML Mgr Error Codes
 **/
#define HS_SML_ERR_A_MGR_ERROR                 0x1001   // a template
#define HS_SML_ERR_MGR_INVALID_INSTANCE_INFO   0x1002   // a invalid Instance Info structure is used
#define HS_SML_ERR_COMMAND_NOT_HANDLED         0x1003   // no callback function is available to handle this command
#define HS_SML_ERR_ALREADY_INITIALIZED         0x1004   // Mgr allready initialized
 

/**
 * SyncML Xlt Error Codes
 **/
#define HS_SML_ERR_XLT_MISSING_CONT            0x2001   // required field content missing
#define HS_SML_ERR_XLT_BUF_ERR                 0x2002   // Buffer too small 
#define HS_SML_ERR_XLT_INVAL_PCDATA_TYPE       0x2003   // Invalid (WBXML) Element Type (STR_I etc.)
#define HS_SML_ERR_XLT_INVAL_LIST_TYPE         0x2004   // Invalid List Type (COL_LIST etc.)
#define HS_SML_ERR_XLT_INVAL_TAG_TYPE          0x2005   // Invalid Tag Type (TT_BEG etc.)
#define HS_SML_ERR_XLT_ENC_UNK	                0x2007   // Unknown Encoding (WBXML, XML)
#define HS_SML_ERR_XLT_INVAL_PROTO_ELEM        0x2008   // Invalid Protocol Element (ADD, Delete, ...)
#define HS_SML_ERR_MISSING_LIST_ELEM           0x2009   // Missing Content of List Elements 
#define HS_SML_ERR_XLT_INCOMP_WBXML_VERS       0x200A   // Incompatible WBXML Content Format Version
#define HS_SML_ERR_XLT_INVAL_SYNCML_DOC        0x200B   // Document does not conform to SyncML DTD 
#define HS_SML_ERR_XLT_INVAL_PCDATA            0x200C   // Invalid PCData elem (e.g. not encoded as OPAQUE data)
#define HS_SML_ERR_XLT_TOKENIZER_ERROR         0x200D   // Unspecified tokenizer error
#define HS_SML_ERR_XLT_INVAL_WBXML_DOC         0x200E   // Document does not conform to WBXML specification
#define HS_SML_ERR_XLT_WBXML_UKN_TOK           0x200F   // Document contains unknown WBXML token
#define HS_SML_ERR_XLT_MISSING_END_TAG         0x2010   // Non-empty start tag without matching end tag
#define HS_SML_ERR_XLT_INVALID_CODEPAGE        0x2011   // WBXML document uses unspecified code page
#define HS_SML_ERR_XLT_END_OF_BUFFER           0x2012   // End of buffer reached
#define HS_SML_ERR_XLT_INVAL_XML_DOC           0x2013   // Document does not conform to XML 1.0 specification
#define HS_SML_ERR_XLT_XML_UKN_TAG             0x2014   // Document contains unknown XML tag
#define HS_SML_ERR_XLT_INVAL_PUB_IDENT         0x2015   // Invalid Public Identifier
#define HS_SML_ERR_XLT_INVAL_EXT               0x2016   // Invalid Codepage Extension
#define HS_SML_ERR_XLT_NO_MATCHING_CODEPAGE    0x2017   // No matching Codepage could be found
#define HS_SML_ERR_XLT_INVAL_INPUT_DATA        0x2018   // Data missing in input structure


/**
 * SyncML Wsm Error Codes
 **/
#define HS_SML_ERR_WSM_BUF_TABLE_FULL          0x3001   // no more empty entries in buffer table available

/**
 * SyncML Util Error Codes
 **/
#define HS_SML_ERR_A_UTI_UNKNOWN_PROTO_ELEMENT 0x7001


//Communication error signals!!!
#define HS_SML_ERR_A_COMM_ERROR                 0x5001
#if 0//del by wangxiaolin  2009.10.13
#define SML_ERR_A_COMM_SERVER_AUTH      HS_SML_ERR_A_COMM_ERROR + 0x02 // Server authentication failed
#define SML_ERR_A_COMM_ACCESS_DENIED    HS_SML_ERR_A_COMM_ERROR + 0x03 // Server rejected request
#define SML_ERR_A_COMM_HTTP_ERROR       HS_SML_ERR_A_COMM_ERROR + 0x04 // HTTP error
#define SML_ERR_A_COMM_COMMUNICATION    HS_SML_ERR_A_COMM_ERROR + 0x10 // communication error

#define SML_ERR_A_COMM_INVALID_PROTOCOL HS_SML_ERR_A_COMM_ERROR + 0x11 // Protocol not supported
#define SML_ERR_A_COMM_INVALID_PARM     HS_SML_ERR_A_COMM_ERROR + 0x12 // Parameter error
#define SML_ERR_A_COMM_INVALID_ID       HS_SML_ERR_A_COMM_ERROR + 0x13 // Invalid Communication/Service ID
#define SML_ERR_A_COMM_MEMORY           HS_SML_ERR_A_COMM_ERROR + 0x14 // Memory allocation error
#define SML_ERR_A_COMM_IN_USE           HS_SML_ERR_A_COMM_ERROR + 0x15 // Too many open communication instances
#define SML_ERR_A_COMM_INVALID_STATE    HS_SML_ERR_A_COMM_ERROR + 0x16 // Invalid communication state
#define SML_ERR_A_COMM_NO_TRANSPORTS    HS_SML_ERR_A_COMM_ERROR + 0x17 // No transports are available
#endif


#endif

