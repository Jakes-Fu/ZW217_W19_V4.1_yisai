/******************************************************************************
 ** File Name:      deepsleep_drvapi.h                                               *
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
#ifndef _DEEPSLEEP_DRVAPI_H_
#define _DEEPSLEEP_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "pwm_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define ENABLE_DEEP_SLEEP       1
#define DISABLE_DEEP_SLEEP      0

#define ENABLE_AHB_SLEEP        1
#define DISABLE_AHB_SLEEP       0

#define ENABLE_PM0_ENG		BIT_0
#define ENABLE_PM1_ENG		BIT_0 |BIT_1
#define ENABLE_DISABLE_ENG		0
typedef enum
{
    UART0_E = 0,
    UART1_E,
    UART2_E,
    UART3_E,
    UART_MAX
} UART_PORT_ID_E;

typedef enum
{
    DISABLE_APB_SLEEP = 0,
    ENABLE_APB_SLEEP
} UART_DSLP_CTRL_E;

typedef enum
{
    ENABLE_PM0,
    ENABLE_PM1,
    DISABLE_SLEEP,
	ENABLE_PM2,
	ENABLE_PM3,
    SLEEP_TYPE_MAX
} SLP_SLEEP_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC void SCI_UPM_EnableDeepSleep (uint32 mode);
PUBLIC void SLEEP_Init (void);
//@CR97469 jiexia.yu add to protect DMA transfer when in deepsleep
PUBLIC void SCI_DMA_EnableDeepSleep (uint32 mode);
//@CR97469 end
PUBLIC void DPSLP_PWM_EnableApbSleep (PWM_PORT_ID_E port_id, uint32 mode);
/*****************************************************************************/
//  Description:    This function is used to config the apb clk request of uart.
//  Author:         Zhengjiang.Lei
//  Note:
//****************************************************************************/
PUBLIC void DPSLP_UART_EnableApbSleep (
    UART_PORT_ID_E port_id,
    uint32 mode
);
// Enable Audio deep sleep
PUBLIC void SCI_AUD_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_LINEIN_EnableDeepSleep (uint32 mode);

/*****************************************************************************/
//  Description:    This function is dsp fiq handler
//  Author:
//  Note:
//****************************************************************************/
PUBLIC void DSP_FiqHandler (
    uint32 fiq_num      // fiq number
);

PUBLIC uint32 RTC_MODE_get (void);

PUBLIC void RTC_MODE_init (void);

uint32 SLEEP_GetAHBModuleStatus (void);
uint32 SLEEP_GetAPBModuleStatus (void);


PUBLIC void SCI_TPC_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_PCM_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_SPI_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_FM_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_MP4_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_RPC_EnableDeepSleep (uint32 mode);

// Enable LCD backlight brightless deep sleep.
// For the brightless is controlled by PWM, if enter deepsleep, the mcu clk stop,
// and the PWM also stop.
PUBLIC void SCI_LCDBacklightBrightless_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_INTC_EnableDeepSleep (uint32 mode);

//
// Set charge deepsleep mode
//
PUBLIC void SCI_CHR_EnableDeepSleep (uint32 mode);
//enable tv deepsleep
PUBLIC void SCI_TV_EnableDeepSleep (uint32 mode);
// Enable MMI deep sleep
PUBLIC void SCI_MMI_EnableDeepSleep (uint32 mode);
// Enable SIM deep sleep
PUBLIC void SCI_SIM_EnableDeepSleep (uint32 mode);
// Enable Layer1 deeep sleep
PUBLIC void SCI_L1_EnableDeepSleep (uint32 mode);
// Enable Bluetooth deep sleep
PUBLIC void SCI_BT_EnableDeepSleep (uint32 mode);
// Get DSP absolute time
PUBLIC uint32 SCI_GetDSPAbsoluteTime (void);
// wifi
PUBLIC void SCI_WIFI_EnableDeepSleep (uint32 mode);

// Init delta time between DSP and MCU
PUBLIC void SCI_InitDSPDeltaTime (void);

// Check enter sleep mode
PUBLIC void SCI_Enter_Pm (void);


PUBLIC void DS_CreateAbsoluteTimer (uint32 abs_time);

PUBLIC void deep_sleep_delay (uint32 count);

PUBLIC void SCI_BTCLK_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_DCAM_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_ROT_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_MIDI_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_Calibration_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_SDIO_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_UPM_EnableDeepSleep (uint32 mode);
PUBLIC unsigned int Slp_Get_WakeupTimer_Ms(void);
/*****************************************************************************/
//  Description:    This function is used to config the apb clk request of pwm.
//  Author:         Younger.Yang
//  Note:
//****************************************************************************/
PUBLIC void DPSLP_Lcdc_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_MpegEnc_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_MpegDec_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_ISP_EnableAhbSleep (uint32 mode);//just used for PLATFORM_SC6800H
PUBLIC void DPSLP_IVSP_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_DMA_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_SetBklight_Sts (BOOLEAN bklight_on);


/*****************************************************************************/
// Description :    This function is used to get the val of sleepmode.
// Global resource dependence :sleep_mode
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 SCI_Getsleepmode (void);

/*****************************************************************************/
// Description :    This function is used to set the flag which indicate if xtl need open in deepsleep
// Global resource dependence :s_sleep_xtl_open_cnt
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC void DPSLP_XTLOpenInSleep (
    BOOLEAN is_open                         //indicate if xtl need open in deepsleep,
    //SCI_TRUE: need open xtl in deepsleep; SCI_FALSE: can close xtl in deepsleep
);

/*****************************************************************************/
// Description :    This function is used to enable or disable rf
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void DSLP_RFLDOControl (
    BOOLEAN enable          // TRUE:enable, FALSE:disable
);

/*****************************************************************************/
// Description :    This function is used to get rf enable flag
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 DSLP_GetRFEnableFlag (void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //

#endif // _DEEPSLEEP_DRVAPI_H_
