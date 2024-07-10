#ifndef __RTMP_DEMP_ID_H
#define __RTMP_DEMP_ID_H 

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
 
#define WIN_ID_DEF(win_id, win_id_name) win_id,
 
typedef enum
{
    MMI_RTMP_WIN_ID_START = (MMI_MODULE_RTMPDEMO << 16),

#include "rtmp_id.def"

    MMI_RTMP_WIN_ID_MAX
} MMI_RTMP_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMI_RTMP_CTRL_ID_START = MMI_RTMP_WIN_ID_MAX,

	// label
	MMI_RTMP_CTRL_ID_LABEL_TITLE,
	// list

	// progress

	// button
	MMI_RTMP_CTRL_ID_BTN_PLAY,
	MMI_RTMP_CTRL_ID_BTN_STOP,

	// image

    MMI_RTMP_CTRL_ID_MAX
} MMI_RTMP_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //__RTMP_DEMP_ID_H