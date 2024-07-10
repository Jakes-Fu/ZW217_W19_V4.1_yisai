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
文件名称:PIMdevinfodtd.h
文件描述:device info dtd file
文件说明:device infomation data file as header file.
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
  #ifndef   HEADER_FILE_DEVINFODTD
  #define  HEADER_FILE_DEVINFODTD
  /*  包含头文件*/
  #include"PIMcomdef.h"

  /*  数据结构声明*/
  #ifdef __HS_USE_DEVINF__
  
typedef struct sml_devinf_synccap_s {
    SmlPcdataListPtr_t  synctype;
} *SmlDevInfSyncCapPtr_t, SmlDevInfSyncCap_t;

typedef struct sml_devinf_dsmem_s {
    unsigned int  flags; /* %%% luz:2003-04-28, mad flag, was PCData (completely wrong) */
    SmlPcdataPtr_t  maxmem; /* optional */
    SmlPcdataPtr_t  maxid;  /* optional */
} *SmlDevInfDSMemPtr_t, SmlDevInfDSMem_t;

typedef struct sml_devinf_ext_s {
    SmlPcdataPtr_t      xnam; 
    SmlPcdataListPtr_t  xval; /* optional */
} *SmlDevInfExtPtr_t, SmlDevInfExt_t;

typedef struct sml_devinf_xmit_s {
    SmlPcdataPtr_t  cttype;
    SmlPcdataPtr_t  verct;
} *SmlDevInfXmitPtr_t, SmlDevInfXmit_t;

typedef struct sml_devinf_ctdata_s {
    SmlPcdataPtr_t  name;
    SmlPcdataPtr_t  dname; /* optional, display name */

    SmlPcdataListPtr_t  valenum;
    SmlPcdataPtr_t  datatype;
    SmlPcdataPtr_t  size;
} *SmlDevInfCTDataPtr_t, SmlDevInfCTData_t;

typedef struct sml_devinf_xmitlist_s {
    SmlDevInfXmitPtr_t      data;
    struct sml_devinf_xmitlist_s  *next;
} *SmlDevInfXmitListPtr_t, SmlDevInfXmitList_t;

typedef struct sml_devinf_datastore_s {
    SmlPcdataPtr_t          sourceref;
    SmlPcdataPtr_t          displayname;   /* optional */
    SmlPcdataPtr_t          maxguidsize;   /* optional */
    SmlDevInfXmitPtr_t      rxpref;
    SmlDevInfXmitListPtr_t  rx;            /* optional */
    SmlDevInfXmitPtr_t      txpref;
    SmlDevInfXmitListPtr_t  tx;            /* optional */
    SmlDevInfDSMemPtr_t     dsmem;         /* optional */
    SmlDevInfSyncCapPtr_t   synccap;
} *SmlDevInfDatastorePtr_t, SmlDevInfDatastore_t;

typedef struct sml_devinf_datastorelist_s {
    SmlDevInfDatastorePtr_t data;
    struct sml_devinf_datastorelist_s *next;
} *SmlDevInfDatastoreListPtr_t, SmlDevInfDatastoreList_t;

typedef struct sml_devinf_ctdatalist_s {
    SmlDevInfCTDataPtr_t       data;
    struct sml_devinf_ctdatalist_s *next;
} *SmlDevInfCTDataListPtr_t, SmlDevInfCTDataList_t;

typedef struct sml_devinf_ctdataprop_s {
    SmlDevInfCTDataPtr_t    prop;
    SmlDevInfCTDataListPtr_t param;
} *SmlDevInfCTDataPropPtr_t, SmlDevInfCTDataProp_t;

typedef struct sml_devinf_ctdataproplist_s {
    SmlDevInfCTDataPropPtr_t    data;
    struct sml_devinf_ctdataproplist_s *next;
} *SmlDevInfCTDataPropListPtr_t, SmlDevInfCTDataPropList_t;

typedef struct sml_devinf_ctcap_s {
    SmlPcdataPtr_t                cttype;
    SmlDevInfCTDataPropListPtr_t  prop; 
} *SmlDevInfCTCapPtr_t, SmlDevInfCTCap_t;

typedef struct sml_devinf_extlist_s {
    SmlDevInfExtPtr_t   data;
    struct sml_devinf_extlist_s *next;
} *SmlDevInfExtListPtr_t, SmlDevInfExtList_t;

typedef struct sml_devinf_ctcaplist_s {
    SmlDevInfCTCapPtr_t data;
    struct sml_devinf_ctcaplist_s *next;
} *SmlDevInfCtcapListPtr_t, SmlDevInfCtcapList_t;

typedef struct sml_devinf_devinf_s {
  SmlPcdataPtr_t      verdtd;
  SmlPcdataPtr_t      man; /* optional */
  SmlPcdataPtr_t      mod; /* optional */
  SmlPcdataPtr_t      oem; /* optional */
  SmlPcdataPtr_t      fwv; /* optional */
  SmlPcdataPtr_t      swv; /* optional */
  SmlPcdataPtr_t      hwv; /* optional */
  SmlPcdataPtr_t      devid;
  SmlPcdataPtr_t      devtyp;
  SmlDevInfDatastoreListPtr_t datastore;
  SmlDevInfCtcapListPtr_t     ctcap;
  SmlDevInfExtListPtr_t       ext;
	/* SCTSTK - 18/03/2002, S.H. 2002-04-05 : SyncML 1.1 */
  unsigned int				flags;
} *SmlDevInfDevInfPtr_t, SmlDevInfDevInf_t;
  #endif
  
  /*  全局变量声明*/

  /*  功能函数声明*/


 #endif
