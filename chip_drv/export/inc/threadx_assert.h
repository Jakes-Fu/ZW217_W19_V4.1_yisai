#ifndef THREADX_ASSERT_H__
#define THREADX_ASSERT_H__

// c standard head file.
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// Basic data types.
#include "sci_types.h"
#include "rtos_cfg.h"

#ifdef WIN32
// Include this file to replace windows.h to avoid compile error
#include "WINDOWS_sci_os.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*****************************************************************************/
//  Description:    The function is used to check if the system is in the assert mode
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsAssert (void);

/*****************************************************************************/
//  Description:    The function is used to check if dsp can run in the assert mode
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsDspRunInAssertMode (void);

/*****************************************************************************/
//  Description:    The function is used to get a global flag. The flag
//                  indicates whether SCI_Assert be called.
//  Global resource dependence:
//  Author:         rick.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_GetAssertFlag (void);

/*****************************************************************************/
//  Description:    The function is used to covert the assert information to char string
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC char *SCI_SaveAssertInfo (const char *x_format, ...);

/*****************************************************************************/
//  Description:    The function is used to print string about assert information
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PrintAssertString (char *x_format, ...);

/*****************************************************************************/
//  Description:    The function is used to register reading asseret Info Function.
//  Global resource dependence:
//  Author:         Johnson.sun
//  Note:
/*****************************************************************************/
PUBLIC void SCI_RegisterReadAssertInfoCmdRoutine (void);

/*****************************************************************************/
//  Description:    The SCI_HandleUndefined function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleUndefined (void);

/*****************************************************************************/
//  Description:    The SCI_HandleSwi function is used handle SWI.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleSwi (void);

/*****************************************************************************/
//  Description:    The SCI_HandlePrefetch function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandlePrefetch (void);

/*****************************************************************************/
//  Description:    The SCI_HandleAbort function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleAbort (void);

/*****************************************************************************/
//  Description:    The SCI_HandleReserved function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleReserved (void);

/*****************************************************************************/
//  Description:    The SCI_HandleWatchDog function is used handle watch dog.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleWatchDog (void);

/*****************************************************************************/
//  Description:    The SCI_SetAssertMode function used to set assert mode.
//                  If b_debug_mode is SCI_TURE, it prints assert information
//                  when assert; else reset.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_SetAssertMode (BOOLEAN b_debug_mode);

/*****************************************************************************/
//  Description:    The SCI_SetAssertMode function used to set assert mode.
//                  If b_debug_mode is SCI_TURE, it prints assert information
//                  when assert; else reset.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_SetRecordAssertFlag (BOOLEAN b_record_assert_flag);

/*****************************************************************************/
//  Description:    The function is used to print thread information to shell
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PrintThreadInfoToShell (void);

/*****************************************************************************/
//  Description:    The function is used to print stack information to shell
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PrintStackInfoToShell (void);

/*****************************************************************************/
//  Description:    The SCI_Assert function is typically used to identify logic
//                  errors during program development, by implementing the
//                  expression argumnet to evaluate to SCI_FALSE only when the
//                  program is operating incorrectly. After debugging is
//                  complete, assertion checking can be turned off without
//                  modifying the source file by deleting the predifinition
//                  _DEBUG.
//  Global resource dependence:
//  Author: Richard.Yang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_Assert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
);

/*****************************************************************************/
//  Description:    The SCI_PAssert function supports printing message and variables
//                  when asserting.
//  Author:     Bin.Lu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PAssert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
    char    *assert_info_ptr
);


/*****************************************************************************/
//  Description:    The SCI_DAssert function will provide debug MMI regardless of release mode
//                  Called by diag task receive assert cmd and other uplayer artificial assert
//  Global resource dependence:
//  Author: Richard.Yang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_DAssert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
);

/*****************************************************************************/
//  Description:    The SCI_SetDSPAssertFlag function used to set dsp assert flag.
//  Global resource dependence: is_dsp_assert
//  Author:         Fei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_SetDSPAssertFlag ( BOOLEAN flag );

/*****************************************************************************/
//  Description:    The SCI_IsDSPAssert function used to get dsp assert flag.
//  Global resource dependence: is_dsp_assert
//  Author:         Fei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsDSPAssert ( void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif
