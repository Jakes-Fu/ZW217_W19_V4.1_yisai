/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

****************************************************************************/

#include <stdarg.h>

#include "os_api.h"
#include "csr_rel_ver.h"

/*lint -e838*/
// Using platform build system, so need to define it here also
#define SCI_CSR_LOG_ENABLE

#ifdef SCI_CSR_LOG_ENABLE
#define DEBUG_BUFFER_SIZE       (1000)
static char CsrLogBuffer[DEBUG_BUFFER_SIZE] = ""; /*lint !e785 */

/* buffer[len - 1] = '\0'; line removes \n from the formated string */
#define FORMAT_TRACE(buffer, buf_len, fmt)       \
{                                                \
    unsigned int len = 0;                        \
    va_list args;                                \
    va_start(args, fmt);                         \
    len = vsnprintf((buffer),                    \
                    (buf_len),                   \
                    fmt,                         \
                    args);                       \
    va_end(args);                                \
    if (len >= (buf_len))                        \
    {                                            \
        (buffer)[(buf_len) - 1] = '\0';          \
    }                                            \
    else                                         \
    {                                            \
        (buffer)[len] = '\0';                    \
    }                                            \
}

#if defined(WIFI_SUPPORT_CSR6027A05)
static char CsrLogPrefix[] = "CSR6027A05 "CSR_RELEASE_VERSION;
#elif defined(WIFI_SUPPORT_CSR6027B07)
static char CsrLogPrefix[] = "CSR6027B07 "CSR_RELEASE_VERSION;
#else
static char CsrLogPrefix[] = "CSR6027 InvVer";
#endif

#endif

void CsrDebugTraceLow(const char* const format, ...)
{
#ifdef SCI_CSR_LOG_ENABLE
    FORMAT_TRACE(CsrLogBuffer, DEBUG_BUFFER_SIZE, format);/*lint !e64 -e628 */
    SCI_TRACE_LOW("%s:%s", CsrLogPrefix, CsrLogBuffer); /*lint !e64 */
#endif
}

void CsrDebugTraceLowDirect(const char* const text)
{
#ifdef SCI_CSR_LOG_ENABLE
    SCI_TRACE_LOW(text); /*lint !e64 */


#endif
}

void CsrDebugTraceLowDirectBreak(const char* const text)
{
    CsrDebugTraceLowDirect(text);
    //SCI_PASSERT(0,(text));
}

void CsrDebugTraceHigh(const char* const format, ...)
{
#ifdef SCI_CSR_LOG_ENABLE
    FORMAT_TRACE(CsrLogBuffer, DEBUG_BUFFER_SIZE, format);/*lint !e64 */
    SCI_TRACE_LOW("%s:%s", CsrLogPrefix, CsrLogBuffer); /*lint !e64 */
#endif
}


void csr_trace_simple(int id, const char* const format, ...)
{
#ifdef SCI_CSR_LOG_ENABLE
    FORMAT_TRACE(CsrLogBuffer, DEBUG_BUFFER_SIZE, format);/*lint !e64 */
    SCI_TRACE_LOW("%s:%s", CsrLogPrefix, CsrLogBuffer); /*lint !e64 */
#endif
}

