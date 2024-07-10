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
文件名称:PIMdecodexml.h
文件描述:xml文档解码功能头文件
文件说明:由codec.c调用，负责xml文档的解码。
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
  #ifndef   HEADER_FILE_DECODE_XML
  #define  HEADER_FILE_DECODE_XML
  /*  包含头文件*/
#include "PIMcodec.h"
  /*  数据结构声明*/
/**
 * Private Interface for the XML scanner.
 */
typedef struct xmlScannerPriv_s xmlScannerPriv_t, *xmlScannerPrivPtr_t;
struct xmlScannerPriv_s
{
    /* public */
    short (*nextTok)(const char* ,  int , XltDecScannerPtr_t); // chenxiang_memory 20080313
    short (*destroy)(XltDecScannerPtr_t);
    short (*pushTok)(XltDecScannerPtr_t);
    void  (*setBuf)(XltDecScannerPtr_t pScanner, unsigned char* pBufStart, unsigned char* pBufEnd); //liugang delete the const symbol for reduce compiling warnings!
    unsigned char* (*getPos)(XltDecScannerPtr_t pScanner); 

    XltDecTokenPtr_t curtok;       /* current token */
    long charset;                /* 0 */
    char* charsetStr;           /* character set */
    long pubID;                  /* 0 */
    char* pubIDStr;             /* document public identifier */
    SmlPcdataExtension_t ext;      /* which is the actual open namespace ? */
    SmlPcdataExtension_t prev_ext; /* which is the previous open namespace ? */
    XltTagID_t ext_tag;            /* which tag started the actual namespace ? */
    XltTagID_t prev_ext_tag;       /* which tag started the previous open namespace ? */
    char*   nsprefix;           /* prefix used for active namespace (if any) */
    unsigned char     nsprelen;           /* how long is the prefix ? (to save hs_smlLibStrlen calls) */
    unsigned int  finished;

    /* private */
    unsigned char* pos;                  /* current position */
    unsigned char* bufend;               /* end of buffer */
};
  
  /*  全局变量声明*/

  /*  功能函数声明*/
short hs_xltDecXmlInit(unsigned char* pBufEnd, unsigned char* *ppBufStart,
      XltDecScannerPtr_t *ppScanner);//liugang delete the const symbol for reduce compiling warnings! 
  #endif
