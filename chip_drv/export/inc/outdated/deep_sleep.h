/******************************************************************************
 ** File Name:      deep_sleep.h                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/25/2003                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for deep sleep.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/25/2003     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _DEEP_SLEEP_H_
#define _DEEP_SLEEP_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "com_drvapi.h"
#include "fiq_drvapi.h"
#include "chip.h"
#ifndef WIN32
#include "tx_api_thumb.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"


#endif //WIN32
#ifdef   __cplusplus
}
#endif
#endif // _DEEP_SLEEP_H_


