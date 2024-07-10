/*****************************************************************************
** File Name:      mmidcd_http.h                                                  *
** Author:                                                                   *
** Date:           10/2007                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2007        byte.guo       Create                                  *
******************************************************************************/
#ifndef _MMIDCD_HTTP_H
#define _MMIDCD_HTTP_H


#include "sci_types.h"
#include "sig_code.h"
#include "sfs.h"

#ifdef MMI_ENABLE_DCD
#include "http_api.h"
#include "in_message.h"
#include "mmidcd_xml_parse.h"
#include "mmidcd_position.h"
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
#define DCD_HTTP_DOWNLOAD_ARRAY_LEN		4
#define DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INIT(valid_value_count) {valid_value_count = 0;}

#define DCD_HTTP_REQ_URI_MAX_LEN					512//HTTP_URI_BUFFER_SIZE	
#define DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN		SFS_MAX_PATH
#define DCD_CTXT_ID_MAX_LEN 32
#define DCD_MAX_HEADER_NUM 50
#define DCD_CONTENT_TYPE_BUFFER_SIZE 128

#define DCD_HTTP_FIELD_VALUE_MAX_LEN	 MMIDCD_MAX_UA_LEN // 

#define DCD_CONTENT_ENCODING_BUFFER_SIZE 128

#define DCD_CLIENT_BUFFER_SIZE 8192

#define DCD_BUFFER_SIZE 512


typedef char					DCD_HTTP_HEADER_CELL[DCD_HTTP_FIELD_VALUE_MAX_LEN];
typedef DCD_HTTP_HEADER_CELL*	DCD_HTTP_HEADER_CELL_PTR;

#define DCD_HTTP_VALIID_FIELD_VALUE_COUNT_INC(field_value,valid_value_count)    \
    if( strlen(field_value) != 0 )                                              \
    {                                                                           \
        valid_value_count++;                                                    \
    }
    
    
#define DCD_HTTP_FIELD_VALUE_SET_START(str_array,index)		{index = 0;}  
    
#define DCD_HTTP_FIELD_VALUE_SET(str_array,index,field_value)        \
    if( strlen(field_value)!=0 )                                    \
    {                                                                           \
        strcpy( str_array[index],field_value );                               \
    }   
                                                             
    

typedef enum _DCD_ERROR_CODE
{
    DCD_ERROR_NULL = 0,
    DCD_ERROR_SIM_FAIL,         // sim 卡检查失败....
    DCD_ERROR_NOT_CMCC,
    DCD_ERROR_SET_CLOSE,        // DCD 设成了关闭....
    DCD_ERROR_FLY_MODE,         // 飞行模式
    DCD_ERROR_IS_BUSY,          // DCD出于非空闲状态...
    DCD_ERROR_XML_FORMAT=701,
    DCD_ERROR_NO_XML=702,
    DCD_ERROR_WRONG_DATA = 703,
    DCD_ERROR_DUMMY_DATA=704,
    DCD_ERROR_NO_MEMORY = 705,
    DCD_ERROR_INVALID_NETWORK = 706,
    DCD_ERROR_NETWORK_USING=707,
    DCD_ERROR_TIMEOUT = 708,
    DCD_ERROR_UDISK_USING,      // U盘正在使用中...    
    DCD_ERROR_UNKNOWN,    
    MDCD_ERROR_END
}MMIDCD_ERROR_CODE_E;

typedef enum _DCD_HTTP_REQ_TYPE_E
{
    HTTP_REQ_TYPE_UNUSE = 0,
        
    HTTP_REQ_TYPE_XML,	
    HTTP_REQ_TYPE_CONTENT,
    HTTP_REQ_TYPE_MIME,
    
    HTTP_REQ_TYPE_MAX,

}MMIDCD_HTTP_REQ_TYPE_E;

typedef enum _DCD_REQ_TYPE
{
    HTTP_REQ_TYPE_NULL, 
    HTTP_REQ_TYPE_MC,        
    HTTP_REQ_TYPE_MR,        
    HTTP_REQ_TYPE_START,        
    HTTP_REQ_TYPE_MAN, 
    HTTP_REQ_TYPE_TTL,
    HTTP_REQ_TYPE_SVR,
    HTTP_REQ_TYPE_RETRY,    
    HTTP_REQ_TYPE_DATA,
    HTTP_REQ_TYPE_DATA2,    
    HTTP_REQ_TYPE_DATA3
        
}MMIDCD_REQ_TYPE_E;


#define DCD_DEFAULT_HOMEPAGE		"http://dcd1.monternet.com"
#define DCD_DEFAULT_PROXY           "10.0.0.172"
#define DCD_DEFAULT_PROXY_PORT      80
#define DCD_DEFAULT_APN             "cmwap"
#define DCD_DEFAULT_USERNAME        "user"
#define DCD_DEFAULT_PASSWORD        "123456"
#define DCD_HELP_PART_URL           "/service/help/help.im"
#define DCD_ADD_CHANNEL_PART_URL    "/service/addchannel"
#define DCD_DEL_CHANNEL_PART_URL    "/service/deletechannel"

//MS00216980 cheney begin
// jun.hu 把它们从代码中拿到这里来作为宏
#define DCD_HTTP_REQUESTTYPE_STR   "X-DP-RequestType"
#define DCD_HTTP_REQUUSTID_STR       "X-DP-Request-Id"
#define DCD_HTTP_USERAGENT_STR      "User-Agent"
#define DCD_HTTP_VERSION_STR           "Version"
#define DCD_HTTP_CONNECTION_STR  "Connection"

#define DCD_APN_MAX_LEN             32
#define DCD_USER_AGENT_MAX_LEN      255
#define DCD_PROXY_MAX_LEN           66
#define DCD_USERNAME_MAX_LEN        66
#define DCD_PASSWORD_MAX_LEN        66
#define DCD_URL_MAX_LEN             255
#define DCD_REQ_TYPE_MAX_LEN        20
#define DCD_VERSION_MAX_LEN         21

typedef struct _DCD_REQ_COMMON_PARAMETER
{
    char Req_Type[DCD_REQ_TYPE_MAX_LEN];
    char User_Agent[DCD_USER_AGENT_MAX_LEN];
    char Version[DCD_REQ_TYPE_MAX_LEN];	
}DCD_REQ_COMMON_PARAMETER_T;

typedef struct _DCD_NETWORK_PARAMETER_T
{
    char URL[DCD_MAX_HREF_LEN];	
    char APN[DCD_APN_MAX_LEN];
    char Username[DCD_USERNAME_MAX_LEN];	
    char Password[DCD_PASSWORD_MAX_LEN];	
    char Proxy[DCD_PROXY_MAX_LEN];
    uint32 Port; 	 
}DCD_NETWORK_PARAMETER_T;


/*need some change to match http*/	
typedef struct _DCD_HTTP_REQ_T
{
    SIGNAL_VARS
    //HTTP_VERB httpverb;=VerbGet	
    uint32  type;
    char    ctxt_id[DCD_CTXT_ID_MAX_LEN];	
    char    uri[DCD_HTTP_REQ_URI_MAX_LEN];
    uint16  gb_full_file_name[DCD_HTTP_REQ_GB_FULL_FILE_NAME_MAX_LEN];	
    uint16  head_len_array[DCD_MAX_HEADER_NUM];
    
    DCD_HTTP_HEADER_CELL_PTR    ext_header;
    struct _DCD_HTTP_REQ_T*     next_ptr;

}DCD_HTTP_REQ_T;

typedef struct
{
    SIGNAL_VARS
    HTTP_CONTEXT_ID_T       context_id; //context id that http assigned for app
    HTTP_APP_MODULE_ID_T    module_id;  //module id of app
    HTTP_APP_PARAM_T        app_param;  //app param
    HTTP_ERROR_E            result;     //request result
}MMI_HTTP_INIT_CNF_SIG_T;


typedef struct
{
    SIGNAL_VARS
    HTTP_CONTEXT_ID_T   context_id;         //context id that http assigned for app
    HTTP_REQUEST_ID_T   request_id;         //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
}MMI_HTTP_REQ_ID_IND_SIG_T;

typedef struct
{
    SIGNAL_VARS
    HTTP_CONTEXT_ID_T       context_id;         //context id that http assigned for app
    HTTP_REQUEST_ID_T       request_id;         //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T     app_instance;       //the app instance
    HTTP_ERROR_E            result;             //request result
    HTTP_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
    
}MMI_HTTP_GET_DATA_CNF_SIG_T;

typedef struct
{
    SIGNAL_VARS
    uint32                     context_id;
    uint16                     module_id;
    char                       buffer[DCD_CLIENT_BUFFER_SIZE/8];
    uint32                     http_status;           // the HTTP status code (200 401 407 act')
    uint32                     http_content_len;
    char                       location_buf[DCD_HTTP_REQ_URI_MAX_LEN];
    char                       content_type_buf[DCD_CONTENT_TYPE_BUFFER_SIZE];
    char                       content_encoding_buf[DCD_CONTENT_ENCODING_BUFFER_SIZE];
}MMI_HTTP_HEADER_IND_SIG_T;

typedef struct 
{
    SIGNAL_VARS
    uint32              context_id;
    uint16              module_id;
    uint32              current_len;
    uint32              content_len;
    char                buffer[DCD_CLIENT_BUFFER_SIZE];

}MMI_HTTP_CONTENT_IND_SIG_T;

typedef struct
{
    SIGNAL_VARS
    HTTP_CONTEXT_ID_T   context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T   request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;   //the app instance
    HTTP_ERROR_E        result;         //http error	
}MMI_HTTP_ERROR_SIGNAL_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  close http session
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_HttpSessionClose(uint32 session_id);
/*****************************************************************************/
//  Description : DCD_SetReqype
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
PUBLIC void MMIDCD_SetReqType(MMIDCD_REQ_TYPE_E req_type);

/*****************************************************************************/
//  Description : DCD_SetReqCommonPara
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
PUBLIC void MMIDCD_SetReqCommonPara( char* ua);
/*****************************************************************************/
//  Description :  when pasering, build the download req list
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_DownloadContentCallback(DCD_CONTENT_T* content_ptr);
/*****************************************************************************/
//  Description :  handle msg when there exist an error
//  Global resource dependence :
//  Author: 
//  Note: modified by @jun.hu, 2009/8/13
/*****************************************************************************/
PUBLIC MMIDCD_ERROR_CODE_E Http_ErrorHandler(uint32 errorcode);
/*****************************************************************************/
//  Description :  get the session id
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 DCD_HttpGetCurSessionId(void);
/*****************************************************************************/
//  Description :  set the session id
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_HttpSetCurSessionId(uint32 id);

/*****************************************************************************/
//  Description :  init http and the req
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_DownloadXMLPre(uint16 module_id);

/*****************************************************************************/
//  Description : DCD_SetReqype
//  Global resource dependence :
//  Author://没写到文档中
//  Note: app interface
/*****************************************************************************/
PUBLIC void Http_FreeXmlReqList(void);

/*****************************************************************************/
//  Description :  send msg for downloading XML
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_DownloadXML(uint32 session_id);
/*****************************************************************************/
//  Description :  set the session id
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_HttpSetCurRequtestId(uint32 id);
/*****************************************************************************/
//  Description :  Http_ContentDownloadPre
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_ContentDownloadPre(void);
/*****************************************************************************/
//  Description :  DCD_HttpGetCurRequtestId
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 DCD_HttpGetCurRequtestId(void);
/*****************************************************************************/
//  Description :  DCD_HttpGetCurNetId
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 DCD_HttpGetCurNetId(void);
/*****************************************************************************/
//  Description :  DCD_HttpGetCurNetId
//  Global resource dependence :
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void DCD_HttpSetCurNetId(uint32 id);
/*****************************************************************************/
//  Description :  the downloaded dcd content is of MIME type, should be interpreted
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
#ifdef DCD_BUFFER_INTERPRETE
PUBLIC BOOLEAN Http_ContentInterpreteProcess(uint8 *data_ptr,uint32 data_len);
#else
PUBLIC BOOLEAN Http_ContentInterpreteProcess(void);
#endif

/*****************************************************************************/
//  Description :  get a req from req list and send the req
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void Http_ContentDownloadProcess(void);
#ifdef   __cplusplus
    }
#endif
    
#endif /*#ifdef MMI_ENABLE_DCD*/
#endif /*#define _MMIDCD_HTTP_H*/
