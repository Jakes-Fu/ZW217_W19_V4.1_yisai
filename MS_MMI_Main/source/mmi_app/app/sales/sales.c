#ifdef  SALES_SUPPORT
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "lcd_cfg.h"
#include "mmk_app.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "http_api.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"

#ifdef WRE_SUPPORT
#include "Mmiwre_export.h"
#endif

#include "gui_ucs2b_converter.h"




// The routine MD5Init initializes the message-digest context
 //  mdContext. All fields are set to zero.
/****************************************************************/
LOCAL void SALE_MD5Init(MD5_CTX *context);
/****************************************************************/
// Decodes input (unsigned char) into output (UINT4). Assumes len is
//  a multiple of 4.
/****************************************************************/
LOCAL void SALE_Decode(unsigned long int *output,
                                unsigned char *input,
                                unsigned int len
                               );
/****************************************************************/
// MD5 basic UPDATE. UPDATE state based on block.
/****************************************************************/
LOCAL void SALE_MD5Update(MD5_CTX *context,                                        /* context */
                                      char *input,                               /* input block */
                                      unsigned int inputLen                     /* length of input block */
                                      );
/****************************************************************/
// MD5 basic FINAL. UPDATE state based on block.
/****************************************************************/
LOCAL void SALE_MD5Final( char digest[16],                         /* message digest */
                                   MD5_CTX *context                                    /* context */
                                 );
LOCAL void Sales_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);
LOCAL wchar* Sales_WritePostBody(wchar* tmp_ptr);
LOCAL int GetPhoneInfoStr(uint8* phone_buf, uint32 buf_len);
LOCAL uint32 GetTimestamp(uint8 g_timezone);

#define SALES_ACCEPT_STRING "text/html, application/xml;q=0.9, application/java-archive, text/vnd.sun.j2me.app-descriptor, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1" //表示接受任意格式的文本
#define SALES_ACCEPT_LANGUAGE_STRING  "zh-cn, en" //表示接受中文英文
#define SALES_USER_AGENT_STRING       "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define SALES_ACCEPT_CHARSET_STRING   "utf-8, utf-16, iso-8859-1, iso-8859-5, iso-10646-ucs-2, GB2312, windows-1252, us-ascii"
#define SALES_HTTP_URI_STRING         "http://sales_statistics.52wre.com/sales_statistics?api_key=sales_log&sign=%s&sign_method=md5&time=%d&v=1.0"//"http://www.baidu.com"
#define SALES_HTTP_URI_STRING_SECRET		"32b8095886f1a67c62a5882bb935b93b"
#define SALES_HTTP_URI_STRING_SECRET_FOR_MD5		"api_keysales_logsign_methodmd5time%dv1.0"

#define HTTP_SALES_CONTENT_TYPE "application/x-www-form-urlencoded"

//#define SALES_REQUEST_ID_DEFAULT_VALUE 4026531839
#define HTTP_SIGNAL_ANALYTIC_EX(param,signal,app_param,http_context_ptr,app_module_id,sig_type) do{\
	    Sales_GetSignalStruct(param, &signal, sizeof(sig_type));\
	    if(signal.module_id != app_module_id)\
	    {\
	    	return MMI_RESULT_FALSE;\
	    }\
	    app_param = signal.app_param;\
	    http_context_ptr->context_id   = signal.context_id;\
	    http_context_ptr->app_instance = 0;\
	    http_context_ptr->request_id = 0;\
	    http_context_ptr->module_id    = signal.module_id;\
}while(0)\

#define HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,sig_type) do{\
	Sales_GetSignalStruct(param, &signal, sizeof(sig_type));\
	if(signal.context_id != http_context_ptr->context_id)\
	{\
	    	return MMI_RESULT_FALSE;\
	}\
}while(0)\

typedef enum
{
	SALES_NV_FLAG = MMI_MODULE_SALES << 16,
	SALES_NV_MAX
}SALES_NV_ENUM;

typedef struct __Sales
{
	BOOLEAN flags;
	MN_DUAL_SYS_E sim_sys;
	uint32 net_id;
}Sales;
typedef struct __SALES_HTTP_CONTEXT
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
}SALES_HTTP_CONTEXT;

LOCAL MMI_RESULT_E HandleHttpSalesDataInd(SALES_HTTP_CONTEXT * http_context_ptr, DPARAM param);

typedef enum
{
    S_KEYPAD_TYPICAL = 0,     //T9 keypad
    S_KEYPAD_QWERTY,          //all keypad
    S_KEYPAD_COMPACT,         //not be used at present
    S_FOUR_KEY_UNIVERSE,      //andoid style
    S_KEYPAD_CUSTOMIZE        //customize by user,not be used at present
}SALE_KEYBOARD;
typedef struct
{
    uint8  sec;  
    uint8  min;
    uint16  hour;
}SALE_MMIENG_TIME_T;
//typedef struct 
//{
//    MMIPDP_LINK_INFO_T          link_arr[MMIPDP_MAX_LINK_NUM];
//    MMIPDP_LIST_HANDLE          pending_list;
//    BOOLEAN                     is_release_all;
//    MMIPDP_RELEASE_ALL_CALLBACK release_all_cb;
//}MMIPDP_MANAGER_INSTANCE_T;



extern PHONE_SERVICE_STATUS_T  g_service_status[MMI_DUAL_SYS_MAX];
//extern MMIPDP_MANAGER_INSTANCE_T g_mmipdp_manager_instance;

const wchar* keyboard_str[]=
{
	L"S_KEYPAD_TYPICAL",
	L"S_KEYPAD_QWERTY",
	L"S_KEYPAD_COMPACT",
	L"S_FOUR_KEY_UNIVERSE",
	L"S_KEYPAD_CUSTOMIZE",	
};
const wchar* default_language_str[]=
{
	L"ENGLISH",
	L"SIMP_CHINESE",
	L"TRAD_CHINESE",	
	L"ARABIC",
	L"FRENCH",
	L"HINDI ",
	L"HUNGARIAN",
	L"INDONESIAN",
	L"MALAY",
	L"PORTUGUESE",
	L"RUSSIAN",
	L"SPANISH",
	L"TAGALOG",
	L"THAI",	
	L"VIETNAMESE",
	L"URDU",
	L"ITALIAN",
	L"PERSIAN",
	L"TURKISH",
	L"GERMAN",
	L"GREEK",
	L"HEBREW",
	L"BENGAL",
	L"CZECH",
	L"SLOVENIAN",
	L"ROMANIAN",
	L"TELUGU",
	L"MARATHI",
	L"TAMIL",
	L"GUJARATI",
	L"KANNADA",
	L"MALAYALAM",
	L"ORIYA",
	L"PUNJABI",
	L"AFRIKAANS",
	L"ALBANIAN",
	L"ARMENIAN",
	L"AZERBAIJANI",
	L"BASQUE",
	L"BULGARIAN",
	L"CATALAN",
	L"CROATIAN",
	L"DANISH",
	L"DUTCH",
	L"ESTONIAN",
	L"FILIPINO",
	L"FINNISH",
	L"GALICIAN",
	L"GEORGIAN",
	L"HAUSA",
	L"ICELANDIC",
	L"IGBO",
	L"IRISH",
	L"KAZAKH",
	L"LATVIAN",
	L"LITHUANIAN",
	L"MACEDONIAN",
	L"MOLDOVAN",
	L"NORWEGIAN",
	L"POLISH",
	L"SERBIAN",
	L"SESOTHO",
	L"SLOVAK",
	L"SWEDISH",
	L"UKRAINIAN",
	L"YORUBA"	
};
/*the length of alarm nv*/
const uint16 sales_nv_len[] =
{
    sizeof(BOOLEAN)
};

MMI_APPLICATION_T           g_sales_app       = {0};

LOCAL SALES_HTTP_CONTEXT   s_sales_http_context      = {0};
LOCAL Sales s_sales={0};
LOCAL const wchar* s_sales_info_arr[] = 
{	  
	L"imsi=\0",
	L"scaddr=\0",//短信中心号码
	L"mcc=\0",//   2
	L"mnc=\0",// 3
	L"lac=\0",// 4
	L"cellid=\0",// 5
	L"ip=\0",// 6
	L"versioninfo=\0",//7
	#ifdef WRE_SUPPORT
	L"wreversioninfo=\0",// 8
	#endif		
	L"lcdsize=\0",// 9
	L"keytype=\0",// 10
	#ifdef BLUETOOTH_SUPPORT
	L"btinfo=\0",// 11
	#endif
	#ifdef MMI_WIFI_SUPPORT
	L"wifiinfo=\0",// 12
	#endif
	#ifdef FM_SUPPORT
    	L"fminfo=\0",// 13
    	#endif
    	#ifdef ATV_SUPPORT
    	L"atvinfo=\0",//14
    	#endif
    	#ifdef MMI_GPS_SUPPORT
    	L"gpsinfo=\0",//15
    	#endif
    	#ifdef MOTION_SENSOR_TYPE//GSENSOR
    	L"gsensorinfo=\0",//16
    	#endif
    	#ifdef CAMERA_SUPPORT//CAMERA
    	L"dcinfo=\0",// 17
    	#endif
    	L"phoneinfo=\0",
    	#ifdef WRE_WRESTORE_SUPPORT//store info
    	L"storeinfo=\0",
    	#endif
    	L"defaultlanguage=\0"
};
LOCAL const MMIPDP_NOTIFY_EVENT_E s_sales_pdp_event[MMIPDP_NOTIFY_EVENT_MAX] = {
    MMIPDP_NOTIFY_EVENT_ACTIVED,
    MMIPDP_NOTIFY_EVENT_DEACTIVED,
    MMIPDP_NOTIFY_EVENT_ATTACH,
    MMIPDP_NOTIFY_EVENT_DETTACH
};


LOCAL void StartSalesHttpRequest()
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;
#ifdef HTTP_SUPPORT
    HTTP_InitRequest(MMI_MODULE_SALES, 1, init_param_ptr);
#endif
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}
void Sales_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size ));
    }
}
LOCAL void HandleSalesPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
	if(PNULL == msg_ptr)
    	{
        	return;
    	}

    	switch(msg_ptr->msg_id) 
    	{
	    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
	    	{
	    		if(MMIPDP_RESULT_SUCC == msg_ptr->result)
	    		{
				s_sales.net_id = msg_ptr->nsapi;
	    			StartSalesHttpRequest();
	    		}
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
	    		else
	    		{
	    			MMIAPIPDP_Deactive(MMI_MODULE_SALES);
	    		}
#endif
	    	}
	    	break;	        
	    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功
	    		s_sales.net_id = 0;
	        break;
	        
	    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活
	    		s_sales.net_id = 0;
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
			MMIAPIPDP_Deactive(MMI_MODULE_SALES);
#endif
	        break;	        
	    default:
	        break;
	}

}
BOOLEAN SalesPDP()
{
	MMIPDP_ACTIVE_INFO_T    active_info     = {0};
	BOOLEAN                 return_val      = FALSE;
	MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    	char*   apn_str = PNULL;
    	uint8   index   = 0;
       {
       	int i = 0;
       	MN_DUAL_SYS_E sim_sys = s_sales.sim_sys;
 			
	    	linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, 0);

	    	if(PNULL != linksetting && 0 != linksetting->apn_len)
	    	{
	        	apn_str = (char*)linksetting->apn;
	    	}		
		active_info.app_handler         = MMI_MODULE_SALES;
		active_info.dual_sys            = sim_sys;
		active_info.apn_ptr             = apn_str;
		active_info.user_name_ptr       = NULL;
		active_info.psw_ptr             = NULL;
		active_info.priority            = 3;
		active_info.ps_service_rat      = MN_UNSPECIFIED;
		active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
		active_info.handle_msg_callback = HandleSalesPdpMsg;
		active_info.ps_service_type = BROWSER_E;
		active_info.storage = MN_GPRS_STORAGE_ALL;

#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
		if(MMIAPIPDP_Active(&active_info))
		{
			return_val = TRUE;
		}
#endif
	}
	return return_val;
}
LOCAL MMI_RESULT_E HandleSalesHttpPostCnf(SALES_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef HTTP_SUPPORT
    HTTP_POST_CNF_SIG_T signal = {0};
    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_POST_CNF_SIG_T);
    SCI_TRACE_LOW("[sales]:HandleHttpMsg receive HTTP_SIG_POST_CNF");
    HTTP_CloseRequest(http_context_ptr->context_id);   
#endif
    return result;
}
LOCAL MMI_RESULT_E HandleSalesHttpPostRequest(SALES_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	int file_read = 0;
	int len = 0;
	uint32 tickcount = 0;
	
	HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr,MMI_MODULE_SALES,HTTP_INIT_CNF_SIG_T);
	SCI_TRACE_LOW("[sales]:HandleHttpMsg receive HTTP_SIG_INIT_CNF");
	post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));
	
	ptr = SCI_ALLOC_APPZ(10*1024);
	if(ptr == NULL) return result;
	
	tmp_ptr = SCI_ALLOC_APPZ(10*1024);
	if(tmp_ptr == NULL) return result;
	SCI_MEMSET(tmp_ptr,0,10*1024);
	SCI_MEMSET(ptr,0,10*1024);
	if(post_param_ptr == NULL || http_context_ptr == NULL)
	{
		return MMI_RESULT_FALSE;
	}	
	memset(post_param_ptr,0,sizeof(post_param_ptr));
	
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;

	len=strlen(SALES_ACCEPT_STRING);
	post_param_ptr->accept.accept_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->accept.accept_ptr,0,len+1);
	strcpy(post_param_ptr->accept.accept_ptr,SALES_ACCEPT_STRING);

	len=strlen(SALES_ACCEPT_LANGUAGE_STRING);
	post_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->accept_language.accept_language_ptr,0,len+1);
	strcpy(post_param_ptr->accept_language.accept_language_ptr,SALES_ACCEPT_LANGUAGE_STRING);	

	len=strlen(SALES_USER_AGENT_STRING);
	post_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC(len+1);
	memset(post_param_ptr->user_agent.user_agent_ptr,0,len+1);
	strcpy(post_param_ptr->user_agent.user_agent_ptr,SALES_USER_AGENT_STRING);	


	/***************数据体*****************/
	len=strlen(HTTP_SALES_CONTENT_TYPE);
	post_param_ptr->post_body.body_type_ptr= SCI_ALLOC(len+1);
	memset(post_param_ptr->post_body.body_type_ptr,0,len+1);
	strcpy(post_param_ptr->post_body.body_type_ptr,HTTP_SALES_CONTENT_TYPE);
	Sales_WritePostBody(ptr);
	GUI_WstrToUTF8(tmp_ptr,10*1024,ptr,10*1024);
	post_param_ptr->post_body.is_use_file = FALSE;
	post_param_ptr->post_body.u.post_buffer.is_copied_by_http= TRUE;
	post_param_ptr->post_body.u.post_buffer.buffer_len=strlen(tmp_ptr);
	post_param_ptr->post_body.u.post_buffer.buffer_ptr=tmp_ptr;

	/********************/
	//添加sign加密
	{
		char* tmp_secret = NULL;
		char* tmp_Signtxt = NULL;
		char secret[64]={0};
		MD5_CTX context={0};
		uint8   Signtxt[16]={0};
		char signtxt_array[3]={0};
		char signtxt_str[36]={0};
		uint8 i = 0;
		
		strcpy(secret,SALES_HTTP_URI_STRING_SECRET);
		tickcount = GetTimestamp(8);
		tmp_Signtxt=SCI_ALLOC_APPZ(1024);
		tmp_secret=SCI_ALLOC_APPZ(1024);
		SCI_MEMSET(tmp_secret,0,1024);
		SCI_MEMSET(tmp_Signtxt,0,1024);
		sprintf(tmp_secret,SALES_HTTP_URI_STRING_SECRET_FOR_MD5,tickcount);
    		strcpy(tmp_Signtxt,secret);
    		strcat(tmp_Signtxt,tmp_secret);		
		memset(tmp_secret,0,1024);

		i = strlen(tmp_Signtxt);
		SALE_MD5Init (&context);
		SALE_MD5Update (&context, (uint8*)tmp_Signtxt, i);
		SALE_MD5Final (Signtxt, &context);
    		for(i = 0 ; i <16;i ++)
    		{
    			sprintf(signtxt_array,"%02X",Signtxt[i]);
    			memcpy(signtxt_str+2*i,signtxt_array,2); 				
    		}
    		//拼接URL
		SCI_MEMSET(tmp_Signtxt,0,1024);
		sprintf(tmp_Signtxt,SALES_HTTP_URI_STRING,signtxt_str,tickcount);
		
		len=strlen(tmp_Signtxt);
		post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(post_param_ptr->uri.uri_ptr,0,len+1);
		strcpy(post_param_ptr->uri.uri_ptr,tmp_Signtxt);

		SCI_Free(tmp_Signtxt);
		SCI_Free(tmp_secret);
	}
#ifdef HTTP_SUPPORT
	if(!HTTP_PostRequest(http_context_ptr->context_id,post_param_ptr, http_context_ptr->app_instance))
	{
		result = MMI_RESULT_TRUE;
	}
#endif
	SCI_Free(post_param_ptr);
	SCI_Free(tmp_ptr);	
	SCI_Free(ptr);
	return	result;	
}
LOCAL MMI_RESULT_E HandleSaleHttpRequestIdInd(SALES_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
	HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)param;
   //当request_id为BILLING_REQUEST_ID_DEFAULT_VALUE时,则没被初始化过,属于本模块的消息
    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_REQUEST_ID_IND_SIG_T);
	SCI_TRACE_LOW("[sales]:HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND");
	SCI_TRACE_LOW("[sales]:HandleHttpMsg receive http_context_ptr->context_id:%d",http_context_ptr->context_id);
    http_context_ptr->context_id = signal.context_id;
    http_context_ptr->app_instance = signal.app_instance;
    http_context_ptr->request_id = signal.request_id;
    SCI_TRACE_LOW("[sales]:HandleSaleHttpRequestIdInd request_id:%ld,http_context_ptr->context_id:%d",signal.request_id,http_context_ptr->context_id);
    result = MMI_RESULT_TRUE;

    return result;
}
LOCAL MMI_RESULT_E HandleSalesHttpCloseCnf(SALES_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    BOOLEAN nv_value = 1;
    Sales_GetSignalStruct(param, &signal, sizeof(HTTP_CLOSE_CNF_SIG_T));
    if(http_context_ptr->context_id == signal.context_id)
    {
    SCI_TRACE_LOW("[sales]:HandleHttpMsg receive HTTP_SIG_CLOSE_CNF");
    	http_context_ptr->context_id = 0;
	    http_context_ptr->request_id = 0;
		result = MMI_RESULT_TRUE;
       MMINV_WRITE(SALES_NV_FLAG,&nv_value);
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
	MMIAPIPDP_Deactive(MMI_MODULE_SALES);
#endif	
}
    return result;
}
LOCAL MMI_RESULT_E HandleSalesHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	//当result为true时，后续注册的app不需要再响应http的消息
    	MMI_RESULT_E result = MMI_RESULT_FALSE;
    	void    *original_param = (void*)((uint32)param);
    	SALES_HTTP_CONTEXT *http_context_ptr = &s_sales_http_context;
	//第一次发送的消息比较特殊
	if(msg_id == HTTP_SIG_INIT_CNF)
	{
	 	//http协议服务已经准备好，开始连接服务器
	      //初始化要发送的数据,get or post
	      HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
	      if(cnf->module_id != MMI_MODULE_SALES || 1 != cnf->app_param)
            {
                return MMI_RESULT_FALSE;
            }
	      result = HandleSalesHttpPostRequest(http_context_ptr, param);
	      return result;//后续app不用再响应
	 }
	 //第一次请求后，http分配给app的id
	 if(msg_id == HTTP_SIG_REQUEST_ID_IND)
	 {
	      result = HandleSaleHttpRequestIdInd(http_context_ptr, param);
	      return result;
	 }	 
	 switch (msg_id)
	 {
	 case HTTP_SIG_HEADER_IND:
	 	{
#ifdef HTTP_SUPPORT
	 		HTTP_HEADER_IND_SIG_T signal = {0};
	 		HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_HEADER_IND_SIG_T);
	 		SCI_TRACE_LOW("[sales]:HandleHttpMsg receive HTTP_SIG_HEADER_IND");
	 		SCI_TRACE_LOW("[sales]:response_code:%d",signal.rsp_header_info.response_code);
	 		SCI_TRACE_LOW("[sales]:context_id:%ld,app_instance:%ld,request_id:%ld",
	 						http_context_ptr->context_id,http_context_ptr->app_instance,http_context_ptr->request_id);
	 		if(signal.rsp_header_info.response_code == 200)
	 		{	 			
	 			result=HTTP_HeaderResponse(http_context_ptr->context_id,http_context_ptr->app_instance,
	 								http_context_ptr->request_id,HTTP_DATA_TRANS_STYLE_BUFFER,PNULL,0);
	 		}
#endif
	 		//return MMI_RESULT_TRUE;
	 		SCI_TRACE_LOW("[sales]:result:%d",result);
	 		result = MMI_RESULT_TRUE;
	 	}
	 	break;
	 case HTTP_SIG_DATA_IND:
	 	{
	 		//HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)original_param;
	             result=HandleHttpSalesDataInd(http_context_ptr,param);
	             //return result;
	 	}
	 	break;
	 case HTTP_SIG_POST_CNF:
	 	result =HandleSalesHttpPostCnf(http_context_ptr,param);
	 	//return result;
	 	break;
   	 case HTTP_SIG_CLOSE_CNF://HTTP_SIG_CLOSE_REQ消息的回复,告知app,http服务关闭
        	result = HandleSalesHttpCloseCnf(http_context_ptr, param);
        	break;	 	
     default:
     	break;
	 }
	 return result;
}
LOCAL void HandlePdpSalesTimer(uint8 timer_id,uint32 param)
{
	 int i=0;
	 //for(i=0;i< MMIPDP_MAX_LINK_NUM;i++)
	 //{
	      	//if(g_mmipdp_manager_instance.link_arr[i].state == MMIPDP_LINK_STATE_ACTIVED)
	       //{
	       	//还在上网,不做任何处理
	       	//return;
	       //}
	 //}   
	 if(MMIAPIPDP_GetPdpLinkSIM() != MMI_DUAL_SYS_MAX)
	 {
	 	return;
	 }
	 MMK_StopTimer(timer_id);
	 SalesPDP();
}
LOCAL MMI_RESULT_E HandleSalesMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_FALSE;
	if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
	{
		result = HandleSalesHttpMsg(app_ptr,msg_id,param);		
	}
	return result;
}
LOCAL uint16 Sales_GetShowVersionInfo(wchar* dst)
{
	    wchar        unicode_cr = 0x0d;
	    MMI_STRING_T    cr_s = {0};
	    //MMI_STRING_T    text_s = {0};
	    uint16           uint16_str[MMIENG_3RDPARTYVER_TEXT_LEN + 1] = {0};
	    uint16          uint16_str_len = 0;
	    char*          temp_ptr = PNULL;
	    uint16          temp_len = 0;
	    
	    cr_s.wstr_ptr = &unicode_cr;
	    cr_s.wstr_len = 1;

	//Software V:  
	    temp_ptr = "Software V:";
	    temp_len = SCI_STRLEN((char*)temp_ptr);       
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef _WIN32
	    temp_ptr = VERSION_GetInfo(PLATFORM_VERSION);   /*lint !e605*/
#else
	    temp_ptr = "Platform Version:  MOCOR_W12.23_Debug";
#endif	
	    temp_len = SCI_STRLEN((char*)temp_ptr);
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行         

	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	//Hardware V:
	    temp_ptr = "Hardware V:";
	    temp_len = SCI_STRLEN((char*)temp_ptr);       
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef _WIN32
	    temp_ptr = VERSION_GetInfo(HW_VERSION);     /*lint !e605*/
#else
	//    temp_ptr = "SP7100A-V1.0.1-B7-M6416 SP7100A-V1.0.1-B7-M6416";	// this is a example for simulator
	    temp_ptr = "HW Version:        SC6530";
#endif	
	    temp_len = SCI_STRLEN((char*)temp_ptr);
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行         

	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

	 //Base V:
	    temp_ptr = "Base V:";
	    temp_len = SCI_STRLEN((char*)temp_ptr);       
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef _WIN32
#ifdef PLATFORM_SC6800H
	    temp_ptr = SCI_GetBaseVersionInfo();
#else    
	    temp_ptr = VERSION_GetInfo(BASE_VERSION);     /*lint !e605*/
#endif
#else
	//    temp_ptr = "BASE_1.0008";	// this is a example for simulator
	    temp_ptr = "BASE Version:     	 BASE_W09.23";
#endif	
	    temp_len = SCI_STRLEN((char*)temp_ptr);
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行    
	    
	    
	 //Build Time:
	 	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行  
	    temp_ptr = "Build Version:";
	    temp_len = SCI_STRLEN((char*)temp_ptr);       
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

#ifndef _WIN32
	    temp_ptr = VERSION_GetInfo(BUILD_TIME);     /*lint !e605*/
#else
	    temp_ptr = "2012-05-23";
#endif	
	    temp_len = SCI_STRLEN((char*)temp_ptr);
	    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len );
	    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行    

	    MMIAPICOM_Wstrcat(dst,uint16_str);
	    return uint16_str_len;
}
//Big_endian的，则返回0；若是Little_endian的，则返回1
LOCAL int checkCPU( )
{
    {
           union w
           {  
                  int a;
                  char b;
           } c;
           c.a = 1;
           return(c.b ==1);
    }
}

LOCAL wchar* Sales_WritePostBody(wchar* ptr)
{
	wchar* tmp_ptr = ptr;
	wchar* tmp_bt= L"TRUE";

	if(tmp_ptr != NULL)
	{
		int i = 0;
		MMIAPICOM_Wstrcpy(tmp_ptr,s_sales_info_arr[i]);// 0
		{
			char imsi_str[(MN_MAX_IMSI_ARR_LEN << 1) + 1]={0};
			wchar tmp_imsi_val_ex[(MN_MAX_IMSI_ARR_LEN << 1) + 1]={0};
			MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(s_sales.sim_sys);

			MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, imsi_str);
			MMIAPICOM_StrToWstr(imsi_str, tmp_imsi_val_ex);
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_imsi_val_ex);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 1
		{
			wchar tmp_sc_val[MMISMS_STRNUM_MAX_LEN+2]={0};
			uint8 tmp_sc_len = 0;
			uint8 tmp_sc_num[MMISMS_STRNUM_MAX_LEN + 2] = {0};
			MMISMS_AppGetSCAddr(s_sales.sim_sys,tmp_sc_num,&tmp_sc_len,MMISMS_STRNUM_MAX_LEN+2);
			MMIAPICOM_StrToWstr(tmp_sc_num,tmp_sc_val);
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_sc_val);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 2
		{
			uint8 tmp_mcc[30]={0};
			wchar w_tmp_mcc[30]={0};
			MMIAPICOM_Int2Str(g_service_status[s_sales.sim_sys].plmn.mcc,tmp_mcc,30);
			MMIAPICOM_StrToWstr(tmp_mcc, w_tmp_mcc);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_mcc);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 3
		{
			uint8 tmp_mnc[30]={0};
			wchar w_tmp_mnc[30]={0};
			MMIAPICOM_Int2Str(g_service_status[s_sales.sim_sys].plmn.mnc,tmp_mnc,30);
			MMIAPICOM_StrToWstr(tmp_mnc, w_tmp_mnc);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_mnc);
		}	
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 4
		{
			uint8 tmp_lac[30]={0};
			wchar w_tmp_lac[30]={0};
			MMIAPICOM_Int2Str(g_service_status[s_sales.sim_sys].lac,tmp_lac,30);
			MMIAPICOM_StrToWstr(tmp_lac, w_tmp_lac);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_lac);
		}	
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 5
		{
			uint8 tmp_cell_id[30]={0};
			wchar w_tmp_cell_id[30]={0};
			MMIAPICOM_Int2Str(g_service_status[s_sales.sim_sys].cell_id,tmp_cell_id,30);
			MMIAPICOM_StrToWstr(tmp_cell_id, w_tmp_cell_id);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_cell_id);
		}	
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 6
		{
			uint8 tmp_ip[33]={0};
			uint8 tmp_tmp_ip[33]={0};
			wchar w_tmp_ip[33]={0};
			int tmp_i = 0,j=0;
#ifdef TCPIP_SUPPORT            
			MMIAPICOM_Int2Str(sci_gethostaddress_ext(s_sales.net_id),tmp_ip,32);
#endif
			j=strlen(tmp_ip);
			if(checkCPU() == 1)
			{
				for(tmp_i=0;tmp_i<j;tmp_i++)
				{
					tmp_tmp_ip[tmp_i] = tmp_ip[j-tmp_i-1];
				}			
			}		
			MMIAPICOM_StrToWstr(tmp_tmp_ip, w_tmp_ip);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_ip);
		}	
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 7
		{
			Sales_GetShowVersionInfo(tmp_ptr);
		}	
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		#ifdef WRE_SUPPORT
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 8
		{
			wchar w_tmp_wre[MMIENG_WRE_VER_MAX_LEN]={0};
			char   * str_ptr    = PNULL;
			uint16          str_len = 0;
			uint16 uint16_str_len=0;
			 str_ptr = MMIWRE_GetVersionInfo();
		        MMIAPICOM_StrToWstr(str_ptr, w_tmp_wre);
		        MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_wre);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 9
		{
			uint8 tmp_size_width[5]={0};
			wchar w_tmp_size_width[5]={0};
			uint8 tmp_size_height[5]={0};
			wchar w_tmp_size_height[5]={0};
			MMIAPICOM_Int2Str(LCD_WIDTH,tmp_size_width,5);
			MMIAPICOM_Int2Str(LCD_HEIGHT,tmp_size_height,5);
			MMIAPICOM_StrToWstr(tmp_size_width, w_tmp_size_width);
			MMIAPICOM_StrToWstr(tmp_size_height, w_tmp_size_height);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_size_width);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_size_height);			
		}	
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 10
		{
			uint8 tmp_key_type[5]={0};
			wchar w_tmp_key_type[5]={0};
		#if defined KEYPAD_TYPE_TYPICAL_KEYPAD
	            //MMIAPICOM_Int2Str(KEYPAD_TYPICAL,tmp_key_type,5);
	            MMIAPICOM_Wstrcat(tmp_ptr,keyboard_str[S_KEYPAD_TYPICAL]);
		#elif defined KEYPAD_TYPE_QWERTY_KEYPAD 
	            //MMIAPICOM_Int2Str(KEYPAD_QWERTY,tmp_key_type,5);
	            MMIAPICOM_Wstrcat(tmp_ptr,keyboard_str[S_KEYPAD_QWERTY]);	            
		#elif defined KEYPAD_TYPE_COMPACT_KEYPAD 
	            //MMIAPICOM_Int2Str(KEYPAD_COMPACT,tmp_key_type,5);
	            MMIAPICOM_Wstrcat(tmp_ptr,keyboard_str[S_KEYPAD_COMPACT]);	            
		#elif defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE
	            //MMIAPICOM_Int2Str(FOUR_KEY_UNIVERSE,tmp_key_type,5);
	            MMIAPICOM_Wstrcat(tmp_ptr,keyboard_str[S_FOUR_KEY_UNIVERSE]);	            
		#else
	            //MMIAPICOM_Int2Str(KEYPAD_CUSTOMIZE,tmp_key_type,5);
	            MMIAPICOM_Wstrcat(tmp_ptr,keyboard_str[S_FOUR_KEY_UNIVERSE]);	           
		#endif
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		#ifdef BLUETOOTH_SUPPORT
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 11
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		#ifdef MMI_WIFI_SUPPORT
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 12
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		#ifdef FM_SUPPORT
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 13
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		#ifdef ATV_SUPPORT
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 14
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		#ifdef MMI_GPS_SUPPORT
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 15
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		#ifdef MOTION_SENSOR_TYPE//GSENSOR
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 16
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		#ifdef CAMERA_SUPPORT//CAMERA
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 17
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 18 //phone info
		{
		      char* str = PNULL;			
			uint8 tmp_phone_info[400]={0};
			wchar w_tmp_phone_info[400]={0};
			str = (char*)tmp_phone_info;
			GetPhoneInfoStr(tmp_phone_info, 320);/*lint !e666*/
			MMIAPICOM_StrToWstr(tmp_phone_info,w_tmp_phone_info);
			MMIAPICOM_Wstrcat(tmp_ptr,w_tmp_phone_info);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		
		#ifdef WRE_WRESTORE_SUPPORT//store info
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 19
		{
			MMIAPICOM_Wstrcat(tmp_ptr,tmp_bt);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
		#endif
		
		MMIAPICOM_Wstrcat(tmp_ptr,s_sales_info_arr[i]);// 20
		{	
			MMIAPICOM_Wstrcat(tmp_ptr,default_language_str[MMISET_EDEFAULT_LANGUAGE]);
		}
		i++;
		MMIAPICOM_Wstrcat(tmp_ptr,L"&");
	}
	return tmp_ptr;
}
LOCAL void HandleSalesPdpEvent(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E notify_event)
{
	if(s_sales.flags == FALSE)
	{
		if(MMIPDP_NOTIFY_EVENT_ACTIVED == notify_event)
		{	
			uint16  nv_return=0;
			uint16  nv_value = 0;
			//开机后只执行一次，除非重新开机
			s_sales.flags = TRUE;
			MMINV_READ(SALES_NV_FLAG,&nv_value,nv_return);
			if(nv_return == MN_RETURN_SUCCESS)
			{
				if(nv_value == 0)
				{
					int i = 0;
					//暂不处理有wifi的情况
					//判断哪张卡连上网了，记录下来，
					//启动一个循环计时器,判断什么时候断网
					//for(i=0;i< MMIPDP_MAX_LINK_NUM;i++)
				      	//{
				      		//if(g_mmipdp_manager_instance.link_arr[i].state == MMIPDP_LINK_STATE_ACTIVED)
				      		{
				      			s_sales.sim_sys=dual_sys;//g_mmipdp_manager_instance.link_arr[i].dual_sys;
				      			MMK_CreateTimerCallback(5000,HandlePdpSalesTimer,PNULL,TRUE);
				      		}
				      	//}		
				}			
			}
		}	
	}
}

PUBLIC void MMI_RegSalesNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_SALES, sales_nv_len, sizeof(sales_nv_len)/sizeof(uint16));
}
PUBLIC void MMISALES_InitModule(void)
{
#if defined(HTTP_SUPPORT)
    MMIAPIPDP_SubscribeEvent(MMI_MODULE_SALES, 
        HandleSalesPdpEvent,
        MMIPDP_NOTIFY_EVENT_MAX,
        (MMIPDP_NOTIFY_EVENT_E *)s_sales_pdp_event);
#endif
    g_sales_app.ProcessMsg = HandleSalesMsg;
    MMI_RegSalesNv();
}
LOCAL int GetPhoneInfoStr(uint8* phone_buf, uint32 buf_len)
{
    SCI_DATE_T cur_date = {31, 12, 2004, 6};
    SCI_TIME_T cur_time = {59, 59, 23};
       
       SCI_DATE_T  last_date={0,0,0,0};
       SCI_TIME_T   last_time={0,0,0};
    MN_IMEI_T imei = {0};
    char tmp_buf1[20] = {0};
    char tmp_buf2[20] = {0};
       MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint32 offset = 0;
    uint32 freespace = 0;
    uint32 totalspace = 0;
    uint32 voltage = 0;
    uint len = 0;
    uint i = 0;
    int32 j = 0;
    SALE_MMIENG_TIME_T s_time_from_turnon = {0};
    uint32  s_sec = 0;

    char*          temp_ptr = PNULL;
    uint16          temp_len = 0;
#ifndef _WIN32    
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
#endif
#ifdef MMI_DUAL_BATTERY_SUPPORT
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
#endif
    if(PNULL == phone_buf)
    {
        //SCI_TRACE_LOW:"mmieng_win.c GetPhoneInfoStr phone_buf == null"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4039_112_2_18_2_17_49_131,(uint8*)"");
        return -1;
    }
    
    //SCI_TRACE_LOW:"mmieng_win.c GetPhoneInfoStr buf_len is %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4043_112_2_18_2_17_49_132,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    //手机开机到现在的时间，不过1193小时后会溢出
    s_sec = SCI_GetTickCount()/1000;
    s_time_from_turnon.hour = s_sec/3600;
    s_time_from_turnon.min = (s_sec/60)%60;
    s_time_from_turnon.sec = s_sec%60;  

    //////////////
    // Date Time
    //////////////
    MMI_MEMCPY(phone_buf, buf_len, "<Date Time>", 11, 11);/*lint !e666*/
    offset = 11;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    if (ERR_TM_NONE == TM_GetSysDate(&cur_date))
    {
        MMIAPISET_FormatDateStrByDateStyle(cur_date.year, cur_date.mon, cur_date.mday,'-',(uint8*)tmp_buf1,20);
        MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 10);
        offset += 10;
    }
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    if (ERR_TM_NONE == TM_GetSysTime(&cur_time))
    {
        MMIAPISET_FormatTimeStrByTime(cur_time.hour, cur_time.min,(uint8*)tmp_buf1,20);
        MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 5);
        offset += 5;
    }
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    //////////////
    // Time From Turn on,
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<Time from Turn on>", 19, 19);/*lint !e666*/
    offset += 19;
    phone_buf[offset] = CR_CHAR;
    offset += 1;    
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "hour:", 5, 5);/*lint !e666*/
    offset += 5;
    sprintf(tmp_buf1, "%d", s_time_from_turnon.hour);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "min:", 4, 4);/*lint !e666*/
    offset += 4;
    sprintf(tmp_buf1, "%d", s_time_from_turnon.min);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    //////////////
    // EFS Status
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<EFS Status>", 12, 12);/*lint !e666*/
    offset += 12;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "total:", 6, 6);/*lint !e666*/
    offset += 6;
    #ifndef _WIN32
    freespace = EFS_GetFreeSpace();
    totalspace = freespace + EFS_GetUsedSpace();
    #endif
    sprintf(tmp_buf1, "%ld", totalspace);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "free:", 5, 5);/*lint !e666*/
    offset += 5;
    sprintf(tmp_buf1, "%ld", freespace);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, SCI_STRLEN(tmp_buf1));/*lint !e666*/
    offset += SCI_STRLEN(tmp_buf1);
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    //////////////
    // Battery
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<Battery>", 9, 9);/*lint !e666*/
    offset += 9;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "voltage:", 8, 8);/*lint !e666*/
    offset += 8;
#ifndef _WIN32
    #ifdef MMI_DUAL_BATTERY_SUPPORT
    voltage=dbat_info->chgmng_info.bat_statistic_vol;
    #else
    voltage = p_chgmng_info->bat_statistic_vol;
    #endif
#endif

    sprintf(tmp_buf1, "%6.2f", (double)voltage / 1000);
    len = SCI_STRLEN(tmp_buf1);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, len);
    offset += len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "cur voltage:", 12, 12);/*lint !e666*/
    offset += 12;
#ifndef _WIN32
    #ifdef MMI_DUAL_BATTERY_SUPPORT
    voltage=dbat_info->chgmng_info.bat_cur_vol;
    #else
    voltage = p_chgmng_info->bat_cur_vol;
    #endif
#endif
    sprintf(tmp_buf1, "%6.2f", (double)voltage / 1000);
    len = SCI_STRLEN(tmp_buf1);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, len);
    offset += len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    //////////////
    // IMEI
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<IMEI>", 6, 6);/*lint !e666*/
    offset += 6;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    for(j=0;j < MMI_DUAL_SYS_MAX; j++)
    {
         if(!MNNV_GetIMEIEx(MN_DUAL_SYS_1 + j, imei))
         {
            SCI_MEMSET(imei, 0, sizeof(MN_IMEI_T));
         }
        
        SCI_MEMSET(tmp_buf1, 0, 18);
        for (i = 0; i < 8; i++) 
        {
            tmp_buf1[2 * i] = (imei[i]) & 0x0F;
            tmp_buf1[2 * i + 1] = imei[i] >> 4;
        }
        sprintf(    tmp_buf2,
                "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
                tmp_buf1[1], tmp_buf1[2], tmp_buf1[3],
                tmp_buf1[4], tmp_buf1[5], tmp_buf1[6], tmp_buf1[7],
                tmp_buf1[8], tmp_buf1[9], tmp_buf1[10], tmp_buf1[11],
                tmp_buf1[12], tmp_buf1[13], tmp_buf1[14], tmp_buf1[15]
            );
        MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf2, 19, 15);
        offset += 15;
        phone_buf[offset] = CR_CHAR;
        offset += 1;
    }
    //////////////
    // CHIP ID
    //////////////
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<CHIP TYPE>", 11, 11);/*lint !e666*/
    offset += 11;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
#ifndef _WIN32
    temp_ptr = CHIP_GetChipInfo();      /*lint !e605*/
#else
    temp_ptr = "SC6600R1";  
#endif
    temp_len = SCI_STRLEN((char*)temp_ptr);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, temp_ptr, temp_len, temp_len);
    offset += temp_len;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, "<LAST POWEROFF TIME>", 20, 20);/*lint !e666*/
    offset += 20;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
#ifdef ENG_SUPPORT
    MMINV_READ(MMIENG_NV_LAST_DATA_ID, &last_date,return_value);   

    MMIAPISET_FormatDateStrByDateStyle(last_date.year, last_date.mon, last_date.mday,'-',(uint8*)tmp_buf1,20);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 10);
    offset += 10;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
    MMINV_READ(MMIENG_NV_LAST_TIME_ID, &last_time,return_value);    

    MMIAPISET_FormatTimeStrByTime(last_time.hour, last_time.min,(uint8*)tmp_buf1,20);
    MMI_MEMCPY(phone_buf + offset, buf_len - offset, tmp_buf1, 19, 5);
    offset += 5;
    phone_buf[offset] = CR_CHAR;
    offset += 1;
#endif
    return (offset);
}
LOCAL uint32 GetTimestamp(uint8 g_timezone)
{	
    int i = 0;
    int j = 0;
    uint32 timestamp = 0;
    uint32 secondsOfpastyears = 0;
    uint32 senondsOfpresentyear = 0;
    uint32 senondsOfleapyears = 0;
    uint32 leap_years = 0;
    uint32 sumOfdays = 0;
    int m = 0;
    uint32 timezone = g_timezone;
    SCI_TM_T lt={0};
    TM_GetTime(&lt);
    
    secondsOfpastyears = (lt.tm_year- 1970)*365*24*60*60;

    for(i=1970;i <lt.tm_year;i++)
        {
            if(i%4==0&&i%100!=0||i%400==0)
                {
                    j++;
                }
        }
    leap_years = j;
    senondsOfleapyears = leap_years*24*60*60;
       
    if(lt.tm_year%4 == 0 && lt.tm_year%100 != 0 || lt.tm_year%400 == 0)
        {
            m = 29;
        }
    else
        {
            m = 28;
        }
    switch(lt.tm_mon)
        {
        case 1:
            sumOfdays = 0;
            break;
        case 2:
            sumOfdays = 31;
            break;
        case 3:
            sumOfdays = m + 31;
            break;
        case 4:
            sumOfdays = m + 62;
            break;
        case 5:
            sumOfdays = m + 92;
            break;
        case 6:
            sumOfdays = m + 123;
            break;
        case 7:
            sumOfdays = m + 153;
            break;
        case 8:
            sumOfdays = m + 184;
            break;
        case 9:
            sumOfdays = m + 215;
            break;
        case 10:
            sumOfdays = m + 245;
            break;
        case 11:
            sumOfdays = m + 276;
            break;
        case 12:
            sumOfdays = m + 306;
            break;
        default:
            break;
        }    
    sumOfdays += (lt.tm_mday- 1);
    
    senondsOfpresentyear = (((sumOfdays*24 + lt.tm_hour)*60 + lt.tm_min)*60 + lt.tm_sec);
    
    timestamp = secondsOfpastyears + senondsOfleapyears + senondsOfpresentyear - timezone*60*60;

    return timestamp;               
}
LOCAL MMI_RESULT_E HandleHttpSalesDataInd(SALES_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    //---1--- 
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T signal = {0};
    //uint32 response_code = 0;
   // uint32 data_len = 0;
    //uint8 *data_ptr = PNULL;
    char* tmp_data_buff=NULL;
    //SCI_TRACE_LOW("[sales]:HandleHttpMsg receive HTTP_SIG_DATA_IND0000");
    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_DATA_IND_SIG_T);

    SCI_TRACE_LOW("[sales]:HandleHttpMsg receive HTTP_SIG_DATA_IND");
    
    http_context_ptr->request_id = signal.request_id;
    http_context_ptr->app_instance = signal.app_instance;
    return result;
}
 #define _DM_MD5_BASE64_C_


#define MAX_COLUMNS 45

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static unsigned char PADDING[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


typedef unsigned long int UINT4;



  /*  全局变量声明*/
#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];

#define F(x,y,z)	(z ^ (x & (y ^ z)))
#define G(x,y,z)	(y ^ (z & (y ^ x)))
#define H(x,y,z)	(x^y^z)
#define I(x,y,z)	(y^(x|(~z)))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
  
  
 #define xppMemset(s, c, n)               memset(s, c, n)
 #define xppMemcpy(target, source, cnt)   memcpy(target, source, cnt)
 #define xppMemmove(target, source, cnt)  memmove(target, source, cnt)
 #define xppMemcmp(target, source, cnt)   memcmp(target, source, cnt)
 #define xppMalloc(size)                  SCI_ALLOC(size)
 #define xppFree(ptr)                     SCI_FREE(ptr)

static void SALE_MD5_memset(unsigned char* output,
                                           int value,
                                           unsigned int len
                                           )
{
  unsigned int i;

  for (i = 0; i < len; i++)
 ((char *)output)[i] = (char)value;
}

static void SALE_MD5_memcpy(unsigned char* output,
                                            unsigned char* input,
                                            unsigned int len
                                            )
{
  unsigned int i;

  for (i = 0; i < len; i++)
    output[i] = input[i];
}
/****************************************************************/
// The routine MD5Init initializes the message-digest context
//   mdContext. All fields are set to zero.
/****************************************************************/
LOCAL void SALE_MD5Init(MD5_CTX *context)
{
  context->count[0] = context->count[1] = 0;
  /* Load magic initialization constants.
*/
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}
/****************************************************************/
// Encodes input (UINT4) into output (unsigned char). Assumes len is
//  a multiple of 4.
 /****************************************************************/
static void SALE_Encode(unsigned char *output,
                                unsigned long int *input,
                                unsigned int len
                                )
{
  unsigned int i, j;

      for (i = 0, j = 0; j < len-3; i++, j += 4)  //modify by wangxiaolin 2009.12.20 ,"j<len"->"j<len-3" for warning
     {
             output[j] = (unsigned char)(input[i] & 0xff);
             output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
             output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
             output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
      }
}
/****************************************************************/
// Decodes input (unsigned char) into output (UINT4). Assumes len is
//  a multiple of 4.
/****************************************************************/
LOCAL void SALE_Decode(unsigned long int *output,
                                unsigned char *input,
                                unsigned int len
                               )
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len-3; i++, j += 4)  //modify by wangxiaolin 2009.12.20,"j<len"->"j<len-3" for warning
  {
     output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j+1]) << 8) |
                     (((unsigned long int)input[j+2]) << 16) | (((unsigned long int)input[j+3]) << 24);
  }
 }
/****************************************************************/
// MD5 basic transformation. Transforms state based on block.
/****************************************************************/
LOCAL void SALE_MD5Transform(unsigned long int state[4],unsigned char block[64])
{
  unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  SALE_Decode(x, block, 64);

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  /* Zeroize sensitive information.*/
  SALE_MD5_memset((unsigned char*)x, 0, sizeof (x));
}
/****************************************************************/
// MD5 basic UPDATE. UPDATE state based on block.
/****************************************************************/
LOCAL void SALE_MD5Update(MD5_CTX *context,                                        /* context */
                                      char *input,                               /* input block */
                                      unsigned int inputLen                     /* length of input block */
                                      )
{
  unsigned int i, index, partLen;

  /* Compute number of bytes mod 64 */
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);

  /* Update number of bits */
  if ((context->count[0] += ((unsigned long int)inputLen << 3))
   < ((unsigned long int)inputLen << 3))
 context->count[1]++;
  context->count[1] += ((unsigned long int)inputLen >> 29);

  partLen = 64 - index;

  /* Transform as many times as possible.
*/
  if (inputLen >= partLen) {
 SALE_MD5_memcpy((unsigned char*)&context->buffer[index], (unsigned char*)input, partLen);
 SALE_MD5Transform(context->state, context->buffer);

 for(i = partLen; i + 63 < inputLen; i += 64)
   SALE_MD5Transform(context->state, (unsigned char *)&input[i]);

 index = 0;
  }
  else
 i = 0;

  /* Buffer remaining input */
  SALE_MD5_memcpy((unsigned char*)&context->buffer[index], (unsigned char*)&input[i],inputLen-i);
}
/****************************************************************/
// MD5 basic FINAL. UPDATE state based on block.
/****************************************************************/
LOCAL void SALE_MD5Final ( char digest[16],                         /* message digest */
                                   MD5_CTX *context                                    /* context */
                                 )
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  SALE_Encode(bits, context->count, 8);

  /* Pad out to 56 mod 64.
*/
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  SALE_MD5Update (context, (char *)PADDING, padLen);

  /* Append length (before padding) */
  SALE_MD5Update (context, (char *)bits, 8);
  /* Store state in digest */
  SALE_Encode((unsigned char *)digest, context->state, 16);

  /* Zeroize sensitive information.
*/
  SALE_MD5_memset((unsigned char*)context, 0, sizeof (*context));
}
#endif
