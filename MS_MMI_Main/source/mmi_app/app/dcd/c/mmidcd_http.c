/*****************************************************************************
** File Name:      mmidcd_http.c                                             *
** Author:                                                                   *
** Date:           11/2008                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008        byte.guo          Create                                   *
******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_dcd_trc.h"
#ifdef MMI_ENABLE_DCD	//!宏定义开头
#ifdef WIN32
#include "std_header.h"
#endif
#include <stdlib.h>
#include <string.h>
#include "Http_api.h"
#include "mmifmm_export.h"
#include "mmiudisk_export.h"

#include "mmidcd_http.h"
#include "mmidcd_main.h"
#include "mmidcd_file.h"
#include "mmi_module.h"
#include "mmiconnection_export.h" 
//#include "mmiset_multisim.h"
#include "mmiset_multisim_export.h"
#include "mmidcd_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
    
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/  

#define DCD_HTTP_FIELD_VALUE_GET(field_value,str_array,index)        \
    if( str_array[index] )                                    \
    {                                                                         \
        strcpy( field_value,str_array[index] );                               \
    }   

#define DCD_HTTP_HEADER_NUM   5
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 static DCD_HTTP_REQ_T*		        s_dcd_http_req_list_needed = PNULL;
static DCD_HTTP_REQ_T*		        s_dcd_http_req_list_finished = PNULL;
static DCD_HTTP_REQ_T*              s_req_data = PNULL;
	
static uint16                       s_moduleid = 0;
static uint32 			            s_session_id = 0;
static uint32 			            s_request_id = 0;
static uint32                             s_dcd_pdp_net_id=0;
static DCD_REQ_COMMON_PARAMETER_T   s_dcd_commom_req_para = {0};

static MMIDCD_HTTP_REQ_TYPE_E          s_req_type = HTTP_REQ_TYPE_UNUSE;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */

/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDCD_HttpSessionInit(uint16 moduleid);
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T* Http_ReqInit(
                                   MMIDCD_REQ_TYPE_E type,
                                   char   *uri_ptr,
                                   uint16 *gb_full_file_name_ptr,
                                   DCD_HTTP_HEADER_CELL_PTR ext_header,
                                   uint16 *head_len_arr_ptr,
                                   char   *ctxt_id_ptr
                                   );
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_ReqFree(DCD_HTTP_REQ_T* http_req_ptr);
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 Http_ReqListNum(DCD_HTTP_REQ_T *list_head_ptr);
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_ReqListFree( DCD_HTTP_REQ_T **list_head_pptr);
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Http_ContentIsExistInList(DCD_HTTP_REQ_T **list_head_pptr, 
                                        DCD_CONTENT_T* content_ptr);
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T *Http_ReqListAppend(DCD_HTTP_REQ_T **list_head_pptr, 
                                         DCD_HTTP_REQ_T *node_ptr);
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T* Http_ReqListPop(DCD_HTTP_REQ_T **list_head_pptr);
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T* Http_ReqListPush(DCD_HTTP_REQ_T **list_head_pptr,
                                       DCD_HTTP_REQ_T *node_ptr);
/*****************************************************************************/
//  Description :  set the cur req
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_SetCurReqData(DCD_HTTP_REQ_T* req_ptr);
/*****************************************************************************/
//  Description :  get the cur req
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T*Http_GetCurReqData();
/*****************************************************************************/
//  Description :  send req to http
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_SendReq(DCD_HTTP_REQ_T  *req_ptr,  MMIDCD_HTTP_REQ_TYPE_E req_type);

/*****************************************************************************/
//  Description : do the porper action when error happen in downloading.
//  Global resource dependence :
//  Author:考虑去掉，用一个代替
//  Note: 
/*****************************************************************************/
LOCAL void Http_DownloadErrorHander(void);

/*****************************************************************************/
//Description : decode the data which is MIME type 
//Global resource dependence : 
//Author:
//Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_HEADER_CELL_PTR Http_XmlReqHeaderInit(
    
    char* X_DP_RequestType_value_ptr,
    char* User_Agent_value_ptr,
    char* Version_value_ptr,
    char* Connection_Value_ptr,   
    
    char* X_DP_Global_Sync_Anchor_value_ptr,
    char* X_DP_Sync_Anchor_value_ptr,
    char* X_DP_Cell_Id_value_ptr,
    
    char* Accept_Encoding_value_ptr
);

/*****************************************************************************/
//Description : decode the data which is MIME type 
//Global resource dependence : 
//Author:
//Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_HEADER_CELL_PTR Http_ContentReqHeaderInit(
                                                        char* X_DP_RequestType_value,
                                                        char* X_DP_Request_Id_value,        
                                                        char* User_Agent_value,
                                                        char* Connection_Value,        
                                                        char* Version_value,
                                                        char* X_DP_Cell_Id_value,   
                                                        char* Accept_Encoding_value
                                                       );
/*****************************************************************************/
//Description : decode the data which is MIME type 
//Global resource dependence : 
//Author:mark.zhang
//Note: 
/*****************************************************************************/
LOCAL uint8*  MIMEDecodeByBuf(uint8* buf_ptr, uint32 *len_ptr);
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:jun.hu for 节省空间......
//  Note: 
/*****************************************************************************/
LOCAL char* GetHttpHeadName(MMIDCD_HTTP_REQ_TYPE_E req_type, uint16 index);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDCD_HttpSessionInit(uint16 moduleid)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    uint32 len=0;
    uint8 setting_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[MMIDCD]:DCD_HttpSessionInit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_204_112_2_18_2_12_46_103,(uint8*)"");

    init_param_ptr = MMIDCD_ALLOC(sizeof(HTTP_INIT_PARAM_T));
    if(PNULL == init_param_ptr)
    {
        return;
    }
    memset(init_param_ptr, 0, sizeof(HTTP_INIT_PARAM_T));

    setting_index = MMIDCD_Setting_GetNetSettingIndex(MMIAPIDCD_GeSettingInfo()->sim_slected);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIAPIDCD_GeSettingInfo()->sim_slected, setting_index);
    if(setting_item_ptr)
    {
        init_param_ptr->proxy.is_use_proxy = TRUE;
        init_param_ptr->proxy.http_port = setting_item_ptr->port;
        len = setting_item_ptr->gateway_len;
        init_param_ptr->proxy.proxy_host_ptr = MMIDCD_ALLOC(len + 1);
        if(PNULL == init_param_ptr->proxy.proxy_host_ptr )
        {
            MMIDCD_FreeMem(init_param_ptr);
            return;
        }
        memset(init_param_ptr->proxy.proxy_host_ptr,0, len + 1);
        memcpy(init_param_ptr->proxy.proxy_host_ptr, setting_item_ptr->gateway, len);
        init_param_ptr->net_id=DCD_HttpGetCurNetId();//DCD_HttpGetCurRequtestId();
#ifndef WIN32    
        HTTP_InitRequest(moduleid,moduleid,init_param_ptr);
#else
        DCD_Sendsig(HTTP_SIG_INIT_CNF,HTTP_REQ_TYPE_NULL);
#endif 
        MMIDCD_FreeMem(init_param_ptr->proxy.proxy_host_ptr);
    }

    
    MMIDCD_FreeMem(init_param_ptr);
    init_param_ptr = PNULL;
}
/*****************************************************************************/
//  Description :  close http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_HttpSessionClose(uint32 session_id)
{
#ifndef WIN32
    HTTP_ERROR_E http_result = HTTP_SUCCESS;
#endif
    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_HttpSessionClose"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_250_112_2_18_2_12_46_104,(uint8*)"");
    s_dcd_pdp_net_id=0;
    s_request_id=0;	
    if(s_dcd_http_req_list_needed)
    {
        Http_ReqListFree(&s_dcd_http_req_list_needed);
    }
    if(s_dcd_http_req_list_finished)
    {
        Http_ReqListFree(&s_dcd_http_req_list_finished);
    }
    
#ifndef WIN32
    http_result = HTTP_CloseRequest(session_id);
    // 只有当请求不成功时，再去调用MMIDCD_DeactivatePDPcontext,否则要等到收到HTTP_SIG_CLOSE_CNF消息后再调用该函数...
    if(http_result != HTTP_SUCCESS)
    {
        MMIDCD_DeactivatePDPcontext();
        s_session_id=0;
    }
#else
    DCD_Sendsig(HTTP_SIG_CLOSE_CNF, HTTP_REQ_TYPE_NULL);  
#endif
    
} 
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T* Http_ReqInit(
                                   MMIDCD_REQ_TYPE_E type,
                                   char   *uri_ptr,
                                   uint16 *gb_full_file_name_ptr,
                                   DCD_HTTP_HEADER_CELL_PTR ext_header,
                                   uint16 *head_len_arr_ptr,
                                   char   *ctxt_id_ptr
                                   )
{
    DCD_HTTP_REQ_T *req_ptr = PNULL;
    uint16 len = 0;
    
    if(type == HTTP_REQ_TYPE_UNUSE 
        || type >= HTTP_REQ_TYPE_MAX
        ||PNULL == uri_ptr
        ||PNULL == gb_full_file_name_ptr
        ||PNULL == ext_header
        ||PNULL == head_len_arr_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:Http_ReqInit ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_298_112_2_18_2_12_46_105,(uint8*)"");
        return PNULL;
    }

    req_ptr = (DCD_HTTP_REQ_T*)MMIDCD_ALLOC( sizeof(DCD_HTTP_REQ_T) );
    if(PNULL == req_ptr )
    {
        return PNULL;
    }
    SCI_MEMSET( req_ptr,0,sizeof(DCD_HTTP_REQ_T) );
    
    req_ptr->type = type;
    
    len= MIN(DCD_HTTP_REQ_URI_MAX_LEN, SCI_STRLEN(uri_ptr)); /*lint !e666*/       
    SCI_MEMCPY(req_ptr->uri, uri_ptr, len);
    
    if(PNULL != ctxt_id_ptr)
    {
        SCI_MEMCPY(req_ptr->ctxt_id, ctxt_id_ptr, DCD_CTXT_ID_MAX_LEN);
    }
    else
    {
        SCI_MEMSET(req_ptr->ctxt_id, 0x00, DCD_CTXT_ID_MAX_LEN);
    }

    SCI_MEMCPY( req_ptr->gb_full_file_name, gb_full_file_name_ptr, DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN);
    req_ptr->ext_header = ext_header;   
    SCI_MEMSET(req_ptr->head_len_array, 0x00, DCD_MAX_HEADER_NUM);
    SCI_MEMCPY(req_ptr->head_len_array, head_len_arr_ptr, DCD_MAX_HEADER_NUM);  
    req_ptr->next_ptr = PNULL;
    
    return req_ptr;
}
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_ReqFree(DCD_HTTP_REQ_T* http_req_ptr)
{
	if( http_req_ptr!=PNULL )
	{
		if( http_req_ptr->ext_header!=PNULL )
		{
			MMIDCD_FreeMem(http_req_ptr->ext_header);
			http_req_ptr->ext_header = PNULL;
		}
			
		MMIDCD_FreeMem(http_req_ptr);
		http_req_ptr = PNULL;
	}
	
}
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 Http_ReqListNum(DCD_HTTP_REQ_T *list_head_ptr)
{
    uint16  list_len = 0;
    DCD_HTTP_REQ_T *cur_ptr = PNULL;

    if(PNULL == list_head_ptr)
    {
        return list_len;
    }    
    
    cur_ptr  =  list_head_ptr;
    while( cur_ptr != PNULL )
    {
        list_len ++;
        cur_ptr = cur_ptr -> next_ptr;
    }
 
    return list_len;
}
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_ReqListFree( DCD_HTTP_REQ_T **list_head_pptr)
{
    DCD_HTTP_REQ_T  *cur_ptr = PNULL;
    DCD_HTTP_REQ_T  *pre_ptr = PNULL;
    
    if(PNULL == list_head_pptr || PNULL == (*list_head_pptr))
    {
        return; 
    }
    cur_ptr = (*list_head_pptr);
    while(cur_ptr != PNULL)
    {        
        pre_ptr = cur_ptr;
        cur_ptr = (DCD_HTTP_REQ_T *)pre_ptr->next_ptr;

        if(pre_ptr ->ext_header != PNULL)	
        {
            MMIDCD_FreeMem(pre_ptr ->ext_header);        
            pre_ptr ->ext_header = PNULL;
        }
        MMIDCD_FreeMem(pre_ptr);
        pre_ptr = PNULL;
    }
    (*list_head_pptr) = PNULL;           
}


/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Http_ContentIsExistInList(DCD_HTTP_REQ_T **list_head_pptr, DCD_CONTENT_T* content_ptr)
{
    DCD_HTTP_REQ_T *next_ptr =PNULL;
    BOOLEAN ret= FALSE;	
               
    if(PNULL == list_head_pptr)
    {
        return FALSE;
    }

    next_ptr = (*list_head_pptr);
    if(content_ptr != PNULL)
    {
        while(next_ptr!= PNULL)
        {
            if(!MMIAPICOM_Stricmp((const uint8*)content_ptr->ctxt_id,(const uint8*)next_ptr->ctxt_id))
            {
                ret= TRUE;
                break;
            }
            next_ptr = next_ptr->next_ptr;
        }
    }
    return ret;
}


/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T *Http_ReqListAppend(DCD_HTTP_REQ_T **list_head_pptr, DCD_HTTP_REQ_T *node_ptr)
{
    DCD_HTTP_REQ_T *next_ptr =PNULL;
 
    if(PNULL == list_head_pptr || PNULL == node_ptr)
    {
        return PNULL;
    }
     
    if(PNULL == (*list_head_pptr))
    {
        (*list_head_pptr)= node_ptr;   
    }
    else    
    {
        next_ptr = (*list_head_pptr);   
        /* if the next ptr is NULL,add the node to the ptr,and loop is over*/
        while(  (PNULL != next_ptr->next_ptr)  )
        {            
            next_ptr = next_ptr->next_ptr;
        }
        
        next_ptr->next_ptr = node_ptr;   
    }
    node_ptr->next_ptr = PNULL;   
    
    return node_ptr;
}
/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T* Http_ReqListPop(DCD_HTTP_REQ_T **list_head_pptr)
{
    DCD_HTTP_REQ_T* node_ptr = PNULL;
 
    if(PNULL == list_head_pptr)
    {
        return PNULL;
    }
    
    if(PNULL == (*list_head_pptr))
    {
    	node_ptr= PNULL;    
    }
    else
    {
    	node_ptr           = (*list_head_pptr);
	(*list_head_pptr)  = node_ptr->next_ptr;
    	node_ptr->next_ptr = PNULL;    	
    	
    }

   return node_ptr;
	
}

/*****************************************************************************/
//  Description :  init http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T* Http_ReqListPush(DCD_HTTP_REQ_T **list_head_pptr,DCD_HTTP_REQ_T *node_ptr)
{
    if(PNULL == list_head_pptr || PNULL == node_ptr)
    {
        return PNULL;
    }
    
    if(PNULL == (*list_head_pptr))
    {
	    (*list_head_pptr) = node_ptr;
	    node_ptr->next_ptr = PNULL;
    }
    else
    {
        node_ptr->next_ptr = (*list_head_pptr);
	    (*list_head_pptr) = node_ptr;
    }
    
    return node_ptr;    
}

/*****************************************************************************/
//  Description :  set the cur req
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_SetCurReqData(DCD_HTTP_REQ_T* req_ptr)
{
    if(PNULL != req_ptr)
    {
        s_req_data = req_ptr;
    }
}

/*****************************************************************************/
//  Description :  get the cur req
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_REQ_T*Http_GetCurReqData()
{
    return  s_req_data;
}

/*****************************************************************************/
//  Description :  set the session id
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_HttpSetCurSessionId(uint32 id)
{
    s_session_id = id;	
}

PUBLIC void DCD_HttpSetCurRequtestId(uint32 id)
{
    s_request_id = id;	
}

PUBLIC void DCD_HttpSetCurNetId(uint32 id)
{
    s_dcd_pdp_net_id= id;	
}

/*****************************************************************************/
//  Description :  get the session id
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 DCD_HttpGetCurSessionId(void)
{
    return s_session_id;	
}

PUBLIC uint32 DCD_HttpGetCurRequtestId(void)
{
    return s_request_id;	
}

PUBLIC uint32 DCD_HttpGetCurNetId(void)
{
    return s_dcd_pdp_net_id;	
}
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL char* GetHttpHeadName(MMIDCD_HTTP_REQ_TYPE_E req_type, uint16 index)
{
    char * ret_char_ptr = NULL;
	
    if(HTTP_REQ_TYPE_XML == req_type)
    {
        switch(index)
        {
            case 0:
                ret_char_ptr = DCD_HTTP_REQUESTTYPE_STR;
                break;
            case 1:
                ret_char_ptr = DCD_HTTP_USERAGENT_STR;
                break;
            case 2:
                ret_char_ptr = DCD_HTTP_VERSION_STR;
                break;
            default:
                ret_char_ptr = NULL;
                break;
        }
    }
    else
    {
        switch(index)
        {
            case 0:
                ret_char_ptr = DCD_HTTP_REQUESTTYPE_STR;
                break;
            case 1:
                ret_char_ptr = DCD_HTTP_REQUUSTID_STR;
                break;
            case 2:
                ret_char_ptr = DCD_HTTP_USERAGENT_STR;
                break;             
             case 3:
                ret_char_ptr = DCD_HTTP_CONNECTION_STR;
                break;
	     case 4:
		  ret_char_ptr = DCD_HTTP_VERSION_STR ;
                break;
            default:
		   ret_char_ptr = NULL;
                break;
        }
    }

    return ret_char_ptr;
}
/*****************************************************************************/
//  Description :  send req to http
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Http_SendReq(DCD_HTTP_REQ_T  *req_ptr,  MMIDCD_HTTP_REQ_TYPE_E req_type)
{
    HTTP_GET_PARAM_T* get_param_ptr = PNULL;		
    uint16 index = 0;
    uint16 i=0;
    uint16 head_num=0;	
    uint16 len = 0;    
    char *name_ptr=PNULL;
    char * value_ptr=PNULL;
    
    if(PNULL == req_ptr)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"[MMIDCD]:Http_SendReq() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_651_112_2_18_2_12_47_106,(uint8*)"");
    
    get_param_ptr = MMIDCD_ALLOC(sizeof (HTTP_GET_PARAM_T));
    
    memset(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
    
    len = strlen(req_ptr->uri);
    get_param_ptr->uri.uri_ptr = MMIDCD_ALLOC(len+1);
    memset(get_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(get_param_ptr->uri.uri_ptr,req_ptr->uri);
    
    get_param_ptr->extern_header.header_num = DCD_HTTP_HEADER_NUM;
    get_param_ptr->extern_header.other_header_ptr = MMIDCD_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
    memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
    
    /*1*/
    head_num = 0;
    do
    {
        char* head_name_ptr = PNULL;
        len =DCD_HTTP_FIELD_VALUE_MAX_LEN;// strlen("X-DP-RequestType");
        
        get_param_ptr->extern_header.other_header_ptr[head_num].header_name_ptr = MMIDCD_ALLOC(len+1);
        
        get_param_ptr->extern_header.other_header_ptr[head_num].header_value_ptr = MMIDCD_ALLOC(len+1);

        name_ptr = get_param_ptr->extern_header.other_header_ptr[head_num].header_name_ptr;
        value_ptr =get_param_ptr->extern_header.other_header_ptr[head_num].header_value_ptr;

        memset(name_ptr, 0, len + 1);
        // added by jun.hu 
        memset(value_ptr, 0, len + 1);
        head_name_ptr = GetHttpHeadName(req_type, index);
        if(NULL != head_name_ptr)
        {
            //SCI_TRACE_LOW:"MMIDCD name = %s, index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_686_112_2_18_2_12_47_107,(uint8*)"sd", head_name_ptr, index);
            strcpy(name_ptr, head_name_ptr);
        }
        else
        {
            break;
        }

        DCD_HTTP_FIELD_VALUE_GET(value_ptr, req_ptr->ext_header,index);

        index++;
        head_num++;	

        len = strlen(req_ptr->ext_header[index]);
        
    }while(len!=0 &&head_num<DCD_HTTP_HEADER_NUM);
    
    
    get_param_ptr->get_data.is_header_use_file = FALSE;          //set header buffer or file
    SCI_MEMSET(get_param_ptr->get_data.content_file_name, 0x00, DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN);
    memcpy(get_param_ptr->get_data.content_file_name,(char*)req_ptr->gb_full_file_name,DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN);
    
    
#ifdef DCD_BUFFER_INTERPRETE//是否使用的是BUFFER机制
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
#else
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_FILE;
#endif
    
    
    get_param_ptr->need_net_prog_ind = TRUE;
    
#ifndef WIN32	
    HTTP_GetRequest(DCD_HttpGetCurSessionId(),get_param_ptr,0); //context_id for init cnf	
#endif	
    
    
    
    MMIDCD_FreeMem(get_param_ptr->uri.uri_ptr);
    
    for(i=0; i<head_num; i++)
    {
        MMIDCD_FreeMem(get_param_ptr->extern_header.other_header_ptr[i].header_name_ptr);
        MMIDCD_FreeMem(get_param_ptr->extern_header.other_header_ptr[i].header_value_ptr);
    }

    MMIDCD_FreeMem(get_param_ptr->extern_header.other_header_ptr);
    MMIDCD_FreeMem(get_param_ptr);
    
#ifdef WIN32
    DCD_Sendsig(HTTP_SIG_GET_CNF,HTTP_REQ_TYPE_NULL);   
#endif
    
    //SCI_TRACE_LOW:"Http_SendReq() exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_739_112_2_18_2_12_47_108,(uint8*)"");
    
    
} 


/*****************************************************************************/
//  Description :  get a req from req list and send the req
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_ContentDownloadProcess(void)
{

	DCD_HTTP_REQ_T  *req_data_ptr = PNULL;

	//SCI_TRACE_LOW:"Http_ContentDownloadProcess() enter"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_754_112_2_18_2_12_47_109,(uint8*)"");
    
	req_data_ptr = (DCD_HTTP_REQ_T*)Http_ReqListPop(&s_dcd_http_req_list_needed);
     
       
	if(PNULL != req_data_ptr)
	{
	    /*the list for the interpreting of the image file */
	    Http_ReqListPush( &s_dcd_http_req_list_finished,req_data_ptr );  	

	    Http_SetCurReqData(req_data_ptr);
		
	    s_req_type = HTTP_REQ_TYPE_CONTENT;
	
	    Http_SendReq(req_data_ptr, HTTP_REQ_TYPE_CONTENT);	
	}
	else
	{
        //表示MIMIE FILE ALL  下在完毕
         DCD_Sendsig(DCD_SIG_MIME_DL_CNF,HTTP_REQ_TYPE_NULL);
        s_req_type = HTTP_REQ_TYPE_UNUSE;	

	}
	//SCI_TRACE_LOW:"Http_ContentDownloadProcess() exit"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_777_112_2_18_2_12_47_110,(uint8*)"");

}

/*****************************************************************************/
//  Description : do the porper action when error happen in downloading.
//  Global resource dependence :
//  Author:考虑去掉，用一个代替
//  Note: 
/*****************************************************************************/
LOCAL void Http_DownloadErrorHander(void)
{
    switch(s_req_type)
    {
    case HTTP_REQ_TYPE_XML:
        //SCI_TRACE_LOW:"Http_DownloadErrorHander() HTTP_REQ_TYPE_XML"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_790_112_2_18_2_12_47_111,(uint8*)"");
        s_req_type = HTTP_REQ_TYPE_UNUSE;
        if(s_dcd_http_req_list_needed)
        {
            Http_ReqListFree(&s_dcd_http_req_list_needed);
            s_dcd_http_req_list_needed = PNULL;
        }	    
        break;
        
    case HTTP_REQ_TYPE_CONTENT:	
        //SCI_TRACE_LOW:"Http_DownloadErrorHander() DCD_HTTP_REQ_TYPE_CONTENT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_800_112_2_18_2_12_47_112,(uint8*)"");
        s_req_type = HTTP_REQ_TYPE_UNUSE;
        if(s_dcd_http_req_list_needed)
        {
            Http_ReqListFree(&s_dcd_http_req_list_needed);
            s_dcd_http_req_list_needed = PNULL;
        }	    			
        break;
        
    case HTTP_REQ_TYPE_UNUSE:
    case HTTP_REQ_TYPE_MIME:
    case HTTP_REQ_TYPE_MAX:		
    default:
        //SCI_TRACE_LOW:"Http_DownloadErrorHander() handle unkown http error!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_813_112_2_18_2_12_47_113,(uint8*)"");
        break;
        
    }
}

#ifdef DCD_BUFFER_INTERPRETE
/*****************************************************************************/
//  Description :  the downloaded dcd content is of MIME type, should be interpreted
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Http_ContentInterpreteProcess(uint8 *data_ptr,uint32 data_len)
{
    SFS_HANDLE      sfs_handle = 0;
    DCD_HTTP_REQ_T *req_data_ptr =PNULL;

    uint32 file_size =data_len;
    uint8 *binary_data_ptr = PNULL;
    SFS_ERROR_E			sfs_error = SFS_ERROR_NONE;
    uint16 full_file_name[DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN]={0};	

    //SCI_TRACE_LOW:"Http_ContentInterpreteProcess() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_834_112_2_18_2_12_47_114,(uint8*)"");
    if(s_dcd_http_req_list_finished)
    {
        req_data_ptr = (DCD_HTTP_REQ_T*)Http_ReqListPop(&s_dcd_http_req_list_finished);

        SCI_MEMSET(full_file_name, 0x00, DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN);
        SCI_MEMCPY(full_file_name, req_data_ptr->gb_full_file_name, DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN)


        if( PNULL != data_ptr)
        {			 	    		 
            /* interprete the image file */	
            binary_data_ptr = MIMEDecodeByBuf(data_ptr,&file_size);

            if(PNULL == binary_data_ptr)
            {
                /*Return ,if invalid*/
                //SCI_TRACE_LOW:"[DCD] Return ,if invalid"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_851_112_2_18_2_12_47_115,(uint8*)"");
                return FALSE;
            }		 

            sfs_handle = MMIAPIFMM_CreateFile(full_file_name, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
            if(sfs_handle)
            {
                sfs_error = MMIAPIFMM_WriteFile(sfs_handle, binary_data_ptr, file_size, &file_size, PNULL);
                if(SFS_ERROR_NONE != sfs_error)
                {
                    //SCI_TRACE_LOW:"[DCD]  Write file error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_861_112_2_18_2_12_47_116,(uint8*)"");
                }
                else
                {	 
                    MMIAPIFMM_CloseFile(sfs_handle);				   
                }
            }
            else
            {
                //SCI_TRACE_LOW:"[DCD]  Create file error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_870_112_2_18_2_12_47_117,(uint8*)"");
                return FALSE;
            }

        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDCD]:Http_ContentInterpreteProcess data_ptr pnull!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_877_112_2_18_2_12_47_118,(uint8*)"");
            return FALSE;
        }
        Http_ReqFree(req_data_ptr);
        return TRUE
    }
    else
    {
       //SCI_TRACE_LOW:"[DCD]  Interprete Over!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_885_112_2_18_2_12_47_119,(uint8*)"");
        return TRUE;
        
    }
}
#else

/*****************************************************************************/
//  Description :  the downloaded dcd content is of MIME type, should be interpreted
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Http_ContentInterpreteProcess(void)
{
    SFS_ERROR_E	    sfs_error  = SFS_ERROR_NONE;
    SFS_HANDLE      sfs_handle = SFS_INVALID_HANDLE;
    
    uint32 file_size =0;
    uint8 *data_ptr = PNULL;
    uint8 *binary_data_ptr = PNULL;
    DCD_HTTP_REQ_T  *user_data_ptr = PNULL;
    
    wchar full_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};	
    
    //SCI_TRACE_MID:"MMIDCD Http_ContentInterpreteProcess entered time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_908_112_2_18_2_12_47_120,(uint8*)"d", SCI_GetTickCount());
    while(s_dcd_http_req_list_finished)
    {
        user_data_ptr = (DCD_HTTP_REQ_T*)Http_ReqListPop(&s_dcd_http_req_list_finished);
        if(PNULL == user_data_ptr)
        {
            continue;
        }

        SCI_MEMSET(full_file_name, 0x00, sizeof(full_file_name));
        SCI_MEMCPY(full_file_name, user_data_ptr->gb_full_file_name, MMIFILE_FILE_NAME_MAX_LEN + 1);
        
        sfs_handle = MMIAPIFMM_CreateFile(full_file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE, PNULL, PNULL); 
        if(SFS_INVALID_HANDLE != sfs_handle)
        {
            file_size = MMIAPIFMM_GetFileSize(sfs_handle);		
            if(file_size == 0)
            {
                MMIAPIFMM_CloseFile(sfs_handle);	
                Http_ReqFree(user_data_ptr);
                return FALSE;		  
            }
            else
            {
                data_ptr = MMIDCD_ALLOC(file_size);   
                if(PNULL == data_ptr)
                {
                    continue;
                }
            }
            
            sfs_error = MMIAPIFMM_ReadFile(sfs_handle, data_ptr, file_size, &file_size, PNULL);	     
            if(SFS_ERROR_NONE != sfs_error)
            {
                //SCI_TRACE_LOW:"[MMIDCD]  Http_ContentInterpreteProcess; READ FILE  ERROR!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_942_112_2_18_2_12_47_121,(uint8*)"");
            }  	
            binary_data_ptr = MIMEDecodeByBuf(data_ptr, &file_size);
            if(PNULL == binary_data_ptr)
            {
                MMIDCD_FreeMem(data_ptr);
                Http_ReqFree(user_data_ptr);
                MMIAPIFMM_CloseFile(sfs_handle);
                MMIAPIFMM_DeleteFile((const uint16 *)full_file_name, PNULL);
                return FALSE;
            }
            SFS_SetFilePointer(sfs_handle, 0, SFS_SEEK_BEGIN);/*lint !e747*/
            sfs_error = MMIAPIFMM_WriteFile(sfs_handle, binary_data_ptr, file_size, &file_size, PNULL);
            if(SFS_ERROR_NONE != sfs_error)
            {
                //SCI_TRACE_MID:"MMIDCD  MMIAPIFMM_WriteFile error = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_957_112_2_18_2_12_47_122,(uint8*)"d", sfs_error);
            }

            MMIAPIFMM_SetFileSize(sfs_handle, file_size);
            MMIDCD_FreeMem(data_ptr);			 
            MMIAPIFMM_CloseFile(sfs_handle);
        }
        else
	 {
		Http_ReqFree(user_data_ptr);
		return FALSE;
	 }

        Http_ReqFree(user_data_ptr);
    }
    
    //SCI_TRACE_MID:"MMIDCD   Http_ContentInterpreteProcess exit time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_973_112_2_18_2_12_47_123,(uint8*)"d", SCI_GetTickCount());
    return TRUE;
}


#endif
/*****************************************************************************/
//  Description :  handle msg when there exist an error
//  Global resource dependence :
//  Author: zhuo.wang
//  Note: modified by @jun.hu, 2009/8/13
/*****************************************************************************/
PUBLIC MMIDCD_ERROR_CODE_E Http_ErrorHandler(uint32 errorcode)
{    
    MMIDCD_ERROR_CODE_E error_code = DCD_ERROR_NULL;
    
    //SCI_TRACE_LOW:"[MMIDCD]:Http_ErrorHandler; errorcode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_987_112_2_18_2_12_47_124,(uint8*)"d",errorcode);
    
    switch(errorcode)
    {   
        case  HTTP_ERROR_NO_MEMORY:               //alloc memory failed
        case  HTTP_ERROR_FILE_NO_SPACE:
            if(MMIAPIUDISK_UdiskIsRun())
            {
                error_code = DCD_ERROR_UDISK_USING;
            }
            else
            {
                error_code = DCD_ERROR_NO_MEMORY;
            }
            break;

        case HTTP_ERROR_FILE_OPEN:              //file open error
        case HTTP_ERROR_FILE_WRITE:              //file write error
        case HTTP_ERROR_FILE_NOT_FOUND:          //not found http file
        case HTTP_ERROR_FILE_DELETE:             //delete file error
        case HTTP_ERROR_FILE_GET_SIZE:           //get file size error
        case HTTP_ERROR_FILE_READ:              //read file error     
            Http_DownloadErrorHander();
            if(MMIAPIUDISK_UdiskIsRun())
            {
                error_code = DCD_ERROR_UDISK_USING;
            }
            else
            {
                error_code = DCD_ERROR_WRONG_DATA;  //703
            }
            break;

        
        case  HTTP_ERROR_FAILED:               //request failed
        case  HTTP_ERROR_INVALID_INPUT:           //input param is invalid
        case HTTP_ERROR_NET_DATA_ERROR:          //the data from server is error
        case HTTP_ERROR_PROXY_CONNECT:           //proxy connect error
        case HTTP_ERROR_SERVER_CLOSE:            //server close
        case HTTP_ERROR_HANDSHAKE:               //handshake error
        case HTTP_ERROR_PROXY_HANDSHAKE:         //proxy handshake error
        case HTTP_ERROR_CONNECT:                //connect error          
            Http_DownloadErrorHander();
            error_code = DCD_ERROR_INVALID_NETWORK;  //706
            break;			
      
        case HTTP_ERROR_RECV:                    //receive error
        case HTTP_ERROR_SEND:                    //send error
            Http_DownloadErrorHander();
            error_code = DCD_ERROR_NO_XML;   //702
            break;
			
        case HTTP_ERROR_UNKNOWN:                 //unknown error     
        case HTTP_ERROR_TIMEOUT:                 //time out 708
        default:        
            Http_DownloadErrorHander();
            error_code = DCD_ERROR_TIMEOUT;  //708
            break;
        
    }
    
    if(!MMIDCD_NetWorkIsValid())
    {
        error_code = DCD_ERROR_INVALID_NETWORK;  //706
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description :  send msg for downloading XML
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_DownloadXML(uint32 session_id)
{   
    DCD_HTTP_REQ_T*  user_data   = PNULL;
    
    //SCI_TRACE_LOW:"[MMIDCD]:Http_DownloadXML() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1064_112_2_18_2_12_48_125,(uint8*)"");
    
    DCD_HttpSetCurSessionId(session_id);
    
    if(Http_ReqListNum(s_dcd_http_req_list_needed) == 0)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:Http_DownloadXML:Http_ReqListNum(s_dcd_http_req_list_needed)==0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1070_112_2_18_2_12_48_126,(uint8*)"");
        return;
    }
    
    user_data = (DCD_HTTP_REQ_T*)Http_ReqListPop(&s_dcd_http_req_list_needed);   
    
    Http_SetCurReqData(user_data);
    s_req_type = HTTP_REQ_TYPE_XML;
    Http_SendReq(user_data, HTTP_REQ_TYPE_XML);

    //SCI_TRACE_LOW:"Http_DownloadXML() exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1080_112_2_18_2_12_48_127,(uint8*)"");
       
}

/*****************************************************************************/
//  Description :  when pasering, build the download req list
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_DownloadContentCallback(DCD_CONTENT_T* content_ptr)
{
    BOOLEAN                  ret = FALSE;
    DCD_HTTP_REQ_T*          req_ptr = PNULL;
    DCD_HTTP_HEADER_CELL_PTR ext_header = PNULL;
    
    char uri[DCD_HTTP_REQ_URI_MAX_LEN] = {0};
    uint16 ucs2_content_default_file[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};
    uint16 head_len_array[DCD_MAX_HEADER_NUM] = {0};  		
    
    if(PNULL == content_ptr)
    {
        return;
    }
    
    MMIDCDFILE_GetOnePictureFullPath(ucs2_content_default_file, content_ptr->ctxt_id, MMIDCD_GetContentTypeStr(content_ptr->content_type), MMIDCD_PICTURE_DIR_PATH);
    /* if the image file named ctxt-id is exist, need not send the data download requst  again*/
//    result= MMIAPIFMM_IsFileExist(ucs2_content_default_file,MMIAPICOM_Wstrlen(ucs2_content_default_file));
//     if(!result)
    {			
        /* send the data download requst with the same ctxt-id */
        ret = Http_ContentIsExistInList(&s_dcd_http_req_list_needed,content_ptr);
        if(!ret)
        {
            //content file
            if(MMIAPIDCD_MAIN_GetNetHomePage())
            {
                strcpy( uri, MMIAPIDCD_MAIN_GetNetHomePage());	
            }
            strcat( uri,"/content/refresh/elements");
            ext_header = (DCD_HTTP_HEADER_CELL_PTR)Http_ContentReqHeaderInit(
                                                                            "data",
                                                                            content_ptr->ctxt_id,
                                                                            s_dcd_commom_req_para.User_Agent,
                                                                            "keep-alive",
                                                                            s_dcd_commom_req_para.Version,
                                                                            PNULL,
                                                                            PNULL
                                                                            );	
            
            head_len_array[0] = strlen("X-DP-RequestType");
            head_len_array[1] = strlen("data"); 
            
            head_len_array[2] = strlen("X-DP-Request-Id");
            head_len_array[3] = strlen(content_ptr->ctxt_id);
            
            head_len_array[4] = strlen("User-Agent");
            head_len_array[5] = strlen(s_dcd_commom_req_para.User_Agent);
            
            head_len_array[6] = strlen("Connection");
            head_len_array[7] = strlen("keep-alive");	
            
            head_len_array[8] = strlen("Version");
            head_len_array[9] = strlen(s_dcd_commom_req_para.Version);	
            
            req_ptr = (DCD_HTTP_REQ_T*)Http_ReqInit(	
                                                    HTTP_REQ_TYPE_CONTENT,    /*lint !e64*/
                                                    uri,
                                                    ucs2_content_default_file,
                                                    ext_header,
                                                    head_len_array,
                                                    content_ptr ->ctxt_id
                                                    );   
            
            //加content数据到该list中				
            Http_ReqListAppend( &s_dcd_http_req_list_needed,req_ptr );	
        }
    }
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_ContentDownloadPre(void)
{
    /*xml解析完后调用*/
    MMIDCDFILE_FindUseablePictures();
    MMIDCD_DownloadContentByList(MMIAPIDCD_GetXmlDoc_Ptr(), Http_DownloadContentCallback);
}
/*****************************************************************************/
//  Description :  init http and the req
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_DownloadXMLPre(uint16 module_id)
{
    DCD_HTTP_REQ_T* req_ptr = PNULL;
    DCD_HTTP_HEADER_CELL_PTR *ext_header = PNULL;
    char    uri[DCD_HTTP_REQ_URI_MAX_LEN] = {0};
    wchar  ucs2_xml_default_file[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};  
    uint16  head_len_array[DCD_MAX_HEADER_NUM] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    //SCI_TRACE_LOW:"Http_DownloadXMLPre() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1180_112_2_18_2_12_48_128,(uint8*)"");
    
    /*++++++++++++++++++++++++++++++<<  组合URL全路径 >>++++++++++++++++++++++++++++++*/
    if(MMIAPIDCD_MAIN_GetNetHomePage())
    {
        strcpy( uri, MMIAPIDCD_MAIN_GetNetHomePage());	
    }
    
    strcat( uri,"/content/refresh/dp");
    
    
    /*++++++++++++++<<  下载前都要把前一次的XML文件换个名字保存一下 >>++++++++++++++++*/
#ifndef WIN32 
   MMIDCDFILE_RenameXMLFile(MMIDCD_INDEX_FILE_PATH, MMIDCD_OLD_FILE_PATH);
#endif
    
    if(!MMIDCDFILE_GetFilePathForRead(ucs2_xml_default_file , &full_path_len, MMIDCD_INDEX_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1198_112_2_18_2_12_48_129,(uint8*)"");
    }
    
    if(s_dcd_http_req_list_needed)
    {
        Http_ReqListFree(&s_dcd_http_req_list_needed);
    }
    
    /*++++++++++++++++++++++++<<  将xmlfile req加入到列表中 >>++++++++++++++++++++++++*/
    //xmlfile                               
    ext_header = (DCD_HTTP_HEADER_CELL_PTR*)Http_XmlReqHeaderInit(
                                                                s_dcd_commom_req_para.Req_Type,
                                                                s_dcd_commom_req_para.User_Agent,
                                                                s_dcd_commom_req_para.Version,
                                                                "keep-alive",						                        
                                                                PNULL,
                                                                PNULL,
                                                                PNULL,
                                                                PNULL
                                                                );	
    
    head_len_array[0] = strlen("X-DP-RequestType");
    head_len_array[1] = strlen(s_dcd_commom_req_para.Req_Type);
    
    head_len_array[2] = strlen("User-Agent");
    head_len_array[3] = strlen(s_dcd_commom_req_para.User_Agent);
    
#if 1  // jun.hu for Http_XmlReqHeaderInit 里面把"Connection"去掉了，外面也去掉....
    head_len_array[4] = strlen("Connection");
    head_len_array[5] = strlen("keep-alive");
    
    head_len_array[6] = strlen("Version");
    head_len_array[7] = strlen(s_dcd_commom_req_para.Version);
#else
    head_len_array[4] = strlen("Version");
    head_len_array[5] = strlen(s_dcd_commom_req_para.Version);
#endif
    
    //SCI_TRACE_LOW:"[MMIDCD]:DCD REQ TYPE :%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1236_112_2_18_2_12_48_130,(uint8*)"s",s_dcd_commom_req_para.Req_Type);
    //SCI_TRACE_LOW:"[MMIDCD]:DCD UA :%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1237_112_2_18_2_12_48_131,(uint8*)"s",s_dcd_commom_req_para.User_Agent);
    //SCI_TRACE_LOW:"[MMIDCD]:DCD URL :%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1238_112_2_18_2_12_48_132,(uint8*)"s",uri);
    
    //加xmlfile数据到该list中
    req_ptr = (DCD_HTTP_REQ_T*)Http_ReqInit(	
                                            HTTP_REQ_TYPE_XML,   /*lint !e64*/
                                            uri,
                                            ucs2_xml_default_file,
                                            ext_header,          /*lint !e64*/
                                            head_len_array,
                                            PNULL
                                            );
				
    
    Http_ReqListAppend( &s_dcd_http_req_list_needed,req_ptr );
    
    
    MMIDCD_HttpSessionInit(module_id);	
    
}  


/*****************************************************************************/
//  Description : DCD_SetReqype
//  Global resource dependence :
//  Author://没写到文档中
//  Note: app interface
/*****************************************************************************/
PUBLIC void Http_FreeXmlReqList(void)
{
    Http_ReqFree(Http_GetCurReqData());
    s_dcd_http_req_list_needed = PNULL;
}
  
/*****************************************************************************/
//  Description : DCD_SetReqype
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
PUBLIC void MMIDCD_SetReqType(MMIDCD_REQ_TYPE_E req_type)
{
    SCI_MEMSET(s_dcd_commom_req_para.Req_Type, 0x00, DCD_REQ_TYPE_MAX_LEN);
    switch(req_type)
    {
        case HTTP_REQ_TYPE_MAN:
	    strcpy(s_dcd_commom_req_para.Req_Type, "man");
           //DCD_TRACE_LOW:"DCD_SetReqype is man"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1280_112_2_18_2_12_48_133,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_TTL:
	    strcpy(s_dcd_commom_req_para.Req_Type, "ttl");
           //DCD_TRACE_LOW:"DCD_SetReqype is ttl"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1285_112_2_18_2_12_48_134,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_SVR:
	    strcpy(s_dcd_commom_req_para.Req_Type, "svr");
           //DCD_TRACE_LOW:"DCD_SetReqype is svr"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1290_112_2_18_2_12_48_135,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_MC:
	    strcpy(s_dcd_commom_req_para.Req_Type, "mc");
           //DCD_TRACE_LOW:"DCD_SetReqype is mc"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1295_112_2_18_2_12_48_136,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_MR:
	    strcpy(s_dcd_commom_req_para.Req_Type, "mr");
           //DCD_TRACE_LOW:"DCD_SetReqype is mr"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1300_112_2_18_2_12_48_137,(uint8*)"");
	    break;			

        case HTTP_REQ_TYPE_START:
	    strcpy(s_dcd_commom_req_para.Req_Type, "start");
           //DCD_TRACE_LOW:"DCD_SetReqype is start"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1305_112_2_18_2_12_48_138,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_RETRY:
	    strcpy(s_dcd_commom_req_para.Req_Type, "retry");
           //DCD_TRACE_LOW:"DCD_SetReqype is retry"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1310_112_2_18_2_12_48_139,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_DATA:
	    strcpy(s_dcd_commom_req_para.Req_Type, "data");
           //DCD_TRACE_LOW:"DCD_SetReqype is data"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1315_112_2_18_2_12_48_140,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_DATA2:
	    strcpy(s_dcd_commom_req_para.Req_Type, "data2");
           //DCD_TRACE_LOW:"DCD_SetReqype is data2"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1320_112_2_18_2_12_48_141,(uint8*)"");
	    break;

        case HTTP_REQ_TYPE_DATA3:
	    strcpy(s_dcd_commom_req_para.Req_Type, "data3");
           //DCD_TRACE_LOW:"DCD_SetReqype is data3"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1325_112_2_18_2_12_48_142,(uint8*)"");
	    break;
        default:
           //DCD_TRACE_LOW:"DCD_SetReqype is unknown"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1328_112_2_18_2_12_48_143,(uint8*)"");
	    break;
    }
}

/*****************************************************************************/
//  Description : DCD_SetReqCommonPara
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
PUBLIC void MMIDCD_SetReqCommonPara(char* ua)
{
    //DCD_TRACE_LOW:"[MMIDCD]:MMIDCD_SetReqCommonPara enter "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1339_112_2_18_2_12_48_144,(uint8*)"");
	
    strcpy(s_dcd_commom_req_para.User_Agent,ua);         		
	
    strcpy(s_dcd_commom_req_para.Version,"1.0.0.0");		
}
	
/*****************************************************************************/
//Description : decode the data which is MIME type 
//Global resource dependence : 
//Author:
//Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_HEADER_CELL_PTR Http_XmlReqHeaderInit(
                                                    char* X_DP_RequestType_value_ptr,
                                                    char* User_Agent_value_ptr,
                                                    char* Version_value_ptr,
                                                    char* Connection_Value_ptr,   
    
                                                    char* X_DP_Global_Sync_Anchor_value_ptr,
                                                    char* X_DP_Sync_Anchor_value_ptr,
                                                    char* X_DP_Cell_Id_value_ptr,
    
                                                    char* Accept_Encoding_value_ptr
                                                   )
{
    
    uint8 valid_count = 0;
    DCD_HTTP_HEADER_CELL_PTR buff_ptr = PNULL;
    uint16  buff_size = 0;
	
    uint8 index = 0;    
    if(PNULL == X_DP_RequestType_value_ptr
        ||PNULL == User_Agent_value_ptr
        ||PNULL == Version_value_ptr)
    {
        return PNULL;
    }
	
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INIT(valid_count);
    
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( X_DP_RequestType_value_ptr        ,valid_count);
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( User_Agent_value_ptr              ,valid_count);   
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( Connection_Value_ptr              ,valid_count);   	
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( Version_value_ptr                 ,valid_count);   
        
    //buff_size = ( DCD_HTTP_FIELD_VALUE_MAX_LEN*2*valid_count);  
    buff_size = ( DCD_HTTP_FIELD_VALUE_MAX_LEN * valid_count);
    buff_ptr  = (DCD_HTTP_HEADER_CELL_PTR)MMIDCD_ALLOC( buff_size ); 
    if(PNULL == buff_ptr )
    {
        return PNULL;
    }
	
    SCI_MEMSET(buff_ptr,0,buff_size); 

    
   
    DCD_HTTP_FIELD_VALUE_SET_START( buff_ptr,index );
    
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, X_DP_RequestType_value_ptr );

    index++;
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, User_Agent_value_ptr );  

    index++;	
    //DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, "Connection",Connection_Value_ptr );  

    //index += 2;	
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, Version_value_ptr );
	
	return buff_ptr;   	                
}

/*****************************************************************************/
//Description : decode the data which is MIME type 
//Global resource dependence : 
//Author:
//Note: 
/*****************************************************************************/
LOCAL DCD_HTTP_HEADER_CELL_PTR Http_ContentReqHeaderInit(
                                                        char* X_DP_RequestType_value,
                                                        char* X_DP_Request_Id_value,        
                                                        char* User_Agent_value,
                                                        char* Connection_Value,        
                                                        char* Version_value,
                                                        char* X_DP_Cell_Id_value,   
                                                        char* Accept_Encoding_value
                                                       )
{

    uint8 valid_count = 0;
    DCD_HTTP_HEADER_CELL_PTR buff_ptr = PNULL;		
    uint16  buff_size = 0;
    uint8 index = 0; 
    
    if(PNULL == X_DP_RequestType_value 
        ||PNULL == X_DP_Request_Id_value
        ||PNULL == User_Agent_value
        ||PNULL == Version_value)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:Http_ContentReqHeaderInit ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1436_112_2_18_2_12_48_145,(uint8*)"");
        return PNULL;
    }   
	
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INIT(valid_count);
    
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( X_DP_RequestType_value        ,valid_count);
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( X_DP_Request_Id_value         ,valid_count);      
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( User_Agent_value              ,valid_count);   
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( Connection_Value              ,valid_count);  	
    DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC( Version_value                 ,valid_count);
	
    //buff_size = ( DCD_HTTP_FIELD_VALUE_MAX_LEN*2*(valid_count+1) ); 
    
    buff_size = ( DCD_HTTP_FIELD_VALUE_MAX_LEN * (valid_count+1) ); 
    buff_ptr  = (DCD_HTTP_HEADER_CELL_PTR)MMIDCD_ALLOC( buff_size ); 
    if(PNULL == buff_ptr )
    {
        return PNULL;
    }
    	       
    SCI_MEMSET(buff_ptr,0,buff_size); 
    DCD_HTTP_FIELD_VALUE_SET_START( buff_ptr,index );
    
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, X_DP_RequestType_value           );

    index++;	
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, X_DP_Request_Id_value            );  

    index++;	
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, User_Agent_value                 );  

    index++;	
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, Connection_Value                 );  
	
    index++;		
    DCD_HTTP_FIELD_VALUE_SET( buff_ptr,index, Version_value                    );
	
	
	return buff_ptr;    	               
}

#define MIME_HEAD_EXTRA_LEN  20
#define MIME_HEAD_LEN   ((strlen("Content-Type: ") + strlen("Content-Id: ") + strlen("Content-Length: ")) + MIME_HEAD_EXTRA_LEN  )
/*****************************************************************************/
//Description : decode the data which is MIME type 
//Global resource dependence : 
//Author:zhuo.wang
//Note: 
/*****************************************************************************/
LOCAL uint8*  MIMEDecodeByBuf(uint8* buf_ptr, uint32 *len_ptr)
{
    char *p = (char *)buf_ptr;
    char type[10];
    char id[20];
    char len[10];
    char *temp;
    uint32 total_len = 0;
    
    /* find string "--MultipartBoundary"*/
    if((PNULL==p) || (0 == (*len_ptr)))
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MIMEDecodeByBuf; buf_ptr or len is NULL!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1496_112_2_18_2_12_48_146,(uint8*)"");
        return PNULL; 
    }
	
	total_len = (*len_ptr);
	/* find string "Content-Type: "*/
	p = strstr(p, "Content-Type: ");
	/* RULL 1:"Content-Type: " can not be found, the file is invalid*/
	if(PNULL == p)
	{
		//SCI_TRACE_LOW:" [MMIDCD]:Content-Type: can not be found, the file is invalid"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1506_112_2_18_2_12_48_147,(uint8*)"");
		p = PNULL;
	}
	else  // 1
	{
		p = strstr(p, "image/");
		if(PNULL == p)
		{
			//SCI_TRACE_LOW:" [MMIDCD]:image: can not be found, the file is invalid"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1514_112_2_18_2_12_48_148,(uint8*)"");
			p = PNULL;
		} 
		else  // 2
		{
			p += (sizeof("image/") - 1);	
			temp = type;
			while(*p != '\r' )
			{
				*temp = *p;
				temp++;
				p++;
			}
			*temp = '\0';
			/* find string "Content-Id: "*/
			p = strstr(p, "Content-Id: ");
			if(PNULL == p)
			{
				//SCI_TRACE_LOW:" [MMIDCD]:Content-Id: can not be found, the file is invalid"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1532_112_2_18_2_12_48_149,(uint8*)"");
				p= PNULL;
			}
			else // 3
			{
				p += (sizeof("Content-Id: ") - 1);
				temp = id;
				while(*p != '\r')
				{
					*temp = *p;
					temp++;
					p++;
				}
				*temp = '\0';
				
				/* find string "Content-Length: "
				and get the binary data point and its length
				*/
				p = strstr(p, "Content-Length: ");
				if(PNULL == p)
				{
					//SCI_TRACE_LOW:" [MMIDCD]:Content-Length: can not be found, the file is invalid"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1553_112_2_18_2_12_49_150,(uint8*)"");
					p = PNULL;
				}
				else // 4
				{
					p += (sizeof("Content-Length: ") - 1);
					temp = len;
					while(*p != '\r')
					{
						*temp = *p;
						temp++;
						p++;
					}
					*temp = '\0';
					*len_ptr = atoi(len);
					
					/* RULL 2:file len < *len_ptr, the file is invalid*/	
					if( total_len < (MIME_HEAD_LEN + *len_ptr))
					{
						//SCI_TRACE_LOW:"RULL 2:file len < *len_ptr, the file is invalid"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_HTTP_1572_112_2_18_2_12_49_151,(uint8*)"");
						p = PNULL;
					}
					else //5
					{
					/*jump to the beginning of image data,
					because the binary ended by /r/n/r/n
						*/
						p += 4;									
					} // 5
				} // 4
			}// 3
		} // 2
	}// 1
	
	return (uint8 *)p;
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_ENABLE_DCD*/
