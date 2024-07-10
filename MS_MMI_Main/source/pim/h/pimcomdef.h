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
�ļ�����:PIMcomdef.h
�ļ�����:�������壬���ڶ���һЩ�������������͡�
�ļ�˵��:�����������ͺ�feature����ͷ�ļ���
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
  #ifndef   HEADER_FILE_COMDEF
  #define  HEADER_FILE_COMDEF
  /*  ����ͷ�ļ�*/
 // #include "Pim.h"

  /*  ���ض�������*/

/* ֧��WBXML���뿪�� */
#define __HS_SML_WBXML__
/* ֧��XML���� ����*/
#define __HS_SML_XML__

/* ֧�� Sub DTD extensions */
#define __HS_USE_EXTENSIONS__

/* ʹ��Metainformation DTD  */
#define __HS_USE_METINF__
/* ʹ�� Device Info DTD  */
#define __HS_USE_DEVINF__

/*֧��syncml1.1����*/
#define __HS_SML_1_1__

/*֧��syncml1.2����*/
//#define __SML_1_2__

/*������ط�ʽ��֧��*/
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
  /*  ���ݽṹ����*/
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
  
  /*  ȫ�ֱ�������*/

  /*  ���ܺ�������*/

#endif
