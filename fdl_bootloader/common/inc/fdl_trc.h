#ifndef _FDL_TRC_H_
#define _FDL_TRC_H_

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

/* Body of header file */
extern void fdl_trace_init(void *, uint32 );
extern void fdl_trace( char* str, uint32 len );
extern uint32 fdl_trace_low(const char *x_format, ...);

//#define DEBUG_TRACE

#ifdef DEBUG_TRACE
//#define SIO_TRACE       fdl_trace_low
//#define SIO_TraceLow    fdl_trace_low
#define FDL_TRACE       fdl_trace_low
#define FDL_TRACELOW    fdl_trace_low
#else
//#define SIO_TraceLow    
//#define SIO_TRACE
#define FDL_TRACE
#define FDL_TRACELOW   
#endif

#ifdef __cplusplus  /* Insert end of extern C construct */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */

#endif