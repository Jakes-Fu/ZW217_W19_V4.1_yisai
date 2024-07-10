/****************************************************************************
** File Name:      MMIDM_PARSEXML.C                                               *
** Author:                                                                 *
** Date:           28/04/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the MMI DM XML PARSE             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2011       maryxiao       解析syncxml协议dm部分xml      *
****************************************************************************/
#define MMIPARSEXML_C

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmidm_app.h"
#include "mmi_id.h"
#include "mmidm_text.h"
//#include "mmi_appmsg.h"
#include "mmidm_internal.h"
//#include "mmidm_config.h"
#include "mmidm_parsexml.h"

#include "xml_sap.h"

//#define  DFRESH_TRACE_SWITCH //只控制非必须的TRACE

DM_PARSE_DATA_T g_parse_data={0};//store parse xml data
DM_STORE_DATA_T g_store_data ={0};//store xml tag data
DM_STATE_DATA_T g_state_data = {0};//xml state data
/*****************************************************************************/
//  Description : 解析收到的xml包，放入相应链表
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void EndDmParse(void);

/*****************************************************************************/
//  Description :set a uri which to send 
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_setResUri(char* ResUri_content);


/*****************************************************************************/
//  Description : release all the node malloc
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_releaseItemContent(DMXML_TAG_ITEM_T* item_tag);
/*****************************************************************************/
//  Description : release all the node malloc
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_ReleaseXMLData(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/


extern PUBLIC int32 CFLBASE64_Decode(
                                                char *out_ptr,      // [OUT] Where to save the converted string
                                                int32 out_len,      // [IN]  Buffer length, out_len = in_len ( suggest! )
                                                const char *in_ptr, // [IN]  String to convert 
                                                int32 in_len        // [IN]  Length of the string to be converted
                                                );




/*****************************************************************************/
//  Description : TAG STRING TABLE
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL const DMXML_TAG_INFO_T  s_dmxml_tag_tab[] = 
{
    {"VerDTD",                       TAG_VERDTD},
    {"VerProto",                     TAG_VERPROTO},
    {"SessionID",                    TAG_SESSIONID},
    {"MsgID",                        TAG_MSGID},
    {"Target",                       TAG_TARGETID},
    {"Source",                       TAG_SOURCEID},
    {"LocURI",                       TAG_LOCURIID},
    {"LocName",                      TAG_LOCNAMEID},
    {"Cred",                         TAG_CREDID},
    {"Meta",                         TAG_METAID}, 
    {"MaxMsgSize",                   TAG_MAXMSGID}, 
    {"MaxObjSize",                   TAG_MAXOBJID},
    {"Data",                         TAG_DATAID},
    {"SyncHdr",                      TAG_SYNCHDRID},
    {"SyncBody",                     TAG_SYNCBODYID},
    {"Alert",                        TAG_ALERTID},
    {"Replace",                      TAG_REPLACEID},
    {"Status",                       TAG_STATUSID},
    {"Results",                      TAG_RESULTID},
    {"Cmd",                          TAG_CMDID},
    {"CmdID",                        TAG_CMDIDID},
    {"MsgRef",                       TAG_MSGREFID},
    {"CmdRef",                       TAG_CMDREFID},
    {"TargetRef",                    TAG_TARGETREFID},
    {"SourceRef",                    TAG_SOURCEREFID}, 
    {"Chal",                         TAG_CHALID}, 
    {"Item",                         TAG_ITEMID}, 
    {"Format",                       TAG_FORMATID}, 
    {"Type",                         TAG_TYPEID}, 
    {"NextNonce",                    TAG_NEXTNONCEID},
    {"RespURI",                      TAG_RESURIID},
    {"Get",                          TAG_GETID}, 
    {"SyncML",                       TAG_SYNCMLID},
    {"Exec",                         TAG_EXECID},
};



extern PUBLIC VDM_Tree_ReadExternalCB DM_GET_TREE_READFUNC(char* path);
extern PUBLIC VDM_Tree_WriteExternalCB DM_GET_TREE_WRITEFUNC(char* path);


/*****************************************************************************/
//  Description :设置session的id
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_setSessionId(uint32 id)
{
   SCI_TRACE_LOW("[MMIDM] MMIDM_setSessionId id %d", id);
   g_state_data.s_g_SessionID = id; 
}


/*****************************************************************************/
//  Description :设置s_g_needreplay flag
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetIsNeedReplay(BOOLEAN is_need_replay)
{
    SCI_TRACE_LOW("MMIDM==> MMIDM_SetIsNeedReplay  is_need_replay=%d",is_need_replay);
    g_state_data.s_g_needreplay = is_need_replay;
}

/*****************************************************************************/
//  Description :设置session的step
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_setSessionStep(MMI_DM_STEP_E step)
{
   g_state_data.s_g_step = step; 
}

/*****************************************************************************/
//  Description :通过id获得tag的name str
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIDM_GetTagStr(MMI_DM_TAGID_E id)
{
    return s_dmxml_tag_tab[id].tagtype;/*lint !e605*/
}

/*****************************************************************************/
//  Description :通过name str获得tag的id
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL MMI_DM_TAGID_E MMIDM_GetTagId(char* name)
{
    MMI_DM_TAGID_E id = TAG_VERDTD;
    for(id = TAG_VERDTD; id<TAG_MAXID; id++)
    {
        if(!strcmp(s_dmxml_tag_tab[id].tagtype, name))
        {
            return id;
        }
    }
    return id;/*lint !e605*/
}
/*****************************************************************************/
//  Description :set a uri which to send 
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_setResUri(char* ResUri_content)
{
    SCI_TRACE_LOW("[MMIDM] MMIDM_setResUri %s", ResUri_content);
    if(MAX_RESURI_LEN > strlen(ResUri_content))
    {
        SCI_STRCPY(g_state_data.s_g_resUri, ResUri_content);
    }
    else
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_setResUri the buf is too small %d", strlen(ResUri_content));
    }
}
/*****************************************************************************/
//  Description :get a uri which to send 
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIDM_getResUri(void)
{
    if(!strlen(g_state_data.s_g_resUri))
    {
        MMIDM_GetDmParaInfo(DM_SRV_ADDR, g_state_data.s_g_resUri);
    }
    if(g_state_data.s_g_step != STEP_CREDED && g_state_data.s_g_step != STEP_GETNONCE_AGAIN)
    {
        MMIDM_GetDmParaInfo(DM_SRV_ADDR, g_state_data.s_g_resUri);
    }
    SCI_TRACE_LOW("[MMIDM] MMIDM_getResUri %s", g_state_data.s_g_resUri);
    return g_state_data.s_g_resUri;
}
/*****************************************************************************/
//  Description : when a session ended clear the data
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_clearDmXmlData(void)
{
   // MMIDM_setResUri("");
    g_state_data.s_g_MsgID = 1;
    g_state_data.s_g_CmdID = 1;
    g_state_data.s_g_step = STEP_GETNONCE;
}


/*****************************************************************************/
//  Description : for the alert confirm window to callback which result the user choose
//  Global resource dependence : 
//  iscontinue: true: yes
//              false: no
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_NotifyAlertResult(BOOL iscontinue)
{    
    char confirm_result[16] = {0};

    SCI_TRACE_LOW("[MMIDM]MMIDM_NotifyAlertResult:g_state_data.s_g_needreplay =%d",g_state_data.s_g_needreplay);

    if(iscontinue)
    {
        SCI_STRCPY(confirm_result, STATUS_OK);/*lint !e779*/
    }
    else
    {
        SCI_STRCPY(confirm_result, STATUS_CANCEL);/*lint !e779*/
    }
    g_state_data.s_g_needreplay = FALSE;
    if(!MMIDM_ConstrucTag(&(g_parse_data.s_statusTag_cur->data),TAG_DATAID,PNULL, confirm_result, FALSE))
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
        return;
    }
#ifdef WIN32
    MMIDM_SendDmData();
#else
    MMIDM_SendSigToDmTask(DM_TASK_START_MESSAGE,MMIDM_GetDmTaskID(),PNULL);
#endif

}

/*****************************************************************************/
//  Description : release all the node malloc
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_releaseItemContent(DMXML_TAG_ITEM_T* item_tag)
{
    if(PNULL == item_tag)
    {
        SCI_TRACE_LOW("[mmidm] MMIDM_releaseItemContent item_tag == PNULL");
        return;
    }
    if(PNULL != item_tag->data.tagContent)
    {
        SCI_FREE(item_tag->data.tagContent);
        item_tag->data.tagContent = PNULL;
    }
     if(PNULL != item_tag->meta.format.tagContent)
    {
        SCI_FREE(item_tag->meta.format.tagContent);
        item_tag->meta.format.tagContent = PNULL;
    }
     if(PNULL != item_tag->meta.nextnonce.tagContent)
    {
        SCI_FREE(item_tag->meta.nextnonce.tagContent);
        item_tag->meta.nextnonce.tagContent = PNULL;
    }
     if(PNULL != item_tag->meta.type.tagContent)
    {
        SCI_FREE(item_tag->meta.type.tagContent);
        item_tag->meta.type.tagContent = PNULL;
    }
     if(PNULL != item_tag->source.locname.tagContent)
    {
        SCI_FREE(item_tag->source.locname.tagContent);
        item_tag->source.locname.tagContent = PNULL;
    }
     if(PNULL != item_tag->source.locuri.tagContent)
    {
        SCI_FREE(item_tag->source.locuri.tagContent);
        item_tag->source.locuri.tagContent = PNULL;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_releaseResultContent(DMXML_TAG_RESULT_T* result_tag)
{
    if(PNULL == result_tag)
    {
        SCI_TRACE_LOW("[mmidm] MMIDM_releaseResultContent result_tag == PNULL");
        return;
    }
    if(PNULL != result_tag->cmd.tagContent)
    {
        SCI_FREE(result_tag->cmd.tagContent);
        result_tag->cmd.tagContent = PNULL;
    }
    if(PNULL != result_tag->CmdId.tagContent)
    {
        SCI_FREE(result_tag->CmdId.tagContent);
        result_tag->CmdId.tagContent = PNULL;
    }
    if(PNULL != result_tag->cmdRef.tagContent)
    {
        SCI_FREE(result_tag->cmdRef.tagContent);
        result_tag->cmdRef.tagContent = PNULL;
    }
    if(PNULL != result_tag->msgRef.tagContent)
    {
        SCI_FREE(result_tag->msgRef.tagContent);
        result_tag->msgRef.tagContent = PNULL;
    }
    if(PNULL != result_tag->sourceRef.tagContent)
    {
        SCI_FREE(result_tag->sourceRef.tagContent);
        result_tag->sourceRef.tagContent = PNULL;
    }
    if(PNULL != result_tag->targetRef.tagContent)
    {
        SCI_FREE(result_tag->targetRef.tagContent);
        result_tag->targetRef.tagContent = PNULL;
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_releaseStatusContent(DMXML_TAG_STATUS_T* status_tag)
{
        int i = 0;
        if(PNULL == status_tag)
        {
            SCI_TRACE_LOW("[mmidm] MMIDM_releaseStatusContent status_tag == PNULL");
            return;
        }
        if(PNULL != status_tag->cmd.tagContent)
        {
            SCI_FREE(status_tag->cmd.tagContent);
            status_tag->cmd.tagContent = PNULL;
        }
        if(PNULL != status_tag->cmd.tagContent)
        {
            SCI_FREE(status_tag->CmdId.tagContent);
            status_tag->CmdId.tagContent = PNULL;
        }
        if(PNULL != status_tag->cmdRef.tagContent)
        {
            SCI_FREE(status_tag->cmdRef.tagContent);
            status_tag->cmdRef.tagContent = PNULL;
        }
        if(PNULL != status_tag->data.tagContent)
        {
            SCI_FREE(status_tag->data.tagContent);
            status_tag->data.tagContent = PNULL;
        }   
        if(PNULL != status_tag->msgRef.tagContent)
        {
            SCI_FREE(status_tag->msgRef.tagContent);
            status_tag->msgRef.tagContent = PNULL;
        }    
        if(PNULL != status_tag->sourceRef.tagContent)
        {
            SCI_FREE(status_tag->sourceRef.tagContent);
            status_tag->sourceRef.tagContent = PNULL;
        }  
	    for(i=0; i<DM_MAX_TARGETREF_NUM; i++)
	    {
	        if(PNULL != status_tag->targetRef[i].tagContent)
	        {
	            SCI_FREE(status_tag->targetRef[i].tagContent);
	            status_tag->targetRef[i].tagContent = PNULL;
	        }  

	    }
        if(PNULL != status_tag->chal.meta.format.tagContent)
        {
            SCI_FREE(status_tag->chal.meta.format.tagContent);
            status_tag->chal.meta.format.tagContent = PNULL;
        } 
        if(PNULL != status_tag->chal.meta.nextnonce.tagContent)
        {
            SCI_FREE(status_tag->chal.meta.nextnonce.tagContent);
            status_tag->chal.meta.nextnonce.tagContent = PNULL;
        }
        if(PNULL != status_tag->chal.meta.type.tagContent)
        {
            SCI_FREE(status_tag->chal.meta.type.tagContent);
            status_tag->chal.meta.type.tagContent = PNULL;
        }    
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_releaseReplaceContent(DMXML_TAG_REPLACE_T* replace_tag)
{
        if(PNULL == replace_tag)
        {
            SCI_TRACE_LOW("[mmidm] MMIDM_releaseReplaceContent replace_tag == PNULL");
            return;
        }
        if(PNULL != replace_tag->CmdId.tagContent)
        {
            SCI_FREE(replace_tag->CmdId.tagContent);
            replace_tag->CmdId.tagContent = PNULL;
        }      
}
/*****************************************************************************/
//  Description : release all the node malloc
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_ReleaseXMLData(void)
{
    DMXML_TAG_RESULT_T* result_cur_tag = PNULL;
    DMXML_TAG_STATUS_T* status_cur_tag = PNULL;
    DMXML_TAG_REPLACE_T* replace_cur_tag = PNULL;
    DMXML_TAG_ITEM_T*       item_tag = PNULL;

    //release result tag buf
    while(PNULL!=g_parse_data.s_resultTag_head)
    {
        while(PNULL!=g_parse_data.s_resultTag_head->item_ptr)
        {
            
            item_tag = g_parse_data.s_resultTag_head->item_ptr->next;
            MMIDM_releaseItemContent(g_parse_data.s_resultTag_head->item_ptr);
            SCI_FREE(g_parse_data.s_resultTag_head->item_ptr);
            g_parse_data.s_resultTag_head->item_ptr= item_tag;
        }
        
        result_cur_tag = g_parse_data.s_resultTag_head->next;
        MMIDM_releaseResultContent(g_parse_data.s_resultTag_head);
        SCI_FREE(g_parse_data.s_resultTag_head);
        g_parse_data.s_resultTag_head= result_cur_tag;
    }
    //release status tag buf
    while(PNULL!=g_parse_data.s_statusTag_head)
    {
        
        status_cur_tag = g_parse_data.s_statusTag_head->next;
        MMIDM_releaseStatusContent(g_parse_data.s_statusTag_head);
        SCI_FREE(g_parse_data.s_statusTag_head);
        g_parse_data.s_statusTag_head= status_cur_tag;
    }
    //release replace tag buf
    while(PNULL!=g_parse_data.s_replaceTag_head)
    {
        while(PNULL!=g_parse_data.s_replaceTag_head->item_ptr)
        {
            item_tag = g_parse_data.s_replaceTag_head->item_ptr->next;
            MMIDM_releaseItemContent(g_parse_data.s_replaceTag_head->item_ptr);
            SCI_FREE(g_parse_data.s_replaceTag_head->item_ptr);
            g_parse_data.s_replaceTag_head->item_ptr= item_tag;
        }
        replace_cur_tag = g_parse_data.s_replaceTag_head->next;
        MMIDM_releaseReplaceContent(g_parse_data.s_replaceTag_head);
        SCI_FREE(g_parse_data.s_replaceTag_head);
        g_parse_data.s_replaceTag_head= replace_cur_tag;
    }

}


/*****************************************************************************/
//  Description : 释放解析过程中申请的数据
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void FreeStoreDmData(void)
{
    if(PNULL != g_store_data.s_status_store_data_ptr)
    {
        SCI_FREE(g_store_data.s_status_store_data_ptr);
        g_store_data.s_status_store_data_ptr = PNULL;
    }
    if(PNULL != g_store_data.s_get_store_data_ptr)
    {
        SCI_FREE(g_store_data.s_get_store_data_ptr);
        g_store_data.s_get_store_data_ptr = PNULL;
    }
    if(PNULL != g_store_data.s_replace_store_data_ptr)
    {
        SCI_FREE(g_store_data.s_replace_store_data_ptr);
        g_store_data.s_replace_store_data_ptr = PNULL;
    }  
    if(PNULL != g_store_data.s_alert_store_data_ptr)
    {
        SCI_FREE(g_store_data.s_alert_store_data_ptr);
        g_store_data.s_alert_store_data_ptr = PNULL;
    } 
    if(PNULL != g_store_data.s_exec_store_data_ptr)
    {
        SCI_FREE(g_store_data.s_exec_store_data_ptr);
        g_store_data.s_exec_store_data_ptr = PNULL;
    }      
}
/*****************************************************************************/
//  Description : 解析SESSIONID的后的处理函数
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 比较接受包的sessionid是否是我们发送请求包的sessionid， 不是的话， 直接结束流程
/*****************************************************************************/

LOCAL void MMIDM_CheckSessionidDataFunc(char* userdata)
{
    char str[10]={0};

    if(PNULL == userdata)
    {
        return;
    }
    MMIDM_IU32toa(g_state_data.s_g_SessionID, str, 32);
    if(strcmp(str, (char*)(userdata)))
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_ParseXMLData, the wrong sessionid %s", userdata);
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
        return;
    } 
}


/*****************************************************************************/
//  Description : 存储status tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL void MMIDM_storeStatusData(char* userdata)
{
    uint16 len = 0;
    uint16 min_len  = 0;
    if(PNULL == userdata /*|| PNULL== g_store_data.s_cur_tagname*/)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeStatusData userdata %d g_store_data.s_cur_tagname %d", userdata, g_store_data.s_cur_tagname);
        return;
    }
    if(PNULL == g_store_data.s_status_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeStatusData g_store_data.s_status_store_data_ptr %d", g_store_data.s_status_store_data_ptr);
        return;
    }

    len = strlen(userdata);
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_CMDID)))
    {
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        strncpy(g_store_data.s_status_store_data_ptr->cmd, userdata, min_len);
    }
    else if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_CMDREFID)))
    {
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        strncpy(g_store_data.s_status_store_data_ptr->cmdref, userdata, min_len);
    }   
    else if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_TARGETREFID)))
    {
        min_len = MIN(len, MAX_TARGET_BUF_SIZE);
        strncpy(g_store_data.s_status_store_data_ptr->targetref, userdata, min_len);
    }     
    else if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_DATAID)))
    {
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        strncpy(g_store_data.s_status_store_data_ptr->data, userdata, min_len);
    }
    else if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_MSGREFID)))
    {
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        strncpy(g_store_data.s_status_store_data_ptr->msgref, userdata, min_len);
    }
    else if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_SOURCEREFID)))
    {
        min_len = MIN(len, MAX_TARGET_BUF_SIZE);
        strncpy(g_store_data.s_status_store_data_ptr->sourceref, userdata, min_len);
    }
    else if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_NEXTNONCEID)))
    {
        min_len = MIN(len, MAX_NONCE_LEN);
        strncpy(g_state_data.s_g_nonce, userdata, min_len);
    }    
}
/*****************************************************************************/
//  Description : 根据参数，组建一个tag
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ConstrucTag(DMXML_TAG_T* tag_ptr, 
                                MMI_DM_TAGID_E tag_id,
                                char* tagArr,
                                char* tagContent,
                                BOOLEAN hasChildTag)
{
    uint16 len =0;
    if(PNULL == tag_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_ConstrucTag PNULL == tag_ptr");
        return FALSE;
    }

    tag_ptr->tagId = tag_id;
    tag_ptr->hasChildTag = hasChildTag;
    if(PNULL != tagContent)
    {
    
        tag_ptr->tagContent =SCI_ALLOC_APPZ(strlen(tagContent)+1);
        if(PNULL == tag_ptr->tagContent)
        {
            SCI_TRACE_LOW("[mmidm], MMIDM_ConstrucTag alloc tag content error tagContent %s", tagContent);
            return FALSE;
        }
        SCI_STRCPY(tag_ptr->tagContent, tagContent);
    }
    if(PNULL != tagArr)
    {
    
        len = MIN(MAX_TAG_ARR_LEN, strlen(tagArr));/*lint !e666*/
        strncpy(tag_ptr->tagArr, tagArr, len);
    }
    return TRUE;
}


/*****************************************************************************/
//  Description : 解析status的staus的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN buildStatusStatusTag(void )
{
    DMXML_TAG_STATUS_T* status_tag = PNULL;
    BOOLEAN result = TRUE;
        char str[32]={0};
        
    if(PNULL == g_store_data.s_status_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] buildStatusTag PNULL == g_store_data.s_status_store_data_ptr");
        return FALSE;
    }

    status_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_STATUS_T));
    if(PNULL== status_tag)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_DealWithStatusData PNULL== status_tag");
        return FALSE;
    }
    
    MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
    g_state_data.s_g_CmdID++;
    if(!MMIDM_ConstrucTag(&(status_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
    {
        result = FALSE;
    }
    MMIDM_IU32toa(g_state_data.s_g_MsgID, str, 32);
    if(!MMIDM_ConstrucTag(&(status_tag->msgRef),TAG_MSGREFID,PNULL, str, FALSE))
    {
        result = FALSE;
    }
    if(!MMIDM_ConstrucTag(&(status_tag->cmdRef),TAG_CMDREFID,PNULL, "0", FALSE))
    {
        result = FALSE;
    }
    if(!MMIDM_ConstrucTag(&(status_tag->targetRef[0]),TAG_TARGETREFID,PNULL, g_store_data.s_status_store_data_ptr->sourceref, FALSE))
    {
        result = FALSE;
    }
	if(MAX_TARGET_BUF_SIZE>= strlen(MMIDM_getResUri()))
	{
        SCI_STRCPY(g_store_data.s_status_store_data_ptr->targetref,MMIDM_getResUri());/*lint !e666*/
	}
	else
	{
       SCI_TRACE_LOW("[MMIDM] MMIDM_DealWithStatusData targetref TOO SMALL"); 
        result = FALSE;				
	}
    if(!MMIDM_ConstrucTag(&(status_tag->sourceRef),TAG_SOURCEREFID,PNULL, g_store_data.s_status_store_data_ptr->targetref, FALSE))
    {
        result = FALSE;
    }
    if(!MMIDM_ConstrucTag(&(status_tag->cmd),TAG_CMDID,PNULL, "SyncHdr", FALSE))
    {
        result = FALSE;
    }    

    if(!MMIDM_ConstrucTag(&(status_tag->chal.meta.format),TAG_FORMATID,ARR_METINFO, "b64", FALSE))
    {
        result = FALSE;
    } 
    if(!MMIDM_ConstrucTag(&(status_tag->chal.meta.type),TAG_TYPEID,ARR_METINFO, "syncml:auth-md5", FALSE))
    {
        result = FALSE;
    } 
    if(!MMIDM_ConstrucTag(&(status_tag->chal.meta.nextnonce),TAG_NEXTNONCEID,ARR_METINFO, COM_NONCE, FALSE))
    {
        result = FALSE;
    } 

	if(g_state_data.s_g_step == STEP_CREDED)
	{
        if(!MMIDM_ConstrucTag(&(status_tag->data),TAG_DATAID,PNULL, CRED_SUSS_DATA, FALSE))
        {
            result = FALSE;
        } 
	}
	else
	{
        if(!MMIDM_ConstrucTag(&(status_tag->data),TAG_DATAID,PNULL, CRED_FAIL1_DATA, FALSE))
        {
            result = FALSE;
        } 
	}

    if(g_parse_data.s_statusTag_head == PNULL)
    {
        g_parse_data.s_statusTag_head = g_parse_data.s_statusTag_tail = status_tag;
    }
    else
    {
        g_parse_data.s_statusTag_tail->next = status_tag;
        g_parse_data.s_statusTag_tail = g_parse_data.s_statusTag_tail->next;
    }
    return result;
}
/*****************************************************************************/
//  Description : 解析status tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  MMIDM_parseStatusData(void)
{
    if(PNULL == g_store_data.s_status_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_parseStatusData PNULL == g_store_data.s_status_store_data_ptr");
        return FALSE;
    }

    if(strlen(g_store_data.s_status_store_data_ptr->cmd))
    {
        if(!strcmp(g_store_data.s_status_store_data_ptr->cmd, "SyncHdr"))
        {
            if(!strcmp(g_store_data.s_status_store_data_ptr->data, CRED_SUSS_DATA))
            {
                g_state_data.s_g_step = STEP_CREDED;
            }
            else if(!strcmp(g_store_data.s_status_store_data_ptr->data, CRED_FAIL1_DATA) || !strcmp(g_store_data.s_status_store_data_ptr->data, CRED_FAIL2_DATA))
            {
                if(g_state_data.s_g_step == STEP_GETNONCE)
                {
                    g_state_data.s_g_step = STEP_GETNONCE_AGAIN;
                }
            else
            {
                MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
                SCI_TRACE_LOW("[MMIDM] MMIDM_parseStatusData A SESSION END");
                return FALSE;
            }
            }
           if(g_state_data.s_g_step == STEP_CREDED || g_state_data.s_g_step == STEP_GETNONCE || g_state_data.s_g_step == STEP_GETNONCE_AGAIN)
            {
                if(!buildStatusStatusTag())
                {
                    MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
                    return FALSE;
                }
            }
        }
    }
    else
    {
       SCI_TRACE_LOW("[MMIDM] MMIDM_DealWithStatusData");
    }
    return TRUE;    
}


/*****************************************************************************/
//  Description : 存储status tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL void MMIDM_storeGetData(char* userdata)
{
    uint16 min_len = 0;

    if(PNULL == userdata)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeGetData userdata %d g_store_data.s_cur_tagname %d", userdata, g_store_data.s_cur_tagname);
        return;
    }
    if(PNULL == g_store_data.s_get_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeGetData g_store_data.s_get_store_data_ptr %d", g_store_data.s_get_store_data_ptr);
        return;
    }

    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_CMDIDID)))
    {
        min_len = MIN(strlen(userdata), MAX_CMDID_BUF_SIZE);/*lint !e666*/
        strncpy(g_store_data.s_get_store_data_ptr->cmdid, userdata, min_len);
    } 
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_LOCURIID)))
    {
        if(DM_MAX_TARGETREF_NUM>g_store_data.s_get_store_data_ptr->uri_index)
        {
        
            min_len = MIN(strlen(userdata), MAX_TARGET_BUF_SIZE);/*lint !e666*/
            strncpy(g_store_data.s_get_store_data_ptr->target_uri[g_store_data.s_get_store_data_ptr->uri_index], userdata, min_len);   
            g_store_data.s_get_store_data_ptr->uri_index++;
        }
    }

}


/*****************************************************************************/
//  Description : 解析status组建staus的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN buildGetStatusTag(void )
{
    DMXML_TAG_STATUS_T* status_tag = PNULL;
    char str[32]={0};
    BOOLEAN ret = TRUE;
    VDM_Tree_ReadExternalCB read_func = PNULL;
    BOOLEAN has_getfunc = TRUE;
    int i= 0 ;

    if(PNULL == g_store_data.s_get_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] buildGetStatusTag PNULL == g_store_data.s_get_store_data_ptr");
        return FALSE;
    }

    status_tag = SCI_ALLOC_APPZ (sizeof(DMXML_TAG_STATUS_T));
    if(PNULL== status_tag)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_DealWithGetData PNULL== status_tag");
        return FALSE;
    }

    
    MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
    g_state_data.s_g_CmdID++;
    if(!MMIDM_ConstrucTag(&(status_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
    {
        ret = FALSE;
    }//cmdid

    MMIDM_IU32toa(g_state_data.s_g_MsgID, str, 32);
    if(!MMIDM_ConstrucTag(&(status_tag->msgRef),TAG_MSGREFID,PNULL, str, FALSE))
    {
        ret = FALSE;
    }//msgref

    if(!MMIDM_ConstrucTag(&(status_tag->cmdRef),TAG_CMDREFID,PNULL, g_store_data.s_get_store_data_ptr->cmdid, FALSE))
    {
        ret = FALSE;
    }//cmdref 
    
    for(i=0; i<g_store_data.s_get_store_data_ptr->uri_index; i++)
    {
        if(!MMIDM_ConstrucTag(&(status_tag->targetRef[i]),TAG_TARGETREFID,PNULL, g_store_data.s_get_store_data_ptr->target_uri[i], FALSE))
        {
            ret = FALSE;
        }//targetref  
        read_func = DM_GET_TREE_READFUNC(g_store_data.s_get_store_data_ptr->target_uri[i]);
        if(PNULL == read_func)
        {
            has_getfunc = FALSE;
        }
    }

    if(!MMIDM_ConstrucTag(&(status_tag->cmd),TAG_CMDID,PNULL, MMIDM_GetTagStr(TAG_GETID), FALSE))
    {
        ret = FALSE;
    }//cmd

    if(has_getfunc)
    {
        strcpy(str, STATUS_OK);
    }
    else
    {
        strcpy(str, STATUS_ERR);
    }

    if(!MMIDM_ConstrucTag(&(status_tag->data),TAG_DATAID,PNULL, str, FALSE))
    {
        ret = FALSE;
    }//data
    if(g_parse_data.s_statusTag_head == PNULL)
    {
        g_parse_data.s_statusTag_head = g_parse_data.s_statusTag_tail = status_tag;
    }
    else
    {
        g_parse_data.s_statusTag_tail->next = status_tag;
        g_parse_data.s_statusTag_tail = g_parse_data.s_statusTag_tail->next;

    }  
    return ret;
}


/*****************************************************************************/
//  Description : 解析Get组建result的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN buildGetResultTag(void )
{
    DMXML_TAG_RESULT_T* result_tag = PNULL;
    DMXML_TAG_ITEM_T* item_ptr = PNULL;
    VDM_Tree_ReadExternalCB read_func = PNULL;
    char str[32]={0};
    int i = 0;
    uint16        len = 0;
    BOOLEAN        ret = TRUE;
    char* content_ptr=PNULL;
    content_ptr = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE + 1) ;
    if(PNULL== content_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] buildGetResultTag PNULL== content"); 
        return FALSE;
    }

    for(i =0; i<g_store_data.s_get_store_data_ptr->uri_index; i++)
    {
    
	    result_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_RESULT_T));
	    if(PNULL== result_tag)
	    {
	        SCI_TRACE_LOW("[MMIDM] buildGetResultTag PNULL== result_tag");	
            SCI_FREE(content_ptr);
            return FALSE;
	    }

	    item_ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ITEM_T));
	    if(PNULL== item_ptr)
	    {
	        SCI_TRACE_LOW("[MMIDM] buildGetResultTag PNULL== item_ptr");
            SCI_FREE(result_tag);
            SCI_FREE(content_ptr);
            return FALSE;
	    }
        read_func = DM_GET_TREE_READFUNC(g_store_data.s_get_store_data_ptr->target_uri[i]);
	    if(PNULL!=read_func)
	    {
	       SCI_MEMSET(content_ptr, 0X0, MAX_TAG_BUF_SIZE + 1);
	        read_func(PNULL, 0, content_ptr, MAX_TAG_BUF_SIZE, &len);/*lint !e64*/
	    }
        
	    MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
	    g_state_data.s_g_CmdID++;
        if(!MMIDM_ConstrucTag(&(result_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
        {
            ret = FALSE;
        }//CMDID  
	    MMIDM_IU32toa(g_state_data.s_g_MsgID, str, 32);
        if(!MMIDM_ConstrucTag(&(result_tag->msgRef),TAG_MSGREFID,PNULL, str, FALSE))
        {
            ret = FALSE;
        }//msgref
        if(!MMIDM_ConstrucTag(&(result_tag->cmdRef),TAG_CMDREFID,PNULL, g_store_data.s_get_store_data_ptr->cmdid, FALSE))
        {
            ret = FALSE;
        }//cmdref
        if(!MMIDM_ConstrucTag(&(item_ptr->source.locuri),TAG_LOCURIID,PNULL, g_store_data.s_get_store_data_ptr->target_uri[i], FALSE))
        {
            ret = FALSE;
        }//source locuri

        if(!MMIDM_ConstrucTag(&(item_ptr->meta.format),TAG_FORMATID,ARR_METINFO, FORMAT_CHAR, FALSE))
        {
            ret = FALSE;
        }//meta  format               

        if(!MMIDM_ConstrucTag(&(item_ptr->data),TAG_DATAID,PNULL, content_ptr, FALSE))
        {
            ret = FALSE;
        }//meta  format   
	    result_tag->item_ptr = item_ptr;

	
	    if(g_parse_data.s_resultTag_head == PNULL)
	    {
	        g_parse_data.s_resultTag_head = g_parse_data.s_resultTag_tail = result_tag;
	    }
	    else
	    {
	        g_parse_data.s_resultTag_tail->next = result_tag;
	        g_parse_data.s_resultTag_tail = g_parse_data.s_resultTag_tail->next;

	    }
    }

    SCI_FREE(content_ptr);
    return ret;
}
/*****************************************************************************/
//  Description : 解析GET tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN MMIDM_parseGetData(void)
{
    
    BOOLEAN ret = TRUE;

    if(PNULL == g_store_data.s_get_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_parseGetData PNULL == g_store_data.s_get_store_data_ptr");
        return FALSE;
    }
    if(strlen(g_store_data.s_get_store_data_ptr->target_uri))
    {

       if(!buildGetStatusTag())
       {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return FALSE;

       }
       if(!buildGetResultTag())
       {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return FALSE;
       }
        
    }
    
    return ret;

}

/*****************************************************************************/
//  Description : 存储replace tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL void MMIDM_storeReplaceData(char* userdata)
{
    uint16 len = 0;
    uint16 min_len  = 0;
    if(PNULL == userdata /*|| PNULL== g_store_data.s_cur_tagname*/)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeGetData userdata %d g_store_data.s_cur_tagname %d", userdata, g_store_data.s_cur_tagname);
        return;
    }
    if(PNULL == g_store_data.s_replace_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeGetData g_store_data.s_replace_store_data_ptr %d", g_store_data.s_replace_store_data_ptr);
        return;
    }
    
    len = strlen(userdata);
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_CMDIDID)))
    {
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        strncpy(g_store_data.s_replace_store_data_ptr->cmdid, userdata, min_len);
    } 
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_LOCURIID)))
    {
        if(DM_MAX_TARGETREF_NUM>g_store_data.s_replace_store_data_ptr->uri_index)
        {
            min_len = MIN(strlen(userdata), MAX_TARGET_BUF_SIZE);/*lint !e666*/
            strncpy(g_store_data.s_replace_store_data_ptr->target_uri[g_store_data.s_replace_store_data_ptr->uri_index], userdata, min_len);   
            g_store_data.s_replace_store_data_ptr->uri_index++;
        }
    }    
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_DATAID)))
    {
        if(DM_MAX_TARGETREF_NUM>g_store_data.s_replace_store_data_ptr->data_index)
        {
            min_len = strlen(userdata);
            min_len = MIN(min_len, MAX_DATA_BUF_SIZE);
            strncpy(g_store_data.s_replace_store_data_ptr->data[g_store_data.s_replace_store_data_ptr->data_index], userdata, min_len);   
            g_store_data.s_replace_store_data_ptr->data_index++;
        }
    }  
}

/*****************************************************************************/
//  Description : 解析replace组建staus的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN buildReplaceStatusTag(void )
{
    DMXML_TAG_STATUS_T* status_tag = PNULL;
    VDM_Tree_WriteExternalCB write_func = PNULL;
     char  str[32] = {0};
     BOOLEAN ret =TRUE;
    int i= 0;

    for(i=0; i<g_store_data.s_replace_store_data_ptr->uri_index; i++)
    {
        status_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_STATUS_T));
        if(PNULL== status_tag)
        {
            SCI_TRACE_LOW("[MMIDM] buildReplaceStatusTag PNULL== status_tag");
            return FALSE;
        }
        write_func = DM_GET_TREE_WRITEFUNC(g_store_data.s_replace_store_data_ptr->target_uri[i]);

        MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
        g_state_data.s_g_CmdID++;
        if(!MMIDM_ConstrucTag(&(status_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
        {
            ret = FALSE;
        }//CMDID
        MMIDM_IU32toa(g_state_data.s_g_MsgID, str, 32);
        if(!MMIDM_ConstrucTag(&(status_tag->msgRef),TAG_MSGREFID,PNULL, str, FALSE))
        {
            ret = FALSE;
        }//MSGREF
        if(!MMIDM_ConstrucTag(&(status_tag->cmdRef),TAG_CMDREFID,PNULL, g_store_data.s_replace_store_data_ptr->cmdid, FALSE))
        {
            ret = FALSE;
        }//CMDREF
        if(!MMIDM_ConstrucTag(&(status_tag->targetRef[0]),TAG_TARGETREFID,PNULL, g_store_data.s_replace_store_data_ptr->target_uri[i], FALSE))
        {
            ret = FALSE;
        }//TARGETREF
        if(!MMIDM_ConstrucTag(&(status_tag->cmd),TAG_CMDID,PNULL, MMIDM_GetTagStr((TAG_REPLACEID)), FALSE))
        {
            ret = FALSE;
        }//CMD
        if(PNULL!=write_func)
        {
            uint16 len = SCI_STRLEN(g_store_data.s_replace_store_data_ptr->data[i]);
            if(!write_func(PNULL, 0, g_store_data.s_replace_store_data_ptr->data[i], len, MAX_DATA_BUF_SIZE))
            {
                SCI_STRCPY(str, STATUS_OK);/*lint !e779*/
            }
            else
            {
                SCI_STRCPY(str, STATUS_ERR);/*lint !e779*/
            }
        }
        else
        {
            SCI_STRCPY(str, STATUS_ERR);/*lint !e779*/
        }
        if(!MMIDM_ConstrucTag(&(status_tag->data),TAG_DATAID,PNULL, str, FALSE))
        {
            ret = FALSE;
        }//DATA
        if(g_parse_data.s_statusTag_head == PNULL)
        {
            g_parse_data.s_statusTag_head = g_parse_data.s_statusTag_tail = status_tag;
        }
        else
        {
            g_parse_data.s_statusTag_tail->next = status_tag;
            g_parse_data.s_statusTag_tail = g_parse_data.s_statusTag_tail->next;
        }
      
    }
    return ret;
}

/*****************************************************************************/
//  Description : 解析replace tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN MMIDM_parseReplaceData(void)
{
    BOOLEAN ret = TRUE;

    if(PNULL == g_store_data.s_replace_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_parseReplaceData PNULL == g_store_data.s_replace_store_data_ptr");
        return FALSE;
    }
   if(!buildReplaceStatusTag())
   {
       MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
       return FALSE;

   }
   return ret;
}

/*****************************************************************************/
//  Description : 存储alert tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL void MMIDM_storeAlertData(char* userdata)
{
    uint16 len = 0;
    uint16 min_len  = 0;
    char* mintimer_ptr = PNULL;
    char* maxtimer_ptr = PNULL;
    if(PNULL == userdata /*|| PNULL== g_store_data.s_cur_tagname*/)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeAlertData userdata %d g_store_data.s_cur_tagname %d", userdata, g_store_data.s_cur_tagname);
        return;
    }
    if(PNULL == g_store_data.s_alert_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeAlertData g_store_data.s_alert_store_data_ptr %d", g_store_data.s_alert_store_data_ptr);
        return;
    }
    
    len = strlen(userdata);
    SCI_TRACE_LOW("[mmidm] alerttip %s", userdata);
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_CMDIDID)))
    {
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        SCI_MEMSET(g_store_data.s_alert_store_data_ptr->cmdid, 0, (MAX_CMDID_BUF_SIZE+1));
        strncpy(g_store_data.s_alert_store_data_ptr->cmdid, userdata, min_len);
    } 
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_DATAID)))
    {
       if(!strcmp(userdata, EXIT_DATA)) 
       {
           return;
       }
       if(!strcmp(userdata, DIALOG_DATA))
       {
           g_store_data.s_alert_store_data_ptr->is_dialog =TRUE;
       }
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        mintimer_ptr = strstr(userdata, MINDT);
        maxtimer_ptr =strstr(userdata, MAXDT);
        if(PNULL!= maxtimer_ptr)
        {
            min_len = MIN(strlen(maxtimer_ptr)- strlen(MAXDT), MAX_CMDID_BUF_SIZE);/*lint !e666*/
            SCI_MEMSET(g_store_data.s_alert_store_data_ptr->maxtime, 0, (DM_COMMON_BUFSIZE+1));
            strncpy(g_store_data.s_alert_store_data_ptr->maxtime, maxtimer_ptr+strlen(MAXDT), min_len);
        }
        if(PNULL!=mintimer_ptr)
        {
            min_len = MIN(strlen(mintimer_ptr)- strlen(MINDT), MAX_CMDID_BUF_SIZE);/*lint !e666*/
            SCI_MEMSET(g_store_data.s_alert_store_data_ptr->minitime, 0, (DM_COMMON_BUFSIZE+1));
            strncpy(g_store_data.s_alert_store_data_ptr->minitime, mintimer_ptr+strlen(MINDT), min_len);
        }
        if((PNULL == maxtimer_ptr) && (PNULL == mintimer_ptr) && g_store_data.s_alert_store_data_ptr->is_dialog)
        {
          SCI_MEMSET(g_store_data.s_alert_store_data_ptr->data, 0, (DM_COMMON_BUFSIZE+1));
          strncpy(g_store_data.s_alert_store_data_ptr->data, userdata, min_len); 
        }
    }
}

/*****************************************************************************/
//  Description : 解析ALERT组建staus的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN buildAlertStatusTag(void )
{
    DMXML_TAG_STATUS_T* status_tag = PNULL;
    char str[64]={0};
    BOOLEAN ret = TRUE;
    if(PNULL == g_store_data.s_alert_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] buildAlertStatusTag PNULL == g_store_data.s_alert_store_data_ptr");
        return FALSE;
    }

    status_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_STATUS_T));
    if(PNULL== status_tag)
    {
        SCI_TRACE_LOW("[MMIDM] buildAlertStatusTag PNULL== status_tag");
        return FALSE;
    }

    MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
    g_state_data.s_g_CmdID++;
    if(!MMIDM_ConstrucTag(&(status_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
    {
        ret = FALSE;
    }//CMDID
    MMIDM_IU32toa(g_state_data.s_g_MsgID, str, 32);
    if(!MMIDM_ConstrucTag(&(status_tag->msgRef),TAG_MSGREFID,PNULL, str, FALSE))
    {
        ret = FALSE;
    }//MSGref
    if(!MMIDM_ConstrucTag(&(status_tag->cmdRef),TAG_CMDREFID,PNULL, g_store_data.s_alert_store_data_ptr->cmdid, FALSE))
    {
        ret = FALSE;
    }//cmdref

    if(!MMIDM_ConstrucTag(&(status_tag->cmd),TAG_CMDID,PNULL, MMIDM_GetTagStr(TAG_ALERTID), FALSE))
    {
        ret = FALSE;
    }//cmdref  
    if(g_parse_data.s_statusTag_head == PNULL)
    {
        g_parse_data.s_statusTag_head = g_parse_data.s_statusTag_tail = status_tag;
    }
    else
    {
        g_parse_data.s_statusTag_tail->next = status_tag;
        g_parse_data.s_statusTag_tail = g_parse_data.s_statusTag_tail->next;
        g_parse_data.s_statusTag_cur = g_parse_data.s_statusTag_tail;

    }   
    return ret;
}

/*****************************************************************************/
//  Description : 解析alert tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN MMIDM_parseAlertData(void)
{
    BOOLEAN ret = TRUE;
    BOOLEAN     retvalue = FALSE;
    if(PNULL == g_store_data.s_alert_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_parseAlertData PNULL == g_store_data.s_alert_store_data_ptr");
        return FALSE;
    }
    if(g_store_data.s_alert_store_data_ptr->is_dialog)
    {
         MMIDM_OpenDmSessionAlertWin(MMIDM_ALERT_PL_CONFIRM,g_store_data.s_alert_store_data_ptr->data, strlen(g_store_data.s_alert_store_data_ptr->data), MMIDM_atoIU32(g_store_data.s_alert_store_data_ptr->maxtime, 10, &retvalue));/*lint !e64*/
         
        if(!buildAlertStatusTag())
        {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return FALSE;

        }
        SCI_TRACE_LOW("[MMIDM] MMIDM_parseAlertData g_state_data.s_g_needreplay =%d",g_state_data.s_g_needreplay);
        g_state_data.s_g_needreplay = TRUE;
    }
   return ret;
}

/*****************************************************************************/
//  Description : 解析EXEC组建staus的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN buildExecStatusTag(BOOLEAN isexec)
{
    DMXML_TAG_STATUS_T* status_tag = PNULL;
    char str[64]={0};
    BOOLEAN ret = TRUE;
    if(PNULL == g_store_data.s_exec_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] buildExecStatusTag PNULL == g_store_data.s_exec_store_data_ptr");
        return FALSE;
    }

    status_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_STATUS_T));
    if(PNULL== status_tag)
    {
        SCI_TRACE_LOW("[MMIDM] buildExecStatusTag PNULL== status_tag");
        return FALSE;
    }

    MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
    g_state_data.s_g_CmdID++;
    if(!MMIDM_ConstrucTag(&(status_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
    {
        ret = FALSE;
    }//CMDID
    MMIDM_IU32toa(g_state_data.s_g_MsgID, str, 32);
    if(!MMIDM_ConstrucTag(&(status_tag->msgRef),TAG_MSGREFID,PNULL, str, FALSE))
    {
        ret = FALSE;
    }//MSGref
    SCI_TRACE_LOW("[mmidm] buildExecStatusTag g_store_data.s_exec_store_data_ptr->cmdid %s", g_store_data.s_exec_store_data_ptr->cmdid);
    if(!MMIDM_ConstrucTag(&(status_tag->cmdRef),TAG_CMDREFID,PNULL, g_store_data.s_exec_store_data_ptr->cmdid, FALSE))
    {
        ret = FALSE;
    }//cmdref

    if(!MMIDM_ConstrucTag(&(status_tag->targetRef[0]),TAG_TARGETREFID,PNULL, g_store_data.s_exec_store_data_ptr->target_uri, FALSE))
    {
        ret = FALSE;
    }//TARGETREF
            
    if(!MMIDM_ConstrucTag(&(status_tag->cmd),TAG_CMDID,PNULL, MMIDM_GetTagStr(TAG_EXECID), FALSE))
    {
        ret = FALSE;
    }//cmd 
    if(isexec)
    {
        SCI_STRCPY(str, STATUS_OK);/*lint !e779*/
    }
    else
    {
        SCI_STRCPY(str, STATUS_ERR);/*lint !e779*/
    }
    
    if(!MMIDM_ConstrucTag(&(status_tag->data),TAG_DATAID,PNULL,str, FALSE))
    {
        ret = FALSE;
    }//DATA

    if(g_parse_data.s_statusTag_head == PNULL)
    {
        g_parse_data.s_statusTag_head = g_parse_data.s_statusTag_tail = status_tag;
    }
    else
    {
        g_parse_data.s_statusTag_tail->next = status_tag;
        g_parse_data.s_statusTag_tail = g_parse_data.s_statusTag_tail->next;

    }   
    return ret;
}

/*****************************************************************************/
//  Description : 存储exec tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_storeExecData(char* userdata)
{
    uint16 len = 0;
    uint16 min_len  = 0;

    if(PNULL == userdata /*|| PNULL== g_store_data.s_cur_tagname*/)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeExecData userdata %d g_store_data.s_cur_tagname %d", userdata, g_store_data.s_cur_tagname);
        return;
    }
    if(PNULL == g_store_data.s_exec_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeExecData g_store_data.s_exec_store_data_ptr %d", g_store_data.s_exec_store_data_ptr);
        return;
    }
    
    len = strlen(userdata);
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_CMDIDID)))
    {
        min_len = MIN(len, MAX_CMDID_BUF_SIZE);
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeExecData userdata %s", userdata);
        strncpy(g_store_data.s_exec_store_data_ptr->cmdid, userdata, min_len);
        SCI_TRACE_LOW("[MMIDM] MMIDM_storeExecData g_store_data.s_exec_store_data_ptr->cmdid %s", g_store_data.s_exec_store_data_ptr->cmdid);
    } 
    if(!strcmp(g_store_data.s_cur_tagname ,MMIDM_GetTagStr(TAG_LOCURIID)))
    {
        min_len = MIN(len, MAX_TARGET_BUF_SIZE);
        strncpy(g_store_data.s_exec_store_data_ptr->target_uri, userdata, min_len);
    } 
}


/*****************************************************************************/
//  Description : 解析exec tag的数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_parseExecData(void)
{
    BOOLEAN ret = TRUE;
    BOOLEAN is_exec = FALSE;
    BOOLEAN     retvalue = FALSE;
    if(PNULL == g_store_data.s_exec_store_data_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_parseExecData PNULL == g_store_data.s_exec_store_data_ptr");
        return FALSE;
    }

    if(!strcmp(g_store_data.s_exec_store_data_ptr->target_uri, DM_FAC_RESET))
    {
        g_state_data.s_g_callClearFunc = TRUE;
        is_exec = TRUE;
    } 
    else if(!strcmp(g_store_data.s_exec_store_data_ptr->target_uri, DM_LOCK))
    {
        g_state_data.s_g_callLockFunc = TRUE;
        is_exec = TRUE;
    }  
    else if(!strcmp(g_store_data.s_exec_store_data_ptr->target_uri, DM_UNLOCK))
    {
        g_state_data.s_g_callUnlockFunc = TRUE;
        is_exec = TRUE;
    } 

    if(!buildExecStatusTag(is_exec))
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
        return FALSE;

    }


   return ret;
}

/*****************************************************************************/
//  Description : get tag name and tag arr
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ParseNameAndArr(char *localname_ptr,
                        char* tag_name,//out
                        char* tag_arr//out
                        )
{
    char* sep_char = "$";
    char* find_str = PNULL;  
    int len =0;

    if(tag_name == PNULL || tag_arr == PNULL)
    {
        return FALSE;
    }
    find_str = strstr(localname_ptr, sep_char);

    if(PNULL == find_str)
    {
        strcpy(tag_name, localname_ptr);
    }
    else
    {
        if(0 != *(find_str+1))
        {
            strcpy(tag_name, find_str+1);/*lint !e774*/
        }
        len = strlen(localname_ptr) -  strlen(find_str);

        strncpy(tag_arr, localname_ptr, len);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void startTagFunc(  const char *tag_name_ptr,
	                    const char *tag_arr_ptr)
{
    
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_SESSIONID)))
    {
        g_store_data.curDealTagFunc = MMIDM_CheckSessionidDataFunc;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_RESURIID)))
    {
        g_store_data.curDealTagFunc = MMIDM_setResUri;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_STATUSID)))
    {
        FreeStoreDmData();//protect from memory leek
        g_store_data.s_status_store_data_ptr =  SCI_ALLOC_APPZ(sizeof(DMXML_TAG_STATUS_DATA_T));
        if(PNULL == g_store_data.s_status_store_data_ptr)
        {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return;
        }
        g_store_data.curDealTagFunc = MMIDM_storeStatusData;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_GETID)))
    {
         FreeStoreDmData();//protect from memory leek
        g_store_data.s_get_store_data_ptr =  SCI_ALLOC_APPZ(sizeof(DMXML_TAG_GET_DATA_T));
        if(PNULL == g_store_data.s_get_store_data_ptr)
        {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return;
        }
        g_store_data.curDealTagFunc = MMIDM_storeGetData;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_REPLACEID)))
    {
         FreeStoreDmData();//protect from memory leek
        g_store_data.s_replace_store_data_ptr =  SCI_ALLOC_APPZ(sizeof(DMXML_TAG_REPLACE_DATA_T));
        if(PNULL == g_store_data.s_replace_store_data_ptr)
        {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return;
        }
        g_store_data.curDealTagFunc = MMIDM_storeReplaceData;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_ALERTID)))
    {
        FreeStoreDmData();//protect from memory leek
        g_store_data.s_alert_store_data_ptr =  SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ALERT_DATA_T));
        if(PNULL == g_store_data.s_alert_store_data_ptr)
        {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return;
        }
        g_store_data.curDealTagFunc = MMIDM_storeAlertData;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_EXECID)))
    {
        FreeStoreDmData();//protect from memory leek
        g_store_data.s_exec_store_data_ptr =  SCI_ALLOC_APPZ(sizeof(DMXML_TAG_EXEC_DATA_T));
        if(PNULL == g_store_data.s_exec_store_data_ptr)
        {
           MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
           return;
        }
        g_store_data.curDealTagFunc = MMIDM_storeExecData;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void endTagFunc(  const char *tag_name_ptr,
	                    const char *tag_arr_ptr)
{
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_SESSIONID)))
    {
        g_store_data.curDealTagFunc = PNULL;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_RESURIID)))
    {
        g_store_data.curDealTagFunc = PNULL;
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_STATUSID)))
    {
        g_store_data.curDealTagFunc = PNULL;
        MMIDM_parseStatusData();
        if(PNULL!=g_store_data.s_status_store_data_ptr)
        {
            SCI_FREE(g_store_data.s_status_store_data_ptr);
        }
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_GETID)))
    {
        g_store_data.curDealTagFunc = PNULL;
        MMIDM_parseGetData();
        if(PNULL!=g_store_data.s_get_store_data_ptr)
        {
            SCI_FREE(g_store_data.s_get_store_data_ptr);
        }        
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_REPLACEID)))
    {
        g_store_data.curDealTagFunc = PNULL;
        MMIDM_parseReplaceData();
        if(PNULL!=g_store_data.s_replace_store_data_ptr)
        {
            SCI_FREE(g_store_data.s_replace_store_data_ptr);
        }        
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_ALERTID)))
    {
        g_store_data.curDealTagFunc = PNULL;
        MMIDM_parseAlertData();
        if(PNULL!=g_store_data.s_alert_store_data_ptr)
        {
            SCI_FREE(g_store_data.s_alert_store_data_ptr);
        }        
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_EXECID)))
    {
        g_store_data.curDealTagFunc = PNULL;
        MMIDM_parseExecData();
        if(PNULL!=g_store_data.s_exec_store_data_ptr)
        {
            SCI_FREE(g_store_data.s_exec_store_data_ptr);
        }        
    }
    if(!strcmp(tag_name_ptr,MMIDM_GetTagStr(TAG_SYNCBODYID)))
    {
            EndDmParse();        
    }
}
/*****************************************************************************/
//  Description : a tag START callback
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void StartElementSax(  /*lint -esym(765,StartElementSax2Test) */
    void *user_data_ptr,
	const char *localname_ptr,
	const char **attributes_pptr)
{
    char* tag_name_ptr = PNULL;
    char* tag_arr_ptr = PNULL;
    int len = 0;
    char*  name_arr_ptr = PNULL; 
    if(PNULL == localname_ptr)
    {
        return;
    }
    len = strlen(localname_ptr);
    tag_name_ptr = SCI_ALLOC_APPZ(len+1);
    tag_arr_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL == tag_name_ptr || PNULL == tag_arr_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] StartElementSax PNULL == tag_name_ptr || PNULL == tag_arr_ptr");
        goto ret;
    }
    ParseNameAndArr(localname_ptr, tag_name_ptr, tag_arr_ptr);
    len = MIN(MAX_TAG_NAME_LEN, strlen(tag_name_ptr));/*lint !e666*/
    SCI_MEMSET(g_store_data.s_cur_tagname, 0X0, (MAX_TAG_NAME_LEN+1));
    strncpy(g_store_data.s_cur_tagname, tag_name_ptr, len);
    startTagFunc(tag_name_ptr, tag_arr_ptr);

ret:
    if( PNULL!= tag_name_ptr)
    {
        SCI_FREE(tag_name_ptr);
    }
    if(PNULL!= tag_arr_ptr)
    {
        SCI_FREE(tag_arr_ptr);
    } 
}

/*****************************************************************************/
//  Description : a tag end callback
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void EndElementSax(  /*lint -esym(765,EndElementSax2Test) */
    void *user_data_ptr,
	const char *localname_ptr)
{	
    char* tag_name_ptr = PNULL;
    char* tag_arr_ptr = PNULL;
    int len = 0;
    char*  name_arr_ptr = PNULL; 
    if(PNULL == localname_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] EndElementSax PNULL == localname_ptr");
        return;
    }
    len = strlen(localname_ptr);
    tag_name_ptr = SCI_ALLOC_APPZ(len+1);
    tag_arr_ptr = SCI_ALLOC_APPZ(len+1);
    if(PNULL == tag_name_ptr || PNULL == tag_arr_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] PNULL == tag_name_ptr || PNULL == tag_arr_ptr");
        goto ret;
    }
    ParseNameAndArr(localname_ptr, tag_name_ptr, tag_arr_ptr);  
    endTagFunc(tag_name_ptr, tag_arr_ptr);
    
    
ret:
    if( PNULL!= tag_name_ptr)
    {
        SCI_FREE(tag_name_ptr);
    }
    if(PNULL!= tag_arr_ptr)
    {
        SCI_FREE(tag_arr_ptr);
    }     
}

/*****************************************************************************/
//  Description : a tag DATA callback
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void CharactersSAX(  /*lint -esym(765,CharactersSAX2Test) */
    void *user_data_ptr,
	const char *ch_ptr )
{
    if(PNULL != g_store_data.curDealTagFunc)
    {
    
        g_store_data.curDealTagFunc(ch_ptr);  
    }
}


/*****************************************************************************/
//  Description : parse dm xml packet
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_ParseXMLData(char* xmlbuf)   /*lint -esym(765,TestXmlParser) */
{
    CFLXML_PARSER_T* parser_ptr = PNULL;
    char sep_char = '$';
    char* syncml_ptr = PNULL;
    MMIDM_ReleaseXMLData();
    syncml_ptr = strstr(xmlbuf, "<SyncML");/*lint !e831*/
    MMIDM_PrintTrace(xmlbuf, 50);
    parser_ptr = CFLXML_CreateParserUTF8( StartElementSax, EndElementSax, CharactersSAX, &sep_char, NULL );
    if( parser_ptr )
    {                
        // parse now
        CFLXML_Parse( parser_ptr, (uint8*)syncml_ptr, strlen( syncml_ptr), 1 );/*lint !e668*/
        CFLXML_FreeParser( parser_ptr );
    }  
    else
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_ParseXMLData, parser_ptr == pnull");
    }
   // SCI_FREE(dm_buf);

 
}


/*****************************************************************************/
//  Description : 结束dm
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_EndDm(void)
{
   int inConnId = 1;
   //g_state_data.s_g_SessionID++;
   MMIDM_ReleaseXMLData();//free the memory alloc the tag content
   MMIDM_clearDmXmlData();
   VDM_Comm_PL_HTTP_close(PNULL, inConnId);
   VDM_Comm_PL_HTTP_term(PNULL); 
  // s_is_dm_run = FALSE;
}

/*****************************************************************************/
//  Description : 开启一个dm的http
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_BeginDmTansport(void)
{
    int inConnId = -1;
    
    if(DM_CANCEL == MMIDM_GetDmState())
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_BeginDmTansport DM_CANCEL == MMIDM_GetDmState()");
        return;
    }    
   
    VDM_Comm_PL_HTTP_open(PNULL, &inConnId, (unsigned char*)MMIDM_getResUri(), 0, 0, PNULL);/*lint !e718 !e18 */
    MMIDM_CreateDmSocketCheckTimer();


}

/*****************************************************************************/
//  Description : 解析收到的xml包，放入相应链表
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void EndDmParse(void)
{
    g_state_data.s_g_MsgID++;
  //  g_state_data.s_g_MsgID = 1;
    SCI_TRACE_LOW("[mmidm], EndDmParse, g_parse_data.s_statusTag_head 0x%x, g_parse_data.s_statusTag_tail 0x%x", g_parse_data.s_statusTag_head, g_parse_data.s_statusTag_tail);
    SCI_TRACE_LOW("[mmidm], EndDmParse, s_g_callClearFunc 0x%x, s_g_callLockFunc 0x%x", g_state_data.s_g_callClearFunc, g_state_data.s_g_callLockFunc);
    SCI_TRACE_LOW("[mmidm], EndDmParse g_state_data.s_g_needreplay %d, g_state_data.s_g_step %d", g_state_data.s_g_needreplay, g_state_data.s_g_step);

    if((g_state_data.s_g_step == STEP_CREDED)&&(g_parse_data.s_statusTag_head == g_parse_data.s_statusTag_tail)&& (PNULL == g_parse_data.s_alerTag_head) && (PNULL == g_parse_data.s_replaceTag_head) && (PNULL == g_parse_data.s_resultTag_head))
    {

        if(g_state_data.s_g_callClearFunc)
        {
            g_state_data.s_g_callClearFunc = FALSE;
            DM_ConfirmFactoryReset();
        }
        else if(g_state_data.s_g_callLockFunc)
        {
            g_state_data.s_g_callLockFunc = FALSE;
            DM_ConfirmPartiallyLock();
        }
        else if(g_state_data.s_g_callUnlockFunc)
        {
            g_state_data.s_g_callUnlockFunc = FALSE;
            DM_ConfirmUnLock();
        }
        else
        {

          //  if(!MMIDM_IsInConform())
            {
                SCI_TRACE_LOW("[mmidm], EndDmParse Exit");
                MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);//maryxiao111
            }
            return;
        }
    }     
    if(DM_CANCEL == MMIDM_GetDmState())
    {

        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }     
    else
    {

        if(!g_state_data.s_g_needreplay)
        {
#ifdef WIN32

            MMIDM_SendDmData();

#else
            MMIDM_SendSigToDmTask(DM_TASK_START_MESSAGE,MMIDM_GetDmTaskID(),PNULL);
#endif
        }

    }

}

/*****************************************************************************/
//  Description : 解析收到的xml包，放入相应链表
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_ParseReceiveData(char* data)
{
#ifndef WIN32
    VDM_Comm_PL_HTTP_close(PNULL,-1);
#endif    
    MMIDM_ParseXMLData(data);
    //g_state_data.s_g_SessionID++;

}
/*****************************************************************************/
//  Description : 产生xml包并发送
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SendDmData(void)
{
    int inConnId = 1;
    char* sendbuf = PNULL;
    if(DM_CANCEL == MMIDM_GetDmState())
    {
        SCI_TRACE_LOW("[MMIDM] MMIDM_SendDmData DM_CANCEL == MMIDM_GetDmState()");
        return;
    }
    sendbuf = SCI_ALLOCA(MAX_XML_BUF_SIZE);
    if(PNULL== sendbuf)
    {
        SCI_TRACE_LOW("MMIDM_SendDmData PNULL== sendbuf");
        return;
    }
     SCI_MEMSET(sendbuf, 0, MAX_XML_BUF_SIZE);
      
    MMIDM_generateXMLData(sendbuf, MAX_XML_BUF_SIZE);
    MMIDM_PrintTrace(sendbuf, strlen(sendbuf));

    VDM_Comm_PL_HTTP_send(PNULL,inConnId, (unsigned char*)sendbuf, strlen(sendbuf));
    
    if(g_state_data.s_g_step == STEP_ABORT)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);//maryxiao111
    }


    
    SCI_FREE(sendbuf);
    sendbuf = PNULL;    
#ifdef WIN32
    if((g_state_data.s_g_step ==STEP_GETNONCE)&&(access(DM_RECEIVE_PACKET1_PATH, 0)==0))
    {
    
        MMIDM_SimulatorReceiveData(DM_RECEIVE_PACKET1_PATH, STEP_GETNONCE);
    }
    else if((g_state_data.s_g_step ==STEP_GETNONCE_AGAIN)&&(access(DM_RECEIVE_PACKET2_PATH, 0)==0))
    {
         MMIDM_SimulatorReceiveData(DM_RECEIVE_PACKET2_PATH, STEP_GETNONCE_AGAIN);
    }
    else if((g_state_data.s_g_step ==STEP_CREDED) &&(access(DM_RECEIVE_PACKET3_PATH, 0)==0))
    {
        MMIDM_SimulatorReceiveData(DM_RECEIVE_PACKET3_PATH, STEP_CREDED);
    }

#endif    
}
/*****************************************************************************/
//  Description : 打印trace的函数
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_PrintTrace(char*buf, int size)
{
#ifdef WIN32

     FILE* fp=PNULL;
        fp=fopen(DM_SEND_PACKET_PATH,"a+");
        if(PNULL != fp)
        {
        
        fwrite(buf, size,1, fp);
        fprintf(fp, "\n");
        fprintf(fp, "\n");
        fclose(fp);
        }
#else
    if (MMIDM_GetLogState()) 
    {
        int i =0;

        for(i=0; i<size;i+=6)
        {
           SCI_TRACE_LOW("[MMIDM] %c%c%c%c%c%c",buf[i],buf[i+1],buf[i+2],buf[i+3],buf[i+4],buf[i+5]); 
        }
    }
#endif
}


#ifdef WIN32
/*****************************************************************************/
//  Description : 模拟接受数据接口
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL  MMIDM_SimulatorReceiveData(char* file_path, MMI_DM_STEP_E step)
{
    char* dm_buf = SCI_ALLOCA(MAX_XML_BUF_SIZE);
    if(PNULL != dm_buf)
    {
        FILE* fp=PNULL;
        SCI_MEMSET(dm_buf, 0X0, MAX_XML_BUF_SIZE);

        MMIDM_setSessionStep(step);
        fp=fopen(file_path,"r");//fp=fopen("g:/dm/dm.log","r");
        if(PNULL != fp)
        {
        
        fgets(dm_buf, MAX_XML_BUF_SIZE, fp);
        fclose(fp);
        }
        MMIDM_ParseReceiveData(dm_buf);

        SCI_FREE(dm_buf);
    }

}
/*****************************************************************************/
//  Description : 模拟收到短信后的第一包数据
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL  void MMIDM_SimulatorFirstPacket()
{
    MMIDM_SendDmData();
}

/*****************************************************************************/
//  Description : 模拟测试接口
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_Test(void)
{
    MMIDM_SimulatorFirstPacket();
}
#endif
