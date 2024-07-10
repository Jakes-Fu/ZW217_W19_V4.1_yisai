/******************************************************************************
 ** File Name:      Clock.h                                           *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************/

#ifndef _CLOCK_H_
#define _CLOCK_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define _CLOCK_DEBUG
#ifdef _CLOCK_DEBUG
#include "sci_log.h"

#define CLOCK_ASSERT(_x_)               SCI_ASSERT(_x_)
#define CLOCK_VERIFY(_x_)               SCI_ASSERT(_x_)
#define CLOCK_ERR_TRACE(_x_)            SCI_TraceLow _x_
#define CLOCK_LOG_TRACE(_x_)            SCI_TraceLow _x_
#define CLOCK_INF_TRACE(_x_)            SCI_TraceLow _x_
#else
#define CLOCK_ASSERT(_x_)               ((void)0)
#define CLOCK_VERIFY(_x_)               ((void)(_x_))
#define CLOCK_ERR_TRACE(_x_)            ((void)0)
#define CLOCK_LOG_TRACE(_x_)            ((void)0)
#define CLOCK_INF_TRACE(_x_)            ((void)0)
#endif /* _CLOCK_DEBUG */

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
#define MAX_DEVICE_NAME_LEN     32

typedef void (*OPEN) (void);
typedef void (*CLOSE) (void);
typedef void (*RESET) (void);
typedef void (*SRCSET) (const char *);
typedef void (*DIVSET) (int);

/******************************************************************************
 * Structure definitions
 ******************************************************************************/
typedef struct _CLOCK
{
    const char  Name[MAX_DEVICE_NAME_LEN];
    uint32      ClockValue;                         /* Clock value */
    const char  DefaultSrc[MAX_DEVICE_NAME_LEN];    /* Default source clock */
    BOOLEAN     bOpen;                              /* Default open status */

    OPEN        Open;
    CLOSE       Close;
} CLOCK;

typedef struct _DEVICE
{
    const char  Name[MAX_DEVICE_NAME_LEN];
    uint32      ClockValue;                         /* Clock value */
    const char  ParentDev[MAX_DEVICE_NAME_LEN];     /* Parent device */
    uint32      Div;                                /* Divider of work clock source */
    BOOLEAN     bEnable;                            /* Default enable status */

    OPEN        Open;
    CLOSE       Close;
    SRCSET      SrcSet;
    DIVSET      DivSet;
    RESET       Reset;
} DEVICE;

typedef struct _CLOCK_CFG
{
    CLOCK      *Clock;
    int        ClockNr;
    DEVICE     *Device;
    int        DeviceNr;
} CLOCK_CFG;

PUBLIC void CLOCKCFG_Get (CLOCK_CFG *thiz);
#endif

