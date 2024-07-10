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
�ļ�����:PIMdecodexml.h
�ļ�����:xml�ĵ����빦��ͷ�ļ�
�ļ�˵��:��codec.c���ã�����xml�ĵ��Ľ��롣
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
  #ifndef   HEADER_FILE_DECODE_XML
  #define  HEADER_FILE_DECODE_XML
  /*  ����ͷ�ļ�*/
#include "PIMcodec.h"
  /*  ���ݽṹ����*/
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
  
  /*  ȫ�ֱ�������*/

  /*  ���ܺ�������*/
short hs_xltDecXmlInit(unsigned char* pBufEnd, unsigned char* *ppBufStart,
      XltDecScannerPtr_t *ppScanner);//liugang delete the const symbol for reduce compiling warnings! 
  #endif
