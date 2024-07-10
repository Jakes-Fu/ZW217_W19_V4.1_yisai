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
文件名称:PIMdecodewbxml.h
文件描述:wbxml文档解码功能头文件
文件说明:由codec.c调用，负责wbxml文档的解码。
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
  #ifndef   HEADER_FILE_DECODE_WBXML
  #define  HEADER_FILE_DECODE_WBXML
  /*  包含头文件*/
  
#include "PIMcodec.h"

  /*  数据结构声明*/
  
  /**
 * Private Interface for the WBXML scanner.
 *
 * The private scanner interface contains some additional member attributes
 * that are not listed in the public interface, e.g. a copy of the string
 * table and some other items that do not need to be known outside the
 * scanner module.
 */
typedef struct wbxmlScannerPriv_s wbxmlScannerPriv_t, *wbxmlScannerPrivPtr_t;
struct wbxmlScannerPriv_s
{
    /* public methods */
    short (*nextTok)(const char* ,  int , XltDecScannerPtr_t); // chenxiang_memory 20080313
    short (*destroy)(XltDecScannerPtr_t);
    short (*pushTok)(XltDecScannerPtr_t);
    void (*setBuf)(XltDecScannerPtr_t pScanner, unsigned char* pBufStart, unsigned char* pBufEnd); //liugang delete the const symbol for reduce compiling warnings! 
    unsigned char* (*getPos)(XltDecScannerPtr_t pScanner); 

    /* public attributes */
    XltDecTokenPtr_t curtok;       /* current token */
    long charset;                /* character set as specified in the
                                      WBXML header */
    char* charsetStr;           /* NULL */
    long pubID;                  /* document public identifier as
                                      specified in the WBXML header */
    char* pubIDStr;             /* pubID as a string - valid only when
                                      pubID == 0 */
    unsigned int finished;               /* set when end of buffer is reached */

    /* private attributes */
    unsigned char* pos;                  /* current buffer position */
    unsigned char* bufend;               /* end of buffer */ 
    long pubIDIdx;               /* strtbl index of the string
                                      version of the pubID - valid only
                                      when pubID == 0 */

    XltUtilStackPtr_t tagstack;    /* stack of open start tags */

    unsigned char* strtbl;                /* copy of the string table */
    long strtbllen;               /* length of the string table */

    unsigned char state;                   /* tag state or attribute state */
    SmlPcdataExtension_t cptag;     /* current codepage for tags */
    unsigned char cpattr;                  /* current codepage for attributes */
    SmlPcdataExtension_t activeExt; /* the active Sub DTD */
};

  
  /*  全局变量声明*/

  /* WBXML version that this parser knows */
  #define _MAJOR_VERSION	1
  #define _MINOR_VERSION	2

  #define TAG_STATE 0
  #define ATTRIBUTE_STATE 1

  
  /*  功能函数声明*/
  short hs_xltDecWbxmlInit(unsigned char* pBufEnd, unsigned char* *ppBufPos,
        XltDecScannerPtr_t *ppScanner); //liugang delete the const symbol for reduce compiling warnings! 

 #ifdef __HS_USE_EXTENSIONS__
/* prototype for function in xltdecwbxml.c */
void hs_subdtdDecodeWbxml(XltDecoderPtr_t pDecoder,SmlPcdataPtr_t *ppPcdata);
#endif

  #endif
