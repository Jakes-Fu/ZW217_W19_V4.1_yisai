#ifndef _BDL_CUSTOMIF_H_
#define _BDL_CUSTOMIF_H_

#include "sc_fdl_reg.h"

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#define  DOWNLOAD_UART_ADDR     ARM_UART1_BASE//ARM_UART0_BASE  
#define  TRACE_UART_ADDR        ARM_UART1_BASE

extern unsigned long FDL_GetUARTBaseAddr(void);
extern unsigned long FDL_GetTraceBaseAddr(void);
extern unsigned long sio_get_char_en( unsigned char* ch_ptr);

#ifdef __cplusplus  /* Insert end of extern C construct */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */

#endif