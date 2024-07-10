/******************************************************************************
 ** File Name:      gpio_cfg.c                                                *
 ** DATE:           2011.01.12                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 **---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "gpio_drv.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "gpio_ext_drv.h"
#include "analog_drv.h"
#include "tpc_drv.h"
#include "chip.h"
#include "wifi_drv.h"
#include "msensor_drv.h"

#define DEBUG_GPIO_PROD_CFG
#ifdef  DEBUG_GPIO_PROD_CFG



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
#define HEADSET_DETECT_SHAKING_TIME     200
#define HEADSET_BUTTON_SHAKING_TIME     100
#define SDCARD_DETECT_SHAKING_TIME        300
#define TP_XL_DETECT_SHAKING_TIME        20

LOCAL uint32 _GPIO_OpenLCMBackLight(BOOLEAN is_open);
LOCAL uint32 _GPIO_OpenKeyPadBackLight( BOOLEAN is_open );
LOCAL uint32 _GPIO_OpenVibrator( BOOLEAN is_open );
LOCAL uint32 _GPIO_OpenSDPower( BOOLEAN is_open );
LOCAL uint32 _GPIO_IsUsbOrAdapter( BOOLEAN is_open );
LOCAL uint32 _GPIO_OpenPA(BOOLEAN is_open);
LOCAL uint32 _GPIO_OpenFlashLight(BOOLEAN is_open);


/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
// GPIO product configure table for customer setting
#define EIC(x) x

LOCAL const GPIO_CFG_INFO_T s_gpio_prod_cfg_cus_table[] =
{
/*    ID                                    TYPE                VALID_LEVEL                NUM.            CALLBACK    */

    {GPIO_PROD_LCD_BL_EN_ID,            GPIO_PROD_TYPE_MAX, GPIO_PROD_HIGH_LEVEL,      GPIO_PROD_NUM_INVALID,    _GPIO_OpenLCMBackLight},
       {GPIO_PROD_KEYPAD_BL_ID,            GPIO_PROD_TYPE_MAX, GPIO_PROD_HIGH_LEVEL,      GPIO_PROD_NUM_INVALID,    _GPIO_OpenKeyPadBackLight},
    {GPIO_PROD_VIBRATIOR_EN_ID,            GPIO_PROD_TYPE_MAX, GPIO_PROD_HIGH_LEVEL,      GPIO_PROD_NUM_INVALID,    _GPIO_OpenVibrator},
    #if defined(ZDT_PCBA_ZW201_SUPPORT) // ZW201 W18 LED TORCH 手电筒GPIO0 //GPIO 0 LED R EN
    {GPIO_PROD_FLASH_EN_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,    0 ,       PNULL},
    #else
    {GPIO_PROD_FLASH_EN_ID,                GPIO_PROD_TYPE_MAX, GPIO_PROD_HIGH_LEVEL,      GPIO_PROD_NUM_INVALID,    _GPIO_OpenFlashLight},
    #endif

    {GPIO_PROD_HEADSET_BUTTON_ID,        GPIO_PROD_TYPE_EIC_DBNC, GPIO_PROD_HIGH_LEVEL,     EICA_AUD_HEAD_BUTTON,  PNULL },
       //{GPIO_PROD_HEADSET_DETECT_ID,        GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,     69,             PNULL   }, // GPIO_69
       {GPIO_PROD_HEADSET_DETECT_ID,        GPIO_PROD_TYPE_EIC_DBNC, GPIO_PROD_HIGH_LEVEL,     EIC_AUD_HEAD_INSERT_ALL,             PNULL   }, // GPIO_69

    //{GPIO_PROD_SENSOR_RESET_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     53,               PNULL    },
     //{GPIO_PROD_SENSOR_PWDN_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     52,                PNULL    },
     {GPIO_PROD_SENSOR_POWER_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     GPIO_PROD_NUM_INVALID,                PNULL    },
     //{GPIO_PROD_ATV_RESET_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     18,                PNULL    },
     //{GPIO_PROD_ATV_POWER_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     3,                PNULL    },
     //{GPIO_PROD_ATV_LDO_ID,                GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     24,                PNULL    },
     //{GPIO_PROD_SENSOR_PWDN_FRONT_ID,    GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     2,                PNULL    },
    {GPIO_PROD_CHARGE_PLUG_DETECT_ID,    GPIO_PROD_TYPE_EIC_DBNC, GPIO_PROD_HIGH_LEVEL,     EICA_CHGR_INT,                PNULL      },

    {GPIO_PROD_POWER_KEY_ID,            GPIO_PROD_TYPE_EIC_DBNC, GPIO_PROD_HIGH_LEVEL,     EICA_PBINT,                PNULL   },
#ifdef BATTERY_DETECT_SUPPORT
    {GPIO_PROD_BATTERY_DETECT_ID,        GPIO_PROD_TYPE_EIC_DBNC, GPIO_PROD_HIGH_LEVEL,     EICA_BATDET_OK,                PNULL      },
#endif


#ifdef ZDT_PCBA_ZW201_SUPPORT
    {GPIO_PROD_SPEAKER_PA_EN_ID,        GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     18,                               PNULL                   }, // use GPIO
#else
#ifdef AUDIO_EXTPA_TYPE_GPIO
    {GPIO_PROD_SPEAKER_PA_EN_ID,        GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     39,                               PNULL                   }, // use GPIO
#else
    {GPIO_PROD_SPEAKER_PA_EN_ID,        GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     GPIO_PROD_NUM_INVALID,            PNULL                   },// unused
#endif
    //{GPIO_PROD_SPEAKER_PA_EN_ID,        GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     GPIO_PROD_NUM_INVALID,            (GPIO_CB)_GPIO_OpenPA   },// GPIO_PROD_NUM_INVALID:do not use Extternal PA, CALLBACK = PNULL: do not use Internal PA
#endif


    {GPIO_PROD_CODEC_EN_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,    11 ,       PNULL},
     {GPIO_PROD_BT_RESET_ID,                GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     GPIO_PROD_NUM_INVALID,             PNULL    },
#ifdef MOTION_SENSOR_TYPE
     {GPIO_PROD_GSENSOR_INT_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,     8,             (GPIO_CB)MSensor_IntHandler    },    //G-SENSOR INT1 USE GPIO8
#endif

#ifdef SIM_PLUG_IN_SUPPORT
    {GPIO_PROD_SIM_PLUG_IN_ID,          GPIO_PROD_TYPE_BB0,    GPIO_PROD_HIGH_LEVEL,    4,  PNULL},
#endif

#ifdef WIFI_SUPPORT
#ifndef WIFI_SUPPORT_UNISOC_RX
    //{GPIO_PROD_WIFI_POWER_ID,           GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,   1 ,       PNULL},
    //{GPIO_PROD_WIFI_PWD_ID,             GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,    29 ,       PNULL},
    //{GPIO_PROD_WIFI_INT_ID,             GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,    47,      (GPIO_CB)WIFI_IrqCallback},
#endif
#endif

    {GPIO_PROD_USB_DETECT_ID,             GPIO_PROD_TYPE_MAX, GPIO_PROD_HIGH_LEVEL,   GPIO_PROD_NUM_INVALID, _GPIO_IsUsbOrAdapter },

    {GPIO_PROD_2720_INT_ID,             GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,    6 ,       PNULL},
    {GPIO_PROD_CALIB_SEL_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,    13 ,       PNULL},

    {GPIO_PROD_TEST_ID,GPIO_PROD_TYPE_BB0,GPIO_PROD_LOW_LEVEL,29,PNULL},
#ifdef FM_LNA_SUPPORT
    //{GPIO_PROD_FMLNA_ID,            GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL,     36,               PNULL    },
#endif

    //{GPIO_PROD_SDCARD_DETECT_ID,        GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL,     97,             PNULL    },
#ifdef LOCK_KEY_SUPPORT
    //{GPIO_PROD_LOCKKEY_INT_ID,          GPIO_PROD_TYPE_BB0,    GPIO_PROD_HIGH_LEVEL,    4,  PNULL},
#endif

#ifdef ZDT_PCBA_ZW201_SUPPORT
     {GPIO_PROD_TP_INT_ID, GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL, 12, PNULL},//macrosky
     {GPIO_PROD_TP_WAKE_ID, GPIO_PROD_TYPE_BB0, GPIO_PROD_HIGH_LEVEL, 43, PNULL},
#else
     {GPIO_PROD_TP_INT_ID, GPIO_PROD_TYPE_BB0, GPIO_PROD_LOW_LEVEL, 42, PNULL},
#endif

      // End flag, must be here !!!!!
     {GPIO_PROD_ID_MAX,                     GPIO_PROD_TYPE_MAX, GPIO_PROD_HIGH_LEVEL,   GPIO_PROD_NUM_INVALID, PNULL},
};

/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
LOCAL uint32 _GPIO_OpenLCMBackLight(BOOLEAN is_open)
{
    if(is_open)
    {
        // Open lcm backlight
        ANA_SetDevValule(ANA_DEV_ID_LCM_BL, ANA_DEV_OPEN);
    }
    else
    {
        ANA_SetDevValule(ANA_DEV_ID_LCM_BL, ANA_DEV_CLOSE);
    }
    return 0;
}

LOCAL uint32 _GPIO_OpenKeyPadBackLight(BOOLEAN is_open)
{
    if(is_open)
    {
        // Set keypad backlight current to 1st level
        ANA_SetDevValule(ANA_DEV_ID_KPD_BL, ANA_DEV_VAL_HIGH);
    }
    else
    {
        ANA_SetDevValule(ANA_DEV_ID_KPD_BL, ANA_DEV_VAL_MIN);
    }
    return 0;
}

LOCAL uint32 _GPIO_OpenVibrator(BOOLEAN is_open)
{
    if(is_open)
    {
        // Set vibrator current to 1st level
        ANA_SetDevValule(ANA_DEV_ID_VIBRATOR, ANA_DEV_VAL_DEF);
    }
    else
    {
        ANA_SetDevValule(ANA_DEV_ID_VIBRATOR, ANA_DEV_VAL_MIN);
    }
    return 0;
}

LOCAL uint32 _GPIO_OpenSDPower(BOOLEAN is_open)
{
    if(SC6600L == CHIP_GetChipType())
    {
        //for SC6600L1 chip, compatibly boards controlled by different sd power
        is_open = !is_open;

        if(is_open)
        {
            ANA_SetDevValule(ANA_DEV_ID_SD_PWR, ANA_DEV_OPEN);
           }
        else
        {
            ANA_SetDevValule(ANA_DEV_ID_SD_PWR, ANA_DEV_CLOSE);
        }
    }
    return 0;
}

LOCAL uint32 _GPIO_OpenFlashLight(BOOLEAN is_open)
{
    if(is_open)
    {
        // Open Flash Light(White led IB4/5)
        ANA_SetDevValule(ANA_DEV_ID_FLASH_LIGHT, ANA_DEV_OPEN);
    }
    else
    {
        // Close Flash Light
        ANA_SetDevValule(ANA_DEV_ID_FLASH_LIGHT, ANA_DEV_CLOSE);
    }
    return 0;
}

LOCAL uint32 _GPIO_IsUsbOrAdapter( BOOLEAN is_open )
{
    BOOLEAN ret_val = FALSE;

    if(CHIP_GetUsbDMValue())
    {
        ret_val = TRUE;
    }
    return ret_val;
}


//PUBLIC uint32   AUDIO_PA_Ctl(uint32 id,uint32 is_open);

LOCAL uint32 _GPIO_OpenPA(BOOLEAN is_open)
{
//    return AUDIO_PA_Ctl(0,is_open);
    return 0;
}


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to initialize customer configure table
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPIO_CFG_INFO_T_PTR GPIO_CFG_GetCusTable(void)
{
    return (GPIO_CFG_INFO_T_PTR)s_gpio_prod_cfg_cus_table;
}

/*****************************************************************************/
//  Description:    This function is used to initialize GPIO  about customer setting
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_CustomizeInit(void)
{

    //SCI_TRACE_LOW:"GPIO_CustomizeInit() Start !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_CFG_192_112_2_18_0_26_25_34,(uint8*)"");

    // Init Product GPIO configure
    GPIO_PROD_InitCfgTable();

    //gpio0-6 detect
    GPIO_PROD_RegGpio(
        GPIO_PROD_2720_INT_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_FALSE,
        HEADSET_DETECT_SHAKING_TIME,
        (GPIO_PROD_CALLBACK)GPIO_R2720IntHandler
        );

    /* GPIO_PROD_2720_INT_ID 的真实GPIO是6， 设置LISR 使得GPIO_R2720IntHandler 在低级中断中处理 */
    GPIO_SetLisrInt(6, TRUE);


    // Charge plug detect
    GPIO_PROD_RegGpio(
        GPIO_PROD_CHARGE_PLUG_DETECT_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_TRUE,
        GPIO_DEFAULT_SHAKING_TIME*4,
        (GPIO_PROD_CALLBACK)GPIO_ChargeIntHandler
        );

#ifdef SIM_PLUG_IN_SUPPORT
    GPIO_PROD_RegGpio(
        GPIO_PROD_SIM_PLUG_IN_ID,
        SCI_FALSE,
        SCI_TRUE,
        SCI_TRUE,
        GPIO_DEFAULT_SHAKING_TIME*4,
        (GPIO_PROD_CALLBACK)GPIO_SIMIntHandler
    );
#endif
#if 0
#ifdef BATTERY_DETECT_SUPPORT
    // Battery detect
    GPIO_PROD_RegGpio(
        GPIO_PROD_BATTERY_DETECT_ID,
        SCI_FALSE,
        SCI_TRUE,
        SCI_FALSE,
        0,
        (GPIO_PROD_CALLBACK)GPIO_BatDetIntHandler
        );
#endif
#endif
    // handle power key interrupt
    GPIO_PROD_RegGpio(
        GPIO_PROD_POWER_KEY_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_TRUE,
        GPIO_DEFAULT_SHAKING_TIME,
        (GPIO_PROD_CALLBACK)GPIO_PowerKeyIntHandler
        );
#ifdef LOCK_KEY_SUPPORT
    GPIO_PROD_RegGpio(
        GPIO_PROD_LOCKKEY_INT_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_TRUE,
        100,
        (GPIO_PROD_CALLBACK)GPIO_LockKey_IntHandler
    );

#endif

#ifdef MOTION_SENSOR_TYPE
    // handle g-sensor interrupt
    GPIO_PROD_RegGpio(
        GPIO_PROD_GSENSOR_INT_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_FALSE,
        0,
        (GPIO_PROD_CALLBACK)GPIO_GSensorIntHandler
        );
#endif

#if 0

#ifndef WIFI_SUPPORT_NONE
        // WLAN rx/tx detect
    GPIO_PROD_RegGpio(
        GPIO_PROD_WIFI_INT_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_FALSE,
        0,
        (GPIO_PROD_CALLBACK)GPIO_WiFiIntHander
        );
#endif


    // SDCARD plug detect
    /*GPIO_PROD_RegGpio(
        GPIO_PROD_SDCARD_DETECT_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_TRUE,
        SDCARD_DETECT_SHAKING_TIME,
        (GPIO_PROD_CALLBACK)GPIO_SdcardDetectIntHandler
        );*/

    GPIO_EXT_Init();

    //Defalut IB4/5 is controlled by HW means it is used for LCM Backlight,
    //If you want to use IB4/5 for flash light , the need to select IB4/5 control by software
    ANA_SelectDevFunc(ANA_DEV_ID_LCM_BL, ANA_LCM_BL_IB4_IB5_CLOSE);

    //SCI_TRACE_LOW:"GPIO_CustomizeInit() End !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_CFG_265_112_2_18_0_26_25_35,(uint8*)"");
#endif
	 GPIO_PROD_RegGpio(
        GPIO_PROD_TEST_ID,
        SCI_FALSE,
        SCI_FALSE,
        SCI_FALSE,
        HEADSET_DETECT_SHAKING_TIME,
        (GPIO_PROD_CALLBACK)GPIO_TEST_IRQ
        );
}

PUBLIC void GPIO_SensorIdGet(uint8 *pid_value)
{
    //step1:get id
    pid_value[0] =0xFF;
    pid_value[1] =0xFF;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // end of gpio_cfg.c
#endif  // End of DEBUG_GPIO_CFG

