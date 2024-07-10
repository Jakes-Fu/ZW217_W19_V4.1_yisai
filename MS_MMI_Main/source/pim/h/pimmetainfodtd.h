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
文件名称:PIMmetainfodtd.h
文件描述:meta info dtd file
文件说明:meta info dtd description file as header file
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
  #ifndef   HEADER_FILE_METAINFODTD
  #define  HEADER_FILE_METAINFODTD
  
  /*  包含头文件*/
  #include "PIMcomdef.h"

  /*  数据结构声明*/
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
  
  /*  全局变量声明*/

  /*  功能函数声明*/

 #endif
