/******************************************************************************
 ** File Name:      clock_hal_internal.h                                           *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************/
#ifndef _CLOCK_HAL_INTERNAL_H_
#define _CLOCK_HAL_INTERNAL_H_

#ifdef __cplusplus
extern   "C"
{
#endif

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
#define CLOCK_SUCCESS     0
#define CLOCK_FAIL        1

struct _CLOCK_OBJECT;
typedef struct _CLOCK_OBJECT *CLOCK_HANDLE;
struct _DEVICE_OBJECT;
typedef struct _DEVICE_OBJECT *DEVICE_HANDLE;
typedef uint32 CLOCK_STATUS;

/*****************************************************************************/
//  Description:    Init system clocks and devices structre
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS CLOCK_System_Init (void);

/*****************************************************************************/
//  Description:    Config all system clocks and devices
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS CLOCK_Config (void);

/*****************************************************************************/
//  Description:    Get device object
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Find (const char *Name, DEVICE_HANDLE *pDevObj);

/*****************************************************************************/
//  Description:    Open device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Open (DEVICE_HANDLE thiz);

/*****************************************************************************/
//  Description:    Reset device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Reset (DEVICE_HANDLE thiz);

/*****************************************************************************/
//  Description:    Close device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Close (DEVICE_HANDLE thiz);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClockAndDiv (DEVICE_HANDLE thiz, int ClockValue, int Div);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClock (DEVICE_HANDLE thiz, int ClockValue);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_EnableClock (DEVICE_HANDLE thiz);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_DisableClock (DEVICE_HANDLE thiz);

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClockDiv (DEVICE_HANDLE thiz, int Div);

/*****************************************************************************/
//  Description:    Get device's clock dividor
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC uint32 DEVICE_GetClockDiv (DEVICE_HANDLE thiz);

/*****************************************************************************/
//  Description:    Get device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC uint32 DEVICE_GetClock (DEVICE_HANDLE thiz);

#ifdef __cplusplus
}
#endif

#endif //_CLOCK_HAL_INTERNAL_H_

