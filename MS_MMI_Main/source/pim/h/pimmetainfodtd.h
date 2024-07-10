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
�ļ�����:PIMmetainfodtd.h
�ļ�����:meta info dtd file
�ļ�˵��:meta info dtd description file as header file
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
  #ifndef   HEADER_FILE_METAINFODTD
  #define  HEADER_FILE_METAINFODTD
  
  /*  ����ͷ�ļ�*/
  #include "PIMcomdef.h"

  /*  ���ݽṹ����*/
  #ifdef __HS_USE_METINF__
  
    typedef struct sml_metinf_anchor_s {
	SmlPcdataPtr_t	last; /* optional */
	SmlPcdataPtr_t	next;
} *SmlMetInfAnchorPtr_t, SmlMetInfAnchor_t;

  typedef struct sml_metinf_mem_s {
	SmlPcdataPtr_t	shared; /* optional */
	SmlPcdataPtr_t	free;
	SmlPcdataPtr_t	freeid;
} *SmlMetInfMemPtr_t, SmlMetInfMem_t;

  typedef struct sml_metinf_metinf_s {
	SmlPcdataPtr_t	format;    /* opt. */
	SmlPcdataPtr_t	type;      /* opt. */
	SmlPcdataPtr_t	mark;      /* opt. */
	SmlPcdataPtr_t	size;      /* opt. */
	SmlPcdataPtr_t  nextnonce; /* opt. */
	SmlPcdataPtr_t	version; 
	SmlPcdataPtr_t	maxmsgsize; /* optional */
	/* SCTSTK - 18/03/2002, S.H. 2002-04-05 : SyncML 1.1 */
	SmlPcdataPtr_t  maxobjsize; /* optional */
	SmlMetInfMemPtr_t	mem;	/* optional */
	SmlPcdataListPtr_t emi; /* optional */
	SmlMetInfAnchorPtr_t anchor; /* opt. */
} *SmlMetInfMetInfPtr_t, SmlMetInfMetInf_t;
  
  #endif
  
  /*  ȫ�ֱ�������*/

  /*  ���ܺ�������*/

 #endif
