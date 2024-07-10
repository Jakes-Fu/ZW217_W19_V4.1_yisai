/******************************************************************************
 ** File Name:      gpio_prod_cfg.h                                           *
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
#ifndef _GPIO_PROD_CFG_
#define _GPIO_PROD_CFG_

#include "sci_types.h"
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
#define GPIO_PROD_NUM_INVALID		0xFFFF

#define GPIO_PROD_HIGH_LEVEL		0x01
#define GPIO_PROD_LOW_LEVEL			0x00
/**---------------------------------------------------------------------------*
 **                         Data Structure 									  *
 **---------------------------------------------------------------------------*/
 typedef enum
 {
	GPIO_PROD_TEST_ID,
	GPIO_PROD_RF_RESET_ID,
 	GPIO_PROD_DSP_RESET_ID,
 	GPIO_PROD_DSP_HLOD_ID,

 	GPIO_PROD_LCD_BL_EN_ID,
 	GPIO_PROD_LCD_BL_BRIGHTNESS_ID,
 	GPIO_PROD_LCD_RST_ID,
 	GPIO_PROD_KEYPAD_BL_ID,
 	GPIO_PROD_CHARGE_PLUG_DETECT_ID,
#ifdef BATTERY_DETECT_SUPPORT
    GPIO_PROD_BATTERY_DETECT_ID,
#endif
 	GPIO_PROD_VIBRATIOR_EN_ID,
 	GPIO_PROD_FLASH_EN_ID,
 	GPIO_PROD_FLIP_ON_ID,

	GPIO_PROD_SLIDE_ON_ID,

 	GPIO_PROD_POWER_KEY_ID,

 	GPIO_PROD_SDCARD_DETECT_ID,
 	GPIO_PROD_SDCARD_CS_ID,
 	GPIO_PROD_SDCARD_PWR_ID,

 	GPIO_PROD_HEADSET_DETECT_ID,
 	GPIO_PROD_HEADSET_BUTTON_ID,
 	GPIO_PROD_TPXL_DETECT_ID,
 	GPIO_PROD_POWER_KEY_DETECT_ID,
 	GPIO_PROD_USB_DETECT_ID,

	GPIO_PROD_SENSOR_POWER_ID,
 	GPIO_PROD_SENSOR_PWDN_ID,
 	GPIO_PROD_SENSOR_PWDN_FRONT_ID,
 	GPIO_PROD_SENSOR_RESET_ID,
 	GPIO_PROD_SENSOR_RESET_FRONT_ID,
 	GPIO_PROD_ATV_RESET_ID,
 	GPIO_PROD_ATV_POWER_ID,
 	GPIO_PROD_ATV_LDO_ID,
 	GPIO_PROD_DC_FLASH_ID,

 	GPIO_PROD_SPEAKER_PA_EN_ID,
 	GPIO_PROD_CODEC_EN_ID,
 	GPIO_PROD_HEADSET_PA_EN_ID,
 	GPIO_PROD_SHARED_PA_SW_ID,

 	GPIO_PROD_BT_POWER_EN_ID,
 	GPIO_PROD_BT_RESET_ID,

 	GPIO_PROD_FM_POWER_EN_ID,
 	GPIO_PROD_FM_RESET_ID,

 	GPIO_PROD_GPS_POWER_EN_ID,
 	GPIO_PROD_GPS_RESET_ID,
 	GPIO_PROD_GPS_BOOT_MODE_ID,

 	GPIO_PROD_CAMERA_COVER_DETECT_ID,
 	GPIO_PROD_DKEY_1_ACTION_DETECT_ID,
 	GPIO_PROD_DKEY_2_ACTION_DETECT_ID,
 	GPIO_PROD_LCD_FMARK_DETECT_ID,

 	GPIO_PROD_AF_VCM_CTL_ID,

 	GPIO_PROD_TV_MIC_SW_ID,

 	GPIO_PROD_BACKEND_IC0_INT_ID,
 	GPIO_PROD_BACKEND_IC2_INT_ID,

 	GPIO_PROD_EXTEND_GPIO_INT_ID,

 	GPIO_PROD_SPI2_EXT_CS_EN_ID,
 	GPIO_PROD_SPI2_EXT_CS_SEL0_ID,
 	GPIO_PROD_SPI2_EXT_CS_SEL1_ID,
 	GPIO_PROD_SPI2_EXT_CS_SEL2_ID,

	GPIO_PROD_MTV_RESET_ID,
	GPIO_PROD_MTV_STANDBY_ID,
	GPIO_PROD_MTV_LCDBYPASS_ID,
	GPIO_PROD_MTV_INT_OUT_ID,
	GPIO_PROD_MTV_SOFTSWITCH_ID,
	GPIO_PROD_DEMOD_POWER,
	GPIO_PROD_DEMOD_INT,
	GPIO_PROD_DEMOD_RESET,
	GPIO_PROD_BT_REQ_CLK_ID,

	GPIO_PROD_GSENSOR_INT_ID,

 	GPIO_PROD_CUS_1_DETECT_ID,
 	GPIO_PROD_CUS_2_DETECT_ID,
 	GPIO_PROD_CUS_3_DETECT_ID,
 	GPIO_PROD_CUS_4_DETECT_ID,

   	GPIO_PROD_EXPAND_KEY0_ID,
 	GPIO_PROD_EXPAND_KEY1_ID,
 	GPIO_PROD_EXPAND_KEY2_ID,
 	GPIO_PROD_EXPAND_KEY3_ID,
 	GPIO_PROD_EXPAND_KEY4_ID,

	GPIO_PROD_WIFI_POWER_ID,        //  single power control pin
	GPIO_PROD_WIFI_PWD_ID,
	GPIO_PROD_WIFI_RESET_ID,
	GPIO_PROD_WIFI_INT_ID,          //  interrupte

	GPIO_PROD_SIM_PLUG_IN_ID,//SIM card hot swap

	GPIO_PROD_HOOK_ID,
	GPIO_PROD_HANDFREE_ID,

	GPIO_PROD_MICRO_USB_INT_ID,	   //Interrupt from Micro USB

#ifdef GPIO_SIMULATE_SPI_SUPPORT
	GPIO_PROD_VIR_SPI0_CLK_ID,
	GPIO_PROD_VIR_SPI0_CS_ID,
	GPIO_PROD_VIR_SPI0_DIN_ID,
	GPIO_PROD_VIR_SPI0_DOUT_ID,
	GPIO_PROD_VIR_SPI1_CLK_ID,
	GPIO_PROD_VIR_SPI1_CS_ID,
	GPIO_PROD_VIR_SPI1_DIN_ID,
	GPIO_PROD_VIR_SPI1_DOUT_ID,
#endif

#ifdef DUAL_BATTERY_SUPPORT
	GPIO_PROD_DBAT_MBAT_INT_ID,
	GPIO_PROD_DBAT_ABAT_INT_ID,
#endif //DUAL_BATTERY_SUPPORT

    GPIO_PROD_TP_INT_ID,
    GPIO_PROD_TP_WAKE_ID,

#if defined(MOTION_SENSOR_TYPE_TS1003)
    GPIO_PROD_GSENSOR_E1_ID,
    GPIO_PROD_GSENSOR_E2_ID,
#endif

#if defined(MOTION_SENSOR_TYPE_OPTICAL_SCM013)
    GPIO_PROD_PROXSENSOR_TX_ID,
    GPIO_PROD_PROXSENSOR_RX_ID,
#endif

    GPIO_PROD_VOLUP_KEY_ID,
    GPIO_USB_RESUME_ID,

#if defined(MOTION_SENSOR_TYPE_OFN_TRULY_A320)
    GPIO_PROD_OFN_INT_ID,   //OFN interrupt
#endif

#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
    GPIO_PROD_SPISWICTH_ID,	       //Switch for CMMB/WIFI share SPI
#endif

    GPIO_PROD_I2C_SDA1, // sw simulation i2c controller1, gpio logic pin
    GPIO_PROD_I2C_SCL1,
    GPIO_PROD_I2C_SDA2,
    GPIO_PROD_I2C_SCL2,
#ifdef MODEM_PLATFORM
    GPIO_PROD_AP_WAKEUP_BB_ID,
    GPIO_PROD_BB_WAKEUP_AP_ID,
    GPIO_PROD_BB_ASSERT_ID,
#endif

#ifdef PLATFORM_UWS6121E
    GPIO_PROD_CALIB_SEL_ID,
    GPIO_PROD_2720_INT_ID,
#endif

#ifdef ZDT_NFC_SUPPORT
    GPIO_PROD_NFC_LDO_PVDD_EN_ID,
    GPIO_PROD_NFC_LDO_SIM_EN_ID,
    GPIO_PROD_NFC_INT_ID,
    GPIO_PROD_NFC_DWL_ID,
    GPIO_PROD_NFC_EN_ID,
#endif
#ifdef ZDT_PCBA_ZW201_SUPPORT
    GPIO_PROD_EXTRSTN_KEY_ID,
    GPIO_PROD_WIFI_BT_RF_SWITCH_ID,
    GPIO_PROD_GSENSOR_INT_2_ID,
    GPIO_PROD_HALL_INT_ID,
    GPIO_PROD_CAMERA_SWITCH_ID,
    GPIO_PROD_CAMERA_SUB_PWDN_ID,
    GPIO_PROD_CAMERA_SUB_RST_ID,
    GPIO_PROD_TW_PWD_ID,
    GPIO_PROD_TW_DIO_ID,
    GPIO_PROD_HSENSOR_INT_ID,
    GPIO_PROD_HSENSOR_PDN_ID,
    GPIO_PROD_HSENSOR_POWER_ID,
    GPIO_PROD_LED_1,
    GPIO_PROD_LED_2,
    GPIO_PROD_LED_R,
    GPIO_PROD_LED_G,
    GPIO_PROD_LED_B,
    GPIO_PROD_RFID_EN_ID,
    GPIO_PROD_GPS_LNA_EN_ID,
#endif

#if defined(PRODUCT_CONFIG_uis8910c_demophone ) || defined(PRODUCT_CONFIG_uis8910c_mpphone) || defined(PRODUCT_CONFIG_uis8910ff_mpphone)  \
	|| defined(PRODUCT_CONFIG_uis8910ff_zxf)
	GPIO_PROD_FMLNA_ID,
#endif
 	GPIO_PROD_ID_MAX
 }GPIO_PROD_ID_E;

 typedef enum
 {
 	GPIO_PROD_TYPE_BB0 = 0x00,	// from Baseband
 	GPIO_PROD_TYPE_EXT,			// from Extend Gpio ic
	GPIO_PROD_TYPE_EIC_DBNC,

 	GPIO_PROD_TYPE_MAX

 }GPIO_PROD_TYPE_E;

#ifdef CMMB_WIFI_SHARE_SPI_SUPPORT
 typedef enum
 {
 	GPIO_SPI_SW_WIFI = 0x00,	// spi switch wifi
 	GPIO_SPI_SW_CMMB,			// spi switch cmmb

 	GPIO_SPI_SW_MAX
 }GPIO_SPI_SW_E;
#endif

 typedef uint32 (*GPIO_CB)(BOOLEAN is_on);

 typedef struct
 {
 	GPIO_PROD_ID_E 		gpio_id;		// gpio virtual id
	GPIO_PROD_TYPE_E	gpio_type;		// gpio type
	uint16				valid_level;	// 1: High level valid; 0: low level valid
	uint16				gpio_num;		// gpio real number
	GPIO_CB				gpio_cb_fun;	// call back function

 }GPIO_CFG_INFO_T, *GPIO_CFG_INFO_T_PTR;


/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to initialize customer configure table
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPIO_CFG_INFO_T_PTR GPIO_CFG_GetCusTable(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of gpio_prod_cfg.h

#endif // _GPIO_PROD_CFG_
