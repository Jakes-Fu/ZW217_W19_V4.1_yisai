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
文件名称:PIMsmlcore.h
文件描述:功能模块核心
文件说明:功能模块的核心内容，关键函数，但并不一定就是
                       真正的实现函数。模块核心的头文件。
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
  #ifndef   HEADER_FILE_SMLCORE
  #define  HEADER_FILE_SMLCORE
  
  /*  包含头文件*/
  #include "PIMsmldef.h"
  #include "PIMwsm.h"

  /*  数据结构声明*/

  /** 
 * structure describing the current status of the global syncml module
 * (holds all global variables within SyncML)
 **/
typedef struct syncml_info_s {
  InstanceInfoPtr_t        instanceListAnchor;// Anchor of the global list of known SyncML instances
  SmlOptionsPtr_t          syncmlOptions;     // Options valid for this SyncML Process
  WsmGlobalsPtr_t          wsmGlobals;        // Workspace global variables
  TokenInfoPtr_t           tokTbl;
} *SyncMLInfoPtr_t, SyncMLInfo_t;

  /*  全局变量声明*/
  extern  SyncMLInfoPtr_t hs_pGlobalAnchor;

  /*  功能函数声明*/
  short hs_smlInit(SmlOptionsPtr_t pOptions);
  short hs_smlInitInstance(SmlCallbacksPtr_t callbacks, SmlInstanceOptionsPtr_t pOptions, void* pUserData, short *pInstanceID);

  short hs_smlStartMessageExt(short id, SmlSyncHdrPtr_t pContent, SmlVersion_t vers);
  short hs_smlStartSync(short id, SmlSyncPtr_t pContent);
  #ifdef HS_ADD_SEND
  short hs_smlAddCmd(short id, SmlAddPtr_t pContent);
  #endif
  short hs_smlDeleteCmd(short id, SmlDeletePtr_t pContent);
  short hs_smlReplaceCmd(short id, SmlReplacePtr_t pContent);
  short hs_smlAlertCmd(short id, SmlAlertPtr_t pContent);
  short hs_smlPutCmd(short id, SmlPutPtr_t pContent);
  short hs_smlGetCmd(short id, SmlGetPtr_t pContent);
  short hs_smlStatusCmd(short id, SmlStatusPtr_t pContent);
  short hs_smlMapCmd(short id, SmlMapPtr_t pContent);
  short hs_smlEndSync(short id);
  short hs_smlSetUserData(short id, void* pUserData);
  short hs_smlProcessData(short id, SmlProcessMode_t mode);
  short hs_smlEndMessage(short id, unsigned char final);
  short hs_smlTerminateInstance (short id);
  short hs_smlTerminate(void);
 
  short hs_smlLockWriteBuffer(short id, unsigned char **pWritePosition, long *freeSize);
  short hs_smlUnlockWriteBuffer(short id, long writtenBytes);
  short hs_smlLockReadBuffer(short id, unsigned char* *pReadPosition, long *usedSize);
  short hs_smlUnlockReadBuffer(short id, long processedBytes);

  short hs_mgrProcessStartMessage(short id, InstanceInfoPtr_t pInstanceInfo);
  short hs_mgrResetWorkspace (short id) ;
  short hs_mgrProcessNextCommand(short id, InstanceInfoPtr_t pInstanceInfo);

#endif
