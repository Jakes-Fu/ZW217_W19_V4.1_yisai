#ifndef SG_DEF_H
#define SG_DEF_H

typedef char SG_INT8;
typedef unsigned char SG_UINT8;
typedef short SG_INT16;
typedef unsigned short SG_UINT16;
typedef long SG_INT32;
typedef unsigned long SG_UINT32;
typedef int SG_INT;
typedef unsigned int SG_UINT;
typedef unsigned long int	uint32;



//#define sg_sprintf sprintf
#define NET_TRACE
#ifdef NET_TRACE
//extern void sg_trace(const SG_INT8 *fmt, ...);
extern uint32 SCI_TraceLow(const char *x_format, ...);
#define sg_printf SCI_TraceLow
#else
#define sg_printf(...)

#endif

#endif

