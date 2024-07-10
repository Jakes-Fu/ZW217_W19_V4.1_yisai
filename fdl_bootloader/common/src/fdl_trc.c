
#include "common.h"
#include "fdl_channel.h"
#include "fdl_trc.h"

#define MAX_STR_SIZE        250
struct FDL_ChannelHandler   *g_pCh = NULL;

void fdl_trace_init(void *ch, uint32 baudrate)
{
    struct FDL_ChannelHandler  *pCh = (struct FDL_ChannelHandler *)ch;
    
    g_pCh = pCh;
    g_pCh->Open(g_pCh, baudrate);
}

void fdl_trace( char* str, uint32 len )
{
#ifdef DEBUG_TRACE
    uint32    i = 0;
     while(i++ < len)
    {
        //SIO_PutChar(&gUart0Channel, (const unsigned char)*str++);
        g_pCh->PutChar(g_pCh, (const unsigned char)*str++);
    }
#endif    
}

uint32 fdl_trace_low(const char *x_format, ...)
{
#ifdef DEBUG_TRACE
    char       format_str[MAX_STR_SIZE + 1];
	va_list    args;
	int        nBuf;
    va_start(args, x_format);
    nBuf = _vsnprintf(format_str,MAX_STR_SIZE,x_format, args);
    va_end(args);
    
    if(nBuf > 0)
    {
      format_str[nBuf] = 0;
      fdl_trace(format_str, nBuf);
    }
#endif    
    return 0;
}
