
#include "http_api.h"
#include "mmi_module.h"
#include "mmk_app.h"
#include "sig_code.h"
#include "in_message.h"
#include "mmi_common.h"
#include "zteweather_main.h"
#include "zteweather_http.h"

LOCAL HttpRequestResult(MMIZTE_HTTP_CONTEXT_T *http_context_ptr,BOOLEAN issuccess) 
{
	SCI_TRACE_LOW("ZTEHttp %s", __FUNCTION__);
	if (http_context_ptr != PNULL) 
	{
		SCI_TRACE_LOW("ZTEHttp %s time", __FUNCTION__);
		if (0 != http_context_ptr->m_request_time_id) {
			MMK_StopTimer(http_context_ptr->m_request_time_id);
			http_context_ptr->m_request_time_id = 0;
		}

		SCI_TRACE_LOW("ZTEHttp %s callback", __FUNCTION__);
		if (http_context_ptr->chatdata != PNULL) {
			http_context_ptr->callback(http_context_ptr->chatdata, issuccess,http_context_ptr->request_win_idx);
		}else{
			http_context_ptr->callback("", issuccess,http_context_ptr->request_win_idx);
		}

		SCI_TRACE_LOW("ZTEHttp %s free chatdata", __FUNCTION__);
		if(http_context_ptr->chatdata!=PNULL)
		{
			SCI_TRACE_LOW("ZTEHttp %s free data_url", __FUNCTION__);
			if(http_context_ptr->chatdata->data_url!=PNULL)
			{
				SCI_FREE(http_context_ptr->chatdata->data_url);
				http_context_ptr->chatdata->data_url=PNULL;
			}
			SCI_TRACE_LOW("ZTEHttp %s free data", __FUNCTION__);
			if(http_context_ptr->chatdata->data!=PNULL)
			{
				SCI_FREE(http_context_ptr->chatdata->data);
				http_context_ptr->chatdata->data=PNULL;
			}
			SCI_TRACE_LOW("ZTEHttp %s free postBody", __FUNCTION__);
			if(http_context_ptr->chatdata->postBody!=PNULL)
			{
				SCI_FREE(http_context_ptr->chatdata->postBody);
				http_context_ptr->chatdata->postBody=PNULL;
			}
			SCI_TRACE_LOW("ZTEHttp %s free chatdata", __FUNCTION__);
			SCI_FREE(http_context_ptr->chatdata);
			http_context_ptr->chatdata=PNULL;
		}
		SCI_TRACE_LOW("ZTEHttp %s free http_context_ptr", __FUNCTION__);
		SCI_FREE(http_context_ptr);
		http_context_ptr=PNULL;
	}
	SCI_TRACE_LOW("ZTEHttp %s end", __FUNCTION__);
}


LOCAL void DslMainHttpRequestTimeout(uint8 timer_id, uint32 param) 
{
	MMIZTE_HTTP_CONTEXT_T *http_context_ptr =(MMIZTE_HTTP_CONTEXT_T *)param;
  	SCI_TRACE_LOW("ZTEHttp %s start", __FUNCTION__);
  	if (0 != http_context_ptr->m_request_time_id) {
    	MMK_StopTimer(http_context_ptr->m_request_time_id);
    	http_context_ptr->m_request_time_id = 0;
  	}
  	HttpRequestResult(http_context_ptr, FALSE);
  	HTTP_CloseRequest(http_context_ptr->context_id);
}

LOCAL void ZTEHttpRequestTimer(MMIZTE_HTTP_CONTEXT_T *http_context_ptr,uint16 time)
{
  	SCI_TRACE_LOW("ZTEHttp %s start", __FUNCTION__);
  	if(time != 0)
	{
    	http_context_ptr->m_request_time_id = MMK_CreateTimerCallback(
			time, 
			DslMainHttpRequestTimeout, 
			(uint32)http_context_ptr,
			FALSE
			);
      	MMK_StartTimerCallback(
			http_context_ptr->m_request_time_id, 
			time, 
			DslMainHttpRequestTimeout,
			(uint32)http_context_ptr,
			FALSE);
  	}
}

PUBLIC void StartAbolutoPostRequest(char *url, char *body, uint16 timeout,uint32 request_http_win_idx, ZTEHTTPGETREQUEST callback) 
{
	ZTE_DATA_CONTENT_T *chatdata = (ZTE_DATA_CONTENT_T *)SCI_ALLOCA(sizeof(ZTE_DATA_CONTENT_T));

	HTTP_INIT_PARAM_T *init_param_ptr = PNULL;
	MMIZTE_HTTP_CONTEXT_T *http_context_ptr = PNULL;

	SCI_TRACE_LOW("ZTEHttp post http %s ", __FUNCTION__);
	SCI_MEMSET(chatdata, 0, sizeof(ZTE_DATA_CONTENT_T));

	chatdata->data_url = (char *)SCI_ALLOCA(255 * sizeof(char));
	if (body != PNULL) {
		int len = strlen(body)+1;
    	chatdata->postBody = (char *)SCI_ALLOCA(len * sizeof(char));
    	SCI_MEMSET(chatdata->postBody, 0, len);
    	strcpy(chatdata->postBody, body);
  	}
  	SCI_MEMSET(chatdata->data_url, 0, 255);
  	strcat(chatdata->data_url, url);
  	SCI_TRACE_LOW("ZTEHttp start %s", __FUNCTION__);
  	init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
  	init_param_ptr->is_cookie_enable = FALSE;
  	init_param_ptr->is_cache_enable = FALSE;

  	http_context_ptr = SCI_ALLOC_APPZ(sizeof(MMIZTE_HTTP_CONTEXT_T));
  	http_context_ptr->chatdata = chatdata;
  	http_context_ptr->type = 1;
  	http_context_ptr->request_win_idx = request_http_win_idx;
  	http_context_ptr->callback = callback;
  	ZTEHttpRequestTimer(http_context_ptr, timeout);
  	HTTP_InitRequest(MMI_MODULE_ZTEWEATHER, (uint32)http_context_ptr, init_param_ptr);
  	SCI_FREE(init_param_ptr);
  	init_param_ptr = PNULL;
}
