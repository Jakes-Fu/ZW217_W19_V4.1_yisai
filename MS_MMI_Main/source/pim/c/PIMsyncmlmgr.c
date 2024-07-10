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
文件名称:PIMsyncmlmgr.c
文件描述:syncml manager,syncml管理模块。
文件说明:syncml管理模块，主要负责与smlXX等syncml内核模块交互
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
  #ifdef WIN32
   #include "std_header.h"
  #endif
  #define _PIMSYNCMLMGR_C_
  
  #include "../h/PIMsyncmlmgr.h"
  #include "../h/PIMlibmem.h"
  #include "../h/PIMDataSyncTask.h"
  #include "../h/PIMbase64.h"
  #include "../h/PIMmd5.h"
  #include "../h/PIMsmlerr.h"

   #include"../h/mmipim.h"
  //#include "msg.h"

  /*  数据结构声明*/
  extern task_relay_info_type*     hs_task_relay_info;
  extern status_cmd_queue_type*  hs_status_cmd_que;
  
 // extern SmlNewMapItemListPtr_t  hs_globlemapItemList;

  extern PIM_T g_pim;/*lint !e526 */ //chenxiang_auth 20071214
  
  /*  全局变量声明*/
#include "mmi_pim_trc.h"
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for 大对象
static BOOLEAN hs_large_object_proc_flag=FALSE; //标志变量

static uint32 hs_lcontent_proc_len=0;//当前处理完毕的长度

static uint32 hs_content_total_len=0;//总长度

static SmlItemListPtr_t hs_sml_item_ptr=NULL; //对象指针

static void * hs_item_content_ptr=NULL;//对象内容指针，如果该指针不正确，释放item_ptr的时候会错误

#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */

  /*  功能函数*/
/******************************************************************* 
** 函数名:hs_calc_b64_cred
** 功能描述：计算b64 authentication所用的cred值
** 输  出: 
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
PUBLIC void hs_calc_b64_cred(char* creddata, unsigned long cbLength); // chenxiang_auth 20071215

/******************************************************************* 
** 函数名:hs_calc_md5_cred
** 功能描述：计算md5 authentication所用的cred值
** 输  出: 
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
PUBLIC char*  hs_calc_md5_cred(char* creddata); // chenxiang_auth 20071215

#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for 大对象
static BOOLEAN hs_syncml_get_large_object_proc_flag(void)
{  
   return  hs_large_object_proc_flag;
}

void hs_syncml_set_large_object_proc_flag(BOOLEAN  proc_flag)
{  
   hs_large_object_proc_flag=proc_flag;
}
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */

/******************************************************************* 
** 函数名:hs_calc_b64_cred
** 功能描述：计算b64 authentication所用的cred值
** 输  出: 
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
PUBLIC void hs_calc_b64_cred(char* creddata, unsigned long cbLength)
{
    /**************************************/
    /* Basic authentication:              */
    /* calculate a base64-encoding of the */
    /* string <user>:<password>           */
    /**************************************/
#if 0
    char abSave[4];
    unsigned long cbLength;
    int cbCopied=0;
    unsigned long cbDigestSize = 80;
    unsigned long cbOffset = 0; // <<<<< ????
    char szSeparator [2] = { ':', 0 };
    abSave [0] = abSave [1] = abSave [2] = abSave [3] = '\0';

    cbLength = (unsigned long) xppStrlen (hs_task_relay_info->syncml_pim_username);
    cbCopied += hs_base64Encode ((unsigned char*)creddata, cbDigestSize,
                     (unsigned char*) hs_task_relay_info->syncml_pim_username,
                     &cbLength, &cbOffset, 0, (unsigned char *)abSave);

    cbLength = 1;
    cbCopied += hs_base64Encode ((unsigned char*)creddata+cbCopied, cbDigestSize-cbCopied,
                     (unsigned char*) szSeparator, &cbLength, &cbOffset, 1, (unsigned char *)abSave);

    cbLength = (unsigned long) xppStrlen (hs_task_relay_info->syncml_pim_password);
    cbCopied += hs_base64Encode ((unsigned char*)creddata+cbCopied, cbDigestSize-cbCopied,
                     (unsigned char*) hs_task_relay_info->syncml_pim_password,
                     &cbLength, &cbOffset, 1, (unsigned char *)abSave);

    creddata[cbCopied] = '\0';
#else // chenxiang_auth
    char abSave[4];
    int cbCopied=0;
    unsigned long cbDigestSize = 80;
    unsigned long cbOffset = 0; 
    unsigned char* outputdata;
    abSave [0] = abSave [1] = abSave [2] = abSave [3] = '\0';
    if (0 == cbLength)
    {
        creddata[0] = '\0'; // chenxiang 20071225
        return;
    }
    outputdata = xppMalloc(cbLength * 2 + 4);
    cbCopied = hs_base64Encode ((unsigned char*)outputdata, cbDigestSize,
                     (unsigned char*) creddata,
                     &cbLength, &cbOffset, 1, (unsigned char *)abSave);
    outputdata[cbCopied] = '\0';
    xppMemcpy(creddata, outputdata, cbCopied);
    creddata[cbCopied] = '\0';
    xppFree(outputdata);
#endif
}

/******************************************************************* 
** 函数名:hs_calc_md5_cred
** 功能描述：计算md5 authentication所用的cred值
** 输  出: 
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
PUBLIC char*  hs_calc_md5_cred(char* creddata)
{
    /*******************************************/
    /* Digest authentication:                  */
    /* compute the digest according to RFC2617 */
    /*******************************************/
#if 0
    MD5_CTX Md5Ctx;
    HASH HA1;
    //syncml_core_message:"MMIPIM $$ enter hs_calc_md5_cred "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_182_112_2_18_3_25_56_242,(uint8*)"");
    //syncml_core_message:"MMIPIM $$ username=%s,pas_word=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_183_112_2_18_3_25_56_243,(uint8*)"ss",hs_task_relay_info->syncml_pim_username,hs_task_relay_info->syncml_pim_password);
    PIMMD5Init(&Md5Ctx);
    PIMMD5Update(&Md5Ctx, hs_task_relay_info->syncml_pim_username, xppStrlen(hs_task_relay_info->syncml_pim_username));
    PIMMD5Update(&Md5Ctx, ":", 1);
    PIMMD5Update(&Md5Ctx, hs_task_relay_info->syncml_pim_password, xppStrlen(hs_task_relay_info->syncml_pim_password));
    PIMMD5Update(&Md5Ctx, ":", 1);  
    PIMMD5Update(&Md5Ctx, hs_task_relay_info->syncml_pim_nextnonce, xppStrlen(hs_task_relay_info->syncml_pim_nextnonce));
    PIMMD5Final(HA1, &Md5Ctx);
    PIMCvtHex(HA1, creddata);
    //hs_smlLibMemcpy(creddata,"27 E6 1A 89 4A 83 43 7B 19 4F B8 79 53 B1 BD 83 ",(long)sizeof(InstanceInfo_t));
    //syncml_core_message:"MMIPIM $$ creddata=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_193_112_2_18_3_25_56_244,(uint8*)"s",creddata);
#else // chenxiang_auth 20071215
    MD5_CTX Md5Ctx;
    HASH HA1;
    //int i = 0;
    PIMMD5Init(&Md5Ctx);
    PIMMD5Update(&Md5Ctx, creddata, xppStrlen(creddata));
    PIMMD5Final(HA1, &Md5Ctx);
    xppMemcpy(creddata, HA1, HASHLEN);
    creddata[HASHLEN] = 0;
    return creddata;
#endif
}


/******************************************************************* 
** 函数名:hs_syncml_init
** 功能描述：同步初始化
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
   short hs_syncml_init()
  {
  	SmlOptions_t options;
	
       /* Set Toolkit options structure */
       options.defaultPrintFunc = NULL;
       options.maxWorkspaceAvailMem = HS_MAX_WSM_BUFFER_SIZE;
	   
       /* Initialize SyncML Toolkit */
       return hs_smlInit(&options);
  }

/******************************************************************* 
** 函数名:hs_syncml_initance
** 功能描述：同步初始化worksapce
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_initance(short * pID, char* workspacename)
{
    SmlCallbacks_t callbacks;
    SmlInstanceOptions_t options;

    /* Set instance */
    options.encoding = hs_task_relay_info->encodetype;
    options.workspaceName = workspacename;
    options.workspaceSize = HS_ONE_WORK_SPACE_SIZE;
   
    /* Allocate for callbacks structure */
    callbacks.startMessageFunc = hs_myHandleStartMessage;
    callbacks.endMessageFunc = hs_myHandleEndMessage;
	
    callbacks.startSyncFunc = hs_myHandleStartSync;
    callbacks.endSyncFunc = hs_myHandleEndSync;
	
#ifdef HS_ATOMIC_RECEIVE
    callbacks.startAtomicFunc = hs_myHandlestartAtomic;
    callbacks.endAtomicFunc = hs_myHandleendAtomic;
#else
    callbacks.startAtomicFunc = NULL;
    callbacks.endAtomicFunc = NULL;
#endif

#ifdef HS_SEQUENCE_RECEIVE
    callbacks.startSequenceFunc = hs_myHandlestartSequence;
    callbacks.endSequenceFunc = hs_myHandleendSequence;
#else
    callbacks.startSequenceFunc = NULL;
    callbacks.endSequenceFunc = NULL;
#endif

    callbacks.addCmdFunc = hs_myHandleAdd;
    callbacks.alertCmdFunc = hs_myHandlealert;
    callbacks.deleteCmdFunc = hs_myHandledelete;
    callbacks.getCmdFunc = hs_myHandleget;
    callbacks.putCmdFunc = hs_myHandleput;

#ifdef HS_MAP_RECEIVE
    callbacks.mapCmdFunc = hs_myHandlemap;
#else
    callbacks.mapCmdFunc = NULL;
#endif

#ifdef HS_RESULT_RECEIVE
    callbacks.resultsCmdFunc = hs_myHandleresults;
#else
    callbacks.resultsCmdFunc = NULL;
#endif

    callbacks.statusCmdFunc = hs_myHandlestatus;
    callbacks.replaceCmdFunc = hs_myHandlereplace;

#ifdef HS_COPY_RECEIVE
    callbacks.copyCmdFunc = hs_myHandlecopy;
#else
    callbacks.copyCmdFunc = NULL;
#endif

#ifdef HS_EXEC_RECEIVE
    callbacks.execCmdFunc = hs_myHandleexec;
#else
    callbacks.execCmdFunc = NULL;
#endif

#ifdef HS_SEARCH_RECEIVE
    callbacks.searchCmdFunc = hs_myHandlesearch;
#else
    callbacks.searchCmdFunc = NULL;
#endif

    callbacks.FinalFunc=hs_myHandleFinal;

    callbacks.handleErrorFunc = hs_myHandlehandleError;
    callbacks.transmitChunkFunc = hs_myHandletransmitChunk;    
    
   
    /* Initialize the SyncML instance; set up callbacks and options */
    /* and get an instance Id "id" */
    return hs_smlInitInstance(&callbacks, &options, NULL, pID);
  }

/******************************************************************* 
** 函数名:hs_syncml_startmessage
** 功能描述：建立同步sync header
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_startmessage(short id)
{
    SmlSyncHdr_t hdr;
    SmlSource_t source;
    SmlTarget_t target;
    SmlCred_t  cred;
    SmlMetInfMetInf_t   metainfo;
    SmlMetInfMetInf_t   metainfo2;
    char creddata[80];
    char* sessionid;
    char* messageid;
    short returnval=HS_SML_ERR_OK; // chenxiang_memory 20080220
    char * max_msg_size=NULL;
    
   //syncml_core_message:"MMIPIM $$ hs_syncml_startmessage  anth_type=%d "
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_338_112_2_18_3_25_57_245,(uint8*)"d",hs_task_relay_info->authtype);
    hs_smlLibMemset(&metainfo, 0, sizeof(metainfo));
    hs_smlLibMemset(&metainfo2, 0, sizeof(metainfo2));
    hs_smlLibMemset(&cred, 0, sizeof(cred)); // chenxiang_memory 20080220
    /* Create SyncML proto element for message header */
    hdr.elementType = SML_PE_HEADER;
    hdr.version = hs_smlString2Pcdata(__FILE__, __LINE__, "1.1");
    hdr.proto = hs_smlString2Pcdata(__FILE__, __LINE__, "SyncML/1.1");

    sessionid=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->sessionID);
    messageid=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->messageID);
    hdr.sessionID = hs_smlString2Pcdata(__FILE__, __LINE__, sessionid);
    hdr.msgID = hs_smlString2Pcdata(__FILE__, __LINE__, messageid);
    hdr.flags = 0;

    target.locURI = hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_server_name);
    target.locName=NULL;
    hdr.target = &target;

    source.locURI = hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_client_imei);
    source.locName=NULL;
    hdr.source = &source;
    hdr.respURI=NULL;

    if((hs_task_relay_info->authtype==AUTH_B64)&&(hs_task_relay_info->authored==0) 
        && (0 != (xppStrlen(hs_task_relay_info->syncml_pim_username) + xppStrlen(hs_task_relay_info->syncml_pim_password))))
    {
        metainfo.type=hs_smlString2Pcdata(__FILE__, __LINE__, "syncml:auth-basic");
#if 0
        hs_calc_b64_cred(creddata);
#else // chenxiang_auth 20071215
        if (0 != xppStrlen(hs_task_relay_info->syncml_pim_password))
            sprintf(creddata, "%s:%s", hs_task_relay_info->syncml_pim_username, hs_task_relay_info->syncml_pim_password);
        else
            sprintf(creddata, "%s", hs_task_relay_info->syncml_pim_username);
        hs_calc_b64_cred(creddata, xppStrlen(creddata));
        //syncml_core_message:"MMIPIM $$ hs_syncml_startmessage  creddata=%s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_374_112_2_18_3_25_57_246,(uint8*)"s",creddata);
#endif
        cred.data = hs_smlString2Pcdata(__FILE__, __LINE__, creddata);
        cred.meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, &metainfo);
        hdr.cred = &cred;
    }
    else if ((hs_task_relay_info->authtype==AUTH_MD5) 
        && (0 != (xppStrlen(hs_task_relay_info->syncml_pim_username) + xppStrlen(hs_task_relay_info->syncml_pim_password))))
    {
        metainfo.type=hs_smlString2Pcdata(__FILE__, __LINE__, "syncml:auth-md5");
#if 0
        hs_calc_md5_cred(creddata);
#else // chenxiang_auth 20071215
        if (0 != xppStrlen(hs_task_relay_info->syncml_pim_password))
            sprintf(creddata, "%s:%s", hs_task_relay_info->syncml_pim_username, hs_task_relay_info->syncml_pim_password);
        else
            sprintf(creddata, "%s", hs_task_relay_info->syncml_pim_username);
        hs_calc_b64_cred(hs_calc_md5_cred(creddata), HASHLEN);
        xppStrcat(creddata, ":");
        xppStrcat(creddata, hs_task_relay_info->syncml_pim_nextnonce);
        hs_calc_b64_cred(hs_calc_md5_cred(creddata), HASHLEN);
        //syncml_core_message:"MMIPIM $$ hs_syncml_startmessage  creddata=%s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_395_112_2_18_3_25_57_247,(uint8*)"s",creddata);
#endif
        cred.data = hs_smlString2Pcdata(__FILE__, __LINE__, creddata);
        cred.meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, &metainfo);
        hdr.cred = &cred;
    }
    else
    {
        hdr.cred = NULL;
    }
	
    max_msg_size=hs_smlunsignedInt2String(__FILE__, __LINE__, PIM_MAX_MSG_SIZE);//add by wangxiaolin 2009.05.22
    metainfo2.maxmsgsize=hs_smlString2Pcdata(__FILE__, __LINE__, max_msg_size);
    
    hdr.meta = hs_smlmeta2extPcdata(__FILE__, __LINE__, &metainfo2);
    returnval = hs_smlStartMessageExt(id, &hdr, SML_VERS_1_1);


	hs_smlFreePcdata( __FILE__, __LINE__, hdr.version);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.proto);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.sessionID);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.msgID);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.respURI);
       hs_smlLibFree(hdr.meta);
       hs_smlFreePcdata( __FILE__, __LINE__, metainfo2.maxmsgsize);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.source->locURI);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.source->locName);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.target->locURI);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.target->locName);
    if (NULL != hdr.cred)
    {
       hs_smlLibFree((hdr.cred->meta));
       hs_smlFreePcdata( __FILE__, __LINE__, metainfo.type);
	hs_smlFreePcdata( __FILE__, __LINE__, hdr.cred->data);
    }
       hs_smlLibFree(sessionid);
       hs_smlLibFree(messageid);
       hs_smlLibFree(max_msg_size);
       
    return returnval;
}

 /******************************************************************* 
** 函数名:hs_syncml_StartSync
** 功能描述：建立同步sync body
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
#define PIM_REPORT_FREE_MEM
short hs_syncml_StartSync(short id)
{
    char* temp;
    SmlSync_t sync;
    SmlSource_t source;
    SmlTarget_t target;
    SmlMetInfMetInf_t  meta;
    SmlMetInfMem_t  mem;
    short returnval=HS_SML_ERR_OK; // chenxiang_memory 20080220
  
    hs_smlLibMemset(&meta, 0 , (long)sizeof(SmlMetInfMetInf_t));
    /* Start sync cmd */
    sync.elementType = SML_PE_SYNC_START;
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    sync.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    target.locURI = hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_server_phonebook_db);
    target.locName = NULL;
    sync.target = &target;
    source.locURI = hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_client_phonebook_db);
    source.locName = NULL;
    sync.source = &source;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    }
    else
    {
        target.locURI = hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_server_calendar_db);
        target.locName = NULL;
        sync.target = &target;
        source.locURI = hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_client_calendar_db);
        source.locName = NULL;
        sync.source = &source;
    }
#endif /* HS_PIM_VCAL */

    sync.cred = NULL;
#ifdef PIM_REPORT_FREE_MEM//del by wangxiaolin   2009.11.19
    meta.mem=&mem;
    mem.shared=NULL;
    mem.free=hs_myGetMSFreeMemery(); 
    mem.freeid=hs_myGetPhoneBookFreeIDs(); 
#else
        meta.mem=NULL;
#endif
    sync.meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, &meta);
    //modify by wangxiaolin 2007.12.06
    if(SYNC_TYPE_S2WAY==hs_task_relay_info->synctype)
    {
        sync.noc=hs_myGetNumberofAllItem();
    }
    else if(SYNC_TYPE_2WAY==hs_task_relay_info->synctype)
    {
       sync.noc=hs_myGetNumberofChanges();
    }
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for 单向同步&  刷新同步
    else if(SYNC_TYPE_1WAYFCLI==hs_task_relay_info->synctype)
    {
       sync.noc=hs_myGetNumberofChanges();
    }
    else if(SYNC_TYPE_RFCLI==hs_task_relay_info->synctype)
    {
       sync.noc=hs_myGetNumberofAllItem();
    }
    else if(SYNC_TYPE_1WAYFSER==hs_task_relay_info->synctype)
    {
       sync.noc=0;
    }
    else if(SYNC_TYPE_RFSER==hs_task_relay_info->synctype)
    {
       sync.noc=0;
    }
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
    else
    {
        // TODO: 同步类型错误处理 /*-wangxiaolin 2009.7.9 -*/
    }
    
    sync.flags = 0; // chenxiang_20080308
    hs_task_relay_info->cmdID++;
    returnval = hs_smlStartSync(id, &sync);
#if 1 // chenxiang_memory 20080220
    hs_smlLibFree(temp);
    hs_smlFreePcdata( __FILE__, __LINE__, sync.cmdID);
    hs_smlLibFree(sync.meta);
#ifdef PIM_REPORT_FREE_MEM//del by wangxiaolin   2009.11.19
    hs_smlFreePcdata( __FILE__, __LINE__, mem.free);
    hs_smlFreePcdata( __FILE__, __LINE__, mem.freeid);
#endif

    hs_smlFreePcdata( __FILE__, __LINE__, sync.source->locURI);
    hs_smlFreePcdata( __FILE__, __LINE__, sync.target->locURI);

    hs_smlFreePcdata( __FILE__, __LINE__, sync.noc);

#endif
    return returnval;
}


 /******************************************************************* 
** 函数名:hs_syncml_AlertCmd
** 功能描述：添加同步命令ALERT
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_AlertCmd(short id,char* cmd)
 {
     SmlAlert_t  alert;
     SmlItemList_t  itemlist;
     SmlItem_t    item;
     SmlMetInfAnchor_t   metinfoanchor;
	 
     SmlMetInfMetInf_t   metainfo;
     SmlTarget_t  target;
     SmlSource_t source;
     char* temp;
    short returnval=HS_SML_ERR_OK; // chenxiang_memory 20080220

     hs_smlLibMemset(&item, 0, sizeof(item));
	 
     itemlist.next=NULL;
     itemlist.item=&item;
     itemlist.item->target=&target;
     itemlist.item->source=&source;
	 
     hs_smlLibMemset(&alert, 0, sizeof(alert));
     hs_smlLibMemset(&metainfo, 0, sizeof(metainfo));
     hs_smlLibMemset(&target, 0, sizeof(target));
     hs_smlLibMemset(&source, 0, sizeof(source));
     hs_smlLibMemset(&metinfoanchor, 0, sizeof(metinfoanchor));
     metainfo.anchor=&metinfoanchor;
     
     /*Alert command*/
     temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
     alert.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
     hs_smlLibFree(temp);
     hs_task_relay_info->cmdID++;
   
     alert.data=hs_smlString2Pcdata(__FILE__, __LINE__, cmd);

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
     itemlist.item->target->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_server_phonebook_db);
     itemlist.item->source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_client_phonebook_db);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
     }
    else
    {
         itemlist.item->target->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_server_calendar_db);
         itemlist.item->source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_client_calendar_db);
    }
#endif /* HS_PIM_VCAL */

     metainfo.anchor->last=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->last_anchor);
     metainfo.anchor->next=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->next_anchor);
     
     itemlist.item->meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, &metainfo);
     alert.itemList=&itemlist;
     returnval = hs_smlAlertCmd(id, &alert);	 
#if 1 // chenxiang_memory 20080220
	hs_smlFreePcdata( __FILE__, __LINE__, alert.cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, alert.data);
	hs_smlFreePcdata( __FILE__, __LINE__, itemlist.item->target->locURI);
	hs_smlFreePcdata( __FILE__, __LINE__, itemlist.item->source->locURI);
	hs_smlFreePcdata( __FILE__, __LINE__, metainfo.anchor->last);
	hs_smlFreePcdata( __FILE__, __LINE__, metainfo.anchor->next);
	hs_smlLibFree((itemlist.item->meta));
#endif
    return returnval;
 }


  /******************************************************************* 
** 函数名:hs_syncml_PutCmd
** 功能描述：添加同步命令PUT
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_PutCmd(short id)
{
    char* temp;
    SmlPut_t put;
    SmlMetInfMetInf_t   metainfo;
    SmlDevInfDevInf_t   deviceinfo;
    SmlItemList_t  itemlist;
    SmlItem_t    item;
    SmlTarget_t target;
    SmlDevInfDatastoreList_t   datastorelist;
    SmlDevInfDatastore_t   datastore;
    SmlDevInfXmit_t rxref,txref;
    SmlDevInfSyncCap_t  synccap;
    SmlPcdataList_t     pcdata1,pcdata2;
    short returnval=HS_SML_ERR_OK; // chenxiang_memory 20080220
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.5.29  
    SmlPcdataList_t     pcdata3,pcdata4,pcdata5,pcdata6;
#endif

/*********************Make the CTCAP****************************/
    int i;
    SmlDevInfCtcapList_t   ctcaplist;
    SmlDevInfCTCap_t      ctcap;
    SmlDevInfCTDataPropList_t  proplist[9];
    SmlDevInfCTDataProp_t       prop[9];
    SmlDevInfCTData_t              propname[9];
    SmlDevInfCTDataList_t         paramlist[9];
    SmlDevInfCTData_t              paramlistdata[9];

    //property no.1 use variable
    SmlPcdataList_t  valuenumlist11;    
    //property no.2 use variable
    SmlPcdataList_t  valuenumlist21;
    //property no.3 use variable
    SmlPcdataList_t  valuenumlist31;
    //property no.5 use variable
#if 0//del by wangxiaolin   2009.12.20
    SmlDevInfCTDataList_t paramlistdata52;
#endif
    SmlDevInfCTData_t    paramdata52;
    //property no.6 use variable
    SmlDevInfCTDataList_t   paramlistdata62;
    SmlDevInfCTDataList_t   paramlistdata63;
    SmlDevInfCTDataList_t   paramlistdata64;
    SmlDevInfCTData_t    paramdata62;
    SmlDevInfCTData_t    paramdata63;
    SmlDevInfCTData_t    paramdata64;
    MMIPIM_UA_INFO_T custom_ua_info={0};
     //syncml_core_message:"MMIPIM  ^_^ Begin hs_syncml_PutCmd g_pim.m_ua_type=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_681_112_2_18_3_25_57_248,(uint8*)"d",g_pim.m_ua_type);

    for(i=0;i<9;i++)
    {
          hs_smlLibMemset(&propname[i], 0, sizeof(propname[0]));
          hs_smlLibMemset(&paramlistdata[i], 0, sizeof(paramlistdata[0]));
          proplist[i].data=&prop[i];
          proplist[i].next=&proplist[i+1];
	   prop[i].prop=&propname[i];
	   prop[i].param=&paramlist[i];
	   paramlist[i].data=&paramlistdata[i];
    }
    proplist[8].next=NULL;  //only 9 properties
    ctcaplist.data=&ctcap;
    ctcaplist.next=NULL;      //only one data base use is vcard,no vcanlendar
    ctcap.cttype=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcard");
    ctcap.prop=&proplist[0];
    
   //property no.1
    propname[0].name=hs_smlString2Pcdata(__FILE__, __LINE__, "BEGIN");
    propname[0].valenum=&valuenumlist11;
    valuenumlist11.data=hs_smlString2Pcdata(__FILE__, __LINE__, "VCARD");
    valuenumlist11.next=NULL;
    prop[0].param=NULL;
   //property no.2
    propname[1].name=hs_smlString2Pcdata(__FILE__, __LINE__, "END");
    propname[1].valenum=&valuenumlist21;
    valuenumlist21.data=hs_smlString2Pcdata(__FILE__, __LINE__, "VCARD");
    valuenumlist21.next=NULL;
    prop[1].param=NULL;
    //property no.3
    propname[2].name=hs_smlString2Pcdata(__FILE__, __LINE__, "VERSION");
    propname[2].valenum=&valuenumlist31;
    valuenumlist31.data=hs_smlString2Pcdata(__FILE__, __LINE__, "2.1");
    valuenumlist31.next=NULL;
    prop[2].param=NULL;
    //property no.4
    propname[3].name=hs_smlString2Pcdata(__FILE__, __LINE__, "N");
    prop[3].param=NULL;
    //property no.5
    hs_smlLibMemset(&paramdata52, 0, sizeof(paramdata52));
    propname[4].name=hs_smlString2Pcdata(__FILE__, __LINE__, "ORG");
#if 0//del by wangxiaolin  2009.8.26
    paramlistdata[4].name=hs_smlString2Pcdata(__FILE__, __LINE__, "ORGNAZITIONAL UNIT");
    paramlist[4].next=&paramlistdata52;
    paramlistdata52.data=&paramdata52;
    paramdata52.name=hs_smlString2Pcdata(__FILE__, __LINE__, "TITLE");
    paramdata52.valenum=NULL;
    paramlistdata52.next=NULL;
#else
    prop[4].param=NULL;
#endif 
    //property no.6
    hs_smlLibMemset(&paramdata62, 0, sizeof(paramdata62));
    hs_smlLibMemset(&paramdata63, 0, sizeof(paramdata63));
    hs_smlLibMemset(&paramdata64, 0, sizeof(paramdata64));
    propname[5].name=hs_smlString2Pcdata(__FILE__, __LINE__, "TEL");
	
    paramlistdata[5].name=hs_smlString2Pcdata(__FILE__, __LINE__, "CELL");
    paramlist[5].next=&paramlistdata62;
	
    paramlistdata62.data=&paramdata62;
    paramdata62.name=hs_smlString2Pcdata(__FILE__, __LINE__, "WORK");
    paramlistdata62.next=&paramlistdata63;
	
    paramlistdata63.data=&paramdata63;
    paramdata63.name=hs_smlString2Pcdata(__FILE__, __LINE__, "HOME");
    paramlistdata63.next=&paramlistdata64;

    paramlistdata64.data=&paramdata64;
    paramdata64.name=hs_smlString2Pcdata(__FILE__, __LINE__, "FAX");
    paramlistdata64.next=NULL;
    //property no.7
    propname[6].name=hs_smlString2Pcdata(__FILE__, __LINE__, "EMAIL");
    paramlist[6].data->name=hs_smlString2Pcdata(__FILE__, __LINE__, "INTERNET");
    paramlist[6].next=NULL;
    //property no.8
    //propname[7].name=hs_smlString2Pcdata(__FILE__, __LINE__, "ADR");
    //paramlist[7].data->name=hs_smlString2Pcdata(__FILE__, __LINE__, "WORK");
    //paramlist[7].next=NULL;
    
    //property no.9
    propname[7].name=hs_smlString2Pcdata(__FILE__, __LINE__, "TITLE");
    prop[7].param=NULL;

    //property no.10
    propname[8].name=hs_smlString2Pcdata(__FILE__, __LINE__, "NOTE");
    prop[8].param=NULL;
    
  /**************************************************************/  
		
    hs_smlLibMemset(&put, 0, sizeof(put));
    hs_smlLibMemset(&metainfo, 0, sizeof(metainfo));
    hs_smlLibMemset(&item, 0, sizeof(item));
    hs_smlLibMemset(&target, 0, sizeof(target));
    hs_smlLibMemset(&deviceinfo, 0, sizeof(deviceinfo));
    hs_smlLibMemset(&datastore, 0, sizeof(datastore));
    hs_smlLibMemset(&rxref, 0, sizeof(rxref));
    hs_smlLibMemset(&txref, 0, sizeof(txref));
	
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    put.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    hs_smlLibFree(temp);
    hs_task_relay_info->cmdID++;
    if(hs_task_relay_info->encodetype==SML_XML)
    {
           metainfo.type = hs_smlString2Pcdata(__FILE__, __LINE__, "application/vnd.syncml-devinf+xml");
    }else if(hs_task_relay_info->encodetype==SML_WBXML)
    {
           metainfo.type = hs_smlString2Pcdata(__FILE__, __LINE__, "application/vnd.syncml-devinf+wbxml");
    }
    put.meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, &metainfo);
		
    itemlist.next=NULL;
    itemlist.item=&item;
    itemlist.item->source=&target;
    itemlist.item->source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, "./devinf11");

    deviceinfo.verdtd= hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_VERDTD);

    MMIPIM_GetUAInfo(g_pim.m_ua_type,&custom_ua_info);
    deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.man);
    deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.oem);
    deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.mod);
//#ifdef HS_PIM_UA_SETTING
//    if (1 == g_pim.m_ua_type)
//    {
//        deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MAN_HS);
//        deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_OEM_HS);
//        deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MOD_HS);
//    }
//    else if (2 == g_pim.m_ua_type)
//    {
//        deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.man);
//        deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.oem);
//        deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.mod);
//    }
//    else
//#endif //end of HS_PIM_UA_SETTING
//    {
//        deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MAN);
//        deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_OEM);
//        deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MOD);
//     }


    deviceinfo.fwv=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_FWV);
    deviceinfo.swv=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SWV);
    deviceinfo.hwv=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_HWV);
    deviceinfo.devid=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_DEVID);
    deviceinfo.devtyp=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_DEVTYP);

    datastorelist.data=&datastore;
    datastorelist.next=NULL;
    deviceinfo.datastore=&datastorelist;
    datastore.sourceref=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SOURCEREF);
    datastore.displayname=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_DISPNAME);
    datastore.maxguidsize=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_MAXGUIDESIZE);

    datastore.rxpref=&rxref;
    datastore.txpref=&txref;
    rxref.cttype=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_CTTYPE);
    rxref.verct=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_VERCT_VCARD);
    txref.cttype=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_CTTYPE);
    txref.verct=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_VERCT_VCARD);

   
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.5.29  
    synccap.synctype=&pcdata1;
    pcdata1.next=&pcdata2;
    pcdata2.next=&pcdata3;
    pcdata3.next=&pcdata4;
    pcdata4.next=&pcdata5;
    pcdata5.next=&pcdata6;
    pcdata6.next=NULL;
    pcdata1.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_TWOWAY);     //two way sync
    pcdata2.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_STWOWAY);  //slow two way sync
    pcdata3.data=hs_smlString2Pcdata(__FILE__, __LINE__, "3");     //client one way sync
    pcdata4.data=hs_smlString2Pcdata(__FILE__, __LINE__, "4");  //client ref sync
    pcdata5.data=hs_smlString2Pcdata(__FILE__, __LINE__, "5");     //server one way sync
    pcdata6.data=hs_smlString2Pcdata(__FILE__, __LINE__, "6");  //server ref sync
    datastore.synccap=&synccap;
#else
    synccap.synctype=&pcdata1;
    pcdata1.next=&pcdata2;
    pcdata2.next=NULL;
    pcdata1.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_TWOWAY);     //two way sync
    pcdata2.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_STWOWAY);  //slow two way sync
    datastore.synccap=&synccap;
#endif //

    deviceinfo.ctcap=&ctcaplist;

    deviceinfo.ext=NULL;
    deviceinfo.flags=SmlDevInfNOfM_f; // chenxiang_pim_ui 20080306  SupportNumberOfChanges
    
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //add by wangxiaolin 2009.07.15
    deviceinfo.flags|=SmlDevInfLargeObject_f;
#endif

    itemlist.item->data=hs_smldevinfo2extPcdata(__FILE__, __LINE__, &deviceinfo);
    put.itemList=&itemlist;
     returnval = hs_smlPutCmd(id, &put);
#if 1 // chenxiang_memory 20080220
	hs_smlFreePcdata( __FILE__, __LINE__, ctcap.cttype);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[0].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[1].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[2].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[3].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[4].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[5].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[6].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[7].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[8].name);
    //  hs_smlFreePcdata( __FILE__, __LINE__, propname[9].name);
#if 0//del by wangxiaolin  2009.8.26
	hs_smlFreePcdata( __FILE__, __LINE__, paramlistdata[4].name);
#endif
	hs_smlFreePcdata( __FILE__, __LINE__, paramlistdata[5].name);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist11.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist21.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist31.data);
#if 0//del by wangxiaolin  2009.8.26
	hs_smlFreePcdata( __FILE__, __LINE__, paramdata52.name);
#endif
	hs_smlFreePcdata( __FILE__, __LINE__, paramdata62.name);
	hs_smlFreePcdata( __FILE__, __LINE__, paramdata63.name);
	hs_smlFreePcdata( __FILE__, __LINE__, paramdata64.name);
	hs_smlFreePcdata( __FILE__, __LINE__, paramlist[6].data->name);
	//hs_smlFreePcdata( __FILE__, __LINE__, paramlist[7].data->name);
	hs_smlFreePcdata( __FILE__, __LINE__, put.cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, metainfo.type);
	hs_smlLibFree(put.meta);
	hs_smlFreePcdata( __FILE__, __LINE__, itemlist.item->source->locURI);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.verdtd);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.man);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.oem);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.mod);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.fwv);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.swv);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.hwv);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.devid);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.devtyp);
	hs_smlFreePcdata( __FILE__, __LINE__, datastore.sourceref);
	hs_smlFreePcdata( __FILE__, __LINE__, datastore.displayname);
	hs_smlFreePcdata( __FILE__, __LINE__, datastore.maxguidsize);
	hs_smlFreePcdata( __FILE__, __LINE__, rxref.cttype);
	hs_smlFreePcdata( __FILE__, __LINE__, rxref.verct);
	hs_smlFreePcdata( __FILE__, __LINE__, txref.cttype);
	hs_smlFreePcdata( __FILE__, __LINE__, txref.verct);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata1.data);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata2.data);
	hs_smlLibFree((itemlist.item->data));
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.5.29  
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata3.data);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata4.data);
       hs_smlFreePcdata( __FILE__, __LINE__, pcdata5.data);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata6.data);
#endif 
#endif
    return returnval;
}
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
/*==========================================================
 * author    : wangzhefeng      
 * function  : hs_syncml_PutVCALCmd
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/10/2009 
 ==========================================================*/
short hs_syncml_PutVCALCmd(short id)
{
    char* temp;
    SmlPut_t put;
    SmlMetInfMetInf_t   metainfo;
    SmlDevInfDevInf_t   deviceinfo;
    SmlItemList_t  itemlist;
    SmlItem_t    item;
    SmlTarget_t target;
    SmlDevInfDatastoreList_t   datastorelist;
    SmlDevInfDatastore_t   datastore;
    SmlDevInfXmit_t rxref,txref;
    SmlDevInfSyncCap_t  synccap;
    SmlPcdataList_t     pcdata1,pcdata2;
    short returnval=HS_SML_ERR_OK; // chenxiang_memory 20080220
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.5.29  
    SmlPcdataList_t     pcdata3,pcdata4,pcdata5,pcdata6;
#endif

/*********************Make the CTCAP****************************/
    int i;
    SmlDevInfCtcapList_t   ctcaplist;
    SmlDevInfCTCap_t      ctcap;
    SmlDevInfCTDataPropList_t  proplist[14];
    SmlDevInfCTDataProp_t       prop[14];
    SmlDevInfCTData_t              propname[14];
#if 0//del by wangxiaolin   2009.12.20
    SmlDevInfCTDataList_t         paramlist[14];
#endif
    //SmlDevInfCTData_t              paramlistdata[14];

    //property no.1 use variable
    SmlPcdataList_t  valuenumlist11;    
    SmlPcdataList_t  valuenumlist12;    
    SmlPcdataList_t  valuenumlist13;    
    //property no.2 use variable
    SmlPcdataList_t  valuenumlist21;
    SmlPcdataList_t  valuenumlist22;    
    SmlPcdataList_t  valuenumlist23;    
    //property no.3 use variable
    SmlPcdataList_t  valuenumlist31;
#if 0//del by wangxiaolin   2009.12.20
    SmlDevInfCTDataList_t   paramlistdata62;
    SmlDevInfCTDataList_t   paramlistdata63;
    SmlDevInfCTDataList_t   paramlistdata64;
    SmlDevInfCTData_t    paramdata62;
    SmlDevInfCTData_t    paramdata63;
    SmlDevInfCTData_t    paramdata64;
#endif
    MMIPIM_UA_INFO_T custom_ua_info={0};
    //syncml_core_message:"MMIPIM  ^_^ Begin hs_syncml_PutVCALCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1001_112_2_18_3_25_58_249,(uint8*)"");

    for(i=0;i<14;i++)
    {
          hs_smlLibMemset(&propname[i], 0, sizeof(propname[0]));
          //hs_smlLibMemset(&paramlistdata[i], 0, sizeof(paramlistdata[0]));
          proplist[i].data=&prop[i];
          proplist[i].next=&proplist[i+1];
	   prop[i].prop=&propname[i];
	   prop[i].param=NULL;//&paramlist[i];
	   //paramlist[i].data=&paramlistdata[i];
    }
    proplist[13].next=NULL;  //only 14 properties
    ctcaplist.data=&ctcap;
    ctcaplist.next=NULL;      //only one data base use is vcard,no vcanlendar
    ctcap.cttype=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcalendar");
    ctcap.prop=&proplist[0];
    
   //property no.1
    propname[0].name=hs_smlString2Pcdata(__FILE__, __LINE__, "BEGIN");
    propname[0].valenum=&valuenumlist11;
    valuenumlist11.data=hs_smlString2Pcdata(__FILE__, __LINE__, "VCALENDAR");
    valuenumlist11.next=&valuenumlist12;
    
    valuenumlist12.data = hs_smlString2Pcdata(__FILE__, __LINE__, "VEVENT");
    valuenumlist12.next=&valuenumlist13;
    
    valuenumlist13.data = hs_smlString2Pcdata(__FILE__, __LINE__, "VTODO");
    valuenumlist13.next=NULL;
    //prop[0].param=NULL;
   //property no.2
    propname[1].name=hs_smlString2Pcdata(__FILE__, __LINE__, "END");
    propname[1].valenum=&valuenumlist21;
    valuenumlist21.data=hs_smlString2Pcdata(__FILE__, __LINE__, "VCALENDAR");
    valuenumlist21.next=&valuenumlist22;
    
    valuenumlist22.data = hs_smlString2Pcdata(__FILE__, __LINE__, "VEVENT");
    valuenumlist22.next=&valuenumlist23;
    
    valuenumlist23.data = hs_smlString2Pcdata(__FILE__, __LINE__, "VTODO");
    valuenumlist23.next=NULL;
    //prop[1].param=NULL;
    //property no.3
    propname[2].name=hs_smlString2Pcdata(__FILE__, __LINE__, "VERSION");
    propname[2].valenum=&valuenumlist31;
    valuenumlist31.data=hs_smlString2Pcdata(__FILE__, __LINE__, "1.0");
    valuenumlist31.next=NULL;
    //prop[2].param=NULL;
    //property no.4
    propname[3].name=hs_smlString2Pcdata(__FILE__, __LINE__, "STATUS");
   // prop[3].param=NULL;
    //property no.5
    propname[4].name=hs_smlString2Pcdata(__FILE__, __LINE__, "PRIORITY");
    //prop[4].param=NULL;

    //property no.6
    propname[5].name=hs_smlString2Pcdata(__FILE__, __LINE__, "CLASS");
    //prop[5].param=NULL;
    //property no.7
    propname[6].name=hs_smlString2Pcdata(__FILE__, __LINE__, "CATEGORIES");
    //prop[6].param=NULL;
    //property no.8
    propname[7].name=hs_smlString2Pcdata(__FILE__, __LINE__, "DESCRIPTION");
    //prop[7].param=NULL;
    
    //property no.9
    propname[8].name=hs_smlString2Pcdata(__FILE__, __LINE__, "SUMMARY");
    //prop[8].param=NULL;

    //property no.10
    propname[9].name=hs_smlString2Pcdata(__FILE__, __LINE__, "DTSTART");
    //prop[9].param=NULL;
    //property no.11
    propname[10].name=hs_smlString2Pcdata(__FILE__, __LINE__, "DTEND");
    //prop[10].param=NULL;
    //property no.12
    propname[11].name=hs_smlString2Pcdata(__FILE__, __LINE__, "DUE");
    //prop[11].param=NULL;
    //property no.13
    propname[12].name=hs_smlString2Pcdata(__FILE__, __LINE__, "COMPLETED");
    //prop[12].param=NULL;
    //property no.14
    propname[13].name=hs_smlString2Pcdata(__FILE__, __LINE__, "RRULE");
    //prop[13].param=NULL;
    
  /**************************************************************/  
		
    hs_smlLibMemset(&put, 0, sizeof(put));
    hs_smlLibMemset(&metainfo, 0, sizeof(metainfo));
    hs_smlLibMemset(&item, 0, sizeof(item));
    hs_smlLibMemset(&target, 0, sizeof(target));
    hs_smlLibMemset(&deviceinfo, 0, sizeof(deviceinfo));
    hs_smlLibMemset(&datastore, 0, sizeof(datastore));
    hs_smlLibMemset(&rxref, 0, sizeof(rxref));
    hs_smlLibMemset(&txref, 0, sizeof(txref));
	
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    put.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    hs_smlLibFree(temp);
    hs_task_relay_info->cmdID++;
    if(hs_task_relay_info->encodetype==SML_XML)
    {
           metainfo.type = hs_smlString2Pcdata(__FILE__, __LINE__, "application/vnd.syncml-devinf+xml");
    }
    else if(hs_task_relay_info->encodetype==SML_WBXML)
    {
           metainfo.type = hs_smlString2Pcdata(__FILE__, __LINE__, "application/vnd.syncml-devinf+wbxml");
    }
    put.meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, &metainfo);
		
    itemlist.next=NULL;
    itemlist.item=&item;
    itemlist.item->source=&target;
    itemlist.item->source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, "./devinf11");

    deviceinfo.verdtd= hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_VERDTD);

    MMIPIM_GetUAInfo(g_pim.m_ua_type,&custom_ua_info);
    deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.man);
    deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.oem);
    deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.mod);
//#ifdef HS_PIM_UA_SETTING  //modify by wangxiaolin 2009.12.21
//    if (1 == g_pim.m_ua_type)
//    {
//        deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MAN_HS);
//        deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_OEM_HS);
//        deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MOD_HS);
//    }
//    else if (2 == g_pim.m_ua_type)
//    {
//        MMIPIM_CUSTOM_UA_INFO_T custom_ua_info={0};
//        MMIPIM_GetCustomUAInfo(&custom_ua_info);
//        deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.man);
//        deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.oem);
//        deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, custom_ua_info.mod);
//    }
//    else 
//#endif  //end of HS_PIM_UA_SETTING
//    {
//        deviceinfo.man=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MAN);
//        deviceinfo.oem=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_OEM);
//        deviceinfo.mod=hs_smlString2Pcdata(__FILE__, __LINE__, DEVICE_INFO_MOD);
//     }

    deviceinfo.fwv=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_FWV);
    deviceinfo.swv=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SWV);
    deviceinfo.hwv=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_HWV);
    deviceinfo.devid=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_DEVID);
/*Start of  wangzhefeng 2009.9.14 for g */
    deviceinfo.devtyp=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_DEVTYP_VCAL);
/*End   of  wangzhefeng 2009.9.14 for g */

    datastorelist.data=&datastore;
    datastorelist.next=NULL;
    deviceinfo.datastore=&datastorelist;
    //wangzhefeng mod 20090910
    datastore.sourceref=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SOURCEREF_VCAL);
    datastore.displayname=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_DISPNAME_VCAL); 
    //wangzhefeng mod end
    datastore.maxguidsize=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_MAXGUIDESIZE);  
    datastore.rxpref=&rxref;
    datastore.txpref=&txref;
    //wangzhefeng mod 20090910
    rxref.cttype=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_CTTYPE_VCAL);
    rxref.verct=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_VERCT_VCAL);     
    txref.cttype=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_CTTYPE_VCAL);   
    txref.verct=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_VERCT_VCAL); 
    //wangzhefeng mod end

   
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.5.29  
    synccap.synctype=&pcdata1;
    pcdata1.next=&pcdata2;
    pcdata2.next=&pcdata3;
    pcdata3.next=&pcdata4;
    pcdata4.next=&pcdata5;
    pcdata5.next=&pcdata6;
    pcdata6.next=NULL;
    pcdata1.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_TWOWAY);     //two way sync
    pcdata2.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_STWOWAY);  //slow two way sync
    pcdata3.data=hs_smlString2Pcdata(__FILE__, __LINE__, "3");     //client one way sync
    pcdata4.data=hs_smlString2Pcdata(__FILE__, __LINE__, "4");  //client ref sync
    pcdata5.data=hs_smlString2Pcdata(__FILE__, __LINE__, "5");     //server one way sync
    pcdata6.data=hs_smlString2Pcdata(__FILE__, __LINE__, "6");  //server ref sync
    datastore.synccap=&synccap;
#else
    synccap.synctype=&pcdata1;
    pcdata1.next=&pcdata2;
    pcdata2.next=NULL;
    pcdata1.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_TWOWAY);     //two way sync
    pcdata2.data=hs_smlString2Pcdata(__FILE__, __LINE__, HS_DEVICE_INFO_SYNCTYPE_STWOWAY);  //slow two way sync
    datastore.synccap=&synccap;
#endif //

    deviceinfo.ctcap=&ctcaplist;

    deviceinfo.ext=NULL;
    deviceinfo.flags=SmlDevInfNOfM_f; // chenxiang_pim_ui 20080306  SupportNumberOfChanges
    
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //add by wangxiaolin 2009.07.15
    deviceinfo.flags|=SmlDevInfLargeObject_f;
#endif

    itemlist.item->data=hs_smldevinfo2extPcdata(__FILE__, __LINE__, &deviceinfo);
    put.itemList=&itemlist;
    //syncml_core_message:"MMIPIM  ^_^ Begin hs_smlPutCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1204_112_2_18_3_25_58_250,(uint8*)"");
     returnval = hs_smlPutCmd(id, &put);
    //syncml_core_message:"MMIPIM  ^_^ End hs_smlPutCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1206_112_2_18_3_25_58_251,(uint8*)"");
	hs_smlFreePcdata( __FILE__, __LINE__, ctcap.cttype);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[0].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[1].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[2].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[3].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[4].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[5].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[6].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[7].name);
	hs_smlFreePcdata( __FILE__, __LINE__, propname[8].name);
       hs_smlFreePcdata( __FILE__, __LINE__, propname[9].name);
       hs_smlFreePcdata( __FILE__, __LINE__, propname[10].name);
       hs_smlFreePcdata( __FILE__, __LINE__, propname[11].name);
       hs_smlFreePcdata( __FILE__, __LINE__, propname[12].name);
       hs_smlFreePcdata( __FILE__, __LINE__, propname[13].name);
	//hs_smlFreePcdata( __FILE__, __LINE__, paramlistdata[5].name);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist13.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist12.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist11.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist23.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist22.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist21.data);
	hs_smlFreePcdata( __FILE__, __LINE__, valuenumlist31.data);
    
	hs_smlFreePcdata( __FILE__, __LINE__, put.cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, metainfo.type);
	hs_smlLibFree(put.meta);
	hs_smlFreePcdata( __FILE__, __LINE__, itemlist.item->source->locURI);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.verdtd);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.man);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.oem);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.mod);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.fwv);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.swv);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.hwv);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.devid);
	hs_smlFreePcdata( __FILE__, __LINE__, deviceinfo.devtyp);
	hs_smlFreePcdata( __FILE__, __LINE__, datastore.sourceref);
	hs_smlFreePcdata( __FILE__, __LINE__, datastore.displayname);
	hs_smlFreePcdata( __FILE__, __LINE__, datastore.maxguidsize);
	hs_smlFreePcdata( __FILE__, __LINE__, rxref.cttype);
	hs_smlFreePcdata( __FILE__, __LINE__, rxref.verct);
	hs_smlFreePcdata( __FILE__, __LINE__, txref.cttype);
	hs_smlFreePcdata( __FILE__, __LINE__, txref.verct);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata1.data);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata2.data);
	hs_smlLibFree((itemlist.item->data));
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.5.29  
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata3.data);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata4.data);
       hs_smlFreePcdata( __FILE__, __LINE__, pcdata5.data);
	hs_smlFreePcdata( __FILE__, __LINE__, pcdata6.data);
#endif 
    return returnval;
}

#endif /* HS_PIM_VCAL */

/******************************************************************* 
** 函数名:hs_syncml_GetCmd
** 功能描述：添加同步命令GET
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_GetCmd(short id)
{
    char* temp;
    SmlGet_t get;
    SmlMetInfMetInf_t   metainfo;
    SmlItemList_t  itemlist;
    SmlItem_t    item;
    SmlTarget_t target;
    short returnval=HS_SML_ERR_OK; // chenxiang_memory 20080220
	
    hs_smlLibMemset(&get, 0, sizeof(get));
    hs_smlLibMemset(&metainfo, 0, sizeof(metainfo));
    hs_smlLibMemset(&item, 0, sizeof(item));
    hs_smlLibMemset(&target, 0, sizeof(target));
     
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    get.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    hs_smlLibFree(temp);
    hs_task_relay_info->cmdID++;
    if(hs_task_relay_info->encodetype==SML_XML)
    {
           metainfo.type = hs_smlString2Pcdata(__FILE__, __LINE__, "application/vnd.syncml-devinf+xml");
    }else if(hs_task_relay_info->encodetype==SML_WBXML)
    {
           metainfo.type = hs_smlString2Pcdata(__FILE__, __LINE__, "application/vnd.syncml-devinf+wbxml");
    }
    get.meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, &metainfo);
		
    itemlist.next=NULL;
    itemlist.item=&item;
    itemlist.item->target=&target;
    itemlist.item->target->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, "./devinf11");

    get.itemList=&itemlist;
    
    returnval = hs_smlGetCmd(id, &get);
#if 1 // chenxiang_memory 20080220
	hs_smlFreePcdata( __FILE__, __LINE__, get.cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, metainfo.type);
	hs_smlLibFree(get.meta);
	hs_smlFreePcdata( __FILE__, __LINE__, itemlist.item->target->locURI);
#endif
    return returnval;
}
  
 /******************************************************************* 
** 函数名:hs_syncml_AddCmd
** 功能描述：添加同步命令ADD
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for 大对象
short hs_syncml_AddCmd(short id) // chenxiang 20071124
{
    char* temp;
    SmlAdd_t add;
    SmlMetInfMetInf_t meta;
    short returnval=HS_SML_ERR_OK;
    SmlItemPtr_t item=NULL;
    SmlItemListPtr_t itemList=NULL;
    SmlSourcePtr_t source=NULL;
    char * size_str=NULL;

    //
    if(FALSE==hs_syncml_get_large_object_proc_flag())
    {
        add.itemList=NULL;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
        if(SYNC_MODE_PB == g_pim.m_sync_mode)
        {
#endif /* HS_PIM_VCAL */
        add.itemList=hs_myGetAddedPBItems((short)hs_task_relay_info->synctype);//add by wangxiaolin 2009.05.29
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
        }
        else
        {
            add.itemList=hs_myGetAddedVCALItems((short)hs_task_relay_info->synctype);
        }
#endif /* HS_PIM_VCAL */
        if(NULL!=add.itemList)
        {
            hs_content_total_len=add.itemList->item->data->length;
        }
        else
        {
             return returnval;
        }
    }
    else//正在处理大对象数据，不需要获得新的列表
    {
        add.itemList=hs_sml_item_ptr;
        //syncml_message:"MMIPIM hs_syncml_AddCmd,now the large object is proc 1 ..."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1365_112_2_18_3_25_59_252,(uint8*)"");
    }
    
    if(NULL==add.itemList) /*there is no items need to be added.*/
    {
      return returnval;
    }
    
    /* Add cmd */
    add.elementType = SML_PE_ADD;
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    hs_task_relay_info->cmdID++;
    add.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    add.flags = 0;
    add.cred = NULL;
    
    
    if((add.itemList->item->flags==0x0400)&&(hs_content_total_len>PIM_MAX_VCARD_DATA_LEN))
    {
        //大对象第一个包
        if(0==hs_lcontent_proc_len)
        {
            //syncml_message:"MMIPIM hs_syncml_AddCmd,the first chunk of the large object, total_len=%d,cur_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1387_112_2_18_3_25_59_253,(uint8*)"dd",hs_content_total_len,hs_lcontent_proc_len);
            hs_smlLibMemset(&meta, 0, (long)sizeof(SmlMetInfMetInf_t));
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.14
            if(SYNC_MODE_PB == g_pim.m_sync_mode)
            {
#endif /* HS_PIM_VCAL */
            meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcard");
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.14
            }
            else
            {
                meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcalendar");
            }
#endif /* HS_PIM_VCAL */
            size_str=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_content_total_len);
            meta.size=hs_smlString2Pcdata(__FILE__, __LINE__, size_str);
            add.meta =hs_smlmeta2extPcdata(__FILE__, __LINE__, &meta);
            
            add.itemList->item->data->length=PIM_MAX_VCARD_DATA_LEN;
            returnval=hs_smlAddCmd(id, &add);
            hs_lcontent_proc_len=PIM_MAX_VCARD_DATA_LEN;
            hs_item_content_ptr=add.itemList->item->data->content;
            add.itemList->item->data->content=(void*)((char*)add.itemList->item->data->content+add.itemList->item->data->length);
            //syncml_message:"MMIPIM ,hs_syncml_AddCmd ,first ,content_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1410_112_2_18_3_25_59_254,(uint8*)"d",add.itemList->item->data->length);
            //hs_myhttplogs((char *)(add.itemList->item->data->content),BASE64_BUF_LEN);
            hs_syncml_set_large_object_proc_flag(TRUE);
            hs_sml_item_ptr=add.itemList;
            
            hs_smlFreePcdata( __FILE__, __LINE__, meta.type);
            hs_smlLibFree(add.meta);
            hs_smlLibFree(size_str);
        }
        else
        {
            add.meta=NULL;
            //剩余的数据一次可以发送完毕，也即这是大对象的最后一个包
            if(hs_content_total_len-hs_lcontent_proc_len<PIM_MAX_VCARD_DATA_LEN)/*lint !e737*/
            {
               //syncml_message:"MMIPIM hs_syncml_AddCmd,the last chunk of the large object, total_len=%d,cur_len=%d"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1425_112_2_18_3_25_59_255,(uint8*)"dd",hs_content_total_len,hs_lcontent_proc_len);

                add.itemList->item->data->length=hs_content_total_len-hs_lcontent_proc_len;/*lint !e737*/
                //将指针后移，发送后面的数据
                //add.itemList->item->data->content=(void*)((char*)add.itemList->item->data->content+hs_lcontent_proc_len);
                //不发送</Moredata>
                add.itemList->item->flags=0;
                returnval=hs_smlAddCmd(id, &add);
                //syncml_message:"MMIPIM ,hs_syncml_AddCmd ,last ,content_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1433_112_2_18_3_25_59_256,(uint8*)"d",add.itemList->item->data->length);
                //hs_myhttplogs((char *)(add.itemList->item->data->content),BASE64_BUF_LEN);
                add.itemList->item->data->content=hs_item_content_ptr;
                hs_syncml_set_large_object_proc_flag(FALSE);
                hs_item_content_ptr=NULL;
                hs_sml_item_ptr=NULL;
                hs_lcontent_proc_len=0;
                hs_content_total_len=0;
            }
            else //大对象中间的包
            {
                //syncml_message:"MMIPIM hs_syncml_AddCmd,the midle chunk of the large object, total_len=%d,cur_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1444_112_2_18_3_25_59_257,(uint8*)"dd",hs_content_total_len,hs_lcontent_proc_len);

                add.itemList->item->data->length=PIM_MAX_VCARD_DATA_LEN;
                //将指针后移，发送后面的数据
                returnval=hs_smlAddCmd(id, &add);
                hs_lcontent_proc_len+=PIM_MAX_VCARD_DATA_LEN;
                add.itemList->item->data->content=(void*)((char*)add.itemList->item->data->content+add.itemList->item->data->length);
                //syncml_message:"MMIPIM ,hs_syncml_AddCmd ,middle ,content_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1451_112_2_18_3_25_59_258,(uint8*)"d",add.itemList->item->data->length);
                //hs_myhttplogs((char *)(add.itemList->item->data->content),BASE64_BUF_LEN);
                hs_syncml_set_large_object_proc_flag(TRUE); 
                hs_sml_item_ptr=add.itemList;
            }
        }
    }
    else
    {
        //设置大对象处理的控制变量
        hs_syncml_set_large_object_proc_flag(FALSE);
        hs_lcontent_proc_len=0;
        hs_sml_item_ptr=NULL;
        
        //虽然被视为大对象，但因为该包只发送一个对象，可以一次发送完毕
        if(add.itemList->item->flags==0x0400) //保护作用而已
        {
            add.itemList->item->flags=0;
        }
        
        hs_smlLibMemset(&meta, 0, (long)sizeof(SmlMetInfMetInf_t));
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.14
        if(SYNC_MODE_PB == g_pim.m_sync_mode)
        {
#endif /* HS_PIM_VCAL */
        meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcard");
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.14
        }
        else
        {
            meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcalendar");
        }
#endif /* HS_PIM_VCAL */
        add.meta =hs_smlmeta2extPcdata(__FILE__, __LINE__, &meta);
        returnval=hs_smlAddCmd(id, &add);
        hs_smlFreePcdata( __FILE__, __LINE__, meta.type);
        hs_smlLibFree(add.meta);
    }
    //本次数据没有发送完，暂时不能释放itemList列表的内存
    if(TRUE==hs_syncml_get_large_object_proc_flag())
    {
        //syncml_message:"MMIPIM hs_syncml_AddCmd,now the large object is proc 2 ..."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1492_112_2_18_3_25_59_259,(uint8*)"");
    }
    else//一次数据包发送完毕
    {
        //free drynamic memorys.
        while(add.itemList!=NULL)
        {
          itemList=add.itemList;
          add.itemList=itemList->next;
          item=itemList->item;
          source=item->source;
          if (NULL != item)
          {
              hs_smlFreePcdata( __FILE__, __LINE__, item->meta);
              hs_smlFreePcdata( __FILE__, __LINE__, item->data);
          }
          if (NULL != source)
          {
            hs_smlFreePcdata( __FILE__, __LINE__, source->locName);
            hs_smlFreePcdata( __FILE__, __LINE__, source->locURI);
          }
          hs_smlLibFree(source);
          source=NULL;
          hs_smlLibFree(item);
          item=NULL;
          hs_smlLibFree(itemList);
          itemList=NULL;
        }
    }
    hs_smlFreePcdata( __FILE__, __LINE__, add.cmdID);   
    hs_smlLibFree(temp); // chenxiang_memory 20080312
    return returnval;

}
#else
short hs_syncml_AddCmd(short id) // chenxiang 20071124
{
    char* temp;
    SmlAdd_t add;
    SmlMetInfMetInf_t meta;
    short returnval=HS_SML_ERR_OK;
    SmlItemPtr_t item=NULL;
    SmlItemListPtr_t itemList=NULL;
    SmlSourcePtr_t source=NULL;
   
    add.itemList=NULL;
    
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    add.itemList=hs_myGetAddedPBItems((short)hs_task_relay_info->synctype); // chenxiang 20071124
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        add.itemList=hs_myGetAddedVCALItems((short)hs_task_relay_info->synctype);
    }
#endif /* HS_PIM_VCAL */
    
    if(NULL==add.itemList) /*there is no items need to be added.*/
    {
      return returnval;
    }

    /* Add cmd */
    add.elementType = SML_PE_ADD;
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    hs_task_relay_info->cmdID++;
    add.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    add.flags = 0;
    add.cred = NULL;
    hs_smlLibMemset(&meta, 0, (long)sizeof(SmlMetInfMetInf_t));
    meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcard");
    add.meta =hs_smlmeta2extPcdata(__FILE__, __LINE__, &meta);

    returnval=hs_smlAddCmd(id, &add);

    //free drynamic memorys.
    while(add.itemList!=NULL)
    {
      itemList=add.itemList;
      add.itemList=itemList->next;
      item=itemList->item;
      source=item->source;

      if (NULL != item)
      {
          hs_smlFreePcdata( __FILE__, __LINE__, item->meta);
          hs_smlFreePcdata( __FILE__, __LINE__, item->data);
      }
      if (NULL != source)
      {
        hs_smlFreePcdata( __FILE__, __LINE__, source->locName);
        hs_smlFreePcdata( __FILE__, __LINE__, source->locURI);
      }

      hs_smlLibFree(source);
      source=NULL;
      hs_smlLibFree(item);
      item=NULL;
      hs_smlLibFree(itemList);
      itemList=NULL;
    }

    hs_smlFreePcdata( __FILE__, __LINE__, add.cmdID);
    hs_smlFreePcdata( __FILE__, __LINE__, meta.type);
    hs_smlLibFree(add.meta);
    hs_smlLibFree(temp); // chenxiang_memory 20080312

    return returnval;

}
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
 
 /******************************************************************* 
** 函数名:hs_syncml_DeleteCmd
** 功能描述：添加同步命令Delete
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_DeleteCmd(short id)
{
    char* temp;
    SmlDelete_t del;
    short returnval=HS_SML_ERR_OK;
    SmlItemListPtr_t itemList=NULL;
    SmlItemPtr_t item=NULL;
    SmlSourcePtr_t source=NULL;

    del.itemList=NULL;
    //syncml_core_message:"MMIPIM  $$enter hs_syncml_DeleteCmd $$"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1625_112_2_18_3_25_59_260,(uint8*)"");
	
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    del.itemList=hs_myGetDeletedPBItems();
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        del.itemList=hs_myGetDeletedVCALItems();
    }
#endif /* HS_PIM_VCAL */
    //syncml_core_message:"MMIPIM  $$ del.itemList=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1639_112_2_18_3_25_59_261,(uint8*)"d",del.itemList);
    if(NULL==del.itemList) /*there is no items to be deleted*/
    {
      return returnval;
    }
    
    /* Delete cmd */
    del.elementType = SML_PE_DELETE;
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    hs_task_relay_info->cmdID++;
    del.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    del.flags = 0;
    del.cred = NULL;
    del.meta=NULL;

    returnval=hs_smlDeleteCmd(id, &del);

    //free drynamic memorys.
    while(del.itemList!=NULL)
    {
      itemList=del.itemList;
      del.itemList=itemList->next;
      item=itemList->item;
      source=item->source;
#if 1 // chenxiang_memory 20080219
      if (NULL != item)
      {
          hs_smlFreePcdata( __FILE__, __LINE__, item->meta);
          hs_smlFreePcdata( __FILE__, __LINE__, item->data);
      }
      if (NULL != source)
      {
        hs_smlFreePcdata( __FILE__, __LINE__, source->locName);
        hs_smlFreePcdata( __FILE__, __LINE__, source->locURI);
      }
#endif
      hs_smlLibFree(source);
      source=NULL;
      hs_smlLibFree(item);
      item=NULL;
      hs_smlLibFree(itemList);
      itemList=NULL;
    }
#if 1 // chenxiang_memory 20080220
      hs_smlFreePcdata( __FILE__, __LINE__, del.cmdID);
      hs_smlLibFree(temp); // chenxiang_memory 20080312
#endif
    return returnval;
}
 
/******************************************************************* 
** 函数名:hs_syncml_StatusCmd
** 功能描述：添加同步响应命令Status,Status的响应已经
**                            由callback函数生成了status cmd queue，这里处
**                            理这个queue。
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_StatusCmd(short id)
{
       char* temp;
       short rc=HS_SML_ERR_OK;
	long counter;
	status_element_type* queueptr;
	queueptr=hs_status_cmd_que->queue;
	for(counter=0;counter<hs_status_cmd_que->totalnumber;counter++)
	{
	      temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
	    
	      queueptr->status->cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
	      hs_task_relay_info->cmdID++;
	      rc|=hs_smlStatusCmd(id, queueptr->status);
	      queueptr=queueptr->next;
             hs_smlLibFree(temp); // chenxiang_memory 20080303
	}
	rc|=hs_myFreestatusofCQ();
       return rc;
}


 /******************************************************************* 
** 函数名:hs_syncml_MapCmd
** 功能描述：添加同步命令map
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_MapCmd(short id)
 {
    char* temp;
    SmlMap_t map;
    SmlSource_t source;
    SmlTarget_t  target;
    short returnval=HS_SML_ERR_OK;
    SmlMapItemPtr_t item=NULL;
    SmlMapItemListPtr_t itemList=NULL;
    SmlSourcePtr_t sourceptr=NULL;
    SmlTargetPtr_t  targetptr=NULL;
   
    map.mapItemList=NULL;
    
    map.mapItemList=hs_ConvertNewMapListToMapList(hs_myGetMapedItems());

    if(NULL==map.mapItemList) /*there is no items need to be added.*/
    {
      return returnval;
    }

    /* Map cmd */
    map.elementType = SML_PE_MAP;
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    hs_task_relay_info->cmdID++;
    map.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    map.target=&target;
    map.source=&source;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    target.locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_server_phonebook_db);
    target.locName=NULL;
    source.locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_client_phonebook_db);
    source.locName=NULL;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    }
    else
    {
        target.locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_server_calendar_db);
        target.locName=NULL;
        source.locURI=hs_smlString2Pcdata(__FILE__, __LINE__, hs_task_relay_info->syncml_pim_client_calendar_db);
        source.locName=NULL;
    }
#endif /* HS_PIM_VCAL */
    map.cred = NULL;
    map.meta=NULL;

    returnval=hs_smlMapCmd(id, &map);

    //free drynamic memorys.
    while(map.mapItemList!=NULL)
    {
      itemList=map.mapItemList;
      map.mapItemList=itemList->next;
      item=itemList->mapItem;
      sourceptr=item->source;
      targetptr=item->target;
#if 1 // chenxiang_memory 20080219
      if (NULL != sourceptr)
      {
        hs_smlFreePcdata( __FILE__, __LINE__, sourceptr->locName);
        hs_smlFreePcdata( __FILE__, __LINE__, sourceptr->locURI);
    }
#endif
      hs_smlLibFree(sourceptr);
      sourceptr=NULL;
#if 1 // chenxiang_memory 20080219
      if (NULL != targetptr)
      {
        hs_smlFreePcdata( __FILE__, __LINE__, targetptr->locName);
        hs_smlFreePcdata( __FILE__, __LINE__, targetptr->locURI);
    }
#endif
      hs_smlLibFree(targetptr);
      targetptr=NULL;
      hs_smlLibFree(item);
      item=NULL;
      hs_smlLibFree(itemList);
      itemList=NULL;
    }

    hs_smlFreePcdata( __FILE__, __LINE__, map.cmdID); // chenxiang_memory 20080312
    hs_smlFreePcdata( __FILE__, __LINE__, target.locURI); // chenxiang_memory 20080312
    hs_smlFreePcdata( __FILE__, __LINE__, source.locURI); // chenxiang_memory 20080312
    hs_smlLibFree(temp); // chenxiang_memory 20080312
    //hs_globlemapItemList=NULL; // chenxiang_map 20080325

    return returnval;

 }	



/******************************************************************* 
** 函数名:hs_syncml_ReplaceCmd
** 功能描述：添加同步命令Replace
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //wangxiaolin  2009.8.6  for 大对象
short hs_syncml_ReplaceCmd(short id) 
{
    char* temp;
    SmlReplace_t replace;
    SmlMetInfMetInf_t meta;
    short returnval=HS_SML_ERR_OK;
    SmlItemPtr_t item=NULL;
    SmlItemListPtr_t itemList=NULL;
    SmlSourcePtr_t source=NULL;
    char * size_str=NULL;

    //
    if(FALSE==hs_syncml_get_large_object_proc_flag())
    {
        replace.itemList=NULL;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
        if(SYNC_MODE_PB == g_pim.m_sync_mode)
        {
#endif /* HS_PIM_VCAL */
        replace.itemList=hs_myGetReplacedPBItems();//add by wangxiaolin 2009.05.29
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
        }
        else
        {
            replace.itemList=hs_myGetReplacedVCALItems();
        }
#endif /* HS_PIM_VCAL */
        if(NULL!=replace.itemList)
        {
            hs_content_total_len=replace.itemList->item->data->length;
        }
        else
        {
             return returnval;
        }
    }
    else//正在处理大对象数据，不需要获得新的列表
    {
        replace.itemList=hs_sml_item_ptr;
        //syncml_message:"MMIPIM hs_syncml_ReplaceCmd,now the large object is proc 1 ..."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1870_112_2_18_3_26_0_262,(uint8*)"");
    }
    
    if(NULL==replace.itemList) /*there is no items need to be added.*/
    {
      return returnval;
    }
    
    /* Add cmd */
    replace.elementType = SML_PE_REPLACE;
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    hs_task_relay_info->cmdID++;
    replace.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    replace.flags = 0;
    replace.cred = NULL;
    
    
    if((replace.itemList->item->flags==0x0400)&&(hs_content_total_len>PIM_MAX_VCARD_DATA_LEN))
    {
        //大对象第一个包
        if(0==hs_lcontent_proc_len)
        {
            //syncml_message:"MMIPIM hs_syncml_ReplaceCmd,the first chunk of the large object, total_len=%d,cur_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1892_112_2_18_3_26_0_263,(uint8*)"dd",hs_content_total_len,hs_lcontent_proc_len);
            hs_smlLibMemset(&meta, 0, (long)sizeof(SmlMetInfMetInf_t));
            meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcard");
            size_str=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_content_total_len);
            meta.size=hs_smlString2Pcdata(__FILE__, __LINE__, size_str);
            replace.meta =hs_smlmeta2extPcdata(__FILE__, __LINE__, &meta);
            
            replace.itemList->item->data->length=PIM_MAX_VCARD_DATA_LEN;
            returnval=hs_smlReplaceCmd(id, &replace);
            hs_lcontent_proc_len=PIM_MAX_VCARD_DATA_LEN;
            hs_item_content_ptr=replace.itemList->item->data->content;
            replace.itemList->item->data->content=(void*)((char*)replace.itemList->item->data->content+replace.itemList->item->data->length);
            //syncml_message:"MMIPIM ,hs_syncml_ReplaceCmd ,first ,content_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1904_112_2_18_3_26_0_264,(uint8*)"d",replace.itemList->item->data->length);
            //hs_myhttplogs((char *)(replace.itemList->item->data->content),BASE64_BUF_LEN);
            hs_syncml_set_large_object_proc_flag(TRUE);
            hs_sml_item_ptr=replace.itemList;
            
            hs_smlFreePcdata( __FILE__, __LINE__, meta.type);
            hs_smlLibFree(replace.meta);
            hs_smlLibFree(size_str);
        }
        else
        {
            replace.meta=NULL;
            //剩余的数据一次可以发送完毕，也即这是大对象的最后一个包
            if(hs_content_total_len-hs_lcontent_proc_len<PIM_MAX_VCARD_DATA_LEN)/*lint !e737*/
            {
               //syncml_message:"MMIPIM hs_syncml_ReplaceCmd,the last chunk of the large object, total_len=%d,cur_len=%d"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1919_112_2_18_3_26_0_265,(uint8*)"dd",hs_content_total_len,hs_lcontent_proc_len);

                replace.itemList->item->data->length=hs_content_total_len-hs_lcontent_proc_len;/*lint !e737*/

                //不发送</Moredata>
                replace.itemList->item->flags=0;
                returnval=hs_smlReplaceCmd(id, &replace);
                //syncml_message:"MMIPIM ,hs_syncml_ReplaceCmd ,last ,content_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1926_112_2_18_3_26_0_266,(uint8*)"d",replace.itemList->item->data->length);
                //hs_myhttplogs((char *)(replace.itemList->item->data->content),BASE64_BUF_LEN);
                replace.itemList->item->data->content=hs_item_content_ptr;
                hs_syncml_set_large_object_proc_flag(FALSE);
                hs_item_content_ptr=NULL;
                hs_sml_item_ptr=NULL;
                hs_lcontent_proc_len=0;
                hs_content_total_len=0;
            }
            else //大对象中间的包
            {
                //syncml_message:"MMIPIM hs_syncml_ReplaceCmd,the midle chunk of the large object, total_len=%d,cur_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1937_112_2_18_3_26_0_267,(uint8*)"dd",hs_content_total_len,hs_lcontent_proc_len);

                replace.itemList->item->data->length=PIM_MAX_VCARD_DATA_LEN;
                //将指针后移，发送后面的数据
                returnval=hs_smlReplaceCmd(id, &replace);
                hs_lcontent_proc_len+=PIM_MAX_VCARD_DATA_LEN;
                replace.itemList->item->data->content=(void*)((char*)replace.itemList->item->data->content+replace.itemList->item->data->length);
                //syncml_message:"MMIPIM ,hs_syncml_ReplaceCmd ,middle ,content_len=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1944_112_2_18_3_26_0_268,(uint8*)"d",replace.itemList->item->data->length);
                //hs_myhttplogs((char *)(replace.itemList->item->data->content),BASE64_BUF_LEN);
                hs_syncml_set_large_object_proc_flag(TRUE); 
                hs_sml_item_ptr=replace.itemList;
            }
        }
    }
    else
    {
        //设置大对象处理的控制变量
        hs_syncml_set_large_object_proc_flag(FALSE);
        hs_lcontent_proc_len=0;
        hs_sml_item_ptr=NULL;
        //虽然被视为大对象，但因为该包只发送一个对象，可以一次发送完毕
        if(replace.itemList->item->flags==0x0400)
        {
            replace.itemList->item->flags=0;
        }
        
        hs_smlLibMemset(&meta, 0, (long)sizeof(SmlMetInfMetInf_t));
        meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcard");
        replace.meta =hs_smlmeta2extPcdata(__FILE__, __LINE__, &meta);
        returnval=hs_smlReplaceCmd(id, &replace);
        hs_smlFreePcdata( __FILE__, __LINE__, meta.type);
        hs_smlLibFree(replace.meta);
    }
    //本次数据没有发送完，暂时不能释放itemList列表的内存
    if(TRUE==hs_syncml_get_large_object_proc_flag())
    {
        //syncml_message:"MMIPIM hs_syncml_ReplaceCmd,now the large object is proc 2 ..."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_1973_112_2_18_3_26_0_269,(uint8*)"");
    }
    else//一次数据包发送完毕
    {
        //free drynamic memorys.
        while(replace.itemList!=NULL)
        {
          itemList=replace.itemList;
          replace.itemList=itemList->next;
          item=itemList->item;
          source=item->source;
          if (NULL != item)
          {
              hs_smlFreePcdata( __FILE__, __LINE__, item->meta);
              hs_smlFreePcdata( __FILE__, __LINE__, item->data);
          }
          if (NULL != source)
          {
            hs_smlFreePcdata( __FILE__, __LINE__, source->locName);
            hs_smlFreePcdata( __FILE__, __LINE__, source->locURI);
          }
          hs_smlLibFree(source);
          source=NULL;
          hs_smlLibFree(item);
          item=NULL;
          hs_smlLibFree(itemList);
          itemList=NULL;
        }
    }
    hs_smlFreePcdata( __FILE__, __LINE__, replace.cmdID);   
    hs_smlLibFree(temp); 
    return returnval;

}
#else /* PIM_SUPPORT_LARGE_OBJECT_PROC */
short hs_syncml_ReplaceCmd(short id) //chenxiang 20071124
 {
    char* temp;
    SmlReplace_t replace;
    SmlMetInfMetInf_t meta;
    short returnval=HS_SML_ERR_OK;
    SmlItemListPtr_t itemList=NULL;
    SmlItemPtr_t item=NULL;
    SmlSourcePtr_t source=NULL;
     //syncml_core_message:"MMIPIM  $$ enter hs_syncml_ReplaceCmd!$$"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2017_112_2_18_3_26_0_270,(uint8*)"");
    replace.itemList=NULL;

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    replace.itemList=hs_myGetReplacedPBItems(); //chenxiang 20071124
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        replace.itemList=hs_myGetReplacedVCALItems();
    }
#endif /* HS_PIM_VCAL */

    if(NULL==replace.itemList) /*there is no items to be deleted*/
    {
      //syncml_core_message:"MMIPIM  $$ there is no items to be replaced!$$"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2035_112_2_18_3_26_0_271,(uint8*)"");
      return returnval;
    }
    
    /* replace cmd */
    replace.elementType = SML_PE_REPLACE;
    temp=hs_smlunsignedInt2String(__FILE__, __LINE__, hs_task_relay_info->cmdID);
    hs_task_relay_info->cmdID++;
    replace.cmdID=hs_smlString2Pcdata(__FILE__, __LINE__, temp);
    hs_smlLibFree(temp); // chenxiang_memory 20080305
    replace.flags = 0;
    replace.cred = NULL;
    hs_smlLibMemset(&meta, 0, (long)sizeof(SmlMetInfMetInf_t));
    meta.type=hs_smlString2Pcdata(__FILE__, __LINE__, "text/x-vcard");
    replace.meta =hs_smlmeta2extPcdata(__FILE__, __LINE__, &meta);

    returnval=hs_smlReplaceCmd(id, &replace);
 
   //free drynamic memorys.
    while(replace.itemList!=NULL)
    {
      itemList=replace.itemList;
      replace.itemList=itemList->next;
      item=itemList->item;
      source=item->source;

      if (NULL != item)
      {
        hs_smlFreePcdata( __FILE__, __LINE__, item->meta);
        hs_smlFreePcdata( __FILE__, __LINE__, item->data);
      }
      if (NULL != source)
      {
        hs_smlFreePcdata( __FILE__, __LINE__, source->locName);
        hs_smlFreePcdata( __FILE__, __LINE__, source->locURI);
      }

      hs_smlLibFree(source);
      source=NULL;
      hs_smlLibFree(item);
      item=NULL;
      hs_smlLibFree(itemList);
      itemList=NULL;
    }


        hs_smlFreePcdata( __FILE__, __LINE__, replace.cmdID);
        hs_smlFreePcdata( __FILE__, __LINE__, meta.type);
        hs_smlLibFree(replace.meta);

    return returnval;
 }
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */


 /******************************************************************* 
** 函数名:hs_syncml_EndSync
** 功能描述：添加同步</SyncBody>
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_EndSync(short id)
{
    return hs_smlEndSync(id);
}

 /******************************************************************* 
** 函数名:hs_syncml_EndMessage
** 功能描述：添加同步</Sync>
** 输  出: 成功与否状态
** 作  者:王晓林
** 日  期:2007-11-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_EndMessage(short id)
{
    /* This ends the SyncML document ... it has been assembled */
    /* SmlFinal_f says this is the last message in the SyncML package */
    /* (since it's the only one) */
    //hs_myFreestatusofCQ(); // chenxiang_memory 20080303
    return hs_smlEndMessage(id, SmlFinal_f);
}

 /******************************************************************* 
** 函数名:hs_syncml_EndMessageWithoutFinal
** 功能描述：添加同步</Sync>
** 输  出: 成功与否状态
** 作  者:王晓林
** 日  期:2007-11-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_EndMessageWithoutFinal(short id)
{
    /* This ends the SyncML document ... it has been assembled */
    /*No SmlFinal_f says this is not the last message in the SyncML package */
    /* the server should send 222 to ask more data in the next message */
    //hs_myFreestatusofCQ(); // chenxiang_memory 20080303
    return hs_smlEndMessage(id, 0x0);
}
/******************************************************************* 
** 函数名:hs_syncml_ReceiveData
** 功能描述：接收sever信息处理函数
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_ReceiveData(short id)
{
       void* userData = NULL;
	short rc;
       /*  Prepare the callback parameter userData here!
       userData is a void pointer handed over to every callback function
       as one of the function arguments. The Toolkit doesn't touch the
       content of this structure. For instance, this mechanism can be used 
       by the application to pass data to the callback routines. */
       rc=hs_smlSetUserData(id, &userData);
	if(rc!=HS_SML_ERR_OK)
	{
             return rc;
	}
	/* --- Parse commands & invoke callback routines of the application -- */
       rc=hs_smlProcessData(id, SML_ALL_COMMANDS);
	if(rc!=HS_SML_ERR_OK)
	{
	     return rc;
	}
	return rc;
}

/******************************************************************* 
** 函数名:hs_syncml_TerminateInstance
** 功能描述：终结instance，释放指定ID的instance申请的
**                             内存
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-18
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_TerminateInstance(short id)
{
    return hs_smlTerminateInstance(id);
}

/******************************************************************* 
** 函数名:hs_syncml_TerminateAllExitInstance
** 功能描述：终结instance，释放所有存在的ID的instance
**                            申请的内存
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-18
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_TerminateAllExitInstance(void)
{
    short rc=0;
    short id;
    for(id=1;id<5;id++)
    {
            rc|=hs_smlTerminateInstance(id);
	     if(rc==HS_SML_ERR_MGR_INVALID_INSTANCE_INFO)
	     {
	         //syncml_core_message:"MMIPIM  $$ release all instances,instance NO.%d is NULL!$$"
	         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2196_112_2_18_3_26_0_272,(uint8*)"d",id);
	     }else if(rc==HS_SML_ERR_OK){
	         //syncml_core_message:"MMIPIM  $$release all instances,instance NO.%d release OK!$$"
	         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2198_112_2_18_3_26_0_273,(uint8*)"d",id);
	     }else{
	         //syncml_core_message:"MMIPIM  $$release all instances,instance NO.%d release error!$$"
	         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2200_112_2_18_3_26_0_274,(uint8*)"d",id);
	     }
    }
    if(hs_pGlobalAnchor->instanceListAnchor!=NULL)
    {
         hs_smlLibFree((hs_pGlobalAnchor->instanceListAnchor));
         hs_pGlobalAnchor->instanceListAnchor=NULL;
    }
    return rc;
}

/******************************************************************* 
** 函数名:hs_syncml_Terminate
** 功能描述：终结pGlobalAnchor，释放申请的
**                             内存
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-18
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_Terminate(void)
{
    short ret=HS_SML_ERR_OK;

   //syncml_core_message:"MMIPIM  $$Delete all deleted pb items error!$$"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2223_112_2_18_3_26_0_275,(uint8*)"");

    ret=hs_smlTerminate();
    return ret;
}


 /******************************************************************* 
** 函数名:hs_syncml_fresh_for_slow_sync
** 功能描述：将慢同步需要的数据加入syncml文档
** 输  出: 成功与否状态
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_fresh_for_slow_sync(short id)
 {
      return hs_syncml_AddCmd(id); // chenxiang 20071124
 }

  /******************************************************************* 
** 函数名:hs_syncml_modify_for_fast_sync
** 功能描述：将快同步需要的数据加入syncml文档
** 输  出: 成功与否状态
** 作  者:王晓林
** 日  期:2007-11-12
** 版本: ver 1.0
*******************************************************************/
short hs_syncml_modify_for_fast_sync(short id)
 {
      short returnval=HS_SML_ERR_OK;
      PIM_T_P  pMe=MMIPIM_GetPIM();
      //syncml_core_message:"MMIPIM  $$enter hs_syncml_modify_for_fast_sync $$"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2255_112_2_18_3_26_1_276,(uint8*)"");
      returnval=hs_syncml_DeleteCmd(id);
      //syncml_core_message:"MMIPIM  $$end hs_syncml_DeleteCmd"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2257_112_2_18_3_26_1_277,(uint8*)"");
      if(HS_SML_ERR_OK!=returnval)
      {
      //syncml_core_message:"MMIPIM  $$delete command error!$$"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2260_112_2_18_3_26_1_278,(uint8*)"");
      }
      if(pMe->g_sync_result.delete_is_final)
      {
         //syncml_core_message:"MMIPIM  $$begin hs_syncml_ReplaceCmd"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2264_112_2_18_3_26_1_279,(uint8*)"");
         returnval=hs_syncml_ReplaceCmd(id); // chenxiang 20071124
        if(HS_SML_ERR_OK!=returnval)
        {
        //syncml_core_message:"MMIPIM  $$ replace command error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2268_112_2_18_3_26_1_280,(uint8*)"");
        }
	 //syncml_core_message:"MMIPIM  $$end hs_syncml_ReplaceCmd"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2270_112_2_18_3_26_1_281,(uint8*)"");
        if(pMe->g_sync_result.replace_is_final)
        {
            //syncml_core_message:"MMIPIM  $$begin hs_syncml_ReplaceCmd"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2273_112_2_18_3_26_1_282,(uint8*)"");
            returnval=hs_syncml_AddCmd(id); // chenxiang 20071124
            if(HS_SML_ERR_OK!=returnval)
            {
            //syncml_core_message:"MMIPIM  $$ add command error!$$"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMSYNCMLMGR_2277_112_2_18_3_26_1_283,(uint8*)"");
            }
        }
      }
      return returnval;
 }
