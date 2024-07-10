/******************************************************************************
 ** File Name:      gpio_prod_api.h                                           *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           12/24/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPIO  *
 					configure
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             		DESCRIPTION                       *
 ** 12/24/2007     Liangwen.Zhen		    Create.                           *
 ******************************************************************************/
#ifndef _GPIO_PROD_API_
#define _GPIO_PROD_API_
#include "tb_hal.h"
#include "sci_types.h"
#include "gpio_prod_cfg.h"
#include "spi_gpio.h"

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define GPIO_PROD_CALLBACK				GPIO_CALLBACK
#define GPIO_PROD_INT_TYPE				GPIO_INT_TYPE
typedef enum
{
    AUD_HEADSET_TYPE_PLUGOUT,
    AUD_HEADSET_TYPE_INSERT4P,
    AUD_HEADSET_TYPE_INSERT3P,
    AUD_HEADSET_TYPE_QTY = 0xFF000000
} AUD_HEADSET_TYPE_T;

/**---------------------------------------------------------------------------*
 **                         Data Structure 									  *
 **---------------------------------------------------------------------------*/

typedef void (*irq_handler_t)(uint32);
/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description:    This function is used to set gpio direction
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void _GPIO_PROD_SetDirection (GPIO_PROD_ID_E  id, BOOLEAN is_output);

//below four for specail use only
PUBLIC BOOLEAN GPIO_GetHookState(void);
PUBLIC void GPIO_EnableHookInt(BOOLEAN b_enable);
PUBLIC void GPIO_HookIntHandler(  uint32 gpio_id, uint32 gpio_state);
PUBLIC void GPIO_EnableHandFreeILed(BOOLEAN b_enable);

/*****************************************************************************/
//  Description:    This function is used to set udc driver reg isr handler
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_RegIntHandler(irq_handler_t irq_handler);

PUBLIC void GPIO_VBUS_RegIntHandler(irq_handler_t);

/*****************************************************************************/
//  Description:    This function is used to get gpio product information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPIO_CFG_INFO_T_PTR GPIO_PROD_GetCfgInfo(GPIO_PROD_ID_E id);

/*****************************************************************************/
//  Description:    This function is used to set gpio product information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PROD_SetCfgInfo(GPIO_CFG_INFO_T_PTR cfg_info_ptr);

/*****************************************************************************/
//  Description:    This function is used to initialize GPIO  about customer setting
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_CustomizeInit(void);

/*****************************************************************************/
//  Description:    This function turn on/off the KeyPad backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:           KeyPad backlight is controlled by GPIO13 on SM5100B EVB1.0
/*****************************************************************************/
PUBLIC void GPIO_SetKeyPadBackLight(BOOLEAN b_on );

/*****************************************************************************/
//  Description:    This function control lcd backlight
//                  is_high = SCI_TRUE, gpio in high level
//                  is_high = SCI_FALSE, gpio in low level
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetLcdBackLightness(BOOLEAN is_high);

/*****************************************************************************/
//  Description:    This function turn on/off the LCD1 backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:           LCD1 backlight is controlled by GPIO7 on SM5100B EVB1.0
/*****************************************************************************/
PUBLIC void GPIO_SetLcdBackLight(BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function set BackLight.
//                  b_light = SCI_TRUE   turn on LED
//                  b_light = SCI_FALSE  turn off LED
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetBackLight(BOOLEAN b_light);

/*****************************************************************************/
//  Description:    This function set the state of vibrator.
//
//  Input      :
//      b_on   : If true, start vibrate. If false, stop it.
//  Return     : None
//  Author     : Lin.liu
//	Note       :
/*****************************************************************************/
PUBLIC void GPIO_SetVibrator(BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function set the state of vibrator.
//
//  Input      :
//      b_on   : If true, start vibrate. If false, stop it.
//      on_period   : unit: ms, only for TURN ON
//      on_stress   : reserved for future use
//      reserved    : reserved for future use
//  Note       :
/*****************************************************************************/
PUBLIC void GPIO_SetVibrator_Ext(BOOLEAN b_on, uint32 on_period, uint32 on_stress, uint32 *reserved);

/*****************************************************************************/
//  Description:    This function query the state of Flip
//                  return value = SCI_TRUE, flip is open
//                  return value = SCI_FALSE, flip is close
//  Author:         Zhemin.Lin
//  Note:
//                  Lin.liu.   don't use this file. (only for compatible with prev version)
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetFlipState(void);

/*****************************************************************************/
//  Description:    This function query the state of Slide
//                  return value = SCI_TRUE, Slide is open
//                  return value = SCI_FALSE, Slide is close
//  Author:         Liangwen.zhen
//  Note:
//                  Lin.liu.   don't use this file. (only for compatible with prev version)
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetSlideState(void);

/*****************************************************************************/
//  Description:    This function check the headset detect pin status.
//
//  Input      : None
//  Return     :
//      True   : the headset is in.
//      False  : the headset is out.
//  Author     : Lin.liu
//	Note       : When call this function, make sure the gpio has been config correctly.
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_CheckHeadsetStatus( void );

/*****************************************************************************/
//  Description:    This function check the SDCard detect pin status.
//
//  Input      : None
//  Return     :
//      True   : the SDCard is in.
//      False  : the SDCard is out.
//  Author     : yuehz
//	Note       : 20060410;when call this function, make sure the gpio has been config correctly.
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_CheckSDCardStatus( void );

/*****************************************************************************/
//  Description:    This function set enable/disable amplifier.
//                  b_enable = SCI_TRUE   Enable amplifier
//                  b_enable = SCI_FALSE  Disable amplifier
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableAmplifier(BOOLEAN b_enable);

/*****************************************************************************/
//  Description:    This function set enable/disable AudioCodec.
//                  b_enable = SCI_TRUE   Enable AudioCodec
//                  b_enable = SCI_FALSE  Disable AudioCodec
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableCodec (BOOLEAN b_enable);

/*****************************************************************************/
//  Description:    This function set enable/disable Micbias.
//                  b_enable = SCI_TRUE   Enable Micbias
//                  b_enable = SCI_FALSE  Disable Micbias
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableMicBias (BOOLEAN b_enable);

/*****************************************************************************/
//  Description:    This function turn on/off the LCD1 backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:           LCD1 backlight is controlled by GPIO7 on SM5100B EVB1.0
/*****************************************************************************/
PUBLIC void GPIO_SetLcd1BackLight(BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function turn on/off the Flash .
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetFlash(BOOLEAN b_on );

/*****************************************************************************/
//  Description:    This function set VCM enable.
//                  b_on = SCI_TRUE, High
//                  b_on = SCI_FALSE, Low
//  Author:        Hansen.sun
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetVCMEnable (BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function turn on/off the Flash .
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetDCFlash (BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function set sensor reset signal .
//                  pulse_level = SCI_TRUE, High
//                  pulse_level = SCI_FALSE, Low
//  Author:         Benny.Zou
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_ResetSensor (
		BOOLEAN pulse_level,
		uint32  pulse_width
		);

/*****************************************************************************/
//  Description:    This function set sensor pwdn .
//                  b_on = SCI_TRUE, High
//                  b_on = SCI_FALSE, Low
//  Author:         Benny.Zou
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetSensorPwdn (BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function set front sensor pwdn .
//                  b_on = SCI_TRUE, High
//                  b_on = SCI_FALSE, Low
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetFrontSensorPwdn (BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function set sensor reset signal .
//                  pulse_level = SCI_TRUE, High
//                  pulse_level = SCI_FALSE, Low
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetSensorResetLevel (BOOLEAN pulse_level);

/*****************************************************************************/
//  Description:    This function set the state of flash light.
//
//  Note       :
/*****************************************************************************/
PUBLIC void GPIO_SetFlashLight(BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function set analog tv reset signal .
//                  pulse_level = SCI_TRUE, High
//                  pulse_level = SCI_FALSE, Low
//  Author:         Tim.zhu
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetAnalogTVResetLevel (BOOLEAN pulse_level);

/*****************************************************************************/
//  Description:    This function get charge interrupt gpio number
//  Author:         Benny.Zou
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_GetChargeIntGpio(void);

/*****************************************************************************/
//  Description:    This function enable SPI CS select SD
//
//  Author:        yuehz
//  Note:           20060512 b_level :0 enable;1:disable
/*****************************************************************************/
PUBLIC void GPIO_SetSDCS(BOOLEAN b_enable);

/*****************************************************************************/
//  Description:    This function selects headset mic or tv out in DVB
//                       gpio_state=Low:headset mic;High: TV Out
//  Author:        yuehz
//  Note:           20060628
/*****************************************************************************/
PUBLIC void GPIO_Sel_MIC_TVOut(BOOLEAN gpio_state);

/*****************************************************************************/
//  Description:    This function Open PA or not
//
//  Author:        yuehz
//  Note:           20060627
/*****************************************************************************/
PUBLIC void GPIO_OpenFMPA(BOOLEAN is_open);

/*****************************************************************************/
//  Description:    This function set GPIO which controls the motor
//
//  Author:        Jianping.wang
//  Note:
/*****************************************************************************/
void GPIO_MOTOR_SetPortValue (
								uint16 ic_num,		// define in gpio_dvb_ext.h
								uint16 gpio_mask,	// when the related bit is '1', use value of related bit in gpio_value to set gpio output value
								uint16 gpio_value	// represent 16 gpio value
							 );

/*****************************************************************************/
//  Description:    This function set enable/disable amplifier.
//                  b_enable = SCI_TRUE   Enable amplifier
//                  b_enable = SCI_FALSE  Disable amplifier
//  Author:         Liangwen.Zhen
//	Note:
/*****************************************************************************/
PUBLIC void GPIO_EnableHeadsetAmplifier(BOOLEAN b_enable);

/*****************************************************************************/
//  Description:    This function control SD CARD Power ;
//                  is_open = SCI_TRUE, 	Low
//                  is_open = SCI_FALSE, 	High
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_OpenSDPower(BOOLEAN is_open);

/*****************************************************************************/
//  Description:    This function detect usb or adapter ,
//  then config the GPIO as GPIO input.
//  before call this function,you must config the gpio input in pinmap_**.c.
//  Author:         junyi.lv
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_DetectUsbOrAdapter(void);

/*****************************************************************************/
//  Description:    This function select spi2 extend cs

//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SelectSPI2EXTCS(uint32 cs_num);

/*****************************************************************************/
//  Description:    This function set RF transceiver reset GPIO output high,
//  then config the GPIO as GPIO input.
//  before call this function,you must config the gpio output low in pinmap_**.c.
//  Author:         Younger.yang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_ConfigRfReset( void );

/*****************************************************************************/
//  Description:    This function set BT set pin status
//  then config the GPIO as GPIO input.
//  before call this function,you must config the gpio output low in pinmap_**.c.
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetBtReset(BOOLEAN is_high);

/*****************************************************************************/
//  Description:    This function is used to Register product gpio
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
BOOLEAN GPIO_PROD_RegGpio(
					GPIO_PROD_ID_E 		id,
					BOOLEAN 			direction,		// SCI_FALSE: Input dir; SCI_TRUE: output dir
					BOOLEAN 			default_value,	//
					BOOLEAN 			shaking_en,
					uint32				shaking_interval,
					GPIO_PROD_CALLBACK	gpio_callback_fun
					   	);

/*****************************************************************************/
//  Description:    This function is used to get gpio product information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PROD_InitCfgTable(void);

/*****************************************************************************/
//  Description:    PowerKey detection handler function.
//  Author:         BenjaminWang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PowerKeyIntHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    This function is used to set GPS module idle time
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_ChargeIntHandler(uint32 gpio_id, uint32 gpio_state);

#ifdef BATTERY_DETECT_SUPPORT
/*****************************************************************************/
//  Description:    This function is used to set GPS module idle time
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_BatDetIntHandler(uint32 gpio_id, uint32 gpio_state);
#endif

/*****************************************************************************/
//  Description:    GPIO extend intrrupt handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_GpioExtendINTHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    FlipOn detection handler function.
//  Author:         BenjaminWang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_FlipOnIntHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    Slide On detection handler function.
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SlideOnIntHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    Touch panel detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_TPDetectIntHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    PowerKey detection handler function.
//  Author:         BenjaminWang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_BackendICIntHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    SDCard detection handler function.
//  Author:         juan.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SdcardDetectIntHandler(  uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    Customer 1 detection handler function.
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_Cus1DetectIntHandler(  uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    Customer 2 detection handler function.
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_Cus2DetectIntHandler(  uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    This function is used to get the gpio data for expand key
//  Author:         hanjun.liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_GetExpandKeyHandler( void);

/*****************************************************************************/
//  Description:    Camera cover detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_CameraCoverDetectHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    LCD Frame mark detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_LCDFrameMarkDetectHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    Double key first action detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DKeyFirstActionHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    Double key secord action detection handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DKeySecordActionHandler( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    This function is used to handler g-sensor interrupt
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_GSensorIntHandler(uint32 gpio_id, uint32 gpio_state);
#ifdef ZDT_NFC_SUPPORT
PUBLIC void GPIO_ZDT_NFCIntHandler(uint32 gpio_id, uint32 gpio_state);
PUBLIC void GPIO_SetNFC_LDO_PVDD_EN (BOOLEAN b_on);
PUBLIC void GPIO_SetNFC_LDO_SIM_EN (BOOLEAN b_on);
PUBLIC void GPIO_SetNFCDWL (BOOLEAN b_on);
PUBLIC BOOLEAN GPIO_GetNFCDWL (void);
PUBLIC void GPIO_SetNFCEnable (BOOLEAN b_on);
PUBLIC BOOLEAN  GPIO_GetNFCIRQ(void);
#endif
#ifdef ZDT_PCBA_ZW31_SUPPORT
PUBLIC void GPIO_ZDT_GSensorIntHandler(uint32 gpio_id, uint32 gpio_state);
PUBLIC void GPIO_ZDT_GSensorIRQCtrl(BOOLEAN is_active);
PUBLIC void GPIO_ZDT_HallIntHandler(uint32 gpio_id, uint32 gpio_state);
#ifdef ZDT_HSENSOR_SUPPORT
PUBLIC void GPIO_ZDT_HSensorIntHandler (uint32 gpio_id, uint32 gpio_state);
#endif
#ifdef ZDT_GPS_SUPPORT
PUBLIC void GPIO_SetGPSPower (BOOLEAN b_on);
#endif
#endif

/*****************************************************************************/
//  Description:    Share bluetooth clock.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_BTIntHandler (uint32 gpio_id,        uint32 gpio_state);

/*****************************************************************************/
//  Description:    wifi interrupt handler function.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_WiFiIntHander( uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    This function is used to handle Micorusb handler
//  Author:        yun.wang
//  Note:           2010-11-23
/*****************************************************************************/
PUBLIC void GPIO_MicroUSBIntHandler(uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    This function is used to control the sensor power if the
//                  the sensor is enabled by a gpio.
//                  b_on = SCI_TRUE, open sensor power
//                  b_on = SCI_FALSE, close sensor power
//  Author:         junyi.lv
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetSensorPower (BOOLEAN b_on);

/******************************************************************************/
// Description:   PULL UP/DOWN RESET PIN TO FORCE WIFI TO ENTER POWERDOWN MODE
// Dependence:
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC void GPIO_SetWifiPowerDown(BOOLEAN is_powerdown);

/*****************************************************************************/
//  Description:    This function handle demod power
//  Author:
//  Note:           pulse_level
/*****************************************************************************/
PUBLIC void GPIO_DemodPower( BOOLEAN is_poweron );

/*****************************************************************************/
//  Description:    This function handle demod reset
//  Author:
//  Note:           pulse_level
/*****************************************************************************/
PUBLIC void GPIO_DemodReset( BOOLEAN pulse_level );

/*****************************************************************************/
//  Description:    demod int handler
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_DemodIntHandler( uint32 gpio_id, uint32 gpio_state);


/*****************************************************************************/
//  Description:    This function is used to set product gpio value
//  Author:         Liangwen.Zhen
//  Note:           First call callback funtion, second set gpio value
/*****************************************************************************/
PUBLIC BOOLEAN _GPIO_PROD_SetVal (GPIO_PROD_ID_E id, BOOLEAN value);

/*****************************************************************************/
//  Description:    This function dot lcd reset
//  before call this function,you must config the gpio output low in pinmap_**.c.
//  Author:
//  Note:           pulse_level: pin level,
//                  delay_ms:  reset pulse time
/*****************************************************************************/
PUBLIC void GPIO_ResetLcd (BOOLEAN pulse_level, uint32  delay_ms);


/*****************************************************************************/
//  Name:           GPIO_TPIRQCtrl
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC void GPIO_TPIRQCtrl(BOOLEAN is_active);

/*****************************************************************************/
//  Name:           GPIO_ResetTP
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC void GPIO_ResetTP(BOOLEAN is_on,uint32 delay_ms);
/*****************************************************************************/
//  Name:           GPIO_TPInterruptSense
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC void GPIO_TPInterruptSense(uint32 status);
/*****************************************************************************/
//  Name:           GPIO_TPInterruptPin
//  Description:    This function set the interrupt pin. Because some tp chip
//                  supports to trigger manual.
/*****************************************************************************/
PUBLIC uint16 GPIO_TPInterruptPin(void);
/*****************************************************************************/
//  Name:           GPIO_TPGetInterruptStatus
//  Description:    This function get valid interrupt level.
/*****************************************************************************/
PUBLIC uint16 GPIO_TPGetInterruptStatus(void);

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//  Description:    SIM card hot swap feature.
//  Input      :
//  Return     :    None
//  Author     :    wuding.yang
//	Note       :
/*****************************************************************************/
PUBLIC void GPIO_SIMIntHandler(uint32 gpio_id, uint32 gpio_state);
#endif

/*****************************************************************************/
//  Description:    Get the GPIO Id which is connected with the hot swap sim slot.
//  Input      :
//  Return     :    None
//  Author     :    wuding.yang
//	Note       :
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_SIM_Hotswap_GetGPIOId(uint32 *gpio_id);

#ifdef GPIO_SIMULATE_SPI_SUPPORT
/*****************************************************************************/
//  Description:    Simulate a SPI by GPIOs
//  Author:        yun.wang
//  Note:           2010-11-23
/*****************************************************************************/
PUBLIC uint32 GPIO_SPI_id_get(GPIO_SPI_PIN_ID_E gpio_spi_pin);
#endif

/*****************************************************************************/
//  Description:    Get the GPIO Id or SPI which is connected to 6302 for quad-sim platform
//  Input      :
//  Return     :    spi logic id
//  Author     :    wuding.yang
//	Note       :
/*****************************************************************************/
PUBLIC uint32 GPIO_SPI_Quadsim_Get_Config(uint8 *which_sim_connect_6302);

#ifdef DUAL_BATTERY_SUPPORT

PUBLIC void GPIO_DBAT_AuxBatDetectIntHandler(uint32 gpio_id, uint32 gpio_state);

#endif //DUAL_BATTERY_SUPPORT

PUBLIC void GPIO_SPIIRQCtrl(BOOLEAN is_active);

/*****************************************************************************/
//  Description:    This function is used to process usb resume signal.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/

PUBLIC void GPIO_USBResumeHandler (uint32 gpio_id, uint32 gpio_state);

/*****************************************************************************/
//  Description:    This function is used to handler ofn-sensor interrupt
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_OFNIntHandler(uint32 gpio_id, uint32 gpio_state);

#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
/*****************************************************************************/
//  Description:    This function is used to get wifi spi cs id
//  Author:         Chi.Chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 WIFI_GetSPIBusLogicID(void);

/*****************************************************************************/
//  Description:    This function is used to get cmmb spi cs id
//  Author:         Chi.Chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 CMMB_GetSPIBusLogicID(void);

/*****************************************************************************/
//  Description:    This function is used to get spi switch default voltage
//  Author:         Chi.Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetSwitchDefaultValue(void);

/*****************************************************************************/
//  Description:    This function is used to get spi switch wifi control voltage
//  Author:         Chi.Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetSwitchWiFiValue(void);

/*****************************************************************************/
//  Description:    This function is used to get spi switch cmmb control voltage
//  Author:         Chi.Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetSwitchCMMBValue(void);

/*****************************************************************************/
//  Description:    Switch SPI Bus by Logic Id
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SPISwitch(uint32 * logic_id);

/*****************************************************************************/
//  Description:    Set GPIO to Switch SPI Bus to WiFi or CMMB
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_SetSPISwitch(GPIO_SPI_SW_E sw_type);
#endif

#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description:    Handle AP wake up BB
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_APWakeUpBBHandler(uint32 gpio_id, uint32 status);

/*****************************************************************************/
//  Description:    Handle AP wake up BB
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_WakeUpAP(BOOLEAN is_wake_up);

/*****************************************************************************/
//  Description:    Handle AP wake up BB
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetAPSleepStatus(void);
#endif

#if defined(PLATFORM_UWS6121E)
PUBLIC void GPIO_R2720IntHandler(uint32 gpio_id, uint32 gpio_state);
#endif

PUBLIC void GPIO_SetFmLNA(BOOLEAN is_high);
PUBLIC void GPIO_TEST_IRQ(uint32 gpio_id, uint32 gpio_state);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of gpio_prod_api.h

#endif // _GPIO_PROD_API_
