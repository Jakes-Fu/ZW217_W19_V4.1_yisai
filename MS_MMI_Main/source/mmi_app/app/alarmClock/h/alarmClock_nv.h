

#ifndef _HELLO_NV_H_
#define _HELLO_NV_H_

#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

typedef enum
{
	MMINV_ALARMCLOCK_FLAG = MMI_MODULE_ALARMCLOCK << 16,

	MMINV_ALARMCLOCK_NUMBER_NV,

	MMINV_ALARMCLOCK_IS_DELAY_NV,

}ALARMCLOCK_NV_ITEM_E;



PUBLIC void MMI_RegAlarmClockNv(void);

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif