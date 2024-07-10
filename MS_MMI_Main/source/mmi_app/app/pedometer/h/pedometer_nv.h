

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
	MMINV_PEDOMETER_FLAG = MMI_MODULE_PEDOMETER << 16,

	MMINV_PEDOMETER_DATE_NV,

	MMINV_PEDOMETER_PRAISE_NV,

	MMINV_PEDOMETER_DAY1_STEP_NV,
	MMINV_PEDOMETER_DAY2_STEP_NV,
	MMINV_PEDOMETER_DAY3_STEP_NV,
	MMINV_PEDOMETER_DAY4_STEP_NV,
	MMINV_PEDOMETER_DAY5_STEP_NV,
	MMINV_PEDOMETER_DAY6_STEP_NV,
	MMINV_PEDOMETER_DAY7_STEP_NV,


}PEDOMETER_NV_ITEM_E;



PUBLIC void MMI_RegPedometerNv(void);

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