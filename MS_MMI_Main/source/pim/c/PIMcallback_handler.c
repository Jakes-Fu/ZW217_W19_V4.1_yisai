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
文件名称:PIMcallback_handler.c
文件描述:callback handler
文件说明:回叫函数处理
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
  #define _PIMCALLBACK_HANDLER_C_
  
  #include "../h/PIMsmlcore.h"
  #include "../h/PIMlibmem.h"
  #include "../h/PIMsmlerr.h"
  #include "../h/PIMdatasynctask.h"
  #include "../h/PIMCommhttp.h"
  //#include "mmi_appmsg.h"
  //#include "mmk_msg.h"


  /*  全局变量声明*/
extern task_protocol_step_type* hs_protocol_step;
extern task_protocol_step_type* hs_protocol_step_priv;

extern task_relay_info_type*     hs_task_relay_info;

extern status_cmd_queue_type*  hs_status_cmd_que;

extern http_globle_indicator_type* hs_http_globle;

extern PIM_T g_pim; // chenxiang 20071206
  /*  功能函数*/

SmlNewMapItemListPtr_t  hs_globlemapItemList=NULL;



/*************************************************************************
 *  Local SyncML API functions
 *************************************************************************/
#ifdef __HS_USE_DEVINF__
   static void hs_smlFreeDevInfDatastore(SmlDevInfDatastorePtr_t data);
   static void hs_smlFreeDevInfDatastoreList(SmlDevInfDatastoreListPtr_t data);
   static void hs_smlFreeDevInfXmitList(SmlDevInfXmitListPtr_t data);
   static void hs_smlFreeDevInfXmit(SmlDevInfXmitPtr_t data);
   static void hs_smlFreeDevInfDSMem(SmlDevInfDSMemPtr_t data);
   static void hs_smlFreeDevInfSynccap(SmlDevInfSyncCapPtr_t data);
   static void hs_smlFreeDevInfExt(SmlDevInfExtPtr_t data);
   static void hs_smlFreeDevInfExtList(SmlDevInfExtListPtr_t data);
   static void hs_smlFreeDevInfCTData(SmlDevInfCTDataPtr_t data);
   static void hs_smlFreeDevInfCTDataList(SmlDevInfCTDataListPtr_t data);
   static void hs_smlFreeDevInfCTDataProp(SmlDevInfCTDataPropPtr_t data);
   static void hs_smlFreeDevInfCTDataPropList(SmlDevInfCTDataPropListPtr_t data);
   static void hs_smlFreeDevInfCTCap(SmlDevInfCTCapPtr_t data);
   static void hs_smlFreeDevInfCtcapList(SmlDevInfCtcapListPtr_t data);
   static void hs_smlFreeDevInfDevInf(SmlDevInfDevInfPtr_t data);
#endif

 static void hs_smlFreePcdataList(SmlPcdataListPtr_t list);
 
  static void hs_smlFreeTargetRefList(SmlTargetRefListPtr_t pTargetRefList);

  static void hs_smlFreeSourceRefList(SmlSourceRefListPtr_t pSourceRefList);
/**
 * FUNCTION: hs_smlFreeProtoElement
 *
 * frees all allocated memory of a smlProtoElement
 *
 * IN:              VoidPtr_t
 *                  Element to free
 *
 * RETURN:          Ret_t
 *                  Return Code
 */

short hs_smlFreeProtoElement(void * pProtoElement)
{
	if (! pProtoElement)
		return(HS_SML_ERR_OK);

	switch (((SmlUnknownProtoElementPtr_t)pProtoElement)->elementType) {

		case SML_PE_HEADER:
			hs_smlFreeSyncHdr((SmlSyncHdrPtr_t)pProtoElement);
			break;

		case SML_PE_SYNC_START:
			hs_smlFreeSync((SmlSyncPtr_t)pProtoElement);
			break;

		case SML_PE_ADD:
		case SML_PE_COPY:
		case SML_PE_REPLACE:
		case SML_PE_DELETE:
		case SML_PE_GENERIC:
			hs_smlFreeGeneric((SmlGenericCmdPtr_t)pProtoElement);
			break;

		case SML_PE_ALERT:
			hs_smlFreeAlert((SmlAlertPtr_t)pProtoElement);
			break;

    case SML_PE_ATOMIC_START:
    case SML_PE_SEQUENCE_START:
		case SML_PE_CMD_GROUP:
			hs_smlFreeAtomic((SmlAtomicPtr_t)pProtoElement);
			break;

#if (defined HS_EXEC_SEND || defined HS_EXEC_RECEIVE)
		case SML_PE_EXEC:
			hs_smlFreeExec((SmlExecPtr_t)pProtoElement);
			break;
#endif

    case SML_PE_PUT:
    case SML_PE_GET:
		case SML_PE_PUT_GET:
			hs_smlFreeGetPut((SmlPutPtr_t)pProtoElement);
			break;

		case SML_PE_MAP:
			hs_smlFreeMap((SmlMapPtr_t)pProtoElement);
			break;

		case SML_PE_RESULTS:
			hs_smlFreeResults((SmlResultsPtr_t)pProtoElement);
			break;

		case SML_PE_STATUS:
			hs_smlFreeStatus((SmlStatusPtr_t)pProtoElement);
			break;

		default:
			return(HS_SML_ERR_A_UTI_UNKNOWN_PROTO_ELEMENT);
	}

	return(HS_SML_ERR_OK);
}

/**
 * FUNCTION: hs_smlFreePcdata
 *
 * frees the Memory of an allocated Pcdata memory object
 *
 * IN:              SmlPcdataPtr_t
 *                  A Pointer to a PcData structure, which should be freed
 *
 * RETURN:          ---
 *
 */
void hs_smlFreePcdata(const char * file,  int line, SmlPcdataPtr_t pPcdata)
{
	if (! pPcdata)
		return;

	if (pPcdata->contentType == SML_PCDATA_EXTENSION) {
		switch ((int)pPcdata->extension) {
#ifdef __HS_USE_METINF__
			case SML_EXT_METINF:
        //SCI_TRACE_MID_PIM("MMIPIM chenxiang hs_smlFreePcdata 2 file: %s , line: %d", file, line);
				hs_smlFreeMetinfMetinf(pPcdata->content);
				hs_smlLibFree(pPcdata);
				break;
#endif
#ifdef __HS_USE_DEVINF__
			case SML_EXT_DEVINF:
        //SCI_TRACE_MID_PIM("MMIPIM chenxiang hs_smlFreePcdata 3 file: %s , line: %d", file, line);
				hs_smlFreeDevInfDevInf(pPcdata->content);
				hs_smlLibFree(pPcdata);
				break;
#endif
			default:
				break;
		}
		return;
	}

        //SCI_TRACE_MID_PIM("MMIPIM chenxiang hs_smlFreePcdata 1 file: %s , line: %d", file, line);
	if (pPcdata->content)
		hs_smlLibFree((pPcdata->content));

	hs_smlLibFree(pPcdata);
}

static void hs_smlFreePcdataList(SmlPcdataListPtr_t list) {
	if (!list) return;
	hs_smlFreePcdataList(list->next);
	hs_smlFreePcdata( __FILE__, __LINE__, list->data);
	hs_smlLibFree(list);
	return;
}

void hs_smlFreeSourceTargetPtr(SmlSourcePtr_t pSourceTarget)
{
	if (! pSourceTarget)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pSourceTarget->locURI);
	hs_smlFreePcdata( __FILE__, __LINE__, pSourceTarget->locName);

	hs_smlLibFree(pSourceTarget);
}

void hs_smlFreeCredPtr(SmlCredPtr_t pCred)
{
	if (! pCred)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pCred->meta);
	hs_smlFreePcdata( __FILE__, __LINE__, pCred->data);

	hs_smlLibFree(pCred);
}


void hs_smlFreeSyncHdr(SmlSyncHdrPtr_t pSyncHdr)
{
	if (! pSyncHdr)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pSyncHdr->version);
	hs_smlFreePcdata( __FILE__, __LINE__, pSyncHdr->proto);
	hs_smlFreePcdata( __FILE__, __LINE__, pSyncHdr->sessionID);
	hs_smlFreePcdata( __FILE__, __LINE__, pSyncHdr->msgID);
	hs_smlFreePcdata( __FILE__, __LINE__, pSyncHdr->respURI);
	hs_smlFreePcdata( __FILE__, __LINE__, pSyncHdr->meta);

	hs_smlFreeSourceTargetPtr(pSyncHdr->source);
	hs_smlFreeSourceTargetPtr(pSyncHdr->target);

	hs_smlFreeCredPtr(pSyncHdr->cred);

	hs_smlLibFree(pSyncHdr);
}

void hs_smlFreeSync(SmlSyncPtr_t pSync)
{
	if (! pSync)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pSync->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pSync->meta);
	hs_smlFreePcdata( __FILE__, __LINE__, pSync->noc); // chenxiang_memory 20080313 
    
	hs_smlFreeSourceTargetPtr(pSync->source);
	hs_smlFreeSourceTargetPtr(pSync->target);

	hs_smlFreeCredPtr(pSync->cred);

	hs_smlLibFree(pSync);
}

void hs_smlFreeItemPtr(SmlItemPtr_t pItem)
{
	if (! pItem)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pItem->meta);
	hs_smlFreePcdata( __FILE__, __LINE__, pItem->data);

	hs_smlFreeSourceTargetPtr(pItem->source);
	hs_smlFreeSourceTargetPtr(pItem->target);

	hs_smlLibFree(pItem);
}

void hs_smlFreeItemList(SmlItemListPtr_t pItemList)
{
	SmlItemListPtr_t	pTmp;

	while (pItemList) {
		pTmp = pItemList->next;
		hs_smlFreeItemPtr(pItemList->item);
		hs_smlLibFree(pItemList);
		pItemList = pTmp;
	}
}


void hs_smlFreeGeneric(SmlGenericCmdPtr_t pGenericCmd)
{
	if (! pGenericCmd)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pGenericCmd->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pGenericCmd->meta);

	hs_smlFreeCredPtr(pGenericCmd->cred);

	hs_smlFreeItemList(pGenericCmd->itemList);

	hs_smlLibFree(pGenericCmd);
}

void hs_smlFreeAlert(SmlAlertPtr_t pAlert)
{
	if (! pAlert)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pAlert->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pAlert->data);

	hs_smlFreeCredPtr(pAlert->cred);

	hs_smlFreeItemList(pAlert->itemList);

	hs_smlLibFree(pAlert);
}


void hs_smlFreeStatus(SmlStatusPtr_t pStatus)
{
	if (! pStatus)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pStatus->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pStatus->msgRef);
	hs_smlFreePcdata( __FILE__, __LINE__, pStatus->cmdRef);
	hs_smlFreePcdata( __FILE__, __LINE__, pStatus->cmd);
	hs_smlFreePcdata( __FILE__, __LINE__, pStatus->data);

	hs_smlFreeCredPtr(pStatus->cred);
	hs_smlFreeChalPtr(pStatus->chal);

	hs_smlFreeTargetRefList(pStatus->targetRefList);
	hs_smlFreeSourceRefList(pStatus->sourceRefList);

	hs_smlFreeItemList(pStatus->itemList);

	hs_smlLibFree(pStatus);
}



 void hs_smlFreeChalPtr(SmlChalPtr_t pChal)
{
	if (! pChal)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pChal->meta);

	hs_smlLibFree(pChal);
}


static void hs_smlFreeTargetRefList(SmlTargetRefListPtr_t pTargetRefList)
{
	SmlTargetRefListPtr_t	pTmp;

	while (pTargetRefList) {
		pTmp = pTargetRefList->next;
		hs_smlFreePcdata( __FILE__, __LINE__, pTargetRefList->targetRef);
		hs_smlLibFree(pTargetRefList);
		pTargetRefList = pTmp;
	}
}


static void hs_smlFreeSourceRefList(SmlSourceRefListPtr_t pSourceRefList)
{
	SmlSourceRefListPtr_t	pTmp;

	while (pSourceRefList) {
		pTmp = pSourceRefList->next;
		hs_smlFreePcdata( __FILE__, __LINE__, pSourceRefList->sourceRef);
		hs_smlLibFree(pSourceRefList);
		pSourceRefList = pTmp;
	}
}


void hs_smlFreeGetPut(SmlPutPtr_t pGetPut)
{
	if (! pGetPut)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pGetPut->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pGetPut->meta);
	hs_smlFreePcdata( __FILE__, __LINE__, pGetPut->lang);

	hs_smlFreeCredPtr(pGetPut->cred);

	hs_smlFreeItemList(pGetPut->itemList);

	hs_smlLibFree(pGetPut);
}

#if (defined HS_ATOMIC_RECEIVE || defined HS_SEQUENCE_RECEIVE)  
void hs_smlFreeAtomic(SmlAtomicPtr_t pAtomic)
{
	if (! pAtomic)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pAtomic->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pAtomic->meta);

	hs_smlLibFree(pAtomic);
}
#endif

#if (defined HS_EXEC_SEND || defined HS_EXEC_RECEIVE)

void hs_smlFreeExec(SmlExecPtr_t pExec)
{
	if (! pExec)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pExec->cmdID);

	hs_smlFreeCredPtr(pExec->cred);

	hs_smlFreeItemPtr(pExec->item);

	hs_smlLibFree(pExec);
}

#endif

#ifdef HS_MAP_RECEIVE
void hs_smlFreeMap(SmlMapPtr_t pMap)
{
	if (! pMap)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pMap->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pMap->meta);

	hs_smlFreeCredPtr(pMap->cred);

	hs_smlFreeSourceTargetPtr(pMap->source);
	hs_smlFreeSourceTargetPtr(pMap->target);

	hs_smlFreeMapItemList(pMap->mapItemList);

	hs_smlLibFree(pMap);
}

 void hs_smlFreeNewMapItemList(SmlNewMapItemListPtr_t pMapItemList)
{
	SmlNewMapItemListPtr_t pTmp;

	while (pMapItemList) {
		pTmp = pMapItemList->next;
		//hs_smlFreeMapItemPtr(pMapItemList->mapItem);
		hs_smlLibFree(pMapItemList);
		pMapItemList = pTmp;
	}
}

 void hs_smlFreeMapItemList(SmlMapItemListPtr_t pMapItemList)
{
	SmlMapItemListPtr_t	pTmp;

	while (pMapItemList) {
		pTmp = pMapItemList->next;
		hs_smlFreeMapItemPtr(pMapItemList->mapItem);
		hs_smlLibFree(pMapItemList);
		pMapItemList = pTmp;
	}
}

void hs_smlFreeMapItemPtr(SmlMapItemPtr_t pMapItem)
{
	if (! pMapItem)
		return;

	hs_smlFreeSourceTargetPtr(pMapItem->source);
	hs_smlFreeSourceTargetPtr(pMapItem->target);

	hs_smlLibFree(pMapItem);
}
#endif

#ifdef HS_RESULT_RECEIVE
void hs_smlFreeResults(SmlResultsPtr_t pResults)
{
	if (! pResults)
		return;

	hs_smlFreePcdata( __FILE__, __LINE__, pResults->cmdID);
	hs_smlFreePcdata( __FILE__, __LINE__, pResults->msgRef);
	hs_smlFreePcdata( __FILE__, __LINE__, pResults->cmdRef);
	hs_smlFreePcdata( __FILE__, __LINE__, pResults->meta);
	hs_smlFreePcdata( __FILE__, __LINE__, pResults->targetRef);
	hs_smlFreePcdata( __FILE__, __LINE__, pResults->sourceRef);

	hs_smlFreeItemList(pResults->itemList);

	hs_smlLibFree(pResults);
}
#endif

SmlPcdataPtr_t hs_concatPCData(SmlPcdataPtr_t pDat1, const SmlPcdataPtr_t pDat2)
{
    if (pDat1->contentType != pDat2->contentType)
        return NULL;

    switch (pDat1->contentType) {
        case SML_PCDATA_STRING:
            pDat1->content = (void*)hs_smlLibStrcat(pDat1->content, pDat2->content);
            pDat1->length += pDat2->length;
            break;
        case SML_PCDATA_OPAQUE:
            if ((pDat1->content = hs_smlLibRealloc(pDat1->content, pDat1->length + pDat2->length)) == NULL) 
                return NULL;
            hs_smlLibMemmove(((unsigned char*)pDat1->content) + pDat1->length, pDat2->content, pDat2->length);
            pDat1->length += pDat2->length;
            break;
        default:
            return NULL;
    }
    return pDat1;
}

#ifdef __HS_USE_METINF__
/** Subfunctions to hs_smlFreePcdata, to freeup MetaInf DTD structures **/
void hs_smlFreeMetinfMetinf(SmlMetInfMetInfPtr_t data) {
    if (!data) return;
	hs_smlFreePcdata( __FILE__, __LINE__, data->format);
	hs_smlFreePcdata( __FILE__, __LINE__, data->type);
	hs_smlFreePcdata( __FILE__, __LINE__, data->mark);
	hs_smlFreePcdata( __FILE__, __LINE__, data->size);
	hs_smlFreePcdata( __FILE__, __LINE__, data->version);
	hs_smlFreePcdata( __FILE__, __LINE__, data->nextnonce);
	hs_smlFreePcdata( __FILE__, __LINE__, data->maxmsgsize);
    /* SCTSTK - 18/03/2002, S.H. 2002-04-05 : SyncML 1.1 */
	hs_smlFreePcdata( __FILE__, __LINE__, data->maxobjsize);
	hs_smlFreeMetinfAnchor(data->anchor);
	hs_smlFreeMetinfMem(data->mem);
	hs_smlFreePcdataList(data->emi);
	hs_smlLibFree(data);
	return;
}

void hs_smlFreeMetinfAnchor(SmlMetInfAnchorPtr_t data) {
    if (!data) return;
	hs_smlFreePcdata( __FILE__, __LINE__, data->last);
	hs_smlFreePcdata( __FILE__, __LINE__, data->next);
	hs_smlLibFree(data);
	return;
}

void hs_smlFreeMetinfMem(SmlMetInfMemPtr_t data) {
    if (!data) return;
	hs_smlFreePcdata( __FILE__, __LINE__, data->shared);
	hs_smlFreePcdata( __FILE__, __LINE__, data->free);
	hs_smlFreePcdata( __FILE__, __LINE__, data->freeid);
	hs_smlLibFree(data);
	return;
}
#endif

#ifdef __HS_USE_DEVINF__
/** Subfunctions to hs_smlFreePcdata, to freeup DevInf DTD structures **/
static void hs_smlFreeDevInfDevInf(SmlDevInfDevInfPtr_t data) {
    if (!data) return;
    hs_smlFreePcdata( __FILE__, __LINE__, data->verdtd);
    hs_smlFreePcdata( __FILE__, __LINE__, data->man);
    hs_smlFreePcdata( __FILE__, __LINE__, data->mod);
    hs_smlFreePcdata( __FILE__, __LINE__, data->oem);
    hs_smlFreePcdata( __FILE__, __LINE__, data->fwv);
    hs_smlFreePcdata( __FILE__, __LINE__, data->hwv);
    hs_smlFreePcdata( __FILE__, __LINE__, data->swv);
    hs_smlFreePcdata( __FILE__, __LINE__, data->devid);
    hs_smlFreePcdata( __FILE__, __LINE__, data->devtyp);
    hs_smlFreeDevInfDatastoreList(data->datastore);
    hs_smlFreeDevInfExtList(data->ext);
    hs_smlFreeDevInfCtcapList(data->ctcap);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfDatastore(SmlDevInfDatastorePtr_t data) {
    if (!data) return;
    hs_smlFreePcdata( __FILE__, __LINE__, data->sourceref);
    hs_smlFreePcdata( __FILE__, __LINE__, data->displayname);
    hs_smlFreePcdata( __FILE__, __LINE__, data->maxguidsize);
    hs_smlFreeDevInfXmit(data->rxpref);
    hs_smlFreeDevInfXmit(data->txpref);
    hs_smlFreeDevInfXmitList(data->rx);
    hs_smlFreeDevInfXmitList(data->tx);
    hs_smlFreeDevInfDSMem(data->dsmem);
    hs_smlFreeDevInfSynccap(data->synccap);
    hs_smlLibFree(data);
    return;
}

static void hs_smlFreeDevInfDatastoreList(SmlDevInfDatastoreListPtr_t data) {
    if (!data) return;
    hs_smlFreeDevInfDatastore(data->data);
    hs_smlFreeDevInfDatastoreList(data->next);
    hs_smlLibFree(data);
    return;
}

static void hs_smlFreeDevInfXmitList(SmlDevInfXmitListPtr_t data) {
    if (!data) return;
    hs_smlFreeDevInfXmit(data->data);
    hs_smlFreeDevInfXmitList(data->next);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfXmit(SmlDevInfXmitPtr_t data) {
    if (!data) return;
    hs_smlFreePcdata( __FILE__, __LINE__, data->cttype);
    hs_smlFreePcdata( __FILE__, __LINE__, data->verct);
    hs_smlLibFree(data);
}

static  void hs_smlFreeDevInfDSMem(SmlDevInfDSMemPtr_t data) {
    if (!data) return;
    // %%%luz:2003-04-28: this is now a flag! hs_smlFreePcdata( __FILE__, __LINE__, data->shared);
    hs_smlFreePcdata( __FILE__, __LINE__, data->maxmem);
    hs_smlFreePcdata( __FILE__, __LINE__, data->maxid);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfSynccap(SmlDevInfSyncCapPtr_t data) {
    if (!data) return;
    hs_smlFreePcdataList(data->synctype);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfExt(SmlDevInfExtPtr_t data) {
    if (!data) return;
    hs_smlFreePcdata( __FILE__, __LINE__, data->xnam);
    hs_smlFreePcdataList(data->xval);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfExtList(SmlDevInfExtListPtr_t data) {
    if (!data) return;
    hs_smlFreeDevInfExt(data->data);
    hs_smlFreeDevInfExtList(data->next);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfCTData(SmlDevInfCTDataPtr_t data) {
    if (!data) return;
    hs_smlFreePcdata( __FILE__, __LINE__, data->name);
    hs_smlFreePcdata( __FILE__, __LINE__, data->dname);
    hs_smlFreePcdataList(data->valenum);
    hs_smlFreePcdata( __FILE__, __LINE__, data->datatype);
    hs_smlFreePcdata( __FILE__, __LINE__, data->size);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfCTDataProp(SmlDevInfCTDataPropPtr_t data) {
    if (!data) return;
    hs_smlFreeDevInfCTData(data->prop);
    hs_smlFreeDevInfCTDataList(data->param);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfCTDataList(SmlDevInfCTDataListPtr_t data) {
    if (!data) return;
    hs_smlFreeDevInfCTData(data->data);
    hs_smlFreeDevInfCTDataList(data->next);
    hs_smlLibFree(data);
}

static  void hs_smlFreeDevInfCTDataPropList(SmlDevInfCTDataPropListPtr_t data) {
    if (!data) return;
    hs_smlFreeDevInfCTDataProp(data->data);
    hs_smlFreeDevInfCTDataPropList(data->next);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfCTCap(SmlDevInfCTCapPtr_t data) {
    if (!data) return;
    hs_smlFreePcdata( __FILE__, __LINE__, data->cttype);
    hs_smlFreeDevInfCTDataPropList(data->prop);
    hs_smlLibFree(data);
}

static void hs_smlFreeDevInfCtcapList(SmlDevInfCtcapListPtr_t data) {
    if (!data) return;

    hs_smlFreeDevInfCTCap(data->data);
    hs_smlFreeDevInfCtcapList(data->next);
    hs_smlLibFree(data);
}
#endif


/**
 * FUNCTION: hs_smlString2Pcdata
 *
 * copy a string into a Pcdata structure
 *
 * IN:              String_t
 *                  Input String
 *
 * RETURN:          SmlPcdataPtr_t
 *                  A Pointer to a PcData structure
 *
 */
SmlPcdataPtr_t hs_smlString2Pcdata(const char * file,  int line, char* str)
{
	/* Definitions */ 
	SmlPcdataPtr_t pcdata;

	/* Invalid Input */ 
    	if (! str) 
		return NULL;

	/* Allocate the PcData Structure */
	pcdata = (SmlPcdataPtr_t)hs_smlLibMalloc(file, line, (long)sizeof(SmlPcdata_t));
	if (! pcdata) 
		return NULL;
    	hs_smlLibMemset (pcdata, 0, (long)sizeof(SmlPcdata_t));

	/* Set the PcData Structure */
	pcdata->contentType = SML_PCDATA_STRING;
	pcdata->extension=SML_EXT_UNDEFINED;
	pcdata->length = hs_smlLibStrlen( str );
	pcdata->content = (void*)hs_smlLibStrdup(str);

	return pcdata;
}

/**
 * FUNCTION: smlString2extPcdata
 *
 * copy a metainfo into a Pcdata structure
 *
 * IN:              String_t
 *                  Input String
 *
 * RETURN:          SmlPcdataPtr_t
 *                  A Pointer to a PcData structure
 *
 */
SmlPcdataPtr_t hs_smlmeta2extPcdata(const char * file,  int line, SmlMetInfMetInfPtr_t meta)
{
	/* Definitions */ 
	SmlPcdataPtr_t pcdata;

	/* Invalid Input */ 
    	if (! meta) 
		return NULL;

	/* Allocate the PcData Structure */
	pcdata = (SmlPcdataPtr_t)hs_smlLibMalloc(file, line, (long)sizeof(SmlPcdata_t));
	if (! pcdata) 
		return NULL;
    	hs_smlLibMemset (pcdata, 0, (long)sizeof(SmlPcdata_t));

	/* Set the PcData Structure */
	pcdata->contentType = SML_PCDATA_EXTENSION;
	pcdata->extension=SML_EXT_METINF;
	pcdata->length = sizeof( meta );
	pcdata->content = (void*)(meta);

	return pcdata;
}

/**
 * FUNCTION: hs_smldevinfo2extPcdata
 *
 * copy a device info into a Pcdata structure
 *
 * IN:              String_t
 *                  Input String
 *
 * RETURN:          SmlPcdataPtr_t
 *                  A Pointer to a PcData structure
 *
 */
SmlPcdataPtr_t hs_smldevinfo2extPcdata(const char * file,  int line, SmlDevInfDevInfPtr_t devinfo)
{
	/* Definitions */ 
	SmlPcdataPtr_t pcdata;

	/* Invalid Input */ 
    	if (! devinfo) 
		return NULL;

	/* Allocate the PcData Structure */
	pcdata = (SmlPcdataPtr_t)hs_smlLibMalloc(file, line, (long)sizeof(SmlPcdata_t));
	if (! pcdata) 
		return NULL;
    	hs_smlLibMemset (pcdata, 0, (long)sizeof(SmlPcdata_t));

	/* Set the PcData Structure */
	pcdata->contentType = SML_PCDATA_EXTENSION;
	pcdata->extension=SML_EXT_DEVINF;
	pcdata->length = sizeof( devinfo );
	pcdata->content = (void*)(devinfo);

	return pcdata;
}


/**
 * FUNCTION: hs_smlPcdata2String
 *
 * copy a Pcdata structure into a string
 *
 * IN:              SmlPcdataPtr_t
 *                  A Pointer to a PcData structure
 * RETURN:          String_t
 *                  Input String
 *
 */
char* hs_smlPcdata2String(const char * file,  int line, SmlPcdataPtr_t pcdata)
{
	/* Definitions */ 
	char* 	str;

	/* Invalid Input */ 
    	if (! pcdata) 
		return NULL;

    	/* Allocate the String */
    //syncml_cb_message:"MMIPIM @@hs_smlPcdata2String size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_836_112_2_18_3_25_24_0,(uint8*)"d",pcdata->length);

    str = (char*)hs_smlLibMalloc(file, line, (long)(pcdata->length+1));
    if (str == NULL)
		return NULL;

	/* Copy the string into the allocated data structure */
    	hs_smlLibMemcpy((unsigned char*)str, (unsigned char*)pcdata->content, pcdata->length);
    	*(str + pcdata->length) = '\0';  

	return str;
}

/*************************************************************/
//modify by wangxiaolin 

/************************************************************/
char* hs_smlunsignedInt2String(const char * file,  int line, unsigned int  integer)
{
      /* Definitions */ 
	char* 	str=NULL;
	char        onechar=0;
	int value=0;
	int left=0;
	int i=0;
	int j=0;
       /* Allocate the String */
       str = (char*)hs_smlLibMalloc(file, line, (long)10);
       if(str==NULL)
       {
            return NULL;
       }

       value=integer;
       do
       { 
	     left=value%10;
           value=value/10;
	     onechar=(char)(0x30+left);
	     *(str+j)=onechar;     //得到的是倒过来的十进制数
	     j++;
       }while(value!=0);
	   j--;
	for(i=0;i<j/2+1;i++)  //modify by wangxiaolin 修改两位时不正确
	{
	       onechar=*(str+i);
		*(str+i)=*(str+j-i);
		*(str+j-i)=onechar;
	}
	*(str+j+1)='\0';
	return str;
}

unsigned int hs_smlString2unsignedInt(char* str)
{
   int i;
   unsigned int temp;
   char achar;
   unsigned int ret=0;
   if(str==NULL)
   {
   		return ret;
   }
   //syncml_cb_message:"MMIPIM @@hs_smlString2unsignedInt =%s"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_897_112_2_18_3_25_24_1,(uint8*)"s",str);

   for(i=0;i<hs_smlLibStrlen(str);i++)
   {
      achar=*(str+i);
      switch(achar)
      {
         case '0':
         {
            temp=0;
         }break;
         case '1':
         {
            temp=1;
         }break;
         case '2':
         {
            temp=2;
         }break;
         case '3':
         {
            temp=3;
         }break;
         case '4':
         {
            temp=4;
         }break;
         case '5':
         {
            temp=5;
         }break;
         case '6':
         {
            temp=6;
         }break;
         case '7':
         {
            temp=7;
         }break;
         case '8':
         {
            temp=8;
         }break;
         case '9':
         {
            temp=9;
         }break;
         default:
         {
            return 0;
         }
      }
      ret=ret*10+temp;/*lint !e737*/
   }
  //syncml_cb_message:"MMIPIM  @@ leave hs_smlString2unsignedInt =%d"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_951_112_2_18_3_25_24_2,(uint8*)"d",ret);

   return ret;
}


  /**
 * Callback handling <SyncHdr> tag
 *
 * @param id instance ID
 * @param pSync pointer containing infomation about <SyncHdr> block
 * @return HS_SML_ERR_OK(=0)
 * @see hs_smlInitInstance
 */
short hs_myHandleStartMessage(short id, void* userData,
                           SmlSyncHdrPtr_t pSyncHdr)
{
   SmlStatusPtr_t  status;
   SmlTargetRefListPtr_t targetlist;
   SmlSourceRefListPtr_t sourcelist;

   status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
   if(NULL==status)
   {
        return hs_smlFreeProtoElement(pSyncHdr); 
   }
   
   targetlist=(SmlTargetRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTargetRefList_t));
   if(NULL==targetlist)
   {
        hs_smlLibFree(status);
        return hs_smlFreeProtoElement(pSyncHdr); 
   }
   sourcelist=(SmlSourceRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlSourceRefList_t));
   if(NULL==sourcelist)
   {
        hs_smlLibFree(status);
        hs_smlLibFree(targetlist);
        return hs_smlFreeProtoElement(pSyncHdr); 
   }
   hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
   status->targetRefList=targetlist;
   status->sourceRefList=sourcelist;

   //hs_status_cmd_que->msgref=pSyncHdr->msgID;
   hs_status_cmd_que->msgref=hs_smlString2Pcdata(__FILE__, __LINE__, pSyncHdr->msgID->content); // chenxiang_memory 20080223

   status->elementType=SML_PE_STATUS;
   //status->msgRef=hs_smlString2Pcdata(hs_status_cmd_que->msgref->content);
   status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, pSyncHdr->msgID->content);// chenxiang_memory 20080223
   status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, "0");//sync header is considered as use default cmdid 0
   status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "SyncHdr");
  // targetlist->targetRef=pSyncHdr->target->locURI;
   targetlist->targetRef=hs_smlString2Pcdata(__FILE__, __LINE__, pSyncHdr->target->locURI->content);// chenxiang_memory 20080223
   targetlist->next=NULL;
   //sourcelist->sourceRef=pSyncHdr->source->locURI;
   sourcelist->sourceRef=hs_smlString2Pcdata(__FILE__, __LINE__, pSyncHdr->source->locURI->content);// chenxiang_memory 20080223
   sourcelist->next=NULL;

   //增加对RespURI的处理
   if(NULL!=pSyncHdr->respURI)
   {
        hs_smlLibMemset(hs_http_globle->server,0,MMIPIM_MAX_URL_LEN);
        hs_smlLibMemset(hs_task_relay_info->syncml_pim_server_name,0,MMIPIM_MAX_URL_LEN);
    	  memcpy(hs_task_relay_info->syncml_pim_server_name,pSyncHdr->respURI->content,MIN(pSyncHdr->respURI->length,MMIPIM_MAX_URL_LEN));
	  memcpy(hs_http_globle->server,pSyncHdr->respURI->content,MIN(pSyncHdr->respURI->length,MMIPIM_MAX_URL_LEN));
   }
   
   if(*hs_protocol_step==STEP_SERVER_INIT_SYNC)
   {
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "212");
   }else{
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
   }
   
   hs_myAddstatustoCQ(status);
   //temp set authored is ok!
   hs_task_relay_info->authored=1;
    return hs_smlFreeProtoElement(pSyncHdr); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}

 short hs_myHandleEndMessage(short id, void * userData, unsigned char final)
{
    //do nothing
    return HS_SML_ERR_OK;
}

  /**
 * Callback handling <Sync> command
 *
 * @param id instance ID
 * @param pSync pointer containing infomation about <Sync> block
 * @return HS_SML_ERR_OK(=0)
 * @see hs_smlInitInstance
 */
short hs_myHandleStartSync(short id, void * userData,
                        SmlSyncPtr_t pSync)
{
   SmlStatusPtr_t  status;
   SmlTargetRefListPtr_t targetlist;
   SmlSourceRefListPtr_t sourcelist;
   
   PIM_T_P  pMe = MMIPIM_GetPIM(); // chenxiang_pim_ui 200080306
   status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
   if(NULL==status)
   {
        return hs_smlFreeProtoElement(pSync); 
   }
   
   targetlist=(SmlTargetRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTargetRefList_t));
   if(NULL==targetlist)
   {
        hs_smlLibFree(status);
        return hs_smlFreeProtoElement(pSync); 
   }
   sourcelist=(SmlSourceRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlSourceRefList_t));
   if(NULL==sourcelist)
   {
        hs_smlLibFree(status);
        hs_smlLibFree(targetlist);
        return hs_smlFreeProtoElement(pSync); 
   }
   hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
   status->targetRefList=targetlist;
   status->sourceRefList=sourcelist;

   status->elementType=SML_PE_STATUS;
   status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
   //status->cmdRef=pSync->cmdID;
   status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pSync->cmdID->content); // chenxiang_memory 20080223
   status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Sync");
   //targetlist->targetRef=pSync->target->locURI;
   targetlist->targetRef=hs_smlString2Pcdata(__FILE__, __LINE__, pSync->target->locURI->content); // chenxiang_memory 20080223
   targetlist->next=NULL;
   //sourcelist->sourceRef=pSync->source->locURI;
   sourcelist->sourceRef=hs_smlString2Pcdata(__FILE__, __LINE__, pSync->source->locURI->content); // chenxiang_memory 20080223
   sourcelist->next=NULL;

   status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");

   if (pSync->noc)
   { // chenxiang_pim_ui 200080306
       pMe->g_sync_result.server_number_of_change = (uint16)hs_smlString2unsignedInt(pSync->noc->content);
   }
   hs_myAddstatustoCQ(status);

    return hs_smlFreeProtoElement(pSync); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}


short hs_myHandleEndSync(short id, void * userData)
{
    //do nothing
    return HS_SML_ERR_OK;
}

#ifdef HS_ATOMIC_RECEIVE  /* these callbacks are NOT included in the Toolkit lite version */
short hs_myHandlestartAtomic(short id, void * userData,SmlAtomicPtr_t pContent)
{
    return HS_SML_ERR_OK;
}

short hs_myHandleendAtomic(short id, void * userData)
{
    return HS_SML_ERR_OK;
}
#endif

#ifdef HS_SEQUENCE_RECEIVE
short hs_myHandlestartSequence(short id, void * userData,SmlSequencePtr_t pContent)
{
    return HS_SML_ERR_OK;
}

short hs_myHandleendSequence(short id, void * userData)
{
    return HS_SML_ERR_OK;
}
#endif
  
/**
 * Callback handling <Add> command
 *
 * @param id instance ID
 * @param pSync pointer containing infomation about <Add> command
 * @return HS_SML_ERR_OK(=0)
 * @see hs_smlInitInstance
 */


/*==========================================================
 * author		: wangxiaolin      
 * function 	: hs_myHandlealert
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  8/11/2009 
 ==========================================================*/
short hs_myHandlealert(short id, void* userData, SmlAlertPtr_t pContent)
{
   SmlStatusPtr_t  status;
   SmlTargetRefListPtr_t targetlist;
   SmlSourceRefListPtr_t sourcelist;
   SmlItemListPtr_t itemlist;
   SmlItemPtr_t  item;
   SmlMetInfMetInfPtr_t   meta;
   SmlMetInfAnchorPtr_t  anchor;
   char* alertdata;
   int alertcode;

   status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
   if(NULL==status)
   {
       return hs_smlFreeProtoElement(pContent); 
   }
   
   targetlist=(SmlTargetRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTargetRefList_t));
   if(NULL==targetlist)
   {
       hs_smlLibFree(status);
       return hs_smlFreeProtoElement(pContent); 
   } 

   sourcelist=(SmlSourceRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlSourceRefList_t));
   if(NULL==sourcelist)
   {
       hs_smlLibFree(status);
       hs_smlLibFree(targetlist);
       return hs_smlFreeProtoElement(pContent); 
   } 
   
   itemlist=(SmlItemListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlItemList_t));
   if(NULL==itemlist)
   {
       hs_smlLibFree(status);
       hs_smlLibFree(targetlist);
       hs_smlLibFree(sourcelist);
       return hs_smlFreeProtoElement(pContent); 
   } 
   
   item=(SmlItemPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlItem_t));
   if(NULL==item)
   {
       hs_smlLibFree(status);
       hs_smlLibFree(targetlist);
       hs_smlLibFree(sourcelist);
       hs_smlLibFree(itemlist);
       return hs_smlFreeProtoElement(pContent); 
   } 

   meta=(SmlMetInfMetInfPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlMetInfMetInf_t));
   if(NULL==meta)
   {
       hs_smlLibFree(status);
       hs_smlLibFree(targetlist);
       hs_smlLibFree(sourcelist);
       hs_smlLibFree(itemlist);
       hs_smlLibFree(item);
       return hs_smlFreeProtoElement(pContent); 
   } 

   anchor=(SmlMetInfAnchorPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlMetInfAnchor_t));
   if(NULL==anchor)
   {
       hs_smlLibFree(status);
       hs_smlLibFree(targetlist);
       hs_smlLibFree(sourcelist);
       hs_smlLibFree(itemlist);
       hs_smlLibFree(item);
       hs_smlLibFree(meta);
       return hs_smlFreeProtoElement(pContent); 
   } 
   
   hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
   hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));
   hs_smlLibMemset(meta, 0, (long)sizeof(SmlMetInfMetInf_t));
   hs_smlLibMemset(anchor, 0, (long)sizeof(SmlMetInfAnchor_t));
   
   status->elementType=SML_PE_STATUS;
   status->targetRefList=targetlist;
   status->sourceRefList=sourcelist;  
   status->itemList=itemlist;
   itemlist->item=item;
   
   meta->anchor=anchor;
  // anchor->next=((SmlMetInfMetInf_t*)(pContent->itemList->item->meta->content))->anchor->next;
   anchor->next=hs_smlString2Pcdata(__FILE__, __LINE__, ((SmlMetInfMetInf_t*)(pContent->itemList->item->meta->content))->anchor->next->content); // chenxiang_memory 20080223
   anchor->last=NULL;
   
   status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
   //status->cmdRef=pContent->cmdID;
   status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->cmdID->content); // chenxiang_memory 20080223
   status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Alert");
   //status->targetRefList->targetRef=pContent->itemList->item->target->locURI;
   status->targetRefList->targetRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->target->locURI->content); // chenxiang_memory 20080223
   status->targetRefList->next=NULL;
   //status->sourceRefList->sourceRef=pContent->itemList->item->source->locURI;
   status->sourceRefList->sourceRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->source->locURI->content); // chenxiang_memory 20080223
   status->sourceRefList->next=NULL;
   
   //here is the server alert  data  process
   //syncml_cb_message:"MMIPIM @@handle alert@@"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1251_112_2_18_3_25_24_3,(uint8*)"");
   alertdata=hs_smlPcdata2String(__FILE__, __LINE__, pContent->data);
   alertcode=atoi(alertdata);
   hs_smlLibFree(alertdata);
   switch(alertcode)
   {
      case 200:   //alert for two way sync
      {
         //syncml_cb_message:"MMIPIM @@handle alert@@ alert for two way sync"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1259_112_2_18_3_25_24_4,(uint8*)"");
         hs_task_relay_info->synctype=SYNC_TYPE_2WAY;
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
      }break;
      case 201:   //alert for slow sync
      {
        //syncml_cb_message:"MMIPIM @@handle alert@@ alert for slow sync"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1265_112_2_18_3_25_24_5,(uint8*)"");
         hs_task_relay_info->synctype=SYNC_TYPE_S2WAY;
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
      }break;
      case 222:   //alert for more
      {
	  //syncml_cb_message:"MMIPIM @@handle alert@@alert for more"
	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1271_112_2_18_3_25_24_6,(uint8*)"");
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
      }break;

#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for 单向同步&  刷新同步
      case 202:
      {
        //syncml_cb_message:"MMIPIM @@handle alert@@ alert for one way sync from client only!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1278_112_2_18_3_25_24_7,(uint8*)"");
         hs_task_relay_info->synctype=SYNC_TYPE_1WAYFCLI;
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
      }
      break;
      
      case 203:
      {
        //syncml_cb_message:"MMIPIM @@handle alert@@ alert for refresh sync from client only!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1286_112_2_18_3_25_25_8,(uint8*)"");
         hs_task_relay_info->synctype=SYNC_TYPE_RFCLI;
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
      }
      break;
      
      case 204:
      {
        //syncml_message:"MMIPIM @@handle alert@@ alert for one way sync from server only!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1294_112_2_18_3_25_25_9,(uint8*)"");
         hs_task_relay_info->synctype=SYNC_TYPE_1WAYFSER;
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
      }
      break;
      
      case 205:
              {
        //syncml_message:"MMIPIM @@handle alert@@ alert for refresh sync from server only!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1302_112_2_18_3_25_25_10,(uint8*)"");
         hs_task_relay_info->synctype=SYNC_TYPE_RFSER;
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");
      }
      break;
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
      
      case 206:
      case 207:
      case 208:
      case 209:
      case 210:
      case 221:
      case 223:
      default:
      {
         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "405");
      }break;         
   }
   
   
   status->itemList->item->meta=hs_smlmeta2extPcdata(__FILE__, __LINE__, meta);
   status->itemList->next=NULL;
   
   hs_myAddstatustoCQ(status);

    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}


short hs_myHandleget(short id, void* userData, SmlGetPtr_t pContent)
{
    //server function
    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}

short hs_myHandleput(short id, void* userData, SmlPutPtr_t pContent)
{
    //server function
    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}

#ifdef HS_MAP_RECEIVE
short hs_myHandlemap(short id, void* userData, SmlMapPtr_t pContent)
{
    //server function
    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}
#endif

#ifdef HS_RESULT_RECEIVE
short hs_myHandleresults(short id, void* userData, SmlResultsPtr_t pContent)
{
    //server function
    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}
#endif



#ifdef HS_COPY_RECEIVE  /* these callbacks are NOT included in the Toolkit lite version */
short hs_myHandlecopy(short id, void* userData, SmlCopyPtr_t param)
{
    return hs_smlFreeProtoElement(param); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}
#endif

#ifdef HS_EXEC_RECEIVE
short hs_myHandleexec(short id, void* userData, SmlExecPtr_t pContent)
{
    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}
#endif

#ifdef HS_SEARCH_RECEIVE
short hs_myHandlesearch(short id, void* userData, SmlSearchPtr_t pContent)
{
    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}
#endif

short hs_myHandleFinal(void)
{
    //syncml_cb_message:"^@@^MMIPIM hs_myHandleFinal !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1385_112_2_18_3_25_25_11,(uint8*)"");
   *hs_protocol_step_priv=*hs_protocol_step;
   
   return HS_SML_ERR_OK;
}

short hs_myHandlehandleError(short id, void* userData)
{
    return HS_SML_ERR_OK;
}

short hs_myHandletransmitChunk(short id, void* userData)
{
    return HS_SML_ERR_OK;
}

/******************************************************************* 
FUNCTION 函数名:hs_myAddstatustoCQ
功能描述：将status加入status command queue。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
****************************************************************/
short hs_myAddstatustoCQ(SmlStatusPtr_t status)
{
      status_element_type* temp = PNULL;
      status_element_type* temptr = PNULL;

      temp=(status_element_type*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(status_element_type));
      if(NULL==temp)
      {
           //hs_smlFreeStatus(temptr->status);
           return HS_SML_ERR_OK;
      }
      
      temp->status=status;
      
      temp->next=NULL;
	  
      temptr=hs_status_cmd_que->queue;

      if(hs_status_cmd_que->totalnumber==0)
      {
           hs_status_cmd_que->queue=temp;
	    hs_status_cmd_que->totalnumber++;
           return HS_SML_ERR_OK;
      }
	  
      while(temptr->next!=NULL)
      {
            temptr = temptr->next;
      	}
	  
      temptr->next=temp;
      hs_status_cmd_que->totalnumber++;
	  
      return HS_SML_ERR_OK;
}

/******************************************************************* 
FUNCTION 函数名:hs_myFreestatusofCQ
功能描述：将status command queue中的status动态申请内存
**                       释放。
** 输  出: null
** 作  者:刘刚
** 日  期:2005-5-10
** 版本: ver 1.0
****************************************************************/
short hs_myFreestatusofCQ(void)
{
        status_element_type* temptr;
	 status_element_type* next;
	 temptr=hs_status_cmd_que->queue;
	 next=temptr->next;
	 while((next!=NULL)&&(hs_status_cmd_que->totalnumber>0))
        {
            temptr->next=next->next;  //断开一个节点
            hs_smlFreeStatus(next->status);
	     hs_smlLibFree(next);
	     next=temptr->next;
	     hs_status_cmd_que->totalnumber--;
        }
     
        if(hs_status_cmd_que->totalnumber>0)
        {
        	 hs_smlFreeStatus(temptr->status);
        	 hs_smlLibFree(temptr);
        	 hs_status_cmd_que->totalnumber--;
        	 hs_smlFreePcdata( __FILE__, __LINE__, hs_status_cmd_que->msgref);
        }
	 return HS_SML_ERR_OK;
}







/******************************************************************* 
FUNCTION 函数名:hs_myGetMapedItems
功能描述：得到添加成功的map Items List.
** 输  出: itemlist连表第一个指针。也就是MapItemList全局变量
                    链表头。此链表在myHandleAdd中生成。
** 作  者:刘刚
** 日  期:2006-8-2
** 版本: ver 1.0
**注意:动态申请的内存，将在它的调用者syncml_MapCmd
**             中释放。
****************************************************************/
SmlMapItemListPtr_t hs_ConvertNewMapListToMapList(SmlNewMapItemListPtr_t new_map_list)
{
    SmlNewMapItemListPtr_t new_tempPtr = new_map_list;
    SmlNewMapItemListPtr_t new_tempPtr2 = NULL;
    SmlMapItemListPtr_t maplist=NULL;
    SmlMapItemListPtr_t templist=NULL;
    SmlMapItemListPtr_t headlist=NULL;
    SmlMapItemPtr_t      mapitem=NULL;
    SmlSourcePtr_t        mapsource=NULL;
    SmlTargetPtr_t         maptarget=NULL;
    
    while (NULL != new_tempPtr)
    {
        maplist=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlMapItemList_t));
        if(NULL==maplist)
        {       
           while (NULL != new_tempPtr)
           {
                new_tempPtr2 = new_tempPtr;
                new_tempPtr = new_tempPtr->next;
                hs_smlLibFree(new_tempPtr2);
           }
            return NULL;
        }
        hs_smlLibMemset(maplist, 0, (long)sizeof(SmlMapItemList_t));
        
        mapitem=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlMapItem_t));
        if(NULL==mapitem)
        {
            hs_smlLibFree(maplist);
            while (NULL != new_tempPtr)
            {
                 new_tempPtr2 = new_tempPtr;
                 new_tempPtr = new_tempPtr->next;
                 hs_smlLibFree(new_tempPtr2);
            }
            return NULL;
        }
        hs_smlLibMemset(mapitem, 0, (long)sizeof(SmlMapItem_t));
        
        mapsource=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlSource_t));
        if(NULL==mapsource)
        {
            hs_smlLibFree(maplist);
            hs_smlLibFree(mapitem);
            while (NULL != new_tempPtr)
            {
                new_tempPtr2 = new_tempPtr;
                new_tempPtr = new_tempPtr->next;
                hs_smlLibFree(new_tempPtr2);
            }
            return NULL;
        }
        hs_smlLibMemset(mapsource, 0, (long)sizeof(SmlSource_t));
        
        maptarget=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTarget_t));
        if(NULL==maptarget)
        {
            hs_smlLibFree(maplist);
            hs_smlLibFree(mapitem);
            hs_smlLibFree(mapsource);
            while (NULL != new_tempPtr)
            {
                new_tempPtr2 = new_tempPtr;
                new_tempPtr = new_tempPtr->next;
                hs_smlLibFree(new_tempPtr2);
            }
            return NULL;
        }
        hs_smlLibMemset(maptarget, 0, (long)sizeof(SmlTarget_t));

        maplist->mapItem=mapitem;
        maplist->next=NULL;
        mapitem->source=mapsource;
        mapitem->target=maptarget;
        maptarget->locName=NULL;
        maptarget->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, new_tempPtr->target);  // GUID
        mapsource->locName=NULL;
        mapsource->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, new_tempPtr->source);   // LUID

        if (NULL == headlist)
        {
            headlist = maplist;
        }
        else if (NULL != templist)
        {
            templist->next = maplist;
        }
        templist = maplist;
        new_tempPtr2 = new_tempPtr;
        new_tempPtr = new_tempPtr->next;
        hs_smlLibFree(new_tempPtr2);
    }
    
    return headlist;
}
/*==========================================================
 * author		: wangxiaolin       
 * function 	: hs_myGetMapedItems
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  1/11/2010 
 ==========================================================*/
SmlNewMapItemListPtr_t hs_myGetMapedItems(void)
{
     /* chenxiang_map 20080325 begin */
    SmlNewMapItemListPtr_t tempPtr2 = NULL;
    uint16 count = 0;
    SmlNewMapItemListPtr_t tempPtr = hs_globlemapItemList;
    
    while ((120 > (++count)) && (NULL != tempPtr))
    {
        tempPtr = tempPtr->next;
    }
    if (NULL != tempPtr)
    {
        tempPtr2 = hs_globlemapItemList;
        hs_globlemapItemList = tempPtr->next;
        tempPtr->next = NULL;
        return tempPtr2;
    }
    else
    {
        tempPtr2 = hs_globlemapItemList;
        hs_globlemapItemList = NULL;
        return tempPtr2;
    }
}

/******************************************************************* 
FUNCTION 函数名:hs_myGetMSFreeMemery
功能描述：得到当前的syncml最大可使用内存。
** 输  出: malloc 了一个Smlpcdataptr型数据，他将在生成syncml
                  文档后由syncml core释放。
** 作  者:刘刚
** 日  期:2006-8-28
** 版本: ver 1.0
**注意:动态申请的内存，将在生成syncml
                  文档后由syncml core释放。
****************************************************************/
SmlPcdataPtr_t hs_myGetMSFreeMemery(void)
{
   SmlPcdataPtr_t pcdata;
   char* memfree=NULL;
   memfree=hs_smlunsignedInt2String(__FILE__, __LINE__, (unsigned int)(hs_pGlobalAnchor->syncmlOptions->maxWorkspaceAvailMem));
   pcdata=hs_smlString2Pcdata(__FILE__, __LINE__, memfree);
   hs_smlLibFree(memfree);
   return pcdata;
}


/******************************************************************* 
FUNCTION 函数名:hs_myGetNumberofChanges
功能描述：得到当前的需要同步的条目数
** 输  出: malloc 了一个Smlpcdataptr型数据，他将在生成syncml
                  文档后由syncml core释放。
** 作  者:刘刚
** 日  期:2006-8-28
** 版本: ver 1.0
**注意:动态申请的内存，将在生成syncml
                  文档后由syncml core释放。
****************************************************************/
SmlPcdataPtr_t hs_myGetNumberofChanges(void)
{
   SmlPcdataPtr_t pcdata;
   char* noc=NULL;
   uint16 add_num=0;
   uint16 replace_num=0;
   uint16 del_num=0;
   uint16 total_num=0;
   PIM_T_P  pMe=MMIPIM_GetPIM();//add by wangxiaolin
   //syncml_efs_message:"MMIPIM &&& enter hs_myGetNumberofChanges"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1663_112_2_18_3_25_25_12,(uint8*)"");

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
   add_num = g_pim.m_recent_status_count.recent_add_count; 
   replace_num = g_pim.m_recent_status_count.recent_replace_count; 
   del_num = g_pim.m_recent_status_count.recent_delete_count; 
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    }
    else
    {
       add_num = g_pim.m_vcal_recent_status_count.recent_add_count; 
       replace_num = g_pim.m_vcal_recent_status_count.recent_replace_count; 
       del_num = g_pim.m_vcal_recent_status_count.recent_delete_count; 
    }
#endif /* HS_PIM_VCAL */

   /* add by wangxiaolin */
   pMe->g_sync_result.added_num_for_server=add_num;
   pMe->g_sync_result.deleted_num_for_server=del_num;
   pMe->g_sync_result.replaced_num_for_server=replace_num;
   /*   end of add  */

   total_num=(uint16)(add_num+replace_num+del_num);
   pMe->g_sync_result.client_number_of_change = total_num;
   noc=hs_smlunsignedInt2String(__FILE__, __LINE__, total_num);
   pcdata=hs_smlString2Pcdata(__FILE__, __LINE__, noc);
   hs_smlLibFree(noc);
   //syncml_efs_message:"MMIPIM &&& add_num=%d,del=%d,rep=%d,num=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1693_112_2_18_3_25_25_13,(uint8*)"dddd",add_num,del_num,replace_num,total_num);
   return pcdata;
}

/******************************************************************* 
FUNCTION 函数名:hs_myGetNumberofAllItem
功能描述：得到慢同步时后所有记录总数
** 输  出: malloc 了一个Smlpcdataptr型数据，他将在生成syncml
                  文档后由syncml core释放。
** 作  者:王晓林
** 日  期:2007-12.06
** 版本: ver 1.0
**注意:动态申请的内存，将在生成syncml
                  文档后由syncml core释放。
****************************************************************/
SmlPcdataPtr_t hs_myGetNumberofAllItem(void)
{
   SmlPcdataPtr_t pcdata;
   char* noc=NULL;
   uint16 total_num=0;
   PIM_T_P  pMe=MMIPIM_GetPIM();
   //syncml_efs_message:"MMIPIM &&& enter hs_myGetNumberofAllItem"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1713_112_2_18_3_25_25_14,(uint8*)"");
   total_num=(uint16)hs_myGetAllPBItemsCount();
   pMe->g_sync_result.added_num_for_server=total_num;
   pMe->g_sync_result.deleted_num_for_server=0;
   pMe->g_sync_result.replaced_num_for_server=0;
   pMe->g_sync_result.client_number_of_change = total_num;
   noc=hs_smlunsignedInt2String(__FILE__, __LINE__, total_num);
   pcdata=hs_smlString2Pcdata(__FILE__, __LINE__, noc);
   hs_smlLibFree(noc);
  
   return pcdata;
}



//these are for syncml core test on windows. liugang make it disabled.2005-11-7.
//make these changed to save logs on efs in debug model.  liugang 2006-5-29.

/*==========================================================
 * author		: wangxiaolin      
 * function 	: hs_myPrintbuffer
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/10/2009 
 ==========================================================*/
void hs_myPrintbuffer(char *pbuffer,int buf_lenth)
{
    #ifdef FEATURE_PIM_DEBUG 

     int i=0;
     //syncml_efs_message:"MMIPIM &&&buf_lenth=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1744_112_2_18_3_25_25_15,(uint8*)"d",buf_lenth);
     for(i=0;i<20;i++) // chenxiang 20080129 15-->20
     {
           char temp_print_buffer[400]={0};
           hs_smlLibMemcpy(temp_print_buffer,pbuffer+i*230,230);
           syncml_efs_message(("MMIPIM &&& content=%s",temp_print_buffer)); // chenxiang 20080129 250-->229
     }
   #endif
    return;
}



/******************************************************************* 
FUNCTION 函数名:hs_myhttplogs
功能描述：打印出交互的http 协议文件(T&R)
** 输  出: null
** 作  者:刘刚
** 日  期:2006-6-6
** 版本: ver 1.0
****************************************************************/

void hs_myhttplogs(char* buffer, uint32  length)
{
  #ifdef FEATURE_PIM_DEBUG 
    //syncml_efs_message:"MMIPIM &&& hs_myhttplogs"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMCALLBACK_HANDLER_1767_112_2_18_3_25_26_16,(uint8*)"");
    if(buffer!=NULL)
    {
        hs_myPrintbuffer((char*)buffer,length);
    }
   #endif
 return;
}
