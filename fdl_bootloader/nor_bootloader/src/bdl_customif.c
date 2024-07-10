
#include "bdl_customif.h"

unsigned long FDL_GetUARTBaseAddr(void)
{
	return DOWNLOAD_UART_ADDR ; 
}

unsigned long FDL_GetTraceBaseAddr(void)
{
	return TRACE_UART_ADDR ; 
}

