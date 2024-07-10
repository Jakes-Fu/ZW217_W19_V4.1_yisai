//***********************************************************//
//********************** Application include ********************//
//***********************************************************//
#include "mn_type.h"
#include "od_adapter_socket.h"
#include "od_adapter_http.h"
#include "od_adapter_log.h"

//***********************************************************//
//************************* Macro define ********************//
//***********************************************************//
#define HTTP_MAX_FIELD_LEN   199	
#define HTTP_MAX_SND_BUFFER_SIZE (1024*40)
#define HTTP_MAX_RCV_BUFFER_SIZE  1600
#define HTTP_MAX_SESSION_ID_LEN 100
#define HTTP_MAX_RETRY_COUNTER 3   

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_206 206
#define HTTP_STATUS_IGNOR 400
#define HTTP_STATUS_404 404

#define HTTP_TRANS_TIMEOUT 15000   /* It means that 15 seconds */

#define HTTP_POST_TIMEOUT 15000  /* It means that 15 seconds */
#define HTTP_WAIT_FOR_MMI_TIMEOUT 40000  /* It means that 30 seconds */
#define HTTP_RETRY_TIMEOUT 1000  /* It means that 1 seconds */
#define HTTP_WAIT_TIMEOUT 1000 /* It means that 5 seconds */


/* bug 4418. anting.hu 2011.5.16 */
#define MISS_MESSAGE_SEC 200
//***********************************************************//
//************************* Extern variables ********************//
//***********************************************************//


//***********************************************************//
//************************* Static variables  ********************//
//***********************************************************//

typedef enum
{
    MSG_ID_HTTP_ERROR_IND, 
} HTTP_MSG_ID_ENUM;

typedef struct
{
    OD_S32		id;
    void* 	param;
}E_MESSAGE;

typedef struct OH_CONTEXT2_TAG
{
	OD_U8 state;
	OD_S8 socket_id;
	OD_U8 connect_retry_counter;
	OD_U8 send_retry_counter;

	OD_S32       acount_id;
	OD_U8  rcvd_buffer[HTTP_MAX_RCV_BUFFER_SIZE];				
	OD_U32 rcvd_data_len;
	/* bug 4418. anting.hu 2011.5.16 */
	OD_U32 send_statu_len;	
	OD_U32 judge_time;
	
	OD_U32 rcvd_counter;									

	OD_U8*   snd_buffer;					
	OD_U32 snd_data_len;
	OD_U32 snd_counter;										

	OD_U32 len_content;										
	OD_U8 sID[HTTP_MAX_SESSION_ID_LEN];							
	OD_S8 priority;					
	OD_U8 server_ip_addr[4];
	OD_U16 server_port;
	OD_HTTP_URL_STRUCT2 *url_parsed_info;

	/* add callBack */
	OD_BEGIN_FUNC2 begin_cb;
	OD_DATA_FUNC2 data_cb;
	OD_END_FUNC2 end_cb;
	OD_SEND_BUF_FUNC2 send_buf_cb;		
	OD_ERROR_FUNC2 error_cb;
	OD_U8* url;				/* example  :http://live.supermonkeylive.cn/FileServer/DownloadServlet*/
	//OD_U8 hostname[50];
	OD_U8 *hostname;
	OD_U16 post_timer;
	OD_U16 wait_timer;
	OD_BOOL send_status_ind;
	OD_U32 error;
	OD_BOOL protocol_type;
    //OD_U8 token;
} OH_CONTEXT2;

 OH_CONTEXT2 od_http_context2 = {0x00};

static OD_HTTP_DATA_STATUS2 http_status = {0};

//***********************************************************//
//************************* Public variables  ********************//
//***********************************************************//


//***********************************************************//
//************************* Extern function declare **************//
//***********************************************************//


//***********************************************************//
//************************* Static function declare***************//
//***********************************************************//

static void http_error_hdlr(OD_S32 ret);
static void http_send_request(void);

static void http_recv_response(void);
static void http_on_response_received( OD_U8* end);
static void http_on_receive_completed(void);
static void http_on_status_ok( OD_U8* end);
static void http_send_status( OD_HTTP_STATE2_ENUM2 state, OD_S32 percent);
static void http_send_error_ind(OD_S32 err_type);
static OD_BOOL http_parse_response();
static OD_S32 http_get_response_state(void);
static OD_S32 http_get_field( const OD_U8 *szSession, OD_U8 *szValue, OD_S32 nMaxLength);
static void http_close( OD_BOOL is_exit);
static void http_soc_notify_hdlr(OD_SOCKET2 s, OD_SocketStatus2 nStatus, OD_SocketRetCode2 nRetcode, OD_S8* pBuff, OD_U32 nLen);
static void http_recv_response(void);

static void http_receive_data(void);

//***********************************************************//
//************************* Public function declare***************//
//***********************************************************//


//***********************************************************//
//************************* Static function implement ************//
//***********************************************************//


static void http_upper2lower_case(OD_U8* pstr, OD_U32 len)
{
	OD_S32 i;	 
	OD_U8* p = pstr;
	for (i=0; i<len; i++, p++)
	{
		if ('A' <= *p && *p <= 'Z') *p += ('a' - 'A');
	}
}

static void http_free_url(void)
{
	//OD_LOG("http_free_url url_parsed_info=%d\n",od_http_context2.url_parsed_info);

	if (od_http_context2.url_parsed_info)
	{	
	//OD_LOG("http_free_url ->host_name=%d\n",od_http_context2.url_parsed_info->host_name);
		if (od_http_context2.url_parsed_info->host_name)
		{
			od_game_free(od_http_context2.url_parsed_info->host_name);
			od_http_context2.url_parsed_info->host_name=0;
		}
		//OD_LOG("http_free_url ->url_link=%d\n",od_http_context2.url_parsed_info->url_link);
		if (od_http_context2.url_parsed_info->url_link)
		{
			od_game_free(od_http_context2.url_parsed_info->url_link);
			od_http_context2.url_parsed_info->url_link = 0;
		}
		//OD_LOG("http_free_url ->param=%d\n",od_http_context2.url_parsed_info->param);
		if (od_http_context2.url_parsed_info->param)
		{
			od_game_free(od_http_context2.url_parsed_info->param);
			od_http_context2.url_parsed_info->param=0;
		}
		//OD_LOG("http_free_url ->free url_parsed_info\n");
		od_game_free(od_http_context2.url_parsed_info);
		od_http_context2.url_parsed_info = 0;
	}
	//OD_LOG("http_free_url leave\n");
}

static OD_HTTP_URL_STRUCT2 *http_get_parsed_url(const OD_S8 *in_str)
{
	OD_U8 *hostname = OD_NULL;
	OD_HTTP_URL_STRUCT2 *rsp = OD_NULL;

	rsp = od_game_malloc(sizeof(OD_HTTP_URL_STRUCT2));
	if (rsp)
	{
		memset(rsp, 0, sizeof(OD_HTTP_URL_STRUCT2));
		hostname = strstr((OD_U8*)in_str, "http://");
		if (!hostname)
		{
		    od_game_free(rsp);
		    return OD_NULL;
		}
		else
		{
		    OD_U8 *slash, *colon, *qs, *http, *port_str;
			
		    hostname += 7;  /* strlen(http://) */
		    slash = strchr(hostname, '/');
		    http = strstr(hostname,"http:");
		    colon = strchr(hostname, ':');
		    if (colon && !http)
		    {
		    	rsp->host_name_len = (colon - hostname);
		    	rsp->host_name = od_game_malloc( (rsp->host_name_len + 1));
				memset(rsp->host_name, 0, rsp->host_name_len + 1);
		    	memcpy(rsp->host_name, hostname, rsp->host_name_len);
				if(slash)
				{
				    port_str = od_game_malloc( (slash - colon));
				    memset(port_str, 0, slash - colon);
				    strncpy(port_str, colon + 1, slash - colon - 1);
				    rsp->ser_port_num = atoi(port_str);
				    od_game_free(port_str);
				}
				else
				{
					rsp->ser_port_num = atoi(colon);
				}			
		    }
		    else
		    {
				rsp->ser_port_num = 80;
				if(slash)
					rsp->host_name_len = slash - hostname;
				else
					rsp->host_name_len = strlen(hostname);
				rsp->host_name = od_game_malloc( (rsp->host_name_len + 1));
				memset(rsp->host_name, 0, rsp->host_name_len + 1);
			    memcpy(rsp->host_name, hostname, rsp->host_name_len);		
		    }

		    if(slash)
		    {		   
				qs = strchr(slash, '?');
				if (qs)
				{
					rsp->param_len = strlen(qs);
					rsp->url_link_len = qs - slash;
					rsp->param = od_game_malloc((rsp->param_len + 1));
					memset(rsp->param, 0, rsp->param_len + 1);
					memcpy(rsp->param, qs, rsp->param_len);
				}
				else
				{
					rsp->url_link_len = strlen(slash);
				}
				rsp->url_link = od_game_malloc( (rsp->url_link_len + 1));
				memset(rsp->url_link, 0, rsp->url_link_len + 1);
				memcpy(rsp->url_link, slash, rsp->url_link_len);				
		    }
			else // taml fixed the mem leak bug and can not access the no path url size . eg: http://www.baidu.com
			{
				rsp->url_link_len = 1;
				rsp->url_link = od_game_malloc( (rsp->url_link_len + 1));
				memset(rsp->url_link, 0, rsp->url_link_len + 1);
				strcpy(rsp->url_link,"/");
			}
			return rsp;
		}
	}
	else
	{
	    return OD_NULL;
	}
	return OD_NULL;
}


#ifdef MMI_GPRS_SWITCH_SUPPORT
#include "mmiconnection_export.h"
OD_BOOL s_od_gprs_is_changed = OD_FALSE;
#endif

static void http_open_stream(void)
{
	ODConnect_Parameter2 param = {0};

 #ifdef MMI_GPRS_SWITCH_SUPPORT
    if(MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_ON);
        s_od_gprs_is_changed = OD_TRUE;
    }
#endif

	OD_LOG("http_open_stream\n");
	
	 /*need to free http url memory,before parseing url*/
	http_free_url();
	od_http_context2.url_parsed_info = (OD_HTTP_URL_STRUCT2*)  http_get_parsed_url((const OD_S8*)od_http_context2.url);
	
	if (od_http_context2.url_parsed_info &&
		od_http_context2.url_parsed_info->host_name &&
		od_http_context2.url_parsed_info->url_link)
	{
	    
		/*if current host name is same as hte previous one, then send http request directly and no need to get host again*/
		if (od_http_context2.socket_id > 0 &&od_http_context2.hostname!=NULL&& strcmp((const OD_U8*)od_http_context2.url_parsed_info->host_name, (OD_U8*)od_http_context2.hostname) == 0 )
		{
		
			http_send_request();
			return;
		}
		else
		{
			if(od_http_context2.socket_id > 0)
				http_close( OD_FALSE);
			if(od_http_context2.hostname!=NULL)
			{
				od_game_free(od_http_context2.hostname);
			}
			od_http_context2.hostname = (OD_U8*)od_game_malloc(od_http_context2.url_parsed_info->host_name_len+1);
			if(od_http_context2.hostname==NULL)return;
			// tanml hostname size is 50 ,are you sure ok?
			memcpy(od_http_context2.hostname, od_http_context2.url_parsed_info->host_name, od_http_context2.url_parsed_info->host_name_len);
			od_http_context2.hostname[od_http_context2.url_parsed_info->host_name_len] = 0;
		}
		param.domain = od_http_context2.url_parsed_info->host_name;
		param.connType = OD_SOCKET2_TCP;
		param.fun = http_soc_notify_hdlr;
		param.nPort = od_http_context2.url_parsed_info->ser_port_num;
		
		memset(&od_http_context2.server_ip_addr, 0, 4);
		od_http_context2.server_port = (od_http_context2.url_parsed_info->ser_port_num);
		od_http_context2.state = OD_HTTP_SOC2_CREATING;
		OD_LOG("http_open_stream--HOST_NAME=%s\n",od_http_context2.url_parsed_info->host_name);
		od_http_context2.socket_id =  Od_SocketStart2(& param);
		
	
	}
	else
	{
		http_error_hdlr(OD_HTTP_SOC2_INVAL_PARA);
	}
}




static OD_BOOL http_format_request(void)
{
	OD_S32 len = 0;
	if(od_http_context2.snd_buffer == OD_NULL)
	{
		od_http_context2.snd_buffer = od_game_malloc(HTTP_MAX_SND_BUFFER_SIZE);
	}
	//SCI_ASSERT((od_http_context2.snd_buffer != OD_NULL));
	if(OD_NULL == od_http_context2.snd_buffer)
	{
		OD_LOG("http_format_request od_http_context2.snd_buffer ==NULL");
		http_error_hdlr( OD_HTTP_BUFFER2_INVALID);
		return OD_FALSE;
	}
	od_http_context2.rcvd_data_len = 0;
	
	if(od_http_context2.send_buf_cb != OD_NULL)
	{
		len = od_http_context2.send_buf_cb(od_http_context2.snd_buffer, 
		                                       od_http_context2.url_parsed_info, 
		                                      &od_http_context2.rcvd_data_len);
	}
	
	od_http_context2.snd_data_len = len;
	od_http_context2.snd_counter = 0;
	
	memset(od_http_context2.rcvd_buffer, '\0', HTTP_MAX_RCV_BUFFER_SIZE);
	od_http_context2.rcvd_counter = 0;

	return OD_TRUE;
}

static void http_send_request(void)
{	
	OD_S32 data_ret;
	OD_LOG("http_send_request-send state=%d",od_http_context2.state);
		if(od_http_context2.state == OD_HTTP_REQ_SENT2)
		{
		
			return;
		}
		else if (od_http_context2.state != OD_HTTP_REQ_SENDING2 )
		{
			//Format http Request
				
			if(!http_format_request())
			{
				return;
			}
		}
			
		if(od_http_context2.snd_data_len > od_http_context2.snd_counter)
		{
			
			data_ret = Od_SocketSendData2(od_http_context2.socket_id, (od_http_context2.snd_buffer + od_http_context2.snd_counter), od_http_context2.snd_data_len - od_http_context2.snd_counter);
			
			OD_LOG("http_send_request-send ret=0x%x counter=0x%x total=0x%x",data_ret,od_http_context2.snd_counter,od_http_context2.snd_data_len);
			if(data_ret == od_http_context2.snd_data_len - od_http_context2.snd_counter)
			{
				od_http_context2.state = OD_HTTP_REQ_SENT2;
				od_http_context2.snd_counter =od_http_context2.snd_data_len;
			}
			else if(data_ret > 0)
			{
				od_http_context2.state = OD_HTTP_REQ_SENDING2;
				od_http_context2.snd_counter += data_ret;
			}
			else if(data_ret == OD_ERR2_SOCK_WOULDBLOCK)
			{

			}
			else
			{
				http_error_hdlr( OD_HTTP_SOC2_SEND_FAILED);
			}
			
		}
}

static void http_recv_response(void)
{
	OD_S32 data_ret;
	
	if(od_http_context2.state == OD_HTTP_RSP_RCVD2)
	{
		http_receive_data();
	}
	else
	{
		data_ret = Od_ReceiveData2(od_http_context2.socket_id, od_http_context2.rcvd_buffer+od_http_context2.rcvd_counter, HTTP_MAX_RCV_BUFFER_SIZE - od_http_context2.rcvd_counter -1);
		
		if (data_ret > 0)
		{
			od_http_context2.rcvd_counter += data_ret;
			http_parse_response();		
		}
		else if(data_ret < 0)
		{
			if (data_ret == OD_ERR2_SOCK_WOULDBLOCK)
			{

			}
			else
			{
				
				http_error_hdlr( OD_HTTP_SOC2_RECV_FAILED);
			}
		}
	}
}

/*devide data : head + \r\n\r\n + data*/
static OD_BOOL http_parse_response(void)
{
	OD_U8* end = OD_NULL;		
	
	od_http_context2.rcvd_buffer[od_http_context2.rcvd_counter] = OD_NULL;
    	end = strstr((const OD_U8*)od_http_context2.rcvd_buffer, "\r\n\r\n");

	if(end != OD_NULL)
	{
		OD_U32 len = (OD_U8*)end - (OD_U8*)od_http_context2.rcvd_buffer;	
		//Must change to lower_case
		http_upper2lower_case((OD_U8*)od_http_context2.rcvd_buffer, len);	
		http_on_response_received(end);
	}
	else if (od_http_context2.rcvd_counter < HTTP_MAX_RCV_BUFFER_SIZE)
	{
		return OD_FALSE;
	}
	else
	{
		http_error_hdlr( OD_HTTP_SOC2_RECV_FAILED);
	}

	return OD_TRUE;
}

/*deal the data in response*/
static void http_on_response_received( OD_U8* end)
{
	OD_S32  nSvrState;
	
	//od_http_context2.state = STREAM_RSP_RCVD;
	nSvrState = http_get_response_state();	
	od_http_context2.len_content  = 0;	 
	OD_LOG("http_on_response_received--nSvrState=%d",nSvrState);
	if(nSvrState  == HTTP_STATUS_OK || nSvrState  == HTTP_STATUS_206)	
	{
		//if response is not ok, but set to STREAM_RSP_RCVD, 
		//will turn to STREAM_TRANSMITTING state in Live_Stream_Receive_Data()
		//then make corrupt. 
		//e.g:a random song name will never get a lyric file,but give error response instead
		od_http_context2.state = OD_HTTP_RSP_RCVD2;
		http_on_status_ok(end);
	}
	else if(nSvrState  == HTTP_STATUS_IGNOR||nSvrState  == HTTP_STATUS_404)
	{
		http_error_hdlr(OD_HTTP_BAD_REQUEST2);
	}
	else 
	{
		http_error_hdlr(OD_HTTP_STATUS_NOK2);	
	}	
	
}

static OD_S32 http_get_response_state(void)
{
	OD_U8 szState[4];
	szState[0] = (OD_U8)od_http_context2.rcvd_buffer[9];
	szState[1] = (OD_U8)od_http_context2.rcvd_buffer[10];
	szState[2] = (OD_U8)od_http_context2.rcvd_buffer[11];
	szState[3] = OD_NULL;

	return atoi(szState);	
}

static OD_BOOL http_receive_begin(void)
{
	
	http_send_status( OD_HTTP_STATE2_BUFFER_0, 0);

  	if(od_http_context2.begin_cb)
  	{
          	if(od_http_context2.begin_cb(od_http_context2.len_content) == OD_FALSE)
          	{
			http_error_hdlr( OD_HTTP_FILE_ACCESS_FAILED2);
			return OD_FALSE;
          	}
  	}
	
	return OD_TRUE;
}

static OD_BOOL http_on_data_arrive( OD_U8* data, OD_U32 len)
{
	OD_S32 percent =0;
	
	if(od_http_context2.len_content != 0)
	{
		
				
		if((od_http_context2.rcvd_data_len - od_http_context2.send_statu_len) >= (od_http_context2.len_content >> 8))
		{
			 od_http_context2.send_statu_len = od_http_context2.rcvd_data_len;
			 
			percent = (OD_S32) od_http_context2.rcvd_data_len * 1000 / od_http_context2.len_content;
			http_send_status(OD_HTTP_STATE2_BUFFERING, percent);
		}
	}
	
	if(od_http_context2.data_cb)
	{
 		if(od_http_context2.data_cb((void*)data, len) == OD_FALSE)
 		{
			http_error_hdlr(OD_HTTP_FILE_ACCESS_FAILED2);
			return OD_FALSE;
 		}
	}

	if (od_http_context2.rcvd_data_len >= od_http_context2.len_content)
	{		   
		http_on_receive_completed();
		return OD_FALSE;
	}

	return OD_TRUE;	
}

static void http_receive_data(void)
{
	OD_S32 ret;
	
	if ( od_http_context2.state == OD_HTTP_RSP_RCVD2 || od_http_context2.state ==  OD_HTTP_TRANSMITTING2 )
	{  	
		od_http_context2.state = OD_HTTP_TRANSMITTING2;
				
		do 
        {	   
			ret = Od_ReceiveData2(od_http_context2.socket_id, 
						   od_http_context2.rcvd_buffer, 
						   HTTP_MAX_RCV_BUFFER_SIZE );
						
			if (ret > 0)
			{
				od_http_context2.rcvd_data_len += ret;
				if (OD_FALSE == http_on_data_arrive(od_http_context2.rcvd_buffer, ret))
				{
					return;
				}
			}
			else
			{				
				if (ret == OD_ERR2_SOCK_WOULDBLOCK)
				{
					
				}
				else
				{
					http_error_hdlr( OD_HTTP_SOC2_RECV_FAILED);
				}
			}
		}while(ret > 0);
	}
}

static void http_on_receive_completed(void)
{
	OD_S32 percent = 0;
	
	if(od_http_context2.len_content != 0)
		percent = (OD_S32) od_http_context2.rcvd_data_len * 1000 / od_http_context2.len_content;
	
		
	/*keep http state as "connected" for the next connecting*/
	od_http_context2.state = OD_HTTP_SOC2_CONNECTED;

	http_send_status(OD_HTTP_STATE2_BUFFERED, percent);

	od_http_context2.connect_retry_counter = 0;
	od_http_context2.send_retry_counter = 0;	 

	if (od_http_context2.end_cb)
		od_http_context2.end_cb();
	http_close( OD_TRUE);
}

static void http_on_status_ok(OD_U8* end)
{
   	OD_U8 szValue[HTTP_MAX_FIELD_LEN+1]={0};
	
	OD_S32  nHeaderLen, len;
	OD_U32 head_len = 0;
	
	http_get_field("content-length", szValue,HTTP_MAX_FIELD_LEN);   	 
	od_http_context2.len_content = atoi(szValue);
	OD_LOG("http_on_status_ok  content-length=%d\n",od_http_context2.len_content );
	//We got a empty file
	if(od_http_context2.len_content == 0)
	{
		od_http_context2.state = OD_HTTP_WAIT_FOR_SERVER2;
		http_error_hdlr(OD_HTTP_DATA_EMPTY2);
		
	}
	else 
	{		
		// Get Session ID		 
		len = http_get_field( "sid", szValue, HTTP_MAX_SESSION_ID_LEN);
	       if (len > 0 )
		{
			   memcpy(od_http_context2.sID, szValue, len+1);
		}
	   
		end += 4;  
		nHeaderLen = (OD_U8*)end - (OD_U8*)od_http_context2.rcvd_buffer ;		 
		head_len = od_http_context2.rcvd_counter - nHeaderLen;
		od_http_context2.rcvd_data_len += head_len;
		
		
		if(od_http_context2.send_status_ind)
		{
			od_http_context2.send_statu_len = od_http_context2.rcvd_data_len;
			  
				 
			
		}
		
		if (OD_FALSE == http_receive_begin())
		{
			return;
		}

		if (head_len > 0)
		{
			if (OD_FALSE == http_on_data_arrive((OD_U8*)end, head_len))
			{
				return;
			}
		}
		//http_receive_data();		   
	}
	
}


/*example: 
rcvd_buffer = "http/1.1 200 ok
	date: mon, 26 apr 2010 03:05:22 gmt
	server: apache/2.2.9 (unix) mod_jk/1.2.14
	content-disposition: attachment; filename=93604000.3gp
	sid: channela2_1272250932000_0
	content-length: 90665
	keep-alive: timeout=20, max=100
	content-type: application/x-msdownload;"
szSession: content-length
*/
static OD_S32 http_get_field( const OD_U8 *szSession, OD_U8 *szValue, OD_S32 nMaxLength)
{	
	OD_U8 *p, *lpsz, *from;
	OD_S32 len;
	
	
	from = (OD_U8*)od_http_context2.rcvd_buffer;
	lpsz = strstr(from, szSession);
	
	if(!lpsz) return -1;
	
	//lpsz++;
	p=lpsz+strlen(szSession)+2;
	lpsz = strstr(p, "\r\n");
	
	if(!lpsz) return -1;
	
	len = lpsz-p;
	if (len >= nMaxLength)
	{
		return -1;
	}
	
	memcpy(szValue, p, len);
	szValue[len] = OD_NULL;
	
	return len;	
}

static void http_free_context(void)
{
	OD_LOG("http_free_context enter\n");

	http_free_url();
	OD_LOG("http_free_context-url=%d\n",od_http_context2.url);
	if(od_http_context2.url)
	{
		od_game_free(od_http_context2.url);
		od_http_context2.url = 0;
	}
	od_http_context2.snd_data_len = od_http_context2.snd_counter = 0;
	OD_LOG("http_free_context leave\n");
}







void http_soc_notify_hdlr(OD_SOCKET2 s, OD_SocketStatus2 nStatus, OD_SocketRetCode2 nRetcode, OD_S8* pBuff, OD_U32 nLen)
{
	OD_S32 len;
    OD_LOG("http_soc_notify_hdlr--code=%d,nStatus=%d\n",nRetcode,nStatus);
	if(nRetcode < 0)
	{
		switch(nRetcode)
		{
		case OD_ERR2_SOCK_CREATE:
		case OD_ERR2_SOCK_LIMIT_RESOURCE:
			
		case OD_ERR2_SOCK_GET_HOST_BYNAME_FAIL:
			http_error_hdlr( OD_HTTP_CREATE_SOC_FAIL2);
			break;
		case OD_ERR2_SOCK_CONN_FAIL:
			http_error_hdlr( OD_HTTP_SOC2_CONNECT_FAILED);
			break;
		
		default:
			http_error_hdlr( OD_HTTP_STATUS_NOK2);
			break;
		}

	}
	else
	{
		
		switch(nStatus)
		{
		case OD_SOCK2_CONNECTED:
		case OD_SOCK2_SENT:
			http_send_request();
			OD_LOG("http_soc_notify_hdlr SENT");
			break;
		case OD_SOCK2_RECEIVED:

    			len = Od_ReceiveData2(od_http_context2.socket_id,od_http_context2.rcvd_buffer,HTTP_MAX_RCV_BUFFER_SIZE);
    			if(len <= 0)
    			{	
    			    if (len != OD_ERR2_SOCK_WOULDBLOCK)
    				{
						http_error_hdlr(OD_HTTP_SOC2_RECV_FAILED);
    				}
    				break;
    			}
	    		if(od_http_context2.protocol_type)
		 		{
                        if(od_http_context2.state == OD_HTTP_RSP_RCVD2)
                        {
                        	od_http_context2.len_content += len;
                        }
                        else if(od_http_context2.state == OD_HTTP_REQ_SENT2)
                        {   
                            od_http_context2.len_content = len;
                            od_http_context2.state = OD_HTTP_RSP_RCVD2;
                        }
	
                        if(od_http_context2.data_cb)
                        {                         
                            if(od_http_context2.data_cb((void*)od_http_context2.rcvd_buffer, len) == OD_FALSE)
                            {                                     
                                http_error_hdlr(OD_HTTP_FILE_ACCESS_FAILED2);
                            }
                        }
                        break;
				}

    			if(od_http_context2.state == OD_HTTP_RSP_RCVD2)
    			{
    				//memcpy(od_http_context2.rcvd_buffer,pBuff,nLen);
    				od_http_context2.rcvd_data_len += len;
    				http_on_data_arrive(od_http_context2.rcvd_buffer, len);
    			}
    			else if(od_http_context2.state == OD_HTTP_REQ_SENT2)
    			{   
    					//memcpy(od_http_context2.rcvd_buffer+od_http_context2.rcvd_counter,pBuff,nLen);
    					od_http_context2.rcvd_counter += len;
    					http_parse_response();			
    			}
			break;
		case OD_SOCK2_CLOSING:
                     //½ÓÊÕÍê±Ï
                     if(od_http_context2.protocol_type && od_http_context2.len_content > 0 && od_http_context2.state == OD_HTTP_RSP_RCVING2)
                     {
                        OD_LOG("od_http_context2.len_content =%d",od_http_context2.len_content);
                        if (od_http_context2.end_cb)
                        {
                            od_http_context2.end_cb();
                        }
                        http_close(OD_TRUE);
                        break;
                     }
                     http_error_hdlr( OD_HTTP_SERVER_CLOSED2);
			break;
		case OD_SOCK2_HOSTGETTED:
			memcpy(od_http_context2.server_ip_addr, pBuff,4);

			break;
		default:
			break;
		}
	}
	
}
static void http_close( OD_BOOL exit)
{

#ifdef MMI_GPRS_SWITCH_SUPPORT
    if(s_od_gprs_is_changed)
    {
        s_od_gprs_is_changed = OD_FALSE;
        MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_OFF);
     }
#endif

	//  Close socket
	od_http_context2.state=OD_HTTP_IDLE2;
	if (od_http_context2.socket_id >= 0)
	{
		Od_SocketClose2(od_http_context2.socket_id );
		od_http_context2.socket_id = -1;
	}
	if(od_http_context2.snd_buffer)
	{
	    od_game_free(od_http_context2.snd_buffer);
		od_http_context2.snd_buffer = OD_NULL;
	}
	
	//http_stop_timer(mod_id, od_http_context2.post_timer);
	//http_stop_timer(mod_id, od_http_context2.wait_timer);
	
	if(exit)
		http_free_context();
	if(od_http_context2.hostname!=NULL)
	{
		od_game_free(od_http_context2.hostname);
	}
	od_http_context2.hostname = NULL;
	//memset(od_http_context2.hostname, 0, sizeof(od_http_context2.hostname));
}

static void http_error_hdlr(OD_S32 ret)
{
	

		OD_LOG("http_error_hdlr-TYPE=%d\n",ret);
		http_close( OD_TRUE);	
		if(od_http_context2.error_cb)
		  (*od_http_context2.error_cb)(ret);
		return;
}



static void http_send_status( OD_HTTP_STATE2_ENUM2 state, OD_S32 percent)
{
		http_status.state = state;
		http_status.percent = percent;
	
}

//***********************************************************//
//************************* Public function implement ************//
//***********************************************************//

void Od_HttpOpen2(OD_U8* url, 
				OD_SEND_BUF_FUNC2 send_buf_cb, 
				OD_BEGIN_FUNC2 begin_cb, 
				OD_DATA_FUNC2 data_cb, 
				OD_END_FUNC2 end_cb, 
				OD_ERROR_FUNC2 error_cb)
{
	OD_U8 sim_id;
	OD_U16 len;
	OD_LOG("Od_HttpOpen2 enter--url=%s\n",url);

	sim_id = Od_AutoChangeSim()-1;

	if(url == OD_NULL||sim_id>=MN_DUAL_SYS_MAX)
	{
		http_error_hdlr( OD_HTTP_SOC2_UNKNOWN_URL);
		return;
	}
	OD_LOG("Od_HttpOpen2- http_free_context\n");
	http_free_context();
	OD_LOG("Od_HttpOpen2- copy url\n");
	len = strlen(url) + 1;
	od_http_context2.url = (OD_U8*)od_game_malloc(len);
	if(od_http_context2.url==NULL)
	{
		http_error_hdlr( OD_HTTP_BUFFER2_INVALID);
		return;
	}
	memset(od_http_context2.url, 0, len);
	strcpy(od_http_context2.url, url);
	
	od_http_context2.begin_cb = begin_cb;
	od_http_context2.data_cb = data_cb;
	od_http_context2.end_cb = end_cb;
	od_http_context2.send_buf_cb = send_buf_cb;
	od_http_context2.error_cb = error_cb;
	http_open_stream();
}

void Od_HttpStop2(void)
{
	 OD_LOG("Od_HttpStop2\n");

	if(od_http_context2.error_cb)
	{
		//(*od_http_context2.error_cb)(OD_HTTP_SERVER_CLOSED2);	
	}
	http_close( OD_TRUE);

	#if 1
	od_http_context2.begin_cb = NULL;
	od_http_context2.data_cb = NULL;
	od_http_context2.end_cb = NULL;
	od_http_context2.send_buf_cb = NULL;
	od_http_context2.error_cb = NULL;
	#endif
}

OD_BOOL Od_HttpInit2(OD_BOOL type)
{
	

	od_http_context2.socket_id = -1;
	od_http_context2.post_timer = 0;
	od_http_context2.wait_timer = 1;
	 OD_LOG("od_http_context2->url_parsed_info=%x state=%d\n",od_http_context2.url_parsed_info,http_status.state);
	 od_http_context2.url_parsed_info=0;
	 od_http_context2.protocol_type = type;
	return OD_TRUE;
	 
}

void Od_HttpGetStatus2(OD_HTTP_DATA_STATUS2 * status)
{
	status->percent = http_status.percent;
	status->state = http_status.state;
}

