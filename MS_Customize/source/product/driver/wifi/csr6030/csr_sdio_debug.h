#ifndef CSR_SDIO_DEBUG_H__
#define CSR_SDIO_DEBUG_H__
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include <stdio.h>

#include "csr_types.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_panic.h"

#include "os_api.h"

#define DEBUGLEVEL 0
#define DEBUG_VERBOSE_FLAG 0

#ifndef DEBUG_VERBOSE_FLAG
#define DEBUG_VERBOSE_FLAG 1
#endif

#ifndef DEBUGLEVEL
#define DEBUGLEVEL 1
#endif

#if DEBUGLEVEL > 0
#if DEBUG_VERBOSE_FLAG == 1
#define CSR_SDIO_PRINTF(x) SCI_TRACE_LOW("[%s:%u] ", __FILE__, __LINE__); SCI_TRACE_LOW x;

#else
#define CSR_SDIO_PRINTF(x) SCI_TRACE_LOW x 
#endif

#else /* DEBUGLEVEL > 0 */

#define CSR_SDIO_PRINTF(x)

#endif /* DEBUGLEVEL > 0 */

#define CSR_SDIO_WARN(x) SCI_TRACE_LOW("[%s:%u] ", __FILE__, __LINE__); SCI_TRACE_LOW x;

/* --------------------------------- */
/*          Asserts                 */
/* --------------------------------- */
#define CSR_SDIO_STRINGIFY_REAL(a) #a
#define CSR_SDIO_STRINGIFY(a) CSR_SDIO_STRINGIFY_REAL(a)

#define CSR_SDIO_ASSERT(cond, str)              \
do {                                                \
    if (!(cond))                                    \
    {                                               \
        char *panic_arg = "[" __FILE__ ":" CSR_SDIO_STRINGIFY(__LINE__) "] - " CSR_SDIO_STRINGIFY(cond); \
        char *panic_str = "[" __FILE__ ":" CSR_SDIO_STRINGIFY(__LINE__) "] - " str; \
        char *panic = CsrPmemZalloc(CsrStrLen(panic_arg) + CsrStrLen(panic_str) + 3); \
        CsrStrCat(panic, panic_arg);                                                \
        CsrStrCat(panic, "\n");                                                     \
        CsrStrCat(panic, panic_str);                                                \
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_ASSERTION_FAIL, panic);                  \
        CsrPmemFree(panic);                                                            \
    }                                                                               \
} while(0)

#endif

