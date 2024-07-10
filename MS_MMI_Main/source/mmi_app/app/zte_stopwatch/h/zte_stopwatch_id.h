#ifndef _STOPWATCH_ID_H_
#define _STOPWATCH_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmk_type.h"
#include "os_api.h"
#include "sci_types.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id) win_id

// window ID
typedef enum {
  STOPWATCH_WIN_ID_START = (MMI_MODULE_ZTE_STOPWATCH << 16),
  STOPWATCH_WIN_ID,
 

  STOPWATCH_MAX_WIN_ID
} STOPWATCH_WINDOW_ID_E;

typedef enum {
  STOPWATCH_CTRL_ID_START = STOPWATCH_MAX_WIN_ID,
  STOPWATCH_CTRL_START,
  STOPWATCH_CTRL_STOP,
  STOPWATCH_CTRL_RESET,
  STOPWATCH_CTRL_TIME,
  /**/

  STOPWATCH_MAX_CTRL_ID
} STOPWATCH_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  //_STOPWATCH_ID_H_
