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
文件名称:PIMwsm.c
文件描述:wsm执行文件
文件说明:syncml workspace manager，用来做功能工作区，主要完成
                       模块功能的操作，内存分配和管理，生成发送、
                       接收数据等等。
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

  /*  包含头文件*/
#include "mmi_pim_trc.h"
#ifdef WIN32
  #include "std_header.h"
#endif
  #define _PIMWSM_C_
  
  #include "string.h"
  #include "../h/PIMlibmem.h"
  #include "../h/PIMsmlcore.h"
  #include "../h/PIMsmlerr.h"

  /*  数据结构声明*/

/** WSM buffer flags */
#define  WSM_VALID_F     (unsigned char) 0x01
#define  WSM_LOCKED_F    (unsigned char) 0x02

#define  WSM_MEMH_UNUSED   -1

  /* defines for convient use of global anchor */
#define wsmRet          hs_pGlobalAnchor->wsmGlobals->wsmRet
#define initWasCalled   hs_pGlobalAnchor->wsmGlobals->initWasCalled
#define maxWsmAvailMem  hs_pGlobalAnchor->syncmlOptions->maxWorkspaceAvailMem
#define wsmBuf          hs_pGlobalAnchor->wsmGlobals->wsmBuf
#define wsmIndex        hs_pGlobalAnchor->wsmGlobals->wsmIndex
#define wsmSm                  hs_pGlobalAnchor->wsmGlobals->wsmSm

#define freeDataStructs() hs_smlLibFree((hs_pGlobalAnchor->wsmGlobals))


//#define  WSM_MEMH_UNUSED   -1

  
  /*  全局变量声明*/
 

  /*  功能函数声明*/

   static short hs_lookup(short memH);

/***********************static basic functions***************************/
void hs_createDataStructs() {
  if ( hs_pGlobalAnchor->wsmGlobals == NULL ) {
    if ( (hs_pGlobalAnchor->wsmGlobals=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(WsmGlobals_t))) == 0 ) {
      return;
    }
    hs_smlLibMemset(hs_pGlobalAnchor->wsmGlobals, 0, sizeof(WsmGlobals_t));
    wsmRet = 0;
    initWasCalled = 0;
    wsmIndex = 0;
    wsmSm = NULL;
  }
}


/**
 * FUNCTION: 
 * Searches an element with the given InstanceID in the list  
 *
 * IN:        InstanceID_t              
 *            ID of the InstanceInfo structure to be retrieved
 *
 * RETURN:    InstanceInfoPtr_t         
 *            Pointer to the InstanceInfo structure with the given ID
 *            NULL, if no InstanceInfo with the given ID has been found
 */
InstanceInfoPtr_t hs_findInfo(short id)
{

  InstanceInfoPtr_t _pTmp;                // A helper pointer

  /* go through the list until end */
  for (_pTmp=hs_pGlobalAnchor->instanceListAnchor; _pTmp!=NULL; _pTmp=_pTmp->nextInfo) 
  {
    if (_pTmp->id == id) { 
      return _pTmp;                       // STOP, we've found the info, return!   
    }
  }
  return NULL;                            // Info was not found, return NULL
}

/**
 * FUNCTION: 
 * Adds a new element to the list
 *
 * IN:        InstanceInfoPtr_t        
 *            pointer to the structure to be be added to list
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if element was added successfully
 */
short hs_addInfo(InstanceInfoPtr_t pInfo)
{

  if (pInfo!=NULL) 
    {
          InstanceInfoPtr_t _pTmp;
          
          /* Remember old beginning of the list */
          _pTmp=hs_pGlobalAnchor->instanceListAnchor;  
        
          /* insert element immediately after anchor */
          hs_pGlobalAnchor->instanceListAnchor=pInfo;      // anchor of list points now to new info element
          pInfo->nextInfo=_pTmp;                // Next info element is the prior first one.
          return HS_SML_ERR_OK;  

    } else {                              // Invalid InstanceInfo pointer was used (NULL)

          return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;
    }

}

/**
 * FUNCTION: 
 * Removes an element with the given InstanceID from the list  
 *
 * IN:        InstanceID_t              
 *            ID of the InstanceInfo structure to be removed
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if element was removed successfully
 */
short hs_removeInfo(short id)
{

  InstanceInfoPtr_t _pTmp;               // A helper pointer
  InstanceInfoPtr_t _pRemember;          // A helper pointer


  /* Remember current anchor */
  _pRemember=hs_pGlobalAnchor->instanceListAnchor;
  
  /* special check, if list is empty */
  if (_pRemember==NULL ) {
     //syncml_task_message:"MMIPIM chenxiang hs_removeInfo 1"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_168_112_2_18_3_26_1_284,(uint8*)"");
    return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;
  }

  /* special check, if first element should be removed */
  if (_pRemember->id == id) 
    {
    // It's the first element, update anchor!
    hs_pGlobalAnchor->instanceListAnchor=_pRemember->nextInfo;  
     //syncml_task_message:"MMIPIM chenxiang hs_removeInfo 2"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_177_112_2_18_3_26_1_285,(uint8*)"");
    return HS_SML_ERR_OK;                    // return
    }


  /* go through the list until end */
  for (_pTmp=_pRemember->nextInfo; _pTmp!=NULL; _pTmp=_pTmp->nextInfo) 
    {
    if (_pTmp->id == id)                  // STOP, we've found the info   
      {                                   
      _pRemember->nextInfo=_pTmp->nextInfo;
     //syncml_task_message:"MMIPIM chenxiang hs_removeInfo 3"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_188_112_2_18_3_26_1_286,(uint8*)"");
      return HS_SML_ERR_OK;                  // return
      
      } else {
      
      _pRemember=_pTmp;                   // update helper pointer
      }
    }
  
     //syncml_task_message:"MMIPIM chenxiang hs_removeInfo 4"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_197_112_2_18_3_26_1_287,(uint8*)"");
  return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;  // Info wasn't found

}


/**
 * Get next free buffer entry.
 * Returns index of next free entry, or -1 if buffer table is full.
 */
short hs_getNextFreeEntry() {
  short i;

  for ( i=0; i < HS_MAX_WSM_BUFFERS; ++i )
    if ( wsmBuf[i].memH == WSM_MEMH_UNUSED )
      return i;

  return -1;
}


unsigned char hs_isMemAvailable(long memToAlloc) {
  int i;
  long actMem = memToAlloc;
  if ( maxWsmAvailMem == 0 )
    return 1;  // no memsize restrictions
  for (i=0; i < HS_MAX_WSM_BUFFERS; ++i) {
    if ( wsmBuf[i].memH != WSM_MEMH_UNUSED )
      actMem += wsmBuf[i].size;
  }
  return ((unsigned char)(actMem <= maxWsmAvailMem));
}


/**
 * Reset values in buffer table for entry memH.
 * If memH doesn't exist create an entry.
 * Return index to memH in buffer table,
 * or -1 if table is full
 */
short hs_resetBufferGlobals(short memH) {
  if ( (wsmIndex = hs_lookup(memH)) < 0 ) {
    // create new one
    if ( (wsmIndex = hs_getNextFreeEntry()) < 0 )
      return -1;  // buffer table full
    wsmBuf[wsmIndex].memH = memH;
  } else
    // use existing one, which has to be reset prior usage
    hs_smlLibFree(wsmBuf[wsmIndex].bufName);   // free mem

  // reset the values
  wsmBuf[wsmIndex].pFirstFree = NULL;
  wsmBuf[wsmIndex].pFirstData = NULL;
  wsmBuf[wsmIndex].size      = 0;
  wsmBuf[wsmIndex].usedBytes = 0;
  wsmBuf[wsmIndex].flags     = WSM_VALID_F;
  wsmBuf[wsmIndex].bufName   = NULL;

  return wsmIndex;
}

/**
 * Get buffer table index for memH.
 * Returns -1 if memH not found.
 */
static short hs_lookup(short memH) {
  short i;

  // first check cache
  if ( wsmBuf[wsmIndex].memH == memH )
    return wsmIndex;

  // search through buffer
  for ( i=0; (i < HS_MAX_WSM_BUFFERS) && (wsmBuf[i].memH != memH); ++i )
    ;
  if ( i < HS_MAX_WSM_BUFFERS ) {
    wsmIndex = i;
    return i;
  }
  else {
    return -1;     // memH not found  
  }
}

/**
 * Find memory handle corresponding to name. 
 * Return WSM_MEMH_UNUSED, if name not found in wsmBuf.
 */
short hs_nameToHandle(char* name) {
  int i;

  // first check cache
  if ( (wsmBuf[wsmIndex].bufName != NULL) &&
       (hs_smlLibStrcmp(wsmBuf[wsmIndex].bufName, name) == 0) )
    return wsmBuf[wsmIndex].memH;

  // search through buffer
  for ( i=0; ((i < HS_MAX_WSM_BUFFERS) && 
	      (wsmBuf[i].bufName == NULL ? 1 :
	       hs_smlLibStrcmp(wsmBuf[i].bufName, name) != 0)); ++i )
    ;
  if ( i < HS_MAX_WSM_BUFFERS ) 
    return wsmBuf[i].memH;
  else {
    return WSM_MEMH_UNUSED;     // name not found  
  }
}


unsigned char hs_isValidMemH(short memH) 
{
    short wsm_index=hs_lookup(memH);
    if(wsm_index<HS_MAX_WSM_BUFFERS)
    {
          return (unsigned char) (wsmBuf[wsm_index].flags & WSM_VALID_F);

    }
    else
    {
        return 0;
    }
}

unsigned char hs_isLockedMemH(short memH) 
{
    short wsm_index=hs_lookup(memH);
    if(wsm_index<HS_MAX_WSM_BUFFERS)
    {
          return (unsigned char) (wsmBuf[wsm_index].flags & WSM_LOCKED_F);

    }
    else
    {
        return 0;
    }
}

/**
 * search for buffer with memHandle memH and return pointer to it in p.
 * return == 0 if not found; 1 if found
 */
// luz %%% NOTE: called only from routines which lock the toolkit already,
//               no separate lock required here
unsigned char hs_locateH(short memH, smWinList_t **p) {
  *p = wsmSm;
  while ( (*p != NULL) && ((*p)->memH != memH) )  {
    *p = (*p)->next;
  }
  if ( *p == NULL ) 
    return 0;
  else 
    return 1;
}
  

/**
 * Delete memory handle from buffer.
 * Return -1, if handle not found.
 */
short hs_deleteBufferHandle(short memH) {
  if ( (wsmIndex = hs_lookup(memH)) < 0 )
    return -1;  // handle not found

  // reset the values
  wsmBuf[wsmIndex].memH = WSM_MEMH_UNUSED;
  wsmBuf[wsmIndex].pFirstFree = NULL;
  wsmBuf[wsmIndex].pFirstData = NULL;
  wsmBuf[wsmIndex].size      = 0;
  wsmBuf[wsmIndex].usedBytes = 0;
  wsmBuf[wsmIndex].flags     = ((unsigned char) ~WSM_VALID_F);
  hs_smlLibFree(wsmBuf[wsmIndex].bufName);   // free mem
  wsmBuf[wsmIndex].bufName   = NULL;

  return 0;
}

/************************sm functions!*****************************/
/**
 * FUNCTION: hs_smCreate
 *
 * Creates a new memory block with name memName and size memSize.
 *
 * PRE-Condition:   OS does not know memName; memSize > 0
 *
 * POST-Condition:  memName exists with size memSize; 
 *                  memH refers to new memory block.
 *
 * IN:      memName
 *          Name of new memory block
 * IN:      memSize
 *          Size of new memory block
 * 
 * OUT:     memH
 *          Handle to new memory block
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_USAGE, if memName is already known to the OS
 *          HS_SML_ERR_INVALID_SIZE, if memSize <= 0
 *          HS_SML_ERR_NOT_ENOUGH_SPACE, if available memory < memSize
 *
 * @see  hs_smDestroy
 */
short hs_smCreate (char* memName, long memSize, short *memH) {
  smWinList_t *pEle;     // pointer to new buffer

  if ( memSize <= 0 ) {
       syncml_task_message(("MMIPIM chenxiang hs_smCreate -- 1")); // chenxiang_20080304
    return HS_SML_ERR_INVALID_SIZE;
  }
  if ( hs_locateEle(memName, &pEle) ) {
       syncml_task_message(("MMIPIM chenxiang hs_smCreate -- 2")); // chenxiang_20080304
    return HS_SML_ERR_WRONG_USAGE;
  }
  
  // create new element in buffer list
  if ( ! hs_newListEle(memName, &pEle, memH) ) {
       syncml_task_message(("MMIPIM chenxiang hs_smCreate -- 3")); // chenxiang_20080304
    return HS_SML_ERR_NOT_ENOUGH_SPACE;
  }
    
  // create memory
  if(pEle->winH)  //add by wangxiaolin 2009.08.13 
  {
      hs_smlLibFree(pEle);
      syncml_task_message(("MMIPIM wangxiaolin hs_smCreate,the memory is not pnull,if malloc onece ,will lead memory leak!!!")); // chenxiang_20080304

  }
  if ( (pEle->winH=hs_smlLibMalloc(__FILE__, __LINE__, memSize)) == 0 )
  {
    hs_smlLibFree((pEle->memName));
 	  hs_smlLibFree(pEle);
       syncml_task_message(("MMIPIM chenxiang hs_smCreate -- 4")); // chenxiang_20080304
    return HS_SML_ERR_NOT_ENOUGH_SPACE;
  }
  else //add by wangxiaolin  for init memory
  {
     //syncml_message:"MMIPIM wangxiaolin_mem init buffer memory!addr=0x%x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_421_112_2_18_3_26_2_288,(uint8*)"d",pEle->winH);
     hs_smlLibMemset(pEle->winH,0,memSize);
  }
  // set new values
  pEle->locked = 0;
  pEle->memH = *memH;
  pEle->memSize = memSize;
  pEle->next = NULL;

       syncml_task_message(("MMIPIM chenxiang hs_smCreate -- 5")); // chenxiang_20080304
  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smOpen
 *
 * Open connection to memory block with name memName.
 *
 * PRE-Condition:   OS does know memName
 *
 * POST-Condition:  memH refers to memory block memName
 *
 * IN:      memName
 *          Name of memory block to open<BR>
 *          Windows version: Name is ignored
 * 
 * OUT:     memH
 *          Handle to opened memory block
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_PARAM, if memName is unknown
 *
 * @see  hs_smClose
 */
short hs_smOpen (char* memName, short *memH) {
  smWinList_t *pEle;     // pointer to buffer element

  if ( ! hs_locateEle(memName, &pEle) ) {
    return HS_SML_ERR_WRONG_PARAM;
  }

  *memH = pEle->memH;
  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smClose
 *
 * Close link to memory block.
 *
 * PRE-Condition:   memH is a valid memory block handle; memH is unlocked;
 *                  no pointers to records are in use
 *
 * POST-Condition:  memH is not valid anymore
 *
 * IN:      memH
 *          Handle to close
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_USAGE, if memH is locked or unknown
 *
 * @see  hs_smOpen
 */
short hs_smClose (short memH) {
  smWinList_t *pEle;     // pointer to buffer element
  //syncml_task_message:"MMIPIM wangxiaolin  hs_smClose , memH=%d"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_485_112_2_18_3_26_2_289,(uint8*)"d", memH);
  if ( ! hs_locateH(memH, &pEle) ) {
    return HS_SML_ERR_WRONG_USAGE;
  }
  //syncml_task_message:"MMIPIM wangxiaolin_mem  hs_smClose ,free  winH!addr=0x%x "
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_489_112_2_18_3_26_2_290,(uint8*)"d",pEle->winH );

  // reset handle  
  hs_smlLibFree((pEle->winH));
  pEle->memH = 0;
  pEle->locked = 0;
  pEle->memSize = 0;

  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smDestroy
 *
 * Remove memory block memName within OS.
 *
 * PRE-Condition:   memName is a valid memory block name; 
 *                  memory block is not in use (i.e. no handles and 
 *                  pointers to this memory block are in use)
 *
 * POST-Condition:  memName is not a valid memory block name anymore
 *
 * IN:      memName
 *          Name of memory block to remove
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_PARAM, if memName is unknown
 *          HS_SML_ERR_WRONG_USAGE, if memory block is still locked
 *
 * @see  hs_smCreate
 */
short hs_smDestroy (char* memName) {
  smWinList_t *pEle;     // pointer to buffer element

  if ( ! hs_locateEle(memName, &pEle) ) {
    return HS_SML_ERR_WRONG_PARAM;
  }
  if ( pEle->locked ) {
    return HS_SML_ERR_WRONG_USAGE;
  }

  // remove memory buffer
  hs_removeEle(memName);

  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smSetSize
 *
 * Set size of memory block memH to newSize.
 *
 * PRE-Condition:   memH is a valid handle; newSize > 0; 
 *                  memory block is unlocked
 *
 * POST-Condition:  memory block size = newSize
 *
 * IN:      memH
 *          Handle to memory block
 * IN:      newSize
 *          New size of memory block
 * 
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_PARAM, if memH is unknown
 *          HS_SML_ERR_WRONG_USAGE, if memH is locked
 *          HS_SML_ERR_INVALID_SIZE, if newSize <= 0
 *          HS_SML_ERR_NOT_ENOUGH_SPACE, if available memory < newSize
 *
 * @see  smGetSize
 */
short hs_smSetSize (short memH, long newSize) {
  smWinList_t *pEle;     // pointer to buffer element

  if ( ! hs_locateH(memH, &pEle) ) {
    return HS_SML_ERR_WRONG_PARAM;
  }
  if ( pEle->locked ) {
    return HS_SML_ERR_WRONG_USAGE;
  }
  if ( newSize <= 0 ) {
    return HS_SML_ERR_INVALID_SIZE;
  }
  //syncml_task_message:"MMIPIM wangxiaolin_mem  hs_smSetSize ,free  winH!addr=0x%x "
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_571_112_2_18_3_26_2_291,(uint8*)"d",pEle->winH );

  hs_smlLibFree((pEle->winH));
  if ( (pEle->winH=hs_smlLibMalloc(__FILE__, __LINE__, newSize)) == 0 ) {
    return HS_SML_ERR_NOT_ENOUGH_SPACE;
  }
  pEle->memSize = newSize;

  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smLock
 *
 * Map memory block memH to local address space.
 *
 * PRE-Condition:   memH is a valid handle; memory block is not locked
 *
 * POST-Condition:  pMem points to memory block memH; 
 *                  memory block is locked
 *
 * IN:      memH
 *          Handle to memory block
 * 
 * OUT:     pMem
 *          Pointer to memory block memH mapped in local address space
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_PARAM, if memH is unknown
 *          HS_SML_ERR_WRONG_USAGE, if memH was already locked
 *          HS_SML_ERR_UNSPECIFIC, if lock failed
 *
 * @see  hs_smUnlock
 */
short hs_smLock (short memH, unsigned char **pMem) {
  smWinList_t *pEle;     // pointer to buffer element

  if ( ! hs_locateH(memH, &pEle) ) {
    return HS_SML_ERR_WRONG_PARAM;
  }
  if ( pEle->locked ) {
    return HS_SML_ERR_WRONG_USAGE;
  }

  *pMem = (unsigned char*)pEle->winH;
  pEle->locked = 1;

  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smUnlock
 *
 * Free pointer mapped to memH memory block.
 *
 * PRE-Condition:   memH is a valid handle; memory block is locked
 *
 * POST-Condition:  memory block is unlocked
 *
 * IN:      memH
 *          Handle to memory block
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_PARAM, if memH is unknown
 *          HS_SML_ERR_WRONG_USAGE, if memH was already unlocked
 *          HS_SML_ERR_UNSPECIFIC, if unlock failed
 *
 * @see  hs_smLock
 */
short hs_smUnlock (short memH) {
  smWinList_t *pEle;     // pointer to buffer element

  if ( ! hs_locateH(memH, &pEle) ) {
    return HS_SML_ERR_WRONG_PARAM;
  }
  if ( ! pEle->locked ) {
    return HS_SML_ERR_WRONG_USAGE;
  }

  pEle->locked = 0;

  return HS_SML_ERR_OK;
}

/**********************sm static basic functions*******************/

/** create new buffer element and assign name to it
 * return pointer to new element and handle of new element
 */
/* 
 SCTSTK - 16/03/2002 S.H. 2002-04-05 : fixed so that it works even if the sequence of buffer termination
 is not in the reverse order of buffer creation
 */

// luz %%% NOTE: called only from routines which lock the toolkit already,
//               no separate lock required here
unsigned char hs_newListEle(const char *name, 
  smWinList_t **newEle, 
  short *newHandle
)
{
  smWinList_t *p;
  short  i;
  for ( i=0; *newHandle < HS_MAX_WSM_BUFFERS && wsmBuf[i].memH != -1; ++i )
  	{};
  
  if (i == HS_MAX_WSM_BUFFERS) return 0;
  *newHandle=(short)(i+1);
 
  if ( ((*newEle) = hs_smlLibMalloc(__FILE__, __LINE__, sizeof(smWinList_t))) == 0 )
    return 0;  // no more memory
  if ( ((*newEle)->memName = hs_smlLibMalloc(__FILE__, __LINE__, strlen(name)+1)) == 0 ){
    hs_smlLibFree((*newEle));
    return 0;  // no more memory
    }
  memcpy((*newEle)->memName, name, strlen(name));
  (*newEle)->memName[strlen(name)] = '\0';
  if ( wsmSm == 0 )
    wsmSm = *newEle;
  else {
  	p=wsmSm;
	  while ( p->next != NULL) p = p->next;
    p->next = *newEle;
  }
  return 1;
}

/**
 * search for buffer with name eleName and return pointer to it in p.
 * return == 0 if not found; 1 if found
 */
// luz %%% NOTE: called only from routines which lock the toolkit already,
//               no separate lock required here
unsigned char hs_locateEle(const char *eleName, smWinList_t **p) {
  *p = wsmSm;
  while ( (*p != NULL) && (strcmp((*p)->memName, eleName) != 0) )  {
    *p = (*p)->next;
  }
  if ( *p == NULL ) 
    return 0;
  else 
    return 1;
}


/**
 * remove buffer with name eleName from smWinList.
 */
// luz %%% NOTE: called only from routines which lock the toolkit already,
//               no separate lock required here
void hs_removeEle(const char *eleName) {
  smWinList_t *act, *old;

  old = act = wsmSm;
  while ( (act != NULL) && (strcmp(act->memName, eleName) != 0) )  {
    old = act;
    act = act->next;
  }
  if ( act != NULL ) {
    if ( old == act )   // delete first list ele
      wsmSm = act->next;
    else
      old->next = act->next;
    hs_smlLibFree((act->memName));
  	hs_smlLibFree(act);
  }
}

/******************************wsm functions****************************/

  /**
 * FUNCTION: hs_wsmInit
 *
 * Initializes all Workspace Manager related resources.<BR>
 * Should only be called once!
 *
 * PRE-Condition:   This is the first function call to WSM
 *
 * POST-Condition:  All WSM resources are initialized
 *
 * IN:      wsmOpts
 *          WSM options, valid options are:
 *          <UL>
 *          <LI> maxAvailMem<BR>
 *               Maximal amount of memory which wsm can use for the buffers<BR>
 *               0 == no limitation
 *          </UL>
 * 
 * OUT:     wsmH
 *          Handle to new buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_OPTIONS, if wsmOpts is not valid
 *          HS_SML_ERR_NOT_ENOUGH_SPACE, if not enough available memory
 *          HS_SML_ERR_WRONG_USAGE, if hs_wsmInit was already called
 */
short hs_wsmInit (const WsmOptions_t *wsmOpts) {
  int i;

  // create global datastructs
  hs_createDataStructs();

  if (NULL == hs_pGlobalAnchor->wsmGlobals) {
 	  return HS_SML_ERR_NOT_ENOUGH_SPACE;
   }

  // check if init was already called
  if ( initWasCalled )
    return HS_SML_ERR_WRONG_USAGE;

  // check options
  if ( wsmOpts != NULL ) {
    if ( wsmOpts->maxAvailMem > 0 ) {
      maxWsmAvailMem = wsmOpts->maxAvailMem;
    }
  }

  // init resources
  for ( i=0; i < HS_MAX_WSM_BUFFERS; ++i )
    wsmBuf[i].memH = WSM_MEMH_UNUSED;
  wsmIndex = 0;
  initWasCalled = (unsigned char) 1;

  return wsmRet=HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_wsmCreate
 *
 * Creates and opens a new buffer with name bufName and size bufSize.<BR> 
 * If a buffer with name bufName already exists, the existing buffer 
 * is resized to bufSize.
 *
 * PRE-Condition:   bufSize > 0
 *
 * POST-Condition:  handle refers to buffer bufName; BufferSize = size
 *
 * IN:      bufName
 *          Name of buffer to be created
 * IN:      bufSize
 *          Size of buffer to be created
 * 
 * OUT:     wsmH
 *          Handle to new buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_SIZE, if bufSize <= 0
 *          HS_SML_ERR_NOT_ENOUGH_SPACE, if available memory < bufSize
 *          HS_SML_ERR_WSM_BUF_TABLE_FULL, if buffer table is full
 *          HS_SML_ERR_WRONG_USAGE, if hs_wsmInit wasn't called before
 *
 * @see  hs_wsmDestroy
 */
short hs_wsmCreate (char* bufName, long bufSize, short *wsmH) {

  *wsmH = 0;    // 0 in case of error

  if ( ! initWasCalled )
  {
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 1")); // chenxiang_20080304
    return HS_SML_ERR_WRONG_USAGE;
  }

  // check buffer space
  if ( hs_getNextFreeEntry() == -1 ) {
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 2")); // chenxiang_20080304
    return wsmRet=HS_SML_ERR_WSM_BUF_TABLE_FULL;
  }
  // check for maxMemAvailable
  if ( ! hs_isMemAvailable(bufSize) ) {
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 3")); // chenxiang_20080304
    return HS_SML_ERR_NOT_ENOUGH_SPACE;
  }

  // create buffer
  if ( (wsmRet = hs_smCreate(bufName, bufSize, wsmH)) != HS_SML_ERR_OK ) {
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 4")); // chenxiang_20080304
    if ( wsmRet == HS_SML_ERR_WRONG_USAGE ) {    // buffer already exists
      // resize existing buffer
      // open buffer
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 5")); // chenxiang_20080304
      if ( (wsmRet = hs_smOpen(bufName, wsmH)) != HS_SML_ERR_OK ) {
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 6")); // chenxiang_20080304
      	return wsmRet=HS_SML_ERR_NOT_ENOUGH_SPACE;
      }
      // resize buffer
      if ( (wsmRet = hs_smSetSize(*wsmH, bufSize)) != HS_SML_ERR_OK ) { 
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 7")); // chenxiang_20080304
      	return wsmRet=HS_SML_ERR_NOT_ENOUGH_SPACE;
      }
    }
    else {
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 8")); // chenxiang_20080304
      return wsmRet;
    }
  }

       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 9")); // chenxiang_20080304
  // reset buffer vars
  wsmIndex = hs_resetBufferGlobals(*wsmH);

  // set buffer vars
  wsmBuf[wsmIndex].size = bufSize;
  wsmBuf[wsmIndex].bufName = hs_smlLibStrdup(bufName);

   if (wsmBuf[wsmIndex].bufName == NULL) {
 	  hs_smClose(*wsmH);
 	  hs_smDestroy(bufName);
       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 10")); // chenxiang_20080304
 	  return wsmRet=HS_SML_ERR_NOT_ENOUGH_SPACE;
   }

       syncml_task_message(("MMIPIM chenxiang hs_wsmCreate 11")); // chenxiang_20080304

  return wsmRet=HS_SML_ERR_OK;
}



/**
 * FUNCTION: hs_wsmClose
 *
 * Close an open buffer.
 *
 * PRE-Condition:   handle is valid; handle is unlocked
 *
 * POST-Condition:  handle is not known to WSM any more
 *
 * IN:      wsmH
 *          Handle to the open buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was still locked
 *
 * @see  wsmOpen
 */
short hs_wsmClose (short wsmH) {

  // check if handle is invalid
  if ( ! hs_isValidMemH(wsmH) ) {
     //syncml_task_message:"MMIPIM chenxiang hs_wsmClose 1"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_894_112_2_18_3_26_2_292,(uint8*)"");
    return wsmRet=HS_SML_ERR_INVALID_HANDLE;
  }

  // close handle
  if ( (wsmRet = hs_smClose(wsmH)) != HS_SML_ERR_OK ) {
     //syncml_task_message:"MMIPIM chenxiang hs_wsmClose 2"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_900_112_2_18_3_26_3_293,(uint8*)"");
    return wsmRet;
  }
  wsmRet = hs_deleteBufferHandle(wsmH);

     //syncml_task_message:"MMIPIM chenxiang hs_wsmClose 3 wsmRet = 0x%4x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_905_112_2_18_3_26_3_294,(uint8*)"d", wsmRet);
  return wsmRet=HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_wsmDestroy
 *
 * Destroy existing buffer with name bufName.
 *
 * PRE-Condition:   WSM knows bufName; handle is unlocked
 *
 * POST-Condition:  buffer is not known to WSM any more; all resources 
 *                  connected to this buffer are freed
 *
 * IN:      bufName
 *          Name of buffer to be opened
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_PARAM, if bufName is unknown to WSM
 *          HS_SML_ERR_WRONG_USAGE, if handle was still locked
 *
 * @see  hs_wsmCreate
 */
short hs_wsmDestroy (char* bufName) {

  // free resources
  if ( (wsmRet = hs_wsmClose(hs_nameToHandle(bufName))) != HS_SML_ERR_OK ) {
     //syncml_task_message:"MMIPIM chenxiang hs_wsmDestroy 1"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_932_112_2_18_3_26_3_295,(uint8*)"");
    return wsmRet;
  }

  // free buffer
  if ( (wsmRet = hs_smDestroy(bufName)) != HS_SML_ERR_OK ) {
     //syncml_task_message:"MMIPIM chenxiang hs_wsmDestroy 2"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_938_112_2_18_3_26_3_296,(uint8*)"");
    return wsmRet;
  }
   
     //syncml_task_message:"MMIPIM chenxiang hs_wsmDestroy 3"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMWSM_942_112_2_18_3_26_3_297,(uint8*)"");
  return wsmRet=HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_wsmTerminate
 *
 * Terminate WSM; free all buffers and resources.
 *
 * PRE-Condition: all handles must be unlocked
 *
 * POST-Condition: all resources are freed
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_USAGE, if a handle was still locked
 *
 */
short hs_wsmTerminate (void) 
{
  int i;

  // free all WSM resources
  for (i=0; i < HS_MAX_WSM_BUFFERS; ++i) {
    if ( wsmBuf[i].memH != WSM_MEMH_UNUSED )
      if ( hs_wsmDestroy(wsmBuf[i].bufName) == HS_SML_ERR_WRONG_USAGE ) {
	return HS_SML_ERR_WRONG_USAGE;
      }
  }

  // free global DataStructs
  freeDataStructs();

  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_wsmLockH
 *
 * Locks handle wsmH and get a pointer to the contents of wsmH. <BR>
 * RequestedPos describes the position in the buffer to which the returned 
 * pointer should point. Valid values are: 
 * <UL>
 *   <LI> SML_FIRST_DATA_ITEM
 *   <LI> SML_FIRST_FREE_ITEM
 * </UL>
 *
 * PRE-Condition:   handle is unlocked; handle is valid
 *
 * POST-Condition:  handle is locked; points to first data item, 
 *                  or first free item.
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * IN:      requestedPos
 *          Requested position of the returned pointer
 *          <UL>
 *            <LI> SML_FIRST_DATA_ITEM : points to first data entry
 *            <LI> SML_FIRST_FREE_ITEM : points to first free entry
 *          </UL>
 * 
 * OUT:     pMem
 *          Pointer to requested memory          
 *          
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was still locked
 *          HS_SML_ERR_UNSPECIFIC, if requested position is unknown, or lock failed
 *
 * @see  hs_wsmUnlockH
 */
short hs_wsmLockH (short wsmH, SmlBufPtrPos_t requestedPos,
		unsigned char **pMem) {

  // check if handle is invalid
  if ( ! hs_isValidMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_INVALID_HANDLE;
  }
  // check if handle is locked
  if ( hs_isLockedMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_WRONG_USAGE;
  }

  // lock
  if ( (wsmRet = hs_smLock(wsmH, pMem)) != HS_SML_ERR_OK ) {
    return wsmRet=HS_SML_ERR_UNSPECIFIC;
  }

  // set local pointers  
  wsmIndex = hs_lookup(wsmH);
  wsmBuf[wsmIndex].pFirstData = *pMem;
  wsmBuf[wsmIndex].pFirstFree = *pMem + wsmBuf[wsmIndex].usedBytes;
  wsmBuf[wsmIndex].flags |= WSM_LOCKED_F;

  switch (requestedPos) {
  case SML_FIRST_DATA_ITEM:
    *pMem = wsmBuf[wsmIndex].pFirstData;
    break;
  case SML_FIRST_FREE_ITEM:
    *pMem = wsmBuf[wsmIndex].pFirstFree;
    break;
  default:
    return wsmRet=HS_SML_ERR_UNSPECIFIC;
  }

  return wsmRet=HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_wsmGetFreeSize
 *
 * Returns the remaining unused bytes in the buffer.
 *
 * PRE-Condition:   handle is valid
 *
 * POST-Condition:  hs_wsmGetFreeSize = BufferSize - hs_wsmGetUsedSize
 *
 * IN:      wsmH
 *          Handle to the open buffer
 *
 * OUT:     freeSize
 *          Number of bytes which are unused in this buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *
 * @see  hs_wsmGetUsedSize
 * @see  hs_wsmProcessedBytes
 */
short hs_wsmGetFreeSize(short wsmH, long *freeSize) {

  // check if handle is invalid
  if ( ! hs_isValidMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_INVALID_HANDLE;
  }
  
  wsmIndex = hs_lookup(wsmH);

  *freeSize = wsmBuf[wsmIndex].size - wsmBuf[wsmIndex].usedBytes;

  return wsmRet=HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_wsmSetUsedSize
 *
 * Tell Workspace how many data were written into buffer.
 *
 * PRE-Condition:   handle is valid; usedSize <= hs_wsmGetFreeSize; handle is 
 *                  locked
 *
 * POST-Condition:  hs_wsmGetUsedSize += usedSize; hs_wsmGetFreeSize -= usedSize;
 *                  instancePtr += usedSize;
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * IN:      usedSize
 *          Number of bytes which were written into buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_INVALID_SIZE, if usedSize <= hs_wsmGetFreeSize
 *
 * @see  hs_wsmGetUsedSize
 */
short hs_wsmSetUsedSize (short wsmH, long usedSize) {

  // check if handle is invalid
  if ( ! hs_isValidMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_INVALID_HANDLE;
  }
  // check if handle is unlocked
  if ( ! hs_isLockedMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_WRONG_USAGE;
  }

  wsmIndex = hs_lookup(wsmH);

  // usedSize > freeSize?
  if ( usedSize > 
       (wsmBuf[wsmIndex].size - wsmBuf[wsmIndex].usedBytes) ) {
    return wsmRet=HS_SML_ERR_INVALID_SIZE;
  }

  // adapt usedSize
  wsmBuf[wsmIndex].usedBytes += usedSize;

  // move pFirstFree
  wsmBuf[wsmIndex].pFirstFree += usedSize;

  return wsmRet=HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wsmGetUsedSize
 *
 * Returns the number of bytes used in the buffer.
 *
 * PRE-Condition:   handle is valid
 *
 * POST-Condition:  usedSize = BufferSize - hs_wsmGetFreeSize
 *
 * IN:      wsmH
 *          Handle to the open buffer
 *
 * OUT:     usedSize
 *          Number of bytes which are already used in this buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *
 * @see  hs_wsmGetFreeSize
 * @see  hs_wsmSetUsedSize
 */
short hs_wsmGetUsedSize(short wsmH, long *usedSize) 
{
  // check if handle is invalid
  if ( ! hs_isValidMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_INVALID_HANDLE;
  }
  
  wsmIndex = hs_lookup(wsmH);

  *usedSize = wsmBuf[wsmIndex].usedBytes;

  return wsmRet=HS_SML_ERR_OK;
}
 
/**
 * FUNCTION: hs_wsmProcessedBytes
 *
 * Tell Workspace Manager the number of bytes already processed.
 *
 * PRE-Condition:   handle is locked; handle is valid;
 *                  noBytes <= hs_wsmGetUsedSize
 *
 * POST-Condition:  noBytes starting at wsmGetPtr() position are deleted; 
 *                  remaining bytes are copied to 
 *                  wsmGetPtr(SML_FIRST_FREE_ITEM) position;
 *                  hs_wsmGetUsedSize -= noBytes; hs_wsmGetFreeSize += noBytes
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * IN:      noBytes
 *          Number of bytes already processed from buffer.
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was not locked
 *          HS_SML_ERR_INVALID_SIZE, if noBytes > hs_wsmGetUsedSize
 *
 * @see  hs_wsmGetFreeSize
 */
short hs_wsmProcessedBytes (short wsmH, long noBytes)
{
  // check if handle is invalid
  if ( ! hs_isValidMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_INVALID_HANDLE;
  }
  // check if handle is unlocked
  if ( ! hs_isLockedMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_WRONG_USAGE;
  }

  wsmIndex = hs_lookup(wsmH);

  if ( noBytes > wsmBuf[wsmIndex].usedBytes ) {
    return wsmRet=HS_SML_ERR_INVALID_SIZE;
  }

  // adapt usedSize
  wsmBuf[wsmIndex].usedBytes -= noBytes;

  // move memory
  // check return ?????
  hs_smlLibMemmove(wsmBuf[wsmIndex].pFirstData,
	  (wsmBuf[wsmIndex].pFirstData + noBytes),
	  wsmBuf[wsmIndex].usedBytes);

  // move pFirstFree
  wsmBuf[wsmIndex].pFirstFree -= noBytes;

  return wsmRet=HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wsmUnlockH
 *
 * Unlock handle wsmH. <BR>
 * After this call all pointers to this memory handle are invalid 
 * and should no longer be used.
 *
 * PRE-Condition:   handle is locked; handle is valid
 *
 * POST-Condition:  handle is unlocked
 *
 * OUT:     wsmH
 *          Handle to unlock
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was not locked
 *          HS_SML_ERR_UNSPECIFIC, unlock failed
 *
 * @see  hs_wsmLockH
 */
short hs_wsmUnlockH (short wsmH) {

  // check if handle is invalid
  if ( ! hs_isValidMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_INVALID_HANDLE;
  }
  // check if handle is already unlocked
  if ( ! hs_isLockedMemH(wsmH) ) {
    return wsmRet=HS_SML_ERR_WRONG_USAGE;
  }

  // unlock
  if ( (wsmRet = hs_smUnlock(wsmH)) != HS_SML_ERR_OK ) {
    return wsmRet=HS_SML_ERR_UNSPECIFIC;
  }

  // set local pointers  
  wsmIndex = hs_lookup(wsmH);
  wsmBuf[wsmIndex].pFirstData = NULL;
  wsmBuf[wsmIndex].pFirstFree = NULL;
  wsmBuf[wsmIndex].flags &= ~WSM_LOCKED_F;

  return wsmRet=HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_pim_wsmReset
 *
 * Reset the Workspace
 *
 * PRE-Condition:   -
 *
 * POST-Condition:  all data is lost. The FirstFree Position equals 
 * the First Data position
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * 
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *
 */
short hs_pim_wsmReset (short wsmH) {

  wsmIndex = hs_lookup(wsmH);
  wsmBuf[wsmIndex].pFirstFree = wsmBuf[wsmIndex].pFirstFree - wsmBuf[wsmIndex].usedBytes ;
  wsmBuf[wsmIndex].pFirstData = wsmBuf[wsmIndex].pFirstFree;
  wsmBuf[wsmIndex].usedBytes = 0;

  return HS_SML_ERR_OK;
}
