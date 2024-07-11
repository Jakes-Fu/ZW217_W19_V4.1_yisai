
#ifndef _MMIHELLO_ID_H_
#define _MMIHELLO_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id

// ´°¿ÚID
typedef enum
{
    MATH_COUNT_WIN_ID_START = (MMI_MODULE_MATH_COUNT << 16),
	
	//win id
	MATH_COUNT_MAIN_WIN_ID,
    MATH_COUNT_COUNTING_WIN_ID,
    MATH_COUNT_SCORE_WIN_ID,
    MATH_COUNT_TASK_WIN_ID,
    MATH_COUNT_MAX_WIN_ID
}MMIMATH_COUNT_WINDOW_ID_E;

#undef WIN_ID_DEF

// ¿Ø¼þID
typedef enum
{
    MATH_COUNT_CTRL_ID_ID_START = MATH_COUNT_MAX_WIN_ID,

	//ctrl id
    MMI_MATH_COUNT_SET_TIME_ADD_BTN,
    MMI_MATH_COUNT_SET_TIME_REDUCE_BTN,
    MMI_MATH_COUNT_SET_RANGE_ADD_BTN,
    MMI_MATH_COUNT_SET_RANGE_REDUCE_BTN,

    MATH_COUNT_MAX_CTRL_ID
}MMIMATH_COUNT_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIHELLO_ID_H_
