#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdarg.h>
#include "abup_typedef.h"
#include "Sci_log.h"

#define ABUP_DELTA_ADD_SIZE		100

#if (ABUP_DELTA_ADD_SIZE == 100)
#include "abup_debug_RES_0100KB.h"
#endif


static abup_int32 abup_debug_switch = 1;

abup_int32 abup_DebugSwitch(abup_int32 nOn)
{
	abup_debug_switch = nOn;

	return E_ABUP_SUCCESS;
}

void abup_DebugPrint(const abup_char *content, ...)
{
	if(abup_debug_switch) {
		va_list Args;
		abup_char out_buf[ABUP_MAX_DEBUG_BUF_LEN] = {0};
		abup_int32 len;
		
		va_start(Args,content);

		len = vsnprintf(out_buf,ABUP_MAX_DEBUG_BUF_LEN, content, Args);
		//SCI_TraceLow("%s\n", out_buf);
		SCI_TRACE_LOW("%s\n", out_buf);
		//SIO_ATC_WriteCmdRes(out_buf,strlen(out_buf));
		
		va_end(Args);
	}
}

void abup_DebugPrint_Ext(const abup_char *key_string,const abup_char *content, ...)
{
	if(abup_debug_switch) {
		va_list Args;
		abup_char out_buf[ABUP_MAX_DEBUG_BUF_LEN] = {0};
		abup_int32 len;
		abup_uint8 key_string_len=0;
		
		va_start(Args,content);
		key_string_len=abup_strlen(key_string);
		abup_strncpy(out_buf,key_string,key_string_len);
		len = vsnprintf(out_buf+key_string_len,ABUP_MAX_DEBUG_BUF_LEN, content, Args);

		//SCI_TraceLow("%s\n", out_buf);
		SCI_TRACE_LOW("%s\n", out_buf);
		
		va_end(Args);
	}
}

void abup_trace2chs(abup_write func, const abup_char *content, ...)
{
	if(abup_debug_switch) {
		va_list Args;
		abup_char out_buf[ABUP_MAX_DEBUG_BUF_LEN] = {0};
		abup_int32 len;
		
		va_start(Args,content);
#ifndef WIN32		
		len = vsnprintf(out_buf,ABUP_MAX_DEBUG_BUF_LEN, content, Args);
#else
		len = _vsnprintf(out_buf,ABUP_MAX_DEBUG_BUF_LEN, content, Args);
#endif
		abup_DebugPrint(out_buf);

		if(func)
			func(out_buf, len);

		va_end(Args);
	}

}

void abup_Debug_print_content(abup_char *content,abup_uint16 size)
{
	abup_uint8 i=0;
	abup_uint16 content_size=size;
	while((content_size-128)>0)
	{
		abup_DebugPrint("%s",content+(i*128));
		i++;
		content_size-=128;
	}
	
	abup_DebugPrint("%s",content+(i*128));	
	
}


void abup_Debug_DeltaNew(void)
{
	abup_char *deviceVersion = (abup_char *)abup_get_device_version();
	abup_uint32 len_tmp=0, idx=0, lenmax=strlen(deviceVersion);
	abup_uint32 tmp[20]={0};
	abup_char *res_data=NULL;

#ifdef ABUP_DELTA_ADD_SIZE

	if(lenmax > 3)
		lenmax = 3;
	
	for(idx=0;idx<lenmax;idx++)
	{	
#if (ABUP_DELTA_ADD_SIZE == 100)		
		res_data=(abup_char *)RES_0100KB;
#endif
		if(res_data)
		{
			len_tmp=res_data[idx]<<24 | res_data[idx+1]<<16 | res_data[idx+2]<<8 | res_data[idx+3]; 
			abup_DebugPrint("abup_Debug_DeltaNew:type: %dKB, idx:%d, len:%d",ABUP_DELTA_ADD_SIZE, idx, len_tmp);
			abup_memcpy(tmp, res_data, lenmax);
		}
	}
#endif	
}

#ifndef WIN32
#ifndef TRACE_INFO_SUPPORT
//abup_uint32 SCI_Trace_ID(abup_uint32 trc_class, abup_uint32 msg_index, const abup_uint8* arg_type,...)
//{
 //   return 1;
//}
#endif
#endif





