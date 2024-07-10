#ifndef _WIN_SCI_TRACE_H_
#define _WIN_SCI_TRACE_H_

#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

PUBLIC uint32 SCI_TraceLow(const char *x_format, ...);   

//PUBLIC uint32 SCI_Trace(const char *x_format, ...);   

#ifdef __cplusplus
    }
#endif

#endif
