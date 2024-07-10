/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

****************************************************************************/
//#include "csr_spt_log_main.h"
#include "csr_types.h"

extern void CsrDebugTraceLow(const char* const format, ...);


void CsrSptLogPortingPrintf(CsrUtf8String *text, CsrSize len)
{
    
#if defined(_WINDOWS_) || defined(__linux__)
    extern int printf(const char *format, ...);
    printf("%s", text);
#endif

    CsrDebugTraceLow("%s", text);
}

