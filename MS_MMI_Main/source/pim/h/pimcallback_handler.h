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
文件名称:PIMcallback_handler.h
文件描述:callback handler
文件说明:回叫函数处理头文件
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
  #ifndef   HEADER_FILE_CALLBACK_HANDLER
  #define  HEADER_FILE_CALLBACK_HANDLER
  
  /*  包含头文件*/
  #include  "std.h"
  #include "PIMmetainfodtd.h"
  #include "PIMdevinfodtd.h"
  #include "PIMsmldtd.h"
  
  #include  "sci_types.h"
  #include "mmipim.h"

  /*  数据结构声明*/
  #ifndef NULL                        // a NULL pointer
  #define NULL (void*) 0
  #endif

  typedef struct status_element_type
  {
        SmlStatusPtr_t status;
	 struct status_element_type* next;
  }status_element_type;
  
  typedef  struct
  {
         status_element_type* queue;
	  long totalnumber;
	  SmlPcdataPtr_t  msgref;
  }status_cmd_queue_type;
  
  /*  全局变量声明*/

  /*  功能函数声明*/
  

  short hs_smlFreeProtoElement(void * pProtoElement); // chenxiang_memory 20080223
  
  void hs_smlFreePcdata(const char * file,  int line, SmlPcdataPtr_t pPcdata);

  void hs_smlFreeSourceTargetPtr(SmlSourcePtr_t pSourceTarget);

  void hs_smlFreeCredPtr(SmlCredPtr_t pCred);

  void hs_smlFreeSyncHdr(SmlSyncHdrPtr_t pSyncHdr);

  void hs_smlFreeSync(SmlSyncPtr_t pSync);

  void hs_smlFreeItemPtr(SmlItemPtr_t pItem);

  void hs_smlFreeItemList(SmlItemListPtr_t pItemList);

  void hs_smlFreeGeneric(SmlGenericCmdPtr_t pGenericCmd);

  void hs_smlFreeAlert(SmlAlertPtr_t pAlert);

  void hs_smlFreeStatus(SmlStatusPtr_t pStatus);

  void hs_smlFreeChalPtr(SmlChalPtr_t pChal);

  void hs_smlFreeGetPut(SmlPutPtr_t pGetPut);

  #if (defined HS_ATOMIC_RECEIVE || defined HS_SEQUENCE_RECEIVE)  
  void hs_smlFreeAtomic(SmlAtomicPtr_t pAtomic);
  #endif

  #if (defined HS_EXEC_SEND || defined HS_EXEC_RECEIVE)
  void hs_smlFreeExec(SmlExecPtr_t pExec);
  #endif

  #ifdef HS_MAP_RECEIVE
  void hs_smlFreeMap(SmlMapPtr_t pMap);
  void hs_smlFreeMapItemList(SmlMapItemListPtr_t pMapItemList);
  void hs_smlFreeMapItemPtr(SmlMapItemPtr_t pMapItem);
  void hs_smlFreeNewMapItemList(SmlNewMapItemListPtr_t pMapItemList);
  #endif

  #ifdef HS_RESULT_RECEIVE
  void hs_smlFreeResults(SmlResultsPtr_t pResults);
  #endif
  
  SmlPcdataPtr_t hs_concatPCData(SmlPcdataPtr_t pDat1, const SmlPcdataPtr_t pDat2);

 #ifdef __HS_USE_METINF__
   void hs_smlFreeMetinfAnchor(SmlMetInfAnchorPtr_t data);
   void hs_smlFreeMetinfMem(SmlMetInfMemPtr_t data);
   void hs_smlFreeMetinfMetinf(SmlMetInfMetInfPtr_t data);
#endif

  SmlPcdataPtr_t hs_smlString2Pcdata(const char * file,  int line, char* str); // chenxiang_memory 20080225

  SmlPcdataPtr_t hs_smlmeta2extPcdata(const char * file,  int line, SmlMetInfMetInfPtr_t meta); // chenxiang_memory 20080225

  SmlPcdataPtr_t hs_smldevinfo2extPcdata(const char * file,  int line, SmlDevInfDevInfPtr_t devinfo); // chenxiang_memory 20080225
  
  char* hs_smlPcdata2String(const char * file,  int line,  SmlPcdataPtr_t pcdata ); // chenxiang_memory 20080225

  char* hs_smlunsignedInt2String(const char * file,  int line, unsigned int  integer); // chenxiang_memory 20080225

  unsigned int hs_smlString2unsignedInt(char* str);

/***********************************************************/
  
  short hs_myHandleStartMessage(short id, void* userData,
                           SmlSyncHdrPtr_t pSyncHdr);

  short hs_myHandleEndMessage(short id, void* userData, unsigned char final);


  short hs_myHandleStartSync(short id, void* userData,
                        SmlSyncPtr_t pSync);

  short hs_myHandleEndSync(short id, void* userData);

  #ifdef HS_ATOMIC_RECEIVE  /* these callbacks are NOT included in the Toolkit lite version */
  short hs_myHandlestartAtomic(short id, void * userData,SmlAtomicPtr_t pContent);

  short hs_myHandleendAtomic(short id, void * userData);
  #endif

  #ifdef HS_SEQUENCE_RECEIVE
  short hs_myHandlestartSequence(short id, void * userData,SmlSequencePtr_t pContent);

  short hs_myHandleendSequence(short id, void * userData);
  #endif

  short hs_myHandleAdd(short id, void* userData, SmlAddPtr_t pContent);

  short hs_myHandlealert(short id, void* userData, SmlAlertPtr_t pContent);

  short hs_myHandledelete(short id, void* userData, SmlDeletePtr_t pContent);

  short hs_myHandleget(short id, void* userData, SmlGetPtr_t pContent);

  short hs_myHandleput(short id, void* userData, SmlPutPtr_t pContent);

  #ifdef HS_MAP_RECEIVE
  short hs_myHandlemap(short id, void* userData, SmlMapPtr_t pContent);
  #endif

  #ifdef HS_RESULT_RECEIVE
  short hs_myHandleresults(short id, void* userData, SmlResultsPtr_t pContent);
  #endif

  short hs_myHandlestatus(short id, void* userData, SmlStatusPtr_t pContent);

  short hs_myHandlereplace(short id, void* userData, SmlReplacePtr_t pContent);

  #ifdef HS_COPY_RECEIVE  /* these callbacks are NOT included in the Toolkit lite version */
  short hs_myHandlecopy(short id, void* userData, SmlCopyPtr_t param);
  #endif

  #ifdef HS_EXEC_RECEIVE
  short hs_myHandleexec(short id, void* userData, SmlExecPtr_t pContent);
  #endif

  #ifdef HS_SEARCH_RECEIVE
  short hs_myHandlesearch(short id, void* userData, SmlSearchPtr_t pContent);
  #endif

  short hs_myHandleFinal(void);

  short hs_myHandlehandleError(short id, void* userData);

  short hs_myHandletransmitChunk(short id, void* userData);

  short hs_myAddstatustoCQ(SmlStatusPtr_t status);

  short hs_myFreestatusofCQ(void);

  SmlItemListPtr_t hs_myGetAddedPBItems(short syncway); // chenxiang 20071124

  unsigned int hs_myGetAddedPBItemsCount(void);

    unsigned int hs_myGetAllPBItemsCount(void); //wangxiaolin 2007.12.06

  SmlItemListPtr_t hs_myGetReplacedPBItems(void); // chenxiang 20071124

  unsigned int hs_myGetReplacedPBItemsCount(void);

  SmlItemListPtr_t hs_myGetDeletedPBItems(void);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
  SmlItemListPtr_t hs_myGetAddedVCALItems(short syncway); 
  SmlItemListPtr_t hs_myGetReplacedVCALItems(void);
  SmlItemListPtr_t hs_myGetDeletedVCALItems(void);
#endif /* HS_PIM_VCAL */
  
#ifdef HS_PIM_CACHE_MAP //  wangxiaolin 20090804

BOOLEAN hs_myLoadCacheMap(void);

void hs_myClearCacheMap(void);

void hs_mySaveCacheMap(void);

BOOLEAN hs_myGetFlagOfSendingCacheMap(void);

void hs_mySetFlagOfSendingCacheMap(BOOLEAN flag);
#endif /* HS_PIM_CACHE_MAP */

  
  SmlMapItemListPtr_t hs_ConvertNewMapListToMapList(SmlNewMapItemListPtr_t new_map_list);
  SmlNewMapItemListPtr_t hs_myGetMapedItems(void);


  SmlPcdataPtr_t hs_myGetMSFreeMemery(void);

  SmlPcdataPtr_t hs_myGetPhoneBookFreeIDs(void);

  SmlPcdataPtr_t hs_myGetNumberofChanges(void);
  
 SmlPcdataPtr_t hs_myGetNumberofAllItem(void);


/*****************************************************************/
 //these are for syncml core test on windows. liugang make it disabled.2005-11-7.
      //make these changed to save logs on efs in debug model.  liugang 2006-5-29.
  void hs_myPrintxmlorwbxml(char* filename,short id);

  void hs_myhttplogs(char* buffer, uint32  length);
  void hs_myPrintbuffer(char *pbuffer,int buf_lenth);

  #endif
