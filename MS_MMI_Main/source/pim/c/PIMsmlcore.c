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
文件名称:PIMsmlcore.c
文件描述:功能模块核心
文件说明:功能模块的核心内容，关键函数，但并不一定就是
                       真正的实现函数。模块核心的实现文件。
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
  #define _PIMSMLCORE_C_
  
  #include "../h/PIMsmlcore.h"
  #include "../h/PIMsmlerr.h"
  #include "../h/PIMlibmem.h"

#include "../h/mmipim.h"
  /*  数据结构声明*/
  
  /*  全局变量声明*/
  SyncMLInfoPtr_t     hs_pGlobalAnchor=NULL; // this global pointer is used to access ALL globals within syncml

 static short hs_smlSetSyncMLOptions (SmlOptionsPtr_t pOptions);
 static short hs_smlSetCallbacks(short id, SmlCallbacksPtr_t pCallbacks);
 static short hs_freeInstanceInfo(InstanceInfoPtr_t pInfo);
 static short hs_freeInstanceOptions (InstanceInfoPtr_t pInfo);
 static short hs_setInstanceOptions (short id, SmlInstanceOptionsPtr_t pOptions);
 static short hs_mgrCreateNextCommand(short id, SmlProtoElement_t cmdType, void* pContent);

/*************************************************************************
 *  Exported SyncML API functions
 *************************************************************************/

/**
 * FUNCTION: hs_smlInit
 *
 * Initializes the SyncML Reference Tookit. This is required, before any
 * other function can be used.
 *
 * IN:              SyncMLOptionsPtr_t
 *                  options to be applied for the toolkit
 *
 * RETURN:          Ret_t 
 *                  Return Code
 */
 short hs_smlInit(SmlOptionsPtr_t pCoreOptions)
{

  /* ---- Definitions --- */
  WsmOptions_t*      pWorkspaceOptions;
  short              rc;
  

  /* --- check, if SyncML has already been initialized --- */
  if (hs_pGlobalAnchor!=NULL) return HS_SML_ERR_ALREADY_INITIALIZED;

  /* --- Check pOptions, which have been passed by the application --- */
  if (!pCoreOptions)
	  return HS_SML_ERR_WRONG_USAGE;


  /* --- Create a SyncML info memory object to store all globals --- */
  hs_pGlobalAnchor = (SyncMLInfoPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SyncMLInfo_t));
  if (hs_pGlobalAnchor==NULL) return HS_SML_ERR_NOT_ENOUGH_SPACE;
  hs_smlLibMemset(hs_pGlobalAnchor,0,(long)sizeof(SyncMLInfo_t));


  /* --- Set SyncML settings and options  --- */
  hs_pGlobalAnchor->instanceListAnchor = NULL;  // no instance exists at the beginning
  rc =hs_smlSetSyncMLOptions (pCoreOptions);   // store the options in the global structure
  if (rc!=HS_SML_ERR_OK){
     hs_smlLibFree(hs_pGlobalAnchor);
     hs_pGlobalAnchor = NULL;
     return rc;
  }

  hs_pGlobalAnchor->tokTbl = (TokenInfoPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(TokenInfo_t));
  if (hs_pGlobalAnchor->tokTbl == NULL)
  {
   hs_smlLibFree(hs_pGlobalAnchor);
   return HS_SML_ERR_NOT_ENOUGH_SPACE;
   }
  hs_smlLibMemset(hs_pGlobalAnchor->tokTbl, 0, sizeof(TokenInfo_t));
  /* --- Init all modules ---*/
 
  /* Init Workspace Module */
  pWorkspaceOptions=(WsmOptions_t*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(WsmOptions_t)); // create workspace options
  if (pWorkspaceOptions == NULL) {
      hs_smlLibFree((hs_pGlobalAnchor->syncmlOptions));
      hs_smlLibFree((hs_pGlobalAnchor->tokTbl));
      hs_smlLibFree(hs_pGlobalAnchor);
      hs_pGlobalAnchor = NULL;
      return HS_SML_ERR_NOT_ENOUGH_SPACE;
  }

  hs_smlLibMemset(pWorkspaceOptions,0,(long)sizeof(WsmOptions_t));
  pWorkspaceOptions->maxAvailMem=(long)hs_pGlobalAnchor->syncmlOptions->maxWorkspaceAvailMem;    
  
  rc = hs_wsmInit (pWorkspaceOptions);
  if (rc!=HS_SML_ERR_OK){
    hs_smlLibFree((hs_pGlobalAnchor->syncmlOptions));
    hs_smlLibFree((hs_pGlobalAnchor->tokTbl));
    hs_smlLibFree(hs_pGlobalAnchor);
    hs_pGlobalAnchor = NULL;
    hs_smlLibFree(pWorkspaceOptions);
    return rc;
  }
  hs_smlLibFree(pWorkspaceOptions);
  return HS_SML_ERR_OK;
}



/**
 * FUNCTION:   hs_smlInitInstance
 *
 * Creates a SyncML instance and assigns a corresponding workspace buffer in
 * which XML documents are assembled or parsed.
 * All callback functions implemented by a particular application are defined.
 * Instance specific options can be passed. This function has to be called 
 * before the first synchronization tasks can be performed. A reference valid
 * for a SyncML instance is returned.
 * An instance is active when processing a synchronization request
 * otherwise it is idle. An instance is terminated when smlTerminateInstance 
 * is called.
 *
 * IN:              SmlCallbacks_t
 *                  A structure holding references to the callback functions
 *                  implemented by the application
 *
 * IN:              SmlInstanceOptionsPtr_t
 *                  Option settings of a particular SyncML instance
 *
 * IN:              VoidPtr_t
 *                  UserData is a pointer to a void structure the application 
 *                  can pass into the SyncML Toolkit instance info. 
 *                  It will be returned to the application with every called 
 *                  callback function call!
 *                  NOTE: This is only a pointer, the memory object itself 
 *                  remains within the responsibility of the calling application.
 *                  The memory object will not be copied, moved or freed by the 
 *                  Toolkit.
 *
 * OUT:             InstanceID_t
 *                  Instance ID assigned to the initialized instance
 *
 * RETURN:          Ret_t
 *                  Error Code
 */
short hs_smlInitInstance(SmlCallbacksPtr_t pCallbacks, SmlInstanceOptionsPtr_t pOptions, void* pUserData, short *pInstanceID)
{
	/***********************************/
     /* --- Definitions --- */
    InstanceInfoPtr_t pInstanceInfo;
    short             rc;
  
  
      /* --- Check pOptions, which have been passed by the application --- */
      if (!pOptions || !pOptions->workspaceName)
    	  return HS_SML_ERR_WRONG_USAGE;
  
  
      /* --- check wether we already know about this instance --- */
        pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(*pInstanceID);
  
      /* --- bail outh when we already have a instance with that id --- */
      if (pInstanceInfo != NULL) 
      {
       syncml_task_message(("MMIPIM chenxiang hs_smlInitInstance")); // chenxiang_20080304
        return HS_SML_ERR_WRONG_USAGE;
      }
  
  
      /* --- Create a workspace for this instance --- */
      if ((rc = hs_wsmCreate(pOptions->workspaceName, pOptions->workspaceSize, pInstanceID)) != HS_SML_ERR_OK) {
      	return rc;
      }
    
    /* --- Create an instance info memory object --- */
    pInstanceInfo = (InstanceInfoPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(InstanceInfo_t));
    if (pInstanceInfo==NULL) {
      hs_wsmDestroy(pOptions->workspaceName);   
      return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
     
    hs_smlLibMemset(pInstanceInfo,0,(long)sizeof(InstanceInfo_t));
  
  
  
    /* --- Set mandatory instance infos for this instance to defaults --- */
    pInstanceInfo->status=MGR_IDLE;
    pInstanceInfo->encoderState=NULL;                  // no encoding in progress, currently not used
    pInstanceInfo->decoderState=NULL;                  // no decoding in progress, currently not used
    pInstanceInfo->id=*pInstanceID;
    pInstanceInfo->workspaceState=NULL;                // to do: some workspace status info
    pInstanceInfo->nextInfo=NULL;
  
  
      /* --- Add instance infos memory object to the instance info list --- */
        rc = hs_addInfo( pInstanceInfo );
        if (rc!=HS_SML_ERR_OK) return rc;
  
  
    /* --- Set the values of instance Infos as defined by the calling application ---*/
  
    /* Set user data pointer */
    pInstanceInfo->userData=pUserData;
    /* Set callback functions implemented by applications */
    if (hs_smlSetCallbacks(*pInstanceID, pCallbacks) != HS_SML_ERR_OK) {
  	  hs_wsmDestroy(pOptions->workspaceName);
  	  return rc;
    }
  
    // luz: %%% this was called twice, probably this is a bug, so I disabled the second call
    //hs_smlSetCallbacks(*pInstanceID, pCallbacks);
  
    /* Set other application defined options for that instance */
    if (hs_setInstanceOptions (*pInstanceID, pOptions) != HS_SML_ERR_OK) {
   	  hs_wsmDestroy(pOptions->workspaceName);
   	  return rc;
    }
  
    return HS_SML_ERR_OK;	
}


/**
 * FUNCTION: hs_smlSetSyncMLOptions
 *
 * change the option settings for syncML
 *
 * IN:              SyncMLOptionsPtr_t
 *                  options to be applied for the toolkit
 *
 * RETURN:          Ret_t 
 *                  Return Code
 */
 static short hs_smlSetSyncMLOptions(SmlOptionsPtr_t pCoreOptions) {


  /* ---- Definitions --- */
  SmlOptionsPtr_t pCoreOptionsCopy;
 

  /* --- Check pOptions, which have been passed by the application --- */
  if (!pCoreOptions)
	  return HS_SML_ERR_WRONG_USAGE;

  
  /* --- free SyncML options --- */
  hs_smlLibFree((hs_pGlobalAnchor->syncmlOptions)); 
  hs_pGlobalAnchor->syncmlOptions = NULL; 
  /* --- Use a copy of pCoreOptions --- */
  pCoreOptionsCopy =  (SmlOptionsPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlOptions_t));
  if (pCoreOptionsCopy==NULL) return HS_SML_ERR_NOT_ENOUGH_SPACE;
  hs_smlLibMemcpy(pCoreOptionsCopy,pCoreOptions,(long)sizeof(SmlOptions_t));


  /* --- set new SyncML options --- */
  hs_pGlobalAnchor->syncmlOptions  = pCoreOptionsCopy;  // set the options,passed from the application 
  
  return HS_SML_ERR_OK;

}



/**
 * FUNCTION:  hs_smlSetCallbacks
 *
 * Sets new callback functions to an instance
 *
 * IN:        InstanceID_t              
 *            ID of the Instance
 *
 * IN:        SmlCallbacksPtr_t
 *            A structure holding references to the callback functions
 *            implemented by the application
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if successful
 */
static short hs_smlSetCallbacks(short id, SmlCallbacksPtr_t pCallbacks)
{

  /* --- Definitions --- */
  InstanceInfoPtr_t	pInstanceInfo;
  SmlCallbacksPtr_t    pCallbacksCopy;

  /* --- Check pCallbacks, which have been passed by the application --- */
  if (!pCallbacks)
    return HS_SML_ERR_WRONG_USAGE;


  /* --- Find that instance --- */
   pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);

  if (pInstanceInfo==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;

  /* --- free old callback structure ---*/
  hs_smlLibFree((pInstanceInfo->callbacks));


  /* --- Use a copy of pCallbacksCopy --- */
  pCallbacksCopy = (SmlCallbacksPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlCallbacks_t));
  if (pCallbacksCopy==NULL) return HS_SML_ERR_NOT_ENOUGH_SPACE;
  hs_smlLibMemcpy(pCallbacksCopy,pCallbacks,(long)sizeof(SmlCallbacks_t));

  
  /* --- set new Callbacks --- */
   pInstanceInfo->callbacks = pCallbacksCopy;
  
   return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smlStartMessageExt
 * (%%% added by luz 2003-08-06 to support SyncML versions other than
 * 1.0 with new vers parameter)
 *
 * Start a SyncML Message 
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *                  SyncML version
 *
 * IN:              SmlSyncHdrPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlStartMessageExt(short id, SmlSyncHdrPtr_t pContent, SmlVersion_t vers)
{

  /* --- Definitions --- */
  InstanceInfoPtr_t   pInstanceInfo;               // pointer the the instance info structure for this id
  short               rc;
  unsigned char*            pCurrentWritePosition;       // current Position from to which to write
  unsigned char*            pBeginPosition;              // saves the first position which has been written
  long           freeSize;                    // size of free memory for writing


  /* --- Retrieve the corresponding instanceInfo structure --- */

  pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);

  if (pInstanceInfo==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;


  /* --- Get Write Access to the workspace --- */
  rc = hs_smlLockWriteBuffer(id, &pCurrentWritePosition, &freeSize);

  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockWriteBuffer(id, (long)0);  
    return rc;
    }

 /* Remember the position we have started writing */
  pBeginPosition=pCurrentWritePosition;             
  
  /* --- Call the encoder module --- */
  /*     (Saves the returned encoder state to the corresponding instanceInfo structure */
  rc = hs_xltEncInit(pInstanceInfo->instanceOptions->encoding, pContent,
                  pCurrentWritePosition+freeSize, &pCurrentWritePosition, 
                  (XltEncoderPtr_t *)&(pInstanceInfo->encoderState),
                  vers);

  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockWriteBuffer(id, (long)0);  
  	// Reset the encoder module (free the encoding object)
	  hs_xltEncReset(pInstanceInfo->encoderState);
    // this encoding job is over! reset instanceInfo pointer
    pInstanceInfo->encoderState=NULL;           

    return rc;
    }

  /* --- End Write Access to the workspace --- */
  rc = hs_smlUnlockWriteBuffer(id, (long)pCurrentWritePosition-(long)pBeginPosition);
  return rc;
}

/**
 * FUNCTION: hs_smlStartSync
 *
 * Start synchronizing
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              SyncPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlStartSync(short id, SmlSyncPtr_t pContent)
{
   //syncml_core_message:"MMIPIM enter hs_smlStartSync "
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSMLCORE_425_112_2_18_3_25_53_228,(uint8*)"");
  return hs_mgrCreateNextCommand(id, SML_PE_SYNC_START, pContent);
}

#ifdef HS_ADD_SEND
/**
 * FUNCTION: hs_smlAddCmd
 *
 * Create a Add Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              SmlAddPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlAddCmd(short id, SmlAddPtr_t pContent)
{
  return hs_mgrCreateNextCommand(id, SML_PE_ADD, pContent);
}
#endif

/**
 * FUNCTION: hs_smlDeleteCmd
 *
 * Create a Delete Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              SmlDeletePtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlDeleteCmd(short id, SmlDeletePtr_t pContent)
{
  return hs_mgrCreateNextCommand(id, SML_PE_DELETE, pContent);
}

/**
 * FUNCTION: hs_smlReplaceCmd
 *
 * Create a Replace Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              SmlReplacePtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlReplaceCmd(short id, SmlReplacePtr_t pContent)
{
  return hs_mgrCreateNextCommand(id, SML_PE_REPLACE, pContent);
}

/**
 * FUNCTION: hs_smlAlertCmd
 *
 * Create a Alert Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              SmlAlertPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlAlertCmd(short id, SmlAlertPtr_t pContent)
{
   return hs_mgrCreateNextCommand(id, SML_PE_ALERT, pContent);
}

/**
 * FUNCTION: hs_smlPutCmd
 *
 * Create a Put Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              PutPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlPutCmd(short id, SmlPutPtr_t pContent)
{
  return hs_mgrCreateNextCommand(id, SML_PE_PUT, pContent);
}

/**
 * FUNCTION: hs_smlGetCmd
 *
 * Create a Get Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              GetPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlGetCmd(short id, SmlGetPtr_t pContent)
{
  return hs_mgrCreateNextCommand(id, SML_PE_GET, pContent);
}

/**
 * FUNCTION: hs_smlStatusCmd
 *
 * Create a Status Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              StatusPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlStatusCmd(short id, SmlStatusPtr_t pContent)
{
  return hs_mgrCreateNextCommand(id, SML_PE_STATUS, pContent);
}

/**
 * FUNCTION: hs_smlMapCmd
 *
 * Create a Map Command
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              MapPtr_t
 *                  Data to pass along with that SyncML command
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlMapCmd(short id, SmlMapPtr_t pContent)
{
  return hs_mgrCreateNextCommand(id, SML_PE_MAP, pContent);
}

/**
 * FUNCTION: hs_smlEndSync
 *
 * End synchronizing
 *
 * IN:              InstanceID_t
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlEndSync(short id)
{
  return hs_mgrCreateNextCommand(id, SML_PE_SYNC_END, NULL);
}


/******************************************************************* 
** 函数名:hs_smlSetUserData
** 功能描述：将instance anchor的userdata指针指向给定的buffer。
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_smlSetUserData(short id, void* pUserData)
{

  /* --- Definitions --- */
  InstanceInfoPtr_t	pInstanceInfo;


  /* --- Find that instance --- */
  pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);

  if (pInstanceInfo==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;


  /* --- set new user data pointer ---*/
  pInstanceInfo->userData=pUserData;

	return HS_SML_ERR_OK;
}


/******************************************************************* 
** 函数名:hs_smlProcessData
** 功能描述：分析收到的存在workspace中的xml/wbxml文档
**                           ，然后分发相应的命令给callback函数执行
**                            具体的功能。此函数是接收功能最重要
**                            的函数。
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*************************************************************************/
short hs_smlProcessData(short id, SmlProcessMode_t mode)
{  
  /* --- Definitions --- */
  InstanceInfoPtr_t   pInstanceInfo;               // state info for the given instanceID
  short               rc;                          // Temporary return code saver


  /* --- Find that instance --- */
  pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);

  if (pInstanceInfo==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;

  /* --- Are callback functions defined? --- */
  if (pInstanceInfo->callbacks==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;

  /* --- Is parsing already in progress? --- */
  if (pInstanceInfo->decoderState==NULL) 
    {
    /* No! Parse the Message header section first */    
    rc = hs_mgrProcessStartMessage(id, pInstanceInfo);    

    if (rc!=HS_SML_ERR_OK) return rc;
    }
    
    
  /* --- Parse now the Message body section! --- */ 
  do {
    rc=hs_mgrProcessNextCommand(id, pInstanceInfo);
  } while (
    // keep processing while no error occurs,
    // AND the document end was not reached (decoderState has been invalidated),
    // AND the ALL_COMMAND mode is used
    (rc==HS_SML_ERR_OK)                                    
    &&((pInstanceInfo->decoderState)!=NULL)   
    &&(mode==SML_ALL_COMMANDS)
  );                         

  if (rc != HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockReadBuffer(id, (long)0);  
    // Reset the decoder module (free the decoding object)
    hs_xltDecReset(pInstanceInfo->decoderState);
    // this decoding job is over! reset Instance Info pointer
    pInstanceInfo->decoderState=NULL;     
    // Reset the Workspace (the remaining unparsed document fragment will be lost)
    hs_mgrResetWorkspace(id);
  }

  return rc;  
}

/**
 * FUNCTION: hs_smlEndMessage
 *
 * End a SyncML Message
 *
 * IN:              InstanceID_t
 *                  ID of the used instance
 *
 * IN:              Boolean_t
 *                  Final Flag indicates last message within a package
 *
 * RETURN:          Ret_t
 *                  Return Code
 */
short hs_smlEndMessage(short id, unsigned char final)
{

  /* --- Definitions --- */
  InstanceInfoPtr_t   pInstanceInfo;               // pointer the the instance info structure for this id
  short               rc;
  unsigned char*            pCurrentWritePosition;       // current Position from to which to write
  unsigned char*            pBeginPosition;              // saves the first position which has been written
  long           freeSize;                    // size of free memory for writing


    /* --- Retrieve the corresponding instanceInfo structure --- */
      pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);

  if (pInstanceInfo==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;
  // %%% luz 2003-08-19: added NULL check as previously failed encoding will delete encoder
  if (pInstanceInfo->encoderState==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;


  /* --- Get Write Access to the workspace --- */
  rc = hs_smlLockWriteBuffer(id, &pCurrentWritePosition, &freeSize);

  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockWriteBuffer(id, (long)0);  
    return rc;
    }
  
  
  /* Remember the position we have started writing */
  pBeginPosition=pCurrentWritePosition;             
 
  /* -- set Final Flag --*/ 
  ((XltEncoderPtr_t)(pInstanceInfo->encoderState))->final = final;
  
  /* --- Call the encoder module --- */
  rc = hs_xltEncTerminate(pInstanceInfo->encoderState, pCurrentWritePosition+freeSize,&pCurrentWritePosition);
  
  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockWriteBuffer(id, (long)0);  
    // this encoding job is over! reset instanceInfo pointer
    pInstanceInfo->encoderState=NULL;           

    return rc;
  }

  // this encoding job is over! reset instanceInfo pointer
  // (the decoding object itself has been freed by the decoder)
  pInstanceInfo->encoderState=NULL;           

  /* --- End Write Access to the workspace --- */
  rc = hs_smlUnlockWriteBuffer(id, (long)pCurrentWritePosition-(long)pBeginPosition);

  
  return rc;
}



/**
 * FUNCTION:   hs_smlTerminateInstance
 *
 * Terminates a SyncML instance. The instance info is removed from the instances
 * list. Allmemory allocated for the workspace and the options variables is freed.
 *
 * IN:              InstanceID_t
 *                  ID of the instance to be terminated
 *
 * RETURN:          Ret_t
 *                  Error Code
 */
short hs_smlTerminateInstance (short id)
{

  /* --- Definitions --- */
  InstanceInfoPtr_t pInstanceInfo;

  short             rc;
  /* --- Find that instance --- */
  pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);
  
  if (pInstanceInfo==NULL) 
  {
     //syncml_task_message:"MMIPIM chenxiang hs_smlTerminateInstance 1"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSMLCORE_774_112_2_18_3_25_54_229,(uint8*)"");
        return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;
  }

  /* --- Close the workspace --- */
  if (pInstanceInfo->instanceOptions != NULL) {
     //syncml_task_message:"MMIPIM chenxiang hs_smlTerminateInstance 2"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSMLCORE_780_112_2_18_3_25_54_230,(uint8*)"");
    	rc = hs_wsmDestroy(pInstanceInfo->instanceOptions->workspaceName);
    	if (rc!=HS_SML_ERR_OK) {
     //syncml_task_message:"MMIPIM chenxiang hs_smlTerminateInstance 3 , rc = 0x%4x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSMLCORE_783_112_2_18_3_25_54_231,(uint8*)"d", rc);
    		return rc;
    	}
   }	
      
 /* --- Delete instance info and options --- */
  hs_removeInfo(id);
  
  hs_freeInstanceInfo (pInstanceInfo);
  
  return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_smlTerminate
 *
 * Terminate SyncML. Frees all memory and other ressources used by 
 * SyncML. This function must be called when terminating SyncML 
 *
 * PRE-Condition:   All instances must have been terminated
 *
 * RETURN:          Ret_t 
 *                  Return Code
 */
short hs_smlTerminate(void) 
{
  // Have all Instances been terminated?
  if (hs_pGlobalAnchor->instanceListAnchor!=NULL)
	  return HS_SML_ERR_WRONG_USAGE;
  
  /* --- Make sure, the workspace is destroyed --*/
  hs_wsmTerminate();

  /* --- Free the global structure --*/
  hs_smlLibFree((hs_pGlobalAnchor->tokTbl->SyncML));
  hs_smlLibFree((hs_pGlobalAnchor->tokTbl->MetInf));
  hs_smlLibFree((hs_pGlobalAnchor->tokTbl->DevInf));
  hs_smlLibFree((hs_pGlobalAnchor->tokTbl));
  hs_smlLibFree((hs_pGlobalAnchor->syncmlOptions)); 
  hs_smlLibFree(hs_pGlobalAnchor);
  hs_pGlobalAnchor=NULL;
  
  return HS_SML_ERR_OK;
  
}


/**
 * FUNCTION: 
 * Free the memory of an removed Instance Info (including referenced sub structures) 
 *
 * IN:        InstanceID_t              
 *            ID of the InstanceInfo structure to be freed
 */
static short hs_freeInstanceInfo(InstanceInfoPtr_t pInfo) 
{
    if (pInfo)
    {
        if (pInfo->workspaceState) 
        	hs_smlLibFree((pInfo->workspaceState));
        if (pInfo->encoderState) 
        	hs_smlLibFree((pInfo->encoderState));
        if (pInfo->decoderState)
        	hs_smlLibFree((pInfo->decoderState));
        if (pInfo->callbacks)
        	hs_smlLibFree((pInfo->callbacks));
        if(pInfo->workspaceHandle)  //add by wangxiaolin 2009.08.13 ,for memory leak
        {
            hs_smlLibFree(pInfo->workspaceHandle);
        }
      
        hs_freeInstanceOptions(pInfo);
    
        hs_smlLibFree(pInfo);
    }
    
    return HS_SML_ERR_OK;
}


/**
 * FUNCTION:  freeInstanceOptions
 * Free Instances Options
 *
 * RETURN:    InstanceInfoPtr_t         
 *            Pointer to the pInstance Info, which options should be freed
 */
static short hs_freeInstanceOptions (InstanceInfoPtr_t pInfo) {

  /* --- Delete instance options (if there are any) --- */
  if (pInfo->instanceOptions!=NULL) {
    if (pInfo->instanceOptions->workspaceName!=NULL) 
      hs_smlLibFree((pInfo->instanceOptions->workspaceName));  // don't forget the substructures
    hs_smlLibFree((pInfo->instanceOptions));
  }

 return HS_SML_ERR_OK;  
}


/**
 * FUNCTION:   setInstanceOptions
 *
 * the options settings of an instance are set to a new value
 *
 * IN:              InstanceID_t
 *                  Instance ID assigned to the instance
 *
 * IN:              SmlInstanceOptionsPtr_t
 *                  New option settings of that particular SyncML instance
 *                  NOTE: only the encoding can be changed during life-time 
 *                  of an instance
 *                  The other parameters of the instance options 
 *                  (workspace size and name cannot be changed) 
 *
 * RETURN:          Ret_t
 *                  Error Code
 */
static short hs_setInstanceOptions (short id, SmlInstanceOptionsPtr_t pOptions)
{

  /* --- Definitions --- */
  InstanceInfoPtr_t         pInstanceInfo;
  SmlInstanceOptionsPtr_t      pOptionsCopy;


    /* --- Ckeck pOptions, which have been passed by the application --- */
    if (!pOptions || !pOptions->workspaceName|| (pOptions->encoding==SML_UNDEF))
  	  return HS_SML_ERR_WRONG_USAGE;

    /* --- Find that instance --- */
      pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);

  if (pInstanceInfo==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;
  
  /* --- free old instance options ---*/
  hs_freeInstanceOptions(pInstanceInfo);

  /* --- Use a copy of pOptionsCopy --- */
  pOptionsCopy = (SmlInstanceOptionsPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlInstanceOptions_t));
  if (pOptionsCopy==NULL) return HS_SML_ERR_NOT_ENOUGH_SPACE;
  hs_smlLibMemcpy(pOptionsCopy,pOptions,(long)sizeof(SmlInstanceOptions_t));

  pOptionsCopy->workspaceName=hs_smlLibStrdup(pOptions->workspaceName);

  if (pOptionsCopy->workspaceName == NULL) {
	  pInstanceInfo->instanceOptions=NULL;
 	  hs_smlLibFree(pOptionsCopy);
 	  return HS_SML_ERR_NOT_ENOUGH_SPACE;
   }
   
  /* --- Assign the new options --- */
  pInstanceInfo->instanceOptions=pOptionsCopy;

  /* --- Let the new settingds take effect --- */
  /* --- Adjust workspace size ---*/
  /* --- Change workspace name ---*/
  // NOT SUPPORTED FOR YELLOW 

  return HS_SML_ERR_OK;
}


/**
 * FUNCTION:  hs_smlLockWriteBuffer
 *
 * Locks the workspace buffer, which is assigned to the given 
 * instance for writing. After this function is called, the 
 * application has access to the workspace buffer, beginning 
 * at the address pWritePosition which is returned by this 
 * function. SyncML will not change the workspace buffer until 
 * hs_smlUnlockWriteBuffer is called.
 * pWritePosition returns a pointer to a valid position in the 
 * SyncML workspace buffer. The pointer can be used by the application 
 * for copying incoming synchronization data from some transport 
 * layer into the buffer. freeSize retrieves the maximum usable 
 * size of the workspace buffer beginning from the address to 
 * which pWritePosition points to. This information is needed by 
 * the application when copying XML code into the buffer (while 
 * receiving synchronization data)
 *
 * IN:        InstanceID_t              
 *            ID of the Instance
 *
 * OUT:       MemPtr_t              
 *            Workspace Pointer to which data can be written
 *
 * OUT:       MemSize_t              
 *            Max free Size of available space for data
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if successful
 */
short hs_smlLockWriteBuffer(short id, unsigned char **pWritePosition, long *freeSize)
{
    short rc;
    
    /* --- Lock Workspace exclusively for writing and get a "Write" pointer --- */
    rc = hs_wsmLockH(id, SML_FIRST_FREE_ITEM, pWritePosition);
    if (rc!=HS_SML_ERR_OK) return rc;

    /* --- Check, how much free space is available for writing --- */
    rc = hs_wsmGetFreeSize(id, freeSize);
    if (rc!=HS_SML_ERR_OK) return rc;
  return HS_SML_ERR_OK;
}

/**
 * FUNCTION:  hs_smlUnlockWriteBuffer
 *
 * End the write access of the application to the workspace buffer. 
 * SyncML is now owner of the buffer again and is able to manipulate its 
 * contents. writtenBytes passes the number of bytes which have been 
 * written into the workspace buffer (e.g. when the application has copied 
 * incoming synchronization data from a communication module into the 
 * workspace). This information is needed by SyncML when processing received 
 * synchronization data.
 *
 * IN:        InstanceID_t              
 *            ID of the Instance
 *
 * IN:        MemSize_t              
 *            Actually written bytes
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if successful
 */
short hs_smlUnlockWriteBuffer(short id, long writtenBytes)
{
    short rc;

    if (writtenBytes > 0)
    {
      /* --- Pass the number of bytes which have been written --- */
      rc = hs_wsmSetUsedSize(id,writtenBytes);
      if (rc!=HS_SML_ERR_OK) return rc;
    }
    /* --- Unlock Workspace --- */
    rc = hs_wsmUnlockH(id);
    if (rc!=HS_SML_ERR_OK) return rc;

  return HS_SML_ERR_OK;
}

/**
 * FUNCTION:  hs_smlLockReadBuffer
 *
 * Locks the workspace buffer, which is assigned to the given instance
 * for reading. After this function is called, the application has 
 * access to the workspace buffer, beginning at the address pReadPosition which 
 * is returned by this function. SyncML will not change the workspace 
 * buffer until hs_smlUnlockReadBuffer is called.
 * pReadPosition returns a pointer to a valid position in the SyncML workspace 
 * buffer. The pointer can be used by the application for copying outgoing 
 * synchronization data from the buffer into some transport layer. usedSize 
 * retrieves the size of synchronization data currently stored in the 
 * workspace buffer beginning from the address to which pReadPosition points to. 
 * This information is needed by the application when copying XML code out 
 * of the buffer (while sending synchronization data)
 *
 * IN:        InstanceID_t              
 *            ID of the Instance
 *
 * OUT:       MemPtr_t              
 *            Workspace Pointer from which data can be read
 *
 * OUT:       MemSize_t              
 *            Size of used data in workspace which may be read
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if successful
 */
short hs_smlLockReadBuffer(short id, unsigned char* *pReadPosition, long *usedSize)
{  
    short rc;
    
    /* --- Lock Workspace exclusively for reading and get a "Read" pointer --- */
    rc = hs_wsmLockH(id, SML_FIRST_DATA_ITEM, pReadPosition);
    if (rc!=HS_SML_ERR_OK) return rc;
    
    /* --- Check, how much data has to be read ---*/
    rc = hs_wsmGetUsedSize(id,usedSize);  
    if (rc!=HS_SML_ERR_OK) return rc;
  
  return HS_SML_ERR_OK;
}

/**
 * FUNCTION:  hs_smlUnlockReadBuffer
 *
 * End the read access of the application to the workspace buffer. 
 * SyncML is now owner of the buffer again and is able to manipulate its contents. 
 * processedBytes passes the number of bytes, which the application has 
 * successfully read and processed (e.g. when the application has copied 
 * outgoing synchronization data from the workspace into a communication module). 
 * SyncML removes the given number of bytes from the workspace!
 *
 * IN:        InstanceID_t              
 *            ID of the Instance
 *
 * IN:        MemSize_t              
 *            Actually read and processed bytes
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if successful
 */
short hs_smlUnlockReadBuffer(short id, long processedBytes)
{
    short rc;
    
    /* --- Pass the number of bytes which have been read --- */
    rc = hs_wsmProcessedBytes (id,processedBytes);
    if (rc!=HS_SML_ERR_OK) return rc;

    /* --- Unlock Workspace --- */
    rc = hs_wsmUnlockH(id);
    if (rc!=HS_SML_ERR_OK) return rc;

  return HS_SML_ERR_OK;
}

/**
 * FUNCTION: 
 * Calls the encoding routines of the Encoder Module for a given Command Type
 * and Command Content
 *
 *
 * IN:        InstanceID_t              
 *            ID of the Instance
 *
 * IN:        ProtoElement_t              
 *            Type of the command (defined by the Proto Element Enumeration)
 *
 * IN:        VoidPtr_t              
 *            Content of the command to encode
 *
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if command has been encoded successfully
 */
static short hs_mgrCreateNextCommand(short id, SmlProtoElement_t cmdType, void* pContent)
{
  /* --- Definitions --- */
  InstanceInfoPtr_t   pInstanceInfo;               // pointer the the instance info structure for this id
  short                     rc;
  unsigned char*      pCurrentWritePosition;       // current Position from to which to write
  unsigned char*      pBeginPosition;              // saves the first position which has been written
  long                      freeSize;                    // size of free memory for writing


    /* --- Retrieve the corresponding instanceInfo structure --- */
      pInstanceInfo = (InstanceInfoPtr_t) hs_findInfo(id);

  if (pInstanceInfo==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;
  // %%% luz 2002-11-27: added NULL check as previously failed encoding will delete encoder
  if (pInstanceInfo->encoderState==NULL) return HS_SML_ERR_MGR_INVALID_INSTANCE_INFO;

  /* --- Get Write Access to the workspace --- */
  rc = hs_smlLockWriteBuffer(id, &pCurrentWritePosition, &freeSize);
  
  if (rc!=HS_SML_ERR_OK)  {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockWriteBuffer(id, (long)0);  
    return rc;
    }
  
    
  // Remember the position we have started writing    
  pBeginPosition=pCurrentWritePosition;             

 
  /* --- Call the encoder module --- */
  rc = hs_xltEncAppend(pInstanceInfo->encoderState, cmdType, pCurrentWritePosition+freeSize, pContent, &pCurrentWritePosition);
  
  if (rc!=HS_SML_ERR_OK) {
	  /* check for full buffer and call TransmitChunk */
	  if (rc == HS_SML_ERR_XLT_BUF_ERR) {
		  // first check wether callback is defined
      if (pInstanceInfo->callbacks->transmitChunkFunc!= NULL) {
			  // abort, unlock the buffer again without changing it's current position
        hs_smlUnlockWriteBuffer(id, (long)0);  
		    // call the callback
			  pInstanceInfo->callbacks->transmitChunkFunc(id,NULL);
		    // lock -> returns the amount of free buffer space
			  hs_smlLockWriteBuffer(id, &pCurrentWritePosition, &freeSize);
        pBeginPosition = pCurrentWritePosition;  
		    // now try again to encode and see wether we now have enough mem available			  
			  rc = hs_xltEncAppend(pInstanceInfo->encoderState, cmdType, pCurrentWritePosition+freeSize, pContent, &pCurrentWritePosition);
		    // if rc == HS_SML_ERR_OK continue else
          // return the errorcode
			  if( rc !=	HS_SML_ERR_OK)
			  {
          hs_smlUnlockWriteBuffer(id, (long)0);  
	        // Reset the encoder module (free the encoding object)
	        hs_xltEncReset(pInstanceInfo->encoderState);
          // this encoding job is over! reset instanceInfo pointer
          pInstanceInfo->encoderState=NULL;           
          return rc;
			  }
		  }
	  } else {
      // abort, unlock the buffer again without changing it's current position
      hs_smlUnlockWriteBuffer(id, (long)0);  
	    // Reset the encoder module (free the encoding object)
	    hs_xltEncReset(pInstanceInfo->encoderState);
      // this encoding job is over! reset instanceInfo pointer
      pInstanceInfo->encoderState=NULL;           
      return rc;
	  }
  }
  /* --- End Write Access to the workspace --- */
  rc = hs_smlUnlockWriteBuffer(id, (long)pCurrentWritePosition-(long)pBeginPosition);
  return rc;
}


/**
 * FUNCTION: 
 * Parses the header information at the beginning of an SyncML document.
 *
 * IN:              InstanceID
 *                  current InstanceID to pass to callback functions
 *
 * IN/OUT:          InstanceInfo
 *                  state information of the given InstanceID (decoder state will be changed)
 *
 * RETURN:          Return value of the Parser,            
 *                  HS_SML_ERR_OK if next command was handled successfully
 */
short hs_mgrProcessStartMessage(short id, InstanceInfoPtr_t pInstanceInfo)
{


  /* --- Definitions --- */ 
  short               rc;                          // Temporary return code saver
  SmlSyncHdrPtr_t     pContent=NULL;               // data of the command to process
  unsigned char*        pCurrentReadPosition ;        // current Position from which is read
  unsigned char*        pBeginPosition;              // saves the first position which has been reading
  long                        usedSize ;                    // size of used memory to be read 


  /* --- Get Read Access to the workspace --- */
  rc = hs_smlLockReadBuffer(id, &pCurrentReadPosition, &usedSize);

  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockReadBuffer(id, (long)0);  
    return rc;
    }
    
  // Remember the position we have started reading    
  pBeginPosition=pCurrentReadPosition;             

  /* --- Start new decoding sequence and pass returned decoder status structure to instanceInfo --- */
  rc = hs_xltDecInit(pInstanceInfo->instanceOptions->encoding, 
                  pCurrentReadPosition+usedSize-1, &pCurrentReadPosition, 
                  (XltDecoderPtr_t *)&(pInstanceInfo->decoderState), &pContent);

  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockReadBuffer(id, (long)0);  
  	// Reset the decoder module (free the decoding object)
	  hs_xltDecReset(pInstanceInfo->decoderState);
    // this decoding job is over! reset Instance Info pointer
    pInstanceInfo->decoderState=NULL;     
    // Reset the Workspace (the remaining unparsed document fragment will be lost)
	  hs_mgrResetWorkspace(id);
    return rc;
  }

  /* --- End Read Access to the workspace --- */
  rc = hs_smlUnlockReadBuffer(id, (long)pCurrentReadPosition-(long)pBeginPosition);
  if (rc!=HS_SML_ERR_OK) return rc;

  /* --- Perform callback to handle the beginning of a new message --- */
  if (pInstanceInfo->callbacks->startMessageFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
  rc=pInstanceInfo->callbacks->startMessageFunc(id, pInstanceInfo->userData, pContent); 

  if (rc != HS_SML_ERR_OK)
  {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockReadBuffer(id, (long)0);  
    // Reset the decoder module (free the decoding object)
    hs_xltDecReset(pInstanceInfo->decoderState);
    // this decoding job is over! reset Instance Info pointer
    pInstanceInfo->decoderState=NULL;     
 	  // Reset the Workspace (the remaining unparsed document fragment will be lost)
 	  hs_mgrResetWorkspace(id);
  }
 
  return rc;
}

/**
 * FUNCTION:  hs_mgrResetWorkspace
 * Reset the Workspace Buffer position to the beginning of the workspace
 * (clears all data in the buffer)
 *
 * IN:        InstanceID_t              
 *            ID of the Instance
 * RETURN:    Return value,            
 *            HS_SML_ERR_OK if successful
 */
short hs_mgrResetWorkspace (short id) {

    short rc;
    rc=hs_pim_wsmReset (id);
    return rc;
}



/**
 * FUNCTION: 
 * Parses the next Sync Command in the sync document.
 *
 * IN:              InstanceID
 *                  current InstanceID to pass to callback functions
 *
 * IN:              InstanceInfo
 *                  state information of the given InstanceID
 *
 * RETURN:          Return value of the Parser of the called application callback,            
 *                  HS_SML_ERR_OK if next command was handled successfully
 */
short hs_mgrProcessNextCommand(short id, InstanceInfoPtr_t pInstanceInfo)
{

  /* --- Definitions --- */ 
  short               rc;                          // Temporary return code saver
  SmlProtoElement_t   cmdType;                     // ID of the command to process
  void*           pContent=NULL;               // data of the command to process
  unsigned char*            pCurrentReadPosition;        // current Position from which is read
  unsigned char*            pBeginPosition;              // saves the first position which has been reading
  long           usedSize;                    // size of used memory to be read 
  unsigned char           final;                       // flag indicates last message within a package
  

  /* --- Get Read Access to the workspace --- */
  rc = hs_smlLockReadBuffer(id, &pCurrentReadPosition, &usedSize);
  
  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockReadBuffer(id, (long)0);  
    return rc;
    }

  // Remember the position we have started reading
  pBeginPosition=pCurrentReadPosition;             
  

  /* --- Parse next Command --- */ 
  rc = hs_xltDecNext(pInstanceInfo->decoderState, pCurrentReadPosition+usedSize, &pCurrentReadPosition, &cmdType, &pContent);

  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockReadBuffer(id, (long)0);  
  	// Reset the decoder module (free the decoding object)
	  hs_xltDecReset(pInstanceInfo->decoderState);
    // this decoding job is over! reset Instance Info pointer
    pInstanceInfo->decoderState=NULL;           
  	// Reset the Workspace (the remaining unparsed document fragment will be lost)
	  hs_mgrResetWorkspace(id);
    return rc;
  }

  /* --- End Read Access to the workspace --- */
  rc = hs_smlUnlockReadBuffer(id, (long)pCurrentReadPosition-(long)pBeginPosition);

  if (rc!=HS_SML_ERR_OK) {
    // abort, unlock the buffer again without changing it's current position
    hs_smlUnlockReadBuffer(id, (long)0);  
    return rc;
  }
  
  /* --- Did we reach end of synchronization document? --- */
  if (((XltDecoderPtr_t)(pInstanceInfo->decoderState))->finished!=0) {
    final = ((XltDecoderPtr_t)(pInstanceInfo->decoderState))->final; // flag is returned to appl. with callback
    rc=hs_xltDecTerminate(pInstanceInfo->decoderState);

    if (rc!=HS_SML_ERR_OK) 
	  {
      // abort, unlock the buffer again without changing it's current position
      hs_smlUnlockReadBuffer(id, (long)0);  
      // Reset the decoder module (free the decoding object)
	    hs_xltDecReset(pInstanceInfo->decoderState);
      // this decoding job is over! reset Instance Info pointer
      pInstanceInfo->decoderState=NULL;           
	    // Reset the Workspace (the remaining unparsed document fragment will be lost)
    	hs_mgrResetWorkspace(id);
      return rc;
	  }

    // this decoding job is over! reset Instance Info pointer
	  // (the decoding object itself has been freed by the decoder)
    pInstanceInfo->decoderState=NULL;
      
    // Call the callback for handling an message ending
    if (pInstanceInfo->callbacks->endMessageFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
    
    rc=pInstanceInfo->callbacks->endMessageFunc(id, pInstanceInfo->userData, final);
    return rc; 
  } 

  /* --- Dispatch parsed command (and call the applications command handler function)--- */
  switch (cmdType)
  {
    /* Handle ADD Command */
    case SML_PE_ADD:
      if (pInstanceInfo->callbacks->addCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->addCmdFunc (id, pInstanceInfo->userData, pContent); 
      break;/*lint !e527*/

    /* Handle ALERT Command */
    case SML_PE_ALERT:
      if (pInstanceInfo->callbacks->alertCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->alertCmdFunc (id, pInstanceInfo->userData, pContent); 
      break;/*lint !e527*/
        
    /* Handle DELETE Command */
    case SML_PE_DELETE:
      if (pInstanceInfo->callbacks->deleteCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->deleteCmdFunc (id, pInstanceInfo->userData, pContent);  
      break;/*lint !e527*/

    /* Handle PUT Command */
    case SML_PE_PUT:
      if (pInstanceInfo->callbacks->putCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->putCmdFunc (id, pInstanceInfo->userData, pContent);   
      break;/*lint !e527*/

    /* Handle GET Command */
    case SML_PE_GET:
      if (pInstanceInfo->callbacks->getCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->getCmdFunc (id, pInstanceInfo->userData, pContent);     
      break;/*lint !e527*/
        
    #ifdef HS_MAP_RECEIVE      
    /* Handle MAP Command */
    case SML_PE_MAP:
      if (pInstanceInfo->callbacks->mapCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->mapCmdFunc (id, pInstanceInfo->userData, pContent);     
      break;/*lint !e527*/
    #endif
      
    #ifdef HS_RESULT_RECEIVE
    /* Handle RESULTS Command */
    case SML_PE_RESULTS:
       //SCI_TRACE_MID_PIM("MMIPIM chenxiang hs_mgrProcessNextCommand SML_PE_RESULTS");
      if (pInstanceInfo->callbacks->resultsCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->resultsCmdFunc (id, pInstanceInfo->userData, pContent); 
      break;/*lint !e527*/
    #endif

    /* Handle STATUS Command */
    case SML_PE_STATUS:
      if (pInstanceInfo->callbacks->statusCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->statusCmdFunc (id, pInstanceInfo->userData, pContent);  
      break;/*lint !e527*/

    /* Handle START SYNC Command */
    case SML_PE_SYNC_START:
      if (pInstanceInfo->callbacks->startSyncFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->startSyncFunc (id, pInstanceInfo->userData, pContent);  
      break;/*lint !e527*/

    /* Handle END SYNC Command */
    case SML_PE_SYNC_END:
      if (pInstanceInfo->callbacks->endSyncFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->endSyncFunc (id, pInstanceInfo->userData);              
      break;/*lint !e527*/

    /* Handle REPLACE Command */
    case SML_PE_REPLACE:
      if (pInstanceInfo->callbacks->replaceCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->replaceCmdFunc (id, pInstanceInfo->userData, pContent);  
      break;/*lint !e527*/

    /* Handle Final Flag */
    case SML_PE_FINAL:
      //syncml_core_message:"MMIPIM  $$ final call back called!$$"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSMLCORE_1451_112_2_18_3_25_55_232,(uint8*)"");
      if (pInstanceInfo->callbacks->FinalFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->FinalFunc ();  
      break;/*lint !e527*/


    #ifdef HS_SEARCH_RECEIVE  /* these API calls are NOT included in the Toolkit lite version */

    /* Handle SEARCH Command */
    case SML_PE_SEARCH:
      if (pInstanceInfo->callbacks->searchCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->searchCmdFunc (id, pInstanceInfo->userData, pContent);  
      break;/*lint !e527*/
    #endif

    #ifdef HS_SEQUENCE_RECEIVE
    /* Handle START SEQUENCE Command */
    case SML_PE_SEQUENCE_START:
      if (pInstanceInfo->callbacks->startSequenceFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->startSequenceFunc (id, pInstanceInfo->userData, pContent); 
      break;/*lint !e527*/

    /* Handle END SEQUENCE Command */
    case SML_PE_SEQUENCE_END:
      if (pInstanceInfo->callbacks->endSequenceFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->endSequenceFunc (id, pInstanceInfo->userData);             
      break;/*lint !e527*/
    #endif

    #ifdef HS_ATOMIC_RECEIVE

    /* Handle START ATOMIC Command */
    case SML_PE_ATOMIC_START:
      if (pInstanceInfo->callbacks->startAtomicFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->startAtomicFunc (id, pInstanceInfo->userData, pContent);
      break;/*lint !e527*/
        
    /* Handle END ATOMIC Command */
    case SML_PE_ATOMIC_END:
      if (pInstanceInfo->callbacks->endAtomicFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->endAtomicFunc (id, pInstanceInfo->userData);            
      break;/*lint !e527*/
    #endif
      
    #ifdef HS_COPY_RECEIVE

    /* Handle COPY Command */
    case SML_PE_COPY:
      if (pInstanceInfo->callbacks->copyCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->copyCmdFunc (id, pInstanceInfo->userData, pContent);    
      break;/*lint !e527*/
    #endif

    #ifdef HS_EXEC_RECEIVE

    /* Handle EXEC Command */
    case SML_PE_EXEC:
      if (pInstanceInfo->callbacks->execCmdFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
      return pInstanceInfo->callbacks->execCmdFunc (id, pInstanceInfo->userData, pContent);    
      break;/*lint !e527*/
        
    #endif
    
    /* Handle ERROR DETECTED  */
    //case SML_PE_ERROR:
    //  if (pInstanceInfo->callbacks->handleErrorFunc==NULL) return HS_SML_ERR_COMMAND_NOT_HANDLED;
    //  return pInstanceInfo->callbacks->handleErrorFunc (id, pInstanceInfo->userData);        
    //  break;

    /* --- Invalid Command Element --- */
    default:
      return HS_SML_ERR_XLT_INVAL_PROTO_ELEM;   
      break;/*lint !e527*/
  } // switch 
}

