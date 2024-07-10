/******************************************************************************
 ** File Name:      tb_hal.h                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           10/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic input and output operations   *
 **                 on hardware, it can be treated as a hardware abstract     *
 **                 layer interface.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2002     Xueliang.Wang    Create.                                   *
 ** 09/24/2003     Xueliang.Wang    Modify, CR4916.                           *
 **                                 Add audio and chip.                       *
 ******************************************************************************/

#ifndef _TB_HAL_H_
#define _TB_HAL_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sc_reg.h"
#include "os_api.h"
#include "isr_drvapi.h"

#ifndef WIN32
#include "uart_drvapi.h"
#include "sio.h"
#endif

#include "com_drvapi.h"
#include "tpc_drvapi.h"
#include "wdg_drvapi.h"
#include "gpio_drv.h"
#include "rtc_drvapi.h"
#include "timer_drvapi.h"
#include "pwm_drvapi.h"
#include "adc_drvapi.h"
#include "kpd_drvapi.h"
#include "chip.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  API mappings.
/*****************************************************************************/
#define HAL_Init()              do{             \
        ISR_Init(); \
    }while(0)
//#define HAL_IsrHandler          ISR_HandlerAll
#define HAL_RegCallback         ISR_RegCallback
//#define HAL_UnRegCallback       ISR_UnRegCallback

// Timer API.
#define HAL_InitTimer           TIMER_Init
#define HAL_SetTime             TIMER_Set
//#define HAL_GetRemainingTime    TIMER_GetRemaining
#define HAL_GetTickCount        TIMER_GetTickCount
//#define HAL_DisableTimer        TIMER_DisableTimer
//#define HAL_ChangeTimerClk      TIMER_ChangeClk
#define HAL_GetSystemCounterReg TIMER_GetSystemCounterReg

//Chng Freq API
#define HAL_SetMCUClk              CHNG_FREQ_SetArmClk
#define HAL_RestoreMCUClk       CHNG_FREQ_RestoreARMClk

// CHIP API
#define HAL_GetHWChipID         CHIP_GetHWChipID

#define HAL_GetMCUClk           CHIP_GetArmClk
#define HAL_TurnOffPower        CHIP_TurnOffPower
#define HAL_MCUReset            CHIP_ResetMCU
#define HAL_MCUBootReset        CHIP_BootReset
//#define HAL_GetPLLClk           CHIP_GetPllClk

//#define HAL_GetOutputClock      CHIP_GetOutputClock
//#define HAL_SelectOutputClock   CHIP_SelectOutputClock


// Keypad API.
#define HAL_InitKpd             KPD_Init
#define HAL_CloseKpd            KPD_Close

//"Uart I/O pin interrupt" API
//#define HAL_UartDSlp_Init UartDSlp_Init

// Watch dog API
#define HAL_WDG_STOP            WDG_TimerStop
//#define HAL_WDG_START           WDG_TimerStart
#define HAL_WDG_INIT            WDG_TimerInit
#define HAL_WDG_LOAD            WDG_TimerLoad
//#define HAL_WDG_HANDLE          WDG_TimerHandler
// GIS API

#define HAL_SDRAM_InitPara             SDRAM_InitPara

void FIQ_Init (void);

#ifdef __cplusplus
}
#endif

#endif  // _TB_HAL_H

// End of tb_hal.h
